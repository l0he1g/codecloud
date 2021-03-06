<?php
/*
 *关键词访问统计
 *chenwenbin@software.ict.ac.cn
 */
require_once("../util/logger.inc");
require_once("../configure/db_config.inc");
require_once("../configure/systemuser_config.inc");
require_once("../util/database.inc");
require_once("../util/util.inc");


class KeywordModel
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
	 * 查询得到某关键字的访问数量.
	 * @params: $pin_keyword,用户要查询的关键字,$pin_day是表示这些统计数据是那一天的。
	 * @return num.最小的返回值是0,
	 */
	public function getKeywordVisitNum($pin_keyword,$pin_day)
	{
		$sql = "select num from ".WORD_DB.".words_day_tb where word='$pin_keyword' and tid='$pin_day'";
		try
		{
			$result = db_query($sql,$this->pri_db_conn);
			$ret =0;
			if($row = $result->fetch_row())
			{
				$ret= $row[0];
			}
			db_result_free($result);
			return $ret;
		}
		catch(Exception $exp)
		{		
			echo $exp->getMessage();
			logger_print("$sql");
			logger_print("error: can not getKeywordVisitNum($pin_keyword,$pin_day)!");	
			return false;
		}
	}
	
	/**
	 * 查询一段时间每天此关键字访问数量.
	 * @params: $pin_keyword,用户要查询的关键字,$pin_date_type是表示查询的时间类型（周或月）,$pin_date_value是表示查询具体是哪个月或周。
	 * @return array,
	 */
	public function getKeywordVisitNums($pin_keyword,$pin_date_type,$pin_date_value)
	{
		$start_end = get_day($pin_date_type,$pin_date_value);
		if(false === $start_end)
		{
			return false;
		}
		$start_day = $start_end['start_day'];
		$end_day = $start_end['end_day'];
		$c_day = $start_day;
		$num_arr = array();
		while($c_day<=$end_day)
		{
			$day_result = $this->getKeywordVisitNum($pin_keyword,$c_day);
			if(false === $day_result)
			{
				logger_print("get keyword visit uid nums error $c_day");
				return false;
			}
			$num_arr[] = array($c_day,$day_result);
			
			$y = intval( substr($c_day,0,4) );
			$m = intval( substr($c_day,4,2) );
			$d = intval( substr($c_day,6,2) );
			$c_day = date('Ymd',mktime(0,0,0,$m,$d+1,$y) );
		}
		return $num_arr;
	}
	
	
	/**
	 * 查询在$pin_day这一天和此关键字相关的uid数目,即在此天有多少uid“访问“过此关键字.
	 * @params: $pin_keyword,用户要查询的关键字,$pin_day是表示这些统计数据是那一天的。
	 * @return num.最小的返回值是0,
	 */
	public function getKeywordVisitUidNum($pin_keyword,$pin_day)
	{
		$sql = "select num from ".WORD_DB.".worduid_day_tb where word='$pin_keyword' and tid='$pin_day'";
		try
		{
			$result = db_query($sql,$this->pri_db_conn);
			$ret =0;
			if($row = $result->fetch_row())
			{
				$ret= $row[0];
			}
			db_result_free($result);
			return $ret;
		}
		catch(Exception $exp)
		{		
			echo $exp->getMessage();
			logger_print("$sql");
			logger_print("error: can not getKeywordVisitUidNum($pin_keyword,$pin_day)!");	
			return false;
		}
	}
	
	/**
	 * 查询一段时间每天和此关键字相关的uid数目.
	 * @params: $pin_keyword,用户要查询的关键字,$pin_date_type是表示查询的时间类型（周或月）,$pin_date_value是表示查询具体是哪个月或周。
	 * @return array,
	 */
	public function getKeywordVisitUidNums($pin_keyword,$pin_date_type,$pin_date_value)
	{
		$start_end = get_day($pin_date_type,$pin_date_value);
		if(false === $start_end)
		{
			return false;
		}
		$start_day = $start_end['start_day'];
		$end_day = $start_end['end_day'];
		$c_day = $start_day;
		$num_arr = array();
		while($c_day<=$end_day)
		{
			$day_result = $this->getKeywordVisitUidNum($pin_keyword,$c_day);
			if(false === $day_result)
			{
				logger_print("get keyword visit uid nums error $c_day");
				return false;
			}
			$num_arr[] = array($c_day,$day_result);
			
			$y = intval( substr($c_day,0,4) );
			$m = intval( substr($c_day,4,2) );
			$d = intval( substr($c_day,6,2) );
			$c_day = date('Ymd',mktime(0,0,0,$m,$d+1,$y) );
		}
		return $num_arr;
	}
	
	/**
	 * 查询得到某关键字的访问的IP数量.
	 * @params: $pin_keyword,用户要查询的关键字,$pin_day是表示这些统计数据是那一天的。
	 * @return num.最小的返回值是0,
	 */
	public function getKeywordVisitIPNum($pin_keyword,$pin_day)
	{
		$sql = "select num from ".WORD_DB.".worduip_day_tb where word='$pin_keyword' and tid='$pin_day'";
		try
		{
			$result = db_query($sql,$this->pri_db_conn);
			$ret =0;
			if($row = $result->fetch_row())
			{
				$ret= $row[0];
			}
			db_result_free($result);
			return $ret;
		}
		catch(Exception $exp)
		{		
			echo $exp->getMessage();
			logger_print("$sql");
			logger_print("error: can not getKeywordVisitIPNum($pin_keyword,$pin_day)!");	
			return false;
		}
	}
	
	/**
	 * 查询一段时间每天和此关键字相关的IP数目.
	 * @params: $pin_keyword,用户要查询的关键字,$pin_date_type是表示查询的时间类型（周或月）,$pin_date_value是表示查询具体是哪个月或周。
	 * @return array,
	 */
	public function getKeywordVisitIPNums($pin_keyword,$pin_date_type,$pin_date_value)
	{
		$start_end = get_day($pin_date_type,$pin_date_value);
		if(false === $start_end)
		{
			return false;
		}
		$start_day = $start_end['start_day'];
		$end_day = $start_end['end_day'];
		$c_day = $start_day;
		$num_arr = array();
		while($c_day<=$end_day)
		{
			$day_result = $this->getKeywordVisitIPNum($pin_keyword,$c_day);
			if(false === $day_result)
			{
				logger_print("get keyword visit IP nums error $c_day");
				return false;
			}
			$num_arr[] = array($c_day,$day_result);
			
			$y = intval( substr($c_day,0,4) );
			$m = intval( substr($c_day,4,2) );
			$d = intval( substr($c_day,6,2) );
			$c_day = date('Ymd',mktime(0,0,0,$m,$d+1,$y) );
		}
		return $num_arr;
	}
	
	
	
	/**
	 * 查询一段时间此关键字相关的Domain top50.
	 * @params: $pin_keyword,用户要查询的关键字,$pin_date_type是表示查询的时间类型（week或month）,$pin_date_value是表示查询具体是哪个月或周。
	 * @return array,
	 */
	public function getKeywordDomainTop($pin_keyword,$pin_date_type,$pin_date_value,$pin_top=50)
	{
		$sql = "select domain,num from ".WORD_DB.".worddomain_".$pin_date_type."_tb where tid=$pin_date_value and word='$pin_keyword'  limit $pin_top";
		try
		{
			$result = db_query($sql,$this->pri_db_conn);
			$ret_arr =array();
			while($row = $result->fetch_row())
			{
				$ret_arr[] = array("domain"=>$row[0],"num"=>$row[1]);
			}
			if(count($ret_arr) == 0)
			{
				$ret_arr[] = array("domain"=>"no","num"=>0);
			}
			db_result_free($result);
			return $ret_arr;
		}
		catch(Exception $exp)
		{		
			echo $exp->getMessage();
			logger_print($sql);
			logger_print("error: can not getKeywordDomainTop($pin_keyword,$pin_date_type,$pin_date_value,$pin_top)!");
			//logger_print($sql);	
			return false;
		}
	}
	
	/**
	 * 查询一段时间此关键字相关的Host top50.
	 * @params: $pin_keyword,用户要查询的关键字,$pin_date_type是表示查询的时间类型（week或month）,$pin_date_value是表示查询具体是哪个月或周。
	 * @return array,
	 */
	public function getKeywordHostTop($pin_keyword,$pin_date_type,$pin_date_value,$pin_top=50)
	{
		$sql = "select host,num from ".WORD_DB.".wordhost_".$pin_date_type."_tb where tid=$pin_date_value and word='$pin_keyword'  limit $pin_top";
		try
		{
			$result = db_query($sql,$this->pri_db_conn);
			$ret_arr =array();
			while($row = $result->fetch_row())
			{
				$ret_arr[] = array("host"=>$row[0],"num"=>$row[1]);
			}
			if(count($ret_arr) == 0)
			{
				$ret_arr[] = array("host"=>"no","num"=>0);
			}
			db_result_free($result);
			return $ret_arr;
		}
		catch(Exception $exp)
		{		
			echo $exp->getMessage();
			logger_print("error: can not getKeywordHostTop($pin_keyword,$pin_date_type,$pin_date_value,$pin_top)!");	
			return false;
		}
	}
	
	/**
	 * 查询一段时间此关键字相关的url top50.
	 * @params: $pin_keyword,用户要查询的关键字,$pin_date_type是表示查询的时间类型（week或month）,$pin_date_value是表示查询具体是哪个月或周。
	 * @return array,
	 */
	public function getKeywordUrlTop($pin_keyword,$pin_date_type,$pin_date_value,$pin_top=50)
	{
		$sql = "select url,num from ".WORD_DB.".wordurl_".$pin_date_type."_tb where tid=$pin_date_value and word='$pin_keyword'  limit $pin_top";
		try
		{
			$result = db_query($sql,$this->pri_db_conn);
			$ret_arr =array();
			while($row = $result->fetch_row())
			{
				$ret_arr[] = array("url"=>$row[0],"num"=>$row[1]);
			}
			if(count($ret_arr) == 0)
			{
				$ret_arr[] = array("url"=>"no","num"=>0);
			}
			db_result_free($result);
			return $ret_arr;
		}
		catch(Exception $exp)
		{
			echo $exp->getMessage();
			logger_print($sql);
			logger_print("error: can not getKeywordUrlTop($pin_keyword,$pin_date_type,$pin_date_value,$pin_top)!");	
			return false;
		}
	}
	
	/**
	 * 查询一段时间此关键字相关的Uid top50.
	 * @params: $pin_keyword,用户要查询的关键字,$pin_date_type是表示查询的时间类型（week或month）,$pin_date_value是表示查询具体是哪个月或周。
	 * @return array,
	 */
	public function getKeywordUidTop($pin_keyword,$pin_date_type,$pin_date_value,$pin_top=50)
	{
		$sql = "select uid,num from ".WORD_DB.".worduid_".$pin_date_type."_tb where tid=$pin_date_value and word='$pin_keyword' limit $pin_top";
		try
		{
			$result = db_query($sql,$this->pri_db_conn);
			$ret_arr =array();
			while($row = $result->fetch_row())
			{
				$ret_arr[] = array("userid"=>$row[0],"num"=>$row[1]);
			}
			if(count($ret_arr) == 0)
			{
				$ret_arr[] = array("userid"=>"no","num"=>0);
			}
			db_result_free($result);
			return $ret_arr;
		}
		catch(Exception $exp)
		{	
			echo $exp->getMessage();
			logger_print($sql);	
			logger_print("error: can not getKeywordUidTop($pin_keyword,$pin_date_type,$pin_date_value,$pin_top)!");	
			return false;
		}
	}

}

?>
