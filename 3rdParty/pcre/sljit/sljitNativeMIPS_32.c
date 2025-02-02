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

/* mips 32-bit arch dependent functions. */

static int load_immediate(struct sljit_compiler *compiler, int dst_ar, sljit_w imm)
{
	if (!(imm & ~0xffff))
		return push_inst(compiler, ORI | SA(0) | TA(dst_ar) | IMM(imm), dst_ar);

	if (imm < 0 && imm >= SIMM_MIN)
		return push_inst(compiler, ADDIU | SA(0) | TA(dst_ar) | IMM(imm), dst_ar);

	FAIL_IF(push_inst(compiler, LUI | TA(dst_ar) | IMM(imm >> 16), dst_ar));
	return (imm & 0xffff) ? push_inst(compiler, ORI | SA(dst_ar) | TA(dst_ar) | IMM(imm), dst_ar) : SLJIT_SUCCESS;
}

#define EMIT_LOGICAL(op_imm, op_norm) \
	if (flags & SRC2_IMM) { \
		if (op & SLJIT_SET_E) \
			FAIL_IF(push_inst(compiler, op_imm | S(src1) | TA(EQUAL_FLAG) | IMM(src2), EQUAL_FLAG)); \
		if (CHECK_FLAGS(SLJIT_SET_E)) \
			FAIL_IF(push_inst(compiler, op_imm | S(src1) | T(dst) | IMM(src2), DR(dst))); \
	} \
	else { \
		if (op & SLJIT_SET_E) \
			FAIL_IF(push_inst(compiler, op_norm | S(src1) | T(src2) | DA(EQUAL_FLAG), EQUAL_FLAG)); \
		if (CHECK_FLAGS(SLJIT_SET_E)) \
			FAIL_IF(push_inst(compiler, op_norm | S(src1) | T(src2) | D(dst), DR(dst))); \
	}

#define EMIT_SHIFT(op_imm, op_norm) \
	if (flags & SRC2_IMM) { \
		if (op & SLJIT_SET_E) \
			FAIL_IF(push_inst(compiler, op_imm | T(src1) | DA(EQUAL_FLAG) | SH_IMM(src2), EQUAL_FLAG)); \
		if (CHECK_FLAGS(SLJIT_SET_E)) \
			FAIL_IF(push_inst(compiler, op_imm | T(src1) | D(dst) | SH_IMM(src2), DR(dst))); \
	} \
	else { \
		if (op & SLJIT_SET_E) \
			FAIL_IF(push_inst(compiler, op_norm | S(src2) | T(src1) | DA(EQUAL_FLAG), EQUAL_FLAG)); \
		if (CHECK_FLAGS(SLJIT_SET_E)) \
			FAIL_IF(push_inst(compiler, op_norm | S(src2) | T(src1) | D(dst), DR(dst))); \
	}

static SLJIT_INLINE int emit_single_op(struct sljit_compiler *compiler, int op, int flags,
	int dst, int src1, sljit_w src2)
{
	int overflow_ra = 0;

	switch (GET_OPCODE(op)) {
	case SLJIT_ADD:
		if (flags & SRC2_IMM) {
			if (op & SLJIT_SET_O) {
				FAIL_IF(push_inst(compiler, SRL | T(src1) | DA(TMP_EREG1) | SH_IMM(31), TMP_EREG1));
				if (src2 < 0)
					FAIL_IF(push_inst(compiler, XORI | SA(TMP_EREG1) | TA(TMP_EREG1) | IMM(1), TMP_EREG1));
			}
			if (op & SLJIT_SET_E)
				FAIL_IF(push_inst(compiler, ADDIU | S(src1) | TA(EQUAL_FLAG) | IMM(src2), EQUAL_FLAG));
			if (op & SLJIT_SET_C) {
				if (src2 >= 0)
					FAIL_IF(push_inst(compiler, ORI | S(src1) | TA(ULESS_FLAG) | IMM(src2), ULESS_FLAG));
				else {
					FAIL_IF(push_inst(compiler, ADDIU | SA(0) | TA(ULESS_FLAG) | IMM(src2), ULESS_FLAG));
					FAIL_IF(push_inst(compiler, OR | S(src1) | TA(ULESS_FLAG) | DA(ULESS_FLAG), ULESS_FLAG));
				}
			}
			/* dst may be the same as src1 or src2. */
			if (CHECK_FLAGS(SLJIT_SET_E))
				FAIL_IF(push_inst(compiler, ADDIU | S(src1) | T(dst) | IMM(src2), DR(dst)));
			if (op & SLJIT_SET_O) {
				FAIL_IF(push_inst(compiler, SRL | T(dst) | DA(OVERFLOW_FLAG) | SH_IMM(31), OVERFLOW_FLAG));
				if (src2 < 0)
					FAIL_IF(push_inst(compiler, XORI | SA(OVERFLOW_FLAG) | TA(OVERFLOW_FLAG) | IMM(1), OVERFLOW_FLAG));
			}
		}
		else {
			if (op & SLJIT_SET_O) {
				FAIL_IF(push_inst(compiler, XOR | S(src1) | T(src2) | DA(TMP_EREG1), TMP_EREG1));
				FAIL_IF(push_inst(compiler, SRL | TA(TMP_EREG1) | DA(TMP_EREG1) | SH_IMM(31), TMP_EREG1));
				if (src1 != dst)
					overflow_ra = DR(src1);
				else if (src2 != dst)
					overflow_ra = DR(src2);
				else {
					/* Rare ocasion. */
					FAIL_IF(push_inst(compiler, ADDU | S(src1) | TA(0) | DA(TMP_EREG2), TMP_EREG2));
					overflow_ra = TMP_EREG2;
				}
			}
			if (op & SLJIT_SET_E)
				FAIL_IF(push_inst(compiler, ADDU | S(src1) | T(src2) | DA(EQUAL_FLAG), EQUAL_FLAG));
			if (op & SLJIT_SET_C)
				FAIL_IF(push_inst(compiler, OR | S(src1) | T(src2) | DA(ULESS_FLAG), ULESS_FLAG));
			/* dst may be the same as src1 or src2. */
			if (CHECK_FLAGS(SLJIT_SET_E))
				FAIL_IF(push_inst(compiler, ADDU | S(src1) | T(src2) | D(dst), DR(dst)));
			if (op & SLJIT_SET_O) {
				FAIL_IF(push_inst(compiler, XOR | S(dst) | TA(overflow_ra) | DA(OVERFLOW_FLAG), OVERFLOW_FLAG));
				FAIL_IF(push_inst(compiler, SRL | TA(OVERFLOW_FLAG) | DA(OVERFLOW_FLAG) | SH_IMM(31), OVERFLOW_FLAG));
			}
		}

		/* a + b >= a | b (otherwise, the carry should be set to 1). */
		if (op & SLJIT_SET_C)
			FAIL_IF(push_inst(compiler, SLTU | S(dst) | TA(ULESS_FLAG) | DA(ULESS_FLAG), ULESS_FLAG));
		if (op & SLJIT_SET_O)
			return push_inst(compiler, MOVN | SA(0) | TA(TMP_EREG1) | DA(OVERFLOW_FLAG), OVERFLOW_FLAG);
		return SLJIT_SUCCESS;

	case SLJIT_ADDC:
		if (flags & SRC2_IMM) {
			if (op & SLJIT_SET_C) {
				if (src2 >= 0)
					FAIL_IF(push_inst(compiler, ORI | S(src1) | TA(TMP_EREG1) | IMM(src2), TMP_EREG1));
				else {
					FAIL_IF(push_inst(compiler, ADDIU | SA(0) | TA(TMP_EREG1) | IMM(src2), TMP_EREG1));
					FAIL_IF(push_inst(compiler, OR | S(src1) | TA(TMP_EREG1) | DA(TMP_EREG1), TMP_EREG1));
				}
			}
			FAIL_IF(push_inst(compiler, ADDIU | S(src1) | T(dst) | IMM(src2), DR(dst)));
		} else {
			if (op & SLJIT_SET_C)
				FAIL_IF(push_inst(compiler, OR | S(src1) | T(src2) | DA(TMP_EREG1), TMP_EREG1));
			/* dst may be the same as src1 or src2. */
			FAIL_IF(push_inst(compiler, ADDU | S(src1) | T(src2) | D(dst), DR(dst)));
		}
		if (op & SLJIT_SET_C)
			FAIL_IF(push_inst(compiler, SLTU | S(dst) | TA(TMP_EREG1) | DA(TMP_EREG1), TMP_EREG1));

		FAIL_IF(push_inst(compiler, ADDU | S(dst) | TA(ULESS_FLAG) | D(dst), DR(dst)));
		if (!(op & SLJIT_SET_C))
			return SLJIT_SUCCESS;

		/* Set TMP_EREG2 (dst == 0) && (ULESS_FLAG == 1). */
		FAIL_IF(push_inst(compiler, SLTIU | S(dst) | TA(TMP_EREG2) | IMM(1), TMP_EREG2));
		FAIL_IF(push_inst(compiler, AND | SA(TMP_EREG2) | TA(ULESS_FLAG) | DA(TMP_EREG2), TMP_EREG2));
		/* Set carry flag. */
		return push_inst(compiler, OR | SA(TMP_EREG2) | TA(TMP_EREG1) | DA(ULESS_FLAG), ULESS_FLAG);

	case SLJIT_SUB:
		if ((flags & SRC2_IMM) && ((op & (SLJIT_SET_S | SLJIT_SET_U)) || src2 == SIMM_MIN)) {
			FAIL_IF(push_inst(compiler, ADDIU | SA(0) | T(TMP_REG2) | IMM(src2), DR(TMP_REG2)));
			src2 = TMP_REG2;
			flags &= ~SRC2_IMM;
		}

		if (flags & SRC2_IMM) {
			if (op & SLJIT_SET_O) {
				FAIL_IF(push_inst(compiler, SRL | T(src1) | DA(TMP_EREG1) | SH_IMM(31), TMP_EREG1));
				if (src2 < 0)
					FAIL_IF(push_inst(compiler, XORI | SA(TMP_EREG1) | TA(TMP_EREG1) | IMM(1), TMP_EREG1));
				if (src1 != dst)
					overflow_ra = DR(src1);
				else {
					/* Rare ocasion. */
					FAIL_IF(push_inst(compiler, ADDU | S(src1) | TA(0) | DA(TMP_EREG2), TMP_EREG2));
					overflow_ra = TMP_EREG2;
				}
			}
			if (op & SLJIT_SET_E)
				FAIL_IF(push_inst(compiler, ADDIU | S(src1) | TA(EQUAL_FLAG) | IMM(-src2), EQUAL_FLAG));
			if (op & SLJIT_SET_C)
				FAIL_IF(push_inst(compiler, SLTIU | S(src1) | TA(ULESS_FLAG) | IMM(src2), ULESS_FLAG));
			/* dst may be the same as src1 or src2. */
			if (CHECK_FLAGS(SLJIT_SET_E))
				FAIL_IF(push_inst(compiler, ADDIU | S(src1) | T(dst) | IMM(-src2), DR(dst)));
		}
		else {
			if (op & SLJIT_SET_O) {
				FAIL_IF(push_inst(compiler, XOR | S(src1) | T(src2) | DA(TMP_EREG1), TMP_EREG1));
				FAIL_IF(push_inst(compiler, SRL | TA(TMP_EREG1) | DA(TMP_EREG1) | SH_IMM(31), TMP_EREG1));
				if (src1 != dst)
					overflow_ra = DR(src1);
				else {
					/* Rare ocasion. */
					FAIL_IF(push_inst(compiler, ADDU | S(src1) | TA(0) | DA(TMP_EREG2), TMP_EREG2));
					overflow_ra = TMP_EREG2;
				}
			}
			if (op & SLJIT_SET_E)
				FAIL_IF(push_inst(compiler, SUBU | S(src1) | T(src2) | DA(EQUAL_FLAG), EQUAL_FLAG));
			if (op & (SLJIT_SET_U | SLJIT_SET_C))
				FAIL_IF(push_inst(compiler, SLTU | S(src1) | T(src2) | DA(ULESS_FLAG), ULESS_FLAG));
			if (op & SLJIT_SET_U)
				FAIL_IF(push_inst(compiler, SLTU | S(src2) | T(src1) | DA(UGREATER_FLAG), UGREATER_FLAG));
			if (op & SLJIT_SET_S) {
				FAIL_IF(push_inst(compiler, SLT | S(src1) | T(src2) | DA(LESS_FLAG), LESS_FLAG));
				FAIL_IF(push_inst(compiler, SLT | S(src2) | T(src1) | DA(GREATER_FLAG), GREATER_FLAG));
			}
			/* dst may be the same as src1 or src2. */
			if (CHECK_FLAGS(SLJIT_SET_E | SLJIT_SET_S | SLJIT_SET_U | SLJIT_SET_C))
				FAIL_IF(push_inst(compiler, SUBU | S(src1) | T(src2) | D(dst), DR(dst)));
		}

		if (op & SLJIT_SET_O) {
			FAIL_IF(push_inst(compiler, XOR | S(dst) | TA(overflow_ra) | DA(OVERFLOW_FLAG), OVERFLOW_FLAG));
			FAIL_IF(push_inst(compiler, SRL | TA(OVERFLOW_FLAG) | DA(OVERFLOW_FLAG) | SH_IMM(31), OVERFLOW_FLAG));
			return push_inst(compiler, MOVZ | SA(0) | TA(TMP_EREG1) | DA(OVERFLOW_FLAG), OVERFLOW_FLAG);
		}
		return SLJIT_SUCCESS;

	case SLJIT_SUBC:
		if ((flags & SRC2_IMM) && src2 == SIMM_MIN) {
			FAIL_IF(push_inst(compiler, ADDIU | SA(0) | T(TMP_REG2) | IMM(src2), DR(TMP_REG2)));
			src2 = TMP_REG2;
			flags &= ~SRC2_IMM;
		}

		if (flags & SRC2_IMM) {
			if (op & SLJIT_SET_C)
				FAIL_IF(push_inst(compiler, SLTIU | S(src1) | TA(TMP_EREG1) | IMM(-src2), TMP_EREG1));
			/* dst may be the same as src1 or src2. */
			FAIL_IF(push_inst(compiler, ADDIU | S(src1) | T(dst) | IMM(-src2), DR(dst)));
		}
		else {
			if (op & SLJIT_SET_C)
				FAIL_IF(push_inst(compiler, SLTU | S(src1) | T(src2) | DA(TMP_EREG1), TMP_EREG1));
			/* dst may be the same as src1 or src2. */
			FAIL_IF(push_inst(compiler, SUBU | S(src1) | T(src2) | D(dst), DR(dst)));
		}

		if (op & SLJIT_SET_C)
			FAIL_IF(push_inst(compiler, MOVZ | SA(ULESS_FLAG) | T(dst) | DA(TMP_EREG1), TMP_EREG1));

		FAIL_IF(push_inst(compiler, SUBU | S(dst) | TA(ULESS_FLAG) | D(dst), DR(dst)));

		if (op & SLJIT_SET_C)
			FAIL_IF(push_inst(compiler, ADDU | SA(TMP_EREG1) | TA(0) | DA(ULESS_FLAG), ULESS_FLAG));

		return SLJIT_SUCCESS;

	case SLJIT_MUL:
		SLJIT_ASSERT(!(flags & SRC2_IMM));
		if (!(op & SLJIT_SET_O)) {
#if (defined SLJIT_MIPS_32_64 && SLJIT_MIPS_32_64)
			return push_inst(compiler, MUL | S(src1) | T(src2) | D(dst), DR(dst));
#else
			FAIL_IF(push_inst(compiler, MULT | S(src1) | T(src2), MOVABLE_INS));
			return push_inst(compiler, MFLO | D(dst), DR(dst));
#endif
		}
		FAIL_IF(push_inst(compiler, MULT | S(src1) | T(src2), MOVABLE_INS));
		FAIL_IF(push_inst(compiler, MFHI | DA(TMP_EREG1), TMP_EREG1));
		FAIL_IF(push_inst(compiler, MFLO | D(dst), DR(dst)));
		FAIL_IF(push_inst(compiler, SRA | T(dst) | DA(TMP_EREG2) | SH_IMM(31), TMP_EREG2));
		return push_inst(compiler, SUBU | SA(TMP_EREG1) | TA(TMP_EREG2) | DA(OVERFLOW_FLAG), OVERFLOW_FLAG);

	case SLJIT_AND:
		EMIT_LOGICAL(ANDI, AND);
		return SLJIT_SUCCESS;

	case SLJIT_OR:
		EMIT_LOGICAL(ORI, OR);
		return SLJIT_SUCCESS;

	case SLJIT_XOR:
		EMIT_LOGICAL(XORI, XOR);
		return SLJIT_SUCCESS;

	case SLJIT_SHL:
		EMIT_SHIFT(SLL, SLLV);
		return SLJIT_SUCCESS;

	case SLJIT_LSHR:
		EMIT_SHIFT(SRL, SRLV);
		return SLJIT_SUCCESS;

	case SLJIT_ASHR:
		EMIT_SHIFT(SRA, SRAV);
		return SLJIT_SUCCESS;

	case SLJIT_MOV:
	case SLJIT_MOV_UI:
	case SLJIT_MOV_SI:
		SLJIT_ASSERT(src1 == TMP_REG1);
		if (dst != src2)
			return push_inst(compiler, ADDU | S(src2) | TA(0) | D(dst), DR(dst));
		return SLJIT_SUCCESS;

	case SLJIT_MOV_UB:
	case SLJIT_MOV_SB:
		SLJIT_ASSERT(src1 == TMP_REG1);
		if ((flags & (REG_DEST | REG2_SOURCE)) == (REG_DEST | REG2_SOURCE)) {
			if (op == SLJIT_MOV_SB) {
#if (defined SLJIT_MIPS_32_64 && SLJIT_MIPS_32_64)
				return push_inst(compiler, SEB | T(src2) | D(dst), DR(dst));
#else
				FAIL_IF(push_inst(compiler, SLL | T(src2) | D(dst) | SH_IMM(24), DR(dst)));
				return push_inst(compiler, SRA | T(dst) | D(dst) | SH_IMM(24), DR(dst));
#endif
			}
			return push_inst(compiler, ANDI | S(src2) | T(dst) | IMM(0xff), DR(dst));
		}
		else if (dst != src2)
			SLJIT_ASSERT_STOP();
		return SLJIT_SUCCESS;

	case SLJIT_MOV_UH:
	case SLJIT_MOV_SH:
		SLJIT_ASSERT(src1 == TMP_REG1);
		if ((flags & (REG_DEST | REG2_SOURCE)) == (REG_DEST | REG2_SOURCE)) {
			if (op == SLJIT_MOV_SH) {
#if (defined SLJIT_MIPS_32_64 && SLJIT_MIPS_32_64)
				return push_inst(compiler, SEH | T(src2) | D(dst), DR(dst));
#else
				FAIL_IF(push_inst(compiler, SLL | T(src2) | D(dst) | SH_IMM(16), DR(dst)));
				return push_inst(compiler, SRA | T(dst) | D(dst) | SH_IMM(16), DR(dst));
#endif
			}
			return push_inst(compiler, ANDI | S(src2) | T(dst) | IMM(0xffff), DR(dst));
		}
		else if (dst != src2)
			SLJIT_ASSERT_STOP();
		return SLJIT_SUCCESS;

	case SLJIT_NOT:
		SLJIT_ASSERT(src1 == TMP_REG1 && !(flags & SRC2_IMM));
		if (op & SLJIT_SET_E)
			FAIL_IF(push_inst(compiler, NOR | S(src2) | T(src2) | DA(EQUAL_FLAG), EQUAL_FLAG));
		if (CHECK_FLAGS(SLJIT_SET_E))
			FAIL_IF(push_inst(compiler, NOR | S(src2) | T(src2) | D(dst), DR(dst)));
		return SLJIT_SUCCESS;

	case SLJIT_CLZ:
		SLJIT_ASSERT(src1 == TMP_REG1 && !(flags & SRC2_IMM));
#if (defined SLJIT_MIPS_32_64 && SLJIT_MIPS_32_64)
		if (op & SLJIT_SET_E)
			FAIL_IF(push_inst(compiler, CLZ | S(src2) | TA(EQUAL_FLAG) | DA(EQUAL_FLAG), EQUAL_FLAG));
		if (CHECK_FLAGS(SLJIT_SET_E))
			FAIL_IF(push_inst(compiler, CLZ | S(src2) | T(dst) | D(dst), DR(dst)));
#else
		if (SLJIT_UNLIKELY(flags & UNUSED_DEST)) {
			FAIL_IF(push_inst(compiler, SRL | T(src2) | DA(EQUAL_FLAG) | SH_IMM(31), EQUAL_FLAG));
			return push_inst(compiler, XORI | SA(EQUAL_FLAG) | TA(EQUAL_FLAG) | IMM(1), EQUAL_FLAG);
		}
		/* Nearly all instructions are unmovable in the following sequence. */
		FAIL_IF(push_inst(compiler, ADDU_W | S(src2) | TA(0) | D(TMP_REG1), DR(TMP_REG1)));
		/* Check zero. */
		FAIL_IF(push_inst(compiler, BEQ | S(TMP_REG1) | TA(0) | IMM(6), UNMOVABLE_INS));
		FAIL_IF(push_inst(compiler, ORI | SA(0) | T(dst) | IMM(32), UNMOVABLE_INS));
		/* Check sign bit. */
		FAIL_IF(push_inst(compiler, BLTZ | S(TMP_REG1) | IMM(4), UNMOVABLE_INS));
		FAIL_IF(push_inst(compiler, ORI | SA(0) | T(dst) | IMM(0), UNMOVABLE_INS));
		/* Loop for searching the highest bit. */
		FAIL_IF(push_inst(compiler, SLL | T(TMP_REG1) | D(TMP_REG1) | SH_IMM(1), DR(TMP_REG1)));
		FAIL_IF(push_inst(compiler, BGEZ | S(TMP_REG1) | IMM(-2), UNMOVABLE_INS));
		FAIL_IF(push_inst(compiler, ADDIU_W | S(dst) | T(dst) | IMM(1), UNMOVABLE_INS));
		if (op & SLJIT_SET_E)
			return push_inst(compiler, ADDU_W | S(dst) | TA(0) | DA(EQUAL_FLAG), EQUAL_FLAG);
#endif
		return SLJIT_SUCCESS;
	}

	SLJIT_ASSERT_STOP();
	return SLJIT_SUCCESS;
}

static SLJIT_INLINE int emit_const(struct sljit_compiler *compiler, int reg, sljit_w init_value)
{
	FAIL_IF(push_inst(compiler, LUI | T(reg) | IMM(init_value >> 16), DR(reg)));
	return push_inst(compiler, ORI | S(reg) | T(reg) | IMM(init_value), DR(reg));
}

SLJIT_API_FUNC_ATTRIBUTE void sljit_set_jump_addr(sljit_uw addr, sljit_uw new_addr)
{
	sljit_ins *inst = (sljit_ins*)addr;

	inst[0] = (inst[0] & 0xffff0000) | ((new_addr >> 16) & 0xffff);
	inst[1] = (inst[1] & 0xffff0000) | (new_addr & 0xffff);
	SLJIT_CACHE_FLUSH(inst, inst + 2);
}

SLJIT_API_FUNC_ATTRIBUTE void sljit_set_const(sljit_uw addr, sljit_w new_constant)
{
	sljit_ins *inst = (sljit_ins*)addr;

	inst[0] = (inst[0] & 0xffff0000) | ((new_constant >> 16) & 0xffff);
	inst[1] = (inst[1] & 0xffff0000) | (new_constant & 0xffff);
	SLJIT_CACHE_FLUSH(inst, inst + 2);
}
