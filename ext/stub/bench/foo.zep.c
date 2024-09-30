
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
#include "kernel/operators.h"
#include "kernel/memory.h"
#include "kernel/object.h"
#include "kernel/fcall.h"


ZEPHIR_INIT_CLASS(Stub_Bench_Foo)
{
	ZEPHIR_REGISTER_CLASS(Stub\\Bench, Foo, stub, bench_foo, stub_bench_foo_method_entry, 0);

	zend_declare_property_long(stub_bench_foo_ce, SL("a"), 0, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC);
	zend_declare_property_long(stub_bench_foo_ce, SL("b"), 0, ZEND_ACC_PUBLIC);
	zephir_declare_class_constant_long(stub_bench_foo_ce, SL("TEST"), 0);

	return SUCCESS;
}

PHP_METHOD(Stub_Bench_Foo, emptyForInRange)
{
	zend_long _1;
	zend_bool _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *n, n_sub, i, _2;

	ZVAL_UNDEF(&n_sub);
	ZVAL_UNDEF(&i);
	ZVAL_UNDEF(&_2);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(n)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &n);
	ZEPHIR_CPY_WRT(&_2, n);
	_1 = 1;
	_0 = 0;
	if (ZEPHIR_GE_LONG(&_2, _1)) {
		while (1) {
			if (_0) {
				_1++;
				if (!(ZEPHIR_GE_LONG(&_2, _1))) {
					break;
				}
			} else {
				_0 = 1;
			}
			ZEPHIR_INIT_NVAR(&i);
			ZVAL_LONG(&i, _1);
		}
	}
	ZEPHIR_MM_RESTORE();
}

PHP_METHOD(Stub_Bench_Foo, readStatic)
{
	zend_long _1;
	zend_bool _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *n, n_sub, x, i, _2;

	ZVAL_UNDEF(&n_sub);
	ZVAL_UNDEF(&x);
	ZVAL_UNDEF(&i);
	ZVAL_UNDEF(&_2);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(n)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &n);
	ZEPHIR_CPY_WRT(&_2, n);
	_1 = 1;
	_0 = 0;
	if (ZEPHIR_GE_LONG(&_2, _1)) {
		while (1) {
			if (_0) {
				_1++;
				if (!(ZEPHIR_GE_LONG(&_2, _1))) {
					break;
				}
			} else {
				_0 = 1;
			}
			ZEPHIR_INIT_NVAR(&i);
			ZVAL_LONG(&i, _1);
			ZEPHIR_OBS_NVAR(&x);
			zephir_read_static_property_ce(&x, stub_bench_foo_ce, SL("a"), PH_NOISY_CC);
		}
	}
	ZEPHIR_MM_RESTORE();
}

PHP_METHOD(Stub_Bench_Foo, writeStatic)
{
	zend_long _1;
	zend_bool _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *n, n_sub, i, _2, _3$$3;

	ZVAL_UNDEF(&n_sub);
	ZVAL_UNDEF(&i);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_3$$3);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(n)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &n);
	ZEPHIR_CPY_WRT(&_2, n);
	_1 = 1;
	_0 = 0;
	if (ZEPHIR_GE_LONG(&_2, _1)) {
		while (1) {
			if (_0) {
				_1++;
				if (!(ZEPHIR_GE_LONG(&_2, _1))) {
					break;
				}
			} else {
				_0 = 1;
			}
			ZEPHIR_INIT_NVAR(&i);
			ZVAL_LONG(&i, _1);
			ZVAL_UNDEF(&_3$$3);
			ZVAL_LONG(&_3$$3, 0);
			zephir_update_static_property_ce(stub_bench_foo_ce, ZEND_STRL("a"), &_3$$3);
		}
	}
	ZEPHIR_MM_RESTORE();
}

PHP_METHOD(Stub_Bench_Foo, issetStatic)
{
	zend_long _1;
	zend_bool x = 0, _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *n, n_sub, i, _2;

	ZVAL_UNDEF(&n_sub);
	ZVAL_UNDEF(&i);
	ZVAL_UNDEF(&_2);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(n)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &n);
	ZEPHIR_CPY_WRT(&_2, n);
	_1 = 1;
	_0 = 0;
	if (ZEPHIR_GE_LONG(&_2, _1)) {
		while (1) {
			if (_0) {
				_1++;
				if (!(ZEPHIR_GE_LONG(&_2, _1))) {
					break;
				}
			} else {
				_0 = 1;
			}
			ZEPHIR_INIT_NVAR(&i);
			ZVAL_LONG(&i, _1);
			x = (0 == 0);
		}
	}
	ZEPHIR_MM_RESTORE();
}

PHP_METHOD(Stub_Bench_Foo, emptyStatic)
{
	zend_long _1;
	zend_bool x = 0, _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *n, n_sub, i, _2, _3$$3;

	ZVAL_UNDEF(&n_sub);
	ZVAL_UNDEF(&i);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_3$$3);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(n)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &n);
	ZEPHIR_CPY_WRT(&_2, n);
	_1 = 1;
	_0 = 0;
	if (ZEPHIR_GE_LONG(&_2, _1)) {
		while (1) {
			if (_0) {
				_1++;
				if (!(ZEPHIR_GE_LONG(&_2, _1))) {
					break;
				}
			} else {
				_0 = 1;
			}
			ZEPHIR_INIT_NVAR(&i);
			ZVAL_LONG(&i, _1);
			ZEPHIR_OBS_NVAR(&_3$$3);
			zephir_read_static_property_ce(&_3$$3, stub_bench_foo_ce, SL("a"), PH_NOISY_CC);
			x = ZEPHIR_IS_EMPTY(&_3$$3);
		}
	}
	ZEPHIR_MM_RESTORE();
}

PHP_METHOD(Stub_Bench_Foo, readProp)
{
	zend_long _1;
	zend_bool _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *n, n_sub, x, i, _2;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&n_sub);
	ZVAL_UNDEF(&x);
	ZVAL_UNDEF(&i);
	ZVAL_UNDEF(&_2);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(n)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &n);
	ZEPHIR_CPY_WRT(&_2, n);
	_1 = 1;
	_0 = 0;
	if (ZEPHIR_GE_LONG(&_2, _1)) {
		while (1) {
			if (_0) {
				_1++;
				if (!(ZEPHIR_GE_LONG(&_2, _1))) {
					break;
				}
			} else {
				_0 = 1;
			}
			ZEPHIR_INIT_NVAR(&i);
			ZVAL_LONG(&i, _1);
			zephir_read_property(&x, this_ptr, ZEND_STRL("b"), PH_NOISY_CC | PH_READONLY);
		}
	}
	ZEPHIR_MM_RESTORE();
}

PHP_METHOD(Stub_Bench_Foo, writeProp)
{
	zend_long _1;
	zend_bool _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *n, n_sub, i, _2, _3$$3;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&n_sub);
	ZVAL_UNDEF(&i);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_3$$3);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(n)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &n);
	ZEPHIR_CPY_WRT(&_2, n);
	_1 = 1;
	_0 = 0;
	if (ZEPHIR_GE_LONG(&_2, _1)) {
		while (1) {
			if (_0) {
				_1++;
				if (!(ZEPHIR_GE_LONG(&_2, _1))) {
					break;
				}
			} else {
				_0 = 1;
			}
			ZEPHIR_INIT_NVAR(&i);
			ZVAL_LONG(&i, _1);
			ZVAL_UNDEF(&_3$$3);
			ZVAL_LONG(&_3$$3, 0);
			zephir_update_property_zval(this_ptr, ZEND_STRL("b"), &_3$$3);
		}
	}
	ZEPHIR_MM_RESTORE();
}

PHP_METHOD(Stub_Bench_Foo, assignAddProp)
{
	zend_long _1;
	zend_bool _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *n, n_sub, i, _2, _3$$3, _4$$3;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&n_sub);
	ZVAL_UNDEF(&i);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_3$$3);
	ZVAL_UNDEF(&_4$$3);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(n)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &n);
	ZEPHIR_CPY_WRT(&_2, n);
	_1 = 1;
	_0 = 0;
	if (ZEPHIR_GE_LONG(&_2, _1)) {
		while (1) {
			if (_0) {
				_1++;
				if (!(ZEPHIR_GE_LONG(&_2, _1))) {
					break;
				}
			} else {
				_0 = 1;
			}
			ZEPHIR_INIT_NVAR(&i);
			ZVAL_LONG(&i, _1);
			ZEPHIR_INIT_NVAR(&_4$$3);
			ZVAL_LONG(&_4$$3, 2);
			zephir_read_property(&_3$$3, this_ptr, ZEND_STRL("b"), PH_NOISY_CC);
			ZEPHIR_ADD_ASSIGN(&_3$$3, &_4$$3)
			zephir_update_property_zval(this_ptr, ZEND_STRL("b"), &_3$$3);
		}
	}
	ZEPHIR_MM_RESTORE();
}

/**
 public function pre_inc_prop(var n) {
 var i;

 for i in range(1, n) {
 ++this->b;
 }
 }

 public function pre_dec_prop($n) {
 var i;

 for i in range(1, n) {
 --$this->b;
 }
 }*/
PHP_METHOD(Stub_Bench_Foo, postIncProp)
{
	zend_long _1;
	zend_bool _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *n, n_sub, i, _2;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&n_sub);
	ZVAL_UNDEF(&i);
	ZVAL_UNDEF(&_2);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(n)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &n);
	ZEPHIR_CPY_WRT(&_2, n);
	_1 = 1;
	_0 = 0;
	if (ZEPHIR_GE_LONG(&_2, _1)) {
		while (1) {
			if (_0) {
				_1++;
				if (!(ZEPHIR_GE_LONG(&_2, _1))) {
					break;
				}
			} else {
				_0 = 1;
			}
			ZEPHIR_INIT_NVAR(&i);
			ZVAL_LONG(&i, _1);
			RETURN_ON_FAILURE(zephir_property_incr(this_ptr, SL("b")));
		}
	}
	ZEPHIR_MM_RESTORE();
}

PHP_METHOD(Stub_Bench_Foo, postDecProp)
{
	zend_long _1;
	zend_bool _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *n, n_sub, i, _2;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&n_sub);
	ZVAL_UNDEF(&i);
	ZVAL_UNDEF(&_2);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(n)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &n);
	ZEPHIR_CPY_WRT(&_2, n);
	_1 = 1;
	_0 = 0;
	if (ZEPHIR_GE_LONG(&_2, _1)) {
		while (1) {
			if (_0) {
				_1++;
				if (!(ZEPHIR_GE_LONG(&_2, _1))) {
					break;
				}
			} else {
				_0 = 1;
			}
			ZEPHIR_INIT_NVAR(&i);
			ZVAL_LONG(&i, _1);
			RETURN_ON_FAILURE(zephir_property_decr(this_ptr, SL("b")));
		}
	}
	ZEPHIR_MM_RESTORE();
}

PHP_METHOD(Stub_Bench_Foo, issetProp)
{
	zend_long _1;
	zend_bool x = 0, _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *n, n_sub, i, _2;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&n_sub);
	ZVAL_UNDEF(&i);
	ZVAL_UNDEF(&_2);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(n)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &n);
	ZEPHIR_CPY_WRT(&_2, n);
	_1 = 1;
	_0 = 0;
	if (ZEPHIR_GE_LONG(&_2, _1)) {
		while (1) {
			if (_0) {
				_1++;
				if (!(ZEPHIR_GE_LONG(&_2, _1))) {
					break;
				}
			} else {
				_0 = 1;
			}
			ZEPHIR_INIT_NVAR(&i);
			ZVAL_LONG(&i, _1);
			x = zephir_isset_property(this_ptr, SL("b"));
		}
	}
	ZEPHIR_MM_RESTORE();
}

PHP_METHOD(Stub_Bench_Foo, emptyProp)
{
	zend_long _1;
	zend_bool x = 0, _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *n, n_sub, i, _2, _3$$3;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&n_sub);
	ZVAL_UNDEF(&i);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_3$$3);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(n)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &n);
	ZEPHIR_CPY_WRT(&_2, n);
	_1 = 1;
	_0 = 0;
	if (ZEPHIR_GE_LONG(&_2, _1)) {
		while (1) {
			if (_0) {
				_1++;
				if (!(ZEPHIR_GE_LONG(&_2, _1))) {
					break;
				}
			} else {
				_0 = 1;
			}
			ZEPHIR_INIT_NVAR(&i);
			ZVAL_LONG(&i, _1);
			ZEPHIR_OBS_NVAR(&_3$$3);
			zephir_read_property(&_3$$3, this_ptr, ZEND_STRL("b"), PH_NOISY_CC);
			x = ZEPHIR_IS_EMPTY(&_3$$3);
		}
	}
	ZEPHIR_MM_RESTORE();
}

PHP_METHOD(Stub_Bench_Foo, g)
{

}

PHP_METHOD(Stub_Bench_Foo, call)
{
	zend_bool _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zephir_fcall_cache_entry *_3 = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS, _1;
	zval *n, n_sub, i, _2;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&n_sub);
	ZVAL_UNDEF(&i);
	ZVAL_UNDEF(&_2);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(n)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &n);
	ZEPHIR_CPY_WRT(&_2, n);
	_1 = 1;
	_0 = 0;
	if (ZEPHIR_GE_LONG(&_2, _1)) {
		while (1) {
			if (_0) {
				_1++;
				if (!(ZEPHIR_GE_LONG(&_2, _1))) {
					break;
				}
			} else {
				_0 = 1;
			}
			ZEPHIR_INIT_NVAR(&i);
			ZVAL_LONG(&i, _1);
			ZEPHIR_CALL_METHOD(NULL, this_ptr, "g", &_3, 0);
			zephir_check_call_status();
		}
	}
	ZEPHIR_MM_RESTORE();
}

PHP_METHOD(Stub_Bench_Foo, staticMethod)
{

}

PHP_METHOD(Stub_Bench_Foo, staticMethod1)
{

	RETURN_BOOL(1);
}

PHP_METHOD(Stub_Bench_Foo, scall)
{
	zend_bool _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS, _1;
	zephir_fcall_cache_entry *_3 = NULL;
	zval *n, n_sub, i, _2;

	ZVAL_UNDEF(&n_sub);
	ZVAL_UNDEF(&i);
	ZVAL_UNDEF(&_2);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(n)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &n);
	ZEPHIR_CPY_WRT(&_2, n);
	_1 = 1;
	_0 = 0;
	if (ZEPHIR_GE_LONG(&_2, _1)) {
		while (1) {
			if (_0) {
				_1++;
				if (!(ZEPHIR_GE_LONG(&_2, _1))) {
					break;
				}
			} else {
				_0 = 1;
			}
			ZEPHIR_INIT_NVAR(&i);
			ZVAL_LONG(&i, _1);
			ZEPHIR_CALL_SELF(NULL, "staticmethod", &_3, 0);
			zephir_check_call_status();
		}
	}
	ZEPHIR_MM_RESTORE();
}

PHP_METHOD(Stub_Bench_Foo, scallWithReturnTrue)
{
	zend_bool _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS, _1;
	zephir_fcall_cache_entry *_3 = NULL;
	zval *n, n_sub, i, _2;

	ZVAL_UNDEF(&n_sub);
	ZVAL_UNDEF(&i);
	ZVAL_UNDEF(&_2);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(n)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &n);
	ZEPHIR_CPY_WRT(&_2, n);
	_1 = 1;
	_0 = 0;
	if (ZEPHIR_GE_LONG(&_2, _1)) {
		while (1) {
			if (_0) {
				_1++;
				if (!(ZEPHIR_GE_LONG(&_2, _1))) {
					break;
				}
			} else {
				_0 = 1;
			}
			ZEPHIR_INIT_NVAR(&i);
			ZVAL_LONG(&i, _1);
			ZEPHIR_CALL_SELF(NULL, "staticmethod1", &_3, 0);
			zephir_check_call_status();
		}
	}
	ZEPHIR_MM_RESTORE();
}

PHP_METHOD(Stub_Bench_Foo, readConst)
{
	zend_long _1;
	zend_bool _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *n, n_sub, i, x, _2;

	ZVAL_UNDEF(&n_sub);
	ZVAL_UNDEF(&i);
	ZVAL_UNDEF(&x);
	ZVAL_UNDEF(&_2);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(n)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &n);
	ZEPHIR_CPY_WRT(&_2, n);
	_1 = 1;
	_0 = 0;
	if (ZEPHIR_GE_LONG(&_2, _1)) {
		while (1) {
			if (_0) {
				_1++;
				if (!(ZEPHIR_GE_LONG(&_2, _1))) {
					break;
				}
			} else {
				_0 = 1;
			}
			ZEPHIR_INIT_NVAR(&i);
			ZVAL_LONG(&i, _1);
			ZEPHIR_INIT_NVAR(&x);
			ZVAL_LONG(&x, 0);
		}
	}
	ZEPHIR_MM_RESTORE();
}

