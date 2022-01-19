#include "cpu/instr.h"
/*
Put the implementations of `pop' instructions here.
*/

static inline void pop_reg(uint8_t opcode){
    OPERAND r;
    r.data_size = data_size;
    r.type = OPR_REG;
    r.addr = opcode & 0x07;
    r.val = vaddr_read(cpu.esp, SREG_SS, data_size / 8);
    cpu.esp = cpu.esp + data_size / 8;
    operand_write(&r);
}

make_instr_func(pop_ebx){
    pop_reg(opcode);
    return 1;
}
make_instr_func(pop_ebp){
    pop_reg(opcode);
    return 1;
}
make_instr_func(pop_esi){
    pop_reg(opcode);
    return 1;
}
make_instr_func(pop_edi){
    pop_reg(opcode);
    return 1;
}

make_instr_func(pop_a){
    cpu.edi = vaddr_read(cpu.esp, SREG_SS, data_size/8);
    cpu.esp += data_size/8;

    cpu.esi = vaddr_read(cpu.esp, SREG_SS, data_size/8);
    cpu.esp += data_size/8;

    cpu.ebp = vaddr_read(cpu.esp, SREG_SS, data_size/8);
    cpu.esp += data_size/8;

    //vaddr_read(cpu.esp, SREG_SS, data_size/8);
    cpu.esp += data_size/8;

    cpu.ebx = vaddr_read(cpu.esp, SREG_SS, data_size/8);
    cpu.esp += data_size/8;

    cpu.edx = vaddr_read(cpu.esp, SREG_SS, data_size/8);
    cpu.esp += data_size/8;

    cpu.ecx = vaddr_read(cpu.esp, SREG_SS, data_size/8);
    cpu.esp += data_size/8;

    cpu.eax = vaddr_read(cpu.esp, SREG_SS, data_size/8);
    cpu.esp += data_size/8;

    return 1;
}
