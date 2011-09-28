#!/bin/bash
# Funciton:import data in in_dir into hive,
#      and then move them into in_dir_done
# 2009-10-17:[Yan Xiaohui]created
# time|user_id|user_ip|url|domain|host|refer|title|words|has_cookie 

# Todo:revise data format
dayimport(){
    data_dir=$in_dir/$day
	
    while read LINE;do
        if [ $day = "$LINE" ];then
            echo "We have got the data of ${day}!"
            return
        fi
    done < $data_log
    
    if [ ! -e $data_dir/data_pattern/part-* ];then
        echo "No data file in:${data_dir}/data_pattern/"
		mv $data_dir $done_dir
        exit 0
    fi
    
    echo "-----------begin import data to weblog_notdropme:$day ----------"
    ql="load data local inpath '$data_dir/data_pattern/part-*' overwrite into table weblog_notdropme partition(dt='$day',city='$city');";
    hive_proc "$ql"
    

    echo "$day" >> $data_log
    mv $data_dir $done_dir
}
