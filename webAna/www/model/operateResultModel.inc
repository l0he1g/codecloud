<?php

require_once ("../configure/systemuser_config.inc");

class OperateResultModel 
{
	private $pri_optFlag = OPERATE_OK;//操作结果标志(没有权限执行/执行成功/执行失败)
	private $pri_optResult = 0;
	private $pri_optRows = array();
	
	public function setOperateSuccess($pin_result,$pin_resultrows)
	{
		$this->pri_optFlag = OPERATE_OK;
		$this->pri_optResult = $pin_result;
		$this->pri_optRows = $pin_resultrows;
	}
	public function setOperateFailure($pin_result,$pin_resultrows)
	{
		$this->pri_optFlag = OPERATE_ERROR;
		$this->pri_optResult = $pin_result;
		$this->pri_optRows = $pin_resultrows;
	}
	public function getTransferResultDatas()
	{
		//return array(TRANSFER_RESULTOPT=>$this->pri_optFlag,TRANSFER_RESULT=>$this->pri_optResult, TRANSFER_RESULTROWS =>$this->pri_optRows);
		if($this->pri_optFlag==OPERATE_OK)
		{
			return array("success"=>true,TRANSFER_RESULT=>$this->pri_optResult, TRANSFER_RESULTROWS =>$this->pri_optRows);
		}
		else
		{
			return array("success"=>false,TRANSFER_RESULT=>$this->pri_optResult, TRANSFER_RESULTROWS =>$this->pri_optRows);
		}
	}
}

?>
