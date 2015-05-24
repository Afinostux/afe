
#pragma once
#include "af3dmath.hpp"
#include "afcstrings.hpp"

typedef struct afPose {
	afVec4 position;
	afQuat rotation;

	inline
	afVec4 transformVertex(const afVec4& other)
	{
		afVec4 result = rotation.transformVertex(other);
		if (other.w != 0) result = result + position;
		return result;
	}

	inline
	afPose operator*(const float other)
	{
		afPose result;
		result.position = position*other;
		result.rotation = rotation*other;
		return result;
	}

	inline
	afPose& operator+=(const afPose& other)
	{
		position += other.position;
		rotation += other.rotation;
		return *this;
	}

	inline
	afPose& operator*=(const float other)
	{
		position *= other;
		rotation *= other;
		return *this;
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

typedef struct afVertex {
	afVec4 position;
	afVec4 color;
} afVertex;

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
	finish() {
		if (finished) return;
		for (int i = 0; i < vertexCount; i++) {
			vertices[i].position;
		}
	}
} afModel;

afModel* afGetModel(const char* modelname);

afModel* afLoadIQE(const char* fname);
bool afLoadIQM(const char* fname);
