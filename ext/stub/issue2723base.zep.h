
extern zend_class_entry *stub_issue2723base_ce;

ZEPHIR_INIT_CLASS(Stub_Issue2723Base);

PHP_METHOD(Stub_Issue2723Base, baseTarget);
PHP_METHOD(Stub_Issue2723Base, baseWithArgument);
PHP_METHOD(Stub_Issue2723Base, baseCalledClass);
PHP_METHOD(Stub_Issue2723Base, baseInstance);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2723base_basetarget, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2723base_basewithargument, 0, 1, IS_STRING, 0)
	ZEND_ARG_INFO(0, argument)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2723base_basecalledclass, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2723base_baseinstance, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_issue2723base_method_entry) {
	PHP_ME(Stub_Issue2723Base, baseTarget, arginfo_stub_issue2723base_basetarget, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2723Base, baseWithArgument, arginfo_stub_issue2723base_basewithargument, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2723Base, baseCalledClass, arginfo_stub_issue2723base_basecalledclass, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2723Base, baseInstance, arginfo_stub_issue2723base_baseinstance, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
