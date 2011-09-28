<?php
/*
 *created by yanxiaohui 20091130
 */
require_once("../util/logger.inc");
require_once("../configure/db_config.inc");
require_once("../configure/systemuser_config.inc");
require_once("../util/database.inc");
require_once("../util/util.inc");

class DateModel
{
	private $pri_db_conn=NULL;
	private $pri_db_ip = NULL;
	private $pri_db_port = NULL;
	private $pri_db_user = NULL;
	private $pri_db_pwd = NULL;
	private $pri_db_database = NULL;
	private $pri_db_sock = NULL;
	/**
	 * 提醒信息持久化存储模型构建函数.
	 */
	function __construct()
	{
		global $glb_mysql_server_ip,$glb_mysql_server_port,$glb_mysql_username;
		global $glb_mysql_password,$glb_mysql_database,$glb_mysql_sock;
		$this->pri_db_ip = $glb_mysql_server_ip;
		$this->pri_db_port = $glb_mysql_server_port;
		$this->pri_db_user = $glb_mysql_username;
		$this->pri_db_pwd = $glb_mysql_password;
		$this->pri_db_database = $glb_mysql_database;
		$this->pri_db_sock = $glb_mysql_sock;
		/*连接数据库得到连接句柄*/
		try
		{
			//printf("%s--%s--%s--%s--%s--%s\n",$this->pri_db_ip, $this->pri_db_user,$this->pri_db_pwd, $this->pri_db_database,$this->pri_db_port,$this->pri_db_sock);
			$this->pri_db_conn = db_connect($this->pri_db_ip, $this->pri_db_user,$this->pri_db_pwd, $this->pri_db_database,$this->pri_db_port,$this->pri_db_sock);
		}
		catch(Exception $exp)
		{
			$this->pri_db_conn = NULL;
			throw new Exception("Database connetion is error:".$exp->getMessage());
		}
	}
	
	/**
	 * 提醒信息持久化存储模型析构函数.
	 */
	function __destruct()
	{
		if($this->pri_db_conn!=NULL)
		{
			//关闭数据库连接
			db_connect_close($this->pri_db_conn);
		}		
		$this->pri_db_conn = NULL;
		$this->pri_db_ip = NULL;
		$this->pri_db_port = NULL;
		$this->pri_db_user = NULL;
		$this->pri_db_pwd = NULL;
		$this->pri_db_database = NULL;
		$this->pri_db_sock = NULL;
	}

	/*
	 *获取周数
	 */
	function get_weeks()
	{
		$sql = "select tid from weeks order by tid desc";
		try
		{
			$result = db_query($sql,$this->pri_db_conn);
			$ret_arr = array();
			if(  mysqli_num_rows($result)==0 ){
				$ret_arr[] = array("week"=>'no');
			}
			else{
				while($row = $result->fetch_row()){
					$items = get_day('week',$row[0]);
					$start = substr($items['start_day'],4,2).'.'.substr($items['start_day'],6,2);
					$end = substr($items['end_day'],4,2).'.'.substr($items['end_day'],6,2);
					$wnote = "${row[0]}周(${start}日-${end}日)";
					$ret_arr[] = array("week"=>$wnote);
				}
			}
			db_result_free($result);
			return $ret_arr;
		}
		catch(Exception $exp)
		{
			print $exp->getMessage();
			return false;
		}
	}
	/*
	 *获取月数
	 */
	function get_months()
	{
		$sql = "select tid from months";
		try
		{
			$result = db_query($sql,$this->pri_db_conn);
			$ret_arr = array();
			if(  mysqli_num_rows($result)==0 ){
				$ret_arr[] = array("month"=>'no');
			}
			else{
				while($row = $result->fetch_row()){
					$ret_arr[] = array("month"=>"${row[0]}");
				}
			}
			db_result_free($result);
			return $ret_arr;
		}
		catch(Exception $exp)
		{
			print $exp->getMessage();
			return false;
		}
	}
}
?>
