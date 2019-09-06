
extern zend_class_entry *test_bench_foo_ce;

ZEPHIR_INIT_CLASS(Test_Bench_Foo);

PHP_METHOD(Test_Bench_Foo, emptyForInRange);
PHP_METHOD(Test_Bench_Foo, readStatic);
PHP_METHOD(Test_Bench_Foo, writeStatic);
PHP_METHOD(Test_Bench_Foo, issetStatic);
PHP_METHOD(Test_Bench_Foo, emptyStatic);
PHP_METHOD(Test_Bench_Foo, readProp);
PHP_METHOD(Test_Bench_Foo, writeProp);
PHP_METHOD(Test_Bench_Foo, assignAddProp);
PHP_METHOD(Test_Bench_Foo, postIncProp);
PHP_METHOD(Test_Bench_Foo, postDecProp);
PHP_METHOD(Test_Bench_Foo, issetProp);
PHP_METHOD(Test_Bench_Foo, emptyProp);
PHP_METHOD(Test_Bench_Foo, g);
PHP_METHOD(Test_Bench_Foo, call);
PHP_METHOD(Test_Bench_Foo, staticMethod);
PHP_METHOD(Test_Bench_Foo, staticMethod1);
PHP_METHOD(Test_Bench_Foo, scall);
PHP_METHOD(Test_Bench_Foo, scallWithReturnTrue);
PHP_METHOD(Test_Bench_Foo, readConst);

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_bench_foo_emptyforinrange, 0, 0, 1)
	ZEND_ARG_INFO(0, n)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_bench_foo_readstatic, 0, 0, 1)
	ZEND_ARG_INFO(0, n)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_bench_foo_writestatic, 0, 0, 1)
	ZEND_ARG_INFO(0, n)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_bench_foo_issetstatic, 0, 0, 1)
	ZEND_ARG_INFO(0, n)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_bench_foo_emptystatic, 0, 0, 1)
	ZEND_ARG_INFO(0, n)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_bench_foo_readprop, 0, 0, 1)
	ZEND_ARG_INFO(0, n)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_bench_foo_writeprop, 0, 0, 1)
	ZEND_ARG_INFO(0, n)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_bench_foo_assignaddprop, 0, 0, 1)
	ZEND_ARG_INFO(0, n)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_bench_foo_postincprop, 0, 0, 1)
	ZEND_ARG_INFO(0, n)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_bench_foo_postdecprop, 0, 0, 1)
	ZEND_ARG_INFO(0, n)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_bench_foo_issetprop, 0, 0, 1)
	ZEND_ARG_INFO(0, n)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_bench_foo_emptyprop, 0, 0, 1)
	ZEND_ARG_INFO(0, n)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_bench_foo_call, 0, 0, 1)
	ZEND_ARG_INFO(0, n)
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70100
#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_bench_foo_staticmethod, 0, 0, IS_VOID, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_bench_foo_staticmethod, 0, 0, IS_VOID, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#else
#define arginfo_test_bench_foo_staticmethod NULL
#endif
#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_bench_foo_staticmethod1, 0, 0, _IS_BOOL, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_bench_foo_staticmethod1, 0, 0, _IS_BOOL, NULL, 0)
#endif
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_bench_foo_scall, 0, 0, 1)
	ZEND_ARG_INFO(0, n)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_bench_foo_scallwithreturntrue, 0, 0, 1)
	ZEND_ARG_INFO(0, n)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_bench_foo_readconst, 0, 0, 1)
	ZEND_ARG_INFO(0, n)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_bench_foo_method_entry) {
	PHP_ME(Test_Bench_Foo, emptyForInRange, arginfo_test_bench_foo_emptyforinrange, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Test_Bench_Foo, readStatic, arginfo_test_bench_foo_readstatic, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Test_Bench_Foo, writeStatic, arginfo_test_bench_foo_writestatic, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Test_Bench_Foo, issetStatic, arginfo_test_bench_foo_issetstatic, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Test_Bench_Foo, emptyStatic, arginfo_test_bench_foo_emptystatic, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Test_Bench_Foo, readProp, arginfo_test_bench_foo_readprop, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bench_Foo, writeProp, arginfo_test_bench_foo_writeprop, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bench_Foo, assignAddProp, arginfo_test_bench_foo_assignaddprop, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bench_Foo, postIncProp, arginfo_test_bench_foo_postincprop, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bench_Foo, postDecProp, arginfo_test_bench_foo_postdecprop, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bench_Foo, issetProp, arginfo_test_bench_foo_issetprop, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bench_Foo, emptyProp, arginfo_test_bench_foo_emptyprop, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bench_Foo, g, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bench_Foo, call, arginfo_test_bench_foo_call, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bench_Foo, staticMethod, arginfo_test_bench_foo_staticmethod, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bench_Foo, staticMethod1, arginfo_test_bench_foo_staticmethod1, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bench_Foo, scall, arginfo_test_bench_foo_scall, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bench_Foo, scallWithReturnTrue, arginfo_test_bench_foo_scallwithreturntrue, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bench_Foo, readConst, arginfo_test_bench_foo_readconst, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
