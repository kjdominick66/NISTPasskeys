#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include <unistd.h>
#include "generator.h"

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
    char welcome[] = "Welcome to Keenan's NIST-Compliant Passphrase Generator!";
    char directions[] = "Click anywhere to generate a passphrase!";
    char copy_directions[] = "Press any key to copy the passphrase!";
    char copied[] = "Copied passphrase to clipboard!";
    char passphrase[36] = "";
    while (XNextEvent(dpy, &evt) == 0) {
        // Switches between "event types" (use event not the event mask)
        switch (evt.type) {
            case Expose:
                XDrawString(dpy, win, gc, 20, 30, welcome, strlen(welcome));
                XDrawString(dpy, win, gc, 20, 80, directions, strlen(directions));
                if (passphrase[0] != '\0') {
                    XDrawString(dpy, win, gc, 20, 130, passphrase, strlen(passphrase));
                    XDrawString(dpy, win, gc, 20, 180, copy_directions, strlen(copy_directions));
                }
                
                break;
            case KeyPress:
                // Open a pipe to clipboard to copy
                FILE *inFile = popen("./xclip -selection clipboard", "w");
                if (inFile != NULL) {
                    fputs(passphrase, inFile);
                    pclose(inFile);
                }
                XClearArea(dpy, win, 0, 0, 0, 0, False);
                XDrawString(dpy, win, gc, 20, 30, welcome, strlen(welcome));
                XDrawString(dpy, win, gc, 20, 80, directions, strlen(directions));
                XDrawString(dpy, win, gc, 20, 130, passphrase, strlen(passphrase));
                XDrawString(dpy, win, gc, 20, 180, copied, strlen(copied));
                break;
            case ButtonPress:
                XClearArea(dpy, win, 0, 0, 0, 0, False);
                generate_passphrase(passphrase);
                XDrawString(dpy, win, gc, 20, 30, welcome, strlen(welcome));
                XDrawString(dpy, win, gc, 20, 80, directions, strlen(directions));
                XDrawString(dpy, win, gc, 20, 130, passphrase, strlen(passphrase));
                XDrawString(dpy, win, gc, 20, 180, copy_directions, strlen(copy_directions));
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

    XFontStruct *font = XLoadQueryFont(dpy, "12x24");
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