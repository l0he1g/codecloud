#coding=utf8
# crawl unrecorded host in dmoz from google related search
import urllib2
from BeautifulSoup import BeautifulSoup,NavigableString,Comment
import traceback
import time,sys,re,os
from proxy import *
from multiprocessing import Pool


opener = urllib2.build_opener()
opener.addheaders = [('User-agent', 'Mozilla/5.0')]
urllib2.install_opener(opener)
urllib2.socket.setdefaulttimeout(10) #10s

def clean_url( url ):
    host = url.string.strip()
    
    if host.startswith('www.'):
        host = host[4:]
    tmp_p = host.find('/')
    if tmp_p!=-1:
        host = host[:tmp_p]

    return host

def writef(html):
     f=open('a.html','w')
     f.write(html)
     
# no retry
def crawl_hosts( host,proxy ):
     url = 'http://www.google.com.hk/search?q=related:www.%s' % host
     hosts = []
     try:
          req = urllib2.Request( url )
          req.set_proxy( proxy ,'http')
          print "\tproxy:",proxy,
          r = urllib2.urlopen(req)
          html = r.read()
          soup = BeautifulSoup( html )
          links = soup.findAll( 'cite' )
          hosts = [clean_url(l) for l in links if l and l.string]
          print "\thosts num:",len(hosts)
     except:
          erro,errv = sys.exc_info()[:2]
          print '\n\t[Faild]',erro,':',errv
     return hosts

# read uncated hosts from uncate_hosts.txt
def read_hosts( path ):
     f = open(path)
     return [l.strip() for l in f if l.strip()]

def ascii_s( s ):
     if re.search(r'[^\x00-\xff]',s ):
          return True
     else:
          return False

def run( paras ):
    hosts,id = paras
    getf = open( 'output/s.%d' % id,'w')
    faildf = open( 'output/f.%d' % id,'w')
    
    cp = CProxys( proxy_path )
    print "pid %s read proxy file:%s, num:%d" % (os.getpid(),proxy_path,cp.num())
    
    for h in hosts:
        try:
            print "[%d/%d] pid %s crawl related hosts of :" % (hosts.index(h),len(hosts),os.getpid()),h
            proxy =  cp.get_proxy_rand()
            r_hosts = crawl_hosts( h,proxy )
            retry = 0
#          r_hosts = filter( ascii_s, r_hosts )
            while len(r_hosts)==0 and retry<2:   # failed,we dont consider proxys reason since they are validated first
                print "[%d/%d] pid %s retry hosts of :" % (hosts.index(h),len(hosts),os.getpid()),h
                proxy =  cp.get_proxy_rand()
                r_hosts = crawl_hosts( h,proxy )
                retry += 1
                pass
            if len(r_hosts)==0:
                faildf.write( '%s\n' % h )
            else:
                getf.write( '%s\t%s\n' % (h,','.join(r_hosts)) )
        except:
            pass

if __name__=='__main__':
     if len(sys.argv)<3:
          print "Usage: python",sys.argv[0],' file1 file2 file3 file4'
          print "file1: [in] uncated host file"
          print "file2: [in] proxys file"
          print "output in output/s,f"
          exit("Bad parameters.")
     
     hosts_path,proxy_path = sys.argv[1:]

     hosts = read_hosts( hosts_path )
     print "read hosts file:%s, num:%d" % (hosts_path,len(hosts))
     
     
     pool = Pool(processes=10)
     paras = []
     chunksize = 200
     for i in range(len(hosts)/chunksize+1):
         start = i*chunksize
         end = (i+1)*chunksize
         paras.append( (hosts[start:end],i) )
         
     pool.map(run, paras) 
