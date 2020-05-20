#include <sys/types.h>
#include <sys/zone.h>
#include <sys/syscallargs.h>

static int
do_zone(struct lwp *l, int cmd, void *arg1, void *arg2, void *arg3, void *arg4, register_t *retval)
{
	*retval = 0;
	return 0;
}

int      
sys_zone(struct lwp *l, const struct sys_zone_args *uap, register_t *retval)
{
	/* {
		syscallarg(int) cmd;
		syscallarg(void *) arg1;
		syscallarg(void *) arg2;
		syscallarg(void *) arg3;
		syscallarg(void *) arg4;
	} */

	return do_zone(l, SCARG(uap, cmd), SCARG(uap, arg1), SCARG(uap, arg2),
	    SCARG(uap, arg3), SCARG(uap, arg4), retval);
}
