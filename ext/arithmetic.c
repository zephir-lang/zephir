
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


	c = 3;
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


	c = (3) ? 1 : 0;
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


	c = (double) (3);
	RETURN_DOUBLE(c);

}

PHP_METHOD(Test_Arithmetic, doubleSum2Simple) {

	double c;


	c = 3;
	RETURN_DOUBLE(c);

}

PHP_METHOD(Test_Arithmetic, varSum) {

	int a, b, c;


	a = 1;
	b = 2;
	c = (a + b);
	RETURN_LONG(c);

}

PHP_METHOD(Test_Arithmetic, varSumSimple) {

	int c;


	c = 3;
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


	c = (long) (3);
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


	c = 3;
	RETURN_DOUBLE(c);

}

PHP_METHOD(Test_Arithmetic, varIntSum) {

	int b, c, a;


	a = 1;
	b = 2;
	c = (a + b);
	RETURN_LONG(c);

}

PHP_METHOD(Test_Arithmetic, intVarSum) {

	int b, c, a;


	a = 1;
	b = 2;
	c = (b + a);
	RETURN_LONG(c);

}

PHP_METHOD(Test_Arithmetic, intVarImplicitCastSum) {

	int a, b, c;


	a = 1;
	b = 2;
	c = (b + a);
	RETURN_LONG(c);

}

PHP_METHOD(Test_Arithmetic, intVarImplicitCast2Sum) {

	int a, b, c;


	a = 1;
	b = 2;
	c = (b + a);
	RETURN_LONG(c);

}

PHP_METHOD(Test_Arithmetic, complexSum) {

	double c;


	c = 3;
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

	int b, c;
	zend_bool a;


	a = (1) ? 1 : 0;
	b = 2;
	c = (a + b);
	RETURN_LONG(c);

}

PHP_METHOD(Test_Arithmetic, complex9Sum) {

	int b, c;
	zend_bool a;


	a = (1) ? 1 : 0;
	b = 2;
	c = (b + a);
	RETURN_LONG(c);

}

PHP_METHOD(Test_Arithmetic, complex10Sum) {

	double c;
	int b;
	zend_bool a;


	a = (1.0) ? 1 : 0;
	b = 2;
	c = (double) ((b + a));
	RETURN_DOUBLE(c);

}

PHP_METHOD(Test_Arithmetic, complex11Sum) {

	double c;
	int b;
	zend_bool a;


	a = (1) ? 1 : 0;
	b = (long) (2.0);
	c = (double) ((b + a));
	RETURN_DOUBLE(c);

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

	zend_bool b;
	double d, c;
	int a;


	a = 1;
	b = (2.0) ? 1 : 0;
	d = (double) ((a + b));
	c = (double) ((a + b) + d);
	RETURN_DOUBLE(c);

}

PHP_METHOD(Test_Arithmetic, complex19Sum) {

	zend_bool b;
	double d, c;
	int a;


	a = 1;
	b = (2.0) ? 1 : 0;
	d = (double) ((a + b));
	c = (((double) a + d) + b);
	RETURN_DOUBLE(c);

}

PHP_METHOD(Test_Arithmetic, complex20Sum) {

	zend_bool b;
	double d, c;
	int a;


	a = 1;
	b = (2.0) ? 1 : 0;
	d = (double) ((a + b));
	c = (double) ((double) ((d + d) + d) + d);
	RETURN_DOUBLE(c);

}

PHP_METHOD(Test_Arithmetic, complex21Sum) {

	zend_bool b;
	double d, c;
	int a;


	a = 1;
	b = (2.0) ? 1 : 0;
	d = (double) ((a + b));
	c = ((double) (((double) ((double) (((b + d) + b) + d) + d) + b) + d) + b);
	RETURN_DOUBLE(c);

}

PHP_METHOD(Test_Arithmetic, complex22Sum) {

	double d, c;
	int a, b;


	a = 1;
	b = (long) (2.0);
	d = (double) ((a + b));
	c = ((double) (((double) ((double) ((double) (((double) b + d) + d) + d) + d) + b) + d) + b);
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
	double d;
	int a;


	a = 1;
	b = (2.0) ? 1 : 0;
	d = (double) ((((1 + a) + 0) + b));
	RETURN_DOUBLE(d);

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
	zval *a = NULL;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	ZVAL_DOUBLE(a, 0.0);
	b = 1;
	ZEPHIR_INIT_NVAR(a);
	ZVAL_LONG(a, zephir_get_numberval(&a) + b);
	RETURN_CCTOR(a);

}

PHP_METHOD(Test_Arithmetic, complexSub) {

	double c;


	c = 1;
	RETURN_DOUBLE(c);

}

