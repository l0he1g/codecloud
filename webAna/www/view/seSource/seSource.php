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
    page_header_top("搜索引擎来源");   
    extjs_config_cc();
	get_include_css("../../css/Spinner.css"); 
	get_include_js("../../js/ux/Ext.allowBlank.js"); 
	get_include_js("../../js/ux/Spinner.js"); 
	get_include_js("../../js/ux/SpinnerField.js");
	get_include_js("../../js/ux/Ext.ux.dateToolbar2.js");
	get_include_js("../../js/ux/PagingMemoryProxy.js");
	get_include_js("../../js/ux/Ext.ux.PieGrid2.js"); 
	get_include_js("seSource.js"); 
	page_header_down();
    loading("../../images/loading.gif");
    page_footer();
?>
