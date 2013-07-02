#include "AppDelegate.h"
#include "platform/android/jni/JniHelper.h"
#include <jni.h>
#include <android/log.h>

#include "HelloWorldScene.h"

#define  LOG_TAG    "main"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)

using namespace cocos2d;

extern "C"
{

jint JNI_OnLoad(JavaVM *vm, void *reserved)
{
    LOGD("JNI_OnLoad");
    JniHelper::setJavaVM(vm);

    return JNI_VERSION_1_4;
}

    void cocos_android_app_init (void) {
        LOGD("cocos_android_app_init");
        simplegame::AppDelegate *pAppDelegate = new simplegame::AppDelegate();
    }
}
