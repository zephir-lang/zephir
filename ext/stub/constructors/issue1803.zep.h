
extern zend_class_entry *stub_constructors_issue1803_ce;

ZEPHIR_INIT_CLASS(Stub_Constructors_Issue1803);

PHP_METHOD(Stub_Constructors_Issue1803, Issue1803);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_constructors_issue1803_issue1803, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_constructors_issue1803_method_entry) {
PHP_ME(Stub_Constructors_Issue1803, Issue1803, arginfo_stub_constructors_issue1803_issue1803, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
