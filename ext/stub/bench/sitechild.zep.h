
extern zend_class_entry *stub_bench_sitechild_ce;

ZEPHIR_INIT_CLASS(Stub_Bench_SiteChild);

PHP_METHOD(Stub_Bench_SiteChild, siteParentLoop);
PHP_METHOD(Stub_Bench_SiteChild, siteParentOnce);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_bench_sitechild_siteparentloop, 0, 1, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, n, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_bench_sitechild_siteparentonce, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_bench_sitechild_method_entry) {
	PHP_ME(Stub_Bench_SiteChild, siteParentLoop, arginfo_stub_bench_sitechild_siteparentloop, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Bench_SiteChild, siteParentOnce, arginfo_stub_bench_sitechild_siteparentonce, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
