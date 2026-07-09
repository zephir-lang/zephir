
extern zend_class_entry *stub_range_ce;

ZEPHIR_INIT_CLASS(Stub_Range);

PHP_METHOD(Stub_Range, inclusive1);
PHP_METHOD(Stub_Range, exclusive1);
PHP_METHOD(Stub_Range, loopInclusive);
PHP_METHOD(Stub_Range, loopExclusive);
PHP_METHOD(Stub_Range, loopReverse);
PHP_METHOD(Stub_Range, loopVarBounds);
PHP_METHOD(Stub_Range, loopRangeFn);
PHP_METHOD(Stub_Range, loopChars);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_range_inclusive1, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_range_exclusive1, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_range_loopinclusive, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_range_loopexclusive, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_range_loopreverse, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_range_loopvarbounds, 0, 2, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, a, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, b, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_range_looprangefn, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_range_loopchars, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_range_method_entry) {
PHP_ME(Stub_Range, inclusive1, arginfo_stub_range_inclusive1, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Range, exclusive1, arginfo_stub_range_exclusive1, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Range, loopInclusive, arginfo_stub_range_loopinclusive, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Range, loopExclusive, arginfo_stub_range_loopexclusive, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Range, loopReverse, arginfo_stub_range_loopreverse, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Range, loopVarBounds, arginfo_stub_range_loopvarbounds, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Range, loopRangeFn, arginfo_stub_range_looprangefn, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Range, loopChars, arginfo_stub_range_loopchars, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
