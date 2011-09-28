<?php
    //相对根目录下的script
   function extjs_config_root($ext_all_css="js/ext/resources/css/ext-all.css",$common_css="css/common.css",$ext_base_js="js/ext/adapter/jquery/jquery.js",$ext_adapter_js="js/ext/adapter/jquery/ext-jquery-adapter.js",$ext_all_js="js/ext/ext-all.js",$ext_cn_js="js/ext/locale/ext-lang-zh_CN.js",$common_js="js/common.js") {
   	 echo'<link rel="stylesheet" type="text/css" href="'.$ext_all_css.'"/>';  
     echo'<link rel="stylesheet" type="text/css" href="'.$common_css.'"/>';
     echo'<script type="text/javascript" src="'.$ext_base_js.'"></script>';
     echo'<script type="text/javascript" src="'.$ext_adapter_js.'"></script>';
     echo'<script type="text/javascript" src="'.$ext_all_js.'"></script>';
   	 echo'<script type="text/javascript" src="'.$ext_cn_js.'"></script>'; 
   	 echo'<script type="text/javascript" src="'.$common_js.'"></script>';	 
   }
   
   //相对子目录下的script
	function extjs_config_cc($ext_all_css="../../js/ext/resources/css/ext-all.css",$common_css="../../css/common.css",$ext_base_js="../../js/ext/adapter/jquery/jquery.js",$ext_adapter_js="../../js/ext/adapter/jquery/ext-jquery-adapter.js",$ext_all_js="../../js/ext/ext-all.js",$ext_cn_js="../../js/ext/locale/ext-lang-zh_CN.js",$common_js="../../js/common.js") {
		echo'<link rel="stylesheet" type="text/css" href="'.$ext_all_css.'"/>';
		echo'<link rel="stylesheet" type="text/css" href="'.$common_css.'"/>';
		echo'<script type="text/javascript" src="'.$ext_base_js.'"></script>';
		echo'<script type="text/javascript" src="'.$ext_adapter_js.'"></script>';
		echo'<script type="text/javascript" src="'.$ext_all_js.'"></script>';
		echo'<script type="text/javascript" src="'.$ext_cn_js.'"></script>';
		echo'<script type="text/javascript" src="'.$common_js.'"></script>';
	}
   
    //相对子子目录下的script
   function extjs_config_ccc($ext_all_css="../../../js/ext/resources/css/ext-all.css",$common_css="../../../css/common.css",$ext_base_js="../../../js/ext/adapter/jquery/jquery.js",$ext_adapter_js="../../../js/ext/adapter/jquery/ext-jquery-adapter.js",$ext_all_js="../../../js/ext/ext-all.js",$ext_cn_js="../../../js/ext/locale/ext-lang-zh_CN.js",$common_js="../../../js/common.js") {
     echo'<link rel="stylesheet" type="text/css" href="'.$ext_all_css.'"/>';
     echo'<link rel="stylesheet" type="text/css" href="'.$common_css.'"/>';
     echo'<script type="text/javascript" src="'.$ext_base_js.'"></script>';
     echo'<script type="text/javascript" src="'.$ext_adapter_js.'"></script>';
     echo'<script type="text/javascript" src="'.$ext_all_js.'"></script>';
   	 echo'<script type="text/javascript" src="'.$ext_cn_js.'"></script>';
   	 echo'<script type="text/javascript" src="'.$common_js.'"></script>';
   }
?>