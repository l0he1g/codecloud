<?php

/**
* 说明：封装了数据库的连接。
* 修改日期：2009-4-13
* 作者：陈文斌,李峰
*/


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
function db_connect($pin_server,$pin_user,$pin_pwd,$pin_db,$pin_port,$pin_socket)
{
	$conn = new mysqli($pin_server, $pin_user,$pin_pwd, $pin_db,$pin_port,$pin_socket);
	if (mysqli_connect_errno())
	{
    	throw new Exception("Error: Could not connect database(".$pin_db.")\n");
	}
	$conn->query("SET NAMES ".CLIENT_CHARSET);
	return $conn;
}
/**
 * 关闭数据库连接
 * @param mysqli 数据库连接句柄
 */
function db_connect_close($pin_conn)
{
	$pin_conn->close();
}

function select_db($pin_db,$pin_conn)
{
	return $pin_conn->select_db($pin_db);
}

/**
 * 执行查询操作.
 * @param String $pin_loaddatainsql 加载数据文件到数据表中的操作sql
 * @param String $pin_conn 数据库连接句柄
 * @return 正常不返回任何值，除非出现错误会抛出异常
 */
function db_query($pin_sql,$pin_conn)
{
	if($result = $pin_conn->query($pin_sql))
	{
		return $result;
	}
	else
	{
		throw new Exception("db execute query error: ".$pin_conn->error."\n");
	}
}

/**
 * 执行更新操作.
 * @param String $pin_loaddatainsql 加载数据文件到数据表中的操作sql
 * @param String $pin_conn 数据库连接句柄
 * @return 正常返回TRUE，否则出现错误会抛出异常
 */
function db_update($pin_updatesql,$pin_conn)
{
	if($result = $pin_conn->query($pin_updatesql))
	{
		return $result;
	}
	else
	{
		throw new Exception("db execute query error: ".$pin_conn->error."\n");
	}
}

/**
 * 执行更新操作.
 * @param String $pin_loaddatainsql 加载数据文件到数据表中的操作sql
 * @param String $pin_conn 数据库连接句柄
 * @return 正常返回TRUE，否则出现错误会抛出异常
 */
function db_insert($pin_insertsql,$pin_conn)
{
	if($result = $pin_conn->query($pin_insertsql))
	{
		return $result;
	}
	else
	{
		throw new Exception("db execute query error: ".$pin_conn->error."\n");
	}
}

/**
 * 执行更新操作.
 * @param String $pin_loaddatainsql 加载数据文件到数据表中的操作sql
 * @param String $pin_conn 数据库连接句柄
 * @return 正常返回TRUE，否则出现错误会抛出异常
 */
function db_delete($pin_deletesql,$pin_conn)
{
	if($result = $pin_conn->query($pin_deletesql))
	{
		return $result;
	}
	else
	{
		throw new Exception("db execute query error: ".$pin_conn->error."\n");
	}
}
/**
 * 释放结果集合.
 * @param Result $pin_result 要释放的结果集合
 */
function db_result_free($pin_result)
{
	$pin_result->close();
}

function db_queryresult_rows($pin_result)
{
	if($pin_result==NULL)
	{
		return 0;
	}
	$rows = $pin_result->num_rows;
	return $rows;
}

function db_affected_rows($pin_conn)
{
	if($pin_conn==NULL)
	{
		return 0;
	}
	$rows = $pin_conn->affected_rows;
	return $rows;
}
?>
