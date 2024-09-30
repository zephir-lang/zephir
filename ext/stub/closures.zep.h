
extern zend_class_entry *stub_closures_ce;

ZEPHIR_INIT_CLASS(Stub_Closures);

PHP_METHOD(Stub_Closures, simple1);
PHP_METHOD(Stub_Closures, simple2);
PHP_METHOD(Stub_Closures, simple3);
PHP_METHOD(Stub_Closures, simple4);
PHP_METHOD(Stub_Closures, simple5);
PHP_METHOD(Stub_Closures, arrow1);
PHP_METHOD(Stub_Closures, arrow2);
PHP_METHOD(Stub_Closures, testUseCommand);
PHP_METHOD(Stub_Closures, issue1860);
PHP_METHOD(Stub_Closures, issue642);
PHP_METHOD(Stub_Closures, issue1036SetArgument);
PHP_METHOD(Stub_Closures, issue1036SetFunction);
PHP_METHOD(Stub_Closures, issue1036Call);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_closures_simple1, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_closures_simple2, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_closures_simple3, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_closures_simple4, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_closures_simple5, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_closures_arrow1, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_closures_arrow2, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_closures_testusecommand, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_closures_issue1860, 0, 0, 1)
	ZEND_ARG_ARRAY_INFO(0, abc, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_closures_issue642, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_closures_issue1036setargument, 0, 0, 1)
	ZEND_ARG_INFO(0, argument)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_closures_issue1036setfunction, 0, 0, 1)
	ZEND_ARG_INFO(0, func)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_closures_issue1036call, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_closures_method_entry) {
PHP_ME(Stub_Closures, simple1, arginfo_stub_closures_simple1, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Closures, simple2, arginfo_stub_closures_simple2, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Closures, simple3, arginfo_stub_closures_simple3, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Closures, simple4, arginfo_stub_closures_simple4, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Closures, simple5, arginfo_stub_closures_simple5, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Closures, arrow1, arginfo_stub_closures_arrow1, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Closures, arrow2, arginfo_stub_closures_arrow2, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Closures, testUseCommand, arginfo_stub_closures_testusecommand, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Closures, issue1860, arginfo_stub_closures_issue1860, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Closures, issue642, arginfo_stub_closures_issue642, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Closures, issue1036SetArgument, arginfo_stub_closures_issue1036setargument, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Closures, issue1036SetFunction, arginfo_stub_closures_issue1036setfunction, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Closures, issue1036Call, arginfo_stub_closures_issue1036call, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
