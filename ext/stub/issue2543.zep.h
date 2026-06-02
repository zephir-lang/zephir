
extern zend_class_entry *stub_issue2543_ce;

ZEPHIR_INIT_CLASS(Stub_Issue2543);

PHP_METHOD(Stub_Issue2543, getVarIntPositive);
PHP_METHOD(Stub_Issue2543, setVarIntPositive);
PHP_METHOD(Stub_Issue2543, getLabel);
PHP_METHOD(Stub_Issue2543, setLabel);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2543_getvarintpositive, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2543_setvarintpositive, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, varIntPositive, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2543_getlabel, 0, 0, IS_STRING, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2543_setlabel, 0, 0, 1)
	ZEND_ARG_INFO(0, label)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_issue2543_method_entry) {
	PHP_ME(Stub_Issue2543, getVarIntPositive, arginfo_stub_issue2543_getvarintpositive, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2543, setVarIntPositive, arginfo_stub_issue2543_setvarintpositive, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2543, getLabel, arginfo_stub_issue2543_getlabel, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2543, setLabel, arginfo_stub_issue2543_setlabel, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
