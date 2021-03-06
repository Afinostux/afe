
#pragma once
#include "afmath.hpp"
bool afCstPartMatch(const char* line, const char* comp);
uint afCstCountCstUntil(const char* line, const char* token, const char* breaktoken);
uint afCstCountCst(const char* line, const char* token);
int afFillCst(const char* line, uint linepos, char * dest, const uint max);
uint afFillBufi(const char* line, uint linepos, const int * defaults, int *dest, const uint count);
uint afFillBuff(const char* line, uint linepos, const float * defaults, float *dest, const uint count);
uint afGetWord(const char* line, char* dest, uint linepos, uint n);
