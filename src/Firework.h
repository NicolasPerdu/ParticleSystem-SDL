#pragma once
#include <SDL_gpu.h>
#include <vector>

#include "Particle.h"

//#define PASSTHROUGH_ALL (PASSTHROUGH_VERTICES | PASSTHROUGH_TEXCOORDS | PASSTHROUGH_COLORS)

class Firework {
public:
	Firework(GPU_Image* image, float x, float y, float resX, float resY, size_t max, float emitFreq);
	~Firework();
	void update(float timeElapsed);
	void render(GPU_Target * screen, double timeElapsed);
	float lifetime;
	float vel;
	float acc;
	float emitFreq;
	size_t max;
	Particle* particles;
private:
	GPU_Image* image;
	float x;
	float y;
	size_t resX;
	size_t resY;
	float timeEmit;
};