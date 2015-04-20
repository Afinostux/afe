#include "af_iqe.hpp"
#include <stdlib.h>
#include <stdio.h>
#include "include/SDL_rwops.h"
#include "include/SDL_timer.h"

enum af_iqe_vert_attributes {
	IQEVA_POSITION = 0,
	IQEVA_TEXCOORD,
	IQEVA_NORMAL,
	IQEVA_BLENDINDEX,
	IQEVA_COLOR,
	IQEVA_SIZE
};

static const float
afIqeDefPositions[4] = {0, 0, 0, 1},
afIqeDefTcoords[2] = {0, 0};

#define AF_MAXMODELS 128
afModel afModelPool[AF_MAXMODELS] = {};
uint afModelCount = 0;

afModel* afAddModel(const char* modelname){
	if (afModelCount == AF_MAXMODELS) return 0;
	afModelPool[afModelCount].name = modelname;
	return &afModelPool[afModelCount++];
}

afModel* afGetModel(const char* modelname){
	afModel* mpp = afModelPool;
	for (uint i = 0; i < afModelCount; i++){
		if (!strcmp(modelname, mpp->name)){
			return mpp;
		}
		mpp++;
	}
	return 0;
}


//iqm text format
#define IQELINESIZE 512
afModel* afLoadIQE(const char* fname){
	Uint64 perfstart = SDL_GetPerformanceCounter(), perfend;
	SDL_RWops * file = SDL_RWFromFile(fname,"r");
	SDL_RWops * output = SDL_RWFromFile("IQE_OUT.txt","w");
	if (!file) {
		fprintf(stderr, "error loading %s\n%s\n", fname, SDL_GetError());
		return 0;
	}

	Uint64 filesize = SDL_RWsize(file);

	char * filecont = (char*)malloc(filesize);
	SDL_RWread(file, filecont, filesize, 1);
	SDL_RWclose(file);
	file = SDL_RWFromMem(filecont, filesize);

	float vpos[4], texcoord[2], normal[3], color[4], custom[4];
	char objectname[32] = {};

	int triindex[3];

	char line[IQELINESIZE] = {};
	char outline[IQELINESIZE] = {};
	char cchar;
	uint 
		linepos = 0,
		linecounter = 1;
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
			snprintf(outline, IQELINESIZE, "%s", 
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
										afFillBuff(line,
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
										afFillBuff(line,
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
										afFillBuff(line,
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
										afFillBuff(line,
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
											afFillBuff(line,
												linepos + 2,
												0,
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
										afFillBufi(
											line,
											linepos + 2,
											0, 
											triindex, 3);
										snprintf(outline, IQELINESIZE,
										"#\tabsolute triangle [%i %i %i]\n",
										triindex[0], triindex[1], triindex[2]);
										SDL_RWwrite(output, outline, strlen(outline), 1);
									} break;
									case 'm':
									{
										afFillBufi(
											line,
											linepos + 2,
											0, 
											triindex, 3);
										snprintf(outline, IQELINESIZE,
										"#\tmesh relative triangle [%i %i %i]\n",
										triindex[0], triindex[1], triindex[2]);
										SDL_RWwrite(output, outline, strlen(outline), 1);
									} break;
									default:
									{
										//error = true;
										fprintf(stderr,
										"Bad command! line %u of %s\n(%s)\n",
										linecounter, fname, line);
									}
								}
							} break;
							default:
							{
								if (afCstPartMatch(&line[linepos], "mesh")){
									afFillCst(line,
										linepos,
										objectname,
										32);
									snprintf(outline, IQELINESIZE,
									"#\tnew mesh [%s]\n",
									objectname);
									SDL_RWwrite(output, outline, strlen(outline), 1);
								} else if (afCstPartMatch(&line[linepos], "material")) {
									afFillCst(line,
										linepos,
										objectname,
										32);
									snprintf(outline, IQELINESIZE,
									"#\tnew material [%s]\n",
									objectname);
									SDL_RWwrite(output, outline, strlen(outline), 1);
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
				validheader = afCstPartMatch(line, "# Inter-Quake Export");
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
	perfend = SDL_GetPerformanceCounter();
	printf("%lu\n", (perfend - perfstart));
	free(filecont);
	return 0;
}
#undef IQELINESIZE

bool afLoadIQM(const char* fname){
	return false;
}

