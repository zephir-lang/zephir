
/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013-2015 Zephir Team and contributors                          |
 +--------------------------------------------------------------------------+
 | This source file is subject the MIT license, that is bundled with        |
 | this package in the file LICENSE, and is available through the           |
 | world-wide-web at the following url:                                     |
 | http://zephir-lang.com/license.html                                      |
 |                                                                          |
 | If you did not receive a copy of the MIT license and are unable          |
 | to obtain it through the world-wide-web, please send a note to           |
 | license@zephir-lang.com so we can mail you a copy immediately.           |
 +--------------------------------------------------------------------------+
*/

#ifndef PHP_ZEPHIR_RUNTIME_H
#define PHP_ZEPHIR_RUNTIME_H 1

#include <llvm-c/Analysis.h>
#include <llvm-c/Target.h>
#include <llvm-c/Transforms/Scalar.h>
#include <llvm-c/ExecutionEngine.h>

#define ZEPHIR_RELEASE 1
#define PHP_ZEPHIR_VERSION "0.0.1"
#define PHP_ZEPHIR_EXTNAME "zephir"

/** Memory frame */
typedef struct _zephir_memory_entry {
	size_t pointer;
	size_t capacity;
	zval ***addresses;
	size_t hash_pointer;
	size_t hash_capacity;
	zval ***hash_addresses;
	struct _zephir_memory_entry *prev;
	struct _zephir_memory_entry *next;
#ifndef ZEPHIR_RELEASE
	const char *func;
#endif
} zephir_memory_entry;

/** Virtual Symbol Table */
typedef struct _zephir_symbol_table {
	struct _zephir_memory_entry *scope;
	HashTable *symbol_table;
	struct _zephir_symbol_table *prev;
} zephir_symbol_table;

typedef struct _zephir_function_cache {
	zend_class_entry *ce;
	zend_function *func;
} zephir_function_cache;

ZEND_BEGIN_MODULE_GLOBALS(zephir)

	/* Memory */
	zephir_memory_entry *start_memory; /**< The first preallocated frame */
	zephir_memory_entry *end_memory; /**< The last preallocate frame */
	zephir_memory_entry *active_memory; /**< The current memory frame */

	/* Virtual Symbol Tables */
	zephir_symbol_table *active_symbol_table;

	/** Function cache */
	HashTable *fcache;

	/* Max recursion control */
	unsigned int recursive_lock;

	LLVMModuleRef module;
    LLVMBuilderRef builder;
    LLVMExecutionEngineRef engine;
    LLVMPassManagerRef  pass_manager;

ZEND_END_MODULE_GLOBALS(zephir)

#ifdef ZTS
#include "TSRM.h"
#endif

ZEND_EXTERN_MODULE_GLOBALS(zephir)

#ifdef ZTS
	#define ZEPHIRT_GLOBAL(v) TSRMG(zephir_globals_id, zend_zephir_globals *, v)
#else
	#define ZEPHIRT_GLOBAL(v) (zephir_globals.v)
#endif

#ifdef ZTS
	#define ZEPHIRT_VGLOBAL ((zend_zephir_globals *) (*((void ***) tsrm_ls))[TSRM_UNSHUFFLE_RSRC_ID(zend_globals_id)])
#else
	#define ZEPHIRT_VGLOBAL &(zephir_globals)
#endif

#if defined(__GNUC__) && (defined(__clang__) || ((__GNUC__ * 100 + __GNUC_MINOR__) >= 405))
# define UNREACHABLE() __builtin_unreachable()
# define ASSUME(x) if (x) {} else __builtin_unreachable()
#else
# define UNREACHABLE() assert(0)
# define ASSUME(x) assert(!!(x));
#endif

/** Define FASTCALL */
#if defined(__GNUC__) && ZEND_GCC_VERSION >= 3004 && defined(__i386__)
# define ZEPHIR_FASTCALL __attribute__((fastcall))
#elif defined(_MSC_VER) && defined(_M_IX86)
# define ZEPHIR_FASTCALL __fastcall
#else
# define ZEPHIR_FASTCALL
#endif

#if defined(__GNUC__) || defined(__clang__)
# define ZEPHIR_ATTR_NONNULL __attribute__((nonnull))
# define ZEPHIR_ATTR_NONNULL1(x) __attribute__((nonnull (x)))
# define ZEPHIR_ATTR_NONNULL2(x, y) __attribute__((nonnull (x, y)))
# define ZEPHIR_ATTR_NONNULL3(x, y, z) __attribute__((nonnull (x, y, z)))
# define ZEPHIR_ATTR_PURE __attribute__((pure))
# define ZEPHIR_ATTR_CONST __attribute__((const))
# define ZEPHIR_ATTR_WARN_UNUSED_RESULT __attribute__((warn_unused_result))
#else
# define ZEPHIR_ATTR_NONNULL
# define ZEPHIR_ATTR_NONNULL1(x)
# define ZEPHIR_ATTR_NONNULL2(x, y)
# define ZEPHIR_ATTR_NONNULL3(x, y, z)
# define ZEPHIR_ATTR_PURE
# define ZEPHIR_ATTR_CONST
# define ZEPHIR_ATTR_WARN_UNUSED_RESULT
#endif

#if defined(__x86_64__) || defined(_M_X64)
#define ZEPHIR_64 1
#define ZEPHIR_32 0
#elif defined(__i386) || defined(_M_IX86)
#define ZEPHIR_64 0
#define ZEPHIR_32 1
#endif

extern zend_module_entry zephir_module_entry;
#define phpext_zephir_ptr &zephir_module_entry

#endif
