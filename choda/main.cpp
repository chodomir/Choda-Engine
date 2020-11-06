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
		objectShader.setMat4("view", view);
		objectShader.setMat4("projection", projection);
		// material parameters
		objectShader.setInt("material.diffuse", container->getId());
		objectShader.setInt("material.specular", containerSpecular->getId());
		objectShader.setInt("material.emmision", emmisionMap->getId());
		objectShader.setFloat("material.shininess", 32.0f);
		// directional light parameters
		objectShader.setVec3f("dirLight.ambient", ap * 0.0f, ap * 0.0f, ap * 0.0f);
		objectShader.setVec3f("dirLight.diffuse", dp * 0.0f, dp * 0.0f, dp * 0.0f);
		objectShader.setVec3f("dirLight.specular", sp * 0.0f, sp * 0.0f, sp * 0.0f);
		objectShader.setVec3f("dirLight.direction", -0.2f, -1.0f, -0.3f);
		// point light parameters
		for (int i = 0; i < lightPositions.size(); i++) {
			std::string pos = std::to_string(i);
			glm::vec3 vplp = view * glm::vec4(lightPositions[i], 1.0f); // quick fix
			
			float r = lampColors[i].x;
			float g = lampColors[i].y;
			float b = lampColors[i].z;

			objectShader.setVec3f(("pointLight[" + pos + "].ambient").c_str(), ap * r, ap * g, ap * b);
			objectShader.setVec3f(("pointLight[" + pos + "].diffuse").c_str(), dp * r, dp * g, dp * b);
			objectShader.setVec3f(("pointLight[" + pos + "].specular").c_str(), sp * 1.0f, sp * 1.0f, sp * 1.0f);
			objectShader.setVec3f(("pointLight[" + pos + "].position").c_str(), vplp.x, vplp.y, vplp.z);
			objectShader.setFloat(("pointLight[" + pos + "].constant").c_str(), 1.0f);
			objectShader.setFloat(("pointLight[" + pos + "].linear").c_str(), 0.7f);
			objectShader.setFloat(("pointLight[" + pos + "].quadratic").c_str(), 1.8f);
		}
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

		// set position of every object & draw them
		for (int i = 0; i < objects.size(); i++) {
			model = glm::mat4(1.0f);
			model = glm::translate(model, positions[i]);
			model = glm::rotate(model, 20.0f * i, glm::vec3(1.0f, 1.0f, 1.0f));

			objectShader.setMat4("model", model);
			objects[i]->draw();
		}

		// change shader program for lights
		lampShader.use();
		lampShader.setMat4("view", view);
		lampShader.setMat4("projection", projection);
		// position for point lights
		for (int i = 0; i < lightPositions.size(); i++) {
			model = glm::mat4(1.0f);
			model = glm::translate(model, lightPositions[i]);
			model = glm::scale(model, glm::vec3(0.4f));

			lampShader.setVec3f("lampColor", lampColors[i]);
			lampShader.setMat4("model", model);
			lamps[i]->draw();
		}

	}

	virtual void onWindowLoad() override {
		std::cout << "Window loaded...\n";

		srand(1);

		// creating shader programs
		objectShader.vertex("C:\\Users\\Chodomir\\source\\repos\\choda\\choda\\shader.vert").fragment("C:\\Users\\Chodomir\\source\\repos\\choda\\choda\\shader.frag");
		objectShader.link();
		lampShader.vertex("C:\\Users\\Chodomir\\source\\repos\\choda\\choda\\lamp.vert").fragment("C:\\Users\\Chodomir\\source\\repos\\choda\\choda\\lamp.frag");
		lampShader.link();
		
		// loading textures
		container = new choda::Texture("C:\\Users\\Chodomir\\source\\repos\\choda\\choda\\container2.png", true);
		containerSpecular = new choda::Texture("C:\\Users\\Chodomir\\source\\repos\\choda\\choda\\container2_specular.png", true);
		emmisionMap = new choda::Texture("C:\\Users\\Chodomir\\source\\repos\\choda\\choda\\matrix.jpg", false);

		// instancing objects
		for (int i = 0; i < 10; i++) {
			if (i % 2 == 0)
				objects.push_back(new choda::Cube());
			else
				objects.push_back(new choda::Sphere());

			objects.back()->init();

			int radius = 5;
			float x = rand() % (2 * radius) - radius;
			float y = rand() % (2 * radius) - radius;
			float z = rand() % (4 * radius) - 2 * radius;
			positions.push_back(glm::vec3(x, y, z));
		}

		lampColors.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
		lampColors.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
		lampColors.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
		lampColors.push_back(glm::vec3(1.0f, 1.0f, 0.0f));
		lightPositions.push_back(glm::vec3(0.7f, 0.2f, 2.0f));
		lightPositions.push_back(glm::vec3(2.3f, -3.3f, -4.0f));
		lightPositions.push_back(glm::vec3(-4.0f, 2.0f, -12.0f));
		lightPositions.push_back(glm::vec3(0.0f, 0.0f, -3.0f));
		for (int i = 0; i < lightPositions.size(); i++) {
			lamps.push_back(new choda::Cube());
			lamps.back()->init();
		}

		// binding & activating textures
		container->activate();
		containerSpecular->activate();
		emmisionMap->activate();

		glEnable(GL_DEPTH_TEST);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	}

	virtual void onWindowClose() override {
		std::cout << "Window closed...\n";

		delete container;
		delete containerSpecular;
		delete emmisionMap;

		for (int i = 0; i < lamps.size(); i++) {
			delete lamps[i];
		}

		for (int i = 0; i < objects.size(); i++) {
			delete objects[i];
		}
	}
private:
	choda::ShaderProgram objectShader, lampShader;
	choda::Camera camera;
	std::vector<choda::Cube*> lamps;
	std::vector<glm::vec3> lampColors;
	std::vector<choda::Mesh*> objects;
	choda::Texture* container, * containerSpecular, * emmisionMap;
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> lightPositions;

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