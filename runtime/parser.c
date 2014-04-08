/** The author disclaims copyright to this source code.
*/
/* First off, code is include which follows the "include" declaration
** in the input file. */
#include <stdio.h>
// 54 "parser.lemon"


#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <php.h>
#include "php_zephir.h"
#include "scanner.h"
#include "zephir.h"
#include "parser.h"

#define zephir_add_active_file(ret, state) { \
	zval *tmp; \
	ALLOC_INIT_ZVAL(tmp); \
	ZVAL_STRING(tmp, state->active_file, 1); \
	zend_symtable_update(Z_ARRVAL_P(ret), "file", strlen("file")+1, &tmp, sizeof(zval*), NULL); \
}

#define zephir_add_active_line(ret, state) { \
	zval *tmp; \
	ALLOC_INIT_ZVAL(tmp); \
	ZVAL_LONG(tmp, state->active_line); \
	zend_symtable_update(Z_ARRVAL_P(ret), "line", strlen("line")+1, &tmp, sizeof(zval*), NULL); \
}

#define zephir_add_active_char(ret, state) { \
	zval *tmp; \
	ALLOC_INIT_ZVAL(tmp); \
	ZVAL_LONG(tmp, state->active_char); \
	zend_symtable_update(Z_ARRVAL_P(ret), "char", strlen("char")+1, &tmp, sizeof(zval*), NULL); \
}

static zval *zephir_new_array()
{
	zval *z;
	ALLOC_INIT_ZVAL(z);
	array_init(z);
	return z;
}

static zval *zephir_new_string(const char *str) {
	zval *tmp;
	ALLOC_INIT_ZVAL(tmp);
	ZVAL_STRING(tmp, str, 1);
	return tmp;
}

static zval *zephir_new_int(long i) {
	zval *tmp;
	ALLOC_INIT_ZVAL(tmp);
	ZVAL_LONG(tmp, i);
	return tmp;
}

static void zephir_array_add(zval *ret, const char *index, zval *value) {
	zend_symtable_update(Z_ARRVAL_P(ret), index, strlen(index)+1, &value, sizeof(zval*), NULL);
}

static zval *zephir_ret_literal(int type, zephir_parser_token *T, zephir_scanner_state *state)
{
	zval *ret = zephir_new_array();

	switch (type) {

		case ZEPHIR_T_CONSTANT:
			zephir_array_add(ret, "type", zephir_new_string("constant"));
			break;

		case ZEPHIR_T_IDENTIFIER:
			zephir_array_add(ret, "type", zephir_new_string("variable"));
			break;

		case ZEPHIR_T_INTEGER:
			zephir_array_add(ret, "type", zephir_new_string("int"));
			break;

		case ZEPHIR_T_DOUBLE:
			zephir_array_add(ret, "type", zephir_new_string("double"));
			break;

		case ZEPHIR_T_NULL:
			zephir_array_add(ret, "type", zephir_new_string("null"));
			break;

		case ZEPHIR_T_STRING:
			zephir_array_add(ret, "type", zephir_new_string("string"));
			break;

		case ZEPHIR_T_CHAR:
			zephir_array_add(ret, "type", zephir_new_string("char"));
			break;

		default:
			if (type == ZEPHIR_T_TRUE) {
				zephir_array_add(ret, "type", zephir_new_string("bool"));
				zephir_array_add(ret, "value", zephir_new_string("true"));
			} else {
				if (type == ZEPHIR_T_FALSE) {
					zephir_array_add(ret, "type", zephir_new_string("bool"));
					zephir_array_add(ret, "value", zephir_new_string("false"));
				} else {
					fprintf(stderr, "literal??\n");
				}
			}
	}

	if (T) {
		zephir_array_add(ret, "value", zephir_new_string(T->token));
		efree(T->token);
		efree(T);
	}

	zephir_add_active_file(ret, state);
	zephir_add_active_line(ret, state);
	zephir_add_active_char(ret, state);

	return ret;
}

static zval *zephir_ret_expr(char *type, zval *left, zval *right, zval *extra, zephir_scanner_state *state)
{
	zval *ret = zephir_new_array();

	zephir_array_add(ret, "type", zephir_new_string(type));

	if (left) {
		zephir_array_add(ret, "left", left);
	}
	if (right) {
		zephir_array_add(ret, "right", right);
	}
	if (extra) {
		zephir_array_add(ret, "extra", extra);
	}

	zephir_add_active_file(ret, state);
	zephir_add_active_line(ret, state);
	zephir_add_active_char(ret, state);

	return ret;
}

static zval *zephir_ret_array_item(zval *key, zval *value, zephir_scanner_state *state)
{
	zval *ret = zephir_new_array();

	if (key) {
		zephir_array_add(ret, "key", key);
	}
	zephir_array_add(ret, "value", value);

	zephir_add_active_file(ret, state);
	zephir_add_active_line(ret, state);
	zephir_add_active_char(ret, state);

	return ret;
}

static zval *zephir_ret_namespace(zephir_parser_token *T, zephir_scanner_state *state)
{
	zval *ret = zephir_new_array();

	zephir_array_add(ret, "type", zephir_new_string("namespace"));
	zephir_array_add(ret, "name", zephir_new_string(T->token));

	zephir_add_active_file(ret, state);
	zephir_add_active_line(ret, state);
	zephir_add_active_char(ret, state);

	efree(T->token);
	efree(T);

	return ret;
}

static zval *zephir_ret_use_aliases(zval *use_aliases_list, zephir_scanner_state *state)
{
	zval *ret = zephir_new_array();

	zephir_array_add(ret, "type", zephir_new_string("use"));
	zephir_array_add(ret, "aliases", use_aliases_list);

	zephir_add_active_file(ret, state);
	zephir_add_active_line(ret, state);
	zephir_add_active_char(ret, state);

	return ret;
}

static zval *zephir_ret_use_aliases_item(zephir_parser_token *T, zephir_parser_token *A, zephir_scanner_state *state)
{
	zval *ret = zephir_new_array();

	zephir_array_add(ret, "name", zephir_new_string(T->token));
	if (A) {
		zephir_array_add(ret, "alias", zephir_new_string(A->token));
	}

	zephir_add_active_file(ret, state);
	zephir_add_active_line(ret, state);
	zephir_add_active_char(ret, state);

	efree(T->token);
	efree(T);

	return ret;
}

static zval *zephir_ret_class(zephir_parser_token *T, zval *class_definition, int is_abstract, int is_final,
	zephir_parser_token *E, zval *I, zephir_scanner_state *state)
{
	zval *ret = zephir_new_array();

	zephir_array_add(ret, "type", zephir_new_string("class"));
	zephir_array_add(ret, "name", zephir_new_string(T->token));

	zephir_array_add(ret, "abstract", zephir_new_int(is_abstract));
	zephir_array_add(ret, "final", zephir_new_int(is_final));

	if (E) {
		zephir_array_add(ret, "extends", zephir_new_string(E->token));
		efree(E->token);
		efree(E);
	}

	if (I) {
		zephir_array_add(ret, "implements", I);
	}

	if (class_definition) {
		zephir_array_add(ret, "definition", class_definition);
	}

	zephir_add_active_file(ret, state);
	zephir_add_active_line(ret, state);
	zephir_add_active_char(ret, state);

	efree(T->token);
	efree(T);

	return ret;
}

static zval *zephir_ret_interface(zephir_parser_token *T, zval *interface_definition, zephir_parser_token *E, zephir_scanner_state *state)
{
	zval *ret = zephir_new_array();

	zephir_array_add(ret, "type", zephir_new_string("interface"));
	zephir_array_add(ret, "name", zephir_new_string(T->token));

	if (E) {
    	zephir_array_add(ret, "extends", zephir_new_string(E->token));
	}

	if (interface_definition) {
		zephir_array_add(ret, "definition", interface_definition);
	}

	zephir_add_active_file(ret, state);
	zephir_add_active_line(ret, state);
	zephir_add_active_char(ret, state);

	efree(T->token);
	efree(T);

	return ret;
}

static zval *zephir_ret_class_definition(zval *properties, zval *methods, zval *constants, zephir_scanner_state *state)
{
	zval *ret = zephir_new_array();

	if (properties) {
		zephir_array_add(ret, "properties", properties);
	}
	if (methods) {
		zephir_array_add(ret, "methods", methods);
	}
	if (constants) {
		zephir_array_add(ret, "constants", constants);
	}

	zephir_add_active_file(ret, state);
	zephir_add_active_line(ret, state);
	zephir_add_active_char(ret, state);

	return ret;
}

static zval *zephir_ret_interface_definition(zval *methods, zval *constants, zephir_scanner_state *state)
{
	zval *ret = zephir_new_array();

	if (methods) {
		zephir_array_add(ret, "methods", methods);
	}
	if (constants) {
		zephir_array_add(ret, "constants", constants);
	}

	zephir_add_active_file(ret, state);
	zephir_add_active_line(ret, state);
	zephir_add_active_char(ret, state);

	return ret;
}

static zval *zephir_ret_class_property(zval *visibility, zephir_parser_token *T,
		zval *default_value, zephir_parser_token *D, zval *shortcuts, zephir_scanner_state *state)
{
	zval *ret = zephir_new_array();

	zephir_array_add(ret, "visibility", visibility);
	zephir_array_add(ret, "type", zephir_new_string("property"));
	zephir_array_add(ret, "name", zephir_new_string(T->token));

	if (default_value) {
		zephir_array_add(ret, "default", default_value);
	}

	if (D) {
		zephir_array_add(ret, "docblock", zephir_new_string(D->token));
		efree(D->token);
		efree(D);
	}

	if (shortcuts) {
		zephir_array_add(ret, "shortcuts", shortcuts);
	}

	zephir_add_active_file(ret, state);
	zephir_add_active_line(ret, state);
	zephir_add_active_char(ret, state);

	efree(T->token);
	efree(T);

	return ret;
}

static zval *zephir_ret_property_shortcut(zephir_parser_token *C, zephir_parser_token *D, zephir_scanner_state *state)
{
	zval *ret = zephir_new_array();

	zephir_array_add(ret, "type", zephir_new_string("shortcut"));
	if (C) {
		zephir_array_add(ret, "docblock", zephir_new_string(C->token));
	}
	zephir_array_add(ret, "name", zephir_new_string(D->token));

	zephir_add_active_file(ret, state);
	zephir_add_active_line(ret, state);
	zephir_add_active_char(ret, state);

	efree(D->token);
	D->free_flag = 0;

	return ret;
}

static zval *zephir_ret_class_const(zephir_parser_token *T, zval *default_value, zephir_parser_token *D, zephir_scanner_state *state)
{
	zval *ret = zephir_new_array();

	zephir_array_add(ret, "type", zephir_new_string("const"));
	zephir_array_add(ret, "name", zephir_new_string(T->token));
	zephir_array_add(ret, "default", default_value);

	if (D) {
		zephir_array_add(ret, "docblock", zephir_new_string(D->token));
	}

	zephir_add_active_file(ret, state);
	zephir_add_active_line(ret, state);
	zephir_add_active_char(ret, state);

	efree(T->token);
	efree(T);

	return ret;
}

static zval *zephir_ret_class_method(zval *visibility, zephir_parser_token *T, zval *parameters,
	zval *statements, zephir_parser_token *D, zval *return_type, zephir_scanner_state *state)
{
	zval *ret = zephir_new_array();

	zephir_array_add(ret, "visibility", visibility);
	zephir_array_add(ret, "type", zephir_new_string("method"));
	zephir_array_add(ret, "name", zephir_new_string(T->token));

	if (parameters) {
		zephir_array_add(ret, "parameters", parameters);
	}

	if (statements) {
		zephir_array_add(ret, "statements", statements);
	}

	if (D) {
		zephir_array_add(ret, "docblock", zephir_new_string(D->token));
		efree(D->token);
		efree(D);
	}

	if (return_type) {
		zephir_array_add(ret, "return-type", return_type);
	}

	zephir_add_active_file(ret, state);
	zephir_add_active_line(ret, state);
	zephir_add_active_char(ret, state);

	efree(T->token);
	efree(T);

	return ret;
}

static zval *zephir_ret_parameter(int const_param, zval *type, zval *cast, zephir_parser_token *N, zval *default_value,
	int mandatory, zephir_scanner_state *state)
{
	zval *ret = zephir_new_array();

	zephir_array_add(ret, "type", zephir_new_string("parameter"));
	zephir_array_add(ret, "name", zephir_new_string(N->token));
	zephir_array_add(ret, "const", zephir_new_int(const_param));

	if (type) {
		zephir_array_add(ret, "data-type", type);
		zephir_array_add(ret, "mandatory", zephir_new_int(mandatory));
	}
	if (cast) {
		zephir_array_add(ret, "cast", cast);
	}
	if (default_value) {
		zephir_array_add(ret, "default", default_value);
	}

	zephir_add_active_file(ret, state);
	zephir_add_active_line(ret, state);
	zephir_add_active_char(ret, state);

	efree(N->token);
	efree(N);

	return ret;
}

static zval *zephir_ret_return_type(int is_void, zval *return_type_list, zephir_scanner_state *state)
{
	zval *ret = zephir_new_array();

	zephir_array_add(ret, "type", zephir_new_string("return-type"));

	zephir_array_add(ret, "list", return_type_list);

	zephir_array_add(ret, "void", zephir_new_int(is_void));

	zephir_add_active_file(ret, state);
	zephir_add_active_line(ret, state);
	zephir_add_active_char(ret, state);

	return ret;
}

static zval *zephir_ret_return_type_item(zval *type, zval *cast, int mandatory, int collection, zephir_scanner_state *state)
{
	zval *ret = zephir_new_array();

	zephir_array_add(ret, "type", zephir_new_string("return-type-parameter"));

	if (type) {
		zephir_array_add(ret, "data-type", type);
		zephir_array_add(ret, "mandatory", zephir_new_int(mandatory));
	}

	if (cast) {
		zephir_array_add(ret, "cast", cast);
		zephir_array_add(ret, "collection", zephir_new_int(collection));
	}

	zephir_add_active_file(ret, state);
	zephir_add_active_line(ret, state);
	zephir_add_active_char(ret, state);

	return ret;
}

static zval *zephir_ret_type(int type)
{
	switch (type) {
		case ZEPHIR_TYPE_INTEGER:
			return zephir_new_string("int");
		case ZEPHIR_TYPE_UINTEGER:
			return zephir_new_string("uint");
		case ZEPHIR_TYPE_DOUBLE:
			return zephir_new_string("double");
		case ZEPHIR_TYPE_BOOL:
			return zephir_new_string("bool");
		case ZEPHIR_TYPE_LONG:
			return zephir_new_string("long");
		case ZEPHIR_TYPE_ULONG:
			return zephir_new_string("ulong");
		case ZEPHIR_TYPE_STRING:
			return zephir_new_string("string");
		case ZEPHIR_TYPE_CHAR:
			return zephir_new_string("char");
		case ZEPHIR_TYPE_ARRAY:
			return zephir_new_string("array");
		case ZEPHIR_TYPE_VAR:
			return zephir_new_string("variable");
        case ZEPHIR_TYPE_CALLABLE:
            return zephir_new_string("callable");
        case ZEPHIR_TYPE_RESOURCE:
            return zephir_new_string("resource");
        case ZEPHIR_TYPE_OBJECT:
            return zephir_new_string("object");
		case ZEPHIR_T_TYPE_NULL:
            return zephir_new_string("null");
        case ZEPHIR_T_TYPE_THIS:
            return zephir_new_string("this");
		default:
			fprintf(stderr, "unknown type?\n");
	}

}

static zval *zephir_ret_list(zval *list_left, zval *right_list)
{
	zval *ret;
	HashPosition pos;
	HashTable *list;

	MAKE_STD_ZVAL(ret);
	array_init(ret);

	if (list_left) {

		list = Z_ARRVAL_P(list_left);
		if (zend_hash_index_exists(list, 0)) {
			zend_hash_internal_pointer_reset_ex(list, &pos);
			for (;; zend_hash_move_forward_ex(list, &pos)) {

				zval ** item;

				if (zend_hash_get_current_data_ex(list, (void**) &item, &pos) == FAILURE) {
					break;
				}

				Z_ADDREF_PP(item);
				add_next_index_zval(ret, *item);

			}
			zval_ptr_dtor(&list_left);
		} else {
			add_next_index_zval(ret, list_left);
		}
	}

	add_next_index_zval(ret, right_list);

	return ret;
}

static zval *zephir_ret_let_statement(zval *assignments, zephir_scanner_state *state)
{
	zval *ret = zephir_new_array();

	zephir_array_add(ret, "type", zephir_new_string("let"));
	zephir_array_add(ret, "assignments", assignments);

	zephir_add_active_file(ret, state);
	zephir_add_active_line(ret, state);
	zephir_add_active_char(ret, state);

	return ret;
}

static zval *zephir_ret_let_assignment(char *type, zval *operator, zephir_parser_token *V, zephir_parser_token *P, zval *index_expr, zval *expr, zephir_scanner_state *state)
{
	zval *ret = zephir_new_array();

	zephir_array_add(ret, "assign-type", zephir_new_string(type));
	if (operator) {
		zephir_array_add(ret, "operator", operator);
	}
	zephir_array_add(ret, "variable", zephir_new_string(V->token));
	if (P) {
		zephir_array_add(ret, "property", zephir_new_string(P->token));
	}
	if (index_expr) {
		zephir_array_add(ret, "index-expr", index_expr);
	}
	if (expr) {
		zephir_array_add(ret, "expr", expr);
	}

	zephir_add_active_file(ret, state);
	zephir_add_active_line(ret, state);
	zephir_add_active_char(ret, state);

	return ret;
}

static zval *zephir_ret_if_statement(zval *expr, zval *statements, zval *else_statements, zephir_scanner_state *state)
{
	zval *ret = zephir_new_array();

	zephir_array_add(ret, "type", zephir_new_string("if"));
	zephir_array_add(ret, "expr", expr);

	if (statements) {
		zephir_array_add(ret, "statements", statements);
	}

	if (else_statements) {
		zephir_array_add(ret, "else_statements", else_statements);
	}

	zephir_add_active_file(ret, state);
	zephir_add_active_line(ret, state);
	zephir_add_active_char(ret, state);

	return ret;
}

static zval *zephir_ret_switch_statement(zval *expr, zval *clauses, zephir_scanner_state *state)
{
	zval *ret = zephir_new_array();

	zephir_array_add(ret, "type", zephir_new_string("switch"));
	zephir_array_add(ret, "expr", expr);

	if (clauses) {
		zephir_array_add(ret, "clauses", clauses);
	}

	zephir_add_active_file(ret, state);
	zephir_add_active_line(ret, state);
	zephir_add_active_char(ret, state);

	return ret;
}

static zval *zephir_ret_case_clause(zval *expr, zval *statements, zephir_scanner_state *state)
{
	zval *ret = zephir_new_array();

	if (expr) {
		zephir_array_add(ret, "type", zephir_new_string("case"));
		zephir_array_add(ret, "expr", expr);
	} else {
		zephir_array_add(ret, "type", zephir_new_string("default"));
	}

	if (statements) {
		zephir_array_add(ret, "statements", statements);
	}

	zephir_add_active_file(ret, state);
	zephir_add_active_line(ret, state);
	zephir_add_active_char(ret, state);

	return ret;
}

static zval *zephir_ret_while_statement(zval *expr, zval *statements, zephir_scanner_state *state)
{
	zval *ret = zephir_new_array();

	zephir_array_add(ret, "type", zephir_new_string("while"));
	zephir_array_add(ret, "expr", expr);

	if (statements) {
		zephir_array_add(ret, "statements", statements);
	}

	zephir_add_active_file(ret, state);
	zephir_add_active_line(ret, state);
	zephir_add_active_char(ret, state);

	return ret;
}

static zval *zephir_ret_do_while_statement(zval *expr, zval *statements, zephir_scanner_state *state)
{
	zval *ret = zephir_new_array();

	zephir_array_add(ret, "type", zephir_new_string("do-while"));
	zephir_array_add(ret, "expr", expr);

	if (statements) {
		zephir_array_add(ret, "statements", statements);
	}

	zephir_add_active_file(ret, state);
	zephir_add_active_line(ret, state);
	zephir_add_active_char(ret, state);

	return ret;
}

static zval *zephir_ret_try_catch_statement(zval *statements, zval *catches, zephir_scanner_state *state)
{
	zval *ret = zephir_new_array();

	zephir_array_add(ret, "type", zephir_new_string("try-catch"));

	if (statements) {
		zephir_array_add(ret, "statements", statements);
	}
	if (catches) {
		zephir_array_add(ret, "catches", catches);
	}

	zephir_add_active_file(ret, state);
	zephir_add_active_line(ret, state);
	zephir_add_active_char(ret, state);

	return ret;
}

static zval *zephir_ret_catch_statement(zval *classes, zval *variable, zval *statements, zephir_scanner_state *state)
{
	zval *ret = zephir_new_array();

	if (classes) {
		zephir_array_add(ret, "classes", classes);
	}

	if (variable) {
		zephir_array_add(ret, "variable", variable);
	}

	if (statements) {
		zephir_array_add(ret, "statements", statements);
	}

	zephir_add_active_file(ret, state);
	zephir_add_active_line(ret, state);
	zephir_add_active_char(ret, state);

	return ret;
}

static zval *zephir_ret_for_statement(zval *expr, zephir_parser_token *K, zephir_parser_token *V, int reverse, zval *statements, zephir_scanner_state *state)
{
	zval *ret = zephir_new_array();

	zephir_array_add(ret, "type", zephir_new_string("for"));
	zephir_array_add(ret, "expr", expr);

	if (K) {
		zephir_array_add(ret, "key", zephir_new_string(K->token));
	}
	if (V) {
		zephir_array_add(ret, "value", zephir_new_string(V->token));
	}

	zephir_array_add(ret, "reverse", zephir_new_int(reverse));

	if (statements) {
		zephir_array_add(ret, "statements", statements);
	}

	zephir_add_active_file(ret, state);
	zephir_add_active_line(ret, state);
	zephir_add_active_char(ret, state);

	return ret;
}

static zval *zephir_ret_loop_statement(zval *statements, zephir_scanner_state *state)
{
	zval *ret = zephir_new_array();

	zephir_array_add(ret, "type", zephir_new_string("loop"));

	if (statements) {
		zephir_array_add(ret, "statements", statements);
	}

	zephir_add_active_file(ret, state);
	zephir_add_active_line(ret, state);
	zephir_add_active_char(ret, state);

	return ret;
}

static zval *zephir_ret_empty_statement(zephir_scanner_state *state)
{
	zval *ret = zephir_new_array();

	zephir_array_add(ret, "type", zephir_new_string("empty"));

	zephir_add_active_file(ret, state);
	zephir_add_active_line(ret, state);
	zephir_add_active_char(ret, state);

	return ret;
}

static zval *zephir_ret_break_statement(zephir_scanner_state *state)
{
	zval *ret = zephir_new_array();

	zephir_array_add(ret, "type", zephir_new_string("break"));

	zephir_add_active_file(ret, state);
	zephir_add_active_line(ret, state);
	zephir_add_active_char(ret, state);

	return ret;
}

static zval *zephir_ret_continue_statement(zephir_scanner_state *state)
{
	zval *ret = zephir_new_array();

	zephir_array_add(ret, "type", zephir_new_string("continue"));

	zephir_add_active_file(ret, state);
	zephir_add_active_line(ret, state);
	zephir_add_active_char(ret, state);

	return ret;
}

static zval *zephir_ret_echo_statement(zval *expressions, zephir_scanner_state *state)
{
	zval *ret = zephir_new_array();

	zephir_array_add(ret, "type", zephir_new_string("echo"));
	zephir_array_add(ret, "expressions", expressions);

	zephir_add_active_file(ret, state);
	zephir_add_active_line(ret, state);
	zephir_add_active_char(ret, state);

	return ret;
}

static zval *zephir_ret_return_statement(zval *expr, zephir_scanner_state *state)
{
	zval *ret = zephir_new_array();

	zephir_array_add(ret, "type", zephir_new_string("return"));
	if (expr) {
		zephir_array_add(ret, "expr", expr);
	}

	zephir_add_active_file(ret, state);
	zephir_add_active_line(ret, state);
	zephir_add_active_char(ret, state);

	return ret;
}

static zval *zephir_ret_require_statement(zval *expr, zephir_scanner_state *state)
{
	zval *ret = zephir_new_array();

	zephir_array_add(ret, "type", zephir_new_string("require"));
	zephir_array_add(ret, "expr", expr);

	zephir_add_active_file(ret, state);
	zephir_add_active_line(ret, state);
	zephir_add_active_char(ret, state);

	return ret;
}

static zval *zephir_ret_fetch_statement(zval *expr, zephir_scanner_state *state)
{
	zval *ret = zephir_new_array();

	zephir_array_add(ret, "type", zephir_new_string("fetch"));
	zephir_array_add(ret, "expr", expr);

	zephir_add_active_file(ret, state);
	zephir_add_active_line(ret, state);
	zephir_add_active_char(ret, state);

	return ret;
}

static zval *zephir_ret_fcall_statement(zval *expr, zephir_scanner_state *state)
{
	zval *ret = zephir_new_array();

	zephir_array_add(ret, "type", zephir_new_string("fcall"));
	zephir_array_add(ret, "expr", expr);

	zephir_add_active_file(ret, state);
	zephir_add_active_line(ret, state);
	zephir_add_active_char(ret, state);

	return ret;
}

static zval *zephir_ret_mcall_statement(zval *expr, zephir_scanner_state *state)
{
	zval *ret = zephir_new_array();

	zephir_array_add(ret, "type", zephir_new_string("mcall"));
	zephir_array_add(ret, "expr", expr);

	zephir_add_active_file(ret, state);
	zephir_add_active_line(ret, state);
	zephir_add_active_char(ret, state);

	return ret;
}

static zval *zephir_ret_scall_statement(zval *expr, zephir_scanner_state *state)
{
	zval *ret = zephir_new_array();

	zephir_array_add(ret, "type", zephir_new_string("scall"));
	zephir_array_add(ret, "expr", expr);

	zephir_add_active_file(ret, state);
	zephir_add_active_line(ret, state);
	zephir_add_active_char(ret, state);

	return ret;
}

static zval *zephir_ret_unset_statement(zval *expr, zephir_scanner_state *state)
{
	zval *ret = zephir_new_array();

	zephir_array_add(ret, "type", zephir_new_string("unset"));

	zephir_array_add(ret, "expr", expr);

	zephir_add_active_file(ret, state);
	zephir_add_active_line(ret, state);
	zephir_add_active_char(ret, state);

	return ret;
}

static zval *zephir_ret_declare_statement(int type, zval *variables, zephir_scanner_state *state)
{
	zval *ret = zephir_new_array();

	zephir_array_add(ret, "type", zephir_new_string("declare"));

	switch (type) {

		case ZEPHIR_T_TYPE_INTEGER:
			zephir_array_add(ret, "data-type", zephir_new_string("int"));
			break;

		case ZEPHIR_T_TYPE_UINTEGER:
			zephir_array_add(ret, "data-type", zephir_new_string("uint"));
			break;

		case ZEPHIR_T_TYPE_LONG:
			zephir_array_add(ret, "data-type", zephir_new_string("long"));
			break;

		case ZEPHIR_T_TYPE_ULONG:
			zephir_array_add(ret, "data-type", zephir_new_string("ulong"));
			break;

		case ZEPHIR_T_TYPE_CHAR:
			zephir_array_add(ret, "data-type", zephir_new_string("char"));
			break;

		case ZEPHIR_T_TYPE_UCHAR:
			zephir_array_add(ret, "data-type", zephir_new_string("uchar"));
			break;

		case ZEPHIR_T_TYPE_DOUBLE:
			zephir_array_add(ret, "data-type", zephir_new_string("double"));
			break;

		case ZEPHIR_T_TYPE_BOOL:
			zephir_array_add(ret, "data-type", zephir_new_string("bool"));
			break;

		case ZEPHIR_T_TYPE_STRING:
			zephir_array_add(ret, "data-type", zephir_new_string("string"));
			break;

		case ZEPHIR_T_TYPE_ARRAY:
			zephir_array_add(ret, "data-type", zephir_new_string("array"));
			break;

		case ZEPHIR_T_TYPE_VAR:
			zephir_array_add(ret, "data-type", zephir_new_string("variable"));
			break;

	    case ZEPHIR_T_TYPE_CALLABLE:
            zephir_array_add(ret, "data-type", zephir_new_string("callable"));
            break;

        case ZEPHIR_T_TYPE_RESOURCE:
            zephir_array_add(ret, "data-type", zephir_new_string("resource"));
            break;

        case ZEPHIR_T_TYPE_OBJECT:
            zephir_array_add(ret, "data-type", zephir_new_string("object"));
            break;

		default:
			fprintf(stderr, "err 2?\n");
	}

	zephir_array_add(ret, "variables", variables);

	zephir_add_active_file(ret, state);
	zephir_add_active_line(ret, state);
	zephir_add_active_char(ret, state);

	return ret;
}

static zval *zephir_ret_declare_variable(zephir_parser_token *T, zval *expr, zephir_scanner_state *state)
{
	zval *ret = zephir_new_array();

	zephir_array_add(ret, "variable", zephir_new_string(T->token));
	if (expr) {
		zephir_array_add(ret, "expr", expr);
	}

	zephir_add_active_file(ret, state);
	zephir_add_active_line(ret, state);
	zephir_add_active_char(ret, state);

	efree(T->token);
	efree(T);

	return ret;
}

static zval *zephir_ret_new_instance(int dynamic, zephir_parser_token *T, zval *parameters, zephir_scanner_state *state)
{
	zval *ret = zephir_new_array();

	zephir_array_add(ret, "type", zephir_new_string("new"));
	zephir_array_add(ret, "class", zephir_new_string(T->token));
	zephir_array_add(ret, "dynamic", zephir_new_int(dynamic));

	if (parameters) {
		zephir_array_add(ret, "parameters", parameters);
	}

	zephir_add_active_file(ret, state);
	zephir_add_active_line(ret, state);
	zephir_add_active_char(ret, state);

	efree(T->token);
	efree(T);

	return ret;
}

static zval *zephir_ret_throw_exception(zval *expr, zephir_scanner_state *state)
{
	zval *ret = zephir_new_array();

	zephir_array_add(ret, "type", zephir_new_string("throw"));
	if (expr) {
		zephir_array_add(ret, "expr", expr);
	}

	zephir_add_active_file(ret, state);
	zephir_add_active_line(ret, state);
	zephir_add_active_char(ret, state);

	return ret;
}

static zval *zephir_ret_fcall(int type, zephir_parser_token *F, zval *parameters, zephir_scanner_state *state)
{
	zval *ret = zephir_new_array();

	zephir_array_add(ret, "type", zephir_new_string("fcall"));
	zephir_array_add(ret, "name", zephir_new_string(F->token));
	zephir_array_add(ret, "call-type", zephir_new_int(type));

	if (parameters) {
		zephir_array_add(ret, "parameters", parameters);
	}

	zephir_add_active_file(ret, state);
	zephir_add_active_line(ret, state);
	zephir_add_active_char(ret, state);

	return ret;
}

static zval *zephir_ret_mcall(int type, zval *O, zephir_parser_token *M, zval *parameters, zephir_scanner_state *state)
{
	zval *ret = zephir_new_array();

	zephir_array_add(ret, "type", zephir_new_string("mcall"));
	zephir_array_add(ret, "variable", O);
	zephir_array_add(ret, "name", zephir_new_string(M->token));
	zephir_array_add(ret, "call-type", zephir_new_int(type));

	if (parameters) {
		zephir_array_add(ret, "parameters", parameters);
	}

	zephir_add_active_file(ret, state);
	zephir_add_active_line(ret, state);
	zephir_add_active_char(ret, state);

	return ret;
}

static zval *zephir_ret_scall(int dynamic_class, zephir_parser_token *O, int dynamic_method, zephir_parser_token *M, zval *parameters, zephir_scanner_state *state)
{
	zval *ret = zephir_new_array();

	zephir_array_add(ret, "type", zephir_new_string("scall"));
	zephir_array_add(ret, "dynamic-class", zephir_new_int(dynamic_class));
	zephir_array_add(ret, "class", zephir_new_string(O->token));
	zephir_array_add(ret, "dynamic", zephir_new_int(dynamic_method));
	zephir_array_add(ret, "name", zephir_new_string(M->token));

	if (parameters) {
		zephir_array_add(ret, "parameters", parameters);
	}

	zephir_add_active_file(ret, state);
	zephir_add_active_line(ret, state);
	zephir_add_active_char(ret, state);

	return ret;
}

static zval *zephir_ret_call_parameter(zephir_parser_token *N, zval *parameter, zephir_scanner_state *state)
{
	zval *ret = zephir_new_array();

	if (N) {
		zephir_array_add(ret, "name", zephir_new_string(N->token));
	}
	zephir_array_add(ret, "parameter", parameter);

	zephir_add_active_file(ret, state);
	zephir_add_active_line(ret, state);
	zephir_add_active_char(ret, state);

	return ret;
}

static zval *zephir_ret_comment(zephir_parser_token *T, zephir_scanner_state *state)
{
	zval *ret = zephir_new_array();

	zephir_array_add(ret, "type", zephir_new_string("comment"));
	zephir_array_add(ret, "value", zephir_new_string(T->token));

	zephir_add_active_file(ret, state);
	zephir_add_active_line(ret, state);
	zephir_add_active_char(ret, state);

	efree(T->token);
	efree(T);

	return ret;
}

static zval *zephir_ret_cblock(zephir_parser_token *T, zephir_scanner_state *state)
{
	zval *ret = zephir_new_array();

	zephir_array_add(ret, "type", zephir_new_string("cblock"));
	zephir_array_add(ret, "value", zephir_new_string(T->token));

	zephir_add_active_file(ret, state);
	zephir_add_active_line(ret, state);
	zephir_add_active_char(ret, state);

	efree(T->token);
	efree(T);

	return ret;
}


// 1198 "parser.c"
/* Next is all token values, in a form suitable for use by makeheaders.
** This section will be null unless lemon is run with the -m switch.
*/
/* 
** These constants (all generated automatically by the parser generator)
** specify the various kinds of tokens (terminals) that the parser
** understands. 
**
** Each symbol here is a terminal symbol in the grammar.
*/
/* Make sure the INTERFACE macro is defined.
*/
#ifndef INTERFACE
# define INTERFACE 1
#endif
/* The next thing included is series of defines which control
** various aspects of the generated parser.
**    YYCODETYPE         is the data type used for storing terminal
**                       and nonterminal numbers.  "unsigned char" is
**                       used if there are fewer than 250 terminals
**                       and nonterminals.  "int" is used otherwise.
**    YYNOCODE           is a number of type YYCODETYPE which corresponds
**                       to no legal terminal or nonterminal number.  This
**                       number is used to fill in empty slots of the hash 
**                       table.
**    YYFALLBACK         If defined, this indicates that one or more tokens
**                       have fall-back values which should be used if the
**                       original value of the token will not parse.
**    YYACTIONTYPE       is the data type used for storing terminal
**                       and nonterminal numbers.  "unsigned char" is
**                       used if there are fewer than 250 rules and
**                       states combined.  "int" is used otherwise.
**    zephir_TOKENTYPE     is the data type used for minor tokens given 
**                       directly to the parser from the tokenizer.
**    YYMINORTYPE        is the data type used for all minor tokens.
**                       This is typically a union of many types, one of
**                       which is zephir_TOKENTYPE.  The entry in the union
**                       for base tokens is called "yy0".
**    YYSTACKDEPTH       is the maximum depth of the parser's stack.
**    zephir_ARG_SDECL     A static variable declaration for the %extra_argument
**    zephir_ARG_PDECL     A parameter declaration for the %extra_argument
**    zephir_ARG_STORE     Code to store %extra_argument into yypParser
**    zephir_ARG_FETCH     Code to extract %extra_argument from yypParser
**    YYNSTATE           the combined number of states.
**    YYNRULE            the number of rules in the grammar
**    YYERRORSYMBOL      is the code number of the error symbol.  If not
**                       defined, then do no error processing.
*/
#define YYCODETYPE unsigned char
#define YYNOCODE 215
#define YYACTIONTYPE unsigned short int
#define zephir_TOKENTYPE zephir_parser_token*
typedef union {
  zephir_TOKENTYPE yy0;
  zval* yy180;
  int yy429;
} YYMINORTYPE;
#define YYSTACKDEPTH 100
#define zephir_ARG_SDECL zephir_parser_status *status;
#define zephir_ARG_PDECL ,zephir_parser_status *status
#define zephir_ARG_FETCH zephir_parser_status *status = yypParser->status
#define zephir_ARG_STORE yypParser->status = status
#define YYNSTATE 792
#define YYNRULE 389
#define YYERRORSYMBOL 118
#define YYERRSYMDT yy429
#define YY_NO_ACTION      (YYNSTATE+YYNRULE+2)
#define YY_ACCEPT_ACTION  (YYNSTATE+YYNRULE+1)
#define YY_ERROR_ACTION   (YYNSTATE+YYNRULE)

/* Next are that tables used to determine what action to take based on the
** current state and lookahead token.  These tables are used to implement
** functions that take a state number and lookahead value and return an
** action integer.  
**
** Suppose the action integer is N.  Then the action is determined as
** follows
**
**   0 <= N < YYNSTATE                  Shift N.  That is, push the lookahead
**                                      token onto the stack and goto state N.
**
**   YYNSTATE <= N < YYNSTATE+YYNRULE   Reduce by rule N-YYNSTATE.
**
**   N == YYNSTATE+YYNRULE              A syntax error has occurred.
**
**   N == YYNSTATE+YYNRULE+1            The parser accepts its input.
**
**   N == YYNSTATE+YYNRULE+2            No such action.  Denotes unused
**                                      slots in the yy_action[] table.
**
** The action table is constructed as a single large table named yy_action[].
** Given state S and lookahead X, the action is computed as
**
**      yy_action[ yy_shift_ofst[S] + X ]
**
** If the index value yy_shift_ofst[S]+X is out of range or if the value
** yy_lookahead[yy_shift_ofst[S]+X] is not equal to X or if yy_shift_ofst[S]
** is equal to YY_SHIFT_USE_DFLT, it means that the action is not in the table
** and that yy_default[S] should be used instead.  
**
** The formula above is for computing the action when the lookahead is
** a terminal symbol.  If the lookahead is a non-terminal (as occurs after
** a reduce action) then the yy_reduce_ofst[] array is used in place of
** the yy_shift_ofst[] array and YY_REDUCE_USE_DFLT is used in place of
** YY_SHIFT_USE_DFLT.
**
** The following are the tables generated in this section:
**
**  yy_action[]        A single table containing all actions.
**  yy_lookahead[]     A table containing the lookahead for each entry in
**                     yy_action.  Used to detect hash collisions.
**  yy_shift_ofst[]    For each state, the offset into yy_action for
**                     shifting terminals.
**  yy_reduce_ofst[]   For each state, the offset into yy_action for
**                     shifting non-terminals after a reduce.
**  yy_default[]       Default action for each state.
*/
static YYACTIONTYPE yy_action[] = {
 /*     0 */   574,  109,  292,  295, 1182,    1,    2,  791,    4,    5,
 /*    10 */     6,    7,    8,    9,  344,  792,   75,   46,   47,   51,
 /*    20 */    52,  277,  130,  138,  741,  142,  280,  354,  289,  359,
 /*    30 */   286,  378,  274,  353,  369,  736,  742,  348,  233,   55,
 /*    40 */   250,  306,  308,  316,  310,  312,  314,  385,  206,  623,
 /*    50 */   130,  138,  368,   82,  338,  303,  299,  365,   56,  426,
 /*    60 */   583,  592,  601,  604,  595,  598,  607,  613,  610,  619,
 /*    70 */   616,  254,  108,  664,  234,  236,  238,  349,  247,  116,
 /*    80 */   124,  302,  258,  262,  267,  456,  759,  467,  725,  747,
 /*    90 */   474,  392,  394,  393,  358,  723,   16,  362,  432,  395,
 /*   100 */   168,  555,  570,  577,  580,  361,  363,  364,  366,  367,
 /*   110 */   624,  574,   21,  292,  295,  195,   25,   14,  197,  757,
 /*   120 */   763,   28,  762,  747,  162,  344,   11,   81,  183,  723,
 /*   130 */   130,  138,  277,  142,  168,   15,  646,  280,  354,  289,
 /*   140 */   359,  286,  378,  274,  652,  369,  104,  353,  348,  233,
 /*   150 */     3,    4,    5,    6,    7,    8,    9,  252,  385,  208,
 /*   160 */   623,   76,  739,  368,  109,  338,   18,  487,  365,  540,
 /*   170 */   509,  583,  592,  601,  604,  595,  598,  607,  613,  610,
 /*   180 */   619,  616,  251,  667,  464,  234,  236,  238,  254,  247,
 /*   190 */   664,  349,  302,  258,  262,  267,  456,  492,  467,  501,
 /*   200 */   509,  474,  392,  394,  393,  358,  103,   12,  362,  428,
 /*   210 */   395,  738,  555,  570,  577,  580,  361,  363,  364,  366,
 /*   220 */   367,  624,  574, 1174,  292,  295,   41,  302,   44,   73,
 /*   230 */    47,   51,   52,  105,  106,  110,  344,  392,  394,  393,
 /*   240 */   358,   94,  120,  277,  423,  395,  153,  117,  280,  354,
 /*   250 */   289,  359,  286,  378,  274,  353,  369,  168,   40,  348,
 /*   260 */   233,   74,   77,  721,  198,   80,  725,  747,  729,  385,
 /*   270 */   669,  623,  183,  723,  368,   17,  338,  735,  168,  365,
 /*   280 */   730, 1155,  583,  592,  601,  604,  595,  598,  607,  613,
 /*   290 */   610,  619,  616,  191,   19,  767,  234,  236,  238,  349,
 /*   300 */   247,  303,  299,  302,  258,  262,  267,  456,  300,  467,
 /*   310 */    81,  200,  474,  392,  394,  393,  358,  199,  435,  362,
 /*   320 */   382,  395,  168,  555,  570,  577,  580,  361,  363,  364,
 /*   330 */   366,  367,  624,  574,   20,  292,  295,   78,  302,   44,
 /*   340 */    73,   47,   51,   52,  163,  377,   35,  344,  392,  394,
 /*   350 */   393,  358,  477,   34,  277,  389,  395,  168,  764,  280,
 /*   360 */   354,  289,  359,  286,  378,  274,  302,  369,  134,  436,
 /*   370 */   348,  233,  523,  131,  529,  509,  392,  394,  393,  358,
 /*   380 */   385,  242,  623,  399,  395,  368,  158,  338,   24,  161,
 /*   390 */   365,  476,  675,  583,  592,  601,  604,  595,  598,  607,
 /*   400 */   613,  610,  619,  616,  205,  376,  193,  234,  236,  238,
 /*   410 */   749,  247,  388,  350,  302,  258,  262,  267,  456,  735,
 /*   420 */   467,  687,  750,  474,  392,  394,  393,  358,  431,  442,
 /*   430 */   362,  406,  395,  683,  555,  570,  577,  580,  361,  363,
 /*   440 */   364,  366,  367,  624,  574,  379,  292,  295,  159,  302,
 /*   450 */    44,   73,   47,   51,   52,  410,  396,  200,  344,  392,
 /*   460 */   394,  393,  358,  199,  513,  277,  415,  395,  168,   28,
 /*   470 */   280,  354,  289,  359,  286,  378,  274,  302,  369,   23,
 /*   480 */   164,  348,  233,  167,  460,  656,   26,  392,  394,  393,
 /*   490 */   358,  385,  668,  623,  439,  395,  368,  187,  338,  768,
 /*   500 */   190,  365,  193,  193,  583,  592,  601,  604,  595,  598,
 /*   510 */   607,  613,  610,  619,  616,   27,  463,  655,  234,  236,
 /*   520 */   238,  632,  247,  397,  517,  302,  258,  262,  267,  456,
 /*   530 */   384,  467,   31,  402,  474,  392,  394,  393,  358,  143,
 /*   540 */   384,  362,  445,  395,  384,  555,  570,  577,  580,  361,
 /*   550 */   363,  364,  366,  367,  624,  574,  384,  292,  295,  722,
 /*   560 */   384,  758,  200,  761,  400,  762,  747,  169,  723,  344,
 /*   570 */   788,  169,  723,  168,  407,  193,  277,  168,  383,  384,
 /*   580 */   384,  280,  354,  289,  359,  286,  378,  274,  560,  369,
 /*   590 */   390,   37,  348,  233,  416,  479,  556,  561,  392,  394,
 /*   600 */   393,  358,  385,  246,  623,  549,   38,  368,  502,  338,
 /*   610 */   469,  508,  365,  424,  429,  583,  592,  601,  604,  595,
 /*   620 */   598,  607,  613,  610,  619,  616,  491,  737,  744,  234,
 /*   630 */   236,  238,  694,  247,  475,  554,  512,  258,  262,  267,
 /*   640 */   456,  703,  467,  168,  165,  474,   44,   73,   47,   51,
 /*   650 */    52,  530,  362,  699,  508,   42,  555,  570,  577,  580,
 /*   660 */   361,  363,  364,  366,  367,  624,  574,  384,  292,  295,
 /*   670 */   676,  239,   44,   73,   47,   51,   52,  558,  455,  688,
 /*   680 */   344,   44,   73,   47,   51,   52,   45,  277,  392,  394,
 /*   690 */   393,  358,  280,  354,  289,  359,  286,  378,  274,  302,
 /*   700 */   369,  433,   48,  348,  233,  384,  584,  591,   54,  392,
 /*   710 */   394,  393,  358,  385,  663,  623,  557,  409,  368,  775,
 /*   720 */   338,  781,  541,  365,  193,  508,  583,  592,  601,  604,
 /*   730 */   595,  598,  607,  613,  610,  619,  616,  593,  591,  440,
 /*   740 */   234,  236,  238,  715,  247,  596,  591,  500,  258,  262,
 /*   750 */   267,  456,  732,  467,  498,  711,  474,  392,  394,  393,
 /*   760 */   358,  735,   57,  362,  599,  591,   58,  555,  570,  577,
 /*   770 */   580,  361,  363,  364,  366,  367,  624,  574,  384,  292,
 /*   780 */   295,  704,  248,   44,   73,   47,   51,   52,   94,  455,
 /*   790 */   716,  344,   44,   73,   47,   51,   52,  586,  277,  392,
 /*   800 */   394,  393,  358,  280,  354,  289,  359,  286,  378,  274,
 /*   810 */   560,  369,  446,   79,  348,  233,  310,  312,  314,  559,
 /*   820 */   392,  394,  393,  358,  385,  261,  623,  303,  299,  368,
 /*   830 */   586,  338,  586,   84,  365,   85,  585,  583,  592,  601,
 /*   840 */   604,  595,  598,  607,  613,  610,  619,  616,  602,  591,
 /*   850 */  1176,  234,  236,  238,   95,  247,  586,  605,  591,  258,
 /*   860 */   262,  267,  456,   33,  467,   35,  727,  474,  586,  594,
 /*   870 */   169,  597,  170,  769,  362,  767,  168,  168,  555,  570,
 /*   880 */   577,  580,  361,  363,  364,  366,  367,  624,  574,  586,
 /*   890 */   292,  295,  681,  489,  779,  603,  767,  488,  130,  138,
 /*   900 */   370,  142,  344,  392,  394,  393,  358,  600,  586,  277,
 /*   910 */   392,  394,  393,  358,  280,  354,  289,  359,  286,  378,
 /*   920 */   274,  420,  369,  709, 1175,  348,  233,   81,  606,  130,
 /*   930 */   138,  114,  142,  608,  591,  385,  662,  623,  611,  591,
 /*   940 */   368,  109,  338,   81,  586,  365,  107,  609,  583,  592,
 /*   950 */   601,  604,  595,  598,  607,  613,  610,  619,  616,  614,
 /*   960 */   591,  682,  234,  236,  238,  752,  247,  586,  118,  275,
 /*   970 */   258,  262,  267,  456,  735,  467,  115,  710,  474,  392,
 /*   980 */   394,  393,  358,  612,  119,  362,  617,  591,  121,  555,
 /*   990 */   570,  577,  580,  361,  363,  364,  366,  367,  624,  574,
 /*  1000 */   586,  292,  295,  782,  489,  767,  615,  741,  493,  102,
 /*  1010 */   111,  106,  110,  344,  392,  394,  393,  358,  586,  742,
 /*  1020 */   277,  620,  591,  122,  123,  280,  354,  289,  359,  286,
 /*  1030 */   378,  274,  454,  369,  125,  778,  348,  233,  193,  618,
 /*  1040 */   126,  127,  392,  394,  393,  358,  385,  266,  623,  129,
 /*  1050 */    55,  368,  132,  338,  133,  135,  365,  621,  136,  583,
 /*  1060 */   592,  601,  604,  595,  598,  607,  613,  610,  619,  616,
 /*  1070 */   137,  139,  140,  234,  236,  238,  141,  247,  155,  156,
 /*  1080 */   278,  258,  262,  267,  456,  160,  467,  166,  173,  474,
 /*  1090 */   392,  394,  393,  358,  174,  175,  362,  176,  178,  180,
 /*  1100 */   555,  570,  577,  580,  361,  363,  364,  366,  367,  624,
 /*  1110 */   574,  179,  292,  295,  181,  489,  185,  186,  188,  497,
 /*  1120 */   189,  766,  192,  196,  344,  392,  394,  393,  358,  202,
 /*  1130 */   240,  277,  203,  235,  237,  243,  280,  354,  289,  359,
 /*  1140 */   286,  378,  274,  453,  369,  244,  249,  348,  233,  255,
 /*  1150 */   253,  256,  259,  392,  394,  393,  358,  385,  658,  623,
 /*  1160 */   264,  268,  368,  271,  338,  273,  301,  365,  342,  345,
 /*  1170 */   583,  592,  601,  604,  595,  598,  607,  613,  610,  619,
 /*  1180 */   616,  346,  351,  355,  234,  236,  238,  356,  247, 1157,
 /*  1190 */   380,  281,  258,  262,  267,  456, 1156,  467,  386,  387,
 /*  1200 */   474,  392,  394,  393,  358,  403,  398,  362,  404,  405,
 /*  1210 */   411,  555,  570,  577,  580,  361,  363,  364,  366,  367,
 /*  1220 */   624,  574,  412,  292,  295,  413,  489,  419,  437,  438,
 /*  1230 */   503,  443,  444,  452,  457,  344,  392,  394,  393,  358,
 /*  1240 */   462,  461,  277,  392,  394,  393,  358,  280,  354,  289,
 /*  1250 */   359,  286,  378,  274,  284,  369,  654,  468,  348,  233,
 /*  1260 */   478,  499,  514,  518,  392,  394,  393,  358,  385,  270,
 /*  1270 */   623,  496,  506,  368,  515,  338,  522,  519,  365,  550,
 /*  1280 */   588,  583,  592,  601,  604,  595,  598,  607,  613,  610,
 /*  1290 */   619,  616,  527,  551,  563,  234,  236,  238,  565,  247,
 /*  1300 */   567,  534,  451,  258,  262,  267,  456,  538,  467,  545,
 /*  1310 */   552,  474,  392,  394,  393,  358,  569,  590,  362,  589,
 /*  1320 */   587,  633,  555,  570,  577,  580,  361,  363,  364,  366,
 /*  1330 */   367,  624,  574,  647,  292,  295,  634,  489,  648,  653,
 /*  1340 */   659,  507,  661,  665,  287,  670,  344,  392,  394,  393,
 /*  1350 */   358,  696,  671,  277,  392,  394,  393,  358,  280,  354,
 /*  1360 */   289,  359,  286,  378,  274,  450,  369,  677,  689,  348,
 /*  1370 */   233,  697,  705,  717,  745,  392,  394,  393,  358,  385,
 /*  1380 */   657,  623,  731,  734,  368,  733,  338,  849,  850,  365,
 /*  1390 */   740,  743,  583,  592,  601,  604,  595,  598,  607,  613,
 /*  1400 */   610,  619,  616,  746,  754,  751,  234,  236,  238,  755,
 /*  1410 */   247,  753,  756,  290,  258,  262,  267,  456,  770,  467,
 /*  1420 */   765,  774,  474,  392,  394,  393,  358,  772,  773,  362,
 /*  1430 */   776,  777,  780,  555,  570,  577,  580,  361,  363,  364,
 /*  1440 */   366,  367,  624,  574,  785,  292,  295,  783,  489,  786,
 /*  1450 */   789,  787,  516,  790,  193,  449,  605,  344,  392,  394,
 /*  1460 */   393,  358,  605,  605,  277,  392,  394,  393,  358,  280,
 /*  1470 */   354,  289,  359,  286,  378,  274,  293,  369,  605,  605,
 /*  1480 */   348,  233,  605,  605,  605,  605,  392,  394,  393,  358,
 /*  1490 */   385,  459,  623,  605,  605,  368,  605,  338,  605,  605,
 /*  1500 */   365,  605,  605,  583,  592,  601,  604,  595,  598,  607,
 /*  1510 */   613,  610,  619,  616,  605,  605,  605,  234,  236,  238,
 /*  1520 */   605,  247,  605,  605,  448,  258,  262,  267,  456,  605,
 /*  1530 */   467,  605,  605,  474,  392,  394,  393,  358,  605,  605,
 /*  1540 */   362,  605,  605,  605,  555,  570,  577,  580,  361,  363,
 /*  1550 */   364,  366,  367,  624,  574,  605,  292,  295,  605,  489,
 /*  1560 */   605,  605,  605,  520,  605,  605,  296,  605,  344,  392,
 /*  1570 */   394,  393,  358,  605,  605,  277,  392,  394,  393,  358,
 /*  1580 */   280,  354,  289,  359,  286,  378,  274,  298,  369,  605,
 /*  1590 */   605,  348,  233,  605,  605,  605,  605,  392,  394,  393,
 /*  1600 */   358,  385,  645,  623,  605,  605,  368,  605,  338,  605,
 /*  1610 */   605,  365,  605,  605,  583,  592,  601,  604,  595,  598,
 /*  1620 */   607,  613,  610,  619,  616,  605,  605,  605,  234,  236,
 /*  1630 */   238,  605,  247,  605,  605,  304,  258,  262,  267,  456,
 /*  1640 */   605,  467,  605,  605,  474,  392,  394,  393,  358,  605,
 /*  1650 */   605,  362,  605,  605,  605,  555,  570,  577,  580,  361,
 /*  1660 */   363,  364,  366,  367,  624,  574,  605,  292,  295,  605,
 /*  1670 */   489,  605,  605,  605,  524,  605,  605,  307,  605,  344,
 /*  1680 */   392,  394,  393,  358,  605,  605,  277,  392,  394,  393,
 /*  1690 */   358,  280,  354,  289,  359,  286,  378,  274,  309,  369,
 /*  1700 */   605,  605,  348,  233,  605,  605,  605,  605,  392,  394,
 /*  1710 */   393,  358,  385,  466,  623,  605,  605,  368,  605,  338,
 /*  1720 */   605,  605,  365,  605,  605,  583,  592,  601,  604,  595,
 /*  1730 */   598,  607,  613,  610,  619,  616,  605,  605,  605,  234,
 /*  1740 */   236,  238,  605,  247,  605,  605,  311,  258,  262,  267,
 /*  1750 */   456,  605,  467,  605,  605,  474,  392,  394,  393,  358,
 /*  1760 */   605,  605,  362,  605,  605,  605,  555,  570,  577,  580,
 /*  1770 */   361,  363,  364,  366,  367,  624,  574,  605,  292,  295,
 /*  1780 */   605,  489,  605,  605,  605,  528,  605,  605,  313,  605,
 /*  1790 */   344,  392,  394,  393,  358,  605,  605,  277,  392,  394,
 /*  1800 */   393,  358,  280,  354,  289,  359,  286,  378,  274,  315,
 /*  1810 */   369,  605,  605,  348,  233,  605,  605,  605,  605,  392,
 /*  1820 */   394,  393,  358,  385,  626,  623,  605,  605,  368,  605,
 /*  1830 */   338,  605,  605,  365,  605,  605,  583,  592,  601,  604,
 /*  1840 */   595,  598,  607,  613,  610,  619,  616,  605,  605,  605,
 /*  1850 */   234,  236,  238,  605,  247,  605,  605,  317,  258,  262,
 /*  1860 */   267,  456,  605,  467,  605,  605,  474,  392,  394,  393,
 /*  1870 */   358,  605,  605,  362,  605,  605,  605,  555,  570,  577,
 /*  1880 */   580,  361,  363,  364,  366,  367,  624,  574,  605,  292,
 /*  1890 */   295,  605,  489,  605,  605,  605,  531,  605,  605,  319,
 /*  1900 */   605,  344,  392,  394,  393,  358,  605,  605,  277,  392,
 /*  1910 */   394,  393,  358,  280,  354,  289,  359,  286,  378,  274,
 /*  1920 */   321,  369,  605,  605,  348,  233,  605,  605,  605,  605,
 /*  1930 */   392,  394,  393,  358,  385,  473,  623,  605,  605,  368,
 /*  1940 */   605,  338,  605,  605,  365,  605,  605,  583,  592,  601,
 /*  1950 */   604,  595,  598,  607,  613,  610,  619,  616,  605,  605,
 /*  1960 */   605,  234,  236,  238,  605,  247,  605,  605,  323,  258,
 /*  1970 */   262,  267,  456,  605,  467,  605,  605,  474,  392,  394,
 /*  1980 */   393,  358,  605,  605,  362,  605,  605,  605,  555,  570,
 /*  1990 */   577,  580,  361,  363,  364,  366,  367,  624,  574,  605,
 /*  2000 */   292,  295,  605,  489,  605,  605,  605,  535,  605,  605,
 /*  2010 */   325,  605,  344,  392,  394,  393,  358,  605,  605,  277,
 /*  2020 */   392,  394,  393,  358,  280,  354,  289,  359,  286,  378,
 /*  2030 */   274,  327,  369,  605,  605,  348,  233,  605,  605,  605,
 /*  2040 */   605,  392,  394,  393,  358,  385,  631,  623,  605,  605,
 /*  2050 */   368,  605,  338,  605,  605,  365,  605,  605,  583,  592,
 /*  2060 */   601,  604,  595,  598,  607,  613,  610,  619,  616,  605,
 /*  2070 */   605,  605,  234,  236,  238,  605,  247,  605,  605,  329,
 /*  2080 */   258,  262,  267,  456,  605,  467,  605,  605,  474,  392,
 /*  2090 */   394,  393,  358,  605,  605,  362,  605,  605,  605,  555,
 /*  2100 */   570,  577,  580,  361,  363,  364,  366,  367,  624,  574,
 /*  2110 */   605,  292,  295,  605,  489,  605,  605,  605,  539,  605,
 /*  2120 */   605,  331,  605,  344,  392,  394,  393,  358,  605,  605,
 /*  2130 */   277,  392,  394,  393,  358,  280,  354,  289,  359,  286,
 /*  2140 */   378,  274,  333,  369,  605,  605,  348,  233,  605,  605,
 /*  2150 */   605,  605,  392,  394,  393,  358,  385,  639,  623,  605,
 /*  2160 */   605,  368,  605,  338,  605,  605,  365,  605,  605,  583,
 /*  2170 */   592,  601,  604,  595,  598,  607,  613,  610,  619,  616,
 /*  2180 */   605,  605,  605,  234,  236,  238,  605,  247,  605,  605,
 /*  2190 */   335,  258,  262,  267,  456,  605,  467,  605,  605,  474,
 /*  2200 */   392,  394,  393,  358,  605,  605,  362,  605,  605,  605,
 /*  2210 */   555,  570,  577,  580,  361,  363,  364,  366,  367,  624,
 /*  2220 */   574,  605,  292,  295,  605,  489,  605,  605,  605,  542,
 /*  2230 */   605,  605,  337,  605,  344,  392,  394,  393,  358,  605,
 /*  2240 */   605,  277,  392,  394,  393,  358,  280,  354,  289,  359,
 /*  2250 */   286,  378,  274,  343,  369,  605,  605,  348,  233,  605,
 /*  2260 */   605,  605,  605,  392,  394,  393,  358,  385,  638,  623,
 /*  2270 */   605,  605,  368,  605,  338,  605,  605,  365,  605,  605,
 /*  2280 */   583,  592,  601,  604,  595,  598,  607,  613,  610,  619,
 /*  2290 */   616,  605,  605,  605,  234,  236,  238,  605,  247,  605,
 /*  2300 */   605,  347,  258,  262,  267,  456,  605,  467,  605,  605,
 /*  2310 */   474,  392,  394,  393,  358,  605,  605,  362,  605,  605,
 /*  2320 */   605,  555,  570,  577,  580,  361,  363,  364,  366,  367,
 /*  2330 */   624,  574,  605,  292,  295,  605,  489,  605,  605,  605,
 /*  2340 */   546,  605,  605,  357,  605,  344,  392,  394,  393,  358,
 /*  2350 */   605,  605,  277,  392,  394,  393,  358,  280,  354,  289,
 /*  2360 */   359,  286,  378,  274,  360,  369,  605,  605,  348,  233,
 /*  2370 */   605,  605,  605,  605,  392,  394,  393,  358,  385,  644,
 /*  2380 */   623,  605,  605,  368,  605,  338,  605,  605,  365,  605,
 /*  2390 */   605,  583,  592,  601,  604,  595,  598,  607,  613,  610,
 /*  2400 */   619,  616,  605,  605,  605,  234,  236,  238,  605,  247,
 /*  2410 */   605,  605,  427,  258,  262,  267,  456,  605,  467,  605,
 /*  2420 */   605,  474,  392,  394,  393,  358,  605,  605,  362,  605,
 /*  2430 */   605,  605,  555,  570,  577,  580,  361,  363,  364,  366,
 /*  2440 */   367,  624,  574,  605,  292,  295,  605,  489,  605,  605,
 /*  2450 */   605,  553,  605,  605,  470,  605,  344,  392,  394,  393,
 /*  2460 */   358,  605,  605,  277,  392,  394,  393,  358,  280,  354,
 /*  2470 */   289,  359,  286,  378,  274,  571,  369,  605,  605,  348,
 /*  2480 */   233,  605,  605,  605,  605,  392,  394,  393,  358,  385,
 /*  2490 */   651,  623,  605,  605,  368,  605,  338,  605,  605,  365,
 /*  2500 */   605,  605,  583,  592,  601,  604,  595,  598,  607,  613,
 /*  2510 */   610,  619,  616,  605,  605,  605,  234,  236,  238,  605,
 /*  2520 */   247,  605,  605,  575,  258,  262,  267,  456,  605,  467,
 /*  2530 */   605,  605,  474,  392,  394,  393,  358,  605,  605,  362,
 /*  2540 */   605,  605,  605,  555,  570,  577,  580,  361,  363,  364,
 /*  2550 */   366,  367,  624,  574,  605,  292,  295,  605,  578,  605,
 /*  2560 */   605,  605,  605,  605,  605,  581,  605,  344,  392,  394,
 /*  2570 */   393,  358,  605,  605,  277,  392,  394,  393,  358,  280,
 /*  2580 */   354,  289,  359,  286,  378,  274,  628,  369,  605,  605,
 /*  2590 */   348,  233,  605,  605,  605,  605,  392,  394,  393,  358,
 /*  2600 */   385,  650,  623,  605,  605,  368,  605,  338,  605,  605,
 /*  2610 */   365,  605,  605,  583,  592,  601,  604,  595,  598,  607,
 /*  2620 */   613,  610,  619,  616,  605,  605,  605,  234,  236,  238,
 /*  2630 */   605,  247,  605,  605,  635,  258,  262,  267,  456,  605,
 /*  2640 */   467,  605,  605,  474,  392,  394,  393,  358,  605,  605,
 /*  2650 */   362,  605,  605,  605,  555,  570,  577,  580,  361,  363,
 /*  2660 */   364,  366,  367,  624,  574,  605,  292,  295,  605,  641,
 /*  2670 */   605,  605,  605,  605,  605,  605,  605,  605,  344,  392,
 /*  2680 */   394,  393,  358,  605,  605,  277,  605,  605,  605,  605,
 /*  2690 */   280,  354,  289,  359,  286,  378,  274,  605,  369,  605,
 /*  2700 */   605,  348,  233,  605,  605,  605,  605,  605,  605,  605,
 /*  2710 */   605,  385,  674,  623,  605,  605,  368,  605,  338,  605,
 /*  2720 */   605,  365,  605,  605,  583,  592,  601,  604,  595,  598,
 /*  2730 */   607,  613,  610,  619,  616,  605,  605,  605,  234,  236,
 /*  2740 */   238,  605,  247,  605,  605,  605,  258,  262,  267,  456,
 /*  2750 */   605,  467,  605,  605,  474,  605,  605,  605,  605,  605,
 /*  2760 */   605,  362,  605,  605,  605,  555,  570,  577,  580,  361,
 /*  2770 */   363,  364,  366,  367,  624,  574,  605,  292,  295,  605,
 /*  2780 */   605,  605,  605,  605,  605,  605,  605,  605,  605,  344,
 /*  2790 */   605,  605,  605,  605,  605,  605,  277,  605,  605,  605,
 /*  2800 */   605,  280,  354,  289,  359,  286,  378,  274,  605,  369,
 /*  2810 */   605,  605,  348,  233,  605,  605,  605,  605,  605,  605,
 /*  2820 */   605,  605,  385,  673,  623,  605,  605,  368,  605,  338,
 /*  2830 */   605,  605,  365,  605,  605,  583,  592,  601,  604,  595,
 /*  2840 */   598,  607,  613,  610,  619,  616,  605,  605,  605,  234,
 /*  2850 */   236,  238,  605,  247,  605,  605,  605,  258,  262,  267,
 /*  2860 */   456,  605,  467,  605,  605,  474,  605,  605,  605,  605,
 /*  2870 */   605,  605,  362,  605,  605,  605,  555,  570,  577,  580,
 /*  2880 */   361,  363,  364,  366,  367,  624,  574,  605,  292,  295,
 /*  2890 */   605,  605,  605,  605,  605,  605,  605,  605,  605,  605,
 /*  2900 */   344,  605,  605,  605,  605,  605,  605,  277,  605,  605,
 /*  2910 */   605,  605,  280,  354,  289,  359,  286,  378,  274,  605,
 /*  2920 */   369,  605,  605,  348,  233,  605,  605,  605,  605,  605,
 /*  2930 */   605,  605,  605,  385,  678,  623,  605,  605,  368,  605,
 /*  2940 */   338,  605,  605,  365,  605,  605,  583,  592,  601,  604,
 /*  2950 */   595,  598,  607,  613,  610,  619,  616,  605,  605,  605,
 /*  2960 */   234,  236,  238,  605,  247,  605,  605,  605,  258,  262,
 /*  2970 */   267,  456,  605,  467,  605,  605,  474,  605,  605,  605,
 /*  2980 */   605,  605,  605,  362,  605,  605,  605,  555,  570,  577,
 /*  2990 */   580,  361,  363,  364,  366,  367,  624,  574,  605,  292,
 /*  3000 */   295,  605,  605,  605,  605,  605,  605,  605,  605,  605,
 /*  3010 */   605,  344,  605,  605,  605,  605,  605,  605,  277,  605,
 /*  3020 */   605,  605,  605,  280,  354,  289,  359,  286,  378,  274,
 /*  3030 */   605,  369,  605,  605,  348,  233,  605,  605,  605,  605,
 /*  3040 */   605,  605,  605,  605,  385,  680,  623,  605,  605,  368,
 /*  3050 */   605,  338,  605,  605,  365,  605,  605,  583,  592,  601,
 /*  3060 */   604,  595,  598,  607,  613,  610,  619,  616,  605,  605,
 /*  3070 */   605,  234,  236,  238,  605,  247,  605,  605,  605,  258,
 /*  3080 */   262,  267,  456,  605,  467,  605,  605,  474,  605,  605,
 /*  3090 */   605,  605,  605,  605,  362,  605,  605,  605,  555,  570,
 /*  3100 */   577,  580,  361,  363,  364,  366,  367,  624,  574,  605,
 /*  3110 */   292,  295,  605,  605,  605,  605,  605,  605,  605,  605,
 /*  3120 */   605,  605,  344,  605,  605,  605,  605,  605,  605,  277,
 /*  3130 */   605,  605,  605,  605,  280,  354,  289,  359,  286,  378,
 /*  3140 */   274,  605,  369,  605,  605,  348,  233,  605,  605,  605,
 /*  3150 */   605,  605,  605,  605,  605,  385,  684,  623,  605,  605,
 /*  3160 */   368,  605,  338,  605,  605,  365,  605,  605,  583,  592,
 /*  3170 */   601,  604,  595,  598,  607,  613,  610,  619,  616,  605,
 /*  3180 */   605,  605,  234,  236,  238,  605,  247,  605,  605,  605,
 /*  3190 */   258,  262,  267,  456,  605,  467,  605,  605,  474,  605,
 /*  3200 */   605,  605,  605,  605,  605,  362,  605,  605,  605,  555,
 /*  3210 */   570,  577,  580,  361,  363,  364,  366,  367,  624,  574,
 /*  3220 */   605,  292,  295,  605,  605,  605,  605,  605,  605,  605,
 /*  3230 */   605,  605,  605,  344,  605,  605,  605,  605,  605,  605,
 /*  3240 */   277,  605,  605,  605,  605,  280,  354,  289,  359,  286,
 /*  3250 */   378,  274,  605,  369,  605,  605,  348,  233,  605,  605,
 /*  3260 */   605,  605,  605,  605,  605,  605,  385,  686,  623,  605,
 /*  3270 */   605,  368,  605,  338,  605,  605,  365,  605,  605,  583,
 /*  3280 */   592,  601,  604,  595,  598,  607,  613,  610,  619,  616,
 /*  3290 */   605,  605,  605,  234,  236,  238,  605,  247,  605,  605,
 /*  3300 */   605,  258,  262,  267,  456,  605,  467,  605,  605,  474,
 /*  3310 */   605,  605,  605,  605,  605,  605,  362,  605,  605,  605,
 /*  3320 */   555,  570,  577,  580,  361,  363,  364,  366,  367,  624,
 /*  3330 */   574,  605,  292,  295,  605,  605,  605,  605,  605,  605,
 /*  3340 */   605,  605,  605,  605,  344,  605,  605,  605,  605,  605,
 /*  3350 */   605,  277,  605,  605,  605,  605,  280,  354,  289,  359,
 /*  3360 */   286,  378,  274,  605,  369,  605,  605,  348,  233,  605,
 /*  3370 */   605,  605,  605,  605,  605,  605,  605,  385,  690,  623,
 /*  3380 */   605,  605,  368,  605,  338,  605,  605,  365,  605,  605,
 /*  3390 */   583,  592,  601,  604,  595,  598,  607,  613,  610,  619,
 /*  3400 */   616,  605,  605,  605,  234,  236,  238,  605,  247,  605,
 /*  3410 */   605,  605,  258,  262,  267,  456,  605,  467,  605,  605,
 /*  3420 */   474,  605,  605,  605,  605,  605,  605,  362,  605,  605,
 /*  3430 */   605,  555,  570,  577,  580,  361,  363,  364,  366,  367,
 /*  3440 */   624,  574,  605,  292,  295,  605,  605,  605,  605,  605,
 /*  3450 */   605,  605,  605,  605,  605,  344,  605,  605,  605,  605,
 /*  3460 */   605,  605,  277,  605,  605,  605,  605,  280,  354,  289,
 /*  3470 */   359,  286,  378,  274,  605,  369,  605,  605,  348,  233,
 /*  3480 */   605,  605,  605,  605,  605,  605,  605,  605,  385,  692,
 /*  3490 */   623,  605,  605,  368,  605,  338,  605,  605,  365,  605,
 /*  3500 */   605,  583,  592,  601,  604,  595,  598,  607,  613,  610,
 /*  3510 */   619,  616,  605,  605,  605,  234,  236,  238,  605,  247,
 /*  3520 */   605,  605,  605,  258,  262,  267,  456,  605,  467,  605,
 /*  3530 */   605,  474,  605,  605,  605,  605,  605,  605,  362,  605,
 /*  3540 */   605,  605,  555,  570,  577,  580,  361,  363,  364,  366,
 /*  3550 */   367,  624,  574,  605,  292,  295,  605,  605,  605,  605,
 /*  3560 */   605,  605,  605,  605,  605,  605,  344,  605,  605,  605,
 /*  3570 */   605,  605,  605,  277,  605,  605,  605,  605,  280,  354,
 /*  3580 */   289,  359,  286,  378,  274,  605,  369,  605,  605,  348,
 /*  3590 */   233,  605,  605,  605,  605,  605,  605,  605,  605,  385,
 /*  3600 */   700,  623,  605,  605,  368,  605,  338,  605,  605,  365,
 /*  3610 */   605,  605,  583,  592,  601,  604,  595,  598,  607,  613,
 /*  3620 */   610,  619,  616,  605,  605,  605,  234,  236,  238,  605,
 /*  3630 */   247,  605,  605,  605,  258,  262,  267,  456,  605,  467,
 /*  3640 */   605,  605,  474,  605,  605,  605,  605,  605,  605,  362,
 /*  3650 */   605,  605,  605,  555,  570,  577,  580,  361,  363,  364,
 /*  3660 */   366,  367,  624,  574,  605,  292,  295,  605,  605,  605,
 /*  3670 */   605,  605,  605,  605,  605,  605,  605,  344,  605,  605,
 /*  3680 */   605,  605,  605,  605,  277,  605,  605,  605,  605,  280,
 /*  3690 */   354,  289,  359,  286,  378,  274,  605,  369,  605,  605,
 /*  3700 */   348,  233,  605,  605,  605,  605,  605,  605,  605,  605,
 /*  3710 */   385,  702,  623,  605,  605,  368,  605,  338,  605,  605,
 /*  3720 */   365,  605,  605,  583,  592,  601,  604,  595,  598,  607,
 /*  3730 */   613,  610,  619,  616,  605,  605,  605,  234,  236,  238,
 /*  3740 */   605,  247,  605,  605,  605,  258,  262,  267,  456,  605,
 /*  3750 */   467,  605,  605,  474,  605,  605,  605,  605,  605,  605,
 /*  3760 */   362,  605,  605,  605,  555,  570,  577,  580,  361,  363,
 /*  3770 */   364,  366,  367,  624,  574,  605,  292,  295,  605,  605,
 /*  3780 */   605,  605,  605,  605,  605,  605,  605,  605,  344,  605,
 /*  3790 */   605,  605,  605,  605,  605,  277,  605,  605,  605,  605,
 /*  3800 */   280,  354,  289,  359,  286,  378,  274,  605,  369,  605,
 /*  3810 */   605,  348,  233,  605,  605,  605,  605,  605,  605,  605,
 /*  3820 */   605,  385,  706,  623,  605,  605,  368,  605,  338,  605,
 /*  3830 */   605,  365,  605,  605,  583,  592,  601,  604,  595,  598,
 /*  3840 */   607,  613,  610,  619,  616,  605,  605,  605,  234,  236,
 /*  3850 */   238,  605,  247,  605,  605,  605,  258,  262,  267,  456,
 /*  3860 */   605,  467,  605,  605,  474,  605,  605,  605,  605,  605,
 /*  3870 */   605,  362,  605,  605,  605,  555,  570,  577,  580,  361,
 /*  3880 */   363,  364,  366,  367,  624,  574,  605,  292,  295,  605,
 /*  3890 */   605,  605,  605,  605,  605,  605,  605,  605,  605,  344,
 /*  3900 */   605,  605,  605,  605,  605,  605,  277,  605,  605,  605,
 /*  3910 */   605,  280,  354,  289,  359,  286,  378,  274,  605,  369,
 /*  3920 */   605,  605,  348,  233,  605,  605,  605,  605,  605,  605,
 /*  3930 */   605,  605,  385,  708,  623,  605,  605,  368,  605,  338,
 /*  3940 */   605,  605,  365,  605,  605,  583,  592,  601,  604,  595,
 /*  3950 */   598,  607,  613,  610,  619,  616,  605,  605,  605,  234,
 /*  3960 */   236,  238,  605,  247,  605,  605,  605,  258,  262,  267,
 /*  3970 */   456,  605,  467,  605,  605,  474,  605,  605,  605,  605,
 /*  3980 */   605,  605,  362,  605,  605,  605,  555,  570,  577,  580,
 /*  3990 */   361,  363,  364,  366,  367,  624,  574,  605,  292,  295,
 /*  4000 */   605,  605,  605,  605,  605,  605,  605,  605,  605,  605,
 /*  4010 */   344,  605,  605,  605,  605,  605,  605,  277,  605,  605,
 /*  4020 */   605,  605,  280,  354,  289,  359,  286,  378,  274,  605,
 /*  4030 */   369,  605,  605,  348,  233,  605,  605,  605,  605,  605,
 /*  4040 */   605,  605,  605,  385,  712,  623,  605,  605,  368,  605,
 /*  4050 */   338,  605,  605,  365,  605,  605,  583,  592,  601,  604,
 /*  4060 */   595,  598,  607,  613,  610,  619,  616,  605,  605,  605,
 /*  4070 */   234,  236,  238,  605,  247,  605,  605,  605,  258,  262,
 /*  4080 */   267,  456,  605,  467,  605,  605,  474,  605,  605,  605,
 /*  4090 */   605,  605,  605,  362,  605,  605,  605,  555,  570,  577,
 /*  4100 */   580,  361,  363,  364,  366,  367,  624,  574,  605,  292,
 /*  4110 */   295,  605,  605,  605,  605,  605,  605,  605,  605,  605,
 /*  4120 */   605,  344,  605,  605,  605,  605,  605,  605,  277,  605,
 /*  4130 */   605,  605,  605,  280,  354,  289,  359,  286,  378,  274,
 /*  4140 */   605,  369,  605,  605,  348,  233,  605,  605,  605,  605,
 /*  4150 */   605,  605,  605,  605,  385,  714,  623,  605,  605,  368,
 /*  4160 */   605,  338,  605,  605,  365,  605,  605,  583,  592,  601,
 /*  4170 */   604,  595,  598,  607,  613,  610,  619,  616,  605,  605,
 /*  4180 */   605,  234,  236,  238,  605,  247,  605,  605,  605,  258,
 /*  4190 */   262,  267,  456,  605,  467,  605,  605,  474,  605,  605,
 /*  4200 */   605,  605,  605,  605,  362,  605,  605,  605,  555,  570,
 /*  4210 */   577,  580,  361,  363,  364,  366,  367,  624,  574,  605,
 /*  4220 */   292,  295,  605,  605,  605,  605,  605,  605,  605,  605,
 /*  4230 */   605,  605,  344,  605,  605,  605,  605,  605,  605,  277,
 /*  4240 */   605,  605,  605,  605,  280,  354,  289,  359,  286,  378,
 /*  4250 */   274,  605,  369,  605,  605,  348,  233,  605,  605,  605,
 /*  4260 */   605,  605,  605,  605,  605,  385,  718,  623,  605,  605,
 /*  4270 */   368,  605,  338,  605,  605,  365,  605,  605,  583,  592,
 /*  4280 */   601,  604,  595,  598,  607,  613,  610,  619,  616,  605,
 /*  4290 */   605,  605,  234,  236,  238,  605,  247,  605,  605,  605,
 /*  4300 */   258,  262,  267,  456,  605,  467,  605,  605,  474,  605,
 /*  4310 */   605,  605,  605,  605,  605,  362,  605,  605,  605,  555,
 /*  4320 */   570,  577,  580,  361,  363,  364,  366,  367,  624,  574,
 /*  4330 */   605,  292,  295,  605,  605,  605,  605,  605,  605,  605,
 /*  4340 */   605,  605,  605,  344,  605,  605,  605,  605,  605,  605,
 /*  4350 */   277,  605,  605,  605,  605,  280,  354,  289,  359,  286,
 /*  4360 */   378,  274,  605,  369,  605,  605,  348,  233,  605,  605,
 /*  4370 */   605,  605,  605,  605,  605,  605,  385,  720,  623,  605,
 /*  4380 */   605,  368,  605,  338,  605,  605,  365,  605,  605,  583,
 /*  4390 */   592,  601,  604,  595,  598,  607,  613,  610,  619,  616,
 /*  4400 */   605,  605,  605,  234,  236,  238,  605,  247,  605,  605,
 /*  4410 */   605,  258,  262,  267,  456,  605,  467,  605,  605,  474,
 /*  4420 */   605,  605,  605,  605,  605,  605,  362,  605,  605,  605,
 /*  4430 */   555,  570,  577,  580,  361,  363,  364,  366,  367,  624,
 /*  4440 */   574,  605,  292,  295,  605,  605,  605,  605,  605,  605,
 /*  4450 */   605,  605,  605,  605,  344,  605,  605,  605,  605,  605,
 /*  4460 */   605,  277,  605,  605,  605,  605,  280,  354,  289,  359,
 /*  4470 */   286,  378,  274,  605,  369,  605,  605,  348,  233,  605,
 /*  4480 */   605,  605,  605,  605,  605,  605,  605,  385,  341,  623,
 /*  4490 */   605,  605,  368,  605,  338,  605,  605,  365,  605,  605,
 /*  4500 */   583,  592,  601,  604,  595,  598,  607,  613,  610,  619,
 /*  4510 */   616,  605,  605,  605,  234,  236,  238,  605,  247,  605,
 /*  4520 */   605,  339,  258,  262,  267,  456,  605,  467,  605,  605,
 /*  4530 */   474,  392,  394,  393,  358,  605,  605,  362,  605,  605,
 /*  4540 */   605,  555,  570,  577,  580,  361,  363,  364,  366,  367,
 /*  4550 */   624,  283,  605,  292,  295,  605,  605,  605,  605,  605,
 /*  4560 */   605,  605,  605,  605,  605,  344,  605,  605,  605,  605,
 /*  4570 */   605,  605,  277,  605,  605,  605,  605,  280,  354,  289,
 /*  4580 */   359,  286,  378,  274,  605,  369,  605,  605,  348,  605,
 /*  4590 */   605,  605,  605,  605,  605,  605,  605,  605,  385,  605,
 /*  4600 */   605,  605,  605,  368,  605,  338,  605,  605,  365,  605,
 /*  4610 */   605,   59,   60,   61,   62,   63,   64,   65,   66,   67,
 /*  4620 */    68,   69,   70,   71,   72,  605,  605,  605,  605,  231,
 /*  4630 */   210,  276,  282,  288,  291,  294,  297,  285,  279,  306,
 /*  4640 */   308,  316,  310,  312,  314,  605,  605,  605,  362,  605,
 /*  4650 */   605,  605,  207,  303,  299,  605,  361,  363,  364,  366,
 /*  4660 */   367,  625,  211,  212,  213,  214,  215,  216,  217,  218,
 /*  4670 */   219,  220,  221,  222,  223,  224,  225,  226,  227,  228,
 /*  4680 */   229,  230,  232,  605,  605,  605,   97,  605,  263,  100,
 /*  4690 */   622,  605,  605,  231,  210,  455,  605,  605,  605,  605,
 /*  4700 */   562,  564,  566,  568,   96,  392,  394,  393,  358,   90,
 /*  4710 */   605,  101,  605,  605,  605,  605,  241,  605,  605,  605,
 /*  4720 */   605,  605,  605,  605,  605,  625,  211,  212,  213,  214,
 /*  4730 */   215,  216,  217,  218,  219,  220,  221,  222,  223,  224,
 /*  4740 */   225,  226,  227,  228,  229,  230,  232,  231,  210,   99,
 /*  4750 */   605,  605,  605,  504,  622,  605,  605,   98,   87,   89,
 /*  4760 */    92,   91,  605,  605,  562,  564,  566,  568,  605,  480,
 /*  4770 */   245,  605,  605,  605,  605,  605,  605,  605,  605,  625,
 /*  4780 */   211,  212,  213,  214,  215,  216,  217,  218,  219,  220,
 /*  4790 */   221,  222,  223,  224,  225,  226,  227,  228,  229,  230,
 /*  4800 */   232,  231,  210,  605,  605,  605,  525,  605,  622,  605,
 /*  4810 */   481,  482,  483,  484,  485,  486,  605,  605,  562,  564,
 /*  4820 */   566,  568,  480,  605,  257,  605,  605,  605,  605,  605,
 /*  4830 */   605,  605,  605,  625,  211,  212,  213,  214,  215,  216,
 /*  4840 */   217,  218,  219,  220,  221,  222,  223,  224,  225,  226,
 /*  4850 */   227,  228,  229,  230,  232,  231,  210,  605,  605,  605,
 /*  4860 */   532,  605,  622,  481,  482,  483,  484,  485,  486,  605,
 /*  4870 */   605,  605,  562,  564,  566,  568,  480,  605,  260,  605,
 /*  4880 */   605,  605,  605,  605,  605,  605,  605,  625,  211,  212,
 /*  4890 */   213,  214,  215,  216,  217,  218,  219,  220,  221,  222,
 /*  4900 */   223,  224,  225,  226,  227,  228,  229,  230,  232,  231,
 /*  4910 */   210,  605,  605,  605,  543,  605,  622,  481,  482,  483,
 /*  4920 */   484,  485,  486,  605,  605,  605,  562,  564,  566,  568,
 /*  4930 */   480,  605,  265,  605,  605,  605,  605,  605,  605,  605,
 /*  4940 */   605,  625,  211,  212,  213,  214,  215,  216,  217,  218,
 /*  4950 */   219,  220,  221,  222,  223,  224,  225,  226,  227,  228,
 /*  4960 */   229,  230,  232,  231,  210,  605,  605,  605,  605,  605,
 /*  4970 */   622,  481,  482,  483,  484,  485,  486,  605,  605,  605,
 /*  4980 */   562,  564,  566,  568,  480,  605,  269,  605,  605,  605,
 /*  4990 */   605,  605,  605,  605,  605,  625,  211,  212,  213,  214,
 /*  5000 */   215,  216,  217,  218,  219,  220,  221,  222,  223,  224,
 /*  5010 */   225,  226,  227,  228,  229,  230,  232,  231,  210,  605,
 /*  5020 */   605,  605,  605,  605,  622,  481,  482,  483,  484,  485,
 /*  5030 */   486,  605,  605,  605,  562,  564,  566,  568,  605,  605,
 /*  5040 */   458,  605,  605,  605,  605,  605,  605,  605,  605,  625,
 /*  5050 */   211,  212,  213,  214,  215,  216,  217,  218,  219,  220,
 /*  5060 */   221,  222,  223,  224,  225,  226,  227,  228,  229,  230,
 /*  5070 */   232,  231,  210,  605,  605,  605,  605,  605,  622,  272,
 /*  5080 */   605,  605,  605,  605,  605,  605,  455,  605,  562,  564,
 /*  5090 */   566,  568,  605,  605,  465,  605,  392,  394,  393,  358,
 /*  5100 */   605,  605,  605,  625,  211,  212,  213,  214,  215,  216,
 /*  5110 */   217,  218,  219,  220,  221,  222,  223,  224,  225,  226,
 /*  5120 */   227,  228,  229,  230,  232,  231,  210,  605,  605,  605,
 /*  5130 */   605,  605,  622,  660,  605,  605,  605,  605,  605,  605,
 /*  5140 */   455,  605,  562,  564,  566,  568,  605,  605,  472,  605,
 /*  5150 */   392,  394,  393,  358,  605,  605,  605,  625,  211,  212,
 /*  5160 */   213,  214,  215,  216,  217,  218,  219,  220,  221,  222,
 /*  5170 */   223,  224,  225,  226,  227,  228,  229,  230,  232,  231,
 /*  5180 */   210,  605,  605,  605,  605,  605,  622,  605,  605,  605,
 /*  5190 */   605,  605,  605,  605,  605,  605,  562,  564,  566,  568,
 /*  5200 */   605,  605,  630,  605,  605,  605,  605,  605,  605,  605,
 /*  5210 */   605,  625,  211,  212,  213,  214,  215,  216,  217,  218,
 /*  5220 */   219,  220,  221,  222,  223,  224,  225,  226,  227,  228,
 /*  5230 */   229,  230,  232,  231,  210,  605,  605,  605,  605,  605,
 /*  5240 */   622,  605,  605,  605,  605,  605,  605,  605,  605,  605,
 /*  5250 */   562,  564,  566,  568,  605,  605,  637,  605,  605,  605,
 /*  5260 */   605,  605,  605,  605,  605,  625,  211,  212,  213,  214,
 /*  5270 */   215,  216,  217,  218,  219,  220,  221,  222,  223,  224,
 /*  5280 */   225,  226,  227,  228,  229,  230,  232,  231,  210,  605,
 /*  5290 */   605,  605,  605,  605,  622,  605,  605,  605,  605,  605,
 /*  5300 */   605,  605,  605,  605,  562,  564,  566,  568,  605,  605,
 /*  5310 */   643,  605,  605,  605,  605,  605,  605,  605,  605,  625,
 /*  5320 */   211,  212,  213,  214,  215,  216,  217,  218,  219,  220,
 /*  5330 */   221,  222,  223,  224,  225,  226,  227,  228,  229,  230,
 /*  5340 */   232,  231,  210,  605,  605,  605,  605,  605,  622,  605,
 /*  5350 */   605,  605,  605,  605,  605,  605,  605,  605,  562,  564,
 /*  5360 */   566,  568,  605,  605,  649,  605,  605,  605,  605,  605,
 /*  5370 */   605,  605,  605,  625,  211,  212,  213,  214,  215,  216,
 /*  5380 */   217,  218,  219,  220,  221,  222,  223,  224,  225,  226,
 /*  5390 */   227,  228,  229,  230,  232,  231,  210,  605,  605,  605,
 /*  5400 */   605,  605,  622,  605,  605,  605,  605,  605,  605,  605,
 /*  5410 */   605,  605,  562,  564,  566,  568,  605,  605,  666,  605,
 /*  5420 */   605,  605,  605,  605,  605,  605,  605,  625,  211,  212,
 /*  5430 */   213,  214,  215,  216,  217,  218,  219,  220,  221,  222,
 /*  5440 */   223,  224,  225,  226,  227,  228,  229,  230,  232,  231,
 /*  5450 */   210,  605,  605,  605,  605,  605,  622,  605,  605,  605,
 /*  5460 */   605,  605,  605,  605,  605,  605,  562,  564,  566,  568,
 /*  5470 */   605,  605,  672,  605,  605,  605,  605,  605,  605,  605,
 /*  5480 */   605,  625,  211,  212,  213,  214,  215,  216,  217,  218,
 /*  5490 */   219,  220,  221,  222,  223,  224,  225,  226,  227,  228,
 /*  5500 */   229,  230,  232,  231,  210,  605,  605,  605,  605,  605,
 /*  5510 */   622,  605,  605,  605,  605,  605,  605,  605,  605,  605,
 /*  5520 */   562,  564,  566,  568,  605,  605,  679,  605,  605,  605,
 /*  5530 */   605,  605,  605,  605,  605,  625,  211,  212,  213,  214,
 /*  5540 */   215,  216,  217,  218,  219,  220,  221,  222,  223,  224,
 /*  5550 */   225,  226,  227,  228,  229,  230,  232,  231,  210,  605,
 /*  5560 */   605,  605,  605,  605,  622,  605,  605,  605,  605,  605,
 /*  5570 */   605,  605,  605,  605,  562,  564,  566,  568,  605,  605,
 /*  5580 */   685,  605,  605,  605,  605,  605,  605,  605,  605,  625,
 /*  5590 */   211,  212,  213,  214,  215,  216,  217,  218,  219,  220,
 /*  5600 */   221,  222,  223,  224,  225,  226,  227,  228,  229,  230,
 /*  5610 */   232,  231,  210,  605,  605,  605,  605,  605,  622,  605,
 /*  5620 */   605,  605,  605,  605,  605,  605,  605,  605,  562,  564,
 /*  5630 */   566,  568,  605,  605,  691,  605,  605,  605,  605,  605,
 /*  5640 */   605,  605,  605,  625,  211,  212,  213,  214,  215,  216,
 /*  5650 */   217,  218,  219,  220,  221,  222,  223,  224,  225,  226,
 /*  5660 */   227,  228,  229,  230,  232,  231,  210,  605,  605,  605,
 /*  5670 */   605,  605,  622,  605,  605,  605,  605,  605,  605,  605,
 /*  5680 */   605,  605,  562,  564,  566,  568,  605,  605,  701,  605,
 /*  5690 */   605,  605,  605,  605,  605,  605,  605,  625,  211,  212,
 /*  5700 */   213,  214,  215,  216,  217,  218,  219,  220,  221,  222,
 /*  5710 */   223,  224,  225,  226,  227,  228,  229,  230,  232,  231,
 /*  5720 */   210,  605,  605,  605,  605,  605,  622,  605,  605,  605,
 /*  5730 */   605,  605,  605,  605,  605,  605,  562,  564,  566,  568,
 /*  5740 */   605,  605,  707,  605,  605,  605,  605,  605,  605,  605,
 /*  5750 */   605,  625,  211,  212,  213,  214,  215,  216,  217,  218,
 /*  5760 */   219,  220,  221,  222,  223,  224,  225,  226,  227,  228,
 /*  5770 */   229,  230,  232,  231,  210,  605,  605,  605,  605,  605,
 /*  5780 */   622,  605,  605,  605,  605,  605,  605,  605,  605,  605,
 /*  5790 */   562,  564,  566,  568,  605,  605,  713,  605,  605,  605,
 /*  5800 */   605,  605,  605,  605,  605,  625,  211,  212,  213,  214,
 /*  5810 */   215,  216,  217,  218,  219,  220,  221,  222,  223,  224,
 /*  5820 */   225,  226,  227,  228,  229,  230,  232,  231,  210,  605,
 /*  5830 */   605,  605,  605,   97,  622,  605,   93,  605,  605,  605,
 /*  5840 */   605,  605,  605,  605,  562,  564,  566,  568,  605,  605,
 /*  5850 */   719,   96,  605,  605,  605,  605,   90,  605,  605,  625,
 /*  5860 */   211,  212,  213,  214,  215,  216,  217,  218,  219,  220,
 /*  5870 */   221,  222,  223,  224,  225,  226,  227,  228,  229,  230,
 /*  5880 */   232,  231,  210,  605,  605,  605,  605,  605,  622,  605,
 /*  5890 */   605,  605,  605,  605,  605,  605,   88,  605,  562,  564,
 /*  5900 */   566,  568,  605,  605,   86,   87,   89,   92,   91,  605,
 /*  5910 */   605,  605,  605,  209,  211,  212,  213,  214,  215,  216,
 /*  5920 */   217,  218,  219,  220,  221,  222,  223,  224,  225,  226,
 /*  5930 */   227,  228,  229,  230,  232,  605,  605,  605,  605,  605,
 /*  5940 */   605,  605,  622,  605,  605,   30,  605,   32,  605,  182,
 /*  5950 */   605,   35,  562,  564,  566,  568,  183,  605,  170,  605,
 /*  5960 */   605,  334,  168,  605,  332,  320,  318,  324,  322,  326,
 /*  5970 */   328,  330,  276,  282,  288,  291,  294,  297,  285,  279,
 /*  5980 */   306,  308,  316,  310,  312,  314,  605,  605,  605,  605,
 /*  5990 */   605,  605,  605,  334,  303,  299,  332,  320,  318,  324,
 /*  6000 */   322,  326,  328,  330,  276,  282,  288,  291,  294,  297,
 /*  6010 */   285,  279,  306,  308,  316,  310,  312,  314,  793,  305,
 /*  6020 */   605,  605,  605,  605,  605,  334,  303,  299,  332,  320,
 /*  6030 */   318,  324,  322,  326,  328,  330,  276,  282,  288,  291,
 /*  6040 */   294,  297,  285,  279,  306,  308,  316,  310,  312,  314,
 /*  6050 */   605,  494,  605,  605,  605,  605,  605,  340,  303,  299,
 /*  6060 */   605,   10,   18,  605,   13,  605,   22,  480,  184,  605,
 /*  6070 */   771,  784,  336,  605,  623,  605,  605,  605,  605,  605,
 /*  6080 */   605,  334,  605,  605,  332,  320,  318,  324,  322,  326,
 /*  6090 */   328,  330,  276,  282,  288,  291,  294,  297,  285,  279,
 /*  6100 */   306,  308,  316,  310,  312,  314,  605,  605,  481,  482,
 /*  6110 */   483,  484,  485,  486,  303,  299,  510,  511,  605,  605,
 /*  6120 */   605,  605,  605,  605,  605,  605,  605,  471,  605,  605,
 /*  6130 */   605,  605,  605,  605,  605,  624,  334,  605,  605,  332,
 /*  6140 */   320,  318,  324,  322,  326,  328,  330,  276,  282,  288,
 /*  6150 */   291,  294,  297,  285,  279,  306,  308,  316,  310,  312,
 /*  6160 */   314,  605,  605,  605,  605,  605,  605,  605,  334,  303,
 /*  6170 */   299,  332,  320,  318,  324,  322,  326,  328,  330,  276,
 /*  6180 */   282,  288,  291,  294,  297,  285,  279,  306,  308,  316,
 /*  6190 */   310,  312,  314,  605, 1038,  605,  605,  605,  605,  605,
 /*  6200 */   605,  303,  299,  334,  605,  572,  332,  320,  318,  324,
 /*  6210 */   322,  326,  328,  330,  276,  282,  288,  291,  294,  297,
 /*  6220 */   285,  279,  306,  308,  316,  310,  312,  314,  605,  605,
 /*  6230 */   605,  605,  605,  605,  605,  605,  303,  299,  605,  605,
 /*  6240 */   576,  605,  605,  605,  605,  334,  605,  605,  332,  320,
 /*  6250 */   318,  324,  322,  326,  328,  330,  276,  282,  288,  291,
 /*  6260 */   294,  297,  285,  279,  306,  308,  316,  310,  312,  314,
 /*  6270 */   605,  605,  605,  605,  605,  605,  605,  605,  303,  299,
 /*  6280 */   605,  605,  579,  605,  605,  605,  605,  334,  605,  605,
 /*  6290 */   332,  320,  318,  324,  322,  326,  328,  330,  276,  282,
 /*  6300 */   288,  291,  294,  297,  285,  279,  306,  308,  316,  310,
 /*  6310 */   312,  314,  605,  144,  145,  147,  146,  148,  605,  605,
 /*  6320 */   303,  299,  605,  605,  582,  605,  605,  605,  605,  334,
 /*  6330 */   605,  605,  332,  320,  318,  324,  322,  326,  328,  330,
 /*  6340 */   276,  282,  288,  291,  294,  297,  285,  279,  306,  308,
 /*  6350 */   316,  310,  312,  314,  605,  605,  605,  605,  605,  605,
 /*  6360 */   605,  605,  303,  299,  150,  151,  605,   29,  171,  605,
 /*  6370 */   177,  605,  605,  605,  149,  629,  144,  145,  147,  146,
 /*  6380 */   148,  605,  605,  605,  334,  605,  605,  332,  320,  318,
 /*  6390 */   324,  322,  326,  328,  330,  276,  282,  288,  291,  294,
 /*  6400 */   297,  285,  279,  306,  308,  316,  310,  312,  314,  605,
 /*  6410 */   605,  605,  605,  370,  605,  605,  605,  303,  299,  728,
 /*  6420 */   605,  605,  605,  392,  394,  393,  358,  150,  151,  375,
 /*  6430 */   636,  605,  422,  418,  421,  695,  605,  149,  605,  334,
 /*  6440 */   605,  605,  332,  320,  318,  324,  322,  326,  328,  330,
 /*  6450 */   276,  282,  288,  291,  294,  297,  285,  279,  306,  308,
 /*  6460 */   316,  310,  312,  314,  605,  605,  605,  605,  605,  605,
 /*  6470 */   605,  605,  303,  299,  144,  145,  147,  146,  148,  605,
 /*  6480 */   605,  605,  605,   10,   18,  642,   13,  605,   22,  605,
 /*  6490 */   184,  605,  771,  784,  334,  605,  623,  332,  320,  318,
 /*  6500 */   324,  322,  326,  328,  330,  276,  282,  288,  291,  294,
 /*  6510 */   297,  285,  279,  306,  308,  316,  310,  312,  314,  283,
 /*  6520 */   605,  292,  295,  605,  605,  150,  151,  303,  299,  605,
 /*  6530 */   605,  172,  605,  344,  605,  149,  605,  605,  605,  605,
 /*  6540 */   277,  605,  605,  605,  605,  280,  354,  289,  359,  286,
 /*  6550 */   378,  274,  434,  369,  605,  605,  352,  624,  283,  605,
 /*  6560 */   292,  295,  605,  605,  605,  605,  385,  605,  605,  605,
 /*  6570 */   605,  368,  344,  338,  605,  605,  365,  605,  605,  277,
 /*  6580 */   605,  605,  605,  605,  280,  354,  289,  359,  286,  378,
 /*  6590 */   274,  430,  369,  605,  605,  352,  605,  605,  605,  605,
 /*  6600 */   605,  605,  605,  605,  605,  385,  605,  605,  605,  605,
 /*  6610 */   368,  605,  338,  605,  605,  365,  362,  144,  145,  147,
 /*  6620 */   146,  148,  605,  605,  361,  363,  364,  366,  367,  320,
 /*  6630 */   318,  324,  322,  326,  328,  330,  276,  282,  288,  291,
 /*  6640 */   294,  297,  285,  279,  306,  308,  316,  310,  312,  314,
 /*  6650 */   605,  605,  605,  605,  605,  362,  605,  605,  303,  299,
 /*  6660 */   748,  605,  605,  361,  363,  364,  366,  367,  150,  151,
 /*  6670 */   605,  283,  605,  292,  295,  605,  201,  605,  149,  605,
 /*  6680 */   605,  605,  605,  605,  605,  344,  605,  605,  605,  605,
 /*  6690 */   605,  605,  277,  605,  605,  605,  605,  280,  354,  289,
 /*  6700 */   359,  286,  378,  274,  425,  369,  605,  605,  352,  605,
 /*  6710 */   283,  605,  292,  295,  605,  605,  605,  605,  385,  605,
 /*  6720 */   605,  605,  605,  368,  344,  338,  605,  605,  365,  605,
 /*  6730 */   605,  277,  605,  605,  605,  605,  280,  354,  289,  359,
 /*  6740 */   286,  378,  274,  605,  369,  605,  605,  371,  605,  605,
 /*  6750 */   605,  605,  605,  605,  605,  605,  605,  385,  605,  605,
 /*  6760 */   605,  605,  368,  605,  338,  605,  605,  365,  362,  374,
 /*  6770 */   144,  145,  147,  146,  148,  605,  361,  363,  364,  366,
 /*  6780 */   367,  332,  320,  318,  324,  322,  326,  328,  330,  276,
 /*  6790 */   282,  288,  291,  294,  297,  285,  279,  306,  308,  316,
 /*  6800 */   310,  312,  314,  605,  605,  605,  605,  373,  605,  605,
 /*  6810 */   605,  303,  299,  605,  605,  372,  363,  364,  366,  367,
 /*  6820 */   605,  150,  151,  283,  605,  292,  295,  605,  605,   36,
 /*  6830 */   605,  149,  605,  605,  605,  605,  605,  344,  605,  605,
 /*  6840 */   605,  605,  605,  605,  277,  605,  605,  605,  605,  280,
 /*  6850 */   354,  289,  359,  286,  378,  274,  381,  369,  605,  605,
 /*  6860 */   352,  605,  283,  605,  292,  295,  605,  605,  605,  605,
 /*  6870 */   385,  605,  605,  605,  605,  368,  344,  338,  605,  605,
 /*  6880 */   365,  605,  605,  277,  605,  605,  605,  605,  280,  354,
 /*  6890 */   289,  359,  286,  378,  274,  391,  369,  605,  605,  352,
 /*  6900 */   605,  605,  605,  605,  605,  605,  605,  605,  605,  385,
 /*  6910 */   605,  605,  605,  605,  368,  605,  338,  605,  605,  365,
 /*  6920 */   362,  605,  144,  145,  147,  146,  148,  605,  361,  363,
 /*  6930 */   364,  366,  367,  318,  324,  322,  326,  328,  330,  276,
 /*  6940 */   282,  288,  291,  294,  297,  285,  279,  306,  308,  316,
 /*  6950 */   310,  312,  314,  605,  605,  605,  605,  605,  605,  362,
 /*  6960 */   605,  303,  299,  605,  605,  605,  605,  361,  363,  364,
 /*  6970 */   366,  367,  605,  150,  151,  283,  605,  292,  295,  605,
 /*  6980 */   605,  154,  605,  149,  605,  605,  605,  605,  605,  344,
 /*  6990 */   605,  605,  605,  605,  605,  605,  277,  605,  605,  605,
 /*  7000 */   605,  280,  354,  289,  359,  286,  378,  274,  401,  369,
 /*  7010 */   605,  605,  352,  605,  283,  605,  292,  295,  605,  605,
 /*  7020 */   605,  605,  385,  605,  605,  605,  605,  368,  344,  338,
 /*  7030 */   605,  605,  365,  605,  605,  277,  605,  605,  605,  605,
 /*  7040 */   280,  354,  289,  359,  286,  378,  274,  408,  369,  605,
 /*  7050 */   605,  352,  605,  605,  605,  605,  605,  605,  605,  605,
 /*  7060 */   605,  385,  605,  605,  605,  605,  368,  605,  338,  605,
 /*  7070 */   605,  365,  362,  605,  144,  145,  147,  146,  148,  605,
 /*  7080 */   361,  363,  364,  366,  367,  605,  324,  322,  326,  328,
 /*  7090 */   330,  276,  282,  288,  291,  294,  297,  285,  279,  306,
 /*  7100 */   308,  316,  310,  312,  314,  605,  605,  605,  605,  605,
 /*  7110 */   605,  362,  605,  303,  299,  605,  605,  605,  605,  361,
 /*  7120 */   363,  364,  366,  367,  605,  150,  151,  283,  605,  292,
 /*  7130 */   295,  605,  605,  201,  605,  149,  605,  605,  605,  605,
 /*  7140 */   605,  344,  605,  605,  605,  605,  605,  605,  277,  605,
 /*  7150 */   605,  605,  605,  280,  354,  289,  359,  286,  378,  274,
 /*  7160 */   414,  369,  605,  605,  352,  605,  283,  605,  292,  295,
 /*  7170 */   605,  605,  605,  605,  385,  605,  605,  605,  605,  368,
 /*  7180 */   344,  338,  605,  605,  365,  605,  605,  277,  605,  605,
 /*  7190 */   605,  128,  280,  354,  289,  359,  286,  378,  274,  441,
 /*  7200 */   369,  605,  605,  352,  605,  144,  145,  147,  146,  148,
 /*  7210 */    39,  605,  605,  385,   83,  605,  605,  605,  368,  605,
 /*  7220 */   338,  605,  605,  365,  362,  605,  605,  605,  112,  605,
 /*  7230 */   605,  605,  361,  363,  364,  366,  367,   59,   60,   61,
 /*  7240 */    62,   63,   64,   65,   66,   67,   68,   69,   70,   71,
 /*  7250 */    72,  605,  605,  605,  605,  605,  150,  151,  605,  836,
 /*  7260 */   171,  605,  177,  362,  605,  605,  149,  605,  605,  605,
 /*  7270 */   605,  361,  363,  364,  366,  367,  370,  605,  605,  283,
 /*  7280 */   605,  292,  295,  605,  605,  605,  392,  394,  393,  358,
 /*  7290 */   605,  605,  605,  344,  605,  417,  418,  421,  605,  605,
 /*  7300 */   277,  605,  605,  605,  605,  280,  354,  289,  359,  286,
 /*  7310 */   378,  274,  447,  369,  605,  605,  352,  605,  283,  605,
 /*  7320 */   292,  295,  605,  605,  605,  605,  385,  605,  605,  605,
 /*  7330 */   605,  368,  344,  338,  605,  605,  365,  605,  605,  277,
 /*  7340 */   605,  605,  605,  128,  280,  354,  289,  359,  286,  378,
 /*  7350 */   274,  605,  369,  605,  605,  348,  605,  144,  145,  147,
 /*  7360 */   146,  148,  157,  605,  605,  385,   83,  605,  605,  605,
 /*  7370 */   368,  605,  338,  605,  605,  365,  362,  605,  605,  605,
 /*  7380 */   112,  605,  605,  605,  361,  363,  364,  366,  367,   59,
 /*  7390 */    60,   61,   62,   63,   64,   65,   66,   67,   68,   69,
 /*  7400 */    70,   71,   72,  605,  605,  605,  605,  627,  150,  151,
 /*  7410 */   605,  194,  726,  605,  177,  362,  605,  605,  149,  605,
 /*  7420 */   605,  605,  605,  361,  363,  364,  366,  367,  605,  605,
 /*  7430 */   605,  283,  605,  292,  295,  605,  605,  605,  605,  605,
 /*  7440 */   605,  605,  605,  605,  605,  344,  605,  605,  605,  605,
 /*  7450 */   605,  605,  277,  605,  605,  605,  605,  280,  354,  289,
 /*  7460 */   359,  286,  378,  274,  605,  369,  605,  605,  348,  605,
 /*  7470 */   283,  605,  292,  295,  605,  605,  605,  605,  385,  605,
 /*  7480 */   605,  605,  605,  368,  344,  338,  605,  605,  365,  605,
 /*  7490 */   495,  277,  605,  605,  605,  605,  280,  354,  289,  359,
 /*  7500 */   286,  378,  274,  605,  369,   53,  605,  348,  605,  605,
 /*  7510 */   605,  605,  605,  605,  605,  605,  605,  385,  605,  605,
 /*  7520 */   605,  605,  368,  605,  338,  605,  605,  365,  362,  505,
 /*  7530 */   536,  490,  605,  605,  605,  605,  361,  363,  364,  366,
 /*  7540 */   367,  605,  605,  605,  605,  605,  480,   43,   49,   50,
 /*  7550 */   605,   59,   60,   61,   62,   63,   64,   65,   66,   67,
 /*  7560 */    68,   69,   70,   71,   72,  605,  605,  362,  605,  605,
 /*  7570 */   605,  605,  605,  605,  605,  361,  363,  364,  366,  367,
 /*  7580 */   605,  605,  605,  283,  605,  292,  295,  481,  482,  483,
 /*  7590 */   484,  485,  486,  605,  521,  547,  548,  344,  605,  605,
 /*  7600 */   605,  605,  605,  605,  277,  605,  605,  605,  605,  280,
 /*  7610 */   354,  289,  359,  286,  378,  274,  605,  369,  605,  605,
 /*  7620 */   348,  605,  283,  605,  292,  295,  605,  605,  605,  605,
 /*  7630 */   385,  605,  605,  605,  605,  368,  344,  338,  605,  605,
 /*  7640 */   365,  605,  526,  277,  605,  605,  605,  128,  280,  354,
 /*  7650 */   289,  359,  286,  378,  274,  605,  369,  605,  605,  348,
 /*  7660 */   605,  144,  145,  147,  146,  148,  204,  605,  605,  385,
 /*  7670 */    83,  605,  605,  605,  368,  605,  338,  605,  605,  365,
 /*  7680 */   362,  533,  605,  605,  112,  605,  605,  605,  361,  363,
 /*  7690 */   364,  366,  367,   59,   60,   61,   62,   63,   64,   65,
 /*  7700 */    66,   67,   68,   69,   70,   71,   72,  144,  145,  147,
 /*  7710 */   146,  148,  150,  151,  605,  827,  726,  605,  177,  362,
 /*  7720 */   605,  605,  149,  605,  605,  605,  605,  361,  363,  364,
 /*  7730 */   366,  367,  605,  605,  605,  283,  605,  292,  295,  605,
 /*  7740 */   605,  605,  605,  605,  605,  605,  605,  605,  605,  344,
 /*  7750 */   605,  605,  605,  605,  605,  605,  277,  605,  150,  151,
 /*  7760 */   605,  280,  354,  289,  359,  286,  378,  274,  149,  369,
 /*  7770 */   605,  605,  348,  605,  283,  605,  292,  295,  605,  605,
 /*  7780 */   605,  605,  385,  605,  605,  605,  605,  368,  344,  338,
 /*  7790 */   605,  605,  365,  605,  537,  277,  605,  605,  605,  128,
 /*  7800 */   280,  354,  289,  359,  286,  378,  274,  605,  369,  605,
 /*  7810 */   605,  348,  605,  144,  145,  147,  146,  148,  698,  605,
 /*  7820 */   605,  385,   83,  605,  605,  605,  368,  605,  338,  605,
 /*  7830 */   605,  365,  362,  544,  605,  605,  112,  605,  605,  605,
 /*  7840 */   361,  363,  364,  366,  367,   59,   60,   61,   62,   63,
 /*  7850 */    64,   65,   66,   67,   68,   69,   70,   71,   72,  605,
 /*  7860 */   605,  605,  605,  605,  150,  151,  605,  831,  724,  605,
 /*  7870 */   177,  362,  605,  605,  149,  605,  605,  605,  605,  361,
 /*  7880 */   363,  364,  366,  367,  605,  605,  605,  283,  605,  292,
 /*  7890 */   295,  605,  605,  605,  605,  605,  605,  605,  605,  605,
 /*  7900 */   605,  344,  605,  605,  605,  605,  605,  605,  277,  605,
 /*  7910 */   605,  605,  605,  280,  354,  289,  359,  286,  378,  274,
 /*  7920 */   605,  369,  605,  605,  348,  573,  283,  605,  292,  295,
 /*  7930 */   605,  605,  605,  605,  385,  605,  605,  605,  605,  368,
 /*  7940 */   344,  338,  605,  605,  365,  605,  605,  277,  605,  605,
 /*  7950 */   605,  128,  280,  354,  289,  359,  286,  378,  274,  605,
 /*  7960 */   369,  605,  605,  348,  605,  144,  145,  147,  146,  148,
 /*  7970 */   605,  605,  605,  385,   83,  605,  605,  605,  368,  605,
 /*  7980 */   338,  605,  605,  365,  362,  605,  605,  605,  112,  605,
 /*  7990 */   605,  605,  361,  363,  364,  366,  367,   59,   60,   61,
 /*  8000 */    62,   63,   64,   65,   66,   67,   68,   69,   70,   71,
 /*  8010 */    72,  605,  605,  605,  605,  640,  150,  151,  605,  828,
 /*  8020 */   726,  605,  177,  362,  605,  605,  149,  605,  605,  605,
 /*  8030 */   605,  361,  363,  364,  366,  367,  605,  605,  605,  283,
 /*  8040 */   605,  292,  295,  605,  605,  605,  605,  605,  605,  605,
 /*  8050 */   605,  605,  605,  344,  605,  605,  605,  605,  605,  605,
 /*  8060 */   277,  605,  605,  605,  605,  280,  354,  289,  359,  286,
 /*  8070 */   378,  274,  605,  369,  605,  605,  348,  605,  283,  605,
 /*  8080 */   292,  295,  605,  605,  605,  605,  385,  605,  605,  605,
 /*  8090 */   605,  368,  344,  338,  605,  605,  365,  605,  605,  277,
 /*  8100 */   605,  605,  605,  605,  280,  354,  289,  359,  286,  378,
 /*  8110 */   274,  605,  369,   53,  605,  371,  605,  605,  605,  605,
 /*  8120 */   605,  605,  605,  605,  605,  385,  605,  605,  605,  605,
 /*  8130 */   368,  605,  338,  605,  605,  365,  362,  144,  145,  147,
 /*  8140 */   146,  148,  605,  605,  361,  363,  364,  366,  367,  144,
 /*  8150 */   145,  147,  146,  148,  605,  605,   49,   50,  605,   59,
 /*  8160 */    60,   61,   62,   63,   64,   65,   66,   67,   68,   69,
 /*  8170 */    70,   71,   72,  605,  605,  373,  605,  605,  605,  605,
 /*  8180 */   605,  605,  605,  372,  363,  364,  366,  367,  150,  151,
 /*  8190 */   605,  283,  605,  292,  295,  605,  695,  605,  149,  605,
 /*  8200 */   150,  151,  605,  838,  152,  344,  128,  605,   97,  605,
 /*  8210 */   149,  100,  277,  605,  605,  605,  605,  280,  354,  289,
 /*  8220 */   359,  286,  378,  274,  605,  369,   96,  605,  352,  113,
 /*  8230 */   605,   90,  605,  605,  605,  605,  605,  605,  385,  605,
 /*  8240 */   605,  605,  605,  368,  605,  338,  605,  605,  365,  605,
 /*  8250 */   605,  605,   59,   60,   61,   62,   63,   64,   65,   66,
 /*  8260 */    67,   68,   69,   70,   71,   72,  144,  145,  147,  146,
 /*  8270 */   148,   99,  605,  605,  605,  605,  605,  605,  605,   98,
 /*  8280 */    87,   89,   92,   91,  605,  605,  605,  605,  362,  605,
 /*  8290 */   144,  145,  147,  146,  148,  605,  361,  363,  364,  366,
 /*  8300 */   367,  144,  145,  147,  146,  148,  144,  145,  147,  146,
 /*  8310 */   148,  605,  605,  605,  605,  605,  605,  150,  151,  605,
 /*  8320 */   837,  152,  144,  145,  147,  146,  148,  149,  605,  605,
 /*  8330 */   605,  605,  605,  605,  605,  605,  605,  605,  605,  605,
 /*  8340 */   605,  150,  151,  605,  830,  693,  144,  145,  147,  146,
 /*  8350 */   148,  149,  150,  151,  605,  834,  693,  150,  151,  605,
 /*  8360 */   832,  760,  149,  605,  605,  605,  605,  149,  605,  605,
 /*  8370 */   605,  605,  605,  150,  151,  605,  835,  693,  144,  145,
 /*  8380 */   147,  146,  148,  149,  605,  605,  605,  605,  605,  605,
 /*  8390 */   605,  605,  605,  605,  605,  605,  605,  150,  151,  605,
 /*  8400 */   833,  693,  605,  605,  605,  605,  605,  149,  605,  605,
 /*  8410 */   605,  605,  605,  605,  605,  605,  605,  605,  605,  605,
 /*  8420 */   605,  605,  605,  605,  605,  605,  605,  605,  605,  150,
 /*  8430 */   151,  605,  829,  693,  605,  605,  605,  605,  605,  149,
};
static YYCODETYPE yy_lookahead[] = {
 /*     0 */     7,  142,    9,   10,  119,  120,  121,  122,  123,  124,
 /*    10 */   125,  126,  127,  128,   21,    0,  149,  154,  155,  156,
 /*    20 */   157,   28,  155,  156,   44,  158,   33,   34,   35,   36,
 /*    30 */    37,   38,   39,   61,   41,   55,   56,   44,   45,   22,
 /*    40 */    55,   27,   28,   29,   30,   31,   32,   54,   55,   56,
 /*    50 */   155,  156,   59,  158,   61,   41,   42,   64,   41,   87,
 /*    60 */    67,   68,   69,   70,   71,   72,   73,   74,   75,   76,
 /*    70 */    77,   86,  213,   88,   81,   82,   83,  105,   85,  155,
 /*    80 */   156,  188,   89,   90,   91,   92,  138,   94,  140,  141,
 /*    90 */    97,  198,  199,  200,  201,  147,    6,  104,  205,  206,
 /*   100 */   152,  108,  109,  110,  111,  112,  113,  114,  115,  116,
 /*   110 */   117,    7,  124,    9,   10,  133,   49,  129,  136,  137,
 /*   120 */   138,   54,  140,  141,  149,   21,   44,    6,  146,  147,
 /*   130 */   155,  156,   28,  158,  152,   45,    6,   33,   34,   35,
 /*   140 */    36,   37,   38,   39,   14,   41,    6,   61,   44,   45,
 /*   150 */   122,  123,  124,  125,  126,  127,  128,   55,   54,   55,
 /*   160 */    56,   40,    6,   59,  142,   61,   44,  191,   64,  193,
 /*   170 */   194,   67,   68,   69,   70,   71,   72,   73,   74,   75,
 /*   180 */    76,   77,  182,  183,   54,   81,   82,   83,   86,   85,
 /*   190 */    88,  105,  188,   89,   90,   91,   92,  191,   94,  193,
 /*   200 */   194,   97,  198,  199,  200,  201,   66,   45,  104,  205,
 /*   210 */   206,   55,  108,  109,  110,  111,  112,  113,  114,  115,
 /*   220 */   116,  117,    7,   87,    9,   10,  151,  188,  153,  154,
 /*   230 */   155,  156,  157,  211,  212,  213,   21,  198,  199,  200,
 /*   240 */   201,  105,   39,   28,  205,  206,  141,   44,   33,   34,
 /*   250 */    35,   36,   37,   38,   39,   61,   41,  152,   42,   44,
 /*   260 */    45,   45,   42,  137,  138,   45,  140,  141,   45,   54,
 /*   270 */    55,   56,  146,  147,   59,  124,   61,   54,  152,   64,
 /*   280 */    57,   87,   67,   68,   69,   70,   71,   72,   73,   74,
 /*   290 */    75,   76,   77,  132,   47,  134,   81,   82,   83,  105,
 /*   300 */    85,   41,   42,  188,   89,   90,   91,   92,   44,   94,
 /*   310 */     6,  141,   97,  198,  199,  200,  201,  147,   54,  104,
 /*   320 */   205,  206,  152,  108,  109,  110,  111,  112,  113,  114,
 /*   330 */   115,  116,  117,    7,   44,    9,   10,  151,  188,  153,
 /*   340 */   154,  155,  156,  157,   40,    6,  141,   21,  198,  199,
 /*   350 */   200,  201,    6,  148,   28,  205,  206,  152,    6,   33,
 /*   360 */    34,   35,   36,   37,   38,   39,  188,   41,   39,   44,
 /*   370 */    44,   45,  191,   44,  193,  194,  198,  199,  200,  201,
 /*   380 */    54,   55,   56,  205,  206,   59,   42,   61,  130,   45,
 /*   390 */    64,   45,   42,   67,   68,   69,   70,   71,   72,   73,
 /*   400 */    74,   75,   76,   77,   54,   66,   54,   81,   82,   83,
 /*   410 */    45,   85,   61,   44,  188,   89,   90,   91,   92,   54,
 /*   420 */    94,   42,   57,   97,  198,  199,  200,  201,   59,  104,
 /*   430 */   104,  205,  206,   54,  108,  109,  110,  111,  112,  113,
 /*   440 */   114,  115,  116,  117,    7,   44,    9,   10,  151,  188,
 /*   450 */   153,  154,  155,  156,  157,   54,  105,  141,   21,  198,
 /*   460 */   199,  200,  201,  147,   44,   28,  205,  206,  152,   54,
 /*   470 */    33,   34,   35,   36,   37,   38,   39,  188,   41,   44,
 /*   480 */    42,   44,   45,   45,  184,  185,   44,  198,  199,  200,
 /*   490 */   201,   54,   55,   56,  205,  206,   59,   49,   61,   51,
 /*   500 */    51,   64,   54,   54,   67,   68,   69,   70,   71,   72,
 /*   510 */    73,   74,   75,   76,   77,  130,  186,  187,   81,   82,
 /*   520 */    83,    6,   85,   44,  104,  188,   89,   90,   91,   92,
 /*   530 */     6,   94,   55,   54,   97,  198,  199,  200,  201,  152,
 /*   540 */     6,  104,  205,  206,    6,  108,  109,  110,  111,  112,
 /*   550 */   113,  114,  115,  116,  117,    7,    6,    9,   10,  138,
 /*   560 */     6,  136,  141,  138,   40,  140,  141,  146,  147,   21,
 /*   570 */    49,  146,  147,  152,   40,   54,   28,  152,   40,    6,
 /*   580 */     6,   33,   34,   35,   36,   37,   38,   39,  188,   41,
 /*   590 */    40,   44,   44,   45,   40,   44,  196,  197,  198,  199,
 /*   600 */   200,  201,   54,   55,   56,   54,   61,   59,  191,   61,
 /*   610 */    95,  194,   64,   40,   40,   67,   68,   69,   70,   71,
 /*   620 */    72,   73,   74,   75,   76,   77,   44,  144,  145,   81,
 /*   630 */    82,   83,  141,   85,  189,  190,   54,   89,   90,   91,
 /*   640 */    92,   42,   94,  152,  151,   97,  153,  154,  155,  156,
 /*   650 */   157,  191,  104,   54,  194,   45,  108,  109,  110,  111,
 /*   660 */   112,  113,  114,  115,  116,  117,    7,    6,    9,   10,
 /*   670 */   151,  181,  153,  154,  155,  156,  157,    6,  188,  151,
 /*   680 */    21,  153,  154,  155,  156,  157,   14,   28,  198,  199,
 /*   690 */   200,  201,   33,   34,   35,   36,   37,   38,   39,  188,
 /*   700 */    41,   40,   39,   44,   45,    6,  202,  203,   44,  198,
 /*   710 */   199,  200,  201,   54,   55,   56,   45,  206,   59,   49,
 /*   720 */    61,   51,  191,   64,   54,  194,   67,   68,   69,   70,
 /*   730 */    71,   72,   73,   74,   75,   76,   77,  202,  203,   40,
 /*   740 */    81,   82,   83,   42,   85,  202,  203,  188,   89,   90,
 /*   750 */    91,   92,   45,   94,  195,   54,   97,  198,  199,  200,
 /*   760 */   201,   54,   66,  104,  202,  203,   22,  108,  109,  110,
 /*   770 */   111,  112,  113,  114,  115,  116,  117,    7,    6,    9,
 /*   780 */    10,  151,  181,  153,  154,  155,  156,  157,  105,  188,
 /*   790 */   151,   21,  153,  154,  155,  156,  157,    6,   28,  198,
 /*   800 */   199,  200,  201,   33,   34,   35,   36,   37,   38,   39,
 /*   810 */   188,   41,   40,   45,   44,   45,   30,   31,   32,  197,
 /*   820 */   198,  199,  200,  201,   54,   55,   56,   41,   42,   59,
 /*   830 */     6,   61,    6,   57,   64,  142,   45,   67,   68,   69,
 /*   840 */    70,   71,   72,   73,   74,   75,   76,   77,  202,  203,
 /*   850 */    87,   81,   82,   83,   59,   85,    6,  202,  203,   89,
 /*   860 */    90,   91,   92,  139,   94,  141,  141,   97,    6,   45,
 /*   870 */   146,   45,  148,  132,  104,  134,  152,  152,  108,  109,
 /*   880 */   110,  111,  112,  113,  114,  115,  116,  117,    7,    6,
 /*   890 */     9,   10,  149,  188,  132,   45,  134,  192,  155,  156,
 /*   900 */   188,  158,   21,  198,  199,  200,  201,   45,    6,   28,
 /*   910 */   198,  199,  200,  201,   33,   34,   35,   36,   37,   38,
 /*   920 */    39,  209,   41,  149,   87,   44,   45,    6,   45,  155,
 /*   930 */   156,   57,  158,  202,  203,   54,   55,   56,  202,  203,
 /*   940 */    59,  142,   61,    6,    6,   64,   87,   45,   67,   68,
 /*   950 */    69,   70,   71,   72,   73,   74,   75,   76,   77,  202,
 /*   960 */   203,   40,   81,   82,   83,   45,   85,    6,   57,  188,
 /*   970 */    89,   90,   91,   92,   54,   94,  142,   40,   97,  198,
 /*   980 */   199,  200,  201,   45,  142,  104,  202,  203,   44,  108,
 /*   990 */   109,  110,  111,  112,  113,  114,  115,  116,  117,    7,
 /*  1000 */     6,    9,   10,  132,  188,  134,   45,   44,  192,  210,
 /*  1010 */   211,  212,  213,   21,  198,  199,  200,  201,    6,   56,
 /*  1020 */    28,  202,  203,   57,  142,   33,   34,   35,   36,   37,
 /*  1030 */    38,   39,  188,   41,   44,   51,   44,   45,   54,   45,
 /*  1040 */    57,  142,  198,  199,  200,  201,   54,   55,   56,   44,
 /*  1050 */    22,   59,   57,   61,  142,   44,   64,   45,   57,   67,
 /*  1060 */    68,   69,   70,   71,   72,   73,   74,   75,   76,   77,
 /*  1070 */   142,   44,   57,   81,   82,   83,  142,   85,   44,   61,
 /*  1080 */   188,   89,   90,   91,   92,   45,   94,   45,   59,   97,
 /*  1090 */   198,  199,  200,  201,   57,  142,  104,   45,   59,  142,
 /*  1100 */   108,  109,  110,  111,  112,  113,  114,  115,  116,  117,
 /*  1110 */     7,   57,    9,   10,   45,  188,   44,  131,   44,  192,
 /*  1120 */   131,   44,  131,   55,   21,  198,  199,  200,  201,   44,
 /*  1130 */    54,   28,   61,   45,   45,   84,   33,   34,   35,   36,
 /*  1140 */    37,   38,   39,  188,   41,   54,   54,   44,   45,  142,
 /*  1150 */   183,   87,   54,  198,  199,  200,  201,   54,   55,   56,
 /*  1160 */    54,   54,   59,   90,   61,   45,   61,   64,   40,   44,
 /*  1170 */    67,   68,   69,   70,   71,   72,   73,   74,   75,   76,
 /*  1180 */    77,   22,   61,   44,   81,   82,   83,    6,   85,   87,
 /*  1190 */    61,  188,   89,   90,   91,   92,   87,   94,   44,   55,
 /*  1200 */    97,  198,  199,  200,  201,   44,   61,  104,   55,   61,
 /*  1210 */    44,  108,  109,  110,  111,  112,  113,  114,  115,  116,
 /*  1220 */   117,    7,   55,    9,   10,   61,  188,   87,   55,   61,
 /*  1230 */   192,   55,   61,  188,   54,   21,  198,  199,  200,  201,
 /*  1240 */    93,  185,   28,  198,  199,  200,  201,   33,   34,   35,
 /*  1250 */    36,   37,   38,   39,  188,   41,   44,   44,   44,   45,
 /*  1260 */   190,   66,   55,   55,  198,  199,  200,  201,   54,   55,
 /*  1270 */    56,  191,  191,   59,  191,   61,   44,  191,   64,   44,
 /*  1280 */    44,   67,   68,   69,   70,   71,   72,   73,   74,   75,
 /*  1290 */    76,   77,  191,   55,   45,   81,   82,   83,   45,   85,
 /*  1300 */    45,  191,  188,   89,   90,   91,   92,  191,   94,  191,
 /*  1310 */   191,   97,  198,  199,  200,  201,   45,  142,  104,   57,
 /*  1320 */   203,   44,  108,  109,  110,  111,  112,  113,  114,  115,
 /*  1330 */   116,  117,    7,   44,    9,   10,   95,  188,   54,  187,
 /*  1340 */    90,  192,   45,   87,  188,   84,   21,  198,  199,  200,
 /*  1350 */   201,   44,   54,   28,  198,  199,  200,  201,   33,   34,
 /*  1360 */    35,   36,   37,   38,   39,  188,   41,   54,   54,   44,
 /*  1370 */    45,   61,   54,   54,  143,  198,  199,  200,  201,   54,
 /*  1380 */    55,   56,  142,   45,   59,  143,   61,   45,   45,   64,
 /*  1390 */   145,   44,   67,   68,   69,   70,   71,   72,   73,   74,
 /*  1400 */    75,   76,   77,   45,   45,  142,   81,   82,   83,  143,
 /*  1410 */    85,  143,   45,  188,   89,   90,   91,   92,  131,   94,
 /*  1420 */   134,  131,   97,  198,  199,  200,  201,   50,   44,  104,
 /*  1430 */    44,  131,  131,  108,  109,  110,  111,  112,  113,  114,
 /*  1440 */   115,  116,  117,    7,   50,    9,   10,  131,  188,   44,
 /*  1450 */    44,  131,  192,  131,   54,  188,  214,   21,  198,  199,
 /*  1460 */   200,  201,  214,  214,   28,  198,  199,  200,  201,   33,
 /*  1470 */    34,   35,   36,   37,   38,   39,  188,   41,  214,  214,
 /*  1480 */    44,   45,  214,  214,  214,  214,  198,  199,  200,  201,
 /*  1490 */    54,   55,   56,  214,  214,   59,  214,   61,  214,  214,
 /*  1500 */    64,  214,  214,   67,   68,   69,   70,   71,   72,   73,
 /*  1510 */    74,   75,   76,   77,  214,  214,  214,   81,   82,   83,
 /*  1520 */   214,   85,  214,  214,  188,   89,   90,   91,   92,  214,
 /*  1530 */    94,  214,  214,   97,  198,  199,  200,  201,  214,  214,
 /*  1540 */   104,  214,  214,  214,  108,  109,  110,  111,  112,  113,
 /*  1550 */   114,  115,  116,  117,    7,  214,    9,   10,  214,  188,
 /*  1560 */   214,  214,  214,  192,  214,  214,  188,  214,   21,  198,
 /*  1570 */   199,  200,  201,  214,  214,   28,  198,  199,  200,  201,
 /*  1580 */    33,   34,   35,   36,   37,   38,   39,  188,   41,  214,
 /*  1590 */   214,   44,   45,  214,  214,  214,  214,  198,  199,  200,
 /*  1600 */   201,   54,   55,   56,  214,  214,   59,  214,   61,  214,
 /*  1610 */   214,   64,  214,  214,   67,   68,   69,   70,   71,   72,
 /*  1620 */    73,   74,   75,   76,   77,  214,  214,  214,   81,   82,
 /*  1630 */    83,  214,   85,  214,  214,  188,   89,   90,   91,   92,
 /*  1640 */   214,   94,  214,  214,   97,  198,  199,  200,  201,  214,
 /*  1650 */   214,  104,  214,  214,  214,  108,  109,  110,  111,  112,
 /*  1660 */   113,  114,  115,  116,  117,    7,  214,    9,   10,  214,
 /*  1670 */   188,  214,  214,  214,  192,  214,  214,  188,  214,   21,
 /*  1680 */   198,  199,  200,  201,  214,  214,   28,  198,  199,  200,
 /*  1690 */   201,   33,   34,   35,   36,   37,   38,   39,  188,   41,
 /*  1700 */   214,  214,   44,   45,  214,  214,  214,  214,  198,  199,
 /*  1710 */   200,  201,   54,   55,   56,  214,  214,   59,  214,   61,
 /*  1720 */   214,  214,   64,  214,  214,   67,   68,   69,   70,   71,
 /*  1730 */    72,   73,   74,   75,   76,   77,  214,  214,  214,   81,
 /*  1740 */    82,   83,  214,   85,  214,  214,  188,   89,   90,   91,
 /*  1750 */    92,  214,   94,  214,  214,   97,  198,  199,  200,  201,
 /*  1760 */   214,  214,  104,  214,  214,  214,  108,  109,  110,  111,
 /*  1770 */   112,  113,  114,  115,  116,  117,    7,  214,    9,   10,
 /*  1780 */   214,  188,  214,  214,  214,  192,  214,  214,  188,  214,
 /*  1790 */    21,  198,  199,  200,  201,  214,  214,   28,  198,  199,
 /*  1800 */   200,  201,   33,   34,   35,   36,   37,   38,   39,  188,
 /*  1810 */    41,  214,  214,   44,   45,  214,  214,  214,  214,  198,
 /*  1820 */   199,  200,  201,   54,   55,   56,  214,  214,   59,  214,
 /*  1830 */    61,  214,  214,   64,  214,  214,   67,   68,   69,   70,
 /*  1840 */    71,   72,   73,   74,   75,   76,   77,  214,  214,  214,
 /*  1850 */    81,   82,   83,  214,   85,  214,  214,  188,   89,   90,
 /*  1860 */    91,   92,  214,   94,  214,  214,   97,  198,  199,  200,
 /*  1870 */   201,  214,  214,  104,  214,  214,  214,  108,  109,  110,
 /*  1880 */   111,  112,  113,  114,  115,  116,  117,    7,  214,    9,
 /*  1890 */    10,  214,  188,  214,  214,  214,  192,  214,  214,  188,
 /*  1900 */   214,   21,  198,  199,  200,  201,  214,  214,   28,  198,
 /*  1910 */   199,  200,  201,   33,   34,   35,   36,   37,   38,   39,
 /*  1920 */   188,   41,  214,  214,   44,   45,  214,  214,  214,  214,
 /*  1930 */   198,  199,  200,  201,   54,   55,   56,  214,  214,   59,
 /*  1940 */   214,   61,  214,  214,   64,  214,  214,   67,   68,   69,
 /*  1950 */    70,   71,   72,   73,   74,   75,   76,   77,  214,  214,
 /*  1960 */   214,   81,   82,   83,  214,   85,  214,  214,  188,   89,
 /*  1970 */    90,   91,   92,  214,   94,  214,  214,   97,  198,  199,
 /*  1980 */   200,  201,  214,  214,  104,  214,  214,  214,  108,  109,
 /*  1990 */   110,  111,  112,  113,  114,  115,  116,  117,    7,  214,
 /*  2000 */     9,   10,  214,  188,  214,  214,  214,  192,  214,  214,
 /*  2010 */   188,  214,   21,  198,  199,  200,  201,  214,  214,   28,
 /*  2020 */   198,  199,  200,  201,   33,   34,   35,   36,   37,   38,
 /*  2030 */    39,  188,   41,  214,  214,   44,   45,  214,  214,  214,
 /*  2040 */   214,  198,  199,  200,  201,   54,   55,   56,  214,  214,
 /*  2050 */    59,  214,   61,  214,  214,   64,  214,  214,   67,   68,
 /*  2060 */    69,   70,   71,   72,   73,   74,   75,   76,   77,  214,
 /*  2070 */   214,  214,   81,   82,   83,  214,   85,  214,  214,  188,
 /*  2080 */    89,   90,   91,   92,  214,   94,  214,  214,   97,  198,
 /*  2090 */   199,  200,  201,  214,  214,  104,  214,  214,  214,  108,
 /*  2100 */   109,  110,  111,  112,  113,  114,  115,  116,  117,    7,
 /*  2110 */   214,    9,   10,  214,  188,  214,  214,  214,  192,  214,
 /*  2120 */   214,  188,  214,   21,  198,  199,  200,  201,  214,  214,
 /*  2130 */    28,  198,  199,  200,  201,   33,   34,   35,   36,   37,
 /*  2140 */    38,   39,  188,   41,  214,  214,   44,   45,  214,  214,
 /*  2150 */   214,  214,  198,  199,  200,  201,   54,   55,   56,  214,
 /*  2160 */   214,   59,  214,   61,  214,  214,   64,  214,  214,   67,
 /*  2170 */    68,   69,   70,   71,   72,   73,   74,   75,   76,   77,
 /*  2180 */   214,  214,  214,   81,   82,   83,  214,   85,  214,  214,
 /*  2190 */   188,   89,   90,   91,   92,  214,   94,  214,  214,   97,
 /*  2200 */   198,  199,  200,  201,  214,  214,  104,  214,  214,  214,
 /*  2210 */   108,  109,  110,  111,  112,  113,  114,  115,  116,  117,
 /*  2220 */     7,  214,    9,   10,  214,  188,  214,  214,  214,  192,
 /*  2230 */   214,  214,  188,  214,   21,  198,  199,  200,  201,  214,
 /*  2240 */   214,   28,  198,  199,  200,  201,   33,   34,   35,   36,
 /*  2250 */    37,   38,   39,  188,   41,  214,  214,   44,   45,  214,
 /*  2260 */   214,  214,  214,  198,  199,  200,  201,   54,   55,   56,
 /*  2270 */   214,  214,   59,  214,   61,  214,  214,   64,  214,  214,
 /*  2280 */    67,   68,   69,   70,   71,   72,   73,   74,   75,   76,
 /*  2290 */    77,  214,  214,  214,   81,   82,   83,  214,   85,  214,
 /*  2300 */   214,  188,   89,   90,   91,   92,  214,   94,  214,  214,
 /*  2310 */    97,  198,  199,  200,  201,  214,  214,  104,  214,  214,
 /*  2320 */   214,  108,  109,  110,  111,  112,  113,  114,  115,  116,
 /*  2330 */   117,    7,  214,    9,   10,  214,  188,  214,  214,  214,
 /*  2340 */   192,  214,  214,  188,  214,   21,  198,  199,  200,  201,
 /*  2350 */   214,  214,   28,  198,  199,  200,  201,   33,   34,   35,
 /*  2360 */    36,   37,   38,   39,  188,   41,  214,  214,   44,   45,
 /*  2370 */   214,  214,  214,  214,  198,  199,  200,  201,   54,   55,
 /*  2380 */    56,  214,  214,   59,  214,   61,  214,  214,   64,  214,
 /*  2390 */   214,   67,   68,   69,   70,   71,   72,   73,   74,   75,
 /*  2400 */    76,   77,  214,  214,  214,   81,   82,   83,  214,   85,
 /*  2410 */   214,  214,  188,   89,   90,   91,   92,  214,   94,  214,
 /*  2420 */   214,   97,  198,  199,  200,  201,  214,  214,  104,  214,
 /*  2430 */   214,  214,  108,  109,  110,  111,  112,  113,  114,  115,
 /*  2440 */   116,  117,    7,  214,    9,   10,  214,  188,  214,  214,
 /*  2450 */   214,  192,  214,  214,  188,  214,   21,  198,  199,  200,
 /*  2460 */   201,  214,  214,   28,  198,  199,  200,  201,   33,   34,
 /*  2470 */    35,   36,   37,   38,   39,  188,   41,  214,  214,   44,
 /*  2480 */    45,  214,  214,  214,  214,  198,  199,  200,  201,   54,
 /*  2490 */    55,   56,  214,  214,   59,  214,   61,  214,  214,   64,
 /*  2500 */   214,  214,   67,   68,   69,   70,   71,   72,   73,   74,
 /*  2510 */    75,   76,   77,  214,  214,  214,   81,   82,   83,  214,
 /*  2520 */    85,  214,  214,  188,   89,   90,   91,   92,  214,   94,
 /*  2530 */   214,  214,   97,  198,  199,  200,  201,  214,  214,  104,
 /*  2540 */   214,  214,  214,  108,  109,  110,  111,  112,  113,  114,
 /*  2550 */   115,  116,  117,    7,  214,    9,   10,  214,  188,  214,
 /*  2560 */   214,  214,  214,  214,  214,  188,  214,   21,  198,  199,
 /*  2570 */   200,  201,  214,  214,   28,  198,  199,  200,  201,   33,
 /*  2580 */    34,   35,   36,   37,   38,   39,  188,   41,  214,  214,
 /*  2590 */    44,   45,  214,  214,  214,  214,  198,  199,  200,  201,
 /*  2600 */    54,   55,   56,  214,  214,   59,  214,   61,  214,  214,
 /*  2610 */    64,  214,  214,   67,   68,   69,   70,   71,   72,   73,
 /*  2620 */    74,   75,   76,   77,  214,  214,  214,   81,   82,   83,
 /*  2630 */   214,   85,  214,  214,  188,   89,   90,   91,   92,  214,
 /*  2640 */    94,  214,  214,   97,  198,  199,  200,  201,  214,  214,
 /*  2650 */   104,  214,  214,  214,  108,  109,  110,  111,  112,  113,
 /*  2660 */   114,  115,  116,  117,    7,  214,    9,   10,  214,  188,
 /*  2670 */   214,  214,  214,  214,  214,  214,  214,  214,   21,  198,
 /*  2680 */   199,  200,  201,  214,  214,   28,  214,  214,  214,  214,
 /*  2690 */    33,   34,   35,   36,   37,   38,   39,  214,   41,  214,
 /*  2700 */   214,   44,   45,  214,  214,  214,  214,  214,  214,  214,
 /*  2710 */   214,   54,   55,   56,  214,  214,   59,  214,   61,  214,
 /*  2720 */   214,   64,  214,  214,   67,   68,   69,   70,   71,   72,
 /*  2730 */    73,   74,   75,   76,   77,  214,  214,  214,   81,   82,
 /*  2740 */    83,  214,   85,  214,  214,  214,   89,   90,   91,   92,
 /*  2750 */   214,   94,  214,  214,   97,  214,  214,  214,  214,  214,
 /*  2760 */   214,  104,  214,  214,  214,  108,  109,  110,  111,  112,
 /*  2770 */   113,  114,  115,  116,  117,    7,  214,    9,   10,  214,
 /*  2780 */   214,  214,  214,  214,  214,  214,  214,  214,  214,   21,
 /*  2790 */   214,  214,  214,  214,  214,  214,   28,  214,  214,  214,
 /*  2800 */   214,   33,   34,   35,   36,   37,   38,   39,  214,   41,
 /*  2810 */   214,  214,   44,   45,  214,  214,  214,  214,  214,  214,
 /*  2820 */   214,  214,   54,   55,   56,  214,  214,   59,  214,   61,
 /*  2830 */   214,  214,   64,  214,  214,   67,   68,   69,   70,   71,
 /*  2840 */    72,   73,   74,   75,   76,   77,  214,  214,  214,   81,
 /*  2850 */    82,   83,  214,   85,  214,  214,  214,   89,   90,   91,
 /*  2860 */    92,  214,   94,  214,  214,   97,  214,  214,  214,  214,
 /*  2870 */   214,  214,  104,  214,  214,  214,  108,  109,  110,  111,
 /*  2880 */   112,  113,  114,  115,  116,  117,    7,  214,    9,   10,
 /*  2890 */   214,  214,  214,  214,  214,  214,  214,  214,  214,  214,
 /*  2900 */    21,  214,  214,  214,  214,  214,  214,   28,  214,  214,
 /*  2910 */   214,  214,   33,   34,   35,   36,   37,   38,   39,  214,
 /*  2920 */    41,  214,  214,   44,   45,  214,  214,  214,  214,  214,
 /*  2930 */   214,  214,  214,   54,   55,   56,  214,  214,   59,  214,
 /*  2940 */    61,  214,  214,   64,  214,  214,   67,   68,   69,   70,
 /*  2950 */    71,   72,   73,   74,   75,   76,   77,  214,  214,  214,
 /*  2960 */    81,   82,   83,  214,   85,  214,  214,  214,   89,   90,
 /*  2970 */    91,   92,  214,   94,  214,  214,   97,  214,  214,  214,
 /*  2980 */   214,  214,  214,  104,  214,  214,  214,  108,  109,  110,
 /*  2990 */   111,  112,  113,  114,  115,  116,  117,    7,  214,    9,
 /*  3000 */    10,  214,  214,  214,  214,  214,  214,  214,  214,  214,
 /*  3010 */   214,   21,  214,  214,  214,  214,  214,  214,   28,  214,
 /*  3020 */   214,  214,  214,   33,   34,   35,   36,   37,   38,   39,
 /*  3030 */   214,   41,  214,  214,   44,   45,  214,  214,  214,  214,
 /*  3040 */   214,  214,  214,  214,   54,   55,   56,  214,  214,   59,
 /*  3050 */   214,   61,  214,  214,   64,  214,  214,   67,   68,   69,
 /*  3060 */    70,   71,   72,   73,   74,   75,   76,   77,  214,  214,
 /*  3070 */   214,   81,   82,   83,  214,   85,  214,  214,  214,   89,
 /*  3080 */    90,   91,   92,  214,   94,  214,  214,   97,  214,  214,
 /*  3090 */   214,  214,  214,  214,  104,  214,  214,  214,  108,  109,
 /*  3100 */   110,  111,  112,  113,  114,  115,  116,  117,    7,  214,
 /*  3110 */     9,   10,  214,  214,  214,  214,  214,  214,  214,  214,
 /*  3120 */   214,  214,   21,  214,  214,  214,  214,  214,  214,   28,
 /*  3130 */   214,  214,  214,  214,   33,   34,   35,   36,   37,   38,
 /*  3140 */    39,  214,   41,  214,  214,   44,   45,  214,  214,  214,
 /*  3150 */   214,  214,  214,  214,  214,   54,   55,   56,  214,  214,
 /*  3160 */    59,  214,   61,  214,  214,   64,  214,  214,   67,   68,
 /*  3170 */    69,   70,   71,   72,   73,   74,   75,   76,   77,  214,
 /*  3180 */   214,  214,   81,   82,   83,  214,   85,  214,  214,  214,
 /*  3190 */    89,   90,   91,   92,  214,   94,  214,  214,   97,  214,
 /*  3200 */   214,  214,  214,  214,  214,  104,  214,  214,  214,  108,
 /*  3210 */   109,  110,  111,  112,  113,  114,  115,  116,  117,    7,
 /*  3220 */   214,    9,   10,  214,  214,  214,  214,  214,  214,  214,
 /*  3230 */   214,  214,  214,   21,  214,  214,  214,  214,  214,  214,
 /*  3240 */    28,  214,  214,  214,  214,   33,   34,   35,   36,   37,
 /*  3250 */    38,   39,  214,   41,  214,  214,   44,   45,  214,  214,
 /*  3260 */   214,  214,  214,  214,  214,  214,   54,   55,   56,  214,
 /*  3270 */   214,   59,  214,   61,  214,  214,   64,  214,  214,   67,
 /*  3280 */    68,   69,   70,   71,   72,   73,   74,   75,   76,   77,
 /*  3290 */   214,  214,  214,   81,   82,   83,  214,   85,  214,  214,
 /*  3300 */   214,   89,   90,   91,   92,  214,   94,  214,  214,   97,
 /*  3310 */   214,  214,  214,  214,  214,  214,  104,  214,  214,  214,
 /*  3320 */   108,  109,  110,  111,  112,  113,  114,  115,  116,  117,
 /*  3330 */     7,  214,    9,   10,  214,  214,  214,  214,  214,  214,
 /*  3340 */   214,  214,  214,  214,   21,  214,  214,  214,  214,  214,
 /*  3350 */   214,   28,  214,  214,  214,  214,   33,   34,   35,   36,
 /*  3360 */    37,   38,   39,  214,   41,  214,  214,   44,   45,  214,
 /*  3370 */   214,  214,  214,  214,  214,  214,  214,   54,   55,   56,
 /*  3380 */   214,  214,   59,  214,   61,  214,  214,   64,  214,  214,
 /*  3390 */    67,   68,   69,   70,   71,   72,   73,   74,   75,   76,
 /*  3400 */    77,  214,  214,  214,   81,   82,   83,  214,   85,  214,
 /*  3410 */   214,  214,   89,   90,   91,   92,  214,   94,  214,  214,
 /*  3420 */    97,  214,  214,  214,  214,  214,  214,  104,  214,  214,
 /*  3430 */   214,  108,  109,  110,  111,  112,  113,  114,  115,  116,
 /*  3440 */   117,    7,  214,    9,   10,  214,  214,  214,  214,  214,
 /*  3450 */   214,  214,  214,  214,  214,   21,  214,  214,  214,  214,
 /*  3460 */   214,  214,   28,  214,  214,  214,  214,   33,   34,   35,
 /*  3470 */    36,   37,   38,   39,  214,   41,  214,  214,   44,   45,
 /*  3480 */   214,  214,  214,  214,  214,  214,  214,  214,   54,   55,
 /*  3490 */    56,  214,  214,   59,  214,   61,  214,  214,   64,  214,
 /*  3500 */   214,   67,   68,   69,   70,   71,   72,   73,   74,   75,
 /*  3510 */    76,   77,  214,  214,  214,   81,   82,   83,  214,   85,
 /*  3520 */   214,  214,  214,   89,   90,   91,   92,  214,   94,  214,
 /*  3530 */   214,   97,  214,  214,  214,  214,  214,  214,  104,  214,
 /*  3540 */   214,  214,  108,  109,  110,  111,  112,  113,  114,  115,
 /*  3550 */   116,  117,    7,  214,    9,   10,  214,  214,  214,  214,
 /*  3560 */   214,  214,  214,  214,  214,  214,   21,  214,  214,  214,
 /*  3570 */   214,  214,  214,   28,  214,  214,  214,  214,   33,   34,
 /*  3580 */    35,   36,   37,   38,   39,  214,   41,  214,  214,   44,
 /*  3590 */    45,  214,  214,  214,  214,  214,  214,  214,  214,   54,
 /*  3600 */    55,   56,  214,  214,   59,  214,   61,  214,  214,   64,
 /*  3610 */   214,  214,   67,   68,   69,   70,   71,   72,   73,   74,
 /*  3620 */    75,   76,   77,  214,  214,  214,   81,   82,   83,  214,
 /*  3630 */    85,  214,  214,  214,   89,   90,   91,   92,  214,   94,
 /*  3640 */   214,  214,   97,  214,  214,  214,  214,  214,  214,  104,
 /*  3650 */   214,  214,  214,  108,  109,  110,  111,  112,  113,  114,
 /*  3660 */   115,  116,  117,    7,  214,    9,   10,  214,  214,  214,
 /*  3670 */   214,  214,  214,  214,  214,  214,  214,   21,  214,  214,
 /*  3680 */   214,  214,  214,  214,   28,  214,  214,  214,  214,   33,
 /*  3690 */    34,   35,   36,   37,   38,   39,  214,   41,  214,  214,
 /*  3700 */    44,   45,  214,  214,  214,  214,  214,  214,  214,  214,
 /*  3710 */    54,   55,   56,  214,  214,   59,  214,   61,  214,  214,
 /*  3720 */    64,  214,  214,   67,   68,   69,   70,   71,   72,   73,
 /*  3730 */    74,   75,   76,   77,  214,  214,  214,   81,   82,   83,
 /*  3740 */   214,   85,  214,  214,  214,   89,   90,   91,   92,  214,
 /*  3750 */    94,  214,  214,   97,  214,  214,  214,  214,  214,  214,
 /*  3760 */   104,  214,  214,  214,  108,  109,  110,  111,  112,  113,
 /*  3770 */   114,  115,  116,  117,    7,  214,    9,   10,  214,  214,
 /*  3780 */   214,  214,  214,  214,  214,  214,  214,  214,   21,  214,
 /*  3790 */   214,  214,  214,  214,  214,   28,  214,  214,  214,  214,
 /*  3800 */    33,   34,   35,   36,   37,   38,   39,  214,   41,  214,
 /*  3810 */   214,   44,   45,  214,  214,  214,  214,  214,  214,  214,
 /*  3820 */   214,   54,   55,   56,  214,  214,   59,  214,   61,  214,
 /*  3830 */   214,   64,  214,  214,   67,   68,   69,   70,   71,   72,
 /*  3840 */    73,   74,   75,   76,   77,  214,  214,  214,   81,   82,
 /*  3850 */    83,  214,   85,  214,  214,  214,   89,   90,   91,   92,
 /*  3860 */   214,   94,  214,  214,   97,  214,  214,  214,  214,  214,
 /*  3870 */   214,  104,  214,  214,  214,  108,  109,  110,  111,  112,
 /*  3880 */   113,  114,  115,  116,  117,    7,  214,    9,   10,  214,
 /*  3890 */   214,  214,  214,  214,  214,  214,  214,  214,  214,   21,
 /*  3900 */   214,  214,  214,  214,  214,  214,   28,  214,  214,  214,
 /*  3910 */   214,   33,   34,   35,   36,   37,   38,   39,  214,   41,
 /*  3920 */   214,  214,   44,   45,  214,  214,  214,  214,  214,  214,
 /*  3930 */   214,  214,   54,   55,   56,  214,  214,   59,  214,   61,
 /*  3940 */   214,  214,   64,  214,  214,   67,   68,   69,   70,   71,
 /*  3950 */    72,   73,   74,   75,   76,   77,  214,  214,  214,   81,
 /*  3960 */    82,   83,  214,   85,  214,  214,  214,   89,   90,   91,
 /*  3970 */    92,  214,   94,  214,  214,   97,  214,  214,  214,  214,
 /*  3980 */   214,  214,  104,  214,  214,  214,  108,  109,  110,  111,
 /*  3990 */   112,  113,  114,  115,  116,  117,    7,  214,    9,   10,
 /*  4000 */   214,  214,  214,  214,  214,  214,  214,  214,  214,  214,
 /*  4010 */    21,  214,  214,  214,  214,  214,  214,   28,  214,  214,
 /*  4020 */   214,  214,   33,   34,   35,   36,   37,   38,   39,  214,
 /*  4030 */    41,  214,  214,   44,   45,  214,  214,  214,  214,  214,
 /*  4040 */   214,  214,  214,   54,   55,   56,  214,  214,   59,  214,
 /*  4050 */    61,  214,  214,   64,  214,  214,   67,   68,   69,   70,
 /*  4060 */    71,   72,   73,   74,   75,   76,   77,  214,  214,  214,
 /*  4070 */    81,   82,   83,  214,   85,  214,  214,  214,   89,   90,
 /*  4080 */    91,   92,  214,   94,  214,  214,   97,  214,  214,  214,
 /*  4090 */   214,  214,  214,  104,  214,  214,  214,  108,  109,  110,
 /*  4100 */   111,  112,  113,  114,  115,  116,  117,    7,  214,    9,
 /*  4110 */    10,  214,  214,  214,  214,  214,  214,  214,  214,  214,
 /*  4120 */   214,   21,  214,  214,  214,  214,  214,  214,   28,  214,
 /*  4130 */   214,  214,  214,   33,   34,   35,   36,   37,   38,   39,
 /*  4140 */   214,   41,  214,  214,   44,   45,  214,  214,  214,  214,
 /*  4150 */   214,  214,  214,  214,   54,   55,   56,  214,  214,   59,
 /*  4160 */   214,   61,  214,  214,   64,  214,  214,   67,   68,   69,
 /*  4170 */    70,   71,   72,   73,   74,   75,   76,   77,  214,  214,
 /*  4180 */   214,   81,   82,   83,  214,   85,  214,  214,  214,   89,
 /*  4190 */    90,   91,   92,  214,   94,  214,  214,   97,  214,  214,
 /*  4200 */   214,  214,  214,  214,  104,  214,  214,  214,  108,  109,
 /*  4210 */   110,  111,  112,  113,  114,  115,  116,  117,    7,  214,
 /*  4220 */     9,   10,  214,  214,  214,  214,  214,  214,  214,  214,
 /*  4230 */   214,  214,   21,  214,  214,  214,  214,  214,  214,   28,
 /*  4240 */   214,  214,  214,  214,   33,   34,   35,   36,   37,   38,
 /*  4250 */    39,  214,   41,  214,  214,   44,   45,  214,  214,  214,
 /*  4260 */   214,  214,  214,  214,  214,   54,   55,   56,  214,  214,
 /*  4270 */    59,  214,   61,  214,  214,   64,  214,  214,   67,   68,
 /*  4280 */    69,   70,   71,   72,   73,   74,   75,   76,   77,  214,
 /*  4290 */   214,  214,   81,   82,   83,  214,   85,  214,  214,  214,
 /*  4300 */    89,   90,   91,   92,  214,   94,  214,  214,   97,  214,
 /*  4310 */   214,  214,  214,  214,  214,  104,  214,  214,  214,  108,
 /*  4320 */   109,  110,  111,  112,  113,  114,  115,  116,  117,    7,
 /*  4330 */   214,    9,   10,  214,  214,  214,  214,  214,  214,  214,
 /*  4340 */   214,  214,  214,   21,  214,  214,  214,  214,  214,  214,
 /*  4350 */    28,  214,  214,  214,  214,   33,   34,   35,   36,   37,
 /*  4360 */    38,   39,  214,   41,  214,  214,   44,   45,  214,  214,
 /*  4370 */   214,  214,  214,  214,  214,  214,   54,   55,   56,  214,
 /*  4380 */   214,   59,  214,   61,  214,  214,   64,  214,  214,   67,
 /*  4390 */    68,   69,   70,   71,   72,   73,   74,   75,   76,   77,
 /*  4400 */   214,  214,  214,   81,   82,   83,  214,   85,  214,  214,
 /*  4410 */   214,   89,   90,   91,   92,  214,   94,  214,  214,   97,
 /*  4420 */   214,  214,  214,  214,  214,  214,  104,  214,  214,  214,
 /*  4430 */   108,  109,  110,  111,  112,  113,  114,  115,  116,  117,
 /*  4440 */     7,  214,    9,   10,  214,  214,  214,  214,  214,  214,
 /*  4450 */   214,  214,  214,  214,   21,  214,  214,  214,  214,  214,
 /*  4460 */   214,   28,  214,  214,  214,  214,   33,   34,   35,   36,
 /*  4470 */    37,   38,   39,  214,   41,  214,  214,   44,   45,  214,
 /*  4480 */   214,  214,  214,  214,  214,  214,  214,   54,  155,   56,
 /*  4490 */   214,  214,   59,  214,   61,  214,  214,   64,  214,  214,
 /*  4500 */    67,   68,   69,   70,   71,   72,   73,   74,   75,   76,
 /*  4510 */    77,  214,  214,  214,   81,   82,   83,  214,   85,  214,
 /*  4520 */   214,  188,   89,   90,   91,   92,  214,   94,  214,  214,
 /*  4530 */    97,  198,  199,  200,  201,  214,  214,  104,  214,  214,
 /*  4540 */   214,  108,  109,  110,  111,  112,  113,  114,  115,  116,
 /*  4550 */   117,    7,  214,    9,   10,  214,  214,  214,  214,  214,
 /*  4560 */   214,  214,  214,  214,  214,   21,  214,  214,  214,  214,
 /*  4570 */   214,  214,   28,  214,  214,  214,  214,   33,   34,   35,
 /*  4580 */    36,   37,   38,   39,  214,   41,  214,  214,   44,  214,
 /*  4590 */   214,  214,  214,  214,  214,  214,  214,  214,   54,  214,
 /*  4600 */   214,  214,  214,   59,  214,   61,  214,  214,   64,  214,
 /*  4610 */   214,   67,   68,   69,   70,   71,   72,   73,   74,   75,
 /*  4620 */    76,   77,   78,   79,   80,  214,  214,  214,  214,  127,
 /*  4630 */   128,   19,   20,   21,   22,   23,   24,   25,   26,   27,
 /*  4640 */    28,   29,   30,   31,   32,  214,  214,  214,  104,  214,
 /*  4650 */   214,  214,  150,   41,   42,  214,  112,  113,  114,  115,
 /*  4660 */   116,  159,  160,  161,  162,  163,  164,  165,  166,  167,
 /*  4670 */   168,  169,  170,  171,  172,  173,  174,  175,  176,  177,
 /*  4680 */   178,  179,  180,  214,  214,  214,   41,  214,  181,   44,
 /*  4690 */   188,  214,  214,  127,  128,  188,  214,  214,  214,  214,
 /*  4700 */   198,  199,  200,  201,   59,  198,  199,  200,  201,   64,
 /*  4710 */   214,   66,  214,  214,  214,  214,  150,  214,  214,  214,
 /*  4720 */   214,  214,  214,  214,  214,  159,  160,  161,  162,  163,
 /*  4730 */   164,  165,  166,  167,  168,  169,  170,  171,  172,  173,
 /*  4740 */   174,  175,  176,  177,  178,  179,  180,  127,  128,  104,
 /*  4750 */   214,  214,  214,   41,  188,  214,  214,  112,  113,  114,
 /*  4760 */   115,  116,  214,  214,  198,  199,  200,  201,  214,   57,
 /*  4770 */   150,  214,  214,  214,  214,  214,  214,  214,  214,  159,
 /*  4780 */   160,  161,  162,  163,  164,  165,  166,  167,  168,  169,
 /*  4790 */   170,  171,  172,  173,  174,  175,  176,  177,  178,  179,
 /*  4800 */   180,  127,  128,  214,  214,  214,   41,  214,  188,  214,
 /*  4810 */    98,   99,  100,  101,  102,  103,  214,  214,  198,  199,
 /*  4820 */   200,  201,   57,  214,  150,  214,  214,  214,  214,  214,
 /*  4830 */   214,  214,  214,  159,  160,  161,  162,  163,  164,  165,
 /*  4840 */   166,  167,  168,  169,  170,  171,  172,  173,  174,  175,
 /*  4850 */   176,  177,  178,  179,  180,  127,  128,  214,  214,  214,
 /*  4860 */    41,  214,  188,   98,   99,  100,  101,  102,  103,  214,
 /*  4870 */   214,  214,  198,  199,  200,  201,   57,  214,  150,  214,
 /*  4880 */   214,  214,  214,  214,  214,  214,  214,  159,  160,  161,
 /*  4890 */   162,  163,  164,  165,  166,  167,  168,  169,  170,  171,
 /*  4900 */   172,  173,  174,  175,  176,  177,  178,  179,  180,  127,
 /*  4910 */   128,  214,  214,  214,   41,  214,  188,   98,   99,  100,
 /*  4920 */   101,  102,  103,  214,  214,  214,  198,  199,  200,  201,
 /*  4930 */    57,  214,  150,  214,  214,  214,  214,  214,  214,  214,
 /*  4940 */   214,  159,  160,  161,  162,  163,  164,  165,  166,  167,
 /*  4950 */   168,  169,  170,  171,  172,  173,  174,  175,  176,  177,
 /*  4960 */   178,  179,  180,  127,  128,  214,  214,  214,  214,  214,
 /*  4970 */   188,   98,   99,  100,  101,  102,  103,  214,  214,  214,
 /*  4980 */   198,  199,  200,  201,   57,  214,  150,  214,  214,  214,
 /*  4990 */   214,  214,  214,  214,  214,  159,  160,  161,  162,  163,
 /*  5000 */   164,  165,  166,  167,  168,  169,  170,  171,  172,  173,
 /*  5010 */   174,  175,  176,  177,  178,  179,  180,  127,  128,  214,
 /*  5020 */   214,  214,  214,  214,  188,   98,   99,  100,  101,  102,
 /*  5030 */   103,  214,  214,  214,  198,  199,  200,  201,  214,  214,
 /*  5040 */   150,  214,  214,  214,  214,  214,  214,  214,  214,  159,
 /*  5050 */   160,  161,  162,  163,  164,  165,  166,  167,  168,  169,
 /*  5060 */   170,  171,  172,  173,  174,  175,  176,  177,  178,  179,
 /*  5070 */   180,  127,  128,  214,  214,  214,  214,  214,  188,  181,
 /*  5080 */   214,  214,  214,  214,  214,  214,  188,  214,  198,  199,
 /*  5090 */   200,  201,  214,  214,  150,  214,  198,  199,  200,  201,
 /*  5100 */   214,  214,  214,  159,  160,  161,  162,  163,  164,  165,
 /*  5110 */   166,  167,  168,  169,  170,  171,  172,  173,  174,  175,
 /*  5120 */   176,  177,  178,  179,  180,  127,  128,  214,  214,  214,
 /*  5130 */   214,  214,  188,  181,  214,  214,  214,  214,  214,  214,
 /*  5140 */   188,  214,  198,  199,  200,  201,  214,  214,  150,  214,
 /*  5150 */   198,  199,  200,  201,  214,  214,  214,  159,  160,  161,
 /*  5160 */   162,  163,  164,  165,  166,  167,  168,  169,  170,  171,
 /*  5170 */   172,  173,  174,  175,  176,  177,  178,  179,  180,  127,
 /*  5180 */   128,  214,  214,  214,  214,  214,  188,  214,  214,  214,
 /*  5190 */   214,  214,  214,  214,  214,  214,  198,  199,  200,  201,
 /*  5200 */   214,  214,  150,  214,  214,  214,  214,  214,  214,  214,
 /*  5210 */   214,  159,  160,  161,  162,  163,  164,  165,  166,  167,
 /*  5220 */   168,  169,  170,  171,  172,  173,  174,  175,  176,  177,
 /*  5230 */   178,  179,  180,  127,  128,  214,  214,  214,  214,  214,
 /*  5240 */   188,  214,  214,  214,  214,  214,  214,  214,  214,  214,
 /*  5250 */   198,  199,  200,  201,  214,  214,  150,  214,  214,  214,
 /*  5260 */   214,  214,  214,  214,  214,  159,  160,  161,  162,  163,
 /*  5270 */   164,  165,  166,  167,  168,  169,  170,  171,  172,  173,
 /*  5280 */   174,  175,  176,  177,  178,  179,  180,  127,  128,  214,
 /*  5290 */   214,  214,  214,  214,  188,  214,  214,  214,  214,  214,
 /*  5300 */   214,  214,  214,  214,  198,  199,  200,  201,  214,  214,
 /*  5310 */   150,  214,  214,  214,  214,  214,  214,  214,  214,  159,
 /*  5320 */   160,  161,  162,  163,  164,  165,  166,  167,  168,  169,
 /*  5330 */   170,  171,  172,  173,  174,  175,  176,  177,  178,  179,
 /*  5340 */   180,  127,  128,  214,  214,  214,  214,  214,  188,  214,
 /*  5350 */   214,  214,  214,  214,  214,  214,  214,  214,  198,  199,
 /*  5360 */   200,  201,  214,  214,  150,  214,  214,  214,  214,  214,
 /*  5370 */   214,  214,  214,  159,  160,  161,  162,  163,  164,  165,
 /*  5380 */   166,  167,  168,  169,  170,  171,  172,  173,  174,  175,
 /*  5390 */   176,  177,  178,  179,  180,  127,  128,  214,  214,  214,
 /*  5400 */   214,  214,  188,  214,  214,  214,  214,  214,  214,  214,
 /*  5410 */   214,  214,  198,  199,  200,  201,  214,  214,  150,  214,
 /*  5420 */   214,  214,  214,  214,  214,  214,  214,  159,  160,  161,
 /*  5430 */   162,  163,  164,  165,  166,  167,  168,  169,  170,  171,
 /*  5440 */   172,  173,  174,  175,  176,  177,  178,  179,  180,  127,
 /*  5450 */   128,  214,  214,  214,  214,  214,  188,  214,  214,  214,
 /*  5460 */   214,  214,  214,  214,  214,  214,  198,  199,  200,  201,
 /*  5470 */   214,  214,  150,  214,  214,  214,  214,  214,  214,  214,
 /*  5480 */   214,  159,  160,  161,  162,  163,  164,  165,  166,  167,
 /*  5490 */   168,  169,  170,  171,  172,  173,  174,  175,  176,  177,
 /*  5500 */   178,  179,  180,  127,  128,  214,  214,  214,  214,  214,
 /*  5510 */   188,  214,  214,  214,  214,  214,  214,  214,  214,  214,
 /*  5520 */   198,  199,  200,  201,  214,  214,  150,  214,  214,  214,
 /*  5530 */   214,  214,  214,  214,  214,  159,  160,  161,  162,  163,
 /*  5540 */   164,  165,  166,  167,  168,  169,  170,  171,  172,  173,
 /*  5550 */   174,  175,  176,  177,  178,  179,  180,  127,  128,  214,
 /*  5560 */   214,  214,  214,  214,  188,  214,  214,  214,  214,  214,
 /*  5570 */   214,  214,  214,  214,  198,  199,  200,  201,  214,  214,
 /*  5580 */   150,  214,  214,  214,  214,  214,  214,  214,  214,  159,
 /*  5590 */   160,  161,  162,  163,  164,  165,  166,  167,  168,  169,
 /*  5600 */   170,  171,  172,  173,  174,  175,  176,  177,  178,  179,
 /*  5610 */   180,  127,  128,  214,  214,  214,  214,  214,  188,  214,
 /*  5620 */   214,  214,  214,  214,  214,  214,  214,  214,  198,  199,
 /*  5630 */   200,  201,  214,  214,  150,  214,  214,  214,  214,  214,
 /*  5640 */   214,  214,  214,  159,  160,  161,  162,  163,  164,  165,
 /*  5650 */   166,  167,  168,  169,  170,  171,  172,  173,  174,  175,
 /*  5660 */   176,  177,  178,  179,  180,  127,  128,  214,  214,  214,
 /*  5670 */   214,  214,  188,  214,  214,  214,  214,  214,  214,  214,
 /*  5680 */   214,  214,  198,  199,  200,  201,  214,  214,  150,  214,
 /*  5690 */   214,  214,  214,  214,  214,  214,  214,  159,  160,  161,
 /*  5700 */   162,  163,  164,  165,  166,  167,  168,  169,  170,  171,
 /*  5710 */   172,  173,  174,  175,  176,  177,  178,  179,  180,  127,
 /*  5720 */   128,  214,  214,  214,  214,  214,  188,  214,  214,  214,
 /*  5730 */   214,  214,  214,  214,  214,  214,  198,  199,  200,  201,
 /*  5740 */   214,  214,  150,  214,  214,  214,  214,  214,  214,  214,
 /*  5750 */   214,  159,  160,  161,  162,  163,  164,  165,  166,  167,
 /*  5760 */   168,  169,  170,  171,  172,  173,  174,  175,  176,  177,
 /*  5770 */   178,  179,  180,  127,  128,  214,  214,  214,  214,  214,
 /*  5780 */   188,  214,  214,  214,  214,  214,  214,  214,  214,  214,
 /*  5790 */   198,  199,  200,  201,  214,  214,  150,  214,  214,  214,
 /*  5800 */   214,  214,  214,  214,  214,  159,  160,  161,  162,  163,
 /*  5810 */   164,  165,  166,  167,  168,  169,  170,  171,  172,  173,
 /*  5820 */   174,  175,  176,  177,  178,  179,  180,  127,  128,  214,
 /*  5830 */   214,  214,  214,   41,  188,  214,   44,  214,  214,  214,
 /*  5840 */   214,  214,  214,  214,  198,  199,  200,  201,  214,  214,
 /*  5850 */   150,   59,  214,  214,  214,  214,   64,  214,  214,  159,
 /*  5860 */   160,  161,  162,  163,  164,  165,  166,  167,  168,  169,
 /*  5870 */   170,  171,  172,  173,  174,  175,  176,  177,  178,  179,
 /*  5880 */   180,  127,  128,  214,  214,  214,  214,  214,  188,  214,
 /*  5890 */   214,  214,  214,  214,  214,  214,  104,  214,  198,  199,
 /*  5900 */   200,  201,  214,  214,  112,  113,  114,  115,  116,  214,
 /*  5910 */   214,  214,  214,  159,  160,  161,  162,  163,  164,  165,
 /*  5920 */   166,  167,  168,  169,  170,  171,  172,  173,  174,  175,
 /*  5930 */   176,  177,  178,  179,  180,  214,  214,  214,  214,  214,
 /*  5940 */   214,  214,  188,  214,  214,  135,  214,  137,  214,  139,
 /*  5950 */   214,  141,  198,  199,  200,  201,  146,  214,  148,  214,
 /*  5960 */   214,    8,  152,  214,   11,   12,   13,   14,   15,   16,
 /*  5970 */    17,   18,   19,   20,   21,   22,   23,   24,   25,   26,
 /*  5980 */    27,   28,   29,   30,   31,   32,  214,  214,  214,  214,
 /*  5990 */   214,  214,  214,    8,   41,   42,   11,   12,   13,   14,
 /*  6000 */    15,   16,   17,   18,   19,   20,   21,   22,   23,   24,
 /*  6010 */    25,   26,   27,   28,   29,   30,   31,   32,    0,   66,
 /*  6020 */   214,  214,  214,  214,  214,    8,   41,   42,   11,   12,
 /*  6030 */    13,   14,   15,   16,   17,   18,   19,   20,   21,   22,
 /*  6040 */    23,   24,   25,   26,   27,   28,   29,   30,   31,   32,
 /*  6050 */   214,   41,  214,  214,  214,  214,  214,   40,   41,   42,
 /*  6060 */   214,   43,   44,  214,   46,  214,   48,   57,   50,  214,
 /*  6070 */    52,   53,   87,  214,   56,  214,  214,  214,  214,  214,
 /*  6080 */   214,    8,  214,  214,   11,   12,   13,   14,   15,   16,
 /*  6090 */    17,   18,   19,   20,   21,   22,   23,   24,   25,   26,
 /*  6100 */    27,   28,   29,   30,   31,   32,  214,  214,   98,   99,
 /*  6110 */   100,  101,  102,  103,   41,   42,  106,  107,  214,  214,
 /*  6120 */   214,  214,  214,  214,  214,  214,  214,   54,  214,  214,
 /*  6130 */   214,  214,  214,  214,  214,  117,    8,  214,  214,   11,
 /*  6140 */    12,   13,   14,   15,   16,   17,   18,   19,   20,   21,
 /*  6150 */    22,   23,   24,   25,   26,   27,   28,   29,   30,   31,
 /*  6160 */    32,  214,  214,  214,  214,  214,  214,  214,    8,   41,
 /*  6170 */    42,   11,   12,   13,   14,   15,   16,   17,   18,   19,
 /*  6180 */    20,   21,   22,   23,   24,   25,   26,   27,   28,   29,
 /*  6190 */    30,   31,   32,  214,   66,  214,  214,  214,  214,  214,
 /*  6200 */   214,   41,   42,    8,  214,   45,   11,   12,   13,   14,
 /*  6210 */    15,   16,   17,   18,   19,   20,   21,   22,   23,   24,
 /*  6220 */    25,   26,   27,   28,   29,   30,   31,   32,  214,  214,
 /*  6230 */   214,  214,  214,  214,  214,  214,   41,   42,  214,  214,
 /*  6240 */    45,  214,  214,  214,  214,    8,  214,  214,   11,   12,
 /*  6250 */    13,   14,   15,   16,   17,   18,   19,   20,   21,   22,
 /*  6260 */    23,   24,   25,   26,   27,   28,   29,   30,   31,   32,
 /*  6270 */   214,  214,  214,  214,  214,  214,  214,  214,   41,   42,
 /*  6280 */   214,  214,   45,  214,  214,  214,  214,    8,  214,  214,
 /*  6290 */    11,   12,   13,   14,   15,   16,   17,   18,   19,   20,
 /*  6300 */    21,   22,   23,   24,   25,   26,   27,   28,   29,   30,
 /*  6310 */    31,   32,  214,    1,    2,    3,    4,    5,  214,  214,
 /*  6320 */    41,   42,  214,  214,   45,  214,  214,  214,  214,    8,
 /*  6330 */   214,  214,   11,   12,   13,   14,   15,   16,   17,   18,
 /*  6340 */    19,   20,   21,   22,   23,   24,   25,   26,   27,   28,
 /*  6350 */    29,   30,   31,   32,  214,  214,  214,  214,  214,  214,
 /*  6360 */   214,  214,   41,   42,   52,   53,  214,   55,   56,  214,
 /*  6370 */    58,  214,  214,  214,   62,   54,    1,    2,    3,    4,
 /*  6380 */     5,  214,  214,  214,    8,  214,  214,   11,   12,   13,
 /*  6390 */    14,   15,   16,   17,   18,   19,   20,   21,   22,   23,
 /*  6400 */    24,   25,   26,   27,   28,   29,   30,   31,   32,  214,
 /*  6410 */   214,  214,  214,  188,  214,  214,  214,   41,   42,   44,
 /*  6420 */   214,  214,  214,  198,  199,  200,  201,   52,   53,  204,
 /*  6430 */    54,  214,  207,  208,  209,   60,  214,   62,  214,    8,
 /*  6440 */   214,  214,   11,   12,   13,   14,   15,   16,   17,   18,
 /*  6450 */    19,   20,   21,   22,   23,   24,   25,   26,   27,   28,
 /*  6460 */    29,   30,   31,   32,  214,  214,  214,  214,  214,  214,
 /*  6470 */   214,  214,   41,   42,    1,    2,    3,    4,    5,  214,
 /*  6480 */   214,  214,  214,   43,   44,   54,   46,  214,   48,  214,
 /*  6490 */    50,  214,   52,   53,    8,  214,   56,   11,   12,   13,
 /*  6500 */    14,   15,   16,   17,   18,   19,   20,   21,   22,   23,
 /*  6510 */    24,   25,   26,   27,   28,   29,   30,   31,   32,    7,
 /*  6520 */   214,    9,   10,  214,  214,   52,   53,   41,   42,  214,
 /*  6530 */   214,   58,  214,   21,  214,   62,  214,  214,  214,  214,
 /*  6540 */    28,  214,  214,  214,  214,   33,   34,   35,   36,   37,
 /*  6550 */    38,   39,   40,   41,  214,  214,   44,  117,    7,  214,
 /*  6560 */     9,   10,  214,  214,  214,  214,   54,  214,  214,  214,
 /*  6570 */   214,   59,   21,   61,  214,  214,   64,  214,  214,   28,
 /*  6580 */   214,  214,  214,  214,   33,   34,   35,   36,   37,   38,
 /*  6590 */    39,   40,   41,  214,  214,   44,  214,  214,  214,  214,
 /*  6600 */   214,  214,  214,  214,  214,   54,  214,  214,  214,  214,
 /*  6610 */    59,  214,   61,  214,  214,   64,  104,    1,    2,    3,
 /*  6620 */     4,    5,  214,  214,  112,  113,  114,  115,  116,   12,
 /*  6630 */    13,   14,   15,   16,   17,   18,   19,   20,   21,   22,
 /*  6640 */    23,   24,   25,   26,   27,   28,   29,   30,   31,   32,
 /*  6650 */   214,  214,  214,  214,  214,  104,  214,  214,   41,   42,
 /*  6660 */    44,  214,  214,  112,  113,  114,  115,  116,   52,   53,
 /*  6670 */   214,    7,  214,    9,   10,  214,   60,  214,   62,  214,
 /*  6680 */   214,  214,  214,  214,  214,   21,  214,  214,  214,  214,
 /*  6690 */   214,  214,   28,  214,  214,  214,  214,   33,   34,   35,
 /*  6700 */    36,   37,   38,   39,   40,   41,  214,  214,   44,  214,
 /*  6710 */     7,  214,    9,   10,  214,  214,  214,  214,   54,  214,
 /*  6720 */   214,  214,  214,   59,   21,   61,  214,  214,   64,  214,
 /*  6730 */   214,   28,  214,  214,  214,  214,   33,   34,   35,   36,
 /*  6740 */    37,   38,   39,  214,   41,  214,  214,   44,  214,  214,
 /*  6750 */   214,  214,  214,  214,  214,  214,  214,   54,  214,  214,
 /*  6760 */   214,  214,   59,  214,   61,  214,  214,   64,  104,   66,
 /*  6770 */     1,    2,    3,    4,    5,  214,  112,  113,  114,  115,
 /*  6780 */   116,   11,   12,   13,   14,   15,   16,   17,   18,   19,
 /*  6790 */    20,   21,   22,   23,   24,   25,   26,   27,   28,   29,
 /*  6800 */    30,   31,   32,  214,  214,  214,  214,  104,  214,  214,
 /*  6810 */   214,   41,   42,  214,  214,  112,  113,  114,  115,  116,
 /*  6820 */   214,   52,   53,    7,  214,    9,   10,  214,  214,   60,
 /*  6830 */   214,   62,  214,  214,  214,  214,  214,   21,  214,  214,
 /*  6840 */   214,  214,  214,  214,   28,  214,  214,  214,  214,   33,
 /*  6850 */    34,   35,   36,   37,   38,   39,   40,   41,  214,  214,
 /*  6860 */    44,  214,    7,  214,    9,   10,  214,  214,  214,  214,
 /*  6870 */    54,  214,  214,  214,  214,   59,   21,   61,  214,  214,
 /*  6880 */    64,  214,  214,   28,  214,  214,  214,  214,   33,   34,
 /*  6890 */    35,   36,   37,   38,   39,   40,   41,  214,  214,   44,
 /*  6900 */   214,  214,  214,  214,  214,  214,  214,  214,  214,   54,
 /*  6910 */   214,  214,  214,  214,   59,  214,   61,  214,  214,   64,
 /*  6920 */   104,  214,    1,    2,    3,    4,    5,  214,  112,  113,
 /*  6930 */   114,  115,  116,   13,   14,   15,   16,   17,   18,   19,
 /*  6940 */    20,   21,   22,   23,   24,   25,   26,   27,   28,   29,
 /*  6950 */    30,   31,   32,  214,  214,  214,  214,  214,  214,  104,
 /*  6960 */   214,   41,   42,  214,  214,  214,  214,  112,  113,  114,
 /*  6970 */   115,  116,  214,   52,   53,    7,  214,    9,   10,  214,
 /*  6980 */   214,   60,  214,   62,  214,  214,  214,  214,  214,   21,
 /*  6990 */   214,  214,  214,  214,  214,  214,   28,  214,  214,  214,
 /*  7000 */   214,   33,   34,   35,   36,   37,   38,   39,   40,   41,
 /*  7010 */   214,  214,   44,  214,    7,  214,    9,   10,  214,  214,
 /*  7020 */   214,  214,   54,  214,  214,  214,  214,   59,   21,   61,
 /*  7030 */   214,  214,   64,  214,  214,   28,  214,  214,  214,  214,
 /*  7040 */    33,   34,   35,   36,   37,   38,   39,   40,   41,  214,
 /*  7050 */   214,   44,  214,  214,  214,  214,  214,  214,  214,  214,
 /*  7060 */   214,   54,  214,  214,  214,  214,   59,  214,   61,  214,
 /*  7070 */   214,   64,  104,  214,    1,    2,    3,    4,    5,  214,
 /*  7080 */   112,  113,  114,  115,  116,  214,   14,   15,   16,   17,
 /*  7090 */    18,   19,   20,   21,   22,   23,   24,   25,   26,   27,
 /*  7100 */    28,   29,   30,   31,   32,  214,  214,  214,  214,  214,
 /*  7110 */   214,  104,  214,   41,   42,  214,  214,  214,  214,  112,
 /*  7120 */   113,  114,  115,  116,  214,   52,   53,    7,  214,    9,
 /*  7130 */    10,  214,  214,   60,  214,   62,  214,  214,  214,  214,
 /*  7140 */   214,   21,  214,  214,  214,  214,  214,  214,   28,  214,
 /*  7150 */   214,  214,  214,   33,   34,   35,   36,   37,   38,   39,
 /*  7160 */    40,   41,  214,  214,   44,  214,    7,  214,    9,   10,
 /*  7170 */   214,  214,  214,  214,   54,  214,  214,  214,  214,   59,
 /*  7180 */    21,   61,  214,  214,   64,  214,  214,   28,  214,  214,
 /*  7190 */   214,   21,   33,   34,   35,   36,   37,   38,   39,   40,
 /*  7200 */    41,  214,  214,   44,  214,    1,    2,    3,    4,    5,
 /*  7210 */    40,  214,  214,   54,   44,  214,  214,  214,   59,  214,
 /*  7220 */    61,  214,  214,   64,  104,  214,  214,  214,   58,  214,
 /*  7230 */   214,  214,  112,  113,  114,  115,  116,   67,   68,   69,
 /*  7240 */    70,   71,   72,   73,   74,   75,   76,   77,   78,   79,
 /*  7250 */    80,  214,  214,  214,  214,  214,   52,   53,  214,   55,
 /*  7260 */    56,  214,   58,  104,  214,  214,   62,  214,  214,  214,
 /*  7270 */   214,  112,  113,  114,  115,  116,  188,  214,  214,    7,
 /*  7280 */   214,    9,   10,  214,  214,  214,  198,  199,  200,  201,
 /*  7290 */   214,  214,  214,   21,  214,  207,  208,  209,  214,  214,
 /*  7300 */    28,  214,  214,  214,  214,   33,   34,   35,   36,   37,
 /*  7310 */    38,   39,   40,   41,  214,  214,   44,  214,    7,  214,
 /*  7320 */     9,   10,  214,  214,  214,  214,   54,  214,  214,  214,
 /*  7330 */   214,   59,   21,   61,  214,  214,   64,  214,  214,   28,
 /*  7340 */   214,  214,  214,   21,   33,   34,   35,   36,   37,   38,
 /*  7350 */    39,  214,   41,  214,  214,   44,  214,    1,    2,    3,
 /*  7360 */     4,    5,   40,  214,  214,   54,   44,  214,  214,  214,
 /*  7370 */    59,  214,   61,  214,  214,   64,  104,  214,  214,  214,
 /*  7380 */    58,  214,  214,  214,  112,  113,  114,  115,  116,   67,
 /*  7390 */    68,   69,   70,   71,   72,   73,   74,   75,   76,   77,
 /*  7400 */    78,   79,   80,  214,  214,  214,  214,   96,   52,   53,
 /*  7410 */   214,   55,   56,  214,   58,  104,  214,  214,   62,  214,
 /*  7420 */   214,  214,  214,  112,  113,  114,  115,  116,  214,  214,
 /*  7430 */   214,    7,  214,    9,   10,  214,  214,  214,  214,  214,
 /*  7440 */   214,  214,  214,  214,  214,   21,  214,  214,  214,  214,
 /*  7450 */   214,  214,   28,  214,  214,  214,  214,   33,   34,   35,
 /*  7460 */    36,   37,   38,   39,  214,   41,  214,  214,   44,  214,
 /*  7470 */     7,  214,    9,   10,  214,  214,  214,  214,   54,  214,
 /*  7480 */   214,  214,  214,   59,   21,   61,  214,  214,   64,  214,
 /*  7490 */    66,   28,  214,  214,  214,  214,   33,   34,   35,   36,
 /*  7500 */    37,   38,   39,  214,   41,   21,  214,   44,  214,  214,
 /*  7510 */   214,  214,  214,  214,  214,  214,  214,   54,  214,  214,
 /*  7520 */   214,  214,   59,  214,   61,  214,  214,   64,  104,   66,
 /*  7530 */    41,   42,  214,  214,  214,  214,  112,  113,  114,  115,
 /*  7540 */   116,  214,  214,  214,  214,  214,   57,   63,   64,   65,
 /*  7550 */   214,   67,   68,   69,   70,   71,   72,   73,   74,   75,
 /*  7560 */    76,   77,   78,   79,   80,  214,  214,  104,  214,  214,
 /*  7570 */   214,  214,  214,  214,  214,  112,  113,  114,  115,  116,
 /*  7580 */   214,  214,  214,    7,  214,    9,   10,   98,   99,  100,
 /*  7590 */   101,  102,  103,  214,  105,  106,  107,   21,  214,  214,
 /*  7600 */   214,  214,  214,  214,   28,  214,  214,  214,  214,   33,
 /*  7610 */    34,   35,   36,   37,   38,   39,  214,   41,  214,  214,
 /*  7620 */    44,  214,    7,  214,    9,   10,  214,  214,  214,  214,
 /*  7630 */    54,  214,  214,  214,  214,   59,   21,   61,  214,  214,
 /*  7640 */    64,  214,   66,   28,  214,  214,  214,   21,   33,   34,
 /*  7650 */    35,   36,   37,   38,   39,  214,   41,  214,  214,   44,
 /*  7660 */   214,    1,    2,    3,    4,    5,   40,  214,  214,   54,
 /*  7670 */    44,  214,  214,  214,   59,  214,   61,  214,  214,   64,
 /*  7680 */   104,   66,  214,  214,   58,  214,  214,  214,  112,  113,
 /*  7690 */   114,  115,  116,   67,   68,   69,   70,   71,   72,   73,
 /*  7700 */    74,   75,   76,   77,   78,   79,   80,    1,    2,    3,
 /*  7710 */     4,    5,   52,   53,  214,   55,   56,  214,   58,  104,
 /*  7720 */   214,  214,   62,  214,  214,  214,  214,  112,  113,  114,
 /*  7730 */   115,  116,  214,  214,  214,    7,  214,    9,   10,  214,
 /*  7740 */   214,  214,  214,  214,  214,  214,  214,  214,  214,   21,
 /*  7750 */   214,  214,  214,  214,  214,  214,   28,  214,   52,   53,
 /*  7760 */   214,   33,   34,   35,   36,   37,   38,   39,   62,   41,
 /*  7770 */   214,  214,   44,  214,    7,  214,    9,   10,  214,  214,
 /*  7780 */   214,  214,   54,  214,  214,  214,  214,   59,   21,   61,
 /*  7790 */   214,  214,   64,  214,   66,   28,  214,  214,  214,   21,
 /*  7800 */    33,   34,   35,   36,   37,   38,   39,  214,   41,  214,
 /*  7810 */   214,   44,  214,    1,    2,    3,    4,    5,   40,  214,
 /*  7820 */   214,   54,   44,  214,  214,  214,   59,  214,   61,  214,
 /*  7830 */   214,   64,  104,   66,  214,  214,   58,  214,  214,  214,
 /*  7840 */   112,  113,  114,  115,  116,   67,   68,   69,   70,   71,
 /*  7850 */    72,   73,   74,   75,   76,   77,   78,   79,   80,  214,
 /*  7860 */   214,  214,  214,  214,   52,   53,  214,   55,   56,  214,
 /*  7870 */    58,  104,  214,  214,   62,  214,  214,  214,  214,  112,
 /*  7880 */   113,  114,  115,  116,  214,  214,  214,    7,  214,    9,
 /*  7890 */    10,  214,  214,  214,  214,  214,  214,  214,  214,  214,
 /*  7900 */   214,   21,  214,  214,  214,  214,  214,  214,   28,  214,
 /*  7910 */   214,  214,  214,   33,   34,   35,   36,   37,   38,   39,
 /*  7920 */   214,   41,  214,  214,   44,   45,    7,  214,    9,   10,
 /*  7930 */   214,  214,  214,  214,   54,  214,  214,  214,  214,   59,
 /*  7940 */    21,   61,  214,  214,   64,  214,  214,   28,  214,  214,
 /*  7950 */   214,   21,   33,   34,   35,   36,   37,   38,   39,  214,
 /*  7960 */    41,  214,  214,   44,  214,    1,    2,    3,    4,    5,
 /*  7970 */   214,  214,  214,   54,   44,  214,  214,  214,   59,  214,
 /*  7980 */    61,  214,  214,   64,  104,  214,  214,  214,   58,  214,
 /*  7990 */   214,  214,  112,  113,  114,  115,  116,   67,   68,   69,
 /*  8000 */    70,   71,   72,   73,   74,   75,   76,   77,   78,   79,
 /*  8010 */    80,  214,  214,  214,  214,   96,   52,   53,  214,   55,
 /*  8020 */    56,  214,   58,  104,  214,  214,   62,  214,  214,  214,
 /*  8030 */   214,  112,  113,  114,  115,  116,  214,  214,  214,    7,
 /*  8040 */   214,    9,   10,  214,  214,  214,  214,  214,  214,  214,
 /*  8050 */   214,  214,  214,   21,  214,  214,  214,  214,  214,  214,
 /*  8060 */    28,  214,  214,  214,  214,   33,   34,   35,   36,   37,
 /*  8070 */    38,   39,  214,   41,  214,  214,   44,  214,    7,  214,
 /*  8080 */     9,   10,  214,  214,  214,  214,   54,  214,  214,  214,
 /*  8090 */   214,   59,   21,   61,  214,  214,   64,  214,  214,   28,
 /*  8100 */   214,  214,  214,  214,   33,   34,   35,   36,   37,   38,
 /*  8110 */    39,  214,   41,   21,  214,   44,  214,  214,  214,  214,
 /*  8120 */   214,  214,  214,  214,  214,   54,  214,  214,  214,  214,
 /*  8130 */    59,  214,   61,  214,  214,   64,  104,    1,    2,    3,
 /*  8140 */     4,    5,  214,  214,  112,  113,  114,  115,  116,    1,
 /*  8150 */     2,    3,    4,    5,  214,  214,   64,   65,  214,   67,
 /*  8160 */    68,   69,   70,   71,   72,   73,   74,   75,   76,   77,
 /*  8170 */    78,   79,   80,  214,  214,  104,  214,  214,  214,  214,
 /*  8180 */   214,  214,  214,  112,  113,  114,  115,  116,   52,   53,
 /*  8190 */   214,    7,  214,    9,   10,  214,   60,  214,   62,  214,
 /*  8200 */    52,   53,  214,   55,   56,   21,   21,  214,   41,  214,
 /*  8210 */    62,   44,   28,  214,  214,  214,  214,   33,   34,   35,
 /*  8220 */    36,   37,   38,   39,  214,   41,   59,  214,   44,   44,
 /*  8230 */   214,   64,  214,  214,  214,  214,  214,  214,   54,  214,
 /*  8240 */   214,  214,  214,   59,  214,   61,  214,  214,   64,  214,
 /*  8250 */   214,  214,   67,   68,   69,   70,   71,   72,   73,   74,
 /*  8260 */    75,   76,   77,   78,   79,   80,    1,    2,    3,    4,
 /*  8270 */     5,  104,  214,  214,  214,  214,  214,  214,  214,  112,
 /*  8280 */   113,  114,  115,  116,  214,  214,  214,  214,  104,  214,
 /*  8290 */     1,    2,    3,    4,    5,  214,  112,  113,  114,  115,
 /*  8300 */   116,    1,    2,    3,    4,    5,    1,    2,    3,    4,
 /*  8310 */     5,  214,  214,  214,  214,  214,  214,   52,   53,  214,
 /*  8320 */    55,   56,    1,    2,    3,    4,    5,   62,  214,  214,
 /*  8330 */   214,  214,  214,  214,  214,  214,  214,  214,  214,  214,
 /*  8340 */   214,   52,   53,  214,   55,   56,    1,    2,    3,    4,
 /*  8350 */     5,   62,   52,   53,  214,   55,   56,   52,   53,  214,
 /*  8360 */    55,   56,   62,  214,  214,  214,  214,   62,  214,  214,
 /*  8370 */   214,  214,  214,   52,   53,  214,   55,   56,    1,    2,
 /*  8380 */     3,    4,    5,   62,  214,  214,  214,  214,  214,  214,
 /*  8390 */   214,  214,  214,  214,  214,  214,  214,   52,   53,  214,
 /*  8400 */    55,   56,  214,  214,  214,  214,  214,   62,  214,  214,
 /*  8410 */   214,  214,  214,  214,  214,  214,  214,  214,  214,  214,
 /*  8420 */   214,  214,  214,  214,  214,  214,  214,  214,  214,   52,
 /*  8430 */    53,  214,   55,   56,  214,  214,  214,  214,  214,   62,
};
#define YY_SHIFT_USE_DFLT (-29)
static short yy_shift_ofst[] = {
 /*     0 */  6440,   15, 6018,  -29,  -29,  -29,  -29,  -29,  -29,  -29,
 /*    10 */    82,  162,  -29,  122,   90,  -29,  122,  -29,  247,  290,
 /*    20 */   -29,  -29,  435,   67,  -29,  442,  415,  -29, 6312,  -29,
 /*    30 */   477,  -29, 7204, 8148,  -29, 6769,  547,  545, 7170,  216,
 /*    40 */  7484,  610,  -29,  -29,  672, 8092,  -29,  663,  -29,  -29,
 /*    50 */   -29,  -29,  -29,  664,   17,  -29,  696,  744,  -29,  -29,
 /*    60 */   -29,  -29,  -29,  -29,  -29,  -29,  -29,  -29,  -29,  -29,
 /*    70 */   -29,  -29,  -29,  -29,  -29,  121,  220, 7484,  768,  -29,
 /*    80 */   -29, 7930,  -29,  776, 5792,  -29,  -29,  -29,  -29,  -29,
 /*    90 */   -29,  -29,  -29,  683,  795,  -29,  -29, 4645,  763,  837,
 /*   100 */   136,  -29,  140,  -29, 8167,  -29,  859, 5792,  -29,  -29,
 /*   110 */   -29,  -29, 8185,  874, 5792,  -29,  203,  911, 5792,  -29,
 /*   120 */   944,  966, 5792,  -29,  990,  983, 5792,  -29, 1005, 1028,
 /*   130 */   329,  995, 5792,  -29, 1011, 1001, 5792,  -29, 1027, 1015,
 /*   140 */  5792,  -29,  -29,  -29,  -29,  -29,  -29,  -29,  -29,  -29,
 /*   150 */   -29,  -29, 7706, 6921, 1034, 1018, 7322,  344, 7484, 1040,
 /*   160 */   -29,  -29,  304,  438, 7484, 1042,  -29,  -29,  -29,  -29,
 /*   170 */   -29, 6473, 1029, 1037, 5792, 1052,  -29, 1039, 1054, 5792,
 /*   180 */  1069,  -29, 8265,  -29, 1072,  448,  -29, 1074,  449,  -29,
 /*   190 */  1077,  352,  -29, 7356,  -29, 1068,  -29, 7660, 8289,  -29,
 /*   200 */  7073, 1085, 1071, 7626,  350,   -7,  -29,  104,  -29,  -29,
 /*   210 */   -29,  -29,  -29,  -29,  -29,  -29,  -29,  -29,  -29,  -29,
 /*   220 */   -29,  -29,  -29,  -29,  -29,  -29,  -29,  -29,  -29,  -29,
 /*   230 */   -29,  -29,  -29,  -29, 1088,  -29, 1089,  -29, 8032, 1076,
 /*   240 */   215,  326, 1051, 1091,  437,  548,  -29, 8032, 1092,  -15,
 /*   250 */   -29,  102,  -29,  -29, 5792, 1064, 4433, 4433, 1098,  659,
 /*   260 */   770,  -29, 8032, 1106,  881,  992,  -29, 1107, 1103, 1214,
 /*   270 */  1073, 8032, 1120,  -29, 8032,  260, 8032, 8032,  786, 8032,
 /*   280 */  8032,  260, 8032, 8032, 6486, 8032, 8032,  260, 8032, 8032,
 /*   290 */   260, 8032, 8032, 6770, 8032, 8032, 6770, 8032,   14,  264,
 /*   300 */  1105, 6512, 6486, 8032, 5953,  -29, 8032,  786, 8032,  786,
 /*   310 */  8032,  260, 8032,  260, 8032,  260, 8032,  786, 8032, 7072,
 /*   320 */  8032, 6920, 8032, 4612, 8032, 4612, 8032, 4612, 8032, 4612,
 /*   330 */  8032, 4612, 8032, 6617, 8032, 5985, 8032, 6486, 4544, 6017,
 /*   340 */   -29, 1128, 8032,  260, 1125, 1159, 8032,   14,   86,  369,
 /*   350 */  1121, 6551,  -28, 6664, 1139, 1181, 8032,  260,  -29, 8032,
 /*   360 */   260,  -29,  -29,  -29,  -29,  -29,  -29,  -29,  -29, 6703,
 /*   370 */  6486,  194, 1102, 1109,  -29,  339,  -29, 8071,  401, 1129,
 /*   380 */  6816,  -29,  538,  -29, 8184, 1154, 1144,  351, 6855,  550,
 /*   390 */   -29,  -29,  -29,  -29,  -29,  -29,  479, 1145, 6968,  524,
 /*   400 */   -29,  -29, 1161, 1153, 1148, 7007,  534,  -29,  -29,  -29,
 /*   410 */  1166, 1167, 1164, 7120,  -29,  554,  -29,  -29, 1140, 8032,
 /*   420 */   -29,  -29,  -29,  573,  -29,  -29, 8032, 6486,  574,  -29,
 /*   430 */   -29,  -29,  661,  -29,  -29,  325, 1173, 1168, 7159,  699,
 /*   440 */   -29,  -29, 1176, 1171, 7272,  772,  -29,  -29,   14,   14,
 /*   450 */    14,   14,   14,   14,   14, 6486, 1180, 1325, 1436, 1147,
 /*   460 */  1147,  -29, 1212,  130, 1547, 1658,  -29, 1213,  515, 7311,
 /*   470 */  6073, 1769, 1880,  -29,  551,  346,  -29,  551,  -29, 7489,
 /*   480 */   -29,  -29,  -29,  -29,  -29,  -29,  -29, 8032,  -29, 6486,
 /*   490 */   582, 6010, 8032,  -29, 7424, 4927, 8032,  -29, 1195,  -29,
 /*   500 */  6128, 4712, 8032,  -29, 7463, 4927, 8032,  -29,  -29,  -29,
 /*   510 */   -29,  -29,  420, 1207, 4927, 8032,  -29, 1208, 4927, 8032,
 /*   520 */   -29, 1232, 4765, 8032,  -29, 7576, 4927, 8032,  -29, 4819,
 /*   530 */  8032,  -29, 7615, 4927, 8032,  -29, 7728, 4927, 8032,  -29,
 /*   540 */  4873, 8032,  -29, 7767, 4927, 8032,  -29,  -29,  -29, 1235,
 /*   550 */  1238, 4927, 8032,  -29,  -29, 8032,  671,  -29, 8032,  -29,
 /*   560 */  6486,  -29, 1249,  -29, 1253,  -29, 1255,  -29, 1271,  -29,
 /*   570 */  7880, 6160,  -29,  -29, 8032, 6195,  -29, 8032, 6237,  -29,
 /*   580 */  8032, 6279,  -29, 1236,  791,  -29, 1236,  -29, 1262, 5792,
 /*   590 */   -29,  -29, 1236,  824,  -29, 1236,  826,  -29, 1236,  862,
 /*   600 */   -29, 1236,  850,  -29, 1236,  883,  -29, 1236,  902,  -29,
 /*   610 */  1236,  938,  -29, 1236,  961,  -29, 1236,  994,  -29, 1236,
 /*   620 */  1012,  -29, 6486,  -29,  -29,  -29,  -29, 8032, 6321, 4433,
 /*   630 */  1991,  -29, 1277, 1241, 7919, 6376, 2102, 2213,  -29,  -29,
 /*   640 */  8032, 6431, 4433, 2324,  -29,  -29, 1289, 1284, 2435, 2546,
 /*   650 */   -29,  -29, 1212,  -29,  -29,  -29,  -29,  -29, 1250, 8032,
 /*   660 */  1297,  -29,  -29,  -29, 1256, 4433, 4433,  -29,  -29, 1261,
 /*   670 */  1298, 2657, 2768,  -29,  -29, 7484, 1313, 2879,  -29, 2990,
 /*   680 */   -29,  921,  379, 3101,  -29, 3212,  -29, 7484, 1314, 3323,
 /*   690 */   -29, 3434,  -29, 7706, 8136, 1307, 1310, 7778,  599, 3545,
 /*   700 */   -29, 3656,  -29, 7484, 1318, 3767,  -29, 3878,  -29,  937,
 /*   710 */   701, 3989,  -29, 4100,  -29, 7484, 1319, 4211,  -29, 4322,
 /*   720 */   -29, 7812, 8300,  -29, 6473,  -29, 6473, 6375,  223,  -29,
 /*   730 */  5792,  707,  -29, 1338,  -29,  -20, 1342,  156, 1343,  963,
 /*   740 */   -29,  -29, 1347,  -29,  -29, 1358,  -29, 6616,  365,  -29,
 /*   750 */  5792,  920,  -29, 1359,  -29, 1367,  -29, 7964, 8305, 8321,
 /*   760 */  7706, 8345,  -29, 8377, 1077,  -29,  -29,  -29, 1077,  352,
 /*   770 */   -29, 1377, 1384,  670,  -29, 1386,  984,  -29, 1077,  352,
 /*   780 */   -29, 1077,  352,  -29, 1394, 1405,  521,  -29, 1406, 1400,
 /*   790 */   -29,  -29,
};
#define YY_REDUCE_USE_DFLT (-142)
static short yy_reduce_ofst[] = {
 /*     0 */  -115, -142,   28, -142, -142, -142, -142, -142, -142, -142,
 /*    10 */  -142, -142, -142,  -12, -142, -142,  151, -142, -142, -142,
 /*    20 */  -142, -142, -142,  258, -142, -142,  385, -142, 5810, -142,
 /*    30 */  -142, -142,  724,  205, -142,  387, -142, -142, -133, -142,
 /*    40 */    75, -142, -142, -142, -142, -137, -142, -142, -142, -142,
 /*    50 */  -142, -142, -142, -142, -142, -142, -142, -142, -142, -142,
 /*    60 */  -142, -142, -142, -142, -142, -142, -142, -142, -142, -142,
 /*    70 */  -142, -142, -142, -142, -142, -142, -142,  186, -142, -142,
 /*    80 */  -142, -105, -142, -142,  693, -142, -142, -142, -142, -142,
 /*    90 */  -142, -142, -142, -142, -142, -142, -142,  799, -142, -142,
 /*   100 */  -142, -142, -142, -142,   22, -142, -142, -141, -142, -142,
 /*   110 */  -142, -142,  -76, -142,  834, -142, -142, -142,  842, -142,
 /*   120 */  -142, -142,  882, -142, -142, -142,  899, -142, -142, -142,
 /*   130 */  -142, -142,  912, -142, -142, -142,  928, -142, -142, -142,
 /*   140 */   934, -142, -142, -142, -142, -142, -142, -142, -142, -142,
 /*   150 */  -142, -142,  105,  387, -142, -142,  -25, -142,  297, -142,
 /*   160 */  -142, -142, -142, -142,  493, -142, -142, -142, -142, -142,
 /*   170 */  -142,  105, -142, -142,  953, -142, -142, -142, -142,  957,
 /*   180 */  -142, -142,  205, -142, -142,  986, -142, -142,  989, -142,
 /*   190 */   161,  991, -142,  -18, -142, -142, -142,  126,  316, -142,
 /*   200 */   387, -142, -142,  743, -142, 4502, -142, 5754, -142, -142,
 /*   210 */  -142, -142, -142, -142, -142, -142, -142, -142, -142, -142,
 /*   220 */  -142, -142, -142, -142, -142, -142, -142, -142, -142, -142,
 /*   230 */  -142, -142, -142, -142, -142, -142, -142, -142,  490, -142,
 /*   240 */  4566, 5754, -142, -142, 4620, 5754, -142,  601, -142,    0,
 /*   250 */  -142,  967, -142, -142, 1007, -142, 4674, 5754, -142, 4728,
 /*   260 */  5754, -142, 4507, -142, 4782, 5754, -142, -142, 4836, 5754,
 /*   270 */  -142, 4898, -142, -142,  781, -142,  844,  892, -142,  955,
 /*   280 */  1003, -142, 1045, 1066, -142, 1114, 1156, -142, 1177, 1225,
 /*   290 */  -142, 1267, 1288, -142, 1336, 1378, -142, 1399, -142, -142,
 /*   300 */  -142, -107, -142, 1447, -142, -142, 1489, -142, 1510, -142,
 /*   310 */  1558, -142, 1600, -142, 1621, -142, 1669, -142, 1711, -142,
 /*   320 */  1732, -142, 1780, -142, 1822, -142, 1843, -142, 1891, -142,
 /*   330 */  1933, -142, 1954, -142, 2002, -142, 2044, -142, 4333, -142,
 /*   340 */  -142, -142, 2065, -142, -142, -142, 2113, -142, -142, -142,
 /*   350 */  -142,    4, -142,   39, -142, -142, 2155, -142, -142, 2176,
 /*   360 */  -142, -142, -142, -142, -142, -142, -142, -142, -142, 6225,
 /*   370 */  -142, -142, -142, -142, -142, -142, -142, 7088, -142, -142,
 /*   380 */   115, -142, -142, -142,  511, -142, -142, -142,  150, -142,
 /*   390 */  -142, -142, -142, -142, -142, -142, -142, -142,  178, -142,
 /*   400 */  -142, -142, -142, -142, -142,  226, -142, -142, -142, -142,
 /*   410 */  -142, -142, -142,  261, -142, -142, -142, -142, -142,  712,
 /*   420 */  -142, -142, -142, -142, -142, -142, 2224, -142, -142, -142,
 /*   430 */  -142, -142, -142, -142, -142, -142, -142, -142,  289, -142,
 /*   440 */  -142, -142, -142, -142,  337, -142, -142, -142, -142, -142,
 /*   450 */  -142, -142, -142, -142, -142, -142, -142, 4890, 5754,  300,
 /*   460 */  1056, -142,  330, -142, 4944, 5754, -142, -142, -142, 2266,
 /*   470 */  -142, 4998, 5754, -142,  445, -142, -142, 1070, -142,  -24,
 /*   480 */  -142, -142, -142, -142, -142, -142, -142,  705, -142, -142,
 /*   490 */  -142,    6,  816, -142,  559, 1080,  927, -142, -142, -142,
 /*   500 */  -142,  417, 1038, -142,  559, 1081, 1149, -142, -142, -142,
 /*   510 */  -142, -142, -142, -142, 1083, 1260, -142, -142, 1086, 1371,
 /*   520 */  -142, -142,  181, 1482, -142,  559, 1101, 1593, -142,  460,
 /*   530 */  1704, -142,  559, 1110, 1815, -142,  559, 1116, 1926, -142,
 /*   540 */   531, 2037, -142,  559, 1118, 2148, -142, -142, -142, -142,
 /*   550 */  -142, 1119, 2259, -142, -142,  400, -142, -142,  622, -142,
 /*   560 */  -142, -142, -142, -142, -142, -142, -142, -142, -142, -142,
 /*   570 */  2287, -142, -142, -142, 2335, -142, -142, 2370, -142, -142,
 /*   580 */  2377, -142, -142,  504, -142, -142, 1117, -142, -142, 1175,
 /*   590 */  -142, -142,  535, -142, -142,  543, -142, -142,  562, -142,
 /*   600 */  -142,  646, -142, -142,  655, -142, -142,  731, -142, -142,
 /*   610 */   736, -142, -142,  757, -142, -142,  784, -142, -142,  819,
 /*   620 */  -142, -142, -142, -142, -142, -142, -142, 2398, -142, 5052,
 /*   630 */  5754, -142, -142, -142, 2446, -142, 5106, 5754, -142, -142,
 /*   640 */  2481, -142, 5160, 5754, -142, -142, -142, -142, 5214, 5754,
 /*   650 */  -142, -142, 1152, -142, -142, -142, -142, -142, -142, 4952,
 /*   660 */  -142, -142, -142, -142, -142, 5268, 5754, -142, -142, -142,
 /*   670 */  -142, 5322, 5754, -142, -142,  519, -142, 5376, -142, 5754,
 /*   680 */  -142, -142, -142, 5430, -142, 5754, -142,  528, -142, 5484,
 /*   690 */  -142, 5754, -142,  491,  387, -142, -142,  774, -142, 5538,
 /*   700 */  -142, 5754, -142,  630, -142, 5592, -142, 5754, -142, -142,
 /*   710 */  -142, 5646, -142, 5754, -142,  639, -142, 5700, -142, 5754,
 /*   720 */  -142,  421,  170, -142,  491, -142,  725,  387, 1231, -142,
 /*   730 */  1240, 1242, -142, -142, -142,  483, -142, -142, -142, 1245,
 /*   740 */  -142, -142, -142, -142, -142, -142, -142,  387, 1266, -142,
 /*   750 */  1263, 1268, -142, -142, -142, -142, -142,  425,  -52,  170,
 /*   760 */   725,  170, -142,  170, 1286, -142, -142, -142,  741, 1287,
 /*   770 */  -142, -142, -142, 1290, -142, -142, 1300, -142,  762, 1301,
 /*   780 */  -142,  871, 1316, -142, -142, -142, 1320, -142, -142, 1322,
 /*   790 */  -142, -142,
};
static YYACTIONTYPE yy_default[] = {
 /*     0 */  1181, 1181, 1181,  794,  796,  797,  798,  799,  800,  801,
 /*    10 */  1181, 1181,  802, 1181, 1181,  803, 1181,  804,  806, 1181,
 /*    20 */   807,  805, 1181, 1181,  808, 1181, 1181,  809, 1181,  825,
 /*    30 */  1181,  826, 1181, 1181,  859, 1181, 1181, 1181, 1181, 1181,
 /*    40 */  1181, 1181,  879,  897,  898, 1181,  899,  901,  904,  902,
 /*    50 */   903,  905,  906, 1181, 1181,  925, 1181, 1181,  926,  927,
 /*    60 */   928,  929,  930,  931,  932,  933,  934,  935,  936,  937,
 /*    70 */   938,  939,  940,  900,  883, 1181, 1181, 1181, 1181,  880,
 /*    80 */   884, 1181,  907,  909, 1181,  917, 1159, 1160, 1161, 1162,
 /*    90 */  1163, 1164, 1165, 1181, 1181, 1166, 1167, 1181, 1159, 1161,
 /*   100 */  1181, 1168, 1181, 1169, 1181, 1170, 1181, 1181, 1172, 1177,
 /*   110 */  1173, 1171, 1181,  910, 1181,  918, 1181,  912, 1181,  920,
 /*   120 */  1181,  914, 1181,  922, 1181,  916, 1181,  924, 1181, 1181,
 /*   130 */  1181,  911, 1181,  919, 1181,  913, 1181,  921, 1181,  915,
 /*   140 */  1181,  923,  908,  887,  889,  890,  891,  892,  893,  894,
 /*   150 */   895,  896, 1181, 1181, 1181, 1181, 1181, 1181, 1181, 1181,
 /*   160 */   881,  885, 1181, 1181, 1181, 1181,  882,  886,  888,  855,
 /*   170 */   860, 1181, 1181, 1181, 1181, 1181,  861, 1181, 1181, 1181,
 /*   180 */  1181,  862, 1181,  856, 1181, 1181,  810, 1181, 1181,  811,
 /*   190 */  1181, 1181,  813, 1181,  820, 1181,  821, 1181, 1181,  857,
 /*   200 */  1181, 1181, 1181, 1181, 1181, 1181,  863, 1181,  865,  941,
 /*   210 */   943,  944,  945,  946,  947,  948,  949,  950,  951,  952,
 /*   220 */   953,  954,  955,  956,  957,  958,  959,  960,  961,  962,
 /*   230 */   963,  964,  965,  966, 1181,  967, 1181,  968, 1181, 1181,
 /*   240 */  1181, 1181,  971, 1181, 1181, 1181,  972, 1181, 1181, 1181,
 /*   250 */   975, 1181,  976,  977, 1181, 1181,  979,  980, 1181, 1181,
 /*   260 */  1181,  983, 1181, 1181, 1181, 1181,  985, 1181, 1181, 1181,
 /*   270 */  1181, 1181, 1181,  987, 1181, 1068, 1181, 1181, 1069, 1181,
 /*   280 */  1181, 1070, 1181, 1181, 1071, 1181, 1181, 1072, 1181, 1181,
 /*   290 */  1073, 1181, 1181, 1074, 1181, 1181, 1075, 1181, 1083, 1181,
 /*   300 */  1087, 1181, 1149, 1181, 1181, 1092, 1181, 1093, 1181, 1094,
 /*   310 */  1181, 1095, 1181, 1096, 1181, 1097, 1181, 1098, 1181, 1099,
 /*   320 */  1181, 1100, 1181, 1101, 1181, 1102, 1181, 1103, 1181, 1104,
 /*   330 */  1181, 1105, 1181, 1106, 1181, 1181, 1181, 1146, 1181, 1181,
 /*   340 */  1084, 1181, 1181, 1085, 1181, 1181, 1181, 1086, 1110, 1181,
 /*   350 */  1090, 1181, 1110, 1181, 1181, 1181, 1181, 1107, 1108, 1181,
 /*   360 */  1109, 1111, 1112, 1113, 1114, 1115, 1116, 1117, 1118, 1181,
 /*   370 */  1158, 1110, 1111, 1112, 1119, 1181, 1120, 1181, 1181, 1121,
 /*   380 */  1181, 1122, 1181, 1123, 1181, 1181, 1181, 1181, 1181, 1181,
 /*   390 */  1129, 1130, 1143, 1144, 1145, 1148, 1181, 1181, 1181, 1181,
 /*   400 */  1133, 1134, 1181, 1181, 1181, 1181, 1181, 1135, 1136, 1147,
 /*   410 */  1181, 1181, 1124, 1181, 1125, 1181, 1126, 1151, 1181, 1181,
 /*   420 */  1153, 1154, 1152, 1181, 1127, 1128, 1181, 1150, 1181, 1131,
 /*   430 */  1132, 1091, 1181, 1137, 1138, 1181, 1181, 1088, 1181, 1181,
 /*   440 */  1139, 1140, 1181, 1089, 1181, 1181, 1141, 1142, 1082, 1081,
 /*   450 */  1080, 1079, 1078, 1077, 1076, 1178, 1181, 1181, 1181,  989,
 /*   460 */   990,  991, 1181, 1181, 1181, 1181,  993, 1181, 1181, 1181,
 /*   470 */  1181, 1181, 1181, 1000, 1181, 1181, 1006, 1181, 1007, 1181,
 /*   480 */  1009, 1010, 1011, 1012, 1013, 1014, 1015, 1181, 1016, 1067,
 /*   490 */  1181, 1181, 1181, 1017, 1181, 1181, 1181, 1020, 1181, 1032,
 /*   500 */  1181, 1181, 1181, 1021, 1181, 1181, 1181, 1022, 1030, 1031,
 /*   510 */  1033, 1034, 1181, 1181, 1181, 1181, 1018, 1181, 1181, 1181,
 /*   520 */  1019, 1181, 1181, 1181, 1023, 1181, 1181, 1181, 1024, 1181,
 /*   530 */  1181, 1025, 1181, 1181, 1181, 1026, 1181, 1181, 1181, 1027,
 /*   540 */  1181, 1181, 1028, 1181, 1181, 1181, 1029, 1035, 1036, 1181,
 /*   550 */  1181, 1181, 1181, 1037, 1008, 1181, 1181, 1039, 1181, 1040,
 /*   560 */  1042, 1041, 1143, 1043, 1145, 1044, 1144, 1045, 1108, 1046,
 /*   570 */  1181, 1181, 1047, 1048, 1181, 1181, 1049, 1181, 1181, 1050,
 /*   580 */  1181, 1181, 1051, 1181, 1181, 1052, 1181, 1063, 1065, 1181,
 /*   590 */  1066, 1064, 1181, 1181, 1053, 1181, 1181, 1054, 1181, 1181,
 /*   600 */  1055, 1181, 1181, 1056, 1181, 1181, 1057, 1181, 1181, 1058,
 /*   610 */  1181, 1181, 1059, 1181, 1181, 1060, 1181, 1181, 1061, 1181,
 /*   620 */  1181, 1062, 1181, 1179, 1180,  942, 1001, 1181, 1181, 1181,
 /*   630 */  1181, 1002, 1181, 1181, 1181, 1181, 1181, 1181, 1003, 1004,
 /*   640 */  1181, 1181, 1181, 1181, 1005,  994, 1181, 1181, 1181, 1181,
 /*   650 */   996,  995, 1181,  997,  999,  998,  992,  988, 1181, 1181,
 /*   660 */  1181,  986,  984,  982, 1181, 1181,  981,  978,  973,  969,
 /*   670 */  1181, 1181, 1181,  974,  970, 1181, 1181, 1181,  871, 1181,
 /*   680 */   873, 1181, 1181, 1181,  864, 1181,  866, 1181, 1181, 1181,
 /*   690 */   872, 1181,  874, 1181, 1181, 1181, 1181, 1181, 1181, 1181,
 /*   700 */   867, 1181,  869, 1181, 1181, 1181,  875, 1181,  877, 1181,
 /*   710 */  1181, 1181,  868, 1181,  870, 1181, 1181, 1181,  876, 1181,
 /*   720 */   878, 1181, 1181,  858, 1181,  839, 1181, 1181, 1181,  841,
 /*   730 */  1181, 1181,  843, 1181,  847, 1181, 1181, 1181, 1181, 1181,
 /*   740 */   851,  853, 1181,  854,  852, 1181,  845, 1181, 1181,  842,
 /*   750 */  1181, 1181,  844, 1181,  848, 1181,  846, 1181, 1181, 1181,
 /*   760 */  1181, 1181,  840, 1181, 1181,  822,  824,  823, 1181, 1181,
 /*   770 */   812, 1181, 1181, 1181,  814, 1181, 1181,  815, 1181, 1181,
 /*   780 */   817, 1181, 1181,  816, 1181, 1181, 1181,  818, 1181, 1181,
 /*   790 */   819,  795,
};
#define YY_SZ_ACTTAB (sizeof(yy_action)/sizeof(yy_action[0]))

/* The next table maps tokens into fallback tokens.  If a construct
** like the following:
** 
**      %fallback ID X Y Z.
**
** appears in the grammer, then ID becomes a fallback token for X, Y,
** and Z.  Whenever one of the tokens X, Y, or Z is input to the parser
** but it does not parse, the type of the token is changed to ID and
** the parse is retried before an error is thrown.
*/
#ifdef YYFALLBACK
static const YYCODETYPE yyFallback[] = {
};
#endif /* YYFALLBACK */

/* The following structure represents a single element of the
** parser's stack.  Information stored includes:
**
**   +  The state number for the parser at this level of the stack.
**
**   +  The value of the token stored at this level of the stack.
**      (In other words, the "major" token.)
**
**   +  The semantic value stored at this level of the stack.  This is
**      the information used by the action routines in the grammar.
**      It is sometimes called the "minor" token.
*/
struct yyStackEntry {
  int stateno;       /* The state-number */
  int major;         /* The major token value.  This is the code
                     ** number for the token at this stack level */
  YYMINORTYPE minor; /* The user-supplied minor token value.  This
                     ** is the value of the token  */
};
typedef struct yyStackEntry yyStackEntry;

/* The state of the parser is completely contained in an instance of
** the following structure */
struct yyParser {
  int yyidx;                    /* Index of top element in stack */
  int yyerrcnt;                 /* Shifts left before out of the error */
  zephir_ARG_SDECL                /* A place to hold %extra_argument */
  yyStackEntry yystack[YYSTACKDEPTH];  /* The parser's stack */
};
typedef struct yyParser yyParser;

#ifndef NDEBUG
#include <stdio.h>
static FILE *yyTraceFILE = 0;
static char *yyTracePrompt = 0;
#endif /* NDEBUG */

#ifndef NDEBUG
/* 
** Turn parser tracing on by giving a stream to which to write the trace
** and a prompt to preface each trace message.  Tracing is turned off
** by making either argument NULL 
**
** Inputs:
** <ul>
** <li> A FILE* to which trace output should be written.
**      If NULL, then tracing is turned off.
** <li> A prefix string written at the beginning of every
**      line of trace output.  If NULL, then tracing is
**      turned off.
** </ul>
**
** Outputs:
** None.
*/
void zephir_Trace(FILE *TraceFILE, char *zTracePrompt){
  yyTraceFILE = TraceFILE;
  yyTracePrompt = zTracePrompt;
  if( yyTraceFILE==0 ) yyTracePrompt = 0;
  else if( yyTracePrompt==0 ) yyTraceFILE = 0;
}
#endif /* NDEBUG */

#ifndef NDEBUG
/* For tracing shifts, the names of all terminals and nonterminals
** are required.  The following table supplies these names */
static const char *yyTokenName[] = { 
  "$",             "PUBLIC",        "PROTECTED",     "STATIC",      
  "PRIVATE",       "SCOPED",        "COMMA",         "REQUIRE",     
  "QUESTION",      "LIKELY",        "UNLIKELY",      "INSTANCEOF",  
  "OR",            "AND",           "BITWISE_OR",    "BITWISE_AND", 
  "BITWISE_XOR",   "BITWISE_SHIFTLEFT",  "BITWISE_SHIFTRIGHT",  "EQUALS",      
  "IDENTICAL",     "LESS",          "GREATER",       "LESSEQUAL",   
  "GREATEREQUAL",  "NOTIDENTICAL",  "NOTEQUALS",     "ADD",         
  "SUB",           "CONCAT",        "MUL",           "DIV",         
  "MOD",           "ISSET",         "FETCH",         "EMPTY",       
  "TYPEOF",        "CLONE",         "NEW",           "NOT",         
  "PARENTHESES_CLOSE",  "SBRACKET_OPEN",  "ARROW",         "NAMESPACE",   
  "IDENTIFIER",    "DOTCOMMA",      "USE",           "AS",          
  "INTERFACE",     "EXTENDS",       "CLASS",         "IMPLEMENTS",  
  "ABSTRACT",      "FINAL",         "BRACKET_OPEN",  "BRACKET_CLOSE",
  "COMMENT",       "ASSIGN",        "CONST",         "CONSTANT",    
  "FUNCTION",      "PARENTHESES_OPEN",  "INLINE",        "VOID",        
  "NULL",          "THIS",          "SBRACKET_CLOSE",  "TYPE_INTEGER",
  "TYPE_UINTEGER",  "TYPE_LONG",     "TYPE_ULONG",    "TYPE_CHAR",   
  "TYPE_UCHAR",    "TYPE_DOUBLE",   "TYPE_BOOL",     "TYPE_STRING", 
  "TYPE_ARRAY",    "TYPE_VAR",      "TYPE_CALLABLE",  "TYPE_RESOURCE",
  "TYPE_OBJECT",   "BREAK",         "CONTINUE",      "IF",          
  "ELSE",          "SWITCH",        "CASE",          "COLON",       
  "DEFAULT",       "LOOP",          "WHILE",         "DO",          
  "TRY",           "CATCH",         "FOR",           "IN",          
  "REVERSE",       "LET",           "ADDASSIGN",     "SUBASSIGN",   
  "MULASSIGN",     "DIVASSIGN",     "CONCATASSIGN",  "MODASSIGN",   
  "STRING",        "DOUBLECOLON",   "INCR",          "DECR",        
  "ECHO",          "RETURN",        "UNSET",         "THROW",       
  "INTEGER",       "CHAR",          "DOUBLE",        "TRUE",        
  "FALSE",         "CBLOCK",        "error",         "program",     
  "zephir_language",  "zephir_top_statement_list",  "zephir_top_statement",  "zephir_namespace_def",
  "zephir_use_aliases",  "zephir_class_def",  "zephir_interface_def",  "zephir_comment",
  "zephir_cblock",  "zephir_use_aliases_list",  "zephir_interface_body",  "zephir_class_body",
  "zephir_implements_list",  "zephir_class_definition",  "zephir_implements",  "zephir_interface_definition",
  "zephir_class_properties_definition",  "zephir_class_consts_definition",  "zephir_class_methods_definition",  "zephir_interface_methods_definition",
  "zephir_class_property_definition",  "zephir_visibility_list",  "zephir_literal_expr",  "zephir_class_property_shortcuts",
  "zephir_class_property_shortcuts_list",  "zephir_class_property_shortcut",  "zephir_class_const_definition",  "zephir_class_method_definition",
  "zephir_interface_method_definition",  "zephir_parameter_list",  "zephir_statement_list",  "zephir_method_return_type",
  "zephir_visibility",  "zephir_method_return_type_list",  "zephir_method_return_type_item",  "zephir_parameter_type",
  "zephir_parameter_cast",  "zephir_parameter_cast_collection",  "zephir_parameter",  "zephir_statement",
  "zephir_let_statement",  "zephir_if_statement",  "zephir_loop_statement",  "zephir_echo_statement",
  "zephir_return_statement",  "zephir_require_statement",  "zephir_fetch_statement",  "zephir_fcall_statement",
  "zephir_mcall_statement",  "zephir_scall_statement",  "zephir_unset_statement",  "zephir_throw_statement",
  "zephir_declare_statement",  "zephir_break_statement",  "zephir_continue_statement",  "zephir_while_statement",
  "zephir_do_while_statement",  "zephir_try_catch_statement",  "zephir_switch_statement",  "zephir_for_statement",
  "zephir_empty_statement",  "zephir_eval_expr",  "zephir_case_clauses",  "zephir_case_clause",
  "zephir_catch_statement_list",  "zephir_catch_statement",  "zephir_catch_classes_list",  "zephir_catch_class",
  "zephir_common_expr",  "zephir_let_assignments",  "zephir_let_assignment",  "zephir_assignment_operator",
  "zephir_assign_expr",  "zephir_array_offset_list",  "zephir_array_offset",  "zephir_index_expr",
  "zephir_echo_expressions",  "zephir_echo_expression",  "zephir_mcall_expr",  "zephir_fcall_expr",
  "zephir_scall_expr",  "zephir_fetch_expr",  "zephir_declare_variable_list",  "zephir_declare_variable",
  "zephir_array_list",  "zephir_call_parameters",  "zephir_call_parameter",  "zephir_array_item",
  "zephir_array_key",  "zephir_array_value",  "zephir_literal_array_list",  "zephir_literal_array_item",
  "zephir_literal_array_key",  "zephir_literal_array_value",
};
#endif /* NDEBUG */

#ifndef NDEBUG
/* For tracing reduce actions, the names of all rules are required.
*/
static const char *yyRuleName[] = {
 /*   0 */ "program ::= zephir_language",
 /*   1 */ "zephir_language ::= zephir_top_statement_list",
 /*   2 */ "zephir_top_statement_list ::= zephir_top_statement_list zephir_top_statement",
 /*   3 */ "zephir_top_statement_list ::= zephir_top_statement",
 /*   4 */ "zephir_top_statement ::= zephir_namespace_def",
 /*   5 */ "zephir_top_statement ::= zephir_use_aliases",
 /*   6 */ "zephir_top_statement ::= zephir_class_def",
 /*   7 */ "zephir_top_statement ::= zephir_interface_def",
 /*   8 */ "zephir_top_statement ::= zephir_comment",
 /*   9 */ "zephir_top_statement ::= zephir_cblock",
 /*  10 */ "zephir_namespace_def ::= NAMESPACE IDENTIFIER DOTCOMMA",
 /*  11 */ "zephir_namespace_def ::= USE zephir_use_aliases_list DOTCOMMA",
 /*  12 */ "zephir_use_aliases_list ::= zephir_use_aliases_list COMMA zephir_use_aliases",
 /*  13 */ "zephir_use_aliases_list ::= zephir_use_aliases",
 /*  14 */ "zephir_use_aliases ::= IDENTIFIER",
 /*  15 */ "zephir_use_aliases ::= IDENTIFIER AS IDENTIFIER",
 /*  16 */ "zephir_interface_def ::= INTERFACE IDENTIFIER zephir_interface_body",
 /*  17 */ "zephir_interface_def ::= INTERFACE IDENTIFIER EXTENDS IDENTIFIER zephir_interface_body",
 /*  18 */ "zephir_class_def ::= CLASS IDENTIFIER zephir_class_body",
 /*  19 */ "zephir_class_def ::= CLASS IDENTIFIER EXTENDS IDENTIFIER zephir_class_body",
 /*  20 */ "zephir_class_def ::= CLASS IDENTIFIER IMPLEMENTS zephir_implements_list zephir_class_body",
 /*  21 */ "zephir_class_def ::= CLASS IDENTIFIER EXTENDS IDENTIFIER IMPLEMENTS zephir_implements_list zephir_class_body",
 /*  22 */ "zephir_class_def ::= ABSTRACT CLASS IDENTIFIER zephir_class_body",
 /*  23 */ "zephir_class_def ::= ABSTRACT CLASS IDENTIFIER EXTENDS IDENTIFIER zephir_class_body",
 /*  24 */ "zephir_class_def ::= ABSTRACT CLASS IDENTIFIER IMPLEMENTS zephir_implements_list zephir_class_body",
 /*  25 */ "zephir_class_def ::= ABSTRACT CLASS IDENTIFIER EXTENDS IDENTIFIER IMPLEMENTS zephir_implements_list zephir_class_body",
 /*  26 */ "zephir_class_def ::= FINAL CLASS IDENTIFIER zephir_class_body",
 /*  27 */ "zephir_class_def ::= FINAL CLASS IDENTIFIER EXTENDS IDENTIFIER zephir_class_body",
 /*  28 */ "zephir_class_body ::= BRACKET_OPEN BRACKET_CLOSE",
 /*  29 */ "zephir_class_body ::= BRACKET_OPEN zephir_class_definition BRACKET_CLOSE",
 /*  30 */ "zephir_implements_list ::= zephir_implements_list COMMA zephir_implements",
 /*  31 */ "zephir_implements_list ::= zephir_implements",
 /*  32 */ "zephir_implements ::= IDENTIFIER",
 /*  33 */ "zephir_interface_body ::= BRACKET_OPEN BRACKET_CLOSE",
 /*  34 */ "zephir_interface_body ::= BRACKET_OPEN zephir_interface_definition BRACKET_CLOSE",
 /*  35 */ "zephir_class_definition ::= zephir_class_properties_definition",
 /*  36 */ "zephir_class_definition ::= zephir_class_consts_definition",
 /*  37 */ "zephir_class_definition ::= zephir_class_methods_definition",
 /*  38 */ "zephir_class_definition ::= zephir_class_properties_definition zephir_class_methods_definition",
 /*  39 */ "zephir_class_definition ::= zephir_class_properties_definition zephir_class_consts_definition",
 /*  40 */ "zephir_class_definition ::= zephir_class_consts_definition zephir_class_properties_definition",
 /*  41 */ "zephir_class_definition ::= zephir_class_consts_definition zephir_class_methods_definition",
 /*  42 */ "zephir_class_definition ::= zephir_class_properties_definition zephir_class_consts_definition zephir_class_methods_definition",
 /*  43 */ "zephir_class_definition ::= zephir_class_consts_definition zephir_class_properties_definition zephir_class_methods_definition",
 /*  44 */ "zephir_interface_definition ::= zephir_class_consts_definition",
 /*  45 */ "zephir_interface_definition ::= zephir_interface_methods_definition",
 /*  46 */ "zephir_interface_definition ::= zephir_class_consts_definition zephir_interface_methods_definition",
 /*  47 */ "zephir_class_properties_definition ::= zephir_class_properties_definition zephir_class_property_definition",
 /*  48 */ "zephir_class_properties_definition ::= zephir_class_property_definition",
 /*  49 */ "zephir_class_property_definition ::= COMMENT zephir_visibility_list IDENTIFIER DOTCOMMA",
 /*  50 */ "zephir_class_property_definition ::= zephir_visibility_list IDENTIFIER DOTCOMMA",
 /*  51 */ "zephir_class_property_definition ::= COMMENT zephir_visibility_list IDENTIFIER ASSIGN zephir_literal_expr DOTCOMMA",
 /*  52 */ "zephir_class_property_definition ::= zephir_visibility_list IDENTIFIER ASSIGN zephir_literal_expr DOTCOMMA",
 /*  53 */ "zephir_class_property_definition ::= COMMENT zephir_visibility_list IDENTIFIER zephir_class_property_shortcuts DOTCOMMA",
 /*  54 */ "zephir_class_property_definition ::= zephir_visibility_list IDENTIFIER zephir_class_property_shortcuts DOTCOMMA",
 /*  55 */ "zephir_class_property_definition ::= COMMENT zephir_visibility_list IDENTIFIER ASSIGN zephir_literal_expr zephir_class_property_shortcuts DOTCOMMA",
 /*  56 */ "zephir_class_property_definition ::= zephir_visibility_list IDENTIFIER ASSIGN zephir_literal_expr zephir_class_property_shortcuts DOTCOMMA",
 /*  57 */ "zephir_class_property_shortcuts ::= BRACKET_OPEN BRACKET_CLOSE",
 /*  58 */ "zephir_class_property_shortcuts ::= BRACKET_OPEN zephir_class_property_shortcuts_list BRACKET_CLOSE",
 /*  59 */ "zephir_class_property_shortcuts_list ::= zephir_class_property_shortcuts_list COMMA zephir_class_property_shortcut",
 /*  60 */ "zephir_class_property_shortcuts_list ::= zephir_class_property_shortcut",
 /*  61 */ "zephir_class_property_shortcut ::= IDENTIFIER",
 /*  62 */ "zephir_class_property_shortcut ::= COMMENT IDENTIFIER",
 /*  63 */ "zephir_class_consts_definition ::= zephir_class_consts_definition zephir_class_const_definition",
 /*  64 */ "zephir_class_consts_definition ::= zephir_class_const_definition",
 /*  65 */ "zephir_class_methods_definition ::= zephir_class_methods_definition zephir_class_method_definition",
 /*  66 */ "zephir_class_methods_definition ::= zephir_class_method_definition",
 /*  67 */ "zephir_interface_methods_definition ::= zephir_interface_methods_definition zephir_interface_method_definition",
 /*  68 */ "zephir_interface_methods_definition ::= zephir_interface_method_definition",
 /*  69 */ "zephir_class_const_definition ::= COMMENT CONST CONSTANT ASSIGN zephir_literal_expr DOTCOMMA",
 /*  70 */ "zephir_class_const_definition ::= CONST CONSTANT ASSIGN zephir_literal_expr DOTCOMMA",
 /*  71 */ "zephir_class_method_definition ::= zephir_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE BRACKET_OPEN BRACKET_CLOSE",
 /*  72 */ "zephir_class_method_definition ::= zephir_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN zephir_parameter_list PARENTHESES_CLOSE BRACKET_OPEN BRACKET_CLOSE",
 /*  73 */ "zephir_class_method_definition ::= zephir_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE BRACKET_OPEN zephir_statement_list BRACKET_CLOSE",
 /*  74 */ "zephir_class_method_definition ::= zephir_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN zephir_parameter_list PARENTHESES_CLOSE BRACKET_OPEN zephir_statement_list BRACKET_CLOSE",
 /*  75 */ "zephir_class_method_definition ::= COMMENT zephir_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE BRACKET_OPEN BRACKET_CLOSE",
 /*  76 */ "zephir_class_method_definition ::= COMMENT zephir_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN zephir_parameter_list PARENTHESES_CLOSE BRACKET_OPEN BRACKET_CLOSE",
 /*  77 */ "zephir_class_method_definition ::= COMMENT zephir_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE BRACKET_OPEN zephir_statement_list BRACKET_CLOSE",
 /*  78 */ "zephir_class_method_definition ::= COMMENT zephir_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN zephir_parameter_list PARENTHESES_CLOSE BRACKET_OPEN zephir_statement_list BRACKET_CLOSE",
 /*  79 */ "zephir_class_method_definition ::= zephir_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE ARROW zephir_method_return_type BRACKET_OPEN BRACKET_CLOSE",
 /*  80 */ "zephir_class_method_definition ::= zephir_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN zephir_parameter_list PARENTHESES_CLOSE ARROW zephir_method_return_type BRACKET_OPEN BRACKET_CLOSE",
 /*  81 */ "zephir_class_method_definition ::= zephir_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE ARROW zephir_method_return_type BRACKET_OPEN zephir_statement_list BRACKET_CLOSE",
 /*  82 */ "zephir_class_method_definition ::= zephir_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN zephir_parameter_list PARENTHESES_CLOSE ARROW zephir_method_return_type BRACKET_OPEN zephir_statement_list BRACKET_CLOSE",
 /*  83 */ "zephir_class_method_definition ::= COMMENT zephir_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE ARROW zephir_method_return_type BRACKET_OPEN BRACKET_CLOSE",
 /*  84 */ "zephir_class_method_definition ::= COMMENT zephir_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN zephir_parameter_list PARENTHESES_CLOSE ARROW zephir_method_return_type BRACKET_OPEN BRACKET_CLOSE",
 /*  85 */ "zephir_class_method_definition ::= COMMENT zephir_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE ARROW zephir_method_return_type BRACKET_OPEN zephir_statement_list BRACKET_CLOSE",
 /*  86 */ "zephir_class_method_definition ::= COMMENT zephir_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN zephir_parameter_list PARENTHESES_CLOSE ARROW zephir_method_return_type BRACKET_OPEN zephir_statement_list BRACKET_CLOSE",
 /*  87 */ "zephir_interface_method_definition ::= zephir_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE ARROW zephir_method_return_type DOTCOMMA",
 /*  88 */ "zephir_interface_method_definition ::= zephir_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN zephir_parameter_list PARENTHESES_CLOSE ARROW zephir_method_return_type DOTCOMMA",
 /*  89 */ "zephir_interface_method_definition ::= COMMENT zephir_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE ARROW zephir_method_return_type DOTCOMMA",
 /*  90 */ "zephir_interface_method_definition ::= COMMENT zephir_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN zephir_parameter_list PARENTHESES_CLOSE ARROW zephir_method_return_type DOTCOMMA",
 /*  91 */ "zephir_interface_method_definition ::= zephir_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE DOTCOMMA",
 /*  92 */ "zephir_interface_method_definition ::= zephir_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN zephir_parameter_list PARENTHESES_CLOSE DOTCOMMA",
 /*  93 */ "zephir_interface_method_definition ::= COMMENT zephir_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE DOTCOMMA",
 /*  94 */ "zephir_interface_method_definition ::= COMMENT zephir_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN zephir_parameter_list PARENTHESES_CLOSE DOTCOMMA",
 /*  95 */ "zephir_visibility_list ::= zephir_visibility_list zephir_visibility",
 /*  96 */ "zephir_visibility_list ::= zephir_visibility",
 /*  97 */ "zephir_visibility ::= PUBLIC",
 /*  98 */ "zephir_visibility ::= PROTECTED",
 /*  99 */ "zephir_visibility ::= PRIVATE",
 /* 100 */ "zephir_visibility ::= STATIC",
 /* 101 */ "zephir_visibility ::= SCOPED",
 /* 102 */ "zephir_visibility ::= INLINE",
 /* 103 */ "zephir_visibility ::= ABSTRACT",
 /* 104 */ "zephir_visibility ::= FINAL",
 /* 105 */ "zephir_method_return_type ::= VOID",
 /* 106 */ "zephir_method_return_type ::= zephir_method_return_type_list",
 /* 107 */ "zephir_method_return_type_list ::= zephir_method_return_type_list BITWISE_OR zephir_method_return_type_item",
 /* 108 */ "zephir_method_return_type_list ::= zephir_method_return_type_item",
 /* 109 */ "zephir_method_return_type_item ::= zephir_parameter_type",
 /* 110 */ "zephir_method_return_type_item ::= NULL",
 /* 111 */ "zephir_method_return_type_item ::= THIS",
 /* 112 */ "zephir_method_return_type_item ::= zephir_parameter_type NOT",
 /* 113 */ "zephir_method_return_type_item ::= zephir_parameter_cast",
 /* 114 */ "zephir_method_return_type_item ::= zephir_parameter_cast_collection",
 /* 115 */ "zephir_parameter_list ::= zephir_parameter_list COMMA zephir_parameter",
 /* 116 */ "zephir_parameter_list ::= zephir_parameter",
 /* 117 */ "zephir_parameter ::= IDENTIFIER",
 /* 118 */ "zephir_parameter ::= CONST IDENTIFIER",
 /* 119 */ "zephir_parameter ::= zephir_parameter_type IDENTIFIER",
 /* 120 */ "zephir_parameter ::= CONST zephir_parameter_type IDENTIFIER",
 /* 121 */ "zephir_parameter ::= zephir_parameter_type NOT IDENTIFIER",
 /* 122 */ "zephir_parameter ::= CONST zephir_parameter_type NOT IDENTIFIER",
 /* 123 */ "zephir_parameter ::= zephir_parameter_cast IDENTIFIER",
 /* 124 */ "zephir_parameter ::= CONST zephir_parameter_cast IDENTIFIER",
 /* 125 */ "zephir_parameter ::= IDENTIFIER ASSIGN zephir_literal_expr",
 /* 126 */ "zephir_parameter ::= CONST IDENTIFIER ASSIGN zephir_literal_expr",
 /* 127 */ "zephir_parameter ::= zephir_parameter_type IDENTIFIER ASSIGN zephir_literal_expr",
 /* 128 */ "zephir_parameter ::= CONST zephir_parameter_type IDENTIFIER ASSIGN zephir_literal_expr",
 /* 129 */ "zephir_parameter ::= zephir_parameter_type NOT IDENTIFIER ASSIGN zephir_literal_expr",
 /* 130 */ "zephir_parameter ::= CONST zephir_parameter_type NOT IDENTIFIER ASSIGN zephir_literal_expr",
 /* 131 */ "zephir_parameter ::= zephir_parameter_cast IDENTIFIER ASSIGN zephir_literal_expr",
 /* 132 */ "zephir_parameter ::= CONST zephir_parameter_cast IDENTIFIER ASSIGN zephir_literal_expr",
 /* 133 */ "zephir_parameter_cast ::= LESS IDENTIFIER GREATER",
 /* 134 */ "zephir_parameter_cast_collection ::= LESS IDENTIFIER SBRACKET_OPEN SBRACKET_CLOSE GREATER",
 /* 135 */ "zephir_parameter_type ::= TYPE_INTEGER",
 /* 136 */ "zephir_parameter_type ::= TYPE_UINTEGER",
 /* 137 */ "zephir_parameter_type ::= TYPE_LONG",
 /* 138 */ "zephir_parameter_type ::= TYPE_ULONG",
 /* 139 */ "zephir_parameter_type ::= TYPE_CHAR",
 /* 140 */ "zephir_parameter_type ::= TYPE_UCHAR",
 /* 141 */ "zephir_parameter_type ::= TYPE_DOUBLE",
 /* 142 */ "zephir_parameter_type ::= TYPE_BOOL",
 /* 143 */ "zephir_parameter_type ::= TYPE_STRING",
 /* 144 */ "zephir_parameter_type ::= TYPE_ARRAY",
 /* 145 */ "zephir_parameter_type ::= TYPE_VAR",
 /* 146 */ "zephir_parameter_type ::= TYPE_CALLABLE",
 /* 147 */ "zephir_parameter_type ::= TYPE_RESOURCE",
 /* 148 */ "zephir_parameter_type ::= TYPE_OBJECT",
 /* 149 */ "zephir_statement_list ::= zephir_statement_list zephir_statement",
 /* 150 */ "zephir_statement_list ::= zephir_statement",
 /* 151 */ "zephir_statement ::= zephir_cblock",
 /* 152 */ "zephir_statement ::= zephir_let_statement",
 /* 153 */ "zephir_statement ::= zephir_if_statement",
 /* 154 */ "zephir_statement ::= zephir_loop_statement",
 /* 155 */ "zephir_statement ::= zephir_echo_statement",
 /* 156 */ "zephir_statement ::= zephir_return_statement",
 /* 157 */ "zephir_statement ::= zephir_require_statement",
 /* 158 */ "zephir_statement ::= zephir_fetch_statement",
 /* 159 */ "zephir_statement ::= zephir_fcall_statement",
 /* 160 */ "zephir_statement ::= zephir_mcall_statement",
 /* 161 */ "zephir_statement ::= zephir_scall_statement",
 /* 162 */ "zephir_statement ::= zephir_unset_statement",
 /* 163 */ "zephir_statement ::= zephir_throw_statement",
 /* 164 */ "zephir_statement ::= zephir_declare_statement",
 /* 165 */ "zephir_statement ::= zephir_break_statement",
 /* 166 */ "zephir_statement ::= zephir_continue_statement",
 /* 167 */ "zephir_statement ::= zephir_while_statement",
 /* 168 */ "zephir_statement ::= zephir_do_while_statement",
 /* 169 */ "zephir_statement ::= zephir_try_catch_statement",
 /* 170 */ "zephir_statement ::= zephir_switch_statement",
 /* 171 */ "zephir_statement ::= zephir_for_statement",
 /* 172 */ "zephir_statement ::= zephir_comment",
 /* 173 */ "zephir_statement ::= zephir_empty_statement",
 /* 174 */ "zephir_empty_statement ::= DOTCOMMA",
 /* 175 */ "zephir_break_statement ::= BREAK DOTCOMMA",
 /* 176 */ "zephir_continue_statement ::= CONTINUE DOTCOMMA",
 /* 177 */ "zephir_if_statement ::= IF zephir_eval_expr BRACKET_OPEN BRACKET_CLOSE",
 /* 178 */ "zephir_if_statement ::= IF zephir_eval_expr BRACKET_OPEN BRACKET_CLOSE ELSE BRACKET_OPEN BRACKET_CLOSE",
 /* 179 */ "zephir_if_statement ::= IF zephir_eval_expr BRACKET_OPEN zephir_statement_list BRACKET_CLOSE",
 /* 180 */ "zephir_if_statement ::= IF zephir_eval_expr BRACKET_OPEN zephir_statement_list BRACKET_CLOSE ELSE BRACKET_OPEN zephir_statement_list BRACKET_CLOSE",
 /* 181 */ "zephir_if_statement ::= IF zephir_eval_expr BRACKET_OPEN zephir_statement_list BRACKET_CLOSE ELSE BRACKET_OPEN BRACKET_CLOSE",
 /* 182 */ "zephir_if_statement ::= IF zephir_eval_expr BRACKET_OPEN BRACKET_CLOSE ELSE BRACKET_OPEN zephir_statement_list BRACKET_CLOSE",
 /* 183 */ "zephir_switch_statement ::= SWITCH zephir_eval_expr BRACKET_OPEN BRACKET_CLOSE",
 /* 184 */ "zephir_switch_statement ::= SWITCH zephir_eval_expr BRACKET_OPEN zephir_case_clauses BRACKET_CLOSE",
 /* 185 */ "zephir_case_clauses ::= zephir_case_clauses zephir_case_clause",
 /* 186 */ "zephir_case_clauses ::= zephir_case_clause",
 /* 187 */ "zephir_case_clause ::= CASE zephir_literal_expr COLON",
 /* 188 */ "zephir_case_clause ::= CASE zephir_literal_expr COLON zephir_statement_list",
 /* 189 */ "zephir_case_clause ::= DEFAULT COLON zephir_statement_list",
 /* 190 */ "zephir_loop_statement ::= LOOP BRACKET_OPEN BRACKET_CLOSE",
 /* 191 */ "zephir_loop_statement ::= LOOP BRACKET_OPEN zephir_statement_list BRACKET_CLOSE",
 /* 192 */ "zephir_while_statement ::= WHILE zephir_eval_expr BRACKET_OPEN BRACKET_CLOSE",
 /* 193 */ "zephir_while_statement ::= WHILE zephir_eval_expr BRACKET_OPEN zephir_statement_list BRACKET_CLOSE",
 /* 194 */ "zephir_do_while_statement ::= DO BRACKET_OPEN BRACKET_CLOSE WHILE zephir_eval_expr DOTCOMMA",
 /* 195 */ "zephir_do_while_statement ::= DO BRACKET_OPEN zephir_statement_list BRACKET_CLOSE WHILE zephir_eval_expr DOTCOMMA",
 /* 196 */ "zephir_try_catch_statement ::= TRY BRACKET_OPEN BRACKET_CLOSE",
 /* 197 */ "zephir_try_catch_statement ::= TRY BRACKET_OPEN zephir_statement_list BRACKET_CLOSE",
 /* 198 */ "zephir_try_catch_statement ::= TRY BRACKET_OPEN zephir_statement_list BRACKET_CLOSE zephir_catch_statement_list",
 /* 199 */ "zephir_catch_statement_list ::= zephir_catch_statement_list zephir_catch_statement",
 /* 200 */ "zephir_catch_statement_list ::= zephir_catch_statement",
 /* 201 */ "zephir_catch_statement ::= CATCH zephir_catch_classes_list BRACKET_OPEN zephir_statement_list BRACKET_CLOSE",
 /* 202 */ "zephir_catch_statement ::= CATCH zephir_catch_classes_list BRACKET_OPEN BRACKET_CLOSE",
 /* 203 */ "zephir_catch_statement ::= CATCH zephir_catch_classes_list COMMA IDENTIFIER BRACKET_OPEN BRACKET_CLOSE",
 /* 204 */ "zephir_catch_statement ::= CATCH zephir_catch_classes_list COMMA IDENTIFIER BRACKET_OPEN zephir_statement_list BRACKET_CLOSE",
 /* 205 */ "zephir_catch_classes_list ::= zephir_catch_classes_list BITWISE_OR zephir_catch_class",
 /* 206 */ "zephir_catch_classes_list ::= zephir_catch_class",
 /* 207 */ "zephir_catch_class ::= IDENTIFIER",
 /* 208 */ "zephir_for_statement ::= FOR IDENTIFIER IN zephir_common_expr BRACKET_OPEN zephir_statement_list BRACKET_CLOSE",
 /* 209 */ "zephir_for_statement ::= FOR IDENTIFIER IN zephir_common_expr BRACKET_OPEN BRACKET_CLOSE",
 /* 210 */ "zephir_for_statement ::= FOR IDENTIFIER IN REVERSE zephir_common_expr BRACKET_OPEN zephir_statement_list BRACKET_CLOSE",
 /* 211 */ "zephir_for_statement ::= FOR IDENTIFIER COMMA IDENTIFIER IN zephir_common_expr BRACKET_OPEN zephir_statement_list BRACKET_CLOSE",
 /* 212 */ "zephir_for_statement ::= FOR IDENTIFIER COMMA IDENTIFIER IN zephir_common_expr BRACKET_OPEN BRACKET_CLOSE",
 /* 213 */ "zephir_for_statement ::= FOR IDENTIFIER COMMA IDENTIFIER IN REVERSE zephir_common_expr BRACKET_OPEN zephir_statement_list BRACKET_CLOSE",
 /* 214 */ "zephir_let_statement ::= LET zephir_let_assignments DOTCOMMA",
 /* 215 */ "zephir_let_assignments ::= zephir_let_assignments COMMA zephir_let_assignment",
 /* 216 */ "zephir_let_assignments ::= zephir_let_assignment",
 /* 217 */ "zephir_assignment_operator ::= ASSIGN",
 /* 218 */ "zephir_assignment_operator ::= ADDASSIGN",
 /* 219 */ "zephir_assignment_operator ::= SUBASSIGN",
 /* 220 */ "zephir_assignment_operator ::= MULASSIGN",
 /* 221 */ "zephir_assignment_operator ::= DIVASSIGN",
 /* 222 */ "zephir_assignment_operator ::= CONCATASSIGN",
 /* 223 */ "zephir_assignment_operator ::= MODASSIGN",
 /* 224 */ "zephir_let_assignment ::= IDENTIFIER zephir_assignment_operator zephir_assign_expr",
 /* 225 */ "zephir_let_assignment ::= IDENTIFIER ARROW IDENTIFIER zephir_assignment_operator zephir_assign_expr",
 /* 226 */ "zephir_let_assignment ::= IDENTIFIER ARROW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE zephir_assignment_operator zephir_assign_expr",
 /* 227 */ "zephir_let_assignment ::= IDENTIFIER ARROW BRACKET_OPEN STRING BRACKET_CLOSE zephir_assignment_operator zephir_assign_expr",
 /* 228 */ "zephir_let_assignment ::= IDENTIFIER ARROW IDENTIFIER SBRACKET_OPEN SBRACKET_CLOSE zephir_assignment_operator zephir_assign_expr",
 /* 229 */ "zephir_let_assignment ::= IDENTIFIER ARROW IDENTIFIER zephir_array_offset_list zephir_assignment_operator zephir_assign_expr",
 /* 230 */ "zephir_let_assignment ::= IDENTIFIER ARROW IDENTIFIER zephir_array_offset_list SBRACKET_OPEN SBRACKET_CLOSE zephir_assignment_operator zephir_assign_expr",
 /* 231 */ "zephir_let_assignment ::= IDENTIFIER DOUBLECOLON IDENTIFIER zephir_assignment_operator zephir_assign_expr",
 /* 232 */ "zephir_let_assignment ::= IDENTIFIER DOUBLECOLON IDENTIFIER SBRACKET_OPEN SBRACKET_CLOSE zephir_assignment_operator zephir_assign_expr",
 /* 233 */ "zephir_let_assignment ::= IDENTIFIER DOUBLECOLON IDENTIFIER zephir_array_offset_list zephir_assignment_operator zephir_assign_expr",
 /* 234 */ "zephir_let_assignment ::= IDENTIFIER DOUBLECOLON IDENTIFIER zephir_array_offset_list SBRACKET_OPEN SBRACKET_CLOSE zephir_assignment_operator zephir_assign_expr",
 /* 235 */ "zephir_let_assignment ::= IDENTIFIER SBRACKET_OPEN SBRACKET_CLOSE zephir_assignment_operator zephir_assign_expr",
 /* 236 */ "zephir_let_assignment ::= IDENTIFIER zephir_array_offset_list zephir_assignment_operator zephir_assign_expr",
 /* 237 */ "zephir_let_assignment ::= IDENTIFIER zephir_array_offset_list SBRACKET_OPEN SBRACKET_CLOSE zephir_assignment_operator zephir_assign_expr",
 /* 238 */ "zephir_array_offset_list ::= zephir_array_offset_list zephir_array_offset",
 /* 239 */ "zephir_array_offset_list ::= zephir_array_offset",
 /* 240 */ "zephir_array_offset ::= SBRACKET_OPEN zephir_index_expr SBRACKET_CLOSE",
 /* 241 */ "zephir_let_assignment ::= IDENTIFIER ARROW IDENTIFIER INCR",
 /* 242 */ "zephir_let_assignment ::= IDENTIFIER ARROW IDENTIFIER DECR",
 /* 243 */ "zephir_let_assignment ::= IDENTIFIER INCR",
 /* 244 */ "zephir_let_assignment ::= IDENTIFIER DECR",
 /* 245 */ "zephir_let_assignment ::= BRACKET_OPEN IDENTIFIER BRACKET_CLOSE zephir_assignment_operator zephir_assign_expr",
 /* 246 */ "zephir_index_expr ::= zephir_common_expr",
 /* 247 */ "zephir_echo_statement ::= ECHO zephir_echo_expressions DOTCOMMA",
 /* 248 */ "zephir_echo_expressions ::= zephir_echo_expressions COMMA zephir_echo_expression",
 /* 249 */ "zephir_echo_expressions ::= zephir_echo_expression",
 /* 250 */ "zephir_echo_expression ::= zephir_common_expr",
 /* 251 */ "zephir_mcall_statement ::= zephir_mcall_expr DOTCOMMA",
 /* 252 */ "zephir_fcall_statement ::= zephir_fcall_expr DOTCOMMA",
 /* 253 */ "zephir_scall_statement ::= zephir_scall_expr DOTCOMMA",
 /* 254 */ "zephir_fetch_statement ::= zephir_fetch_expr DOTCOMMA",
 /* 255 */ "zephir_return_statement ::= RETURN zephir_common_expr DOTCOMMA",
 /* 256 */ "zephir_return_statement ::= RETURN DOTCOMMA",
 /* 257 */ "zephir_require_statement ::= REQUIRE zephir_common_expr DOTCOMMA",
 /* 258 */ "zephir_unset_statement ::= UNSET zephir_common_expr DOTCOMMA",
 /* 259 */ "zephir_throw_statement ::= THROW zephir_common_expr DOTCOMMA",
 /* 260 */ "zephir_declare_statement ::= TYPE_INTEGER zephir_declare_variable_list DOTCOMMA",
 /* 261 */ "zephir_declare_statement ::= TYPE_UINTEGER zephir_declare_variable_list DOTCOMMA",
 /* 262 */ "zephir_declare_statement ::= TYPE_CHAR zephir_declare_variable_list DOTCOMMA",
 /* 263 */ "zephir_declare_statement ::= TYPE_UCHAR zephir_declare_variable_list DOTCOMMA",
 /* 264 */ "zephir_declare_statement ::= TYPE_LONG zephir_declare_variable_list DOTCOMMA",
 /* 265 */ "zephir_declare_statement ::= TYPE_ULONG zephir_declare_variable_list DOTCOMMA",
 /* 266 */ "zephir_declare_statement ::= TYPE_DOUBLE zephir_declare_variable_list DOTCOMMA",
 /* 267 */ "zephir_declare_statement ::= TYPE_STRING zephir_declare_variable_list DOTCOMMA",
 /* 268 */ "zephir_declare_statement ::= TYPE_BOOL zephir_declare_variable_list DOTCOMMA",
 /* 269 */ "zephir_declare_statement ::= TYPE_VAR zephir_declare_variable_list DOTCOMMA",
 /* 270 */ "zephir_declare_statement ::= TYPE_ARRAY zephir_declare_variable_list DOTCOMMA",
 /* 271 */ "zephir_declare_variable_list ::= zephir_declare_variable_list COMMA zephir_declare_variable",
 /* 272 */ "zephir_declare_variable_list ::= zephir_declare_variable",
 /* 273 */ "zephir_declare_variable ::= IDENTIFIER",
 /* 274 */ "zephir_declare_variable ::= IDENTIFIER ASSIGN zephir_literal_expr",
 /* 275 */ "zephir_assign_expr ::= zephir_common_expr",
 /* 276 */ "zephir_common_expr ::= NOT zephir_common_expr",
 /* 277 */ "zephir_common_expr ::= SUB zephir_common_expr",
 /* 278 */ "zephir_common_expr ::= ISSET zephir_common_expr",
 /* 279 */ "zephir_common_expr ::= REQUIRE zephir_common_expr",
 /* 280 */ "zephir_common_expr ::= CLONE zephir_common_expr",
 /* 281 */ "zephir_common_expr ::= EMPTY zephir_common_expr",
 /* 282 */ "zephir_common_expr ::= LIKELY zephir_common_expr",
 /* 283 */ "zephir_common_expr ::= UNLIKELY zephir_common_expr",
 /* 284 */ "zephir_common_expr ::= zephir_common_expr EQUALS zephir_common_expr",
 /* 285 */ "zephir_common_expr ::= zephir_common_expr NOTEQUALS zephir_common_expr",
 /* 286 */ "zephir_common_expr ::= zephir_common_expr IDENTICAL zephir_common_expr",
 /* 287 */ "zephir_common_expr ::= zephir_common_expr NOTIDENTICAL zephir_common_expr",
 /* 288 */ "zephir_common_expr ::= zephir_common_expr LESS zephir_common_expr",
 /* 289 */ "zephir_common_expr ::= zephir_common_expr GREATER zephir_common_expr",
 /* 290 */ "zephir_common_expr ::= zephir_common_expr LESSEQUAL zephir_common_expr",
 /* 291 */ "zephir_common_expr ::= zephir_common_expr GREATEREQUAL zephir_common_expr",
 /* 292 */ "zephir_common_expr ::= PARENTHESES_OPEN zephir_common_expr PARENTHESES_CLOSE",
 /* 293 */ "zephir_common_expr ::= PARENTHESES_OPEN zephir_parameter_type PARENTHESES_CLOSE zephir_common_expr",
 /* 294 */ "zephir_common_expr ::= LESS IDENTIFIER GREATER zephir_common_expr",
 /* 295 */ "zephir_common_expr ::= zephir_common_expr ARROW IDENTIFIER",
 /* 296 */ "zephir_common_expr ::= zephir_common_expr ARROW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE",
 /* 297 */ "zephir_common_expr ::= zephir_common_expr ARROW BRACKET_OPEN STRING BRACKET_CLOSE",
 /* 298 */ "zephir_common_expr ::= IDENTIFIER DOUBLECOLON IDENTIFIER",
 /* 299 */ "zephir_common_expr ::= IDENTIFIER DOUBLECOLON CONSTANT",
 /* 300 */ "zephir_common_expr ::= zephir_common_expr SBRACKET_OPEN zephir_common_expr SBRACKET_CLOSE",
 /* 301 */ "zephir_common_expr ::= zephir_common_expr ADD zephir_common_expr",
 /* 302 */ "zephir_common_expr ::= zephir_common_expr SUB zephir_common_expr",
 /* 303 */ "zephir_common_expr ::= zephir_common_expr MUL zephir_common_expr",
 /* 304 */ "zephir_common_expr ::= zephir_common_expr DIV zephir_common_expr",
 /* 305 */ "zephir_common_expr ::= zephir_common_expr MOD zephir_common_expr",
 /* 306 */ "zephir_common_expr ::= zephir_common_expr CONCAT zephir_common_expr",
 /* 307 */ "zephir_common_expr ::= zephir_common_expr AND zephir_common_expr",
 /* 308 */ "zephir_common_expr ::= zephir_common_expr OR zephir_common_expr",
 /* 309 */ "zephir_common_expr ::= zephir_common_expr BITWISE_AND zephir_common_expr",
 /* 310 */ "zephir_common_expr ::= zephir_common_expr BITWISE_OR zephir_common_expr",
 /* 311 */ "zephir_common_expr ::= zephir_common_expr BITWISE_XOR zephir_common_expr",
 /* 312 */ "zephir_common_expr ::= zephir_common_expr BITWISE_SHIFTLEFT zephir_common_expr",
 /* 313 */ "zephir_common_expr ::= zephir_common_expr BITWISE_SHIFTRIGHT zephir_common_expr",
 /* 314 */ "zephir_common_expr ::= zephir_common_expr INSTANCEOF zephir_common_expr",
 /* 315 */ "zephir_fetch_expr ::= FETCH IDENTIFIER COMMA zephir_common_expr",
 /* 316 */ "zephir_common_expr ::= zephir_fetch_expr",
 /* 317 */ "zephir_common_expr ::= TYPEOF zephir_common_expr",
 /* 318 */ "zephir_common_expr ::= IDENTIFIER",
 /* 319 */ "zephir_common_expr ::= INTEGER",
 /* 320 */ "zephir_common_expr ::= STRING",
 /* 321 */ "zephir_common_expr ::= CHAR",
 /* 322 */ "zephir_common_expr ::= DOUBLE",
 /* 323 */ "zephir_common_expr ::= NULL",
 /* 324 */ "zephir_common_expr ::= TRUE",
 /* 325 */ "zephir_common_expr ::= FALSE",
 /* 326 */ "zephir_common_expr ::= CONSTANT",
 /* 327 */ "zephir_common_expr ::= SBRACKET_OPEN SBRACKET_CLOSE",
 /* 328 */ "zephir_common_expr ::= SBRACKET_OPEN zephir_array_list SBRACKET_CLOSE",
 /* 329 */ "zephir_common_expr ::= NEW IDENTIFIER",
 /* 330 */ "zephir_common_expr ::= NEW IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 331 */ "zephir_common_expr ::= NEW IDENTIFIER PARENTHESES_OPEN zephir_call_parameters PARENTHESES_CLOSE",
 /* 332 */ "zephir_common_expr ::= NEW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE",
 /* 333 */ "zephir_common_expr ::= NEW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 334 */ "zephir_common_expr ::= NEW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN zephir_call_parameters PARENTHESES_CLOSE",
 /* 335 */ "zephir_fcall_expr ::= IDENTIFIER PARENTHESES_OPEN zephir_call_parameters PARENTHESES_CLOSE",
 /* 336 */ "zephir_fcall_expr ::= IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 337 */ "zephir_fcall_expr ::= BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN zephir_call_parameters PARENTHESES_CLOSE",
 /* 338 */ "zephir_fcall_expr ::= BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 339 */ "zephir_scall_expr ::= IDENTIFIER DOUBLECOLON IDENTIFIER PARENTHESES_OPEN zephir_call_parameters PARENTHESES_CLOSE",
 /* 340 */ "zephir_scall_expr ::= IDENTIFIER DOUBLECOLON IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 341 */ "zephir_scall_expr ::= BRACKET_OPEN IDENTIFIER BRACKET_CLOSE DOUBLECOLON IDENTIFIER PARENTHESES_OPEN zephir_call_parameters PARENTHESES_CLOSE",
 /* 342 */ "zephir_scall_expr ::= BRACKET_OPEN IDENTIFIER BRACKET_CLOSE DOUBLECOLON IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 343 */ "zephir_scall_expr ::= BRACKET_OPEN IDENTIFIER BRACKET_CLOSE DOUBLECOLON BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN zephir_call_parameters PARENTHESES_CLOSE",
 /* 344 */ "zephir_scall_expr ::= BRACKET_OPEN IDENTIFIER BRACKET_CLOSE DOUBLECOLON BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 345 */ "zephir_mcall_expr ::= zephir_common_expr ARROW IDENTIFIER PARENTHESES_OPEN zephir_call_parameters PARENTHESES_CLOSE",
 /* 346 */ "zephir_mcall_expr ::= zephir_common_expr ARROW IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 347 */ "zephir_mcall_expr ::= zephir_common_expr ARROW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN zephir_call_parameters PARENTHESES_CLOSE",
 /* 348 */ "zephir_mcall_expr ::= zephir_common_expr ARROW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 349 */ "zephir_mcall_expr ::= zephir_common_expr ARROW BRACKET_OPEN STRING BRACKET_CLOSE PARENTHESES_OPEN zephir_call_parameters PARENTHESES_CLOSE",
 /* 350 */ "zephir_mcall_expr ::= zephir_common_expr ARROW BRACKET_OPEN STRING BRACKET_CLOSE PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 351 */ "zephir_common_expr ::= zephir_mcall_expr",
 /* 352 */ "zephir_common_expr ::= zephir_scall_expr",
 /* 353 */ "zephir_common_expr ::= zephir_fcall_expr",
 /* 354 */ "zephir_common_expr ::= zephir_common_expr QUESTION zephir_common_expr COLON zephir_common_expr",
 /* 355 */ "zephir_call_parameters ::= zephir_call_parameters COMMA zephir_call_parameter",
 /* 356 */ "zephir_call_parameters ::= zephir_call_parameter",
 /* 357 */ "zephir_call_parameter ::= zephir_common_expr",
 /* 358 */ "zephir_call_parameter ::= IDENTIFIER COLON zephir_common_expr",
 /* 359 */ "zephir_array_list ::= zephir_array_list COMMA zephir_array_item",
 /* 360 */ "zephir_array_list ::= zephir_array_item",
 /* 361 */ "zephir_array_item ::= zephir_array_key COLON zephir_array_value",
 /* 362 */ "zephir_array_item ::= zephir_array_value",
 /* 363 */ "zephir_array_key ::= IDENTIFIER",
 /* 364 */ "zephir_array_key ::= STRING",
 /* 365 */ "zephir_array_key ::= INTEGER",
 /* 366 */ "zephir_array_value ::= zephir_common_expr",
 /* 367 */ "zephir_literal_expr ::= INTEGER",
 /* 368 */ "zephir_literal_expr ::= CHAR",
 /* 369 */ "zephir_literal_expr ::= STRING",
 /* 370 */ "zephir_literal_expr ::= DOUBLE",
 /* 371 */ "zephir_literal_expr ::= NULL",
 /* 372 */ "zephir_literal_expr ::= FALSE",
 /* 373 */ "zephir_literal_expr ::= TRUE",
 /* 374 */ "zephir_literal_expr ::= IDENTIFIER DOUBLECOLON CONSTANT",
 /* 375 */ "zephir_literal_expr ::= CONSTANT",
 /* 376 */ "zephir_literal_expr ::= SBRACKET_OPEN SBRACKET_CLOSE",
 /* 377 */ "zephir_literal_expr ::= SBRACKET_OPEN zephir_literal_array_list SBRACKET_CLOSE",
 /* 378 */ "zephir_literal_array_list ::= zephir_literal_array_list COMMA zephir_literal_array_item",
 /* 379 */ "zephir_literal_array_list ::= zephir_literal_array_item",
 /* 380 */ "zephir_literal_array_item ::= zephir_literal_array_key COLON zephir_literal_array_value",
 /* 381 */ "zephir_literal_array_item ::= zephir_literal_array_value",
 /* 382 */ "zephir_literal_array_key ::= IDENTIFIER",
 /* 383 */ "zephir_literal_array_key ::= STRING",
 /* 384 */ "zephir_literal_array_key ::= INTEGER",
 /* 385 */ "zephir_literal_array_value ::= zephir_literal_expr",
 /* 386 */ "zephir_eval_expr ::= zephir_common_expr",
 /* 387 */ "zephir_comment ::= COMMENT",
 /* 388 */ "zephir_cblock ::= CBLOCK",
};
#endif /* NDEBUG */

/*
** This function returns the symbolic name associated with a token
** value.
*/
const char *zephir_TokenName(int tokenType){
#ifndef NDEBUG
  if( tokenType>0 && tokenType<(sizeof(yyTokenName)/sizeof(yyTokenName[0])) ){
    return yyTokenName[tokenType];
  }else{
    return "Unknown";
  }
#else
  return "";
#endif
}

/* 
** This function allocates a new parser.
** The only argument is a pointer to a function which works like
** malloc.
**
** Inputs:
** A pointer to the function used to allocate memory.
**
** Outputs:
** A pointer to a parser.  This pointer is used in subsequent calls
** to zephir_ and zephir_Free.
*/
void *zephir_Alloc(void *(*mallocProc)(size_t)){
  yyParser *pParser;
  pParser = (yyParser*)(*mallocProc)( (size_t)sizeof(yyParser) );
  if( pParser ){
    pParser->yyidx = -1;
  }
  return pParser;
}

/* The following function deletes the value associated with a
** symbol.  The symbol can be either a terminal or nonterminal.
** "yymajor" is the symbol code, and "yypminor" is a pointer to
** the value.
*/
static void yy_destructor(YYCODETYPE yymajor, YYMINORTYPE *yypminor){
  switch( yymajor ){
    /* Here is inserted the actions which take place when a
    ** terminal or non-terminal is destroyed.  This can happen
    ** when the symbol is popped from the stack during a
    ** reduce or during error processing or when a parser is 
    ** being destroyed before it is finished parsing.
    **
    ** Note: during a reduce, the only symbols destroyed are those
    ** which appear on the RHS of the rule, but which are not used
    ** inside the C code.
    */
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
    case 16:
    case 17:
    case 18:
    case 19:
    case 20:
    case 21:
    case 22:
    case 23:
    case 24:
    case 25:
    case 26:
    case 27:
    case 28:
    case 29:
    case 30:
    case 31:
    case 32:
    case 33:
    case 34:
    case 35:
    case 36:
    case 37:
    case 38:
    case 39:
    case 40:
    case 41:
    case 42:
    case 43:
    case 44:
    case 45:
    case 46:
    case 47:
    case 48:
    case 49:
    case 50:
    case 51:
    case 52:
    case 53:
    case 54:
    case 55:
    case 56:
    case 57:
    case 58:
    case 59:
    case 60:
    case 61:
    case 62:
    case 63:
    case 64:
    case 65:
    case 66:
    case 67:
    case 68:
    case 69:
    case 70:
    case 71:
    case 72:
    case 73:
    case 74:
    case 75:
    case 76:
    case 77:
    case 78:
    case 79:
    case 80:
    case 81:
    case 82:
    case 83:
    case 84:
    case 85:
    case 86:
    case 87:
    case 88:
    case 89:
    case 90:
    case 91:
    case 92:
    case 93:
    case 94:
    case 95:
    case 96:
    case 97:
    case 98:
    case 99:
    case 100:
    case 101:
    case 102:
    case 103:
    case 104:
    case 105:
    case 106:
    case 107:
    case 108:
    case 109:
    case 110:
    case 111:
    case 112:
    case 113:
    case 114:
    case 115:
    case 116:
    case 117:
// 1278 "parser.lemon"
{
	if ((yypminor->yy0)) {
		if ((yypminor->yy0)->free_flag) {
			efree((yypminor->yy0)->token);
		}
		efree((yypminor->yy0));
	}
}
// 3973 "parser.c"
      break;
    case 120:
// 1291 "parser.lemon"
{ zval_ptr_dtor(&(yypminor->yy180)); }
// 3978 "parser.c"
      break;
    default:  break;   /* If no destructor action specified: do nothing */
  }
}

/*
** Pop the parser's stack once.
**
** If there is a destructor routine associated with the token which
** is popped from the stack, then call it.
**
** Return the major token number for the symbol popped.
*/
static int yy_pop_parser_stack(yyParser *pParser){
  YYCODETYPE yymajor;
  yyStackEntry *yytos = &pParser->yystack[pParser->yyidx];

  if( pParser->yyidx<0 ) return 0;
#ifndef NDEBUG
  if( yyTraceFILE && pParser->yyidx>=0 ){
    fprintf(yyTraceFILE,"%sPopping %s\n",
      yyTracePrompt,
      yyTokenName[yytos->major]);
  }
#endif
  yymajor = yytos->major;
  yy_destructor( yymajor, &yytos->minor);
  pParser->yyidx--;
  return yymajor;
}

/* 
** Deallocate and destroy a parser.  Destructors are all called for
** all stack elements before shutting the parser down.
**
** Inputs:
** <ul>
** <li>  A pointer to the parser.  This should be a pointer
**       obtained from zephir_Alloc.
** <li>  A pointer to a function used to reclaim memory obtained
**       from malloc.
** </ul>
*/
void zephir_Free(
  void *p,                    /* The parser to be deleted */
  void (*freeProc)(void*)     /* Function used to reclaim memory */
){
  yyParser *pParser = (yyParser*)p;
  if( pParser==0 ) return;
  while( pParser->yyidx>=0 ) yy_pop_parser_stack(pParser);
  (*freeProc)((void*)pParser);
}

/*
** Find the appropriate action for a parser given the terminal
** look-ahead token iLookAhead.
**
** If the look-ahead token is YYNOCODE, then check to see if the action is
** independent of the look-ahead.  If it is, return the action, otherwise
** return YY_NO_ACTION.
*/
static int yy_find_shift_action(
  yyParser *pParser,        /* The parser */
  int iLookAhead            /* The look-ahead token */
){
  int i;
  int stateno = pParser->yystack[pParser->yyidx].stateno;
 
  /* if( pParser->yyidx<0 ) return YY_NO_ACTION;  */
  i = yy_shift_ofst[stateno];
  if( i==YY_SHIFT_USE_DFLT ){
    return yy_default[stateno];
  }
  if( iLookAhead==YYNOCODE ){
    return YY_NO_ACTION;
  }
  i += iLookAhead;
  if( i<0 || i>=YY_SZ_ACTTAB || yy_lookahead[i]!=iLookAhead ){
#ifdef YYFALLBACK
    int iFallback;            /* Fallback token */
    if( iLookAhead<sizeof(yyFallback)/sizeof(yyFallback[0])
           && (iFallback = yyFallback[iLookAhead])!=0 ){
#ifndef NDEBUG
      if( yyTraceFILE ){
        fprintf(yyTraceFILE, "%sFALLBACK %s => %s\n",
           yyTracePrompt, yyTokenName[iLookAhead], yyTokenName[iFallback]);
      }
#endif
      return yy_find_shift_action(pParser, iFallback);
    }
#endif
    return yy_default[stateno];
  }else{
    return yy_action[i];
  }
}

/*
** Find the appropriate action for a parser given the non-terminal
** look-ahead token iLookAhead.
**
** If the look-ahead token is YYNOCODE, then check to see if the action is
** independent of the look-ahead.  If it is, return the action, otherwise
** return YY_NO_ACTION.
*/
static int yy_find_reduce_action(
  yyParser *pParser,        /* The parser */
  int iLookAhead            /* The look-ahead token */
){
  int i;
  int stateno = pParser->yystack[pParser->yyidx].stateno;
 
  i = yy_reduce_ofst[stateno];
  if( i==YY_REDUCE_USE_DFLT ){
    return yy_default[stateno];
  }
  if( iLookAhead==YYNOCODE ){
    return YY_NO_ACTION;
  }
  i += iLookAhead;
  if( i<0 || i>=YY_SZ_ACTTAB || yy_lookahead[i]!=iLookAhead ){
    return yy_default[stateno];
  }else{
    return yy_action[i];
  }
}

/*
** Perform a shift action.
*/
static void yy_shift(
  yyParser *yypParser,          /* The parser to be shifted */
  int yyNewState,               /* The new state to shift in */
  int yyMajor,                  /* The major token to shift in */
  YYMINORTYPE *yypMinor         /* Pointer ot the minor token to shift in */
){
  yyStackEntry *yytos;
  yypParser->yyidx++;
  if( yypParser->yyidx>=YYSTACKDEPTH ){
     zephir_ARG_FETCH;
     yypParser->yyidx--;
#ifndef NDEBUG
     if( yyTraceFILE ){
       fprintf(yyTraceFILE,"%sStack Overflow!\n",yyTracePrompt);
     }
#endif
     while( yypParser->yyidx>=0 ) yy_pop_parser_stack(yypParser);
     /* Here code is inserted which will execute if the parser
     ** stack every overflows */
     zephir_ARG_STORE; /* Suppress warning about unused %extra_argument var */
     return;
  }
  yytos = &yypParser->yystack[yypParser->yyidx];
  yytos->stateno = yyNewState;
  yytos->major = yyMajor;
  yytos->minor = *yypMinor;
#ifndef NDEBUG
  if( yyTraceFILE && yypParser->yyidx>0 ){
    int i;
    fprintf(yyTraceFILE,"%sShift %d\n",yyTracePrompt,yyNewState);
    fprintf(yyTraceFILE,"%sStack:",yyTracePrompt);
    for(i=1; i<=yypParser->yyidx; i++)
      fprintf(yyTraceFILE," %s",yyTokenName[yypParser->yystack[i].major]);
    fprintf(yyTraceFILE,"\n");
  }
#endif
}

/* The following table contains information about every rule that
** is used during the reduce.
*/
static struct {
  YYCODETYPE lhs;         /* Symbol on the left-hand side of the rule */
  unsigned char nrhs;     /* Number of right-hand side symbols in the rule */
} yyRuleInfo[] = {
  { 119, 1 },
  { 120, 1 },
  { 121, 2 },
  { 121, 1 },
  { 122, 1 },
  { 122, 1 },
  { 122, 1 },
  { 122, 1 },
  { 122, 1 },
  { 122, 1 },
  { 123, 3 },
  { 123, 3 },
  { 129, 3 },
  { 129, 1 },
  { 124, 1 },
  { 124, 3 },
  { 126, 3 },
  { 126, 5 },
  { 125, 3 },
  { 125, 5 },
  { 125, 5 },
  { 125, 7 },
  { 125, 4 },
  { 125, 6 },
  { 125, 6 },
  { 125, 8 },
  { 125, 4 },
  { 125, 6 },
  { 131, 2 },
  { 131, 3 },
  { 132, 3 },
  { 132, 1 },
  { 134, 1 },
  { 130, 2 },
  { 130, 3 },
  { 133, 1 },
  { 133, 1 },
  { 133, 1 },
  { 133, 2 },
  { 133, 2 },
  { 133, 2 },
  { 133, 2 },
  { 133, 3 },
  { 133, 3 },
  { 135, 1 },
  { 135, 1 },
  { 135, 2 },
  { 136, 2 },
  { 136, 1 },
  { 140, 4 },
  { 140, 3 },
  { 140, 6 },
  { 140, 5 },
  { 140, 5 },
  { 140, 4 },
  { 140, 7 },
  { 140, 6 },
  { 143, 2 },
  { 143, 3 },
  { 144, 3 },
  { 144, 1 },
  { 145, 1 },
  { 145, 2 },
  { 137, 2 },
  { 137, 1 },
  { 138, 2 },
  { 138, 1 },
  { 139, 2 },
  { 139, 1 },
  { 146, 6 },
  { 146, 5 },
  { 147, 7 },
  { 147, 8 },
  { 147, 8 },
  { 147, 9 },
  { 147, 8 },
  { 147, 9 },
  { 147, 9 },
  { 147, 10 },
  { 147, 9 },
  { 147, 10 },
  { 147, 10 },
  { 147, 11 },
  { 147, 10 },
  { 147, 11 },
  { 147, 11 },
  { 147, 12 },
  { 148, 8 },
  { 148, 9 },
  { 148, 9 },
  { 148, 10 },
  { 148, 6 },
  { 148, 7 },
  { 148, 7 },
  { 148, 8 },
  { 141, 2 },
  { 141, 1 },
  { 152, 1 },
  { 152, 1 },
  { 152, 1 },
  { 152, 1 },
  { 152, 1 },
  { 152, 1 },
  { 152, 1 },
  { 152, 1 },
  { 151, 1 },
  { 151, 1 },
  { 153, 3 },
  { 153, 1 },
  { 154, 1 },
  { 154, 1 },
  { 154, 1 },
  { 154, 2 },
  { 154, 1 },
  { 154, 1 },
  { 149, 3 },
  { 149, 1 },
  { 158, 1 },
  { 158, 2 },
  { 158, 2 },
  { 158, 3 },
  { 158, 3 },
  { 158, 4 },
  { 158, 2 },
  { 158, 3 },
  { 158, 3 },
  { 158, 4 },
  { 158, 4 },
  { 158, 5 },
  { 158, 5 },
  { 158, 6 },
  { 158, 4 },
  { 158, 5 },
  { 156, 3 },
  { 157, 5 },
  { 155, 1 },
  { 155, 1 },
  { 155, 1 },
  { 155, 1 },
  { 155, 1 },
  { 155, 1 },
  { 155, 1 },
  { 155, 1 },
  { 155, 1 },
  { 155, 1 },
  { 155, 1 },
  { 155, 1 },
  { 155, 1 },
  { 155, 1 },
  { 150, 2 },
  { 150, 1 },
  { 159, 1 },
  { 159, 1 },
  { 159, 1 },
  { 159, 1 },
  { 159, 1 },
  { 159, 1 },
  { 159, 1 },
  { 159, 1 },
  { 159, 1 },
  { 159, 1 },
  { 159, 1 },
  { 159, 1 },
  { 159, 1 },
  { 159, 1 },
  { 159, 1 },
  { 159, 1 },
  { 159, 1 },
  { 159, 1 },
  { 159, 1 },
  { 159, 1 },
  { 159, 1 },
  { 159, 1 },
  { 159, 1 },
  { 180, 1 },
  { 173, 2 },
  { 174, 2 },
  { 161, 4 },
  { 161, 7 },
  { 161, 5 },
  { 161, 9 },
  { 161, 8 },
  { 161, 8 },
  { 178, 4 },
  { 178, 5 },
  { 182, 2 },
  { 182, 1 },
  { 183, 3 },
  { 183, 4 },
  { 183, 3 },
  { 162, 3 },
  { 162, 4 },
  { 175, 4 },
  { 175, 5 },
  { 176, 6 },
  { 176, 7 },
  { 177, 3 },
  { 177, 4 },
  { 177, 5 },
  { 184, 2 },
  { 184, 1 },
  { 185, 5 },
  { 185, 4 },
  { 185, 6 },
  { 185, 7 },
  { 186, 3 },
  { 186, 1 },
  { 187, 1 },
  { 179, 7 },
  { 179, 6 },
  { 179, 8 },
  { 179, 9 },
  { 179, 8 },
  { 179, 10 },
  { 160, 3 },
  { 189, 3 },
  { 189, 1 },
  { 191, 1 },
  { 191, 1 },
  { 191, 1 },
  { 191, 1 },
  { 191, 1 },
  { 191, 1 },
  { 191, 1 },
  { 190, 3 },
  { 190, 5 },
  { 190, 7 },
  { 190, 7 },
  { 190, 7 },
  { 190, 6 },
  { 190, 8 },
  { 190, 5 },
  { 190, 7 },
  { 190, 6 },
  { 190, 8 },
  { 190, 5 },
  { 190, 4 },
  { 190, 6 },
  { 193, 2 },
  { 193, 1 },
  { 194, 3 },
  { 190, 4 },
  { 190, 4 },
  { 190, 2 },
  { 190, 2 },
  { 190, 5 },
  { 195, 1 },
  { 163, 3 },
  { 196, 3 },
  { 196, 1 },
  { 197, 1 },
  { 168, 2 },
  { 167, 2 },
  { 169, 2 },
  { 166, 2 },
  { 164, 3 },
  { 164, 2 },
  { 165, 3 },
  { 170, 3 },
  { 171, 3 },
  { 172, 3 },
  { 172, 3 },
  { 172, 3 },
  { 172, 3 },
  { 172, 3 },
  { 172, 3 },
  { 172, 3 },
  { 172, 3 },
  { 172, 3 },
  { 172, 3 },
  { 172, 3 },
  { 202, 3 },
  { 202, 1 },
  { 203, 1 },
  { 203, 3 },
  { 192, 1 },
  { 188, 2 },
  { 188, 2 },
  { 188, 2 },
  { 188, 2 },
  { 188, 2 },
  { 188, 2 },
  { 188, 2 },
  { 188, 2 },
  { 188, 3 },
  { 188, 3 },
  { 188, 3 },
  { 188, 3 },
  { 188, 3 },
  { 188, 3 },
  { 188, 3 },
  { 188, 3 },
  { 188, 3 },
  { 188, 4 },
  { 188, 4 },
  { 188, 3 },
  { 188, 5 },
  { 188, 5 },
  { 188, 3 },
  { 188, 3 },
  { 188, 4 },
  { 188, 3 },
  { 188, 3 },
  { 188, 3 },
  { 188, 3 },
  { 188, 3 },
  { 188, 3 },
  { 188, 3 },
  { 188, 3 },
  { 188, 3 },
  { 188, 3 },
  { 188, 3 },
  { 188, 3 },
  { 188, 3 },
  { 188, 3 },
  { 201, 4 },
  { 188, 1 },
  { 188, 2 },
  { 188, 1 },
  { 188, 1 },
  { 188, 1 },
  { 188, 1 },
  { 188, 1 },
  { 188, 1 },
  { 188, 1 },
  { 188, 1 },
  { 188, 1 },
  { 188, 2 },
  { 188, 3 },
  { 188, 2 },
  { 188, 4 },
  { 188, 5 },
  { 188, 4 },
  { 188, 6 },
  { 188, 7 },
  { 199, 4 },
  { 199, 3 },
  { 199, 6 },
  { 199, 5 },
  { 200, 6 },
  { 200, 5 },
  { 200, 8 },
  { 200, 7 },
  { 200, 10 },
  { 200, 9 },
  { 198, 6 },
  { 198, 5 },
  { 198, 8 },
  { 198, 7 },
  { 198, 8 },
  { 198, 7 },
  { 188, 1 },
  { 188, 1 },
  { 188, 1 },
  { 188, 5 },
  { 205, 3 },
  { 205, 1 },
  { 206, 1 },
  { 206, 3 },
  { 204, 3 },
  { 204, 1 },
  { 207, 3 },
  { 207, 1 },
  { 208, 1 },
  { 208, 1 },
  { 208, 1 },
  { 209, 1 },
  { 142, 1 },
  { 142, 1 },
  { 142, 1 },
  { 142, 1 },
  { 142, 1 },
  { 142, 1 },
  { 142, 1 },
  { 142, 3 },
  { 142, 1 },
  { 142, 2 },
  { 142, 3 },
  { 210, 3 },
  { 210, 1 },
  { 211, 3 },
  { 211, 1 },
  { 212, 1 },
  { 212, 1 },
  { 212, 1 },
  { 213, 1 },
  { 181, 1 },
  { 127, 1 },
  { 128, 1 },
};

static void yy_accept(yyParser*);  /* Forward Declaration */

/*
** Perform a reduce action and the shift that must immediately
** follow the reduce.
*/
static void yy_reduce(
  yyParser *yypParser,         /* The parser */
  int yyruleno                 /* Number of the rule by which to reduce */
){
  int yygoto;                     /* The next state */
  int yyact;                      /* The next action */
  YYMINORTYPE yygotominor;        /* The LHS of the rule reduced */
  yyStackEntry *yymsp;            /* The top of the parser's stack */
  int yysize;                     /* Amount to pop the stack */
  zephir_ARG_FETCH;
  yymsp = &yypParser->yystack[yypParser->yyidx];
#ifndef NDEBUG
  if( yyTraceFILE && yyruleno>=0 
        && yyruleno<sizeof(yyRuleName)/sizeof(yyRuleName[0]) ){
    fprintf(yyTraceFILE, "%sReduce [%s].\n", yyTracePrompt,
      yyRuleName[yyruleno]);
  }
#endif /* NDEBUG */

  switch( yyruleno ){
  /* Beginning here are the reduction cases.  A typical example
  ** follows:
  **   case 0:
  **  // <lineno> <grammarfile>
  **     { ... }           // User supplied code
  **  // <lineno> <thisfile>
  **     break;
  */
      case 0:
// 1287 "parser.lemon"
{
	status->ret = yymsp[0].minor.yy180;
}
// 4584 "parser.c"
        break;
      case 1:
      case 4:
      case 5:
      case 6:
      case 7:
      case 8:
      case 9:
      case 151:
      case 152:
      case 153:
      case 154:
      case 155:
      case 156:
      case 157:
      case 158:
      case 159:
      case 160:
      case 161:
      case 162:
      case 163:
      case 164:
      case 165:
      case 166:
      case 167:
      case 168:
      case 169:
      case 170:
      case 171:
      case 172:
      case 173:
      case 246:
      case 275:
      case 316:
      case 351:
      case 352:
      case 353:
      case 366:
      case 385:
      case 386:
// 1293 "parser.lemon"
{
	yygotominor.yy180 = yymsp[0].minor.yy180;
}
// 4629 "parser.c"
        break;
      case 2:
      case 47:
      case 63:
      case 65:
      case 67:
      case 95:
      case 149:
      case 185:
      case 199:
      case 238:
// 1297 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_list(yymsp[-1].minor.yy180, yymsp[0].minor.yy180);
}
// 4645 "parser.c"
        break;
      case 3:
      case 13:
      case 31:
      case 48:
      case 60:
      case 64:
      case 66:
      case 68:
      case 96:
      case 108:
      case 116:
      case 150:
      case 186:
      case 200:
      case 206:
      case 216:
      case 239:
      case 249:
      case 272:
      case 356:
      case 360:
      case 379:
// 1301 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_list(NULL, yymsp[0].minor.yy180);
}
// 4673 "parser.c"
        break;
      case 10:
// 1329 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_namespace(yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(43,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 4682 "parser.c"
        break;
      case 11:
// 1333 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_use_aliases(yymsp[-1].minor.yy180, status->scanner_state);
  yy_destructor(46,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 4691 "parser.c"
        break;
      case 12:
      case 30:
      case 59:
      case 115:
      case 215:
      case 248:
      case 271:
      case 355:
      case 359:
      case 378:
// 1337 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_list(yymsp[-2].minor.yy180, yymsp[0].minor.yy180);
  yy_destructor(6,&yymsp[-1].minor);
}
// 4708 "parser.c"
        break;
      case 14:
// 1345 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_use_aliases_item(yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 4715 "parser.c"
        break;
      case 15:
// 1349 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_use_aliases_item(yymsp[-2].minor.yy0, yymsp[0].minor.yy0, status->scanner_state);
  yy_destructor(47,&yymsp[-1].minor);
}
// 4723 "parser.c"
        break;
      case 16:
// 1353 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_interface(yymsp[-1].minor.yy0, yymsp[0].minor.yy180, NULL, status->scanner_state);
  yy_destructor(48,&yymsp[-2].minor);
}
// 4731 "parser.c"
        break;
      case 17:
// 1357 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_interface(yymsp[-3].minor.yy0, yymsp[0].minor.yy180, yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(48,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
}
// 4740 "parser.c"
        break;
      case 18:
// 1361 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_class(yymsp[-1].minor.yy0, yymsp[0].minor.yy180, 0, 0, NULL, NULL, status->scanner_state);
  yy_destructor(50,&yymsp[-2].minor);
}
// 4748 "parser.c"
        break;
      case 19:
// 1365 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy180, 0, 0, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(50,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
}
// 4757 "parser.c"
        break;
      case 20:
// 1369 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy180, 0, 0, NULL, yymsp[-1].minor.yy180, status->scanner_state);
  yy_destructor(50,&yymsp[-4].minor);
  yy_destructor(51,&yymsp[-2].minor);
}
// 4766 "parser.c"
        break;
      case 21:
// 1373 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_class(yymsp[-5].minor.yy0, yymsp[0].minor.yy180, 0, 0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy180, status->scanner_state);
  yy_destructor(50,&yymsp[-6].minor);
  yy_destructor(49,&yymsp[-4].minor);
  yy_destructor(51,&yymsp[-2].minor);
}
// 4776 "parser.c"
        break;
      case 22:
// 1377 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_class(yymsp[-1].minor.yy0, yymsp[0].minor.yy180, 1, 0, NULL, NULL, status->scanner_state);
  yy_destructor(52,&yymsp[-3].minor);
  yy_destructor(50,&yymsp[-2].minor);
}
// 4785 "parser.c"
        break;
      case 23:
// 1381 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy180, 1, 0, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(52,&yymsp[-5].minor);
  yy_destructor(50,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
}
// 4795 "parser.c"
        break;
      case 24:
// 1385 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy180, 1, 0, NULL, yymsp[-1].minor.yy180, status->scanner_state);
  yy_destructor(52,&yymsp[-5].minor);
  yy_destructor(50,&yymsp[-4].minor);
  yy_destructor(51,&yymsp[-2].minor);
}
// 4805 "parser.c"
        break;
      case 25:
// 1389 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_class(yymsp[-5].minor.yy0, yymsp[0].minor.yy180, 1, 0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy180, status->scanner_state);
  yy_destructor(52,&yymsp[-7].minor);
  yy_destructor(50,&yymsp[-6].minor);
  yy_destructor(49,&yymsp[-4].minor);
  yy_destructor(51,&yymsp[-2].minor);
}
// 4816 "parser.c"
        break;
      case 26:
// 1393 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_class(yymsp[-1].minor.yy0, yymsp[0].minor.yy180, 0, 1, NULL, NULL, status->scanner_state);
  yy_destructor(53,&yymsp[-3].minor);
  yy_destructor(50,&yymsp[-2].minor);
}
// 4825 "parser.c"
        break;
      case 27:
// 1397 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy180, 0, 1, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(53,&yymsp[-5].minor);
  yy_destructor(50,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
}
// 4835 "parser.c"
        break;
      case 28:
      case 57:
// 1401 "parser.lemon"
{
	yygotominor.yy180 = NULL;
  yy_destructor(54,&yymsp[-1].minor);
  yy_destructor(55,&yymsp[0].minor);
}
// 4845 "parser.c"
        break;
      case 29:
      case 58:
// 1405 "parser.lemon"
{
	yygotominor.yy180 = yymsp[-1].minor.yy180;
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(55,&yymsp[0].minor);
}
// 4855 "parser.c"
        break;
      case 32:
      case 207:
      case 318:
      case 363:
      case 382:
// 1417 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_literal(ZEPHIR_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state);
}
// 4866 "parser.c"
        break;
      case 33:
// 1421 "parser.lemon"
{
  yygotominor.yy180 = NULL;
  yy_destructor(54,&yymsp[-1].minor);
  yy_destructor(55,&yymsp[0].minor);
}
// 4875 "parser.c"
        break;
      case 34:
// 1425 "parser.lemon"
{
  yygotominor.yy180 = yymsp[-1].minor.yy180;
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(55,&yymsp[0].minor);
}
// 4884 "parser.c"
        break;
      case 35:
// 1429 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_class_definition(yymsp[0].minor.yy180, NULL, NULL, status->scanner_state);
}
// 4891 "parser.c"
        break;
      case 36:
// 1433 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_class_definition(NULL, NULL, yymsp[0].minor.yy180, status->scanner_state);
}
// 4898 "parser.c"
        break;
      case 37:
// 1437 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_class_definition(NULL, yymsp[0].minor.yy180, NULL, status->scanner_state);
}
// 4905 "parser.c"
        break;
      case 38:
// 1441 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_class_definition(yymsp[-1].minor.yy180, yymsp[0].minor.yy180, NULL, status->scanner_state);
}
// 4912 "parser.c"
        break;
      case 39:
// 1445 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_class_definition(yymsp[-1].minor.yy180, NULL, yymsp[0].minor.yy180, status->scanner_state);
}
// 4919 "parser.c"
        break;
      case 40:
// 1449 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_class_definition(yymsp[0].minor.yy180, NULL, yymsp[-1].minor.yy180, status->scanner_state);
}
// 4926 "parser.c"
        break;
      case 41:
// 1453 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_class_definition(NULL, yymsp[0].minor.yy180, yymsp[-1].minor.yy180, status->scanner_state);
}
// 4933 "parser.c"
        break;
      case 42:
// 1457 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_class_definition(yymsp[-2].minor.yy180, yymsp[0].minor.yy180, yymsp[-1].minor.yy180, status->scanner_state);
}
// 4940 "parser.c"
        break;
      case 43:
// 1461 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_class_definition(yymsp[-1].minor.yy180, yymsp[0].minor.yy180, yymsp[-2].minor.yy180, status->scanner_state);
}
// 4947 "parser.c"
        break;
      case 44:
// 1465 "parser.lemon"
{
  yygotominor.yy180 = zephir_ret_interface_definition(NULL, yymsp[0].minor.yy180, status->scanner_state);
}
// 4954 "parser.c"
        break;
      case 45:
// 1469 "parser.lemon"
{
  yygotominor.yy180 = zephir_ret_interface_definition(yymsp[0].minor.yy180, NULL, status->scanner_state);
}
// 4961 "parser.c"
        break;
      case 46:
// 1473 "parser.lemon"
{
  yygotominor.yy180 = zephir_ret_interface_definition(yymsp[0].minor.yy180, yymsp[-1].minor.yy180, status->scanner_state);
}
// 4968 "parser.c"
        break;
      case 49:
// 1486 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_class_property(yymsp[-2].minor.yy180, yymsp[-1].minor.yy0, NULL, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(45,&yymsp[0].minor);
}
// 4976 "parser.c"
        break;
      case 50:
// 1490 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_class_property(yymsp[-2].minor.yy180, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(45,&yymsp[0].minor);
}
// 4984 "parser.c"
        break;
      case 51:
// 1494 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_class_property(yymsp[-4].minor.yy180, yymsp[-3].minor.yy0, yymsp[-1].minor.yy180, yymsp[-5].minor.yy0, NULL, status->scanner_state);
  yy_destructor(57,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 4993 "parser.c"
        break;
      case 52:
// 1498 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_class_property(yymsp[-4].minor.yy180, yymsp[-3].minor.yy0, yymsp[-1].minor.yy180, NULL, NULL, status->scanner_state);
  yy_destructor(57,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 5002 "parser.c"
        break;
      case 53:
// 1502 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_class_property(yymsp[-3].minor.yy180, yymsp[-2].minor.yy0, NULL, yymsp[-4].minor.yy0, yymsp[-1].minor.yy180, status->scanner_state);
  yy_destructor(45,&yymsp[0].minor);
}
// 5010 "parser.c"
        break;
      case 54:
// 1506 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_class_property(yymsp[-3].minor.yy180, yymsp[-2].minor.yy0, NULL, NULL, yymsp[-1].minor.yy180, status->scanner_state);
  yy_destructor(45,&yymsp[0].minor);
}
// 5018 "parser.c"
        break;
      case 55:
// 1510 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_class_property(yymsp[-5].minor.yy180, yymsp[-4].minor.yy0, yymsp[-2].minor.yy180, yymsp[-6].minor.yy0, yymsp[-1].minor.yy180, status->scanner_state);
  yy_destructor(57,&yymsp[-3].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 5027 "parser.c"
        break;
      case 56:
// 1514 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_class_property(yymsp[-5].minor.yy180, yymsp[-4].minor.yy0, yymsp[-2].minor.yy180, NULL, yymsp[-1].minor.yy180, status->scanner_state);
  yy_destructor(57,&yymsp[-3].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 5036 "parser.c"
        break;
      case 61:
// 1534 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_property_shortcut(NULL, yymsp[0].minor.yy0, status->scanner_state);
}
// 5043 "parser.c"
        break;
      case 62:
// 1538 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_property_shortcut(yymsp[-1].minor.yy0, yymsp[0].minor.yy0, status->scanner_state);
}
// 5050 "parser.c"
        break;
      case 69:
// 1567 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy180, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(58,&yymsp[-4].minor);
  yy_destructor(57,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 5060 "parser.c"
        break;
      case 70:
// 1571 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy180, NULL, status->scanner_state);
  yy_destructor(58,&yymsp[-4].minor);
  yy_destructor(57,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 5070 "parser.c"
        break;
      case 71:
// 1576 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_class_method(yymsp[-6].minor.yy180, yymsp[-4].minor.yy0, NULL, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(60,&yymsp[-5].minor);
  yy_destructor(61,&yymsp[-3].minor);
  yy_destructor(40,&yymsp[-2].minor);
  yy_destructor(54,&yymsp[-1].minor);
  yy_destructor(55,&yymsp[0].minor);
}
// 5082 "parser.c"
        break;
      case 72:
// 1580 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_class_method(yymsp[-7].minor.yy180, yymsp[-5].minor.yy0, yymsp[-3].minor.yy180, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(60,&yymsp[-6].minor);
  yy_destructor(61,&yymsp[-4].minor);
  yy_destructor(40,&yymsp[-2].minor);
  yy_destructor(54,&yymsp[-1].minor);
  yy_destructor(55,&yymsp[0].minor);
}
// 5094 "parser.c"
        break;
      case 73:
// 1584 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_class_method(yymsp[-7].minor.yy180, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy180, NULL, NULL, status->scanner_state);
  yy_destructor(60,&yymsp[-6].minor);
  yy_destructor(61,&yymsp[-4].minor);
  yy_destructor(40,&yymsp[-3].minor);
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(55,&yymsp[0].minor);
}
// 5106 "parser.c"
        break;
      case 74:
// 1588 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_class_method(yymsp[-8].minor.yy180, yymsp[-6].minor.yy0, yymsp[-4].minor.yy180, yymsp[-1].minor.yy180, NULL, NULL, status->scanner_state);
  yy_destructor(60,&yymsp[-7].minor);
  yy_destructor(61,&yymsp[-5].minor);
  yy_destructor(40,&yymsp[-3].minor);
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(55,&yymsp[0].minor);
}
// 5118 "parser.c"
        break;
      case 75:
// 1592 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_class_method(yymsp[-6].minor.yy180, yymsp[-4].minor.yy0, NULL, NULL, yymsp[-7].minor.yy0, NULL, status->scanner_state);
  yy_destructor(60,&yymsp[-5].minor);
  yy_destructor(61,&yymsp[-3].minor);
  yy_destructor(40,&yymsp[-2].minor);
  yy_destructor(54,&yymsp[-1].minor);
  yy_destructor(55,&yymsp[0].minor);
}
// 5130 "parser.c"
        break;
      case 76:
// 1596 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_class_method(yymsp[-7].minor.yy180, yymsp[-5].minor.yy0, yymsp[-3].minor.yy180, NULL, yymsp[-8].minor.yy0, NULL, status->scanner_state);
  yy_destructor(60,&yymsp[-6].minor);
  yy_destructor(61,&yymsp[-4].minor);
  yy_destructor(40,&yymsp[-2].minor);
  yy_destructor(54,&yymsp[-1].minor);
  yy_destructor(55,&yymsp[0].minor);
}
// 5142 "parser.c"
        break;
      case 77:
// 1600 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_class_method(yymsp[-7].minor.yy180, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy180, yymsp[-8].minor.yy0, NULL, status->scanner_state);
  yy_destructor(60,&yymsp[-6].minor);
  yy_destructor(61,&yymsp[-4].minor);
  yy_destructor(40,&yymsp[-3].minor);
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(55,&yymsp[0].minor);
}
// 5154 "parser.c"
        break;
      case 78:
// 1604 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_class_method(yymsp[-8].minor.yy180, yymsp[-6].minor.yy0, yymsp[-4].minor.yy180, yymsp[-1].minor.yy180, yymsp[-9].minor.yy0, NULL, status->scanner_state);
  yy_destructor(60,&yymsp[-7].minor);
  yy_destructor(61,&yymsp[-5].minor);
  yy_destructor(40,&yymsp[-3].minor);
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(55,&yymsp[0].minor);
}
// 5166 "parser.c"
        break;
      case 79:
// 1608 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_class_method(yymsp[-8].minor.yy180, yymsp[-6].minor.yy0, NULL, NULL, NULL, yymsp[-2].minor.yy180, status->scanner_state);
  yy_destructor(60,&yymsp[-7].minor);
  yy_destructor(61,&yymsp[-5].minor);
  yy_destructor(40,&yymsp[-4].minor);
  yy_destructor(42,&yymsp[-3].minor);
  yy_destructor(54,&yymsp[-1].minor);
  yy_destructor(55,&yymsp[0].minor);
}
// 5179 "parser.c"
        break;
      case 80:
// 1612 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_class_method(yymsp[-9].minor.yy180, yymsp[-7].minor.yy0, yymsp[-5].minor.yy180, NULL, NULL, yymsp[-2].minor.yy180, status->scanner_state);
  yy_destructor(60,&yymsp[-8].minor);
  yy_destructor(61,&yymsp[-6].minor);
  yy_destructor(40,&yymsp[-4].minor);
  yy_destructor(42,&yymsp[-3].minor);
  yy_destructor(54,&yymsp[-1].minor);
  yy_destructor(55,&yymsp[0].minor);
}
// 5192 "parser.c"
        break;
      case 81:
// 1616 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_class_method(yymsp[-9].minor.yy180, yymsp[-7].minor.yy0, NULL, yymsp[-1].minor.yy180, NULL, yymsp[-3].minor.yy180, status->scanner_state);
  yy_destructor(60,&yymsp[-8].minor);
  yy_destructor(61,&yymsp[-6].minor);
  yy_destructor(40,&yymsp[-5].minor);
  yy_destructor(42,&yymsp[-4].minor);
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(55,&yymsp[0].minor);
}
// 5205 "parser.c"
        break;
      case 82:
// 1620 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_class_method(yymsp[-10].minor.yy180, yymsp[-8].minor.yy0, yymsp[-6].minor.yy180, yymsp[-1].minor.yy180, NULL, yymsp[-3].minor.yy180, status->scanner_state);
  yy_destructor(60,&yymsp[-9].minor);
  yy_destructor(61,&yymsp[-7].minor);
  yy_destructor(40,&yymsp[-5].minor);
  yy_destructor(42,&yymsp[-4].minor);
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(55,&yymsp[0].minor);
}
// 5218 "parser.c"
        break;
      case 83:
// 1624 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_class_method(yymsp[-8].minor.yy180, yymsp[-6].minor.yy0, NULL, NULL, yymsp[-9].minor.yy0, yymsp[-2].minor.yy180, status->scanner_state);
  yy_destructor(60,&yymsp[-7].minor);
  yy_destructor(61,&yymsp[-5].minor);
  yy_destructor(40,&yymsp[-4].minor);
  yy_destructor(42,&yymsp[-3].minor);
  yy_destructor(54,&yymsp[-1].minor);
  yy_destructor(55,&yymsp[0].minor);
}
// 5231 "parser.c"
        break;
      case 84:
// 1628 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_class_method(yymsp[-9].minor.yy180, yymsp[-7].minor.yy0, yymsp[-5].minor.yy180, NULL, yymsp[-10].minor.yy0, yymsp[-2].minor.yy180, status->scanner_state);
  yy_destructor(60,&yymsp[-8].minor);
  yy_destructor(61,&yymsp[-6].minor);
  yy_destructor(40,&yymsp[-4].minor);
  yy_destructor(42,&yymsp[-3].minor);
  yy_destructor(54,&yymsp[-1].minor);
  yy_destructor(55,&yymsp[0].minor);
}
// 5244 "parser.c"
        break;
      case 85:
// 1632 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_class_method(yymsp[-9].minor.yy180, yymsp[-7].minor.yy0, NULL, yymsp[-1].minor.yy180, yymsp[-10].minor.yy0, yymsp[-3].minor.yy180, status->scanner_state);
  yy_destructor(60,&yymsp[-8].minor);
  yy_destructor(61,&yymsp[-6].minor);
  yy_destructor(40,&yymsp[-5].minor);
  yy_destructor(42,&yymsp[-4].minor);
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(55,&yymsp[0].minor);
}
// 5257 "parser.c"
        break;
      case 86:
// 1636 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_class_method(yymsp[-10].minor.yy180, yymsp[-8].minor.yy0, yymsp[-6].minor.yy180, yymsp[-1].minor.yy180, yymsp[-11].minor.yy0, yymsp[-3].minor.yy180, status->scanner_state);
  yy_destructor(60,&yymsp[-9].minor);
  yy_destructor(61,&yymsp[-7].minor);
  yy_destructor(40,&yymsp[-5].minor);
  yy_destructor(42,&yymsp[-4].minor);
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(55,&yymsp[0].minor);
}
// 5270 "parser.c"
        break;
      case 87:
// 1641 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_class_method(yymsp[-7].minor.yy180, yymsp[-5].minor.yy0, NULL, NULL, NULL, yymsp[-1].minor.yy180, status->scanner_state);
  yy_destructor(60,&yymsp[-6].minor);
  yy_destructor(61,&yymsp[-4].minor);
  yy_destructor(40,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 5282 "parser.c"
        break;
      case 88:
// 1645 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_class_method(yymsp[-8].minor.yy180, yymsp[-6].minor.yy0, yymsp[-4].minor.yy180, NULL, NULL, yymsp[-1].minor.yy180, status->scanner_state);
  yy_destructor(60,&yymsp[-7].minor);
  yy_destructor(61,&yymsp[-5].minor);
  yy_destructor(40,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 5294 "parser.c"
        break;
      case 89:
// 1649 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_class_method(yymsp[-7].minor.yy180, yymsp[-5].minor.yy0, NULL, NULL, yymsp[-8].minor.yy0, yymsp[-1].minor.yy180, status->scanner_state);
  yy_destructor(60,&yymsp[-6].minor);
  yy_destructor(61,&yymsp[-4].minor);
  yy_destructor(40,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 5306 "parser.c"
        break;
      case 90:
// 1653 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_class_method(yymsp[-8].minor.yy180, yymsp[-6].minor.yy0, yymsp[-4].minor.yy180, NULL, yymsp[-9].minor.yy0, yymsp[-1].minor.yy180, status->scanner_state);
  yy_destructor(60,&yymsp[-7].minor);
  yy_destructor(61,&yymsp[-5].minor);
  yy_destructor(40,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 5318 "parser.c"
        break;
      case 91:
// 1657 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_class_method(yymsp[-5].minor.yy180, yymsp[-3].minor.yy0, NULL, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(60,&yymsp[-4].minor);
  yy_destructor(61,&yymsp[-2].minor);
  yy_destructor(40,&yymsp[-1].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 5329 "parser.c"
        break;
      case 92:
// 1661 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_class_method(yymsp[-6].minor.yy180, yymsp[-4].minor.yy0, yymsp[-2].minor.yy180, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(60,&yymsp[-5].minor);
  yy_destructor(61,&yymsp[-3].minor);
  yy_destructor(40,&yymsp[-1].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 5340 "parser.c"
        break;
      case 93:
// 1665 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_class_method(yymsp[-5].minor.yy180, yymsp[-3].minor.yy0, NULL, NULL, yymsp[-6].minor.yy0, NULL, status->scanner_state);
  yy_destructor(60,&yymsp[-4].minor);
  yy_destructor(61,&yymsp[-2].minor);
  yy_destructor(40,&yymsp[-1].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 5351 "parser.c"
        break;
      case 94:
// 1669 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_class_method(yymsp[-6].minor.yy180, yymsp[-4].minor.yy0, yymsp[-2].minor.yy180, NULL, yymsp[-7].minor.yy0, NULL, status->scanner_state);
  yy_destructor(60,&yymsp[-5].minor);
  yy_destructor(61,&yymsp[-3].minor);
  yy_destructor(40,&yymsp[-1].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 5362 "parser.c"
        break;
      case 97:
// 1682 "parser.lemon"
{
	yygotominor.yy180 = zephir_new_string("public");
  yy_destructor(1,&yymsp[0].minor);
}
// 5370 "parser.c"
        break;
      case 98:
// 1686 "parser.lemon"
{
	yygotominor.yy180 = zephir_new_string("protected");
  yy_destructor(2,&yymsp[0].minor);
}
// 5378 "parser.c"
        break;
      case 99:
// 1690 "parser.lemon"
{
	yygotominor.yy180 = zephir_new_string("private");
  yy_destructor(4,&yymsp[0].minor);
}
// 5386 "parser.c"
        break;
      case 100:
// 1694 "parser.lemon"
{
	yygotominor.yy180 = zephir_new_string("static");
  yy_destructor(3,&yymsp[0].minor);
}
// 5394 "parser.c"
        break;
      case 101:
// 1698 "parser.lemon"
{
	yygotominor.yy180 = zephir_new_string("scoped");
  yy_destructor(5,&yymsp[0].minor);
}
// 5402 "parser.c"
        break;
      case 102:
// 1702 "parser.lemon"
{
	yygotominor.yy180 = zephir_new_string("inline");
  yy_destructor(62,&yymsp[0].minor);
}
// 5410 "parser.c"
        break;
      case 103:
// 1706 "parser.lemon"
{
	yygotominor.yy180 = zephir_new_string("abstract");
  yy_destructor(52,&yymsp[0].minor);
}
// 5418 "parser.c"
        break;
      case 104:
// 1710 "parser.lemon"
{
	yygotominor.yy180 = zephir_new_string("final");
  yy_destructor(53,&yymsp[0].minor);
}
// 5426 "parser.c"
        break;
      case 105:
// 1715 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_return_type(1, NULL, status->scanner_state);
  yy_destructor(63,&yymsp[0].minor);
}
// 5434 "parser.c"
        break;
      case 106:
// 1719 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_return_type(0, yymsp[0].minor.yy180, status->scanner_state);
}
// 5441 "parser.c"
        break;
      case 107:
      case 205:
// 1723 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_list(yymsp[-2].minor.yy180, yymsp[0].minor.yy180);
  yy_destructor(14,&yymsp[-1].minor);
}
// 5450 "parser.c"
        break;
      case 109:
// 1731 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_return_type_item(yymsp[0].minor.yy180, NULL, 0, 0, status->scanner_state);
}
// 5457 "parser.c"
        break;
      case 110:
// 1735 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_return_type_item(zephir_ret_type(ZEPHIR_T_TYPE_NULL), NULL, 0, 0, status->scanner_state);
  yy_destructor(64,&yymsp[0].minor);
}
// 5465 "parser.c"
        break;
      case 111:
// 1739 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_return_type_item(zephir_ret_type(ZEPHIR_T_TYPE_THIS), NULL, 0, 0, status->scanner_state);
  yy_destructor(65,&yymsp[0].minor);
}
// 5473 "parser.c"
        break;
      case 112:
// 1743 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_return_type_item(yymsp[-1].minor.yy180, NULL, 1, 0, status->scanner_state);
  yy_destructor(39,&yymsp[0].minor);
}
// 5481 "parser.c"
        break;
      case 113:
// 1747 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_return_type_item(NULL, yymsp[0].minor.yy180, 0, 0, status->scanner_state);
}
// 5488 "parser.c"
        break;
      case 114:
// 1751 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_return_type_item(NULL, yymsp[0].minor.yy180, 0, 1, status->scanner_state);
}
// 5495 "parser.c"
        break;
      case 117:
// 1765 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_parameter(0, NULL, NULL, yymsp[0].minor.yy0, NULL, 0, status->scanner_state);
}
// 5502 "parser.c"
        break;
      case 118:
// 1769 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_parameter(1, NULL, NULL, yymsp[0].minor.yy0, NULL, 0, status->scanner_state);
  yy_destructor(58,&yymsp[-1].minor);
}
// 5510 "parser.c"
        break;
      case 119:
// 1773 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_parameter(0, yymsp[-1].minor.yy180, NULL, yymsp[0].minor.yy0, NULL, 0, status->scanner_state);
}
// 5517 "parser.c"
        break;
      case 120:
// 1777 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_parameter(1, yymsp[-1].minor.yy180, NULL, yymsp[0].minor.yy0, NULL, 0, status->scanner_state);
  yy_destructor(58,&yymsp[-2].minor);
}
// 5525 "parser.c"
        break;
      case 121:
// 1781 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_parameter(0, yymsp[-2].minor.yy180, NULL, yymsp[0].minor.yy0, NULL, 1, status->scanner_state);
  yy_destructor(39,&yymsp[-1].minor);
}
// 5533 "parser.c"
        break;
      case 122:
// 1785 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_parameter(1, yymsp[-2].minor.yy180, NULL, yymsp[0].minor.yy0, NULL, 1, status->scanner_state);
  yy_destructor(58,&yymsp[-3].minor);
  yy_destructor(39,&yymsp[-1].minor);
}
// 5542 "parser.c"
        break;
      case 123:
// 1789 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_parameter(0, NULL, yymsp[-1].minor.yy180, yymsp[0].minor.yy0, NULL, 0, status->scanner_state);
}
// 5549 "parser.c"
        break;
      case 124:
// 1793 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_parameter(1, NULL, yymsp[-1].minor.yy180, yymsp[0].minor.yy0, NULL, 0, status->scanner_state);
  yy_destructor(58,&yymsp[-2].minor);
}
// 5557 "parser.c"
        break;
      case 125:
// 1797 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_parameter(0, NULL, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy180, 0, status->scanner_state);
  yy_destructor(57,&yymsp[-1].minor);
}
// 5565 "parser.c"
        break;
      case 126:
// 1801 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_parameter(1, NULL, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy180, 0, status->scanner_state);
  yy_destructor(58,&yymsp[-3].minor);
  yy_destructor(57,&yymsp[-1].minor);
}
// 5574 "parser.c"
        break;
      case 127:
// 1805 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_parameter(0, yymsp[-3].minor.yy180, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy180, 0, status->scanner_state);
  yy_destructor(57,&yymsp[-1].minor);
}
// 5582 "parser.c"
        break;
      case 128:
// 1809 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_parameter(1, yymsp[-3].minor.yy180, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy180, 0, status->scanner_state);
  yy_destructor(58,&yymsp[-4].minor);
  yy_destructor(57,&yymsp[-1].minor);
}
// 5591 "parser.c"
        break;
      case 129:
// 1813 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_parameter(0, yymsp[-4].minor.yy180, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy180, 1, status->scanner_state);
  yy_destructor(39,&yymsp[-3].minor);
  yy_destructor(57,&yymsp[-1].minor);
}
// 5600 "parser.c"
        break;
      case 130:
// 1817 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_parameter(1, yymsp[-4].minor.yy180, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy180, 1, status->scanner_state);
  yy_destructor(58,&yymsp[-5].minor);
  yy_destructor(39,&yymsp[-3].minor);
  yy_destructor(57,&yymsp[-1].minor);
}
// 5610 "parser.c"
        break;
      case 131:
// 1821 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_parameter(0, NULL, yymsp[-3].minor.yy180, yymsp[-2].minor.yy0, yymsp[0].minor.yy180, 0, status->scanner_state);
  yy_destructor(57,&yymsp[-1].minor);
}
// 5618 "parser.c"
        break;
      case 132:
// 1825 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_parameter(1, NULL, yymsp[-3].minor.yy180, yymsp[-2].minor.yy0, yymsp[0].minor.yy180, 0, status->scanner_state);
  yy_destructor(58,&yymsp[-4].minor);
  yy_destructor(57,&yymsp[-1].minor);
}
// 5627 "parser.c"
        break;
      case 133:
// 1830 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_literal(ZEPHIR_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(21,&yymsp[-2].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 5636 "parser.c"
        break;
      case 134:
// 1834 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_literal(ZEPHIR_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(21,&yymsp[-4].minor);
  yy_destructor(41,&yymsp[-2].minor);
  yy_destructor(66,&yymsp[-1].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 5647 "parser.c"
        break;
      case 135:
// 1838 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_type(ZEPHIR_TYPE_INTEGER);
  yy_destructor(67,&yymsp[0].minor);
}
// 5655 "parser.c"
        break;
      case 136:
// 1842 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_type(ZEPHIR_TYPE_UINTEGER);
  yy_destructor(68,&yymsp[0].minor);
}
// 5663 "parser.c"
        break;
      case 137:
// 1846 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_type(ZEPHIR_TYPE_LONG);
  yy_destructor(69,&yymsp[0].minor);
}
// 5671 "parser.c"
        break;
      case 138:
// 1850 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_type(ZEPHIR_TYPE_ULONG);
  yy_destructor(70,&yymsp[0].minor);
}
// 5679 "parser.c"
        break;
      case 139:
// 1854 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_type(ZEPHIR_TYPE_CHAR);
  yy_destructor(71,&yymsp[0].minor);
}
// 5687 "parser.c"
        break;
      case 140:
// 1858 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_type(ZEPHIR_TYPE_UCHAR);
  yy_destructor(72,&yymsp[0].minor);
}
// 5695 "parser.c"
        break;
      case 141:
// 1862 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_type(ZEPHIR_TYPE_DOUBLE);
  yy_destructor(73,&yymsp[0].minor);
}
// 5703 "parser.c"
        break;
      case 142:
// 1866 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_type(ZEPHIR_TYPE_BOOL);
  yy_destructor(74,&yymsp[0].minor);
}
// 5711 "parser.c"
        break;
      case 143:
// 1870 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_type(ZEPHIR_TYPE_STRING);
  yy_destructor(75,&yymsp[0].minor);
}
// 5719 "parser.c"
        break;
      case 144:
// 1874 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_type(ZEPHIR_TYPE_ARRAY);
  yy_destructor(76,&yymsp[0].minor);
}
// 5727 "parser.c"
        break;
      case 145:
// 1878 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_type(ZEPHIR_TYPE_VAR);
  yy_destructor(77,&yymsp[0].minor);
}
// 5735 "parser.c"
        break;
      case 146:
// 1882 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_type(ZEPHIR_TYPE_CALLABLE);
  yy_destructor(78,&yymsp[0].minor);
}
// 5743 "parser.c"
        break;
      case 147:
// 1886 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_type(ZEPHIR_TYPE_RESOURCE);
  yy_destructor(79,&yymsp[0].minor);
}
// 5751 "parser.c"
        break;
      case 148:
// 1890 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_type(ZEPHIR_TYPE_OBJECT);
  yy_destructor(80,&yymsp[0].minor);
}
// 5759 "parser.c"
        break;
      case 174:
// 1996 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_empty_statement(status->scanner_state);
  yy_destructor(45,&yymsp[0].minor);
}
// 5767 "parser.c"
        break;
      case 175:
// 2000 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_break_statement(status->scanner_state);
  yy_destructor(81,&yymsp[-1].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 5776 "parser.c"
        break;
      case 176:
// 2004 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_continue_statement(status->scanner_state);
  yy_destructor(82,&yymsp[-1].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 5785 "parser.c"
        break;
      case 177:
// 2008 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_if_statement(yymsp[-2].minor.yy180, NULL, NULL, status->scanner_state);
  yy_destructor(83,&yymsp[-3].minor);
  yy_destructor(54,&yymsp[-1].minor);
  yy_destructor(55,&yymsp[0].minor);
}
// 5795 "parser.c"
        break;
      case 178:
// 2012 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_if_statement(yymsp[-5].minor.yy180, NULL, NULL, status->scanner_state);
  yy_destructor(83,&yymsp[-6].minor);
  yy_destructor(54,&yymsp[-4].minor);
  yy_destructor(55,&yymsp[-3].minor);
  yy_destructor(84,&yymsp[-2].minor);
  yy_destructor(54,&yymsp[-1].minor);
  yy_destructor(55,&yymsp[0].minor);
}
// 5808 "parser.c"
        break;
      case 179:
// 2016 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_if_statement(yymsp[-3].minor.yy180, yymsp[-1].minor.yy180, NULL, status->scanner_state);
  yy_destructor(83,&yymsp[-4].minor);
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(55,&yymsp[0].minor);
}
// 5818 "parser.c"
        break;
      case 180:
// 2020 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_if_statement(yymsp[-7].minor.yy180, yymsp[-5].minor.yy180, yymsp[-1].minor.yy180, status->scanner_state);
  yy_destructor(83,&yymsp[-8].minor);
  yy_destructor(54,&yymsp[-6].minor);
  yy_destructor(55,&yymsp[-4].minor);
  yy_destructor(84,&yymsp[-3].minor);
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(55,&yymsp[0].minor);
}
// 5831 "parser.c"
        break;
      case 181:
// 2024 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_if_statement(yymsp[-6].minor.yy180, yymsp[-4].minor.yy180, NULL, status->scanner_state);
  yy_destructor(83,&yymsp[-7].minor);
  yy_destructor(54,&yymsp[-5].minor);
  yy_destructor(55,&yymsp[-3].minor);
  yy_destructor(84,&yymsp[-2].minor);
  yy_destructor(54,&yymsp[-1].minor);
  yy_destructor(55,&yymsp[0].minor);
}
// 5844 "parser.c"
        break;
      case 182:
// 2028 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_if_statement(yymsp[-6].minor.yy180, NULL, yymsp[-1].minor.yy180, status->scanner_state);
  yy_destructor(83,&yymsp[-7].minor);
  yy_destructor(54,&yymsp[-5].minor);
  yy_destructor(55,&yymsp[-4].minor);
  yy_destructor(84,&yymsp[-3].minor);
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(55,&yymsp[0].minor);
}
// 5857 "parser.c"
        break;
      case 183:
// 2032 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_switch_statement(yymsp[-2].minor.yy180, NULL, status->scanner_state);
  yy_destructor(85,&yymsp[-3].minor);
  yy_destructor(54,&yymsp[-1].minor);
  yy_destructor(55,&yymsp[0].minor);
}
// 5867 "parser.c"
        break;
      case 184:
// 2036 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_switch_statement(yymsp[-3].minor.yy180, yymsp[-1].minor.yy180, status->scanner_state);
  yy_destructor(85,&yymsp[-4].minor);
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(55,&yymsp[0].minor);
}
// 5877 "parser.c"
        break;
      case 187:
// 2048 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_case_clause(yymsp[-1].minor.yy180, NULL, status->scanner_state);
  yy_destructor(86,&yymsp[-2].minor);
  yy_destructor(87,&yymsp[0].minor);
}
// 5886 "parser.c"
        break;
      case 188:
// 2052 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_case_clause(yymsp[-2].minor.yy180, yymsp[0].minor.yy180, status->scanner_state);
  yy_destructor(86,&yymsp[-3].minor);
  yy_destructor(87,&yymsp[-1].minor);
}
// 5895 "parser.c"
        break;
      case 189:
// 2056 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_case_clause(NULL, yymsp[0].minor.yy180, status->scanner_state);
  yy_destructor(88,&yymsp[-2].minor);
  yy_destructor(87,&yymsp[-1].minor);
}
// 5904 "parser.c"
        break;
      case 190:
// 2060 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_loop_statement(NULL, status->scanner_state);
  yy_destructor(89,&yymsp[-2].minor);
  yy_destructor(54,&yymsp[-1].minor);
  yy_destructor(55,&yymsp[0].minor);
}
// 5914 "parser.c"
        break;
      case 191:
// 2064 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_loop_statement(yymsp[-1].minor.yy180, status->scanner_state);
  yy_destructor(89,&yymsp[-3].minor);
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(55,&yymsp[0].minor);
}
// 5924 "parser.c"
        break;
      case 192:
// 2068 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_while_statement(yymsp[-2].minor.yy180, NULL, status->scanner_state);
  yy_destructor(90,&yymsp[-3].minor);
  yy_destructor(54,&yymsp[-1].minor);
  yy_destructor(55,&yymsp[0].minor);
}
// 5934 "parser.c"
        break;
      case 193:
// 2072 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_while_statement(yymsp[-3].minor.yy180, yymsp[-1].minor.yy180, status->scanner_state);
  yy_destructor(90,&yymsp[-4].minor);
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(55,&yymsp[0].minor);
}
// 5944 "parser.c"
        break;
      case 194:
// 2076 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_do_while_statement(yymsp[-1].minor.yy180, NULL, status->scanner_state);
  yy_destructor(91,&yymsp[-5].minor);
  yy_destructor(54,&yymsp[-4].minor);
  yy_destructor(55,&yymsp[-3].minor);
  yy_destructor(90,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 5956 "parser.c"
        break;
      case 195:
// 2080 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_do_while_statement(yymsp[-1].minor.yy180, yymsp[-4].minor.yy180, status->scanner_state);
  yy_destructor(91,&yymsp[-6].minor);
  yy_destructor(54,&yymsp[-5].minor);
  yy_destructor(55,&yymsp[-3].minor);
  yy_destructor(90,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 5968 "parser.c"
        break;
      case 196:
// 2084 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_try_catch_statement(NULL, NULL, status->scanner_state);
  yy_destructor(92,&yymsp[-2].minor);
  yy_destructor(54,&yymsp[-1].minor);
  yy_destructor(55,&yymsp[0].minor);
}
// 5978 "parser.c"
        break;
      case 197:
// 2088 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_try_catch_statement(yymsp[-1].minor.yy180, NULL, status->scanner_state);
  yy_destructor(92,&yymsp[-3].minor);
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(55,&yymsp[0].minor);
}
// 5988 "parser.c"
        break;
      case 198:
// 2092 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_try_catch_statement(yymsp[-2].minor.yy180, yymsp[0].minor.yy180, status->scanner_state);
  yy_destructor(92,&yymsp[-4].minor);
  yy_destructor(54,&yymsp[-3].minor);
  yy_destructor(55,&yymsp[-1].minor);
}
// 5998 "parser.c"
        break;
      case 201:
// 2104 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_catch_statement(yymsp[-3].minor.yy180, NULL, yymsp[-1].minor.yy180, status->scanner_state);
  yy_destructor(93,&yymsp[-4].minor);
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(55,&yymsp[0].minor);
}
// 6008 "parser.c"
        break;
      case 202:
// 2108 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_catch_statement(yymsp[-2].minor.yy180, NULL, NULL, status->scanner_state);
  yy_destructor(93,&yymsp[-3].minor);
  yy_destructor(54,&yymsp[-1].minor);
  yy_destructor(55,&yymsp[0].minor);
}
// 6018 "parser.c"
        break;
      case 203:
// 2112 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_catch_statement(yymsp[-4].minor.yy180, zephir_ret_literal(ZEPHIR_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(93,&yymsp[-5].minor);
  yy_destructor(6,&yymsp[-3].minor);
  yy_destructor(54,&yymsp[-1].minor);
  yy_destructor(55,&yymsp[0].minor);
}
// 6029 "parser.c"
        break;
      case 204:
// 2116 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_catch_statement(yymsp[-5].minor.yy180, zephir_ret_literal(ZEPHIR_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), yymsp[-1].minor.yy180, status->scanner_state);
  yy_destructor(93,&yymsp[-6].minor);
  yy_destructor(6,&yymsp[-4].minor);
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(55,&yymsp[0].minor);
}
// 6040 "parser.c"
        break;
      case 208:
// 2132 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_for_statement(yymsp[-3].minor.yy180, NULL, yymsp[-5].minor.yy0, 0, yymsp[-1].minor.yy180, status->scanner_state);
  yy_destructor(94,&yymsp[-6].minor);
  yy_destructor(95,&yymsp[-4].minor);
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(55,&yymsp[0].minor);
}
// 6051 "parser.c"
        break;
      case 209:
// 2136 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_for_statement(yymsp[-2].minor.yy180, NULL, yymsp[-4].minor.yy0, 0, NULL, status->scanner_state);
  yy_destructor(94,&yymsp[-5].minor);
  yy_destructor(95,&yymsp[-3].minor);
  yy_destructor(54,&yymsp[-1].minor);
  yy_destructor(55,&yymsp[0].minor);
}
// 6062 "parser.c"
        break;
      case 210:
// 2140 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_for_statement(yymsp[-3].minor.yy180, NULL, yymsp[-6].minor.yy0, 1, yymsp[-1].minor.yy180, status->scanner_state);
  yy_destructor(94,&yymsp[-7].minor);
  yy_destructor(95,&yymsp[-5].minor);
  yy_destructor(96,&yymsp[-4].minor);
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(55,&yymsp[0].minor);
}
// 6074 "parser.c"
        break;
      case 211:
// 2144 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_for_statement(yymsp[-3].minor.yy180, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, 0, yymsp[-1].minor.yy180, status->scanner_state);
  yy_destructor(94,&yymsp[-8].minor);
  yy_destructor(6,&yymsp[-6].minor);
  yy_destructor(95,&yymsp[-4].minor);
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(55,&yymsp[0].minor);
}
// 6086 "parser.c"
        break;
      case 212:
// 2148 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_for_statement(yymsp[-2].minor.yy180, yymsp[-6].minor.yy0, yymsp[-4].minor.yy0, 0, NULL, status->scanner_state);
  yy_destructor(94,&yymsp[-7].minor);
  yy_destructor(6,&yymsp[-5].minor);
  yy_destructor(95,&yymsp[-3].minor);
  yy_destructor(54,&yymsp[-1].minor);
  yy_destructor(55,&yymsp[0].minor);
}
// 6098 "parser.c"
        break;
      case 213:
// 2152 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_for_statement(yymsp[-3].minor.yy180, yymsp[-8].minor.yy0, yymsp[-6].minor.yy0, 1, yymsp[-1].minor.yy180, status->scanner_state);
  yy_destructor(94,&yymsp[-9].minor);
  yy_destructor(6,&yymsp[-7].minor);
  yy_destructor(95,&yymsp[-5].minor);
  yy_destructor(96,&yymsp[-4].minor);
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(55,&yymsp[0].minor);
}
// 6111 "parser.c"
        break;
      case 214:
// 2156 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_let_statement(yymsp[-1].minor.yy180, status->scanner_state);
  yy_destructor(97,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 6120 "parser.c"
        break;
      case 217:
// 2169 "parser.lemon"
{
	yygotominor.yy180 = zephir_new_string("assign");
  yy_destructor(57,&yymsp[0].minor);
}
// 6128 "parser.c"
        break;
      case 218:
// 2174 "parser.lemon"
{
	yygotominor.yy180 = zephir_new_string("add-assign");
  yy_destructor(98,&yymsp[0].minor);
}
// 6136 "parser.c"
        break;
      case 219:
// 2179 "parser.lemon"
{
	yygotominor.yy180 = zephir_new_string("sub-assign");
  yy_destructor(99,&yymsp[0].minor);
}
// 6144 "parser.c"
        break;
      case 220:
// 2183 "parser.lemon"
{
	yygotominor.yy180 = zephir_new_string("mul-assign");
  yy_destructor(100,&yymsp[0].minor);
}
// 6152 "parser.c"
        break;
      case 221:
// 2187 "parser.lemon"
{
	yygotominor.yy180 = zephir_new_string("div-assign");
  yy_destructor(101,&yymsp[0].minor);
}
// 6160 "parser.c"
        break;
      case 222:
// 2191 "parser.lemon"
{
	yygotominor.yy180 = zephir_new_string("concat-assign");
  yy_destructor(102,&yymsp[0].minor);
}
// 6168 "parser.c"
        break;
      case 223:
// 2195 "parser.lemon"
{
	yygotominor.yy180 = zephir_new_string("mod-assign");
  yy_destructor(103,&yymsp[0].minor);
}
// 6176 "parser.c"
        break;
      case 224:
// 2200 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_let_assignment("variable", yymsp[-1].minor.yy180, yymsp[-2].minor.yy0, NULL, NULL, yymsp[0].minor.yy180, status->scanner_state);
}
// 6183 "parser.c"
        break;
      case 225:
// 2205 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_let_assignment("object-property", yymsp[-1].minor.yy180, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, yymsp[0].minor.yy180, status->scanner_state);
  yy_destructor(42,&yymsp[-3].minor);
}
// 6191 "parser.c"
        break;
      case 226:
// 2210 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_let_assignment("variable-dynamic-object-property", yymsp[-1].minor.yy180, yymsp[-6].minor.yy0, yymsp[-3].minor.yy0, NULL, yymsp[0].minor.yy180, status->scanner_state);
  yy_destructor(42,&yymsp[-5].minor);
  yy_destructor(54,&yymsp[-4].minor);
  yy_destructor(55,&yymsp[-2].minor);
}
// 6201 "parser.c"
        break;
      case 227:
// 2215 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_let_assignment("string-dynamic-object-property", yymsp[-1].minor.yy180, yymsp[-6].minor.yy0, yymsp[-3].minor.yy0, NULL, yymsp[0].minor.yy180, status->scanner_state);
  yy_destructor(42,&yymsp[-5].minor);
  yy_destructor(54,&yymsp[-4].minor);
  yy_destructor(55,&yymsp[-2].minor);
}
// 6211 "parser.c"
        break;
      case 228:
// 2220 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_let_assignment("object-property-append", yymsp[-1].minor.yy180, yymsp[-6].minor.yy0, yymsp[-4].minor.yy0, NULL, yymsp[0].minor.yy180, status->scanner_state);
  yy_destructor(42,&yymsp[-5].minor);
  yy_destructor(41,&yymsp[-3].minor);
  yy_destructor(66,&yymsp[-2].minor);
}
// 6221 "parser.c"
        break;
      case 229:
// 2225 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_let_assignment("object-property-array-index", yymsp[-1].minor.yy180, yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-2].minor.yy180, yymsp[0].minor.yy180, status->scanner_state);
  yy_destructor(42,&yymsp[-4].minor);
}
// 6229 "parser.c"
        break;
      case 230:
// 2229 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_let_assignment("object-property-array-index-append", yymsp[-1].minor.yy180, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, yymsp[-4].minor.yy180, yymsp[0].minor.yy180, status->scanner_state);
  yy_destructor(42,&yymsp[-6].minor);
  yy_destructor(41,&yymsp[-3].minor);
  yy_destructor(66,&yymsp[-2].minor);
}
// 6239 "parser.c"
        break;
      case 231:
// 2234 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_let_assignment("static-property", yymsp[-1].minor.yy180, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, yymsp[0].minor.yy180, status->scanner_state);
  yy_destructor(105,&yymsp[-3].minor);
}
// 6247 "parser.c"
        break;
      case 232:
// 2239 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_let_assignment("static-property-append", yymsp[-1].minor.yy180, yymsp[-6].minor.yy0, yymsp[-4].minor.yy0, NULL, yymsp[0].minor.yy180, status->scanner_state);
  yy_destructor(105,&yymsp[-5].minor);
  yy_destructor(41,&yymsp[-3].minor);
  yy_destructor(66,&yymsp[-2].minor);
}
// 6257 "parser.c"
        break;
      case 233:
// 2244 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_let_assignment("static-property-array-index", yymsp[-1].minor.yy180, yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-2].minor.yy180, yymsp[0].minor.yy180, status->scanner_state);
  yy_destructor(105,&yymsp[-4].minor);
}
// 6265 "parser.c"
        break;
      case 234:
// 2249 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_let_assignment("static-property-array-index-append", yymsp[-1].minor.yy180, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, yymsp[-4].minor.yy180, yymsp[0].minor.yy180, status->scanner_state);
  yy_destructor(105,&yymsp[-6].minor);
  yy_destructor(41,&yymsp[-3].minor);
  yy_destructor(66,&yymsp[-2].minor);
}
// 6275 "parser.c"
        break;
      case 235:
// 2254 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_let_assignment("variable-append", yymsp[-1].minor.yy180, yymsp[-4].minor.yy0, NULL, NULL, yymsp[0].minor.yy180, status->scanner_state);
  yy_destructor(41,&yymsp[-3].minor);
  yy_destructor(66,&yymsp[-2].minor);
}
// 6284 "parser.c"
        break;
      case 236:
// 2259 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_let_assignment("array-index", yymsp[-1].minor.yy180, yymsp[-3].minor.yy0, NULL, yymsp[-2].minor.yy180, yymsp[0].minor.yy180, status->scanner_state);
}
// 6291 "parser.c"
        break;
      case 237:
// 2264 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_let_assignment("array-index-append", yymsp[-1].minor.yy180, yymsp[-5].minor.yy0, NULL, yymsp[-4].minor.yy180, yymsp[0].minor.yy180, status->scanner_state);
  yy_destructor(41,&yymsp[-3].minor);
  yy_destructor(66,&yymsp[-2].minor);
}
// 6300 "parser.c"
        break;
      case 240:
// 2276 "parser.lemon"
{
	yygotominor.yy180 = yymsp[-1].minor.yy180;
  yy_destructor(41,&yymsp[-2].minor);
  yy_destructor(66,&yymsp[0].minor);
}
// 6309 "parser.c"
        break;
      case 241:
// 2281 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_let_assignment("object-property-incr", NULL, yymsp[-3].minor.yy0, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(106,&yymsp[0].minor);
}
// 6318 "parser.c"
        break;
      case 242:
// 2286 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_let_assignment("object-property-decr", NULL, yymsp[-3].minor.yy0, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(107,&yymsp[0].minor);
}
// 6327 "parser.c"
        break;
      case 243:
// 2291 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_let_assignment("incr", NULL, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(106,&yymsp[0].minor);
}
// 6335 "parser.c"
        break;
      case 244:
// 2296 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_let_assignment("decr", NULL, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(107,&yymsp[0].minor);
}
// 6343 "parser.c"
        break;
      case 245:
// 2301 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_let_assignment("dynamic-variable", yymsp[-1].minor.yy180, yymsp[-3].minor.yy0, NULL, NULL, yymsp[0].minor.yy180, status->scanner_state);
  yy_destructor(54,&yymsp[-4].minor);
  yy_destructor(55,&yymsp[-2].minor);
}
// 6352 "parser.c"
        break;
      case 247:
// 2309 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_echo_statement(yymsp[-1].minor.yy180, status->scanner_state);
  yy_destructor(108,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 6361 "parser.c"
        break;
      case 250:
// 2321 "parser.lemon"
{
	yygotominor.yy180 = yymsp[0].minor.yy180;;
}
// 6368 "parser.c"
        break;
      case 251:
// 2326 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_mcall_statement(yymsp[-1].minor.yy180, status->scanner_state);
  yy_destructor(45,&yymsp[0].minor);
}
// 6376 "parser.c"
        break;
      case 252:
// 2331 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_fcall_statement(yymsp[-1].minor.yy180, status->scanner_state);
  yy_destructor(45,&yymsp[0].minor);
}
// 6384 "parser.c"
        break;
      case 253:
// 2336 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_scall_statement(yymsp[-1].minor.yy180, status->scanner_state);
  yy_destructor(45,&yymsp[0].minor);
}
// 6392 "parser.c"
        break;
      case 254:
// 2341 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_fetch_statement(yymsp[-1].minor.yy180, status->scanner_state);
  yy_destructor(45,&yymsp[0].minor);
}
// 6400 "parser.c"
        break;
      case 255:
// 2346 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_return_statement(yymsp[-1].minor.yy180, status->scanner_state);
  yy_destructor(109,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 6409 "parser.c"
        break;
      case 256:
// 2351 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_return_statement(NULL, status->scanner_state);
  yy_destructor(109,&yymsp[-1].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 6418 "parser.c"
        break;
      case 257:
// 2356 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_require_statement(yymsp[-1].minor.yy180, status->scanner_state);
  yy_destructor(7,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 6427 "parser.c"
        break;
      case 258:
// 2361 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_unset_statement(yymsp[-1].minor.yy180, status->scanner_state);
  yy_destructor(110,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 6436 "parser.c"
        break;
      case 259:
// 2366 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_throw_exception(yymsp[-1].minor.yy180, status->scanner_state);
  yy_destructor(111,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 6445 "parser.c"
        break;
      case 260:
// 2370 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_declare_statement(ZEPHIR_T_TYPE_INTEGER, yymsp[-1].minor.yy180, status->scanner_state);
  yy_destructor(67,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 6454 "parser.c"
        break;
      case 261:
// 2374 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_declare_statement(ZEPHIR_T_TYPE_UINTEGER, yymsp[-1].minor.yy180, status->scanner_state);
  yy_destructor(68,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 6463 "parser.c"
        break;
      case 262:
// 2378 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_declare_statement(ZEPHIR_T_TYPE_CHAR, yymsp[-1].minor.yy180, status->scanner_state);
  yy_destructor(71,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 6472 "parser.c"
        break;
      case 263:
// 2382 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_declare_statement(ZEPHIR_T_TYPE_UCHAR, yymsp[-1].minor.yy180, status->scanner_state);
  yy_destructor(72,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 6481 "parser.c"
        break;
      case 264:
// 2386 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_declare_statement(ZEPHIR_T_TYPE_LONG, yymsp[-1].minor.yy180, status->scanner_state);
  yy_destructor(69,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 6490 "parser.c"
        break;
      case 265:
// 2390 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_declare_statement(ZEPHIR_T_TYPE_ULONG, yymsp[-1].minor.yy180, status->scanner_state);
  yy_destructor(70,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 6499 "parser.c"
        break;
      case 266:
// 2394 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_declare_statement(ZEPHIR_T_TYPE_DOUBLE, yymsp[-1].minor.yy180, status->scanner_state);
  yy_destructor(73,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 6508 "parser.c"
        break;
      case 267:
// 2398 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_declare_statement(ZEPHIR_T_TYPE_STRING, yymsp[-1].minor.yy180, status->scanner_state);
  yy_destructor(75,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 6517 "parser.c"
        break;
      case 268:
// 2402 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_declare_statement(ZEPHIR_T_TYPE_BOOL, yymsp[-1].minor.yy180, status->scanner_state);
  yy_destructor(74,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 6526 "parser.c"
        break;
      case 269:
// 2406 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_declare_statement(ZEPHIR_T_TYPE_VAR, yymsp[-1].minor.yy180, status->scanner_state);
  yy_destructor(77,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 6535 "parser.c"
        break;
      case 270:
// 2410 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_declare_statement(ZEPHIR_T_TYPE_ARRAY, yymsp[-1].minor.yy180, status->scanner_state);
  yy_destructor(76,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 6544 "parser.c"
        break;
      case 273:
// 2422 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_declare_variable(yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 6551 "parser.c"
        break;
      case 274:
// 2426 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_declare_variable(yymsp[-2].minor.yy0, yymsp[0].minor.yy180, status->scanner_state);
  yy_destructor(57,&yymsp[-1].minor);
}
// 6559 "parser.c"
        break;
      case 276:
// 2434 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_expr("not", yymsp[0].minor.yy180, NULL, NULL, status->scanner_state);
  yy_destructor(39,&yymsp[-1].minor);
}
// 6567 "parser.c"
        break;
      case 277:
// 2438 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_expr("minus", yymsp[0].minor.yy180, NULL, NULL, status->scanner_state);
  yy_destructor(28,&yymsp[-1].minor);
}
// 6575 "parser.c"
        break;
      case 278:
// 2442 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_expr("isset", yymsp[0].minor.yy180, NULL, NULL, status->scanner_state);
  yy_destructor(33,&yymsp[-1].minor);
}
// 6583 "parser.c"
        break;
      case 279:
// 2446 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_expr("require", yymsp[0].minor.yy180, NULL, NULL, status->scanner_state);
  yy_destructor(7,&yymsp[-1].minor);
}
// 6591 "parser.c"
        break;
      case 280:
// 2450 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_expr("clone", yymsp[0].minor.yy180, NULL, NULL, status->scanner_state);
  yy_destructor(37,&yymsp[-1].minor);
}
// 6599 "parser.c"
        break;
      case 281:
// 2454 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_expr("empty", yymsp[0].minor.yy180, NULL, NULL, status->scanner_state);
  yy_destructor(35,&yymsp[-1].minor);
}
// 6607 "parser.c"
        break;
      case 282:
// 2458 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_expr("likely", yymsp[0].minor.yy180, NULL, NULL, status->scanner_state);
  yy_destructor(9,&yymsp[-1].minor);
}
// 6615 "parser.c"
        break;
      case 283:
// 2462 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_expr("unlikely", yymsp[0].minor.yy180, NULL, NULL, status->scanner_state);
  yy_destructor(10,&yymsp[-1].minor);
}
// 6623 "parser.c"
        break;
      case 284:
// 2466 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_expr("equals", yymsp[-2].minor.yy180, yymsp[0].minor.yy180, NULL, status->scanner_state);
  yy_destructor(19,&yymsp[-1].minor);
}
// 6631 "parser.c"
        break;
      case 285:
// 2470 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_expr("not-equals", yymsp[-2].minor.yy180, yymsp[0].minor.yy180, NULL, status->scanner_state);
  yy_destructor(26,&yymsp[-1].minor);
}
// 6639 "parser.c"
        break;
      case 286:
// 2474 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_expr("identical", yymsp[-2].minor.yy180, yymsp[0].minor.yy180, NULL, status->scanner_state);
  yy_destructor(20,&yymsp[-1].minor);
}
// 6647 "parser.c"
        break;
      case 287:
// 2478 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_expr("not-identical", yymsp[-2].minor.yy180, yymsp[0].minor.yy180, NULL, status->scanner_state);
  yy_destructor(25,&yymsp[-1].minor);
}
// 6655 "parser.c"
        break;
      case 288:
// 2482 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_expr("less", yymsp[-2].minor.yy180, yymsp[0].minor.yy180, NULL, status->scanner_state);
  yy_destructor(21,&yymsp[-1].minor);
}
// 6663 "parser.c"
        break;
      case 289:
// 2486 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_expr("greater", yymsp[-2].minor.yy180, yymsp[0].minor.yy180, NULL, status->scanner_state);
  yy_destructor(22,&yymsp[-1].minor);
}
// 6671 "parser.c"
        break;
      case 290:
// 2490 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_expr("less-equal", yymsp[-2].minor.yy180, yymsp[0].minor.yy180, NULL, status->scanner_state);
  yy_destructor(23,&yymsp[-1].minor);
}
// 6679 "parser.c"
        break;
      case 291:
// 2494 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_expr("greater-equal", yymsp[-2].minor.yy180, yymsp[0].minor.yy180, NULL, status->scanner_state);
  yy_destructor(24,&yymsp[-1].minor);
}
// 6687 "parser.c"
        break;
      case 292:
// 2498 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_expr("list", yymsp[-1].minor.yy180, NULL, NULL, status->scanner_state);
  yy_destructor(61,&yymsp[-2].minor);
  yy_destructor(40,&yymsp[0].minor);
}
// 6696 "parser.c"
        break;
      case 293:
// 2502 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_expr("cast", yymsp[-2].minor.yy180, yymsp[0].minor.yy180, NULL, status->scanner_state);
  yy_destructor(61,&yymsp[-3].minor);
  yy_destructor(40,&yymsp[-1].minor);
}
// 6705 "parser.c"
        break;
      case 294:
// 2506 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_expr("type-hint", zephir_ret_literal(ZEPHIR_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), yymsp[0].minor.yy180, NULL, status->scanner_state);
  yy_destructor(21,&yymsp[-3].minor);
  yy_destructor(22,&yymsp[-1].minor);
}
// 6714 "parser.c"
        break;
      case 295:
// 2510 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_expr("property-access", yymsp[-2].minor.yy180, zephir_ret_literal(ZEPHIR_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(42,&yymsp[-1].minor);
}
// 6722 "parser.c"
        break;
      case 296:
// 2514 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_expr("property-dynamic-access", yymsp[-4].minor.yy180, zephir_ret_literal(ZEPHIR_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(42,&yymsp[-3].minor);
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(55,&yymsp[0].minor);
}
// 6732 "parser.c"
        break;
      case 297:
// 2518 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_expr("property-string-access", yymsp[-4].minor.yy180, zephir_ret_literal(ZEPHIR_T_STRING, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(42,&yymsp[-3].minor);
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(55,&yymsp[0].minor);
}
// 6742 "parser.c"
        break;
      case 298:
// 2522 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_expr("static-property-access", zephir_ret_literal(ZEPHIR_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), zephir_ret_literal(ZEPHIR_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(105,&yymsp[-1].minor);
}
// 6750 "parser.c"
        break;
      case 299:
      case 374:
// 2526 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_expr("static-constant-access", zephir_ret_literal(ZEPHIR_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), zephir_ret_literal(ZEPHIR_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(105,&yymsp[-1].minor);
}
// 6759 "parser.c"
        break;
      case 300:
// 2535 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_expr("array-access", yymsp[-3].minor.yy180, yymsp[-1].minor.yy180, NULL, status->scanner_state);
  yy_destructor(41,&yymsp[-2].minor);
  yy_destructor(66,&yymsp[0].minor);
}
// 6768 "parser.c"
        break;
      case 301:
// 2540 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_expr("add", yymsp[-2].minor.yy180, yymsp[0].minor.yy180, NULL, status->scanner_state);
  yy_destructor(27,&yymsp[-1].minor);
}
// 6776 "parser.c"
        break;
      case 302:
// 2545 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_expr("sub", yymsp[-2].minor.yy180, yymsp[0].minor.yy180, NULL, status->scanner_state);
  yy_destructor(28,&yymsp[-1].minor);
}
// 6784 "parser.c"
        break;
      case 303:
// 2550 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_expr("mul", yymsp[-2].minor.yy180, yymsp[0].minor.yy180, NULL, status->scanner_state);
  yy_destructor(30,&yymsp[-1].minor);
}
// 6792 "parser.c"
        break;
      case 304:
// 2555 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_expr("div", yymsp[-2].minor.yy180, yymsp[0].minor.yy180, NULL, status->scanner_state);
  yy_destructor(31,&yymsp[-1].minor);
}
// 6800 "parser.c"
        break;
      case 305:
// 2560 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_expr("mod", yymsp[-2].minor.yy180, yymsp[0].minor.yy180, NULL, status->scanner_state);
  yy_destructor(32,&yymsp[-1].minor);
}
// 6808 "parser.c"
        break;
      case 306:
// 2565 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_expr("concat", yymsp[-2].minor.yy180, yymsp[0].minor.yy180, NULL, status->scanner_state);
  yy_destructor(29,&yymsp[-1].minor);
}
// 6816 "parser.c"
        break;
      case 307:
// 2570 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_expr("and", yymsp[-2].minor.yy180, yymsp[0].minor.yy180, NULL, status->scanner_state);
  yy_destructor(13,&yymsp[-1].minor);
}
// 6824 "parser.c"
        break;
      case 308:
// 2575 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_expr("or", yymsp[-2].minor.yy180, yymsp[0].minor.yy180, NULL, status->scanner_state);
  yy_destructor(12,&yymsp[-1].minor);
}
// 6832 "parser.c"
        break;
      case 309:
// 2580 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_expr("bitwise_and", yymsp[-2].minor.yy180, yymsp[0].minor.yy180, NULL, status->scanner_state);
  yy_destructor(15,&yymsp[-1].minor);
}
// 6840 "parser.c"
        break;
      case 310:
// 2585 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_expr("bitwise_or", yymsp[-2].minor.yy180, yymsp[0].minor.yy180, NULL, status->scanner_state);
  yy_destructor(14,&yymsp[-1].minor);
}
// 6848 "parser.c"
        break;
      case 311:
// 2590 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_expr("bitwise_xor", yymsp[-2].minor.yy180, yymsp[0].minor.yy180, NULL, status->scanner_state);
  yy_destructor(16,&yymsp[-1].minor);
}
// 6856 "parser.c"
        break;
      case 312:
// 2595 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_expr("bitwise_shiftleft", yymsp[-2].minor.yy180, yymsp[0].minor.yy180, NULL, status->scanner_state);
  yy_destructor(17,&yymsp[-1].minor);
}
// 6864 "parser.c"
        break;
      case 313:
// 2600 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_expr("bitwise_shiftright", yymsp[-2].minor.yy180, yymsp[0].minor.yy180, NULL, status->scanner_state);
  yy_destructor(18,&yymsp[-1].minor);
}
// 6872 "parser.c"
        break;
      case 314:
// 2605 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_expr("instanceof", yymsp[-2].minor.yy180, yymsp[0].minor.yy180, NULL, status->scanner_state);
  yy_destructor(11,&yymsp[-1].minor);
}
// 6880 "parser.c"
        break;
      case 315:
// 2610 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_expr("fetch", zephir_ret_literal(ZEPHIR_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), yymsp[0].minor.yy180, NULL, status->scanner_state);
  yy_destructor(34,&yymsp[-3].minor);
  yy_destructor(6,&yymsp[-1].minor);
}
// 6889 "parser.c"
        break;
      case 317:
// 2620 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_expr("typeof", yymsp[0].minor.yy180, NULL, NULL, status->scanner_state);
  yy_destructor(36,&yymsp[-1].minor);
}
// 6897 "parser.c"
        break;
      case 319:
      case 365:
      case 367:
      case 384:
// 2630 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_literal(ZEPHIR_T_INTEGER, yymsp[0].minor.yy0, status->scanner_state);
}
// 6907 "parser.c"
        break;
      case 320:
      case 364:
      case 369:
      case 383:
// 2635 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_literal(ZEPHIR_T_STRING, yymsp[0].minor.yy0, status->scanner_state);
}
// 6917 "parser.c"
        break;
      case 321:
      case 368:
// 2640 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_literal(ZEPHIR_T_CHAR, yymsp[0].minor.yy0, status->scanner_state);
}
// 6925 "parser.c"
        break;
      case 322:
      case 370:
// 2645 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_literal(ZEPHIR_T_DOUBLE, yymsp[0].minor.yy0, status->scanner_state);
}
// 6933 "parser.c"
        break;
      case 323:
      case 371:
// 2650 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_literal(ZEPHIR_T_NULL, NULL, status->scanner_state);
  yy_destructor(64,&yymsp[0].minor);
}
// 6942 "parser.c"
        break;
      case 324:
      case 373:
// 2655 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_literal(ZEPHIR_T_TRUE, NULL, status->scanner_state);
  yy_destructor(115,&yymsp[0].minor);
}
// 6951 "parser.c"
        break;
      case 325:
      case 372:
// 2660 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_literal(ZEPHIR_T_FALSE, NULL, status->scanner_state);
  yy_destructor(116,&yymsp[0].minor);
}
// 6960 "parser.c"
        break;
      case 326:
      case 375:
// 2665 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_literal(ZEPHIR_T_CONSTANT, yymsp[0].minor.yy0, status->scanner_state);
}
// 6968 "parser.c"
        break;
      case 327:
      case 376:
// 2670 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_expr("empty-array", NULL, NULL, NULL, status->scanner_state);
  yy_destructor(41,&yymsp[-1].minor);
  yy_destructor(66,&yymsp[0].minor);
}
// 6978 "parser.c"
        break;
      case 328:
      case 377:
// 2675 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_expr("array", yymsp[-1].minor.yy180, NULL, NULL, status->scanner_state);
  yy_destructor(41,&yymsp[-2].minor);
  yy_destructor(66,&yymsp[0].minor);
}
// 6988 "parser.c"
        break;
      case 329:
// 2680 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_new_instance(0, yymsp[0].minor.yy0, NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-1].minor);
}
// 6996 "parser.c"
        break;
      case 330:
// 2685 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_new_instance(0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-3].minor);
  yy_destructor(61,&yymsp[-1].minor);
  yy_destructor(40,&yymsp[0].minor);
}
// 7006 "parser.c"
        break;
      case 331:
// 2690 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_new_instance(0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy180, status->scanner_state);
  yy_destructor(38,&yymsp[-4].minor);
  yy_destructor(61,&yymsp[-2].minor);
  yy_destructor(40,&yymsp[0].minor);
}
// 7016 "parser.c"
        break;
      case 332:
// 2695 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_new_instance(1, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-3].minor);
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(55,&yymsp[0].minor);
}
// 7026 "parser.c"
        break;
      case 333:
// 2700 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_new_instance(1, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-5].minor);
  yy_destructor(54,&yymsp[-4].minor);
  yy_destructor(55,&yymsp[-2].minor);
  yy_destructor(61,&yymsp[-1].minor);
  yy_destructor(40,&yymsp[0].minor);
}
// 7038 "parser.c"
        break;
      case 334:
// 2705 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_new_instance(1, yymsp[-4].minor.yy0, yymsp[-1].minor.yy180, status->scanner_state);
  yy_destructor(38,&yymsp[-6].minor);
  yy_destructor(54,&yymsp[-5].minor);
  yy_destructor(55,&yymsp[-3].minor);
  yy_destructor(61,&yymsp[-2].minor);
  yy_destructor(40,&yymsp[0].minor);
}
// 7050 "parser.c"
        break;
      case 335:
// 2710 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_fcall(1, yymsp[-3].minor.yy0, yymsp[-1].minor.yy180, status->scanner_state);
  yy_destructor(61,&yymsp[-2].minor);
  yy_destructor(40,&yymsp[0].minor);
}
// 7059 "parser.c"
        break;
      case 336:
// 2715 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_fcall(1, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(61,&yymsp[-1].minor);
  yy_destructor(40,&yymsp[0].minor);
}
// 7068 "parser.c"
        break;
      case 337:
// 2720 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_fcall(2, yymsp[-4].minor.yy0, yymsp[-1].minor.yy180, status->scanner_state);
  yy_destructor(54,&yymsp[-5].minor);
  yy_destructor(55,&yymsp[-3].minor);
  yy_destructor(61,&yymsp[-2].minor);
  yy_destructor(40,&yymsp[0].minor);
}
// 7079 "parser.c"
        break;
      case 338:
// 2725 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_fcall(2, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(54,&yymsp[-4].minor);
  yy_destructor(55,&yymsp[-2].minor);
  yy_destructor(61,&yymsp[-1].minor);
  yy_destructor(40,&yymsp[0].minor);
}
// 7090 "parser.c"
        break;
      case 339:
// 2730 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_scall(0, yymsp[-5].minor.yy0, 0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy180, status->scanner_state);
  yy_destructor(105,&yymsp[-4].minor);
  yy_destructor(61,&yymsp[-2].minor);
  yy_destructor(40,&yymsp[0].minor);
}
// 7100 "parser.c"
        break;
      case 340:
// 2735 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_scall(0, yymsp[-4].minor.yy0, 0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(105,&yymsp[-3].minor);
  yy_destructor(61,&yymsp[-1].minor);
  yy_destructor(40,&yymsp[0].minor);
}
// 7110 "parser.c"
        break;
      case 341:
// 2740 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_scall(1, yymsp[-6].minor.yy0, 0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy180, status->scanner_state);
  yy_destructor(54,&yymsp[-7].minor);
  yy_destructor(55,&yymsp[-5].minor);
  yy_destructor(105,&yymsp[-4].minor);
  yy_destructor(61,&yymsp[-2].minor);
  yy_destructor(40,&yymsp[0].minor);
}
// 7122 "parser.c"
        break;
      case 342:
// 2745 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_scall(1, yymsp[-5].minor.yy0, 0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(54,&yymsp[-6].minor);
  yy_destructor(55,&yymsp[-4].minor);
  yy_destructor(105,&yymsp[-3].minor);
  yy_destructor(61,&yymsp[-1].minor);
  yy_destructor(40,&yymsp[0].minor);
}
// 7134 "parser.c"
        break;
      case 343:
// 2750 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_scall(1, yymsp[-8].minor.yy0, 1, yymsp[-4].minor.yy0, yymsp[-1].minor.yy180, status->scanner_state);
  yy_destructor(54,&yymsp[-9].minor);
  yy_destructor(55,&yymsp[-7].minor);
  yy_destructor(105,&yymsp[-6].minor);
  yy_destructor(54,&yymsp[-5].minor);
  yy_destructor(55,&yymsp[-3].minor);
  yy_destructor(61,&yymsp[-2].minor);
  yy_destructor(40,&yymsp[0].minor);
}
// 7148 "parser.c"
        break;
      case 344:
// 2755 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_scall(1, yymsp[-7].minor.yy0, 1, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(54,&yymsp[-8].minor);
  yy_destructor(55,&yymsp[-6].minor);
  yy_destructor(105,&yymsp[-5].minor);
  yy_destructor(54,&yymsp[-4].minor);
  yy_destructor(55,&yymsp[-2].minor);
  yy_destructor(61,&yymsp[-1].minor);
  yy_destructor(40,&yymsp[0].minor);
}
// 7162 "parser.c"
        break;
      case 345:
// 2760 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_mcall(1, yymsp[-5].minor.yy180, yymsp[-3].minor.yy0, yymsp[-1].minor.yy180, status->scanner_state);
  yy_destructor(42,&yymsp[-4].minor);
  yy_destructor(61,&yymsp[-2].minor);
  yy_destructor(40,&yymsp[0].minor);
}
// 7172 "parser.c"
        break;
      case 346:
// 2765 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_mcall(1, yymsp[-4].minor.yy180, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(42,&yymsp[-3].minor);
  yy_destructor(61,&yymsp[-1].minor);
  yy_destructor(40,&yymsp[0].minor);
}
// 7182 "parser.c"
        break;
      case 347:
// 2770 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_mcall(2, yymsp[-7].minor.yy180, yymsp[-4].minor.yy0, yymsp[-1].minor.yy180, status->scanner_state);
  yy_destructor(42,&yymsp[-6].minor);
  yy_destructor(54,&yymsp[-5].minor);
  yy_destructor(55,&yymsp[-3].minor);
  yy_destructor(61,&yymsp[-2].minor);
  yy_destructor(40,&yymsp[0].minor);
}
// 7194 "parser.c"
        break;
      case 348:
// 2775 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_mcall(2, yymsp[-6].minor.yy180, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(42,&yymsp[-5].minor);
  yy_destructor(54,&yymsp[-4].minor);
  yy_destructor(55,&yymsp[-2].minor);
  yy_destructor(61,&yymsp[-1].minor);
  yy_destructor(40,&yymsp[0].minor);
}
// 7206 "parser.c"
        break;
      case 349:
// 2780 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_mcall(3, yymsp[-7].minor.yy180, yymsp[-4].minor.yy0, yymsp[-1].minor.yy180, status->scanner_state);
  yy_destructor(42,&yymsp[-6].minor);
  yy_destructor(54,&yymsp[-5].minor);
  yy_destructor(55,&yymsp[-3].minor);
  yy_destructor(61,&yymsp[-2].minor);
  yy_destructor(40,&yymsp[0].minor);
}
// 7218 "parser.c"
        break;
      case 350:
// 2785 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_mcall(3, yymsp[-6].minor.yy180, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(42,&yymsp[-5].minor);
  yy_destructor(54,&yymsp[-4].minor);
  yy_destructor(55,&yymsp[-2].minor);
  yy_destructor(61,&yymsp[-1].minor);
  yy_destructor(40,&yymsp[0].minor);
}
// 7230 "parser.c"
        break;
      case 354:
// 2805 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_expr("ternary", yymsp[-4].minor.yy180, yymsp[-2].minor.yy180, yymsp[0].minor.yy180, status->scanner_state);
  yy_destructor(8,&yymsp[-3].minor);
  yy_destructor(87,&yymsp[-1].minor);
}
// 7239 "parser.c"
        break;
      case 357:
// 2817 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_call_parameter(NULL, yymsp[0].minor.yy180, status->scanner_state);
}
// 7246 "parser.c"
        break;
      case 358:
// 2821 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_call_parameter(yymsp[-2].minor.yy0, yymsp[0].minor.yy180, status->scanner_state);
  yy_destructor(87,&yymsp[-1].minor);
}
// 7254 "parser.c"
        break;
      case 361:
      case 380:
// 2833 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_array_item(yymsp[-2].minor.yy180, yymsp[0].minor.yy180, status->scanner_state);
  yy_destructor(87,&yymsp[-1].minor);
}
// 7263 "parser.c"
        break;
      case 362:
      case 381:
// 2837 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_array_item(NULL, yymsp[0].minor.yy180, status->scanner_state);
}
// 7271 "parser.c"
        break;
      case 387:
// 2938 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_comment(yymsp[0].minor.yy0, status->scanner_state);
}
// 7278 "parser.c"
        break;
      case 388:
// 2942 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_cblock(yymsp[0].minor.yy0, status->scanner_state);
}
// 7285 "parser.c"
        break;
  };
  yygoto = yyRuleInfo[yyruleno].lhs;
  yysize = yyRuleInfo[yyruleno].nrhs;
  yypParser->yyidx -= yysize;
  yyact = yy_find_reduce_action(yypParser,yygoto);
  if( yyact < YYNSTATE ){
    yy_shift(yypParser,yyact,yygoto,&yygotominor);
  }else if( yyact == YYNSTATE + YYNRULE + 1 ){
    yy_accept(yypParser);
  }
}

/*
** The following code executes when the parse fails
*/
static void yy_parse_failed(
  yyParser *yypParser           /* The parser */
){
  zephir_ARG_FETCH;
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sFail!\n",yyTracePrompt);
  }
#endif
  while( yypParser->yyidx>=0 ) yy_pop_parser_stack(yypParser);
  /* Here code is inserted which will be executed whenever the
  ** parser fails */
  zephir_ARG_STORE; /* Suppress warning about unused %extra_argument variable */
}

/*
** The following code executes when a syntax error first occurs.
*/
static void yy_syntax_error(
  yyParser *yypParser,           /* The parser */
  int yymajor,                   /* The major type of the error token */
  YYMINORTYPE yyminor            /* The minor type of the error token */
){
  zephir_ARG_FETCH;
#define TOKEN (yyminor.yy0)
// 1245 "parser.lemon"


	//fprintf(stderr, "error!\n");

	//zval *syntax_error = json_object_new_object();

	//zephir_array_add(syntax_error, "type", zephir_new_string("error"));

	if (status->scanner_state->start_length) {
		fprintf(stderr, "Syntax error, %s", status->scanner_state->start);
	} else {
		fprintf(stderr, "EOF");
	}

	//status->syntax_error_len = 48 + Z_STRLEN_P(status->scanner_state->active_file);
	//status->syntax_error = emalloc(sizeof(char) * status->syntax_error_len);

	if (status->scanner_state->start_length) {
		//zephir_array_add(syntax_error, "message", zephir_new_string("Syntax error"));
	} else {
		//zephir_array_add(syntax_error, "message", zephir_new_string("Unexpected EOF"));
	}

	//zephir_array_add(syntax_error, "file", zephir_new_string(status->scanner_state->active_file));
	//zephir_array_add(syntax_error, "line", zephir_new_int(status->scanner_state->active_line));
	//zephir_array_add(syntax_error, "char", zephir_new_int(status->scanner_state->active_char));

	status->status = ZEPHIR_PARSING_FAILED;
	//status->ret = syntax_error;

	//status->scanner_state->active_file

// 7360 "parser.c"
  zephir_ARG_STORE; /* Suppress warning about unused %extra_argument variable */
}

/*
** The following is executed when the parser accepts
*/
static void yy_accept(
  yyParser *yypParser           /* The parser */
){
  zephir_ARG_FETCH;
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sAccept!\n",yyTracePrompt);
  }
#endif
  while( yypParser->yyidx>=0 ) yy_pop_parser_stack(yypParser);
  /* Here code is inserted which will be executed whenever the
  ** parser accepts */
  zephir_ARG_STORE; /* Suppress warning about unused %extra_argument variable */
}

/* The main parser program.
** The first argument is a pointer to a structure obtained from
** "zephir_Alloc" which describes the current state of the parser.
** The second argument is the major token number.  The third is
** the minor token.  The fourth optional argument is whatever the
** user wants (and specified in the grammar) and is available for
** use by the action routines.
**
** Inputs:
** <ul>
** <li> A pointer to the parser (an opaque structure.)
** <li> The major token number.
** <li> The minor token number.
** <li> An option argument of a grammar-specified type.
** </ul>
**
** Outputs:
** None.
*/
void zephir_(
  void *yyp,                   /* The parser */
  int yymajor,                 /* The major token code number */
  zephir_TOKENTYPE yyminor       /* The value for the token */
  zephir_ARG_PDECL               /* Optional %extra_argument parameter */
){
  YYMINORTYPE yyminorunion;
  int yyact;            /* The parser action. */
  int yyendofinput;     /* True if we are at the end of input */
  int yyerrorhit = 0;   /* True if yymajor has invoked an error */
  yyParser *yypParser;  /* The parser */

  /* (re)initialize the parser, if necessary */
  yypParser = (yyParser*)yyp;
  if( yypParser->yyidx<0 ){
    if( yymajor==0 ) return;
    yypParser->yyidx = 0;
    yypParser->yyerrcnt = -1;
    yypParser->yystack[0].stateno = 0;
    yypParser->yystack[0].major = 0;
  }
  yyminorunion.yy0 = yyminor;
  yyendofinput = (yymajor==0);
  zephir_ARG_STORE;

#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sInput %s\n",yyTracePrompt,yyTokenName[yymajor]);
  }
#endif

  do{
    yyact = yy_find_shift_action(yypParser,yymajor);
    if( yyact<YYNSTATE ){
      yy_shift(yypParser,yyact,yymajor,&yyminorunion);
      yypParser->yyerrcnt--;
      if( yyendofinput && yypParser->yyidx>=0 ){
        yymajor = 0;
      }else{
        yymajor = YYNOCODE;
      }
    }else if( yyact < YYNSTATE + YYNRULE ){
      yy_reduce(yypParser,yyact-YYNSTATE);
    }else if( yyact == YY_ERROR_ACTION ){
      int yymx;
#ifndef NDEBUG
      if( yyTraceFILE ){
        fprintf(yyTraceFILE,"%sSyntax Error!\n",yyTracePrompt);
      }
#endif
#ifdef YYERRORSYMBOL
      /* A syntax error has occurred.
      ** The response to an error depends upon whether or not the
      ** grammar defines an error token "ERROR".  
      **
      ** This is what we do if the grammar does define ERROR:
      **
      **  * Call the %syntax_error function.
      **
      **  * Begin popping the stack until we enter a state where
      **    it is legal to shift the error symbol, then shift
      **    the error symbol.
      **
      **  * Set the error count to three.
      **
      **  * Begin accepting and shifting new tokens.  No new error
      **    processing will occur until three tokens have been
      **    shifted successfully.
      **
      */
      if( yypParser->yyerrcnt<0 ){
        yy_syntax_error(yypParser,yymajor,yyminorunion);
      }
      yymx = yypParser->yystack[yypParser->yyidx].major;
      if( yymx==YYERRORSYMBOL || yyerrorhit ){
#ifndef NDEBUG
        if( yyTraceFILE ){
          fprintf(yyTraceFILE,"%sDiscard input token %s\n",
             yyTracePrompt,yyTokenName[yymajor]);
        }
#endif
        yy_destructor(yymajor,&yyminorunion);
        yymajor = YYNOCODE;
      }else{
         while(
          yypParser->yyidx >= 0 &&
          yymx != YYERRORSYMBOL &&
          (yyact = yy_find_shift_action(yypParser,YYERRORSYMBOL)) >= YYNSTATE
        ){
          yy_pop_parser_stack(yypParser);
        }
        if( yypParser->yyidx < 0 || yymajor==0 ){
          yy_destructor(yymajor,&yyminorunion);
          yy_parse_failed(yypParser);
          yymajor = YYNOCODE;
        }else if( yymx!=YYERRORSYMBOL ){
          YYMINORTYPE u2;
          u2.YYERRSYMDT = 0;
          yy_shift(yypParser,yyact,YYERRORSYMBOL,&u2);
        }
      }
      yypParser->yyerrcnt = 3;
      yyerrorhit = 1;
#else  /* YYERRORSYMBOL is not defined */
      /* This is what we do if the grammar does not define ERROR:
      **
      **  * Report an error message, and throw away the input token.
      **
      **  * If the input token is $, then fail the parse.
      **
      ** As before, subsequent error messages are suppressed until
      ** three input tokens have been successfully shifted.
      */
      if( yypParser->yyerrcnt<=0 ){
        yy_syntax_error(yypParser,yymajor,yyminorunion);
      }
      yypParser->yyerrcnt = 3;
      yy_destructor(yymajor,&yyminorunion);
      if( yyendofinput ){
        yy_parse_failed(yypParser);
      }
      yymajor = YYNOCODE;
#endif
    }else{
      yy_accept(yypParser);
      yymajor = YYNOCODE;
    }
  }while( yymajor!=YYNOCODE && yypParser->yyidx>=0 );
  return;
}

/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013-2014 Zephir Team and contributors                          |
 +--------------------------------------------------------------------------+
 | This source file is subject the MIT license, that is bundled with        |
 | this package in the file LICENSE, and is available through the           |
 | world-wide-web at the following url:                                     |
 | http://zephir-lang.com/license.html                                      |
 |                                                                          |
 | If you did not receive a copy of the MIT license and are unable          |
 | to obtain it through the world-wide-web, please send a note to           |
 | license@zephir-lang.com so we can mail you a copy immediately.           |
 +--------------------------------------------------------------------------+
*/

const zephir_token_names zephir_tokens[] =
{
	{ ZEPHIR_T_INTEGER,             "INTEGER" },
	{ ZEPHIR_T_DOUBLE,              "DOUBLE" },
	{ ZEPHIR_T_STRING,              "STRING" },
	{ ZEPHIR_T_IDENTIFIER,    		  "IDENTIFIER" },
	{ ZEPHIR_T_AT,                  "@" },
	{ ZEPHIR_T_COMMA,               "," },
	{ ZEPHIR_T_ASSIGN,              "=" },
	{ ZEPHIR_T_COLON,               ":" },
	{ ZEPHIR_T_PARENTHESES_OPEN,    "(" },
	{ ZEPHIR_T_PARENTHESES_CLOSE,   ")" },
	{ ZEPHIR_T_BRACKET_OPEN,        "{" },
	{ ZEPHIR_T_BRACKET_CLOSE,       "}" },
  { ZEPHIR_T_SBRACKET_OPEN,       "[" },
	{ ZEPHIR_T_SBRACKET_CLOSE,      "]" },
	{  0, NULL }
};

/**
 * Wrapper to alloc memory within the parser
 */
static void *zephir_wrapper_alloc(size_t bytes){
	return emalloc(bytes);
}

/**
 * Wrapper to free memory within the parser
 */
static void zephir_wrapper_free(void *pointer){
	efree(pointer);
}

/**
 * Creates a parser_token to be passed to the parser
 */
static void zephir_parse_with_token(void* zephir_parser, int opcode, int parsercode, zephir_scanner_token *token, zephir_parser_status *parser_status){

	zephir_parser_token *pToken;

	pToken = emalloc(sizeof(zephir_parser_token));
	pToken->opcode = opcode;
	pToken->token = token->value;
	pToken->token_len = token->len;
	pToken->free_flag = 1;

	zephir_(zephir_parser, parsercode, pToken, parser_status);

	token->value = NULL;
	token->len = 0;
}

/**
 * Creates an error message when it's triggered by the scanner
 */
static void zephir_scanner_error_msg(zephir_parser_status *parser_status){

	/*char *error, *error_part;
	ZEPHIR_scanner_state *state = parser_status->scanner_state;

	//PHALCON_INIT_VAR(*error_msg);
	ALLOC_INIT_ZVAL(*error_msg);
	if (state->start) {
		error = emalloc(sizeof(char) * (128 + state->start_length +  Z_STRLEN_P(state->active_file)));
		if (state->start_length > 16) {
			error_part = estrndup(state->start, 16);
			sprintf(error, "Parsing error before '%s...' in %s on line %d", error_part, Z_STRVAL_P(state->active_file), state->active_line);
			efree(error_part);
		} else {
			sprintf(error, "Parsing error before '%s' in %s on line %d", state->start, Z_STRVAL_P(state->active_file), state->active_line);
		}
		ZVAL_STRING(*error_msg, error, 1);
	} else {
		error = emalloc(sizeof(char) * (64 + Z_STRLEN_P(state->active_file)));
		sprintf(error, "Parsing error near to EOF in %s", Z_STRVAL_P(state->active_file));
		ZVAL_STRING(*error_msg, error, 1);
	}
	efree(error);*/
}

/**
 * Parses a comment returning an intermediate array representation
 */
int zephir_parse_program(zval **return_value, char *program, unsigned int program_length, char *file_path TSRMLS_DC) {

	char *error;
	zephir_scanner_state *state;
	zephir_scanner_token token;
	int scanner_status, status = SUCCESS, start_lines;
	zephir_parser_status *parser_status = NULL;
	void* zephir_parser;

	/**
	 * Check if the program has any length
	 */
	if (!program_length) {
		return FAILURE;
	}

	if (program_length < 2) {
		return SUCCESS;
	}

	/**
	 * Start the reentrant parser
	 */
	zephir_parser = zephir_Alloc(zephir_wrapper_alloc);

	parser_status = emalloc(sizeof(zephir_parser_status));
	state = emalloc(sizeof(zephir_scanner_state));

	parser_status->status = ZEPHIR_PARSING_OK;
	parser_status->scanner_state = state;
	parser_status->ret = NULL;
	parser_status->token = &token;
	parser_status->syntax_error = NULL;
	parser_status->number_brackets = 0;

	/**
	 * Initialize the scanner state
	 */
	state->active_token = 0;
	state->start = program;
	state->start_length = 0;
	state->active_file = file_path;
	state->active_line = 1;
	state->active_char = 1;

	state->end = state->start;

	while (0 <= (scanner_status = zephir_get_token(state, &token))) {

		state->active_token = token.opcode;

		state->start_length = (program + program_length - state->start);

		switch (token.opcode) {

			case ZEPHIR_T_IGNORE:
				break;

			case ZEPHIR_T_NAMESPACE:
				zephir_(zephir_parser, ZEPHIR_NAMESPACE, NULL, parser_status);
				break;
			case ZEPHIR_T_ABSTRACT:
				zephir_(zephir_parser, ZEPHIR_ABSTRACT, NULL, parser_status);
				break;
			case ZEPHIR_T_CLASS:
				zephir_(zephir_parser, ZEPHIR_CLASS, NULL, parser_status);
				break;
			case ZEPHIR_T_INTERFACE:
				zephir_(zephir_parser, ZEPHIR_INTERFACE, NULL, parser_status);
				break;
			case ZEPHIR_T_EXTENDS:
				zephir_(zephir_parser, ZEPHIR_EXTENDS, NULL, parser_status);
				break;
			case ZEPHIR_T_IMPLEMENTS:
				zephir_(zephir_parser, ZEPHIR_IMPLEMENTS, NULL, parser_status);
				break;
			case ZEPHIR_T_PUBLIC:
				zephir_(zephir_parser, ZEPHIR_PUBLIC, NULL, parser_status);
				break;
			case ZEPHIR_T_PROTECTED:
				zephir_(zephir_parser, ZEPHIR_PROTECTED, NULL, parser_status);
				break;
			case ZEPHIR_T_PRIVATE:
				zephir_(zephir_parser, ZEPHIR_PRIVATE, NULL, parser_status);
				break;
			case ZEPHIR_T_STATIC:
				zephir_(zephir_parser, ZEPHIR_STATIC, NULL, parser_status);
				break;
			case ZEPHIR_T_INLINE:
				zephir_(zephir_parser, ZEPHIR_INLINE, NULL, parser_status);
				break;
			case ZEPHIR_T_FINAL:
				zephir_(zephir_parser, ZEPHIR_FINAL, NULL, parser_status);
				break;
			case ZEPHIR_T_FUNCTION:
				zephir_(zephir_parser, ZEPHIR_FUNCTION, NULL, parser_status);
				break;
			case ZEPHIR_T_LET:
				zephir_(zephir_parser, ZEPHIR_LET, NULL, parser_status);
				break;
			case ZEPHIR_T_ECHO:
				zephir_(zephir_parser, ZEPHIR_ECHO, NULL, parser_status);
				break;
			case ZEPHIR_T_RETURN:
				zephir_(zephir_parser, ZEPHIR_RETURN, NULL, parser_status);
				break;
			case ZEPHIR_T_REQUIRE:
				zephir_(zephir_parser, ZEPHIR_REQUIRE, NULL, parser_status);
				break;
			case ZEPHIR_T_CLONE:
				zephir_(zephir_parser, ZEPHIR_CLONE, NULL, parser_status);
				break;
			case ZEPHIR_T_EMPTY:
				zephir_(zephir_parser, ZEPHIR_EMPTY, NULL, parser_status);
				break;
			case ZEPHIR_T_IF:
				zephir_(zephir_parser, ZEPHIR_IF, NULL, parser_status);
				break;
			case ZEPHIR_T_ELSE:
				zephir_(zephir_parser, ZEPHIR_ELSE, NULL, parser_status);
				break;
			case ZEPHIR_T_LOOP:
				zephir_(zephir_parser, ZEPHIR_LOOP, NULL, parser_status);
				break;
			case ZEPHIR_T_CONTINUE:
				zephir_(zephir_parser, ZEPHIR_CONTINUE, NULL, parser_status);
				break;
			case ZEPHIR_T_BREAK:
				zephir_(zephir_parser, ZEPHIR_BREAK, NULL, parser_status);
				break;
			case ZEPHIR_T_WHILE:
				zephir_(zephir_parser, ZEPHIR_WHILE, NULL, parser_status);
				break;
			case ZEPHIR_T_DO:
				zephir_(zephir_parser, ZEPHIR_DO, NULL, parser_status);
				break;
			case ZEPHIR_T_NEW:
				zephir_(zephir_parser, ZEPHIR_NEW, NULL, parser_status);
				break;
			case ZEPHIR_T_CONST:
				zephir_(zephir_parser, ZEPHIR_CONST, NULL, parser_status);
				break;
			case ZEPHIR_T_TYPEOF:
				zephir_(zephir_parser, ZEPHIR_TYPEOF, NULL, parser_status);
				break;
			case ZEPHIR_T_INSTANCEOF:
				zephir_(zephir_parser, ZEPHIR_INSTANCEOF, NULL, parser_status);
				break;
			case ZEPHIR_T_ISSET:
				zephir_(zephir_parser, ZEPHIR_ISSET, NULL, parser_status);
				break;
			case ZEPHIR_T_UNSET:
				zephir_(zephir_parser, ZEPHIR_UNSET, NULL, parser_status);
				break;
			case ZEPHIR_T_THROW:
				zephir_(zephir_parser, ZEPHIR_THROW, NULL, parser_status);
				break;
			case ZEPHIR_T_FOR:
				zephir_(zephir_parser, ZEPHIR_FOR, NULL, parser_status);
				break;
			case ZEPHIR_T_IN:
				zephir_(zephir_parser, ZEPHIR_IN, NULL, parser_status);
				break;
			case ZEPHIR_T_REVERSE:
				zephir_(zephir_parser, ZEPHIR_REVERSE, NULL, parser_status);
				break;
			case ZEPHIR_T_USE:
				zephir_(zephir_parser, ZEPHIR_USE, NULL, parser_status);
				break;
			case ZEPHIR_T_AS:
				zephir_(zephir_parser, ZEPHIR_AS, NULL, parser_status);
				break;
			case ZEPHIR_T_TRY:
				zephir_(zephir_parser, ZEPHIR_TRY, NULL, parser_status);
				break;
			case ZEPHIR_T_CATCH:
				zephir_(zephir_parser, ZEPHIR_CATCH, NULL, parser_status);
				break;

			case ZEPHIR_T_DOTCOMMA:
				zephir_(zephir_parser, ZEPHIR_DOTCOMMA, NULL, parser_status);
				break;
			case ZEPHIR_T_COMMA:
				zephir_(zephir_parser, ZEPHIR_COMMA, NULL, parser_status);
				break;
			case ZEPHIR_T_ASSIGN:
				zephir_(zephir_parser, ZEPHIR_ASSIGN, NULL, parser_status);
				break;
			case ZEPHIR_T_ADDASSIGN:
				zephir_(zephir_parser, ZEPHIR_ADDASSIGN, NULL, parser_status);
				break;
			case ZEPHIR_T_SUBASSIGN:
				zephir_(zephir_parser, ZEPHIR_SUBASSIGN, NULL, parser_status);
				break;
			case ZEPHIR_T_DIVASSIGN:
				zephir_(zephir_parser, ZEPHIR_DIVASSIGN, NULL, parser_status);
				break;
			case ZEPHIR_T_MULASSIGN:
				zephir_(zephir_parser, ZEPHIR_MULASSIGN, NULL, parser_status);
				break;
			case ZEPHIR_T_CONCATASSIGN:
				zephir_(zephir_parser, ZEPHIR_CONCATASSIGN, NULL, parser_status);
				break;
			case ZEPHIR_T_MODASSIGN:
				zephir_(zephir_parser, ZEPHIR_MODASSIGN, NULL, parser_status);
				break;
			case ZEPHIR_T_EQUALS:
				zephir_(zephir_parser, ZEPHIR_EQUALS, NULL, parser_status);
				break;
			case ZEPHIR_T_NOTEQUALS:
				zephir_(zephir_parser, ZEPHIR_NOTEQUALS, NULL, parser_status);
				break;
			case ZEPHIR_T_IDENTICAL:
				zephir_(zephir_parser, ZEPHIR_IDENTICAL, NULL, parser_status);
				break;
			case ZEPHIR_T_NOTIDENTICAL:
				zephir_(zephir_parser, ZEPHIR_NOTIDENTICAL, NULL, parser_status);
				break;
			case ZEPHIR_T_LESS:
				zephir_(zephir_parser, ZEPHIR_LESS, NULL, parser_status);
				break;
			case ZEPHIR_T_GREATER:
				zephir_(zephir_parser, ZEPHIR_GREATER, NULL, parser_status);
				break;
			case ZEPHIR_T_LESSEQUAL:
				zephir_(zephir_parser, ZEPHIR_LESSEQUAL, NULL, parser_status);
				break;
			case ZEPHIR_T_GREATEREQUAL:
				zephir_(zephir_parser, ZEPHIR_GREATEREQUAL, NULL, parser_status);
				break;
			case ZEPHIR_T_QUESTION:
				zephir_(zephir_parser, ZEPHIR_QUESTION, NULL, parser_status);
				break;
			case ZEPHIR_T_COLON:
				zephir_(zephir_parser, ZEPHIR_COLON, NULL, parser_status);
				break;
			case ZEPHIR_T_ARROW:
				zephir_(zephir_parser, ZEPHIR_ARROW, NULL, parser_status);
				break;
			case ZEPHIR_T_DOUBLECOLON:
				zephir_(zephir_parser, ZEPHIR_DOUBLECOLON, NULL, parser_status);
				break;
			case ZEPHIR_T_NOT:
				zephir_(zephir_parser, ZEPHIR_NOT, NULL, parser_status);
				break;
			case ZEPHIR_T_FETCH:
				zephir_(zephir_parser, ZEPHIR_FETCH, NULL, parser_status);
				break;
			case ZEPHIR_T_SWITCH:
				zephir_(zephir_parser, ZEPHIR_SWITCH, NULL, parser_status);
				break;
			case ZEPHIR_T_CASE:
				zephir_(zephir_parser, ZEPHIR_CASE, NULL, parser_status);
				break;
			case ZEPHIR_T_DEFAULT:
				zephir_(zephir_parser, ZEPHIR_DEFAULT, NULL, parser_status);
				break;

			case ZEPHIR_T_PARENTHESES_OPEN:
				zephir_(zephir_parser, ZEPHIR_PARENTHESES_OPEN, NULL, parser_status);
				break;
			case ZEPHIR_T_PARENTHESES_CLOSE:
				zephir_(zephir_parser, ZEPHIR_PARENTHESES_CLOSE, NULL, parser_status);
				break;

			case ZEPHIR_T_BRACKET_OPEN:
				parser_status->number_brackets++;
				zephir_(zephir_parser, ZEPHIR_BRACKET_OPEN, NULL, parser_status);
				break;
			case ZEPHIR_T_BRACKET_CLOSE:
				parser_status->number_brackets--;
				zephir_(zephir_parser, ZEPHIR_BRACKET_CLOSE, NULL, parser_status);
				break;

			case ZEPHIR_T_SBRACKET_OPEN:
				zephir_(zephir_parser, ZEPHIR_SBRACKET_OPEN, NULL, parser_status);
				break;
			case ZEPHIR_T_SBRACKET_CLOSE:
				zephir_(zephir_parser, ZEPHIR_SBRACKET_CLOSE, NULL, parser_status);
				break;

			case ZEPHIR_T_NULL:
				zephir_(zephir_parser, ZEPHIR_NULL, NULL, parser_status);
				break;
			case ZEPHIR_T_TRUE:
				zephir_(zephir_parser, ZEPHIR_TRUE, NULL, parser_status);
				break;
			case ZEPHIR_T_FALSE:
				zephir_(zephir_parser, ZEPHIR_FALSE, NULL, parser_status);
				break;
			case ZEPHIR_T_COMMENT:
				if (parser_status->number_brackets <= 1) {
					zephir_parse_with_token(zephir_parser, ZEPHIR_T_COMMENT, ZEPHIR_COMMENT, &token, parser_status);
				}
				break;
			case ZEPHIR_T_CBLOCK:
				zephir_parse_with_token(zephir_parser, ZEPHIR_T_CBLOCK, ZEPHIR_CBLOCK, &token, parser_status);
				break;
			case ZEPHIR_T_TYPE_INTEGER:
				zephir_(zephir_parser, ZEPHIR_TYPE_INTEGER, NULL, parser_status);
				break;
			case ZEPHIR_T_TYPE_UINTEGER:
				zephir_(zephir_parser, ZEPHIR_TYPE_UINTEGER, NULL, parser_status);
				break;
			case ZEPHIR_T_TYPE_CHAR:
				zephir_(zephir_parser, ZEPHIR_TYPE_CHAR, NULL, parser_status);
				break;
			case ZEPHIR_T_TYPE_UCHAR:
				zephir_(zephir_parser, ZEPHIR_TYPE_UCHAR, NULL, parser_status);
				break;
			case ZEPHIR_T_TYPE_LONG:
				zephir_(zephir_parser, ZEPHIR_TYPE_LONG, NULL, parser_status);
				break;
			case ZEPHIR_T_TYPE_ULONG:
				zephir_(zephir_parser, ZEPHIR_TYPE_ULONG, NULL, parser_status);
				break;
			case ZEPHIR_T_TYPE_DOUBLE:
				zephir_(zephir_parser, ZEPHIR_TYPE_DOUBLE, NULL, parser_status);
				break;
			case ZEPHIR_T_TYPE_STRING:
				zephir_(zephir_parser, ZEPHIR_TYPE_STRING, NULL, parser_status);
				break;
			case ZEPHIR_T_TYPE_BOOL:
				zephir_(zephir_parser, ZEPHIR_TYPE_BOOL, NULL, parser_status);
				break;
			case ZEPHIR_T_TYPE_ARRAY:
				zephir_(zephir_parser, ZEPHIR_TYPE_ARRAY, NULL, parser_status);
				break;
			case ZEPHIR_T_TYPE_VAR:
				zephir_(zephir_parser, ZEPHIR_TYPE_VAR, NULL, parser_status);
				break;
		    case ZEPHIR_T_TYPE_OBJECT:
                zephir_(zephir_parser, ZEPHIR_TYPE_OBJECT, NULL, parser_status);
                break;
		    case ZEPHIR_T_TYPE_RESOURCE:
                zephir_(zephir_parser, ZEPHIR_TYPE_RESOURCE, NULL, parser_status);
                break;
            case ZEPHIR_T_TYPE_CALLABLE:
                zephir_(zephir_parser, ZEPHIR_TYPE_CALLABLE, NULL, parser_status);
                break;

			case ZEPHIR_T_ADD:
				zephir_(zephir_parser, ZEPHIR_ADD, NULL, parser_status);
				break;
			case ZEPHIR_T_SUB:
				zephir_(zephir_parser, ZEPHIR_SUB, NULL, parser_status);
				break;
			case ZEPHIR_T_MUL:
				zephir_(zephir_parser, ZEPHIR_MUL, NULL, parser_status);
				break;
			case ZEPHIR_T_DIV:
				zephir_(zephir_parser, ZEPHIR_DIV, NULL, parser_status);
				break;
			case ZEPHIR_T_MOD:
				zephir_(zephir_parser, ZEPHIR_MOD, NULL, parser_status);
				break;
			case ZEPHIR_T_DOT:
				zephir_(zephir_parser, ZEPHIR_CONCAT, NULL, parser_status);
				break;
			case ZEPHIR_T_INCR:
				zephir_(zephir_parser, ZEPHIR_INCR, NULL, parser_status);
				break;
			case ZEPHIR_T_DECR:
				zephir_(zephir_parser, ZEPHIR_DECR, NULL, parser_status);
				break;
			case ZEPHIR_T_AND:
				zephir_(zephir_parser, ZEPHIR_AND, NULL, parser_status);
				break;
			case ZEPHIR_T_OR:
				zephir_(zephir_parser, ZEPHIR_OR, NULL, parser_status);
				break;
			case ZEPHIR_T_BITWISE_AND:
				zephir_(zephir_parser, ZEPHIR_BITWISE_AND, NULL, parser_status);
				break;
			case ZEPHIR_T_BITWISE_OR:
				zephir_(zephir_parser, ZEPHIR_BITWISE_OR, NULL, parser_status);
				break;
			case ZEPHIR_T_BITWISE_XOR:
				zephir_(zephir_parser, ZEPHIR_BITWISE_XOR, NULL, parser_status);
				break;
			case ZEPHIR_T_BITWISE_SHIFTLEFT:
				zephir_(zephir_parser, ZEPHIR_BITWISE_SHIFTLEFT, NULL, parser_status);
				break;
			case ZEPHIR_T_BITWISE_SHIFTRIGHT:
				zephir_(zephir_parser, ZEPHIR_BITWISE_SHIFTRIGHT, NULL, parser_status);
				break;
			case ZEPHIR_T_INTEGER:
				zephir_parse_with_token(zephir_parser, ZEPHIR_T_INTEGER, ZEPHIR_INTEGER, &token, parser_status);
				break;
			case ZEPHIR_T_DOUBLE:
				zephir_parse_with_token(zephir_parser, ZEPHIR_T_DOUBLE, ZEPHIR_DOUBLE, &token, parser_status);
				break;
			case ZEPHIR_T_STRING:
				zephir_parse_with_token(zephir_parser, ZEPHIR_T_STRING, ZEPHIR_STRING, &token, parser_status);
				break;
			case ZEPHIR_T_CHAR:
				zephir_parse_with_token(zephir_parser, ZEPHIR_T_CHAR, ZEPHIR_CHAR, &token, parser_status);
				break;
			case ZEPHIR_T_IDENTIFIER:
				zephir_parse_with_token(zephir_parser, ZEPHIR_T_IDENTIFIER, ZEPHIR_IDENTIFIER, &token, parser_status);
				break;
			case ZEPHIR_T_CONSTANT:
				zephir_parse_with_token(zephir_parser, ZEPHIR_T_CONSTANT, ZEPHIR_CONSTANT, &token, parser_status);
				break;

			case ZEPHIR_T_VOID:
				zephir_(zephir_parser, ZEPHIR_VOID, NULL, parser_status);
				break;
			case ZEPHIR_T_LIKELY:
				zephir_(zephir_parser, ZEPHIR_LIKELY, NULL, parser_status);
				break;
			case ZEPHIR_T_UNLIKELY:
				zephir_(zephir_parser, ZEPHIR_UNLIKELY, NULL, parser_status);
				break;

			default:
				parser_status->status = ZEPHIR_PARSING_FAILED;
				fprintf(stderr, "Scanner: unknown opcode %d\n", token.opcode);
				/*if (!*error_msg) {
					error = emalloc(sizeof(char) * (48 + Z_STRLEN_P(state->active_file)));
					sprintf(error, "Scanner: unknown opcode %d on in %s line %d", token.opcode, Z_STRVAL_P(state->active_file), state->active_line);
					//PHALCON_INIT_VAR(*error_msg);
					ALLOC_INIT_ZVAL(*error_msg);
					ZVAL_STRING(*error_msg, error, 1);
					efree(error);
				}*/
				break;
		}

		if (parser_status->status != ZEPHIR_PARSING_OK) {
			status = FAILURE;
			break;
		}

		state->end = state->start;
	}

	if (status != FAILURE) {
		switch (scanner_status) {
			case ZEPHIR_SCANNER_RETCODE_ERR:
			case ZEPHIR_SCANNER_RETCODE_IMPOSSIBLE:
				{
					char *x = malloc(sizeof(char) * 1024);
					if (state->start) {
						sprintf(x, "Scanner error: %d %s", scanner_status, state->start);
					} else {
						sprintf(x, "Scanner error: %d", scanner_status);
					}
					//json_object *syntax_error = json_object_new_object();
					//json_object_object_add(syntax_error, "type", json_object_new_string("error"));
					//json_object_object_add(syntax_error, "message", json_object_new_string(x));
					fprintf(stderr, "%s\n", x);
					//free(x);
					//parser_status->ret = syntax_error;
					status = FAILURE;
				}
				break;
			default:
				zephir_(zephir_parser, 0, NULL, parser_status);
		}
	}

	state->active_token = 0;
	state->start = NULL;

	if (parser_status->status != ZEPHIR_PARSING_OK) {
		status = FAILURE;
		/*if (parser_status->syntax_error) {
			if (!*error_msg) {
				//PHALCON_INIT_VAR(*error_msg);
				ALLOC_INIT_ZVAL(*error_msg);
				ZVAL_STRING(*error_msg, parser_status->syntax_error, 1);
			}
			efree(parser_status->syntax_error);
		}*/
		//fprintf(stderr, "error!\n");
	}

	zephir_Free(zephir_parser, zephir_wrapper_free);

	if (status != FAILURE) {
		if (parser_status->status == ZEPHIR_PARSING_OK) {
			//printf("%s\n", json_object_to_json_string(parser_status->ret));
			/*if (parser_status->ret) {
				ZVAL_ZVAL(*result, parser_status->ret, 0, 0);
				ZVAL_NULL(parser_status->ret);
				zval_ptr_dtor(&parser_status->ret);
			} else {
				array_init(*result);
			}*/
		}
	}

	if (parser_status->ret) {
		//printf("%s\n", json_object_to_json_string(parser_status->ret));
		//zend_print_zval_r(parser_status->ret, 0 TSRMLS_CC);
		*return_value = parser_status->ret;
	}

	//efree(Z_STRVAL(processed_comment));*/

	efree(parser_status);
	efree(state);

	return status;
}
