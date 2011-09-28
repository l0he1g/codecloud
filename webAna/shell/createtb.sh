#!/bin/bash
# Function: create tables at the beginning,周表和月表，不建日表
# 2009-10-26:[Yan Xiaohui]created
sh_path=/home/hadoop/webAna/src/shell
source $sh_path/hive_utils.sh
source $sh_path/config.properties

# weblog table
hive_proc "
create table weblog_notdropme(
time string,uid string,uip string,
url string,domain string,host string,refer string,
title string,words string,has_cookie tinyint
)
partitioned by( dt string,city string)
clustered by(host) into 64 buckets 
row format delimited 
    fields terminated by '|';"

############# 1. keywords statistic ##################
hive_proc "
create table wordlog_medium( 
uid string, uip string, url string,
domain string, host string,word string)
PARTITIONED BY(dt STRING,city string)
clustered by(word) into 64 buckets;"

# for speeding phase stattistic
hive_proc "
create table words_notdropme(
word string,num bigint)
PARTITIONED BY(dt string,city string)
clustered by(word) into 64 buckets;"

hive_proc "
create table worddomain_notdropme(
word string,domain string,num bigint)
PARTITIONED BY(dt string,city string)
clustered by(word) into 64 buckets;"
hive_proc "
create table wordhost_notdropme(
word string,host string,num bigint)
PARTITIONED BY(dt string,city string)
clustered by(word) into 64 buckets ;"
hive_proc "
create table wordurl_notdropme(
word string,url string,num bigint)
PARTITIONED BY(dt string,city string)
clustered by(word) into 64 buckets;"
hive_proc "
create table worduid_notdropme (
word string,uid string,num bigint)
PARTITIONED BY(dt string,city string)
clustered by(word) into 64 buckets ;"

############# 2. websits statistic ###################
# host category info
hive_proc "
create table hostcate_notdropme(major string,sub string,host string)
row format delimited 
    fields terminated by '\t';"
############# 3. stream statistic ####################
hive_proc "
create table hostrefer_notdropme(host string,refer string,num bigint)
partitioned by (dt string,city string)
clustered by (host) sorted by (num) into 64 buckets;"

############# 4. search engine ######################
hive_proc "
create table hostse_notdropme(
   host string,word string,engine string,num bigint)
partitioned by (dt string,city string)
clustered by (host) sorted by (num) into 64 buckets;"

############ 5.load hostcate ##########################
hive_proc "
load data local inpath '$hostcate_path' overwrite into table hostcate_notdropme;"

