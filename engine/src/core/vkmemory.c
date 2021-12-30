#include "vkcmemory.h"

#include "logger.h"
#include "platform/platform.h"

//TODO: Custom string lib.
#include <string.h>
#include <stdio.h>

struct memory_stats {
    u64 total_allocated;
    u64 tagged_allocations[MEMORY_MAX_TAGS];
};

static const char* memory_tag_strings[MEMORY_MAX_TAGS] = {
    "UNKNOWN    ",
    "ARRAY      ",
    "DARRAY     ",
    "DICT       ",
    "RING_QUEUE ",
    "BST        ",
    "STRING     ",
    "APPLICATION",
    "JOB        ",
    "TEXTURE    ",
    "MAT_INST   ",
    "RENDERER   ",
    "GAME       ",
    "TRANFORM   ",
    "ENTITY     ",
    "ENITY_NODE ",
    "SCENE      "
};

static struct memory_stats stats;

void initialize_memory() {
    platform_memzero(&stats, sizeof(stats));
}

void shutdown_memory() {

}

void* vkalloc(u64 size, memory_tag tag) {
    if(tag == MEMORY_TAG_UNKNOWN) 
        VKCWARN("vkallocate called using MEMORY_TAG_UNKNOWN. Re-class the allocation!");

    stats.total_allocated += size;
    stats.tagged_allocations[tag] += size;

    //TODO: Memory alignment.
    void* block = platform_alloc(size, FALSE);
    platform_memzero(block, size);
    return block;
}

void vkfree(void* block, u64 size, memory_tag tag) {
    if(tag == MEMORY_TAG_UNKNOWN) 
            VKCWARN("vkfree called using MEMORY_TAG_UNKNOWN. Re-class the de-allocation!");

    stats.total_allocated -= size;
    stats.tagged_allocations[tag] -= size;

    platform_free(block, FALSE);
}

void* vkmemzero(void* block, u64 size) {
    return platform_memzero(block, size);
}

void* vkmemcpy(void* dest, const void* source, u64 size) {
    return platform_memcpy(dest, source, size);
}

void* vkmemset(void* dest, i32 value, u64 size) {
    return platform_memset(dest, value, size);
}

char* get_memory_usage_str() {
    const u64 kib = 1024;
    const u64 mib = 1024 * 1024;
    const u64 gib = 1024 * 1024 * 1024;

    char buffer[8000] = "System memory use (tagged):\n";
    u64 offset = strlen(buffer);
    for(int i = 0; i < MEMORY_MAX_TAGS; ++i) {
        char unit[4] = "XiB";
        float amount = 1.0f;

        if(stats.tagged_allocations[i] >= gib) {
            unit[0] = 'G';
            amount = stats.tagged_allocations[i] / (float)gib;
        } else if(stats.tagged_allocations[i] >= mib) {
            unit[0] = 'M';
            amount = stats.tagged_allocations[i] / (float)mib;
        } else if(stats.tagged_allocations[i] >= kib) {
            unit[0] = 'K';
            amount = stats.tagged_allocations[i] / (float)kib;
        } else {
            unit[0] = 'B';
            unit[1] = 0; // String terminator.
            amount = (float)stats.tagged_allocations[i];
        }

        i32 length = snprintf(buffer + offset, 8000, "%s: %.2f%s\n", memory_tag_strings[i], amount, unit);
        offset += length;
    }

    char* out_string = strdup(buffer);
    return out_string;
}