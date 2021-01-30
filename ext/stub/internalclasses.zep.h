
extern zend_class_entry *stub_internalclasses_ce;

ZEPHIR_INIT_CLASS(Stub_InternalClasses);

PHP_METHOD(Stub_InternalClasses, testStaticCall);
PHP_METHOD(Stub_InternalClasses, testStaticPropertyFetch);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_internalclasses_teststaticcall, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_internalclasses_teststaticpropertyfetch, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_internalclasses_method_entry) {
	PHP_ME(Stub_InternalClasses, testStaticCall, arginfo_stub_internalclasses_teststaticcall, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_InternalClasses, testStaticPropertyFetch, arginfo_stub_internalclasses_teststaticpropertyfetch, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
