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
