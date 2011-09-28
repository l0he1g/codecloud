<?php
  //function: 每天加载hive处理后的天数据到mysql
  //Useage: php -f load_day.php 20091102
  //2009-11-2: created by yanxiaohui
if( $argc<2 ){
	exit("Useage:php -f $agrv[0] <date>\n");
}
$day = $argv[1];

define('FILE_PATH',"/mysql/source/webAna/input/day/$day");//上传的数据文件的路径
require_once 'mysql.inc';

/**
 * 加载每天的单项访问排名数据文件到mysql,
 *  majorcate,subcate,domain,host,url
 */
function load_day_1( $kind,$db,$day ){
	$dir = FILE_PATH."/${kind}s";//要导入的数据文件名
	$tb_name = "${db}.${kind}s_day_tb";
	$loadsql = "load data infile '%%fpath%%' ignore into table $tb_name fields terminated by 0x01 ($kind,num,tid,city);";
	$procsql = "alter table $tb_name add partition( partition p$day values in ($day));";
	processDataFileToDB($loadsql,$procsql,$dir);
}

/**
 * 加载每天的双项访问排名数据文件到mysql,包括:
 *  majorcateuid,majorcateuip,majorcatedomain,...
 * 组装数据目录名，sql语句模板
 */
function load_day_2( $kind1,$kind2,$db,$day ){
	$dir = FILE_PATH."/${kind1}${kind2}";//要导入的数据文件目录
	$tb_name = "${db}.${kind1}${kind2}_day_tb";
	$loadsql = "load data infile '%%fpath%%' ignore into table $tb_name fields terminated by 0x01 ($kind1,num,tid,city);";
	$procsql = "alter table $tb_name add partition( partition p$day values in ($day));";
	processDataFileToDB($loadsql,$procsql,$dir);
}

// 执行导入
function main(){
	global $day;
  	printf("Load data for day:%s\n",$day);
    //////////////////////// words //////////////////////
    load_day_1( 'word',WORD_DB,$day);
    load_day_2( 'word','uid',WORD_DB,$day);
    load_day_2( 'word','uip',WORD_DB,$day);
    //////////////////////// sites //////////////////////
    //1.分类访问排名部分天数据导入
	load_day_1( 'majorcate',SITES_DB,$day);
	load_day_2( 'majorcate','uid',SITES_DB,$day);
	load_day_2( 'majorcate','uip',SITES_DB,$day);

	load_day_1( 'subcate' ,SITES_DB,$day);
	load_day_2( 'subcate','uid',SITES_DB,$day);
	load_day_2( 'subcate','uip',SITES_DB,$day);
	//2.domain访问排名数据导入
	 load_day_1( 'domain',SITES_DB,$day);
	load_day_2( 'domain','uid',SITES_DB,$day);
	load_day_2( 'domain','uip',SITES_DB,$day);
	//3.host访问排名数据导入
	load_day_1( 'host',SITES_DB,$day);
	load_day_2( 'host','uid',SITES_DB,$day);
	load_day_2( 'host','uip',SITES_DB,$day);
	//4.uid访问排名数据导入
	//////////////////////// uid //////////////////////
	load_day_1( 'uid' ,UID_DB,$day);
	load_day_2( 'uid','domain',UID_DB,$day);
	load_day_2( 'uid','host',UID_DB,$day);
  	printf("Task is done successedly:)");
}

/***** main ********/
main();

?>