#include "imgoper.h"

#include <stdio.h>
#include <string.h>

#include "convol.h"
#include "median.h"
#include "pgmfil.h"
#include "matrix.h"
#include "filter.h"
#include "hist.h"
#include "edge.h"
#include "kmeans.h"

void mkOpName(char *opNam, const char *opStr, const char nbPref, const char *ipNam, size_t strSiz){
    char *ipFilNamWoExt = malloc(strlen(ipNam) + 1 - 3);
    char *ext = malloc(3 + 1);
    const char *ipPtr = ipNam;
    char *ipFWEPtr = ipFilNamWoExt;
    char *extPtr = ext;
    bool befExt = true;
    while(*ipPtr != 0){
        if(befExt == true){
            *ipFWEPtr = *ipPtr;
            ++ipFWEPtr;
        } else {
            *extPtr = *ipPtr;
            ++extPtr;
        }
        ++ipPtr;
        if(*ipPtr == '.'){
            befExt = false;
            *ipFWEPtr = 0;
        }
    }
    *extPtr = 0;
    snprintf(opNam, strSiz, "%s%s%d%s", ipFilNamWoExt, opStr, nbPref, ext);
    free(ipFilNamWoExt);
    free(ext);
}

void doBinConv(const char* ipFilNam, size_t n){
    mat *imgMat;
    size_t row, col, ch, maxVal;
    PGMType type;

    pgmToImgMat(ipFilNam, &imgMat, &row, &col, &ch, &maxVal, &type);
    GRAY_IMG_CHK(ch);

    mat *biConv = allocMatMem(row, col, ch);
    binomFilConvolN(biConv, imgMat, row, col, n);

    char convOpFilNam[MAX_FIL_NAM_SIZ];
    mkOpName(convOpFilNam, "BiConv", FILTER_SIZ, ipFilNam, sizeof (convOpFilNam));
    imgMatToPgm(convOpFilNam, biConv, row, col, ch, maxVal, type);
    free(imgMat);
    free(biConv);
}

void doScharrEdgeDet(const char* ipFilNam, size_t gradTh){
    mat *imgMat;
    size_t row, col, ch, maxVal;
    PGMType type;

    pgmToImgMat(ipFilNam, &imgMat, &row, &col, &ch, &maxVal, &type);
    GRAY_IMG_CHK(ch);

    mat *xComp = allocMatMem(row, col, ch);
    mat *yComp = allocMatMem(row, col, ch);
    mat *grad = allocMatMem(row, col, ch);

    scharrEdgeDet(imgMat, xComp, yComp, grad, gradTh, row, col);

    char convXOpFilNam[MAX_FIL_NAM_SIZ];
    mkOpName(convXOpFilNam, "ScharrX", FILTER_SIZ, ipFilNam, sizeof (convXOpFilNam));
    imgMatToPgm(convXOpFilNam, xComp, row, col, ch, maxVal, type);

    char convOpYFilNam[MAX_FIL_NAM_SIZ];
    mkOpName(convOpYFilNam, "ScharrY", FILTER_SIZ, ipFilNam, sizeof (convOpYFilNam));
    imgMatToPgm(convOpYFilNam, yComp, row, col, ch, maxVal, type);

    char convOpGFilNam[MAX_FIL_NAM_SIZ];
    mkOpName(convOpGFilNam, "ScharrG", FILTER_SIZ, ipFilNam, sizeof (convOpGFilNam));
    imgMatToPgm(convOpGFilNam, grad, row, col, ch, maxVal, type);

    free(imgMat);
    free(xComp);
    free(yComp);
}

void doSobelEdgeDet(const char* ipFilNam, size_t gradTh){
    mat *imgMat;
    size_t row, col, ch, maxVal;
    PGMType type;

    pgmToImgMat(ipFilNam, &imgMat, &row, &col, &ch, &maxVal, &type);
    GRAY_IMG_CHK(ch);

    mat *xComp = allocMatMem(row, col, ch);
    mat *yComp = allocMatMem(row, col, ch);
    mat *grad = allocMatMem(row, col, ch);

    sobelEdgeDet(imgMat, xComp, yComp, grad, gradTh, row, col);
    free(imgMat);

    char convXOpFilNam[MAX_FIL_NAM_SIZ];
    mkOpName(convXOpFilNam, "SobelX", FILTER_SIZ, ipFilNam, sizeof (convXOpFilNam));
    imgMatToPgm(convXOpFilNam, xComp, row, col, ch, maxVal, type);
    free(xComp);

    char convOpYFilNam[MAX_FIL_NAM_SIZ];
    mkOpName(convOpYFilNam, "SobelY", FILTER_SIZ, ipFilNam, sizeof (convOpYFilNam));
    imgMatToPgm(convOpYFilNam, yComp, row, col, ch, maxVal, type);
    free(yComp);

    char convOpGFilNam[MAX_FIL_NAM_SIZ];
    mkOpName(convOpGFilNam, "SobelG", FILTER_SIZ, ipFilNam, sizeof (convOpGFilNam));
    imgMatToPgm(convOpGFilNam, grad, row, col, ch, maxVal, type);
    free(grad);
}

void doCannyEdgeDet(const char* ipFilNam){
    mat *imgMat;
    size_t row, col, ch, maxVal;
    PGMType type;

    pgmToImgMat(ipFilNam, &imgMat, &row, &col, &ch, &maxVal, &type);
    GRAY_IMG_CHK(ch);

    mat *filMat = allocMatMem(row, col, ch);

    mat *xComp = allocMatMem(row, col, ch);
    mat *yComp = allocMatMem(row, col, ch);
    mat *gradThComp = allocMatMem(row, col, ch);

    mat *nmsMat = allocMatMem(row, col, ch);

    mat *hysThMat = allocMatMem(row, col, ch);

    mat *edgeImg = allocMatMem(row, col, ch);

    cannyEdgeDet(imgMat, filMat, xComp, yComp, gradThComp, nmsMat, hysThMat, edgeImg, row, col);
    free(imgMat);

    char cannyFilOpFilNam[MAX_FIL_NAM_SIZ];
    mkOpName(cannyFilOpFilNam, "CannyFil", FILTER_SIZ, ipFilNam, sizeof (cannyFilOpFilNam));
    imgMatToPgm(cannyFilOpFilNam, filMat, row, col, ch, maxVal, type);
    free(filMat);

    char convXOpFilNam[MAX_FIL_NAM_SIZ];
    mkOpName(convXOpFilNam, "CannyX", FILTER_SIZ, ipFilNam, sizeof (convXOpFilNam));
    imgMatToPgm(convXOpFilNam, xComp, row, col, ch, maxVal, type);
    free(xComp);

    char convOpYFilNam[MAX_FIL_NAM_SIZ];
    mkOpName(convOpYFilNam, "CannyY", FILTER_SIZ, ipFilNam, sizeof (convOpYFilNam));
    imgMatToPgm(convOpYFilNam, yComp, row, col, ch, maxVal, type);
    free(yComp);

    char convOpGFilNam[MAX_FIL_NAM_SIZ];
    mkOpName(convOpGFilNam, "CannyG", FILTER_SIZ, ipFilNam, sizeof (convOpGFilNam));
    imgMatToPgm(convOpGFilNam, gradThComp, row, col, ch, maxVal, type);
    free(gradThComp);

    char nmsOpFilNam[MAX_FIL_NAM_SIZ];
    mkOpName(nmsOpFilNam, "CannyNMS", FILTER_SIZ, ipFilNam, sizeof (nmsOpFilNam));
    imgMatToPgm(nmsOpFilNam, nmsMat, row, col, ch, maxVal, type);
    free(nmsMat);

    char hysThOpFilNam[MAX_FIL_NAM_SIZ];
    mkOpName(hysThOpFilNam, "CannyHysTh", FILTER_SIZ, ipFilNam, sizeof (hysThOpFilNam));
    imgMatToPgm(hysThOpFilNam, hysThMat, row, col, ch, maxVal, type);
    free(hysThMat);

    char cannyOpFilNam[MAX_FIL_NAM_SIZ];
    mkOpName(cannyOpFilNam, "Canny", FILTER_SIZ, ipFilNam, sizeof (cannyOpFilNam));
    imgMatToPgm(cannyOpFilNam, edgeImg, row, col, ch, maxVal, type);
    free(edgeImg);
}

void doMedianFilter(const char* ipFilNam, size_t n){
    mat *imgMat;
    size_t row, col, ch, maxVal;
    PGMType type;

    pgmToImgMat(ipFilNam, &imgMat, &row, &col, &ch, &maxVal, &type);
    GRAY_IMG_CHK(ch);

    mat *med = allocMatMem(row, col, ch);
    medianN(med, imgMat, row, col, n);

    char medOpFilNam[MAX_FIL_NAM_SIZ];
    mkOpName(medOpFilNam, "Med", FILTER_SIZ, ipFilNam, sizeof (medOpFilNam));
    imgMatToPgm(medOpFilNam, med, row, col, ch, maxVal, type);
    free(imgMat);
    free(med);
}

void doHistStch(const char* ipFilNam){
    mat *imgMat;
    size_t row, col, ch, maxVal;
    PGMType type;

    pgmToImgMat(ipFilNam, &imgMat, &row, &col, &ch, &maxVal, &type);
    GRAY_IMG_CHK(ch);

    mat *histStch = allocMatMem(row, col, ch);
    stretchHist(histStch, imgMat, row, col);

    char histStchOpFilNam[MAX_FIL_NAM_SIZ];
    mkOpName(histStchOpFilNam, "HistStch", FILTER_SIZ, ipFilNam, sizeof (histStchOpFilNam));
    imgMatToPgm(histStchOpFilNam, histStch, row, col, ch, maxVal, type);
    free(imgMat);
    free(histStch);
}

void doHistEq(const char* ipFilNam){
    mat *imgMat;
    size_t row, col, ch, maxVal;
    PGMType type;

    pgmToImgMat(ipFilNam, &imgMat, &row, &col, &ch, &maxVal, &type);
    GRAY_IMG_CHK(ch);

    mat *histEq = allocMatMem(row, col, ch);
    eqHist(histEq, imgMat, row, col);

    char histEqOpFilNam[MAX_FIL_NAM_SIZ];
    mkOpName(histEqOpFilNam, "HistEq", FILTER_SIZ, ipFilNam, sizeof (histEqOpFilNam));
    imgMatToPgm(histEqOpFilNam, histEq, row, col, ch, maxVal, type);
    free(imgMat);
    free(histEq);
}

void doKMeans(const char* ipFilNam, size_t K){
    mat *imgMat;
    size_t row, col, ch, maxVal;
    PGMType type;

    pgmToImgMat(ipFilNam, &imgMat, &row, &col, &ch, &maxVal, &type);

    mat *kMeans = allocMatMem(row, col, ch);
    applyKMeans(kMeans, imgMat, row, col, ch, K);

    char kMeansOpFilNam[MAX_FIL_NAM_SIZ];
    mkOpName(kMeansOpFilNam, "KMeans", (char)K, ipFilNam, sizeof (kMeansOpFilNam));
    imgMatToPgm(kMeansOpFilNam, kMeans, row, col, ch, maxVal, type);
    free(imgMat);
    free(kMeans);
}

void doSpatialKMeans(const char* ipFilNam, size_t K){
    mat *imgMat;
    size_t row, col, ch, maxVal;
    PGMType type;

    pgmToImgMat(ipFilNam, &imgMat, &row, &col, &ch, &maxVal, &type);

    mat *sKMeans = allocMatMem(row, col, ch);
    applySpatialKMeans(sKMeans, imgMat, row, col, ch, K);

    char sKMeansOpFilNam[MAX_FIL_NAM_SIZ];
    mkOpName(sKMeansOpFilNam, "SKMeans", (char)K, ipFilNam, sizeof (sKMeansOpFilNam));
    imgMatToPgm(sKMeansOpFilNam, sKMeans, row, col, ch, maxVal, type);
    free(imgMat);
    free(sKMeans);
}

void doAll(const char *ipFilNam){
    doBinConv(ipFilNam, 2);
    doMedianFilter(ipFilNam, 1);
    doHistStch(ipFilNam);
    doHistEq(ipFilNam);
    doScharrEdgeDet(ipFilNam, 0);
    doCannyEdgeDet(ipFilNam);
    doKMeans(ipFilNam, 10);
    doSpatialKMeans(ipFilNam, 10);
}
