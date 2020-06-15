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
 * Copyright (c) 2003, 2010, Oracle and/or its affiliates. All rights reserved.
 * Copyright 2015, Joyent Inc. All rights reserved.
 * Copyright (c) 2016 by Delphix. All rights reserved.
 * Copyright 2018 OmniOS Community Edition (OmniOSce) Association.
 */

/*
 * Zones
 *
 *   A zone is a named collection of processes, namespace constraints,
 *   and other system resources which comprise a secure and manageable
 *   application containment facility.
 *
 *   Zones (represented by the reference counted zone_t) are tracked in
 *   the kernel in the zonehash.  Elsewhere in the kernel, Zone IDs
 *   (zoneid_t) are used to track zone association.  Zone IDs are
 *   dynamically generated when the zone is created; if a persistent
 *   identifier is needed (core files, accounting logs, audit trail,
 *   etc.), the zone name should be used.
 *
 *
 *   Global Zone:
 *
 *   The global zone (zoneid 0) is automatically associated with all
 *   system resources that have not been bound to a user-created zone.
 *   This means that even systems where zones are not in active use
 *   have a global zone, and all processes, mounts, etc. are
 *   associated with that zone.  The global zone is generally
 *   unconstrained in terms of privileges and access, though the usual
 *   credential and privilege based restrictions apply.
 *
 *
 *   Zone States:
 *
 *   The states in which a zone may be in and the transitions are as
 *   follows:
 *
 *   ZONE_IS_UNINITIALIZED: primordial state for a zone. The partially
 *   initialized zone is added to the list of active zones on the system but
 *   isn't accessible.
 *
 *   ZONE_IS_INITIALIZED: Initialization complete except the ZSD callbacks are
 *   not yet completed. Not possible to enter the zone, but attributes can
 *   be retrieved.
 *
 *   ZONE_IS_READY: zsched (the kernel dummy process for a zone) is
 *   ready.  The zone is made visible after the ZSD constructor callbacks are
 *   executed.  A zone remains in this state until it transitions into
 *   the ZONE_IS_BOOTING state as a result of a call to zone_boot().
 *
 *   ZONE_IS_BOOTING: in this shortlived-state, zsched attempts to start
 *   init.  Should that fail, the zone proceeds to the ZONE_IS_SHUTTING_DOWN
 *   state.
 *
 *   ZONE_IS_RUNNING: The zone is open for business: zsched has
 *   successfully started init.   A zone remains in this state until
 *   zone_shutdown() is called.
 *
 *   ZONE_IS_SHUTTING_DOWN: zone_shutdown() has been called, the system is
 *   killing all processes running in the zone. The zone remains
 *   in this state until there are no more user processes running in the zone.
 *   zone_create(), zone_enter(), and zone_destroy() on this zone will fail.
 *   Since zone_shutdown() is restartable, it may be called successfully
 *   multiple times for the same zone_t.  Setting of the zone's state to
 *   ZONE_IS_SHUTTING_DOWN is synchronized with mounts, so VOP_MOUNT() may check
 *   the zone's status without worrying about it being a moving target.
 *
 *   ZONE_IS_EMPTY: zone_shutdown() has been called, and there
 *   are no more user processes in the zone.  The zone remains in this
 *   state until there are no more kernel threads associated with the
 *   zone.  zone_create(), zone_enter(), and zone_destroy() on this zone will
 *   fail.
 *
 *   ZONE_IS_DOWN: All kernel threads doing work on behalf of the zone
 *   have exited.  zone_shutdown() returns.  Henceforth it is not possible to
 *   join the zone or create kernel threads therein.
 *
 *   ZONE_IS_DYING: zone_destroy() has been called on the zone; zone
 *   remains in this state until zsched exits.  Calls to zone_find_by_*()
 *   return NULL from now on.
 *
 *   ZONE_IS_DEAD: zsched has exited (zone_ntasks == 0).  There are no
 *   processes or threads doing work on behalf of the zone.  The zone is
 *   removed from the list of active zones.  zone_destroy() returns, and
 *   the zone can be recreated.
 *
 *   ZONE_IS_FREE (internal state): zone_ref goes to 0, ZSD destructor
 *   callbacks are executed, and all memory associated with the zone is
 *   freed.
 *
 *   Threads can wait for the zone to enter a requested state by using
 *   zone_status_wait() or zone_status_timedwait() with the desired
 *   state passed in as an argument.  Zone state transitions are
 *   uni-directional; it is not possible to move back to an earlier state.
 *
 *
 *   Zone-Specific Data:
 *
 *   Subsystems needing to maintain zone-specific data can store that
 *   data using the ZSD mechanism.  This provides a zone-specific data
 *   store, similar to thread-specific data (see pthread_getspecific(3C)
 *   or the TSD code in uts/common/disp/thread.c.  Also, ZSD can be used
 *   to register callbacks to be invoked when a zone is created, shut
 *   down, or destroyed.  This can be used to initialize zone-specific
 *   data for new zones and to clean up when zones go away.
 *
 *
 *   Data Structures:
 *
 *   The per-zone structure (zone_t) is reference counted, and freed
 *   when all references are released.  zone_hold and zone_rele can be
 *   used to adjust the reference count.  In addition, reference counts
 *   associated with the cred_t structure are tracked separately using
 *   zone_cred_hold and zone_cred_rele.
 *
 *   Pointers to active zone_t's are stored in two hash tables; one
 *   for searching by id, the other for searching by name.  Lookups
 *   can be performed on either basis, using zone_find_by_id and
 *   zone_find_by_name.  Both return zone_t pointers with the zone
 *   held, so zone_rele should be called when the pointer is no longer
 *   needed.  Zones can also be searched by path; zone_find_by_path
 *   returns the zone with which a path name is associated (global
 *   zone if the path is not within some other zone's file system
 *   hierarchy).  This currently requires iterating through each zone,
 *   so it is slower than an id or name search via a hash table.
 *
 *
 *   Locking:
 *
 *   zonehash_lock: This is a top-level global lock used to protect the
 *       zone hash tables and lists.  Zones cannot be created or destroyed
 *       while this lock is held.
 *   zone_status_lock: This is a global lock protecting zone state.
 *       Zones cannot change state while this lock is held.  It also
 *       protects the list of kernel threads associated with a zone.
 *   zone_lock: This is a per-zone lock used to protect several fields of
 *       the zone_t (see <sys/zone.h> for details).  In addition, holding
 *       this lock means that the zone cannot go away.
 *   zone_nlwps_lock: This is a per-zone lock used to protect the fields
 *	 related to the zone.max-lwps rctl.
 *   zone_mem_lock: This is a per-zone lock used to protect the fields
 *	 related to the zone.max-locked-memory and zone.max-swap rctls.
 *   zone_rctl_lock: This is a per-zone lock used to protect other rctls,
 *       currently just max_lofi
 *   zsd_key_lock: This is a global lock protecting the key state for ZSD.
 *   zone_deathrow_lock: This is a global lock protecting the "deathrow"
 *       list (a list of zones in the ZONE_IS_DEAD state).
 *
 *   Ordering requirements:
 *       pool_lock --> cpu_lock --> zonehash_lock --> zone_status_lock -->
 *       zone_lock --> zsd_key_lock --> pidlock --> p_lock
 *
 *   When taking zone_mem_lock or zone_nlwps_lock, the lock ordering is:
 *	zonehash_lock --> a_lock --> pidlock --> p_lock --> zone_mem_lock
 *	zonehash_lock --> a_lock --> pidlock --> p_lock --> zone_nlwps_lock
 *
 *   Blocking memory allocations are permitted while holding any of the
 *   zone locks.
 *
 *
 *   System Call Interface:
 *
 *   The zone subsystem can be managed and queried from user level with
 *   the following system calls (all subcodes of the primary "zone"
 *   system call):
 *   - zone_create: creates a zone with selected attributes (name,
 *     root path, privileges, resource controls, ZFS datasets)
 *   - zone_enter: allows the current process to enter a zone
 *   - zone_getattr: reports attributes of a zone
 *   - zone_setattr: set attributes of a zone
 *   - zone_boot: set 'init' running for the zone
 *   - zone_list: lists all zones active in the system
 *   - zone_lookup: looks up zone id based on name
 *   - zone_shutdown: initiates shutdown process (see states above)
 *   - zone_destroy: completes shutdown process (see states above)
 *
 */

#include <sys/types.h>
#include <sys/zone.h>
#include <sys/syscallargs.h>

#define set_errno(x) (x)



static int
do_zone(int cmd, void *arg1, void *arg2, void *arg3, void *arg4)
{
	zone_def zs;
	int err;

	switch (cmd) {
	case ZONE_CREATE:
//		if (get_udatamodel() == DATAMODEL_NATIVE) {
			if (copyin(arg1, &zs, sizeof (zone_def))) {
				return (set_errno(EFAULT));
			}
//		} else {
//#ifdef _SYSCALL32_IMPL
//			zone_def32 zs32;
//
//			if (copyin(arg1, &zs32, sizeof (zone_def32))) {
//				return (set_errno(EFAULT));
//			}
//			zs.zone_name =
//			    (const char *)(unsigned long)zs32.zone_name;
//			zs.zone_root =
//			    (const char *)(unsigned long)zs32.zone_root;
//			zs.zone_privs =
//			    (const struct priv_set *)
//			    (unsigned long)zs32.zone_privs;
//			zs.zone_privssz = zs32.zone_privssz;
//			zs.rctlbuf = (caddr_t)(unsigned long)zs32.rctlbuf;
//			zs.rctlbufsz = zs32.rctlbufsz;
//			zs.zfsbuf = (caddr_t)(unsigned long)zs32.zfsbuf;
//			zs.zfsbufsz = zs32.zfsbufsz;
//			zs.extended_error =
//			    (int *)(unsigned long)zs32.extended_error;
//			zs.match = zs32.match;
//			zs.doi = zs32.doi;
//			zs.label = (const bslabel_t *)(uintptr_t)zs32.label;
//			zs.flags = zs32.flags;
//#else
//			panic("get_udatamodel() returned bogus result\n");
//#endif
//		}

		return (zone_create(zs.zone_name, zs.zone_root,
		    zs.zone_privs, zs.zone_privssz,
		    (caddr_t)zs.rctlbuf, zs.rctlbufsz,
		    (caddr_t)zs.zfsbuf, zs.zfsbufsz,
		    zs.extended_error, zs.match, zs.doi,
		    zs.label, zs.flags));
	case ZONE_BOOT:
		return (zone_boot((zoneid_t)(uintptr_t)arg1));
	case ZONE_DESTROY:
		return (zone_destroy((zoneid_t)(uintptr_t)arg1));
	case ZONE_GETATTR:
		return (zone_getattr((zoneid_t)(uintptr_t)arg1,
		    (int)(uintptr_t)arg2, arg3, (size_t)arg4));
	case ZONE_SETATTR:
		return (zone_setattr((zoneid_t)(uintptr_t)arg1,
		    (int)(uintptr_t)arg2, arg3, (size_t)arg4));
	case ZONE_ENTER:
		return (zone_enter((zoneid_t)(uintptr_t)arg1));
	case ZONE_LIST:
		return (zone_list((zoneid_t *)arg1, (uint_t *)arg2));
	case ZONE_SHUTDOWN:
		return (zone_shutdown((zoneid_t)(uintptr_t)arg1));
	case ZONE_LOOKUP:
		return (zone_lookup((const char *)arg1));
	case ZONE_VERSION:
		return (zone_version((int *)arg1));
	case ZONE_ADD_DATALINK:
		return (zone_add_datalink((zoneid_t)(uintptr_t)arg1,
		    (datalink_id_t)(uintptr_t)arg2));
	case ZONE_DEL_DATALINK:
		return (zone_remove_datalink((zoneid_t)(uintptr_t)arg1,
		    (datalink_id_t)(uintptr_t)arg2));
	case ZONE_CHECK_DATALINK: {
		zoneid_t	zoneid;
		boolean_t	need_copyout;

		if (copyin(arg1, &zoneid, sizeof (zoneid)) != 0)
			return (EFAULT);
		need_copyout = (zoneid == ALL_ZONES);
		err = zone_check_datalink(&zoneid,
		    (datalink_id_t)(uintptr_t)arg2);
		if (err == 0 && need_copyout) {
			if (copyout(&zoneid, arg1, sizeof (zoneid)) != 0)
				err = EFAULT;
		}
		return (err == 0 ? 0 : set_errno(err));
	}
	case ZONE_LIST_DATALINK:
		return (zone_list_datalink((zoneid_t)(uintptr_t)arg1,
		    (int *)arg2, (datalink_id_t *)(uintptr_t)arg3));
	default:
		return (set_errno(EINVAL));
	}

	return 0;
}

int
sys_zone(struct lwp *l, const struct sys_zone_args *uap, register_t *retval __unused)
{
	/* {
		syscallarg(int) cmd;
		syscallarg(void *) arg1;
		syscallarg(void *) arg2;
		syscallarg(void *) arg3;
		syscallarg(void *) arg4;
	} */

	return do_zone(l, SCARG(uap, cmd), SCARG(uap, arg1), SCARG(uap, arg2),
	    SCARG(uap, arg3), SCARG(uap, arg4));
}
