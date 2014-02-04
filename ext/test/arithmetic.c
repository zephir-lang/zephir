
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
ZEPHIR_INIT_CLASS(Test_Arithmetic) {

	ZEPHIR_REGISTER_CLASS(Test, Arithmetic, test, arithmetic, test_arithmetic_method_entry, 0);


	return SUCCESS;

}

/* ---------- SUM ----------------*/
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

	zend_bool c;


	c = (1 |((1.0) ? 1 : 0));
	RETURN_BOOL(c);

}

PHP_METHOD(Test_Arithmetic, complex4Sum) {

	zval c;


	ZEPHIR_SINIT_VAR(c);
	ZVAL_DOUBLE(&c, (1.0 + 1));
	RETURN_LCTORW(c);

}

PHP_METHOD(Test_Arithmetic, complex5Sum) {

	int c;


	c = (1 + 1);
	RETURN_LONG(c);

}

PHP_METHOD(Test_Arithmetic, complex6Sum) {

	zend_bool c;


	c = (1 |((1) ? 1 : 0));
	RETURN_BOOL(c);

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

PHP_METHOD(Test_Arithmetic, addSum2b) {

	int a;


	a = 5;
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

PHP_METHOD(Test_Arithmetic, addSum8b) {

	double a;


	a = 6.3;
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

	int a, b;


	a = 0;
	b = 10;
	a += b;
	RETURN_LONG(a);

}

PHP_METHOD(Test_Arithmetic, addSum13) {

	zend_bool b;
	int a;


	a = 0;
	b = 1;
	a += b;
	RETURN_LONG(a);

}

PHP_METHOD(Test_Arithmetic, addSum14) {

	zend_bool b;
	int a;


	a = 0;
	b = 0;
	a += b;
	RETURN_LONG(a);

}

PHP_METHOD(Test_Arithmetic, addSum15) {

	double b;
	int a;


	a = 0;
	b = 1.0;
	a += (long) b;
	RETURN_LONG(a);

}

PHP_METHOD(Test_Arithmetic, addSum16) {

	double a, b;


	a = 0.0;
	b = 10.0;
	a += b;
	RETURN_DOUBLE(a);

}

PHP_METHOD(Test_Arithmetic, addSum17) {

	zend_bool b;
	double a;


	a = 0.0;
	b = 1;
	a += (double) b;
	RETURN_DOUBLE(a);

}

PHP_METHOD(Test_Arithmetic, addSum18) {

	zend_bool b;
	double a;


	a = 0.0;
	b = 0;
	a += (double) b;
	RETURN_DOUBLE(a);

}

PHP_METHOD(Test_Arithmetic, addSum19) {

	int b;
	double a;


	a = 0.0;
	b = 1;
	a += (double) b;
	RETURN_DOUBLE(a);

}

PHP_METHOD(Test_Arithmetic, addSum20) {

	int a, b;


	a = (long) (0.0);
	b = 1;
	a += b;
	RETURN_LONG(a);

}

PHP_METHOD(Test_Arithmetic, addSum21) {

	int b;
	double a;


	a = 0.0;
	b = 1;
	a += (double) b;
	RETURN_DOUBLE(a);

}

PHP_METHOD(Test_Arithmetic, addSum22) {

	int b, _0;
	zval *a;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	ZVAL_DOUBLE(a, 0.0);
	b = 1;
	ZEPHIR_INIT_BNVAR(a);
	_0 = zephir_get_numberval(a);
	ZVAL_LONG(a, _0 + b);
	RETURN_CCTOR(a);

}

/* --------------- SUB ----------------*/
PHP_METHOD(Test_Arithmetic, intSub) {

	int a, b, c;


	a = 1;
	b = 2;
	c = (a - b);
	RETURN_LONG(c);

}

PHP_METHOD(Test_Arithmetic, int2Sub) {

	int a, c;


	a = 1;
	c = (a - 2);
	RETURN_LONG(c);

}

PHP_METHOD(Test_Arithmetic, intSubSimple) {

	int c;


	c = -1;
	RETURN_LONG(c);

}

PHP_METHOD(Test_Arithmetic, boolSub) {

	zend_bool a, b, c;


	a = (1) ? 1 : 0;
	b = (2) ? 1 : 0;
	c = (a & b);
	RETURN_BOOL(c);

}

PHP_METHOD(Test_Arithmetic, bool2Sub) {

	zend_bool a, c;


	a = (1) ? 1 : 0;
	c = (a - 2);
	RETURN_BOOL(c);

}

PHP_METHOD(Test_Arithmetic, bool3Sub) {

	zend_bool a, c;


	a = 1;
	c = (a & 0);
	RETURN_BOOL(c);

}

PHP_METHOD(Test_Arithmetic, bool4Sub) {

	zend_bool a, c;


	a = 1;
	c = (a & 1);
	RETURN_BOOL(c);

}

PHP_METHOD(Test_Arithmetic, boolSubSimple) {

	zend_bool c;


	c = (-1) ? 1 : 0;
	RETURN_BOOL(c);

}

PHP_METHOD(Test_Arithmetic, doubleSub) {

	double a, b, c;


	a = (double) (1);
	b = (double) (2);
	c = (a - b);
	RETURN_DOUBLE(c);

}

PHP_METHOD(Test_Arithmetic, double2Sub) {

	double a, b, c;


	a = 1.0;
	b = 2.0;
	c = (a - b);
	RETURN_DOUBLE(c);

}

PHP_METHOD(Test_Arithmetic, doubleSubSimple) {

	double c;


	c = (double) (-1);
	RETURN_DOUBLE(c);

}

PHP_METHOD(Test_Arithmetic, doubleSub2Simple) {

	double c;


	c = -1;
	RETURN_DOUBLE(c);

}

PHP_METHOD(Test_Arithmetic, varSub) {

	int a, b, c;


	a = 1;
	b = 2;
	c = (a - b);
	RETURN_LONG(c);

}

PHP_METHOD(Test_Arithmetic, varSubSimple) {

	int c;


	c = -1;
	RETURN_LONG(c);

}

PHP_METHOD(Test_Arithmetic, intDoubleSub) {

	double a;
	int b, c;


	a = (double) (1);
	b = 2;
	c = (long) ((a -  (double) b));
	RETURN_LONG(c);

}

PHP_METHOD(Test_Arithmetic, intDoubleSubSimple) {

	int c;


	c = (long) (-1);
	RETURN_LONG(c);

}

PHP_METHOD(Test_Arithmetic, doubleIntSub) {

	double a, c;
	int b;


	a = (double) (1);
	b = 2;
	c = (a -  (double) b);
	RETURN_DOUBLE(c);

}

PHP_METHOD(Test_Arithmetic, doubleIntSubSimple) {

	double c;


	c = -1;
	RETURN_DOUBLE(c);

}

PHP_METHOD(Test_Arithmetic, varIntSub) {

	int b, c, a;


	a = 1;
	b = 2;
	c = (a - b);
	RETURN_LONG(c);

}

PHP_METHOD(Test_Arithmetic, intVarSub) {

	int b, c, a;


	a = 1;
	b = 2;
	c = (b - a);
	RETURN_LONG(c);

}

PHP_METHOD(Test_Arithmetic, intVarImplicitCastSub) {

	int a, b, c;


	a = 1;
	b = 2;
	c = (b - a);
	RETURN_LONG(c);

}

PHP_METHOD(Test_Arithmetic, intVarImplicitCast2Sub) {

	int a, b, c;


	a = 1;
	b = 2;
	c = (b - a);
	RETURN_LONG(c);

}

PHP_METHOD(Test_Arithmetic, complexSub) {

	double c;


	c = 1;
	RETURN_DOUBLE(c);

}

PHP_METHOD(Test_Arithmetic, complex2Sub) {

	zend_bool c;


	c = (1 & 1);
	RETURN_BOOL(c);

}

PHP_METHOD(Test_Arithmetic, complex3Sub) {

	zend_bool c;


	c = (1 &((1.0) ? 1 : 0));
	RETURN_BOOL(c);

}

PHP_METHOD(Test_Arithmetic, complex4Sub) {

	zval c;


	ZEPHIR_SINIT_VAR(c);
	ZVAL_DOUBLE(&c, (1.0 - 1));
	RETURN_LCTORW(c);

}

PHP_METHOD(Test_Arithmetic, complex5Sub) {

	int c;


	c = (1 - 1);
	RETURN_LONG(c);

}

PHP_METHOD(Test_Arithmetic, complex6Sub) {

	zend_bool c;


	c = (1 &((1) ? 1 : 0));
	RETURN_BOOL(c);

}

PHP_METHOD(Test_Arithmetic, complex7Sub) {

	int b, c;
	zend_bool a;


	a = (1) ? 1 : 0;
	b = 2;
	c = (a - b);
	RETURN_LONG(c);

}

PHP_METHOD(Test_Arithmetic, complex9Sub) {

	int b, c;
	zend_bool a;


	a = (1) ? 1 : 0;
	b = 2;
	c = (b - a);
	RETURN_LONG(c);

}

PHP_METHOD(Test_Arithmetic, complex10Sub) {

	double c;
	int b;
	zend_bool a;


	a = (1.0) ? 1 : 0;
	b = 2;
	c = (double) ((b - a));
	RETURN_DOUBLE(c);

}

PHP_METHOD(Test_Arithmetic, complex11Sub) {

	double c;
	int b;
	zend_bool a;


	a = (1) ? 1 : 0;
	b = (long) (2.0);
	c = (double) ((b - a));
	RETURN_DOUBLE(c);

}

PHP_METHOD(Test_Arithmetic, complex12Sub) {

	int b, c;
	zend_bool a;


	a = (1) ? 1 : 0;
	b = (long) (2.0);
	c = (b - a);
	RETURN_LONG(c);

}

PHP_METHOD(Test_Arithmetic, complex13Sub) {

	int c;
	zend_bool a, b;


	a = (1) ? 1 : 0;
	b = (2.0) ? 1 : 0;
	c = (b & a);
	RETURN_LONG(c);

}

PHP_METHOD(Test_Arithmetic, complex14Sub) {

	zend_bool b;
	int a, c;


	a = 1;
	b = (2.0) ? 1 : 0;
	c = (b - a);
	RETURN_LONG(c);

}

PHP_METHOD(Test_Arithmetic, complex15Sub) {

	zend_bool c;
	double b;
	int a;


	a = 1;
	b = 2.0;
	c = ((b -  (double) a)) ? 1 : 0;
	RETURN_BOOL(c);

}

PHP_METHOD(Test_Arithmetic, complex16Sub) {

	double b, c;
	int a;


	a = 1;
	b = 2.0;
	c = (b -  (double) a);
	RETURN_DOUBLE(c);

}

PHP_METHOD(Test_Arithmetic, complex17Sub) {

	double c;
	zend_bool b;
	int a;


	a = 1;
	b = (2.0) ? 1 : 0;
	c = (double) ((a - b));
	RETURN_DOUBLE(c);

}

PHP_METHOD(Test_Arithmetic, complex18Sub) {

	zend_bool b;
	double d, c;
	int a;


	a = 1;
	b = (2.0) ? 1 : 0;
	d = (double) ((a - b));
	c = (double) ((a - b) - d);
	RETURN_DOUBLE(c);

}

PHP_METHOD(Test_Arithmetic, complex19Sub) {

	zend_bool b;
	double d, c;
	int a;


	a = 1;
	b = (2.0) ? 1 : 0;
	d = (double) ((a - b));
	c = (((double) a - d) - b);
	RETURN_DOUBLE(c);

}

PHP_METHOD(Test_Arithmetic, complex20Sub) {

	zend_bool b;
	double d, c;
	int a;


	a = 1;
	b = (2.0) ? 1 : 0;
	d = (double) ((a - b));
	c = (double) ((double) ((d - d) - d) - d);
	RETURN_DOUBLE(c);

}

PHP_METHOD(Test_Arithmetic, complex21Sub) {

	zend_bool b;
	double d, c;
	int a;


	a = 1;
	b = (2.0) ? 1 : 0;
	d = (double) ((a - b));
	c = ((double) (((double) ((double) (((b - d) - b) - d) - d) - b) - d) - b);
	RETURN_DOUBLE(c);

}

PHP_METHOD(Test_Arithmetic, complex22Sub) {

	double d, c;
	int a, b;


	a = 1;
	b = (long) (2.0);
	d = (double) ((a - b));
	c = ((double) (((double) ((double) ((double) (((double) b - d) - d) - d) - d) - b) - d) - b);
	RETURN_DOUBLE(c);

}

PHP_METHOD(Test_Arithmetic, complex23Sub) {

	double d;
	zend_bool b;
	int a;


	a = 1;
	b = (2.0) ? 1 : 0;
	d = (double) ((a - b));
	RETURN_DOUBLE(d);

}

PHP_METHOD(Test_Arithmetic, complex24Sub) {

	zend_bool b;
	double d;
	int a;


	a = 1;
	b = (2.0) ? 1 : 0;
	d = (double) ((((1 - a) - 0) - b));
	RETURN_DOUBLE(d);

}

PHP_METHOD(Test_Arithmetic, sub1) {

	int a;


	a = 0;
	a -= 10;
	RETURN_LONG(a);

}

PHP_METHOD(Test_Arithmetic, sub2) {

	int a;


	a = 0;
	a -= 0;
	RETURN_LONG(a);

}

PHP_METHOD(Test_Arithmetic, sub2b) {

	int a;


	a = 5;
	a -= 0;
	RETURN_LONG(a);

}

PHP_METHOD(Test_Arithmetic, sub3) {

	int a;


	a = 0;
	a -= 1;
	RETURN_LONG(a);

}

PHP_METHOD(Test_Arithmetic, sub4) {

	int a;


	a = 0;
	a -= 0;
	RETURN_LONG(a);

}

PHP_METHOD(Test_Arithmetic, sub5) {

	int a;


	a = 0;
	a -= (long) (1.0);
	RETURN_LONG(a);

}

PHP_METHOD(Test_Arithmetic, sub6) {

	int a;


	a = 0;
	a -= (long) (1.0);
	RETURN_LONG(a);

}

PHP_METHOD(Test_Arithmetic, sub7) {

	double a;


	a = 0.0;
	a -= (double) (10);
	RETURN_DOUBLE(a);

}

PHP_METHOD(Test_Arithmetic, sub8) {

	double a;


	a = 0.0;
	a -= 0.0;
	RETURN_DOUBLE(a);

}

PHP_METHOD(Test_Arithmetic, sub8b) {

	double a;


	a = 6.3;
	a -= 0.0;
	RETURN_DOUBLE(a);

}

PHP_METHOD(Test_Arithmetic, sub9) {

	double a;


	a = 0.0;
	a -= 1;
	RETURN_DOUBLE(a);

}

PHP_METHOD(Test_Arithmetic, sub10) {

	double a;


	a = 0.0;
	a -= 0;
	RETURN_DOUBLE(a);

}

PHP_METHOD(Test_Arithmetic, sub11) {

	double a;


	a = 0.0;
	a -= 1.0;
	RETURN_DOUBLE(a);

}

PHP_METHOD(Test_Arithmetic, sub12) {

	int a, b;


	a = 0;
	b = 10;
	a -= b;
	RETURN_LONG(a);

}

PHP_METHOD(Test_Arithmetic, sub13) {

	zend_bool b;
	int a;


	a = 0;
	b = 1;
	a -= b;
	RETURN_LONG(a);

}

PHP_METHOD(Test_Arithmetic, sub14) {

	zend_bool b;
	int a;


	a = 0;
	b = 0;
	a -= b;
	RETURN_LONG(a);

}

PHP_METHOD(Test_Arithmetic, sub15) {

	double b;
	int a;


	a = 0;
	b = 1.0;
	a -= (long) b;
	RETURN_LONG(a);

}

PHP_METHOD(Test_Arithmetic, sub16) {

	double a, b;


	a = 0.0;
	b = 10.0;
	a -= b;
	RETURN_DOUBLE(a);

}

PHP_METHOD(Test_Arithmetic, sub17) {

	zend_bool b;
	double a;


	a = 0.0;
	b = 1;
	a -= (double) b;
	RETURN_DOUBLE(a);

}

PHP_METHOD(Test_Arithmetic, sub18) {

	zend_bool b;
	double a;


	a = 0.0;
	b = 0;
	a -= (double) b;
	RETURN_DOUBLE(a);

}

PHP_METHOD(Test_Arithmetic, sub19) {

	int b;
	double a;


	a = 0.0;
	b = 1;
	a -= (double) b;
	RETURN_DOUBLE(a);

}

PHP_METHOD(Test_Arithmetic, sub20) {

	int a, b;


	a = (long) (0.0);
	b = 1;
	a -= b;
	RETURN_LONG(a);

}

PHP_METHOD(Test_Arithmetic, sub21) {

	int b;
	double a;


	a = 0.0;
	b = 1;
	a -= (double) b;
	RETURN_DOUBLE(a);

}

PHP_METHOD(Test_Arithmetic, sub22) {

	int b, _0;
	zval *a;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	ZVAL_DOUBLE(a, 0.0);
	b = 1;
	ZEPHIR_INIT_BNVAR(a);
	_0 = zephir_get_numberval(a);
	ZVAL_LONG(a, _0 - b);
	RETURN_CCTOR(a);

}

PHP_METHOD(Test_Arithmetic, letStatementIntMinus) {

	int a;


	a = -1;
	RETURN_LONG(a);

}

PHP_METHOD(Test_Arithmetic, letStatementVarMinus) {

	zval *b, *a = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &b);



	zephir_negate(b);
	ZEPHIR_CPY_WRT(a, b);
	RETURN_CCTOR(a);

}

