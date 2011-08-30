% tensor factorization

M = dlmread('test_data/test.txt');

msp = sptensor( M(:,1:3)+1, M(:,4) );

msr = tucker_als(msp,[2 2 1]);

tensor(msr)


