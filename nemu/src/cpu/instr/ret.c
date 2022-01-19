#include "cpu/instr.h"
/*
Put the implementations of `ret' instructions here.
*/
make_instr_func(ret_near){
    // pop eip
    uint32_t previous_eip = vaddr_read(cpu.esp, SREG_SS, data_size / 8);
    cpu.esp += data_size / 8;
    // end pop
    
    cpu.eip = previous_eip & ((uint32_t)0xffffffff >> (32 - data_size));
    
    //return zero
    return 0;
}
make_instr_func(ret_near_iw){
    OPERAND imm;
    imm.data_size = 16;
    imm.type = OPR_IMM;
    imm.addr = eip+1;
    operand_read(&imm);
    
    // pop eip
    uint32_t previous_eip = vaddr_read(cpu.esp, SREG_SS, data_size / 8);
    cpu.esp += data_size / 8;
    
    cpu.esp += imm.val;
    
    // end pop
    
    
    
    cpu.eip = previous_eip & ((uint32_t)0xffffffff >> (32 - data_size));
    
    
    // not change eip in loop
    return 0;
}