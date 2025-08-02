#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include <stdint.h>
#include <stdbool.h>

// IDT entry structure
typedef struct {
    uint16_t base_low;
    uint16_t sel;
    uint8_t always0;
    uint8_t flags;
    uint16_t base_high;
} __attribute__((packed)) idt_entry_t;

typedef struct {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) idt_ptr_t;


#define IDT_SIZE 256
#define ISR_STACK_SIZE 8192  
static uint8_t interrupt_stack[ISR_STACK_SIZE] __attribute__((aligned(16)));  

#define PIC1_COMMAND       0x20
#define PIC1_DATA          0x21
#define PIC2_COMMAND       0xA0
#define PIC2_DATA          0xA1

#define ICW1_INIT          0x10
#define ICW1_ICW4          0x01
#define ICW4_8086          0x01

#define PIC1_V_OFFSET      0x20
#define PIC2_V_OFFSET      0x28
#define PIC1_IRQ2          0x04
#define PIC2_CSC_ID        0x02

#define PIC1_MASK          0xFC
#define PIC2_MASK          0xFF

#define PIC_EOI            0x20

#define PIT_COMMAND_PORT   0x43
#define PIT_CHANNEL0_PORT  0x40
#define PIT_BASE_FREQUENCY 1193182

#define PIT_COMMAND_MODE 0x36
#define PIT_CHANNEL0     0x40
#define PIT_DIVISOR_LSB_MASK 0xFF
#define PIT_DIVISOR_MSB_SHIFT 8
#define KERNEL_CODE_SEGMENT 0x08




void init_interrupts();

void set_idt_entry(int n, uint32_t base, uint16_t sel, uint8_t flags);



#define IA32_INT_MASK() __asm__ volatile(                  \
                                             "cli" :::     \
                                             "memory"      \
                                        ) 

#define IA32_INT_UNMASK() __asm__ volatile("sti" ::: "memory")

#define IA32_CPU_RELAX() \
    __asm__ volatile (   \
        "pause"          \
        :                \
        :                \
        : "memory"       \
    )
#define IA32_INT_ENABLED() ({ \
    uint32_t eflags;          \
    __asm__ volatile(         \
        "pushf\n\t"           \
        "pop %0"              \
        : "=r"(eflags));      \
    (eflags & (1 << 9)) != 0; \
})
extern uint32_t global_tick_count;
#endif // INTERRUPTS_H
