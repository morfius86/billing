<?php
//
// API CLASS SIMPLE-BILLING TO SNMP
// version: 1.0.0
//

Class NETWORK_MONITOR extends API{
	public $snmp_reply = FALSE;
	private $snmp_ip;
	private $snmp_comm;
	private $snmp_walk;
	
    public function __construct()
    {
		parent::__construct();
    }
	/* PING */
	public function ping($host, $timeout = 1)
	{
		$package = "\x08\x00\x7d\x4b\x00\x00\x00\x00PingHost";
		$socket  = socket_create(AF_INET, SOCK_RAW, 1);
		socket_set_option($socket, SOL_SOCKET, SO_RCVTIMEO, array('sec' => $timeout, 'usec' => 1));
		socket_connect($socket, $host, null);

		$ts = microtime(true);
		socket_send($socket, $package, strLen($package), 0);
		if (socket_read($socket, 255)){
			$result = microtime(true) - $ts;
		}else{
			$result = false;
		}
		socket_close($socket);
		return $result;
	}
	/* SNMP */
	public function SetSnmp($snmp_ip, $snmp_comm, $snmp_walk = 'snmpbulkwalk')
    {
		$this->snmp_ip = $snmp_ip;
		$this->snmp_comm = $snmp_comm;
		$this->snmp_walk = $snmp_walk;
    }
	
	public function snmpwalk($snmp_mib, $vlan_fdb=false)
	{
		if (!empty($this->snmp_ip) or !empty($this->snmp_comm) or !empty($snmp_mib)){
			$handle = proc_open("".$this->snmp_walk." -c ".$this->snmp_comm."".(($vlan_fdb) ? '@'.$vlan_fdb.'':'')." -v 2c -Cc ".$this->snmp_ip." ".$snmp_mib."", array(0 => array("pipe", "r"), 1 => array("pipe", "w"), 2 => array("pipe", "a")), $pipes);		
			$get_mib=explode("\n", stream_get_contents($pipes[1]));
			if (count($get_mib)!=1 && strlen($get_mib[0])!=0){
				$this->snmp_reply=$get_mib;
				#update lastconnect time
				parent::Query("UPDATE billing_switch SET switch_snmp_lasttime = '".date("Y-m-d H:i:s")."' WHERE switch_ip = '".$this->snmp_ip."'");
			}else{
				$this->snmp_reply=FALSE;
				parent::APICreatEvent("snmp", "", "", "", "-1", "Ошибка получения данных с ".$this->snmp_ip."");
			}
			$retval = proc_close($handle);
		}else{
			$this->snmp_reply=FALSE;
		}
		return $this->snmp_reply;
	}
	
	public function get_snmpwalk_fdb ($type_switch, $uplink, $port="\d+", $mac=".*")
	{	
		$result = array();
		if ($this->snmp_reply != FALSE){
			$snmp_reply = $this->snmp_reply;
			// Dlink
			if ($type_switch>=1 && $type_switch<=99){
				foreach ($snmp_reply as $get_mib_val){
					#example: SNMPv2-SMI::mib-2.17.7.1.2.2.1.2.25.244.109.4.155.123.69 = INTEGER: 10
					(preg_match("/1.2.2.1.2.\d+.(".($mac!=".*" ? $this->mac10_mac16($mac) : $mac).") = INTEGER: (".$port.")/", $get_mib_val, $matches) && !in_array($matches[2], explode(",", $uplink.",0")) ? $result[]=$matches : '');
				}
			// Cisco (отдает реальные номера портов, а не index порта)
			}elseif ($type_switch>=100 && $type_switch<=150){
				$get_type_switch=parent::APIGetSwitchType($type_switch);
				// получаем index портов
				$this->snmpwalk($get_type_switch[1]["ifindex"]);
				$ifindex = $this->get_snmpwalk_ifindex($type_switch);
				
				foreach ($snmp_reply as $get_mib_val){
					#получили список mac длЯ default vlan (Cisco отдает маки только по default vlan)
					#example: SNMPv2-SMI::mib-2.17.4.3.1.2.0.38.24.70.98.231 = INTEGER: 8
					if (preg_match("/4.3.1.2.(".($mac!=".*" ? $this->mac10_mac16($mac) : $mac).") = INTEGER: (".$port.")/", $get_mib_val, $matches) && !in_array($matches[2], explode(",", $uplink.",0"))){
						$matches[2] = $ifindex[$matches[2]];	// заменяем номера портов индексами
						$result[]=$matches;
					} 
				}
				// Дополучим список vlan (кроме влан 1 и стандартных)
				$this->snmpwalk(".1.3.6.1.4.1.9.9.46.1.3.1.1.2");
				$get_vlan=$this->get_snmpwalk_vlan();
				
				// Дополнительно опрашиваем каждый vlan отдельно
				if (count($get_vlan)>0){
					foreach ($get_vlan as $vlan_id){
						$this->snmpwalk($get_type_switch[1]["switch_mibfdb"], $vlan_id);
						if ($this->snmp_reply != FALSE){
							foreach ($this->snmp_reply as $get_mib_val){
								if (preg_match("/4.3.1.2.(".($mac!=".*" ? $this->mac10_mac16($mac) : $mac).") = INTEGER: (".$port.")/", $get_mib_val, $matches) && !in_array($matches[2], explode(",", $uplink.",0"))){
									$matches[2] = $ifindex[$matches[2]];	// заменяем номера портов индексами
									$result[]=$matches;
								} 
							}
						}
					}
				}	
			//bdcom
			}elseif ($type_switch>=151 && $type_switch<=200){
				foreach ($snmp_reply as $get_mib_val){
					#SNMPv2-SMI::enterprises.3320.152.1.1.1.76.50.152.222.208.53.210.235 = INTEGER: 76
					(preg_match("/3320.152.1.1.1.\d+.\d+.(".($mac!=".*" ? $this->mac10_mac16($mac) : $mac).") = INTEGER: (".$port.")/", $get_mib_val, $matches) && !in_array($matches[2], explode(",", $uplink.",0")) ? $result[]=$matches : '');
				}
			}
		}else{
			return FALSE;
		} 
		return $result;
	}
	// состояние портов 
	public function get_snmpwalk_ports ($type_switch, $port="\d+")
	{	
		$result = array();
		if ($this->snmp_reply != FALSE){
			$get_snmp=$this->snmp_reply;
			// dlink
			// такой хуйней не занимается
			if ($type_switch>=1 && $type_switch<=99){
				foreach ($get_snmp as $get_mib_val){
					#example: IF-MIB::ifOperStatus.1 = INTEGER: up(1)
					(preg_match("/Status.(".$port.") = INTEGER: (.*)\(/", $get_mib_val, $matches) ? $result[$matches[1]]=$matches[2] : '');
				}
			// cisco 
			// отдает состояние портов с ключем порта, а не реальным номером
			}elseif ($type_switch>=100 && $type_switch<=150){
				#example: IF-MIB::ifOperStatus.1 = INTEGER: up(1)
				foreach ($get_snmp as $get_mib_val){
					#example: IF-MIB::ifOperStatus.1 = INTEGER: up(1)
					if(preg_match("/Status.(".$port.") = INTEGER: (.*)\(/", $get_mib_val, $matches)){
						$result[$matches[1]]=$matches[2];
					}
				}
			}elseif ($type_switch>=151 && $type_switch<=200){
				#example: IF-MIB::ifOperStatus.1 = INTEGER: up(1)
				foreach ($get_snmp as $get_mib_val){
					#example: IF-MIB::ifOperStatus.1 = INTEGER: up(1)
					if(preg_match("/Status.(".$port.") = INTEGER: (.*)\(/", $get_mib_val, $matches)){
						$result[$matches[1]]=$matches[2];
					}
				}
			}
		}else{
			return FALSE;
		} 
		return $result;
	}
	
	// нумерация портов, соответствие ключа (index) порта его реальному номеру 
	public function get_snmpwalk_ifdescr ($type_switch, $port="\d+")
	{	
		$result = array();
		if ($this->snmp_reply != FALSE){
			//print_r($this->snmp_reply);
			foreach ($this->snmp_reply as $get_mib_val){
				#dlink
				if ($type_switch>=1 && $type_switch<=99){
					// IF-MIB::ifDescr.11 = STRING: D-Link DES-1228/ME R2.00 Port 11
					// IF-MIB::ifDescr.1 = STRING: RMON Port  1 on Unit 1
					if (preg_match("/Descr.(".$port.") = STRING: .*(Port\s+\d+)/", $get_mib_val, $matches)) {
						$result[$matches[1]]=$matches[2];
					}elseif (preg_match("/Descr.(".$port.") = STRING: Ethernet Interface/", $get_mib_val, $matches)) {
						$result[$matches[1]] = "Port ".$matches[1];
					}
					// IF-MIB::ifDescr.10 = STRING: Ethernet Interface

				#cisco
				}elseif ($type_switch>=100 && $type_switch<=150){
					// IF-MIB::ifDescr.48 = STRING: FastEthernet0/1
					if (preg_match("/Descr.(".$port.") = STRING: (.*)/", $get_mib_val, $matches)){
						$result[$matches[1]]=$matches[2];
					} 
				}elseif ($type_switch>=151 && $type_switch<=200){
					// IF-MIB::ifDescr.48 = STRING: FastEthernet0/1
					if (preg_match("/Descr.(".$port.") = STRING: (.*)/", $get_mib_val, $matches)){
						$result[$matches[1]]=$matches[2];
					} 
				}
			}
		}else{
			return FALSE;
		} 
		return $result;
	}
	
	public function get_snmpwalk_ifindex ($type_switch, $port="\d+")
	{	
		$result = array();
		if ($this->snmp_reply != FALSE){
			$snmp_reply = $this->snmp_reply;
			// Dlink | #SNMPv2-SMI::mib-2.17.1.4.1.2.23 = INTEGER: 19
			if ($type_switch>=1 && $type_switch<=99){		
				foreach ($snmp_reply as $get_mib_val){				
					(preg_match("/.(".$port.") = INTEGER: (\d+)/", $get_mib_val, $matches) ? $result[$matches[1]]=$matches[2] : '');
				}
			// Cisco | #SNMPv2-SMI::mib-2.17.1.4.1.2.23 = INTEGER: 19	
			// Чтобы получить port-index, нужно опросить! каждый vlan. По-умолчанию отдается vlan 1
			}elseif ($type_switch>=100 && $type_switch<=150){				
				foreach ($snmp_reply as $get_mib_val){				
					(preg_match("/.(".$port.") = INTEGER: (\d+)/", $get_mib_val, $matches) ? $result[$matches[1]]=$matches[2] : '');
				}			

				$get_type_switch=parent::APIGetSwitchType($type_switch);	// Мибы 
				$this->snmpwalk(".1.3.6.1.4.1.9.9.46.1.3.1.1.2");			// Список vlan
				$get_vlan=$this->get_snmpwalk_vlan();						// обрабатываем
				
				// Дополнительно опрашиваем каждый vlan отдельно
				if (count($get_vlan)>0){
					foreach ($get_vlan as $vlan_id){
						$this->snmpwalk($get_type_switch[1]["ifindex"], $vlan_id);
						if ($this->snmp_reply != FALSE){
							foreach ($this->snmp_reply as $get_mib_val){
								(preg_match("/.(".$port.") = INTEGER: (\d+)/", $get_mib_val, $matches) ? $result[$matches[1]]=$matches[2] : '');
							}
						}
					}
				}
			}
			
		}else{
			return FALSE;
		} 
		return $result;
	}
	
	public function get_snmpwalk_ifoctets ($type_switch, $port="\d+")
	{	
		$result = array();
		if ($this->snmp_reply != FALSE){
			foreach ($this->snmp_reply as $get_mib_val){
				# IF-MIB::ifInOctets.49 = Counter32: 226616719
				(preg_match("/Octets.(".$port.") = Counter32: (.*)/",  $get_mib_val, $matches) ? $result[$matches[1]]=$matches[2] : '');
			}
		}else{ return FALSE; } 
		return $result;
	}
	
	public function get_snmpwalk_ifspeed ($type_switch, $port="\d+")
	{	
		$result = array();
		if ($this->snmp_reply != FALSE){
			foreach ($this->snmp_reply as $get_mib_val){
				// dlink
				if ($type_switch>=1 && $type_switch<=99){
					#example: IF-MIB::ifSpeed.5 = Gauge32: 100000000					
					(preg_match("/ifSpeed.(".$port.") = Gauge32: (.*)/", $get_mib_val, $matches) ? $result[$matches[1]]=$matches[2] : '');
				// cisco
				}elseif ($type_switch>=100 && $type_switch<=150){
					#example: IF-MIB::ifSpeed.5 = Gauge32: 100000000					
					(preg_match("/ifSpeed.(".$port.") = Gauge32: (.*)/", $get_mib_val, $matches) ? $result[$matches[1]]=$matches[2] : '');
				// bdcom
				}elseif ($type_switch>=151 && $type_switch<=200){
					#example: IF-MIB::ifSpeed.5 = Gauge32: 100000000					
					(preg_match("/ifSpeed.(".$port.") = Gauge32: (.*)/", $get_mib_val, $matches) ? $result[$matches[1]]=$matches[2] : '');
				}
			}
		}else{
			return FALSE;
		} 
		return $result;
	}
	
	public function get_snmpwalk_iferror ($type_switch, $port="\d+")
	{	
		$result = array();
		if ($this->snmp_reply != FALSE){
			foreach ($this->snmp_reply as $get_mib_val){
				#example: IF-MIB::ifInErrors.32 = Counter32: 0					
				(preg_match("/if(?:In|Out)Errors.(".$port.") = Counter\d+: (.*)/", $get_mib_val, $matches) ? $result[$matches[1]]=$matches[2] : '');
			}
		}else{
			return FALSE;
		} 
		return $result;
	}

	
	public function get_snmpwalk_uptime ($type_switch)
	{	
		$result = array();
		$get_snmp=$this->snmp_reply;
		
		if ($get_snmp != FALSE){
			// общий для всех			
			(preg_match("/Timeticks: \((.*)\)/", $get_snmp[0], $matches) ? $result=$matches[1] : '');	
			
		}else{
			return FALSE;
		} 
		return $result;
	}
	
	public function get_snmpwalk_vlan ()
	{	
		$result = array();
		$novlan = array("1", "1002", "1003", "1004", "1005");
		if ($this->snmp_reply != FALSE){
			foreach ($this->snmp_reply as $get_mib_val){
				#example: SNMPv2-SMI::enterprises.9.9.46.1.3.1.1.2.1.1002 = INTEGER: 1
				if(preg_match("/.(\d+) = INTEGER: (.*)/", $get_mib_val, $matches)){
					if (!in_array($matches[1], $novlan)) {
						$result[]=$matches[1];
					}
				} 
			}
		}else{
			return FALSE;
		} 
		return $result;
	}
	
	// ---------------------- //
	//		PON FUNCTION		
	// ---------------------- //
	
	// мак-адреса ONU
	public function get_mac_onu ($type_switch, $port="\d+", $mac=".*")
	{	
		$result = array();
		if ($this->snmp_reply != FALSE){
			$snmp_reply = $this->snmp_reply;
			if ($type_switch>=151 && $type_switch<=200){
				foreach ($snmp_reply as $get_mib_val){
					#SNMPv2-SMI::enterprises.3320.101.10.1.1.3.86 = Hex-STRING: 84 79 73 23 7E 0C
					if(preg_match("/.(".$port.") = Hex-STRING: (".$mac.")/", $get_mib_val, $matches)){ 
						$result[$matches[1]]=str_replace(" ", ":", trim($matches[2]));
					}
				}
			}
		}else{
			return FALSE;
		} 
		return $result;
	}
	
	// rx-уровни ONU
	public function get_onu_rx_power ($type_switch, $port="\d+")
	{	
		$result = array();
		if ($this->snmp_reply != FALSE){
			$snmp_reply = $this->snmp_reply;
			if ($type_switch>=151 && $type_switch<=200){
				foreach ($snmp_reply as $get_mib_val){
					#SNMPv2-SMI::enterprises.3320.101.10.5.1.5.90 = INTEGER: -213
					if(preg_match("/.(".$port.") = INTEGER: (.*)/", $get_mib_val, $matches)){ 
						$result[$matches[1]]=trim($matches[2])/10;
					}
				}
			}
		}else{
			return FALSE;
		} 
		return $result;
	}
	
	// vendor ONU
	public function get_onu_vendor ($type_switch, $port="\d+")
	{	
		$result = array();
		if ($this->snmp_reply != FALSE){
			$snmp_reply = $this->snmp_reply;
			if ($type_switch>=151 && $type_switch<=200){
				foreach ($snmp_reply as $get_mib_val){
					# SNMPv2-SMI::enterprises.3320.101.10.1.1.1.90 = STRING: "FORA"
					if(preg_match("/.(".$port.") = STRING: \"(.*)\"/", $get_mib_val, $matches)){ 
						$result[$matches[1]]=trim($matches[2]);
					}
				}
			}
		}else{
			return FALSE;
		} 
		return $result;
	}
	
	// определяем вендора по мак
	public function get_mac_vendor ($mac)
	{	
		$vendor = array (
			'C0:7E' => 'EXLNK'
		);

		if(preg_match("/(?<octet>\w+[:]\w+)/i", $mac, $matches)){ 
		
			if (isset ($vendor[$matches["octet"]])) {
				return $vendor[$matches["octet"]];
			}
		}
	}
	
	public function mac10_mac16($mac)
	{
		$mac_hex = '';
		$mac_arr =array();
		if (strlen($mac) == "17") { 
			$mac_arr=explode(":", $mac);
			foreach ($mac_arr as $k => $mac_val){
				if (isset($mac_arr[$k+1])){
					$mac_hex.=hexdec($mac_val).".";
				}else{
					$mac_hex.=hexdec($mac_val);
				}
			}
		}else{
			return false;
		}
		return $mac_hex;
	}
	
	public function mac16_mac10($mac)
	{	
		$mac_str='';
		if (substr_count($mac, ".")==5) { 
			foreach(explode(".", $mac) as $m){
				$mac16=dechex($m);
				$mac_str.=(strlen($mac16)==1 ? '0'.$mac16.'': $mac16);
			}
		}else{
			return false;
		}
		return $mac_str;
	}
}
?>

