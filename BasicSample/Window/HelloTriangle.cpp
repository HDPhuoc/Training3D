#ifdef DRAW_TRIANGLE

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <cmath>
#include <thread>
#include <chrono>
#include <GL/glew.h>					// NOTE: You MUST include GLEW before any other OpenGL-related libraries!
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"					// NOTE: Need to compile, hence the quotes
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <SOIL.h>
#include "Shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
using namespace std;

/*///////////////////////////////////////////////////////////////////////////////
GLOBALS
///////////////////////////////////////////////////////////////////////////////*/
int windowWidth = 640;
int windowHeight = 480;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// stores how much we're seeing of either texture
float mixValue = 0.2f;
/*///////////////////////////////////////////////////////////////////////////////
FUNCTIONS - Main
///////////////////////////////////////////////////////////////////////////////*/
int main(void)
{
	// glfw: initialize and configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// glfw window creation
	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window); //we tell GLFW to make the context of our window the main context on the current thread

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}
	// ====build and compile our shader program====
	// ------------------------------------
	Shader ourShader1("../Shader/Triangle_VS.glsl", "../Shader/Triangle_FS.glsl");
	Shader ourShader2("../Shader/TextureTriangle_VS.glsl", "../Shader/TextureTriangle_FS.glsl");

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float Triangle[] = {
		0.0f, -0.5f, 0.0f,	0.0f, 0.0f,  // lower-left corner
		0.9f, -0.5f, 0.0f,	1.0f, 0.0f,  // lower-right corner
		0.45f, 0.5f, 0.0f,	0.5f, 1.0f   // top-center corner
	};

	float SecondTriangle[] = {
		// positions          // colors           // texture coords
		0.5f,  0.5f, 0.0f,	1.0f, 0.0f, 0.0f,	2.0f, 2.0f,   // top right
		0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   2.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
		-0.5f,  0.5f, 0.0f,  1.0f, 1.0f, 0.0f,   0.0f, 2.0f    // top left 
	};

	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	//EBO-element buffer objects a better solution is to store only the unique vertices and then specify the order at which we want to draw these vertices
	unsigned int VBOs[2], VAOs[2], EBO;
	glGenVertexArrays(2, VAOs); // we can also generate multiple VAOs or buffers at the same time
	glGenBuffers(2, VBOs);
	glGenBuffers(1, &EBO);
	//============================
	//==========Triangle==========
	//============================
	glBindVertexArray(VAOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Triangle), Triangle, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//============================
	//=======TextureTriangle======
	//============================
	glBindVertexArray(VAOs[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(SecondTriangle), SecondTriangle, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//Loading and creating textures
	//rectangle
	unsigned int texture, texture1, texture2;

	glGenTextures(1, &texture1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // note that we set the container wrapping method to GL_CLAMP_TO_EDGE
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // set texture filtering to nearest neighbor to clearly see the texels/pixels
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	int width, height, nrChannels;
	unsigned char *data = stbi_load("../Texture/container.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		//1= texture target,2= mipmap level,3= format want to store the texture,4/5= width & height of texture
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	glGenTextures(1, &texture2);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // set texture filtering to nearest neighbor to clearly see the texels/pixels
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	data = stbi_load("../Texture/awesomeface.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		// note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	//triangle
	glGenTextures(1, &texture);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, texture);
	int widthTexture, heightTexture, nrChannelsTexture;
	data = stbi_load("../Texture/wall.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		//1= texture target,2= mipmap level,3= format want to store the texture,4/5= width & height of texture
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	// tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
	// -------------------------------------------------------------------------------------------

	ourShader2.use(); // don't forget to activate/use the shader before setting uniforms!
	glUniform1i(ourShader2.getUniformLocation("texture1"), 0);
	glUniform1i(ourShader2.getUniformLocation("texture2"), 1);

	ourShader1.use();
	glUniform1i(ourShader1.getUniformLocation("texture3"), 2);

	// render loop
	while (!glfwWindowShouldClose(window))
	{
		// input
		processInput(window);

		// render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		//==== draw our triangle ====
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, texture);
		ourShader1.use();
		glBindVertexArray(VAOs[0]);
		float timeValue = glfwGetTime();
		float greenValue = sin(timeValue) / 2.0f + 0.5f;
		int triangleColor = ourShader1.getUniformLocation("ourColor");
		glUniform4f(triangleColor, 0.0f, greenValue, 0.0f, 1.0f);

		float rightValue = 0.07f;
		int trianglePosition = ourShader1.getUniformLocation("myPosTriangle");
		glUniform1f(trianglePosition, rightValue);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		//==== draw our texture triangle ====
		// bind textures on corresponding texture units
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		//render container
		ourShader2.use();
		glBindVertexArray(VAOs[1]);
		glUniform1f(ourShader2.getUniformLocation("mixValue"), mixValue);
		float leftValue = 0.0f;
		int textureTriangleLocation = ourShader2.getUniformLocation("myPos");
		glUniform1f(textureTriangleLocation, leftValue);

		glm::mat4 trans;
		trans = glm::translate(trans, glm::vec3(0.5f, 0.5f, 0.0f));
		//trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
		trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0, 0.0, 1.0));
		trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));
		unsigned int transformLoc = ourShader2.getUniformLocation("transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		ourShader2.use();
		glBindVertexArray(VAOs[1]);
		glm::mat4 trans1;
		trans1 = glm::translate(trans1, glm::vec3(-0.5f, -0.5f, 0.0f));

		float scaleFollowTime = sin(glfwGetTime());
		trans1 = glm::scale(trans1, glm::vec3(scaleFollowTime, scaleFollowTime, scaleFollowTime));
		unsigned int transformLoc1 = ourShader2.getUniformLocation("transform");
		glUniformMatrix4fv(transformLoc1, 1, GL_FALSE, glm::value_ptr(trans1));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(2, VAOs);
	glDeleteBuffers(2, VBOs);
	glDeleteBuffers(1, &EBO);
	// glfw: terminate, clearing all previously allocated GLFW resources.
	glfwTerminate();
	return 0;
}


/*///////////////////////////////////////////////////////////////////////////////
FUNCTIONS - Setup and OpenGL-related
///////////////////////////////////////////////////////////////////////////////*/
// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		std::cout << "processInput GLFW_KEY_UP" << std::endl;
		mixValue = 1.0f;
	}

	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		std::cout << "processInput GLFW_KEY_DOWN" << std::endl;
		mixValue = 0.0f;
	}
}
// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	//We have to tell OpenGL the size of the rendering window so OpenGL knows how we want to display the data and coordinates with respect to the window
	glViewport(0, 0, width, height);
}

#endif // DRAW_TRIANGLE