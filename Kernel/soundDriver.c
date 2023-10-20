#include <soundDriver.h>
#include <videoDriver.h>
#include <time.h>

////extraido de https://wiki.osdev.org/PC_Speaker

extern void outb_asm();
extern uint8_t inb_asm(uint16_t port);
 
//Play sound using built in speaker 
static void play_sound(uint32_t nFrequence) {
	uint32_t Div;
	uint8_t tmp;

      //Set the PIT to the desired frequency
	Div = 1193180 / nFrequence;
	outb_asm(0x43, 0xb6);
	outb_asm(0x42, (uint8_t) (Div) );
	outb_asm(0x42, (uint8_t) (Div >> 8));

      //And play the sound using the PC speaker
	tmp = inb_asm(0x61);
	if (tmp != (tmp | 3)) {
		outb_asm(0x61, tmp | 3);
	}
}

//make it shutup
static void nosound() {
	    uint8_t tmp = inb_asm(0x61) & 0xFC;
      outb_asm(0x61, tmp);
}

void wait(uint32_t millis) {
    uint32_t countdown = millis;
    while (countdown > 0) {
        _hlt();
        countdown--;
    }
}

//Make a beep
void beep() {
	 play_sound(1000);
	 wait(10);
	 nosound();
     //set_PIT_2(old_frequency);
}
