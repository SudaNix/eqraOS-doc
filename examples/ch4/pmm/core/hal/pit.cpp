#include <hal.h>
#include "idt.h"
#include "pic.h"
#include "pit.h"

static volatile uint32_t _pit_ticks = 0;
static bool _pit_is_init = false;

void _cdecl i386_pit_irq();

void i386_pit_send_command(uint8_t cmd) {
	outportb(I386_PIT_COMMAND_REG,cmd);
}

void i386_pit_send_data(uint16_t data,uint8_t counter) {
	uint8_t port;
	
	if (counter == I386_PIT_OCW_COUNTER_0)
		port = I386_PIT_COUNTER0_REG;
	else if ( counter == I386_PIT_OCW_COUNTER_1)
		port = I386_PIT_COUNTER1_REG;
	else
		port = I386_PIT_COUNTER2_REG;
		
	outportb(port,uint8_t(data));
}

uint8_t i386_pit_read_data(uint16_t counter) {
	uint8_t port;
	
	if (counter == I386_PIT_OCW_COUNTER_0)
		port = I386_PIT_COUNTER0_REG;
	else if ( counter == I386_PIT_OCW_COUNTER_1)
		port = I386_PIT_COUNTER1_REG;
	else
		port = I386_PIT_COUNTER2_REG;
		
	return inportb(port);
}

uint32_t i386_pit_set_tick_count(uint32_t i) {
	uint32_t prev = _pit_ticks;
	_pit_ticks = i;
	return prev;
}

uint32_t i386_pit_get_tick_count() {
	return _pit_ticks;
}

void i386_pit_start_counter(uint32_t freq,uint8_t counter,uint8_t mode) {
	if (freq == 0)
		return;
	
	uint16_t divisor = uint16_t(1193181/uint16_t(freq));
	
	/*	send operation command	*/
	uint8_t ocw = 0;
	
	ocw = (ocw & ~I386_PIT_OCW_MASK_MODE) | mode;
	ocw = (ocw & ~I386_PIT_OCW_MASK_RL) | I386_PIT_OCW_RL_DATA;
	ocw = (ocw & ~I386_PIT_OCW_MASK_COUNTER) | counter;
	
	i386_pit_send_command(ocw);
	
	/*	set frequency rate	*/
	i386_pit_send_data(divisor & 0xff,0);
	i386_pit_send_data((divisor >> 8) & 0xff,0);
	
	/*	reset ticks count	*/
	_pit_ticks = 0;
}

void _cdecl i386_pit_init() {
	set_vector(32,i386_pit_irq);
	_pit_is_init = true;
}

bool _cdecl i386_pit_is_initialized() {
	return _pit_is_init;
}

void _cdecl i386_pit_irq() {
	
	_asm {
		add esp,12
		pushad
	}
	
	_pit_ticks++;
	
	int_done(0);
	
	_asm {
		popad
		iretd
	}
}
