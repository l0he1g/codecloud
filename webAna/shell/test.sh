sh_path=/home/xh/projects/webAna/trunk/shell
source $sh_path/hive_utils.sh
source $sh_path/config.properties
day=20091103
opath=$out_dir/day/$day

# 1. 导入网站分类数据到hive
load data local inpath '/home/hadoop/cates' overwrite into table hostcate_notdropme;
select * from hostcate_notdropme limit 3;

# 2.导入weblog测试数据到hive
load data local inpath 'projects/webAna/testdata/weblog' into table weblog_notdropme partition(dt='20091103',city='gz');
select * from weblog_notdropme limit 3;
# 测试weblog大小:691M 记录数：3,681,705 平均每条记录大小:197B，约等于平均每行的字符数
select count(1) from weblog_notdropme;
select count(distinct host) from weblog_notdropme;

# 3.从hive统计出主类和子类访问排名,
# hostcate原有分类网站20087个(去掉子类为0，余16394个),weblog中有网站75478，连接后网站：5134--太少!
set mapred.reduce.tasks = 1;
from hostcate_notdropme a join 
(select host,count(1) as num
from weblog_notdropme 
where dt='20091103' and city='gz'
group by host) b
on ( a.host = b.host )
    insert overwrite local directory '/home/hadoop/webAna/data/day/20091103/subcates'
    select a.sub,sum(b.num) as s1,'20091103','gz'
    group by a.sub
    sort by s1 desc

    insert overwrite local directory '/home/hadoop/webAna/data/day/20091103/majorcates'
    select a.major,sum(b.num) as s2,'20091103','gz'
    group by a.major
    sort by s2 desc;

# 4.从hive统计主类和子类的用户id访问排名
# 子类用户访问id不加limit有:268,749行
set mapred.reduce.tasks = 1;
from hostcate_notdropme a join 
(select host,uid,count(1) as num 
from weblog_notdropme 
where dt='20091103' and city='gz'
group by host,uid) b
on ( a.host = b.host )
    insert overwrite local directory '/home/hadoop/webAna/data/day/20091103/subuid'
    select a.sub,b.uid,sum(b.num) as s1
    group by a.sub,b.uid,'20091103','gz'
    sort by s1 desc
	limit 30000
    insert overwrite local directory '/home/hadoop/webAna/data/day/20091103/majoruid'
    select a.major,b.uid,sum(b.num) as s2
    group by a.major,b.uid,'20091103','gz'
    sort by s2 desc
    limit 30000;
#cat attempt_200911032044_0027_r_000000_0|awk -F '\001' '$3>2{print $1,$2,$3}'|wc -l
#统计结果：>1,2,3,4,5,6,7
# 268749,143740,87337,63254,46655,37239,29757,24861
#当num>=5时,余下记录37239，变化趋缓,可用做过滤条件。
#http://chart.apis.google.com/chart?chs=320x200&cht=lc&chtt=Live Preview&chd=s:9hUOLIHG&chdl=visits&chxl=0:|>num|1|2|3|4|5|6|7|1:|subcateuid|50000|100000|150000|200000|2500000|&chxt=x,y

#其余按sh文件执行
查询类型     3w记录中min次数
domains         2
domainuid       18
domainuip       17
domainhost      4
doaminurl       7
hostuid       16
hostuip       15
hosturl       7
urls         7
hosts         4
hostuid       16
hostuip       15
hosturl       7
urls         7
urluid       5
urluip       4
hostrefer    2
hostse       1
