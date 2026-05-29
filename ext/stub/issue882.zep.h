
extern zend_class_entry *stub_issue882_ce;

ZEPHIR_INIT_CLASS(Stub_Issue882);

PHP_METHOD(Stub_Issue882, initClass);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue882_initclass, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, className, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_issue882_method_entry) {
	PHP_ME(Stub_Issue882, initClass, arginfo_stub_issue882_initclass, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_FE_END
};
