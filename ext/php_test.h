
#ifndef PHP_TEST_H
#define PHP_TEST_H 1

#define PHP_TEST_VERSION "0.0.1"
#define PHP_TEST_EXTNAME "test"

#define PHALCON_MAX_MEMORY_STACK 48

/** Memory frame */
typedef struct _test_memory_entry {
	int pointer;
	zval ***addresses;
	int hash_pointer;
	zval ***hash_addresses;
	struct _test_memory_entry *prev;
	struct _test_memory_entry *next;
} test_memory_entry;

/** Virtual Symbol Table */
typedef struct _test_symbol_table {
	struct _test_memory_entry *scope;
	HashTable *symbol_table;
	struct _test_symbol_table *prev;
} test_symbol_table;

ZEND_BEGIN_MODULE_GLOBALS(test)

	/** Memory */
	test_memory_entry *start_memory;
	test_memory_entry *active_memory;

	/** Virtual Symbol Tables */
	test_symbol_table *active_symbol_table;

	/** Function cache */
	HashTable *function_cache;

	/** Max recursion control */
	unsigned int recursive_lock;

ZEND_END_MODULE_GLOBALS(test)

#ifdef ZTS
#include "TSRM.h"
#endif

ZEND_EXTERN_MODULE_GLOBALS(test)

//#ifdef ZTS
//	#define PHALCON_GLOBAL(v) TSRMG(test_globals_id, zend_phalcon_globals *, v)
//#else
//	#define PHALCON_GLOBAL(v) (test_globals.v)
//#endif

//#ifdef ZTS
//	#define PHALCON_VGLOBAL ((zend_phalcon_globals *) (*((void ***) tsrm_ls))[TSRM_UNSHUFFLE_RSRC_ID(phalcon_globals_id)])
//#else
//	#define PHALCON_VGLOBAL &(phalcon_globals)
//#endif

extern zend_module_entry test_module_entry;
#define phpext_test_ptr &test_module_entry

#endif

#if PHP_VERSION_ID >= 50400
	#define TEST_INIT_FUNCS(class_functions) static const zend_function_entry class_functions[] =
#else
	#define TEST_INIT_FUNCS(class_functions) static const function_entry class_functions[] =
#endif

#ifndef PHP_FE_END
	#define PHP_FE_END { NULL, NULL, NULL, 0, 0 }
#endif

/** Define FASTCALL */
#if defined(__GNUC__) && ZEND_GCC_VERSION >= 3004 && defined(__i386__)
# define TEST_FASTCALL __attribute__((fastcall))
#elif defined(_MSC_VER) && defined(_M_IX86)
# define TEST_FASTCALL __fastcall
#else
# define TEST_FASTCALL
#endif

#define TEST_INIT_CLASS(name) \
	int test_ ##name## _init(INIT_FUNC_ARGS)

#define TEST_INIT(name) \
	if (test_ ##name## _init(INIT_FUNC_ARGS_PASSTHRU) == FAILURE) { \
		return FAILURE; \
	}

/** Macros for branch prediction */
#if defined(__GNUC__) && ZEND_GCC_VERSION >= 3004 && defined(__i386__)
#define likely(x)       __builtin_expect((x), 1)
#define unlikely(x)     __builtin_expect((x), 0)
#else
#define likely(x)       EXPECTED(x)
#define unlikely(x)     UNEXPECTED(x)
#endif

#if defined(__GNUC__) && (defined(__clang__) || ((__GNUC__ * 100 + __GNUC_MINOR__) >= 405))
#define UNREACHABLE() __builtin_unreachable()
#define ASSUME(x)     if (x) {} else __builtin_unreachable()
#else
#define UNREACHABLE() assert(0)
#define ASSUME(x)     assert(!!(x));
#endif
