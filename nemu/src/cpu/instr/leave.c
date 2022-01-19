#include "cpu/instr.h"
/*
Put the implementations of `leave' instructions here.
*/
make_instr_func(leave_){
    
    cpu.esp = cpu.ebp;
    
    
    OPERAND m;
    m.data_size = data_size;
    m.type = OPR_MEM;
    m.sreg = SREG_SS;
    m.addr = cpu.esp;
    operand_read(&m);
    
    cpu.ebp = m.val;
    
    cpu.esp = cpu.esp + data_size/8;
    return 1;
}