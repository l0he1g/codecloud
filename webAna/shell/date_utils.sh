#!/bin/bash
# Funciton:some tools for date
# 2009-10-17:[Yan Xiaohui]created
# Usage example: sea_num=`season_num 20090917`

# weej
get_week(){
	day=$1
	year=`date -d $day +%Y`
	#ISO week number, with Monday as first day of week (01..53)
	wnum=`date -d $day +%V`
	echo ${year}${wnum}
}

get_month(){
	day=$1
	year=`date -d $day +%Y`
	mnum=`date -d $day +%m`
	echo ${year}${mnum}
}

get_season(){
	day=$1
	year=`date -d $day +%Y`
	mnum=`date -d $day +%-m`
	snum=`expr $(( $mnum - 1 )) / 3 + 1`
	echo ${year}0${snum}
}

get_year(){
	day=$1
	echo `date -d $day +%Y`
}

############### move #############
tomorrow(){
	day=$1
	ns=`date -d $day +%s`
	ts=$[ $ns + 86400 ]
	echo `date -d @$ts +%Y%m%d`
}

yesterday(){
	day=$1
	ns=`date -d $day +%s`
	ts=$[ $ns - 86400]
	echo `date -d @$ts +%Y%m%d`
}

get_week_start(){
	week_end=$1
	ns=`date -d $week_end +%s`
	ts=$[ $ns - 6 * 86400 ]
	echo `date -d @$ts +%Y%m%d`
}

get_month_start(){
	month_id=`get_month $1`
	echo ${month_id}01
}

# judge a day is the last day of a month or not
# @para:
#     $1 date,e.m 20091002
# @return:
#     true $day is the last day
#     false $day is not the last day
end_month(){
	day=$1
	m1=`date -d $day +%m`
	
	# count tomorrow's month num
	next_day=`tomorrow $day`
	m2=`date -d $next_day +%m`

	if [ "$m1" = "$m2" ];then
		echo false
	else
		echo true
	fi
}

end_season(){
	day=$1
	s1=`season_num $day`

	# count tomorrow's season num
	next_day=`tomorrow $day`
	s2=`season_num $next_day`
	
	if [ "$s1" = "$s2" ];then
		echo false
	else
		echo true
	fi
}

end_year(){
	day=$1
	y1=`date -d $day +%Y`

	# count tomorrow's season num
	next_day=`tomorrow $day`
	y2=`date -d $next_day +%Y`
	
	if [ "$y1" = "$y2" ];then
		echo false
	else
		echo true
	fi
}