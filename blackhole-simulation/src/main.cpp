#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

int main() {
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 800, "grav-sim", NULL, NULL);
	if (window == NULL) {
		std::cout << "window formation failed" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	//Loading colors
	gladLoadGL();



	//Informing openGL on the viewport of our window
	glViewport(0, 0, 800, 800);  //bottom left to top right

	//glfunction to clear the current buffer and replace with a desired color
	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);

	//We then want to execute this color change onto our buffer, and do so by clearing the color buffer bit
	glClear(GL_COLOR_BUFFER_BIT);

	//We now swap whatever we've been doing on the background
	glfwSwapBuffers(window);

	while (!glfwWindowShouldClose(window)) {

		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}