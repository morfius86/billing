#!/usr/bin/perl -w

use strict;
use vars qw(%RAD_REQUEST %RAD_REPLY %RAD_CHECK %REQUEST );
use Data::Dumper;
use DBI;

# This the remapping of return values

    use constant    RLM_MODULE_REJECT=>    0;#  /* immediately reject the request */
    use constant    RLM_MODULE_FAIL=>      1;#  /* module failed, don't reply */
    use constant    RLM_MODULE_OK=>        2;#  /* the module is OK, continue */
    use constant    RLM_MODULE_HANDLED=>   3;#  /* the module handled the request, so stop. */
    use constant    RLM_MODULE_INVALID=>   4;#  /* the module considers the request invalid. */
    use constant    RLM_MODULE_USERLOCK=>  5;#  /* reject the request (user is locked out) */
    use constant    RLM_MODULE_NOTFOUND=>  6;#  /* user not found */
    use constant    RLM_MODULE_NOOP=>      7;#  /* module succeeded without doing anything */
    use constant    RLM_MODULE_UPDATED=>   8;#  /* OK (pairs modified) */
    use constant    RLM_MODULE_NUMCODES=>  9;#  /* How many return codes there are */
	
sub ReturnRequest{
	my ($nas_type,$param) = @_;
	
	while ( my ($k, $v) = each %$param) {
		if (@$v[0] eq '1') {
			$RAD_REPLY{$k} = @$v[1];
		}
	}
}

#Mysql
sub sql_connect {
	my $dsn = 'DBI:mysql:radius:localhost';
	my $db_user_name = 'root';
	my $db_password = '123';
	my $dbh = DBI->connect_cached($dsn, $db_user_name, $db_password);
	return $dbh;
}	

sub convert_radpairs {
	%REQUEST = ();
	while(my($k, $v)=each %RAD_REQUEST) {
		$k =~ s/-/_/g;
		$k =~ tr/[a-z]/[A-Z]/;
		$REQUEST{$k}=$v;
	 }
}


sub pre_post_auth{
	my ($type, $type_auth) = @_;
	my $db = sql_connect();
	my $SqlAdd = '';
	convert_radpairs();
	
	#ищем NAS-сервер и смотрим параметры
	my $output = $db->prepare("SELECT * from `billing_nas` where `nas_ip` like '$REQUEST{NAS_IP_ADDRESS}'");
	$output->execute; 
	my $nas=$output->fetchrow_hashref;
	$output->finish;
	
	if ($nas){
		if (!defined($REQUEST{'FRAMED_IP_ADDRESS'})) {
			$REQUEST{'FRAMED_IP_ADDRESS'} =  '';
		}
		#pap or mschap
		if (defined($REQUEST{'USER_PASSWORD'})) {
			$SqlAdd = "and `user_password` like '$REQUEST{USER_PASSWORD}'";
		}else{
			$SqlAdd = "";
		}
		my %sql_add = (
			"1" => "`user_ip` like '$REQUEST{FRAMED_IP_ADDRESS}' and `user_mac` like '$REQUEST{USER_NAME}'", 
			"2" => "`user_ip` like '$REQUEST{FRAMED_IP_ADDRESS}'",
			"3" => "`user_mac` like '$REQUEST{USER_NAME}'",
			"4" => "`user_login` like '$REQUEST{USER_NAME}' $SqlAdd",
			"5" => "`user_login` like '$REQUEST{USER_NAME}' $SqlAdd and `user_ip` like '$REQUEST{CALLING_STATION_ID}'"
		);

		my $find = $db->prepare("SELECT billing_users.* FROM `billing_users`
		where $sql_add{$nas->{'nas_type'}} and `user_block` = '0' limit 1");
		$find->execute; 
		my $find_array=$find->fetchrow_hashref;
		$find->finish;
		
		#user is found or accaunting
		if ($find_array or $type eq "acct") {
		
			#pre/post/auth
			if ($type eq "pre_auth"){
				return RLM_MODULE_OK;
				
			}elsif($type eq "auth"){
					#индивидуальный тариф
					my $tarif = $db->prepare("SELECT `tarif_id`, `mik_adlist`, `mik_ippool`, `mik_ippool_name`  from `billing_tarification` where `tarif_id` like '$find_array->{tarif_id}' limit 1");
					$tarif->execute; 
					my $tarif_array=$tarif->fetchrow_hashref;
					$tarif->finish;			
					#авторизация ppp
					if ($type_auth eq "ppp") {
						my %test =(
							"Mikrotik-Address-List" => [$nas->{'nas_addresslist'}, $tarif_array->{'mik_adlist'}], 
							"Service-Type" => [1, 'Framed-User'],
							"Framed-Protocol" => [1, 'PPP'],
							"Framed-IP-Netmask" => [1, '255.255.255.255'],
							"Framed-Pool" => [$tarif_array->{'mik_ippool'}, $tarif_array->{'mik_ippool_name'}]
						);
						if ($find_array->{'user_pppip'} ne '' ){
							$test{"Framed-IP-Address"} = [1, $find_array->{'user_pppip'}];
						}
						
						$RAD_CHECK{'Cleartext-Password'} = $find_array->{'user_password'};
						ReturnRequest ($nas->{'nas_type'}, \%test);
					#другая авторизация
					}else{
						my %test =(
							'Mikrotik-Address-List' => [$nas->{'nas_addresslist'}, $tarif_array->{'mik_adlist'}]
						);
						
						ReturnRequest ($nas->{'nas_type'}, \%test);
					}
				
				#добавить в базу				
				return RLM_MODULE_OK;
			}elsif($type eq "acct"){
				#новая сессия 
				if ($REQUEST{'ACCT_STATUS_TYPE'} eq "Start") {
					$db->do("insert into billing_users_auth(`user_id`, `session_id`, `user_name`, `framed_ip_address`, `authdate_start`) values ('$find_array->{user_id}', '$REQUEST{ACCT_SESSION_ID}', '$REQUEST{USER_NAME}', '$REQUEST{FRAMED_IP_ADDRESS}', NOW())");
				#остановка сессии
				}elsif($REQUEST{'ACCT_STATUS_TYPE'} eq "Stop"){
					$db->do("update billing_users_auth SET authdate_stop = NOW(), acctinputoctets = '$REQUEST{ACCT_INPUT_OCTETS}', acctoutputoctets = '$REQUEST{ACCT_OUTPUT_OCTETS}', acctterminatecause = '$REQUEST{ACCT_TERMINATE_CAUSE}' WHERE session_id = '$REQUEST{ACCT_SESSION_ID}'");
				}
				return RLM_MODULE_OK;
			}else{
				return RLM_MODULE_REJECT;
			}
		}else{
			return RLM_MODULE_REJECT;
		}		
	}
	
	
	$db->disconnect;
}

# authorize
sub authorize {
	convert_radpairs();
	#IF MS-CHAP
	if (defined($REQUEST{'MS_CHAP_CHALLENGE'}) && defined($REQUEST{'MS_CHAP2_RESPONSE'})){
		return pre_post_auth("auth", "ppp");
	}else{
		return pre_post_auth("pre_auth", "");
	}
	
	
}

#  authenticate
sub authenticate {
	#PPP AUTH
	if (defined($REQUEST{'FRAMED_PROTOCOL'}) && $REQUEST{'FRAMED_PROTOCOL'} eq "PPP"){
		return pre_post_auth("auth", "ppp");
	}else{
		return pre_post_auth("auth", "");
	}			
}

# Function to handle preacct
sub preacct {
    # For debugging purposes only
	# &log_request_attributes;
    return RLM_MODULE_OK;
}

# Function to handle accounting
sub accounting {
	#добавить в базу	
	return pre_post_auth("acct", "");			
}

# Function to handle checksimul
sub checksimul {
    # For debugging purposes only
#       &log_request_attributes;

    return RLM_MODULE_OK;
}

# Function to handle pre_proxy
sub pre_proxy {
    # For debugging purposes only
#       &log_request_attributes;

    return RLM_MODULE_OK;
}

# Function to handle post_proxy
sub post_proxy {
    # For debugging purposes only
#       &log_request_attributes;

    return RLM_MODULE_OK;
}

# Function to handle post_auth
sub post_auth {
    # For debugging purposes only
#       &log_request_attributes;s
    return RLM_MODULE_OK;
}

# Function to handle xlat
sub xlat {
    # For debugging purposes only
#       &log_request_attributes;

    # Loads some external perl and evaluate it
    my ($filename,$a,$b,$c,$d) = @_;
    &radiusd::radlog(1, "From xlat $filename ");
    &radiusd::radlog(1,"From xlat $a $b $c $d ");
    local *FH;
    open FH, $filename or die "open '$filename' $!";
    local($/) = undef;
    my $sub = <FH>;
    close FH;
    my $eval = qq{ sub handler{ $sub;} };
    eval $eval;
    eval {main->handler;};
}

# Function to handle detach
sub detach {
    # For debugging purposes only
#       &log_request_attributes;

    # Do some logging.
    &radiusd::radlog(0,"rlm_perl::Detaching. Reloading. Done.");
}

#
# Some functions that can be called from other functions
#

sub test_call {
    # Some code goes here
}

sub log_request_attributes {
    # This shouldn't be done in production environments!
    # This is only meant for debugging!
    for (keys %RAD_REQUEST) {
            &radiusd::radlog(1, "RAD_REQUEST: $_ = $RAD_REQUEST{$_}");
    }
}