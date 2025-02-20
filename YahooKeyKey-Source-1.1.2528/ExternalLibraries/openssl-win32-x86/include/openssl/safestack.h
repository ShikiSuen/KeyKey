/* ====================================================================
 * Copyright (c) 1999 The OpenSSL Project.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. All advertising materials mentioning features or use of this
 *    software must display the following acknowledgment:
 *    "This product includes software developed by the OpenSSL Project
 *    for use in the OpenSSL Toolkit. (http://www.openssl.org/)"
 *
 * 4. The names "OpenSSL Toolkit" and "OpenSSL Project" must not be used to
 *    endorse or promote products derived from this software without
 *    prior written permission. For written permission, please contact
 *    openssl-core@openssl.org.
 *
 * 5. Products derived from this software may not be called "OpenSSL"
 *    nor may "OpenSSL" appear in their names without prior written
 *    permission of the OpenSSL Project.
 *
 * 6. Redistributions of any form whatsoever must retain the following
 *    acknowledgment:
 *    "This product includes software developed by the OpenSSL Project
 *    for use in the OpenSSL Toolkit (http://www.openssl.org/)"
 *
 * THIS SOFTWARE IS PROVIDED BY THE OpenSSL PROJECT ``AS IS'' AND ANY
 * EXPRESSED OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE OpenSSL PROJECT OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 * ====================================================================
 *
 * This product includes cryptographic software written by Eric Young
 * (eay@cryptsoft.com).  This product includes software written by Tim
 * Hudson (tjh@cryptsoft.com).
 *
 */

#ifndef HEADER_SAFESTACK_H
#define HEADER_SAFESTACK_H

#include <openssl/stack.h>

#ifdef DEBUG_SAFESTACK

#ifndef CHECKED_PTR_OF
#define CHECKED_PTR_OF(type, p) ((void *)(1 ? p : (type *)0))
#endif

#define CHECKED_SK_FREE_FUNC(type, p) \
  ((void (*)(void *))((1 ? p : (void (*)(type *))0)))

#define CHECKED_SK_CMP_FUNC(type, p)                    \
  ((int (*)(const char *const *, const char *const *))( \
      (1 ? p : (int (*)(const type *const *, const type *const *))0)))

#define STACK_OF(type) struct stack_st_##type
#define PREDECLARE_STACK_OF(type) STACK_OF(type);

#define DECLARE_STACK_OF(type) \
  STACK_OF(type) { STACK stack; };

#define IMPLEMENT_STACK_OF(type) /* nada (obsolete in new safestack \
                                    approach)*/

/* SKM_sk_... stack macros are internal to safestack.h:
 * never use them directly, use sk_<type>_... instead */
#define SKM_sk_new(type, cmp) \
  ((STACK_OF(type) *)sk_new(CHECKED_SK_CMP_FUNC(type, cmp)))
#define SKM_sk_new_null(type) ((STACK_OF(type) *)sk_new_null())
#define SKM_sk_free(type, st) sk_free(CHECKED_PTR_OF(STACK_OF(type), st))
#define SKM_sk_num(type, st) sk_num(CHECKED_PTR_OF(STACK_OF(type), st))
#define SKM_sk_value(type, st, i) \
  ((type *)sk_value(CHECKED_PTR_OF(STACK_OF(type), st), i))
#define SKM_sk_set(type, st, i, val) \
  sk_set(CHECKED_PTR_OF(STACK_OF(type), st), i, CHECKED_PTR_OF(type, val))
#define SKM_sk_zero(type, st) sk_zero(CHECKED_PTR_OF(STACK_OF(type), st))
#define SKM_sk_push(type, st, val) \
  sk_push(CHECKED_PTR_OF(STACK_OF(type), st), CHECKED_PTR_OF(type, val))
#define SKM_sk_unshift(type, st, val) \
  sk_unshift(CHECKED_PTR_OF(STACK_OF(type), st), CHECKED_PTR_OF(type, val))
#define SKM_sk_find(type, st, val) \
  sk_find(CHECKED_PTR_OF(STACK_OF(type), st), CHECKED_PTR_OF(type, val))
#define SKM_sk_delete(type, st, i) \
  (type *)sk_delete(CHECKED_PTR_OF(STACK_OF(type), st), i)
#define SKM_sk_delete_ptr(type, st, ptr)                    \
  (type *)sk_delete_ptr(CHECKED_PTR_OF(STACK_OF(type), st), \
                        CHECKED_PTR_OF(type, ptr))
#define SKM_sk_insert(type, st, val, i) \
  sk_insert(CHECKED_PTR_OF(STACK_OF(type), st), CHECKED_PTR_OF(type, val), i)
#define SKM_sk_set_cmp_func(type, st, cmp)                             \
  ((int (*)(const type *const *, const type *const *))sk_set_cmp_func( \
      CHECKED_PTR_OF(STACK_OF(type), st), CHECKED_SK_CMP_FUNC(type, cmp)))
#define SKM_sk_dup(type, st) \
  (STACK_OF(type) *)sk_dup(CHECKED_PTR_OF(STACK_OF(type), st))
#define SKM_sk_pop_free(type, st, free_func)      \
  sk_pop_free(CHECKED_PTR_OF(STACK_OF(type), st), \
              CHECKED_SK_FREE_FUNC(type, free_func))
#define SKM_sk_shift(type, st) \
  (type *)sk_shift(CHECKED_PTR_OF(STACK_OF(type), st))
#define SKM_sk_pop(type, st) (type *)sk_pop(CHECKED_PTR_OF(STACK_OF(type), st))
#define SKM_sk_sort(type, st) sk_sort(CHECKED_PTR_OF(STACK_OF(type), st))
#define SKM_sk_is_sorted(type, st) \
  sk_is_sorted(CHECKED_PTR_OF(STACK_OF(type), st))

#define SKM_ASN1_SET_OF_d2i(type, st, pp, length, d2i_func, free_func, ex_tag, \
                            ex_class)                                          \
  (STACK_OF(type) *)d2i_ASN1_SET(CHECKED_PTR_OF(STACK_OF(type), st), pp,       \
                                 length, CHECKED_D2I_OF(type, d2i_func),       \
                                 CHECKED_SK_FREE_FUNC(type, free_func),        \
                                 ex_tag, ex_class)

#define SKM_ASN1_SET_OF_i2d(type, st, pp, i2d_func, ex_tag, ex_class, is_set) \
  i2d_ASN1_SET(CHECKED_PTR_OF(STACK_OF(type), st), pp,                        \
               CHECKED_I2D_OF(type, i2d_func), ex_tag, ex_class, is_set)

#define SKM_ASN1_seq_pack(type, st, i2d_func, buf, len) \
  ASN1_seq_pack(CHECKED_PTR_OF(STACK_OF(type), st),     \
                CHECKED_I2D_OF(type, i2d_func), buf, len)

#define SKM_ASN1_seq_unpack(type, buf, len, d2i_func, free_func)              \
  (STACK_OF(type) *)ASN1_seq_unpack(buf, len, CHECKED_D2I_OF(type, d2i_func), \
                                    CHECKED_SK_FREE_FUNC(type, free_func))

#define SKM_PKCS12_decrypt_d2i(type, algor, d2i_func, free_func, pass,        \
                               passlen, oct, seq)                             \
  (STACK_OF(type) *)PKCS12_decrypt_d2i(algor, CHECKED_D2I_OF(type, d2i_func), \
                                       CHECKED_SK_FREE_FUNC(type, free_func), \
                                       pass, passlen, oct, seq)

#else

#define STACK_OF(type) STACK
#define PREDECLARE_STACK_OF(type) /* nada */
#define DECLARE_STACK_OF(type)    /* nada */
#define IMPLEMENT_STACK_OF(type)  /* nada */

#define SKM_sk_new(type, cmp) \
  sk_new((int (*)(const char *const *, const char *const *))(cmp))
#define SKM_sk_new_null(type) sk_new_null()
#define SKM_sk_free(type, st) sk_free(st)
#define SKM_sk_num(type, st) sk_num(st)
#define SKM_sk_value(type, st, i) ((type *)sk_value(st, i))
#define SKM_sk_set(type, st, i, val) ((type *)sk_set(st, i, (char *)val))
#define SKM_sk_zero(type, st) sk_zero(st)
#define SKM_sk_push(type, st, val) sk_push(st, (char *)val)
#define SKM_sk_unshift(type, st, val) sk_unshift(st, val)
#define SKM_sk_find(type, st, val) sk_find(st, (char *)val)
#define SKM_sk_delete(type, st, i) ((type *)sk_delete(st, i))
#define SKM_sk_delete_ptr(type, st, ptr) \
  ((type *)sk_delete_ptr(st, (char *)ptr))
#define SKM_sk_insert(type, st, val, i) sk_insert(st, (char *)val, i)
#define SKM_sk_set_cmp_func(type, st, cmp)                             \
  ((int (*)(const type *const *, const type *const *))sk_set_cmp_func( \
      st, (int (*)(const char *const *, const char *const *))(cmp)))
#define SKM_sk_dup(type, st) sk_dup(st)
#define SKM_sk_pop_free(type, st, free_func) \
  sk_pop_free(st, (void (*)(void *))free_func)
#define SKM_sk_shift(type, st) ((type *)sk_shift(st))
#define SKM_sk_pop(type, st) ((type *)sk_pop(st))
#define SKM_sk_sort(type, st) sk_sort(st)
#define SKM_sk_is_sorted(type, st) sk_is_sorted(st)

#define SKM_ASN1_SET_OF_d2i(type, st, pp, length, d2i_func, free_func, ex_tag, \
                            ex_class)                                          \
  d2i_ASN1_SET(st, pp, length,                                                 \
               (void *(*)(void **, const unsigned char **, long))d2i_func,     \
               (void (*)(void *))free_func, ex_tag, ex_class)
#define SKM_ASN1_SET_OF_i2d(type, st, pp, i2d_func, ex_tag, ex_class, is_set) \
  i2d_ASN1_SET(st, pp, (int (*)(void *, unsigned char **))i2d_func, ex_tag,   \
               ex_class, is_set)

#define SKM_ASN1_seq_pack(type, st, i2d_func, buf, len) \
  ASN1_seq_pack(st, (int (*)(void *, unsigned char **))i2d_func, buf, len)
#define SKM_ASN1_seq_unpack(type, buf, len, d2i_func, free_func)              \
  ASN1_seq_unpack(buf, len,                                                   \
                  (void *(*)(void **, const unsigned char **, long))d2i_func, \
                  (void (*)(void *))free_func)

#define SKM_PKCS12_decrypt_d2i(type, algor, d2i_func, free_func, pass,     \
                               passlen, oct, seq)                          \
  ((STACK *)PKCS12_decrypt_d2i(algor, (char *(*)())d2i_func,               \
                               (void (*)(void *))free_func, pass, passlen, \
                               oct, seq))

#endif

/* This block of defines is updated by util/mkstack.pl, please do not touch! */
#define sk_ACCESS_DESCRIPTION_new(st) SKM_sk_new(ACCESS_DESCRIPTION, (st))
#define sk_ACCESS_DESCRIPTION_new_null() SKM_sk_new_null(ACCESS_DESCRIPTION)
#define sk_ACCESS_DESCRIPTION_free(st) SKM_sk_free(ACCESS_DESCRIPTION, (st))
#define sk_ACCESS_DESCRIPTION_num(st) SKM_sk_num(ACCESS_DESCRIPTION, (st))
#define sk_ACCESS_DESCRIPTION_value(st, i) \
  SKM_sk_value(ACCESS_DESCRIPTION, (st), (i))
#define sk_ACCESS_DESCRIPTION_set(st, i, val) \
  SKM_sk_set(ACCESS_DESCRIPTION, (st), (i), (val))
#define sk_ACCESS_DESCRIPTION_zero(st) SKM_sk_zero(ACCESS_DESCRIPTION, (st))
#define sk_ACCESS_DESCRIPTION_push(st, val) \
  SKM_sk_push(ACCESS_DESCRIPTION, (st), (val))
#define sk_ACCESS_DESCRIPTION_unshift(st, val) \
  SKM_sk_unshift(ACCESS_DESCRIPTION, (st), (val))
#define sk_ACCESS_DESCRIPTION_find(st, val) \
  SKM_sk_find(ACCESS_DESCRIPTION, (st), (val))
#define sk_ACCESS_DESCRIPTION_find_ex(st, val) \
  SKM_sk_find_ex(ACCESS_DESCRIPTION, (st), (val))
#define sk_ACCESS_DESCRIPTION_delete(st, i) \
  SKM_sk_delete(ACCESS_DESCRIPTION, (st), (i))
#define sk_ACCESS_DESCRIPTION_delete_ptr(st, ptr) \
  SKM_sk_delete_ptr(ACCESS_DESCRIPTION, (st), (ptr))
#define sk_ACCESS_DESCRIPTION_insert(st, val, i) \
  SKM_sk_insert(ACCESS_DESCRIPTION, (st), (val), (i))
#define sk_ACCESS_DESCRIPTION_set_cmp_func(st, cmp) \
  SKM_sk_set_cmp_func(ACCESS_DESCRIPTION, (st), (cmp))
#define sk_ACCESS_DESCRIPTION_dup(st) SKM_sk_dup(ACCESS_DESCRIPTION, st)
#define sk_ACCESS_DESCRIPTION_pop_free(st, free_func) \
  SKM_sk_pop_free(ACCESS_DESCRIPTION, (st), (free_func))
#define sk_ACCESS_DESCRIPTION_shift(st) SKM_sk_shift(ACCESS_DESCRIPTION, (st))
#define sk_ACCESS_DESCRIPTION_pop(st) SKM_sk_pop(ACCESS_DESCRIPTION, (st))
#define sk_ACCESS_DESCRIPTION_sort(st) SKM_sk_sort(ACCESS_DESCRIPTION, (st))
#define sk_ACCESS_DESCRIPTION_is_sorted(st) \
  SKM_sk_is_sorted(ACCESS_DESCRIPTION, (st))

#define sk_ASIdOrRange_new(st) SKM_sk_new(ASIdOrRange, (st))
#define sk_ASIdOrRange_new_null() SKM_sk_new_null(ASIdOrRange)
#define sk_ASIdOrRange_free(st) SKM_sk_free(ASIdOrRange, (st))
#define sk_ASIdOrRange_num(st) SKM_sk_num(ASIdOrRange, (st))
#define sk_ASIdOrRange_value(st, i) SKM_sk_value(ASIdOrRange, (st), (i))
#define sk_ASIdOrRange_set(st, i, val) SKM_sk_set(ASIdOrRange, (st), (i), (val))
#define sk_ASIdOrRange_zero(st) SKM_sk_zero(ASIdOrRange, (st))
#define sk_ASIdOrRange_push(st, val) SKM_sk_push(ASIdOrRange, (st), (val))
#define sk_ASIdOrRange_unshift(st, val) SKM_sk_unshift(ASIdOrRange, (st), (val))
#define sk_ASIdOrRange_find(st, val) SKM_sk_find(ASIdOrRange, (st), (val))
#define sk_ASIdOrRange_find_ex(st, val) SKM_sk_find_ex(ASIdOrRange, (st), (val))
#define sk_ASIdOrRange_delete(st, i) SKM_sk_delete(ASIdOrRange, (st), (i))
#define sk_ASIdOrRange_delete_ptr(st, ptr) \
  SKM_sk_delete_ptr(ASIdOrRange, (st), (ptr))
#define sk_ASIdOrRange_insert(st, val, i) \
  SKM_sk_insert(ASIdOrRange, (st), (val), (i))
#define sk_ASIdOrRange_set_cmp_func(st, cmp) \
  SKM_sk_set_cmp_func(ASIdOrRange, (st), (cmp))
#define sk_ASIdOrRange_dup(st) SKM_sk_dup(ASIdOrRange, st)
#define sk_ASIdOrRange_pop_free(st, free_func) \
  SKM_sk_pop_free(ASIdOrRange, (st), (free_func))
#define sk_ASIdOrRange_shift(st) SKM_sk_shift(ASIdOrRange, (st))
#define sk_ASIdOrRange_pop(st) SKM_sk_pop(ASIdOrRange, (st))
#define sk_ASIdOrRange_sort(st) SKM_sk_sort(ASIdOrRange, (st))
#define sk_ASIdOrRange_is_sorted(st) SKM_sk_is_sorted(ASIdOrRange, (st))

#define sk_ASN1_GENERALSTRING_new(st) SKM_sk_new(ASN1_GENERALSTRING, (st))
#define sk_ASN1_GENERALSTRING_new_null() SKM_sk_new_null(ASN1_GENERALSTRING)
#define sk_ASN1_GENERALSTRING_free(st) SKM_sk_free(ASN1_GENERALSTRING, (st))
#define sk_ASN1_GENERALSTRING_num(st) SKM_sk_num(ASN1_GENERALSTRING, (st))
#define sk_ASN1_GENERALSTRING_value(st, i) \
  SKM_sk_value(ASN1_GENERALSTRING, (st), (i))
#define sk_ASN1_GENERALSTRING_set(st, i, val) \
  SKM_sk_set(ASN1_GENERALSTRING, (st), (i), (val))
#define sk_ASN1_GENERALSTRING_zero(st) SKM_sk_zero(ASN1_GENERALSTRING, (st))
#define sk_ASN1_GENERALSTRING_push(st, val) \
  SKM_sk_push(ASN1_GENERALSTRING, (st), (val))
#define sk_ASN1_GENERALSTRING_unshift(st, val) \
  SKM_sk_unshift(ASN1_GENERALSTRING, (st), (val))
#define sk_ASN1_GENERALSTRING_find(st, val) \
  SKM_sk_find(ASN1_GENERALSTRING, (st), (val))
#define sk_ASN1_GENERALSTRING_find_ex(st, val) \
  SKM_sk_find_ex(ASN1_GENERALSTRING, (st), (val))
#define sk_ASN1_GENERALSTRING_delete(st, i) \
  SKM_sk_delete(ASN1_GENERALSTRING, (st), (i))
#define sk_ASN1_GENERALSTRING_delete_ptr(st, ptr) \
  SKM_sk_delete_ptr(ASN1_GENERALSTRING, (st), (ptr))
#define sk_ASN1_GENERALSTRING_insert(st, val, i) \
  SKM_sk_insert(ASN1_GENERALSTRING, (st), (val), (i))
#define sk_ASN1_GENERALSTRING_set_cmp_func(st, cmp) \
  SKM_sk_set_cmp_func(ASN1_GENERALSTRING, (st), (cmp))
#define sk_ASN1_GENERALSTRING_dup(st) SKM_sk_dup(ASN1_GENERALSTRING, st)
#define sk_ASN1_GENERALSTRING_pop_free(st, free_func) \
  SKM_sk_pop_free(ASN1_GENERALSTRING, (st), (free_func))
#define sk_ASN1_GENERALSTRING_shift(st) SKM_sk_shift(ASN1_GENERALSTRING, (st))
#define sk_ASN1_GENERALSTRING_pop(st) SKM_sk_pop(ASN1_GENERALSTRING, (st))
#define sk_ASN1_GENERALSTRING_sort(st) SKM_sk_sort(ASN1_GENERALSTRING, (st))
#define sk_ASN1_GENERALSTRING_is_sorted(st) \
  SKM_sk_is_sorted(ASN1_GENERALSTRING, (st))

#define sk_ASN1_INTEGER_new(st) SKM_sk_new(ASN1_INTEGER, (st))
#define sk_ASN1_INTEGER_new_null() SKM_sk_new_null(ASN1_INTEGER)
#define sk_ASN1_INTEGER_free(st) SKM_sk_free(ASN1_INTEGER, (st))
#define sk_ASN1_INTEGER_num(st) SKM_sk_num(ASN1_INTEGER, (st))
#define sk_ASN1_INTEGER_value(st, i) SKM_sk_value(ASN1_INTEGER, (st), (i))
#define sk_ASN1_INTEGER_set(st, i, val) \
  SKM_sk_set(ASN1_INTEGER, (st), (i), (val))
#define sk_ASN1_INTEGER_zero(st) SKM_sk_zero(ASN1_INTEGER, (st))
#define sk_ASN1_INTEGER_push(st, val) SKM_sk_push(ASN1_INTEGER, (st), (val))
#define sk_ASN1_INTEGER_unshift(st, val) \
  SKM_sk_unshift(ASN1_INTEGER, (st), (val))
#define sk_ASN1_INTEGER_find(st, val) SKM_sk_find(ASN1_INTEGER, (st), (val))
#define sk_ASN1_INTEGER_find_ex(st, val) \
  SKM_sk_find_ex(ASN1_INTEGER, (st), (val))
#define sk_ASN1_INTEGER_delete(st, i) SKM_sk_delete(ASN1_INTEGER, (st), (i))
#define sk_ASN1_INTEGER_delete_ptr(st, ptr) \
  SKM_sk_delete_ptr(ASN1_INTEGER, (st), (ptr))
#define sk_ASN1_INTEGER_insert(st, val, i) \
  SKM_sk_insert(ASN1_INTEGER, (st), (val), (i))
#define sk_ASN1_INTEGER_set_cmp_func(st, cmp) \
  SKM_sk_set_cmp_func(ASN1_INTEGER, (st), (cmp))
#define sk_ASN1_INTEGER_dup(st) SKM_sk_dup(ASN1_INTEGER, st)
#define sk_ASN1_INTEGER_pop_free(st, free_func) \
  SKM_sk_pop_free(ASN1_INTEGER, (st), (free_func))
#define sk_ASN1_INTEGER_shift(st) SKM_sk_shift(ASN1_INTEGER, (st))
#define sk_ASN1_INTEGER_pop(st) SKM_sk_pop(ASN1_INTEGER, (st))
#define sk_ASN1_INTEGER_sort(st) SKM_sk_sort(ASN1_INTEGER, (st))
#define sk_ASN1_INTEGER_is_sorted(st) SKM_sk_is_sorted(ASN1_INTEGER, (st))

#define sk_ASN1_OBJECT_new(st) SKM_sk_new(ASN1_OBJECT, (st))
#define sk_ASN1_OBJECT_new_null() SKM_sk_new_null(ASN1_OBJECT)
#define sk_ASN1_OBJECT_free(st) SKM_sk_free(ASN1_OBJECT, (st))
#define sk_ASN1_OBJECT_num(st) SKM_sk_num(ASN1_OBJECT, (st))
#define sk_ASN1_OBJECT_value(st, i) SKM_sk_value(ASN1_OBJECT, (st), (i))
#define sk_ASN1_OBJECT_set(st, i, val) SKM_sk_set(ASN1_OBJECT, (st), (i), (val))
#define sk_ASN1_OBJECT_zero(st) SKM_sk_zero(ASN1_OBJECT, (st))
#define sk_ASN1_OBJECT_push(st, val) SKM_sk_push(ASN1_OBJECT, (st), (val))
#define sk_ASN1_OBJECT_unshift(st, val) SKM_sk_unshift(ASN1_OBJECT, (st), (val))
#define sk_ASN1_OBJECT_find(st, val) SKM_sk_find(ASN1_OBJECT, (st), (val))
#define sk_ASN1_OBJECT_find_ex(st, val) SKM_sk_find_ex(ASN1_OBJECT, (st), (val))
#define sk_ASN1_OBJECT_delete(st, i) SKM_sk_delete(ASN1_OBJECT, (st), (i))
#define sk_ASN1_OBJECT_delete_ptr(st, ptr) \
  SKM_sk_delete_ptr(ASN1_OBJECT, (st), (ptr))
#define sk_ASN1_OBJECT_insert(st, val, i) \
  SKM_sk_insert(ASN1_OBJECT, (st), (val), (i))
#define sk_ASN1_OBJECT_set_cmp_func(st, cmp) \
  SKM_sk_set_cmp_func(ASN1_OBJECT, (st), (cmp))
#define sk_ASN1_OBJECT_dup(st) SKM_sk_dup(ASN1_OBJECT, st)
#define sk_ASN1_OBJECT_pop_free(st, free_func) \
  SKM_sk_pop_free(ASN1_OBJECT, (st), (free_func))
#define sk_ASN1_OBJECT_shift(st) SKM_sk_shift(ASN1_OBJECT, (st))
#define sk_ASN1_OBJECT_pop(st) SKM_sk_pop(ASN1_OBJECT, (st))
#define sk_ASN1_OBJECT_sort(st) SKM_sk_sort(ASN1_OBJECT, (st))
#define sk_ASN1_OBJECT_is_sorted(st) SKM_sk_is_sorted(ASN1_OBJECT, (st))

#define sk_ASN1_STRING_TABLE_new(st) SKM_sk_new(ASN1_STRING_TABLE, (st))
#define sk_ASN1_STRING_TABLE_new_null() SKM_sk_new_null(ASN1_STRING_TABLE)
#define sk_ASN1_STRING_TABLE_free(st) SKM_sk_free(ASN1_STRING_TABLE, (st))
#define sk_ASN1_STRING_TABLE_num(st) SKM_sk_num(ASN1_STRING_TABLE, (st))
#define sk_ASN1_STRING_TABLE_value(st, i) \
  SKM_sk_value(ASN1_STRING_TABLE, (st), (i))
#define sk_ASN1_STRING_TABLE_set(st, i, val) \
  SKM_sk_set(ASN1_STRING_TABLE, (st), (i), (val))
#define sk_ASN1_STRING_TABLE_zero(st) SKM_sk_zero(ASN1_STRING_TABLE, (st))
#define sk_ASN1_STRING_TABLE_push(st, val) \
  SKM_sk_push(ASN1_STRING_TABLE, (st), (val))
#define sk_ASN1_STRING_TABLE_unshift(st, val) \
  SKM_sk_unshift(ASN1_STRING_TABLE, (st), (val))
#define sk_ASN1_STRING_TABLE_find(st, val) \
  SKM_sk_find(ASN1_STRING_TABLE, (st), (val))
#define sk_ASN1_STRING_TABLE_find_ex(st, val) \
  SKM_sk_find_ex(ASN1_STRING_TABLE, (st), (val))
#define sk_ASN1_STRING_TABLE_delete(st, i) \
  SKM_sk_delete(ASN1_STRING_TABLE, (st), (i))
#define sk_ASN1_STRING_TABLE_delete_ptr(st, ptr) \
  SKM_sk_delete_ptr(ASN1_STRING_TABLE, (st), (ptr))
#define sk_ASN1_STRING_TABLE_insert(st, val, i) \
  SKM_sk_insert(ASN1_STRING_TABLE, (st), (val), (i))
#define sk_ASN1_STRING_TABLE_set_cmp_func(st, cmp) \
  SKM_sk_set_cmp_func(ASN1_STRING_TABLE, (st), (cmp))
#define sk_ASN1_STRING_TABLE_dup(st) SKM_sk_dup(ASN1_STRING_TABLE, st)
#define sk_ASN1_STRING_TABLE_pop_free(st, free_func) \
  SKM_sk_pop_free(ASN1_STRING_TABLE, (st), (free_func))
#define sk_ASN1_STRING_TABLE_shift(st) SKM_sk_shift(ASN1_STRING_TABLE, (st))
#define sk_ASN1_STRING_TABLE_pop(st) SKM_sk_pop(ASN1_STRING_TABLE, (st))
#define sk_ASN1_STRING_TABLE_sort(st) SKM_sk_sort(ASN1_STRING_TABLE, (st))
#define sk_ASN1_STRING_TABLE_is_sorted(st) \
  SKM_sk_is_sorted(ASN1_STRING_TABLE, (st))

#define sk_ASN1_TYPE_new(st) SKM_sk_new(ASN1_TYPE, (st))
#define sk_ASN1_TYPE_new_null() SKM_sk_new_null(ASN1_TYPE)
#define sk_ASN1_TYPE_free(st) SKM_sk_free(ASN1_TYPE, (st))
#define sk_ASN1_TYPE_num(st) SKM_sk_num(ASN1_TYPE, (st))
#define sk_ASN1_TYPE_value(st, i) SKM_sk_value(ASN1_TYPE, (st), (i))
#define sk_ASN1_TYPE_set(st, i, val) SKM_sk_set(ASN1_TYPE, (st), (i), (val))
#define sk_ASN1_TYPE_zero(st) SKM_sk_zero(ASN1_TYPE, (st))
#define sk_ASN1_TYPE_push(st, val) SKM_sk_push(ASN1_TYPE, (st), (val))
#define sk_ASN1_TYPE_unshift(st, val) SKM_sk_unshift(ASN1_TYPE, (st), (val))
#define sk_ASN1_TYPE_find(st, val) SKM_sk_find(ASN1_TYPE, (st), (val))
#define sk_ASN1_TYPE_find_ex(st, val) SKM_sk_find_ex(ASN1_TYPE, (st), (val))
#define sk_ASN1_TYPE_delete(st, i) SKM_sk_delete(ASN1_TYPE, (st), (i))
#define sk_ASN1_TYPE_delete_ptr(st, ptr) \
  SKM_sk_delete_ptr(ASN1_TYPE, (st), (ptr))
#define sk_ASN1_TYPE_insert(st, val, i) \
  SKM_sk_insert(ASN1_TYPE, (st), (val), (i))
#define sk_ASN1_TYPE_set_cmp_func(st, cmp) \
  SKM_sk_set_cmp_func(ASN1_TYPE, (st), (cmp))
#define sk_ASN1_TYPE_dup(st) SKM_sk_dup(ASN1_TYPE, st)
#define sk_ASN1_TYPE_pop_free(st, free_func) \
  SKM_sk_pop_free(ASN1_TYPE, (st), (free_func))
#define sk_ASN1_TYPE_shift(st) SKM_sk_shift(ASN1_TYPE, (st))
#define sk_ASN1_TYPE_pop(st) SKM_sk_pop(ASN1_TYPE, (st))
#define sk_ASN1_TYPE_sort(st) SKM_sk_sort(ASN1_TYPE, (st))
#define sk_ASN1_TYPE_is_sorted(st) SKM_sk_is_sorted(ASN1_TYPE, (st))

#define sk_ASN1_VALUE_new(st) SKM_sk_new(ASN1_VALUE, (st))
#define sk_ASN1_VALUE_new_null() SKM_sk_new_null(ASN1_VALUE)
#define sk_ASN1_VALUE_free(st) SKM_sk_free(ASN1_VALUE, (st))
#define sk_ASN1_VALUE_num(st) SKM_sk_num(ASN1_VALUE, (st))
#define sk_ASN1_VALUE_value(st, i) SKM_sk_value(ASN1_VALUE, (st), (i))
#define sk_ASN1_VALUE_set(st, i, val) SKM_sk_set(ASN1_VALUE, (st), (i), (val))
#define sk_ASN1_VALUE_zero(st) SKM_sk_zero(ASN1_VALUE, (st))
#define sk_ASN1_VALUE_push(st, val) SKM_sk_push(ASN1_VALUE, (st), (val))
#define sk_ASN1_VALUE_unshift(st, val) SKM_sk_unshift(ASN1_VALUE, (st), (val))
#define sk_ASN1_VALUE_find(st, val) SKM_sk_find(ASN1_VALUE, (st), (val))
#define sk_ASN1_VALUE_find_ex(st, val) SKM_sk_find_ex(ASN1_VALUE, (st), (val))
#define sk_ASN1_VALUE_delete(st, i) SKM_sk_delete(ASN1_VALUE, (st), (i))
#define sk_ASN1_VALUE_delete_ptr(st, ptr) \
  SKM_sk_delete_ptr(ASN1_VALUE, (st), (ptr))
#define sk_ASN1_VALUE_insert(st, val, i) \
  SKM_sk_insert(ASN1_VALUE, (st), (val), (i))
#define sk_ASN1_VALUE_set_cmp_func(st, cmp) \
  SKM_sk_set_cmp_func(ASN1_VALUE, (st), (cmp))
#define sk_ASN1_VALUE_dup(st) SKM_sk_dup(ASN1_VALUE, st)
#define sk_ASN1_VALUE_pop_free(st, free_func) \
  SKM_sk_pop_free(ASN1_VALUE, (st), (free_func))
#define sk_ASN1_VALUE_shift(st) SKM_sk_shift(ASN1_VALUE, (st))
#define sk_ASN1_VALUE_pop(st) SKM_sk_pop(ASN1_VALUE, (st))
#define sk_ASN1_VALUE_sort(st) SKM_sk_sort(ASN1_VALUE, (st))
#define sk_ASN1_VALUE_is_sorted(st) SKM_sk_is_sorted(ASN1_VALUE, (st))

#define sk_BIO_new(st) SKM_sk_new(BIO, (st))
#define sk_BIO_new_null() SKM_sk_new_null(BIO)
#define sk_BIO_free(st) SKM_sk_free(BIO, (st))
#define sk_BIO_num(st) SKM_sk_num(BIO, (st))
#define sk_BIO_value(st, i) SKM_sk_value(BIO, (st), (i))
#define sk_BIO_set(st, i, val) SKM_sk_set(BIO, (st), (i), (val))
#define sk_BIO_zero(st) SKM_sk_zero(BIO, (st))
#define sk_BIO_push(st, val) SKM_sk_push(BIO, (st), (val))
#define sk_BIO_unshift(st, val) SKM_sk_unshift(BIO, (st), (val))
#define sk_BIO_find(st, val) SKM_sk_find(BIO, (st), (val))
#define sk_BIO_find_ex(st, val) SKM_sk_find_ex(BIO, (st), (val))
#define sk_BIO_delete(st, i) SKM_sk_delete(BIO, (st), (i))
#define sk_BIO_delete_ptr(st, ptr) SKM_sk_delete_ptr(BIO, (st), (ptr))
#define sk_BIO_insert(st, val, i) SKM_sk_insert(BIO, (st), (val), (i))
#define sk_BIO_set_cmp_func(st, cmp) SKM_sk_set_cmp_func(BIO, (st), (cmp))
#define sk_BIO_dup(st) SKM_sk_dup(BIO, st)
#define sk_BIO_pop_free(st, free_func) SKM_sk_pop_free(BIO, (st), (free_func))
#define sk_BIO_shift(st) SKM_sk_shift(BIO, (st))
#define sk_BIO_pop(st) SKM_sk_pop(BIO, (st))
#define sk_BIO_sort(st) SKM_sk_sort(BIO, (st))
#define sk_BIO_is_sorted(st) SKM_sk_is_sorted(BIO, (st))

#define sk_CONF_IMODULE_new(st) SKM_sk_new(CONF_IMODULE, (st))
#define sk_CONF_IMODULE_new_null() SKM_sk_new_null(CONF_IMODULE)
#define sk_CONF_IMODULE_free(st) SKM_sk_free(CONF_IMODULE, (st))
#define sk_CONF_IMODULE_num(st) SKM_sk_num(CONF_IMODULE, (st))
#define sk_CONF_IMODULE_value(st, i) SKM_sk_value(CONF_IMODULE, (st), (i))
#define sk_CONF_IMODULE_set(st, i, val) \
  SKM_sk_set(CONF_IMODULE, (st), (i), (val))
#define sk_CONF_IMODULE_zero(st) SKM_sk_zero(CONF_IMODULE, (st))
#define sk_CONF_IMODULE_push(st, val) SKM_sk_push(CONF_IMODULE, (st), (val))
#define sk_CONF_IMODULE_unshift(st, val) \
  SKM_sk_unshift(CONF_IMODULE, (st), (val))
#define sk_CONF_IMODULE_find(st, val) SKM_sk_find(CONF_IMODULE, (st), (val))
#define sk_CONF_IMODULE_find_ex(st, val) \
  SKM_sk_find_ex(CONF_IMODULE, (st), (val))
#define sk_CONF_IMODULE_delete(st, i) SKM_sk_delete(CONF_IMODULE, (st), (i))
#define sk_CONF_IMODULE_delete_ptr(st, ptr) \
  SKM_sk_delete_ptr(CONF_IMODULE, (st), (ptr))
#define sk_CONF_IMODULE_insert(st, val, i) \
  SKM_sk_insert(CONF_IMODULE, (st), (val), (i))
#define sk_CONF_IMODULE_set_cmp_func(st, cmp) \
  SKM_sk_set_cmp_func(CONF_IMODULE, (st), (cmp))
#define sk_CONF_IMODULE_dup(st) SKM_sk_dup(CONF_IMODULE, st)
#define sk_CONF_IMODULE_pop_free(st, free_func) \
  SKM_sk_pop_free(CONF_IMODULE, (st), (free_func))
#define sk_CONF_IMODULE_shift(st) SKM_sk_shift(CONF_IMODULE, (st))
#define sk_CONF_IMODULE_pop(st) SKM_sk_pop(CONF_IMODULE, (st))
#define sk_CONF_IMODULE_sort(st) SKM_sk_sort(CONF_IMODULE, (st))
#define sk_CONF_IMODULE_is_sorted(st) SKM_sk_is_sorted(CONF_IMODULE, (st))

#define sk_CONF_MODULE_new(st) SKM_sk_new(CONF_MODULE, (st))
#define sk_CONF_MODULE_new_null() SKM_sk_new_null(CONF_MODULE)
#define sk_CONF_MODULE_free(st) SKM_sk_free(CONF_MODULE, (st))
#define sk_CONF_MODULE_num(st) SKM_sk_num(CONF_MODULE, (st))
#define sk_CONF_MODULE_value(st, i) SKM_sk_value(CONF_MODULE, (st), (i))
#define sk_CONF_MODULE_set(st, i, val) SKM_sk_set(CONF_MODULE, (st), (i), (val))
#define sk_CONF_MODULE_zero(st) SKM_sk_zero(CONF_MODULE, (st))
#define sk_CONF_MODULE_push(st, val) SKM_sk_push(CONF_MODULE, (st), (val))
#define sk_CONF_MODULE_unshift(st, val) SKM_sk_unshift(CONF_MODULE, (st), (val))
#define sk_CONF_MODULE_find(st, val) SKM_sk_find(CONF_MODULE, (st), (val))
#define sk_CONF_MODULE_find_ex(st, val) SKM_sk_find_ex(CONF_MODULE, (st), (val))
#define sk_CONF_MODULE_delete(st, i) SKM_sk_delete(CONF_MODULE, (st), (i))
#define sk_CONF_MODULE_delete_ptr(st, ptr) \
  SKM_sk_delete_ptr(CONF_MODULE, (st), (ptr))
#define sk_CONF_MODULE_insert(st, val, i) \
  SKM_sk_insert(CONF_MODULE, (st), (val), (i))
#define sk_CONF_MODULE_set_cmp_func(st, cmp) \
  SKM_sk_set_cmp_func(CONF_MODULE, (st), (cmp))
#define sk_CONF_MODULE_dup(st) SKM_sk_dup(CONF_MODULE, st)
#define sk_CONF_MODULE_pop_free(st, free_func) \
  SKM_sk_pop_free(CONF_MODULE, (st), (free_func))
#define sk_CONF_MODULE_shift(st) SKM_sk_shift(CONF_MODULE, (st))
#define sk_CONF_MODULE_pop(st) SKM_sk_pop(CONF_MODULE, (st))
#define sk_CONF_MODULE_sort(st) SKM_sk_sort(CONF_MODULE, (st))
#define sk_CONF_MODULE_is_sorted(st) SKM_sk_is_sorted(CONF_MODULE, (st))

#define sk_CONF_VALUE_new(st) SKM_sk_new(CONF_VALUE, (st))
#define sk_CONF_VALUE_new_null() SKM_sk_new_null(CONF_VALUE)
#define sk_CONF_VALUE_free(st) SKM_sk_free(CONF_VALUE, (st))
#define sk_CONF_VALUE_num(st) SKM_sk_num(CONF_VALUE, (st))
#define sk_CONF_VALUE_value(st, i) SKM_sk_value(CONF_VALUE, (st), (i))
#define sk_CONF_VALUE_set(st, i, val) SKM_sk_set(CONF_VALUE, (st), (i), (val))
#define sk_CONF_VALUE_zero(st) SKM_sk_zero(CONF_VALUE, (st))
#define sk_CONF_VALUE_push(st, val) SKM_sk_push(CONF_VALUE, (st), (val))
#define sk_CONF_VALUE_unshift(st, val) SKM_sk_unshift(CONF_VALUE, (st), (val))
#define sk_CONF_VALUE_find(st, val) SKM_sk_find(CONF_VALUE, (st), (val))
#define sk_CONF_VALUE_find_ex(st, val) SKM_sk_find_ex(CONF_VALUE, (st), (val))
#define sk_CONF_VALUE_delete(st, i) SKM_sk_delete(CONF_VALUE, (st), (i))
#define sk_CONF_VALUE_delete_ptr(st, ptr) \
  SKM_sk_delete_ptr(CONF_VALUE, (st), (ptr))
#define sk_CONF_VALUE_insert(st, val, i) \
  SKM_sk_insert(CONF_VALUE, (st), (val), (i))
#define sk_CONF_VALUE_set_cmp_func(st, cmp) \
  SKM_sk_set_cmp_func(CONF_VALUE, (st), (cmp))
#define sk_CONF_VALUE_dup(st) SKM_sk_dup(CONF_VALUE, st)
#define sk_CONF_VALUE_pop_free(st, free_func) \
  SKM_sk_pop_free(CONF_VALUE, (st), (free_func))
#define sk_CONF_VALUE_shift(st) SKM_sk_shift(CONF_VALUE, (st))
#define sk_CONF_VALUE_pop(st) SKM_sk_pop(CONF_VALUE, (st))
#define sk_CONF_VALUE_sort(st) SKM_sk_sort(CONF_VALUE, (st))
#define sk_CONF_VALUE_is_sorted(st) SKM_sk_is_sorted(CONF_VALUE, (st))

#define sk_CRYPTO_EX_DATA_FUNCS_new(st) SKM_sk_new(CRYPTO_EX_DATA_FUNCS, (st))
#define sk_CRYPTO_EX_DATA_FUNCS_new_null() SKM_sk_new_null(CRYPTO_EX_DATA_FUNCS)
#define sk_CRYPTO_EX_DATA_FUNCS_free(st) SKM_sk_free(CRYPTO_EX_DATA_FUNCS, (st))
#define sk_CRYPTO_EX_DATA_FUNCS_num(st) SKM_sk_num(CRYPTO_EX_DATA_FUNCS, (st))
#define sk_CRYPTO_EX_DATA_FUNCS_value(st, i) \
  SKM_sk_value(CRYPTO_EX_DATA_FUNCS, (st), (i))
#define sk_CRYPTO_EX_DATA_FUNCS_set(st, i, val) \
  SKM_sk_set(CRYPTO_EX_DATA_FUNCS, (st), (i), (val))
#define sk_CRYPTO_EX_DATA_FUNCS_zero(st) SKM_sk_zero(CRYPTO_EX_DATA_FUNCS, (st))
#define sk_CRYPTO_EX_DATA_FUNCS_push(st, val) \
  SKM_sk_push(CRYPTO_EX_DATA_FUNCS, (st), (val))
#define sk_CRYPTO_EX_DATA_FUNCS_unshift(st, val) \
  SKM_sk_unshift(CRYPTO_EX_DATA_FUNCS, (st), (val))
#define sk_CRYPTO_EX_DATA_FUNCS_find(st, val) \
  SKM_sk_find(CRYPTO_EX_DATA_FUNCS, (st), (val))
#define sk_CRYPTO_EX_DATA_FUNCS_find_ex(st, val) \
  SKM_sk_find_ex(CRYPTO_EX_DATA_FUNCS, (st), (val))
#define sk_CRYPTO_EX_DATA_FUNCS_delete(st, i) \
  SKM_sk_delete(CRYPTO_EX_DATA_FUNCS, (st), (i))
#define sk_CRYPTO_EX_DATA_FUNCS_delete_ptr(st, ptr) \
  SKM_sk_delete_ptr(CRYPTO_EX_DATA_FUNCS, (st), (ptr))
#define sk_CRYPTO_EX_DATA_FUNCS_insert(st, val, i) \
  SKM_sk_insert(CRYPTO_EX_DATA_FUNCS, (st), (val), (i))
#define sk_CRYPTO_EX_DATA_FUNCS_set_cmp_func(st, cmp) \
  SKM_sk_set_cmp_func(CRYPTO_EX_DATA_FUNCS, (st), (cmp))
#define sk_CRYPTO_EX_DATA_FUNCS_dup(st) SKM_sk_dup(CRYPTO_EX_DATA_FUNCS, st)
#define sk_CRYPTO_EX_DATA_FUNCS_pop_free(st, free_func) \
  SKM_sk_pop_free(CRYPTO_EX_DATA_FUNCS, (st), (free_func))
#define sk_CRYPTO_EX_DATA_FUNCS_shift(st) \
  SKM_sk_shift(CRYPTO_EX_DATA_FUNCS, (st))
#define sk_CRYPTO_EX_DATA_FUNCS_pop(st) SKM_sk_pop(CRYPTO_EX_DATA_FUNCS, (st))
#define sk_CRYPTO_EX_DATA_FUNCS_sort(st) SKM_sk_sort(CRYPTO_EX_DATA_FUNCS, (st))
#define sk_CRYPTO_EX_DATA_FUNCS_is_sorted(st) \
  SKM_sk_is_sorted(CRYPTO_EX_DATA_FUNCS, (st))

#define sk_CRYPTO_dynlock_new(st) SKM_sk_new(CRYPTO_dynlock, (st))
#define sk_CRYPTO_dynlock_new_null() SKM_sk_new_null(CRYPTO_dynlock)
#define sk_CRYPTO_dynlock_free(st) SKM_sk_free(CRYPTO_dynlock, (st))
#define sk_CRYPTO_dynlock_num(st) SKM_sk_num(CRYPTO_dynlock, (st))
#define sk_CRYPTO_dynlock_value(st, i) SKM_sk_value(CRYPTO_dynlock, (st), (i))
#define sk_CRYPTO_dynlock_set(st, i, val) \
  SKM_sk_set(CRYPTO_dynlock, (st), (i), (val))
#define sk_CRYPTO_dynlock_zero(st) SKM_sk_zero(CRYPTO_dynlock, (st))
#define sk_CRYPTO_dynlock_push(st, val) SKM_sk_push(CRYPTO_dynlock, (st), (val))
#define sk_CRYPTO_dynlock_unshift(st, val) \
  SKM_sk_unshift(CRYPTO_dynlock, (st), (val))
#define sk_CRYPTO_dynlock_find(st, val) SKM_sk_find(CRYPTO_dynlock, (st), (val))
#define sk_CRYPTO_dynlock_find_ex(st, val) \
  SKM_sk_find_ex(CRYPTO_dynlock, (st), (val))
#define sk_CRYPTO_dynlock_delete(st, i) SKM_sk_delete(CRYPTO_dynlock, (st), (i))
#define sk_CRYPTO_dynlock_delete_ptr(st, ptr) \
  SKM_sk_delete_ptr(CRYPTO_dynlock, (st), (ptr))
#define sk_CRYPTO_dynlock_insert(st, val, i) \
  SKM_sk_insert(CRYPTO_dynlock, (st), (val), (i))
#define sk_CRYPTO_dynlock_set_cmp_func(st, cmp) \
  SKM_sk_set_cmp_func(CRYPTO_dynlock, (st), (cmp))
#define sk_CRYPTO_dynlock_dup(st) SKM_sk_dup(CRYPTO_dynlock, st)
#define sk_CRYPTO_dynlock_pop_free(st, free_func) \
  SKM_sk_pop_free(CRYPTO_dynlock, (st), (free_func))
#define sk_CRYPTO_dynlock_shift(st) SKM_sk_shift(CRYPTO_dynlock, (st))
#define sk_CRYPTO_dynlock_pop(st) SKM_sk_pop(CRYPTO_dynlock, (st))
#define sk_CRYPTO_dynlock_sort(st) SKM_sk_sort(CRYPTO_dynlock, (st))
#define sk_CRYPTO_dynlock_is_sorted(st) SKM_sk_is_sorted(CRYPTO_dynlock, (st))

#define sk_DIST_POINT_new(st) SKM_sk_new(DIST_POINT, (st))
#define sk_DIST_POINT_new_null() SKM_sk_new_null(DIST_POINT)
#define sk_DIST_POINT_free(st) SKM_sk_free(DIST_POINT, (st))
#define sk_DIST_POINT_num(st) SKM_sk_num(DIST_POINT, (st))
#define sk_DIST_POINT_value(st, i) SKM_sk_value(DIST_POINT, (st), (i))
#define sk_DIST_POINT_set(st, i, val) SKM_sk_set(DIST_POINT, (st), (i), (val))
#define sk_DIST_POINT_zero(st) SKM_sk_zero(DIST_POINT, (st))
#define sk_DIST_POINT_push(st, val) SKM_sk_push(DIST_POINT, (st), (val))
#define sk_DIST_POINT_unshift(st, val) SKM_sk_unshift(DIST_POINT, (st), (val))
#define sk_DIST_POINT_find(st, val) SKM_sk_find(DIST_POINT, (st), (val))
#define sk_DIST_POINT_find_ex(st, val) SKM_sk_find_ex(DIST_POINT, (st), (val))
#define sk_DIST_POINT_delete(st, i) SKM_sk_delete(DIST_POINT, (st), (i))
#define sk_DIST_POINT_delete_ptr(st, ptr) \
  SKM_sk_delete_ptr(DIST_POINT, (st), (ptr))
#define sk_DIST_POINT_insert(st, val, i) \
  SKM_sk_insert(DIST_POINT, (st), (val), (i))
#define sk_DIST_POINT_set_cmp_func(st, cmp) \
  SKM_sk_set_cmp_func(DIST_POINT, (st), (cmp))
#define sk_DIST_POINT_dup(st) SKM_sk_dup(DIST_POINT, st)
#define sk_DIST_POINT_pop_free(st, free_func) \
  SKM_sk_pop_free(DIST_POINT, (st), (free_func))
#define sk_DIST_POINT_shift(st) SKM_sk_shift(DIST_POINT, (st))
#define sk_DIST_POINT_pop(st) SKM_sk_pop(DIST_POINT, (st))
#define sk_DIST_POINT_sort(st) SKM_sk_sort(DIST_POINT, (st))
#define sk_DIST_POINT_is_sorted(st) SKM_sk_is_sorted(DIST_POINT, (st))

#define sk_ENGINE_new(st) SKM_sk_new(ENGINE, (st))
#define sk_ENGINE_new_null() SKM_sk_new_null(ENGINE)
#define sk_ENGINE_free(st) SKM_sk_free(ENGINE, (st))
#define sk_ENGINE_num(st) SKM_sk_num(ENGINE, (st))
#define sk_ENGINE_value(st, i) SKM_sk_value(ENGINE, (st), (i))
#define sk_ENGINE_set(st, i, val) SKM_sk_set(ENGINE, (st), (i), (val))
#define sk_ENGINE_zero(st) SKM_sk_zero(ENGINE, (st))
#define sk_ENGINE_push(st, val) SKM_sk_push(ENGINE, (st), (val))
#define sk_ENGINE_unshift(st, val) SKM_sk_unshift(ENGINE, (st), (val))
#define sk_ENGINE_find(st, val) SKM_sk_find(ENGINE, (st), (val))
#define sk_ENGINE_find_ex(st, val) SKM_sk_find_ex(ENGINE, (st), (val))
#define sk_ENGINE_delete(st, i) SKM_sk_delete(ENGINE, (st), (i))
#define sk_ENGINE_delete_ptr(st, ptr) SKM_sk_delete_ptr(ENGINE, (st), (ptr))
#define sk_ENGINE_insert(st, val, i) SKM_sk_insert(ENGINE, (st), (val), (i))
#define sk_ENGINE_set_cmp_func(st, cmp) SKM_sk_set_cmp_func(ENGINE, (st), (cmp))
#define sk_ENGINE_dup(st) SKM_sk_dup(ENGINE, st)
#define sk_ENGINE_pop_free(st, free_func) \
  SKM_sk_pop_free(ENGINE, (st), (free_func))
#define sk_ENGINE_shift(st) SKM_sk_shift(ENGINE, (st))
#define sk_ENGINE_pop(st) SKM_sk_pop(ENGINE, (st))
#define sk_ENGINE_sort(st) SKM_sk_sort(ENGINE, (st))
#define sk_ENGINE_is_sorted(st) SKM_sk_is_sorted(ENGINE, (st))

#define sk_ENGINE_CLEANUP_ITEM_new(st) SKM_sk_new(ENGINE_CLEANUP_ITEM, (st))
#define sk_ENGINE_CLEANUP_ITEM_new_null() SKM_sk_new_null(ENGINE_CLEANUP_ITEM)
#define sk_ENGINE_CLEANUP_ITEM_free(st) SKM_sk_free(ENGINE_CLEANUP_ITEM, (st))
#define sk_ENGINE_CLEANUP_ITEM_num(st) SKM_sk_num(ENGINE_CLEANUP_ITEM, (st))
#define sk_ENGINE_CLEANUP_ITEM_value(st, i) \
  SKM_sk_value(ENGINE_CLEANUP_ITEM, (st), (i))
#define sk_ENGINE_CLEANUP_ITEM_set(st, i, val) \
  SKM_sk_set(ENGINE_CLEANUP_ITEM, (st), (i), (val))
#define sk_ENGINE_CLEANUP_ITEM_zero(st) SKM_sk_zero(ENGINE_CLEANUP_ITEM, (st))
#define sk_ENGINE_CLEANUP_ITEM_push(st, val) \
  SKM_sk_push(ENGINE_CLEANUP_ITEM, (st), (val))
#define sk_ENGINE_CLEANUP_ITEM_unshift(st, val) \
  SKM_sk_unshift(ENGINE_CLEANUP_ITEM, (st), (val))
#define sk_ENGINE_CLEANUP_ITEM_find(st, val) \
  SKM_sk_find(ENGINE_CLEANUP_ITEM, (st), (val))
#define sk_ENGINE_CLEANUP_ITEM_find_ex(st, val) \
  SKM_sk_find_ex(ENGINE_CLEANUP_ITEM, (st), (val))
#define sk_ENGINE_CLEANUP_ITEM_delete(st, i) \
  SKM_sk_delete(ENGINE_CLEANUP_ITEM, (st), (i))
#define sk_ENGINE_CLEANUP_ITEM_delete_ptr(st, ptr) \
  SKM_sk_delete_ptr(ENGINE_CLEANUP_ITEM, (st), (ptr))
#define sk_ENGINE_CLEANUP_ITEM_insert(st, val, i) \
  SKM_sk_insert(ENGINE_CLEANUP_ITEM, (st), (val), (i))
#define sk_ENGINE_CLEANUP_ITEM_set_cmp_func(st, cmp) \
  SKM_sk_set_cmp_func(ENGINE_CLEANUP_ITEM, (st), (cmp))
#define sk_ENGINE_CLEANUP_ITEM_dup(st) SKM_sk_dup(ENGINE_CLEANUP_ITEM, st)
#define sk_ENGINE_CLEANUP_ITEM_pop_free(st, free_func) \
  SKM_sk_pop_free(ENGINE_CLEANUP_ITEM, (st), (free_func))
#define sk_ENGINE_CLEANUP_ITEM_shift(st) SKM_sk_shift(ENGINE_CLEANUP_ITEM, (st))
#define sk_ENGINE_CLEANUP_ITEM_pop(st) SKM_sk_pop(ENGINE_CLEANUP_ITEM, (st))
#define sk_ENGINE_CLEANUP_ITEM_sort(st) SKM_sk_sort(ENGINE_CLEANUP_ITEM, (st))
#define sk_ENGINE_CLEANUP_ITEM_is_sorted(st) \
  SKM_sk_is_sorted(ENGINE_CLEANUP_ITEM, (st))

#define sk_GENERAL_NAME_new(st) SKM_sk_new(GENERAL_NAME, (st))
#define sk_GENERAL_NAME_new_null() SKM_sk_new_null(GENERAL_NAME)
#define sk_GENERAL_NAME_free(st) SKM_sk_free(GENERAL_NAME, (st))
#define sk_GENERAL_NAME_num(st) SKM_sk_num(GENERAL_NAME, (st))
#define sk_GENERAL_NAME_value(st, i) SKM_sk_value(GENERAL_NAME, (st), (i))
#define sk_GENERAL_NAME_set(st, i, val) \
  SKM_sk_set(GENERAL_NAME, (st), (i), (val))
#define sk_GENERAL_NAME_zero(st) SKM_sk_zero(GENERAL_NAME, (st))
#define sk_GENERAL_NAME_push(st, val) SKM_sk_push(GENERAL_NAME, (st), (val))
#define sk_GENERAL_NAME_unshift(st, val) \
  SKM_sk_unshift(GENERAL_NAME, (st), (val))
#define sk_GENERAL_NAME_find(st, val) SKM_sk_find(GENERAL_NAME, (st), (val))
#define sk_GENERAL_NAME_find_ex(st, val) \
  SKM_sk_find_ex(GENERAL_NAME, (st), (val))
#define sk_GENERAL_NAME_delete(st, i) SKM_sk_delete(GENERAL_NAME, (st), (i))
#define sk_GENERAL_NAME_delete_ptr(st, ptr) \
  SKM_sk_delete_ptr(GENERAL_NAME, (st), (ptr))
#define sk_GENERAL_NAME_insert(st, val, i) \
  SKM_sk_insert(GENERAL_NAME, (st), (val), (i))
#define sk_GENERAL_NAME_set_cmp_func(st, cmp) \
  SKM_sk_set_cmp_func(GENERAL_NAME, (st), (cmp))
#define sk_GENERAL_NAME_dup(st) SKM_sk_dup(GENERAL_NAME, st)
#define sk_GENERAL_NAME_pop_free(st, free_func) \
  SKM_sk_pop_free(GENERAL_NAME, (st), (free_func))
#define sk_GENERAL_NAME_shift(st) SKM_sk_shift(GENERAL_NAME, (st))
#define sk_GENERAL_NAME_pop(st) SKM_sk_pop(GENERAL_NAME, (st))
#define sk_GENERAL_NAME_sort(st) SKM_sk_sort(GENERAL_NAME, (st))
#define sk_GENERAL_NAME_is_sorted(st) SKM_sk_is_sorted(GENERAL_NAME, (st))

#define sk_GENERAL_SUBTREE_new(st) SKM_sk_new(GENERAL_SUBTREE, (st))
#define sk_GENERAL_SUBTREE_new_null() SKM_sk_new_null(GENERAL_SUBTREE)
#define sk_GENERAL_SUBTREE_free(st) SKM_sk_free(GENERAL_SUBTREE, (st))
#define sk_GENERAL_SUBTREE_num(st) SKM_sk_num(GENERAL_SUBTREE, (st))
#define sk_GENERAL_SUBTREE_value(st, i) SKM_sk_value(GENERAL_SUBTREE, (st), (i))
#define sk_GENERAL_SUBTREE_set(st, i, val) \
  SKM_sk_set(GENERAL_SUBTREE, (st), (i), (val))
#define sk_GENERAL_SUBTREE_zero(st) SKM_sk_zero(GENERAL_SUBTREE, (st))
#define sk_GENERAL_SUBTREE_push(st, val) \
  SKM_sk_push(GENERAL_SUBTREE, (st), (val))
#define sk_GENERAL_SUBTREE_unshift(st, val) \
  SKM_sk_unshift(GENERAL_SUBTREE, (st), (val))
#define sk_GENERAL_SUBTREE_find(st, val) \
  SKM_sk_find(GENERAL_SUBTREE, (st), (val))
#define sk_GENERAL_SUBTREE_find_ex(st, val) \
  SKM_sk_find_ex(GENERAL_SUBTREE, (st), (val))
#define sk_GENERAL_SUBTREE_delete(st, i) \
  SKM_sk_delete(GENERAL_SUBTREE, (st), (i))
#define sk_GENERAL_SUBTREE_delete_ptr(st, ptr) \
  SKM_sk_delete_ptr(GENERAL_SUBTREE, (st), (ptr))
#define sk_GENERAL_SUBTREE_insert(st, val, i) \
  SKM_sk_insert(GENERAL_SUBTREE, (st), (val), (i))
#define sk_GENERAL_SUBTREE_set_cmp_func(st, cmp) \
  SKM_sk_set_cmp_func(GENERAL_SUBTREE, (st), (cmp))
#define sk_GENERAL_SUBTREE_dup(st) SKM_sk_dup(GENERAL_SUBTREE, st)
#define sk_GENERAL_SUBTREE_pop_free(st, free_func) \
  SKM_sk_pop_free(GENERAL_SUBTREE, (st), (free_func))
#define sk_GENERAL_SUBTREE_shift(st) SKM_sk_shift(GENERAL_SUBTREE, (st))
#define sk_GENERAL_SUBTREE_pop(st) SKM_sk_pop(GENERAL_SUBTREE, (st))
#define sk_GENERAL_SUBTREE_sort(st) SKM_sk_sort(GENERAL_SUBTREE, (st))
#define sk_GENERAL_SUBTREE_is_sorted(st) SKM_sk_is_sorted(GENERAL_SUBTREE, (st))

#define sk_IPAddressFamily_new(st) SKM_sk_new(IPAddressFamily, (st))
#define sk_IPAddressFamily_new_null() SKM_sk_new_null(IPAddressFamily)
#define sk_IPAddressFamily_free(st) SKM_sk_free(IPAddressFamily, (st))
#define sk_IPAddressFamily_num(st) SKM_sk_num(IPAddressFamily, (st))
#define sk_IPAddressFamily_value(st, i) SKM_sk_value(IPAddressFamily, (st), (i))
#define sk_IPAddressFamily_set(st, i, val) \
  SKM_sk_set(IPAddressFamily, (st), (i), (val))
#define sk_IPAddressFamily_zero(st) SKM_sk_zero(IPAddressFamily, (st))
#define sk_IPAddressFamily_push(st, val) \
  SKM_sk_push(IPAddressFamily, (st), (val))
#define sk_IPAddressFamily_unshift(st, val) \
  SKM_sk_unshift(IPAddressFamily, (st), (val))
#define sk_IPAddressFamily_find(st, val) \
  SKM_sk_find(IPAddressFamily, (st), (val))
#define sk_IPAddressFamily_find_ex(st, val) \
  SKM_sk_find_ex(IPAddressFamily, (st), (val))
#define sk_IPAddressFamily_delete(st, i) \
  SKM_sk_delete(IPAddressFamily, (st), (i))
#define sk_IPAddressFamily_delete_ptr(st, ptr) \
  SKM_sk_delete_ptr(IPAddressFamily, (st), (ptr))
#define sk_IPAddressFamily_insert(st, val, i) \
  SKM_sk_insert(IPAddressFamily, (st), (val), (i))
#define sk_IPAddressFamily_set_cmp_func(st, cmp) \
  SKM_sk_set_cmp_func(IPAddressFamily, (st), (cmp))
#define sk_IPAddressFamily_dup(st) SKM_sk_dup(IPAddressFamily, st)
#define sk_IPAddressFamily_pop_free(st, free_func) \
  SKM_sk_pop_free(IPAddressFamily, (st), (free_func))
#define sk_IPAddressFamily_shift(st) SKM_sk_shift(IPAddressFamily, (st))
#define sk_IPAddressFamily_pop(st) SKM_sk_pop(IPAddressFamily, (st))
#define sk_IPAddressFamily_sort(st) SKM_sk_sort(IPAddressFamily, (st))
#define sk_IPAddressFamily_is_sorted(st) SKM_sk_is_sorted(IPAddressFamily, (st))

#define sk_IPAddressOrRange_new(st) SKM_sk_new(IPAddressOrRange, (st))
#define sk_IPAddressOrRange_new_null() SKM_sk_new_null(IPAddressOrRange)
#define sk_IPAddressOrRange_free(st) SKM_sk_free(IPAddressOrRange, (st))
#define sk_IPAddressOrRange_num(st) SKM_sk_num(IPAddressOrRange, (st))
#define sk_IPAddressOrRange_value(st, i) \
  SKM_sk_value(IPAddressOrRange, (st), (i))
#define sk_IPAddressOrRange_set(st, i, val) \
  SKM_sk_set(IPAddressOrRange, (st), (i), (val))
#define sk_IPAddressOrRange_zero(st) SKM_sk_zero(IPAddressOrRange, (st))
#define sk_IPAddressOrRange_push(st, val) \
  SKM_sk_push(IPAddressOrRange, (st), (val))
#define sk_IPAddressOrRange_unshift(st, val) \
  SKM_sk_unshift(IPAddressOrRange, (st), (val))
#define sk_IPAddressOrRange_find(st, val) \
  SKM_sk_find(IPAddressOrRange, (st), (val))
#define sk_IPAddressOrRange_find_ex(st, val) \
  SKM_sk_find_ex(IPAddressOrRange, (st), (val))
#define sk_IPAddressOrRange_delete(st, i) \
  SKM_sk_delete(IPAddressOrRange, (st), (i))
#define sk_IPAddressOrRange_delete_ptr(st, ptr) \
  SKM_sk_delete_ptr(IPAddressOrRange, (st), (ptr))
#define sk_IPAddressOrRange_insert(st, val, i) \
  SKM_sk_insert(IPAddressOrRange, (st), (val), (i))
#define sk_IPAddressOrRange_set_cmp_func(st, cmp) \
  SKM_sk_set_cmp_func(IPAddressOrRange, (st), (cmp))
#define sk_IPAddressOrRange_dup(st) SKM_sk_dup(IPAddressOrRange, st)
#define sk_IPAddressOrRange_pop_free(st, free_func) \
  SKM_sk_pop_free(IPAddressOrRange, (st), (free_func))
#define sk_IPAddressOrRange_shift(st) SKM_sk_shift(IPAddressOrRange, (st))
#define sk_IPAddressOrRange_pop(st) SKM_sk_pop(IPAddressOrRange, (st))
#define sk_IPAddressOrRange_sort(st) SKM_sk_sort(IPAddressOrRange, (st))
#define sk_IPAddressOrRange_is_sorted(st) \
  SKM_sk_is_sorted(IPAddressOrRange, (st))

#define sk_KRB5_APREQBODY_new(st) SKM_sk_new(KRB5_APREQBODY, (st))
#define sk_KRB5_APREQBODY_new_null() SKM_sk_new_null(KRB5_APREQBODY)
#define sk_KRB5_APREQBODY_free(st) SKM_sk_free(KRB5_APREQBODY, (st))
#define sk_KRB5_APREQBODY_num(st) SKM_sk_num(KRB5_APREQBODY, (st))
#define sk_KRB5_APREQBODY_value(st, i) SKM_sk_value(KRB5_APREQBODY, (st), (i))
#define sk_KRB5_APREQBODY_set(st, i, val) \
  SKM_sk_set(KRB5_APREQBODY, (st), (i), (val))
#define sk_KRB5_APREQBODY_zero(st) SKM_sk_zero(KRB5_APREQBODY, (st))
#define sk_KRB5_APREQBODY_push(st, val) SKM_sk_push(KRB5_APREQBODY, (st), (val))
#define sk_KRB5_APREQBODY_unshift(st, val) \
  SKM_sk_unshift(KRB5_APREQBODY, (st), (val))
#define sk_KRB5_APREQBODY_find(st, val) SKM_sk_find(KRB5_APREQBODY, (st), (val))
#define sk_KRB5_APREQBODY_find_ex(st, val) \
  SKM_sk_find_ex(KRB5_APREQBODY, (st), (val))
#define sk_KRB5_APREQBODY_delete(st, i) SKM_sk_delete(KRB5_APREQBODY, (st), (i))
#define sk_KRB5_APREQBODY_delete_ptr(st, ptr) \
  SKM_sk_delete_ptr(KRB5_APREQBODY, (st), (ptr))
#define sk_KRB5_APREQBODY_insert(st, val, i) \
  SKM_sk_insert(KRB5_APREQBODY, (st), (val), (i))
#define sk_KRB5_APREQBODY_set_cmp_func(st, cmp) \
  SKM_sk_set_cmp_func(KRB5_APREQBODY, (st), (cmp))
#define sk_KRB5_APREQBODY_dup(st) SKM_sk_dup(KRB5_APREQBODY, st)
#define sk_KRB5_APREQBODY_pop_free(st, free_func) \
  SKM_sk_pop_free(KRB5_APREQBODY, (st), (free_func))
#define sk_KRB5_APREQBODY_shift(st) SKM_sk_shift(KRB5_APREQBODY, (st))
#define sk_KRB5_APREQBODY_pop(st) SKM_sk_pop(KRB5_APREQBODY, (st))
#define sk_KRB5_APREQBODY_sort(st) SKM_sk_sort(KRB5_APREQBODY, (st))
#define sk_KRB5_APREQBODY_is_sorted(st) SKM_sk_is_sorted(KRB5_APREQBODY, (st))

#define sk_KRB5_AUTHDATA_new(st) SKM_sk_new(KRB5_AUTHDATA, (st))
#define sk_KRB5_AUTHDATA_new_null() SKM_sk_new_null(KRB5_AUTHDATA)
#define sk_KRB5_AUTHDATA_free(st) SKM_sk_free(KRB5_AUTHDATA, (st))
#define sk_KRB5_AUTHDATA_num(st) SKM_sk_num(KRB5_AUTHDATA, (st))
#define sk_KRB5_AUTHDATA_value(st, i) SKM_sk_value(KRB5_AUTHDATA, (st), (i))
#define sk_KRB5_AUTHDATA_set(st, i, val) \
  SKM_sk_set(KRB5_AUTHDATA, (st), (i), (val))
#define sk_KRB5_AUTHDATA_zero(st) SKM_sk_zero(KRB5_AUTHDATA, (st))
#define sk_KRB5_AUTHDATA_push(st, val) SKM_sk_push(KRB5_AUTHDATA, (st), (val))
#define sk_KRB5_AUTHDATA_unshift(st, val) \
  SKM_sk_unshift(KRB5_AUTHDATA, (st), (val))
#define sk_KRB5_AUTHDATA_find(st, val) SKM_sk_find(KRB5_AUTHDATA, (st), (val))
#define sk_KRB5_AUTHDATA_find_ex(st, val) \
  SKM_sk_find_ex(KRB5_AUTHDATA, (st), (val))
#define sk_KRB5_AUTHDATA_delete(st, i) SKM_sk_delete(KRB5_AUTHDATA, (st), (i))
#define sk_KRB5_AUTHDATA_delete_ptr(st, ptr) \
  SKM_sk_delete_ptr(KRB5_AUTHDATA, (st), (ptr))
#define sk_KRB5_AUTHDATA_insert(st, val, i) \
  SKM_sk_insert(KRB5_AUTHDATA, (st), (val), (i))
#define sk_KRB5_AUTHDATA_set_cmp_func(st, cmp) \
  SKM_sk_set_cmp_func(KRB5_AUTHDATA, (st), (cmp))
#define sk_KRB5_AUTHDATA_dup(st) SKM_sk_dup(KRB5_AUTHDATA, st)
#define sk_KRB5_AUTHDATA_pop_free(st, free_func) \
  SKM_sk_pop_free(KRB5_AUTHDATA, (st), (free_func))
#define sk_KRB5_AUTHDATA_shift(st) SKM_sk_shift(KRB5_AUTHDATA, (st))
#define sk_KRB5_AUTHDATA_pop(st) SKM_sk_pop(KRB5_AUTHDATA, (st))
#define sk_KRB5_AUTHDATA_sort(st) SKM_sk_sort(KRB5_AUTHDATA, (st))
#define sk_KRB5_AUTHDATA_is_sorted(st) SKM_sk_is_sorted(KRB5_AUTHDATA, (st))

#define sk_KRB5_AUTHENTBODY_new(st) SKM_sk_new(KRB5_AUTHENTBODY, (st))
#define sk_KRB5_AUTHENTBODY_new_null() SKM_sk_new_null(KRB5_AUTHENTBODY)
#define sk_KRB5_AUTHENTBODY_free(st) SKM_sk_free(KRB5_AUTHENTBODY, (st))
#define sk_KRB5_AUTHENTBODY_num(st) SKM_sk_num(KRB5_AUTHENTBODY, (st))
#define sk_KRB5_AUTHENTBODY_value(st, i) \
  SKM_sk_value(KRB5_AUTHENTBODY, (st), (i))
#define sk_KRB5_AUTHENTBODY_set(st, i, val) \
  SKM_sk_set(KRB5_AUTHENTBODY, (st), (i), (val))
#define sk_KRB5_AUTHENTBODY_zero(st) SKM_sk_zero(KRB5_AUTHENTBODY, (st))
#define sk_KRB5_AUTHENTBODY_push(st, val) \
  SKM_sk_push(KRB5_AUTHENTBODY, (st), (val))
#define sk_KRB5_AUTHENTBODY_unshift(st, val) \
  SKM_sk_unshift(KRB5_AUTHENTBODY, (st), (val))
#define sk_KRB5_AUTHENTBODY_find(st, val) \
  SKM_sk_find(KRB5_AUTHENTBODY, (st), (val))
#define sk_KRB5_AUTHENTBODY_find_ex(st, val) \
  SKM_sk_find_ex(KRB5_AUTHENTBODY, (st), (val))
#define sk_KRB5_AUTHENTBODY_delete(st, i) \
  SKM_sk_delete(KRB5_AUTHENTBODY, (st), (i))
#define sk_KRB5_AUTHENTBODY_delete_ptr(st, ptr) \
  SKM_sk_delete_ptr(KRB5_AUTHENTBODY, (st), (ptr))
#define sk_KRB5_AUTHENTBODY_insert(st, val, i) \
  SKM_sk_insert(KRB5_AUTHENTBODY, (st), (val), (i))
#define sk_KRB5_AUTHENTBODY_set_cmp_func(st, cmp) \
  SKM_sk_set_cmp_func(KRB5_AUTHENTBODY, (st), (cmp))
#define sk_KRB5_AUTHENTBODY_dup(st) SKM_sk_dup(KRB5_AUTHENTBODY, st)
#define sk_KRB5_AUTHENTBODY_pop_free(st, free_func) \
  SKM_sk_pop_free(KRB5_AUTHENTBODY, (st), (free_func))
#define sk_KRB5_AUTHENTBODY_shift(st) SKM_sk_shift(KRB5_AUTHENTBODY, (st))
#define sk_KRB5_AUTHENTBODY_pop(st) SKM_sk_pop(KRB5_AUTHENTBODY, (st))
#define sk_KRB5_AUTHENTBODY_sort(st) SKM_sk_sort(KRB5_AUTHENTBODY, (st))
#define sk_KRB5_AUTHENTBODY_is_sorted(st) \
  SKM_sk_is_sorted(KRB5_AUTHENTBODY, (st))

#define sk_KRB5_CHECKSUM_new(st) SKM_sk_new(KRB5_CHECKSUM, (st))
#define sk_KRB5_CHECKSUM_new_null() SKM_sk_new_null(KRB5_CHECKSUM)
#define sk_KRB5_CHECKSUM_free(st) SKM_sk_free(KRB5_CHECKSUM, (st))
#define sk_KRB5_CHECKSUM_num(st) SKM_sk_num(KRB5_CHECKSUM, (st))
#define sk_KRB5_CHECKSUM_value(st, i) SKM_sk_value(KRB5_CHECKSUM, (st), (i))
#define sk_KRB5_CHECKSUM_set(st, i, val) \
  SKM_sk_set(KRB5_CHECKSUM, (st), (i), (val))
#define sk_KRB5_CHECKSUM_zero(st) SKM_sk_zero(KRB5_CHECKSUM, (st))
#define sk_KRB5_CHECKSUM_push(st, val) SKM_sk_push(KRB5_CHECKSUM, (st), (val))
#define sk_KRB5_CHECKSUM_unshift(st, val) \
  SKM_sk_unshift(KRB5_CHECKSUM, (st), (val))
#define sk_KRB5_CHECKSUM_find(st, val) SKM_sk_find(KRB5_CHECKSUM, (st), (val))
#define sk_KRB5_CHECKSUM_find_ex(st, val) \
  SKM_sk_find_ex(KRB5_CHECKSUM, (st), (val))
#define sk_KRB5_CHECKSUM_delete(st, i) SKM_sk_delete(KRB5_CHECKSUM, (st), (i))
#define sk_KRB5_CHECKSUM_delete_ptr(st, ptr) \
  SKM_sk_delete_ptr(KRB5_CHECKSUM, (st), (ptr))
#define sk_KRB5_CHECKSUM_insert(st, val, i) \
  SKM_sk_insert(KRB5_CHECKSUM, (st), (val), (i))
#define sk_KRB5_CHECKSUM_set_cmp_func(st, cmp) \
  SKM_sk_set_cmp_func(KRB5_CHECKSUM, (st), (cmp))
#define sk_KRB5_CHECKSUM_dup(st) SKM_sk_dup(KRB5_CHECKSUM, st)
#define sk_KRB5_CHECKSUM_pop_free(st, free_func) \
  SKM_sk_pop_free(KRB5_CHECKSUM, (st), (free_func))
#define sk_KRB5_CHECKSUM_shift(st) SKM_sk_shift(KRB5_CHECKSUM, (st))
#define sk_KRB5_CHECKSUM_pop(st) SKM_sk_pop(KRB5_CHECKSUM, (st))
#define sk_KRB5_CHECKSUM_sort(st) SKM_sk_sort(KRB5_CHECKSUM, (st))
#define sk_KRB5_CHECKSUM_is_sorted(st) SKM_sk_is_sorted(KRB5_CHECKSUM, (st))

#define sk_KRB5_ENCDATA_new(st) SKM_sk_new(KRB5_ENCDATA, (st))
#define sk_KRB5_ENCDATA_new_null() SKM_sk_new_null(KRB5_ENCDATA)
#define sk_KRB5_ENCDATA_free(st) SKM_sk_free(KRB5_ENCDATA, (st))
#define sk_KRB5_ENCDATA_num(st) SKM_sk_num(KRB5_ENCDATA, (st))
#define sk_KRB5_ENCDATA_value(st, i) SKM_sk_value(KRB5_ENCDATA, (st), (i))
#define sk_KRB5_ENCDATA_set(st, i, val) \
  SKM_sk_set(KRB5_ENCDATA, (st), (i), (val))
#define sk_KRB5_ENCDATA_zero(st) SKM_sk_zero(KRB5_ENCDATA, (st))
#define sk_KRB5_ENCDATA_push(st, val) SKM_sk_push(KRB5_ENCDATA, (st), (val))
#define sk_KRB5_ENCDATA_unshift(st, val) \
  SKM_sk_unshift(KRB5_ENCDATA, (st), (val))
#define sk_KRB5_ENCDATA_find(st, val) SKM_sk_find(KRB5_ENCDATA, (st), (val))
#define sk_KRB5_ENCDATA_find_ex(st, val) \
  SKM_sk_find_ex(KRB5_ENCDATA, (st), (val))
#define sk_KRB5_ENCDATA_delete(st, i) SKM_sk_delete(KRB5_ENCDATA, (st), (i))
#define sk_KRB5_ENCDATA_delete_ptr(st, ptr) \
  SKM_sk_delete_ptr(KRB5_ENCDATA, (st), (ptr))
#define sk_KRB5_ENCDATA_insert(st, val, i) \
  SKM_sk_insert(KRB5_ENCDATA, (st), (val), (i))
#define sk_KRB5_ENCDATA_set_cmp_func(st, cmp) \
  SKM_sk_set_cmp_func(KRB5_ENCDATA, (st), (cmp))
#define sk_KRB5_ENCDATA_dup(st) SKM_sk_dup(KRB5_ENCDATA, st)
#define sk_KRB5_ENCDATA_pop_free(st, free_func) \
  SKM_sk_pop_free(KRB5_ENCDATA, (st), (free_func))
#define sk_KRB5_ENCDATA_shift(st) SKM_sk_shift(KRB5_ENCDATA, (st))
#define sk_KRB5_ENCDATA_pop(st) SKM_sk_pop(KRB5_ENCDATA, (st))
#define sk_KRB5_ENCDATA_sort(st) SKM_sk_sort(KRB5_ENCDATA, (st))
#define sk_KRB5_ENCDATA_is_sorted(st) SKM_sk_is_sorted(KRB5_ENCDATA, (st))

#define sk_KRB5_ENCKEY_new(st) SKM_sk_new(KRB5_ENCKEY, (st))
#define sk_KRB5_ENCKEY_new_null() SKM_sk_new_null(KRB5_ENCKEY)
#define sk_KRB5_ENCKEY_free(st) SKM_sk_free(KRB5_ENCKEY, (st))
#define sk_KRB5_ENCKEY_num(st) SKM_sk_num(KRB5_ENCKEY, (st))
#define sk_KRB5_ENCKEY_value(st, i) SKM_sk_value(KRB5_ENCKEY, (st), (i))
#define sk_KRB5_ENCKEY_set(st, i, val) SKM_sk_set(KRB5_ENCKEY, (st), (i), (val))
#define sk_KRB5_ENCKEY_zero(st) SKM_sk_zero(KRB5_ENCKEY, (st))
#define sk_KRB5_ENCKEY_push(st, val) SKM_sk_push(KRB5_ENCKEY, (st), (val))
#define sk_KRB5_ENCKEY_unshift(st, val) SKM_sk_unshift(KRB5_ENCKEY, (st), (val))
#define sk_KRB5_ENCKEY_find(st, val) SKM_sk_find(KRB5_ENCKEY, (st), (val))
#define sk_KRB5_ENCKEY_find_ex(st, val) SKM_sk_find_ex(KRB5_ENCKEY, (st), (val))
#define sk_KRB5_ENCKEY_delete(st, i) SKM_sk_delete(KRB5_ENCKEY, (st), (i))
#define sk_KRB5_ENCKEY_delete_ptr(st, ptr) \
  SKM_sk_delete_ptr(KRB5_ENCKEY, (st), (ptr))
#define sk_KRB5_ENCKEY_insert(st, val, i) \
  SKM_sk_insert(KRB5_ENCKEY, (st), (val), (i))
#define sk_KRB5_ENCKEY_set_cmp_func(st, cmp) \
  SKM_sk_set_cmp_func(KRB5_ENCKEY, (st), (cmp))
#define sk_KRB5_ENCKEY_dup(st) SKM_sk_dup(KRB5_ENCKEY, st)
#define sk_KRB5_ENCKEY_pop_free(st, free_func) \
  SKM_sk_pop_free(KRB5_ENCKEY, (st), (free_func))
#define sk_KRB5_ENCKEY_shift(st) SKM_sk_shift(KRB5_ENCKEY, (st))
#define sk_KRB5_ENCKEY_pop(st) SKM_sk_pop(KRB5_ENCKEY, (st))
#define sk_KRB5_ENCKEY_sort(st) SKM_sk_sort(KRB5_ENCKEY, (st))
#define sk_KRB5_ENCKEY_is_sorted(st) SKM_sk_is_sorted(KRB5_ENCKEY, (st))

#define sk_KRB5_PRINCNAME_new(st) SKM_sk_new(KRB5_PRINCNAME, (st))
#define sk_KRB5_PRINCNAME_new_null() SKM_sk_new_null(KRB5_PRINCNAME)
#define sk_KRB5_PRINCNAME_free(st) SKM_sk_free(KRB5_PRINCNAME, (st))
#define sk_KRB5_PRINCNAME_num(st) SKM_sk_num(KRB5_PRINCNAME, (st))
#define sk_KRB5_PRINCNAME_value(st, i) SKM_sk_value(KRB5_PRINCNAME, (st), (i))
#define sk_KRB5_PRINCNAME_set(st, i, val) \
  SKM_sk_set(KRB5_PRINCNAME, (st), (i), (val))
#define sk_KRB5_PRINCNAME_zero(st) SKM_sk_zero(KRB5_PRINCNAME, (st))
#define sk_KRB5_PRINCNAME_push(st, val) SKM_sk_push(KRB5_PRINCNAME, (st), (val))
#define sk_KRB5_PRINCNAME_unshift(st, val) \
  SKM_sk_unshift(KRB5_PRINCNAME, (st), (val))
#define sk_KRB5_PRINCNAME_find(st, val) SKM_sk_find(KRB5_PRINCNAME, (st), (val))
#define sk_KRB5_PRINCNAME_find_ex(st, val) \
  SKM_sk_find_ex(KRB5_PRINCNAME, (st), (val))
#define sk_KRB5_PRINCNAME_delete(st, i) SKM_sk_delete(KRB5_PRINCNAME, (st), (i))
#define sk_KRB5_PRINCNAME_delete_ptr(st, ptr) \
  SKM_sk_delete_ptr(KRB5_PRINCNAME, (st), (ptr))
#define sk_KRB5_PRINCNAME_insert(st, val, i) \
  SKM_sk_insert(KRB5_PRINCNAME, (st), (val), (i))
#define sk_KRB5_PRINCNAME_set_cmp_func(st, cmp) \
  SKM_sk_set_cmp_func(KRB5_PRINCNAME, (st), (cmp))
#define sk_KRB5_PRINCNAME_dup(st) SKM_sk_dup(KRB5_PRINCNAME, st)
#define sk_KRB5_PRINCNAME_pop_free(st, free_func) \
  SKM_sk_pop_free(KRB5_PRINCNAME, (st), (free_func))
#define sk_KRB5_PRINCNAME_shift(st) SKM_sk_shift(KRB5_PRINCNAME, (st))
#define sk_KRB5_PRINCNAME_pop(st) SKM_sk_pop(KRB5_PRINCNAME, (st))
#define sk_KRB5_PRINCNAME_sort(st) SKM_sk_sort(KRB5_PRINCNAME, (st))
#define sk_KRB5_PRINCNAME_is_sorted(st) SKM_sk_is_sorted(KRB5_PRINCNAME, (st))

#define sk_KRB5_TKTBODY_new(st) SKM_sk_new(KRB5_TKTBODY, (st))
#define sk_KRB5_TKTBODY_new_null() SKM_sk_new_null(KRB5_TKTBODY)
#define sk_KRB5_TKTBODY_free(st) SKM_sk_free(KRB5_TKTBODY, (st))
#define sk_KRB5_TKTBODY_num(st) SKM_sk_num(KRB5_TKTBODY, (st))
#define sk_KRB5_TKTBODY_value(st, i) SKM_sk_value(KRB5_TKTBODY, (st), (i))
#define sk_KRB5_TKTBODY_set(st, i, val) \
  SKM_sk_set(KRB5_TKTBODY, (st), (i), (val))
#define sk_KRB5_TKTBODY_zero(st) SKM_sk_zero(KRB5_TKTBODY, (st))
#define sk_KRB5_TKTBODY_push(st, val) SKM_sk_push(KRB5_TKTBODY, (st), (val))
#define sk_KRB5_TKTBODY_unshift(st, val) \
  SKM_sk_unshift(KRB5_TKTBODY, (st), (val))
#define sk_KRB5_TKTBODY_find(st, val) SKM_sk_find(KRB5_TKTBODY, (st), (val))
#define sk_KRB5_TKTBODY_find_ex(st, val) \
  SKM_sk_find_ex(KRB5_TKTBODY, (st), (val))
#define sk_KRB5_TKTBODY_delete(st, i) SKM_sk_delete(KRB5_TKTBODY, (st), (i))
#define sk_KRB5_TKTBODY_delete_ptr(st, ptr) \
  SKM_sk_delete_ptr(KRB5_TKTBODY, (st), (ptr))
#define sk_KRB5_TKTBODY_insert(st, val, i) \
  SKM_sk_insert(KRB5_TKTBODY, (st), (val), (i))
#define sk_KRB5_TKTBODY_set_cmp_func(st, cmp) \
  SKM_sk_set_cmp_func(KRB5_TKTBODY, (st), (cmp))
#define sk_KRB5_TKTBODY_dup(st) SKM_sk_dup(KRB5_TKTBODY, st)
#define sk_KRB5_TKTBODY_pop_free(st, free_func) \
  SKM_sk_pop_free(KRB5_TKTBODY, (st), (free_func))
#define sk_KRB5_TKTBODY_shift(st) SKM_sk_shift(KRB5_TKTBODY, (st))
#define sk_KRB5_TKTBODY_pop(st) SKM_sk_pop(KRB5_TKTBODY, (st))
#define sk_KRB5_TKTBODY_sort(st) SKM_sk_sort(KRB5_TKTBODY, (st))
#define sk_KRB5_TKTBODY_is_sorted(st) SKM_sk_is_sorted(KRB5_TKTBODY, (st))

#define sk_MIME_HEADER_new(st) SKM_sk_new(MIME_HEADER, (st))
#define sk_MIME_HEADER_new_null() SKM_sk_new_null(MIME_HEADER)
#define sk_MIME_HEADER_free(st) SKM_sk_free(MIME_HEADER, (st))
#define sk_MIME_HEADER_num(st) SKM_sk_num(MIME_HEADER, (st))
#define sk_MIME_HEADER_value(st, i) SKM_sk_value(MIME_HEADER, (st), (i))
#define sk_MIME_HEADER_set(st, i, val) SKM_sk_set(MIME_HEADER, (st), (i), (val))
#define sk_MIME_HEADER_zero(st) SKM_sk_zero(MIME_HEADER, (st))
#define sk_MIME_HEADER_push(st, val) SKM_sk_push(MIME_HEADER, (st), (val))
#define sk_MIME_HEADER_unshift(st, val) SKM_sk_unshift(MIME_HEADER, (st), (val))
#define sk_MIME_HEADER_find(st, val) SKM_sk_find(MIME_HEADER, (st), (val))
#define sk_MIME_HEADER_find_ex(st, val) SKM_sk_find_ex(MIME_HEADER, (st), (val))
#define sk_MIME_HEADER_delete(st, i) SKM_sk_delete(MIME_HEADER, (st), (i))
#define sk_MIME_HEADER_delete_ptr(st, ptr) \
  SKM_sk_delete_ptr(MIME_HEADER, (st), (ptr))
#define sk_MIME_HEADER_insert(st, val, i) \
  SKM_sk_insert(MIME_HEADER, (st), (val), (i))
#define sk_MIME_HEADER_set_cmp_func(st, cmp) \
  SKM_sk_set_cmp_func(MIME_HEADER, (st), (cmp))
#define sk_MIME_HEADER_dup(st) SKM_sk_dup(MIME_HEADER, st)
#define sk_MIME_HEADER_pop_free(st, free_func) \
  SKM_sk_pop_free(MIME_HEADER, (st), (free_func))
#define sk_MIME_HEADER_shift(st) SKM_sk_shift(MIME_HEADER, (st))
#define sk_MIME_HEADER_pop(st) SKM_sk_pop(MIME_HEADER, (st))
#define sk_MIME_HEADER_sort(st) SKM_sk_sort(MIME_HEADER, (st))
#define sk_MIME_HEADER_is_sorted(st) SKM_sk_is_sorted(MIME_HEADER, (st))

#define sk_MIME_PARAM_new(st) SKM_sk_new(MIME_PARAM, (st))
#define sk_MIME_PARAM_new_null() SKM_sk_new_null(MIME_PARAM)
#define sk_MIME_PARAM_free(st) SKM_sk_free(MIME_PARAM, (st))
#define sk_MIME_PARAM_num(st) SKM_sk_num(MIME_PARAM, (st))
#define sk_MIME_PARAM_value(st, i) SKM_sk_value(MIME_PARAM, (st), (i))
#define sk_MIME_PARAM_set(st, i, val) SKM_sk_set(MIME_PARAM, (st), (i), (val))
#define sk_MIME_PARAM_zero(st) SKM_sk_zero(MIME_PARAM, (st))
#define sk_MIME_PARAM_push(st, val) SKM_sk_push(MIME_PARAM, (st), (val))
#define sk_MIME_PARAM_unshift(st, val) SKM_sk_unshift(MIME_PARAM, (st), (val))
#define sk_MIME_PARAM_find(st, val) SKM_sk_find(MIME_PARAM, (st), (val))
#define sk_MIME_PARAM_find_ex(st, val) SKM_sk_find_ex(MIME_PARAM, (st), (val))
#define sk_MIME_PARAM_delete(st, i) SKM_sk_delete(MIME_PARAM, (st), (i))
#define sk_MIME_PARAM_delete_ptr(st, ptr) \
  SKM_sk_delete_ptr(MIME_PARAM, (st), (ptr))
#define sk_MIME_PARAM_insert(st, val, i) \
  SKM_sk_insert(MIME_PARAM, (st), (val), (i))
#define sk_MIME_PARAM_set_cmp_func(st, cmp) \
  SKM_sk_set_cmp_func(MIME_PARAM, (st), (cmp))
#define sk_MIME_PARAM_dup(st) SKM_sk_dup(MIME_PARAM, st)
#define sk_MIME_PARAM_pop_free(st, free_func) \
  SKM_sk_pop_free(MIME_PARAM, (st), (free_func))
#define sk_MIME_PARAM_shift(st) SKM_sk_shift(MIME_PARAM, (st))
#define sk_MIME_PARAM_pop(st) SKM_sk_pop(MIME_PARAM, (st))
#define sk_MIME_PARAM_sort(st) SKM_sk_sort(MIME_PARAM, (st))
#define sk_MIME_PARAM_is_sorted(st) SKM_sk_is_sorted(MIME_PARAM, (st))

#define sk_NAME_FUNCS_new(st) SKM_sk_new(NAME_FUNCS, (st))
#define sk_NAME_FUNCS_new_null() SKM_sk_new_null(NAME_FUNCS)
#define sk_NAME_FUNCS_free(st) SKM_sk_free(NAME_FUNCS, (st))
#define sk_NAME_FUNCS_num(st) SKM_sk_num(NAME_FUNCS, (st))
#define sk_NAME_FUNCS_value(st, i) SKM_sk_value(NAME_FUNCS, (st), (i))
#define sk_NAME_FUNCS_set(st, i, val) SKM_sk_set(NAME_FUNCS, (st), (i), (val))
#define sk_NAME_FUNCS_zero(st) SKM_sk_zero(NAME_FUNCS, (st))
#define sk_NAME_FUNCS_push(st, val) SKM_sk_push(NAME_FUNCS, (st), (val))
#define sk_NAME_FUNCS_unshift(st, val) SKM_sk_unshift(NAME_FUNCS, (st), (val))
#define sk_NAME_FUNCS_find(st, val) SKM_sk_find(NAME_FUNCS, (st), (val))
#define sk_NAME_FUNCS_find_ex(st, val) SKM_sk_find_ex(NAME_FUNCS, (st), (val))
#define sk_NAME_FUNCS_delete(st, i) SKM_sk_delete(NAME_FUNCS, (st), (i))
#define sk_NAME_FUNCS_delete_ptr(st, ptr) \
  SKM_sk_delete_ptr(NAME_FUNCS, (st), (ptr))
#define sk_NAME_FUNCS_insert(st, val, i) \
  SKM_sk_insert(NAME_FUNCS, (st), (val), (i))
#define sk_NAME_FUNCS_set_cmp_func(st, cmp) \
  SKM_sk_set_cmp_func(NAME_FUNCS, (st), (cmp))
#define sk_NAME_FUNCS_dup(st) SKM_sk_dup(NAME_FUNCS, st)
#define sk_NAME_FUNCS_pop_free(st, free_func) \
  SKM_sk_pop_free(NAME_FUNCS, (st), (free_func))
#define sk_NAME_FUNCS_shift(st) SKM_sk_shift(NAME_FUNCS, (st))
#define sk_NAME_FUNCS_pop(st) SKM_sk_pop(NAME_FUNCS, (st))
#define sk_NAME_FUNCS_sort(st) SKM_sk_sort(NAME_FUNCS, (st))
#define sk_NAME_FUNCS_is_sorted(st) SKM_sk_is_sorted(NAME_FUNCS, (st))

#define sk_OCSP_CERTID_new(st) SKM_sk_new(OCSP_CERTID, (st))
#define sk_OCSP_CERTID_new_null() SKM_sk_new_null(OCSP_CERTID)
#define sk_OCSP_CERTID_free(st) SKM_sk_free(OCSP_CERTID, (st))
#define sk_OCSP_CERTID_num(st) SKM_sk_num(OCSP_CERTID, (st))
#define sk_OCSP_CERTID_value(st, i) SKM_sk_value(OCSP_CERTID, (st), (i))
#define sk_OCSP_CERTID_set(st, i, val) SKM_sk_set(OCSP_CERTID, (st), (i), (val))
#define sk_OCSP_CERTID_zero(st) SKM_sk_zero(OCSP_CERTID, (st))
#define sk_OCSP_CERTID_push(st, val) SKM_sk_push(OCSP_CERTID, (st), (val))
#define sk_OCSP_CERTID_unshift(st, val) SKM_sk_unshift(OCSP_CERTID, (st), (val))
#define sk_OCSP_CERTID_find(st, val) SKM_sk_find(OCSP_CERTID, (st), (val))
#define sk_OCSP_CERTID_find_ex(st, val) SKM_sk_find_ex(OCSP_CERTID, (st), (val))
#define sk_OCSP_CERTID_delete(st, i) SKM_sk_delete(OCSP_CERTID, (st), (i))
#define sk_OCSP_CERTID_delete_ptr(st, ptr) \
  SKM_sk_delete_ptr(OCSP_CERTID, (st), (ptr))
#define sk_OCSP_CERTID_insert(st, val, i) \
  SKM_sk_insert(OCSP_CERTID, (st), (val), (i))
#define sk_OCSP_CERTID_set_cmp_func(st, cmp) \
  SKM_sk_set_cmp_func(OCSP_CERTID, (st), (cmp))
#define sk_OCSP_CERTID_dup(st) SKM_sk_dup(OCSP_CERTID, st)
#define sk_OCSP_CERTID_pop_free(st, free_func) \
  SKM_sk_pop_free(OCSP_CERTID, (st), (free_func))
#define sk_OCSP_CERTID_shift(st) SKM_sk_shift(OCSP_CERTID, (st))
#define sk_OCSP_CERTID_pop(st) SKM_sk_pop(OCSP_CERTID, (st))
#define sk_OCSP_CERTID_sort(st) SKM_sk_sort(OCSP_CERTID, (st))
#define sk_OCSP_CERTID_is_sorted(st) SKM_sk_is_sorted(OCSP_CERTID, (st))

#define sk_OCSP_ONEREQ_new(st) SKM_sk_new(OCSP_ONEREQ, (st))
#define sk_OCSP_ONEREQ_new_null() SKM_sk_new_null(OCSP_ONEREQ)
#define sk_OCSP_ONEREQ_free(st) SKM_sk_free(OCSP_ONEREQ, (st))
#define sk_OCSP_ONEREQ_num(st) SKM_sk_num(OCSP_ONEREQ, (st))
#define sk_OCSP_ONEREQ_value(st, i) SKM_sk_value(OCSP_ONEREQ, (st), (i))
#define sk_OCSP_ONEREQ_set(st, i, val) SKM_sk_set(OCSP_ONEREQ, (st), (i), (val))
#define sk_OCSP_ONEREQ_zero(st) SKM_sk_zero(OCSP_ONEREQ, (st))
#define sk_OCSP_ONEREQ_push(st, val) SKM_sk_push(OCSP_ONEREQ, (st), (val))
#define sk_OCSP_ONEREQ_unshift(st, val) SKM_sk_unshift(OCSP_ONEREQ, (st), (val))
#define sk_OCSP_ONEREQ_find(st, val) SKM_sk_find(OCSP_ONEREQ, (st), (val))
#define sk_OCSP_ONEREQ_find_ex(st, val) SKM_sk_find_ex(OCSP_ONEREQ, (st), (val))
#define sk_OCSP_ONEREQ_delete(st, i) SKM_sk_delete(OCSP_ONEREQ, (st), (i))
#define sk_OCSP_ONEREQ_delete_ptr(st, ptr) \
  SKM_sk_delete_ptr(OCSP_ONEREQ, (st), (ptr))
#define sk_OCSP_ONEREQ_insert(st, val, i) \
  SKM_sk_insert(OCSP_ONEREQ, (st), (val), (i))
#define sk_OCSP_ONEREQ_set_cmp_func(st, cmp) \
  SKM_sk_set_cmp_func(OCSP_ONEREQ, (st), (cmp))
#define sk_OCSP_ONEREQ_dup(st) SKM_sk_dup(OCSP_ONEREQ, st)
#define sk_OCSP_ONEREQ_pop_free(st, free_func) \
  SKM_sk_pop_free(OCSP_ONEREQ, (st), (free_func))
#define sk_OCSP_ONEREQ_shift(st) SKM_sk_shift(OCSP_ONEREQ, (st))
#define sk_OCSP_ONEREQ_pop(st) SKM_sk_pop(OCSP_ONEREQ, (st))
#define sk_OCSP_ONEREQ_sort(st) SKM_sk_sort(OCSP_ONEREQ, (st))
#define sk_OCSP_ONEREQ_is_sorted(st) SKM_sk_is_sorted(OCSP_ONEREQ, (st))

#define sk_OCSP_SINGLERESP_new(st) SKM_sk_new(OCSP_SINGLERESP, (st))
#define sk_OCSP_SINGLERESP_new_null() SKM_sk_new_null(OCSP_SINGLERESP)
#define sk_OCSP_SINGLERESP_free(st) SKM_sk_free(OCSP_SINGLERESP, (st))
#define sk_OCSP_SINGLERESP_num(st) SKM_sk_num(OCSP_SINGLERESP, (st))
#define sk_OCSP_SINGLERESP_value(st, i) SKM_sk_value(OCSP_SINGLERESP, (st), (i))
#define sk_OCSP_SINGLERESP_set(st, i, val) \
  SKM_sk_set(OCSP_SINGLERESP, (st), (i), (val))
#define sk_OCSP_SINGLERESP_zero(st) SKM_sk_zero(OCSP_SINGLERESP, (st))
#define sk_OCSP_SINGLERESP_push(st, val) \
  SKM_sk_push(OCSP_SINGLERESP, (st), (val))
#define sk_OCSP_SINGLERESP_unshift(st, val) \
  SKM_sk_unshift(OCSP_SINGLERESP, (st), (val))
#define sk_OCSP_SINGLERESP_find(st, val) \
  SKM_sk_find(OCSP_SINGLERESP, (st), (val))
#define sk_OCSP_SINGLERESP_find_ex(st, val) \
  SKM_sk_find_ex(OCSP_SINGLERESP, (st), (val))
#define sk_OCSP_SINGLERESP_delete(st, i) \
  SKM_sk_delete(OCSP_SINGLERESP, (st), (i))
#define sk_OCSP_SINGLERESP_delete_ptr(st, ptr) \
  SKM_sk_delete_ptr(OCSP_SINGLERESP, (st), (ptr))
#define sk_OCSP_SINGLERESP_insert(st, val, i) \
  SKM_sk_insert(OCSP_SINGLERESP, (st), (val), (i))
#define sk_OCSP_SINGLERESP_set_cmp_func(st, cmp) \
  SKM_sk_set_cmp_func(OCSP_SINGLERESP, (st), (cmp))
#define sk_OCSP_SINGLERESP_dup(st) SKM_sk_dup(OCSP_SINGLERESP, st)
#define sk_OCSP_SINGLERESP_pop_free(st, free_func) \
  SKM_sk_pop_free(OCSP_SINGLERESP, (st), (free_func))
#define sk_OCSP_SINGLERESP_shift(st) SKM_sk_shift(OCSP_SINGLERESP, (st))
#define sk_OCSP_SINGLERESP_pop(st) SKM_sk_pop(OCSP_SINGLERESP, (st))
#define sk_OCSP_SINGLERESP_sort(st) SKM_sk_sort(OCSP_SINGLERESP, (st))
#define sk_OCSP_SINGLERESP_is_sorted(st) SKM_sk_is_sorted(OCSP_SINGLERESP, (st))

#define sk_PKCS12_SAFEBAG_new(st) SKM_sk_new(PKCS12_SAFEBAG, (st))
#define sk_PKCS12_SAFEBAG_new_null() SKM_sk_new_null(PKCS12_SAFEBAG)
#define sk_PKCS12_SAFEBAG_free(st) SKM_sk_free(PKCS12_SAFEBAG, (st))
#define sk_PKCS12_SAFEBAG_num(st) SKM_sk_num(PKCS12_SAFEBAG, (st))
#define sk_PKCS12_SAFEBAG_value(st, i) SKM_sk_value(PKCS12_SAFEBAG, (st), (i))
#define sk_PKCS12_SAFEBAG_set(st, i, val) \
  SKM_sk_set(PKCS12_SAFEBAG, (st), (i), (val))
#define sk_PKCS12_SAFEBAG_zero(st) SKM_sk_zero(PKCS12_SAFEBAG, (st))
#define sk_PKCS12_SAFEBAG_push(st, val) SKM_sk_push(PKCS12_SAFEBAG, (st), (val))
#define sk_PKCS12_SAFEBAG_unshift(st, val) \
  SKM_sk_unshift(PKCS12_SAFEBAG, (st), (val))
#define sk_PKCS12_SAFEBAG_find(st, val) SKM_sk_find(PKCS12_SAFEBAG, (st), (val))
#define sk_PKCS12_SAFEBAG_find_ex(st, val) \
  SKM_sk_find_ex(PKCS12_SAFEBAG, (st), (val))
#define sk_PKCS12_SAFEBAG_delete(st, i) SKM_sk_delete(PKCS12_SAFEBAG, (st), (i))
#define sk_PKCS12_SAFEBAG_delete_ptr(st, ptr) \
  SKM_sk_delete_ptr(PKCS12_SAFEBAG, (st), (ptr))
#define sk_PKCS12_SAFEBAG_insert(st, val, i) \
  SKM_sk_insert(PKCS12_SAFEBAG, (st), (val), (i))
#define sk_PKCS12_SAFEBAG_set_cmp_func(st, cmp) \
  SKM_sk_set_cmp_func(PKCS12_SAFEBAG, (st), (cmp))
#define sk_PKCS12_SAFEBAG_dup(st) SKM_sk_dup(PKCS12_SAFEBAG, st)
#define sk_PKCS12_SAFEBAG_pop_free(st, free_func) \
  SKM_sk_pop_free(PKCS12_SAFEBAG, (st), (free_func))
#define sk_PKCS12_SAFEBAG_shift(st) SKM_sk_shift(PKCS12_SAFEBAG, (st))
#define sk_PKCS12_SAFEBAG_pop(st) SKM_sk_pop(PKCS12_SAFEBAG, (st))
#define sk_PKCS12_SAFEBAG_sort(st) SKM_sk_sort(PKCS12_SAFEBAG, (st))
#define sk_PKCS12_SAFEBAG_is_sorted(st) SKM_sk_is_sorted(PKCS12_SAFEBAG, (st))

#define sk_PKCS7_new(st) SKM_sk_new(PKCS7, (st))
#define sk_PKCS7_new_null() SKM_sk_new_null(PKCS7)
#define sk_PKCS7_free(st) SKM_sk_free(PKCS7, (st))
#define sk_PKCS7_num(st) SKM_sk_num(PKCS7, (st))
#define sk_PKCS7_value(st, i) SKM_sk_value(PKCS7, (st), (i))
#define sk_PKCS7_set(st, i, val) SKM_sk_set(PKCS7, (st), (i), (val))
#define sk_PKCS7_zero(st) SKM_sk_zero(PKCS7, (st))
#define sk_PKCS7_push(st, val) SKM_sk_push(PKCS7, (st), (val))
#define sk_PKCS7_unshift(st, val) SKM_sk_unshift(PKCS7, (st), (val))
#define sk_PKCS7_find(st, val) SKM_sk_find(PKCS7, (st), (val))
#define sk_PKCS7_find_ex(st, val) SKM_sk_find_ex(PKCS7, (st), (val))
#define sk_PKCS7_delete(st, i) SKM_sk_delete(PKCS7, (st), (i))
#define sk_PKCS7_delete_ptr(st, ptr) SKM_sk_delete_ptr(PKCS7, (st), (ptr))
#define sk_PKCS7_insert(st, val, i) SKM_sk_insert(PKCS7, (st), (val), (i))
#define sk_PKCS7_set_cmp_func(st, cmp) SKM_sk_set_cmp_func(PKCS7, (st), (cmp))
#define sk_PKCS7_dup(st) SKM_sk_dup(PKCS7, st)
#define sk_PKCS7_pop_free(st, free_func) \
  SKM_sk_pop_free(PKCS7, (st), (free_func))
#define sk_PKCS7_shift(st) SKM_sk_shift(PKCS7, (st))
#define sk_PKCS7_pop(st) SKM_sk_pop(PKCS7, (st))
#define sk_PKCS7_sort(st) SKM_sk_sort(PKCS7, (st))
#define sk_PKCS7_is_sorted(st) SKM_sk_is_sorted(PKCS7, (st))

#define sk_PKCS7_RECIP_INFO_new(st) SKM_sk_new(PKCS7_RECIP_INFO, (st))
#define sk_PKCS7_RECIP_INFO_new_null() SKM_sk_new_null(PKCS7_RECIP_INFO)
#define sk_PKCS7_RECIP_INFO_free(st) SKM_sk_free(PKCS7_RECIP_INFO, (st))
#define sk_PKCS7_RECIP_INFO_num(st) SKM_sk_num(PKCS7_RECIP_INFO, (st))
#define sk_PKCS7_RECIP_INFO_value(st, i) \
  SKM_sk_value(PKCS7_RECIP_INFO, (st), (i))
#define sk_PKCS7_RECIP_INFO_set(st, i, val) \
  SKM_sk_set(PKCS7_RECIP_INFO, (st), (i), (val))
#define sk_PKCS7_RECIP_INFO_zero(st) SKM_sk_zero(PKCS7_RECIP_INFO, (st))
#define sk_PKCS7_RECIP_INFO_push(st, val) \
  SKM_sk_push(PKCS7_RECIP_INFO, (st), (val))
#define sk_PKCS7_RECIP_INFO_unshift(st, val) \
  SKM_sk_unshift(PKCS7_RECIP_INFO, (st), (val))
#define sk_PKCS7_RECIP_INFO_find(st, val) \
  SKM_sk_find(PKCS7_RECIP_INFO, (st), (val))
#define sk_PKCS7_RECIP_INFO_find_ex(st, val) \
  SKM_sk_find_ex(PKCS7_RECIP_INFO, (st), (val))
#define sk_PKCS7_RECIP_INFO_delete(st, i) \
  SKM_sk_delete(PKCS7_RECIP_INFO, (st), (i))
#define sk_PKCS7_RECIP_INFO_delete_ptr(st, ptr) \
  SKM_sk_delete_ptr(PKCS7_RECIP_INFO, (st), (ptr))
#define sk_PKCS7_RECIP_INFO_insert(st, val, i) \
  SKM_sk_insert(PKCS7_RECIP_INFO, (st), (val), (i))
#define sk_PKCS7_RECIP_INFO_set_cmp_func(st, cmp) \
  SKM_sk_set_cmp_func(PKCS7_RECIP_INFO, (st), (cmp))
#define sk_PKCS7_RECIP_INFO_dup(st) SKM_sk_dup(PKCS7_RECIP_INFO, st)
#define sk_PKCS7_RECIP_INFO_pop_free(st, free_func) \
  SKM_sk_pop_free(PKCS7_RECIP_INFO, (st), (free_func))
#define sk_PKCS7_RECIP_INFO_shift(st) SKM_sk_shift(PKCS7_RECIP_INFO, (st))
#define sk_PKCS7_RECIP_INFO_pop(st) SKM_sk_pop(PKCS7_RECIP_INFO, (st))
#define sk_PKCS7_RECIP_INFO_sort(st) SKM_sk_sort(PKCS7_RECIP_INFO, (st))
#define sk_PKCS7_RECIP_INFO_is_sorted(st) \
  SKM_sk_is_sorted(PKCS7_RECIP_INFO, (st))

#define sk_PKCS7_SIGNER_INFO_new(st) SKM_sk_new(PKCS7_SIGNER_INFO, (st))
#define sk_PKCS7_SIGNER_INFO_new_null() SKM_sk_new_null(PKCS7_SIGNER_INFO)
#define sk_PKCS7_SIGNER_INFO_free(st) SKM_sk_free(PKCS7_SIGNER_INFO, (st))
#define sk_PKCS7_SIGNER_INFO_num(st) SKM_sk_num(PKCS7_SIGNER_INFO, (st))
#define sk_PKCS7_SIGNER_INFO_value(st, i) \
  SKM_sk_value(PKCS7_SIGNER_INFO, (st), (i))
#define sk_PKCS7_SIGNER_INFO_set(st, i, val) \
  SKM_sk_set(PKCS7_SIGNER_INFO, (st), (i), (val))
#define sk_PKCS7_SIGNER_INFO_zero(st) SKM_sk_zero(PKCS7_SIGNER_INFO, (st))
#define sk_PKCS7_SIGNER_INFO_push(st, val) \
  SKM_sk_push(PKCS7_SIGNER_INFO, (st), (val))
#define sk_PKCS7_SIGNER_INFO_unshift(st, val) \
  SKM_sk_unshift(PKCS7_SIGNER_INFO, (st), (val))
#define sk_PKCS7_SIGNER_INFO_find(st, val) \
  SKM_sk_find(PKCS7_SIGNER_INFO, (st), (val))
#define sk_PKCS7_SIGNER_INFO_find_ex(st, val) \
  SKM_sk_find_ex(PKCS7_SIGNER_INFO, (st), (val))
#define sk_PKCS7_SIGNER_INFO_delete(st, i) \
  SKM_sk_delete(PKCS7_SIGNER_INFO, (st), (i))
#define sk_PKCS7_SIGNER_INFO_delete_ptr(st, ptr) \
  SKM_sk_delete_ptr(PKCS7_SIGNER_INFO, (st), (ptr))
#define sk_PKCS7_SIGNER_INFO_insert(st, val, i) \
  SKM_sk_insert(PKCS7_SIGNER_INFO, (st), (val), (i))
#define sk_PKCS7_SIGNER_INFO_set_cmp_func(st, cmp) \
  SKM_sk_set_cmp_func(PKCS7_SIGNER_INFO, (st), (cmp))
#define sk_PKCS7_SIGNER_INFO_dup(st) SKM_sk_dup(PKCS7_SIGNER_INFO, st)
#define sk_PKCS7_SIGNER_INFO_pop_free(st, free_func) \
  SKM_sk_pop_free(PKCS7_SIGNER_INFO, (st), (free_func))
#define sk_PKCS7_SIGNER_INFO_shift(st) SKM_sk_shift(PKCS7_SIGNER_INFO, (st))
#define sk_PKCS7_SIGNER_INFO_pop(st) SKM_sk_pop(PKCS7_SIGNER_INFO, (st))
#define sk_PKCS7_SIGNER_INFO_sort(st) SKM_sk_sort(PKCS7_SIGNER_INFO, (st))
#define sk_PKCS7_SIGNER_INFO_is_sorted(st) \
  SKM_sk_is_sorted(PKCS7_SIGNER_INFO, (st))

#define sk_POLICYINFO_new(st) SKM_sk_new(POLICYINFO, (st))
#define sk_POLICYINFO_new_null() SKM_sk_new_null(POLICYINFO)
#define sk_POLICYINFO_free(st) SKM_sk_free(POLICYINFO, (st))
#define sk_POLICYINFO_num(st) SKM_sk_num(POLICYINFO, (st))
#define sk_POLICYINFO_value(st, i) SKM_sk_value(POLICYINFO, (st), (i))
#define sk_POLICYINFO_set(st, i, val) SKM_sk_set(POLICYINFO, (st), (i), (val))
#define sk_POLICYINFO_zero(st) SKM_sk_zero(POLICYINFO, (st))
#define sk_POLICYINFO_push(st, val) SKM_sk_push(POLICYINFO, (st), (val))
#define sk_POLICYINFO_unshift(st, val) SKM_sk_unshift(POLICYINFO, (st), (val))
#define sk_POLICYINFO_find(st, val) SKM_sk_find(POLICYINFO, (st), (val))
#define sk_POLICYINFO_find_ex(st, val) SKM_sk_find_ex(POLICYINFO, (st), (val))
#define sk_POLICYINFO_delete(st, i) SKM_sk_delete(POLICYINFO, (st), (i))
#define sk_POLICYINFO_delete_ptr(st, ptr) \
  SKM_sk_delete_ptr(POLICYINFO, (st), (ptr))
#define sk_POLICYINFO_insert(st, val, i) \
  SKM_sk_insert(POLICYINFO, (st), (val), (i))
#define sk_POLICYINFO_set_cmp_func(st, cmp) \
  SKM_sk_set_cmp_func(POLICYINFO, (st), (cmp))
#define sk_POLICYINFO_dup(st) SKM_sk_dup(POLICYINFO, st)
#define sk_POLICYINFO_pop_free(st, free_func) \
  SKM_sk_pop_free(POLICYINFO, (st), (free_func))
#define sk_POLICYINFO_shift(st) SKM_sk_shift(POLICYINFO, (st))
#define sk_POLICYINFO_pop(st) SKM_sk_pop(POLICYINFO, (st))
#define sk_POLICYINFO_sort(st) SKM_sk_sort(POLICYINFO, (st))
#define sk_POLICYINFO_is_sorted(st) SKM_sk_is_sorted(POLICYINFO, (st))

#define sk_POLICYQUALINFO_new(st) SKM_sk_new(POLICYQUALINFO, (st))
#define sk_POLICYQUALINFO_new_null() SKM_sk_new_null(POLICYQUALINFO)
#define sk_POLICYQUALINFO_free(st) SKM_sk_free(POLICYQUALINFO, (st))
#define sk_POLICYQUALINFO_num(st) SKM_sk_num(POLICYQUALINFO, (st))
#define sk_POLICYQUALINFO_value(st, i) SKM_sk_value(POLICYQUALINFO, (st), (i))
#define sk_POLICYQUALINFO_set(st, i, val) \
  SKM_sk_set(POLICYQUALINFO, (st), (i), (val))
#define sk_POLICYQUALINFO_zero(st) SKM_sk_zero(POLICYQUALINFO, (st))
#define sk_POLICYQUALINFO_push(st, val) SKM_sk_push(POLICYQUALINFO, (st), (val))
#define sk_POLICYQUALINFO_unshift(st, val) \
  SKM_sk_unshift(POLICYQUALINFO, (st), (val))
#define sk_POLICYQUALINFO_find(st, val) SKM_sk_find(POLICYQUALINFO, (st), (val))
#define sk_POLICYQUALINFO_find_ex(st, val) \
  SKM_sk_find_ex(POLICYQUALINFO, (st), (val))
#define sk_POLICYQUALINFO_delete(st, i) SKM_sk_delete(POLICYQUALINFO, (st), (i))
#define sk_POLICYQUALINFO_delete_ptr(st, ptr) \
  SKM_sk_delete_ptr(POLICYQUALINFO, (st), (ptr))
#define sk_POLICYQUALINFO_insert(st, val, i) \
  SKM_sk_insert(POLICYQUALINFO, (st), (val), (i))
#define sk_POLICYQUALINFO_set_cmp_func(st, cmp) \
  SKM_sk_set_cmp_func(POLICYQUALINFO, (st), (cmp))
#define sk_POLICYQUALINFO_dup(st) SKM_sk_dup(POLICYQUALINFO, st)
#define sk_POLICYQUALINFO_pop_free(st, free_func) \
  SKM_sk_pop_free(POLICYQUALINFO, (st), (free_func))
#define sk_POLICYQUALINFO_shift(st) SKM_sk_shift(POLICYQUALINFO, (st))
#define sk_POLICYQUALINFO_pop(st) SKM_sk_pop(POLICYQUALINFO, (st))
#define sk_POLICYQUALINFO_sort(st) SKM_sk_sort(POLICYQUALINFO, (st))
#define sk_POLICYQUALINFO_is_sorted(st) SKM_sk_is_sorted(POLICYQUALINFO, (st))

#define sk_POLICY_MAPPING_new(st) SKM_sk_new(POLICY_MAPPING, (st))
#define sk_POLICY_MAPPING_new_null() SKM_sk_new_null(POLICY_MAPPING)
#define sk_POLICY_MAPPING_free(st) SKM_sk_free(POLICY_MAPPING, (st))
#define sk_POLICY_MAPPING_num(st) SKM_sk_num(POLICY_MAPPING, (st))
#define sk_POLICY_MAPPING_value(st, i) SKM_sk_value(POLICY_MAPPING, (st), (i))
#define sk_POLICY_MAPPING_set(st, i, val) \
  SKM_sk_set(POLICY_MAPPING, (st), (i), (val))
#define sk_POLICY_MAPPING_zero(st) SKM_sk_zero(POLICY_MAPPING, (st))
#define sk_POLICY_MAPPING_push(st, val) SKM_sk_push(POLICY_MAPPING, (st), (val))
#define sk_POLICY_MAPPING_unshift(st, val) \
  SKM_sk_unshift(POLICY_MAPPING, (st), (val))
#define sk_POLICY_MAPPING_find(st, val) SKM_sk_find(POLICY_MAPPING, (st), (val))
#define sk_POLICY_MAPPING_find_ex(st, val) \
  SKM_sk_find_ex(POLICY_MAPPING, (st), (val))
#define sk_POLICY_MAPPING_delete(st, i) SKM_sk_delete(POLICY_MAPPING, (st), (i))
#define sk_POLICY_MAPPING_delete_ptr(st, ptr) \
  SKM_sk_delete_ptr(POLICY_MAPPING, (st), (ptr))
#define sk_POLICY_MAPPING_insert(st, val, i) \
  SKM_sk_insert(POLICY_MAPPING, (st), (val), (i))
#define sk_POLICY_MAPPING_set_cmp_func(st, cmp) \
  SKM_sk_set_cmp_func(POLICY_MAPPING, (st), (cmp))
#define sk_POLICY_MAPPING_dup(st) SKM_sk_dup(POLICY_MAPPING, st)
#define sk_POLICY_MAPPING_pop_free(st, free_func) \
  SKM_sk_pop_free(POLICY_MAPPING, (st), (free_func))
#define sk_POLICY_MAPPING_shift(st) SKM_sk_shift(POLICY_MAPPING, (st))
#define sk_POLICY_MAPPING_pop(st) SKM_sk_pop(POLICY_MAPPING, (st))
#define sk_POLICY_MAPPING_sort(st) SKM_sk_sort(POLICY_MAPPING, (st))
#define sk_POLICY_MAPPING_is_sorted(st) SKM_sk_is_sorted(POLICY_MAPPING, (st))

#define sk_SSL_CIPHER_new(st) SKM_sk_new(SSL_CIPHER, (st))
#define sk_SSL_CIPHER_new_null() SKM_sk_new_null(SSL_CIPHER)
#define sk_SSL_CIPHER_free(st) SKM_sk_free(SSL_CIPHER, (st))
#define sk_SSL_CIPHER_num(st) SKM_sk_num(SSL_CIPHER, (st))
#define sk_SSL_CIPHER_value(st, i) SKM_sk_value(SSL_CIPHER, (st), (i))
#define sk_SSL_CIPHER_set(st, i, val) SKM_sk_set(SSL_CIPHER, (st), (i), (val))
#define sk_SSL_CIPHER_zero(st) SKM_sk_zero(SSL_CIPHER, (st))
#define sk_SSL_CIPHER_push(st, val) SKM_sk_push(SSL_CIPHER, (st), (val))
#define sk_SSL_CIPHER_unshift(st, val) SKM_sk_unshift(SSL_CIPHER, (st), (val))
#define sk_SSL_CIPHER_find(st, val) SKM_sk_find(SSL_CIPHER, (st), (val))
#define sk_SSL_CIPHER_find_ex(st, val) SKM_sk_find_ex(SSL_CIPHER, (st), (val))
#define sk_SSL_CIPHER_delete(st, i) SKM_sk_delete(SSL_CIPHER, (st), (i))
#define sk_SSL_CIPHER_delete_ptr(st, ptr) \
  SKM_sk_delete_ptr(SSL_CIPHER, (st), (ptr))
#define sk_SSL_CIPHER_insert(st, val, i) \
  SKM_sk_insert(SSL_CIPHER, (st), (val), (i))
#define sk_SSL_CIPHER_set_cmp_func(st, cmp) \
  SKM_sk_set_cmp_func(SSL_CIPHER, (st), (cmp))
#define sk_SSL_CIPHER_dup(st) SKM_sk_dup(SSL_CIPHER, st)
#define sk_SSL_CIPHER_pop_free(st, free_func) \
  SKM_sk_pop_free(SSL_CIPHER, (st), (free_func))
#define sk_SSL_CIPHER_shift(st) SKM_sk_shift(SSL_CIPHER, (st))
#define sk_SSL_CIPHER_pop(st) SKM_sk_pop(SSL_CIPHER, (st))
#define sk_SSL_CIPHER_sort(st) SKM_sk_sort(SSL_CIPHER, (st))
#define sk_SSL_CIPHER_is_sorted(st) SKM_sk_is_sorted(SSL_CIPHER, (st))

#define sk_SSL_COMP_new(st) SKM_sk_new(SSL_COMP, (st))
#define sk_SSL_COMP_new_null() SKM_sk_new_null(SSL_COMP)
#define sk_SSL_COMP_free(st) SKM_sk_free(SSL_COMP, (st))
#define sk_SSL_COMP_num(st) SKM_sk_num(SSL_COMP, (st))
#define sk_SSL_COMP_value(st, i) SKM_sk_value(SSL_COMP, (st), (i))
#define sk_SSL_COMP_set(st, i, val) SKM_sk_set(SSL_COMP, (st), (i), (val))
#define sk_SSL_COMP_zero(st) SKM_sk_zero(SSL_COMP, (st))
#define sk_SSL_COMP_push(st, val) SKM_sk_push(SSL_COMP, (st), (val))
#define sk_SSL_COMP_unshift(st, val) SKM_sk_unshift(SSL_COMP, (st), (val))
#define sk_SSL_COMP_find(st, val) SKM_sk_find(SSL_COMP, (st), (val))
#define sk_SSL_COMP_find_ex(st, val) SKM_sk_find_ex(SSL_COMP, (st), (val))
#define sk_SSL_COMP_delete(st, i) SKM_sk_delete(SSL_COMP, (st), (i))
#define sk_SSL_COMP_delete_ptr(st, ptr) SKM_sk_delete_ptr(SSL_COMP, (st), (ptr))
#define sk_SSL_COMP_insert(st, val, i) SKM_sk_insert(SSL_COMP, (st), (val), (i))
#define sk_SSL_COMP_set_cmp_func(st, cmp) \
  SKM_sk_set_cmp_func(SSL_COMP, (st), (cmp))
#define sk_SSL_COMP_dup(st) SKM_sk_dup(SSL_COMP, st)
#define sk_SSL_COMP_pop_free(st, free_func) \
  SKM_sk_pop_free(SSL_COMP, (st), (free_func))
#define sk_SSL_COMP_shift(st) SKM_sk_shift(SSL_COMP, (st))
#define sk_SSL_COMP_pop(st) SKM_sk_pop(SSL_COMP, (st))
#define sk_SSL_COMP_sort(st) SKM_sk_sort(SSL_COMP, (st))
#define sk_SSL_COMP_is_sorted(st) SKM_sk_is_sorted(SSL_COMP, (st))

#define sk_STORE_OBJECT_new(st) SKM_sk_new(STORE_OBJECT, (st))
#define sk_STORE_OBJECT_new_null() SKM_sk_new_null(STORE_OBJECT)
#define sk_STORE_OBJECT_free(st) SKM_sk_free(STORE_OBJECT, (st))
#define sk_STORE_OBJECT_num(st) SKM_sk_num(STORE_OBJECT, (st))
#define sk_STORE_OBJECT_value(st, i) SKM_sk_value(STORE_OBJECT, (st), (i))
#define sk_STORE_OBJECT_set(st, i, val) \
  SKM_sk_set(STORE_OBJECT, (st), (i), (val))
#define sk_STORE_OBJECT_zero(st) SKM_sk_zero(STORE_OBJECT, (st))
#define sk_STORE_OBJECT_push(st, val) SKM_sk_push(STORE_OBJECT, (st), (val))
#define sk_STORE_OBJECT_unshift(st, val) \
  SKM_sk_unshift(STORE_OBJECT, (st), (val))
#define sk_STORE_OBJECT_find(st, val) SKM_sk_find(STORE_OBJECT, (st), (val))
#define sk_STORE_OBJECT_find_ex(st, val) \
  SKM_sk_find_ex(STORE_OBJECT, (st), (val))
#define sk_STORE_OBJECT_delete(st, i) SKM_sk_delete(STORE_OBJECT, (st), (i))
#define sk_STORE_OBJECT_delete_ptr(st, ptr) \
  SKM_sk_delete_ptr(STORE_OBJECT, (st), (ptr))
#define sk_STORE_OBJECT_insert(st, val, i) \
  SKM_sk_insert(STORE_OBJECT, (st), (val), (i))
#define sk_STORE_OBJECT_set_cmp_func(st, cmp) \
  SKM_sk_set_cmp_func(STORE_OBJECT, (st), (cmp))
#define sk_STORE_OBJECT_dup(st) SKM_sk_dup(STORE_OBJECT, st)
#define sk_STORE_OBJECT_pop_free(st, free_func) \
  SKM_sk_pop_free(STORE_OBJECT, (st), (free_func))
#define sk_STORE_OBJECT_shift(st) SKM_sk_shift(STORE_OBJECT, (st))
#define sk_STORE_OBJECT_pop(st) SKM_sk_pop(STORE_OBJECT, (st))
#define sk_STORE_OBJECT_sort(st) SKM_sk_sort(STORE_OBJECT, (st))
#define sk_STORE_OBJECT_is_sorted(st) SKM_sk_is_sorted(STORE_OBJECT, (st))

#define sk_SXNETID_new(st) SKM_sk_new(SXNETID, (st))
#define sk_SXNETID_new_null() SKM_sk_new_null(SXNETID)
#define sk_SXNETID_free(st) SKM_sk_free(SXNETID, (st))
#define sk_SXNETID_num(st) SKM_sk_num(SXNETID, (st))
#define sk_SXNETID_value(st, i) SKM_sk_value(SXNETID, (st), (i))
#define sk_SXNETID_set(st, i, val) SKM_sk_set(SXNETID, (st), (i), (val))
#define sk_SXNETID_zero(st) SKM_sk_zero(SXNETID, (st))
#define sk_SXNETID_push(st, val) SKM_sk_push(SXNETID, (st), (val))
#define sk_SXNETID_unshift(st, val) SKM_sk_unshift(SXNETID, (st), (val))
#define sk_SXNETID_find(st, val) SKM_sk_find(SXNETID, (st), (val))
#define sk_SXNETID_find_ex(st, val) SKM_sk_find_ex(SXNETID, (st), (val))
#define sk_SXNETID_delete(st, i) SKM_sk_delete(SXNETID, (st), (i))
#define sk_SXNETID_delete_ptr(st, ptr) SKM_sk_delete_ptr(SXNETID, (st), (ptr))
#define sk_SXNETID_insert(st, val, i) SKM_sk_insert(SXNETID, (st), (val), (i))
#define sk_SXNETID_set_cmp_func(st, cmp) \
  SKM_sk_set_cmp_func(SXNETID, (st), (cmp))
#define sk_SXNETID_dup(st) SKM_sk_dup(SXNETID, st)
#define sk_SXNETID_pop_free(st, free_func) \
  SKM_sk_pop_free(SXNETID, (st), (free_func))
#define sk_SXNETID_shift(st) SKM_sk_shift(SXNETID, (st))
#define sk_SXNETID_pop(st) SKM_sk_pop(SXNETID, (st))
#define sk_SXNETID_sort(st) SKM_sk_sort(SXNETID, (st))
#define sk_SXNETID_is_sorted(st) SKM_sk_is_sorted(SXNETID, (st))

#define sk_UI_STRING_new(st) SKM_sk_new(UI_STRING, (st))
#define sk_UI_STRING_new_null() SKM_sk_new_null(UI_STRING)
#define sk_UI_STRING_free(st) SKM_sk_free(UI_STRING, (st))
#define sk_UI_STRING_num(st) SKM_sk_num(UI_STRING, (st))
#define sk_UI_STRING_value(st, i) SKM_sk_value(UI_STRING, (st), (i))
#define sk_UI_STRING_set(st, i, val) SKM_sk_set(UI_STRING, (st), (i), (val))
#define sk_UI_STRING_zero(st) SKM_sk_zero(UI_STRING, (st))
#define sk_UI_STRING_push(st, val) SKM_sk_push(UI_STRING, (st), (val))
#define sk_UI_STRING_unshift(st, val) SKM_sk_unshift(UI_STRING, (st), (val))
#define sk_UI_STRING_find(st, val) SKM_sk_find(UI_STRING, (st), (val))
#define sk_UI_STRING_find_ex(st, val) SKM_sk_find_ex(UI_STRING, (st), (val))
#define sk_UI_STRING_delete(st, i) SKM_sk_delete(UI_STRING, (st), (i))
#define sk_UI_STRING_delete_ptr(st, ptr) \
  SKM_sk_delete_ptr(UI_STRING, (st), (ptr))
#define sk_UI_STRING_insert(st, val, i) \
  SKM_sk_insert(UI_STRING, (st), (val), (i))
#define sk_UI_STRING_set_cmp_func(st, cmp) \
  SKM_sk_set_cmp_func(UI_STRING, (st), (cmp))
#define sk_UI_STRING_dup(st) SKM_sk_dup(UI_STRING, st)
#define sk_UI_STRING_pop_free(st, free_func) \
  SKM_sk_pop_free(UI_STRING, (st), (free_func))
#define sk_UI_STRING_shift(st) SKM_sk_shift(UI_STRING, (st))
#define sk_UI_STRING_pop(st) SKM_sk_pop(UI_STRING, (st))
#define sk_UI_STRING_sort(st) SKM_sk_sort(UI_STRING, (st))
#define sk_UI_STRING_is_sorted(st) SKM_sk_is_sorted(UI_STRING, (st))

#define sk_X509_new(st) SKM_sk_new(X509, (st))
#define sk_X509_new_null() SKM_sk_new_null(X509)
#define sk_X509_free(st) SKM_sk_free(X509, (st))
#define sk_X509_num(st) SKM_sk_num(X509, (st))
#define sk_X509_value(st, i) SKM_sk_value(X509, (st), (i))
#define sk_X509_set(st, i, val) SKM_sk_set(X509, (st), (i), (val))
#define sk_X509_zero(st) SKM_sk_zero(X509, (st))
#define sk_X509_push(st, val) SKM_sk_push(X509, (st), (val))
#define sk_X509_unshift(st, val) SKM_sk_unshift(X509, (st), (val))
#define sk_X509_find(st, val) SKM_sk_find(X509, (st), (val))
#define sk_X509_find_ex(st, val) SKM_sk_find_ex(X509, (st), (val))
#define sk_X509_delete(st, i) SKM_sk_delete(X509, (st), (i))
#define sk_X509_delete_ptr(st, ptr) SKM_sk_delete_ptr(X509, (st), (ptr))
#define sk_X509_insert(st, val, i) SKM_sk_insert(X509, (st), (val), (i))
#define sk_X509_set_cmp_func(st, cmp) SKM_sk_set_cmp_func(X509, (st), (cmp))
#define sk_X509_dup(st) SKM_sk_dup(X509, st)
#define sk_X509_pop_free(st, free_func) SKM_sk_pop_free(X509, (st), (free_func))
#define sk_X509_shift(st) SKM_sk_shift(X509, (st))
#define sk_X509_pop(st) SKM_sk_pop(X509, (st))
#define sk_X509_sort(st) SKM_sk_sort(X509, (st))
#define sk_X509_is_sorted(st) SKM_sk_is_sorted(X509, (st))

#define sk_X509V3_EXT_METHOD_new(st) SKM_sk_new(X509V3_EXT_METHOD, (st))
#define sk_X509V3_EXT_METHOD_new_null() SKM_sk_new_null(X509V3_EXT_METHOD)
#define sk_X509V3_EXT_METHOD_free(st) SKM_sk_free(X509V3_EXT_METHOD, (st))
#define sk_X509V3_EXT_METHOD_num(st) SKM_sk_num(X509V3_EXT_METHOD, (st))
#define sk_X509V3_EXT_METHOD_value(st, i) \
  SKM_sk_value(X509V3_EXT_METHOD, (st), (i))
#define sk_X509V3_EXT_METHOD_set(st, i, val) \
  SKM_sk_set(X509V3_EXT_METHOD, (st), (i), (val))
#define sk_X509V3_EXT_METHOD_zero(st) SKM_sk_zero(X509V3_EXT_METHOD, (st))
#define sk_X509V3_EXT_METHOD_push(st, val) \
  SKM_sk_push(X509V3_EXT_METHOD, (st), (val))
#define sk_X509V3_EXT_METHOD_unshift(st, val) \
  SKM_sk_unshift(X509V3_EXT_METHOD, (st), (val))
#define sk_X509V3_EXT_METHOD_find(st, val) \
  SKM_sk_find(X509V3_EXT_METHOD, (st), (val))
#define sk_X509V3_EXT_METHOD_find_ex(st, val) \
  SKM_sk_find_ex(X509V3_EXT_METHOD, (st), (val))
#define sk_X509V3_EXT_METHOD_delete(st, i) \
  SKM_sk_delete(X509V3_EXT_METHOD, (st), (i))
#define sk_X509V3_EXT_METHOD_delete_ptr(st, ptr) \
  SKM_sk_delete_ptr(X509V3_EXT_METHOD, (st), (ptr))
#define sk_X509V3_EXT_METHOD_insert(st, val, i) \
  SKM_sk_insert(X509V3_EXT_METHOD, (st), (val), (i))
#define sk_X509V3_EXT_METHOD_set_cmp_func(st, cmp) \
  SKM_sk_set_cmp_func(X509V3_EXT_METHOD, (st), (cmp))
#define sk_X509V3_EXT_METHOD_dup(st) SKM_sk_dup(X509V3_EXT_METHOD, st)
#define sk_X509V3_EXT_METHOD_pop_free(st, free_func) \
  SKM_sk_pop_free(X509V3_EXT_METHOD, (st), (free_func))
#define sk_X509V3_EXT_METHOD_shift(st) SKM_sk_shift(X509V3_EXT_METHOD, (st))
#define sk_X509V3_EXT_METHOD_pop(st) SKM_sk_pop(X509V3_EXT_METHOD, (st))
#define sk_X509V3_EXT_METHOD_sort(st) SKM_sk_sort(X509V3_EXT_METHOD, (st))
#define sk_X509V3_EXT_METHOD_is_sorted(st) \
  SKM_sk_is_sorted(X509V3_EXT_METHOD, (st))

#define sk_X509_ALGOR_new(st) SKM_sk_new(X509_ALGOR, (st))
#define sk_X509_ALGOR_new_null() SKM_sk_new_null(X509_ALGOR)
#define sk_X509_ALGOR_free(st) SKM_sk_free(X509_ALGOR, (st))
#define sk_X509_ALGOR_num(st) SKM_sk_num(X509_ALGOR, (st))
#define sk_X509_ALGOR_value(st, i) SKM_sk_value(X509_ALGOR, (st), (i))
#define sk_X509_ALGOR_set(st, i, val) SKM_sk_set(X509_ALGOR, (st), (i), (val))
#define sk_X509_ALGOR_zero(st) SKM_sk_zero(X509_ALGOR, (st))
#define sk_X509_ALGOR_push(st, val) SKM_sk_push(X509_ALGOR, (st), (val))
#define sk_X509_ALGOR_unshift(st, val) SKM_sk_unshift(X509_ALGOR, (st), (val))
#define sk_X509_ALGOR_find(st, val) SKM_sk_find(X509_ALGOR, (st), (val))
#define sk_X509_ALGOR_find_ex(st, val) SKM_sk_find_ex(X509_ALGOR, (st), (val))
#define sk_X509_ALGOR_delete(st, i) SKM_sk_delete(X509_ALGOR, (st), (i))
#define sk_X509_ALGOR_delete_ptr(st, ptr) \
  SKM_sk_delete_ptr(X509_ALGOR, (st), (ptr))
#define sk_X509_ALGOR_insert(st, val, i) \
  SKM_sk_insert(X509_ALGOR, (st), (val), (i))
#define sk_X509_ALGOR_set_cmp_func(st, cmp) \
  SKM_sk_set_cmp_func(X509_ALGOR, (st), (cmp))
#define sk_X509_ALGOR_dup(st) SKM_sk_dup(X509_ALGOR, st)
#define sk_X509_ALGOR_pop_free(st, free_func) \
  SKM_sk_pop_free(X509_ALGOR, (st), (free_func))
#define sk_X509_ALGOR_shift(st) SKM_sk_shift(X509_ALGOR, (st))
#define sk_X509_ALGOR_pop(st) SKM_sk_pop(X509_ALGOR, (st))
#define sk_X509_ALGOR_sort(st) SKM_sk_sort(X509_ALGOR, (st))
#define sk_X509_ALGOR_is_sorted(st) SKM_sk_is_sorted(X509_ALGOR, (st))

#define sk_X509_ATTRIBUTE_new(st) SKM_sk_new(X509_ATTRIBUTE, (st))
#define sk_X509_ATTRIBUTE_new_null() SKM_sk_new_null(X509_ATTRIBUTE)
#define sk_X509_ATTRIBUTE_free(st) SKM_sk_free(X509_ATTRIBUTE, (st))
#define sk_X509_ATTRIBUTE_num(st) SKM_sk_num(X509_ATTRIBUTE, (st))
#define sk_X509_ATTRIBUTE_value(st, i) SKM_sk_value(X509_ATTRIBUTE, (st), (i))
#define sk_X509_ATTRIBUTE_set(st, i, val) \
  SKM_sk_set(X509_ATTRIBUTE, (st), (i), (val))
#define sk_X509_ATTRIBUTE_zero(st) SKM_sk_zero(X509_ATTRIBUTE, (st))
#define sk_X509_ATTRIBUTE_push(st, val) SKM_sk_push(X509_ATTRIBUTE, (st), (val))
#define sk_X509_ATTRIBUTE_unshift(st, val) \
  SKM_sk_unshift(X509_ATTRIBUTE, (st), (val))
#define sk_X509_ATTRIBUTE_find(st, val) SKM_sk_find(X509_ATTRIBUTE, (st), (val))
#define sk_X509_ATTRIBUTE_find_ex(st, val) \
  SKM_sk_find_ex(X509_ATTRIBUTE, (st), (val))
#define sk_X509_ATTRIBUTE_delete(st, i) SKM_sk_delete(X509_ATTRIBUTE, (st), (i))
#define sk_X509_ATTRIBUTE_delete_ptr(st, ptr) \
  SKM_sk_delete_ptr(X509_ATTRIBUTE, (st), (ptr))
#define sk_X509_ATTRIBUTE_insert(st, val, i) \
  SKM_sk_insert(X509_ATTRIBUTE, (st), (val), (i))
#define sk_X509_ATTRIBUTE_set_cmp_func(st, cmp) \
  SKM_sk_set_cmp_func(X509_ATTRIBUTE, (st), (cmp))
#define sk_X509_ATTRIBUTE_dup(st) SKM_sk_dup(X509_ATTRIBUTE, st)
#define sk_X509_ATTRIBUTE_pop_free(st, free_func) \
  SKM_sk_pop_free(X509_ATTRIBUTE, (st), (free_func))
#define sk_X509_ATTRIBUTE_shift(st) SKM_sk_shift(X509_ATTRIBUTE, (st))
#define sk_X509_ATTRIBUTE_pop(st) SKM_sk_pop(X509_ATTRIBUTE, (st))
#define sk_X509_ATTRIBUTE_sort(st) SKM_sk_sort(X509_ATTRIBUTE, (st))
#define sk_X509_ATTRIBUTE_is_sorted(st) SKM_sk_is_sorted(X509_ATTRIBUTE, (st))

#define sk_X509_CRL_new(st) SKM_sk_new(X509_CRL, (st))
#define sk_X509_CRL_new_null() SKM_sk_new_null(X509_CRL)
#define sk_X509_CRL_free(st) SKM_sk_free(X509_CRL, (st))
#define sk_X509_CRL_num(st) SKM_sk_num(X509_CRL, (st))
#define sk_X509_CRL_value(st, i) SKM_sk_value(X509_CRL, (st), (i))
#define sk_X509_CRL_set(st, i, val) SKM_sk_set(X509_CRL, (st), (i), (val))
#define sk_X509_CRL_zero(st) SKM_sk_zero(X509_CRL, (st))
#define sk_X509_CRL_push(st, val) SKM_sk_push(X509_CRL, (st), (val))
#define sk_X509_CRL_unshift(st, val) SKM_sk_unshift(X509_CRL, (st), (val))
#define sk_X509_CRL_find(st, val) SKM_sk_find(X509_CRL, (st), (val))
#define sk_X509_CRL_find_ex(st, val) SKM_sk_find_ex(X509_CRL, (st), (val))
#define sk_X509_CRL_delete(st, i) SKM_sk_delete(X509_CRL, (st), (i))
#define sk_X509_CRL_delete_ptr(st, ptr) SKM_sk_delete_ptr(X509_CRL, (st), (ptr))
#define sk_X509_CRL_insert(st, val, i) SKM_sk_insert(X509_CRL, (st), (val), (i))
#define sk_X509_CRL_set_cmp_func(st, cmp) \
  SKM_sk_set_cmp_func(X509_CRL, (st), (cmp))
#define sk_X509_CRL_dup(st) SKM_sk_dup(X509_CRL, st)
#define sk_X509_CRL_pop_free(st, free_func) \
  SKM_sk_pop_free(X509_CRL, (st), (free_func))
#define sk_X509_CRL_shift(st) SKM_sk_shift(X509_CRL, (st))
#define sk_X509_CRL_pop(st) SKM_sk_pop(X509_CRL, (st))
#define sk_X509_CRL_sort(st) SKM_sk_sort(X509_CRL, (st))
#define sk_X509_CRL_is_sorted(st) SKM_sk_is_sorted(X509_CRL, (st))

#define sk_X509_EXTENSION_new(st) SKM_sk_new(X509_EXTENSION, (st))
#define sk_X509_EXTENSION_new_null() SKM_sk_new_null(X509_EXTENSION)
#define sk_X509_EXTENSION_free(st) SKM_sk_free(X509_EXTENSION, (st))
#define sk_X509_EXTENSION_num(st) SKM_sk_num(X509_EXTENSION, (st))
#define sk_X509_EXTENSION_value(st, i) SKM_sk_value(X509_EXTENSION, (st), (i))
#define sk_X509_EXTENSION_set(st, i, val) \
  SKM_sk_set(X509_EXTENSION, (st), (i), (val))
#define sk_X509_EXTENSION_zero(st) SKM_sk_zero(X509_EXTENSION, (st))
#define sk_X509_EXTENSION_push(st, val) SKM_sk_push(X509_EXTENSION, (st), (val))
#define sk_X509_EXTENSION_unshift(st, val) \
  SKM_sk_unshift(X509_EXTENSION, (st), (val))
#define sk_X509_EXTENSION_find(st, val) SKM_sk_find(X509_EXTENSION, (st), (val))
#define sk_X509_EXTENSION_find_ex(st, val) \
  SKM_sk_find_ex(X509_EXTENSION, (st), (val))
#define sk_X509_EXTENSION_delete(st, i) SKM_sk_delete(X509_EXTENSION, (st), (i))
#define sk_X509_EXTENSION_delete_ptr(st, ptr) \
  SKM_sk_delete_ptr(X509_EXTENSION, (st), (ptr))
#define sk_X509_EXTENSION_insert(st, val, i) \
  SKM_sk_insert(X509_EXTENSION, (st), (val), (i))
#define sk_X509_EXTENSION_set_cmp_func(st, cmp) \
  SKM_sk_set_cmp_func(X509_EXTENSION, (st), (cmp))
#define sk_X509_EXTENSION_dup(st) SKM_sk_dup(X509_EXTENSION, st)
#define sk_X509_EXTENSION_pop_free(st, free_func) \
  SKM_sk_pop_free(X509_EXTENSION, (st), (free_func))
#define sk_X509_EXTENSION_shift(st) SKM_sk_shift(X509_EXTENSION, (st))
#define sk_X509_EXTENSION_pop(st) SKM_sk_pop(X509_EXTENSION, (st))
#define sk_X509_EXTENSION_sort(st) SKM_sk_sort(X509_EXTENSION, (st))
#define sk_X509_EXTENSION_is_sorted(st) SKM_sk_is_sorted(X509_EXTENSION, (st))

#define sk_X509_INFO_new(st) SKM_sk_new(X509_INFO, (st))
#define sk_X509_INFO_new_null() SKM_sk_new_null(X509_INFO)
#define sk_X509_INFO_free(st) SKM_sk_free(X509_INFO, (st))
#define sk_X509_INFO_num(st) SKM_sk_num(X509_INFO, (st))
#define sk_X509_INFO_value(st, i) SKM_sk_value(X509_INFO, (st), (i))
#define sk_X509_INFO_set(st, i, val) SKM_sk_set(X509_INFO, (st), (i), (val))
#define sk_X509_INFO_zero(st) SKM_sk_zero(X509_INFO, (st))
#define sk_X509_INFO_push(st, val) SKM_sk_push(X509_INFO, (st), (val))
#define sk_X509_INFO_unshift(st, val) SKM_sk_unshift(X509_INFO, (st), (val))
#define sk_X509_INFO_find(st, val) SKM_sk_find(X509_INFO, (st), (val))
#define sk_X509_INFO_find_ex(st, val) SKM_sk_find_ex(X509_INFO, (st), (val))
#define sk_X509_INFO_delete(st, i) SKM_sk_delete(X509_INFO, (st), (i))
#define sk_X509_INFO_delete_ptr(st, ptr) \
  SKM_sk_delete_ptr(X509_INFO, (st), (ptr))
#define sk_X509_INFO_insert(st, val, i) \
  SKM_sk_insert(X509_INFO, (st), (val), (i))
#define sk_X509_INFO_set_cmp_func(st, cmp) \
  SKM_sk_set_cmp_func(X509_INFO, (st), (cmp))
#define sk_X509_INFO_dup(st) SKM_sk_dup(X509_INFO, st)
#define sk_X509_INFO_pop_free(st, free_func) \
  SKM_sk_pop_free(X509_INFO, (st), (free_func))
#define sk_X509_INFO_shift(st) SKM_sk_shift(X509_INFO, (st))
#define sk_X509_INFO_pop(st) SKM_sk_pop(X509_INFO, (st))
#define sk_X509_INFO_sort(st) SKM_sk_sort(X509_INFO, (st))
#define sk_X509_INFO_is_sorted(st) SKM_sk_is_sorted(X509_INFO, (st))

#define sk_X509_LOOKUP_new(st) SKM_sk_new(X509_LOOKUP, (st))
#define sk_X509_LOOKUP_new_null() SKM_sk_new_null(X509_LOOKUP)
#define sk_X509_LOOKUP_free(st) SKM_sk_free(X509_LOOKUP, (st))
#define sk_X509_LOOKUP_num(st) SKM_sk_num(X509_LOOKUP, (st))
#define sk_X509_LOOKUP_value(st, i) SKM_sk_value(X509_LOOKUP, (st), (i))
#define sk_X509_LOOKUP_set(st, i, val) SKM_sk_set(X509_LOOKUP, (st), (i), (val))
#define sk_X509_LOOKUP_zero(st) SKM_sk_zero(X509_LOOKUP, (st))
#define sk_X509_LOOKUP_push(st, val) SKM_sk_push(X509_LOOKUP, (st), (val))
#define sk_X509_LOOKUP_unshift(st, val) SKM_sk_unshift(X509_LOOKUP, (st), (val))
#define sk_X509_LOOKUP_find(st, val) SKM_sk_find(X509_LOOKUP, (st), (val))
#define sk_X509_LOOKUP_find_ex(st, val) SKM_sk_find_ex(X509_LOOKUP, (st), (val))
#define sk_X509_LOOKUP_delete(st, i) SKM_sk_delete(X509_LOOKUP, (st), (i))
#define sk_X509_LOOKUP_delete_ptr(st, ptr) \
  SKM_sk_delete_ptr(X509_LOOKUP, (st), (ptr))
#define sk_X509_LOOKUP_insert(st, val, i) \
  SKM_sk_insert(X509_LOOKUP, (st), (val), (i))
#define sk_X509_LOOKUP_set_cmp_func(st, cmp) \
  SKM_sk_set_cmp_func(X509_LOOKUP, (st), (cmp))
#define sk_X509_LOOKUP_dup(st) SKM_sk_dup(X509_LOOKUP, st)
#define sk_X509_LOOKUP_pop_free(st, free_func) \
  SKM_sk_pop_free(X509_LOOKUP, (st), (free_func))
#define sk_X509_LOOKUP_shift(st) SKM_sk_shift(X509_LOOKUP, (st))
#define sk_X509_LOOKUP_pop(st) SKM_sk_pop(X509_LOOKUP, (st))
#define sk_X509_LOOKUP_sort(st) SKM_sk_sort(X509_LOOKUP, (st))
#define sk_X509_LOOKUP_is_sorted(st) SKM_sk_is_sorted(X509_LOOKUP, (st))

#define sk_X509_NAME_new(st) SKM_sk_new(X509_NAME, (st))
#define sk_X509_NAME_new_null() SKM_sk_new_null(X509_NAME)
#define sk_X509_NAME_free(st) SKM_sk_free(X509_NAME, (st))
#define sk_X509_NAME_num(st) SKM_sk_num(X509_NAME, (st))
#define sk_X509_NAME_value(st, i) SKM_sk_value(X509_NAME, (st), (i))
#define sk_X509_NAME_set(st, i, val) SKM_sk_set(X509_NAME, (st), (i), (val))
#define sk_X509_NAME_zero(st) SKM_sk_zero(X509_NAME, (st))
#define sk_X509_NAME_push(st, val) SKM_sk_push(X509_NAME, (st), (val))
#define sk_X509_NAME_unshift(st, val) SKM_sk_unshift(X509_NAME, (st), (val))
#define sk_X509_NAME_find(st, val) SKM_sk_find(X509_NAME, (st), (val))
#define sk_X509_NAME_find_ex(st, val) SKM_sk_find_ex(X509_NAME, (st), (val))
#define sk_X509_NAME_delete(st, i) SKM_sk_delete(X509_NAME, (st), (i))
#define sk_X509_NAME_delete_ptr(st, ptr) \
  SKM_sk_delete_ptr(X509_NAME, (st), (ptr))
#define sk_X509_NAME_insert(st, val, i) \
  SKM_sk_insert(X509_NAME, (st), (val), (i))
#define sk_X509_NAME_set_cmp_func(st, cmp) \
  SKM_sk_set_cmp_func(X509_NAME, (st), (cmp))
#define sk_X509_NAME_dup(st) SKM_sk_dup(X509_NAME, st)
#define sk_X509_NAME_pop_free(st, free_func) \
  SKM_sk_pop_free(X509_NAME, (st), (free_func))
#define sk_X509_NAME_shift(st) SKM_sk_shift(X509_NAME, (st))
#define sk_X509_NAME_pop(st) SKM_sk_pop(X509_NAME, (st))
#define sk_X509_NAME_sort(st) SKM_sk_sort(X509_NAME, (st))
#define sk_X509_NAME_is_sorted(st) SKM_sk_is_sorted(X509_NAME, (st))

#define sk_X509_NAME_ENTRY_new(st) SKM_sk_new(X509_NAME_ENTRY, (st))
#define sk_X509_NAME_ENTRY_new_null() SKM_sk_new_null(X509_NAME_ENTRY)
#define sk_X509_NAME_ENTRY_free(st) SKM_sk_free(X509_NAME_ENTRY, (st))
#define sk_X509_NAME_ENTRY_num(st) SKM_sk_num(X509_NAME_ENTRY, (st))
#define sk_X509_NAME_ENTRY_value(st, i) SKM_sk_value(X509_NAME_ENTRY, (st), (i))
#define sk_X509_NAME_ENTRY_set(st, i, val) \
  SKM_sk_set(X509_NAME_ENTRY, (st), (i), (val))
#define sk_X509_NAME_ENTRY_zero(st) SKM_sk_zero(X509_NAME_ENTRY, (st))
#define sk_X509_NAME_ENTRY_push(st, val) \
  SKM_sk_push(X509_NAME_ENTRY, (st), (val))
#define sk_X509_NAME_ENTRY_unshift(st, val) \
  SKM_sk_unshift(X509_NAME_ENTRY, (st), (val))
#define sk_X509_NAME_ENTRY_find(st, val) \
  SKM_sk_find(X509_NAME_ENTRY, (st), (val))
#define sk_X509_NAME_ENTRY_find_ex(st, val) \
  SKM_sk_find_ex(X509_NAME_ENTRY, (st), (val))
#define sk_X509_NAME_ENTRY_delete(st, i) \
  SKM_sk_delete(X509_NAME_ENTRY, (st), (i))
#define sk_X509_NAME_ENTRY_delete_ptr(st, ptr) \
  SKM_sk_delete_ptr(X509_NAME_ENTRY, (st), (ptr))
#define sk_X509_NAME_ENTRY_insert(st, val, i) \
  SKM_sk_insert(X509_NAME_ENTRY, (st), (val), (i))
#define sk_X509_NAME_ENTRY_set_cmp_func(st, cmp) \
  SKM_sk_set_cmp_func(X509_NAME_ENTRY, (st), (cmp))
#define sk_X509_NAME_ENTRY_dup(st) SKM_sk_dup(X509_NAME_ENTRY, st)
#define sk_X509_NAME_ENTRY_pop_free(st, free_func) \
  SKM_sk_pop_free(X509_NAME_ENTRY, (st), (free_func))
#define sk_X509_NAME_ENTRY_shift(st) SKM_sk_shift(X509_NAME_ENTRY, (st))
#define sk_X509_NAME_ENTRY_pop(st) SKM_sk_pop(X509_NAME_ENTRY, (st))
#define sk_X509_NAME_ENTRY_sort(st) SKM_sk_sort(X509_NAME_ENTRY, (st))
#define sk_X509_NAME_ENTRY_is_sorted(st) SKM_sk_is_sorted(X509_NAME_ENTRY, (st))

#define sk_X509_OBJECT_new(st) SKM_sk_new(X509_OBJECT, (st))
#define sk_X509_OBJECT_new_null() SKM_sk_new_null(X509_OBJECT)
#define sk_X509_OBJECT_free(st) SKM_sk_free(X509_OBJECT, (st))
#define sk_X509_OBJECT_num(st) SKM_sk_num(X509_OBJECT, (st))
#define sk_X509_OBJECT_value(st, i) SKM_sk_value(X509_OBJECT, (st), (i))
#define sk_X509_OBJECT_set(st, i, val) SKM_sk_set(X509_OBJECT, (st), (i), (val))
#define sk_X509_OBJECT_zero(st) SKM_sk_zero(X509_OBJECT, (st))
#define sk_X509_OBJECT_push(st, val) SKM_sk_push(X509_OBJECT, (st), (val))
#define sk_X509_OBJECT_unshift(st, val) SKM_sk_unshift(X509_OBJECT, (st), (val))
#define sk_X509_OBJECT_find(st, val) SKM_sk_find(X509_OBJECT, (st), (val))
#define sk_X509_OBJECT_find_ex(st, val) SKM_sk_find_ex(X509_OBJECT, (st), (val))
#define sk_X509_OBJECT_delete(st, i) SKM_sk_delete(X509_OBJECT, (st), (i))
#define sk_X509_OBJECT_delete_ptr(st, ptr) \
  SKM_sk_delete_ptr(X509_OBJECT, (st), (ptr))
#define sk_X509_OBJECT_insert(st, val, i) \
  SKM_sk_insert(X509_OBJECT, (st), (val), (i))
#define sk_X509_OBJECT_set_cmp_func(st, cmp) \
  SKM_sk_set_cmp_func(X509_OBJECT, (st), (cmp))
#define sk_X509_OBJECT_dup(st) SKM_sk_dup(X509_OBJECT, st)
#define sk_X509_OBJECT_pop_free(st, free_func) \
  SKM_sk_pop_free(X509_OBJECT, (st), (free_func))
#define sk_X509_OBJECT_shift(st) SKM_sk_shift(X509_OBJECT, (st))
#define sk_X509_OBJECT_pop(st) SKM_sk_pop(X509_OBJECT, (st))
#define sk_X509_OBJECT_sort(st) SKM_sk_sort(X509_OBJECT, (st))
#define sk_X509_OBJECT_is_sorted(st) SKM_sk_is_sorted(X509_OBJECT, (st))

#define sk_X509_POLICY_DATA_new(st) SKM_sk_new(X509_POLICY_DATA, (st))
#define sk_X509_POLICY_DATA_new_null() SKM_sk_new_null(X509_POLICY_DATA)
#define sk_X509_POLICY_DATA_free(st) SKM_sk_free(X509_POLICY_DATA, (st))
#define sk_X509_POLICY_DATA_num(st) SKM_sk_num(X509_POLICY_DATA, (st))
#define sk_X509_POLICY_DATA_value(st, i) \
  SKM_sk_value(X509_POLICY_DATA, (st), (i))
#define sk_X509_POLICY_DATA_set(st, i, val) \
  SKM_sk_set(X509_POLICY_DATA, (st), (i), (val))
#define sk_X509_POLICY_DATA_zero(st) SKM_sk_zero(X509_POLICY_DATA, (st))
#define sk_X509_POLICY_DATA_push(st, val) \
  SKM_sk_push(X509_POLICY_DATA, (st), (val))
#define sk_X509_POLICY_DATA_unshift(st, val) \
  SKM_sk_unshift(X509_POLICY_DATA, (st), (val))
#define sk_X509_POLICY_DATA_find(st, val) \
  SKM_sk_find(X509_POLICY_DATA, (st), (val))
#define sk_X509_POLICY_DATA_find_ex(st, val) \
  SKM_sk_find_ex(X509_POLICY_DATA, (st), (val))
#define sk_X509_POLICY_DATA_delete(st, i) \
  SKM_sk_delete(X509_POLICY_DATA, (st), (i))
#define sk_X509_POLICY_DATA_delete_ptr(st, ptr) \
  SKM_sk_delete_ptr(X509_POLICY_DATA, (st), (ptr))
#define sk_X509_POLICY_DATA_insert(st, val, i) \
  SKM_sk_insert(X509_POLICY_DATA, (st), (val), (i))
#define sk_X509_POLICY_DATA_set_cmp_func(st, cmp) \
  SKM_sk_set_cmp_func(X509_POLICY_DATA, (st), (cmp))
#define sk_X509_POLICY_DATA_dup(st) SKM_sk_dup(X509_POLICY_DATA, st)
#define sk_X509_POLICY_DATA_pop_free(st, free_func) \
  SKM_sk_pop_free(X509_POLICY_DATA, (st), (free_func))
#define sk_X509_POLICY_DATA_shift(st) SKM_sk_shift(X509_POLICY_DATA, (st))
#define sk_X509_POLICY_DATA_pop(st) SKM_sk_pop(X509_POLICY_DATA, (st))
#define sk_X509_POLICY_DATA_sort(st) SKM_sk_sort(X509_POLICY_DATA, (st))
#define sk_X509_POLICY_DATA_is_sorted(st) \
  SKM_sk_is_sorted(X509_POLICY_DATA, (st))

#define sk_X509_POLICY_NODE_new(st) SKM_sk_new(X509_POLICY_NODE, (st))
#define sk_X509_POLICY_NODE_new_null() SKM_sk_new_null(X509_POLICY_NODE)
#define sk_X509_POLICY_NODE_free(st) SKM_sk_free(X509_POLICY_NODE, (st))
#define sk_X509_POLICY_NODE_num(st) SKM_sk_num(X509_POLICY_NODE, (st))
#define sk_X509_POLICY_NODE_value(st, i) \
  SKM_sk_value(X509_POLICY_NODE, (st), (i))
#define sk_X509_POLICY_NODE_set(st, i, val) \
  SKM_sk_set(X509_POLICY_NODE, (st), (i), (val))
#define sk_X509_POLICY_NODE_zero(st) SKM_sk_zero(X509_POLICY_NODE, (st))
#define sk_X509_POLICY_NODE_push(st, val) \
  SKM_sk_push(X509_POLICY_NODE, (st), (val))
#define sk_X509_POLICY_NODE_unshift(st, val) \
  SKM_sk_unshift(X509_POLICY_NODE, (st), (val))
#define sk_X509_POLICY_NODE_find(st, val) \
  SKM_sk_find(X509_POLICY_NODE, (st), (val))
#define sk_X509_POLICY_NODE_find_ex(st, val) \
  SKM_sk_find_ex(X509_POLICY_NODE, (st), (val))
#define sk_X509_POLICY_NODE_delete(st, i) \
  SKM_sk_delete(X509_POLICY_NODE, (st), (i))
#define sk_X509_POLICY_NODE_delete_ptr(st, ptr) \
  SKM_sk_delete_ptr(X509_POLICY_NODE, (st), (ptr))
#define sk_X509_POLICY_NODE_insert(st, val, i) \
  SKM_sk_insert(X509_POLICY_NODE, (st), (val), (i))
#define sk_X509_POLICY_NODE_set_cmp_func(st, cmp) \
  SKM_sk_set_cmp_func(X509_POLICY_NODE, (st), (cmp))
#define sk_X509_POLICY_NODE_dup(st) SKM_sk_dup(X509_POLICY_NODE, st)
#define sk_X509_POLICY_NODE_pop_free(st, free_func) \
  SKM_sk_pop_free(X509_POLICY_NODE, (st), (free_func))
#define sk_X509_POLICY_NODE_shift(st) SKM_sk_shift(X509_POLICY_NODE, (st))
#define sk_X509_POLICY_NODE_pop(st) SKM_sk_pop(X509_POLICY_NODE, (st))
#define sk_X509_POLICY_NODE_sort(st) SKM_sk_sort(X509_POLICY_NODE, (st))
#define sk_X509_POLICY_NODE_is_sorted(st) \
  SKM_sk_is_sorted(X509_POLICY_NODE, (st))

#define sk_X509_POLICY_REF_new(st) SKM_sk_new(X509_POLICY_REF, (st))
#define sk_X509_POLICY_REF_new_null() SKM_sk_new_null(X509_POLICY_REF)
#define sk_X509_POLICY_REF_free(st) SKM_sk_free(X509_POLICY_REF, (st))
#define sk_X509_POLICY_REF_num(st) SKM_sk_num(X509_POLICY_REF, (st))
#define sk_X509_POLICY_REF_value(st, i) SKM_sk_value(X509_POLICY_REF, (st), (i))
#define sk_X509_POLICY_REF_set(st, i, val) \
  SKM_sk_set(X509_POLICY_REF, (st), (i), (val))
#define sk_X509_POLICY_REF_zero(st) SKM_sk_zero(X509_POLICY_REF, (st))
#define sk_X509_POLICY_REF_push(st, val) \
  SKM_sk_push(X509_POLICY_REF, (st), (val))
#define sk_X509_POLICY_REF_unshift(st, val) \
  SKM_sk_unshift(X509_POLICY_REF, (st), (val))
#define sk_X509_POLICY_REF_find(st, val) \
  SKM_sk_find(X509_POLICY_REF, (st), (val))
#define sk_X509_POLICY_REF_find_ex(st, val) \
  SKM_sk_find_ex(X509_POLICY_REF, (st), (val))
#define sk_X509_POLICY_REF_delete(st, i) \
  SKM_sk_delete(X509_POLICY_REF, (st), (i))
#define sk_X509_POLICY_REF_delete_ptr(st, ptr) \
  SKM_sk_delete_ptr(X509_POLICY_REF, (st), (ptr))
#define sk_X509_POLICY_REF_insert(st, val, i) \
  SKM_sk_insert(X509_POLICY_REF, (st), (val), (i))
#define sk_X509_POLICY_REF_set_cmp_func(st, cmp) \
  SKM_sk_set_cmp_func(X509_POLICY_REF, (st), (cmp))
#define sk_X509_POLICY_REF_dup(st) SKM_sk_dup(X509_POLICY_REF, st)
#define sk_X509_POLICY_REF_pop_free(st, free_func) \
  SKM_sk_pop_free(X509_POLICY_REF, (st), (free_func))
#define sk_X509_POLICY_REF_shift(st) SKM_sk_shift(X509_POLICY_REF, (st))
#define sk_X509_POLICY_REF_pop(st) SKM_sk_pop(X509_POLICY_REF, (st))
#define sk_X509_POLICY_REF_sort(st) SKM_sk_sort(X509_POLICY_REF, (st))
#define sk_X509_POLICY_REF_is_sorted(st) SKM_sk_is_sorted(X509_POLICY_REF, (st))

#define sk_X509_PURPOSE_new(st) SKM_sk_new(X509_PURPOSE, (st))
#define sk_X509_PURPOSE_new_null() SKM_sk_new_null(X509_PURPOSE)
#define sk_X509_PURPOSE_free(st) SKM_sk_free(X509_PURPOSE, (st))
#define sk_X509_PURPOSE_num(st) SKM_sk_num(X509_PURPOSE, (st))
#define sk_X509_PURPOSE_value(st, i) SKM_sk_value(X509_PURPOSE, (st), (i))
#define sk_X509_PURPOSE_set(st, i, val) \
  SKM_sk_set(X509_PURPOSE, (st), (i), (val))
#define sk_X509_PURPOSE_zero(st) SKM_sk_zero(X509_PURPOSE, (st))
#define sk_X509_PURPOSE_push(st, val) SKM_sk_push(X509_PURPOSE, (st), (val))
#define sk_X509_PURPOSE_unshift(st, val) \
  SKM_sk_unshift(X509_PURPOSE, (st), (val))
#define sk_X509_PURPOSE_find(st, val) SKM_sk_find(X509_PURPOSE, (st), (val))
#define sk_X509_PURPOSE_find_ex(st, val) \
  SKM_sk_find_ex(X509_PURPOSE, (st), (val))
#define sk_X509_PURPOSE_delete(st, i) SKM_sk_delete(X509_PURPOSE, (st), (i))
#define sk_X509_PURPOSE_delete_ptr(st, ptr) \
  SKM_sk_delete_ptr(X509_PURPOSE, (st), (ptr))
#define sk_X509_PURPOSE_insert(st, val, i) \
  SKM_sk_insert(X509_PURPOSE, (st), (val), (i))
#define sk_X509_PURPOSE_set_cmp_func(st, cmp) \
  SKM_sk_set_cmp_func(X509_PURPOSE, (st), (cmp))
#define sk_X509_PURPOSE_dup(st) SKM_sk_dup(X509_PURPOSE, st)
#define sk_X509_PURPOSE_pop_free(st, free_func) \
  SKM_sk_pop_free(X509_PURPOSE, (st), (free_func))
#define sk_X509_PURPOSE_shift(st) SKM_sk_shift(X509_PURPOSE, (st))
#define sk_X509_PURPOSE_pop(st) SKM_sk_pop(X509_PURPOSE, (st))
#define sk_X509_PURPOSE_sort(st) SKM_sk_sort(X509_PURPOSE, (st))
#define sk_X509_PURPOSE_is_sorted(st) SKM_sk_is_sorted(X509_PURPOSE, (st))

#define sk_X509_REVOKED_new(st) SKM_sk_new(X509_REVOKED, (st))
#define sk_X509_REVOKED_new_null() SKM_sk_new_null(X509_REVOKED)
#define sk_X509_REVOKED_free(st) SKM_sk_free(X509_REVOKED, (st))
#define sk_X509_REVOKED_num(st) SKM_sk_num(X509_REVOKED, (st))
#define sk_X509_REVOKED_value(st, i) SKM_sk_value(X509_REVOKED, (st), (i))
#define sk_X509_REVOKED_set(st, i, val) \
  SKM_sk_set(X509_REVOKED, (st), (i), (val))
#define sk_X509_REVOKED_zero(st) SKM_sk_zero(X509_REVOKED, (st))
#define sk_X509_REVOKED_push(st, val) SKM_sk_push(X509_REVOKED, (st), (val))
#define sk_X509_REVOKED_unshift(st, val) \
  SKM_sk_unshift(X509_REVOKED, (st), (val))
#define sk_X509_REVOKED_find(st, val) SKM_sk_find(X509_REVOKED, (st), (val))
#define sk_X509_REVOKED_find_ex(st, val) \
  SKM_sk_find_ex(X509_REVOKED, (st), (val))
#define sk_X509_REVOKED_delete(st, i) SKM_sk_delete(X509_REVOKED, (st), (i))
#define sk_X509_REVOKED_delete_ptr(st, ptr) \
  SKM_sk_delete_ptr(X509_REVOKED, (st), (ptr))
#define sk_X509_REVOKED_insert(st, val, i) \
  SKM_sk_insert(X509_REVOKED, (st), (val), (i))
#define sk_X509_REVOKED_set_cmp_func(st, cmp) \
  SKM_sk_set_cmp_func(X509_REVOKED, (st), (cmp))
#define sk_X509_REVOKED_dup(st) SKM_sk_dup(X509_REVOKED, st)
#define sk_X509_REVOKED_pop_free(st, free_func) \
  SKM_sk_pop_free(X509_REVOKED, (st), (free_func))
#define sk_X509_REVOKED_shift(st) SKM_sk_shift(X509_REVOKED, (st))
#define sk_X509_REVOKED_pop(st) SKM_sk_pop(X509_REVOKED, (st))
#define sk_X509_REVOKED_sort(st) SKM_sk_sort(X509_REVOKED, (st))
#define sk_X509_REVOKED_is_sorted(st) SKM_sk_is_sorted(X509_REVOKED, (st))

#define sk_X509_TRUST_new(st) SKM_sk_new(X509_TRUST, (st))
#define sk_X509_TRUST_new_null() SKM_sk_new_null(X509_TRUST)
#define sk_X509_TRUST_free(st) SKM_sk_free(X509_TRUST, (st))
#define sk_X509_TRUST_num(st) SKM_sk_num(X509_TRUST, (st))
#define sk_X509_TRUST_value(st, i) SKM_sk_value(X509_TRUST, (st), (i))
#define sk_X509_TRUST_set(st, i, val) SKM_sk_set(X509_TRUST, (st), (i), (val))
#define sk_X509_TRUST_zero(st) SKM_sk_zero(X509_TRUST, (st))
#define sk_X509_TRUST_push(st, val) SKM_sk_push(X509_TRUST, (st), (val))
#define sk_X509_TRUST_unshift(st, val) SKM_sk_unshift(X509_TRUST, (st), (val))
#define sk_X509_TRUST_find(st, val) SKM_sk_find(X509_TRUST, (st), (val))
#define sk_X509_TRUST_find_ex(st, val) SKM_sk_find_ex(X509_TRUST, (st), (val))
#define sk_X509_TRUST_delete(st, i) SKM_sk_delete(X509_TRUST, (st), (i))
#define sk_X509_TRUST_delete_ptr(st, ptr) \
  SKM_sk_delete_ptr(X509_TRUST, (st), (ptr))
#define sk_X509_TRUST_insert(st, val, i) \
  SKM_sk_insert(X509_TRUST, (st), (val), (i))
#define sk_X509_TRUST_set_cmp_func(st, cmp) \
  SKM_sk_set_cmp_func(X509_TRUST, (st), (cmp))
#define sk_X509_TRUST_dup(st) SKM_sk_dup(X509_TRUST, st)
#define sk_X509_TRUST_pop_free(st, free_func) \
  SKM_sk_pop_free(X509_TRUST, (st), (free_func))
#define sk_X509_TRUST_shift(st) SKM_sk_shift(X509_TRUST, (st))
#define sk_X509_TRUST_pop(st) SKM_sk_pop(X509_TRUST, (st))
#define sk_X509_TRUST_sort(st) SKM_sk_sort(X509_TRUST, (st))
#define sk_X509_TRUST_is_sorted(st) SKM_sk_is_sorted(X509_TRUST, (st))

#define sk_X509_VERIFY_PARAM_new(st) SKM_sk_new(X509_VERIFY_PARAM, (st))
#define sk_X509_VERIFY_PARAM_new_null() SKM_sk_new_null(X509_VERIFY_PARAM)
#define sk_X509_VERIFY_PARAM_free(st) SKM_sk_free(X509_VERIFY_PARAM, (st))
#define sk_X509_VERIFY_PARAM_num(st) SKM_sk_num(X509_VERIFY_PARAM, (st))
#define sk_X509_VERIFY_PARAM_value(st, i) \
  SKM_sk_value(X509_VERIFY_PARAM, (st), (i))
#define sk_X509_VERIFY_PARAM_set(st, i, val) \
  SKM_sk_set(X509_VERIFY_PARAM, (st), (i), (val))
#define sk_X509_VERIFY_PARAM_zero(st) SKM_sk_zero(X509_VERIFY_PARAM, (st))
#define sk_X509_VERIFY_PARAM_push(st, val) \
  SKM_sk_push(X509_VERIFY_PARAM, (st), (val))
#define sk_X509_VERIFY_PARAM_unshift(st, val) \
  SKM_sk_unshift(X509_VERIFY_PARAM, (st), (val))
#define sk_X509_VERIFY_PARAM_find(st, val) \
  SKM_sk_find(X509_VERIFY_PARAM, (st), (val))
#define sk_X509_VERIFY_PARAM_find_ex(st, val) \
  SKM_sk_find_ex(X509_VERIFY_PARAM, (st), (val))
#define sk_X509_VERIFY_PARAM_delete(st, i) \
  SKM_sk_delete(X509_VERIFY_PARAM, (st), (i))
#define sk_X509_VERIFY_PARAM_delete_ptr(st, ptr) \
  SKM_sk_delete_ptr(X509_VERIFY_PARAM, (st), (ptr))
#define sk_X509_VERIFY_PARAM_insert(st, val, i) \
  SKM_sk_insert(X509_VERIFY_PARAM, (st), (val), (i))
#define sk_X509_VERIFY_PARAM_set_cmp_func(st, cmp) \
  SKM_sk_set_cmp_func(X509_VERIFY_PARAM, (st), (cmp))
#define sk_X509_VERIFY_PARAM_dup(st) SKM_sk_dup(X509_VERIFY_PARAM, st)
#define sk_X509_VERIFY_PARAM_pop_free(st, free_func) \
  SKM_sk_pop_free(X509_VERIFY_PARAM, (st), (free_func))
#define sk_X509_VERIFY_PARAM_shift(st) SKM_sk_shift(X509_VERIFY_PARAM, (st))
#define sk_X509_VERIFY_PARAM_pop(st) SKM_sk_pop(X509_VERIFY_PARAM, (st))
#define sk_X509_VERIFY_PARAM_sort(st) SKM_sk_sort(X509_VERIFY_PARAM, (st))
#define sk_X509_VERIFY_PARAM_is_sorted(st) \
  SKM_sk_is_sorted(X509_VERIFY_PARAM, (st))

#define d2i_ASN1_SET_OF_ACCESS_DESCRIPTION(st, pp, length, d2i_func,        \
                                           free_func, ex_tag, ex_class)     \
  SKM_ASN1_SET_OF_d2i(ACCESS_DESCRIPTION, (st), (pp), (length), (d2i_func), \
                      (free_func), (ex_tag), (ex_class))
#define i2d_ASN1_SET_OF_ACCESS_DESCRIPTION(st, pp, i2d_func, ex_tag, ex_class, \
                                           is_set)                             \
  SKM_ASN1_SET_OF_i2d(ACCESS_DESCRIPTION, (st), (pp), (i2d_func), (ex_tag),    \
                      (ex_class), (is_set))
#define ASN1_seq_pack_ACCESS_DESCRIPTION(st, i2d_func, buf, len) \
  SKM_ASN1_seq_pack(ACCESS_DESCRIPTION, (st), (i2d_func), (buf), (len))
#define ASN1_seq_unpack_ACCESS_DESCRIPTION(buf, len, d2i_func, free_func) \
  SKM_ASN1_seq_unpack(ACCESS_DESCRIPTION, (buf), (len), (d2i_func), (free_func))

#define d2i_ASN1_SET_OF_ASN1_INTEGER(st, pp, length, d2i_func, free_func, \
                                     ex_tag, ex_class)                    \
  SKM_ASN1_SET_OF_d2i(ASN1_INTEGER, (st), (pp), (length), (d2i_func),     \
                      (free_func), (ex_tag), (ex_class))
#define i2d_ASN1_SET_OF_ASN1_INTEGER(st, pp, i2d_func, ex_tag, ex_class, \
                                     is_set)                             \
  SKM_ASN1_SET_OF_i2d(ASN1_INTEGER, (st), (pp), (i2d_func), (ex_tag),    \
                      (ex_class), (is_set))
#define ASN1_seq_pack_ASN1_INTEGER(st, i2d_func, buf, len) \
  SKM_ASN1_seq_pack(ASN1_INTEGER, (st), (i2d_func), (buf), (len))
#define ASN1_seq_unpack_ASN1_INTEGER(buf, len, d2i_func, free_func) \
  SKM_ASN1_seq_unpack(ASN1_INTEGER, (buf), (len), (d2i_func), (free_func))

#define d2i_ASN1_SET_OF_ASN1_OBJECT(st, pp, length, d2i_func, free_func, \
                                    ex_tag, ex_class)                    \
  SKM_ASN1_SET_OF_d2i(ASN1_OBJECT, (st), (pp), (length), (d2i_func),     \
                      (free_func), (ex_tag), (ex_class))
#define i2d_ASN1_SET_OF_ASN1_OBJECT(st, pp, i2d_func, ex_tag, ex_class, \
                                    is_set)                             \
  SKM_ASN1_SET_OF_i2d(ASN1_OBJECT, (st), (pp), (i2d_func), (ex_tag),    \
                      (ex_class), (is_set))
#define ASN1_seq_pack_ASN1_OBJECT(st, i2d_func, buf, len) \
  SKM_ASN1_seq_pack(ASN1_OBJECT, (st), (i2d_func), (buf), (len))
#define ASN1_seq_unpack_ASN1_OBJECT(buf, len, d2i_func, free_func) \
  SKM_ASN1_seq_unpack(ASN1_OBJECT, (buf), (len), (d2i_func), (free_func))

#define d2i_ASN1_SET_OF_ASN1_TYPE(st, pp, length, d2i_func, free_func, ex_tag, \
                                  ex_class)                                    \
  SKM_ASN1_SET_OF_d2i(ASN1_TYPE, (st), (pp), (length), (d2i_func),             \
                      (free_func), (ex_tag), (ex_class))
#define i2d_ASN1_SET_OF_ASN1_TYPE(st, pp, i2d_func, ex_tag, ex_class, is_set)  \
  SKM_ASN1_SET_OF_i2d(ASN1_TYPE, (st), (pp), (i2d_func), (ex_tag), (ex_class), \
                      (is_set))
#define ASN1_seq_pack_ASN1_TYPE(st, i2d_func, buf, len) \
  SKM_ASN1_seq_pack(ASN1_TYPE, (st), (i2d_func), (buf), (len))
#define ASN1_seq_unpack_ASN1_TYPE(buf, len, d2i_func, free_func) \
  SKM_ASN1_seq_unpack(ASN1_TYPE, (buf), (len), (d2i_func), (free_func))

#define d2i_ASN1_SET_OF_DIST_POINT(st, pp, length, d2i_func, free_func, \
                                   ex_tag, ex_class)                    \
  SKM_ASN1_SET_OF_d2i(DIST_POINT, (st), (pp), (length), (d2i_func),     \
                      (free_func), (ex_tag), (ex_class))
#define i2d_ASN1_SET_OF_DIST_POINT(st, pp, i2d_func, ex_tag, ex_class, is_set) \
  SKM_ASN1_SET_OF_i2d(DIST_POINT, (st), (pp), (i2d_func), (ex_tag),            \
                      (ex_class), (is_set))
#define ASN1_seq_pack_DIST_POINT(st, i2d_func, buf, len) \
  SKM_ASN1_seq_pack(DIST_POINT, (st), (i2d_func), (buf), (len))
#define ASN1_seq_unpack_DIST_POINT(buf, len, d2i_func, free_func) \
  SKM_ASN1_seq_unpack(DIST_POINT, (buf), (len), (d2i_func), (free_func))

#define d2i_ASN1_SET_OF_GENERAL_NAME(st, pp, length, d2i_func, free_func, \
                                     ex_tag, ex_class)                    \
  SKM_ASN1_SET_OF_d2i(GENERAL_NAME, (st), (pp), (length), (d2i_func),     \
                      (free_func), (ex_tag), (ex_class))
#define i2d_ASN1_SET_OF_GENERAL_NAME(st, pp, i2d_func, ex_tag, ex_class, \
                                     is_set)                             \
  SKM_ASN1_SET_OF_i2d(GENERAL_NAME, (st), (pp), (i2d_func), (ex_tag),    \
                      (ex_class), (is_set))
#define ASN1_seq_pack_GENERAL_NAME(st, i2d_func, buf, len) \
  SKM_ASN1_seq_pack(GENERAL_NAME, (st), (i2d_func), (buf), (len))
#define ASN1_seq_unpack_GENERAL_NAME(buf, len, d2i_func, free_func) \
  SKM_ASN1_seq_unpack(GENERAL_NAME, (buf), (len), (d2i_func), (free_func))

#define d2i_ASN1_SET_OF_OCSP_ONEREQ(st, pp, length, d2i_func, free_func, \
                                    ex_tag, ex_class)                    \
  SKM_ASN1_SET_OF_d2i(OCSP_ONEREQ, (st), (pp), (length), (d2i_func),     \
                      (free_func), (ex_tag), (ex_class))
#define i2d_ASN1_SET_OF_OCSP_ONEREQ(st, pp, i2d_func, ex_tag, ex_class, \
                                    is_set)                             \
  SKM_ASN1_SET_OF_i2d(OCSP_ONEREQ, (st), (pp), (i2d_func), (ex_tag),    \
                      (ex_class), (is_set))
#define ASN1_seq_pack_OCSP_ONEREQ(st, i2d_func, buf, len) \
  SKM_ASN1_seq_pack(OCSP_ONEREQ, (st), (i2d_func), (buf), (len))
#define ASN1_seq_unpack_OCSP_ONEREQ(buf, len, d2i_func, free_func) \
  SKM_ASN1_seq_unpack(OCSP_ONEREQ, (buf), (len), (d2i_func), (free_func))

#define d2i_ASN1_SET_OF_OCSP_SINGLERESP(st, pp, length, d2i_func, free_func, \
                                        ex_tag, ex_class)                    \
  SKM_ASN1_SET_OF_d2i(OCSP_SINGLERESP, (st), (pp), (length), (d2i_func),     \
                      (free_func), (ex_tag), (ex_class))
#define i2d_ASN1_SET_OF_OCSP_SINGLERESP(st, pp, i2d_func, ex_tag, ex_class, \
                                        is_set)                             \
  SKM_ASN1_SET_OF_i2d(OCSP_SINGLERESP, (st), (pp), (i2d_func), (ex_tag),    \
                      (ex_class), (is_set))
#define ASN1_seq_pack_OCSP_SINGLERESP(st, i2d_func, buf, len) \
  SKM_ASN1_seq_pack(OCSP_SINGLERESP, (st), (i2d_func), (buf), (len))
#define ASN1_seq_unpack_OCSP_SINGLERESP(buf, len, d2i_func, free_func) \
  SKM_ASN1_seq_unpack(OCSP_SINGLERESP, (buf), (len), (d2i_func), (free_func))

#define d2i_ASN1_SET_OF_PKCS12_SAFEBAG(st, pp, length, d2i_func, free_func, \
                                       ex_tag, ex_class)                    \
  SKM_ASN1_SET_OF_d2i(PKCS12_SAFEBAG, (st), (pp), (length), (d2i_func),     \
                      (free_func), (ex_tag), (ex_class))
#define i2d_ASN1_SET_OF_PKCS12_SAFEBAG(st, pp, i2d_func, ex_tag, ex_class, \
                                       is_set)                             \
  SKM_ASN1_SET_OF_i2d(PKCS12_SAFEBAG, (st), (pp), (i2d_func), (ex_tag),    \
                      (ex_class), (is_set))
#define ASN1_seq_pack_PKCS12_SAFEBAG(st, i2d_func, buf, len) \
  SKM_ASN1_seq_pack(PKCS12_SAFEBAG, (st), (i2d_func), (buf), (len))
#define ASN1_seq_unpack_PKCS12_SAFEBAG(buf, len, d2i_func, free_func) \
  SKM_ASN1_seq_unpack(PKCS12_SAFEBAG, (buf), (len), (d2i_func), (free_func))

#define d2i_ASN1_SET_OF_PKCS7(st, pp, length, d2i_func, free_func, ex_tag,  \
                              ex_class)                                     \
  SKM_ASN1_SET_OF_d2i(PKCS7, (st), (pp), (length), (d2i_func), (free_func), \
                      (ex_tag), (ex_class))
#define i2d_ASN1_SET_OF_PKCS7(st, pp, i2d_func, ex_tag, ex_class, is_set)  \
  SKM_ASN1_SET_OF_i2d(PKCS7, (st), (pp), (i2d_func), (ex_tag), (ex_class), \
                      (is_set))
#define ASN1_seq_pack_PKCS7(st, i2d_func, buf, len) \
  SKM_ASN1_seq_pack(PKCS7, (st), (i2d_func), (buf), (len))
#define ASN1_seq_unpack_PKCS7(buf, len, d2i_func, free_func) \
  SKM_ASN1_seq_unpack(PKCS7, (buf), (len), (d2i_func), (free_func))

#define d2i_ASN1_SET_OF_PKCS7_RECIP_INFO(st, pp, length, d2i_func, free_func, \
                                         ex_tag, ex_class)                    \
  SKM_ASN1_SET_OF_d2i(PKCS7_RECIP_INFO, (st), (pp), (length), (d2i_func),     \
                      (free_func), (ex_tag), (ex_class))
#define i2d_ASN1_SET_OF_PKCS7_RECIP_INFO(st, pp, i2d_func, ex_tag, ex_class, \
                                         is_set)                             \
  SKM_ASN1_SET_OF_i2d(PKCS7_RECIP_INFO, (st), (pp), (i2d_func), (ex_tag),    \
                      (ex_class), (is_set))
#define ASN1_seq_pack_PKCS7_RECIP_INFO(st, i2d_func, buf, len) \
  SKM_ASN1_seq_pack(PKCS7_RECIP_INFO, (st), (i2d_func), (buf), (len))
#define ASN1_seq_unpack_PKCS7_RECIP_INFO(buf, len, d2i_func, free_func) \
  SKM_ASN1_seq_unpack(PKCS7_RECIP_INFO, (buf), (len), (d2i_func), (free_func))

#define d2i_ASN1_SET_OF_PKCS7_SIGNER_INFO(st, pp, length, d2i_func, free_func, \
                                          ex_tag, ex_class)                    \
  SKM_ASN1_SET_OF_d2i(PKCS7_SIGNER_INFO, (st), (pp), (length), (d2i_func),     \
                      (free_func), (ex_tag), (ex_class))
#define i2d_ASN1_SET_OF_PKCS7_SIGNER_INFO(st, pp, i2d_func, ex_tag, ex_class, \
                                          is_set)                             \
  SKM_ASN1_SET_OF_i2d(PKCS7_SIGNER_INFO, (st), (pp), (i2d_func), (ex_tag),    \
                      (ex_class), (is_set))
#define ASN1_seq_pack_PKCS7_SIGNER_INFO(st, i2d_func, buf, len) \
  SKM_ASN1_seq_pack(PKCS7_SIGNER_INFO, (st), (i2d_func), (buf), (len))
#define ASN1_seq_unpack_PKCS7_SIGNER_INFO(buf, len, d2i_func, free_func) \
  SKM_ASN1_seq_unpack(PKCS7_SIGNER_INFO, (buf), (len), (d2i_func), (free_func))

#define d2i_ASN1_SET_OF_POLICYINFO(st, pp, length, d2i_func, free_func, \
                                   ex_tag, ex_class)                    \
  SKM_ASN1_SET_OF_d2i(POLICYINFO, (st), (pp), (length), (d2i_func),     \
                      (free_func), (ex_tag), (ex_class))
#define i2d_ASN1_SET_OF_POLICYINFO(st, pp, i2d_func, ex_tag, ex_class, is_set) \
  SKM_ASN1_SET_OF_i2d(POLICYINFO, (st), (pp), (i2d_func), (ex_tag),            \
                      (ex_class), (is_set))
#define ASN1_seq_pack_POLICYINFO(st, i2d_func, buf, len) \
  SKM_ASN1_seq_pack(POLICYINFO, (st), (i2d_func), (buf), (len))
#define ASN1_seq_unpack_POLICYINFO(buf, len, d2i_func, free_func) \
  SKM_ASN1_seq_unpack(POLICYINFO, (buf), (len), (d2i_func), (free_func))

#define d2i_ASN1_SET_OF_POLICYQUALINFO(st, pp, length, d2i_func, free_func, \
                                       ex_tag, ex_class)                    \
  SKM_ASN1_SET_OF_d2i(POLICYQUALINFO, (st), (pp), (length), (d2i_func),     \
                      (free_func), (ex_tag), (ex_class))
#define i2d_ASN1_SET_OF_POLICYQUALINFO(st, pp, i2d_func, ex_tag, ex_class, \
                                       is_set)                             \
  SKM_ASN1_SET_OF_i2d(POLICYQUALINFO, (st), (pp), (i2d_func), (ex_tag),    \
                      (ex_class), (is_set))
#define ASN1_seq_pack_POLICYQUALINFO(st, i2d_func, buf, len) \
  SKM_ASN1_seq_pack(POLICYQUALINFO, (st), (i2d_func), (buf), (len))
#define ASN1_seq_unpack_POLICYQUALINFO(buf, len, d2i_func, free_func) \
  SKM_ASN1_seq_unpack(POLICYQUALINFO, (buf), (len), (d2i_func), (free_func))

#define d2i_ASN1_SET_OF_SXNETID(st, pp, length, d2i_func, free_func, ex_tag,  \
                                ex_class)                                     \
  SKM_ASN1_SET_OF_d2i(SXNETID, (st), (pp), (length), (d2i_func), (free_func), \
                      (ex_tag), (ex_class))
#define i2d_ASN1_SET_OF_SXNETID(st, pp, i2d_func, ex_tag, ex_class, is_set)  \
  SKM_ASN1_SET_OF_i2d(SXNETID, (st), (pp), (i2d_func), (ex_tag), (ex_class), \
                      (is_set))
#define ASN1_seq_pack_SXNETID(st, i2d_func, buf, len) \
  SKM_ASN1_seq_pack(SXNETID, (st), (i2d_func), (buf), (len))
#define ASN1_seq_unpack_SXNETID(buf, len, d2i_func, free_func) \
  SKM_ASN1_seq_unpack(SXNETID, (buf), (len), (d2i_func), (free_func))

#define d2i_ASN1_SET_OF_X509(st, pp, length, d2i_func, free_func, ex_tag,  \
                             ex_class)                                     \
  SKM_ASN1_SET_OF_d2i(X509, (st), (pp), (length), (d2i_func), (free_func), \
                      (ex_tag), (ex_class))
#define i2d_ASN1_SET_OF_X509(st, pp, i2d_func, ex_tag, ex_class, is_set)  \
  SKM_ASN1_SET_OF_i2d(X509, (st), (pp), (i2d_func), (ex_tag), (ex_class), \
                      (is_set))
#define ASN1_seq_pack_X509(st, i2d_func, buf, len) \
  SKM_ASN1_seq_pack(X509, (st), (i2d_func), (buf), (len))
#define ASN1_seq_unpack_X509(buf, len, d2i_func, free_func) \
  SKM_ASN1_seq_unpack(X509, (buf), (len), (d2i_func), (free_func))

#define d2i_ASN1_SET_OF_X509_ALGOR(st, pp, length, d2i_func, free_func, \
                                   ex_tag, ex_class)                    \
  SKM_ASN1_SET_OF_d2i(X509_ALGOR, (st), (pp), (length), (d2i_func),     \
                      (free_func), (ex_tag), (ex_class))
#define i2d_ASN1_SET_OF_X509_ALGOR(st, pp, i2d_func, ex_tag, ex_class, is_set) \
  SKM_ASN1_SET_OF_i2d(X509_ALGOR, (st), (pp), (i2d_func), (ex_tag),            \
                      (ex_class), (is_set))
#define ASN1_seq_pack_X509_ALGOR(st, i2d_func, buf, len) \
  SKM_ASN1_seq_pack(X509_ALGOR, (st), (i2d_func), (buf), (len))
#define ASN1_seq_unpack_X509_ALGOR(buf, len, d2i_func, free_func) \
  SKM_ASN1_seq_unpack(X509_ALGOR, (buf), (len), (d2i_func), (free_func))

#define d2i_ASN1_SET_OF_X509_ATTRIBUTE(st, pp, length, d2i_func, free_func, \
                                       ex_tag, ex_class)                    \
  SKM_ASN1_SET_OF_d2i(X509_ATTRIBUTE, (st), (pp), (length), (d2i_func),     \
                      (free_func), (ex_tag), (ex_class))
#define i2d_ASN1_SET_OF_X509_ATTRIBUTE(st, pp, i2d_func, ex_tag, ex_class, \
                                       is_set)                             \
  SKM_ASN1_SET_OF_i2d(X509_ATTRIBUTE, (st), (pp), (i2d_func), (ex_tag),    \
                      (ex_class), (is_set))
#define ASN1_seq_pack_X509_ATTRIBUTE(st, i2d_func, buf, len) \
  SKM_ASN1_seq_pack(X509_ATTRIBUTE, (st), (i2d_func), (buf), (len))
#define ASN1_seq_unpack_X509_ATTRIBUTE(buf, len, d2i_func, free_func) \
  SKM_ASN1_seq_unpack(X509_ATTRIBUTE, (buf), (len), (d2i_func), (free_func))

#define d2i_ASN1_SET_OF_X509_CRL(st, pp, length, d2i_func, free_func, ex_tag,  \
                                 ex_class)                                     \
  SKM_ASN1_SET_OF_d2i(X509_CRL, (st), (pp), (length), (d2i_func), (free_func), \
                      (ex_tag), (ex_class))
#define i2d_ASN1_SET_OF_X509_CRL(st, pp, i2d_func, ex_tag, ex_class, is_set)  \
  SKM_ASN1_SET_OF_i2d(X509_CRL, (st), (pp), (i2d_func), (ex_tag), (ex_class), \
                      (is_set))
#define ASN1_seq_pack_X509_CRL(st, i2d_func, buf, len) \
  SKM_ASN1_seq_pack(X509_CRL, (st), (i2d_func), (buf), (len))
#define ASN1_seq_unpack_X509_CRL(buf, len, d2i_func, free_func) \
  SKM_ASN1_seq_unpack(X509_CRL, (buf), (len), (d2i_func), (free_func))

#define d2i_ASN1_SET_OF_X509_EXTENSION(st, pp, length, d2i_func, free_func, \
                                       ex_tag, ex_class)                    \
  SKM_ASN1_SET_OF_d2i(X509_EXTENSION, (st), (pp), (length), (d2i_func),     \
                      (free_func), (ex_tag), (ex_class))
#define i2d_ASN1_SET_OF_X509_EXTENSION(st, pp, i2d_func, ex_tag, ex_class, \
                                       is_set)                             \
  SKM_ASN1_SET_OF_i2d(X509_EXTENSION, (st), (pp), (i2d_func), (ex_tag),    \
                      (ex_class), (is_set))
#define ASN1_seq_pack_X509_EXTENSION(st, i2d_func, buf, len) \
  SKM_ASN1_seq_pack(X509_EXTENSION, (st), (i2d_func), (buf), (len))
#define ASN1_seq_unpack_X509_EXTENSION(buf, len, d2i_func, free_func) \
  SKM_ASN1_seq_unpack(X509_EXTENSION, (buf), (len), (d2i_func), (free_func))

#define d2i_ASN1_SET_OF_X509_NAME_ENTRY(st, pp, length, d2i_func, free_func, \
                                        ex_tag, ex_class)                    \
  SKM_ASN1_SET_OF_d2i(X509_NAME_ENTRY, (st), (pp), (length), (d2i_func),     \
                      (free_func), (ex_tag), (ex_class))
#define i2d_ASN1_SET_OF_X509_NAME_ENTRY(st, pp, i2d_func, ex_tag, ex_class, \
                                        is_set)                             \
  SKM_ASN1_SET_OF_i2d(X509_NAME_ENTRY, (st), (pp), (i2d_func), (ex_tag),    \
                      (ex_class), (is_set))
#define ASN1_seq_pack_X509_NAME_ENTRY(st, i2d_func, buf, len) \
  SKM_ASN1_seq_pack(X509_NAME_ENTRY, (st), (i2d_func), (buf), (len))
#define ASN1_seq_unpack_X509_NAME_ENTRY(buf, len, d2i_func, free_func) \
  SKM_ASN1_seq_unpack(X509_NAME_ENTRY, (buf), (len), (d2i_func), (free_func))

#define d2i_ASN1_SET_OF_X509_REVOKED(st, pp, length, d2i_func, free_func, \
                                     ex_tag, ex_class)                    \
  SKM_ASN1_SET_OF_d2i(X509_REVOKED, (st), (pp), (length), (d2i_func),     \
                      (free_func), (ex_tag), (ex_class))
#define i2d_ASN1_SET_OF_X509_REVOKED(st, pp, i2d_func, ex_tag, ex_class, \
                                     is_set)                             \
  SKM_ASN1_SET_OF_i2d(X509_REVOKED, (st), (pp), (i2d_func), (ex_tag),    \
                      (ex_class), (is_set))
#define ASN1_seq_pack_X509_REVOKED(st, i2d_func, buf, len) \
  SKM_ASN1_seq_pack(X509_REVOKED, (st), (i2d_func), (buf), (len))
#define ASN1_seq_unpack_X509_REVOKED(buf, len, d2i_func, free_func) \
  SKM_ASN1_seq_unpack(X509_REVOKED, (buf), (len), (d2i_func), (free_func))

#define PKCS12_decrypt_d2i_PKCS12_SAFEBAG(algor, d2i_func, free_func, pass, \
                                          passlen, oct, seq)                \
  SKM_PKCS12_decrypt_d2i(PKCS12_SAFEBAG, (algor), (d2i_func), (free_func),  \
                         (pass), (passlen), (oct), (seq))

#define PKCS12_decrypt_d2i_PKCS7(algor, d2i_func, free_func, pass, passlen, \
                                 oct, seq)                                  \
  SKM_PKCS12_decrypt_d2i(PKCS7, (algor), (d2i_func), (free_func), (pass),   \
                         (passlen), (oct), (seq))
/* End of util/mkstack.pl block, you may now edit :-) */

#endif /* !defined HEADER_SAFESTACK_H */
