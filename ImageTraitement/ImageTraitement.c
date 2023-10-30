#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>

void grayscale


int main(int argc, char** argv)
{
	//check if the number of argument is okay
	if(argc < 2 || argc > 3)
		err(EXIT_FAILURE, "Too many/ Not enough argument passed to Image traitement");

	//Load input image
	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_JPG);

	SDL_Surface* image = IMG_Load("%s", argv[1]);
	if(!image)
	{
		err(EXIT_FAILURE, "Image NULL");
	}

	// create target surface
	SDL_Surface *output_surface = SDL_CreateRGBSurface(0,
            image->w, image->h, 32,
            0, 0, 0, 0);

	// create software renderer that renders to target surface
	SDL_Renderer *renderer = SDL_CreateSoftwareRenderer(output_surface);

	// create texture from input surface
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, image);

	//find the angle to rotate with
	int angle = 0;
	if(argc == 3)
	{
		for(size_t i = 0; argv[2][i]; i++)
			angle = angle * 10 + (int)(argv[2][i] - '0');
	}
	else
	{
		//find the angle automatically
	}

	//grayscale + Black n white



	//rotate



	//vage



	// save target surface to JPEG file
	IMG_SaveJPG(output_surface, "output_%s", argv[1], 90);


	//free the memory
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(image);
	SDL_DestroyRenderer(renderer);
	SDL_FreeSurface(output_surface);

	IMG_Quit();
	SDL_Quit();

	return EXIT_SUCCESS;
}
