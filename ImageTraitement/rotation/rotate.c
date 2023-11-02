#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <math.h>
#include <err.h>


Uint32 getpixel(SDL_Surface* surface, int x, int y) {
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




SDL_Surface* rotate(SDL_Surface* image , double a){
	
	int newWidth = ceil(fabs(image->w * cos(a)) + fabs(image->h * sin(a)));
	int newLength = ceil(fabs(image->w * sin(a)) + fabs(image->h * cos(a)));

	SDL_Surface* newImage = SDL_CreateRGBSurface(0,newWidth,newLength,32,0,0,0,0); 
	if(newImage == NULL){
		err(EXIT_FAILURE,"Failed to create a new image !\n");
	}



        for(int y = 0; y<newLength;++y){
		for(int x = 0; x < newWidth ; ++x){
			double angle = atan2(y-newLength / 2, x - newWidth / 2);

			double distance = sqrt(pow((y-newLength/2),2)+pow((x-newWidth/2),2));

			double newAngle = angle - a;

			int newX = distance*cos(newAngle) + image->w /2;
			int newY = distance*sin(newAngle) + image->h /2;

			if(newX >= 0 && newX < image->w && newY >= 0 && newY < image->h){
				putpixel(newImage, x, y, getpixel(image, newX, newY));
			}
		
		}
	
	}

	return newImage;

}

int main(int argc,char** argv){
	if(argc < 3 || argc > 4){
		err(EXIT_FAILURE,"Too many or not enough arguments !\n");

	}
	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_JPG);
	SDL_Surface* image = IMG_Load(argv[1]);
	if(!image)
		err(EXIT_FAILURE,"Image does not exist !\n");

	char* angle = argv[2];
	double numangle = strtod(angle, NULL);
	numangle = (M_PI/180) * numangle;
	SDL_Surface* newImage = rotate(image,numangle);

	IMG_SaveJPG(newImage,"rotated.jpg",90);

}
