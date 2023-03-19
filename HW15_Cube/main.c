#include "SDL2/SDL.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define WINDOW_SIZE 500

SDL_Window *g_Window = NULL;
SDL_GLContext *g_Context = NULL;
float g_angle = 0;

bool init()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf( "Unable to init SDL.\n");
		return false;
	}

	g_Window = SDL_CreateWindow("CUBE",
							   SDL_WINDOWPOS_UNDEFINED,
							   SDL_WINDOWPOS_UNDEFINED,
							   WINDOW_SIZE,
							   WINDOW_SIZE,
							   SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	if (g_Window == NULL) {
		printf("Window could not be created!\n");
		return false;
	}

	g_Context = SDL_GL_CreateContext(g_Window);
	if (g_Context == NULL) {
		printf("OpenGL context could not be created!\n");
		return false;
	}

	if (SDL_GL_SetSwapInterval(1) < 0) {
		printf("Warning: Unable to set VSync!\n");
	}

	GLenum error = GL_NO_ERROR;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	error = glGetError();
	if (error != GL_NO_ERROR) {
		printf("Unable to initialize OpenGL!\n");
		return false;
	}
	glClearColor(0, 0, 0, 0);
	return true;
}

void updateScene()
{
	glLoadIdentity();
	glRotatef(g_angle, 0.f, 1.0f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBegin(GL_QUADS);
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(-0.5f, -0.5f, -0.5f);
	glVertex3f(0.5f, -0.5f, -0.5f);
	glVertex3f(0.5f, 0.5f, -0.5f);
	glVertex3f(-0.5f, 0.5f, -0.5f);

	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(-0.5f, -0.5f, 0.5f);
	glVertex3f(0.5f, -0.5f, 0.5f);
	glVertex3f(0.5f, 0.5f, 0.5f);
	glVertex3f(-0.5f, 0.5f, 0.5f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-0.5f, 0.5f, -0.5f);
	glVertex3f(-0.5f, 0.5f, 0.5f);
	glVertex3f(0.5f, 0.5f, 0.5f);
	glVertex3f(0.5f, 0.5f, -0.5f);

	glColor3f(1.0f, 0.0f, 1.0f);
	glVertex3f(-0.5f, -0.5f, -0.5f);
	glVertex3f(-0.5f, -0.5f, 0.5f);
	glVertex3f(0.5f, -0.5f, 0.5f);
	glVertex3f(0.5f, -0.5f, -0.5f);

	glColor3f(1.0f, 1.0f, 0.0f);
	glVertex3f(0.5f, -0.5f, -0.5f);
	glVertex3f(0.5f, 0.5f, -0.5f);
	glVertex3f(0.5f, 0.5f, 0.5f);
	glVertex3f(0.5f, -0.5f, 0.5f);

	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(-0.5f, -0.5f, -0.5f);
	glVertex3f(-0.5f, 0.5f, -0.5f);
	glVertex3f(-0.5f, 0.5f, 0.5f);
	glVertex3f(-0.5f, -0.5f, 0.5f);
	glEnd();
	glPopMatrix();
	glEnable(GL_DEPTH_TEST);

}

static void deinit()
{
	if (g_Context) {
		SDL_GL_DeleteContext(g_Context);
		g_Context = NULL;
	}
	if (g_Window) {
		SDL_DestroyWindow(g_Window);
		g_Window = NULL;
	}
	SDL_Quit();
}

int main()
{
	if (!init()) {
		printf("Failed to initialize!\n");
		return EXIT_FAILURE;
	}

	bool quit = false;
	SDL_Event event;
	SDL_StartTextInput();

	while (!quit) {
		while (SDL_PollEvent(&event) != 0) {
			switch(event.type) {
			case SDL_QUIT:
				quit = true;
				break;
			case SDL_KEYUP:
				if (event.key.keysym.sym == SDLK_ESCAPE) {
					quit = true;
				}
				break;

			default:
				break;
			}
		}
		g_angle = g_angle + 1 % 360;
		usleep(10000); // 10 мс.
		updateScene();
		SDL_GL_SwapWindow(g_Window);
	}
	SDL_StopTextInput();
	deinit();
	return 0;
}
