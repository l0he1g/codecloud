#!/bin/bash
# Function:specified host day statistic about up/down stream,search 
# 2009-10-30:[Yan Xiaohui]created

hostrefer_day_proc(){
    echo "[Info]$day:weblog_notdropme -> hostrefer_notdropme"
	hive_proc "
insert overwrite table hostrefer_notdropme partition(dt='$day',city='$city')
select tb.host,tb.refer_host,tb.num 
from(
    select host,parse_url(refer,'HOST') as refer_host,count(1) as num
    from weblog_notdropme
    where dt='$day' and city='$city' and refer is not null and refer<>''
    group by host,parse_url(refer,'HOST') ) tb 
where tb.host<>tb.refer_host 
 cluster by host;"
    echo "[Info]$day:hostrefer_notdropme -> $opath/hostrefer"
	hive_proc "
set mapred.reduce.tasks = 1;
insert overwrite local directory '$opath/hostrefer'
select * from hostrefer_notdropme
where dt='$day' and city='$city'
sort by num desc;"
}

hostse_day_proc(){
    echo "[Info]$day:weblog_notdropme -> hostse_notdropme"
    hive_proc "
add file $hostse_jar;
from(
   from weblog_notdropme
   select transform( host,refer )
   using 'java -Dfile.encoding=UTF-8 -jar `basename $hostse_jar`'
   as host,word,engine
   where dt='$day' and city='$city'
   cluster by host
 )maptb
insert overwrite table hostse_notdropme
partition( dt='$day',city='$city')
select host,word,engine,count(1) as num
where word IS NOT NULL and engine IS NOT NULL
group by host,word,engine;"
    echo "[Info]$day:hostse_notdropme -> $opath/hostse"
	hive_proc "
set mapred.reduce.tasks = 1;
insert overwrite local directory '$opath/hostse'
select * from hostse_notdropme
where dt='$day' and city='$city'
sort by num desc;"
}