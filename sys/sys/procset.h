/*	$NetBSD$	*/

/*-
 * Copyright (c) 2020 The NetBSD Foundation, Inc.
 * All rights reserved.
 *
 * This code is derived from software contributed to The NetBSD Foundation
 * by Kamil Rytarowski.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE NETBSD FOUNDATION, INC. AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _SYS_PROCSET_H_
#define _SYS_PROCSET_H_

#include <sys/types.h>		/* for id_t */
#include <sys/idtype.h>		/* for idtype_t */

typedef enum idop {
	POP_AND,	/* set disjunction */
	POP_OR,		/* set conjunction */
	POP_XOR,	/* set exclusive or */
	POP_DIFF,	/* set difference */
	_POP_MAXIDTYPE = 0x7fffffff
} idop_t;

typedef struct procset {
	idop_t p_op;		/* the operator connecting the left
				 * and the right operand */
	idtype_t p_lidtype;	/* the left operand id type */
	id_t p_lid;		/* the left operand id */
	idtype_t p_ridtype;	/* the right operand id type */
	id_t p_rid;		/* the right operand id */
} procset_t;

static __inline void
_setprocset(procset_t *_procset, idop_t _op, idtype_t _ltype, id_t _lid,
    idtype_t _rtype, id_t _rid)
{
	_procset->p_op = _op;
	_procset->p_lidtype = _ltype;
	_procset->p_lid = _lid;
	_procset->p_ridtype = _rtype;
	_procset->p_rid = _rid;
}

#define setprocset(psp, op, ltype, lid, rtype, rid) \
    _setprocset((psp), (op), (ltype), (lid), (rtype), (rid))

#endif /* _SYS_PROCSET_H_ */
