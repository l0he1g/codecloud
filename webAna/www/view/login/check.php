<?php
  /**
   * 核查用户登陆是否成功
   * @copyright 中科院计算所HyperCloud@team
   * @author liujun <liujun@software.ict.ac.cn>
   * @date 2009-3-3
   * @edited by xiaohui 2009-3-19
   * @version 1.1
   */
session_start();
require_once("../../util/logger.inc");

$username = $_POST['username'];
$password = $_POST['password'];

/*added by xiaohui*/
logger_enable();
//transfer request to paraments:$_REQUEST->$glb_paras

logger_print($username, " logined in. ");

if ( ($username == 'admin') && ($password == 'lighttpd') )
{
    $_SESSION['login']=true;          //设置 session的值，为后面判断用户是否登陆提供判断
     echo ("{success:true,msg:'ok'}");    //此处输出服务端返回的信息，必须是json格式
}
else{
	echo ( "{success:false,msg:'用户名或密码错误!'}" );
}
 
?>