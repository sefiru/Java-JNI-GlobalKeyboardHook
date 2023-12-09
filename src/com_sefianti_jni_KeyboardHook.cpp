#include "com_sefianti_jni_KeyboardHook.h"

#include <windows.h>

MSG msg;
HHOOK hook;
HANDLE thread;
JavaVM* jvm;
jobject gObject = NULL;
int running = 1;
JNIEnv* env;
jclass cls;
jmethodID keyDown;
jmethodID keyUp;

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if ( nCode >= 0 ) {
        KBDLLHOOKSTRUCT* keyInfo = (KBDLLHOOKSTRUCT*)lParam;
        
        if ( wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN ) {
            env->CallVoidMethod(gObject, keyDown, keyInfo->vkCode);
        }
        if ( wParam == WM_KEYUP || wParam == WM_SYSKEYUP ) {
            env->CallVoidMethod(gObject, keyUp, keyInfo->vkCode);
        }
        
    }

    return CallNextHookEx(NULL, nCode, wParam, lParam);
}


DWORD WINAPI threadFunc(void* data) {
    if (jvm->AttachCurrentThread((void**)&env, NULL) == JNI_OK) {
        hook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0);
        while ( running ) {
            while ( PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) ) {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
            Sleep(1);
        }
        jvm->DetachCurrentThread();
    }
    return 0;
}


JNIEXPORT void JNICALL Java_com_sefianti_jni_KeyboardHook_init
  (JNIEnv * env, jobject obj) {
    env->GetJavaVM(&jvm);
    gObject = env->NewGlobalRef(obj);
    cls = env->FindClass("com/sefianti/jni/KeyboardHook");
    keyDown = env->GetMethodID(cls, "dispatchKeyDown", "(I)V");
    keyUp = env->GetMethodID(cls, "dispatchKeyUp", "(I)V");
    thread = CreateThread(NULL, 0, threadFunc, NULL, 0, NULL);
}


JNIEXPORT void JNICALL Java_com_sefianti_jni_KeyboardHook_destroy
  (JNIEnv * env, jobject) {
    running = 0;
    CloseHandle(thread);
    env->DeleteGlobalRef(gObject);
    UnhookWindowsHookEx(hook);
  }