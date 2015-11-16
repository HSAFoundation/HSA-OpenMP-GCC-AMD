/* Single entry single exit control flow regions.
   Copyright (C) 2008-2015 Free Software Foundation, Inc.
   Contributed by Jan Sjodin <jan.sjodin@amd.com> and
   Sebastian Pop <sebastian.pop@amd.com>.

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

#ifndef GCC_SESE_H
#define GCC_SESE_H

typedef hash_map<tree, tree> parameter_rename_map_t;

/* A Single Entry, Single Exit region is a part of the CFG delimited
   by two edges.  */
struct sese_l
{
  sese_l (edge e, edge x) : entry (e), exit (x) {}

  operator bool () const { return entry && exit; }

  edge entry;
  edge exit;
};

/* Get the entry of an sese S.  */

static inline basic_block
get_entry_bb (sese_l &s)
{
  return s.entry->dest;
}

/* Get the exit of an sese S.  */

static inline basic_block
get_exit_bb (sese_l &s)
{
  return s.exit->src;
}

/* A helper structure for bookkeeping information about a scop in graphite.  */
typedef struct sese_info_t
{
  /* The SESE region.  */
  sese_l region;

  /* Parameters used within the SCOP.  */
  vec<tree> params;

  /* Parameters to be renamed.  */
  parameter_rename_map_t *parameter_rename_map;

  /* Loops completely contained in this SESE.  */
  bitmap loops;
  vec<loop_p> loop_nest;

  /* Basic blocks contained in this SESE.  */
  vec<basic_block> bbs;
} *sese_info_p;

#define SESE_PARAMS(S) (S->params)
#define SESE_LOOPS(S) (S->loops)
#define SESE_LOOP_NEST(S) (S->loop_nest)

extern sese_info_p new_sese_info (edge, edge);
extern void free_sese_info (sese_info_p);
extern void sese_insert_phis_for_liveouts (sese_info_p, basic_block, edge, edge);
extern void build_sese_loop_nests (sese_info_p);
extern edge copy_bb_and_scalar_dependences (basic_block, sese_info_p, edge,
					    vec<tree> , bool *);
extern struct loop *outermost_loop_in_sese (sese_l &, basic_block);
extern tree scalar_evolution_in_region (sese_l &, loop_p, tree);
extern bool invariant_in_sese_p_rec (tree, sese_l &, bool *);

/* Check that SESE contains LOOP.  */

static inline bool
sese_contains_loop (sese_info_p sese, struct loop *loop)
{
  return bitmap_bit_p (SESE_LOOPS (sese), loop->num);
}

/* The number of parameters in REGION. */

static inline unsigned
sese_nb_params (sese_info_p region)
{
  return SESE_PARAMS (region).length ();
}

/* Checks whether BB is contained in the region delimited by ENTRY and
   EXIT blocks.  */

static inline bool
bb_in_region (basic_block bb, basic_block entry, basic_block exit)
{
#ifdef ENABLE_CHECKING
  {
    edge e;
    edge_iterator ei;

    /* Check that there are no edges coming in the region: all the
       predecessors of EXIT are dominated by ENTRY.  */
    FOR_EACH_EDGE (e, ei, exit->preds)
      dominated_by_p (CDI_DOMINATORS, e->src, entry);
  }
#endif

  return dominated_by_p (CDI_DOMINATORS, bb, entry)
	 && !(dominated_by_p (CDI_DOMINATORS, bb, exit)
	      && !dominated_by_p (CDI_DOMINATORS, entry, exit));
}

/* Checks whether BB is contained in the region delimited by ENTRY and
   EXIT blocks.  */

static inline bool
bb_in_sese_p (basic_block bb, sese_l &r)
{
  return bb_in_region (bb, r.entry->dest, r.exit->dest);
}

/* Returns true when STMT is defined in REGION.  */

static inline bool
stmt_in_sese_p (gimple *stmt, sese_l &r)
{
  basic_block bb = gimple_bb (stmt);
  return bb && bb_in_sese_p (bb, r);
}

/* Returns true when NAME is defined in REGION.  */

static inline bool
defined_in_sese_p (tree name, sese_l &r)
{
  return stmt_in_sese_p (SSA_NAME_DEF_STMT (name), r);
}

/* Returns true when LOOP is in REGION.  */

static inline bool
loop_in_sese_p (struct loop *loop, sese_l &region)
{
  return (bb_in_sese_p (loop->header, region)
	  && bb_in_sese_p (loop->latch, region));
}

/* Returns the loop depth of LOOP in REGION.  The loop depth
   is the same as the normal loop depth, but limited by a region.

   Example:

   loop_0
     loop_1
       {
         S0
            <- region start
         S1

         loop_2
           S2

         S3
            <- region end
       }

    loop_0 does not exist in the region -> invalid
    loop_1 exists, but is not completely contained in the region -> depth 0
    loop_2 is completely contained -> depth 1  */

static inline unsigned int
sese_loop_depth (sese_l &region, loop_p loop)
{
  unsigned int depth = 0;

  while (loop_in_sese_p (loop, region))
    {
      depth++;
      loop = loop_outer (loop);
    }

  return depth;
}

/* A single entry single exit specialized for conditions.  */

typedef struct ifsese_s {
  sese_info_p region;
  sese_info_p true_region;
  sese_info_p false_region;
} *ifsese;

extern void if_region_set_false_region (ifsese, sese_info_p);
extern ifsese move_sese_in_condition (sese_info_p);
extern edge get_true_edge_from_guard_bb (basic_block);
extern edge get_false_edge_from_guard_bb (basic_block);
extern void set_ifsese_condition (ifsese, tree);

static inline edge
if_region_entry (ifsese if_region)
{
  return if_region->region->region.entry;
}

static inline edge
if_region_exit (ifsese if_region)
{
  return if_region->region->region.exit;
}

static inline basic_block
if_region_get_condition_block (ifsese if_region)
{
  return if_region_entry (if_region)->dest;
}

/* Free and compute again all the dominators information.  */

static inline void
recompute_all_dominators (void)
{
  mark_irreducible_loops ();
  free_dominance_info (CDI_DOMINATORS);
  calculate_dominance_info (CDI_DOMINATORS);

  free_dominance_info (CDI_POST_DOMINATORS);
  calculate_dominance_info (CDI_POST_DOMINATORS);
}

typedef struct gimple_poly_bb
{
  basic_block bb;
  struct poly_bb *pbb;

  /* Lists containing the restrictions of the conditional statements
     dominating this bb.  This bb can only be executed, if all conditions
     are true.

     Example:

     for (i = 0; i <= 20; i++)
     {
       A

       if (2i <= 8)
         B
     }

     So for B there is an additional condition (2i <= 8).

     List of COND_EXPR and SWITCH_EXPR.  A COND_EXPR is true only if the
     corresponding element in CONDITION_CASES is not NULL_TREE.  For a
     SWITCH_EXPR the corresponding element in CONDITION_CASES is a
     CASE_LABEL_EXPR.  */
  vec<gimple *> conditions;
  vec<gimple *> condition_cases;
  vec<data_reference_p> data_refs;
} *gimple_poly_bb_p;

#define GBB_BB(GBB) (GBB)->bb
#define GBB_PBB(GBB) (GBB)->pbb
#define GBB_DATA_REFS(GBB) (GBB)->data_refs
#define GBB_CONDITIONS(GBB) (GBB)->conditions
#define GBB_CONDITION_CASES(GBB) (GBB)->condition_cases

/* Return the innermost loop that contains the basic block GBB.  */

static inline struct loop *
gbb_loop (gimple_poly_bb_p gbb)
{
  return GBB_BB (gbb)->loop_father;
}

/* Returns the gimple loop, that corresponds to the loop_iterator_INDEX.
   If there is no corresponding gimple loop, we return NULL.  */

static inline loop_p
gbb_loop_at_index (gimple_poly_bb_p gbb, sese_l &region, int index)
{
  loop_p loop = gbb_loop (gbb);
  int depth = sese_loop_depth (region, loop);

  while (--depth > index)
    loop = loop_outer (loop);

  gcc_assert (loop_in_sese_p (loop, region));

  return loop;
}

/* The number of common loops in REGION for GBB1 and GBB2.  */

static inline int
nb_common_loops (sese_l &region, gimple_poly_bb_p gbb1, gimple_poly_bb_p gbb2)
{
  loop_p l1 = gbb_loop (gbb1);
  loop_p l2 = gbb_loop (gbb2);
  loop_p common = find_common_loop (l1, l2);

  return sese_loop_depth (region, common);
}

/* Return true when DEF can be analyzed in REGION by the scalar
   evolution analyzer.  */

static inline bool
scev_analyzable_p (tree def, sese_l &region)
{
  loop_p loop;
  tree scev;
  tree type = TREE_TYPE (def);

  /* When Graphite generates code for a scev, the code generator
     expresses the scev in function of a single induction variable.
     This is unsafe for floating point computations, as it may replace
     a floating point sum reduction with a multiplication.  The
     following test returns false for non integer types to avoid such
     problems.  */
  if (!INTEGRAL_TYPE_P (type)
      && !POINTER_TYPE_P (type))
    return false;

  loop = loop_containing_stmt (SSA_NAME_DEF_STMT (def));
  scev = scalar_evolution_in_region (region, loop, def);

  return !chrec_contains_undetermined (scev)
    && (TREE_CODE (scev) != SSA_NAME
	|| !defined_in_sese_p (scev, region))
    && (tree_does_not_contain_chrecs (scev)
	|| evolution_function_is_affine_p (scev));
}

#endif
