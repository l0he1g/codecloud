#!/bin/bash
# Function:specified keywords statistic about uid,uip,domain,host,url

# 统计时间段内访问关键词访问排名
words_phase_proc(){
    echo "$phase $phase_id:words_notdropme -> $opath/words"
    hive_proc "
set mapred.reduce.tasks = 1;
insert overwrite local directory '$opath/words' 
select word,sum(num) as s,'$phase_id','$city' 
from words_notdropme
where dt>='$start_day' and dt<='$end_day' and city='$city' 
group by word
sort by s desc;"
}

worddomain_phase_proc(){
    echo "[Info]$phase $phase_id:worddomain_notdropme -> $opath/worddomain"
    hive_proc "
set mapred.reduce.tasks = 1;
insert overwrite local directory '$opath/worddomain' 
select word,domain,sum(num) as domainnum,'$phase_id','$city' 
from worddomain_notdropme 
where dt>='$start_day' and dt<='$end_day' and city='$city' 
group by word,domain 
sort by domainnum desc;"
}

wordurl_phase_proc(){
    echo "[Info]$phase $phase_id:wordurl_notdropme -> $opath/wordurl"
    hive_proc "set mapred.reduce.tasks = 1;
insert overwrite local directory '$opath/wordurl'
select word,url,sum(num) as urlnum,'$phase_id','$city'
from wordurl_notdropme where dt>='$start_day' and dt<='$end_day' and city='$city' 
group by word,url
sort by urlnum desc;"
}

wordhost_phase_proc(){
    echo "[Info]$phase $phase_id:wordhost_notdropme -> $opath/wordhost"
    hive_proc "set mapred.reduce.tasks = 1;
insert overwrite local directory '$opath/wordhost'
select word,host,sum(num) as hostnum,'$phase_id','$city' 
from wordhost_notdropme 
where dt>='$start_day' and dt<='$end_day' and city='$city' 
group by word,host 
sort by hostnum desc;"
}

# 访问word最多的用户id统计
worduid_phase_proc(){
    echo "[Info]$phase $phase_id:worduid_notdropme -> $opath/worduid"
    hive_proc "set mapred.reduce.tasks = 1;
insert overwrite local directory '$opath/worduid' 
select word,uid,sum(num) as uidnum,'$phase_id','$city' 
from worduid_notdropme where dt>='$start_day' and dt<='$end_day' and city='$city' 
group by word,uid 
sort by uidnum desc;"
}