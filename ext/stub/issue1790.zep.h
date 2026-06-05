
extern zend_class_entry *stub_issue1790_ce;

ZEPHIR_INIT_CLASS(Stub_Issue1790);

PHP_METHOD(Stub_Issue1790, setStringProp);
PHP_METHOD(Stub_Issue1790, setVariableProp);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue1790_setstringprop, 0, 0, 1)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue1790_setvariableprop, 0, 0, 2)
	ZEND_ARG_INFO(0, key)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_issue1790_method_entry) {
	PHP_ME(Stub_Issue1790, setStringProp, arginfo_stub_issue1790_setstringprop, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue1790, setVariableProp, arginfo_stub_issue1790_setvariableprop, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
