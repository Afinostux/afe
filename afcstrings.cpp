
#include "afcstrings.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "afperf.hpp"



af_perf_new(partMatch, "afCstPartMatch"); // g0
bool afCstPartMatch(const char* line,
	const char* comp)
{
	af_perf_start(partMatch);

	uint linepos = 0;
	while (comp[linepos]){
		if (line[linepos] != comp[linepos]) return false;
		linepos++;
	}

	af_perf_end(partMatch);
	return true;
}

af_perf_new(cstCountCstUntil, "afCstCountCstUntil");// g1
uint afCstCountCstUntil(const char* line,
	const char* token,
	const char* breaktoken)
{
	af_perf_start(cstCountCstUntil);

	uint 
	count = 0,
	tokenpos = 0,
	breaktokenpos = 0;

	while (*line){
		if ((*line) == token[tokenpos]) {
			tokenpos++;
			if (!token[tokenpos]){
				tokenpos = 0;
				count++;
			}
		}
		if ((*line) == breaktoken[breaktokenpos]) {
			breaktokenpos++;
			if (!breaktoken[breaktokenpos]){
				break;
			}
		}
		line++;
	}

	af_perf_end(cstCountCstUntil);
	return count;
}

af_perf_new(cstCountCst, "afCstCountCst");// g2
uint afCstCountCst(const char* line,
	const char* token)
{
	af_perf_start(cstCountCst);

	uint 
	count = 0,
	tokenpos = 0;

	while (*line){
		if ((*line) == token[tokenpos]) {
			tokenpos++;
			if (!token[tokenpos]){
				tokenpos = 0;
				count++;
			}
		}
		line++;
	}
	af_perf_end(cstCountCst);
	return count;
}

af_perf_new(fillCst, "afFillCst");// g3
int afFillCst(const char* line,
	uint linepos,
	char * dest,
	const uint max)
{
	af_perf_start(fillCst);

	int sizeofnewline = 0;

	while (line[++linepos]!='\"');
	linepos++;
	for (uint i = 0; i < max; i++){
		dest[i] = line[linepos];
		sizeofnewline++;
		if (!dest[i]) return sizeofnewline;
		if (dest[i] == '\"'){
			dest[i] = 0;
			return sizeofnewline;
		}
		linepos++;
	}
	af_perf_end(fillCst);
	return sizeofnewline;
}


af_perf_new(fillBufi, "afFillBufi");// g4
uint afFillBufi(const char* line,
	uint linepos,
	const int * defaults,
	int *dest,
	const uint count)
{
	af_perf_start(fillBufi);

	if (defaults) {
		for (uint vpi = 0; vpi < count; vpi++){
			while (isspace(line[++linepos]));
			if (line[linepos])
				dest[vpi] = atoi(&line[linepos]);
			else
				dest[vpi] = defaults[vpi];
			while (!isspace(line[++linepos]));
		}
	} else {
		for (uint vpi = 0; vpi < count; vpi++){
			while (isspace(line[++linepos]));
			if (line[linepos])
				dest[vpi] = atoi(&line[linepos]);
			else
				dest[vpi] = 0;
			while (!isspace(line[++linepos]));
		}
	}
	af_perf_end(fillBufi);
	return linepos;
}

af_perf_new(fillBuff, "afFillBuff");// g5
uint afFillBuff(const char* line,
	uint linepos,
	const float * defaults,
	float *dest,
	const uint count)
{
	af_perf_start(fillBuff);

	if (defaults) {
		for (uint vpi = 0; vpi < count; vpi++){
			while (isspace(line[++linepos]));
			if (line[linepos])
				dest[vpi] = atof(&line[linepos]);
			else
				dest[vpi] = defaults[vpi];
			while (!isspace(line[++linepos]));
		}
	} else {
		for (uint vpi = 0; vpi < count; vpi++){
			while (isspace(line[++linepos]));
			if (line[linepos])
				dest[vpi] = atof(&line[linepos]);
			else
				dest[vpi] = 0;
			while (!isspace(line[++linepos]));
		}
	}
	af_perf_end(fillBuff);
	return linepos;
}

af_perf_new(getWord, "afGetWord");// g6
uint afGetWord(const char* line, char* dest, uint linepos, uint n)
{
	af_perf_start(getWord);
	while (line[++linepos] && line[linepos] != '"');
	uint i;
	for (i = 0; (i < n - 1); i++){
		if (!line[++linepos]) {
			dest[i+1] = 0;
			return 0;
		}
		if (line[linepos] == '"') break;
		dest[i] = line[linepos];
	}
	dest[i] = 0;

	//NOTE(afox): make sure to step out of the quote mark!
	af_perf_end(getWord);
	return linepos + 1;
}

