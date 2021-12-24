#include <core/logger.h>
#include <core/asserts.h>

#include <platform/platform.h>

int main(void) {
    VKCINFO("Hello from log system! %f", 3.1415f);

    VKCASSERT(1==1);

    platform_state state;
    if(platform_startup(&state, "Vulkan, ahi voy!", 100, 100, 1080, 720)) {
        while (TRUE)
        {
            platform_pump_message(&state);
        }
    }

    platform_shutdown(&state);
    return 0;
}