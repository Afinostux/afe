#include "af_iqe.hpp"
#include <stdlib.h>
#include <stdio.h>
#include "include/SDL_rwops.h"

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

static const float
afIqeDefPositions[4] = {0, 0, 0, 1},
afIqeDefTcoords[2] = {0, 0};

bool afIQEPartMatch(const char* line, const char* comp){
	uint linepos = 0;
	while (comp[linepos]){
		if (line[linepos] != comp[linepos]) return false;
		linepos++;
	}
	return true;
}

void afIQEFillf(const char* line,
		uint linepos,
		const float * defaults,
		float *dest,
		const uint count) {
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

//iqm text format
#define IQELINESIZE 512
bool afLoadIQE(const char* fname){
	SDL_RWops * file = SDL_RWFromFile(fname,"r");
	SDL_RWops * output = SDL_RWFromFile("IQE_OUT.txt","w");
	if (!file) {
		fprintf(stderr, "error loading %s\n%s\n", fname, SDL_GetError());
		return false;
	}

	float vpos[4], texcoord[2], normal[3], color[4], custom[4];
	afIQEBlendIndex index[4] = {};

	int triindex[3];

	char line[IQELINESIZE] = {};
	char outline[IQELINESIZE] = {};
	char cchar;
	uint 
		linepos = 0,
		linecounter = 0;
	bool validheader = false;
	bool error = false;
	bool emptyline;
	
	while (SDL_RWread(file, &line[linepos++], 1, 1) && !error) {
		if (linepos > IQELINESIZE) {
			fprintf(stderr,
			"line overflow! line %u of %s\n",
			linecounter, fname);
			break;
		}
		cchar = line[linepos-1];

		if (cchar == '\n') {
			line[linepos] = 0;
			snprintf(outline, IQELINESIZE, "(%u) %s", linecounter,
					line);
			SDL_RWwrite(output, outline, strlen(outline), 1);
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
						switch (line[linepos]) 
						{
							case 'v': /*vertex*/
							{
								switch(line[linepos+1])
								{
									case 'p': /*vp = position*/
									{
										afIQEFillf(line,
											linepos + 2,
											afIqeDefPositions,
											vpos, 4);

										snprintf(outline, IQELINESIZE,
										"#	vertex position [%.2f %.2f %.2f %.2f]\n",
										vpos[0], vpos[1], vpos[2], vpos[3]);
										SDL_RWwrite(output, outline, strlen(outline), 1);
									} break;
									case 't': /*vt = texcoord*/
									{
										afIQEFillf(line,
											linepos + 2,
											afIqeDefTcoords,
											texcoord, 2);
										snprintf(outline, IQELINESIZE,
										"#	texcoord [%.2f %.2f]\n",
										texcoord[0], texcoord[1]);
										SDL_RWwrite(output, outline, strlen(outline), 1);
									} break;
									case 'n': /*vn = normal*/
									{
										afIQEFillf(line,
											linepos + 2,
											0,
											normal, 3);
										snprintf(outline, IQELINESIZE,
										"#	normal [%.2f %.2f %.2f]\n",
										normal[0], normal[1], normal[2]);
										SDL_RWwrite(output, outline, strlen(outline), 1);
									} break;
									case 'c': /*vc = color*/
									{
										afIQEFillf(line,
											linepos + 2,
											afIqeDefPositions,
											color, 4);
										snprintf(outline, IQELINESIZE,
										"#	color [%.2f %.2f %.2f %.2f]\n",
										color[0], color[1], color[2], color[3]);
										SDL_RWwrite(output, outline, strlen(outline), 1);
									} break;
									default:
									{
										if (isdigit(line[linepos+1])){
											afIQEFillf(line,
												linepos + 2,
												afIqeDefCustom,
												custom, 4);
											int customi = atoi(&line[linepos+1]);
											snprintf(outline, IQELINESIZE,
											"#	custom%u [%.2f %.2f %.2f %.2f]\n",
											customi, custom[0], custom[1], custom[2], custom[3]);
											SDL_RWwrite(output, outline, strlen(outline), 1);
										} else {
											//error = true;
											fprintf(stderr,
											"Bad command! line %u of %s\n(%s)\n",
											linecounter, fname, line);
										}
									}
								}
							} break;
							case 'f':
							{
								switch (line[linepos+1])
								{
									case 'a':
									{
										afIQEFillf(
											line,
											linepos + 2,

									} break;
									case 'm':
									{
									} break;
								}
							}
							default:
							{
								if (afIQEPartMatch(&line[linepos], "mesh")){
								} else if (afIQEPartMatch(&line[linepos], "material")) {
								} else {
									//error = true;
									fprintf(stderr,
									"Unexpected character! line %u of %s\n(%s) << %c\n",
									linecounter, fname, line, line[linepos]);
								}
							} break;
						}
					}
				}
			} else {
				validheader = afIQEPartMatch(line, "# Inter-Quake Export");
				if (!validheader) {
					fprintf(stderr,
					"Bad header! line %u of %s\n",
					linecounter, fname);
					validheader = false;
					break;
				}
			}

			linecounter++;
			linepos = 0;
		}

	}

	SDL_RWclose(output);
	SDL_RWclose(file);
	return false;
}
#undef IQELINESIZE

bool afLoadIQM(const char* fname){
	return false;
}

