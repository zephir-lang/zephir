
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
#include "kernel/memory.h"
#include "kernel/iterator.h"
#include "kernel/array.h"
#include "kernel/object.h"


ZEPHIR_INIT_CLASS(Stub_Issue820)
{
	ZEPHIR_REGISTER_CLASS(Stub, Issue820, stub, issue820, stub_issue820_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_Issue820, testIterator)
{
	zend_object_iterator *_0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *obj, obj_sub, k, v, arr;

	ZVAL_NULL(&obj_sub);
	ZVAL_NULL(&k);
	ZVAL_NULL(&v);
	ZVAL_NULL(&arr);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(obj)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &obj);
	ZEPHIR_INIT_VAR(&arr);
	array_init(&arr);
	_0 = zephir_get_iterator(obj);
	if (EXPECTED(_0 != NULL)) {
		_0->funcs->rewind(_0);
		for (;_0->funcs->valid(_0) == SUCCESS && !EG(exception); _0->funcs->move_forward(_0)) {
		ZEPHIR_GET_IMKEY(k, _0);
		{
			ZEPHIR_ITERATOR_COPY(&v, _0);
		}
		zephir_array_update_zval(&arr, &k, &v, PH_COPY | PH_SEPARATE);
	}
	zend_iterator_dtor(_0);
	}
	RETURN_CCTOR(&arr);
}

