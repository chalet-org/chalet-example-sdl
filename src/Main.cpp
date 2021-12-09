#include <iostream>

int main(const int argc, const char* const argv[])
{
	std::cout << "Hello world!\n\n";
	std::cout << "Args:\n";

	for (int i = 0; i < argc; ++i)
	{
		std::cout << "  " << argv[i] << '\n';
	}

	std::cout.flush();

	// The window we'll be rendering to
	SDL_Window* window = nullptr;

	// The surface contained by the window
	SDL_Surface* screenSurface = nullptr;

	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << "\n";
	}
	else
	{
		constexpr int screenWidth = 640;
		constexpr int screenHeight = 480;

		// Create window
		window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
		if (window == nullptr)
		{
			std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << "\n";
		}
		else
		{
			bool quit = false;

			// Get window surface
			screenSurface = SDL_GetWindowSurface(window);

			//Event handler
			SDL_Event ev;
			while (!quit)
			{
				while (SDL_PollEvent(&ev) != 0)
				{
					//User requests quit
					if (ev.type == SDL_QUIT)
					{
						quit = true;
					}
				}

				// Fill the surface
				SDL_FillRect(screenSurface, nullptr, SDL_MapRGB(screenSurface->format, 0x22, 0x22, 0x55));

				// Update the surface
				SDL_UpdateWindowSurface(window);
			}
		}
	}

	// Destroy window
	SDL_DestroyWindow(window);

	// Quit SDL subsystems
	SDL_Quit();

	return 0;
}
