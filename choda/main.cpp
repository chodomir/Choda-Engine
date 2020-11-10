/*
	Choda-Engine

	A simple OpenGL app, made for fun.


	Backpack model made by Berk Gedik --- https://sketchfab.com/3d-models/survival-guitar-backpack-low-poly-799f8c4511f84fab8c3f12887f7e6b36
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
#include "Model.h"

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

		float dp = 1.0f; // diffuse power
		float ap = 0.0f; // ambient power
		float sp = 1.0f; // specular power

		// construct matrices
		glm::mat4 model(1.0f);
		glm::mat4 view(1.0f);
		glm::mat4 projection(1.0f);
		view = camera.getViewMatrix();
		projection = glm::perspective(glm::radians(45.0f), (float)winWidth / (float)winHeight, 0.1f, 100.0f);
		// set uniform values
		objectShader.use();
		objectShader.setMat4("model", model);
		objectShader.setMat4("view", view);
		objectShader.setMat4("projection", projection);
		// material parameters
		objectShader.setFloat("material.shininess", 32.0f);
		// directional light parameters
		objectShader.setVec3f("dirLight.ambient", ap * 0.2f, ap * 0.2f, ap * 0.2f);
		objectShader.setVec3f("dirLight.diffuse", dp * 0.5f, dp * 0.5f, dp * 0.5f);
		objectShader.setVec3f("dirLight.specular", sp * 1.0f, sp * 1.0f, sp * 1.0f);
		objectShader.setVec3f("dirLight.direction", -0.2f, -1.0f, -0.3f);
		// point light parameters
		objectShader.setVec3f("pointLight[0].ambient", ap * 0.2f, ap * 0.2f, ap * 0.2f);
		objectShader.setVec3f("pointLight[0].diffuse", dp * 0.5f, dp * 0.5f, dp * 0.5f);
		objectShader.setVec3f("pointLight[0].specular", sp * 1.0f, sp * 1.0f, sp * 1.0f);
		glm::vec4 vplp = view * glm::vec4(1.0f);
		objectShader.setVec3f("pointLight[0].position", vplp.x, vplp.y, vplp.z);
		objectShader.setFloat("pointLight[0].constant", 1.0f);
		objectShader.setFloat("pointLight[0].linear", 0.7f);
		objectShader.setFloat("pointLight[0].quadratic", 1.8f);
		// spotlight parameters
		objectShader.setVec3f("spotlight.ambient", ap * 0.0f, ap * 0.0f, ap * 0.0f);
		objectShader.setVec3f("spotlight.diffuse", dp * 1.0f, dp * 1.0f, dp * 1.0f);
		objectShader.setVec3f("spotlight.specular", sp * 1.0f, sp * 1.0f, sp * 1.0f);
		objectShader.setFloat("spotlight.innerCutOff", glm::cos(glm::radians(12.5)));
		objectShader.setFloat("spotlight.outerCutOff", glm::cos(glm::radians(20.5)));
		objectShader.setFloat("spotlight.constant", 1.0f);
		objectShader.setFloat("spotlight.linear", 0.09f);
		objectShader.setFloat("spotlight.quadratic", 0.032f);
		objectShader.setFloat("time", glfwGetTime());
		
		backpackModel->draw(objectShader);

		// change shader program for lights
		lampShader.use();
		lampShader.setMat4("model", model);
		lampShader.setMat4("view", view);
		lampShader.setMat4("projection", projection);
		lamp->draw(lampShader);
	}

	virtual void onWindowLoad() override {
		std::cout << "Window loaded...\n";

		// creating shader programs
		objectShader.vertex("C:\\Users\\Chodomir\\source\\repos\\choda\\choda\\shader.vert").fragment("C:\\Users\\Chodomir\\source\\repos\\choda\\choda\\shader.frag");
		objectShader.link();
		lampShader.vertex("C:\\Users\\Chodomir\\source\\repos\\choda\\choda\\lamp.vert").fragment("C:\\Users\\Chodomir\\source\\repos\\choda\\choda\\lamp.frag");
		lampShader.link();

		lamp = new choda::Sphere();
		lamp->init();

		std::cout << "Loading model...\n";
		backpackModel = new choda::Model("C:\\Users\\Chodomir\\source\\repos\\choda\\choda\\backpack\\backpack.obj");
		
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	}

	virtual void onWindowClose() override {
		std::cout << "Window closed...\n";

		delete backpackModel;
		delete lamp;
	}
private:
	choda::ShaderProgram objectShader, lampShader;
	choda::Camera camera;
	choda::Model *backpackModel;
	choda::Sphere *lamp;

private:
	float lastX, lastY;
	// firstMouse = true initially
	bool firstMouse; 
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