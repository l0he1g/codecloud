#!/bin/bash
# Function:website phase statistic about domain,host,url
# 2009-10-30:[Yan Xiaohui]created
cates_phase_proc(){
	echo "[Info]$phase $phase_id:weblog_notdropme -> $opath/[major|sub]cates"
    hive_proc "
set mapred.reduce.tasks = 1;
from hostcate_notdropme a join 
(select host,count(1) as num 
from weblog_notdropme 
where dt>='$start_day' and dt<='$end_day' and city='$city'
group by host) b
on ( a.host = b.host )
    insert overwrite local directory '$opath/subcates'
    select a.sub,sum(b.num) as s1,'$phase_id','$city'
    group by a.sub
    sort by s1 desc
    insert overwrite local directory '$opath/majorcates'
    select a.major,sum(b.num) as s2,'$phase_id','$city'
    group by a.major
    sort by s2 desc;"
}

cateuid_phase_proc(){
	echo "[Info]$phase $phase_id:weblog_notdropme -> $opath/cateuid"
    hive_proc "
set mapred.reduce.tasks = 1;
from hostcate_notdropme a join 
(select host,uid,count(1) as num
from weblog_notdropme 
where dt>='$start_day' and dt<='$end_day' and city='$city' 
group by host,uid) b
on ( a.host = b.host )
    insert overwrite local directory '$opath/subcateuid'
    select a.sub,b.uid,sum(b.num) as s1,'$phase_id','$city'
    group by a.sub,b.uid
    sort by s1 desc
    insert overwrite local directory '$opath/majorcateuid'
    select a.major,b.uid,sum(b.num) as s2,'$phase_id','$city'
    group by a.major,b.uid
    sort by s2 desc;"
}

catedomain_phase_proc(){
	echo "[Info]$phase $phase_id:weblog_notdropme -> $opath/catedomain"
    hive_proc "
set mapred.reduce.tasks = 1;
from hostcate_notdropme a join 
(select host,domain,count(1) as num 
from weblog_notdropme 
where dt>='$start_day' and dt<='$end_day' and city='$city' 
group by host,domain) b
on ( a.host = b.host )
    insert overwrite local directory '$opath/subcatedomain'
    select a.sub,b.domain,sum(b.num) as s1,'$phase_id','$city'
    group by a.sub,b.domain
    sort by s1 desc
    insert overwrite local directory '$opath/majorcatedomain'
    select a.major,b.domain,sum(b.num) as s2,'$phase_id','$city'
    group by a.major,b.domain
    sort by s2 desc;"
}

catehost_phase_proc(){
	echo "[Info]$phase $phase_id:weblog_notdropme -> $opath/catehost"
    hive_proc "
set mapred.reduce.tasks = 1;
from hostcate_notdropme a join 
(select host,count(1) as num
from weblog_notdropme 
where dt>='$start_day' and dt<='$end_day' and city='$city' 
group by host) b
on ( a.host = b.host )
    insert overwrite local directory '$opath/subcatehost'
    select a.sub,a.host,sum(b.num) as s1,'$phase_id','$city'
    group by a.sub,a.host
    sort by s1 desc
    insert overwrite local directory '$opath/majorcatehost'
    select a.major,a.host,sum(b.num) as s2,'$phase_id','$city'
    group by a.major,a.host
    sort by s2 desc;"
}

cateurl_phase_proc(){
	echo "[Info]$phase $phase_id:weblog_notdropme -> $opath/cateurl"
    hive_proc "
set mapred.reduce.tasks = 1;
from hostcate_notdropme a join 
(select host,url,count(1) as num
from weblog_notdropme 
where dt>='$start_day' and dt<='$end_day' and city='$city' 
group by host,url) b
on ( a.host = b.host )
    insert overwrite local directory '$opath/subcateurl'
    select a.sub,b.url,sum(b.num) as s1,'$phase_id','$city'
    group by a.sub,b.url
    sort by s1 desc
    insert overwrite local directory '$opath/majorcateurl'
    select a.major,b.url,sum(b.num) as s2,'$phase_id','$city'
    group by a.major,b.url
    sort by s2 desc;"
}
########### domain ##################
domains_phase_proc(){
	echo "[Info]$phase $phase_id:weblog_notdropme -> $opath/domains"
    hive_proc "set mapred.reduce.tasks = 1;
insert overwrite local directory '$opath/domains'
select domain,count(1) as num,'$phase_id','$city' from weblog_notdropme
where dt>='$start_day' and dt<='$end_day' and city='$city' 
group by domain
sort by num desc;"
}

domainuid_phase_proc(){
	echo "[Info]$phase $phase_id:weblog_notdropme -> $opath/domainuid"
    hive_proc "set mapred.reduce.tasks = 1;
insert overwrite local directory '$opath/domainuid'
select domain,uid,count(1) as num,'$phase_id','$city' from weblog_notdropme
where dt>='$start_day' and dt<='$end_day' and city='$city' 
group by domain,uid
sort by num desc ;"
}

domainhost_phase_proc(){
	echo "[Info]$phase $phase_id:weblog_notdropme -> $opath/domainhost"
    hive_proc "set mapred.reduce.tasks = 1;
insert overwrite local directory '$opath/domainhost'
select domain,host,count(1) as num,'$phase_id','$city' from weblog_notdropme
where dt>='$start_day' and dt<='$end_day' and city='$city' 
group by domain,host
sort by num desc ;"
}

domainurl_phase_proc(){
	echo "[Info]$phase $phase_id:weblog_notdropme -> $opath/domainurl"
    hive_proc "set mapred.reduce.tasks = 1;
insert overwrite local directory '$opath/domainurl'
select domain,url,count(1) as num,'$phase_id','$city' from weblog_notdropme
where dt>='$start_day' and dt<='$end_day' and city='$city'
group by domain,url
sort by num desc ;"
}

################# host #####################
hosts_phase_proc(){
	echo "[Info]$phase $phase_id:weblog_notdropme -> $opath/hosts"
    hive_proc "set mapred.reduce.tasks = 1;
insert overwrite local directory '$opath/hosts'
select host,count(1) as num,'$phase_id','$city' from weblog_notdropme
where dt>='$start_day' and dt<='$end_day' and city='$city' 
group by host
sort by num desc ;"
}

hostuid_phase_proc(){
	echo "[Info]$phase $phase_id:weblog_notdropme -> $opath/hostuid"
    hive_proc "set mapred.reduce.tasks = 1;
insert overwrite local directory '$opath/hostuid'
select host,uid,count(1) as num,'$phase_id','$city' from weblog_notdropme
where dt>='$start_day' and dt<='$end_day' and city='$city' 
group by host,uid
sort by num desc;"
}

hosturl_phase_proc(){
	echo "[Info]$phase $phase_id:weblog_notdropme -> $opath/hosturl"
    hive_proc "set mapred.reduce.tasks = 1;
insert overwrite local directory '$opath/hosturl'
select host,url,count(1) as num,'$phase_id','$city' from weblog_notdropme
where dt>='$start_day' and dt<='$end_day' and city='$city' 
group by host,url
sort by num desc ;"
}
