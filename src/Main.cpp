#include <iostream>

#include "Platform/Platform.hpp"

int main(const int argc, const char* const argv[])
{
#if defined(_DEBUG)
	std::cout << "Hello world!\n\n";
	std::cout << "Args:\n";

	for (int i = 0; i < argc; ++i)
	{
		std::cout << "  " << argv[i] << '\n';
	}
#else
	UNUSED(argc);
	UNUSED(argv);
#endif

	std::cout.flush();

	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << '\n';
		return 1;
	}

	constexpr int screenWidth = 640;
	constexpr int screenHeight = 480;

	window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, SDL_WINDOW_HIDDEN);
	if (window == nullptr)
	{
		std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << '\n';
		SDL_Quit();
		return 1;
	}

	Platform::initialize(window);

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == nullptr)
	{
		std::cout << "Renderer could not be created! SDL_Error: " << SDL_GetError() << '\n';
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}

	constexpr int vertSize = 4;
	SDL_Vertex vert[vertSize];
	int indices[6];

	SDL_FPoint padding;
	padding.x = 128;
	padding.y = 64;

	vert[0].position.x = 0 + padding.x;
	vert[0].position.y = screenHeight - padding.y;
	vert[0].color = { 0, 0, 255, 255 };

	vert[1].position.x = screenWidth - padding.x;
	vert[1].position.y = screenHeight - padding.y;
	vert[1].color = { 0, 255, 0, 255 };

	vert[2].position.x = screenWidth - padding.x;
	vert[2].position.y = 0 + padding.y;
	vert[2].color = { 0, 0, 0, 255 };

	vert[3].position.x = 0 + padding.x;
	vert[3].position.y = 0 + padding.y;
	vert[3].color = { 255, 0, 0, 255 };

	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 3;

	indices[3] = 3;
	indices[4] = 1;
	indices[5] = 2;

	SDL_Color clearColor{ 100, 149, 237, 255 };

	SDL_SetRenderDrawColor(renderer, clearColor.r, clearColor.g, clearColor.b, clearColor.a);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);

	SDL_ShowWindow(window);

	SDL_Event ev;
	bool quit = false;
	while (!quit)
	{
		while (SDL_PollEvent(&ev) != 0)
		{
			if (ev.type == SDL_QUIT)
			{
				quit = true;
				break;
			}
		}

		SDL_SetRenderDrawColor(renderer, clearColor.r, clearColor.g, clearColor.b, clearColor.a);
		SDL_RenderClear(renderer);

		// Draw stuff
		SDL_RenderGeometry(renderer, nullptr, vert, vertSize, indices, 6);

		SDL_RenderPresent(renderer);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
