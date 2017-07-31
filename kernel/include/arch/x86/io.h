#pragma once

#include <stdint.h>

// Check architecture
#if !defined(__i386)
#error "Architecture not supported"
#endif

uint8_t inb(uint16_t const port);
uint16_t inw(uint16_t const port);
uint32_t inl(uint16_t const port);

void outb(uint16_t const port, uint8_t const val);
void outw(uint16_t const port, uint16_t const val);
void outl(uint16_t const port, uint32_t const val);
