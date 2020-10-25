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
	MyApp() : choda::Engine(), camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f)), firstMouse(true), lampPos(3.5f, 0.0f, 0.0f) { }
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
		std::vector<float> lampColor = { 1.0f, 1.0f, 1.0f };
		std::vector<float> objectColor = { 1.0f, 0.5f, 0.31f };

		// rotate lamp
		float theta = glfwGetTime();
		lampPos = 2.5f * glm::vec3(std::cosf(1.2f * theta), 0.0f, std::sinf(1.2f * theta));

		view = camera.getViewMatrix();
		projection = glm::perspective(glm::radians(45.0f), (float)winWidth / (float)winHeight, 0.1f, 100.0f);

		model = glm::scale(model, glm::vec3(0.5f));
		//model = glm::rotate(model, theta, glm::vec3(1.0f, 0.0f, 1.0f));
		objectShader.use();
		objectShader.setMat4("model", model);
		objectShader.setMat4("view", view);
		objectShader.setMat4("projection", projection);
		objectShader.setVec3f("objectColor", objectColor);
		objectShader.setVec3f("lampColor", lampColor);
		objectShader.setVec3f("lightPos", { lampPos.x, lampPos.y, lampPos.z });
		objectShader.setVec3f("viewPos", { camera.getPosition().x, camera.getPosition().y, camera.getPosition().z });
		object->draw();
		
		model = glm::mat4(1.0f);
		model = glm::translate(model, lampPos);
		model = glm::scale(model, glm::vec3(0.1f));
		lampShader.use();
		lampShader.setMat4("model", model);
		lampShader.setMat4("view", view);
		lampShader.setMat4("projection", projection);
		lampShader.setVec3f("lampColor", lampColor);
		lamp->draw();

	}

	virtual void onWindowLoad() override {
		std::cout << "Window loaded...\n";

		objectShader.vertex("shader.vert").fragment("shader.frag");
		objectShader.link();
		lampShader.vertex("lamp.vert").fragment("lamp.frag");
		lampShader.link();
		
		lamp = new choda::Sphere();
		object = new choda::Sphere();
		lamp->init();
		object->init();

		glEnable(GL_DEPTH_TEST);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	}

	virtual void onWindowClose() override {
		std::cout << "Window closed...\n";

		delete lamp;
		delete object;
	}
private:
	choda::ShaderProgram objectShader, lampShader;
	choda::Camera camera;
	choda::Sphere *lamp;
	choda::Mesh *object;
	glm::vec3 lampPos;

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