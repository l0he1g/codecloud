<?php
// session_start();
// if(!isset($_SESSION["login"]) || $_SESSION["login"] !==true){
//     $_SESSION['login']=false;
//     header("Location:../view/login/login.php");
// }

require_once ("../model/exportModel.inc");
require_once ("../model/operateResultModel.inc");
require_once ("../util/util.inc");

$date_type = http_get('dt','month'); //month or week，时间周期类型
$date_value = http_get('dv','200910'); //时间周期的具体值
$op = http_get('op','0');//'0'表示查符合要求的用户id数目，'1'表示导出uid
$op_type = http_get('ot','word');//可以是word,majorcate,subcate,domain,host,表示查询哪一项
if( $op_type == 'cate' ){
    $website_cate = http_get('cate','博客日记');
    $webcate_arr = explode("/",$website_cate);
    $majorcate = http_get('mcate','博客日记');
    if(strcmp($webcate_arr[0],$webcate_arr[1])==0)
    {
        $op_type = 'majorcate';
        $op_value=$webcate_arr[1];
    }
    else
    {
        $op_type = 'subcate';
        $op_value=$webcate_arr[1];
    }
}
else{
    $op_value = trim( http_get('ov','游戏') );//查询项的值
}
$num = http_get('num',2);

//$ret = process($date_type,$date_value,$op,$op_type,$op_value,$num);

//$ret_datas = new OperateResultModel();
$mod = new ExportModel();

//统计符合要求的用户id数目,返回数值
if( $op=='0' ){
    $result = $mod->count($date_type,$date_value,$op_type,$op_value,$num);
    //echo $num;
    echo json_encode($result);
}
//导出uid文件,并直接打开文件链接
else if( $op == '1' ){
    $path = $mod->export($date_type,$date_value,$op_type,$op_value,$num);
    // 打开导出文件
    if( $path ){
        $path = "../".$path;
        header("Location:$path");
    }
    else{
        $result = array('success'=>false,'msg'=>'see log');
        echo json_encode($result);
    }
}
?>
