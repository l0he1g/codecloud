#!/bin/bash
# Function:specified keywords statistic about uid,uip,domain,host,url

if [ $# != 1 ] && { [ $1 != 'week'] || [ $1 != 'month'] ;};then
    echo "Usage: nohup $0 [week/month]&"
    exit -1
fi

phase=$1
http_dir=/mysql/source/webAna/input
done_dir=/mysql/source/webAna/done_data
log_dir=/mysql/source/webAna/logs
done_log=$log_dir/done_${phase}.log
php_file=/mysql/source/webAna/php/load_phase.php

cd $http_dir/${phase}
for data_tar in `ls 200???.tar.gz`;do
    phase_id=${data_tar%.tar.gz}

	# 从done_[weekid].log中读取已算过的日期，若有，不再重复计算
    while read LINE;do
        if [ $phase_id = "$LINE" ];then
            echo "We have got the data of ${phase_id}!"
            exit -1
        fi
    done < $done_log

    tar xzf $data_tar

    log=$log_dir/${phase}_${phase_id}_running.log
    php -f $php_file $phase $phase_id > $log 2>&1
    if [ $? -ne 0 ];then
        new_log=${log/running/failed}
    else
        new_log=${log/running/successed}
        rm -rf $phase_id

        mv $data_tar $done_dir/${phase}/
        echo $phase_id>>$done_log
        echo "$phase $phase_id data has been successfully load to mysql:)"
    fi
    mv $log $new_log
    cat $new_log|mail -s "mysql:`basename $new_log`" l0he1g@gmail.com
done