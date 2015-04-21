
#include "afcstrings.hpp"
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

uint afCstCountCstUntil(const char* line,
	const char* token,
	const char* breaktoken){

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

	return count;
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

int afFillCst(const char* line,
	uint linepos,
	char * dest,
	const uint max){

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
	return sizeofnewline;
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
