#pragma once

#include "defines.h"

typedef enum memory_tag {
    MEMORY_TAG_UNKNOWN,
    MEMORY_TAG_ARRAY,
    MEMORY_TAG_DARRAY,
    MEMORY_TAG_DICT,
    MEMORY_TAG_RING_QUEUE,
    MEMORY_TAG_BST,
    MEMORY_TAG_STRING,
    MEMORY_TAG_APPLICATION,
    MEMORY_TAG_JOB,
    MEMORY_TAG_TEXTURE,
    MEMORY_TAG_MATERIAL_INSTANCE,
    MEMORY_TAG_RENDERER,
    MEMORY_TAG_GAME,
    MEMORY_TAG_TRANFORM,
    MEMORY_TAG_ENTITY,
    MEMORY_TAG_ENITY_NODE,
    MEMORY_TAG_SCENE,
    MEMORY_MAX_TAGS
} memory_tag;

VKCAPI void initialize_memory();
VKCAPI void shutdown_memory();

VKCAPI void* vkalloc(u64 size, memory_tag tag);
VKCAPI void vkfree(void* block, u64 size, memory_tag tag);
VKCAPI void* vkmemzero(void* block, u64 size);
VKCAPI void* vkmemcpy(void* dest, const void* source, u64 size);
VKCAPI void* vkmemset(void* dest, i32 value, u64 size);
VKCAPI char* get_memory_usage_str();