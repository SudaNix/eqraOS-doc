#include <hal.h>
#include "pic.h"


uint8_t i386_pic_read_data(uint8_t pic_num) {
	if (pic_num > 1)
		return 0;
	
	uint8_t reg = (pic_num == 1)?I386_PIC2_DATA_REG:I386_PIC1_DATA_REG;
	return inportb(reg);
}

void i386_pic_send_data(uint8_t data,uint8_t pic_num) {
	if (pic_num > 1)
		return;
		
	uint8_t reg = (pic_num == 1)?I386_PIC2_DATA_REG:I386_PIC1_DATA_REG;
	outportb(reg,data);
}

void i386_pic_send_command(uint8_t cmd,uint8_t pic_num) {
	
	if (pic_num > 1)
		return;
		
	uint8_t reg = (pic_num == 1)?I386_PIC2_COMMAND_REG:I386_PIC1_COMMAND_REG;
	outportb(reg,cmd);
}



void i386_pic_init(uint8_t base0,uint8_t base1) {
	
	uint8_t icw = 0;
	
	disable_irq();			/* disable hardware interrupt (cli) */
	
	/* init PIC, send ICW1 */
	icw = (icw & ~I386_PIC_ICW1_MASK_INIT) | I386_PIC_ICW1_INIT_YES;
	icw = (icw & ~I386_PIC_ICW1_MASK_IC4) | I386_PIC_ICW1_IC4_EXPECT;
	/* icw = 0x11 */
	
	i386_pic_send_command(icw,0);
	i386_pic_send_command(icw,1);
	
	/* ICW2 : remapping irq */
	i386_pic_send_data(base0,0);
	i386_pic_send_data(base1,1);
	
	/* ICW3 : irq for master/slave pic*/
	i386_pic_send_data(0x4,0);
	i386_pic_send_data(0x2,1);
	
	/* ICW4: enable i386 mode. */
	icw = (icw & ~I386_PIC_ICW4_MASK_UPM) | I386_PIC_ICW4_UPM_86MODE ; /* icw = 1 */
	i386_pic_send_data(icw,0);
	i386_pic_send_data(icw,1);
	
}
