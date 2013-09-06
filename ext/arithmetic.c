
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_test.h"
#include "test.h"

#include "Zend/zend_operators.h"
#include "Zend/zend_exceptions.h"
#include "Zend/zend_interfaces.h"

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/operators.h"


/**
 * Arithmetic operations
 */
ZEPHIR_INIT_CLASS(Test_Arithmetic) {

	ZEPHIR_REGISTER_CLASS(Test, Arithmetic, arithmetic, test_arithmetic_method_entry, 0);


	return SUCCESS;

}

PHP_METHOD(Test_Arithmetic, intSum) {

	int a, b, c;


	a = 1;
	b = 2;
	c = (a + b);
	RETURN_LONG(c);

}

PHP_METHOD(Test_Arithmetic, int2Sum) {

	int a, c;


	a = 1;
	c = (a + 2);
	RETURN_LONG(c);

}

PHP_METHOD(Test_Arithmetic, intSumSimple) {

	int c;


	c = (1 + 2);
	RETURN_LONG(c);

}

PHP_METHOD(Test_Arithmetic, boolSum) {

	zend_bool a, b, c;


	a = (1) ? 1 : 0;
	b = (2) ? 1 : 0;
	c = (a | b);
	RETURN_BOOL(c);

}

PHP_METHOD(Test_Arithmetic, bool2Sum) {

	zend_bool a, c;


	a = (1) ? 1 : 0;
	c = (a + 2);
	RETURN_BOOL(c);

}

PHP_METHOD(Test_Arithmetic, bool3Sum) {

	zend_bool a, c;


	a = 1;
	c = (a | 0);
	RETURN_BOOL(c);

}

PHP_METHOD(Test_Arithmetic, boolSumSimple) {

	zend_bool c;


	c = ((1 + 2)) ? 1 : 0;
	RETURN_BOOL(c);

}

PHP_METHOD(Test_Arithmetic, doubleSum) {

	double a, b, c;


	a = (double) (1);
	b = (double) (2);
	c = (a + b);
	RETURN_DOUBLE(c);

}

PHP_METHOD(Test_Arithmetic, double2Sum) {

	double a, b, c;


	a = 1.0;
	b = 2.0;
	c = (a + b);
	RETURN_DOUBLE(c);

}

PHP_METHOD(Test_Arithmetic, doubleSumSimple) {

	double c;


	c = (double) ((1 + 2));
	RETURN_DOUBLE(c);

}

PHP_METHOD(Test_Arithmetic, doubleSum2Simple) {

	double c;


	c = (1.0 + 2.0);
	RETURN_DOUBLE(c);

}

PHP_METHOD(Test_Arithmetic, varSum) {

	zval *c;
	int a, b;

	ZEPHIR_MM_GROW();

	a = 1;
	b = 2;
	ZEPHIR_INIT_VAR(c);
	ZVAL_LONG(c, (a + b));
	RETURN_CCTOR(c);

}

PHP_METHOD(Test_Arithmetic, varSumSimple) {

	int c;


	c = (1 + 2);
	RETURN_LONG(c);

}

PHP_METHOD(Test_Arithmetic, intDoubleSum) {

	double a;
	int b, c;


	a = (double) (1);
	b = 2;
	c = (long) ((a +  (double) b));
	RETURN_LONG(c);

}

PHP_METHOD(Test_Arithmetic, intDoubleSumSimple) {

	int c;


	c = (long) (((double) 1 + 2.0));
	RETURN_LONG(c);

}

PHP_METHOD(Test_Arithmetic, doubleIntSum) {

	double a, c;
	int b;


	a = (double) (1);
	b = 2;
	c = (a +  (double) b);
	RETURN_DOUBLE(c);

}

PHP_METHOD(Test_Arithmetic, doubleIntSumSimple) {

	double c;


	c = ((double) 1 + 2.0);
	RETURN_DOUBLE(c);

}

PHP_METHOD(Test_Arithmetic, varIntSum) {

	zval *c;
	int b, a;

	ZEPHIR_MM_GROW();

	a = 1;
	b = 2;
	ZEPHIR_INIT_VAR(c);
	ZVAL_LONG(c, (a + b));
	RETURN_CCTOR(c);

}

PHP_METHOD(Test_Arithmetic, intVarSum) {

	int b, c, a;


	a = 1;
	b = 2;
	c = (b + a);
	RETURN_LONG(c);

}

PHP_METHOD(Test_Arithmetic, intVarImplicitCastSum) {

	int c;
	zval *a, *b, *_0;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	ZVAL_STRING(a, "1", 1);
	ZEPHIR_INIT_VAR(b);
	ZVAL_STRING(b, "2", 1);
	ZEPHIR_INIT_VAR(_0);
	zephir_add_function(_0, b, a TSRMLS_CC);
	c = zephir_get_numberval(_0);
	RETURN_MM_LONG(c);

}

PHP_METHOD(Test_Arithmetic, intVarImplicitCast2Sum) {

	int b, c;
	zval *a;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	ZVAL_STRING(a, "1", 1);
	b = 2;
	c = (b + zephir_get_numberval(a));
	RETURN_MM_LONG(c);

}

PHP_METHOD(Test_Arithmetic, complexSum) {

	double c;


	c = ((double) 2 + 1.0);
	RETURN_DOUBLE(c);

}

PHP_METHOD(Test_Arithmetic, complex2Sum) {

	zend_bool c;


	c = (1 | 1);
	RETURN_BOOL(c);

}

PHP_METHOD(Test_Arithmetic, complex3Sum) {

	zval *c;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(c);
	ZVAL_BOOL(c, (1 |((1.0) ? 1 : 0)));
	RETURN_CCTOR(c);

}

PHP_METHOD(Test_Arithmetic, complex4Sum) {

	zval *c;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(c);
	ZVAL_DOUBLE(c, (1.0 + 1));
	RETURN_CCTOR(c);

}

PHP_METHOD(Test_Arithmetic, complex5Sum) {

	int c;


	c = (1 + 1);
	RETURN_LONG(c);

}

PHP_METHOD(Test_Arithmetic, complex6Sum) {

	int c;


	c = (1 |((1) ? 1 : 0));
	RETURN_LONG(c);

}

PHP_METHOD(Test_Arithmetic, complex7Sum) {

	zval *c;
	int b;
	zend_bool a;

	ZEPHIR_MM_GROW();

	a = (1) ? 1 : 0;
	b = 2;
	ZEPHIR_INIT_VAR(c);
	ZVAL_LONG(c, (a + b));
	RETURN_CCTOR(c);

}

PHP_METHOD(Test_Arithmetic, complex9Sum) {

	zval *c;
	int b;
	zend_bool a;

	ZEPHIR_MM_GROW();

	a = (1) ? 1 : 0;
	b = 2;
	ZEPHIR_INIT_VAR(c);
	ZVAL_LONG(c, (b + a));
	RETURN_CCTOR(c);

}

PHP_METHOD(Test_Arithmetic, complex10Sum) {

	zval *c;
	int b;
	zend_bool a;

	ZEPHIR_MM_GROW();

	a = (1.0) ? 1 : 0;
	b = 2;
	ZEPHIR_INIT_VAR(c);
	ZVAL_LONG(c, (b + a));
	RETURN_CCTOR(c);

}

PHP_METHOD(Test_Arithmetic, complex11Sum) {

	zval *c;
	int b;
	zend_bool a;

	ZEPHIR_MM_GROW();

	a = (1) ? 1 : 0;
	b = (long) (2.0);
	ZEPHIR_INIT_VAR(c);
	ZVAL_LONG(c, (b + a));
	RETURN_CCTOR(c);

}

PHP_METHOD(Test_Arithmetic, complex12Sum) {

	int b, c;
	zend_bool a;


	a = (1) ? 1 : 0;
	b = (long) (2.0);
	c = (b + a);
	RETURN_LONG(c);

}

PHP_METHOD(Test_Arithmetic, complex13Sum) {

	int c;
	zend_bool a, b;


	a = (1) ? 1 : 0;
	b = (2.0) ? 1 : 0;
	c = (b | a);
	RETURN_LONG(c);

}

PHP_METHOD(Test_Arithmetic, complex14Sum) {

	zend_bool b;
	int a, c;


	a = 1;
	b = (2.0) ? 1 : 0;
	c = (b + a);
	RETURN_LONG(c);

}

PHP_METHOD(Test_Arithmetic, complex15Sum) {

	zend_bool c;
	double b;
	int a;


	a = 1;
	b = 2.0;
	c = ((b +  (double) a)) ? 1 : 0;
	RETURN_BOOL(c);

}

PHP_METHOD(Test_Arithmetic, complex16Sum) {

	double b, c;
	int a;


	a = 1;
	b = 2.0;
	c = (b +  (double) a);
	RETURN_DOUBLE(c);

}

PHP_METHOD(Test_Arithmetic, complex17Sum) {

	double c;
	zend_bool b;
	int a;


	a = 1;
	b = (2.0) ? 1 : 0;
	c = (double) ((a + b));
	RETURN_DOUBLE(c);

}

PHP_METHOD(Test_Arithmetic, complex18Sum) {

	double c;
	zend_bool b;
	zval d;
	int a;


	a = 1;
	b = (2.0) ? 1 : 0;
	ZEPHIR_SINIT_VAR(d);
	ZVAL_LONG(&d, (a + b));
	c = (double) (((a + b) + zephir_get_numberval(&d)));
	RETURN_DOUBLE(c);

}

PHP_METHOD(Test_Arithmetic, complex19Sum) {

	double c;
	zend_bool b;
	zval d;
	int a;


	a = 1;
	b = (2.0) ? 1 : 0;
	ZEPHIR_SINIT_VAR(d);
	ZVAL_LONG(&d, (a + b));
	c = (double) (((a + zephir_get_numberval(&d)) + b));
	RETURN_DOUBLE(c);

}

PHP_METHOD(Test_Arithmetic, complex20Sum) {

	double c;
	zend_bool b;
	zval d, *_0 = NULL, *_1;
	int a;

	ZEPHIR_MM_GROW();

	a = 1;
	b = (2.0) ? 1 : 0;
	ZEPHIR_SINIT_VAR(d);
	ZVAL_LONG(&d, (a + b));
	ZEPHIR_INIT_VAR(_0);
	zephir_add_function(_0, &d, &d TSRMLS_CC);
	ZEPHIR_INIT_VAR(_1);
	zephir_add_function(_1, _0, &d TSRMLS_CC);
	ZEPHIR_INIT_LNVAR(_0);
	zephir_add_function(_0, _1, &d TSRMLS_CC);
	c = zephir_get_numberval(_0);
	RETURN_MM_DOUBLE(c);

}

PHP_METHOD(Test_Arithmetic, complex21Sum) {

	double c;
	zend_bool b;
	zval d;
	int a;


	a = 1;
	b = (2.0) ? 1 : 0;
	ZEPHIR_SINIT_VAR(d);
	ZVAL_LONG(&d, (a + b));
	c = (double) ((((((((b + zephir_get_numberval(&d)) + b) + zephir_get_numberval(&d)) + zephir_get_numberval(&d)) + b) + zephir_get_numberval(&d)) + b));
	RETURN_DOUBLE(c);

}

PHP_METHOD(Test_Arithmetic, complex22Sum) {

	double c;
	zval d;
	int a, b;


	a = 1;
	b = (long) (2.0);
	ZEPHIR_SINIT_VAR(d);
	ZVAL_LONG(&d, (a + b));
	c = (double) ((((((((b + zephir_get_numberval(&d)) + zephir_get_numberval(&d)) + zephir_get_numberval(&d)) + zephir_get_numberval(&d)) + b) + zephir_get_numberval(&d)) + b));
	RETURN_DOUBLE(c);

}

PHP_METHOD(Test_Arithmetic, complex23Sum) {

	double d;
	zend_bool b;
	int a;


	a = 1;
	b = (2.0) ? 1 : 0;
	d = (double) ((a + b));
	RETURN_DOUBLE(d);

}

PHP_METHOD(Test_Arithmetic, complex24Sum) {

	zend_bool b;
	zval *d;
	int a;

	ZEPHIR_MM_GROW();

	a = 1;
	b = (2.0) ? 1 : 0;
	ZEPHIR_INIT_VAR(d);
	ZVAL_LONG(d, (((1 + a) + 0) + b));
	RETURN_CCTOR(d);

}

PHP_METHOD(Test_Arithmetic, addSum1) {

	int a;


	a = 0;
	a += 10;
	RETURN_LONG(a);

}

PHP_METHOD(Test_Arithmetic, addSum2) {

	int a;


	a = 0;
	a += 0;
	RETURN_LONG(a);

}

PHP_METHOD(Test_Arithmetic, addSum3) {

	int a;


	a = 0;
	a += 1;
	RETURN_LONG(a);

}

PHP_METHOD(Test_Arithmetic, addSum4) {

	int a;


	a = 0;
	a += 0;
	RETURN_LONG(a);

}

PHP_METHOD(Test_Arithmetic, addSum5) {

	int a;


	a = 0;
	a += (long) (1.0);
	RETURN_LONG(a);

}

PHP_METHOD(Test_Arithmetic, addSum6) {

	int a;


	a = 0;
	a += (long) (1.0);
	RETURN_LONG(a);

}

PHP_METHOD(Test_Arithmetic, addSum7) {

	double a;


	a = 0.0;
	a += (double) (10);
	RETURN_DOUBLE(a);

}

PHP_METHOD(Test_Arithmetic, addSum8) {

	double a;


	a = 0.0;
	a += 0.0;
	RETURN_DOUBLE(a);

}

PHP_METHOD(Test_Arithmetic, addSum9) {

	double a;


	a = 0.0;
	a += 1;
	RETURN_DOUBLE(a);

}

PHP_METHOD(Test_Arithmetic, addSum10) {

	double a;


	a = 0.0;
	a += 0;
	RETURN_DOUBLE(a);

}

PHP_METHOD(Test_Arithmetic, addSum11) {

	double a;


	a = 0.0;
	a += 1.0;
	RETURN_DOUBLE(a);

}

PHP_METHOD(Test_Arithmetic, addSum12) {

	double a;


	a = 0.0;
	a += 1.0;
	RETURN_DOUBLE(a);

}

PHP_METHOD(Test_Arithmetic, addSum13) {

	int a, b;


	a = 0;
	b = 10;
	a += b;
	RETURN_LONG(a);

}

PHP_METHOD(Test_Arithmetic, addSum14) {

	zend_bool b;
	int a;


	a = 0;
	b = 1;
	a += b;
	RETURN_LONG(a);

}

PHP_METHOD(Test_Arithmetic, addSum15) {

	zend_bool b;
	int a;


	a = 0;
	b = 0;
	a += b;
	RETURN_LONG(a);

}

PHP_METHOD(Test_Arithmetic, addSum16) {

	double b;
	int a;


	a = 0;
	b = 1.0;
	a += (long) b;
	RETURN_LONG(a);

}

PHP_METHOD(Test_Arithmetic, addSum17) {

	double a, b;


	a = 0.0;
	b = 10.0;
	a += b;
	RETURN_DOUBLE(a);

}

PHP_METHOD(Test_Arithmetic, addSum18) {

	zend_bool b;
	double a;


	a = 0.0;
	b = 1;
	a += (double) b;
	RETURN_DOUBLE(a);

}

PHP_METHOD(Test_Arithmetic, addSum19) {

	zend_bool b;
	double a;


	a = 0.0;
	b = 0;
	a += (double) b;
	RETURN_DOUBLE(a);

}

PHP_METHOD(Test_Arithmetic, addSum20) {

	int b;
	double a;


	a = 0.0;
	b = 1;
	a += (double) b;
	RETURN_DOUBLE(a);

}

PHP_METHOD(Test_Arithmetic, addSum21) {

	int a, b;


	a = (long) (0.0);
	b = 1;
	a += b;
	RETURN_LONG(a);

}

PHP_METHOD(Test_Arithmetic, addSum22) {

	int b;
	double a;


	a = 0.0;
	b = 1;
	a += (double) b;
	RETURN_DOUBLE(a);

}

PHP_METHOD(Test_Arithmetic, addSum23) {

	int b;
	double a;


	a = 0.0;
	b = 1;
	a += (double) b;
	RETURN_DOUBLE(a);

}

PHP_METHOD(Test_Arithmetic, complexSub) {

	double c;


	c = ((double) 2 - 1.0);
	RETURN_DOUBLE(c);

}

