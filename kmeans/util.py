#coding=utf8

# 判断一个unicode字符是否是
# para:
#     ch unicode编码的一个字符
def is_ch_char( ch ):
    if not isinstance( ch,unicode ):
        raise RuntimeError,'Parameter is not unicode:'+ch
    if len(ch)!=1:
        raise RuntimeError,'Parameter is not a char:'+ch
    return (ch >= u'\u4e00' and ch<=u'\u9fa5')

def readfile( path ,codeset='gb2312'):
    print "[begin] read data from: %s" % path
    data = []
    inf = open( path ,'rb')
    for line in inf:
        if line.startswith('#'): # 注释行
            continue
        try:
            line = line.strip().decode( codeset )
            if line :
                data.append( line )
        except:
            pass
    inf.close()
    return data

if __name__ =='__main__':
    data = readfile( '/tmp/test.txt'  )
    for l in data:
        print l
