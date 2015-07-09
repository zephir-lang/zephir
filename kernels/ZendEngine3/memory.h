
/*
  +------------------------------------------------------------------------+
  | Zephir Language                                                        |
  +------------------------------------------------------------------------+
  | Copyright (c) 2011-2015 Zephir Team (http://www.zephir-lang.com)       |
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

#define ZEPHIR_NUM_PREALLOCATED_FRAMES 25

void zephir_initialize_memory(zend_zephir_globals_def *zephir_globals_ptr);
int zephir_cleanup_fcache(void *pDest, int num_args, va_list args, zend_hash_key *hash_key);
void zephir_deinitialize_memory();

/* Memory Frames */
#ifndef ZEPHIR_RELEASE
void ZEPHIR_FASTCALL zephir_memory_grow_stack(const char *func);
int ZEPHIR_FASTCALL zephir_memory_restore_stack(const char *func);

#define ZEPHIR_MM_GROW() zephir_memory_grow_stack(NULL)
#define ZEPHIR_MM_RESTORE() zephir_memory_restore_stack(NULL)

#else
void ZEPHIR_FASTCALL zephir_memory_grow_stack();
int ZEPHIR_FASTCALL zephir_memory_restore_stack();

#define ZEPHIR_MM_GROW() zephir_memory_grow_stack()
#define ZEPHIR_MM_RESTORE() zephir_memory_restore_stack()

#endif

#define zephir_dtor(x) zval_dtor(x)
#define zephir_ptr_dtor(x) zval_ptr_dtor(x)

void ZEPHIR_FASTCALL zephir_memory_observe(zval *var);
void ZEPHIR_FASTCALL zephir_memory_alloc(zval *var);

int ZEPHIR_FASTCALL zephir_clean_restore_stack(TSRMLS_D);

#define ZEPHIR_INIT_VAR(z) { \
		zephir_memory_alloc(&z); \
	} \

#define ZEPHIR_INIT_ZVAL_NREF(z) \
	ZVAL_UNDEF(&z); \

#define ZEPHIR_INIT_NVAR(z)\
	if (Z_REFCOUNTED(z) && !Z_ISREF(z)) { \
		if (Z_REFCOUNT(z) > 1) { \
			Z_DELREF(z); \
		} else { \
			zephir_dtor(&z); \
		} \
		ZVAL_NULL(&z); \
	} \

#define ZEPHIR_CPY_WRT(d, v) \
	Z_TRY_ADDREF_P(v); \
	if (Z_REFCOUNTED_P(d) && Z_REFCOUNT_P(d) > 0) { \
		zephir_ptr_dtor(d); \
	} \
	ZVAL_COPY_VALUE(d, v);

#define ZEPHIR_CPY_WRT_CTOR(d, v) \
	Z_TRY_ADDREF_P(v); \
	if (d) { \
		if (Z_REFCOUNTED_P(d) && Z_REFCOUNT_P(d) > 0) { \
			zephir_ptr_dtor(d); \
		} \
	} else { \
		zephir_memory_observe(d); \
	} \
	ZVAL_DUP(d, v); \
	Z_TRY_DELREF_P(v);

/* TODO: this might causes troubles, since we cannot observe here, since we aren't using double pointers
 * figure out away to fix this (if it's an issue, which it isn't if observing isn't necessary)
 */
#define ZEPHIR_OBSERVE_OR_NULLIFY_PPZV(ppzv) \
	do { \
		zval * restrict tmp_ = (ppzv); \
		if (tmp_ != NULL) { \
			if (Z_TYPE_P(tmp_) != IS_UNDEF) { \
				zephir_ptr_dtor(tmp_); \
				ZVAL_UNDEF(tmp_); \
			} else { \
				zephir_memory_observe(tmp_); \
			} \
		} \
	} while (0)

#endif
