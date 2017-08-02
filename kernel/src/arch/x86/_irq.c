#include <arch/x86/interrupts.h>
#include <arch/x86/io.h>
#include <stdlib.h>
#include <string.h>

// Declaration of IRQs handlers
extern void _irq0();
extern void _irq1();
extern void _irq2();
extern void _irq3();
extern void _irq4();
extern void _irq5();
extern void _irq6();
extern void _irq7();
extern void _irq8();
extern void _irq9();
extern void _irq10();
extern void _irq11();
extern void _irq12();
extern void _irq13();
extern void _irq14();
extern void _irq15();

// Declaration of ISRs handlers
extern void _isr0();
extern void _isr1();
extern void _isr2();
extern void _isr3();
extern void _isr4();
extern void _isr5();
extern void _isr6();
extern void _isr7();
extern void _isr8();
extern void _isr9();
extern void _isr10();
extern void _isr11();
extern void _isr12();
extern void _isr13();
extern void _isr14();
extern void _isr15();
extern void _isr16();
extern void _isr17();
extern void _isr18();
extern void _isr19();
extern void _isr20();
extern void _isr21();
extern void _isr22();
extern void _isr23();
extern void _isr24();
extern void _isr25();
extern void _isr26();
extern void _isr27();
extern void _isr28();
extern void _isr29();
extern void _isr30();
extern void _isr31();

//
// IRQs
//

static void init_irq(void);
static void irq_remap(void);

static void (*irq_routines[16])(struct s_regs *reg) = {0};

void irq_set_routine(int32_t irq, void (*handler)(struct s_regs *reg)) {
  irq_routines[irq] = handler;
}

// Map IRQs 0 - 15 to IDT 32 - 47
static void irq_remap(void) { return; }

void irq_handler(struct s_regs *reg) { return; }

//
// ISRs
//

void isr_default_int(struct s_regs *r) { return; }

//
// IDT
//
extern struct idt_entry *idtptr;
