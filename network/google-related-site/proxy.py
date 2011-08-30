#coding=utf8
# manual validate:wget -e "http_proxy=http://pg.whcsc.edu.cn:8080" http://baidu.com
import sys
from random import randint

class CProxys:
    def __init__(self,path='proxy.in'):
        self.path = path
        self.idx = 0
        self.proxys = []
        self.read_proxys(path)
        
    # read proxy ips
    def read_proxys(self,path='proxy.in'):
        f = open( path )
        self.proxys = [l.strip() for l in f if l.strip()]

    # seelct one proxy one by one
    def get_proxy(self):
        p = self.proxys[self.idx]
        self.idx = (self.idx+1) % len(self.proxys)
        return p

    # seelct one proxy by random
    def get_proxy_rand(self):
        l = len(self.proxys)
        p = randint(0,l-1)
        return self.proxys[p]

    def del_proxy( self,proxy ):
        self.proxys.remove( proxy )
    
    def save_proxy(self):
        f = open( self.path,'w' )
        f.write( '\n'.join(self.proxys) )

    def num(self):
        return len( self.proxys )
    
    def validate(self,test_url='http://sogo.com/'):
        import urllib2
        from urllib2 import URLError
        urllib2.socket.setdefaulttimeout(6) #10s
        len_p = len(self.proxys)
        print " all proxys num:",len_p,':[V]alid or [U]nvalid'
        for i in range(len_p-1,-1,-1):
            req = urllib2.Request( test_url )
            req.set_proxy( self.proxys[i] ,'http')
            print "test[%d/%d]:" % (i,len_p),self.proxys[i],'--',
            try:
                r = urllib2.urlopen(req)
                if len(r.read())>0:
                    print "V."
                else:
                    print "U."
                    del self.proxys[i]
            except:
                print "Unvalid."
                del self.proxys[i]

        print " valid proxys num:",len(self.proxys)

if __name__=='__main__':
    if len(sys.argv)>1:
        print 'load proxies file:',sys.argv[1]
        cp = CProxys(sys.argv[1])
    else:
        cp = CProxys()
        
    cp.validate()
    cp.save_proxy()
