# $NetBSD: Makefile,v 1.4 2020/06/30 20:32:11 riastradh Exp $

.include <bsd.own.mk>

TESTSDIR=	${TESTSBASE}/sys

TESTS_SUBDIRS+=	crypto
TESTS_SUBDIRS+=	net
TESTS_SUBDIRS+=	netatalk
TESTS_SUBDIRS+=	netinet
TESTS_SUBDIRS+=	netinet6
TESTS_SUBDIRS+=	rc
.if ${MACHINE} == amd64 || ${MACHINE} == i386
TESTS_SUBDIRS+=	x86
.endif

.include <bsd.test.mk>
