#ifndef SHADER_H
#define SHADER_H

#if OS_ANDROID
#include <GLES3/gl3.h>
#include <EGL/egl.h>
#include <android/log.h>
#define LOG_TAG "HDPhuoc"
#define ALOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#else
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#define ALOGI(...)
#endif
class Shader
{
public:
	Shader();
	~Shader();
	unsigned int createShader(GLenum shaderType, const char* shaderSource);
	unsigned int createProgram(const char* vertexSource, const char* fragmentSource);
private:
	bool CheckGlError(const char* funcName);
};

Shader::Shader()
{
}

Shader::~Shader()
{
}
#endif /* SHADER_H */