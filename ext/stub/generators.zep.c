
#ifdef HAVE_CONFIG_H
#include "../ext_config.h"
#endif

#include <php.h>
#include "../php_ext.h"
#include "../ext.h"

#include <Zend/zend_generators.h>
#include <Zend/zend_operators.h>
#include <Zend/zend_exceptions.h>
#include <Zend/zend_interfaces.h>

#include "kernel/main.h"
#include "kernel/object.h"

//static zend_object_handlers zend_generator_handlers;


ZEPHIR_INIT_CLASS(Stub_Generators)
{
	ZEPHIR_REGISTER_CLASS(Stub, Generators, stub, generators, stub_generators_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_Generators, simpleYield)
{
	//zend_ast *ast_val = zend_ast_create_zval_from_str(zend_string_init("test", sizeof("test") - 1, 0));
	zval vals;

	zephir_create_array(&vals, 5, 0);
	zephir_array_update_string(&vals, SL("server"), zend_string_init("test", sizeof("test") - 1, 0), PH_COPY | PH_SEPARATE);

	zend_generator *generator, *root;

	generator = emalloc(sizeof(zend_generator));
	memset(generator, 0, sizeof(zend_generator));

	/* The key will be incremented on first use, so it'll start at 0 */
	generator->largest_used_integer_key = -1;

	ZVAL_UNDEF(&generator->retval);
	ZVAL_UNDEF(&generator->values);

	/* By default we have a tree of only one node */
	generator->node.parent = NULL;
	generator->node.children = 0;
	generator->node.ptr.root = NULL;
	generator->values = vals;

	zend_object_std_init(&generator->std, zend_standard_class_def);
	//generator->std.handlers = &zend_generator_handlers;

	root = zend_generator_get_current(generator);
	RETURN_COPY_DEREF(root->values);
	//if (EXPECTED(generator->execute_data != NULL && Z_TYPE(root->key) != IS_UNDEF)) {
	//	RETURN_COPY_DEREF(&root->key);
	//}

	//RETURN_OBJ_COPY(generator);

	// CG(active_op_array)->fn_flags |= ZEND_ACC_GENERATOR;
	// zend_ast_create(ZEND_AST_YIELD, $2, NULL); CG(extra_fn_flags) |= ZEND_ACC_GENERATOR;

	//CG(active_op_array)->fn_flags |= ZEND_ACC_GENERATOR;

	//zend_emit_op(NULL, ZEND_YIELD, ast_val, NULL);
}

