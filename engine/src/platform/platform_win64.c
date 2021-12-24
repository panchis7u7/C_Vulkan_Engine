#include "platform.h"

// Windows platform layer.
#if WINDOWS_PLATFORM

#include "core\logger.h"

#include <windows.h>
#include <windowsx.h>
#include <stdlib.h>

/////////////////////////////////////////////////////
// Global Variables.

typedef struct internal_state {
    HINSTANCE h_instance;
    HWND hwnd;
} internal_state;

// Clock.
static f64 clock_frequency;
static LARGE_INTEGER start_time;

/////////////////////////////////////////////////////
// Callback functions.

LRESULT CALLBACK win_process_message(HWND hwnd, u32 msg, WPARAM w_param, LPARAM l_param);


/////////////////////////////////////////////////////
// Function definitions.

//-------------------------------------------------------------------------------------------------------------
bool8 platform_startup(platform_state* plat_state, const char* app_name, i32 x, i32 y, i32 width, i32 height) {
    plat_state->internal_state = malloc(sizeof(internal_state));
    internal_state* state = (internal_state *)plat_state->internal_state;

    state->h_instance = GetModuleHandleA(0);

    // Setup and regster window class.
    HICON icon = LoadIcon(state->h_instance, IDI_APPLICATION);
    WNDCLASSA wc;
    memset(&wc, 0, sizeof(wc));
    wc.style = CS_DBLCLKS;  //Get double clicks.
    wc.lpfnWndProc = win_process_message;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = state->h_instance;
    wc.hIcon = icon;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = NULL;
    wc.lpszClassName = "Vulkan_Window_Class";

    if(!RegisterClassA(&wc)){
        MessageBoxA(0, "Window registation failed", "Error", MB_ICONEXCLAMATION | MB_OK);    
        return FALSE;
    }

    // Client area.
    u32 client_x = x;
    u32 client_y = y;
    u32 client_width = width;
    u32 client_height = height;

    // Window area.
    u32 window_x = client_x;
    u32 window_y = client_y;
    u32 window_width = client_width;
    u32 window_height = client_height;

    u32 window_style = WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION;
    u32 window_ex_style = WS_EX_APPWINDOW;

    // Describe how the window looks and the individual botton that composes it.
    window_style |= WS_MAXIMIZEBOX;
    window_style |= WS_MINIMIZEBOX;
    window_style |= WS_THICKFRAME;

    // Obtain the size of the border.
    RECT border_rect = { 0, 0, 0, 0 };
    AdjustWindowRectEx(&border_rect, window_style, 0, window_ex_style);

    // Negative border rectangle.
    window_x += border_rect.left;
    window_y += border_rect.top;

    // Grow by the size of the OS border.
    window_width += border_rect.right - border_rect.left;
    window_height += border_rect.bottom - border_rect.top;

    HWND handle = CreateWindowEx( window_ex_style, "Vulkan_Window_Class", app_name,
        window_style, window_x, window_y, window_width, window_height, 0, 0, state->h_instance, 0);

    if(handle == 0) {
        MessageBoxA(NULL, "Window creation failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        VKCFATAL("Window creation failed!");
        return FALSE;
    } else {
        state->hwnd = handle;
    }

    // Show the window.
    bool32 should_activate = TRUE;
    i32 show_window_command_flags = should_activate ? SW_SHOW : SW_SHOWNOACTIVATE;
    ShowWindow(state->hwnd, show_window_command_flags);

    // Clock setup.
    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);
    clock_frequency = 1.0 / (f64)frequency.QuadPart;
    QueryPerformanceCounter(&start_time);

    return TRUE;
}

//-------------------------------------------------------------------------------------------------------------
void platform_shutdown(platform_state* plat_state) {
    internal_state* state = (internal_state *)plat_state->internal_state;

    if(state->hwnd) {
        DestroyWindow(state->hwnd);
        state->hwnd = 0;
    }
}

//-------------------------------------------------------------------------------------------------------------
bool8 platform_pump_message(platform_state* plat_state) {
    MSG message;
    while (PeekMessageA(&message, NULL, 0, 0, PM_REMOVE)) {
        TranslateMessage(&message);
        DispatchMessageA(&message);
    }

    return TRUE;
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
    HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    // FATAL, ERROR, WARN, INFO, DEBUG, TRACE.
    static u8 levels[6] = { 64, 4, 6, 2, 1, 8 };
    SetConsoleTextAttribute(console_handle, levels[colour]);

    OutputDebugStringA(message);
    u64 length = strlen(message);
    LPDWORD numberr_written = 0;
    WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), message, (DWORD)length, numberr_written, 0);
}

//-------------------------------------------------------------------------------------------------------------
void platform_console_write_error(const char* message, u8 colour) {
    HANDLE console_handle = GetStdHandle(STD_ERROR_HANDLE);
    // FATAL, ERROR, WARN, INFO, DEBUG, TRACE.
    static u8 levels[6] = { 64, 4, 6, 2, 1, 8 };
    SetConsoleTextAttribute(console_handle, levels[colour]);

    OutputDebugStringA(message);
    u64 length = strlen(message);
    LPDWORD numberr_written = 0;
    WriteConsoleA(GetStdHandle(STD_ERROR_HANDLE), message, (DWORD)length, numberr_written, 0);
}

//-------------------------------------------------------------------------------------------------------------
f64 platform_get_absolute_time() {
    LARGE_INTEGER now_time;
    QueryPerformanceCounter(&now_time);
    return (f64)now_time.QuadPart * clock_frequency;
}

//------------------------------------------------------------------------------------------------------------- 
void platform_sleep(u64 ms) {
    Sleep(ms);
}

//-------------------------------------------------------------------------------------------------------------
LRESULT CALLBACK win_process_message(HWND hwnd, u32 msg, WPARAM w_param, LPARAM l_param) {
    switch (msg) {
        case WM_ERASEBKGND:
            // Notify the OS that erassing wll be handled by the application to prevent flicker.
            return 1;
        case WM_CLOSE:
            // TODO: Fire an event for the application to quit.
            exit(0);
            return 0;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        case WM_SIZE: {
            // Get the updated size of the window.
            // RECT r;
            // GetClientRect(hwnd, &r);
            // u32 width = r.right - r.left;
            // u32 height = r.bottom - r.top;

            // TODO: Fire an event for the window resize.
        } break;
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
        case WM_KEYUP:
        case WM_SYSKEYUP: {
            // Key pressed/released.
            // bool8 pressed = (msg == WM_KEYDOWN || WM_SYSKEYDOWN);
            // TODO: Input processing. 
        } break;
        case WM_MOUSEMOVE: {
            // Mouse move.
            // i32 x_position = GET_X_LPARAM(l_param);
            // i32 y_position = GET_Y_LPARAM(l_param);
            // TODO: Input processing. 
        } break;
        case WM_MOUSEHWHEEL: {
            // i32 z_delta = GET_WHEEL_DELTA_WPARAM(w_param);
            // if(z_delta != 0) {
            //     // Flatten input to OS independent (-1, 1).
            //     z_delta = (z_delta < 0) ? -1 : 1;
            // }
            // TODO: Input processing.
        } break;
        case WM_LBUTTONDOWN:
        case WM_MBUTTONDOWN:
        case WM_RBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_MBUTTONUP:
        case WM_RBUTTONUP: {
            // bool8 pressed = msg == WM_LBUTTONDOWN || msg == WM_RBUTTONDOWN || msg == WM_MBUTTONDOWN;
            // TODO: Input processing.
        } break;
    }

    return DefWindowProcA(hwnd, msg, w_param, l_param);
}

#endif // WINDOWS_PLATFORM