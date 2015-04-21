
#pragma once
#include "af_3dmath.hpp"
#include "af_cstrings.hpp"

typedef struct afVertex {
	afVec4 position;
	afVec4 color;
} afVertex;

typedef struct afMesh {
	char* name;
	int * indices;
	int indexCount;
} afMesh;

typedef struct afModel {
	char* name;
	afVertex* vertices;
	afMesh* meshes;

	uint 
	afVertexCount,
	afMeshCount;

} afModel;

afModel* afGetModel(const char* modelname);

afModel* afLoadIQE(const char* fname);
bool afLoadIQM(const char* fname);
