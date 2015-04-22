
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
	return r;
}

