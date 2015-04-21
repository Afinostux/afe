#include "afiqe.hpp"
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include "include/SDL_rwops.h"
#include "include/SDL_timer.h"

enum afiqe_vert_attributes {
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
	afModel * result;
	if (afModelCount == AF_MAXMODELS) return 0;
	result = &afModelPool[afModelCount];
	uint mnamepos = 0, i;

	while (modelname[mnamepos] && modelname[mnamepos] != '.') mnamepos++;
	result->name = (char*)malloc(mnamepos+1);

	for (i = 0; i < mnamepos; i++){
		result->name[i] = modelname[i];
	}
	result->name[mnamepos] = 0;
	afModelCount++;
	return result;
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
	if (!file) {
		fprintf(stderr, "error loading %s\n%s\n", fname, SDL_GetError());
		return 0;
	}


	Uint64 filesize = SDL_RWsize(file);

	char * filecont = (char*)malloc(filesize+1);
	SDL_RWread(file, filecont, filesize, 1);
	SDL_RWclose(file);

	float vpos[4]={}, texcoord[2]={}, normal[3]={}, color[4]={}, custom[4]={};

	uint 
	vposindex = 0,
	texcoordindex = 0,
	normalindex = 0,
	colorindex = 0,
	customindex = 0;

	char objectname[32] = {};
	uint objectnamesize = 0;

	uint triindexg = 0;

	char line[IQELINESIZE] = {};
	char cchar;
	uint 
		linepos = 0,
		linecounter = 1,
		filepos = 0;
	bool validheader = false;
	bool error = false;
	bool emptyline;

	afModel * newmodel = afAddModel(fname);
	newmodel->afVertexCount = afCstCountCst(filecont, "vp");
	newmodel->afMeshCount = afCstCountCst(filecont, "mesh");

	newmodel->meshes = (afMesh*)malloc(sizeof(afMesh) * newmodel->afMeshCount);
	afMesh* currentmesh = 0;
	uint meshindex = 0;
	uint meshtricount = 0;
	newmodel->vertices = (afVertex*)malloc(sizeof(afVertex) * newmodel->afVertexCount);
	
	while (filepos < filesize && !error) {
		if (linepos > IQELINESIZE) {
			fprintf(stderr,
			"line overflow! line %u of %s\n",
			linecounter, fname);
			break;
		}

		line[linepos++] = filecont[filepos++];

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
						switch (line[linepos]) 
						{
							case 'v': /*vertex*/
							{
								switch(line[linepos+1])
								{
									case 'p': /*vp = position*/
									{
										afVec4 *cpos = &newmodel->vertices[vposindex++].position;
										afFillBuff(line,
											linepos + 2,
											afIqeDefPositions,
											vpos, 4);
										cpos->x = vpos[0];
										cpos->y = vpos[1];
										cpos->z = vpos[2];
										cpos->w = vpos[3];
									} break;
									case 't': /*vt = texcoord*/
									{
										afFillBuff(line,
											linepos + 2,
											afIqeDefTcoords,
											texcoord, 2);
									} break;
									case 'n': /*vn = normal*/
									{
										afFillBuff(line,
											linepos + 2,
											0,
											normal, 3);
									} break;
									case 'c': /*vc = color*/
									{
										afVec4 *ccol = &newmodel->vertices[colorindex++].color;
										afFillBuff(line,
											linepos + 2,
											afIqeDefPositions,
											color, 4);
										ccol->x = color[0];
										ccol->y = color[1];
										ccol->z = color[2];
										ccol->w = color[3];
									} break;
									default:
									{
										if (isdigit(line[linepos+1])){
											afFillBuff(line,
												linepos + 2,
												0,
												custom, 4);
											int customi = atoi(&line[linepos+1]);
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
#if 0
									case 'a':
									{
										afFillBufi(
											line,
											linepos + 2,
											0, 
											triindex, 3);
									} break;
#endif
									case 'm':
									{
										if (!currentmesh){
											fprintf(stderr,
											"Mesh relative index with no mesh! line %u of %s\n(%s)\n",
											linecounter, fname, line);
										} else {
											afFillBufi(
												line,
												linepos + 2,
												0, 
												&currentmesh->indices[triindexg], 3);
											triindexg += 3;
										}
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
									objectnamesize = afFillCst(line,
										linepos,
										objectname,
										32);
									currentmesh = &newmodel->meshes[meshindex++];
									currentmesh->indexCount = afCstCountCstUntil(&filecont[filepos+linepos+4], "fm", "mesh") * 3;
									currentmesh->indices = (int*)malloc(sizeof(int) * (currentmesh->indexCount * 3));
									triindexg = 0;
								} else if (afCstPartMatch(&line[linepos], "material")) {
									afFillCst(line,
										linepos,
										objectname,
										32);
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

	perfend = SDL_GetPerformanceCounter();
	printf("%lu\n", (perfend - perfstart));
	free(filecont);
	return newmodel;
}
#undef IQELINESIZE

bool afLoadIQM(const char* fname){
	return false;
}

