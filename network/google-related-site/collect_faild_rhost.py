#coding=utf8
# collect faild related search result

def read_hosts( path ):
     f = open(path)
     return [l.split()[0].strip() for l in f if l.strip()]

def clean_url( url ):
    url = url.strip()
    tmp_p = url.find('/')
    if tmp_p!=-1:
        url = url[:tmp_p]

    return url.strip()

def clean_rhosts( path ):
     f = open(path)
     data = {}
     for line in f:
         h,rs = line.strip().split('\t',1)
         if not data.has_key(h):
             hs = rs.split(',')
             data[h]= [ clean_url(h) for h in hs ]
         
     outf = open( 'output/new_rhosts.txt','w')
     for h,hs in data.items():
         outf.write('%s\t%s\n' % (h,','.join(hs)) )
                    
if __name__=='__main__':
    uncate_host_path = 'data/uncate_hosts.txt'

    all_hosts = read_hosts( uncate_host_path )
    
    return_path = 'data/remain_hosts.txt'
    rhosts = read_hosts( return_path )
    print "all hosts num :",len(all_hosts)
    print 'get hsots num:',len(rhosts)

    remain = []#[h for h in all_hosts if h not in rhosts]
    for h in all_hosts:
        if h not in rhosts and h not in remain:
            remain.append(h)
    
    print 'remain hsots num:',len(remain)
    print "write faild hosts"
    f=open( 'data/hosts_2.txt','w')
    f.write('\n'.join( remain ) )

#    return_path = 'output/rhosts.txt'
#    clean_rhosts( return_path )
