#ifndef EGLWRAPPER_HPP
#define EGLWRAPPER_HPP

#include <EGL/egl.h>

class EGLWrapper
{
public:
    EGLWrapper(EGLNativeDisplayType display, EGLNativeWindowType window);
    void swap();

private:
    static const EGLint sConfigAttribs[];
    static const EGLint sContextAttribs[];
    static const EGLint sSurfaceAttribs[];
    EGLDisplay mDisplay;
    EGLContext mContext;
    EGLSurface mSurface;
};

#endif // EGLWRAPPER_HPP
