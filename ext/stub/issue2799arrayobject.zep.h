
extern zend_class_entry *stub_issue2799arrayobject_ce;

ZEPHIR_INIT_CLASS(Stub_Issue2799ArrayObject);

PHP_METHOD(Stub_Issue2799ArrayObject, getExtra);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2799arrayobject_getextra, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_issue2799arrayobject_method_entry) {
PHP_ME(Stub_Issue2799ArrayObject, getExtra, arginfo_stub_issue2799arrayobject_getextra, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
