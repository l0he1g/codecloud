#coding=utf8
# Given qr, compute how much other qrs connect qr
# concetivity is defined as share some same <u,qt> in triples
from time import time
import random

def read_triples( path ):
    rf = open( path )
    triples = []
    for line in rf:
        line = line.strip()
        r,u,t,v = line.split('\t')
        k = "%s_%s" % (u,t)
        triples.append( (r,k) )
        
    return triples

# get <u,t> for given qr from <qr,u,t>s
def get_uts( triples, qr ):
    uts = []
    for r,k in triples:
        if r == qr and k not in uts:
            uts.append( k )
            
    return uts

# for [(u,t),(u,t)..], get r from <r,u,t>s
def qrs_count( triples, qr_uts, qr ):
    dic = {}
    for r,k in triples:
        if r.strip() != qr.strip() and k in qr_uts:
            if dic.has_key( k ):
                dic[k].append( r )
            else:
                dic[k] = [r]
    return dic

if __name__ == '__main__':
    triples_pt = './result/triples_cate.txt'
#    triples_pt = '../em_multi/data/voca_triples_cate.txt'
    
    triples = read_triples( triples_pt )
    wf = open('qrs.txt','w')
#    import sys
#    wf = sys.stdout
    print >>wf,"num of triples:",len(triples)

    sample = random.sample(xrange(len(triples)),  3)
    li = []
    for i in sample:
        qr = triples[i][0]
        t1 = time()
        qr_uts = get_uts( triples, qr )
        dic = qrs_count( triples, qr_uts,qr )
        t2 = time()
        print >>wf,"\n".join( [ "[%s]\t%s" % (k,' ,'.join(v)) for k,v in dic.items()] )
        sn = sum( [len(v) for v in dic.values()] )
        li.append( sn )
        print >>wf,"neighbours of ",qr," :", sn 
        print >>wf,"time cost:",t2-t1
        
    print >>wf, "avg neighbours :", float(sum(li))/len(li)
