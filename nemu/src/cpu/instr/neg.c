#include "cpu/instr.h"
/*
Put the implementations of `neg' instructions here.
*/
make_instr_func(neg_rm_v){
    int len = 1;
    OPERAND rm;
    rm.data_size = data_size;
    len += modrm_rm(eip+1, &rm);
    operand_read(&rm);
    cpu.eflags.CF = (rm.val == 0x0 ? 0 : 1);
    rm.val = - rm.val;
    operand_write(&rm);
    return len;
}
