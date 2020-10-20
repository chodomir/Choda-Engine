#include "Engine.h"

#include <stdexcept>
#include <iostream>

using namespace choda;

float lastFrameTime = 0.0f;

Engine::Engine(bool captureMouse) : winWidth(800), winHeight(600), winTitle("My Title"),
window(NULL), dt(0.0f)
{
	init(captureMouse);
}

void Engine::onKeyPressed(int key) {
	if (key == GLFW_KEY_ESCAPE)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void Engine::onMouseMove(double xpos, double ypos) { }

// initializes GLFW and OpenGL function pointers
void Engine::init(bool captureMouse) {
	if (!glfwInit())
		throw std::runtime_error("Failed to load GLFW");

	glfwDefaultWindowHints();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(winWidth, winHeight, winTitle, NULL, NULL);
	if (!window)
		throw std::runtime_error("Failed to load GLFW window");

	glfwMakeContextCurrent(window);
	if (captureMouse) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		throw std::runtime_error("Failed to load GLAD");

	// window object will now see methods of Engine class
	glfwSetWindowUserPointer(window, this);
	// set event callbacks
	glViewport(0, 0, winWidth, winHeight);
	glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height) {
		glViewport(0, 0, width, height);
		});
	glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
		// GLFW is a C API and this is a way to make it more OO friendly
		if (action == GLFW_PRESS) {
			static_cast<Engine*>(glfwGetWindowUserPointer(window))->onKeyPressed(key);
		}
		});
	glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xpos, double ypos) {
		static_cast<Engine*>(glfwGetWindowUserPointer(window))->onMouseMove(xpos, ypos);
		});

}


Engine::~Engine() {
	glfwDestroyWindow(window);
	glfwTerminate();
}


void Engine::run() {
	onWindowLoad();

	while (!glfwWindowShouldClose(window)) {
		// update delta time value
		float currentFrameTime = glfwGetTime();
		dt = currentFrameTime - lastFrameTime;
		lastFrameTime = currentFrameTime;
		// continue rendering and managing events
		render(glfwGetTime());

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	onWindowClose();
}