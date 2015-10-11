#include <iostream>
#include <cstdint>
#define EGL_EGLEXT_PROTOTYPES
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES/gl.h>
//#include <gui/Surface.h>

using namespace std;
//using namespace android;

class X11Wrapper
{
public:
    X11Wrapper(uint32_t width, uint32_t height) {
        mDisplay = XOpenDisplay(NULL);
        if (not mDisplay) {
            cerr << "mDisplay initialization failed";
        }
        mRoot = DefaultRootWindow(mDisplay);
        XSetWindowAttributes swa;
        swa.event_mask = ExposureMask | PointerMotionMask | KeyPressMask;
        mWindow = XCreateWindow(mDisplay, mRoot,
                                0, 0, width, height,
                                0, CopyFromParent, InputOutput,
                                CopyFromParent, CWEventMask, &swa);
        XMapWindow(mDisplay, mWindow);
        XFlush(mDisplay);
    }

    Display* getDisplay() { return mDisplay; }
    Window getWindow() { return mWindow; }

    Pixmap mPixmap;
private:
    Display* mDisplay;
    Window mRoot;
    Window mWindow;
};

class X11Backend
{
public:
    X11Backend(uint32_t width, uint32_t height) {
        //LOGVP("W: %d H: %d", width, height);
        mWidth = width;
        mHeight = height;
        mDisplay = XOpenDisplay(NULL);
        if (not mDisplay) {
            //Exception(__FUNCTION__);
        }
        mRoot = DefaultRootWindow(mDisplay);
        mWindow = XCreateSimpleWindow(mDisplay, mRoot, 0, 0, mWidth, mHeight, 0, 0, 0);
        XMapWindow(mDisplay, mWindow);
        XFlush(mDisplay);
        mPixmap = XCreatePixmap(mDisplay, mWindow, mWidth, mHeight, sDepth);
        XGCValues gcvalues;
        mGraphicContext = XCreateGC(mDisplay, mPixmap, 0, &gcvalues);
    }

    Display* getDisplay() { return mDisplay; }
    Window getWindow() { return mWindow; }

    void drawBuffer(uint32_t* buffer) {
        XImage *img = XCreateImage(mDisplay, nullptr, sDepth, ZPixmap, 0, (char*)buffer,
                                   mWidth, mHeight, 32, 0);
        if (not img) {
            fprintf(stderr, "Creation failed");
            return;
        }
        XPutImage(mDisplay, mPixmap, mGraphicContext, img, 0, 0, 0, 0, mWidth, mHeight);
        XSetWindowBackgroundPixmap(mDisplay, mWindow, mPixmap);
        XClearWindow(mDisplay, mWindow);
        XFlush(mDisplay);
        XDestroyImage(img);
    }

private:
    static constexpr int sDepth = 24;
    Pixmap mPixmap;
    Display* mDisplay;
    Window mRoot;
    Window mWindow;
    GC mGraphicContext;
    uint32_t mWidth;
    uint32_t mHeight;

};

int main()
{
    X11Backend win(600, 600);
    //Surface* s = new Surface(nullptr, false);
//    static const EGLint attribs[] = {
//       EGL_RED_SIZE, 8,
//       EGL_GREEN_SIZE, 8,
//       EGL_BLUE_SIZE, 8,
////       EGL_DOUBLEBUFFER,
//       EGL_DEPTH_SIZE, 8,
//       EGL_NONE
//    };

//    EGLConfig config;
//    EGLint num_configs, vid;

//    EGLDisplay dpy = eglGetDisplay(win.getDisplay());
//    eglInitialize(dpy, 0, 0);

//    if (!eglChooseConfig( dpy, attribs, &config, 1, &num_configs)) {
//       printf("Error: couldn't get an EGL visual config\n");
//       exit(1);
//    }

//    if (!eglGetConfigAttrib(dpy, config, EGL_NATIVE_VISUAL_ID, &vid)) {
//       printf("Error: eglGetConfigAttrib() failed\n");
//       exit(1);
//    }

//    EGLContext ctx = eglCreateContext(dpy, config, EGL_NO_CONTEXT, NULL);

//    EGLSurface  surface = eglCreateWindowSurface(dpy, config, win.getWindow(), NULL);

//    eglMakeCurrent(dpy, surface, surface, ctx);

//    win.mPixmap = XCreatePixmap(win.getDisplay(), win.getWindow(),
//                                600, 600, 8);
    //XFillRectangle(win.getDisplay(), win.mPixmap, 0xEFEFEF, 0, 0, 600, 600);

//    EGLImageKHR eglImage = eglCreateImageKHR(dpy, ctx, EGL_NATIVE_PIXMAP_KHR, (EGLClientBuffer)win.mPixmap, NULL);
//    if (eglImage == EGL_NO_IMAGE_KHR) {
//        printf("Wtf");
//    }

    //glEGLImageTargetTexture2D(GL_TEXTURE_2D, eglImage);

    uint32_t size = 600 * 600;
    uint32_t* b = new uint32_t[size];

    while(size--) {
        b[size] = 0xFFBB00FF;
    }
    win.drawBuffer(b);
    for(;;) {

//      XEvent e;
//      XNextEvent(win.getDisplay(), &e);
//      if (e.type == MapNotify)
//        break;
//      win.drawBuffer(b);
      //eglSwapBuffers(dpy, surface);
    }

    return 0;
}

