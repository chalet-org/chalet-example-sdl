#include <iostream>

#include "Application.hpp"
#include "Libraries/Emscripten.hpp"

void mainLoop()
{
	sdl::view.get().runMainLoop();
}

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

	// Note: The app is structured this way to play nicely with Emscripten

	auto& app = sdl::view.get();

	app.setClearColor(100, 149, 237);

	sdl::ApplicationSettings settings;
	settings.title = "SDL Demo";
	settings.width = 640;
	settings.height = 480;

	if (!app.initialize(settings))
		return 1;

	auto demoGeometry = app.getDemoGeometry();
	app.onRender = [&app, &demoGeometry]() {
		app.render(demoGeometry);
	};


#if defined(APP_EMSCRIPTEN)
    emscripten_set_main_loop(mainLoop, 0, 1);
#else
    while (1) {
		mainLoop();
	}
#endif
}
