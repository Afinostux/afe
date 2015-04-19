#include "af_iqe.hpp"
#include <stdio.h>
#include "include/SDL_rwops.h"

static const char *afIqeHeader = "# Inter-Quake Export";

enum af_iqe_vert_attributes {
	IQEVA_POSITION = 0,
	IQEVA_TEXCOORD,
	IQEVA_NORMAL,
	IQEVA_BLENDINDEX,
	IQEVA_COLOR,
	IQEVA_SIZE
};

struct afIQEBlendIndex {
	uint index;
	float weight;
};

//iqm text format
#define IQELINESIZE 512
bool afLoadIQE(const char* fname){
	SDL_RWops * file = SDL_RWFromFile(fname,"r");
	if (!file) {
		fprintf(stderr, "error loading %s\n%s\n", fname, SDL_GetError());
		return false;
	}

	float vpos[4], texcoord[2], normal[3], color[4], custom[4];
	afIQEBlendIndex index;

	char line[IQELINESIZE] = {};
	char cchar;
	uint 
		linepos = 0,
		linecounter = 0;
	bool validheader = false;
	bool emptyline;
	
	while (SDL_RWread(file, &line[linepos++], 1, 1)) {
		cchar = line[linepos-1];

		if (cchar == '\n') {
			line[linepos] = 0;
			linepos = 0;
			emptyline = false;

			if (validheader) {
				if (line[0] != '#') {
					//trim leading space
					while (isspace(line[linepos])){
						if (line[linepos] == '\n'){
							emptyline = true;
							break;
						}
						linepos++;
					}
					if (!emptyline) {
					}
				}
			} else {
				validheader = true;
				while (afIqeHeader[linepos]) {
					if (afIqeHeader[linepos] != line[linepos]) {
						fprintf(stderr,
						"Bad header! line %u of %s\n(%s) !=\n(%s)\n",
						linecounter, fname, line, afIqeHeader);
						validheader = false;
						break;
					}
					linepos++;
				}
				if (!validheader) {
					break;
				}
			}

			linecounter++;
			linepos = 0;
		}

		if (linepos == IQELINESIZE) {
			fprintf(stderr,
			"line overflow! line %u of %s\n",
			linecounter, fname);
			break;
		}
	}

	SDL_RWclose(file);
	return false;
}
#undef IQELINESIZE

bool afLoadIQM(const char* fname){
	return false;
}

