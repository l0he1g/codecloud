#coding=utf8
# FUNCTION:将聚类结果，整合原doc显示
from config import *
from util import readfile
from preprocess import readvoca

# cluster format:{doc_id:sim,...}
def read_clusters():
    data = readfile( cluster_path )
    clusters = [] 
    centers = []
    for line in data:           # 每一行一个cluster
        if line.startswith('center:'): # 这是一个类中心
            center = {}
            items = line[8:-1].split(',')
            for item in items:
                item = item.strip()
                if item:
                    wid,weight = item.split(':')
                    center[ int( wid.strip() ) ] = float(weight.strip())
                pass
            centers.append( center )
        else:
            c = {}
            docs = line.split(',')
            for doc in docs:
                id,sim = doc.split(':')
                id = int(id)
                sim = float( sim )
                c[id] = sim
                pass
            clusters.append( c )
    return clusters,centers

def post_run():
    docs = readfile( corpus_path )
    clusters,centers = read_clusters()
    voca = readvoca()
    
    outf = open( new_cluster_path,'w' )
    for i in range(0,len(centers)):
        c = clusters[i]
        # outf.write( 30*'#'+'cluster '+str(i)+30*'#'+'\n' )
        # # subsitute wordid
        # words = sorted(centers[i].items(),key=lambda d:d[1],reverse=True)
        # outf.write("# ")
        # for wid,weight in words[:num_center_words]:
        #     outf.write( voca[ wid ].encode('gb2312') )
        #     outf.write( '[%f] ' % weight )
        #     pass
        # outf.write("\n")
        
        tmp_li = sorted( c.items(),key=lambda d:d[1],reverse=True)
        for doc_id,sim in tmp_li:
            outf.write( docs[ doc_id ].encode('gb2312')+"\n" )
        #    outf.write( '[%f]\n' % sim )
        pass
    # the other cluster
    c = clusters[-1]
    #outf.write( 30*'#'+'cluster others'+30*'#'+'\n' )
    tmp_li = sorted( c.items(),key=lambda d:d[1],reverse=True)
    for doc_id,sim in tmp_li:
        outf.write( docs[ doc_id ].encode('gb2312') )
    #    outf.write( '[%f]\n' % sim )
    print "[finished]result has been writen to %s" % new_cluster_path
    
if __name__ == '__main__':
    post_run()
