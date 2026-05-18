
extern zend_class_entry *stub_issue2505_ce;

ZEPHIR_INIT_CLASS(Stub_Issue2505);

PHP_METHOD(Stub_Issue2505, makeSelf);
PHP_METHOD(Stub_Issue2505, makeSelfStatic);
PHP_METHOD(Stub_Issue2505, makeStatic);
PHP_METHOD(Stub_Issue2505, makeStaticFromStatic);

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_stub_issue2505_makeself, 0, 0, self, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_stub_issue2505_makeselfstatic, 0, 0, self, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_stub_issue2505_makestatic, 0, 0, static, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_stub_issue2505_makestaticfromstatic, 0, 0, static, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_issue2505_method_entry) {
	PHP_ME(Stub_Issue2505, makeSelf, arginfo_stub_issue2505_makeself, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2505, makeSelfStatic, arginfo_stub_issue2505_makeselfstatic, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2505, makeStatic, arginfo_stub_issue2505_makestatic, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2505, makeStaticFromStatic, arginfo_stub_issue2505_makestaticfromstatic, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_FE_END
};
