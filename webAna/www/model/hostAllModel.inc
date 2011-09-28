<?php
/*
 *created by chenwenbin 20091103
 *chenwenbin@software.ict.ac.cn
 * 网站总排名:major/subcate排名，domain,host排名
 */
require_once("../util/logger.inc");
require_once("../configure/db_config.inc");
require_once("../configure/systemuser_config.inc");
require_once("../util/database.inc");
require_once("../util/util.inc");


class HostAllModel
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
	
	/**
	 * 查询一段时间的majorcate排名
	 * @params: $pin_date_type是表示查询的时间类型（week或month）,$pin_date_value是表示查询具体是哪个月或周。
	 * @return array,
	 * 
	 */
	public function getMajorcatesTop($pin_date_type,$pin_date_value)
	{
		$sql = "select majorcate,num from ".SITES_DB.".majorcates_${pin_date_type}_tb where tid=$pin_date_value;";
		try
		{
			$result = db_query($sql,$this->pri_db_conn);
			$ret_arr =array();
			if( !$result ){
				$ret_arr[]=array("host"=>'no',"num"=>0);
			}
			else{
				while($row = $result->fetch_row()){
					$ret_arr[] = array("majorcate"=>$row[0],"num"=>$row[1]);
				}
			}
			db_result_free($result);
			return $ret_arr;
		}
		catch(Exception $exp)
		{
			echo $exp->getMessage();
			return false;
		}
	}

		/**
	 * 查询一段时间的subcate排名
	 * @params: $pin_date_type是表示查询的时间类型（week或month）,$pin_date_value是表示查询具体是哪个月或周。
	 * @return array,
	 * 
	 */
	public function getSubcatesTop($pin_date_type,$pin_date_value)
	{
		$sql = "select subcate,num from ".SITES_DB.".subcates_${pin_date_type}_tb where tid=$pin_date_value and subcate<>'0';";
		try
		{
			$result = db_query($sql,$this->pri_db_conn);
			$ret_arr =array();
			if( !$result ){
				$ret_arr[]=array("host"=>'no',"num"=>0);
			}
			else{
				while($row = $result->fetch_row()){
					$ret_arr[] = array("subcate"=>$row[0],"num"=>$row[1]);
				}
			}
			db_result_free($result);
			return $ret_arr;
		}
		catch(Exception $exp)
		{
			echo $exp->getMessage();
			return false;
		}
	}
		/**
	 * 查询一段时间的domain排名
	 * @params: $pin_date_type是表示查询的时间类型（week或month）,$pin_date_value是表示查询具体是哪个月或周。
	 * @return array,
	 * 
	 */
	public function getDomainsTop($pin_date_type,$pin_date_value,$pin_top=50)
	{
		$sql = "select domain,num from ".SITES_DB.".domains_${pin_date_type}_tb where tid=$pin_date_value limit $pin_top;";
		try
		{
			$result = db_query($sql,$this->pri_db_conn);
			$ret_arr =array();
			if( !$result ){
				$ret_arr[]=array("domain"=>'no',"num"=>0);
			}
			else{
				while($row = $result->fetch_row()){
					$ret_arr[] = array("domain"=>$row[0],"num"=>$row[1]);
				}
			}
			db_result_free($result);
			return $ret_arr;
		}
		catch(Exception $exp)
		{
			echo $exp->getMessage();
			return false;
		}
	}

			/**
	 * 查询一段时间的host排名
	 * @params: $pin_date_type是表示查询的时间类型（week或month）,$pin_date_value是表示查询具体是哪个月或周。
	 * @return array,
	 */
	public function getHostsTop($pin_date_type,$pin_date_value,$pin_top=50)
	{
		$sql = "select host,num from ".SITES_DB.".hosts_${pin_date_type}_tb where tid=$pin_date_value limit $pin_top;";
		try
		{
			$result = db_query($sql,$this->pri_db_conn);
			$ret_arr =array();
			if( !$result ){
				$ret_arr[]=array("host"=>'no',"num"=>0);
			}
			else{
				while($row = $result->fetch_row()){
					$ret_arr[] = array("host"=>$row[0],"num"=>$row[1]);
				}
			}
			db_result_free($result);
			return $ret_arr;
		}
		catch(Exception $exp)
		{
			echo $exp->getMessage();
			return false;
		}
	}

			/**
	 * 查询一段时间的关键词排名
	 * @params: $pin_date_type是表示查询的时间类型（week或month）,$pin_date_value是表示查询具体是哪个月或周。
	 * @return array,
	 */
	public function getWordsTop($pin_date_type,$pin_date_value,$pin_top=50)
	{
		$sql = "select word,num from ".WORD_DB.".words_${pin_date_type}_tb where tid=$pin_date_value limit $pin_top;";
		try
		{
			$result = db_query($sql,$this->pri_db_conn);
			$ret_arr =array();
			if( !$result ){
				$ret_arr[]=array("host"=>'no',"num"=>0);
			}
			else{
				while($row = $result->fetch_row()){
					$ret_arr[] = array("keywords"=>$row[0],"num"=>$row[1]);
				}
			}
			db_result_free($result);
			return $ret_arr;
		}
		catch(Exception $exp)
		{
			echo $exp->getMessage();
			return false;
		}
	}

}

?>
