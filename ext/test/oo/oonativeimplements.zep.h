
extern zend_class_entry *test_oo_oonativeimplements_ce;

ZEPHIR_INIT_CLASS(Test_Oo_OoNativeImplements);

PHP_METHOD(Test_Oo_OoNativeImplements, count);
PHP_METHOD(Test_Oo_OoNativeImplements, current);
PHP_METHOD(Test_Oo_OoNativeImplements, key);
PHP_METHOD(Test_Oo_OoNativeImplements, next);
PHP_METHOD(Test_Oo_OoNativeImplements, rewind);
PHP_METHOD(Test_Oo_OoNativeImplements, valid);
PHP_METHOD(Test_Oo_OoNativeImplements, getInnerIterator);
PHP_METHOD(Test_Oo_OoNativeImplements, getChildren);
PHP_METHOD(Test_Oo_OoNativeImplements, hasChildren);
PHP_METHOD(Test_Oo_OoNativeImplements, seek);
PHP_METHOD(Test_Oo_OoNativeImplements, getIterator);
PHP_METHOD(Test_Oo_OoNativeImplements, offsetExists);
PHP_METHOD(Test_Oo_OoNativeImplements, offsetGet);
PHP_METHOD(Test_Oo_OoNativeImplements, offsetSet);
PHP_METHOD(Test_Oo_OoNativeImplements, offsetUnset);
PHP_METHOD(Test_Oo_OoNativeImplements, serialize);
PHP_METHOD(Test_Oo_OoNativeImplements, unserialize);

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_oo_oonativeimplements_seek, 0, 0, 1)
	ZEND_ARG_INFO(0, position)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_oo_oonativeimplements_offsetexists, 0, 0, 1)
	ZEND_ARG_INFO(0, offset)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_oo_oonativeimplements_offsetget, 0, 0, 1)
	ZEND_ARG_INFO(0, offset)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_oo_oonativeimplements_offsetset, 0, 0, 2)
	ZEND_ARG_INFO(0, offset)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_oo_oonativeimplements_offsetunset, 0, 0, 1)
	ZEND_ARG_INFO(0, offset)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_oo_oonativeimplements_unserialize, 0, 0, 1)
	ZEND_ARG_INFO(0, serialized)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_oo_oonativeimplements_method_entry) {
	PHP_ME(Test_Oo_OoNativeImplements, count, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Oo_OoNativeImplements, current, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Oo_OoNativeImplements, key, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Oo_OoNativeImplements, next, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Oo_OoNativeImplements, rewind, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Oo_OoNativeImplements, valid, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Oo_OoNativeImplements, getInnerIterator, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Oo_OoNativeImplements, getChildren, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Oo_OoNativeImplements, hasChildren, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Oo_OoNativeImplements, seek, arginfo_test_oo_oonativeimplements_seek, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Oo_OoNativeImplements, getIterator, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Oo_OoNativeImplements, offsetExists, arginfo_test_oo_oonativeimplements_offsetexists, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Oo_OoNativeImplements, offsetGet, arginfo_test_oo_oonativeimplements_offsetget, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Oo_OoNativeImplements, offsetSet, arginfo_test_oo_oonativeimplements_offsetset, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Oo_OoNativeImplements, offsetUnset, arginfo_test_oo_oonativeimplements_offsetunset, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Oo_OoNativeImplements, serialize, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Oo_OoNativeImplements, unserialize, arginfo_test_oo_oonativeimplements_unserialize, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
