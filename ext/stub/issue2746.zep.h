
extern zend_class_entry *stub_issue2746_ce;

ZEPHIR_INIT_CLASS(Stub_Issue2746);

PHP_METHOD(Stub_Issue2746, toLong);
PHP_METHOD(Stub_Issue2746, castInt);
PHP_METHOD(Stub_Issue2746, intvalOf);
PHP_METHOD(Stub_Issue2746, toDouble);
PHP_METHOD(Stub_Issue2746, castDouble);
PHP_METHOD(Stub_Issue2746, doublevalOf);
PHP_METHOD(Stub_Issue2746, doubleParam);
PHP_METHOD(Stub_Issue2746, optionalDoubleParam);
PHP_METHOD(Stub_Issue2746, nullableDoubleParam);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2746_tolong, 0, 1, IS_LONG, 0)
	ZEND_ARG_INFO(0, a)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2746_castint, 0, 1, IS_LONG, 0)
	ZEND_ARG_INFO(0, a)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2746_intvalof, 0, 1, IS_LONG, 0)
	ZEND_ARG_INFO(0, a)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2746_todouble, 0, 1, IS_DOUBLE, 0)
	ZEND_ARG_INFO(0, a)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2746_castdouble, 0, 1, IS_DOUBLE, 0)
	ZEND_ARG_INFO(0, a)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2746_doublevalof, 0, 1, IS_DOUBLE, 0)
	ZEND_ARG_INFO(0, a)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2746_doubleparam, 0, 1, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, a, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2746_optionaldoubleparam, 0, 0, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, a, IS_DOUBLE, 0, "1.5")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2746_nullabledoubleparam, 0, 0, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, a, IS_DOUBLE, 1, "null")
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_issue2746_method_entry) {
	PHP_ME(Stub_Issue2746, toLong, arginfo_stub_issue2746_tolong, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2746, castInt, arginfo_stub_issue2746_castint, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2746, intvalOf, arginfo_stub_issue2746_intvalof, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2746, toDouble, arginfo_stub_issue2746_todouble, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2746, castDouble, arginfo_stub_issue2746_castdouble, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2746, doublevalOf, arginfo_stub_issue2746_doublevalof, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2746, doubleParam, arginfo_stub_issue2746_doubleparam, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2746, optionalDoubleParam, arginfo_stub_issue2746_optionaldoubleparam, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2746, nullableDoubleParam, arginfo_stub_issue2746_nullabledoubleparam, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
