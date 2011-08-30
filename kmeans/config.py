#coding=utf8

# proprecess
corpus_path = "200.txt"
voca_path = "tmp/voca.txt"
new_docs_path = "tmp/new_docs.txt"

min_word_tf = 2                 # 最小字频，低于此字频的词过滤

# kmeans 
num_iter = 10
min_sim_centers = 0.99          # kmeans迭代终止条件：两次的clusters中心相似度
cluster_path = "tmp/clusters.txt"
min_doc_center_sim = 0.0        # 最小文档和中心的相似度，小于此值的文档归于其他类--TODO
max_centers_sim = 0.8           # 如果有两个cluster中心的相似度超过此值，合并之
init_k = 50                    # 初始的k，尽量大

# post process
new_cluster_path = "clusters_result.txt"
num_center_words = 50           # how many words display


