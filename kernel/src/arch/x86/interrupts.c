#include <arch/x86/interrupts.h>
#include <arch/x86/io.h>
#include <kernel/logger.h>
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
static void irq_remap(void) {
  outb(0x20, 0x11);
  outb(0xA0, 0x11);

  outb(0x21, 0x20);
  outb(0xA1, 0x28);

  outb(0x21, 0x04);
  outb(0xA1, 0x02);

  outb(0x21, 0x01);
  outb(0xA1, 0x01);

  outb(0x21, 0x00);
  outb(0xA1, 0x00);
}

static void init_irq(void) {
  irq_remap();

  idt_set_gate(32, (unsigned)_irq0, 0x08, 0x8E);
  idt_set_gate(33, (unsigned)_irq1, 0x08, 0x8E);
  idt_set_gate(34, (unsigned)_irq2, 0x08, 0x8E);
  idt_set_gate(35, (unsigned)_irq3, 0x08, 0x8E);
  idt_set_gate(36, (unsigned)_irq4, 0x08, 0x8E);
  idt_set_gate(37, (unsigned)_irq5, 0x08, 0x8E);
  idt_set_gate(38, (unsigned)_irq6, 0x08, 0x8E);
  idt_set_gate(39, (unsigned)_irq7, 0x08, 0x8E);
  idt_set_gate(40, (unsigned)_irq8, 0x08, 0x8E);
  idt_set_gate(41, (unsigned)_irq9, 0x08, 0x8E);
  idt_set_gate(42, (unsigned)_irq10, 0x08, 0x8E);
  idt_set_gate(43, (unsigned)_irq11, 0x08, 0x8E);
  idt_set_gate(44, (unsigned)_irq12, 0x08, 0x8E);
  idt_set_gate(45, (unsigned)_irq13, 0x08, 0x8E);
  idt_set_gate(46, (unsigned)_irq14, 0x08, 0x8E);
  idt_set_gate(47, (unsigned)_irq15, 0x08, 0x8E);
}

void irq_handler(struct s_regs *reg) {
  void (*handler)(struct s_regs * reg);

  handler = irq_routines[reg->int_no - 32];
  if (handler) {
    handler(reg);
  }
  if (reg->int_no >= 40) {
    outb(0xA0, 0x20);
  }
  outb(0x20, 0x20);
}

//
// ISRs
//

void isr_default_int(struct s_regs *r) {
  static char const *exception_message[] = {
      "Division By Zero",
      "Debug",
      "Non Maskable Interrupt Exception",
      "Breakpoint Exception",
      "Into Detected Overflow Exception",
      "Out of Bounds Exception",
      "Invalid Opcode Exception",
      "No Coprocessor Exception",
      "Double Fault Exception",
      "Coprocessor Segment Overrun Exception",
      "Bad TSS Exception",
      "Segment Not Present Exception",
      "Stack Fault Exception",
      "General Protection Fault Exception",
      "Page Fault Exception",
      "Unknown Interrupt Exception",
      "Coprocessor Fault Exception",
      "Alignment Check Exception (486+)",
      "Machine Check Exception (Pentium/586+)",
      "Reserved Exceptions",
      "Reserved Exceptions",
      "Reserved Exceptions",
      "Reserved Exceptions",
      "Reserved Exceptions",
      "Reserved Exceptions",
      "Reserved Exceptions",
      "Reserved Exceptions",
      "Reserved Exceptions",
      "Reserved Exceptions",
      "Reserved Exceptions",
      "Reserved Exceptions",
      "Reserved Exceptions"};

  if (r->int_no < 32) {
    panic(exception_message[r->int_no]);
  }
}

//
// IDT
//
extern void idt_load(void);
static struct idt_entry idt[256] = {{0, 0, 0, 0, 0}};
struct idt_reg idtr = {0, 0};

void idt_set_gate(uint8_t num, uint64_t base, uint16_t sel, uint8_t flags) {
  // Base (callback)
  idt[num].base_low = base & 0xFFFF;
  idt[num].base_high = (base >> 16) & 0xFFFF;

  // Segments + flags
  idt[num].sel = sel;
  idt[num].always0 = 0;
  idt[num].flags = flags;
}

void init_idt(void) {
  // Setup idtr
  idtr.limit = (sizeof(struct idt_entry) * 256) - 1;
  idtr.base = (uint32_t)&idt;
  memset(idt, 0, sizeof(idt));

  // Add ISR handler
  idt_set_gate(0, (unsigned)_isr0, 0x08, 0x8E);
  idt_set_gate(1, (unsigned)_isr1, 0x08, 0x8E);
  idt_set_gate(2, (unsigned)_isr2, 0x08, 0x8E);
  idt_set_gate(3, (unsigned)_isr3, 0x08, 0x8E);
  idt_set_gate(4, (unsigned)_isr4, 0x08, 0x8E);
  idt_set_gate(5, (unsigned)_isr5, 0x08, 0x8E);
  idt_set_gate(6, (unsigned)_isr6, 0x08, 0x8E);
  idt_set_gate(7, (unsigned)_isr7, 0x08, 0x8E);
  idt_set_gate(8, (unsigned)_isr8, 0x08, 0x8E);
  idt_set_gate(9, (unsigned)_isr9, 0x08, 0x8E);
  idt_set_gate(10, (unsigned)_isr10, 0x08, 0x8E);
  idt_set_gate(11, (unsigned)_isr11, 0x08, 0x8E);
  idt_set_gate(12, (unsigned)_isr12, 0x08, 0x8E);
  idt_set_gate(13, (unsigned)_isr13, 0x08, 0x8E);
  idt_set_gate(14, (unsigned)_isr14, 0x08, 0x8E);
  idt_set_gate(15, (unsigned)_isr15, 0x08, 0x8E);
  idt_set_gate(16, (unsigned)_isr16, 0x08, 0x8E);
  idt_set_gate(17, (unsigned)_isr17, 0x08, 0x8E);
  idt_set_gate(18, (unsigned)_isr18, 0x08, 0x8E);
  idt_set_gate(19, (unsigned)_isr19, 0x08, 0x8E);
  idt_set_gate(20, (unsigned)_isr20, 0x08, 0x8E);
  idt_set_gate(21, (unsigned)_isr21, 0x08, 0x8E);
  idt_set_gate(22, (unsigned)_isr22, 0x08, 0x8E);
  idt_set_gate(23, (unsigned)_isr23, 0x08, 0x8E);
  idt_set_gate(24, (unsigned)_isr24, 0x08, 0x8E);
  idt_set_gate(25, (unsigned)_isr25, 0x08, 0x8E);
  idt_set_gate(26, (unsigned)_isr26, 0x08, 0x8E);
  idt_set_gate(27, (unsigned)_isr27, 0x08, 0x8E);
  idt_set_gate(28, (unsigned)_isr28, 0x08, 0x8E);
  idt_set_gate(29, (unsigned)_isr29, 0x08, 0x8E);
  idt_set_gate(30, (unsigned)_isr30, 0x08, 0x8E);
  idt_set_gate(31, (unsigned)_isr31, 0x08, 0x8E);

  // Install IDT
  idt_load();
  init_irq();

  // Activate interrupts
  __asm__("sti;");
}
