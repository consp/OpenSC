/*
 * asn1.h: ASN.1 header file
 *
 * Copyright (C) 2001, 2002  Juha Yrj�l� <juha.yrjola@iki.fi>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef _OPENSC_ASN1_H
#define _OPENSC_ASN1_H

#ifdef __cplusplus
extern "C" {
#endif

#include <opensc/opensc.h>
#include <opensc/pkcs15.h>

struct sc_asn1_entry {
	const char *name;
	unsigned int type;
	unsigned int tag;
	unsigned int flags;
	void *parm;
	void *arg;
};

struct sc_asn1_pkcs15_object {
	struct sc_pkcs15_object *p15_obj;
	struct sc_asn1_entry *asn1_class_attr;
	struct sc_asn1_entry *asn1_subclass_attr;
	struct sc_asn1_entry *asn1_type_attr;
};

struct sc_asn1_pkcs15_algorithm_info {
	int id;
	struct sc_object_id oid;
	int (*decode)(struct sc_context *, void **, const u8 *, size_t, int);
	int (*encode)(struct sc_context *, void *, u8 **, size_t *, int);
	void (*free)(void *);
};


/* Utility functions */
void sc_format_asn1_entry(struct sc_asn1_entry *entry, void *parm, void *arg,
			  int set_present);
void sc_copy_asn1_entry(const struct sc_asn1_entry *src,
			struct sc_asn1_entry *dest);
			
/* DER tag and length parsing */
int sc_asn1_decode(struct sc_context *ctx, struct sc_asn1_entry *asn1,
		   const u8 *in, size_t len, const u8 **newp, size_t *left);
int sc_asn1_decode_choice(struct sc_context *ctx, struct sc_asn1_entry *asn1,
		   const u8 *in, size_t len, const u8 **newp, size_t *left);
int sc_asn1_encode(struct sc_context *ctx, const struct sc_asn1_entry *asn1,
		   u8 **buf, size_t *bufsize);
int _sc_asn1_decode(struct sc_context *, struct sc_asn1_entry *,
		   const u8 *, size_t, const u8 **, size_t *,
		   int, int);
int _sc_asn1_encode(struct sc_context *, const struct sc_asn1_entry *,
		   u8 **, size_t *, int);

const u8 *sc_asn1_find_tag(struct sc_context *ctx, const u8 * buf,
			   size_t buflen, unsigned int tag, size_t *taglen);
const u8 *sc_asn1_verify_tag(struct sc_context *ctx, const u8 * buf,
			     size_t buflen, unsigned int tag, size_t *taglen);
const u8 *sc_asn1_skip_tag(struct sc_context *ctx, const u8 ** buf,
			   size_t *buflen, unsigned int tag, size_t *taglen);

/* DER encoding */

/* Argument 'ptr' is set to the location of the next possible ASN.1 object.
 * If NULL, no action on 'ptr' is performed. */
int sc_asn1_put_tag(int tag, const u8 * data, int datalen, u8 * out, int outlen, u8 ** ptr);

/* ASN.1 printing functions */
void sc_asn1_print_tags(const u8 * buf, size_t buflen);

/* ASN.1 object decoding functions */
int sc_asn1_utf8string_to_ascii(const u8 * buf, size_t buflen,
				u8 * outbuf, size_t outlen);
int sc_asn1_decode_bit_string(const u8 * inbuf, size_t inlen,
			      void *outbuf, size_t outlen);
/* non-inverting version */
int sc_asn1_decode_bit_string_ni(const u8 * inbuf, size_t inlen,
				 void *outbuf, size_t outlen);
int sc_asn1_decode_integer(const u8 * inbuf, size_t inlen, int *out);
int sc_asn1_decode_object_id(const u8 * inbuf, size_t inlen,
			     struct sc_object_id *id);

/* algorithm encoding/decoding */
int sc_asn1_decode_algorithm_id(struct sc_context *,
				const u8 *, size_t,
				struct sc_algorithm_id *, int);
int sc_asn1_encode_algorithm_id(struct sc_context *,
				u8 **, size_t *,
				const struct sc_algorithm_id *, int);
void sc_asn1_clear_algorithm_id(struct sc_algorithm_id *);

#define SC_ASN1_CLASS_MASK		0x30000000
#define SC_ASN1_UNI			0x00000000 /* Universal */
#define SC_ASN1_APP			0x10000000 /* Application */
#define SC_ASN1_CTX			0x20000000 /* Context */
#define SC_ASN1_PRV			0x30000000 /* Private */
#define SC_ASN1_CONS			0x01000000

#define SC_ASN1_TAG_MASK		0x00FFFFFF

#define SC_ASN1_PRESENT			0x00000001
#define SC_ASN1_OPTIONAL		0x00000002
#define SC_ASN1_ALLOC			0x00000004
#define SC_ASN1_UNSIGNED		0x00000008

#define SC_ASN1_BOOLEAN                 1
#define SC_ASN1_INTEGER                 2
#define SC_ASN1_BIT_STRING              3
#define SC_ASN1_BIT_STRING_NI           128
#define SC_ASN1_OCTET_STRING            4
#define SC_ASN1_NULL                    5
#define SC_ASN1_OBJECT                  6
#define SC_ASN1_ENUMERATED              10
#define SC_ASN1_UTF8STRING              12
#define SC_ASN1_SEQUENCE                16
#define SC_ASN1_SET                     17
#define SC_ASN1_PRINTABLESTRING         19
#define SC_ASN1_UTCTIME                 23
#define SC_ASN1_GENERALIZEDTIME         24

/* internal structures */
#define SC_ASN1_STRUCT			129
#define SC_ASN1_CHOICE			130
#define SC_ASN1_BIT_FIELD		131	/* bit string as integer */

/* 'complex' structures */
#define SC_ASN1_PATH			256
#define SC_ASN1_PKCS15_ID		257
#define SC_ASN1_PKCS15_OBJECT		258
#define SC_ASN1_ALGORITHM_ID		259

/* use callback function */
#define SC_ASN1_CALLBACK		384

#define ASN1_TAG_CLASS			0xC0
#define ASN1_TAG_UNIVERSAL		0x00
#define ASN1_TAG_APPLICATION		0x40
#define ASN1_TAG_CONTEXT		0x80
#define ASN1_TAG_PRIVATE		0xC0

#define ASN1_TAG_CONSTRUCTED		0x20
#define ASN1_TAG_PRIMITIVE		0x1F

#define ASN1_EOC                      0
#define ASN1_BOOLEAN                  1
#define ASN1_INTEGER                  2
#define ASN1_NEG_INTEGER              (2 | ASN1_NEG)
#define ASN1_BIT_STRING               3
#define ASN1_OCTET_STRING             4
#define ASN1_NULL                     5
#define ASN1_OBJECT                   6
#define ASN1_OBJECT_DESCRIPTOR        7
#define ASN1_EXTERNAL                 8
#define ASN1_REAL                     9
#define ASN1_ENUMERATED               10
#define ASN1_NEG_ENUMERATED           (10 | ASN1_NEG)
#define ASN1_UTF8STRING               12
#define ASN1_SEQUENCE                 16
#define ASN1_SET                      17
#define ASN1_NUMERICSTRING            18
#define ASN1_PRINTABLESTRING          19
#define ASN1_T61STRING                20
#define ASN1_TELETEXSTRING            20
#define ASN1_VIDEOTEXSTRING           21
#define ASN1_IA5STRING                22
#define ASN1_UTCTIME                  23
#define ASN1_GENERALIZEDTIME          24
#define ASN1_GRAPHICSTRING            25
#define ASN1_ISO64STRING              26
#define ASN1_VISIBLESTRING            26
#define ASN1_GENERALSTRING            27
#define ASN1_UNIVERSALSTRING          28
#define ASN1_BMPSTRING                30

#ifdef __cplusplus
}
#endif

#endif
