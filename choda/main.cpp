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


		glm::mat4 view(1.0f);
		glm::mat4 projection(1.0f);
		std::vector<float> lampColor = { 1.0f, 1.0f, 1.0f };

		// rotate lamp
		float theta = glfwGetTime();
		lampPos = 2.5f * glm::vec3(std::cosf(1.2f * theta), 0.0f, std::sinf(1.2f * theta));

		view = camera.getViewMatrix();
		projection = glm::perspective(glm::radians(45.0f), (float)winWidth / (float)winHeight, 0.1f, 100.0f);

		glm::mat4 objectModel(1.0f);
		objectModel = glm::scale(objectModel, glm::vec3(1.5f));
		
		glm::mat4 lampModel(1.0f);
		lampModel = glm::mat4(1.0f);
		lampModel = glm::translate(lampModel, lampPos);
		lampModel = glm::scale(lampModel, glm::vec3(0.1f));

		objectShader.use();
		objectShader.setMat4("model", objectModel);
		objectShader.setMat4("view", view);
		objectShader.setMat4("projection", projection);
		objectShader.setInt("material.diffuse", container->getId());
		objectShader.setInt("material.specular", containerSpecular->getId());
		objectShader.setFloat("material.shininess", 32.0f);
		objectShader.setVec3f("light.ambient", 0.2f, 0.2f, 0.2f);
		objectShader.setVec3f("light.diffuse", 0.5f, 0.5f, 0.5f);
		objectShader.setVec3f("light.specular", 1.0f, 1.0f, 1.0f);
		objectShader.setVec3f("lightColor", lampColor);
		objectShader.setVec3f("lightPos", lampPos);
		object->draw();
		

		lampShader.use();
		lampShader.setMat4("model", lampModel);
		lampShader.setMat4("view", view);
		lampShader.setMat4("projection", projection);
		lampShader.setVec3f("lampColor", lampColor);
		lamp->draw();

	}

	virtual void onWindowLoad() override {
		std::cout << "Window loaded...\n";

		objectShader.vertex("C:\\Users\\Chodomir\\source\\repos\\choda\\choda\\shader.vert").fragment("C:\\Users\\Chodomir\\source\\repos\\choda\\choda\\shader.frag");
		objectShader.link();
		lampShader.vertex("C:\\Users\\Chodomir\\source\\repos\\choda\\choda\\lamp.vert").fragment("C:\\Users\\Chodomir\\source\\repos\\choda\\choda\\lamp.frag");
		lampShader.link();
		
		container = new choda::Texture("C:\\Users\\Chodomir\\source\\repos\\choda\\choda\\container2.png", true);
		containerSpecular = new choda::Texture("C:\\Users\\Chodomir\\source\\repos\\choda\\choda\\container2_specular.png", true);
		lamp = new choda::Sphere();
		object = new choda::Cube();
		lamp->init();
		object->init();
		container->activate();
		containerSpecular->activate();

		glEnable(GL_DEPTH_TEST);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	}

	virtual void onWindowClose() override {
		std::cout << "Window closed...\n";

		delete container;
		delete lamp;
		delete object;
	}
private:
	choda::ShaderProgram objectShader, lampShader;
	choda::Camera camera;
	choda::Sphere* lamp;
	choda::Mesh* object;
	choda::Texture* container, * containerSpecular;
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