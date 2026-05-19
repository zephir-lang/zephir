
extern zend_class_entry *stub_issue2505_ce;

ZEPHIR_INIT_CLASS(Stub_Issue2505);

PHP_METHOD(Stub_Issue2505, makeSelf);
PHP_METHOD(Stub_Issue2505, makeSelfStatic);
PHP_METHOD(Stub_Issue2505, makeStatic);
PHP_METHOD(Stub_Issue2505, makeStaticFromStatic);
PHP_METHOD(Stub_Issue2505, chainedStatic);
PHP_METHOD(Stub_Issue2505, chainedSelf);
PHP_METHOD(Stub_Issue2505, checkPrivate);

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_stub_issue2505_makeself, 0, 0, self, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_stub_issue2505_makeselfstatic, 0, 0, self, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_stub_issue2505_makestatic, 0, 0, static, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_stub_issue2505_makestaticfromstatic, 0, 0, static, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2505_chainedstatic, 0, 0, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2505_chainedself, 0, 0, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2505_checkprivate, 0, 0, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_issue2505_method_entry) {
	PHP_ME(Stub_Issue2505, makeSelf, arginfo_stub_issue2505_makeself, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2505, makeSelfStatic, arginfo_stub_issue2505_makeselfstatic, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2505, makeStatic, arginfo_stub_issue2505_makestatic, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2505, makeStaticFromStatic, arginfo_stub_issue2505_makestaticfromstatic, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2505, chainedStatic, arginfo_stub_issue2505_chainedstatic, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2505, chainedSelf, arginfo_stub_issue2505_chainedself, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2505, checkPrivate, arginfo_stub_issue2505_checkprivate, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
