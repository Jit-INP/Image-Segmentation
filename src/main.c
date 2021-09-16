#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "imgoper.h"
#include "utils.h"

typedef enum{
    eBinConv = 0,
    eMedFilter,
    eHistStch,
    eHistEq,
    eScharrEdge,
    eCannyEdge,
    eKMeans,
    eSpatialKMeans,
    eNbImgOper
}ImgOper;

void printHelpNExit(const char *fileNam){
    printf("\nUsage: %s filename operation option\n\n", fileNam);
    printf("Operations:\n");
    printf("0 n( > 0): n times Convolution With Binomial Filter\n");
    printf("1 n( > 0): n times Median Filtering\n");
    printf("2 : Histogram Stretching\n");
    printf("3 : Histogram Equalization\n");
    printf("4 (0 < n < 255): Scharr Edge Detection\n");
    printf("5 : Canny Edge Detection\n");
    printf("6 k( > 0): K-Means\n");
    printf("7 k( > 0): Spatial K-Means\n");
    printf("8 : All of the above(with default values)\n");
    exit(0);
}

#ifdef IDE

#include "convol.h"
#include "filter.h"
#include "hist.h"
#include "matrix.h"
#include "median.h"
#include "kmeans.h"

int main(){
    //    doCannyEdgeDet("emma.pgm");
    //    doAll("emma.pgm", 1);
    // doScharrEdgeDet("wheel.pgm", 1);
    doKMeans("emma.pgm", 10);
    doKMeans("frog.ppm", 10);
    doSpatialKMeans("emma.pgm", 10);
    doSpatialKMeans("frog.ppm", 10);
    return 0;
}
#else
int main(int argc, char **argv){
    if((argc > 4) || (argc < 3)){
        printHelpNExit(argv[0]);
    }
    char ipFilNam[MAX_FIL_NAM_SIZ];
    strncpy(ipFilNam, argv[1], MAX_FIL_NAM_SIZ);
    int oper = atoi(argv[2]);
    int subOper = 1;
    if(argc == 4){
        if( ( subOper = atoi(argv[3]) ) < 1 ){
            printHelpNExit(argv[0]);
        }
    }
    switch (oper) {
    case eBinConv:
        doBinConv(ipFilNam, subOper);
        break;
    case eMedFilter:
        doMedianFilter(ipFilNam, subOper);
        break;
    case eHistStch:
        doHistStch(ipFilNam);
        break;
    case eHistEq:
        doHistEq(ipFilNam);
        break;
    case eScharrEdge:
        doScharrEdgeDet(ipFilNam, subOper);
        break;
    case eCannyEdge:
        doCannyEdgeDet(ipFilNam);
        break;
    case eKMeans:
        doKMeans(ipFilNam, subOper);
        break;
    case eSpatialKMeans:
        doSpatialKMeans(ipFilNam, subOper);
        break;
    case eNbImgOper:
        doAll(ipFilNam);
        break;
    default:
        printHelpNExit(argv[0]);
        break;
    }
    return 0;
}
#endif
