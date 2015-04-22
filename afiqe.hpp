
#pragma once
#include "af3dmath.hpp"
#include "afcstrings.hpp"

typedef struct afPose {
	afVec4 position;
	afVec4 rotation;
} afPose;

#define AFBONE_NAMESIZE 16
typedef struct afBone {
	afPose * poses;
	uint poseCount;
	int parentID;
	char name[AFBONE_NAMESIZE];
	afBone() : poses(0), poseCount(0), parentID(0), name{} {}
} afBone;

typedef struct afVertexWeight {
	float boneWeight;
	int boneIndex;
	afVertexWeight() : boneWeight(0), boneIndex(0) {}
} afVertexWeight;

typedef struct afVertex {
	afVec4 position;
	afVec4 color;
	afVertexWeight weights[4];
	uint weightCount;
	afVertex() : weights{}, weightCount(0){};
} afVertex;

#define AFMESH_NAMESIZE 16
typedef struct afMesh {
	char name[AFMESH_NAMESIZE];
	int * indices;
	int indexCount;
	afMesh(): name{}, indices(0), indexCount(0) {}
} afMesh;

#define AFMODEL_NAMESIZE 32
typedef struct afModel {
	afVertex* vertices;
	afMesh* meshes;

	afBone* bones;
	char name[AFMODEL_NAMESIZE];

	uint 
	vertexCount,
	meshCount;

	afModel() : vertices(0), meshes(0), bones(0), name{}, vertexCount(0), meshCount(0) {}
} afModel;

afModel* afGetModel(const char* modelname);

afModel* afLoadIQE(const char* fname);
bool afLoadIQM(const char* fname);
