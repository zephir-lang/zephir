
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


ZEPHIR_INIT_CLASS(Stub_Issue820) {

	ZEPHIR_REGISTER_CLASS(Stub, Issue820, stub, issue820, stub_issue820_method_entry, 0);

	return SUCCESS;

}

/**
 * @issue https://github.com/zephir-lang/zephir/issues/820
 */
PHP_METHOD(Stub_Issue820, iterate) {

	zend_object_iterator *_0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *obj, obj_sub, k, v, arr;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&obj_sub);
	ZVAL_UNDEF(&k);
	ZVAL_UNDEF(&v);
	ZVAL_UNDEF(&arr);
#if PHP_VERSION_ID >= 80000
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(obj)
	ZEND_PARSE_PARAMETERS_END();

#endif


	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &obj);



	ZEPHIR_INIT_VAR(&arr);
	array_init(&arr);
	_0 = zephir_get_iterator(obj);
	_0->funcs->rewind(_0);
	for (;_0->funcs->valid(_0) == SUCCESS && !EG(exception); _0->funcs->move_forward(_0)) {
		ZEPHIR_GET_IMKEY(k, _0);
		{
			ZEPHIR_ITERATOR_COPY(&v, _0);
		}
		zephir_array_update_zval(&arr, &k, &v, PH_COPY | PH_SEPARATE);
	}
	zend_iterator_dtor(_0);
	RETURN_CCTOR(&arr);

}

