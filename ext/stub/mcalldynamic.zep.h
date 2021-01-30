
extern zend_class_entry *stub_mcalldynamic_ce;

ZEPHIR_INIT_CLASS(Stub_McallDynamic);

PHP_METHOD(Stub_McallDynamic, testMethod1);
PHP_METHOD(Stub_McallDynamic, testMagicCall1);
PHP_METHOD(Stub_McallDynamic, __call);
PHP_METHOD(Stub_McallDynamic, testCallAnonymousFunctionWithContext);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_mcalldynamic_testmethod1, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_mcalldynamic_testmagiccall1, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_mcalldynamic___call, 0, 0, 2)
	ZEND_ARG_INFO(0, method)
	ZEND_ARG_INFO(0, arguments)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_mcalldynamic_testcallanonymousfunctionwithcontext, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_mcalldynamic_method_entry) {
	PHP_ME(Stub_McallDynamic, testMethod1, arginfo_stub_mcalldynamic_testmethod1, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_McallDynamic, testMagicCall1, arginfo_stub_mcalldynamic_testmagiccall1, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_McallDynamic, __call, arginfo_stub_mcalldynamic___call, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_McallDynamic, testCallAnonymousFunctionWithContext, arginfo_stub_mcalldynamic_testcallanonymousfunctionwithcontext, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
