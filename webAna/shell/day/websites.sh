#!/bin/bash
# Function:website day statistic about domain,host,url
# 2009-10-30:[Yan Xiaohui]created

cates_day_proc(){
	echo "[Info] $day:weblog_notdropme -> $opath/[major|sub]cates"
    hive_proc "
set mapred.reduce.tasks = 1;
from hostcate_notdropme a join 
(select host,count(1) as num 
from weblog_notdropme 
where dt='$day' and city='$city'
group by host) b
on ( a.host = b.host )
    insert overwrite local directory '$opath/subcates'
    select a.sub,sum(b.num) as s1,'$day','$city'
    group by a.sub
    sort by s1 desc
    insert overwrite local directory '$opath/majorcates'
    select a.major,sum(b.num) as s2,'$day','$city'
    group by a.major
    sort by s2 desc;"
}

cateuid_day_proc(){
	echo "[Info] $day:weblog_notdropme -> $opath/cateuid"
    hive_proc "
set mapred.reduce.tasks = 1;
from hostcate_notdropme a join 
(select host,uid
from weblog_notdropme 
where dt='$day' and city='$city'
group by host,uid) b
on ( a.host = b.host )
    insert overwrite local directory '$opath/subcateuid'
    select a.sub,count(distinct b.uid) as s1,'$day','$city'
    group by a.sub
    sort by s1 desc

    insert overwrite local directory '$opath/majorcateuid'
    select a.major,count(distinct b.uid) as s2,'$day','$city'
    group by a.major
    sort by s2 desc;"
}

cateuip_day_proc(){
	echo "[Info] $day:weblog_notdropme -> $opath/cateuip"
    hive_proc "
set mapred.reduce.tasks = 1;
from hostcate_notdropme a join 
(select host,uip
from weblog_notdropme 
where dt='$day' and city='$city'
group by host,uip) b
on ( a.host = b.host )
    insert overwrite local directory '$opath/subcateuip'
    select a.sub,count(distinct b.uip) as s1,'$day','$city'
    group by a.sub
    sort by s1 desc
    insert overwrite local directory '$opath/majorcateuip'
    select a.major,count(distinct b.uip) as s2,'$day','$city'
    group by a.major
    sort by s2 desc;"
}

domains_day_proc(){
	echo "[Info] $day:weblog_notdropme -> $opath/domains"
    hive_proc "
set mapred.reduce.tasks = 1;
insert overwrite local directory '$opath/domains'
select domain,count(1) as num,'$day','$city' from 
weblog_notdropme
where dt='$day' and city='$city'
group by domain
sort by num desc;"
}

domainuid_day_proc(){
	echo "[Info] $day:weblog_notdropme -> $opath/domainuid"
    hive_proc "
set mapred.reduce.tasks = 1;
insert overwrite local directory '$opath/domainuid'
select domain,count(distinct uid) as num,'$day','$city' 
from weblog_notdropme
where dt='$day' and city='$city'
group by domain
sort by num desc;"
}

domainuip_day_proc(){
	echo "[Info] $day:weblog_notdropme -> $opath/domainuip"
    hive_proc "
set mapred.reduce.tasks = 1;
insert overwrite local directory '$opath/domainuip'
select domain,count(distinct uip) as num,'$day','$city' 
from weblog_notdropme
where dt='$day' and city='$city'
group by domain
sort by num desc;"
}

hosts_day_proc(){
	echo "[Info] $day:weblog_notdropme -> $opath/hosts"
    hive_proc "
set mapred.reduce.tasks = 1;
insert overwrite local directory '$opath/hosts'
select host,count(1) as num,'$day','$city' from weblog_notdropme
where dt='$day' and city='$city'
group by host
sort by num desc;"
}

hostuid_day_proc(){
	echo "[Info] $day:weblog_notdropme -> $opath/hostuid"
    hive_proc "
set mapred.reduce.tasks = 1;
insert overwrite local directory '$opath/hostuid'
select host,count(distinct uid) as num,'$day','$city' from weblog_notdropme
where dt='$day' and city='$city'
group by host
sort by num desc;"
}

hostuip_day_proc(){
	echo "[Info] $day:weblog_notdropme -> $opath/hostuip"
    hive_proc "
set mapred.reduce.tasks = 1;
insert overwrite local directory '$opath/hostuip'
select host,count(distinct uip) as num,'$day','$city' from weblog_notdropme
where dt='$day' and city='$city'
group by host
sort by num desc;"
}

urls_day_proc(){
	echo "[Info] $day:weblog_notdropme -> $opath/urls"
    hive_proc "
set mapred.reduce.tasks = 1;
insert overwrite local directory '$opath/urls'
select url,count(1) as num,'$day','$city' from weblog_notdropme
where dt='$day' and city='$city'
group by url
sort by num desc;"
}

urluid_day_proc(){
	echo "[Info] $day:weblog_notdropme -> $opath/urluid"
    hive_proc "
set mapred.reduce.tasks = 1;
insert overwrite local directory '$opath/urluid'
select url,count(distinct uid) as num,'$day','$city' from weblog_notdropme
where dt='$day' and city='$city'
group by url
sort by num desc;"
}

urluip_day_proc(){
	echo "[Info] $day:weblog_notdropme -> $opath/urluip-------"
    hive_proc "
set mapred.reduce.tasks = 1;
insert overwrite local directory '$opath/urluip'
select url,count(distinct uip) as num,'$day','$city' from weblog_notdropme
where dt='$day' and city='$city'
group by url
sort by num desc;"
}