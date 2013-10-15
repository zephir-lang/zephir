/** The author disclaims copyright to this source code.
*/
/* First off, code is include which follows the "include" declaration
** in the input file. */
#include <stdio.h>
// 51 "parser.lemon"


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

static json_object *xx_ret_mcall(int type, json_object *O, xx_parser_token *M, json_object *parameters, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("mcall"));
	json_object_object_add(ret, "variable", O);
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

static json_object *xx_ret_cblock(xx_parser_token *T, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("cblock"));
	json_object_object_add(ret, "value", json_object_new_string(T->token));

	json_object_object_add(ret, "file", json_object_new_string(state->active_file));
	json_object_object_add(ret, "line", json_object_new_int(state->active_line));
	json_object_object_add(ret, "char", json_object_new_int(state->active_char));

	return ret;
}


// 937 "parser.c"
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
#define YYNOCODE 190
#define YYACTIONTYPE unsigned short int
#define xx_TOKENTYPE xx_parser_token*
typedef union {
  xx_TOKENTYPE yy0;
  json_object* yy259;
  int yy379;
} YYMINORTYPE;
#define YYSTACKDEPTH 100
#define xx_ARG_SDECL xx_parser_status *status;
#define xx_ARG_PDECL ,xx_parser_status *status
#define xx_ARG_FETCH xx_parser_status *status = yypParser->status
#define xx_ARG_STORE yypParser->status = status
#define YYNSTATE 688
#define YYNRULE 330
#define YYERRORSYMBOL 105
#define YYERRSYMDT yy379
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
 /*     0 */   217,  226,  229,  220,  223,  117,  637,   32,   56,   35,
 /*    10 */    37,   38,  268, 1019,    1,    2,  687,    4,    5,    6,
 /*    20 */     7,    8,  481,  478,  213,  276,  281,  295,  210,  431,
 /*    30 */   291,   64,  133,  272,  169,  135,  656,  662,  661,  645,
 /*    40 */    23,  302,  202,  525,  655,  611,  290,   22,  262,  636,
 /*    50 */   120,  120,  488,  497,  506,  509,  500,  503,  512,  518,
 /*    60 */   515,  115,  521,  170,  172,  174,  657,  183,  660,  661,
 /*    70 */   645,  194,  198,  203,  384,  610,  611,  391,   10,  460,
 /*    80 */   449,  120,  403,  284,  451,  448,  453,  457,  472,  476,
 /*    90 */   485,  283,  285,  286,  287,  288,  289,  526,  217,  226,
 /*   100 */   229,  220,  223,  244,  246,  254,  248,  250,  252,  294,
 /*   110 */   268,   58,  489,  496,  459,  241,  237,   80,   88,  562,
 /*   120 */    94,  188,  213,  276,  281,  295,  210,  596,  291,  143,
 /*   130 */   998,  272,  169,   80,   88,  609,   94,  138,  688,  302,
 /*   140 */   589,  525,  610,  611,  290,  273,  262,  190,  120,  551,
 /*   150 */   488,  497,  506,  509,  500,  503,  512,  518,  515,  533,
 /*   160 */   521,  170,  172,  174,  452,  183,  658,  623,  645,  194,
 /*   170 */   198,  203,  384,  627,  611,  391,  491,  630,  397,  120,
 /*   180 */   633,  284,  491,  628,  633,  457,  472,  476,  485,  283,
 /*   190 */   285,  286,  287,  288,  289,  526,  217,  226,  229,  220,
 /*   200 */   223,  591,  407,   32,   56,   35,   37,   38,  268,  639,
 /*   210 */   421,  490,  398,  399,  400,  401,  402,  499,  634,  640,
 /*   220 */   213,  276,  281,  295,  210,  114,  291,  498,  496,  272,
 /*   230 */   169,   80,   88,  116,   94,  386,  119,  302,  545,  525,
 /*   240 */    80,   88,  290,   65,  262,  635,  642,  491,  488,  497,
 /*   250 */   506,  509,  500,  503,  512,  518,  515,   11,  521,  170,
 /*   260 */   172,  174,  208,  183,  647,  383,  397,  194,  198,  203,
 /*   270 */   384,  633,   13,  391,  648,  309,  311,  310,  280,  284,
 /*   280 */   501,  496,  502,  457,  472,  476,  485,  283,  285,  286,
 /*   290 */   287,  288,  289,  526,  217,  226,  229,  220,  223,  462,
 /*   300 */   398,  399,  400,  401,  402,  422,  268,  458,  463,  309,
 /*   310 */   311,  310,  280,   34,   35,   37,   38,  186,  213,  276,
 /*   320 */   281,  295,  210,  128,  291,   14,  131,  272,  169,  248,
 /*   330 */   250,  252,  504,  496,  663,  302,  146,  525,  241,  237,
 /*   340 */   290,   18,  262,  190,  491,  551,  488,  497,  506,  509,
 /*   350 */   500,  503,  512,  518,  515,  426,  521,  170,  172,  174,
 /*   360 */   434,  183,  129,  568,  666,  194,  198,  203,  384,   80,
 /*   370 */    88,  391,   94,  674,  397,  677,  131,  284,  131,  505,
 /*   380 */   491,  457,  472,  476,  485,  283,  285,  286,  287,  288,
 /*   390 */   289,  526,  217,  226,  229,  220,  223,  603,   16,   32,
 /*   400 */    56,   35,   37,   38,  268,  507,  496,  491,  398,  399,
 /*   410 */   400,  401,  402,  510,  496,  508,  213,  276,  281,  295,
 /*   420 */   210,  678,  291,  666,  274,  272,  169,   20,  513,  496,
 /*   430 */   138,   17,   23,  302,  178,  525,  137,  352,  290,  121,
 /*   440 */   262,  120,  511,  120,  488,  497,  506,  509,  500,  503,
 /*   450 */   512,  518,  515,  650,  521,  170,  172,  174,  175,  183,
 /*   460 */   633,  383,   15,  194,  198,  203,  384,   18,  491,  391,
 /*   470 */    95,  309,  311,  310,  280,  284,  516,  496,  491,  457,
 /*   480 */   472,  476,  485,  283,  285,  286,  287,  288,  289,  526,
 /*   490 */   217,  226,  229,  220,  223,  519,  496,  608,  136,  623,
 /*   500 */   645,  378,  268,  514,  491,  655,  611,  125,  581,  667,
 /*   510 */   301,  120,  131,  517,  213,  276,  281,  295,  210,  120,
 /*   520 */   291,  522,  496,  272,  169,  491,  392,  456,   25,   84,
 /*   530 */   394,  302,  555,  525,   81,  668,  290,  666,  262,  520,
 /*   540 */   367,   26,  488,  497,  506,  509,  500,  503,  512,  518,
 /*   550 */   515,  380,  521,  170,  172,  174,  342,  183,  376,  462,
 /*   560 */   523,  194,  198,  203,  384,  393,  377,  391,  461,  309,
 /*   570 */   311,  310,  280,  284,  357,  187,  554,  457,  472,  476,
 /*   580 */   485,  283,  285,  286,  287,  288,  289,  526,  217,  226,
 /*   590 */   229,  220,  223,  405,  684,   30,  301,  433,  301,  131,
 /*   600 */   268,  574,  314,  309,  311,  310,  280,  301,  341,   33,
 /*   610 */   319,  570,  213,  276,  281,  295,  210,   60,  291,  301,
 /*   620 */    63,  272,  169,   36,  363,  590,  361,  625,  354,  302,
 /*   630 */   579,  525,   43,  301,  290,  586,  262,  350,  120,   41,
 /*   640 */   488,  497,  506,  509,  500,  503,  512,  518,  515,  335,
 /*   650 */   521,  170,  172,  174,  184,  183,   42,  383,   40,  194,
 /*   660 */   198,  203,  384,  333,   64,  391,   64,  309,  311,  310,
 /*   670 */   280,  284,  305,  241,  237,  457,  472,  476,  485,  283,
 /*   680 */   285,  286,  287,  288,  289,  526,  217,  226,  229,  220,
 /*   690 */   223,  405,  301,   44,   59,  437,  569,  294,  268,  602,
 /*   700 */    62,  309,  311,  310,  280,   64,  301,   67,  313,  598,
 /*   710 */   213,  276,  281,  295,  210,   28,  291,  301,   57,  272,
 /*   720 */   169,   68,  324,  296,  301,  216,  375,  302,  593,  525,
 /*   730 */    77,  327,  290,  273,  262,  597,  307,   78,  488,  497,
 /*   740 */   506,  509,  500,  503,  512,  518,  515,  317,  521,  170,
 /*   750 */   172,  174,  547,  183,  300,  383,  238,  194,  198,  203,
 /*   760 */   384,  639,   85,  391,  356,  309,  311,  310,  280,  284,
 /*   770 */    82,  640,   83,  457,  472,  476,  485,  283,  285,  286,
 /*   780 */   287,  288,  289,  526,  217,  226,  229,  220,  223,  405,
 /*   790 */    86,  110,  105,  425,  113,   87,  268,   89,   90,  309,
 /*   800 */   311,  310,  280,  120,   91,   93,   41,  107,  213,  276,
 /*   810 */   281,  295,  210,  108,  291,  112,  118,  272,  169,  123,
 /*   820 */   124,  126,  127,  665,  130,  302,  577,  525,  140,  134,
 /*   830 */   290,  141,  262,  171,  173,  176,  488,  497,  506,  509,
 /*   840 */   500,  503,  512,  518,  515,  179,  521,  170,  172,  174,
 /*   850 */   199,  183,  180,  383,  185,  194,  198,  203,  384,  189,
 /*   860 */   191,  391,  195,  309,  311,  310,  280,  284,  200,  204,
 /*   870 */   192,  457,  472,  476,  485,  283,  285,  286,  287,  288,
 /*   880 */   289,  526,  217,  226,  229,  220,  223,   61,  207,   32,
 /*   890 */    56,   35,   37,   38,  268,   29,  209,   32,   56,   35,
 /*   900 */    37,   38,  215,  239,  266,  214,  213,  276,  281,  295,
 /*   910 */   210,  269,  291,  270,  275,  272,  169,  277,  278,  279,
 /*   920 */   297,  303,  304,  302,  595,  525,  320,  315,  290,  321,
 /*   930 */   262,  322,  328,  329,  488,  497,  506,  509,  500,  503,
 /*   940 */   512,  518,  515,  330,  521,  170,  172,  174,  358,  183,
 /*   950 */   473,  359,  364,  194,  198,  203,  384,  365,  379,  391,
 /*   960 */   309,  311,  310,  280,  381,  284, 1000,  999,  385,  457,
 /*   970 */   472,  476,  485,  283,  285,  286,  287,  288,  289,  526,
 /*   980 */   217,  226,  229,  220,  223,  405,  345,  396,  415,  409,
 /*   990 */   395,  423,  268,  408,  412,  309,  311,  310,  280,  416,
 /*  1000 */   424,  427,  428,  432,  213,  276,  281,  295,  210,  436,
 /*  1010 */   291,  439,  440,  272,  169,  444,  447,  465,  467,  469,
 /*  1020 */   471,  302,  182,  525,  477,  479,  290,  480,  262,  484,
 /*  1030 */   493,  492,  488,  497,  506,  509,  500,  503,  512,  518,
 /*  1040 */   515,  534,  521,  170,  172,  174,  494,  183,  486,  495,
 /*  1050 */   535,  194,  198,  203,  384,  548,  558,  391,  309,  311,
 /*  1060 */   310,  280,  546,  284,  557,  552,  564,  457,  472,  476,
 /*  1070 */   485,  283,  285,  286,  287,  288,  289,  526,  217,  226,
 /*  1080 */   229,  220,  223,  563,  576,   32,   56,   35,   37,   38,
 /*  1090 */   268,  111,  583,   32,   56,   35,   37,   38,  584,  592,
 /*  1100 */   614,  604,  213,  276,  281,  295,  210,  615,  291,  617,
 /*  1110 */   619,  272,  169,  616,  620,  622,  632,  735,  621,  302,
 /*  1120 */   599,  525,  736,  643,  290,  629,  262,  631,  641,  653,
 /*  1130 */   488,  497,  506,  509,  500,  503,  512,  518,  515,  638,
 /*  1140 */   521,  170,  172,  174,  644,  183,  221,  649,  652,  194,
 /*  1150 */   198,  203,  384,  651,  664,  391,  309,  311,  310,  280,
 /*  1160 */   654,  284,  669,  672,  671,  457,  472,  476,  485,  283,
 /*  1170 */   285,  286,  287,  288,  289,  526,  217,  226,  229,  220,
 /*  1180 */   223,  405,  673,  675,  131,  417,  676,  682,  268,  679,
 /*  1190 */   681,  309,  311,  310,  280,  683,  685,  521,  686,  521,
 /*  1200 */   213,  276,  281,  295,  210,  521,  291,  521,  521,  272,
 /*  1210 */   169,  521,  521,  521,  521,  521,  521,  302,  573,  525,
 /*  1220 */   521,  521,  290,  521,  262,  521,  521,  521,  488,  497,
 /*  1230 */   506,  509,  500,  503,  512,  518,  515,  521,  521,  170,
 /*  1240 */   172,  174,  521,  183,  227,  521,  521,  194,  198,  203,
 /*  1250 */   384,  521,  521,  391,  309,  311,  310,  280,  521,  284,
 /*  1260 */   521,  521,  521,  457,  472,  476,  485,  283,  285,  286,
 /*  1270 */   287,  288,  289,  526,  217,  226,  229,  220,  223,  405,
 /*  1280 */   521,  521,  521,  445,  521,  521,  268,  521,  521,  309,
 /*  1290 */   311,  310,  280,  521,  521,  521,  521,  521,  213,  276,
 /*  1300 */   281,  295,  210,  521,  291,  521,  521,  272,  169,  521,
 /*  1310 */   521,  521,  521,  521,  521,  302,  601,  525,  521,  521,
 /*  1320 */   290,  521,  262,  521,  521,  521,  488,  497,  506,  509,
 /*  1330 */   500,  503,  512,  518,  515,  521,  521,  170,  172,  174,
 /*  1340 */   521,  183,  255,  521,  521,  194,  198,  203,  384,  521,
 /*  1350 */   521,  391,  309,  311,  310,  280,  521,  284,  521,  521,
 /*  1360 */   521,  457,  472,  476,  485,  283,  285,  286,  287,  288,
 /*  1370 */   289,  526,  217,  226,  229,  220,  223,  405,  521,  521,
 /*  1380 */   521,  429,  521,  521,  268,  521,  521,  309,  311,  310,
 /*  1390 */   280,  521,  521,  521,  521,  521,  213,  276,  281,  295,
 /*  1400 */   210,  521,  291,  521,  521,  272,  169,  521,  521,  521,
 /*  1410 */   521,  521,  521,  302,  571,  525,  521,  521,  290,  521,
 /*  1420 */   262,  521,  521,  521,  488,  497,  506,  509,  500,  503,
 /*  1430 */   512,  518,  515,  521,  521,  170,  172,  174,  521,  183,
 /*  1440 */   257,  521,  521,  194,  198,  203,  384,  521,  521,  391,
 /*  1450 */   309,  311,  310,  280,  521,  284,  521,  521,  521,  457,
 /*  1460 */   472,  476,  485,  283,  285,  286,  287,  288,  289,  526,
 /*  1470 */   217,  226,  229,  220,  223,  405,  521,  521,  521,  450,
 /*  1480 */   521,  521,  268,  521,  521,  309,  311,  310,  280,  521,
 /*  1490 */   521,  521,  521,  521,  213,  276,  281,  295,  210,  521,
 /*  1500 */   291,  521,  521,  272,  169,  521,  521,  521,  521,  521,
 /*  1510 */   521,  302,  556,  525,  521,  521,  290,  521,  262,  521,
 /*  1520 */   521,  521,  488,  497,  506,  509,  500,  503,  512,  518,
 /*  1530 */   515,  521,  521,  170,  172,  174,  521,  183,  253,  521,
 /*  1540 */   521,  194,  198,  203,  384,  521,  521,  391,  309,  311,
 /*  1550 */   310,  280,  521,  284,  521,  521,  521,  457,  472,  476,
 /*  1560 */   485,  283,  285,  286,  287,  288,  289,  526,  217,  226,
 /*  1570 */   229,  220,  223,  418,  521,  521,  521,  521,  446,  521,
 /*  1580 */   268,  521,  521,  309,  311,  310,  280,  521,  521,  521,
 /*  1590 */   521,  521,  213,  276,  281,  295,  210,  521,  291,  521,
 /*  1600 */   521,  272,  169,  521,  521,  521,  521,  521,  521,  302,
 /*  1610 */   605,  525,  521,  521,  290,  521,  262,  521,  521,  521,
 /*  1620 */   488,  497,  506,  509,  500,  503,  512,  518,  515,  521,
 /*  1630 */   521,  170,  172,  174,  521,  183,  271,  521,  521,  194,
 /*  1640 */   198,  203,  384,  521,  521,  391,  309,  311,  310,  280,
 /*  1650 */   521,  284,  521,  521,  521,  457,  472,  476,  485,  283,
 /*  1660 */   285,  286,  287,  288,  289,  526,  217,  226,  229,  220,
 /*  1670 */   223,  405,  521,  521,  521,  441,  521,  521,  268,  521,
 /*  1680 */   521,  309,  311,  310,  280,  521,  521,  521,  521,  521,
 /*  1690 */   213,  276,  281,  295,  210,  521,  291,  521,  521,  272,
 /*  1700 */   169,  521,  521,  521,  521,  521,  521,  302,  607,  525,
 /*  1710 */   521,  521,  290,  521,  262,  521,  521,  521,  488,  497,
 /*  1720 */   506,  509,  500,  503,  512,  518,  515,  521,  521,  170,
 /*  1730 */   172,  174,  521,  183,  251,  521,  521,  194,  198,  203,
 /*  1740 */   384,  521,  521,  391,  309,  311,  310,  280,  521,  284,
 /*  1750 */   521,  521,  521,  457,  472,  476,  485,  283,  285,  286,
 /*  1760 */   287,  288,  289,  526,  217,  226,  229,  220,  223,    3,
 /*  1770 */     4,    5,    6,    7,    8,  575,  268,   32,   56,   35,
 /*  1780 */    37,   38,  521,  521,  521,  521,  521,  521,  213,  276,
 /*  1790 */   281,  295,  210,  521,  291,  521,  521,  272,  169,  521,
 /*  1800 */   521,  521,  521,  521,  521,  302,  539,  525,  521,  521,
 /*  1810 */   290,  521,  262,  521,  521,  521,  488,  497,  506,  509,
 /*  1820 */   500,  503,  512,  518,  515,  521,  521,  170,  172,  174,
 /*  1830 */   521,  183,  249,  521,  521,  194,  198,  203,  384,  521,
 /*  1840 */   521,  391,  309,  311,  310,  280,  521,  284,  521,  521,
 /*  1850 */   521,  457,  472,  476,  485,  283,  285,  286,  287,  288,
 /*  1860 */   289,  526,  217,  226,  229,  220,  223,  405,  521,  521,
 /*  1870 */   521,  413,  521,  521,  268,  521,  521,  309,  311,  310,
 /*  1880 */   280,  521,  521,  521,  521,  521,  213,  276,  281,  295,
 /*  1890 */   210,  521,  291,  521,  521,  272,  169,  521,  521,  521,
 /*  1900 */   521,  521,  521,  302,  567,  525,  521,  521,  290,  521,
 /*  1910 */   262,  521,  521,  521,  488,  497,  506,  509,  500,  503,
 /*  1920 */   512,  518,  515,  521,  521,  170,  172,  174,  521,  183,
 /*  1930 */   259,  521,  521,  194,  198,  203,  384,  521,  521,  391,
 /*  1940 */   309,  311,  310,  280,  521,  284,  521,  521,  521,  457,
 /*  1950 */   472,  476,  485,  283,  285,  286,  287,  288,  289,  526,
 /*  1960 */   217,  226,  229,  220,  223,  418,  521,  521,  521,  521,
 /*  1970 */   438,  521,  268,  521,  521,  309,  311,  310,  280,  521,
 /*  1980 */   521,  521,  521,  521,  213,  276,  281,  295,  210,  521,
 /*  1990 */   291,  521,  521,  272,  169,  521,  521,  521,  521,  521,
 /*  2000 */   521,  302,  587,  525,  521,  521,  290,  521,  262,  521,
 /*  2010 */   521,  521,  488,  497,  506,  509,  500,  503,  512,  518,
 /*  2020 */   515,  521,  521,  170,  172,  174,  521,  183,  247,  521,
 /*  2030 */   521,  194,  198,  203,  384,  521,  521,  391,  309,  311,
 /*  2040 */   310,  280,  521,  284,  521,  521,  521,  457,  472,  476,
 /*  2050 */   485,  283,  285,  286,  287,  288,  289,  526,  217,  226,
 /*  2060 */   229,  220,  223,  405,  521,  521,  521,  404,  521,  521,
 /*  2070 */   268,  521,  521,  309,  311,  310,  280,  521,  521,  521,
 /*  2080 */   521,  521,  213,  276,  281,  295,  210,  521,  291,  521,
 /*  2090 */   521,  272,  169,  521,  521,  521,  521,  521,  521,  302,
 /*  2100 */   390,  525,  521,  521,  290,  521,  262,  521,  521,  521,
 /*  2110 */   488,  497,  506,  509,  500,  503,  512,  518,  515,  521,
 /*  2120 */   521,  170,  172,  174,  521,  183,  245,  521,  521,  194,
 /*  2130 */   198,  203,  384,  521,  265,  391,  309,  311,  310,  280,
 /*  2140 */   521,  284,  521,  521,  521,  457,  472,  476,  485,  283,
 /*  2150 */   285,  286,  287,  288,  289,  526,  217,  226,  229,  220,
 /*  2160 */   223,  263,  521,  521,  521,  521,  521,  521,  268,  521,
 /*  2170 */   521,  309,  311,  310,  280,  521,  521,  521,  521,  521,
 /*  2180 */   213,  276,  281,  295,  210,  521,  291,  521,  521,  272,
 /*  2190 */   169,  521,  521,  521,  521,  521,  521,  302,  144,  525,
 /*  2200 */   521,  521,  290,  521,  262,  521,  521,  521,  488,  497,
 /*  2210 */   506,  509,  500,  503,  512,  518,  515,  521,  521,  170,
 /*  2220 */   172,  174,  521,  183,  261,  521,  521,  194,  198,  203,
 /*  2230 */   384,  521,  521,  391,  309,  311,  310,  280,  521,  284,
 /*  2240 */   521,  521,  521,  457,  472,  476,  485,  283,  285,  286,
 /*  2250 */   287,  288,  289,  526,  217,  226,  229,  220,  223,  418,
 /*  2260 */   521,  521,  521,  521,  414,  521,  268,  521,  521,  309,
 /*  2270 */   311,  310,  280,  521,  521,  521,  521,  521,  213,  276,
 /*  2280 */   281,  295,  210,  521,  291,  521,  521,  272,  169,  521,
 /*  2290 */   521,  521,  521,  521,  521,  302,  206,  525,  521,  521,
 /*  2300 */   290,  521,  262,  521,  521,  521,  488,  497,  506,  509,
 /*  2310 */   500,  503,  512,  518,  515,  521,  521,  170,  172,  174,
 /*  2320 */   521,  183,  242,  521,  521,  194,  198,  203,  384,  521,
 /*  2330 */   521,  391,  309,  311,  310,  280,  521,  284,  521,  521,
 /*  2340 */   521,  457,  472,  476,  485,  283,  285,  286,  287,  288,
 /*  2350 */   289,  526,  217,  226,  229,  220,  223,  370,  521,  521,
 /*  2360 */   521,  521,  521,  521,  268,  521,  521,  309,  311,  310,
 /*  2370 */   280,  521,  521,  521,  521,  521,  213,  276,  281,  295,
 /*  2380 */   210,  521,  291,  521,  521,  272,  169,  521,  521,  521,
 /*  2390 */   521,  521,  521,  302,  550,  525,  521,  521,  290,  521,
 /*  2400 */   262,  521,  521,  521,  488,  497,  506,  509,  500,  503,
 /*  2410 */   512,  518,  515,  521,  521,  170,  172,  174,  521,  183,
 /*  2420 */   371,  521,  521,  194,  198,  203,  384,  521,  521,  391,
 /*  2430 */   309,  311,  310,  280,  521,  284,  521,  521,  521,  457,
 /*  2440 */   472,  476,  485,  283,  285,  286,  287,  288,  289,  526,
 /*  2450 */   217,  226,  229,  220,  223,  236,  521,  521,  521,  521,
 /*  2460 */   521,  521,  268,  521,  521,  309,  311,  310,  280,  521,
 /*  2470 */   521,  521,  521,  521,  213,  276,  281,  295,  210,  521,
 /*  2480 */   291,  521,  521,  272,  169,  521,  521,  521,  521,  521,
 /*  2490 */   521,  302,  565,  525,  521,  521,  290,  521,  262,  521,
 /*  2500 */   521,  521,  488,  497,  506,  509,  500,  503,  512,  518,
 /*  2510 */   515,  521,  521,  170,  172,  174,  521,  183,  267,  521,
 /*  2520 */   521,  194,  198,  203,  384,  521,  521,  391,  309,  311,
 /*  2530 */   310,  280,  521,  284,  521,  521,  521,  457,  472,  476,
 /*  2540 */   485,  283,  285,  286,  287,  288,  289,  526,  217,  226,
 /*  2550 */   229,  220,  223,  211,  521,  521,  521,  521,  521,  521,
 /*  2560 */   268,  521,  521,  309,  311,  310,  280,  521,  521,  521,
 /*  2570 */   521,  521,  213,  276,  281,  295,  210,  521,  291,  521,
 /*  2580 */   521,  272,  169,  521,  521,  521,  521,  521,  521,  302,
 /*  2590 */   197,  525,  521,  521,  290,  521,  262,  521,  521,  521,
 /*  2600 */   488,  497,  506,  509,  500,  503,  512,  518,  515,  521,
 /*  2610 */   521,  170,  172,  174,  521,  183,  369,  521,  521,  194,
 /*  2620 */   198,  203,  384,  521,  521,  391,  309,  311,  310,  280,
 /*  2630 */   521,  284,  521,  521,  521,  457,  472,  476,  485,  283,
 /*  2640 */   285,  286,  287,  288,  289,  526,  217,  226,  229,  220,
 /*  2650 */   223,  282,  521,  521,  521,  521,  521,  521,  268,  521,
 /*  2660 */   521,  309,  311,  310,  280,  521,  521,  521,  521,  521,
 /*  2670 */   213,  276,  281,  295,  210,  521,  291,  521,  521,  272,
 /*  2680 */   169,  521,  521,  521,  521,  521,  521,  302,  549,  525,
 /*  2690 */   521,  521,  290,  521,  262,  521,  521,  521,  488,  497,
 /*  2700 */   506,  509,  500,  503,  512,  518,  515,  521,  521,  170,
 /*  2710 */   172,  174,  521,  183,  382,  521,  521,  194,  198,  203,
 /*  2720 */   384,  521,  521,  391,  309,  311,  310,  280,  521,  284,
 /*  2730 */   521,  521,  521,  457,  472,  476,  485,  283,  285,  286,
 /*  2740 */   287,  288,  289,  526,  217,  226,  229,  220,  223,  234,
 /*  2750 */   521,  521,  521,  521,  521,  521,  268,  521,  521,  309,
 /*  2760 */   311,  310,  280,  521,  521,  521,  521,  521,  213,  276,
 /*  2770 */   281,  295,  210,  521,  291,  521,  521,  272,  169,  521,
 /*  2780 */   521,  521,  521,  521,  521,  302,  532,  525,  521,  521,
 /*  2790 */   290,  521,  262,  521,  521,  521,  488,  497,  506,  509,
 /*  2800 */   500,  503,  512,  518,  515,  521,  521,  170,  172,  174,
 /*  2810 */   521,  183,  529,  521,  521,  194,  198,  203,  384,  521,
 /*  2820 */   521,  391,  309,  311,  310,  280,  521,  284,  521,  521,
 /*  2830 */   521,  457,  472,  476,  485,  283,  285,  286,  287,  288,
 /*  2840 */   289,  526,  217,  226,  229,  220,  223,  373,  521,  521,
 /*  2850 */   521,  521,  521,  521,  268,  521,  521,  309,  311,  310,
 /*  2860 */   280,  521,  521,  521,  521,  521,  213,  276,  281,  295,
 /*  2870 */   210,  521,  291,  521,  521,  272,  169,  521,  521,  521,
 /*  2880 */   521,  521,  521,  302,  560,  525,  521,  521,  290,  521,
 /*  2890 */   262,  521,  521,  521,  488,  497,  506,  509,  500,  503,
 /*  2900 */   512,  518,  515,  521,  521,  170,  172,  174,  521,  183,
 /*  2910 */   232,  521,  521,  194,  198,  203,  384,  521,  521,  391,
 /*  2920 */   309,  311,  310,  280,  521,  284,  521,  521,  521,  457,
 /*  2930 */   472,  476,  485,  283,  285,  286,  287,  288,  289,  526,
 /*  2940 */   217,  226,  229,  220,  223,  218,  521,  521,  521,  521,
 /*  2950 */   521,  521,  268,  521,  521,  309,  311,  310,  280,  521,
 /*  2960 */   521,  521,  521,  521,  213,  276,  281,  295,  210,  521,
 /*  2970 */   291,  521,  521,  272,  169,  521,  521,  521,  521,  521,
 /*  2980 */   521,  302,  561,  525,  521,  521,  290,  521,  262,  521,
 /*  2990 */   521,  521,  488,  497,  506,  509,  500,  503,  512,  518,
 /*  3000 */   515,  521,  521,  170,  172,  174,  521,  183,  230,  521,
 /*  3010 */   521,  194,  198,  203,  384,  521,  521,  391,  309,  311,
 /*  3020 */   310,  280,  521,  284,  521,  521,  521,  457,  472,  476,
 /*  3030 */   485,  283,  285,  286,  287,  288,  289,  526,  217,  226,
 /*  3040 */   229,  220,  223,  536,  521,  521,  521,  521,  521,  521,
 /*  3050 */   268,  521,  521,  309,  311,  310,  280,  521,  521,  521,
 /*  3060 */   521,  521,  213,  276,  281,  295,  210,  521,  291,  521,
 /*  3070 */   521,  272,  169,  521,  521,  521,  521,  521,  521,  302,
 /*  3080 */   544,  525,  521,  521,  290,  521,  262,  521,  521,  521,
 /*  3090 */   488,  497,  506,  509,  500,  503,  512,  518,  515,  521,
 /*  3100 */   521,  170,  172,  174,  521,  183,  387,  521,  521,  194,
 /*  3110 */   198,  203,  384,  521,  521,  391,  309,  311,  310,  280,
 /*  3120 */   521,  284,  521,  521,  521,  457,  472,  476,  485,  283,
 /*  3130 */   285,  286,  287,  288,  289,  526,  217,  226,  229,  220,
 /*  3140 */   223,  541,  521,  521,  521,  521,  521,  521,  268,  521,
 /*  3150 */   521,  309,  311,  310,  280,  521,  521,  521,  521,  521,
 /*  3160 */   213,  276,  281,  295,  210,  521,  291,  521,  521,  272,
 /*  3170 */   169,  521,  521,  521,  521,  521,  521,  302,  521,  525,
 /*  3180 */   521,  521,  290,  521,  262,  521,  521,  521,  488,  497,
 /*  3190 */   506,  509,  500,  503,  512,  518,  515,  521,  521,  170,
 /*  3200 */   172,  174,  521,  183,  372,  521,  521,  194,  198,  203,
 /*  3210 */   384,  521,  521,  391,  309,  311,  310,  280,  521,  284,
 /*  3220 */   521,  521,  521,  457,  472,  476,  485,  283,  285,  286,
 /*  3230 */   287,  288,  289,  526,  217,  226,  229,  220,  223,  521,
 /*  3240 */     9,  521,  521,   12,  521,  122,  268,  670,  680,  521,
 /*  3250 */   521,  525,  521,  521,  521,  521,  521,  521,  213,  276,
 /*  3260 */   281,  295,  210,  521,  291,  521,   39,  272,  521,  521,
 /*  3270 */   521,  521,  521,  521,  521,  302,  521,  521,  521,  521,
 /*  3280 */   290,  521,  262,  521,  521,  521,   45,   46,   47,   48,
 /*  3290 */    49,   50,   51,   52,   53,   54,   55,  521,  521,  521,
 /*  3300 */   521,  521,  167,  148,   31,  526,   45,   46,   47,   48,
 /*  3310 */    49,   50,   51,   52,   53,   54,   55,  284,  521,  521,
 /*  3320 */   521,  521,  521,  531,  521,  283,  285,  286,  287,  288,
 /*  3330 */   289,  521,  527,  149,  150,  151,  152,  153,  154,  155,
 /*  3340 */   156,  157,  158,  159,  160,  161,  162,  163,  164,  165,
 /*  3350 */   166,  168,  167,  148,  521,  524,  521,  521,  521,  292,
 /*  3360 */   521,  521,  521,  521,  521,  464,  466,  468,  470,  309,
 /*  3370 */   311,  310,  280,  193,  521,   96,   97,   99,   98,  100,
 /*  3380 */   521,  346,  527,  149,  150,  151,  152,  153,  154,  155,
 /*  3390 */   156,  157,  158,  159,  160,  161,  162,  163,  164,  165,
 /*  3400 */   166,  168,  167,  148,  521,  524,  521,  521,  521,  521,
 /*  3410 */   521,  521,  521,  521,  646,  464,  466,  468,  470,  521,
 /*  3420 */   102,  103,  521,  145,  521,  521,  521,  521,  139,  521,
 /*  3430 */   101,  521,  527,  149,  150,  151,  152,  153,  154,  155,
 /*  3440 */   156,  157,  158,  159,  160,  161,  162,  163,  164,  165,
 /*  3450 */   166,  168,  167,  148,  521,  524,  521,  521,  521,  224,
 /*  3460 */   521,  521,  521,  521,  521,  464,  466,  468,  470,  309,
 /*  3470 */   311,  310,  280,  572,  521,   96,   97,   99,   98,  100,
 /*  3480 */   521,  521,  527,  149,  150,  151,  152,  153,  154,  155,
 /*  3490 */   156,  157,  158,  159,  160,  161,  162,  163,  164,  165,
 /*  3500 */   166,  168,  167,  148,  521,  524,  521,  521,  521,  521,
 /*  3510 */   521,  521,  521,  521,  626,  464,  466,  468,  470,  521,
 /*  3520 */   102,  103,  521,  588,  521,  521,  521,  521,  582,  521,
 /*  3530 */   101,  521,  527,  149,  150,  151,  152,  153,  154,  155,
 /*  3540 */   156,  157,  158,  159,  160,  161,  162,  163,  164,  165,
 /*  3550 */   166,  168,  167,  148,  521,  524,  521,  521,  410,  521,
 /*  3560 */   521,  521,  521,  521,  521,  464,  466,  468,  470,  521,
 /*  3570 */   521,  521,  397,  543,  521,   96,   97,   99,   98,  100,
 /*  3580 */   521,  521,  527,  149,  150,  151,  152,  153,  154,  155,
 /*  3590 */   156,  157,  158,  159,  160,  161,  162,  163,  164,  165,
 /*  3600 */   166,  168,  167,  148,  521,  524,  398,  399,  400,  401,
 /*  3610 */   402,  521,  521,  419,  420,  464,  466,  468,  470,  521,
 /*  3620 */   102,  103,  521,  566,  521,  521,  521,  521,   24,  521,
 /*  3630 */   101,  521,  527,  149,  150,  151,  152,  153,  154,  155,
 /*  3640 */   156,  157,  158,  159,  160,  161,  162,  163,  164,  165,
 /*  3650 */   166,  168,  167,  148,  521,  524,  521,  521,  521,  482,
 /*  3660 */   521,  521,  521,  521,  521,  464,  466,  468,  470,  309,
 /*  3670 */   311,  310,  280,  538,  521,   96,   97,   99,   98,  100,
 /*  3680 */   521,  521,  527,  149,  150,  151,  152,  153,  154,  155,
 /*  3690 */   156,  157,  158,  159,  160,  161,  162,  163,  164,  165,
 /*  3700 */   166,  168,  167,  148,  521,  524,  521,  521,  521,  521,
 /*  3710 */   521,  521,  521,  521,  521,  464,  466,  468,  470,  521,
 /*  3720 */   102,  103,  521,  606,  521,  521,  521,  521,  582,  521,
 /*  3730 */   101,  521,  527,  149,  150,  151,  152,  153,  154,  155,
 /*  3740 */   156,  157,  158,  159,  160,  161,  162,  163,  164,  165,
 /*  3750 */   166,  168,  167,  148,  521,  524,  521,  521,  521,  521,
 /*  3760 */   521,  521,  521,  521,  521,  464,  466,  468,  470,  521,
 /*  3770 */   521,  521,  521,  578,  521,   96,   97,   99,   98,  100,
 /*  3780 */   521,  521,  527,  149,  150,  151,  152,  153,  154,  155,
 /*  3790 */   156,  157,  158,  159,  160,  161,  162,  163,  164,  165,
 /*  3800 */   166,  168,  167,  148,  521,  524,  521,  521,  521,  521,
 /*  3810 */   521,  521,  521,  521,  521,  464,  466,  468,  470,  521,
 /*  3820 */   102,  103,  521,  594,  521,  521,  521,  521,  106,  521,
 /*  3830 */   101,  521,  527,  149,  150,  151,  152,  153,  154,  155,
 /*  3840 */   156,  157,  158,  159,  160,  161,  162,  163,  164,  165,
 /*  3850 */   166,  168,  167,  148,  521,  524,  521,  521,  521,  521,
 /*  3860 */   521,  521,  521,  521,  521,  464,  466,  468,  470,  521,
 /*  3870 */   521,  521,  521,  177,  521,   96,   97,   99,   98,  100,
 /*  3880 */   521,  521,  527,  149,  150,  151,  152,  153,  154,  155,
 /*  3890 */   156,  157,  158,  159,  160,  161,  162,  163,  164,  165,
 /*  3900 */   166,  168,  167,  148,  521,  524,  521,  521,  521,  521,
 /*  3910 */   521,  521,  521,  521,  521,  464,  466,  468,  470,  521,
 /*  3920 */   102,  103,  521,  201,  521,  521,  521,  521,  139,  521,
 /*  3930 */   101,  521,  527,  149,  150,  151,  152,  153,  154,  155,
 /*  3940 */   156,  157,  158,  159,  160,  161,  162,  163,  164,  165,
 /*  3950 */   166,  168,  167,  148,  521,  524,  521,  521,  521,  521,
 /*  3960 */   521,  521,  521,  521,  521,  464,  466,  468,  470,  521,
 /*  3970 */   521,  521,  521,  205,  521,   96,   97,   99,   98,  100,
 /*  3980 */   521,  521,  527,  149,  150,  151,  152,  153,  154,  155,
 /*  3990 */   156,  157,  158,  159,  160,  161,  162,  163,  164,  165,
 /*  4000 */   166,  168,  167,  148,  521,  524,  521,  521,  521,  521,
 /*  4010 */   521,  521,  521,  521,  521,  464,  466,  468,  470,  521,
 /*  4020 */   102,  103,  521,  181,  521,  521,  613,  521,  521,  521,
 /*  4030 */   101,  521,  527,  149,  150,  151,  152,  153,  154,  155,
 /*  4040 */   156,  157,  158,  159,  160,  161,  162,  163,  164,  165,
 /*  4050 */   166,  168,  167,  148,  521,  524,  521,  521,  521,  292,
 /*  4060 */   521,  521,  521,  521,  521,  464,  466,  468,  470,  309,
 /*  4070 */   311,  310,  280,  553,  521,  521,  340,  521,  521,  348,
 /*  4080 */   344,  347,  527,  149,  150,  151,  152,  153,  154,  155,
 /*  4090 */   156,  157,  158,  159,  160,  161,  162,  163,  164,  165,
 /*  4100 */   166,  168,  167,  148,  521,  524,  521,  521,  521,  521,
 /*  4110 */   521,  521,  521,  521,  521,  464,  466,  468,  470,  521,
 /*  4120 */   521,  521,  521,  196,  521,   96,   97,   99,   98,  100,
 /*  4130 */   521,  521,  527,  149,  150,  151,  152,  153,  154,  155,
 /*  4140 */   156,  157,  158,  159,  160,  161,  162,  163,  164,  165,
 /*  4150 */   166,  168,  167,  148,  521,  524,  521,  521,  521,  521,
 /*  4160 */   521,  521,  521,  521,  521,  464,  466,  468,  470,  521,
 /*  4170 */   102,  103,  521,  389,  521,  521,  521,  521,  521,  521,
 /*  4180 */   101,  521,  527,  149,  150,  151,  152,  153,  154,  155,
 /*  4190 */   156,  157,  158,  159,  160,  161,  162,  163,  164,  165,
 /*  4200 */   166,  168,  167,  148,  521,  524,  521,  521,  521,  292,
 /*  4210 */   521,  521,  521,  521,  521,  464,  466,  468,  470,  309,
 /*  4220 */   311,  310,  280,  600,  521,  521,  521,  521,  521,  343,
 /*  4230 */   344,  347,  527,  149,  150,  151,  152,  153,  154,  155,
 /*  4240 */   156,  157,  158,  159,  160,  161,  162,  163,  164,  165,
 /*  4250 */   166,  168,  167,  148,  521,  524,  521,  521,  521,  521,
 /*  4260 */   521,  521,  521,  521,  521,  464,  466,  468,  470,  521,
 /*  4270 */   521,  521,  521,  559,  521,   96,   97,   99,   98,  100,
 /*  4280 */   521,  521,  527,  149,  150,  151,  152,  153,  154,  155,
 /*  4290 */   156,  157,  158,  159,  160,  161,  162,  163,  164,  165,
 /*  4300 */   166,  168,  167,  148,  521,  524,  521,  521,  521,  521,
 /*  4310 */   521,  521,  521,  521,  521,  464,  466,  468,  470,  521,
 /*  4320 */   102,  103,  521,  717,  624,  521,  618,  521,  521,  521,
 /*  4330 */   101,  521,  147,  149,  150,  151,  152,  153,  154,  155,
 /*  4340 */   156,  157,  158,  159,  160,  161,  162,  163,  164,  165,
 /*  4350 */   166,  168,  521,  521,  521,  524,  521,  217,  226,  229,
 /*  4360 */   220,  223,  521,  521,  521,  464,  466,  468,  470,  268,
 /*  4370 */   521,  521,  521,  521,  521,  521,  521,  521,  521,  521,
 /*  4380 */   521,  213,  276,  281,  295,  210,  318,  291,  521,  240,
 /*  4390 */   272,  521,  521,  521,  521,  521,  521,  521,  302,  309,
 /*  4400 */   311,  310,  280,  290,  521,  262,  521,  323,  312,  521,
 /*  4410 */   521,  260,  256,  258,  521,  521,  212,  222,  228,  231,
 /*  4420 */   233,  235,  225,  219,  244,  246,  254,  248,  250,  252,
 /*  4430 */   521,  521,  521,  521,  521,  264,  241,  237,  521,  521,
 /*  4440 */   284,  521,  521,  521,  521,  521,  521,  521,  283,  285,
 /*  4450 */   286,  287,  288,  289,  240,  521,  217,  226,  229,  220,
 /*  4460 */   223,  521,  521,  521,  309,  311,  310,  280,  268,  521,
 /*  4470 */   521,  521,  316,  312,  521,  521,  521,  521,  521,  521,
 /*  4480 */   213,  276,  281,  295,  210,  336,  291,  521,  240,  272,
 /*  4490 */   521,  521,  521,  521,  521,  521,  521,  302,  309,  311,
 /*  4500 */   310,  280,  290,  521,  262,  521,  299,  312,  521,  521,
 /*  4510 */   260,  256,  258,  521,  521,  212,  222,  228,  231,  233,
 /*  4520 */   235,  225,  219,  244,  246,  254,  248,  250,  252,  521,
 /*  4530 */   521,  521,  521,  521,  521,  241,  237,  521,  521,  284,
 /*  4540 */   521,  521,  521,  521,  521,  521,  542,  283,  285,  286,
 /*  4550 */   287,  288,  289,  240,  521,  217,  226,  229,  220,  223,
 /*  4560 */   521,  521,  521,  309,  311,  310,  280,  268,  521,  521,
 /*  4570 */   521,  360,  312,  521,  521,  521,  521,  521,  521,  213,
 /*  4580 */   276,  281,  295,  210,  355,  291,  521,  240,  272,  521,
 /*  4590 */   521,  521,  521,  521,  521,  521,  302,  309,  311,  310,
 /*  4600 */   280,  290,  521,  262,  521,  334,  312,  521,  521,  260,
 /*  4610 */   256,  258,  521,  521,  212,  222,  228,  231,  233,  235,
 /*  4620 */   225,  219,  244,  246,  254,  248,  250,  252,   96,   97,
 /*  4630 */    99,   98,  100,  521,  241,  237,  521,  521,  284,  521,
 /*  4640 */   521,  521,  521,  521,  521,  388,  283,  285,  286,  287,
 /*  4650 */   288,  289,  240,  521,  217,  226,  229,  220,  223,  689,
 /*  4660 */   521,  521,  309,  311,  310,  280,  268,  521,  521,  521,
 /*  4670 */   349,  312,  521,  102,  103,  521,  722,  580,  213,  276,
 /*  4680 */   281,  295,  210,  101,  291,  521,  521,  272,  475,  521,
 /*  4690 */   521,  521,  521,  521,  521,  302,  521,  521,    9,  521,
 /*  4700 */   290,   12,  262,  122,  521,  670,  680,  521,  521,  525,
 /*  4710 */   521,  521,  521,  260,  256,  258,  521,  521,  212,  222,
 /*  4720 */   228,  231,  233,  235,  225,  219,  244,  246,  254,  248,
 /*  4730 */   250,  252,  521,  521,  521,  521,  521,  284,  241,  237,
 /*  4740 */   521,  521,  521,  521,  521,  283,  285,  286,  287,  288,
 /*  4750 */   289,  521,  521,  217,  226,  229,  220,  223,  521,  890,
 /*  4760 */   521,  521,  521,  526,  521,  268,  521,  521,  521,  521,
 /*  4770 */   521,  521,  521,  521,  521,  521,  521,  213,  276,  281,
 /*  4780 */   295,  210,  521,  291,  521,  521,  293,  521,  521,  521,
 /*  4790 */   521,  521,  521,  521,  302,  521,  521,  240,  521,  290,
 /*  4800 */   521,  262,  521,  521,  339,  521,  521,  309,  311,  310,
 /*  4810 */   280,  521,  260,  256,  258,  332,  312,  212,  222,  228,
 /*  4820 */   231,  233,  235,  225,  219,  244,  246,  254,  248,  250,
 /*  4830 */   252,  521,  521,  521,  521,  521,  338,  241,  237,  521,
 /*  4840 */   521,  521,  521,  521,  337,  285,  286,  287,  288,  289,
 /*  4850 */   240,  521,  217,  226,  229,  220,  223,  521,  483,  521,
 /*  4860 */   309,  311,  310,  280,  268,  521,  521,  521,  306,  312,
 /*  4870 */   521,  521,  521,  521,  521,  521,  213,  276,  281,  295,
 /*  4880 */   210,  521,  291,  521,  521,  272,  521,  521,  521,  521,
 /*  4890 */   521,  240,  521,  302,  521,  521,  521,  521,  290,  521,
 /*  4900 */   262,  309,  311,  310,  280,  521,  260,  256,  258,  353,
 /*  4910 */   312,  212,  222,  228,  231,  233,  235,  225,  219,  244,
 /*  4920 */   246,  254,  248,  250,  252,  521,  521,  521,  540,  521,
 /*  4930 */   521,  241,  237,  521,  521,  284,  521,  521,  521,  521,
 /*  4940 */   521,  521,  537,  283,  285,  286,  287,  288,  289,  521,
 /*  4950 */   521,  217,  226,  229,  220,  223,  521,  521,  521,  521,
 /*  4960 */   521,  521,  521,  268,  521,  521,  521,  521,  521,  521,
 /*  4970 */   521,  521,  521,  521,  521,  213,  276,  281,  295,  210,
 /*  4980 */   362,  291,  521,  240,  272,  521,  521,  521,  521,  521,
 /*  4990 */   521,  521,  302,  309,  311,  310,  280,  290,  521,  262,
 /*  5000 */   521,  366,  312,  521,  521,  521,  521,  521,  521,  521,
 /*  5010 */   260,  256,  258,  521,  521,  212,  222,  228,  231,  233,
 /*  5020 */   235,  225,  219,  244,  246,  254,  248,  250,  252,  521,
 /*  5030 */   521,  521,  521,  521,  284,  241,  237,  521,  521,  487,
 /*  5040 */   521,  521,  283,  285,  286,  287,  288,  289,  521,  521,
 /*  5050 */   217,  226,  229,  220,  223,  521,  521,  521,  521,  521,
 /*  5060 */   521,  521,  268,  521,  521,  521,  521,  521,  521,  521,
 /*  5070 */   521,  521,  521,  521,  213,  276,  281,  295,  210,  521,
 /*  5080 */   291,  521,  521,  272,  521,  521,  521,  521,  521,  521,
 /*  5090 */   521,  302,  521,  521,  521,  521,  290,  521,  262,  521,
 /*  5100 */   521,  411,  521,  521,  521,  521,  521,  521,  521,  260,
 /*  5110 */   256,  258,  521,  521,  212,  222,  228,  231,  233,  235,
 /*  5120 */   225,  219,  244,  246,  254,  248,  250,  252,  521,  521,
 /*  5130 */   521,  521,  521,  284,  241,  237,  521,  521,  521,  521,
 /*  5140 */   521,  283,  285,  286,  287,  288,  289,  240,  521,  217,
 /*  5150 */   226,  229,  220,  223,  521,  374,  521,  309,  311,  310,
 /*  5160 */   280,  268,  521,  521,  521,  521,  326,  521,  521,  521,
 /*  5170 */   521,  521,  521,  213,  276,  281,  295,  210,  298,  291,
 /*  5180 */   521,  521,  272,  521,  521,  521,  521,  521,  521,  521,
 /*  5190 */   302,  521,  521,  521,  521,  290,  521,  262,  521,  521,
 /*  5200 */   521,  521,  521,  521,  521,  521,  521,  521,  260,  256,
 /*  5210 */   258,  521,  521,  212,  222,  228,  231,  233,  235,  225,
 /*  5220 */   219,  244,  246,  254,  248,  250,  252,  521,  521,  521,
 /*  5230 */   521,  521,  284,  241,  237,  521,  521,  521,  521,  521,
 /*  5240 */   283,  285,  286,  287,  288,  289,  521,  521,  217,  226,
 /*  5250 */   229,  220,  223,  521,  243,  521,  521,  521,  521,  521,
 /*  5260 */   268,  521,  521,  521,  521,  521,  521,  521,  521,  521,
 /*  5270 */   521,  521,  213,  276,  281,  295,  210,  331,  291,  521,
 /*  5280 */   521,  272,  521,  521,  521,  521,  521,  521,  521,  302,
 /*  5290 */   521,  521,  521,  521,  290,  521,  262,  521,  521,  521,
 /*  5300 */   521,  521,  521,  521,  521,  521,  521,  260,  256,  258,
 /*  5310 */   521,  521,  212,  222,  228,  231,  233,  235,  225,  219,
 /*  5320 */   244,  246,  254,  248,  250,  252,  521,  521,  521,  521,
 /*  5330 */   521,  284,  241,  237,  521,  521,  474,  521,  521,  283,
 /*  5340 */   285,  286,  287,  288,  289,  521,  521,  217,  226,  229,
 /*  5350 */   220,  223,  521,  521,  521,  521,  521,  521,  521,  268,
 /*  5360 */   521,  521,  521,  521,  521,  521,  521,  521,  521,  521,
 /*  5370 */   521,  213,  276,  281,  295,  210,  521,  291,  521,  521,
 /*  5380 */   272,  521,  521,  521,  521,  521,  521,  521,  302,  521,
 /*  5390 */   521,  521,  521,  290,  521,  262,  521,  521,  443,  521,
 /*  5400 */   521,  260,  256,  258,  521,  521,  212,  222,  228,  231,
 /*  5410 */   233,  235,  225,  219,  244,  246,  254,  248,  250,  252,
 /*  5420 */   521,  521,  521,  521,  521,  521,  241,  237,  521,  521,
 /*  5430 */   284,  521,  521,  521,  521,  521,  521,  530,  283,  285,
 /*  5440 */   286,  287,  288,  289,  521,  521,  217,  226,  229,  220,
 /*  5450 */   223,  521,  521,  521,  521,  521,  521,  521,  268,  521,
 /*  5460 */   521,  521,  521,  521,  521,  521,  521,  521,  521,  521,
 /*  5470 */   213,  276,  281,  295,  210,  351,  291,  521,  521,  272,
 /*  5480 */   521,  521,  521,  521,  521,  521,  521,  302,  521,  521,
 /*  5490 */   521,  521,  290,  521,  262,  521,  521,  521,  521,  521,
 /*  5500 */   260,  256,  258,  521,  521,  212,  222,  228,  231,  233,
 /*  5510 */   235,  225,  219,  244,  246,  254,  248,  250,  252,  521,
 /*  5520 */   521,  521,  521,  521,  521,  241,  237,  521,  521,  284,
 /*  5530 */   521,  521,  521,  521,  521,  521,  521,  283,  285,  286,
 /*  5540 */   287,  288,  289,  521,  521,  217,  226,  229,  220,  223,
 /*  5550 */   521,  521,  521,  521,  521,  521,  521,  268,  521,  521,
 /*  5560 */   521,  521,  521,  521,  521,  521,  521,  521,  521,  213,
 /*  5570 */   276,  281,  295,  210,  325,  291,  521,  521,  272,  521,
 /*  5580 */   521,  521,  521,  521,  521,  521,  302,  521,  521,  521,
 /*  5590 */   521,  290,  521,  262,  521,  521,  521,  521,  521,  521,
 /*  5600 */   256,  258,  521,  521,  212,  222,  228,  231,  233,  235,
 /*  5610 */   225,  219,  244,  246,  254,  248,  250,  252,   96,   97,
 /*  5620 */    99,   98,  100,  521,  241,  237,  521,  521,  284,  521,
 /*  5630 */   521,  521,  521,  521,  521,  521,  283,  285,  286,  287,
 /*  5640 */   288,  289,  521,  521,  217,  226,  229,  220,  223,  521,
 /*  5650 */   521,  521,  521,  521,  521,  521,  268,  521,  521,  521,
 /*  5660 */   521,  521,  521,  102,  103,  521,  719,  580,  213,  276,
 /*  5670 */   281,  295,  210,  101,  291,  521,  521,  272,  521,  521,
 /*  5680 */   521,  521,  521,  521,  521,  302,  521,  521,  521,  521,
 /*  5690 */   290,  521,  262,  521,  521,  435,  521,  521,  521,  521,
 /*  5700 */   521,  521,  521,  212,  222,  228,  231,  233,  235,  225,
 /*  5710 */   219,  244,  246,  254,  248,  250,  252,  521,  521,  521,
 /*  5720 */   521,  521,  521,  241,  237,  521,  521,  284,  521,  521,
 /*  5730 */   521,  521,  521,  521,  521,  283,  285,  286,  287,  288,
 /*  5740 */   289,  521,  521,  217,  226,  229,  220,  223,  521,  521,
 /*  5750 */   521,  521,  521,  521,  521,  268,  521,   92,  521,  521,
 /*  5760 */   521,  521,  521,  521,  521,  521,  521,  213,  276,  281,
 /*  5770 */   295,  210,  308,  291,  109,   92,  272,  521,   66,  521,
 /*  5780 */   521,  521,  521,  521,  302,  521,  521,  521,  521,  290,
 /*  5790 */   521,  262,  142,  521,  521,  521,   66,   45,   46,   47,
 /*  5800 */    48,   49,   50,   51,   52,   53,   54,   55,  521,  521,
 /*  5810 */   521,  521,  521,  521,  521,   45,   46,   47,   48,   49,
 /*  5820 */    50,   51,   52,   53,   54,   55,  284,  521,  521,  521,
 /*  5830 */   521,  521,  521,  521,  283,  285,  286,  287,  288,  289,
 /*  5840 */   521,  521,  217,  226,  229,  220,  223,  521,  521,  521,
 /*  5850 */   521,  521,  521,  521,  268,  521,   92,  521,  521,  521,
 /*  5860 */   442,  406,  521,  521,  521,  521,  213,  276,  281,  295,
 /*  5870 */   210,  521,  291,  585,  397,  272,  521,   66,  521,  521,
 /*  5880 */   521,  521,  521,  302,  521,  521,  521,  521,  290,  521,
 /*  5890 */   262,  521,  521,  521,  521,  521,   45,   46,   47,   48,
 /*  5900 */    49,   50,   51,   52,   53,   54,   55,  521,  398,  399,
 /*  5910 */   400,  401,  402,  521,  430,  454,  455,  521,  528,  521,
 /*  5920 */   521,  521,  521,  521,  521,  284,  521,  521,  521,  521,
 /*  5930 */   521,  521,  521,  283,  285,  286,  287,  288,  289,  521,
 /*  5940 */   521,  217,  226,  229,  220,  223,  521,  521,  521,  521,
 /*  5950 */   521,  521,  521,  268,  521,   92,  521,  521,  521,  521,
 /*  5960 */   521,  521,  521,  521,  521,  213,  276,  281,  295,  210,
 /*  5970 */   368,  291,   27,   92,  272,  521,   66,  521,  521,  521,
 /*  5980 */   521,  521,  302,  521,  521,  521,  521,  290,  521,  262,
 /*  5990 */   521,  521,  521,  521,   66,   45,   46,   47,   48,   49,
 /*  6000 */    50,   51,   52,   53,   54,   55,  521,  521,  521,  521,
 /*  6010 */   521,  521,  521,   45,   46,   47,   48,   49,   50,   51,
 /*  6020 */    52,   53,   54,   55,  284,  521,  521,  521,  521,  521,
 /*  6030 */   521,  521,  283,  285,  286,  287,  288,  289,  521,  521,
 /*  6040 */   217,  226,  229,  220,  223,  521,  521,  521,  521,  521,
 /*  6050 */   521,  521,  268,   39,  521,  521,  521,  521,  521,   76,
 /*  6060 */   521,  521,  521,  521,  213,  276,  281,  295,  210,  521,
 /*  6070 */   291,  521,   79,  272,  521,  521,  521,  521,  521,  521,
 /*  6080 */   521,  302,  521,  521,  521,  521,  290,  521,  262,  521,
 /*  6090 */   521,  521,  521,   45,   46,   47,   48,   49,   50,   51,
 /*  6100 */    52,   53,   54,   55,   96,   97,   99,   98,  100,   71,
 /*  6110 */   521,  521,  521,  521,  521,  521,  521,   69,   70,   72,
 /*  6120 */    73,   75,   74,  284,  521,  521,  521,  521,  521,  521,
 /*  6130 */   521,  283,  285,  286,  287,  288,  289,  521,  521,  217,
 /*  6140 */   226,  229,  220,  223,   96,   97,   99,   98,  100,  102,
 /*  6150 */   103,  268,  132,  624,  521,  618,  521,  521,  521,  101,
 /*  6160 */   521,  521,  521,  213,  276,  281,  295,  210,  521,  291,
 /*  6170 */   521,  521,  293,  521,  521,  521,  521,  521,  521,  521,
 /*  6180 */   302,  521,  521,  521,  521,  290,  521,  262,  521,  102,
 /*  6190 */   103,  521,  716,  624,  521,  618,  521,  521,  521,  101,
 /*  6200 */   521,   96,   97,   99,   98,  100,  521,  521,  521,  521,
 /*  6210 */   521,  521,  521,  521,  521,  521,  521,  521,  521,  521,
 /*  6220 */   521,  521,  338,  521,   96,   97,   99,   98,  100,  521,
 /*  6230 */   337,  285,  286,  287,  288,  289,   96,   97,   99,   98,
 /*  6240 */   100,   96,   97,   99,   98,  100,  102,  103,  521,  720,
 /*  6250 */   612,  521,  618,  521,  521,  521,  101,  521,   96,   97,
 /*  6260 */    99,   98,  100,   96,   97,   99,   98,  100,  521,  102,
 /*  6270 */   103,  521,  718,  580,   96,   97,   99,   98,  100,  101,
 /*  6280 */   521,  102,  103,  521,  721,  659,  102,  103,  521,  724,
 /*  6290 */   580,  101,  521,  521,  521,  521,  101,  521,  521,  521,
 /*  6300 */   521,  521,  521,  102,  103,  521,   21,  104,  102,  103,
 /*  6310 */   521,  723,  580,  101,  521,  521,  521,  521,  101,  102,
 /*  6320 */   103,  521,   19,  104,  521,  521,  521,  521,  521,  101,
};
static YYCODETYPE yy_lookahead[] = {
 /*     0 */     7,    8,    9,   10,   11,  135,    6,  137,  138,  139,
 /*    10 */   140,  141,   19,  106,  107,  108,  109,  110,  111,  112,
 /*    20 */   113,  114,   37,   38,   31,   32,   33,   34,   35,   40,
 /*    30 */    37,    6,  118,   40,   41,  121,  122,  123,  124,  125,
 /*    40 */   125,   48,   49,   50,  130,  131,   53,  132,   55,   49,
 /*    50 */   136,  136,   59,   60,   61,   62,   63,   64,   65,   66,
 /*    60 */    67,   36,   69,   70,   71,   72,  121,   74,  123,  124,
 /*    70 */   125,   78,   79,   80,   81,  130,  131,   84,   40,    6,
 /*    80 */   169,  136,  169,   90,  173,  172,  173,   94,   95,   96,
 /*    90 */    97,   98,   99,  100,  101,  102,  103,  104,    7,    8,
 /*   100 */     9,   10,   11,   25,   26,   27,   28,   29,   30,   55,
 /*   110 */    19,  133,  180,  181,   41,   37,   38,  139,  140,   38,
 /*   120 */   142,   49,   31,   32,   33,   34,   35,  133,   37,   48,
 /*   130 */    76,   40,   41,  139,  140,  123,  142,  125,    0,   48,
 /*   140 */    49,   50,  130,  131,   53,   91,   55,   75,  136,   77,
 /*   150 */    59,   60,   61,   62,   63,   64,   65,   66,   67,    6,
 /*   160 */    69,   70,   71,   72,   37,   74,  123,  124,  125,   78,
 /*   170 */    79,   80,   81,   41,  131,   84,    6,   41,   51,  136,
 /*   180 */    48,   90,    6,   51,   48,   94,   95,   96,   97,   98,
 /*   190 */    99,  100,  101,  102,  103,  104,    7,    8,    9,   10,
 /*   200 */    11,  135,   40,  137,  138,  139,  140,  141,   19,   40,
 /*   210 */    48,   41,   85,   86,   87,   88,   89,   41,   49,   50,
 /*   220 */    31,   32,   33,   34,   35,  133,   37,  180,  181,   40,
 /*   230 */    41,  139,  140,   38,  142,   82,   41,   48,   49,   50,
 /*   240 */   139,  140,   53,  142,   55,  128,  129,    6,   59,   60,
 /*   250 */    61,   62,   63,   64,   65,   66,   67,   41,   69,   70,
 /*   260 */    71,   72,  163,   74,   41,  166,   51,   78,   79,   80,
 /*   270 */    81,   48,   40,   84,   51,  176,  177,  178,  179,   90,
 /*   280 */   180,  181,   41,   94,   95,   96,   97,   98,   99,  100,
 /*   290 */   101,  102,  103,  104,    7,    8,    9,   10,   11,  166,
 /*   300 */    85,   86,   87,   88,   89,   40,   19,  174,  175,  176,
 /*   310 */   177,  178,  179,  138,  139,  140,  141,   49,   31,   32,
 /*   320 */    33,   34,   35,   45,   37,  115,   48,   40,   41,   28,
 /*   330 */    29,   30,  180,  181,    6,   48,   49,   50,   37,   38,
 /*   340 */    53,   48,   55,   75,    6,   77,   59,   60,   61,   62,
 /*   350 */    63,   64,   65,   66,   67,   90,   69,   70,   71,   72,
 /*   360 */    37,   74,  117,  133,  119,   78,   79,   80,   81,  139,
 /*   370 */   140,   84,  142,   43,   51,   45,   48,   90,   48,   41,
 /*   380 */     6,   94,   95,   96,   97,   98,   99,  100,  101,  102,
 /*   390 */   103,  104,    7,    8,    9,   10,   11,  135,   40,  137,
 /*   400 */   138,  139,  140,  141,   19,  180,  181,    6,   85,   86,
 /*   410 */    87,   88,   89,  180,  181,   41,   31,   32,   33,   34,
 /*   420 */    35,  117,   37,  119,   40,   40,   41,  120,  180,  181,
 /*   430 */   125,  115,  125,   48,   49,   50,  131,   53,   53,  132,
 /*   440 */    55,  136,   41,  136,   59,   60,   61,   62,   63,   64,
 /*   450 */    65,   66,   67,   41,   69,   70,   71,   72,  163,   74,
 /*   460 */    48,  166,   43,   78,   79,   80,   81,   48,    6,   84,
 /*   470 */   136,  176,  177,  178,  179,   90,  180,  181,    6,   94,
 /*   480 */    95,   96,   97,   98,   99,  100,  101,  102,  103,  104,
 /*   490 */     7,    8,    9,   10,   11,  180,  181,  122,  123,  124,
 /*   500 */   125,   40,   19,   41,    6,  130,  131,   43,  125,   45,
 /*   510 */     6,  136,   48,   41,   31,   32,   33,   34,   35,  136,
 /*   520 */    37,  180,  181,   40,   41,    6,  167,  168,   40,   35,
 /*   530 */     6,   48,   49,   50,   40,  117,   53,  119,   55,   41,
 /*   540 */    36,   55,   59,   60,   61,   62,   63,   64,   65,   66,
 /*   550 */    67,   90,   69,   70,   71,   72,    6,   74,   40,  166,
 /*   560 */    41,   78,   79,   80,   81,   41,   48,   84,  175,  176,
 /*   570 */   177,  178,  179,   90,   40,  164,  165,   94,   95,   96,
 /*   580 */    97,   98,   99,  100,  101,  102,  103,  104,    7,    8,
 /*   590 */     9,   10,   11,  166,   43,   41,    6,  170,    6,   48,
 /*   600 */    19,   38,   40,  176,  177,  178,  179,    6,   58,   15,
 /*   610 */    48,   48,   31,   32,   33,   34,   35,   38,   37,    6,
 /*   620 */    41,   40,   41,   35,   90,   38,   36,  125,   36,   48,
 /*   630 */    49,   50,   58,    6,   53,   48,   55,   36,  136,   20,
 /*   640 */    59,   60,   61,   62,   63,   64,   65,   66,   67,   36,
 /*   650 */    69,   70,   71,   72,  163,   74,   37,  166,   40,   78,
 /*   660 */    79,   80,   81,   36,    6,   84,    6,  176,  177,  178,
 /*   670 */   179,   90,   55,   37,   38,   94,   95,   96,   97,   98,
 /*   680 */    99,  100,  101,  102,  103,  104,    7,    8,    9,   10,
 /*   690 */    11,  166,    6,   20,   36,  170,   36,   55,   19,   38,
 /*   700 */    41,  176,  177,  178,  179,    6,    6,   51,   91,   48,
 /*   710 */    31,   32,   33,   34,   35,   38,   37,    6,   41,   40,
 /*   720 */    41,  126,   36,   40,    6,   37,   38,   48,   49,   50,
 /*   730 */    91,   48,   53,   91,   55,   36,   36,   53,   59,   60,
 /*   740 */    61,   62,   63,   64,   65,   66,   67,   36,   69,   70,
 /*   750 */    71,   72,  163,   74,   36,  166,   40,   78,   79,   80,
 /*   760 */    81,   40,   40,   84,   48,  176,  177,  178,  179,   90,
 /*   770 */    51,   50,  126,   94,   95,   96,   97,   98,   99,  100,
 /*   780 */   101,  102,  103,  104,    7,    8,    9,   10,   11,  166,
 /*   790 */    51,   38,  125,  170,   41,  126,   19,   40,   51,  176,
 /*   800 */   177,  178,  179,  136,  126,   40,   20,   40,   31,   32,
 /*   810 */    33,   34,   35,   55,   37,   41,   41,   40,   41,   40,
 /*   820 */   116,   40,  116,   40,  116,   48,   49,   50,   40,   49,
 /*   830 */    53,   55,   55,   41,   41,   48,   59,   60,   61,   62,
 /*   840 */    63,   64,   65,   66,   67,   73,   69,   70,   71,   72,
 /*   850 */   163,   74,   48,  166,   48,   78,   79,   80,   81,  165,
 /*   860 */   126,   84,   48,  176,  177,  178,  179,   90,   48,   48,
 /*   870 */    76,   94,   95,   96,   97,   98,   99,  100,  101,  102,
 /*   880 */   103,  104,    7,    8,    9,   10,   11,  135,   79,  137,
 /*   890 */   138,  139,  140,  141,   19,  135,   41,  137,  138,  139,
 /*   900 */   140,  141,   40,   55,   36,  182,   31,   32,   33,   34,
 /*   910 */    35,   40,   37,   20,   55,   40,   41,   40,    6,  182,
 /*   920 */    55,   40,   49,   48,   49,   50,   40,   55,   53,   49,
 /*   930 */    55,   55,   40,   49,   59,   60,   61,   62,   63,   64,
 /*   940 */    65,   66,   67,   55,   69,   70,   71,   72,   49,   74,
 /*   950 */   166,   55,   49,   78,   79,   80,   81,   55,   49,   84,
 /*   960 */   176,  177,  178,  179,   49,   90,   76,   76,   40,   94,
 /*   970 */    95,   96,   97,   98,   99,  100,  101,  102,  103,  104,
 /*   980 */     7,    8,    9,   10,   11,  166,   76,   40,   58,  170,
 /*   990 */   168,   49,   19,  169,  169,  176,  177,  178,  179,  169,
 /*  1000 */   169,   49,  169,  169,   31,   32,   33,   34,   35,  169,
 /*  1010 */    37,   58,  169,   40,   41,  169,   58,   41,   41,   41,
 /*  1020 */    41,   48,   49,   50,   40,   40,   53,   41,   55,   41,
 /*  1030 */    40,  181,   59,   60,   61,   62,   63,   64,   65,   66,
 /*  1040 */    67,   40,   69,   70,   71,   72,   51,   74,  166,  126,
 /*  1050 */    82,   78,   79,   80,   81,   41,   48,   84,  176,  177,
 /*  1060 */   178,  179,   79,   90,   73,   76,   48,   94,   95,   96,
 /*  1070 */    97,   98,   99,  100,  101,  102,  103,  104,    7,    8,
 /*  1080 */     9,   10,   11,  135,   48,  137,  138,  139,  140,  141,
 /*  1090 */    19,  135,   40,  137,  138,  139,  140,  141,   55,   48,
 /*  1100 */    53,   48,   31,   32,   33,   34,   35,   51,   37,   41,
 /*  1110 */    53,   40,   41,  126,   51,   41,   41,   41,  126,   48,
 /*  1120 */    49,   50,   41,  127,   53,  126,   55,  127,   40,  127,
 /*  1130 */    59,   60,   61,   62,   63,   64,   65,   66,   67,  129,
 /*  1140 */    69,   70,   71,   72,   41,   74,  166,  126,   41,   78,
 /*  1150 */    79,   80,   81,  127,  119,   84,  176,  177,  178,  179,
 /*  1160 */    41,   90,  116,   40,   44,   94,   95,   96,   97,   98,
 /*  1170 */    99,  100,  101,  102,  103,  104,    7,    8,    9,   10,
 /*  1180 */    11,  166,  116,   40,   48,  170,  116,   40,   19,  116,
 /*  1190 */    44,  176,  177,  178,  179,  116,   40,  189,  116,  189,
 /*  1200 */    31,   32,   33,   34,   35,  189,   37,  189,  189,   40,
 /*  1210 */    41,  189,  189,  189,  189,  189,  189,   48,   49,   50,
 /*  1220 */   189,  189,   53,  189,   55,  189,  189,  189,   59,   60,
 /*  1230 */    61,   62,   63,   64,   65,   66,   67,  189,   69,   70,
 /*  1240 */    71,   72,  189,   74,  166,  189,  189,   78,   79,   80,
 /*  1250 */    81,  189,  189,   84,  176,  177,  178,  179,  189,   90,
 /*  1260 */   189,  189,  189,   94,   95,   96,   97,   98,   99,  100,
 /*  1270 */   101,  102,  103,  104,    7,    8,    9,   10,   11,  166,
 /*  1280 */   189,  189,  189,  170,  189,  189,   19,  189,  189,  176,
 /*  1290 */   177,  178,  179,  189,  189,  189,  189,  189,   31,   32,
 /*  1300 */    33,   34,   35,  189,   37,  189,  189,   40,   41,  189,
 /*  1310 */   189,  189,  189,  189,  189,   48,   49,   50,  189,  189,
 /*  1320 */    53,  189,   55,  189,  189,  189,   59,   60,   61,   62,
 /*  1330 */    63,   64,   65,   66,   67,  189,   69,   70,   71,   72,
 /*  1340 */   189,   74,  166,  189,  189,   78,   79,   80,   81,  189,
 /*  1350 */   189,   84,  176,  177,  178,  179,  189,   90,  189,  189,
 /*  1360 */   189,   94,   95,   96,   97,   98,   99,  100,  101,  102,
 /*  1370 */   103,  104,    7,    8,    9,   10,   11,  166,  189,  189,
 /*  1380 */   189,  170,  189,  189,   19,  189,  189,  176,  177,  178,
 /*  1390 */   179,  189,  189,  189,  189,  189,   31,   32,   33,   34,
 /*  1400 */    35,  189,   37,  189,  189,   40,   41,  189,  189,  189,
 /*  1410 */   189,  189,  189,   48,   49,   50,  189,  189,   53,  189,
 /*  1420 */    55,  189,  189,  189,   59,   60,   61,   62,   63,   64,
 /*  1430 */    65,   66,   67,  189,   69,   70,   71,   72,  189,   74,
 /*  1440 */   166,  189,  189,   78,   79,   80,   81,  189,  189,   84,
 /*  1450 */   176,  177,  178,  179,  189,   90,  189,  189,  189,   94,
 /*  1460 */    95,   96,   97,   98,   99,  100,  101,  102,  103,  104,
 /*  1470 */     7,    8,    9,   10,   11,  166,  189,  189,  189,  170,
 /*  1480 */   189,  189,   19,  189,  189,  176,  177,  178,  179,  189,
 /*  1490 */   189,  189,  189,  189,   31,   32,   33,   34,   35,  189,
 /*  1500 */    37,  189,  189,   40,   41,  189,  189,  189,  189,  189,
 /*  1510 */   189,   48,   49,   50,  189,  189,   53,  189,   55,  189,
 /*  1520 */   189,  189,   59,   60,   61,   62,   63,   64,   65,   66,
 /*  1530 */    67,  189,   69,   70,   71,   72,  189,   74,  166,  189,
 /*  1540 */   189,   78,   79,   80,   81,  189,  189,   84,  176,  177,
 /*  1550 */   178,  179,  189,   90,  189,  189,  189,   94,   95,   96,
 /*  1560 */    97,   98,   99,  100,  101,  102,  103,  104,    7,    8,
 /*  1570 */     9,   10,   11,  166,  189,  189,  189,  189,  171,  189,
 /*  1580 */    19,  189,  189,  176,  177,  178,  179,  189,  189,  189,
 /*  1590 */   189,  189,   31,   32,   33,   34,   35,  189,   37,  189,
 /*  1600 */   189,   40,   41,  189,  189,  189,  189,  189,  189,   48,
 /*  1610 */    49,   50,  189,  189,   53,  189,   55,  189,  189,  189,
 /*  1620 */    59,   60,   61,   62,   63,   64,   65,   66,   67,  189,
 /*  1630 */    69,   70,   71,   72,  189,   74,  166,  189,  189,   78,
 /*  1640 */    79,   80,   81,  189,  189,   84,  176,  177,  178,  179,
 /*  1650 */   189,   90,  189,  189,  189,   94,   95,   96,   97,   98,
 /*  1660 */    99,  100,  101,  102,  103,  104,    7,    8,    9,   10,
 /*  1670 */    11,  166,  189,  189,  189,  170,  189,  189,   19,  189,
 /*  1680 */   189,  176,  177,  178,  179,  189,  189,  189,  189,  189,
 /*  1690 */    31,   32,   33,   34,   35,  189,   37,  189,  189,   40,
 /*  1700 */    41,  189,  189,  189,  189,  189,  189,   48,   49,   50,
 /*  1710 */   189,  189,   53,  189,   55,  189,  189,  189,   59,   60,
 /*  1720 */    61,   62,   63,   64,   65,   66,   67,  189,   69,   70,
 /*  1730 */    71,   72,  189,   74,  166,  189,  189,   78,   79,   80,
 /*  1740 */    81,  189,  189,   84,  176,  177,  178,  179,  189,   90,
 /*  1750 */   189,  189,  189,   94,   95,   96,   97,   98,   99,  100,
 /*  1760 */   101,  102,  103,  104,    7,    8,    9,   10,   11,  109,
 /*  1770 */   110,  111,  112,  113,  114,  135,   19,  137,  138,  139,
 /*  1780 */   140,  141,  189,  189,  189,  189,  189,  189,   31,   32,
 /*  1790 */    33,   34,   35,  189,   37,  189,  189,   40,   41,  189,
 /*  1800 */   189,  189,  189,  189,  189,   48,   49,   50,  189,  189,
 /*  1810 */    53,  189,   55,  189,  189,  189,   59,   60,   61,   62,
 /*  1820 */    63,   64,   65,   66,   67,  189,   69,   70,   71,   72,
 /*  1830 */   189,   74,  166,  189,  189,   78,   79,   80,   81,  189,
 /*  1840 */   189,   84,  176,  177,  178,  179,  189,   90,  189,  189,
 /*  1850 */   189,   94,   95,   96,   97,   98,   99,  100,  101,  102,
 /*  1860 */   103,  104,    7,    8,    9,   10,   11,  166,  189,  189,
 /*  1870 */   189,  170,  189,  189,   19,  189,  189,  176,  177,  178,
 /*  1880 */   179,  189,  189,  189,  189,  189,   31,   32,   33,   34,
 /*  1890 */    35,  189,   37,  189,  189,   40,   41,  189,  189,  189,
 /*  1900 */   189,  189,  189,   48,   49,   50,  189,  189,   53,  189,
 /*  1910 */    55,  189,  189,  189,   59,   60,   61,   62,   63,   64,
 /*  1920 */    65,   66,   67,  189,   69,   70,   71,   72,  189,   74,
 /*  1930 */   166,  189,  189,   78,   79,   80,   81,  189,  189,   84,
 /*  1940 */   176,  177,  178,  179,  189,   90,  189,  189,  189,   94,
 /*  1950 */    95,   96,   97,   98,   99,  100,  101,  102,  103,  104,
 /*  1960 */     7,    8,    9,   10,   11,  166,  189,  189,  189,  189,
 /*  1970 */   171,  189,   19,  189,  189,  176,  177,  178,  179,  189,
 /*  1980 */   189,  189,  189,  189,   31,   32,   33,   34,   35,  189,
 /*  1990 */    37,  189,  189,   40,   41,  189,  189,  189,  189,  189,
 /*  2000 */   189,   48,   49,   50,  189,  189,   53,  189,   55,  189,
 /*  2010 */   189,  189,   59,   60,   61,   62,   63,   64,   65,   66,
 /*  2020 */    67,  189,   69,   70,   71,   72,  189,   74,  166,  189,
 /*  2030 */   189,   78,   79,   80,   81,  189,  189,   84,  176,  177,
 /*  2040 */   178,  179,  189,   90,  189,  189,  189,   94,   95,   96,
 /*  2050 */    97,   98,   99,  100,  101,  102,  103,  104,    7,    8,
 /*  2060 */     9,   10,   11,  166,  189,  189,  189,  170,  189,  189,
 /*  2070 */    19,  189,  189,  176,  177,  178,  179,  189,  189,  189,
 /*  2080 */   189,  189,   31,   32,   33,   34,   35,  189,   37,  189,
 /*  2090 */   189,   40,   41,  189,  189,  189,  189,  189,  189,   48,
 /*  2100 */    49,   50,  189,  189,   53,  189,   55,  189,  189,  189,
 /*  2110 */    59,   60,   61,   62,   63,   64,   65,   66,   67,  189,
 /*  2120 */    69,   70,   71,   72,  189,   74,  166,  189,  189,   78,
 /*  2130 */    79,   80,   81,  189,  139,   84,  176,  177,  178,  179,
 /*  2140 */   189,   90,  189,  189,  189,   94,   95,   96,   97,   98,
 /*  2150 */    99,  100,  101,  102,  103,  104,    7,    8,    9,   10,
 /*  2160 */    11,  166,  189,  189,  189,  189,  189,  189,   19,  189,
 /*  2170 */   189,  176,  177,  178,  179,  189,  189,  189,  189,  189,
 /*  2180 */    31,   32,   33,   34,   35,  189,   37,  189,  189,   40,
 /*  2190 */    41,  189,  189,  189,  189,  189,  189,   48,   49,   50,
 /*  2200 */   189,  189,   53,  189,   55,  189,  189,  189,   59,   60,
 /*  2210 */    61,   62,   63,   64,   65,   66,   67,  189,   69,   70,
 /*  2220 */    71,   72,  189,   74,  166,  189,  189,   78,   79,   80,
 /*  2230 */    81,  189,  189,   84,  176,  177,  178,  179,  189,   90,
 /*  2240 */   189,  189,  189,   94,   95,   96,   97,   98,   99,  100,
 /*  2250 */   101,  102,  103,  104,    7,    8,    9,   10,   11,  166,
 /*  2260 */   189,  189,  189,  189,  171,  189,   19,  189,  189,  176,
 /*  2270 */   177,  178,  179,  189,  189,  189,  189,  189,   31,   32,
 /*  2280 */    33,   34,   35,  189,   37,  189,  189,   40,   41,  189,
 /*  2290 */   189,  189,  189,  189,  189,   48,   49,   50,  189,  189,
 /*  2300 */    53,  189,   55,  189,  189,  189,   59,   60,   61,   62,
 /*  2310 */    63,   64,   65,   66,   67,  189,   69,   70,   71,   72,
 /*  2320 */   189,   74,  166,  189,  189,   78,   79,   80,   81,  189,
 /*  2330 */   189,   84,  176,  177,  178,  179,  189,   90,  189,  189,
 /*  2340 */   189,   94,   95,   96,   97,   98,   99,  100,  101,  102,
 /*  2350 */   103,  104,    7,    8,    9,   10,   11,  166,  189,  189,
 /*  2360 */   189,  189,  189,  189,   19,  189,  189,  176,  177,  178,
 /*  2370 */   179,  189,  189,  189,  189,  189,   31,   32,   33,   34,
 /*  2380 */    35,  189,   37,  189,  189,   40,   41,  189,  189,  189,
 /*  2390 */   189,  189,  189,   48,   49,   50,  189,  189,   53,  189,
 /*  2400 */    55,  189,  189,  189,   59,   60,   61,   62,   63,   64,
 /*  2410 */    65,   66,   67,  189,   69,   70,   71,   72,  189,   74,
 /*  2420 */   166,  189,  189,   78,   79,   80,   81,  189,  189,   84,
 /*  2430 */   176,  177,  178,  179,  189,   90,  189,  189,  189,   94,
 /*  2440 */    95,   96,   97,   98,   99,  100,  101,  102,  103,  104,
 /*  2450 */     7,    8,    9,   10,   11,  166,  189,  189,  189,  189,
 /*  2460 */   189,  189,   19,  189,  189,  176,  177,  178,  179,  189,
 /*  2470 */   189,  189,  189,  189,   31,   32,   33,   34,   35,  189,
 /*  2480 */    37,  189,  189,   40,   41,  189,  189,  189,  189,  189,
 /*  2490 */   189,   48,   49,   50,  189,  189,   53,  189,   55,  189,
 /*  2500 */   189,  189,   59,   60,   61,   62,   63,   64,   65,   66,
 /*  2510 */    67,  189,   69,   70,   71,   72,  189,   74,  166,  189,
 /*  2520 */   189,   78,   79,   80,   81,  189,  189,   84,  176,  177,
 /*  2530 */   178,  179,  189,   90,  189,  189,  189,   94,   95,   96,
 /*  2540 */    97,   98,   99,  100,  101,  102,  103,  104,    7,    8,
 /*  2550 */     9,   10,   11,  166,  189,  189,  189,  189,  189,  189,
 /*  2560 */    19,  189,  189,  176,  177,  178,  179,  189,  189,  189,
 /*  2570 */   189,  189,   31,   32,   33,   34,   35,  189,   37,  189,
 /*  2580 */   189,   40,   41,  189,  189,  189,  189,  189,  189,   48,
 /*  2590 */    49,   50,  189,  189,   53,  189,   55,  189,  189,  189,
 /*  2600 */    59,   60,   61,   62,   63,   64,   65,   66,   67,  189,
 /*  2610 */    69,   70,   71,   72,  189,   74,  166,  189,  189,   78,
 /*  2620 */    79,   80,   81,  189,  189,   84,  176,  177,  178,  179,
 /*  2630 */   189,   90,  189,  189,  189,   94,   95,   96,   97,   98,
 /*  2640 */    99,  100,  101,  102,  103,  104,    7,    8,    9,   10,
 /*  2650 */    11,  166,  189,  189,  189,  189,  189,  189,   19,  189,
 /*  2660 */   189,  176,  177,  178,  179,  189,  189,  189,  189,  189,
 /*  2670 */    31,   32,   33,   34,   35,  189,   37,  189,  189,   40,
 /*  2680 */    41,  189,  189,  189,  189,  189,  189,   48,   49,   50,
 /*  2690 */   189,  189,   53,  189,   55,  189,  189,  189,   59,   60,
 /*  2700 */    61,   62,   63,   64,   65,   66,   67,  189,   69,   70,
 /*  2710 */    71,   72,  189,   74,  166,  189,  189,   78,   79,   80,
 /*  2720 */    81,  189,  189,   84,  176,  177,  178,  179,  189,   90,
 /*  2730 */   189,  189,  189,   94,   95,   96,   97,   98,   99,  100,
 /*  2740 */   101,  102,  103,  104,    7,    8,    9,   10,   11,  166,
 /*  2750 */   189,  189,  189,  189,  189,  189,   19,  189,  189,  176,
 /*  2760 */   177,  178,  179,  189,  189,  189,  189,  189,   31,   32,
 /*  2770 */    33,   34,   35,  189,   37,  189,  189,   40,   41,  189,
 /*  2780 */   189,  189,  189,  189,  189,   48,   49,   50,  189,  189,
 /*  2790 */    53,  189,   55,  189,  189,  189,   59,   60,   61,   62,
 /*  2800 */    63,   64,   65,   66,   67,  189,   69,   70,   71,   72,
 /*  2810 */   189,   74,  166,  189,  189,   78,   79,   80,   81,  189,
 /*  2820 */   189,   84,  176,  177,  178,  179,  189,   90,  189,  189,
 /*  2830 */   189,   94,   95,   96,   97,   98,   99,  100,  101,  102,
 /*  2840 */   103,  104,    7,    8,    9,   10,   11,  166,  189,  189,
 /*  2850 */   189,  189,  189,  189,   19,  189,  189,  176,  177,  178,
 /*  2860 */   179,  189,  189,  189,  189,  189,   31,   32,   33,   34,
 /*  2870 */    35,  189,   37,  189,  189,   40,   41,  189,  189,  189,
 /*  2880 */   189,  189,  189,   48,   49,   50,  189,  189,   53,  189,
 /*  2890 */    55,  189,  189,  189,   59,   60,   61,   62,   63,   64,
 /*  2900 */    65,   66,   67,  189,   69,   70,   71,   72,  189,   74,
 /*  2910 */   166,  189,  189,   78,   79,   80,   81,  189,  189,   84,
 /*  2920 */   176,  177,  178,  179,  189,   90,  189,  189,  189,   94,
 /*  2930 */    95,   96,   97,   98,   99,  100,  101,  102,  103,  104,
 /*  2940 */     7,    8,    9,   10,   11,  166,  189,  189,  189,  189,
 /*  2950 */   189,  189,   19,  189,  189,  176,  177,  178,  179,  189,
 /*  2960 */   189,  189,  189,  189,   31,   32,   33,   34,   35,  189,
 /*  2970 */    37,  189,  189,   40,   41,  189,  189,  189,  189,  189,
 /*  2980 */   189,   48,   49,   50,  189,  189,   53,  189,   55,  189,
 /*  2990 */   189,  189,   59,   60,   61,   62,   63,   64,   65,   66,
 /*  3000 */    67,  189,   69,   70,   71,   72,  189,   74,  166,  189,
 /*  3010 */   189,   78,   79,   80,   81,  189,  189,   84,  176,  177,
 /*  3020 */   178,  179,  189,   90,  189,  189,  189,   94,   95,   96,
 /*  3030 */    97,   98,   99,  100,  101,  102,  103,  104,    7,    8,
 /*  3040 */     9,   10,   11,  166,  189,  189,  189,  189,  189,  189,
 /*  3050 */    19,  189,  189,  176,  177,  178,  179,  189,  189,  189,
 /*  3060 */   189,  189,   31,   32,   33,   34,   35,  189,   37,  189,
 /*  3070 */   189,   40,   41,  189,  189,  189,  189,  189,  189,   48,
 /*  3080 */    49,   50,  189,  189,   53,  189,   55,  189,  189,  189,
 /*  3090 */    59,   60,   61,   62,   63,   64,   65,   66,   67,  189,
 /*  3100 */    69,   70,   71,   72,  189,   74,  166,  189,  189,   78,
 /*  3110 */    79,   80,   81,  189,  189,   84,  176,  177,  178,  179,
 /*  3120 */   189,   90,  189,  189,  189,   94,   95,   96,   97,   98,
 /*  3130 */    99,  100,  101,  102,  103,  104,    7,    8,    9,   10,
 /*  3140 */    11,  166,  189,  189,  189,  189,  189,  189,   19,  189,
 /*  3150 */   189,  176,  177,  178,  179,  189,  189,  189,  189,  189,
 /*  3160 */    31,   32,   33,   34,   35,  189,   37,  189,  189,   40,
 /*  3170 */    41,  189,  189,  189,  189,  189,  189,   48,  189,   50,
 /*  3180 */   189,  189,   53,  189,   55,  189,  189,  189,   59,   60,
 /*  3190 */    61,   62,   63,   64,   65,   66,   67,  189,   69,   70,
 /*  3200 */    71,   72,  189,   74,  166,  189,  189,   78,   79,   80,
 /*  3210 */    81,  189,  189,   84,  176,  177,  178,  179,  189,   90,
 /*  3220 */   189,  189,  189,   94,   95,   96,   97,   98,   99,  100,
 /*  3230 */   101,  102,  103,  104,    7,    8,    9,   10,   11,  189,
 /*  3240 */    39,  189,  189,   42,  189,   44,   19,   46,   47,  189,
 /*  3250 */   189,   50,  189,  189,  189,  189,  189,  189,   31,   32,
 /*  3260 */    33,   34,   35,  189,   37,  189,   19,   40,  189,  189,
 /*  3270 */   189,  189,  189,  189,  189,   48,  189,  189,  189,  189,
 /*  3280 */    53,  189,   55,  189,  189,  189,   59,   60,   61,   62,
 /*  3290 */    63,   64,   65,   66,   67,   68,   69,  189,  189,  189,
 /*  3300 */   189,  189,  113,  114,   57,  104,   59,   60,   61,   62,
 /*  3310 */    63,   64,   65,   66,   67,   68,   69,   90,  189,  189,
 /*  3320 */   189,  189,  189,  134,  189,   98,   99,  100,  101,  102,
 /*  3330 */   103,  189,  143,  144,  145,  146,  147,  148,  149,  150,
 /*  3340 */   151,  152,  153,  154,  155,  156,  157,  158,  159,  160,
 /*  3350 */   161,  162,  113,  114,  189,  166,  189,  189,  189,  166,
 /*  3360 */   189,  189,  189,  189,  189,  176,  177,  178,  179,  176,
 /*  3370 */   177,  178,  179,  134,  189,    1,    2,    3,    4,    5,
 /*  3380 */   189,  188,  143,  144,  145,  146,  147,  148,  149,  150,
 /*  3390 */   151,  152,  153,  154,  155,  156,  157,  158,  159,  160,
 /*  3400 */   161,  162,  113,  114,  189,  166,  189,  189,  189,  189,
 /*  3410 */   189,  189,  189,  189,   40,  176,  177,  178,  179,  189,
 /*  3420 */    46,   47,  189,  134,  189,  189,  189,  189,   54,  189,
 /*  3430 */    56,  189,  143,  144,  145,  146,  147,  148,  149,  150,
 /*  3440 */   151,  152,  153,  154,  155,  156,  157,  158,  159,  160,
 /*  3450 */   161,  162,  113,  114,  189,  166,  189,  189,  189,  166,
 /*  3460 */   189,  189,  189,  189,  189,  176,  177,  178,  179,  176,
 /*  3470 */   177,  178,  179,  134,  189,    1,    2,    3,    4,    5,
 /*  3480 */   189,  189,  143,  144,  145,  146,  147,  148,  149,  150,
 /*  3490 */   151,  152,  153,  154,  155,  156,  157,  158,  159,  160,
 /*  3500 */   161,  162,  113,  114,  189,  166,  189,  189,  189,  189,
 /*  3510 */   189,  189,  189,  189,   40,  176,  177,  178,  179,  189,
 /*  3520 */    46,   47,  189,  134,  189,  189,  189,  189,   54,  189,
 /*  3530 */    56,  189,  143,  144,  145,  146,  147,  148,  149,  150,
 /*  3540 */   151,  152,  153,  154,  155,  156,  157,  158,  159,  160,
 /*  3550 */   161,  162,  113,  114,  189,  166,  189,  189,   37,  189,
 /*  3560 */   189,  189,  189,  189,  189,  176,  177,  178,  179,  189,
 /*  3570 */   189,  189,   51,  134,  189,    1,    2,    3,    4,    5,
 /*  3580 */   189,  189,  143,  144,  145,  146,  147,  148,  149,  150,
 /*  3590 */   151,  152,  153,  154,  155,  156,  157,  158,  159,  160,
 /*  3600 */   161,  162,  113,  114,  189,  166,   85,   86,   87,   88,
 /*  3610 */    89,  189,  189,   92,   93,  176,  177,  178,  179,  189,
 /*  3620 */    46,   47,  189,  134,  189,  189,  189,  189,   54,  189,
 /*  3630 */    56,  189,  143,  144,  145,  146,  147,  148,  149,  150,
 /*  3640 */   151,  152,  153,  154,  155,  156,  157,  158,  159,  160,
 /*  3650 */   161,  162,  113,  114,  189,  166,  189,  189,  189,  166,
 /*  3660 */   189,  189,  189,  189,  189,  176,  177,  178,  179,  176,
 /*  3670 */   177,  178,  179,  134,  189,    1,    2,    3,    4,    5,
 /*  3680 */   189,  189,  143,  144,  145,  146,  147,  148,  149,  150,
 /*  3690 */   151,  152,  153,  154,  155,  156,  157,  158,  159,  160,
 /*  3700 */   161,  162,  113,  114,  189,  166,  189,  189,  189,  189,
 /*  3710 */   189,  189,  189,  189,  189,  176,  177,  178,  179,  189,
 /*  3720 */    46,   47,  189,  134,  189,  189,  189,  189,   54,  189,
 /*  3730 */    56,  189,  143,  144,  145,  146,  147,  148,  149,  150,
 /*  3740 */   151,  152,  153,  154,  155,  156,  157,  158,  159,  160,
 /*  3750 */   161,  162,  113,  114,  189,  166,  189,  189,  189,  189,
 /*  3760 */   189,  189,  189,  189,  189,  176,  177,  178,  179,  189,
 /*  3770 */   189,  189,  189,  134,  189,    1,    2,    3,    4,    5,
 /*  3780 */   189,  189,  143,  144,  145,  146,  147,  148,  149,  150,
 /*  3790 */   151,  152,  153,  154,  155,  156,  157,  158,  159,  160,
 /*  3800 */   161,  162,  113,  114,  189,  166,  189,  189,  189,  189,
 /*  3810 */   189,  189,  189,  189,  189,  176,  177,  178,  179,  189,
 /*  3820 */    46,   47,  189,  134,  189,  189,  189,  189,   54,  189,
 /*  3830 */    56,  189,  143,  144,  145,  146,  147,  148,  149,  150,
 /*  3840 */   151,  152,  153,  154,  155,  156,  157,  158,  159,  160,
 /*  3850 */   161,  162,  113,  114,  189,  166,  189,  189,  189,  189,
 /*  3860 */   189,  189,  189,  189,  189,  176,  177,  178,  179,  189,
 /*  3870 */   189,  189,  189,  134,  189,    1,    2,    3,    4,    5,
 /*  3880 */   189,  189,  143,  144,  145,  146,  147,  148,  149,  150,
 /*  3890 */   151,  152,  153,  154,  155,  156,  157,  158,  159,  160,
 /*  3900 */   161,  162,  113,  114,  189,  166,  189,  189,  189,  189,
 /*  3910 */   189,  189,  189,  189,  189,  176,  177,  178,  179,  189,
 /*  3920 */    46,   47,  189,  134,  189,  189,  189,  189,   54,  189,
 /*  3930 */    56,  189,  143,  144,  145,  146,  147,  148,  149,  150,
 /*  3940 */   151,  152,  153,  154,  155,  156,  157,  158,  159,  160,
 /*  3950 */   161,  162,  113,  114,  189,  166,  189,  189,  189,  189,
 /*  3960 */   189,  189,  189,  189,  189,  176,  177,  178,  179,  189,
 /*  3970 */   189,  189,  189,  134,  189,    1,    2,    3,    4,    5,
 /*  3980 */   189,  189,  143,  144,  145,  146,  147,  148,  149,  150,
 /*  3990 */   151,  152,  153,  154,  155,  156,  157,  158,  159,  160,
 /*  4000 */   161,  162,  113,  114,  189,  166,  189,  189,  189,  189,
 /*  4010 */   189,  189,  189,  189,  189,  176,  177,  178,  179,  189,
 /*  4020 */    46,   47,  189,  134,  189,  189,   52,  189,  189,  189,
 /*  4030 */    56,  189,  143,  144,  145,  146,  147,  148,  149,  150,
 /*  4040 */   151,  152,  153,  154,  155,  156,  157,  158,  159,  160,
 /*  4050 */   161,  162,  113,  114,  189,  166,  189,  189,  189,  166,
 /*  4060 */   189,  189,  189,  189,  189,  176,  177,  178,  179,  176,
 /*  4070 */   177,  178,  179,  134,  189,  189,  183,  189,  189,  186,
 /*  4080 */   187,  188,  143,  144,  145,  146,  147,  148,  149,  150,
 /*  4090 */   151,  152,  153,  154,  155,  156,  157,  158,  159,  160,
 /*  4100 */   161,  162,  113,  114,  189,  166,  189,  189,  189,  189,
 /*  4110 */   189,  189,  189,  189,  189,  176,  177,  178,  179,  189,
 /*  4120 */   189,  189,  189,  134,  189,    1,    2,    3,    4,    5,
 /*  4130 */   189,  189,  143,  144,  145,  146,  147,  148,  149,  150,
 /*  4140 */   151,  152,  153,  154,  155,  156,  157,  158,  159,  160,
 /*  4150 */   161,  162,  113,  114,  189,  166,  189,  189,  189,  189,
 /*  4160 */   189,  189,  189,  189,  189,  176,  177,  178,  179,  189,
 /*  4170 */    46,   47,  189,  134,  189,  189,  189,  189,  189,  189,
 /*  4180 */    56,  189,  143,  144,  145,  146,  147,  148,  149,  150,
 /*  4190 */   151,  152,  153,  154,  155,  156,  157,  158,  159,  160,
 /*  4200 */   161,  162,  113,  114,  189,  166,  189,  189,  189,  166,
 /*  4210 */   189,  189,  189,  189,  189,  176,  177,  178,  179,  176,
 /*  4220 */   177,  178,  179,  134,  189,  189,  189,  189,  189,  186,
 /*  4230 */   187,  188,  143,  144,  145,  146,  147,  148,  149,  150,
 /*  4240 */   151,  152,  153,  154,  155,  156,  157,  158,  159,  160,
 /*  4250 */   161,  162,  113,  114,  189,  166,  189,  189,  189,  189,
 /*  4260 */   189,  189,  189,  189,  189,  176,  177,  178,  179,  189,
 /*  4270 */   189,  189,  189,  134,  189,    1,    2,    3,    4,    5,
 /*  4280 */   189,  189,  143,  144,  145,  146,  147,  148,  149,  150,
 /*  4290 */   151,  152,  153,  154,  155,  156,  157,  158,  159,  160,
 /*  4300 */   161,  162,  113,  114,  189,  166,  189,  189,  189,  189,
 /*  4310 */   189,  189,  189,  189,  189,  176,  177,  178,  179,  189,
 /*  4320 */    46,   47,  189,   49,   50,  189,   52,  189,  189,  189,
 /*  4330 */    56,  189,  143,  144,  145,  146,  147,  148,  149,  150,
 /*  4340 */   151,  152,  153,  154,  155,  156,  157,  158,  159,  160,
 /*  4350 */   161,  162,  189,  189,  189,  166,  189,    7,    8,    9,
 /*  4360 */    10,   11,  189,  189,  189,  176,  177,  178,  179,   19,
 /*  4370 */   189,  189,  189,  189,  189,  189,  189,  189,  189,  189,
 /*  4380 */   189,   31,   32,   33,   34,   35,   36,   37,  189,  166,
 /*  4390 */    40,  189,  189,  189,  189,  189,  189,  189,   48,  176,
 /*  4400 */   177,  178,  179,   53,  189,   55,  189,  184,  185,  189,
 /*  4410 */   189,   12,   13,   14,  189,  189,   17,   18,   19,   20,
 /*  4420 */    21,   22,   23,   24,   25,   26,   27,   28,   29,   30,
 /*  4430 */   189,  189,  189,  189,  189,   36,   37,   38,  189,  189,
 /*  4440 */    90,  189,  189,  189,  189,  189,  189,  189,   98,   99,
 /*  4450 */   100,  101,  102,  103,  166,  189,    7,    8,    9,   10,
 /*  4460 */    11,  189,  189,  189,  176,  177,  178,  179,   19,  189,
 /*  4470 */   189,  189,  184,  185,  189,  189,  189,  189,  189,  189,
 /*  4480 */    31,   32,   33,   34,   35,   36,   37,  189,  166,   40,
 /*  4490 */   189,  189,  189,  189,  189,  189,  189,   48,  176,  177,
 /*  4500 */   178,  179,   53,  189,   55,  189,  184,  185,  189,  189,
 /*  4510 */    12,   13,   14,  189,  189,   17,   18,   19,   20,   21,
 /*  4520 */    22,   23,   24,   25,   26,   27,   28,   29,   30,  189,
 /*  4530 */   189,  189,  189,  189,  189,   37,   38,  189,  189,   90,
 /*  4540 */   189,  189,  189,  189,  189,  189,   48,   98,   99,  100,
 /*  4550 */   101,  102,  103,  166,  189,    7,    8,    9,   10,   11,
 /*  4560 */   189,  189,  189,  176,  177,  178,  179,   19,  189,  189,
 /*  4570 */   189,  184,  185,  189,  189,  189,  189,  189,  189,   31,
 /*  4580 */    32,   33,   34,   35,   36,   37,  189,  166,   40,  189,
 /*  4590 */   189,  189,  189,  189,  189,  189,   48,  176,  177,  178,
 /*  4600 */   179,   53,  189,   55,  189,  184,  185,  189,  189,   12,
 /*  4610 */    13,   14,  189,  189,   17,   18,   19,   20,   21,   22,
 /*  4620 */    23,   24,   25,   26,   27,   28,   29,   30,    1,    2,
 /*  4630 */     3,    4,    5,  189,   37,   38,  189,  189,   90,  189,
 /*  4640 */   189,  189,  189,  189,  189,   48,   98,   99,  100,  101,
 /*  4650 */   102,  103,  166,  189,    7,    8,    9,   10,   11,    0,
 /*  4660 */   189,  189,  176,  177,  178,  179,   19,  189,  189,  189,
 /*  4670 */   184,  185,  189,   46,   47,  189,   49,   50,   31,   32,
 /*  4680 */    33,   34,   35,   56,   37,  189,  189,   40,   41,  189,
 /*  4690 */   189,  189,  189,  189,  189,   48,  189,  189,   39,  189,
 /*  4700 */    53,   42,   55,   44,  189,   46,   47,  189,  189,   50,
 /*  4710 */   189,  189,  189,   12,   13,   14,  189,  189,   17,   18,
 /*  4720 */    19,   20,   21,   22,   23,   24,   25,   26,   27,   28,
 /*  4730 */    29,   30,  189,  189,  189,  189,  189,   90,   37,   38,
 /*  4740 */   189,  189,  189,  189,  189,   98,   99,  100,  101,  102,
 /*  4750 */   103,  189,  189,    7,    8,    9,   10,   11,  189,   58,
 /*  4760 */   189,  189,  189,  104,  189,   19,  189,  189,  189,  189,
 /*  4770 */   189,  189,  189,  189,  189,  189,  189,   31,   32,   33,
 /*  4780 */    34,   35,  189,   37,  189,  189,   40,  189,  189,  189,
 /*  4790 */   189,  189,  189,  189,   48,  189,  189,  166,  189,   53,
 /*  4800 */   189,   55,  189,  189,   58,  189,  189,  176,  177,  178,
 /*  4810 */   179,  189,   12,   13,   14,  184,  185,   17,   18,   19,
 /*  4820 */    20,   21,   22,   23,   24,   25,   26,   27,   28,   29,
 /*  4830 */    30,  189,  189,  189,  189,  189,   90,   37,   38,  189,
 /*  4840 */   189,  189,  189,  189,   98,   99,  100,  101,  102,  103,
 /*  4850 */   166,  189,    7,    8,    9,   10,   11,  189,   58,  189,
 /*  4860 */   176,  177,  178,  179,   19,  189,  189,  189,  184,  185,
 /*  4870 */   189,  189,  189,  189,  189,  189,   31,   32,   33,   34,
 /*  4880 */    35,  189,   37,  189,  189,   40,  189,  189,  189,  189,
 /*  4890 */   189,  166,  189,   48,  189,  189,  189,  189,   53,  189,
 /*  4900 */    55,  176,  177,  178,  179,  189,   12,   13,   14,  184,
 /*  4910 */   185,   17,   18,   19,   20,   21,   22,   23,   24,   25,
 /*  4920 */    26,   27,   28,   29,   30,  189,  189,  189,   83,  189,
 /*  4930 */   189,   37,   38,  189,  189,   90,  189,  189,  189,  189,
 /*  4940 */   189,  189,   48,   98,   99,  100,  101,  102,  103,  189,
 /*  4950 */   189,    7,    8,    9,   10,   11,  189,  189,  189,  189,
 /*  4960 */   189,  189,  189,   19,  189,  189,  189,  189,  189,  189,
 /*  4970 */   189,  189,  189,  189,  189,   31,   32,   33,   34,   35,
 /*  4980 */    36,   37,  189,  166,   40,  189,  189,  189,  189,  189,
 /*  4990 */   189,  189,   48,  176,  177,  178,  179,   53,  189,   55,
 /*  5000 */   189,  184,  185,  189,  189,  189,  189,  189,  189,  189,
 /*  5010 */    12,   13,   14,  189,  189,   17,   18,   19,   20,   21,
 /*  5020 */    22,   23,   24,   25,   26,   27,   28,   29,   30,  189,
 /*  5030 */   189,  189,  189,  189,   90,   37,   38,  189,  189,   41,
 /*  5040 */   189,  189,   98,   99,  100,  101,  102,  103,  189,  189,
 /*  5050 */     7,    8,    9,   10,   11,  189,  189,  189,  189,  189,
 /*  5060 */   189,  189,   19,  189,  189,  189,  189,  189,  189,  189,
 /*  5070 */   189,  189,  189,  189,   31,   32,   33,   34,   35,  189,
 /*  5080 */    37,  189,  189,   40,  189,  189,  189,  189,  189,  189,
 /*  5090 */   189,   48,  189,  189,  189,  189,   53,  189,   55,  189,
 /*  5100 */   189,   58,  189,  189,  189,  189,  189,  189,  189,   12,
 /*  5110 */    13,   14,  189,  189,   17,   18,   19,   20,   21,   22,
 /*  5120 */    23,   24,   25,   26,   27,   28,   29,   30,  189,  189,
 /*  5130 */   189,  189,  189,   90,   37,   38,  189,  189,  189,  189,
 /*  5140 */   189,   98,   99,  100,  101,  102,  103,  166,  189,    7,
 /*  5150 */     8,    9,   10,   11,  189,   58,  189,  176,  177,  178,
 /*  5160 */   179,   19,  189,  189,  189,  189,  185,  189,  189,  189,
 /*  5170 */   189,  189,  189,   31,   32,   33,   34,   35,   36,   37,
 /*  5180 */   189,  189,   40,  189,  189,  189,  189,  189,  189,  189,
 /*  5190 */    48,  189,  189,  189,  189,   53,  189,   55,  189,  189,
 /*  5200 */   189,  189,  189,  189,  189,  189,  189,  189,   12,   13,
 /*  5210 */    14,  189,  189,   17,   18,   19,   20,   21,   22,   23,
 /*  5220 */    24,   25,   26,   27,   28,   29,   30,  189,  189,  189,
 /*  5230 */   189,  189,   90,   37,   38,  189,  189,  189,  189,  189,
 /*  5240 */    98,   99,  100,  101,  102,  103,  189,  189,    7,    8,
 /*  5250 */     9,   10,   11,  189,   58,  189,  189,  189,  189,  189,
 /*  5260 */    19,  189,  189,  189,  189,  189,  189,  189,  189,  189,
 /*  5270 */   189,  189,   31,   32,   33,   34,   35,   36,   37,  189,
 /*  5280 */   189,   40,  189,  189,  189,  189,  189,  189,  189,   48,
 /*  5290 */   189,  189,  189,  189,   53,  189,   55,  189,  189,  189,
 /*  5300 */   189,  189,  189,  189,  189,  189,  189,   12,   13,   14,
 /*  5310 */   189,  189,   17,   18,   19,   20,   21,   22,   23,   24,
 /*  5320 */    25,   26,   27,   28,   29,   30,  189,  189,  189,  189,
 /*  5330 */   189,   90,   37,   38,  189,  189,   41,  189,  189,   98,
 /*  5340 */    99,  100,  101,  102,  103,  189,  189,    7,    8,    9,
 /*  5350 */    10,   11,  189,  189,  189,  189,  189,  189,  189,   19,
 /*  5360 */   189,  189,  189,  189,  189,  189,  189,  189,  189,  189,
 /*  5370 */   189,   31,   32,   33,   34,   35,  189,   37,  189,  189,
 /*  5380 */    40,  189,  189,  189,  189,  189,  189,  189,   48,  189,
 /*  5390 */   189,  189,  189,   53,  189,   55,  189,  189,   58,  189,
 /*  5400 */   189,   12,   13,   14,  189,  189,   17,   18,   19,   20,
 /*  5410 */    21,   22,   23,   24,   25,   26,   27,   28,   29,   30,
 /*  5420 */   189,  189,  189,  189,  189,  189,   37,   38,  189,  189,
 /*  5430 */    90,  189,  189,  189,  189,  189,  189,   48,   98,   99,
 /*  5440 */   100,  101,  102,  103,  189,  189,    7,    8,    9,   10,
 /*  5450 */    11,  189,  189,  189,  189,  189,  189,  189,   19,  189,
 /*  5460 */   189,  189,  189,  189,  189,  189,  189,  189,  189,  189,
 /*  5470 */    31,   32,   33,   34,   35,   36,   37,  189,  189,   40,
 /*  5480 */   189,  189,  189,  189,  189,  189,  189,   48,  189,  189,
 /*  5490 */   189,  189,   53,  189,   55,  189,  189,  189,  189,  189,
 /*  5500 */    12,   13,   14,  189,  189,   17,   18,   19,   20,   21,
 /*  5510 */    22,   23,   24,   25,   26,   27,   28,   29,   30,  189,
 /*  5520 */   189,  189,  189,  189,  189,   37,   38,  189,  189,   90,
 /*  5530 */   189,  189,  189,  189,  189,  189,  189,   98,   99,  100,
 /*  5540 */   101,  102,  103,  189,  189,    7,    8,    9,   10,   11,
 /*  5550 */   189,  189,  189,  189,  189,  189,  189,   19,  189,  189,
 /*  5560 */   189,  189,  189,  189,  189,  189,  189,  189,  189,   31,
 /*  5570 */    32,   33,   34,   35,   36,   37,  189,  189,   40,  189,
 /*  5580 */   189,  189,  189,  189,  189,  189,   48,  189,  189,  189,
 /*  5590 */   189,   53,  189,   55,  189,  189,  189,  189,  189,  189,
 /*  5600 */    13,   14,  189,  189,   17,   18,   19,   20,   21,   22,
 /*  5610 */    23,   24,   25,   26,   27,   28,   29,   30,    1,    2,
 /*  5620 */     3,    4,    5,  189,   37,   38,  189,  189,   90,  189,
 /*  5630 */   189,  189,  189,  189,  189,  189,   98,   99,  100,  101,
 /*  5640 */   102,  103,  189,  189,    7,    8,    9,   10,   11,  189,
 /*  5650 */   189,  189,  189,  189,  189,  189,   19,  189,  189,  189,
 /*  5660 */   189,  189,  189,   46,   47,  189,   49,   50,   31,   32,
 /*  5670 */    33,   34,   35,   56,   37,  189,  189,   40,  189,  189,
 /*  5680 */   189,  189,  189,  189,  189,   48,  189,  189,  189,  189,
 /*  5690 */    53,  189,   55,  189,  189,   58,  189,  189,  189,  189,
 /*  5700 */   189,  189,  189,   17,   18,   19,   20,   21,   22,   23,
 /*  5710 */    24,   25,   26,   27,   28,   29,   30,  189,  189,  189,
 /*  5720 */   189,  189,  189,   37,   38,  189,  189,   90,  189,  189,
 /*  5730 */   189,  189,  189,  189,  189,   98,   99,  100,  101,  102,
 /*  5740 */   103,  189,  189,    7,    8,    9,   10,   11,  189,  189,
 /*  5750 */   189,  189,  189,  189,  189,   19,  189,   19,  189,  189,
 /*  5760 */   189,  189,  189,  189,  189,  189,  189,   31,   32,   33,
 /*  5770 */    34,   35,   36,   37,   36,   19,   40,  189,   40,  189,
 /*  5780 */   189,  189,  189,  189,   48,  189,  189,  189,  189,   53,
 /*  5790 */   189,   55,   36,  189,  189,  189,   40,   59,   60,   61,
 /*  5800 */    62,   63,   64,   65,   66,   67,   68,   69,  189,  189,
 /*  5810 */   189,  189,  189,  189,  189,   59,   60,   61,   62,   63,
 /*  5820 */    64,   65,   66,   67,   68,   69,   90,  189,  189,  189,
 /*  5830 */   189,  189,  189,  189,   98,   99,  100,  101,  102,  103,
 /*  5840 */   189,  189,    7,    8,    9,   10,   11,  189,  189,  189,
 /*  5850 */   189,  189,  189,  189,   19,  189,   19,  189,  189,  189,
 /*  5860 */    37,   38,  189,  189,  189,  189,   31,   32,   33,   34,
 /*  5870 */    35,  189,   37,   36,   51,   40,  189,   40,  189,  189,
 /*  5880 */   189,  189,  189,   48,  189,  189,  189,  189,   53,  189,
 /*  5890 */    55,  189,  189,  189,  189,  189,   59,   60,   61,   62,
 /*  5900 */    63,   64,   65,   66,   67,   68,   69,  189,   85,   86,
 /*  5910 */    87,   88,   89,  189,   91,   92,   93,  189,   83,  189,
 /*  5920 */   189,  189,  189,  189,  189,   90,  189,  189,  189,  189,
 /*  5930 */   189,  189,  189,   98,   99,  100,  101,  102,  103,  189,
 /*  5940 */   189,    7,    8,    9,   10,   11,  189,  189,  189,  189,
 /*  5950 */   189,  189,  189,   19,  189,   19,  189,  189,  189,  189,
 /*  5960 */   189,  189,  189,  189,  189,   31,   32,   33,   34,   35,
 /*  5970 */    36,   37,   36,   19,   40,  189,   40,  189,  189,  189,
 /*  5980 */   189,  189,   48,  189,  189,  189,  189,   53,  189,   55,
 /*  5990 */   189,  189,  189,  189,   40,   59,   60,   61,   62,   63,
 /*  6000 */    64,   65,   66,   67,   68,   69,  189,  189,  189,  189,
 /*  6010 */   189,  189,  189,   59,   60,   61,   62,   63,   64,   65,
 /*  6020 */    66,   67,   68,   69,   90,  189,  189,  189,  189,  189,
 /*  6030 */   189,  189,   98,   99,  100,  101,  102,  103,  189,  189,
 /*  6040 */     7,    8,    9,   10,   11,  189,  189,  189,  189,  189,
 /*  6050 */   189,  189,   19,   19,  189,  189,  189,  189,  189,   40,
 /*  6060 */   189,  189,  189,  189,   31,   32,   33,   34,   35,  189,
 /*  6070 */    37,  189,   53,   40,  189,  189,  189,  189,  189,  189,
 /*  6080 */   189,   48,  189,  189,  189,  189,   53,  189,   55,  189,
 /*  6090 */   189,  189,  189,   59,   60,   61,   62,   63,   64,   65,
 /*  6100 */    66,   67,   68,   69,    1,    2,    3,    4,    5,   90,
 /*  6110 */   189,  189,  189,  189,  189,  189,  189,   98,   99,  100,
 /*  6120 */   101,  102,  103,   90,  189,  189,  189,  189,  189,  189,
 /*  6130 */   189,   98,   99,  100,  101,  102,  103,  189,  189,    7,
 /*  6140 */     8,    9,   10,   11,    1,    2,    3,    4,    5,   46,
 /*  6150 */    47,   19,   49,   50,  189,   52,  189,  189,  189,   56,
 /*  6160 */   189,  189,  189,   31,   32,   33,   34,   35,  189,   37,
 /*  6170 */   189,  189,   40,  189,  189,  189,  189,  189,  189,  189,
 /*  6180 */    48,  189,  189,  189,  189,   53,  189,   55,  189,   46,
 /*  6190 */    47,  189,   49,   50,  189,   52,  189,  189,  189,   56,
 /*  6200 */   189,    1,    2,    3,    4,    5,  189,  189,  189,  189,
 /*  6210 */   189,  189,  189,  189,  189,  189,  189,  189,  189,  189,
 /*  6220 */   189,  189,   90,  189,    1,    2,    3,    4,    5,  189,
 /*  6230 */    98,   99,  100,  101,  102,  103,    1,    2,    3,    4,
 /*  6240 */     5,    1,    2,    3,    4,    5,   46,   47,  189,   49,
 /*  6250 */    50,  189,   52,  189,  189,  189,   56,  189,    1,    2,
 /*  6260 */     3,    4,    5,    1,    2,    3,    4,    5,  189,   46,
 /*  6270 */    47,  189,   49,   50,    1,    2,    3,    4,    5,   56,
 /*  6280 */   189,   46,   47,  189,   49,   50,   46,   47,  189,   49,
 /*  6290 */    50,   56,  189,  189,  189,  189,   56,  189,  189,  189,
 /*  6300 */   189,  189,  189,   46,   47,  189,   49,   50,   46,   47,
 /*  6310 */   189,   49,   50,   56,  189,  189,  189,  189,   56,   46,
 /*  6320 */    47,  189,   49,   50,  189,  189,  189,  189,  189,   56,
};
#define YY_SHIFT_USE_DFLT (-16)
static short yy_shift_ofst[] = {
 /*     0 */  3201,  138, 4659,  -16,  -16,  -16,  -16,  -16,  -16,   38,
 /*    10 */   216,  -16,  232,  419,  -16,  358,  293,  -16, 6273,  -16,
 /*    20 */  6257,  -16,  -16, 3574,  488,  486, 5936,  677, 3247,  554,
 /*    30 */   -16,  -16,  594, 6034,  -16,  588,  -16,  -16,  -16,  618,
 /*    40 */   619,  -16,  574,  673,  -16,  -16,  -16,  -16,  -16,  -16,
 /*    50 */   -16,  -16,  -16,  -16,  -16,  -16,  -16,  -16,  658,  579,
 /*    60 */  3247,  659,  -16,  -16, 5954,  -16,  656, 6019,  -16,  -16,
 /*    70 */   -16,  -16,  -16,  -16,  -16,  -16,  639,  684,  -16,  -16,
 /*    80 */   494,  719, 6019,  -16,  722,  739, 6019,  -16,  757,  747,
 /*    90 */  6019,  -16,  765,  786,  -16,  -16,  -16,  -16,  -16,  -16,
 /*   100 */   -16,  -16,  -16,  -16, 4124, 3774,  767,  758, 5738,  753,
 /*   110 */  3247,  774,  -16,  -16,   25,  195, 3247,  775,  -16,  -16,
 /*   120 */   -16,  -16,  779,  464,  -16,  781,  278,  -16,  783,  328,
 /*   130 */   -16, 6103,  -16,  780,  -16, 6143, 5617,  -16, 3874,  788,
 /*   140 */   776, 5756,   81, 2149,  -16,  287,  -16,  -16,  -16,  -16,
 /*   150 */   -16,  -16,  -16,  -16,  -16,  -16,  -16,  -16,  -16,  -16,
 /*   160 */   -16,  -16,  -16,  -16,  -16,  -16,  -16,  -16,  -16,  -16,
 /*   170 */   792,  -16,  793,  -16, 6033,  787, 1463,  385,  772,  804,
 /*   180 */   483,  973,  -16, 6033,  806,  268,  -16,   72,  -16,  -16,
 /*   190 */  6019,  794, 3129, 3129,  814, 2345, 2541,  -16, 6033,  820,
 /*   200 */  2639,   -7,  -16,  821,  189, 2247,  809, 6033,  855,  -16,
 /*   210 */  6033,  636, 6033,  862,  -16,  688, 6033, 6033, 5488, 6033,
 /*   220 */  6033, 5488, 6033, 6033, 5488, 6033, 6033, 5488, 6033, 6033,
 /*   230 */  5488, 6033,   78, 6033,   78, 6033,   78,  716,  848, 4548,
 /*   240 */  5488, 6033, 5196,  -16, 6033,  301, 6033,  301, 6033,  636,
 /*   250 */  6033,  636, 6033,  636, 6033,  301, 6033, 5686, 6033, 5686,
 /*   260 */  6033, 5587, 3227, 4399,  -16,  868, 6033,  636,  871,  893,
 /*   270 */  6033,   78,  642,  384,  859, 5439,  877,  912,  862,  -16,
 /*   280 */   -16, 6033,  636,  -16,  -16,  -16,  -16,  -16,  -16,  -16,
 /*   290 */   -16, 4746, 5488,   54, 4449,  683,  865, 5142,  -16,  718,
 /*   300 */   -16, 6033,  881,  873,  617, 5736,  700,  -16,  -16,  -16,
 /*   310 */   -16,  -16,  -16,  562,  872, 4350,  711,  -16,  -16,  886,
 /*   320 */   880,  876, 5538,  686,  -16,  -16,  -16,  892,  884,  888,
 /*   330 */  5241,  -16,  627,  -16,  613,  -16,  -16,  890,  891,  -16,
 /*   340 */   550,  -16, 6132,  -16,  910, 6033,  -16,  -16,  -16,  601,
 /*   350 */   -16,  -16,  -16,  592,  -16,  -16,  534,  899,  896, 4944,
 /*   360 */   590,  -16,  -16,  903,  902, 5934,  504,  -16,  -16,   78,
 /*   370 */    78,   78,   78, 5097,  -16,  518,  -16,  461,  909,  -16,
 /*   380 */   915,  -16,   78, 5488,  928,  153, 5835, 4597, 3129, 2051,
 /*   390 */   -16,  947,  524,  -16,  947,  -16, 5823,  -16,  -16,  -16,
 /*   400 */   -16,  -16,  -16, 6033,  -16, 5488,  162, 3521, 6033,  -16,
 /*   410 */  5043,  215, 6033,  -16,  930,  215, 6033,  -16, 4701,  -16,
 /*   420 */   -16,  265,  942,  215, 6033,  -16,  952,  215, 6033,  -16,
 /*   430 */   -11,  323, 6033,  -16, 5637,  215, 6033,  -16,  953,  215,
 /*   440 */  6033,  -16, 5340,  215, 6033,  -16,  958,  -16,  127, 6033,
 /*   450 */   -16,  -16, 6033,  -16,  -16,  -16,  -16, 6033,   73,  -16,
 /*   460 */  6033,  -16, 5488,  -16,  976,  -16,  977,  -16,  978,  -16,
 /*   470 */   979,  -16, 4647, 5295,  -16,  -16,  984,  -15,  985,  986,
 /*   480 */   -16, 6033, 4800,  988,  -16, 6033, 4998,  -16,  990,  170,
 /*   490 */   -16,  990,  -16,  995, 6019,  -16,  -16,  990,  176,  -16,
 /*   500 */   990,  241,  -16,  990,  338,  -16,  990,  374,  -16,  990,
 /*   510 */   401,  -16,  990,  462,  -16,  990,  472,  -16,  990,  498,
 /*   520 */   -16,  990,  519,  -16, 5488,  -16,  -16,  -16, 6033, 5389,
 /*   530 */  3129, 2737,  -16, 1001,  968, 4845, 4894, 3129, 1757,  -16,
 /*   540 */  6033, 4498, 3129, 3031,  -16,  983, 6033, 1014,  -16,  -16,
 /*   550 */   -16,  989, 3129, 3129,  -16,  -16,  991, 1008, 2933, 2835,
 /*   560 */   -16,  -16, 3247, 1018, 2443,  -16, 1855,  -16,  660,  563,
 /*   570 */  1365,  -16, 1169,  -16, 3247, 1036,  777,  -16,  581,  -16,
 /*   580 */  4124, 3674, 1052, 1043, 5837,  587, 1953,  -16,   91,  -16,
 /*   590 */  3247, 1051,  679,  -16,  875,  -16,  699,  661, 1071,  -16,
 /*   600 */  1267,  -16, 3247, 1053, 1561,  -16, 1659,  -16, 6200, 6262,
 /*   610 */   -16,  -16, 3974, 1047, 1056, 6019, 1068,  -16, 1057, 1063,
 /*   620 */  6019, 1074,  -16,  -16, 3974, 3474,  132,  -16, 6019,  136,
 /*   630 */   -16, 1075,  -16,  169, 1076,    0, 1081,  721,  -16,  -16,
 /*   640 */  1088,  -16,  -16, 1103,  -16, 3374,  223,  -16, 6019,  412,
 /*   650 */   -16, 1107,  -16, 1119,  -16,  -16, 4274, 6235, 6240, 4124,
 /*   660 */  4627,  -16, 6223,  783,  -16,  -16,  -16,  783,  328,  -16,
 /*   670 */  1120, 1123,  330,  -16, 1143, 1136,  -16,  783,  328,  -16,
 /*   680 */  1146, 1147,  551,  -16, 1156, 1136,  -16,  -16,
};
#define YY_REDUCE_USE_DFLT (-131)
static short yy_reduce_ofst[] = {
 /*     0 */   -93, -131, 1660, -131, -131, -131, -131, -131, -131, -131,
 /*    10 */  -131, -131, -131,  210, -131, -131,  316, -131,  307, -131,
 /*    20 */   -85, -131, -131,  334, -131, -131,  -22, -131,  760, -131,
 /*    30 */  -131, -131, -131,  175, -131, -131, -131, -131, -131, -131,
 /*    40 */  -131, -131, -131, -131, -131, -131, -131, -131, -131, -131,
 /*    50 */  -131, -131, -131, -131, -131, -131, -131, -131, -131, -131,
 /*    60 */   752, -131, -131, -131,  101, -131, -131,  595, -131, -131,
 /*    70 */  -131, -131, -131, -131, -131, -131, -131, -131, -131, -131,
 /*    80 */  -131, -131,  646, -131, -131, -131,  669, -131, -131, -131,
 /*    90 */   678, -131, -131, -131, -131, -131, -131, -131, -131, -131,
 /*   100 */  -131, -131, -131, -131,  667,  334, -131, -131,   92, -131,
 /*   110 */   956, -131, -131, -131, -131, -131, -130, -131, -131, -131,
 /*   120 */  -131, -131, -131,  704, -131, -131,  706, -131,  245,  708,
 /*   130 */  -131,  -86, -131, -131, -131,  375,  305, -131,  334, -131,
 /*   140 */  -131,  230, -131, 3289, -131, 4189, -131, -131, -131, -131,
 /*   150 */  -131, -131, -131, -131, -131, -131, -131, -131, -131, -131,
 /*   160 */  -131, -131, -131, -131, -131, -131, -131, -131, -131, -131,
 /*   170 */  -131, -131, -131, -131,  295, -131, 3739, 4189, -131, -131,
 /*   180 */  3889, 4189, -131,  491, -131,  411, -131,  694, -131, -131,
 /*   190 */   734, -131, 3239, 4189, -131, 3989, 4189, -131,  687, -131,
 /*   200 */  3789, 4189, -131, -131, 3839, 4189, -131,   99, -131, -131,
 /*   210 */  2387, -131, 2548,  723, -131, -131, 2681, 2779, -131, 3038,
 /*   220 */   980, -131, 2254, 3293, -131, 2191, 1078, -131, 2450, 2842,
 /*   230 */  -131, 2744, -131, 2583, -131, 2289, -131, -131, -131, 4725,
 /*   240 */  -131, 2156, -131, -131, 1960, -131, 1862, -131, 1666, -131,
 /*   250 */  1568, -131, 1372, -131, 1176, -131, 1274, -131, 1764, -131,
 /*   260 */  2058, -131, 1995, -131, -131, -131, 2352, -131, -131, -131,
 /*   270 */  1470, -131, -131, -131, -131, 4486, -131, -131,  737, -131,
 /*   280 */  -131, 2485, -131, -131, -131, -131, -131, -131, -131, -131,
 /*   290 */  -131, 3893, -131, -131, 4421, -131, -131, 4322, -131, -131,
 /*   300 */  -131, 4981, -131, -131, -131, 4684, -131, -131, -131, -131,
 /*   310 */  -131, -131, -131, -131, -131, 4288, -131, -131, -131, -131,
 /*   320 */  -131, -131, 4223, -131, -131, -131, -131, -131, -131, -131,
 /*   330 */  4631, -131, -131, -131, -131, -131, -131, -131, -131, -131,
 /*   340 */  -131, -131, 4043, -131, -131, 3193, -131, -131, -131, -131,
 /*   350 */  -131, -131, -131, -131, -131, -131, -131, -131, -131, 4387,
 /*   360 */  -131, -131, -131, -131, -131, 4817, -131, -131, -131, -131,
 /*   370 */  -131, -131, -131, -131, -131, -131, -131, -131, -131, -131,
 /*   380 */  -131, -131, -131, -131, -131, -131, 2940, -131, 4039, 4189,
 /*   390 */  -131,  359, -131, -131,  822, -131,  -87, -131, -131, -131,
 /*   400 */  -131, -131, -131, 1897, -131, -131, -131,  824,  819, -131,
 /*   410 */  2093,  825, 1701, -131, -131,  830, 1015, -131, -131, -131,
 /*   420 */  -131, -131, -131,  831,  623, -131, -131,  833, 1211, -131,
 /*   430 */  -131,  834,  427, -131, 1799,  840,  525, -131, -131,  843,
 /*   440 */  1505, -131, 1407,  846, 1113, -131, -131, -131,  -89, 1309,
 /*   450 */  -131, -131, 1407, -131, -131, -131, -131,  133, -131, -131,
 /*   460 */   393, -131, -131, -131, -131, -131, -131, -131, -131, -131,
 /*   470 */  -131, -131,  784, -131, -131, -131, -131, -131, -131, -131,
 /*   480 */  -131, 3493, -131, -131, -131,  882, -131, -131,  -68, -131,
 /*   490 */  -131,  850, -131, -131,  923, -131, -131,   47, -131, -131,
 /*   500 */   100, -131, -131,  152, -131, -131,  225, -131, -131,  233,
 /*   510 */  -131, -131,  248, -131, -131,  296, -131, -131,  315, -131,
 /*   520 */  -131,  341, -131, -131, -131, -131, -131, -131, 2646, -131,
 /*   530 */  3189, 4189, -131, -131, -131, 2877, -131, 3539, 4189, -131,
 /*   540 */  2975, -131, 3439, 4189, -131, -131,  589, -131, -131, -131,
 /*   550 */  -131, -131, 3939, 4189, -131, -131, -131, -131, 4139, 4189,
 /*   560 */  -131, -131,  948, -131, 3489, -131, 4189, -131, -131, -131,
 /*   570 */  3339, -131, 4189, -131, 1640, -131, 3639, -131, 4189, -131,
 /*   580 */   383,  334, -131, -131,   -6, -131, 3389, -131, 4189, -131,
 /*   590 */    66, -131, 3689, -131, 4189, -131, -131, -131, 4089, -131,
 /*   600 */  4189, -131,  262, -131, 3589, -131, 4189, -131,   12,  305,
 /*   610 */  -131, -131,  383, -131, -131,  987, -131, -131, -131, -131,
 /*   620 */   992, -131, -131, -131,  502,  334,  996, -131,  999, 1000,
 /*   630 */  -131, -131, -131,  117, -131, -131, -131, 1010, -131, -131,
 /*   640 */  -131, -131, -131, -131, -131,  334, 1002, -131, 1021, 1026,
 /*   650 */  -131, -131, -131, -131, -131, -131,  -55,   43,  305,  502,
 /*   660 */   305, -131,  305, 1035, -131, -131, -131,  418, 1046, -131,
 /*   670 */  -131, -131, 1066, -131, -131, 1070, -131,  304, 1073, -131,
 /*   680 */  -131, -131, 1079, -131, -131, 1082, -131, -131,
};
static YYACTIONTYPE yy_default[] = {
 /*     0 */  1018, 1018, 1018,  690,  692,  693,  694,  695,  696, 1018,
 /*    10 */  1018,  697, 1018, 1018,  698, 1018, 1018,  699, 1018,  714,
 /*    20 */  1018,  715,  745, 1018, 1018, 1018, 1018, 1018, 1018, 1018,
 /*    30 */   765,  783,  784, 1018,  785,  787,  788,  789,  790, 1018,
 /*    40 */  1018,  801, 1018, 1018,  802,  803,  804,  805,  806,  807,
 /*    50 */   808,  809,  810,  811,  812,  813,  786,  769, 1018, 1018,
 /*    60 */  1018, 1018,  766,  770, 1018,  791,  793, 1018,  797, 1002,
 /*    70 */  1003, 1004, 1005, 1006, 1007, 1008, 1018, 1018, 1009, 1010,
 /*    80 */  1018,  794, 1018,  798, 1018,  795, 1018,  799, 1018,  796,
 /*    90 */  1018,  800, 1018, 1018,  792,  773,  775,  776,  777,  778,
 /*   100 */   779,  780,  781,  782, 1018, 1018, 1018, 1018, 1018, 1018,
 /*   110 */  1018, 1018,  767,  771, 1018, 1018, 1018, 1018,  768,  772,
 /*   120 */   774,  746, 1018, 1018,  700, 1018, 1018,  701, 1018, 1018,
 /*   130 */   703, 1018,  709, 1018,  710, 1018, 1018,  743, 1018, 1018,
 /*   140 */  1018, 1018, 1018, 1018,  749, 1018,  751,  814,  816,  817,
 /*   150 */   818,  819,  820,  821,  822,  823,  824,  825,  826,  827,
 /*   160 */   828,  829,  830,  831,  832,  833,  834,  835,  836,  837,
 /*   170 */  1018,  838, 1018,  839, 1018, 1018, 1018, 1018,  842, 1018,
 /*   180 */  1018, 1018,  843, 1018, 1018, 1018,  846, 1018,  847,  848,
 /*   190 */  1018, 1018,  850,  851, 1018, 1018, 1018,  854, 1018, 1018,
 /*   200 */  1018, 1018,  856, 1018, 1018, 1018, 1018, 1018, 1018,  858,
 /*   210 */  1018,  919, 1018, 1018,  920, 1018, 1018, 1018,  921, 1018,
 /*   220 */  1018,  922, 1018, 1018,  923, 1018, 1018,  924, 1018, 1018,
 /*   230 */   925, 1018,  931, 1018,  932, 1018,  933, 1018,  937, 1018,
 /*   240 */   993, 1018, 1018,  942, 1018,  943, 1018,  944, 1018,  945,
 /*   250 */  1018,  946, 1018,  947, 1018,  948, 1018,  949, 1018,  950,
 /*   260 */  1018,  951, 1018, 1018,  934, 1018, 1018,  935, 1018, 1018,
 /*   270 */  1018,  936,  955, 1018,  940, 1018, 1018, 1018, 1018,  952,
 /*   280 */   953, 1018,  954,  956,  957,  958,  959,  960,  961,  962,
 /*   290 */   963, 1018, 1001,  955, 1018, 1018,  966, 1018,  967, 1018,
 /*   300 */   968, 1018, 1018, 1018, 1018, 1018, 1018,  974,  975,  988,
 /*   310 */   989,  990,  992, 1018, 1018, 1018, 1018,  978,  979, 1018,
 /*   320 */  1018, 1018, 1018, 1018,  980,  981,  991, 1018, 1018,  969,
 /*   330 */  1018,  970, 1018,  971, 1018,  972,  973,  956,  957,  964,
 /*   340 */  1018,  965, 1018,  994, 1018, 1018,  996,  997,  995, 1018,
 /*   350 */   976,  977,  941, 1018,  982,  983, 1018, 1018,  938, 1018,
 /*   360 */  1018,  984,  985, 1018,  939, 1018, 1018,  986,  987,  930,
 /*   370 */   929,  928,  927, 1018, 1011, 1018, 1012, 1018, 1018, 1013,
 /*   380 */  1018, 1014,  926, 1015, 1018, 1018, 1018, 1018, 1018, 1018,
 /*   390 */   859, 1018, 1018,  863, 1018,  864, 1018,  866,  867,  868,
 /*   400 */   869,  870,  871, 1018,  872,  918, 1018, 1018, 1018,  873,
 /*   410 */  1018, 1018, 1018,  876, 1018, 1018, 1018,  877, 1018,  886,
 /*   420 */   887, 1018, 1018, 1018, 1018,  874, 1018, 1018, 1018,  875,
 /*   430 */  1018, 1018, 1018,  878, 1018, 1018, 1018,  879, 1018, 1018,
 /*   440 */  1018,  880, 1018, 1018, 1018,  881, 1018,  885, 1018, 1018,
 /*   450 */   882,  883, 1018,  884,  888,  889,  865, 1018, 1018,  891,
 /*   460 */  1018,  892,  894,  893,  988,  895,  990,  896,  989,  897,
 /*   470 */   953,  898, 1018, 1018,  899,  900, 1018, 1018, 1018, 1018,
 /*   480 */   901, 1018, 1018, 1018,  902, 1018, 1018,  903, 1018, 1018,
 /*   490 */   904, 1018,  914,  916, 1018,  917,  915, 1018, 1018,  905,
 /*   500 */  1018, 1018,  906, 1018, 1018,  907, 1018, 1018,  908, 1018,
 /*   510 */  1018,  909, 1018, 1018,  910, 1018, 1018,  911, 1018, 1018,
 /*   520 */   912, 1018, 1018,  913, 1018, 1016, 1017,  815, 1018, 1018,
 /*   530 */  1018, 1018,  860, 1018, 1018, 1018, 1018, 1018, 1018,  861,
 /*   540 */  1018, 1018, 1018, 1018,  862, 1018, 1018, 1018,  857,  855,
 /*   550 */   853, 1018, 1018,  852,  849,  844,  840, 1018, 1018, 1018,
 /*   560 */   845,  841, 1018, 1018, 1018,  757, 1018,  759, 1018, 1018,
 /*   570 */  1018,  750, 1018,  752, 1018, 1018, 1018,  758, 1018,  760,
 /*   580 */  1018, 1018, 1018, 1018, 1018, 1018, 1018,  753, 1018,  755,
 /*   590 */  1018, 1018, 1018,  761, 1018,  763, 1018, 1018, 1018,  754,
 /*   600 */  1018,  756, 1018, 1018, 1018,  762, 1018,  764, 1018, 1018,
 /*   610 */   741,  744, 1018, 1018, 1018, 1018, 1018,  747, 1018, 1018,
 /*   620 */  1018, 1018,  748,  725, 1018, 1018, 1018,  727, 1018, 1018,
 /*   630 */   729, 1018,  733, 1018, 1018, 1018, 1018, 1018,  737,  739,
 /*   640 */  1018,  740,  738, 1018,  731, 1018, 1018,  728, 1018, 1018,
 /*   650 */   730, 1018,  734, 1018,  732,  742, 1018, 1018, 1018, 1018,
 /*   660 */  1018,  726, 1018, 1018,  711,  713,  712, 1018, 1018,  702,
 /*   670 */  1018, 1018, 1018,  704, 1018, 1018,  705, 1018, 1018,  706,
 /*   680 */  1018, 1018, 1018,  707, 1018, 1018,  708,  691,
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
  "PARENTHESES_CLOSE",  "SBRACKET_OPEN",  "ARROW",         "NAMESPACE",   
  "IDENTIFIER",    "DOTCOMMA",      "INTERFACE",     "EXTENDS",     
  "CLASS",         "IMPLEMENTS",    "ABSTRACT",      "FINAL",       
  "BRACKET_OPEN",  "BRACKET_CLOSE",  "COMMENT",       "ASSIGN",      
  "CONST",         "CONSTANT",      "FUNCTION",      "PARENTHESES_OPEN",
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
  "CBLOCK",        "error",         "program",       "xx_language", 
  "xx_top_statement_list",  "xx_top_statement",  "xx_namespace_def",  "xx_class_def",
  "xx_interface_def",  "xx_comment",    "xx_cblock",     "xx_interface_body",
  "xx_class_body",  "xx_implements_list",  "xx_class_definition",  "xx_implements",
  "xx_interface_methods_definition",  "xx_class_properties_definition",  "xx_class_consts_definition",  "xx_class_methods_definition",
  "xx_class_property_definition",  "xx_visibility_list",  "xx_literal_expr",  "xx_class_property_shortcuts",
  "xx_class_property_shortcuts_list",  "xx_class_property_shortcut",  "xx_class_const_definition",  "xx_class_method_definition",
  "xx_interface_method_definition",  "xx_parameter_list",  "xx_statement_list",  "xx_method_return_type",
  "xx_visibility",  "xx_method_return_type_list",  "xx_method_return_type_item",  "xx_parameter_type",
  "xx_parameter_cast",  "xx_parameter_cast_collection",  "xx_parameter",  "xx_statement",
  "xx_let_statement",  "xx_if_statement",  "xx_loop_statement",  "xx_echo_statement",
  "xx_return_statement",  "xx_fetch_statement",  "xx_fcall_statement",  "xx_mcall_statement",
  "xx_scall_statement",  "xx_unset_statement",  "xx_throw_statement",  "xx_declare_statement",
  "xx_break_statement",  "xx_continue_statement",  "xx_while_statement",  "xx_do_while_statement",
  "xx_switch_statement",  "xx_for_statement",  "xx_empty_statement",  "xx_eval_expr",
  "xx_case_clauses",  "xx_case_clause",  "xx_common_expr",  "xx_let_assignments",
  "xx_let_assignment",  "xx_assignment_operator",  "xx_assign_expr",  "xx_index_expr",
  "xx_array_offset_list",  "xx_array_offset",  "xx_echo_expressions",  "xx_echo_expression",
  "xx_mcall_expr",  "xx_fcall_expr",  "xx_scall_expr",  "xx_fetch_expr",
  "xx_declare_variable_list",  "xx_declare_variable",  "xx_isset_expr",  "xx_array_list",
  "xx_call_parameters",  "xx_call_parameter",  "xx_array_item",  "xx_array_key",
  "xx_array_value",
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
 /*   8 */ "xx_top_statement ::= xx_cblock",
 /*   9 */ "xx_namespace_def ::= NAMESPACE IDENTIFIER DOTCOMMA",
 /*  10 */ "xx_interface_def ::= INTERFACE IDENTIFIER xx_interface_body",
 /*  11 */ "xx_interface_def ::= INTERFACE IDENTIFIER EXTENDS IDENTIFIER xx_interface_body",
 /*  12 */ "xx_class_def ::= CLASS IDENTIFIER xx_class_body",
 /*  13 */ "xx_class_def ::= CLASS IDENTIFIER EXTENDS IDENTIFIER xx_class_body",
 /*  14 */ "xx_class_def ::= CLASS IDENTIFIER IMPLEMENTS xx_implements_list xx_class_body",
 /*  15 */ "xx_class_def ::= CLASS IDENTIFIER EXTENDS IDENTIFIER IMPLEMENTS xx_implements_list xx_class_body",
 /*  16 */ "xx_class_def ::= ABSTRACT CLASS IDENTIFIER xx_class_body",
 /*  17 */ "xx_class_def ::= ABSTRACT CLASS IDENTIFIER EXTENDS IDENTIFIER xx_class_body",
 /*  18 */ "xx_class_def ::= ABSTRACT CLASS IDENTIFIER IMPLEMENTS xx_implements_list xx_class_body",
 /*  19 */ "xx_class_def ::= FINAL CLASS IDENTIFIER xx_class_body",
 /*  20 */ "xx_class_def ::= FINAL CLASS IDENTIFIER EXTENDS IDENTIFIER xx_class_body",
 /*  21 */ "xx_class_body ::= BRACKET_OPEN BRACKET_CLOSE",
 /*  22 */ "xx_class_body ::= BRACKET_OPEN xx_class_definition BRACKET_CLOSE",
 /*  23 */ "xx_implements_list ::= xx_implements_list COMMA xx_implements",
 /*  24 */ "xx_implements_list ::= xx_implements",
 /*  25 */ "xx_implements ::= IDENTIFIER",
 /*  26 */ "xx_interface_body ::= BRACKET_OPEN BRACKET_CLOSE",
 /*  27 */ "xx_interface_body ::= BRACKET_OPEN xx_interface_methods_definition BRACKET_CLOSE",
 /*  28 */ "xx_class_definition ::= xx_class_properties_definition",
 /*  29 */ "xx_class_definition ::= xx_class_consts_definition",
 /*  30 */ "xx_class_definition ::= xx_class_methods_definition",
 /*  31 */ "xx_class_definition ::= xx_class_properties_definition xx_class_methods_definition",
 /*  32 */ "xx_class_definition ::= xx_class_properties_definition xx_class_consts_definition",
 /*  33 */ "xx_class_definition ::= xx_class_consts_definition xx_class_properties_definition",
 /*  34 */ "xx_class_definition ::= xx_class_consts_definition xx_class_methods_definition",
 /*  35 */ "xx_class_definition ::= xx_class_properties_definition xx_class_consts_definition xx_class_methods_definition",
 /*  36 */ "xx_class_definition ::= xx_class_consts_definition xx_class_properties_definition xx_class_methods_definition",
 /*  37 */ "xx_class_properties_definition ::= xx_class_properties_definition xx_class_property_definition",
 /*  38 */ "xx_class_properties_definition ::= xx_class_property_definition",
 /*  39 */ "xx_class_property_definition ::= COMMENT xx_visibility_list IDENTIFIER DOTCOMMA",
 /*  40 */ "xx_class_property_definition ::= xx_visibility_list IDENTIFIER DOTCOMMA",
 /*  41 */ "xx_class_property_definition ::= COMMENT xx_visibility_list IDENTIFIER ASSIGN xx_literal_expr DOTCOMMA",
 /*  42 */ "xx_class_property_definition ::= xx_visibility_list IDENTIFIER ASSIGN xx_literal_expr DOTCOMMA",
 /*  43 */ "xx_class_property_definition ::= COMMENT xx_visibility_list IDENTIFIER xx_class_property_shortcuts DOTCOMMA",
 /*  44 */ "xx_class_property_definition ::= xx_visibility_list IDENTIFIER xx_class_property_shortcuts DOTCOMMA",
 /*  45 */ "xx_class_property_definition ::= COMMENT xx_visibility_list IDENTIFIER ASSIGN xx_literal_expr xx_class_property_shortcuts DOTCOMMA",
 /*  46 */ "xx_class_property_definition ::= xx_visibility_list IDENTIFIER ASSIGN xx_literal_expr xx_class_property_shortcuts DOTCOMMA",
 /*  47 */ "xx_class_property_shortcuts ::= BRACKET_OPEN BRACKET_CLOSE",
 /*  48 */ "xx_class_property_shortcuts ::= BRACKET_OPEN xx_class_property_shortcuts_list BRACKET_CLOSE",
 /*  49 */ "xx_class_property_shortcuts_list ::= xx_class_property_shortcuts_list COMMA xx_class_property_shortcut",
 /*  50 */ "xx_class_property_shortcuts_list ::= xx_class_property_shortcut",
 /*  51 */ "xx_class_property_shortcut ::= IDENTIFIER",
 /*  52 */ "xx_class_property_shortcut ::= COMMENT IDENTIFIER",
 /*  53 */ "xx_class_consts_definition ::= xx_class_consts_definition xx_class_const_definition",
 /*  54 */ "xx_class_consts_definition ::= xx_class_const_definition",
 /*  55 */ "xx_class_methods_definition ::= xx_class_methods_definition xx_class_method_definition",
 /*  56 */ "xx_class_methods_definition ::= xx_class_method_definition",
 /*  57 */ "xx_interface_methods_definition ::= xx_interface_methods_definition xx_interface_method_definition",
 /*  58 */ "xx_interface_methods_definition ::= xx_interface_method_definition",
 /*  59 */ "xx_class_const_definition ::= COMMENT CONST CONSTANT ASSIGN xx_literal_expr DOTCOMMA",
 /*  60 */ "xx_class_const_definition ::= CONST CONSTANT ASSIGN xx_literal_expr DOTCOMMA",
 /*  61 */ "xx_class_method_definition ::= xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE BRACKET_OPEN BRACKET_CLOSE",
 /*  62 */ "xx_class_method_definition ::= xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE BRACKET_OPEN BRACKET_CLOSE",
 /*  63 */ "xx_class_method_definition ::= xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  64 */ "xx_class_method_definition ::= xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  65 */ "xx_class_method_definition ::= COMMENT xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE BRACKET_OPEN BRACKET_CLOSE",
 /*  66 */ "xx_class_method_definition ::= COMMENT xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE BRACKET_OPEN BRACKET_CLOSE",
 /*  67 */ "xx_class_method_definition ::= COMMENT xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  68 */ "xx_class_method_definition ::= COMMENT xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  69 */ "xx_class_method_definition ::= xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE ARROW xx_method_return_type BRACKET_OPEN BRACKET_CLOSE",
 /*  70 */ "xx_class_method_definition ::= xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE ARROW xx_method_return_type BRACKET_OPEN BRACKET_CLOSE",
 /*  71 */ "xx_class_method_definition ::= xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE ARROW xx_method_return_type BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  72 */ "xx_class_method_definition ::= xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE ARROW xx_method_return_type BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  73 */ "xx_class_method_definition ::= COMMENT xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE ARROW xx_method_return_type BRACKET_OPEN BRACKET_CLOSE",
 /*  74 */ "xx_class_method_definition ::= COMMENT xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE ARROW xx_method_return_type BRACKET_OPEN BRACKET_CLOSE",
 /*  75 */ "xx_class_method_definition ::= COMMENT xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE ARROW xx_method_return_type BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  76 */ "xx_class_method_definition ::= COMMENT xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE ARROW xx_method_return_type BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  77 */ "xx_interface_method_definition ::= xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE ARROW xx_method_return_type DOTCOMMA",
 /*  78 */ "xx_interface_method_definition ::= xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE ARROW xx_method_return_type DOTCOMMA",
 /*  79 */ "xx_interface_method_definition ::= COMMENT xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE ARROW xx_method_return_type DOTCOMMA",
 /*  80 */ "xx_interface_method_definition ::= COMMENT xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE ARROW xx_method_return_type DOTCOMMA",
 /*  81 */ "xx_interface_method_definition ::= xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE DOTCOMMA",
 /*  82 */ "xx_interface_method_definition ::= xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE DOTCOMMA",
 /*  83 */ "xx_interface_method_definition ::= COMMENT xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE DOTCOMMA",
 /*  84 */ "xx_interface_method_definition ::= COMMENT xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE DOTCOMMA",
 /*  85 */ "xx_visibility_list ::= xx_visibility_list xx_visibility",
 /*  86 */ "xx_visibility_list ::= xx_visibility",
 /*  87 */ "xx_visibility ::= PUBLIC",
 /*  88 */ "xx_visibility ::= PROTECTED",
 /*  89 */ "xx_visibility ::= PRIVATE",
 /*  90 */ "xx_visibility ::= STATIC",
 /*  91 */ "xx_visibility ::= SCOPED",
 /*  92 */ "xx_visibility ::= INLINE",
 /*  93 */ "xx_visibility ::= ABSTRACT",
 /*  94 */ "xx_visibility ::= FINAL",
 /*  95 */ "xx_method_return_type ::= VOID",
 /*  96 */ "xx_method_return_type ::= xx_method_return_type_list",
 /*  97 */ "xx_method_return_type_list ::= xx_method_return_type_list BITWISE_OR xx_method_return_type_item",
 /*  98 */ "xx_method_return_type_list ::= xx_method_return_type_item",
 /*  99 */ "xx_method_return_type_item ::= xx_parameter_type",
 /* 100 */ "xx_method_return_type_item ::= xx_parameter_type NOT",
 /* 101 */ "xx_method_return_type_item ::= xx_parameter_cast",
 /* 102 */ "xx_method_return_type_item ::= xx_parameter_cast_collection",
 /* 103 */ "xx_parameter_list ::= xx_parameter_list COMMA xx_parameter",
 /* 104 */ "xx_parameter_list ::= xx_parameter",
 /* 105 */ "xx_parameter ::= IDENTIFIER",
 /* 106 */ "xx_parameter ::= xx_parameter_type IDENTIFIER",
 /* 107 */ "xx_parameter ::= xx_parameter_type NOT IDENTIFIER",
 /* 108 */ "xx_parameter ::= xx_parameter_cast IDENTIFIER",
 /* 109 */ "xx_parameter ::= IDENTIFIER ASSIGN xx_literal_expr",
 /* 110 */ "xx_parameter ::= xx_parameter_type IDENTIFIER ASSIGN xx_literal_expr",
 /* 111 */ "xx_parameter ::= xx_parameter_type NOT IDENTIFIER ASSIGN xx_literal_expr",
 /* 112 */ "xx_parameter ::= xx_parameter_cast IDENTIFIER ASSIGN xx_literal_expr",
 /* 113 */ "xx_parameter_cast ::= LESS IDENTIFIER GREATER",
 /* 114 */ "xx_parameter_cast_collection ::= LESS IDENTIFIER SBRACKET_OPEN SBRACKET_CLOSE GREATER",
 /* 115 */ "xx_parameter_type ::= TYPE_INTEGER",
 /* 116 */ "xx_parameter_type ::= TYPE_UINTEGER",
 /* 117 */ "xx_parameter_type ::= TYPE_LONG",
 /* 118 */ "xx_parameter_type ::= TYPE_ULONG",
 /* 119 */ "xx_parameter_type ::= TYPE_CHAR",
 /* 120 */ "xx_parameter_type ::= TYPE_UCHAR",
 /* 121 */ "xx_parameter_type ::= TYPE_DOUBLE",
 /* 122 */ "xx_parameter_type ::= TYPE_BOOL",
 /* 123 */ "xx_parameter_type ::= TYPE_STRING",
 /* 124 */ "xx_parameter_type ::= TYPE_ARRAY",
 /* 125 */ "xx_parameter_type ::= TYPE_VAR",
 /* 126 */ "xx_statement_list ::= xx_statement_list xx_statement",
 /* 127 */ "xx_statement_list ::= xx_statement",
 /* 128 */ "xx_statement ::= xx_cblock",
 /* 129 */ "xx_statement ::= xx_let_statement",
 /* 130 */ "xx_statement ::= xx_if_statement",
 /* 131 */ "xx_statement ::= xx_loop_statement",
 /* 132 */ "xx_statement ::= xx_echo_statement",
 /* 133 */ "xx_statement ::= xx_return_statement",
 /* 134 */ "xx_statement ::= xx_fetch_statement",
 /* 135 */ "xx_statement ::= xx_fcall_statement",
 /* 136 */ "xx_statement ::= xx_mcall_statement",
 /* 137 */ "xx_statement ::= xx_scall_statement",
 /* 138 */ "xx_statement ::= xx_unset_statement",
 /* 139 */ "xx_statement ::= xx_throw_statement",
 /* 140 */ "xx_statement ::= xx_declare_statement",
 /* 141 */ "xx_statement ::= xx_break_statement",
 /* 142 */ "xx_statement ::= xx_continue_statement",
 /* 143 */ "xx_statement ::= xx_while_statement",
 /* 144 */ "xx_statement ::= xx_do_while_statement",
 /* 145 */ "xx_statement ::= xx_switch_statement",
 /* 146 */ "xx_statement ::= xx_for_statement",
 /* 147 */ "xx_statement ::= xx_comment",
 /* 148 */ "xx_statement ::= xx_empty_statement",
 /* 149 */ "xx_empty_statement ::= DOTCOMMA",
 /* 150 */ "xx_break_statement ::= BREAK DOTCOMMA",
 /* 151 */ "xx_continue_statement ::= CONTINUE DOTCOMMA",
 /* 152 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN BRACKET_CLOSE",
 /* 153 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN BRACKET_CLOSE ELSE BRACKET_OPEN BRACKET_CLOSE",
 /* 154 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 155 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE ELSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 156 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE ELSE BRACKET_OPEN BRACKET_CLOSE",
 /* 157 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN BRACKET_CLOSE ELSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 158 */ "xx_switch_statement ::= SWITCH xx_eval_expr BRACKET_OPEN BRACKET_CLOSE",
 /* 159 */ "xx_switch_statement ::= SWITCH xx_eval_expr BRACKET_OPEN xx_case_clauses BRACKET_CLOSE",
 /* 160 */ "xx_case_clauses ::= xx_case_clauses xx_case_clause",
 /* 161 */ "xx_case_clauses ::= xx_case_clause",
 /* 162 */ "xx_case_clause ::= CASE xx_literal_expr COLON",
 /* 163 */ "xx_case_clause ::= CASE xx_literal_expr COLON xx_statement_list",
 /* 164 */ "xx_case_clause ::= DEFAULT COLON xx_statement_list",
 /* 165 */ "xx_loop_statement ::= LOOP BRACKET_OPEN BRACKET_CLOSE",
 /* 166 */ "xx_loop_statement ::= LOOP BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 167 */ "xx_while_statement ::= WHILE xx_eval_expr BRACKET_OPEN BRACKET_CLOSE",
 /* 168 */ "xx_while_statement ::= WHILE xx_eval_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 169 */ "xx_do_while_statement ::= DO BRACKET_OPEN BRACKET_CLOSE WHILE xx_eval_expr DOTCOMMA",
 /* 170 */ "xx_do_while_statement ::= DO BRACKET_OPEN xx_statement_list BRACKET_CLOSE WHILE xx_eval_expr DOTCOMMA",
 /* 171 */ "xx_for_statement ::= FOR IDENTIFIER IN xx_common_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 172 */ "xx_for_statement ::= FOR IDENTIFIER IN REVERSE xx_common_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 173 */ "xx_for_statement ::= FOR IDENTIFIER COMMA IDENTIFIER IN xx_common_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 174 */ "xx_for_statement ::= FOR IDENTIFIER COMMA IDENTIFIER IN REVERSE xx_common_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 175 */ "xx_let_statement ::= LET xx_let_assignments DOTCOMMA",
 /* 176 */ "xx_let_assignments ::= xx_let_assignments COMMA xx_let_assignment",
 /* 177 */ "xx_let_assignments ::= xx_let_assignment",
 /* 178 */ "xx_assignment_operator ::= ASSIGN",
 /* 179 */ "xx_assignment_operator ::= ADDASSIGN",
 /* 180 */ "xx_assignment_operator ::= SUBASSIGN",
 /* 181 */ "xx_assignment_operator ::= MULASSIGN",
 /* 182 */ "xx_assignment_operator ::= DIVASSIGN",
 /* 183 */ "xx_assignment_operator ::= CONCATASSIGN",
 /* 184 */ "xx_let_assignment ::= IDENTIFIER xx_assignment_operator xx_assign_expr",
 /* 185 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER xx_assignment_operator xx_assign_expr",
 /* 186 */ "xx_let_assignment ::= IDENTIFIER ARROW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 187 */ "xx_let_assignment ::= IDENTIFIER ARROW BRACKET_OPEN STRING BRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 188 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER SBRACKET_OPEN SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 189 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER SBRACKET_OPEN xx_index_expr SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 190 */ "xx_let_assignment ::= IDENTIFIER DOUBLECOLON IDENTIFIER xx_assignment_operator xx_assign_expr",
 /* 191 */ "xx_let_assignment ::= IDENTIFIER DOUBLECOLON IDENTIFIER SBRACKET_OPEN SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 192 */ "xx_let_assignment ::= IDENTIFIER DOUBLECOLON IDENTIFIER SBRACKET_OPEN xx_index_expr SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 193 */ "xx_let_assignment ::= IDENTIFIER SBRACKET_OPEN SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 194 */ "xx_let_assignment ::= IDENTIFIER xx_array_offset_list xx_assignment_operator xx_assign_expr",
 /* 195 */ "xx_array_offset_list ::= xx_array_offset_list xx_array_offset",
 /* 196 */ "xx_array_offset_list ::= xx_array_offset",
 /* 197 */ "xx_array_offset ::= SBRACKET_OPEN xx_index_expr SBRACKET_CLOSE",
 /* 198 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER INCR",
 /* 199 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER DECR",
 /* 200 */ "xx_let_assignment ::= IDENTIFIER INCR",
 /* 201 */ "xx_let_assignment ::= IDENTIFIER DECR",
 /* 202 */ "xx_index_expr ::= xx_common_expr",
 /* 203 */ "xx_echo_statement ::= ECHO xx_echo_expressions DOTCOMMA",
 /* 204 */ "xx_echo_expressions ::= xx_echo_expressions COMMA xx_echo_expression",
 /* 205 */ "xx_echo_expressions ::= xx_echo_expression",
 /* 206 */ "xx_echo_expression ::= xx_common_expr",
 /* 207 */ "xx_mcall_statement ::= xx_mcall_expr DOTCOMMA",
 /* 208 */ "xx_fcall_statement ::= xx_fcall_expr DOTCOMMA",
 /* 209 */ "xx_scall_statement ::= xx_scall_expr DOTCOMMA",
 /* 210 */ "xx_fetch_statement ::= xx_fetch_expr DOTCOMMA",
 /* 211 */ "xx_return_statement ::= RETURN xx_common_expr DOTCOMMA",
 /* 212 */ "xx_return_statement ::= RETURN DOTCOMMA",
 /* 213 */ "xx_unset_statement ::= UNSET IDENTIFIER ARROW IDENTIFIER DOTCOMMA",
 /* 214 */ "xx_unset_statement ::= UNSET IDENTIFIER SBRACKET_OPEN xx_common_expr SBRACKET_CLOSE DOTCOMMA",
 /* 215 */ "xx_throw_statement ::= THROW xx_common_expr DOTCOMMA",
 /* 216 */ "xx_declare_statement ::= TYPE_INTEGER xx_declare_variable_list DOTCOMMA",
 /* 217 */ "xx_declare_statement ::= TYPE_UINTEGER xx_declare_variable_list DOTCOMMA",
 /* 218 */ "xx_declare_statement ::= TYPE_CHAR xx_declare_variable_list DOTCOMMA",
 /* 219 */ "xx_declare_statement ::= TYPE_UCHAR xx_declare_variable_list DOTCOMMA",
 /* 220 */ "xx_declare_statement ::= TYPE_LONG xx_declare_variable_list DOTCOMMA",
 /* 221 */ "xx_declare_statement ::= TYPE_ULONG xx_declare_variable_list DOTCOMMA",
 /* 222 */ "xx_declare_statement ::= TYPE_DOUBLE xx_declare_variable_list DOTCOMMA",
 /* 223 */ "xx_declare_statement ::= TYPE_STRING xx_declare_variable_list DOTCOMMA",
 /* 224 */ "xx_declare_statement ::= TYPE_BOOL xx_declare_variable_list DOTCOMMA",
 /* 225 */ "xx_declare_statement ::= TYPE_VAR xx_declare_variable_list DOTCOMMA",
 /* 226 */ "xx_declare_variable_list ::= xx_declare_variable_list COMMA xx_declare_variable",
 /* 227 */ "xx_declare_variable_list ::= xx_declare_variable",
 /* 228 */ "xx_declare_variable ::= IDENTIFIER",
 /* 229 */ "xx_declare_variable ::= IDENTIFIER ASSIGN xx_literal_expr",
 /* 230 */ "xx_assign_expr ::= xx_common_expr",
 /* 231 */ "xx_common_expr ::= NOT xx_common_expr",
 /* 232 */ "xx_common_expr ::= ISSET xx_isset_expr",
 /* 233 */ "xx_common_expr ::= REQUIRE xx_common_expr",
 /* 234 */ "xx_common_expr ::= CLONE xx_common_expr",
 /* 235 */ "xx_common_expr ::= EMPTY xx_common_expr",
 /* 236 */ "xx_common_expr ::= LIKELY xx_common_expr",
 /* 237 */ "xx_common_expr ::= UNLIKELY xx_common_expr",
 /* 238 */ "xx_common_expr ::= xx_common_expr EQUALS xx_common_expr",
 /* 239 */ "xx_common_expr ::= xx_common_expr NOTEQUALS xx_common_expr",
 /* 240 */ "xx_common_expr ::= xx_common_expr IDENTICAL xx_common_expr",
 /* 241 */ "xx_common_expr ::= xx_common_expr NOTIDENTICAL xx_common_expr",
 /* 242 */ "xx_common_expr ::= xx_common_expr LESS xx_common_expr",
 /* 243 */ "xx_common_expr ::= xx_common_expr GREATER xx_common_expr",
 /* 244 */ "xx_common_expr ::= xx_common_expr LESSEQUAL xx_common_expr",
 /* 245 */ "xx_common_expr ::= xx_common_expr GREATEREQUAL xx_common_expr",
 /* 246 */ "xx_common_expr ::= PARENTHESES_OPEN xx_common_expr PARENTHESES_CLOSE",
 /* 247 */ "xx_common_expr ::= PARENTHESES_OPEN xx_parameter_type PARENTHESES_CLOSE xx_common_expr",
 /* 248 */ "xx_common_expr ::= LESS IDENTIFIER GREATER xx_common_expr",
 /* 249 */ "xx_common_expr ::= xx_common_expr ARROW IDENTIFIER",
 /* 250 */ "xx_common_expr ::= xx_common_expr ARROW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE",
 /* 251 */ "xx_common_expr ::= xx_common_expr ARROW BRACKET_OPEN STRING BRACKET_CLOSE",
 /* 252 */ "xx_common_expr ::= IDENTIFIER DOUBLECOLON IDENTIFIER",
 /* 253 */ "xx_common_expr ::= IDENTIFIER DOUBLECOLON CONSTANT",
 /* 254 */ "xx_common_expr ::= xx_common_expr SBRACKET_OPEN xx_common_expr SBRACKET_CLOSE",
 /* 255 */ "xx_common_expr ::= xx_common_expr ADD xx_common_expr",
 /* 256 */ "xx_common_expr ::= xx_common_expr SUB xx_common_expr",
 /* 257 */ "xx_common_expr ::= xx_common_expr MUL xx_common_expr",
 /* 258 */ "xx_common_expr ::= xx_common_expr DIV xx_common_expr",
 /* 259 */ "xx_common_expr ::= xx_common_expr MOD xx_common_expr",
 /* 260 */ "xx_common_expr ::= xx_common_expr CONCAT xx_common_expr",
 /* 261 */ "xx_common_expr ::= xx_common_expr AND xx_common_expr",
 /* 262 */ "xx_common_expr ::= xx_common_expr OR xx_common_expr",
 /* 263 */ "xx_common_expr ::= xx_common_expr INSTANCEOF xx_common_expr",
 /* 264 */ "xx_fetch_expr ::= FETCH IDENTIFIER COMMA xx_isset_expr",
 /* 265 */ "xx_common_expr ::= xx_fetch_expr",
 /* 266 */ "xx_common_expr ::= TYPEOF xx_common_expr",
 /* 267 */ "xx_common_expr ::= IDENTIFIER",
 /* 268 */ "xx_common_expr ::= INTEGER",
 /* 269 */ "xx_common_expr ::= STRING",
 /* 270 */ "xx_common_expr ::= CHAR",
 /* 271 */ "xx_common_expr ::= DOUBLE",
 /* 272 */ "xx_common_expr ::= NULL",
 /* 273 */ "xx_common_expr ::= TRUE",
 /* 274 */ "xx_common_expr ::= FALSE",
 /* 275 */ "xx_common_expr ::= CONSTANT",
 /* 276 */ "xx_common_expr ::= SBRACKET_OPEN SBRACKET_CLOSE",
 /* 277 */ "xx_common_expr ::= SBRACKET_OPEN xx_array_list SBRACKET_CLOSE",
 /* 278 */ "xx_common_expr ::= NEW IDENTIFIER",
 /* 279 */ "xx_common_expr ::= NEW IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 280 */ "xx_common_expr ::= NEW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 281 */ "xx_common_expr ::= NEW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE",
 /* 282 */ "xx_common_expr ::= NEW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 283 */ "xx_common_expr ::= NEW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 284 */ "xx_fcall_expr ::= IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 285 */ "xx_fcall_expr ::= IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 286 */ "xx_fcall_expr ::= BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 287 */ "xx_fcall_expr ::= BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 288 */ "xx_scall_expr ::= IDENTIFIER DOUBLECOLON IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 289 */ "xx_scall_expr ::= IDENTIFIER DOUBLECOLON IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 290 */ "xx_scall_expr ::= BRACKET_OPEN IDENTIFIER BRACKET_CLOSE DOUBLECOLON IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 291 */ "xx_scall_expr ::= BRACKET_OPEN IDENTIFIER BRACKET_CLOSE DOUBLECOLON IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 292 */ "xx_scall_expr ::= BRACKET_OPEN IDENTIFIER BRACKET_CLOSE DOUBLECOLON BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 293 */ "xx_scall_expr ::= BRACKET_OPEN IDENTIFIER BRACKET_CLOSE DOUBLECOLON BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 294 */ "xx_mcall_expr ::= xx_common_expr ARROW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 295 */ "xx_mcall_expr ::= xx_common_expr ARROW IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 296 */ "xx_mcall_expr ::= xx_common_expr ARROW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 297 */ "xx_mcall_expr ::= xx_common_expr ARROW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 298 */ "xx_mcall_expr ::= xx_common_expr ARROW BRACKET_OPEN STRING BRACKET_CLOSE PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 299 */ "xx_mcall_expr ::= xx_common_expr ARROW BRACKET_OPEN STRING BRACKET_CLOSE PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 300 */ "xx_common_expr ::= xx_mcall_expr",
 /* 301 */ "xx_common_expr ::= xx_scall_expr",
 /* 302 */ "xx_common_expr ::= xx_fcall_expr",
 /* 303 */ "xx_call_parameters ::= xx_call_parameters COMMA xx_call_parameter",
 /* 304 */ "xx_call_parameters ::= xx_call_parameter",
 /* 305 */ "xx_call_parameter ::= xx_common_expr",
 /* 306 */ "xx_array_list ::= xx_array_list COMMA xx_array_item",
 /* 307 */ "xx_array_list ::= xx_array_item",
 /* 308 */ "xx_array_item ::= xx_array_key COLON xx_array_value",
 /* 309 */ "xx_array_item ::= xx_array_value",
 /* 310 */ "xx_array_key ::= IDENTIFIER",
 /* 311 */ "xx_array_key ::= STRING",
 /* 312 */ "xx_array_key ::= INTEGER",
 /* 313 */ "xx_array_value ::= xx_common_expr",
 /* 314 */ "xx_literal_expr ::= INTEGER",
 /* 315 */ "xx_literal_expr ::= CHAR",
 /* 316 */ "xx_literal_expr ::= STRING",
 /* 317 */ "xx_literal_expr ::= DOUBLE",
 /* 318 */ "xx_literal_expr ::= NULL",
 /* 319 */ "xx_literal_expr ::= FALSE",
 /* 320 */ "xx_literal_expr ::= TRUE",
 /* 321 */ "xx_literal_expr ::= IDENTIFIER DOUBLECOLON CONSTANT",
 /* 322 */ "xx_literal_expr ::= CONSTANT",
 /* 323 */ "xx_isset_expr ::= IDENTIFIER SBRACKET_OPEN xx_common_expr SBRACKET_CLOSE",
 /* 324 */ "xx_isset_expr ::= IDENTIFIER ARROW IDENTIFIER",
 /* 325 */ "xx_isset_expr ::= IDENTIFIER ARROW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE",
 /* 326 */ "xx_isset_expr ::= IDENTIFIER ARROW BRACKET_OPEN STRING BRACKET_CLOSE",
 /* 327 */ "xx_eval_expr ::= xx_common_expr",
 /* 328 */ "xx_comment ::= COMMENT",
 /* 329 */ "xx_cblock ::= CBLOCK",
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
    case 104:
// 1015 "parser.lemon"
{
	/*if ((yypminor->yy0)) {
		if ((yypminor->yy0)->free_flag) {
			efree((yypminor->yy0)->token);
		}
		efree((yypminor->yy0));
	}*/
}
// 3179 "parser.c"
      break;
    case 107:
// 1028 "parser.lemon"
{ json_object_put((yypminor->yy259)); }
// 3184 "parser.c"
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
  { 106, 1 },
  { 107, 1 },
  { 108, 2 },
  { 108, 1 },
  { 109, 1 },
  { 109, 1 },
  { 109, 1 },
  { 109, 1 },
  { 109, 1 },
  { 110, 3 },
  { 112, 3 },
  { 112, 5 },
  { 111, 3 },
  { 111, 5 },
  { 111, 5 },
  { 111, 7 },
  { 111, 4 },
  { 111, 6 },
  { 111, 6 },
  { 111, 4 },
  { 111, 6 },
  { 116, 2 },
  { 116, 3 },
  { 117, 3 },
  { 117, 1 },
  { 119, 1 },
  { 115, 2 },
  { 115, 3 },
  { 118, 1 },
  { 118, 1 },
  { 118, 1 },
  { 118, 2 },
  { 118, 2 },
  { 118, 2 },
  { 118, 2 },
  { 118, 3 },
  { 118, 3 },
  { 121, 2 },
  { 121, 1 },
  { 124, 4 },
  { 124, 3 },
  { 124, 6 },
  { 124, 5 },
  { 124, 5 },
  { 124, 4 },
  { 124, 7 },
  { 124, 6 },
  { 127, 2 },
  { 127, 3 },
  { 128, 3 },
  { 128, 1 },
  { 129, 1 },
  { 129, 2 },
  { 122, 2 },
  { 122, 1 },
  { 123, 2 },
  { 123, 1 },
  { 120, 2 },
  { 120, 1 },
  { 130, 6 },
  { 130, 5 },
  { 131, 7 },
  { 131, 8 },
  { 131, 8 },
  { 131, 9 },
  { 131, 8 },
  { 131, 9 },
  { 131, 9 },
  { 131, 10 },
  { 131, 9 },
  { 131, 10 },
  { 131, 10 },
  { 131, 11 },
  { 131, 10 },
  { 131, 11 },
  { 131, 11 },
  { 131, 12 },
  { 132, 8 },
  { 132, 9 },
  { 132, 9 },
  { 132, 10 },
  { 132, 6 },
  { 132, 7 },
  { 132, 7 },
  { 132, 8 },
  { 125, 2 },
  { 125, 1 },
  { 136, 1 },
  { 136, 1 },
  { 136, 1 },
  { 136, 1 },
  { 136, 1 },
  { 136, 1 },
  { 136, 1 },
  { 136, 1 },
  { 135, 1 },
  { 135, 1 },
  { 137, 3 },
  { 137, 1 },
  { 138, 1 },
  { 138, 2 },
  { 138, 1 },
  { 138, 1 },
  { 133, 3 },
  { 133, 1 },
  { 142, 1 },
  { 142, 2 },
  { 142, 3 },
  { 142, 2 },
  { 142, 3 },
  { 142, 4 },
  { 142, 5 },
  { 142, 4 },
  { 140, 3 },
  { 141, 5 },
  { 139, 1 },
  { 139, 1 },
  { 139, 1 },
  { 139, 1 },
  { 139, 1 },
  { 139, 1 },
  { 139, 1 },
  { 139, 1 },
  { 139, 1 },
  { 139, 1 },
  { 139, 1 },
  { 134, 2 },
  { 134, 1 },
  { 143, 1 },
  { 143, 1 },
  { 143, 1 },
  { 143, 1 },
  { 143, 1 },
  { 143, 1 },
  { 143, 1 },
  { 143, 1 },
  { 143, 1 },
  { 143, 1 },
  { 143, 1 },
  { 143, 1 },
  { 143, 1 },
  { 143, 1 },
  { 143, 1 },
  { 143, 1 },
  { 143, 1 },
  { 143, 1 },
  { 143, 1 },
  { 143, 1 },
  { 143, 1 },
  { 162, 1 },
  { 156, 2 },
  { 157, 2 },
  { 145, 4 },
  { 145, 7 },
  { 145, 5 },
  { 145, 9 },
  { 145, 8 },
  { 145, 8 },
  { 160, 4 },
  { 160, 5 },
  { 164, 2 },
  { 164, 1 },
  { 165, 3 },
  { 165, 4 },
  { 165, 3 },
  { 146, 3 },
  { 146, 4 },
  { 158, 4 },
  { 158, 5 },
  { 159, 6 },
  { 159, 7 },
  { 161, 7 },
  { 161, 8 },
  { 161, 9 },
  { 161, 10 },
  { 144, 3 },
  { 167, 3 },
  { 167, 1 },
  { 169, 1 },
  { 169, 1 },
  { 169, 1 },
  { 169, 1 },
  { 169, 1 },
  { 169, 1 },
  { 168, 3 },
  { 168, 5 },
  { 168, 7 },
  { 168, 7 },
  { 168, 7 },
  { 168, 8 },
  { 168, 5 },
  { 168, 7 },
  { 168, 8 },
  { 168, 5 },
  { 168, 4 },
  { 172, 2 },
  { 172, 1 },
  { 173, 3 },
  { 168, 4 },
  { 168, 4 },
  { 168, 2 },
  { 168, 2 },
  { 171, 1 },
  { 147, 3 },
  { 174, 3 },
  { 174, 1 },
  { 175, 1 },
  { 151, 2 },
  { 150, 2 },
  { 152, 2 },
  { 149, 2 },
  { 148, 3 },
  { 148, 2 },
  { 153, 5 },
  { 153, 6 },
  { 154, 3 },
  { 155, 3 },
  { 155, 3 },
  { 155, 3 },
  { 155, 3 },
  { 155, 3 },
  { 155, 3 },
  { 155, 3 },
  { 155, 3 },
  { 155, 3 },
  { 155, 3 },
  { 180, 3 },
  { 180, 1 },
  { 181, 1 },
  { 181, 3 },
  { 170, 1 },
  { 166, 2 },
  { 166, 2 },
  { 166, 2 },
  { 166, 2 },
  { 166, 2 },
  { 166, 2 },
  { 166, 2 },
  { 166, 3 },
  { 166, 3 },
  { 166, 3 },
  { 166, 3 },
  { 166, 3 },
  { 166, 3 },
  { 166, 3 },
  { 166, 3 },
  { 166, 3 },
  { 166, 4 },
  { 166, 4 },
  { 166, 3 },
  { 166, 5 },
  { 166, 5 },
  { 166, 3 },
  { 166, 3 },
  { 166, 4 },
  { 166, 3 },
  { 166, 3 },
  { 166, 3 },
  { 166, 3 },
  { 166, 3 },
  { 166, 3 },
  { 166, 3 },
  { 166, 3 },
  { 166, 3 },
  { 179, 4 },
  { 166, 1 },
  { 166, 2 },
  { 166, 1 },
  { 166, 1 },
  { 166, 1 },
  { 166, 1 },
  { 166, 1 },
  { 166, 1 },
  { 166, 1 },
  { 166, 1 },
  { 166, 1 },
  { 166, 2 },
  { 166, 3 },
  { 166, 2 },
  { 166, 4 },
  { 166, 5 },
  { 166, 4 },
  { 166, 6 },
  { 166, 7 },
  { 177, 4 },
  { 177, 3 },
  { 177, 6 },
  { 177, 5 },
  { 178, 6 },
  { 178, 5 },
  { 178, 8 },
  { 178, 7 },
  { 178, 10 },
  { 178, 9 },
  { 176, 6 },
  { 176, 5 },
  { 176, 8 },
  { 176, 7 },
  { 176, 8 },
  { 176, 7 },
  { 166, 1 },
  { 166, 1 },
  { 166, 1 },
  { 184, 3 },
  { 184, 1 },
  { 185, 1 },
  { 183, 3 },
  { 183, 1 },
  { 186, 3 },
  { 186, 1 },
  { 187, 1 },
  { 187, 1 },
  { 187, 1 },
  { 188, 1 },
  { 126, 1 },
  { 126, 1 },
  { 126, 1 },
  { 126, 1 },
  { 126, 1 },
  { 126, 1 },
  { 126, 1 },
  { 126, 3 },
  { 126, 1 },
  { 182, 4 },
  { 182, 3 },
  { 182, 5 },
  { 182, 5 },
  { 163, 1 },
  { 113, 1 },
  { 114, 1 },
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
// 1024 "parser.lemon"
{
	status->ret = yymsp[0].minor.yy259;
}
// 3731 "parser.c"
        break;
      case 1:
      case 4:
      case 5:
      case 6:
      case 7:
      case 8:
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
      case 147:
      case 148:
      case 202:
      case 230:
      case 265:
      case 300:
      case 301:
      case 302:
      case 305:
      case 313:
      case 327:
// 1030 "parser.lemon"
{
	yygotominor.yy259 = yymsp[0].minor.yy259;
}
// 3773 "parser.c"
        break;
      case 2:
      case 37:
      case 53:
      case 55:
      case 57:
      case 85:
      case 126:
      case 160:
      case 195:
// 1034 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_list(yymsp[-1].minor.yy259, yymsp[0].minor.yy259);
}
// 3788 "parser.c"
        break;
      case 3:
      case 24:
      case 38:
      case 50:
      case 54:
      case 56:
      case 58:
      case 86:
      case 98:
      case 104:
      case 127:
      case 161:
      case 177:
      case 196:
      case 205:
      case 227:
      case 304:
      case 307:
// 1038 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_list(NULL, yymsp[0].minor.yy259);
}
// 3812 "parser.c"
        break;
      case 9:
// 1063 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_namespace(yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(39,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3821 "parser.c"
        break;
      case 10:
// 1067 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_interface(yymsp[-1].minor.yy0, yymsp[0].minor.yy259, NULL, status->scanner_state);
  yy_destructor(42,&yymsp[-2].minor);
}
// 3829 "parser.c"
        break;
      case 11:
// 1071 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_interface(yymsp[-3].minor.yy0, yymsp[0].minor.yy259, yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(42,&yymsp[-4].minor);
  yy_destructor(43,&yymsp[-2].minor);
}
// 3838 "parser.c"
        break;
      case 12:
// 1075 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class(yymsp[-1].minor.yy0, yymsp[0].minor.yy259, 0, 0, NULL, NULL, status->scanner_state);
  yy_destructor(44,&yymsp[-2].minor);
}
// 3846 "parser.c"
        break;
      case 13:
// 1079 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy259, 0, 0, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(44,&yymsp[-4].minor);
  yy_destructor(43,&yymsp[-2].minor);
}
// 3855 "parser.c"
        break;
      case 14:
// 1083 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy259, 0, 0, NULL, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(44,&yymsp[-4].minor);
  yy_destructor(45,&yymsp[-2].minor);
}
// 3864 "parser.c"
        break;
      case 15:
// 1087 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class(yymsp[-5].minor.yy0, yymsp[0].minor.yy259, 0, 0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(44,&yymsp[-6].minor);
  yy_destructor(43,&yymsp[-4].minor);
  yy_destructor(45,&yymsp[-2].minor);
}
// 3874 "parser.c"
        break;
      case 16:
// 1091 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class(yymsp[-1].minor.yy0, yymsp[0].minor.yy259, 1, 0, NULL, NULL, status->scanner_state);
  yy_destructor(46,&yymsp[-3].minor);
  yy_destructor(44,&yymsp[-2].minor);
}
// 3883 "parser.c"
        break;
      case 17:
// 1095 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy259, 1, 0, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(46,&yymsp[-5].minor);
  yy_destructor(44,&yymsp[-4].minor);
  yy_destructor(43,&yymsp[-2].minor);
}
// 3893 "parser.c"
        break;
      case 18:
// 1099 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy259, 1, 0, NULL, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(46,&yymsp[-5].minor);
  yy_destructor(44,&yymsp[-4].minor);
  yy_destructor(45,&yymsp[-2].minor);
}
// 3903 "parser.c"
        break;
      case 19:
// 1103 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class(yymsp[-1].minor.yy0, yymsp[0].minor.yy259, 0, 1, NULL, NULL, status->scanner_state);
  yy_destructor(47,&yymsp[-3].minor);
  yy_destructor(44,&yymsp[-2].minor);
}
// 3912 "parser.c"
        break;
      case 20:
// 1107 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy259, 0, 1, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(47,&yymsp[-5].minor);
  yy_destructor(44,&yymsp[-4].minor);
  yy_destructor(43,&yymsp[-2].minor);
}
// 3922 "parser.c"
        break;
      case 21:
      case 26:
      case 47:
// 1111 "parser.lemon"
{
	yygotominor.yy259 = NULL;
  yy_destructor(48,&yymsp[-1].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 3933 "parser.c"
        break;
      case 22:
      case 27:
      case 48:
// 1115 "parser.lemon"
{
	yygotominor.yy259 = yymsp[-1].minor.yy259;
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 3944 "parser.c"
        break;
      case 23:
      case 49:
      case 103:
      case 176:
      case 204:
      case 226:
      case 303:
      case 306:
// 1119 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_list(yymsp[-2].minor.yy259, yymsp[0].minor.yy259);
  yy_destructor(6,&yymsp[-1].minor);
}
// 3959 "parser.c"
        break;
      case 25:
      case 267:
      case 310:
// 1127 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state);
}
// 3968 "parser.c"
        break;
      case 28:
// 1139 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_definition(yymsp[0].minor.yy259, NULL, NULL, status->scanner_state);
}
// 3975 "parser.c"
        break;
      case 29:
// 1143 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_definition(NULL, NULL, yymsp[0].minor.yy259, status->scanner_state);
}
// 3982 "parser.c"
        break;
      case 30:
// 1147 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_definition(NULL, yymsp[0].minor.yy259, NULL, status->scanner_state);
}
// 3989 "parser.c"
        break;
      case 31:
// 1151 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_definition(yymsp[-1].minor.yy259, yymsp[0].minor.yy259, NULL, status->scanner_state);
}
// 3996 "parser.c"
        break;
      case 32:
// 1155 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_definition(yymsp[-1].minor.yy259, NULL, yymsp[0].minor.yy259, status->scanner_state);
}
// 4003 "parser.c"
        break;
      case 33:
// 1159 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_definition(yymsp[0].minor.yy259, NULL, yymsp[-1].minor.yy259, status->scanner_state);
}
// 4010 "parser.c"
        break;
      case 34:
// 1163 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_definition(NULL, yymsp[0].minor.yy259, yymsp[-1].minor.yy259, status->scanner_state);
}
// 4017 "parser.c"
        break;
      case 35:
// 1167 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_definition(yymsp[-2].minor.yy259, yymsp[0].minor.yy259, yymsp[-1].minor.yy259, status->scanner_state);
}
// 4024 "parser.c"
        break;
      case 36:
// 1171 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_definition(yymsp[-1].minor.yy259, yymsp[0].minor.yy259, yymsp[-2].minor.yy259, status->scanner_state);
}
// 4031 "parser.c"
        break;
      case 39:
// 1188 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_property(yymsp[-2].minor.yy259, yymsp[-1].minor.yy0, NULL, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(41,&yymsp[0].minor);
}
// 4039 "parser.c"
        break;
      case 40:
// 1192 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_property(yymsp[-2].minor.yy259, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(41,&yymsp[0].minor);
}
// 4047 "parser.c"
        break;
      case 41:
// 1196 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_property(yymsp[-4].minor.yy259, yymsp[-3].minor.yy0, yymsp[-1].minor.yy259, yymsp[-5].minor.yy0, NULL, status->scanner_state);
  yy_destructor(51,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 4056 "parser.c"
        break;
      case 42:
// 1200 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_property(yymsp[-4].minor.yy259, yymsp[-3].minor.yy0, yymsp[-1].minor.yy259, NULL, NULL, status->scanner_state);
  yy_destructor(51,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 4065 "parser.c"
        break;
      case 43:
// 1204 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_property(yymsp[-3].minor.yy259, yymsp[-2].minor.yy0, NULL, yymsp[-4].minor.yy0, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(41,&yymsp[0].minor);
}
// 4073 "parser.c"
        break;
      case 44:
// 1208 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_property(yymsp[-3].minor.yy259, yymsp[-2].minor.yy0, NULL, NULL, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(41,&yymsp[0].minor);
}
// 4081 "parser.c"
        break;
      case 45:
// 1212 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_property(yymsp[-5].minor.yy259, yymsp[-4].minor.yy0, yymsp[-2].minor.yy259, yymsp[-6].minor.yy0, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(51,&yymsp[-3].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 4090 "parser.c"
        break;
      case 46:
// 1216 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_property(yymsp[-5].minor.yy259, yymsp[-4].minor.yy0, yymsp[-2].minor.yy259, NULL, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(51,&yymsp[-3].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 4099 "parser.c"
        break;
      case 51:
// 1236 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_property_shortcut(NULL, yymsp[0].minor.yy0, status->scanner_state);
}
// 4106 "parser.c"
        break;
      case 52:
// 1240 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_property_shortcut(yymsp[-1].minor.yy0, yymsp[0].minor.yy0, status->scanner_state);
}
// 4113 "parser.c"
        break;
      case 59:
// 1269 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy259, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(52,&yymsp[-4].minor);
  yy_destructor(51,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 4123 "parser.c"
        break;
      case 60:
// 1273 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy259, NULL, status->scanner_state);
  yy_destructor(52,&yymsp[-4].minor);
  yy_destructor(51,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 4133 "parser.c"
        break;
      case 61:
// 1278 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_method(yymsp[-6].minor.yy259, yymsp[-4].minor.yy0, NULL, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(54,&yymsp[-5].minor);
  yy_destructor(55,&yymsp[-3].minor);
  yy_destructor(36,&yymsp[-2].minor);
  yy_destructor(48,&yymsp[-1].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4145 "parser.c"
        break;
      case 62:
// 1282 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_method(yymsp[-7].minor.yy259, yymsp[-5].minor.yy0, yymsp[-3].minor.yy259, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(54,&yymsp[-6].minor);
  yy_destructor(55,&yymsp[-4].minor);
  yy_destructor(36,&yymsp[-2].minor);
  yy_destructor(48,&yymsp[-1].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4157 "parser.c"
        break;
      case 63:
// 1286 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_method(yymsp[-7].minor.yy259, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy259, NULL, NULL, status->scanner_state);
  yy_destructor(54,&yymsp[-6].minor);
  yy_destructor(55,&yymsp[-4].minor);
  yy_destructor(36,&yymsp[-3].minor);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4169 "parser.c"
        break;
      case 64:
// 1290 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_method(yymsp[-8].minor.yy259, yymsp[-6].minor.yy0, yymsp[-4].minor.yy259, yymsp[-1].minor.yy259, NULL, NULL, status->scanner_state);
  yy_destructor(54,&yymsp[-7].minor);
  yy_destructor(55,&yymsp[-5].minor);
  yy_destructor(36,&yymsp[-3].minor);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4181 "parser.c"
        break;
      case 65:
// 1294 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_method(yymsp[-6].minor.yy259, yymsp[-4].minor.yy0, NULL, NULL, yymsp[-7].minor.yy0, NULL, status->scanner_state);
  yy_destructor(54,&yymsp[-5].minor);
  yy_destructor(55,&yymsp[-3].minor);
  yy_destructor(36,&yymsp[-2].minor);
  yy_destructor(48,&yymsp[-1].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4193 "parser.c"
        break;
      case 66:
// 1298 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_method(yymsp[-7].minor.yy259, yymsp[-5].minor.yy0, yymsp[-3].minor.yy259, NULL, yymsp[-8].minor.yy0, NULL, status->scanner_state);
  yy_destructor(54,&yymsp[-6].minor);
  yy_destructor(55,&yymsp[-4].minor);
  yy_destructor(36,&yymsp[-2].minor);
  yy_destructor(48,&yymsp[-1].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4205 "parser.c"
        break;
      case 67:
// 1302 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_method(yymsp[-7].minor.yy259, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy259, yymsp[-8].minor.yy0, NULL, status->scanner_state);
  yy_destructor(54,&yymsp[-6].minor);
  yy_destructor(55,&yymsp[-4].minor);
  yy_destructor(36,&yymsp[-3].minor);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4217 "parser.c"
        break;
      case 68:
// 1306 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_method(yymsp[-8].minor.yy259, yymsp[-6].minor.yy0, yymsp[-4].minor.yy259, yymsp[-1].minor.yy259, yymsp[-9].minor.yy0, NULL, status->scanner_state);
  yy_destructor(54,&yymsp[-7].minor);
  yy_destructor(55,&yymsp[-5].minor);
  yy_destructor(36,&yymsp[-3].minor);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4229 "parser.c"
        break;
      case 69:
// 1310 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_method(yymsp[-8].minor.yy259, yymsp[-6].minor.yy0, NULL, NULL, NULL, yymsp[-2].minor.yy259, status->scanner_state);
  yy_destructor(54,&yymsp[-7].minor);
  yy_destructor(55,&yymsp[-5].minor);
  yy_destructor(36,&yymsp[-4].minor);
  yy_destructor(38,&yymsp[-3].minor);
  yy_destructor(48,&yymsp[-1].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4242 "parser.c"
        break;
      case 70:
// 1314 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_method(yymsp[-9].minor.yy259, yymsp[-7].minor.yy0, yymsp[-5].minor.yy259, NULL, NULL, yymsp[-2].minor.yy259, status->scanner_state);
  yy_destructor(54,&yymsp[-8].minor);
  yy_destructor(55,&yymsp[-6].minor);
  yy_destructor(36,&yymsp[-4].minor);
  yy_destructor(38,&yymsp[-3].minor);
  yy_destructor(48,&yymsp[-1].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4255 "parser.c"
        break;
      case 71:
// 1318 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_method(yymsp[-9].minor.yy259, yymsp[-7].minor.yy0, NULL, yymsp[-1].minor.yy259, NULL, yymsp[-3].minor.yy259, status->scanner_state);
  yy_destructor(54,&yymsp[-8].minor);
  yy_destructor(55,&yymsp[-6].minor);
  yy_destructor(36,&yymsp[-5].minor);
  yy_destructor(38,&yymsp[-4].minor);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4268 "parser.c"
        break;
      case 72:
// 1322 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_method(yymsp[-10].minor.yy259, yymsp[-8].minor.yy0, yymsp[-6].minor.yy259, yymsp[-1].minor.yy259, NULL, yymsp[-3].minor.yy259, status->scanner_state);
  yy_destructor(54,&yymsp[-9].minor);
  yy_destructor(55,&yymsp[-7].minor);
  yy_destructor(36,&yymsp[-5].minor);
  yy_destructor(38,&yymsp[-4].minor);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4281 "parser.c"
        break;
      case 73:
// 1326 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_method(yymsp[-8].minor.yy259, yymsp[-6].minor.yy0, NULL, NULL, yymsp[-9].minor.yy0, yymsp[-2].minor.yy259, status->scanner_state);
  yy_destructor(54,&yymsp[-7].minor);
  yy_destructor(55,&yymsp[-5].minor);
  yy_destructor(36,&yymsp[-4].minor);
  yy_destructor(38,&yymsp[-3].minor);
  yy_destructor(48,&yymsp[-1].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4294 "parser.c"
        break;
      case 74:
// 1330 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_method(yymsp[-9].minor.yy259, yymsp[-7].minor.yy0, yymsp[-5].minor.yy259, NULL, yymsp[-10].minor.yy0, yymsp[-2].minor.yy259, status->scanner_state);
  yy_destructor(54,&yymsp[-8].minor);
  yy_destructor(55,&yymsp[-6].minor);
  yy_destructor(36,&yymsp[-4].minor);
  yy_destructor(38,&yymsp[-3].minor);
  yy_destructor(48,&yymsp[-1].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4307 "parser.c"
        break;
      case 75:
// 1334 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_method(yymsp[-9].minor.yy259, yymsp[-7].minor.yy0, NULL, yymsp[-1].minor.yy259, yymsp[-10].minor.yy0, yymsp[-3].minor.yy259, status->scanner_state);
  yy_destructor(54,&yymsp[-8].minor);
  yy_destructor(55,&yymsp[-6].minor);
  yy_destructor(36,&yymsp[-5].minor);
  yy_destructor(38,&yymsp[-4].minor);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4320 "parser.c"
        break;
      case 76:
// 1338 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_method(yymsp[-10].minor.yy259, yymsp[-8].minor.yy0, yymsp[-6].minor.yy259, yymsp[-1].minor.yy259, yymsp[-11].minor.yy0, yymsp[-3].minor.yy259, status->scanner_state);
  yy_destructor(54,&yymsp[-9].minor);
  yy_destructor(55,&yymsp[-7].minor);
  yy_destructor(36,&yymsp[-5].minor);
  yy_destructor(38,&yymsp[-4].minor);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4333 "parser.c"
        break;
      case 77:
// 1343 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_method(yymsp[-7].minor.yy259, yymsp[-5].minor.yy0, NULL, NULL, NULL, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(54,&yymsp[-6].minor);
  yy_destructor(55,&yymsp[-4].minor);
  yy_destructor(36,&yymsp[-3].minor);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 4345 "parser.c"
        break;
      case 78:
// 1347 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_method(yymsp[-8].minor.yy259, yymsp[-6].minor.yy0, yymsp[-4].minor.yy259, NULL, NULL, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(54,&yymsp[-7].minor);
  yy_destructor(55,&yymsp[-5].minor);
  yy_destructor(36,&yymsp[-3].minor);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 4357 "parser.c"
        break;
      case 79:
// 1351 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_method(yymsp[-7].minor.yy259, yymsp[-5].minor.yy0, NULL, NULL, yymsp[-8].minor.yy0, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(54,&yymsp[-6].minor);
  yy_destructor(55,&yymsp[-4].minor);
  yy_destructor(36,&yymsp[-3].minor);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 4369 "parser.c"
        break;
      case 80:
// 1355 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_method(yymsp[-8].minor.yy259, yymsp[-6].minor.yy0, yymsp[-4].minor.yy259, NULL, yymsp[-9].minor.yy0, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(54,&yymsp[-7].minor);
  yy_destructor(55,&yymsp[-5].minor);
  yy_destructor(36,&yymsp[-3].minor);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 4381 "parser.c"
        break;
      case 81:
// 1359 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_method(yymsp[-5].minor.yy259, yymsp[-3].minor.yy0, NULL, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(54,&yymsp[-4].minor);
  yy_destructor(55,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[-1].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 4392 "parser.c"
        break;
      case 82:
// 1363 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_method(yymsp[-6].minor.yy259, yymsp[-4].minor.yy0, yymsp[-2].minor.yy259, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(54,&yymsp[-5].minor);
  yy_destructor(55,&yymsp[-3].minor);
  yy_destructor(36,&yymsp[-1].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 4403 "parser.c"
        break;
      case 83:
// 1367 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_method(yymsp[-5].minor.yy259, yymsp[-3].minor.yy0, NULL, NULL, yymsp[-6].minor.yy0, NULL, status->scanner_state);
  yy_destructor(54,&yymsp[-4].minor);
  yy_destructor(55,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[-1].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 4414 "parser.c"
        break;
      case 84:
// 1371 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_method(yymsp[-6].minor.yy259, yymsp[-4].minor.yy0, yymsp[-2].minor.yy259, NULL, yymsp[-7].minor.yy0, NULL, status->scanner_state);
  yy_destructor(54,&yymsp[-5].minor);
  yy_destructor(55,&yymsp[-3].minor);
  yy_destructor(36,&yymsp[-1].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 4425 "parser.c"
        break;
      case 87:
// 1384 "parser.lemon"
{
	yygotominor.yy259 = json_object_new_string("public");
  yy_destructor(1,&yymsp[0].minor);
}
// 4433 "parser.c"
        break;
      case 88:
// 1388 "parser.lemon"
{
	yygotominor.yy259 = json_object_new_string("protected");
  yy_destructor(2,&yymsp[0].minor);
}
// 4441 "parser.c"
        break;
      case 89:
// 1392 "parser.lemon"
{
	yygotominor.yy259 = json_object_new_string("private");
  yy_destructor(4,&yymsp[0].minor);
}
// 4449 "parser.c"
        break;
      case 90:
// 1396 "parser.lemon"
{
	yygotominor.yy259 = json_object_new_string("static");
  yy_destructor(3,&yymsp[0].minor);
}
// 4457 "parser.c"
        break;
      case 91:
// 1400 "parser.lemon"
{
	yygotominor.yy259 = json_object_new_string("scoped");
  yy_destructor(5,&yymsp[0].minor);
}
// 4465 "parser.c"
        break;
      case 92:
// 1404 "parser.lemon"
{
	yygotominor.yy259 = json_object_new_string("inline");
  yy_destructor(56,&yymsp[0].minor);
}
// 4473 "parser.c"
        break;
      case 93:
// 1408 "parser.lemon"
{
	yygotominor.yy259 = json_object_new_string("abstract");
  yy_destructor(46,&yymsp[0].minor);
}
// 4481 "parser.c"
        break;
      case 94:
// 1412 "parser.lemon"
{
	yygotominor.yy259 = json_object_new_string("final");
  yy_destructor(47,&yymsp[0].minor);
}
// 4489 "parser.c"
        break;
      case 95:
// 1417 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_return_type(1, NULL, status->scanner_state);
  yy_destructor(57,&yymsp[0].minor);
}
// 4497 "parser.c"
        break;
      case 96:
// 1421 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_return_type(0, yymsp[0].minor.yy259, status->scanner_state);
}
// 4504 "parser.c"
        break;
      case 97:
// 1425 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_list(yymsp[-2].minor.yy259, yymsp[0].minor.yy259);
  yy_destructor(15,&yymsp[-1].minor);
}
// 4512 "parser.c"
        break;
      case 99:
// 1433 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_return_type_item(yymsp[0].minor.yy259, NULL, 0, 0, status->scanner_state);
}
// 4519 "parser.c"
        break;
      case 100:
// 1437 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_return_type_item(yymsp[-1].minor.yy259, NULL, 1, 0, status->scanner_state);
  yy_destructor(35,&yymsp[0].minor);
}
// 4527 "parser.c"
        break;
      case 101:
// 1441 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_return_type_item(NULL, yymsp[0].minor.yy259, 0, 0, status->scanner_state);
}
// 4534 "parser.c"
        break;
      case 102:
// 1445 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_return_type_item(NULL, yymsp[0].minor.yy259, 0, 1, status->scanner_state);
}
// 4541 "parser.c"
        break;
      case 105:
// 1458 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_parameter(NULL, NULL, yymsp[0].minor.yy0, NULL, 0, status->scanner_state);
}
// 4548 "parser.c"
        break;
      case 106:
// 1462 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_parameter(yymsp[-1].minor.yy259, NULL, yymsp[0].minor.yy0, NULL, 0, status->scanner_state);
}
// 4555 "parser.c"
        break;
      case 107:
// 1466 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_parameter(yymsp[-2].minor.yy259, NULL, yymsp[0].minor.yy0, NULL, 1, status->scanner_state);
  yy_destructor(35,&yymsp[-1].minor);
}
// 4563 "parser.c"
        break;
      case 108:
// 1470 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_parameter(NULL, yymsp[-1].minor.yy259, yymsp[0].minor.yy0, NULL, 0, status->scanner_state);
}
// 4570 "parser.c"
        break;
      case 109:
// 1474 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_parameter(NULL, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy259, 0, status->scanner_state);
  yy_destructor(51,&yymsp[-1].minor);
}
// 4578 "parser.c"
        break;
      case 110:
// 1478 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_parameter(yymsp[-3].minor.yy259, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy259, 0, status->scanner_state);
  yy_destructor(51,&yymsp[-1].minor);
}
// 4586 "parser.c"
        break;
      case 111:
// 1482 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_parameter(yymsp[-4].minor.yy259, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy259, 1, status->scanner_state);
  yy_destructor(35,&yymsp[-3].minor);
  yy_destructor(51,&yymsp[-1].minor);
}
// 4595 "parser.c"
        break;
      case 112:
// 1486 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_parameter(NULL, yymsp[-3].minor.yy259, yymsp[-2].minor.yy0, yymsp[0].minor.yy259, 0, status->scanner_state);
  yy_destructor(51,&yymsp[-1].minor);
}
// 4603 "parser.c"
        break;
      case 113:
// 1490 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(19,&yymsp[-2].minor);
  yy_destructor(20,&yymsp[0].minor);
}
// 4612 "parser.c"
        break;
      case 114:
// 1494 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(19,&yymsp[-4].minor);
  yy_destructor(37,&yymsp[-2].minor);
  yy_destructor(58,&yymsp[-1].minor);
  yy_destructor(20,&yymsp[0].minor);
}
// 4623 "parser.c"
        break;
      case 115:
// 1498 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_type(XX_TYPE_INTEGER);
  yy_destructor(59,&yymsp[0].minor);
}
// 4631 "parser.c"
        break;
      case 116:
// 1502 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_type(XX_TYPE_UINTEGER);
  yy_destructor(60,&yymsp[0].minor);
}
// 4639 "parser.c"
        break;
      case 117:
// 1506 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_type(XX_TYPE_LONG);
  yy_destructor(61,&yymsp[0].minor);
}
// 4647 "parser.c"
        break;
      case 118:
// 1510 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_type(XX_TYPE_ULONG);
  yy_destructor(62,&yymsp[0].minor);
}
// 4655 "parser.c"
        break;
      case 119:
// 1514 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_type(XX_TYPE_CHAR);
  yy_destructor(63,&yymsp[0].minor);
}
// 4663 "parser.c"
        break;
      case 120:
// 1518 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_type(XX_TYPE_UCHAR);
  yy_destructor(64,&yymsp[0].minor);
}
// 4671 "parser.c"
        break;
      case 121:
// 1522 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_type(XX_TYPE_DOUBLE);
  yy_destructor(65,&yymsp[0].minor);
}
// 4679 "parser.c"
        break;
      case 122:
// 1526 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_type(XX_TYPE_BOOL);
  yy_destructor(66,&yymsp[0].minor);
}
// 4687 "parser.c"
        break;
      case 123:
// 1530 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_type(XX_TYPE_STRING);
  yy_destructor(67,&yymsp[0].minor);
}
// 4695 "parser.c"
        break;
      case 124:
// 1534 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_type(XX_TYPE_ARRAY);
  yy_destructor(68,&yymsp[0].minor);
}
// 4703 "parser.c"
        break;
      case 125:
// 1538 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_type(XX_TYPE_VAR);
  yy_destructor(69,&yymsp[0].minor);
}
// 4711 "parser.c"
        break;
      case 149:
// 1636 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_empty_statement(status->scanner_state);
  yy_destructor(41,&yymsp[0].minor);
}
// 4719 "parser.c"
        break;
      case 150:
// 1640 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_break_statement(status->scanner_state);
  yy_destructor(70,&yymsp[-1].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 4728 "parser.c"
        break;
      case 151:
// 1644 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_continue_statement(status->scanner_state);
  yy_destructor(71,&yymsp[-1].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 4737 "parser.c"
        break;
      case 152:
// 1648 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_if_statement(yymsp[-2].minor.yy259, NULL, NULL, status->scanner_state);
  yy_destructor(72,&yymsp[-3].minor);
  yy_destructor(48,&yymsp[-1].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4747 "parser.c"
        break;
      case 153:
// 1652 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_if_statement(yymsp[-5].minor.yy259, NULL, NULL, status->scanner_state);
  yy_destructor(72,&yymsp[-6].minor);
  yy_destructor(48,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-3].minor);
  yy_destructor(73,&yymsp[-2].minor);
  yy_destructor(48,&yymsp[-1].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4760 "parser.c"
        break;
      case 154:
// 1656 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_if_statement(yymsp[-3].minor.yy259, yymsp[-1].minor.yy259, NULL, status->scanner_state);
  yy_destructor(72,&yymsp[-4].minor);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4770 "parser.c"
        break;
      case 155:
// 1660 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_if_statement(yymsp[-7].minor.yy259, yymsp[-5].minor.yy259, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(72,&yymsp[-8].minor);
  yy_destructor(48,&yymsp[-6].minor);
  yy_destructor(49,&yymsp[-4].minor);
  yy_destructor(73,&yymsp[-3].minor);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4783 "parser.c"
        break;
      case 156:
// 1664 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_if_statement(yymsp[-6].minor.yy259, yymsp[-4].minor.yy259, NULL, status->scanner_state);
  yy_destructor(72,&yymsp[-7].minor);
  yy_destructor(48,&yymsp[-5].minor);
  yy_destructor(49,&yymsp[-3].minor);
  yy_destructor(73,&yymsp[-2].minor);
  yy_destructor(48,&yymsp[-1].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4796 "parser.c"
        break;
      case 157:
// 1668 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_if_statement(yymsp[-6].minor.yy259, NULL, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(72,&yymsp[-7].minor);
  yy_destructor(48,&yymsp[-5].minor);
  yy_destructor(49,&yymsp[-4].minor);
  yy_destructor(73,&yymsp[-3].minor);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4809 "parser.c"
        break;
      case 158:
// 1672 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_switch_statement(yymsp[-2].minor.yy259, NULL, status->scanner_state);
  yy_destructor(74,&yymsp[-3].minor);
  yy_destructor(48,&yymsp[-1].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4819 "parser.c"
        break;
      case 159:
// 1676 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_switch_statement(yymsp[-3].minor.yy259, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(74,&yymsp[-4].minor);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4829 "parser.c"
        break;
      case 162:
// 1688 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_case_clause(yymsp[-1].minor.yy259, NULL, status->scanner_state);
  yy_destructor(75,&yymsp[-2].minor);
  yy_destructor(76,&yymsp[0].minor);
}
// 4838 "parser.c"
        break;
      case 163:
// 1692 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_case_clause(yymsp[-2].minor.yy259, yymsp[0].minor.yy259, status->scanner_state);
  yy_destructor(75,&yymsp[-3].minor);
  yy_destructor(76,&yymsp[-1].minor);
}
// 4847 "parser.c"
        break;
      case 164:
// 1696 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_case_clause(NULL, yymsp[0].minor.yy259, status->scanner_state);
  yy_destructor(77,&yymsp[-2].minor);
  yy_destructor(76,&yymsp[-1].minor);
}
// 4856 "parser.c"
        break;
      case 165:
// 1700 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_loop_statement(NULL, status->scanner_state);
  yy_destructor(78,&yymsp[-2].minor);
  yy_destructor(48,&yymsp[-1].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4866 "parser.c"
        break;
      case 166:
// 1704 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_loop_statement(yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(78,&yymsp[-3].minor);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4876 "parser.c"
        break;
      case 167:
// 1708 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_while_statement(yymsp[-2].minor.yy259, NULL, status->scanner_state);
  yy_destructor(79,&yymsp[-3].minor);
  yy_destructor(48,&yymsp[-1].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4886 "parser.c"
        break;
      case 168:
// 1712 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_while_statement(yymsp[-3].minor.yy259, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(79,&yymsp[-4].minor);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4896 "parser.c"
        break;
      case 169:
// 1716 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_do_while_statement(yymsp[-1].minor.yy259, NULL, status->scanner_state);
  yy_destructor(80,&yymsp[-5].minor);
  yy_destructor(48,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-3].minor);
  yy_destructor(79,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 4908 "parser.c"
        break;
      case 170:
// 1720 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_do_while_statement(yymsp[-1].minor.yy259, yymsp[-4].minor.yy259, status->scanner_state);
  yy_destructor(80,&yymsp[-6].minor);
  yy_destructor(48,&yymsp[-5].minor);
  yy_destructor(49,&yymsp[-3].minor);
  yy_destructor(79,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 4920 "parser.c"
        break;
      case 171:
// 1724 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_for_statement(yymsp[-3].minor.yy259, NULL, yymsp[-5].minor.yy0, 0, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(81,&yymsp[-6].minor);
  yy_destructor(82,&yymsp[-4].minor);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4931 "parser.c"
        break;
      case 172:
// 1728 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_for_statement(yymsp[-3].minor.yy259, NULL, yymsp[-6].minor.yy0, 1, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(81,&yymsp[-7].minor);
  yy_destructor(82,&yymsp[-5].minor);
  yy_destructor(83,&yymsp[-4].minor);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4943 "parser.c"
        break;
      case 173:
// 1732 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_for_statement(yymsp[-3].minor.yy259, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, 0, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(81,&yymsp[-8].minor);
  yy_destructor(6,&yymsp[-6].minor);
  yy_destructor(82,&yymsp[-4].minor);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4955 "parser.c"
        break;
      case 174:
// 1736 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_for_statement(yymsp[-3].minor.yy259, yymsp[-8].minor.yy0, yymsp[-6].minor.yy0, 1, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(81,&yymsp[-9].minor);
  yy_destructor(6,&yymsp[-7].minor);
  yy_destructor(82,&yymsp[-5].minor);
  yy_destructor(83,&yymsp[-4].minor);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4968 "parser.c"
        break;
      case 175:
// 1740 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_let_statement(yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(84,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 4977 "parser.c"
        break;
      case 178:
// 1753 "parser.lemon"
{
	yygotominor.yy259 = json_object_new_string("assign");
  yy_destructor(51,&yymsp[0].minor);
}
// 4985 "parser.c"
        break;
      case 179:
// 1758 "parser.lemon"
{
	yygotominor.yy259 = json_object_new_string("add-assign");
  yy_destructor(85,&yymsp[0].minor);
}
// 4993 "parser.c"
        break;
      case 180:
// 1763 "parser.lemon"
{
	yygotominor.yy259 = json_object_new_string("sub-assign");
  yy_destructor(86,&yymsp[0].minor);
}
// 5001 "parser.c"
        break;
      case 181:
// 1767 "parser.lemon"
{
	yygotominor.yy259 = json_object_new_string("mult-assign");
  yy_destructor(87,&yymsp[0].minor);
}
// 5009 "parser.c"
        break;
      case 182:
// 1771 "parser.lemon"
{
	yygotominor.yy259 = json_object_new_string("div-assign");
  yy_destructor(88,&yymsp[0].minor);
}
// 5017 "parser.c"
        break;
      case 183:
// 1775 "parser.lemon"
{
	yygotominor.yy259 = json_object_new_string("concat-assign");
  yy_destructor(89,&yymsp[0].minor);
}
// 5025 "parser.c"
        break;
      case 184:
// 1780 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_let_assignment("variable", yymsp[-1].minor.yy259, yymsp[-2].minor.yy0, NULL, NULL, yymsp[0].minor.yy259, status->scanner_state);
}
// 5032 "parser.c"
        break;
      case 185:
// 1785 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_let_assignment("object-property", yymsp[-1].minor.yy259, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, yymsp[0].minor.yy259, status->scanner_state);
  yy_destructor(38,&yymsp[-3].minor);
}
// 5040 "parser.c"
        break;
      case 186:
// 1790 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_let_assignment("variable-dynamic-object-property", yymsp[-1].minor.yy259, yymsp[-6].minor.yy0, yymsp[-3].minor.yy0, NULL, yymsp[0].minor.yy259, status->scanner_state);
  yy_destructor(38,&yymsp[-5].minor);
  yy_destructor(48,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
}
// 5050 "parser.c"
        break;
      case 187:
// 1795 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_let_assignment("string-dynamic-object-property", yymsp[-1].minor.yy259, yymsp[-6].minor.yy0, yymsp[-3].minor.yy0, NULL, yymsp[0].minor.yy259, status->scanner_state);
  yy_destructor(38,&yymsp[-5].minor);
  yy_destructor(48,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
}
// 5060 "parser.c"
        break;
      case 188:
// 1800 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_let_assignment("object-property-append", yymsp[-1].minor.yy259, yymsp[-6].minor.yy0, yymsp[-4].minor.yy0, NULL, yymsp[0].minor.yy259, status->scanner_state);
  yy_destructor(38,&yymsp[-5].minor);
  yy_destructor(37,&yymsp[-3].minor);
  yy_destructor(58,&yymsp[-2].minor);
}
// 5070 "parser.c"
        break;
      case 189:
// 1805 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_let_assignment("object-property-array-index", yymsp[-1].minor.yy259, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, yymsp[-3].minor.yy259, yymsp[0].minor.yy259, status->scanner_state);
  yy_destructor(38,&yymsp[-6].minor);
  yy_destructor(37,&yymsp[-4].minor);
  yy_destructor(58,&yymsp[-2].minor);
}
// 5080 "parser.c"
        break;
      case 190:
// 1810 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_let_assignment("static-property", yymsp[-1].minor.yy259, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, yymsp[0].minor.yy259, status->scanner_state);
  yy_destructor(91,&yymsp[-3].minor);
}
// 5088 "parser.c"
        break;
      case 191:
// 1815 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_let_assignment("static-property-append", yymsp[-1].minor.yy259, yymsp[-6].minor.yy0, yymsp[-4].minor.yy0, NULL, yymsp[0].minor.yy259, status->scanner_state);
  yy_destructor(91,&yymsp[-5].minor);
  yy_destructor(37,&yymsp[-3].minor);
  yy_destructor(58,&yymsp[-2].minor);
}
// 5098 "parser.c"
        break;
      case 192:
// 1820 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_let_assignment("static-property-array-index", yymsp[-1].minor.yy259, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, yymsp[-3].minor.yy259, yymsp[0].minor.yy259, status->scanner_state);
  yy_destructor(91,&yymsp[-6].minor);
  yy_destructor(37,&yymsp[-4].minor);
  yy_destructor(58,&yymsp[-2].minor);
}
// 5108 "parser.c"
        break;
      case 193:
// 1825 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_let_assignment("variable-append", yymsp[-1].minor.yy259, yymsp[-4].minor.yy0, NULL, NULL, yymsp[0].minor.yy259, status->scanner_state);
  yy_destructor(37,&yymsp[-3].minor);
  yy_destructor(58,&yymsp[-2].minor);
}
// 5117 "parser.c"
        break;
      case 194:
// 1830 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_let_assignment("array-index", yymsp[-1].minor.yy259, yymsp[-3].minor.yy0, NULL, yymsp[-2].minor.yy259, yymsp[0].minor.yy259, status->scanner_state);
}
// 5124 "parser.c"
        break;
      case 197:
// 1842 "parser.lemon"
{
	yygotominor.yy259 = yymsp[-1].minor.yy259;
  yy_destructor(37,&yymsp[-2].minor);
  yy_destructor(58,&yymsp[0].minor);
}
// 5133 "parser.c"
        break;
      case 198:
// 1847 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_let_assignment("incr", NULL, yymsp[-3].minor.yy0, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(92,&yymsp[0].minor);
}
// 5142 "parser.c"
        break;
      case 199:
// 1852 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_let_assignment("decr", NULL, yymsp[-3].minor.yy0, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(93,&yymsp[0].minor);
}
// 5151 "parser.c"
        break;
      case 200:
// 1857 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_let_assignment("incr", NULL, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(92,&yymsp[0].minor);
}
// 5159 "parser.c"
        break;
      case 201:
// 1862 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_let_assignment("decr", NULL, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(93,&yymsp[0].minor);
}
// 5167 "parser.c"
        break;
      case 203:
// 1870 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_echo_statement(yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(94,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 5176 "parser.c"
        break;
      case 206:
// 1882 "parser.lemon"
{
	yygotominor.yy259 = yymsp[0].minor.yy259;;
}
// 5183 "parser.c"
        break;
      case 207:
// 1887 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_mcall_statement(yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(41,&yymsp[0].minor);
}
// 5191 "parser.c"
        break;
      case 208:
// 1892 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_fcall_statement(yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(41,&yymsp[0].minor);
}
// 5199 "parser.c"
        break;
      case 209:
// 1897 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_scall_statement(yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(41,&yymsp[0].minor);
}
// 5207 "parser.c"
        break;
      case 210:
// 1902 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_fetch_statement(yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(41,&yymsp[0].minor);
}
// 5215 "parser.c"
        break;
      case 211:
// 1907 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_return_statement(yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(95,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 5224 "parser.c"
        break;
      case 212:
// 1912 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_return_statement(NULL, status->scanner_state);
  yy_destructor(95,&yymsp[-1].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 5233 "parser.c"
        break;
      case 213:
// 1917 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(96,&yymsp[-4].minor);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 5247 "parser.c"
        break;
      case 214:
// 1926 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state),
		yymsp[-2].minor.yy259,
		status->scanner_state
	);
  yy_destructor(96,&yymsp[-5].minor);
  yy_destructor(37,&yymsp[-3].minor);
  yy_destructor(58,&yymsp[-1].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 5262 "parser.c"
        break;
      case 215:
// 1935 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_throw_exception(yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(97,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 5271 "parser.c"
        break;
      case 216:
// 1939 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_declare_statement(XX_T_TYPE_INTEGER, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(59,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 5280 "parser.c"
        break;
      case 217:
// 1943 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_declare_statement(XX_T_TYPE_UINTEGER, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(60,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 5289 "parser.c"
        break;
      case 218:
// 1947 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_declare_statement(XX_T_TYPE_CHAR, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(63,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 5298 "parser.c"
        break;
      case 219:
// 1951 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_declare_statement(XX_T_TYPE_UCHAR, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(64,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 5307 "parser.c"
        break;
      case 220:
// 1955 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_declare_statement(XX_T_TYPE_LONG, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(61,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 5316 "parser.c"
        break;
      case 221:
// 1959 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_declare_statement(XX_T_TYPE_ULONG, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(62,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 5325 "parser.c"
        break;
      case 222:
// 1963 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_declare_statement(XX_T_TYPE_DOUBLE, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(65,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 5334 "parser.c"
        break;
      case 223:
// 1967 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_declare_statement(XX_T_TYPE_STRING, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(67,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 5343 "parser.c"
        break;
      case 224:
// 1971 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_declare_statement(XX_T_TYPE_BOOL, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(66,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 5352 "parser.c"
        break;
      case 225:
// 1975 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_declare_statement(XX_T_TYPE_VAR, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(69,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 5361 "parser.c"
        break;
      case 228:
// 1987 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_declare_variable(yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 5368 "parser.c"
        break;
      case 229:
// 1991 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_declare_variable(yymsp[-2].minor.yy0, yymsp[0].minor.yy259, status->scanner_state);
  yy_destructor(51,&yymsp[-1].minor);
}
// 5376 "parser.c"
        break;
      case 231:
// 1999 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("not", yymsp[0].minor.yy259, NULL, NULL, status->scanner_state);
  yy_destructor(35,&yymsp[-1].minor);
}
// 5384 "parser.c"
        break;
      case 232:
// 2003 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("isset", yymsp[0].minor.yy259, NULL, NULL, status->scanner_state);
  yy_destructor(31,&yymsp[-1].minor);
}
// 5392 "parser.c"
        break;
      case 233:
// 2007 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("require", yymsp[0].minor.yy259, NULL, NULL, status->scanner_state);
  yy_destructor(7,&yymsp[-1].minor);
}
// 5400 "parser.c"
        break;
      case 234:
// 2011 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("clone", yymsp[0].minor.yy259, NULL, NULL, status->scanner_state);
  yy_destructor(10,&yymsp[-1].minor);
}
// 5408 "parser.c"
        break;
      case 235:
// 2015 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("empty", yymsp[0].minor.yy259, NULL, NULL, status->scanner_state);
  yy_destructor(11,&yymsp[-1].minor);
}
// 5416 "parser.c"
        break;
      case 236:
// 2019 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("likely", yymsp[0].minor.yy259, NULL, NULL, status->scanner_state);
  yy_destructor(8,&yymsp[-1].minor);
}
// 5424 "parser.c"
        break;
      case 237:
// 2023 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("unlikely", yymsp[0].minor.yy259, NULL, NULL, status->scanner_state);
  yy_destructor(9,&yymsp[-1].minor);
}
// 5432 "parser.c"
        break;
      case 238:
// 2027 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("equals", yymsp[-2].minor.yy259, yymsp[0].minor.yy259, NULL, status->scanner_state);
  yy_destructor(17,&yymsp[-1].minor);
}
// 5440 "parser.c"
        break;
      case 239:
// 2031 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("not-equals", yymsp[-2].minor.yy259, yymsp[0].minor.yy259, NULL, status->scanner_state);
  yy_destructor(24,&yymsp[-1].minor);
}
// 5448 "parser.c"
        break;
      case 240:
// 2035 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("identical", yymsp[-2].minor.yy259, yymsp[0].minor.yy259, NULL, status->scanner_state);
  yy_destructor(18,&yymsp[-1].minor);
}
// 5456 "parser.c"
        break;
      case 241:
// 2039 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("not-identical", yymsp[-2].minor.yy259, yymsp[0].minor.yy259, NULL, status->scanner_state);
  yy_destructor(23,&yymsp[-1].minor);
}
// 5464 "parser.c"
        break;
      case 242:
// 2043 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("less", yymsp[-2].minor.yy259, yymsp[0].minor.yy259, NULL, status->scanner_state);
  yy_destructor(19,&yymsp[-1].minor);
}
// 5472 "parser.c"
        break;
      case 243:
// 2047 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("greater", yymsp[-2].minor.yy259, yymsp[0].minor.yy259, NULL, status->scanner_state);
  yy_destructor(20,&yymsp[-1].minor);
}
// 5480 "parser.c"
        break;
      case 244:
// 2051 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("less-equal", yymsp[-2].minor.yy259, yymsp[0].minor.yy259, NULL, status->scanner_state);
  yy_destructor(21,&yymsp[-1].minor);
}
// 5488 "parser.c"
        break;
      case 245:
// 2055 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("greater-equal", yymsp[-2].minor.yy259, yymsp[0].minor.yy259, NULL, status->scanner_state);
  yy_destructor(22,&yymsp[-1].minor);
}
// 5496 "parser.c"
        break;
      case 246:
// 2059 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("list", yymsp[-1].minor.yy259, NULL, NULL, status->scanner_state);
  yy_destructor(55,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5505 "parser.c"
        break;
      case 247:
// 2063 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("cast", yymsp[-2].minor.yy259, yymsp[0].minor.yy259, NULL, status->scanner_state);
  yy_destructor(55,&yymsp[-3].minor);
  yy_destructor(36,&yymsp[-1].minor);
}
// 5514 "parser.c"
        break;
      case 248:
// 2067 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("type-hint", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), yymsp[0].minor.yy259, NULL, status->scanner_state);
  yy_destructor(19,&yymsp[-3].minor);
  yy_destructor(20,&yymsp[-1].minor);
}
// 5523 "parser.c"
        break;
      case 249:
// 2071 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("property-access", yymsp[-2].minor.yy259, xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-1].minor);
}
// 5531 "parser.c"
        break;
      case 250:
// 2075 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("property-dynamic-access", yymsp[-4].minor.yy259, xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-3].minor);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 5541 "parser.c"
        break;
      case 251:
// 2079 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("property-string-access", yymsp[-4].minor.yy259, xx_ret_literal(XX_T_STRING, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-3].minor);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 5551 "parser.c"
        break;
      case 252:
// 2083 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("static-property-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(91,&yymsp[-1].minor);
}
// 5559 "parser.c"
        break;
      case 253:
      case 321:
// 2087 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("static-constant-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(91,&yymsp[-1].minor);
}
// 5568 "parser.c"
        break;
      case 254:
// 2096 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("array-access", yymsp[-3].minor.yy259, yymsp[-1].minor.yy259, NULL, status->scanner_state);
  yy_destructor(37,&yymsp[-2].minor);
  yy_destructor(58,&yymsp[0].minor);
}
// 5577 "parser.c"
        break;
      case 255:
// 2101 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("add", yymsp[-2].minor.yy259, yymsp[0].minor.yy259, NULL, status->scanner_state);
  yy_destructor(25,&yymsp[-1].minor);
}
// 5585 "parser.c"
        break;
      case 256:
// 2106 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("sub", yymsp[-2].minor.yy259, yymsp[0].minor.yy259, NULL, status->scanner_state);
  yy_destructor(26,&yymsp[-1].minor);
}
// 5593 "parser.c"
        break;
      case 257:
// 2111 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("mul", yymsp[-2].minor.yy259, yymsp[0].minor.yy259, NULL, status->scanner_state);
  yy_destructor(28,&yymsp[-1].minor);
}
// 5601 "parser.c"
        break;
      case 258:
// 2116 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("div", yymsp[-2].minor.yy259, yymsp[0].minor.yy259, NULL, status->scanner_state);
  yy_destructor(29,&yymsp[-1].minor);
}
// 5609 "parser.c"
        break;
      case 259:
// 2121 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("mod", yymsp[-2].minor.yy259, yymsp[0].minor.yy259, NULL, status->scanner_state);
  yy_destructor(30,&yymsp[-1].minor);
}
// 5617 "parser.c"
        break;
      case 260:
// 2126 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("concat", yymsp[-2].minor.yy259, yymsp[0].minor.yy259, NULL, status->scanner_state);
  yy_destructor(27,&yymsp[-1].minor);
}
// 5625 "parser.c"
        break;
      case 261:
// 2131 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("and", yymsp[-2].minor.yy259, yymsp[0].minor.yy259, NULL, status->scanner_state);
  yy_destructor(13,&yymsp[-1].minor);
}
// 5633 "parser.c"
        break;
      case 262:
// 2136 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("or", yymsp[-2].minor.yy259, yymsp[0].minor.yy259, NULL, status->scanner_state);
  yy_destructor(14,&yymsp[-1].minor);
}
// 5641 "parser.c"
        break;
      case 263:
// 2141 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("instanceof", yymsp[-2].minor.yy259, yymsp[0].minor.yy259, NULL, status->scanner_state);
  yy_destructor(12,&yymsp[-1].minor);
}
// 5649 "parser.c"
        break;
      case 264:
// 2146 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("fetch", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), yymsp[0].minor.yy259, NULL, status->scanner_state);
  yy_destructor(32,&yymsp[-3].minor);
  yy_destructor(6,&yymsp[-1].minor);
}
// 5658 "parser.c"
        break;
      case 266:
// 2156 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("typeof", yymsp[0].minor.yy259, NULL, NULL, status->scanner_state);
  yy_destructor(33,&yymsp[-1].minor);
}
// 5666 "parser.c"
        break;
      case 268:
      case 312:
      case 314:
// 2166 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_literal(XX_T_INTEGER, yymsp[0].minor.yy0, status->scanner_state);
}
// 5675 "parser.c"
        break;
      case 269:
      case 311:
      case 316:
// 2171 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_literal(XX_T_STRING, yymsp[0].minor.yy0, status->scanner_state);
}
// 5684 "parser.c"
        break;
      case 270:
      case 315:
// 2176 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_literal(XX_T_CHAR, yymsp[0].minor.yy0, status->scanner_state);
}
// 5692 "parser.c"
        break;
      case 271:
      case 317:
// 2181 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_literal(XX_T_DOUBLE, yymsp[0].minor.yy0, status->scanner_state);
}
// 5700 "parser.c"
        break;
      case 272:
      case 318:
// 2186 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_literal(XX_T_NULL, NULL, status->scanner_state);
  yy_destructor(101,&yymsp[0].minor);
}
// 5709 "parser.c"
        break;
      case 273:
      case 320:
// 2191 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_literal(XX_T_TRUE, NULL, status->scanner_state);
  yy_destructor(102,&yymsp[0].minor);
}
// 5718 "parser.c"
        break;
      case 274:
      case 319:
// 2196 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_literal(XX_T_FALSE, NULL, status->scanner_state);
  yy_destructor(103,&yymsp[0].minor);
}
// 5727 "parser.c"
        break;
      case 275:
      case 322:
// 2201 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_literal(XX_T_CONSTANT, yymsp[0].minor.yy0, status->scanner_state);
}
// 5735 "parser.c"
        break;
      case 276:
// 2206 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("empty-array", NULL, NULL, NULL, status->scanner_state);
  yy_destructor(37,&yymsp[-1].minor);
  yy_destructor(58,&yymsp[0].minor);
}
// 5744 "parser.c"
        break;
      case 277:
// 2211 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("array", yymsp[-1].minor.yy259, NULL, NULL, status->scanner_state);
  yy_destructor(37,&yymsp[-2].minor);
  yy_destructor(58,&yymsp[0].minor);
}
// 5753 "parser.c"
        break;
      case 278:
// 2216 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_new_instance(0, yymsp[0].minor.yy0, NULL, status->scanner_state);
  yy_destructor(34,&yymsp[-1].minor);
}
// 5761 "parser.c"
        break;
      case 279:
// 2221 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_new_instance(0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(34,&yymsp[-3].minor);
  yy_destructor(55,&yymsp[-1].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5771 "parser.c"
        break;
      case 280:
// 2226 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_new_instance(0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(34,&yymsp[-4].minor);
  yy_destructor(55,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5781 "parser.c"
        break;
      case 281:
// 2231 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_new_instance(1, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(34,&yymsp[-3].minor);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 5791 "parser.c"
        break;
      case 282:
// 2236 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_new_instance(1, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(34,&yymsp[-5].minor);
  yy_destructor(48,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(55,&yymsp[-1].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5803 "parser.c"
        break;
      case 283:
// 2241 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_new_instance(1, yymsp[-4].minor.yy0, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(34,&yymsp[-6].minor);
  yy_destructor(48,&yymsp[-5].minor);
  yy_destructor(49,&yymsp[-3].minor);
  yy_destructor(55,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5815 "parser.c"
        break;
      case 284:
// 2246 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_fcall(1, yymsp[-3].minor.yy0, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(55,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5824 "parser.c"
        break;
      case 285:
// 2251 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_fcall(1, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(55,&yymsp[-1].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5833 "parser.c"
        break;
      case 286:
// 2256 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_fcall(2, yymsp[-4].minor.yy0, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(48,&yymsp[-5].minor);
  yy_destructor(49,&yymsp[-3].minor);
  yy_destructor(55,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5844 "parser.c"
        break;
      case 287:
// 2261 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_fcall(2, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(48,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(55,&yymsp[-1].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5855 "parser.c"
        break;
      case 288:
// 2266 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_scall(0, yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(91,&yymsp[-4].minor);
  yy_destructor(55,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5865 "parser.c"
        break;
      case 289:
// 2271 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_scall(0, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(91,&yymsp[-3].minor);
  yy_destructor(55,&yymsp[-1].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5875 "parser.c"
        break;
      case 290:
// 2276 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_scall(1, yymsp[-6].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(48,&yymsp[-7].minor);
  yy_destructor(49,&yymsp[-5].minor);
  yy_destructor(91,&yymsp[-4].minor);
  yy_destructor(55,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5887 "parser.c"
        break;
      case 291:
// 2281 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_scall(1, yymsp[-5].minor.yy0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(48,&yymsp[-6].minor);
  yy_destructor(49,&yymsp[-4].minor);
  yy_destructor(91,&yymsp[-3].minor);
  yy_destructor(55,&yymsp[-1].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5899 "parser.c"
        break;
      case 292:
// 2286 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_scall(1, yymsp[-8].minor.yy0, yymsp[-4].minor.yy0, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(48,&yymsp[-9].minor);
  yy_destructor(49,&yymsp[-7].minor);
  yy_destructor(91,&yymsp[-6].minor);
  yy_destructor(48,&yymsp[-5].minor);
  yy_destructor(49,&yymsp[-3].minor);
  yy_destructor(55,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5913 "parser.c"
        break;
      case 293:
// 2291 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_scall(1, yymsp[-7].minor.yy0, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(48,&yymsp[-8].minor);
  yy_destructor(49,&yymsp[-6].minor);
  yy_destructor(91,&yymsp[-5].minor);
  yy_destructor(48,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(55,&yymsp[-1].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5927 "parser.c"
        break;
      case 294:
// 2296 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_mcall(1, yymsp[-5].minor.yy259, yymsp[-3].minor.yy0, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(38,&yymsp[-4].minor);
  yy_destructor(55,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5937 "parser.c"
        break;
      case 295:
// 2301 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_mcall(1, yymsp[-4].minor.yy259, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-3].minor);
  yy_destructor(55,&yymsp[-1].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5947 "parser.c"
        break;
      case 296:
// 2306 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_mcall(2, yymsp[-7].minor.yy259, yymsp[-4].minor.yy0, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(38,&yymsp[-6].minor);
  yy_destructor(48,&yymsp[-5].minor);
  yy_destructor(49,&yymsp[-3].minor);
  yy_destructor(55,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5959 "parser.c"
        break;
      case 297:
// 2311 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_mcall(2, yymsp[-6].minor.yy259, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-5].minor);
  yy_destructor(48,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(55,&yymsp[-1].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5971 "parser.c"
        break;
      case 298:
// 2316 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_mcall(3, yymsp[-7].minor.yy259, yymsp[-4].minor.yy0, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(38,&yymsp[-6].minor);
  yy_destructor(48,&yymsp[-5].minor);
  yy_destructor(49,&yymsp[-3].minor);
  yy_destructor(55,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5983 "parser.c"
        break;
      case 299:
// 2321 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_mcall(3, yymsp[-6].minor.yy259, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-5].minor);
  yy_destructor(48,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(55,&yymsp[-1].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5995 "parser.c"
        break;
      case 308:
// 2360 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_array_item(yymsp[-2].minor.yy259, yymsp[0].minor.yy259, status->scanner_state);
  yy_destructor(76,&yymsp[-1].minor);
}
// 6003 "parser.c"
        break;
      case 309:
// 2364 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_array_item(NULL, yymsp[0].minor.yy259, status->scanner_state);
}
// 6010 "parser.c"
        break;
      case 323:
// 2421 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), yymsp[-1].minor.yy259, NULL, status->scanner_state);
  yy_destructor(37,&yymsp[-2].minor);
  yy_destructor(58,&yymsp[0].minor);
}
// 6019 "parser.c"
        break;
      case 324:
// 2426 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("property-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-1].minor);
}
// 6027 "parser.c"
        break;
      case 325:
// 2431 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("property-dynamic-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-3].minor);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 6037 "parser.c"
        break;
      case 326:
// 2436 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("property-string-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_STRING, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-3].minor);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 6047 "parser.c"
        break;
      case 328:
// 2444 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_comment(yymsp[0].minor.yy0, status->scanner_state);
}
// 6054 "parser.c"
        break;
      case 329:
// 2448 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_cblock(yymsp[0].minor.yy0, status->scanner_state);
}
// 6061 "parser.c"
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
// 981 "parser.lemon"


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

// 6137 "parser.c"
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
			case XX_T_CBLOCK:
				xx_parse_with_token(xx_parser, XX_T_CBLOCK, XX_CBLOCK, &token, parser_status);
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
