# create database for websites statistic
# 2009-10-30:[Yan Xiaohui]created
#1.add database
create database if not exists wasitesdb default character set utf8 collate utf8_general_ci;

#2.add user and pwd
grant ALL on wasitesdb.* to 'hpclouduser'@'%' identified by 'hpcloud';
grant ALL on wasitesdb.* to 'hpclouduser'@'localhost' identified by 'hpcloud';
grant FILE on *.* to 'hpclouduser'@'%' identified by 'hpcloud';
grant FILE on *.* to 'hpclouduser'@'localhost' identified by 'hpcloud';

#3.add table to wasitesdb
use wasitesdb;
################ cates ##################
# 网站分类信息表
create table if not exists hostcate_tb(
majorcate varchar(16) not null,
subcate varchar(16) not null,
host varchar(150) not null,
index hostcate_index(host));
# 分类视图
create view cates as select majorcate,subcate from hostcate_tb group by majorcate,subcate;

# cates day table
create table if not exists majorcates_day_tb(
majorcate varchar(16) NOT NULL,
num bigint default 0,
tid int(8),
city char(2)
)engine=myisam
partition by list(tid)(
   partition p0 values in (0)
);

# cateuid day table
create table if not exists majorcateuid_day_tb(
majorcate varchar(16) NOT NULL,
num bigint default 0,
tid int(8),
city char(2)
)engine=myisam
partition by list(tid)(
   partition p0 values in (0)
);

# cateuip day table
create table if not exists majorcateuip_day_tb(
majorcate varchar(16) NOT NULL,
num bigint default 0,
tid int(8),
city char(2)
)engine=myisam
partition by list(tid)(
   partition p0 values in (0)
);

# subcate
create table if not exists subcates_day_tb(
subcate varchar(16) NOT NULL,
num bigint default 0,
tid int(8),
city char(2)
)engine=myisam
partition by list(tid)(
   partition p0 values in (0)
);

# subcateuid
create table if not exists subcateuid_day_tb(
subcate varchar(16) NOT NULL,
num bigint default 0,
tid int(8),
city char(2)
)engine=myisam
partition by list(tid)(
   partition p0 values in (0)
);

# subcateuip
create table if not exists subcateuip_day_tb(
subcate varchar(16) NOT NULL,
num bigint default 0,
tid int(8),
city char(2)
)engine=myisam
partition by list(tid)(
   partition p0 values in (0)
);

############ week template ##############
# 1.cates
create table if not exists majorcates_week_tb(
majorcate varchar(16) NOT NULL,
num bigint default 0,
tid int(6),
city char(2)
)engine=myisam
partition by list(tid)(
   partition p0 values in (0)
);

create table if not exists majorcates_month_tb(
majorcate varchar(16) NOT NULL,
num bigint default 0,
tid int(6),
city char(2)
)engine=myisam
partition by list(tid)(
   partition p0 values in (0)
);

### subcates
create table if not exists subcates_week_tb(
subcate varchar(16) NOT NULL,
num bigint default 0,
tid int(6),
city char(2)
)engine=myisam
partition by list(tid)(
   partition p0 values in (0)
);

create table if not exists subcates_month_tb(
subcate varchar(16) NOT NULL,
num bigint default 0,
tid int(6),
city char(2)
)engine=myisam
partition by list(tid)(
   partition p0 values in (0)
);

# 2.cateuid
create table if not exists majorcateuid_week_tb(
majorcate varchar(16) NOT NULL,
uid varchar(32) NOT NULL,
num bigint default 0,
tid int(6),
city char(2)
)engine=myisam
partition by list(tid)(
   partition p0 values in (0)
);

create table if not exists majorcateuid_month_tb(
majorcate varchar(16) NOT NULL,
uid varchar(32) NOT NULL,
num bigint default 0,
tid int(6),
city char(2)
)engine=myisam
partition by list(tid)(
   partition p0 values in (0)
);

create table if not exists subcateuid_week_tb(
subcate varchar(16) NOT NULL,
uid varchar(32) NOT NULL,
num bigint default 0,
tid int(6),
city char(2)
)engine=myisam
partition by list(tid)(
   partition p0 values in (0)
);

create table if not exists subcateuid_month_tb(
subcate varchar(16) NOT NULL,
uid varchar(32) NOT NULL,
num bigint default 0,
tid int(6),
city char(2)
)engine=myisam
partition by list(tid)(
   partition p0 values in (0)
);

# 4.catedomain
create table if not exists majorcatedomain_week_tb(
majorcate varchar(16) NOT NULL,
domain varchar(32) NOT NULL,
num bigint default 0,
tid int(6),
city char(2)
)engine=myisam
partition by list(tid)(
   partition p0 values in (0)
);

create table if not exists majorcatedomain_month_tb(
majorcate varchar(16) NOT NULL,
domain varchar(32) NOT NULL,
num bigint default 0,
tid int(6),
city char(2)
)engine=myisam
partition by list(tid)(
   partition p0 values in (0)
);

# subcatedomain
create table if not exists subcatedomain_week_tb(
subcate varchar(16) NOT NULL,
domain varchar(32) NOT NULL,
num bigint default 0,
tid int(6),
city char(2)
)engine=myisam
partition by list(tid)(
   partition p0 values in (0)
);

create table if not exists subcatedomain_month_tb(
subcate varchar(16) NOT NULL,
domain varchar(32) NOT NULL,
num bigint default 0,
tid int(6),
city char(2)
)engine=myisam
partition by list(tid)(
   partition p0 values in (0)
);

# 4.catehost 
create table if not exists majorcatehost_week_tb(
majorcate varchar(16) NOT NULL,
host varchar(150) NOT NULL,
num bigint default 0,
tid int(6),
city char(2)
)engine=myisam
partition by list(tid)(
   partition p0 values in (0)
);

create table if not exists majorcatehost_month_tb(
majorcate varchar(16) NOT NULL,
host varchar(150) NOT NULL,
num bigint default 0,
tid int(6),
city char(2)
)engine=myisam
partition by list(tid)(
   partition p0 values in (0)
);

# subcatehost
create table if not exists subcatehost_week_tb(
subcate varchar(16) NOT NULL,
host varchar(150) NOT NULL,
num bigint default 0,
tid int(6),
city char(2)
)engine=myisam
partition by list(tid)(
   partition p0 values in (0)
);

create table if not exists subcatehost_month_tb(
subcate varchar(16) NOT NULL,
host varchar(150) NOT NULL,
num bigint default 0,
tid int(6),
city char(2)
)engine=myisam
partition by list(tid)(
   partition p0 values in (0)
);

# 5.cateurl week table
create table if not exists majorcateurl_week_tb(
majorcate varchar(16) NOT NULL,
url varchar(150) NOT NULL,
num bigint default 0,
tid int(6),
city char(2)
)engine=myisam
partition by list(tid)(
   partition p0 values in (0)
);

create table if not exists majorcateurl_month_tb(
majorcate varchar(16) NOT NULL,
url varchar(150) NOT NULL,
num bigint default 0,
tid int(6),
city char(2)
)engine=myisam
partition by list(tid)(
   partition p0 values in (0)
);

create table if not exists subcateurl_week_tb(
subcate varchar(16) NOT NULL,
url varchar(150) NOT NULL,
num bigint default 0,
tid int(6),
city char(2)
)engine=myisam
partition by list(tid)(
   partition p0 values in (0)
);

create table if not exists subcateurl_month_tb(
subcate varchar(16) NOT NULL,
url varchar(150) NOT NULL,
num bigint default 0,
tid int(6),
city char(2)
)engine=myisam
partition by list(tid)(
   partition p0 values in (0)
);


############ domain #######################
# 3.1 domains day table
create table if not exists domains_day_tb(
domain varchar(32) NOT NULL,
num bigint default 0,
tid int(8),
city char(2),
index domain_index(domain)
)engine=myisam
partition by list(tid)(
   partition p0 values in (0)
);

# 3.2 domainuid day table
create table if not exists domainuid_day_tb(
domain varchar(32) NOT NULL,
num bigint default 0,
tid int(6),
city char(2),
index domain_index(domain)
)engine=myisam
partition by list(tid)(
   partition p0 values in (0)
);

# 3.3 domainuip day table
create table if not exists domainuip_day_tb(
domain varchar(32) NOT NULL,
num bigint default 0,
tid int(6),
city char(2),
index domain_index(domain)
)engine=myisam
partition by list(tid)(
   partition p0 values in (0)
);

############ week template ##############
# 1.domains
create table if not exists domains_week_tb(
domain varchar(32) NOT NULL,
num bigint default 0,
tid int(6),
city char(2),
index domain_index(domain)
)engine=myisam
partition by list(tid)(
   partition p0 values in (0)
);

create table if not exists domains_month_tb(
domain varchar(32) NOT NULL,
num bigint default 0,
tid int(6),
city char(2),
index domain_index(domain)
)engine=myisam
partition by list(tid)(
   partition p0 values in (0)
);

# 2.domainuid
create table if not exists domainuid_week_tb(
domain varchar(32) NOT NULL,
uid varchar(32) NOT NULL,
num bigint default 0,
tid int(6),
city char(2),
index domain_index(domain),
index uid_index(uid)
)engine=myisam
partition by list(tid)(
   partition p0 values in (0)
);

create table if not exists domainuid_month_tb(
domain varchar(32) NOT NULL,
uid varchar(32) NOT NULL,
num bigint default 0,
tid int(6),
city char(2),
index domain_index(domain),
index uid_index(uid)
)engine=myisam
partition by list(tid)(
   partition p0 values in (0)
);


# 4.domainhost 
create table if not exists domainhost_week_tb(
domain varchar(32) NOT NULL,
host varchar(150) NOT NULL,
num bigint default 0,
tid int(6),
city char(2),
index domain_index(domain)
)engine=myisam
partition by list(tid)(
   partition p0 values in (0)
);

create table if not exists domainhost_month_tb(
domain varchar(32) NOT NULL,
host varchar(150) NOT NULL,
num bigint default 0,
tid int(6),
city char(2),
index domain_index(domain)
)engine=myisam
partition by list(tid)(
   partition p0 values in (0)
);

# 5.domainurl week table
create table if not exists domainurl_week_tb(
  domain varchar(32) NOT NULL,
url varchar(150) NOT NULL,
num bigint default 0,
tid int(6),
city char(2),
index domain_index(domain)
)engine=myisam
partition by list(tid)(
   partition p0 values in (0)
);

create table if not exists domainurl_month_tb(
domain varchar(32) NOT NULL,
url varchar(150) NOT NULL,
num bigint default 0,
tid int(6),
city char(2),
index domain_index(domain)
)engine=myisam
partition by list(tid)(
   partition p0 values in (0)
);
################### hosts ##############################
# 4.1 hosts day table
create table if not exists hosts_day_tb(
host varchar(150) NOT NULL,
num bigint default 0,
tid int(8),
city char(2),
index host_index(host)
)engine=myisam
partition by list(tid)(
   partition p0 values in (0)
);

# 4.2 hostuid day table
create table if not exists hostuid_day_tb(
host varchar(150) NOT NULL,
num bigint default 0,
tid int(8),
city char(2),
index host_index(host)
)engine=myisam
partition by list(tid)(
   partition p0 values in (0)
);


# 4.3 hostuip day table
create table if not exists hostuip_day_tb(
host varchar(150) NOT NULL,
num bigint default 0,
tid int(8),
city char(2),
index host_index(host)
)engine=myisam
partition by list(tid)(
   partition p0 values in (0)
);

######### week template #########
# 1.hosts周访问排名
create table if not exists hosts_week_tb(
host varchar(150) NOT NULL,
num bigint default 0,
tid int(6),
city char(2),
index host_index(host)
)engine=myisam
partition by list(tid)(
   partition p0 values in (0)
);

create table if not exists hosts_month_tb(
host varchar(150) NOT NULL,
num bigint default 0,
tid int(6),
city char(2),
index host_index(host)
)engine=myisam
partition by list(tid)(
   partition p0 values in (0)
);

# 2.hostuid week table
create table if not exists hostuid_week_tb(
host varchar(150) NOT NULL,
uid varchar(32) NOT NULL,
num bigint default 0,
tid int(6),
city char(2),
index host_index(host),
index uid_index(uid)
)engine=myisam
partition by list(tid)(
   partition p0 values in (0)
);

create table if not exists hostuid_month_tb(
host varchar(150) NOT NULL,
uid varchar(32) NOT NULL,
num bigint default 0,
tid int(6),
city char(2),
index host_index(host),
index uid_index(uid)
)engine=myisam
partition by list(tid)(
   partition p0 values in (0)
);


# 4.hosturl week table
create table if not exists hosturl_week_tb(
host varchar(150) NOT NULL,
url varchar(150) NOT NULL,
num bigint default 0,
tid int(6),
city char(2),
index host_index(host)
)engine=myisam
partition by list(tid)(
   partition p0 values in (0)
);

create table if not exists hosturl_month_tb(
host varchar(150) NOT NULL,
url varchar(150) NOT NULL,
num bigint default 0,
tid int(6),
city char(2),
index host_index(host)
)engine=myisam
partition by list(tid)(
   partition p0 values in (0)
);

