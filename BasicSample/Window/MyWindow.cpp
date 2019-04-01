#include "../Source/Std.h"
#include "MyWindow.h"

MyWindow::MyWindow()
	: windowWidth(1480)
	, windowHeight(720)
{
}

MyWindow::~MyWindow()
{
	// glfw: terminate, clearing all previously allocated GLFW resources.
	glfwDestroyWindow(window);
	glfwTerminate();
}

bool MyWindow::Init()
{
	// glfw: initialize and configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	window = glfwCreateWindow(windowWidth, windowHeight, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window); //we tell GLFW to make the context of our window the main context on the current thread

									// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}
}

bool MyWindow::ShouldClose()
{
	return glfwWindowShouldClose(window) != 0;
}


void MyWindow::PollEvents()
{
	glfwPollEvents();
}

void MyWindow::SwapBuffers()
{
	glfwSwapBuffers(window);
}

GLFWwindow* MyWindow::getWindow()
{
	return window;
}