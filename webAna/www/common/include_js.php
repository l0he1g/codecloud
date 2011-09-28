<?php
   //每个php页面所调用ext的js文件
   function get_include_js($js_src="../../js/login.js") {
   	echo '<script type="text/javascript"  src="'.$js_src.'"></script>';
   }
   function get_include_css($css_src="../../css/main.css"){
	echo '<link rel="stylesheet" type="text/css" href="'.$css_src.'" />';
   }
   //通过session来判断用户是否登陆
   function get_session_js($ext_image_url="../../ext/resources/images/default/s.gif",$location_login="login.php",$height="20",$width="365") {
   echo '<script type="text/javascript">invalid_login_js("'.$ext_image_url.'","'.$location_login.'","'.$height.'","'.$width.'");</script>';
   }
   
   function get_error_session(){
   echo 'session_start();';
   echo 'if(!isset($_SESSION["login"]) || $_SESSION["login"] !==true){';
   echo 'get_session_js();';
   echo '$_SESSION["login"]=false; ';
   echo 'exit;' ;  
   echo '};';
   echo '}';
   } 
  
?>