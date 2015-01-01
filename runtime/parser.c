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
 /*     0 */   574, 1157,  292,  295, 1182,    1,    2,  791,    4,    5,
 /*    10 */     6,    7,    8,    9,  344,  398,  162,   46,   47,   51,
 /*    20 */    52,  277,  130,  138,  749,  142,  280,  354,  289,  359,
 /*    30 */   286,  378,  274,  735,  369,  773,  750,  348,  233,  741,
 /*    40 */   252,  306,  308,  316,  310,  312,  314,  385,  246,  623,
 /*    50 */   736,  742,  368,  514,  338,  303,  299,  365,  303,  299,
 /*    60 */   583,  592,  601,  604,  595,  598,  607,  613,  610,  619,
 /*    70 */   616,  254,  353,  664,  234,  236,  238,  487,  247,  540,
 /*    80 */   509,  302,  258,  262,  267,  456,  759,  467,  725,  747,
 /*    90 */   474,  392,  394,  393,  358,  723,  515,  362,  432,  395,
 /*   100 */   168,  555,  570,  577,  580,  361,  363,  364,  366,  367,
 /*   110 */   624,  574,  788,  292,  295,  195,  349,  193,  197,  757,
 /*   120 */   763,  120,  762,  747,  681,  344,  117,  384,  183,  723,
 /*   130 */   130,  138,  277,  142,  168,  584,  591,  280,  354,  289,
 /*   140 */   359,  286,  378,  274,  353,  369,   84,  350,  348,  233,
 /*   150 */     3,    4,    5,    6,    7,    8,    9,  729,  385,  718,
 /*   160 */   623,  440,  431,  368,  109,  338,  735,  586,  365,  730,
 /*   170 */   426,  583,  592,  601,  604,  595,  598,  607,  613,  610,
 /*   180 */   619,  616,  593,  591,  565,  234,  236,  238,  349,  247,
 /*   190 */   596,  591,  302,  258,  262,  267,  456,  158,  467,  646,
 /*   200 */   161,  474,  392,  394,  393,  358,  585,  652,  362,  428,
 /*   210 */   395,  675,  555,  570,  577,  580,  361,  363,  364,  366,
 /*   220 */   367,  624,  574,  205,  292,  295,  676,  302,   44,   73,
 /*   230 */    47,   51,   52,  105,  106,  110,  344,  392,  394,  393,
 /*   240 */   358,  384,  586,  277,  423,  395,   81,  464,  280,  354,
 /*   250 */   289,  359,  286,  378,  274,  492,  369,  501,  509,  348,
 /*   260 */   233,  384,   77,  721,  198,   80,  725,  747,  250,  385,
 /*   270 */   206,  623,  183,  723,  368,  446,  338,  143,  168,  365,
 /*   280 */    76,  594,  583,  592,  601,  604,  595,  598,  607,  613,
 /*   290 */   610,  619,  616,  599,  591,  383,  234,  236,  238,  254,
 /*   300 */   247,  664,   20,  302,  258,  262,  267,  456,  479,  467,
 /*   310 */   764,  200,  474,  392,  394,  393,  358,  199,  549,  362,
 /*   320 */   382,  395,  168,  555,  570,  577,  580,  361,  363,  364,
 /*   330 */   366,  367,  624,  574,  588,  292,  295,   78,  302,   44,
 /*   340 */    73,   47,   51,   52,  602,  591,  200,  344,  392,  394,
 /*   350 */   393,  358,  199,  586,  277,  389,  395,  168,  193,  280,
 /*   360 */   354,  289,  359,  286,  378,  274,  302,  369,  605,  591,
 /*   370 */   348,  233,  130,  138,  586,   82,  392,  394,  393,  358,
 /*   380 */   385,  714,  623,  399,  395,  368,  523,  338,  529,  509,
 /*   390 */   365,  774,  597,  583,  592,  601,  604,  595,  598,  607,
 /*   400 */   613,  610,  619,  616,  608,  591,  696,  234,  236,  238,
 /*   410 */   632,  247,   55,  600,  302,  258,  262,  267,  456,  353,
 /*   420 */   467,  611,  591,  474,  392,  394,  393,  358,  460,  656,
 /*   430 */   362,  406,  395,  697,  555,  570,  577,  580,  361,  363,
 /*   440 */   364,  366,  367,  624,  574, 1155,  292,  295,  704,  302,
 /*   450 */    44,   73,   47,   51,   52,  614,  591,   35,  344,  392,
 /*   460 */   394,  393,  358,  349,   34,  277,  415,  395,  168,  518,
 /*   470 */   280,  354,  289,  359,  286,  378,  274,  302,  369,  617,
 /*   480 */   591,  348,  233,   40,  586,  586,   74,  392,  394,  393,
 /*   490 */   358,  385,  712,  623,  439,  395,  368,   19,  338,  469,
 /*   500 */   586,  365,  620,  591,  583,  592,  601,  604,  595,  598,
 /*   510 */   607,  613,  610,  619,  616,  463,  655,   37,  234,  236,
 /*   520 */   238,  586,  247,  603,  606,  302,  258,  262,  267,  456,
 /*   530 */    38,  467,  475,  554,  474,  392,  394,  393,  358,  609,
 /*   540 */   477,  362,  445,  395,  384,  555,  570,  577,  580,  361,
 /*   550 */   363,  364,  366,  367,  624,  574,  586,  292,  295,  722,
 /*   560 */   612,  758,  200,  761,  109,  762,  747,  169,  723,  344,
 /*   570 */   300,  169,  723,  168,  377,  586,  277,  168,  390,  476,
 /*   580 */   435,  280,  354,  289,  359,  286,  378,  274,  560,  369,
 /*   590 */   116,  124,  348,  233,  104,  615,  556,  561,  392,  394,
 /*   600 */   393,  358,  385,  459,  623,  251,  667,  368,  187,  338,
 /*   610 */   768,   94,  365,  193,  618,  583,  592,  601,  604,  595,
 /*   620 */   598,  607,  613,  610,  619,  616,  513,   25,  586,  234,
 /*   630 */   236,  238,   28,  247,  376,  108,  500,  258,  262,  267,
 /*   640 */   456,  789,  467,  498,  709,  474,  392,  394,  393,  358,
 /*   650 */   130,  138,  362,  142,  103,  519,  555,  570,  577,  580,
 /*   660 */   361,  363,  364,  366,  367,  624,  574,  621,  292,  295,
 /*   670 */    75,  489,  787,   81,  153,  516,  130,  138,  635,  142,
 /*   680 */   344,  392,  394,  393,  358,  168,  517,  277,  392,  394,
 /*   690 */   393,  358,  280,  354,  289,  359,  286,  378,  274,  302,
 /*   700 */   369,  388,  190,  348,  233,  193,  379,  682,  478,  392,
 /*   710 */   394,  393,  358,  385,  708,  623,  410,  409,  368,  775,
 /*   720 */   338,  781,  164,  365,  193,  167,  583,  592,  601,  604,
 /*   730 */   595,  598,  607,  613,  610,  619,  616,  491,   17,   18,
 /*   740 */   234,  236,  238,  502,  247,  396,  508,  512,  258,  262,
 /*   750 */   267,  456,  687,  467,  694,   41,  474,   44,   73,   47,
 /*   760 */    51,   52,  384,  362,  683,  168,  192,  555,  570,  577,
 /*   770 */   580,  361,  363,  364,  366,  367,  624,  574,  397,  292,
 /*   780 */   295,  191,  489,  767,  530,  134,  553,  508,  402,  370,
 /*   790 */   131,  344,  392,  394,  393,  358,  400,   16,  277,  392,
 /*   800 */   394,  393,  358,  280,  354,  289,  359,  286,  378,  274,
 /*   810 */   420,  369,   21,  259,  348,  233,  688,   14,   44,   73,
 /*   820 */    47,   51,   52,  703,  385,  706,  623,  541,  522,  368,
 /*   830 */   508,  338, 1174,  133,  365,  699,   15,  583,  592,  601,
 /*   840 */   604,  595,  598,  607,  613,  610,  619,  616,  732,  739,
 /*   850 */    94,  234,  236,  238,   55,  247,  558,  735,   81,  258,
 /*   860 */   262,  267,  456,  715,  467,  384,  159,  474,   44,   73,
 /*   870 */    47,   51,   52,   56,  362,  711,  737,  744,  555,  570,
 /*   880 */   577,  580,  361,  363,  364,  366,  367,  624,  574,  741,
 /*   890 */   292,  295,  710,  489,  384,  557,  727,  488,  738,  407,
 /*   900 */   317,  742,  344,  392,  394,  393,  358,  168,  436,  277,
 /*   910 */   392,  394,  393,  358,  280,  354,  289,  359,  286,  378,
 /*   920 */   274,  782,  369,  767,  705,  348,  233,  165,  416,   44,
 /*   930 */    73,   47,   51,   52,  752,  385,  645,  623,  403,  769,
 /*   940 */   368,  767,  338,  735,  778,  365,   42,  193,  583,  592,
 /*   950 */   601,  604,  595,  598,  607,  613,  610,  619,  616,  779,
 /*   960 */   634,  767,  234,  236,  238,  404,  247,  776,  442,  384,
 /*   970 */   258,  262,  267,  456,  114,  467,   81,  716,  474,   44,
 /*   980 */    73,   47,   51,   52,  384,  362,  122,  633,  777,  555,
 /*   990 */   570,  577,  580,  361,  363,  364,  366,  367,  624,  574,
 /*  1000 */   384,  292,  295,  424,  489,  405,  155,  140,  503,  160,
 /*  1010 */   163,  293,  129,  344,  392,  394,  393,  358,  429,  527,
 /*  1020 */   277,  392,  394,  393,  358,  280,  354,  289,  359,  286,
 /*  1030 */   378,  274,  560,  369,  433,  786,  348,  233,  166,   45,
 /*  1040 */   567,  559,  392,  394,  393,  358,  385,  702,  623,  271,
 /*  1050 */    12,  368,  717,  338,  115,  766,  365,   58,   11,  583,
 /*  1060 */   592,  601,  604,  595,  598,  607,  613,  610,  619,  616,
 /*  1070 */   203,  181,  180,  234,  236,  238,  468,  247,  244,  186,
 /*  1080 */   329,  258,  262,  267,  456,  193,  467,  534,   57,  474,
 /*  1090 */   392,  394,  393,  358,  243,  127,  362,  174,  438,  411,
 /*  1100 */   555,  570,  577,  580,  361,  363,  364,  366,  367,  624,
 /*  1110 */   574,  654,  292,  295,  790,  489,  538,  156,  745,  497,
 /*  1120 */   780,  135,  641,  412,  344,  392,  394,  393,  358,  731,
 /*  1130 */   237,  277,  392,  394,  393,  358,  280,  354,  289,  359,
 /*  1140 */   286,  378,  274,  347,  369,  733,  461,  348,  233,  413,
 /*  1150 */   734,  240,  462,  392,  394,  393,  358,  385,  700,  623,
 /*  1160 */   569,  849,  368,  346,  338,  850,  740,  365,  444,  118,
 /*  1170 */   583,  592,  601,  604,  595,  598,  607,  613,  610,  619,
 /*  1180 */   616,  545,   54,  743,  234,  236,  238,  202,  247, 1175,
 /*  1190 */   273,  315,  258,  262,  267,  456,  264,  467,  746,  196,
 /*  1200 */   474,  392,  394,  393,  358,  457, 1176,  362,  550,  126,
 /*  1210 */   551,  555,  570,  577,  580,  361,  363,  364,  366,  367,
 /*  1220 */   624,  574,  755,  292,  295,  356,  489,  419,  751,   48,
 /*  1230 */   539,  754,  753,  448,  552,  344,  392,  394,  393,  358,
 /*  1240 */   355,  136,  277,  392,  394,  393,  358,  280,  354,  289,
 /*  1250 */   359,  286,  378,  274,  313,  369,  756,  125,  348,  233,
 /*  1260 */   175,  185,  783,  119,  392,  394,  393,  358,  385,  208,
 /*  1270 */   623,  176,  342,  368,  235,  338,  268,  345,  365,   79,
 /*  1280 */   792,  583,  592,  601,  604,  595,  598,  607,  613,  610,
 /*  1290 */   619,  616,  351,  765,  443,  234,  236,  238,  590,  247,
 /*  1300 */   437,  661,  311,  258,  262,  267,  456,  253,  467,   85,
 /*  1310 */   665,  474,  392,  394,  393,  358,  496,  659,  362,   31,
 /*  1320 */   139,  386,  555,  570,  577,  580,  361,  363,  364,  366,
 /*  1330 */   367,  624,  574,   95,  292,  295,  499,  489,  387,  670,
 /*  1340 */   189,  542,  589,  310,  312,  314,  344,  392,  394,  393,
 /*  1350 */   358,  671,  179,  277,  303,  299,  785,  173,  280,  354,
 /*  1360 */   289,  359,  286,  378,  274,  296,  369,   27,  587,  348,
 /*  1370 */   233,  563,  301,  107,   28,  392,  394,  393,  358,  385,
 /*  1380 */   663,  623,  380,  677,  368,  653,  338,   26,  188,  365,
 /*  1390 */   506,  255,  583,  592,  601,  604,  595,  598,  607,  613,
 /*  1400 */   610,  619,  616,  249,  132,  648,  234,  236,  238,  121,
 /*  1410 */   247,  256,   24,  309,  258,  262,  267,  456,  770,  467,
 /*  1420 */   137, 1156,  474,  392,  394,  393,  358,   23,  647,  362,
 /*  1430 */   772,  123,  178,  555,  570,  577,  580,  361,  363,  364,
 /*  1440 */   366,  367,  624,  574,  141,  292,  295,  689,  489,  605,
 /*  1450 */   605,  605,  493,  605,  605,  298,  605,  344,  392,  394,
 /*  1460 */   393,  358,  605,  605,  277,  392,  394,  393,  358,  280,
 /*  1470 */   354,  289,  359,  286,  378,  274,  307,  369,  605,  605,
 /*  1480 */   348,  233,  605,  605,  605,  605,  392,  394,  393,  358,
 /*  1490 */   385,  466,  623,  605,  605,  368,  605,  338,  605,  605,
 /*  1500 */   365,  605,  605,  583,  592,  601,  604,  595,  598,  607,
 /*  1510 */   613,  610,  619,  616,  605,  605,  605,  234,  236,  238,
 /*  1520 */   605,  247,  605,  605,  605,  258,  262,  267,  456,   33,
 /*  1530 */   467,   35,  605,  474,  605,  605,  169,  605,  170,  605,
 /*  1540 */   362,  605,  168,  605,  555,  570,  577,  580,  361,  363,
 /*  1550 */   364,  366,  367,  624,  574,  605,  292,  295,  605,  489,
 /*  1560 */   605,  605,  605,  520,  605,  605,  453,  605,  344,  392,
 /*  1570 */   394,  393,  358,  605,  605,  277,  392,  394,  393,  358,
 /*  1580 */   280,  354,  289,  359,  286,  378,  274,  578,  369,  605,
 /*  1590 */   605,  348,  233,  605,  605,  605,  605,  392,  394,  393,
 /*  1600 */   358,  385,  692,  623,  605,  605,  368,  605,  338,  605,
 /*  1610 */   605,  365,  605,  605,  583,  592,  601,  604,  595,  598,
 /*  1620 */   607,  613,  610,  619,  616,  605,  605,  605,  234,  236,
 /*  1630 */   238,  605,  247,  605,  605,  343,  258,  262,  267,  456,
 /*  1640 */   605,  467,  605,  605,  474,  392,  394,  393,  358,  605,
 /*  1650 */   605,  362,  605,  605,  605,  555,  570,  577,  580,  361,
 /*  1660 */   363,  364,  366,  367,  624,  574,  605,  292,  295,  605,
 /*  1670 */   489,  605,  605,  605,  524,  605,  605,  304,  605,  344,
 /*  1680 */   392,  394,  393,  358,  605,  605,  277,  392,  394,  393,
 /*  1690 */   358,  280,  354,  289,  359,  286,  378,  274,  281,  369,
 /*  1700 */   605,  605,  348,  233,  605,  605,  605,  605,  392,  394,
 /*  1710 */   393,  358,  385,  690,  623,  605,  605,  368,  109,  338,
 /*  1720 */   605,  605,  365,  605,  605,  583,  592,  601,  604,  595,
 /*  1730 */   598,  607,  613,  610,  619,  616,  605,  605,  605,  234,
 /*  1740 */   236,  238,  605,  247,  605,  605,  337,  258,  262,  267,
 /*  1750 */   456,  605,  467,  605,  605,  474,  392,  394,  393,  358,
 /*  1760 */   605,  605,  362,  605,  605,  605,  555,  570,  577,  580,
 /*  1770 */   361,  363,  364,  366,  367,  624,  574,  605,  292,  295,
 /*  1780 */   605,  489,  605,  605,  605,  546,  102,  111,  106,  110,
 /*  1790 */   344,  392,  394,  393,  358,  605,  605,  277,  605,  605,
 /*  1800 */   605,  605,  280,  354,  289,  359,  286,  378,  274,  275,
 /*  1810 */   369,  605,  605,  348,  233,  605,  605,  605,  605,  392,
 /*  1820 */   394,  393,  358,  385,  668,  623,  605,  605,  368,  605,
 /*  1830 */   338,  605,  605,  365,  605,  605,  583,  592,  601,  604,
 /*  1840 */   595,  598,  607,  613,  610,  619,  616,  605,  605,  605,
 /*  1850 */   234,  236,  238,  605,  247,  605,  605,  327,  258,  262,
 /*  1860 */   267,  456,  605,  467,  605,  605,  474,  392,  394,  393,
 /*  1870 */   358,  605,  605,  362,  605,  605,  605,  555,  570,  577,
 /*  1880 */   580,  361,  363,  364,  366,  367,  624,  574,  605,  292,
 /*  1890 */   295,  605,  489,  605,  605,  605,  528,  605,  605,  325,
 /*  1900 */   605,  344,  392,  394,  393,  358,  605,  605,  277,  392,
 /*  1910 */   394,  393,  358,  280,  354,  289,  359,  286,  378,  274,
 /*  1920 */   335,  369,  605,  605,  348,  233,  605,  605,  605,  605,
 /*  1930 */   392,  394,  393,  358,  385,  686,  623,  605,  605,  368,
 /*  1940 */   605,  338,  605,  605,  365,  605,  605,  583,  592,  601,
 /*  1950 */   604,  595,  598,  607,  613,  610,  619,  616,  605,  605,
 /*  1960 */   605,  234,  236,  238,  605,  247,  605,  605,  454,  258,
 /*  1970 */   262,  267,  456,  605,  467,  605,  605,  474,  392,  394,
 /*  1980 */   393,  358,  605,  605,  362,  605,  605,  605,  555,  570,
 /*  1990 */   577,  580,  361,  363,  364,  366,  367,  624,  574,  605,
 /*  2000 */   292,  295,  605,  489,  605,  605,  605,  535,  605,  605,
 /*  2010 */   357,  605,  344,  392,  394,  393,  358,  605,  605,  277,
 /*  2020 */   392,  394,  393,  358,  280,  354,  289,  359,  286,  378,
 /*  2030 */   274,  575,  369,  605,  605,  348,  233,  605,  605,  605,
 /*  2040 */   605,  392,  394,  393,  358,  385,  684,  623,  605,  605,
 /*  2050 */   368,  605,  338,  605,  605,  365,  605,  605,  583,  592,
 /*  2060 */   601,  604,  595,  598,  607,  613,  610,  619,  616,  605,
 /*  2070 */   605,  605,  234,  236,  238,  605,  247,  605,  605,  427,
 /*  2080 */   258,  262,  267,  456,  605,  467,  605,  605,  474,  392,
 /*  2090 */   394,  393,  358,  605,  605,  362,  605,  605,  605,  555,
 /*  2100 */   570,  577,  580,  361,  363,  364,  366,  367,  624,  574,
 /*  2110 */   605,  292,  295,  605,  489,  605,  605,  605,  507,  605,
 /*  2120 */   605,  452,  605,  344,  392,  394,  393,  358,  605,  605,
 /*  2130 */   277,  392,  394,  393,  358,  280,  354,  289,  359,  286,
 /*  2140 */   378,  274,  278,  369,  605,  605,  348,  233,  605,  605,
 /*  2150 */   605,  605,  392,  394,  393,  358,  385,  680,  623,  605,
 /*  2160 */   605,  368,  605,  338,  605,  605,  365,  605,  605,  583,
 /*  2170 */   592,  601,  604,  595,  598,  607,  613,  610,  619,  616,
 /*  2180 */   605,  605,  605,  234,  236,  238,  605,  247,  605,  605,
 /*  2190 */   323,  258,  262,  267,  456,  605,  467,  605,  605,  474,
 /*  2200 */   392,  394,  393,  358,  605,  605,  362,  605,  605,  605,
 /*  2210 */   555,  570,  577,  580,  361,  363,  364,  366,  367,  624,
 /*  2220 */   574,  605,  292,  295,  605,  489,  605,  605,  605,  531,
 /*  2230 */   605,  605,  333,  605,  344,  392,  394,  393,  358,  605,
 /*  2240 */   605,  277,  392,  394,  393,  358,  280,  354,  289,  359,
 /*  2250 */   286,  378,  274,  284,  369,  605,  605,  348,  233,  605,
 /*  2260 */   605,  605,  605,  392,  394,  393,  358,  385,  678,  623,
 /*  2270 */   605,  605,  368,  605,  338,  605,  605,  365,  605,  605,
 /*  2280 */   583,  592,  601,  604,  595,  598,  607,  613,  610,  619,
 /*  2290 */   616,  605,  605,  605,  234,  236,  238,  605,  247,  605,
 /*  2300 */   605,  360,  258,  262,  267,  456,  605,  467,  605,  605,
 /*  2310 */   474,  392,  394,  393,  358,  605,  605,  362,  605,  605,
 /*  2320 */   605,  555,  570,  577,  580,  361,  363,  364,  366,  367,
 /*  2330 */   624,  574,  605,  292,  295,  605,  581,  605,  605,  605,
 /*  2340 */   605,  605,  605,  451,  605,  344,  392,  394,  393,  358,
 /*  2350 */   605,  605,  277,  392,  394,  393,  358,  280,  354,  289,
 /*  2360 */   359,  286,  378,  274,  321,  369,  605,  605,  348,  233,
 /*  2370 */   605,  605,  605,  605,  392,  394,  393,  358,  385,  662,
 /*  2380 */   623,  605,  605,  368,  605,  338,  605,  605,  365,  605,
 /*  2390 */   605,  583,  592,  601,  604,  595,  598,  607,  613,  610,
 /*  2400 */   619,  616,  605,  605,  605,  234,  236,  238,  605,  247,
 /*  2410 */   605,  605,  287,  258,  262,  267,  456,  605,  467,  605,
 /*  2420 */   605,  474,  392,  394,  393,  358,  605,  605,  362,  605,
 /*  2430 */   605,  605,  555,  570,  577,  580,  361,  363,  364,  366,
 /*  2440 */   367,  624,  574,  605,  292,  295,  605,  628,  605,  605,
 /*  2450 */   605,  605,  605,  605,  470,  605,  344,  392,  394,  393,
 /*  2460 */   358,  605,  605,  277,  392,  394,  393,  358,  280,  354,
 /*  2470 */   289,  359,  286,  378,  274,  319,  369,  605,  605,  348,
 /*  2480 */   233,  605,  605,  605,  605,  392,  394,  393,  358,  385,
 /*  2490 */   673,  623,  605,  605,  368,  605,  338,  605,  605,  365,
 /*  2500 */   605,  605,  583,  592,  601,  604,  595,  598,  607,  613,
 /*  2510 */   610,  619,  616,  605,  605,  605,  234,  236,  238,  605,
 /*  2520 */   247,  605,  605,  450,  258,  262,  267,  456,  605,  467,
 /*  2530 */   605,  605,  474,  392,  394,  393,  358,  605,  605,  362,
 /*  2540 */   605,  605,  605,  555,  570,  577,  580,  361,  363,  364,
 /*  2550 */   366,  367,  624,  574,  605,  292,  295,  605,  290,  605,
 /*  2560 */   605,  605,  605,  605,  605,  331,  605,  344,  392,  394,
 /*  2570 */   393,  358,  605,  605,  277,  392,  394,  393,  358,  280,
 /*  2580 */   354,  289,  359,  286,  378,  274,  449,  369,  605,  605,
 /*  2590 */   348,  233,  605,  605,  605,  605,  392,  394,  393,  358,
 /*  2600 */   385,  261,  623,  605,  605,  368,  605,  338,  605,  605,
 /*  2610 */   365,  605,  605,  583,  592,  601,  604,  595,  598,  607,
 /*  2620 */   613,  610,  619,  616,  605,  605,  605,  234,  236,  238,
 /*  2630 */   605,  247,  605,  605,  571,  258,  262,  267,  456,  605,
 /*  2640 */   467,  605,  605,  474,  392,  394,  393,  358,  605,  605,
 /*  2650 */   362,  605,  605,  605,  555,  570,  577,  580,  361,  363,
 /*  2660 */   364,  366,  367,  624,  574,  605,  292,  295,  605,  605,
 /*  2670 */   605,  605,  605,  605,  605,  605,  605,  605,  344,  605,
 /*  2680 */   605,  605,  605,  605,  605,  277,  605,  605,  605,  605,
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
 /*  2820 */   605,  605,  385,  242,  623,  605,  605,  368,  605,  338,
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
 /*  2930 */   605,  605,  605,  385,  473,  623,  605,  605,  368,  605,
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
 /*  3040 */   605,  605,  605,  605,  385,  626,  623,  605,  605,  368,
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
 /*  3150 */   605,  605,  605,  605,  605,  385,  270,  623,  605,  605,
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
 /*  3260 */   605,  605,  605,  605,  605,  605,  385,  650,  623,  605,
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
 /*  3370 */   605,  605,  605,  605,  605,  605,  605,  385,  651,  623,
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
 /*  3480 */   605,  605,  605,  605,  605,  605,  605,  605,  385,  644,
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
 /*  3600 */   669,  623,  605,  605,  368,  605,  338,  605,  605,  365,
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
 /*  3710 */   385,  658,  623,  605,  605,  368,  605,  338,  605,  605,
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
 /*  3820 */   605,  385,  266,  623,  605,  605,  368,  605,  338,  605,
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
 /*  3930 */   605,  605,  385,  638,  623,  605,  605,  368,  605,  338,
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
 /*  4040 */   605,  605,  605,  385,  639,  623,  605,  605,  368,  605,
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
 /*  4150 */   605,  605,  605,  605,  385,  631,  623,  605,  605,  368,
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
 /*  4260 */   605,  605,  605,  605,  605,  385,  657,  623,  605,  605,
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
 /*  4650 */   605,  605,  637,  303,  299,  605,  361,  363,  364,  366,
 /*  4660 */   367,  625,  211,  212,  213,  214,  215,  216,  217,  218,
 /*  4670 */   219,  220,  221,  222,  223,  224,  225,  226,  227,  228,
 /*  4680 */   229,  230,  232,  605,  605,  605,   97,  605,  660,  100,
 /*  4690 */   622,  605,  605,  231,  210,  455,  605,  605,  605,  605,
 /*  4700 */   562,  564,  566,  568,   96,  392,  394,  393,  358,   90,
 /*  4710 */   605,  101,  605,  605,  605,  605,  679,  605,  605,  605,
 /*  4720 */   605,  605,  605,  605,  605,  625,  211,  212,  213,  214,
 /*  4730 */   215,  216,  217,  218,  219,  220,  221,  222,  223,  224,
 /*  4740 */   225,  226,  227,  228,  229,  230,  232,  231,  210,   99,
 /*  4750 */   605,  605,  605,  532,  622,  605,  605,   98,   87,   89,
 /*  4760 */    92,   91,  605,  605,  562,  564,  566,  568,  605,  480,
 /*  4770 */   265,  144,  145,  147,  146,  148,  605,  605,  605,  625,
 /*  4780 */   211,  212,  213,  214,  215,  216,  217,  218,  219,  220,
 /*  4790 */   221,  222,  223,  224,  225,  226,  227,  228,  229,  230,
 /*  4800 */   232,  231,  210,  605,  605,  605,  605,  605,  622,  504,
 /*  4810 */   481,  482,  483,  484,  485,  486,  605,  605,  562,  564,
 /*  4820 */   566,  568,  150,  151,  465,  480,  605,  605,  605,  605,
 /*  4830 */   605,  605,  149,  625,  211,  212,  213,  214,  215,  216,
 /*  4840 */   217,  218,  219,  220,  221,  222,  223,  224,  225,  226,
 /*  4850 */   227,  228,  229,  230,  232,  231,  210,  605,  605,  605,
 /*  4860 */   525,  605,  622,  605,  605,  605,  481,  482,  483,  484,
 /*  4870 */   485,  486,  562,  564,  566,  568,  480,  605,  630,  605,
 /*  4880 */   605,  605,  605,  605,  605,  605,  605,  625,  211,  212,
 /*  4890 */   213,  214,  215,  216,  217,  218,  219,  220,  221,  222,
 /*  4900 */   223,  224,  225,  226,  227,  228,  229,  230,  232,  231,
 /*  4910 */   210,  605,  605,  605,  543,  605,  622,  481,  482,  483,
 /*  4920 */   484,  485,  486,  605,  605,  605,  562,  564,  566,  568,
 /*  4930 */   480,  605,  649,  605,  605,  605,  605,  605,  605,  605,
 /*  4940 */   605,  625,  211,  212,  213,  214,  215,  216,  217,  218,
 /*  4950 */   219,  220,  221,  222,  223,  224,  225,  226,  227,  228,
 /*  4960 */   229,  230,  232,  231,  210,  605,  605,  605,  605,  605,
 /*  4970 */   622,  481,  482,  483,  484,  485,  486,  605,  605,  605,
 /*  4980 */   562,  564,  566,  568,  480,  605,  472,  605,  605,  605,
 /*  4990 */   605,  605,  605,  605,  605,  625,  211,  212,  213,  214,
 /*  5000 */   215,  216,  217,  218,  219,  220,  221,  222,  223,  224,
 /*  5010 */   225,  226,  227,  228,  229,  230,  232,  231,  210,  605,
 /*  5020 */   605,  605,  605,  605,  622,  481,  482,  483,  484,  485,
 /*  5030 */   486,  605,  605,  605,  562,  564,  566,  568,  605,  605,
 /*  5040 */   719,  605,  605,  605,  605,  605,  605,  605,  605,  625,
 /*  5050 */   211,  212,  213,  214,  215,  216,  217,  218,  219,  220,
 /*  5060 */   221,  222,  223,  224,  225,  226,  227,  228,  229,  230,
 /*  5070 */   232,  231,  210,  605,  605,  605,  605,  605,  622,  263,
 /*  5080 */   605,  605,  605,  605,  605,  605,  455,  605,  562,  564,
 /*  5090 */   566,  568,  605,  605,  691,  605,  392,  394,  393,  358,
 /*  5100 */   605,  605,  605,  625,  211,  212,  213,  214,  215,  216,
 /*  5110 */   217,  218,  219,  220,  221,  222,  223,  224,  225,  226,
 /*  5120 */   227,  228,  229,  230,  232,  231,  210,  605,  605,  605,
 /*  5130 */   605,  605,  622,  248,  605,  605,  605,  605,  605,  605,
 /*  5140 */   455,  605,  562,  564,  566,  568,  605,  605,  458,  605,
 /*  5150 */   392,  394,  393,  358,  605,  605,  605,  625,  211,  212,
 /*  5160 */   213,  214,  215,  216,  217,  218,  219,  220,  221,  222,
 /*  5170 */   223,  224,  225,  226,  227,  228,  229,  230,  232,  231,
 /*  5180 */   210,  605,  605,  605,  605,  605,  622,  272,  605,  605,
 /*  5190 */   605,  605,  605,  605,  455,  605,  562,  564,  566,  568,
 /*  5200 */   605,  605,  666,  605,  392,  394,  393,  358,  605,  605,
 /*  5210 */   605,  625,  211,  212,  213,  214,  215,  216,  217,  218,
 /*  5220 */   219,  220,  221,  222,  223,  224,  225,  226,  227,  228,
 /*  5230 */   229,  230,  232,  231,  210,  605,  605,  605,  605,  605,
 /*  5240 */   622,  239,  605,  605,  605,  605,  605,  605,  455,  605,
 /*  5250 */   562,  564,  566,  568,  605,  605,  713,  605,  392,  394,
 /*  5260 */   393,  358,  605,  605,  605,  625,  211,  212,  213,  214,
 /*  5270 */   215,  216,  217,  218,  219,  220,  221,  222,  223,  224,
 /*  5280 */   225,  226,  227,  228,  229,  230,  232,  231,  210,  605,
 /*  5290 */   605,  605,  605,  605,  622,  605,  605,  605,  605,  605,
 /*  5300 */   605,  605,  605,  605,  562,  564,  566,  568,  605,  605,
 /*  5310 */   672,  605,  605,  605,  605,  605,  605,  605,  605,  625,
 /*  5320 */   211,  212,  213,  214,  215,  216,  217,  218,  219,  220,
 /*  5330 */   221,  222,  223,  224,  225,  226,  227,  228,  229,  230,
 /*  5340 */   232,  231,  210,  605,  605,  605,  605,  605,  622,  605,
 /*  5350 */   605,  605,  605,  605,  605,  605,  605,  605,  562,  564,
 /*  5360 */   566,  568,  605,  605,  260,  605,  605,  605,  605,  605,
 /*  5370 */   605,  605,  605,  625,  211,  212,  213,  214,  215,  216,
 /*  5380 */   217,  218,  219,  220,  221,  222,  223,  224,  225,  226,
 /*  5390 */   227,  228,  229,  230,  232,  231,  210,  605,  605,  605,
 /*  5400 */   605,  605,  622,  605,  605,  605,  605,  605,  605,  605,
 /*  5410 */   605,  605,  562,  564,  566,  568,  605,  605,  707,  605,
 /*  5420 */   605,  605,  605,  605,  605,  605,  605,  625,  211,  212,
 /*  5430 */   213,  214,  215,  216,  217,  218,  219,  220,  221,  222,
 /*  5440 */   223,  224,  225,  226,  227,  228,  229,  230,  232,  231,
 /*  5450 */   210,  605,  605,  605,  605,  605,  622,  605,  605,  605,
 /*  5460 */   605,  605,  605,  605,  605,  605,  562,  564,  566,  568,
 /*  5470 */   605,  605,  257,  605,  605,  605,  605,  605,  605,  605,
 /*  5480 */   605,  625,  211,  212,  213,  214,  215,  216,  217,  218,
 /*  5490 */   219,  220,  221,  222,  223,  224,  225,  226,  227,  228,
 /*  5500 */   229,  230,  232,  231,  210,  605,  605,  605,  605,  605,
 /*  5510 */   622,  605,  605,  605,  605,  605,  605,  605,  605,  605,
 /*  5520 */   562,  564,  566,  568,  605,  605,  685,  605,  605,  605,
 /*  5530 */   605,  605,  605,  605,  605,  625,  211,  212,  213,  214,
 /*  5540 */   215,  216,  217,  218,  219,  220,  221,  222,  223,  224,
 /*  5550 */   225,  226,  227,  228,  229,  230,  232,  231,  210,  605,
 /*  5560 */   605,  605,  605,  605,  622,  605,  605,  605,  605,  605,
 /*  5570 */   605,  605,  605,  605,  562,  564,  566,  568,  605,  605,
 /*  5580 */   207,  605,  605,  605,  605,  605,  605,  605,  605,  625,
 /*  5590 */   211,  212,  213,  214,  215,  216,  217,  218,  219,  220,
 /*  5600 */   221,  222,  223,  224,  225,  226,  227,  228,  229,  230,
 /*  5610 */   232,  231,  210,  605,  605,  605,  605,  605,  622,  605,
 /*  5620 */   605,  605,  605,  605,  605,  605,  605,  605,  562,  564,
 /*  5630 */   566,  568,  605,  605,  643,  605,  605,  605,  605,  605,
 /*  5640 */   605,  605,  605,  625,  211,  212,  213,  214,  215,  216,
 /*  5650 */   217,  218,  219,  220,  221,  222,  223,  224,  225,  226,
 /*  5660 */   227,  228,  229,  230,  232,  231,  210,  605,  605,  605,
 /*  5670 */   605,  605,  622,  605,  605,  605,  605,  605,  605,  605,
 /*  5680 */   605,  605,  562,  564,  566,  568,  605,  605,  269,  605,
 /*  5690 */   605,  605,  605,  605,  605,  605,  605,  625,  211,  212,
 /*  5700 */   213,  214,  215,  216,  217,  218,  219,  220,  221,  222,
 /*  5710 */   223,  224,  225,  226,  227,  228,  229,  230,  232,  231,
 /*  5720 */   210,  605,  605,  605,  605,  605,  622,  605,  605,  605,
 /*  5730 */   605,  605,  605,  605,  605,  605,  562,  564,  566,  568,
 /*  5740 */   605,  605,  245,  605,  605,  605,  605,  605,  605,  605,
 /*  5750 */   605,  625,  211,  212,  213,  214,  215,  216,  217,  218,
 /*  5760 */   219,  220,  221,  222,  223,  224,  225,  226,  227,  228,
 /*  5770 */   229,  230,  232,  231,  210,  605,  605,  605,  605,  605,
 /*  5780 */   622,  605,  605,  605,  605,  605,  605,  605,  605,  605,
 /*  5790 */   562,  564,  566,  568,  605,  605,  701,  605,  605,  605,
 /*  5800 */   605,  605,  605,  605,  605,  625,  211,  212,  213,  214,
 /*  5810 */   215,  216,  217,  218,  219,  220,  221,  222,  223,  224,
 /*  5820 */   225,  226,  227,  228,  229,  230,  232,  231,  210,  605,
 /*  5830 */   605,  605,  605,   97,  622,  605,   93,  605,  605,  605,
 /*  5840 */   605,  605,  605,  605,  562,  564,  566,  568,  605,  605,
 /*  5850 */   241,   96,  605,  605,  605,  605,   90,  605,  605,  625,
 /*  5860 */   211,  212,  213,  214,  215,  216,  217,  218,  219,  220,
 /*  5870 */   221,  222,  223,  224,  225,  226,  227,  228,  229,  230,
 /*  5880 */   232,  231,  210,  605,  605,  605,  605,  605,  622,  605,
 /*  5890 */   605,  605,  605,  605,  605,  605,   88,  605,  562,  564,
 /*  5900 */   566,  568,  605,  605,   86,   87,   89,   92,   91,  605,
 /*  5910 */   605,  605,  605,  209,  211,  212,  213,  214,  215,  216,
 /*  5920 */   217,  218,  219,  220,  221,  222,  223,  224,  225,  226,
 /*  5930 */   227,  228,  229,  230,  232,  144,  145,  147,  146,  148,
 /*  5940 */   536,  490,  622,  605,  605,  605,  605,  605,  605,  605,
 /*  5950 */   605,  605,  562,  564,  566,  568,  480,  605,  605,  605,
 /*  5960 */   605,  334,  605,  605,  332,  320,  318,  324,  322,  326,
 /*  5970 */   328,  330,  276,  282,  288,  291,  294,  297,  285,  279,
 /*  5980 */   306,  308,  316,  310,  312,  314,  150,  151,  605,   29,
 /*  5990 */   171,  605,  177,  605,  303,  299,  149,  481,  482,  483,
 /*  6000 */   484,  485,  486,  605,  521,  547,  548,  642,  605,  605,
 /*  6010 */   605,  605,  605,  605,  605,  605,  334,  605,  605,  332,
 /*  6020 */   320,  318,  324,  322,  326,  328,  330,  276,  282,  288,
 /*  6030 */   291,  294,  297,  285,  279,  306,  308,  316,  310,  312,
 /*  6040 */   314,  605,  605,  605,  605,  605,  605,  605,  605,  303,
 /*  6050 */   299,  605,  605,  579,  605,  605,  605,  605,  334,  605,
 /*  6060 */   605,  332,  320,  318,  324,  322,  326,  328,  330,  276,
 /*  6070 */   282,  288,  291,  294,  297,  285,  279,  306,  308,  316,
 /*  6080 */   310,  312,  314,  605,  605,  605,  605,  605,  605,  605,
 /*  6090 */   605,  303,  299,  605,  605,  572,  605,  605,  605,  605,
 /*  6100 */   334,  605,  605,  332,  320,  318,  324,  322,  326,  328,
 /*  6110 */   330,  276,  282,  288,  291,  294,  297,  285,  279,  306,
 /*  6120 */   308,  316,  310,  312,  314,  605,  605,  605,  605,  605,
 /*  6130 */   605,  605,  334,  303,  299,  332,  320,  318,  324,  322,
 /*  6140 */   326,  328,  330,  276,  282,  288,  291,  294,  297,  285,
 /*  6150 */   279,  306,  308,  316,  310,  312,  314,  494,  305,  605,
 /*  6160 */   605,  605,  605,  605,  370,  303,  299,  605,  605,  605,
 /*  6170 */   605,  605,  605,  480,  392,  394,  393,  358,  471,  605,
 /*  6180 */   375,  605,  605,  422,  418,  421,  605,  334,  605,  605,
 /*  6190 */   332,  320,  318,  324,  322,  326,  328,  330,  276,  282,
 /*  6200 */   288,  291,  294,  297,  285,  279,  306,  308,  316,  310,
 /*  6210 */   312,  314,  605,  605,  481,  482,  483,  484,  485,  486,
 /*  6220 */   303,  299,  510,  511,  582,  605,  605,  605,  605,  334,
 /*  6230 */   605,  605,  332,  320,  318,  324,  322,  326,  328,  330,
 /*  6240 */   276,  282,  288,  291,  294,  297,  285,  279,  306,  308,
 /*  6250 */   316,  310,  312,  314,  605,  144,  145,  147,  146,  148,
 /*  6260 */   605,  334,  303,  299,  332,  320,  318,  324,  322,  326,
 /*  6270 */   328,  330,  276,  282,  288,  291,  294,  297,  285,  279,
 /*  6280 */   306,  308,  316,  310,  312,  314,  605,  605,  605,  605,
 /*  6290 */   605,  605,  605,  605,  303,  299,  144,  145,  147,  146,
 /*  6300 */   148,  144,  145,  147,  146,  148,  150,  151,  336,  194,
 /*  6310 */   726,  605,  177,  605,  605,  605,  149,  334,  605, 1038,
 /*  6320 */   332,  320,  318,  324,  322,  326,  328,  330,  276,  282,
 /*  6330 */   288,  291,  294,  297,  285,  279,  306,  308,  316,  310,
 /*  6340 */   312,  314,  605,  605,  605,  605,  605,  150,  151,  605,
 /*  6350 */   303,  299,  150,  151,  605,  154,  605,  149,  172,  605,
 /*  6360 */   605,  605,  149,  629,  605,  605,  605,  605,  605,  605,
 /*  6370 */   605,  605,  334,  605,  605,  332,  320,  318,  324,  322,
 /*  6380 */   326,  328,  330,  276,  282,  288,  291,  294,  297,  285,
 /*  6390 */   279,  306,  308,  316,  310,  312,  314,  605,  144,  145,
 /*  6400 */   147,  146,  148,  605,  605,  303,  299,  605,  605,  576,
 /*  6410 */   605,  605,  605,  605,  334,  605,  605,  332,  320,  318,
 /*  6420 */   324,  322,  326,  328,  330,  276,  282,  288,  291,  294,
 /*  6430 */   297,  285,  279,  306,  308,  316,  310,  312,  314,  605,
 /*  6440 */   605,  605,  605,  605,  605,  605,  605,  303,  299,  150,
 /*  6450 */   151,  605,  827,  726,  605,  177,  605,  605,  605,  149,
 /*  6460 */   636,  605,  605,  605,  605,  605,  605,  605,  605,  334,
 /*  6470 */   605,  605,  332,  320,  318,  324,  322,  326,  328,  330,
 /*  6480 */   276,  282,  288,  291,  294,  297,  285,  279,  306,  308,
 /*  6490 */   316,  310,  312,  314,  283,  605,  292,  295,  605,  605,
 /*  6500 */   605,  340,  303,  299,  605,  605,  605,  605,  344,  605,
 /*  6510 */   605,  605,  605,  605,  605,  277,  605,  605,  605,  605,
 /*  6520 */   280,  354,  289,  359,  286,  378,  274,  605,  369,  605,
 /*  6530 */   605,  371,  605,  605,  605,  605,  605,  605,  605,  605,
 /*  6540 */   605,  385,  605,  605,  605,  605,  368,  605,  338,  605,
 /*  6550 */   605,  365,  605,  374,  605,  334,  605,  605,  332,  320,
 /*  6560 */   318,  324,  322,  326,  328,  330,  276,  282,  288,  291,
 /*  6570 */   294,  297,  285,  279,  306,  308,  316,  310,  312,  314,
 /*  6580 */   605,  605,  605,  283,  605,  292,  295,  605,  303,  299,
 /*  6590 */   605,  373,  605,  605,  605,  605,  605,  344,  605,  372,
 /*  6600 */   363,  364,  366,  367,  277,  605,  605,  605,  605,  280,
 /*  6610 */   354,  289,  359,  286,  378,  274,  425,  369,  605,  605,
 /*  6620 */   352,  605,  283,  605,  292,  295,  605,  605,  605,  605,
 /*  6630 */   385,  605,  605,  605,  605,  368,  344,  338,  605,  605,
 /*  6640 */   365,  605,  605,  277,  605,  605,  605,  605,  280,  354,
 /*  6650 */   289,  359,  286,  378,  274,  430,  369,  605,  605,  352,
 /*  6660 */    30,  605,   32,  605,  182,  605,   35,  605,  605,  385,
 /*  6670 */   605,  183,  605,  170,  368,  605,  338,  168,  605,  365,
 /*  6680 */   362,  144,  145,  147,  146,  148,  605,  605,  361,  363,
 /*  6690 */   364,  366,  367,  320,  318,  324,  322,  326,  328,  330,
 /*  6700 */   276,  282,  288,  291,  294,  297,  285,  279,  306,  308,
 /*  6710 */   316,  310,  312,  314,  605,  605,  605,  605,  605,  362,
 /*  6720 */   605,  605,  303,  299,  728,  605,  605,  361,  363,  364,
 /*  6730 */   366,  367,  150,  151,  605,  283,  605,  292,  295,  605,
 /*  6740 */   695,  605,  149,  605,  605,   10,   18,  605,   13,  344,
 /*  6750 */    22,  605,  184,  605,  771,  784,  277,  605,  623,  605,
 /*  6760 */   605,  280,  354,  289,  359,  286,  378,  274,  605,  369,
 /*  6770 */   605,  605,  348,  605,  283,  605,  292,  295,  605,  605,
 /*  6780 */   605,  605,  385,  605,  605,  605,  605,  368,  344,  338,
 /*  6790 */   605,  605,  365,  605,  537,  277,  605,  605,  605,  605,
 /*  6800 */   280,  354,  289,  359,  286,  378,  274,  434,  369,  605,
 /*  6810 */   605,  352,  605,  605,  605,  605,  605,  605,  605,  624,
 /*  6820 */   605,  385,  605,  605,  605,  605,  368,  605,  338,  605,
 /*  6830 */   605,  365,  362,  605,  144,  145,  147,  146,  148,  605,
 /*  6840 */   361,  363,  364,  366,  367,  318,  324,  322,  326,  328,
 /*  6850 */   330,  276,  282,  288,  291,  294,  297,  285,  279,  306,
 /*  6860 */   308,  316,  310,  312,  314,  605,  605,  605,  605,  605,
 /*  6870 */   605,  362,  605,  303,  299,  605,  605,  748,  605,  361,
 /*  6880 */   363,  364,  366,  367,  605,  150,  151,  283,  605,  292,
 /*  6890 */   295,  605,  605,  201,  605,  149,  605,  605,  605,  605,
 /*  6900 */   605,  344,  605,  605,  605,  605,  605,  605,  277,  605,
 /*  6910 */   605,  605,  605,  280,  354,  289,  359,  286,  378,  274,
 /*  6920 */   605,  369,  605,  605,  348,  605,  283,  605,  292,  295,
 /*  6930 */   605,  605,  605,  605,  385,  605,  605,  605,  605,  368,
 /*  6940 */   344,  338,  605,  605,  365,  605,  533,  277,  605,  605,
 /*  6950 */   605,  605,  280,  354,  289,  359,  286,  378,  274,  605,
 /*  6960 */   369,  605,  605,  348,  605,  605,  605,  605,  605,  605,
 /*  6970 */   605,  605,  605,  385,  605,  605,  605,  605,  368,  605,
 /*  6980 */   338,  605,  605,  365,  362,  495,  144,  145,  147,  146,
 /*  6990 */   148,  605,  361,  363,  364,  366,  367,  332,  320,  318,
 /*  7000 */   324,  322,  326,  328,  330,  276,  282,  288,  291,  294,
 /*  7010 */   297,  285,  279,  306,  308,  316,  310,  312,  314,  605,
 /*  7020 */   605,  605,  605,  362,  605,  605,  605,  303,  299,  605,
 /*  7030 */   605,  361,  363,  364,  366,  367,  605,  150,  151,  283,
 /*  7040 */   605,  292,  295,  605,  605,   36,  605,  149,  605,  605,
 /*  7050 */   605,  605,  605,  344,  605,  605,  605,  605,  605,  605,
 /*  7060 */   277,  605,  605,  605,  605,  280,  354,  289,  359,  286,
 /*  7070 */   378,  274,  605,  369,  605,  605,  348,  605,  283,  605,
 /*  7080 */   292,  295,  605,  605,  605,  605,  385,  605,  605,  605,
 /*  7090 */   605,  368,  344,  338,  605,  605,  365,  605,  526,  277,
 /*  7100 */   605,  605,  605,  605,  280,  354,  289,  359,  286,  378,
 /*  7110 */   274,  605,  369,  605,  605,  348,  605,  605,  605,  605,
 /*  7120 */   605,  605,  605,  605,  605,  385,  605,  605,  605,  605,
 /*  7130 */   368,  605,  338,  605,  605,  365,  362,  544,  144,  145,
 /*  7140 */   147,  146,  148,  605,  361,  363,  364,  366,  367,  605,
 /*  7150 */   324,  322,  326,  328,  330,  276,  282,  288,  291,  294,
 /*  7160 */   297,  285,  279,  306,  308,  316,  310,  312,  314,  605,
 /*  7170 */   605,  605,  605,  605,  605,  362,  605,  303,  299,  605,
 /*  7180 */   605,  605,  605,  361,  363,  364,  366,  367,  605,  150,
 /*  7190 */   151,  283,  605,  292,  295,  605,  605,  201,  605,  149,
 /*  7200 */   605,  605,  605,  605,  605,  344,  605,  605,  605,  605,
 /*  7210 */   605,  605,  277,  605,  605,  605,  605,  280,  354,  289,
 /*  7220 */   359,  286,  378,  274,  447,  369,  605,  605,  352,  605,
 /*  7230 */   283,  605,  292,  295,  605,  605,  605,  605,  385,  605,
 /*  7240 */   605,  605,  605,  368,  344,  338,  605,  605,  365,  605,
 /*  7250 */   605,  277,  605,  605,  605,  605,  280,  354,  289,  359,
 /*  7260 */   286,  378,  274,  441,  369,   53,  605,  352,  605,  605,
 /*  7270 */   605,  605,  605,  605,  605,  605,  605,  385,  605,  605,
 /*  7280 */   605,  605,  368,  605,  338,  605,  605,  365,  362,  144,
 /*  7290 */   145,  147,  146,  148,  605,  605,  361,  363,  364,  366,
 /*  7300 */   367,  144,  145,  147,  146,  148,  605,   43,   49,   50,
 /*  7310 */   605,   59,   60,   61,   62,   63,   64,   65,   66,   67,
 /*  7320 */    68,   69,   70,   71,   72,  605,  605,  362,  605,  605,
 /*  7330 */   605,  605,  605,  605,  605,  361,  363,  364,  366,  367,
 /*  7340 */   150,  151,  605,  283,  605,  292,  295,  605,  695,  605,
 /*  7350 */   149,  605,  150,  151,  605,  828,  726,  344,  177,  605,
 /*  7360 */   605,  605,  149,  605,  277,  605,  605,  605,  605,  280,
 /*  7370 */   354,  289,  359,  286,  378,  274,  605,  369,  605,  605,
 /*  7380 */   348,  605,  283,  605,  292,  295,  605,  605,  605,  605,
 /*  7390 */   385,  605,  605,  605,  605,  368,  344,  338,  605,  605,
 /*  7400 */   365,  605,  605,  277,  605,  605,  605,  128,  280,  354,
 /*  7410 */   289,  359,  286,  378,  274,  605,  369,  605,  605,  348,
 /*  7420 */   573,  144,  145,  147,  146,  148,  698,  605,  605,  385,
 /*  7430 */    83,  605,  640,  605,  368,  605,  338,  605,  605,  365,
 /*  7440 */   362,  605,  605,  605,  112,  605,  605,  605,  361,  363,
 /*  7450 */   364,  366,  367,   59,   60,   61,   62,   63,   64,   65,
 /*  7460 */    66,   67,   68,   69,   70,   71,   72,  605,  605,  605,
 /*  7470 */   605,  605,  150,  151,  605,  831,  724,  605,  177,  362,
 /*  7480 */   605,  605,  149,  605,  605,  605,  605,  361,  363,  364,
 /*  7490 */   366,  367,  370,  605,  605,  283,  605,  292,  295,  605,
 /*  7500 */   605,  605,  392,  394,  393,  358,  605,  605,  605,  344,
 /*  7510 */   605,  417,  418,  421,  605,  605,  277,  605,  605,  605,
 /*  7520 */   605,  280,  354,  289,  359,  286,  378,  274,  414,  369,
 /*  7530 */   605,  605,  352,  605,  283,  605,  292,  295,  605,  605,
 /*  7540 */   605,  605,  385,  605,  605,  605,  605,  368,  344,  338,
 /*  7550 */   605,  605,  365,  605,  605,  277,  605,  605,  605,  128,
 /*  7560 */   280,  354,  289,  359,  286,  378,  274,  408,  369,  605,
 /*  7570 */   605,  352,  605,  144,  145,  147,  146,  148,   39,  605,
 /*  7580 */   605,  385,   83,  605,  605,  605,  368,  605,  338,  605,
 /*  7590 */   605,  365,  362,  605,  605,  605,  112,  605,  605,  605,
 /*  7600 */   361,  363,  364,  366,  367,   59,   60,   61,   62,   63,
 /*  7610 */    64,   65,   66,   67,   68,   69,   70,   71,   72,  605,
 /*  7620 */   605,  605,  605,  605,  150,  151,  605,  836,  171,  605,
 /*  7630 */   177,  362,  605,  605,  149,  605,  605,  605,  605,  361,
 /*  7640 */   363,  364,  366,  367,  605,  605,  605,  283,  605,  292,
 /*  7650 */   295,  605,  605,  605,  605,  605,  605,  605,  605,  605,
 /*  7660 */   605,  344,  605,  605,  605,  605,  605,  605,  277,  605,
 /*  7670 */   605,  605,  605,  280,  354,  289,  359,  286,  378,  274,
 /*  7680 */   401,  369,  605,  605,  352,  605,  283,  605,  292,  295,
 /*  7690 */   605,  605,  605,  605,  385,  605,  605,  605,  605,  368,
 /*  7700 */   344,  338,  605,  605,  365,  605,  605,  277,  605,  605,
 /*  7710 */   605,  128,  280,  354,  289,  359,  286,  378,  274,  391,
 /*  7720 */   369,  605,  605,  352,  605,  144,  145,  147,  146,  148,
 /*  7730 */   204,  605,  605,  385,   83,  605,  605,  605,  368,  605,
 /*  7740 */   338,  605,  605,  365,  362,  605,  605,  605,  112,  605,
 /*  7750 */   605,  605,  361,  363,  364,  366,  367,   59,   60,   61,
 /*  7760 */    62,   63,   64,   65,   66,   67,   68,   69,   70,   71,
 /*  7770 */    72,  605,  605,  605,  605,  605,  150,  151,  605,  838,
 /*  7780 */   152,  605,  605,  362,  605,  605,  149,  605,  605,  605,
 /*  7790 */   605,  361,  363,  364,  366,  367,  605,  605,  605,  283,
 /*  7800 */   605,  292,  295,  605,  605,  605,  605,  605,  605,  605,
 /*  7810 */   605,  605,  605,  344,  605,  605,  605,  605,  605,  605,
 /*  7820 */   277,  605,  605,  605,  605,  280,  354,  289,  359,  286,
 /*  7830 */   378,  274,  381,  369,  605,  605,  352,  605,  283,  605,
 /*  7840 */   292,  295,  605,  605,  605,  605,  385,  605,  605,  605,
 /*  7850 */   605,  368,  344,  338,  605,  605,  365,  605,  605,  277,
 /*  7860 */   605,  605,  605,  128,  280,  354,  289,  359,  286,  378,
 /*  7870 */   274,  605,  369,  605,  605,  348,  605,  144,  145,  147,
 /*  7880 */   146,  148,  157,  605,  605,  385,   83,  605,  605,  605,
 /*  7890 */   368,  605,  338,  605,  605,  365,  362,  605,  605,  605,
 /*  7900 */   112,  605,  605,  605,  361,  363,  364,  366,  367,   59,
 /*  7910 */    60,   61,   62,   63,   64,   65,   66,   67,   68,   69,
 /*  7920 */    70,   71,   72,  605,  605,  605,  605,  627,  150,  151,
 /*  7930 */   605,  829,  693,  605,  605,  362,  605,  605,  149,  605,
 /*  7940 */   605,  605,  605,  361,  363,  364,  366,  367,  605,  605,
 /*  7950 */   605,  283,  605,  292,  295,  605,  605,  605,  605,  605,
 /*  7960 */   605,  605,  605,  605,  605,  344,  605,  605,  605,  605,
 /*  7970 */   605,  605,  277,  605,  605,  605,  605,  280,  354,  289,
 /*  7980 */   359,  286,  378,  274,  605,  369,  605,  605,  348,  605,
 /*  7990 */   283,  605,  292,  295,  605,  605,  605,  605,  385,  605,
 /*  8000 */   605,  605,  605,  368,  344,  338,  605,  605,  365,  605,
 /*  8010 */   505,  277,  605,  605,  605,  128,  280,  354,  289,  359,
 /*  8020 */   286,  378,  274,  605,  369,  605,  605,  348,  605,  144,
 /*  8030 */   145,  147,  146,  148,  605,  605,  605,  385,   83,  605,
 /*  8040 */   605,  605,  368,  605,  338,  605,  605,  365,  362,  605,
 /*  8050 */   605,  605,  112,  605,  605,  605,  361,  363,  364,  366,
 /*  8060 */   367,   59,   60,   61,   62,   63,   64,   65,   66,   67,
 /*  8070 */    68,   69,   70,   71,   72,  605,  605,  605,  605,  605,
 /*  8080 */   150,  151,  605,  834,  693,  605,  605,  362,  605,  605,
 /*  8090 */   149,  605,  605,  605,  605,  361,  363,  364,  366,  367,
 /*  8100 */   605,  605,  605,  283,  605,  292,  295,  605,  605,  605,
 /*  8110 */   605,  605,  605,  605,  605,  605,  605,  344,  605,  605,
 /*  8120 */   605,  605,  605,  605,  277,  605,  605,  605,  605,  280,
 /*  8130 */   354,  289,  359,  286,  378,  274,  605,  369,  605,  605,
 /*  8140 */   352,  605,  283,  605,  292,  295,  605,  605,  605,  605,
 /*  8150 */   385,  605,  605,  605,  605,  368,  344,  338,  605,  605,
 /*  8160 */   365,  605,  605,  277,  605,  605,  605,  605,  280,  354,
 /*  8170 */   289,  359,  286,  378,  274,  605,  369,   53,  605,  371,
 /*  8180 */   605,  605,  605,  605,  605,  605,  605,  605,  605,  385,
 /*  8190 */   605,  605,  605,  605,  368,  605,  338,  605,  605,  365,
 /*  8200 */   362,  144,  145,  147,  146,  148,  605,  605,  361,  363,
 /*  8210 */   364,  366,  367,  605,  605,  605,  605,  605,  128,  605,
 /*  8220 */    49,   50,  605,   59,   60,   61,   62,   63,   64,   65,
 /*  8230 */    66,   67,   68,   69,   70,   71,   72,  793,  605,  373,
 /*  8240 */   605,  113,  605,  605,  605,  605,  605,  372,  363,  364,
 /*  8250 */   366,  367,  150,  151,  605,  832,  760,  144,  145,  147,
 /*  8260 */   146,  148,  149,  605,   59,   60,   61,   62,   63,   64,
 /*  8270 */    65,   66,   67,   68,   69,   70,   71,   72,  605,  605,
 /*  8280 */    10,   18,  605,   13,  605,   22,  605,  184,  605,  771,
 /*  8290 */   784,  605,  605,  623,  605,  144,  145,  147,  146,  148,
 /*  8300 */   605,  605,  605,  605,  605,  605,  605,   97,  150,  151,
 /*  8310 */   100,  835,  693,  144,  145,  147,  146,  148,  149,  605,
 /*  8320 */   605,  605,  605,  605,  605,   96,  605,  605,  605,  605,
 /*  8330 */    90,  605,  605,  605,  605,  605,  605,  605,  605,  605,
 /*  8340 */   605,  605,  605,  605,  605,  605,  150,  151,  605,  833,
 /*  8350 */   693,  605,  605,  605,  624,  605,  149,  605,  144,  145,
 /*  8360 */   147,  146,  148,  605,  150,  151,  605,  830,  693,  605,
 /*  8370 */    99,  605,  605,  605,  149,  605,  605,  605,   98,   87,
 /*  8380 */    89,   92,   91,  605,  605,  605,  605,  605,  605,  605,
 /*  8390 */   605,  605,  605,  605,  605,  605,  605,  605,  605,  605,
 /*  8400 */   605,  605,  605,  605,  605,  605,  605,  605,  605,  150,
 /*  8410 */   151,  605,  837,  152,  605,  605,  605,  605,  605,  149,
};
static YYCODETYPE yy_lookahead[] = {
 /*     0 */     7,   87,    9,   10,  119,  120,  121,  122,  123,  124,
 /*    10 */   125,  126,  127,  128,   21,   61,  149,  154,  155,  156,
 /*    20 */   157,   28,  155,  156,   45,  158,   33,   34,   35,   36,
 /*    30 */    37,   38,   39,   54,   41,   44,   57,   44,   45,   44,
 /*    40 */    55,   27,   28,   29,   30,   31,   32,   54,   55,   56,
 /*    50 */    55,   56,   59,   55,   61,   41,   42,   64,   41,   42,
 /*    60 */    67,   68,   69,   70,   71,   72,   73,   74,   75,   76,
 /*    70 */    77,   86,   61,   88,   81,   82,   83,  191,   85,  193,
 /*    80 */   194,  188,   89,   90,   91,   92,  138,   94,  140,  141,
 /*    90 */    97,  198,  199,  200,  201,  147,  191,  104,  205,  206,
 /*   100 */   152,  108,  109,  110,  111,  112,  113,  114,  115,  116,
 /*   110 */   117,    7,   49,    9,   10,  133,  105,   54,  136,  137,
 /*   120 */   138,   39,  140,  141,  149,   21,   44,    6,  146,  147,
 /*   130 */   155,  156,   28,  158,  152,  202,  203,   33,   34,   35,
 /*   140 */    36,   37,   38,   39,   61,   41,   57,   44,   44,   45,
 /*   150 */   122,  123,  124,  125,  126,  127,  128,   45,   54,   55,
 /*   160 */    56,   40,   59,   59,  142,   61,   54,    6,   64,   57,
 /*   170 */    87,   67,   68,   69,   70,   71,   72,   73,   74,   75,
 /*   180 */    76,   77,  202,  203,   45,   81,   82,   83,  105,   85,
 /*   190 */   202,  203,  188,   89,   90,   91,   92,   42,   94,    6,
 /*   200 */    45,   97,  198,  199,  200,  201,   45,   14,  104,  205,
 /*   210 */   206,   42,  108,  109,  110,  111,  112,  113,  114,  115,
 /*   220 */   116,  117,    7,   54,    9,   10,  151,  188,  153,  154,
 /*   230 */   155,  156,  157,  211,  212,  213,   21,  198,  199,  200,
 /*   240 */   201,    6,    6,   28,  205,  206,    6,   54,   33,   34,
 /*   250 */    35,   36,   37,   38,   39,  191,   41,  193,  194,   44,
 /*   260 */    45,    6,   42,  137,  138,   45,  140,  141,   55,   54,
 /*   270 */    55,   56,  146,  147,   59,   40,   61,  152,  152,   64,
 /*   280 */    40,   45,   67,   68,   69,   70,   71,   72,   73,   74,
 /*   290 */    75,   76,   77,  202,  203,   40,   81,   82,   83,   86,
 /*   300 */    85,   88,   44,  188,   89,   90,   91,   92,   44,   94,
 /*   310 */     6,  141,   97,  198,  199,  200,  201,  147,   54,  104,
 /*   320 */   205,  206,  152,  108,  109,  110,  111,  112,  113,  114,
 /*   330 */   115,  116,  117,    7,   44,    9,   10,  151,  188,  153,
 /*   340 */   154,  155,  156,  157,  202,  203,  141,   21,  198,  199,
 /*   350 */   200,  201,  147,    6,   28,  205,  206,  152,   54,   33,
 /*   360 */    34,   35,   36,   37,   38,   39,  188,   41,  202,  203,
 /*   370 */    44,   45,  155,  156,    6,  158,  198,  199,  200,  201,
 /*   380 */    54,   55,   56,  205,  206,   59,  191,   61,  193,  194,
 /*   390 */    64,  131,   45,   67,   68,   69,   70,   71,   72,   73,
 /*   400 */    74,   75,   76,   77,  202,  203,   44,   81,   82,   83,
 /*   410 */     6,   85,   22,   45,  188,   89,   90,   91,   92,   61,
 /*   420 */    94,  202,  203,   97,  198,  199,  200,  201,  184,  185,
 /*   430 */   104,  205,  206,   61,  108,  109,  110,  111,  112,  113,
 /*   440 */   114,  115,  116,  117,    7,   87,    9,   10,  151,  188,
 /*   450 */   153,  154,  155,  156,  157,  202,  203,  141,   21,  198,
 /*   460 */   199,  200,  201,  105,  148,   28,  205,  206,  152,   55,
 /*   470 */    33,   34,   35,   36,   37,   38,   39,  188,   41,  202,
 /*   480 */   203,   44,   45,   42,    6,    6,   45,  198,  199,  200,
 /*   490 */   201,   54,   55,   56,  205,  206,   59,   47,   61,   95,
 /*   500 */     6,   64,  202,  203,   67,   68,   69,   70,   71,   72,
 /*   510 */    73,   74,   75,   76,   77,  186,  187,   44,   81,   82,
 /*   520 */    83,    6,   85,   45,   45,  188,   89,   90,   91,   92,
 /*   530 */    61,   94,  189,  190,   97,  198,  199,  200,  201,   45,
 /*   540 */     6,  104,  205,  206,    6,  108,  109,  110,  111,  112,
 /*   550 */   113,  114,  115,  116,  117,    7,    6,    9,   10,  138,
 /*   560 */    45,  136,  141,  138,  142,  140,  141,  146,  147,   21,
 /*   570 */    44,  146,  147,  152,    6,    6,   28,  152,   40,   45,
 /*   580 */    54,   33,   34,   35,   36,   37,   38,   39,  188,   41,
 /*   590 */   155,  156,   44,   45,    6,   45,  196,  197,  198,  199,
 /*   600 */   200,  201,   54,   55,   56,  182,  183,   59,   49,   61,
 /*   610 */    51,  105,   64,   54,   45,   67,   68,   69,   70,   71,
 /*   620 */    72,   73,   74,   75,   76,   77,   44,   49,    6,   81,
 /*   630 */    82,   83,   54,   85,   66,  213,  188,   89,   90,   91,
 /*   640 */    92,   44,   94,  195,  149,   97,  198,  199,  200,  201,
 /*   650 */   155,  156,  104,  158,   66,  191,  108,  109,  110,  111,
 /*   660 */   112,  113,  114,  115,  116,  117,    7,   45,    9,   10,
 /*   670 */   149,  188,  131,    6,  141,  192,  155,  156,  188,  158,
 /*   680 */    21,  198,  199,  200,  201,  152,  104,   28,  198,  199,
 /*   690 */   200,  201,   33,   34,   35,   36,   37,   38,   39,  188,
 /*   700 */    41,   61,   51,   44,   45,   54,   44,   40,  190,  198,
 /*   710 */   199,  200,  201,   54,   55,   56,   54,  206,   59,   49,
 /*   720 */    61,   51,   42,   64,   54,   45,   67,   68,   69,   70,
 /*   730 */    71,   72,   73,   74,   75,   76,   77,   44,  124,   44,
 /*   740 */    81,   82,   83,  191,   85,  105,  194,   54,   89,   90,
 /*   750 */    91,   92,   42,   94,  141,  151,   97,  153,  154,  155,
 /*   760 */   156,  157,    6,  104,   54,  152,  131,  108,  109,  110,
 /*   770 */   111,  112,  113,  114,  115,  116,  117,    7,   44,    9,
 /*   780 */    10,  132,  188,  134,  191,   39,  192,  194,   54,  188,
 /*   790 */    44,   21,  198,  199,  200,  201,   40,    6,   28,  198,
 /*   800 */   199,  200,  201,   33,   34,   35,   36,   37,   38,   39,
 /*   810 */   209,   41,  124,   54,   44,   45,  151,  129,  153,  154,
 /*   820 */   155,  156,  157,   42,   54,   55,   56,  191,   44,   59,
 /*   830 */   194,   61,   87,  142,   64,   54,   45,   67,   68,   69,
 /*   840 */    70,   71,   72,   73,   74,   75,   76,   77,   45,    6,
 /*   850 */   105,   81,   82,   83,   22,   85,    6,   54,    6,   89,
 /*   860 */    90,   91,   92,   42,   94,    6,  151,   97,  153,  154,
 /*   870 */   155,  156,  157,   41,  104,   54,  144,  145,  108,  109,
 /*   880 */   110,  111,  112,  113,  114,  115,  116,  117,    7,   44,
 /*   890 */     9,   10,   40,  188,    6,   45,  141,  192,   55,   40,
 /*   900 */   188,   56,   21,  198,  199,  200,  201,  152,   44,   28,
 /*   910 */   198,  199,  200,  201,   33,   34,   35,   36,   37,   38,
 /*   920 */    39,  132,   41,  134,   54,   44,   45,  151,   40,  153,
 /*   930 */   154,  155,  156,  157,   45,   54,   55,   56,   44,  132,
 /*   940 */    59,  134,   61,   54,   51,   64,   45,   54,   67,   68,
 /*   950 */    69,   70,   71,   72,   73,   74,   75,   76,   77,  132,
 /*   960 */    95,  134,   81,   82,   83,   55,   85,   44,  104,    6,
 /*   970 */    89,   90,   91,   92,   57,   94,    6,  151,   97,  153,
 /*   980 */   154,  155,  156,  157,    6,  104,   57,   44,  131,  108,
 /*   990 */   109,  110,  111,  112,  113,  114,  115,  116,  117,    7,
 /*  1000 */     6,    9,   10,   40,  188,   61,   44,   57,  192,   45,
 /*  1010 */    40,  188,   44,   21,  198,  199,  200,  201,   40,  191,
 /*  1020 */    28,  198,  199,  200,  201,   33,   34,   35,   36,   37,
 /*  1030 */    38,   39,  188,   41,   40,   44,   44,   45,   45,   14,
 /*  1040 */    45,  197,  198,  199,  200,  201,   54,   55,   56,   90,
 /*  1050 */    45,   59,   54,   61,  142,   44,   64,   22,   44,   67,
 /*  1060 */    68,   69,   70,   71,   72,   73,   74,   75,   76,   77,
 /*  1070 */    61,   45,  142,   81,   82,   83,   44,   85,   54,  131,
 /*  1080 */   188,   89,   90,   91,   92,   54,   94,  191,   66,   97,
 /*  1090 */   198,  199,  200,  201,   84,  142,  104,   57,   61,   44,
 /*  1100 */   108,  109,  110,  111,  112,  113,  114,  115,  116,  117,
 /*  1110 */     7,   44,    9,   10,  131,  188,  191,   61,  143,  192,
 /*  1120 */   131,   44,  188,   55,   21,  198,  199,  200,  201,  142,
 /*  1130 */    45,   28,  198,  199,  200,  201,   33,   34,   35,   36,
 /*  1140 */    37,   38,   39,  188,   41,  143,  185,   44,   45,   61,
 /*  1150 */    45,   54,   93,  198,  199,  200,  201,   54,   55,   56,
 /*  1160 */    45,   45,   59,   22,   61,   45,  145,   64,   61,   57,
 /*  1170 */    67,   68,   69,   70,   71,   72,   73,   74,   75,   76,
 /*  1180 */    77,  191,   44,   44,   81,   82,   83,   44,   85,   87,
 /*  1190 */    45,  188,   89,   90,   91,   92,   54,   94,   45,   55,
 /*  1200 */    97,  198,  199,  200,  201,   54,   87,  104,   44,   57,
 /*  1210 */    55,  108,  109,  110,  111,  112,  113,  114,  115,  116,
 /*  1220 */   117,    7,  143,    9,   10,    6,  188,   87,  142,   39,
 /*  1230 */   192,   45,  143,  188,  191,   21,  198,  199,  200,  201,
 /*  1240 */    44,   57,   28,  198,  199,  200,  201,   33,   34,   35,
 /*  1250 */    36,   37,   38,   39,  188,   41,   45,   44,   44,   45,
 /*  1260 */   142,   44,  131,  142,  198,  199,  200,  201,   54,   55,
 /*  1270 */    56,   45,   40,   59,   45,   61,   54,   44,   64,   45,
 /*  1280 */     0,   67,   68,   69,   70,   71,   72,   73,   74,   75,
 /*  1290 */    76,   77,   61,  134,   55,   81,   82,   83,  142,   85,
 /*  1300 */    55,   45,  188,   89,   90,   91,   92,  183,   94,  142,
 /*  1310 */    87,   97,  198,  199,  200,  201,  191,   90,  104,   55,
 /*  1320 */    44,   44,  108,  109,  110,  111,  112,  113,  114,  115,
 /*  1330 */   116,  117,    7,   59,    9,   10,   66,  188,   55,   84,
 /*  1340 */   131,  192,   57,   30,   31,   32,   21,  198,  199,  200,
 /*  1350 */   201,   54,   57,   28,   41,   42,   50,   59,   33,   34,
 /*  1360 */    35,   36,   37,   38,   39,  188,   41,  130,  203,   44,
 /*  1370 */    45,   45,   61,   87,   54,  198,  199,  200,  201,   54,
 /*  1380 */    55,   56,   61,   54,   59,  187,   61,   44,   44,   64,
 /*  1390 */   191,  142,   67,   68,   69,   70,   71,   72,   73,   74,
 /*  1400 */    75,   76,   77,   54,   57,   54,   81,   82,   83,   44,
 /*  1410 */    85,   87,  130,  188,   89,   90,   91,   92,  131,   94,
 /*  1420 */   142,   87,   97,  198,  199,  200,  201,   44,   44,  104,
 /*  1430 */    50,  142,   59,  108,  109,  110,  111,  112,  113,  114,
 /*  1440 */   115,  116,  117,    7,  142,    9,   10,   54,  188,  214,
 /*  1450 */   214,  214,  192,  214,  214,  188,  214,   21,  198,  199,
 /*  1460 */   200,  201,  214,  214,   28,  198,  199,  200,  201,   33,
 /*  1470 */    34,   35,   36,   37,   38,   39,  188,   41,  214,  214,
 /*  1480 */    44,   45,  214,  214,  214,  214,  198,  199,  200,  201,
 /*  1490 */    54,   55,   56,  214,  214,   59,  214,   61,  214,  214,
 /*  1500 */    64,  214,  214,   67,   68,   69,   70,   71,   72,   73,
 /*  1510 */    74,   75,   76,   77,  214,  214,  214,   81,   82,   83,
 /*  1520 */   214,   85,  214,  214,  214,   89,   90,   91,   92,  139,
 /*  1530 */    94,  141,  214,   97,  214,  214,  146,  214,  148,  214,
 /*  1540 */   104,  214,  152,  214,  108,  109,  110,  111,  112,  113,
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
 /*  1710 */   200,  201,   54,   55,   56,  214,  214,   59,  142,   61,
 /*  1720 */   214,  214,   64,  214,  214,   67,   68,   69,   70,   71,
 /*  1730 */    72,   73,   74,   75,   76,   77,  214,  214,  214,   81,
 /*  1740 */    82,   83,  214,   85,  214,  214,  188,   89,   90,   91,
 /*  1750 */    92,  214,   94,  214,  214,   97,  198,  199,  200,  201,
 /*  1760 */   214,  214,  104,  214,  214,  214,  108,  109,  110,  111,
 /*  1770 */   112,  113,  114,  115,  116,  117,    7,  214,    9,   10,
 /*  1780 */   214,  188,  214,  214,  214,  192,  210,  211,  212,  213,
 /*  1790 */    21,  198,  199,  200,  201,  214,  214,   28,  214,  214,
 /*  1800 */   214,  214,   33,   34,   35,   36,   37,   38,   39,  188,
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
 /*  2340 */   214,  214,  214,  188,  214,   21,  198,  199,  200,  201,
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
 /*  2450 */   214,  214,  214,  214,  188,  214,   21,  198,  199,  200,
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
 /*  2660 */   114,  115,  116,  117,    7,  214,    9,   10,  214,  214,
 /*  2670 */   214,  214,  214,  214,  214,  214,  214,  214,   21,  214,
 /*  2680 */   214,  214,  214,  214,  214,   28,  214,  214,  214,  214,
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
 /*  4770 */   150,    1,    2,    3,    4,    5,  214,  214,  214,  159,
 /*  4780 */   160,  161,  162,  163,  164,  165,  166,  167,  168,  169,
 /*  4790 */   170,  171,  172,  173,  174,  175,  176,  177,  178,  179,
 /*  4800 */   180,  127,  128,  214,  214,  214,  214,  214,  188,   41,
 /*  4810 */    98,   99,  100,  101,  102,  103,  214,  214,  198,  199,
 /*  4820 */   200,  201,   52,   53,  150,   57,  214,  214,  214,  214,
 /*  4830 */   214,  214,   62,  159,  160,  161,  162,  163,  164,  165,
 /*  4840 */   166,  167,  168,  169,  170,  171,  172,  173,  174,  175,
 /*  4850 */   176,  177,  178,  179,  180,  127,  128,  214,  214,  214,
 /*  4860 */    41,  214,  188,  214,  214,  214,   98,   99,  100,  101,
 /*  4870 */   102,  103,  198,  199,  200,  201,   57,  214,  150,  214,
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
 /*  5180 */   128,  214,  214,  214,  214,  214,  188,  181,  214,  214,
 /*  5190 */   214,  214,  214,  214,  188,  214,  198,  199,  200,  201,
 /*  5200 */   214,  214,  150,  214,  198,  199,  200,  201,  214,  214,
 /*  5210 */   214,  159,  160,  161,  162,  163,  164,  165,  166,  167,
 /*  5220 */   168,  169,  170,  171,  172,  173,  174,  175,  176,  177,
 /*  5230 */   178,  179,  180,  127,  128,  214,  214,  214,  214,  214,
 /*  5240 */   188,  181,  214,  214,  214,  214,  214,  214,  188,  214,
 /*  5250 */   198,  199,  200,  201,  214,  214,  150,  214,  198,  199,
 /*  5260 */   200,  201,  214,  214,  214,  159,  160,  161,  162,  163,
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
 /*  5930 */   176,  177,  178,  179,  180,    1,    2,    3,    4,    5,
 /*  5940 */    41,   42,  188,  214,  214,  214,  214,  214,  214,  214,
 /*  5950 */   214,  214,  198,  199,  200,  201,   57,  214,  214,  214,
 /*  5960 */   214,    8,  214,  214,   11,   12,   13,   14,   15,   16,
 /*  5970 */    17,   18,   19,   20,   21,   22,   23,   24,   25,   26,
 /*  5980 */    27,   28,   29,   30,   31,   32,   52,   53,  214,   55,
 /*  5990 */    56,  214,   58,  214,   41,   42,   62,   98,   99,  100,
 /*  6000 */   101,  102,  103,  214,  105,  106,  107,   54,  214,  214,
 /*  6010 */   214,  214,  214,  214,  214,  214,    8,  214,  214,   11,
 /*  6020 */    12,   13,   14,   15,   16,   17,   18,   19,   20,   21,
 /*  6030 */    22,   23,   24,   25,   26,   27,   28,   29,   30,   31,
 /*  6040 */    32,  214,  214,  214,  214,  214,  214,  214,  214,   41,
 /*  6050 */    42,  214,  214,   45,  214,  214,  214,  214,    8,  214,
 /*  6060 */   214,   11,   12,   13,   14,   15,   16,   17,   18,   19,
 /*  6070 */    20,   21,   22,   23,   24,   25,   26,   27,   28,   29,
 /*  6080 */    30,   31,   32,  214,  214,  214,  214,  214,  214,  214,
 /*  6090 */   214,   41,   42,  214,  214,   45,  214,  214,  214,  214,
 /*  6100 */     8,  214,  214,   11,   12,   13,   14,   15,   16,   17,
 /*  6110 */    18,   19,   20,   21,   22,   23,   24,   25,   26,   27,
 /*  6120 */    28,   29,   30,   31,   32,  214,  214,  214,  214,  214,
 /*  6130 */   214,  214,    8,   41,   42,   11,   12,   13,   14,   15,
 /*  6140 */    16,   17,   18,   19,   20,   21,   22,   23,   24,   25,
 /*  6150 */    26,   27,   28,   29,   30,   31,   32,   41,   66,  214,
 /*  6160 */   214,  214,  214,  214,  188,   41,   42,  214,  214,  214,
 /*  6170 */   214,  214,  214,   57,  198,  199,  200,  201,   54,  214,
 /*  6180 */   204,  214,  214,  207,  208,  209,  214,    8,  214,  214,
 /*  6190 */    11,   12,   13,   14,   15,   16,   17,   18,   19,   20,
 /*  6200 */    21,   22,   23,   24,   25,   26,   27,   28,   29,   30,
 /*  6210 */    31,   32,  214,  214,   98,   99,  100,  101,  102,  103,
 /*  6220 */    41,   42,  106,  107,   45,  214,  214,  214,  214,    8,
 /*  6230 */   214,  214,   11,   12,   13,   14,   15,   16,   17,   18,
 /*  6240 */    19,   20,   21,   22,   23,   24,   25,   26,   27,   28,
 /*  6250 */    29,   30,   31,   32,  214,    1,    2,    3,    4,    5,
 /*  6260 */   214,    8,   41,   42,   11,   12,   13,   14,   15,   16,
 /*  6270 */    17,   18,   19,   20,   21,   22,   23,   24,   25,   26,
 /*  6280 */    27,   28,   29,   30,   31,   32,  214,  214,  214,  214,
 /*  6290 */   214,  214,  214,  214,   41,   42,    1,    2,    3,    4,
 /*  6300 */     5,    1,    2,    3,    4,    5,   52,   53,   87,   55,
 /*  6310 */    56,  214,   58,  214,  214,  214,   62,    8,  214,   66,
 /*  6320 */    11,   12,   13,   14,   15,   16,   17,   18,   19,   20,
 /*  6330 */    21,   22,   23,   24,   25,   26,   27,   28,   29,   30,
 /*  6340 */    31,   32,  214,  214,  214,  214,  214,   52,   53,  214,
 /*  6350 */    41,   42,   52,   53,  214,   60,  214,   62,   58,  214,
 /*  6360 */   214,  214,   62,   54,  214,  214,  214,  214,  214,  214,
 /*  6370 */   214,  214,    8,  214,  214,   11,   12,   13,   14,   15,
 /*  6380 */    16,   17,   18,   19,   20,   21,   22,   23,   24,   25,
 /*  6390 */    26,   27,   28,   29,   30,   31,   32,  214,    1,    2,
 /*  6400 */     3,    4,    5,  214,  214,   41,   42,  214,  214,   45,
 /*  6410 */   214,  214,  214,  214,    8,  214,  214,   11,   12,   13,
 /*  6420 */    14,   15,   16,   17,   18,   19,   20,   21,   22,   23,
 /*  6430 */    24,   25,   26,   27,   28,   29,   30,   31,   32,  214,
 /*  6440 */   214,  214,  214,  214,  214,  214,  214,   41,   42,   52,
 /*  6450 */    53,  214,   55,   56,  214,   58,  214,  214,  214,   62,
 /*  6460 */    54,  214,  214,  214,  214,  214,  214,  214,  214,    8,
 /*  6470 */   214,  214,   11,   12,   13,   14,   15,   16,   17,   18,
 /*  6480 */    19,   20,   21,   22,   23,   24,   25,   26,   27,   28,
 /*  6490 */    29,   30,   31,   32,    7,  214,    9,   10,  214,  214,
 /*  6500 */   214,   40,   41,   42,  214,  214,  214,  214,   21,  214,
 /*  6510 */   214,  214,  214,  214,  214,   28,  214,  214,  214,  214,
 /*  6520 */    33,   34,   35,   36,   37,   38,   39,  214,   41,  214,
 /*  6530 */   214,   44,  214,  214,  214,  214,  214,  214,  214,  214,
 /*  6540 */   214,   54,  214,  214,  214,  214,   59,  214,   61,  214,
 /*  6550 */   214,   64,  214,   66,  214,    8,  214,  214,   11,   12,
 /*  6560 */    13,   14,   15,   16,   17,   18,   19,   20,   21,   22,
 /*  6570 */    23,   24,   25,   26,   27,   28,   29,   30,   31,   32,
 /*  6580 */   214,  214,  214,    7,  214,    9,   10,  214,   41,   42,
 /*  6590 */   214,  104,  214,  214,  214,  214,  214,   21,  214,  112,
 /*  6600 */   113,  114,  115,  116,   28,  214,  214,  214,  214,   33,
 /*  6610 */    34,   35,   36,   37,   38,   39,   40,   41,  214,  214,
 /*  6620 */    44,  214,    7,  214,    9,   10,  214,  214,  214,  214,
 /*  6630 */    54,  214,  214,  214,  214,   59,   21,   61,  214,  214,
 /*  6640 */    64,  214,  214,   28,  214,  214,  214,  214,   33,   34,
 /*  6650 */    35,   36,   37,   38,   39,   40,   41,  214,  214,   44,
 /*  6660 */   135,  214,  137,  214,  139,  214,  141,  214,  214,   54,
 /*  6670 */   214,  146,  214,  148,   59,  214,   61,  152,  214,   64,
 /*  6680 */   104,    1,    2,    3,    4,    5,  214,  214,  112,  113,
 /*  6690 */   114,  115,  116,   12,   13,   14,   15,   16,   17,   18,
 /*  6700 */    19,   20,   21,   22,   23,   24,   25,   26,   27,   28,
 /*  6710 */    29,   30,   31,   32,  214,  214,  214,  214,  214,  104,
 /*  6720 */   214,  214,   41,   42,   44,  214,  214,  112,  113,  114,
 /*  6730 */   115,  116,   52,   53,  214,    7,  214,    9,   10,  214,
 /*  6740 */    60,  214,   62,  214,  214,   43,   44,  214,   46,   21,
 /*  6750 */    48,  214,   50,  214,   52,   53,   28,  214,   56,  214,
 /*  6760 */   214,   33,   34,   35,   36,   37,   38,   39,  214,   41,
 /*  6770 */   214,  214,   44,  214,    7,  214,    9,   10,  214,  214,
 /*  6780 */   214,  214,   54,  214,  214,  214,  214,   59,   21,   61,
 /*  6790 */   214,  214,   64,  214,   66,   28,  214,  214,  214,  214,
 /*  6800 */    33,   34,   35,   36,   37,   38,   39,   40,   41,  214,
 /*  6810 */   214,   44,  214,  214,  214,  214,  214,  214,  214,  117,
 /*  6820 */   214,   54,  214,  214,  214,  214,   59,  214,   61,  214,
 /*  6830 */   214,   64,  104,  214,    1,    2,    3,    4,    5,  214,
 /*  6840 */   112,  113,  114,  115,  116,   13,   14,   15,   16,   17,
 /*  6850 */    18,   19,   20,   21,   22,   23,   24,   25,   26,   27,
 /*  6860 */    28,   29,   30,   31,   32,  214,  214,  214,  214,  214,
 /*  6870 */   214,  104,  214,   41,   42,  214,  214,   44,  214,  112,
 /*  6880 */   113,  114,  115,  116,  214,   52,   53,    7,  214,    9,
 /*  6890 */    10,  214,  214,   60,  214,   62,  214,  214,  214,  214,
 /*  6900 */   214,   21,  214,  214,  214,  214,  214,  214,   28,  214,
 /*  6910 */   214,  214,  214,   33,   34,   35,   36,   37,   38,   39,
 /*  6920 */   214,   41,  214,  214,   44,  214,    7,  214,    9,   10,
 /*  6930 */   214,  214,  214,  214,   54,  214,  214,  214,  214,   59,
 /*  6940 */    21,   61,  214,  214,   64,  214,   66,   28,  214,  214,
 /*  6950 */   214,  214,   33,   34,   35,   36,   37,   38,   39,  214,
 /*  6960 */    41,  214,  214,   44,  214,  214,  214,  214,  214,  214,
 /*  6970 */   214,  214,  214,   54,  214,  214,  214,  214,   59,  214,
 /*  6980 */    61,  214,  214,   64,  104,   66,    1,    2,    3,    4,
 /*  6990 */     5,  214,  112,  113,  114,  115,  116,   11,   12,   13,
 /*  7000 */    14,   15,   16,   17,   18,   19,   20,   21,   22,   23,
 /*  7010 */    24,   25,   26,   27,   28,   29,   30,   31,   32,  214,
 /*  7020 */   214,  214,  214,  104,  214,  214,  214,   41,   42,  214,
 /*  7030 */   214,  112,  113,  114,  115,  116,  214,   52,   53,    7,
 /*  7040 */   214,    9,   10,  214,  214,   60,  214,   62,  214,  214,
 /*  7050 */   214,  214,  214,   21,  214,  214,  214,  214,  214,  214,
 /*  7060 */    28,  214,  214,  214,  214,   33,   34,   35,   36,   37,
 /*  7070 */    38,   39,  214,   41,  214,  214,   44,  214,    7,  214,
 /*  7080 */     9,   10,  214,  214,  214,  214,   54,  214,  214,  214,
 /*  7090 */   214,   59,   21,   61,  214,  214,   64,  214,   66,   28,
 /*  7100 */   214,  214,  214,  214,   33,   34,   35,   36,   37,   38,
 /*  7110 */    39,  214,   41,  214,  214,   44,  214,  214,  214,  214,
 /*  7120 */   214,  214,  214,  214,  214,   54,  214,  214,  214,  214,
 /*  7130 */    59,  214,   61,  214,  214,   64,  104,   66,    1,    2,
 /*  7140 */     3,    4,    5,  214,  112,  113,  114,  115,  116,  214,
 /*  7150 */    14,   15,   16,   17,   18,   19,   20,   21,   22,   23,
 /*  7160 */    24,   25,   26,   27,   28,   29,   30,   31,   32,  214,
 /*  7170 */   214,  214,  214,  214,  214,  104,  214,   41,   42,  214,
 /*  7180 */   214,  214,  214,  112,  113,  114,  115,  116,  214,   52,
 /*  7190 */    53,    7,  214,    9,   10,  214,  214,   60,  214,   62,
 /*  7200 */   214,  214,  214,  214,  214,   21,  214,  214,  214,  214,
 /*  7210 */   214,  214,   28,  214,  214,  214,  214,   33,   34,   35,
 /*  7220 */    36,   37,   38,   39,   40,   41,  214,  214,   44,  214,
 /*  7230 */     7,  214,    9,   10,  214,  214,  214,  214,   54,  214,
 /*  7240 */   214,  214,  214,   59,   21,   61,  214,  214,   64,  214,
 /*  7250 */   214,   28,  214,  214,  214,  214,   33,   34,   35,   36,
 /*  7260 */    37,   38,   39,   40,   41,   21,  214,   44,  214,  214,
 /*  7270 */   214,  214,  214,  214,  214,  214,  214,   54,  214,  214,
 /*  7280 */   214,  214,   59,  214,   61,  214,  214,   64,  104,    1,
 /*  7290 */     2,    3,    4,    5,  214,  214,  112,  113,  114,  115,
 /*  7300 */   116,    1,    2,    3,    4,    5,  214,   63,   64,   65,
 /*  7310 */   214,   67,   68,   69,   70,   71,   72,   73,   74,   75,
 /*  7320 */    76,   77,   78,   79,   80,  214,  214,  104,  214,  214,
 /*  7330 */   214,  214,  214,  214,  214,  112,  113,  114,  115,  116,
 /*  7340 */    52,   53,  214,    7,  214,    9,   10,  214,   60,  214,
 /*  7350 */    62,  214,   52,   53,  214,   55,   56,   21,   58,  214,
 /*  7360 */   214,  214,   62,  214,   28,  214,  214,  214,  214,   33,
 /*  7370 */    34,   35,   36,   37,   38,   39,  214,   41,  214,  214,
 /*  7380 */    44,  214,    7,  214,    9,   10,  214,  214,  214,  214,
 /*  7390 */    54,  214,  214,  214,  214,   59,   21,   61,  214,  214,
 /*  7400 */    64,  214,  214,   28,  214,  214,  214,   21,   33,   34,
 /*  7410 */    35,   36,   37,   38,   39,  214,   41,  214,  214,   44,
 /*  7420 */    45,    1,    2,    3,    4,    5,   40,  214,  214,   54,
 /*  7430 */    44,  214,   96,  214,   59,  214,   61,  214,  214,   64,
 /*  7440 */   104,  214,  214,  214,   58,  214,  214,  214,  112,  113,
 /*  7450 */   114,  115,  116,   67,   68,   69,   70,   71,   72,   73,
 /*  7460 */    74,   75,   76,   77,   78,   79,   80,  214,  214,  214,
 /*  7470 */   214,  214,   52,   53,  214,   55,   56,  214,   58,  104,
 /*  7480 */   214,  214,   62,  214,  214,  214,  214,  112,  113,  114,
 /*  7490 */   115,  116,  188,  214,  214,    7,  214,    9,   10,  214,
 /*  7500 */   214,  214,  198,  199,  200,  201,  214,  214,  214,   21,
 /*  7510 */   214,  207,  208,  209,  214,  214,   28,  214,  214,  214,
 /*  7520 */   214,   33,   34,   35,   36,   37,   38,   39,   40,   41,
 /*  7530 */   214,  214,   44,  214,    7,  214,    9,   10,  214,  214,
 /*  7540 */   214,  214,   54,  214,  214,  214,  214,   59,   21,   61,
 /*  7550 */   214,  214,   64,  214,  214,   28,  214,  214,  214,   21,
 /*  7560 */    33,   34,   35,   36,   37,   38,   39,   40,   41,  214,
 /*  7570 */   214,   44,  214,    1,    2,    3,    4,    5,   40,  214,
 /*  7580 */   214,   54,   44,  214,  214,  214,   59,  214,   61,  214,
 /*  7590 */   214,   64,  104,  214,  214,  214,   58,  214,  214,  214,
 /*  7600 */   112,  113,  114,  115,  116,   67,   68,   69,   70,   71,
 /*  7610 */    72,   73,   74,   75,   76,   77,   78,   79,   80,  214,
 /*  7620 */   214,  214,  214,  214,   52,   53,  214,   55,   56,  214,
 /*  7630 */    58,  104,  214,  214,   62,  214,  214,  214,  214,  112,
 /*  7640 */   113,  114,  115,  116,  214,  214,  214,    7,  214,    9,
 /*  7650 */    10,  214,  214,  214,  214,  214,  214,  214,  214,  214,
 /*  7660 */   214,   21,  214,  214,  214,  214,  214,  214,   28,  214,
 /*  7670 */   214,  214,  214,   33,   34,   35,   36,   37,   38,   39,
 /*  7680 */    40,   41,  214,  214,   44,  214,    7,  214,    9,   10,
 /*  7690 */   214,  214,  214,  214,   54,  214,  214,  214,  214,   59,
 /*  7700 */    21,   61,  214,  214,   64,  214,  214,   28,  214,  214,
 /*  7710 */   214,   21,   33,   34,   35,   36,   37,   38,   39,   40,
 /*  7720 */    41,  214,  214,   44,  214,    1,    2,    3,    4,    5,
 /*  7730 */    40,  214,  214,   54,   44,  214,  214,  214,   59,  214,
 /*  7740 */    61,  214,  214,   64,  104,  214,  214,  214,   58,  214,
 /*  7750 */   214,  214,  112,  113,  114,  115,  116,   67,   68,   69,
 /*  7760 */    70,   71,   72,   73,   74,   75,   76,   77,   78,   79,
 /*  7770 */    80,  214,  214,  214,  214,  214,   52,   53,  214,   55,
 /*  7780 */    56,  214,  214,  104,  214,  214,   62,  214,  214,  214,
 /*  7790 */   214,  112,  113,  114,  115,  116,  214,  214,  214,    7,
 /*  7800 */   214,    9,   10,  214,  214,  214,  214,  214,  214,  214,
 /*  7810 */   214,  214,  214,   21,  214,  214,  214,  214,  214,  214,
 /*  7820 */    28,  214,  214,  214,  214,   33,   34,   35,   36,   37,
 /*  7830 */    38,   39,   40,   41,  214,  214,   44,  214,    7,  214,
 /*  7840 */     9,   10,  214,  214,  214,  214,   54,  214,  214,  214,
 /*  7850 */   214,   59,   21,   61,  214,  214,   64,  214,  214,   28,
 /*  7860 */   214,  214,  214,   21,   33,   34,   35,   36,   37,   38,
 /*  7870 */    39,  214,   41,  214,  214,   44,  214,    1,    2,    3,
 /*  7880 */     4,    5,   40,  214,  214,   54,   44,  214,  214,  214,
 /*  7890 */    59,  214,   61,  214,  214,   64,  104,  214,  214,  214,
 /*  7900 */    58,  214,  214,  214,  112,  113,  114,  115,  116,   67,
 /*  7910 */    68,   69,   70,   71,   72,   73,   74,   75,   76,   77,
 /*  7920 */    78,   79,   80,  214,  214,  214,  214,   96,   52,   53,
 /*  7930 */   214,   55,   56,  214,  214,  104,  214,  214,   62,  214,
 /*  7940 */   214,  214,  214,  112,  113,  114,  115,  116,  214,  214,
 /*  7950 */   214,    7,  214,    9,   10,  214,  214,  214,  214,  214,
 /*  7960 */   214,  214,  214,  214,  214,   21,  214,  214,  214,  214,
 /*  7970 */   214,  214,   28,  214,  214,  214,  214,   33,   34,   35,
 /*  7980 */    36,   37,   38,   39,  214,   41,  214,  214,   44,  214,
 /*  7990 */     7,  214,    9,   10,  214,  214,  214,  214,   54,  214,
 /*  8000 */   214,  214,  214,   59,   21,   61,  214,  214,   64,  214,
 /*  8010 */    66,   28,  214,  214,  214,   21,   33,   34,   35,   36,
 /*  8020 */    37,   38,   39,  214,   41,  214,  214,   44,  214,    1,
 /*  8030 */     2,    3,    4,    5,  214,  214,  214,   54,   44,  214,
 /*  8040 */   214,  214,   59,  214,   61,  214,  214,   64,  104,  214,
 /*  8050 */   214,  214,   58,  214,  214,  214,  112,  113,  114,  115,
 /*  8060 */   116,   67,   68,   69,   70,   71,   72,   73,   74,   75,
 /*  8070 */    76,   77,   78,   79,   80,  214,  214,  214,  214,  214,
 /*  8080 */    52,   53,  214,   55,   56,  214,  214,  104,  214,  214,
 /*  8090 */    62,  214,  214,  214,  214,  112,  113,  114,  115,  116,
 /*  8100 */   214,  214,  214,    7,  214,    9,   10,  214,  214,  214,
 /*  8110 */   214,  214,  214,  214,  214,  214,  214,   21,  214,  214,
 /*  8120 */   214,  214,  214,  214,   28,  214,  214,  214,  214,   33,
 /*  8130 */    34,   35,   36,   37,   38,   39,  214,   41,  214,  214,
 /*  8140 */    44,  214,    7,  214,    9,   10,  214,  214,  214,  214,
 /*  8150 */    54,  214,  214,  214,  214,   59,   21,   61,  214,  214,
 /*  8160 */    64,  214,  214,   28,  214,  214,  214,  214,   33,   34,
 /*  8170 */    35,   36,   37,   38,   39,  214,   41,   21,  214,   44,
 /*  8180 */   214,  214,  214,  214,  214,  214,  214,  214,  214,   54,
 /*  8190 */   214,  214,  214,  214,   59,  214,   61,  214,  214,   64,
 /*  8200 */   104,    1,    2,    3,    4,    5,  214,  214,  112,  113,
 /*  8210 */   114,  115,  116,  214,  214,  214,  214,  214,   21,  214,
 /*  8220 */    64,   65,  214,   67,   68,   69,   70,   71,   72,   73,
 /*  8230 */    74,   75,   76,   77,   78,   79,   80,    0,  214,  104,
 /*  8240 */   214,   44,  214,  214,  214,  214,  214,  112,  113,  114,
 /*  8250 */   115,  116,   52,   53,  214,   55,   56,    1,    2,    3,
 /*  8260 */     4,    5,   62,  214,   67,   68,   69,   70,   71,   72,
 /*  8270 */    73,   74,   75,   76,   77,   78,   79,   80,  214,  214,
 /*  8280 */    43,   44,  214,   46,  214,   48,  214,   50,  214,   52,
 /*  8290 */    53,  214,  214,   56,  214,    1,    2,    3,    4,    5,
 /*  8300 */   214,  214,  214,  214,  214,  214,  214,   41,   52,   53,
 /*  8310 */    44,   55,   56,    1,    2,    3,    4,    5,   62,  214,
 /*  8320 */   214,  214,  214,  214,  214,   59,  214,  214,  214,  214,
 /*  8330 */    64,  214,  214,  214,  214,  214,  214,  214,  214,  214,
 /*  8340 */   214,  214,  214,  214,  214,  214,   52,   53,  214,   55,
 /*  8350 */    56,  214,  214,  214,  117,  214,   62,  214,    1,    2,
 /*  8360 */     3,    4,    5,  214,   52,   53,  214,   55,   56,  214,
 /*  8370 */   104,  214,  214,  214,   62,  214,  214,  214,  112,  113,
 /*  8380 */   114,  115,  116,  214,  214,  214,  214,  214,  214,  214,
 /*  8390 */   214,  214,  214,  214,  214,  214,  214,  214,  214,  214,
 /*  8400 */   214,  214,  214,  214,  214,  214,  214,  214,  214,   52,
 /*  8410 */    53,  214,   55,   56,  214,  214,  214,  214,  214,   62,
};
#define YY_SHIFT_USE_DFLT (-87)
static short yy_shift_ofst[] = {
 /*     0 */  6702, 1280, 8237,  -87,  -87,  -87,  -87,  -87,  -87,  -87,
 /*    10 */  1014, 1005,  -87,  695,  791,  -87,  695,  -87,  450,  258,
 /*    20 */   -87,  -87, 1383,  578,  -87, 1343, 1320,  -87, 5934,  -87,
 /*    30 */  1264,  -87, 7572, 7724,  -87, 6985,  473,  469, 7538,  441,
 /*    40 */  7244,  901,  -87,  -87, 1025, 8156,  -87, 1190,  -87,  -87,
 /*    50 */   -87,  -87,  -87, 1138,  832,  -87, 1022, 1035,  -87,  -87,
 /*    60 */   -87,  -87,  -87,  -87,  -87,  -87,  -87,  -87,  -87,  -87,
 /*    70 */   -87,  -87,  -87,  -87,  -87,  240,  220, 7244, 1234,  -87,
 /*    80 */   -87, 7994,  -87,   89, 5792,  -87,  -87,  -87,  -87,  -87,
 /*    90 */   -87,  -87,  -87,  506, 1274,  -87,  -87, 4645, 1119, 1102,
 /*   100 */   745,  -87,  588,  -87, 8266,  -87, 1286, 5792,  -87,  -87,
 /*   110 */   -87,  -87, 8197,  917, 5792,  -87,   82, 1112, 5792,  -87,
 /*   120 */  1365,  929, 5792,  -87, 1213, 1152, 5792,  -87,  968,  390,
 /*   130 */   746, 1347, 5792,  -87, 1077, 1184, 5792,  -87, 1276,  950,
 /*   140 */  5792,  -87,  -87,  -87,  -87,  -87,  -87,  -87,  -87,  -87,
 /*   150 */   -87,  -87, 4770, 6295,  962, 1056, 7842,  155, 7244,  964,
 /*   160 */   -87,  -87,  970,  680, 7244,  993,  -87,  -87,  -87,  -87,
 /*   170 */   -87, 6300, 1298, 1040, 5792, 1226,  -87, 1373, 1295, 5792,
 /*   180 */  1026,  -87, 8357,  -87, 1217,  559,  -87, 1344,  651,  -87,
 /*   190 */  1011,  304,  -87, 6254,  -87, 1144,  -87, 6397, 8312,  -87,
 /*   200 */  7137, 1143, 1009, 7690,  169,  215,  -87, 1214,  -87,  -87,
 /*   210 */   -87,  -87,  -87,  -87,  -87,  -87,  -87,  -87,  -87,  -87,
 /*   220 */   -87,  -87,  -87,  -87,  -87,  -87,  -87,  -87,  -87,  -87,
 /*   230 */   -87,  -87,  -87,  -87, 1229,  -87, 1085,  -87, 7983, 1097,
 /*   240 */  3545, 2768, 1010, 1024, 1769,   -7,  -87, 7983, 1349,  213,
 /*   250 */   -87,  -15,  -87,  -87, 5792, 1324, 4433, 4433,  759, 1325,
 /*   260 */  2546,  -87, 7983, 1142, 2324, 3767,  -87, 1222, 3656, 3101,
 /*   270 */   959, 7983, 1145,  -87, 7983,   17, 7983, 7983, 1313, 7983,
 /*   280 */  7983,   17, 7983, 7983, 6547, 7983, 7983,   17, 7983, 7983,
 /*   290 */    17, 7983, 7983, 6986, 7983, 7983, 6986, 7983,   14,  526,
 /*   300 */  1311, 6767, 6547, 7983, 6092,  -87, 7983, 1313, 7983, 1313,
 /*   310 */  7983,   17, 7983,   17, 7983,   17, 7983, 1313, 7983, 7136,
 /*   320 */  7983, 6832, 7983, 4612, 7983, 4612, 7983, 4612, 7983, 4612,
 /*   330 */  7983, 4612, 7983, 6681, 7983, 6221, 7983, 6547, 4544, 6461,
 /*   340 */   -87, 1232, 7983,   17, 1233, 1141, 7983,   14,   11,  103,
 /*   350 */  1231, 6615,   83, 6576, 1196, 1219, 7983,   17,  -87, 7983,
 /*   360 */    17,  -87,  -87,  -87,  -87,  -87,  -87,  -87,  -87, 6487,
 /*   370 */  6547,  358,  -86, 1334,  -87,  568,  -87, 8135,  662, 1321,
 /*   380 */  7792,  -87,  255,  -87, 8096, 1277, 1283,  640, 7679,  538,
 /*   390 */   -87,  -87,  -87,  -87,  -87,  -87,  734,  -46, 7640,  756,
 /*   400 */   -87,  -87,  894,  910,  944, 7527,  859,  -87,  -87,  -87,
 /*   410 */  1055, 1068, 1088, 7488,  -87,  888,  -87,  -87, 1140, 7983,
 /*   420 */   -87,  -87,  -87,  963,  -87,  -87, 7983, 6547,  978,  -87,
 /*   430 */   -87,  -87,  994,  -87,  -87,  864, 1245, 1037, 7223,  121,
 /*   440 */   -87,  -87, 1239, 1107, 7184,  235,  -87,  -87,   14,   14,
 /*   450 */    14,   14,   14,   14,   14, 6547, 1151, 4211,  548, 1059,
 /*   460 */  1059,  -87, 1067,  193,  881, 1436,  -87, 1032,  404, 7831,
 /*   470 */  6124, 2990, 2879,  -87,  264,  534,  -87,  264,  -87, 5899,
 /*   480 */   -87,  -87,  -87,  -87,  -87,  -87,  -87, 7983,  -87, 6547,
 /*   490 */   693, 6116, 7983,  -87, 6919, 4927, 7983,  -87, 1270,  -87,
 /*   500 */  6253, 4768, 7983,  -87, 7944, 4927, 7983,  -87,  -87,  -87,
 /*   510 */   -87,  -87,  582,   -2, 4927, 7983,  -87,  414, 4927, 7983,
 /*   520 */   -87,  784, 4819, 7983,  -87, 7032, 4927, 7983,  -87, 4712,
 /*   530 */  7983,  -87, 6880, 4927, 7983,  -87, 6728, 4927, 7983,  -87,
 /*   540 */  4873, 7983,  -87, 7071, 4927, 7983,  -87,  -87,  -87, 1164,
 /*   550 */  1155, 4927, 7983,  -87,  -87, 7983,  850,  -87, 7983,  -87,
 /*   560 */  6547,  -87, 1326,  -87,  139,  -87,  995,  -87, 1115,  -87,
 /*   570 */  7375, 6050,  -87,  -87, 7983, 6364,  -87, 7983, 6008,  -87,
 /*   580 */  7983, 6179,  -87,  290,  161,  -87,  290,  -87, 1285, 5792,
 /*   590 */   -87,  -87,  290,  236,  -87,  290,  347,  -87,  290,  368,
 /*   600 */   -87,  290,  478,  -87,  290,  479,  -87,  290,  494,  -87,
 /*   610 */   290,  515,  -87,  290,  550,  -87,  290,  569,  -87,  290,
 /*   620 */   622,  -87, 6547,  -87,  -87,  -87,  -87, 7983, 6309, 4433,
 /*   630 */  4100,  -87,  943,  865, 7336, 6406, 3989, 3878,  -87,  -87,
 /*   640 */  7983, 5953, 4433, 3434,  -87,  -87, 1384, 1351, 3323, 3212,
 /*   650 */   -87,  -87, 1067,  -87,  -87,  -87,  -87,  -87, 1227, 7983,
 /*   660 */  1256,  -87,  -87,  -87, 1223, 4433, 4433,  -87,  -87, 1255,
 /*   670 */  1297, 2657, 2435,  -87,  -87, 7244, 1329, 2213,  -87, 2102,
 /*   680 */   -87,  667,  710, 1991,  -87, 1880,  -87, 7244, 1393, 1658,
 /*   690 */   -87, 1547,  -87, 4770, 7288,  362,  372, 7386,  781, 1103,
 /*   700 */   -87,  992,  -87, 7244,  870,  770,  -87,  659,  -87,  852,
 /*   710 */   821,  437,  -87,  326,  -87, 7244,  998,  104,  -87, 4322,
 /*   720 */   -87, 7420, 8028,  -87, 6300,  -87, 6300, 6680,  112,  -87,
 /*   730 */  5792,  803,  -87, 1105,  -87,   -5, 1116,  843, 1120,  845,
 /*   740 */   -87,  -87, 1139,  -87,  -87, 1153,  -87, 6833,  -21,  -87,
 /*   750 */  5792,  889,  -87, 1186,  -87, 1211,  -87, 7300, 8200, 8256,
 /*   760 */  4770, 8294,  -87, 7876, 1011,  -87,  -87,  -87, 1011,  304,
 /*   770 */   -87, 1380,   -9,  670,  -87,  923,  893,  -87, 1011,  304,
 /*   780 */   -87, 1011,  304,  -87, 1306,  991,   63,  -87,  597, 1031,
 /*   790 */   -87,  -87,
};
#define YY_REDUCE_USE_DFLT (-138)
static short yy_reduce_ofst[] = {
 /*     0 */  -115, -138,   28, -138, -138, -138, -138, -138, -138, -138,
 /*    10 */  -138, -138, -138,  688, -138, -138,  614, -138, -138, -138,
 /*    20 */  -138, -138, -138, 1282, -138, -138, 1237, -138, 6525, -138,
 /*    30 */  -138, -138, 1390,  316, -138,  125, -138, -138,  521, -138,
 /*    40 */   604, -138, -138, -138, -138, -137, -138, -138, -138, -138,
 /*    50 */  -138, -138, -138, -138, -138, -138, -138, -138, -138, -138,
 /*    60 */  -138, -138, -138, -138, -138, -138, -138, -138, -138, -138,
 /*    70 */  -138, -138, -138, -138, -138, -138, -138,  186, -138, -138,
 /*    80 */  -138,  217, -138, -138, 1167, -138, -138, -138, -138, -138,
 /*    90 */  -138, -138, -138, -138, -138, -138, -138, 1576, -138, -138,
 /*   100 */  -138, -138, -138, -138,   22, -138, -138,  422, -138, -138,
 /*   110 */  -138, -138,  435, -138,  912, -138, -138, -138, 1121, -138,
 /*   120 */  -138, -138, 1289, -138, -138, -138,  953, -138, -138, -138,
 /*   130 */  -138, -138,  691, -138, -138, -138, 1278, -138, -138, -138,
 /*   140 */  1302, -138, -138, -138, -138, -138, -138, -138, -138, -138,
 /*   150 */  -138, -138,  533,  125, -138, -138, -133, -138,  715, -138,
 /*   160 */  -138, -138, -138, -138,  776, -138, -138, -138, -138, -138,
 /*   170 */  -138,  533, -138, -138, 1118, -138, -138, -138, -138,  930,
 /*   180 */  -138, -138,  316, -138, -138,  948, -138, -138, 1209, -138,
 /*   190 */   649,  635, -138,  -18, -138, -138, -138,  126,  170, -138,
 /*   200 */   125, -138, -138,  -25, -138, 5430, -138, 5754, -138, -138,
 /*   210 */  -138, -138, -138, -138, -138, -138, -138, -138, -138, -138,
 /*   220 */  -138, -138, -138, -138, -138, -138, -138, -138, -138, -138,
 /*   230 */  -138, -138, -138, -138, -138, -138, -138, -138, 5060, -138,
 /*   240 */  5700, 5754, -138, -138, 5592, 5754, -138, 4952, -138,  423,
 /*   250 */  -138, 1124, -138, -138, 1249, -138, 5322, 5754, -138, 5214,
 /*   260 */  5754, -138, 4898, -138, 4620, 5754, -138, -138, 5538, 5754,
 /*   270 */  -138, 5006, -138, -138, 1621, -138, 1780, 1954, -138, 1378,
 /*   280 */  1510, -138, 1933, 2065, -138, 2155, 2224, -138, 2335, 2370,
 /*   290 */  -138, 2398,  823, -138, 1045, 1177, -138, 1267, -138, -138,
 /*   300 */  -138, -107, -138, 1489, -138, -138, 1288, -138, 1225, -138,
 /*   310 */  1114, -138, 1066, -138, 1003, -138,  712, -138, 2287, -138,
 /*   320 */  2176, -138, 2002, -138, 1711, -138, 1669, -138,  892, -138,
 /*   330 */  2377, -138, 2044, -138, 1732, -138, 1558, -138, 4333, -138,
 /*   340 */  -138, -138, 1447, -138, -138, -138,  955, -138, -138, -138,
 /*   350 */  -138,    4, -138,   39, -138, -138, 1822, -138, -138, 2113,
 /*   360 */  -138, -138, -138, -138, -138, -138, -138, -138, -138, 5976,
 /*   370 */  -138, -138, -138, -138, -138, -138, -138, 7304, -138, -138,
 /*   380 */   115, -138, -138, -138,  511, -138, -138, -138,  150, -138,
 /*   390 */  -138, -138, -138, -138, -138, -138, -138, -138,  178, -138,
 /*   400 */  -138, -138, -138, -138, -138,  226, -138, -138, -138, -138,
 /*   410 */  -138, -138, -138,  261, -138, -138, -138, -138, -138,  601,
 /*   420 */  -138, -138, -138, -138, -138, -138, 1891, -138, -138, -138,
 /*   430 */  -138, -138, -138, -138, -138, -138, -138, -138,  289, -138,
 /*   440 */  -138, -138, -138, -138,  337, -138, -138, -138, -138, -138,
 /*   450 */  -138, -138, -138, -138, -138, -138, -138, 4998, 5754,  244,
 /*   460 */   961, -138,  329, -138, 4674, 5754, -138, -138, -138, 2266,
 /*   470 */  -138, 4836, 5754, -138,  343, -138, -138,  518, -138, -114,
 /*   480 */  -138, -138, -138, -138, -138, -138, -138,  705, -138, -138,
 /*   490 */  -138,   64, 1260, -138,  448, 1125,  927, -138, -138, -138,
 /*   500 */  -138,  552,  816, -138,  448, 1199, 1926, -138, -138, -138,
 /*   510 */  -138, -138, -138, -138,  -95,  483, -138, -138,  464, 1371,
 /*   520 */  -138, -138,  195, 1482, -138,  448,  828, 1704, -138,  593,
 /*   530 */  2037, -138,  448,  896, 1815, -138,  448,  925, 1038, -138,
 /*   540 */   636, 1149, -138,  448,  990, 1593, -138, -138, -138, -138,
 /*   550 */  -138, 1043,  594, -138, -138,  400, -138, -138,  844, -138,
 /*   560 */  -138, -138, -138, -138, -138, -138, -138, -138, -138, -138,
 /*   570 */  2446, -138, -138, -138, 1843, -138, -138, 1399, -138, -138,
 /*   580 */  2148, -138, -138,  -67, -138, -138, 1165, -138, -138, 1156,
 /*   590 */  -138, -138,  -20, -138, -138,  -12, -138, -138,   91, -138,
 /*   600 */  -138,  142, -138, -138,  166, -138, -138,  202, -138, -138,
 /*   610 */   219, -138, -138,  253, -138, -138,  277, -138, -138,  300,
 /*   620 */  -138, -138, -138, -138, -138, -138, -138, 2259, -138, 4728,
 /*   630 */  5754, -138, -138, -138,  490, -138, 4502, 5754, -138, -138,
 /*   640 */   934, -138, 5484, 5754, -138, -138, -138, -138, 4782, 5754,
 /*   650 */  -138, -138, 1198, -138, -138, -138, -138, -138, -138, 4507,
 /*   660 */  -138, -138, -138, -138, -138, 5052, 5754, -138, -138, -138,
 /*   670 */  -138, 5160, 5754, -138, -138,   75, -138, 4566, -138, 5754,
 /*   680 */  -138, -138, -138, 5376, -138, 5754, -138,  665, -138, 4944,
 /*   690 */  -138, 5754, -138,  613,  125, -138, -138,  495, -138, 5646,
 /*   700 */  -138, 5754, -138,  297, -138, 5268, -138, 5754, -138, -138,
 /*   710 */  -138, 5106, -138, 5754, -138,  826, -138, 4890, -138, 5754,
 /*   720 */  -138,  421,  170, -138,  613, -138,  755,  125,  975, -138,
 /*   730 */   987, 1002, -138, -138, -138,  732, -138, -138, -138, 1021,
 /*   740 */  -138, -138, -138, -138, -138, -138, -138,  125, 1079, -138,
 /*   750 */  1086, 1089, -138, -138, -138, -138, -138,  425,  -52,  170,
 /*   760 */   755,  170, -138,  205, 1159, -138, -138, -138,  807, 1287,
 /*   770 */  -138, -138, -138,  260, -138, -138,  857, -138,  827,  989,
 /*   780 */  -138,  789, 1131, -138, -138, -138,  541, -138, -138,  983,
 /*   790 */  -138, -138,
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
// 3969 "parser.c"
      break;
    case 120:
// 1291 "parser.lemon"
{ zval_ptr_dtor(&(yypminor->yy180)); }
// 3974 "parser.c"
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
// 4580 "parser.c"
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
// 4625 "parser.c"
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
// 4641 "parser.c"
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
// 4669 "parser.c"
        break;
      case 10:
// 1329 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_namespace(yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(43,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 4678 "parser.c"
        break;
      case 11:
// 1333 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_use_aliases(yymsp[-1].minor.yy180, status->scanner_state);
  yy_destructor(46,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 4687 "parser.c"
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
// 4704 "parser.c"
        break;
      case 14:
// 1345 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_use_aliases_item(yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 4711 "parser.c"
        break;
      case 15:
// 1349 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_use_aliases_item(yymsp[-2].minor.yy0, yymsp[0].minor.yy0, status->scanner_state);
  yy_destructor(47,&yymsp[-1].minor);
}
// 4719 "parser.c"
        break;
      case 16:
// 1353 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_interface(yymsp[-1].minor.yy0, yymsp[0].minor.yy180, NULL, status->scanner_state);
  yy_destructor(48,&yymsp[-2].minor);
}
// 4727 "parser.c"
        break;
      case 17:
// 1357 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_interface(yymsp[-3].minor.yy0, yymsp[0].minor.yy180, yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(48,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
}
// 4736 "parser.c"
        break;
      case 18:
// 1361 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_class(yymsp[-1].minor.yy0, yymsp[0].minor.yy180, 0, 0, NULL, NULL, status->scanner_state);
  yy_destructor(50,&yymsp[-2].minor);
}
// 4744 "parser.c"
        break;
      case 19:
// 1365 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy180, 0, 0, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(50,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
}
// 4753 "parser.c"
        break;
      case 20:
// 1369 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy180, 0, 0, NULL, yymsp[-1].minor.yy180, status->scanner_state);
  yy_destructor(50,&yymsp[-4].minor);
  yy_destructor(51,&yymsp[-2].minor);
}
// 4762 "parser.c"
        break;
      case 21:
// 1373 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_class(yymsp[-5].minor.yy0, yymsp[0].minor.yy180, 0, 0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy180, status->scanner_state);
  yy_destructor(50,&yymsp[-6].minor);
  yy_destructor(49,&yymsp[-4].minor);
  yy_destructor(51,&yymsp[-2].minor);
}
// 4772 "parser.c"
        break;
      case 22:
// 1377 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_class(yymsp[-1].minor.yy0, yymsp[0].minor.yy180, 1, 0, NULL, NULL, status->scanner_state);
  yy_destructor(52,&yymsp[-3].minor);
  yy_destructor(50,&yymsp[-2].minor);
}
// 4781 "parser.c"
        break;
      case 23:
// 1381 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy180, 1, 0, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(52,&yymsp[-5].minor);
  yy_destructor(50,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
}
// 4791 "parser.c"
        break;
      case 24:
// 1385 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy180, 1, 0, NULL, yymsp[-1].minor.yy180, status->scanner_state);
  yy_destructor(52,&yymsp[-5].minor);
  yy_destructor(50,&yymsp[-4].minor);
  yy_destructor(51,&yymsp[-2].minor);
}
// 4801 "parser.c"
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
// 4812 "parser.c"
        break;
      case 26:
// 1393 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_class(yymsp[-1].minor.yy0, yymsp[0].minor.yy180, 0, 1, NULL, NULL, status->scanner_state);
  yy_destructor(53,&yymsp[-3].minor);
  yy_destructor(50,&yymsp[-2].minor);
}
// 4821 "parser.c"
        break;
      case 27:
// 1397 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy180, 0, 1, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(53,&yymsp[-5].minor);
  yy_destructor(50,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
}
// 4831 "parser.c"
        break;
      case 28:
      case 57:
// 1401 "parser.lemon"
{
	yygotominor.yy180 = NULL;
  yy_destructor(54,&yymsp[-1].minor);
  yy_destructor(55,&yymsp[0].minor);
}
// 4841 "parser.c"
        break;
      case 29:
      case 58:
// 1405 "parser.lemon"
{
	yygotominor.yy180 = yymsp[-1].minor.yy180;
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(55,&yymsp[0].minor);
}
// 4851 "parser.c"
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
// 4862 "parser.c"
        break;
      case 33:
// 1421 "parser.lemon"
{
  yygotominor.yy180 = NULL;
  yy_destructor(54,&yymsp[-1].minor);
  yy_destructor(55,&yymsp[0].minor);
}
// 4871 "parser.c"
        break;
      case 34:
// 1425 "parser.lemon"
{
  yygotominor.yy180 = yymsp[-1].minor.yy180;
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(55,&yymsp[0].minor);
}
// 4880 "parser.c"
        break;
      case 35:
// 1429 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_class_definition(yymsp[0].minor.yy180, NULL, NULL, status->scanner_state);
}
// 4887 "parser.c"
        break;
      case 36:
// 1433 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_class_definition(NULL, NULL, yymsp[0].minor.yy180, status->scanner_state);
}
// 4894 "parser.c"
        break;
      case 37:
// 1437 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_class_definition(NULL, yymsp[0].minor.yy180, NULL, status->scanner_state);
}
// 4901 "parser.c"
        break;
      case 38:
// 1441 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_class_definition(yymsp[-1].minor.yy180, yymsp[0].minor.yy180, NULL, status->scanner_state);
}
// 4908 "parser.c"
        break;
      case 39:
// 1445 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_class_definition(yymsp[-1].minor.yy180, NULL, yymsp[0].minor.yy180, status->scanner_state);
}
// 4915 "parser.c"
        break;
      case 40:
// 1449 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_class_definition(yymsp[0].minor.yy180, NULL, yymsp[-1].minor.yy180, status->scanner_state);
}
// 4922 "parser.c"
        break;
      case 41:
// 1453 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_class_definition(NULL, yymsp[0].minor.yy180, yymsp[-1].minor.yy180, status->scanner_state);
}
// 4929 "parser.c"
        break;
      case 42:
// 1457 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_class_definition(yymsp[-2].minor.yy180, yymsp[0].minor.yy180, yymsp[-1].minor.yy180, status->scanner_state);
}
// 4936 "parser.c"
        break;
      case 43:
// 1461 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_class_definition(yymsp[-1].minor.yy180, yymsp[0].minor.yy180, yymsp[-2].minor.yy180, status->scanner_state);
}
// 4943 "parser.c"
        break;
      case 44:
// 1465 "parser.lemon"
{
  yygotominor.yy180 = zephir_ret_interface_definition(NULL, yymsp[0].minor.yy180, status->scanner_state);
}
// 4950 "parser.c"
        break;
      case 45:
// 1469 "parser.lemon"
{
  yygotominor.yy180 = zephir_ret_interface_definition(yymsp[0].minor.yy180, NULL, status->scanner_state);
}
// 4957 "parser.c"
        break;
      case 46:
// 1473 "parser.lemon"
{
  yygotominor.yy180 = zephir_ret_interface_definition(yymsp[0].minor.yy180, yymsp[-1].minor.yy180, status->scanner_state);
}
// 4964 "parser.c"
        break;
      case 49:
// 1486 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_class_property(yymsp[-2].minor.yy180, yymsp[-1].minor.yy0, NULL, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(45,&yymsp[0].minor);
}
// 4972 "parser.c"
        break;
      case 50:
// 1490 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_class_property(yymsp[-2].minor.yy180, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(45,&yymsp[0].minor);
}
// 4980 "parser.c"
        break;
      case 51:
// 1494 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_class_property(yymsp[-4].minor.yy180, yymsp[-3].minor.yy0, yymsp[-1].minor.yy180, yymsp[-5].minor.yy0, NULL, status->scanner_state);
  yy_destructor(57,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 4989 "parser.c"
        break;
      case 52:
// 1498 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_class_property(yymsp[-4].minor.yy180, yymsp[-3].minor.yy0, yymsp[-1].minor.yy180, NULL, NULL, status->scanner_state);
  yy_destructor(57,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 4998 "parser.c"
        break;
      case 53:
// 1502 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_class_property(yymsp[-3].minor.yy180, yymsp[-2].minor.yy0, NULL, yymsp[-4].minor.yy0, yymsp[-1].minor.yy180, status->scanner_state);
  yy_destructor(45,&yymsp[0].minor);
}
// 5006 "parser.c"
        break;
      case 54:
// 1506 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_class_property(yymsp[-3].minor.yy180, yymsp[-2].minor.yy0, NULL, NULL, yymsp[-1].minor.yy180, status->scanner_state);
  yy_destructor(45,&yymsp[0].minor);
}
// 5014 "parser.c"
        break;
      case 55:
// 1510 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_class_property(yymsp[-5].minor.yy180, yymsp[-4].minor.yy0, yymsp[-2].minor.yy180, yymsp[-6].minor.yy0, yymsp[-1].minor.yy180, status->scanner_state);
  yy_destructor(57,&yymsp[-3].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 5023 "parser.c"
        break;
      case 56:
// 1514 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_class_property(yymsp[-5].minor.yy180, yymsp[-4].minor.yy0, yymsp[-2].minor.yy180, NULL, yymsp[-1].minor.yy180, status->scanner_state);
  yy_destructor(57,&yymsp[-3].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 5032 "parser.c"
        break;
      case 61:
// 1534 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_property_shortcut(NULL, yymsp[0].minor.yy0, status->scanner_state);
}
// 5039 "parser.c"
        break;
      case 62:
// 1538 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_property_shortcut(yymsp[-1].minor.yy0, yymsp[0].minor.yy0, status->scanner_state);
}
// 5046 "parser.c"
        break;
      case 69:
// 1567 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy180, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(58,&yymsp[-4].minor);
  yy_destructor(57,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 5056 "parser.c"
        break;
      case 70:
// 1571 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy180, NULL, status->scanner_state);
  yy_destructor(58,&yymsp[-4].minor);
  yy_destructor(57,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 5066 "parser.c"
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
// 5078 "parser.c"
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
// 5090 "parser.c"
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
// 5102 "parser.c"
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
// 5114 "parser.c"
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
// 5126 "parser.c"
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
// 5138 "parser.c"
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
// 5150 "parser.c"
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
// 5162 "parser.c"
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
// 5175 "parser.c"
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
// 5188 "parser.c"
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
// 5201 "parser.c"
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
// 5214 "parser.c"
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
// 5227 "parser.c"
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
// 5240 "parser.c"
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
// 5253 "parser.c"
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
// 5266 "parser.c"
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
// 5278 "parser.c"
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
// 5290 "parser.c"
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
// 5302 "parser.c"
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
// 5314 "parser.c"
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
// 5325 "parser.c"
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
// 5336 "parser.c"
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
// 5347 "parser.c"
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
// 5358 "parser.c"
        break;
      case 97:
// 1682 "parser.lemon"
{
	yygotominor.yy180 = zephir_new_string("public");
  yy_destructor(1,&yymsp[0].minor);
}
// 5366 "parser.c"
        break;
      case 98:
// 1686 "parser.lemon"
{
	yygotominor.yy180 = zephir_new_string("protected");
  yy_destructor(2,&yymsp[0].minor);
}
// 5374 "parser.c"
        break;
      case 99:
// 1690 "parser.lemon"
{
	yygotominor.yy180 = zephir_new_string("private");
  yy_destructor(4,&yymsp[0].minor);
}
// 5382 "parser.c"
        break;
      case 100:
// 1694 "parser.lemon"
{
	yygotominor.yy180 = zephir_new_string("static");
  yy_destructor(3,&yymsp[0].minor);
}
// 5390 "parser.c"
        break;
      case 101:
// 1698 "parser.lemon"
{
	yygotominor.yy180 = zephir_new_string("scoped");
  yy_destructor(5,&yymsp[0].minor);
}
// 5398 "parser.c"
        break;
      case 102:
// 1702 "parser.lemon"
{
	yygotominor.yy180 = zephir_new_string("inline");
  yy_destructor(62,&yymsp[0].minor);
}
// 5406 "parser.c"
        break;
      case 103:
// 1706 "parser.lemon"
{
	yygotominor.yy180 = zephir_new_string("abstract");
  yy_destructor(52,&yymsp[0].minor);
}
// 5414 "parser.c"
        break;
      case 104:
// 1710 "parser.lemon"
{
	yygotominor.yy180 = zephir_new_string("final");
  yy_destructor(53,&yymsp[0].minor);
}
// 5422 "parser.c"
        break;
      case 105:
// 1715 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_return_type(1, NULL, status->scanner_state);
  yy_destructor(63,&yymsp[0].minor);
}
// 5430 "parser.c"
        break;
      case 106:
// 1719 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_return_type(0, yymsp[0].minor.yy180, status->scanner_state);
}
// 5437 "parser.c"
        break;
      case 107:
      case 205:
// 1723 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_list(yymsp[-2].minor.yy180, yymsp[0].minor.yy180);
  yy_destructor(14,&yymsp[-1].minor);
}
// 5446 "parser.c"
        break;
      case 109:
// 1731 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_return_type_item(yymsp[0].minor.yy180, NULL, 0, 0, status->scanner_state);
}
// 5453 "parser.c"
        break;
      case 110:
// 1735 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_return_type_item(zephir_ret_type(ZEPHIR_T_TYPE_NULL), NULL, 0, 0, status->scanner_state);
  yy_destructor(64,&yymsp[0].minor);
}
// 5461 "parser.c"
        break;
      case 111:
// 1739 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_return_type_item(zephir_ret_type(ZEPHIR_T_TYPE_THIS), NULL, 0, 0, status->scanner_state);
  yy_destructor(65,&yymsp[0].minor);
}
// 5469 "parser.c"
        break;
      case 112:
// 1743 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_return_type_item(yymsp[-1].minor.yy180, NULL, 1, 0, status->scanner_state);
  yy_destructor(39,&yymsp[0].minor);
}
// 5477 "parser.c"
        break;
      case 113:
// 1747 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_return_type_item(NULL, yymsp[0].minor.yy180, 0, 0, status->scanner_state);
}
// 5484 "parser.c"
        break;
      case 114:
// 1751 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_return_type_item(NULL, yymsp[0].minor.yy180, 0, 1, status->scanner_state);
}
// 5491 "parser.c"
        break;
      case 117:
// 1765 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_parameter(0, NULL, NULL, yymsp[0].minor.yy0, NULL, 0, status->scanner_state);
}
// 5498 "parser.c"
        break;
      case 118:
// 1769 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_parameter(1, NULL, NULL, yymsp[0].minor.yy0, NULL, 0, status->scanner_state);
  yy_destructor(58,&yymsp[-1].minor);
}
// 5506 "parser.c"
        break;
      case 119:
// 1773 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_parameter(0, yymsp[-1].minor.yy180, NULL, yymsp[0].minor.yy0, NULL, 0, status->scanner_state);
}
// 5513 "parser.c"
        break;
      case 120:
// 1777 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_parameter(1, yymsp[-1].minor.yy180, NULL, yymsp[0].minor.yy0, NULL, 0, status->scanner_state);
  yy_destructor(58,&yymsp[-2].minor);
}
// 5521 "parser.c"
        break;
      case 121:
// 1781 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_parameter(0, yymsp[-2].minor.yy180, NULL, yymsp[0].minor.yy0, NULL, 1, status->scanner_state);
  yy_destructor(39,&yymsp[-1].minor);
}
// 5529 "parser.c"
        break;
      case 122:
// 1785 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_parameter(1, yymsp[-2].minor.yy180, NULL, yymsp[0].minor.yy0, NULL, 1, status->scanner_state);
  yy_destructor(58,&yymsp[-3].minor);
  yy_destructor(39,&yymsp[-1].minor);
}
// 5538 "parser.c"
        break;
      case 123:
// 1789 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_parameter(0, NULL, yymsp[-1].minor.yy180, yymsp[0].minor.yy0, NULL, 0, status->scanner_state);
}
// 5545 "parser.c"
        break;
      case 124:
// 1793 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_parameter(1, NULL, yymsp[-1].minor.yy180, yymsp[0].minor.yy0, NULL, 0, status->scanner_state);
  yy_destructor(58,&yymsp[-2].minor);
}
// 5553 "parser.c"
        break;
      case 125:
// 1797 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_parameter(0, NULL, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy180, 0, status->scanner_state);
  yy_destructor(57,&yymsp[-1].minor);
}
// 5561 "parser.c"
        break;
      case 126:
// 1801 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_parameter(1, NULL, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy180, 0, status->scanner_state);
  yy_destructor(58,&yymsp[-3].minor);
  yy_destructor(57,&yymsp[-1].minor);
}
// 5570 "parser.c"
        break;
      case 127:
// 1805 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_parameter(0, yymsp[-3].minor.yy180, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy180, 0, status->scanner_state);
  yy_destructor(57,&yymsp[-1].minor);
}
// 5578 "parser.c"
        break;
      case 128:
// 1809 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_parameter(1, yymsp[-3].minor.yy180, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy180, 0, status->scanner_state);
  yy_destructor(58,&yymsp[-4].minor);
  yy_destructor(57,&yymsp[-1].minor);
}
// 5587 "parser.c"
        break;
      case 129:
// 1813 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_parameter(0, yymsp[-4].minor.yy180, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy180, 1, status->scanner_state);
  yy_destructor(39,&yymsp[-3].minor);
  yy_destructor(57,&yymsp[-1].minor);
}
// 5596 "parser.c"
        break;
      case 130:
// 1817 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_parameter(1, yymsp[-4].minor.yy180, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy180, 1, status->scanner_state);
  yy_destructor(58,&yymsp[-5].minor);
  yy_destructor(39,&yymsp[-3].minor);
  yy_destructor(57,&yymsp[-1].minor);
}
// 5606 "parser.c"
        break;
      case 131:
// 1821 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_parameter(0, NULL, yymsp[-3].minor.yy180, yymsp[-2].minor.yy0, yymsp[0].minor.yy180, 0, status->scanner_state);
  yy_destructor(57,&yymsp[-1].minor);
}
// 5614 "parser.c"
        break;
      case 132:
// 1825 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_parameter(1, NULL, yymsp[-3].minor.yy180, yymsp[-2].minor.yy0, yymsp[0].minor.yy180, 0, status->scanner_state);
  yy_destructor(58,&yymsp[-4].minor);
  yy_destructor(57,&yymsp[-1].minor);
}
// 5623 "parser.c"
        break;
      case 133:
// 1830 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_literal(ZEPHIR_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(21,&yymsp[-2].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 5632 "parser.c"
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
// 5643 "parser.c"
        break;
      case 135:
// 1838 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_type(ZEPHIR_TYPE_INTEGER);
  yy_destructor(67,&yymsp[0].minor);
}
// 5651 "parser.c"
        break;
      case 136:
// 1842 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_type(ZEPHIR_TYPE_UINTEGER);
  yy_destructor(68,&yymsp[0].minor);
}
// 5659 "parser.c"
        break;
      case 137:
// 1846 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_type(ZEPHIR_TYPE_LONG);
  yy_destructor(69,&yymsp[0].minor);
}
// 5667 "parser.c"
        break;
      case 138:
// 1850 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_type(ZEPHIR_TYPE_ULONG);
  yy_destructor(70,&yymsp[0].minor);
}
// 5675 "parser.c"
        break;
      case 139:
// 1854 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_type(ZEPHIR_TYPE_CHAR);
  yy_destructor(71,&yymsp[0].minor);
}
// 5683 "parser.c"
        break;
      case 140:
// 1858 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_type(ZEPHIR_TYPE_UCHAR);
  yy_destructor(72,&yymsp[0].minor);
}
// 5691 "parser.c"
        break;
      case 141:
// 1862 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_type(ZEPHIR_TYPE_DOUBLE);
  yy_destructor(73,&yymsp[0].minor);
}
// 5699 "parser.c"
        break;
      case 142:
// 1866 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_type(ZEPHIR_TYPE_BOOL);
  yy_destructor(74,&yymsp[0].minor);
}
// 5707 "parser.c"
        break;
      case 143:
// 1870 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_type(ZEPHIR_TYPE_STRING);
  yy_destructor(75,&yymsp[0].minor);
}
// 5715 "parser.c"
        break;
      case 144:
// 1874 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_type(ZEPHIR_TYPE_ARRAY);
  yy_destructor(76,&yymsp[0].minor);
}
// 5723 "parser.c"
        break;
      case 145:
// 1878 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_type(ZEPHIR_TYPE_VAR);
  yy_destructor(77,&yymsp[0].minor);
}
// 5731 "parser.c"
        break;
      case 146:
// 1882 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_type(ZEPHIR_TYPE_CALLABLE);
  yy_destructor(78,&yymsp[0].minor);
}
// 5739 "parser.c"
        break;
      case 147:
// 1886 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_type(ZEPHIR_TYPE_RESOURCE);
  yy_destructor(79,&yymsp[0].minor);
}
// 5747 "parser.c"
        break;
      case 148:
// 1890 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_type(ZEPHIR_TYPE_OBJECT);
  yy_destructor(80,&yymsp[0].minor);
}
// 5755 "parser.c"
        break;
      case 174:
// 1996 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_empty_statement(status->scanner_state);
  yy_destructor(45,&yymsp[0].minor);
}
// 5763 "parser.c"
        break;
      case 175:
// 2000 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_break_statement(status->scanner_state);
  yy_destructor(81,&yymsp[-1].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 5772 "parser.c"
        break;
      case 176:
// 2004 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_continue_statement(status->scanner_state);
  yy_destructor(82,&yymsp[-1].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 5781 "parser.c"
        break;
      case 177:
// 2008 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_if_statement(yymsp[-2].minor.yy180, NULL, NULL, status->scanner_state);
  yy_destructor(83,&yymsp[-3].minor);
  yy_destructor(54,&yymsp[-1].minor);
  yy_destructor(55,&yymsp[0].minor);
}
// 5791 "parser.c"
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
// 5804 "parser.c"
        break;
      case 179:
// 2016 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_if_statement(yymsp[-3].minor.yy180, yymsp[-1].minor.yy180, NULL, status->scanner_state);
  yy_destructor(83,&yymsp[-4].minor);
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(55,&yymsp[0].minor);
}
// 5814 "parser.c"
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
// 5827 "parser.c"
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
// 5840 "parser.c"
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
// 5853 "parser.c"
        break;
      case 183:
// 2032 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_switch_statement(yymsp[-2].minor.yy180, NULL, status->scanner_state);
  yy_destructor(85,&yymsp[-3].minor);
  yy_destructor(54,&yymsp[-1].minor);
  yy_destructor(55,&yymsp[0].minor);
}
// 5863 "parser.c"
        break;
      case 184:
// 2036 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_switch_statement(yymsp[-3].minor.yy180, yymsp[-1].minor.yy180, status->scanner_state);
  yy_destructor(85,&yymsp[-4].minor);
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(55,&yymsp[0].minor);
}
// 5873 "parser.c"
        break;
      case 187:
// 2048 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_case_clause(yymsp[-1].minor.yy180, NULL, status->scanner_state);
  yy_destructor(86,&yymsp[-2].minor);
  yy_destructor(87,&yymsp[0].minor);
}
// 5882 "parser.c"
        break;
      case 188:
// 2052 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_case_clause(yymsp[-2].minor.yy180, yymsp[0].minor.yy180, status->scanner_state);
  yy_destructor(86,&yymsp[-3].minor);
  yy_destructor(87,&yymsp[-1].minor);
}
// 5891 "parser.c"
        break;
      case 189:
// 2056 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_case_clause(NULL, yymsp[0].minor.yy180, status->scanner_state);
  yy_destructor(88,&yymsp[-2].minor);
  yy_destructor(87,&yymsp[-1].minor);
}
// 5900 "parser.c"
        break;
      case 190:
// 2060 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_loop_statement(NULL, status->scanner_state);
  yy_destructor(89,&yymsp[-2].minor);
  yy_destructor(54,&yymsp[-1].minor);
  yy_destructor(55,&yymsp[0].minor);
}
// 5910 "parser.c"
        break;
      case 191:
// 2064 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_loop_statement(yymsp[-1].minor.yy180, status->scanner_state);
  yy_destructor(89,&yymsp[-3].minor);
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(55,&yymsp[0].minor);
}
// 5920 "parser.c"
        break;
      case 192:
// 2068 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_while_statement(yymsp[-2].minor.yy180, NULL, status->scanner_state);
  yy_destructor(90,&yymsp[-3].minor);
  yy_destructor(54,&yymsp[-1].minor);
  yy_destructor(55,&yymsp[0].minor);
}
// 5930 "parser.c"
        break;
      case 193:
// 2072 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_while_statement(yymsp[-3].minor.yy180, yymsp[-1].minor.yy180, status->scanner_state);
  yy_destructor(90,&yymsp[-4].minor);
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(55,&yymsp[0].minor);
}
// 5940 "parser.c"
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
// 5952 "parser.c"
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
// 5964 "parser.c"
        break;
      case 196:
// 2084 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_try_catch_statement(NULL, NULL, status->scanner_state);
  yy_destructor(92,&yymsp[-2].minor);
  yy_destructor(54,&yymsp[-1].minor);
  yy_destructor(55,&yymsp[0].minor);
}
// 5974 "parser.c"
        break;
      case 197:
// 2088 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_try_catch_statement(yymsp[-1].minor.yy180, NULL, status->scanner_state);
  yy_destructor(92,&yymsp[-3].minor);
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(55,&yymsp[0].minor);
}
// 5984 "parser.c"
        break;
      case 198:
// 2092 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_try_catch_statement(yymsp[-2].minor.yy180, yymsp[0].minor.yy180, status->scanner_state);
  yy_destructor(92,&yymsp[-4].minor);
  yy_destructor(54,&yymsp[-3].minor);
  yy_destructor(55,&yymsp[-1].minor);
}
// 5994 "parser.c"
        break;
      case 201:
// 2104 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_catch_statement(yymsp[-3].minor.yy180, NULL, yymsp[-1].minor.yy180, status->scanner_state);
  yy_destructor(93,&yymsp[-4].minor);
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(55,&yymsp[0].minor);
}
// 6004 "parser.c"
        break;
      case 202:
// 2108 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_catch_statement(yymsp[-2].minor.yy180, NULL, NULL, status->scanner_state);
  yy_destructor(93,&yymsp[-3].minor);
  yy_destructor(54,&yymsp[-1].minor);
  yy_destructor(55,&yymsp[0].minor);
}
// 6014 "parser.c"
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
// 6025 "parser.c"
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
// 6036 "parser.c"
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
// 6047 "parser.c"
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
// 6058 "parser.c"
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
// 6070 "parser.c"
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
// 6082 "parser.c"
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
// 6094 "parser.c"
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
// 6107 "parser.c"
        break;
      case 214:
// 2156 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_let_statement(yymsp[-1].minor.yy180, status->scanner_state);
  yy_destructor(97,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 6116 "parser.c"
        break;
      case 217:
// 2169 "parser.lemon"
{
	yygotominor.yy180 = zephir_new_string("assign");
  yy_destructor(57,&yymsp[0].minor);
}
// 6124 "parser.c"
        break;
      case 218:
// 2174 "parser.lemon"
{
	yygotominor.yy180 = zephir_new_string("add-assign");
  yy_destructor(98,&yymsp[0].minor);
}
// 6132 "parser.c"
        break;
      case 219:
// 2179 "parser.lemon"
{
	yygotominor.yy180 = zephir_new_string("sub-assign");
  yy_destructor(99,&yymsp[0].minor);
}
// 6140 "parser.c"
        break;
      case 220:
// 2183 "parser.lemon"
{
	yygotominor.yy180 = zephir_new_string("mul-assign");
  yy_destructor(100,&yymsp[0].minor);
}
// 6148 "parser.c"
        break;
      case 221:
// 2187 "parser.lemon"
{
	yygotominor.yy180 = zephir_new_string("div-assign");
  yy_destructor(101,&yymsp[0].minor);
}
// 6156 "parser.c"
        break;
      case 222:
// 2191 "parser.lemon"
{
	yygotominor.yy180 = zephir_new_string("concat-assign");
  yy_destructor(102,&yymsp[0].minor);
}
// 6164 "parser.c"
        break;
      case 223:
// 2195 "parser.lemon"
{
	yygotominor.yy180 = zephir_new_string("mod-assign");
  yy_destructor(103,&yymsp[0].minor);
}
// 6172 "parser.c"
        break;
      case 224:
// 2200 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_let_assignment("variable", yymsp[-1].minor.yy180, yymsp[-2].minor.yy0, NULL, NULL, yymsp[0].minor.yy180, status->scanner_state);
}
// 6179 "parser.c"
        break;
      case 225:
// 2205 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_let_assignment("object-property", yymsp[-1].minor.yy180, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, yymsp[0].minor.yy180, status->scanner_state);
  yy_destructor(42,&yymsp[-3].minor);
}
// 6187 "parser.c"
        break;
      case 226:
// 2210 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_let_assignment("variable-dynamic-object-property", yymsp[-1].minor.yy180, yymsp[-6].minor.yy0, yymsp[-3].minor.yy0, NULL, yymsp[0].minor.yy180, status->scanner_state);
  yy_destructor(42,&yymsp[-5].minor);
  yy_destructor(54,&yymsp[-4].minor);
  yy_destructor(55,&yymsp[-2].minor);
}
// 6197 "parser.c"
        break;
      case 227:
// 2215 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_let_assignment("string-dynamic-object-property", yymsp[-1].minor.yy180, yymsp[-6].minor.yy0, yymsp[-3].minor.yy0, NULL, yymsp[0].minor.yy180, status->scanner_state);
  yy_destructor(42,&yymsp[-5].minor);
  yy_destructor(54,&yymsp[-4].minor);
  yy_destructor(55,&yymsp[-2].minor);
}
// 6207 "parser.c"
        break;
      case 228:
// 2220 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_let_assignment("object-property-append", yymsp[-1].minor.yy180, yymsp[-6].minor.yy0, yymsp[-4].minor.yy0, NULL, yymsp[0].minor.yy180, status->scanner_state);
  yy_destructor(42,&yymsp[-5].minor);
  yy_destructor(41,&yymsp[-3].minor);
  yy_destructor(66,&yymsp[-2].minor);
}
// 6217 "parser.c"
        break;
      case 229:
// 2225 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_let_assignment("object-property-array-index", yymsp[-1].minor.yy180, yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-2].minor.yy180, yymsp[0].minor.yy180, status->scanner_state);
  yy_destructor(42,&yymsp[-4].minor);
}
// 6225 "parser.c"
        break;
      case 230:
// 2229 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_let_assignment("object-property-array-index-append", yymsp[-1].minor.yy180, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, yymsp[-4].minor.yy180, yymsp[0].minor.yy180, status->scanner_state);
  yy_destructor(42,&yymsp[-6].minor);
  yy_destructor(41,&yymsp[-3].minor);
  yy_destructor(66,&yymsp[-2].minor);
}
// 6235 "parser.c"
        break;
      case 231:
// 2234 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_let_assignment("static-property", yymsp[-1].minor.yy180, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, yymsp[0].minor.yy180, status->scanner_state);
  yy_destructor(105,&yymsp[-3].minor);
}
// 6243 "parser.c"
        break;
      case 232:
// 2239 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_let_assignment("static-property-append", yymsp[-1].minor.yy180, yymsp[-6].minor.yy0, yymsp[-4].minor.yy0, NULL, yymsp[0].minor.yy180, status->scanner_state);
  yy_destructor(105,&yymsp[-5].minor);
  yy_destructor(41,&yymsp[-3].minor);
  yy_destructor(66,&yymsp[-2].minor);
}
// 6253 "parser.c"
        break;
      case 233:
// 2244 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_let_assignment("static-property-array-index", yymsp[-1].minor.yy180, yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-2].minor.yy180, yymsp[0].minor.yy180, status->scanner_state);
  yy_destructor(105,&yymsp[-4].minor);
}
// 6261 "parser.c"
        break;
      case 234:
// 2249 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_let_assignment("static-property-array-index-append", yymsp[-1].minor.yy180, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, yymsp[-4].minor.yy180, yymsp[0].minor.yy180, status->scanner_state);
  yy_destructor(105,&yymsp[-6].minor);
  yy_destructor(41,&yymsp[-3].minor);
  yy_destructor(66,&yymsp[-2].minor);
}
// 6271 "parser.c"
        break;
      case 235:
// 2254 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_let_assignment("variable-append", yymsp[-1].minor.yy180, yymsp[-4].minor.yy0, NULL, NULL, yymsp[0].minor.yy180, status->scanner_state);
  yy_destructor(41,&yymsp[-3].minor);
  yy_destructor(66,&yymsp[-2].minor);
}
// 6280 "parser.c"
        break;
      case 236:
// 2259 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_let_assignment("array-index", yymsp[-1].minor.yy180, yymsp[-3].minor.yy0, NULL, yymsp[-2].minor.yy180, yymsp[0].minor.yy180, status->scanner_state);
}
// 6287 "parser.c"
        break;
      case 237:
// 2264 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_let_assignment("array-index-append", yymsp[-1].minor.yy180, yymsp[-5].minor.yy0, NULL, yymsp[-4].minor.yy180, yymsp[0].minor.yy180, status->scanner_state);
  yy_destructor(41,&yymsp[-3].minor);
  yy_destructor(66,&yymsp[-2].minor);
}
// 6296 "parser.c"
        break;
      case 240:
// 2276 "parser.lemon"
{
	yygotominor.yy180 = yymsp[-1].minor.yy180;
  yy_destructor(41,&yymsp[-2].minor);
  yy_destructor(66,&yymsp[0].minor);
}
// 6305 "parser.c"
        break;
      case 241:
// 2281 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_let_assignment("object-property-incr", NULL, yymsp[-3].minor.yy0, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(106,&yymsp[0].minor);
}
// 6314 "parser.c"
        break;
      case 242:
// 2286 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_let_assignment("object-property-decr", NULL, yymsp[-3].minor.yy0, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(107,&yymsp[0].minor);
}
// 6323 "parser.c"
        break;
      case 243:
// 2291 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_let_assignment("incr", NULL, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(106,&yymsp[0].minor);
}
// 6331 "parser.c"
        break;
      case 244:
// 2296 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_let_assignment("decr", NULL, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(107,&yymsp[0].minor);
}
// 6339 "parser.c"
        break;
      case 245:
// 2301 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_let_assignment("dynamic-variable", yymsp[-1].minor.yy180, yymsp[-3].minor.yy0, NULL, NULL, yymsp[0].minor.yy180, status->scanner_state);
  yy_destructor(54,&yymsp[-4].minor);
  yy_destructor(55,&yymsp[-2].minor);
}
// 6348 "parser.c"
        break;
      case 247:
// 2309 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_echo_statement(yymsp[-1].minor.yy180, status->scanner_state);
  yy_destructor(108,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 6357 "parser.c"
        break;
      case 250:
// 2321 "parser.lemon"
{
	yygotominor.yy180 = yymsp[0].minor.yy180;;
}
// 6364 "parser.c"
        break;
      case 251:
// 2326 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_mcall_statement(yymsp[-1].minor.yy180, status->scanner_state);
  yy_destructor(45,&yymsp[0].minor);
}
// 6372 "parser.c"
        break;
      case 252:
// 2331 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_fcall_statement(yymsp[-1].minor.yy180, status->scanner_state);
  yy_destructor(45,&yymsp[0].minor);
}
// 6380 "parser.c"
        break;
      case 253:
// 2336 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_scall_statement(yymsp[-1].minor.yy180, status->scanner_state);
  yy_destructor(45,&yymsp[0].minor);
}
// 6388 "parser.c"
        break;
      case 254:
// 2341 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_fetch_statement(yymsp[-1].minor.yy180, status->scanner_state);
  yy_destructor(45,&yymsp[0].minor);
}
// 6396 "parser.c"
        break;
      case 255:
// 2346 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_return_statement(yymsp[-1].minor.yy180, status->scanner_state);
  yy_destructor(109,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 6405 "parser.c"
        break;
      case 256:
// 2351 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_return_statement(NULL, status->scanner_state);
  yy_destructor(109,&yymsp[-1].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 6414 "parser.c"
        break;
      case 257:
// 2356 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_require_statement(yymsp[-1].minor.yy180, status->scanner_state);
  yy_destructor(7,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 6423 "parser.c"
        break;
      case 258:
// 2361 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_unset_statement(yymsp[-1].minor.yy180, status->scanner_state);
  yy_destructor(110,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 6432 "parser.c"
        break;
      case 259:
// 2366 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_throw_exception(yymsp[-1].minor.yy180, status->scanner_state);
  yy_destructor(111,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 6441 "parser.c"
        break;
      case 260:
// 2370 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_declare_statement(ZEPHIR_T_TYPE_INTEGER, yymsp[-1].minor.yy180, status->scanner_state);
  yy_destructor(67,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 6450 "parser.c"
        break;
      case 261:
// 2374 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_declare_statement(ZEPHIR_T_TYPE_UINTEGER, yymsp[-1].minor.yy180, status->scanner_state);
  yy_destructor(68,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 6459 "parser.c"
        break;
      case 262:
// 2378 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_declare_statement(ZEPHIR_T_TYPE_CHAR, yymsp[-1].minor.yy180, status->scanner_state);
  yy_destructor(71,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 6468 "parser.c"
        break;
      case 263:
// 2382 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_declare_statement(ZEPHIR_T_TYPE_UCHAR, yymsp[-1].minor.yy180, status->scanner_state);
  yy_destructor(72,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 6477 "parser.c"
        break;
      case 264:
// 2386 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_declare_statement(ZEPHIR_T_TYPE_LONG, yymsp[-1].minor.yy180, status->scanner_state);
  yy_destructor(69,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 6486 "parser.c"
        break;
      case 265:
// 2390 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_declare_statement(ZEPHIR_T_TYPE_ULONG, yymsp[-1].minor.yy180, status->scanner_state);
  yy_destructor(70,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 6495 "parser.c"
        break;
      case 266:
// 2394 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_declare_statement(ZEPHIR_T_TYPE_DOUBLE, yymsp[-1].minor.yy180, status->scanner_state);
  yy_destructor(73,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 6504 "parser.c"
        break;
      case 267:
// 2398 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_declare_statement(ZEPHIR_T_TYPE_STRING, yymsp[-1].minor.yy180, status->scanner_state);
  yy_destructor(75,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 6513 "parser.c"
        break;
      case 268:
// 2402 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_declare_statement(ZEPHIR_T_TYPE_BOOL, yymsp[-1].minor.yy180, status->scanner_state);
  yy_destructor(74,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 6522 "parser.c"
        break;
      case 269:
// 2406 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_declare_statement(ZEPHIR_T_TYPE_VAR, yymsp[-1].minor.yy180, status->scanner_state);
  yy_destructor(77,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 6531 "parser.c"
        break;
      case 270:
// 2410 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_declare_statement(ZEPHIR_T_TYPE_ARRAY, yymsp[-1].minor.yy180, status->scanner_state);
  yy_destructor(76,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 6540 "parser.c"
        break;
      case 273:
// 2422 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_declare_variable(yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 6547 "parser.c"
        break;
      case 274:
// 2426 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_declare_variable(yymsp[-2].minor.yy0, yymsp[0].minor.yy180, status->scanner_state);
  yy_destructor(57,&yymsp[-1].minor);
}
// 6555 "parser.c"
        break;
      case 276:
// 2434 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_expr("not", yymsp[0].minor.yy180, NULL, NULL, status->scanner_state);
  yy_destructor(39,&yymsp[-1].minor);
}
// 6563 "parser.c"
        break;
      case 277:
// 2438 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_expr("minus", yymsp[0].minor.yy180, NULL, NULL, status->scanner_state);
  yy_destructor(28,&yymsp[-1].minor);
}
// 6571 "parser.c"
        break;
      case 278:
// 2442 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_expr("isset", yymsp[0].minor.yy180, NULL, NULL, status->scanner_state);
  yy_destructor(33,&yymsp[-1].minor);
}
// 6579 "parser.c"
        break;
      case 279:
// 2446 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_expr("require", yymsp[0].minor.yy180, NULL, NULL, status->scanner_state);
  yy_destructor(7,&yymsp[-1].minor);
}
// 6587 "parser.c"
        break;
      case 280:
// 2450 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_expr("clone", yymsp[0].minor.yy180, NULL, NULL, status->scanner_state);
  yy_destructor(37,&yymsp[-1].minor);
}
// 6595 "parser.c"
        break;
      case 281:
// 2454 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_expr("empty", yymsp[0].minor.yy180, NULL, NULL, status->scanner_state);
  yy_destructor(35,&yymsp[-1].minor);
}
// 6603 "parser.c"
        break;
      case 282:
// 2458 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_expr("likely", yymsp[0].minor.yy180, NULL, NULL, status->scanner_state);
  yy_destructor(9,&yymsp[-1].minor);
}
// 6611 "parser.c"
        break;
      case 283:
// 2462 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_expr("unlikely", yymsp[0].minor.yy180, NULL, NULL, status->scanner_state);
  yy_destructor(10,&yymsp[-1].minor);
}
// 6619 "parser.c"
        break;
      case 284:
// 2466 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_expr("equals", yymsp[-2].minor.yy180, yymsp[0].minor.yy180, NULL, status->scanner_state);
  yy_destructor(19,&yymsp[-1].minor);
}
// 6627 "parser.c"
        break;
      case 285:
// 2470 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_expr("not-equals", yymsp[-2].minor.yy180, yymsp[0].minor.yy180, NULL, status->scanner_state);
  yy_destructor(26,&yymsp[-1].minor);
}
// 6635 "parser.c"
        break;
      case 286:
// 2474 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_expr("identical", yymsp[-2].minor.yy180, yymsp[0].minor.yy180, NULL, status->scanner_state);
  yy_destructor(20,&yymsp[-1].minor);
}
// 6643 "parser.c"
        break;
      case 287:
// 2478 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_expr("not-identical", yymsp[-2].minor.yy180, yymsp[0].minor.yy180, NULL, status->scanner_state);
  yy_destructor(25,&yymsp[-1].minor);
}
// 6651 "parser.c"
        break;
      case 288:
// 2482 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_expr("less", yymsp[-2].minor.yy180, yymsp[0].minor.yy180, NULL, status->scanner_state);
  yy_destructor(21,&yymsp[-1].minor);
}
// 6659 "parser.c"
        break;
      case 289:
// 2486 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_expr("greater", yymsp[-2].minor.yy180, yymsp[0].minor.yy180, NULL, status->scanner_state);
  yy_destructor(22,&yymsp[-1].minor);
}
// 6667 "parser.c"
        break;
      case 290:
// 2490 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_expr("less-equal", yymsp[-2].minor.yy180, yymsp[0].minor.yy180, NULL, status->scanner_state);
  yy_destructor(23,&yymsp[-1].minor);
}
// 6675 "parser.c"
        break;
      case 291:
// 2494 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_expr("greater-equal", yymsp[-2].minor.yy180, yymsp[0].minor.yy180, NULL, status->scanner_state);
  yy_destructor(24,&yymsp[-1].minor);
}
// 6683 "parser.c"
        break;
      case 292:
// 2498 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_expr("list", yymsp[-1].minor.yy180, NULL, NULL, status->scanner_state);
  yy_destructor(61,&yymsp[-2].minor);
  yy_destructor(40,&yymsp[0].minor);
}
// 6692 "parser.c"
        break;
      case 293:
// 2502 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_expr("cast", yymsp[-2].minor.yy180, yymsp[0].minor.yy180, NULL, status->scanner_state);
  yy_destructor(61,&yymsp[-3].minor);
  yy_destructor(40,&yymsp[-1].minor);
}
// 6701 "parser.c"
        break;
      case 294:
// 2506 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_expr("type-hint", zephir_ret_literal(ZEPHIR_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), yymsp[0].minor.yy180, NULL, status->scanner_state);
  yy_destructor(21,&yymsp[-3].minor);
  yy_destructor(22,&yymsp[-1].minor);
}
// 6710 "parser.c"
        break;
      case 295:
// 2510 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_expr("property-access", yymsp[-2].minor.yy180, zephir_ret_literal(ZEPHIR_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(42,&yymsp[-1].minor);
}
// 6718 "parser.c"
        break;
      case 296:
// 2514 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_expr("property-dynamic-access", yymsp[-4].minor.yy180, zephir_ret_literal(ZEPHIR_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(42,&yymsp[-3].minor);
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(55,&yymsp[0].minor);
}
// 6728 "parser.c"
        break;
      case 297:
// 2518 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_expr("property-string-access", yymsp[-4].minor.yy180, zephir_ret_literal(ZEPHIR_T_STRING, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(42,&yymsp[-3].minor);
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(55,&yymsp[0].minor);
}
// 6738 "parser.c"
        break;
      case 298:
// 2522 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_expr("static-property-access", zephir_ret_literal(ZEPHIR_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), zephir_ret_literal(ZEPHIR_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(105,&yymsp[-1].minor);
}
// 6746 "parser.c"
        break;
      case 299:
      case 374:
// 2526 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_expr("static-constant-access", zephir_ret_literal(ZEPHIR_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), zephir_ret_literal(ZEPHIR_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(105,&yymsp[-1].minor);
}
// 6755 "parser.c"
        break;
      case 300:
// 2535 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_expr("array-access", yymsp[-3].minor.yy180, yymsp[-1].minor.yy180, NULL, status->scanner_state);
  yy_destructor(41,&yymsp[-2].minor);
  yy_destructor(66,&yymsp[0].minor);
}
// 6764 "parser.c"
        break;
      case 301:
// 2540 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_expr("add", yymsp[-2].minor.yy180, yymsp[0].minor.yy180, NULL, status->scanner_state);
  yy_destructor(27,&yymsp[-1].minor);
}
// 6772 "parser.c"
        break;
      case 302:
// 2545 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_expr("sub", yymsp[-2].minor.yy180, yymsp[0].minor.yy180, NULL, status->scanner_state);
  yy_destructor(28,&yymsp[-1].minor);
}
// 6780 "parser.c"
        break;
      case 303:
// 2550 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_expr("mul", yymsp[-2].minor.yy180, yymsp[0].minor.yy180, NULL, status->scanner_state);
  yy_destructor(30,&yymsp[-1].minor);
}
// 6788 "parser.c"
        break;
      case 304:
// 2555 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_expr("div", yymsp[-2].minor.yy180, yymsp[0].minor.yy180, NULL, status->scanner_state);
  yy_destructor(31,&yymsp[-1].minor);
}
// 6796 "parser.c"
        break;
      case 305:
// 2560 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_expr("mod", yymsp[-2].minor.yy180, yymsp[0].minor.yy180, NULL, status->scanner_state);
  yy_destructor(32,&yymsp[-1].minor);
}
// 6804 "parser.c"
        break;
      case 306:
// 2565 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_expr("concat", yymsp[-2].minor.yy180, yymsp[0].minor.yy180, NULL, status->scanner_state);
  yy_destructor(29,&yymsp[-1].minor);
}
// 6812 "parser.c"
        break;
      case 307:
// 2570 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_expr("and", yymsp[-2].minor.yy180, yymsp[0].minor.yy180, NULL, status->scanner_state);
  yy_destructor(13,&yymsp[-1].minor);
}
// 6820 "parser.c"
        break;
      case 308:
// 2575 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_expr("or", yymsp[-2].minor.yy180, yymsp[0].minor.yy180, NULL, status->scanner_state);
  yy_destructor(12,&yymsp[-1].minor);
}
// 6828 "parser.c"
        break;
      case 309:
// 2580 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_expr("bitwise_and", yymsp[-2].minor.yy180, yymsp[0].minor.yy180, NULL, status->scanner_state);
  yy_destructor(15,&yymsp[-1].minor);
}
// 6836 "parser.c"
        break;
      case 310:
// 2585 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_expr("bitwise_or", yymsp[-2].minor.yy180, yymsp[0].minor.yy180, NULL, status->scanner_state);
  yy_destructor(14,&yymsp[-1].minor);
}
// 6844 "parser.c"
        break;
      case 311:
// 2590 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_expr("bitwise_xor", yymsp[-2].minor.yy180, yymsp[0].minor.yy180, NULL, status->scanner_state);
  yy_destructor(16,&yymsp[-1].minor);
}
// 6852 "parser.c"
        break;
      case 312:
// 2595 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_expr("bitwise_shiftleft", yymsp[-2].minor.yy180, yymsp[0].minor.yy180, NULL, status->scanner_state);
  yy_destructor(17,&yymsp[-1].minor);
}
// 6860 "parser.c"
        break;
      case 313:
// 2600 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_expr("bitwise_shiftright", yymsp[-2].minor.yy180, yymsp[0].minor.yy180, NULL, status->scanner_state);
  yy_destructor(18,&yymsp[-1].minor);
}
// 6868 "parser.c"
        break;
      case 314:
// 2605 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_expr("instanceof", yymsp[-2].minor.yy180, yymsp[0].minor.yy180, NULL, status->scanner_state);
  yy_destructor(11,&yymsp[-1].minor);
}
// 6876 "parser.c"
        break;
      case 315:
// 2610 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_expr("fetch", zephir_ret_literal(ZEPHIR_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), yymsp[0].minor.yy180, NULL, status->scanner_state);
  yy_destructor(34,&yymsp[-3].minor);
  yy_destructor(6,&yymsp[-1].minor);
}
// 6885 "parser.c"
        break;
      case 317:
// 2620 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_expr("typeof", yymsp[0].minor.yy180, NULL, NULL, status->scanner_state);
  yy_destructor(36,&yymsp[-1].minor);
}
// 6893 "parser.c"
        break;
      case 319:
      case 365:
      case 367:
      case 384:
// 2630 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_literal(ZEPHIR_T_INTEGER, yymsp[0].minor.yy0, status->scanner_state);
}
// 6903 "parser.c"
        break;
      case 320:
      case 364:
      case 369:
      case 383:
// 2635 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_literal(ZEPHIR_T_STRING, yymsp[0].minor.yy0, status->scanner_state);
}
// 6913 "parser.c"
        break;
      case 321:
      case 368:
// 2640 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_literal(ZEPHIR_T_CHAR, yymsp[0].minor.yy0, status->scanner_state);
}
// 6921 "parser.c"
        break;
      case 322:
      case 370:
// 2645 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_literal(ZEPHIR_T_DOUBLE, yymsp[0].minor.yy0, status->scanner_state);
}
// 6929 "parser.c"
        break;
      case 323:
      case 371:
// 2650 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_literal(ZEPHIR_T_NULL, NULL, status->scanner_state);
  yy_destructor(64,&yymsp[0].minor);
}
// 6938 "parser.c"
        break;
      case 324:
      case 373:
// 2655 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_literal(ZEPHIR_T_TRUE, NULL, status->scanner_state);
  yy_destructor(115,&yymsp[0].minor);
}
// 6947 "parser.c"
        break;
      case 325:
      case 372:
// 2660 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_literal(ZEPHIR_T_FALSE, NULL, status->scanner_state);
  yy_destructor(116,&yymsp[0].minor);
}
// 6956 "parser.c"
        break;
      case 326:
      case 375:
// 2665 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_literal(ZEPHIR_T_CONSTANT, yymsp[0].minor.yy0, status->scanner_state);
}
// 6964 "parser.c"
        break;
      case 327:
      case 376:
// 2670 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_expr("empty-array", NULL, NULL, NULL, status->scanner_state);
  yy_destructor(41,&yymsp[-1].minor);
  yy_destructor(66,&yymsp[0].minor);
}
// 6974 "parser.c"
        break;
      case 328:
      case 377:
// 2675 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_expr("array", yymsp[-1].minor.yy180, NULL, NULL, status->scanner_state);
  yy_destructor(41,&yymsp[-2].minor);
  yy_destructor(66,&yymsp[0].minor);
}
// 6984 "parser.c"
        break;
      case 329:
// 2680 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_new_instance(0, yymsp[0].minor.yy0, NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-1].minor);
}
// 6992 "parser.c"
        break;
      case 330:
// 2685 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_new_instance(0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-3].minor);
  yy_destructor(61,&yymsp[-1].minor);
  yy_destructor(40,&yymsp[0].minor);
}
// 7002 "parser.c"
        break;
      case 331:
// 2690 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_new_instance(0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy180, status->scanner_state);
  yy_destructor(38,&yymsp[-4].minor);
  yy_destructor(61,&yymsp[-2].minor);
  yy_destructor(40,&yymsp[0].minor);
}
// 7012 "parser.c"
        break;
      case 332:
// 2695 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_new_instance(1, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-3].minor);
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(55,&yymsp[0].minor);
}
// 7022 "parser.c"
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
// 7034 "parser.c"
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
// 7046 "parser.c"
        break;
      case 335:
// 2710 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_fcall(1, yymsp[-3].minor.yy0, yymsp[-1].minor.yy180, status->scanner_state);
  yy_destructor(61,&yymsp[-2].minor);
  yy_destructor(40,&yymsp[0].minor);
}
// 7055 "parser.c"
        break;
      case 336:
// 2715 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_fcall(1, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(61,&yymsp[-1].minor);
  yy_destructor(40,&yymsp[0].minor);
}
// 7064 "parser.c"
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
// 7075 "parser.c"
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
// 7086 "parser.c"
        break;
      case 339:
// 2730 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_scall(0, yymsp[-5].minor.yy0, 0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy180, status->scanner_state);
  yy_destructor(105,&yymsp[-4].minor);
  yy_destructor(61,&yymsp[-2].minor);
  yy_destructor(40,&yymsp[0].minor);
}
// 7096 "parser.c"
        break;
      case 340:
// 2735 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_scall(0, yymsp[-4].minor.yy0, 0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(105,&yymsp[-3].minor);
  yy_destructor(61,&yymsp[-1].minor);
  yy_destructor(40,&yymsp[0].minor);
}
// 7106 "parser.c"
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
// 7118 "parser.c"
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
// 7130 "parser.c"
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
// 7144 "parser.c"
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
// 7158 "parser.c"
        break;
      case 345:
// 2760 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_mcall(1, yymsp[-5].minor.yy180, yymsp[-3].minor.yy0, yymsp[-1].minor.yy180, status->scanner_state);
  yy_destructor(42,&yymsp[-4].minor);
  yy_destructor(61,&yymsp[-2].minor);
  yy_destructor(40,&yymsp[0].minor);
}
// 7168 "parser.c"
        break;
      case 346:
// 2765 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_mcall(1, yymsp[-4].minor.yy180, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(42,&yymsp[-3].minor);
  yy_destructor(61,&yymsp[-1].minor);
  yy_destructor(40,&yymsp[0].minor);
}
// 7178 "parser.c"
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
// 7190 "parser.c"
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
// 7202 "parser.c"
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
// 7214 "parser.c"
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
// 7226 "parser.c"
        break;
      case 354:
// 2805 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_expr("ternary", yymsp[-4].minor.yy180, yymsp[-2].minor.yy180, yymsp[0].minor.yy180, status->scanner_state);
  yy_destructor(8,&yymsp[-3].minor);
  yy_destructor(87,&yymsp[-1].minor);
}
// 7235 "parser.c"
        break;
      case 357:
// 2817 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_call_parameter(NULL, yymsp[0].minor.yy180, status->scanner_state);
}
// 7242 "parser.c"
        break;
      case 358:
// 2821 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_call_parameter(yymsp[-2].minor.yy0, yymsp[0].minor.yy180, status->scanner_state);
  yy_destructor(87,&yymsp[-1].minor);
}
// 7250 "parser.c"
        break;
      case 361:
      case 380:
// 2833 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_array_item(yymsp[-2].minor.yy180, yymsp[0].minor.yy180, status->scanner_state);
  yy_destructor(87,&yymsp[-1].minor);
}
// 7259 "parser.c"
        break;
      case 362:
      case 381:
// 2837 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_array_item(NULL, yymsp[0].minor.yy180, status->scanner_state);
}
// 7267 "parser.c"
        break;
      case 387:
// 2938 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_comment(yymsp[0].minor.yy0, status->scanner_state);
}
// 7274 "parser.c"
        break;
      case 388:
// 2942 "parser.lemon"
{
	yygotominor.yy180 = zephir_ret_cblock(yymsp[0].minor.yy0, status->scanner_state);
}
// 7281 "parser.c"
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

// 7356 "parser.c"
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
 | Copyright (c) 2013-2015 Zephir Team and contributors                     |
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
	{ ZEPHIR_T_IDENTIFIER,    		"IDENTIFIER" },
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
 * Parses a program returning an intermediate array representation
 */
int zephir_parse_program(zval **return_value, char *program, unsigned int program_length, const char *file_path, zval **error_msg TSRMLS_DC) {

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
				if (!*error_msg) {
                    unsigned int length = sizeof(char) * 48; // Z_STRLEN_P(state->active_file);
					error = emalloc(length);
					snprintf(error, length, "Scanner: unknown opcode %d on in %s line %d", token.opcode, state->active_file, state->active_line);
					ALLOC_INIT_ZVAL(*error_msg);
					ZVAL_STRING(*error_msg, error, 1);
					efree(error);
				}
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
					char *x = emalloc(sizeof(char) * 1024);
					if (state->start) {
						snprintf(x, 1024, "Scanner error: %d %s", scanner_status, state->start);
					} else {
						snprintf(x, 1024, "Scanner error: %d", scanner_status);
					}
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
		if (parser_status->syntax_error) {
			if (!*error_msg) {
				ALLOC_INIT_ZVAL(*error_msg);
				ZVAL_STRING(*error_msg, parser_status->syntax_error, 1);
			}
			efree(parser_status->syntax_error);
		}
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
		//zend_print_zval_r(parser_status->ret, 0 TSRMLS_CC);
		*return_value = parser_status->ret;
	}

	//efree(Z_STRVAL(processed_comment));*/

	efree(parser_status);
	efree(state);

	return status;
}
