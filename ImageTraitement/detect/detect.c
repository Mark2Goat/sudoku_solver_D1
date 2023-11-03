#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

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

Uint32 getpixel(SDL_Surface* surface,int x ,int y){
	int bpp = surface->format->BytesPerPixel;
	Uint8* p = (Uint8*)surface->pixels + y*surface->pitch+x*bpp;

	switch(bpp){
		case 1:
			return *p;
		case 2:
			return *(Uint16*)p;
		case 3:
			if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
				return p[0] << 16 | p[1] << 8 | p[2];
			else
				return p[0] | p[1] << 8 | p[2] << 16;
		case 4:
			return *(Uint32*)p;
		default:
			return 0;
	
	}

}

SDL_Surface* houghTransform(SDL_Surface* image){
	int maxDist = sqrt(image->w*image->w + image->h*image->h);
	int *accumulator = calloc(maxDist*180,sizeof(int));
	SDL_Surface* newImage = SDL_CreateRGBSurface(0,image->w,image->h,32,0,0,0,0);
	if(newImage == NULL)
		err(EXIT_FAILURE,"fail to create newImage");
	for(int x=0;x<image->h;++x){
		for(int y =0;y<image->w;++y){
			Uint32 pixel = getpixel(image,y,x);
			Uint8 r,g,b;
			SDL_GetRGB(pixel,image->format,&r,&g,&b);
			if(r > 0){
				for(int theta = 0;theta<180;theta++){
					double radians = theta*M_PI/180;
					int dist = x*cos(radians)+y*sin(radians);
					dist += maxDist / 2;
					accumulator[theta*maxDist + dist]++;
					}	
			}
		}
	}
	for(int x=0;x<image->h;++x){
		for(int y =0;y<image->w;++y){
			Uint32 pixel = getpixel(image,y,x);
			Uint8 r,g,b;
			SDL_GetRGB(pixel,image->format,&r,&g,&b);
			if(r > 0){
				for(int theta = 0;theta<180;theta++){
					double radians = theta*M_PI/180;
					int dist = x*cos(radians)+y*sin(radians);
					dist += maxDist / 2;
					if(accumulator[theta*maxDist + dist] > image->w*0.4){
						putpixel(newImage,y,x,255);						
					}
				}
					
			}
		}
	}
	return newImage;
}


int main(int argc, char** argv){
	SDL_Surface* image = IMG_Load(argv[1]);
	//houghTransform(image);
	
	IMG_SavePNG(houghTransform(image),"grid_detect");
}

