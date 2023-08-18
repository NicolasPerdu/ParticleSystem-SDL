#include <chrono>
#include <vector>
#include <algorithm>
#include <iostream>
#include "Firework.h"
#include "gl_core_3_0.h"

#include "SDL.h"
#include "SDL_gpu.h"

typedef std::chrono::high_resolution_clock Clock;

int main(int argc, char *argv[]) {
	SDL_Init(SDL_INIT_VIDEO);

	GPU_Target *screen = GPU_InitRenderer(GPU_RENDERER_OPENGL_3, 1920, 1080, GPU_DEFAULT_INIT_FLAGS);

	if (screen == NULL || ogl_LoadFunctions() == ogl_LOAD_FAILED) {
		std::cout << "error initialization OpenGL\n";
	}

	GPU_Image* image = GPU_LoadImage("particle.psd");
	double fps = 0;

	Firework gp(image, 1920/2.0f, 1080/2.0f, 19020, 1080, 75000, 10);

	gp.lifetime = 1000;
	gp.vel = 0.00f;
	gp.acc = 0.0025f;

	double timeElapsed = 0;
	SDL_Event event;
	bool done = false;

	while (!done) {
		double elapsedNano = 0;
		auto t1 = Clock::now();

		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_KEYDOWN) {
				if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
					done = 1;
				}
			}	
		}

		GPU_Clear(screen);
		gp.render(screen, timeElapsed);
		GPU_Flip(screen);

		auto t2 = Clock::now();

		elapsedNano = (double)(std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count());

		if (elapsedNano > 0) {
			double diff = ((1000000000.f / 30.f) - elapsedNano) / 1000000.f; // milli

			if (diff > 0) {
				SDL_Delay((Uint32)diff);
			}
		}

		auto t3 = Clock::now();

		timeElapsed = (double)(std::chrono::duration_cast<std::chrono::nanoseconds>(t3 - t1).count())/ 1000000.f;
		fps = 1000.f / timeElapsed;
	}

	GPU_FreeImage(image);
	GPU_Quit();

	return 0;
}