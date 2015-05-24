
#include "afrendergl2.hpp"
#include "af3dmath.hpp"

void afG2Init(afRenderer * self, SDL_Window * window){
	self->context = SDL_GL_CreateContext(window);
	self->window = window;
	afLoadGL();
	glClearColor(0,0,0,1);
	//self->projection.transpose();
}

void afG2BeginFrame(afRenderer * self){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void afG2EndFrame(afRenderer * self){
	SDL_GL_SwapWindow(self->window);
}

void afG2Begin3D(afRenderer * self){
	int w, h;
	SDL_GetWindowSize(self->window, &w, &h);
	glEnable(GL_DEPTH_TEST);
	self->projection.perspective(90, (float)w/h, 1, 100);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//gluPerspective(90, (float)w/h, 1, 100);
	glLoadMatrixf(self->projection.m);
	glMatrixMode(GL_MODELVIEW);
}

void afG2DrawModel(afRenderer * self, afModel* model){
	glColor3f(1.f, 0.f, 1.f);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(4, GL_FLOAT, sizeof(afVertex), &(model->vertices[0].position));
	for (uint i = 0; i < model->meshCount; i++){
		afMesh * cmesh = &model->meshes[i];
		glDrawElements(GL_TRIANGLES, cmesh->indexCount, GL_UNSIGNED_INT, (unsigned int*)cmesh->indices);
	}
	glDisableClientState(GL_VERTEX_ARRAY);
}

afVertex * skinBuffer = 0;
int skinBufferSize = 0;
int skinBufferMaxSize = 0;

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

void afG2DrawModelSkin(afRenderer * self, afModel* model, afPoseBuffer* buffer)
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
			for (int i = 0; i < skinBufferSize; i++) {
				skinBuffer[i] = model->vertices[i];
				
				afVec4 mix = {}, cur = {};
				float accum = 0;
				for (; weightIndex < model->weightCount &&
						model->weights[weightIndex].vertOffset == i;
						weightIndex++)
				{
					accum += model->weights[weightIndex].boneWeight;

					cur = buffer->poses[model->weights[weightIndex].boneIndex]
						.transformVertex(skinBuffer[i].position)
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
			return afG2DrawModel(self, model);
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


void afG2Begin2D(afRenderer * self){
	glDisable(GL_DEPTH_TEST);
}

afRenderer createRendererG2(SDL_GLContext context){
	afRenderer r = {};
	r.context = context;
	r.init = afG2Init;
	r.beginFrame = afG2BeginFrame;
	r.endFrame = afG2EndFrame;
	r.begin2D = afG2Begin2D;
	r.begin3D = afG2Begin3D;
	r.drawModel = afG2DrawModel;
	r.drawModelSkin = afG2DrawModelSkin;
	return r;
}

