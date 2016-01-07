
extern zend_class_entry *test_bench_foo_ce;

ZEPHIR_INIT_CLASS(Test_Bench_Foo);

PHP_METHOD(Test_Bench_Foo, empty_for_in_range);
PHP_METHOD(Test_Bench_Foo, read_static);
PHP_METHOD(Test_Bench_Foo, write_static);
PHP_METHOD(Test_Bench_Foo, isset_static);
PHP_METHOD(Test_Bench_Foo, empty_static);
PHP_METHOD(Test_Bench_Foo, f);
PHP_METHOD(Test_Bench_Foo, call_static);
PHP_METHOD(Test_Bench_Foo, read_prop);
PHP_METHOD(Test_Bench_Foo, write_prop);
PHP_METHOD(Test_Bench_Foo, assign_add_prop);
PHP_METHOD(Test_Bench_Foo, post_inc_prop);
PHP_METHOD(Test_Bench_Foo, post_dec_prop);
PHP_METHOD(Test_Bench_Foo, isset_prop);
PHP_METHOD(Test_Bench_Foo, empty_prop);
PHP_METHOD(Test_Bench_Foo, g);
PHP_METHOD(Test_Bench_Foo, call);
PHP_METHOD(Test_Bench_Foo, read_const);

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_bench_foo_empty_for_in_range, 0, 0, 1)
	ZEND_ARG_INFO(0, n)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_bench_foo_read_static, 0, 0, 1)
	ZEND_ARG_INFO(0, n)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_bench_foo_write_static, 0, 0, 1)
	ZEND_ARG_INFO(0, n)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_bench_foo_isset_static, 0, 0, 1)
	ZEND_ARG_INFO(0, n)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_bench_foo_empty_static, 0, 0, 1)
	ZEND_ARG_INFO(0, n)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_bench_foo_call_static, 0, 0, 1)
	ZEND_ARG_INFO(0, n)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_bench_foo_read_prop, 0, 0, 1)
	ZEND_ARG_INFO(0, n)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_bench_foo_write_prop, 0, 0, 1)
	ZEND_ARG_INFO(0, n)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_bench_foo_assign_add_prop, 0, 0, 1)
	ZEND_ARG_INFO(0, n)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_bench_foo_post_inc_prop, 0, 0, 1)
	ZEND_ARG_INFO(0, n)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_bench_foo_post_dec_prop, 0, 0, 1)
	ZEND_ARG_INFO(0, n)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_bench_foo_isset_prop, 0, 0, 1)
	ZEND_ARG_INFO(0, n)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_bench_foo_empty_prop, 0, 0, 1)
	ZEND_ARG_INFO(0, n)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_bench_foo_call, 0, 0, 1)
	ZEND_ARG_INFO(0, n)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_bench_foo_read_const, 0, 0, 1)
	ZEND_ARG_INFO(0, n)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_bench_foo_method_entry) {
	PHP_ME(Test_Bench_Foo, empty_for_in_range, arginfo_test_bench_foo_empty_for_in_range, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Test_Bench_Foo, read_static, arginfo_test_bench_foo_read_static, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Test_Bench_Foo, write_static, arginfo_test_bench_foo_write_static, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Test_Bench_Foo, isset_static, arginfo_test_bench_foo_isset_static, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Test_Bench_Foo, empty_static, arginfo_test_bench_foo_empty_static, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Test_Bench_Foo, f, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Test_Bench_Foo, call_static, arginfo_test_bench_foo_call_static, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Test_Bench_Foo, read_prop, arginfo_test_bench_foo_read_prop, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bench_Foo, write_prop, arginfo_test_bench_foo_write_prop, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bench_Foo, assign_add_prop, arginfo_test_bench_foo_assign_add_prop, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bench_Foo, post_inc_prop, arginfo_test_bench_foo_post_inc_prop, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bench_Foo, post_dec_prop, arginfo_test_bench_foo_post_dec_prop, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bench_Foo, isset_prop, arginfo_test_bench_foo_isset_prop, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bench_Foo, empty_prop, arginfo_test_bench_foo_empty_prop, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bench_Foo, g, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bench_Foo, call, arginfo_test_bench_foo_call, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bench_Foo, read_const, arginfo_test_bench_foo_read_const, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
