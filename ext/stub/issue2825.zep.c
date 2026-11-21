
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
#include "ext/spl/spl_array.h"
#include "kernel/object.h"
#include "kernel/concat.h"
#include "ext/date/php_date.h"
#include "kernel/fcall.h"
#include "kernel/memory.h"


/**
 * `X::class` is the name PHP resolves at compile time: as written, through
 * `use` aliases matched in any case, without a leading backslash and without
 * loading the class. `parent::class` is the `extends` name as written.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2825
 */
ZEPHIR_INIT_CLASS(Stub_Issue2825)
{
	ZEPHIR_REGISTER_CLASS_EX(Stub, Issue2825, stub, issue2825, spl_ce_ArrayObject, stub_issue2825_method_entry, 0);

	zend_declare_property_string(stub_issue2825_ce, SL("propDefault"), "DateTime", ZEND_ACC_PUBLIC);
	zend_declare_property_string(stub_issue2825_ce, SL("staticDefault"), "ArrayObject", ZEND_ACC_PUBLIC|ZEND_ACC_STATIC);
	zephir_declare_class_constant_string(stub_issue2825_ce, SL("GLOBAL_NAME"), "DateTime");

	zephir_declare_class_constant_string(stub_issue2825_ce, SL("ALIAS_NAME"), "ArrayObject");

	zephir_declare_class_constant_string(stub_issue2825_ce, SL("FOLDED_NAME"), "DateTime!");

	return SUCCESS;
}

PHP_METHOD(Stub_Issue2825, imported)
{

	RETURN_STRING("DateTime");
}

PHP_METHOD(Stub_Issue2825, aliased)
{

	RETURN_STRING("ArrayObject");
}

PHP_METHOD(Stub_Issue2825, aliasInOtherCase)
{

	RETURN_STRING("DateTime");
}

PHP_METHOD(Stub_Issue2825, qualified)
{

	RETURN_STRING("DateTime");
}

PHP_METHOD(Stub_Issue2825, importedWithBackslash)
{

	RETURN_STRING("Countable");
}

PHP_METHOD(Stub_Issue2825, keptAsWritten)
{

	RETURN_STRING("datetime");
}

PHP_METHOD(Stub_Issue2825, unknown)
{

	RETURN_STRING("Stub\\NotLoaded");
}

PHP_METHOD(Stub_Issue2825, unknownQualified)
{

	RETURN_STRING("Foo\\bar");
}

PHP_METHOD(Stub_Issue2825, concatenated)
{

	ZEPHIR_CONCAT_SS(return_value, "DateTime", "!");
	if (UNEXPECTED(EG(exception))) {
		return;
	}
	return;
}

PHP_METHOD(Stub_Issue2825, parentName)
{

	RETURN_STRING("ArrayObject");
}

PHP_METHOD(Stub_Issue2825, newAliasInOtherCase)
{
	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;

	ZVAL_UNDEF(&_0);
	ZEND_TLS zephir_method_cache _zephir_call_cache[1];
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	object_init_ex(return_value, php_date_get_date_ce());
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "2020-01-01");
	ZEPHIR_CALL_METHOD_CACHED(NULL, return_value, "__construct", &_zephir_call_cache[0], &_0);
	zephir_check_call_status();
	RETURN_MM();
}

