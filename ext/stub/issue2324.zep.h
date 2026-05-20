
extern zend_class_entry *stub_issue2324_ce;

ZEPHIR_INIT_CLASS(Stub_Issue2324);

PHP_METHOD(Stub_Issue2324, __construct);
PHP_METHOD(Stub_Issue2324, makeStatic);
PHP_METHOD(Stub_Issue2324, makeSelf);
PHP_METHOD(Stub_Issue2324, makeStaticInstance);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2324___construct, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2324_makestatic, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2324_makeself, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2324_makestaticinstance, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_issue2324_method_entry) {
PHP_ME(Stub_Issue2324, __construct, arginfo_stub_issue2324___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
PHP_ME(Stub_Issue2324, makeStatic, arginfo_stub_issue2324_makestatic, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
PHP_ME(Stub_Issue2324, makeSelf, arginfo_stub_issue2324_makeself, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
PHP_ME(Stub_Issue2324, makeStaticInstance, arginfo_stub_issue2324_makestaticinstance, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
