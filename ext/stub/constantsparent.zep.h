
extern zend_class_entry *stub_constantsparent_ce;

ZEPHIR_INIT_CLASS(Stub_ConstantsParent);

PHP_METHOD(Stub_ConstantsParent, testStaticClassInherited);
PHP_METHOD(Stub_ConstantsParent, testSelfClassInherited);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_constantsparent_teststaticclassinherited, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_constantsparent_testselfclassinherited, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_constantsparent_method_entry) {
PHP_ME(Stub_ConstantsParent, testStaticClassInherited, arginfo_stub_constantsparent_teststaticclassinherited, ZEND_ACC_PUBLIC)
PHP_ME(Stub_ConstantsParent, testSelfClassInherited, arginfo_stub_constantsparent_testselfclassinherited, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
