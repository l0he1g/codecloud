<?php
/*
 *created by chenwenbin 20091103
 *chenwenbin@software.ict.ac.cn
 */
//error_reporting(1);
session_start();
if(!isset($_SESSION["login"]) || $_SESSION["login"] !==true){
    $_SESSION['login']=false;
    header("Location:../view/login/login.php");
}

require_once ("../model/hostSeModel.inc");
require_once ("../model/operateResultModel.inc");

$date_type = $_REQUEST[TRANSFER_DATE_TYPE]; //
$date_value = $_REQUEST[TRANSFER_DATE_VALUE]; //
$operation = $_REQUEST[TRANSFER_OPERATION];
if( isset($_REQUEST['host']) ){
	$host = trim($_REQUEST['host']);
}else{
	$host = trim($_REQUEST['word']);
}
//$date_type = "week"; //
//$date_value = '200933'; //
//$operation = 2;
//$host = "相册";
$ret = process($date_type, $date_value,$host,$operation);
echo json_encode($ret);
//print_r($ret);

function process($pin_date_type, $pin_date_value,$pin_host,$pin_operation)
{
	logger_enable();
	
    	$ret_datas = new OperateResultModel();
	/*if(!isset($_SESSION[SESSION_SYSUSER]))
	{	
		$ret_datas->setOperateFailure(NONE_SYSTEMUSER,NULL);
		return $ret_datas->getTransferResultDatas();
	}
	$sys_usermodel = unserialize($_SESSION[SESSION_SYSUSER]);*/
	
	//params authentication
	if(strlen($pin_host) <=0 )
	{
		$ret_datas->setOperateFailure(ARGU_INVALID,NULL);
		return $ret_datas->getTransferResultDatas();
	}
	if(strcmp("$pin_date_type","week")==0 || strcmp("$pin_date_type","month")==0)
	{
		if(strcmp($pin_date_value,"this")==0 || strcmp($pin_date_value,"last")==0)
		{
			$pin_date_value = get_phase_id( $pin_date_type,$pin_date_value );
			if($pin_date_value === false)
			{
				logger_print("get date_value num error of get_phase_id( $pin_date_type,$pin_date_value )");
				$ret_datas->setOperateFailure(ARGU_INVALID,NULL);
				return $ret_datas->getTransferResultDatas();
			}
		}
	}
	else
	{
		$ret_datas->setOperateFailure(ARGU_INVALID,NULL);
		return $ret_datas->getTransferResultDatas();
	}
	
	try
	{
		$hostMod = new HostSeModel();
		switch($pin_operation)
		{
			case TOP_HOSTSW_QUERY:
			{
				$result = $hostMod->getHostSwTop($pin_host,$pin_date_type,$pin_date_value);
				
				if($result===FALSE )
				{
					$ret_datas->setOperateFailure(SERVER_ERROR,NULL);
					return $ret_datas->getTransferResultDatas();
				}
				$ret_datas->setOperateSuccess(count($result),$result);
				return $ret_datas->getTransferResultDatas();
				break;
			}
			case TOP_HOSTSE_QUERY:
			{
				$result = $hostMod->getHostseTop($pin_host,$pin_date_type,$pin_date_value);
				
				if($result===FALSE )
				{
					$ret_datas->setOperateFailure(SERVER_ERROR,NULL);
					return $ret_datas->getTransferResultDatas();
				}
				$ret_datas->setOperateSuccess(count($result),$result);
				return $ret_datas->getTransferResultDatas();
				break;
			}
			case TOP_SWHOST_QUERY:
			{
				$result = $hostMod->getSwHostTop($pin_host,$pin_date_type,$pin_date_value);
				
				if($result===FALSE )
				{
					$ret_datas->setOperateFailure(SERVER_ERROR,NULL);
					return $ret_datas->getTransferResultDatas();
				}
				$ret_datas->setOperateSuccess(count($result),$result);
				return $ret_datas->getTransferResultDatas();
				break;
			}
			default:
    		{
    			$ret_datas->setOperateFailure(ARGU_INVALID,NULL);
	    		return $ret_datas->getTransferResultDatas();	
    		} 
		}
	}
	catch(Exception $e)
	{
		$str = "In processHost.php:".$e->getMessage();
		logger_print($str,"error: ");
		$ret_datas->setOperateFailure(SERVER_ERROR,NULL);
		return $ret_datas->getTransferResultDatas();
	}
}
?>
