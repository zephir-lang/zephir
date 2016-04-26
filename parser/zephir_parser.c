/*
  +----------------------------------------------------------------------+
  | PHP Version 7                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2015 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author: Steffen Butzer <steffen.butzer@outlook.com>                  |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_zephir_parser.h"

extern void *xx_parse_program(zval *return_value, char *program, size_t program_length, char *file_path, zval **error_msg);

/* {{{ proto string zephir_parse_file(string arg)
   Return a string to confirm that the module is compiled in */
PHP_FUNCTION(zephir_parse_file)
{
    size_t filepath_len = 0;
    size_t content_len = 0;
	char *content = NULL;
    char *filepath = NULL;
#if PHP_VERSION_ID >= 70000
	zend_array *arr = NULL;
    zval ret;
#else
    zval *ret = NULL;
#endif
    zval **error_msg = NULL;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss", &content, &content_len, &filepath, &filepath_len) == FAILURE) {
		return;
	}

#if PHP_VERSION_ID >= 70000
    xx_parse_program(&ret, content, content_len, filepath, error_msg);
#else
    MAKE_STD_ZVAL(ret);
    xx_parse_program(ret, content, content_len, filepath, error_msg);
#endif

#if PHP_VERSION_ID >= 70000
    RETURN_ZVAL(&ret, 1, 1);
#else
	RETVAL_ZVAL(ret, 1, 0);
#endif
}
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(zephir_parser)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(zephir_parser)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(zephir_parser)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "zephir_parser support", "enabled");
	php_info_print_table_end();
}
/* }}} */

/* {{{ zephir_parser_functions[] */
const zend_function_entry zephir_parser_functions[] = {
	PHP_FE(zephir_parse_file,	NULL)		/* For testing, remove later. */
	PHP_FE_END	/* Must be the last line in zephir_parser_functions[] */
};
/* }}} */

/* {{{ zephir_parser_module_entry
 */
zend_module_entry zephir_parser_module_entry = {
	STANDARD_MODULE_HEADER,
	"zephir_parser",
	zephir_parser_functions,
	PHP_MINIT(zephir_parser),
	PHP_MSHUTDOWN(zephir_parser),
	NULL,
	NULL,
	PHP_MINFO(zephir_parser),
	PHP_ZEPHIR_PARSER_VERSION,
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_ZEPHIR_PARSER
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE();
#endif
ZEND_GET_MODULE(zephir_parser)
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
