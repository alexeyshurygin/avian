/* Copyright (c) 2008-2013, Avian Contributors

   Permission to use, copy, modify, and/or distribute this software
   for any purpose with or without fee is hereby granted, provided
   that the above copyright notice and this permission notice appear
   in all copies.

   There is NO WARRANTY for this software.  See license.txt for
   details. */

#ifndef AVIAN_CODEGEN_ASSEMBLER_ARM_ENCODE_H
#define AVIAN_CODEGEN_ASSEMBLER_ARM_ENCODE_H

#include <avian/codegen/lir.h>
#include <avian/codegen/assembler.h>

namespace avian {
namespace codegen {
namespace arm {

namespace isa {

// SYSTEM REGISTERS
const int FPSID = 0x0;
const int FPSCR = 0x1;
const int FPEXC = 0x8;
// INSTRUCTION OPTIONS
enum CONDITION { EQ, NE, CS, CC, MI, PL, VS, VC, HI, LS, GE, LT, GT, LE, AL, NV };
enum SHIFTOP { LSL, LSR, ASR, ROR };
// INSTRUCTION FORMATS
inline int DATA(int cond, int opcode, int S, int Rn, int Rd, int shift, int Sh, int Rm)
{ return cond<<28 | opcode<<21 | S<<20 | Rn<<16 | Rd<<12 | shift<<7 | Sh<<5 | Rm; }
inline int DATAS(int cond, int opcode, int S, int Rn, int Rd, int Rs, int Sh, int Rm)
{ return cond<<28 | opcode<<21 | S<<20 | Rn<<16 | Rd<<12 | Rs<<8 | Sh<<5 | 1<<4 | Rm; }
inline int DATAI(int cond, int opcode, int S, int Rn, int Rd, int rot, int imm)
{ return cond<<28 | 1<<25 | opcode<<21 | S<<20 | Rn<<16 | Rd<<12 | rot<<8 | (imm&0xff); }
inline int BRANCH(int cond, int L, int offset)
{ return cond<<28 | 5<<25 | L<<24 | (offset&0xffffff); }
inline int BRANCHX(int cond, int L, int Rm)
{ return cond<<28 | 0x4bffc<<6 | L<<5 | 1<<4 | Rm; }
inline int MULTIPLY(int cond, int mul, int S, int Rd, int Rn, int Rs, int Rm)
{ return cond<<28 | mul<<21 | S<<20 | Rd<<16 | Rn<<12 | Rs<<8 | 9<<4 | Rm; }
inline int XFER(int cond, int P, int U, int B, int W, int L, int Rn, int Rd, int shift, int Sh, int Rm)
{ return cond<<28 | 3<<25 | P<<24 | U<<23 | B<<22 | W<<21 | L<<20 | Rn<<16 | Rd<<12 | shift<<7 | Sh<<5 | Rm; }
inline int XFERI(int cond, int P, int U, int B, int W, int L, int Rn, int Rd, int offset)
{ return cond<<28 | 2<<25 | P<<24 | U<<23 | B<<22 | W<<21 | L<<20 | Rn<<16 | Rd<<12 | (offset&0xfff); }
inline int XFER2(int cond, int P, int U, int W, int L, int Rn, int Rd, int S, int H, int Rm)
{ return cond<<28 | P<<24 | U<<23 | W<<21 | L<<20 | Rn<<16 | Rd<<12 | 1<<7 | S<<6 | H<<5 | 1<<4 | Rm; }
inline int XFER2I(int cond, int P, int U, int W, int L, int Rn, int Rd, int offsetH, int S, int H, int offsetL)
{ return cond<<28 | P<<24 | U<<23 | 1<<22 | W<<21 | L<<20 | Rn<<16 | Rd<<12 | offsetH<<8 | 1<<7 | S<<6 | H<<5 | 1<<4 | (offsetL&0xf); }
inline int COOP(int cond, int opcode_1, int CRn, int CRd, int cp_num, int opcode_2, int CRm)
{ return cond<<28 | 0xe<<24 | opcode_1<<20 | CRn<<16 | CRd<<12 | cp_num<<8 | opcode_2<<5 | CRm; }
inline int COXFER(int cond, int P, int U, int N, int W, int L, int Rn, int CRd, int cp_num, int offset) // offset is in words, not bytes
{ return cond<<28 | 0x6<<25 | P<<24 | U<<23 | N<<22 | W<<21 | L<<20 | Rn<<16 | CRd<<12 | cp_num<<8 | (offset&0xff)>>2; }
inline int COREG(int cond, int opcode_1, int L, int CRn, int Rd, int cp_num, int opcode_2, int CRm)
{ return cond<<28 | 0xe<<24 | opcode_1<<21 | L<<20 | CRn<<16 | Rd<<12 | cp_num<<8 | opcode_2<<5 | 1<<4 | CRm; }
inline int COREG2(int cond, int L, int Rn, int Rd, int cp_num, int opcode, int CRm)
{ return cond<<28 | 0xc4<<20 | L<<20 | Rn<<16 | Rd<<12 | cp_num<<8 | opcode<<4 | CRm;}
// FIELD CALCULATORS
inline int calcU(int imm) { return imm >= 0 ? 1 : 0; }
// INSTRUCTIONS
// The "cond" and "S" fields are set using the SETCOND() and SETS() functions
inline int b(int offset) { return BRANCH(AL, 0, offset); }
inline int bl(int offset) { return BRANCH(AL, 1, offset); }
inline int bx(int Rm) { return BRANCHX(AL, 0, Rm); }
inline int blx(int Rm) { return BRANCHX(AL, 1, Rm); }
inline int and_(int Rd, int Rn, int Rm, int Sh=0, int shift=0) { return DATA(AL, 0x0, 0, Rn, Rd, shift, Sh, Rm); }
inline int eor(int Rd, int Rn, int Rm, int Sh=0, int shift=0) { return DATA(AL, 0x1, 0, Rn, Rd, shift, Sh, Rm); }
inline int rsb(int Rd, int Rn, int Rm, int Sh=0, int shift=0) { return DATA(AL, 0x3, 0, Rn, Rd, shift, Sh, Rm); }
inline int add(int Rd, int Rn, int Rm, int Sh=0, int shift=0) { return DATA(AL, 0x4, 0, Rn, Rd, shift, Sh, Rm); }
inline int adc(int Rd, int Rn, int Rm, int Sh=0, int shift=0) { return DATA(AL, 0x5, 0, Rn, Rd, shift, Sh, Rm); }
inline int rsc(int Rd, int Rn, int Rm, int Sh=0, int shift=0) { return DATA(AL, 0x7, 0, Rn, Rd, shift, Sh, Rm); }
inline int cmp(int Rn, int Rm, int Sh=0, int shift=0) { return DATA(AL, 0xa, 1, Rn, 0, shift, Sh, Rm); }
inline int orr(int Rd, int Rn, int Rm, int Sh=0, int shift=0) { return DATA(AL, 0xc, 0, Rn, Rd, shift, Sh, Rm); }
inline int mov(int Rd, int Rm, int Sh=0, int shift=0) { return DATA(AL, 0xd, 0, 0, Rd, shift, Sh, Rm); }
inline int mvn(int Rd, int Rm, int Sh=0, int shift=0) { return DATA(AL, 0xf, 0, 0, Rd, shift, Sh, Rm); }
inline int andi(int Rd, int Rn, int imm, int rot=0) { return DATAI(AL, 0x0, 0, Rn, Rd, rot, imm); }
inline int subi(int Rd, int Rn, int imm, int rot=0) { return DATAI(AL, 0x2, 0, Rn, Rd, rot, imm); }
inline int rsbi(int Rd, int Rn, int imm, int rot=0) { return DATAI(AL, 0x3, 0, Rn, Rd, rot, imm); }
inline int addi(int Rd, int Rn, int imm, int rot=0) { return DATAI(AL, 0x4, 0, Rn, Rd, rot, imm); }
inline int adci(int Rd, int Rn, int imm, int rot=0) { return DATAI(AL, 0x5, 0, Rn, Rd, rot, imm); }
inline int bici(int Rd, int Rn, int imm, int rot=0) { return DATAI(AL, 0xe, 0, Rn, Rd, rot, imm); }
inline int cmpi(int Rn, int imm, int rot=0) { return DATAI(AL, 0xa, 1, Rn, 0, rot, imm); }
inline int movi(int Rd, int imm, int rot=0) { return DATAI(AL, 0xd, 0, 0, Rd, rot, imm); }
inline int orrsh(int Rd, int Rn, int Rm, int Rs, int Sh) { return DATAS(AL, 0xc, 0, Rn, Rd, Rs, Sh, Rm); }
inline int movsh(int Rd, int Rm, int Rs, int Sh) { return DATAS(AL, 0xd, 0, 0, Rd, Rs, Sh, Rm); }
inline int mul(int Rd, int Rm, int Rs) { return MULTIPLY(AL, 0, 0, Rd, 0, Rs, Rm); }
inline int mla(int Rd, int Rm, int Rs, int Rn) { return MULTIPLY(AL, 1, 0, Rd, Rn, Rs, Rm); }
inline int umull(int RdLo, int RdHi, int Rm, int Rs) { return MULTIPLY(AL, 4, 0, RdHi, RdLo, Rs, Rm); }
inline int ldr(int Rd, int Rn, int Rm, int W=0) { return XFER(AL, 1, 1, 0, W, 1, Rn, Rd, 0, 0, Rm); }
inline int ldri(int Rd, int Rn, int imm, int W=0) { return XFERI(AL, 1, calcU(imm), 0, W, 1, Rn, Rd, abs(imm)); }
inline int ldrb(int Rd, int Rn, int Rm) { return XFER(AL, 1, 1, 1, 0, 1, Rn, Rd, 0, 0, Rm); }
inline int ldrbi(int Rd, int Rn, int imm) { return XFERI(AL, 1, calcU(imm), 1, 0, 1, Rn, Rd, abs(imm)); }
inline int str(int Rd, int Rn, int Rm, int W=0) { return XFER(AL, 1, 1, 0, W, 0, Rn, Rd, 0, 0, Rm); }
inline int stri(int Rd, int Rn, int imm, int W=0) { return XFERI(AL, 1, calcU(imm), 0, W, 0, Rn, Rd, abs(imm)); }
inline int strb(int Rd, int Rn, int Rm) { return XFER(AL, 1, 1, 1, 0, 0, Rn, Rd, 0, 0, Rm); }
inline int strbi(int Rd, int Rn, int imm) { return XFERI(AL, 1, calcU(imm), 1, 0, 0, Rn, Rd, abs(imm)); }
inline int ldrh(int Rd, int Rn, int Rm) { return XFER2(AL, 1, 1, 0, 1, Rn, Rd, 0, 1, Rm); }
inline int ldrhi(int Rd, int Rn, int imm) { return XFER2I(AL, 1, calcU(imm), 0, 1, Rn, Rd, abs(imm)>>4 & 0xf, 0, 1, abs(imm)&0xf); }
inline int strh(int Rd, int Rn, int Rm) { return XFER2(AL, 1, 1, 0, 0, Rn, Rd, 0, 1, Rm); }
inline int strhi(int Rd, int Rn, int imm) { return XFER2I(AL, 1, calcU(imm), 0, 0, Rn, Rd, abs(imm)>>4 & 0xf, 0, 1, abs(imm)&0xf); }
inline int ldrsh(int Rd, int Rn, int Rm) { return XFER2(AL, 1, 1, 0, 1, Rn, Rd, 1, 1, Rm); }
inline int ldrshi(int Rd, int Rn, int imm) { return XFER2I(AL, 1, calcU(imm), 0, 1, Rn, Rd, abs(imm)>>4 & 0xf, 1, 1, abs(imm)&0xf); }
inline int ldrsb(int Rd, int Rn, int Rm) { return XFER2(AL, 1, 1, 0, 1, Rn, Rd, 1, 0, Rm); }
inline int ldrsbi(int Rd, int Rn, int imm) { return XFER2I(AL, 1, calcU(imm), 0, 1, Rn, Rd, abs(imm)>>4 & 0xf, 1, 0, abs(imm)&0xf); }
// breakpoint instruction, this really has its own instruction format
inline int bkpt(int16_t immed) { return 0xe1200070 | (((unsigned)immed & 0xffff) >> 4 << 8) | (immed & 0xf); }
// COPROCESSOR INSTRUCTIONS
inline int mcr(int coproc, int opcode_1, int Rd, int CRn, int CRm, int opcode_2=0) { return COREG(AL, opcode_1, 0, CRn, Rd, coproc, opcode_2, CRm); }
inline int mcrr(int coproc, int opcode, int Rd, int Rn, int CRm) { return COREG2(AL, 0, Rn, Rd, coproc, opcode, CRm); }
inline int mrc(int coproc, int opcode_1, int Rd, int CRn, int CRm, int opcode_2=0) { return COREG(AL, opcode_1, 1, CRn, Rd, coproc, opcode_2, CRm); }
inline int mrrc(int coproc, int opcode, int Rd, int Rn, int CRm) { return COREG2(AL, 1, Rn, Rd, coproc, opcode, CRm); }
// VFP FLOATING-POINT INSTRUCTIONS
inline int fmuls(int Sd, int Sn, int Sm) { return COOP(AL, (Sd&1)<<2|2, Sn>>1, Sd>>1, 10, (Sn&1)<<2|(Sm&1), Sm>>1); }
inline int fadds(int Sd, int Sn, int Sm) { return COOP(AL, (Sd&1)<<2|3, Sn>>1, Sd>>1, 10, (Sn&1)<<2|(Sm&1), Sm>>1); }
inline int fsubs(int Sd, int Sn, int Sm) { return COOP(AL, (Sd&1)<<2|3, Sn>>1, Sd>>1, 10, (Sn&1)<<2|(Sm&1)|2, Sm>>1); }
inline int fdivs(int Sd, int Sn, int Sm) { return COOP(AL, (Sd&1)<<2|8, Sn>>1, Sd>>1, 10, (Sn&1)<<2|(Sm&1), Sm>>1); }
inline int fmuld(int Dd, int Dn, int Dm) { return COOP(AL, 2, Dn, Dd, 11, 0, Dm); }
inline int faddd(int Dd, int Dn, int Dm) { return COOP(AL, 3, Dn, Dd, 11, 0, Dm); }
inline int fsubd(int Dd, int Dn, int Dm) { return COOP(AL, 3, Dn, Dd, 11, 2, Dm); }
inline int fdivd(int Dd, int Dn, int Dm) { return COOP(AL, 8, Dn, Dd, 11, 0, Dm); }
inline int fcpys(int Sd, int Sm) { return COOP(AL, 0xb|(Sd&1)<<2, 0, Sd>>1, 10, 2|(Sm&1), Sm>>1); }
inline int fabss(int Sd, int Sm) { return COOP(AL, 0xb|(Sd&1)<<2, 0, Sd>>1, 10, 6|(Sm&1), Sm>>1); }
inline int fnegs(int Sd, int Sm) { return COOP(AL, 0xb|(Sd&1)<<2, 1, Sd>>1, 10, 2|(Sm&1), Sm>>1); }
inline int fsqrts(int Sd, int Sm) { return COOP(AL, 0xb|(Sd&1)<<2, 1, Sd>>1, 10, 6|(Sm&1), Sm>>1); }
inline int fcmps(int Sd, int Sm) { return COOP(AL, 0xb|(Sd&1)<<2, 4, Sd>>1, 10, 2|(Sm&1), Sm>>1); }
inline int fcvtds(int Dd, int Sm) { return COOP(AL, 0xb, 7, Dd, 10, 6|(Sm&1), Sm>>1); }
inline int fsitos(int Sd, int Sm) { return COOP(AL, 0xb|(Sd&1)<<2, 8, Sd>>1, 10, 6|(Sm&1), Sm>>1); }
inline int ftosizs(int Sd, int Sm) { return COOP(AL, 0xb|(Sd&1)<<2, 0xd, Sd>>1, 10, 6|(Sm&1), Sm>>1); }
inline int fcpyd(int Dd, int Dm) { return COOP(AL, 0xb, 0, Dd, 11, 2, Dm); }
inline int fabsd(int Dd, int Dm) { return COOP(AL, 0xb, 0, Dd, 11, 6, Dm); }
inline int fnegd(int Dd, int Dm) { return COOP(AL, 0xb, 1, Dd, 11, 2, Dm); }
inline int fsqrtd(int Dd, int Dm) { return COOP(AL, 0xb, 1, Dd, 11, 6, Dm); }
// double-precision comparison instructions
inline int fcmpd(int Dd, int Dm) { return COOP(AL, 0xb, 4, Dd, 11, 2, Dm); }
// double-precision conversion instructions
inline int fcvtsd(int Sd, int Dm) { return COOP(AL, 0xb|(Sd&1)<<2, 7, Sd>>1, 11, 6, Dm); }
inline int fsitod(int Dd, int Sm) { return COOP(AL, 0xb, 8, Dd, 11, 6|(Sm&1), Sm>>1); }
inline int ftosizd(int Sd, int Dm) { return COOP(AL, 0xb|(Sd&1)<<2, 0xd, Sd>>1, 11, 6, Dm); }
// single load/store instructions for both precision types
inline int flds(int Sd, int Rn, int offset=0) { return COXFER(AL, 1, 1, Sd&1, 0, 1, Rn, Sd>>1, 10, offset); };
inline int fldd(int Dd, int Rn, int offset=0) { return COXFER(AL, 1, 1, 0, 0, 1, Rn, Dd, 11, offset); };
inline int fsts(int Sd, int Rn, int offset=0) { return COXFER(AL, 1, 1, Sd&1, 0, 0, Rn, Sd>>1, 10, offset); };
inline int fstd(int Dd, int Rn, int offset=0) { return COXFER(AL, 1, 1, 0, 0, 0, Rn, Dd, 11, offset); };
// move between GPRs and FPRs
inline int fmsr(int Sn, int Rd) { return mcr(10, 0, Rd, Sn>>1, 0, (Sn&1)<<2); }
inline int fmrs(int Rd, int Sn) { return mrc(10, 0, Rd, Sn>>1, 0, (Sn&1)<<2); }
// move to/from VFP system registers
inline int fmrx(int Rd, int reg) { return mrc(10, 7, Rd, reg, 0); }
// these move around pairs of single-precision registers
inline int fmdrr(int Dm, int Rd, int Rn) { return mcrr(11, 1, Rd, Rn, Dm); }
inline int fmrrd(int Rd, int Rn, int Dm) { return mrrc(11, 1, Rd, Rn, Dm); }
// FLAG SETTERS
inline int SETCOND(int ins, int cond) { return ((ins&0x0fffffff) | (cond<<28)); }
inline int SETS(int ins) { return ins | 1<<20; }
// PSEUDO-INSTRUCTIONS
inline int lsl(int Rd, int Rm, int Rs) { return movsh(Rd, Rm, Rs, LSL); }
inline int lsli(int Rd, int Rm, int imm) { return mov(Rd, Rm, LSL, imm); }
inline int lsr(int Rd, int Rm, int Rs) { return movsh(Rd, Rm, Rs, LSR); }
inline int lsri(int Rd, int Rm, int imm) { return mov(Rd, Rm, LSR, imm); }
inline int asr(int Rd, int Rm, int Rs) { return movsh(Rd, Rm, Rs, ASR); }
inline int asri(int Rd, int Rm, int imm) { return mov(Rd, Rm, ASR, imm); }
inline int beq(int offset) { return SETCOND(b(offset), EQ); }
inline int bne(int offset) { return SETCOND(b(offset), NE); }
inline int bls(int offset) { return SETCOND(b(offset), LS); }
inline int bhi(int offset) { return SETCOND(b(offset), HI); }
inline int blt(int offset) { return SETCOND(b(offset), LT); }
inline int bgt(int offset) { return SETCOND(b(offset), GT); }
inline int ble(int offset) { return SETCOND(b(offset), LE); }
inline int bge(int offset) { return SETCOND(b(offset), GE); }
inline int blo(int offset) { return SETCOND(b(offset), CC); }
inline int bhs(int offset) { return SETCOND(b(offset), CS); }
inline int bpl(int offset) { return SETCOND(b(offset), PL); }
inline int fmstat() { return fmrx(15, FPSCR); }
// todo: make this pretty:
inline int dmb() { return 0xf57ff05f; }

} // namespace isa

inline void emit(Context* con, int code) { con->code.append4(code); }

} // namespace arm
} // namespace codegen
} // namespace avian

#endif // AVIAN_CODEGEN_ASSEMBLER_ARM_ENCODE_H
