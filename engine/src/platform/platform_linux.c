#include "platform.h"

// Linx platform layer.
#if LINUX_PLATFORM

#include "core/logger.h"

#include <xcb/xcb.h>
#include <X11/keysym.h>
#include <X11/XKBlib.h>
#include <X11/Xlib.h>
#include <X11/Xlib-xcb.h>
#include <sys/time.h>
#include <stdlib.h>
#include <string.h>

#if _POSIX_C_SOURCE >= 199309L
#include <time.h>
#else
#include <unistd.h>
#endif

/////////////////////////////////////////////////////
// Global Variables.

typedef struct internal_state {
    Display* display;
    xcb_connection_t* connection;
    xcb_window_t window;
    xcb_screen_t* screen;
    xcb_atom_t wm_protocols;
    xcb_atom_t wm_delete_win;
} internal_state;

// Clock.


/////////////////////////////////////////////////////
// Callback functions.


/////////////////////////////////////////////////////
// Function definitions.

//-------------------------------------------------------------------------------------------------------------
bool8 platform_startup(platform_state* plat_state, const char* app_name, i32 x, i32 y, i32 width, i32 height) {
    plat_state->internal_state = malloc(sizeof(internal_state));
    internal_state* state = (internal_state*)plat_state->internal_state;

    // Connect to X-Server.
    state->display = XOpenDisplay(NULL);

    // Turn off key repeats.
    XAutoRepeatOff(state->display);

    // Retrrieve the connection from the display.
    state->connection = XGetXCBConnection(state->display);

    if(xcb_connection_has_error(state->connection)) {
        VKCFATAL("Failed to connect to X server via XCB.");
        return FALSE;
    }

    // Get setup data from the X-Server.
    const struct xcb_setup_t* setup = xcb_get_setup(state->connection);

    // Loop through all the available screens using a iterator.
    xcb_screen_iterator_t it = xcb_setup_roots_iterator(setup);
    int screen_p = 0;

    for(i32 s = screen_p; s > 0; s--)
        xcb_screen_next(&it);

    // After screen have been looped through, assign it.
    state->screen = it.data;

    // Allocate a XID for the window to be created.
    state->window = xcb_generate_id(state->connection);

    // Register event types.
    u32 event_mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;

    // Listen for keyboard and mouse buttons.
    u32 event_values =  XCB_EVENT_MASK_BUTTON_PRESS | XCB_EVENT_MASK_BUTTON_RELEASE |
                        XCB_EVENT_MASK_KEY_PRESS | XCB_EVENT_MASK_KEY_RELEASE |
                        XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_POINTER_MOTION |
                        XCB_EVENT_MASK_STRUCTURE_NOTIFY;

    // Values to be sent over XCB (background colour, events).
    u32 value_list[] = { state->screen->black_pixel, event_values };

    // Create window.
    xcb_void_cookie_t cookie = xcb_create_window(
        state->connection, XCB_COPY_FROM_PARENT,
        state->window, state->screen->root,
        x, y, width, height, 0,
        XCB_WINDOW_CLASS_INPUT_OUTPUT, state->screen->root_visual,
        event_mask, value_list
    );

    // Change the title of the window.
    xcb_change_property(
        state->connection, XCB_PROP_MODE_REPLACE,
        state->window, XCB_ATOM_WM_NAME,
        XCB_ATOM_STRING, 8, strlen(app_name), app_name
    );

    // Tell the server to notify when the window manager 
    xcb_intern_atom_cookie_t wm_delete_cookie = xcb_intern_atom(
        state->connection, 0, strlen("WM_DELETE_WINDOW"), "WM_DELETE_WINDOW"
    );

    xcb_intern_atom_cookie_t wm_protocols_cookie = xcb_intern_atom(
        state->connection, 0, strlen("WM_PROTOCOLS"), "WM_PROTOCOLS"
    );

    xcb_intern_atom_reply_t* wm_delete_reply = xcb_intern_atom_reply(
        state->connection, wm_delete_cookie, NULL
    );

    xcb_intern_atom_reply_t* wm_protocols_reply = xcb_intern_atom_reply(
        state->connection, wm_protocols_cookie, NULL
    );

    state->wm_delete_win = wm_delete_reply->atom;
    state->wm_protocols = wm_protocols_reply->atom;

    xcb_change_property(
        state->connection, XCB_PROP_MODE_REPLACE,
        state->window, wm_protocols_reply->atom,
        4, 32, 1, &wm_delete_reply->atom 
    );

    // Map the window to the screen. 
    xcb_map_window(state->connection, state->window);

    // Flush the stream.
    i32 stream_result = xcb_flush(state->connection);
    if(stream_result <= 0) {
        VKCFATAL("An error ocurred when attempting to flush the stream: %d", stream_result);
        return FALSE;
    }

    return TRUE;
}

//-------------------------------------------------------------------------------------------------------------
void platform_shutdown(platform_state* plat_state) {
    internal_state* state = (internal_state*)plat_state->internal_state;

    // Turn key repeats on since this is global for the OS.
    XAutoRepeatOn(state->display);

    xcb_destroy_window(state->connection, state->window);
}

//-------------------------------------------------------------------------------------------------------------
bool8 platform_pump_message(platform_state* plat_state) {
    
}

//-------------------------------------------------------------------------------------------------------------
void* platform_alloc(u64 size, bool8 aligned) {
    return malloc(size);
}

//-------------------------------------------------------------------------------------------------------------
void platform_free(void* block, bool8 aligned) {
    free(block);
}

//-------------------------------------------------------------------------------------------------------------
void* platform_zero_memory(void* block, u64 size) {
    return memset(block, 0, size);
}

//-------------------------------------------------------------------------------------------------------------
void* platform_copy_memory(void* dest, const void* source, u64 size) {
    return memcpy(dest, source, size);
}

//-------------------------------------------------------------------------------------------------------------
void* platform_set_memory(void* dest, i32 value, u64 size) {
    return memset(dest, value, size);
}

//-------------------------------------------------------------------------------------------------------------
void platform_console_write(const char* message, u8 colour) {
    
}

//-------------------------------------------------------------------------------------------------------------
void platform_console_write_error(const char* message, u8 colour) {
    
}

//-------------------------------------------------------------------------------------------------------------
f64 platform_get_absolute_time() {

}

//------------------------------------------------------------------------------------------------------------- 
void platform_sleep(u64 ms) {

}

#endif //LINUX_PLATFORM