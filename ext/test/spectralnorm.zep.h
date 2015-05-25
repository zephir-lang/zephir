
extern zend_class_entry *test_spectralnorm_ce;

ZEPHIR_INIT_CLASS(Test_SpectralNorm);

PHP_METHOD(Test_SpectralNorm, Ax);
PHP_METHOD(Test_SpectralNorm, Au);
PHP_METHOD(Test_SpectralNorm, Atu);
PHP_METHOD(Test_SpectralNorm, AtAu);
PHP_METHOD(Test_SpectralNorm, process);

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_spectralnorm_ax, 0, 0, 2)
	ZEND_ARG_INFO(0, i)
	ZEND_ARG_INFO(0, j)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_spectralnorm_au, 0, 0, 3)
	ZEND_ARG_INFO(0, n)
	ZEND_ARG_OBJ_INFO(0, u, SplFixedArray, 0)
	ZEND_ARG_OBJ_INFO(0, v, SplFixedArray, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_spectralnorm_atu, 0, 0, 3)
	ZEND_ARG_INFO(0, n)
	ZEND_ARG_OBJ_INFO(0, u, SplFixedArray, 0)
	ZEND_ARG_OBJ_INFO(0, v, SplFixedArray, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_spectralnorm_atau, 0, 0, 4)
	ZEND_ARG_INFO(0, n)
	ZEND_ARG_INFO(0, u)
	ZEND_ARG_INFO(0, v)
	ZEND_ARG_INFO(0, w)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_spectralnorm_process, 0, 0, 1)
	ZEND_ARG_INFO(0, n)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_spectralnorm_method_entry) {
	PHP_ME(Test_SpectralNorm, Ax, arginfo_test_spectralnorm_ax, ZEND_ACC_PRIVATE)
	PHP_ME(Test_SpectralNorm, Au, arginfo_test_spectralnorm_au, ZEND_ACC_PRIVATE)
	PHP_ME(Test_SpectralNorm, Atu, arginfo_test_spectralnorm_atu, ZEND_ACC_PRIVATE)
	PHP_ME(Test_SpectralNorm, AtAu, arginfo_test_spectralnorm_atau, ZEND_ACC_PRIVATE)
	PHP_ME(Test_SpectralNorm, process, arginfo_test_spectralnorm_process, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
