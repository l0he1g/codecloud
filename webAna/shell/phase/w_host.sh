#!/bin/bash
# Function:specified host week statistic about up/down stream,search 
# 2009-10-30:[Yan Xiaohui]created

hostrefer_phase_proc(){
	echo "[Info]$phase $phase_id:hostrefer_notdropme -> $opath/hostrefer"
    hive_proc "set mapred.reduce.tasks = 1;
insert overwrite local directory '$opath/hostrefer'
select host,refer,sum(num) as s,'$phase_id','$city' from hostrefer_notdropme
where dt>='$start_day' and dt<='$end_day' and city='$city' 
group by host,refer
sort by s desc;"
}

hostse_phase_proc(){
	echo "[Info]$phase $phase_id:hostse_notdropme -> $opath/hostse"
    hive_proc "set mapred.reduce.tasks = 1;
insert overwrite local directory '$opath/hostse'
select host,word,engine,sum(num) as s,'$phase_id','$city' from hostse_notdropme
where dt>='$start_day' and dt<='$end_day' and city='$city' 
group by host,word,engine
sort by s desc;"
}
