#pragma once

#include <arch/x86/registers.h>
#include <stdint.h>

struct idt_entry {
  uint16_t base_low;
  uint16_t sel;
  uint8_t always0;
  uint8_t flags;
  uint16_t base_high;
} __attribute__((packed));

_Static_assert(sizeof(struct idt_entry) == 8, "Invalid IDT entry size");

struct idt_reg {
  uint16_t limit;
  uint32_t base;
} __attribute__((packed));

_Static_assert(sizeof(struct idt_reg) == 6, "Invalid IDT reg size");

void irq_handler(struct s_regs *reg);
void irq_set_routine(int32_t irq, void (*handler)(struct s_regs *reg));
void isr_default_int(struct s_regs *r);

void init_idt(void);
void idt_set_gate(uint8_t num, uint64_t base, uint16_t sel, uint8_t flags);

extern struct idt_reg idtr;
