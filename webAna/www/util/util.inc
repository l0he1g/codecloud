<?php
# @para1 time_type week或month
# @para2 time_value 月份
# retrun:时间段的起始日期和终止日期
# 只保留上周功能，本周查询仅用
require_once("../util/logger.inc");
# 获取http的get或post参数，如果不存在，取默认值
function http_get($name, $default = '')
{
	global $_REQUEST;
	if( isset($_REQUEST[$name]))
		return $_REQUEST[$name];
	return $default;
}

function get_day( $type,$value )
{
	if( $type == 'week' )
	{
		$inweek = date('N');//昨天星期几
		if( strcmp("$value","this")==0 )
		{
			//禁用
			logger_print("s;akf");
			$end = date('Ymd');
			$start = date('Ymd',mktime(0,0,0,date('m'),
			date('d')-${inweek},date('Y')) );//This Monday
		}
		elseif( $value == 'last' )
		{
			$end = date('Ymd',mktime(0,0,0,date('m'),date('d')-$inweek,date('Y')) );//Last Sunday
			$start = date('Ymd',mktime(0,0,0,date('m'),date('d')-$inweek-6,date('Y')) );//Last Monday
		}
		else
		{
			$todayofweek = date("YW");
			$y = intval( substr($value,0,4) );
			$w = intval( substr($value,4,2) );
			
			$today_y = intval( substr($todayofweek,0,4) );
			$today_w = intval( substr($todayofweek,4,2) );
			
			$w_diff = $today_w - $w;
			$y_diff = $today_y - $y;
			$end = date('Ymd',mktime(0,0,0,date('m'),date('d')-$inweek-$w_diff*7+7,date('Y')-$y_diff) );//Last Sunday
			$start = date('Ymd',mktime(0,0,0,date('m'),date('d')-$inweek-6-$w_diff*7+7,date('Y')-$y_diff) );//Last Monday
		
		}
	}
	elseif( $type == 'month' )
	{
		if( $value == 'this' )
		{
			$end = date('Ymd'); 
			$start = date('Ymd',mktime(0,0,0,date('m'),1,date('Y')) );//This 1st
		}
		elseif( $value == 'last' )
		{
			$end = date('Ymd',mktime(0,0,0,date('m'),0,date('Y')) );//last day of month
			$start = date('Ymd',mktime(0,0,0,date('m')-1,1,date('Y')) );//Last 1st
		}
		else
		{
			# 指定月份
			$y = intval( substr($value,0,4) );
			$m = intval( substr($value,4,2) );
			$end = date('Ymd',mktime(0,0,0,$m+1,0,$y) );//last day of month
			$start = date('Ymd',mktime(0,0,0,$m,1,$y) );//Last 1st
		}

	}
	else
	{
		logger_print( "wrong:$type,$value" );
		return false;
	}
	return array( 'start_day'=>$start,'end_day'=>$end );
}

function get_phase_id( $type,$value )
{
	$id;
	if( $type == 'week' )
	{
		if( $value == 'this' )
		{
			
			$id = date('YW');
		}
		elseif( $value == 'last' )
		{
			$id = date( 'YW',mktime(0,0,0,date('m'),date('d')-7,date('Y')) );
		}
		else
		{
			logger_print( "wrong:$type,$value" );
			return false;
		}
	}
	elseif( $type == 'month' )
	{
		if( $value == 'this' )
		{
			$id = date('Ym');
		}
		elseif( $value == 'last' )
		{
			$id = date( 'Ym',mktime(0,0,0,date('m')-1,1,date('Y')) );
		}
		else
		{
			# 指定月份
			$id = $value;
		}
	}
	else
	{
		logger_print( "wrong:$type,$value" );
		return false;
	}
	return $id;
}
?>
