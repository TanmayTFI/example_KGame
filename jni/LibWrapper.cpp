#include <jni.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

extern void androidJniInitWithAssetManager(JNIEnv* env, jobject obj, jobject assetManager);
extern void androidJniSurfaceCreated (JNIEnv *env, jobject obj, jobject lpCaller, const char *lpszPackage);
extern void androidJniShutdown (JNIEnv *env, jobject obj, jobject lpCaller);
extern void androidJniSizeChanged (JNIEnv *env, jobject obj, jobject lpCaller, int width, int height);
extern void androidJniHandleTouchEvent (JNIEnv *env, jobject obj, jobject lpCaller, int nPointerId, int nPointerCount, float x, float y, float z, int nAction);
extern void androidJniHandleKeyEvent (JNIEnv *env, jobject obj, jobject lpCaller, int keycode, int down, int unicode, int eventSource, int deviceId);
extern void androidJniHandleAccelerometerEvent (JNIEnv *env, jobject obj, jobject lpCaller, float x, float y, float z);
extern void androidJniHandleFocusEvent (JNIEnv *env, jobject obj, jobject lpCaller, int nFocusGained);
extern void androidJniHandleUserEvent (JNIEnv *env, jobject obj, jobject lpCaller, int nEventType);
extern void androidJniHandleSoundEvent (JNIEnv *env, jobject obj, jobject lpCaller, int nEventType, int nEventData);
extern void androidJniHandleJoystickEvent (JNIEnv *env, jobject obj, jobject lpCaller, float x, float y, float z, float rx, float ry, float rz,
                                           float hatx, float haty, float hscroll, float vscroll,
                                           float brake, float gas, float rudder, float throttle, float tilt, float wheel,
                                           int eventSource, int deviceId);
extern int androidJniGetOpenGLESVersion (JNIEnv *env, jobject obj, jobject lpCaller);
extern void androidJniRun (JNIEnv *env, jobject obj, jobject lpCaller);

extern "C" {
   JNIEXPORT void JNICALL Java_com_bigfishgames_kanji_KanjiGameLib_initWithAssetManager(JNIEnv* env, jobject obj, jobject assetManager);
   JNIEXPORT void JNICALL Java_com_bigfishgames_kanji_KanjiGameLib_surfaceCreated(JNIEnv * env, jobject obj, jobject lpCaller);
   JNIEXPORT void JNICALL Java_com_bigfishgames_kanji_KanjiGameLib_shutdown(JNIEnv * env, jobject obj, jobject lpCaller);
   JNIEXPORT void JNICALL Java_com_bigfishgames_kanji_KanjiGameLib_sizeChanged(JNIEnv * env, jobject obj, jobject lpCaller, int width, int height);
   JNIEXPORT void JNICALL Java_com_bigfishgames_kanji_KanjiGameLib_handleTouchEvent(JNIEnv * env, jobject obj, jobject lpCaller, int nPointerId, int nPointerCount, float x, float y, float z, int nAction);
   JNIEXPORT void JNICALL Java_com_bigfishgames_kanji_KanjiGameLib_handleKeyEvent(JNIEnv * env, jobject obj, jobject lpCaller, int keycode, int down, int unicode, int eventSource, int deviceId);
   JNIEXPORT void JNICALL Java_com_bigfishgames_kanji_KanjiGameLib_handleAccelerometerEvent(JNIEnv * env, jobject obj, jobject lpCaller, float x, float y, float z);
   JNIEXPORT void JNICALL Java_com_bigfishgames_kanji_KanjiGameLib_handleFocusEvent(JNIEnv * env, jobject obj, jobject lpCaller, int nFocusGained);
   JNIEXPORT void JNICALL Java_com_bigfishgames_kanji_KanjiGameLib_handleSoundEvent(JNIEnv * env, jobject obj, jobject lpCaller, int nEventType, int nEventData);
   JNIEXPORT void JNICALL Java_com_bigfishgames_kanji_KanjiGameLib_handleUserEvent(JNIEnv * env, jobject obj, jobject lpCaller, int nEventType);
   JNIEXPORT void JNICALL Java_com_bigfishgames_kanji_KanjiGameLib_handleJoystickEvent(JNIEnv * env, jobject obj, jobject lpCaller, float x, float y, float z, float rx, float ry, float rz,
                                                                                       float hatx, float haty, float hscroll, float vscroll,
                                                                                       float brake, float gas, float rudder, float throttle, float tilt, float wheel,
                                                                                       int eventSource, int deviceId);
   JNIEXPORT int JNICALL Java_com_bigfishgames_kanji_KanjiGameLib_getOpenGLESVersion(JNIEnv * env, jobject obj, jobject lpCaller);
   JNIEXPORT void JNICALL Java_com_bigfishgames_kanji_KanjiGameLib_run(JNIEnv * env, jobject obj, jobject lpCaller);
};

JNIEXPORT void JNICALL Java_com_bigfishgames_kanji_KanjiGameLib_initWithAssetManager(JNIEnv* env, jobject obj, jobject assetManager) {
   androidJniInitWithAssetManager (env, obj, assetManager);
}

JNIEXPORT void JNICALL Java_com_bigfishgames_kanji_KanjiGameLib_surfaceCreated(JNIEnv * env, jobject obj, jobject lpCaller) {
   androidJniSurfaceCreated (env, obj, lpCaller, "com/bigfishgames/kanji");
}

JNIEXPORT void JNICALL Java_com_bigfishgames_kanji_KanjiGameLib_shutdown(JNIEnv * env, jobject obj, jobject lpCaller) {
   androidJniShutdown (env, obj, lpCaller);
}

JNIEXPORT void JNICALL Java_com_bigfishgames_kanji_KanjiGameLib_sizeChanged(JNIEnv * env, jobject obj, jobject lpCaller, int width, int height) {
   androidJniSizeChanged (env, obj, lpCaller, width, height);
}

JNIEXPORT void JNICALL Java_com_bigfishgames_kanji_KanjiGameLib_handleTouchEvent(JNIEnv * env, jobject obj, jobject lpCaller, int nPointerId, int nPointerCount, float x, float y, float z, int nAction) {
   androidJniHandleTouchEvent (env, obj, lpCaller, nPointerId, nPointerCount, x, y, z, nAction);
}

JNIEXPORT void JNICALL Java_com_bigfishgames_kanji_KanjiGameLib_handleKeyEvent(JNIEnv * env, jobject obj, jobject lpCaller, int keycode, int down, int unicode, int eventSource, int deviceId) {
   androidJniHandleKeyEvent (env, obj, lpCaller, keycode, down, unicode, eventSource, deviceId);
}

JNIEXPORT void JNICALL Java_com_bigfishgames_kanji_KanjiGameLib_handleAccelerometerEvent(JNIEnv * env, jobject obj, jobject lpCaller, float x, float y, float z) {
   androidJniHandleAccelerometerEvent (env, obj, lpCaller, x, y, z);
}

JNIEXPORT void JNICALL Java_com_bigfishgames_kanji_KanjiGameLib_handleFocusEvent(JNIEnv * env, jobject obj, jobject lpCaller, int nFocusGained) {
   androidJniHandleFocusEvent (env, obj, lpCaller, nFocusGained);
}

JNIEXPORT void JNICALL Java_com_bigfishgames_kanji_KanjiGameLib_handleSoundEvent(JNIEnv * env, jobject obj, jobject lpCaller, int nEventType, int nEventData) {
   androidJniHandleSoundEvent (env, obj, lpCaller, nEventType, nEventData);   
}

JNIEXPORT void JNICALL Java_com_bigfishgames_kanji_KanjiGameLib_handleUserEvent(JNIEnv * env, jobject obj, jobject lpCaller, int nEventType) {
   androidJniHandleUserEvent (env, obj, lpCaller, nEventType);
}

JNIEXPORT void JNICALL Java_com_bigfishgames_kanji_KanjiGameLib_handleJoystickEvent(JNIEnv * env, jobject obj, jobject lpCaller, float x, float y, float z, float rx, float ry, float rz,
                                                                                    float hatx, float haty, float hscroll, float vscroll,
                                                                                    float brake, float gas, float rudder, float throttle, float tilt, float wheel,
                                                                                    int eventSource, int deviceId) {
   androidJniHandleJoystickEvent (env, obj, lpCaller, x, y, z, rx, ry, rz,
                                  hatx, haty, hscroll, vscroll, brake, gas, rudder, throttle, tilt, wheel, eventSource, deviceId);
}

JNIEXPORT int JNICALL Java_com_bigfishgames_kanji_KanjiGameLib_getOpenGLESVersion(JNIEnv * env, jobject obj, jobject lpCaller) {
   return androidJniGetOpenGLESVersion (env, obj, lpCaller);
}

JNIEXPORT void JNICALL Java_com_bigfishgames_kanji_KanjiGameLib_run(JNIEnv * env, jobject obj, jobject lpCaller) {
   androidJniRun (env, obj, lpCaller);
}
