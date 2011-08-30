import sys

if __name__=='__main__':
    assert(len(sys.argv)==2)
    path = sys.argv[1] 
    f=open(path)
    d=[]
    for l in f:
        s = l.split()[0]
        if len(s)>10 and s not in d:
            d.append(s)
            
    f.close()
    f = open(path,'w')
    f.write('\n'.join(d))
            
