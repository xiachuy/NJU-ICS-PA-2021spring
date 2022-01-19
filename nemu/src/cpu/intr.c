#include "cpu/intr.h"
#include "cpu/instr.h"
#include "memory/memory.h"
#include <assert.h>
void raise_intr(uint8_t intr_no)
{
#ifdef IA32_INTR
	/*printf("Please implement raise_intr()");
	fflush(stdout);
	assert(0);*/
    // Trigger an exception/interrupt with 'intr_no'
    // 'intr_no' is the index to the IDT
    // Push EFLAGS, CS, and EIP
    // Find the IDT entry using 'intr_no'
    // Clear IF if it is an interrupt
    // Set CS:EIP to the entry of the interrupt handler
    cpu.esp -= 4;
    vaddr_write(cpu.esp, SREG_SS, 4, cpu.eflags.val);

    cpu.esp -= 2;
    vaddr_write(cpu.esp, SREG_SS, 2, cpu.cs.val);
    
    cpu.esp -= 4;
    vaddr_write(cpu.esp, SREG_SS, 4, cpu.eip);

    GateDesc *descriptor = (GateDesc *)(hw_mem + page_translate(
            segment_translate(cpu.idtr.base + 8 * intr_no, SREG_CS)));

    if(descriptor->type == 0xe) cpu.eflags.IF = 0;
    uint32_t addr = ((descriptor->offset_31_16) << 16) + descriptor->offset_15_0;
    cpu.cs.val = descriptor->selector;

    cpu.eip = addr;
    
#endif
}

void raise_sw_intr(uint8_t intr_no)
{
	// return address is the next instruction
	cpu.eip += 2;
	raise_intr(intr_no);
}
