
extern zend_class_entry *stub_unspacedminus_ce;

ZEPHIR_INIT_CLASS(Stub_UnspacedMinus);

PHP_METHOD(Stub_UnspacedMinus, rangeLen);
PHP_METHOD(Stub_UnspacedMinus, glued);
PHP_METHOD(Stub_UnspacedMinus, spaceBefore);
PHP_METHOD(Stub_UnspacedMinus, arrayIndex);
PHP_METHOD(Stub_UnspacedMinus, negativeLiteral);
PHP_METHOD(Stub_UnspacedMinus, gluedAdd);
PHP_METHOD(Stub_UnspacedMinus, gluedMul);
PHP_METHOD(Stub_UnspacedMinus, gluedDiv);
PHP_METHOD(Stub_UnspacedMinus, gluedMod);
PHP_METHOD(Stub_UnspacedMinus, mulByNegative);
PHP_METHOD(Stub_UnspacedMinus, chained);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_unspacedminus_rangelen, 0, 1, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, len, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_unspacedminus_glued, 0, 1, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, a, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_unspacedminus_spacebefore, 0, 1, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, a, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_unspacedminus_arrayindex, 0, 1, IS_LONG, 0)
	ZEND_ARG_ARRAY_INFO(0, a, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_unspacedminus_negativeliteral, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_unspacedminus_gluedadd, 0, 1, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, a, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_unspacedminus_gluedmul, 0, 1, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, a, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_unspacedminus_glueddiv, 0, 1, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, a, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_unspacedminus_gluedmod, 0, 1, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, a, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_unspacedminus_mulbynegative, 0, 1, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, a, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_unspacedminus_chained, 0, 1, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, a, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_unspacedminus_method_entry) {
	PHP_ME(Stub_UnspacedMinus, rangeLen, arginfo_stub_unspacedminus_rangelen, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_UnspacedMinus, glued, arginfo_stub_unspacedminus_glued, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_UnspacedMinus, spaceBefore, arginfo_stub_unspacedminus_spacebefore, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_UnspacedMinus, arrayIndex, arginfo_stub_unspacedminus_arrayindex, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_UnspacedMinus, negativeLiteral, arginfo_stub_unspacedminus_negativeliteral, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_UnspacedMinus, gluedAdd, arginfo_stub_unspacedminus_gluedadd, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_UnspacedMinus, gluedMul, arginfo_stub_unspacedminus_gluedmul, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_UnspacedMinus, gluedDiv, arginfo_stub_unspacedminus_glueddiv, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_UnspacedMinus, gluedMod, arginfo_stub_unspacedminus_gluedmod, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_UnspacedMinus, mulByNegative, arginfo_stub_unspacedminus_mulbynegative, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_UnspacedMinus, chained, arginfo_stub_unspacedminus_chained, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
