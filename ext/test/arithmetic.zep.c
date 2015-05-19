
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
#include "kernel/fcall.h"
#include "kernel/operators.h"
#include "kernel/object.h"


/**
 * Arithmetic operations
 */
ZEPHIR_INIT_CLASS(Test_Arithmetic) {

	ZEPHIR_REGISTER_CLASS(Test, Arithmetic, test, arithmetic, test_arithmetic_method_entry, 0);

	zend_declare_property_long(test_arithmetic_ce, SL("tmp1"), 100, ZEND_ACC_PROTECTED TSRMLS_CC);

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

PHP_METHOD(Test_Arithmetic, boolSumExpression) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval _0, *_1 = NULL;
	zend_bool a;

	ZEPHIR_MM_GROW();

	a = 1;
	ZEPHIR_SINIT_VAR(_0);
	ZVAL_LONG(&_0, 0);
	ZEPHIR_CALL_FUNCTION(&_1, "exp", NULL, 1, &_0);
	zephir_check_call_status();
	RETURN_MM_LONG((a + zephir_get_numberval(_1)));

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

PHP_METHOD(Test_Arithmetic, doubleSumExpression) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval _0, *_1 = NULL;

	ZEPHIR_MM_GROW();

	ZEPHIR_SINIT_VAR(_0);
	ZVAL_LONG(&_0, 0);
	ZEPHIR_CALL_FUNCTION(&_1, "exp", NULL, 1, &_0);
	zephir_check_call_status();
	RETURN_MM_DOUBLE((1.0 + zephir_get_numberval(_1)));

}

PHP_METHOD(Test_Arithmetic, doubleSumVarExpression) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval _0, *_1 = NULL;
	double a;

	ZEPHIR_MM_GROW();

	a = 1.0;
	ZEPHIR_SINIT_VAR(_0);
	ZVAL_LONG(&_0, 0);
	ZEPHIR_CALL_FUNCTION(&_1, "exp", NULL, 1, &_0);
	zephir_check_call_status();
	RETURN_MM_LONG((a + zephir_get_numberval(_1)));

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

	int c;
	long a, b;


	a = 1;
	b = 2;
	c = (b + a);
	RETURN_LONG(c);

}

PHP_METHOD(Test_Arithmetic, intVarImplicitCast2Sum) {

	int b, c;
	long a;


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


	c = ((1 + 1.0)) ? 1 : 0;
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


	c = ((1 + 1)) ? 1 : 0;
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
	zval *a = NULL;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	ZVAL_DOUBLE(a, 0.0);
	b = 1;
	ZEPHIR_INIT_NVAR(a);
	_0 = zephir_get_numberval(a);
	ZVAL_LONG(a, _0 + b);
	RETURN_CCTOR(a);

}

PHP_METHOD(Test_Arithmetic, addSum23) {

	int a;


	a = 1;
	a += 1024;
	RETURN_LONG(a);

}

PHP_METHOD(Test_Arithmetic, addSum24) {

	zval *a, *_0;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &a);

	ZEPHIR_SEPARATE_PARAM(a);


	ZEPHIR_INIT_VAR(_0);
	ZVAL_LONG(_0, 1024);
	ZEPHIR_ADD_ASSIGN(a, _0);
	RETVAL_ZVAL(a, 1, 0);
	RETURN_MM();

}

PHP_METHOD(Test_Arithmetic, intSub) {

	int a, b, c;


	a = 1;
	b = 2;
	c = (a - b);
	RETURN_LONG(c);

}

PHP_METHOD(Test_Arithmetic, intLetSub) {

	int b, c;


	c = 1;
	b = 2;
	c -= b;
	RETURN_LONG(c);

}

PHP_METHOD(Test_Arithmetic, intSub2) {

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

	int c;
	long a, b;


	a = 1;
	b = 2;
	c = (b - a);
	RETURN_LONG(c);

}

PHP_METHOD(Test_Arithmetic, intVarImplicitCast2Sub) {

	int b, c;
	long a;


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


	c = ((1 + 1.0)) ? 1 : 0;
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


	c = ((1 + 1)) ? 1 : 0;
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
	zval *a = NULL;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	ZVAL_DOUBLE(a, 0.0);
	b = 1;
	ZEPHIR_INIT_NVAR(a);
	_0 = zephir_get_numberval(a);
	ZVAL_LONG(a, _0 - b);
	RETURN_CCTOR(a);

}

PHP_METHOD(Test_Arithmetic, sub23) {

	int a;


	a = 1;
	a -= 1024;
	RETURN_LONG(a);

}

PHP_METHOD(Test_Arithmetic, sub24) {

	zval *a, *_0;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &a);

	ZEPHIR_SEPARATE_PARAM(a);


	ZEPHIR_INIT_VAR(_0);
	ZVAL_LONG(_0, 1024);
	ZEPHIR_SUB_ASSIGN(a, _0);
	RETVAL_ZVAL(a, 1, 0);
	RETURN_MM();

}

PHP_METHOD(Test_Arithmetic, mul1) {

	int a = 1;


	a *= 5;
	RETURN_LONG(a);

}

PHP_METHOD(Test_Arithmetic, mul2) {

	zval *a, *_0;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &a);

	ZEPHIR_SEPARATE_PARAM(a);


	ZEPHIR_INIT_VAR(_0);
	ZVAL_LONG(_0, 5);
	ZEPHIR_MUL_ASSIGN(a, _0);
	RETVAL_ZVAL(a, 1, 0);
	RETURN_MM();

}

PHP_METHOD(Test_Arithmetic, mul3) {

	int a = 1;


	a *= 1024;
	RETURN_LONG(a);

}

PHP_METHOD(Test_Arithmetic, less1) {

	int a = 1, b = 2;


	RETURN_BOOL(a < b);

}

PHP_METHOD(Test_Arithmetic, less2) {

	int a = 2, b = 1;


	RETURN_BOOL(a < b);

}

PHP_METHOD(Test_Arithmetic, less3) {

	zval *a;

	zephir_fetch_params(0, 1, 0, &a);



	RETURN_BOOL(ZEPHIR_LT_LONG(a, 5));

}

PHP_METHOD(Test_Arithmetic, less4) {

	zval *a;

	zephir_fetch_params(0, 1, 0, &a);



	RETURN_BOOL(ZEPHIR_LT_LONG(a, 5.0));

}

PHP_METHOD(Test_Arithmetic, greater1) {

	int a = 1, b = 2;


	RETURN_BOOL(a > b);

}

PHP_METHOD(Test_Arithmetic, greater2) {

	int a = 2, b = 1;


	RETURN_BOOL(a > b);

}

PHP_METHOD(Test_Arithmetic, greater3) {

	zval *a;

	zephir_fetch_params(0, 1, 0, &a);



	RETURN_BOOL(ZEPHIR_GT_LONG(a, 5));

}

PHP_METHOD(Test_Arithmetic, greater4) {

	zval *a;

	zephir_fetch_params(0, 1, 0, &a);



	RETURN_BOOL(ZEPHIR_GT_LONG(a, 5.0));

}

PHP_METHOD(Test_Arithmetic, letStatementIntMinus) {

	int a;


	a = -1;
	RETURN_LONG(a);

}

PHP_METHOD(Test_Arithmetic, declaredIntMinus) {

	int a = -1;


	RETURN_LONG(a);

}

/**
 * Boolean var can`t be -1 when we set -1
 */
PHP_METHOD(Test_Arithmetic, letStatementBoolMinus) {

	zend_bool a;
	zval *b;

	zephir_fetch_params(0, 1, 0, &b);



	zephir_negate(b TSRMLS_CC);
	a = zephir_is_true(b);
	RETURN_BOOL(a);

}

PHP_METHOD(Test_Arithmetic, letStatementVarMinus) {

	zval *b, *a = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &b);



	zephir_negate(b TSRMLS_CC);
	ZEPHIR_CPY_WRT(a, b);
	RETURN_CCTOR(a);

}

PHP_METHOD(Test_Arithmetic, div1) {

	int a = 100;


	RETURN_DOUBLE(zephir_safe_div_long_long(((a - 1)), 4 TSRMLS_CC));

}

PHP_METHOD(Test_Arithmetic, div2) {

	zval *_0;


	_0 = zephir_fetch_nproperty_this(this_ptr, SL("tmp1"), PH_NOISY_CC);
	RETURN_DOUBLE(zephir_safe_div_long_long(((zephir_get_numberval(_0) - 1)), 4 TSRMLS_CC));

}

