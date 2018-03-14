<?php
if (isset($_GET["data"]) && !empty($_GET["data"])) {
	$data = unserialize(base64_decode($_GET["data"])); 
	/*
	$data = array(
		array("39:25", 23),
		array("39:30", 24),
		array("39:35", 15),
		array("39:40", 10),
		array("39:45", 20),
		array("39:50", 24),
		array("39:55", 24),
		array("40:00", 24),
		array("39:50", 24),
		array("39:55", 24),
		array("40:00", 24),
		array("40:05", 24)	
	);
	*/
	require 'phplot.php';
	$plot = new PHPlot(500,170);
	# No 3-D shading of the bars:
$plot->SetShading(0);

# Turn off X tick labels and ticks because they don't apply here:

$plot->SetPlotType('linepoints');
$plot->SetDataType('text-data');
$plot->SetDataValues($data);
//$plot->SetFontTTF('generic', '', 36);
# Make a legend for the 2 functions:
$plot->SetLegend(array('D', 'U'));
//$plot->SetLegendPosition(0, 0, 'image', 0, 0, 5, 5);
$plot->SetXLabelAngle(90);
// $plot->SetYDataLabelPos('plotin');
# Turn on X data label lines (drawn from X axis up to data point):
$plot->SetDrawXDataLabelLines(True);

# With Y data labels, we don't need Y ticks, Y tick labels, or grid lines.
//$plot->SetYTickLabelPos('none');
$plot->SetYTickPos('none');
$plot->SetDrawYGrid(False);
# X tick marks are meaningless with this data:
$plot->SetXTickPos('none');
$plot->SetXTickLabelPos('none');


	
	//$plot->SetTitle('First Test Plot');
	$plot->DrawGraph();


}


?>