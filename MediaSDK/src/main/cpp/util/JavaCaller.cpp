/**
 * C++调用Java类方法，调用器
 *
 * @author kejunyao
 * @since 2020年05月31日
 */

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
    jmidSupportMediaCodec = env->GetMethodID(clz, "isSupportMediaCodec", "(Ljava/lang/String;)Z");
    jmidInitMediaCoder = env->GetMethodID(clz, "initMediaCoder", "(Ljava/lang/String;II[B[B)V");
    jmidMediaDecode = env->GetMethodID(clz, "mediaDecode", "(I[B)V");
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

bool JavaCaller::isSupportMediaCodec(const char *codec_name) {
    JNIEnv *jniEnv;
    if (jvm->AttachCurrentThread(&jniEnv, 0) != JNI_OK) {
        if (LOG_DEBUG) {
            LOGE("call isSupportMediaCodec wrong");
        }
        return false;
    }
    jstring type = jniEnv->NewStringUTF(codec_name);
    bool support = jniEnv->CallBooleanMethod(instance, jmidSupportMediaCodec, type);
    jniEnv->DeleteLocalRef(type);
    jvm->DetachCurrentThread();
    return support;
}

void JavaCaller::initMediaCodec(const char *mime, int width, int height, int csd0_size, int csd1_size,
                           uint8_t *csd_0, uint8_t *csd_1) {
    JNIEnv *jniEnv;
    if (jvm->AttachCurrentThread(&jniEnv, 0) != JNI_OK) {
        if (LOG_DEBUG) {
            LOGE("call initMediaCodec wrong");
        }
        return;
    }

    jstring type = jniEnv->NewStringUTF(mime);
    jbyteArray csd0 = jniEnv->NewByteArray(csd0_size);
    jniEnv->SetByteArrayRegion(csd0, 0, csd0_size, reinterpret_cast<const jbyte *>(csd_0));
    jbyteArray csd1 = jniEnv->NewByteArray(csd1_size);
    jniEnv->SetByteArrayRegion(csd1, 0, csd1_size, reinterpret_cast<const jbyte *>(csd_1));

    jniEnv->CallVoidMethod(instance, jmidInitMediaCoder, type, width, height, csd0, csd1);

    jniEnv->DeleteLocalRef(csd0);
    jniEnv->DeleteLocalRef(csd1);
    jniEnv->DeleteLocalRef(type);
    jvm->DetachCurrentThread();
}

void JavaCaller::mediaDecode(int size, uint8_t *data) {
    JNIEnv *jniEnv;
    if (jvm->AttachCurrentThread(&jniEnv, 0) != JNI_OK) {
        if (LOG_DEBUG) {
            LOGE("call mediaDecode wrong");
        }
        return;
    }
    jbyteArray array = jniEnv->NewByteArray(size);
    jniEnv->SetByteArrayRegion(array, 0, size, reinterpret_cast<const jbyte *>(data));
    jniEnv->CallVoidMethod(instance, jmidMediaDecode, size, array);
    jniEnv->DeleteLocalRef(array);
    jvm->DetachCurrentThread();
}




