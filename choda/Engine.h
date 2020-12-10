#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace choda {

	class Engine
	{
	public:
		Engine(bool captureMouse = true);
		~Engine();
	public:
		void run();
		virtual void onKeyPressed(int key);
		virtual void onMouseMove(double xpos, double ypos);
	protected:
		virtual void render(double dt) = 0;
		virtual void onWindowLoad() = 0;
		virtual void onWindowClose() = 0;
	private:
		void init(bool captureMouse);

	// member variables
	public:
		int winWidth, winHeight;
		const char* winTitle;
	protected:
		GLFWwindow* window;
		float dt;
	};

}

