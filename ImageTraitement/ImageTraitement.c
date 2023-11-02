#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <err.h>

Uint32 puissance(int x, int n)
{
	Uint32 res = 1;

	for(int i = 0; i < n; i++)
		res = res * x;

	return res;
}

int fact(int n)
{
	if(n <= 1) return 1;
	return n * fact(n - 1);
}

int KparmisN(int k, int n)
{
	return fact(n) / (fact(k) * fact(n - k));
}

//Create a gauss matrix of n*n to make a good blur
//See https://fr.wikipedia.org/wiki/Noyau_(traitement_d'image)
int** GaussMat(int n)
{
	int** a = malloc(n*sizeof(int));
	int* binome = malloc(n*sizeof(int));

	//calcul every coef of the mat
	for(size_t i = 0; i < (size_t)n; i++)
		binome[i] = KparmisN(i,n);

	//create the mat
	for(size_t i = 0; i < (size_t)n; i++)
	{
		a[i] = malloc(n*sizeof(int));

		for(size_t j = 0; j < (size_t)n; j++)
			a[i][j] = binome[j] * binome[j];

	}
	free(binome);

	return a;
}

Uint32 ApplyMatOnPix(SDL_Surface* image, int** mat, size_t n,
			ssize_t i, ssize_t j, char blur)
{
	Uint32 tot = 0;
	Uint32* pixels = image->pixels;

	i--;
	j--;
	for(size_t m = 0; m < n; m++)
	{
		for(size_t p = 0; p < n; p++)
		{
			if(i >= 0 && i < image->h
				&& j >= 0 && j < image->w)
				tot += mat[m][p] *
					pixels[i * image->h + j];

			j++;
		}

		i++;
		j = j - n + 1;
	}
	if(blur) tot = tot / puissance(2, 2*((int)n +1));

	return tot;

}


void ApplyMatOnImage(int** mat, size_t n,
			SDL_Surface* image, char blur)
{
	Uint32* pixels = image->pixels;

	SDL_LockSurface(image);

	//run through the list to get every pixel
	for(ssize_t i = 0; i < image->h; i++)
	{
		for(ssize_t j = 0; j < image->w; i++)
		{
			pixels[i * image->h + j] =
				ApplyMatOnPix(image,
					mat, n, i, j, blur);
		}
	}

	SDL_UnlockSurface(image);

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
	int** gauss = GaussMat(3);
	ApplyMatOnImage(gauss, 3, image, 1);



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
