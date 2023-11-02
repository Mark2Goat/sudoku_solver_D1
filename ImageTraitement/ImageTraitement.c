#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <err.h>

int fact(int n)
{
	if(n <= 1) return 1;
	return n * fact(n - 1);
}

int KparmisN(int k, int n)
{
	return fact(n) / (fact(k) * fact(n - k));
}

int** GaussMat(int n)
{
	int** a = malloc(n*sizeof(int));
	for(size_t i = 0; i < (size_t)n; i++)
	{
		a[i] = malloc(n*sizeof(int));
		for(size_t j = 0; j < (size_t)n; j++)
		{

		}
	}
	return a;
}

Uint32 PtG(Uint32 pixel_color, SDL_PixelFormat* format)
{
	Uint8 r, g, b;

	SDL_GetRGB(pixel_color, format,&r, &g, &b);

	Uint8 average = 0.3*r+0.59*g+0.11*b;

	if(average <= 127) average = 0;

	else average = 255;

	Uint32 color = SDL_MapRGB(format, average, average, average);

	return color;
}

void grayscale(SDL_Surface* image)
{
	Uint32* pixels = image->pixels;

	int len = image->w * image->h;

	SDL_PixelFormat* format = image->format;

	if(format == NULL)
		errx(EXIT_FAILURE, "can't find the format of the image");

	SDL_LockSurface(image);

	for(size_t i = 0; i < (size_t)len; i++)
		pixels[i] = PtG(pixels[i], format);

	SDL_UnlockSurface(image);
}


int main(int argc, char** argv)
{
	//check if the number of argument is okay
	if(argc < 2 || argc > 3)
		err(EXIT_FAILURE, "Too many/ Not enough argument passed to Image traitement");

	//Load input image
	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_JPG);

	SDL_Surface* image = IMG_Load(argv[1]);
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
	grayscale(image);


	//rotate



	//blur




	// save target surface to JPEG file
	IMG_SaveJPG(output_surface, "output.jpeg", 90);


	//free the memory
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(image);
	SDL_DestroyRenderer(renderer);
	SDL_FreeSurface(output_surface);

	IMG_Quit();
	SDL_Quit();

	return EXIT_SUCCESS;
}
