#pragma once

#include <functional>

namespace sdl
{
struct ApplicationSettings
{
	std::string title = "Untitled";
	int width = 640;
	int height = 480;
};

struct Geometry
{
	std::array<SDL_Vertex, 4> vert;
	std::array<int, 6> indices;
};

class Application
{
public:
	Application() = default;

	bool initialize(const ApplicationSettings& settings);
	void dispose();
	void runMainLoop();

	void setClearColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255);

	Geometry getDemoGeometry();

	bool render(Geometry& inGeometry);

	std::function<void()> onRender;

private:
	void clearRenderer();
	void presentRenderer();
	void showWindow();

	bool isRunning();

	SDL_Window* m_window = nullptr;
	SDL_Renderer* m_renderer = nullptr;

	SDL_Event m_ev;

	SDL_Point m_dimensions;
	SDL_Color m_clearColor;
};

struct ApplicationView
{
	ApplicationView();

	Application& get();

private:
	friend class Application;

	std::unique_ptr<Application> m_application;

};

extern ApplicationView view;
}