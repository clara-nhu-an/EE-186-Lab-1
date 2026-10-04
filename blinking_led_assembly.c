#include <stdint.h>

int main(){
    __asm__ volatile (
        /* Enable GPIOB clock: RCC_AHB2ENR bit 1. */
        "LDR R0, =0x4002104C\n\t"
        "LDR R1, [R0]\n\t"
        "ORR R1, R1, #0x2\n\t"
        "STR R1, [R0]\n\t"

        /* Read back after enabling the clock. */
        "LDR R1, [R0]\n\t"

        /* Initialize PB7 output level to low using BSRR. */
        "LDR R0, =0x48000418\n\t"
        "LDR R1, =0x00800000\n\t"
        "STR R1, [R0]\n\t"

        /* Push-pull output: clear OTYPER bit 7. */
        "LDR R0, =0x48000404\n\t"
        "LDR R1, [R0]\n\t"
        "BIC R1, R1, #0x80\n\t"
        "STR R1, [R0]\n\t"

        /* Low speed: clear OSPEEDR bits 15:14. */
        "LDR R0, =0x48000408\n\t"
        "LDR R1, [R0]\n\t"
        "BIC R1, R1, #0xC000\n\t"
        "STR R1, [R0]\n\t"

        /* No pull-up/pull-down: clear PUPDR bits 15:14. */
        "LDR R0, =0x4800040C\n\t"
        "LDR R1, [R0]\n\t"
        "BIC R1, R1, #0xC000\n\t"
        "STR R1, [R0]\n\t"

        /* Output mode: set MODER bits 15:14 to 01. */
        "LDR R0, =0x48000400\n\t"
        "LDR R1, [R0]\n\t"
        "BIC R1, R1, #0xC000\n\t"
        "ORR R1, R1, #0x4000\n\t"
        "STR R1, [R0]\n\t"

        /* Turn blue LED on: set PB7 using BSRR bit 7. */
        "LDR R0, =0x48000418\n\t"
        "MOV R1, #0x80\n\t"
        "STR R1, [R0]\n\t"

        :
        :
        : "r0", "r1", "cc", "memory"
    );

    /* Keep running with the blue LED on. */
    while (1) {
    }
}
