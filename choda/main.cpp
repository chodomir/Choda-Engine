/*
	Choda-Engine

	A simple OpenGL app, made for fun.
*/
#include <iostream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


#include "Engine.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "Camera.h"
#include "Sphere.h"
#include "Cube.h"

class MyApp : public choda::Engine {
public:
	MyApp() : choda::Engine(), camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f)), firstMouse(true) { }
public:
	void processInput() {
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			camera.move(choda::Camera::Direction::Forward, dt);
		} if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			camera.move(choda::Camera::Direction::Left, dt);
		} if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			camera.move(choda::Camera::Direction::Backward, dt);
		} if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			camera.move(choda::Camera::Direction::Right, dt);
		}
	}

public:
	virtual void onMouseMove(double xpos, double ypos) override {
		if (firstMouse) {
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}

		float xOffset = camera.getSensitivity() * (xpos - lastX);
		float yOffset = camera.getSensitivity() * (lastY - ypos);
		lastX = xpos;
		lastY = ypos;

		camera.rotate(yOffset, xOffset);
	}

	virtual void render(double dt) override {

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		processInput();

		glm::mat4 model(1.0f);
		glm::mat4 view(1.0f);
		glm::mat4 projection(1.0f);
		view = camera.getViewMatrix();
		projection = glm::perspective(glm::radians(45.0f), (float)winWidth / (float)winHeight, 0.1f, 100.0f);
		program.setMat4("view", view);
		program.setMat4("projection", projection);

		for (int i = 0; i < 10; i++) {
			model = glm::translate(model, positions[i]);
			program.setMat4("model", model);
			shapes[i]->draw();
			
			model = glm::mat4(1.0f);
		}
	}

	virtual void onWindowLoad() override {
		std::cout << "Window loaded...\n";
		
		float pi = 3.141592f;
		float radius = 5.0f;
		for (int i = 0; i < 10; i++) {
			float theta = i * 2 * pi / 10;
			if (i % 2 == 0)
				shapes.push_back(new choda::Cube(2.0f));
			else
				shapes.push_back(new choda::Sphere(1.0f, 18, 36));

			shapes.back()->init();
			
			positions.push_back(glm::vec3(radius * std::sinf(theta), 1.0f, radius * std::cosf(theta)));
		}

		program.vertex("shader.vert").fragment("shader.frag");
		program.link();
		program.use();

		glEnable(GL_DEPTH_TEST);
		glClearColor(0.1f, 0.1f, 0.7f, 1.0f);
	}

	virtual void onWindowClose() override {
		std::cout << "Window closed...\n";

		for (int i = 0; i < 10; i++)
			delete shapes[i];
	}
private:
	choda::ShaderProgram program;
	choda::Camera camera;
	std::vector<choda::Mesh*> shapes;
	std::vector<glm::vec3> positions;

	float lastX, lastY;
	bool firstMouse; // initially set to true
};

int main() {
	try {
		MyApp app;
		app.run();
	}
	catch (std::runtime_error& e) {
		std::cerr << "[Runtime Error]: " << e.what() << '\n';
	}

	return 0;
}