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
		// proccess keyboard input for camera movement
		processInput();
		
		// MVP matrices
		glm::mat4 model(1.0f);
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)winWidth / winHeight, 0.1f, 100.0f);
		glm::mat4 view = camera.getViewMatrix(); // get view matrix of the translated camera

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glEnable(GL_DEPTH_TEST);

		// render the scene to a custom framebuffer (mirror scene)
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBindTexture(GL_TEXTURE_2D, tex1);
		shader.use();
		shader.setMat4("view", view);
		shader.setMat4("projection", projection);
		for (int i = 0; i < meshes.size(); i++) {
			model = glm::translate(glm::mat4(1.0f), positions[i]);
			shader.setMat4("model", model);
			meshes[i].draw(shader);
		}

		// render the scene to window (default framebuffer)
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		for (int i = 0; i < meshes.size(); i++) {
			model = glm::translate(glm::mat4(1.0f), positions[i]);
			shader.setMat4("model", model);
			meshes[i].draw(shader);
		}

		// draw the mirror
		glBindTexture(GL_TEXTURE_2D, tex2);
		fboShader.use();
		fxQuad.draw(fboShader);
	}

	virtual void onWindowLoad() override {
		std::cout << "Window loaded...\n";

		// generate the texture
		std::string texDirPath = choda::FileSystem::GetAbsolutePath("resources\\image\\");
		std::string containerPath = texDirPath + "container2.png";
		std::string windowPath = texDirPath + "window.png";
		tex1 = choda::Texture::GenerateFromFile(containerPath.c_str());

		// creating shader programs
		shader.vertex("shader/texture.vert").fragment("shader/texture.frag").link();
		fboShader.vertex("shader/fboShader.vert").fragment("shader/fboShader.frag").link();

		// populate meshes array
		meshes.push_back(choda::Cube());
		meshes.push_back(choda::Cube());
		meshes.push_back(choda::Cube());
		// initialize cube positions
		positions.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
		positions.push_back(glm::vec3(3.0f, 0.0f, -2.0f));
		positions.push_back(glm::vec3(1.0f, 0.0f, 7.0f));

		// initialize meshes
		for (auto& mesh : meshes)
			mesh.init();
		
		// FRAMEBUFFER CODE
		fxQuad.init();

		// create framebuffer object
		glGenFramebuffers(1, &fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);

		// create a texture for a framebuffer
		glGenTextures(1, &tex2);
		glBindTexture(GL_TEXTURE_2D, tex2);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, winWidth, winHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);
		// attach the texture to framebuffer
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex2, 0);

		// create renderbuffer object for depth and stencil test
		glGenRenderbuffers(1, &rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, winWidth, winHeight);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		// attach renderbuffer to framebuffer
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
		
		// check if everything is okay
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			throw std::runtime_error("Framebuffer is not complete!");
		// if ok, unbind the framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	virtual void onWindowClose() override {
		std::cout << "Window closed...\n";

		glDeleteTextures(1, &tex1);
		glDeleteTextures(1, &tex2);
		glDeleteFramebuffers(1, &fbo);
		glDeleteRenderbuffers(1, &rbo);
	}
private:
	choda::ShaderProgram shader, fboShader;
	choda::Camera camera;
	choda::Quad fxQuad;
	std::vector<choda::Mesh> meshes;
	std::vector<glm::vec3> positions;
	GLuint tex1, tex2;

	// FRAMEBUFFER CODE
	GLuint fbo, rbo;

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