# create database for word statistic
# 2009-10-18:[Chenwenbin]created
#1.add database
create database if not exists waworddb default character set utf8 collate utf8_general_ci;

#2.add user and pwd
grant ALL on waworddb.* to 'hpclouduser'@'%' identified by 'hpcloud';
grant ALL on waworddb.* to 'hpclouduser'@'localhost' identified by 'hpcloud';
grant FILE on *.* to 'hpclouduser'@'%' identified by 'hpcloud';
grant FILE on *.* to 'hpclouduser'@'localhost' identified by 'hpcloud';

#3.add table to waworddb
use waworddb

#####################################################
#day:day_tb has key          #
#####################################################
########### words day table ###########
create table if not exists words_day_tb(
word varchar(10) NOT NULL,
num bigint default 0,
tid int(8),
city char(2),
index word_index(word)
)engine=myisam
partition by list(tid)(
   partition p0 values in (0));
############ worduid day table##############
create table if not exists worduid_day_tb(
word varchar(10) NOT NULL,
num bigint default 0,
tid int(8),
city char(2),
index word_index(word)
)engine=myisam
partition by list(tid)(
   partition p0 values in (0));

############ wordip day table ################
create table if not exists worduip_day_tb(
word varchar(10) NOT NULL,
num bigint default 0,
tid int(8),
city char(2),
index word_index(word)
)engine=myisam
partition by list(tid)(
   partition p0 values in (0));

#####################################################
#                   week/month                      #
#####################################################
############## words week table ################
create table if not exists words_week_tb(
word varchar(10) NOT NULL,
num bigint default 0,
tid int(6),
city char(2),
index word_index(word)
)engine=myisam
partition by list(tid)(
   partition p0 values in (0));


# worduid month table
create table if not exists words_month_tb(
word varchar(10) NOT NULL,
num bigint default 0,
tid int(6),
city char(2),
index word_index(word)
)engine=myisam
partition by list(tid)(
   partition p0 values in (0));

########## worduid week table ###############
create table if not exists worduid_week_tb(
word varchar(10) NOT NULL,
uid varchar(32) NOT NULL,
num bigint default 0,
tid int(6),
city char(2),
index word_index(word),
index uid_index(uid)
)engine=myisam
partition by list(tid)(
   partition p0 values in (0));

# worduid month table
create table if not exists worduid_month_tb(
word varchar(10) NOT NULL,
uid varchar(32) NOT NULL,
num bigint default 0,
tid int(6),
city char(2),
index word_index(word),
index uid_index(uid)
)engine=myisam
partition by list(tid)(
   partition p0 values in (0));

# worddomain week table
create table if not exists worddomain_week_tb(
word varchar(10) NOT NULL,
domain varchar(32) not null,
num bigint default 0,
tid int(6),
city char(2),
index word_index(word)
)engine=myisam
partition by list(tid)(
   partition p0 values in (0));
   
# worddomain month table
create table if not exists worddomain_month_tb(
word varchar(10) NOT NULL,
domain varchar(32) not null,
num bigint default 0,
tid int(6),
city char(2),
index word_index(word)
)engine=myisam
partition by list(tid)(
   partition p0 values in (0));

# wordhost table template
create table if not exists wordhost_week_tb(
word varchar(10) NOT NULL,
host varchar(150) not null,
num bigint default 0,
tid int(6),
city char(2),
index word_index(word)
)engine=myisam
partition by list(tid)(
   partition p0 values in (0));

create table if not exists wordhost_month_tb(
word varchar(10) NOT NULL,
host varchar(150) not null,
num bigint default 0,
tid int(6),
city char(2),
index word_index(word)
)engine=myisam
partition by list(tid)(
   partition p0 values in (0));

# wordurl table template
create table if not exists wordurl_week_tb(
word varchar(10) NOT NULL,
url varchar(150) not null,
num bigint default 0,
tid int(6),
city char(2),
index word_index(word)
)engine=myisam
partition by list(tid)(
   partition p0 values in (0));

create table if not exists wordurl_month_tb(
word varchar(10) NOT NULL,
url varchar(150) not null,
num bigint default 0,
tid int(6),
city char(2),
index word_index(word)
)engine=myisam
partition by list(tid)(
   partition p0 values in (0));
