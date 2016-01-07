
extern zend_class_entry *test_router_ce;

ZEPHIR_INIT_CLASS(Test_Router);

PHP_METHOD(Test_Router, __construct);
PHP_METHOD(Test_Router, setDI);
PHP_METHOD(Test_Router, getDI);
PHP_METHOD(Test_Router, getRewriteUri);
PHP_METHOD(Test_Router, setUriSource);
PHP_METHOD(Test_Router, removeExtraSlashes);
PHP_METHOD(Test_Router, setDefaultNamespace);
PHP_METHOD(Test_Router, setDefaultModule);
PHP_METHOD(Test_Router, setDefaultController);
PHP_METHOD(Test_Router, setDefaultAction);
PHP_METHOD(Test_Router, setDefaults);
PHP_METHOD(Test_Router, doRemoveExtraSlashes);
PHP_METHOD(Test_Router, handle);
PHP_METHOD(Test_Router, add);
PHP_METHOD(Test_Router, addGet);
PHP_METHOD(Test_Router, addPost);
PHP_METHOD(Test_Router, addPut);
PHP_METHOD(Test_Router, addPatch);
PHP_METHOD(Test_Router, addDelete);
PHP_METHOD(Test_Router, addOptions);
PHP_METHOD(Test_Router, addHead);
PHP_METHOD(Test_Router, mount);
PHP_METHOD(Test_Router, notFound);
PHP_METHOD(Test_Router, clear);
PHP_METHOD(Test_Router, getNamespaceName);
PHP_METHOD(Test_Router, getModuleName);
PHP_METHOD(Test_Router, getControllerName);
PHP_METHOD(Test_Router, getActionName);
PHP_METHOD(Test_Router, getParams);
PHP_METHOD(Test_Router, getMatchedRoute);
PHP_METHOD(Test_Router, getMatches);
PHP_METHOD(Test_Router, wasMatched);
PHP_METHOD(Test_Router, getRoutes);
PHP_METHOD(Test_Router, getRouteById);
PHP_METHOD(Test_Router, getRouteByName);

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_router___construct, 0, 0, 0)
	ZEND_ARG_INFO(0, defaultRoutes)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_router_setdi, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, dependencyInjector, Test\\DiInterface, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_router_seturisource, 0, 0, 1)
	ZEND_ARG_INFO(0, uriSource)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_router_removeextraslashes, 0, 0, 1)
	ZEND_ARG_INFO(0, remove)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_router_setdefaultnamespace, 0, 0, 1)
	ZEND_ARG_INFO(0, namespaceName)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_router_setdefaultmodule, 0, 0, 1)
	ZEND_ARG_INFO(0, moduleName)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_router_setdefaultcontroller, 0, 0, 1)
	ZEND_ARG_INFO(0, controllerName)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_router_setdefaultaction, 0, 0, 1)
	ZEND_ARG_INFO(0, actionName)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_router_setdefaults, 0, 0, 1)
	ZEND_ARG_INFO(0, defaults)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_router_doremoveextraslashes, 0, 0, 1)
	ZEND_ARG_INFO(0, route)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_router_handle, 0, 0, 0)
	ZEND_ARG_INFO(0, uri)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_router_add, 0, 0, 1)
	ZEND_ARG_INFO(0, pattern)
	ZEND_ARG_INFO(0, paths)
	ZEND_ARG_INFO(0, httpMethods)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_router_addget, 0, 0, 1)
	ZEND_ARG_INFO(0, pattern)
	ZEND_ARG_INFO(0, paths)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_router_addpost, 0, 0, 1)
	ZEND_ARG_INFO(0, pattern)
	ZEND_ARG_INFO(0, paths)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_router_addput, 0, 0, 1)
	ZEND_ARG_INFO(0, pattern)
	ZEND_ARG_INFO(0, paths)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_router_addpatch, 0, 0, 1)
	ZEND_ARG_INFO(0, pattern)
	ZEND_ARG_INFO(0, paths)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_router_adddelete, 0, 0, 1)
	ZEND_ARG_INFO(0, pattern)
	ZEND_ARG_INFO(0, paths)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_router_addoptions, 0, 0, 1)
	ZEND_ARG_INFO(0, pattern)
	ZEND_ARG_INFO(0, paths)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_router_addhead, 0, 0, 1)
	ZEND_ARG_INFO(0, pattern)
	ZEND_ARG_INFO(0, paths)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_router_mount, 0, 0, 1)
	ZEND_ARG_INFO(0, group)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_router_notfound, 0, 0, 1)
	ZEND_ARG_INFO(0, paths)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_router_getroutebyid, 0, 0, 1)
	ZEND_ARG_INFO(0, id)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_router_getroutebyname, 0, 0, 1)
	ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_router_method_entry) {
	PHP_ME(Test_Router, __construct, arginfo_test_router___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Test_Router, setDI, arginfo_test_router_setdi, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Router, getDI, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Router, getRewriteUri, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Router, setUriSource, arginfo_test_router_seturisource, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Router, removeExtraSlashes, arginfo_test_router_removeextraslashes, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Router, setDefaultNamespace, arginfo_test_router_setdefaultnamespace, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Router, setDefaultModule, arginfo_test_router_setdefaultmodule, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Router, setDefaultController, arginfo_test_router_setdefaultcontroller, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Router, setDefaultAction, arginfo_test_router_setdefaultaction, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Router, setDefaults, arginfo_test_router_setdefaults, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Router, doRemoveExtraSlashes, arginfo_test_router_doremoveextraslashes, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Router, handle, arginfo_test_router_handle, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Router, add, arginfo_test_router_add, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Router, addGet, arginfo_test_router_addget, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Router, addPost, arginfo_test_router_addpost, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Router, addPut, arginfo_test_router_addput, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Router, addPatch, arginfo_test_router_addpatch, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Router, addDelete, arginfo_test_router_adddelete, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Router, addOptions, arginfo_test_router_addoptions, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Router, addHead, arginfo_test_router_addhead, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Router, mount, arginfo_test_router_mount, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Router, notFound, arginfo_test_router_notfound, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Router, clear, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Router, getNamespaceName, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Router, getModuleName, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Router, getControllerName, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Router, getActionName, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Router, getParams, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Router, getMatchedRoute, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Router, getMatches, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Router, wasMatched, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Router, getRoutes, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Router, getRouteById, arginfo_test_router_getroutebyid, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Router, getRouteByName, arginfo_test_router_getroutebyname, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
