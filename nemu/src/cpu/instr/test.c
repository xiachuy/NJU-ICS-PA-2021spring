#include "cpu/instr.h"
/*
Put the implementations of `test' instructions here.
*/

static void instr_execute_2op(){
    operand_read(&opr_src);
	operand_read(&opr_dest);
	opr_src.val = sign_ext(opr_src.val, opr_src.data_size);
	alu_and(opr_src.val, opr_dest.val, opr_dest.data_size);
}

make_instr_impl_2op(test, r, rm, v)
make_instr_impl_2op(test, r, rm, b)
make_instr_impl_2op(test, i, rm, v)
make_instr_impl_2op(test, i, rm, b)
make_instr_func(test_i2a_v){
    OPERAND r, imm;
    r.data_size = imm.data_size = data_size;
    r.type = OPR_REG;
    imm.type = OPR_IMM;
    r.addr = 0x0;
    imm.addr = eip + 1;
    operand_read(&r); operand_read(&imm);
    
    alu_and(r.val, imm.val, r.data_size);
    return 1 + data_size/8;
}
make_instr_func(test_i2a_b){
    OPERAND r, imm;
    r.data_size = imm.data_size = 8;
    r.type = OPR_REG;
    imm.type = OPR_IMM;
    r.addr = 0x0;
    imm.addr = eip + 1;
    operand_read(&r); operand_read(&imm);
    
    alu_and(r.val, imm.val, r.data_size);
    return 1 + 1;
}
/*make_instr_func(test_r2rm_v){
    OPERAND r,rm;
    r.data_size = rm.data_size = data_size;
    int len = 1;
    len += modrm_r_rm(eip + 1, &r, &rm);
    operand_read(&r);
    operand_read(&rm);
    rm.val =alu_and(rm.val, r.val, data_size);
    operand_write(&rm);
    return len;
}*/