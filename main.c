#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include <unistd.h>
#include "generator.c"

/* This is my first time using X11
 *
 *
 * */

#define POSX    500
#define POSY    500
#define WIDTH   1000
#define HEIGHT  1000
#define BORDER  15

static Display* dpy;
static int scr;
static Window root;
static Window win;

// Sets the color so that .pixel can be used
static XColor set_color(char color_code[8]) {
    XColor color;
    Colormap colormap;
    colormap = DefaultColormap(dpy, scr);
    XParseColor(dpy, colormap, color_code, &color);
    XAllocColor(dpy, colormap, &color);
    return color;
}

// Makes it easy to quickly create a window
static Window create_win(int x, int y, int w, int h, int b) {
    Window win;
    XSetWindowAttributes xwa;

    xwa.background_pixel = set_color("#515151").pixel;
    xwa.border_pixel = BlackPixel(dpy, scr);
    // Each event that can occur (use event mask when subscribing here)
    xwa.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask;

    win = XCreateWindow(dpy, root, x, y, w, h, b, DefaultDepth(dpy, scr), InputOutput, DefaultVisual(dpy, scr), CWBackPixel | CWBorderPixel | CWEventMask, &xwa);
    return win;
}

// Implements the run loop to clean up main 
static void run(GC gc) {
    XEvent evt;

    while (XNextEvent(dpy, &evt) == 0) {
        // Switches between "event types" (use event not the event mask)
        switch (evt.type) {
            case Expose:
                char welcome[] = "Welcome to Keenan's NIST-Compliant Passphrase Generator!";
                XDrawString(dpy, win, gc, 20, 30, welcome, strlen(welcome));
                char directions[] = "Click anywhere to generate a passphrase! Press any key to copy the passphrase!";
                XDrawString(dpy, win, gc, 20, 30, directions, strlen(directions));
                
                
                break;
            case KeyPress:

                return;
            case ButtonPress:

                break;
        }
    }
}

int main() {
    // Creating default display, screen, and root
    if ((dpy = XOpenDisplay(NULL)) == NULL) {
        printf("Could not open window");
        exit(1);
    }
    scr = DefaultScreen(dpy);
    root = RootWindow(dpy, scr);

    // Creating and mapping simple window
    win = create_win(POSX, POSY, WIDTH, HEIGHT, BORDER);

    GC gc = XCreateGC(dpy, win, 0, NULL);

    XFontStruct *font = XLoadQueryFont(dpy, "9x15");
    XSetFont(dpy, gc, font->fid);
    XSetForeground(dpy, gc, set_color("#B6B6B6").pixel);

    XMapWindow(dpy, win);

    run(gc);

    // Window cleanup
    XUnmapWindow(dpy, win);
    XDestroyWindow(dpy, win);
    XCloseDisplay(dpy);

    return 0;
}