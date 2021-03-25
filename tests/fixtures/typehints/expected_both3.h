
extern zend_class_entry *typehints_both_ce;

ZEPHIR_INIT_CLASS(TypeHints_Both);

PHP_METHOD(TypeHints_Both, both);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_typehints_both_both, 0, 1, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, _string, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(typehints_both_method_entry) {
	PHP_ME(TypeHints_Both, both, arginfo_typehints_both_both, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
