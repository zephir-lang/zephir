
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
#include "kernel/memory.h"
#include "kernel/fcall.h"
#include "kernel/operators.h"
#include "ext/spl/spl_exceptions.h"
#include "kernel/exception.h"


/**
 * Arithmetic operations
 */
ZEPHIR_INIT_CLASS(Stub_Arithmetic)
{
	ZEPHIR_REGISTER_CLASS(Stub, Arithmetic, stub, arithmetic, stub_arithmetic_method_entry, 0);

	zend_declare_property_long(stub_arithmetic_ce, SL("tmp1"), 100, ZEND_ACC_PROTECTED);
	return SUCCESS;
}

PHP_METHOD(Stub_Arithmetic, intSum)
{
	zend_long a = 0, b = 0, c = 0;
	a = 1;
	b = 2;
	c = (a + b);
	RETURN_LONG(c);
}

PHP_METHOD(Stub_Arithmetic, int2Sum)
{
	zend_long a = 0, c = 0;
	a = 1;
	c = (a + 2);
	RETURN_LONG(c);
}

PHP_METHOD(Stub_Arithmetic, intSumSimple)
{
	zend_long c = 0;
	c = 3;
	RETURN_LONG(c);
}

PHP_METHOD(Stub_Arithmetic, boolSum)
{
	zend_bool a = 0, b = 0, c = 0;
	a = ((1) ? 1 : 0);
	b = ((2) ? 1 : 0);
	c = (a | b);
	RETURN_BOOL(c);
}

PHP_METHOD(Stub_Arithmetic, bool2Sum)
{
	zend_bool a = 0, c = 0;
	a = ((1) ? 1 : 0);
	c = (a + 2);
	RETURN_BOOL(c);
}

PHP_METHOD(Stub_Arithmetic, bool3Sum)
{
	zend_bool a = 0, c = 0;
	a = 1;
	c = (a | 0);
	RETURN_BOOL(c);
}

PHP_METHOD(Stub_Arithmetic, boolSumSimple)
{
	zend_bool c = 0;
	c = ((3) ? 1 : 0);
	RETURN_BOOL(c);
}

PHP_METHOD(Stub_Arithmetic, boolSumExpression)
{
	zval _0, _1;
	zend_bool a = 0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	a = 1;
	ZVAL_LONG(&_0, 0);
	ZEPHIR_CALL_FUNCTION(&_1, "exp", NULL, 2, &_0);
	zephir_check_call_status();
	RETURN_MM_LONG((a + zephir_get_numberval(&_1)));
}

PHP_METHOD(Stub_Arithmetic, doubleSum)
{
	double a = 0, b = 0, c = 0;
	a = (double) (1);
	b = (double) (2);
	c =  ((a + b));
	RETURN_DOUBLE(c);
}

PHP_METHOD(Stub_Arithmetic, double2Sum)
{
	double a = 0, b = 0, c = 0;
	a =  (1.0);
	b =  (2.0);
	c =  ((a + b));
	RETURN_DOUBLE(c);
}

PHP_METHOD(Stub_Arithmetic, doubleSumSimple)
{
	double c = 0;
	c = (double) (3);
	RETURN_DOUBLE(c);
}

PHP_METHOD(Stub_Arithmetic, doubleSum2Simple)
{
	double c = 0;
	c =  (3);
	RETURN_DOUBLE(c);
}

PHP_METHOD(Stub_Arithmetic, doubleSumExpression)
{
	zval _0, _1;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZVAL_LONG(&_0, 0);
	ZEPHIR_CALL_FUNCTION(&_1, "exp", NULL, 2, &_0);
	zephir_check_call_status();
	RETURN_MM_DOUBLE((1.0 + zephir_get_numberval(&_1)));
}

PHP_METHOD(Stub_Arithmetic, doubleSumVarExpression)
{
	zval _0, _1;
	double a = 0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	a =  (1.0);
	ZVAL_LONG(&_0, 0);
	ZEPHIR_CALL_FUNCTION(&_1, "exp", NULL, 2, &_0);
	zephir_check_call_status();
	RETURN_MM_LONG((a + zephir_get_numberval(&_1)));
}

PHP_METHOD(Stub_Arithmetic, varSum)
{
	zend_long a = 0, b = 0, c = 0;
	a = 1;
	b = 2;
	c = (a + b);
	RETURN_LONG(c);
}

PHP_METHOD(Stub_Arithmetic, varSumSimple)
{
	zend_long c = 0;
	c = 3;
	RETURN_LONG(c);
}

PHP_METHOD(Stub_Arithmetic, intDoubleSum)
{
	double a = 0;
	zend_long b = 0, c = 0;
	a = (double) (1);
	b = 2;
	c = (long) ((a +  (double) b));
	RETURN_LONG(c);
}

PHP_METHOD(Stub_Arithmetic, intDoubleSumSimple)
{
	zend_long c = 0;
	c = (long) (3);
	RETURN_LONG(c);
}

PHP_METHOD(Stub_Arithmetic, doubleIntSum)
{
	double a = 0, c = 0;
	zend_long b = 0;
	a = (double) (1);
	b = 2;
	c =  ((a +  (double) b));
	RETURN_DOUBLE(c);
}

PHP_METHOD(Stub_Arithmetic, doubleIntSumSimple)
{
	double c = 0;
	c =  (3);
	RETURN_DOUBLE(c);
}

PHP_METHOD(Stub_Arithmetic, varIntSum)
{
	zend_long b = 0, c = 0, a = 0;
	a = 1;
	b = 2;
	c = (a + b);
	RETURN_LONG(c);
}

PHP_METHOD(Stub_Arithmetic, intVarSum)
{
	zend_long b = 0, c = 0, a = 0;
	a = 1;
	b = 2;
	c = (b + a);
	RETURN_LONG(c);
}

PHP_METHOD(Stub_Arithmetic, intVarImplicitCastSum)
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
	zephir_add_function(&_0, &b, &a);
	c = zephir_get_numberval(&_0);
	RETURN_MM_LONG(c);
}

PHP_METHOD(Stub_Arithmetic, intVarImplicitCast2Sum)
{
	zend_long b = 0, c = 0;
	zval a;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&a);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&a);
	ZVAL_STRING(&a, "1");
	b = 2;
	c = (b + zephir_get_numberval(&a));
	RETURN_MM_LONG(c);
}

PHP_METHOD(Stub_Arithmetic, complexSum)
{
	double c = 0;
	c =  (3);
	RETURN_DOUBLE(c);
}

PHP_METHOD(Stub_Arithmetic, complex2Sum)
{
	zend_bool c = 0;
	c = (1 | 1);
	RETURN_BOOL(c);
}

PHP_METHOD(Stub_Arithmetic, complex3Sum)
{
	zend_bool c = 0;
	c = (((1 + 1.0)) ? 1 : 0);
	RETURN_BOOL(c);
}

PHP_METHOD(Stub_Arithmetic, complex4Sum)
{
	zval c;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&c);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&c);
	ZVAL_DOUBLE(&c, (1.0 + 1));
	RETURN_CCTOR(&c);
}

PHP_METHOD(Stub_Arithmetic, complex5Sum)
{
	zend_long c = 0;
	c = (1 + 1);
	RETURN_LONG(c);
}

PHP_METHOD(Stub_Arithmetic, complex6Sum)
{
	zend_bool c = 0;
	c = (((1 + 1)) ? 1 : 0);
	RETURN_BOOL(c);
}

PHP_METHOD(Stub_Arithmetic, complex7Sum)
{
	zend_long b = 0, c = 0;
	zend_bool a = 0;
	a = ((1) ? 1 : 0);
	b = 2;
	c = (a + b);
	RETURN_LONG(c);
}

PHP_METHOD(Stub_Arithmetic, complex9Sum)
{
	zend_long b = 0, c = 0;
	zend_bool a = 0;
	a = ((1) ? 1 : 0);
	b = 2;
	c = (b + a);
	RETURN_LONG(c);
}

PHP_METHOD(Stub_Arithmetic, complex10Sum)
{
	double c = 0;
	zend_long b = 0;
	zend_bool a = 0;
	a = ((1.0 != 0.0) ? 1 : 0);
	b = 2;
	c = (double) ((b + a));
	RETURN_DOUBLE(c);
}

PHP_METHOD(Stub_Arithmetic, complex11Sum)
{
	double c = 0;
	zend_long b = 0;
	zend_bool a = 0;
	a = ((1) ? 1 : 0);
	b = (long) (2.0);
	c = (double) ((b + a));
	RETURN_DOUBLE(c);
}

PHP_METHOD(Stub_Arithmetic, complex12Sum)
{
	zend_long b = 0, c = 0;
	zend_bool a = 0;
	a = ((1) ? 1 : 0);
	b = (long) (2.0);
	c = (b + a);
	RETURN_LONG(c);
}

PHP_METHOD(Stub_Arithmetic, complex13Sum)
{
	zend_long c = 0;
	zend_bool a = 0, b = 0;
	a = ((1) ? 1 : 0);
	b = ((2.0 != 0.0) ? 1 : 0);
	c = (b | a);
	RETURN_LONG(c);
}

PHP_METHOD(Stub_Arithmetic, complex14Sum)
{
	zend_bool b = 0;
	zend_long a = 0, c = 0;
	a = 1;
	b = ((2.0 != 0.0) ? 1 : 0);
	c = (b + a);
	RETURN_LONG(c);
}

PHP_METHOD(Stub_Arithmetic, complex15Sum)
{
	zend_bool c = 0;
	double b = 0;
	zend_long a = 0;
	a = 1;
	b =  (2.0);
	c = (((b +  (double) a) != 0.0) ? 1 : 0);
	RETURN_BOOL(c);
}

PHP_METHOD(Stub_Arithmetic, complex16Sum)
{
	double b = 0, c = 0;
	zend_long a = 0;
	a = 1;
	b =  (2.0);
	c =  ((b +  (double) a));
	RETURN_DOUBLE(c);
}

PHP_METHOD(Stub_Arithmetic, complex17Sum)
{
	double c = 0;
	zend_bool b = 0;
	zend_long a = 0;
	a = 1;
	b = ((2.0 != 0.0) ? 1 : 0);
	c = (double) ((a + b));
	RETURN_DOUBLE(c);
}

PHP_METHOD(Stub_Arithmetic, complex18Sum)
{
	zend_bool b = 0;
	double d = 0, c = 0;
	zend_long a = 0;
	a = 1;
	b = ((2.0 != 0.0) ? 1 : 0);
	d = (double) ((a + b));
	c =  ((double) ((a + b) + d));
	RETURN_DOUBLE(c);
}

PHP_METHOD(Stub_Arithmetic, complex19Sum)
{
	zend_bool b = 0;
	double d = 0, c = 0;
	zend_long a = 0;
	a = 1;
	b = ((2.0 != 0.0) ? 1 : 0);
	d = (double) ((a + b));
	c =  ((((double) a + d) + b));
	RETURN_DOUBLE(c);
}

PHP_METHOD(Stub_Arithmetic, complex20Sum)
{
	zend_bool b = 0;
	double d = 0, c = 0;
	zend_long a = 0;
	a = 1;
	b = ((2.0 != 0.0) ? 1 : 0);
	d = (double) ((a + b));
	c =  ((double) ((double) ((d + d) + d) + d));
	RETURN_DOUBLE(c);
}

PHP_METHOD(Stub_Arithmetic, complex21Sum)
{
	zend_bool b = 0;
	double d = 0, c = 0;
	zend_long a = 0;
	a = 1;
	b = ((2.0 != 0.0) ? 1 : 0);
	d = (double) ((a + b));
	c =  (((double) (((double) ((double) (((b + d) + b) + d) + d) + b) + d) + b));
	RETURN_DOUBLE(c);
}

PHP_METHOD(Stub_Arithmetic, complex22Sum)
{
	double d = 0, c = 0;
	zend_long a = 0, b = 0;
	a = 1;
	b = (long) (2.0);
	d = (double) ((a + b));
	c =  (((double) (((double) ((double) ((double) (((double) b + d) + d) + d) + d) + b) + d) + b));
	RETURN_DOUBLE(c);
}

PHP_METHOD(Stub_Arithmetic, complex23Sum)
{
	double d = 0;
	zend_bool b = 0;
	zend_long a = 0;
	a = 1;
	b = ((2.0 != 0.0) ? 1 : 0);
	d = (double) ((a + b));
	RETURN_DOUBLE(d);
}

PHP_METHOD(Stub_Arithmetic, complex24Sum)
{
	zend_bool b = 0;
	double d = 0;
	zend_long a = 0;
	a = 1;
	b = ((2.0 != 0.0) ? 1 : 0);
	d = (double) ((((1 + a) + 0) + b));
	RETURN_DOUBLE(d);
}

PHP_METHOD(Stub_Arithmetic, addSum1)
{
	zend_long a = 0;
	a = 0;
	a += 10;
	RETURN_LONG(a);
}

PHP_METHOD(Stub_Arithmetic, addSum2)
{
	zend_long a = 0;
	a = 0;
	a += 0;
	RETURN_LONG(a);
}

PHP_METHOD(Stub_Arithmetic, addSum2b)
{
	zend_long a = 0;
	a = 5;
	a += 0;
	RETURN_LONG(a);
}

PHP_METHOD(Stub_Arithmetic, addSum3)
{
	zend_long a = 0;
	a = 0;
	a += 1;
	RETURN_LONG(a);
}

PHP_METHOD(Stub_Arithmetic, addSum4)
{
	zend_long a = 0;
	a = 0;
	a += 0;
	RETURN_LONG(a);
}

PHP_METHOD(Stub_Arithmetic, addSum5)
{
	zend_long a = 0;
	a = 0;
	a += (long) (1.0);
	RETURN_LONG(a);
}

PHP_METHOD(Stub_Arithmetic, addSum6)
{
	zend_long a = 0;
	a = 0;
	a += (long) (1.0);
	RETURN_LONG(a);
}

PHP_METHOD(Stub_Arithmetic, addSum7)
{
	double a = 0;
	a =  (0.0);
	a += (double) (10);
	RETURN_DOUBLE(a);
}

PHP_METHOD(Stub_Arithmetic, addSum8)
{
	double a = 0;
	a =  (0.0);
	a += 0.0;
	RETURN_DOUBLE(a);
}

PHP_METHOD(Stub_Arithmetic, addSum8b)
{
	double a = 0;
	a =  (6.3);
	a += 0.0;
	RETURN_DOUBLE(a);
}

PHP_METHOD(Stub_Arithmetic, addSum9)
{
	double a = 0;
	a =  (0.0);
	a += 1;
	RETURN_DOUBLE(a);
}

PHP_METHOD(Stub_Arithmetic, addSum10)
{
	double a = 0;
	a =  (0.0);
	a += 0;
	RETURN_DOUBLE(a);
}

PHP_METHOD(Stub_Arithmetic, addSum11)
{
	double a = 0;
	a =  (0.0);
	a +=  (1.0);
	RETURN_DOUBLE(a);
}

PHP_METHOD(Stub_Arithmetic, addSum12)
{
	zend_long a = 0, b = 0;
	a = 0;
	b = 10;
	a += b;
	RETURN_LONG(a);
}

PHP_METHOD(Stub_Arithmetic, addSum13)
{
	zend_bool b = 0;
	zend_long a = 0;
	a = 0;
	b = 1;
	a += b;
	RETURN_LONG(a);
}

PHP_METHOD(Stub_Arithmetic, addSum14)
{
	zend_bool b = 0;
	zend_long a = 0;
	a = 0;
	b = 0;
	a += b;
	RETURN_LONG(a);
}

PHP_METHOD(Stub_Arithmetic, addSum15)
{
	double b = 0;
	zend_long a = 0;
	a = 0;
	b =  (1.0);
	a += (long) b;
	RETURN_LONG(a);
}

PHP_METHOD(Stub_Arithmetic, addSum16)
{
	double a = 0, b = 0;
	a =  (0.0);
	b =  (10.0);
	a +=  b;
	RETURN_DOUBLE(a);
}

PHP_METHOD(Stub_Arithmetic, addSum17)
{
	zend_bool b = 0;
	double a = 0;
	a =  (0.0);
	b = 1;
	a += (double) b;
	RETURN_DOUBLE(a);
}

PHP_METHOD(Stub_Arithmetic, addSum18)
{
	zend_bool b = 0;
	double a = 0;
	a =  (0.0);
	b = 0;
	a += (double) b;
	RETURN_DOUBLE(a);
}

PHP_METHOD(Stub_Arithmetic, addSum19)
{
	zend_long b = 0;
	double a = 0;
	a =  (0.0);
	b = 1;
	a += (double) b;
	RETURN_DOUBLE(a);
}

PHP_METHOD(Stub_Arithmetic, addSum20)
{
	zend_long a = 0, b = 0;
	a = (long) (0.0);
	b = 1;
	a += b;
	RETURN_LONG(a);
}

PHP_METHOD(Stub_Arithmetic, addSum21)
{
	zend_long b = 0;
	double a = 0;
	a =  (0.0);
	b = 1;
	a += (double) b;
	RETURN_DOUBLE(a);
}

PHP_METHOD(Stub_Arithmetic, addSum22)
{
	zend_long b = 0, _0;
	zval a;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&a);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&a);
	ZVAL_DOUBLE(&a, 0.0);
	b = 1;
	ZEPHIR_INIT_NVAR(&a);
	_0 = zephir_get_numberval(&a) + b;
	ZVAL_LONG(&a, _0);
	RETURN_CCTOR(&a);
}

PHP_METHOD(Stub_Arithmetic, addSum23)
{
	zend_long a = 0;
	a = 1;
	a += 1024;
	RETURN_LONG(a);
}

PHP_METHOD(Stub_Arithmetic, addSum24)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a, a_sub, _0;

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(a)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &a);
	ZEPHIR_SEPARATE_PARAM(a);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, 1024);
	ZEPHIR_ADD_ASSIGN(a, &_0);
	RETVAL_ZVAL(a, 1, 0);
	RETURN_MM();
}

PHP_METHOD(Stub_Arithmetic, intSub)
{
	zend_long a = 0, b = 0, c = 0;
	a = 1;
	b = 2;
	c = (a - b);
	RETURN_LONG(c);
}

PHP_METHOD(Stub_Arithmetic, intLetSub)
{
	zend_long b = 0, c = 0;
	c = 1;
	b = 2;
	c -= b;
	RETURN_LONG(c);
}

PHP_METHOD(Stub_Arithmetic, intSub2)
{
	zend_long a = 0, c = 0;
	a = 1;
	c = (a - 2);
	RETURN_LONG(c);
}

PHP_METHOD(Stub_Arithmetic, intSubSimple)
{
	zend_long c = 0;
	c = -1;
	RETURN_LONG(c);
}

PHP_METHOD(Stub_Arithmetic, boolSub)
{
	zend_bool a = 0, b = 0, c = 0;
	a = ((1) ? 1 : 0);
	b = ((2) ? 1 : 0);
	c = (a & b);
	RETURN_BOOL(c);
}

PHP_METHOD(Stub_Arithmetic, bool2Sub)
{
	zend_bool a = 0, c = 0;
	a = ((1) ? 1 : 0);
	c = (a - 2);
	RETURN_BOOL(c);
}

PHP_METHOD(Stub_Arithmetic, bool3Sub)
{
	zend_bool a = 0, c = 0;
	a = 1;
	c = (a & 0);
	RETURN_BOOL(c);
}

PHP_METHOD(Stub_Arithmetic, bool4Sub)
{
	zend_bool a = 0, c = 0;
	a = 1;
	c = (a & 1);
	RETURN_BOOL(c);
}

PHP_METHOD(Stub_Arithmetic, boolSubSimple)
{
	zend_bool c = 0;
	c = ((-1) ? 1 : 0);
	RETURN_BOOL(c);
}

PHP_METHOD(Stub_Arithmetic, doubleSub)
{
	double a = 0, b = 0, c = 0;
	a = (double) (1);
	b = (double) (2);
	c =  ((a - b));
	RETURN_DOUBLE(c);
}

PHP_METHOD(Stub_Arithmetic, double2Sub)
{
	double a = 0, b = 0, c = 0;
	a =  (1.0);
	b =  (2.0);
	c =  ((a - b));
	RETURN_DOUBLE(c);
}

PHP_METHOD(Stub_Arithmetic, doubleSubSimple)
{
	double c = 0;
	c = (double) (-1);
	RETURN_DOUBLE(c);
}

PHP_METHOD(Stub_Arithmetic, doubleSub2Simple)
{
	double c = 0;
	c =  (-1);
	RETURN_DOUBLE(c);
}

PHP_METHOD(Stub_Arithmetic, varSub)
{
	zend_long a = 0, b = 0, c = 0;
	a = 1;
	b = 2;
	c = (a - b);
	RETURN_LONG(c);
}

PHP_METHOD(Stub_Arithmetic, varSubSimple)
{
	zend_long c = 0;
	c = -1;
	RETURN_LONG(c);
}

PHP_METHOD(Stub_Arithmetic, intDoubleSub)
{
	double a = 0;
	zend_long b = 0, c = 0;
	a = (double) (1);
	b = 2;
	c = (long) ((a -  (double) b));
	RETURN_LONG(c);
}

PHP_METHOD(Stub_Arithmetic, intDoubleSubSimple)
{
	zend_long c = 0;
	c = (long) (-1);
	RETURN_LONG(c);
}

PHP_METHOD(Stub_Arithmetic, doubleIntSub)
{
	double a = 0, c = 0;
	zend_long b = 0;
	a = (double) (1);
	b = 2;
	c =  ((a -  (double) b));
	RETURN_DOUBLE(c);
}

PHP_METHOD(Stub_Arithmetic, doubleIntSubSimple)
{
	double c = 0;
	c =  (-1);
	RETURN_DOUBLE(c);
}

PHP_METHOD(Stub_Arithmetic, varIntSub)
{
	zend_long b = 0, c = 0, a = 0;
	a = 1;
	b = 2;
	c = (a - b);
	RETURN_LONG(c);
}

PHP_METHOD(Stub_Arithmetic, intVarSub)
{
	zend_long b = 0, c = 0, a = 0;
	a = 1;
	b = 2;
	c = (b - a);
	RETURN_LONG(c);
}

PHP_METHOD(Stub_Arithmetic, intVarImplicitCastSub)
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
	zephir_sub_function(&_0, &b, &a);
	c = zephir_get_numberval(&_0);
	RETURN_MM_LONG(c);
}

PHP_METHOD(Stub_Arithmetic, intVarImplicitCast2Sub)
{
	zend_long b = 0, c = 0;
	zval a;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&a);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&a);
	ZVAL_STRING(&a, "1");
	b = 2;
	c = (b - zephir_get_numberval(&a));
	RETURN_MM_LONG(c);
}

PHP_METHOD(Stub_Arithmetic, complexSub)
{
	double c = 0;
	c =  (1);
	RETURN_DOUBLE(c);
}

PHP_METHOD(Stub_Arithmetic, complex2Sub)
{
	zend_bool c = 0;
	c = (1 & 1);
	RETURN_BOOL(c);
}

PHP_METHOD(Stub_Arithmetic, complex3Sub)
{
	zend_bool c = 0;
	c = (((1 + 1.0)) ? 1 : 0);
	RETURN_BOOL(c);
}

PHP_METHOD(Stub_Arithmetic, complex4Sub)
{
	zval c;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&c);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&c);
	ZVAL_DOUBLE(&c, (1.0 - 1));
	RETURN_CCTOR(&c);
}

PHP_METHOD(Stub_Arithmetic, complex5Sub)
{
	zend_long c = 0;
	c = (1 - 1);
	RETURN_LONG(c);
}

PHP_METHOD(Stub_Arithmetic, complex6Sub)
{
	zend_bool c = 0;
	c = (((1 + 1)) ? 1 : 0);
	RETURN_BOOL(c);
}

PHP_METHOD(Stub_Arithmetic, complex7Sub)
{
	zend_long b = 0, c = 0;
	zend_bool a = 0;
	a = ((1) ? 1 : 0);
	b = 2;
	c = (a - b);
	RETURN_LONG(c);
}

PHP_METHOD(Stub_Arithmetic, complex9Sub)
{
	zend_long b = 0, c = 0;
	zend_bool a = 0;
	a = ((1) ? 1 : 0);
	b = 2;
	c = (b - a);
	RETURN_LONG(c);
}

PHP_METHOD(Stub_Arithmetic, complex10Sub)
{
	double c = 0;
	zend_long b = 0;
	zend_bool a = 0;
	a = ((1.0 != 0.0) ? 1 : 0);
	b = 2;
	c = (double) ((b - a));
	RETURN_DOUBLE(c);
}

PHP_METHOD(Stub_Arithmetic, complex11Sub)
{
	double c = 0;
	zend_long b = 0;
	zend_bool a = 0;
	a = ((1) ? 1 : 0);
	b = (long) (2.0);
	c = (double) ((b - a));
	RETURN_DOUBLE(c);
}

PHP_METHOD(Stub_Arithmetic, complex12Sub)
{
	zend_long b = 0, c = 0;
	zend_bool a = 0;
	a = ((1) ? 1 : 0);
	b = (long) (2.0);
	c = (b - a);
	RETURN_LONG(c);
}

PHP_METHOD(Stub_Arithmetic, complex13Sub)
{
	zend_long c = 0;
	zend_bool a = 0, b = 0;
	a = ((1) ? 1 : 0);
	b = ((2.0 != 0.0) ? 1 : 0);
	c = (b & a);
	RETURN_LONG(c);
}

PHP_METHOD(Stub_Arithmetic, complex14Sub)
{
	zend_bool b = 0;
	zend_long a = 0, c = 0;
	a = 1;
	b = ((2.0 != 0.0) ? 1 : 0);
	c = (b - a);
	RETURN_LONG(c);
}

PHP_METHOD(Stub_Arithmetic, complex15Sub)
{
	zend_bool c = 0;
	double b = 0;
	zend_long a = 0;
	a = 1;
	b =  (2.0);
	c = (((b -  (double) a) != 0.0) ? 1 : 0);
	RETURN_BOOL(c);
}

PHP_METHOD(Stub_Arithmetic, complex16Sub)
{
	double b = 0, c = 0;
	zend_long a = 0;
	a = 1;
	b =  (2.0);
	c =  ((b -  (double) a));
	RETURN_DOUBLE(c);
}

PHP_METHOD(Stub_Arithmetic, complex17Sub)
{
	double c = 0;
	zend_bool b = 0;
	zend_long a = 0;
	a = 1;
	b = ((2.0 != 0.0) ? 1 : 0);
	c = (double) ((a - b));
	RETURN_DOUBLE(c);
}

PHP_METHOD(Stub_Arithmetic, complex18Sub)
{
	zend_bool b = 0;
	double d = 0, c = 0;
	zend_long a = 0;
	a = 1;
	b = ((2.0 != 0.0) ? 1 : 0);
	d = (double) ((a - b));
	c =  ((double) ((a - b) - d));
	RETURN_DOUBLE(c);
}

PHP_METHOD(Stub_Arithmetic, complex19Sub)
{
	zend_bool b = 0;
	double d = 0, c = 0;
	zend_long a = 0;
	a = 1;
	b = ((2.0 != 0.0) ? 1 : 0);
	d = (double) ((a - b));
	c =  ((((double) a - d) - b));
	RETURN_DOUBLE(c);
}

PHP_METHOD(Stub_Arithmetic, complex20Sub)
{
	zend_bool b = 0;
	double d = 0, c = 0;
	zend_long a = 0;
	a = 1;
	b = ((2.0 != 0.0) ? 1 : 0);
	d = (double) ((a - b));
	c =  ((double) ((double) ((d - d) - d) - d));
	RETURN_DOUBLE(c);
}

PHP_METHOD(Stub_Arithmetic, complex21Sub)
{
	zend_bool b = 0;
	double d = 0, c = 0;
	zend_long a = 0;
	a = 1;
	b = ((2.0 != 0.0) ? 1 : 0);
	d = (double) ((a - b));
	c =  (((double) (((double) ((double) (((b - d) - b) - d) - d) - b) - d) - b));
	RETURN_DOUBLE(c);
}

PHP_METHOD(Stub_Arithmetic, complex22Sub)
{
	double d = 0, c = 0;
	zend_long a = 0, b = 0;
	a = 1;
	b = (long) (2.0);
	d = (double) ((a - b));
	c =  (((double) (((double) ((double) ((double) (((double) b - d) - d) - d) - d) - b) - d) - b));
	RETURN_DOUBLE(c);
}

PHP_METHOD(Stub_Arithmetic, complex23Sub)
{
	double d = 0;
	zend_bool b = 0;
	zend_long a = 0;
	a = 1;
	b = ((2.0 != 0.0) ? 1 : 0);
	d = (double) ((a - b));
	RETURN_DOUBLE(d);
}

PHP_METHOD(Stub_Arithmetic, complex24Sub)
{
	zend_bool b = 0;
	double d = 0;
	zend_long a = 0;
	a = 1;
	b = ((2.0 != 0.0) ? 1 : 0);
	d = (double) ((((1 - a) - 0) - b));
	RETURN_DOUBLE(d);
}

PHP_METHOD(Stub_Arithmetic, sub1)
{
	zend_long a = 0;
	a = 0;
	a -= 10;
	RETURN_LONG(a);
}

PHP_METHOD(Stub_Arithmetic, sub2)
{
	zend_long a = 0;
	a = 0;
	a -= 0;
	RETURN_LONG(a);
}

PHP_METHOD(Stub_Arithmetic, sub2b)
{
	zend_long a = 0;
	a = 5;
	a -= 0;
	RETURN_LONG(a);
}

PHP_METHOD(Stub_Arithmetic, sub3)
{
	zend_long a = 0;
	a = 0;
	a -= 1;
	RETURN_LONG(a);
}

PHP_METHOD(Stub_Arithmetic, sub4)
{
	zend_long a = 0;
	a = 0;
	a -= 0;
	RETURN_LONG(a);
}

PHP_METHOD(Stub_Arithmetic, sub5)
{
	zend_long a = 0;
	a = 0;
	a -= (long) (1.0);
	RETURN_LONG(a);
}

PHP_METHOD(Stub_Arithmetic, sub6)
{
	zend_long a = 0;
	a = 0;
	a -= (long) (1.0);
	RETURN_LONG(a);
}

PHP_METHOD(Stub_Arithmetic, sub7)
{
	double a = 0;
	a =  (0.0);
	a -= (double) (10);
	RETURN_DOUBLE(a);
}

PHP_METHOD(Stub_Arithmetic, sub8)
{
	double a = 0;
	a =  (0.0);
	a -= 0.0;
	RETURN_DOUBLE(a);
}

PHP_METHOD(Stub_Arithmetic, sub8b)
{
	double a = 0;
	a =  (6.3);
	a -= 0.0;
	RETURN_DOUBLE(a);
}

PHP_METHOD(Stub_Arithmetic, sub9)
{
	double a = 0;
	a =  (0.0);
	a -= 1;
	RETURN_DOUBLE(a);
}

PHP_METHOD(Stub_Arithmetic, sub10)
{
	double a = 0;
	a =  (0.0);
	a -= 0;
	RETURN_DOUBLE(a);
}

PHP_METHOD(Stub_Arithmetic, sub11)
{
	double a = 0;
	a =  (0.0);
	a -=  (1.0);
	RETURN_DOUBLE(a);
}

PHP_METHOD(Stub_Arithmetic, sub12)
{
	zend_long a = 0, b = 0;
	a = 0;
	b = 10;
	a -= b;
	RETURN_LONG(a);
}

PHP_METHOD(Stub_Arithmetic, sub13)
{
	zend_bool b = 0;
	zend_long a = 0;
	a = 0;
	b = 1;
	a -= b;
	RETURN_LONG(a);
}

PHP_METHOD(Stub_Arithmetic, sub14)
{
	zend_bool b = 0;
	zend_long a = 0;
	a = 0;
	b = 0;
	a -= b;
	RETURN_LONG(a);
}

PHP_METHOD(Stub_Arithmetic, sub15)
{
	double b = 0;
	zend_long a = 0;
	a = 0;
	b =  (1.0);
	a -= (long) b;
	RETURN_LONG(a);
}

PHP_METHOD(Stub_Arithmetic, sub16)
{
	double a = 0, b = 0;
	a =  (0.0);
	b =  (10.0);
	a -=  b;
	RETURN_DOUBLE(a);
}

PHP_METHOD(Stub_Arithmetic, sub17)
{
	zend_bool b = 0;
	double a = 0;
	a =  (0.0);
	b = 1;
	a -= (double) b;
	RETURN_DOUBLE(a);
}

PHP_METHOD(Stub_Arithmetic, sub18)
{
	zend_bool b = 0;
	double a = 0;
	a =  (0.0);
	b = 0;
	a -= (double) b;
	RETURN_DOUBLE(a);
}

PHP_METHOD(Stub_Arithmetic, sub19)
{
	zend_long b = 0;
	double a = 0;
	a =  (0.0);
	b = 1;
	a -= (double) b;
	RETURN_DOUBLE(a);
}

PHP_METHOD(Stub_Arithmetic, sub20)
{
	zend_long a = 0, b = 0;
	a = (long) (0.0);
	b = 1;
	a -= b;
	RETURN_LONG(a);
}

PHP_METHOD(Stub_Arithmetic, sub21)
{
	zend_long b = 0;
	double a = 0;
	a =  (0.0);
	b = 1;
	a -= (double) b;
	RETURN_DOUBLE(a);
}

PHP_METHOD(Stub_Arithmetic, sub22)
{
	zend_long b = 0, _0;
	zval a;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&a);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&a);
	ZVAL_DOUBLE(&a, 0.0);
	b = 1;
	ZEPHIR_INIT_NVAR(&a);
	_0 = zephir_get_numberval(&a);
	ZVAL_LONG(&a, _0 - b);
	RETURN_CCTOR(&a);
}

PHP_METHOD(Stub_Arithmetic, sub23)
{
	zend_long a = 0;
	a = 1;
	a -= 1024;
	RETURN_LONG(a);
}

PHP_METHOD(Stub_Arithmetic, sub24)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a, a_sub, _0;

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(a)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &a);
	ZEPHIR_SEPARATE_PARAM(a);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, 1024);
	ZEPHIR_SUB_ASSIGN(a, &_0);
	RETVAL_ZVAL(a, 1, 0);
	RETURN_MM();
}

PHP_METHOD(Stub_Arithmetic, mul1)
{
	zend_long a;
	a = 1;
	a *= 5;
	RETURN_LONG(a);
}

PHP_METHOD(Stub_Arithmetic, mul2)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a, a_sub, _0;

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(a)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &a);
	ZEPHIR_SEPARATE_PARAM(a);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, 5);
	ZEPHIR_MUL_ASSIGN(a, &_0);
	RETVAL_ZVAL(a, 1, 0);
	RETURN_MM();
}

PHP_METHOD(Stub_Arithmetic, mul3)
{
	zend_long a;
	a = 1;
	a *= 1024;
	RETURN_LONG(a);
}

PHP_METHOD(Stub_Arithmetic, less1)
{
	zend_long a, b;
	a = 1;
	b = 2;
	RETURN_BOOL(a < b);
}

PHP_METHOD(Stub_Arithmetic, less2)
{
	zend_long a, b;
	a = 2;
	b = 1;
	RETURN_BOOL(a < b);
}

PHP_METHOD(Stub_Arithmetic, less3)
{
	zval *a, a_sub;

	ZVAL_UNDEF(&a_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(a)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &a);
	RETURN_BOOL(ZEPHIR_LT_LONG(a, 5));
}

PHP_METHOD(Stub_Arithmetic, less4)
{
	zval *a, a_sub;

	ZVAL_UNDEF(&a_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(a)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &a);
	RETURN_BOOL(ZEPHIR_LT_LONG(a, 5.0));
}

PHP_METHOD(Stub_Arithmetic, greater1)
{
	zend_long a, b;
	a = 1;
	b = 2;
	RETURN_BOOL(a > b);
}

PHP_METHOD(Stub_Arithmetic, greater2)
{
	zend_long a, b;
	a = 2;
	b = 1;
	RETURN_BOOL(a > b);
}

PHP_METHOD(Stub_Arithmetic, greater3)
{
	zval *a, a_sub;

	ZVAL_UNDEF(&a_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(a)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &a);
	RETURN_BOOL(ZEPHIR_GT_LONG(a, 5));
}

PHP_METHOD(Stub_Arithmetic, greater4)
{
	zval *a, a_sub;

	ZVAL_UNDEF(&a_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(a)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &a);
	RETURN_BOOL(ZEPHIR_GT_LONG(a, 5.0));
}

PHP_METHOD(Stub_Arithmetic, letStatementIntMinus)
{
	zend_long a = 0;
	a = -1;
	RETURN_LONG(a);
}

PHP_METHOD(Stub_Arithmetic, declaredIntMinus)
{
	zend_long a;
	a = -1;
	RETURN_LONG(a);
}

/**
 * Boolean var can`t be -1 when we set -1
 */
PHP_METHOD(Stub_Arithmetic, letStatementBoolMinus)
{
	zend_bool a = 0;
	zval *b, b_sub;

	ZVAL_UNDEF(&b_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(b)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &b);
	zephir_negate(b);
	a = zephir_is_true(b);
	RETURN_BOOL(a);
}

PHP_METHOD(Stub_Arithmetic, letStatementVarMinus)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *b, b_sub, a;

	ZVAL_UNDEF(&b_sub);
	ZVAL_UNDEF(&a);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(b)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &b);
	zephir_negate(b);
	ZEPHIR_CPY_WRT(&a, b);
	RETURN_CCTOR(&a);
}

PHP_METHOD(Stub_Arithmetic, div1)
{
	zend_long a;
	a = 100;
	RETURN_DOUBLE(zephir_safe_div_long_long(((a - 1)), 4));
}

PHP_METHOD(Stub_Arithmetic, div2)
{
	zval _0;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	zephir_read_property(&_0, this_ptr, ZEND_STRL("tmp1"), PH_NOISY_CC | PH_READONLY);
	RETURN_DOUBLE(zephir_safe_div_long_long(((zephir_get_numberval(&_0) - 1)), 4));
}

/**
 * @issue https://github.com/zephir-lang/zephir/issues/812
 */
PHP_METHOD(Stub_Arithmetic, absParam)
{
	zval *val_param = NULL;
	zend_ulong val;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(val)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &val_param);
	if (UNEXPECTED(Z_TYPE_P(val_param) != IS_LONG)) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'val' must be of the type uint"));
		RETURN_NULL();
	}
	val = ZEND_ABS(Z_LVAL_P(val_param));
	RETURN_LONG(val);
}

PHP_METHOD(Stub_Arithmetic, negativeInt)
{
	zval *val_param = NULL;
	zend_long val;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(val)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &val_param);
	RETURN_LONG(val);
}

PHP_METHOD(Stub_Arithmetic, negativeLong)
{
	zval *val_param = NULL;
	long val;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(val)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &val_param);
	RETURN_LONG(val);
}

