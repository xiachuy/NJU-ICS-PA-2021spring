#include "cpu/instr.h"
/*
Put the implementations of `cmp' instructions here.
*/

/*
pay attention to src's sign extension because src can be immediate which is byte or word
*/
static void instr_execute_2op(){
    operand_read(&opr_dest);
	operand_read(&opr_src);
	opr_src.val = sign_ext(opr_src.val, opr_src.data_size);
	alu_sub(opr_src.val, opr_dest.val, opr_dest.data_size);
}

make_instr_impl_2op(cmp, i, rm, bv)
make_instr_impl_2op(cmp, r, rm, v)
make_instr_impl_2op(cmp, i, rm, v)
make_instr_impl_2op(cmp, rm, r, v)
make_instr_impl_2op(cmp, i, rm, b)
make_instr_impl_2op(cmp, rm, r, b)
make_instr_impl_2op(cmp, r, rm, b)

make_instr_func(cmp_i2a_b){
    OPERAND r, imm;
    r.data_size = imm.data_size = 8;
    imm.type = OPR_IMM;
    r.type = OPR_REG;
    r.addr = 0x0;   // eax is 000b 
    imm.addr = eip + 1;
    operand_read(&r);
    operand_read(&imm);
    alu_sub(imm.val, r.val, r.data_size);
    return 1+1;
}
make_instr_func(cmp_i2a_v){
    OPERAND r, imm;
    r.data_size = imm.data_size = data_size;
    imm.type = OPR_IMM;
    r.type = OPR_REG;
    r.addr = 0x0;   // eax is 000b 
    imm.addr = eip + 1;
    operand_read(&r);
    operand_read(&imm);
    alu_sub(imm.val, r.val, r.data_size);
    return 1+data_size/8;
}





