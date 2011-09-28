#!/bin/bash
# Function:website day statistic about domain,host,url
# 2009-10-30:[Yan Xiaohui]created

uids_day_proc(){
	echo "[Info]day $day:weblog_notdropme -> $opath/uids"
    hive_proc "
set mapred.reduce.tasks = 1;
insert overwrite local directory '$opath/uids'
select uid,count(1) as num,'$day','$city' 
from weblog_notdropme
where dt='$day' and city='$city'
group by uid
sort by num desc;"
}
uiddomain_day_proc(){
	echo "[Info]day $day:weblog_notdropme -> $opath/uiddomain"
    hive_proc "
set mapred.reduce.tasks = 1;
insert overwrite local directory '$opath/uiddomain'
select uid,count(distinct domain) as num,'$day','$city' 
from weblog_notdropme
where dt='$day' and city='$city'
group by uid
sort by num desc;"
}

uidhost_day_proc(){
    #用户关注最多的host
	echo "[Info]day $day:weblog_notdropme -> $opath/uidhost"
    hive_proc "
set mapred.reduce.tasks = 1;
insert overwrite local directory '$opath/uidhost'
select uid,count(distinct host) as num,'$day','$city' from weblog_notdropme
where dt='$day' and city='$city'
group by uid
sort by num desc;"
}