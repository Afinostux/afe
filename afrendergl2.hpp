
#pragma once
#include "af3dmath.hpp"
#include "afgl.hpp"
#include "afrenderi.hpp"

typedef struct afRendererGL2 : public afRenderer {
	virtual
	void init(SDL_Window * window){
		context = SDL_GL_CreateContext(window);
		this->window = window;
		afLoadGL();
		glClearColor(0,0,0,1);
		modelview = {};
		projection = {};
		if (skinBuffer) delete(skinBuffer);
		skinBuffer = 0;
		skinBufferSize = 0;
		skinBufferMaxSize = 0;
	}

	virtual
	void beginFrame() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	virtual
	void endFrame() {
		SDL_GL_SwapWindow(window);
	}

	virtual
	void begin3D() {
		int w, h;
		SDL_GetWindowSize(window, &w, &h);
		glEnable(GL_DEPTH_TEST);
		this->projection.perspective(90, (float)w/h, 1, 100);
	
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glLoadMatrixf(this->projection.m);
		glMatrixMode(GL_MODELVIEW);
	}

	virtual
	void drawModel(afModel* model) {
		glColor3f(1.f, 0.f, 1.f);
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(4, GL_FLOAT, sizeof(afVertex), &(model->vertices[0].position));
		for (int i = 0; i < model->meshCount; i++){
			afMesh * cmesh = &model->meshes[i];
			glDrawElements(GL_TRIANGLES, cmesh->indexCount, GL_UNSIGNED_INT, (unsigned int*)cmesh->indices);
		}
		glDisableClientState(GL_VERTEX_ARRAY);
	}

	afVertex * skinBuffer;
	int skinBufferSize;
	int skinBufferMaxSize;
	
	inline
	void
	initSkinBuffer(int vertexCount)
	{
		if (vertexCount > skinBufferMaxSize) {
			delete(skinBuffer);
			skinBuffer = new afVertex[vertexCount];
			skinBufferMaxSize = vertexCount;
		}
		skinBufferSize = vertexCount;
	}

	virtual
	void drawModelSkin(afModel* model, afPoseBuffer* buffer)
	{
		glColor3f(0.f, 1.f, 1.f);
	
		int poseState;
		if (buffer && (buffer->posecount == model->boneCount)) poseState = (model->boneCount)?1:0;
		else if (model->boneCount && model->bones[0].poseCount) poseState = 2;
		else poseState = 0;
	
		int weightIndex = 0;
		switch (poseState) {
			// NOTE(afox): get pose from passed in posebuffer
			case 1:
				initSkinBuffer(model->vertexCount);
				//model->transformPoseBuffer(buffer);
				for (int i = 0; i < skinBufferSize; i++) {
					skinBuffer[i] = model->vertices[i];
					
					afVec4 mix = {}, cur = {};
					float accum = 0, weight;
					for (; weightIndex < model->weightCount &&
							model->weights[weightIndex].vertOffset == i;
							weightIndex++)
					{
						weight = model->weights[weightIndex].boneWeight;
						accum += model->weights[weightIndex].boneWeight;
	
						cur = buffer->poses[model->weights[weightIndex].boneIndex]
							.transform(skinBuffer[i].position)
							*model->weights[weightIndex].boneWeight;
	
						mix += cur;
					}
					skinBuffer[i].position = mix/accum;
				}
				break;
	
			// NOTE(afox): use model's bind pose
			case 2:
				initSkinBuffer(model->vertexCount);
				for (int i = 0; i < skinBufferSize; i++) {
				}
				break;
			default:
				return drawModel(model);
				break;
		}
	
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(4, GL_FLOAT, sizeof(afVertex), &(skinBuffer[0].position));
		for (int i = 0; i < model->meshCount; i++){
			afMesh * cmesh = &model->meshes[i];
			glDrawElements(GL_TRIANGLES, cmesh->indexCount, GL_UNSIGNED_INT, (unsigned int*)cmesh->indices);
		}
		glDisableClientState(GL_VERTEX_ARRAY);
	}

	virtual
	void begin2D() {
		glDisable(GL_DEPTH_TEST);
	}

} afRendererGL2;
