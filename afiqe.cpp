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
afIqeDefPositions[4] = {0, 0, 0, 1};

#define AF_MAXMODELS 128
afModel afModelPool[AF_MAXMODELS] = {};
uint afModelCount = 0;

afModel* afAddModel(const char* modelname){
	afModel * result;
	if (afModelCount == AF_MAXMODELS) return 0;
	result = &afModelPool[afModelCount];
	uint mnamepos = 0, i;

	while (
			(mnamepos < AFMODEL_NAMESIZE - 1) && 
			(modelname[mnamepos]) && 
			(modelname[mnamepos] != '.')) mnamepos++;

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

//
//	iqe parsing stuff
//
enum {
	IQE_NULL = 0,
	IQE_ROOT,
	IQE_JOINT,
	IQE_MESH,
	IQE_MATERIAL,
	IQE_VERTEXPOS,
	IQE_TEXCOORD,
	IQE_BONEWEIGHT,
	IQE_NORMAL,
	IQE_COLOR,
	IQE_CUSTOM,
	IQE_MESHTRIANGLE,
	IQE_POSE,
	IQE_HEADER,
	IQE_TYPE_COUNT
};
static const char * IQEnames[IQE_TYPE_COUNT] = {
		"IQE_NULL",
		"IQE_ROOT",
		"IQE_JOINT",
		"IQE_MESH",
		"IQE_MATERIAL",
		"IQE_VERTEXPOS",
		"IQE_TEXCOORD",
		"IQE_BONEWEIGHT",
		"IQE_NORMAL",
		"IQE_COLOR",
		"IQE_CUSTOM",
		"IQE_MESHTRIANGLE",
		"IQE_POSE",
		"IQE_HEADER"
};
typedef struct iqeNode {
	iqeNode* next;
	iqeNode* previous;
	iqeNode* root;
	uint type;
	union {
		/*name points to first char of name in flatly loaded file*/
		struct {const char name[AFMESH_NAMESIZE];} mesh;
		struct {} joint;
		float vertexPos[4];
		float texCoord[2];
		float normal[3];
		float color[4];
		struct {float custom[4]; uint customid;} custom;
		struct {int indices[3];} triangle;
	};

	iqeNode() : next(0), previous(0), root(this), type(0) {
	};

	~iqeNode() {
		if (next) delete(next);
	}

	iqeNode* findNext(uint findType) {
		iqeNode *result = next;
		while (result) {
			if (result->type == findType) break;
			result = result->next;
		}
		return result;
	}

	iqeNode* findNextStop(uint findType, uint stopType) {
		iqeNode *result = next;
		while (result) {
			if (result->type == findType) break;
			if (result->type == stopType) result = 0;
			else result = result->next;
		}
		return result;
	}

	int countAstopB(uint countType, uint stopType) {
		int count = 0;
		iqeNode * cur = next;
		while (cur && cur->type != stopType) {
			if (cur->type == countType) count++;
			cur = cur->next;
		}
		return count;
	}
	//convert a line into an iqenode
	iqeNode* append(const char* line) {
		iqeNode * result = 0;
		uint linepos = 0;

		if (afCstPartMatch(line, "# Inter-Quake Export")) {
			result = new iqeNode;
			result->type = IQE_HEADER;

		} else if (afCstPartMatch(line, "vp ")) {
			while (!isspace(line[++linepos]));
			result = new iqeNode;
			result->type = IQE_VERTEXPOS;
			afFillBuff( line,
				linepos,
				afIqeDefPositions,
				result->vertexPos,
				4);

		} else if (afCstPartMatch(line, "vt ")) {
			while (!isspace(line[++linepos]));
			result = new iqeNode;
			result->type = IQE_TEXCOORD;
			afFillBuff( line,
				linepos,
				0,
				result->texCoord,
				2);

		} else if (afCstPartMatch(line, "vn ")) {
			while (!isspace(line[++linepos]));
			result = new iqeNode;
			result->type = IQE_NORMAL;
			afFillBuff( line,
				linepos,
				0,
				result->normal,
				3);

		} else if (afCstPartMatch(line, "vb ")) {
			while (!isspace(line[++linepos]));
			result = new iqeNode;
			result->type = IQE_BONEWEIGHT;

		} else if (afCstPartMatch(line, "fm ")) {
			while (!isspace(line[++linepos]));
			result = new iqeNode;
			result->type = IQE_MESHTRIANGLE;
			afFillBufi( line,
					linepos,
					0,
					result->triangle.indices, 3);

		} else if (afCstPartMatch(line, "joint ")) {
			while (!isspace(line[++linepos]));
			result = new iqeNode;
			result->type = IQE_JOINT;

		} else if (afCstPartMatch(line, "mesh ")) {
			while (!isspace(line[++linepos]));
			result = new iqeNode;
			result->type = IQE_MESH;

		} else if (afCstPartMatch(line, "pq ")) {
			while (!isspace(line[++linepos]));
			result = new iqeNode;
			result->type = IQE_POSE;

		}

		if (result) {
			result->previous = this;
			this->next = result;
			result->root = this->root;
		}
		return result;
	}
} iqeNode;

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


	iqeNode iqeroot;
	iqeNode *iqetemp;
	iqeNode *iqelast = &iqeroot;

	char line[IQELINESIZE] = {};
	char cchar;
	uint 
		linepos = 0,
		linecounter = 1,
		filepos = 0;

	afModel * newmodel = afAddModel(fname);
	
	while (filepos < filesize) {
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

			if (isspace(line[0]))
			while (line[++linepos] && isspace(line[linepos]));

			iqetemp = iqelast->append(&line[linepos]);
			if (iqetemp) iqelast = iqetemp;

			linecounter++;
			linepos = 0;
		}

	}

	newmodel->vertexCount = iqeroot.countAstopB(IQE_VERTEXPOS, 0);
	newmodel->vertices = new afVertex[newmodel->vertexCount];
	newmodel->meshCount = iqeroot.countAstopB(IQE_MESH, 0);
	newmodel->meshes = new afMesh[newmodel->meshCount];

	iqeNode *curnode = iqeroot.findNext(IQE_VERTEXPOS);
	uint vindex = 0;
	uint i;
	while (curnode){
		newmodel->vertices[vindex].position.x = curnode->vertexPos[0];
		newmodel->vertices[vindex].position.y = curnode->vertexPos[1];
		newmodel->vertices[vindex].position.z = curnode->vertexPos[2];
		newmodel->vertices[vindex].position.w = curnode->vertexPos[3];
		curnode = curnode->findNext(IQE_VERTEXPOS);
		vindex++;
	}

	iqeNode *meshnode = iqeroot.findNext(IQE_MESH);
	afMesh *curmesh;
	uint meshindex = 0;
	uint triindex;
	while (meshnode) {
		triindex = 0;
		curmesh = &newmodel->meshes[meshindex];
		curmesh->indexCount = meshnode->countAstopB(IQE_MESHTRIANGLE, IQE_MESH) * 3;
		curmesh->indices = new int[curmesh->indexCount];
		curnode = meshnode->findNextStop(IQE_MESHTRIANGLE, IQE_MESH);
		while (curnode){
			curmesh->indices[triindex++] = curnode->triangle.indices[0];
			curmesh->indices[triindex++] = curnode->triangle.indices[1];
			curmesh->indices[triindex++] = curnode->triangle.indices[2];

			curnode = curnode->findNextStop(IQE_MESHTRIANGLE, IQE_MESH);
		}
		
		meshindex++;
		meshnode = meshnode->findNext(IQE_MESH);
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

