#include "afiqe.hpp"
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include "afperf.hpp"
#include "SDL/SDL_rwops.h"
#include "SDL/SDL_timer.h"

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
		struct {char name[AFMESH_NAMESIZE];} mesh;
		struct {int parentID; char name[AFBONE_NAMESIZE];} joint;
		struct {float position[4]; float rotation[4];} pose;
		struct {struct {int boneID; float weight;} link[4]; int link_count;} weight;
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
			int lineTest;

			result->weight.link_count = 0;
			while ( result->weight.link_count < 4){
				linepos = afFillBufi(line, linepos, 0, &result->weight.link[result->weight.link_count].boneID, 1);
				linepos = afFillBuff(line, linepos, 0, &result->weight.link[result->weight.link_count].weight, 1);
				result->weight.link_count++;

				lineTest = linepos;

				while (line[lineTest] && isspace(line[lineTest])) lineTest++;
				if (line[lineTest] == 0) break;
			}
		} else if (afCstPartMatch(line, "fm ")) {
			while (!isspace(line[++linepos]));
			result = new iqeNode;
			result->type = IQE_MESHTRIANGLE;
			afFillBufi( line,
					linepos,
					0,
					result->triangle.indices, 3);

		} else if (afCstPartMatch(line, "joint ")) {
			result = new iqeNode;
			result->type = IQE_JOINT;
			linepos = afGetWord(line, result->joint.name, linepos, AFBONE_NAMESIZE);
			afFillBufi(line, linepos, 0, &result->joint.parentID, 1);

		} else if (afCstPartMatch(line, "mesh ")) {
			result = new iqeNode;
			result->type = IQE_MESH;
			afGetWord(line, result->mesh.name, linepos, AFMESH_NAMESIZE);

		} else if (afCstPartMatch(line, "pq ")) {
			while (!isspace(line[++linepos]));
			result = new iqeNode;
			result->type = IQE_POSE;
			//NOTE(afox): iqe assumes poses are vec3
			// this is in spite of the fact that positions
			// are always vec4... cool.
			result->pose.position[3] = 1;
			linepos = afFillBuff(line, linepos, 0, result->pose.position, 3);
			afFillBuff(line, linepos, 0, result->pose.rotation, 4);
		}

		if (result) {
			result->previous = this;
			this->next = result;
			result->root = this->root;
		}
		return result;
	}

	void print()
	{
		iqeNode *cur = this;
		while (cur) {
			printf("(%s)\t", IQEnames[cur->type]);
			switch (cur->type) {
				case (IQE_JOINT):
					printf("%s, %i\n", cur->joint.name, cur->joint.parentID);
					break;
				case (IQE_MESH):
					printf("%s\n", cur->mesh.name);
					break;
				case (IQE_MATERIAL):
					printf("its a material\n");
					break;
				case (IQE_VERTEXPOS):
					printf("%f, %f, %f, %f\n", cur->vertexPos[0],cur->vertexPos[1],cur->vertexPos[2],cur->vertexPos[3]);
					break;
				case (IQE_TEXCOORD):
					printf("%f, %f\n", cur->texCoord[0],cur->texCoord[1]);
					break;
				case (IQE_BONEWEIGHT):
					printf("[%i] ", cur->weight.link_count);
					for (int i = 0; i < cur->weight.link_count; i++){
						printf("(%i:%f) ", cur->weight.link[i].boneID, cur->weight.link[i].weight);
					}
					printf("\n");
					break;
				case (IQE_NORMAL):
					printf("%f, %f, %f\n", cur->normal[0],cur->normal[1],cur->normal[2]);
					break;
				case (IQE_COLOR):
					printf("%f, %f, %f, %f\n", cur->color[0],cur->color[1],cur->color[2],cur->color[3]);
					break;
				case (IQE_CUSTOM):
					printf("its a custom\n");
					break;
				case (IQE_MESHTRIANGLE):
					printf("its a triangle\n");
					break;
				case (IQE_POSE):
					printf("p: %f, %f, %f, q: %f, %f, %f, %f\n", cur->pose.position[0], cur->pose.position[1], cur->pose.position[2], cur->pose.rotation[0], cur->pose.rotation[1], cur->pose.rotation[2], cur->pose.rotation[3]);
					break;
				case (IQE_HEADER):
					printf("its a header\n");
					break;
				case (IQE_TYPE_COUNT):
					printf("its a problem\n");
					break;
			}
			cur = cur->next;
		}
	}
} iqeNode;

//iqm text format
#define IQELINESIZE 512
afModel* afLoadIQE(const char* fname){
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

	iqeNode *curnode;
	iqeNode *curnode2;

	newmodel->vertexCount = iqeroot.countAstopB(IQE_VERTEXPOS, 0);
	newmodel->vertices = new afVertex[newmodel->vertexCount];
	newmodel->meshCount = iqeroot.countAstopB(IQE_MESH, 0);
	newmodel->meshes = new afMesh[newmodel->meshCount];
	newmodel->boneCount = iqeroot.countAstopB(IQE_JOINT, 0);
	newmodel->bones = new afBone[newmodel->boneCount];

	newmodel->weightCount = 0;
	for (curnode = iqeroot.findNext(IQE_BONEWEIGHT); curnode;
			curnode = curnode->findNext(IQE_BONEWEIGHT)){
		newmodel->weightCount += curnode->weight.link_count;
	}
	newmodel->weights = new afVertexWeight[newmodel->weightCount];

	uint vindex = 0, windex = 0;
	for (curnode = iqeroot.findNext(IQE_VERTEXPOS); curnode;
			curnode = curnode->findNext(IQE_VERTEXPOS)){
		newmodel->vertices[vindex].position.x = curnode->vertexPos[0];
		newmodel->vertices[vindex].position.y = curnode->vertexPos[1];
		newmodel->vertices[vindex].position.z = curnode->vertexPos[2];
		newmodel->vertices[vindex].position.w = curnode->vertexPos[3];
		for (curnode2 = curnode->findNextStop(IQE_BONEWEIGHT, IQE_VERTEXPOS); curnode2;
				curnode2 = curnode2->findNextStop(IQE_BONEWEIGHT, IQE_VERTEXPOS)){
			int n = curnode2->weight.link_count;
			for (int i = 0; i < n; i++) {
				newmodel->weights[windex].vertOffset = vindex;
				newmodel->weights[windex].boneIndex = curnode2->weight.link[i].boneID;
				newmodel->weights[windex].boneWeight = curnode2->weight.link[i].weight;
				windex++;
			}
		}
		vindex++;
	}

	uint bindex = 0, pindex;
	for (curnode = iqeroot.findNext(IQE_JOINT);
			curnode; curnode = curnode->findNext(IQE_JOINT)) {
		newmodel->bones[bindex].poseCount = curnode->countAstopB(IQE_POSE, IQE_JOINT);
		newmodel->bones[bindex].poses = new afPose[newmodel->bones[bindex].poseCount];
		pindex = 0;
		for (curnode2 = curnode->findNextStop(IQE_POSE, IQE_JOINT); curnode2;
				curnode2 = curnode2->findNextStop(IQE_POSE, IQE_JOINT)){
			newmodel->bones[bindex].poses[pindex].position
				= V4(curnode2->pose.position);
			newmodel->bones[bindex].poses[pindex].rotation
				= QUAT(curnode2->pose.rotation);
			pindex++;
		}
		bindex++;
	}

	afMesh *curmesh;
	uint meshindex = 0;
	uint triindex;
	for (curnode = iqeroot.findNext(IQE_MESH); curnode;
			curnode = curnode->findNext(IQE_MESH)) {
		triindex = 0;
		curmesh = &newmodel->meshes[meshindex];
		curmesh->indexCount = curnode->countAstopB(IQE_MESHTRIANGLE, IQE_MESH) * 3;
		curmesh->indices = new int[curmesh->indexCount];
		
		for (curnode2 = curnode->findNextStop(IQE_MESHTRIANGLE, IQE_MESH); curnode2;
				curnode2 = curnode2->findNextStop(IQE_MESHTRIANGLE, IQE_MESH)){
			curmesh->indices[triindex++] = curnode2->triangle.indices[0];
			curmesh->indices[triindex++] = curnode2->triangle.indices[1];
			curmesh->indices[triindex++] = curnode2->triangle.indices[2];
		}
		
		meshindex++;
	}

	
	//iqeroot.print();

	free(filecont);
	af_perf_endframe();
	return newmodel;
}
#undef IQELINESIZE

bool afLoadIQM(const char* fname){
	return false;
}

