
extern zend_class_entry *stub_destructure_ce;

ZEPHIR_INIT_CLASS(Stub_Destructure);

PHP_METHOD(Stub_Destructure, basic);
PHP_METHOD(Stub_Destructure, skipped);
PHP_METHOD(Stub_Destructure, trailingSkip);
PHP_METHOD(Stub_Destructure, literal);
PHP_METHOD(Stub_Destructure, pair);
PHP_METHOD(Stub_Destructure, fromCall);
PHP_METHOD(Stub_Destructure, swap);
PHP_METHOD(Stub_Destructure, fewerTargets);
PHP_METHOD(Stub_Destructure, missingSlot);
PHP_METHOD(Stub_Destructure, nestedValue);
PHP_METHOD(Stub_Destructure, typedTargets);
PHP_METHOD(Stub_Destructure, keyedSource);
PHP_METHOD(Stub_Destructure, inLoop);
PHP_METHOD(Stub_Destructure, chained);
PHP_METHOD(Stub_Destructure, fromProperty);
zend_object *zephir_init_properties_Stub_Destructure(zend_class_entry *class_type);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_destructure_basic, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_destructure_skipped, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_destructure_trailingskip, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_destructure_literal, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_destructure_pair, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_destructure_fromcall, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_destructure_swap, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_destructure_fewertargets, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_destructure_missingslot, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_destructure_nestedvalue, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_destructure_typedtargets, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_destructure_keyedsource, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_destructure_inloop, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_destructure_chained, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_destructure_fromproperty, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_destructure_zephir_init_properties_stub_destructure, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_destructure_method_entry) {
	PHP_ME(Stub_Destructure, basic, arginfo_stub_destructure_basic, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Destructure, skipped, arginfo_stub_destructure_skipped, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Destructure, trailingSkip, arginfo_stub_destructure_trailingskip, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Destructure, literal, arginfo_stub_destructure_literal, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Destructure, pair, arginfo_stub_destructure_pair, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Destructure, fromCall, arginfo_stub_destructure_fromcall, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Destructure, swap, arginfo_stub_destructure_swap, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Destructure, fewerTargets, arginfo_stub_destructure_fewertargets, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Destructure, missingSlot, arginfo_stub_destructure_missingslot, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Destructure, nestedValue, arginfo_stub_destructure_nestedvalue, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Destructure, typedTargets, arginfo_stub_destructure_typedtargets, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Destructure, keyedSource, arginfo_stub_destructure_keyedsource, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Destructure, inLoop, arginfo_stub_destructure_inloop, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Destructure, chained, arginfo_stub_destructure_chained, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Destructure, fromProperty, arginfo_stub_destructure_fromproperty, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
