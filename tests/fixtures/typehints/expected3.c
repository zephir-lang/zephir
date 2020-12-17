
/* This file was generated automatically by Zephir do not modify it! */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <php.h>

#include "php_ext.h"
#include "typehints.h"

#include <ext/standard/info.h>

#include <Zend/zend_operators.h>
#include <Zend/zend_exceptions.h>
#include <Zend/zend_interfaces.h>

#include "kernel/globals.h"
#include "kernel/main.h"
#include "kernel/fcall.h"
#include "kernel/memory.h"



zend_class_entry *typehints_args_ce;
zend_class_entry *typehints_both_ce;
zend_class_entry *typehints_retval_ce;

ZEND_DECLARE_MODULE_GLOBALS(typehints)

PHP_INI_BEGIN()
	
PHP_INI_END()

static PHP_MINIT_FUNCTION(typehints)
{
	REGISTER_INI_ENTRIES();
	zephir_module_init();
	ZEPHIR_INIT(TypeHints_Args);
	ZEPHIR_INIT(TypeHints_Both);
	ZEPHIR_INIT(TypeHints_RetVal);
	
	return SUCCESS;
}

#ifndef ZEPHIR_RELEASE
static PHP_MSHUTDOWN_FUNCTION(typehints)
{
	
	zephir_deinitialize_memory();
	UNREGISTER_INI_ENTRIES();
	return SUCCESS;
}
#endif

/**
 * Initialize globals on each request or each thread started
 */
static void php_zephir_init_globals(zend_typehints_globals *typehints_globals)
{
	typehints_globals->initialized = 0;

	/* Cache Enabled */
	typehints_globals->cache_enabled = 1;

	/* Recursive Lock */
	typehints_globals->recursive_lock = 0;

	/* Static cache */
	memset(typehints_globals->scache, '\0', sizeof(zephir_fcall_cache_entry*) * ZEPHIR_MAX_CACHE_SLOTS);

	
	
}

/**
 * Initialize globals only on each thread started
 */
static void php_zephir_init_module_globals(zend_typehints_globals *typehints_globals)
{
	
}

static PHP_RINIT_FUNCTION(typehints)
{
	zend_typehints_globals *typehints_globals_ptr;
	typehints_globals_ptr = ZEPHIR_VGLOBAL;

	php_zephir_init_globals(typehints_globals_ptr);
	zephir_initialize_memory(typehints_globals_ptr);

	
	return SUCCESS;
}

static PHP_RSHUTDOWN_FUNCTION(typehints)
{
	
	zephir_deinitialize_memory();
	return SUCCESS;
}



static PHP_MINFO_FUNCTION(typehints)
{
	php_info_print_box_start(0);
	php_printf("%s", PHP_TYPEHINTS_DESCRIPTION);
	php_info_print_box_end();

	php_info_print_table_start();
	php_info_print_table_header(2, PHP_TYPEHINTS_NAME, "enabled");
	php_info_print_table_row(2, "Author", PHP_TYPEHINTS_AUTHOR);
	php_info_print_table_row(2, "Version", PHP_TYPEHINTS_VERSION);
	php_info_print_table_row(2, "Build Date", __DATE__ " " __TIME__ );
	php_info_print_table_row(2, "Powered by Zephir", "Version " PHP_TYPEHINTS_ZEPVERSION);
	php_info_print_table_end();
	
	DISPLAY_INI_ENTRIES();
}

static PHP_GINIT_FUNCTION(typehints)
{
#if defined(COMPILE_DL_TYPEHINTS) && defined(ZTS)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif

	php_zephir_init_globals(typehints_globals);
	php_zephir_init_module_globals(typehints_globals);
}

static PHP_GSHUTDOWN_FUNCTION(typehints)
{
	
}

PHP_FUNCTION(f_TypeHints_args);
ZEND_BEGIN_ARG_INFO_EX(arginfo_f_typehints_args, 0, 0, 7)
	ZEND_ARG_INFO(0, _var)
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, _string, IS_STRING, 0)
#else
	ZEND_ARG_INFO(0, _string)
#endif
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, _bool, _IS_BOOL, 0)
#else
	ZEND_ARG_INFO(0, _bool)
#endif
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, _int, IS_LONG, 0)
#else
	ZEND_ARG_INFO(0, _int)
#endif
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, _long, IS_LONG, 0)
#else
	ZEND_ARG_INFO(0, _long)
#endif
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, _double, IS_DOUBLE, 0)
#else
	ZEND_ARG_INFO(0, _double)
#endif
	ZEND_ARG_OBJ_INFO(0, _args, TypeHints\\Args, 0)
ZEND_END_ARG_INFO()

PHP_FUNCTION(f_TypeHints_both);
#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_f_typehints_both, 0, 1, IS_STRING, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_f_typehints_both, 0, 1, IS_STRING, NULL, 0)
#endif
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, _string, IS_STRING, 0)
#else
	ZEND_ARG_INFO(0, _string)
#endif
ZEND_END_ARG_INFO()

PHP_FUNCTION(f_TypeHints_retval_var_var_builit_1);
PHP_FUNCTION(f_TypeHints_retval_var_var_builit_2);
PHP_FUNCTION(f_TypeHints_retval_var);
PHP_FUNCTION(f_TypeHints_retval_string);
#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_f_typehints_retval_string, 0, 0, IS_STRING, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_f_typehints_retval_string, 0, 0, IS_STRING, NULL, 0)
#endif
ZEND_END_ARG_INFO()

PHP_FUNCTION(f_TypeHints_retval_boolean);
#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_f_typehints_retval_boolean, 0, 0, _IS_BOOL, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_f_typehints_retval_boolean, 0, 0, _IS_BOOL, NULL, 0)
#endif
ZEND_END_ARG_INFO()

PHP_FUNCTION(f_TypeHints_retval_int);
#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_f_typehints_retval_int, 0, 0, IS_LONG, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_f_typehints_retval_int, 0, 0, IS_LONG, NULL, 0)
#endif
ZEND_END_ARG_INFO()

PHP_FUNCTION(f_TypeHints_retval_long);
#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_f_typehints_retval_long, 0, 0, IS_LONG, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_f_typehints_retval_long, 0, 0, IS_LONG, NULL, 0)
#endif
ZEND_END_ARG_INFO()

PHP_FUNCTION(f_TypeHints_retval_double);
#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_f_typehints_retval_double, 0, 0, IS_DOUBLE, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_f_typehints_retval_double, 0, 0, IS_DOUBLE, NULL, 0)
#endif
ZEND_END_ARG_INFO()

PHP_FUNCTION(f_TypeHints_retval_retval);
#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_f_typehints_retval_retval, 0, 0, TypeHints\\RetVal, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_f_typehints_retval_retval, 0, 0, IS_OBJECT, "TypeHints\\RetVal", 0)
#endif
ZEND_END_ARG_INFO()

PHP_FUNCTION(f_TypeHints_retval_nullable_int);
#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_f_typehints_retval_nullable_int, 0, 0, IS_LONG, 1)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_f_typehints_retval_nullable_int, 0, 0, IS_LONG, NULL, 1)
#endif
ZEND_END_ARG_INFO()

PHP_FUNCTION(f_TypeHints_retval_nullable_uint);
#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_f_typehints_retval_nullable_uint, 0, 0, IS_LONG, 1)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_f_typehints_retval_nullable_uint, 0, 0, IS_LONG, NULL, 1)
#endif
ZEND_END_ARG_INFO()

PHP_FUNCTION(f_TypeHints_retval_nullable_long);
#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_f_typehints_retval_nullable_long, 0, 0, IS_LONG, 1)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_f_typehints_retval_nullable_long, 0, 0, IS_LONG, NULL, 1)
#endif
ZEND_END_ARG_INFO()

PHP_FUNCTION(f_TypeHints_retval_nullable_float);
#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_f_typehints_retval_nullable_float, 0, 0, IS_DOUBLE, 1)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_f_typehints_retval_nullable_float, 0, 0, IS_DOUBLE, NULL, 1)
#endif
ZEND_END_ARG_INFO()

PHP_FUNCTION(f_TypeHints_retval_nullable_double);
#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_f_typehints_retval_nullable_double, 0, 0, IS_DOUBLE, 1)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_f_typehints_retval_nullable_double, 0, 0, IS_DOUBLE, NULL, 1)
#endif
ZEND_END_ARG_INFO()

PHP_FUNCTION(f_TypeHints_retval_nullable_string);
#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_f_typehints_retval_nullable_string, 0, 0, IS_STRING, 1)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_f_typehints_retval_nullable_string, 0, 0, IS_STRING, NULL, 1)
#endif
ZEND_END_ARG_INFO()

PHP_FUNCTION(f_TypeHints_retval_nullable_boolean);
#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_f_typehints_retval_nullable_boolean, 0, 0, _IS_BOOL, 1)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_f_typehints_retval_nullable_boolean, 0, 0, _IS_BOOL, NULL, 1)
#endif
ZEND_END_ARG_INFO()

PHP_FUNCTION(f_TypeHints_retval_nullable_char);
#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_f_typehints_retval_nullable_char, 0, 0, IS_LONG, 1)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_f_typehints_retval_nullable_char, 0, 0, IS_LONG, NULL, 1)
#endif
ZEND_END_ARG_INFO()

PHP_FUNCTION(f_TypeHints_retval_object_or_scalar);
PHP_FUNCTION(f_TypeHints_retval_array_of_objects);
#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_f_typehints_retval_array_of_objects, 0, 0, IS_ARRAY, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_f_typehints_retval_array_of_objects, 0, 0, IS_ARRAY, NULL, 0)
#endif
ZEND_END_ARG_INFO()


zend_function_entry php_typehints_functions[] = {
	ZEND_NS_NAMED_FE("TypeHints", args, ZEND_FN(f_TypeHints_args), arginfo_f_typehints_args)
ZEND_NS_NAMED_FE("TypeHints", both, ZEND_FN(f_TypeHints_both), arginfo_f_typehints_both)
ZEND_NS_NAMED_FE("TypeHints", retval_var_var_builit_1, ZEND_FN(f_TypeHints_retval_var_var_builit_1), NULL)
ZEND_NS_NAMED_FE("TypeHints", retval_var_var_builit_2, ZEND_FN(f_TypeHints_retval_var_var_builit_2), NULL)
ZEND_NS_NAMED_FE("TypeHints", retval_var, ZEND_FN(f_TypeHints_retval_var), NULL)
ZEND_NS_NAMED_FE("TypeHints", retval_string, ZEND_FN(f_TypeHints_retval_string), arginfo_f_typehints_retval_string)
ZEND_NS_NAMED_FE("TypeHints", retval_boolean, ZEND_FN(f_TypeHints_retval_boolean), arginfo_f_typehints_retval_boolean)
ZEND_NS_NAMED_FE("TypeHints", retval_int, ZEND_FN(f_TypeHints_retval_int), arginfo_f_typehints_retval_int)
ZEND_NS_NAMED_FE("TypeHints", retval_long, ZEND_FN(f_TypeHints_retval_long), arginfo_f_typehints_retval_long)
ZEND_NS_NAMED_FE("TypeHints", retval_double, ZEND_FN(f_TypeHints_retval_double), arginfo_f_typehints_retval_double)
ZEND_NS_NAMED_FE("TypeHints", retval_retval, ZEND_FN(f_TypeHints_retval_retval), arginfo_f_typehints_retval_retval)
ZEND_NS_NAMED_FE("TypeHints", retval_nullable_int, ZEND_FN(f_TypeHints_retval_nullable_int), arginfo_f_typehints_retval_nullable_int)
ZEND_NS_NAMED_FE("TypeHints", retval_nullable_uint, ZEND_FN(f_TypeHints_retval_nullable_uint), arginfo_f_typehints_retval_nullable_uint)
ZEND_NS_NAMED_FE("TypeHints", retval_nullable_long, ZEND_FN(f_TypeHints_retval_nullable_long), arginfo_f_typehints_retval_nullable_long)
ZEND_NS_NAMED_FE("TypeHints", retval_nullable_float, ZEND_FN(f_TypeHints_retval_nullable_float), arginfo_f_typehints_retval_nullable_float)
ZEND_NS_NAMED_FE("TypeHints", retval_nullable_double, ZEND_FN(f_TypeHints_retval_nullable_double), arginfo_f_typehints_retval_nullable_double)
ZEND_NS_NAMED_FE("TypeHints", retval_nullable_string, ZEND_FN(f_TypeHints_retval_nullable_string), arginfo_f_typehints_retval_nullable_string)
ZEND_NS_NAMED_FE("TypeHints", retval_nullable_boolean, ZEND_FN(f_TypeHints_retval_nullable_boolean), arginfo_f_typehints_retval_nullable_boolean)
ZEND_NS_NAMED_FE("TypeHints", retval_nullable_char, ZEND_FN(f_TypeHints_retval_nullable_char), arginfo_f_typehints_retval_nullable_char)
ZEND_NS_NAMED_FE("TypeHints", retval_object_or_scalar, ZEND_FN(f_TypeHints_retval_object_or_scalar), NULL)
ZEND_NS_NAMED_FE("TypeHints", retval_array_of_objects, ZEND_FN(f_TypeHints_retval_array_of_objects), arginfo_f_typehints_retval_array_of_objects)
ZEND_FE_END

};

static const zend_module_dep php_typehints_deps[] = {
	
	ZEND_MOD_END
};

zend_module_entry typehints_module_entry = {
	STANDARD_MODULE_HEADER_EX,
	NULL,
	php_typehints_deps,
	PHP_TYPEHINTS_EXTNAME,
	php_typehints_functions,
	PHP_MINIT(typehints),
#ifndef ZEPHIR_RELEASE
	PHP_MSHUTDOWN(typehints),
#else
	NULL,
#endif
	PHP_RINIT(typehints),
	PHP_RSHUTDOWN(typehints),
	PHP_MINFO(typehints),
	PHP_TYPEHINTS_VERSION,
	ZEND_MODULE_GLOBALS(typehints),
	PHP_GINIT(typehints),
	PHP_GSHUTDOWN(typehints),
#ifdef ZEPHIR_POST_REQUEST
	PHP_PRSHUTDOWN(typehints),
#else
	NULL,
#endif
	STANDARD_MODULE_PROPERTIES_EX
};

/* implement standard "stub" routine to introduce ourselves to Zend */
#ifdef COMPILE_DL_TYPEHINTS
# ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
# endif
ZEND_GET_MODULE(typehints)
#endif
