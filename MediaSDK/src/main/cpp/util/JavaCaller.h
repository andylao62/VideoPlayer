/**
 * C++调用Java类方法，调用器
 *
 * @author kejunyao
 * @since 2020年05月31日
 */

#ifndef AUDIOPLAYER_JAVACALLER_H
#define AUDIOPLAYER_JAVACALLER_H

#include <jni.h>
#include <linux/stddef.h>
#include <stdint.h>
#include "Log.h"

class JavaCaller {

private:
    JavaVM *jvm = NULL;
    JNIEnv *jenv = NULL;
    jobject instance;
    jmethodID jmidPostEvent;
    jmethodID jmidRenderYUV;
    jmethodID jmidSupportMediaCodec;
    jmethodID jmidInitMediaCoder;
    jmethodID jmidMediaDecode;
public:
    JavaCaller(JavaVM *javaVM, JNIEnv *env, jobject *instance);
    ~JavaCaller();

    /**
     * 调用Java方法
     * @param isWorkThread true，C++子线程; false，当前线程
     * @param event 事件类型
     * @param arg1 参数1
     * @param arg2 参数2
     */
    void callJavaMethod(bool isWorkThread, int event, int arg1, int arg2);

    void callRenderYUV(int width, int height, uint8_t *fy, uint8_t *fu, uint8_t *fv);

    /**
     * 还是否支持硬解码
     * @param codec_name
     * @return
     */
    bool isSupportMediaCodec(const char *codec_name);

    void initMediaCodec(const char *mime, int width, int height, int csd0_size, int csd1_size, uint8_t *csd_0, uint8_t *csd_1);

    void mediaDecode(int size, uint8_t *data);
};


#endif //AUDIOPLAYER_JAVACALLER_H
