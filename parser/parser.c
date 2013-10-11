/** The author disclaims copyright to this source code.
*/
/* First off, code is include which follows the "include" declaration
** in the input file. */
#include <stdio.h>
// 50 "parser.lemon"


#include "json/json.h"

#include "string.h"
#include "parser.h"
#include "scanner.h"
#include "xx.h"

static json_object *xx_ret_literal(int type, xx_parser_token *T, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	switch (type) {
		case XX_T_CONSTANT:
			json_object_object_add(ret, "type", json_object_new_string("constant"));
			break;
		case XX_T_IDENTIFIER:
			json_object_object_add(ret, "type", json_object_new_string("variable"));
			break;
		case XX_T_INTEGER:
			json_object_object_add(ret, "type", json_object_new_string("int"));
			break;
		case XX_T_DOUBLE:
			json_object_object_add(ret, "type", json_object_new_string("double"));
			break;
		case XX_T_NULL:
			json_object_object_add(ret, "type", json_object_new_string("null"));
			break;
		case XX_T_STRING:
			json_object_object_add(ret, "type", json_object_new_string("string"));
			break;
		case XX_T_CHAR:
			json_object_object_add(ret, "type", json_object_new_string("char"));
			break;
		default:
			if (type == XX_T_TRUE) {
				json_object_object_add(ret, "type", json_object_new_string("bool"));
				json_object_object_add(ret, "value", json_object_new_string("true"));
			} else {
				if (type == XX_T_FALSE) {
					json_object_object_add(ret, "type", json_object_new_string("bool"));
					json_object_object_add(ret, "value", json_object_new_string("false"));
				} else {
					fprintf(stderr, "literal??\n");
				}
			}
	}

	if (T) {
		json_object_object_add(ret, "value", json_object_new_string(T->token));
	}

	json_object_object_add(ret, "file", json_object_new_string(state->active_file));
	json_object_object_add(ret, "line", json_object_new_int(state->active_line));
	json_object_object_add(ret, "char", json_object_new_int(state->active_char));

	return ret;
}

static json_object *xx_ret_expr(char *type, json_object *left, json_object *right, json_object *extra, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string(type));

	if (left) {
		json_object_object_add(ret, "left", left);
	}
	if (right) {
		json_object_object_add(ret, "right", right);
	}
	if (extra) {
		json_object_object_add(ret, "extra", extra);
	}

	json_object_object_add(ret, "file", json_object_new_string(state->active_file));
	json_object_object_add(ret, "line", json_object_new_int(state->active_line));
	json_object_object_add(ret, "char", json_object_new_int(state->active_char));

	return ret;
}

static json_object *xx_ret_array_item(json_object *key, json_object *value, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	if (key) {
		json_object_object_add(ret, "key", key);
	}
	json_object_object_add(ret, "value", value);

	json_object_object_add(ret, "file", json_object_new_string(state->active_file));
	json_object_object_add(ret, "line", json_object_new_int(state->active_line));
	json_object_object_add(ret, "char", json_object_new_int(state->active_char));

	return ret;
}

static json_object *xx_ret_namespace(xx_parser_token *T, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("namespace"));
	json_object_object_add(ret, "name", json_object_new_string(T->token));

	json_object_object_add(ret, "file", json_object_new_string(state->active_file));
	json_object_object_add(ret, "line", json_object_new_int(state->active_line));
	json_object_object_add(ret, "char", json_object_new_int(state->active_char));

	return ret;
}

static json_object *xx_ret_class(xx_parser_token *T, json_object *class_definition, int is_abstract, int is_final,
	xx_parser_token *E, json_object *I, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("class"));
	json_object_object_add(ret, "name", json_object_new_string(T->token));

	json_object_object_add(ret, "abstract", json_object_new_int(is_abstract));
	json_object_object_add(ret, "final", json_object_new_int(is_final));

	if (E) {
		json_object_object_add(ret, "extends", json_object_new_string(E->token));
	}

	if (I) {
		json_object_object_add(ret, "implements", I);
	}

	if (class_definition) {
		json_object_object_add(ret, "definition", class_definition);
	}

	json_object_object_add(ret, "file", json_object_new_string(state->active_file));
	json_object_object_add(ret, "line", json_object_new_int(state->active_line));
	json_object_object_add(ret, "char", json_object_new_int(state->active_char));

	return ret;
}

static json_object *xx_ret_interface(xx_parser_token *T, json_object *methods_definition, xx_parser_token *E, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("interface"));
	json_object_object_add(ret, "name", json_object_new_string(T->token));

	if (E) {
		json_object_object_add(ret, "extends", json_object_new_string(E->token));
	}

	if (methods_definition) {
		json_object_object_add(ret, "methods", methods_definition);
	}

	json_object_object_add(ret, "file", json_object_new_string(state->active_file));
	json_object_object_add(ret, "line", json_object_new_int(state->active_line));
	json_object_object_add(ret, "char", json_object_new_int(state->active_char));

	return ret;
}

static json_object *xx_ret_class_definition(json_object *properties, json_object *methods, json_object *constants, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	if (properties) {
		json_object_object_add(ret, "properties", properties);
	}
	if (methods) {
		json_object_object_add(ret, "methods", methods);
	}
	if (constants) {
		json_object_object_add(ret, "constants", constants);
	}

	json_object_object_add(ret, "file", json_object_new_string(state->active_file));
	json_object_object_add(ret, "line", json_object_new_int(state->active_line));
	json_object_object_add(ret, "char", json_object_new_int(state->active_char));

	return ret;
}

static json_object *xx_ret_interface_definition(json_object *methods, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	if (methods) {
		json_object_object_add(ret, "methods", methods);
	}

	json_object_object_add(ret, "file", json_object_new_string(state->active_file));
	json_object_object_add(ret, "line", json_object_new_int(state->active_line));
	json_object_object_add(ret, "char", json_object_new_int(state->active_char));

	return ret;
}

static json_object *xx_ret_class_property(json_object *visibility, xx_parser_token *T,
		json_object *default_value, xx_parser_token *D, json_object *shortcuts, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "visibility", visibility);
	json_object_object_add(ret, "type", json_object_new_string("property"));
	json_object_object_add(ret, "name", json_object_new_string(T->token));

	if (default_value) {
		json_object_object_add(ret, "default", default_value);
	}

	if (D) {
		json_object_object_add(ret, "docblock", json_object_new_string(D->token));
	}

	if (shortcuts) {
		json_object_object_add(ret, "shortcuts", shortcuts);
	}

	json_object_object_add(ret, "file", json_object_new_string(state->active_file));
	json_object_object_add(ret, "line", json_object_new_int(state->active_line));
	json_object_object_add(ret, "char", json_object_new_int(state->active_char));

	return ret;
}

static json_object *xx_ret_property_shortcut(xx_parser_token *C, xx_parser_token *D, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("shortcut"));
	if (C) {
		json_object_object_add(ret, "docblock", json_object_new_string(C->token));
	}
	json_object_object_add(ret, "name", json_object_new_string(D->token));

	json_object_object_add(ret, "file", json_object_new_string(state->active_file));
	json_object_object_add(ret, "line", json_object_new_int(state->active_line));
	json_object_object_add(ret, "char", json_object_new_int(state->active_char));

	return ret;
}

static json_object *xx_ret_class_const(xx_parser_token *T, json_object *default_value, xx_parser_token *D, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("const"));
	json_object_object_add(ret, "name", json_object_new_string(T->token));
	json_object_object_add(ret, "default", default_value);

	if (D) {
		json_object_object_add(ret, "docblock", json_object_new_string(D->token));
	}

	json_object_object_add(ret, "file", json_object_new_string(state->active_file));
	json_object_object_add(ret, "line", json_object_new_int(state->active_line));
	json_object_object_add(ret, "char", json_object_new_int(state->active_char));

	return ret;
}

static json_object *xx_ret_class_method(json_object *visibility, xx_parser_token *T, json_object *parameters,
	json_object *statements, xx_parser_token *D, json_object *return_type, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "visibility", visibility);
	json_object_object_add(ret, "type", json_object_new_string("method"));
	json_object_object_add(ret, "name", json_object_new_string(T->token));

	if (parameters) {
		json_object_object_add(ret, "parameters", parameters);
	}

	if (statements) {
		json_object_object_add(ret, "statements", statements);
	}

	if (D) {
		json_object_object_add(ret, "docblock", json_object_new_string(D->token));
	}

	if (return_type) {
		json_object_object_add(ret, "return-type", return_type);
	}

	json_object_object_add(ret, "file", json_object_new_string(state->active_file));
	json_object_object_add(ret, "line", json_object_new_int(state->active_line));
	json_object_object_add(ret, "char", json_object_new_int(state->active_char));

	return ret;
}

static json_object *xx_ret_parameter(json_object *type, json_object *cast, xx_parser_token *N, json_object *default_value,
	int mandatory, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("parameter"));
	json_object_object_add(ret, "name", json_object_new_string(N->token));

	if (type) {
		json_object_object_add(ret, "data-type", type);
		json_object_object_add(ret, "mandatory", json_object_new_int(mandatory));
	}
	if (cast) {
		json_object_object_add(ret, "cast", cast);
	}
	if (default_value) {
		json_object_object_add(ret, "default", default_value);
	}

	json_object_object_add(ret, "file", json_object_new_string(state->active_file));
	json_object_object_add(ret, "line", json_object_new_int(state->active_line));
	json_object_object_add(ret, "char", json_object_new_int(state->active_char));

	return ret;
}

static json_object *xx_ret_return_type(int is_void, json_object *return_type_list, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("return-type"));

	json_object_object_add(ret, "list", return_type_list);

	json_object_object_add(ret, "void", json_object_new_int(is_void));

	json_object_object_add(ret, "file", json_object_new_string(state->active_file));
	json_object_object_add(ret, "line", json_object_new_int(state->active_line));
	json_object_object_add(ret, "char", json_object_new_int(state->active_char));

	return ret;
}

static json_object *xx_ret_return_type_item(json_object *type, json_object *cast, int mandatory, int collection, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("return-type-parameter"));

	if (type) {
		json_object_object_add(ret, "data-type", type);
		json_object_object_add(ret, "mandatory", json_object_new_int(mandatory));
	}

	if (cast) {
		json_object_object_add(ret, "cast", cast);
		json_object_object_add(ret, "collection", json_object_new_int(collection));
	}

	json_object_object_add(ret, "file", json_object_new_string(state->active_file));
	json_object_object_add(ret, "line", json_object_new_int(state->active_line));
	json_object_object_add(ret, "char", json_object_new_int(state->active_char));

	return ret;
}

static json_object *xx_ret_type(int type)
{
	switch (type) {
		case XX_TYPE_INTEGER:
			return json_object_new_string("int");
		case XX_TYPE_UINTEGER:
			return json_object_new_string("uint");
		case XX_TYPE_DOUBLE:
			return json_object_new_string("double");
		case XX_TYPE_BOOL:
			return json_object_new_string("bool");
		case XX_TYPE_STRING:
			return json_object_new_string("string");
		case XX_TYPE_CHAR:
			return json_object_new_string("char");
		case XX_TYPE_ARRAY:
			return json_object_new_string("array");
		case XX_TYPE_VAR:
			return json_object_new_string("variable");
		default:
			fprintf(stderr, "err 1?\n");
	}

}

static json_object *xx_ret_list(json_object *list_left, json_object *right_list)
{
	json_object *ret;
	int i, array_length;

	ret = json_object_new_array();

	if (list_left) {

		if (json_object_get_type(list_left) == json_type_array) {
			array_length = json_object_array_length(list_left);
			for (i = 0; i < array_length; i++) {
				json_object_array_add(ret, json_object_array_get_idx(list_left, i));
			}
			//json_object_put(list_left);
		} else {
			json_object_array_add(ret, list_left);
		}

	}

	json_object_array_add(ret, right_list);

	return ret;
}

static json_object *xx_ret_let_statement(json_object *assignments, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("let"));
	json_object_object_add(ret, "assignments", assignments);

	json_object_object_add(ret, "file", json_object_new_string(state->active_file));
	json_object_object_add(ret, "line", json_object_new_int(state->active_line));
	json_object_object_add(ret, "char", json_object_new_int(state->active_char));

	return ret;
}

static json_object *xx_ret_let_assignment(char *type, json_object *operator, xx_parser_token *V, xx_parser_token *P, json_object *index_expr, json_object *expr, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "assign-type", json_object_new_string(type));
	if (operator) {
		json_object_object_add(ret, "operator", operator);
	}
	json_object_object_add(ret, "variable", json_object_new_string(V->token));
	if (P) {
		json_object_object_add(ret, "property", json_object_new_string(P->token));
	}
	if (index_expr) {
		json_object_object_add(ret, "index-expr", index_expr);
	}
	if (expr) {
		json_object_object_add(ret, "expr", expr);
	}

	json_object_object_add(ret, "file", json_object_new_string(state->active_file));
	json_object_object_add(ret, "line", json_object_new_int(state->active_line));
	json_object_object_add(ret, "char", json_object_new_int(state->active_char));

	return ret;
}

static json_object *xx_ret_if_statement(json_object *expr, json_object *statements, json_object *else_statements, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("if"));
	json_object_object_add(ret, "expr", expr);

	if (statements) {
		json_object_object_add(ret, "statements", statements);
	}

	if (else_statements) {
		json_object_object_add(ret, "else_statements", else_statements);
	}

	json_object_object_add(ret, "file", json_object_new_string(state->active_file));
	json_object_object_add(ret, "line", json_object_new_int(state->active_line));
	json_object_object_add(ret, "char", json_object_new_int(state->active_char));

	return ret;
}

static json_object *xx_ret_switch_statement(json_object *expr, json_object *clauses, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("switch"));
	json_object_object_add(ret, "expr", expr);

	if (clauses) {
		json_object_object_add(ret, "clauses", clauses);
	}

	json_object_object_add(ret, "file", json_object_new_string(state->active_file));
	json_object_object_add(ret, "line", json_object_new_int(state->active_line));
	json_object_object_add(ret, "char", json_object_new_int(state->active_char));

	return ret;
}

static json_object *xx_ret_case_clause(json_object *expr, json_object *statements, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	if (expr) {
		json_object_object_add(ret, "type", json_object_new_string("case"));
		json_object_object_add(ret, "expr", expr);
	} else {
		json_object_object_add(ret, "type", json_object_new_string("default"));
	}

	if (statements) {
		json_object_object_add(ret, "statements", statements);
	}

	json_object_object_add(ret, "file", json_object_new_string(state->active_file));
	json_object_object_add(ret, "line", json_object_new_int(state->active_line));
	json_object_object_add(ret, "char", json_object_new_int(state->active_char));

	return ret;
}

static json_object *xx_ret_while_statement(json_object *expr, json_object *statements, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("while"));
	json_object_object_add(ret, "expr", expr);

	if (statements) {
		json_object_object_add(ret, "statements", statements);
	}

	json_object_object_add(ret, "file", json_object_new_string(state->active_file));
	json_object_object_add(ret, "line", json_object_new_int(state->active_line));
	json_object_object_add(ret, "char", json_object_new_int(state->active_char));

	return ret;
}

static json_object *xx_ret_do_while_statement(json_object *expr, json_object *statements, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("do-while"));
	json_object_object_add(ret, "expr", expr);

	if (statements) {
		json_object_object_add(ret, "statements", statements);
	}

	json_object_object_add(ret, "file", json_object_new_string(state->active_file));
	json_object_object_add(ret, "line", json_object_new_int(state->active_line));
	json_object_object_add(ret, "char", json_object_new_int(state->active_char));

	return ret;
}

static json_object *xx_ret_for_statement(json_object *expr, xx_parser_token *K, xx_parser_token *V, int reverse, json_object *statements, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("for"));
	json_object_object_add(ret, "expr", expr);

	if (K) {
		json_object_object_add(ret, "key", json_object_new_string(K->token));
	}
	if (V) {
		json_object_object_add(ret, "value", json_object_new_string(V->token));
	}

	json_object_object_add(ret, "reverse", json_object_new_int(reverse));

	if (statements) {
		json_object_object_add(ret, "statements", statements);
	}

	json_object_object_add(ret, "file", json_object_new_string(state->active_file));
	json_object_object_add(ret, "line", json_object_new_int(state->active_line));
	json_object_object_add(ret, "char", json_object_new_int(state->active_char));

	return ret;
}

static json_object *xx_ret_loop_statement(json_object *statements, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("loop"));

	if (statements) {
		json_object_object_add(ret, "statements", statements);
	}

	json_object_object_add(ret, "file", json_object_new_string(state->active_file));
	json_object_object_add(ret, "line", json_object_new_int(state->active_line));
	json_object_object_add(ret, "char", json_object_new_int(state->active_char));

	return ret;
}

static json_object *xx_ret_empty_statement(xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("empty"));

	json_object_object_add(ret, "file", json_object_new_string(state->active_file));
	json_object_object_add(ret, "line", json_object_new_int(state->active_line));
	json_object_object_add(ret, "char", json_object_new_int(state->active_char));

	return ret;
}

static json_object *xx_ret_break_statement(xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("break"));

	json_object_object_add(ret, "file", json_object_new_string(state->active_file));
	json_object_object_add(ret, "line", json_object_new_int(state->active_line));
	json_object_object_add(ret, "char", json_object_new_int(state->active_char));

	return ret;
}

static json_object *xx_ret_continue_statement(xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("continue"));

	json_object_object_add(ret, "file", json_object_new_string(state->active_file));
	json_object_object_add(ret, "line", json_object_new_int(state->active_line));
	json_object_object_add(ret, "char", json_object_new_int(state->active_char));

	return ret;
}

static json_object *xx_ret_echo_statement(json_object *expressions, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("echo"));
	json_object_object_add(ret, "expressions", expressions);

	json_object_object_add(ret, "file", json_object_new_string(state->active_file));
	json_object_object_add(ret, "line", json_object_new_int(state->active_line));
	json_object_object_add(ret, "char", json_object_new_int(state->active_char));

	return ret;
}

static json_object *xx_ret_return_statement(json_object *expr, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("return"));
	if (expr) {
		json_object_object_add(ret, "expr", expr);
	}

	json_object_object_add(ret, "file", json_object_new_string(state->active_file));
	json_object_object_add(ret, "line", json_object_new_int(state->active_line));
	json_object_object_add(ret, "char", json_object_new_int(state->active_char));

	return ret;
}

static json_object *xx_ret_fetch_statement(json_object *expr, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("fetch"));
	json_object_object_add(ret, "expr", expr);

	json_object_object_add(ret, "file", json_object_new_string(state->active_file));
	json_object_object_add(ret, "line", json_object_new_int(state->active_line));
	json_object_object_add(ret, "char", json_object_new_int(state->active_char));

	return ret;
}

static json_object *xx_ret_fcall_statement(json_object *expr, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("fcall"));
	json_object_object_add(ret, "expr", expr);

	json_object_object_add(ret, "file", json_object_new_string(state->active_file));
	json_object_object_add(ret, "line", json_object_new_int(state->active_line));
	json_object_object_add(ret, "char", json_object_new_int(state->active_char));

	return ret;
}

static json_object *xx_ret_mcall_statement(json_object *expr, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("mcall"));
	json_object_object_add(ret, "expr", expr);

	json_object_object_add(ret, "file", json_object_new_string(state->active_file));
	json_object_object_add(ret, "line", json_object_new_int(state->active_line));
	json_object_object_add(ret, "char", json_object_new_int(state->active_char));

	return ret;
}

static json_object *xx_ret_scall_statement(json_object *expr, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("scall"));
	json_object_object_add(ret, "expr", expr);

	json_object_object_add(ret, "file", json_object_new_string(state->active_file));
	json_object_object_add(ret, "line", json_object_new_int(state->active_line));
	json_object_object_add(ret, "char", json_object_new_int(state->active_char));

	return ret;
}

static json_object *xx_ret_unset_statement(json_object *domain, json_object *index, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("unset"));

	json_object_object_add(ret, "domain", domain);
	json_object_object_add(ret, "index", index);

	json_object_object_add(ret, "file", json_object_new_string(state->active_file));
	json_object_object_add(ret, "line", json_object_new_int(state->active_line));
	json_object_object_add(ret, "char", json_object_new_int(state->active_char));

	return ret;
}

static json_object *xx_ret_declare_statement(int type, json_object *variables, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("declare"));

	switch (type) {
		case XX_T_TYPE_INTEGER:
			json_object_object_add(ret, "data-type", json_object_new_string("int"));
			break;
		case XX_T_TYPE_UINTEGER:
			json_object_object_add(ret, "data-type", json_object_new_string("uint"));
			break;
		case XX_T_TYPE_LONG:
			json_object_object_add(ret, "data-type", json_object_new_string("long"));
			break;
		case XX_T_TYPE_ULONG:
			json_object_object_add(ret, "data-type", json_object_new_string("ulong"));
			break;
		case XX_T_TYPE_CHAR:
			json_object_object_add(ret, "data-type", json_object_new_string("char"));
			break;
		case XX_T_TYPE_UCHAR:
			json_object_object_add(ret, "data-type", json_object_new_string("uchar"));
			break;
		case XX_T_TYPE_DOUBLE:
			json_object_object_add(ret, "data-type", json_object_new_string("double"));
			break;
		case XX_T_TYPE_BOOL:
			json_object_object_add(ret, "data-type", json_object_new_string("bool"));
			break;
		case XX_T_TYPE_STRING:
			json_object_object_add(ret, "data-type", json_object_new_string("string"));
			break;
		/*case XX_T_TYPE_ARRAY:
			json_object_object_add(ret, "data-type", json_object_new_string("array"));
			break;*/
		case XX_T_TYPE_VAR:
			json_object_object_add(ret, "data-type", json_object_new_string("variable"));
			break;
		default:
			fprintf(stderr, "err 2?\n");
	}

	json_object_object_add(ret, "variables", variables);

	json_object_object_add(ret, "file", json_object_new_string(state->active_file));
	json_object_object_add(ret, "line", json_object_new_int(state->active_line));
	json_object_object_add(ret, "char", json_object_new_int(state->active_char));

	return ret;
}

static json_object *xx_ret_declare_variable(xx_parser_token *T, json_object *expr, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "variable", json_object_new_string(T->token));
	if (expr) {
		json_object_object_add(ret, "expr", expr);
	}

	json_object_object_add(ret, "file", json_object_new_string(state->active_file));
	json_object_object_add(ret, "line", json_object_new_int(state->active_line));
	json_object_object_add(ret, "char", json_object_new_int(state->active_char));

	return ret;
}

static json_object *xx_ret_new_instance(int dynamic, xx_parser_token *T, json_object *parameters, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("new"));
	json_object_object_add(ret, "class", json_object_new_string(T->token));
	json_object_object_add(ret, "dynamic", json_object_new_int(dynamic));

	if (parameters) {
		json_object_object_add(ret, "parameters", parameters);
	}

	json_object_object_add(ret, "file", json_object_new_string(state->active_file));
	json_object_object_add(ret, "line", json_object_new_int(state->active_line));
	json_object_object_add(ret, "char", json_object_new_int(state->active_char));

	return ret;
}

static json_object *xx_ret_throw_exception(json_object *expr, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("throw"));
	if (expr) {
		json_object_object_add(ret, "expr", expr);
	}

	json_object_object_add(ret, "file", json_object_new_string(state->active_file));
	json_object_object_add(ret, "line", json_object_new_int(state->active_line));
	json_object_object_add(ret, "char", json_object_new_int(state->active_char));

	return ret;
}

static json_object *xx_ret_fcall(int type, xx_parser_token *F, json_object *parameters, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("fcall"));
	json_object_object_add(ret, "name", json_object_new_string(F->token));
	json_object_object_add(ret, "call-type", json_object_new_int(type));

	if (parameters) {
		json_object_object_add(ret, "parameters", parameters);
	}

	json_object_object_add(ret, "file", json_object_new_string(state->active_file));
	json_object_object_add(ret, "line", json_object_new_int(state->active_line));
	json_object_object_add(ret, "char", json_object_new_int(state->active_char));

	return ret;
}

static json_object *xx_ret_mcall(int type, xx_parser_token *O, xx_parser_token *M, json_object *parameters, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("mcall"));
	json_object_object_add(ret, "variable", json_object_new_string(O->token));
	json_object_object_add(ret, "name", json_object_new_string(M->token));
	json_object_object_add(ret, "call-type", json_object_new_int(type));

	if (parameters) {
		json_object_object_add(ret, "parameters", parameters);
	}

	json_object_object_add(ret, "file", json_object_new_string(state->active_file));
	json_object_object_add(ret, "line", json_object_new_int(state->active_line));
	json_object_object_add(ret, "char", json_object_new_int(state->active_char));

	return ret;
}

static json_object *xx_ret_scall(int dynamic_class, xx_parser_token *O, xx_parser_token *M, json_object *parameters, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("scall"));
	json_object_object_add(ret, "dynamic-class", json_object_new_int(dynamic_class));
	json_object_object_add(ret, "class", json_object_new_string(O->token));
	json_object_object_add(ret, "name", json_object_new_string(M->token));

	if (parameters) {
		json_object_object_add(ret, "parameters", parameters);
	}

	json_object_object_add(ret, "file", json_object_new_string(state->active_file));
	json_object_object_add(ret, "line", json_object_new_int(state->active_line));
	json_object_object_add(ret, "char", json_object_new_int(state->active_char));

	return ret;
}

static json_object *xx_ret_comment(xx_parser_token *T, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("comment"));
	json_object_object_add(ret, "value", json_object_new_string(T->token));

	json_object_object_add(ret, "file", json_object_new_string(state->active_file));
	json_object_object_add(ret, "line", json_object_new_int(state->active_line));
	json_object_object_add(ret, "char", json_object_new_int(state->active_char));

	return ret;
}


// 923 "parser.c"
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
**    xx_TOKENTYPE     is the data type used for minor tokens given 
**                       directly to the parser from the tokenizer.
**    YYMINORTYPE        is the data type used for all minor tokens.
**                       This is typically a union of many types, one of
**                       which is xx_TOKENTYPE.  The entry in the union
**                       for base tokens is called "yy0".
**    YYSTACKDEPTH       is the maximum depth of the parser's stack.
**    xx_ARG_SDECL     A static variable declaration for the %extra_argument
**    xx_ARG_PDECL     A parameter declaration for the %extra_argument
**    xx_ARG_STORE     Code to store %extra_argument into yypParser
**    xx_ARG_FETCH     Code to extract %extra_argument from yypParser
**    YYNSTATE           the combined number of states.
**    YYNRULE            the number of rules in the grammar
**    YYERRORSYMBOL      is the code number of the error symbol.  If not
**                       defined, then do no error processing.
*/
#define YYCODETYPE unsigned char
#define YYNOCODE 188
#define YYACTIONTYPE unsigned short int
#define xx_TOKENTYPE xx_parser_token*
typedef union {
  xx_TOKENTYPE yy0;
  json_object* yy115;
  int yy375;
} YYMINORTYPE;
#define YYSTACKDEPTH 100
#define xx_ARG_SDECL xx_parser_status *status;
#define xx_ARG_PDECL ,xx_parser_status *status
#define xx_ARG_FETCH xx_parser_status *status = yypParser->status
#define xx_ARG_STORE yypParser->status = status
#define YYNSTATE 694
#define YYNRULE 327
#define YYERRORSYMBOL 104
#define YYERRSYMDT yy375
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
 /*     0 */   215,  224,  227,  218,  221,  238,  240,  248,  242,  244,
 /*    10 */   246,  684,  262,  672,  335,  267,  440,  235,  174,    3,
 /*    20 */     4,    5,    6,    7,  211,  271,  276,  292,  208,  395,
 /*    30 */   286,  270,  266,  116,  404,   31,   55,   34,   36,   37,
 /*    40 */   299,  306,  308,  307,  275,  285,  694,  256,  489,  303,
 /*    50 */   309,  289,   44,   45,   46,   47,   48,   49,   50,   51,
 /*    60 */    52,   53,   54,  489,  396,  397,  398,  399,  400,  298,
 /*    70 */   428,  452,  453,  271,  403,    9,   79,   87,  423,   64,
 /*    80 */   522,  167,  488,  279,  306,  308,  307,  275,  299,  200,
 /*    90 */   532,  278,  280,  281,  282,  283,  284,  497,  104,  321,
 /*   100 */   486,  495,  504,  507,  498,  501,  510,  516,  513,  119,
 /*   110 */   519,  168,  170,  172,  173,  181,  271,  381,  311,  192,
 /*   120 */   196,  201,  382,  522,  167,  389,  316,  306,  308,  307,
 /*   130 */   275,  299,  556,  532,  526,  455,  470,  474,  483,  408,
 /*   140 */   487,  494,  527,  486,  495,  504,  507,  498,  501,  510,
 /*   150 */   516,  513,  395,  519,  168,  170,  172,   10,  181,   17,
 /*   160 */   653,  416,  192,  196,  201,  382,  436,  639,  389,  271,
 /*   170 */   654,  306,  308,  307,  275,  298,  522,  167,  455,  470,
 /*   180 */   474,  483,   12,  645,  299,  176,  532,  396,  397,  398,
 /*   190 */   399,  400,  640,  646,  417,  418,  486,  495,  504,  507,
 /*   200 */   498,  501,  510,  516,  513,  314,  519,  168,  170,  172,
 /*   210 */   401,  181,  271,  446,  451,  192,  196,  201,  382,  522,
 /*   220 */   167,  389,  496,  494,  674,  416,  672,  299,  561,  532,
 /*   230 */   444,  455,  470,  474,  483,  306,  308,  307,  275,  486,
 /*   240 */   495,  504,  507,  498,  501,  510,  516,  513,   13,  519,
 /*   250 */   168,  170,  172,  663,  181,  666,  667,  651,  192,  196,
 /*   260 */   201,  382,  616,  617,  389,  271,  499,  494,  119,  335,
 /*   270 */   525,  298,  522,  167,  455,  470,  474,  483,  502,  494,
 /*   280 */   299,  562,  532, 1022,    1,    2,  693,    4,    5,    6,
 /*   290 */     7,   15,  486,  495,  504,  507,  498,  501,  510,  516,
 /*   300 */   513,  304,  519,  168,  170,  172,  523,  181,  271,  460,
 /*   310 */   669,  192,  196,  201,  382,  522,  167,  389,  459,  306,
 /*   320 */   308,  307,  275,  299,  611,  532,   16,  455,  470,  474,
 /*   330 */   483,  432,  489,  505,  494,  486,  495,  504,  507,  498,
 /*   340 */   501,  510,  516,  513,  395,  519,  168,  170,  172,  645,
 /*   350 */   181,  130,  287,   14,  192,  196,  201,  382,   17,  646,
 /*   360 */   389,  271,  306,  308,  307,  275,  500,  298,  522,  167,
 /*   370 */   455,  470,  474,  483,  348,  344,  299,  613,  532,  396,
 /*   380 */   397,  398,  399,  400,   33,   34,   36,   37,  486,  495,
 /*   390 */   504,  507,  498,  501,  510,  516,  513,  359,  519,  168,
 /*   400 */   170,  172,  206,  181,  271,  381,  489,  192,  196,  201,
 /*   410 */   382,  522,  167,  389,  293,  306,  308,  307,  275,  299,
 /*   420 */   204,  532,  324,  455,  470,  474,  483,  343,  489,  508,
 /*   430 */   494,  486,  495,  504,  507,  498,  501,  510,  516,  513,
 /*   440 */   503,  519,  168,  170,  172,  182,  181,  656,  381,   94,
 /*   450 */   192,  196,  201,  382,  639,   24,  389,  271,  306,  308,
 /*   460 */   307,  275,  506,  298,  522,  167,  455,  470,  474,  483,
 /*   470 */   460,   25,  299,  583,  532,  528,  489,   29,  456,  461,
 /*   480 */   306,  308,  307,  275,  486,  495,  504,  507,  498,  501,
 /*   490 */   510,  516,  513,  297,  519,  168,  170,  172,  553,  181,
 /*   500 */   271,  381,   32,  192,  196,  201,  382,  522,  167,  389,
 /*   510 */   509,  306,  308,  307,  275,  299,  143,  532,   35,  455,
 /*   520 */   470,  474,  483,  450,  511,  494,  530,  486,  495,  504,
 /*   530 */   507,  498,  501,  510,  516,  513,  395,  519,  168,  170,
 /*   540 */   172,  197,  181,  576,  381,   39,  192,  196,  201,  382,
 /*   550 */   268,  580,  389,  271,  306,  308,  307,  275,  354,  592,
 /*   560 */   522,  167,  455,  470,  474,  483,  368,  596,  299,  551,
 /*   570 */   532,  396,  397,  398,  399,  400,  306,  308,  307,  275,
 /*   580 */   486,  495,  504,  507,  498,  501,  510,  516,  513,  690,
 /*   590 */   519,  168,  170,  172,  130,  181,  271,  514,  494,  192,
 /*   600 */   196,  201,  382,  522,  167,  389,  489,  517,  494,  416,
 /*   610 */    42,  299,  145,  532,  412,  455,  470,  474,  483,  306,
 /*   620 */   308,  307,  275,  486,  495,  504,  507,  498,  501,  510,
 /*   630 */   516,  513,  395,  519,  168,  170,  172,  290,  181,  615,
 /*   640 */   512,  137,  192,  196,  201,  382,  616,  617,  389,  271,
 /*   650 */   334,  489,  119,  520,  494,  604,  522,  167,  455,  470,
 /*   660 */   474,  483,  225,  608,  299,  595,  532,  396,  397,  398,
 /*   670 */   399,  400,  306,  308,  307,  275,  486,  495,  504,  507,
 /*   680 */   498,  501,  510,  516,  513,  515,  519,  168,  170,  172,
 /*   690 */    83,  181,  271,  277,   80,  192,  196,  201,  382,  522,
 /*   700 */   167,  389,   43,  306,  308,  307,  275,  299,  555,  532,
 /*   710 */   259,  455,  470,  474,  483,  390,  454,  631,   62,  486,
 /*   720 */   495,  504,  507,  498,  501,  510,  516,  513,  119,  519,
 /*   730 */   168,  170,  172,   59,  181,  636,  633,  257,  192,  196,
 /*   740 */   201,  382,  639,  639,  389,  271,  634,  306,  308,  307,
 /*   750 */   275,   63,  522,  167,  455,  470,  474,  483,   61,  587,
 /*   760 */   299,  195,  532,  569,  489,   31,   55,   34,   36,   37,
 /*   770 */   119,  118,  486,  495,  504,  507,  498,  501,  510,  516,
 /*   780 */   513,  575,  519,  168,  170,  172,  115,  181,  271,  367,
 /*   790 */   489,  192,  196,  201,  382,  522,  167,  389,  518,  306,
 /*   800 */   308,  307,  275,  299,  388,  532,  539,  455,  470,  474,
 /*   810 */   483,  214,  641,  648,   40,  486,  495,  504,  507,  498,
 /*   820 */   501,  510,  516,  513,  521,  519,  168,  170,  172,  373,
 /*   830 */   181,   41,  228,  392,  192,  196,  201,  382,  374,   66,
 /*   840 */   389,  271,  306,  308,  307,  275,  375,   63,  522,  167,
 /*   850 */   455,  470,  474,  483,  185,  560,  299,  579,  532,  110,
 /*   860 */   643,   31,   55,   34,   36,   37,   67,  391,  486,  495,
 /*   870 */   504,  507,  498,  501,  510,  516,  513,   58,  519,  168,
 /*   880 */   170,  172,  384,  181,  271,  230,  405,  192,  196,  201,
 /*   890 */   382,  522,  167,  389,  419,  306,  308,  307,  275,  299,
 /*   900 */   545,  532,  642,  455,  470,  474,  483,  128,   77,  672,
 /*   910 */    76,  486,  495,  504,  507,  498,  501,  510,  516,  513,
 /*   920 */    63,  519,  168,  170,  172,  302,  181,  376,  232,  458,
 /*   930 */   192,  196,  201,  382,  142,   81,  389,  271,  306,  308,
 /*   940 */   307,  275,  568,  298,  522,  167,  455,  470,  474,  483,
 /*   950 */   603,   84,  299,  577,  532,   60,   82,   31,   55,   34,
 /*   960 */    36,   37,  310,  457,  486,  495,  504,  507,  498,  501,
 /*   970 */   510,  516,  513,  365,  519,  168,  170,  172,  378,  181,
 /*   980 */   271,  234,   85,  192,  196,  201,  382,  522,  167,  389,
 /*   990 */    86,  306,  308,  307,  275,  299,  599,  532,   90,  455,
 /*  1000 */   470,  474,  483,  127,   88,   89,  130,  486,  495,  504,
 /*  1010 */   507,  498,  501,  510,  516,  513,  298,  519,  168,  170,
 /*  1020 */   172,   92,  181,  420,  236,   40,  192,  196,  201,  382,
 /*  1030 */   106,  107,  389,  271,  306,  308,  307,  275,  111,  298,
 /*  1040 */   522,  167,  455,  470,  474,  483,  352,  117,  299,  601,
 /*  1050 */   532,  609,  122,   31,   55,   34,   36,   37,  123,  125,
 /*  1060 */   486,  495,  504,  507,  498,  501,  510,  516,  513,  337,
 /*  1070 */   519,  168,  170,  172,  424,  181,  271,  239,  126,  192,
 /*  1080 */   196,  201,  382,  522,  167,  389,   56,  306,  308,  307,
 /*  1090 */   275,  299,  550,  532,  671,  455,  470,  474,  483,  129,
 /*  1100 */   139,   27,  133,  486,  495,  504,  507,  498,  501,  510,
 /*  1110 */   516,  513,  298,  519,  168,  170,  172,  447,  181,  355,
 /*  1120 */   265,  449,  192,  196,  201,  382,  140,  169,  389,  271,
 /*  1130 */   306,  308,  307,  275,  171,  298,  522,  167,  455,  470,
 /*  1140 */   474,  483,  332,  177,  299,  180,  532,   28,  178,   31,
 /*  1150 */    55,   34,   36,   37,  183,  187,  486,  495,  504,  507,
 /*  1160 */   498,  501,  510,  516,  513,  330,  519,  168,  170,  172,
 /*  1170 */   361,  181,  271,  241,  189,  192,  196,  201,  382,  522,
 /*  1180 */   167,  389,  112,  306,  308,  307,  275,  299,  605,  532,
 /*  1190 */   190,  455,  470,  474,  483,  479,  193,  109,  198,  486,
 /*  1200 */   495,  504,  507,  498,  501,  510,  516,  513,   63,  519,
 /*  1210 */   168,  170,  172,  476,  181,  202,  243,  205,  192,  196,
 /*  1220 */   201,  382,  207,  235,  389,  271,  306,  308,  307,  275,
 /*  1230 */   213,  212,  522,  167,  455,  470,  474,  483,  114,  260,
 /*  1240 */   299,  567,  532,  581,  263,   31,   55,   34,   36,   37,
 /*  1250 */   264,  269,  486,  495,  504,  507,  498,  501,  510,  516,
 /*  1260 */   513,  272,  519,  168,  170,  172,  273,  181,  271,  261,
 /*  1270 */   274,  192,  196,  201,  382,  522,  167,  389,  291,  306,
 /*  1280 */   308,  307,  275,  299,  593,  532,  294,  455,  470,  474,
 /*  1290 */   483,  300,  312,  301,  317,  486,  495,  504,  507,  498,
 /*  1300 */   501,  510,  516,  513,  318,  519,  168,  170,  172,  325,
 /*  1310 */   181,  319,  245,  326,  192,  196,  201,  382,  327, 1004,
 /*  1320 */   389,  271,  306,  308,  307,  275, 1003,  347,  522,  167,
 /*  1330 */   455,  470,  474,  483,  356,  357,  299,  573,  532,  597,
 /*  1340 */   362,   31,   55,   34,   36,   37,  377,  363,  486,  495,
 /*  1350 */   504,  507,  498,  501,  510,  516,  513,  379,  519,  168,
 /*  1360 */   170,  172,  383,  181,  271,  247,  394,  192,  196,  201,
 /*  1370 */   382,  522,  167,  389,  393,  306,  308,  307,  275,  299,
 /*  1380 */   585,  532,  406,  455,  470,  474,  483,  410,  413,  421,
 /*  1390 */   414,  486,  495,  504,  507,  498,  501,  510,  516,  513,
 /*  1400 */   422,  519,  168,  170,  172,  425,  181,  426,  249,  186,
 /*  1410 */   192,  196,  201,  382,  429,  430,  389,  271,  306,  308,
 /*  1420 */   307,  275,  437,  445,  522,  167,  455,  470,  474,  483,
 /*  1430 */   251,  463,  299,  571,  532,  434,  188,  465,  557,  438,
 /*  1440 */   306,  308,  307,  275,  486,  495,  504,  507,  498,  501,
 /*  1450 */   510,  516,  513,  467,  519,  168,  170,  172,  442,  181,
 /*  1460 */   271,  253,  469,  192,  196,  201,  382,  522,  167,  389,
 /*  1470 */   475,  306,  308,  307,  275,  299,  566,  532,  477,  455,
 /*  1480 */   470,  474,  483,  478,  482,  491,  490,  486,  495,  504,
 /*  1490 */   507,  498,  501,  510,  516,  513,  493,  519,  168,  170,
 /*  1500 */   172,  492,  181,  524,  255,  529,  192,  196,  201,  382,
 /*  1510 */   531,  540,  389,  271,  306,  308,  307,  275,  541,  552,
 /*  1520 */   522,  167,  455,  470,  474,  483,  547,  554,  299,  607,
 /*  1530 */   532,  558,  335,  267,  563,  564,  306,  308,  307,  275,
 /*  1540 */   486,  495,  504,  507,  498,  501,  510,  516,  513,  570,
 /*  1550 */   519,  168,  170,  172, 1002,  181,  271,  582,  589,  192,
 /*  1560 */   196,  201,  382,  522,  167,  389,  664,  629,  651,  289,
 /*  1570 */   184,  299,  538,  532,  617,  455,  470,  474,  483,  119,
 /*  1580 */   590,  598,  610,  486,  495,  504,  507,  498,  501,  510,
 /*  1590 */   516,  513,  620,  519,  168,  170,  172,  188,  181,  557,
 /*  1600 */    22,  621,  192,  196,  201,  382,   57,   21,  389,  271,
 /*  1610 */   622,  119,   79,   87,  623,   93,  522,  167,  455,  470,
 /*  1620 */   474,  483,    8,  625,  299,   11,  532,  121,  626,  676,
 /*  1630 */   686,  627,  628,  532,  649,  635,  486,  495,  504,  507,
 /*  1640 */   498,  501,  510,  516,  513,  637,  519,  168,  170,  172,
 /*  1650 */   638,  181,  740,  741,  644,  192,  196,  201,  382,  165,
 /*  1660 */   647,  389,  680,  650,  683,  287,  659,  130,  655,   38,
 /*  1670 */   657,  455,  470,  474,  483,  306,  308,  307,  275,  179,
 /*  1680 */   658,  660,  342,  670,  675,  350,  346,  349,  533,  147,
 /*  1690 */   148,  149,  150,  151,  152,  153,  154,  155,  156,  157,
 /*  1700 */   158,  159,  160,  161,  162,  163,  164,  166,  165,   44,
 /*  1710 */    45,   46,   47,   48,   49,   50,   51,   52,   53,   54,
 /*  1720 */   695,  462,  464,  466,  468,  677,  678,  681,  549,  679,
 /*  1730 */    95,   96,   98,   97,   99,  687,  682,  533,  147,  148,
 /*  1740 */   149,  150,  151,  152,  153,  154,  155,  156,  157,  158,
 /*  1750 */   159,  160,  161,  162,  163,  164,  166,  165,    8,  130,
 /*  1760 */   685,   11,  688,  121,  516,  676,  686,  689,  632,  532,
 /*  1770 */   462,  464,  466,  468,  101,  102,  124,  584,  673,  691,
 /*  1780 */   692,  130,  588,  516,  516,  100,  533,  147,  148,  149,
 /*  1790 */   150,  151,  152,  153,  154,  155,  156,  157,  158,  159,
 /*  1800 */   160,  161,  162,  163,  164,  166,  165,  113,  516,  614,
 /*  1810 */   135,  629,  651,   79,   87,  516,   93,  661,  617,  462,
 /*  1820 */   464,  466,  468,  119,  516,  516,  578,  516,   95,   96,
 /*  1830 */    98,   97,   99,  516,  516,  533,  147,  148,  149,  150,
 /*  1840 */   151,  152,  153,  154,  155,  156,  157,  158,  159,  160,
 /*  1850 */   161,  162,  163,  164,  166,  165,  602,  516,  242,  244,
 /*  1860 */   246,  516,   79,   87,  516,   93,  652,  235,  462,  464,
 /*  1870 */   466,  468,  101,  102,  516,  559,  516,  516,  516,  516,
 /*  1880 */   138,  516,  516,  100,  533,  147,  148,  149,  150,  151,
 /*  1890 */   152,  153,  154,  155,  156,  157,  158,  159,  160,  161,
 /*  1900 */   162,  163,  164,  166,  165,  574,  516,  516,  516,  516,
 /*  1910 */   137,   79,   87,  516,   93,  516,  136,  462,  464,  466,
 /*  1920 */   468,  119,  516,  516,  565,  516,   95,   96,   98,   97,
 /*  1930 */    99,  516,  516,  533,  147,  148,  149,  150,  151,  152,
 /*  1940 */   153,  154,  155,  156,  157,  158,  159,  160,  161,  162,
 /*  1950 */   163,  164,  166,  165,  516,  516,  516,  516,  516,  516,
 /*  1960 */   516,  516,  516,  516,  516,  516,  462,  464,  466,  468,
 /*  1970 */   101,  102,  516,  572,  516,  516,  619,  516,  516,  516,
 /*  1980 */   516,  100,  533,  147,  148,  149,  150,  151,  152,  153,
 /*  1990 */   154,  155,  156,  157,  158,  159,  160,  161,  162,  163,
 /*  2000 */   164,  166,  165,  516,  516,  516,  516,   19,  516,  516,
 /*  2010 */   516,  516,   22,  516,  516,  462,  464,  466,  468,  120,
 /*  2020 */   516,  516,  144,  119,   95,   96,   98,   97,   99,  516,
 /*  2030 */   516,  533,  147,  148,  149,  150,  151,  152,  153,  154,
 /*  2040 */   155,  156,  157,  158,  159,  160,  161,  162,  163,  164,
 /*  2050 */   166,  165,  516,  516,  516,  516,  516,  516,  516,  516,
 /*  2060 */   516,  516,  542,  516,  462,  464,  466,  468,  101,  102,
 /*  2070 */   516,  191,  306,  308,  307,  275,  138,  516,  516,  100,
 /*  2080 */   533,  147,  148,  149,  150,  151,  152,  153,  154,  155,
 /*  2090 */   156,  157,  158,  159,  160,  161,  162,  163,  164,  166,
 /*  2100 */   165,  516,  516,  516,  516,  132,  516,  516,  134,  662,
 /*  2110 */   668,  667,  651,  462,  464,  466,  468,  661,  617,  516,
 /*  2120 */   175,  516,  516,  119,  516,  516,  516,  516,  516,  533,
 /*  2130 */   147,  148,  149,  150,  151,  152,  153,  154,  155,  156,
 /*  2140 */   157,  158,  159,  160,  161,  162,  163,  164,  166,  165,
 /*  2150 */   516,  516,  516,  516,  516,  516,  516,  516,  516,  516,
 /*  2160 */   516,  516,  462,  464,  466,  468,  516,  516,  516,  194,
 /*  2170 */   516,   95,   96,   98,   97,   99,  516,  516,  533,  147,
 /*  2180 */   148,  149,  150,  151,  152,  153,  154,  155,  156,  157,
 /*  2190 */   158,  159,  160,  161,  162,  163,  164,  166,  165,  516,
 /*  2200 */   516,  516,  516,  516,  516,  516,  516,  516,  516,  222,
 /*  2210 */   516,  462,  464,  466,  468,  101,  102,  516,  387,  306,
 /*  2220 */   308,  307,  275,  105,  516,  516,  100,  533,  147,  148,
 /*  2230 */   149,  150,  151,  152,  153,  154,  155,  156,  157,  158,
 /*  2240 */   159,  160,  161,  162,  163,  164,  166,  165,  516,  516,
 /*  2250 */   516,  516,  516,  516,  516,  516,  516,  516,  516,  516,
 /*  2260 */   462,  464,  466,  468,  516,  516,  516,  199,  516,   95,
 /*  2270 */    96,   98,   97,   99,  516,  516,  533,  147,  148,  149,
 /*  2280 */   150,  151,  152,  153,  154,  155,  156,  157,  158,  159,
 /*  2290 */   160,  161,  162,  163,  164,  166,  165,  516,  516,  516,
 /*  2300 */   516,  516,  516,  516,  516,  516,  516,  535,  516,  462,
 /*  2310 */   464,  466,  468,  101,  102,  516,  203,  306,  308,  307,
 /*  2320 */   275,  588,  516,  516,  100,  533,  147,  148,  149,  150,
 /*  2330 */   151,  152,  153,  154,  155,  156,  157,  158,  159,  160,
 /*  2340 */   161,  162,  163,  164,  166,  165,  516,  516,  516,  516,
 /*  2350 */   516,  516,  516,  516,  516,  516,  516,  516,  462,  464,
 /*  2360 */   466,  468,  516,  516,  516,  612,  516,   95,   96,   98,
 /*  2370 */    97,   99,  516,  516,  533,  147,  148,  149,  150,  151,
 /*  2380 */   152,  153,  154,  155,  156,  157,  158,  159,  160,  161,
 /*  2390 */   162,  163,  164,  166,  165,  516,  516,  516,  516,  516,
 /*  2400 */   516,  516,  516,  516,  516,  484,  516,  462,  464,  466,
 /*  2410 */   468,  101,  102,  516,  537,  306,  308,  307,  275,   23,
 /*  2420 */   516,  516,  100,  533,  147,  148,  149,  150,  151,  152,
 /*  2430 */   153,  154,  155,  156,  157,  158,  159,  160,  161,  162,
 /*  2440 */   163,  164,  166,  165,  516,  516,  516,  516,  516,  516,
 /*  2450 */   516,  516,  516,  516,  516,  516,  462,  464,  466,  468,
 /*  2460 */   516,  516,  516,  606,  516,   95,   96,   98,   97,   99,
 /*  2470 */   516,  516,  533,  147,  148,  149,  150,  151,  152,  153,
 /*  2480 */   154,  155,  156,  157,  158,  159,  160,  161,  162,  163,
 /*  2490 */   164,  166,  165,  516,  516,  516,  516,  516,  516,  516,
 /*  2500 */   516,  516,  516,  480,  516,  462,  464,  466,  468,  101,
 /*  2510 */   102,  516,  600,  306,  308,  307,  275,  516,  516,  516,
 /*  2520 */   100,  533,  147,  148,  149,  150,  151,  152,  153,  154,
 /*  2530 */   155,  156,  157,  158,  159,  160,  161,  162,  163,  164,
 /*  2540 */   166,  165,  516,  516,  516,  516,  516,  516,  516,  516,
 /*  2550 */   516,  516,  471,  516,  462,  464,  466,  468,  516,  516,
 /*  2560 */   516,  544,  306,  308,  307,  275,  516,  516,  516,  516,
 /*  2570 */   533,  147,  148,  149,  150,  151,  152,  153,  154,  155,
 /*  2580 */   156,  157,  158,  159,  160,  161,  162,  163,  164,  166,
 /*  2590 */   165,  516,  516,  516,  516,  516,  516,  516,  516,  516,
 /*  2600 */   516,  516,  516,  462,  464,  466,  468,  516,  516,  516,
 /*  2610 */   594,  516,   95,   96,   98,   97,   99,  516,  516,  533,
 /*  2620 */   147,  148,  149,  150,  151,  152,  153,  154,  155,  156,
 /*  2630 */   157,  158,  159,  160,  161,  162,  163,  164,  166,  165,
 /*  2640 */   516,  516,  516,  516,  516,  516,  516,  516,  516,  516,
 /*  2650 */   516,  516,  462,  464,  466,  468,  101,  102,  516,  722,
 /*  2660 */   630,  516,  624,  516,  516,  516,  516,  100,  146,  147,
 /*  2670 */   148,  149,  150,  151,  152,  153,  154,  155,  156,  157,
 /*  2680 */   158,  159,  160,  161,  162,  163,  164,  166,  215,  224,
 /*  2690 */   227,  218,  221,  516,  516,  516,  516,  516,  270,  516,
 /*  2700 */   262,  462,  464,  466,  468,  516,  516,  516,  306,  308,
 /*  2710 */   307,  275,  211,  271,  276,  292,  208,  323,  286,  287,
 /*  2720 */   266,  516,  215,  224,  227,  218,  221,  516,  299,  306,
 /*  2730 */   308,  307,  275,  285,  262,  256,  516,  516,  516,  345,
 /*  2740 */   346,  349,  516,  516,  516,  385,  211,  271,  276,  292,
 /*  2750 */   208,  305,  286,  516,  266,  306,  308,  307,  275,  516,
 /*  2760 */   516,  516,  299,  516,  534,  516,  516,  285,  516,  256,
 /*  2770 */   516,  279,  516,  516,  516,   75,  516,  516,  516,  278,
 /*  2780 */   280,  281,  282,  283,  284,  250,  252,  516,   78,  210,
 /*  2790 */   220,  226,  229,  231,  233,  223,  217,  238,  240,  248,
 /*  2800 */   242,  244,  246,  516,  516,  279,  516,  516,  516,  235,
 /*  2810 */   516,  516,  516,  278,  280,  281,  282,  283,  284,  209,
 /*  2820 */   516,  215,  224,  227,  218,  221,   70,  516,  516,  306,
 /*  2830 */   308,  307,  275,  262,   68,   69,   71,   72,   74,   73,
 /*  2840 */   516,  516,  516,  516,  516,  211,  271,  276,  292,  208,
 /*  2850 */   366,  286,  270,  266,  516,  215,  224,  227,  218,  221,
 /*  2860 */   516,  299,  306,  308,  307,  275,  285,  262,  256,  516,
 /*  2870 */   313,  309,  516,  516,  516,  516,  516,  516,  270,  211,
 /*  2880 */   271,  276,  292,  208,  516,  286,  516,  266,  306,  308,
 /*  2890 */   307,  275,  516,  516,  516,  299,  320,  309,  380,  516,
 /*  2900 */   285,  516,  256,  516,  279,  516,  433,  516,  306,  308,
 /*  2910 */   307,  275,  278,  280,  281,  282,  283,  284,  516,  254,
 /*  2920 */   250,  252,  516,  516,  210,  220,  226,  229,  231,  233,
 /*  2930 */   223,  217,  238,  240,  248,  242,  244,  246,  279,  516,
 /*  2940 */   516,  516,  516,  516,  235,  516,  278,  280,  281,  282,
 /*  2950 */   283,  284,  516,  516,  215,  224,  227,  218,  221,  403,
 /*  2960 */   516,  516,  516,  435,  403,  481,  262,  516,  443,  306,
 /*  2970 */   308,  307,  275,  516,  306,  308,  307,  275,  211,  271,
 /*  2980 */   276,  292,  208,  295,  286,  516,  266,  516,  215,  224,
 /*  2990 */   227,  218,  221,  516,  299,  371,  516,  516,  516,  285,
 /*  3000 */   262,  256,  516,  516,  516,  306,  308,  307,  275,  516,
 /*  3010 */   516,  270,  211,  271,  276,  292,  208,  516,  286,  516,
 /*  3020 */   288,  306,  308,  307,  275,  516,  516,  516,  299,  329,
 /*  3030 */   309,  216,  516,  285,  516,  256,  516,  279,  516,  341,
 /*  3040 */   516,  306,  308,  307,  275,  278,  280,  281,  282,  283,
 /*  3050 */   284,  516,  254,  250,  252,  516,  516,  210,  220,  226,
 /*  3060 */   229,  231,  233,  223,  217,  238,  240,  248,  242,  244,
 /*  3070 */   246,  340,  516,  516,  516,  516,  258,  235,  516,  339,
 /*  3080 */   280,  281,  282,  283,  284,  270,  516,  215,  224,  227,
 /*  3090 */   218,  221,  516,  516,  516,  306,  308,  307,  275,  262,
 /*  3100 */   516,  516,  516,  336,  309,  516,  516,  516,  516,  516,
 /*  3110 */   516,  211,  271,  276,  292,  208,  353,  286,  516,  266,
 /*  3120 */   516,  215,  224,  227,  218,  221,  516,  299,  370,  516,
 /*  3130 */   516,  516,  285,  262,  256,  516,  516,  516,  306,  308,
 /*  3140 */   307,  275,  516,  516,  270,  211,  271,  276,  292,  208,
 /*  3150 */   516,  286,  516,  266,  306,  308,  307,  275,  516,  516,
 /*  3160 */   516,  299,  364,  309,  219,  516,  285,  516,  256,  516,
 /*  3170 */   279,  516,  409,  516,  306,  308,  307,  275,  278,  280,
 /*  3180 */   281,  282,  283,  284,  516,  254,  250,  252,  516,  516,
 /*  3190 */   210,  220,  226,  229,  231,  233,  223,  217,  238,  240,
 /*  3200 */   248,  242,  244,  246,  279,  516,  516,  516,  516,  516,
 /*  3210 */   235,  516,  278,  280,  281,  282,  283,  284,  516,  516,
 /*  3220 */   215,  224,  227,  218,  221,  403,  516,  516,  516,  407,
 /*  3230 */   403,  372,  262,  516,  415,  306,  308,  307,  275,  516,
 /*  3240 */   306,  308,  307,  275,  211,  271,  276,  292,  208,  315,
 /*  3250 */   286,  516,  266,  516,  215,  224,  227,  218,  221,  516,
 /*  3260 */   299,  369,  516,  516,  516,  285,  262,  256,  516,  516,
 /*  3270 */   516,  306,  308,  307,  275,  516,  516,  270,  211,  271,
 /*  3280 */   276,  292,  208,  338,  286,  516,  266,  306,  308,  307,
 /*  3290 */   275,  516,  516,  516,  299,  358,  309,  516,  516,  285,
 /*  3300 */   516,  256,  516,  279,  516,  516,  516,  516,  516,  516,
 /*  3310 */   516,  278,  280,  281,  282,  283,  284,  516,  516,  210,
 /*  3320 */   220,  226,  229,  231,  233,  223,  217,  238,  240,  248,
 /*  3330 */   242,  244,  246,  516,  516,  516,  516,  279,  516,  235,
 /*  3340 */   516,  516,  516,  516,  516,  278,  280,  281,  282,  283,
 /*  3350 */   284,  270,  516,  215,  224,  227,  218,  221,  516,  516,
 /*  3360 */   516,  306,  308,  307,  275,  262,  516,  516,  516,  296,
 /*  3370 */   309,  516,  516,  516,  516,  516,  516,  211,  271,  276,
 /*  3380 */   292,  208,  322,  286,  516,  266,  516,  215,  224,  227,
 /*  3390 */   218,  221,  516,  299,  516,  516,  516,  516,  285,  262,
 /*  3400 */   256,  516,  516,  516,  516,  516,  516,  516,  516,  516,
 /*  3410 */    91,  211,  271,  276,  292,  208,  516,  286,  516,  266,
 /*  3420 */   516,   95,   96,   98,   97,   99,  516,  299,  516,  516,
 /*  3430 */    65,  516,  285,  516,  256,  516,  279,  516,  516,  516,
 /*  3440 */   516,  516,  516,  516,  278,  280,  281,  282,  283,  284,
 /*  3450 */    44,   45,   46,   47,   48,   49,   50,   51,   52,   53,
 /*  3460 */    54,  516,  516,  546,  516,  101,  102,  516,  721,  630,
 /*  3470 */   279,  624,  516,  516,  516,  516,  100,  516,  278,  280,
 /*  3480 */   281,  282,  283,  284,  270,  516,  215,  224,  227,  218,
 /*  3490 */   221,  516,  516,  516,  306,  308,  307,  275,  262,  516,
 /*  3500 */   516,  516,  331,  309,  516,  516,  516,  516,  516,  516,
 /*  3510 */   211,  271,  276,  292,  208,  328,  286,  516,  266,  516,
 /*  3520 */   215,  224,  227,  218,  221,  516,  299,  516,  516,  516,
 /*  3530 */   516,  285,  262,  256,  516,  516,  516,  516,  516,  516,
 /*  3540 */   516,  516,  516,  270,  211,  271,  276,  292,  208,  516,
 /*  3550 */   286,  516,  266,  306,  308,  307,  275,  516,  516,  516,
 /*  3560 */   299,  351,  309,  516,  516,  285,  516,  256,  516,  279,
 /*  3570 */   516,  441,  516,  516,  516,  516,  516,  278,  280,  281,
 /*  3580 */   282,  283,  284,  516,  254,  250,  252,  516,  516,  210,
 /*  3590 */   220,  226,  229,  231,  233,  223,  217,  238,  240,  248,
 /*  3600 */   242,  244,  246,  279,  516,  516,  516,  516,  516,  235,
 /*  3610 */   516,  278,  280,  281,  282,  283,  284,  516,  516,  215,
 /*  3620 */   224,  227,  218,  221,  403,  516,  516,  516,  448,  516,
 /*  3630 */   894,  262,  516,  516,  306,  308,  307,  275,  516,  516,
 /*  3640 */   516,  516,  516,  211,  271,  276,  292,  208,  516,  286,
 /*  3650 */   516,  266,  473,  215,  224,  227,  218,  221,  516,  299,
 /*  3660 */   516,  516,  516,  403,  285,  262,  256,  431,  516,  516,
 /*  3670 */   516,  516,  516,  306,  308,  307,  275,  211,  271,  276,
 /*  3680 */   292,  208,  333,  286,   91,  266,  516,  516,  516,  516,
 /*  3690 */   516,  516,  516,  299,  516,  516,  516,  516,  285,  516,
 /*  3700 */   256,   26,  279,  516,   65,  516,  516,  516,  516,  516,
 /*  3710 */   278,  280,  281,  282,  283,  284,  516,  516,  516,  516,
 /*  3720 */   516,  516,  516,  516,   44,   45,   46,   47,   48,   49,
 /*  3730 */    50,   51,   52,   53,   54,  516,  279,  516,  516,  516,
 /*  3740 */   516,  516,  516,  516,  278,  280,  281,  282,  283,  284,
 /*  3750 */   516,  516,  215,  224,  227,  218,  221,  403,  516,  516,
 /*  3760 */   516,  427,  403,  516,  262,  516,  411,  306,  308,  307,
 /*  3770 */   275,  516,  306,  308,  307,  275,  211,  271,  276,  292,
 /*  3780 */   208,  360,  286,  516,  266,  516,  215,  224,  227,  218,
 /*  3790 */   221,  516,  299,  516,  516,  516,  403,  285,  262,  256,
 /*  3800 */   402,  516,  516,  516,  516,  516,  306,  308,  307,  275,
 /*  3810 */   211,  271,  276,  292,  208,  516,  286,   91,  266,  516,
 /*  3820 */   516,  516,  516,  516,  516,  516,  299,  516,  403,  516,
 /*  3830 */   516,  285,  439,  256,  141,  279,  516,   65,  306,  308,
 /*  3840 */   307,  275,  516,  278,  280,  281,  282,  283,  284,  516,
 /*  3850 */   516,  516,  516,  516,  516,  516,  516,   44,   45,   46,
 /*  3860 */    47,   48,   49,   50,   51,   52,   53,   54,  516,  279,
 /*  3870 */   516,  516,  516,  516,  516,  516,  516,  278,  280,  281,
 /*  3880 */   282,  283,  284,  516,  516,  215,  224,  227,  218,  221,
 /*  3890 */   516,  516,  516,  516,  516,  516,  516,  262,  516,  516,
 /*  3900 */   516,  516,  516,  516,  516,  516,  516,  516,  516,  211,
 /*  3910 */   271,  276,  292,  208,  516,  286,  516,  288,  516,  516,
 /*  3920 */   516,  516,  516,  516,  516,  299,  516,  516,  516,  516,
 /*  3930 */   285,  516,  256,  516,  516,  516,  516,  516,  254,  250,
 /*  3940 */   252,  516,  516,  210,  220,  226,  229,  231,  233,  223,
 /*  3950 */   217,  238,  240,  248,  242,  244,  246,  516,  516,  516,
 /*  3960 */   516,  516,  516,  235,  516,  516,  516,  516,  340,  516,
 /*  3970 */   516,  516,  516,  386,  516,  516,  339,  280,  281,  282,
 /*  3980 */   283,  284,  516,  516,  516,  516,  254,  250,  252,  516,
 /*  3990 */   516,  210,  220,  226,  229,  231,  233,  223,  217,  238,
 /*  4000 */   240,  248,  242,  244,  246,  516,  516,  516,  516,  516,
 /*  4010 */   516,  235,  516,  516,  472,  254,  250,  252,  516,  516,
 /*  4020 */   210,  220,  226,  229,  231,  233,  223,  217,  238,  240,
 /*  4030 */   248,  242,  244,  246,  516,  516,  516,  516,  516,  516,
 /*  4040 */   235,  516,  516,  485,  254,  250,  252,  516,  516,  210,
 /*  4050 */   220,  226,  229,  231,  233,  223,  217,  238,  240,  248,
 /*  4060 */   242,  244,  246,  516,  516,  516,  516,  516,  516,  235,
 /*  4070 */   516,  516,  516,  516,  516,  254,  250,  252,  516,  536,
 /*  4080 */   210,  220,  226,  229,  231,  233,  223,  217,  238,  240,
 /*  4090 */   248,  242,  244,  246,  516,  516,  516,  516,  516,  516,
 /*  4100 */   235,  516,  516,  516,  516,  516,  254,  250,  252,  516,
 /*  4110 */   548,  210,  220,  226,  229,  231,  233,  223,  217,  238,
 /*  4120 */   240,  248,  242,  244,  246,  516,  516,  516,  516,  516,
 /*  4130 */   516,  235,  516,  516,  516,  516,  516,  254,  250,  252,
 /*  4140 */   516,  543,  210,  220,  226,  229,  231,  233,  223,  217,
 /*  4150 */   238,  240,  248,  242,  244,  246,  516,  516,  516,  254,
 /*  4160 */   250,  252,  235,  516,  210,  220,  226,  229,  231,  233,
 /*  4170 */   223,  217,  238,  240,  248,  242,  244,  246,  516,   91,
 /*  4180 */   516,  516,  516,  237,  235,  516,  516,  516,  516,  516,
 /*  4190 */    91,  516,  516,  516,  516,  516,  108,  516,  516,   65,
 /*  4200 */   516,  516,  516,   38,  516,  516,  516,  591,  516,  516,
 /*  4210 */    65,  516,   95,   96,   98,   97,   99,  516,  516,   44,
 /*  4220 */    45,   46,   47,   48,   49,   50,   51,   52,   53,   54,
 /*  4230 */    44,   45,   46,   47,   48,   49,   50,   51,   52,   53,
 /*  4240 */    54,   30,  516,   44,   45,   46,   47,   48,   49,   50,
 /*  4250 */    51,   52,   53,   54,  516,  516,  101,  102,  516,  131,
 /*  4260 */   630,  516,  624,  516,  516,  516,  516,  100,  516,   95,
 /*  4270 */    96,   98,   97,   99,  516,  516,  516,  516,  516,  516,
 /*  4280 */   516,  516,  516,  516,  516,  516,  516,  516,  516,  516,
 /*  4290 */   516,  516,  516,  516,  516,  516,  516,  516,  516,  516,
 /*  4300 */   516,  516,  516,  516,  516,  516,  516,  516,  516,  516,
 /*  4310 */   516,  516,  516,  101,  102,  516,  725,  618,  516,  624,
 /*  4320 */   516,  516,  516,  516,  100,  516,   95,   96,   98,   97,
 /*  4330 */    99,   95,   96,   98,   97,   99,  516,  516,  516,  516,
 /*  4340 */   516,  516,  516,  516,  516,  516,  516,  516,  516,  516,
 /*  4350 */   516,  516,  516,  516,  516,  516,  516,  516,  516,  516,
 /*  4360 */   516,  516,  516,  516,  516,  516,  516,  516,  516,  516,
 /*  4370 */   101,  102,  516,  727,  586,  101,  102,  516,  723,  586,
 /*  4380 */   516,  100,  516,  516,  516,  516,  100,  516,   95,   96,
 /*  4390 */    98,   97,   99,   95,   96,   98,   97,   99,  516,  516,
 /*  4400 */   516,  516,  516,  516,  516,  516,  516,  516,  516,  516,
 /*  4410 */   516,  516,  516,  516,  516,  516,  516,  516,  516,  516,
 /*  4420 */   516,  516,  516,  516,  516,  516,  516,  516,  516,  516,
 /*  4430 */   516,  516,  101,  102,  516,  724,  586,  101,  102,  516,
 /*  4440 */   726,  665,  516,  100,  516,  516,  516,  516,  100,  516,
 /*  4450 */    95,   96,   98,   97,   99,   95,   96,   98,   97,   99,
 /*  4460 */   516,  516,  516,  516,  516,  516,  516,  516,  516,  516,
 /*  4470 */   516,  516,  516,  516,  516,  516,  516,  516,  516,  516,
 /*  4480 */   516,  516,  516,  516,  516,  516,  516,  516,  516,  516,
 /*  4490 */   516,  516,  516,  516,  101,  102,  516,  728,  586,  101,
 /*  4500 */   102,  516,   18,  103,  516,  100,  516,  516,  516,  516,
 /*  4510 */   100,  516,   95,   96,   98,   97,   99,   95,   96,   98,
 /*  4520 */    97,   99,  516,  516,  516,  516,  516,  516,  516,  516,
 /*  4530 */   516,  516,  516,  516,  516,  516,  516,  516,  516,  516,
 /*  4540 */   516,  516,  516,  516,  516,  516,  516,  516,  516,  516,
 /*  4550 */   516,  516,  516,  516,  516,  516,  101,  102,  516,  729,
 /*  4560 */   586,  101,  102,  516,   20,  103,  516,  100,  516,  516,
 /*  4570 */   516,  516,  100,
};
static YYCODETYPE yy_lookahead[] = {
 /*     0 */     7,    8,    9,   10,   11,   25,   26,   27,   28,   29,
 /*    10 */    30,  115,   19,  117,   54,   55,   37,   37,   47,  108,
 /*    20 */   109,  110,  111,  112,   31,   32,   33,   34,   35,   50,
 /*    30 */    37,  164,   39,  133,   55,  135,  136,  137,  138,  139,
 /*    40 */    47,  174,  175,  176,  177,   52,    0,   54,    6,  182,
 /*    50 */   183,   91,   59,   60,   61,   62,   63,   64,   65,   66,
 /*    60 */    67,   68,   69,    6,   85,   86,   87,   88,   89,    6,
 /*    70 */    91,   92,   93,   32,  164,   39,  137,  138,  168,  140,
 /*    80 */    39,   40,   40,   90,  174,  175,  176,  177,   47,   48,
 /*    90 */    49,   98,   99,  100,  101,  102,  103,   40,  123,   36,
 /*   100 */    59,   60,   61,   62,   63,   64,   65,   66,   67,  134,
 /*   110 */    69,   70,   71,   72,  161,   74,   32,  164,   39,   78,
 /*   120 */    79,   80,   81,   39,   40,   84,   47,  174,  175,  176,
 /*   130 */   177,   47,   48,   49,   39,   94,   95,   96,   97,   37,
 /*   140 */   178,  179,   47,   59,   60,   61,   62,   63,   64,   65,
 /*   150 */    66,   67,   50,   69,   70,   71,   72,   40,   74,   47,
 /*   160 */    40,  164,   78,   79,   80,   81,  169,   47,   84,   32,
 /*   170 */    50,  174,  175,  176,  177,    6,   39,   40,   94,   95,
 /*   180 */    96,   97,   39,   39,   47,   48,   49,   85,   86,   87,
 /*   190 */    88,   89,   48,   49,   92,   93,   59,   60,   61,   62,
 /*   200 */    63,   64,   65,   66,   67,   36,   69,   70,   71,   72,
 /*   210 */   167,   74,   32,  170,  171,   78,   79,   80,   81,   39,
 /*   220 */    40,   84,  178,  179,  115,  164,  117,   47,   48,   49,
 /*   230 */   169,   94,   95,   96,   97,  174,  175,  176,  177,   59,
 /*   240 */    60,   61,   62,   63,   64,   65,   66,   67,  113,   69,
 /*   250 */    70,   71,   72,  119,   74,  121,  122,  123,   78,   79,
 /*   260 */    80,   81,  128,  129,   84,   32,  178,  179,  134,   54,
 /*   270 */    55,    6,   39,   40,   94,   95,   96,   97,  178,  179,
 /*   280 */    47,   48,   49,  105,  106,  107,  108,  109,  110,  111,
 /*   290 */   112,   39,   59,   60,   61,   62,   63,   64,   65,   66,
 /*   300 */    67,   36,   69,   70,   71,   72,   91,   74,   32,  164,
 /*   310 */     6,   78,   79,   80,   81,   39,   40,   84,  173,  174,
 /*   320 */   175,  176,  177,   47,   48,   49,  113,   94,   95,   96,
 /*   330 */    97,   37,    6,  178,  179,   59,   60,   61,   62,   63,
 /*   340 */    64,   65,   66,   67,   50,   69,   70,   71,   72,   39,
 /*   350 */    74,   47,  164,   42,   78,   79,   80,   81,   47,   49,
 /*   360 */    84,   32,  174,  175,  176,  177,   40,    6,   39,   40,
 /*   370 */    94,   95,   96,   97,  186,    6,   47,   48,   49,   85,
 /*   380 */    86,   87,   88,   89,  136,  137,  138,  139,   59,   60,
 /*   390 */    61,   62,   63,   64,   65,   66,   67,   36,   69,   70,
 /*   400 */    71,   72,  161,   74,   32,  164,    6,   78,   79,   80,
 /*   410 */    81,   39,   40,   84,   39,  174,  175,  176,  177,   47,
 /*   420 */    48,   49,   47,   94,   95,   96,   97,   58,    6,  178,
 /*   430 */   179,   59,   60,   61,   62,   63,   64,   65,   66,   67,
 /*   440 */    40,   69,   70,   71,   72,  161,   74,   40,  164,  134,
 /*   450 */    78,   79,   80,   81,   47,   39,   84,   32,  174,  175,
 /*   460 */   176,  177,   40,    6,   39,   40,   94,   95,   96,   97,
 /*   470 */   164,   54,   47,   48,   49,   39,    6,   40,  172,  173,
 /*   480 */   174,  175,  176,  177,   59,   60,   61,   62,   63,   64,
 /*   490 */    65,   66,   67,   36,   69,   70,   71,   72,  161,   74,
 /*   500 */    32,  164,   15,   78,   79,   80,   81,   39,   40,   84,
 /*   510 */    40,  174,  175,  176,  177,   47,   48,   49,   35,   94,
 /*   520 */    95,   96,   97,   37,  178,  179,   90,   59,   60,   61,
 /*   530 */    62,   63,   64,   65,   66,   67,   50,   69,   70,   71,
 /*   540 */    72,  161,   74,   47,  164,   39,   78,   79,   80,   81,
 /*   550 */    39,   55,   84,   32,  174,  175,  176,  177,   47,   47,
 /*   560 */    39,   40,   94,   95,   96,   97,  164,   55,   47,   48,
 /*   570 */    49,   85,   86,   87,   88,   89,  174,  175,  176,  177,
 /*   580 */    59,   60,   61,   62,   63,   64,   65,   66,   67,   42,
 /*   590 */    69,   70,   71,   72,   47,   74,   32,  178,  179,   78,
 /*   600 */    79,   80,   81,   39,   40,   84,    6,  178,  179,  164,
 /*   610 */    58,   47,   48,   49,  169,   94,   95,   96,   97,  174,
 /*   620 */   175,  176,  177,   59,   60,   61,   62,   63,   64,   65,
 /*   630 */    66,   67,   50,   69,   70,   71,   72,   39,   74,  121,
 /*   640 */    40,  123,   78,   79,   80,   81,  128,  129,   84,   32,
 /*   650 */    52,    6,  134,  178,  179,   47,   39,   40,   94,   95,
 /*   660 */    96,   97,  164,   55,   47,   48,   49,   85,   86,   87,
 /*   670 */    88,   89,  174,  175,  176,  177,   59,   60,   61,   62,
 /*   680 */    63,   64,   65,   66,   67,   40,   69,   70,   71,   72,
 /*   690 */    35,   74,   32,  164,   39,   78,   79,   80,   81,   39,
 /*   700 */    40,   84,   20,  174,  175,  176,  177,   47,   48,   49,
 /*   710 */   137,   94,   95,   96,   97,  165,  166,  123,   40,   59,
 /*   720 */    60,   61,   62,   63,   64,   65,   66,   67,  134,   69,
 /*   730 */    70,   71,   72,   55,   74,   40,   40,  164,   78,   79,
 /*   740 */    80,   81,   47,   47,   84,   32,   50,  174,  175,  176,
 /*   750 */   177,    6,   39,   40,   94,   95,   96,   97,   40,  123,
 /*   760 */    47,   48,   49,  133,    6,  135,  136,  137,  138,  139,
 /*   770 */   134,   40,   59,   60,   61,   62,   63,   64,   65,   66,
 /*   780 */    67,   36,   69,   70,   71,   72,   55,   74,   32,  164,
 /*   790 */     6,   78,   79,   80,   81,   39,   40,   84,   40,  174,
 /*   800 */   175,  176,  177,   47,   48,   49,    6,   94,   95,   96,
 /*   810 */    97,   37,  126,  127,   20,   59,   60,   61,   62,   63,
 /*   820 */    64,   65,   66,   67,   40,   69,   70,   71,   72,   55,
 /*   830 */    74,   37,  164,    6,   78,   79,   80,   81,   39,   50,
 /*   840 */    84,   32,  174,  175,  176,  177,   47,    6,   39,   40,
 /*   850 */    94,   95,   96,   97,  162,  163,   47,   48,   49,  133,
 /*   860 */     6,  135,  136,  137,  138,  139,  124,   40,   59,   60,
 /*   870 */    61,   62,   63,   64,   65,   66,   67,   36,   69,   70,
 /*   880 */    71,   72,   82,   74,   32,  164,   39,   78,   79,   80,
 /*   890 */    81,   39,   40,   84,   47,  174,  175,  176,  177,   47,
 /*   900 */    48,   49,   48,   94,   95,   96,   97,  115,   52,  117,
 /*   910 */    91,   59,   60,   61,   62,   63,   64,   65,   66,   67,
 /*   920 */     6,   69,   70,   71,   72,   54,   74,   39,  164,    6,
 /*   930 */    78,   79,   80,   81,   47,   50,   84,   32,  174,  175,
 /*   940 */   176,  177,   55,    6,   39,   40,   94,   95,   96,   97,
 /*   950 */    36,   39,   47,   48,   49,  133,  124,  135,  136,  137,
 /*   960 */   138,  139,   91,   40,   59,   60,   61,   62,   63,   64,
 /*   970 */    65,   66,   67,   36,   69,   70,   71,   72,   90,   74,
 /*   980 */    32,  164,   50,   78,   79,   80,   81,   39,   40,   84,
 /*   990 */   124,  174,  175,  176,  177,   47,   48,   49,  124,   94,
 /*  1000 */    95,   96,   97,   44,   39,   50,   47,   59,   60,   61,
 /*  1010 */    62,   63,   64,   65,   66,   67,    6,   69,   70,   71,
 /*  1020 */    72,   39,   74,   39,  164,   20,   78,   79,   80,   81,
 /*  1030 */    39,   54,   84,   32,  174,  175,  176,  177,   40,    6,
 /*  1040 */    39,   40,   94,   95,   96,   97,   36,   40,   47,   48,
 /*  1050 */    49,  133,   39,  135,  136,  137,  138,  139,  114,   39,
 /*  1060 */    59,   60,   61,   62,   63,   64,   65,   66,   67,   36,
 /*  1070 */    69,   70,   71,   72,   90,   74,   32,  164,  114,   78,
 /*  1080 */    79,   80,   81,   39,   40,   84,   40,  174,  175,  176,
 /*  1090 */   177,   47,   48,   49,   39,   94,   95,   96,   97,  114,
 /*  1100 */    39,   55,   48,   59,   60,   61,   62,   63,   64,   65,
 /*  1110 */    66,   67,    6,   69,   70,   71,   72,  167,   74,   39,
 /*  1120 */   164,  171,   78,   79,   80,   81,   54,   40,   84,   32,
 /*  1130 */   174,  175,  176,  177,   40,    6,   39,   40,   94,   95,
 /*  1140 */    96,   97,   36,   73,   47,   48,   49,  133,   47,  135,
 /*  1150 */   136,  137,  138,  139,   47,  163,   59,   60,   61,   62,
 /*  1160 */    63,   64,   65,   66,   67,   36,   69,   70,   71,   72,
 /*  1170 */    90,   74,   32,  164,  124,   78,   79,   80,   81,   39,
 /*  1180 */    40,   84,   40,  174,  175,  176,  177,   47,   48,   49,
 /*  1190 */    76,   94,   95,   96,   97,   37,   47,   55,   47,   59,
 /*  1200 */    60,   61,   62,   63,   64,   65,   66,   67,    6,   69,
 /*  1210 */    70,   71,   72,   55,   74,   47,  164,   79,   78,   79,
 /*  1220 */    80,   81,   40,   37,   84,   32,  174,  175,  176,  177,
 /*  1230 */    39,  180,   39,   40,   94,   95,   96,   97,   36,   36,
 /*  1240 */    47,   48,   49,  133,   39,  135,  136,  137,  138,  139,
 /*  1250 */    20,   54,   59,   60,   61,   62,   63,   64,   65,   66,
 /*  1260 */    67,   39,   69,   70,   71,   72,    6,   74,   32,  164,
 /*  1270 */   180,   78,   79,   80,   81,   39,   40,   84,   54,  174,
 /*  1280 */   175,  176,  177,   47,   48,   49,   54,   94,   95,   96,
 /*  1290 */    97,   39,   54,   48,   39,   59,   60,   61,   62,   63,
 /*  1300 */    64,   65,   66,   67,   48,   69,   70,   71,   72,   39,
 /*  1310 */    74,   54,  164,   48,   78,   79,   80,   81,   54,   76,
 /*  1320 */    84,   32,  174,  175,  176,  177,   76,   76,   39,   40,
 /*  1330 */    94,   95,   96,   97,   48,   54,   47,   48,   49,  133,
 /*  1340 */    48,  135,  136,  137,  138,  139,   48,   54,   59,   60,
 /*  1350 */    61,   62,   63,   64,   65,   66,   67,   48,   69,   70,
 /*  1360 */    71,   72,   39,   74,   32,  164,   39,   78,   79,   80,
 /*  1370 */    81,   39,   40,   84,  166,  174,  175,  176,  177,   47,
 /*  1380 */    48,   49,  167,   94,   95,   96,   97,  167,   58,   48,
 /*  1390 */   167,   59,   60,   61,   62,   63,   64,   65,   66,   67,
 /*  1400 */   167,   69,   70,   71,   72,   48,   74,  167,  164,   48,
 /*  1410 */    78,   79,   80,   81,   39,  167,   84,   32,  174,  175,
 /*  1420 */   176,  177,   58,   58,   39,   40,   94,   95,   96,   97,
 /*  1430 */   164,   40,   47,   48,   49,  167,   75,   40,   77,  167,
 /*  1440 */   174,  175,  176,  177,   59,   60,   61,   62,   63,   64,
 /*  1450 */    65,   66,   67,   40,   69,   70,   71,   72,  167,   74,
 /*  1460 */    32,  164,   40,   78,   79,   80,   81,   39,   40,   84,
 /*  1470 */    39,  174,  175,  176,  177,   47,   48,   49,   39,   94,
 /*  1480 */    95,   96,   97,   40,   40,   39,  179,   59,   60,   61,
 /*  1490 */    62,   63,   64,   65,   66,   67,  124,   69,   70,   71,
 /*  1500 */    72,   50,   74,   39,  164,   48,   78,   79,   80,   81,
 /*  1510 */    48,   39,   84,   32,  174,  175,  176,  177,   82,   79,
 /*  1520 */    39,   40,   94,   95,   96,   97,  164,   40,   47,   48,
 /*  1530 */    49,   76,   54,   55,   73,   47,  174,  175,  176,  177,
 /*  1540 */    59,   60,   61,   62,   63,   64,   65,   66,   67,   47,
 /*  1550 */    69,   70,   71,   72,   76,   74,   32,   47,   39,   78,
 /*  1560 */    79,   80,   81,   39,   40,   84,  121,  122,  123,   91,
 /*  1570 */    48,   47,   48,   49,  129,   94,   95,   96,   97,  134,
 /*  1580 */    54,   47,   47,   59,   60,   61,   62,   63,   64,   65,
 /*  1590 */    66,   67,   52,   69,   70,   71,   72,   75,   74,   77,
 /*  1600 */   123,   50,   78,   79,   80,   81,  131,  130,   84,   32,
 /*  1610 */   124,  134,  137,  138,   40,  140,   39,   40,   94,   95,
 /*  1620 */    96,   97,   38,   52,   47,   41,   49,   43,   50,   45,
 /*  1630 */    46,  124,   40,   49,  125,  124,   59,   60,   61,   62,
 /*  1640 */    63,   64,   65,   66,   67,  125,   69,   70,   71,   72,
 /*  1650 */    40,   74,   40,   40,  127,   78,   79,   80,   81,  112,
 /*  1660 */    39,   84,   42,   40,   44,  164,  125,   47,  124,   19,
 /*  1670 */   125,   94,   95,   96,   97,  174,  175,  176,  177,  132,
 /*  1680 */    40,   40,  181,  117,  114,  184,  185,  186,  141,  142,
 /*  1690 */   143,  144,  145,  146,  147,  148,  149,  150,  151,  152,
 /*  1700 */   153,  154,  155,  156,  157,  158,  159,  160,  112,   59,
 /*  1710 */    60,   61,   62,   63,   64,   65,   66,   67,   68,   69,
 /*  1720 */     0,  174,  175,  176,  177,   43,   39,   39,  132,  114,
 /*  1730 */     1,    2,    3,    4,    5,   43,  114,  141,  142,  143,
 /*  1740 */   144,  145,  146,  147,  148,  149,  150,  151,  152,  153,
 /*  1750 */   154,  155,  156,  157,  158,  159,  160,  112,   38,   47,
 /*  1760 */   114,   41,   39,   43,  187,   45,   46,  114,   39,   49,
 /*  1770 */   174,  175,  176,  177,   45,   46,   42,  132,   44,   39,
 /*  1780 */   114,   47,   53,  187,  187,   56,  141,  142,  143,  144,
 /*  1790 */   145,  146,  147,  148,  149,  150,  151,  152,  153,  154,
 /*  1800 */   155,  156,  157,  158,  159,  160,  112,  131,  187,  120,
 /*  1810 */   121,  122,  123,  137,  138,  187,  140,  128,  129,  174,
 /*  1820 */   175,  176,  177,  134,  187,  187,  132,  187,    1,    2,
 /*  1830 */     3,    4,    5,  187,  187,  141,  142,  143,  144,  145,
 /*  1840 */   146,  147,  148,  149,  150,  151,  152,  153,  154,  155,
 /*  1850 */   156,  157,  158,  159,  160,  112,  131,  187,   28,   29,
 /*  1860 */    30,  187,  137,  138,  187,  140,   39,   37,  174,  175,
 /*  1870 */   176,  177,   45,   46,  187,  132,  187,  187,  187,  187,
 /*  1880 */    53,  187,  187,   56,  141,  142,  143,  144,  145,  146,
 /*  1890 */   147,  148,  149,  150,  151,  152,  153,  154,  155,  156,
 /*  1900 */   157,  158,  159,  160,  112,  131,  187,  187,  187,  187,
 /*  1910 */   123,  137,  138,  187,  140,  187,  129,  174,  175,  176,
 /*  1920 */   177,  134,  187,  187,  132,  187,    1,    2,    3,    4,
 /*  1930 */     5,  187,  187,  141,  142,  143,  144,  145,  146,  147,
 /*  1940 */   148,  149,  150,  151,  152,  153,  154,  155,  156,  157,
 /*  1950 */   158,  159,  160,  112,  187,  187,  187,  187,  187,  187,
 /*  1960 */   187,  187,  187,  187,  187,  187,  174,  175,  176,  177,
 /*  1970 */    45,   46,  187,  132,  187,  187,   51,  187,  187,  187,
 /*  1980 */   187,   56,  141,  142,  143,  144,  145,  146,  147,  148,
 /*  1990 */   149,  150,  151,  152,  153,  154,  155,  156,  157,  158,
 /*  2000 */   159,  160,  112,  187,  187,  187,  187,  118,  187,  187,
 /*  2010 */   187,  187,  123,  187,  187,  174,  175,  176,  177,  130,
 /*  2020 */   187,  187,  132,  134,    1,    2,    3,    4,    5,  187,
 /*  2030 */   187,  141,  142,  143,  144,  145,  146,  147,  148,  149,
 /*  2040 */   150,  151,  152,  153,  154,  155,  156,  157,  158,  159,
 /*  2050 */   160,  112,  187,  187,  187,  187,  187,  187,  187,  187,
 /*  2060 */   187,  187,  164,  187,  174,  175,  176,  177,   45,   46,
 /*  2070 */   187,  132,  174,  175,  176,  177,   53,  187,  187,   56,
 /*  2080 */   141,  142,  143,  144,  145,  146,  147,  148,  149,  150,
 /*  2090 */   151,  152,  153,  154,  155,  156,  157,  158,  159,  160,
 /*  2100 */   112,  187,  187,  187,  187,  116,  187,  187,  119,  120,
 /*  2110 */   121,  122,  123,  174,  175,  176,  177,  128,  129,  187,
 /*  2120 */   132,  187,  187,  134,  187,  187,  187,  187,  187,  141,
 /*  2130 */   142,  143,  144,  145,  146,  147,  148,  149,  150,  151,
 /*  2140 */   152,  153,  154,  155,  156,  157,  158,  159,  160,  112,
 /*  2150 */   187,  187,  187,  187,  187,  187,  187,  187,  187,  187,
 /*  2160 */   187,  187,  174,  175,  176,  177,  187,  187,  187,  132,
 /*  2170 */   187,    1,    2,    3,    4,    5,  187,  187,  141,  142,
 /*  2180 */   143,  144,  145,  146,  147,  148,  149,  150,  151,  152,
 /*  2190 */   153,  154,  155,  156,  157,  158,  159,  160,  112,  187,
 /*  2200 */   187,  187,  187,  187,  187,  187,  187,  187,  187,  164,
 /*  2210 */   187,  174,  175,  176,  177,   45,   46,  187,  132,  174,
 /*  2220 */   175,  176,  177,   53,  187,  187,   56,  141,  142,  143,
 /*  2230 */   144,  145,  146,  147,  148,  149,  150,  151,  152,  153,
 /*  2240 */   154,  155,  156,  157,  158,  159,  160,  112,  187,  187,
 /*  2250 */   187,  187,  187,  187,  187,  187,  187,  187,  187,  187,
 /*  2260 */   174,  175,  176,  177,  187,  187,  187,  132,  187,    1,
 /*  2270 */     2,    3,    4,    5,  187,  187,  141,  142,  143,  144,
 /*  2280 */   145,  146,  147,  148,  149,  150,  151,  152,  153,  154,
 /*  2290 */   155,  156,  157,  158,  159,  160,  112,  187,  187,  187,
 /*  2300 */   187,  187,  187,  187,  187,  187,  187,  164,  187,  174,
 /*  2310 */   175,  176,  177,   45,   46,  187,  132,  174,  175,  176,
 /*  2320 */   177,   53,  187,  187,   56,  141,  142,  143,  144,  145,
 /*  2330 */   146,  147,  148,  149,  150,  151,  152,  153,  154,  155,
 /*  2340 */   156,  157,  158,  159,  160,  112,  187,  187,  187,  187,
 /*  2350 */   187,  187,  187,  187,  187,  187,  187,  187,  174,  175,
 /*  2360 */   176,  177,  187,  187,  187,  132,  187,    1,    2,    3,
 /*  2370 */     4,    5,  187,  187,  141,  142,  143,  144,  145,  146,
 /*  2380 */   147,  148,  149,  150,  151,  152,  153,  154,  155,  156,
 /*  2390 */   157,  158,  159,  160,  112,  187,  187,  187,  187,  187,
 /*  2400 */   187,  187,  187,  187,  187,  164,  187,  174,  175,  176,
 /*  2410 */   177,   45,   46,  187,  132,  174,  175,  176,  177,   53,
 /*  2420 */   187,  187,   56,  141,  142,  143,  144,  145,  146,  147,
 /*  2430 */   148,  149,  150,  151,  152,  153,  154,  155,  156,  157,
 /*  2440 */   158,  159,  160,  112,  187,  187,  187,  187,  187,  187,
 /*  2450 */   187,  187,  187,  187,  187,  187,  174,  175,  176,  177,
 /*  2460 */   187,  187,  187,  132,  187,    1,    2,    3,    4,    5,
 /*  2470 */   187,  187,  141,  142,  143,  144,  145,  146,  147,  148,
 /*  2480 */   149,  150,  151,  152,  153,  154,  155,  156,  157,  158,
 /*  2490 */   159,  160,  112,  187,  187,  187,  187,  187,  187,  187,
 /*  2500 */   187,  187,  187,  164,  187,  174,  175,  176,  177,   45,
 /*  2510 */    46,  187,  132,  174,  175,  176,  177,  187,  187,  187,
 /*  2520 */    56,  141,  142,  143,  144,  145,  146,  147,  148,  149,
 /*  2530 */   150,  151,  152,  153,  154,  155,  156,  157,  158,  159,
 /*  2540 */   160,  112,  187,  187,  187,  187,  187,  187,  187,  187,
 /*  2550 */   187,  187,  164,  187,  174,  175,  176,  177,  187,  187,
 /*  2560 */   187,  132,  174,  175,  176,  177,  187,  187,  187,  187,
 /*  2570 */   141,  142,  143,  144,  145,  146,  147,  148,  149,  150,
 /*  2580 */   151,  152,  153,  154,  155,  156,  157,  158,  159,  160,
 /*  2590 */   112,  187,  187,  187,  187,  187,  187,  187,  187,  187,
 /*  2600 */   187,  187,  187,  174,  175,  176,  177,  187,  187,  187,
 /*  2610 */   132,  187,    1,    2,    3,    4,    5,  187,  187,  141,
 /*  2620 */   142,  143,  144,  145,  146,  147,  148,  149,  150,  151,
 /*  2630 */   152,  153,  154,  155,  156,  157,  158,  159,  160,  112,
 /*  2640 */   187,  187,  187,  187,  187,  187,  187,  187,  187,  187,
 /*  2650 */   187,  187,  174,  175,  176,  177,   45,   46,  187,   48,
 /*  2660 */    49,  187,   51,  187,  187,  187,  187,   56,  141,  142,
 /*  2670 */   143,  144,  145,  146,  147,  148,  149,  150,  151,  152,
 /*  2680 */   153,  154,  155,  156,  157,  158,  159,  160,    7,    8,
 /*  2690 */     9,   10,   11,  187,  187,  187,  187,  187,  164,  187,
 /*  2700 */    19,  174,  175,  176,  177,  187,  187,  187,  174,  175,
 /*  2710 */   176,  177,   31,   32,   33,   34,   35,  183,   37,  164,
 /*  2720 */    39,  187,    7,    8,    9,   10,   11,  187,   47,  174,
 /*  2730 */   175,  176,  177,   52,   19,   54,  187,  187,  187,  184,
 /*  2740 */   185,  186,  187,  187,  187,  164,   31,   32,   33,   34,
 /*  2750 */    35,   36,   37,  187,   39,  174,  175,  176,  177,  187,
 /*  2760 */   187,  187,   47,  187,   83,  187,  187,   52,  187,   54,
 /*  2770 */   187,   90,  187,  187,  187,   39,  187,  187,  187,   98,
 /*  2780 */    99,  100,  101,  102,  103,   13,   14,  187,   52,   17,
 /*  2790 */    18,   19,   20,   21,   22,   23,   24,   25,   26,   27,
 /*  2800 */    28,   29,   30,  187,  187,   90,  187,  187,  187,   37,
 /*  2810 */   187,  187,  187,   98,   99,  100,  101,  102,  103,  164,
 /*  2820 */   187,    7,    8,    9,   10,   11,   90,  187,  187,  174,
 /*  2830 */   175,  176,  177,   19,   98,   99,  100,  101,  102,  103,
 /*  2840 */   187,  187,  187,  187,  187,   31,   32,   33,   34,   35,
 /*  2850 */    36,   37,  164,   39,  187,    7,    8,    9,   10,   11,
 /*  2860 */   187,   47,  174,  175,  176,  177,   52,   19,   54,  187,
 /*  2870 */   182,  183,  187,  187,  187,  187,  187,  187,  164,   31,
 /*  2880 */    32,   33,   34,   35,  187,   37,  187,   39,  174,  175,
 /*  2890 */   176,  177,  187,  187,  187,   47,  182,  183,  164,  187,
 /*  2900 */    52,  187,   54,  187,   90,  187,   58,  187,  174,  175,
 /*  2910 */   176,  177,   98,   99,  100,  101,  102,  103,  187,   12,
 /*  2920 */    13,   14,  187,  187,   17,   18,   19,   20,   21,   22,
 /*  2930 */    23,   24,   25,   26,   27,   28,   29,   30,   90,  187,
 /*  2940 */   187,  187,  187,  187,   37,  187,   98,   99,  100,  101,
 /*  2950 */   102,  103,  187,  187,    7,    8,    9,   10,   11,  164,
 /*  2960 */   187,  187,  187,  168,  164,   58,   19,  187,  168,  174,
 /*  2970 */   175,  176,  177,  187,  174,  175,  176,  177,   31,   32,
 /*  2980 */    33,   34,   35,   36,   37,  187,   39,  187,    7,    8,
 /*  2990 */     9,   10,   11,  187,   47,  164,  187,  187,  187,   52,
 /*  3000 */    19,   54,  187,  187,  187,  174,  175,  176,  177,  187,
 /*  3010 */   187,  164,   31,   32,   33,   34,   35,  187,   37,  187,
 /*  3020 */    39,  174,  175,  176,  177,  187,  187,  187,   47,  182,
 /*  3030 */   183,  164,  187,   52,  187,   54,  187,   90,  187,   58,
 /*  3040 */   187,  174,  175,  176,  177,   98,   99,  100,  101,  102,
 /*  3050 */   103,  187,   12,   13,   14,  187,  187,   17,   18,   19,
 /*  3060 */    20,   21,   22,   23,   24,   25,   26,   27,   28,   29,
 /*  3070 */    30,   90,  187,  187,  187,  187,   36,   37,  187,   98,
 /*  3080 */    99,  100,  101,  102,  103,  164,  187,    7,    8,    9,
 /*  3090 */    10,   11,  187,  187,  187,  174,  175,  176,  177,   19,
 /*  3100 */   187,  187,  187,  182,  183,  187,  187,  187,  187,  187,
 /*  3110 */   187,   31,   32,   33,   34,   35,   36,   37,  187,   39,
 /*  3120 */   187,    7,    8,    9,   10,   11,  187,   47,  164,  187,
 /*  3130 */   187,  187,   52,   19,   54,  187,  187,  187,  174,  175,
 /*  3140 */   176,  177,  187,  187,  164,   31,   32,   33,   34,   35,
 /*  3150 */   187,   37,  187,   39,  174,  175,  176,  177,  187,  187,
 /*  3160 */   187,   47,  182,  183,  164,  187,   52,  187,   54,  187,
 /*  3170 */    90,  187,   58,  187,  174,  175,  176,  177,   98,   99,
 /*  3180 */   100,  101,  102,  103,  187,   12,   13,   14,  187,  187,
 /*  3190 */    17,   18,   19,   20,   21,   22,   23,   24,   25,   26,
 /*  3200 */    27,   28,   29,   30,   90,  187,  187,  187,  187,  187,
 /*  3210 */    37,  187,   98,   99,  100,  101,  102,  103,  187,  187,
 /*  3220 */     7,    8,    9,   10,   11,  164,  187,  187,  187,  168,
 /*  3230 */   164,   58,   19,  187,  168,  174,  175,  176,  177,  187,
 /*  3240 */   174,  175,  176,  177,   31,   32,   33,   34,   35,   36,
 /*  3250 */    37,  187,   39,  187,    7,    8,    9,   10,   11,  187,
 /*  3260 */    47,  164,  187,  187,  187,   52,   19,   54,  187,  187,
 /*  3270 */   187,  174,  175,  176,  177,  187,  187,  164,   31,   32,
 /*  3280 */    33,   34,   35,   36,   37,  187,   39,  174,  175,  176,
 /*  3290 */   177,  187,  187,  187,   47,  182,  183,  187,  187,   52,
 /*  3300 */   187,   54,  187,   90,  187,  187,  187,  187,  187,  187,
 /*  3310 */   187,   98,   99,  100,  101,  102,  103,  187,  187,   17,
 /*  3320 */    18,   19,   20,   21,   22,   23,   24,   25,   26,   27,
 /*  3330 */    28,   29,   30,  187,  187,  187,  187,   90,  187,   37,
 /*  3340 */   187,  187,  187,  187,  187,   98,   99,  100,  101,  102,
 /*  3350 */   103,  164,  187,    7,    8,    9,   10,   11,  187,  187,
 /*  3360 */   187,  174,  175,  176,  177,   19,  187,  187,  187,  182,
 /*  3370 */   183,  187,  187,  187,  187,  187,  187,   31,   32,   33,
 /*  3380 */    34,   35,   36,   37,  187,   39,  187,    7,    8,    9,
 /*  3390 */    10,   11,  187,   47,  187,  187,  187,  187,   52,   19,
 /*  3400 */    54,  187,  187,  187,  187,  187,  187,  187,  187,  187,
 /*  3410 */    19,   31,   32,   33,   34,   35,  187,   37,  187,   39,
 /*  3420 */   187,    1,    2,    3,    4,    5,  187,   47,  187,  187,
 /*  3430 */    39,  187,   52,  187,   54,  187,   90,  187,  187,  187,
 /*  3440 */   187,  187,  187,  187,   98,   99,  100,  101,  102,  103,
 /*  3450 */    59,   60,   61,   62,   63,   64,   65,   66,   67,   68,
 /*  3460 */    69,  187,  187,   83,  187,   45,   46,  187,   48,   49,
 /*  3470 */    90,   51,  187,  187,  187,  187,   56,  187,   98,   99,
 /*  3480 */   100,  101,  102,  103,  164,  187,    7,    8,    9,   10,
 /*  3490 */    11,  187,  187,  187,  174,  175,  176,  177,   19,  187,
 /*  3500 */   187,  187,  182,  183,  187,  187,  187,  187,  187,  187,
 /*  3510 */    31,   32,   33,   34,   35,   36,   37,  187,   39,  187,
 /*  3520 */     7,    8,    9,   10,   11,  187,   47,  187,  187,  187,
 /*  3530 */   187,   52,   19,   54,  187,  187,  187,  187,  187,  187,
 /*  3540 */   187,  187,  187,  164,   31,   32,   33,   34,   35,  187,
 /*  3550 */    37,  187,   39,  174,  175,  176,  177,  187,  187,  187,
 /*  3560 */    47,  182,  183,  187,  187,   52,  187,   54,  187,   90,
 /*  3570 */   187,   58,  187,  187,  187,  187,  187,   98,   99,  100,
 /*  3580 */   101,  102,  103,  187,   12,   13,   14,  187,  187,   17,
 /*  3590 */    18,   19,   20,   21,   22,   23,   24,   25,   26,   27,
 /*  3600 */    28,   29,   30,   90,  187,  187,  187,  187,  187,   37,
 /*  3610 */   187,   98,   99,  100,  101,  102,  103,  187,  187,    7,
 /*  3620 */     8,    9,   10,   11,  164,  187,  187,  187,  168,  187,
 /*  3630 */    58,   19,  187,  187,  174,  175,  176,  177,  187,  187,
 /*  3640 */   187,  187,  187,   31,   32,   33,   34,   35,  187,   37,
 /*  3650 */   187,   39,   40,    7,    8,    9,   10,   11,  187,   47,
 /*  3660 */   187,  187,  187,  164,   52,   19,   54,  168,  187,  187,
 /*  3670 */   187,  187,  187,  174,  175,  176,  177,   31,   32,   33,
 /*  3680 */    34,   35,   36,   37,   19,   39,  187,  187,  187,  187,
 /*  3690 */   187,  187,  187,   47,  187,  187,  187,  187,   52,  187,
 /*  3700 */    54,   36,   90,  187,   39,  187,  187,  187,  187,  187,
 /*  3710 */    98,   99,  100,  101,  102,  103,  187,  187,  187,  187,
 /*  3720 */   187,  187,  187,  187,   59,   60,   61,   62,   63,   64,
 /*  3730 */    65,   66,   67,   68,   69,  187,   90,  187,  187,  187,
 /*  3740 */   187,  187,  187,  187,   98,   99,  100,  101,  102,  103,
 /*  3750 */   187,  187,    7,    8,    9,   10,   11,  164,  187,  187,
 /*  3760 */   187,  168,  164,  187,   19,  187,  168,  174,  175,  176,
 /*  3770 */   177,  187,  174,  175,  176,  177,   31,   32,   33,   34,
 /*  3780 */    35,   36,   37,  187,   39,  187,    7,    8,    9,   10,
 /*  3790 */    11,  187,   47,  187,  187,  187,  164,   52,   19,   54,
 /*  3800 */   168,  187,  187,  187,  187,  187,  174,  175,  176,  177,
 /*  3810 */    31,   32,   33,   34,   35,  187,   37,   19,   39,  187,
 /*  3820 */   187,  187,  187,  187,  187,  187,   47,  187,  164,  187,
 /*  3830 */   187,   52,  168,   54,   36,   90,  187,   39,  174,  175,
 /*  3840 */   176,  177,  187,   98,   99,  100,  101,  102,  103,  187,
 /*  3850 */   187,  187,  187,  187,  187,  187,  187,   59,   60,   61,
 /*  3860 */    62,   63,   64,   65,   66,   67,   68,   69,  187,   90,
 /*  3870 */   187,  187,  187,  187,  187,  187,  187,   98,   99,  100,
 /*  3880 */   101,  102,  103,  187,  187,    7,    8,    9,   10,   11,
 /*  3890 */   187,  187,  187,  187,  187,  187,  187,   19,  187,  187,
 /*  3900 */   187,  187,  187,  187,  187,  187,  187,  187,  187,   31,
 /*  3910 */    32,   33,   34,   35,  187,   37,  187,   39,  187,  187,
 /*  3920 */   187,  187,  187,  187,  187,   47,  187,  187,  187,  187,
 /*  3930 */    52,  187,   54,  187,  187,  187,  187,  187,   12,   13,
 /*  3940 */    14,  187,  187,   17,   18,   19,   20,   21,   22,   23,
 /*  3950 */    24,   25,   26,   27,   28,   29,   30,  187,  187,  187,
 /*  3960 */   187,  187,  187,   37,  187,  187,  187,  187,   90,  187,
 /*  3970 */   187,  187,  187,   47,  187,  187,   98,   99,  100,  101,
 /*  3980 */   102,  103,  187,  187,  187,  187,   12,   13,   14,  187,
 /*  3990 */   187,   17,   18,   19,   20,   21,   22,   23,   24,   25,
 /*  4000 */    26,   27,   28,   29,   30,  187,  187,  187,  187,  187,
 /*  4010 */   187,   37,  187,  187,   40,   12,   13,   14,  187,  187,
 /*  4020 */    17,   18,   19,   20,   21,   22,   23,   24,   25,   26,
 /*  4030 */    27,   28,   29,   30,  187,  187,  187,  187,  187,  187,
 /*  4040 */    37,  187,  187,   40,   12,   13,   14,  187,  187,   17,
 /*  4050 */    18,   19,   20,   21,   22,   23,   24,   25,   26,   27,
 /*  4060 */    28,   29,   30,  187,  187,  187,  187,  187,  187,   37,
 /*  4070 */   187,  187,  187,  187,  187,   12,   13,   14,  187,   47,
 /*  4080 */    17,   18,   19,   20,   21,   22,   23,   24,   25,   26,
 /*  4090 */    27,   28,   29,   30,  187,  187,  187,  187,  187,  187,
 /*  4100 */    37,  187,  187,  187,  187,  187,   12,   13,   14,  187,
 /*  4110 */    47,   17,   18,   19,   20,   21,   22,   23,   24,   25,
 /*  4120 */    26,   27,   28,   29,   30,  187,  187,  187,  187,  187,
 /*  4130 */   187,   37,  187,  187,  187,  187,  187,   12,   13,   14,
 /*  4140 */   187,   47,   17,   18,   19,   20,   21,   22,   23,   24,
 /*  4150 */    25,   26,   27,   28,   29,   30,  187,  187,  187,   12,
 /*  4160 */    13,   14,   37,  187,   17,   18,   19,   20,   21,   22,
 /*  4170 */    23,   24,   25,   26,   27,   28,   29,   30,  187,   19,
 /*  4180 */   187,  187,  187,   58,   37,  187,  187,  187,  187,  187,
 /*  4190 */    19,  187,  187,  187,  187,  187,   36,  187,  187,   39,
 /*  4200 */   187,  187,  187,   19,  187,  187,  187,   36,  187,  187,
 /*  4210 */    39,  187,    1,    2,    3,    4,    5,  187,  187,   59,
 /*  4220 */    60,   61,   62,   63,   64,   65,   66,   67,   68,   69,
 /*  4230 */    59,   60,   61,   62,   63,   64,   65,   66,   67,   68,
 /*  4240 */    69,   57,  187,   59,   60,   61,   62,   63,   64,   65,
 /*  4250 */    66,   67,   68,   69,  187,  187,   45,   46,  187,   48,
 /*  4260 */    49,  187,   51,  187,  187,  187,  187,   56,  187,    1,
 /*  4270 */     2,    3,    4,    5,  187,  187,  187,  187,  187,  187,
 /*  4280 */   187,  187,  187,  187,  187,  187,  187,  187,  187,  187,
 /*  4290 */   187,  187,  187,  187,  187,  187,  187,  187,  187,  187,
 /*  4300 */   187,  187,  187,  187,  187,  187,  187,  187,  187,  187,
 /*  4310 */   187,  187,  187,   45,   46,  187,   48,   49,  187,   51,
 /*  4320 */   187,  187,  187,  187,   56,  187,    1,    2,    3,    4,
 /*  4330 */     5,    1,    2,    3,    4,    5,  187,  187,  187,  187,
 /*  4340 */   187,  187,  187,  187,  187,  187,  187,  187,  187,  187,
 /*  4350 */   187,  187,  187,  187,  187,  187,  187,  187,  187,  187,
 /*  4360 */   187,  187,  187,  187,  187,  187,  187,  187,  187,  187,
 /*  4370 */    45,   46,  187,   48,   49,   45,   46,  187,   48,   49,
 /*  4380 */   187,   56,  187,  187,  187,  187,   56,  187,    1,    2,
 /*  4390 */     3,    4,    5,    1,    2,    3,    4,    5,  187,  187,
 /*  4400 */   187,  187,  187,  187,  187,  187,  187,  187,  187,  187,
 /*  4410 */   187,  187,  187,  187,  187,  187,  187,  187,  187,  187,
 /*  4420 */   187,  187,  187,  187,  187,  187,  187,  187,  187,  187,
 /*  4430 */   187,  187,   45,   46,  187,   48,   49,   45,   46,  187,
 /*  4440 */    48,   49,  187,   56,  187,  187,  187,  187,   56,  187,
 /*  4450 */     1,    2,    3,    4,    5,    1,    2,    3,    4,    5,
 /*  4460 */   187,  187,  187,  187,  187,  187,  187,  187,  187,  187,
 /*  4470 */   187,  187,  187,  187,  187,  187,  187,  187,  187,  187,
 /*  4480 */   187,  187,  187,  187,  187,  187,  187,  187,  187,  187,
 /*  4490 */   187,  187,  187,  187,   45,   46,  187,   48,   49,   45,
 /*  4500 */    46,  187,   48,   49,  187,   56,  187,  187,  187,  187,
 /*  4510 */    56,  187,    1,    2,    3,    4,    5,    1,    2,    3,
 /*  4520 */     4,    5,  187,  187,  187,  187,  187,  187,  187,  187,
 /*  4530 */   187,  187,  187,  187,  187,  187,  187,  187,  187,  187,
 /*  4540 */   187,  187,  187,  187,  187,  187,  187,  187,  187,  187,
 /*  4550 */   187,  187,  187,  187,  187,  187,   45,   46,  187,   48,
 /*  4560 */    49,   45,   46,  187,   48,   49,  187,   56,  187,  187,
 /*  4570 */   187,  187,   56,
};
#define YY_SHIFT_USE_DFLT (-41)
static short yy_shift_ofst[] = {
 /*     0 */  1584,   46, 1720,  -41,  -41,  -41,  -41,  -41,   36,  117,
 /*    10 */   -41,  143,  311,  -41,  252,  112,  -41, 4454,  -41, 4516,
 /*    20 */   -41,  -41, 2366,  416,  417, 3665, 1046, 4184,  437,  -41,
 /*    30 */   -41,  487, 1650,  -41,  483,  -41,  -41,  -41,  506,  794,
 /*    40 */   -41,  552,  682,  -41,  -41,  -41,  -41,  -41,  -41,  -41,
 /*    50 */   -41,  -41,  -41,  -41,  -41,  -41,  -41,  841,  678, 4184,
 /*    60 */   718,  -41,  -41, 3391,  -41,  789, 2736,  -41,  -41,  -41,
 /*    70 */   -41,  -41,  -41,  -41,  -41,  819,  856,  -41,  -41,  655,
 /*    80 */   885, 2736,  -41,  912,  932, 2736,  -41,  965,  955, 2736,
 /*    90 */   -41,  982, 1005,  -41,  -41,  -41,  -41,  -41,  -41,  -41,
 /*   100 */   -41,  -41,  -41, 2464, 2170,  991,  977, 4160, 1142, 4184,
 /*   110 */   998,  -41,  -41, 1202,  731, 4184, 1007,  -41,  -41,  -41,
 /*   120 */   -41, 1013, 1734,  -41, 1020,  959,  -41, 1055,  304,  -41,
 /*   130 */  4211,  -41, 1054,  -41, 3420, 4387,  -41, 2023, 1061, 1072,
 /*   140 */  3798,  887,  468,  -41,  564,  -41,  -41,  -41,  -41,  -41,
 /*   150 */   -41,  -41,  -41,  -41,  -41,  -41,  -41,  -41,  -41,  -41,
 /*   160 */   -41,  -41,  -41,  -41,  -41,  -41,  -41,  -41, 1087,  -41,
 /*   170 */  1094,  -41, 3779,  -29,  233,  137, 1070, 1101,  180, 1097,
 /*   180 */   -41, 3779, 1107, 1522,  -41, 1361,  -41,  -41, 2736, 1114,
 /*   190 */  1577, 1577, 1149,   84,  713,  -41, 3779, 1151,  660,   41,
 /*   200 */   -41, 1168,  521,  372, 1138, 3779, 1182,  -41, 3779, 1186,
 /*   210 */  3779, 1191,  -41,  774, 3779, 3779, 4147, 3779, 3779, 4147,
 /*   220 */  3779, 3779, 4147, 3779, 3779, 4147, 3779, 3779, 4147, 3779,
 /*   230 */   -20, 3779,  -20, 3779,  -20, 3779, 4125,  -41, 3779, 1830,
 /*   240 */  3779, 1830, 3779, 1186, 3779, 1186, 3779, 1186, 3779, 1830,
 /*   250 */  3779, 3302, 3779, 3302, 3779, 2772,   -7, 3040,  -41, 1203,
 /*   260 */  3779, 1186, 1205, 1230, 3779,  -20,  -40,  511, 1197, 3080,
 /*   270 */  4147, 1222, 1260, 1191,  -41,  -41, 3779, 1186,  -41,  -41,
 /*   280 */   -41,  -41,  -41,  -41,  -41,  -41, 2981, 4147, 1478,  598,
 /*   290 */  1224, 3646,  375, 1232, 2947,  -41,  457,  -41, 3779, 1252,
 /*   300 */  1245,  871, 2715,  265,  -41,  -41,  -41,  -41,  -41,  -41,
 /*   310 */    79, 1238, 3213,  169,  -41,  -41, 1255, 1256, 1257, 3346,
 /*   320 */    63,  -41,  -41,  -41, 1270, 1265, 1264, 3479,  -41, 1129,
 /*   330 */   -41, 1106,  -41,  -41,  -41, 3247, 1033,  -41,  -41, 1243,
 /*   340 */  1250,  -41,  369,  -41, 3878,  -41, 1251, 3779,  -41,  -41,
 /*   350 */   -41, 1010,  -41,  -41, 1080, 1286, 1281, 3745,  361,  -41,
 /*   360 */   -41, 1292, 1293, 2814,  937,  -41,  -41,  -20,  -20,  -20,
 /*   370 */   -20, 3173,  -41,  799,  -41,  888, 1298,  -41, 1309,  -41,
 /*   380 */   -20, 4147, 1323,  800, 2681, 3926, 1577,  756,  -41, 1327,
 /*   390 */   827,  -41, 1327,  -41,  -21,  -41,  -41,  -41,  -41,  -41,
 /*   400 */   -41, 3779,  -41, 4147,  847,  102, 3779,  -41, 3114,  582,
 /*   410 */  3779,  -41, 1330,  582, 3779,  -41, 3572,  -41,  -41,  984,
 /*   420 */  1341,  582, 3779,  -41, 1357,  582, 3779,  -41, 1375,  294,
 /*   430 */  3779,  -41, 2848,  582, 3779,  -41, 1364,  582, 3779,  -41,
 /*   440 */  3513,  582, 3779,  -41, 1365,  -41,  486, 3779,  -41,  -41,
 /*   450 */  3779,  -41,  -41,  -41,  -41, 3779,  923,  -41, 3779,  -41,
 /*   460 */  4147,  -41, 1391,  -41, 1397,  -41, 1413,  -41, 1422,  -41,
 /*   470 */  3612, 3974,  -41,  -41, 1431, 1158, 1439, 1443,  -41, 3779,
 /*   480 */  2907, 1444,  -41, 3779, 4003,  -41, 1446,   42,  -41, 1446,
 /*   490 */   -41, 1451, 2736,  -41,  -41, 1446,   57,  -41, 1446,  326,
 /*   500 */   -41, 1446,  400,  -41, 1446,  422,  -41, 1446,  470,  -41,
 /*   510 */  1446,  600,  -41, 1446,  645,  -41, 1446,  758,  -41, 1446,
 /*   520 */   784,  -41,  215, 1464, 1224,   95, 1197,  436, 1457, 1281,
 /*   530 */  1462, 1293,  -41,  -41, 3779, 4032, 1577, 1524,  -41, 1472,
 /*   540 */  1436, 3380, 4094, 1577,  852,  -41, 3779, 4063, 1577, 1044,
 /*   550 */   -41, 1440, 3779, 1487,  -41,  -41,  -41, 1455, 1577, 1577,
 /*   560 */   -41,  -41, 1461, 1488, 1193, 1428,  -41,  -41, 4184, 1502,
 /*   570 */  1385,  -41, 1289,  -41,  745,  496,  905,  -41,  809,  -41,
 /*   580 */  4184, 1510,  425,  -41, 1332,  -41, 2464, 2268, 1519, 1526,
 /*   590 */  4171,  512, 1236,  -41,  617,  -41, 4184, 1534,  948,  -41,
 /*   600 */  1001,  -41,  914,  608, 1140,  -41, 1481,  -41, 4184, 1535,
 /*   610 */   276,  -41,  329,  -41, 4268, 4449,  -41,  -41, 1925, 1540,
 /*   620 */  1551, 2736, 1574,  -41, 1571, 1578, 2736, 1592,  -41,  -41,
 /*   630 */  1925, 1729,  696,  -41, 2736,  695,  -41, 1610,  -41,  144,
 /*   640 */  1612,  854, 1613,  310,  -41,  -41, 1621,  -41,  -41, 1623,
 /*   650 */   -41, 1827,  120,  -41, 2736,  407,  -41, 1640,  -41, 1641,
 /*   660 */   -41,  -41, 2611, 4392, 4511, 2464, 4325,  -41, 4330, 1055,
 /*   670 */   -41,  -41,  -41, 1055,  304,  -41, 1682, 1687, 1620,  -41,
 /*   680 */  1688, 1712,  -41, 1055,  304,  -41, 1692, 1723,  547,  -41,
 /*   690 */  1740, 1712,  -41,  -41,
};
#define YY_REDUCE_USE_DFLT (-134)
static short yy_reduce_ofst[] = {
 /*     0 */   178, -134,  -89, -134, -134, -134, -134, -134, -134, -134,
 /*    10 */  -134, -134,  135, -134, -134,  213, -134, 1889, -134, 1477,
 /*    20 */  -134, -134,  315, -134, -134, 1475, -134, 1014, -134, -134,
 /*    30 */  -134, -134,  248, -134, -134, -134, -134, -134, -134, -134,
 /*    40 */  -134, -134, -134, -134, -134, -134, -134, -134, -134, -134,
 /*    50 */  -134, -134, -134, -134, -134, -134, -134, -134, -134,  822,
 /*    60 */  -134, -134, -134,  -61, -134, -134,  742, -134, -134, -134,
 /*    70 */  -134, -134, -134, -134, -134, -134, -134, -134, -134, -134,
 /*    80 */  -134,  832, -134, -134, -134,  866, -134, -134, -134,  874,
 /*    90 */  -134, -134, -134, -134, -134, -134, -134, -134, -134, -134,
 /*   100 */  -134, -134, -134,  -25,  315, -134, -134, 1676, -134,  726,
 /*   110 */  -134, -134, -134, -134, -134, -100, -134, -134, -134, -134,
 /*   120 */  -134, -134,  944, -134, -134,  964, -134,  792,  985, -134,
 /*   130 */  1989, -134, -134, -134, 1689, 1787, -134,  315, -134, -134,
 /*   140 */  1774, -134, 1890, -134, 2527, -134, -134, -134, -134, -134,
 /*   150 */  -134, -134, -134, -134, -134, -134, -134, -134, -134, -134,
 /*   160 */  -134, -134, -134, -134, -134, -134, -134, -134, -134, -134,
 /*   170 */  -134, -134,  -47, -134, 1988, 2527, -134, -134, 1547, 2527,
 /*   180 */  -134,  284, -134,  692, -134,  992, -134, -134, 1050, -134,
 /*   190 */  1939, 2527, -134, 2037, 2527, -134,  380, -134, 2135, 2527,
 /*   200 */  -134, -134, 2184, 2527, -134,  241, -134, -134, 2655, -134,
 /*   210 */  2734, 1051, -134, -134, 2831, 2867, -134, 2964, 3000, -134,
 /*   220 */  3097, 2045, -134,  402,  498, -134,  625,  668, -134,  721,
 /*   230 */  -134,  764, -134,  817, -134,  860, -134, -134,  913, -134,
 /*   240 */  1009, -134, 1052, -134, 1148, -134, 1201, -134, 1244, -134,
 /*   250 */  1266, -134, 1297, -134, 1340, -134,  573, -134, -134, -134,
 /*   260 */  1105, -134, -134, -134,  956, -134, -134, -134, -134, 3379,
 /*   270 */  -134, -134, -134, 1090, -134, -134,  529, -134, -134, -134,
 /*   280 */  -134, -134, -134, -134, -134, -134, 1501, -134, -134, -134,
 /*   290 */  -134, 3320, -134, -134, 3187, -134, -134, -134, 2534, -134,
 /*   300 */  -134, -134, -133, -134, -134, -134, -134, -134, -134, -134,
 /*   310 */  -134, -134, 2688, -134, -134, -134, -134, -134, -134, 2714,
 /*   320 */  -134, -134, -134, -134, -134, -134, -134, 2847, -134, -134,
 /*   330 */  -134, -134, -134, -134, -134, 2921, -134, -134, -134, -134,
 /*   340 */  -134, -134, -134, -134, 2555, -134, -134,  188, -134, -134,
 /*   350 */  -134, -134, -134, -134, -134, -134, -134, 3113, -134, -134,
 /*   360 */  -134, -134, -134, 2980, -134, -134, -134, -134, -134, -134,
 /*   370 */  -134, -134, -134, -134, -134, -134, -134, -134, -134, -134,
 /*   380 */  -134, -134, -134, -134, 2581, -134, 2086, 2527, -134,  550,
 /*   390 */  -134, -134, 1208, -134,   43, -134, -134, -134, -134, -134,
 /*   400 */  -134, 3632, -134, -134, -134, 1215, 3061, -134,  445, 1220,
 /*   410 */  3598, -134, -134, 1223, 3066, -134, -134, -134, -134, -134,
 /*   420 */  -134, 1233,  -90, -134, -134, 1240, 3593, -134, -134, 1248,
 /*   430 */  3499, -134,   -3, 1268, 2795, -134, -134, 1272, 3664, -134,
 /*   440 */    61, 1291, 2800, -134, -134, -134,  950, 3460, -134, -134,
 /*   450 */    61, -134, -134, -134, -134,  306, -134, -134,  145, -134,
 /*   460 */  -134, -134, -134, -134, -134, -134, -134, -134, -134, -134,
 /*   470 */  2388, -134, -134, -134, -134, -134, -134, -134, -134, 2339,
 /*   480 */  -134, -134, -134, 2241, -134, -134,  -38, -134, -134, 1307,
 /*   490 */  -134, -134, 1372, -134, -134,   44, -134, -134,   88, -134,
 /*   500 */  -134,  100, -134, -134,  155, -134, -134,  251, -134, -134,
 /*   510 */   346, -134, -134,  419, -134, -134,  429, -134, -134,  475,
 /*   520 */  -134, -134, -134, -134, -134, -134, -134, -134, -134, -134,
 /*   530 */  -134, -134, -134, -134, 2143, -134, 2282, 2527, -134, -134,
 /*   540 */  -134, 1898, -134, 2429, 2527, -134, 1362, -134, 1596, 2527,
 /*   550 */  -134, -134,  337, -134, -134, -134, -134, -134, 1743, 2527,
 /*   560 */  -134, -134, -134, -134, 1792, 2527, -134, -134,  630, -134,
 /*   570 */  1841, -134, 2527, -134, -134, -134, 1694, -134, 2527, -134,
 /*   580 */  1110, -134, 1645, -134, 2527, -134,  636,  315, -134, -134,
 /*   590 */  1725, -134, 2478, -134, 2527, -134, 1206, -134, 2380, -134,
 /*   600 */  2527, -134, -134, -134, 2331, -134, 2527, -134,  918, -134,
 /*   610 */  2233, -134, 2527, -134,  518, 1787, -134, -134,  636, -134,
 /*   620 */  -134, 1486, -134, -134, -134, -134, 1507, -134, -134, -134,
 /*   630 */   594,  315, 1509, -134, 1511, 1520, -134, -134, -134,  686,
 /*   640 */  -134, -134, -134, 1527, -134, -134, -134, -134, -134, -134,
 /*   650 */  -134,  315, 1541, -134, 1544, 1545, -134, -134, -134, -134,
 /*   660 */  -134, -134,  134, 1445, 1787,  594, 1787, -134, 1787, 1566,
 /*   670 */  -134, -134, -134,  109, 1570, -134, -134, -134, 1615, -134,
 /*   680 */  -134, 1622, -134, -104, 1646, -134, -134, -134, 1653, -134,
 /*   690 */  -134, 1666, -134, -134,
};
static YYACTIONTYPE yy_default[] = {
 /*     0 */  1021, 1021, 1021,  696,  698,  699,  700,  701, 1021, 1021,
 /*    10 */   702, 1021, 1021,  703, 1021, 1021,  704, 1021,  719, 1021,
 /*    20 */   720,  750, 1021, 1021, 1021, 1021, 1021, 1021, 1021,  770,
 /*    30 */   788,  789, 1021,  790,  792,  793,  794,  795, 1021, 1021,
 /*    40 */   806, 1021, 1021,  807,  808,  809,  810,  811,  812,  813,
 /*    50 */   814,  815,  816,  817,  818,  791,  774, 1021, 1021, 1021,
 /*    60 */  1021,  771,  775, 1021,  796,  798, 1021,  802, 1006, 1007,
 /*    70 */  1008, 1009, 1010, 1011, 1012, 1021, 1021, 1013, 1014, 1021,
 /*    80 */   799, 1021,  803, 1021,  800, 1021,  804, 1021,  801, 1021,
 /*    90 */   805, 1021, 1021,  797,  778,  780,  781,  782,  783,  784,
 /*   100 */   785,  786,  787, 1021, 1021, 1021, 1021, 1021, 1021, 1021,
 /*   110 */  1021,  772,  776, 1021, 1021, 1021, 1021,  773,  777,  779,
 /*   120 */   751, 1021, 1021,  705, 1021, 1021,  706, 1021, 1021,  708,
 /*   130 */  1021,  714, 1021,  715, 1021, 1021,  748, 1021, 1021, 1021,
 /*   140 */  1021, 1021, 1021,  754, 1021,  756,  819,  821,  822,  823,
 /*   150 */   824,  825,  826,  827,  828,  829,  830,  831,  832,  833,
 /*   160 */   834,  835,  836,  837,  838,  839,  840,  841, 1021,  842,
 /*   170 */  1021,  843, 1021, 1021, 1021, 1021,  846, 1021, 1021, 1021,
 /*   180 */   847, 1021, 1021, 1021,  850, 1021,  851,  852, 1021, 1021,
 /*   190 */   854,  855, 1021, 1021, 1021,  858, 1021, 1021, 1021, 1021,
 /*   200 */   860, 1021, 1021, 1021, 1021, 1021, 1021,  862, 1021,  923,
 /*   210 */  1021, 1021,  924, 1021, 1021, 1021,  925, 1021, 1021,  926,
 /*   220 */  1021, 1021,  927, 1021, 1021,  928, 1021, 1021,  929, 1021,
 /*   230 */   935, 1021,  936, 1021,  937, 1021, 1021,  946, 1021,  947,
 /*   240 */  1021,  948, 1021,  949, 1021,  950, 1021,  951, 1021,  952,
 /*   250 */  1021,  953, 1021,  954, 1021,  955, 1021, 1021,  938, 1021,
 /*   260 */  1021,  939, 1021, 1021, 1021,  940,  959, 1021,  941, 1021,
 /*   270 */   997, 1021, 1021, 1021,  956,  957, 1021,  958,  960,  961,
 /*   280 */   962,  963,  964,  965,  966,  967, 1021, 1005,  959, 1021,
 /*   290 */   944, 1021, 1021,  970, 1021,  971, 1021,  972, 1021, 1021,
 /*   300 */  1021, 1021, 1021, 1021,  978,  979,  992,  993,  994,  996,
 /*   310 */  1021, 1021, 1021, 1021,  982,  983, 1021, 1021, 1021, 1021,
 /*   320 */  1021,  984,  985,  995, 1021, 1021,  973, 1021,  974, 1021,
 /*   330 */   975, 1021,  980,  981,  945, 1021, 1021,  976,  977,  960,
 /*   340 */   961,  968, 1021,  969, 1021,  998, 1021, 1021, 1000, 1001,
 /*   350 */   999, 1021,  986,  987, 1021, 1021,  942, 1021, 1021,  988,
 /*   360 */   989, 1021,  943, 1021, 1021,  990,  991,  934,  933,  932,
 /*   370 */   931, 1021, 1015, 1021, 1016, 1021, 1021, 1017, 1021, 1018,
 /*   380 */   930, 1019, 1021, 1021, 1021, 1021, 1021, 1021,  863, 1021,
 /*   390 */  1021,  867, 1021,  868, 1021,  870,  871,  872,  873,  874,
 /*   400 */   875, 1021,  876,  922, 1021, 1021, 1021,  877, 1021, 1021,
 /*   410 */  1021,  880, 1021, 1021, 1021,  881, 1021,  890,  891, 1021,
 /*   420 */  1021, 1021, 1021,  878, 1021, 1021, 1021,  879, 1021, 1021,
 /*   430 */  1021,  882, 1021, 1021, 1021,  883, 1021, 1021, 1021,  884,
 /*   440 */  1021, 1021, 1021,  885, 1021,  889, 1021, 1021,  886,  887,
 /*   450 */  1021,  888,  892,  893,  869, 1021, 1021,  895, 1021,  896,
 /*   460 */   898,  897, 1021,  899, 1021,  900, 1021,  901, 1021,  902,
 /*   470 */  1021, 1021,  903,  904, 1021, 1021, 1021, 1021,  905, 1021,
 /*   480 */  1021, 1021,  906, 1021, 1021,  907, 1021, 1021,  908, 1021,
 /*   490 */   918,  920, 1021,  921,  919, 1021, 1021,  909, 1021, 1021,
 /*   500 */   910, 1021, 1021,  911, 1021, 1021,  912, 1021, 1021,  913,
 /*   510 */  1021, 1021,  914, 1021, 1021,  915, 1021, 1021,  916, 1021,
 /*   520 */  1021,  917, 1021, 1021, 1021, 1021, 1021, 1021, 1021, 1021,
 /*   530 */  1021, 1021, 1020,  820, 1021, 1021, 1021, 1021,  864, 1021,
 /*   540 */  1021, 1021, 1021, 1021, 1021,  865, 1021, 1021, 1021, 1021,
 /*   550 */   866, 1021, 1021, 1021,  861,  859,  857, 1021, 1021,  856,
 /*   560 */   853,  848,  844, 1021, 1021, 1021,  849,  845, 1021, 1021,
 /*   570 */  1021,  762, 1021,  764, 1021, 1021, 1021,  755, 1021,  757,
 /*   580 */  1021, 1021, 1021,  763, 1021,  765, 1021, 1021, 1021, 1021,
 /*   590 */  1021, 1021, 1021,  758, 1021,  760, 1021, 1021, 1021,  766,
 /*   600 */  1021,  768, 1021, 1021, 1021,  759, 1021,  761, 1021, 1021,
 /*   610 */  1021,  767, 1021,  769, 1021, 1021,  746,  749, 1021, 1021,
 /*   620 */  1021, 1021, 1021,  752, 1021, 1021, 1021, 1021,  753,  730,
 /*   630 */  1021, 1021, 1021,  732, 1021, 1021,  734, 1021,  738, 1021,
 /*   640 */  1021, 1021, 1021, 1021,  742,  744, 1021,  745,  743, 1021,
 /*   650 */   736, 1021, 1021,  733, 1021, 1021,  735, 1021,  739, 1021,
 /*   660 */   737,  747, 1021, 1021, 1021, 1021, 1021,  731, 1021, 1021,
 /*   670 */   716,  718,  717, 1021, 1021,  707, 1021, 1021, 1021,  709,
 /*   680 */  1021, 1021,  710, 1021, 1021,  711, 1021, 1021, 1021,  712,
 /*   690 */  1021, 1021,  713,  697,
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
  xx_ARG_SDECL                /* A place to hold %extra_argument */
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
void xx_Trace(FILE *TraceFILE, char *zTracePrompt){
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
  "LIKELY",        "UNLIKELY",      "CLONE",         "EMPTY",       
  "INSTANCEOF",    "AND",           "OR",            "BITWISE_OR",  
  "BITWISE_AND",   "EQUALS",        "IDENTICAL",     "LESS",        
  "GREATER",       "LESSEQUAL",     "GREATEREQUAL",  "NOTIDENTICAL",
  "NOTEQUALS",     "ADD",           "SUB",           "CONCAT",      
  "MUL",           "DIV",           "MOD",           "ISSET",       
  "FETCH",         "TYPEOF",        "NEW",           "NOT",         
  "PARENTHESES_CLOSE",  "SBRACKET_OPEN",  "NAMESPACE",     "IDENTIFIER",  
  "DOTCOMMA",      "INTERFACE",     "EXTENDS",       "CLASS",       
  "IMPLEMENTS",    "ABSTRACT",      "FINAL",         "BRACKET_OPEN",
  "BRACKET_CLOSE",  "COMMENT",       "ASSIGN",        "CONST",       
  "CONSTANT",      "FUNCTION",      "PARENTHESES_OPEN",  "ARROW",       
  "INLINE",        "VOID",          "SBRACKET_CLOSE",  "TYPE_INTEGER",
  "TYPE_UINTEGER",  "TYPE_LONG",     "TYPE_ULONG",    "TYPE_CHAR",   
  "TYPE_UCHAR",    "TYPE_DOUBLE",   "TYPE_BOOL",     "TYPE_STRING", 
  "TYPE_ARRAY",    "TYPE_VAR",      "BREAK",         "CONTINUE",    
  "IF",            "ELSE",          "SWITCH",        "CASE",        
  "COLON",         "DEFAULT",       "LOOP",          "WHILE",       
  "DO",            "FOR",           "IN",            "REVERSE",     
  "LET",           "ADDASSIGN",     "SUBASSIGN",     "MULASSIGN",   
  "DIVASSIGN",     "CONCATASSIGN",  "STRING",        "DOUBLECOLON", 
  "INCR",          "DECR",          "ECHO",          "RETURN",      
  "UNSET",         "THROW",         "INTEGER",       "CHAR",        
  "DOUBLE",        "NULL",          "TRUE",          "FALSE",       
  "error",         "program",       "xx_language",   "xx_top_statement_list",
  "xx_top_statement",  "xx_namespace_def",  "xx_class_def",  "xx_interface_def",
  "xx_comment",    "xx_interface_body",  "xx_class_body",  "xx_implements_list",
  "xx_class_definition",  "xx_implements",  "xx_interface_methods_definition",  "xx_class_properties_definition",
  "xx_class_consts_definition",  "xx_class_methods_definition",  "xx_class_property_definition",  "xx_visibility_list",
  "xx_literal_expr",  "xx_class_property_shortcuts",  "xx_class_property_shortcuts_list",  "xx_class_property_shortcut",
  "xx_class_const_definition",  "xx_class_method_definition",  "xx_interface_method_definition",  "xx_parameter_list",
  "xx_statement_list",  "xx_method_return_type",  "xx_visibility",  "xx_method_return_type_list",
  "xx_method_return_type_item",  "xx_parameter_type",  "xx_parameter_cast",  "xx_parameter_cast_collection",
  "xx_parameter",  "xx_statement",  "xx_let_statement",  "xx_if_statement",
  "xx_loop_statement",  "xx_echo_statement",  "xx_return_statement",  "xx_fetch_statement",
  "xx_fcall_statement",  "xx_mcall_statement",  "xx_scall_statement",  "xx_unset_statement",
  "xx_throw_statement",  "xx_declare_statement",  "xx_break_statement",  "xx_continue_statement",
  "xx_while_statement",  "xx_do_while_statement",  "xx_switch_statement",  "xx_for_statement",
  "xx_empty_statement",  "xx_eval_expr",  "xx_case_clauses",  "xx_case_clause",
  "xx_common_expr",  "xx_let_assignments",  "xx_let_assignment",  "xx_assignment_operator",
  "xx_assign_expr",  "xx_index_expr",  "xx_array_offset_list",  "xx_array_offset",
  "xx_echo_expressions",  "xx_echo_expression",  "xx_mcall_expr",  "xx_fcall_expr",
  "xx_scall_expr",  "xx_fetch_expr",  "xx_declare_variable_list",  "xx_declare_variable",
  "xx_isset_expr",  "xx_array_list",  "xx_call_parameters",  "xx_call_parameter",
  "xx_array_item",  "xx_array_key",  "xx_array_value",
};
#endif /* NDEBUG */

#ifndef NDEBUG
/* For tracing reduce actions, the names of all rules are required.
*/
static const char *yyRuleName[] = {
 /*   0 */ "program ::= xx_language",
 /*   1 */ "xx_language ::= xx_top_statement_list",
 /*   2 */ "xx_top_statement_list ::= xx_top_statement_list xx_top_statement",
 /*   3 */ "xx_top_statement_list ::= xx_top_statement",
 /*   4 */ "xx_top_statement ::= xx_namespace_def",
 /*   5 */ "xx_top_statement ::= xx_class_def",
 /*   6 */ "xx_top_statement ::= xx_interface_def",
 /*   7 */ "xx_top_statement ::= xx_comment",
 /*   8 */ "xx_namespace_def ::= NAMESPACE IDENTIFIER DOTCOMMA",
 /*   9 */ "xx_interface_def ::= INTERFACE IDENTIFIER xx_interface_body",
 /*  10 */ "xx_interface_def ::= INTERFACE IDENTIFIER EXTENDS IDENTIFIER xx_interface_body",
 /*  11 */ "xx_class_def ::= CLASS IDENTIFIER xx_class_body",
 /*  12 */ "xx_class_def ::= CLASS IDENTIFIER EXTENDS IDENTIFIER xx_class_body",
 /*  13 */ "xx_class_def ::= CLASS IDENTIFIER IMPLEMENTS xx_implements_list xx_class_body",
 /*  14 */ "xx_class_def ::= CLASS IDENTIFIER EXTENDS IDENTIFIER IMPLEMENTS xx_implements_list xx_class_body",
 /*  15 */ "xx_class_def ::= ABSTRACT CLASS IDENTIFIER xx_class_body",
 /*  16 */ "xx_class_def ::= ABSTRACT CLASS IDENTIFIER EXTENDS IDENTIFIER xx_class_body",
 /*  17 */ "xx_class_def ::= ABSTRACT CLASS IDENTIFIER IMPLEMENTS xx_implements_list xx_class_body",
 /*  18 */ "xx_class_def ::= FINAL CLASS IDENTIFIER xx_class_body",
 /*  19 */ "xx_class_def ::= FINAL CLASS IDENTIFIER EXTENDS IDENTIFIER xx_class_body",
 /*  20 */ "xx_class_body ::= BRACKET_OPEN BRACKET_CLOSE",
 /*  21 */ "xx_class_body ::= BRACKET_OPEN xx_class_definition BRACKET_CLOSE",
 /*  22 */ "xx_implements_list ::= xx_implements_list COMMA xx_implements",
 /*  23 */ "xx_implements_list ::= xx_implements",
 /*  24 */ "xx_implements ::= IDENTIFIER",
 /*  25 */ "xx_interface_body ::= BRACKET_OPEN BRACKET_CLOSE",
 /*  26 */ "xx_interface_body ::= BRACKET_OPEN xx_interface_methods_definition BRACKET_CLOSE",
 /*  27 */ "xx_class_definition ::= xx_class_properties_definition",
 /*  28 */ "xx_class_definition ::= xx_class_consts_definition",
 /*  29 */ "xx_class_definition ::= xx_class_methods_definition",
 /*  30 */ "xx_class_definition ::= xx_class_properties_definition xx_class_methods_definition",
 /*  31 */ "xx_class_definition ::= xx_class_properties_definition xx_class_consts_definition",
 /*  32 */ "xx_class_definition ::= xx_class_consts_definition xx_class_properties_definition",
 /*  33 */ "xx_class_definition ::= xx_class_consts_definition xx_class_methods_definition",
 /*  34 */ "xx_class_definition ::= xx_class_properties_definition xx_class_consts_definition xx_class_methods_definition",
 /*  35 */ "xx_class_definition ::= xx_class_consts_definition xx_class_properties_definition xx_class_methods_definition",
 /*  36 */ "xx_class_properties_definition ::= xx_class_properties_definition xx_class_property_definition",
 /*  37 */ "xx_class_properties_definition ::= xx_class_property_definition",
 /*  38 */ "xx_class_property_definition ::= COMMENT xx_visibility_list IDENTIFIER DOTCOMMA",
 /*  39 */ "xx_class_property_definition ::= xx_visibility_list IDENTIFIER DOTCOMMA",
 /*  40 */ "xx_class_property_definition ::= COMMENT xx_visibility_list IDENTIFIER ASSIGN xx_literal_expr DOTCOMMA",
 /*  41 */ "xx_class_property_definition ::= xx_visibility_list IDENTIFIER ASSIGN xx_literal_expr DOTCOMMA",
 /*  42 */ "xx_class_property_definition ::= COMMENT xx_visibility_list IDENTIFIER xx_class_property_shortcuts DOTCOMMA",
 /*  43 */ "xx_class_property_definition ::= xx_visibility_list IDENTIFIER xx_class_property_shortcuts DOTCOMMA",
 /*  44 */ "xx_class_property_definition ::= COMMENT xx_visibility_list IDENTIFIER ASSIGN xx_literal_expr xx_class_property_shortcuts DOTCOMMA",
 /*  45 */ "xx_class_property_definition ::= xx_visibility_list IDENTIFIER ASSIGN xx_literal_expr xx_class_property_shortcuts DOTCOMMA",
 /*  46 */ "xx_class_property_shortcuts ::= BRACKET_OPEN BRACKET_CLOSE",
 /*  47 */ "xx_class_property_shortcuts ::= BRACKET_OPEN xx_class_property_shortcuts_list BRACKET_CLOSE",
 /*  48 */ "xx_class_property_shortcuts_list ::= xx_class_property_shortcuts_list COMMA xx_class_property_shortcut",
 /*  49 */ "xx_class_property_shortcuts_list ::= xx_class_property_shortcut",
 /*  50 */ "xx_class_property_shortcut ::= IDENTIFIER",
 /*  51 */ "xx_class_property_shortcut ::= COMMENT IDENTIFIER",
 /*  52 */ "xx_class_consts_definition ::= xx_class_consts_definition xx_class_const_definition",
 /*  53 */ "xx_class_consts_definition ::= xx_class_const_definition",
 /*  54 */ "xx_class_methods_definition ::= xx_class_methods_definition xx_class_method_definition",
 /*  55 */ "xx_class_methods_definition ::= xx_class_method_definition",
 /*  56 */ "xx_interface_methods_definition ::= xx_interface_methods_definition xx_interface_method_definition",
 /*  57 */ "xx_interface_methods_definition ::= xx_interface_method_definition",
 /*  58 */ "xx_class_const_definition ::= COMMENT CONST CONSTANT ASSIGN xx_literal_expr DOTCOMMA",
 /*  59 */ "xx_class_const_definition ::= CONST CONSTANT ASSIGN xx_literal_expr DOTCOMMA",
 /*  60 */ "xx_class_method_definition ::= xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE BRACKET_OPEN BRACKET_CLOSE",
 /*  61 */ "xx_class_method_definition ::= xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE BRACKET_OPEN BRACKET_CLOSE",
 /*  62 */ "xx_class_method_definition ::= xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  63 */ "xx_class_method_definition ::= xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  64 */ "xx_class_method_definition ::= COMMENT xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE BRACKET_OPEN BRACKET_CLOSE",
 /*  65 */ "xx_class_method_definition ::= COMMENT xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE BRACKET_OPEN BRACKET_CLOSE",
 /*  66 */ "xx_class_method_definition ::= COMMENT xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  67 */ "xx_class_method_definition ::= COMMENT xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  68 */ "xx_class_method_definition ::= xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE ARROW xx_method_return_type BRACKET_OPEN BRACKET_CLOSE",
 /*  69 */ "xx_class_method_definition ::= xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE ARROW xx_method_return_type BRACKET_OPEN BRACKET_CLOSE",
 /*  70 */ "xx_class_method_definition ::= xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE ARROW xx_method_return_type BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  71 */ "xx_class_method_definition ::= xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE ARROW xx_method_return_type BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  72 */ "xx_class_method_definition ::= COMMENT xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE ARROW xx_method_return_type BRACKET_OPEN BRACKET_CLOSE",
 /*  73 */ "xx_class_method_definition ::= COMMENT xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE ARROW xx_method_return_type BRACKET_OPEN BRACKET_CLOSE",
 /*  74 */ "xx_class_method_definition ::= COMMENT xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE ARROW xx_method_return_type BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  75 */ "xx_class_method_definition ::= COMMENT xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE ARROW xx_method_return_type BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  76 */ "xx_interface_method_definition ::= xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE ARROW xx_method_return_type DOTCOMMA",
 /*  77 */ "xx_interface_method_definition ::= xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE ARROW xx_method_return_type DOTCOMMA",
 /*  78 */ "xx_interface_method_definition ::= COMMENT xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE ARROW xx_method_return_type DOTCOMMA",
 /*  79 */ "xx_interface_method_definition ::= COMMENT xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE ARROW xx_method_return_type DOTCOMMA",
 /*  80 */ "xx_interface_method_definition ::= xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE DOTCOMMA",
 /*  81 */ "xx_interface_method_definition ::= xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE DOTCOMMA",
 /*  82 */ "xx_interface_method_definition ::= COMMENT xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE DOTCOMMA",
 /*  83 */ "xx_interface_method_definition ::= COMMENT xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE DOTCOMMA",
 /*  84 */ "xx_visibility_list ::= xx_visibility_list xx_visibility",
 /*  85 */ "xx_visibility_list ::= xx_visibility",
 /*  86 */ "xx_visibility ::= PUBLIC",
 /*  87 */ "xx_visibility ::= PROTECTED",
 /*  88 */ "xx_visibility ::= PRIVATE",
 /*  89 */ "xx_visibility ::= STATIC",
 /*  90 */ "xx_visibility ::= SCOPED",
 /*  91 */ "xx_visibility ::= INLINE",
 /*  92 */ "xx_visibility ::= ABSTRACT",
 /*  93 */ "xx_visibility ::= FINAL",
 /*  94 */ "xx_method_return_type ::= VOID",
 /*  95 */ "xx_method_return_type ::= xx_method_return_type_list",
 /*  96 */ "xx_method_return_type_list ::= xx_method_return_type_list BITWISE_OR xx_method_return_type_item",
 /*  97 */ "xx_method_return_type_list ::= xx_method_return_type_item",
 /*  98 */ "xx_method_return_type_item ::= xx_parameter_type",
 /*  99 */ "xx_method_return_type_item ::= xx_parameter_type NOT",
 /* 100 */ "xx_method_return_type_item ::= xx_parameter_cast",
 /* 101 */ "xx_method_return_type_item ::= xx_parameter_cast_collection",
 /* 102 */ "xx_parameter_list ::= xx_parameter_list COMMA xx_parameter",
 /* 103 */ "xx_parameter_list ::= xx_parameter",
 /* 104 */ "xx_parameter ::= IDENTIFIER",
 /* 105 */ "xx_parameter ::= xx_parameter_type IDENTIFIER",
 /* 106 */ "xx_parameter ::= xx_parameter_type NOT IDENTIFIER",
 /* 107 */ "xx_parameter ::= xx_parameter_cast IDENTIFIER",
 /* 108 */ "xx_parameter ::= IDENTIFIER ASSIGN xx_literal_expr",
 /* 109 */ "xx_parameter ::= xx_parameter_type IDENTIFIER ASSIGN xx_literal_expr",
 /* 110 */ "xx_parameter ::= xx_parameter_type NOT IDENTIFIER ASSIGN xx_literal_expr",
 /* 111 */ "xx_parameter ::= xx_parameter_cast IDENTIFIER ASSIGN xx_literal_expr",
 /* 112 */ "xx_parameter_cast ::= LESS IDENTIFIER GREATER",
 /* 113 */ "xx_parameter_cast_collection ::= LESS IDENTIFIER SBRACKET_OPEN SBRACKET_CLOSE GREATER",
 /* 114 */ "xx_parameter_type ::= TYPE_INTEGER",
 /* 115 */ "xx_parameter_type ::= TYPE_UINTEGER",
 /* 116 */ "xx_parameter_type ::= TYPE_LONG",
 /* 117 */ "xx_parameter_type ::= TYPE_ULONG",
 /* 118 */ "xx_parameter_type ::= TYPE_CHAR",
 /* 119 */ "xx_parameter_type ::= TYPE_UCHAR",
 /* 120 */ "xx_parameter_type ::= TYPE_DOUBLE",
 /* 121 */ "xx_parameter_type ::= TYPE_BOOL",
 /* 122 */ "xx_parameter_type ::= TYPE_STRING",
 /* 123 */ "xx_parameter_type ::= TYPE_ARRAY",
 /* 124 */ "xx_parameter_type ::= TYPE_VAR",
 /* 125 */ "xx_statement_list ::= xx_statement_list xx_statement",
 /* 126 */ "xx_statement_list ::= xx_statement",
 /* 127 */ "xx_statement ::= xx_let_statement",
 /* 128 */ "xx_statement ::= xx_if_statement",
 /* 129 */ "xx_statement ::= xx_loop_statement",
 /* 130 */ "xx_statement ::= xx_echo_statement",
 /* 131 */ "xx_statement ::= xx_return_statement",
 /* 132 */ "xx_statement ::= xx_fetch_statement",
 /* 133 */ "xx_statement ::= xx_fcall_statement",
 /* 134 */ "xx_statement ::= xx_mcall_statement",
 /* 135 */ "xx_statement ::= xx_scall_statement",
 /* 136 */ "xx_statement ::= xx_unset_statement",
 /* 137 */ "xx_statement ::= xx_throw_statement",
 /* 138 */ "xx_statement ::= xx_declare_statement",
 /* 139 */ "xx_statement ::= xx_break_statement",
 /* 140 */ "xx_statement ::= xx_continue_statement",
 /* 141 */ "xx_statement ::= xx_while_statement",
 /* 142 */ "xx_statement ::= xx_do_while_statement",
 /* 143 */ "xx_statement ::= xx_switch_statement",
 /* 144 */ "xx_statement ::= xx_for_statement",
 /* 145 */ "xx_statement ::= xx_comment",
 /* 146 */ "xx_statement ::= xx_empty_statement",
 /* 147 */ "xx_empty_statement ::= DOTCOMMA",
 /* 148 */ "xx_break_statement ::= BREAK DOTCOMMA",
 /* 149 */ "xx_continue_statement ::= CONTINUE DOTCOMMA",
 /* 150 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN BRACKET_CLOSE",
 /* 151 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN BRACKET_CLOSE ELSE BRACKET_OPEN BRACKET_CLOSE",
 /* 152 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 153 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE ELSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 154 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE ELSE BRACKET_OPEN BRACKET_CLOSE",
 /* 155 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN BRACKET_CLOSE ELSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 156 */ "xx_switch_statement ::= SWITCH xx_eval_expr BRACKET_OPEN BRACKET_CLOSE",
 /* 157 */ "xx_switch_statement ::= SWITCH xx_eval_expr BRACKET_OPEN xx_case_clauses BRACKET_CLOSE",
 /* 158 */ "xx_case_clauses ::= xx_case_clauses xx_case_clause",
 /* 159 */ "xx_case_clauses ::= xx_case_clause",
 /* 160 */ "xx_case_clause ::= CASE xx_literal_expr COLON",
 /* 161 */ "xx_case_clause ::= CASE xx_literal_expr COLON xx_statement_list",
 /* 162 */ "xx_case_clause ::= DEFAULT COLON xx_statement_list",
 /* 163 */ "xx_loop_statement ::= LOOP BRACKET_OPEN BRACKET_CLOSE",
 /* 164 */ "xx_loop_statement ::= LOOP BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 165 */ "xx_while_statement ::= WHILE xx_eval_expr BRACKET_OPEN BRACKET_CLOSE",
 /* 166 */ "xx_while_statement ::= WHILE xx_eval_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 167 */ "xx_do_while_statement ::= DO BRACKET_OPEN BRACKET_CLOSE WHILE xx_eval_expr DOTCOMMA",
 /* 168 */ "xx_do_while_statement ::= DO BRACKET_OPEN xx_statement_list BRACKET_CLOSE WHILE xx_eval_expr DOTCOMMA",
 /* 169 */ "xx_for_statement ::= FOR IDENTIFIER IN xx_common_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 170 */ "xx_for_statement ::= FOR IDENTIFIER IN REVERSE xx_common_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 171 */ "xx_for_statement ::= FOR IDENTIFIER COMMA IDENTIFIER IN xx_common_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 172 */ "xx_for_statement ::= FOR IDENTIFIER COMMA IDENTIFIER IN REVERSE xx_common_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 173 */ "xx_let_statement ::= LET xx_let_assignments DOTCOMMA",
 /* 174 */ "xx_let_assignments ::= xx_let_assignments COMMA xx_let_assignment",
 /* 175 */ "xx_let_assignments ::= xx_let_assignment",
 /* 176 */ "xx_assignment_operator ::= ASSIGN",
 /* 177 */ "xx_assignment_operator ::= ADDASSIGN",
 /* 178 */ "xx_assignment_operator ::= SUBASSIGN",
 /* 179 */ "xx_assignment_operator ::= MULASSIGN",
 /* 180 */ "xx_assignment_operator ::= DIVASSIGN",
 /* 181 */ "xx_assignment_operator ::= CONCATASSIGN",
 /* 182 */ "xx_let_assignment ::= IDENTIFIER xx_assignment_operator xx_assign_expr",
 /* 183 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER xx_assignment_operator xx_assign_expr",
 /* 184 */ "xx_let_assignment ::= IDENTIFIER ARROW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 185 */ "xx_let_assignment ::= IDENTIFIER ARROW BRACKET_OPEN STRING BRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 186 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER SBRACKET_OPEN SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 187 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER SBRACKET_OPEN xx_index_expr SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 188 */ "xx_let_assignment ::= IDENTIFIER DOUBLECOLON IDENTIFIER xx_assignment_operator xx_assign_expr",
 /* 189 */ "xx_let_assignment ::= IDENTIFIER DOUBLECOLON IDENTIFIER SBRACKET_OPEN SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 190 */ "xx_let_assignment ::= IDENTIFIER DOUBLECOLON IDENTIFIER SBRACKET_OPEN xx_index_expr SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 191 */ "xx_let_assignment ::= IDENTIFIER SBRACKET_OPEN SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 192 */ "xx_let_assignment ::= IDENTIFIER xx_array_offset_list xx_assignment_operator xx_assign_expr",
 /* 193 */ "xx_array_offset_list ::= xx_array_offset_list xx_array_offset",
 /* 194 */ "xx_array_offset_list ::= xx_array_offset",
 /* 195 */ "xx_array_offset ::= SBRACKET_OPEN xx_index_expr SBRACKET_CLOSE",
 /* 196 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER INCR",
 /* 197 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER DECR",
 /* 198 */ "xx_let_assignment ::= IDENTIFIER INCR",
 /* 199 */ "xx_let_assignment ::= IDENTIFIER DECR",
 /* 200 */ "xx_index_expr ::= xx_common_expr",
 /* 201 */ "xx_echo_statement ::= ECHO xx_echo_expressions DOTCOMMA",
 /* 202 */ "xx_echo_expressions ::= xx_echo_expressions COMMA xx_echo_expression",
 /* 203 */ "xx_echo_expressions ::= xx_echo_expression",
 /* 204 */ "xx_echo_expression ::= xx_common_expr",
 /* 205 */ "xx_mcall_statement ::= xx_mcall_expr DOTCOMMA",
 /* 206 */ "xx_fcall_statement ::= xx_fcall_expr DOTCOMMA",
 /* 207 */ "xx_scall_statement ::= xx_scall_expr DOTCOMMA",
 /* 208 */ "xx_fetch_statement ::= xx_fetch_expr DOTCOMMA",
 /* 209 */ "xx_return_statement ::= RETURN xx_common_expr DOTCOMMA",
 /* 210 */ "xx_return_statement ::= RETURN DOTCOMMA",
 /* 211 */ "xx_unset_statement ::= UNSET IDENTIFIER ARROW IDENTIFIER DOTCOMMA",
 /* 212 */ "xx_unset_statement ::= UNSET IDENTIFIER SBRACKET_OPEN xx_common_expr SBRACKET_CLOSE DOTCOMMA",
 /* 213 */ "xx_throw_statement ::= THROW xx_common_expr DOTCOMMA",
 /* 214 */ "xx_declare_statement ::= TYPE_INTEGER xx_declare_variable_list DOTCOMMA",
 /* 215 */ "xx_declare_statement ::= TYPE_UINTEGER xx_declare_variable_list DOTCOMMA",
 /* 216 */ "xx_declare_statement ::= TYPE_CHAR xx_declare_variable_list DOTCOMMA",
 /* 217 */ "xx_declare_statement ::= TYPE_UCHAR xx_declare_variable_list DOTCOMMA",
 /* 218 */ "xx_declare_statement ::= TYPE_LONG xx_declare_variable_list DOTCOMMA",
 /* 219 */ "xx_declare_statement ::= TYPE_ULONG xx_declare_variable_list DOTCOMMA",
 /* 220 */ "xx_declare_statement ::= TYPE_DOUBLE xx_declare_variable_list DOTCOMMA",
 /* 221 */ "xx_declare_statement ::= TYPE_STRING xx_declare_variable_list DOTCOMMA",
 /* 222 */ "xx_declare_statement ::= TYPE_BOOL xx_declare_variable_list DOTCOMMA",
 /* 223 */ "xx_declare_statement ::= TYPE_VAR xx_declare_variable_list DOTCOMMA",
 /* 224 */ "xx_declare_variable_list ::= xx_declare_variable_list COMMA xx_declare_variable",
 /* 225 */ "xx_declare_variable_list ::= xx_declare_variable",
 /* 226 */ "xx_declare_variable ::= IDENTIFIER",
 /* 227 */ "xx_declare_variable ::= IDENTIFIER ASSIGN xx_literal_expr",
 /* 228 */ "xx_assign_expr ::= xx_common_expr",
 /* 229 */ "xx_common_expr ::= NOT xx_common_expr",
 /* 230 */ "xx_common_expr ::= ISSET xx_isset_expr",
 /* 231 */ "xx_common_expr ::= REQUIRE xx_common_expr",
 /* 232 */ "xx_common_expr ::= CLONE xx_common_expr",
 /* 233 */ "xx_common_expr ::= EMPTY xx_common_expr",
 /* 234 */ "xx_common_expr ::= LIKELY xx_common_expr",
 /* 235 */ "xx_common_expr ::= UNLIKELY xx_common_expr",
 /* 236 */ "xx_common_expr ::= xx_common_expr EQUALS xx_common_expr",
 /* 237 */ "xx_common_expr ::= xx_common_expr NOTEQUALS xx_common_expr",
 /* 238 */ "xx_common_expr ::= xx_common_expr IDENTICAL xx_common_expr",
 /* 239 */ "xx_common_expr ::= xx_common_expr NOTIDENTICAL xx_common_expr",
 /* 240 */ "xx_common_expr ::= xx_common_expr LESS xx_common_expr",
 /* 241 */ "xx_common_expr ::= xx_common_expr GREATER xx_common_expr",
 /* 242 */ "xx_common_expr ::= xx_common_expr LESSEQUAL xx_common_expr",
 /* 243 */ "xx_common_expr ::= xx_common_expr GREATEREQUAL xx_common_expr",
 /* 244 */ "xx_common_expr ::= PARENTHESES_OPEN xx_common_expr PARENTHESES_CLOSE",
 /* 245 */ "xx_common_expr ::= PARENTHESES_OPEN xx_parameter_type PARENTHESES_CLOSE xx_common_expr",
 /* 246 */ "xx_common_expr ::= LESS IDENTIFIER GREATER xx_common_expr",
 /* 247 */ "xx_common_expr ::= IDENTIFIER ARROW IDENTIFIER",
 /* 248 */ "xx_common_expr ::= IDENTIFIER ARROW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE",
 /* 249 */ "xx_common_expr ::= IDENTIFIER ARROW BRACKET_OPEN STRING BRACKET_CLOSE",
 /* 250 */ "xx_common_expr ::= IDENTIFIER DOUBLECOLON IDENTIFIER",
 /* 251 */ "xx_common_expr ::= IDENTIFIER DOUBLECOLON CONSTANT",
 /* 252 */ "xx_common_expr ::= xx_common_expr SBRACKET_OPEN xx_common_expr SBRACKET_CLOSE",
 /* 253 */ "xx_common_expr ::= xx_common_expr ADD xx_common_expr",
 /* 254 */ "xx_common_expr ::= xx_common_expr SUB xx_common_expr",
 /* 255 */ "xx_common_expr ::= xx_common_expr MUL xx_common_expr",
 /* 256 */ "xx_common_expr ::= xx_common_expr DIV xx_common_expr",
 /* 257 */ "xx_common_expr ::= xx_common_expr MOD xx_common_expr",
 /* 258 */ "xx_common_expr ::= xx_common_expr CONCAT xx_common_expr",
 /* 259 */ "xx_common_expr ::= xx_common_expr AND xx_common_expr",
 /* 260 */ "xx_common_expr ::= xx_common_expr OR xx_common_expr",
 /* 261 */ "xx_common_expr ::= xx_common_expr INSTANCEOF xx_common_expr",
 /* 262 */ "xx_fetch_expr ::= FETCH IDENTIFIER COMMA xx_isset_expr",
 /* 263 */ "xx_common_expr ::= xx_fetch_expr",
 /* 264 */ "xx_common_expr ::= TYPEOF xx_common_expr",
 /* 265 */ "xx_common_expr ::= IDENTIFIER",
 /* 266 */ "xx_common_expr ::= INTEGER",
 /* 267 */ "xx_common_expr ::= STRING",
 /* 268 */ "xx_common_expr ::= CHAR",
 /* 269 */ "xx_common_expr ::= DOUBLE",
 /* 270 */ "xx_common_expr ::= NULL",
 /* 271 */ "xx_common_expr ::= TRUE",
 /* 272 */ "xx_common_expr ::= FALSE",
 /* 273 */ "xx_common_expr ::= CONSTANT",
 /* 274 */ "xx_common_expr ::= SBRACKET_OPEN SBRACKET_CLOSE",
 /* 275 */ "xx_common_expr ::= SBRACKET_OPEN xx_array_list SBRACKET_CLOSE",
 /* 276 */ "xx_common_expr ::= NEW IDENTIFIER",
 /* 277 */ "xx_common_expr ::= NEW IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 278 */ "xx_common_expr ::= NEW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 279 */ "xx_common_expr ::= NEW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE",
 /* 280 */ "xx_common_expr ::= NEW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 281 */ "xx_common_expr ::= NEW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 282 */ "xx_fcall_expr ::= IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 283 */ "xx_fcall_expr ::= IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 284 */ "xx_fcall_expr ::= BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 285 */ "xx_fcall_expr ::= BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 286 */ "xx_scall_expr ::= IDENTIFIER DOUBLECOLON IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 287 */ "xx_scall_expr ::= IDENTIFIER DOUBLECOLON IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 288 */ "xx_scall_expr ::= BRACKET_OPEN IDENTIFIER BRACKET_CLOSE DOUBLECOLON IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 289 */ "xx_scall_expr ::= BRACKET_OPEN IDENTIFIER BRACKET_CLOSE DOUBLECOLON IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 290 */ "xx_scall_expr ::= BRACKET_OPEN IDENTIFIER BRACKET_CLOSE DOUBLECOLON BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 291 */ "xx_scall_expr ::= BRACKET_OPEN IDENTIFIER BRACKET_CLOSE DOUBLECOLON BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 292 */ "xx_mcall_expr ::= IDENTIFIER ARROW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 293 */ "xx_mcall_expr ::= IDENTIFIER ARROW IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 294 */ "xx_mcall_expr ::= IDENTIFIER ARROW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 295 */ "xx_mcall_expr ::= IDENTIFIER ARROW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 296 */ "xx_mcall_expr ::= IDENTIFIER ARROW BRACKET_OPEN STRING BRACKET_CLOSE PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 297 */ "xx_mcall_expr ::= IDENTIFIER ARROW BRACKET_OPEN STRING BRACKET_CLOSE PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 298 */ "xx_common_expr ::= xx_mcall_expr",
 /* 299 */ "xx_common_expr ::= xx_scall_expr",
 /* 300 */ "xx_common_expr ::= xx_fcall_expr",
 /* 301 */ "xx_call_parameters ::= xx_call_parameters COMMA xx_call_parameter",
 /* 302 */ "xx_call_parameters ::= xx_call_parameter",
 /* 303 */ "xx_call_parameter ::= xx_common_expr",
 /* 304 */ "xx_array_list ::= xx_array_list COMMA xx_array_item",
 /* 305 */ "xx_array_list ::= xx_array_item",
 /* 306 */ "xx_array_item ::= xx_array_key COLON xx_array_value",
 /* 307 */ "xx_array_item ::= xx_array_value",
 /* 308 */ "xx_array_key ::= IDENTIFIER",
 /* 309 */ "xx_array_key ::= STRING",
 /* 310 */ "xx_array_key ::= INTEGER",
 /* 311 */ "xx_array_value ::= xx_common_expr",
 /* 312 */ "xx_literal_expr ::= INTEGER",
 /* 313 */ "xx_literal_expr ::= CHAR",
 /* 314 */ "xx_literal_expr ::= STRING",
 /* 315 */ "xx_literal_expr ::= DOUBLE",
 /* 316 */ "xx_literal_expr ::= NULL",
 /* 317 */ "xx_literal_expr ::= FALSE",
 /* 318 */ "xx_literal_expr ::= TRUE",
 /* 319 */ "xx_literal_expr ::= IDENTIFIER DOUBLECOLON CONSTANT",
 /* 320 */ "xx_literal_expr ::= CONSTANT",
 /* 321 */ "xx_isset_expr ::= IDENTIFIER SBRACKET_OPEN xx_common_expr SBRACKET_CLOSE",
 /* 322 */ "xx_isset_expr ::= IDENTIFIER ARROW IDENTIFIER",
 /* 323 */ "xx_isset_expr ::= IDENTIFIER ARROW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE",
 /* 324 */ "xx_isset_expr ::= IDENTIFIER ARROW BRACKET_OPEN STRING BRACKET_CLOSE",
 /* 325 */ "xx_eval_expr ::= xx_common_expr",
 /* 326 */ "xx_comment ::= COMMENT",
};
#endif /* NDEBUG */

/*
** This function returns the symbolic name associated with a token
** value.
*/
const char *xx_TokenName(int tokenType){
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
** to xx_ and xx_Free.
*/
void *xx_Alloc(void *(*mallocProc)(size_t)){
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
// 1000 "parser.lemon"
{
	/*if ((yypminor->yy0)) {
		if ((yypminor->yy0)->free_flag) {
			efree((yypminor->yy0)->token);
		}
		efree((yypminor->yy0));
	}*/
}
// 2813 "parser.c"
      break;
    case 106:
// 1013 "parser.lemon"
{ json_object_put((yypminor->yy115)); }
// 2818 "parser.c"
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
**       obtained from xx_Alloc.
** <li>  A pointer to a function used to reclaim memory obtained
**       from malloc.
** </ul>
*/
void xx_Free(
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
     xx_ARG_FETCH;
     yypParser->yyidx--;
#ifndef NDEBUG
     if( yyTraceFILE ){
       fprintf(yyTraceFILE,"%sStack Overflow!\n",yyTracePrompt);
     }
#endif
     while( yypParser->yyidx>=0 ) yy_pop_parser_stack(yypParser);
     /* Here code is inserted which will execute if the parser
     ** stack every overflows */
     xx_ARG_STORE; /* Suppress warning about unused %extra_argument var */
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
  { 105, 1 },
  { 106, 1 },
  { 107, 2 },
  { 107, 1 },
  { 108, 1 },
  { 108, 1 },
  { 108, 1 },
  { 108, 1 },
  { 109, 3 },
  { 111, 3 },
  { 111, 5 },
  { 110, 3 },
  { 110, 5 },
  { 110, 5 },
  { 110, 7 },
  { 110, 4 },
  { 110, 6 },
  { 110, 6 },
  { 110, 4 },
  { 110, 6 },
  { 114, 2 },
  { 114, 3 },
  { 115, 3 },
  { 115, 1 },
  { 117, 1 },
  { 113, 2 },
  { 113, 3 },
  { 116, 1 },
  { 116, 1 },
  { 116, 1 },
  { 116, 2 },
  { 116, 2 },
  { 116, 2 },
  { 116, 2 },
  { 116, 3 },
  { 116, 3 },
  { 119, 2 },
  { 119, 1 },
  { 122, 4 },
  { 122, 3 },
  { 122, 6 },
  { 122, 5 },
  { 122, 5 },
  { 122, 4 },
  { 122, 7 },
  { 122, 6 },
  { 125, 2 },
  { 125, 3 },
  { 126, 3 },
  { 126, 1 },
  { 127, 1 },
  { 127, 2 },
  { 120, 2 },
  { 120, 1 },
  { 121, 2 },
  { 121, 1 },
  { 118, 2 },
  { 118, 1 },
  { 128, 6 },
  { 128, 5 },
  { 129, 7 },
  { 129, 8 },
  { 129, 8 },
  { 129, 9 },
  { 129, 8 },
  { 129, 9 },
  { 129, 9 },
  { 129, 10 },
  { 129, 9 },
  { 129, 10 },
  { 129, 10 },
  { 129, 11 },
  { 129, 10 },
  { 129, 11 },
  { 129, 11 },
  { 129, 12 },
  { 130, 8 },
  { 130, 9 },
  { 130, 9 },
  { 130, 10 },
  { 130, 6 },
  { 130, 7 },
  { 130, 7 },
  { 130, 8 },
  { 123, 2 },
  { 123, 1 },
  { 134, 1 },
  { 134, 1 },
  { 134, 1 },
  { 134, 1 },
  { 134, 1 },
  { 134, 1 },
  { 134, 1 },
  { 134, 1 },
  { 133, 1 },
  { 133, 1 },
  { 135, 3 },
  { 135, 1 },
  { 136, 1 },
  { 136, 2 },
  { 136, 1 },
  { 136, 1 },
  { 131, 3 },
  { 131, 1 },
  { 140, 1 },
  { 140, 2 },
  { 140, 3 },
  { 140, 2 },
  { 140, 3 },
  { 140, 4 },
  { 140, 5 },
  { 140, 4 },
  { 138, 3 },
  { 139, 5 },
  { 137, 1 },
  { 137, 1 },
  { 137, 1 },
  { 137, 1 },
  { 137, 1 },
  { 137, 1 },
  { 137, 1 },
  { 137, 1 },
  { 137, 1 },
  { 137, 1 },
  { 137, 1 },
  { 132, 2 },
  { 132, 1 },
  { 141, 1 },
  { 141, 1 },
  { 141, 1 },
  { 141, 1 },
  { 141, 1 },
  { 141, 1 },
  { 141, 1 },
  { 141, 1 },
  { 141, 1 },
  { 141, 1 },
  { 141, 1 },
  { 141, 1 },
  { 141, 1 },
  { 141, 1 },
  { 141, 1 },
  { 141, 1 },
  { 141, 1 },
  { 141, 1 },
  { 141, 1 },
  { 141, 1 },
  { 160, 1 },
  { 154, 2 },
  { 155, 2 },
  { 143, 4 },
  { 143, 7 },
  { 143, 5 },
  { 143, 9 },
  { 143, 8 },
  { 143, 8 },
  { 158, 4 },
  { 158, 5 },
  { 162, 2 },
  { 162, 1 },
  { 163, 3 },
  { 163, 4 },
  { 163, 3 },
  { 144, 3 },
  { 144, 4 },
  { 156, 4 },
  { 156, 5 },
  { 157, 6 },
  { 157, 7 },
  { 159, 7 },
  { 159, 8 },
  { 159, 9 },
  { 159, 10 },
  { 142, 3 },
  { 165, 3 },
  { 165, 1 },
  { 167, 1 },
  { 167, 1 },
  { 167, 1 },
  { 167, 1 },
  { 167, 1 },
  { 167, 1 },
  { 166, 3 },
  { 166, 5 },
  { 166, 7 },
  { 166, 7 },
  { 166, 7 },
  { 166, 8 },
  { 166, 5 },
  { 166, 7 },
  { 166, 8 },
  { 166, 5 },
  { 166, 4 },
  { 170, 2 },
  { 170, 1 },
  { 171, 3 },
  { 166, 4 },
  { 166, 4 },
  { 166, 2 },
  { 166, 2 },
  { 169, 1 },
  { 145, 3 },
  { 172, 3 },
  { 172, 1 },
  { 173, 1 },
  { 149, 2 },
  { 148, 2 },
  { 150, 2 },
  { 147, 2 },
  { 146, 3 },
  { 146, 2 },
  { 151, 5 },
  { 151, 6 },
  { 152, 3 },
  { 153, 3 },
  { 153, 3 },
  { 153, 3 },
  { 153, 3 },
  { 153, 3 },
  { 153, 3 },
  { 153, 3 },
  { 153, 3 },
  { 153, 3 },
  { 153, 3 },
  { 178, 3 },
  { 178, 1 },
  { 179, 1 },
  { 179, 3 },
  { 168, 1 },
  { 164, 2 },
  { 164, 2 },
  { 164, 2 },
  { 164, 2 },
  { 164, 2 },
  { 164, 2 },
  { 164, 2 },
  { 164, 3 },
  { 164, 3 },
  { 164, 3 },
  { 164, 3 },
  { 164, 3 },
  { 164, 3 },
  { 164, 3 },
  { 164, 3 },
  { 164, 3 },
  { 164, 4 },
  { 164, 4 },
  { 164, 3 },
  { 164, 5 },
  { 164, 5 },
  { 164, 3 },
  { 164, 3 },
  { 164, 4 },
  { 164, 3 },
  { 164, 3 },
  { 164, 3 },
  { 164, 3 },
  { 164, 3 },
  { 164, 3 },
  { 164, 3 },
  { 164, 3 },
  { 164, 3 },
  { 177, 4 },
  { 164, 1 },
  { 164, 2 },
  { 164, 1 },
  { 164, 1 },
  { 164, 1 },
  { 164, 1 },
  { 164, 1 },
  { 164, 1 },
  { 164, 1 },
  { 164, 1 },
  { 164, 1 },
  { 164, 2 },
  { 164, 3 },
  { 164, 2 },
  { 164, 4 },
  { 164, 5 },
  { 164, 4 },
  { 164, 6 },
  { 164, 7 },
  { 175, 4 },
  { 175, 3 },
  { 175, 6 },
  { 175, 5 },
  { 176, 6 },
  { 176, 5 },
  { 176, 8 },
  { 176, 7 },
  { 176, 10 },
  { 176, 9 },
  { 174, 6 },
  { 174, 5 },
  { 174, 8 },
  { 174, 7 },
  { 174, 8 },
  { 174, 7 },
  { 164, 1 },
  { 164, 1 },
  { 164, 1 },
  { 182, 3 },
  { 182, 1 },
  { 183, 1 },
  { 181, 3 },
  { 181, 1 },
  { 184, 3 },
  { 184, 1 },
  { 185, 1 },
  { 185, 1 },
  { 185, 1 },
  { 186, 1 },
  { 124, 1 },
  { 124, 1 },
  { 124, 1 },
  { 124, 1 },
  { 124, 1 },
  { 124, 1 },
  { 124, 1 },
  { 124, 3 },
  { 124, 1 },
  { 180, 4 },
  { 180, 3 },
  { 180, 5 },
  { 180, 5 },
  { 161, 1 },
  { 112, 1 },
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
  xx_ARG_FETCH;
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
// 1009 "parser.lemon"
{
	status->ret = yymsp[0].minor.yy115;
}
// 3362 "parser.c"
        break;
      case 1:
      case 4:
      case 5:
      case 6:
      case 7:
      case 127:
      case 128:
      case 129:
      case 130:
      case 131:
      case 132:
      case 133:
      case 134:
      case 135:
      case 136:
      case 137:
      case 138:
      case 139:
      case 140:
      case 141:
      case 142:
      case 143:
      case 144:
      case 145:
      case 146:
      case 200:
      case 228:
      case 263:
      case 298:
      case 299:
      case 300:
      case 303:
      case 311:
      case 325:
// 1015 "parser.lemon"
{
	yygotominor.yy115 = yymsp[0].minor.yy115;
}
// 3402 "parser.c"
        break;
      case 2:
      case 36:
      case 52:
      case 54:
      case 56:
      case 84:
      case 125:
      case 158:
      case 193:
// 1019 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_list(yymsp[-1].minor.yy115, yymsp[0].minor.yy115);
}
// 3417 "parser.c"
        break;
      case 3:
      case 23:
      case 37:
      case 49:
      case 53:
      case 55:
      case 57:
      case 85:
      case 97:
      case 103:
      case 126:
      case 159:
      case 175:
      case 194:
      case 203:
      case 225:
      case 302:
      case 305:
// 1023 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_list(NULL, yymsp[0].minor.yy115);
}
// 3441 "parser.c"
        break;
      case 8:
// 1043 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_namespace(yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(40,&yymsp[0].minor);
}
// 3450 "parser.c"
        break;
      case 9:
// 1047 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_interface(yymsp[-1].minor.yy0, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(41,&yymsp[-2].minor);
}
// 3458 "parser.c"
        break;
      case 10:
// 1051 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_interface(yymsp[-3].minor.yy0, yymsp[0].minor.yy115, yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(41,&yymsp[-4].minor);
  yy_destructor(42,&yymsp[-2].minor);
}
// 3467 "parser.c"
        break;
      case 11:
// 1055 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class(yymsp[-1].minor.yy0, yymsp[0].minor.yy115, 0, 0, NULL, NULL, status->scanner_state);
  yy_destructor(43,&yymsp[-2].minor);
}
// 3475 "parser.c"
        break;
      case 12:
// 1059 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy115, 0, 0, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(43,&yymsp[-4].minor);
  yy_destructor(42,&yymsp[-2].minor);
}
// 3484 "parser.c"
        break;
      case 13:
// 1063 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy115, 0, 0, NULL, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(43,&yymsp[-4].minor);
  yy_destructor(44,&yymsp[-2].minor);
}
// 3493 "parser.c"
        break;
      case 14:
// 1067 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class(yymsp[-5].minor.yy0, yymsp[0].minor.yy115, 0, 0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(43,&yymsp[-6].minor);
  yy_destructor(42,&yymsp[-4].minor);
  yy_destructor(44,&yymsp[-2].minor);
}
// 3503 "parser.c"
        break;
      case 15:
// 1071 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class(yymsp[-1].minor.yy0, yymsp[0].minor.yy115, 1, 0, NULL, NULL, status->scanner_state);
  yy_destructor(45,&yymsp[-3].minor);
  yy_destructor(43,&yymsp[-2].minor);
}
// 3512 "parser.c"
        break;
      case 16:
// 1075 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy115, 1, 0, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(45,&yymsp[-5].minor);
  yy_destructor(43,&yymsp[-4].minor);
  yy_destructor(42,&yymsp[-2].minor);
}
// 3522 "parser.c"
        break;
      case 17:
// 1079 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy115, 1, 0, NULL, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(45,&yymsp[-5].minor);
  yy_destructor(43,&yymsp[-4].minor);
  yy_destructor(44,&yymsp[-2].minor);
}
// 3532 "parser.c"
        break;
      case 18:
// 1083 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class(yymsp[-1].minor.yy0, yymsp[0].minor.yy115, 0, 1, NULL, NULL, status->scanner_state);
  yy_destructor(46,&yymsp[-3].minor);
  yy_destructor(43,&yymsp[-2].minor);
}
// 3541 "parser.c"
        break;
      case 19:
// 1087 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy115, 0, 1, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(46,&yymsp[-5].minor);
  yy_destructor(43,&yymsp[-4].minor);
  yy_destructor(42,&yymsp[-2].minor);
}
// 3551 "parser.c"
        break;
      case 20:
      case 25:
      case 46:
// 1091 "parser.lemon"
{
	yygotominor.yy115 = NULL;
  yy_destructor(47,&yymsp[-1].minor);
  yy_destructor(48,&yymsp[0].minor);
}
// 3562 "parser.c"
        break;
      case 21:
      case 26:
      case 47:
// 1095 "parser.lemon"
{
	yygotominor.yy115 = yymsp[-1].minor.yy115;
  yy_destructor(47,&yymsp[-2].minor);
  yy_destructor(48,&yymsp[0].minor);
}
// 3573 "parser.c"
        break;
      case 22:
      case 48:
      case 102:
      case 174:
      case 202:
      case 224:
      case 301:
      case 304:
// 1099 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_list(yymsp[-2].minor.yy115, yymsp[0].minor.yy115);
  yy_destructor(6,&yymsp[-1].minor);
}
// 3588 "parser.c"
        break;
      case 24:
      case 265:
      case 308:
// 1107 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state);
}
// 3597 "parser.c"
        break;
      case 27:
// 1119 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_definition(yymsp[0].minor.yy115, NULL, NULL, status->scanner_state);
}
// 3604 "parser.c"
        break;
      case 28:
// 1123 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_definition(NULL, NULL, yymsp[0].minor.yy115, status->scanner_state);
}
// 3611 "parser.c"
        break;
      case 29:
// 1127 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_definition(NULL, yymsp[0].minor.yy115, NULL, status->scanner_state);
}
// 3618 "parser.c"
        break;
      case 30:
// 1131 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_definition(yymsp[-1].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
}
// 3625 "parser.c"
        break;
      case 31:
// 1135 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_definition(yymsp[-1].minor.yy115, NULL, yymsp[0].minor.yy115, status->scanner_state);
}
// 3632 "parser.c"
        break;
      case 32:
// 1139 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_definition(yymsp[0].minor.yy115, NULL, yymsp[-1].minor.yy115, status->scanner_state);
}
// 3639 "parser.c"
        break;
      case 33:
// 1143 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_definition(NULL, yymsp[0].minor.yy115, yymsp[-1].minor.yy115, status->scanner_state);
}
// 3646 "parser.c"
        break;
      case 34:
// 1147 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_definition(yymsp[-2].minor.yy115, yymsp[0].minor.yy115, yymsp[-1].minor.yy115, status->scanner_state);
}
// 3653 "parser.c"
        break;
      case 35:
// 1151 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_definition(yymsp[-1].minor.yy115, yymsp[0].minor.yy115, yymsp[-2].minor.yy115, status->scanner_state);
}
// 3660 "parser.c"
        break;
      case 38:
// 1168 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_property(yymsp[-2].minor.yy115, yymsp[-1].minor.yy0, NULL, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(40,&yymsp[0].minor);
}
// 3668 "parser.c"
        break;
      case 39:
// 1172 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_property(yymsp[-2].minor.yy115, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(40,&yymsp[0].minor);
}
// 3676 "parser.c"
        break;
      case 40:
// 1176 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_property(yymsp[-4].minor.yy115, yymsp[-3].minor.yy0, yymsp[-1].minor.yy115, yymsp[-5].minor.yy0, NULL, status->scanner_state);
  yy_destructor(50,&yymsp[-2].minor);
  yy_destructor(40,&yymsp[0].minor);
}
// 3685 "parser.c"
        break;
      case 41:
// 1180 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_property(yymsp[-4].minor.yy115, yymsp[-3].minor.yy0, yymsp[-1].minor.yy115, NULL, NULL, status->scanner_state);
  yy_destructor(50,&yymsp[-2].minor);
  yy_destructor(40,&yymsp[0].minor);
}
// 3694 "parser.c"
        break;
      case 42:
// 1184 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_property(yymsp[-3].minor.yy115, yymsp[-2].minor.yy0, NULL, yymsp[-4].minor.yy0, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(40,&yymsp[0].minor);
}
// 3702 "parser.c"
        break;
      case 43:
// 1188 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_property(yymsp[-3].minor.yy115, yymsp[-2].minor.yy0, NULL, NULL, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(40,&yymsp[0].minor);
}
// 3710 "parser.c"
        break;
      case 44:
// 1192 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_property(yymsp[-5].minor.yy115, yymsp[-4].minor.yy0, yymsp[-2].minor.yy115, yymsp[-6].minor.yy0, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(50,&yymsp[-3].minor);
  yy_destructor(40,&yymsp[0].minor);
}
// 3719 "parser.c"
        break;
      case 45:
// 1196 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_property(yymsp[-5].minor.yy115, yymsp[-4].minor.yy0, yymsp[-2].minor.yy115, NULL, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(50,&yymsp[-3].minor);
  yy_destructor(40,&yymsp[0].minor);
}
// 3728 "parser.c"
        break;
      case 50:
// 1216 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_property_shortcut(NULL, yymsp[0].minor.yy0, status->scanner_state);
}
// 3735 "parser.c"
        break;
      case 51:
// 1220 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_property_shortcut(yymsp[-1].minor.yy0, yymsp[0].minor.yy0, status->scanner_state);
}
// 3742 "parser.c"
        break;
      case 58:
// 1249 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy115, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(51,&yymsp[-4].minor);
  yy_destructor(50,&yymsp[-2].minor);
  yy_destructor(40,&yymsp[0].minor);
}
// 3752 "parser.c"
        break;
      case 59:
// 1253 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy115, NULL, status->scanner_state);
  yy_destructor(51,&yymsp[-4].minor);
  yy_destructor(50,&yymsp[-2].minor);
  yy_destructor(40,&yymsp[0].minor);
}
// 3762 "parser.c"
        break;
      case 60:
// 1258 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_method(yymsp[-6].minor.yy115, yymsp[-4].minor.yy0, NULL, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(53,&yymsp[-5].minor);
  yy_destructor(54,&yymsp[-3].minor);
  yy_destructor(36,&yymsp[-2].minor);
  yy_destructor(47,&yymsp[-1].minor);
  yy_destructor(48,&yymsp[0].minor);
}
// 3774 "parser.c"
        break;
      case 61:
// 1262 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_method(yymsp[-7].minor.yy115, yymsp[-5].minor.yy0, yymsp[-3].minor.yy115, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(53,&yymsp[-6].minor);
  yy_destructor(54,&yymsp[-4].minor);
  yy_destructor(36,&yymsp[-2].minor);
  yy_destructor(47,&yymsp[-1].minor);
  yy_destructor(48,&yymsp[0].minor);
}
// 3786 "parser.c"
        break;
      case 62:
// 1266 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_method(yymsp[-7].minor.yy115, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy115, NULL, NULL, status->scanner_state);
  yy_destructor(53,&yymsp[-6].minor);
  yy_destructor(54,&yymsp[-4].minor);
  yy_destructor(36,&yymsp[-3].minor);
  yy_destructor(47,&yymsp[-2].minor);
  yy_destructor(48,&yymsp[0].minor);
}
// 3798 "parser.c"
        break;
      case 63:
// 1270 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_method(yymsp[-8].minor.yy115, yymsp[-6].minor.yy0, yymsp[-4].minor.yy115, yymsp[-1].minor.yy115, NULL, NULL, status->scanner_state);
  yy_destructor(53,&yymsp[-7].minor);
  yy_destructor(54,&yymsp[-5].minor);
  yy_destructor(36,&yymsp[-3].minor);
  yy_destructor(47,&yymsp[-2].minor);
  yy_destructor(48,&yymsp[0].minor);
}
// 3810 "parser.c"
        break;
      case 64:
// 1274 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_method(yymsp[-6].minor.yy115, yymsp[-4].minor.yy0, NULL, NULL, yymsp[-7].minor.yy0, NULL, status->scanner_state);
  yy_destructor(53,&yymsp[-5].minor);
  yy_destructor(54,&yymsp[-3].minor);
  yy_destructor(36,&yymsp[-2].minor);
  yy_destructor(47,&yymsp[-1].minor);
  yy_destructor(48,&yymsp[0].minor);
}
// 3822 "parser.c"
        break;
      case 65:
// 1278 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_method(yymsp[-7].minor.yy115, yymsp[-5].minor.yy0, yymsp[-3].minor.yy115, NULL, yymsp[-8].minor.yy0, NULL, status->scanner_state);
  yy_destructor(53,&yymsp[-6].minor);
  yy_destructor(54,&yymsp[-4].minor);
  yy_destructor(36,&yymsp[-2].minor);
  yy_destructor(47,&yymsp[-1].minor);
  yy_destructor(48,&yymsp[0].minor);
}
// 3834 "parser.c"
        break;
      case 66:
// 1282 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_method(yymsp[-7].minor.yy115, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy115, yymsp[-8].minor.yy0, NULL, status->scanner_state);
  yy_destructor(53,&yymsp[-6].minor);
  yy_destructor(54,&yymsp[-4].minor);
  yy_destructor(36,&yymsp[-3].minor);
  yy_destructor(47,&yymsp[-2].minor);
  yy_destructor(48,&yymsp[0].minor);
}
// 3846 "parser.c"
        break;
      case 67:
// 1286 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_method(yymsp[-8].minor.yy115, yymsp[-6].minor.yy0, yymsp[-4].minor.yy115, yymsp[-1].minor.yy115, yymsp[-9].minor.yy0, NULL, status->scanner_state);
  yy_destructor(53,&yymsp[-7].minor);
  yy_destructor(54,&yymsp[-5].minor);
  yy_destructor(36,&yymsp[-3].minor);
  yy_destructor(47,&yymsp[-2].minor);
  yy_destructor(48,&yymsp[0].minor);
}
// 3858 "parser.c"
        break;
      case 68:
// 1290 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_method(yymsp[-8].minor.yy115, yymsp[-6].minor.yy0, NULL, NULL, NULL, yymsp[-2].minor.yy115, status->scanner_state);
  yy_destructor(53,&yymsp[-7].minor);
  yy_destructor(54,&yymsp[-5].minor);
  yy_destructor(36,&yymsp[-4].minor);
  yy_destructor(55,&yymsp[-3].minor);
  yy_destructor(47,&yymsp[-1].minor);
  yy_destructor(48,&yymsp[0].minor);
}
// 3871 "parser.c"
        break;
      case 69:
// 1294 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_method(yymsp[-9].minor.yy115, yymsp[-7].minor.yy0, yymsp[-5].minor.yy115, NULL, NULL, yymsp[-2].minor.yy115, status->scanner_state);
  yy_destructor(53,&yymsp[-8].minor);
  yy_destructor(54,&yymsp[-6].minor);
  yy_destructor(36,&yymsp[-4].minor);
  yy_destructor(55,&yymsp[-3].minor);
  yy_destructor(47,&yymsp[-1].minor);
  yy_destructor(48,&yymsp[0].minor);
}
// 3884 "parser.c"
        break;
      case 70:
// 1298 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_method(yymsp[-9].minor.yy115, yymsp[-7].minor.yy0, NULL, yymsp[-1].minor.yy115, NULL, yymsp[-3].minor.yy115, status->scanner_state);
  yy_destructor(53,&yymsp[-8].minor);
  yy_destructor(54,&yymsp[-6].minor);
  yy_destructor(36,&yymsp[-5].minor);
  yy_destructor(55,&yymsp[-4].minor);
  yy_destructor(47,&yymsp[-2].minor);
  yy_destructor(48,&yymsp[0].minor);
}
// 3897 "parser.c"
        break;
      case 71:
// 1302 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_method(yymsp[-10].minor.yy115, yymsp[-8].minor.yy0, yymsp[-6].minor.yy115, yymsp[-1].minor.yy115, NULL, yymsp[-3].minor.yy115, status->scanner_state);
  yy_destructor(53,&yymsp[-9].minor);
  yy_destructor(54,&yymsp[-7].minor);
  yy_destructor(36,&yymsp[-5].minor);
  yy_destructor(55,&yymsp[-4].minor);
  yy_destructor(47,&yymsp[-2].minor);
  yy_destructor(48,&yymsp[0].minor);
}
// 3910 "parser.c"
        break;
      case 72:
// 1306 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_method(yymsp[-8].minor.yy115, yymsp[-6].minor.yy0, NULL, NULL, yymsp[-9].minor.yy0, yymsp[-2].minor.yy115, status->scanner_state);
  yy_destructor(53,&yymsp[-7].minor);
  yy_destructor(54,&yymsp[-5].minor);
  yy_destructor(36,&yymsp[-4].minor);
  yy_destructor(55,&yymsp[-3].minor);
  yy_destructor(47,&yymsp[-1].minor);
  yy_destructor(48,&yymsp[0].minor);
}
// 3923 "parser.c"
        break;
      case 73:
// 1310 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_method(yymsp[-9].minor.yy115, yymsp[-7].minor.yy0, yymsp[-5].minor.yy115, NULL, yymsp[-10].minor.yy0, yymsp[-2].minor.yy115, status->scanner_state);
  yy_destructor(53,&yymsp[-8].minor);
  yy_destructor(54,&yymsp[-6].minor);
  yy_destructor(36,&yymsp[-4].minor);
  yy_destructor(55,&yymsp[-3].minor);
  yy_destructor(47,&yymsp[-1].minor);
  yy_destructor(48,&yymsp[0].minor);
}
// 3936 "parser.c"
        break;
      case 74:
// 1314 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_method(yymsp[-9].minor.yy115, yymsp[-7].minor.yy0, NULL, yymsp[-1].minor.yy115, yymsp[-10].minor.yy0, yymsp[-3].minor.yy115, status->scanner_state);
  yy_destructor(53,&yymsp[-8].minor);
  yy_destructor(54,&yymsp[-6].minor);
  yy_destructor(36,&yymsp[-5].minor);
  yy_destructor(55,&yymsp[-4].minor);
  yy_destructor(47,&yymsp[-2].minor);
  yy_destructor(48,&yymsp[0].minor);
}
// 3949 "parser.c"
        break;
      case 75:
// 1318 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_method(yymsp[-10].minor.yy115, yymsp[-8].minor.yy0, yymsp[-6].minor.yy115, yymsp[-1].minor.yy115, yymsp[-11].minor.yy0, yymsp[-3].minor.yy115, status->scanner_state);
  yy_destructor(53,&yymsp[-9].minor);
  yy_destructor(54,&yymsp[-7].minor);
  yy_destructor(36,&yymsp[-5].minor);
  yy_destructor(55,&yymsp[-4].minor);
  yy_destructor(47,&yymsp[-2].minor);
  yy_destructor(48,&yymsp[0].minor);
}
// 3962 "parser.c"
        break;
      case 76:
// 1323 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_method(yymsp[-7].minor.yy115, yymsp[-5].minor.yy0, NULL, NULL, NULL, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(53,&yymsp[-6].minor);
  yy_destructor(54,&yymsp[-4].minor);
  yy_destructor(36,&yymsp[-3].minor);
  yy_destructor(55,&yymsp[-2].minor);
  yy_destructor(40,&yymsp[0].minor);
}
// 3974 "parser.c"
        break;
      case 77:
// 1327 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_method(yymsp[-8].minor.yy115, yymsp[-6].minor.yy0, yymsp[-4].minor.yy115, NULL, NULL, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(53,&yymsp[-7].minor);
  yy_destructor(54,&yymsp[-5].minor);
  yy_destructor(36,&yymsp[-3].minor);
  yy_destructor(55,&yymsp[-2].minor);
  yy_destructor(40,&yymsp[0].minor);
}
// 3986 "parser.c"
        break;
      case 78:
// 1331 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_method(yymsp[-7].minor.yy115, yymsp[-5].minor.yy0, NULL, NULL, yymsp[-8].minor.yy0, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(53,&yymsp[-6].minor);
  yy_destructor(54,&yymsp[-4].minor);
  yy_destructor(36,&yymsp[-3].minor);
  yy_destructor(55,&yymsp[-2].minor);
  yy_destructor(40,&yymsp[0].minor);
}
// 3998 "parser.c"
        break;
      case 79:
// 1335 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_method(yymsp[-8].minor.yy115, yymsp[-6].minor.yy0, yymsp[-4].minor.yy115, NULL, yymsp[-9].minor.yy0, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(53,&yymsp[-7].minor);
  yy_destructor(54,&yymsp[-5].minor);
  yy_destructor(36,&yymsp[-3].minor);
  yy_destructor(55,&yymsp[-2].minor);
  yy_destructor(40,&yymsp[0].minor);
}
// 4010 "parser.c"
        break;
      case 80:
// 1339 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_method(yymsp[-5].minor.yy115, yymsp[-3].minor.yy0, NULL, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(53,&yymsp[-4].minor);
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[-1].minor);
  yy_destructor(40,&yymsp[0].minor);
}
// 4021 "parser.c"
        break;
      case 81:
// 1343 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_method(yymsp[-6].minor.yy115, yymsp[-4].minor.yy0, yymsp[-2].minor.yy115, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(53,&yymsp[-5].minor);
  yy_destructor(54,&yymsp[-3].minor);
  yy_destructor(36,&yymsp[-1].minor);
  yy_destructor(40,&yymsp[0].minor);
}
// 4032 "parser.c"
        break;
      case 82:
// 1347 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_method(yymsp[-5].minor.yy115, yymsp[-3].minor.yy0, NULL, NULL, yymsp[-6].minor.yy0, NULL, status->scanner_state);
  yy_destructor(53,&yymsp[-4].minor);
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[-1].minor);
  yy_destructor(40,&yymsp[0].minor);
}
// 4043 "parser.c"
        break;
      case 83:
// 1351 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_method(yymsp[-6].minor.yy115, yymsp[-4].minor.yy0, yymsp[-2].minor.yy115, NULL, yymsp[-7].minor.yy0, NULL, status->scanner_state);
  yy_destructor(53,&yymsp[-5].minor);
  yy_destructor(54,&yymsp[-3].minor);
  yy_destructor(36,&yymsp[-1].minor);
  yy_destructor(40,&yymsp[0].minor);
}
// 4054 "parser.c"
        break;
      case 86:
// 1364 "parser.lemon"
{
	yygotominor.yy115 = json_object_new_string("public");
  yy_destructor(1,&yymsp[0].minor);
}
// 4062 "parser.c"
        break;
      case 87:
// 1368 "parser.lemon"
{
	yygotominor.yy115 = json_object_new_string("protected");
  yy_destructor(2,&yymsp[0].minor);
}
// 4070 "parser.c"
        break;
      case 88:
// 1372 "parser.lemon"
{
	yygotominor.yy115 = json_object_new_string("private");
  yy_destructor(4,&yymsp[0].minor);
}
// 4078 "parser.c"
        break;
      case 89:
// 1376 "parser.lemon"
{
	yygotominor.yy115 = json_object_new_string("static");
  yy_destructor(3,&yymsp[0].minor);
}
// 4086 "parser.c"
        break;
      case 90:
// 1380 "parser.lemon"
{
	yygotominor.yy115 = json_object_new_string("scoped");
  yy_destructor(5,&yymsp[0].minor);
}
// 4094 "parser.c"
        break;
      case 91:
// 1384 "parser.lemon"
{
	yygotominor.yy115 = json_object_new_string("inline");
  yy_destructor(56,&yymsp[0].minor);
}
// 4102 "parser.c"
        break;
      case 92:
// 1388 "parser.lemon"
{
	yygotominor.yy115 = json_object_new_string("abstract");
  yy_destructor(45,&yymsp[0].minor);
}
// 4110 "parser.c"
        break;
      case 93:
// 1392 "parser.lemon"
{
	yygotominor.yy115 = json_object_new_string("final");
  yy_destructor(46,&yymsp[0].minor);
}
// 4118 "parser.c"
        break;
      case 94:
// 1397 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_return_type(1, NULL, status->scanner_state);
  yy_destructor(57,&yymsp[0].minor);
}
// 4126 "parser.c"
        break;
      case 95:
// 1401 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_return_type(0, yymsp[0].minor.yy115, status->scanner_state);
}
// 4133 "parser.c"
        break;
      case 96:
// 1405 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_list(yymsp[-2].minor.yy115, yymsp[0].minor.yy115);
  yy_destructor(15,&yymsp[-1].minor);
}
// 4141 "parser.c"
        break;
      case 98:
// 1413 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_return_type_item(yymsp[0].minor.yy115, NULL, 0, 0, status->scanner_state);
}
// 4148 "parser.c"
        break;
      case 99:
// 1417 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_return_type_item(yymsp[-1].minor.yy115, NULL, 1, 0, status->scanner_state);
  yy_destructor(35,&yymsp[0].minor);
}
// 4156 "parser.c"
        break;
      case 100:
// 1421 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_return_type_item(NULL, yymsp[0].minor.yy115, 0, 0, status->scanner_state);
}
// 4163 "parser.c"
        break;
      case 101:
// 1425 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_return_type_item(NULL, yymsp[0].minor.yy115, 0, 1, status->scanner_state);
}
// 4170 "parser.c"
        break;
      case 104:
// 1438 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_parameter(NULL, NULL, yymsp[0].minor.yy0, NULL, 0, status->scanner_state);
}
// 4177 "parser.c"
        break;
      case 105:
// 1442 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_parameter(yymsp[-1].minor.yy115, NULL, yymsp[0].minor.yy0, NULL, 0, status->scanner_state);
}
// 4184 "parser.c"
        break;
      case 106:
// 1446 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_parameter(yymsp[-2].minor.yy115, NULL, yymsp[0].minor.yy0, NULL, 1, status->scanner_state);
  yy_destructor(35,&yymsp[-1].minor);
}
// 4192 "parser.c"
        break;
      case 107:
// 1450 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_parameter(NULL, yymsp[-1].minor.yy115, yymsp[0].minor.yy0, NULL, 0, status->scanner_state);
}
// 4199 "parser.c"
        break;
      case 108:
// 1454 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_parameter(NULL, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy115, 0, status->scanner_state);
  yy_destructor(50,&yymsp[-1].minor);
}
// 4207 "parser.c"
        break;
      case 109:
// 1458 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_parameter(yymsp[-3].minor.yy115, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy115, 0, status->scanner_state);
  yy_destructor(50,&yymsp[-1].minor);
}
// 4215 "parser.c"
        break;
      case 110:
// 1462 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_parameter(yymsp[-4].minor.yy115, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy115, 1, status->scanner_state);
  yy_destructor(35,&yymsp[-3].minor);
  yy_destructor(50,&yymsp[-1].minor);
}
// 4224 "parser.c"
        break;
      case 111:
// 1466 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_parameter(NULL, yymsp[-3].minor.yy115, yymsp[-2].minor.yy0, yymsp[0].minor.yy115, 0, status->scanner_state);
  yy_destructor(50,&yymsp[-1].minor);
}
// 4232 "parser.c"
        break;
      case 112:
// 1470 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(19,&yymsp[-2].minor);
  yy_destructor(20,&yymsp[0].minor);
}
// 4241 "parser.c"
        break;
      case 113:
// 1474 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(19,&yymsp[-4].minor);
  yy_destructor(37,&yymsp[-2].minor);
  yy_destructor(58,&yymsp[-1].minor);
  yy_destructor(20,&yymsp[0].minor);
}
// 4252 "parser.c"
        break;
      case 114:
// 1478 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_type(XX_TYPE_INTEGER);
  yy_destructor(59,&yymsp[0].minor);
}
// 4260 "parser.c"
        break;
      case 115:
// 1482 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_type(XX_TYPE_UINTEGER);
  yy_destructor(60,&yymsp[0].minor);
}
// 4268 "parser.c"
        break;
      case 116:
// 1486 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_type(XX_TYPE_LONG);
  yy_destructor(61,&yymsp[0].minor);
}
// 4276 "parser.c"
        break;
      case 117:
// 1490 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_type(XX_TYPE_ULONG);
  yy_destructor(62,&yymsp[0].minor);
}
// 4284 "parser.c"
        break;
      case 118:
// 1494 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_type(XX_TYPE_CHAR);
  yy_destructor(63,&yymsp[0].minor);
}
// 4292 "parser.c"
        break;
      case 119:
// 1498 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_type(XX_TYPE_UCHAR);
  yy_destructor(64,&yymsp[0].minor);
}
// 4300 "parser.c"
        break;
      case 120:
// 1502 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_type(XX_TYPE_DOUBLE);
  yy_destructor(65,&yymsp[0].minor);
}
// 4308 "parser.c"
        break;
      case 121:
// 1506 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_type(XX_TYPE_BOOL);
  yy_destructor(66,&yymsp[0].minor);
}
// 4316 "parser.c"
        break;
      case 122:
// 1510 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_type(XX_TYPE_STRING);
  yy_destructor(67,&yymsp[0].minor);
}
// 4324 "parser.c"
        break;
      case 123:
// 1514 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_type(XX_TYPE_ARRAY);
  yy_destructor(68,&yymsp[0].minor);
}
// 4332 "parser.c"
        break;
      case 124:
// 1518 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_type(XX_TYPE_VAR);
  yy_destructor(69,&yymsp[0].minor);
}
// 4340 "parser.c"
        break;
      case 147:
// 1610 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_empty_statement(status->scanner_state);
  yy_destructor(40,&yymsp[0].minor);
}
// 4348 "parser.c"
        break;
      case 148:
// 1614 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_break_statement(status->scanner_state);
  yy_destructor(70,&yymsp[-1].minor);
  yy_destructor(40,&yymsp[0].minor);
}
// 4357 "parser.c"
        break;
      case 149:
// 1618 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_continue_statement(status->scanner_state);
  yy_destructor(71,&yymsp[-1].minor);
  yy_destructor(40,&yymsp[0].minor);
}
// 4366 "parser.c"
        break;
      case 150:
// 1622 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_if_statement(yymsp[-2].minor.yy115, NULL, NULL, status->scanner_state);
  yy_destructor(72,&yymsp[-3].minor);
  yy_destructor(47,&yymsp[-1].minor);
  yy_destructor(48,&yymsp[0].minor);
}
// 4376 "parser.c"
        break;
      case 151:
// 1626 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_if_statement(yymsp[-5].minor.yy115, NULL, NULL, status->scanner_state);
  yy_destructor(72,&yymsp[-6].minor);
  yy_destructor(47,&yymsp[-4].minor);
  yy_destructor(48,&yymsp[-3].minor);
  yy_destructor(73,&yymsp[-2].minor);
  yy_destructor(47,&yymsp[-1].minor);
  yy_destructor(48,&yymsp[0].minor);
}
// 4389 "parser.c"
        break;
      case 152:
// 1630 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_if_statement(yymsp[-3].minor.yy115, yymsp[-1].minor.yy115, NULL, status->scanner_state);
  yy_destructor(72,&yymsp[-4].minor);
  yy_destructor(47,&yymsp[-2].minor);
  yy_destructor(48,&yymsp[0].minor);
}
// 4399 "parser.c"
        break;
      case 153:
// 1634 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_if_statement(yymsp[-7].minor.yy115, yymsp[-5].minor.yy115, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(72,&yymsp[-8].minor);
  yy_destructor(47,&yymsp[-6].minor);
  yy_destructor(48,&yymsp[-4].minor);
  yy_destructor(73,&yymsp[-3].minor);
  yy_destructor(47,&yymsp[-2].minor);
  yy_destructor(48,&yymsp[0].minor);
}
// 4412 "parser.c"
        break;
      case 154:
// 1638 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_if_statement(yymsp[-6].minor.yy115, yymsp[-4].minor.yy115, NULL, status->scanner_state);
  yy_destructor(72,&yymsp[-7].minor);
  yy_destructor(47,&yymsp[-5].minor);
  yy_destructor(48,&yymsp[-3].minor);
  yy_destructor(73,&yymsp[-2].minor);
  yy_destructor(47,&yymsp[-1].minor);
  yy_destructor(48,&yymsp[0].minor);
}
// 4425 "parser.c"
        break;
      case 155:
// 1642 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_if_statement(yymsp[-6].minor.yy115, NULL, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(72,&yymsp[-7].minor);
  yy_destructor(47,&yymsp[-5].minor);
  yy_destructor(48,&yymsp[-4].minor);
  yy_destructor(73,&yymsp[-3].minor);
  yy_destructor(47,&yymsp[-2].minor);
  yy_destructor(48,&yymsp[0].minor);
}
// 4438 "parser.c"
        break;
      case 156:
// 1646 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_switch_statement(yymsp[-2].minor.yy115, NULL, status->scanner_state);
  yy_destructor(74,&yymsp[-3].minor);
  yy_destructor(47,&yymsp[-1].minor);
  yy_destructor(48,&yymsp[0].minor);
}
// 4448 "parser.c"
        break;
      case 157:
// 1650 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_switch_statement(yymsp[-3].minor.yy115, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(74,&yymsp[-4].minor);
  yy_destructor(47,&yymsp[-2].minor);
  yy_destructor(48,&yymsp[0].minor);
}
// 4458 "parser.c"
        break;
      case 160:
// 1662 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_case_clause(yymsp[-1].minor.yy115, NULL, status->scanner_state);
  yy_destructor(75,&yymsp[-2].minor);
  yy_destructor(76,&yymsp[0].minor);
}
// 4467 "parser.c"
        break;
      case 161:
// 1666 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_case_clause(yymsp[-2].minor.yy115, yymsp[0].minor.yy115, status->scanner_state);
  yy_destructor(75,&yymsp[-3].minor);
  yy_destructor(76,&yymsp[-1].minor);
}
// 4476 "parser.c"
        break;
      case 162:
// 1670 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_case_clause(NULL, yymsp[0].minor.yy115, status->scanner_state);
  yy_destructor(77,&yymsp[-2].minor);
  yy_destructor(76,&yymsp[-1].minor);
}
// 4485 "parser.c"
        break;
      case 163:
// 1674 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_loop_statement(NULL, status->scanner_state);
  yy_destructor(78,&yymsp[-2].minor);
  yy_destructor(47,&yymsp[-1].minor);
  yy_destructor(48,&yymsp[0].minor);
}
// 4495 "parser.c"
        break;
      case 164:
// 1678 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_loop_statement(yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(78,&yymsp[-3].minor);
  yy_destructor(47,&yymsp[-2].minor);
  yy_destructor(48,&yymsp[0].minor);
}
// 4505 "parser.c"
        break;
      case 165:
// 1682 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_while_statement(yymsp[-2].minor.yy115, NULL, status->scanner_state);
  yy_destructor(79,&yymsp[-3].minor);
  yy_destructor(47,&yymsp[-1].minor);
  yy_destructor(48,&yymsp[0].minor);
}
// 4515 "parser.c"
        break;
      case 166:
// 1686 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_while_statement(yymsp[-3].minor.yy115, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(79,&yymsp[-4].minor);
  yy_destructor(47,&yymsp[-2].minor);
  yy_destructor(48,&yymsp[0].minor);
}
// 4525 "parser.c"
        break;
      case 167:
// 1690 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_do_while_statement(yymsp[-1].minor.yy115, NULL, status->scanner_state);
  yy_destructor(80,&yymsp[-5].minor);
  yy_destructor(47,&yymsp[-4].minor);
  yy_destructor(48,&yymsp[-3].minor);
  yy_destructor(79,&yymsp[-2].minor);
  yy_destructor(40,&yymsp[0].minor);
}
// 4537 "parser.c"
        break;
      case 168:
// 1694 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_do_while_statement(yymsp[-1].minor.yy115, yymsp[-4].minor.yy115, status->scanner_state);
  yy_destructor(80,&yymsp[-6].minor);
  yy_destructor(47,&yymsp[-5].minor);
  yy_destructor(48,&yymsp[-3].minor);
  yy_destructor(79,&yymsp[-2].minor);
  yy_destructor(40,&yymsp[0].minor);
}
// 4549 "parser.c"
        break;
      case 169:
// 1698 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_for_statement(yymsp[-3].minor.yy115, NULL, yymsp[-5].minor.yy0, 0, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(81,&yymsp[-6].minor);
  yy_destructor(82,&yymsp[-4].minor);
  yy_destructor(47,&yymsp[-2].minor);
  yy_destructor(48,&yymsp[0].minor);
}
// 4560 "parser.c"
        break;
      case 170:
// 1702 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_for_statement(yymsp[-3].minor.yy115, NULL, yymsp[-6].minor.yy0, 1, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(81,&yymsp[-7].minor);
  yy_destructor(82,&yymsp[-5].minor);
  yy_destructor(83,&yymsp[-4].minor);
  yy_destructor(47,&yymsp[-2].minor);
  yy_destructor(48,&yymsp[0].minor);
}
// 4572 "parser.c"
        break;
      case 171:
// 1706 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_for_statement(yymsp[-3].minor.yy115, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, 0, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(81,&yymsp[-8].minor);
  yy_destructor(6,&yymsp[-6].minor);
  yy_destructor(82,&yymsp[-4].minor);
  yy_destructor(47,&yymsp[-2].minor);
  yy_destructor(48,&yymsp[0].minor);
}
// 4584 "parser.c"
        break;
      case 172:
// 1710 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_for_statement(yymsp[-3].minor.yy115, yymsp[-8].minor.yy0, yymsp[-6].minor.yy0, 1, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(81,&yymsp[-9].minor);
  yy_destructor(6,&yymsp[-7].minor);
  yy_destructor(82,&yymsp[-5].minor);
  yy_destructor(83,&yymsp[-4].minor);
  yy_destructor(47,&yymsp[-2].minor);
  yy_destructor(48,&yymsp[0].minor);
}
// 4597 "parser.c"
        break;
      case 173:
// 1714 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_let_statement(yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(84,&yymsp[-2].minor);
  yy_destructor(40,&yymsp[0].minor);
}
// 4606 "parser.c"
        break;
      case 176:
// 1727 "parser.lemon"
{
	yygotominor.yy115 = json_object_new_string("assign");
  yy_destructor(50,&yymsp[0].minor);
}
// 4614 "parser.c"
        break;
      case 177:
// 1732 "parser.lemon"
{
	yygotominor.yy115 = json_object_new_string("add-assign");
  yy_destructor(85,&yymsp[0].minor);
}
// 4622 "parser.c"
        break;
      case 178:
// 1737 "parser.lemon"
{
	yygotominor.yy115 = json_object_new_string("sub-assign");
  yy_destructor(86,&yymsp[0].minor);
}
// 4630 "parser.c"
        break;
      case 179:
// 1741 "parser.lemon"
{
	yygotominor.yy115 = json_object_new_string("mult-assign");
  yy_destructor(87,&yymsp[0].minor);
}
// 4638 "parser.c"
        break;
      case 180:
// 1745 "parser.lemon"
{
	yygotominor.yy115 = json_object_new_string("div-assign");
  yy_destructor(88,&yymsp[0].minor);
}
// 4646 "parser.c"
        break;
      case 181:
// 1749 "parser.lemon"
{
	yygotominor.yy115 = json_object_new_string("concat-assign");
  yy_destructor(89,&yymsp[0].minor);
}
// 4654 "parser.c"
        break;
      case 182:
// 1754 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_let_assignment("variable", yymsp[-1].minor.yy115, yymsp[-2].minor.yy0, NULL, NULL, yymsp[0].minor.yy115, status->scanner_state);
}
// 4661 "parser.c"
        break;
      case 183:
// 1759 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_let_assignment("object-property", yymsp[-1].minor.yy115, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, yymsp[0].minor.yy115, status->scanner_state);
  yy_destructor(55,&yymsp[-3].minor);
}
// 4669 "parser.c"
        break;
      case 184:
// 1764 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_let_assignment("variable-dynamic-object-property", yymsp[-1].minor.yy115, yymsp[-6].minor.yy0, yymsp[-3].minor.yy0, NULL, yymsp[0].minor.yy115, status->scanner_state);
  yy_destructor(55,&yymsp[-5].minor);
  yy_destructor(47,&yymsp[-4].minor);
  yy_destructor(48,&yymsp[-2].minor);
}
// 4679 "parser.c"
        break;
      case 185:
// 1769 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_let_assignment("string-dynamic-object-property", yymsp[-1].minor.yy115, yymsp[-6].minor.yy0, yymsp[-3].minor.yy0, NULL, yymsp[0].minor.yy115, status->scanner_state);
  yy_destructor(55,&yymsp[-5].minor);
  yy_destructor(47,&yymsp[-4].minor);
  yy_destructor(48,&yymsp[-2].minor);
}
// 4689 "parser.c"
        break;
      case 186:
// 1774 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_let_assignment("object-property-append", yymsp[-1].minor.yy115, yymsp[-6].minor.yy0, yymsp[-4].minor.yy0, NULL, yymsp[0].minor.yy115, status->scanner_state);
  yy_destructor(55,&yymsp[-5].minor);
  yy_destructor(37,&yymsp[-3].minor);
  yy_destructor(58,&yymsp[-2].minor);
}
// 4699 "parser.c"
        break;
      case 187:
// 1779 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_let_assignment("object-property-array-index", yymsp[-1].minor.yy115, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, yymsp[-3].minor.yy115, yymsp[0].minor.yy115, status->scanner_state);
  yy_destructor(55,&yymsp[-6].minor);
  yy_destructor(37,&yymsp[-4].minor);
  yy_destructor(58,&yymsp[-2].minor);
}
// 4709 "parser.c"
        break;
      case 188:
// 1784 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_let_assignment("static-property", yymsp[-1].minor.yy115, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, yymsp[0].minor.yy115, status->scanner_state);
  yy_destructor(91,&yymsp[-3].minor);
}
// 4717 "parser.c"
        break;
      case 189:
// 1789 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_let_assignment("static-property-append", yymsp[-1].minor.yy115, yymsp[-6].minor.yy0, yymsp[-4].minor.yy0, NULL, yymsp[0].minor.yy115, status->scanner_state);
  yy_destructor(91,&yymsp[-5].minor);
  yy_destructor(37,&yymsp[-3].minor);
  yy_destructor(58,&yymsp[-2].minor);
}
// 4727 "parser.c"
        break;
      case 190:
// 1794 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_let_assignment("static-property-array-index", yymsp[-1].minor.yy115, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, yymsp[-3].minor.yy115, yymsp[0].minor.yy115, status->scanner_state);
  yy_destructor(91,&yymsp[-6].minor);
  yy_destructor(37,&yymsp[-4].minor);
  yy_destructor(58,&yymsp[-2].minor);
}
// 4737 "parser.c"
        break;
      case 191:
// 1799 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_let_assignment("variable-append", yymsp[-1].minor.yy115, yymsp[-4].minor.yy0, NULL, NULL, yymsp[0].minor.yy115, status->scanner_state);
  yy_destructor(37,&yymsp[-3].minor);
  yy_destructor(58,&yymsp[-2].minor);
}
// 4746 "parser.c"
        break;
      case 192:
// 1804 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_let_assignment("array-index", yymsp[-1].minor.yy115, yymsp[-3].minor.yy0, NULL, yymsp[-2].minor.yy115, yymsp[0].minor.yy115, status->scanner_state);
}
// 4753 "parser.c"
        break;
      case 195:
// 1816 "parser.lemon"
{
	yygotominor.yy115 = yymsp[-1].minor.yy115;
  yy_destructor(37,&yymsp[-2].minor);
  yy_destructor(58,&yymsp[0].minor);
}
// 4762 "parser.c"
        break;
      case 196:
// 1821 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_let_assignment("incr", NULL, yymsp[-3].minor.yy0, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(55,&yymsp[-2].minor);
  yy_destructor(92,&yymsp[0].minor);
}
// 4771 "parser.c"
        break;
      case 197:
// 1826 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_let_assignment("decr", NULL, yymsp[-3].minor.yy0, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(55,&yymsp[-2].minor);
  yy_destructor(93,&yymsp[0].minor);
}
// 4780 "parser.c"
        break;
      case 198:
// 1831 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_let_assignment("incr", NULL, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(92,&yymsp[0].minor);
}
// 4788 "parser.c"
        break;
      case 199:
// 1836 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_let_assignment("decr", NULL, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(93,&yymsp[0].minor);
}
// 4796 "parser.c"
        break;
      case 201:
// 1844 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_echo_statement(yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(94,&yymsp[-2].minor);
  yy_destructor(40,&yymsp[0].minor);
}
// 4805 "parser.c"
        break;
      case 204:
// 1856 "parser.lemon"
{
	yygotominor.yy115 = yymsp[0].minor.yy115;;
}
// 4812 "parser.c"
        break;
      case 205:
// 1861 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_mcall_statement(yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(40,&yymsp[0].minor);
}
// 4820 "parser.c"
        break;
      case 206:
// 1866 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_fcall_statement(yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(40,&yymsp[0].minor);
}
// 4828 "parser.c"
        break;
      case 207:
// 1871 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_scall_statement(yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(40,&yymsp[0].minor);
}
// 4836 "parser.c"
        break;
      case 208:
// 1876 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_fetch_statement(yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(40,&yymsp[0].minor);
}
// 4844 "parser.c"
        break;
      case 209:
// 1881 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_return_statement(yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(95,&yymsp[-2].minor);
  yy_destructor(40,&yymsp[0].minor);
}
// 4853 "parser.c"
        break;
      case 210:
// 1886 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_return_statement(NULL, status->scanner_state);
  yy_destructor(95,&yymsp[-1].minor);
  yy_destructor(40,&yymsp[0].minor);
}
// 4862 "parser.c"
        break;
      case 211:
// 1891 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(96,&yymsp[-4].minor);
  yy_destructor(55,&yymsp[-2].minor);
  yy_destructor(40,&yymsp[0].minor);
}
// 4876 "parser.c"
        break;
      case 212:
// 1900 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state),
		yymsp[-2].minor.yy115,
		status->scanner_state
	);
  yy_destructor(96,&yymsp[-5].minor);
  yy_destructor(37,&yymsp[-3].minor);
  yy_destructor(58,&yymsp[-1].minor);
  yy_destructor(40,&yymsp[0].minor);
}
// 4891 "parser.c"
        break;
      case 213:
// 1909 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_throw_exception(yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(97,&yymsp[-2].minor);
  yy_destructor(40,&yymsp[0].minor);
}
// 4900 "parser.c"
        break;
      case 214:
// 1913 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_declare_statement(XX_T_TYPE_INTEGER, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(59,&yymsp[-2].minor);
  yy_destructor(40,&yymsp[0].minor);
}
// 4909 "parser.c"
        break;
      case 215:
// 1917 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_declare_statement(XX_T_TYPE_UINTEGER, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(60,&yymsp[-2].minor);
  yy_destructor(40,&yymsp[0].minor);
}
// 4918 "parser.c"
        break;
      case 216:
// 1921 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_declare_statement(XX_T_TYPE_CHAR, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(63,&yymsp[-2].minor);
  yy_destructor(40,&yymsp[0].minor);
}
// 4927 "parser.c"
        break;
      case 217:
// 1925 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_declare_statement(XX_T_TYPE_UCHAR, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(64,&yymsp[-2].minor);
  yy_destructor(40,&yymsp[0].minor);
}
// 4936 "parser.c"
        break;
      case 218:
// 1929 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_declare_statement(XX_T_TYPE_LONG, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(61,&yymsp[-2].minor);
  yy_destructor(40,&yymsp[0].minor);
}
// 4945 "parser.c"
        break;
      case 219:
// 1933 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_declare_statement(XX_T_TYPE_ULONG, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(62,&yymsp[-2].minor);
  yy_destructor(40,&yymsp[0].minor);
}
// 4954 "parser.c"
        break;
      case 220:
// 1937 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_declare_statement(XX_T_TYPE_DOUBLE, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(65,&yymsp[-2].minor);
  yy_destructor(40,&yymsp[0].minor);
}
// 4963 "parser.c"
        break;
      case 221:
// 1941 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_declare_statement(XX_T_TYPE_STRING, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(67,&yymsp[-2].minor);
  yy_destructor(40,&yymsp[0].minor);
}
// 4972 "parser.c"
        break;
      case 222:
// 1945 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_declare_statement(XX_T_TYPE_BOOL, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(66,&yymsp[-2].minor);
  yy_destructor(40,&yymsp[0].minor);
}
// 4981 "parser.c"
        break;
      case 223:
// 1949 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_declare_statement(XX_T_TYPE_VAR, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(69,&yymsp[-2].minor);
  yy_destructor(40,&yymsp[0].minor);
}
// 4990 "parser.c"
        break;
      case 226:
// 1961 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_declare_variable(yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 4997 "parser.c"
        break;
      case 227:
// 1965 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_declare_variable(yymsp[-2].minor.yy0, yymsp[0].minor.yy115, status->scanner_state);
  yy_destructor(50,&yymsp[-1].minor);
}
// 5005 "parser.c"
        break;
      case 229:
// 1973 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("not", yymsp[0].minor.yy115, NULL, NULL, status->scanner_state);
  yy_destructor(35,&yymsp[-1].minor);
}
// 5013 "parser.c"
        break;
      case 230:
// 1977 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("isset", yymsp[0].minor.yy115, NULL, NULL, status->scanner_state);
  yy_destructor(31,&yymsp[-1].minor);
}
// 5021 "parser.c"
        break;
      case 231:
// 1981 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("require", yymsp[0].minor.yy115, NULL, NULL, status->scanner_state);
  yy_destructor(7,&yymsp[-1].minor);
}
// 5029 "parser.c"
        break;
      case 232:
// 1985 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("clone", yymsp[0].minor.yy115, NULL, NULL, status->scanner_state);
  yy_destructor(10,&yymsp[-1].minor);
}
// 5037 "parser.c"
        break;
      case 233:
// 1989 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("empty", yymsp[0].minor.yy115, NULL, NULL, status->scanner_state);
  yy_destructor(11,&yymsp[-1].minor);
}
// 5045 "parser.c"
        break;
      case 234:
// 1993 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("likely", yymsp[0].minor.yy115, NULL, NULL, status->scanner_state);
  yy_destructor(8,&yymsp[-1].minor);
}
// 5053 "parser.c"
        break;
      case 235:
// 1997 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("unlikely", yymsp[0].minor.yy115, NULL, NULL, status->scanner_state);
  yy_destructor(9,&yymsp[-1].minor);
}
// 5061 "parser.c"
        break;
      case 236:
// 2001 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("equals", yymsp[-2].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(17,&yymsp[-1].minor);
}
// 5069 "parser.c"
        break;
      case 237:
// 2005 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("not-equals", yymsp[-2].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(24,&yymsp[-1].minor);
}
// 5077 "parser.c"
        break;
      case 238:
// 2009 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("identical", yymsp[-2].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(18,&yymsp[-1].minor);
}
// 5085 "parser.c"
        break;
      case 239:
// 2013 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("not-identical", yymsp[-2].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(23,&yymsp[-1].minor);
}
// 5093 "parser.c"
        break;
      case 240:
// 2017 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("less", yymsp[-2].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(19,&yymsp[-1].minor);
}
// 5101 "parser.c"
        break;
      case 241:
// 2021 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("greater", yymsp[-2].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(20,&yymsp[-1].minor);
}
// 5109 "parser.c"
        break;
      case 242:
// 2025 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("less-equal", yymsp[-2].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(21,&yymsp[-1].minor);
}
// 5117 "parser.c"
        break;
      case 243:
// 2029 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("greater-equal", yymsp[-2].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(22,&yymsp[-1].minor);
}
// 5125 "parser.c"
        break;
      case 244:
// 2033 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("list", yymsp[-1].minor.yy115, NULL, NULL, status->scanner_state);
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5134 "parser.c"
        break;
      case 245:
// 2037 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("cast", yymsp[-2].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(54,&yymsp[-3].minor);
  yy_destructor(36,&yymsp[-1].minor);
}
// 5143 "parser.c"
        break;
      case 246:
// 2041 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("type-hint", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(19,&yymsp[-3].minor);
  yy_destructor(20,&yymsp[-1].minor);
}
// 5152 "parser.c"
        break;
      case 247:
      case 322:
// 2045 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("property-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(55,&yymsp[-1].minor);
}
// 5161 "parser.c"
        break;
      case 248:
      case 323:
// 2049 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("property-dynamic-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(55,&yymsp[-3].minor);
  yy_destructor(47,&yymsp[-2].minor);
  yy_destructor(48,&yymsp[0].minor);
}
// 5172 "parser.c"
        break;
      case 249:
      case 324:
// 2053 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("property-string-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_STRING, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(55,&yymsp[-3].minor);
  yy_destructor(47,&yymsp[-2].minor);
  yy_destructor(48,&yymsp[0].minor);
}
// 5183 "parser.c"
        break;
      case 250:
// 2057 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("static-property-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(91,&yymsp[-1].minor);
}
// 5191 "parser.c"
        break;
      case 251:
      case 319:
// 2061 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("static-constant-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(91,&yymsp[-1].minor);
}
// 5200 "parser.c"
        break;
      case 252:
// 2070 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("array-access", yymsp[-3].minor.yy115, yymsp[-1].minor.yy115, NULL, status->scanner_state);
  yy_destructor(37,&yymsp[-2].minor);
  yy_destructor(58,&yymsp[0].minor);
}
// 5209 "parser.c"
        break;
      case 253:
// 2075 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("add", yymsp[-2].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(25,&yymsp[-1].minor);
}
// 5217 "parser.c"
        break;
      case 254:
// 2080 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("sub", yymsp[-2].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(26,&yymsp[-1].minor);
}
// 5225 "parser.c"
        break;
      case 255:
// 2085 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("mul", yymsp[-2].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(28,&yymsp[-1].minor);
}
// 5233 "parser.c"
        break;
      case 256:
// 2090 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("div", yymsp[-2].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(29,&yymsp[-1].minor);
}
// 5241 "parser.c"
        break;
      case 257:
// 2095 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("mod", yymsp[-2].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(30,&yymsp[-1].minor);
}
// 5249 "parser.c"
        break;
      case 258:
// 2100 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("concat", yymsp[-2].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(27,&yymsp[-1].minor);
}
// 5257 "parser.c"
        break;
      case 259:
// 2105 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("and", yymsp[-2].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(13,&yymsp[-1].minor);
}
// 5265 "parser.c"
        break;
      case 260:
// 2110 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("or", yymsp[-2].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(14,&yymsp[-1].minor);
}
// 5273 "parser.c"
        break;
      case 261:
// 2115 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("instanceof", yymsp[-2].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(12,&yymsp[-1].minor);
}
// 5281 "parser.c"
        break;
      case 262:
// 2120 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("fetch", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(32,&yymsp[-3].minor);
  yy_destructor(6,&yymsp[-1].minor);
}
// 5290 "parser.c"
        break;
      case 264:
// 2130 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("typeof", yymsp[0].minor.yy115, NULL, NULL, status->scanner_state);
  yy_destructor(33,&yymsp[-1].minor);
}
// 5298 "parser.c"
        break;
      case 266:
      case 310:
      case 312:
// 2140 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_literal(XX_T_INTEGER, yymsp[0].minor.yy0, status->scanner_state);
}
// 5307 "parser.c"
        break;
      case 267:
      case 309:
      case 314:
// 2145 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_literal(XX_T_STRING, yymsp[0].minor.yy0, status->scanner_state);
}
// 5316 "parser.c"
        break;
      case 268:
      case 313:
// 2150 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_literal(XX_T_CHAR, yymsp[0].minor.yy0, status->scanner_state);
}
// 5324 "parser.c"
        break;
      case 269:
      case 315:
// 2155 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_literal(XX_T_DOUBLE, yymsp[0].minor.yy0, status->scanner_state);
}
// 5332 "parser.c"
        break;
      case 270:
      case 316:
// 2160 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_literal(XX_T_NULL, NULL, status->scanner_state);
  yy_destructor(101,&yymsp[0].minor);
}
// 5341 "parser.c"
        break;
      case 271:
      case 318:
// 2165 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_literal(XX_T_TRUE, NULL, status->scanner_state);
  yy_destructor(102,&yymsp[0].minor);
}
// 5350 "parser.c"
        break;
      case 272:
      case 317:
// 2170 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_literal(XX_T_FALSE, NULL, status->scanner_state);
  yy_destructor(103,&yymsp[0].minor);
}
// 5359 "parser.c"
        break;
      case 273:
      case 320:
// 2175 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_literal(XX_T_CONSTANT, yymsp[0].minor.yy0, status->scanner_state);
}
// 5367 "parser.c"
        break;
      case 274:
// 2180 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("empty-array", NULL, NULL, NULL, status->scanner_state);
  yy_destructor(37,&yymsp[-1].minor);
  yy_destructor(58,&yymsp[0].minor);
}
// 5376 "parser.c"
        break;
      case 275:
// 2185 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("array", yymsp[-1].minor.yy115, NULL, NULL, status->scanner_state);
  yy_destructor(37,&yymsp[-2].minor);
  yy_destructor(58,&yymsp[0].minor);
}
// 5385 "parser.c"
        break;
      case 276:
// 2190 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_new_instance(0, yymsp[0].minor.yy0, NULL, status->scanner_state);
  yy_destructor(34,&yymsp[-1].minor);
}
// 5393 "parser.c"
        break;
      case 277:
// 2195 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_new_instance(0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(34,&yymsp[-3].minor);
  yy_destructor(54,&yymsp[-1].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5403 "parser.c"
        break;
      case 278:
// 2200 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_new_instance(0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(34,&yymsp[-4].minor);
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5413 "parser.c"
        break;
      case 279:
// 2205 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_new_instance(1, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(34,&yymsp[-3].minor);
  yy_destructor(47,&yymsp[-2].minor);
  yy_destructor(48,&yymsp[0].minor);
}
// 5423 "parser.c"
        break;
      case 280:
// 2210 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_new_instance(1, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(34,&yymsp[-5].minor);
  yy_destructor(47,&yymsp[-4].minor);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(54,&yymsp[-1].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5435 "parser.c"
        break;
      case 281:
// 2215 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_new_instance(1, yymsp[-4].minor.yy0, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(34,&yymsp[-6].minor);
  yy_destructor(47,&yymsp[-5].minor);
  yy_destructor(48,&yymsp[-3].minor);
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5447 "parser.c"
        break;
      case 282:
// 2220 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_fcall(1, yymsp[-3].minor.yy0, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5456 "parser.c"
        break;
      case 283:
// 2225 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_fcall(1, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(54,&yymsp[-1].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5465 "parser.c"
        break;
      case 284:
// 2230 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_fcall(2, yymsp[-4].minor.yy0, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(47,&yymsp[-5].minor);
  yy_destructor(48,&yymsp[-3].minor);
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5476 "parser.c"
        break;
      case 285:
// 2235 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_fcall(2, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(47,&yymsp[-4].minor);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(54,&yymsp[-1].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5487 "parser.c"
        break;
      case 286:
// 2240 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_scall(0, yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(91,&yymsp[-4].minor);
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5497 "parser.c"
        break;
      case 287:
// 2245 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_scall(0, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(91,&yymsp[-3].minor);
  yy_destructor(54,&yymsp[-1].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5507 "parser.c"
        break;
      case 288:
// 2250 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_scall(1, yymsp[-6].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(47,&yymsp[-7].minor);
  yy_destructor(48,&yymsp[-5].minor);
  yy_destructor(91,&yymsp[-4].minor);
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5519 "parser.c"
        break;
      case 289:
// 2255 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_scall(1, yymsp[-5].minor.yy0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(47,&yymsp[-6].minor);
  yy_destructor(48,&yymsp[-4].minor);
  yy_destructor(91,&yymsp[-3].minor);
  yy_destructor(54,&yymsp[-1].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5531 "parser.c"
        break;
      case 290:
// 2260 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_scall(1, yymsp[-8].minor.yy0, yymsp[-4].minor.yy0, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(47,&yymsp[-9].minor);
  yy_destructor(48,&yymsp[-7].minor);
  yy_destructor(91,&yymsp[-6].minor);
  yy_destructor(47,&yymsp[-5].minor);
  yy_destructor(48,&yymsp[-3].minor);
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5545 "parser.c"
        break;
      case 291:
// 2265 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_scall(1, yymsp[-7].minor.yy0, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(47,&yymsp[-8].minor);
  yy_destructor(48,&yymsp[-6].minor);
  yy_destructor(91,&yymsp[-5].minor);
  yy_destructor(47,&yymsp[-4].minor);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(54,&yymsp[-1].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5559 "parser.c"
        break;
      case 292:
// 2270 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_mcall(1, yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(55,&yymsp[-4].minor);
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5569 "parser.c"
        break;
      case 293:
// 2275 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_mcall(1, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(55,&yymsp[-3].minor);
  yy_destructor(54,&yymsp[-1].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5579 "parser.c"
        break;
      case 294:
// 2280 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_mcall(2, yymsp[-7].minor.yy0, yymsp[-4].minor.yy0, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(55,&yymsp[-6].minor);
  yy_destructor(47,&yymsp[-5].minor);
  yy_destructor(48,&yymsp[-3].minor);
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5591 "parser.c"
        break;
      case 295:
// 2285 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_mcall(2, yymsp[-6].minor.yy0, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(55,&yymsp[-5].minor);
  yy_destructor(47,&yymsp[-4].minor);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(54,&yymsp[-1].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5603 "parser.c"
        break;
      case 296:
// 2290 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_mcall(3, yymsp[-7].minor.yy0, yymsp[-4].minor.yy0, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(55,&yymsp[-6].minor);
  yy_destructor(47,&yymsp[-5].minor);
  yy_destructor(48,&yymsp[-3].minor);
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5615 "parser.c"
        break;
      case 297:
// 2295 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_mcall(3, yymsp[-6].minor.yy0, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(55,&yymsp[-5].minor);
  yy_destructor(47,&yymsp[-4].minor);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(54,&yymsp[-1].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5627 "parser.c"
        break;
      case 306:
// 2334 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_array_item(yymsp[-2].minor.yy115, yymsp[0].minor.yy115, status->scanner_state);
  yy_destructor(76,&yymsp[-1].minor);
}
// 5635 "parser.c"
        break;
      case 307:
// 2338 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_array_item(NULL, yymsp[0].minor.yy115, status->scanner_state);
}
// 5642 "parser.c"
        break;
      case 321:
// 2395 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), yymsp[-1].minor.yy115, NULL, status->scanner_state);
  yy_destructor(37,&yymsp[-2].minor);
  yy_destructor(58,&yymsp[0].minor);
}
// 5651 "parser.c"
        break;
      case 326:
// 2418 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_comment(yymsp[0].minor.yy0, status->scanner_state);
}
// 5658 "parser.c"
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
  xx_ARG_FETCH;
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sFail!\n",yyTracePrompt);
  }
#endif
  while( yypParser->yyidx>=0 ) yy_pop_parser_stack(yypParser);
  /* Here code is inserted which will be executed whenever the
  ** parser fails */
  xx_ARG_STORE; /* Suppress warning about unused %extra_argument variable */
}

/*
** The following code executes when a syntax error first occurs.
*/
static void yy_syntax_error(
  yyParser *yypParser,           /* The parser */
  int yymajor,                   /* The major type of the error token */
  YYMINORTYPE yyminor            /* The minor type of the error token */
){
  xx_ARG_FETCH;
#define TOKEN (yyminor.yy0)
// 966 "parser.lemon"


	//fprintf(stderr, "error!\n");

	json_object *syntax_error = json_object_new_object();

	json_object_object_add(syntax_error, "type", json_object_new_string("error"));

	/*if (status->scanner_state->start_length) {
		fprintf(stderr, "Syntax error, %s", status->scanner_state->start);
	} else {
		fprintf(stderr, "EOF");
	}*/

	//status->syntax_error_len = 48 + Z_STRLEN_P(status->scanner_state->active_file);
	//status->syntax_error = emalloc(sizeof(char) * status->syntax_error_len);

	if (status->scanner_state->start_length) {
		json_object_object_add(syntax_error, "message", json_object_new_string("Syntax error"));
	} else {
		json_object_object_add(syntax_error, "message", json_object_new_string("Unexpected EOF"));
	}

	json_object_object_add(syntax_error, "file", json_object_new_string(status->scanner_state->active_file));
	json_object_object_add(syntax_error, "line", json_object_new_int(status->scanner_state->active_line));
	json_object_object_add(syntax_error, "char", json_object_new_int(status->scanner_state->active_char));

	status->status = XX_PARSING_FAILED;

	status->ret = syntax_error;

	//status->scanner_state->active_file

// 5734 "parser.c"
  xx_ARG_STORE; /* Suppress warning about unused %extra_argument variable */
}

/*
** The following is executed when the parser accepts
*/
static void yy_accept(
  yyParser *yypParser           /* The parser */
){
  xx_ARG_FETCH;
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sAccept!\n",yyTracePrompt);
  }
#endif
  while( yypParser->yyidx>=0 ) yy_pop_parser_stack(yypParser);
  /* Here code is inserted which will be executed whenever the
  ** parser accepts */
  xx_ARG_STORE; /* Suppress warning about unused %extra_argument variable */
}

/* The main parser program.
** The first argument is a pointer to a structure obtained from
** "xx_Alloc" which describes the current state of the parser.
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
void xx_(
  void *yyp,                   /* The parser */
  int yymajor,                 /* The major token code number */
  xx_TOKENTYPE yyminor       /* The value for the token */
  xx_ARG_PDECL               /* Optional %extra_argument parameter */
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
  xx_ARG_STORE;

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
 | Copyright (c) 2013 Zephir Team and contributors                          |
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

#define SUCCESS 1
#define FAILURE 0

const xx_token_names xx_tokens[] =
{
	{ XX_T_INTEGER,       		"INTEGER" },
	{ XX_T_DOUBLE,        		"DOUBLE" },
	{ XX_T_STRING,        		"STRING" },
	{ XX_T_IDENTIFIER,    		"IDENTIFIER" },
	{ XX_T_AT,	   	    		"@" },
	{ XX_T_COMMA,	   	    	"," },
	{ XX_T_ASSIGN,       		"=" },
	{ XX_T_COLON,            	":" },
	{ XX_T_PARENTHESES_OPEN,  	"(" },
	{ XX_T_PARENTHESES_CLOSE, 	")" },
	{ XX_T_BRACKET_OPEN,     	"{" },
	{ XX_T_BRACKET_CLOSE,    	"}" },
 	{ XX_T_SBRACKET_OPEN,    	"[" },
	{ XX_T_SBRACKET_CLOSE,   	"]" },
	{  0, NULL }
};

/**
 * Wrapper to alloc memory within the parser
 */
static void *xx_wrapper_alloc(size_t bytes){
	return malloc(bytes);
}

/**
 * Wrapper to free memory within the parser
 */
static void xx_wrapper_free(void *pointer){
	//free(pointer);
}

/**
 * Creates a parser_token to be passed to the parser
 */
static void xx_parse_with_token(void* xx_parser, int opcode, int parsercode, xx_scanner_token *token, xx_parser_status *parser_status){

	xx_parser_token *pToken;

	pToken = malloc(sizeof(xx_parser_token));
	pToken->opcode = opcode;
	pToken->token = token->value;
	pToken->token_len = token->len;
	pToken->free_flag = 1;

	xx_(xx_parser, parsercode, pToken, parser_status);

	token->value = NULL;
	token->len = 0;
}

/**
 * Creates an error message when it's triggered by the scanner
 */
static void xx_scanner_error_msg(xx_parser_status *parser_status){

	/*char *error, *error_part;
	XX_scanner_state *state = parser_status->scanner_state;

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
int xx_parse_program(char *program, unsigned int program_length, char *file_path) {

	char *error;
	xx_scanner_state *state;
	xx_scanner_token token;
	int scanner_status, status = SUCCESS, start_lines;
	xx_parser_status *parser_status = NULL;
	void* xx_parser;

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
	xx_parser = xx_Alloc(xx_wrapper_alloc);

	parser_status = malloc(sizeof(xx_parser_status));
	state = malloc(sizeof(xx_scanner_state));

	parser_status->status = XX_PARSING_OK;
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

	while (0 <= (scanner_status = xx_get_token(state, &token))) {

		state->active_token = token.opcode;

		state->start_length = (program + program_length - state->start);

		switch (token.opcode) {

			case XX_T_IGNORE:
				break;

			case XX_T_NAMESPACE:
				xx_(xx_parser, XX_NAMESPACE, NULL, parser_status);
				break;
			case XX_T_ABSTRACT:
				xx_(xx_parser, XX_ABSTRACT, NULL, parser_status);
				break;
			case XX_T_CLASS:
				xx_(xx_parser, XX_CLASS, NULL, parser_status);
				break;
			case XX_T_INTERFACE:
				xx_(xx_parser, XX_INTERFACE, NULL, parser_status);
				break;
			case XX_T_EXTENDS:
				xx_(xx_parser, XX_EXTENDS, NULL, parser_status);
				break;
			case XX_T_IMPLEMENTS:
				xx_(xx_parser, XX_IMPLEMENTS, NULL, parser_status);
				break;
			case XX_T_PUBLIC:
				xx_(xx_parser, XX_PUBLIC, NULL, parser_status);
				break;
			case XX_T_PROTECTED:
				xx_(xx_parser, XX_PROTECTED, NULL, parser_status);
				break;
			case XX_T_PRIVATE:
				xx_(xx_parser, XX_PRIVATE, NULL, parser_status);
				break;
			case XX_T_STATIC:
				xx_(xx_parser, XX_STATIC, NULL, parser_status);
				break;
			case XX_T_INLINE:
				xx_(xx_parser, XX_INLINE, NULL, parser_status);
				break;
			case XX_T_FINAL:
				xx_(xx_parser, XX_FINAL, NULL, parser_status);
				break;
			case XX_T_FUNCTION:
				xx_(xx_parser, XX_FUNCTION, NULL, parser_status);
				break;
			case XX_T_LET:
				xx_(xx_parser, XX_LET, NULL, parser_status);
				break;
			case XX_T_ECHO:
				xx_(xx_parser, XX_ECHO, NULL, parser_status);
				break;
			case XX_T_RETURN:
				xx_(xx_parser, XX_RETURN, NULL, parser_status);
				break;
			case XX_T_REQUIRE:
				xx_(xx_parser, XX_REQUIRE, NULL, parser_status);
				break;
			case XX_T_CLONE:
				xx_(xx_parser, XX_CLONE, NULL, parser_status);
				break;
			case XX_T_EMPTY:
				xx_(xx_parser, XX_EMPTY, NULL, parser_status);
				break;
			case XX_T_IF:
				xx_(xx_parser, XX_IF, NULL, parser_status);
				break;
			case XX_T_ELSE:
				xx_(xx_parser, XX_ELSE, NULL, parser_status);
				break;
			case XX_T_LOOP:
				xx_(xx_parser, XX_LOOP, NULL, parser_status);
				break;
			case XX_T_CONTINUE:
				xx_(xx_parser, XX_CONTINUE, NULL, parser_status);
				break;
			case XX_T_BREAK:
				xx_(xx_parser, XX_BREAK, NULL, parser_status);
				break;
			case XX_T_WHILE:
				xx_(xx_parser, XX_WHILE, NULL, parser_status);
				break;
			case XX_T_DO:
				xx_(xx_parser, XX_DO, NULL, parser_status);
				break;
			case XX_T_NEW:
				xx_(xx_parser, XX_NEW, NULL, parser_status);
				break;
			case XX_T_CONST:
				xx_(xx_parser, XX_CONST, NULL, parser_status);
				break;
			case XX_T_TYPEOF:
				xx_(xx_parser, XX_TYPEOF, NULL, parser_status);
				break;
			case XX_T_INSTANCEOF:
				xx_(xx_parser, XX_INSTANCEOF, NULL, parser_status);
				break;
			case XX_T_ISSET:
				xx_(xx_parser, XX_ISSET, NULL, parser_status);
				break;
			case XX_T_UNSET:
				xx_(xx_parser, XX_UNSET, NULL, parser_status);
				break;
			case XX_T_THROW:
				xx_(xx_parser, XX_THROW, NULL, parser_status);
				break;
			case XX_T_FOR:
				xx_(xx_parser, XX_FOR, NULL, parser_status);
				break;
			case XX_T_IN:
				xx_(xx_parser, XX_IN, NULL, parser_status);
				break;
			case XX_T_REVERSE:
				xx_(xx_parser, XX_REVERSE, NULL, parser_status);
				break;

			case XX_T_DOTCOMMA:
				xx_(xx_parser, XX_DOTCOMMA, NULL, parser_status);
				break;
			case XX_T_COMMA:
				xx_(xx_parser, XX_COMMA, NULL, parser_status);
				break;
			case XX_T_ASSIGN:
				xx_(xx_parser, XX_ASSIGN, NULL, parser_status);
				break;
			case XX_T_ADDASSIGN:
				xx_(xx_parser, XX_ADDASSIGN, NULL, parser_status);
				break;
			case XX_T_SUBASSIGN:
				xx_(xx_parser, XX_SUBASSIGN, NULL, parser_status);
				break;
			case XX_T_MULASSIGN:
				xx_(xx_parser, XX_MULASSIGN, NULL, parser_status);
				break;
			case XX_T_CONCATASSIGN:
				xx_(xx_parser, XX_CONCATASSIGN, NULL, parser_status);
				break;
			case XX_T_EQUALS:
				xx_(xx_parser, XX_EQUALS, NULL, parser_status);
				break;
			case XX_T_NOTEQUALS:
				xx_(xx_parser, XX_NOTEQUALS, NULL, parser_status);
				break;
			case XX_T_IDENTICAL:
				xx_(xx_parser, XX_IDENTICAL, NULL, parser_status);
				break;
			case XX_T_NOTIDENTICAL:
				xx_(xx_parser, XX_NOTIDENTICAL, NULL, parser_status);
				break;
			case XX_T_LESS:
				xx_(xx_parser, XX_LESS, NULL, parser_status);
				break;
			case XX_T_GREATER:
				xx_(xx_parser, XX_GREATER, NULL, parser_status);
				break;
			case XX_T_LESSEQUAL:
				xx_(xx_parser, XX_LESSEQUAL, NULL, parser_status);
				break;
			case XX_T_GREATEREQUAL:
				xx_(xx_parser, XX_GREATEREQUAL, NULL, parser_status);
				break;
			case XX_T_COLON:
				xx_(xx_parser, XX_COLON, NULL, parser_status);
				break;
			case XX_T_ARROW:
				xx_(xx_parser, XX_ARROW, NULL, parser_status);
				break;
			case XX_T_DOUBLECOLON:
				xx_(xx_parser, XX_DOUBLECOLON, NULL, parser_status);
				break;
			case XX_T_NOT:
				xx_(xx_parser, XX_NOT, NULL, parser_status);
				break;
			case XX_T_FETCH:
				xx_(xx_parser, XX_FETCH, NULL, parser_status);
				break;
			case XX_T_SWITCH:
				xx_(xx_parser, XX_SWITCH, NULL, parser_status);
				break;
			case XX_T_CASE:
				xx_(xx_parser, XX_CASE, NULL, parser_status);
				break;
			case XX_T_DEFAULT:
				xx_(xx_parser, XX_DEFAULT, NULL, parser_status);
				break;

			case XX_T_PARENTHESES_OPEN:
				xx_(xx_parser, XX_PARENTHESES_OPEN, NULL, parser_status);
				break;
			case XX_T_PARENTHESES_CLOSE:
				xx_(xx_parser, XX_PARENTHESES_CLOSE, NULL, parser_status);
				break;

			case XX_T_BRACKET_OPEN:
				parser_status->number_brackets++;
				xx_(xx_parser, XX_BRACKET_OPEN, NULL, parser_status);
				break;
			case XX_T_BRACKET_CLOSE:
				parser_status->number_brackets--;
				xx_(xx_parser, XX_BRACKET_CLOSE, NULL, parser_status);
				break;

			case XX_T_SBRACKET_OPEN:
				xx_(xx_parser, XX_SBRACKET_OPEN, NULL, parser_status);
				break;
			case XX_T_SBRACKET_CLOSE:
				xx_(xx_parser, XX_SBRACKET_CLOSE, NULL, parser_status);
				break;

			case XX_T_NULL:
				xx_(xx_parser, XX_NULL, NULL, parser_status);
				break;
			case XX_T_TRUE:
				xx_(xx_parser, XX_TRUE, NULL, parser_status);
				break;
			case XX_T_FALSE:
				xx_(xx_parser, XX_FALSE, NULL, parser_status);
				break;
			case XX_T_COMMENT:
				if (parser_status->number_brackets <= 1) {
					xx_parse_with_token(xx_parser, XX_T_COMMENT, XX_COMMENT, &token, parser_status);
				}
				break;

			case XX_T_TYPE_INTEGER:
				xx_(xx_parser, XX_TYPE_INTEGER, NULL, parser_status);
				break;
			case XX_T_TYPE_UINTEGER:
				xx_(xx_parser, XX_TYPE_UINTEGER, NULL, parser_status);
				break;
			case XX_T_TYPE_CHAR:
				xx_(xx_parser, XX_TYPE_CHAR, NULL, parser_status);
				break;
			case XX_T_TYPE_UCHAR:
				xx_(xx_parser, XX_TYPE_UCHAR, NULL, parser_status);
				break;
			case XX_T_TYPE_LONG:
				xx_(xx_parser, XX_TYPE_LONG, NULL, parser_status);
				break;
			case XX_T_TYPE_ULONG:
				xx_(xx_parser, XX_TYPE_ULONG, NULL, parser_status);
				break;
			case XX_T_TYPE_DOUBLE:
				xx_(xx_parser, XX_TYPE_DOUBLE, NULL, parser_status);
				break;
			case XX_T_TYPE_STRING:
				xx_(xx_parser, XX_TYPE_STRING, NULL, parser_status);
				break;
			case XX_T_TYPE_BOOL:
				xx_(xx_parser, XX_TYPE_BOOL, NULL, parser_status);
				break;
			case XX_T_TYPE_VAR:
				xx_(xx_parser, XX_TYPE_VAR, NULL, parser_status);
				break;

			case XX_T_ADD:
				xx_(xx_parser, XX_ADD, NULL, parser_status);
				break;
			case XX_T_SUB:
				xx_(xx_parser, XX_SUB, NULL, parser_status);
				break;
			case XX_T_MUL:
				xx_(xx_parser, XX_MUL, NULL, parser_status);
				break;
			case XX_T_DIV:
				xx_(xx_parser, XX_DIV, NULL, parser_status);
				break;
			case XX_T_MOD:
				xx_(xx_parser, XX_MOD, NULL, parser_status);
				break;
			case XX_T_DOT:
				xx_(xx_parser, XX_CONCAT, NULL, parser_status);
				break;
			case XX_T_INCR:
				xx_(xx_parser, XX_INCR, NULL, parser_status);
				break;
			case XX_T_DECR:
				xx_(xx_parser, XX_DECR, NULL, parser_status);
				break;
			case XX_T_AND:
				xx_(xx_parser, XX_AND, NULL, parser_status);
				break;
			case XX_T_OR:
				xx_(xx_parser, XX_OR, NULL, parser_status);
				break;
			case XX_T_BITWISE_AND:
				xx_(xx_parser, XX_BITWISE_AND, NULL, parser_status);
				break;
			case XX_T_BITWISE_OR:
				xx_(xx_parser, XX_BITWISE_OR, NULL, parser_status);
				break;

			case XX_T_INTEGER:
				xx_parse_with_token(xx_parser, XX_T_INTEGER, XX_INTEGER, &token, parser_status);
				break;
			case XX_T_DOUBLE:
				xx_parse_with_token(xx_parser, XX_T_DOUBLE, XX_DOUBLE, &token, parser_status);
				break;
			case XX_T_STRING:
				xx_parse_with_token(xx_parser, XX_T_STRING, XX_STRING, &token, parser_status);
				break;
			case XX_T_CHAR:
				xx_parse_with_token(xx_parser, XX_T_CHAR, XX_CHAR, &token, parser_status);
				break;
			case XX_T_IDENTIFIER:
				xx_parse_with_token(xx_parser, XX_T_IDENTIFIER, XX_IDENTIFIER, &token, parser_status);
				break;
			case XX_T_CONSTANT:
				xx_parse_with_token(xx_parser, XX_T_CONSTANT, XX_CONSTANT, &token, parser_status);
				break;

			case XX_T_VOID:
				xx_(xx_parser, XX_VOID, NULL, parser_status);
				break;
			case XX_T_LIKELY:
				xx_(xx_parser, XX_LIKELY, NULL, parser_status);
				break;
			case XX_T_UNLIKELY:
				xx_(xx_parser, XX_UNLIKELY, NULL, parser_status);
				break;

			default:
				parser_status->status = XX_PARSING_FAILED;
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

		if (parser_status->status != XX_PARSING_OK) {
			status = FAILURE;
			break;
		}

		state->end = state->start;
	}

	if (status != FAILURE) {
		switch (scanner_status) {
			case XX_SCANNER_RETCODE_ERR:
			case XX_SCANNER_RETCODE_IMPOSSIBLE:
				{
					char *x = malloc(sizeof(char) * 10000);
					if (state->start) {
						sprintf(x, "Scanner error: %d %s", scanner_status, state->start);
					} else {
						sprintf(x, "Scanner error: %d", scanner_status);
					}
					json_object *syntax_error = json_object_new_object();
					json_object_object_add(syntax_error, "type", json_object_new_string("error"));
					json_object_object_add(syntax_error, "message", json_object_new_string(x));
					//free(x);
					parser_status->ret = syntax_error;
					status = FAILURE;
				}
				break;
			default:
				xx_(xx_parser, 0, NULL, parser_status);
		}
	}

	state->active_token = 0;
	state->start = NULL;

	if (parser_status->status != XX_PARSING_OK) {
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

	xx_Free(xx_parser, xx_wrapper_free);

	if (status != FAILURE) {
		if (parser_status->status == XX_PARSING_OK) {
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
		printf("%s\n", json_object_to_json_string(parser_status->ret));
	}

	//efree(Z_STRVAL(processed_comment));*/

	free(parser_status);
	free(state);

	return status;
}

int main(int argc, char **argv) {

	FILE *fp;
	char ch;
	char *program;
	int i, length;

	if (argc > 0) {

		fp = fopen(argv[1], "r");
		if (!fp) {
			fprintf(stderr, "Cant open file\n");
			exit(1);
		}

		length = 1024;
		program = malloc(sizeof(char) * length);

		i = 0;
		while (!feof(fp)) {
			ch = fgetc(fp);
			if (i == length) {
				length += 1024;
				program = realloc(program, sizeof(char) * length);
			}
			program[i++] = ch;
		}
		program[i - 1] = '\0';
		fclose(fp);

		xx_parse_program(program, i - 1, argv[1]);

		free(program);
	}

}
