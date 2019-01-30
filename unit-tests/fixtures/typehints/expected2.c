
/* This file was generated automatically by Zephir do not modify it! */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <php.h>

// TODO: Deprecated. Will be removed in future
#if PHP_VERSION_ID < 50500
#include <locale.h>
#endif

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
// TODO: Deprecated. Will be removed in future
#if PHP_VERSION_ID < 50500
	char* old_lc_all = setlocale(LC_ALL, NULL);
	if (old_lc_all) {
		size_t len = strlen(old_lc_all);
		char *tmp  = calloc(len+1, 1);
		if (UNEXPECTED(!tmp)) {
			return FAILURE;
		}

		memcpy(tmp, old_lc_all, len);
		old_lc_all = tmp;
	}

	setlocale(LC_ALL, "C");
#endif
	REGISTER_INI_ENTRIES();
	ZEPHIR_INIT(TypeHints_Args);
	ZEPHIR_INIT(TypeHints_Both);
	ZEPHIR_INIT(TypeHints_RetVal);
	

// TODO: Deprecated. Will be removed in future
#if PHP_VERSION_ID < 50500
	setlocale(LC_ALL, old_lc_all);
	free(old_lc_all);
#endif
	return SUCCESS;
}

#ifndef ZEPHIR_RELEASE
static PHP_MSHUTDOWN_FUNCTION(typehints)
{
	
	zephir_deinitialize_memory(TSRMLS_C);
	UNREGISTER_INI_ENTRIES();
	return SUCCESS;
}
#endif

/**
 * Initialize globals on each request or each thread started
 */
static void php_zephir_init_globals(zend_typehints_globals *typehints_globals TSRMLS_DC)
{
	typehints_globals->initialized = 0;

	/* Memory options */
	typehints_globals->active_memory = NULL;

	/* Virtual Symbol Tables */
	typehints_globals->active_symbol_table = NULL;

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
static void php_zephir_init_module_globals(zend_typehints_globals *typehints_globals TSRMLS_DC)
{
	
}

static PHP_RINIT_FUNCTION(typehints)
{
	zend_typehints_globals *typehints_globals_ptr = ZEPHIR_VGLOBAL;

	php_zephir_init_globals(typehints_globals_ptr TSRMLS_CC);
	//zephir_init_interned_strings(TSRMLS_C);
	zephir_initialize_memory(typehints_globals_ptr TSRMLS_CC);

	
	return SUCCESS;
}

static PHP_RSHUTDOWN_FUNCTION(typehints)
{
	
	zephir_deinitialize_memory(TSRMLS_C);
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
	php_zephir_init_globals(typehints_globals TSRMLS_CC);
	php_zephir_init_module_globals(typehints_globals TSRMLS_CC);
}

static PHP_GSHUTDOWN_FUNCTION(typehints)
{
	
}

PHP_FUNCTION(f_TypeHints_args);
ZEND_BEGIN_ARG_INFO_EX(arginfo_f_typehints_args, 0, 0, 7)
	ZEND_ARG_INFO(0, _var)
	ZEND_ARG_INFO(0, _string)
	ZEND_ARG_INFO(0, _bool)
	ZEND_ARG_INFO(0, _int)
	ZEND_ARG_INFO(0, _long)
	ZEND_ARG_INFO(0, _double)
	ZEND_ARG_OBJ_INFO(0, _args, TypeHints\\Args, 0)
ZEND_END_ARG_INFO()

PHP_FUNCTION(f_TypeHints_both);
ZEND_BEGIN_ARG_INFO_EX(arginfo_f_typehints_both, 0, 0, 1)
	ZEND_ARG_INFO(0, _string)
ZEND_END_ARG_INFO()

PHP_FUNCTION(f_TypeHints_retval_var_var_builit_1);
PHP_FUNCTION(f_TypeHints_retval_var_var_builit_2);
PHP_FUNCTION(f_TypeHints_retval_var);
PHP_FUNCTION(f_TypeHints_retval_string);
PHP_FUNCTION(f_TypeHints_retval_boolean);
PHP_FUNCTION(f_TypeHints_retval_int);
PHP_FUNCTION(f_TypeHints_retval_long);
PHP_FUNCTION(f_TypeHints_retval_double);
PHP_FUNCTION(f_TypeHints_retval_retval);
PHP_FUNCTION(f_TypeHints_retval_nullable_int);
PHP_FUNCTION(f_TypeHints_retval_nullable_uint);
PHP_FUNCTION(f_TypeHints_retval_nullable_long);
PHP_FUNCTION(f_TypeHints_retval_nullable_float);
PHP_FUNCTION(f_TypeHints_retval_nullable_double);
PHP_FUNCTION(f_TypeHints_retval_nullable_string);
PHP_FUNCTION(f_TypeHints_retval_nullable_boolean);
PHP_FUNCTION(f_TypeHints_retval_nullable_char);
PHP_FUNCTION(f_TypeHints_retval_object_or_scalar);
PHP_FUNCTION(f_TypeHints_retval_array_of_objects);

zend_function_entry php_typehints_functions[] = {
	ZEND_NS_NAMED_FE("TypeHints", args, ZEND_FN(f_TypeHints_args), arginfo_f_typehints_args)
ZEND_NS_NAMED_FE("TypeHints", both, ZEND_FN(f_TypeHints_both), arginfo_f_typehints_both)
ZEND_NS_NAMED_FE("TypeHints", retval_var_var_builit_1, ZEND_FN(f_TypeHints_retval_var_var_builit_1), NULL)
ZEND_NS_NAMED_FE("TypeHints", retval_var_var_builit_2, ZEND_FN(f_TypeHints_retval_var_var_builit_2), NULL)
ZEND_NS_NAMED_FE("TypeHints", retval_var, ZEND_FN(f_TypeHints_retval_var), NULL)
ZEND_NS_NAMED_FE("TypeHints", retval_string, ZEND_FN(f_TypeHints_retval_string), NULL)
ZEND_NS_NAMED_FE("TypeHints", retval_boolean, ZEND_FN(f_TypeHints_retval_boolean), NULL)
ZEND_NS_NAMED_FE("TypeHints", retval_int, ZEND_FN(f_TypeHints_retval_int), NULL)
ZEND_NS_NAMED_FE("TypeHints", retval_long, ZEND_FN(f_TypeHints_retval_long), NULL)
ZEND_NS_NAMED_FE("TypeHints", retval_double, ZEND_FN(f_TypeHints_retval_double), NULL)
ZEND_NS_NAMED_FE("TypeHints", retval_retval, ZEND_FN(f_TypeHints_retval_retval), NULL)
ZEND_NS_NAMED_FE("TypeHints", retval_nullable_int, ZEND_FN(f_TypeHints_retval_nullable_int), NULL)
ZEND_NS_NAMED_FE("TypeHints", retval_nullable_uint, ZEND_FN(f_TypeHints_retval_nullable_uint), NULL)
ZEND_NS_NAMED_FE("TypeHints", retval_nullable_long, ZEND_FN(f_TypeHints_retval_nullable_long), NULL)
ZEND_NS_NAMED_FE("TypeHints", retval_nullable_float, ZEND_FN(f_TypeHints_retval_nullable_float), NULL)
ZEND_NS_NAMED_FE("TypeHints", retval_nullable_double, ZEND_FN(f_TypeHints_retval_nullable_double), NULL)
ZEND_NS_NAMED_FE("TypeHints", retval_nullable_string, ZEND_FN(f_TypeHints_retval_nullable_string), NULL)
ZEND_NS_NAMED_FE("TypeHints", retval_nullable_boolean, ZEND_FN(f_TypeHints_retval_nullable_boolean), NULL)
ZEND_NS_NAMED_FE("TypeHints", retval_nullable_char, ZEND_FN(f_TypeHints_retval_nullable_char), NULL)
ZEND_NS_NAMED_FE("TypeHints", retval_object_or_scalar, ZEND_FN(f_TypeHints_retval_object_or_scalar), NULL)
ZEND_NS_NAMED_FE("TypeHints", retval_array_of_objects, ZEND_FN(f_TypeHints_retval_array_of_objects), NULL)
ZEND_FE_END

};

zend_module_entry typehints_module_entry = {
	STANDARD_MODULE_HEADER_EX,
	NULL,
	NULL,
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

#ifdef COMPILE_DL_TYPEHINTS
ZEND_GET_MODULE(typehints)
#endif
