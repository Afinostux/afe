
#pragma once
#include "afmath.hpp"
bool afCstPartMatch(const char* line, const char* comp);
uint afCstCountCstUntil(const char* line, const char* token, const char* breaktoken);
uint afCstCountCst(const char* line, const char* token);
int afFillCst(const char* line, uint linepos, char * dest, const uint max);
void afFillBufi(const char* line, uint linepos, const int * defaults, int *dest, const uint count);
void afFillBuff(const char* line, uint linepos, const float * defaults, float *dest, const uint count);
