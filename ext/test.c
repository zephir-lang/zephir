
/* This file was generated automatically by Zephir do not modify it! */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <php.h>

#if PHP_VERSION_ID < 50500
#include <locale.h>
#endif

#include "php_ext.h"
#include "test.h"

#include <ext/standard/info.h>

#include <Zend/zend_operators.h>
#include <Zend/zend_exceptions.h>
#include <Zend/zend_interfaces.h>

#include "kernel/main.h"
#include "kernel/fcall.h"
#include "kernel/memory.h"



zend_class_entry *test_fcall_ce;

ZEND_DECLARE_MODULE_GLOBALS(test)

static PHP_MINIT_FUNCTION(test)
{
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

	ZEPHIR_INIT(Test_Fcall);

#if PHP_VERSION_ID < 50500
	setlocale(LC_ALL, old_lc_all);
	free(old_lc_all);
#endif
	return SUCCESS;
}

#ifndef ZEPHIR_RELEASE
static PHP_MSHUTDOWN_FUNCTION(test)
{

	zephir_deinitialize_memory(TSRMLS_C);

	return SUCCESS;
}
#endif

/**
 * Initialize globals on each request or each thread started
 */
static void php_zephir_init_globals(zend_test_globals *zephir_globals TSRMLS_DC)
{
	zephir_globals->initialized = 0;

	/* Memory options */
	zephir_globals->active_memory = NULL;

	/* Virtual Symbol Tables */
	zephir_globals->active_symbol_table = NULL;

	/* Cache Enabled */
	zephir_globals->cache_enabled = 1;

	/* Recursive Lock */
	zephir_globals->recursive_lock = 0;

	zephir_globals->test.my_setting_1 = 1;
	zephir_globals->test.my_setting_2 = 100;
	zephir_globals->test.my_setting_3 = 7.5;
	zephir_globals->my_setting_1 = 1;
	zephir_globals->my_setting_2 = 10;
	zephir_globals->my_setting_3 = 15.2;

}

static PHP_RINIT_FUNCTION(test)
{

	zend_test_globals *zephir_globals_ptr = ZEPHIR_VGLOBAL;

	php_zephir_init_globals(zephir_globals_ptr TSRMLS_CC);
	//zephir_init_interned_strings(TSRMLS_C);

	zephir_initialize_memory(zephir_globals_ptr TSRMLS_CC);

	return SUCCESS;
}

static PHP_RSHUTDOWN_FUNCTION(test)
{

	

	zephir_deinitialize_memory(TSRMLS_C);
	return SUCCESS;
}

static PHP_MINFO_FUNCTION(test)
{
	php_info_print_box_start(0);
	php_printf("%s", PHP_TEST_DESCRIPTION);
	php_info_print_box_end();

	php_info_print_table_start();
	php_info_print_table_header(2, PHP_TEST_NAME, "enabled");
	php_info_print_table_row(2, "Author", PHP_TEST_AUTHOR);
	php_info_print_table_row(2, "Version", PHP_TEST_VERSION);
	php_info_print_table_row(2, "Powered by Zephir", "Version " PHP_TEST_ZEPVERSION);
	php_info_print_table_end();

	php_info_print_table_start();
	php_info_print_table_header(2, "Directive", "Value");
	php_info_print_table_row(2, "setting1", "value1");
	php_info_print_table_row(2, "setting2", "value2");
	php_info_print_table_end();
	php_info_print_table_start();
	php_info_print_table_header(2, "Directive", "Value");
	php_info_print_table_row(2, "setting3", "value3");
	php_info_print_table_row(2, "setting4", "value4");
	php_info_print_table_end();

}

static PHP_GINIT_FUNCTION(test)
{
	php_zephir_init_globals(test_globals TSRMLS_CC);
}

static PHP_GSHUTDOWN_FUNCTION(test)
{

}

PHP_FUNCTION(Test_zephir_global_method_test);
ZEND_BEGIN_ARG_INFO_EX(arginfo_test_zephir_global_method_test, 0, 0, 1)
	ZEND_ARG_INFO(0, str)
ZEND_END_ARG_INFO()

PHP_FUNCTION(Test_zephir_namespaced_method_test);
ZEND_BEGIN_ARG_INFO_EX(arginfo_test_zephir_namespaced_method_test, 0, 0, 1)
	ZEND_ARG_INFO(0, str)
ZEND_END_ARG_INFO()

PHP_FUNCTION(Test_test_call_relative_object_hint);
ZEND_BEGIN_ARG_INFO_EX(arginfo_test_test_call_relative_object_hint, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, a, Test\\Oo\\PropertyAccess, 0)
ZEND_END_ARG_INFO()

PHP_FUNCTION(Test_test_call_object_hint);
ZEND_BEGIN_ARG_INFO_EX(arginfo_test_test_call_object_hint, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, a, Test\\Oo\\PropertyAccess, 0)
ZEND_END_ARG_INFO()


zend_function_entry php_test_functions[] = {
ZEND_NAMED_FE(zephir_global_method_test, ZEND_FN(Test_zephir_global_method_test), arginfo_test_zephir_global_method_test)
ZEND_NS_NAMED_FE("Test", zephir_namespaced_method_test, ZEND_FN(Test_zephir_namespaced_method_test), arginfo_test_zephir_namespaced_method_test)
ZEND_NS_NAMED_FE("Test", test_call_relative_object_hint, ZEND_FN(Test_test_call_relative_object_hint), arginfo_test_test_call_relative_object_hint)
ZEND_NS_NAMED_FE("Test", test_call_object_hint, ZEND_FN(Test_test_call_object_hint), arginfo_test_test_call_object_hint)
ZEND_FE_END

};

zend_module_entry test_module_entry = {
	STANDARD_MODULE_HEADER_EX,
	NULL,
	NULL,
	PHP_TEST_EXTNAME,
	php_test_functions,
	PHP_MINIT(test),
#ifndef ZEPHIR_RELEASE
	PHP_MSHUTDOWN(test),
#else
	NULL,
#endif
	PHP_RINIT(test),
	PHP_RSHUTDOWN(test),
	PHP_MINFO(test),
	PHP_TEST_VERSION,
	ZEND_MODULE_GLOBALS(test),
	PHP_GINIT(test),
	PHP_GSHUTDOWN(test),
	NULL,
	STANDARD_MODULE_PROPERTIES_EX
};

#ifdef COMPILE_DL_TEST
ZEND_GET_MODULE(test)
#endif
