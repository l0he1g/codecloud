#coding=utf8
# compute a host's category by referring the google Similar search

def readf( path ):
    hosts = {}
    f = open(path)
    for line in f:
        line = line.strip()
        host, rs = line.split('\t',1)
        s_hosts = rs.split(',')
        hosts[host] = s_hosts
        
    return hosts

def read_host_cates( path ):
    host_cates = {}
    f = open(path)
    for line in f:
        line = line.strip()
        host, rs = line.split('\t',1)
        cates = rs.split(',')
        n = len(cates)
        host_cates[host] = zip(cates,n*[float(1)/n])

    return host_cates

# retrival cate of h
# input: h  host
#        hosts_cates   {host:[(cate1,v1),(cate2,v2),...],}
# return: cates  [(cate1,v1),(cate2,v2)] or NONE
def get_cates( h,host_cates):
    if host_cates.has_key( h ):
        return host_cates[h]
    else:
        return None

# sum value with the same cate
# input: [(cate1,v1),(cate2,v2),...]
# return: [cate:v,...]
def compress( cate_vs ):
    dic = {}
    for c,v in cate_vs:
        if dic.has_key(c):
            dic[c] += v
        else:
            dic[c] = v

    return dic

def compute_cate( host_sims, host_cates, result_path ):
    wf = open( result_path, 'w' )
    for host,sims in host_sims.items():
        print 'host:',host,'\trel_host num:',len(sims), 
        cate_vs = []
        nh = 0                  # valid similiar hosts
        for rh in sims:
            t = get_cates(rh,host_cates)
            if t:
                nh += 1
                cate_vs.extend( t )
            
        cate_vs = compress( cate_vs )
        print '\tcate num:',len(cate_vs)
        if len(cate_vs)>0:
            wf.write('%s\t%s\n' % (host,','.join(['%s:%f' % (k,v/nh) for k,v in sorted(cate_vs.items(),key=lambda d:d[1],reverse=True)])) )
                           
if __name__ =='__main__':
    data_path = 'data/getted.txt'
    host_sims = readf( data_path )
    
    host_cates_path = 'data/cate_hosts.txt'
    host_cates = read_host_cates( host_cates_path )
    
    result_path = 'data/cate_hosts_google.txt'
    print "compute cate"
    compute_cate( host_sims, host_cates, result_path )
    
