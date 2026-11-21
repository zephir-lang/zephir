
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
#include "kernel/object.h"
#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/operators.h"
#include "kernel/fcall.h"


/**
 * Arithmetic operations
 */
ZEPHIR_INIT_CLASS(Stub_Bitwise)
{
	ZEPHIR_REGISTER_CLASS(Stub, Bitwise, stub, bitwise, stub_bitwise_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_Bitwise, intAnd)
{
	zend_long a = 0, b = 0, c = 0;
	a = 1;
	b = 2;
	c = (a & b);
	RETURN_LONG(c);
}

PHP_METHOD(Stub_Bitwise, int2And)
{
	zend_long a = 0, c = 0;
	a = 1;
	c = (a & 2);
	RETURN_LONG(c);
}

PHP_METHOD(Stub_Bitwise, intAndSimple)
{
	zend_long c = 0;
	c = 0;
	RETURN_LONG(c);
}

PHP_METHOD(Stub_Bitwise, boolAnd)
{
	zend_bool a = 0, b = 0, c = 0;
	a = ((1) ? 1 : 0);
	b = ((2) ? 1 : 0);
	c = ((((zend_long) a & (zend_long) b)) ? 1 : 0);
	RETURN_BOOL(c);
}

PHP_METHOD(Stub_Bitwise, bool2And)
{
	zend_bool a = 0, c = 0;
	a = ((1) ? 1 : 0);
	c = ((((zend_long) a & 2)) ? 1 : 0);
	RETURN_BOOL(c);
}

PHP_METHOD(Stub_Bitwise, bool3And)
{
	zend_bool a = 0, c = 0;
	a = 1;
	c = ((((zend_long) a & (zend_long) 0)) ? 1 : 0);
	RETURN_BOOL(c);
}

PHP_METHOD(Stub_Bitwise, boolAndSimple)
{
	zend_bool c = 0;
	c = ((0) ? 1 : 0);
	RETURN_BOOL(c);
}

PHP_METHOD(Stub_Bitwise, doubleAnd)
{
	zend_long _0, _1;
	double a = 0, b = 0, c = 0;
	a = (double) (1);
	b = (double) (2);
	_0 = ZEPHIR_DVAL_TO_LVAL(a);
	if (UNEXPECTED(EG(exception))) {
		return;
	}
	_1 = ZEPHIR_DVAL_TO_LVAL(b);
	if (UNEXPECTED(EG(exception))) {
		return;
	}
	c = (double) ((_0 & _1));
	RETURN_DOUBLE(c);
}

PHP_METHOD(Stub_Bitwise, double2And)
{
	zend_long _0, _1;
	double a = 0, b = 0, c = 0;
	a =  (1.0);
	b =  (2.0);
	_0 = ZEPHIR_DVAL_TO_LVAL(a);
	if (UNEXPECTED(EG(exception))) {
		return;
	}
	_1 = ZEPHIR_DVAL_TO_LVAL(b);
	if (UNEXPECTED(EG(exception))) {
		return;
	}
	c = (double) ((_0 & _1));
	RETURN_DOUBLE(c);
}

PHP_METHOD(Stub_Bitwise, doubleAndSimple)
{
	double c = 0;
	c = (double) (0);
	RETURN_DOUBLE(c);
}

PHP_METHOD(Stub_Bitwise, doubleAnd2Simple)
{
	double c = 0;
	c = (double) (0);
	RETURN_DOUBLE(c);
}

PHP_METHOD(Stub_Bitwise, varAnd)
{
	zend_long a = 0, b = 0, c = 0;
	a = 1;
	b = 2;
	c = (a & b);
	RETURN_LONG(c);
}

PHP_METHOD(Stub_Bitwise, varAndSimple)
{
	zend_long c = 0;
	c = 0;
	RETURN_LONG(c);
}

PHP_METHOD(Stub_Bitwise, intDoubleAnd)
{
	zend_long _0;
	double a = 0;
	zend_long b = 0, c = 0;
	a = (double) (1);
	b = 2;
	_0 = ZEPHIR_DVAL_TO_LVAL(a);
	if (UNEXPECTED(EG(exception))) {
		return;
	}
	c = (_0 & b);
	RETURN_LONG(c);
}

PHP_METHOD(Stub_Bitwise, intDoubleAndSimple)
{
	zend_long c = 0;
	c = 0;
	RETURN_LONG(c);
}

PHP_METHOD(Stub_Bitwise, doubleIntAnd)
{
	zend_long _0;
	double a = 0, c = 0;
	zend_long b = 0;
	a = (double) (1);
	b = 2;
	_0 = ZEPHIR_DVAL_TO_LVAL(a);
	if (UNEXPECTED(EG(exception))) {
		return;
	}
	c = (double) ((_0 & b));
	RETURN_DOUBLE(c);
}

PHP_METHOD(Stub_Bitwise, doubleIntAndSimple)
{
	double c = 0;
	c = (double) (0);
	RETURN_DOUBLE(c);
}

PHP_METHOD(Stub_Bitwise, varIntAnd)
{
	zend_long b = 0, c = 0, a = 0;
	a = 1;
	b = 2;
	c = (a & b);
	RETURN_LONG(c);
}

PHP_METHOD(Stub_Bitwise, intVarAnd)
{
	zend_long b = 0, c = 0, a = 0;
	a = 1;
	b = 2;
	c = (b & a);
	RETURN_LONG(c);
}

PHP_METHOD(Stub_Bitwise, intVarImplicitCastAnd)
{
	zend_long c = 0;
	zval a, b, _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&b);
	ZVAL_UNDEF(&_0);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&a);
	ZVAL_STRING(&a, "1");
	ZEPHIR_INIT_VAR(&b);
	ZVAL_STRING(&b, "2");
	ZEPHIR_INIT_VAR(&_0);
	zephir_bitwise_and_function(&_0, &b, &a);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	c = zephir_get_intval(&_0);
	RETURN_MM_LONG(c);
}

PHP_METHOD(Stub_Bitwise, intVarImplicitCast2And)
{
	zend_long b = 0, c = 0;
	zval a, _0, _1;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&a);
	ZVAL_STRING(&a, "1");
	b = 2;
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, b);
	ZEPHIR_INIT_VAR(&_1);
	zephir_bitwise_and_function(&_1, &_0, &a);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	c = zephir_get_intval(&_1);
	RETURN_MM_LONG(c);
}

PHP_METHOD(Stub_Bitwise, complexAnd)
{
	zend_long c = 0;
	c = 0;
	RETURN_LONG(c);
}

PHP_METHOD(Stub_Bitwise, complex2And)
{
	zend_long c = 0;
	c = ((zend_long) 1 & (zend_long) 1);
	RETURN_LONG(c);
}

PHP_METHOD(Stub_Bitwise, complex3And)
{
	zend_long c = 0;
	c = ((zend_long) 1 & 1);
	RETURN_LONG(c);
}

PHP_METHOD(Stub_Bitwise, complex4And)
{
	zend_long c = 0;
	c = (1 & (zend_long) 1);
	RETURN_LONG(c);
}

PHP_METHOD(Stub_Bitwise, complex5And)
{
	zend_long c = 0;
	c = (1 & (zend_long) 1);
	RETURN_LONG(c);
}

PHP_METHOD(Stub_Bitwise, complex6And)
{
	zend_long c = 0;
	c = ((zend_long) 1 & 1);
	RETURN_LONG(c);
}

PHP_METHOD(Stub_Bitwise, complex7And)
{
	zend_long b = 0, c = 0;
	zend_bool a = 0;
	a = ((1) ? 1 : 0);
	b = 2;
	c = ((zend_long) a & b);
	RETURN_LONG(c);
}

PHP_METHOD(Stub_Bitwise, complex9And)
{
	zend_long b = 0, c = 0;
	zend_bool a = 0;
	a = ((1) ? 1 : 0);
	b = 2;
	c = (b & (zend_long) a);
	RETURN_LONG(c);
}

PHP_METHOD(Stub_Bitwise, complex10And)
{
	zend_long b = 0, c = 0;
	zend_bool a = 0;
	a = ((1.0 != 0.0) ? 1 : 0);
	b = 2;
	c = (b & (zend_long) a);
	RETURN_LONG(c);
}

PHP_METHOD(Stub_Bitwise, complex11And)
{
	zend_long b = 0, c = 0;
	zend_bool a = 0;
	a = ((1) ? 1 : 0);
	b = (long) (2.0);
	c = (b & (zend_long) a);
	RETURN_LONG(c);
}

PHP_METHOD(Stub_Bitwise, complex12And)
{
	zend_long b = 0, c = 0;
	zend_bool a = 0;
	a = ((1) ? 1 : 0);
	b = (long) (2.0);
	c = (b & (zend_long) a);
	RETURN_LONG(c);
}

PHP_METHOD(Stub_Bitwise, complex13And)
{
	zend_long c = 0;
	zend_bool a = 0, b = 0;
	a = ((1) ? 1 : 0);
	b = ((2.0 != 0.0) ? 1 : 0);
	c = ((zend_long) b & (zend_long) a);
	RETURN_LONG(c);
}

PHP_METHOD(Stub_Bitwise, complex14And)
{
	zend_bool b = 0;
	zend_long a = 0, c = 0;
	a = 1;
	b = ((2.0 != 0.0) ? 1 : 0);
	c = ((zend_long) b & a);
	RETURN_LONG(c);
}

PHP_METHOD(Stub_Bitwise, complex15And)
{
	zend_long _0;
	zend_bool c = 0;
	double b = 0;
	zend_long a = 0;
	a = 1;
	b =  (2.0);
	_0 = ZEPHIR_DVAL_TO_LVAL(b);
	if (UNEXPECTED(EG(exception))) {
		return;
	}
	c = (((_0 & a)) ? 1 : 0);
	RETURN_BOOL(c);
}

PHP_METHOD(Stub_Bitwise, complex16And)
{
	zend_long _0;
	double b = 0, c = 0;
	zend_long a = 0;
	a = 1;
	b =  (2.0);
	_0 = ZEPHIR_DVAL_TO_LVAL(b);
	if (UNEXPECTED(EG(exception))) {
		return;
	}
	c = (double) ((_0 & a));
	RETURN_DOUBLE(c);
}

PHP_METHOD(Stub_Bitwise, complex17And)
{
	double c = 0;
	zend_bool b = 0;
	zend_long a = 0;
	a = 1;
	b = ((2.0 != 0.0) ? 1 : 0);
	c = (double) ((a & (zend_long) b));
	RETURN_DOUBLE(c);
}

PHP_METHOD(Stub_Bitwise, complex18And)
{
	double c = 0;
	zend_bool b = 0;
	zend_long a = 0, d = 0;
	a = 1;
	b = ((2.0 != 0.0) ? 1 : 0);
	d = (a & (zend_long) b);
	c = (double) ((a & ((zend_long) b & d)));
	RETURN_DOUBLE(c);
}

PHP_METHOD(Stub_Bitwise, complex19And)
{
	double c = 0;
	zend_bool b = 0;
	zend_long a = 0, d = 0;
	a = 1;
	b = ((2.0 != 0.0) ? 1 : 0);
	d = (a & (zend_long) b);
	c = (double) ((a & (d & (zend_long) b)));
	RETURN_DOUBLE(c);
}

PHP_METHOD(Stub_Bitwise, complex20And)
{
	double c = 0;
	zend_bool b = 0;
	zend_long a = 0, d = 0;
	a = 1;
	b = ((2.0 != 0.0) ? 1 : 0);
	d = (a & (zend_long) b);
	c = (double) ((d & (d & (d & d))));
	RETURN_DOUBLE(c);
}

PHP_METHOD(Stub_Bitwise, complex21And)
{
	double c = 0;
	zend_bool b = 0;
	zend_long a = 0, d = 0;
	a = 1;
	b = ((2.0 != 0.0) ? 1 : 0);
	d = (a & (zend_long) b);
	c = (double) (((zend_long) b & (d & ((zend_long) b & (d & (d & ((zend_long) b & (d & (zend_long) b))))))));
	RETURN_DOUBLE(c);
}

PHP_METHOD(Stub_Bitwise, complex22And)
{
	double c = 0;
	zend_long a = 0, d = 0, b = 0;
	a = 1;
	b = (long) (2.0);
	d = (a & b);
	c = (double) ((b & (d & (d & (d & (d & (b & (d & b))))))));
	RETURN_DOUBLE(c);
}

PHP_METHOD(Stub_Bitwise, complex23And)
{
	double d = 0;
	zend_bool b = 0;
	zend_long a = 0;
	a = 1;
	b = ((2.0 != 0.0) ? 1 : 0);
	d = (double) ((a & (zend_long) b));
	RETURN_DOUBLE(d);
}

PHP_METHOD(Stub_Bitwise, complex24And)
{
	zend_bool b = 0;
	zend_long a = 0, d = 0;
	a = 1;
	b = ((2.0 != 0.0) ? 1 : 0);
	d = ((a & ((zend_long) b & (zend_long) 0)) & 1);
	RETURN_LONG(d);
}

PHP_METHOD(Stub_Bitwise, intOr)
{
	zend_long a = 0, b = 0, c = 0;
	a = 1;
	b = 2;
	c = (a | b);
	RETURN_LONG(c);
}

PHP_METHOD(Stub_Bitwise, int2Or)
{
	zend_long a = 0, c = 0;
	a = 1;
	c = (a | 2);
	RETURN_LONG(c);
}

PHP_METHOD(Stub_Bitwise, intOrSimple)
{
	zend_long c = 0;
	c = 3;
	RETURN_LONG(c);
}

PHP_METHOD(Stub_Bitwise, boolOr)
{
	zend_bool a = 0, b = 0, c = 0;
	a = ((1) ? 1 : 0);
	b = ((2) ? 1 : 0);
	c = ((((zend_long) a | (zend_long) b)) ? 1 : 0);
	RETURN_BOOL(c);
}

PHP_METHOD(Stub_Bitwise, bool2Or)
{
	zend_bool a = 0, c = 0;
	a = ((1) ? 1 : 0);
	c = ((((zend_long) a | 2)) ? 1 : 0);
	RETURN_BOOL(c);
}

PHP_METHOD(Stub_Bitwise, bool3Or)
{
	zend_bool a = 0, c = 0;
	a = 1;
	c = ((((zend_long) a | (zend_long) 0)) ? 1 : 0);
	RETURN_BOOL(c);
}

PHP_METHOD(Stub_Bitwise, bool4Or)
{
	zend_bool a = 0, c = 0;
	a = 1;
	c = ((((zend_long) a | (zend_long) 1)) ? 1 : 0);
	RETURN_BOOL(c);
}

PHP_METHOD(Stub_Bitwise, boolOrSimple)
{
	zend_bool c = 0;
	c = ((3) ? 1 : 0);
	RETURN_BOOL(c);
}

PHP_METHOD(Stub_Bitwise, doubleOr)
{
	zend_long _0, _1;
	double a = 0, b = 0, c = 0;
	a = (double) (1);
	b = (double) (2);
	_0 = ZEPHIR_DVAL_TO_LVAL(a);
	if (UNEXPECTED(EG(exception))) {
		return;
	}
	_1 = ZEPHIR_DVAL_TO_LVAL(b);
	if (UNEXPECTED(EG(exception))) {
		return;
	}
	c = (double) ((_0 | _1));
	RETURN_DOUBLE(c);
}

PHP_METHOD(Stub_Bitwise, double2Or)
{
	zend_long _0, _1;
	double a = 0, b = 0, c = 0;
	a =  (1.0);
	b =  (2.0);
	_0 = ZEPHIR_DVAL_TO_LVAL(a);
	if (UNEXPECTED(EG(exception))) {
		return;
	}
	_1 = ZEPHIR_DVAL_TO_LVAL(b);
	if (UNEXPECTED(EG(exception))) {
		return;
	}
	c = (double) ((_0 | _1));
	RETURN_DOUBLE(c);
}

PHP_METHOD(Stub_Bitwise, doubleOrSimple)
{
	double c = 0;
	c = (double) (3);
	RETURN_DOUBLE(c);
}

PHP_METHOD(Stub_Bitwise, doubleOr2Simple)
{
	double c = 0;
	c = (double) (3);
	RETURN_DOUBLE(c);
}

PHP_METHOD(Stub_Bitwise, varOr)
{
	zend_long a = 0, b = 0, c = 0;
	a = 1;
	b = 2;
	c = (a | b);
	RETURN_LONG(c);
}

PHP_METHOD(Stub_Bitwise, varOrSimple)
{
	zend_long c = 0;
	c = 3;
	RETURN_LONG(c);
}

PHP_METHOD(Stub_Bitwise, intDoubleOr)
{
	zend_long _0;
	double a = 0;
	zend_long b = 0, c = 0;
	a = (double) (1);
	b = 2;
	_0 = ZEPHIR_DVAL_TO_LVAL(a);
	if (UNEXPECTED(EG(exception))) {
		return;
	}
	c = (_0 | b);
	RETURN_LONG(c);
}

PHP_METHOD(Stub_Bitwise, intDoubleOrSimple)
{
	zend_long c = 0;
	c = 3;
	RETURN_LONG(c);
}

PHP_METHOD(Stub_Bitwise, doubleIntOr)
{
	zend_long _0;
	double a = 0, c = 0;
	zend_long b = 0;
	a = (double) (1);
	b = 2;
	_0 = ZEPHIR_DVAL_TO_LVAL(a);
	if (UNEXPECTED(EG(exception))) {
		return;
	}
	c = (double) ((_0 | b));
	RETURN_DOUBLE(c);
}

PHP_METHOD(Stub_Bitwise, doubleIntOrSimple)
{
	double c = 0;
	c = (double) (3);
	RETURN_DOUBLE(c);
}

PHP_METHOD(Stub_Bitwise, varIntOr)
{
	zend_long b = 0, c = 0, a = 0;
	a = 1;
	b = 2;
	c = (a | b);
	RETURN_LONG(c);
}

PHP_METHOD(Stub_Bitwise, intVarOr)
{
	zend_long b = 0, c = 0, a = 0;
	a = 1;
	b = 2;
	c = (b | a);
	RETURN_LONG(c);
}

PHP_METHOD(Stub_Bitwise, intVarImplicitCastOr)
{
	zend_long c = 0;
	zval a, b, _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&b);
	ZVAL_UNDEF(&_0);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&a);
	ZVAL_STRING(&a, "1");
	ZEPHIR_INIT_VAR(&b);
	ZVAL_STRING(&b, "2");
	ZEPHIR_INIT_VAR(&_0);
	zephir_bitwise_or_function(&_0, &b, &a);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	c = zephir_get_intval(&_0);
	RETURN_MM_LONG(c);
}

PHP_METHOD(Stub_Bitwise, intVarImplicitCast2Or)
{
	zend_long b = 0, c = 0;
	zval a, _0, _1;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&a);
	ZVAL_STRING(&a, "1");
	b = 2;
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, b);
	ZEPHIR_INIT_VAR(&_1);
	zephir_bitwise_or_function(&_1, &_0, &a);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	c = zephir_get_intval(&_1);
	RETURN_MM_LONG(c);
}

PHP_METHOD(Stub_Bitwise, complexOr)
{
	zend_long c = 0;
	c = 3;
	RETURN_LONG(c);
}

PHP_METHOD(Stub_Bitwise, complex2Or)
{
	zend_long c = 0;
	c = ((zend_long) 1 | (zend_long) 1);
	RETURN_LONG(c);
}

PHP_METHOD(Stub_Bitwise, complex3Or)
{
	zend_long c = 0;
	c = ((zend_long) 1 | 1);
	RETURN_LONG(c);
}

PHP_METHOD(Stub_Bitwise, complex4Or)
{
	zend_long c = 0;
	c = (1 | (zend_long) 1);
	RETURN_LONG(c);
}

PHP_METHOD(Stub_Bitwise, complex5Or)
{
	zend_long c = 0;
	c = (1 | (zend_long) 1);
	RETURN_LONG(c);
}

PHP_METHOD(Stub_Bitwise, complex6Or)
{
	zend_long c = 0;
	c = ((zend_long) 1 | 1);
	RETURN_LONG(c);
}

PHP_METHOD(Stub_Bitwise, complex7Or)
{
	zend_long b = 0, c = 0;
	zend_bool a = 0;
	a = ((1) ? 1 : 0);
	b = 2;
	c = ((zend_long) a | b);
	RETURN_LONG(c);
}

PHP_METHOD(Stub_Bitwise, complex9Or)
{
	zend_long b = 0, c = 0;
	zend_bool a = 0;
	a = ((1) ? 1 : 0);
	b = 2;
	c = (b | (zend_long) a);
	RETURN_LONG(c);
}

PHP_METHOD(Stub_Bitwise, complex10Or)
{
	zend_long b = 0, c = 0;
	zend_bool a = 0;
	a = ((1.0 != 0.0) ? 1 : 0);
	b = 2;
	c = (b | (zend_long) a);
	RETURN_LONG(c);
}

PHP_METHOD(Stub_Bitwise, complex11Or)
{
	zend_long b = 0, c = 0;
	zend_bool a = 0;
	a = ((1) ? 1 : 0);
	b = (long) (2.0);
	c = (b | (zend_long) a);
	RETURN_LONG(c);
}

PHP_METHOD(Stub_Bitwise, complex12Or)
{
	zend_long b = 0, c = 0;
	zend_bool a = 0;
	a = ((1) ? 1 : 0);
	b = (long) (2.0);
	c = (b | (zend_long) a);
	RETURN_LONG(c);
}

PHP_METHOD(Stub_Bitwise, complex13Or)
{
	zend_long c = 0;
	zend_bool a = 0, b = 0;
	a = ((1) ? 1 : 0);
	b = ((2.0 != 0.0) ? 1 : 0);
	c = ((zend_long) b | (zend_long) a);
	RETURN_LONG(c);
}

PHP_METHOD(Stub_Bitwise, complex14Or)
{
	zend_bool b = 0;
	zend_long a = 0, c = 0;
	a = 1;
	b = ((2.0 != 0.0) ? 1 : 0);
	c = ((zend_long) b | a);
	RETURN_LONG(c);
}

PHP_METHOD(Stub_Bitwise, complex15Or)
{
	zend_long _0;
	zend_bool c = 0;
	double b = 0;
	zend_long a = 0;
	a = 1;
	b =  (2.0);
	_0 = ZEPHIR_DVAL_TO_LVAL(b);
	if (UNEXPECTED(EG(exception))) {
		return;
	}
	c = (((_0 | a)) ? 1 : 0);
	RETURN_BOOL(c);
}

PHP_METHOD(Stub_Bitwise, complex16Or)
{
	zend_long _0;
	double b = 0, c = 0;
	zend_long a = 0;
	a = 1;
	b =  (2.0);
	_0 = ZEPHIR_DVAL_TO_LVAL(b);
	if (UNEXPECTED(EG(exception))) {
		return;
	}
	c = (double) ((_0 | a));
	RETURN_DOUBLE(c);
}

PHP_METHOD(Stub_Bitwise, complex17Or)
{
	double c = 0;
	zend_bool b = 0;
	zend_long a = 0;
	a = 1;
	b = ((2.0 != 0.0) ? 1 : 0);
	c = (double) ((a | (zend_long) b));
	RETURN_DOUBLE(c);
}

PHP_METHOD(Stub_Bitwise, complex18Or)
{
	double c = 0;
	zend_bool b = 0;
	zend_long a = 0, d = 0;
	a = 1;
	b = ((2.0 != 0.0) ? 1 : 0);
	d = (a | (zend_long) b);
	c = (double) ((d | (a | (zend_long) b)));
	RETURN_DOUBLE(c);
}

PHP_METHOD(Stub_Bitwise, complex19Or)
{
	double c = 0;
	zend_bool b = 0;
	zend_long a = 0, d = 0;
	a = 1;
	b = ((2.0 != 0.0) ? 1 : 0);
	d = (a | (zend_long) b);
	c = (double) (((zend_long) b | (a | d)));
	RETURN_DOUBLE(c);
}

PHP_METHOD(Stub_Bitwise, complex20Or)
{
	double c = 0;
	zend_bool b = 0;
	zend_long a = 0, d = 0;
	a = 1;
	b = ((2.0 != 0.0) ? 1 : 0);
	d = (a | (zend_long) b);
	c = (double) ((d | (d | (d | d))));
	RETURN_DOUBLE(c);
}

PHP_METHOD(Stub_Bitwise, complex21Or)
{
	double c = 0;
	zend_bool b = 0;
	zend_long a = 0, d = 0;
	a = 1;
	b = ((2.0 != 0.0) ? 1 : 0);
	d = (a | (zend_long) b);
	c = (double) (((zend_long) b | (d | ((zend_long) b | (d | (d | ((zend_long) b | ((zend_long) b | d))))))));
	RETURN_DOUBLE(c);
}

PHP_METHOD(Stub_Bitwise, complex22Or)
{
	double c = 0;
	zend_long a = 0, d = 0, b = 0;
	a = 1;
	b = (long) (2.0);
	d = (a | b);
	c = (double) ((b | (d | (b | (d | (d | (d | (b | d))))))));
	RETURN_DOUBLE(c);
}

PHP_METHOD(Stub_Bitwise, complex23Or)
{
	double d = 0;
	zend_bool b = 0;
	zend_long a = 0;
	a = 1;
	b = ((2.0 != 0.0) ? 1 : 0);
	d = (double) ((a | (zend_long) b));
	RETURN_DOUBLE(d);
}

PHP_METHOD(Stub_Bitwise, complex24Or)
{
	zend_bool b = 0;
	zend_long a = 0, d = 0;
	a = 1;
	b = ((2.0 != 0.0) ? 1 : 0);
	d = ((zend_long) b | ((a | 1) | (zend_long) 0));
	RETURN_LONG(d);
}

PHP_METHOD(Stub_Bitwise, intShiftLeft)
{
	zend_long _0;
	zend_long a = 0, b = 0, c = 0;
	a = 1;
	b = 2;
	_0 = zephir_safe_shift_left_long(a, b);
	if (UNEXPECTED(EG(exception))) {
		return;
	}
	c = _0;
	RETURN_LONG(c);
}

PHP_METHOD(Stub_Bitwise, int2ShiftLeft)
{
	zend_long a = 0, c = 0;
	a = 1;
	c = ((zend_long) ((zend_ulong) (a) << 2));
	RETURN_LONG(c);
}

PHP_METHOD(Stub_Bitwise, intShiftLeftSimple)
{
	zend_long c = 0;
	c = 4;
	RETURN_LONG(c);
}

PHP_METHOD(Stub_Bitwise, intShiftRight)
{
	zend_long _0;
	zend_long a = 0, b = 0, c = 0;
	a = 1;
	b = 2;
	_0 = zephir_safe_shift_right_long(a, b);
	if (UNEXPECTED(EG(exception))) {
		return;
	}
	c = _0;
	RETURN_LONG(c);
}

PHP_METHOD(Stub_Bitwise, int2ShiftRight)
{
	zend_long a = 0, c = 0;
	a = 1;
	c = (a >> 2);
	RETURN_LONG(c);
}

PHP_METHOD(Stub_Bitwise, intShiftRightSimple)
{
	zend_long c = 0;
	c = 0;
	RETURN_LONG(c);
}

PHP_METHOD(Stub_Bitwise, testBitwiseNot)
{
	zval *a_param = NULL;
	zend_long a;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(a)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &a_param);
	RETURN_LONG(~(a));
}

PHP_METHOD(Stub_Bitwise, testBitwiseAndNot)
{
	zval *a_param = NULL, *b_param = NULL;
	zend_long a, b;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_LONG(a)
		Z_PARAM_LONG(b)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(2, 0, &a_param, &b_param);
	RETURN_LONG((a & ~(b)));
}

PHP_METHOD(Stub_Bitwise, getInt)
{
	zval *num_param = NULL;
	zend_long num;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(num)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &num_param);
	RETURN_LONG(num);
}

/**
 * @issue https://github.com/zephir-lang/zephir/issues/1581
 */
PHP_METHOD(Stub_Bitwise, testbitwiseXor)
{
	zval i, _0, j;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&i);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&j);
	ZEND_TLS zephir_method_cache _zephir_call_cache[2];
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZVAL_LONG(&_0, 123);
	ZEPHIR_CALL_METHOD_CACHED(&i, this_ptr, "getInt", &_zephir_call_cache[0], &_0);
	zephir_check_call_status();
	ZVAL_LONG(&_0, 321);
	ZEPHIR_CALL_METHOD_CACHED(&j, this_ptr, "getInt", &_zephir_call_cache[1], &_0);
	zephir_check_call_status();
	zephir_bitwise_xor_function(return_value, &i, &j);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM();
}

/**
 * Hexadecimal literals in constant-folded operations.
 *
 * @issue https://github.com/zephir-lang/zephir/issues/2014
 */
PHP_METHOD(Stub_Bitwise, issue2014ShiftLeft)
{
	zend_long highmap = 0;
	highmap = -4294967296;
	RETURN_LONG(highmap);
}

PHP_METHOD(Stub_Bitwise, issue2014HexAnd)
{

	RETURN_LONG(15);
}

PHP_METHOD(Stub_Bitwise, issue2014HexArithmetic)
{

	RETURN_LONG(256);
}

/**
 * A dynamic right operand under a non-dynamic left operand.
 * Mirrors Phalcon\Http\Request::isIpAddressInCIDR().
 */
PHP_METHOD(Stub_Bitwise, intLiteralShiftLeftDynamic)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *remainingBits, remainingBits_sub, _0, _1, _2;

	ZVAL_UNDEF(&remainingBits_sub);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(remainingBits)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &remainingBits);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, 8);
	ZEPHIR_INIT_VAR(&_1);
	zephir_sub_function(&_1, &_0, remainingBits);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, 1);
	ZEPHIR_INIT_VAR(&_2);
	zephir_shift_left_function(&_2, &_0, &_1);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, 1);
	zephir_sub_function(return_value, &_2, &_0);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM();
}

PHP_METHOD(Stub_Bitwise, intLiteralXorDynamic)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *mask, mask_sub, _0;

	ZVAL_UNDEF(&mask_sub);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(mask)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &mask);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, 0xFF);
	zephir_bitwise_xor_function(return_value, mask, &_0);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM();
}

PHP_METHOD(Stub_Bitwise, boolLiteralOrDynamic)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *value, value_sub, _0;

	ZVAL_UNDEF(&value_sub);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(value)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &value);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_BOOL(&_0, 1);
	zephir_bitwise_or_function(return_value, value, &_0);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM();
}

PHP_METHOD(Stub_Bitwise, doubleLiteralAndDynamic)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *value, value_sub, _0;

	ZVAL_UNDEF(&value_sub);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(value)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &value);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_DOUBLE(&_0, 7.9);
	zephir_bitwise_and_function(return_value, value, &_0);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM();
}

PHP_METHOD(Stub_Bitwise, boolVariableOrDynamic)
{
	zend_bool flag;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *value, value_sub, _0;

	ZVAL_UNDEF(&value_sub);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(value)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &value);
	flag = 1;
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_BOOL(&_0, flag);
	zephir_bitwise_or_function(return_value, &_0, value);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM();
}

PHP_METHOD(Stub_Bitwise, intVariableXorDynamic)
{
	zend_long base;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *value, value_sub, _0;

	ZVAL_UNDEF(&value_sub);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(value)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &value);
	base = 255;
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, base);
	zephir_bitwise_xor_function(return_value, &_0, value);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM();
}

PHP_METHOD(Stub_Bitwise, dynamicAndIntLiteral)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *value, value_sub, _0;

	ZVAL_UNDEF(&value_sub);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(value)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &value);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, 0xFFFFFFFFFF);
	zephir_bitwise_and_function(return_value, value, &_0);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM();
}

