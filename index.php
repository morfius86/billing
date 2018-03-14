<?php include_once("auth.php"); ?>
<html>
<head>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8">
    <meta http-equiv="Content-Style-Type" content="text/css">
    <title>::Simple MT-Billing::</title>
    <script type="text/javascript" src="js/jquery.js"></script>
    <script type="text/javascript" src="js/tabs.js"></script>
    <script type="text/javascript" src="js/tablesorter.js"></script>
    <script type="text/javascript" src="js/tableedit.js"></script>
    <script type="text/javascript" src="js/column_hover.js"></script>
    <script type="text/javascript" src="js/masked.js"></script>
    <script type="text/javascript" src="js/colortip-1.0.js"></script>
	<script type="text/javascript" src="js/jquery.multiple.select.js"></script>

    <script type="text/javascript">
        $(document).ready(function () {
            $("ul.subnav").parent().append("<span></span>");
            $("ul.topnav li span").click(function () {
                $(this).parent().find("ul.subnav").slideDown('fast').show();

                $(this).parent().hover(function () {
                }, function () {
                    $(this).parent().find("ul.subnav").slideUp('slow');
                });
            }).hover(function () {
                        $(this).addClass("subhover");
                    }, function () {
                        $(this).removeClass("subhover");
                    });
            $('[title]').colorTip({color:'yellow'});

        });
    </script>
    <link href="css/default.css" rel="stylesheet" type="text/css"/>
    <link href="css/menu.css" rel="stylesheet" type="text/css"/>
    <link href="css/tabs.css" rel="stylesheet" type="text/css"/>
    <link href="css/table.css" rel="stylesheet" type="text/css"/>
    <link href="css/impromtu.css" rel="stylesheet" type="text/css"/>
    <link href="css/cal.css" rel="stylesheet" type="text/css"/>
    <link href="css/colortip-1.0-jquery.css" rel="stylesheet" type="text/css"/>
	<link href="css/multiple-select.css" rel="stylesheet" type="text/css" />

</head>
<body>
<div class="container">
    <?php
#include header
    include("header.php");

#include navigation files
    if (isset($_GET["page"])) {
        if (file_exists("include/" . $_GET["page"] . ".inc")) {
            #access
            if ($_SESSION["access"] == "ALL_ACCESS" or eregi($_GET["page"], $_SESSION["access"])) {
                require("include/" . $_GET["page"] . ".inc");
            } else {
                echo '<p id="tabname"><br><img src="img/no_access.png" width=100><p>';
            }
        } else {
            //error and logs
            //....
        }
#include opener files
    } else {
        //include default page
        require("include/search.inc");
    }
    ?>

</div>

</body>

</html>

