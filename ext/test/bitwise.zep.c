
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
#include "kernel/operators.h"


/**
 * Arithmetic operations
 */
ZEPHIR_INIT_CLASS(Test_Bitwise) {

	ZEPHIR_REGISTER_CLASS(Test, Bitwise, test, bitwise, test_bitwise_method_entry, 0);

	return SUCCESS;

}

PHP_METHOD(Test_Bitwise, intAnd) {

	int a = 0, b = 0, c = 0;


	a = 1;
	b = 2;
	c = (a & b);
	RETURN_LONG(c);

}

PHP_METHOD(Test_Bitwise, int2And) {

	int a = 0, c = 0;


	a = 1;
	c = (a & 2);
	RETURN_LONG(c);

}

PHP_METHOD(Test_Bitwise, intAndSimple) {

	int c = 0;


	c = 0;
	RETURN_LONG(c);

}

PHP_METHOD(Test_Bitwise, boolAnd) {

	zend_bool a = 0, b = 0, c = 0;


	a = (1) ? 1 : 0;
	b = (2) ? 1 : 0;
	c = ((a & b)) ? 1 : 0;
	RETURN_BOOL(c);

}

PHP_METHOD(Test_Bitwise, bool2And) {

	zend_bool a = 0, c = 0;


	a = (1) ? 1 : 0;
	c = ((a & 2)) ? 1 : 0;
	RETURN_BOOL(c);

}

PHP_METHOD(Test_Bitwise, bool3And) {

	zend_bool a = 0, c = 0;


	a = 1;
	c = ((a & 0)) ? 1 : 0;
	RETURN_BOOL(c);

}

PHP_METHOD(Test_Bitwise, boolAndSimple) {

	zend_bool c = 0;


	c = (0) ? 1 : 0;
	RETURN_BOOL(c);

}

PHP_METHOD(Test_Bitwise, doubleAnd) {

	double a = 0, b = 0, c = 0;


	a = (double) (1);
	b = (double) (2);
	c = (double) (((int) (a) & (int) (b)));
	RETURN_DOUBLE(c);

}

PHP_METHOD(Test_Bitwise, double2And) {

	double a = 0, b = 0, c = 0;


	a = 1.0;
	b = 2.0;
	c = (double) (((int) (a) & (int) (b)));
	RETURN_DOUBLE(c);

}

PHP_METHOD(Test_Bitwise, doubleAndSimple) {

	double c = 0;


	c = (double) (0);
	RETURN_DOUBLE(c);

}

PHP_METHOD(Test_Bitwise, doubleAnd2Simple) {

	double c = 0;


	c = (double) (0.0);
	RETURN_DOUBLE(c);

}

PHP_METHOD(Test_Bitwise, varAnd) {

	int a = 0, b = 0, c = 0;


	a = 1;
	b = 2;
	c = (a & b);
	RETURN_LONG(c);

}

PHP_METHOD(Test_Bitwise, varAndSimple) {

	int c = 0;


	c = 0;
	RETURN_LONG(c);

}

PHP_METHOD(Test_Bitwise, intDoubleAnd) {

	double a = 0;
	int b = 0, c = 0;


	a = (double) (1);
	b = 2;
	c = ((int) (a) &  b);
	RETURN_LONG(c);

}

PHP_METHOD(Test_Bitwise, intDoubleAndSimple) {

	int c = 0;


	c = 0;
	RETURN_LONG(c);

}

PHP_METHOD(Test_Bitwise, doubleIntAnd) {

	double a = 0, c = 0;
	int b = 0;


	a = (double) (1);
	b = 2;
	c = (double) (((int) (a) &  b));
	RETURN_DOUBLE(c);

}

PHP_METHOD(Test_Bitwise, doubleIntAndSimple) {

	double c = 0;


	c = (double) (0);
	RETURN_DOUBLE(c);

}

PHP_METHOD(Test_Bitwise, varIntAnd) {

	int b = 0, c = 0, a = 0;


	a = 1;
	b = 2;
	c = (a & b);
	RETURN_LONG(c);

}

PHP_METHOD(Test_Bitwise, intVarAnd) {

	int b = 0, c = 0, a = 0;


	a = 1;
	b = 2;
	c = (b & a);
	RETURN_LONG(c);

}

PHP_METHOD(Test_Bitwise, intVarImplicitCastAnd) {

	int c = 0;
	long a = 0, b = 0;


	a = 1;
	b = 2;
	c = (b & a);
	RETURN_LONG(c);

}

PHP_METHOD(Test_Bitwise, intVarImplicitCast2And) {

	int b = 0, c = 0;
	long a = 0;


	a = 1;
	b = 2;
	c = (b & a);
	RETURN_LONG(c);

}

PHP_METHOD(Test_Bitwise, complexAnd) {

	double c = 0;


	c = (double) (0);
	RETURN_DOUBLE(c);

}

PHP_METHOD(Test_Bitwise, complex2And) {

	zend_bool c = 0;


	c = ((1 & 1)) ? 1 : 0;
	RETURN_BOOL(c);

}

PHP_METHOD(Test_Bitwise, complex3And) {

	zend_bool c = 0;


	c = ((1 &((1.0) ? 1 : 0))) ? 1 : 0;
	RETURN_BOOL(c);

}

PHP_METHOD(Test_Bitwise, complex4And) {

	zval *c = NULL;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(c);
	ZVAL_LONG(c, ((int) (1.0) & 1));
	RETURN_CCTOR(c);

}

PHP_METHOD(Test_Bitwise, complex5And) {

	int c = 0;


	c = (1 & 1);
	RETURN_LONG(c);

}

PHP_METHOD(Test_Bitwise, complex6And) {

	zend_bool c = 0;


	c = ((1 &((1) ? 1 : 0))) ? 1 : 0;
	RETURN_BOOL(c);

}

PHP_METHOD(Test_Bitwise, complex7And) {

	int b = 0, c = 0;
	zend_bool a = 0;


	a = (1) ? 1 : 0;
	b = 2;
	c = (a & b);
	RETURN_LONG(c);

}

PHP_METHOD(Test_Bitwise, complex9And) {

	int b = 0, c = 0;
	zend_bool a = 0;


	a = (1) ? 1 : 0;
	b = 2;
	c = (b & a);
	RETURN_LONG(c);

}

PHP_METHOD(Test_Bitwise, complex10And) {

	double c = 0;
	int b = 0;
	zend_bool a = 0;


	a = (1.0) ? 1 : 0;
	b = 2;
	c = (double) ((b & a));
	RETURN_DOUBLE(c);

}

PHP_METHOD(Test_Bitwise, complex11And) {

	double c = 0;
	int b = 0;
	zend_bool a = 0;


	a = (1) ? 1 : 0;
	b = (long) (2.0);
	c = (double) ((b & a));
	RETURN_DOUBLE(c);

}

PHP_METHOD(Test_Bitwise, complex12And) {

	int b = 0, c = 0;
	zend_bool a = 0;


	a = (1) ? 1 : 0;
	b = (long) (2.0);
	c = (b & a);
	RETURN_LONG(c);

}

PHP_METHOD(Test_Bitwise, complex13And) {

	int c = 0;
	zend_bool a = 0, b = 0;


	a = (1) ? 1 : 0;
	b = (2.0) ? 1 : 0;
	c = (b & a);
	RETURN_LONG(c);

}

PHP_METHOD(Test_Bitwise, complex14And) {

	zend_bool b = 0;
	int a = 0, c = 0;


	a = 1;
	b = (2.0) ? 1 : 0;
	c = (b & a);
	RETURN_LONG(c);

}

PHP_METHOD(Test_Bitwise, complex15And) {

	zend_bool c = 0;
	double b = 0;
	int a = 0;


	a = 1;
	b = 2.0;
	c = (((int) (b) &  a)) ? 1 : 0;
	RETURN_BOOL(c);

}

PHP_METHOD(Test_Bitwise, complex16And) {

	double b = 0, c = 0;
	int a = 0;


	a = 1;
	b = 2.0;
	c = (double) (((int) (b) &  a));
	RETURN_DOUBLE(c);

}

PHP_METHOD(Test_Bitwise, complex17And) {

	double c = 0;
	zend_bool b = 0;
	int a = 0;


	a = 1;
	b = (2.0) ? 1 : 0;
	c = (double) ((a & b));
	RETURN_DOUBLE(c);

}

PHP_METHOD(Test_Bitwise, complex18And) {

	zend_bool b = 0;
	double d = 0, c = 0;
	int a = 0;


	a = 1;
	b = (2.0) ? 1 : 0;
	d = (double) ((a & b));
	c = (double) ((a & (b & (int) (d))));
	RETURN_DOUBLE(c);

}

PHP_METHOD(Test_Bitwise, complex19And) {

	zend_bool b = 0;
	double d = 0, c = 0;
	int a = 0;


	a = 1;
	b = (2.0) ? 1 : 0;
	d = (double) ((a & b));
	c = (double) ((a & ((int) (d) & b)));
	RETURN_DOUBLE(c);

}

PHP_METHOD(Test_Bitwise, complex20And) {

	zend_bool b = 0;
	double d = 0, c = 0;
	int a = 0;


	a = 1;
	b = (2.0) ? 1 : 0;
	d = (double) ((a & b));
	c = (double) (((int) (d) & ((int) (d) & ((int) (d) & (int) (d)))));
	RETURN_DOUBLE(c);

}

PHP_METHOD(Test_Bitwise, complex21And) {

	zend_bool b = 0;
	double d = 0, c = 0;
	int a = 0;


	a = 1;
	b = (2.0) ? 1 : 0;
	d = (double) ((a & b));
	c = (double) ((b & ((int) (d) & (b & ((int) (d) & ((int) (d) & (b & ((int) (d) & b))))))));
	RETURN_DOUBLE(c);

}

PHP_METHOD(Test_Bitwise, complex22And) {

	double d = 0, c = 0;
	int a = 0, b = 0;


	a = 1;
	b = (long) (2.0);
	d = (double) ((a & b));
	c = (double) ((b & ((int) (d) & ((int) (d) & ((int) (d) & ((int) (d) & (b & ((int) (d) &  b))))))));
	RETURN_DOUBLE(c);

}

PHP_METHOD(Test_Bitwise, complex23And) {

	double d = 0;
	zend_bool b = 0;
	int a = 0;


	a = 1;
	b = (2.0) ? 1 : 0;
	d = (double) ((a & b));
	RETURN_DOUBLE(d);

}

PHP_METHOD(Test_Bitwise, complex24And) {

	zend_bool b = 0;
	int a = 0, d = 0;


	a = 1;
	b = (2.0) ? 1 : 0;
	d = (1 & (a & ((int) (0) & b)));
	RETURN_LONG(d);

}

PHP_METHOD(Test_Bitwise, intOr) {

	int a = 0, b = 0, c = 0;


	a = 1;
	b = 2;
	c = (a | b);
	RETURN_LONG(c);

}

PHP_METHOD(Test_Bitwise, int2Or) {

	int a = 0, c = 0;


	a = 1;
	c = (a | 2);
	RETURN_LONG(c);

}

PHP_METHOD(Test_Bitwise, intOrSimple) {

	int c = 0;


	c = 3;
	RETURN_LONG(c);

}

PHP_METHOD(Test_Bitwise, boolOr) {

	zend_bool a = 0, b = 0, c = 0;


	a = (1) ? 1 : 0;
	b = (2) ? 1 : 0;
	c = ((a | b)) ? 1 : 0;
	RETURN_BOOL(c);

}

PHP_METHOD(Test_Bitwise, bool2Or) {

	zend_bool a = 0, c = 0;


	a = (1) ? 1 : 0;
	c = ((a | 2)) ? 1 : 0;
	RETURN_BOOL(c);

}

PHP_METHOD(Test_Bitwise, bool3Or) {

	zend_bool a = 0, c = 0;


	a = 1;
	c = ((a | 0)) ? 1 : 0;
	RETURN_BOOL(c);

}

PHP_METHOD(Test_Bitwise, bool4Or) {

	zend_bool a = 0, c = 0;


	a = 1;
	c = ((a | 1)) ? 1 : 0;
	RETURN_BOOL(c);

}

PHP_METHOD(Test_Bitwise, boolOrSimple) {

	zend_bool c = 0;


	c = (3) ? 1 : 0;
	RETURN_BOOL(c);

}

PHP_METHOD(Test_Bitwise, doubleOr) {

	double a = 0, b = 0, c = 0;


	a = (double) (1);
	b = (double) (2);
	c = (double) (((int) (a) | (int) (b)));
	RETURN_DOUBLE(c);

}

PHP_METHOD(Test_Bitwise, double2Or) {

	double a = 0, b = 0, c = 0;


	a = 1.0;
	b = 2.0;
	c = (double) (((int) (a) | (int) (b)));
	RETURN_DOUBLE(c);

}

PHP_METHOD(Test_Bitwise, doubleOrSimple) {

	double c = 0;


	c = (double) (3);
	RETURN_DOUBLE(c);

}

PHP_METHOD(Test_Bitwise, doubleOr2Simple) {

	double c = 0;


	c = (double) (3.0);
	RETURN_DOUBLE(c);

}

PHP_METHOD(Test_Bitwise, varOr) {

	int a = 0, b = 0, c = 0;


	a = 1;
	b = 2;
	c = (a | b);
	RETURN_LONG(c);

}

PHP_METHOD(Test_Bitwise, varOrSimple) {

	int c = 0;


	c = 3;
	RETURN_LONG(c);

}

PHP_METHOD(Test_Bitwise, intDoubleOr) {

	double a = 0;
	int b = 0, c = 0;


	a = (double) (1);
	b = 2;
	c = ((int) (a) |  b);
	RETURN_LONG(c);

}

PHP_METHOD(Test_Bitwise, intDoubleOrSimple) {

	int c = 0;


	c = 3.0;
	RETURN_LONG(c);

}

PHP_METHOD(Test_Bitwise, doubleIntOr) {

	double a = 0, c = 0;
	int b = 0;


	a = (double) (1);
	b = 2;
	c = (double) (((int) (a) |  b));
	RETURN_DOUBLE(c);

}

PHP_METHOD(Test_Bitwise, doubleIntOrSimple) {

	double c = 0;


	c = (double) (3.0);
	RETURN_DOUBLE(c);

}

PHP_METHOD(Test_Bitwise, varIntOr) {

	int b = 0, c = 0, a = 0;


	a = 1;
	b = 2;
	c = (a | b);
	RETURN_LONG(c);

}

PHP_METHOD(Test_Bitwise, intVarOr) {

	int b = 0, c = 0, a = 0;


	a = 1;
	b = 2;
	c = (b | a);
	RETURN_LONG(c);

}

PHP_METHOD(Test_Bitwise, intVarImplicitCastOr) {

	int c = 0;
	long a = 0, b = 0;


	a = 1;
	b = 2;
	c = (b | a);
	RETURN_LONG(c);

}

PHP_METHOD(Test_Bitwise, intVarImplicitCast2Or) {

	int b = 0, c = 0;
	long a = 0;


	a = 1;
	b = 2;
	c = (b | a);
	RETURN_LONG(c);

}

PHP_METHOD(Test_Bitwise, complexOr) {

	double c = 0;


	c = (double) (3.0);
	RETURN_DOUBLE(c);

}

PHP_METHOD(Test_Bitwise, complex2Or) {

	zend_bool c = 0;


	c = ((1 | 1)) ? 1 : 0;
	RETURN_BOOL(c);

}

PHP_METHOD(Test_Bitwise, complex3Or) {

	zend_bool c = 0;


	c = ((1 |((1.0) ? 1 : 0))) ? 1 : 0;
	RETURN_BOOL(c);

}

PHP_METHOD(Test_Bitwise, complex4Or) {

	zval *c = NULL;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(c);
	ZVAL_LONG(c, ((int) (1.0) | 1));
	RETURN_CCTOR(c);

}

PHP_METHOD(Test_Bitwise, complex5Or) {

	int c = 0;


	c = (1 | 1);
	RETURN_LONG(c);

}

PHP_METHOD(Test_Bitwise, complex6Or) {

	zend_bool c = 0;


	c = ((1 |((1) ? 1 : 0))) ? 1 : 0;
	RETURN_BOOL(c);

}

PHP_METHOD(Test_Bitwise, complex7Or) {

	int b = 0, c = 0;
	zend_bool a = 0;


	a = (1) ? 1 : 0;
	b = 2;
	c = (a | b);
	RETURN_LONG(c);

}

PHP_METHOD(Test_Bitwise, complex9Or) {

	int b = 0, c = 0;
	zend_bool a = 0;


	a = (1) ? 1 : 0;
	b = 2;
	c = (b | a);
	RETURN_LONG(c);

}

PHP_METHOD(Test_Bitwise, complex10Or) {

	double c = 0;
	int b = 0;
	zend_bool a = 0;


	a = (1.0) ? 1 : 0;
	b = 2;
	c = (double) ((b | a));
	RETURN_DOUBLE(c);

}

PHP_METHOD(Test_Bitwise, complex11Or) {

	double c = 0;
	int b = 0;
	zend_bool a = 0;


	a = (1) ? 1 : 0;
	b = (long) (2.0);
	c = (double) ((b | a));
	RETURN_DOUBLE(c);

}

PHP_METHOD(Test_Bitwise, complex12Or) {

	int b = 0, c = 0;
	zend_bool a = 0;


	a = (1) ? 1 : 0;
	b = (long) (2.0);
	c = (b | a);
	RETURN_LONG(c);

}

PHP_METHOD(Test_Bitwise, complex13Or) {

	int c = 0;
	zend_bool a = 0, b = 0;


	a = (1) ? 1 : 0;
	b = (2.0) ? 1 : 0;
	c = (b | a);
	RETURN_LONG(c);

}

PHP_METHOD(Test_Bitwise, complex14Or) {

	zend_bool b = 0;
	int a = 0, c = 0;


	a = 1;
	b = (2.0) ? 1 : 0;
	c = (b | a);
	RETURN_LONG(c);

}

PHP_METHOD(Test_Bitwise, complex15Or) {

	zend_bool c = 0;
	double b = 0;
	int a = 0;


	a = 1;
	b = 2.0;
	c = (((int) (b) |  a)) ? 1 : 0;
	RETURN_BOOL(c);

}

PHP_METHOD(Test_Bitwise, complex16Or) {

	double b = 0, c = 0;
	int a = 0;


	a = 1;
	b = 2.0;
	c = (double) (((int) (b) |  a));
	RETURN_DOUBLE(c);

}

PHP_METHOD(Test_Bitwise, complex17Or) {

	double c = 0;
	zend_bool b = 0;
	int a = 0;


	a = 1;
	b = (2.0) ? 1 : 0;
	c = (double) ((a | b));
	RETURN_DOUBLE(c);

}

PHP_METHOD(Test_Bitwise, complex18Or) {

	zend_bool b = 0;
	double d = 0, c = 0;
	int a = 0;


	a = 1;
	b = (2.0) ? 1 : 0;
	d = (double) ((a | b));
	c = (double) (((a | b) | (int) (d)));
	RETURN_DOUBLE(c);

}

PHP_METHOD(Test_Bitwise, complex19Or) {

	zend_bool b = 0;
	double d = 0, c = 0;
	int a = 0;


	a = 1;
	b = (2.0) ? 1 : 0;
	d = (double) ((a | b));
	c = (double) (((a | (int) (d)) | b));
	RETURN_DOUBLE(c);

}

PHP_METHOD(Test_Bitwise, complex20Or) {

	zend_bool b = 0;
	double d = 0, c = 0;
	int a = 0;


	a = 1;
	b = (2.0) ? 1 : 0;
	d = (double) ((a | b));
	c = (double) (((((int) (d) | (int) (d)) | (int) (d)) | (int) (d)));
	RETURN_DOUBLE(c);

}

PHP_METHOD(Test_Bitwise, complex21Or) {

	zend_bool b = 0;
	double d = 0, c = 0;
	int a = 0;


	a = 1;
	b = (2.0) ? 1 : 0;
	d = (double) ((a | b));
	c = (double) ((((((((b | (int) (d)) | b) | (int) (d)) | (int) (d)) | b) | (int) (d)) | b));
	RETURN_DOUBLE(c);

}

PHP_METHOD(Test_Bitwise, complex22Or) {

	double d = 0, c = 0;
	int a = 0, b = 0;


	a = 1;
	b = (long) (2.0);
	d = (double) ((a | b));
	c = (double) ((((((((b | (int) (d)) | (int) (d)) | (int) (d)) | (int) (d)) | b) | (int) (d)) | b));
	RETURN_DOUBLE(c);

}

PHP_METHOD(Test_Bitwise, complex23Or) {

	double d = 0;
	zend_bool b = 0;
	int a = 0;


	a = 1;
	b = (2.0) ? 1 : 0;
	d = (double) ((a | b));
	RETURN_DOUBLE(d);

}

PHP_METHOD(Test_Bitwise, complex24Or) {

	zend_bool b = 0;
	double d = 0;
	int a = 0;


	a = 1;
	b = (2.0) ? 1 : 0;
	d = (double) ((((1 | a) | 0) | b));
	RETURN_DOUBLE(d);

}

PHP_METHOD(Test_Bitwise, intShiftLeft) {

	int a = 0, b = 0, c = 0;


	a = 1;
	b = 2;
	c = (a << b);
	RETURN_LONG(c);

}

PHP_METHOD(Test_Bitwise, int2ShiftLeft) {

	int a = 0, c = 0;


	a = 1;
	c = (a << 2);
	RETURN_LONG(c);

}

PHP_METHOD(Test_Bitwise, intShiftLeftSimple) {

	int c = 0;


	c = 4;
	RETURN_LONG(c);

}

PHP_METHOD(Test_Bitwise, intShiftRight) {

	int a = 0, b = 0, c = 0;


	a = 1;
	b = 2;
	c = (a >> b);
	RETURN_LONG(c);

}

PHP_METHOD(Test_Bitwise, int2ShiftRight) {

	int a = 0, c = 0;


	a = 1;
	c = (a >> 2);
	RETURN_LONG(c);

}

PHP_METHOD(Test_Bitwise, intShiftRightSimple) {

	int c = 0;


	c = 0;
	RETURN_LONG(c);

}

PHP_METHOD(Test_Bitwise, testBitwiseNot) {

	zval *a_param = NULL;
	int a;

	zephir_fetch_params(0, 1, 0, &a_param);

	a = zephir_get_intval(a_param);


	RETURN_LONG(~a);

}

PHP_METHOD(Test_Bitwise, testBitwiseAndNot) {

	zval *a_param = NULL, *b_param = NULL;
	int a, b;

	zephir_fetch_params(0, 2, 0, &a_param, &b_param);

	a = zephir_get_intval(a_param);
	b = zephir_get_intval(b_param);


	RETURN_LONG((a & ~b));

}

