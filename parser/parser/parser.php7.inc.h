
#include <php.h>
#include "string.h"
#include "parser.php7.h"
#include "xx.h"
#include "scanner.h"

#define SL(str) ZEND_STRL(str)

static void parser_add_str(zval *arr, const char *key, const char *val) {
	zval tmp;
	zend_string *tmp_str = zend_string_init(val, strlen(val), 0);
	ZVAL_STR(&tmp, tmp_str);
	zend_hash_str_add(Z_ARRVAL_P(arr), key, strlen(key), &tmp);
}

static void parser_add_str_free(zval *arr, const char *key, char *val) {
	zval tmp;
	zend_string *tmp_str = zend_string_init(val, strlen(val), 0);
	ZVAL_STR(&tmp, tmp_str);
	zend_hash_str_add(Z_ARRVAL_P(arr), key, strlen(key), &tmp);
	efree(val);
}

static void parser_add_int(zval *arr, const char *key, int i) {
	zval tmp;
	ZVAL_LONG(&tmp, i);
	zend_hash_str_add(Z_ARRVAL_P(arr), key, strlen(key), &tmp);
}

static void parser_add_zval(zval *arr, const char *key, zval *zv) {
	zend_hash_str_add(Z_ARRVAL_P(arr), key, strlen(key), zv);
}

static void parser_array_append(zval *arr, zval *zv) {
	add_next_index_zval(arr, zv);
}

static void parser_get_string(zval *tmp, const char *str) {
	zend_string *zs = zend_string_init(str, strlen(str), 0);
	ZVAL_STR(tmp, zs);
}

static void xx_ret_literal(zval *ret, int type, xx_parser_token *T, xx_scanner_state *state)
{
	array_init(ret);

	switch (type) {

		case XX_T_CONSTANT:
			parser_add_str(ret, "type", "constant");
			break;

		case XX_T_IDENTIFIER:
			parser_add_str(ret, "type", "variable");
			break;

		case XX_T_INTEGER:
			parser_add_str(ret, "type", "int");
			break;

		case XX_T_DOUBLE:
			parser_add_str(ret, "type", "double");
			break;

		case XX_T_NULL:
			parser_add_str(ret, "type", "null");
			break;

		case XX_T_STRING:
			parser_add_str(ret, "type", "string");
			break;

		case XX_T_ISTRING:
			parser_add_str(ret, "type", "istring");
			break;

		case XX_T_CHAR:
			parser_add_str(ret, "type", "char");
			break;

		default:
			if (type == XX_T_TRUE) {
				parser_add_str(ret, "type", "bool");
				parser_add_str(ret, "value", "true");
			} else {
				if (type == XX_T_FALSE) {
					parser_add_str(ret, "type", "bool");
					parser_add_str(ret, "value", "false");
				} else {
					fprintf(stderr, "literal??\n");
				}
			}
	}

	if (T) {
		parser_add_str_free(ret, "value", T->token);
		efree(T);
	}

	parser_add_str(ret, "file", state->active_file);
	parser_add_int(ret, "line", state->active_line);
	parser_add_int(ret, "char", state->active_char);
}

static void xx_ret_expr(zval *ret, const char *type, zval *left, zval *right, zval *extra, xx_scanner_state *state)
{
	array_init(ret);

	parser_add_str(ret, "type", type);

	if (left) {
		parser_add_zval(ret, "left", left);
	}
	if (right) {
		parser_add_zval(ret, "right", right);
	}
	if (extra) {
		parser_add_zval(ret, "extra", extra);
	}

	parser_add_str(ret, "file", state->active_file);
	parser_add_int(ret, "line", state->active_line);
	parser_add_int(ret, "char", state->active_char);
}

static void xx_ret_array_item(zval *ret, zval *key, zval *value, xx_scanner_state *state)
{
	array_init(ret);

	if (key) {
		parser_add_zval(ret, "key", key);
	}
	parser_add_zval(ret, "value", value);

	parser_add_str(ret, "file", state->active_file);
	parser_add_int(ret, "line", state->active_line);
	parser_add_int(ret, "char", state->active_char);
}

static void xx_ret_namespace(zval *ret, xx_parser_token *T, xx_scanner_state *state)
{
	array_init(ret);

	parser_add_str(ret, "type", "namespace");
	parser_add_str_free(ret, "name", T->token);
	efree(T);

	parser_add_str(ret, "file", state->active_file);
	parser_add_int(ret, "line", state->active_line);
	parser_add_int(ret, "char", state->active_char);
}

static void xx_ret_use_aliases(zval *ret, zval *use_aliases_list, xx_scanner_state *state)
{
	array_init(ret);

	parser_add_str(ret, "type", "use");
	parser_add_zval(ret, "aliases", use_aliases_list);

	parser_add_str(ret, "file", state->active_file);
	parser_add_int(ret, "line", state->active_line);
	parser_add_int(ret, "char", state->active_char);
}

static void xx_ret_use_aliases_item(zval *ret, xx_parser_token *T, xx_parser_token *A, xx_scanner_state *state)
{
	array_init(ret);

	parser_add_str_free(ret, "name", T->token);
	efree(T);

	if (A) {
		parser_add_str_free(ret, "alias", A->token);
		efree(A);
	}

	parser_add_str(ret, "file", state->active_file);
	parser_add_int(ret, "line", state->active_line);
	parser_add_int(ret, "char", state->active_char);
}

static void xx_ret_function(zval *ret, xx_parser_token *T, zval *parameters,
	zval *statements, xx_parser_token *D, zval *return_type, xx_scanner_state *state)
{
	array_init(ret);

	parser_add_str(ret, "type", "function");
	parser_add_str_free(ret, "name", T->token);
	efree(T);

	if (parameters) {
		parser_add_zval(ret, "parameters", parameters);
	}

	if (statements) {
		parser_add_zval(ret, "statements", statements);
	}

	if (D) {
		parser_add_str_free(ret, "docblock", D->token);
		efree(D);
	}

	if (return_type) {
		parser_add_zval(ret, "return-type", return_type);
	}

	parser_add_str(ret, "file", state->active_file);
	parser_add_int(ret, "line", state->method_line);
	parser_add_int(ret, "char", state->method_char);
}

static void xx_ret_class(zval *ret, xx_parser_token *T, zval *class_definition, int is_abstract, int is_final,
	xx_parser_token *E, zval *I, xx_scanner_state *state)
{
	array_init(ret);

	parser_add_str(ret, "type", "class");
	parser_add_str_free(ret, "name", T->token);
	efree(T);

	parser_add_int(ret, "abstract", is_abstract);
	parser_add_int(ret, "final", is_final);

	if (E) {
		parser_add_str_free(ret, "extends", E->token);
		efree(E);
	}

	if (I) {
		parser_add_zval(ret, "implements", I);
	}

	if (class_definition) {
		parser_add_zval(ret, "definition", class_definition);
	}

	parser_add_str(ret, "file", state->active_file);
	parser_add_int(ret, "line", state->class_line);
	parser_add_int(ret, "char", state->class_char);
}

static void xx_ret_interface(zval *ret, xx_parser_token *T, zval *interface_definition, zval *extends_list, xx_scanner_state *state)
{
	array_init(ret);

	parser_add_str(ret, "type", "interface");
	parser_add_str_free(ret, "name", T->token);
	efree(T);

	if (extends_list) {
		parser_add_zval(ret, "extends", extends_list);
	}

	if (interface_definition) {
		parser_add_zval(ret, "definition", interface_definition);
	}

	parser_add_str(ret, "file", state->active_file);
	parser_add_int(ret, "line", state->class_line);
	parser_add_int(ret, "char", state->class_char);
}

static void xx_ret_class_definition(zval *ret, zval *properties, zval *methods, zval *constants, xx_scanner_state *state)
{
	array_init(ret);

	if (properties) {
		parser_add_zval(ret, "properties", properties);
	}
	if (methods) {
		parser_add_zval(ret, "methods", methods);
	}
	if (constants) {
		parser_add_zval(ret, "constants", constants);
	}

	parser_add_str(ret, "file", state->active_file);
	parser_add_int(ret, "line", state->class_line);
	parser_add_int(ret, "char", state->class_char);
}

static void xx_ret_interface_definition(zval *ret, zval *methods, zval *constants, xx_scanner_state *state)
{
	array_init(ret);

	if (methods) {
		parser_add_zval(ret, "methods", methods);
	}
	if (constants) {
		parser_add_zval(ret, "constants", constants);
	}

	parser_add_str(ret, "file", state->active_file);
	parser_add_int(ret, "line", state->active_line);
	parser_add_int(ret, "char", state->active_char);
}

static void xx_ret_class_property(zval *ret, zval *visibility, xx_parser_token *T,
		zval *default_value, xx_parser_token *D, zval *shortcuts, xx_scanner_state *state)
{
	array_init(ret);

	parser_add_zval(ret, "visibility", visibility);
	parser_add_str(ret, "type", "property");

	parser_add_str_free(ret, "name", T->token);
	efree(T);

	if (default_value) {
		parser_add_zval(ret, "default", default_value);
	}

	if (D) {
		parser_add_str_free(ret, "docblock", D->token);
		efree(D);
	}

	if (shortcuts) {
		parser_add_zval(ret, "shortcuts", shortcuts);
	}

	parser_add_str(ret, "file", state->active_file);
	parser_add_int(ret, "line", state->active_line);
	parser_add_int(ret, "char", state->active_char);
}

static void xx_ret_property_shortcut(zval *ret, xx_parser_token *C, xx_parser_token *D, xx_scanner_state *state)
{
	array_init(ret);

	parser_add_str(ret, "type", "shortcut");
	if (C) {
		parser_add_str_free(ret, "docblock", C->token);
		efree(C);
	}

	parser_add_str_free(ret, "name", D->token);
	efree(D);

	parser_add_str(ret, "file", state->active_file);
	parser_add_int(ret, "line", state->active_line);
	parser_add_int(ret, "char", state->active_char);
}

static void xx_ret_class_const(zval *ret, xx_parser_token *T, zval *default_value, xx_parser_token *D, xx_scanner_state *state)
{
	array_init(ret);

	parser_add_str(ret, "type", "const");
	parser_add_str_free(ret, "name", T->token);
	efree(T);

	parser_add_zval(ret, "default", default_value);

	if (D) {
		parser_add_str_free(ret, "docblock", D->token);
		efree(D);
	}

	parser_add_str(ret, "file", state->active_file);
	parser_add_int(ret, "line", state->active_line);
	parser_add_int(ret, "char", state->active_char);
}

static void xx_ret_class_method(zval *ret, zval *visibility, xx_parser_token *T, zval *parameters,
	zval *statements, xx_parser_token *D, zval *return_type, xx_scanner_state *state)
{
	array_init(ret);

	parser_add_zval(ret, "visibility", visibility);
	parser_add_str(ret, "type", "method");

	parser_add_str_free(ret, "name", T->token);
	efree(T);

	if (parameters) {
		parser_add_zval(ret, "parameters", parameters);
	}

	if (statements) {
		parser_add_zval(ret, "statements", statements);
	}

	if (D) {
		parser_add_str_free(ret, "docblock", D->token);
		efree(D);
	}

	if (return_type) {
		parser_add_zval(ret, "return-type", return_type);
	}

	parser_add_str(ret, "file", state->active_file);
	parser_add_int(ret, "line", state->method_line);
	parser_add_int(ret, "last-line", state->active_line);
	parser_add_int(ret, "char", state->method_char);
}

static void xx_ret_parameter(zval *ret, int const_param, zval *type, zval *cast, xx_parser_token *N, zval *default_value,
	int mandatory, int reference, xx_scanner_state *state)
{
	array_init(ret);

	parser_add_str(ret, "type", "parameter");
	parser_add_str_free(ret, "name", N->token);
	efree(N);

	parser_add_int(ret, "const", const_param);

	if (type) {
		parser_add_zval(ret, "data-type", type);
		parser_add_int(ret, "mandatory", mandatory);
	} else {
		parser_add_str(ret, "data-type", "variable");
		parser_add_int(ret, "mandatory", 0);
	}

	if (cast) {
		parser_add_zval(ret, "cast", cast);
	}
	if (default_value) {
		parser_add_zval(ret, "default", default_value);
	}

    parser_add_int(ret, "reference", reference);

	parser_add_str(ret, "file", state->active_file);
	parser_add_int(ret, "line", state->active_line);
	parser_add_int(ret, "char", state->active_char);
}

static void xx_ret_return_type(zval *ret, int is_void, zval *return_type_list, xx_scanner_state *state)
{
	array_init(ret);

	parser_add_str(ret, "type", "return-type");

    if (return_type_list) {
	   parser_add_zval(ret, "list", return_type_list);
    }

	parser_add_int(ret, "void", is_void);

	parser_add_str(ret, "file", state->active_file);
	parser_add_int(ret, "line", state->active_line);
	parser_add_int(ret, "char", state->active_char);
}

static void xx_ret_return_type_item(zval *ret, zval *type, zval *cast, int mandatory, int collection, xx_scanner_state *state)
{
	array_init(ret);

	parser_add_str(ret, "type", "return-type-parameter");

	if (type) {
		parser_add_zval(ret, "data-type", type);
		parser_add_int(ret, "mandatory", mandatory);
	}

	if (cast) {
		parser_add_zval(ret, "cast", cast);
		parser_add_int(ret, "collection", collection);
	}

	parser_add_str(ret, "file", state->active_file);
	parser_add_int(ret, "line", state->active_line);
	parser_add_int(ret, "char", state->active_char);
}

static void xx_ret_type(zval *ret, int type)
{
	switch (type) {

		case XX_TYPE_INTEGER:
			parser_get_string(ret, "int");
            return;

		case XX_TYPE_UINTEGER:
			parser_get_string(ret, "uint");
            return;

		case XX_TYPE_DOUBLE:
			parser_get_string(ret, "double");
            return;

		case XX_TYPE_BOOL:
			parser_get_string(ret, "bool");
            return;

		case XX_TYPE_LONG:
			parser_get_string(ret, "long");
            return;

		case XX_TYPE_ULONG:
			parser_get_string(ret, "ulong");
            return;

		case XX_TYPE_STRING:
			parser_get_string(ret, "string");
            return;

		case XX_TYPE_CHAR:
			parser_get_string(ret, "char");
            return;

		case XX_TYPE_ARRAY:
			parser_get_string(ret, "array");
            return;

		case XX_TYPE_VAR:
			parser_get_string(ret, "variable");
            return;

		case XX_TYPE_CALLABLE:
			parser_get_string(ret, "callable");
            return;

		case XX_TYPE_RESOURCE:
			parser_get_string(ret, "resource");
            return;

		case XX_TYPE_OBJECT:
			parser_get_string(ret, "object");
            return;

		case XX_T_TYPE_NULL:
			parser_get_string(ret, "null");
            return;

		case XX_T_TYPE_THIS:
			parser_get_string(ret, "this");
            return;

		default:
			fprintf(stderr, "unknown type?\n");
	}

	parser_get_string(ret, "unknown");
}

static void xx_ret_list(zval *ret, zval *list_left, zval *list_right, xx_scanner_state *state)
{
	zval *val;

	array_init(ret);

	if (list_left) {
		if (Z_TYPE_P(list_left) == IS_ARRAY) {
			ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(list_left), val) {
				Z_TRY_ADDREF_P(val);
				parser_array_append(ret, val);
			} ZEND_HASH_FOREACH_END();
			zval_ptr_dtor(list_left);
		} else {
			parser_array_append(ret, list_left);
		}
	}

	parser_array_append(ret, list_right);
}

static void xx_ret_let_statement(zval *ret, zval *assignments, xx_scanner_state *state)
{
	array_init(ret);

	parser_add_str(ret, "type", "let");
	parser_add_zval(ret, "assignments", assignments);

	parser_add_str(ret, "file", state->active_file);
	parser_add_int(ret, "line", state->active_line);
	parser_add_int(ret, "char", state->active_char);
}

static void xx_ret_let_assignment(zval *ret, char *type, zval *operator, xx_parser_token *V, xx_parser_token *P, zval *index_expr, zval *expr, xx_scanner_state *state)
{
	array_init(ret);

	parser_add_str(ret, "assign-type", type);
	if (operator) {
		parser_add_zval(ret, "operator", operator);
	}

	parser_add_str_free(ret, "variable", V->token);
	efree(V);

	if (P) {
		parser_add_str_free(ret, "property", P->token);
		efree(P);
	}
	if (index_expr) {
		parser_add_zval(ret, "index-expr", index_expr);
	}
	if (expr) {
		parser_add_zval(ret, "expr", expr);
	}

	parser_add_str(ret, "file", state->active_file);
	parser_add_int(ret, "line", state->active_line);
	parser_add_int(ret, "char", state->active_char);
}

static void xx_ret_if_statement(zval *ret, zval *expr, zval *statements, zval *elseif_statements, zval *else_statements, xx_scanner_state *state)
{
	array_init(ret);

	parser_add_str(ret, "type", "if");
	parser_add_zval(ret, "expr", expr);

	if (statements) {
		parser_add_zval(ret, "statements", statements);
	}

	if (elseif_statements) {
		parser_add_zval(ret, "elseif_statements", elseif_statements);
	}

	if (else_statements) {
		parser_add_zval(ret, "else_statements", else_statements);
	}

	parser_add_str(ret, "file", state->active_file);
	parser_add_int(ret, "line", state->active_line);
	parser_add_int(ret, "char", state->active_char);
}

static void xx_ret_switch_statement(zval *ret, zval *expr, zval *clauses, xx_scanner_state *state)
{
	array_init(ret);

	parser_add_str(ret, "type", "switch");
	parser_add_zval(ret, "expr", expr);

	if (clauses) {
		parser_add_zval(ret, "clauses", clauses);
	}

	parser_add_str(ret, "file", state->active_file);
	parser_add_int(ret, "line", state->active_line);
	parser_add_int(ret, "char", state->active_char);
}

static void xx_ret_case_clause(zval *ret, zval *expr, zval *statements, xx_scanner_state *state)
{
	array_init(ret);

	if (expr) {
		parser_add_str(ret, "type", "case");
		parser_add_zval(ret, "expr", expr);
	} else {
		parser_add_str(ret, "type", "default");
	}

	if (statements) {
		parser_add_zval(ret, "statements", statements);
	}

	parser_add_str(ret, "file", state->active_file);
	parser_add_int(ret, "line", state->active_line);
	parser_add_int(ret, "char", state->active_char);
}

static void xx_ret_while_statement(zval *ret, zval *expr, zval *statements, xx_scanner_state *state)
{
	array_init(ret);

	parser_add_str(ret, "type", "while");
	parser_add_zval(ret, "expr", expr);

	if (statements) {
		parser_add_zval(ret, "statements", statements);
	}

	parser_add_str(ret, "file", state->active_file);
	parser_add_int(ret, "line", state->active_line);
	parser_add_int(ret, "char", state->active_char);
}

static void xx_ret_do_while_statement(zval *ret, zval *expr, zval *statements, xx_scanner_state *state)
{
	array_init(ret);

	parser_add_str(ret, "type", "do-while");
	parser_add_zval(ret, "expr", expr);

	if (statements) {
		parser_add_zval(ret, "statements", statements);
	}

	parser_add_str(ret, "file", state->active_file);
	parser_add_int(ret, "line", state->active_line);
	parser_add_int(ret, "char", state->active_char);
}

static void xx_ret_try_catch_statement(zval *ret, zval *statements, zval *catches, xx_scanner_state *state)
{
	array_init(ret);

	parser_add_str(ret, "type", "try-catch");

	if (statements) {
		parser_add_zval(ret, "statements", statements);
	}
	if (catches) {
		parser_add_zval(ret, "catches", catches);
	}

	parser_add_str(ret, "file", state->active_file);
	parser_add_int(ret, "line", state->active_line);
	parser_add_int(ret, "char", state->active_char);
}

static void xx_ret_catch_statement(zval *ret, zval *classes, zval *variable, zval *statements, xx_scanner_state *state)
{
	array_init(ret);

	if (classes) {
		parser_add_zval(ret, "classes", classes);
	}

	if (variable) {
		parser_add_zval(ret, "variable", variable);
	}

	if (statements) {
		parser_add_zval(ret, "statements", statements);
	}

	parser_add_str(ret, "file", state->active_file);
	parser_add_int(ret, "line", state->active_line);
	parser_add_int(ret, "char", state->active_char);
}

static void xx_ret_for_statement(zval *ret, zval *expr, xx_parser_token *K, xx_parser_token *V, int reverse, zval *statements, xx_scanner_state *state)
{
	array_init(ret);

	parser_add_str(ret, "type", "for");
	parser_add_zval(ret, "expr", expr);

	if (K) {
		parser_add_str_free(ret, "key", K->token);
		efree(K);
	}

	if (V) {
		parser_add_str_free(ret, "value", V->token);
		efree(V);
	}

	parser_add_int(ret, "reverse", reverse);

	if (statements) {
		parser_add_zval(ret, "statements", statements);
	}

	parser_add_str(ret, "file", state->active_file);
	parser_add_int(ret, "line", state->active_line);
	parser_add_int(ret, "char", state->active_char);
}

static void xx_ret_loop_statement(zval *ret, zval *statements, xx_scanner_state *state)
{
	array_init(ret);

	parser_add_str(ret, "type", "loop");

	if (statements) {
		parser_add_zval(ret, "statements", statements);
	}

	parser_add_str(ret, "file", state->active_file);
	parser_add_int(ret, "line", state->active_line);
	parser_add_int(ret, "char", state->active_char);
}

static void xx_ret_empty_statement(zval *ret, xx_scanner_state *state)
{
	array_init(ret);

	parser_add_str(ret, "type", "empty");

	parser_add_str(ret, "file", state->active_file);
	parser_add_int(ret, "line", state->active_line);
	parser_add_int(ret, "char", state->active_char);
}

static void xx_ret_break_statement(zval *ret, xx_scanner_state *state)
{
	array_init(ret);

	parser_add_str(ret, "type", "break");

	parser_add_str(ret, "file", state->active_file);
	parser_add_int(ret, "line", state->active_line);
	parser_add_int(ret, "char", state->active_char);
}

static void xx_ret_continue_statement(zval *ret, xx_scanner_state *state)
{
	array_init(ret);

	parser_add_str(ret, "type", "continue");

	parser_add_str(ret, "file", state->active_file);
	parser_add_int(ret, "line", state->active_line);
	parser_add_int(ret, "char", state->active_char);
}

static void xx_ret_echo_statement(zval *ret, zval *expressions, xx_scanner_state *state)
{
	array_init(ret);

	parser_add_str(ret, "type", "echo");
	parser_add_zval(ret, "expressions", expressions);

	parser_add_str(ret, "file", state->active_file);
	parser_add_int(ret, "line", state->active_line);
	parser_add_int(ret, "char", state->active_char);
}

static void xx_ret_return_statement(zval *ret, zval *expr, xx_scanner_state *state)
{
	array_init(ret);

	parser_add_str(ret, "type", "return");
	if (expr) {
		parser_add_zval(ret, "expr", expr);
	}

	parser_add_str(ret, "file", state->active_file);
	parser_add_int(ret, "line", state->active_line);
	parser_add_int(ret, "char", state->active_char);
}

static void xx_ret_require_statement(zval *ret, zval *expr, xx_scanner_state *state)
{
	array_init(ret);

	parser_add_str(ret, "type", "require");
	parser_add_zval(ret, "expr", expr);

	parser_add_str(ret, "file", state->active_file);
	parser_add_int(ret, "line", state->active_line);
	parser_add_int(ret, "char", state->active_char);
}

static void xx_ret_fetch_statement(zval *ret, zval *expr, xx_scanner_state *state)
{
	array_init(ret);

	parser_add_str(ret, "type", "fetch");
	parser_add_zval(ret, "expr", expr);

	parser_add_str(ret, "file", state->active_file);
	parser_add_int(ret, "line", state->active_line);
	parser_add_int(ret, "char", state->active_char);
}

static void xx_ret_fcall_statement(zval *ret, zval *expr, xx_scanner_state *state)
{
	array_init(ret);

	parser_add_str(ret, "type", "fcall");
	parser_add_zval(ret, "expr", expr);

	parser_add_str(ret, "file", state->active_file);
	parser_add_int(ret, "line", state->active_line);
	parser_add_int(ret, "char", state->active_char);
}

static void xx_ret_mcall_statement(zval *ret, zval *expr, xx_scanner_state *state)
{
	array_init(ret);

	parser_add_str(ret, "type", "mcall");
	parser_add_zval(ret, "expr", expr);

	parser_add_str(ret, "file", state->active_file);
	parser_add_int(ret, "line", state->active_line);
	parser_add_int(ret, "char", state->active_char);
}

static void xx_ret_scall_statement(zval *ret, zval *expr, xx_scanner_state *state)
{
	array_init(ret);

	parser_add_str(ret, "type", "scall");
	parser_add_zval(ret, "expr", expr);

	parser_add_str(ret, "file", state->active_file);
	parser_add_int(ret, "line", state->active_line);
	parser_add_int(ret, "char", state->active_char);
}

static void xx_ret_unset_statement(zval *ret, zval *expr, xx_scanner_state *state)
{
	array_init(ret);

	parser_add_str(ret, "type", "unset");
	parser_add_zval(ret, "expr", expr);

	parser_add_str(ret, "file", state->active_file);
	parser_add_int(ret, "line", state->active_line);
	parser_add_int(ret, "char", state->active_char);
}

static void xx_ret_declare_statement(zval *ret, int type, zval *variables, xx_scanner_state *state)
{
	array_init(ret);

	parser_add_str(ret, "type", "declare");

	switch (type) {

		case XX_T_TYPE_INTEGER:
			parser_add_str(ret, "data-type", "int");
			break;

		case XX_T_TYPE_UINTEGER:
			parser_add_str(ret, "data-type", "uint");
			break;

		case XX_T_TYPE_LONG:
			parser_add_str(ret, "data-type", "long");
			break;

		case XX_T_TYPE_ULONG:
			parser_add_str(ret, "data-type", "ulong");
			break;

		case XX_T_TYPE_CHAR:
			parser_add_str(ret, "data-type", "char");
			break;

		case XX_T_TYPE_UCHAR:
			parser_add_str(ret, "data-type", "uchar");
			break;

		case XX_T_TYPE_DOUBLE:
			parser_add_str(ret, "data-type", "double");
			break;

		case XX_T_TYPE_BOOL:
			parser_add_str(ret, "data-type", "bool");
			break;

		case XX_T_TYPE_STRING:
			parser_add_str(ret, "data-type", "string");
			break;

		case XX_T_TYPE_ARRAY:
			parser_add_str(ret, "data-type", "array");
			break;

		case XX_T_TYPE_VAR:
			parser_add_str(ret, "data-type", "variable");
			break;

		case XX_T_TYPE_CALLABLE:
			parser_add_str(ret, "data-type", "callable");
			break;

		case XX_T_TYPE_RESOURCE:
			parser_add_str(ret, "data-type", "resource");
			break;

		case XX_T_TYPE_OBJECT:
			parser_add_str(ret, "data-type", "object");
			break;

		default:
			fprintf(stderr, "err 2?\n");
	}

	parser_add_zval(ret, "variables", variables);

	parser_add_str(ret, "file", state->active_file);
	parser_add_int(ret, "line", state->active_line);
	parser_add_int(ret, "char", state->active_char);
}

static void xx_ret_declare_variable(zval *ret, xx_parser_token *T, zval *expr, xx_scanner_state *state)
{
	array_init(ret);

	parser_add_str_free(ret, "variable", T->token);
	efree(T);

	if (expr) {
		parser_add_zval(ret, "expr", expr);
	}

	parser_add_str(ret, "file", state->active_file);
	parser_add_int(ret, "line", state->active_line);
	parser_add_int(ret, "char", state->active_char);
}

static void xx_ret_new_static_instance(zval *ret, zval *parameters, xx_scanner_state *state)
{
	array_init(ret);

	parser_add_str(ret, "type", "new");
	parser_add_str(ret, "class", "static");
	parser_add_int(ret, "dynamic", 0);

	if (parameters) {
		parser_add_zval(ret, "parameters", parameters);
	}

	parser_add_str(ret, "file", state->active_file);
	parser_add_int(ret, "line", state->active_line);
	parser_add_int(ret, "char", state->active_char);
}

static void xx_ret_new_instance(zval *ret, int dynamic, xx_parser_token *T, zval *parameters, xx_scanner_state *state)
{
	array_init(ret);

	parser_add_str(ret, "type", "new");

	parser_add_str_free(ret, "class", T->token);
	efree(T);

	parser_add_int(ret, "dynamic", dynamic);

	if (parameters) {
		parser_add_zval(ret, "parameters", parameters);
	}

	parser_add_str(ret, "file", state->active_file);
	parser_add_int(ret, "line", state->active_line);
	parser_add_int(ret, "char", state->active_char);
}

static void xx_ret_new_instance_type(zval *ret, zval *type, zval *parameters, xx_scanner_state *state)
{
	array_init(ret);

	parser_add_str(ret, "type", "new-type");
	parser_add_zval(ret, "internal-type", type);

	if (parameters) {
		parser_add_zval(ret, "parameters", parameters);
	}

	parser_add_str(ret, "file", state->active_file);
	parser_add_int(ret, "line", state->active_line);
	parser_add_int(ret, "char", state->active_char);
}

static void xx_ret_throw_exception(zval *ret, zval *expr, xx_scanner_state *state)
{
	array_init(ret);

	parser_add_str(ret, "type", "throw");
	if (expr) {
		parser_add_zval(ret, "expr", expr);
	}

	parser_add_str(ret, "file", state->active_file);
	parser_add_int(ret, "line", state->active_line);
	parser_add_int(ret, "char", state->active_char);
}

static void xx_ret_fcall(zval *ret, int type, xx_parser_token *F, zval *parameters, xx_scanner_state *state)
{
	array_init(ret);

	parser_add_str(ret, "type", "fcall");

	parser_add_str_free(ret, "name", F->token);
	efree(F);

	parser_add_int(ret, "call-type", type);

	if (parameters) {
		parser_add_zval(ret, "parameters", parameters);
	}

	parser_add_str(ret, "file", state->active_file);
	parser_add_int(ret, "line", state->active_line);
	parser_add_int(ret, "char", state->active_char);
}

static void xx_ret_mcall(zval *ret, int type, zval *O, xx_parser_token *M, zval *parameters, xx_scanner_state *state)
{
	array_init(ret);

	parser_add_str(ret, "type", "mcall");
	parser_add_zval(ret, "variable", O);

	parser_add_str_free(ret, "name", M->token);
	efree(M);

	parser_add_int(ret, "call-type", type);

	if (parameters) {
		parser_add_zval(ret, "parameters", parameters);
	}

	parser_add_str(ret, "file", state->active_file);
	parser_add_int(ret, "line", state->active_line);
	parser_add_int(ret, "char", state->active_char);
}

static void xx_ret_scall(zval *ret, int dynamic_class, char *class_name, int dynamic_method, xx_parser_token *M, zval *parameters, xx_scanner_state *state)
{
	array_init(ret);

	parser_add_str(ret, "type", "scall");
	parser_add_int(ret, "dynamic-class", dynamic_class);
	parser_add_str(ret, "class", class_name);
	parser_add_int(ret, "dynamic", dynamic_method);

	parser_add_str_free(ret, "name", M->token);
	efree(M);

	if (parameters) {
		parser_add_zval(ret, "parameters", parameters);
	}

	parser_add_str(ret, "file", state->active_file);
	parser_add_int(ret, "line", state->active_line);
	parser_add_int(ret, "char", state->active_char);
}

static void xx_ret_call_parameter(zval *ret, xx_parser_token *N, zval *parameter, xx_scanner_state *state)
{
	array_init(ret);

	if (N) {
		parser_add_str_free(ret, "name", N->token);
		efree(N);
	}

	parser_add_zval(ret, "parameter", parameter);

	parser_add_str(ret, "file", state->active_file);
	parser_add_int(ret, "line", state->active_line);
	parser_add_int(ret, "char", state->active_char);
}

static void xx_ret_comment(zval *ret, xx_parser_token *T, xx_scanner_state *state)
{
	array_init(ret);

	parser_add_str(ret, "type", "comment");

	parser_add_str_free(ret, "value", T->token);
	efree(T);

	parser_add_str(ret, "file", state->active_file);
	parser_add_int(ret, "line", state->active_line);
	parser_add_int(ret, "char", state->active_char);
}

static void xx_ret_cblock(zval *ret, xx_parser_token *T, xx_scanner_state *state)
{
    array_init(ret);

	parser_add_str(ret, "type", "cblock");

	parser_add_str_free(ret, "value", T->token);
	efree(T);

	parser_add_str(ret, "file", state->active_file);
	parser_add_int(ret, "line", state->active_line);
	parser_add_int(ret, "char", state->active_char);
}
