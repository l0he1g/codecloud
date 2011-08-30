#coding=utf8
import numpy as np
import os
import psyco
psyco.full()
#! must make sure I>L,J>M,K>N

# normaliztion of 1-dimension
def normalize(a, out=None):
    if out is None: out = np.empty_like(a)
    s = np.sum(a)
    if s != 0.0 and len(a) != 1:
        np.divide(a, s, out)
    return out

# A is dict:{(i,j,k):v}
# P_lmn is Tensor:
# P_i_l,P_j_m,P_k_n is dict:
def loglikelihood(A, P_lmn, P_i_l, P_j_m, P_k_n):
    likelihood = 0.0
    sum_a = float( sum( A.values() ) )
    for key,v in A.items(): # for indexs of all nonzero items,reduce compute cost
        i,j,k = key
        tmp = 0.0
        for l in range(L):
            for m in range(M):
                for n in range(N):
                    tmp += P_lmn[l,m,n] * P_i_l[i][l] * P_j_m[j][m] * P_k_n[k][n]
 #       print "p(%d,%d,%d) = %f [%f]" % (i,j,k,tmp, A[key]/sum_a )
        assert( tmp>0 )
        likelihood += v * np.log( tmp )
    return likelihood

def com_p_lmn( P_lmn_ijk, A ):
    R = np.zeros((L, M, N))
    ii = 0
    for key,P in P_lmn_ijk.items():
        R += A[key]*P
        
    return normalize( R )

# return a dict: {i:vector,...}
# such as P_i_l or P_j_m or P_k_n
def com_p2( P_lmn_ijk, A, index ):
    if index ==0:
        axis = [1,1]
    elif index == 1:
        axis = [0,1]
    else:
        assert index==2
        axis = [0,0]
    
    # init R
    R = {}
    # sum on j,k
    ii = 0
    for k,v in A.items():
        i = k[index]
        ii += 1

        if R.has_key(i):
            R[i] += A[k]*P_lmn_ijk[k]
        else:
            R[i] = np.zeros((L,M,N))
            R[i] += A[k]*P_lmn_ijk[k]
            
    tmp_key = R.keys()[0]
    dim_i = R[tmp_key].shape[index] 
    sum_li = np.zeros( dim_i )
    tmp = 0.0
    # sum on m,n
    for k,v in R.items():
        R[k] = np.sum(R[k], axis=axis[0] )
        R[k] = np.sum(R[k], axis=axis[1] )
        # normalize
        sum_li +=  R[k]
        
    # normarlize for P(i|l)
    for k,v in R.items():
        R[ k ] /= sum_li
        
    #test
    sum_li = np.zeros( dim_i )
    for k,v in R.items():
        sum_li += v
        
    assert np.absolute(np.sum( sum_li )-dim_i)<0.01
    return R

def m_setp( P_lmn_ijk, A ):
    print "m 1:compute P(lmn)"
    P_lmn = com_p_lmn( P_lmn_ijk, A )
    assert np.absolute(np.sum( P_lmn ) - 1)<0.01
    
    print "m 2:compute P(i|l)"
    P_i_l = com_p2( P_lmn_ijk, A, 0 )

    print "m 3:compute P(j|m)"
    P_j_m = com_p2( P_lmn_ijk, A, 1 )
    
    print "m 4:compute P(k|n)"
    P_k_n = com_p2( P_lmn_ijk, A, 2 )
    
    return P_lmn, P_i_l, P_j_m, P_k_n

# P_lmn is a L x M x N Tensor
# P_i_l is a dict:{'i':vector_l,...}, the same are P_j_m, P_k_n
# retrun a dict:
#       P_lmn_ijk:{'ijk':tensor,...}
def e_step( P_lmn, P_i_l, P_j_m, P_k_n, A):
    P_lmn_ijk = {}
    
    ii = 0
    for key in A.keys():
        ii += 1
        # print "num %d computation at i,j,k: %d %d %d" % (ii,key[0],key[1],key[2])
        P_lmn_ijk[key] = np.zeros((L, M, N))
        i,j,k = key
        for l in range(L):
            for m in range(M):
                for n in range(N):
                    P_lmn_ijk[key][l,m,n] = P_lmn[l,m,n] * P_i_l[i][l] * P_j_m[j][m] * P_k_n[k][n]
        P_lmn_ijk[key] = normalize( P_lmn_ijk[key] )
        assert np.absolute( np.sum(P_lmn_ijk[key])-1)<0.01
    return P_lmn_ijk

# normalize a P(i|j)
# d_v is a dict:{i:vector,...}
def normalize_2( d_v ):
    keys = d_v.keys()
    sum_li = np.zeros( len(d_v[keys[0]]) )
    for k in keys:
        sum_li += d_v[ k ]
        
    for k in keys:
        d_v[ k ] /= sum_li
    return d_v

# init p by random value
# return
#     P_lmn is a L x M x N Tensor
#     P_i_l is a dict:{'i':vector_l,...}, the same are P_j_m, P_k_n
def random_init( A ):
    P_lmn = normalize( np.random.rand(L,M,N) )
    P_i_l = {}
    P_j_m = {}
    P_k_n = {}
    for key in A.keys():
        i,j,k = key
        if not P_i_l.has_key( i ):
            P_i_l[i] = np.random.rand(L)
        if not P_j_m.has_key( j ):
            P_j_m[j] = np.random.rand(M)
        if not P_k_n.has_key( k ):
            P_k_n[k] = np.random.rand(N)

    P_i_l = normalize_2( P_i_l )
    P_j_m = normalize_2( P_j_m )
    P_k_n = normalize_2( P_k_n )
    return P_lmn,P_i_l,P_j_m,P_k_n

def tensor_str( t ):
    str_li = []
    for l in range(L):
        for m in range(M):
            for n in range(N):
                str_li.append( "%d\t%d\t%d\t%f" % (l,m,n,t[l,m,n]) )
     
    return str_li

# write a CPD
#     P is a dict:{i:vector_l,...}, such as P_i_l, P_j_m, P_k_n
def write_p2( P, path ):
    outf = open( path, 'w')
    tmp_li = sorted( P.items(), key=lambda d:d[0] )
    str_li = []
#    outf.write( "#i\tl\tp(i|l)\n")
    for key,v in tmp_li:
        for i in range(len(v)):
            str_li.append( "%d\t%d\t%f" % (key,i,v[i]) )
    outf.writelines( '\n'.join( str_li  ))
    outf.close()

# write to 4 file
def  write_res( P_lmn_ijk, P_lmn, P_i_l, P_j_m, P_k_n ):
    # write P_lmn_ijk----not need,but heavy
#     path = dir_path + 'p_lmn_ijk.txt'
#     print "write P_lmn_ijk to: ",path
#     outf = open( path, 'w')
# #    outf.write( "#i\tj\tk\t\tl\tm\tn\tp(lmn|ijk)\n")
#     tmp_str = ''
#     tmp_li = sorted( P_lmn_ijk.items(), key=lambda d:d[0] )
#     for key,v in tmp_li:
#         i,j,k = key
#         str_li = tensor_str( P_lmn_ijk[key] )
#         str_li = ["%d\t%d\t%d\t%s" % (i,j,k,s) for s in str_li] 
#         outf.write( '\n'.join( str_li ) )
#         outf.write('\n')
#     outf.close()

    # write P_lmn
    path = dir_path+'p_lmn.txt'
    print "write P(lmn) to: ",path
    outf = open( path, 'w')
#    outf.write( "#l\tm\tn\tp(lmn)\n")
    str_li = tensor_str( P_lmn )
    outf.writelines( '\n'.join( str_li ) )
    outf.close()
    
    # write P_i_l
    path = dir_path + 'p_i_l.txt'
    print "write P(i|l) to: ",path
    write_p2( P_i_l, path )

    # write P_j_m
    path = dir_path + 'p_j_m.txt'
    print "write P(j|m) to: ",path
    write_p2( P_j_m, path )

    # write P_k_n
    path = dir_path + 'p_k_n.txt'
    print "write P(k|n) to: ",path
    write_p2( P_k_n, path )

def train( A, maxiter=100, eps=0.01 ):
    print "init"
    P_lmn,P_i_l,P_j_m,P_k_n = random_init(A)
    
    P_lmn_ijk = {}
    old_like = loglikelihood(A, P_lmn, P_i_l, P_j_m, P_k_n)
    print "init likihood = %f" % old_like

    for looper in range( maxiter ):
        print "\nbegin %d iteration" % looper
        
        print "e-setp"
        P_lmn_ijk = e_step( P_lmn, P_i_l, P_j_m, P_k_n, A)
        
        print "m-setp"
        P_lmn,P_i_l,P_j_m,P_k_n = m_setp( P_lmn_ijk, A )

        print "compute likelihood"
        cur_like= loglikelihood(A, P_lmn, P_i_l, P_j_m, P_k_n)
        diff_like = cur_like - old_like
        print "likelihood progress = %f - %f = %f" % (cur_like,old_like,diff_like)
        old_like = cur_like
        
        if diff_like < eps:
            print "No more progress, stopping EM at iteration", looper
            break
        
    # write result
    write_res( P_lmn_ijk, P_lmn, P_i_l, P_j_m, P_k_n )

# input format:i   j   k   v
# reutrn dict A:{{i,j,k):v,...}
def read_data( path ):
    inf = open( path )
    A = {}
    for line in inf:
        if not line.startswith( '#' ):
            items = line.split('\t')
            items = [ int(i) for i in items]
            A[tuple(items[:3])] = items[-1]
    inf.close()
    return A

if __name__=='__main__':
    path = '/home/xh/mypaper/ppf/code/msn/result/voca_triples.txt'
    L = 30
    M = 30
    N = 30
    dir_path = "%d_%d_%d" % (L,M,N) 
    os.mkdir( dir_path )
#    path = 'test.txt'
    A = read_data( path )
    print "len of A:",len(A)
    train(A)
    print "finished :)"
