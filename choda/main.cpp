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
#include "Quad.h"
#include "FileSystem.h"

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

	void drawObject(choda::Mesh& mesh, choda::ShaderProgram& shader, glm::mat4& model) {
		glm::mat4 view = camera.getViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)winWidth / winHeight, 0.1f, 100.0f);

		shader.use();
		shader.setMat4("model", model);
		shader.setMat4("view", view);
		shader.setMat4("projection", projection);
		mesh.draw(shader);
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
		// clear buffers
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// proccess keyboard input for camera movement
		processInput();

		// set texture
		glBindTexture(GL_TEXTURE_2D, tex1);
		for (int i = 0; i < meshes.size(); i++) {
			if (i == 2) glBindTexture(GL_TEXTURE_2D, tex2);
			glm::mat4 model = glm::translate(glm::mat4(1.0f), positions[i]);
			drawObject(meshes[i], shader, model);
		}

	}

	virtual void onWindowLoad() override {
		std::cout << "Window loaded...\n";

		// generate the texture
		std::string texDirPath = choda::FileSystem::GetAbsolutePath("resources\\image\\");
		std::string containerPath = texDirPath + "container2.png";
		std::string windowPath = texDirPath + "window.png";
		tex1 = choda::Texture::GenerateFromFile(containerPath.c_str());
		tex2 = choda::Texture::GenerateFromFile(windowPath.c_str());

		// creating shader programs
		shader.vertex("shader/texture.vert").fragment("shader/texture.frag").link();
		singleColorShader.vertex("shader/simpleColor.vert").fragment("shader/simpleColor.frag").link();

		// populate meshes array
		meshes.push_back(choda::Cube());
		meshes.push_back(choda::Cube());
		meshes.push_back(choda::Quad());
		meshes.push_back(choda::Quad());
		meshes.push_back(choda::Quad());
		// initialize cube positions
		positions.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
		positions.push_back(glm::vec3(3.0f, 0.0f, -2.0f));
		positions.push_back(glm::vec3(0.0f, 0.0f, 1.01f));
		positions.push_back(glm::vec3(5.0f, 1.0f, 3.0f));
		positions.push_back(glm::vec3(4.0f, 1.0f, -0.99f));
		// initialize meshes
		for (auto& mesh : meshes)
			mesh.init();

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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
	std::vector<choda::Mesh> meshes;
	std::vector<glm::vec3> positions;
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