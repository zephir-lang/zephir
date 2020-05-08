
extern zend_class_entry *stub_router_ce;

ZEPHIR_INIT_CLASS(Stub_Router);

PHP_METHOD(Stub_Router, __construct);
PHP_METHOD(Stub_Router, setDI);
PHP_METHOD(Stub_Router, getDI);
PHP_METHOD(Stub_Router, getRewriteUri);
PHP_METHOD(Stub_Router, setUriSource);
PHP_METHOD(Stub_Router, removeExtraSlashes);
PHP_METHOD(Stub_Router, setDefaultNamespace);
PHP_METHOD(Stub_Router, setDefaultModule);
PHP_METHOD(Stub_Router, setDefaultController);
PHP_METHOD(Stub_Router, setDefaultAction);
PHP_METHOD(Stub_Router, setDefaults);
PHP_METHOD(Stub_Router, doRemoveExtraSlashes);
PHP_METHOD(Stub_Router, handle);
PHP_METHOD(Stub_Router, add);
PHP_METHOD(Stub_Router, addGet);
PHP_METHOD(Stub_Router, addPost);
PHP_METHOD(Stub_Router, addPut);
PHP_METHOD(Stub_Router, addPatch);
PHP_METHOD(Stub_Router, addDelete);
PHP_METHOD(Stub_Router, addOptions);
PHP_METHOD(Stub_Router, addHead);
PHP_METHOD(Stub_Router, mount);
PHP_METHOD(Stub_Router, notFound);
PHP_METHOD(Stub_Router, clear);
PHP_METHOD(Stub_Router, getNamespaceName);
PHP_METHOD(Stub_Router, getModuleName);
PHP_METHOD(Stub_Router, getControllerName);
PHP_METHOD(Stub_Router, getActionName);
PHP_METHOD(Stub_Router, getParams);
PHP_METHOD(Stub_Router, getMatchedRoute);
PHP_METHOD(Stub_Router, getMatches);
PHP_METHOD(Stub_Router, wasMatched);
PHP_METHOD(Stub_Router, getRoutes);
PHP_METHOD(Stub_Router, getRouteById);
PHP_METHOD(Stub_Router, getRouteByName);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_router___construct, 0, 0, 0)
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, defaultRoutes, _IS_BOOL, 0)
#else
	ZEND_ARG_INFO(0, defaultRoutes)
#endif
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_router_setdi, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, dependencyInjector, Stub\\DiInterface, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_router_seturisource, 0, 0, 1)
	ZEND_ARG_INFO(0, uriSource)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_router_removeextraslashes, 0, 0, 1)
	ZEND_ARG_INFO(0, remove)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_router_setdefaultnamespace, 0, 0, 1)
	ZEND_ARG_INFO(0, namespaceName)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_router_setdefaultmodule, 0, 0, 1)
	ZEND_ARG_INFO(0, moduleName)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_router_setdefaultcontroller, 0, 0, 1)
	ZEND_ARG_INFO(0, controllerName)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_router_setdefaultaction, 0, 0, 1)
	ZEND_ARG_INFO(0, actionName)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_router_setdefaults, 0, 0, 1)
	ZEND_ARG_INFO(0, defaults)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_router_doremoveextraslashes, 0, 0, 1)
	ZEND_ARG_INFO(0, route)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_router_handle, 0, 0, 0)
	ZEND_ARG_INFO(0, uri)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_router_add, 0, 0, 1)
	ZEND_ARG_INFO(0, pattern)
	ZEND_ARG_INFO(0, paths)
	ZEND_ARG_INFO(0, httpMethods)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_router_addget, 0, 0, 1)
	ZEND_ARG_INFO(0, pattern)
	ZEND_ARG_INFO(0, paths)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_router_addpost, 0, 0, 1)
	ZEND_ARG_INFO(0, pattern)
	ZEND_ARG_INFO(0, paths)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_router_addput, 0, 0, 1)
	ZEND_ARG_INFO(0, pattern)
	ZEND_ARG_INFO(0, paths)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_router_addpatch, 0, 0, 1)
	ZEND_ARG_INFO(0, pattern)
	ZEND_ARG_INFO(0, paths)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_router_adddelete, 0, 0, 1)
	ZEND_ARG_INFO(0, pattern)
	ZEND_ARG_INFO(0, paths)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_router_addoptions, 0, 0, 1)
	ZEND_ARG_INFO(0, pattern)
	ZEND_ARG_INFO(0, paths)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_router_addhead, 0, 0, 1)
	ZEND_ARG_INFO(0, pattern)
	ZEND_ARG_INFO(0, paths)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_router_mount, 0, 0, 1)
	ZEND_ARG_INFO(0, group)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_router_notfound, 0, 0, 1)
	ZEND_ARG_INFO(0, paths)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_router_getroutebyid, 0, 0, 1)
	ZEND_ARG_INFO(0, id)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_router_getroutebyname, 0, 0, 1)
	ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_router_method_entry) {
	PHP_ME(Stub_Router, __construct, arginfo_stub_router___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Stub_Router, setDI, arginfo_stub_router_setdi, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Router, getDI, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Router, getRewriteUri, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Router, setUriSource, arginfo_stub_router_seturisource, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Router, removeExtraSlashes, arginfo_stub_router_removeextraslashes, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Router, setDefaultNamespace, arginfo_stub_router_setdefaultnamespace, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Router, setDefaultModule, arginfo_stub_router_setdefaultmodule, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Router, setDefaultController, arginfo_stub_router_setdefaultcontroller, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Router, setDefaultAction, arginfo_stub_router_setdefaultaction, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Router, setDefaults, arginfo_stub_router_setdefaults, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Router, doRemoveExtraSlashes, arginfo_stub_router_doremoveextraslashes, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Router, handle, arginfo_stub_router_handle, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Router, add, arginfo_stub_router_add, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Router, addGet, arginfo_stub_router_addget, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Router, addPost, arginfo_stub_router_addpost, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Router, addPut, arginfo_stub_router_addput, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Router, addPatch, arginfo_stub_router_addpatch, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Router, addDelete, arginfo_stub_router_adddelete, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Router, addOptions, arginfo_stub_router_addoptions, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Router, addHead, arginfo_stub_router_addhead, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Router, mount, arginfo_stub_router_mount, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Router, notFound, arginfo_stub_router_notfound, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Router, clear, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Router, getNamespaceName, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Router, getModuleName, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Router, getControllerName, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Router, getActionName, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Router, getParams, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Router, getMatchedRoute, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Router, getMatches, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Router, wasMatched, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Router, getRoutes, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Router, getRouteById, arginfo_stub_router_getroutebyid, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Router, getRouteByName, arginfo_stub_router_getroutebyname, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
