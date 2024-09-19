#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/Xrandr.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Global variable to store the error flag
static int x_error_occurred = 0;

// Custom error handler
int handle_x_errors(Display *display, XErrorEvent *error_event) {
    if (error_event->error_code == BadWindow) {
        x_error_occurred = 1;
        return 0;
    }
    // else do default error handling
    return 1;  // Return 0 to ignore the error
}

Window find_window_by_name(Display *display, Window root, const char *window_name) {
    Window parent;
    Window *children;
    unsigned int num_children;
    char *window_name_ret = NULL;
    Window result = 0;

    // Query the tree for children
    if (XQueryTree(display, root, &root, &parent, &children, &num_children)) {
        for (unsigned int i = 0; i < num_children; i++) {
            // Set custom error handler
            x_error_occurred = 0;
            XSetErrorHandler(handle_x_errors);

            XWindowAttributes attrs;
            // Check if window is valid using XGetWindowAttributes
            // if it throws, skip it
            if (XGetWindowAttributes(display, children[i], &attrs) && !x_error_occurred) {
                if (XFetchName(display, children[i], &window_name_ret)) {
                    if (window_name_ret && strstr(window_name_ret, window_name) != NULL) {
                        XFree(window_name_ret);
                        result = children[i]; // Found the window
                        break;
                    }
                    XFree(window_name_ret);
                }

                // Recursive search in child windows
                result = find_window_by_name(display, children[i], window_name);
                if (result != 0) {
                    break;
                }
            } else if (x_error_occurred) {
                fprintf(stderr, "Skipped invalid window: 0x%lx\n", children[i]);
            }

            // Restore the default error handler
            XSetErrorHandler(NULL);
        }
        if (children != NULL) {
            XFree(children);
        }
    }

    return result;
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s window_name\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    Display *display = XOpenDisplay(NULL);
    if (display == NULL) {
        fprintf(stderr, "Unable to open X display\n");
        exit(EXIT_FAILURE);
    }

    int screen = DefaultScreen(display);
    Window root = DefaultRootWindow(display);

    int event_base, error_base;
    if (!XRRQueryExtension(display, &event_base, &error_base)) {
        fprintf(stderr, "XRandR extension not available\n");
        exit(EXIT_FAILURE);
    }

    XRRSelectInput(display, root, RRScreenChangeNotifyMask);

    Window window = find_window_by_name(display, root, argv[1]);
    if (window != 0) {
        // Resize the window to the full screen size
        int screen_width = XDisplayWidth(display, screen);
        int screen_height = XDisplayHeight(display, screen);
        XResizeWindow(display, window, screen_width, screen_height);
        XMoveWindow(display, window, 0, 0);
        XFlush(display);
    }

    XEvent event;
    while (1) {
        XNextEvent(display, &event);

        // Check if we received a screen change notify event
        if (event.type == event_base + RRScreenChangeNotify) {

            Window window = find_window_by_name(display, root, argv[1]);
            if (window != 0) {
                // Resize the window to the full screen size
                int screen_width = XDisplayWidth(display, screen);
                int screen_height = XDisplayHeight(display, screen);
                XResizeWindow(display, window, screen_width, screen_height);
                XMoveWindow(display, window, 0, 0);
                XFlush(display);
            }
        }
    }

    XCloseDisplay(display);

    return 0;
}
