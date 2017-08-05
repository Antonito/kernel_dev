#pragma once

#include <stdint.h>

uint32_t atomic_exchange(uint32_t volatile *ptr, uint32_t const val);
void repinsw(uint16_t port, uint8_t *data, uint32_t size);
void repoutsw(uint16_t port, uint8_t *data, uint32_t size);
