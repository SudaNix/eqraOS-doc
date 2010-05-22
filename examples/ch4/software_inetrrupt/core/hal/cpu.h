#ifndef CPU_H
#define CPU_H

#ifndef i386
#error "[cpu.h] is not implemented in this platform"
#endif

#include <stdint.h>
#include "regs.h"

extern int i386_cpu_init();
extern void i386_cpu_close();

#endif // CPU_H