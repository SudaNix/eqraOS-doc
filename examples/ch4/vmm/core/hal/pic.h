#ifndef PIC_H
#define PIC_H

#ifndef i386
#error "[pic.h] is not implemented in this platform"
#endif

#include <stdint.h>

// PIC 1 Devices IRQ
#define I386_PIC_IRQ_TIMER			0
#define I386_PIC_IRQ_KEYBOARD		1
#define I386_PIC_IRQ_SERIAL2		3
#define I386_PIC_IRQ_SERIAL1		4
#define I386_PIC_IRQ_PARALLEL2		5
#define I386_PIC_IRQ_DESKETTE		6
#define I386_PIC_IRQ_PARALLEL1		7

// PIC 2 Devices IRQ
#define I386_PIC_IRQ_CMOSTIMER		0
#define I386_PIC_IRQ_CGARETRACE		1
#define I386_PIC_IRQ_AUXILIRY		4
#define I386_PIC_IRQ_FPU			5
#define I386_PIC_IRQ_HDC			6

// Operation Commamd Word 2 (OCW2)
#define I386_PIC_OCW2_MASK_L1		1
#define I386_PIC_OCW2_MASK_L2		2
#define I386_PIC_OCW2_MASK_L3		4
#define I386_PIC_OCW2_MASK_EOI		0x20
#define I386_PIC_OCW2_MASK_SL		0x40
#define I386_PIC_OCW2_MASK_ROTATE	0x80


// Operation Commamd Word 3 (OCW3)
#define I386_PIC_OCW3_MASK_RIS		1
#define I386_PIC_OCW3_MASK_RIR		2
#define I386_PIC_OCW3_MASK_MODE		4
#define I386_PIC_OCW3_MASK_SMM		0x20
#define I386_PIC_OCW3_MASK_ESMM		0x40
#define I386_PIC_OCW3_MASK_D7		0x80


// PIC 1 port address
#define I386_PIC1_COMMAND_REG		0x20
#define I386_PIC1_STATUS_REG		0x20
#define I386_PIC1_IMR_REG			0x21
#define I386_PIC1_DATA_REG			0x21


// PIC 2 port address
#define I386_PIC2_COMMAND_REG		0xa0
#define I386_PIC2_STATUS_REG		0xa0
#define I386_PIC2_IMR_REG			0xa1
#define I386_PIC2_DATA_REG			0xa1

// Initializing Commamd Word 1 (ICW1) Mask
#define I386_PIC_ICW1_MASK_IC4		0x1
#define I386_PIC_ICW1_MASK_SNGL		0x2
#define I386_PIC_ICW1_MASK_ADI		0x4
#define I386_PIC_ICW1_MASK_LTIM		0x8
#define I386_PIC_ICW1_MASK_INIT		0x10


// Initializing Commamd Word 4 (ICW4) Mask
#define I386_PIC_ICW4_MASK_UPM		0x1
#define I386_PIC_ICW4_MASK_AEOI		0x2
#define I386_PIC_ICW4_MASK_MS		0x4
#define I386_PIC_ICW4_MASK_BUF		0x8
#define I386_PIC_ICW4_MASK_SFNM		0x10


// Initializing command 1 control bits
#define I386_PIC_ICW1_IC4_EXPECT			1
#define I386_PIC_ICW1_IC4_NO				0
#define I386_PIC_ICW1_SNGL_YES				2
#define I386_PIC_ICW1_SNGL_NO				0
#define I386_PIC_ICW1_ADI_CALLINTERVAL4		4
#define I386_PIC_ICW1_ADI_CALLINTERVAL8		0
#define I386_PIC_ICW1_LTIM_LEVELTRIGGERED	8
#define I386_PIC_ICW1_LTIM_EDGETRIGGERED	0
#define I386_PIC_ICW1_INIT_YES				0x10
#define I386_PIC_ICW1_INIT_NO				0

// Initializing command 4 control bits
#define I386_PIC_ICW4_UPM_86MODE			1
#define I386_PIC_ICW4_UPM_MCSMODE			0
#define I386_PIC_ICW4_AEOI_AUTOEOI			2
#define I386_PIC_ICW4_AEOI_NOAUTOEOI		0
#define I386_PIC_ICW4_MS_BUFFERMASTER		4
#define I386_PIC_ICW4_MS_BUFFERSLAVE		0
#define I386_PIC_ICW4_BUF_MODEYES			8
#define I386_PIC_ICW4_BUF_MODENO			0
#define I386_PIC_ICW4_SFNM_NESTEDMODE		0x10
#define I386_PIC_ICW4_SFNM_NOTNESTED		0


extern uint8_t i386_pic_read_data(uint8_t pic_num);
extern void i386_pic_send_data(uint8_t data,uint8_t pic_num);
extern void i386_pic_send_command(uint8_t cmd,uint8_t pic_num);
extern void i386_pic_mask_irq(uint8_t mask,uint8_t pic_num);
extern void i386_pic_init(uint8_t base0,uint8_t base1);


#endif // PIC_H