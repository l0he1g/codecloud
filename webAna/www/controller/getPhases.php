<?php
/*
 *created by yanxiaohui 20091203
 */
session_start();
if(!isset($_SESSION["login"]) || $_SESSION["login"] !==true){
    $_SESSION['login']=false;
    header("Location:../view/login/login.php");
}

require_once ("../model/dateModel.inc");
require_once ("../model/operateResultModel.inc");

//$type = $_REQUEST['type']; //type is week or month
$type = http_get('type','week'); //type is week or month
$ret = process($type);
echo json_encode($ret);

function process( $type ){
	logger_enable();
	$ret_datas = new OperateResultModel();
	$dm = new DateModel();

	$result;
	if( $type == 'week' ){
		$result = $dm->get_weeks();
	}
	elseif( $type == 'month' ){
		$result = $dm->get_months();
	}
	else{
		$ret_datas->setOperateFailure(ARGU_INVALID,NULL);
		return $ret_datas->getTransferResultDatas();
	}
	$ret_datas->setOperateSuccess(count($result),$result);
	return $ret_datas->getTransferResultDatas();
}

?>
