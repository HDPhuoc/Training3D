#include "../RendererES3.h"

RendererES3* rendererES3;

#ifdef OS_ANDROID
#include <jni.h>
#include <android/log.h>
extern "C" JNIEXPORT void JNICALL
Java_dotdreamstudio_com_training3d_MainLib_createProgram(JNIEnv *env, jobject abc)
{
	__android_log_print(ANDROID_LOG_INFO, "HDPhuoc", "Java_dotdreamstudio_com_training3d_MainLib_createProgram");
	rendererES3 = new RendererES3();
	rendererES3->CreateProgram();
	return;
}

extern "C" JNIEXPORT void JNICALL
Java_dotdreamstudio_com_training3d_MainLib_loadTexture(JNIEnv *env, jobject abc)
{
	__android_log_print(ANDROID_LOG_INFO, "HDPhuoc", "Java_dotdreamstudio_com_training3d_MainLib_loadTexture");
	rendererES3->LoadTexture();
	return;
}

extern "C" JNIEXPORT void JNICALL
Java_dotdreamstudio_com_training3d_MainLib_init(JNIEnv *env, jobject abc)
{
	__android_log_print(ANDROID_LOG_INFO, "HDPhuoc", "Java_dotdreamstudio_com_training3d_MainLib_init");
	rendererES3->Init();
	return;
}

extern "C" JNIEXPORT void JNICALL
Java_dotdreamstudio_com_training3d_MainLib_resize(JNIEnv *env, jobject abc)
{
	__android_log_print(ANDROID_LOG_INFO, "HDPhuoc", "Java_dotdreamstudio_com_training3d_MainLib_resize new");
	return;
}

extern "C" JNIEXPORT void JNICALL
Java_dotdreamstudio_com_training3d_MainLib_step(JNIEnv *env, jobject abc, jfloat deltaTime)
{
	rendererES3->Draw(deltaTime);
	return;
}

extern "C" JNIEXPORT void JNICALL
Java_dotdreamstudio_com_training3d_MainLib_inputDirection(JNIEnv *env, jobject abc, jint direction)
{
	rendererES3->InputDirection(direction);
	return;
}

extern "C" JNIEXPORT void JNICALL
Java_dotdreamstudio_com_training3d_MainLib_mouseMovement(JNIEnv *env, jobject abc, jfloat xoffset, jfloat yoffset)
{
	rendererES3->MouseMovement(xoffset, yoffset);
	return;
}
#endif // OS_ANDROID