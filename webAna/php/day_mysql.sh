#!/bin/bash
# Function:specified keywords statistic about uid,uip,domain,host,url

if [ $# != 0 ];then
	echo "Usage: nohup $0 &"
    exit -1
fi

http_dir=/mysql/source/webAna/input
done_dir=/mysql/source/webAna/done_data
log_dir=/mysql/source/webAna/logs
done_log=$log_dir/done_day.log
php_file=/mysql/source/webAna/php/load_day.php

cd $http_dir/day
for data_tar in `ls 200?????.tar.gz`;do
    day=${data_tar%.tar.gz}

	while read LINE;do
		if [ $day = "$LINE" ];then
			echo "We have got the data of ${day}!"
			exit -1
		fi
	done < $done_log

	tar xzf $data_tar

    log=$log_dir/day_${day}_running.log
	php -f $php_file $day > $log 2>&1
	if [ $? -ne 0 ];then
		new_log=${log/running/failed}
	else
		new_log=${log/running/successed}
		rm -rf $day
		
		mv $data_tar $done_dir/day/
		echo $day>>$done_log
		echo "$day data has been successfully load to mysql:)"
	fi
	mv $log $new_log
	cat $new_log|mail -s "mysql:`basename $new_log`" l0he1g@gmail.com
done