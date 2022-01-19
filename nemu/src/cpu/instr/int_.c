#include "cpu/intr.h"
#include "cpu/instr.h"

/*
Put the implementations of `int' instructions here.

Special note for `int': please use the instruction name `int_' instead of `int'.
*/

make_instr_func(int_){
    OPERAND imm;
    imm.data_size = 8;
    imm.sreg = SREG_CS;
    imm.type = OPR_IMM;
    imm.addr = eip+1;
    operand_read(&imm);
    raise_sw_intr(imm.val);
    // raise_sw_intr eip += 2, so return zero;
    return 0;
}
