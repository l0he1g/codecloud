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

require_once ("../model/keywordModel.inc");
require_once ("../model/operateResultModel.inc");

$date_type = $_REQUEST[TRANSFER_DATE_TYPE]; //
$date_value = $_REQUEST[TRANSFER_DATE_VALUE]; //
$operation = $_REQUEST[TRANSFER_OPERATION];
$keyword = trim($_REQUEST[TRANSFER_KEYWORD]);

//$date_type = "week"; //
//$date_value = '200932'; //
//$operation = 2;
//$keyword = "相册";
$ret = processKeyword($date_type, $date_value,$keyword,$operation);
echo json_encode($ret);
//print_r($ret);

function processKeyword($pin_date_type, $pin_date_value,$pin_keyword,$pin_operation)
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
	if(strlen($pin_keyword) <=0 )
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
		$keywordMod = new KeywordModel();
		switch($pin_operation)
		{
			case DAY_QUERY:
			{
				//$pin_date_value="200946";
				$result_keyword = $keywordMod->getKeywordVisitNums($pin_keyword,$pin_date_type,$pin_date_value);
				
				$result_ip = $keywordMod->getKeywordVisitIPNums($pin_keyword,$pin_date_type,$pin_date_value);
				
				$result_uid = $keywordMod->getKeywordVisitUidNums($pin_keyword,$pin_date_type,$pin_date_value);
				
				if($result_keyword===FALSE || $result_ip===FALSE || $result_uid===FALSE)
				{
					$ret_datas->setOperateFailure(SERVER_ERROR,NULL);
					return $ret_datas->getTransferResultDatas();
				}
				$count = count($result_keyword);
				$result = array();
				for($i=0;$i<$count;$i++)
				{
					$result[] = array("date"=>$result_keyword[$i][0],'visits_num'=>$result_keyword[$i][1],'uid_num'=>$result_uid[$i][1],'ip_num'=>$result_ip[$i][1]);
				}
				$ret_datas->setOperateSuccess(count($result),$result);
				return $ret_datas->getTransferResultDatas();
				break;
			}
			case TOP_DOMAIN_QUERY:
			{
				$result_top_damain = $keywordMod->getKeywordDomainTop($pin_keyword,$pin_date_type,$pin_date_value);
				
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
				$result_top_host = $keywordMod->getKeywordHostTop($pin_keyword,$pin_date_type,$pin_date_value);
				
				if($result_top_host===FALSE )
				{
					$ret_datas->setOperateFailure(SERVER_ERROR,NULL);
					return $ret_datas->getTransferResultDatas();
				}
				$ret_datas->setOperateSuccess(count($result_top_host),$result_top_host);
				return $ret_datas->getTransferResultDatas();
				break;
			}
			case TOP_URL_QUERY:
			{
				$result_top_url = $keywordMod->getKeywordUrlTop($pin_keyword,$pin_date_type,$pin_date_value);
				
				if($result_top_url===FALSE )
				{
					$ret_datas->setOperateFailure(SERVER_ERROR,NULL);
					return $ret_datas->getTransferResultDatas();
				}
				$ret_datas->setOperateSuccess(count($result_top_url),$result_top_url);
				return $ret_datas->getTransferResultDatas();
				break;
			}
			case TOP_UID_QUERY:
			{
				$result_top_uid = $keywordMod->getKeywordUidTop($pin_keyword,$pin_date_type,$pin_date_value);
				
				if($result_top_uid===FALSE )
				{
					$ret_datas->setOperateFailure(SERVER_ERROR,NULL);
					return $ret_datas->getTransferResultDatas();
				}
				$ret_datas->setOperateSuccess(count($result_top_uid),$result_top_uid);
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
