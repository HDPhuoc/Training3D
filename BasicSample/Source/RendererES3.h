#pragma once

class RendererES3
{
public:
	RendererES3();
	~RendererES3();
	bool CreateProgram(); 
	void LoadTexture();
	bool Init();
	void Draw(float deltaTime);
	void InputDirection(int direction);
	void MouseMovement(float xoffset, float yoffset);
private:
	unsigned int mCubeShader;
	unsigned int mLightShader;
	unsigned int mCubeVAO;
	unsigned int mLightVAO;
	unsigned int mVBO;
	unsigned int mEBO;
	unsigned int mTexture1;
	unsigned int mTexture2;
};

