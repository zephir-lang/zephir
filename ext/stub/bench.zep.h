
extern zend_class_entry *stub_bench_ce;

ZEPHIR_INIT_CLASS(Stub_Bench);

PHP_METHOD(Stub_Bench, __construct);
PHP_METHOD(Stub_Bench, forOverArray);
PHP_METHOD(Stub_Bench, forOverIterator);
PHP_METHOD(Stub_Bench, forOverIteratorWithContinue);
PHP_METHOD(Stub_Bench, propertyReadLoop);
PHP_METHOD(Stub_Bench, arrayFetchStringLoop);
PHP_METHOD(Stub_Bench, methodDispatchLoop);
PHP_METHOD(Stub_Bench, returnConst);
PHP_METHOD(Stub_Bench, stringConcatLoop);
PHP_METHOD(Stub_Bench, newInstanceLoop);
PHP_METHOD(Stub_Bench, variadicSum);
PHP_METHOD(Stub_Bench, variadicSumNarrowLoop);
PHP_METHOD(Stub_Bench, variadicSumWideLoop);
PHP_METHOD(Stub_Bench, funcGetArgsSum);
PHP_METHOD(Stub_Bench, funcGetArgsSumLoop);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_bench___construct, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_bench_foroverarray, 0, 1, IS_LONG, 0)
	ZEND_ARG_ARRAY_INFO(0, arr, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_bench_foroveriterator, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_bench_foroveriteratorwithcontinue, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_bench_propertyreadloop, 0, 1, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, n, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_bench_arrayfetchstringloop, 0, 2, IS_LONG, 0)
	ZEND_ARG_ARRAY_INFO(0, arr, 0)
	ZEND_ARG_TYPE_INFO(0, n, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_bench_methoddispatchloop, 0, 1, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, n, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_bench_returnconst, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_bench_stringconcatloop, 0, 2, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, base, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, n, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_bench_newinstanceloop, 0, 1, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, n, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_bench_variadicsum, 0, 0, IS_LONG, 0)
	ZEND_ARG_VARIADIC_INFO(0, numbers)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_bench_variadicsumnarrowloop, 0, 1, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, n, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_bench_variadicsumwideloop, 0, 1, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, n, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_bench_funcgetargssum, 0, 5, IS_LONG, 0)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, b)
	ZEND_ARG_INFO(0, c)
	ZEND_ARG_INFO(0, d)
	ZEND_ARG_INFO(0, e)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_bench_funcgetargssumloop, 0, 1, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, n, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_bench_method_entry) {
PHP_ME(Stub_Bench, __construct, arginfo_stub_bench___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Stub_Bench, forOverArray, arginfo_stub_bench_foroverarray, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Bench, forOverIterator, arginfo_stub_bench_foroveriterator, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Bench, forOverIteratorWithContinue, arginfo_stub_bench_foroveriteratorwithcontinue, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Bench, propertyReadLoop, arginfo_stub_bench_propertyreadloop, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Bench, arrayFetchStringLoop, arginfo_stub_bench_arrayfetchstringloop, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Bench, methodDispatchLoop, arginfo_stub_bench_methoddispatchloop, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Bench, returnConst, arginfo_stub_bench_returnconst, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Bench, stringConcatLoop, arginfo_stub_bench_stringconcatloop, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Bench, newInstanceLoop, arginfo_stub_bench_newinstanceloop, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Bench, variadicSum, arginfo_stub_bench_variadicsum, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Bench, variadicSumNarrowLoop, arginfo_stub_bench_variadicsumnarrowloop, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Bench, variadicSumWideLoop, arginfo_stub_bench_variadicsumwideloop, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Bench, funcGetArgsSum, arginfo_stub_bench_funcgetargssum, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Bench, funcGetArgsSumLoop, arginfo_stub_bench_funcgetargssumloop, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
