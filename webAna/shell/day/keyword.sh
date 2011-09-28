#!/bin/bash
# Function:specified keywords statistic about uid,uip,domain,host,url

wordpre_day_proc()
{
    echo "day $day: weblog_notdropme->wordlog_medium "
    hive_proc "
add file $keyword_proc_py;
FROM (
   FROM weblog_notdropme
   select transform(uid,uip,url,domain,host,words)
   USING 'python `basename $keyword_proc_py`' 
   AS uid_1,uip_1,url_1,domain_1,host_1,word_1
   where dt='$day' and city='$city') map_output 
   INSERT OVERWRITE TABLE wordlog_medium PARTITION(dt='$day',city='$city')
select uid_1,uip_1,url_1,domain_1,host_1,trim(word_1)
WHERE trim(word_1) IS NOT NULL;"
  
    echo "--------------[1/5]Day process for words phase"
    hive_proc "
insert overwrite table words_notdropme partition(dt='$day',city='$city')  
select word,count(1) as num 
from wordlog_medium where dt='$day' and city='$city' 
group by word;"

    echo "--------------[2/5]Day process for worddomain phase"
    hive_proc "
insert overwrite table worddomain_notdropme partition(dt='$day',city='$city')  
select word,domain,count(1) as num 
from wordlog_medium where dt='$day' and city='$city' 
group by word,domain;"
    
    echo "--------------[3/5]Day process for wordhost phase"
    hive_proc "
insert overwrite table wordhost_notdropme partition(dt='$day',city='$city') 
select word,host,count(1) as num 
from wordlog_medium where dt='$day' and city='$city' 
group by word,host;"
    
    echo "--------------[4/5]Day process for wordurl phase"
    hive_proc "
insert overwrite table wordurl_notdropme partition(dt='$day',city='$city') 
select word,url,count(1) as num 
from wordlog_medium where dt='$day' and city='$city' 
group by word,url;"
        
    echo "--------------[5/5]Day process for worduid phase"
    hive_proc "
insert overwrite table worduid_notdropme partition(dt='$day',city='$city') 
select word,uid,count(1) as num 
from wordlog_medium where dt='$day' and city='$city' 
group by word,uid;"
}

words_day_proc()
{
    echo "day $day:wordlog_medium -> $opath/words"
    hive_proc "
set mapred.reduce.tasks = 1;
from words_notdropme
    insert overwrite local directory '$opath/words' 
    select word,num,'$day','$city' 
    where dt='$day' and city='$city'
    order by num desc;"
}

worduid_day_proc(){
    echo "day $day:worduid_notdropme -> $opath/worduid"
    hive_proc "set mapred.reduce.tasks = 1;
from worduid_notdropme
    insert overwrite local directory '$opath/worduid' 
    select word,count(distinct uid) as num,'$day','$city' 
    where dt='$day' and city='$city'
    group by word sort by num desc;"
}

worduip_day_proc(){
    echo "day $day:wordlog_medium -> $opath/worduip"
    hive_proc "set mapred.reduce.tasks = 1;
from wordlog_medium
    insert overwrite local directory '$opath/worduip' 
    select word,count(distinct uip) as ipnum,'$day','$city' 
    where dt='$day' and city='$city' 
    group by word sort by ipnum desc;"
}