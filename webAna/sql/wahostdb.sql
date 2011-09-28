 # create database for specified host statistic
# 2009-10-30:[Yan Xiaohui]created
#1.add database
create database if not exists wahostdb default character set utf8 collate utf8_general_ci;

#2.add user and pwd
grant ALL on wahostdb.* to 'hpclouduser'@'%' identified by 'hpcloud';
grant ALL on wahostdb.* to 'hpclouduser'@'localhost' identified by 'hpcloud';
grant FILE on *.* to 'hpclouduser'@'%' identified by 'hpcloud';
grant FILE on *.* to 'hpclouduser'@'localhost' identified by 'hpcloud';

#3.add table to websitesdb
use wahostdb;

############# 1. host refer data tb ###############
# hostrefer week table template
create table if not exists hostrefer_week_tb(
host varchar(150) NOT NULL,
refer varchar(150) NOT NULL,
num bigint default 0,
tid int(6),
city char(2),
index host_index(host),
index refer_index(refer)
)engine=myisam
partition by list(tid)(
   partition p0 values in (0));

# hostrefer week table template
create table if not exists hostrefer_month_tb(
host varchar(150) NOT NULL,
refer varchar(150) NOT NULL,
num bigint default 0,
tid int(6),
city char(2),
index host_index(host),
index refer_index(refer)
)engine=myisam
partition by list(tid)(
   partition p0 values in (0));

############# 2. search engine data tb ###############
create table if not exists hostse_week_tb(
host varchar(150) NOT NULL,
word varchar(10) NOT NULL,
engine varchar(8) NOT NULL,
num bigint default 0,
tid int(6),
city char(2),
index host_index(host,word),
index word_index(word)
)engine=myisam
partition by list(tid)(
   partition p0 values in (0));

create table if not exists hostse_month_tb(
host varchar(150) NOT NULL,
word varchar(10) NOT NULL,
engine varchar(8) NOT NULL,
num bigint default 0,
tid int(6),
city char(2),
index host_index(host,word),
index word_index(word)
)engine=myisam
partition by list(tid)(
   partition p0 values in (0));

