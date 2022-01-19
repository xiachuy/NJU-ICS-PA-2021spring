#include "cpu/instr.h"

make_instr_func(jmp_near)
{
        OPERAND rel;
        rel.type = OPR_IMM;
        rel.sreg = SREG_CS;
        rel.data_size = data_size;
        rel.addr = eip + 1;

        operand_read(&rel);

        int offset = sign_ext(rel.val, data_size);
        // thank Ting Xu from CS'17 for finding this bug
        print_asm_1("jmp", "", 1 + data_size / 8, &rel);

        cpu.eip += offset;

        return 1 + data_size / 8;
}

make_instr_func(jmp_short){
    OPERAND rel;
    rel.type = OPR_IMM;
    rel.sreg = SREG_CS;
    rel.data_size = 8;
    rel.addr = eip + 1;
        
    operand_read(&rel);
    // bug ext error
    int offset = sign_ext(rel.val, rel.data_size);
    print_asm_1("jmp", "",1+rel.data_size/8, &rel);
    cpu.eip += offset;
    if (data_size == 16) cpu.eip &= 0x0000ffff;
        
    return 1 + 1;
}
make_instr_func(jmp_near_indirect){
    int len = 1;
    OPERAND rm;
    rm.data_size = data_size;
    len += modrm_rm(eip+1, &rm);
    operand_read(&rm);
    cpu.eip = rm.val & (0xffffffff >> (32 - data_size));
    return 0;
}

make_instr_func(jmp_far_imm){
    // pay attention to little endin
    OPERAND rel;
    rel.type = OPR_IMM;
    rel.sreg = SREG_CS;
    rel.data_size = data_size;
    rel.addr = eip + 1;
    
    
    operand_read(&rel);
    
    cpu.eip = rel.val & (0xffffffff >> (32 - data_size));
    
    rel.data_size = 16;
    
    rel.addr += data_size / 8;
    
    operand_read(&rel);
    
    cpu.segReg[1].val = rel.val;
    load_sreg(1);
    return 0;
}
