#include "eglwrapper.hpp"
#include <cstdio>
#include <exception>

const EGLint EGLWrapper::sConfigAttribs[] = {
    EGL_COLOR_BUFFER_TYPE,     EGL_RGB_BUFFER,
    EGL_BUFFER_SIZE,           32,
    EGL_RED_SIZE,              8,
    EGL_GREEN_SIZE,            8,
    EGL_BLUE_SIZE,             8,
    EGL_ALPHA_SIZE,            8,

    EGL_DEPTH_SIZE,            24,
    EGL_STENCIL_SIZE,          8,

    EGL_SAMPLE_BUFFERS,        0,
    EGL_SAMPLES,               0,

    EGL_SURFACE_TYPE,          EGL_WINDOW_BIT,
    EGL_RENDERABLE_TYPE,       EGL_OPENGL_ES2_BIT,

    EGL_NONE,
};

const EGLint EGLWrapper::sContextAttribs[] = {
    EGL_CONTEXT_CLIENT_VERSION, 2,
    EGL_NONE,
};

const EGLint EGLWrapper::sSurfaceAttribs[] = {
       EGL_RENDER_BUFFER, EGL_BACK_BUFFER,
       EGL_NONE,
   };

EGLWrapper::EGLWrapper(EGLNativeDisplayType display, EGLNativeWindowType window)
{
    if (not eglBindAPI(EGL_OPENGL_ES_API)) {
        printf("Cannot bind to GLES API");
        throw std::exception();
    }
    mDisplay = eglGetDisplay(display);
    if (mDisplay == EGL_NO_DISPLAY) {
        printf("EGL_NO_DISPLAY");
        throw std::exception();
    }
    if (not eglInitialize(mDisplay, nullptr, nullptr) ){
        printf("Cannot initialize egl");
        throw std::exception();
    }
    EGLint configs_size = 256;
    EGLConfig* configs = new EGLConfig[configs_size];
    EGLint numConfigs = 0;
    eglChooseConfig(mDisplay, sConfigAttribs, configs, configs_size, &numConfigs);
    if (numConfigs == 0) {
        printf("Failed to get config");
        throw std::exception();
    }
    EGLConfig config = configs[0];
    delete[] configs;
    mContext = eglCreateContext(mDisplay, config, EGL_NO_CONTEXT, sContextAttribs);
    if (not mContext) {
        printf("Can't create context");
        throw std::exception();
    }
    mSurface = eglCreateWindowSurface(mDisplay, config, window, sSurfaceAttribs);
    eglMakeCurrent(mDisplay, mSurface, mSurface, mContext);
}

void EGLWrapper::swap()
{
    eglSwapBuffers(mDisplay, mSurface);
}

