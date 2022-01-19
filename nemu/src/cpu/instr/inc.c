#include "cpu/instr.h"
#include<assert.h>
/*
Put the implementations of `inc' instructions here.
*/


static inline void inc_reg(uint8_t opcode){
    OPERAND r;
    r.data_size = data_size;
    r.type = OPR_REG;
    r.addr = opcode & 0x07;
    operand_read(&r);
    uint8_t cf = cpu.eflags.CF;
    r.val = alu_add(0x01, r.val, r.data_size);
    cpu.eflags.CF = cf;
    operand_write(&r);
}


make_instr_func(inc_edx){
    inc_reg(opcode);
    return 1;
}
make_instr_func(inc_eax){
    inc_reg(opcode);
    return 1;
}
make_instr_func(inc_edi){
    inc_reg(opcode);
    return 1;
}
make_instr_func(inc_rm_v){
    OPERAND rm;
    rm.data_size =  data_size;
    int len = 1;
    len += modrm_rm(eip+1, &rm);
    operand_read(&rm);
    uint8_t cf = cpu.eflags.CF;
    rm.val = alu_add(0x01, rm.val, rm.data_size);
    cpu.eflags.CF = cf;
    operand_write(&rm);
    return len;
}