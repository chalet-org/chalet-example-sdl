#include "Application.hpp"

#include "Libraries/Emscripten.hpp"
#include "Platform/Platform.hpp"

namespace sdl
{
/*****************************************************************************/
bool Application::initialize(const ApplicationSettings& settings)
{
	if (!SDL_Init(SDL_INIT_VIDEO))
	{
		std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << '\n';
		return 1;
	}

	m_dimensions.x = settings.width;
	m_dimensions.y = settings.height;

	m_window = SDL_CreateWindow(settings.title.c_str(), m_dimensions.x, m_dimensions.y, SDL_WINDOW_HIDDEN);
	if (m_window == nullptr)
	{
		std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << '\n';
		dispose();
		return 1;
	}

	Platform::initialize(m_window);

	auto renderProps = SDL_CreateProperties();
	// SDL_SetStringProperty(renderProps, SDL_PROP_RENDERER_CREATE_NAME_STRING, "vulkan");
	SDL_SetPointerProperty(renderProps, SDL_PROP_RENDERER_CREATE_WINDOW_POINTER, m_window);
	SDL_SetNumberProperty(renderProps, SDL_PROP_RENDERER_CREATE_PRESENT_VSYNC_NUMBER, 1);
	SDL_SetNumberProperty(renderProps, SDL_PROP_RENDERER_CREATE_OUTPUT_COLORSPACE_NUMBER, SDL_COLORSPACE_SRGB);
	m_renderer = SDL_CreateRendererWithProperties(renderProps);
	if (m_renderer == nullptr)
	{
		std::cout << "Renderer could not be created! SDL_Error: " << SDL_GetError() << '\n';
		dispose();
		return 1;
	}

	clearRenderer();
	presentRenderer();
	showWindow();

	return true;
}

/*****************************************************************************/
void Application::dispose()
{
	if (m_renderer != nullptr)
	{
		SDL_DestroyRenderer(m_renderer);
		m_renderer = nullptr;
	}

	if (m_window != nullptr)
	{
		SDL_DestroyWindow(m_window);
		m_window = nullptr;
	}

	SDL_Quit();
	view.m_application.reset();
}

/*****************************************************************************/
void Application::runMainLoop()
{
	if (!isRunning())
	{
		dispose();
#if defined(APP_EMSCRIPTEN)
		emscripten_cancel_main_loop();
#else
		::exit(0);
#endif
	}

	clearRenderer();
	if (onRender != nullptr)
		onRender();

	presentRenderer();
}

/*****************************************************************************/
bool Application::isRunning()
{
	while (SDL_PollEvent(&m_ev) != 0)
	{
		if (m_ev.type == SDL_EVENT_QUIT)
			return false;

		// Maybe manage other events outside of this function
	}

	return true;
}

/*****************************************************************************/
void Application::setClearColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	m_clearColor = SDL_Color { r, g, b, a };
}

/*****************************************************************************/
Geometry Application::getDemoGeometry()
{
	Geometry result;

	auto screenWidth = static_cast<float>(m_dimensions.x);
	auto screenHeight = static_cast<float>(m_dimensions.y);

	SDL_FPoint padding;
	padding.x = 128;
	padding.y = 64;

	result.vert[0].position.x = 0 + padding.x;
	result.vert[0].position.y = screenHeight - padding.y;
	result.vert[0].color = { 0.0f, 0.0f, 1.0f, 1.0f };

	result.vert[1].position.x = screenWidth - padding.x;
	result.vert[1].position.y = screenHeight - padding.y;
	result.vert[1].color = { 0.0f, 1.0f, 0.0f, 1.0f };

	result.vert[2].position.x = screenWidth - padding.x;
	result.vert[2].position.y = 0 + padding.y;
	result.vert[2].color = { 0.0f, 0.0f, 0.0f, 1.0f };

	result.vert[3].position.x = 0 + padding.x;
	result.vert[3].position.y = 0 + padding.y;
	result.vert[3].color = { 1.0f, 0.0f, 0.0f, 1.0f };

	result.indices[0] = 0;
	result.indices[1] = 1;
	result.indices[2] = 3;

	result.indices[3] = 3;
	result.indices[4] = 1;
	result.indices[5] = 2;

	return result;
}

/*****************************************************************************/
void Application::clearRenderer()
{
	SDL_SetRenderDrawColor(m_renderer, m_clearColor.r, m_clearColor.g, m_clearColor.b, m_clearColor.a);
	SDL_RenderClear(m_renderer);
}

/*****************************************************************************/
void Application::presentRenderer()
{
	SDL_RenderPresent(m_renderer);
}

/*****************************************************************************/
void Application::showWindow()
{
	SDL_ShowWindow(m_window);
}

/*****************************************************************************/
bool Application::render(Geometry& inGeometry)
{
	auto& vertices = inGeometry.vert;
	auto& indices = inGeometry.indices;

	int result = SDL_RenderGeometry(m_renderer,
		nullptr, // texture
		vertices.data(),
		static_cast<int>(vertices.size()),
		indices.data(),
		static_cast<int>(indices.size()));

	return result < 0;
}

/*****************************************************************************/
ApplicationView::ApplicationView() :
	m_application(std::make_unique<Application>())
{}

/*****************************************************************************/
Application& ApplicationView::get()
{
	return *m_application;
}

ApplicationView view = ApplicationView();
}