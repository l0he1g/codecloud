<?php
require_once("../configure/db_config.inc");
require_once("../configure/systemuser_config.inc");
require_once("../util/database.inc");

class ExportModel
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
	function __destruct(){
        if($this->pri_db_conn!=NULL){
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
	

    function count($date_type,$date_value,$op_type,$op_value,$num){
        switch($op_type){
        case 'word':
            $sql = "select count(uid) from ".WORD_DB.".worduid_".$date_type."_tb where tid=$date_value and word='$op_value' and num>=$num";
            break;
        case 'majorcate':
            $sql = "select count(uid) from ".SITES_DB.".majorcateuid_".$date_type."_tb where tid=$date_value and majorcate='$op_value' and num>=$num";
            break;
        case 'subcate':
            $sql = "select count(uid) from ".SITES_DB.".subcateuid_".$date_type."_tb where tid=$date_value and subcate='$op_value' and num>=$num";
            break;
        case 'domain':
            $sql = "select count(uid) from ".SITES_DB.".domainuid_".$date_type."_tb where tid=$date_value and domain='$op_value' and num>=$num";
            break;
        case 'host':
            $sql = "select count(uid) from ".SITES_DB.".hostuid_".$date_type."_tb where tid=$date_value and host='$op_value' and num>=$num";
            break;
        }
        try{
            $result = db_query($sql,$this->pri_db_conn);
            $ret_arr =array();
            $row = $result->fetch_row();
            $ret_arr["success"] =true;
            $ret_arr["results"] = intval($row[0]);
            db_result_free($result);
            return $ret_arr;
        }
        catch(Exception $exp)
        {	
            $ret_arr =array();
            $ret_arr["success"] = false;
            $ret_arr["msg"] = $exp->getMessage();
            echo $exp->getMessage();
            return $ret_arr;
        }
    }

    function export($date_type,$date_value,$op_type,$op_value,$num){
        $dir = "/var/www/htdocs/webana/";
        $file = "export/uid_".$date_type."_".$date_value."_".$op_value."_"."$num";
        $filename = $dir.$file;
        switch( $op_type ){
        case 'word':
            $sql = "select uid into outfile '$filename' from ".WORD_DB.".worduid_".$date_type."_tb where tid=$date_value and word='$op_value' and num>=$num";
            break;
        case 'majorcate':
            $sql = "select uid into outfile '$filename' from ".SITES_DB.".majorcateuid_".$date_type."_tb where tid=$date_value and majorcate='$op_value' and num>=$num";
            break;
        case 'subcate':
            $sql = "select uid into outfile '$filename' from ".SITES_DB.".subcateuid_".$date_type."_tb where tid=$date_value and subcate='$op_value' and num>=$num";
            break;
        case 'domain':
            $sql = "select uid into outfile '$filename' from ".SITES_DB.".domainuid_".$date_type."_tb where tid=$date_value and domain='$op_value' and num>=$num";
            break;
        case 'host':
            $sql = "select uid into outfile '$filename' from ".SITES_DB.".hostuid_".$date_type."_tb where tid=$date_value and host='$op_value' and num>=$num";
            break;
        }
        try{
            $result = db_query($sql,$this->pri_db_conn);
            return $file;
        }
        catch(Exception $exp){	
            echo $exp->getMessage();
            return NULL;
        }
    }
}
?>
