#ifndef _SYS_ZONE_H_
#define _SYS_ZONE_H_

#include <sys/cdefs.h>

/* system call subcodes */
#define ZONE_CREATE             0
#define ZONE_DESTROY            1
#define ZONE_GETATTR            2
#define ZONE_ENTER              3
#define ZONE_LIST               4
#define ZONE_SHUTDOWN           5
#define ZONE_LOOKUP             6
#define ZONE_BOOT               7
#define ZONE_VERSION            8
#define ZONE_SETATTR            9
#define ZONE_ADD_DATALINK       10
#define ZONE_DEL_DATALINK       11
#define ZONE_CHECK_DATALINK     12
#define ZONE_LIST_DATALINK      13

/* zone attributes */
#define ZONE_ATTR_ROOT          1
#define ZONE_ATTR_NAME          2
#define ZONE_ATTR_STATUS        3
#define ZONE_ATTR_PRIVSET       4
#define ZONE_ATTR_UNIQID        5
#define ZONE_ATTR_POOLID        6
#define ZONE_ATTR_INITPID       7
#define ZONE_ATTR_SLBL          8
#define ZONE_ATTR_INITNAME      9
#define ZONE_ATTR_BOOTARGS      10
#define ZONE_ATTR_BRAND         11
#define ZONE_ATTR_PHYS_MCAP     12
#define ZONE_ATTR_SCHED_CLASS   13
#define ZONE_ATTR_FLAGS         14
#define ZONE_ATTR_HOSTID        15
#define ZONE_ATTR_FS_ALLOWED    16
#define ZONE_ATTR_NETWORK       17
#define ZONE_ATTR_INITNORESTART 20
#define ZONE_ATTR_SECFLAGS      21

__BEGIN_DECLS

int zone(int, void *, void *, void *, void *);

__END_DECLS

#endif /* _SYS_ZONE_H_ */
