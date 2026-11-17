#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include <unistd.h>

/* This is my first time using X11
 *
 *
 * */

#define POSX    500
#define POSY    500
#define WIDTH   500
#define HEIGHT  500
#define BORDER  15

static Display* dpy;
static int scr;
static Window root;

// Makes it easy to quickly create a window
static Window create_win(int x, int y, int w, int h, int b) {
    Window win;
    XSetWindowAttributes xwa;

    xwa.background_pixel = WhitePixel(dpy, scr);
    xwa.border_pixel = BlackPixel(dpy, scr);
    xwa.event_mask = ButtonPress;

    win = XCreateWindow(dpy, root, x, y, w, h, b, DefaultDepth(dpy, scr), InputOutput, DefaultVisual(dpy, scr), CWBackPixel | CWBorderPixel | CWEventMask, &xwa);
    return win;
}

// Implements the run loop to clean up main 
static void run() {
    XEvent evt;

    while (XNextEvent(dpy, &evt) == 0) {
        // Switches between "event types"
        switch (evt.type) {
            case ButtonPress:
                return;
        }
    }
}

int main() {

    Window win;

    // Creating default display, screen, and root
    if ((dpy = XOpenDisplay(NULL)) == NULL) {
        printf("Could not open window");
        exit(1);
    }
    scr = DefaultScreen(dpy);
    root = RootWindow(dpy, scr);

    // Creating and mapping simple window
    win = create_win(POSX, POSY, WIDTH, HEIGHT, BORDER);
    XMapWindow(dpy, win);

    run();

    // Window cleanup
    XUnmapWindow(dpy, scr);
    XDestroyWindow(dpy, scr);
    XCloseDisplay(dpy);

    return 0;
}