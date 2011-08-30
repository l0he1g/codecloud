#coding=utf8
from util import is_ch_char,readfile
from config import *

# 第一遍读文档，编写字典(term为单个中文字)
# return:{word:num,...}
def collect_voca():
    voca = {}
    docs = readfile( corpus_path )
    for doc in docs:
        doc = doc.split()[-1]   # input format has been changed
        for char in doc:
            if is_ch_char( char ): # only take care of Chinese char
                if char in voca:
                    voca[char] += 1 # count occurrence of a char
                else:
                    voca[char] = 1 
        pass
    # write voca to file
    outf = open( voca_path,'w')
    voca_li = sorted( voca.items(),key=lambda d:d[1],reverse=True )
    i = 0
    for w,n in voca_li:
        outf.write( "%d\t%s\t%d\n" % (i,w.encode("gb2312"),n) )
        i += 1
    print "[finished]write voca to %s" % voca_path

# 第二遍读文档，将文档转换成以下格式
# doc_id:w_id:num,w_id:num,...
def transform_docs():
    docs = readfile( corpus_path )
    outf = open( new_docs_path,'w')
    voca = readvoca()
    outf.write( "# docs num %d,voca num:%d\n" % (len(docs),len(voca)) )
    i = 0
    for doc in docs:
        doc = doc.split()[-1]   # input format has been changed
        doc_wn = {}             # 统计doc中的每个词的词频
        for ch in doc:
            if is_ch_char( ch ) and ch in voca:
                v_id = voca.index( ch )
                if doc_wn.has_key( v_id ):
                    doc_wn[v_id] += 1 # 第i个词又一次出现
                else:
                    doc_wn[v_id] = 1 # 第i个词第一次出现
                pass
        # 将doc_wn写文档
        #print "write new doc:%d" % i
        words_li = ["%d:%d" % (w,n) for w,n in doc_wn.items() ]
        words_str = ','.join( words_li )
        outf.write( "%d\t%s\n" % (i,words_str) )
        i += 1
        pass
    outf.close()

def readvoca():
    voca = []
    lines = readfile( voca_path )
    for line in lines:
        id,w,n = line.split()
        n = int(n)
        if n>1:
            voca.append( w )
    return voca

def pre_run():
    print "1). collect_voca()"
    collect_voca()
    print "2). transform_docs()"
    transform_docs()
    print "new docs have been writen to  %s" % new_docs_path
    
if __name__=="__main__":
    pre_run()
