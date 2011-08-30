#coding=utf8

from preprocess import pre_run
from kmeans import kmeans_run
from postprocess import post_run

if __name__ == "__main__":
    # preprocess
    print "1. preprocess"
    pre_run()
    
    # kmeans
    print "\n2. begin kmeans clustering"
    kmeans_run()

    # 3. postprocess
    print "\n3. postprocess "
    post_run()

    print "done."
