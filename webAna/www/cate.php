<?php
  /** majordate.php
   * 功能:用户数据分析展示
   * GET参数：
   *     majorcate:'string'
   *     time_type:'week'/'month',默认为week
   *     time_value:'this'/'last'/'200910',默认为last
   * 返回数据格式:
   *返回数据格式：
   {'success':true,rows:[{daydata:[{date:'2009-10-13',visits_num:13,uid_num:13:ip_num:13},......],topdomain:{chartStore:[{domain:'',rate:13},......],gridStore:[{domain:'',num:13},......]},tophost:{chartStore:[{host:'',rate:13},......],gridStore:[{host:'',num:13},......]},topurl:{chartStore:[{url:'',rate:13},......],gridStore:[{url:'',num:13},......]},topuserid:{chartStore:[{userid:'',rate:13},......],gridStore:[{userid:'',num:13},......]}}]}
   * @author xiaohui yan <yanxiaohui@software.ict.ac.cn>
   * @date 2009-11-02
   * @version 1.0
   */
require_once 'include/base.inc';
require_once 'include/fun.inc';

$time_type = http_get('time_type','week');
$time_value = http_get('time_value','last');
$days = array();
$days = get_day( $time_type,$time_value );
$phase_id = get_phase_id( $time_type,$time_value );

$cate = $_GET['cate'];
$items = explode('-',$cate);
$major = $items[0];
$sub = $items[1];

$amount = 50;
# 判断是查询主分类，还是子分类
if( $major == $sub ){
	$kind = 'majorcate';
}
else{
	$kind = 'subcate';
}

base_init();
# 1.分类访问排名天数据查询，一周或一月数据
$daydata = array();
foreach( $days as $day ){
	$arr = array();
	$arr['date']="$day";
	# 分类访问排名 
	$res = base_get_one("select num from ${kind}s_day_tb where dt='$day';");
	$arr['visits_num']=$res[0];
	# 获取分类访问用户id排名
	$res = base_get_one("select num from ${kind}uid_day_tb where dt='$day';");
	$arr['uid_num']=$res[0];
	# 获取分类访问用户ip排名
	$res = base_get_one("select num from ${kind}uip_day_tb where dt='$day';");
	$arr['uip_num']=$res[0];
	$daydata[]=$arr;
}
# 2.时间段查询--分类的domain访问排名
$topdomain = array();
$chartStore = array();
$gridStore = array();
sum = 0;
$res = base_get("select domain,num from ${kind}domain_${time_type}${phase_id} limit $amount;"); 
for( $res as $r ){
	$arr = array();
	$arr['domain'] = $r[0];
	$arr['num'] = $r[1];
	$sum += intval($r[1]);
	$gridStore[] = $arr;
}
for( $i=0;$i<min(9,count($res));$i++ ){
	$arr = array();
	$arr['domain'] = $r[0];
	$arr['rate'] = $r[1];
	$chartStore[] = $arr;	
}
$chartStore[]= array( 'domain'=>'other','rate'=>$remain );
$topdomain['chartStore'] = $chartStore;
$topdomain['gridStore'] = $gridStore;

# 3.时间段查询--分类的host访问排名
$tophost = array();
$chartStore = array();
$gridStore = array();
$res = base_get("select host,num from ${kind}host_${time_type}${phase_id} limit $amount;"); 
for( $res as $r ){
	$arr = array();
	$arr['host'] = $r[0];
	$arr['num'] = $r[1];
	$tophost[] = $r;
}
$tophost['chartStore'] = $chartStore;
$tophost['gridStore'] = $gridStore;

# 4.时间段查询--分类的url访问排名
$topurl = array();
$chartStore = array();
$gridStore = array();
$res = base_get("select url,num from ${kind}url_${time_type}${phase_id} limit $amount;"); 
for( $res as $r ){
	$arr = array();
	$arr['url'] = $r[0];
	$arr['num'] = $r[1];
	$topurl[] = $r;
}
$topurl['chartStore'] = $chartStore;
$topurl['gridStore'] = $gridStore;

# 5.时间段查询--分类的uid访问排名
$topuid = array();
$chartStore = array();
$gridStore = array();
$res = base_get("select uid,num from ${kind}uid_${time_type}${phase_id} limit $amount;"); 
for( $res as $r ){
	$arr = array();
	$arr['uid'] = $r[0];
	$arr['num'] = $r[1];
	$topuid[] = $arr;
}
$topuid['chartStore'] = $chartStore;
$topuid['gridStore'] = $gridStore;

# 构造返回结果
$ret = array();
if( count($daydata) ){
	$ret['success'] = 'true';
	$rows = array();
	$rows['daydata'] = $daydata;
	$rows['topdomain'] = $topdomain;
	$rows['tophost'] = $tophost;
	$rows['topurl'] = $topurl;
	$rows['topuid'] = $topuid;
	$ret['rows'] = $rows;
}
else{
	$ret['success'] = 'false';
	$ret['rows'] = null;
}

echo josn_encode( $ret );
?>