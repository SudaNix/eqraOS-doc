#ifndef PIT_H
#define PIT_H

#ifndef i386
#error "[pic.h] is not implemented in this platform"
#endif

#include <stdint.h>

#define I386_PIT_COUNTER0_REG				0x40
#define I386_PIT_COUNTER1_REG				0x41
#define I386_PIT_COUNTER2_REG				0x42
#define I386_PIT_COMMAND_REG				0x43

#define I386_PIT_OCW_MASK_BINCOUNT			0x1
#define I386_PIT_OCW_MASK_MODE				0xe
#define I386_PIT_OCW_MASK_RL				0x30
#define I386_PIT_OCW_MASK_COUNTER			0xc0


#define I386_PIT_OCW_BINCOUNT_BINARY		0x0
#define I386_PIT_OCW_BINCOUNT_BCD			0x1

#define I386_PIT_OCW_MODE_TERMINALCOUNT		0x0
#define I386_PIT_OCW_MODE_ONESHOT			0x2
#define I386_PIT_OCW_MODE_RATEGEN			0x4
#define I386_PIT_OCW_MODE_SQUAREWAVEGEN		0x6
#define I386_PIT_OCW_MODE_SOFTWARETRIG		0x8
#define I386_PIT_OCW_MODE_HARDWARETRIG		0xa

#define I386_PIT_OCW_RL_LATCH				0x0
#define I386_PIT_OCW_RL_LSBONLY				0x10
#define I386_PIT_OCW_RL_MSBONLY				0x20
#define I386_PIT_OCW_RL_DATA				0x30

#define I386_PIT_OCW_COUNTER_0				0x0
#define I386_PIT_OCW_COUNTER_1				0x40
#define I386_PIT_OCW_COUNTER_2				0x80

extern void i386_pit_send_command(uint8_t cmd);
extern void i386_pit_send_data(uint16_t data,uint8_t counter);
extern uint8_t i386_pit_read_data(uint16_t counter);
extern uint32_t i386_pit_set_tick_count(uint32_t i);
extern uint32_t i386_pit_get_tick_count();
extern void i386_pit_start_counter(uint32_t freq,uint8_t counter,uint8_t mode);
extern void _cdecl i386_pit_init();
extern bool _cdecl i386_pit_is_initialized();
 

#endif // PIT_H