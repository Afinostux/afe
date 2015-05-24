#pragma once

#define AF_PERF 0
#include "SDL/SDL_timer.h"

#if AF_PERF
typedef struct perfInfo {
	unsigned long rstart;
	unsigned long max;
	unsigned int hitcount;
} perfInfo;
#define AF_PERF_MAX 256
perfInfo afPerfs[AF_PERF_MAX] = {};
int afPerfCount = 0;

#define af_perf_new(perfname, perfword) static int perfname = afPerfCount++;
#define af_perf_start(perfname) afPerfs[perfname].rstart = SDL_GetPerformanceCounter();
#define af_perf_end(perfname) {unsigned long rend = SDL_GetPerformanceCounter(); if (rend - afPerfs[perfname].rstart > afPerfs[perfname].max) afPerfs[perfname].max = rend - afPerfs[perfname].rstart; afPerfs[perfname].hitcount++;};
#define af_perf_endframe() unsigned long total = 0; printf("\n~perf~\n"); for (int i = 0; i < afPerfCount; i++) {if (afPerfs[i].hitcount) printf("(%i) %lu\t%i\n", i, afPerfs[i].max, afPerfs[i].hitcount); total += afPerfs[i].max * afPerfs[i].hitcount; afPerfs[i].hitcount = 0; afPerfs[i].max = 0;} printf("%lu\n", total);

#else
#define AF_PERF_MAX 0
#define af_perf_new(perfname, perfword)
#define af_perf_start(perfname)
#define af_perf_end(perfname)
#define af_perf_endframe()
#endif

