#	$NetBSD: Makefile.inc,v 1.26 2019/10/13 07:28:10 mrg Exp $

# zone sources
.PATH:	${.CURDIR}/zone

SRCS+=	zone.c

MAN+=		zone_create.3 getzoneid.3

MLINKS+=	zone_create.3 getzoneidbyname.3 \
		zone_create.3 getzonenamebyid.3 \
		zone_create.3 zone_boot.3 \
		zone_create.3 zone_destroy.3 \
		zone_create.3 zone_getattr.3 \
		zone_create.3 zone_setattr.3 \
		zone_create.3 zone_enter.3 \
		zone_create.3 zone_list.3 \
		zone_create.3 zone_shutdown.3 \
		zone_create.3 zone_add_datalink.3 \
		zone_create.3 zone_remove_datalink.3 \
		zone_create.3 zone_check_datalink.3 \
		zone_create.3 zone_list_datalink.3
