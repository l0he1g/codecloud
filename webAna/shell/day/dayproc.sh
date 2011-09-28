#!/bin/bash
# Function: day data process for data in weblog
# 2009-10-26:[Yan Xiaohui]created
# 所有的$day,$opath源于此处的全局变量
# time|user_id|user_ip|domain|host|url|refer|title|words|has_cookie 
source $sh_path/dayimport.sh
source $sh_path/day/keyword.sh
source $sh_path/day/websites.sh
source $sh_path/day/uid.sh
source $sh_path/day/host.sh

dayrun(){
    echo "begin at:`date`"
    # import weblog of one day to hive
    dayimport

    # keyword
    echo "===================== A.keyword statistic===================="
    echo "------------`date +%T` [1/18]keyword pre process: $day-------------"
    wordpre_day_proc
    # echo "------------`date +%T` [2/18]words day process: $day-------------"
    words_day_proc
    echo "------------`date +%T` [3/18]worduid day process: $day-------------"
    worduid_day_proc
    echo "------------`date +%T` [4/18]worduip day process: $day-------------"
    worduip_day_proc
    echo "===================== B.websites statistic===================="
    echo "------------`date +%T` [5/18]cates day porcess: $day-------------"
    cates_day_proc
    echo "------------`date +%T` [6/18]cateuid day porcess: $day-------------"
    cateuid_day_proc
    echo "------------`date +%T` [7/18]cateuip day porcess: $day-------------"
    cateuip_day_proc
    
    echo "------------`date +%T` [8/18]domains day porcess: $day-----------"
    domains_day_proc
    echo "------------`date +%T` [9/18]domainuid day porcess: $day-------------"
    domainuid_day_proc
    echo "------------`date +%T` [10/18]domainuip day porcess: $day-------------"
    domainuip_day_proc
    
    echo "------------`date +%T` [11/18]hosts day porcess: $day-------------"
    hosts_day_proc
    echo "------------`date +%T` [12/18]hostuid day porcess: $day-------------"
    hostuid_day_proc
    echo "------------`date +%T` [13/18]hostuip day porcess: $day-------------"
    hostuip_day_proc
    
    echo "===================== C.specified host statistic===================="
    echo "------------`date +%T` [14/18]hostrefer day porcess: $day------------"
    hostrefer_day_proc
    echo "------------`date +%T` [15/18]hostse day porcess: $day------------"
    hostse_day_proc
    echo "===================== D.specified uid statistic===================="
    echo "------------`date +%T` [16/18]uids day porcess: $day-------------"
    uids_day_proc
    echo "------------`date +%T` [17/18]uiddomain day porcess: $day-------------"
    uiddomain_day_proc
    echo "------------`date +%T` [18/18]uidhost day porcess: $day-------------"
    uidhost_day_proc
    
    echo "-------(:---Day process successed:`date`---:)-----------"
}

dayproc(){
    export opath=$out_dir/day/$day
    export log=$log_dir/day_${day}_running.log
    dayrun > $log 2>&1

    # scp day data to httpserver
    cd $out_dir/day
    tar czf ${day}.tar.gz $day
    rm -rf $day
    scp ${day}.tar.gz ${http_user}@${http_server}:${http_dir}/input/day/
    if [ $? -eq 0 ];then
        rm -f ${day}.tar.gz
        echo "Transport ${day}.tar.gz to $http_server end at:`date`.">>$log
        ssh ${http_user}@${http_server} ". ${load_day}">>$log
		echo "Load data for ${day} to mysql end at:`date`.">>$log
        # task successed,send log to me
        new_log=${log/running/successed}
        mv $log $new_log 
        cat $new_log|mail -s `basename $new_log` l0he1g@gmail.com
       #cat $log_dir/$day.log|mail -s ${subject} cwbdemail@163.com
    fi
	cd -
}