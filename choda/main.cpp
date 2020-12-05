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
#include "Plane.h"
#include "FileSystem.h"

class MyApp : public choda::Engine {
public:
	MyApp() : choda::Engine(), camera(glm::vec3(0.0f, 1.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f)), firstMouse(true) { }
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

	void drawFloor() {

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

	virtual void onKeyPressed(int key) {
		if (key == GLFW_KEY_ESCAPE) {
			std::cout << "Closing...";
			glfwSetWindowShouldClose(window, GLFW_TRUE);
		}
	}

	virtual void render(double dt) override {
		// testing
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

		// proccess keyboard input for camera movement
		processInput();

		float boxSize = 1.0f;
		glm::vec3 pos1(2.0f, 0.51f * boxSize, 0.0f);
		glm::vec3 pos2(-1.2f, 0.51f * boxSize, 2.0f);
		glm::mat4 model = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f));
		glm::mat4 view = camera.getViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)winWidth / winHeight, 0.1f, 100.0f);

		shader.use();
		// disable writing to stencil buffer
		glStencilMask(0x00);
		// Draw plane
		shader.setMat4("model", model);
		shader.setMat4("view", view);
		shader.setMat4("projection", projection);
		glBindTexture(GL_TEXTURE_2D, tex1);
		plane.draw(shader);

		// enable writing to stencil buffer and set stencil test function
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF);
		// draw cubes
		model = glm::translate(glm::mat4(1.0f), pos1);
		shader.setMat4("model", model);
		glBindTexture(GL_TEXTURE_2D, tex2);
		cube1.draw(shader);
		model = glm::translate(glm::mat4(1.0f), pos2);
		shader.setMat4("model", model);
		cube2.draw(shader);

		// TODO: UNDERSTAND THIS REALLY REALLY GOOD
		// change shader program
		singleColorShader.use();
		singleColorShader.setMat4("view", view);
		singleColorShader.setMat4("projection", projection);
		// draw scaled cubes (outline)
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilMask(0x00);
		glDisable(GL_DEPTH_TEST);
		// scale up the containers
		model = glm::scale(model, glm::vec3(1.05f));
		singleColorShader.setMat4("model", model);
		cube2.draw(singleColorShader);
		model = glm::translate(glm::mat4(1.0f), pos1);
		model = glm::scale(model, glm::vec3(1.05f));
		singleColorShader.setMat4("model", model);
		cube1.draw(singleColorShader);
		glStencilMask(0xFF);
		//glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glEnable(GL_DEPTH_TEST);

	}

	virtual void onWindowLoad() override {
		std::cout << "Window loaded...\n";

		// creating shader programs
		shader.vertex("shader/texture.vert").fragment("shader/texture.frag").link();
		singleColorShader.vertex("shader/simpleColor.vert").fragment("shader/simpleColor.frag").link();

		cube1.init();
		cube2.init();
		plane.init();

		tex1 = choda::Texture::GenerateFromFile(choda::FileSystem::GetAbsolutePath("resources\\image\\floor.png").c_str());
		tex2 = choda::Texture::GenerateFromFile(choda::FileSystem::GetAbsolutePath("resources\\image\\container2.png").c_str());

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_STENCIL_TEST);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	}

	virtual void onWindowClose() override {
		std::cout << "Window closed...\n";

		glDeleteTextures(1, &tex1);
		glDeleteTextures(1, &tex2);
	}
private:
	choda::ShaderProgram shader, singleColorShader;
	choda::Camera camera;
	choda::Cube cube1, cube2;
	choda::Plane plane;
	GLuint tex1, tex2;

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