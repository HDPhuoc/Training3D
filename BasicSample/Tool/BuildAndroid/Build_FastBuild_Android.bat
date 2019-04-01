@echo off

cls

del /s /q D:\BasicSample\build\Android\armeabi-v7a\lib3DEngineSO.so

echo **** BUILDING FASTBUILD ANDROID ****
call ..\FastBuild\FBuild.exe -summary
echo **** BUILDING FASTBUILD ANDROID END ****

if not exist D:\BasicSample\build\Android\armeabi-v7a\lib3DEngineSO.so (
	goto END
)

echo **** COPY
del C:\Users\phuoc.hoduy\AndroidStudioProjects\Training3D\app\src\main\jniLibs\armeabi-v7a\lib3DEngineSO.so 
REM xcopy D:\BasicSample\build\Android\arm64-v8a\lib3DEngineSO.so C:\Users\phuoc.hoduy\AndroidStudioProjects\Training3D\app\src\main\jniLibs\arm64-v8a
xcopy D:\BasicSample\build\Android\armeabi-v7a\lib3DEngineSO.so C:\Users\phuoc.hoduy\AndroidStudioProjects\Training3D\app\src\main\jniLibs\armeabi-v7a

echo ==================================
echo ==================================
echo BUILD APK
rmdir C:\Users\phuoc.hoduy\AndroidStudioProjects\Training3D\app\build\outputs\apk\debug

cd /d C:\Users\phuoc.hoduy\AndroidStudioProjects\Training3D
call gradlew.bat assembleDebug

echo UNINSTALL
adb shell pm uninstall -k dotdreamstudio.com.training3d

echo PUSH DATA
cd /d D:\BasicSample
adb push Data/. /sdcard/Android/data/

echo INSTALL
cd /d C:\Users\phuoc.hoduy\AndroidStudioProjects\Training3D\app\build\outputs\apk\debug
adb install -r -d app-debug.apk

echo RUN APP
adb shell am start -n "dotdreamstudio.com.training3d/.MainActivity"

:END
echo RETURN SCRIPT FOLDER
cd /d D:\BasicSample\Tool\BuildAndroid

pause