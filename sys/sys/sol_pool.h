/*
 * CDDL HEADER START
 *
 * The contents of this file are subject to the terms of the
 * Common Development and Distribution License (the "License").
 * You may not use this file except in compliance with the License.
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
 * Copyright 2010 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#ifndef	_SYS_XOOL_H
#define	_SYS_XOOL_H

#include <sys/types.h>
#include <sys/time.h>
#include <sys/nvpair.h>
#include <sys/idtype.h>
#include <sys/list.h>

#ifdef	__cplusplus
extern "C" {
#endif

#define	XOOL_DEFAULT		0		/* default xool's ID */
#define	XOOL_MAXID		999999		/* maximum possible xool ID */
#define	XOOL_INVALID		-1

/* xools states */
#define	XOOL_DISABLED		0		/* xools enabled */
#define	XOOL_ENABLED		1		/* xools disabled */

#ifdef	_KERNEL

struct xool_pset;

typedef struct xool {
	xoolid_t		xool_id;	/* xool ID */
	uint32_t		xool_ref;	/* # of procs in this xool */
	list_node_t		xool_link;	/* links to next/prev xools */
	nvlist_t		*xool_props;	/* xool properties */
	struct xool_pset	*xool_pset;	/* xool's pset */
} xool_t;

/*
 * Flags for xool_do_bind
 */
#define	XOOL_BIND_PSET	0x00000001
#define	XOOL_BIND_ALL	XOOL_BIND_PSET

/*
 * Result codes for xool_get_class()
 */
#define	XOOL_CLASS_UNSET	-1		/* no scheduling class set */
#define	XOOL_CLASS_INVAL	-2		/* class is invalid */

extern int	xool_count;	/* current number of xools */
extern xool_t	*xool_default;	/* default xool pointer */
extern int	xool_state;	/* xools state -- enabled/disabled */
extern void	*xool_buf;	/* last state snapshot */
extern size_t	xool_bufsz;	/* size of xool_buf */

/*
 * Lookup routines
 */
extern xool_t	*xool_lookup_xool_by_id(xoolid_t);
extern xool_t	*xool_lookup_xool_by_name(char *);
extern xool_t	*xool_lookup_xool_by_pset(int);

/*
 * Configuration routines
 */
extern void 	xool_init(void);
extern int	xool_status(int);
extern int	xool_create(int, int, id_t *);
extern int	xool_destroy(int, int, id_t);
extern int	xool_transfer(int, id_t, id_t, uint64_t);
extern int	xool_assoc(xoolid_t, int, id_t);
extern int	xool_dissoc(xoolid_t, int);
extern int	xool_bind(xoolid_t, idtype_t, id_t);
extern id_t	xool_get_class(xool_t *);
extern int	xool_do_bind(xool_t *, idtype_t, id_t, int);
extern int	xool_query_binding(idtype_t, id_t, id_t *);
extern int	xool_xtransfer(int, id_t, id_t, uint_t, id_t *);
extern int	xool_pack_conf(void *, size_t, size_t *);
extern int	xool_propput(int, int, id_t, nvpair_t *);
extern int	xool_proprm(int, int, id_t, char *);
extern int	xool_propget(char *, int, int, id_t, nvlist_t **);
extern int	xool_commit(int);
extern void	xool_get_name(xool_t *, char **);

/*
 * Synchronization routines
 */
extern void	xool_lock(void);
extern int	xool_lock_intr(void);
extern int	xool_lock_held(void);
extern void	xool_unlock(void);
extern void	xool_barrier_enter(void);
extern void	xool_barrier_exit(void);

typedef enum {
	XOOL_E_ENABLE,
	XOOL_E_DISABLE,
	XOOL_E_CHANGE,
} xool_event_t;

typedef void xool_event_cb_func_t(xool_event_t, xoolid_t,  void *);

typedef struct xool_event_cb {
	xool_event_cb_func_t	*pec_func;
	void			*pec_arg;
	list_node_t		pec_list;
} xool_event_cb_t;

/*
 * Routines used to register interest in changes in cpu xools.
 */
extern void xool_event_cb_register(xool_event_cb_t *);
extern void xool_event_cb_unregister(xool_event_cb_t *);
#endif	/* _KERNEL */

#ifdef	__cplusplus
}
#endif

#endif	/* _SYS_XOOL_H */
