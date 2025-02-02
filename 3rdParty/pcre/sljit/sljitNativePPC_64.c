/*
 *    Stack-less Just-In-Time compiler
 *
 *    Copyright 2009-2010 Zoltan Herczeg (hzmester@freemail.hu). All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are
 * permitted provided that the following conditions are met:
 *
 *   1. Redistributions of source code must retain the above copyright notice, this list of
 *      conditions and the following disclaimer.
 *
 *   2. Redistributions in binary form must reproduce the above copyright notice, this list
 *      of conditions and the following disclaimer in the documentation and/or other materials
 *      provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND CONTRIBUTORS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE COPYRIGHT HOLDER(S) OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* ppc 64-bit arch dependent functions. */

#ifdef __GNUC__
#define ASM_SLJIT_CLZ(src, dst) \
	asm volatile ( "cntlzd %0, %1" : "=r"(dst) : "r"(src) )
#else
#error "Must implement count leading zeroes"
#endif

#define RLDI(dst, src, sh, mb, type) \
	(HI(30) | S(src) | A(dst) | ((type) << 2) | (((sh) & 0x1f) << 11) | (((sh) & 0x20) >> 4) | (((mb) & 0x1f) << 6) | ((mb) & 0x20))

#define PUSH_RLDICR(reg, shift) \
	push_inst(compiler, RLDI(reg, reg, 63 - shift, shift, 1))

static int load_immediate(struct sljit_compiler *compiler, int reg, sljit_w imm)
{
	sljit_uw tmp;
	sljit_uw shift;
	sljit_uw tmp2;
	sljit_uw shift2;

	if (imm <= SIMM_MAX && imm >= SIMM_MIN)
		return push_inst(compiler, ADDI | D(reg) | A(0) | IMM(imm));

	if (imm <= SLJIT_W(0x7fffffff) && imm >= SLJIT_W(-0x80000000)) {
		FAIL_IF(push_inst(compiler, ADDIS | D(reg) | A(0) | IMM(imm >> 16)));
		return (imm & 0xffff) ? push_inst(compiler, ORI | S(reg) | A(reg) | IMM(imm)) : SLJIT_SUCCESS;
	}

	/* Count leading zeroes. */
	tmp = (imm >= 0) ? imm : ~imm;
	ASM_SLJIT_CLZ(tmp, shift);
	SLJIT_ASSERT(shift > 0);
	shift--;
	tmp = (imm << shift);

	if ((tmp & ~0xffff000000000000ul) == 0) {
		FAIL_IF(push_inst(compiler, ADDI | D(reg) | A(0) | IMM(tmp >> 48)));
		shift += 15;
		return PUSH_RLDICR(reg, shift);
	}

	if ((tmp & ~0xffffffff00000000ul) == 0) {
		FAIL_IF(push_inst(compiler, ADDIS | D(reg) | A(0) | IMM(tmp >> 48)));
		FAIL_IF(push_inst(compiler, ORI | S(reg) | A(reg) | IMM(tmp >> 32)));
		shift += 31;
		return PUSH_RLDICR(reg, shift);
	}

	/* Cut out the 16 bit from immediate. */
	shift += 15;
	tmp2 = imm & ((1ul << (63 - shift)) - 1);

	if (tmp2 <= 0xffff) {
		FAIL_IF(push_inst(compiler, ADDI | D(reg) | A(0) | IMM(tmp >> 48)));
		FAIL_IF(PUSH_RLDICR(reg, shift));
		return push_inst(compiler, ORI | S(reg) | A(reg) | tmp2);
	}

	if (tmp2 <= 0xffffffff) {
		FAIL_IF(push_inst(compiler, ADDI | D(reg) | A(0) | IMM(tmp >> 48)));
		FAIL_IF(PUSH_RLDICR(reg, shift));
		FAIL_IF(push_inst(compiler, ORIS | S(reg) | A(reg) | (tmp2 >> 16)));
		return (imm & 0xffff) ? push_inst(compiler, ORI | S(reg) | A(reg) | IMM(tmp2)) : SLJIT_SUCCESS;
	}

	ASM_SLJIT_CLZ(tmp2, shift2);
	tmp2 <<= shift2;

	if ((tmp2 & ~0xffff000000000000ul) == 0) {
		FAIL_IF(push_inst(compiler, ADDI | D(reg) | A(0) | IMM(tmp >> 48)));
		shift2 += 15;
		shift += (63 - shift2);
		FAIL_IF(PUSH_RLDICR(reg, shift));
		FAIL_IF(push_inst(compiler, ORI | S(reg) | A(reg) | (tmp2 >> 48)));
		return PUSH_RLDICR(reg, shift2);
	}

	/* The general version. */
	FAIL_IF(push_inst(compiler, ADDIS | D(reg) | A(0) | IMM(imm >> 48)));
	FAIL_IF(push_inst(compiler, ORI | S(reg) | A(reg) | IMM(imm >> 32)));
	FAIL_IF(PUSH_RLDICR(reg, 31));
	FAIL_IF(push_inst(compiler, ORIS | S(reg) | A(reg) | IMM(imm >> 16)));
	return push_inst(compiler, ORI | S(reg) | A(reg) | IMM(imm));
}

/* Simplified mnemonics: clrldi. */
#define INS_CLEAR_LEFT(dst, src, from) \
	(RLDICL | S(src) | A(dst) | ((from) << 6) | (1 << 5))

/* Sign extension for integer operations. */
#define UN_EXTS() \
	if ((flags & (ALT_SIGN_EXT | REG2_SOURCE)) == (ALT_SIGN_EXT | REG2_SOURCE)) { \
		FAIL_IF(push_inst(compiler, EXTSW | S(src2) | A(TMP_REG2))); \
		src2 = TMP_REG2; \
	}

#define BIN_EXTS() \
	if (flags & ALT_SIGN_EXT) { \
		if (flags & REG1_SOURCE) { \
			FAIL_IF(push_inst(compiler, EXTSW | S(src1) | A(TMP_REG1))); \
			src1 = TMP_REG1; \
		} \
		if (flags & REG2_SOURCE) { \
			FAIL_IF(push_inst(compiler, EXTSW | S(src2) | A(TMP_REG2))); \
			src2 = TMP_REG2; \
		} \
	}

#define BIN_IMM_EXTS() \
	if ((flags & (ALT_SIGN_EXT | REG1_SOURCE)) == (ALT_SIGN_EXT | REG1_SOURCE)) { \
		FAIL_IF(push_inst(compiler, EXTSW | S(src1) | A(TMP_REG1))); \
		src1 = TMP_REG1; \
	}

static SLJIT_INLINE int emit_single_op(struct sljit_compiler *compiler, int op, int flags,
	int dst, int src1, int src2)
{
	switch (op) {
	case SLJIT_ADD:
		if (flags & ALT_FORM1) {
			/* Flags not set: BIN_IMM_EXTS unnecessary. */
			SLJIT_ASSERT(src2 == TMP_REG2);
			return push_inst(compiler, ADDI | D(dst) | A(src1) | compiler->imm);
		}
		if (flags & ALT_FORM2) {
			/* Flags not set: BIN_IMM_EXTS unnecessary. */
			SLJIT_ASSERT(src2 == TMP_REG2);
			return push_inst(compiler, ADDIS | D(dst) | A(src1) | compiler->imm);
		}
		if (flags & ALT_FORM3) {
			SLJIT_ASSERT(src2 == TMP_REG2);
			BIN_IMM_EXTS();
			return push_inst(compiler, ADDIC | D(dst) | A(src1) | compiler->imm);
		}
		if (!(flags & ALT_SET_FLAGS))
			return push_inst(compiler, ADD | D(dst) | A(src1) | B(src2));
		BIN_EXTS();
		return push_inst(compiler, ADDC | OERC(ALT_SET_FLAGS) | D(dst) | A(src1) | B(src2));

	case SLJIT_ADDC:
		if (flags & ALT_FORM1) {
			FAIL_IF(push_inst(compiler, MFXER | S(0)));
			FAIL_IF(push_inst(compiler, ADDE | D(dst) | A(src1) | B(src2)));
			return push_inst(compiler, MTXER | S(0));
		}
		BIN_EXTS();
		return push_inst(compiler, ADDE | D(dst) | A(src1) | B(src2));

	case SLJIT_SUB:
		if (flags & ALT_FORM1) {
			/* Flags not set: BIN_IMM_EXTS unnecessary. */
			SLJIT_ASSERT(src2 == TMP_REG2);
			return push_inst(compiler, SUBFIC | D(dst) | A(src1) | compiler->imm);
		}
		if (flags & ALT_FORM2) {
			SLJIT_ASSERT(src2 == TMP_REG2);
			return push_inst(compiler, CMPI | CRD(0 | ((flags & ALT_SIGN_EXT) ? 0 : 1)) | A(src1) | compiler->imm);
		}
		if (flags & ALT_FORM3) {
			SLJIT_ASSERT(src2 == TMP_REG2);
			return push_inst(compiler, CMPLI | CRD(4 | ((flags & ALT_SIGN_EXT) ? 0 : 1)) | A(src1) | compiler->imm);
		}
		if (flags & ALT_FORM4)
			return push_inst(compiler, CMPL | CRD(4 | ((flags & ALT_SIGN_EXT) ? 0 : 1)) | A(src1) | B(src2));
		if (!(flags & ALT_SET_FLAGS))
			return push_inst(compiler, SUBF | D(dst) | A(src2) | B(src1));
		BIN_EXTS();
		if (flags & ALT_FORM5)
			FAIL_IF(push_inst(compiler, CMPL | CRD(4 | ((flags & ALT_SIGN_EXT) ? 0 : 1)) | A(src1) | B(src2)));
		return push_inst(compiler, SUBFC | OERC(ALT_SET_FLAGS) | D(dst) | A(src2) | B(src1));

	case SLJIT_SUBC:
		if (flags & ALT_FORM1) {
			FAIL_IF(push_inst(compiler, MFXER | S(0)));
			FAIL_IF(push_inst(compiler, SUBFE | D(dst) | A(src2) | B(src1)));
			return push_inst(compiler, MTXER | S(0));
		}
		BIN_EXTS();
		return push_inst(compiler, SUBFE | D(dst) | A(src2) | B(src1));

	case SLJIT_MUL:
		if (flags & ALT_FORM1) {
			SLJIT_ASSERT(src2 == TMP_REG2);
			return push_inst(compiler, MULLI | D(dst) | A(src1) | compiler->imm);
		}
		BIN_EXTS();
		if (flags & ALT_FORM2)
			return push_inst(compiler, MULLW | OERC(flags) | D(dst) | A(src2) | B(src1));
		return push_inst(compiler, MULLD | OERC(flags) | D(dst) | A(src2) | B(src1));

	case SLJIT_AND:
		if (flags & ALT_FORM1) {
			SLJIT_ASSERT(src2 == TMP_REG2);
			return push_inst(compiler, ANDI | S(src1) | A(dst) | compiler->imm);
		}
		if (flags & ALT_FORM2) {
			SLJIT_ASSERT(src2 == TMP_REG2);
			return push_inst(compiler, ANDIS | S(src1) | A(dst) | compiler->imm);
		}
		return push_inst(compiler, AND | RC(flags) | S(src1) | A(dst) | B(src2));

	case SLJIT_OR:
		if (flags & ALT_FORM1) {
			SLJIT_ASSERT(src2 == TMP_REG2);
			return push_inst(compiler, ORI | S(src1) | A(dst) | compiler->imm);
		}
		if (flags & ALT_FORM2) {
			SLJIT_ASSERT(src2 == TMP_REG2);
			return push_inst(compiler, ORIS | S(src1) | A(dst) | compiler->imm);
		}
		if (flags & ALT_FORM3) {
			SLJIT_ASSERT(src2 == TMP_REG2);
			FAIL_IF(push_inst(compiler, ORI | S(src1) | A(dst) | IMM(compiler->imm)));
			return push_inst(compiler, ORIS | S(dst) | A(dst) | IMM(compiler->imm >> 16));
		}
		return push_inst(compiler, OR | RC(flags) | S(src1) | A(dst) | B(src2));

	case SLJIT_XOR:
		if (flags & ALT_FORM1) {
			SLJIT_ASSERT(src2 == TMP_REG2);
			return push_inst(compiler, XORI | S(src1) | A(dst) | compiler->imm);
		}
		if (flags & ALT_FORM2) {
			SLJIT_ASSERT(src2 == TMP_REG2);
			return push_inst(compiler, XORIS | S(src1) | A(dst) | compiler->imm);
		}
		if (flags & ALT_FORM3) {
			SLJIT_ASSERT(src2 == TMP_REG2);
			FAIL_IF(push_inst(compiler, XORI | S(src1) | A(dst) | IMM(compiler->imm)));
			return push_inst(compiler, XORIS | S(dst) | A(dst) | IMM(compiler->imm >> 16));
		}
		return push_inst(compiler, XOR | RC(flags) | S(src1) | A(dst) | B(src2));

	case SLJIT_SHL:
		if (flags & ALT_FORM1) {
			SLJIT_ASSERT(src2 == TMP_REG2);
			if (flags & ALT_FORM2) {
				compiler->imm &= 0x1f;
				return push_inst(compiler, RLWINM | RC(flags) | S(src1) | A(dst) | (compiler->imm << 11) | ((31 - compiler->imm) << 1));
			}
			else {
				compiler->imm &= 0x3f;
				return push_inst(compiler, RLDI(dst, src1, compiler->imm, 63 - compiler->imm, 1) | RC(flags));
			}
		}
		if (flags & ALT_FORM2)
			return push_inst(compiler, SLW | RC(flags) | S(src1) | A(dst) | B(src2));
		return push_inst(compiler, SLD | RC(flags) | S(src1) | A(dst) | B(src2));

	case SLJIT_LSHR:
		if (flags & ALT_FORM1) {
			SLJIT_ASSERT(src2 == TMP_REG2);
			if (flags & ALT_FORM2) {
				compiler->imm &= 0x1f;
				return push_inst(compiler, RLWINM | RC(flags) | S(src1) | A(dst) | (((32 - compiler->imm) & 0x1f) << 11) | (compiler->imm << 6) | (31 << 1));
			}
			else {
				compiler->imm &= 0x3f;
				return push_inst(compiler, RLDI(dst, src1, 64 - compiler->imm, compiler->imm, 0) | RC(flags));
			}
		}
		if (flags & ALT_FORM2)
			return push_inst(compiler, SRW | RC(flags) | S(src1) | A(dst) | B(src2));
		return push_inst(compiler, SRD | RC(flags) | S(src1) | A(dst) | B(src2));

	case SLJIT_ASHR:
		if (flags & ALT_FORM1) {
			SLJIT_ASSERT(src2 == TMP_REG2);
			if (flags & ALT_FORM2) {
				compiler->imm &= 0x1f;
				return push_inst(compiler, SRAWI | RC(flags) | S(src1) | A(dst) | (compiler->imm << 11));
			}
			else {
				compiler->imm &= 0x3f;
				return push_inst(compiler, SRADI | RC(flags) | S(src1) | A(dst) | ((compiler->imm & 0x1f) << 11) | ((compiler->imm & 0x20) >> 4));
			}
		}
		if (flags & ALT_FORM2)
			return push_inst(compiler, SRAW | RC(flags) | S(src1) | A(dst) | B(src2));
		return push_inst(compiler, SRAD | RC(flags) | S(src1) | A(dst) | B(src2));

	case SLJIT_MOV:
		SLJIT_ASSERT(src1 == TMP_REG1);
		if (dst != src2)
			return push_inst(compiler, OR | S(src2) | A(dst) | B(src2));
		return SLJIT_SUCCESS;

	case SLJIT_MOV_UI:
	case SLJIT_MOV_SI:
		SLJIT_ASSERT(src1 == TMP_REG1);
		if ((flags & (REG_DEST | REG2_SOURCE)) == (REG_DEST | REG2_SOURCE)) {
			if (op == SLJIT_MOV_SI)
				return push_inst(compiler, EXTSW | S(src2) | A(dst));
			return push_inst(compiler, INS_CLEAR_LEFT(dst, src2, 0));
		}
		else if (dst != src2)
			SLJIT_ASSERT_STOP();
		return SLJIT_SUCCESS;

	case SLJIT_MOV_UB:
	case SLJIT_MOV_SB:
		SLJIT_ASSERT(src1 == TMP_REG1);
		if ((flags & (REG_DEST | REG2_SOURCE)) == (REG_DEST | REG2_SOURCE)) {
			if (op == SLJIT_MOV_SB)
				return push_inst(compiler, EXTSB | S(src2) | A(dst));
			return push_inst(compiler, INS_CLEAR_LEFT(dst, src2, 24));
		}
		else if ((flags & REG_DEST) && op == SLJIT_MOV_SB)
			return push_inst(compiler, EXTSB | S(src2) | A(dst));
		else if (dst != src2)
			SLJIT_ASSERT_STOP();
		return SLJIT_SUCCESS;

	case SLJIT_MOV_UH:
	case SLJIT_MOV_SH:
		SLJIT_ASSERT(src1 == TMP_REG1);
		if ((flags & (REG_DEST | REG2_SOURCE)) == (REG_DEST | REG2_SOURCE)) {
			if (op == SLJIT_MOV_SH)
				return push_inst(compiler, EXTSH | S(src2) | A(dst));
			return push_inst(compiler, INS_CLEAR_LEFT(dst, src2, 16));
		}
		else if (dst != src2)
			SLJIT_ASSERT_STOP();
		return SLJIT_SUCCESS;

	case SLJIT_NOT:
		SLJIT_ASSERT(src1 == TMP_REG1);
		UN_EXTS();
		return push_inst(compiler, NOR | RC(flags) | S(src2) | A(dst) | B(src2));

	case SLJIT_NEG:
		SLJIT_ASSERT(src1 == TMP_REG1);
		UN_EXTS();
		return push_inst(compiler, NEG | OERC(flags) | D(dst) | A(src2));

	case SLJIT_CLZ:
		SLJIT_ASSERT(src1 == TMP_REG1);
		if (flags & ALT_FORM1)
			return push_inst(compiler, CNTLZW | RC(flags) | S(src2) | A(dst));
		return push_inst(compiler, CNTLZD | RC(flags) | S(src2) | A(dst));
	}

	SLJIT_ASSERT_STOP();
	return SLJIT_SUCCESS;
}

static SLJIT_INLINE int emit_const(struct sljit_compiler *compiler, int reg, sljit_w init_value)
{
	FAIL_IF(push_inst(compiler, ADDIS | D(reg) | A(0) | IMM(init_value >> 48)));
	FAIL_IF(push_inst(compiler, ORI | S(reg) | A(reg) | IMM(init_value >> 32)));
	FAIL_IF(PUSH_RLDICR(reg, 31));
	FAIL_IF(push_inst(compiler, ORIS | S(reg) | A(reg) | IMM(init_value >> 16)));
	return push_inst(compiler, ORI | S(reg) | A(reg) | IMM(init_value));
}

SLJIT_API_FUNC_ATTRIBUTE void sljit_set_jump_addr(sljit_uw addr, sljit_uw new_addr)
{
	sljit_ins *inst = (sljit_ins*)addr;

	inst[0] = (inst[0] & 0xffff0000) | ((new_addr >> 48) & 0xffff);
	inst[1] = (inst[1] & 0xffff0000) | ((new_addr >> 32) & 0xffff);
	inst[3] = (inst[3] & 0xffff0000) | ((new_addr >> 16) & 0xffff);
	inst[4] = (inst[4] & 0xffff0000) | (new_addr & 0xffff);
	SLJIT_CACHE_FLUSH(inst, inst + 5);
}

SLJIT_API_FUNC_ATTRIBUTE void sljit_set_const(sljit_uw addr, sljit_w new_constant)
{
	sljit_ins *inst = (sljit_ins*)addr;

	inst[0] = (inst[0] & 0xffff0000) | ((new_constant >> 48) & 0xffff);
	inst[1] = (inst[1] & 0xffff0000) | ((new_constant >> 32) & 0xffff);
	inst[3] = (inst[3] & 0xffff0000) | ((new_constant >> 16) & 0xffff);
	inst[4] = (inst[4] & 0xffff0000) | (new_constant & 0xffff);
	SLJIT_CACHE_FLUSH(inst, inst + 5);
}

SLJIT_API_FUNC_ATTRIBUTE void sljit_set_function_context(void** func_ptr, struct sljit_function_context* context, sljit_w addr, void* func)
{
	sljit_w* ptrs;
	if (func_ptr)
		*func_ptr = (void*)context;
	ptrs = (sljit_w*)func;
	context->addr = addr ? addr : ptrs[0];
	context->r2 = ptrs[1];
	context->r11 = ptrs[2];
}
