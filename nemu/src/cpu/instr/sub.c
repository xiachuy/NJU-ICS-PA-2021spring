#include "cpu/instr.h"
/*
Put the implementations of `sub' instructions here.
*/

static void instr_execute_2op(){
    operand_read(&opr_src);
    operand_read(&opr_dest);
    opr_src.val = sign_ext(opr_src.val, opr_src.data_size); // must be extended
    opr_dest.val = alu_sub(opr_src.val, opr_dest.val, opr_dest.data_size);
    operand_write(&opr_dest);
}
make_instr_impl_2op(sub, i, rm, bv)
make_instr_impl_2op(sub, rm, r, v)
make_instr_impl_2op(sub, r, rm, v)
make_instr_impl_2op(sub, i, rm, v)

make_instr_func(sub_i2a_v){
    OPERAND imm,r;
    imm.data_size = r.data_size = data_size;
    imm.addr = eip + 1;
    r.addr = 0x0;
    imm.type = OPR_IMM;
    r.type = OPR_REG;
    operand_read(&imm);
    operand_read(&r);
    r.val = alu_sub(imm.val, r.val, r.data_size);
    operand_write(&r);
    return 1 + data_size/8;
}

