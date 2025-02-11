#include <stdint.h>
#include <stddef.h>
#include "../apps/echo.h"
#include "../drivers/vga/vgahandler.h"

void *flop_memset(void *dest, int value, size_t size) {
    uint8_t val = (uint8_t)value;
    __asm__ volatile (
        "cld\n\t"
        "rep stosb"
        : "+D" (dest), "+c" (size)
        : "a" (val)
        : "memory"
    );
    return dest;
}


int flop_memcmp(const void *ptr1, const void *ptr2, size_t num) {
    if (!ptr1 || !ptr2) {
        echo("flop_memcmp: NULL pointer detected!\n", RED);
        return -1;
    }

    const uint8_t *p1 = (const uint8_t *)ptr1;
    const uint8_t *p2 = (const uint8_t *)ptr2;

    for (size_t i = 0; i < num; i++) {
        if (p1[i] != p2[i]) {
            return (p1[i] - p2[i]);
        }
    }
    return 0;
}



void *flop_memcpy(void *dest, const void *src, size_t n) {
    if (!dest || !src) {
        echo("flop_memcpy: NULL pointer detected!\n", RED);
        return NULL;
    }

    __asm__ volatile (
        "cld\n\t"
        "rep movsb"
        : "+D" (dest), "+S" (src), "+c" (n)
        :
        : "memory"
    );
    return dest;
}

void *flop_memmove(void *dest, const void *src, size_t n) {
    if (!dest || !src) {
        echo("flop_memmove: NULL pointer detected!\n", RED);
        return NULL;
    }

    if (dest < src) {
        return flop_memcpy(dest, src, n);
    } else {
        uint8_t *d = (uint8_t *)dest + n - 1;
        const uint8_t *s = (const uint8_t *)src + n - 1;

        __asm__ volatile (
            "std\n\t"
            "rep movsb"
            : "+D" (d), "+S" (s), "+c" (n)
            :
            : "memory"
        );
        __asm__ volatile ("cld" ::: "cc");
    }

    return dest;
}