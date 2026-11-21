
extern zend_class_entry *stub_issue2839_ce;

ZEPHIR_INIT_CLASS(Stub_Issue2839);

PHP_METHOD(Stub_Issue2839, build);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2839_build, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, className, IS_STRING, 0)
	ZEND_ARG_ARRAY_INFO(0, parameters, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_issue2839_method_entry) {
	PHP_ME(Stub_Issue2839, build, arginfo_stub_issue2839_build, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
