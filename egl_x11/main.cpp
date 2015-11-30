#include <cassert>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include "eglwrapper.hpp"

#include <EGL/egl.h>
#include <xcb/xcb.h>
#include <X11/Xlib-xcb.h>
#include <unistd.h>
#include <GLES2/gl2.h>
#include "triangleprogram.hpp"

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

int
main() {
    const char *x_display_name = NULL;
    Display *x_display;
    xcb_connection_t* x_connection;
    int x_screen;
    xcb_window_t x_window;
    setup_x(x_display_name,
            0, 0, // x, y
            64, 64, // width, height
            &x_display,
            &x_connection,
            &x_screen,
            &x_window);

    EGLWrapper egl(x_display, x_window);

    glClearColor(1.0, 1.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();

    TriangleProgram tr;
    for (int i = 0; ; i++) {
        glClear(GL_COLOR_BUFFER_BIT);
        tr.render();
        egl.swap();
        usleep(16);
    }


    return 0;
}
