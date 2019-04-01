#include <fstream>
#include <sstream>
#include <math.h>
#include "Std.h"
#include "RendererES3.h"
#include "Camera.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#ifdef _WIN32
#define SHADERPATH "../Shader/";
#define TEXTUREPATH "../Texture/";
#elif OS_ANDROID
#define SHADERPATH "/storage/emulated/0/Android/data/dotdreamstudio.com.training3d/files/Shader/";
#define TEXTUREPATH "/storage/emulated/0/Android/data/dotdreamstudio.com.training3d/files/Texture/";
#endif

Camera camera;
// timing
float mDeltaTime = 0.0f;	// time between current frame and last frame
float mLastFrame = 0.0f;
//mouse offset
float lastX = 1480 / 2;
float lastY = 720 / 2;
bool firstMouse = true;
// lighting
//glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

// set up vertex data (and buffer(s)) and configure vertex attributes
// ------------------------------------------------------------------
float vertices[] = {
	// positions          // normals           // texture coords
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
	0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
	0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
	0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
	0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

	0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
	0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
	0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
	0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
};
// positions all containers
glm::vec3 cubePositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
};
// positions of the point lights
glm::vec3 pointLightPositions[] = {
	glm::vec3(0.7f,  0.2f,  2.0f),
	glm::vec3(2.3f, -3.3f, -4.0f),
	glm::vec3(-4.0f,  2.0f, -12.0f),
	glm::vec3(0.0f,  0.0f, -3.0f)
};
glm::vec3 pointLightColors[] = {
	glm::vec3(1.0f, 0.6f, 0.0f),
	glm::vec3(1.0f, 0.0f, 0.0f),
	glm::vec3(1.0f, 1.0, 0.0),
	glm::vec3(0.2f, 0.2f, 1.0f)
};
bool CheckGlError(const char* funcName) {
	GLenum err = glGetError();
	if (err != GL_NO_ERROR) {
#ifdef _WIN32
		std::cout << "GL error after " << funcName << " : " << err << std::endl;
#else
		ALOGI("GL error after %s(): 0x%08x\n", funcName, err);
#endif
		return true;
	}
	return false;
}

unsigned int createShader(GLenum shaderType, const char* shaderSource) {
	unsigned int shader = glCreateShader(shaderType);
	if (!shader) {
		CheckGlError("glCreateShader");
		return 0;
	}
	glShaderSource(shader, 1, &shaderSource, NULL);
	glCompileShader(shader);
	int success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		ALOGI("Could not compile %s shader:\n%s\n", shaderType == GL_VERTEX_SHADER ? "vertex" : "fragment", infoLog);
		glDeleteShader(shader);
		return 0;
	}
	return shader;
}

unsigned int createProgram(const char* vertexSource, const char* fragmentSource) 
{
	ALOGI("RendererES3::createProgram vertexSource=%s fragmentSource=%s", vertexSource, fragmentSource);
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	vShaderFile.exceptions(std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::badbit);
	try
	{
		// open files
		vShaderFile.open(vertexSource);
		fShaderFile.open(fragmentSource);
		std::stringstream vShaderStream, fShaderStream;
		// read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		// close file handlers
		vShaderFile.close();
		fShaderFile.close();
		// convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();
	ALOGI("RendererES3::createProgram 2 vertexPath=%s fragmentPath=%s", vShaderCode, fShaderCode);

	unsigned int vertexShader = 0;
	unsigned int fragmentShader = 0;
	unsigned int shaderProgram = 0;

	vertexShader = createShader(GL_VERTEX_SHADER, vShaderCode);
	if (!vertexShader)
		return 0;

	fragmentShader = createShader(GL_FRAGMENT_SHADER, fShaderCode);
	if (!fragmentShader)
		return 0;

	shaderProgram = glCreateProgram();
	if (!shaderProgram) {
		CheckGlError("glCreateProgram");
		return 0;
	}
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	int success;
	char infoLog[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		ALOGI("Could not link program");
		glDeleteProgram(shaderProgram);
		shaderProgram = 0;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	return shaderProgram;
}

unsigned int loadTexture(const char* path)
{
	ALOGI("RendererES3::loadTexture path=%s", path);
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}

RendererES3::RendererES3()
{}

RendererES3::~RendererES3()
{
	glDeleteVertexArrays(1, &mCubeVAO);
	glDeleteVertexArrays(1, &mLightVAO);
	glDeleteBuffers(1, &mVBO);
	glDeleteProgram(mCubeShader);
	glDeleteProgram(mLightShader);
}

bool RendererES3::CreateProgram()
{
	std::string Cube_VS_Path = SHADERPATH;
	Cube_VS_Path += "Cube_VS.glsl";
	std::string Cube_FS_Path = SHADERPATH;
	Cube_FS_Path += "Cube_FS.glsl";

	mCubeShader = createProgram(Cube_VS_Path.c_str(), Cube_FS_Path.c_str());
	if (!mCubeShader)
		return false;

	std::string Light_VS_Path = SHADERPATH;
	Light_VS_Path += "Light_VS.glsl";
	std::string Light_FS_Path = SHADERPATH;
	Light_FS_Path += "Light_FS.glsl";

	mLightShader = createProgram(Light_VS_Path.c_str(), Light_FS_Path.c_str());
	if (!mLightShader)
		return false;

	return true;
}

void RendererES3::LoadTexture()
{
	std::string Cube_Texture_Path = TEXTUREPATH;
	Cube_Texture_Path += "container2.png";
	mTexture1 = loadTexture(Cube_Texture_Path.c_str());

	std::string Spec_Texture_Path = TEXTUREPATH;
	Spec_Texture_Path += "container2_specular.png";
	mTexture2 = loadTexture(Spec_Texture_Path.c_str());
}

bool RendererES3::Init()
{
	camera.Init();
	glEnable(GL_DEPTH_TEST);

	glGenBuffers(1, &mVBO);

	glGenVertexArrays(1, &mCubeVAO);
	glBindVertexArray(mCubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glGenVertexArrays(1, &mLightVAO);
	glBindVertexArray(mLightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glUseProgram(mCubeShader);
	unsigned int mTexture1Loc = glGetUniformLocation(mCubeShader, "materialDiffuse");
	glUniform1i(mTexture1Loc, 0);
	unsigned int mTexture2Loc = glGetUniformLocation(mCubeShader, "materialSpecular");
	glUniform1i(mTexture2Loc, 1);
	return true;
}

void RendererES3::Draw(float deltaTime)
{
	ALOGI("RendererES3::Draw mCubeShader=%d mLightShader=%d", mCubeShader, mLightShader);
	float currentFrame = deltaTime;
	mDeltaTime = currentFrame - mLastFrame;
	mLastFrame = currentFrame;

	glClearColor(0.75f, 0.52f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(mCubeShader);
	// be sure to activate shader when setting uniforms/drawing objects
	unsigned int viewPosLoc = glGetUniformLocation(mCubeShader, "viewPos");
	glUniform3fv(viewPosLoc, 1, &camera.CameraPosition[0]);
	unsigned int shininessLoc = glGetUniformLocation(mCubeShader, "materialShininess");
	glUniform1f(shininessLoc, 32.0f);
	/*
	Here we set all the uniforms for the 5/6 types of lights we have. We have to set them manually and index
	the proper PointLight struct in the array to set each uniform variable. This can be done more code-friendly
	by defining light types as classes and set their values in there, or by using a more efficient uniform approach
	by using 'Uniform buffer objects', but that is something we'll discuss in the 'Advanced GLSL' tutorial.
	*/
	// directional light
	unsigned int dirLightDirLoc = glGetUniformLocation(mCubeShader, "dirLight.direction");
	glUniform3f(dirLightDirLoc, -0.2f, -1.0f, -0.3f);
	unsigned int dirLightAmbientLoc = glGetUniformLocation(mCubeShader, "dirLight.ambient");
	glUniform3f(dirLightAmbientLoc, 0.3f, 0.24f, 0.14f);
	unsigned int dirLightDiffuseLoc = glGetUniformLocation(mCubeShader, "dirLight.diffuse");
	glUniform3f(dirLightDiffuseLoc, 0.7f, 0.42f, 0.26f);
	unsigned int dirLightSpecularLoc = glGetUniformLocation(mCubeShader, "dirLight.specular");
	glUniform3f(dirLightSpecularLoc, 0.5f, 0.5f, 0.5f);

	// point light 1
	unsigned int pointLight0PosLoc = glGetUniformLocation(mCubeShader, "pointLights[0].position");
	glUniform3f(pointLight0PosLoc, pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
	unsigned int pointLight0AmbientLoc = glGetUniformLocation(mCubeShader, "pointLights[0].ambient");
	glUniform3f(pointLight0AmbientLoc, pointLightColors[0].x * 0.1, pointLightColors[0].y * 0.1, pointLightColors[0].z * 0.1);
	unsigned int pointLight0DiffuseLoc = glGetUniformLocation(mCubeShader, "pointLights[0].diffuse");
	glUniform3f(pointLight0DiffuseLoc, pointLightColors[0].x, pointLightColors[0].y, pointLightColors[0].z);
	unsigned int pointLight0SpecularLoc = glGetUniformLocation(mCubeShader, "pointLights[0].specular");
	glUniform3f(pointLight0SpecularLoc, pointLightColors[0].x, pointLightColors[0].y, pointLightColors[0].z);
	unsigned int pointLight0ConstantLoc = glGetUniformLocation(mCubeShader, "pointLights[0].constant");
	glUniform1f(pointLight0ConstantLoc, 1.0f);
	unsigned int pointLight0LinearLoc = glGetUniformLocation(mCubeShader, "pointLights[0].linear");
	glUniform1f(pointLight0LinearLoc, 0.09);
	unsigned int pointLight0QuadraticLoc = glGetUniformLocation(mCubeShader, "pointLights[0].quadratic");
	glUniform1f(pointLight0QuadraticLoc, 0.032);
	// point light 2
	unsigned int pointLight1PosLoc = glGetUniformLocation(mCubeShader, "pointLights[1].position");
	glUniform3f(pointLight1PosLoc, pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
	unsigned int pointLight1AmbientLoc = glGetUniformLocation(mCubeShader, "pointLights[1].ambient");
	glUniform3f(pointLight1AmbientLoc, pointLightColors[1].x * 0.1, pointLightColors[1].y * 0.1, pointLightColors[1].z * 0.1);
	unsigned int pointLight1DiffuseLoc = glGetUniformLocation(mCubeShader, "pointLights[1].diffuse");
	glUniform3f(pointLight1DiffuseLoc, pointLightColors[1].x, pointLightColors[1].y, pointLightColors[1].z);
	unsigned int pointLight1SpecularLoc = glGetUniformLocation(mCubeShader, "pointLights[1].specular");
	glUniform3f(pointLight1SpecularLoc, pointLightColors[1].x, pointLightColors[1].y, pointLightColors[1].z);
	unsigned int pointLight1ConstantLoc = glGetUniformLocation(mCubeShader, "pointLights[1].constant");
	glUniform1f(pointLight1ConstantLoc, 1.0f);
	unsigned int pointLight1LinearLoc = glGetUniformLocation(mCubeShader, "pointLights[1].linear");
	glUniform1f(pointLight1LinearLoc, 0.09);
	unsigned int pointLight1QuadraticLoc = glGetUniformLocation(mCubeShader, "pointLights[1].quadratic");
	glUniform1f(pointLight1QuadraticLoc, 0.032);
	// point light 3
	unsigned int pointLight2PosLoc = glGetUniformLocation(mCubeShader, "pointLights[2].position");
	glUniform3f(pointLight2PosLoc, pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
	unsigned int pointLight2AmbientLoc = glGetUniformLocation(mCubeShader, "pointLights[2].ambient");
	glUniform3f(pointLight2AmbientLoc, pointLightColors[2].x * 0.1, pointLightColors[2].y * 0.1, pointLightColors[2].z * 0.1);
	unsigned int pointLight2DiffuseLoc = glGetUniformLocation(mCubeShader, "pointLights[2].diffuse");
	glUniform3f(pointLight2DiffuseLoc, pointLightColors[2].x, pointLightColors[2].y, pointLightColors[2].z);
	unsigned int pointLight2SpecularLoc = glGetUniformLocation(mCubeShader, "pointLights[2].specular");
	glUniform3f(pointLight2SpecularLoc, pointLightColors[2].x, pointLightColors[2].y, pointLightColors[2].z);
	unsigned int pointLight2ConstantLoc = glGetUniformLocation(mCubeShader, "pointLights[2].constant");
	glUniform1f(pointLight2ConstantLoc, 1.0f);
	unsigned int pointLight2LinearLoc = glGetUniformLocation(mCubeShader, "pointLights[2].linear");
	glUniform1f(pointLight2LinearLoc, 0.09);
	unsigned int pointLight2QuadraticLoc = glGetUniformLocation(mCubeShader, "pointLights[2].quadratic");
	glUniform1f(pointLight2QuadraticLoc, 0.032);
	// point light 4
	unsigned int pointLight3PosLoc = glGetUniformLocation(mCubeShader, "pointLights[3].position");
	glUniform3f(pointLight3PosLoc, pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
	unsigned int pointLight3AmbientLoc = glGetUniformLocation(mCubeShader, "pointLights[3].ambient");
	glUniform3f(pointLight3AmbientLoc, pointLightColors[3].x * 0.1, pointLightColors[3].y * 0.1, pointLightColors[3].z * 0.1);
	unsigned int pointLight3DiffuseLoc = glGetUniformLocation(mCubeShader, "pointLights[3].diffuse");
	glUniform3f(pointLight3DiffuseLoc, pointLightColors[3].x, pointLightColors[3].y, pointLightColors[3].z);
	unsigned int pointLight3SpecularLoc = glGetUniformLocation(mCubeShader, "pointLights[3].specular");
	glUniform3f(pointLight3SpecularLoc, pointLightColors[3].x, pointLightColors[3].y, pointLightColors[3].z);
	unsigned int pointLight3ConstantLoc = glGetUniformLocation(mCubeShader, "pointLights[3].constant");
	glUniform1f(pointLight3ConstantLoc, 1.0f);
	unsigned int pointLight3LinearLoc = glGetUniformLocation(mCubeShader, "pointLights[3].linear");
	glUniform1f(pointLight3LinearLoc, 0.09);
	unsigned int pointLight3QuadraticLoc = glGetUniformLocation(mCubeShader, "pointLights[3].quadratic");
	glUniform1f(pointLight3QuadraticLoc, 0.032);

	// spotLight
	unsigned int spotLightPosLoc = glGetUniformLocation(mCubeShader, "spotLight.position");
	glUniform3fv(spotLightPosLoc, 1, &camera.CameraPosition[0]);
	unsigned int spotLightDirLoc = glGetUniformLocation(mCubeShader, "spotLight.direction");
	glUniform3fv(spotLightDirLoc, 1, &camera.CameraTarget[0]);
	unsigned int spotLightAmbientLoc = glGetUniformLocation(mCubeShader, "spotLight.ambient");
	glUniform3f(spotLightAmbientLoc, 0.0f, 0.0f, 0.0f);
	unsigned int spotLightDiffuseLoc = glGetUniformLocation(mCubeShader, "spotLight.diffuse");
	glUniform3f(spotLightDiffuseLoc, 0.8f, 0.8f, 0.0f);
	unsigned int spotLightSpecularLoc = glGetUniformLocation(mCubeShader, "spotLight.specular");
	glUniform3f(spotLightSpecularLoc, 0.8f, 0.8f, 0.0f);
	unsigned int spotLightConstantLoc = glGetUniformLocation(mCubeShader, "spotLight.constant");
	glUniform1f(spotLightConstantLoc, 1.0f);
	unsigned int spotLightLinearLoc = glGetUniformLocation(mCubeShader, "spotLight.linear");
	glUniform1f(spotLightLinearLoc, 0.09);
	unsigned int spotLightQuadraticLoc = glGetUniformLocation(mCubeShader, "spotLight.quadratic");
	glUniform1f(spotLightQuadraticLoc, 0.032);
	unsigned int spotLightCutOffLoc = glGetUniformLocation(mCubeShader, "spotLight.cutOff");
	glUniform1f(spotLightCutOffLoc, glm::cos(glm::radians(12.5f)));
	unsigned int spotLightOuterCutOffLoc = glGetUniformLocation(mCubeShader, "spotLight.outerCutOff");
	glUniform1f(spotLightOuterCutOffLoc, glm::cos(glm::radians(13.0f)));

	// view/projection transformations
	glm::mat4 projection = camera.GetProjectionMatrix();
	unsigned int projectionLoc = glGetUniformLocation(mCubeShader, "projection");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, &projection[0][0]);

	glm::mat4 view = camera.GetViewMatrix();
	unsigned int viewLoc = glGetUniformLocation(mCubeShader, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);

	glm::mat4 model = glm::mat4(1.0f);
	unsigned int modelLoc = glGetUniformLocation(mCubeShader, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mTexture1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, mTexture2);
	glBindVertexArray(mCubeVAO);
	for (unsigned int i = 0; i < 10; i++)
	{
		// calculate the model matrix for each object and pass it to shader before drawing
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, cubePositions[i]);
		float angle = 20.0f * i;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		unsigned int modelLightLoc = glGetUniformLocation(mCubeShader, "model");
		glUniformMatrix4fv(modelLightLoc, 1, GL_FALSE, glm::value_ptr(model));

		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	CheckGlError("RendererES3::Draw Cube");

	// also draw the light object(s)
	glUseProgram(mLightShader);
	unsigned int lightProjectionLoc = glGetUniformLocation(mLightShader, "projection");
	glUniformMatrix4fv(lightProjectionLoc, 1, GL_FALSE, &projection[0][0]);
	unsigned int lightViewLoc = glGetUniformLocation(mLightShader, "view");
	glUniformMatrix4fv(lightViewLoc, 1, GL_FALSE, &view[0][0]);
	glBindVertexArray(mLightVAO);
	for (unsigned int i = 0; i < 4; i++)
	{
		unsigned int colorLoc = glGetUniformLocation(mLightShader, "color");
		glUniform3fv(colorLoc, 1, &pointLightColors[i][0]);
		model = glm::mat4(1.0f);
		model = glm::translate(model, pointLightPositions[i]);
		model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		unsigned int lightModelLoc = glGetUniformLocation(mLightShader, "model");
		glUniformMatrix4fv(lightModelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	CheckGlError("RendererES3::Draw Light");
}

void RendererES3::InputDirection(int direction)
{
	Camera_Direction mCamera_Direction;

	if (direction == 0)
		mCamera_Direction = Camera_Direction::FORWARD;
	else if (direction == 1)
		mCamera_Direction = Camera_Direction::BACKWARD;
	else if (direction == 2)
		mCamera_Direction = Camera_Direction::LEFT;
	else if (direction == 3)
		mCamera_Direction = Camera_Direction::RIGHT;
	else if (direction == 4)
		mCamera_Direction = Camera_Direction::UP;
	else if (direction == 5)
		mCamera_Direction = Camera_Direction::DOWN;

#if OS_ANDROID
	//mDeltaTime = mDeltaTime / 4.0f;
	mDeltaTime = 0.000001;
#endif
	//ALOGI("RendererES3::InputDirection mDeltaTime=%f", mDeltaTime);
	camera.CalculateCameraDirection(mCamera_Direction, mDeltaTime);
}

void RendererES3::MouseMovement(float xpos, float ypos)
{
#ifdef _WIN32
	std::cout << "RendererES3::MouseMovement xpos=" << xpos << " ypos=" << ypos << std::endl;
#else
	ALOGI("RendererES3::MouseMovement xpos=%f ypos=%f", xpos, ypos);
#endif // _WIN32
	if (firstMouse) {
		std::cout << "mouse_callback firstMouse" << std::endl;
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;
#ifdef _WIN32
	std::cout << "RendererES3::MouseMovement xoffset=" << xoffset << " yoffset=" << yoffset << std::endl;
#else
	ALOGI("RendererES3::MouseMovement xoffset=%f yoffset=%f", xoffset, yoffset);
#endif // _WIN32
	camera.CalculateMouseMovement(xoffset, yoffset);
}
