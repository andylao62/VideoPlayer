/**
 * 音频播放Player
 *
 * @author kejunyao
 * @since 2020年05月30日
 */
#include <jni.h>
#include <string>
#include "MediaPlayerController.h"

JavaVM *javaVM = NULL;

extern "C"
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *jvm, void *reserved) {
    javaVM = jvm;
    JNIEnv *env;
    if (jvm->GetEnv((void **) (&env), JNI_VERSION_1_4) != JNI_OK) {
        return -1;
    }
    return JNI_VERSION_1_4;
}

MediaPlayerController *playerController = NULL;

extern "C"
JNIEXPORT void JNICALL
Java_com_kejunyao_media_MediaPlayer__1start(JNIEnv *env, jobject thiz) {
    if (playerController == NULL) {
        return;
    }
    playerController->start();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_kejunyao_media_MediaPlayer__1pause(JNIEnv *env, jobject thiz) {
    if (playerController == NULL) {
        return;
    }
    playerController->pause();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_kejunyao_media_MediaPlayer__1stop(JNIEnv *env, jobject thiz) {
    if (playerController == NULL) {
        return;
    }
    playerController->stop();
}

pthread_t threadRelease;

void *releaseCallback(void *data) {
    if (playerController != NULL) {
        playerController->release();
        delete(playerController);
        playerController = NULL;
    }
    pthread_exit(&threadRelease);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_kejunyao_media_MediaPlayer__1release(JNIEnv *env, jobject thiz) {
    if (playerController == NULL) {
        return;
    }
    pthread_create(&threadRelease, NULL, releaseCallback, NULL);

}

extern "C"
JNIEXPORT void JNICALL
Java_com_kejunyao_media_MediaPlayer__1prepare(JNIEnv *env, jobject thiz, jstring _source) {
    const char *source = env->GetStringUTFChars(_source, 0);
    if (playerController == NULL) {
        playerController = new MediaPlayerController(javaVM, env, &thiz);
    }
    playerController -> prepare(source);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_kejunyao_media_MediaPlayer__1resume(JNIEnv *env, jobject thiz) {
    if (playerController == NULL) {
        return;
    }
    playerController->resume();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_kejunyao_media_MediaPlayer__1seekByPercent(JNIEnv *env, jobject thiz, jfloat percent) {
    if (playerController == NULL) {
        return;
    }
    playerController->seekByPercent(percent);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_kejunyao_media_MediaPlayer__1seek(JNIEnv *env, jobject thiz, jint second) {
    if (playerController == NULL) {
        return;
    }
    playerController->seek(second);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_kejunyao_media_MediaPlayer__1setVolume(JNIEnv *env, jobject thiz, jfloat percent) {
    if (playerController == NULL) {
        return;
    }
    playerController->setVolume(percent);
}