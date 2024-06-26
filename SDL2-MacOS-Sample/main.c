#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SDL2/SDL.h>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#define STBI_MAX_DIMENSIONS 8192
#include "stb_image.h"

static const int WINDOW_WIDTH = 800;
static const int WINDOW_HEIGHT = 600;

static void ShowError(const char *title, const char *message)
{
	fprintf(stderr, "ERROR: %s: %s\n", title, message);
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, title, message, NULL);
}

static SDL_Texture *loadImage(SDL_Renderer *renderer, const char *filename)
{
	char absolutePathBuf[512];
	const char *absolutePath;
	if(filename[0] == '/') {
		absolutePath = filename;
	} else {
		SDL_snprintf(absolutePathBuf, sizeof(absolutePathBuf), "%s%s", SDL_GetBasePath(), filename);
		absolutePath = absolutePathBuf;
	}

#ifdef DEBUG
	printf("%s: absolutePath: %s\n", __FUNCTION__, absolutePath);
#endif

	int width, height;
	uint8_t *pixelData = stbi_load(absolutePath, &width, &height, NULL, 4);
	if(pixelData == NULL) {
		ShowError("Can't Load Image", stbi_failure_reason());
		return NULL;
	}

	SDL_Surface *surf = SDL_CreateRGBSurfaceWithFormatFrom(pixelData, width, height, 32, width * 4, SDL_PIXELFORMAT_RGBA32);
	stbi_image_free(pixelData);
	if(surf == NULL) {
		ShowError("Can't Create Surface", SDL_GetError());
		return NULL;
	}

	SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, surf);
	SDL_FreeSurface(surf);
	if(tex == NULL) {
		ShowError("Can't Create Texture", SDL_GetError());
	}

	return tex;
}

int main(int argc, char **argv)
{
	if(SDL_Init(SDL_INIT_VIDEO) != 0) {
		ShowError("Can't Init SDL", SDL_GetError());
		return EXIT_FAILURE;
	}

	atexit(SDL_Quit);

	uint32_t windowFlags = SDL_WINDOW_SHOWN;
	SDL_Window *window = SDL_CreateWindow("Hello, SDL2!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
										  WINDOW_WIDTH, WINDOW_HEIGHT, windowFlags);
	if(window == NULL) {
		ShowError("Can't Create Window", SDL_GetError());
		return EXIT_FAILURE;
	}

	uint32_t renderFlags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, renderFlags);
	if(renderer == NULL) {
		ShowError("Can't Create Renderer", SDL_GetError());
		return EXIT_FAILURE;
	}

	SDL_Texture *image = loadImage(renderer, "hello_sdl2.png");
	if(image == NULL) {
		return EXIT_FAILURE;
	}

	SDL_SetTextureScaleMode(image, SDL_ScaleModeBest);

	int run = 1;
	double angle = 0.0f;
	const double rotateAmount = 0.5;
	float scaleFactor = 1.0f;
	float scaleAmount = 0.01f;
	while(run) {
		SDL_Event event;
		while(SDL_PollEvent(&event)) {
			switch(event.type) {
			case SDL_QUIT:
				run = 0;
				break;
			}
		}

		SDL_SetRenderDrawColor(renderer, 0, 127, 255, 255);
		SDL_RenderClear(renderer);

		int tw, th;
		SDL_QueryTexture(image, NULL, NULL, &tw, &th);
		SDL_FRect displayRect;
		displayRect.w = (float)tw * scaleFactor;
		displayRect.h = (float)th * scaleFactor;
		displayRect.x = ((float)WINDOW_WIDTH / 2.0f) - ((float)displayRect.w / 2.0f);
		displayRect.y = ((float)WINDOW_HEIGHT / 2.0f) - ((float)displayRect.h / 2.0f);

		SDL_RenderCopyExF(renderer, image, NULL, &displayRect, angle, NULL, SDL_FLIP_NONE);

		angle += rotateAmount;
		if(angle >= 360.0) {
			angle -= 360.0;
		}

		scaleFactor += scaleAmount;
		if(scaleFactor >= 2.0f || scaleFactor <= 0.5f) {
			scaleAmount *= -1.0f;
		}

		SDL_RenderPresent(renderer);
	}

	SDL_DestroyTexture(image);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	return 0;
}
