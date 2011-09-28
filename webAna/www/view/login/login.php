<?php
    require_once '../../common/page.php';
    require_once '../../common/include_js.php';
    require_once '../../common/loading.php';
    require_once '../../common/extjs_config.php';
    page_header_top("网站分析系统登录");   
    extjs_config_cc();
	get_include_js("login.js"); 
	page_header_down();
    loading("../../images/loading.gif");
    page_footer();
?>