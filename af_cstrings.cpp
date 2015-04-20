
#include "af_cstrings.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

bool afCstPartMatch(const char* line,
	const char* comp){

	uint linepos = 0;
	while (comp[linepos]){
		if (line[linepos] != comp[linepos]) return false;
		linepos++;
	}
	return true;
}

uint afCstCountCst(const char* line,
	const char* token){

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

	return count;
}

void afFillCst(const char* line,
	uint linepos,
	char * dest,
	const uint max){

	while (line[++linepos]!='\"');
	linepos++;
	for (uint i = 0; i < max; i++){
		dest[i] = line[linepos];
		if (!dest[i]) return;
		if (dest[i] == '\"'){
			dest[i] = 0;
			return;
		}
		linepos++;
	}
}


void afFillBufi(const char* line,
	uint linepos,
	const int * defaults,
	int *dest,
	const uint count){

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
}

void afFillBuff(const char* line,
	uint linepos,
	const float * defaults,
	float *dest,
	const uint count){

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
}
