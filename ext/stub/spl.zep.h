
extern zend_class_entry *stub_spl_ce;

ZEPHIR_INIT_CLASS(Stub_Spl);

PHP_METHOD(Stub_Spl, issue1212);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_spl_issue1212, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, file, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_spl_method_entry) {
	PHP_ME(Stub_Spl, issue1212, arginfo_stub_spl_issue1212, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_FE_END
};
