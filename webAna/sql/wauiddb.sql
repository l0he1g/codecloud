# create database for websites statistic
# 2009-10-30:[Yan Xiaohui]created
#1.add database
create database if not exists wauiddb default character set utf8 collate utf8_general_ci;

#2.add user and pwd
grant ALL on wauiddb.* to 'hpclouduser'@'%' identified by 'hpcloud';
grant ALL on wauiddb.* to 'hpclouduser'@'localhost' identified by 'hpcloud';
grant FILE on *.* to 'hpclouduser'@'%' identified by 'hpcloud';
grant FILE on *.* to 'hpclouduser'@'localhost' identified by 'hpcloud';

#3.add table to wauiddb
use wauiddb;

################### uid day table(no week table)############################
create table if not exists uids_day_tb(
uid varchar(32) NOT NULL,
num bigint default 0,
tid int(8),
city char(2),
index uid_index(uid)
)engine=myisam
partition by list(tid)(
   partition p0 values in (0)
);

create table if not exists uiddomain_day_tb(
uid varchar(32) NOT NULL,
num bigint default 0,
tid int(8),
city char(2),
index uid_index(uid)
)engine=myisam
partition by list(tid)(
   partition p0 values in (0)
);

create table if not exists uidhost_day_tb(
uid varchar(32) NOT NULL,
num bigint default 0,
tid int(8),
city char(2),
index uid_index(uid)
)engine=myisam
partition by list(tid)(
   partition p0 values in (0)
);
