#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <math.h>
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

Uint32 getpixel(SDL_Surface* surface, int x, int y)
{
    int bpp = surface->format->BytesPerPixel;
    Uint8* p = (Uint8*)surface->pixels + y * surface->pitch + x * bpp;

    switch (bpp) {
        case 1:
            return *p;
        case 2:
            return *(Uint16*)p;
        case 3:
            if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
                return p[0] << 16 | p[1] << 8 | p[2];
            else
                return p[0] | p[1] << 8 | p[2] << 16;
        case 4:
            return *(Uint32*)p;
        default:
            return 0;
    }
}


void putpixel(SDL_Surface* surface, int x, int y, Uint32 pixel) {
    int bpp = surface->format->BytesPerPixel;
    Uint8* p = (Uint8*)surface->pixels + y * surface->pitch + x * bpp;

    switch (bpp) {
        case 1:
            *p = pixel;
            break;
        case 2:
            *(Uint16*)p = pixel;
            break;
        case 3:
            if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
                p[0] = (pixel >> 16) & 0xFF;
                p[1] = (pixel >> 8) & 0xFF;
                p[2] = pixel & 0xFF;
            } else {
                p[0] = pixel & 0xFF;
                p[1] = (pixel >> 8) & 0xFF;
                p[2] = (pixel >> 16) & 0xFF;
            }
            break;
        case 4:
            *(Uint32*)p = pixel;
            break;
    }
}

//Create a gauss matrix of n*n to make a good blur
//See https://fr.wikipedia.org/wiki/Noyau_(traitement_d'image)
int** GaussMat(int n)
{
	int** a = malloc(n*sizeof(int));
	int* binome = malloc(n*sizeof(int));

	//calcul every coef of the mat
	for(size_t i = 0; i < (size_t)n; i++)
		binome[i] = KparmisN(i,n - 1);

	//create the mat
	for(size_t i = 0; i < (size_t)n; i++)
	{
		a[i] = malloc(n*sizeof(int));

		for(size_t j = 0; j < (size_t)n; j++)
			a[i][j] = binome[i] * binome[j];

	}
	free(binome);

	return a;
}

void ApplyMatOnPix(SDL_Surface* image, int** mat, int n, int x, int y, char blur)
{
	Uint8 r, g, b;
	int newR = 0, newG = 0, newB = 0;

	for(int i = -1; i <= 1; i++)
	{
		for(int j = -1; j <= 1; j++)
		{
			int nx = x + i;
			int ny = y + j;
			if(nx >= 0 && nx < image->h
				&& ny >= 0 && ny < image->w)
			{
				Uint32 pixel = getpixel(image, ny, nx);
				SDL_GetRGB(pixel, image->format, &r, &g, &b);

				newR += mat[i+1][j+1] * r;
				newG += mat[i+1][j+1] * g;
				newB += mat[i+1][j+1] * b;
			}
		}
	}
	if(blur)
	{
		int pui = (int)pow(2, 2*((double)n - 1));
		newR = newR / pui;
		newG = newG / pui;
		newB = newB / pui;
	}

	Uint32 pix = SDL_MapRGB(image->format, (Uint8)newR, (Uint8)newG, (Uint8)newB);
	putpixel(image, y, x, pix);

}


void ApplyMatOnImage(int** mat, int n,
			SDL_Surface* image, char blur)
{
	SDL_LockSurface(image);

	//run through the list to get every pixel
	for(int i = 0; i < image->h; i++)
	{
		for(int j = 0; j < image->w; j++)
		{
			ApplyMatOnPix(image, mat, n, i, j, blur);
		}
	}

	SDL_UnlockSurface(image);

}


void grayscale(SDL_Surface* image, int ave)
{
	Uint32 pixel;
	Uint8 r, g, b, a;

	for(int i = 0; i < image->h; i++)
	{
		for(int j = 0; j < image->w; j++)
		{
			pixel = getpixel(image, j, i);
			SDL_GetRGBA(pixel, image->format, &r, &g, &b, &a);

			Uint8 average = 0.3*r+0.59*g+0.11*b;
			//average = (average / 16)*16;

			if((int)pixel <= ave) average = 0;
			else average = 255;

			pixel = SDL_MapRGBA(image->format, average, average, average, a);
			putpixel(image, j, i, pixel);
		}
	}
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

	//find the angle to rotate with
	//int angle = 0;
	//if(argc == 3)
	//{
	//	for(size_t i = 0; argv[2][i]; i++)
	//		angle = angle * 10 + (int)(argv[2][i] - '0');
	//}
	//else
	//{
	//	//find the angle automatically
	//}


	//grayscale + Black n white
	int average = 0;
	int count = 0;
	for(int x = 0; x < image->h; x++)
	{
		for(int y = 0; y < image->w; y++)
		{
			average += getpixel(image, y, x);
			count++;
		}
	}
	average = average / count;

	grayscale(image, average);


	//rotate



	//blur
	//int size = 5;
	//int** gauss = GaussMat(size);
	//ApplyMatOnImage(gauss, size, image, 1);



	// save target surface to JPEG file
	IMG_SaveJPG(image, "output.jpg", 100);


	//free the memory
	SDL_FreeSurface(image);

	IMG_Quit();
	SDL_Quit();

	return EXIT_SUCCESS;
}
