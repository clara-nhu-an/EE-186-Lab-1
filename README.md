# EE-186-Lab-1

## 1. Flashing & Debugging Code
(a)
**What happens during the flashing process?**

STM32CubeIDE compiles the C source code into machine code and links it with the required components. It then connects to the microcontroller through ST-LINK, erases the necessary flash memory pages, and writes the compiled program into flash memory.

**What files are generated when you build the project?**

The build generates intermediate object files (.o) and an executable file (.elf), which contains the machine code and debugging information. 

**What part of memory is written to on the MCU?**

The program is written into the microcontroller’s internal flash memory. This memory is nonvolatile.

**What enables STM32CubeIDE to communicate with your board?**

The board’s onboard ST-LINK debugger/programmer enables communication with STM32CubeIDE through a USB connection to the computer.

**What tool or protocol is used to transfer the compiled binary to the microcontroller?**

The ST-LINK debugger/programmer transfers the compiled program to the microcontroller using the SWD (Serial Wire Debug) protocol. STM32CubeIDE controls ST-LINK through the ST-LINK GDB server.


(b)
Before:
<img width="1425" height="836" alt="image" src="https://github.com/user-attachments/assets/3e9786b9-2218-4140-8163-326ac3919caa" />

After:
<img width="1450" height="501" alt="image" src="https://github.com/user-attachments/assets/d76f3218-aa4b-483e-a404-55e07bc617ae" />

## 2. Blinking LEDs

(a)
1. The NUCLEO-L4R5ZI-P has three user LEDs: LD1 (green), LD2 (blue), and LD3 (red). 
2. In the default board configuration, they are connected to PC7, PB7, and PB14, respectively.
3. These pins support GPIO, meaning General-Purpose Input/Output, which allows software to read or control digital signals at the pins.
4. We configure them as general-purpose outputs because the microcontroller must drive their voltage levels to control the LEDs. A high output turns the corresponding LED on, and a low output turns it off.

(b) Screenshot of debugger
<img width="1192" height="417" alt="image" src="https://github.com/user-attachments/assets/6950e7ef-8d85-4752-acc2-7f21dd597fbf" />

(c) Source Code: (may also be found in blinking_led_c.c file)

```c
#include <stdint.h>

#define REG32(address) (*(volatile uint32_t *)(address))

#define RCC_AHB2ENR  REG32(0x4002104Cu)

#define GPIOB_MODER   REG32(0x48000400u)
#define GPIOB_OTYPER  REG32(0x48000404u)
#define GPIOB_OSPEEDR REG32(0x48000408u)
#define GPIOB_PUPDR   REG32(0x4800040Cu)
#define GPIOB_BSRR    REG32(0x48000418u)

#define GPIOC_MODER   REG32(0x48000800u)
#define GPIOC_OTYPER  REG32(0x48000804u)
#define GPIOC_OSPEEDR REG32(0x48000808u)
#define GPIOC_PUPDR   REG32(0x4800080Cu)
#define GPIOC_BSRR    REG32(0x48000818u)

#define GREEN (1u << 7)   /* PC7 */
#define BLUE  (1u << 7)   /* PB7 */
#define RED   (1u << 14)  /* PB14 */

static void delay(void)
{
    volatile uint32_t count = 200000u;

    while (count > 0u) {
        count--;
    }
}

int main(void)
{
    /* Enable clocks for GPIOB and GPIOC. */
    RCC_AHB2ENR |= (1u << 1) | (1u << 2);

    /* Read back to allow clock enabling to take effect. */
    (void)RCC_AHB2ENR;

    /* Initialize all LED output levels to low. */
    GPIOB_BSRR = (BLUE | RED) << 16;
    GPIOC_BSRR = GREEN << 16;

    /* Push-pull output: clear each pin's output-type bit. */
    GPIOB_OTYPER &= ~(BLUE | RED);
    GPIOC_OTYPER &= ~GREEN;

    /* Low speed: clear each pin's two-bit speed field. */
    GPIOB_OSPEEDR &= ~((3u << 14) | (3u << 28));
    GPIOC_OSPEEDR &= ~(3u << 14);

    /* No pull-up or pull-down. */
    GPIOB_PUPDR &= ~((3u << 14) | (3u << 28));
    GPIOC_PUPDR &= ~(3u << 14);

    /* Output mode: clear the mode fields, then set to 01. */
    GPIOB_MODER &= ~((3u << 14) | (3u << 28));
    GPIOB_MODER |=  (1u << 14) | (1u << 28);

    GPIOC_MODER &= ~(3u << 14);
    GPIOC_MODER |=  (1u << 14);

    while (1) {
        /* Green on. */
        GPIOC_BSRR = GREEN;
        delay();

        /* Green off, blue on. */
        GPIOC_BSRR = GREEN << 16;
        GPIOB_BSRR = BLUE;
        delay();

        /* Blue off, red on. */
        GPIOB_BSRR = BLUE << 16;
        GPIOB_BSRR = RED;
        delay();

        /* Red off, then repeat. */
        GPIOB_BSRR = RED << 16;
    }
}
```

Video of LEDs:

https://github.com/user-attachments/assets/26e06b26-d1d7-4c07-a611-47daf53a400b


## Blinking LEDs in assembly
deliverable in the blinking_led_assembly.c file











