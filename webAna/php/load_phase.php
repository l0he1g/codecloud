<?php
  //function: 加载周数据
  //Useage: php -f load_day.php week 200911
  //2009-11-2: created by yanxiaohui
if( $argc<3 ){
	exit("Useage:php -f $agrv[0] <type> <type_id>\n");
}
$phase = $argv[1];
$phase_id = $argv[2];

define('FILE_PATH',"/mysql/source/webAna/input/$phase/$phase_id");//上传的数据文件的路径
require_once 'mysql.inc';

/*phase_proc1( db varchar(16),kind varchar(10),type varchar(5),fpath varchar(100))
 *  单项数据加载:majorcate,subcate,domain,host,url
 */
function load_phase_1( $kind,$phase,$db,$phase_id ){
	$dir = FILE_PATH."/${kind}s";//要导入的数据文件名
	$tb_name = "${db}.${kind}s_${phase}_tb";
	$loadsql = "load data infile '%%fpath%%' ignore into table $tb_name fields terminated by 0x01 ($kind,num,tid,city);";
	$procsql = "alter table $tb_name add partition( partition p$phase_id values in ($phase_id));";
	processDataFileToDB($loadsql,$procsql,$dir);
}

/*
 *  双项数据加载:majorcateuid,subcateuip,domainhost,hostuid,urluid...
 */
function load_phase_2( $kind1,$kind2,$phase,$db,$phase_id ){
	$dir = FILE_PATH."/${kind1}${kind2}";//要导入的数据文件目录
	$tb_name = "${db}.${kind1}${kind2}_${phase}_tb";
	$loadsql = "load data infile '%%fpath%%' ignore into table $tb_name fields terminated by 0x01 ($kind1,$kind2,num,tid,city);";
	$procsql = "alter table $tb_name add partition( partition p$phase_id values in ($phase_id));";
	processDataFileToDB($loadsql,$procsql,$dir);
}

/*
 *  指定host的搜索统计数据文件加载:hostse hostse_proc( fpath varchar(100),type varchar(5))
 */
function load_phase_hostse($phase,$db,$phase_id ){
	$dir = FILE_PATH."/hostse";//要导入的数据文件名
	$tb_name = "${db}.hostse_${phase}_tb";
	$loadsql = "load data infile '%%fpath%%' ignore into table $tb_name fields terminated by 0x01 (host,word,engine,num,tid,city);";
	$procsql = "alter table $tb_name add partition( partition p$phase_id values in ($phase_id));";
	processDataFileToDB($loadsql,$procsql,$dir);
}

# 执行导入
function main(){
	global $day;
	global $phase;
	global $phase_id;
    ############ keywords ###########
    load_phase_1( 'word',$phase,WORD_DB,$phase_id );
    load_phase_2( 'word','uid',$phase,WORD_DB,$phase_id );
    load_phase_2( 'word','domain',$phase,WORD_DB,$phase_id );
    load_phase_2( 'word','host',$phase,WORD_DB,$phase_id );
    load_phase_2( 'word','url',$phase,WORD_DB,$phase_id );
    ########### websites ###########
    //1.分类访问排名部分数据导入
	load_phase_1( 'majorcate',$phase,SITES_DB,$phase_id );
	load_phase_2( 'majorcate','uid',$phase,SITES_DB,$phase_id );
	load_phase_2( 'majorcate','domain',$phase,SITES_DB,$phase_id );
	load_phase_2( 'majorcate','host',$phase,SITES_DB,$phase_id );
	load_phase_2( 'majorcate','url',$phase,SITES_DB,$phase_id );
	load_phase_1( 'subcate',$phase,SITES_DB,$phase_id );
	load_phase_2( 'subcate','uid',$phase,SITES_DB,$phase_id );
	load_phase_2( 'subcate','domain',$phase,SITES_DB,$phase_id );
	load_phase_2( 'subcate','host',$phase,SITES_DB,$phase_id );
	load_phase_2( 'subcate','url',$phase,SITES_DB,$phase_id );
	//2.domain访问排名数据导入
	load_phase_1( 'domain',$phase,SITES_DB,$phase_id );
	load_phase_2( 'domain','uid',$phase,SITES_DB,$phase_id );
	load_phase_2( 'domain','host',$phase,SITES_DB,$phase_id );
	load_phase_2( 'domain','url',$phase,SITES_DB,$phase_id );
	//3.host访问排名数据导入
	load_phase_1( 'host',$phase,SITES_DB,$phase_id );
	load_phase_2( 'host','uid',$phase,SITES_DB,$phase_id );
	load_phase_2( 'host','url',$phase,SITES_DB,$phase_id );
    ########### websites ###########
	//hostrefer数据
	load_phase_2( 'host','refer',$phase,HOST_DB,$phase_id );
	load_phase_hostse($phase,HOST_DB,$phase_id);
  	printf("Task is done successedly:)");
}

main();
?>
