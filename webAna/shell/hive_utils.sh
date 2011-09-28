#!/bin/bash
# Function: tools
# 2009-10-26:[Yan Xiaohui]created

hive_proc(){
    ql="$1"
    cd ~
    hive -e "$ql"
    if [ $? -ne 0 ];then
        echo "############### [Failed ql for above] ###############"
		echo "$ql"
		new_log=${log/running/failed}
		mv $log $new_log
		cat $new_log|mail -s `basename $new_log` l0he1g@gmail.com
        exit 1
    fi
}
