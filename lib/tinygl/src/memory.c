/*
 * Memory allocator for TinyGL
 * ПРАВКА PDA2: PSRAM на ESP32 через heap_caps_malloc, обычный malloc на симуляторе.
 */

#include <stdlib.h>
#include <string.h>
#include "zgl.h"

#ifdef ESP_PLATFORM
#include <esp_heap_caps.h>
#endif

void gl_free(void *p)
{
    free(p);
}

void *gl_malloc(GLint size)
{
#ifdef ESP_PLATFORM
    return heap_caps_malloc(size, MALLOC_CAP_SPIRAM);
#else
    return malloc(size); // симулятор: PSRAM как понятия нет
#endif
}

void *gl_zalloc(GLint size)
{
    void *p = gl_malloc(size);
    if (p) memset(p, 0, size);
    return p;
}