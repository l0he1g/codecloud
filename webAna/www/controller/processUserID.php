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

require_once ("../model/userIDModel.inc");
require_once ("../model/operateResultModel.inc");

$date_type = $_REQUEST[TRANSFER_DATE_TYPE]; //
$date_value = $_REQUEST[TRANSFER_DATE_VALUE]; //
$operation = $_REQUEST[TRANSFER_OPERATION];
$uid = trim($_REQUEST[TRANSFER_USERID]);

$ret = processUserID($date_type, $date_value,$uid,$operation);
echo json_encode($ret);
//print_r($ret);

function processUserID($pin_date_type, $pin_date_value,$pin_uid,$pin_operation)
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
	if(strlen($pin_uid) <=0 )
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
		$userIDMod = new UserIDModel();
		switch($pin_operation)
		{
			case DAY_QUERY:
			{
				//$pin_date_value="200946";
				$result_domain = $userIDMod->getUidVisitDomainNums($pin_uid,$pin_date_type,$pin_date_value);
				
				$result_visits = $userIDMod->getUidVisitsNums($pin_uid,$pin_date_type,$pin_date_value);
				
				$result_host = $userIDMod->getUidVisitHostNums($pin_uid,$pin_date_type,$pin_date_value);
				
				if($result_domain===FALSE || $result_visits===FALSE || $result_host===FALSE)
				{
					$ret_datas->setOperateFailure(SERVER_ERROR,NULL);
					return $ret_datas->getTransferResultDatas();
				}
				$count = count($result_domain);
				$result = array();
				for($i=0;$i<$count;$i++)
				{
					$result[] = array("date"=>$result_domain[$i][0],'domain_num'=>$result_domain[$i][1],'visits_num'=>$result_visits[$i][1],'host_num'=>$result_host[$i][1]);
				}
				$ret_datas->setOperateSuccess(count($result),$result);
				return $ret_datas->getTransferResultDatas();
				break;
			}
			case TOP_DOMAIN_QUERY:
			{
				$result_top_damain = $userIDMod->getUidDomainTop($pin_uid,$pin_date_type,$pin_date_value);
				
				if($result_top_damain===FALSE )
				{
					$ret_datas->setOperateFailure(SERVER_ERROR,NULL);
					return $ret_datas->getTransferResultDatas();
				}
				$ret_datas->setOperateSuccess(count($result_top_damain),$result_top_damain);
				return $ret_datas->getTransferResultDatas();
				break;
			}
			case TOP_HOST_QUERY:
			{
				$result_top_host = $userIDMod->getUidHostTop($pin_uid,$pin_date_type,$pin_date_value);
				
				if($result_top_host===FALSE )
				{
					$ret_datas->setOperateFailure(SERVER_ERROR,NULL);
					return $ret_datas->getTransferResultDatas();
				}
				$ret_datas->setOperateSuccess(count($result_top_host),$result_top_host);
				return $ret_datas->getTransferResultDatas();
				break;
			}
			case TOP_KEYWORD_QUERY:
			{
				$result_top_key = $userIDMod->getUidKeywordTop($pin_uid,$pin_date_type,$pin_date_value);
				
				if($result_top_key===FALSE )
				{
					$ret_datas->setOperateFailure(SERVER_ERROR,NULL);
					return $ret_datas->getTransferResultDatas();
				}
				$ret_datas->setOperateSuccess(count($result_top_key),$result_top_key);
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
		$str = "In processKeyword.php:".$e->getMessage();
		logger_print($str,"error: ");
		$ret_datas->setOperateFailure(SERVER_ERROR,NULL);
		return $ret_datas->getTransferResultDatas();
	}
}
?>
