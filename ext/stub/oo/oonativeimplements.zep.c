
#ifdef HAVE_CONFIG_H
#include "../../ext_config.h"
#endif

#include <php.h>
#include "../../php_ext.h"
#include "../../ext.h"

#include <Zend/zend_operators.h>
#include <Zend/zend_exceptions.h>
#include <Zend/zend_interfaces.h>

#include "kernel/main.h"
#include "ext/spl/spl_iterators.h"
#include "kernel/object.h"
#include "kernel/operators.h"
#include "kernel/memory.h"


/**
 * Class with constructor
 */
ZEPHIR_INIT_CLASS(Stub_Oo_OoNativeImplements)
{
	ZEPHIR_REGISTER_CLASS(Stub\\Oo, OoNativeImplements, stub, oo_oonativeimplements, stub_oo_oonativeimplements_method_entry, 0);

	zend_class_implements(stub_oo_oonativeimplements_ce, 1, zend_ce_countable);
	zend_class_implements(stub_oo_oonativeimplements_ce, 1, zend_ce_iterator);
	zend_class_implements(stub_oo_oonativeimplements_ce, 1, spl_ce_OuterIterator);
	zend_class_implements(stub_oo_oonativeimplements_ce, 1, spl_ce_RecursiveIterator);
	zend_class_implements(stub_oo_oonativeimplements_ce, 1, spl_ce_SeekableIterator);
	zend_class_implements(stub_oo_oonativeimplements_ce, 1, zend_ce_serializable);
	return SUCCESS;
}

PHP_METHOD(Stub_Oo_OoNativeImplements, count)
{

}

PHP_METHOD(Stub_Oo_OoNativeImplements, current)
{

}

PHP_METHOD(Stub_Oo_OoNativeImplements, key)
{

}

PHP_METHOD(Stub_Oo_OoNativeImplements, next)
{

}

PHP_METHOD(Stub_Oo_OoNativeImplements, rewind)
{

}

PHP_METHOD(Stub_Oo_OoNativeImplements, valid)
{

}

PHP_METHOD(Stub_Oo_OoNativeImplements, getInnerIterator)
{

}

PHP_METHOD(Stub_Oo_OoNativeImplements, getChildren)
{

}

PHP_METHOD(Stub_Oo_OoNativeImplements, hasChildren)
{

}

PHP_METHOD(Stub_Oo_OoNativeImplements, seek)
{
	zval *position_param = NULL;
	zend_long position;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(position)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &position_param);
}

PHP_METHOD(Stub_Oo_OoNativeImplements, getIterator)
{

}

PHP_METHOD(Stub_Oo_OoNativeImplements, offsetExists)
{
	zval *offset, offset_sub;

	ZVAL_UNDEF(&offset_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(offset)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &offset);
}

PHP_METHOD(Stub_Oo_OoNativeImplements, offsetGet)
{
	zval *offset, offset_sub;

	ZVAL_UNDEF(&offset_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(offset)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &offset);
}

PHP_METHOD(Stub_Oo_OoNativeImplements, offsetSet)
{
	zval *offset, offset_sub, *value, value_sub;

	ZVAL_UNDEF(&offset_sub);
	ZVAL_UNDEF(&value_sub);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(offset)
		Z_PARAM_ZVAL(value)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(2, 0, &offset, &value);
}

PHP_METHOD(Stub_Oo_OoNativeImplements, offsetUnset)
{
	zval *offset, offset_sub;

	ZVAL_UNDEF(&offset_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(offset)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &offset);
}

PHP_METHOD(Stub_Oo_OoNativeImplements, serialize)
{

}

PHP_METHOD(Stub_Oo_OoNativeImplements, unserialize)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *serialized_param = NULL;
	zval serialized;

	ZVAL_UNDEF(&serialized);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(serialized)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &serialized_param);
	zephir_get_strval(&serialized, serialized_param);
}

PHP_METHOD(Stub_Oo_OoNativeImplements, __serialize)
{

	array_init(return_value);
	return;
}

PHP_METHOD(Stub_Oo_OoNativeImplements, __unserialize)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *data_param = NULL;
	zval data;

	ZVAL_UNDEF(&data);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ARRAY(data)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &data_param);
	zephir_get_arrval(&data, data_param);
}

