
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


ZEPHIR_INIT_CLASS(Test_Bench_Foo) {

	ZEPHIR_REGISTER_CLASS(Test\\Bench, Foo, test, bench_foo, test_bench_foo_method_entry, 0);

	zend_declare_property_long(test_bench_foo_ce, SL("a"), 0, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC TSRMLS_CC);

	zend_declare_property_long(test_bench_foo_ce, SL("b"), 0, ZEND_ACC_PUBLIC TSRMLS_CC);

	zend_declare_class_constant_long(test_bench_foo_ce, SL("TEST"), 0 TSRMLS_CC);

	return SUCCESS;

}

PHP_METHOD(Test_Bench_Foo, emptyForInRange) {

	int _1;
	zend_bool _0;
	zval *n, *i = NULL, *_2 = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &n);



	ZEPHIR_CPY_WRT(_2, n);
	_1 = 1;
	_0 = 0;
	if (ZEPHIR_GE_LONG(_2, _1)) {
		while (1) {
			if (_0) {
				_1++;
				if (!(ZEPHIR_GE_LONG(_2, _1))) {
					break;
				}
			} else {
				_0 = 1;
			}
			ZEPHIR_INIT_NVAR(i);
			ZVAL_LONG(i, _1);
		}
	}
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_Bench_Foo, readStatic) {

	int _1;
	zend_bool _0;
	zval *n, *x = NULL, *i = NULL, *_2 = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &n);



	ZEPHIR_CPY_WRT(_2, n);
	_1 = 1;
	_0 = 0;
	if (ZEPHIR_GE_LONG(_2, _1)) {
		while (1) {
			if (_0) {
				_1++;
				if (!(ZEPHIR_GE_LONG(_2, _1))) {
					break;
				}
			} else {
				_0 = 1;
			}
			ZEPHIR_INIT_NVAR(i);
			ZVAL_LONG(i, _1);
			ZEPHIR_OBS_NVAR(x);
			zephir_read_static_property_ce(&x, test_bench_foo_ce, SL("a") TSRMLS_CC);
		}
	}
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_Bench_Foo, writeStatic) {

	int _1;
	zend_bool _0;
	zval *n, *i = NULL, *_2 = NULL, *_3;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &n);



	ZEPHIR_CPY_WRT(_2, n);
	_1 = 1;
	_0 = 0;
	if (ZEPHIR_GE_LONG(_2, _1)) {
		while (1) {
			if (_0) {
				_1++;
				if (!(ZEPHIR_GE_LONG(_2, _1))) {
					break;
				}
			} else {
				_0 = 1;
			}
			ZEPHIR_INIT_NVAR(i);
			ZVAL_LONG(i, _1);
			ZEPHIR_INIT_ZVAL_NREF(_3);
			ZVAL_LONG(_3, 0);
			zephir_update_static_property_ce(test_bench_foo_ce, SL("a"), &_3 TSRMLS_CC);
		}
	}
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_Bench_Foo, issetStatic) {

	int _1;
	zend_bool x, _0;
	zval *n, *i = NULL, *_2 = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &n);



	ZEPHIR_CPY_WRT(_2, n);
	_1 = 1;
	_0 = 0;
	if (ZEPHIR_GE_LONG(_2, _1)) {
		while (1) {
			if (_0) {
				_1++;
				if (!(ZEPHIR_GE_LONG(_2, _1))) {
					break;
				}
			} else {
				_0 = 1;
			}
			ZEPHIR_INIT_NVAR(i);
			ZVAL_LONG(i, _1);
			x = (0 == 0);
		}
	}
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_Bench_Foo, emptyStatic) {

	int _1;
	zend_bool x, _0;
	zval *n, *i = NULL, *_2 = NULL, *_3 = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &n);



	ZEPHIR_CPY_WRT(_2, n);
	_1 = 1;
	_0 = 0;
	if (ZEPHIR_GE_LONG(_2, _1)) {
		while (1) {
			if (_0) {
				_1++;
				if (!(ZEPHIR_GE_LONG(_2, _1))) {
					break;
				}
			} else {
				_0 = 1;
			}
			ZEPHIR_INIT_NVAR(i);
			ZVAL_LONG(i, _1);
			ZEPHIR_OBS_NVAR(_3);
			zephir_read_static_property_ce(&_3, test_bench_foo_ce, SL("a") TSRMLS_CC);
			x = ZEPHIR_IS_EMPTY(_3);
		}
	}
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_Bench_Foo, readProp) {

	int _1;
	zend_bool _0;
	zval *n, *x, *i = NULL, *_2 = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &n);



	ZEPHIR_CPY_WRT(_2, n);
	_1 = 1;
	_0 = 0;
	if (ZEPHIR_GE_LONG(_2, _1)) {
		while (1) {
			if (_0) {
				_1++;
				if (!(ZEPHIR_GE_LONG(_2, _1))) {
					break;
				}
			} else {
				_0 = 1;
			}
			ZEPHIR_INIT_NVAR(i);
			ZVAL_LONG(i, _1);
			x = zephir_fetch_nproperty_this(this_ptr, SL("b"), PH_NOISY_CC);
		}
	}
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_Bench_Foo, writeProp) {

	int _1;
	zend_bool _0;
	zval *n, *i = NULL, *_2 = NULL, *_3;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &n);



	ZEPHIR_CPY_WRT(_2, n);
	_1 = 1;
	_0 = 0;
	if (ZEPHIR_GE_LONG(_2, _1)) {
		while (1) {
			if (_0) {
				_1++;
				if (!(ZEPHIR_GE_LONG(_2, _1))) {
					break;
				}
			} else {
				_0 = 1;
			}
			ZEPHIR_INIT_NVAR(i);
			ZVAL_LONG(i, _1);
			ZEPHIR_INIT_ZVAL_NREF(_3);
			ZVAL_LONG(_3, 0);
			zephir_update_property_this(this_ptr, SL("b"), _3 TSRMLS_CC);
		}
	}
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_Bench_Foo, assignAddProp) {

	int _1;
	zend_bool _0;
	zval *n, *i = NULL, *_2 = NULL, *_3, *_4 = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &n);



	ZEPHIR_CPY_WRT(_2, n);
	_1 = 1;
	_0 = 0;
	if (ZEPHIR_GE_LONG(_2, _1)) {
		while (1) {
			if (_0) {
				_1++;
				if (!(ZEPHIR_GE_LONG(_2, _1))) {
					break;
				}
			} else {
				_0 = 1;
			}
			ZEPHIR_INIT_NVAR(i);
			ZVAL_LONG(i, _1);
			ZEPHIR_INIT_NVAR(_4);
			ZVAL_LONG(_4, 2);
			ZEPHIR_ADD_ASSIGN(_3, _4)
			zephir_update_property_this(this_ptr, SL("b"), _3 TSRMLS_CC);
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
PHP_METHOD(Test_Bench_Foo, postIncProp) {

	int _1;
	zend_bool _0;
	zval *n, *i = NULL, *_2 = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &n);



	ZEPHIR_CPY_WRT(_2, n);
	_1 = 1;
	_0 = 0;
	if (ZEPHIR_GE_LONG(_2, _1)) {
		while (1) {
			if (_0) {
				_1++;
				if (!(ZEPHIR_GE_LONG(_2, _1))) {
					break;
				}
			} else {
				_0 = 1;
			}
			ZEPHIR_INIT_NVAR(i);
			ZVAL_LONG(i, _1);
			RETURN_ON_FAILURE(zephir_property_incr(this_ptr, SL("b") TSRMLS_CC));
		}
	}
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_Bench_Foo, postDecProp) {

	int _1;
	zend_bool _0;
	zval *n, *i = NULL, *_2 = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &n);



	ZEPHIR_CPY_WRT(_2, n);
	_1 = 1;
	_0 = 0;
	if (ZEPHIR_GE_LONG(_2, _1)) {
		while (1) {
			if (_0) {
				_1++;
				if (!(ZEPHIR_GE_LONG(_2, _1))) {
					break;
				}
			} else {
				_0 = 1;
			}
			ZEPHIR_INIT_NVAR(i);
			ZVAL_LONG(i, _1);
			RETURN_ON_FAILURE(zephir_property_decr(this_ptr, SL("b") TSRMLS_CC));
		}
	}
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_Bench_Foo, issetProp) {

	int _1;
	zend_bool x, _0;
	zval *n, *i = NULL, *_2 = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &n);



	ZEPHIR_CPY_WRT(_2, n);
	_1 = 1;
	_0 = 0;
	if (ZEPHIR_GE_LONG(_2, _1)) {
		while (1) {
			if (_0) {
				_1++;
				if (!(ZEPHIR_GE_LONG(_2, _1))) {
					break;
				}
			} else {
				_0 = 1;
			}
			ZEPHIR_INIT_NVAR(i);
			ZVAL_LONG(i, _1);
			x = zephir_isset_property(this_ptr, SS("b") TSRMLS_CC);
		}
	}
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_Bench_Foo, emptyProp) {

	int _1;
	zend_bool x, _0;
	zval *n, *i = NULL, *_2 = NULL, *_3 = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &n);



	ZEPHIR_CPY_WRT(_2, n);
	_1 = 1;
	_0 = 0;
	if (ZEPHIR_GE_LONG(_2, _1)) {
		while (1) {
			if (_0) {
				_1++;
				if (!(ZEPHIR_GE_LONG(_2, _1))) {
					break;
				}
			} else {
				_0 = 1;
			}
			ZEPHIR_INIT_NVAR(i);
			ZVAL_LONG(i, _1);
			ZEPHIR_OBS_NVAR(_3);
			zephir_read_property_this(&_3, this_ptr, SL("b"), PH_NOISY_CC);
			x = ZEPHIR_IS_EMPTY(_3);
		}
	}
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_Bench_Foo, g) {



}

PHP_METHOD(Test_Bench_Foo, call) {

	zephir_fcall_cache_entry *_3 = NULL;
	int _1, ZEPHIR_LAST_CALL_STATUS;
	zend_bool _0;
	zval *n, *i = NULL, *_2 = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &n);



	ZEPHIR_CPY_WRT(_2, n);
	_1 = 1;
	_0 = 0;
	if (ZEPHIR_GE_LONG(_2, _1)) {
		while (1) {
			if (_0) {
				_1++;
				if (!(ZEPHIR_GE_LONG(_2, _1))) {
					break;
				}
			} else {
				_0 = 1;
			}
			ZEPHIR_INIT_NVAR(i);
			ZVAL_LONG(i, _1);
			ZEPHIR_CALL_METHOD(NULL, this_ptr, "g", &_3, 0);
			zephir_check_call_status();
		}
	}
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_Bench_Foo, staticMethod) {



}

PHP_METHOD(Test_Bench_Foo, staticMethod1) {


	RETURN_BOOL(1);

}

PHP_METHOD(Test_Bench_Foo, scall) {

	zephir_fcall_cache_entry *_3 = NULL;
	int _1, ZEPHIR_LAST_CALL_STATUS;
	zend_bool _0;
	zval *n, *i = NULL, *_2 = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &n);



	ZEPHIR_CPY_WRT(_2, n);
	_1 = 1;
	_0 = 0;
	if (ZEPHIR_GE_LONG(_2, _1)) {
		while (1) {
			if (_0) {
				_1++;
				if (!(ZEPHIR_GE_LONG(_2, _1))) {
					break;
				}
			} else {
				_0 = 1;
			}
			ZEPHIR_INIT_NVAR(i);
			ZVAL_LONG(i, _1);
			ZEPHIR_CALL_SELF(NULL, "staticmethod", &_3, 0);
			zephir_check_call_status();
		}
	}
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_Bench_Foo, scallWithReturnTrue) {

	zephir_fcall_cache_entry *_3 = NULL;
	int _1, ZEPHIR_LAST_CALL_STATUS;
	zend_bool _0;
	zval *n, *i = NULL, *_2 = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &n);



	ZEPHIR_CPY_WRT(_2, n);
	_1 = 1;
	_0 = 0;
	if (ZEPHIR_GE_LONG(_2, _1)) {
		while (1) {
			if (_0) {
				_1++;
				if (!(ZEPHIR_GE_LONG(_2, _1))) {
					break;
				}
			} else {
				_0 = 1;
			}
			ZEPHIR_INIT_NVAR(i);
			ZVAL_LONG(i, _1);
			ZEPHIR_CALL_SELF(NULL, "staticmethod1", &_3, 0);
			zephir_check_call_status();
		}
	}
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_Bench_Foo, readConst) {

	int _1;
	zend_bool _0;
	zval *n, *i = NULL, *x = NULL, *_2 = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &n);



	ZEPHIR_CPY_WRT(_2, n);
	_1 = 1;
	_0 = 0;
	if (ZEPHIR_GE_LONG(_2, _1)) {
		while (1) {
			if (_0) {
				_1++;
				if (!(ZEPHIR_GE_LONG(_2, _1))) {
					break;
				}
			} else {
				_0 = 1;
			}
			ZEPHIR_INIT_NVAR(i);
			ZVAL_LONG(i, _1);
			ZEPHIR_INIT_NVAR(x);
			ZVAL_LONG(x, 0);
		}
	}
	ZEPHIR_MM_RESTORE();

}

