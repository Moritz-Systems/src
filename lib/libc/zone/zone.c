#include <sys/zone.h>

zoneid_t
zone_create(void)
{
}

int
zone_boot(zoneid_t zoneid)
{
}

int
zone_shutdown(zoneid_t zoneid)
{
}

int
zone_destroy(zoneid_t zoneid)
{
}

ssize_t
zone_getattr(zoneid_t zoneid, int attr, void *valp,m, size_t size)
{
}

int
zone_setattr(zoneid_t zoneid, int attr, void *valp, size_t size)
{
}

int
zone_enter(zoneid_t zoneid)
{
}

int
zone_get_id(const char *str, zoneid_t *zip)
{
}

int
zone_list(zoneid_t *zonelist, uint_t *numzones)
{
}

static zoneid_t
zone_lookup(const char *name)
{
}

zoneid_t
getzoneid(void)
{
	return zone_lookup(NULL);
}

zoneid_t
getzonenamebyid(zoneid_t zoneid, char *buf, size_t buflen)
{
}

int
zone_add_datalink(zoneid_t zoneid, datalink_id_t linkid)
{
}

int
zone_remove_datalink(zoneid_t zoneid, datalink_id_t linkid)
{
}

int
zone_check_datalink(zoneid_t *zoneid, datalink_id_t linkid)
{
}

int
zone_list_datalink(zoneid_t zoneid, int *dlnump, datalink_id_t *linkids)
{
	return zone();
}
