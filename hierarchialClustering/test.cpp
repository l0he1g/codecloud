#include <iostream>
#include "Clustering.h"

//test
int main() {
    //	Clustering job("D:\\vc\\cacb\\Debug\\config.in");
	Clustering job("config.in");
    if (job.initialize()) {
        job.run();
        job.write_clusters();
    }
}
