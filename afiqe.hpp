
#pragma once
#include "af3dmath.hpp"
#include "afcstrings.hpp"

typedef struct afVertex {
	afVec4 position;
	afVec4 color;
} afVertex;

typedef struct afPose {
	afCoord c;

	inline
	afVec4 transform(const afVec4& o) const
	{
		afVec4 result;
		result = o - c.position;
		result = c.transform(result);
		return result;
	}

	inline
	void invert()
	{
		c.position = -c.position;
		c.rotation = -c.rotation;
	}
} afPose;

#define AFBONE_NAMESIZE 16
typedef struct afBone {
	int poseCount;
	afPose * poses;
	int parentID;
	char name[AFBONE_NAMESIZE];
} afBone;

typedef struct afVertexWeight {
	int vertOffset;
	float boneWeight;
	int boneIndex;
} afVertexWeight;

#define AFMESH_NAMESIZE 16
typedef struct afMesh {
	char name[AFMESH_NAMESIZE];
	int indexCount;
	int * indices;
} afMesh;

typedef struct afPoseBuffer {
	afPose * poses;
	int posecount;

	inline
	void clear() {
		delete(poses);
		posecount = 0;
		poses = 0;
	}
} afPoseBuffer;

#define AFMODEL_NAMESIZE 32
typedef struct afModel {
	afVertex* vertices;
	afMesh* meshes;
	afBone* bones;
	afVertexWeight* weights;
	char name[AFMODEL_NAMESIZE];
	bool finished;

	int 
	vertexCount,
	meshCount,
	boneCount,
	weightCount;

	inline
	afPoseBuffer
	getPoseBuffer() {
		afPoseBuffer result;
		result.posecount = boneCount;
		result.poses = new afPose[result.posecount];
		for (int i = 0; i < boneCount; i++) {
			result.poses[i] = *bones[i].poses;
		}
		return result;
	}

	inline
	void
	transformPoseBuffer(afPoseBuffer* buf) {
		for (int i = 1; i < boneCount; i++) {
			buf->poses[i].c = buf->poses[i].c + buf->poses[bones[i].parentID].c;
		}
	}

	inline
	void
	finish() {
		if (finished) return;
		finished = true;

#if 0
		if (boneCount) {
			afPoseBuffer p = getPoseBuffer();
			for (int i = 0; i < p.posecount; i++) {
				p.poses[i].invert();
			}
			transformPoseBuffer(&p);
			int WI = 0;
			for (int i = 0; i < vertexCount; i++) {
				float accum = 0;
				afVec4 mix = {};
				for (; WI < weightCount && weights[WI].vertOffset == i; WI++) {
					mix += bones[weights[WI].boneIndex].poses[0].transform(vertices[i].position) * weights[WI].boneWeight;
					accum += weights[WI].boneWeight;
				}
				vertices[i].position = mix/accum;
			}
		}
#endif
	}
} afModel;

afModel* afGetModel(const char* modelname);

