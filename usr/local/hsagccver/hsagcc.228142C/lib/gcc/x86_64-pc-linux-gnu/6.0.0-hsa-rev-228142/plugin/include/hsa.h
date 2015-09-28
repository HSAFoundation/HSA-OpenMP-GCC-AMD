/* HSAIL and BRIG related macros and definitions.
   Copyright (C) 2013 Free Software Foundation, Inc.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3, or (at your option)
any later version.

GCC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

#ifndef HSA_H
#define HSA_H

#include "hsa-brig-format.h"
#include "is-a.h"
#include "predict.h"
#include "tree.h"
#include "vec.h"
#include "hash-table.h"
#include "basic-block.h"


/* Return true if the compiler should produce HSAIL.  */

static inline bool
hsa_gen_requested_p (void)
{
#ifndef ENABLE_HSA
  return false;
#endif
  return !flag_disable_hsa;
}

class hsa_op_immed;
class hsa_op_cst_list;
class hsa_insn_basic;
class hsa_op_address;
class hsa_op_reg;
class hsa_bb;
typedef hsa_insn_basic *hsa_insn_basic_p;

/* Class representing an input argument, output argument (result) or a
   variable, that will eventually end up being a symbol directive.  */

struct hsa_symbol
{
  /* Pointer to the original tree, which is PARM_DECL for input parameters and
     RESULT_DECL for the output parameters.  */
  tree decl;

  /* Name of the symbol, that will be written into output and dumps.  Can be
     NULL, see name_number below.*/
  const char *name;

  /* If name is NULL, artificial name will be formed from the segment name and
     this number.  */
  int name_number;

  /* Once written, this is the offset of the associated symbol directive.  Zero
     means the symbol has not been written yet.  */
  unsigned directive_offset;

  /* HSA type of the parameter.  */
  BrigType16_t type;

  /* The HSA segment this will eventually end up in.  */
  BrigSegment8_t segment;

  /* The HSA kind of linkage.  */
  BrigLinkage8_t linkage;

  /* Array dimension, if non-zero.  */
  unsigned HOST_WIDE_INT dim;

  /* Constant value, used for string constants.  */
  hsa_op_immed *cst_value;

  /* Is in global scope.  */
  bool global_scope_p;
};

/* Abstract class for HSA instruction operands. */

class hsa_op_base
{
public:

  /* Next operand scheduled to be written when writing BRIG operand
     section.  */
  hsa_op_base *next;

  /* Offset to which the associated operand structure will be written.  Zero if
     yet not scheduled for writing.  */
  unsigned brig_op_offset;

  /* The type of a particular operand.  */
  BrigKind16_t kind;

protected:
  hsa_op_base (BrigKind16_t k);
private:
  /* Make the default constructor inaccessible.  */
  hsa_op_base () {}
};

/* Common abstract ancestor for operands which have a type.  */

class hsa_op_with_type : public hsa_op_base
{
public:
  /* The type.  */
  BrigType16_t type;

protected:
  hsa_op_with_type (BrigKind16_t k, BrigType16_t t);
private:
  /* Make the default constructor inaccessible.  */
  hsa_op_with_type () : hsa_op_base (BRIG_KIND_NONE) {}
};

/* An immediate HSA operand.  */

class hsa_op_immed : public hsa_op_with_type
{
public:
  hsa_op_immed (tree tree_val, bool min32int = true);
  hsa_op_immed (HOST_WIDE_INT int_value, BrigKind16_t type);
  void *operator new (size_t);
  ~hsa_op_immed ();
  void set_type (BrigKind16_t t);

  /* Value as represented by middle end.  */
  tree tree_value;

  /* Integer value representation.  */
  HOST_WIDE_INT int_value;

  /* Brig data representation.  */
  char *brig_repr;

  /* Brig data representation size in bytes.  */
  unsigned brig_repr_size;

private:
  /* Make the default constructor inaccessible.  */
  hsa_op_immed () : hsa_op_with_type (BRIG_KIND_NONE, BRIG_TYPE_NONE) {}
  /* All objects are deallocated by destroying their pool, so make delete
     inaccessible too.  */
  void operator delete (void *) {}
  void emit_to_buffer (tree value);
};

/* Report whether or not P is a an immediate operand.  */

template <>
template <>
inline bool
is_a_helper <hsa_op_immed *>::test (hsa_op_base *p)
{
  return p->kind == BRIG_KIND_OPERAND_CONSTANT_BYTES;
}

/* HSA register operand.  */

class hsa_op_reg : public hsa_op_with_type
{
  friend class hsa_insn_basic;
  friend class hsa_insn_phi;
public:
  hsa_op_reg (BrigType16_t t);
  void *operator new (size_t);

  /* Verify register operand.  */
  void verify_ssa ();

  /* If NON-NULL, gimple SSA that we come from.  NULL if none.  */
  tree gimple_ssa;

  /* Defining instruction while still in the SSA.  */
  hsa_insn_basic *def_insn;

  /* If the register allocator decides to spill the register, this is the
     appropriate spill symbol.  */
  hsa_symbol *spill_sym;

  /* Number of this register structure in the order in which they were
     allocated.  */
  int order;
  int lr_begin, lr_end;

  /* Zero if the register is not yet allocated.  After, allocation, this must
     be 'c', 's', 'd' or 'q'.  */
  char reg_class;
  /* If allocated, the number of the HW register (within its HSA register
     class). */
  char hard_num;

private:
  /* Make the default constructor inaccessible.  */
  hsa_op_reg () : hsa_op_with_type (BRIG_KIND_NONE, BRIG_TYPE_NONE) {}
  /* All objects are deallocated by destroying their pool, so make delete
     inaccessible too.  */
  void operator delete (void *) {}
  /* Set difinition where the register is defined.  */
  void set_definition (hsa_insn_basic *insn);
  /* Uses of the value while still in SSA.  */
  auto_vec <hsa_insn_basic_p> uses;
};

typedef class hsa_op_reg *hsa_op_reg_p;

/* Report whether or not P is a register operand.  */

template <>
template <>
inline bool
is_a_helper <hsa_op_reg *>::test (hsa_op_base *p)
{
  return p->kind == BRIG_KIND_OPERAND_REGISTER;
}

/* An address HSA operand.  */

class hsa_op_address : public hsa_op_base
{
public:
  /* set up a new address operand consisting of base symbol SYM, register R and
     immediate OFFSET.  If the machine model is not large and offset is 64 bit,
     the upper, 32 bits have to be zero.  */
  hsa_op_address (hsa_symbol *sym, hsa_op_reg *reg,
		  HOST_WIDE_INT offset = 0);

  void *operator new (size_t);

  /* Set up a new address operand consisting of base symbol SYM and
     immediate OFFSET.  If the machine model is not large and offset is 64 bit,
     the upper, 32 bits have to be zero.  */
  hsa_op_address (hsa_symbol *sym, HOST_WIDE_INT offset = 0);

  /* Set up a new address operand consisting of register R and
     immediate OFFSET.  If the machine model is not large and offset is 64 bit,
     the upper, 32 bits have to be zero.  */
  hsa_op_address (hsa_op_reg *reg, HOST_WIDE_INT offset = 0);

  /* Symbol base of the address.  Can be NULL if there is none.  */
  hsa_symbol *symbol;

  /* Register offset.  Can be NULL if there is none.  */
  hsa_op_reg *reg;

  /* Immediate byte offset.  */
  HOST_WIDE_INT imm_offset;

private:
  /* Make the default constructor inaccessible.  */
  hsa_op_address () : hsa_op_base (BRIG_KIND_NONE) {}
  /* All objects are deallocated by destroying their pool, so make delete
     inaccessible too.  */
  void operator delete (void *) {}
};

/* Report whether or not P is an address operand.  */

template <>
template <>
inline bool
is_a_helper <hsa_op_address *>::test (hsa_op_base *p)
{
  return p->kind == BRIG_KIND_OPERAND_ADDRESS;
}

/* A reference to code HSA operand. It can be either reference
   to a start of a BB or a start of a function.  */

class hsa_op_code_ref : public hsa_op_base
{
public:
  hsa_op_code_ref ();

  /* Offset in the code section that this refers to.  */
  unsigned directive_offset;
};

/* Report whether or not P is a code reference operand.  */

template <>
template <>
inline bool
is_a_helper <hsa_op_code_ref *>::test (hsa_op_base *p)
{
  return p->kind == BRIG_KIND_OPERAND_CODE_REF;
}

/* Code list HSA operand.  */

class hsa_op_code_list: public hsa_op_base
{
public:
  hsa_op_code_list (unsigned elements);
  void *operator new (size_t);

  /* Offset to variable-sized array in hsa_data section, where
     are offsets to entries in the hsa_code section.  */
  auto_vec<unsigned> offsets;
private:
  /* Make the default constructor inaccessible.  */
  hsa_op_code_list () : hsa_op_base (BRIG_KIND_NONE) {}
  /* All objects are deallocated by destroying their pool, so make delete
     inaccessible too.  */
  void operator delete (void *) {}
};

/* Report whether or not P is a code list operand.  */

template <>
template <>
inline bool
is_a_helper <hsa_op_code_list *>::test (hsa_op_base *p)
{
  return p->kind == BRIG_KIND_OPERAND_CODE_LIST;
}

/* Opcodes of instructions that are not part of HSA but that we use to
   represent it nevertheless.  */

#define HSA_OPCODE_PHI (-1)
#define HSA_OPCODE_ARG_BLOCK (-2)

/* The number of operand pointers we can directly in an instruction.  */
#define HSA_BRIG_INT_STORAGE_OPERANDS 5

/* Class representing an HSA instruction.  Unlike typical ancestors for
   specialized classes, this one is also directly used for all instructions
   that are then represented as BrigInstBasic.  */

class hsa_insn_basic
{
public:
  hsa_insn_basic (unsigned nops, int opc);
  hsa_insn_basic (unsigned nops, int opc, BrigType16_t t,
		  hsa_op_base *arg0 = NULL,
		  hsa_op_base *arg1 = NULL,
		  hsa_op_base *arg2 = NULL,
		  hsa_op_base *arg3 = NULL);

  void *operator new (size_t);
  void set_op (int index, hsa_op_base *op);
  hsa_op_base *get_op (int index);
  hsa_op_base **get_op_addr (int index);
  unsigned int operand_count ();
  void verify ();
  unsigned input_count ();
  unsigned num_used_ops ();

  /* The previous and next instruction in the basic block.  */
  hsa_insn_basic *prev, *next;

  /* Basic block this instruction belongs to.  */
  basic_block bb;

  /* Operand code distinguishing different types of instructions.  Eventually
     these should only be BRIG_INST_* values from the BrigOpcode16_t range but
     initially we use negative values for PHI nodes and such.  */
  int opcode;

  int number;			/* FIXME: What is this for? */

  /* Type of the destination of the operations.  */
  BrigType16_t type;

private:
  /* Make the default constructor inaccessible.  */
  hsa_insn_basic () {}
  /* All objects are deallocated by destroying their pool, so make delete
     inaccessible too.  */
  void operator delete (void *) {}
  /* The individual operands.  All instructions but PHI nodes have five or
     fewer instructions and so will fit the internal storage.  */
  /* TODO: Vast majority of instructions have three or fewer operands, so we
     may actually try reducing it.  */
  auto_vec<hsa_op_base *, HSA_BRIG_INT_STORAGE_OPERANDS> operands;
};

/* Class representing a PHI node of the SSA form of HSA virtual
   registers.  */

class hsa_insn_phi : public hsa_insn_basic
{
public:
  hsa_insn_phi (unsigned nops, hsa_op_reg *dst);

  void *operator new (size_t);

  /* Destination.  */
  hsa_op_reg *dest;

private:
  /* Make the default constructor inaccessible.  */
  hsa_insn_phi () : hsa_insn_basic (1, HSA_OPCODE_PHI) {}
  /* All objects are deallocated by destroying their pool, so make delete
     inaccessible too.  */
  void operator delete (void *) {}
};

/* Report whether or not P is a PHI node.  */

template <>
template <>
inline bool
is_a_helper <hsa_insn_phi *>::test (hsa_insn_basic *p)
{
  return p->opcode == HSA_OPCODE_PHI;
}

/* HSA instruction for branches.  Currently we explicitely represent only
   conditional branches.  */

class hsa_insn_br : public hsa_insn_basic
{
public:
  hsa_insn_br (hsa_op_reg *ctrl);

  void *operator new (size_t);

  /* Width as described in HSA documentation.  */
  BrigWidth8_t width;
private:
  /* Make the default constructor inaccessible.  */
  hsa_insn_br () : hsa_insn_basic (1, BRIG_OPCODE_CBR) {}
  /* All objects are deallocated by destroying their pool, so make delete
     inaccessible too.  */
  void operator delete (void *) {}
};

/* Report whether P is a branching instruction.  */

template <>
template <>
inline bool
is_a_helper <hsa_insn_br *>::test (hsa_insn_basic *p)
{
  return p->opcode == BRIG_OPCODE_BR
    || p->opcode == BRIG_OPCODE_CBR;
}

/* HSA instruction for swtich branche.  */

class hsa_insn_sbr : public hsa_insn_basic
{
public:
  hsa_insn_sbr (hsa_op_reg *index, unsigned jump_count);

  void *operator new (size_t);

  void replace_all_labels (basic_block old_bb, basic_block new_bb);

  /* Width as described in HSA documentation.  */
  BrigWidth8_t width;

  /* Jump table.  */
  vec <basic_block> jump_table;

  /* Default label basic block.  */
  basic_block default_bb;

  /* Code list for label references.  */
  hsa_op_code_list *label_code_list;

private:
  /* Make the default constructor inaccessible.  */
  hsa_insn_sbr () : hsa_insn_basic (1, BRIG_OPCODE_SBR) {}
  /* All objects are deallocated by destroying their pool, so make delete
     inaccessible too.  */
  void operator delete (void *) {}
};

/* Report whether P is a switch branching instruction.  */

template <>
template <>
inline bool
is_a_helper <hsa_insn_sbr *>::test (hsa_insn_basic *p)
{
  return p->opcode == BRIG_OPCODE_SBR;
}

/* HSA instruction for comparisons.  */

class hsa_insn_cmp : public hsa_insn_basic
{
public:
  hsa_insn_cmp (BrigCompareOperation8_t cmp, BrigType16_t t,
		hsa_op_base *arg0 = NULL, hsa_op_base *arg1 = NULL,
		hsa_op_base *arg2 = NULL);

  void *operator new (size_t);

  /* Source type should be derived from operand types.  */

  /* The comparison operation.  */
  BrigCompareOperation8_t compare;

  /* TODO: Modifiers and packing control are missing but so are everywhere
     else.  */
private:
  /* Make the default constructor inaccessible.  */
  hsa_insn_cmp () : hsa_insn_basic (1, BRIG_OPCODE_CMP) {}
  /* All objects are deallocated by destroying their pool, so make delete
     inaccessible too.  */
  void operator delete (void *) {}
};

/* Report whether or not P is a comparison instruction.  */

template <>
template <>
inline bool
is_a_helper <hsa_insn_cmp *>::test (hsa_insn_basic *p)
{
  return p->opcode == BRIG_OPCODE_CMP;
}

/* HSA instruction for memory operations.  */

class hsa_insn_mem : public hsa_insn_basic
{
public:
  hsa_insn_mem (int opc, BrigType16_t t, hsa_op_base *arg0, hsa_op_base *arg1);

  void *operator new (size_t);

  /* The segment is of the memory access is either the segment of the symbol in
     the address operand or flat address is there is no symbol there.  */

  /* HSA equiv class, basically an alias set number. */
  uint8_t equiv_class;

  /* TODO:  Add width modifier, perhaps also other things.  */
protected:
  hsa_insn_mem (unsigned nops, int opc, BrigType16_t t,
		hsa_op_base *arg0 = NULL, hsa_op_base *arg1 = NULL,
		hsa_op_base *arg2 = NULL, hsa_op_base *arg3 = NULL);

private:
  /* Make the default constructor inaccessible.  */
  hsa_insn_mem () : hsa_insn_basic (1, BRIG_OPCODE_LD) {}
  /* All objects are deallocated by destroying their pool, so make delete
     inaccessible too.  */
  void operator delete (void *) {}
};

/* Report whether or not P is a memory instruction.  */

template <>
template <>
inline bool
is_a_helper <hsa_insn_mem *>::test (hsa_insn_basic *p)
{
  return (p->opcode == BRIG_OPCODE_LD
	  || p->opcode == BRIG_OPCODE_ST);
}

/* HSA instruction for atomic operations.  */

class hsa_insn_atomic : public hsa_insn_mem
{
public:
  hsa_insn_atomic (int nops, int opc, enum BrigAtomicOperation aop,
		   BrigType16_t t,
		   hsa_op_base *arg0 = NULL, hsa_op_base *arg1 = NULL,
		   hsa_op_base *arg2 = NULL, hsa_op_base *arg3 = NULL);
  void *operator new (size_t);

  /* The operation itself.  */
  enum BrigAtomicOperation atomicop;

  /* Things like acquire/release/aligned.  */
  enum BrigMemoryOrder memoryorder;

  /* Scope of the atomic operation. */
  enum BrigMemoryScope memoryscope;

private:
  /* Make the default constructor inaccessible.  */
  hsa_insn_atomic () : hsa_insn_mem (1, BRIG_KIND_NONE, BRIG_TYPE_NONE) {}
  /* All objects are deallocated by destroying their pool, so make delete
     inaccessible too.  */
  void operator delete (void *) {}
};

/* Report whether or not P is an atomic instruction.  */

template <>
template <>
inline bool
is_a_helper <hsa_insn_atomic *>::test (hsa_insn_basic *p)
{
  return (p->opcode == BRIG_OPCODE_ATOMIC
	  || p->opcode == BRIG_OPCODE_ATOMICNORET);
}

/* HSA instruction for signal operations.  */

class hsa_insn_signal : public hsa_insn_atomic
{
public:
  hsa_insn_signal (int nops, int opc, enum BrigAtomicOperation sop,
		   BrigType16_t t, hsa_op_base *arg0 = NULL,
		   hsa_op_base *arg1 = NULL,
		   hsa_op_base *arg2 = NULL, hsa_op_base *arg3 = NULL);

  void *operator new (size_t);

private:
  /* All objects are deallocated by destroying their pool, so make delete
     inaccessible too.  */
  void operator delete (void *) {}
};

/* Report whether or not P is a signal instruction.  */

template <>
template <>
inline bool
is_a_helper <hsa_insn_signal *>::test (hsa_insn_basic *p)
{
  return (p->opcode == BRIG_OPCODE_SIGNAL
	  || p->opcode == BRIG_OPCODE_SIGNALNORET);
}

/* HSA instruction to convert between flat addressing and segments.  */

class hsa_insn_seg : public hsa_insn_basic
{
public:
  hsa_insn_seg (int opc, BrigType16_t destt, BrigType16_t srct,
		BrigSegment8_t seg, hsa_op_base *arg0, hsa_op_base *arg1);

  void *operator new (size_t);

  /* Source type.  Depends on the source addressing/segment.  */
  BrigType16_t src_type;
  /* The segment we are converting from or to.  */
  BrigSegment8_t segment;
private:
  /* Make the default constructor inaccessible.  */
  hsa_insn_seg () : hsa_insn_basic (1, BRIG_OPCODE_STOF) {}
  /* All objects are deallocated by destroying their pool, so make delete
     inaccessible too.  */
  void operator delete (void *) {}
};

/* Report whether or not P is a segment conversion instruction.  */

template <>
template <>
inline bool
is_a_helper <hsa_insn_seg *>::test (hsa_insn_basic *p)
{
  return (p->opcode == BRIG_OPCODE_STOF
	  || p->opcode == BRIG_OPCODE_FTOS);
}

/* HSA instruction for function call.  */

class hsa_insn_call : public hsa_insn_basic
{
public:
  hsa_insn_call (tree callee);

  void *operator new (size_t);

  /* Called function */
  tree called_function;

  /* Input formal arguments.  */
  auto_vec <hsa_symbol *> input_args;

  /* Input arguments store instructions.  */
  auto_vec <hsa_insn_mem *> input_arg_insns;

  /* Output argument, can be NULL for void functions.  */
  hsa_symbol *output_arg;

  /* Called function code reference.  */
  hsa_op_code_ref func;

  /* Argument symbols.  */
  auto_vec <hsa_symbol *> args_symbols;

  /* Code list for arguments of the function.  */
  hsa_op_code_list *args_code_list;

  /* Result symbol.  */
  hsa_symbol *result_symbol;

  /* Code list for result of the function.  */
  hsa_op_code_list *result_code_list;
private:
  /* Make the default constructor inaccessible.  */
  hsa_insn_call () : hsa_insn_basic (0, BRIG_OPCODE_CALL) {}
  /* All objects are deallocated by destroying their pool, so make delete
     inaccessible too.  */
  void operator delete (void *) {}
};

/* Report whether or not P is a call instruction.  */

template <>
template <>
inline bool
is_a_helper <hsa_insn_call *>::test (hsa_insn_basic *p)
{
  return (p->opcode == BRIG_OPCODE_CALL);
}

/* HSA call instruction block encapsulates definition of arguments,
   result type, corresponding loads and a possible store.
   Moreover, it contains a single call instruction.
   Emission of the instruction will produce multiple
   HSAIL instructions.  */

class hsa_insn_arg_block : public hsa_insn_basic
{
public:
  hsa_insn_arg_block (BrigKind brig_kind, hsa_insn_call * call);

  void *operator new (size_t);

  /* Kind of argument block.  */
  BrigKind kind;

  /* Call instruction.  */
  hsa_insn_call *call_insn;
private:
  /* All objects are deallocated by destroying their pool, so make delete
     inaccessible too.  */
  void operator delete (void *) {}
};

/* Report whether or not P is a call block instruction.  */

template <>
template <>
inline bool
is_a_helper <hsa_insn_arg_block *>::test (hsa_insn_basic *p)
{
  return (p->opcode == HSA_OPCODE_ARG_BLOCK);
}

/* HSA comment instruction.  */

class hsa_insn_comment: public hsa_insn_basic
{
public:
  /* Constructor of class representing the comment in HSAIL.  */
  hsa_insn_comment (const char *s);

  void *operator new (size_t);

  /* Destructor.  */
  ~hsa_insn_comment ();

  /* Release memory for comment.  */
  void release_string ();

  char *comment;
};

/* Report whether or not P is a call block instruction.  */

template <>
template <>
inline bool
is_a_helper <hsa_insn_comment *>::test (hsa_insn_basic *p)
{
  return (p->opcode == BRIG_KIND_DIRECTIVE_COMMENT);
}

/* HSA queue instruction.  */

class hsa_insn_queue: public hsa_insn_basic
{
public:
  hsa_insn_queue (int nops, BrigOpcode opcode);

  /* Destructor.  */
  ~hsa_insn_queue ();
};

/* Report whether or not P is a call block instruction.  */

template <>
template <>
inline bool
is_a_helper <hsa_insn_queue *>::test (hsa_insn_basic *p)
{
  return (p->opcode == BRIG_OPCODE_ADDQUEUEWRITEINDEX);
}

/* Basic block of HSA instructions.  */

class hsa_bb
{
public:
  hsa_bb (basic_block cfg_bb);
  hsa_bb (basic_block cfg_bb, int idx);
  ~hsa_bb ();

  /* Append an instruction INSN into the basic block.  */
  void append_insn (hsa_insn_basic *insn);

  /* The real CFG BB that this HBB belongs to.  */
  basic_block bb;

  /* The operand that refers to the label to this BB.  */
  hsa_op_code_ref label_ref;

  /* The first and last instruction.  */
  hsa_insn_basic *first_insn, *last_insn;
  /* The first and last phi node.  */
  hsa_insn_phi *first_phi, *last_phi;

  /* Just a number to construct names from.  */
  int index;

  bitmap liveout, livein;
private:
  /* Make the default constructor inaccessible.  */
  hsa_bb ();
  /* All objects are deallocated by destroying their pool, so make delete
     inaccessible too.  */
  void operator delete (void *) {}
};

/* Return the corresponding HSA basic block structure for the given control
   flow basic_block BB.  */

static inline hsa_bb *
hsa_bb_for_bb (basic_block bb)
{
  return (struct hsa_bb *) bb->aux;
}

/* Class for hashing local hsa_symbols.  */

struct hsa_noop_symbol_hasher : nofree_ptr_hash <hsa_symbol>
{
  static inline hashval_t hash (const value_type);
  static inline bool equal (const value_type, const compare_type);
};

/* Hash hsa_symbol.  */

inline hashval_t
hsa_noop_symbol_hasher::hash (const value_type item)
{
  return DECL_UID (item->decl);
}

/* Return true if the DECL_UIDs of decls both symbols refer to  are equal.  */

inline bool
hsa_noop_symbol_hasher::equal (const value_type a, const compare_type b)
{
  return (DECL_UID (a->decl) == DECL_UID (b->decl));
}

/* Class for hashing global hsa_symbols.  */

struct hsa_free_symbol_hasher : free_ptr_hash <hsa_symbol>
{
  static inline hashval_t hash (const value_type);
  static inline bool equal (const value_type, const compare_type);
};

/* Hash hsa_symbol.  */

inline hashval_t
hsa_free_symbol_hasher::hash (const value_type item)
{
  return DECL_UID (item->decl);
}

/* Return true if the DECL_UIDs of decls both symbols refer to  are equal.  */

inline bool
hsa_free_symbol_hasher::equal (const value_type a, const compare_type b)
{
  return (DECL_UID (a->decl) == DECL_UID (b->decl));
}

/* Structure that encapsulates intermediate representation of a HSA
   function.  */

class hsa_function_representation
{
public:
  hsa_function_representation ();
  ~hsa_function_representation ();

  /* Builds a shadow register that is utilized to a kernel dispatch.  */
  hsa_op_reg *get_shadow_reg ();

  /* Name of the function.  */
  char *name;

  /* Input arguments of the function.  */
  /* FIXME: Normally we'd use a vector, however our C++ vectors seem to have
     problems with derived classes, so for now we'll use a simple array.  */
  unsigned input_args_count;

  /* Number of allocated register structures.  */
  int reg_count;

  struct hsa_symbol *input_args;
  /* Output argument or NULL if there is none.  */
  struct hsa_symbol *output_arg;
  /* Hash table of local variable symbols.  */
  hash_table <hsa_noop_symbol_hasher> *local_symbols;

  /* Hash map for string constants.  */
  hash_map <tree, hsa_symbol *> string_constants_map;

  /* Vector of pointers to spill symbols.  */
  vec <struct hsa_symbol *> spill_symbols;

  /* Vector of pointers to symbols (string constants and global,
     noni-addressable variables with a constructor).  */
  vec <struct hsa_symbol *> readonly_variables;

  /* Vector of called function declarations.  */
  vec <tree> called_functions;

  /* Number of HBB BBs.  */
  int hbb_count;

  /* Whether or not we could check and enforce SSA properties.  */
  bool in_ssa;

  /* True if the function is kernel function.  */
  bool kern_p;

  /* True if the function representation is a declaration.  */
  bool declaration_p;

  /* Function declaration tree.  */
  tree decl;

  /* Runtime shadow register.  */
  hsa_op_reg *shadow_reg;

  /* Number of kernel dispatched which take place in the function.  */
  unsigned kernel_dispatch_count;

  /* If the function representation contains a kernel dispatch,
     OMP data size is necessary memory that is used for copying before
     a kernel dispatch.  */
  unsigned maximum_omp_data_size;
};

enum hsa_function_kind
{
  HSA_NONE,
  HSA_KERNEL,
  HSA_FUNCTION
};

struct hsa_function_summary
{
  /* Default constructor.  */
  hsa_function_summary ();

  /* Kind of GPU/hostfunction.  */
  hsa_function_kind kind;

  /* Pointer to a cgraph node which is a HSA implementation of the function.
     In case of the function is a HSA function, the binded function points
     to the host function.  */
  cgraph_node *binded_function;

  /* Identifies if the function is an HSA function or a host function.  */
  bool gpu_implementation_p;
};

inline
hsa_function_summary::hsa_function_summary (): kind (HSA_NONE),
  binded_function (NULL), gpu_implementation_p (false)
{
}

/* Function summary for HSA functions.  */
class hsa_summary_t: public function_summary <hsa_function_summary *>
{
public:
  hsa_summary_t (symbol_table *table):
    function_summary<hsa_function_summary *> (table) { }

  void link_functions (cgraph_node *gpu, cgraph_node *host,
		       hsa_function_kind kind);
};

inline void
hsa_summary_t::link_functions (cgraph_node *gpu, cgraph_node *host,
			       hsa_function_kind kind)
{
  hsa_function_summary *gpu_summary = get (gpu);
  hsa_function_summary *host_summary = get (host);

  gpu_summary->kind = kind;
  host_summary->kind = kind;

  gpu_summary->gpu_implementation_p = true;
  host_summary->gpu_implementation_p = false;

  gpu_summary->binded_function = host;
  host_summary->binded_function = gpu;
}

/* in hsa.c */
extern struct hsa_function_representation *hsa_cfun;
extern hash_table <hsa_free_symbol_hasher> *hsa_global_variable_symbols;
extern hash_map <tree, vec <char *> *> *hsa_decl_kernel_dependencies;
extern hsa_summary_t *hsa_summaries;
extern hsa_symbol *hsa_num_threads;
extern unsigned hsa_kernel_calls_counter;
bool hsa_callable_function_p (tree fndecl);
void hsa_init_compilation_unit_data (void);
void hsa_deinit_compilation_unit_data (void);
bool hsa_machine_large_p (void);
bool hsa_full_profile_p (void);
bool hsa_opcode_op_output_p (int, int);
bool hsa_opcode_floating_bit_insn_p (BrigOpcode16_t);
unsigned hsa_type_bit_size (BrigType16_t t);
BrigType16_t hsa_bittype_for_type (BrigType16_t t);
bool hsa_type_float_p (BrigType16_t type);
bool hsa_type_integer_p (BrigType16_t type);
void hsa_destroy_insn (hsa_insn_basic *insn);
void hsa_add_kern_decl_mapping (tree decl, char *name, unsigned);
unsigned hsa_get_number_decl_kernel_mappings (void);
tree hsa_get_decl_kernel_mapping_decl (unsigned i);
char *hsa_get_decl_kernel_mapping_name (unsigned i);
unsigned hsa_get_decl_kernel_mapping_omp_size (unsigned i);
void hsa_free_decl_kernel_mapping (void);
void hsa_add_kernel_dependency (tree caller, char *called_function);
void hsa_sanitize_name (char *p);
char *hsa_brig_function_name (const char *p);
const char *hsa_get_declaration_name (tree decl);
void hsa_register_kernel (cgraph_node *host);
void hsa_register_kernel (cgraph_node *gpu, cgraph_node *host);

/* In hsa-gen.c.  */
void hsa_build_append_simple_mov (hsa_op_reg *, hsa_op_base *, hsa_bb *);
hsa_symbol *hsa_get_spill_symbol (BrigType16_t);
hsa_symbol *hsa_get_string_cst_symbol (BrigType16_t);
hsa_op_reg *hsa_spill_in (hsa_insn_basic *, hsa_op_reg *, hsa_op_reg **);
hsa_op_reg *hsa_spill_out (hsa_insn_basic *, hsa_op_reg *, hsa_op_reg **);
hsa_bb *hsa_init_new_bb (basic_block);
hsa_function_representation *hsa_generate_function_declaration (tree decl);
tree hsa_get_host_function (tree decl);

/* In hsa-regalloc.c.  */
void hsa_regalloc (void);

/* In hsa-brig.c.  */
void hsa_brig_emit_function (void);
void hsa_output_brig (void);
BrigType16_t bittype_for_type (BrigType16_t t);
unsigned hsa_get_imm_brig_type_len (BrigType16_t type);
void hsa_brig_emit_omp_symbols (void);

/*  In hsa-dump.c.  */
const char *hsa_seg_name (BrigSegment8_t);
void dump_hsa_bb (FILE *, hsa_bb *);
void dump_hsa_cfun (FILE *);
DEBUG_FUNCTION void debug_hsa_operand (hsa_op_base *opc);
DEBUG_FUNCTION void debug_hsa_insn (hsa_insn_basic *insn);

union hsa_bytes
{
  uint8_t b8;
  uint16_t b16;
  uint32_t b32;
  uint64_t b64;
};

/* Return true if a function DECL is an HSA implementation.  */

static inline bool
hsa_gpu_implementation_p (tree decl)
{
  if (hsa_summaries == NULL)
    return false;

  hsa_function_summary *s = hsa_summaries->get (cgraph_node::get_create (decl));

  return s->gpu_implementation_p;
}

#endif /* HSA_H */
