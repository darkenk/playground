#include <cassert>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include "eglwrapper.hpp"

#include <EGL/egl.h>
#include <xcb/xcb.h>
#include <xcb/xcb_icccm.h>
#include <X11/Xlib-xcb.h>
#include <unistd.h>
#include <GLES2/gl2.h>
#include "triangleprogram.hpp"

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

using namespace std;

void
error_fatal(const char* format, ...) {
    printf("error: ");

    va_list va;
    va_start(va, format);
    vprintf(format, va);
    va_end(va);

    printf("\n");
    exit(1);
}

const uint32_t xcb_window_attrib_mask = XCB_CW_EVENT_MASK;
const uint32_t xcb_window_attrib_list[] = {
    XCB_EVENT_MASK_BUTTON_PRESS,
    XCB_EVENT_MASK_EXPOSURE,
    XCB_EVENT_MASK_KEY_PRESS,
};

void
setup_x(const char *display_name,
        int window_x,
        int window_y,
        int window_width,
        int window_height,
        Display** out_display,
        xcb_connection_t** out_connection,
        int* out_screen,
        xcb_window_t* out_window) {

    xcb_generic_error_t* error;

    Display* display = XOpenDisplay(display_name);
    if (!display)
        error_fatal("XOpenDisplay() failed");

    xcb_connection_t *connection = XGetXCBConnection(display);
    if (!connection)
        error_fatal("XGetXCBConnection() failed");
    if (xcb_connection_has_error(connection))
        error_fatal("errors occured in connecting to X server");

    const xcb_setup_t* setup = xcb_get_setup(connection);
    xcb_screen_t* screen = xcb_setup_roots_iterator(setup).data;
    assert(screen != 0);

    xcb_window_t window = xcb_generate_id(connection);
    if (window <= 0)
        error_fatal("failed to generate X window id");

    xcb_void_cookie_t create_cookie = xcb_create_window_checked(
        connection,
        XCB_COPY_FROM_PARENT, // depth
        window,
        screen->root, // parent window
        window_x,
        window_y,
        window_width,
        window_height,
        0, // border width
        XCB_WINDOW_CLASS_INPUT_OUTPUT, // class
        screen->root_visual, // visual
        xcb_window_attrib_mask,
        xcb_window_attrib_list);

    xcb_size_hints_t hints;

    xcb_icccm_size_hints_set_min_size(&hints, window_width, window_height);
    xcb_icccm_size_hints_set_max_size(&hints, window_width, window_height);

    xcb_icccm_set_wm_size_hints(connection, window, XCB_ATOM_WM_NORMAL_HINTS, &hints);

//    XSizeHints* sizeHints = XAllocSizeHints();
//    sizeHints->flags = PMinSize | PMaxSize;
//    sizeHints->min_width = sizeHints->max_width = static_cast<int>(window_x);
//    sizeHints->min_height = sizeHints->max_height = static_cast<int>(window_y);
//    XSetWMNormalHints(display, window, sizeHints);
//    XFree(sizeHints);

    xcb_void_cookie_t map_cookie = xcb_map_window_checked(connection, window);

    // Check errors.
    error = xcb_request_check(connection, create_cookie);
    if (error)
        error_fatal("failed to create X window: %d", error->error_code);
    error = xcb_request_check(connection, map_cookie);
    if (error)
        error_fatal("failed to map X window: %d", error->error_code);

    *out_display = display;
    *out_connection = connection;
    *out_window = window;
}

#include "drawablefactory.hpp"
#include "quaddrawable.hpp"
#include "quadtexture.hpp"
#include <stack>

class Drawer : public Node<QuadDrawable>::Visitor {
public:
    Drawer(const glm::mat4& viewProj): mViewProj(viewProj), mRef(0) {
        glEnable(GL_STENCIL_TEST);
        //mLevels.push(mRef);
        glStencilFunc(GL_EQUAL, mRef, 0xFF);
        glStencilOp(GL_KEEP, GL_INCR, GL_INCR);
        mDrawZ = 0.f;
    }
    virtual void beforeGoingDown(QuadDrawable& q) {
        glStencilOp(GL_KEEP, GL_INCR, GL_INCR);
        q.setViewProj(mViewProj);
        q.updateModel();
        q.draw();
        mDrawZ += (1.f /256.f);
        mZets.push(mDrawZ);
        //glDepthFunc(GL_EQUAL);
        fprintf(stderr, "Down id: %d mRef %d\n", q.id(), mRef);
        mRef++;
        glStencilFunc(GL_EQUAL, mRef, 0xFF);
        //glStencilFunc(GL_EQUAL, 0, 0xFF);
        mLevels.push(mRef);
    };
    virtual void beforeGoingUp(QuadDrawable& q) {
        glStencilOp(GL_KEEP, GL_DECR, GL_DECR);
        glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
        q.draw();
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        GLint r = --mRef;
        glStencilFunc(GL_EQUAL, r, 0xFF);
        fprintf(stderr, "Up id: %d mRef %d\n", q.id(), mRef);
    };

private:
    glm::mat4 mViewProj;
    GLint mRef;
    std::stack<GLint> mLevels;

    GLint mDrawZ;
    std::stack<GLint> mZets;
};

int
main() {
    const char *x_display_name = NULL;
    Display *x_display;
    xcb_connection_t* x_connection;
    int x_screen;
    xcb_window_t x_window;
    const int x = 0;
    const int y = 0;
    const int width = 512;
    const int height = 256;
    setup_x(x_display_name,
            x, y, // x, y
            width, height, // width, height
            &x_display,
            &x_connection,
            &x_screen,
            &x_window);

    EGLWrapper egl(x_display, x_window);
    glViewport(0, 0, width, height);
    DrawableFactory df;
    Node<QuadDrawable> rootNode();
    auto p = df.createDrawable<QuadDrawable>(glm::vec4(1.0, 0.0, 0.0, 1.0f));
    p->setPosition(glm::vec3(12, 100, 10.0));
//    rootNode.addChild(p);

    p = df.createDrawable<QuadDrawable>(glm::vec4(0.0, 1.0, 0.0, 1.0f));
    p->setPosition(glm::vec3(19, 115, 10.0));
    //rootNode.addChild(p);

    auto n = df.createDrawable<QuadDrawable>(glm::vec4(0.0, 0.0, 1.0, 1.0f));
    n->setPosition(glm::vec3(40, 105, 10.0));
    p->addChild(n);

    auto s = df.createDrawable<QuadTexture>(glm::vec4(0.0, 1.0, 0.0, 1.0f));
    s->setPosition(glm::vec3(0, 200, 10.0));
    //rootNode.addChild(p);

    glClearColor(1.0, 1.0, 0.0, 1.0);
    glEnable(GL_STENCIL_TEST);
    glClearStencil(0x00);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glEnable(GL_BLEND);
    //glEnable(GL_DEPTH_TEST);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

    auto view = glm::ortho<float>(width, x, height, y, 1.0f, 100.f);
    auto proj = glm::lookAt(glm::vec3(0.f, 0.f, -1.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
//    Drawer d(proj * view);
//    rootNode.traverse(d);
    s->setViewProj(proj * view);
    s->updateModel();
    s->draw();
    egl.swap();
    usleep(1600000);


    return 0;
}
