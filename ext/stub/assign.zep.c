
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
#include "kernel/array.h"
#include "kernel/operators.h"
#include "kernel/fcall.h"


/**
 * Control Flow
 */
ZEPHIR_INIT_CLASS(Stub_Assign)
{
	ZEPHIR_REGISTER_CLASS(Stub, Assign, stub, assign, stub_assign_method_entry, 0);

	zend_declare_property_null(stub_assign_ce, SL("testVar"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(stub_assign_ce, SL("myArray"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(stub_assign_ce, SL("testVarStatic"), ZEND_ACC_PROTECTED|ZEND_ACC_STATIC);
	return SUCCESS;
}

PHP_METHOD(Stub_Assign, getTestVar)
{

	RETURN_MEMBER(getThis(), "testVar");
}

PHP_METHOD(Stub_Assign, getMyArray)
{

	RETURN_MEMBER(getThis(), "myArray");
}

PHP_METHOD(Stub_Assign, testAssign1)
{
	zend_long a = 0;
	a = 1;
	RETURN_LONG(a);
}

PHP_METHOD(Stub_Assign, testAssign2)
{
	zend_long a = 0;
	a = 1;
	RETURN_LONG(a);
}

PHP_METHOD(Stub_Assign, testAssign3)
{
	zend_long a = 0;
	a = 0;
	RETURN_LONG(a);
}

PHP_METHOD(Stub_Assign, testAssign4)
{
	zend_long a = 0;
	a = 0;
	RETURN_LONG(a);
}

PHP_METHOD(Stub_Assign, testAssign5)
{
	zend_long a = 0;
	a = (long) (2.0);
	RETURN_LONG(a);
}

PHP_METHOD(Stub_Assign, testAssign6)
{
	zend_bool a = 0;
	a = ((1) ? 1 : 0);
	RETURN_BOOL(a);
}

PHP_METHOD(Stub_Assign, testAssign7)
{
	zend_bool a = 0;
	a = ((1.0 != 0.0) ? 1 : 0);
	RETURN_BOOL(a);
}

PHP_METHOD(Stub_Assign, testAssign8)
{
	zend_bool a = 0;
	a = 1;
	RETURN_BOOL(a);
}

PHP_METHOD(Stub_Assign, testAssign9)
{
	zend_bool a = 0;
	a = 0;
	RETURN_BOOL(a);
}

PHP_METHOD(Stub_Assign, testAssign10)
{
	zend_bool a = 0;
	a = 0;
	RETURN_BOOL(a);
}

PHP_METHOD(Stub_Assign, testAssign11)
{
	double a = 0;
	a = 0.0;
	RETURN_DOUBLE(a);
}

PHP_METHOD(Stub_Assign, testAssign12)
{
	double a = 0;
	a = (double) (4);
	RETURN_DOUBLE(a);
}

PHP_METHOD(Stub_Assign, testAssign13)
{
	double a = 0;
	a = 0;
	RETURN_DOUBLE(a);
}

PHP_METHOD(Stub_Assign, testAssign14)
{
	double a = 0;
	a = 1;
	RETURN_DOUBLE(a);
}

PHP_METHOD(Stub_Assign, testAssign15)
{
	double a = 0;
	a =  (5.0);
	RETURN_DOUBLE(a);
}

PHP_METHOD(Stub_Assign, testAssign16)
{
	zend_long a = 0;
	a = 1;
	RETURN_LONG(a);
}

PHP_METHOD(Stub_Assign, testAssign17)
{
	double a = 0;
	a =  (1.0);
	RETURN_DOUBLE(a);
}

PHP_METHOD(Stub_Assign, testAssign18)
{
	zend_bool a = 0;
	a = 0;
	RETURN_BOOL(a);
}

PHP_METHOD(Stub_Assign, testAssign19)
{
	zend_bool a = 0;
	a = 1;
	RETURN_BOOL(a);
}

PHP_METHOD(Stub_Assign, testAssign20)
{
	zval a;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&a);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&a);
	ZVAL_NULL(&a);
	RETURN_CCTOR(&a);
}

PHP_METHOD(Stub_Assign, testAssign21)
{
	zend_long a = 0, b = 0;
	a = 1;
	b = a;
	RETURN_LONG(b);
}

PHP_METHOD(Stub_Assign, testAssign22)
{
	double a = 0, b = 0;
	a =  (1.0);
	b =  a;
	RETURN_DOUBLE(b);
}

PHP_METHOD(Stub_Assign, testAssign23)
{
	zend_bool a = 0, b = 0;
	a = 1;
	b = a;
	RETURN_BOOL(b);
}

PHP_METHOD(Stub_Assign, testAssign24)
{
	double b = 0;
	zend_long a = 0;
	a = 1;
	b = (double) a;
	RETURN_DOUBLE(b);
}

PHP_METHOD(Stub_Assign, testAssign25)
{
	double b = 0;
	zend_long a = 0;
	a = 1;
	b = (double) a;
	RETURN_DOUBLE(b);
}

PHP_METHOD(Stub_Assign, testAssign26)
{
	double a = 0;
	zend_long b = 0;
	a = (double) (1);
	b = (long) a;
	RETURN_LONG(b);
}

PHP_METHOD(Stub_Assign, testAssign27)
{
	double a = 0;
	zend_long b = 0;
	b = 1;
	a = (double) b;
	RETURN_DOUBLE(a);
}

PHP_METHOD(Stub_Assign, testAssign28)
{
	zend_bool a = 0;
	zend_long b = 0;
	b = 1;
	a = ((b) ? 1 : 0);
	RETURN_BOOL(a);
}

PHP_METHOD(Stub_Assign, testAssign29)
{
	zend_bool a = 0;
	double b = 0;
	b =  (1.0);
	a = ((b != 0.0) ? 1 : 0);
	RETURN_BOOL(a);
}

PHP_METHOD(Stub_Assign, testAssign30)
{
	zend_long a = 0;
	zend_bool b = 0;
	b = 0;
	a = b;
	RETURN_LONG(a);
}

PHP_METHOD(Stub_Assign, testAssign31)
{
	double a = 0;
	zend_bool b = 0;
	b = 0;
	a = (double) b;
	RETURN_DOUBLE(a);
}

PHP_METHOD(Stub_Assign, testAssign32)
{
	zend_bool b = 0, a = 0;
	b = 0;
	a = b;
	RETURN_BOOL(a);
}

PHP_METHOD(Stub_Assign, testAssign33)
{
	zend_bool b = 0, a = 0;
	b = 0;
	a = b;
	RETURN_BOOL(a);
}

PHP_METHOD(Stub_Assign, testAssign34)
{
	zend_bool a = 0;
	zend_long b = 0;
	b = 0;
	a = ((b) ? 1 : 0);
	RETURN_BOOL(a);
}

PHP_METHOD(Stub_Assign, testAssign35)
{
	zend_bool a = 0;
	double b = 0;
	b = 0;
	a = ((b != 0.0) ? 1 : 0);
	RETURN_BOOL(a);
}

PHP_METHOD(Stub_Assign, testAssign36)
{
	zend_bool b = 0, a = 0;
	b = 0;
	a = b;
	RETURN_BOOL(a);
}

PHP_METHOD(Stub_Assign, testAssign37)
{
	zval _1, _4;
	zval v, arr, _0, _2, _3;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&v);
	ZVAL_UNDEF(&arr);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_3);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_4);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&v);
	ZVAL_STRING(&v, "abc");
	ZEPHIR_INIT_VAR(&arr);
	array_init(&arr);
	ZEPHIR_INIT_VAR(&_0);
	zephir_create_array(&_0, 1, 0);
	ZEPHIR_INIT_VAR(&_1);
	zephir_create_array(&_1, 2, 0);
	add_assoc_stringl_ex(&_1, SL("b_key"), SL("b_val"));
	ZEPHIR_INIT_VAR(&_2);
	array_init(&_2);
	zephir_array_update_string(&_1, SL("b"), &_2, PH_COPY | PH_SEPARATE);
	zephir_array_update_string(&_0, SL("a"), &_1, PH_COPY | PH_SEPARATE);
	ZEPHIR_CPY_WRT(&arr, &_0);
	ZEPHIR_INIT_NVAR(&_2);
	ZVAL_STRING(&_2, "d_val");
	zephir_array_update_multi(&arr, &_2, SL("sss"), 6, SL("a"), SL("b"), SL("d_key"));
	ZEPHIR_INIT_NVAR(&_2);
	ZVAL_LONG(&_2, 1);
	zephir_array_update_string(&arr, SL("s"), &_2, PH_COPY | PH_SEPARATE);
	ZEPHIR_INIT_VAR(&_3);
	ZVAL_STRING(&_3, "f");
	zephir_array_update_multi(&arr, &_3, SL("sssss"), 10, SL("a"), SL("b"), SL("c"), SL("d"), SL("e"));
	ZEPHIR_INIT_NVAR(&_1);
	zephir_create_array(&_1, 1, 0);
	ZEPHIR_INIT_VAR(&_4);
	zephir_create_array(&_4, 1, 0);
	add_index_long(&_4, 3, 4);
	zephir_array_update_long(&_1, 2, &_4, PH_COPY ZEPHIR_DEBUG_PARAMS_DUMMY);
	zephir_array_update_long(&arr, 1, &_1, PH_COPY | PH_SEPARATE ZEPHIR_DEBUG_PARAMS_DUMMY);
	ZEPHIR_INIT_NVAR(&_3);
	ZVAL_LONG(&_3, 6);
	zephir_array_update_multi(&arr, &_3, SL("lll"), 3, 1, 2, 5);
	zephir_array_update_multi(&arr, &v, SL("llz"), 3, 1, 2, &v);
	RETURN_CCTOR(&arr);
}

PHP_METHOD(Stub_Assign, testAssign38)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *index, index_sub, arr, _0;

	ZVAL_UNDEF(&index_sub);
	ZVAL_UNDEF(&arr);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(index)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &index);
	ZEPHIR_INIT_VAR(&arr);
	array_init(&arr);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "val");
	zephir_array_update_zval(&arr, index, &_0, PH_COPY | PH_SEPARATE);
	RETURN_CCTOR(&arr);
}

PHP_METHOD(Stub_Assign, testAssign39)
{
	zval a, b, c;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&b);
	ZVAL_UNDEF(&c);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&a);
	ZVAL_STRING(&a, "hello");
	ZEPHIR_INIT_VAR(&b);
	ZVAL_BOOL(&b, 0);
	ZEPHIR_INIT_NVAR(&a);
	ZVAL_LONG(&a, 10);
	ZEPHIR_INIT_NVAR(&b);
	ZVAL_STRING(&b, "140");
	ZEPHIR_INIT_VAR(&c);
	zephir_add_function(&c, &a, &b);
	RETURN_CCTOR(&c);
}

PHP_METHOD(Stub_Assign, testAssign40)
{
	zval a, b;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&b);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&b);
	object_init(&b);
	ZEPHIR_INIT_VAR(&a);
	array_init(&a);
	ZEPHIR_INIT_NVAR(&a);
	ZVAL_STRING(&a, "123");
	ZEPHIR_INIT_NVAR(&b);
	ZVAL_LONG(&b, 7);
	zephir_sub_function(return_value, &a, &b);
	RETURN_MM();
}

/**
 * @link https://github.com/zephir-lang/zephir/issues/1573
 */
PHP_METHOD(Stub_Assign, testAssign41)
{
	zval *num_param = NULL;
	zend_long num, a = 0;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(num)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &num_param);
	a = 42;
	a /= num;
	RETURN_LONG(a);
}

/**
 * @link https://github.com/zephir-lang/zephir/issues/1573
 */
PHP_METHOD(Stub_Assign, testAssign42)
{
	zval *num_param = NULL;
	zend_long num, a = 0;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(num)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &num_param);
	a = 2;
	num /= a;
	RETURN_LONG(num);
}

PHP_METHOD(Stub_Assign, testAssign43)
{
	zval *num_param = NULL;
	zend_long num, a = 0;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(num)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &num_param);
	a = 43;
	a %= num;
	RETURN_LONG(a);
}

PHP_METHOD(Stub_Assign, testAssign44)
{
	zval *num_param = NULL;
	zend_long num, a = 0;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(num)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &num_param);
	a = 2;
	num %= a;
	RETURN_LONG(num);
}

PHP_METHOD(Stub_Assign, testPropertyAssign1)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval __$null, __$true, __$false, _0, _1;
	zval *this_ptr = getThis();

	ZVAL_NULL(&__$null);
	ZVAL_BOOL(&__$true, 1);
	ZVAL_BOOL(&__$false, 0);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZVAL_UNDEF(&_0);
	ZVAL_LONG(&_0, 1);
	zephir_update_property_zval(this_ptr, ZEND_STRL("testVar"), &_0);
	ZVAL_UNDEF(&_0);
	ZVAL_DOUBLE(&_0, 1.5);
	zephir_update_property_zval(this_ptr, ZEND_STRL("testVar"), &_0);
	zephir_update_property_zval(this_ptr, ZEND_STRL("testVar"), &__$null);
	if (0) {
		zephir_update_property_zval(this_ptr, ZEND_STRL("testVar"), &__$true);
	} else {
		zephir_update_property_zval(this_ptr, ZEND_STRL("testVar"), &__$false);
	}
	if (1) {
		zephir_update_property_zval(this_ptr, ZEND_STRL("testVar"), &__$true);
	} else {
		zephir_update_property_zval(this_ptr, ZEND_STRL("testVar"), &__$false);
	}
	ZVAL_UNDEF(&_0);
	ZVAL_LONG(&_0, 'A');
	zephir_update_property_zval(this_ptr, ZEND_STRL("testVar"), &_0);
	ZEPHIR_INIT_VAR(&_1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_STRING(&_1, "hello");
	zephir_update_property_zval(this_ptr, ZEND_STRL("testVar"), &_1);
	ZEPHIR_INIT_NVAR(&_1);
	array_init(&_1);
	zephir_update_property_zval(this_ptr, ZEND_STRL("testVar"), &_1);
	RETURN_MM_MEMBER(getThis(), "testVar");
}

PHP_METHOD(Stub_Assign, testPropertyAssign2)
{
	zval f;
	char e;
	zend_bool d;
	double b;
	zend_long a;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval __$true, __$false, c, _0;
	zval *this_ptr = getThis();

	ZVAL_BOOL(&__$true, 1);
	ZVAL_BOOL(&__$false, 0);
	ZVAL_UNDEF(&c);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&f);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	a = 1;
	b =  (1.5);
	ZEPHIR_INIT_VAR(&c);
	ZVAL_NULL(&c);
	d = 0;
	e = 'A';
	ZEPHIR_INIT_VAR(&f);
	array_init(&f);
	ZVAL_UNDEF(&_0);
	ZVAL_LONG(&_0, a);
	zephir_update_property_zval(this_ptr, ZEND_STRL("testVar"), &_0);
	ZVAL_UNDEF(&_0);
	ZVAL_DOUBLE(&_0, b);
	zephir_update_property_zval(this_ptr, ZEND_STRL("testVar"), &_0);
	zephir_update_property_zval(this_ptr, ZEND_STRL("testVar"), &c);
	if (d) {
		zephir_update_property_zval(this_ptr, ZEND_STRL("testVar"), &__$true);
	} else {
		zephir_update_property_zval(this_ptr, ZEND_STRL("testVar"), &__$false);
	}
	ZVAL_UNDEF(&_0);
	ZVAL_LONG(&_0, e);
	zephir_update_property_zval(this_ptr, ZEND_STRL("testVar"), &_0);
	zephir_update_property_zval(this_ptr, ZEND_STRL("testVar"), &f);
	RETURN_MM_MEMBER(getThis(), "testVar");
}

PHP_METHOD(Stub_Assign, testPropertyIncr1)
{
	zval _0;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_0);
	ZVAL_LONG(&_0, 1);
	zephir_update_property_zval(this_ptr, ZEND_STRL("testVar"), &_0);
	RETURN_ON_FAILURE(zephir_property_incr(this_ptr, SL("testVar")));
	RETURN_MEMBER(getThis(), "testVar");
}

PHP_METHOD(Stub_Assign, testPropertyAddAssign1)
{
	zval _0, _1;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZVAL_UNDEF(&_0);
	ZVAL_LONG(&_0, 0);
	zephir_update_property_zval(this_ptr, ZEND_STRL("testVar"), &_0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, 2);
	zephir_read_property(&_0, this_ptr, ZEND_STRL("testVar"), PH_NOISY_CC);
	ZEPHIR_ADD_ASSIGN(&_0, &_1)
	zephir_update_property_zval(this_ptr, ZEND_STRL("testVar"), &_0);
	RETURN_MM_MEMBER(getThis(), "testVar");
}

PHP_METHOD(Stub_Assign, testPropertyAddAssign2)
{
	zval _0, _1;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZVAL_UNDEF(&_0);
	ZVAL_LONG(&_0, 1);
	zephir_update_property_zval(this_ptr, ZEND_STRL("testVar"), &_0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, 2);
	zephir_read_property(&_0, this_ptr, ZEND_STRL("testVar"), PH_NOISY_CC);
	ZEPHIR_ADD_ASSIGN(&_0, &_1)
	zephir_update_property_zval(this_ptr, ZEND_STRL("testVar"), &_0);
	RETURN_MM_MEMBER(getThis(), "testVar");
}

PHP_METHOD(Stub_Assign, testPropertyAssignValuePlus1)
{
	zval _0, _1;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_0);
	ZVAL_LONG(&_0, 1);
	zephir_update_property_zval(this_ptr, ZEND_STRL("testVar"), &_0);
	zephir_read_property(&_0, this_ptr, ZEND_STRL("testVar"), PH_NOISY_CC | PH_READONLY);
	ZVAL_UNDEF(&_1);
	ZVAL_LONG(&_1, (zephir_get_numberval(&_0) + 1));
	zephir_update_property_zval(this_ptr, ZEND_STRL("testVar"), &_1);
	RETURN_MEMBER(getThis(), "testVar");
}

PHP_METHOD(Stub_Assign, testPropertyDecr)
{
	zval _0;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_0);
	ZVAL_LONG(&_0, 2);
	zephir_update_property_zval(this_ptr, ZEND_STRL("testVar"), &_0);
	RETURN_ON_FAILURE(zephir_property_decr(this_ptr, SL("testVar")));
	RETURN_MEMBER(getThis(), "testVar");
}

PHP_METHOD(Stub_Assign, testPropertySubAssign1)
{
	zval _0, _1;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZVAL_UNDEF(&_0);
	ZVAL_LONG(&_0, 0);
	zephir_update_property_zval(this_ptr, ZEND_STRL("testVar"), &_0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, 2);
	zephir_read_property(&_0, this_ptr, ZEND_STRL("testVar"), PH_NOISY_CC);
	ZEPHIR_SUB_ASSIGN(&_0, &_1)
	zephir_update_property_zval(this_ptr, ZEND_STRL("testVar"), &_0);
	RETURN_MM_MEMBER(getThis(), "testVar");
}

PHP_METHOD(Stub_Assign, testPropertySubAssign2)
{
	zval _0, _1;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZVAL_UNDEF(&_0);
	ZVAL_LONG(&_0, 1);
	zephir_update_property_zval(this_ptr, ZEND_STRL("testVar"), &_0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, 2);
	zephir_read_property(&_0, this_ptr, ZEND_STRL("testVar"), PH_NOISY_CC);
	ZEPHIR_SUB_ASSIGN(&_0, &_1)
	zephir_update_property_zval(this_ptr, ZEND_STRL("testVar"), &_0);
	RETURN_MM_MEMBER(getThis(), "testVar");
}

PHP_METHOD(Stub_Assign, testPropertyMulAssign1)
{
	zval _0, _1;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZVAL_UNDEF(&_0);
	ZVAL_LONG(&_0, 1);
	zephir_update_property_zval(this_ptr, ZEND_STRL("testVar"), &_0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, 2);
	zephir_read_property(&_0, this_ptr, ZEND_STRL("testVar"), PH_NOISY_CC);
	ZEPHIR_MUL_ASSIGN(&_0, &_1)
	zephir_update_property_zval(this_ptr, ZEND_STRL("testVar"), &_0);
	RETURN_MM_MEMBER(getThis(), "testVar");
}

PHP_METHOD(Stub_Assign, testPropertyMulAssign2)
{
	zval _0, _1;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZVAL_UNDEF(&_0);
	ZVAL_LONG(&_0, 1);
	zephir_update_property_zval(this_ptr, ZEND_STRL("testVar"), &_0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, 3);
	zephir_read_property(&_0, this_ptr, ZEND_STRL("testVar"), PH_NOISY_CC);
	ZEPHIR_MUL_ASSIGN(&_0, &_1)
	zephir_update_property_zval(this_ptr, ZEND_STRL("testVar"), &_0);
	RETURN_MM_MEMBER(getThis(), "testVar");
}

PHP_METHOD(Stub_Assign, testPropertyAssignStringConcat)
{
	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "test");
	zephir_update_property_zval(this_ptr, ZEND_STRL("testVar"), &_0);
	zephir_concat_self_str(&_0, " string", sizeof(" string") - 1);
	zephir_update_property_zval(this_ptr, ZEND_STRL("testVar"), &_0);
	RETURN_MM_MEMBER(getThis(), "testVar");
}

PHP_METHOD(Stub_Assign, testPropertyArray1)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval __$null, __$true, __$false, _0, _1, _2;
	zval *this_ptr = getThis();

	ZVAL_NULL(&__$null);
	ZVAL_BOOL(&__$true, 1);
	ZVAL_BOOL(&__$false, 0);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	array_init(&_0);
	zephir_update_property_zval(this_ptr, ZEND_STRL("myArray"), &_0);
	ZVAL_UNDEF(&_1);
	ZVAL_LONG(&_1, 1);
	zephir_update_property_array_append(this_ptr, SL("myArray"), &_1);
	ZVAL_UNDEF(&_1);
	ZVAL_DOUBLE(&_1, 1.5);
	zephir_update_property_array_append(this_ptr, SL("myArray"), &_1);
	zephir_update_property_array_append(this_ptr, SL("myArray"), &__$null);
	if (0) {
		zephir_update_property_array_append(this_ptr, SL("myArray"), &__$true);
	} else {
		zephir_update_property_array_append(this_ptr, SL("myArray"), &__$false);
	}
	if (1) {
		zephir_update_property_array_append(this_ptr, SL("myArray"), &__$true);
	} else {
		zephir_update_property_array_append(this_ptr, SL("myArray"), &__$false);
	}
	ZVAL_UNDEF(&_1);
	ZVAL_LONG(&_1, 'A');
	zephir_update_property_array_append(this_ptr, SL("myArray"), &_1);
	ZVAL_UNDEF(&_1);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_STRING(&_1, "hello");
	zephir_update_property_array_append(this_ptr, SL("myArray"), &_1);
	ZEPHIR_INIT_VAR(&_2);
	array_init(&_2);
	zephir_update_property_array_append(this_ptr, SL("myArray"), &_2);
	RETURN_MM_MEMBER(getThis(), "myArray");
}

PHP_METHOD(Stub_Assign, testPropertyArray2)
{
	zval f;
	char e;
	zend_bool d;
	zval c, _0, _1;
	double b;
	zend_long a;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&c);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&f);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	a = 1;
	b =  (1.5);
	ZEPHIR_INIT_VAR(&c);
	ZVAL_NULL(&c);
	d = 0;
	e = 'A';
	ZEPHIR_INIT_VAR(&f);
	array_init(&f);
	ZEPHIR_INIT_VAR(&_0);
	array_init(&_0);
	zephir_update_property_zval(this_ptr, ZEND_STRL("myArray"), &_0);
	ZVAL_UNDEF(&_1);
	ZVAL_LONG(&_1, a);
	zephir_update_property_array_append(this_ptr, SL("myArray"), &_1);
	ZVAL_UNDEF(&_1);
	ZVAL_DOUBLE(&_1, b);
	zephir_update_property_array_append(this_ptr, SL("myArray"), &_1);
	zephir_update_property_array_append(this_ptr, SL("myArray"), &c);
	ZVAL_UNDEF(&_1);
	ZVAL_BOOL(&_1, d);
	zephir_update_property_array_append(this_ptr, SL("myArray"), &_1);
	ZVAL_UNDEF(&_1);
	ZVAL_LONG(&_1, e);
	zephir_update_property_array_append(this_ptr, SL("myArray"), &_1);
	zephir_update_property_array_append(this_ptr, SL("myArray"), &f);
	RETURN_MM_MEMBER(getThis(), "myArray");
}

PHP_METHOD(Stub_Assign, testPropertyArray3)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval __$null, __$false, __$true, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11;
	zval *this_ptr = getThis();

	ZVAL_NULL(&__$null);
	ZVAL_BOOL(&__$false, 0);
	ZVAL_BOOL(&__$true, 1);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_3);
	ZVAL_UNDEF(&_4);
	ZVAL_UNDEF(&_5);
	ZVAL_UNDEF(&_6);
	ZVAL_UNDEF(&_7);
	ZVAL_UNDEF(&_8);
	ZVAL_UNDEF(&_9);
	ZVAL_UNDEF(&_10);
	ZVAL_UNDEF(&_11);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	array_init(&_0);
	zephir_update_property_zval(this_ptr, ZEND_STRL("myArray"), &_0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, 0);
	ZEPHIR_INIT_VAR(&_2);
	ZVAL_LONG(&_2, 1);
	zephir_update_property_array(this_ptr, SL("myArray"), &_1, &_2);
	ZEPHIR_INIT_VAR(&_3);
	ZVAL_LONG(&_3, 0);
	ZEPHIR_INIT_VAR(&_4);
	ZVAL_DOUBLE(&_4, 1.5);
	zephir_update_property_array(this_ptr, SL("myArray"), &_3, &_4);
	ZEPHIR_INIT_VAR(&_5);
	ZVAL_LONG(&_5, 0);
	zephir_update_property_array(this_ptr, SL("myArray"), &_5, &__$null);
	ZEPHIR_INIT_VAR(&_6);
	ZVAL_LONG(&_6, 0);
	zephir_update_property_array(this_ptr, SL("myArray"), &_6, &__$false);
	ZEPHIR_INIT_VAR(&_7);
	ZVAL_LONG(&_7, 0);
	zephir_update_property_array(this_ptr, SL("myArray"), &_7, &__$true);
	ZEPHIR_INIT_VAR(&_8);
	ZVAL_LONG(&_8, 0);
	ZEPHIR_INIT_VAR(&_9);
	ZVAL_LONG(&_9, 'A');
	zephir_update_property_array(this_ptr, SL("myArray"), &_8, &_9);
	ZEPHIR_INIT_VAR(&_10);
	array_init(&_10);
	ZEPHIR_INIT_VAR(&_11);
	ZVAL_LONG(&_11, 0);
	zephir_update_property_array(this_ptr, SL("myArray"), &_11, &_10);
	RETURN_MM_MEMBER(getThis(), "myArray");
}

PHP_METHOD(Stub_Assign, testPropertyArray4)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *index, index_sub, __$null, __$false, __$true, _0, _1, _2, _3, _4;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&index_sub);
	ZVAL_NULL(&__$null);
	ZVAL_BOOL(&__$false, 0);
	ZVAL_BOOL(&__$true, 1);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_3);
	ZVAL_UNDEF(&_4);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(index)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &index);
	ZEPHIR_INIT_VAR(&_0);
	array_init(&_0);
	zephir_update_property_zval(this_ptr, ZEND_STRL("myArray"), &_0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, 1);
	zephir_update_property_array(this_ptr, SL("myArray"), index, &_1);
	ZEPHIR_INIT_VAR(&_2);
	ZVAL_DOUBLE(&_2, 1.5);
	zephir_update_property_array(this_ptr, SL("myArray"), index, &_2);
	zephir_update_property_array(this_ptr, SL("myArray"), index, &__$null);
	zephir_update_property_array(this_ptr, SL("myArray"), index, &__$false);
	zephir_update_property_array(this_ptr, SL("myArray"), index, &__$true);
	ZEPHIR_INIT_VAR(&_3);
	ZVAL_LONG(&_3, 'A');
	zephir_update_property_array(this_ptr, SL("myArray"), index, &_3);
	ZEPHIR_INIT_VAR(&_4);
	array_init(&_4);
	zephir_update_property_array(this_ptr, SL("myArray"), index, &_4);
	RETURN_MM_MEMBER(getThis(), "myArray");
}

PHP_METHOD(Stub_Assign, testPropertyArray5)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *index_param = NULL, __$null, __$false, __$true, _0, _1, _2, _3, _4;
	zval index;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&index);
	ZVAL_NULL(&__$null);
	ZVAL_BOOL(&__$false, 0);
	ZVAL_BOOL(&__$true, 1);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_3);
	ZVAL_UNDEF(&_4);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(index)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &index_param);
	zephir_get_strval(&index, index_param);
	ZEPHIR_INIT_VAR(&_0);
	array_init(&_0);
	zephir_update_property_zval(this_ptr, ZEND_STRL("myArray"), &_0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, 1);
	zephir_update_property_array(this_ptr, SL("myArray"), &index, &_1);
	ZEPHIR_INIT_VAR(&_2);
	ZVAL_DOUBLE(&_2, 1.5);
	zephir_update_property_array(this_ptr, SL("myArray"), &index, &_2);
	zephir_update_property_array(this_ptr, SL("myArray"), &index, &__$null);
	zephir_update_property_array(this_ptr, SL("myArray"), &index, &__$false);
	zephir_update_property_array(this_ptr, SL("myArray"), &index, &__$true);
	ZEPHIR_INIT_VAR(&_3);
	ZVAL_LONG(&_3, 'A');
	zephir_update_property_array(this_ptr, SL("myArray"), &index, &_3);
	ZEPHIR_INIT_VAR(&_4);
	array_init(&_4);
	zephir_update_property_array(this_ptr, SL("myArray"), &index, &_4);
	RETURN_MM_MEMBER(getThis(), "myArray");
}

PHP_METHOD(Stub_Assign, testPropertyArray6)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval __$null, __$false, __$true, _0, _1;
	zval *this_ptr = getThis();

	ZVAL_NULL(&__$null);
	ZVAL_BOOL(&__$false, 0);
	ZVAL_BOOL(&__$true, 1);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	array_init(&_0);
	zephir_update_property_zval(this_ptr, ZEND_STRL("myArray"), &_0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, 1);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_1, SL("ll"), 2, 0, 1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_DOUBLE(&_1, 1.5);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_1, SL("ll"), 2, 0, 1);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &__$null, SL("ll"), 2, 0, 1);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &__$false, SL("ll"), 2, 0, 1);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &__$true, SL("ll"), 2, 0, 1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 'A');
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_1, SL("ll"), 2, 0, 1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_STRING(&_1, "hello");
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_1, SL("ll"), 2, 0, 1);
	ZEPHIR_INIT_NVAR(&_1);
	array_init(&_1);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_1, SL("ll"), 2, 0, 1);
	RETURN_MM_MEMBER(getThis(), "myArray");
}

PHP_METHOD(Stub_Assign, testPropertyArray7)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval __$null, __$false, __$true, _0, _1;
	zval *this_ptr = getThis();

	ZVAL_NULL(&__$null);
	ZVAL_BOOL(&__$false, 0);
	ZVAL_BOOL(&__$true, 1);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	array_init(&_0);
	zephir_update_property_zval(this_ptr, ZEND_STRL("myArray"), &_0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, 1);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_1, SL("ss"), 4, SL("hello"), SL("hello"));
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_DOUBLE(&_1, 1.5);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_1, SL("ss"), 4, SL("hello"), SL("hello"));
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &__$null, SL("ss"), 4, SL("hello"), SL("hello"));
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &__$false, SL("ss"), 4, SL("hello"), SL("hello"));
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &__$true, SL("ss"), 4, SL("hello"), SL("hello"));
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 'A');
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_1, SL("ss"), 4, SL("hello"), SL("hello"));
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_STRING(&_1, "hello");
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_1, SL("ss"), 4, SL("hello"), SL("hello"));
	ZEPHIR_INIT_NVAR(&_1);
	array_init(&_1);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_1, SL("ss"), 4, SL("hello"), SL("hello"));
	RETURN_MM_MEMBER(getThis(), "myArray");
}

PHP_METHOD(Stub_Assign, testPropertyArray8)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *index, index_sub, __$null, __$false, __$true, _0, _1;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&index_sub);
	ZVAL_NULL(&__$null);
	ZVAL_BOOL(&__$false, 0);
	ZVAL_BOOL(&__$true, 1);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(index)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &index);
	ZEPHIR_INIT_VAR(&_0);
	array_init(&_0);
	zephir_update_property_zval(this_ptr, ZEND_STRL("myArray"), &_0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, 1);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_1, SL("zz"), 2, index, index);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_DOUBLE(&_1, 1.5);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_1, SL("zz"), 2, index, index);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &__$null, SL("zz"), 2, index, index);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &__$false, SL("zz"), 2, index, index);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &__$true, SL("zz"), 2, index, index);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 'A');
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_1, SL("zz"), 2, index, index);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_STRING(&_1, "hello");
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_1, SL("zz"), 2, index, index);
	ZEPHIR_INIT_NVAR(&_1);
	array_init(&_1);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_1, SL("zz"), 2, index, index);
	RETURN_MM_MEMBER(getThis(), "myArray");
}

PHP_METHOD(Stub_Assign, testPropertyArray9)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *index_param = NULL, __$null, __$false, __$true, _0, _1;
	zend_long index;
	zval *this_ptr = getThis();

	ZVAL_NULL(&__$null);
	ZVAL_BOOL(&__$false, 0);
	ZVAL_BOOL(&__$true, 1);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(index)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &index_param);
	ZEPHIR_INIT_VAR(&_0);
	array_init(&_0);
	zephir_update_property_zval(this_ptr, ZEND_STRL("myArray"), &_0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, 1);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_1, SL("ll"), 2, index, index);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_DOUBLE(&_1, 1.5);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_1, SL("ll"), 2, index, index);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &__$null, SL("ll"), 2, index, index);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &__$false, SL("ll"), 2, index, index);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &__$true, SL("ll"), 2, index, index);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 'A');
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_1, SL("ll"), 2, index, index);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_STRING(&_1, "hello");
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_1, SL("ll"), 2, index, index);
	ZEPHIR_INIT_NVAR(&_1);
	array_init(&_1);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_1, SL("ll"), 2, index, index);
	RETURN_MM_MEMBER(getThis(), "myArray");
}

PHP_METHOD(Stub_Assign, testPropertyArray10)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *index_param = NULL, __$null, __$false, __$true, _0, _1;
	zval index;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&index);
	ZVAL_NULL(&__$null);
	ZVAL_BOOL(&__$false, 0);
	ZVAL_BOOL(&__$true, 1);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(index)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &index_param);
	zephir_get_strval(&index, index_param);
	ZEPHIR_INIT_VAR(&_0);
	array_init(&_0);
	zephir_update_property_zval(this_ptr, ZEND_STRL("myArray"), &_0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, 1);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_1, SL("zz"), 2, &index, &index);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_DOUBLE(&_1, 1.5);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_1, SL("zz"), 2, &index, &index);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &__$null, SL("zz"), 2, &index, &index);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &__$false, SL("zz"), 2, &index, &index);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &__$true, SL("zz"), 2, &index, &index);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 'A');
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_1, SL("zz"), 2, &index, &index);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_STRING(&_1, "hello");
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_1, SL("zz"), 2, &index, &index);
	ZEPHIR_INIT_NVAR(&_1);
	array_init(&_1);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_1, SL("zz"), 2, &index, &index);
	RETURN_MM_MEMBER(getThis(), "myArray");
}

PHP_METHOD(Stub_Assign, testPropertyArray11)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *index, index_sub, __$null, __$false, __$true, _0, _1, _2, _3, _4;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&index_sub);
	ZVAL_NULL(&__$null);
	ZVAL_BOOL(&__$false, 0);
	ZVAL_BOOL(&__$true, 1);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_3);
	ZVAL_UNDEF(&_4);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(index)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &index);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, 1);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_0, SL("zza"), 3, index, index);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_DOUBLE(&_1, 1.5);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_1, SL("zza"), 3, index, index);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &__$null, SL("zza"), 3, index, index);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &__$false, SL("zza"), 3, index, index);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &__$true, SL("zza"), 3, index, index);
	ZEPHIR_INIT_VAR(&_2);
	ZVAL_LONG(&_2, 'A');
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_2, SL("zza"), 3, index, index);
	ZEPHIR_INIT_VAR(&_3);
	ZVAL_STRING(&_3, "hello");
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_3, SL("zza"), 3, index, index);
	ZEPHIR_INIT_VAR(&_4);
	array_init(&_4);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_4, SL("zza"), 3, index, index);
	RETURN_MM_MEMBER(getThis(), "myArray");
}

PHP_METHOD(Stub_Assign, testPropertyArray12)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *index, index_sub, __$null, __$false, __$true, temp1, temp2, temp3, _0, _1, _2, _3, _4, _5, _6;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&index_sub);
	ZVAL_NULL(&__$null);
	ZVAL_BOOL(&__$false, 0);
	ZVAL_BOOL(&__$true, 1);
	ZVAL_UNDEF(&temp1);
	ZVAL_UNDEF(&temp2);
	ZVAL_UNDEF(&temp3);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_3);
	ZVAL_UNDEF(&_4);
	ZVAL_UNDEF(&_5);
	ZVAL_UNDEF(&_6);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(index)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &index);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, 1);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_0, SL("zza"), 3, index, index);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_DOUBLE(&_1, 1.5);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_1, SL("zza"), 3, index, index);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &__$null, SL("zza"), 3, index, index);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &__$false, SL("zza"), 3, index, index);
	zephir_read_property(&_2, this_ptr, ZEND_STRL("myArray"), PH_NOISY_CC | PH_READONLY);
	zephir_memory_observe(&temp1);
	zephir_array_fetch(&temp1, &_2, index, PH_NOISY, "stub/assign.zep", 657);
	zephir_memory_observe(&temp2);
	zephir_read_property(&temp2, this_ptr, ZEND_STRL("myArray"), PH_NOISY_CC);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &__$true, SL("zza"), 3, index, index);
	ZEPHIR_INIT_VAR(&_3);
	ZVAL_LONG(&_3, 'A');
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_3, SL("zza"), 3, index, index);
	ZEPHIR_INIT_VAR(&_4);
	ZVAL_STRING(&_4, "hello");
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_4, SL("zza"), 3, index, index);
	ZEPHIR_INIT_VAR(&_5);
	array_init(&_5);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_5, SL("zza"), 3, index, index);
	zephir_read_property(&_6, this_ptr, ZEND_STRL("myArray"), PH_NOISY_CC | PH_READONLY);
	ZEPHIR_INIT_VAR(&temp3);
	ZVAL_LONG(&temp3, zephir_fast_count_int(&_6));
	RETURN_MM_MEMBER(getThis(), "myArray");
}

PHP_METHOD(Stub_Assign, testPropertyArray13)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *index, index_sub, __$null, __$false, __$true, temp1, temp2, temp3, _0, _1, _2, _3, _4, _5, _6;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&index_sub);
	ZVAL_NULL(&__$null);
	ZVAL_BOOL(&__$false, 0);
	ZVAL_BOOL(&__$true, 1);
	ZVAL_UNDEF(&temp1);
	ZVAL_UNDEF(&temp2);
	ZVAL_UNDEF(&temp3);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_3);
	ZVAL_UNDEF(&_4);
	ZVAL_UNDEF(&_5);
	ZVAL_UNDEF(&_6);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(index)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &index);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, 1);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_0, SL("za"), 2, index);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_DOUBLE(&_1, 1.5);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_1, SL("za"), 2, index);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &__$null, SL("za"), 2, index);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &__$false, SL("za"), 2, index);
	zephir_read_property(&_2, this_ptr, ZEND_STRL("myArray"), PH_NOISY_CC | PH_READONLY);
	zephir_memory_observe(&temp1);
	zephir_array_fetch(&temp1, &_2, index, PH_NOISY, "stub/assign.zep", 674);
	zephir_memory_observe(&temp2);
	zephir_read_property(&temp2, this_ptr, ZEND_STRL("myArray"), PH_NOISY_CC);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &__$true, SL("za"), 2, index);
	ZEPHIR_INIT_VAR(&_3);
	ZVAL_LONG(&_3, 'A');
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_3, SL("za"), 2, index);
	ZEPHIR_INIT_VAR(&_4);
	ZVAL_STRING(&_4, "hello");
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_4, SL("za"), 2, index);
	ZEPHIR_INIT_VAR(&_5);
	array_init(&_5);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_5, SL("za"), 2, index);
	zephir_read_property(&_6, this_ptr, ZEND_STRL("myArray"), PH_NOISY_CC | PH_READONLY);
	ZEPHIR_INIT_VAR(&temp3);
	ZVAL_LONG(&temp3, zephir_fast_count_int(&_6));
	RETURN_MM_MEMBER(getThis(), "myArray");
}

PHP_METHOD(Stub_Assign, testPropertyArray14)
{
	zval _0, _1, _3;
	zval v, _2, _4, _5, _6;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&v);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_4);
	ZVAL_UNDEF(&_5);
	ZVAL_UNDEF(&_6);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_3);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&v);
	ZVAL_STRING(&v, "abc");
	ZEPHIR_INIT_VAR(&_0);
	zephir_create_array(&_0, 1, 0);
	ZEPHIR_INIT_VAR(&_1);
	zephir_create_array(&_1, 1, 0);
	add_assoc_stringl_ex(&_1, SL("b_key"), SL("b_val"));
	zephir_array_update_string(&_0, SL("a"), &_1, PH_COPY | PH_SEPARATE);
	zephir_update_property_zval(this_ptr, ZEND_STRL("myArray"), &_0);
	ZEPHIR_INIT_VAR(&_2);
	ZVAL_STRING(&_2, "d_val");
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_2, SL("sss"), 6, SL("a"), SL("b"), SL("d_key"));
	ZEPHIR_INIT_NVAR(&_1);
	zephir_create_array(&_1, 1, 0);
	ZEPHIR_INIT_VAR(&_3);
	zephir_create_array(&_3, 1, 0);
	add_index_long(&_3, 3, 4);
	zephir_array_update_long(&_1, 2, &_3, PH_COPY ZEPHIR_DEBUG_PARAMS_DUMMY);
	ZEPHIR_INIT_NVAR(&_2);
	ZVAL_LONG(&_2, 1);
	zephir_update_property_array(this_ptr, SL("myArray"), &_2, &_1);
	ZEPHIR_INIT_VAR(&_4);
	ZVAL_STRING(&_4, "s");
	ZEPHIR_INIT_VAR(&_5);
	ZVAL_LONG(&_5, 1);
	zephir_update_property_array(this_ptr, SL("myArray"), &_4, &_5);
	ZEPHIR_INIT_VAR(&_6);
	ZVAL_STRING(&_6, "f");
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_6, SL("sssss"), 10, SL("a"), SL("b"), SL("c"), SL("d"), SL("e"));
	ZEPHIR_INIT_NVAR(&_6);
	ZVAL_LONG(&_6, 6);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_6, SL("lll"), 3, 1, 2, 5);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &v, SL("llz"), 3, 1, 2, &v);
	RETURN_MM_MEMBER(getThis(), "myArray");
}

PHP_METHOD(Stub_Assign, testStaticPropertyAssign1)
{
	zval _2;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval __$null, __$false, __$true, _0, _1, _3;

	ZVAL_NULL(&__$null);
	ZVAL_BOOL(&__$false, 0);
	ZVAL_BOOL(&__$true, 1);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_3);
	ZVAL_UNDEF(&_2);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZVAL_UNDEF(&_0);
	ZVAL_LONG(&_0, 1);
	zephir_update_static_property_ce(stub_assign_ce, ZEND_STRL("testVarStatic"), &_0);
	ZVAL_UNDEF(&_0);
	ZVAL_DOUBLE(&_0, 1.5);
	zephir_update_static_property_ce(stub_assign_ce, ZEND_STRL("testVarStatic"), &_0);
	zephir_update_static_property_ce(stub_assign_ce, ZEND_STRL("testVarStatic"), &__$null);
	zephir_update_static_property_ce(stub_assign_ce, ZEND_STRL("testVarStatic"), &__$false);
	zephir_update_static_property_ce(stub_assign_ce, ZEND_STRL("testVarStatic"), &__$true);
	ZVAL_UNDEF(&_0);
	ZVAL_LONG(&_0, 'A');
	zephir_update_static_property_ce(stub_assign_ce, ZEND_STRL("testVarStatic"), &_0);
	ZVAL_UNDEF(&_0);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "hello");
	zephir_update_static_property_ce(stub_assign_ce, ZEND_STRL("testVarStatic"), &_0);
	ZEPHIR_INIT_VAR(&_1);
	array_init(&_1);
	zephir_update_static_property_ce(stub_assign_ce, ZEND_STRL("testVarStatic"), &_1);
	ZEPHIR_INIT_VAR(&_2);
	zephir_create_array(&_2, 3, 0);
	ZEPHIR_INIT_VAR(&_3);
	ZVAL_LONG(&_3, 1);
	zephir_array_fast_append(&_2, &_3);
	ZEPHIR_INIT_NVAR(&_3);
	ZVAL_LONG(&_3, 2);
	zephir_array_fast_append(&_2, &_3);
	ZEPHIR_INIT_NVAR(&_3);
	ZVAL_LONG(&_3, 3);
	zephir_array_fast_append(&_2, &_3);
	zephir_update_static_property_ce(stub_assign_ce, ZEND_STRL("testVarStatic"), &_2);
	zephir_read_static_property_ce(&_0, stub_assign_ce, SL("testVarStatic"), PH_NOISY_CC | PH_READONLY);
	RETURN_CTOR(&_0);
}

PHP_METHOD(Stub_Assign, testStaticPropertyAssign2)
{
	zval f;
	char e;
	zend_bool d;
	zval c, _0;
	double b;
	zend_long a;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&c);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&f);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	a = 1;
	b =  (1.5);
	ZEPHIR_INIT_VAR(&c);
	ZVAL_NULL(&c);
	d = 0;
	e = 'A';
	ZEPHIR_INIT_VAR(&f);
	array_init(&f);
	ZVAL_UNDEF(&_0);
	ZVAL_LONG(&_0, a);
	zephir_update_static_property_ce(stub_assign_ce, ZEND_STRL("testVarStatic"), &_0);
	ZVAL_UNDEF(&_0);
	ZVAL_DOUBLE(&_0, b);
	zephir_update_static_property_ce(stub_assign_ce, ZEND_STRL("testVarStatic"), &_0);
	zephir_update_static_property_ce(stub_assign_ce, ZEND_STRL("testVarStatic"), &c);
	ZVAL_UNDEF(&_0);
	ZVAL_BOOL(&_0, d);
	zephir_update_static_property_ce(stub_assign_ce, ZEND_STRL("testVarStatic"), &_0);
	ZVAL_UNDEF(&_0);
	ZVAL_LONG(&_0, e);
	zephir_update_static_property_ce(stub_assign_ce, ZEND_STRL("testVarStatic"), &_0);
	zephir_update_static_property_ce(stub_assign_ce, ZEND_STRL("testVarStatic"), &f);
	zephir_read_static_property_ce(&_0, stub_assign_ce, SL("testVarStatic"), PH_NOISY_CC | PH_READONLY);
	RETURN_CTOR(&_0);
}

PHP_METHOD(Stub_Assign, testStaticPropertyArray1)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval __$null, __$false, __$true, _0, _1, _2;

	ZVAL_NULL(&__$null);
	ZVAL_BOOL(&__$false, 0);
	ZVAL_BOOL(&__$true, 1);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	array_init(&_0);
	zephir_update_static_property_ce(stub_assign_ce, ZEND_STRL("testVarStatic"), &_0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, 1);
	zephir_update_static_property_array_multi_ce(stub_assign_ce, SL("testVarStatic"), &_1, SL("l"), 1, 0);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_DOUBLE(&_1, 1.5);
	zephir_update_static_property_array_multi_ce(stub_assign_ce, SL("testVarStatic"), &_1, SL("l"), 1, 0);
	zephir_update_static_property_array_multi_ce(stub_assign_ce, SL("testVarStatic"), &__$null, SL("l"), 1, 0);
	zephir_update_static_property_array_multi_ce(stub_assign_ce, SL("testVarStatic"), &__$false, SL("l"), 1, 0);
	zephir_update_static_property_array_multi_ce(stub_assign_ce, SL("testVarStatic"), &__$true, SL("l"), 1, 0);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 'A');
	zephir_update_static_property_array_multi_ce(stub_assign_ce, SL("testVarStatic"), &_1, SL("l"), 1, 0);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_STRING(&_1, "hello");
	zephir_update_static_property_array_multi_ce(stub_assign_ce, SL("testVarStatic"), &_1, SL("l"), 1, 0);
	ZEPHIR_INIT_NVAR(&_1);
	array_init(&_1);
	zephir_update_static_property_array_multi_ce(stub_assign_ce, SL("testVarStatic"), &_1, SL("l"), 1, 0);
	zephir_read_static_property_ce(&_2, stub_assign_ce, SL("testVarStatic"), PH_NOISY_CC | PH_READONLY);
	RETURN_CTOR(&_2);
}

PHP_METHOD(Stub_Assign, testStaticPropertyArray2)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval __$null, __$false, __$true, _0, _1, _2;

	ZVAL_NULL(&__$null);
	ZVAL_BOOL(&__$false, 0);
	ZVAL_BOOL(&__$true, 1);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	array_init(&_0);
	zephir_update_static_property_ce(stub_assign_ce, ZEND_STRL("testVarStatic"), &_0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, 1);
	zephir_update_static_property_array_multi_ce(stub_assign_ce, SL("testVarStatic"), &_1, SL("s"), 2, SL("hello"));
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_DOUBLE(&_1, 1.5);
	zephir_update_static_property_array_multi_ce(stub_assign_ce, SL("testVarStatic"), &_1, SL("s"), 2, SL("hello"));
	zephir_update_static_property_array_multi_ce(stub_assign_ce, SL("testVarStatic"), &__$null, SL("s"), 2, SL("hello"));
	zephir_update_static_property_array_multi_ce(stub_assign_ce, SL("testVarStatic"), &__$false, SL("s"), 2, SL("hello"));
	zephir_update_static_property_array_multi_ce(stub_assign_ce, SL("testVarStatic"), &__$true, SL("s"), 2, SL("hello"));
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 'A');
	zephir_update_static_property_array_multi_ce(stub_assign_ce, SL("testVarStatic"), &_1, SL("s"), 2, SL("hello"));
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_STRING(&_1, "hello");
	zephir_update_static_property_array_multi_ce(stub_assign_ce, SL("testVarStatic"), &_1, SL("s"), 2, SL("hello"));
	ZEPHIR_INIT_NVAR(&_1);
	array_init(&_1);
	zephir_update_static_property_array_multi_ce(stub_assign_ce, SL("testVarStatic"), &_1, SL("s"), 2, SL("hello"));
	zephir_read_static_property_ce(&_2, stub_assign_ce, SL("testVarStatic"), PH_NOISY_CC | PH_READONLY);
	RETURN_CTOR(&_2);
}

PHP_METHOD(Stub_Assign, testStaticPropertyArray3)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *index, index_sub, __$null, __$false, __$true, _0, _1, _2;

	ZVAL_UNDEF(&index_sub);
	ZVAL_NULL(&__$null);
	ZVAL_BOOL(&__$false, 0);
	ZVAL_BOOL(&__$true, 1);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(index)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &index);
	ZEPHIR_INIT_VAR(&_0);
	array_init(&_0);
	zephir_update_static_property_ce(stub_assign_ce, ZEND_STRL("testVarStatic"), &_0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, 1);
	zephir_update_static_property_array_multi_ce(stub_assign_ce, SL("testVarStatic"), &_1, SL("z"), 1, index);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_DOUBLE(&_1, 1.5);
	zephir_update_static_property_array_multi_ce(stub_assign_ce, SL("testVarStatic"), &_1, SL("z"), 1, index);
	zephir_update_static_property_array_multi_ce(stub_assign_ce, SL("testVarStatic"), &__$null, SL("z"), 1, index);
	zephir_update_static_property_array_multi_ce(stub_assign_ce, SL("testVarStatic"), &__$false, SL("z"), 1, index);
	zephir_update_static_property_array_multi_ce(stub_assign_ce, SL("testVarStatic"), &__$true, SL("z"), 1, index);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 'A');
	zephir_update_static_property_array_multi_ce(stub_assign_ce, SL("testVarStatic"), &_1, SL("z"), 1, index);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_STRING(&_1, "hello");
	zephir_update_static_property_array_multi_ce(stub_assign_ce, SL("testVarStatic"), &_1, SL("z"), 1, index);
	ZEPHIR_INIT_NVAR(&_1);
	array_init(&_1);
	zephir_update_static_property_array_multi_ce(stub_assign_ce, SL("testVarStatic"), &_1, SL("z"), 1, index);
	zephir_read_static_property_ce(&_2, stub_assign_ce, SL("testVarStatic"), PH_NOISY_CC | PH_READONLY);
	RETURN_CTOR(&_2);
}

PHP_METHOD(Stub_Assign, testStaticPropertyArrayAppend)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval __$false, _0, _1, _2;

	ZVAL_BOOL(&__$false, 0);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	array_init(&_0);
	zephir_update_static_property_ce(stub_assign_ce, ZEND_STRL("testVarStatic"), &_0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_STRING(&_1, "test");
	zephir_update_static_property_array_multi_ce(stub_assign_ce, SL("testVarStatic"), &_1, SL("a"), 1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 1);
	zephir_update_static_property_array_multi_ce(stub_assign_ce, SL("testVarStatic"), &_1, SL("a"), 1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_DOUBLE(&_1, 1.5);
	zephir_update_static_property_array_multi_ce(stub_assign_ce, SL("testVarStatic"), &_1, SL("a"), 1);
	zephir_update_static_property_array_multi_ce(stub_assign_ce, SL("testVarStatic"), &__$false, SL("a"), 1);
	ZEPHIR_INIT_NVAR(&_1);
	array_init(&_1);
	zephir_update_static_property_array_multi_ce(stub_assign_ce, SL("testVarStatic"), &_1, SL("a"), 1);
	zephir_read_static_property_ce(&_2, stub_assign_ce, SL("testVarStatic"), PH_NOISY_CC | PH_READONLY);
	RETURN_CTOR(&_2);
}

PHP_METHOD(Stub_Assign, testStaticPropertyArrayMutli1)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval __$null, __$false, __$true, _0, _1, _2;

	ZVAL_NULL(&__$null);
	ZVAL_BOOL(&__$false, 0);
	ZVAL_BOOL(&__$true, 1);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	array_init(&_0);
	zephir_update_static_property_ce(stub_assign_ce, ZEND_STRL("testVarStatic"), &_0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, 1);
	zephir_update_static_property_array_multi_ce(stub_assign_ce, SL("testVarStatic"), &_1, SL("ll"), 2, 0, 0);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_DOUBLE(&_1, 1.5);
	zephir_update_static_property_array_multi_ce(stub_assign_ce, SL("testVarStatic"), &_1, SL("ll"), 2, 0, 0);
	zephir_update_static_property_array_multi_ce(stub_assign_ce, SL("testVarStatic"), &__$null, SL("ll"), 2, 0, 0);
	zephir_update_static_property_array_multi_ce(stub_assign_ce, SL("testVarStatic"), &__$false, SL("ll"), 2, 0, 0);
	zephir_update_static_property_array_multi_ce(stub_assign_ce, SL("testVarStatic"), &__$true, SL("ll"), 2, 0, 0);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 'A');
	zephir_update_static_property_array_multi_ce(stub_assign_ce, SL("testVarStatic"), &_1, SL("ll"), 2, 0, 0);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_STRING(&_1, "hello");
	zephir_update_static_property_array_multi_ce(stub_assign_ce, SL("testVarStatic"), &_1, SL("ll"), 2, 0, 0);
	ZEPHIR_INIT_NVAR(&_1);
	array_init(&_1);
	zephir_update_static_property_array_multi_ce(stub_assign_ce, SL("testVarStatic"), &_1, SL("ll"), 2, 0, 0);
	zephir_read_static_property_ce(&_2, stub_assign_ce, SL("testVarStatic"), PH_NOISY_CC | PH_READONLY);
	RETURN_CTOR(&_2);
}

PHP_METHOD(Stub_Assign, testStaticPropertyArrayMutli2)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval __$null, __$false, __$true, _0, _1, _2;

	ZVAL_NULL(&__$null);
	ZVAL_BOOL(&__$false, 0);
	ZVAL_BOOL(&__$true, 1);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	array_init(&_0);
	zephir_update_static_property_ce(stub_assign_ce, ZEND_STRL("testVarStatic"), &_0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, 1);
	zephir_update_static_property_array_multi_ce(stub_assign_ce, SL("testVarStatic"), &_1, SL("ss"), 4, SL("hello"), SL("hello"));
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_DOUBLE(&_1, 1.5);
	zephir_update_static_property_array_multi_ce(stub_assign_ce, SL("testVarStatic"), &_1, SL("ss"), 4, SL("hello"), SL("hello"));
	zephir_update_static_property_array_multi_ce(stub_assign_ce, SL("testVarStatic"), &__$null, SL("ss"), 4, SL("hello"), SL("hello"));
	zephir_update_static_property_array_multi_ce(stub_assign_ce, SL("testVarStatic"), &__$false, SL("ss"), 4, SL("hello"), SL("hello"));
	zephir_update_static_property_array_multi_ce(stub_assign_ce, SL("testVarStatic"), &__$true, SL("ss"), 4, SL("hello"), SL("hello"));
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 'A');
	zephir_update_static_property_array_multi_ce(stub_assign_ce, SL("testVarStatic"), &_1, SL("ss"), 4, SL("hello"), SL("hello"));
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_STRING(&_1, "hello");
	zephir_update_static_property_array_multi_ce(stub_assign_ce, SL("testVarStatic"), &_1, SL("ss"), 4, SL("hello"), SL("hello"));
	ZEPHIR_INIT_NVAR(&_1);
	array_init(&_1);
	zephir_update_static_property_array_multi_ce(stub_assign_ce, SL("testVarStatic"), &_1, SL("ss"), 4, SL("hello"), SL("hello"));
	zephir_read_static_property_ce(&_2, stub_assign_ce, SL("testVarStatic"), PH_NOISY_CC | PH_READONLY);
	RETURN_CTOR(&_2);
}

PHP_METHOD(Stub_Assign, testStaticPropertyArrayMutli3)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *index, index_sub, __$null, __$false, __$true, _0, _1, _2;

	ZVAL_UNDEF(&index_sub);
	ZVAL_NULL(&__$null);
	ZVAL_BOOL(&__$false, 0);
	ZVAL_BOOL(&__$true, 1);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(index)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &index);
	ZEPHIR_INIT_VAR(&_0);
	array_init(&_0);
	zephir_update_static_property_ce(stub_assign_ce, ZEND_STRL("testVarStatic"), &_0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, 1);
	zephir_update_static_property_array_multi_ce(stub_assign_ce, SL("testVarStatic"), &_1, SL("zz"), 2, index, index);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_DOUBLE(&_1, 1.5);
	zephir_update_static_property_array_multi_ce(stub_assign_ce, SL("testVarStatic"), &_1, SL("zz"), 2, index, index);
	zephir_update_static_property_array_multi_ce(stub_assign_ce, SL("testVarStatic"), &__$null, SL("zz"), 2, index, index);
	zephir_update_static_property_array_multi_ce(stub_assign_ce, SL("testVarStatic"), &__$false, SL("zz"), 2, index, index);
	zephir_update_static_property_array_multi_ce(stub_assign_ce, SL("testVarStatic"), &__$true, SL("zz"), 2, index, index);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 'A');
	zephir_update_static_property_array_multi_ce(stub_assign_ce, SL("testVarStatic"), &_1, SL("zz"), 2, index, index);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_STRING(&_1, "hello");
	zephir_update_static_property_array_multi_ce(stub_assign_ce, SL("testVarStatic"), &_1, SL("zz"), 2, index, index);
	ZEPHIR_INIT_NVAR(&_1);
	array_init(&_1);
	zephir_update_static_property_array_multi_ce(stub_assign_ce, SL("testVarStatic"), &_1, SL("zz"), 2, index, index);
	zephir_read_static_property_ce(&_2, stub_assign_ce, SL("testVarStatic"), PH_NOISY_CC | PH_READONLY);
	RETURN_CTOR(&_2);
}

PHP_METHOD(Stub_Assign, testStaticPropertyArrayMulti4)
{
	zval _0, _1, _3;
	zval v, _2, _4;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&v);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_4);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_3);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&v);
	ZVAL_STRING(&v, "abc");
	ZEPHIR_INIT_VAR(&_0);
	zephir_create_array(&_0, 1, 0);
	ZEPHIR_INIT_VAR(&_1);
	zephir_create_array(&_1, 1, 0);
	add_assoc_stringl_ex(&_1, SL("b_key"), SL("b_val"));
	zephir_array_update_string(&_0, SL("a"), &_1, PH_COPY | PH_SEPARATE);
	zephir_update_static_property_ce(stub_assign_ce, ZEND_STRL("testVarStatic"), &_0);
	ZEPHIR_INIT_VAR(&_2);
	ZVAL_STRING(&_2, "d_val");
	zephir_update_static_property_array_multi_ce(stub_assign_ce, SL("testVarStatic"), &_2, SL("sss"), 6, SL("a"), SL("b"), SL("d_key"));
	ZEPHIR_INIT_NVAR(&_1);
	zephir_create_array(&_1, 1, 0);
	ZEPHIR_INIT_VAR(&_3);
	zephir_create_array(&_3, 1, 0);
	add_index_long(&_3, 3, 4);
	zephir_array_update_long(&_1, 2, &_3, PH_COPY ZEPHIR_DEBUG_PARAMS_DUMMY);
	zephir_update_static_property_array_multi_ce(stub_assign_ce, SL("testVarStatic"), &_1, SL("l"), 1, 1);
	ZEPHIR_INIT_NVAR(&_2);
	ZVAL_LONG(&_2, 1);
	zephir_update_static_property_array_multi_ce(stub_assign_ce, SL("testVarStatic"), &_2, SL("s"), 2, SL("s"));
	ZEPHIR_INIT_NVAR(&_2);
	ZVAL_STRING(&_2, "f");
	zephir_update_static_property_array_multi_ce(stub_assign_ce, SL("testVarStatic"), &_2, SL("sssss"), 10, SL("a"), SL("b"), SL("c"), SL("d"), SL("e"));
	ZEPHIR_INIT_NVAR(&_2);
	ZVAL_LONG(&_2, 6);
	zephir_update_static_property_array_multi_ce(stub_assign_ce, SL("testVarStatic"), &_2, SL("lll"), 3, 1, 2, 5);
	zephir_update_static_property_array_multi_ce(stub_assign_ce, SL("testVarStatic"), &v, SL("llz"), 3, 1, 2, &v);
	zephir_read_static_property_ce(&_4, stub_assign_ce, SL("testVarStatic"), PH_NOISY_CC | PH_READONLY);
	RETURN_CTOR(&_4);
}

PHP_METHOD(Stub_Assign, testStaticPropertyArrayAppend1)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval __$null, __$false, __$true, _0, _1, _2;

	ZVAL_NULL(&__$null);
	ZVAL_BOOL(&__$false, 0);
	ZVAL_BOOL(&__$true, 1);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	array_init(&_0);
	zephir_update_static_property_ce(stub_assign_ce, ZEND_STRL("testVarStatic"), &_0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, 1);
	zephir_update_static_property_array_multi_ce(stub_assign_ce, SL("testVarStatic"), &_1, SL("la"), 2, 0);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_DOUBLE(&_1, 1.5);
	zephir_update_static_property_array_multi_ce(stub_assign_ce, SL("testVarStatic"), &_1, SL("la"), 2, 0);
	zephir_update_static_property_array_multi_ce(stub_assign_ce, SL("testVarStatic"), &__$null, SL("la"), 2, 0);
	zephir_update_static_property_array_multi_ce(stub_assign_ce, SL("testVarStatic"), &__$false, SL("la"), 2, 0);
	zephir_update_static_property_array_multi_ce(stub_assign_ce, SL("testVarStatic"), &__$true, SL("la"), 2, 0);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 'A');
	zephir_update_static_property_array_multi_ce(stub_assign_ce, SL("testVarStatic"), &_1, SL("la"), 2, 0);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_STRING(&_1, "hello");
	zephir_update_static_property_array_multi_ce(stub_assign_ce, SL("testVarStatic"), &_1, SL("la"), 2, 0);
	ZEPHIR_INIT_NVAR(&_1);
	array_init(&_1);
	zephir_update_static_property_array_multi_ce(stub_assign_ce, SL("testVarStatic"), &_1, SL("la"), 2, 0);
	zephir_read_static_property_ce(&_2, stub_assign_ce, SL("testVarStatic"), PH_NOISY_CC | PH_READONLY);
	RETURN_CTOR(&_2);
}

PHP_METHOD(Stub_Assign, testArrayVarAssign1)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *index, index_sub, *value, value_sub, a;

	ZVAL_UNDEF(&index_sub);
	ZVAL_UNDEF(&value_sub);
	ZVAL_UNDEF(&a);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(index)
		Z_PARAM_ZVAL(value)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &index, &value);
	ZEPHIR_INIT_VAR(&a);
	array_init(&a);
	zephir_array_update_zval(&a, index, value, PH_COPY | PH_SEPARATE);
	RETURN_CCTOR(&a);
}

PHP_METHOD(Stub_Assign, testArrayVarAssign2)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *index, index_sub, *value, value_sub, _POST;

	ZVAL_UNDEF(&index_sub);
	ZVAL_UNDEF(&value_sub);
	ZVAL_UNDEF(&_POST);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(index)
		Z_PARAM_ZVAL(value)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &index, &value);
	ZEPHIR_INIT_VAR(&_POST);
	array_init(&_POST);
	zephir_array_update_zval(&_POST, index, value, PH_COPY | PH_SEPARATE);
	RETURN_CCTOR(&_POST);
}

PHP_METHOD(Stub_Assign, testArrayProperty)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *index, index_sub, *value, value_sub, _POST;

	ZVAL_UNDEF(&index_sub);
	ZVAL_UNDEF(&value_sub);
	ZVAL_UNDEF(&_POST);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(index)
		Z_PARAM_ZVAL(value)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &index, &value);
	ZEPHIR_INIT_VAR(&_POST);
	array_init(&_POST);
	zephir_array_update_zval(&_POST, index, value, PH_COPY | PH_SEPARATE);
	RETURN_CCTOR(&_POST);
}

/**
 * @link https://github.com/zephir-lang/zephir/issues/159
 */
PHP_METHOD(Stub_Assign, testGlobalVarAssign)
{
	zval *index, index_sub, *value, value_sub, _POST;

	ZVAL_UNDEF(&index_sub);
	ZVAL_UNDEF(&value_sub);
	ZVAL_UNDEF(&_POST);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(index)
		Z_PARAM_ZVAL(value)
	ZEND_PARSE_PARAMETERS_END();
	zephir_get_global(&_POST, SL("_POST"));
	zephir_fetch_params_without_memory_grow(2, 0, &index, &value);
	zephir_array_update_zval(&_POST, index, value, PH_COPY | PH_SEPARATE);
}

/**
 * @link https://github.com/zephir-lang/zephir/issues/523
 */
PHP_METHOD(Stub_Assign, testConstantKeyAssign)
{
	zval elements;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&elements);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&elements);
	zephir_create_array(&elements, 3, 0);
	add_assoc_long_ex(&elements, SL("abc"), 1);
	add_index_long(&elements, 131072, 131079);
	add_index_long(&elements, 131073, 131080);
	ZEPHIR_MM_RESTORE();
}

/**
 * @link https://github.com/zephir-lang/zephir/issues/746
 *
 */
PHP_METHOD(Stub_Assign, testArrayBoolExpressionAssign)
{
	zval str, _0, _1, _2, _3, _4;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&str);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_3);
	ZVAL_UNDEF(&_4);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&str);
	ZVAL_STRING(&str, "abc");
	ZEPHIR_INIT_VAR(&_0);
	array_init(&_0);
	zephir_update_property_zval(this_ptr, ZEND_STRL("myArray"), &_0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_STRING(&_1, "a");
	ZEPHIR_INIT_VAR(&_2);
	ZVAL_BOOL(&_2, ZEPHIR_IS_STRING(&str, "abc"));
	zephir_update_property_array(this_ptr, SL("myArray"), &_1, &_2);
	ZEPHIR_INIT_VAR(&_3);
	ZVAL_STRING(&_3, "b");
	ZEPHIR_INIT_VAR(&_4);
	ZVAL_BOOL(&_4, !ZEPHIR_IS_STRING(&str, "abc"));
	zephir_update_property_array(this_ptr, SL("myArray"), &_3, &_4);
	RETURN_MM_MEMBER(getThis(), "myArray");
}

PHP_METHOD(Stub_Assign, testAssignBitwiseX)
{
	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a_param = NULL, *b_param = NULL, op, i, result, _1, *_2, _3;
	zend_long a, b, ZEPHIR_LAST_CALL_STATUS, _4$$4, _5$$5, _6$$6, _7$$7, _8$$8, _9$$10, _10$$11, _11$$12, _12$$13, _13$$14;

	ZVAL_UNDEF(&op);
	ZVAL_UNDEF(&i);
	ZVAL_UNDEF(&result);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_3);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_LONG(a)
		Z_PARAM_LONG(b)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &a_param, &b_param);
	ZEPHIR_INIT_VAR(&result);
	array_init(&result);
	ZEPHIR_INIT_VAR(&_0);
	zephir_create_array(&_0, 5, 0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_STRING(&_1, "or");
	zephir_array_fast_append(&_0, &_1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_STRING(&_1, "and");
	zephir_array_fast_append(&_0, &_1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_STRING(&_1, "xor");
	zephir_array_fast_append(&_0, &_1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_STRING(&_1, "shiftleft");
	zephir_array_fast_append(&_0, &_1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_STRING(&_1, "shiftright");
	zephir_array_fast_append(&_0, &_1);
	zephir_is_iterable(&_0, 0, "stub/assign.zep", 941);
	if (Z_TYPE_P(&_0) == IS_ARRAY) {
		ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(&_0), _2)
		{
			ZEPHIR_INIT_NVAR(&op);
			ZVAL_COPY(&op, _2);
			ZEPHIR_INIT_NVAR(&i);
			ZVAL_LONG(&i, a);
			do {
				if (ZEPHIR_IS_STRING(&op, "or")) {
					_4$$4 = ((int) (zephir_get_numberval(&i)) | b);
					ZEPHIR_INIT_NVAR(&i);
					ZVAL_LONG(&i, _4$$4);
					break;
				}
				if (ZEPHIR_IS_STRING(&op, "and")) {
					_5$$5 = ((int) (zephir_get_numberval(&i)) & b);
					ZEPHIR_INIT_NVAR(&i);
					ZVAL_LONG(&i, _5$$5);
					break;
				}
				if (ZEPHIR_IS_STRING(&op, "xor")) {
					_6$$6 = ((int) (zephir_get_numberval(&i)) ^ b);
					ZEPHIR_INIT_NVAR(&i);
					ZVAL_LONG(&i, _6$$6);
					break;
				}
				if (ZEPHIR_IS_STRING(&op, "shiftleft")) {
					_7$$7 = ((int) (zephir_get_numberval(&i)) << b);
					ZEPHIR_INIT_NVAR(&i);
					ZVAL_LONG(&i, _7$$7);
					break;
				}
				if (ZEPHIR_IS_STRING(&op, "shiftright")) {
					_8$$8 = ((int) (zephir_get_numberval(&i)) >> b);
					ZEPHIR_INIT_NVAR(&i);
					ZVAL_LONG(&i, _8$$8);
					break;
				}
			} while(0);

			zephir_array_update_zval(&result, &op, &i, PH_COPY | PH_SEPARATE);
		} ZEND_HASH_FOREACH_END();
	} else {
		ZEPHIR_CALL_METHOD(NULL, &_0, "rewind", NULL, 0);
		zephir_check_call_status();
		while (1) {
			ZEPHIR_CALL_METHOD(&_3, &_0, "valid", NULL, 0);
			zephir_check_call_status();
			if (!zend_is_true(&_3)) {
				break;
			}
			ZEPHIR_CALL_METHOD(&op, &_0, "current", NULL, 0);
			zephir_check_call_status();
				ZEPHIR_INIT_NVAR(&i);
				ZVAL_LONG(&i, a);
				do {
					if (ZEPHIR_IS_STRING(&op, "or")) {
						_9$$10 = ((int) (zephir_get_numberval(&i)) | b);
						ZEPHIR_INIT_NVAR(&i);
						ZVAL_LONG(&i, _9$$10);
						break;
					}
					if (ZEPHIR_IS_STRING(&op, "and")) {
						_10$$11 = ((int) (zephir_get_numberval(&i)) & b);
						ZEPHIR_INIT_NVAR(&i);
						ZVAL_LONG(&i, _10$$11);
						break;
					}
					if (ZEPHIR_IS_STRING(&op, "xor")) {
						_11$$12 = ((int) (zephir_get_numberval(&i)) ^ b);
						ZEPHIR_INIT_NVAR(&i);
						ZVAL_LONG(&i, _11$$12);
						break;
					}
					if (ZEPHIR_IS_STRING(&op, "shiftleft")) {
						_12$$13 = ((int) (zephir_get_numberval(&i)) << b);
						ZEPHIR_INIT_NVAR(&i);
						ZVAL_LONG(&i, _12$$13);
						break;
					}
					if (ZEPHIR_IS_STRING(&op, "shiftright")) {
						_13$$14 = ((int) (zephir_get_numberval(&i)) >> b);
						ZEPHIR_INIT_NVAR(&i);
						ZVAL_LONG(&i, _13$$14);
						break;
					}
				} while(0);

				zephir_array_update_zval(&result, &op, &i, PH_COPY | PH_SEPARATE);
			ZEPHIR_CALL_METHOD(NULL, &_0, "next", NULL, 0);
			zephir_check_call_status();
		}
	}
	ZEPHIR_INIT_NVAR(&op);
	RETURN_CCTOR(&result);
}

/**
 * @link https://github.com/zephir-lang/zephir/issues/725
 */
PHP_METHOD(Stub_Assign, testAssignSuperGlobals)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval _GET, v, _0, _1, _2, _3;

	ZVAL_UNDEF(&_GET);
	ZVAL_UNDEF(&v);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_3);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_get_global(&_GET, SL("_GET"));

	ZEPHIR_INIT_VAR(&v);
	ZVAL_STRING(&v, "stest2");
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "stest");
	zephir_array_update_string(&_GET, SL("steststr"), &_0, PH_COPY | PH_SEPARATE);
	zephir_array_update_string(&_GET, SL("steststr2"), &v, PH_COPY | PH_SEPARATE);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, 1);
	zephir_array_update_string(&_GET, SL("stestint"), &_1, PH_COPY | PH_SEPARATE);
	ZEPHIR_INIT_VAR(&_2);
	ZVAL_LONG(&_2, 2);
	zephir_array_update_string(&_GET, SL("stestint2"), &_2, PH_COPY | PH_SEPARATE);
	ZEPHIR_INIT_VAR(&_3);
	ZVAL_STRING(&_3, "testval");
	zephir_array_update_zval(&_GET, &v, &_3, PH_COPY | PH_SEPARATE);
	ZEPHIR_MM_RESTORE();
}

/**
 * @link https://github.com/zephir-lang/zephir/issues/1917
 */
PHP_METHOD(Stub_Assign, testAssignSuperGlobalsSERVER)
{
	zval _1;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval _SERVER, _0;

	ZVAL_UNDEF(&_SERVER);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_get_global(&_SERVER, SL("_SERVER"));

	ZEPHIR_INIT_VAR(&_0);
	ZEPHIR_INIT_VAR(&_1);
	zephir_create_array(&_1, 2, 0);
	add_assoc_stringl_ex(&_1, SL("g1"), SL("aaa"));
	add_assoc_stringl_ex(&_1, SL("g2"), SL("bbb"));
	zephir_fast_array_merge(&_0, &_SERVER, &_1);
	ZEPHIR_HASH_COPY(&_SERVER, &_0);
	ZEPHIR_MM_RESTORE();
}

/**
 * @link https://github.com/zephir-lang/zephir/issues/1917
 */
PHP_METHOD(Stub_Assign, testAssignSuperGlobalsGET)
{
	zval _1;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval _GET, _0;

	ZVAL_UNDEF(&_GET);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_get_global(&_GET, SL("_GET"));

	ZEPHIR_INIT_VAR(&_0);
	ZEPHIR_INIT_VAR(&_1);
	zephir_create_array(&_1, 2, 0);
	add_assoc_stringl_ex(&_1, SL("g1"), SL("aaa"));
	add_assoc_stringl_ex(&_1, SL("g2"), SL("bbb"));
	zephir_fast_array_merge(&_0, &_GET, &_1);
	ZEPHIR_HASH_COPY(&_GET, &_0);
	ZEPHIR_MM_RESTORE();
}

PHP_METHOD(Stub_Assign, issue597)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval _POST, _GET, s$$4, _0$$4;

	ZVAL_UNDEF(&_POST);
	ZVAL_UNDEF(&_GET);
	ZVAL_UNDEF(&s$$4);
	ZVAL_UNDEF(&_0$$4);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_get_global(&_GET, SL("_GET"));
	zephir_get_global(&_POST, SL("_POST"));

	if (zephir_array_isset_string(&_POST, SL("a"))) {
		if (zephir_array_isset_string(&_GET, SL("r"))) {
		}
	}
	if (zephir_array_isset_string(&_GET, SL("s"))) {
		zephir_array_fetch_string(&_0$$4, &_GET, SL("s"), PH_NOISY | PH_READONLY, "stub/assign.zep", 983);
		ZEPHIR_INIT_VAR(&s$$4);
		ZVAL_LONG(&s$$4, (zephir_get_numberval(&_0$$4) * 5));
		zephir_array_update_string(&_GET, SL("s"), &s$$4, PH_COPY | PH_SEPARATE);
		RETURN_CCTOR(&s$$4);
	}
	ZEPHIR_MM_RESTORE();
}

