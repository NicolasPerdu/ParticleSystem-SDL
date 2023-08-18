#include "Firework.h"

#include <iostream>

//Group gp(image, 1280/2.0f, 720/2.0f, 100000, 10)
Firework::Firework(GPU_Image* image, float x, float y, float resX, float resY, size_t max, float emitFreq) {
	this->image = image;
	this->x = x;
	this->y = y;
	this->resX = resX;
	this->resY = resY;
	this->max = max;
	this->emitFreq = emitFreq;
	this->particles = new Particle[max];
}

Firework::~Firework() {
	delete[](particles);
}

void Firework::render(GPU_Target *screen, double time) {
	update(time);

	for (size_t i = 0; i < max; ++i) {
		if (particles[i].set == 1) {
			GPU_SetColor(image, SDL_Color{ particles[i].color.r, particles[i].color.g, particles[i].color.b, (Uint8)(255 * particles[i].cur_lifetime / particles[i].lifetime) });
			GPU_BlitTransformX(image, &particles[i].rect, screen, particles[i].x, particles[i].y, image->w / 4.0f, image->w / 4.0f, particles[i].rot, 0.2f, 0.2f);
		}
	}
}

void Firework::update(float time) {
	for (size_t i = 0; i < max; ++i) {
		if (particles[i].set == 1) {
			particles[i].x = particles[i].x + time * particles[i].vx + 0.5f*time*time*particles[i].ax;
			particles[i].y = particles[i].y + time * particles[i].vy + 0.5f*time*time*particles[i].ay;
			particles[i].cur_lifetime = particles[i].cur_lifetime - time;

			if (particles[i].cur_lifetime <= 0) {
				particles[i].reset();
			}
		}
	}

	timeEmit = timeEmit + time;
	GPU_Rect rect{ 0.0f, 0.0f, image->w, image->h };
	float r = 1;
	size_t nb_explosion = 4 + rand()%13;

	while (emitFreq < timeEmit) {
		for (size_t i = 0; i < nb_explosion; i++) {
			size_t maxEmitSimul = 50 + rand()%100;
			size_t nbEmit = 0;
			float xO = rand() % resX;
			float yO = rand() % resY;

			Uint8 color_r = rand() % 255;
			Uint8 color_g = rand() % 255;
			Uint8 color_b = rand() % 255;
		
			float th = (float)((rand() % 360)*M_PI / 180.0f);

			for (size_t i = 0; i < max; ++i) {
				if (nbEmit >= maxEmitSimul) {
					break;
				}
				else if (particles[i].set == 0) {
					float th2 = (float)((rand() % 360)*M_PI / 180.0f);
					float posX = xO + r * cos(th2);
					float posY = yO + r * sin(th2);

					particles[i].update(image, rect, lifetime, posX, posY, xO, yO, (vel + (rand() % 10) / 50.f)*cos(th2) , (vel + (rand() % 10) / 50.f)*sin(th2), acc*cos(th2), acc*sin(th2), (float)(rand() % 360), th2, SDL_Color{ color_r, color_g, color_b, 255 });
					++nbEmit;
				}
			}
		}
		timeEmit = timeEmit - emitFreq;
	}
}