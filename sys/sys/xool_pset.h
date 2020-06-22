/*
 * CDDL HEADER START
 *
 * The contents of this file are subject to the terms of the
 * Common Development and Distribution License, Version 1.0 only
 * (the "License").  You may not use this file except in compliance
 * with the License.
 *
 * You can obtain a copy of the license at usr/src/OPENSOLARIS.LICENSE
 * or http://www.opensolaris.org/os/licensing.
 * See the License for the specific language governing permissions
 * and limitations under the License.
 *
 * When distributing Covered Code, include this CDDL HEADER in each
 * file and include the License file at usr/src/OPENSOLARIS.LICENSE.
 * If applicable, add the following below this CDDL HEADER, with the
 * fields enclosed by brackets "[]" replaced with your own identifying
 * information: Portions Copyright [yyyy] [name of copyright owner]
 *
 * CDDL HEADER END
 */
/*
 * Copyright 2004 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#ifndef	_SYS_XOOL_PSET_H
#define	_SYS_XOOL_PSET_H

#pragma ident	"%Z%%M%	%I%	%E% SMI"

#include <sys/types.h>
//#include <sys/cpupart.h>
#include <sys/idtype.h>
#include <sys/nvpair.h>
//#include <sys/exacct.h>
#include <sys/processor.h>
#include <sys/time.h>
#include <sys/list.h>

#ifdef	__cplusplus
extern "C" {
#endif

#ifdef	_KERNEL

struct zone;

typedef struct xool_pset {
	psetid_t		pset_id;	/* pset ID */
	uint_t			pset_nxools;	/* # of xools we belong to */
	list_node_t		pset_link;	/* link to next/prev pset */
	nvlist_t		*pset_props;	/* pset properties */
} xool_pset_t;

extern xool_pset_t *xool_pset_default;		/* default pset */
extern hrtime_t xool_pset_mod;			/* pset modification time */
extern hrtime_t xool_cpu_mod;			/* cpu modification time */

extern void xool_pset_init(void);
extern int xool_pset_enable(void);
extern int xool_pset_disable(void);
extern int xool_pset_create(psetid_t *);
extern int xool_pset_destroy(psetid_t);
extern int xool_pset_assoc(xoolid_t, psetid_t);
extern void xool_pset_bind(proc_t *, psetid_t, void *, void *);
extern int xool_pset_xtransfer(id_t, id_t, size_t, id_t *);
extern int xool_pset_proprm(psetid_t, char *);
extern int xool_pset_propput(psetid_t, nvpair_t *);
extern int xool_pset_propget(psetid_t, char *, nvlist_t *);
extern int xool_cpu_proprm(processorid_t, char *);
extern int xool_cpu_propput(processorid_t, nvpair_t *);
extern int xool_cpu_propget(processorid_t, char *, nvlist_t *);
extern int xool_pset_pack(ea_object_t *);

extern int pset_bind_start(struct proc **, struct xool *);
extern void pset_bind_abort(struct proc **, struct xool *);
extern void pset_bind_finish(void);

extern boolean_t xool_pset_enabled(void);

extern void xool_pset_visibility_add(psetid_t, struct zone *);
extern void xool_pset_visibility_remove(psetid_t, struct zone *);

#endif	/* _KERNEL */

#ifdef	__cplusplus
}
#endif

#endif	/* _SYS_XOOL_PSET_H */
