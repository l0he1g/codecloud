#coding=utf8
# FUNCTION:k-means聚类
# DESCRIPTION:预先指定k个类，程序会自动生成一个其他类，用以包含和其他所有文档相似度都很低的文档。“其他类”不具备类中心，默认为0
# DATA STRUCTURE
#    doc format: {'id':n,'words':{w_id:n,w_id,...}} 常量
#    cluster format:{'center':{w_id:n,...},'docs':{doc_id:simility,...}}
from math import sqrt
from config import *
from util import readfile
from random import randrange

# 读取docs
def get_docs():
    print "[begin] get_docs ..."
    docs = []
    lines = readfile( new_docs_path )
    for i in range(0,len(lines)):
        doc = {'id':i,'words':{}}
        items = lines[i].split()
        if len(items)==2:
            word_nums = items[1].split(',')
            for w_n in word_nums:
                w,n = w_n.split(':')
                w = int( w )
                doc['words'][w] = int(n)
            pass
        docs.append( doc )
    return docs

def kmeans( cnum,docs ):
    print "[begin] kmeans ..."
    docs_num = len( docs )
    if cnum > docs_num:
        print "Error:k=%d is larger than docs num:%d!" % (k,docs_num)
        return 
    
    print "[begin] init_centers ..."
    clusters = init_centers( cnum,docs )
    assign_points( docs,clusters ) # change docs
    while 1:
        # 一轮k-means
        num_clusters = len( clusters )-1
        print "A new round of different k=%d" % num_clusters
        j=0
        while 1:
            # 记录上次的cluster centers
            last_centers = [c['center'] for c in clusters if c['center']]
            
            # 重新计算类中心
            print "[%d] re_center ..." % j
            re_center( docs,clusters )
            this_centers = [c['center'] for c in clusters if c['center']]
            
            # 根据新类中心，再次聚类
            print "[%d] assign_points ..." % j
            assign_points( docs,clusters ) # change docs
        
            # 根据与上次cluster centers相似度判断迭代是否该终止
            sim_c = sim_centers( last_centers,this_centers )
            print "similarity with last clusters: %f" % sim_c
            if sim_c > min_sim_centers :       # 迭代num_iter次
                break                # 聚类终止条件
        
            j += 1
            pass
        print "[begin]compute similarity between two clusters"
        # 若有必要，合并一些类
        for i in range(0,num_clusters-1):
            center_i = clusters[i]['center']
            for j in range(i+1,num_clusters):
                center_j = clusters[j]['center']
                tmp_sim = similarity( center_i,center_j )
                if tmp_sim > max_centers_sim:
                    print "Combine clusters %d and %d with simility:%f" % (i,j,tmp_sim)
                    # 合并cluster i到j，并清空cluster i
                    clusters[j]['docs'].update( clusters[i]['docs'] )
                    clusters[i]['docs'].clear()
                else:
                    #print "simility of Clusters %d and %d is:%f" % (i,j,tmp_sim)
                    pass
        clusters = [ c for c in clusters if c['docs'] ]
        new_num_clusters = len(clusters)-1
        print "after combine,left clusters:%d" % new_num_clusters
        if new_num_clusters == num_clusters:
            print "==>Get the right k=%d" % new_num_clusters
            break               # 退出寻k过程
        pass                    # end while
    finish( clusters )

# 计算两个文档集合的相似度：每个文档的相似度的平均值
# words_li:[{wid:num,...},...]
def sim_centers( li1,li2 ):
    assert len(li1) == len(li2)
    
    sim_c = 0.0
    for i in range(0,len(li1)):
        words = li1[i]
        max_sim = 0.0
        rj = 0
        for j in range(0,len(li2)):
            tmp_sim = similarity( words,li2[j])
            if max_sim < tmp_sim:
                max_sim = tmp_sim
                rj = j
            pass
        sim_c += max_sim
        del li2[rj]
    return sim_c/len(li1)

def init_centers( cnum,docs ):
    # 选择前k不为空文档个点作为类中心,且必须保证任
    # 两个类中心不相等
    clusters = []
    i = 0
    centers = []
    while len(clusters) < cnum:
        i = randrange( 0,len(docs) )
        words = docs[i]['words']
        if words and words not in centers:
            c = {'center':words,'docs':{} }
            clusters.append( c ) 
            centers.append( words )
            pass
        i += 1
    # add other cluster
    other = {'center':{},'docs':{} }
    clusters.append( other )

    return clusters

# 将文档分给距离它最近的类中心
def assign_points( docs,clusters ):
    for c in clusters:
        c['docs'].clear()      # 清除原分配
    cnum = len(clusters) - 1
    
    # 计算文档与个类中心的距离
    for doc in docs:
        if not doc['words']:             # 空文档加入到other类
            clusters[cnum]['docs'][doc['id']] = 0
            continue
        
        tmp_c = clusters[ cnum ]    # 记录cluster,默认分配给其他类
        max_sim = min_doc_center_sim                 
        for c in clusters:
            tmp_sim = similarity( doc['words'],c['center'] )
            if tmp_sim > max_sim:
                tmp_c = c
                max_sim = tmp_sim
            pass
        # 将doc分配给tmp_c
        tmp_c['docs'][doc['id']] = max_sim
        pass
    return clusters

# 对一个{w:n,w:n,...}的dict normalize
def normalize( doc ):
    sq = sqrt( sum( [v*v for v in doc.values()] ) )
    for w,n in doc.items():
        doc[w] = n/sq
        
# 计算doc1和doc2之间的相似度(cosine)
# doc1:{w:n,w:n,...}
# return float
def similarity( doc1,doc2 ):
    # 如果doc1,doc2之中有一个是空文档，返回0
    if not doc1 or not doc2:
        return 0.0
    
    # normalize
    normalize( doc1 )
    normalize( doc2 )
    
    numerator = 0.0
    for w,n in doc1.items():
        if doc2.has_key( w ):
            numerator += doc1[w]*doc2[w]
    
    return numerator

# 对聚类结果重新计算类中心
# 一个cluster:[{1: 1, 2: 2, 3: 3}, {1: 1, 2: 1}]
def re_center( docs,clusters ):
    cnum = len( clusters ) - 1
    for i in range(0,cnum):
        # 计算每个cluster的中心
        docs_num = len( clusters[i]['docs'] )
        assert docs_num > 0
        center = {}               # 新类中心
        for doc_id in clusters[i]['docs'].keys():
            c_doc = {}          # docs中找具有doc_id的doc的词列表
            for tmp_doc in docs:
                if tmp_doc['id'] == doc_id:
                    c_doc = tmp_doc
                    break
            # 累加clusters中各doc中的word权重给center
            for w,n in c_doc['words'].items():
                if center.has_key( w ):
                    center[w] += n
                else:
                    center[w] = n
        # 产生新的cluster中心
        for w,n in center.items():
            center[w] = float(center[w])/docs_num
        # 更新cluster的类中心
        clusters[i]['center'] = center
    return clusters
    

def finish( clusters ):
    print "[begin] write clusters to: %s" % cluster_path
    outf = open( cluster_path,'w' )
    for i in range(0,len(clusters)-1):
        outf.write( 30*'#'+'cluster '+str(i)+30*'#'+'\n' )
        outf.write( "center:"  + str(clusters[i]['center'])+'\n' )
        docs_str = ','.join( [ "%d:%s" % (id,s) for id,s in clusters[i]['docs'].items() ] )
        outf.write( "%s\n" % docs_str )
        pass
    # write cluster "other"
    outf.write( 30*'#'+'cluster others '+30*'#'+'\n' )
    docs_str = ','.join( [ "%d:%s" % (id,s) for id,s in clusters[-1]['docs'].items() ] )
    outf.write( "%s\n" % docs_str )
    
    print "[finished] write clusters result to %s:)" % cluster_path

def kmeans_run():
    docs = get_docs()
    # test for kmeans
    kmeans( init_k,docs )
    
if __name__ == '__main__':
    kmeans_run()
