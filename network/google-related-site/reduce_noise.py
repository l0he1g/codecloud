#coding=utf8
# remove cate where score<0.01,and reassign score to other cates
# input:data/cate_hosts_google.txt
#      host   cate:v,cate:v,...
# output:data/cate_hosts_google_reduced.txt

# input:host   cate1:score,cate2:score
# output:{host:[(cate1,score),...)}
def read_host_cates_google( path):
    rf = open( path )
    data = {}
    for line in rf:
        line = line.strip()
        h,s = line.split('\t')
        items = s.split(',')
        data[h] = [it.split(':') for it in items] 
    
    return data

def reduce( host_cates,path ):
    wf = open(path,'w')
    for h,cates in host_cates.items():
        s = 0
        new_cates = {}
        for c,v in cates:
            v = float(v)
            if v>=0.01:
                s += v
                new_cates[c] = v
        if s>0: 
            wf.write("%s\t%s\n" % (h,','.join(["%s:%f" % (c,v/s) for c,v in sorted(new_cates.items(),key=lambda d:d[1],reverse=True)])))
            
if __name__=='__main__':
    host_cates_google_path = 'data/cate_hosts_google.txt'
    result_path = 'data/host_cates_google_reduced.txt'
    print "read:",host_cates_google_path
    host_cates_google = read_host_cates_google( host_cates_google_path )
    reduce( host_cates_google, result_path )
    print "result:",result_path
