#include "cpu/instr.h"
/*
Put the implementations of `call' instructions here.
*/
make_instr_func(call_near){
    OPERAND imm;
    imm.data_size = data_size;
    imm.addr = eip + 1;
    imm.type = OPR_IMM;
    operand_read(&imm);
    //push eip
    int len = data_size/8 + 1;
    eip += len;
    cpu.esp -= data_size/8;
    vaddr_write(cpu.esp, SREG_SS, data_size/8, eip);
    //end push
    cpu.eip = (eip  + imm.val) & ((uint32_t)0xffffffff >> (32 - data_size));
    return 0;
}
make_instr_func(call_near_indirect){
    OPERAND rm;
    rm.data_size = data_size;
    int len = 1;
    len += modrm_rm(eip+1, &rm);
    operand_read(&rm);
    // push eip
    eip += len; // eip is parameter which equals to cpu.eip
    cpu.esp -= data_size/8;
    vaddr_write(cpu.esp, SREG_SS, data_size/8, eip);
    //end push
    cpu.eip = rm.val& (((uint32_t)0xffffffff) >> (32 - data_size));
    return 0;
}
