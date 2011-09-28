<?php
/*
 *网站访问综合统计
 *chenwenbin@software.ict.ac.cn
 */
require_once("../util/logger.inc");
require_once("../configure/db_config.inc");
require_once("../configure/systemuser_config.inc");
require_once("../util/database.inc");
require_once("../util/util.inc");


class WebsiteCateModel
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
	 * 查询得到某网站分类的访问数量.
	 * @params: $pin_webcate_type表示是主类还是分类,$pin_webcate_value,用户要查询的网站分类,$pin_day是表示这些统计数据是那一天的。
	 * @return num.最小的返回值是0,
	 */
	public function getWebsiteCateVisitNum($pin_webcate_type,$pin_webcate_value,$pin_day)
	{
		if(strcmp($pin_webcate_type,MAJOR_CATEGORY)==0)
		{
			$sql = "select num from ".SITES_DB.".majorcates_day_tb where majorcate='$pin_webcate_value' and tid='$pin_day'";
		}
		else if(strcmp($pin_webcate_type,SUB_CATEGORY)==0)
		{
			$sql = "select num from ".SITES_DB.".subcates_day_tb where subcate='$pin_webcate_value' and tid='$pin_day'";
		}
		else
		{
			logger_print("error: can not know what kind of cate $pin_webcate_type!");	
			return false;
		}
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
			logger_print("error: can not getWebsiteCateVisitNum($pin_webcate_type,$pin_webcate_value,$pin_day)!");	
			return false;
		}
	}
	
	/**
	 * 查询一段时间每天此关键字访问数量.
	 * @params: $pin_webcate_type表示是主类还是分类,$pin_webcate_value,用户要查询的网站分类,$pin_date_type是表示查询的时间类型（周或月）,$pin_date_value是表示查询具体是哪个月或周。
	 * @return array,
	 */
	public function getWebsiteCateVisitNums($pin_webcate_type,$pin_webcate_value,$pin_date_type,$pin_date_value)
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
		while($c_day<$end_day)
		{
			$day_result = $this->getWebsiteCateVisitNum($pin_webcate_type,$pin_webcate_value,$c_day);
			if(false === $day_result)
			{
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
	 * 查询在$pin_day这一天和此网站分类相关的uid数目,即在此天有多少uid“访问“过此网站分类中的网站.
	 * @params: $pin_webcate_type表示是主类还是分类,$pin_webcate_value,用户要查询的网站分类,$pin_day是表示这些统计数据是那一天的。
	 * @return num.最小的返回值是0,
	 */
	public function getWebsiteCateVisitUidNum($pin_webcate_type,$pin_webcate_value,$pin_day)
	{
		if(strcmp($pin_webcate_type,MAJOR_CATEGORY)==0)
		{
			$sql = "select num from ".SITES_DB.".majorcateuid_day_tb where majorcate='$pin_webcate_value' and tid='$pin_day' order by num desc;";
		}
		else if(strcmp($pin_webcate_type,SUB_CATEGORY)==0)
		{
			$sql = "select num from ".SITES_DB.".subcateuid_day_tb where subcate='$pin_webcate_value' and tid='$pin_day' order by num desc;";
		}
		else
		{
			logger_print("error: can not know what kind of cate $pin_webcate_type!");	
			return false;
		}
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
			logger_print("error: can not getWebsiteCateVisitUidNum($pin_webcate_type,$pin_webcate_value,$pin_day)!");	
			return false;
		}
	}
	
	/**
	 * 查询一段时间每天和此关键字相关的uid数目.
	 * @params: $pin_webcate_type表示是主类还是分类,$pin_webcate_value,用户要查询的网站分类,$pin_date_type是表示查询的时间类型（周或月）,$pin_date_value是表示查询具体是哪个月或周。
	 * @return array,
	 */
	public function getWebsiteCateVisitUidNums($pin_webcate_type,$pin_webcate_value,$pin_date_type,$pin_date_value)
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
		while($c_day<$end_day)
		{
			$day_result = $this->getWebsiteCateVisitUidNum($pin_webcate_type,$pin_webcate_value,$c_day);
			if(false === $day_result)
			{
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
	 * 查询在$pin_day这一天和此网站分类相关的ip数目,即在此天有多少ip“访问“过此网站分类中的网站.
	 * @params: $pin_webcate_type表示是主类还是分类,$pin_webcate_value,用户要查询的网站分类,$pin_day是表示这些统计数据是那一天的。
	 * @return num.最小的返回值是0,
	 */
	public function getWebsiteCateVisitIPNum($pin_webcate_type,$pin_webcate_value,$pin_day)
	{
		if(strcmp($pin_webcate_type,MAJOR_CATEGORY)==0)
		{
			$sql = "select num from ".SITES_DB.".majorcateuip_day_tb where majorcate='$pin_webcate_value' and tid='$pin_day' order by num desc";
		}
		else if(strcmp($pin_webcate_type,SUB_CATEGORY)==0)
		{
			$sql = "select num from ".SITES_DB.".subcateuip_day_tb where subcate='$pin_webcate_value' and tid='$pin_day' order by num desc";
		}
		else
		{
			logger_print("error: can not know what kind of cate $pin_webcate_type!");	
			return false;
		}
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
			logger_print($sql);
			logger_print("error: can not getWebsiteCateVisitIPNum($pin_webcate_type,$pin_webcate_value,$pin_day)!");	
			return false;
		}
	}
	
	/**
	 * 查询一段时间每天和此网站分类相关的IP数目.
	 * @params: $pin_webcate_type表示是主类还是分类,$pin_webcate_value,用户要查询的网站分类,$pin_date_type是表示查询的时间类型（周或月）,$pin_date_value是表示查询具体是哪个月或周。
	 * @return array,
	 */
	public function getWebsiteCateVisitIPNums($pin_webcate_type,$pin_webcate_value,$pin_date_type,$pin_date_value)
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
		while($c_day<$end_day)
		{
			$day_result = $this->getWebsiteCateVisitIPNum($pin_webcate_type,$pin_webcate_value,$c_day);
			if(false === $num_arr["$c_day"])
			{
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
	 * 查询一段时间此网站分类相关的Domain top50.
	 * @params: $pin_webcate_type表示是主类还是分类,$pin_webcate_value,用户要查询的网站分类,$pin_date_type是表示查询的时间类型（week或month）,$pin_date_value是表示查询具体是哪个月或周。
	 * @return array,
	 * 
	 */
	public function getWebsiteCateDomainTop($pin_webcate_type,$pin_webcate_value,$pin_date_type,$pin_date_value,$pin_top=50)
	{
		if(strcmp($pin_webcate_type,MAJOR_CATEGORY)==0)
		{
			$sql = "select domain,num from ".SITES_DB.".majorcatedomain_".$pin_date_type."_tb where tid=$pin_date_value and majorcate='$pin_webcate_value'  limit $pin_top";
		}
		else if(strcmp($pin_webcate_type,SUB_CATEGORY)==0)
		{
			$sql = "select domain,num from ".SITES_DB.".subcatedomain_".$pin_date_type."_tb where tid=$pin_date_value and subcate='$pin_webcate_value'  limit $pin_top";
		}
		else
		{
			logger_print("error: can not know what kind of cate $pin_webcate_type!");	
			return false;
		}
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
			logger_print("error: can not getWebsiteCateDomainTop($pin_webcate_type,$pin_webcate_value,$pin_date_type,$pin_date_value,$pin_top)!");			
			return false;
		}
	}
	
	/**
	 * 查询一段时间此网站分类相关的Host top50.
	 * @params: $pin_webcate_type表示是主类还是分类,$pin_webcate_value,用户要查询的网站分类,$pin_date_type是表示查询的时间类型（week或month）,$pin_date_value是表示查询具体是哪个月或周。
	 * @return array,
	 * 
	 */
	public function getWebsiteCateHostTop($pin_webcate_type,$pin_webcate_value,$pin_date_type,$pin_date_value,$pin_top=50)
	{
		if(strcmp($pin_webcate_type,MAJOR_CATEGORY)==0)
		{
			$sql = "select host,num from ".SITES_DB.".majorcatehost_".$pin_date_type."_tb where tid=$pin_date_value and majorcate='$pin_webcate_value'  limit $pin_top";
		}
		else if(strcmp($pin_webcate_type,SUB_CATEGORY)==0)
		{
			$sql = "select host,num from ".SITES_DB.".subcatehost_".$pin_date_type."_tb where tid=$pin_date_value and subcate='$pin_webcate_value'  limit $pin_top";
		}
		else
		{
			logger_print("error: can not know what kind of cate $pin_webcate_type!");	
			return false;
		}
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
			logger_print("error: can not getWebsiteCateHostTop($pin_webcate_type,$pin_webcate_value,$pin_date_type,$pin_date_value,$pin_top)!");
			logger_print($sql);	
			return false;
		}
	}
	
	/**
	 * 查询一段时间此网站分类相关的Url top50.
	 * @params: $pin_webcate_type表示是主类还是分类,$pin_webcate_value,用户要查询的网站分类,$pin_date_type是表示查询的时间类型（week或month）,$pin_date_value是表示查询具体是哪个月或周。
	 * @return array,
	 * 
	 */
	public function getWebsiteCateUrlTop($pin_webcate_type,$pin_webcate_value,$pin_date_type,$pin_date_value,$pin_top=50)
	{
		if(strcmp($pin_webcate_type,MAJOR_CATEGORY)==0)
		{
			$sql = "select url,num from ".SITES_DB.".majorcateurl_".$pin_date_type."_tb where tid=$pin_date_value and majorcate='$pin_webcate_value'  limit $pin_top";
		}
		else if(strcmp($pin_webcate_type,SUB_CATEGORY)==0)
		{
			$sql = "select url,num from ".SITES_DB.".subcateurl_".$pin_date_type."_tb where tid=$pin_date_value and subcate='$pin_webcate_value'  limit $pin_top";
		}
		else
		{
			logger_print("error: can not know what kind of cate $pin_webcate_type!");	
			return false;
		}
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
			logger_print("error: can not getWebsiteCateUrlTop($pin_webcate_type,$pin_webcate_value,$pin_date_type,$pin_date_value,$pin_top)!");
			logger_print($sql);	
			return false;
		}
	}
	
	/**
	 * 查询一段时间此网站分类相关的Uid top50.
	 * @params: $pin_webcate_type表示是主类还是分类,$pin_webcate_value,用户要查询的网站分类,$pin_date_type是表示查询的时间类型（week或month）,$pin_date_value是表示查询具体是哪个月或周。
	 * @return array,
	 * 
	 */
	public function getWebsiteCateUidTop($pin_webcate_type,$pin_webcate_value,$pin_date_type,$pin_date_value,$pin_top=50)
	{
		if(strcmp($pin_webcate_type,MAJOR_CATEGORY)==0)
		{
			$sql = "select uid,num from ".SITES_DB.".majorcateuid_".$pin_date_type."_tb where tid=$pin_date_value and majorcate='$pin_webcate_value' limit $pin_top";
		}
		else if(strcmp($pin_webcate_type,SUB_CATEGORY)==0)
		{
			$sql = "select uid,num from ".SITES_DB.".subcateuid_".$pin_date_type."_tb where tid=$pin_date_value and subcate='$pin_webcate_value' limit $pin_top";
		}
		else
		{
			echo $exp->getMessage();
			logger_print("error: can not know what kind of cate $pin_webcate_type!");	
			return false;
		}
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
			logger_print("error: can not getWebsiteCateUidTnop($pin_webcate_type,$pin_webcate_value,$pin_date_type,$pin_date_value,$pin_top)!");
			logger_print($sql);	
			return false;
		}
	}
	
}

?>
