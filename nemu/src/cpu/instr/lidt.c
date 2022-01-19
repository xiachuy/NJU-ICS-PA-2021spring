#include "cpu/instr.h"
/*
Put the implementations of `lidt' instructions here.
*/

make_instr_func(lidt){
    // m is in memory, not imm
    OPERAND m;
    m.data_size=16;
    int len = 1;
    len += modrm_rm(eip+1, &m);
    operand_read(&m);
    cpu.idtr.limit = m.val;

    m.data_size = (data_size == 16 ? 24 : 32);
    m.addr += 2;
    operand_read(&m);
    cpu.idtr.base = m.val;

    // len = 2, 0x0f opcode is two bytes code
    return len;
}
