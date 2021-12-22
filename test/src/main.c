#include <core/logger.h>
#include <core/asserts.h>

int main(void) {
    VKCINFO("Hello from log system! %f", 3.1415f);

    VKCASSERT(1==1);
    return 0;
}