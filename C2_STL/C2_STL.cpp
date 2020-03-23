#include <SDL.h>
#include <iostream>
#include <stdio.h>
#include <string>
#include <cmath>
#include <math.h>
#include <list>
#include <stdlib.h>
#include <string.h>
//#include "Matriz.h"

//Screen dimension constants
int SCREEN_WIDTH = 720;
int SCREEN_HEIGHT = 480;
int tam = 30;

//Starts up SDL and creates window
bool init();


//Frees media and shuts down SDL
void close();

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;
SDL_Renderer* renderer = NULL;

int xA = 0;
int yA = 0;
int xB = 0;
int yB = 0;
int radio = 0;
bool drawing = false;


struct Line
{
	int xA;
	int yA;
	int xB;
	int yB;
};
struct Circle
{
	int radio;
	int xA;
	int yA;

};
struct Pixel
{
	int xA;
	int yA;
};
struct Erase
{
	int xA;
	int yA;
	int radio;
};

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}

		//Create window
		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
			if (gRenderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);


			}
		}
	}

	return success;
}

void close()
{
	//Destroy window
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;


	SDL_Quit();
}

void SetScreen()
{
	if (SCREEN_WIDTH == 0 || SCREEN_HEIGHT == 0)
	{
		SCREEN_WIDTH = 720;
		SCREEN_HEIGHT = 480;
	}
	if (tam == 0)
	{
		tam = 32;
	}
}

void DrawPlain()
{
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

	//Dibujamos en y desde la mitad
	SDL_RenderDrawPoint(gRenderer, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	for (int i = SCREEN_HEIGHT / 2; i < SCREEN_HEIGHT;)
	{
		i += tam;
		SDL_RenderDrawPoint(gRenderer, SCREEN_WIDTH / 2, SCREEN_HEIGHT - i);
		SDL_RenderDrawPoint(gRenderer, SCREEN_WIDTH / 2, i);
	}

	//Dibujamos en x desde la mitad
	for (int i = SCREEN_WIDTH / 2; i < SCREEN_WIDTH;)
	{
		i += tam;
		SDL_RenderDrawPoint(gRenderer, SCREEN_WIDTH - i, SCREEN_HEIGHT / 2);
		SDL_RenderDrawPoint(gRenderer, i, SCREEN_HEIGHT / 2);
	}

	SDL_SetRenderDrawColor(gRenderer, 0x44, 0x44, 0x44, 0xFF);
	for (int i = (SCREEN_WIDTH / 2) + tam; i < SCREEN_WIDTH; i += tam)
	{
		for (int j = (SCREEN_HEIGHT / 2) + tam; j < SCREEN_HEIGHT; j += tam)
		{
			SDL_RenderDrawPoint(gRenderer, i, j);
			SDL_RenderDrawPoint(gRenderer, SCREEN_WIDTH - i, j);
			SDL_RenderDrawPoint(gRenderer, SCREEN_WIDTH - i, SCREEN_HEIGHT - j);
			SDL_RenderDrawPoint(gRenderer, i, SCREEN_HEIGHT - j);
		}
	}


}
void DrawFilledCircle(int x0, int y0, int radius)
{
	int x = radius;
	int y = 0;
	int xChange = 1 - (radius << 1);
	int yChange = 0;
	int radiusError = 0;
	SDL_SetRenderDrawColor(renderer, 242, 242, 242, 255);
	while (x >= y)
	{
		for (int i = x0 - x; i <= x0 + x; i++)
		{
			SDL_RenderDrawPoint(renderer,i, y0 + y);
			SDL_RenderDrawPoint(renderer,i, y0 - y);
		}
		for (int i = x0 - y; i <= x0 + y; i++)
		{
			SDL_RenderDrawPoint(renderer,i, y0 + x);
			SDL_RenderDrawPoint(renderer,i, y0 - x);
		}

		y++;
		radiusError += yChange;
		yChange += 2;
		if (((radiusError << 1) + xChange) > 0)
		{
			x--;
			radiusError += xChange;
			xChange += 2;
		}
	}
}


void bezierCurve(int x[], int y[])
{
	double xu = 0.0, yu = 0.0, u = 0.0;
	int i = 0;
	for (u = 0.0; u <= 1.0; u += 0.0001)
	{
		xu = pow(1 - u, 3) * x[0] + 3 * u * pow(1 - u, 2) * x[1] + 3 * pow(u, 2) * (1 - u) * x[2]
			+ pow(u, 3) * x[3];
		yu = pow(1 - u, 3) * y[0] + 3 * u * pow(1 - u, 2) * y[1] + 3 * pow(u, 2) * (1 - u) * y[2]
			+ pow(u, 3) * y[3];
		SDL_RenderDrawPoint(renderer, (int)xu, (int)yu);
	}
}
void DrawCircle(int radio, float XC, float YC)
{
	float f = 1 - radio;
	float ddF_x = 1;
	float ddF_y = -2 * radio;
	float x = 0;
	float y = radio;
	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
	SDL_RenderDrawPoint(renderer, XC, YC + radio);
	SDL_RenderDrawPoint(renderer, XC, YC - radio);
	SDL_RenderDrawPoint(renderer, XC + radio, YC);
	SDL_RenderDrawPoint(renderer, XC - radio, YC);

	while (x < y)
	{
		if (f >= 0)
		{
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;
		SDL_RenderDrawPoint(renderer, XC + x , YC + y);
		SDL_RenderDrawPoint(renderer, XC - x , YC + y);
		SDL_RenderDrawPoint(renderer, XC + x , YC - y);
		SDL_RenderDrawPoint(renderer, XC - x , YC - y);
		SDL_RenderDrawPoint(renderer, XC + y, YC + x);
		SDL_RenderDrawPoint(renderer, XC - y, YC + x);
		SDL_RenderDrawPoint(renderer, XC + y, YC - x);
		SDL_RenderDrawPoint(renderer, XC - y, YC - x);
	}
}
void Linea_de_colores(float XA, float XB, float YA, float YB)
{
	float dx;
	float dy;
	float x;
	float y;
	float p;
	float i;
	int signox;
	int signoy;
	int pixeles = 0;

	dx = XB - XA;
	dy = YB - YA;
	x = XA;
	y = YA;

	if (dx >= 0)
	{
		signox = +1;
	}
	else
	{
		signox = -1;
	}
	if (dy >= 0)
	{
		signoy = 1;
	}
	else
	{
		signoy = -1;
	}
	
	if (x == XB)
	{
		SDL_RenderDrawLine(gRenderer, x, y, XB, YB);
	}
	if (abs(dy) <= abs(dx))
	{		
		p = 2 * dy * signoy - dx * signox;
		for (pixeles = 0; pixeles <= dx * signox; pixeles++)
		{
			if (pixeles <= dx / 3) {
				SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
			}
			else if (pixeles <= (dx/3)*2)
			{
				SDL_SetRenderDrawColor(gRenderer, 0x255, 0x00, 0x00, 0xFF);
			}
			else if (pixeles >= (dx / 3) * 2){
				SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x255, 0xFF);
			}
			SDL_RenderDrawPoint(gRenderer, x, y);
			if (p < 0)
			{
				x = x + signox;
				y = y;
				p = p + 2 * dy * signoy;
			}
			else
			{
				x = x + signox;
				y = y + signoy;
				p = p + 2 * dy * signoy - 2 * dx * signox;
			}
		}
	}
	else if (abs(dy) > abs(dx))
	{
		p = 2 * dx * signox - dy * signoy;
		i = 0;
		for (i = 0; 1 <= dy * signoy; i++)
		{
			SDL_RenderDrawPoint(gRenderer, x, y);
			if (p < 0)
			{
				x = x;
				y = y + signoy;
				p = p + 2 * dx * signox - 2 * dy * signoy;
			}
			else if (p >= 0)
			{
				x = x + signox;
				y = y + signoy;
				p = p + 2 * dx + signox - 2 * dy * signoy;
			}

		}
	}
}
void Algotimo_de_Brecenham_completo(float XA, float XB, float YA, float YB)
{
	float dx;
	float dy;
	float x;
	float y;
	float p;
	float i;
	int signox;
	int signoy;

	dx = XB - XA;
	dy = YB - YA;
	x = XA;
	y = YA;

	if (dx >= 0)
	{
		signox = +1;
	}
	else
	{
		signox = -1;
		dx = -dx;
	}
	if (dy >= 0)
	{
		signoy = 1;
	}
	else
	{
		signoy = -1;
		dy = -dy;
	}
	SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0x00);
	if (x == XB)
	{
		SDL_RenderDrawLine(gRenderer, x, y, XB, YB);
	}
	if (abs(dy) <= abs(dx))
	{
		p = 2 * dy * signoy - dx * signox;
		for (int i = 0; i <= dx * signox; i++)
		{
			SDL_RenderDrawPoint(gRenderer, x, y);
			if (p < 0)
			{
				x = x + signox;
				y = y;
				p = p + 2 * dy * signoy;
			}
			else
			{
				x = x + signox;
				y = y + signoy;
				p = p + 2 * dy * signoy - 2 * dx * signox;
			}
		}
	}
	else if (abs(dy) > abs(dx))
	{
		p = 2 * dx * signox - dy * signoy;
		i = 0;
		for (i = 0; 1 <= dy * signoy; i++)
		{
			SDL_RenderDrawPoint(gRenderer, x, y);
			if (p < 0)
			{
				x = x;
				y = y + signoy;
				p = p + 2 * dx * signox - 2 * dy * signoy;
			}
			else if (p >= 0)
			{
				x = x + signox;
				y = y + signoy;
				p = p + 2 * dx + signox - 2 * dy * signoy;
			}

		}
	}
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
}
void DrawSquare(float XA, float YA, float XB, float YB, float XC, float YC, float XD, float YD)
{
	Algotimo_de_Brecenham_completo(XA, XB, YA, YB);
	Algotimo_de_Brecenham_completo(XB, XC, YB, YC);
	Algotimo_de_Brecenham_completo(XC, XD, YC, YD);
	Algotimo_de_Brecenham_completo(XD, XA, YD, YA);
}

void DrawPoint(float x, float y)
{
	x = (SCREEN_WIDTH / 2) + (tam * x);
	y = (SCREEN_HEIGHT / 2) - (tam * y);
	SDL_SetRenderDrawColor(gRenderer, 0x00, 0xFF, 0xFF, 0xFF);
	SDL_RenderDrawPoint(gRenderer, x, y);
}
void DigitalDiferencialAnalyzer(float XA, float XB, float YA, float YB)
{
	int deno;
	float dx;
	float dy;
	float Yinc;
	float Xinc;
	float x;
	float y;
	dx = XB - XA;
	dy = YB - YA;
	if (std::abs(dy) < std::abs(dx))
	{
		deno = std::abs(dx);
	}
	else if (std::abs(dy) >= std::abs(dx))
	{
		deno = std::abs(dy);
	}
	Xinc = dx / deno;
	Yinc = dy / deno;
	x = XA;
	y = YA;
	for (int i = 0; i <= deno; i++) {
		//SDL_SetRenderDrawColor(gRenderer, 0x00, 0xFF, 0xFF, 0xFF);
		/*SDL_RenderDrawPoint(gRenderer, x, y);*/
		DrawPoint(x,y);
		x = x + Xinc;
		y = x + Yinc;
	}


}


//int main(int argc, char* args[])
//{
//	SetScreen();
//
//	if (!init())
//	{
//		printf("Failed to initialize!\n");
//	}
//	else
//	{
//		//Main loop flag
//		bool quit = false;
//
//		//Event handler
//		SDL_Event e;
//
//		//While application is running
//		while (!quit)
//		{
//			SDL_Delay(10);
//			SDL_PollEvent(&e);
//			//Handle events on queue
//			switch (e.type)
//			{
//				//User requests quit
//			case SDL_QUIT:
//				quit = true;
//				break;
//			case SDL_MOUSEBUTTONDOWN:
//				switch (e.button.button)
//				{
//				case SDL_BUTTON_LEFT:
//					xA = e.motion.x;
//					yA = e.motion.y;
//					xB = e.motion.x;
//					yB = e.motion.y;
//					drawing = true;
//					break;
//				}
//					break;
//				case SDL_MOUSEBUTTONUP:
//					switch (e.button.button)
//					{
//					case SDL_BUTTON_LEFT:
//						drawing = false;
//						break;
//					}
//					break;
//				case SDL_MOUSEMOTION:
//					if (drawing)
//					{
//						xB = e.motion.x;
//						yB = e.motion.y;
//					}
//					break;
//			}
//			SDL_SetRenderDrawColor(gRenderer, 0x55, 0x00, 0x00, 0xFF);
//			SDL_RenderClear(gRenderer);
//
//			if(drawing)
//				{
//				SDL_RenderDrawLine(gRenderer, xA, yA, xB, yB);
//				}
//			//Linea_de_colores(300, 500, 300, 200);
//			///*DrawSquare(100,100,200,100,200,200,100,200);
//			//DrawCircle(50, 600, 400);
//			Algotimo_de_Brecenham_completo(575, 575, 400, 375);
//			//Algotimo_de_Brecenham_completo(625, 625, 400, 375);
//			//Algotimo_de_Brecenham_completo(600, 625, 400, 415);
//			//Algotimo_de_Brecenham_completo(600, 625, 415, 415);
//			//Algotimo_de_Brecenham_completo(575, 625, 425, 425);
//			//Algotimo_de_Brecenham_completo(565, 575, 415, 425);
//			DrawFilledCircle(100, 100, 50);
//
//			DrawPlain();
//			
//
//			SDL_RenderPresent(gRenderer);
//		}
//		SDL_DestroyRenderer(gRenderer);
//		SDL_DestroyWindow(gWindow);
//		SDL_Quit();
//	}
//
//	//Free resources and close SDL
//	close();
//
//	return 0;
//}

//Botones
SDL_Rect bPixel;
SDL_Rect bLine;
SDL_Rect bBezier;
SDL_Rect bCircle;
SDL_Rect bErase;
SDL_Rect ColorRect;
SDL_Rect Red;
SDL_Rect Green;
SDL_Rect Blue;


std::list<Line> lines;
std::list<Circle> circles;
std::list<Pixel> pixels;
std::list<Erase> erases;

int i = 0;
int xbez[4], ybez[4], flagDrawn = 0;
int xbb[4] = { 25,30,35,30 };
int ybb[4] = { 110,105,110,115 };
int mousePosX, mousePosY;
int xnew, ynew;
SDL_Event event;
SDL_Window* window;
bool quit = false;

bool pixel = false;
bool line = false;
bool bezier = false;
bool circle = false;
bool erasePixel = false;

bool redb = false;
bool greenb = false;
bool blueb = false;


int main(int argc, char** argv)
{



	// init SDL

	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow("SDL2 line drawing",SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, 0);
	renderer = SDL_CreateRenderer(window, -1, 0);

	// handle events
	SDL_StartTextInput();
	SDL_SetTextInputRect(&Red);
	//SDL_SetTextInputRect(&Green);
	//SDL_SetTextInputRect(&Blue);
	while (!quit)
	{
		SDL_Delay(10);
		SDL_PollEvent(&event);

		switch (event.type)
		{
		case SDL_QUIT:
			quit = true;
			break;
			// TODO input handling code goes here
		case SDL_TEXTINPUT:
		
			/*strcat(text, event.text.text);*/
			break;

		case SDL_MOUSEBUTTONDOWN:
			switch (event.button.button)
			{
			case SDL_BUTTON_LEFT:
				xA = event.motion.x;
				yA = event.motion.y;
				xB = event.motion.x;
				yB = event.motion.y;
				if(bezier)
					if (i < 4)
					{
						xbez[i] = xA;
						ybez[i] = yA;
						i++;
					}

				drawing = true;
				break;
			}
			break;
		case SDL_MOUSEBUTTONUP:
			switch (event.button.button)
			{
			case SDL_BUTTON_LEFT:
				drawing = false;
				if (xA >= 20 && xB >= 20 && xA <= 40 && xB <= 40 && yA >= 20 && yB >= 20 && yA <= 40 && yB <= 40) {
					pixel = true;
					line = false;
					bezier = false;
					circle = false;
					erasePixel = false;
				}
				if (xA >= 20 && xB >= 20 && xA <= 40 && xB <= 40 && yA >= 60 && yB >= 60 && yA <= 80 && yB <= 80) {
					pixel = false;
					line = true;
					bezier = false;
					circle = false;
					erasePixel = false;
				}
				if (xA >= 20 && xB >= 20 && xA <= 40 && xB <= 40 && yA >= 100 && yB >= 100 && yA <= 120 && yB <= 120) {
					pixel = false;
					line = false;
					bezier = true;
					circle = false;
					erasePixel = false;
				}
				if (xA >= 20 && xB >= 20 && xA <= 40 && xB <= 40 && yA >= 140 && yB >= 140 && yA <= 160 && yB <= 160) {
					pixel = false;
					line = false;
					bezier = false;
					circle = true;
					erasePixel = false;
				}
				if (xA >= 20 && xB >= 20 && xA <= 40 && xB <= 40 && yA >= 180 && yB >= 180 && yA <= 200 && yB <= 200) {
					pixel = false;
					line = false;
					bezier = false;
					circle = false;
					erasePixel = true;
				}
				if (xA >= 20 && xB >= 20 && xA <= 40 && xB <= 40 && yA >= 220 && yB >= 220 && yA <= 240 && yB <= 240) {
					redb = false;
					greenb = false;
					blueb = false;
				}
				if (xA >= 20 && xB >= 20 && xA <= 40 && xB <= 40 && yA >= 260 && yB >= 260 && yA <= 280 && yB <= 280) {
					redb = true;
				}
				if (xA >= 20 && xB >= 20 && xA <= 40 && xB <= 40 && yA >= 300 && yB >= 300 && yA <= 320 && yB <= 320) {
					greenb = true;
				}
				if (xA >= 20 && xB >= 20 && xA <= 40 && xB <= 40 && yA >= 340 && yB >= 340 && yA <= 360 && yB <= 360) {
					blueb = true;
				}
				if (line) {
					Line line = { xA, yA, xB, yB };
					lines.push_back(line);
				}
				if (circle) {
					Circle circle = { radio = xB - xA,xA, yA };
					circles.push_back(circle);
				}
				if (pixel) {
					Pixel pixel1 = { xA,yA };
					pixels.push_back(pixel1);
				}
				if (erasePixel)
				{
					Erase erases1 = { xA,yA,radio = xB - xA };
					erases.push_back(erases1);
				}
				break;
			}
			break;
		case SDL_MOUSEMOTION:
			if (drawing)
			{
				xB = event.motion.x;
				yB = event.motion.y;
				if (bezier) {
					xnew = event.motion.x;
					ynew = event.motion.y;

					int j;
					if (flagDrawn == 1)
					{
						for (j = 0; j < i; j++)
						{
							if ((float)sqrt(abs(xnew - xbez[j]) * abs(xnew - xbez[j])
								+ abs(ynew - ybez[j]) * abs(ynew - ybez[j])) < 8.0){
								xbez[j] = xnew;
								ybez[j] = ynew;
							}
						}
					}
					mousePosX = xnew;
					mousePosY = ynew;
				}
			}
			break;
		}
		

		// clear window

		SDL_SetRenderDrawColor(renderer, 242, 242, 242, 255);
		SDL_RenderClear(renderer);

		// TODO rendering code goes here
		bPixel.x = 20;
		bPixel.y = 20;
		bPixel.w = 20;
		bPixel.h = 20;
		bLine.x = 20;
		bLine.y = 60;
		bLine.w = 20;
		bLine.h = 20;
		bBezier.x = 20;
		bBezier.y = 100;
		bBezier.w = 20;
		bBezier.h = 20;
		bCircle.x = 20;
		bCircle.y = 140;
		bCircle.w = 20;
		bCircle.h = 20;
		bErase.x = 20;
		bErase.y = 180;
		bErase.w = 20;
		bErase.h = 20;
		ColorRect.x = 20;
		ColorRect.y = 220;
		ColorRect.w = 20;
		ColorRect.h = 20;
		Red.x = 20;
		Red.y = 260;
		Red.w = 20;
		Red.h = 20;
		Green.x = 20;
		Green.y = 300;
		Green.w = 20;
		Green.h = 20;
		Blue.x = 20;
		Blue.y = 340;
		Blue.w = 20;
		Blue.h = 20;
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderDrawRect(renderer, &bPixel);
		if(pixel)
			SDL_RenderFillRect(renderer, &bPixel);
		SDL_RenderDrawRect(renderer, &bLine);
		if (line)
			SDL_RenderFillRect(renderer, &bLine);
		SDL_RenderDrawRect(renderer, &bBezier);
		if (bezier)
			SDL_RenderFillRect(renderer, &bBezier);
		SDL_RenderDrawRect(renderer, &bCircle);
		if (circle)
			SDL_RenderFillRect(renderer, &bCircle);
		SDL_RenderDrawRect(renderer, &bErase);
		if (erasePixel)
			SDL_RenderFillRect(renderer, &bErase);
		//Colores

		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		SDL_RenderDrawRect(renderer, &Red);
		if(redb)
		SDL_RenderFillRect(renderer, &Red);
		SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
		SDL_RenderDrawRect(renderer, &Green);
		if(greenb)
		SDL_RenderFillRect(renderer, &Green);
		SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
		SDL_RenderDrawRect(renderer, &Blue);
		if(blueb)
		SDL_RenderFillRect(renderer, &Blue);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		//UI
		SDL_RenderDrawLine(renderer, 25, 65, 35, 75);
		SDL_RenderDrawPoint(renderer, 30, 30);
		bezierCurve(xbb, ybb);
		DrawCircle(6, 30, 150);
		SDL_RenderDrawLine(renderer, 25, 185, 25, 195);
		SDL_RenderDrawLine(renderer, 25, 185, 35, 185);
		SDL_RenderDrawLine(renderer, 25, 190, 30, 190);
		SDL_RenderDrawLine(renderer, 25, 195, 35, 195);
		if (redb && !greenb && !blueb)
			SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		if (greenb && !redb && !blueb)
			SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
		if (blueb && !redb && !greenb)
			SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
		if (redb && greenb && !blueb)
			SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
		if (!redb && greenb && blueb)
			SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
		if (redb && !greenb && blueb)
			SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
		SDL_RenderDrawRect(renderer, &ColorRect);
		SDL_RenderFillRect(renderer, &ColorRect);

		if (drawing && pixel)
			SDL_RenderDrawPoint(renderer, xA, yA);
		if(drawing && bezier)
			if (i == 4){
				bezierCurve(xbez, ybez);
				flagDrawn = 1;
			}
		if (drawing && line)
			SDL_RenderDrawLine(renderer, xA, yA, xB, yB);
		if(drawing && circle)
			DrawCircle(xB - xA, xA, yA);

			


		for (std::list<Line>::iterator i = lines.begin(); i != lines.end(); ++i)
		{
			Line line = *i;
			SDL_RenderDrawLine(renderer, line.xA, line.yA, line.xB, line.yB);
		}
		for (std::list<Circle>::iterator i = circles.begin(); i != circles.end(); ++i)
		{
			Circle circle = *i;
			DrawCircle(circle.radio, circle.xA,circle.yA);
		}
		for (std::list<Pixel>::iterator i = pixels.begin(); i != pixels.end(); ++i)
		{
			Pixel pixel = *i;
			SDL_RenderDrawPoint(renderer, pixel.xA,pixel.yA);
		}
		if (drawing && erasePixel)
		{
			SDL_SetRenderDrawColor(renderer, 242, 242, 242, 255);
			DrawFilledCircle(xA, yA, xB - xA);
		}
		for (std::list<Erase>::iterator i = erases.begin(); i != erases.end(); ++i)
		{
			Erase erase = *i;
			DrawFilledCircle(erase.xA, erase.yA, erase.radio);
		}



		// render window

		SDL_RenderPresent(renderer);
		
	}

	// cleanup SDL
	SDL_StopTextInput();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}