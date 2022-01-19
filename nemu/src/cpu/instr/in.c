#include "cpu/instr.h"
#include "device/port_io.h"
/*
Put the implementations of `in' instructions here.
*/

make_instr_func(in_immport2a_b){
    OPERAND imm, ra;
    imm.data_size = 8;
    imm.type = OPR_IMM;
    imm.sreg = SREG_CS;
    imm.addr = eip+1;
    operand_read(&imm);

    ra.data_size = 8;
    ra.type = OPR_REG;
    ra.addr = REG_EAX;
    ra.val= pio_read(imm.val, 1);
    operand_write(&ra);
    return 2;
}

make_instr_func(in_immport2a_v){
    OPERAND imm, ra;
    imm.data_size = 8;
    imm.type = OPR_IMM;
    imm.sreg = SREG_CS;
    imm.addr = eip+1;
    operand_read(&imm);
    ra.data_size = data_size;
    ra.type = OPR_REG;
    ra.addr = REG_EAX;
    ra.val= pio_read(imm.val, data_size/8);
    operand_write(&ra);
    return 2;
}
make_instr_func(in_edxport2a_b){
    OPERAND ra, rd;
    ra.data_size = 8;
    ra.type = OPR_REG;
    ra.addr = REG_EAX;
    rd.data_size = 16;
    rd.type = OPR_REG;
    rd.addr = REG_EDX;
    operand_read(&rd);
    
    ra.val = pio_read(rd.val, 1);
    operand_write(&ra);
    
    return 1;
}

make_instr_func(in_edxport2a_v){
    OPERAND ra, rd;
    ra.data_size = data_size;
    ra.type = OPR_REG;
    ra.addr = REG_EAX;
    
    rd.data_size = 16;
    rd.type = OPR_REG;
    rd.addr = REG_EDX;
    
    operand_read(&rd);
    
    ra.val = pio_read(rd.val, data_size/8);
    operand_write(&ra);
    return 1;
}


