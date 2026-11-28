
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
#include "ext/pdo/php_pdo_driver.h"
#include "ext/spl/spl_directory.h"
#include "kernel/memory.h"
#include "kernel/object.h"
#include "kernel/array.h"
#include "kernel/operators.h"


/**
 * Constants of PHP classes whose values depend on the PHP that compiles the
 * extension, not on the one that ran `zephir generate` (#2869). PHP 8.5
 * renumbered the `PDO::FETCH_*` flags and PHP 8.1 the `FilesystemIterator`
 * flags. Any other constant of a PHP class is read when the code runs.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2869
 */
ZEPHIR_INIT_CLASS(Stub_Issue2869)
{
	ZEPHIR_REGISTER_CLASS(Stub, Issue2869, stub, issue2869, stub_issue2869_method_entry, 0);

	zend_declare_property_long(stub_issue2869_ce, SL("serialize"), ((zend_long) PDO_FETCH_SERIALIZE), ZEND_ACC_PUBLIC);
	{
		zval _zc0;
		ZVAL_LONG(&_zc0, SPL_FILE_DIR_OTHERS_MASK);
		zephir_declare_typed_property(stub_issue2869_ce, SL("mask"), &_zc0, ZEND_ACC_PUBLIC, MAY_BE_LONG, NULL, 0);
	}

	zephir_declare_class_constant_long(stub_issue2869_ce, SL("GROUP"), ((zend_long) PDO_FETCH_GROUP));

	zephir_declare_class_constant_long(stub_issue2869_ce, SL("UNIQUE_ASSOC"), 2 | ((zend_long) PDO_FETCH_UNIQUE));

	{
		zval _zc0;
		array_init_size(&_zc0, 4);
		add_next_index_long(&_zc0, ((zend_long) PDO_FETCH_CLASSTYPE));
		add_assoc_long_ex(&_zc0, SL("late"), ((zend_long) PDO_FETCH_PROPS_LATE));
		add_assoc_long_ex(&_zc0, SL("serialize"), ((zend_long) PDO_FETCH_SERIALIZE));
		zephir_declare_class_constant_array(stub_issue2869_ce, SL("LIST"), &_zc0);
	}

	zephir_declare_class_constant_long(stub_issue2869_ce, SL("SYMLINKS"), SPL_FILE_DIR_FOLLOW_SYMLINKS);

	zephir_declare_class_constant_long(stub_issue2869_ce, SL("MASK"), SPL_FILE_DIR_OTHERS_MASK);

	return SUCCESS;
}

PHP_METHOD(Stub_Issue2869, withDefault)
{
	zval *flags_param = NULL;
	zend_long flags = ((zend_long) PDO_FETCH_GROUP);

	ZEND_PARSE_PARAMETERS_START(0, 1)
		Z_PARAM_OPTIONAL
		Z_PARAM_LONG(flags)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(0, 1, &flags_param);
	RETURN_LONG(flags);
}

PHP_METHOD(Stub_Issue2869, grouped)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *flags_param = NULL, _0;
	zend_long flags;

	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(flags)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &flags_param);
	zephir_create_array(return_value, 1, 0);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, (flags | ((zend_long) PDO_FETCH_GROUP)));
	zephir_array_fast_append(return_value, &_0);
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2869, inverted)
{

	RETURN_LONG(~(((zend_long) PDO_FETCH_GROUP)));
}

PHP_METHOD(Stub_Issue2869, unique)
{

	RETURN_LONG(((zend_long) PDO_FETCH_UNIQUE));
}

PHP_METHOD(Stub_Issue2869, isGroup)
{
	zval *mode_param = NULL;
	zend_long mode;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(mode)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &mode_param);
	RETURN_BOOL(((mode & ((zend_long) PDO_FETCH_GROUP))) == ((zend_long) PDO_FETCH_GROUP));
}

PHP_METHOD(Stub_Issue2869, symlinks)
{

	RETURN_LONG(SPL_FILE_DIR_FOLLOW_SYMLINKS);
}

PHP_METHOD(Stub_Issue2869, assoc)
{

	zephir_get_class_constant(return_value, zephir_fetch_class_str_ex(SL("PDO"), ZEND_FETCH_CLASS_DEFAULT | ZEND_FETCH_CLASS_EXCEPTION), SL("FETCH_ASSOC"));
	if (UNEXPECTED(EG(exception))) {
		return;
	}
	return;
}

PHP_METHOD(Stub_Issue2869, props)
{
	zval flags;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&flags);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&flags);
	zephir_get_class_constant(&flags, zephir_fetch_class_str_ex(SL("ArrayObject"), ZEND_FETCH_CLASS_DEFAULT | ZEND_FETCH_CLASS_EXCEPTION), SL("ARRAY_AS_PROPS"));
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_CCTOR(&flags);
}

