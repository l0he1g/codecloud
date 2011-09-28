#!/bin/bash
# Function: last week statistic
# NOTICE: excuted at the first day of week,namely Monday
# 2009-10-26:[Yan Xiaohui]created
source $sh_path/phase/w_keyword.sh
source $sh_path/phase/w_websites.sh
source $sh_path/phase/w_host.sh

phaserun(){
    echo "begin at:`date`"
    echo "===================== A.keyword statistic===================="
    echo "------------`date +%T` [1/19]words $phase process: $phase_id(${start_day}-${end_day})-------------"
    words_phase_proc
    echo "------------`date +%T` [2/19]worddomain $phase process: $phase_id(${start_day}-${end_day})-------------"
    worddomain_phase_proc
    echo "------------`date +%T` [3/19]wordurl $phase process: $phase_id(${start_day}-${end_day})-------------"
    wordurl_phase_proc
    echo "------------`date +%T` [4/19]wordhost $phase process: $phase_id(${start_day}-${end_day})-------------"
    wordhost_phase_proc
    echo "------------`date +%T` [5/19]worduid $phase process: $phase_id(${start_day}-${end_day})-------------"
    worduid_phase_proc
    echo "===================== B.websites statistic===================="
    # cate
    echo "------------`date +%T` [6/19]cates $phase process: $phase_id(${start_day}-${end_day})-------------"
    cates_phase_proc
    echo "------------`date +%T` [7/19]cateuid $phase process: $phase_id(${start_day}-${end_day})-------------"
    cateuid_phase_proc
    echo "------------`date +%T` [8/19]catedomain $phase process: $phase_id(${start_day}-${end_day})-------------"
    catedomain_phase_proc
    echo "------------`date +%T` [9/19]catehost $phase process: $phase_id(${start_day}-${end_day})-------------"
    catehost_phase_proc
    echo "------------`date +%T` [10/19]cateurl $phase process: $phase_id(${start_day}-${end_day})-------------"
    cateurl_phase_proc
    # domain
    echo "------------`date +%T` [11/19]domains $phase process: $phase_id(${start_day}-${end_day})-------------"
    domains_phase_proc
    echo "------------`date +%T` [12/19]domainuid $phase process: $phase_id(${start_day}-${end_day})-------------"
    domainuid_phase_proc
    echo "------------`date +%T` [13/19]domainhost $phase process: $phase_id(${start_day}-${end_day})-------------"
    domainhost_phase_proc
    echo "------------`date +%T` [14/19]domainurl $phase process: $phase_id(${start_day}-${end_day})-------------"
    domainurl_phase_proc
    # host
    echo "------------`date +%T` [15/19]hosts $phase process: $phase_id(${start_day}-${end_day})-------------"
    hosts_phase_proc
    echo "------------`date +%T` [16/19]hostuid $phase process: $phase_id(${start_day}-${end_day})-------------"
    hostuid_phase_proc
    echo "------------`date +%T` [17/19]hosturl $phase process: $phase_id(${start_day}-${end_day})-------------"
    hosturl_phase_proc
    echo "===================== C.specified host statistic===================="
    echo "------------`date +%T` [18/19]hostrefer $phase process: $phase_id(${start_day}-${end_day})-------------"
    hostrefer_phase_proc
    echo "------------`date +%T` [19/19]hostse $phase process: $phase_id(${start_day}-${end_day})-------------"
    hostse_phase_proc
    
    echo "-------(:---$Phase process successed at:`date`---:)-----------"
}

phaseproc(){
    export log=$log_dir/${phase}_${phase_id}_running.log
    phaserun > $log 2>&1 

    # scp day data to http server
    cd $out_dir/$phase
    tar czf ${phase_id}.tar.gz ${phase_id}
    rm -rf ${phase_id}
    scp ${phase_id}.tar.gz ${http_user}@${http_server}:${http_dir}/input/$phase
    if [ $? -eq 0 ];then
        rm -f ${phase_id}.tar.gz
        echo "Transport ${phase_id}.tar.gz ${http_server} end at:`date`.">>$log
        ssh ${http_user}@${http_server} ". ${load_phase} $phase">>$log
		echo "Load data for $phase ${phase_id} to mysql end at:`date`.">>$log

        # Successed,send log to me 
        new_log=${log/running/successed}
        mv $log $new_log
        cat $new_log|mail -s `basename $new_log` l0he1g@gmail.com
    fi
	cd -
}