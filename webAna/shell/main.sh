#!/bin/bash
# Function: last month statistic
# NOTICE: start dayproc
# 2009-11-12:[Yan Xiaohui]created

sh_path=/home/hadoop/webAna/src/shell
source $sh_path/config.properties
source $sh_path/date_utils.sh
source $sh_path/day/dayproc.sh
source $sh_path/hive_utils.sh
source $sh_path/phase/phaseproc.sh

weekproc(){
    # global variables define
    export phase='week'
    export phase_id=`get_week $day`
    export opath=$out_dir/$phase/$phase_id

    export end_day=$day
    export start_day=`get_week_start $day`

    phaseproc
}

monthproc(){
    # global variables define
    export phase='month'
    export phase_id=`get_month $day`
    export opath=$out_dir/$phase/$phase_id

    export end_day=$day
    export start_day=`get_month_start $day`

    phaseproc
}
################ main ######################
# global variables define
for day in `ls $in_dir`;do
#for day in 2009080{1..9};do
    dayproc

    # if need week process
    test `date -d $day +%u` -eq 7 && weekproc

    # if need month process
    test `end_month $day` == 'true' && monthproc
done