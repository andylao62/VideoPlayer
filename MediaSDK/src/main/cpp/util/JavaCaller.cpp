//
// Created by kejunyao on 2020/05/31.
//

#include "JavaCaller.h"

JavaCaller::JavaCaller(JavaVM *jvm, JNIEnv *env, jobject *instance) {
    this->jvm = jvm;
    this->jenv = env;
    this->instance = env->NewGlobalRef(*instance);
    jclass clz = jenv->GetObjectClass(this->instance);
    if (!clz) {
        if (LOG_DEBUG) {
            LOGE("can not get instance class");
        }
        return;
    }
    jmidPostEvent = jenv->GetMethodID(clz, "postEventFromNative", "(III)V");
    jmidRenderYUV = env->GetMethodID(clz, "renderYUV", "(II[B[B[B)V");
}

JavaCaller::~JavaCaller() {
}

void JavaCaller::callJavaMethod(bool isWorkThread, int event, int arg1, int arg2) {
    if (isWorkThread) {
        JNIEnv *env;
        if (jvm->AttachCurrentThread(&env, 0) != JNI_OK) {
            if (LOG_DEBUG) {
                LOGE("Attach current thread failure.");
            }
        }
        env->CallVoidMethod(instance, jmidPostEvent, event, arg1, arg2);
        jvm->DetachCurrentThread();
        return;
    }
    jenv->CallVoidMethod(instance, jmidPostEvent, event, arg1, arg2);
}

void JavaCaller::callRenderYUV(int width, int height, uint8_t *fy, uint8_t *fu, uint8_t *fv) {
    JNIEnv *jniEnv;
    if (jvm->AttachCurrentThread(&jniEnv, 0) != JNI_OK) {
        if (LOG_DEBUG) {
            LOGE("call onCallComplete worng");
        }
        return;
    }

    jbyteArray y = jniEnv->NewByteArray(width * height);
    jniEnv->SetByteArrayRegion(y, 0, width * height, reinterpret_cast<const jbyte *>(fy));

    jbyteArray u = jniEnv->NewByteArray(width * height / 4);
    jniEnv->SetByteArrayRegion(u, 0, width * height / 4, reinterpret_cast<const jbyte *>(fu));

    jbyteArray v = jniEnv->NewByteArray(width * height / 4);
    jniEnv->SetByteArrayRegion(v, 0, width * height / 4, reinterpret_cast<const jbyte *>(fv));

    jniEnv->CallVoidMethod(instance, jmidRenderYUV, width, height, y, u, v);

    jniEnv->DeleteLocalRef(y);
    jniEnv->DeleteLocalRef(u);
    jniEnv->DeleteLocalRef(v);

    jvm->DetachCurrentThread();
}


