
#ifdef HAVE_CONFIG_H
#include "../ext_config.h"
#endif

#include <php.h>
#include "../php_ext.h"
#include "../ext.h"

#include <Zend/zend_operators.h>
#include <Zend/zend_exceptions.h>
#include <Zend/zend_interfaces.h>

#include "kernel/main.h"
#include "ext/spl/spl_directory.h"
#include "kernel/fcall.h"
#include "kernel/operators.h"
#include "kernel/memory.h"
#include "kernel/object.h"


ZEPHIR_INIT_CLASS(Stub_Spl)
{
	ZEPHIR_REGISTER_CLASS(Stub, Spl, stub, spl, stub_spl_method_entry, 0);

	return SUCCESS;
}

/**
 * @issue https://github.com/zephir-lang/zephir/issues/1212
 */
PHP_METHOD(Stub_Spl, issue1212)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *file_param = NULL;
	zval file;

	ZVAL_UNDEF(&file);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(file)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &file_param);
	zephir_get_strval(&file, file_param);
	object_init_ex(return_value, spl_ce_SplFileObject);
	ZEPHIR_CALL_METHOD(NULL, return_value, "__construct", NULL, 102, &file);
	zephir_check_call_status();
	RETURN_MM();
}

