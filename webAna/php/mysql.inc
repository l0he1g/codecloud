<?php
define('SERVER', "localhost");//数据库IP
define('PORT', "3306");//数据库PORT
define('SOCKET', "/mysql/3306/mysql.sock");//连接数据库的socket
//define('SOCKET', NULL);//连接数据库的socket
define('USER', "hpclouduser");//连接数据库的用户名
define('PWD', "hpcloud");//连接数据库的密码
define('CLIENT_CHARSET',"UTF8");//连接数据库客户端的字符集合

define('HOST_DB', "wahostdb");//单个网站流量和搜索统计数据库
define('SITES_DB', "wasitesdb");//网站分类排名数据库
define('WORD_DB', "waworddb");//keywords数据库
define('UID_DB', "wauiddb");//服务数据库
define('SRV_DB', "wasrvdb");//服务数据库
/**
 * 连接数据库，设置连接客户端的字符集合为默认字符集
 * @param String $pin_server IP
 * @param int $pin_user 用户名
 * @param String $pin_pwd 密码
 * @param String $pin_db 连接的数据库db
 * @param String $pin_port 端口号
 * @param String $pin_socket 使用的客户端连接socket
 * @return Connetion 连接成功后会返回数据库连接句柄，否则出现连接失败会抛出异常
 */
function db_connect($db)
{
	$conn = new mysqli(SERVER, USER,PWD, $db,PORT,SOCKET);
	if (mysqli_connect_errno())
	{
		exit("[Error]: Could not connect database(".$db.")\n");
	}
	$conn->query("SET NAMES ".CLIENT_CHARSET);
	return $conn;
}

function db_exec($loaddatainsql,$conn)
{
	if(!$conn->query($loaddatainsql))
	{
		throw new Exception("load data in db execute error: ".$conn->error."\n");
	}
}

/**
 * 执行不需要对外返回结果数据集合的存储过程执行操作
 * @param String $pin_procsql 存储过程调用的sql
 * @param String $pin_conn 数据库连接
 * @return 正常不返回任何值，除非出现错误会抛出异常
 */
function db_callproc_noresult($pin_procsql,$mysqli )
{
	/*执行支持多结果返回的查询*/
	$i = 0;
	if($mysqli->multi_query($pin_procsql))
	{
		do
		{
			$i = $i + 1;
			/* store first result set */
			if ($result = $mysqli->store_result())
			{
				while ($row = $result->fetch_row())
				{
					printf("\n%d. %s\n", $i,$row[0]);
				}
				$result->close();
			}
			/* print divider */
			if ($mysqli->more_results())
			{
				printf("-----------------------");
			}
		} while ($mysqli->next_result());
	}
	else
	{
		throw new Exception("call procedure execute error: ".$mysqli->error."\n");
	}
	
}

/**
 * 处理每天的数据文件，并加载到相应的数据库中
 * @param String $db 数据库
 * @param String $loaddatasql 加载数据文件中数据的sql
 * @param String $fpath 数据文件存储目录
 * @return 正常不返回任何值，除非出现错误会抛出异常
 */
function processDataFileToDB($loadsql,$procsql,$dir)
{
	$begin = time();
    /*判断给定目录是否是目录*/
	if(!is_dir($dir)){
		printf("no data file in %s",$dir);
  		return;
  	}
  	/*得到目录中的表示每天的数据文件*/
	$filelist = &files_list($dir);
	$file_count = count($filelist);
	if($file_count<=0){
		return;
	}
	/*连接数据库得到连接句柄*/
	$mysqli = db_connect(SRV_DB);

  	printf("\n---------------------Start at:%s------------------------\n",date("Y-m-d H:i:s"));
	try{
    	printf("%s\n",$procsql);
		printf("-----------------------\n");
		db_exec( $procsql,$mysqli );
	}
	catch(Exception $exp){
		printf("######################### [Failed!] ####################\n");
		printf("%s",$exp->getMessage());
		exit(-1);
	}
	/*将指定目录中每天的数据文件加载并处理到数据库中的相应数据表中*/
	for($i=0;$i<$file_count;$i=$i+1)
	{
		/*每天的数据文件含有目录全称*/
		$file_fullname = $dir."/".$filelist[$i];
		if(is_dir($file_fullname) || $filelist[$i][0]=='.' ){
	  		continue;
	  	}
	  	$load_sql = str_replace("%%fpath%%",$file_fullname,$loadsql);
	  	/*将每天数据的数据文件加载到相应数据库的每天的数据表中*/
		try{
    		printf("%s\n",$load_sql);
			db_exec( $load_sql,$mysqli );
		}
		catch(Exception $exp){
			printf("######################### [Failed!] ####################\n");
			printf("%s",$exp->getMessage());
			exit(-1);
		}
	}

  	db_close($mysqli);
	$last=date('H:i:s',mktime(0,0,time()-$begin,1,1,1970));
  	printf("-------------------------Successed end:%s(%s)------------------------\n",date("Y-m-d H:i:s"),$last);
}

/**
 * 关闭数据库连接
 * @param mysqli 数据库连接句柄
 */
function db_close($conn)
{
	$conn->close();
}

/**
 * 得到指定文件目录中的文件名称数组。
 * @param String $directory 指定的文件目录
 * @return Array 文件目录中的文件名称数组。如果该文件目录不是目录则返回false。
 */
function files_list($directory)
{
	if (is_dir($directory))
	{//是文件目录得到目录中的文件名称数组
		$filelist = scandir($directory);//得到文件目录中的文件名称数组
		return $filelist;		
	}
	return false;
}
?>
