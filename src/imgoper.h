#ifndef IMGOPER_H
#define IMGOPER_H

#include <stdlib.h>

void doBinConv(const char* ipFilNam, size_t n);
void doMedianFilter(const char* ipFilNam, size_t n);
void doHistStch(const char* ipFilNam);
void doHistEq(const char* ipFilNam);
void doScharrEdgeDet(const char* ipFilNam, size_t gradTh);
void doSobelEdgeDet(const char* ipFilNam, size_t gradTh);
void doCannyEdgeDet(const char* ipFilNam);
void doKMeans(const char* ipFilNam, size_t K);
void doSpatialKMeans(const char* ipFilNam, size_t K);
void doAll(const char *ipFilNam);

#endif // IMGOPER_H
