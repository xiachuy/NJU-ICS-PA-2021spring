#include "cpu/instr.h"
#include "device/port_io.h"
/*
Put the implementations of `out' instructions here.
*/

make_instr_func(out_a2immport_b){
    OPERAND imm;
    imm.data_size = 8;
    imm.type = OPR_IMM;
    imm.sreg = SREG_CS;
    imm.addr = eip+1;
    operand_read(&imm);
    pio_write(imm.val, 1, (cpu.eax & 0xff));
    
    return 2;
}
make_instr_func(out_a2immport_v){
    OPERAND imm;
    imm.data_size = 8;
    imm.type = OPR_IMM;
    imm.sreg = SREG_CS;
    imm.addr = eip+1;
    operand_read(&imm);
    uint32_t data = cpu.eax;
    if(data_size == 16) data = data & 0xffff;
    pio_write(imm.val, data_size/8, data);
    
    return 2;
}
make_instr_func(out_a2edxport_b){
    uint16_t port = cpu.edx & 0xffff;
    uint32_t data = cpu.eax & 0xff;
    pio_write(port, 1, data);
    return 1;
}
make_instr_func(out_a2edxport_v){
    uint16_t port = cpu.edx & 0xffff;
    OPERAND ra;
    ra.data_size = data_size;
    ra.type = OPR_REG;
    ra.addr = REG_EAX;
    operand_read(&ra);
    pio_write(port, data_size/8,ra.val);

    return 1;    
}


