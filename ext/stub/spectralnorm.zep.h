
extern zend_class_entry *stub_spectralnorm_ce;

ZEPHIR_INIT_CLASS(Stub_SpectralNorm);

PHP_METHOD(Stub_SpectralNorm, Ax);
PHP_METHOD(Stub_SpectralNorm, Au);
PHP_METHOD(Stub_SpectralNorm, Atu);
PHP_METHOD(Stub_SpectralNorm, AtAu);
PHP_METHOD(Stub_SpectralNorm, process);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_spectralnorm_ax, 0, 0, 2)
	ZEND_ARG_INFO(0, i)
	ZEND_ARG_INFO(0, j)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_spectralnorm_au, 0, 0, 3)
	ZEND_ARG_TYPE_INFO(0, n, IS_LONG, 0)
	ZEND_ARG_OBJ_INFO(0, u, SplFixedArray, 0)
	ZEND_ARG_OBJ_INFO(0, v, SplFixedArray, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_spectralnorm_atu, 0, 0, 3)
	ZEND_ARG_TYPE_INFO(0, n, IS_LONG, 0)
	ZEND_ARG_OBJ_INFO(0, u, SplFixedArray, 0)
	ZEND_ARG_OBJ_INFO(0, v, SplFixedArray, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_spectralnorm_atau, 0, 0, 4)
	ZEND_ARG_INFO(0, n)
	ZEND_ARG_INFO(0, u)
	ZEND_ARG_INFO(0, v)
	ZEND_ARG_INFO(0, w)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_spectralnorm_process, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, n, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_spectralnorm_method_entry) {
	PHP_ME(Stub_SpectralNorm, Ax, arginfo_stub_spectralnorm_ax, ZEND_ACC_PRIVATE)
	PHP_ME(Stub_SpectralNorm, Au, arginfo_stub_spectralnorm_au, ZEND_ACC_PRIVATE)
	PHP_ME(Stub_SpectralNorm, Atu, arginfo_stub_spectralnorm_atu, ZEND_ACC_PRIVATE)
	PHP_ME(Stub_SpectralNorm, AtAu, arginfo_stub_spectralnorm_atau, ZEND_ACC_PRIVATE)
	PHP_ME(Stub_SpectralNorm, process, arginfo_stub_spectralnorm_process, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
