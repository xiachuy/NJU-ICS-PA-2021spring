#include "cpu/instr.h"
/*
Put the implementations of `dec' instructions here.
*/

static inline void dec_reg(uint8_t opcode){
    OPERAND r;
    r.data_size = data_size;
    r.type = OPR_REG;
    r.addr = opcode & 0x07;
    operand_read(&r);
    uint8_t cf = cpu.eflags.CF;
    r.val = alu_sub(0x01, r.val, r.data_size);
    cpu.eflags.CF = cf;
    operand_write(&r);
}

make_instr_func(dec_eax){
    dec_reg(opcode);
    return 1;
}
make_instr_func(dec_ecx){
    dec_reg(opcode);
    return 1;
}
make_instr_func(dec_edx){
    dec_reg(opcode);
    return 1;
}

make_instr_func(dec_rm_v){
    OPERAND rm;
    rm.data_size = data_size;
    int len = 1;
    len += modrm_rm(eip+1, &rm);
    operand_read(&rm);
    uint8_t cf = cpu.eflags.CF;
    rm.val = alu_sub(0x01, rm.val, rm.data_size);
    cpu.eflags.CF = cf;
    operand_write(&rm);
    return len;
}
