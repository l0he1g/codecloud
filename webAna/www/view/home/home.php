<?php
session_start();
if(!isset($_SESSION["login"]) || $_SESSION["login"] !==true){
    $_SESSION['login']=false;
    header("Location:../login/login.php");
}
    require_once '../../common/page.php';
    require_once '../../common/include_js.php';
    require_once '../../common/loading.php';
    require_once '../../common/extjs_config.php';
    page_header_top("网站统计");   
    extjs_config_cc();
	get_include_js("home.js"); 
	page_header_down();
    loading();
    page_footer();
?>
