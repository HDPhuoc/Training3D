#pragma once

class GLFWwindow;

class MyWindow
{
public:
	MyWindow();
	~MyWindow();
	bool Init();
	bool ShouldClose();
	void PollEvents();
	void SwapBuffers();
	GLFWwindow* getWindow();
private:
	GLFWwindow* window;
	int windowWidth;
	int windowHeight;
};

