
extern zend_class_entry *stub_optimizers_strreplace_ce;

ZEPHIR_INIT_CLASS(Stub_Optimizers_StrReplace);

PHP_METHOD(Stub_Optimizers_StrReplace, issue1055);
PHP_METHOD(Stub_Optimizers_StrReplace, issue1087);
PHP_METHOD(Stub_Optimizers_StrReplace, issue732A);
PHP_METHOD(Stub_Optimizers_StrReplace, issue732B);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_optimizers_strreplace_issue1055, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_optimizers_strreplace_issue1087, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_optimizers_strreplace_issue732a, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_optimizers_strreplace_issue732b, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_optimizers_strreplace_method_entry) {
	PHP_ME(Stub_Optimizers_StrReplace, issue1055, arginfo_stub_optimizers_strreplace_issue1055, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Optimizers_StrReplace, issue1087, arginfo_stub_optimizers_strreplace_issue1087, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Optimizers_StrReplace, issue732A, arginfo_stub_optimizers_strreplace_issue732a, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Optimizers_StrReplace, issue732B, arginfo_stub_optimizers_strreplace_issue732b, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_FE_END
};
