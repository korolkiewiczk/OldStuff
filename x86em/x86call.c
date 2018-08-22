/*
  X86 EMULATOR by Kamil Korolkiewicz 2007
  Plik:x86call.c
  Opis:Wywolywane funkcje symulujace instrukcje procesora
*/



/****************
  MOV
*****************/

void x86_MOV_b()
{
 rm_get(par2,rm_byte);
 rm_set(par1,rm_byte);
}

void x86_MOV_w()
{
}

void x86_MOV_d()
{
}

/****************
  STOS
*****************/

void x86_STOS_b()
{
}

void x86_STOS_w()
{
}

void x86_STOS_d()
{
}

/****************
  MOVS
*****************/

void x86_MOVS_b()
{
}

void x86_MOVS_w()
{
}

void x86_MOVS_d()
{
}

/****************
  CMPS
*****************/

void x86_CMPS_b()
{
}

void x86_CMPS_w()
{
}

void x86_CMPS_d()
{
}

/****************
  XCHG
*****************/

void x86_XCHG_b()
{
}

void x86_XCHG_w()
{
}

void x86_XCHG_d()
{
}

/****************
  NOP
*****************/

void x86_NOP()
{
}


/****************
  PUSH
*****************/

void x86_PUSH_b()
{
}

void x86_PUSH_w()
{
 rm_get(par1,rm_word);
}

void x86_PUSH_d()
{
}

/****************
  POP
*****************/

void x86_POP_b()
{
}

void x86_POP_w()
{
}

void x86_POP_d()
{
}

/****************
  ADD
*****************/

void x86_ADD_b()
{
}

void x86_ADD_w()
{
}

void x86_ADD_d()
{
}

/****************
  OR
*****************/

void x86_OR_b()
{
}

void x86_OR_w()
{
}

void x86_OR_d()
{
}

/****************
  ADC
*****************/

void x86_ADC_b()
{
}

void x86_ADC_w()
{
}

void x86_ADC_d()
{
}

/****************
  SBB
*****************/

void x86_SBB_b()
{
}

void x86_SBB_w()
{
}

void x86_SBB_d()
{
}

/****************
  AND
*****************/

void x86_AND_b()
{
}

void x86_AND_w()
{
}

void x86_AND_d()
{
}

/****************
  SUB
*****************/

void x86_SUB_b()
{
}

void x86_SUB_w()
{
}

void x86_SUB_d()
{
}

/****************
  XOR
*****************/

void x86_XOR_b()
{
}

void x86_XOR_w()
{
}

void x86_XOR_d()
{
}

/****************
  CMP
*****************/

void x86_CMP_b()
{
}

void x86_CMP_w()
{
}

void x86_CMP_d()
{
}

/****************
  ADDX
*****************/

void x86_ADDX_b()
{
}

void x86_ADDX_w()
{
}

void x86_ADDX_d()
{
}

/****************
  ORX
*****************/

void x86_ORX_b()
{
}

void x86_ORX_w()
{
}

void x86_ORX_d()
{
}

/****************
  ADCX
*****************/

void x86_ADCX_b()
{
}

void x86_ADCX_w()
{
}

void x86_ADCX_d()
{
}

/****************
  SBBX
*****************/

void x86_SBBX_b()
{
}

void x86_SBBX_w()
{
}

void x86_SBBX_d()
{
}

/****************
  ANDX
*****************/

void x86_ANDX_b()
{
}

void x86_ANDX_w()
{
}

void x86_ANDX_d()
{
}

/****************
  SUBX
*****************/

void x86_SUBX_b()
{
}

void x86_SUBX_w()
{
}

void x86_SUBX_d()
{
}

/****************
  XORX
*****************/

void x86_XORX_b()
{
}

void x86_XORX_w()
{
}

void x86_XORX_d()
{
}

/****************
  CMPX
*****************/

void x86_CMPX_b()
{
}

void x86_CMPX_w()
{
}

void x86_CMPX_d()
{
}

/****************
  INC
*****************/

void x86_INC_b()
{
}

void x86_INC_w()
{
}

void x86_INC_d()
{
}

/****************
  DEC
*****************/

void x86_DEC_b()
{
}

void x86_DEC_w()
{
}

void x86_DEC_d()
{
}

/****************
  CALLN
*****************/

void x86_CALLN_b()
{
}

void x86_CALLN_w()
{
}

void x86_CALLN_d()
{
}

/****************
  CALLF
*****************/

void x86_CALLF_b()
{
}

void x86_CALLF_w()
{
}

void x86_CALLF_d()
{
}

/****************
  JMPN
*****************/

void x86_JMPN_b()
{
}

void x86_JMPN_w()
{
}

void x86_JMPN_d()
{
}

/****************
  JMPF
*****************/

void x86_JMPF_b()
{
}

void x86_JMPF_w()
{
}

void x86_JMPF_d()
{
}

/****************
  DAA
*****************/

void x86_DAA_b()
{
}

void x86_DAA_w()
{
}

void x86_DAA_d()
{
}

/****************
  DAS
*****************/

void x86_DAS_b()
{
}

void x86_DAS_w()
{
}

void x86_DAS_d()
{
}

/****************
  AAA
*****************/

void x86_AAA_b()
{
}

void x86_AAA_w()
{
}

void x86_AAA_d()
{
}

/****************
  AAS
*****************/

void x86_AAS_b()
{
}

void x86_AAS_w()
{
}

void x86_AAS_d()
{
}

/****************
  PUSHA
*****************/

void x86_PUSHA_b()
{
}

void x86_PUSHA_w()
{
}

void x86_PUSHA_d()
{
}

/****************
  POPA
*****************/

void x86_POPA_b()
{
}

void x86_POPA_w()
{
}

void x86_POPA_d()
{
}

/****************
  LEA
*****************/

void x86_LEA_b()
{
}

void x86_LEA_w()
{
}

void x86_LEA_d()
{
}

/****************
  CBW
*****************/

void x86_CBW_b()
{
}

void x86_CBW_w()
{
}

void x86_CBW_d()
{
}

/****************
  CWD
*****************/

void x86_CWD_b()
{
}

void x86_CWD_w()
{
}

void x86_CWD_d()
{
}

/****************
  TEST
*****************/

void x86_TEST_b()
{
}

void x86_TEST_w()
{
}

void x86_TEST_d()
{
}

/****************
  NOT
*****************/

void x86_NOT_b()
{
}

void x86_NOT_w()
{
}

void x86_NOT_d()
{
}

/****************
  NEG
*****************/

void x86_NEG_b()
{
}

void x86_NEG_w()
{
}

void x86_NEG_d()
{
}

/****************
  MUL
*****************/

void x86_MUL_b()
{
}

void x86_MUL_w()
{
}

void x86_MUL_d()
{
}

/****************
  IMUL
*****************/

void x86_IMUL_b()
{
}

void x86_IMUL_w()
{
}

void x86_IMUL_d()
{
}

/****************
  DIV
*****************/

void x86_DIV_b()
{
}

void x86_DIV_w()
{
}

void x86_DIV_d()
{
}

/****************
  IDIV
*****************/

void x86_IDIV_b()
{
}

void x86_IDIV_w()
{
}

void x86_IDIV_d()
{
}

/****************
  HLT
*****************/

void x86_HLT_b()
{
}

void x86_HLT_w()
{
}

void x86_HLT_d()
{
}

/****************
  ROR
*****************/

void x86_ROR_b()
{
}

void x86_ROR_w()
{
}

void x86_ROR_d()
{
}

/****************
  ROL
*****************/

void x86_ROL_b()
{
}

void x86_ROL_w()
{
}

void x86_ROL_d()
{
}

/****************
  RCL
*****************/

void x86_RCL_b()
{
}

void x86_RCL_w()
{
}

void x86_RCL_d()
{
}

/****************
  RCR
*****************/

void x86_RCR_b()
{
}

void x86_RCR_w()
{
}

void x86_RCR_d()
{
}

/****************
  SHL
*****************/

void x86_SHL_b()
{
}

void x86_SHL_w()
{
}

void x86_SHL_d()
{
}

/****************
  SHR
*****************/

void x86_SHR_b()
{
}

void x86_SHR_w()
{
}

void x86_SHR_d()
{
}

/****************
  SAR
*****************/

void x86_SAR_b()
{
}

void x86_SAR_w()
{
}

void x86_SAR_d()
{
}

/****************
  RETN
*****************/

void x86_RETN_b()
{
}

void x86_RETN_w()
{
}

void x86_RETN_d()
{
}

/****************
  RETF
*****************/

void x86_RETF_b()
{
}

void x86_RETF_w()
{
}

void x86_RETF_d()
{
}

/****************
  INP
*****************/

void x86_INP_b()
{
}

void x86_INP_w()
{
}

void x86_INP_d()
{
}

/****************
  OUTP
*****************/

void x86_OUTP_b()
{
}

void x86_OUTP_w()
{
}

void x86_OUTP_d()
{
}

/****************
  SCAS
*****************/

void x86_SCAS_b()
{
}

void x86_SCAS_w()
{
}

void x86_SCAS_d()
{
}

/****************
  XLAT
*****************/

void x86_XLAT_b()
{
}

void x86_XLAT_w()
{
}

void x86_XLAT_d()
{
}

/****************
  AAM
*****************/

void x86_AAM_b()
{
}

void x86_AAM_w()
{
}

void x86_AAM_d()
{
}

/****************
  AAD
*****************/

void x86_AAD_b()
{
}

void x86_AAD_w()
{
}

void x86_AAD_d()
{
}

/****************
  CALL
*****************/

void x86_CALL_b()
{
}

void x86_CALL_w()
{
}

void x86_CALL_d()
{
}

/****************
  JMP
*****************/

void x86_JMP_b()
{
}

void x86_JMP_w()
{
}

void x86_JMP_d()
{
}

/****************
  LODS
*****************/

void x86_LODS_b()
{
}

void x86_LODS_w()
{
}

void x86_LODS_d()
{
}

/****************
  INT3
*****************/

void x86_INT3_b()
{
}

void x86_INT3_w()
{
}

void x86_INT3_d()
{
}

/****************
  INTN
*****************/

void x86_INTN_b()
{
}

void x86_INTN_w()
{
}

void x86_INTN_d()
{
}

/****************
  INTO
*****************/

void x86_INTO_b()
{
}

void x86_INTO_w()
{
}

void x86_INTO_d()
{
}

/****************
  IRET
*****************/

void x86_IRET_b()
{
}

void x86_IRET_w()
{
}

void x86_IRET_d()
{
}

/****************
  ENTER
*****************/

void x86_ENTER_b()
{
}

void x86_ENTER_w()
{
}

void x86_ENTER_d()
{
}

/****************
  LEAVE
*****************/

void x86_LEAVE_b()
{
}

void x86_LEAVE_w()
{
}

void x86_LEAVE_d()
{
}

/****************
  LES
*****************/

void x86_LES_b()
{
}

void x86_LES_w()
{
}

void x86_LES_d()
{
}

/****************
  LDS
*****************/

void x86_LDS_b()
{
}

void x86_LDS_w()
{
}

void x86_LDS_d()
{
}

/****************
  INS
*****************/

void x86_INS_b()
{
}

void x86_INS_w()
{
}

void x86_INS_d()
{
}

/****************
  OUTS
*****************/

void x86_OUTS_b()
{
}

void x86_OUTS_w()
{
}

void x86_OUTS_d()
{
}

/****************
  LOOPNE
*****************/

void x86_LOOPNE_b()
{
}

void x86_LOOPNE_w()
{
}

void x86_LOOPNE_d()
{
}

/****************
  LOOPE
*****************/

void x86_LOOPE_b()
{
}

void x86_LOOPE_w()
{
}

void x86_LOOPE_d()
{
}

/****************
  LOOP
*****************/

void x86_LOOP_b()
{
}

void x86_LOOP_w()
{
}

void x86_LOOP_d()
{
}

/****************
  JCXZ
*****************/

void x86_JCXZ_b()
{
}

void x86_JCXZ_w()
{
}

void x86_JCXZ_d()
{
}

/****************
  CLC
*****************/

void x86_CLC_b()
{
}

void x86_CLC_w()
{
}

void x86_CLC_d()
{
}

/****************
  STC
*****************/

void x86_STC_b()
{
}

void x86_STC_w()
{
}

void x86_STC_d()
{
}

/****************
  CLI
*****************/

void x86_CLI_b()
{
}

void x86_CLI_w()
{
}

void x86_CLI_d()
{
}

/****************
  STI
*****************/

void x86_STI_b()
{
}

void x86_STI_w()
{
}

void x86_STI_d()
{
}

/****************
  CLD
*****************/

void x86_CLD_b()
{
}

void x86_CLD_w()
{
}

void x86_CLD_d()
{
}

/****************
  STD
*****************/

void x86_STD_b()
{
}

void x86_STD_w()
{
}

void x86_STD_d()
{
}

/****************
  CMC
*****************/

void x86_CMC_b()
{
}

void x86_CMC_w()
{
}

void x86_CMC_d()
{
}

/****************
  WAIT
*****************/

void x86_WAIT_b()
{
}

void x86_WAIT_w()
{
}

void x86_WAIT_d()
{
}

/****************
  PUSHF
*****************/

void x86_PUSHF_b()
{
}

void x86_PUSHF_w()
{
}

void x86_PUSHF_d()
{
}

/****************
  POPF
*****************/

void x86_POPF_b()
{
}

void x86_POPF_w()
{
}

void x86_POPF_d()
{
}

/****************
  SAHF
*****************/

void x86_SAHF_b()
{
}

void x86_SAHF_w()
{
}

void x86_SAHF_d()
{
}

/****************
  LAHF
*****************/

void x86_LAHF_b()
{
}

void x86_LAHF_w()
{
}

void x86_LAHF_d()
{
}

/****************
  BOUND
*****************/

void x86_BOUND_b()
{
}

void x86_BOUND_w()
{
}

void x86_BOUND_d()
{
}

/****************
  ARPL
*****************/

void x86_ARPL_b()
{
}

void x86_ARPL_w()
{
}

void x86_ARPL_d()
{
}

/****************
  JO
*****************/

void x86_JO_b()
{
}

void x86_JO_w()
{
}

void x86_JO_d()
{
}

/****************
  JNO
*****************/

void x86_JNO_b()
{
}

void x86_JNO_w()
{
}

void x86_JNO_d()
{
}

/****************
  JC
*****************/

void x86_JC_b()
{
}

void x86_JC_w()
{
}

void x86_JC_d()
{
}

/****************
  JNC
*****************/

void x86_JNC_b()
{
}

void x86_JNC_w()
{
}

void x86_JNC_d()
{
}

/****************
  JZ
*****************/

void x86_JZ_b()
{
}

void x86_JZ_w()
{
}

void x86_JZ_d()
{
}

/****************
  JNZ
*****************/

void x86_JNZ_b()
{
}

void x86_JNZ_w()
{
}

void x86_JNZ_d()
{
}

/****************
  JCZ
*****************/

void x86_JCZ_b()
{
}

void x86_JCZ_w()
{
}

void x86_JCZ_d()
{
}

/****************
  JNCZ
*****************/

void x86_JNCZ_b()
{
}

void x86_JNCZ_w()
{
}

void x86_JNCZ_d()
{
}

/****************
  JS
*****************/

void x86_JS_b()
{
}

void x86_JS_w()
{
}

void x86_JS_d()
{
}

/****************
  JNS
*****************/

void x86_JNS_b()
{
}

void x86_JNS_w()
{
}

void x86_JNS_d()
{
}

/****************
  JP
*****************/

void x86_JP_b()
{
}

void x86_JP_w()
{
}

void x86_JP_d()
{
}

/****************
  JNP
*****************/

void x86_JNP_b()
{
}

void x86_JNP_w()
{
}

void x86_JNP_d()
{
}

/****************
  JL
*****************/

void x86_JL_b()
{
}

void x86_JL_w()
{
}

void x86_JL_d()
{
}

/****************
  JNL
*****************/

void x86_JNL_b()
{
}

void x86_JNL_w()
{
}

void x86_JNL_d()
{
}

/****************
  JLE
*****************/

void x86_JLE_b()
{
}

void x86_JLE_w()
{
}

void x86_JLE_d()
{
}

/****************
  JNLE
*****************/

void x86_JNLE_b()
{
}

void x86_JNLE_w()
{
}

void x86_JNLE_d()
{
}

/****************
  SHLD
*****************/

void x86_SHLD_b()
{
}

void x86_SHLD_w()
{
}

void x86_SHLD_d()
{
}

/****************
  SHRD
*****************/

void x86_SHRD_b()
{
}

void x86_SHRD_w()
{
}

void x86_SHRD_d()
{
}

/****************
  SLDT
*****************/

void x86_SLDT_b()
{
}

void x86_SLDT_w()
{
}

void x86_SLDT_d()
{
}

/****************
  SGDT
*****************/

void x86_SGDT_b()
{
}

void x86_SGDT_w()
{
}

void x86_SGDT_d()
{
}

/****************
  LLDT
*****************/

void x86_LLDT_b()
{
}

void x86_LLDT_w()
{
}

void x86_LLDT_d()
{
}

/****************
  LGDT
*****************/

void x86_LGDT_b()
{
}

void x86_LGDT_w()
{
}

void x86_LGDT_d()
{
}

/****************
  LTR
*****************/

void x86_LTR_b()
{
}

void x86_LTR_w()
{
}

void x86_LTR_d()
{
}

/****************
  STR
*****************/

void x86_STR_b()
{
}

void x86_STR_w()
{
}

void x86_STR_d()
{
}

/****************
  LIDT
*****************/

void x86_LIDT_b()
{
}

void x86_LIDT_w()
{
}

void x86_LIDT_d()
{
}

/****************
  SIDT
*****************/

void x86_SIDT_b()
{
}

void x86_SIDT_w()
{
}

void x86_SIDT_d()
{
}

/****************
  SMSW
*****************/

void x86_SMSW_b()
{
}

void x86_SMSW_w()
{
}

void x86_SMSW_d()
{
}

/****************
  LMSW
*****************/

void x86_LMSW_b()
{
}

void x86_LMSW_w()
{
}

void x86_LMSW_d()
{
}

/****************
  VERR
*****************/

void x86_VERR_b()
{
}

void x86_VERR_w()
{
}

void x86_VERR_d()
{
}

/****************
  VERW
*****************/

void x86_VERW_b()
{
}

void x86_VERW_w()
{
}

void x86_VERW_d()
{
}

/****************
  INVLPG
*****************/

void x86_INVLPG_b()
{
}

void x86_INVLPG_w()
{
}

void x86_INVLPG_d()
{
}

/****************
  FADD
*****************/

void x86_FADD_b()
{
}

void x86_FADD_w()
{
}

void x86_FADD_d()
{
}

/****************
  FMUL
*****************/

void x86_FMUL_b()
{
}

void x86_FMUL_w()
{
}

void x86_FMUL_d()
{
}

/****************
  FCOM
*****************/

void x86_FCOM_b()
{
}

void x86_FCOM_w()
{
}

void x86_FCOM_d()
{
}

/****************
  FCOMP
*****************/

void x86_FCOMP_b()
{
}

void x86_FCOMP_w()
{
}

void x86_FCOMP_d()
{
}

/****************
  FSUB
*****************/

void x86_FSUB_b()
{
}

void x86_FSUB_w()
{
}

void x86_FSUB_d()
{
}

/****************
  FSUBR
*****************/

void x86_FSUBR_b()
{
}

void x86_FSUBR_w()
{
}

void x86_FSUBR_d()
{
}

/****************
  FDIV
*****************/

void x86_FDIV_b()
{
}

void x86_FDIV_w()
{
}

void x86_FDIV_d()
{
}

/****************
  FDIVR
*****************/

void x86_FDIVR_b()
{
}

void x86_FDIVR_w()
{
}

void x86_FDIVR_d()
{
}

/****************
  FLD
*****************/

void x86_FLD_b()
{
}

void x86_FLD_w()
{
}

void x86_FLD_d()
{
}

/****************
  FST
*****************/

void x86_FST_b()
{
}

void x86_FST_w()
{
}

void x86_FST_d()
{
}

/****************
  FSTP
*****************/

void x86_FSTP_b()
{
}

void x86_FSTP_w()
{
}

void x86_FSTP_d()
{
}

/****************
  FLDENV
*****************/

void x86_FLDENV_b()
{
}

void x86_FLDENV_w()
{
}

void x86_FLDENV_d()
{
}

/****************
  FLDCW
*****************/

void x86_FLDCW_b()
{
}

void x86_FLDCW_w()
{
}

void x86_FLDCW_d()
{
}

/****************
  FSTENV
*****************/

void x86_FSTENV_b()
{
}

void x86_FSTENV_w()
{
}

void x86_FSTENV_d()
{
}

/****************
  FSTCW
*****************/

void x86_FSTCW_b()
{
}

void x86_FSTCW_w()
{
}

void x86_FSTCW_d()
{
}

/****************
  FXCH
*****************/

void x86_FXCH_b()
{
}

void x86_FXCH_w()
{
}

void x86_FXCH_d()
{
}

/****************
  FABS
*****************/

void x86_FABS_b()
{
}

void x86_FABS_w()
{
}

void x86_FABS_d()
{
}

/****************
  FTST
*****************/

void x86_FTST_b()
{
}

void x86_FTST_w()
{
}

void x86_FTST_d()
{
}

/****************
  FXAM
*****************/

void x86_FXAM_b()
{
}

void x86_FXAM_w()
{
}

void x86_FXAM_d()
{
}

/****************
  FLD1
*****************/

void x86_FLD1_b()
{
}

void x86_FLD1_w()
{
}

void x86_FLD1_d()
{
}

/****************
  FLDL2T
*****************/

void x86_FLDL2T_b()
{
}

void x86_FLDL2T_w()
{
}

void x86_FLDL2T_d()
{
}

/****************
  FLDL2E
*****************/

void x86_FLDL2E_b()
{
}

void x86_FLDL2E_w()
{
}

void x86_FLDL2E_d()
{
}

/****************
  FLDPI
*****************/

void x86_FLDPI_b()
{
}

void x86_FLDPI_w()
{
}

void x86_FLDPI_d()
{
}

/****************
  FLDLG2
*****************/

void x86_FLDLG2_b()
{
}

void x86_FLDLG2_w()
{
}

void x86_FLDLG2_d()
{
}

/****************
  FLDLN2
*****************/

void x86_FLDLN2_b()
{
}

void x86_FLDLN2_w()
{
}

void x86_FLDLN2_d()
{
}

/****************
  FLDZ
*****************/

void x86_FLDZ_b()
{
}

void x86_FLDZ_w()
{
}

void x86_FLDZ_d()
{
}

/****************
  FYL2X
*****************/

void x86_FYL2X_b()
{
}

void x86_FYL2X_w()
{
}

void x86_FYL2X_d()
{
}

/****************
  FPTAN
*****************/

void x86_FPTAN_b()
{
}

void x86_FPTAN_w()
{
}

void x86_FPTAN_d()
{
}

/****************
  FPATAN
*****************/

void x86_FPATAN_b()
{
}

void x86_FPATAN_w()
{
}

void x86_FPATAN_d()
{
}

/****************
  FXTRACT
*****************/

void x86_FXTRACT_b()
{
}

void x86_FXTRACT_w()
{
}

void x86_FXTRACT_d()
{
}

/****************
  FPREM1
*****************/

void x86_FPREM1_b()
{
}

void x86_FPREM1_w()
{
}

void x86_FPREM1_d()
{
}

/****************
  FDECSTP
*****************/

void x86_FDECSTP_b()
{
}

void x86_FDECSTP_w()
{
}

void x86_FDECSTP_d()
{
}

/****************
  FINCSTP
*****************/

void x86_FINCSTP_b()
{
}

void x86_FINCSTP_w()
{
}

void x86_FINCSTP_d()
{
}

/****************
  FPREM
*****************/

void x86_FPREM_b()
{
}

void x86_FPREM_w()
{
}

void x86_FPREM_d()
{
}

/****************
  FYL2XP1
*****************/

void x86_FYL2XP1_b()
{
}

void x86_FYL2XP1_w()
{
}

void x86_FYL2XP1_d()
{
}

/****************
  FSQRT
*****************/

void x86_FSQRT_b()
{
}

void x86_FSQRT_w()
{
}

void x86_FSQRT_d()
{
}

/****************
  FSINCOS
*****************/

void x86_FSINCOS_b()
{
}

void x86_FSINCOS_w()
{
}

void x86_FSINCOS_d()
{
}

/****************
  FRNDINT
*****************/

void x86_FRNDINT_b()
{
}

void x86_FRNDINT_w()
{
}

void x86_FRNDINT_d()
{
}

/****************
  FSCALE
*****************/

void x86_FSCALE_b()
{
}

void x86_FSCALE_w()
{
}

void x86_FSCALE_d()
{
}

/****************
  FSIN
*****************/

void x86_FSIN_b()
{
}

void x86_FSIN_w()
{
}

void x86_FSIN_d()
{
}

/****************
  FCOS
*****************/

void x86_FCOS_b()
{
}

void x86_FCOS_w()
{
}

void x86_FCOS_d()
{
}

/****************
  FCHS
*****************/

void x86_FCHS_b()
{
}

void x86_FCHS_w()
{
}

void x86_FCHS_d()
{
}

/****************
  FIADD
*****************/

void x86_FIADD_b()
{
}

void x86_FIADD_w()
{
}

void x86_FIADD_d()
{
}

/****************
  FIMUL
*****************/

void x86_FIMUL_b()
{
}

void x86_FIMUL_w()
{
}

void x86_FIMUL_d()
{
}

/****************
  FICOM
*****************/

void x86_FICOM_b()
{
}

void x86_FICOM_w()
{
}

void x86_FICOM_d()
{
}

/****************
  FICOMP
*****************/

void x86_FICOMP_b()
{
}

void x86_FICOMP_w()
{
}

void x86_FICOMP_d()
{
}

/****************
  FISUB
*****************/

void x86_FISUB_b()
{
}

void x86_FISUB_w()
{
}

void x86_FISUB_d()
{
}

/****************
  FISUBR
*****************/

void x86_FISUBR_b()
{
}

void x86_FISUBR_w()
{
}

void x86_FISUBR_d()
{
}

/****************
  FIDIV
*****************/

void x86_FIDIV_b()
{
}

void x86_FIDIV_w()
{
}

void x86_FIDIV_d()
{
}

/****************
  FIDIVR
*****************/

void x86_FIDIVR_b()
{
}

void x86_FIDIVR_w()
{
}

void x86_FIDIVR_d()
{
}

/****************
  FCMOVB
*****************/

void x86_FCMOVB_b()
{
}

void x86_FCMOVB_w()
{
}

void x86_FCMOVB_d()
{
}

/****************
  FCMOVE
*****************/

void x86_FCMOVE_b()
{
}

void x86_FCMOVE_w()
{
}

void x86_FCMOVE_d()
{
}

/****************
  FCMOVBE
*****************/

void x86_FCMOVBE_b()
{
}

void x86_FCMOVBE_w()
{
}

void x86_FCMOVBE_d()
{
}

/****************
  FCMOVU
*****************/

void x86_FCMOVU_b()
{
}

void x86_FCMOVU_w()
{
}

void x86_FCMOVU_d()
{
}

/****************
  FUCOMPP
*****************/

void x86_FUCOMPP_b()
{
}

void x86_FUCOMPP_w()
{
}

void x86_FUCOMPP_d()
{
}

/****************
  FCMOVNB
*****************/

void x86_FCMOVNB_b()
{
}

void x86_FCMOVNB_w()
{
}

void x86_FCMOVNB_d()
{
}

/****************
  FCMOVNE
*****************/

void x86_FCMOVNE_b()
{
}

void x86_FCMOVNE_w()
{
}

void x86_FCMOVNE_d()
{
}

/****************
  FCMOVNBE
*****************/

void x86_FCMOVNBE_b()
{
}

void x86_FCMOVNBE_w()
{
}

void x86_FCMOVNBE_d()
{
}

/****************
  FCMOVNU
*****************/

void x86_FCMOVNU_b()
{
}

void x86_FCMOVNU_w()
{
}

void x86_FCMOVNU_d()
{
}

/****************
  FUCOMI
*****************/

void x86_FUCOMI_b()
{
}

void x86_FUCOMI_w()
{
}

void x86_FUCOMI_d()
{
}

/****************
  FCOMI
*****************/

void x86_FCOMI_b()
{
}

void x86_FCOMI_w()
{
}

void x86_FCOMI_d()
{
}

/****************
  FILD
*****************/

void x86_FILD_b()
{
}

void x86_FILD_w()
{
}

void x86_FILD_d()
{
}

/****************
  FIST
*****************/

void x86_FIST_b()
{
}

void x86_FIST_w()
{
}

void x86_FIST_d()
{
}

/****************
  FISTP
*****************/

void x86_FISTP_b()
{
}

void x86_FISTP_w()
{
}

void x86_FISTP_d()
{
}

/****************
  FCLEX
*****************/

void x86_FCLEX_b()
{
}

void x86_FCLEX_w()
{
}

void x86_FCLEX_d()
{
}

/****************
  FINIT
*****************/

void x86_FINIT_b()
{
}

void x86_FINIT_w()
{
}

void x86_FINIT_d()
{
}

/****************
  FRSTOR
*****************/

void x86_FRSTOR_b()
{
}

void x86_FRSTOR_w()
{
}

void x86_FRSTOR_d()
{
}

/****************
  FSAVE
*****************/

void x86_FSAVE_b()
{
}

void x86_FSAVE_w()
{
}

void x86_FSAVE_d()
{
}

/****************
  FSTSW
*****************/

void x86_FSTSW_b()
{
}

void x86_FSTSW_w()
{
}

void x86_FSTSW_d()
{
}

/****************
  FFREE
*****************/

void x86_FFREE_b()
{
}

void x86_FFREE_w()
{
}

void x86_FFREE_d()
{
}

/****************
  FUCOM
*****************/

void x86_FUCOM_b()
{
}

void x86_FUCOM_w()
{
}

void x86_FUCOM_d()
{
}

/****************
  FUCOMP
*****************/

void x86_FUCOMP_b()
{
}

void x86_FUCOMP_w()
{
}

void x86_FUCOMP_d()
{
}

/****************
  FADDP
*****************/

void x86_FADDP_b()
{
}

void x86_FADDP_w()
{
}

void x86_FADDP_d()
{
}

/****************
  FMULP
*****************/

void x86_FMULP_b()
{
}

void x86_FMULP_w()
{
}

void x86_FMULP_d()
{
}

/****************
  FCOMPP
*****************/

void x86_FCOMPP_b()
{
}

void x86_FCOMPP_w()
{
}

void x86_FCOMPP_d()
{
}

/****************
  FSUBRP
*****************/

void x86_FSUBRP_b()
{
}

void x86_FSUBRP_w()
{
}

void x86_FSUBRP_d()
{
}

/****************
  FSUBP
*****************/

void x86_FSUBP_b()
{
}

void x86_FSUBP_w()
{
}

void x86_FSUBP_d()
{
}

/****************
  FDIVRP
*****************/

void x86_FDIVRP_b()
{
}

void x86_FDIVRP_w()
{
}

void x86_FDIVRP_d()
{
}

/****************
  FDIVP
*****************/

void x86_FDIVP_b()
{
}

void x86_FDIVP_w()
{
}

void x86_FDIVP_d()
{
}

/****************
  FBLD
*****************/

void x86_FBLD_b()
{
}

void x86_FBLD_w()
{
}

void x86_FBLD_d()
{
}

/****************
  FBSTP
*****************/

void x86_FBSTP_b()
{
}

void x86_FBSTP_w()
{
}

void x86_FBSTP_d()
{
}

/****************
  FUCOMIP
*****************/

void x86_FUCOMIP_b()
{
}

void x86_FUCOMIP_w()
{
}

void x86_FUCOMIP_d()
{
}

/****************
  FCOMIP
*****************/

void x86_FCOMIP_b()
{
}

void x86_FCOMIP_w()
{
}

void x86_FCOMIP_d()
{
}

/****************
  MOVZX8
*****************/

void x86_MOVZX8_b()
{
}

void x86_MOVZX8_w()
{
}

void x86_MOVZX8_d()
{
}

/****************
  MOVZX16
*****************/

void x86_MOVZX16_b()
{
}

void x86_MOVZX16_w()
{
}

void x86_MOVZX16_d()
{
}

/****************
  MOVSX8
*****************/

void x86_MOVSX8_b()
{
}

void x86_MOVSX8_w()
{
}

void x86_MOVSX8_d()
{
}

/****************
  MOVSX16
*****************/

void x86_MOVSX16_b()
{
}

void x86_MOVSX16_w()
{
}

void x86_MOVSX16_d()
{
}

/****************
  SETO
*****************/

void x86_SETO_b()
{
}

void x86_SETO_w()
{
}

void x86_SETO_d()
{
}

/****************
  SETNO
*****************/

void x86_SETNO_b()
{
}

void x86_SETNO_w()
{
}

void x86_SETNO_d()
{
}

/****************
  SETC
*****************/

void x86_SETC_b()
{
}

void x86_SETC_w()
{
}

void x86_SETC_d()
{
}

/****************
  SETNC
*****************/

void x86_SETNC_b()
{
}

void x86_SETNC_w()
{
}

void x86_SETNC_d()
{
}

/****************
  SETZ
*****************/

void x86_SETZ_b()
{
}

void x86_SETZ_w()
{
}

void x86_SETZ_d()
{
}

/****************
  SETNZ
*****************/

void x86_SETNZ_b()
{
}

void x86_SETNZ_w()
{
}

void x86_SETNZ_d()
{
}

/****************
  SETCZ
*****************/

void x86_SETCZ_b()
{
}

void x86_SETCZ_w()
{
}

void x86_SETCZ_d()
{
}

/****************
  SETNCZ
*****************/

void x86_SETNCZ_b()
{
}

void x86_SETNCZ_w()
{
}

void x86_SETNCZ_d()
{
}

/****************
  SETS
*****************/

void x86_SETS_b()
{
}

void x86_SETS_w()
{
}

void x86_SETS_d()
{
}

/****************
  SETNS
*****************/

void x86_SETNS_b()
{
}

void x86_SETNS_w()
{
}

void x86_SETNS_d()
{
}

/****************
  SETP
*****************/

void x86_SETP_b()
{
}

void x86_SETP_w()
{
}

void x86_SETP_d()
{
}

/****************
  SETNP
*****************/

void x86_SETNP_b()
{
}

void x86_SETNP_w()
{
}

void x86_SETNP_d()
{
}

/****************
  SETL
*****************/

void x86_SETL_b()
{
}

void x86_SETL_w()
{
}

void x86_SETL_d()
{
}

/****************
  SETNL
*****************/

void x86_SETNL_b()
{
}

void x86_SETNL_w()
{
}

void x86_SETNL_d()
{
}

/****************
  SETLE
*****************/

void x86_SETLE_b()
{
}

void x86_SETLE_w()
{
}

void x86_SETLE_d()
{
}

/****************
  SETNLE
*****************/

void x86_SETNLE_b()
{
}

void x86_SETNLE_w()
{
}

void x86_SETNLE_d()
{
}

/****************
  CMOVO
*****************/

void x86_CMOVO_b()
{
}

void x86_CMOVO_w()
{
}

void x86_CMOVO_d()
{
}

/****************
  CMOVNO
*****************/

void x86_CMOVNO_b()
{
}

void x86_CMOVNO_w()
{
}

void x86_CMOVNO_d()
{
}

/****************
  CMOVC
*****************/

void x86_CMOVC_b()
{
}

void x86_CMOVC_w()
{
}

void x86_CMOVC_d()
{
}

/****************
  CMOVNC
*****************/

void x86_CMOVNC_b()
{
}

void x86_CMOVNC_w()
{
}

void x86_CMOVNC_d()
{
}

/****************
  CMOVZ
*****************/

void x86_CMOVZ_b()
{
}

void x86_CMOVZ_w()
{
}

void x86_CMOVZ_d()
{
}

/****************
  CMOVNZ
*****************/

void x86_CMOVNZ_b()
{
}

void x86_CMOVNZ_w()
{
}

void x86_CMOVNZ_d()
{
}

/****************
  CMOVCZ
*****************/

void x86_CMOVCZ_b()
{
}

void x86_CMOVCZ_w()
{
}

void x86_CMOVCZ_d()
{
}

/****************
  CMOVNCZ
*****************/

void x86_CMOVNCZ_b()
{
}

void x86_CMOVNCZ_w()
{
}

void x86_CMOVNCZ_d()
{
}

/****************
  CMOVS
*****************/

void x86_CMOVS_b()
{
}

void x86_CMOVS_w()
{
}

void x86_CMOVS_d()
{
}

/****************
  CMOVNS
*****************/

void x86_CMOVNS_b()
{
}

void x86_CMOVNS_w()
{
}

void x86_CMOVNS_d()
{
}

/****************
  CMOVP
*****************/

void x86_CMOVP_b()
{
}

void x86_CMOVP_w()
{
}

void x86_CMOVP_d()
{
}

/****************
  CMOVNP
*****************/

void x86_CMOVNP_b()
{
}

void x86_CMOVNP_w()
{
}

void x86_CMOVNP_d()
{
}

/****************
  CMOVL
*****************/

void x86_CMOVL_b()
{
}

void x86_CMOVL_w()
{
}

void x86_CMOVL_d()
{
}

/****************
  CMOVNL
*****************/

void x86_CMOVNL_b()
{
}

void x86_CMOVNL_w()
{
}

void x86_CMOVNL_d()
{
}

/****************
  CMOVLE
*****************/

void x86_CMOVLE_b()
{
}

void x86_CMOVLE_w()
{
}

void x86_CMOVLE_d()
{
}

/****************
  CMOVNLE
*****************/

void x86_CMOVNLE_b()
{
}

void x86_CMOVNLE_w()
{
}

void x86_CMOVNLE_d()
{
}

/****************
  LSS
*****************/

void x86_LSS_b()
{
}

void x86_LSS_w()
{
}

void x86_LSS_d()
{
}

/****************
  LFS
*****************/

void x86_LFS_b()
{
}

void x86_LFS_w()
{
}

void x86_LFS_d()
{
}

/****************
  LGS
*****************/

void x86_LGS_b()
{
}

void x86_LGS_w()
{
}

void x86_LGS_d()
{
}

/****************
  CPUID
*****************/

void x86_CPUID_b()
{
}

void x86_CPUID_w()
{
}

void x86_CPUID_d()
{
}

/****************
  XADD
*****************/

void x86_XADD_b()
{
}

void x86_XADD_w()
{
}

void x86_XADD_d()
{
}

/****************
  MOVC
*****************/

void x86_MOVC_b()
{
}

void x86_MOVC_w()
{
}

void x86_MOVC_d()
{
}

/****************
  MOVD
*****************/

void x86_MOVD_b()
{
}

void x86_MOVD_w()
{
}

void x86_MOVD_d()
{
}

/****************
  MOVSG
*****************/

void x86_MOVSG_b()
{
}

void x86_MOVSG_w()
{
}

void x86_MOVSG_d()
{
}

ptr x86calltab[FCNT][3]=
{
 { 0,0,0},
 {       x86_MOV_b,       x86_MOV_w,       x86_MOV_d},     //1
 {      x86_STOS_b,      x86_STOS_w,      x86_STOS_d},     //2
 {      x86_MOVS_b,      x86_MOVS_w,      x86_MOVS_d},     //3
 {      x86_CMPS_b,      x86_CMPS_w,      x86_CMPS_d},     //4
 {      x86_XCHG_b,      x86_XCHG_w,      x86_XCHG_d},     //5
 {       x86_NOP,       x86_NOP,       x86_NOP},     //6
 {      x86_PUSH_b,      x86_PUSH_w,      x86_PUSH_d},     //7
 {       x86_POP_b,       x86_POP_w,       x86_POP_d},     //8
 {       x86_ADD_b,       x86_ADD_w,       x86_ADD_d},     //9
 {        x86_OR_b,        x86_OR_w,        x86_OR_d},     //10
 {       x86_ADC_b,       x86_ADC_w,       x86_ADC_d},     //11
 {       x86_SBB_b,       x86_SBB_w,       x86_SBB_d},     //12
 {       x86_AND_b,       x86_AND_w,       x86_AND_d},     //13
 {       x86_SUB_b,       x86_SUB_w,       x86_SUB_d},     //14
 {       x86_XOR_b,       x86_XOR_w,       x86_XOR_d},     //15
 {       x86_CMP_b,       x86_CMP_w,       x86_CMP_d},     //16
 {      x86_ADDX_b,      x86_ADDX_w,      x86_ADDX_d},     //17
 {       x86_ORX_b,       x86_ORX_w,       x86_ORX_d},     //18
 {      x86_ADCX_b,      x86_ADCX_w,      x86_ADCX_d},     //19
 {      x86_SBBX_b,      x86_SBBX_w,      x86_SBBX_d},     //20
 {      x86_ANDX_b,      x86_ANDX_w,      x86_ANDX_d},     //21
 {      x86_SUBX_b,      x86_SUBX_w,      x86_SUBX_d},     //22
 {      x86_XORX_b,      x86_XORX_w,      x86_XORX_d},     //23
 {      x86_CMPX_b,      x86_CMPX_w,      x86_CMPX_d},     //24
 {       x86_INC_b,       x86_INC_w,       x86_INC_d},     //25
 {       x86_DEC_b,       x86_DEC_w,       x86_DEC_d},     //26
 {     x86_CALLN_b,     x86_CALLN_w,     x86_CALLN_d},     //27
 {     x86_CALLF_b,     x86_CALLF_w,     x86_CALLF_d},     //28
 {      x86_JMPN_b,      x86_JMPN_w,      x86_JMPN_d},     //29
 {      x86_JMPF_b,      x86_JMPF_w,      x86_JMPF_d},     //30
 {       x86_DAA_b,       x86_DAA_w,       x86_DAA_d},     //31
 {       x86_DAS_b,       x86_DAS_w,       x86_DAS_d},     //32
 {       x86_AAA_b,       x86_AAA_w,       x86_AAA_d},     //33
 {       x86_AAS_b,       x86_AAS_w,       x86_AAS_d},     //34
 {     x86_PUSHA_b,     x86_PUSHA_w,     x86_PUSHA_d},     //35
 {      x86_POPA_b,      x86_POPA_w,      x86_POPA_d},     //36
 {       x86_LEA_b,       x86_LEA_w,       x86_LEA_d},     //37
 {       x86_CBW_b,       x86_CBW_w,       x86_CBW_d},     //38
 {       x86_CWD_b,       x86_CWD_w,       x86_CWD_d},     //39
 {      x86_TEST_b,      x86_TEST_w,      x86_TEST_d},     //40
 {       x86_NOT_b,       x86_NOT_w,       x86_NOT_d},     //41
 {       x86_NEG_b,       x86_NEG_w,       x86_NEG_d},     //42
 {       x86_MUL_b,       x86_MUL_w,       x86_MUL_d},     //43
 {      x86_IMUL_b,      x86_IMUL_w,      x86_IMUL_d},     //44
 {       x86_DIV_b,       x86_DIV_w,       x86_DIV_d},     //45
 {      x86_IDIV_b,      x86_IDIV_w,      x86_IDIV_d},     //46
 {       x86_HLT_b,       x86_HLT_w,       x86_HLT_d},     //47
 {       x86_ROR_b,       x86_ROR_w,       x86_ROR_d},     //48
 {       x86_ROL_b,       x86_ROL_w,       x86_ROL_d},     //49
 {       x86_RCL_b,       x86_RCL_w,       x86_RCL_d},     //50
 {       x86_RCR_b,       x86_RCR_w,       x86_RCR_d},     //51
 {       x86_SHL_b,       x86_SHL_w,       x86_SHL_d},     //52
 {       x86_SHR_b,       x86_SHR_w,       x86_SHR_d},     //53
 {       x86_SAR_b,       x86_SAR_w,       x86_SAR_d},     //54
 {      x86_RETN_b,      x86_RETN_w,      x86_RETN_d},     //55
 {      x86_RETF_b,      x86_RETF_w,      x86_RETF_d},     //56
 {       x86_INP_b,       x86_INP_w,       x86_INP_d},     //57
 {      x86_OUTP_b,      x86_OUTP_w,      x86_OUTP_d},     //58
 {      x86_SCAS_b,      x86_SCAS_w,      x86_SCAS_d},     //59
 {      x86_XLAT_b,      x86_XLAT_w,      x86_XLAT_d},     //60
 {       x86_AAM_b,       x86_AAM_w,       x86_AAM_d},     //61
 {       x86_AAD_b,       x86_AAD_w,       x86_AAD_d},     //62
 {      x86_CALL_b,      x86_CALL_w,      x86_CALL_d},     //63
 {       x86_JMP_b,       x86_JMP_w,       x86_JMP_d},     //64
 {      x86_LODS_b,      x86_LODS_w,      x86_LODS_d},     //65
 {      x86_INT3_b,      x86_INT3_w,      x86_INT3_d},     //66
 {      x86_INTN_b,      x86_INTN_w,      x86_INTN_d},     //67
 {      x86_INTO_b,      x86_INTO_w,      x86_INTO_d},     //68
 {      x86_IRET_b,      x86_IRET_w,      x86_IRET_d},     //69
 {     x86_ENTER_b,     x86_ENTER_w,     x86_ENTER_d},     //70
 {     x86_LEAVE_b,     x86_LEAVE_w,     x86_LEAVE_d},     //71
 {       x86_LES_b,       x86_LES_w,       x86_LES_d},     //72
 {       x86_LDS_b,       x86_LDS_w,       x86_LDS_d},     //73
 {       x86_INS_b,       x86_INS_w,       x86_INS_d},     //74
 {      x86_OUTS_b,      x86_OUTS_w,      x86_OUTS_d},     //75
 {    x86_LOOPNE_b,    x86_LOOPNE_w,    x86_LOOPNE_d},     //76
 {     x86_LOOPE_b,     x86_LOOPE_w,     x86_LOOPE_d},     //77
 {      x86_LOOP_b,      x86_LOOP_w,      x86_LOOP_d},     //78
 {      x86_JCXZ_b,      x86_JCXZ_w,      x86_JCXZ_d},     //79
 {       x86_CLC_b,       x86_CLC_w,       x86_CLC_d},     //80
 {       x86_STC_b,       x86_STC_w,       x86_STC_d},     //81
 {       x86_CLI_b,       x86_CLI_w,       x86_CLI_d},     //82
 {       x86_STI_b,       x86_STI_w,       x86_STI_d},     //83
 {       x86_CLD_b,       x86_CLD_w,       x86_CLD_d},     //84
 {       x86_STD_b,       x86_STD_w,       x86_STD_d},     //85
 {       x86_CMC_b,       x86_CMC_w,       x86_CMC_d},     //86
 {      x86_WAIT_b,      x86_WAIT_w,      x86_WAIT_d},     //87
 {     x86_PUSHF_b,     x86_PUSHF_w,     x86_PUSHF_d},     //88
 {      x86_POPF_b,      x86_POPF_w,      x86_POPF_d},     //89
 {      x86_SAHF_b,      x86_SAHF_w,      x86_SAHF_d},     //90
 {      x86_LAHF_b,      x86_LAHF_w,      x86_LAHF_d},     //91
 {     x86_BOUND_b,     x86_BOUND_w,     x86_BOUND_d},     //92
 {      x86_ARPL_b,      x86_ARPL_w,      x86_ARPL_d},     //93
 {        x86_JO_b,        x86_JO_w,        x86_JO_d},     //94
 {       x86_JNO_b,       x86_JNO_w,       x86_JNO_d},     //95
 {        x86_JC_b,        x86_JC_w,        x86_JC_d},     //96
 {       x86_JNC_b,       x86_JNC_w,       x86_JNC_d},     //97
 {        x86_JZ_b,        x86_JZ_w,        x86_JZ_d},     //98
 {       x86_JNZ_b,       x86_JNZ_w,       x86_JNZ_d},     //99
 {       x86_JCZ_b,       x86_JCZ_w,       x86_JCZ_d},     //100
 {      x86_JNCZ_b,      x86_JNCZ_w,      x86_JNCZ_d},     //101
 {        x86_JS_b,        x86_JS_w,        x86_JS_d},     //102
 {       x86_JNS_b,       x86_JNS_w,       x86_JNS_d},     //103
 {        x86_JP_b,        x86_JP_w,        x86_JP_d},     //104
 {       x86_JNP_b,       x86_JNP_w,       x86_JNP_d},     //105
 {        x86_JL_b,        x86_JL_w,        x86_JL_d},     //106
 {       x86_JNL_b,       x86_JNL_w,       x86_JNL_d},     //107
 {       x86_JLE_b,       x86_JLE_w,       x86_JLE_d},     //108
 {      x86_JNLE_b,      x86_JNLE_w,      x86_JNLE_d},     //109
 {      x86_SHLD_b,      x86_SHLD_w,      x86_SHLD_d},     //110
 {      x86_SHRD_b,      x86_SHRD_w,      x86_SHRD_d},     //111
 {      x86_SLDT_b,      x86_SLDT_w,      x86_SLDT_d},     //112
 {      x86_SGDT_b,      x86_SGDT_w,      x86_SGDT_d},     //113
 {      x86_LLDT_b,      x86_LLDT_w,      x86_LLDT_d},     //114
 {      x86_LGDT_b,      x86_LGDT_w,      x86_LGDT_d},     //115
 {       x86_LTR_b,       x86_LTR_w,       x86_LTR_d},     //116
 {       x86_STR_b,       x86_STR_w,       x86_STR_d},     //117
 {      x86_LIDT_b,      x86_LIDT_w,      x86_LIDT_d},     //118
 {      x86_SIDT_b,      x86_SIDT_w,      x86_SIDT_d},     //119
 {      x86_SMSW_b,      x86_SMSW_w,      x86_SMSW_d},     //120
 {      x86_LMSW_b,      x86_LMSW_w,      x86_LMSW_d},     //121
 {      x86_VERR_b,      x86_VERR_w,      x86_VERR_d},     //122
 {      x86_VERW_b,      x86_VERW_w,      x86_VERW_d},     //123
 {    x86_INVLPG_b,    x86_INVLPG_w,    x86_INVLPG_d},     //124
 {      x86_FADD_b,      x86_FADD_w,      x86_FADD_d},     //125
 {      x86_FMUL_b,      x86_FMUL_w,      x86_FMUL_d},     //126
 {      x86_FCOM_b,      x86_FCOM_w,      x86_FCOM_d},     //127
 {     x86_FCOMP_b,     x86_FCOMP_w,     x86_FCOMP_d},     //128
 {      x86_FSUB_b,      x86_FSUB_w,      x86_FSUB_d},     //129
 {     x86_FSUBR_b,     x86_FSUBR_w,     x86_FSUBR_d},     //130
 {      x86_FDIV_b,      x86_FDIV_w,      x86_FDIV_d},     //131
 {     x86_FDIVR_b,     x86_FDIVR_w,     x86_FDIVR_d},     //132
 {       x86_FLD_b,       x86_FLD_w,       x86_FLD_d},     //133
 {       x86_FST_b,       x86_FST_w,       x86_FST_d},     //134
 {      x86_FSTP_b,      x86_FSTP_w,      x86_FSTP_d},     //135
 {    x86_FLDENV_b,    x86_FLDENV_w,    x86_FLDENV_d},     //136
 {     x86_FLDCW_b,     x86_FLDCW_w,     x86_FLDCW_d},     //137
 {    x86_FSTENV_b,    x86_FSTENV_w,    x86_FSTENV_d},     //138
 {     x86_FSTCW_b,     x86_FSTCW_w,     x86_FSTCW_d},     //139
 {      x86_FXCH_b,      x86_FXCH_w,      x86_FXCH_d},     //140
 {      x86_FABS_b,      x86_FABS_w,      x86_FABS_d},     //141
 {      x86_FTST_b,      x86_FTST_w,      x86_FTST_d},     //142
 {      x86_FXAM_b,      x86_FXAM_w,      x86_FXAM_d},     //143
 {      x86_FLD1_b,      x86_FLD1_w,      x86_FLD1_d},     //144
 {    x86_FLDL2T_b,    x86_FLDL2T_w,    x86_FLDL2T_d},     //145
 {    x86_FLDL2E_b,    x86_FLDL2E_w,    x86_FLDL2E_d},     //146
 {     x86_FLDPI_b,     x86_FLDPI_w,     x86_FLDPI_d},     //147
 {    x86_FLDLG2_b,    x86_FLDLG2_w,    x86_FLDLG2_d},     //148
 {    x86_FLDLN2_b,    x86_FLDLN2_w,    x86_FLDLN2_d},     //149
 {      x86_FLDZ_b,      x86_FLDZ_w,      x86_FLDZ_d},     //150
 {     x86_FYL2X_b,     x86_FYL2X_w,     x86_FYL2X_d},     //151
 {     x86_FPTAN_b,     x86_FPTAN_w,     x86_FPTAN_d},     //152
 {    x86_FPATAN_b,    x86_FPATAN_w,    x86_FPATAN_d},     //153
 {   x86_FXTRACT_b,   x86_FXTRACT_w,   x86_FXTRACT_d},     //154
 {    x86_FPREM1_b,    x86_FPREM1_w,    x86_FPREM1_d},     //155
 {   x86_FDECSTP_b,   x86_FDECSTP_w,   x86_FDECSTP_d},     //156
 {   x86_FINCSTP_b,   x86_FINCSTP_w,   x86_FINCSTP_d},     //157
 {     x86_FPREM_b,     x86_FPREM_w,     x86_FPREM_d},     //158
 {   x86_FYL2XP1_b,   x86_FYL2XP1_w,   x86_FYL2XP1_d},     //159
 {     x86_FSQRT_b,     x86_FSQRT_w,     x86_FSQRT_d},     //160
 {   x86_FSINCOS_b,   x86_FSINCOS_w,   x86_FSINCOS_d},     //161
 {   x86_FRNDINT_b,   x86_FRNDINT_w,   x86_FRNDINT_d},     //162
 {    x86_FSCALE_b,    x86_FSCALE_w,    x86_FSCALE_d},     //163
 {      x86_FSIN_b,      x86_FSIN_w,      x86_FSIN_d},     //164
 {      x86_FCOS_b,      x86_FCOS_w,      x86_FCOS_d},     //165
 {      x86_FCHS_b,      x86_FCHS_w,      x86_FCHS_d},     //166
 {     x86_FIADD_b,     x86_FIADD_w,     x86_FIADD_d},     //167
 {     x86_FIMUL_b,     x86_FIMUL_w,     x86_FIMUL_d},     //168
 {     x86_FICOM_b,     x86_FICOM_w,     x86_FICOM_d},     //169
 {    x86_FICOMP_b,    x86_FICOMP_w,    x86_FICOMP_d},     //170
 {     x86_FISUB_b,     x86_FISUB_w,     x86_FISUB_d},     //171
 {    x86_FISUBR_b,    x86_FISUBR_w,    x86_FISUBR_d},     //172
 {     x86_FIDIV_b,     x86_FIDIV_w,     x86_FIDIV_d},     //173
 {    x86_FIDIVR_b,    x86_FIDIVR_w,    x86_FIDIVR_d},     //174
 {    x86_FCMOVB_b,    x86_FCMOVB_w,    x86_FCMOVB_d},     //175
 {    x86_FCMOVE_b,    x86_FCMOVE_w,    x86_FCMOVE_d},     //176
 {   x86_FCMOVBE_b,   x86_FCMOVBE_w,   x86_FCMOVBE_d},     //177
 {    x86_FCMOVU_b,    x86_FCMOVU_w,    x86_FCMOVU_d},     //178
 {   x86_FUCOMPP_b,   x86_FUCOMPP_w,   x86_FUCOMPP_d},     //179
 {   x86_FCMOVNB_b,   x86_FCMOVNB_w,   x86_FCMOVNB_d},     //180
 {   x86_FCMOVNE_b,   x86_FCMOVNE_w,   x86_FCMOVNE_d},     //181
 {  x86_FCMOVNBE_b,  x86_FCMOVNBE_w,  x86_FCMOVNBE_d},     //182
 {   x86_FCMOVNU_b,   x86_FCMOVNU_w,   x86_FCMOVNU_d},     //183
 {    x86_FUCOMI_b,    x86_FUCOMI_w,    x86_FUCOMI_d},     //184
 {     x86_FCOMI_b,     x86_FCOMI_w,     x86_FCOMI_d},     //185
 {      x86_FILD_b,      x86_FILD_w,      x86_FILD_d},     //186
 {      x86_FIST_b,      x86_FIST_w,      x86_FIST_d},     //187
 {     x86_FISTP_b,     x86_FISTP_w,     x86_FISTP_d},     //188
 {     x86_FCLEX_b,     x86_FCLEX_w,     x86_FCLEX_d},     //189
 {     x86_FINIT_b,     x86_FINIT_w,     x86_FINIT_d},     //190
 {    x86_FRSTOR_b,    x86_FRSTOR_w,    x86_FRSTOR_d},     //191
 {     x86_FSAVE_b,     x86_FSAVE_w,     x86_FSAVE_d},     //192
 {     x86_FSTSW_b,     x86_FSTSW_w,     x86_FSTSW_d},     //193
 {     x86_FFREE_b,     x86_FFREE_w,     x86_FFREE_d},     //194
 {     x86_FUCOM_b,     x86_FUCOM_w,     x86_FUCOM_d},     //195
 {    x86_FUCOMP_b,    x86_FUCOMP_w,    x86_FUCOMP_d},     //196
 {     x86_FADDP_b,     x86_FADDP_w,     x86_FADDP_d},     //197
 {     x86_FMULP_b,     x86_FMULP_w,     x86_FMULP_d},     //198
 {    x86_FCOMPP_b,    x86_FCOMPP_w,    x86_FCOMPP_d},     //199
 {    x86_FSUBRP_b,    x86_FSUBRP_w,    x86_FSUBRP_d},     //200
 {     x86_FSUBP_b,     x86_FSUBP_w,     x86_FSUBP_d},     //201
 {    x86_FDIVRP_b,    x86_FDIVRP_w,    x86_FDIVRP_d},     //202
 {     x86_FDIVP_b,     x86_FDIVP_w,     x86_FDIVP_d},     //203
 {      x86_FBLD_b,      x86_FBLD_w,      x86_FBLD_d},     //204
 {     x86_FBSTP_b,     x86_FBSTP_w,     x86_FBSTP_d},     //205
 {   x86_FUCOMIP_b,   x86_FUCOMIP_w,   x86_FUCOMIP_d},     //206
 {    x86_FCOMIP_b,    x86_FCOMIP_w,    x86_FCOMIP_d},     //207
 {    x86_MOVZX8_b,    x86_MOVZX8_w,    x86_MOVZX8_d},     //208
 {   x86_MOVZX16_b,   x86_MOVZX16_w,   x86_MOVZX16_d},     //209
 {    x86_MOVSX8_b,    x86_MOVSX8_w,    x86_MOVSX8_d},     //210
 {   x86_MOVSX16_b,   x86_MOVSX16_w,   x86_MOVSX16_d},     //211
 {      x86_SETO_b,      x86_SETO_w,      x86_SETO_d},     //212
 {     x86_SETNO_b,     x86_SETNO_w,     x86_SETNO_d},     //213
 {      x86_SETC_b,      x86_SETC_w,      x86_SETC_d},     //214
 {     x86_SETNC_b,     x86_SETNC_w,     x86_SETNC_d},     //215
 {      x86_SETZ_b,      x86_SETZ_w,      x86_SETZ_d},     //216
 {     x86_SETNZ_b,     x86_SETNZ_w,     x86_SETNZ_d},     //217
 {     x86_SETCZ_b,     x86_SETCZ_w,     x86_SETCZ_d},     //218
 {    x86_SETNCZ_b,    x86_SETNCZ_w,    x86_SETNCZ_d},     //219
 {      x86_SETS_b,      x86_SETS_w,      x86_SETS_d},     //220
 {     x86_SETNS_b,     x86_SETNS_w,     x86_SETNS_d},     //221
 {      x86_SETP_b,      x86_SETP_w,      x86_SETP_d},     //222
 {     x86_SETNP_b,     x86_SETNP_w,     x86_SETNP_d},     //223
 {      x86_SETL_b,      x86_SETL_w,      x86_SETL_d},     //224
 {     x86_SETNL_b,     x86_SETNL_w,     x86_SETNL_d},     //225
 {     x86_SETLE_b,     x86_SETLE_w,     x86_SETLE_d},     //226
 {    x86_SETNLE_b,    x86_SETNLE_w,    x86_SETNLE_d},     //227
 {     x86_CMOVO_b,     x86_CMOVO_w,     x86_CMOVO_d},     //228
 {    x86_CMOVNO_b,    x86_CMOVNO_w,    x86_CMOVNO_d},     //229
 {     x86_CMOVC_b,     x86_CMOVC_w,     x86_CMOVC_d},     //230
 {    x86_CMOVNC_b,    x86_CMOVNC_w,    x86_CMOVNC_d},     //231
 {     x86_CMOVZ_b,     x86_CMOVZ_w,     x86_CMOVZ_d},     //232
 {    x86_CMOVNZ_b,    x86_CMOVNZ_w,    x86_CMOVNZ_d},     //233
 {    x86_CMOVCZ_b,    x86_CMOVCZ_w,    x86_CMOVCZ_d},     //234
 {   x86_CMOVNCZ_b,   x86_CMOVNCZ_w,   x86_CMOVNCZ_d},     //235
 {     x86_CMOVS_b,     x86_CMOVS_w,     x86_CMOVS_d},     //236
 {    x86_CMOVNS_b,    x86_CMOVNS_w,    x86_CMOVNS_d},     //237
 {     x86_CMOVP_b,     x86_CMOVP_w,     x86_CMOVP_d},     //238
 {    x86_CMOVNP_b,    x86_CMOVNP_w,    x86_CMOVNP_d},     //239
 {     x86_CMOVL_b,     x86_CMOVL_w,     x86_CMOVL_d},     //240
 {    x86_CMOVNL_b,    x86_CMOVNL_w,    x86_CMOVNL_d},     //241
 {    x86_CMOVLE_b,    x86_CMOVLE_w,    x86_CMOVLE_d},     //242
 {   x86_CMOVNLE_b,   x86_CMOVNLE_w,   x86_CMOVNLE_d},     //243
 {       x86_LSS_b,       x86_LSS_w,       x86_LSS_d},     //244
 {       x86_LFS_b,       x86_LFS_w,       x86_LFS_d},     //245
 {       x86_LGS_b,       x86_LGS_w,       x86_LGS_d},     //246
 {     x86_CPUID_b,     x86_CPUID_w,     x86_CPUID_d},     //247
 {      x86_XADD_b,      x86_XADD_w,      x86_XADD_d},     //248
 {      x86_MOVC_b,      x86_MOVC_w,      x86_MOVC_d},     //249
 {      x86_MOVD_b,      x86_MOVD_w,      x86_MOVD_d},     //250
 {     x86_MOVSG_b,     x86_MOVSG_w,     x86_MOVSG_d},     //251
};
