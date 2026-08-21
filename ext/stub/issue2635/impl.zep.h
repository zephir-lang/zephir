
extern zend_class_entry *stub_issue2635_impl_ce;

ZEPHIR_INIT_CLASS(Stub_Issue2635_Impl);

PHP_METHOD(Stub_Issue2635_Impl, go);
PHP_METHOD(Stub_Issue2635_Impl, extra);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2635_impl_go, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2635_impl_extra, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_issue2635_impl_method_entry) {
	PHP_ME(Stub_Issue2635_Impl, go, arginfo_stub_issue2635_impl_go, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2635_Impl, extra, arginfo_stub_issue2635_impl_extra, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
