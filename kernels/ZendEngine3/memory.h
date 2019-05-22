
/*
  +------------------------------------------------------------------------+
  | Zephir Language                                                        |
  +------------------------------------------------------------------------+
  | Copyright (c) 2011-2017 Zephir Team (http://www.zephir-lang.com)       |
  +------------------------------------------------------------------------+
  | This source file is subject to the New BSD License that is bundled     |
  | with this package in the file docs/LICENSE.txt.                        |
  |                                                                        |
  | If you did not receive a copy of the license and are unable to         |
  | obtain it through the world-wide-web, please send an email             |
  | to license@zephir-lang.com so we can send you a copy immediately.      |
  +------------------------------------------------------------------------+
  | Authors: Andres Gutierrez <andres@zephir-lang.com>                     |
  |          Eduar Carvajal <eduar@zephir-lang.com>                        |
  |          Vladimir Kolesnikov <vladimir@extrememember.com>              |
  +------------------------------------------------------------------------+
*/

#ifndef ZEPHIR_KERNEL_MEMORY_H
#define ZEPHIR_KERNEL_MEMORY_H

#include <php.h>
#include <Zend/zend.h>
#include "php_ext.h"
#include "kernel/globals.h"
#include "kernel/debug.h"

/* Backwards compatibility for GC API change in PHP 7.3 */
#if PHP_VERSION_ID < 70300
#  define GC_ADDREF(p)            ++GC_REFCOUNT(p)
#  define GC_DELREF(p)            --GC_REFCOUNT(p)
#  define GC_SET_REFCOUNT(p, rc)  GC_REFCOUNT(p) = rc
#endif

typedef struct _zephir_memory_entry {
	zval val;
	struct _zephir_memory_entry *prev;
} zephir_memory_entry;

/** Virtual Symbol Table */
typedef struct _zephir_symbol_table {
	zend_array *symbol_table;
	struct _zephir_symbol_table *prev;
} zephir_symbol_table;

typedef struct _zephir_memory_def {
	zephir_memory_entry *active_entry;
	zephir_symbol_table *active_symbol_table;
} zephir_memory_def;

void zephir_memory_grow_stack(zephir_memory_def *zephir_memory_ptr);
void zephir_memory_observe(zephir_memory_def *zephir_memory_ptr, zval *var);
void zephir_memory_restore_stack(zephir_memory_def *zephir_memory_ptr);
int zephir_cleanup_fcache(void *pDest, int num_args, va_list args, zend_hash_key *hash_key);

/* Memory Frames */
#define ZEPHIR_MM_GROW() \
	zephir_memory_def zephir_memory; \
	zephir_memory_grow_stack(&zephir_memory);

#define ZEPHIR_MM_ADD_ENTRY(z) \
	zephir_memory_observe(&zephir_memory, z);

#define ZEPHIR_MM_RESTORE() zephir_memory_restore_stack(&zephir_memory)

#define ZEPHIR_MM_ZVAL_EMPTY_STRING(z) \
	do { \
		ZVAL_EMPTY_STRING(z); \
		zephir_memory_observe(&zephir_memory, z); \
	} while (0)

#define ZEPHIR_MM_ZVAL_STRING(z, s) \
	do { \
		ZVAL_STRING(z, s); \
		zephir_memory_observe(&zephir_memory, z); \
	} while (0)

#define ZEPHIR_MM_ZVAL_STRINGL(z, s, l) \
	do { \
		ZVAL_STRINGL(z, s, l); \
		zephir_memory_observe(&zephir_memory, z); \
	} while (0)

#define zephir_dtor(x) zval_dtor(x)
#define zephir_ptr_dtor(x) zval_ptr_dtor(x)

#define ZEPHIR_INIT_ZVAL_NREF(z) \
	ZVAL_UNDEF(&z); \

#define ZEPHIR_INIT_VAR(z) \
	ZVAL_NULL(z);

#define ZEPHIR_CPY_WRT(d, v) \
	ZVAL_COPY_VALUE(d, v);

#define ZEPHIR_SEPARATE(z) \
	do { \
		zval *_zv = (z); \
		if (Z_TYPE_P(_zv) == IS_ARRAY) { \
			ZVAL_ARR(_zv, zend_array_dup(Z_ARR_P(_zv))); \
		} else if (Z_ISREF_P(_zv)) { \
			ZVAL_DUP(_zv, Z_REFVAL_P(_zv)); \
		} else if (Z_REFCOUNTED_P(_zv)) { \
			Z_ADDREF_P(_zv); \
		} \
	} while (0)

#define ZEPHIR_SEPARATE_PARAM(z) \
	do { \
		zval *orig_ptr = z;\
		SEPARATE_ARG_IF_REF(orig_ptr); \
	} while (0)

void zephir_create_symbol_table(zephir_memory_def *zephir_memory_ptr);
int zephir_set_symbol(zval *key_name, zval *value);
int zephir_set_symbol_str(char *key_name, unsigned int key_length, zval *value);

#endif
