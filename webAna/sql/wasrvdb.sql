#1.add database
create database if not exists wasrvdb default character set utf8 collate utf8_general_ci;

#2.add user and pwd
grant ALL on wasrvdb.* to 'hpclouduser'@'%' identified by 'hpcloud';
grant ALL on wasrvdb.* to 'hpclouduser'@'localhost' identified by 'hpcloud';

#3.add table to hpwasrvdb
use wasrvdb;
set names utf8;

create view weeks as select distinct tid from wasitesdb.majorcates_week_tb order by tid desc;
create view months as select distinct tid from wasitesdb.majorcates_month_tb order by tid desc;