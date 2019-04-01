#include "../Source/Std.h"
#include "../Source/Timer.h"
#include "../Source/RendererES3.h"
#include "../Window/MyWindow.h"
using namespace std;

/*///////////////////////////////////////////////////////////////////////////////
GLOBALS
///////////////////////////////////////////////////////////////////////////////*/
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow *window);
/*///////////////////////////////////////////////////////////////////////////////
FUNCTIONS - Main
///////////////////////////////////////////////////////////////////////////////*/
RendererES3 rendererES3;
Timer timer;
int main(void)
{
	MyWindow myWindow;
	myWindow.Init();
	rendererES3.CreateProgram();
	rendererES3.LoadTexture();

	rendererES3.Init();
	glfwSetFramebufferSizeCallback(myWindow.getWindow(), framebuffer_size_callback);
	glfwSetCursorPosCallback(myWindow.getWindow(), mouse_callback);
	// render loop
	while (!myWindow.ShouldClose())
	{

		// input
		processInput(myWindow.getWindow());

		// render
		rendererES3.Draw(timer.getTime());

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		myWindow.PollEvents();
		myWindow.SwapBuffers();
	}
	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------

	return 0;
}


/*///////////////////////////////////////////////////////////////////////////////
FUNCTIONS - Setup and OpenGL-related
///////////////////////////////////////////////////////////////////////////////*/
// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	//We have to tell OpenGL the size of the rendering window so OpenGL knows how we want to display the data and coordinates with respect to the window
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	std::cout << "mouse_callback xpos=" << xpos << " ypos=" << ypos << std::endl;
	rendererES3.MouseMovement(xpos, ypos);
}
// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		std::cout << "processInput GLFW_KEY_ESCAPE" << std::endl;
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		std::cout << "processInput GLFW_KEY_UP" << std::endl;
		rendererES3.InputDirection(4);
	}

	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		std::cout << "processInput GLFW_KEY_DOWN" << std::endl;
		rendererES3.InputDirection(5);
	}

	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		std::cout << "processInput GLFW_KEY_RIGHT" << std::endl;
		rendererES3.InputDirection(3);
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		std::cout << "processInput GLFW_KEY_LEFT" << std::endl;
		rendererES3.InputDirection(2);
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		std::cout << "processInput GLFW_KEY_W" << std::endl;
		rendererES3.InputDirection(0);
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		std::cout << "processInput GLFW_KEY_S" << std::endl;
		rendererES3.InputDirection(1);
	}
}


