/*	$NetBSD: der-private.h,v 1.2 2019/12/15 22:50:47 christos Exp $	*/

/* This is a generated file */
#ifndef __der_private_h__
#define __der_private_h__

#include <stdarg.h>

int
_asn1_bmember_isset_bit (
	const void */*data*/,
	unsigned int /*bit*/,
	size_t /*size*/);

void
_asn1_bmember_put_bit (
	unsigned char */*p*/,
	const void */*data*/,
	unsigned int /*bit*/,
	size_t /*size*/,
	unsigned int */*bitset*/);

int
_asn1_copy (
	const struct asn1_template */*t*/,
	const void */*from*/,
	void */*to*/);

int
_asn1_copy_top (
	const struct asn1_template */*t*/,
	const void */*from*/,
	void */*to*/);

int
_asn1_decode (
	const struct asn1_template */*t*/,
	unsigned /*flags*/,
	const unsigned char */*p*/,
	size_t /*len*/,
	void */*data*/,
	size_t */*size*/);

int
_asn1_decode_top (
	const struct asn1_template */*t*/,
	unsigned /*flags*/,
	const unsigned char */*p*/,
	size_t /*len*/,
	void */*data*/,
	size_t */*size*/);

int
_asn1_encode (
	const struct asn1_template */*t*/,
	unsigned char */*p*/,
	size_t /*len*/,
	const void */*data*/,
	size_t */*size*/);

int
_asn1_encode_fuzzer (
	const struct asn1_template */*t*/,
	unsigned char */*p*/,
	size_t /*len*/,
	const void */*data*/,
	size_t */*size*/);

void
_asn1_free (
	const struct asn1_template */*t*/,
	void */*data*/);

void
_asn1_free_top (
	const struct asn1_template */*t*/,
	void */*data*/);

size_t
_asn1_length (
	const struct asn1_template */*t*/,
	const void */*data*/);

size_t
_asn1_length_fuzzer (
	const struct asn1_template */*t*/,
	const void */*data*/);

size_t
_asn1_sizeofType (const struct asn1_template */*t*/);

struct tm *
_der_gmtime (
	time_t /*t*/,
	struct tm */*tm*/);

int
_heim_der_set_sort (
	const void */*a1*/,
	const void */*a2*/);

int
_heim_fix_dce (
	size_t /*reallen*/,
	size_t */*len*/);

size_t
_heim_len_int (int /*val*/);

size_t
_heim_len_int64 (int64_t /*val*/);

size_t
_heim_len_unsigned (unsigned /*val*/);

size_t
_heim_len_unsigned64 (uint64_t /*val*/);

int
_heim_time2generalizedtime (
	time_t /*t*/,
	heim_octet_string */*s*/,
	int /*gtimep*/);

#endif /* __der_private_h__ */
