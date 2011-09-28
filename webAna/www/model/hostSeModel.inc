<?php
/*
 *搜索引擎统计
 */
require_once("../util/logger.inc");
require_once("../configure/db_config.inc");
require_once("../configure/systemuser_config.inc");
require_once("../util/database.inc");
require_once("../util/util.inc");


class HostSeModel
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
	 * 查询一段时间此host相关搜索关键词
	 * @params: $pin_value,用户要查询的host,$pin_date_type是表示查询的时间类型（week或month）,$pin_date_value
	 * @return array,
	 * 
	 */
	public function getHostSwTop($pin_value,$pin_date_type,$pin_date_value,$pin_top=50)
	{
	   	$sql = "select word,sum(num) as s from ".HOST_DB.".hostse_${pin_date_type}_tb where tid=$pin_date_value and host='$pin_value' group by word order by s desc limit $pin_top;";
		try
		{
			$result = db_query($sql,$this->pri_db_conn);
			$ret_arr =array();
			while($row = $result->fetch_row())
			{
				$ret_arr[] = array("word"=>$row[0],"num"=>$row[1]);
			}
			db_result_free($result);
			return $ret_arr;
		}
		catch(Exception $exp)
		{		
			logger_print("error: can not getHostUrlTop($pin_value,$pin_date_type,$pin_date_value,$pin_top)!");
			//logger_print($sql);	
			return false;
		}
	}
	/**
	 * 查询一段时间此host搜索最多的关键词及其来源
	 * @params: $pin_value,用户要查询的host,$pin_date_type是表示查询的时间类型（week或month）,$pin_date_value是表示查询具体是哪个月或周。
	 * @return array,
	 * 
	 */
	public function getHostSeTop($pin_value,$pin_date_type,$pin_date_value,$pin_top=50)
	{
		$sql = "select word,sum(num) as s from ".HOST_DB.".hostse_${pin_date_type}_tb where tid=$pin_date_value and host='$pin_value' group by word order by s desc limit $pin_top;";
		
		try
		{
			$result = db_query($sql,$this->pri_db_conn);
			$ret_arr =array();
			while($row = $result->fetch_row())
			{
				$wd = $row[0];
				$sql_google = "select sum(num) from ".HOST_DB.".hostse_${pin_date_type}_tb where tid=$pin_date_value and host='$pin_value' and word='$wd' and engine='google';";
				$result_google = db_query($sql_google,$this->pri_db_conn);
				$tmp = $result_google->fetch_row();
				$num_google = intval($tmp[0]);
				$sql_baidu = "select sum(num) from ".HOST_DB.".hostse_${pin_date_type}_tb where tid=$pin_date_value and host='$pin_value' and word='$wd' and engine='baidu';";
				$result_baidu = db_query($sql_baidu,$this->pri_db_conn);
				$tmp = $result_baidu->fetch_row();
				$num_baidu = intval($tmp[0]);

				$ret_arr[] = array("word"=>$wd,
								   "google"=>$num_google,
								   "baidu"=>$num_baidu,
								   "others"=>intval($row[1])-$num_google-$num_baidu,
								   "sum"=>intval($row[1])
					);
			}
			db_result_free($result);
			return $ret_arr;
		}
		catch(Exception $exp)
		{
			echo $exp->getMessage();
			logger_print("error: can not getHostSeTop($pin_value,$pin_date_type,$pin_date_value,$pin_top)!");
			//logger_print($sql);	
			return false;
		}
	}

	/**
	 * 查询一段时间总的搜索引擎访问量排名
	 * @params: $pin_value,用户要查询的host,$pin_date_type是表示查询的时间类型（week或month）,$pin_date_value
	 * @return array,
	 * 
	 */
	public function getEngineTop($pin_date_type,$pin_date_value,$pin_top=50)
	{
	   	$sql = "select engine,sum(num) as s from ".HOST_DB.".hostse_${pin_date_type}_tb where tid=$pin_date_value group by engine order by s desc limit $pin_top;";
		try
		{
			$result = db_query($sql,$this->pri_db_conn);
			$ret_arr =array();
			while($row = $result->fetch_row())
			{
				$ret_arr[] = array("engine"=>$row[0],"num"=>$row[1]);
			}
			db_result_free($result);
			return $ret_arr;
		}
		catch(Exception $exp)
		{		
			logger_print("error: can not getHostUrlTop($pin_value,$pin_date_type,$pin_date_value,$pin_top)!");
			//logger_print($sql);	
			return false;
		}
	}

	/**
	 * 查询一段时间索最多的关键词来源
	 * @params: $pin_value,用户要查询的host,$pin_date_type是表示查询的时间类型（week或month）,$pin_date_value是表示查询具体是哪个月或周。
	 * @return array,
	 */
	public function getSeTop($pin_date_type,$pin_date_value,$pin_top=50)
	{
		$sql = "select word,sum(num) as s from ".HOST_DB.".hostse_${pin_date_type}_tb where tid=$pin_date_value group by word order by s desc limit $pin_top;";
		
		try
		{
			$result = db_query($sql,$this->pri_db_conn);
			$ret_arr =array();
			while($row = $result->fetch_row())
			{
				$wd = $row[0];
				$sql_google = "select sum(num) from ".HOST_DB.".hostse_${pin_date_type}_tb where tid=$pin_date_value and word='$wd' and engine='google';";
				$result_google = db_query($sql_google,$this->pri_db_conn);
				$tmp = $result_google->fetch_row();
				$num_google = intval($tmp[0]);
				$sql_baidu = "select sum(num) from ".HOST_DB.".hostse_${pin_date_type}_tb where tid=$pin_date_value and word='$wd' and engine='baidu'";
				$result_baidu = db_query($sql_baidu,$this->pri_db_conn);
				$tmp = $result_baidu->fetch_row();
				$num_baidu = intval($tmp[0]);

				$ret_arr[] = array("word"=>$wd,
								   "google"=>$num_google,
								   "baidu"=>$num_baidu,
								   "others"=>intval($row[1])-$num_google-$num_baidu,
								   "sum"=>intval($row[1])
					);
			}
			db_result_free($result);
			return $ret_arr;
		}
		catch(Exception $exp)
		{
			echo $exp->getMessage();
			logger_print("error: can not getHostSeTop($pin_value,$pin_date_type,$pin_date_value,$pin_top)!");
			//logger_print($sql);	
			return false;
		}
	}
		/**
	 * 查询一段时间通过搜索此关键词进入的最多的网站
	 * @params: $pin_value,用户要查询的word,$pin_date_type是表示查询的时间类型（week或month）,$pin_date_value
	 * @return array,
	 * 
	 */
	public function getSwHostTop($pin_value,$pin_date_type,$pin_date_value,$pin_top=50)
	{
	   	$sql = "select host,sum(num) as s from ".HOST_DB.".hostse_${pin_date_type}_tb where tid=$pin_date_value and word='$pin_value' group by host order by s desc limit $pin_top;"; 
		try
		{
			$result = db_query($sql,$this->pri_db_conn);
			$ret_arr =array();
			while($row = $result->fetch_row())
			{
				$ret_arr[] = array("host"=>$row[0],"num"=>$row[1]);
			}
			db_result_free($result);
			return $ret_arr;
		}
		catch(Exception $exp)
		{
			echo $exp->getMessage();
			logger_print("error: can not getHostUrlTop($pin_value,$pin_date_type,$pin_date_value,$pin_top)!");
			//logger_print($sql);	
			return false;
		}
	}
}
?>
