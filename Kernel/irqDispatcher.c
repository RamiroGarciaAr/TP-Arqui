#include <time.h>
#include <stdint.h>
#include <naiveConsole.h>
#include <keyboardDriver.h>
#include <irqDispatcher.h>
#include <screenDriver.h>

static void int_20();
static void int_21();
static void (*interruptions[])(void) = {int_20, int_21};

void irqDispatcher(uint64_t irq) {
	interruptions[irq]();
}

static void int_20() {
    timer_handler();
}

static void int_21() {
	saveKeyInBuffer();
}
