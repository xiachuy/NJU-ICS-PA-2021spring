#include "cpu/instr.h"

/*
Put the implementations of `push' instructions here.
*/

static inline void push_reg(uint8_t opcode){
    OPERAND r;
    r.data_size = data_size;
    r.type = OPR_REG;
    r.addr = opcode & 0x7;
    operand_read(&r);
    cpu.esp = cpu.esp - data_size/8;
    vaddr_write(cpu.esp, SREG_SS, data_size/8, r.val);
}

make_instr_func(push_ebp){
    push_reg(opcode);
    return 1;
}
make_instr_func(push_esp){
    push_reg(opcode);
    return 1;
}
make_instr_func(push_ebx){
    push_reg(opcode);
    return 1;
}
make_instr_func(push_edi){
    push_reg(opcode);
    return 1;
}
make_instr_func(push_esi){
    push_reg(opcode);
    return 1;
}
make_instr_func(push_ecx){
    push_reg(opcode);
    return 1;
}
make_instr_func(push_edx){
    push_reg(opcode);
    return 1;
}
make_instr_func(push_eax){
    push_reg(opcode);
    return 1;
}
make_instr_func(push_i_b){
    OPERAND imm;
    imm.data_size = 8;
    imm.type = OPR_IMM;
    imm.addr = eip + 1;
    operand_read(&imm);
    cpu.esp -= (data_size / 8);
    imm.val = sign_ext(imm.val, imm.data_size);
    vaddr_write(cpu.esp, SREG_SS, data_size/8, imm.val);
    return 1 + 1;

}
make_instr_func(push_m_v){
    OPERAND m;
    m.data_size = data_size;
    int len = 1;
    len += modrm_rm(eip + 1, &m);
    cpu.esp = cpu.esp - data_size / 8;
    operand_read(&m);
    vaddr_write(cpu.esp, SREG_SS, data_size/8, m.val);
    return len;

}
make_instr_func(push_i_l){
    OPERAND imm;
    imm.data_size = 32;
    imm.type = OPR_IMM;
    imm.addr = eip + 1;
    operand_read(&imm);
    //esp -= 4 is necessary, which is different from push_i_b
    cpu.esp -= (32 / 8);
    vaddr_write(cpu.esp, SREG_SS, 32 / 8, imm.val);
    return 1 + 32 / 8;
}

make_instr_func(push_a){
    OPERAND reg;
    reg.data_size = data_size;
    reg.sreg = SREG_SS;

    uint32_t temp = cpu.esp;

    cpu.esp -= (reg.data_size/8);
    reg.val = cpu.eax;
    vaddr_write(cpu.esp, reg.sreg, reg.data_size/8, reg.val);

    cpu.esp -= (reg.data_size/8);
    reg.val = cpu.ecx;
    vaddr_write(cpu.esp, reg.sreg, reg.data_size/8, reg.val);

    cpu.esp -= (reg.data_size/8);
    reg.val = cpu.edx;
    vaddr_write(cpu.esp, reg.sreg, reg.data_size/8, reg.val);

    cpu.esp -= (reg.data_size/8);
    reg.val = cpu.ebx;
    vaddr_write(cpu.esp, reg.sreg, reg.data_size/8, reg.val);

    cpu.esp -= (reg.data_size/8);
    reg.val = temp;
    vaddr_write(cpu.esp, reg.sreg, reg.data_size/8, reg.val);

    cpu.esp -= (reg.data_size/8);
    reg.val = cpu.ebp;
    vaddr_write(cpu.esp, reg.sreg, reg.data_size/8, reg.val);

    cpu.esp -= (reg.data_size/8);
    reg.val = cpu.esi;
    vaddr_write(cpu.esp, reg.sreg, reg.data_size/8, reg.val);

    cpu.esp -= (reg.data_size/8);
    reg.val = cpu.edi;
    vaddr_write(cpu.esp, reg.sreg, reg.data_size/8, reg.val);

    return 1;
}
