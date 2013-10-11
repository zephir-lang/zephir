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
#define YYNSTATE 685
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
 /*     0 */   215,  224,  227,  218,  221,  242,  244,  252,  246,  248,
 /*    10 */   250,  429,  266,   33,   34,   36,   37,  239,  235,    3,
 /*    20 */     4,    5,    6,    7,  211,  274,  279,  293,  208,    9,
 /*    30 */   289,   63,  132,  270,  167,  134,  653,  659,  658,  642,
 /*    40 */    22,  300,  388,  523,  652,  608,  288,   21,  260,  299,
 /*    50 */   119,  119,  486,  495,  504,  507,  498,  501,  510,  516,
 /*    60 */   513,  594,  519,  168,  170,  172,  654,  181,  657,  658,
 /*    70 */   642,  192,  196,  201,  382,  607,  608,  389,  685,  298,
 /*    80 */   671,  119,  674,  282,  303,  130,   40,  455,  470,  474,
 /*    90 */   483,  281,  283,  284,  285,  286,  287,  215,  224,  227,
 /*   100 */   218,  221,  600,   41,   31,   55,   34,   36,   37,  266,
 /*   110 */  1013,    1,    2,  684,    4,    5,    6,    7,  184,  571,
 /*   120 */   311,  211,  274,  279,  293,  208,  137,  289,   10,  567,
 /*   130 */   270,  167,  136,  605,  135,  620,  642,  119,  300,  542,
 /*   140 */   523,  652,  608,  288,  188,  260,  548,  119,  636,  486,
 /*   150 */   495,  504,  507,  498,  501,  510,  516,  513,  637,  519,
 /*   160 */   168,  170,  172,  636,  181,  432,  395,   12,  192,  196,
 /*   170 */   201,  382,  631,  637,  389,   79,   87,   13,   64,  395,
 /*   180 */   282,  239,  235,   15,  455,  470,  474,  483,  281,  283,
 /*   190 */   284,  285,  286,  287,  215,  224,  227,  218,  221,  460,
 /*   200 */   396,  397,  398,  399,  400,  420,  266,  456,  461,  307,
 /*   210 */   309,  308,  278,  396,  397,  398,  399,  400,  211,  274,
 /*   220 */   279,  293,  208,  627,  289,  214,  373,  270,  167,   19,
 /*   230 */   630,  606,  578,  137,   22,  300,  145,  523,  607,  608,
 /*   240 */   288,  120,  260,  119,  119,  119,  486,  495,  504,  507,
 /*   250 */   498,  501,  510,  516,  513,  424,  519,  168,  170,  172,
 /*   260 */   450,  181,  655,  620,  642,  192,  196,  201,  382,  644,
 /*   270 */   608,  389,  294,   14,  395,  119,  630,  282,   17,  645,
 /*   280 */   325,  455,  470,  474,  483,  281,  283,  284,  285,  286,
 /*   290 */   287,  215,  224,  227,  218,  221,  588,  587,   31,   55,
 /*   300 */    34,   36,   37,  266,  246,  248,  250,  583,  396,  397,
 /*   310 */   398,  399,  400,  239,  235,  211,  274,  279,  293,  208,
 /*   320 */   593,  289,   17,  236,  270,  167,   79,   87,  624,   93,
 /*   330 */   622,  354,  300,  586,  523,  630,   63,  288,  625,  260,
 /*   340 */   292,  119,  599,  486,  495,  504,  507,  498,  501,  510,
 /*   350 */   516,  513,  595,  519,  168,  170,  172,  686,  181,   16,
 /*   360 */   460,  993,  192,  196,  201,  382,  114,  186,  389,  459,
 /*   370 */   307,  309,  308,  278,  282,  665,  271,  663,  455,  470,
 /*   380 */   474,  483,  281,  283,  284,  285,  286,  287,  215,  224,
 /*   390 */   227,  218,  221,  188,  634,  548,    8,  660,  369,   11,
 /*   400 */   266,  121,  401,  667,  677,  446,  451,  523,  307,  309,
 /*   410 */   308,  278,  211,  274,  279,  293,  208,  113,  289,  390,
 /*   420 */   454,  270,  167,   79,   87,  559,   93,  632,  639,  300,
 /*   430 */   552,  523,  479,  476,  288,  142,  260,  633,  392,  130,
 /*   440 */   486,  495,  504,  507,  498,  501,  510,  516,  513,   94,
 /*   450 */   519,  168,  170,  172,  544,  181,  128,  381,  663,  192,
 /*   460 */   196,  201,  382,  263,  458,  389,  405,  307,  309,  308,
 /*   470 */   278,  282,  292,  391,  419,  455,  470,  474,  483,  281,
 /*   480 */   283,  284,  285,  286,  287,  215,  224,  227,  218,  221,
 /*   490 */   261,  124,  127,  664,  447,  130,  130,  266,  449,  457,
 /*   500 */   307,  309,  308,  278,  115,  376,  299,  118,  271,  211,
 /*   510 */   274,  279,  293,  208,   57,  289,  374,  272,  270,  167,
 /*   520 */    79,   87,  109,   93,  375,  112,  300,  176,  523,  530,
 /*   530 */   350,  288,   24,  260,  489,  104,  365,  486,  495,  504,
 /*   540 */   507,  498,  501,  510,  516,  513,  119,  519,  168,  170,
 /*   550 */   172,  173,  181,  489,  381,  378,  192,  196,  201,  382,
 /*   560 */   487,  494,  389,   25,  307,  309,  308,  278,  282,  488,
 /*   570 */   496,  494,  455,  470,  474,  483,  281,  283,  284,  285,
 /*   580 */   286,  287,  215,  224,  227,  218,  221,  572,  497,   31,
 /*   590 */    55,   34,   36,   37,  266,   60,  299,   31,   55,   34,
 /*   600 */    36,   37,  499,  494,   29,  384,  211,  274,  279,  293,
 /*   610 */   208,  565,  289,  502,  494,  270,  167,   79,   87,  675,
 /*   620 */    93,  663,  489,  300,  602,  523,  359,   35,  288,   27,
 /*   630 */   260,  489,   56,   39,  486,  495,  504,  507,  498,  501,
 /*   640 */   510,  516,  513,  355,  519,  168,  170,  172,  182,  181,
 /*   650 */   340,  381,  312,  192,  196,  201,  382,  500,   32,  389,
 /*   660 */   317,  307,  309,  308,  278,  282,  503,  505,  494,  455,
 /*   670 */   470,  474,  483,  281,  283,  284,  285,  286,  287,  215,
 /*   680 */   224,  227,  218,  221,  560,  299,   31,   55,   34,   36,
 /*   690 */    37,  266,  110,  361,   31,   55,   34,   36,   37,  299,
 /*   700 */   508,  494,  339,  211,  274,  279,  293,  208,   42,  289,
 /*   710 */   299,  647,  270,  167,  489,  352,  511,  494,  630,  489,
 /*   720 */   300,  576,  523,  514,  494,  288,   43,  260,  489,  348,
 /*   730 */   489,  486,  495,  504,  507,  498,  501,  510,  516,  513,
 /*   740 */   333,  519,  168,  170,  172,  197,  181,   66,  381,  506,
 /*   750 */   192,  196,  201,  382,  509,  489,  389,   61,  307,  309,
 /*   760 */   308,  278,  282,  512,   67,  515,  455,  470,  474,  483,
 /*   770 */   281,  283,  284,  285,  286,  287,  215,  224,  227,  218,
 /*   780 */   221,  116,  299,   31,   55,   34,   36,   37,  266,   28,
 /*   790 */   518,   31,   55,   34,   36,   37,  517,  494,  520,  494,
 /*   800 */   211,  274,  279,  293,  208,   59,  289,  299,   62,  270,
 /*   810 */   167,  489,  331,  185,  551,   83,   76,  300,  604,  523,
 /*   820 */    80,  299,  288,   77,  260,   81,   82,   84,  486,  495,
 /*   830 */   504,  507,  498,  501,  510,  516,  513,  322,  519,  168,
 /*   840 */   170,  172,  206,  181,   85,  381,  521,  192,  196,  201,
 /*   850 */   382,  315,  299,  389,   86,  307,  309,  308,  278,  282,
 /*   860 */    88,   89,   63,  455,  470,  474,  483,  281,  283,  284,
 /*   870 */   285,  286,  287,  215,  224,  227,  218,  221,  416,  681,
 /*   880 */    90,   63,  305,  444,  130,  266,  107,   92,  307,  309,
 /*   890 */   308,  278,  566,   40,  106,  123,  111,  211,  274,  279,
 /*   900 */   293,  208,  117,  289,  122,  125,  270,  167,  126,  662,
 /*   910 */   129,   58,  133,  140,  300,  180,  523,  139,  174,  288,
 /*   920 */   169,  260,  171,  177,  178,  486,  495,  504,  507,  498,
 /*   930 */   501,  510,  516,  513,  183,  519,  168,  170,  172,  193,
 /*   940 */   181,  480,  187,  189,  192,  196,  201,  382,  190,  198,
 /*   950 */   389,  307,  309,  308,  278,  202,  282,  205,  207,  213,
 /*   960 */   455,  470,  474,  483,  281,  283,  284,  285,  286,  287,
 /*   970 */   215,  224,  227,  218,  221,  403,  237,  212,  264,  448,
 /*   980 */   267,  268,  266,  273,  275,  307,  309,  308,  278,  276,
 /*   990 */   295,  301,  277,  313,  211,  274,  279,  293,  208,  302,
 /*  1000 */   289,  319,  318,  270,  167,  326,  327,  995,  320,  328,
 /*  1010 */   994,  300,  200,  523,  357,  343,  288,  356,  260,  362,
 /*  1020 */   383,  363,  486,  495,  504,  507,  498,  501,  510,  516,
 /*  1030 */   513,  377,  519,  168,  170,  172,  379,  181,  247,  394,
 /*  1040 */   421,  192,  196,  201,  382,  425,  393,  389,  307,  309,
 /*  1050 */   308,  278,  406,  282,  410,  413,  414,  455,  470,  474,
 /*  1060 */   483,  281,  283,  284,  285,  286,  287,  215,  224,  227,
 /*  1070 */   218,  221,  403,  422,  437,  445,  443,  426,  430,  266,
 /*  1080 */   434,  438,  307,  309,  308,  278,  463,  442,  465,  467,
 /*  1090 */   469,  211,  274,  279,  293,  208,  475,  289,  477,  491,
 /*  1100 */   270,  167,  478,  482,  490,  492,  531,  532,  300,  536,
 /*  1110 */   523,  543,  493,  288,  545,  260,  549,  554,  555,  486,
 /*  1120 */   495,  504,  507,  498,  501,  510,  516,  513,  561,  519,
 /*  1130 */   168,  170,  172,  573,  181,  209,  580,  581,  192,  196,
 /*  1140 */   201,  382,  589,  601,  389,  307,  309,  308,  278,  614,
 /*  1150 */   282,  611,  612,  613,  455,  470,  474,  483,  281,  283,
 /*  1160 */   284,  285,  286,  287,  215,  224,  227,  218,  221,  403,
 /*  1170 */   616,  617,  619,  439,  618,  626,  266,  640,  628,  307,
 /*  1180 */   309,  308,  278,  629,  731,  732,  638,  646,  211,  274,
 /*  1190 */   279,  293,  208,  641,  289,  649,  635,  270,  167,  650,
 /*  1200 */   648,  651,  666,  661,  670,  300,  574,  523,  668,  669,
 /*  1210 */   288,  672,  260,  673,  130,  679,  486,  495,  504,  507,
 /*  1220 */   498,  501,  510,  516,  513,  676,  519,  168,  170,  172,
 /*  1230 */   678,  181,  245,  680,  682,  192,  196,  201,  382,  683,
 /*  1240 */   516,  389,  307,  309,  308,  278,  516,  282,  516,  516,
 /*  1250 */   516,  455,  470,  474,  483,  281,  283,  284,  285,  286,
 /*  1260 */   287,  215,  224,  227,  218,  221,  403,  516,  516,  516,
 /*  1270 */   435,  516,  516,  266,  516,  516,  307,  309,  308,  278,
 /*  1280 */   516,  516,  516,  516,  516,  211,  274,  279,  293,  208,
 /*  1290 */   516,  289,  516,  516,  270,  167,  516,  516,  516,  516,
 /*  1300 */   516,  516,  300,  204,  523,  516,  516,  288,  516,  260,
 /*  1310 */   516,  516,  516,  486,  495,  504,  507,  498,  501,  510,
 /*  1320 */   516,  513,  516,  519,  168,  170,  172,  516,  181,  380,
 /*  1330 */   516,  516,  192,  196,  201,  382,  516,  516,  389,  307,
 /*  1340 */   309,  308,  278,  516,  282,  516,  516,  516,  455,  470,
 /*  1350 */   474,  483,  281,  283,  284,  285,  286,  287,  215,  224,
 /*  1360 */   227,  218,  221,  416,  516,  516,  516,  516,  436,  516,
 /*  1370 */   266,  516,  516,  307,  309,  308,  278,  516,  516,  516,
 /*  1380 */   516,  516,  211,  274,  279,  293,  208,  516,  289,  516,
 /*  1390 */   516,  270,  167,  516,  516,  516,  516,  516,  516,  300,
 /*  1400 */   553,  523,  516,  516,  288,  516,  260,  516,  516,  516,
 /*  1410 */   486,  495,  504,  507,  498,  501,  510,  516,  513,  516,
 /*  1420 */   519,  168,  170,  172,  516,  181,  243,  516,  516,  192,
 /*  1430 */   196,  201,  382,  516,  516,  389,  307,  309,  308,  278,
 /*  1440 */   516,  282,  516,  516,  516,  455,  470,  474,  483,  281,
 /*  1450 */   283,  284,  285,  286,  287,  215,  224,  227,  218,  221,
 /*  1460 */   403,  516,  516,  516,  431,  516,  516,  266,  516,  516,
 /*  1470 */   307,  309,  308,  278,  516,  516,  516,  516,  516,  211,
 /*  1480 */   274,  279,  293,  208,  516,  289,  516,  516,  270,  167,
 /*  1490 */   516,  516,  516,  516,  516,  516,  300,  143,  523,  516,
 /*  1500 */   516,  288,  516,  260,  516,  516,  516,  486,  495,  504,
 /*  1510 */   507,  498,  501,  510,  516,  513,  516,  519,  168,  170,
 /*  1520 */   172,  516,  181,  240,  516,  516,  192,  196,  201,  382,
 /*  1530 */   516,  516,  389,  307,  309,  308,  278,  516,  282,  516,
 /*  1540 */   516,  516,  455,  470,  474,  483,  281,  283,  284,  285,
 /*  1550 */   286,  287,  215,  224,  227,  218,  221,  516,    8,  516,
 /*  1560 */   516,   11,  516,  121,  266,  667,  677,  516,  516,  523,
 /*  1570 */   516,  516,  516,  516,  516,  516,  211,  274,  279,  293,
 /*  1580 */   208,  516,  289,  516,  516,  270,  167,  246,  248,  250,
 /*  1590 */   516,  516,  516,  300,  529,  523,  239,  235,  288,  516,
 /*  1600 */   260,  516,  516,  516,  486,  495,  504,  507,  498,  501,
 /*  1610 */   510,  516,  513,  516,  519,  168,  170,  172,  516,  181,
 /*  1620 */   232,  516,  516,  192,  196,  201,  382,  516,  516,  389,
 /*  1630 */   307,  309,  308,  278,  516,  282,  516,  516,  516,  455,
 /*  1640 */   470,  474,  483,  281,  283,  284,  285,  286,  287,  215,
 /*  1650 */   224,  227,  218,  221,  403,  516,  516,  516,  427,  516,
 /*  1660 */   516,  266,  516,  516,  307,  309,  308,  278,  516,  516,
 /*  1670 */   516,  516,  516,  211,  274,  279,  293,  208,  516,  289,
 /*  1680 */   516,  516,  270,  167,  516,  516,  516,  516,  516,  516,
 /*  1690 */   300,  584,  523,  516,  516,  288,  516,  260,  516,  516,
 /*  1700 */   516,  486,  495,  504,  507,  498,  501,  510,  516,  513,
 /*  1710 */   516,  519,  168,  170,  172,  516,  181,  234,  516,  516,
 /*  1720 */   192,  196,  201,  382,  516,  516,  389,  307,  309,  308,
 /*  1730 */   278,  516,  282,  516,  516,  516,  455,  470,  474,  483,
 /*  1740 */   281,  283,  284,  285,  286,  287,  215,  224,  227,  218,
 /*  1750 */   221,  403,  516,  516,  516,  423,  516,  516,  266,  516,
 /*  1760 */   516,  307,  309,  308,  278,  516,  516,  516,  516,  516,
 /*  1770 */   211,  274,  279,  293,  208,  516,  289,  516,  516,  270,
 /*  1780 */   167,  516,  516,  516,  516,  516,  516,  300,  590,  523,
 /*  1790 */   516,  516,  288,  516,  260,  516,  516,  516,  486,  495,
 /*  1800 */   504,  507,  498,  501,  510,  516,  513,  516,  519,  168,
 /*  1810 */   170,  172,  516,  181,  265,  516,  516,  192,  196,  201,
 /*  1820 */   382,  516,  516,  389,  307,  309,  308,  278,  516,  282,
 /*  1830 */   516,  516,  516,  455,  470,  474,  483,  281,  283,  284,
 /*  1840 */   285,  286,  287,  215,  224,  227,  218,  221,  403,  516,
 /*  1850 */   516,  516,  415,  516,  516,  266,  516,  516,  307,  309,
 /*  1860 */   308,  278,  516,  516,  516,  516,  516,  211,  274,  279,
 /*  1870 */   293,  208,  516,  289,  516,  516,  270,  167,  516,  516,
 /*  1880 */   516,  516,  516,  516,  300,  570,  523,  516,  516,  288,
 /*  1890 */   516,  260,  516,  516,  516,  486,  495,  504,  507,  498,
 /*  1900 */   501,  510,  516,  513,  516,  519,  168,  170,  172,  516,
 /*  1910 */   181,  371,  516,  516,  192,  196,  201,  382,  516,  516,
 /*  1920 */   389,  307,  309,  308,  278,  516,  282,  516,  516,  516,
 /*  1930 */   455,  470,  474,  483,  281,  283,  284,  285,  286,  287,
 /*  1940 */   215,  224,  227,  218,  221,  403,  516,  516,  516,  411,
 /*  1950 */   516,  516,  266,  516,  516,  307,  309,  308,  278,  516,
 /*  1960 */   516,  516,  516,  516,  211,  274,  279,  293,  208,  516,
 /*  1970 */   289,  516,  516,  270,  167,  516,  516,  516,  516,  516,
 /*  1980 */   516,  300,  195,  523,  516,  516,  288,  516,  260,  516,
 /*  1990 */   516,  516,  486,  495,  504,  507,  498,  501,  510,  516,
 /*  2000 */   513,  516,  519,  168,  170,  172,  516,  181,  230,  516,
 /*  2010 */   516,  192,  196,  201,  382,  516,  516,  389,  307,  309,
 /*  2020 */   308,  278,  516,  282,  516,  516,  516,  455,  470,  474,
 /*  2030 */   483,  281,  283,  284,  285,  286,  287,  215,  224,  227,
 /*  2040 */   218,  221,  416,  516,  516,  516,  516,  412,  516,  266,
 /*  2050 */   516,  516,  307,  309,  308,  278,  516,  516,  516,  516,
 /*  2060 */   516,  211,  274,  279,  293,  208,  516,  289,  516,  516,
 /*  2070 */   270,  167,  516,  516,  516,  516,  516,  516,  300,  568,
 /*  2080 */   523,  516,  516,  288,  516,  260,  516,  516,  516,  486,
 /*  2090 */   495,  504,  507,  498,  501,  510,  516,  513,  516,  519,
 /*  2100 */   168,  170,  172,  516,  181,  216,  516,  516,  192,  196,
 /*  2110 */   201,  382,  516,  516,  389,  307,  309,  308,  278,  516,
 /*  2120 */   282,  516,  516,  516,  455,  470,  474,  483,  281,  283,
 /*  2130 */   284,  285,  286,  287,  215,  224,  227,  218,  221,  403,
 /*  2140 */   516,  516,  516,  407,  516,  516,  266,  516,  516,  307,
 /*  2150 */   309,  308,  278,  516,  516,  516,  516,  516,  211,  274,
 /*  2160 */   279,  293,  208,  516,  289,  516,  516,  270,  167,  516,
 /*  2170 */   516,  516,  516,  516,  516,  300,  558,  523,  516,  516,
 /*  2180 */   288,  516,  260,  516,  516,  516,  486,  495,  504,  507,
 /*  2190 */   498,  501,  510,  516,  513,  516,  519,  168,  170,  172,
 /*  2200 */   516,  181,  533,  516,  516,  192,  196,  201,  382,  516,
 /*  2210 */   516,  389,  307,  309,  308,  278,  516,  282,  516,  516,
 /*  2220 */   516,  455,  470,  474,  483,  281,  283,  284,  285,  286,
 /*  2230 */   287,  215,  224,  227,  218,  221,  403,  516,  516,  516,
 /*  2240 */   402,  516,  516,  266,  516,  516,  307,  309,  308,  278,
 /*  2250 */   516,  516,  516,  516,  516,  211,  274,  279,  293,  208,
 /*  2260 */   516,  289,  516,  516,  270,  167,  516,  516,  516,  516,
 /*  2270 */   516,  516,  300,  547,  523,  516,  516,  288,  516,  260,
 /*  2280 */   516,  516,  516,  486,  495,  504,  507,  498,  501,  510,
 /*  2290 */   516,  513,  516,  519,  168,  170,  172,  516,  181,  228,
 /*  2300 */   516,  516,  192,  196,  201,  382,  516,  516,  389,  307,
 /*  2310 */   309,  308,  278,  516,  282,  516,  516,  516,  455,  470,
 /*  2320 */   474,  483,  281,  283,  284,  285,  286,  287,  215,  224,
 /*  2330 */   227,  218,  221,  538,  516,  516,  516,  516,  516,  516,
 /*  2340 */   266,  516,  516,  307,  309,  308,  278,  516,  516,  516,
 /*  2350 */   516,  516,  211,  274,  279,  293,  208,  516,  289,  516,
 /*  2360 */   516,  270,  167,  516,  516,  516,  516,  516,  516,  300,
 /*  2370 */   592,  523,  516,  516,  288,  516,  260,  516,  516,  516,
 /*  2380 */   486,  495,  504,  507,  498,  501,  510,  516,  513,  516,
 /*  2390 */   519,  168,  170,  172,  516,  181,  280,  516,  516,  192,
 /*  2400 */   196,  201,  382,  516,  516,  389,  307,  309,  308,  278,
 /*  2410 */   516,  282,  516,  516,  516,  455,  470,  474,  483,  281,
 /*  2420 */   283,  284,  285,  286,  287,  215,  224,  227,  218,  221,
 /*  2430 */   269,  516,  516,  516,  516,  516,  516,  266,  516,  516,
 /*  2440 */   307,  309,  308,  278,  516,  516,  516,  516,  516,  211,
 /*  2450 */   274,  279,  293,  208,  516,  289,  516,  516,  270,  167,
 /*  2460 */   516,  516,  516,  516,  516,  516,  300,  541,  523,  516,
 /*  2470 */   516,  288,  516,  260,  516,  516,  516,  486,  495,  504,
 /*  2480 */   507,  498,  501,  510,  516,  513,  516,  519,  168,  170,
 /*  2490 */   172,  516,  181,  370,  516,  516,  192,  196,  201,  382,
 /*  2500 */   516,  516,  389,  307,  309,  308,  278,  516,  282,  516,
 /*  2510 */   516,  516,  455,  470,  474,  483,  281,  283,  284,  285,
 /*  2520 */   286,  287,  215,  224,  227,  218,  221,  484,  516,  516,
 /*  2530 */   516,  516,  516,  516,  266,  516,  516,  307,  309,  308,
 /*  2540 */   278,  516,  516,  516,  516,  516,  211,  274,  279,  293,
 /*  2550 */   208,  516,  289,  516,  516,  270,  167,  516,  516,  516,
 /*  2560 */   516,  516,  516,  300,  596,  523,  516,  516,  288,  516,
 /*  2570 */   260,  516,  516,  516,  486,  495,  504,  507,  498,  501,
 /*  2580 */   510,  516,  513,  516,  519,  168,  170,  172,  516,  181,
 /*  2590 */   471,  516,  516,  192,  196,  201,  382,  516,  516,  389,
 /*  2600 */   307,  309,  308,  278,  516,  282,  516,  516,  516,  455,
 /*  2610 */   470,  474,  483,  281,  283,  284,  285,  286,  287,  215,
 /*  2620 */   224,  227,  218,  221,  368,  516,  516,  516,  516,  516,
 /*  2630 */   516,  266,  516,  516,  307,  309,  308,  278,  516,  516,
 /*  2640 */   516,  516,  516,  211,  274,  279,  293,  208,  516,  289,
 /*  2650 */   516,  516,  270,  167,  516,  516,  516,  516,  516,  516,
 /*  2660 */   300,  598,  523,  516,  516,  288,  516,  260,  516,  516,
 /*  2670 */   516,  486,  495,  504,  507,  498,  501,  510,  516,  513,
 /*  2680 */   516,  519,  168,  170,  172,  516,  181,  367,  516,  516,
 /*  2690 */   192,  196,  201,  382,  516,  516,  389,  307,  309,  308,
 /*  2700 */   278,  516,  282,  516,  516,  516,  455,  470,  474,  483,
 /*  2710 */   281,  283,  284,  285,  286,  287,  215,  224,  227,  218,
 /*  2720 */   221,  222,  516,  516,  516,  516,  516,  516,  266,  516,
 /*  2730 */   516,  307,  309,  308,  278,  516,  516,  516,  516,  516,
 /*  2740 */   211,  274,  279,  293,  208,  516,  289,  516,  516,  270,
 /*  2750 */   167,  516,  516,  516,  516,  516,  516,  300,  564,  523,
 /*  2760 */   516,  516,  288,  516,  260,  516,  516,  516,  486,  495,
 /*  2770 */   504,  507,  498,  501,  510,  516,  513,  516,  519,  168,
 /*  2780 */   170,  172,  516,  181,  385,  516,  516,  192,  196,  201,
 /*  2790 */   382,  516,  516,  389,  307,  309,  308,  278,  516,  282,
 /*  2800 */   516,  516,  516,  455,  470,  474,  483,  281,  283,  284,
 /*  2810 */   285,  286,  287,  215,  224,  227,  218,  221,  255,  516,
 /*  2820 */   516,  516,  516,  516,  516,  266,  516,  516,  307,  309,
 /*  2830 */   308,  278,  516,  516,  516,  516,  516,  211,  274,  279,
 /*  2840 */   293,  208,  516,  289,  516,  516,  270,  167,  516,  516,
 /*  2850 */   516,  516,  516,  516,  300,  546,  523,  516,  516,  288,
 /*  2860 */   516,  260,  516,  516,  516,  486,  495,  504,  507,  498,
 /*  2870 */   501,  510,  516,  513,  516,  519,  168,  170,  172,  516,
 /*  2880 */   181,  257,  516,  516,  192,  196,  201,  382,  516,  516,
 /*  2890 */   389,  307,  309,  308,  278,  516,  282,  516,  516,  516,
 /*  2900 */   455,  470,  474,  483,  281,  283,  284,  285,  286,  287,
 /*  2910 */   215,  224,  227,  218,  221,  253,  516,  516,  516,  516,
 /*  2920 */   516,  516,  266,  516,  516,  307,  309,  308,  278,  516,
 /*  2930 */   516,  516,  516,  516,  211,  274,  279,  293,  208,  516,
 /*  2940 */   289,  516,  516,  270,  167,  516,  516,  516,  516,  516,
 /*  2950 */   516,  300,  562,  523,  516,  516,  288,  516,  260,  516,
 /*  2960 */   516,  516,  486,  495,  504,  507,  498,  501,  510,  516,
 /*  2970 */   513,  516,  519,  168,  170,  172,  516,  181,  251,  516,
 /*  2980 */   516,  192,  196,  201,  382,  516,  516,  389,  307,  309,
 /*  2990 */   308,  278,  516,  282,  516,  516,  516,  455,  470,  474,
 /*  3000 */   483,  281,  283,  284,  285,  286,  287,  215,  224,  227,
 /*  3010 */   218,  221,  526,  516,  516,  516,  516,  516,  516,  266,
 /*  3020 */   516,  516,  307,  309,  308,  278,  516,  516,  516,  516,
 /*  3030 */   516,  211,  274,  279,  293,  208,  516,  289,  516,  516,
 /*  3040 */   270,  167,  516,  516,  516,  516,  516,  516,  300,  557,
 /*  3050 */   523,  516,  516,  288,  516,  260,  516,  516,  516,  486,
 /*  3060 */   495,  504,  507,  498,  501,  510,  516,  513,  516,  519,
 /*  3070 */   168,  170,  172,  516,  181,  219,  516,  516,  192,  196,
 /*  3080 */   201,  382,  516,  516,  389,  307,  309,  308,  278,  516,
 /*  3090 */   282,  516,  516,  516,  455,  470,  474,  483,  281,  283,
 /*  3100 */   284,  285,  286,  287,  215,  224,  227,  218,  221,  249,
 /*  3110 */   516,  516,  516,  516,  516,  516,  266,  516,  516,  307,
 /*  3120 */   309,  308,  278,  516,  516,  516,  516,  516,  211,  274,
 /*  3130 */   279,  293,  208,  516,  289,  516,  516,  270,  167,  516,
 /*  3140 */   516,  516,  516,  516,  516,  300,  516,  523,  516,  516,
 /*  3150 */   288,  516,  260,  516,  516,  516,  486,  495,  504,  507,
 /*  3160 */   498,  501,  510,  516,  513,  516,  519,  168,  170,  172,
 /*  3170 */   516,  181,  225,  516,  516,  192,  196,  201,  382,  516,
 /*  3180 */   516,  389,  307,  309,  308,  278,  516,  282,  516,  516,
 /*  3190 */   516,  455,  470,  474,  483,  281,  283,  284,  285,  286,
 /*  3200 */   287,  215,  224,  227,  218,  221,  259,  516,  516,  516,
 /*  3210 */   516,  516,  516,  266,  516,  516,  307,  309,  308,  278,
 /*  3220 */   516,  516,  516,  516,  516,  211,  274,  279,  293,  208,
 /*  3230 */   516,  289,  516,   38,  270,  516,  516,  516,  516,  516,
 /*  3240 */   516,  516,  300,  516,  516,  516,  516,  288,  516,  260,
 /*  3250 */   516,  516,  516,   44,   45,   46,   47,   48,   49,   50,
 /*  3260 */    51,   52,   53,   54,  516,  516,  516,  516,  516,  165,
 /*  3270 */   516,   30,  516,   44,   45,   46,   47,   48,   49,   50,
 /*  3280 */    51,   52,   53,   54,  282,  516,  516,  516,  516,  563,
 /*  3290 */   516,  516,  281,  283,  284,  285,  286,  287,  524,  147,
 /*  3300 */   148,  149,  150,  151,  152,  153,  154,  155,  156,  157,
 /*  3310 */   158,  159,  160,  161,  162,  163,  164,  166,  165,  516,
 /*  3320 */   516,  522,  516,   95,   96,   98,   97,   99,  516,  516,
 /*  3330 */   516,  462,  464,  466,  468,  516,  516,  516,  575,  516,
 /*  3340 */    95,   96,   98,   97,   99,  516,  516,  524,  147,  148,
 /*  3350 */   149,  150,  151,  152,  153,  154,  155,  156,  157,  158,
 /*  3360 */   159,  160,  161,  162,  163,  164,  166,  165,  101,  102,
 /*  3370 */   522,  713,  621,  516,  615,  516,  516,  516,  100,  516,
 /*  3380 */   462,  464,  466,  468,  516,  101,  102,  556,  131,  621,
 /*  3390 */   516,  615,  516,  516,  516,  100,  524,  147,  148,  149,
 /*  3400 */   150,  151,  152,  153,  154,  155,  156,  157,  158,  159,
 /*  3410 */   160,  161,  162,  163,  164,  166,  165,  516,  516,  522,
 /*  3420 */   516,  440,  404,  516,  516,  516,  516,  516,  516,  462,
 /*  3430 */   464,  466,  468,  516,  516,  395,  569,  516,   95,   96,
 /*  3440 */    98,   97,   99,  516,  516,  524,  147,  148,  149,  150,
 /*  3450 */   151,  152,  153,  154,  155,  156,  157,  158,  159,  160,
 /*  3460 */   161,  162,  163,  164,  166,  165,  516,  516,  522,  396,
 /*  3470 */   397,  398,  399,  400,  516,  428,  452,  453,  462,  464,
 /*  3480 */   466,  468,  516,  101,  102,  194,  716,  609,  516,  615,
 /*  3490 */   516,  516,  516,  100,  524,  147,  148,  149,  150,  151,
 /*  3500 */   152,  153,  154,  155,  156,  157,  158,  159,  160,  161,
 /*  3510 */   162,  163,  164,  166,  165,  516,  516,  522,  516,   95,
 /*  3520 */    96,   98,   97,   99,  516,  516,  516,  462,  464,  466,
 /*  3530 */   468,  516,  516,  516,  191,  516,   95,   96,   98,   97,
 /*  3540 */    99,  516,  516,  524,  147,  148,  149,  150,  151,  152,
 /*  3550 */   153,  154,  155,  156,  157,  158,  159,  160,  161,  162,
 /*  3560 */   163,  164,  166,  165,  101,  102,  522,  712,  621,  516,
 /*  3570 */   615,  516,  516,  516,  100,  516,  462,  464,  466,  468,
 /*  3580 */   516,  101,  102,  179,  719,  577,  516,  516,  516,  516,
 /*  3590 */   516,  100,  524,  147,  148,  149,  150,  151,  152,  153,
 /*  3600 */   154,  155,  156,  157,  158,  159,  160,  161,  162,  163,
 /*  3610 */   164,  166,  165,  516,  516,  522,  516,   95,   96,   98,
 /*  3620 */    97,   99,  516,  516,  516,  462,  464,  466,  468,  516,
 /*  3630 */   516,  516,  387,  516,   95,   96,   98,   97,   99,  516,
 /*  3640 */   516,  524,  147,  148,  149,  150,  151,  152,  153,  154,
 /*  3650 */   155,  156,  157,  158,  159,  160,  161,  162,  163,  164,
 /*  3660 */   166,  165,  101,  102,  522,  720,  577,  516,  516,  516,
 /*  3670 */   516,  516,  100,  516,  462,  464,  466,  468,  516,  101,
 /*  3680 */   102,  144,   18,  103,  516,  516,  516,  516,  516,  100,
 /*  3690 */   524,  147,  148,  149,  150,  151,  152,  153,  154,  155,
 /*  3700 */   156,  157,  158,  159,  160,  161,  162,  163,  164,  166,
 /*  3710 */   165,  516,  516,  522,  516,   95,   96,   98,   97,   99,
 /*  3720 */   516,  516,  516,  462,  464,  466,  468,  516,  516,  516,
 /*  3730 */   528,  516,   95,   96,   98,   97,   99,  516,  516,  524,
 /*  3740 */   147,  148,  149,  150,  151,  152,  153,  154,  155,  156,
 /*  3750 */   157,  158,  159,  160,  161,  162,  163,  164,  166,  165,
 /*  3760 */   101,  102,  522,   20,  103,  516,  516,  516,  516,  516,
 /*  3770 */   100,  516,  462,  464,  466,  468,  516,  101,  102,  203,
 /*  3780 */   714,  577,  516,  516,  516,  516,  516,  100,  524,  147,
 /*  3790 */   148,  149,  150,  151,  152,  153,  154,  155,  156,  157,
 /*  3800 */   158,  159,  160,  161,  162,  163,  164,  166,  165,  516,
 /*  3810 */   516,  522,  516,   95,   96,   98,   97,   99,  516,  516,
 /*  3820 */   516,  462,  464,  466,  468,  516,  516,  516,  175,  516,
 /*  3830 */    95,   96,   98,   97,   99,  516,  516,  524,  147,  148,
 /*  3840 */   149,  150,  151,  152,  153,  154,  155,  156,  157,  158,
 /*  3850 */   159,  160,  161,  162,  163,  164,  166,  165,  101,  102,
 /*  3860 */   522,  718,  577,  516,  516,  516,  516,  516,  100,  643,
 /*  3870 */   462,  464,  466,  468,  516,  101,  102,  535,   95,   96,
 /*  3880 */    98,   97,   99,  138,  516,  100,  524,  147,  148,  149,
 /*  3890 */   150,  151,  152,  153,  154,  155,  156,  157,  158,  159,
 /*  3900 */   160,  161,  162,  163,  164,  166,  165,  516,  516,  522,
 /*  3910 */   516,   95,   96,   98,   97,   99,  516,  623,  516,  462,
 /*  3920 */   464,  466,  468,  101,  102,  516,  603,  516,  516,  516,
 /*  3930 */   516,  579,  516,  100,  516,  524,  147,  148,  149,  150,
 /*  3940 */   151,  152,  153,  154,  155,  156,  157,  158,  159,  160,
 /*  3950 */   161,  162,  163,  164,  166,  165,  101,  102,  522,  715,
 /*  3960 */   577,  516,  516,  516,  516,  516,  100,  516,  462,  464,
 /*  3970 */   466,  468,  516,  516,  516,  199,  516,   95,   96,   98,
 /*  3980 */    97,   99,  516,  516,  524,  147,  148,  149,  150,  151,
 /*  3990 */   152,  153,  154,  155,  156,  157,  158,  159,  160,  161,
 /*  4000 */   162,  163,  164,  166,  165,  516,  516,  522,  516,   95,
 /*  4010 */    96,   98,   97,   99,  516,  516,  516,  462,  464,  466,
 /*  4020 */   468,  516,  101,  102,  597,  717,  656,  516,  516,  516,
 /*  4030 */   516,  516,  100,  524,  147,  148,  149,  150,  151,  152,
 /*  4040 */   153,  154,  155,  156,  157,  158,  159,  160,  161,  162,
 /*  4050 */   163,  164,  166,  165,  101,  102,  522,  516,  516,  516,
 /*  4060 */   610,  516,  516,  516,  100,  516,  462,  464,  466,  468,
 /*  4070 */   516,  516,  516,  540,  516,   95,   96,   98,   97,   99,
 /*  4080 */   516,  516,  524,  147,  148,  149,  150,  151,  152,  153,
 /*  4090 */   154,  155,  156,  157,  158,  159,  160,  161,  162,  163,
 /*  4100 */   164,  166,  165,  516,  516,  522,  516,   95,   96,   98,
 /*  4110 */    97,   99,  516,  516,  516,  462,  464,  466,  468,  516,
 /*  4120 */   101,  102,  591,   95,   96,   98,   97,   99,   23,  516,
 /*  4130 */   100,  524,  147,  148,  149,  150,  151,  152,  153,  154,
 /*  4140 */   155,  156,  157,  158,  159,  160,  161,  162,  163,  164,
 /*  4150 */   166,  165,  101,  102,  522,  516,  516,  516,  516,  516,
 /*  4160 */   138,  516,  100,  516,  462,  464,  466,  468,  101,  102,
 /*  4170 */   516,  550,  516,  516,  516,  516,  579,  516,  100,  516,
 /*  4180 */   524,  147,  148,  149,  150,  151,  152,  153,  154,  155,
 /*  4190 */   156,  157,  158,  159,  160,  161,  162,  163,  164,  166,
 /*  4200 */   165,  516,  516,  522,  516,  516,  516,  516,   75,  516,
 /*  4210 */   516,  516,  516,  462,  464,  466,  468,  516,  516,  516,
 /*  4220 */   585,   78,  516,  516,  516,  516,  516,  516,  516,  524,
 /*  4230 */   147,  148,  149,  150,  151,  152,  153,  154,  155,  156,
 /*  4240 */   157,  158,  159,  160,  161,  162,  163,  164,  166,  165,
 /*  4250 */   516,  516,  522,  516,  516,  516,  516,  516,   70,  516,
 /*  4260 */   516,  516,  462,  464,  466,  468,   68,   69,   71,   72,
 /*  4270 */    74,   73,  516,  516,  516,  516,  516,  516,  146,  147,
 /*  4280 */   148,  149,  150,  151,  152,  153,  154,  155,  156,  157,
 /*  4290 */   158,  159,  160,  161,  162,  163,  164,  166,  516,  516,
 /*  4300 */   516,  522,  516,  215,  224,  227,  218,  221,  516,  516,
 /*  4310 */   516,  462,  464,  466,  468,  266,  516,  516,  516,  516,
 /*  4320 */   516,  516,  516,  516,  516,  516,  516,  211,  274,  279,
 /*  4330 */   293,  208,  296,  289,  516,  516,  270,  516,   95,   96,
 /*  4340 */    98,   97,   99,  516,  300,  516,  516,  238,  516,  288,
 /*  4350 */   516,  260,  516,  516,  516,  516,  516,  307,  309,  308,
 /*  4360 */   278,  516,  258,  254,  256,  304,  310,  210,  220,  226,
 /*  4370 */   229,  231,  233,  223,  217,  242,  244,  252,  246,  248,
 /*  4380 */   250,  516,  516,  101,  102,  516,  282,  239,  235,  516,
 /*  4390 */   516,  105,  516,  100,  281,  283,  284,  285,  286,  287,
 /*  4400 */   290,  516,  215,  224,  227,  218,  221,  516,  241,  516,
 /*  4410 */   307,  309,  308,  278,  266,  516,  516,  338,  516,  516,
 /*  4420 */   346,  342,  345,  516,  516,  516,  211,  274,  279,  293,
 /*  4430 */   208,  516,  289,  516,  238,  270,  516,  516,  516,  516,
 /*  4440 */   516,  516,  516,  300,  307,  309,  308,  278,  288,  516,
 /*  4450 */   260,  516,  332,  310,  516,  516,  258,  254,  256,  516,
 /*  4460 */   516,  210,  220,  226,  229,  231,  233,  223,  217,  242,
 /*  4470 */   244,  252,  246,  248,  250,  516,  516,  290,  537,  516,
 /*  4480 */   516,  239,  235,  516,  516,  282,  516,  307,  309,  308,
 /*  4490 */   278,  516,  534,  281,  283,  284,  285,  286,  287,  344,
 /*  4500 */   516,  215,  224,  227,  218,  221,  516,  516,  516,  516,
 /*  4510 */   516,  516,  516,  266,  516,  516,  516,  516,  516,  516,
 /*  4520 */   516,  516,  516,  516,  516,  211,  274,  279,  293,  208,
 /*  4530 */   516,  289,  516,  516,  270,  516,  516,  516,  516,  516,
 /*  4540 */   516,  516,  300,  516,  516,  516,  516,  288,  516,  260,
 /*  4550 */   516,  516,  433,  516,  516,  258,  254,  256,  516,  516,
 /*  4560 */   210,  220,  226,  229,  231,  233,  223,  217,  242,  244,
 /*  4570 */   252,  246,  248,  250,  516,  516,  516,  516,  516,  516,
 /*  4580 */   239,  235,  516,  516,  282,  516,  516,  516,  516,  516,
 /*  4590 */   516,  386,  281,  283,  284,  285,  286,  287,  516,  516,
 /*  4600 */   215,  224,  227,  218,  221,  516,  516,  516,  516,  516,
 /*  4610 */   516,  516,  266,  516,  516,  516,  516,  516,  516,  516,
 /*  4620 */   516,  516,  516,  516,  211,  274,  279,  293,  208,  329,
 /*  4630 */   289,  516,  238,  270,  516,  516,  516,  516,  516,  516,
 /*  4640 */   516,  300,  307,  309,  308,  278,  288,  516,  260,  516,
 /*  4650 */   297,  310,  516,  516,  258,  254,  256,  516,  516,  210,
 /*  4660 */   220,  226,  229,  231,  233,  223,  217,  242,  244,  252,
 /*  4670 */   246,  248,  250,   95,   96,   98,   97,   99,  516,  239,
 /*  4680 */   235,  516,  516,  282,  516,  516,  516,  516,  516,  516,
 /*  4690 */   527,  281,  283,  284,  285,  286,  287,  516,  516,  215,
 /*  4700 */   224,  227,  218,  221,  516,  516,  516,  516,  516,  516,
 /*  4710 */   516,  266,  516,  516,  516,  516,  516,  516,  101,  102,
 /*  4720 */   516,  516,  516,  211,  274,  279,  293,  208,  100,  289,
 /*  4730 */   516,  238,  270,  473,  516,  516,  516,  516,  516,  516,
 /*  4740 */   300,  307,  309,  308,  278,  288,  516,  260,  516,  364,
 /*  4750 */   310,  516,  516,  258,  254,  256,  516,  516,  210,  220,
 /*  4760 */   226,  229,  231,  233,  223,  217,  242,  244,  252,  246,
 /*  4770 */   248,  250,  516,  516,  516,  516,  516,  516,  239,  235,
 /*  4780 */   516,  516,  282,  516,  516,  516,  516,  516,  516,  539,
 /*  4790 */   281,  283,  284,  285,  286,  287,  516,  516,  215,  224,
 /*  4800 */   227,  218,  221,  516,  516,  516,  516,  516,  238,  516,
 /*  4810 */   266,  516,  516,  516,  516,  516,  516,  516,  307,  309,
 /*  4820 */   308,  278,  211,  274,  279,  293,  208,  324,  289,  516,
 /*  4830 */   290,  270,  516,  516,  516,  516,  516,  516,  516,  300,
 /*  4840 */   307,  309,  308,  278,  288,  516,  260,  516,  516,  409,
 /*  4850 */   341,  342,  345,  516,  516,  516,  516,  258,  254,  256,
 /*  4860 */   516,  516,  210,  220,  226,  229,  231,  233,  223,  217,
 /*  4870 */   242,  244,  252,  246,  248,  250,  516,  516,  516,  516,
 /*  4880 */   516,  282,  239,  235,  516,  516,  472,  516,  516,  281,
 /*  4890 */   283,  284,  285,  286,  287,  516,  516,  215,  224,  227,
 /*  4900 */   218,  221,  516,  516,  516,  516,  516,  516,  516,  266,
 /*  4910 */   516,  516,  516,  516,  516,  516,  516,  516,  516,  516,
 /*  4920 */   516,  211,  274,  279,  293,  208,  349,  289,  516,  238,
 /*  4930 */   270,  516,  516,  516,  516,  516,  516,  516,  300,  307,
 /*  4940 */   309,  308,  278,  288,  516,  260,  516,  358,  310,  516,
 /*  4950 */   516,  516,  516,  516,  516,  516,  258,  254,  256,  516,
 /*  4960 */   516,  210,  220,  226,  229,  231,  233,  223,  217,  242,
 /*  4970 */   244,  252,  246,  248,  250,  516,  516,  516,  516,  516,
 /*  4980 */   282,  239,  235,  516,  516,  516,  516,  516,  281,  283,
 /*  4990 */   284,  285,  286,  287,  516,  516,  215,  224,  227,  218,
 /*  5000 */   221,  516,  372,  516,  516,  516,  516,  516,  266,  516,
 /*  5010 */   516,  516,  516,  516,  516,  516,  516,  516,  516,  516,
 /*  5020 */   211,  274,  279,  293,  208,  516,  289,  516,  516,  291,
 /*  5030 */   516,  516,  516,  516,  516,  516,  516,  300,  516,  516,
 /*  5040 */   238,  516,  288,  516,  260,  516,  516,  337,  516,  516,
 /*  5050 */   307,  309,  308,  278,  516,  258,  254,  256,  314,  310,
 /*  5060 */   210,  220,  226,  229,  231,  233,  223,  217,  242,  244,
 /*  5070 */   252,  246,  248,  250,  516,  516,  516,  516,  516,  336,
 /*  5080 */   239,  235,  516,  516,  516,  516,  516,  335,  283,  284,
 /*  5090 */   285,  286,  287,  516,  516,  215,  224,  227,  218,  221,
 /*  5100 */   516,  481,  516,  516,  516,  516,  516,  266,  516,  516,
 /*  5110 */   516,  516,  516,  516,  516,  516,  516,  516,  516,  211,
 /*  5120 */   274,  279,  293,  208,  360,  289,  516,  238,  270,  516,
 /*  5130 */   516,  516,  516,  516,  516,  516,  300,  307,  309,  308,
 /*  5140 */   278,  288,  516,  260,  516,  351,  310,  516,  516,  516,
 /*  5150 */   516,  516,  516,  516,  258,  254,  256,  516,  516,  210,
 /*  5160 */   220,  226,  229,  231,  233,  223,  217,  242,  244,  252,
 /*  5170 */   246,  248,  250,  516,  516,  516,  516,  516,  282,  239,
 /*  5180 */   235,  516,  516,  485,  516,  516,  281,  283,  284,  285,
 /*  5190 */   286,  287,  516,  516,  215,  224,  227,  218,  221,  516,
 /*  5200 */   516,  516,  516,  516,  516,  516,  266,  516,  516,  516,
 /*  5210 */   516,  516,  516,  516,  516,  516,  516,  516,  211,  274,
 /*  5220 */   279,  293,  208,  366,  289,  516,  238,  270,  516,  516,
 /*  5230 */   516,  516,  516,  516,  516,  300,  307,  309,  308,  278,
 /*  5240 */   288,  516,  260,  516,  321,  310,  516,  516,  516,  516,
 /*  5250 */   516,  516,  516,  258,  254,  256,  516,  516,  210,  220,
 /*  5260 */   226,  229,  231,  233,  223,  217,  242,  244,  252,  246,
 /*  5270 */   248,  250,  516,  516,  516,  516,  516,  282,  239,  235,
 /*  5280 */   516,  516,  516,  516,  516,  281,  283,  284,  285,  286,
 /*  5290 */   287,  516,  516,  215,  224,  227,  218,  221,  516,  885,
 /*  5300 */   516,  516,  516,  516,  516,  266,  516,  516,  516,  516,
 /*  5310 */   516,  516,  516,  516,  516,  516,  516,  211,  274,  279,
 /*  5320 */   293,  208,  334,  289,  516,  238,  270,  516,  516,  516,
 /*  5330 */   516,  516,  516,  516,  300,  307,  309,  308,  278,  288,
 /*  5340 */   516,  260,  516,  347,  310,  516,  516,  258,  254,  256,
 /*  5350 */   516,  516,  210,  220,  226,  229,  231,  233,  223,  217,
 /*  5360 */   242,  244,  252,  246,  248,  250,  516,  516,  516,  516,
 /*  5370 */   516,  262,  239,  235,  516,  516,  282,  516,  516,  516,
 /*  5380 */   516,  516,  516,  516,  281,  283,  284,  285,  286,  287,
 /*  5390 */   516,  516,  215,  224,  227,  218,  221,  516,  516,  516,
 /*  5400 */   516,  516,  516,  516,  266,  516,  516,  516,  516,  516,
 /*  5410 */   516,  516,  516,  516,  516,  516,  211,  274,  279,  293,
 /*  5420 */   208,  516,  289,  516,  516,  270,  516,  516,  516,  516,
 /*  5430 */   516,  516,  516,  300,  516,  516,  516,  516,  288,  516,
 /*  5440 */   260,  516,  516,  441,  516,  516,  258,  254,  256,  516,
 /*  5450 */   516,  210,  220,  226,  229,  231,  233,  223,  217,  242,
 /*  5460 */   244,  252,  246,  248,  250,  516,  516,  516,  516,  516,
 /*  5470 */   516,  239,  235,  516,  516,  282,  516,  516,  516,  516,
 /*  5480 */   516,  516,  516,  281,  283,  284,  285,  286,  287,  516,
 /*  5490 */   516,  215,  224,  227,  218,  221,  516,  516,  516,  516,
 /*  5500 */   516,  516,  516,  266,  516,  516,  516,  516,  516,  516,
 /*  5510 */   516,  516,  516,  516,  516,  211,  274,  279,  293,  208,
 /*  5520 */   316,  289,  516,  238,  270,  516,  516,  516,  516,  516,
 /*  5530 */   516,  516,  300,  307,  309,  308,  278,  288,  516,  260,
 /*  5540 */   516,  330,  310,  516,  516,  516,  254,  256,  516,  516,
 /*  5550 */   210,  220,  226,  229,  231,  233,  223,  217,  242,  244,
 /*  5560 */   252,  246,  248,  250,  516,  516,  516,  516,  516,  516,
 /*  5570 */   239,  235,  516,  516,  282,  516,  516,  516,  516,  516,
 /*  5580 */   516,  516,  281,  283,  284,  285,  286,  287,  516,  516,
 /*  5590 */   215,  224,  227,  218,  221,  516,  516,  516,  516,  516,
 /*  5600 */   516,  516,  266,  516,  516,  516,  516,  516,  408,  516,
 /*  5610 */   516,  516,  516,  516,  211,  274,  279,  293,  208,  323,
 /*  5620 */   289,  516,  395,  270,  516,  516,  516,  516,  516,  516,
 /*  5630 */   516,  300,  516,  516,  516,  516,  288,  516,  260,  516,
 /*  5640 */   516,  210,  220,  226,  229,  231,  233,  223,  217,  242,
 /*  5650 */   244,  252,  246,  248,  250,  516,  396,  397,  398,  399,
 /*  5660 */   400,  239,  235,  417,  418,  516,  516,  516,  516,  516,
 /*  5670 */   516,  516,  516,  282,  516,  516,  516,  516,  516,  516,
 /*  5680 */   516,  281,  283,  284,  285,  286,  287,  516,  516,  215,
 /*  5690 */   224,  227,  218,  221,  516,  516,  516,  516,  516,  516,
 /*  5700 */   516,  266,  516,   91,  516,  516,  516,  516,  516,  516,
 /*  5710 */   516,  516,  516,  211,  274,  279,  293,  208,  306,  289,
 /*  5720 */    26,   91,  270,  516,   65,  516,  516,  516,  516,  516,
 /*  5730 */   300,  516,  516,  516,  516,  288,  516,  260,  108,  516,
 /*  5740 */   516,  516,   65,   44,   45,   46,   47,   48,   49,   50,
 /*  5750 */    51,   52,   53,   54,  516,  516,  516,  516,  516,  516,
 /*  5760 */   516,   44,   45,   46,   47,   48,   49,   50,   51,   52,
 /*  5770 */    53,   54,  282,  516,  516,  516,  516,  516,  516,  516,
 /*  5780 */   281,  283,  284,  285,  286,  287,  516,  516,  215,  224,
 /*  5790 */   227,  218,  221,  516,  516,  516,  516,  516,  516,  516,
 /*  5800 */   266,  516,   91,  516,  516,  516,  516,  516,  516,  516,
 /*  5810 */   516,  516,  211,  274,  279,  293,  208,  516,  289,  582,
 /*  5820 */   516,  270,  516,   65,  516,  516,  516,  516,  516,  300,
 /*  5830 */   516,  516,  516,  516,  288,  516,  260,  516,  516,  516,
 /*  5840 */   516,  516,   44,   45,   46,   47,   48,   49,   50,   51,
 /*  5850 */    52,   53,   54,  516,  516,  516,  516,  516,  516,  516,
 /*  5860 */   516,  516,  516,  516,  525,  516,  516,  516,  516,  516,
 /*  5870 */   516,  282,  516,  516,  516,  516,  516,  516,  516,  281,
 /*  5880 */   283,  284,  285,  286,  287,  516,  516,  215,  224,  227,
 /*  5890 */   218,  221,  516,  516,  516,  516,  516,  516,  516,  266,
 /*  5900 */   516,   91,  516,  516,  516,  516,  516,  516,  516,  516,
 /*  5910 */   516,  211,  274,  279,  293,  208,  353,  289,  141,   91,
 /*  5920 */   270,  516,   65,  516,  516,  516,  516,  516,  300,  516,
 /*  5930 */   516,  516,  516,  288,  516,  260,  516,  516,  516,  516,
 /*  5940 */    65,   44,   45,   46,   47,   48,   49,   50,   51,   52,
 /*  5950 */    53,   54,  516,  516,  516,  516,  516,  516,  516,   44,
 /*  5960 */    45,   46,   47,   48,   49,   50,   51,   52,   53,   54,
 /*  5970 */   282,  516,  516,  516,  516,  516,  516,  516,  281,  283,
 /*  5980 */   284,  285,  286,  287,  516,  516,  215,  224,  227,  218,
 /*  5990 */   221,  516,  516,  516,  516,  516,  516,  516,  266,   38,
 /*  6000 */   516,  516,  516,  516,  516,  516,  516,  516,  516,  516,
 /*  6010 */   211,  274,  279,  293,  208,  516,  289,  516,  516,  270,
 /*  6020 */   516,  516,  516,  516,  516,  516,  516,  300,  516,  516,
 /*  6030 */   516,  516,  288,  516,  260,  516,  516,  516,  516,   44,
 /*  6040 */    45,   46,   47,   48,   49,   50,   51,   52,   53,   54,
 /*  6050 */   516,  516,  516,  516,  516,  516,  516,  516,  516,  516,
 /*  6060 */   516,  516,  516,  516,  516,  516,  516,  516,  516,  282,
 /*  6070 */   516,  516,  516,  516,  516,  516,  516,  281,  283,  284,
 /*  6080 */   285,  286,  287,  516,  516,  215,  224,  227,  218,  221,
 /*  6090 */   516,  516,  516,  516,  516,  516,  516,  266,  516,  516,
 /*  6100 */   516,  516,  516,  516,  516,  516,  516,  516,  516,  211,
 /*  6110 */   274,  279,  293,  208,  516,  289,  516,  516,  291,  516,
 /*  6120 */   516,  516,  516,  516,  516,  516,  300,  516,  516,  516,
 /*  6130 */   516,  288,  516,  260,  516,  516,  516,  516,  516,  516,
 /*  6140 */   516,  516,  516,  516,  516,  516,  516,  516,  516,  516,
 /*  6150 */   516,  516,  516,  516,  516,  516,  516,  516,  516,  516,
 /*  6160 */   516,  516,  516,  516,  516,  516,  516,  516,  336,  516,
 /*  6170 */   516,  516,  516,  516,  516,  516,  335,  283,  284,  285,
 /*  6180 */   286,  287,
};
static YYCODETYPE yy_lookahead[] = {
 /*     0 */     7,    8,    9,   10,   11,   25,   26,   27,   28,   29,
 /*    10 */    30,   40,   19,  136,  137,  138,  139,   37,   38,  108,
 /*    20 */   109,  110,  111,  112,   31,   32,   33,   34,   35,   40,
 /*    30 */    37,    6,  116,   40,   41,  119,  120,  121,  122,  123,
 /*    40 */   123,   48,   49,   50,  128,  129,   53,  130,   55,    6,
 /*    50 */   134,  134,   59,   60,   61,   62,   63,   64,   65,   66,
 /*    60 */    67,   36,   69,   70,   71,   72,  119,   74,  121,  122,
 /*    70 */   123,   78,   79,   80,   81,  128,  129,   84,    0,   36,
 /*    80 */    43,  134,   45,   90,   55,   48,   20,   94,   95,   96,
 /*    90 */    97,   98,   99,  100,  101,  102,  103,    7,    8,    9,
 /*   100 */    10,   11,  133,   37,  135,  136,  137,  138,  139,   19,
 /*   110 */   105,  106,  107,  108,  109,  110,  111,  112,   49,   38,
 /*   120 */    91,   31,   32,   33,   34,   35,  123,   37,   41,   48,
 /*   130 */    40,   41,  129,  120,  121,  122,  123,  134,   48,   49,
 /*   140 */    50,  128,  129,   53,   75,   55,   77,  134,   40,   59,
 /*   150 */    60,   61,   62,   63,   64,   65,   66,   67,   50,   69,
 /*   160 */    70,   71,   72,   40,   74,   37,   51,   40,   78,   79,
 /*   170 */    80,   81,   49,   50,   84,  137,  138,  113,  140,   51,
 /*   180 */    90,   37,   38,   40,   94,   95,   96,   97,   98,   99,
 /*   190 */   100,  101,  102,  103,    7,    8,    9,   10,   11,  164,
 /*   200 */    85,   86,   87,   88,   89,   40,   19,  172,  173,  174,
 /*   210 */   175,  176,  177,   85,   86,   87,   88,   89,   31,   32,
 /*   220 */    33,   34,   35,   41,   37,   37,   38,   40,   41,  118,
 /*   230 */    48,  121,  123,  123,  123,   48,   49,   50,  128,  129,
 /*   240 */    53,  130,   55,  134,  134,  134,   59,   60,   61,   62,
 /*   250 */    63,   64,   65,   66,   67,   90,   69,   70,   71,   72,
 /*   260 */    37,   74,  121,  122,  123,   78,   79,   80,   81,   41,
 /*   270 */   129,   84,   40,   43,   51,  134,   48,   90,   48,   51,
 /*   280 */    48,   94,   95,   96,   97,   98,   99,  100,  101,  102,
 /*   290 */   103,    7,    8,    9,   10,   11,  133,   38,  135,  136,
 /*   300 */   137,  138,  139,   19,   28,   29,   30,   48,   85,   86,
 /*   310 */    87,   88,   89,   37,   38,   31,   32,   33,   34,   35,
 /*   320 */   131,   37,   48,   40,   40,   41,  137,  138,   41,  140,
 /*   330 */   123,   48,   48,   49,   50,   48,    6,   53,   51,   55,
 /*   340 */    55,  134,   38,   59,   60,   61,   62,   63,   64,   65,
 /*   350 */    66,   67,   48,   69,   70,   71,   72,    0,   74,  113,
 /*   360 */   164,   76,   78,   79,   80,   81,   36,   49,   84,  173,
 /*   370 */   174,  175,  176,  177,   90,  115,   91,  117,   94,   95,
 /*   380 */    96,   97,   98,   99,  100,  101,  102,  103,    7,    8,
 /*   390 */     9,   10,   11,   75,    6,   77,   39,    6,  164,   42,
 /*   400 */    19,   44,  167,   46,   47,  170,  171,   50,  174,  175,
 /*   410 */   176,  177,   31,   32,   33,   34,   35,  131,   37,  165,
 /*   420 */   166,   40,   41,  137,  138,   38,  140,  126,  127,   48,
 /*   430 */    49,   50,   37,   38,   53,   48,   55,   49,    6,   48,
 /*   440 */    59,   60,   61,   62,   63,   64,   65,   66,   67,  134,
 /*   450 */    69,   70,   71,   72,  161,   74,  115,  164,  117,   78,
 /*   460 */    79,   80,   81,  137,    6,   84,   40,  174,  175,  176,
 /*   470 */   177,   90,   55,   41,   48,   94,   95,   96,   97,   98,
 /*   480 */    99,  100,  101,  102,  103,    7,    8,    9,   10,   11,
 /*   490 */   164,   43,   45,   45,  167,   48,   48,   19,  171,   41,
 /*   500 */   174,  175,  176,  177,   38,   40,    6,   41,   91,   31,
 /*   510 */    32,   33,   34,   35,  131,   37,   40,   40,   40,   41,
 /*   520 */   137,  138,   38,  140,   48,   41,   48,   49,   50,    6,
 /*   530 */    53,   53,   40,   55,    6,  123,   36,   59,   60,   61,
 /*   540 */    62,   63,   64,   65,   66,   67,  134,   69,   70,   71,
 /*   550 */    72,  161,   74,    6,  164,   90,   78,   79,   80,   81,
 /*   560 */   178,  179,   84,   55,  174,  175,  176,  177,   90,   41,
 /*   570 */   178,  179,   94,   95,   96,   97,   98,   99,  100,  101,
 /*   580 */   102,  103,    7,    8,    9,   10,   11,  133,   41,  135,
 /*   590 */   136,  137,  138,  139,   19,  133,    6,  135,  136,  137,
 /*   600 */   138,  139,  178,  179,   41,   82,   31,   32,   33,   34,
 /*   610 */    35,  131,   37,  178,  179,   40,   41,  137,  138,  115,
 /*   620 */   140,  117,    6,   48,   49,   50,   36,   35,   53,   38,
 /*   630 */    55,    6,   41,   40,   59,   60,   61,   62,   63,   64,
 /*   640 */    65,   66,   67,   40,   69,   70,   71,   72,  161,   74,
 /*   650 */     6,  164,   40,   78,   79,   80,   81,   41,   15,   84,
 /*   660 */    48,  174,  175,  176,  177,   90,   41,  178,  179,   94,
 /*   670 */    95,   96,   97,   98,   99,  100,  101,  102,  103,    7,
 /*   680 */     8,    9,   10,   11,  133,    6,  135,  136,  137,  138,
 /*   690 */   139,   19,  133,   90,  135,  136,  137,  138,  139,    6,
 /*   700 */   178,  179,   58,   31,   32,   33,   34,   35,   58,   37,
 /*   710 */     6,   41,   40,   41,    6,   36,  178,  179,   48,    6,
 /*   720 */    48,   49,   50,  178,  179,   53,   20,   55,    6,   36,
 /*   730 */     6,   59,   60,   61,   62,   63,   64,   65,   66,   67,
 /*   740 */    36,   69,   70,   71,   72,  161,   74,   51,  164,   41,
 /*   750 */    78,   79,   80,   81,   41,    6,   84,   41,  174,  175,
 /*   760 */   176,  177,   90,   41,  124,   41,   94,   95,   96,   97,
 /*   770 */    98,   99,  100,  101,  102,  103,    7,    8,    9,   10,
 /*   780 */    11,  133,    6,  135,  136,  137,  138,  139,   19,  133,
 /*   790 */    41,  135,  136,  137,  138,  139,  178,  179,  178,  179,
 /*   800 */    31,   32,   33,   34,   35,   38,   37,    6,   41,   40,
 /*   810 */    41,    6,   36,  162,  163,   35,   91,   48,   49,   50,
 /*   820 */    40,    6,   53,   53,   55,   51,  124,   40,   59,   60,
 /*   830 */    61,   62,   63,   64,   65,   66,   67,   36,   69,   70,
 /*   840 */    71,   72,  161,   74,   51,  164,   41,   78,   79,   80,
 /*   850 */    81,   36,    6,   84,  124,  174,  175,  176,  177,   90,
 /*   860 */    40,   51,    6,   94,   95,   96,   97,   98,   99,  100,
 /*   870 */   101,  102,  103,    7,    8,    9,   10,   11,  164,   43,
 /*   880 */   124,    6,   36,  169,   48,   19,   55,   40,  174,  175,
 /*   890 */   176,  177,   36,   20,   40,  114,   41,   31,   32,   33,
 /*   900 */    34,   35,   41,   37,   40,   40,   40,   41,  114,   40,
 /*   910 */   114,   36,   49,   55,   48,   49,   50,   40,   48,   53,
 /*   920 */    41,   55,   41,   73,   48,   59,   60,   61,   62,   63,
 /*   930 */    64,   65,   66,   67,   48,   69,   70,   71,   72,   48,
 /*   940 */    74,  164,  163,  124,   78,   79,   80,   81,   76,   48,
 /*   950 */    84,  174,  175,  176,  177,   48,   90,   79,   41,   40,
 /*   960 */    94,   95,   96,   97,   98,   99,  100,  101,  102,  103,
 /*   970 */     7,    8,    9,   10,   11,  164,   55,  180,   36,  168,
 /*   980 */    40,   20,   19,   55,   40,  174,  175,  176,  177,    6,
 /*   990 */    55,   40,  180,   55,   31,   32,   33,   34,   35,   49,
 /*  1000 */    37,   49,   40,   40,   41,   40,   49,   76,   55,   55,
 /*  1010 */    76,   48,   49,   50,   55,   76,   53,   49,   55,   49,
 /*  1020 */    40,   55,   59,   60,   61,   62,   63,   64,   65,   66,
 /*  1030 */    67,   49,   69,   70,   71,   72,   49,   74,  164,   40,
 /*  1040 */    49,   78,   79,   80,   81,   49,  166,   84,  174,  175,
 /*  1050 */   176,  177,  167,   90,  167,   58,  167,   94,   95,   96,
 /*  1060 */    97,   98,   99,  100,  101,  102,  103,    7,    8,    9,
 /*  1070 */    10,   11,  164,  167,   58,   58,  168,  167,  167,   19,
 /*  1080 */   167,  167,  174,  175,  176,  177,   41,  167,   41,   41,
 /*  1090 */    41,   31,   32,   33,   34,   35,   40,   37,   40,   40,
 /*  1100 */    40,   41,   41,   41,  179,   51,   40,   82,   48,   49,
 /*  1110 */    50,   79,  124,   53,   41,   55,   76,   73,   48,   59,
 /*  1120 */    60,   61,   62,   63,   64,   65,   66,   67,   48,   69,
 /*  1130 */    70,   71,   72,   48,   74,  164,   40,   55,   78,   79,
 /*  1140 */    80,   81,   48,   48,   84,  174,  175,  176,  177,   41,
 /*  1150 */    90,   53,   51,  124,   94,   95,   96,   97,   98,   99,
 /*  1160 */   100,  101,  102,  103,    7,    8,    9,   10,   11,  164,
 /*  1170 */    53,   51,   41,  168,  124,  124,   19,  125,  125,  174,
 /*  1180 */   175,  176,  177,   41,   41,   41,   40,  124,   31,   32,
 /*  1190 */    33,   34,   35,   41,   37,   41,  127,   40,   41,  125,
 /*  1200 */   125,   41,  114,  117,  114,   48,   49,   50,   44,   40,
 /*  1210 */    53,   40,   55,  114,   48,   40,   59,   60,   61,   62,
 /*  1220 */    63,   64,   65,   66,   67,  114,   69,   70,   71,   72,
 /*  1230 */    44,   74,  164,  114,   40,   78,   79,   80,   81,  114,
 /*  1240 */   187,   84,  174,  175,  176,  177,  187,   90,  187,  187,
 /*  1250 */   187,   94,   95,   96,   97,   98,   99,  100,  101,  102,
 /*  1260 */   103,    7,    8,    9,   10,   11,  164,  187,  187,  187,
 /*  1270 */   168,  187,  187,   19,  187,  187,  174,  175,  176,  177,
 /*  1280 */   187,  187,  187,  187,  187,   31,   32,   33,   34,   35,
 /*  1290 */   187,   37,  187,  187,   40,   41,  187,  187,  187,  187,
 /*  1300 */   187,  187,   48,   49,   50,  187,  187,   53,  187,   55,
 /*  1310 */   187,  187,  187,   59,   60,   61,   62,   63,   64,   65,
 /*  1320 */    66,   67,  187,   69,   70,   71,   72,  187,   74,  164,
 /*  1330 */   187,  187,   78,   79,   80,   81,  187,  187,   84,  174,
 /*  1340 */   175,  176,  177,  187,   90,  187,  187,  187,   94,   95,
 /*  1350 */    96,   97,   98,   99,  100,  101,  102,  103,    7,    8,
 /*  1360 */     9,   10,   11,  164,  187,  187,  187,  187,  169,  187,
 /*  1370 */    19,  187,  187,  174,  175,  176,  177,  187,  187,  187,
 /*  1380 */   187,  187,   31,   32,   33,   34,   35,  187,   37,  187,
 /*  1390 */   187,   40,   41,  187,  187,  187,  187,  187,  187,   48,
 /*  1400 */    49,   50,  187,  187,   53,  187,   55,  187,  187,  187,
 /*  1410 */    59,   60,   61,   62,   63,   64,   65,   66,   67,  187,
 /*  1420 */    69,   70,   71,   72,  187,   74,  164,  187,  187,   78,
 /*  1430 */    79,   80,   81,  187,  187,   84,  174,  175,  176,  177,
 /*  1440 */   187,   90,  187,  187,  187,   94,   95,   96,   97,   98,
 /*  1450 */    99,  100,  101,  102,  103,    7,    8,    9,   10,   11,
 /*  1460 */   164,  187,  187,  187,  168,  187,  187,   19,  187,  187,
 /*  1470 */   174,  175,  176,  177,  187,  187,  187,  187,  187,   31,
 /*  1480 */    32,   33,   34,   35,  187,   37,  187,  187,   40,   41,
 /*  1490 */   187,  187,  187,  187,  187,  187,   48,   49,   50,  187,
 /*  1500 */   187,   53,  187,   55,  187,  187,  187,   59,   60,   61,
 /*  1510 */    62,   63,   64,   65,   66,   67,  187,   69,   70,   71,
 /*  1520 */    72,  187,   74,  164,  187,  187,   78,   79,   80,   81,
 /*  1530 */   187,  187,   84,  174,  175,  176,  177,  187,   90,  187,
 /*  1540 */   187,  187,   94,   95,   96,   97,   98,   99,  100,  101,
 /*  1550 */   102,  103,    7,    8,    9,   10,   11,  187,   39,  187,
 /*  1560 */   187,   42,  187,   44,   19,   46,   47,  187,  187,   50,
 /*  1570 */   187,  187,  187,  187,  187,  187,   31,   32,   33,   34,
 /*  1580 */    35,  187,   37,  187,  187,   40,   41,   28,   29,   30,
 /*  1590 */   187,  187,  187,   48,   49,   50,   37,   38,   53,  187,
 /*  1600 */    55,  187,  187,  187,   59,   60,   61,   62,   63,   64,
 /*  1610 */    65,   66,   67,  187,   69,   70,   71,   72,  187,   74,
 /*  1620 */   164,  187,  187,   78,   79,   80,   81,  187,  187,   84,
 /*  1630 */   174,  175,  176,  177,  187,   90,  187,  187,  187,   94,
 /*  1640 */    95,   96,   97,   98,   99,  100,  101,  102,  103,    7,
 /*  1650 */     8,    9,   10,   11,  164,  187,  187,  187,  168,  187,
 /*  1660 */   187,   19,  187,  187,  174,  175,  176,  177,  187,  187,
 /*  1670 */   187,  187,  187,   31,   32,   33,   34,   35,  187,   37,
 /*  1680 */   187,  187,   40,   41,  187,  187,  187,  187,  187,  187,
 /*  1690 */    48,   49,   50,  187,  187,   53,  187,   55,  187,  187,
 /*  1700 */   187,   59,   60,   61,   62,   63,   64,   65,   66,   67,
 /*  1710 */   187,   69,   70,   71,   72,  187,   74,  164,  187,  187,
 /*  1720 */    78,   79,   80,   81,  187,  187,   84,  174,  175,  176,
 /*  1730 */   177,  187,   90,  187,  187,  187,   94,   95,   96,   97,
 /*  1740 */    98,   99,  100,  101,  102,  103,    7,    8,    9,   10,
 /*  1750 */    11,  164,  187,  187,  187,  168,  187,  187,   19,  187,
 /*  1760 */   187,  174,  175,  176,  177,  187,  187,  187,  187,  187,
 /*  1770 */    31,   32,   33,   34,   35,  187,   37,  187,  187,   40,
 /*  1780 */    41,  187,  187,  187,  187,  187,  187,   48,   49,   50,
 /*  1790 */   187,  187,   53,  187,   55,  187,  187,  187,   59,   60,
 /*  1800 */    61,   62,   63,   64,   65,   66,   67,  187,   69,   70,
 /*  1810 */    71,   72,  187,   74,  164,  187,  187,   78,   79,   80,
 /*  1820 */    81,  187,  187,   84,  174,  175,  176,  177,  187,   90,
 /*  1830 */   187,  187,  187,   94,   95,   96,   97,   98,   99,  100,
 /*  1840 */   101,  102,  103,    7,    8,    9,   10,   11,  164,  187,
 /*  1850 */   187,  187,  168,  187,  187,   19,  187,  187,  174,  175,
 /*  1860 */   176,  177,  187,  187,  187,  187,  187,   31,   32,   33,
 /*  1870 */    34,   35,  187,   37,  187,  187,   40,   41,  187,  187,
 /*  1880 */   187,  187,  187,  187,   48,   49,   50,  187,  187,   53,
 /*  1890 */   187,   55,  187,  187,  187,   59,   60,   61,   62,   63,
 /*  1900 */    64,   65,   66,   67,  187,   69,   70,   71,   72,  187,
 /*  1910 */    74,  164,  187,  187,   78,   79,   80,   81,  187,  187,
 /*  1920 */    84,  174,  175,  176,  177,  187,   90,  187,  187,  187,
 /*  1930 */    94,   95,   96,   97,   98,   99,  100,  101,  102,  103,
 /*  1940 */     7,    8,    9,   10,   11,  164,  187,  187,  187,  168,
 /*  1950 */   187,  187,   19,  187,  187,  174,  175,  176,  177,  187,
 /*  1960 */   187,  187,  187,  187,   31,   32,   33,   34,   35,  187,
 /*  1970 */    37,  187,  187,   40,   41,  187,  187,  187,  187,  187,
 /*  1980 */   187,   48,   49,   50,  187,  187,   53,  187,   55,  187,
 /*  1990 */   187,  187,   59,   60,   61,   62,   63,   64,   65,   66,
 /*  2000 */    67,  187,   69,   70,   71,   72,  187,   74,  164,  187,
 /*  2010 */   187,   78,   79,   80,   81,  187,  187,   84,  174,  175,
 /*  2020 */   176,  177,  187,   90,  187,  187,  187,   94,   95,   96,
 /*  2030 */    97,   98,   99,  100,  101,  102,  103,    7,    8,    9,
 /*  2040 */    10,   11,  164,  187,  187,  187,  187,  169,  187,   19,
 /*  2050 */   187,  187,  174,  175,  176,  177,  187,  187,  187,  187,
 /*  2060 */   187,   31,   32,   33,   34,   35,  187,   37,  187,  187,
 /*  2070 */    40,   41,  187,  187,  187,  187,  187,  187,   48,   49,
 /*  2080 */    50,  187,  187,   53,  187,   55,  187,  187,  187,   59,
 /*  2090 */    60,   61,   62,   63,   64,   65,   66,   67,  187,   69,
 /*  2100 */    70,   71,   72,  187,   74,  164,  187,  187,   78,   79,
 /*  2110 */    80,   81,  187,  187,   84,  174,  175,  176,  177,  187,
 /*  2120 */    90,  187,  187,  187,   94,   95,   96,   97,   98,   99,
 /*  2130 */   100,  101,  102,  103,    7,    8,    9,   10,   11,  164,
 /*  2140 */   187,  187,  187,  168,  187,  187,   19,  187,  187,  174,
 /*  2150 */   175,  176,  177,  187,  187,  187,  187,  187,   31,   32,
 /*  2160 */    33,   34,   35,  187,   37,  187,  187,   40,   41,  187,
 /*  2170 */   187,  187,  187,  187,  187,   48,   49,   50,  187,  187,
 /*  2180 */    53,  187,   55,  187,  187,  187,   59,   60,   61,   62,
 /*  2190 */    63,   64,   65,   66,   67,  187,   69,   70,   71,   72,
 /*  2200 */   187,   74,  164,  187,  187,   78,   79,   80,   81,  187,
 /*  2210 */   187,   84,  174,  175,  176,  177,  187,   90,  187,  187,
 /*  2220 */   187,   94,   95,   96,   97,   98,   99,  100,  101,  102,
 /*  2230 */   103,    7,    8,    9,   10,   11,  164,  187,  187,  187,
 /*  2240 */   168,  187,  187,   19,  187,  187,  174,  175,  176,  177,
 /*  2250 */   187,  187,  187,  187,  187,   31,   32,   33,   34,   35,
 /*  2260 */   187,   37,  187,  187,   40,   41,  187,  187,  187,  187,
 /*  2270 */   187,  187,   48,   49,   50,  187,  187,   53,  187,   55,
 /*  2280 */   187,  187,  187,   59,   60,   61,   62,   63,   64,   65,
 /*  2290 */    66,   67,  187,   69,   70,   71,   72,  187,   74,  164,
 /*  2300 */   187,  187,   78,   79,   80,   81,  187,  187,   84,  174,
 /*  2310 */   175,  176,  177,  187,   90,  187,  187,  187,   94,   95,
 /*  2320 */    96,   97,   98,   99,  100,  101,  102,  103,    7,    8,
 /*  2330 */     9,   10,   11,  164,  187,  187,  187,  187,  187,  187,
 /*  2340 */    19,  187,  187,  174,  175,  176,  177,  187,  187,  187,
 /*  2350 */   187,  187,   31,   32,   33,   34,   35,  187,   37,  187,
 /*  2360 */   187,   40,   41,  187,  187,  187,  187,  187,  187,   48,
 /*  2370 */    49,   50,  187,  187,   53,  187,   55,  187,  187,  187,
 /*  2380 */    59,   60,   61,   62,   63,   64,   65,   66,   67,  187,
 /*  2390 */    69,   70,   71,   72,  187,   74,  164,  187,  187,   78,
 /*  2400 */    79,   80,   81,  187,  187,   84,  174,  175,  176,  177,
 /*  2410 */   187,   90,  187,  187,  187,   94,   95,   96,   97,   98,
 /*  2420 */    99,  100,  101,  102,  103,    7,    8,    9,   10,   11,
 /*  2430 */   164,  187,  187,  187,  187,  187,  187,   19,  187,  187,
 /*  2440 */   174,  175,  176,  177,  187,  187,  187,  187,  187,   31,
 /*  2450 */    32,   33,   34,   35,  187,   37,  187,  187,   40,   41,
 /*  2460 */   187,  187,  187,  187,  187,  187,   48,   49,   50,  187,
 /*  2470 */   187,   53,  187,   55,  187,  187,  187,   59,   60,   61,
 /*  2480 */    62,   63,   64,   65,   66,   67,  187,   69,   70,   71,
 /*  2490 */    72,  187,   74,  164,  187,  187,   78,   79,   80,   81,
 /*  2500 */   187,  187,   84,  174,  175,  176,  177,  187,   90,  187,
 /*  2510 */   187,  187,   94,   95,   96,   97,   98,   99,  100,  101,
 /*  2520 */   102,  103,    7,    8,    9,   10,   11,  164,  187,  187,
 /*  2530 */   187,  187,  187,  187,   19,  187,  187,  174,  175,  176,
 /*  2540 */   177,  187,  187,  187,  187,  187,   31,   32,   33,   34,
 /*  2550 */    35,  187,   37,  187,  187,   40,   41,  187,  187,  187,
 /*  2560 */   187,  187,  187,   48,   49,   50,  187,  187,   53,  187,
 /*  2570 */    55,  187,  187,  187,   59,   60,   61,   62,   63,   64,
 /*  2580 */    65,   66,   67,  187,   69,   70,   71,   72,  187,   74,
 /*  2590 */   164,  187,  187,   78,   79,   80,   81,  187,  187,   84,
 /*  2600 */   174,  175,  176,  177,  187,   90,  187,  187,  187,   94,
 /*  2610 */    95,   96,   97,   98,   99,  100,  101,  102,  103,    7,
 /*  2620 */     8,    9,   10,   11,  164,  187,  187,  187,  187,  187,
 /*  2630 */   187,   19,  187,  187,  174,  175,  176,  177,  187,  187,
 /*  2640 */   187,  187,  187,   31,   32,   33,   34,   35,  187,   37,
 /*  2650 */   187,  187,   40,   41,  187,  187,  187,  187,  187,  187,
 /*  2660 */    48,   49,   50,  187,  187,   53,  187,   55,  187,  187,
 /*  2670 */   187,   59,   60,   61,   62,   63,   64,   65,   66,   67,
 /*  2680 */   187,   69,   70,   71,   72,  187,   74,  164,  187,  187,
 /*  2690 */    78,   79,   80,   81,  187,  187,   84,  174,  175,  176,
 /*  2700 */   177,  187,   90,  187,  187,  187,   94,   95,   96,   97,
 /*  2710 */    98,   99,  100,  101,  102,  103,    7,    8,    9,   10,
 /*  2720 */    11,  164,  187,  187,  187,  187,  187,  187,   19,  187,
 /*  2730 */   187,  174,  175,  176,  177,  187,  187,  187,  187,  187,
 /*  2740 */    31,   32,   33,   34,   35,  187,   37,  187,  187,   40,
 /*  2750 */    41,  187,  187,  187,  187,  187,  187,   48,   49,   50,
 /*  2760 */   187,  187,   53,  187,   55,  187,  187,  187,   59,   60,
 /*  2770 */    61,   62,   63,   64,   65,   66,   67,  187,   69,   70,
 /*  2780 */    71,   72,  187,   74,  164,  187,  187,   78,   79,   80,
 /*  2790 */    81,  187,  187,   84,  174,  175,  176,  177,  187,   90,
 /*  2800 */   187,  187,  187,   94,   95,   96,   97,   98,   99,  100,
 /*  2810 */   101,  102,  103,    7,    8,    9,   10,   11,  164,  187,
 /*  2820 */   187,  187,  187,  187,  187,   19,  187,  187,  174,  175,
 /*  2830 */   176,  177,  187,  187,  187,  187,  187,   31,   32,   33,
 /*  2840 */    34,   35,  187,   37,  187,  187,   40,   41,  187,  187,
 /*  2850 */   187,  187,  187,  187,   48,   49,   50,  187,  187,   53,
 /*  2860 */   187,   55,  187,  187,  187,   59,   60,   61,   62,   63,
 /*  2870 */    64,   65,   66,   67,  187,   69,   70,   71,   72,  187,
 /*  2880 */    74,  164,  187,  187,   78,   79,   80,   81,  187,  187,
 /*  2890 */    84,  174,  175,  176,  177,  187,   90,  187,  187,  187,
 /*  2900 */    94,   95,   96,   97,   98,   99,  100,  101,  102,  103,
 /*  2910 */     7,    8,    9,   10,   11,  164,  187,  187,  187,  187,
 /*  2920 */   187,  187,   19,  187,  187,  174,  175,  176,  177,  187,
 /*  2930 */   187,  187,  187,  187,   31,   32,   33,   34,   35,  187,
 /*  2940 */    37,  187,  187,   40,   41,  187,  187,  187,  187,  187,
 /*  2950 */   187,   48,   49,   50,  187,  187,   53,  187,   55,  187,
 /*  2960 */   187,  187,   59,   60,   61,   62,   63,   64,   65,   66,
 /*  2970 */    67,  187,   69,   70,   71,   72,  187,   74,  164,  187,
 /*  2980 */   187,   78,   79,   80,   81,  187,  187,   84,  174,  175,
 /*  2990 */   176,  177,  187,   90,  187,  187,  187,   94,   95,   96,
 /*  3000 */    97,   98,   99,  100,  101,  102,  103,    7,    8,    9,
 /*  3010 */    10,   11,  164,  187,  187,  187,  187,  187,  187,   19,
 /*  3020 */   187,  187,  174,  175,  176,  177,  187,  187,  187,  187,
 /*  3030 */   187,   31,   32,   33,   34,   35,  187,   37,  187,  187,
 /*  3040 */    40,   41,  187,  187,  187,  187,  187,  187,   48,   49,
 /*  3050 */    50,  187,  187,   53,  187,   55,  187,  187,  187,   59,
 /*  3060 */    60,   61,   62,   63,   64,   65,   66,   67,  187,   69,
 /*  3070 */    70,   71,   72,  187,   74,  164,  187,  187,   78,   79,
 /*  3080 */    80,   81,  187,  187,   84,  174,  175,  176,  177,  187,
 /*  3090 */    90,  187,  187,  187,   94,   95,   96,   97,   98,   99,
 /*  3100 */   100,  101,  102,  103,    7,    8,    9,   10,   11,  164,
 /*  3110 */   187,  187,  187,  187,  187,  187,   19,  187,  187,  174,
 /*  3120 */   175,  176,  177,  187,  187,  187,  187,  187,   31,   32,
 /*  3130 */    33,   34,   35,  187,   37,  187,  187,   40,   41,  187,
 /*  3140 */   187,  187,  187,  187,  187,   48,  187,   50,  187,  187,
 /*  3150 */    53,  187,   55,  187,  187,  187,   59,   60,   61,   62,
 /*  3160 */    63,   64,   65,   66,   67,  187,   69,   70,   71,   72,
 /*  3170 */   187,   74,  164,  187,  187,   78,   79,   80,   81,  187,
 /*  3180 */   187,   84,  174,  175,  176,  177,  187,   90,  187,  187,
 /*  3190 */   187,   94,   95,   96,   97,   98,   99,  100,  101,  102,
 /*  3200 */   103,    7,    8,    9,   10,   11,  164,  187,  187,  187,
 /*  3210 */   187,  187,  187,   19,  187,  187,  174,  175,  176,  177,
 /*  3220 */   187,  187,  187,  187,  187,   31,   32,   33,   34,   35,
 /*  3230 */   187,   37,  187,   19,   40,  187,  187,  187,  187,  187,
 /*  3240 */   187,  187,   48,  187,  187,  187,  187,   53,  187,   55,
 /*  3250 */   187,  187,  187,   59,   60,   61,   62,   63,   64,   65,
 /*  3260 */    66,   67,   68,   69,  187,  187,  187,  187,  187,  112,
 /*  3270 */   187,   57,  187,   59,   60,   61,   62,   63,   64,   65,
 /*  3280 */    66,   67,   68,   69,   90,  187,  187,  187,  187,  132,
 /*  3290 */   187,  187,   98,   99,  100,  101,  102,  103,  141,  142,
 /*  3300 */   143,  144,  145,  146,  147,  148,  149,  150,  151,  152,
 /*  3310 */   153,  154,  155,  156,  157,  158,  159,  160,  112,  187,
 /*  3320 */   187,  164,  187,    1,    2,    3,    4,    5,  187,  187,
 /*  3330 */   187,  174,  175,  176,  177,  187,  187,  187,  132,  187,
 /*  3340 */     1,    2,    3,    4,    5,  187,  187,  141,  142,  143,
 /*  3350 */   144,  145,  146,  147,  148,  149,  150,  151,  152,  153,
 /*  3360 */   154,  155,  156,  157,  158,  159,  160,  112,   46,   47,
 /*  3370 */   164,   49,   50,  187,   52,  187,  187,  187,   56,  187,
 /*  3380 */   174,  175,  176,  177,  187,   46,   47,  132,   49,   50,
 /*  3390 */   187,   52,  187,  187,  187,   56,  141,  142,  143,  144,
 /*  3400 */   145,  146,  147,  148,  149,  150,  151,  152,  153,  154,
 /*  3410 */   155,  156,  157,  158,  159,  160,  112,  187,  187,  164,
 /*  3420 */   187,   37,   38,  187,  187,  187,  187,  187,  187,  174,
 /*  3430 */   175,  176,  177,  187,  187,   51,  132,  187,    1,    2,
 /*  3440 */     3,    4,    5,  187,  187,  141,  142,  143,  144,  145,
 /*  3450 */   146,  147,  148,  149,  150,  151,  152,  153,  154,  155,
 /*  3460 */   156,  157,  158,  159,  160,  112,  187,  187,  164,   85,
 /*  3470 */    86,   87,   88,   89,  187,   91,   92,   93,  174,  175,
 /*  3480 */   176,  177,  187,   46,   47,  132,   49,   50,  187,   52,
 /*  3490 */   187,  187,  187,   56,  141,  142,  143,  144,  145,  146,
 /*  3500 */   147,  148,  149,  150,  151,  152,  153,  154,  155,  156,
 /*  3510 */   157,  158,  159,  160,  112,  187,  187,  164,  187,    1,
 /*  3520 */     2,    3,    4,    5,  187,  187,  187,  174,  175,  176,
 /*  3530 */   177,  187,  187,  187,  132,  187,    1,    2,    3,    4,
 /*  3540 */     5,  187,  187,  141,  142,  143,  144,  145,  146,  147,
 /*  3550 */   148,  149,  150,  151,  152,  153,  154,  155,  156,  157,
 /*  3560 */   158,  159,  160,  112,   46,   47,  164,   49,   50,  187,
 /*  3570 */    52,  187,  187,  187,   56,  187,  174,  175,  176,  177,
 /*  3580 */   187,   46,   47,  132,   49,   50,  187,  187,  187,  187,
 /*  3590 */   187,   56,  141,  142,  143,  144,  145,  146,  147,  148,
 /*  3600 */   149,  150,  151,  152,  153,  154,  155,  156,  157,  158,
 /*  3610 */   159,  160,  112,  187,  187,  164,  187,    1,    2,    3,
 /*  3620 */     4,    5,  187,  187,  187,  174,  175,  176,  177,  187,
 /*  3630 */   187,  187,  132,  187,    1,    2,    3,    4,    5,  187,
 /*  3640 */   187,  141,  142,  143,  144,  145,  146,  147,  148,  149,
 /*  3650 */   150,  151,  152,  153,  154,  155,  156,  157,  158,  159,
 /*  3660 */   160,  112,   46,   47,  164,   49,   50,  187,  187,  187,
 /*  3670 */   187,  187,   56,  187,  174,  175,  176,  177,  187,   46,
 /*  3680 */    47,  132,   49,   50,  187,  187,  187,  187,  187,   56,
 /*  3690 */   141,  142,  143,  144,  145,  146,  147,  148,  149,  150,
 /*  3700 */   151,  152,  153,  154,  155,  156,  157,  158,  159,  160,
 /*  3710 */   112,  187,  187,  164,  187,    1,    2,    3,    4,    5,
 /*  3720 */   187,  187,  187,  174,  175,  176,  177,  187,  187,  187,
 /*  3730 */   132,  187,    1,    2,    3,    4,    5,  187,  187,  141,
 /*  3740 */   142,  143,  144,  145,  146,  147,  148,  149,  150,  151,
 /*  3750 */   152,  153,  154,  155,  156,  157,  158,  159,  160,  112,
 /*  3760 */    46,   47,  164,   49,   50,  187,  187,  187,  187,  187,
 /*  3770 */    56,  187,  174,  175,  176,  177,  187,   46,   47,  132,
 /*  3780 */    49,   50,  187,  187,  187,  187,  187,   56,  141,  142,
 /*  3790 */   143,  144,  145,  146,  147,  148,  149,  150,  151,  152,
 /*  3800 */   153,  154,  155,  156,  157,  158,  159,  160,  112,  187,
 /*  3810 */   187,  164,  187,    1,    2,    3,    4,    5,  187,  187,
 /*  3820 */   187,  174,  175,  176,  177,  187,  187,  187,  132,  187,
 /*  3830 */     1,    2,    3,    4,    5,  187,  187,  141,  142,  143,
 /*  3840 */   144,  145,  146,  147,  148,  149,  150,  151,  152,  153,
 /*  3850 */   154,  155,  156,  157,  158,  159,  160,  112,   46,   47,
 /*  3860 */   164,   49,   50,  187,  187,  187,  187,  187,   56,   40,
 /*  3870 */   174,  175,  176,  177,  187,   46,   47,  132,    1,    2,
 /*  3880 */     3,    4,    5,   54,  187,   56,  141,  142,  143,  144,
 /*  3890 */   145,  146,  147,  148,  149,  150,  151,  152,  153,  154,
 /*  3900 */   155,  156,  157,  158,  159,  160,  112,  187,  187,  164,
 /*  3910 */   187,    1,    2,    3,    4,    5,  187,   40,  187,  174,
 /*  3920 */   175,  176,  177,   46,   47,  187,  132,  187,  187,  187,
 /*  3930 */   187,   54,  187,   56,  187,  141,  142,  143,  144,  145,
 /*  3940 */   146,  147,  148,  149,  150,  151,  152,  153,  154,  155,
 /*  3950 */   156,  157,  158,  159,  160,  112,   46,   47,  164,   49,
 /*  3960 */    50,  187,  187,  187,  187,  187,   56,  187,  174,  175,
 /*  3970 */   176,  177,  187,  187,  187,  132,  187,    1,    2,    3,
 /*  3980 */     4,    5,  187,  187,  141,  142,  143,  144,  145,  146,
 /*  3990 */   147,  148,  149,  150,  151,  152,  153,  154,  155,  156,
 /*  4000 */   157,  158,  159,  160,  112,  187,  187,  164,  187,    1,
 /*  4010 */     2,    3,    4,    5,  187,  187,  187,  174,  175,  176,
 /*  4020 */   177,  187,   46,   47,  132,   49,   50,  187,  187,  187,
 /*  4030 */   187,  187,   56,  141,  142,  143,  144,  145,  146,  147,
 /*  4040 */   148,  149,  150,  151,  152,  153,  154,  155,  156,  157,
 /*  4050 */   158,  159,  160,  112,   46,   47,  164,  187,  187,  187,
 /*  4060 */    52,  187,  187,  187,   56,  187,  174,  175,  176,  177,
 /*  4070 */   187,  187,  187,  132,  187,    1,    2,    3,    4,    5,
 /*  4080 */   187,  187,  141,  142,  143,  144,  145,  146,  147,  148,
 /*  4090 */   149,  150,  151,  152,  153,  154,  155,  156,  157,  158,
 /*  4100 */   159,  160,  112,  187,  187,  164,  187,    1,    2,    3,
 /*  4110 */     4,    5,  187,  187,  187,  174,  175,  176,  177,  187,
 /*  4120 */    46,   47,  132,    1,    2,    3,    4,    5,   54,  187,
 /*  4130 */    56,  141,  142,  143,  144,  145,  146,  147,  148,  149,
 /*  4140 */   150,  151,  152,  153,  154,  155,  156,  157,  158,  159,
 /*  4150 */   160,  112,   46,   47,  164,  187,  187,  187,  187,  187,
 /*  4160 */    54,  187,   56,  187,  174,  175,  176,  177,   46,   47,
 /*  4170 */   187,  132,  187,  187,  187,  187,   54,  187,   56,  187,
 /*  4180 */   141,  142,  143,  144,  145,  146,  147,  148,  149,  150,
 /*  4190 */   151,  152,  153,  154,  155,  156,  157,  158,  159,  160,
 /*  4200 */   112,  187,  187,  164,  187,  187,  187,  187,   40,  187,
 /*  4210 */   187,  187,  187,  174,  175,  176,  177,  187,  187,  187,
 /*  4220 */   132,   53,  187,  187,  187,  187,  187,  187,  187,  141,
 /*  4230 */   142,  143,  144,  145,  146,  147,  148,  149,  150,  151,
 /*  4240 */   152,  153,  154,  155,  156,  157,  158,  159,  160,  112,
 /*  4250 */   187,  187,  164,  187,  187,  187,  187,  187,   90,  187,
 /*  4260 */   187,  187,  174,  175,  176,  177,   98,   99,  100,  101,
 /*  4270 */   102,  103,  187,  187,  187,  187,  187,  187,  141,  142,
 /*  4280 */   143,  144,  145,  146,  147,  148,  149,  150,  151,  152,
 /*  4290 */   153,  154,  155,  156,  157,  158,  159,  160,  187,  187,
 /*  4300 */   187,  164,  187,    7,    8,    9,   10,   11,  187,  187,
 /*  4310 */   187,  174,  175,  176,  177,   19,  187,  187,  187,  187,
 /*  4320 */   187,  187,  187,  187,  187,  187,  187,   31,   32,   33,
 /*  4330 */    34,   35,   36,   37,  187,  187,   40,  187,    1,    2,
 /*  4340 */     3,    4,    5,  187,   48,  187,  187,  164,  187,   53,
 /*  4350 */   187,   55,  187,  187,  187,  187,  187,  174,  175,  176,
 /*  4360 */   177,  187,   12,   13,   14,  182,  183,   17,   18,   19,
 /*  4370 */    20,   21,   22,   23,   24,   25,   26,   27,   28,   29,
 /*  4380 */    30,  187,  187,   46,   47,  187,   90,   37,   38,  187,
 /*  4390 */   187,   54,  187,   56,   98,   99,  100,  101,  102,  103,
 /*  4400 */   164,  187,    7,    8,    9,   10,   11,  187,   58,  187,
 /*  4410 */   174,  175,  176,  177,   19,  187,  187,  181,  187,  187,
 /*  4420 */   184,  185,  186,  187,  187,  187,   31,   32,   33,   34,
 /*  4430 */    35,  187,   37,  187,  164,   40,  187,  187,  187,  187,
 /*  4440 */   187,  187,  187,   48,  174,  175,  176,  177,   53,  187,
 /*  4450 */    55,  187,  182,  183,  187,  187,   12,   13,   14,  187,
 /*  4460 */   187,   17,   18,   19,   20,   21,   22,   23,   24,   25,
 /*  4470 */    26,   27,   28,   29,   30,  187,  187,  164,   83,  187,
 /*  4480 */   187,   37,   38,  187,  187,   90,  187,  174,  175,  176,
 /*  4490 */   177,  187,   48,   98,   99,  100,  101,  102,  103,  186,
 /*  4500 */   187,    7,    8,    9,   10,   11,  187,  187,  187,  187,
 /*  4510 */   187,  187,  187,   19,  187,  187,  187,  187,  187,  187,
 /*  4520 */   187,  187,  187,  187,  187,   31,   32,   33,   34,   35,
 /*  4530 */   187,   37,  187,  187,   40,  187,  187,  187,  187,  187,
 /*  4540 */   187,  187,   48,  187,  187,  187,  187,   53,  187,   55,
 /*  4550 */   187,  187,   58,  187,  187,   12,   13,   14,  187,  187,
 /*  4560 */    17,   18,   19,   20,   21,   22,   23,   24,   25,   26,
 /*  4570 */    27,   28,   29,   30,  187,  187,  187,  187,  187,  187,
 /*  4580 */    37,   38,  187,  187,   90,  187,  187,  187,  187,  187,
 /*  4590 */   187,   48,   98,   99,  100,  101,  102,  103,  187,  187,
 /*  4600 */     7,    8,    9,   10,   11,  187,  187,  187,  187,  187,
 /*  4610 */   187,  187,   19,  187,  187,  187,  187,  187,  187,  187,
 /*  4620 */   187,  187,  187,  187,   31,   32,   33,   34,   35,   36,
 /*  4630 */    37,  187,  164,   40,  187,  187,  187,  187,  187,  187,
 /*  4640 */   187,   48,  174,  175,  176,  177,   53,  187,   55,  187,
 /*  4650 */   182,  183,  187,  187,   12,   13,   14,  187,  187,   17,
 /*  4660 */    18,   19,   20,   21,   22,   23,   24,   25,   26,   27,
 /*  4670 */    28,   29,   30,    1,    2,    3,    4,    5,  187,   37,
 /*  4680 */    38,  187,  187,   90,  187,  187,  187,  187,  187,  187,
 /*  4690 */    48,   98,   99,  100,  101,  102,  103,  187,  187,    7,
 /*  4700 */     8,    9,   10,   11,  187,  187,  187,  187,  187,  187,
 /*  4710 */   187,   19,  187,  187,  187,  187,  187,  187,   46,   47,
 /*  4720 */   187,  187,  187,   31,   32,   33,   34,   35,   56,   37,
 /*  4730 */   187,  164,   40,   41,  187,  187,  187,  187,  187,  187,
 /*  4740 */    48,  174,  175,  176,  177,   53,  187,   55,  187,  182,
 /*  4750 */   183,  187,  187,   12,   13,   14,  187,  187,   17,   18,
 /*  4760 */    19,   20,   21,   22,   23,   24,   25,   26,   27,   28,
 /*  4770 */    29,   30,  187,  187,  187,  187,  187,  187,   37,   38,
 /*  4780 */   187,  187,   90,  187,  187,  187,  187,  187,  187,   48,
 /*  4790 */    98,   99,  100,  101,  102,  103,  187,  187,    7,    8,
 /*  4800 */     9,   10,   11,  187,  187,  187,  187,  187,  164,  187,
 /*  4810 */    19,  187,  187,  187,  187,  187,  187,  187,  174,  175,
 /*  4820 */   176,  177,   31,   32,   33,   34,   35,  183,   37,  187,
 /*  4830 */   164,   40,  187,  187,  187,  187,  187,  187,  187,   48,
 /*  4840 */   174,  175,  176,  177,   53,  187,   55,  187,  187,   58,
 /*  4850 */   184,  185,  186,  187,  187,  187,  187,   12,   13,   14,
 /*  4860 */   187,  187,   17,   18,   19,   20,   21,   22,   23,   24,
 /*  4870 */    25,   26,   27,   28,   29,   30,  187,  187,  187,  187,
 /*  4880 */   187,   90,   37,   38,  187,  187,   41,  187,  187,   98,
 /*  4890 */    99,  100,  101,  102,  103,  187,  187,    7,    8,    9,
 /*  4900 */    10,   11,  187,  187,  187,  187,  187,  187,  187,   19,
 /*  4910 */   187,  187,  187,  187,  187,  187,  187,  187,  187,  187,
 /*  4920 */   187,   31,   32,   33,   34,   35,   36,   37,  187,  164,
 /*  4930 */    40,  187,  187,  187,  187,  187,  187,  187,   48,  174,
 /*  4940 */   175,  176,  177,   53,  187,   55,  187,  182,  183,  187,
 /*  4950 */   187,  187,  187,  187,  187,  187,   12,   13,   14,  187,
 /*  4960 */   187,   17,   18,   19,   20,   21,   22,   23,   24,   25,
 /*  4970 */    26,   27,   28,   29,   30,  187,  187,  187,  187,  187,
 /*  4980 */    90,   37,   38,  187,  187,  187,  187,  187,   98,   99,
 /*  4990 */   100,  101,  102,  103,  187,  187,    7,    8,    9,   10,
 /*  5000 */    11,  187,   58,  187,  187,  187,  187,  187,   19,  187,
 /*  5010 */   187,  187,  187,  187,  187,  187,  187,  187,  187,  187,
 /*  5020 */    31,   32,   33,   34,   35,  187,   37,  187,  187,   40,
 /*  5030 */   187,  187,  187,  187,  187,  187,  187,   48,  187,  187,
 /*  5040 */   164,  187,   53,  187,   55,  187,  187,   58,  187,  187,
 /*  5050 */   174,  175,  176,  177,  187,   12,   13,   14,  182,  183,
 /*  5060 */    17,   18,   19,   20,   21,   22,   23,   24,   25,   26,
 /*  5070 */    27,   28,   29,   30,  187,  187,  187,  187,  187,   90,
 /*  5080 */    37,   38,  187,  187,  187,  187,  187,   98,   99,  100,
 /*  5090 */   101,  102,  103,  187,  187,    7,    8,    9,   10,   11,
 /*  5100 */   187,   58,  187,  187,  187,  187,  187,   19,  187,  187,
 /*  5110 */   187,  187,  187,  187,  187,  187,  187,  187,  187,   31,
 /*  5120 */    32,   33,   34,   35,   36,   37,  187,  164,   40,  187,
 /*  5130 */   187,  187,  187,  187,  187,  187,   48,  174,  175,  176,
 /*  5140 */   177,   53,  187,   55,  187,  182,  183,  187,  187,  187,
 /*  5150 */   187,  187,  187,  187,   12,   13,   14,  187,  187,   17,
 /*  5160 */    18,   19,   20,   21,   22,   23,   24,   25,   26,   27,
 /*  5170 */    28,   29,   30,  187,  187,  187,  187,  187,   90,   37,
 /*  5180 */    38,  187,  187,   41,  187,  187,   98,   99,  100,  101,
 /*  5190 */   102,  103,  187,  187,    7,    8,    9,   10,   11,  187,
 /*  5200 */   187,  187,  187,  187,  187,  187,   19,  187,  187,  187,
 /*  5210 */   187,  187,  187,  187,  187,  187,  187,  187,   31,   32,
 /*  5220 */    33,   34,   35,   36,   37,  187,  164,   40,  187,  187,
 /*  5230 */   187,  187,  187,  187,  187,   48,  174,  175,  176,  177,
 /*  5240 */    53,  187,   55,  187,  182,  183,  187,  187,  187,  187,
 /*  5250 */   187,  187,  187,   12,   13,   14,  187,  187,   17,   18,
 /*  5260 */    19,   20,   21,   22,   23,   24,   25,   26,   27,   28,
 /*  5270 */    29,   30,  187,  187,  187,  187,  187,   90,   37,   38,
 /*  5280 */   187,  187,  187,  187,  187,   98,   99,  100,  101,  102,
 /*  5290 */   103,  187,  187,    7,    8,    9,   10,   11,  187,   58,
 /*  5300 */   187,  187,  187,  187,  187,   19,  187,  187,  187,  187,
 /*  5310 */   187,  187,  187,  187,  187,  187,  187,   31,   32,   33,
 /*  5320 */    34,   35,   36,   37,  187,  164,   40,  187,  187,  187,
 /*  5330 */   187,  187,  187,  187,   48,  174,  175,  176,  177,   53,
 /*  5340 */   187,   55,  187,  182,  183,  187,  187,   12,   13,   14,
 /*  5350 */   187,  187,   17,   18,   19,   20,   21,   22,   23,   24,
 /*  5360 */    25,   26,   27,   28,   29,   30,  187,  187,  187,  187,
 /*  5370 */   187,   36,   37,   38,  187,  187,   90,  187,  187,  187,
 /*  5380 */   187,  187,  187,  187,   98,   99,  100,  101,  102,  103,
 /*  5390 */   187,  187,    7,    8,    9,   10,   11,  187,  187,  187,
 /*  5400 */   187,  187,  187,  187,   19,  187,  187,  187,  187,  187,
 /*  5410 */   187,  187,  187,  187,  187,  187,   31,   32,   33,   34,
 /*  5420 */    35,  187,   37,  187,  187,   40,  187,  187,  187,  187,
 /*  5430 */   187,  187,  187,   48,  187,  187,  187,  187,   53,  187,
 /*  5440 */    55,  187,  187,   58,  187,  187,   12,   13,   14,  187,
 /*  5450 */   187,   17,   18,   19,   20,   21,   22,   23,   24,   25,
 /*  5460 */    26,   27,   28,   29,   30,  187,  187,  187,  187,  187,
 /*  5470 */   187,   37,   38,  187,  187,   90,  187,  187,  187,  187,
 /*  5480 */   187,  187,  187,   98,   99,  100,  101,  102,  103,  187,
 /*  5490 */   187,    7,    8,    9,   10,   11,  187,  187,  187,  187,
 /*  5500 */   187,  187,  187,   19,  187,  187,  187,  187,  187,  187,
 /*  5510 */   187,  187,  187,  187,  187,   31,   32,   33,   34,   35,
 /*  5520 */    36,   37,  187,  164,   40,  187,  187,  187,  187,  187,
 /*  5530 */   187,  187,   48,  174,  175,  176,  177,   53,  187,   55,
 /*  5540 */   187,  182,  183,  187,  187,  187,   13,   14,  187,  187,
 /*  5550 */    17,   18,   19,   20,   21,   22,   23,   24,   25,   26,
 /*  5560 */    27,   28,   29,   30,  187,  187,  187,  187,  187,  187,
 /*  5570 */    37,   38,  187,  187,   90,  187,  187,  187,  187,  187,
 /*  5580 */   187,  187,   98,   99,  100,  101,  102,  103,  187,  187,
 /*  5590 */     7,    8,    9,   10,   11,  187,  187,  187,  187,  187,
 /*  5600 */   187,  187,   19,  187,  187,  187,  187,  187,   37,  187,
 /*  5610 */   187,  187,  187,  187,   31,   32,   33,   34,   35,   36,
 /*  5620 */    37,  187,   51,   40,  187,  187,  187,  187,  187,  187,
 /*  5630 */   187,   48,  187,  187,  187,  187,   53,  187,   55,  187,
 /*  5640 */   187,   17,   18,   19,   20,   21,   22,   23,   24,   25,
 /*  5650 */    26,   27,   28,   29,   30,  187,   85,   86,   87,   88,
 /*  5660 */    89,   37,   38,   92,   93,  187,  187,  187,  187,  187,
 /*  5670 */   187,  187,  187,   90,  187,  187,  187,  187,  187,  187,
 /*  5680 */   187,   98,   99,  100,  101,  102,  103,  187,  187,    7,
 /*  5690 */     8,    9,   10,   11,  187,  187,  187,  187,  187,  187,
 /*  5700 */   187,   19,  187,   19,  187,  187,  187,  187,  187,  187,
 /*  5710 */   187,  187,  187,   31,   32,   33,   34,   35,   36,   37,
 /*  5720 */    36,   19,   40,  187,   40,  187,  187,  187,  187,  187,
 /*  5730 */    48,  187,  187,  187,  187,   53,  187,   55,   36,  187,
 /*  5740 */   187,  187,   40,   59,   60,   61,   62,   63,   64,   65,
 /*  5750 */    66,   67,   68,   69,  187,  187,  187,  187,  187,  187,
 /*  5760 */   187,   59,   60,   61,   62,   63,   64,   65,   66,   67,
 /*  5770 */    68,   69,   90,  187,  187,  187,  187,  187,  187,  187,
 /*  5780 */    98,   99,  100,  101,  102,  103,  187,  187,    7,    8,
 /*  5790 */     9,   10,   11,  187,  187,  187,  187,  187,  187,  187,
 /*  5800 */    19,  187,   19,  187,  187,  187,  187,  187,  187,  187,
 /*  5810 */   187,  187,   31,   32,   33,   34,   35,  187,   37,   36,
 /*  5820 */   187,   40,  187,   40,  187,  187,  187,  187,  187,   48,
 /*  5830 */   187,  187,  187,  187,   53,  187,   55,  187,  187,  187,
 /*  5840 */   187,  187,   59,   60,   61,   62,   63,   64,   65,   66,
 /*  5850 */    67,   68,   69,  187,  187,  187,  187,  187,  187,  187,
 /*  5860 */   187,  187,  187,  187,   83,  187,  187,  187,  187,  187,
 /*  5870 */   187,   90,  187,  187,  187,  187,  187,  187,  187,   98,
 /*  5880 */    99,  100,  101,  102,  103,  187,  187,    7,    8,    9,
 /*  5890 */    10,   11,  187,  187,  187,  187,  187,  187,  187,   19,
 /*  5900 */   187,   19,  187,  187,  187,  187,  187,  187,  187,  187,
 /*  5910 */   187,   31,   32,   33,   34,   35,   36,   37,   36,   19,
 /*  5920 */    40,  187,   40,  187,  187,  187,  187,  187,   48,  187,
 /*  5930 */   187,  187,  187,   53,  187,   55,  187,  187,  187,  187,
 /*  5940 */    40,   59,   60,   61,   62,   63,   64,   65,   66,   67,
 /*  5950 */    68,   69,  187,  187,  187,  187,  187,  187,  187,   59,
 /*  5960 */    60,   61,   62,   63,   64,   65,   66,   67,   68,   69,
 /*  5970 */    90,  187,  187,  187,  187,  187,  187,  187,   98,   99,
 /*  5980 */   100,  101,  102,  103,  187,  187,    7,    8,    9,   10,
 /*  5990 */    11,  187,  187,  187,  187,  187,  187,  187,   19,   19,
 /*  6000 */   187,  187,  187,  187,  187,  187,  187,  187,  187,  187,
 /*  6010 */    31,   32,   33,   34,   35,  187,   37,  187,  187,   40,
 /*  6020 */   187,  187,  187,  187,  187,  187,  187,   48,  187,  187,
 /*  6030 */   187,  187,   53,  187,   55,  187,  187,  187,  187,   59,
 /*  6040 */    60,   61,   62,   63,   64,   65,   66,   67,   68,   69,
 /*  6050 */   187,  187,  187,  187,  187,  187,  187,  187,  187,  187,
 /*  6060 */   187,  187,  187,  187,  187,  187,  187,  187,  187,   90,
 /*  6070 */   187,  187,  187,  187,  187,  187,  187,   98,   99,  100,
 /*  6080 */   101,  102,  103,  187,  187,    7,    8,    9,   10,   11,
 /*  6090 */   187,  187,  187,  187,  187,  187,  187,   19,  187,  187,
 /*  6100 */   187,  187,  187,  187,  187,  187,  187,  187,  187,   31,
 /*  6110 */    32,   33,   34,   35,  187,   37,  187,  187,   40,  187,
 /*  6120 */   187,  187,  187,  187,  187,  187,   48,  187,  187,  187,
 /*  6130 */   187,   53,  187,   55,  187,  187,  187,  187,  187,  187,
 /*  6140 */   187,  187,  187,  187,  187,  187,  187,  187,  187,  187,
 /*  6150 */   187,  187,  187,  187,  187,  187,  187,  187,  187,  187,
 /*  6160 */   187,  187,  187,  187,  187,  187,  187,  187,   90,  187,
 /*  6170 */   187,  187,  187,  187,  187,  187,   98,   99,  100,  101,
 /*  6180 */   102,  103,
};
#define YY_SHIFT_USE_DFLT (-30)
static short yy_shift_ofst[] = {
 /*     0 */  1519,   78,  357,  -30,  -30,  -30,  -30,  -30,  -11,   87,
 /*    10 */   -30,  127,  230,  -30,  143,  274,  -30, 3633,  -30, 3714,
 /*    20 */   -30,  -30, 4074,  492,  508, 5684,  591, 3214,  563,  -30,
 /*    30 */   -30,  643, 5980,  -30,  592,  -30,  -30,  -30,  593,   66,
 /*    40 */   -30,  650,  706,  -30,  -30,  -30,  -30,  -30,  -30,  -30,
 /*    50 */   -30,  -30,  -30,  -30,  -30,  -30,  -30,  875,  767, 3214,
 /*    60 */   716,  -30,  -30, 5900,  -30,  696, 4168,  -30,  -30,  -30,
 /*    70 */   -30,  -30,  -30,  -30,  -30,  725,  770,  -30,  -30,  780,
 /*    80 */   774, 4168,  -30,  787,  793, 4168,  -30,  820,  810, 4168,
 /*    90 */   -30,  847,  873,  -30,  -30,  -30,  -30,  -30,  -30,  -30,
 /*   100 */   -30,  -30,  -30, 4672, 4337,  854,  831, 5702,  484, 3214,
 /*   110 */   855,  -30,  -30,  330,  466, 3214,  861,  -30,  -30,  -30,
 /*   120 */   -30,  864,  448,  -30,  865,  447,  -30,  869,  391,  -30,
 /*   130 */  3339,  -30,  863,  -30, 3518, 3910,  -30, 4106,  877,  858,
 /*   140 */  5882,  387, 1448,  -30,  187,  -30,  -30,  -30,  -30,  -30,
 /*   150 */   -30,  -30,  -30,  -30,  -30,  -30,  -30,  -30,  -30,  -30,
 /*   160 */   -30,  -30,  -30,  -30,  -30,  -30,  -30,  -30,  879,  -30,
 /*   170 */   881,  -30, 5979,  870, 1351,  478,  850,  876,  381,  866,
 /*   180 */   -30, 5979,  886,   69,  -30,  318,  -30,  -30, 4168,  872,
 /*   190 */  3097, 3097,  891, 2224, 1933,  -30, 5979,  901, 2806,  963,
 /*   200 */   -30,  907,   90, 1254,  878, 5979,  917,  -30, 5979,  144,
 /*   210 */  5979,  919,  -30,  188, 5979, 5979, 5434, 5979, 5979, 5434,
 /*   220 */  5979, 5979, 5434, 5979, 5979, 5434, 5979, 5979, 5434, 5979,
 /*   230 */   -20, 5979,  -20, 5979,  -20,  283,  921, 5880, 5434, 5979,
 /*   240 */  4350,  -30, 5979, 1559, 5979, 1559, 5979,  144, 5979,  144,
 /*   250 */  5979,  144, 5979,  276, 5979, 5624, 5979, 5624, 5979, 5533,
 /*   260 */  3194, 5335,  -30,  942, 5979,  144,  940,  961, 5979,  -20,
 /*   270 */   417,  477,  928, 4890,  944,  983,  919,  -30,  -30, 5979,
 /*   280 */   144,  -30,  -30,  -30,  -30,  -30,  -30,  -30,  -30, 4989,
 /*   290 */  5434,  285, 5286,  232,  935, 4296,  -30,   43,  -30, 5979,
 /*   300 */   951,  950,   29, 5682,  846,  -30,  -30,  -30,  -30,  -30,
 /*   310 */   -30,  612,  938, 5484,  815,  -30,  -30,  962,  952,  953,
 /*   320 */  5583,  801,  -30,  -30,  -30,  965,  957,  954, 4593,  -30,
 /*   330 */   776,  -30,  704,  -30,  -30,  931,  934,  -30,  644,  -30,
 /*   340 */  6078,  -30,  939, 5979,  -30,  -30,  -30,  693,  -30,  -30,
 /*   350 */   -30,  679,  -30,  -30,  603,  968,  959, 5088,  590,  -30,
 /*   360 */   -30,  970,  966, 5187,  500,  -30,  -30,  -20,  -20,  -20,
 /*   370 */   -20, 4944,  -30,  476,  -30,  465,  982,  -30,  987,  -30,
 /*   380 */   -20, 5434,  980,  523, 5781, 4543, 3097,   -7,  -30,  999,
 /*   390 */   432,  -30,  999,  -30, 3384,  -30,  -30,  -30,  -30,  -30,
 /*   400 */   -30, 5979,  -30, 5434,  426, 5571, 5979,  -30, 4791,  115,
 /*   410 */  5979,  -30,  997,  115, 5979,  -30, 5241,  -30,  -30,  165,
 /*   420 */   991,  115, 5979,  -30,  996,  115, 5979,  -30,  -29,  128,
 /*   430 */  5979,  -30, 4494,  115, 5979,  -30, 1016,  115, 5979,  -30,
 /*   440 */  5385,  115, 5979,  -30, 1017,  -30,  223, 5979,  -30,  -30,
 /*   450 */  5979,  -30,  -30,  -30,  -30, 5979,  458,  -30, 5979,  -30,
 /*   460 */  5434,  -30, 1045,  -30, 1047,  -30, 1048,  -30, 1049,  -30,
 /*   470 */  4692, 4845,  -30,  -30, 1056,  395, 1058, 1061,  -30, 5979,
 /*   480 */  5043, 1062,  -30, 5979, 5142,  -30, 1059,  528,  -30, 1059,
 /*   490 */   -30, 1054, 4168,  -30,  -30, 1059,  547,  -30, 1059,  616,
 /*   500 */   -30, 1059,  625,  -30, 1059,  708,  -30, 1059,  713,  -30,
 /*   510 */  1059,  722,  -30, 1059,  724,  -30, 1059,  749,  -30, 1059,
 /*   520 */   805,  -30, 5434,  -30,  -30, 5979, 4642, 3097, 1545,  -30,
 /*   530 */  1066, 1025, 4395, 4444, 3097, 1060,  -30, 5979, 4741, 3097,
 /*   540 */  2418,  -30, 1032, 5979, 1073,  -30,  -30,  -30, 1040, 3097,
 /*   550 */  3097,  -30,  -30, 1044, 1070, 2127, 3000,  -30,  -30, 3214,
 /*   560 */  1080, 2903,  -30, 2709,  -30,  856,   81, 2030,  -30, 1836,
 /*   570 */   -30, 3214, 1085, 1157,  -30,  672,  -30, 4672, 4122, 1096,
 /*   580 */  1082, 5783,  259, 1642,  -30,  284,  -30, 3214, 1094, 1739,
 /*   590 */   -30, 2321,  -30,   25,  304, 2515,  -30, 2612,  -30, 3214,
 /*   600 */  1095,  575,  -30,  769,  -30, 3437, 3535,  -30,  -30, 4008,
 /*   610 */  1098, 1101, 4168, 1108,  -30, 1117, 1120, 4168, 1131,  -30,
 /*   620 */   -30, 4008, 3877,  287,  -30, 4168,  182,  -30, 1142,  -30,
 /*   630 */   123, 1143,  388, 1144,  108,  -30,  -30, 1146,  -30,  -30,
 /*   640 */  1152,  -30, 3829,  228,  -30, 4168,  670,  -30, 1154,  -30,
 /*   650 */  1160,  -30,  -30, 3322, 3976, 3616, 4672, 3812,  -30, 3731,
 /*   660 */   869,  -30,  -30,  -30,  869,  391,  -30, 1164, 1169,   37,
 /*   670 */   -30, 1171, 1166,  -30,  869,  391,  -30, 1186, 1175,  836,
 /*   680 */   -30, 1194, 1166,  -30,  -30,
};
#define YY_REDUCE_USE_DFLT (-124)
static short yy_reduce_ofst[] = {
 /*     0 */     5, -124,  -89, -124, -124, -124, -124, -124, -124, -124,
 /*    10 */  -124, -124,   64, -124, -124,  246, -124,  111, -124,  -83,
 /*    20 */  -124, -124,  315, -124, -124,  383, -124,  656, -124, -124,
 /*    30 */  -124, -124, -123, -124, -124, -124, -124, -124, -124, -124,
 /*    40 */  -124, -124, -124, -124, -124, -124, -124, -124, -124, -124,
 /*    50 */  -124, -124, -124, -124, -124, -124, -124, -124, -124,  462,
 /*    60 */  -124, -124, -124,   38, -124, -124,  640, -124, -124, -124,
 /*    70 */  -124, -124, -124, -124, -124, -124, -124, -124, -124, -124,
 /*    80 */  -124,  702, -124, -124, -124,  730, -124, -124, -124,  756,
 /*    90 */  -124, -124, -124, -124, -124, -124, -124, -124, -124, -124,
 /*   100 */  -124, -124, -124,  412,  315, -124, -124,  286, -124,  559,
 /*   110 */  -124, -124, -124, -124, -124,  648, -124, -124, -124, -124,
 /*   120 */  -124, -124,  781, -124, -124,  794, -124,  341,  796, -124,
 /*   130 */   -84, -124, -124, -124,   13,    3, -124,  315, -124, -124,
 /*   140 */   480, -124, 3549, -124, 4137, -124, -124, -124, -124, -124,
 /*   150 */  -124, -124, -124, -124, -124, -124, -124, -124, -124, -124,
 /*   160 */  -124, -124, -124, -124, -124, -124, -124, -124, -124, -124,
 /*   170 */  -124, -124,  390, -124, 3696, 4137, -124, -124, 3451, 4137,
 /*   180 */  -124,  487, -124,  651, -124,  779, -124, -124,  819, -124,
 /*   190 */  3402, 4137, -124, 3353, 4137, -124,  584, -124, 3843, 4137,
 /*   200 */  -124, -124, 3647, 4137, -124,  681, -124, -124,  971, -124,
 /*   210 */  1165,  797, -124, -124, 1747, 1941, -124, 2329, 2911, -124,
 /*   220 */   234, 2557, -124, 2460, 3008, -124, 2523, 2135, -124, 1844,
 /*   230 */  -124, 1456, -124, 1553, -124, -124, -124, 4963, -124, 1359,
 /*   240 */  -124, -124, 1262, -124, 1068, -124,  874, -124, 2945, -124,
 /*   250 */  2814, -124, 2751, -124, 2654, -124, 2717, -124, 3042, -124,
 /*   260 */   326, -124, -124, -124, 1650, -124, -124, -124, 2266, -124,
 /*   270 */  -124, -124, -124, 5161, -124, -124,  812, -124, -124, 2232,
 /*   280 */  -124, -124, -124, -124, -124, -124, -124, -124, -124, 4236,
 /*   290 */  -124, -124, 4270, -124, -124, 4468, -124, -124, -124, 4644,
 /*   300 */  -124, -124, -124, 4183, -124, -124, -124, -124, -124, -124,
 /*   310 */  -124, -124, -124, 4876, -124, -124, -124, -124, -124, -124,
 /*   320 */  5062, -124, -124, -124, -124, -124, -124, -124, 5359, -124,
 /*   330 */  -124, -124, -124, -124, -124, -124, -124, -124, -124, -124,
 /*   340 */  4666, -124, -124, 4313, -124, -124, -124, -124, -124, -124,
 /*   350 */  -124, -124, -124, -124, -124, -124, -124, 4765, -124, -124,
 /*   360 */  -124, -124, -124, 4567, -124, -124, -124, -124, -124, -124,
 /*   370 */  -124, -124, -124, -124, -124, -124, -124, -124, -124, -124,
 /*   380 */  -124, -124, -124, -124, 2620, -124, 3500, 4137, -124,  254,
 /*   390 */  -124, -124,  880, -124,  235, -124, -124, -124, -124, -124,
 /*   400 */  -124, 2072, -124, -124, -124,  885, 1975, -124, 1878,  887,
 /*   410 */  1781, -124, -124,  889, 1684, -124, -124, -124, -124, -124,
 /*   420 */  -124,  906, 1587, -124, -124,  910, 1490, -124, -124,  911,
 /*   430 */  1296, -124, 1199,  913, 1102, -124, -124,  914, 1005, -124,
 /*   440 */   714,  920,  908, -124, -124, -124,  327,  811, -124, -124,
 /*   450 */   714, -124, -124, -124, -124,   35, -124, -124,  196, -124,
 /*   460 */  -124, -124, -124, -124, -124, -124, -124, -124, -124, -124,
 /*   470 */  2426, -124, -124, -124, -124, -124, -124, -124, -124,  777,
 /*   480 */  -124, -124, -124, 2363, -124, -124,  382, -124, -124,  925,
 /*   490 */  -124, -124,  988, -124, -124,  392, -124, -124,  424, -124,
 /*   500 */  -124,  435, -124, -124,  489, -124, -124,  522, -124, -124,
 /*   510 */   538, -124, -124,  545, -124, -124,  618, -124, -124,  620,
 /*   520 */  -124, -124, -124, -124, -124, 2848, -124, 3598, 4137, -124,
 /*   530 */  -124, -124, 2038, -124, 3745, 4137, -124, 2169, -124, 3941,
 /*   540 */  4137, -124, -124,  293, -124, -124, -124, -124, -124, 4039,
 /*   550 */  4137, -124, -124, -124, -124, 3255, 4137, -124, -124,  551,
 /*   560 */  -124, 3157, -124, 4137, -124, -124, -124, 3304, -124, 4137,
 /*   570 */  -124,  454, -124, 3206, -124, 4137, -124,  109,  315, -124,
 /*   580 */  -124,  189, -124, 4088, -124, 4137, -124,  163, -124, 3990,
 /*   590 */  -124, 4137, -124, -124, -124, 3892, -124, 4137, -124,  -31,
 /*   600 */  -124, 3794, -124, 4137, -124,  110,    3, -124, -124,  109,
 /*   610 */  -124, -124, 1029, -124, -124, -124, -124, 1050, -124, -124,
 /*   620 */  -124,  207,  315, 1052, -124, 1051, 1053, -124, -124, -124,
 /*   630 */   301, -124, -124, -124, 1069, -124, -124, -124, -124, -124,
 /*   640 */  -124, -124,  315, 1074, -124, 1063, 1075, -124, -124, -124,
 /*   650 */  -124, -124, -124,  -53,  141,    3,  207,    3, -124,    3,
 /*   660 */  1086, -124, -124, -124,  260, 1088, -124, -124, -124, 1090,
 /*   670 */  -124, -124, 1099, -124,  504, 1111, -124, -124, -124, 1119,
 /*   680 */  -124, -124, 1125, -124, -124,
};
static YYACTIONTYPE yy_default[] = {
 /*     0 */  1012, 1012, 1012,  687,  689,  690,  691,  692, 1012, 1012,
 /*    10 */   693, 1012, 1012,  694, 1012, 1012,  695, 1012,  710, 1012,
 /*    20 */   711,  741, 1012, 1012, 1012, 1012, 1012, 1012, 1012,  761,
 /*    30 */   779,  780, 1012,  781,  783,  784,  785,  786, 1012, 1012,
 /*    40 */   797, 1012, 1012,  798,  799,  800,  801,  802,  803,  804,
 /*    50 */   805,  806,  807,  808,  809,  782,  765, 1012, 1012, 1012,
 /*    60 */  1012,  762,  766, 1012,  787,  789, 1012,  793,  997,  998,
 /*    70 */   999, 1000, 1001, 1002, 1003, 1012, 1012, 1004, 1005, 1012,
 /*    80 */   790, 1012,  794, 1012,  791, 1012,  795, 1012,  792, 1012,
 /*    90 */   796, 1012, 1012,  788,  769,  771,  772,  773,  774,  775,
 /*   100 */   776,  777,  778, 1012, 1012, 1012, 1012, 1012, 1012, 1012,
 /*   110 */  1012,  763,  767, 1012, 1012, 1012, 1012,  764,  768,  770,
 /*   120 */   742, 1012, 1012,  696, 1012, 1012,  697, 1012, 1012,  699,
 /*   130 */  1012,  705, 1012,  706, 1012, 1012,  739, 1012, 1012, 1012,
 /*   140 */  1012, 1012, 1012,  745, 1012,  747,  810,  812,  813,  814,
 /*   150 */   815,  816,  817,  818,  819,  820,  821,  822,  823,  824,
 /*   160 */   825,  826,  827,  828,  829,  830,  831,  832, 1012,  833,
 /*   170 */  1012,  834, 1012, 1012, 1012, 1012,  837, 1012, 1012, 1012,
 /*   180 */   838, 1012, 1012, 1012,  841, 1012,  842,  843, 1012, 1012,
 /*   190 */   845,  846, 1012, 1012, 1012,  849, 1012, 1012, 1012, 1012,
 /*   200 */   851, 1012, 1012, 1012, 1012, 1012, 1012,  853, 1012,  914,
 /*   210 */  1012, 1012,  915, 1012, 1012, 1012,  916, 1012, 1012,  917,
 /*   220 */  1012, 1012,  918, 1012, 1012,  919, 1012, 1012,  920, 1012,
 /*   230 */   926, 1012,  927, 1012,  928, 1012,  932, 1012,  988, 1012,
 /*   240 */  1012,  937, 1012,  938, 1012,  939, 1012,  940, 1012,  941,
 /*   250 */  1012,  942, 1012,  943, 1012,  944, 1012,  945, 1012,  946,
 /*   260 */  1012, 1012,  929, 1012, 1012,  930, 1012, 1012, 1012,  931,
 /*   270 */   950, 1012,  935, 1012, 1012, 1012, 1012,  947,  948, 1012,
 /*   280 */   949,  951,  952,  953,  954,  955,  956,  957,  958, 1012,
 /*   290 */   996,  950, 1012, 1012,  961, 1012,  962, 1012,  963, 1012,
 /*   300 */  1012, 1012, 1012, 1012, 1012,  969,  970,  983,  984,  985,
 /*   310 */   987, 1012, 1012, 1012, 1012,  973,  974, 1012, 1012, 1012,
 /*   320 */  1012, 1012,  975,  976,  986, 1012, 1012,  964, 1012,  965,
 /*   330 */  1012,  966, 1012,  967,  968,  951,  952,  959, 1012,  960,
 /*   340 */  1012,  989, 1012, 1012,  991,  992,  990, 1012,  971,  972,
 /*   350 */   936, 1012,  977,  978, 1012, 1012,  933, 1012, 1012,  979,
 /*   360 */   980, 1012,  934, 1012, 1012,  981,  982,  925,  924,  923,
 /*   370 */   922, 1012, 1006, 1012, 1007, 1012, 1012, 1008, 1012, 1009,
 /*   380 */   921, 1010, 1012, 1012, 1012, 1012, 1012, 1012,  854, 1012,
 /*   390 */  1012,  858, 1012,  859, 1012,  861,  862,  863,  864,  865,
 /*   400 */   866, 1012,  867,  913, 1012, 1012, 1012,  868, 1012, 1012,
 /*   410 */  1012,  871, 1012, 1012, 1012,  872, 1012,  881,  882, 1012,
 /*   420 */  1012, 1012, 1012,  869, 1012, 1012, 1012,  870, 1012, 1012,
 /*   430 */  1012,  873, 1012, 1012, 1012,  874, 1012, 1012, 1012,  875,
 /*   440 */  1012, 1012, 1012,  876, 1012,  880, 1012, 1012,  877,  878,
 /*   450 */  1012,  879,  883,  884,  860, 1012, 1012,  886, 1012,  887,
 /*   460 */   889,  888,  983,  890,  985,  891,  984,  892,  948,  893,
 /*   470 */  1012, 1012,  894,  895, 1012, 1012, 1012, 1012,  896, 1012,
 /*   480 */  1012, 1012,  897, 1012, 1012,  898, 1012, 1012,  899, 1012,
 /*   490 */   909,  911, 1012,  912,  910, 1012, 1012,  900, 1012, 1012,
 /*   500 */   901, 1012, 1012,  902, 1012, 1012,  903, 1012, 1012,  904,
 /*   510 */  1012, 1012,  905, 1012, 1012,  906, 1012, 1012,  907, 1012,
 /*   520 */  1012,  908, 1012, 1011,  811, 1012, 1012, 1012, 1012,  855,
 /*   530 */  1012, 1012, 1012, 1012, 1012, 1012,  856, 1012, 1012, 1012,
 /*   540 */  1012,  857, 1012, 1012, 1012,  852,  850,  848, 1012, 1012,
 /*   550 */   847,  844,  839,  835, 1012, 1012, 1012,  840,  836, 1012,
 /*   560 */  1012, 1012,  753, 1012,  755, 1012, 1012, 1012,  746, 1012,
 /*   570 */   748, 1012, 1012, 1012,  754, 1012,  756, 1012, 1012, 1012,
 /*   580 */  1012, 1012, 1012, 1012,  749, 1012,  751, 1012, 1012, 1012,
 /*   590 */   757, 1012,  759, 1012, 1012, 1012,  750, 1012,  752, 1012,
 /*   600 */  1012, 1012,  758, 1012,  760, 1012, 1012,  737,  740, 1012,
 /*   610 */  1012, 1012, 1012, 1012,  743, 1012, 1012, 1012, 1012,  744,
 /*   620 */   721, 1012, 1012, 1012,  723, 1012, 1012,  725, 1012,  729,
 /*   630 */  1012, 1012, 1012, 1012, 1012,  733,  735, 1012,  736,  734,
 /*   640 */  1012,  727, 1012, 1012,  724, 1012, 1012,  726, 1012,  730,
 /*   650 */  1012,  728,  738, 1012, 1012, 1012, 1012, 1012,  722, 1012,
 /*   660 */  1012,  707,  709,  708, 1012, 1012,  698, 1012, 1012, 1012,
 /*   670 */   700, 1012, 1012,  701, 1012, 1012,  702, 1012, 1012, 1012,
 /*   680 */   703, 1012, 1012,  704,  688,
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
 /* 247 */ "xx_common_expr ::= xx_common_expr ARROW IDENTIFIER",
 /* 248 */ "xx_common_expr ::= xx_common_expr ARROW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE",
 /* 249 */ "xx_common_expr ::= xx_common_expr ARROW BRACKET_OPEN STRING BRACKET_CLOSE",
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
 /* 292 */ "xx_mcall_expr ::= xx_common_expr ARROW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 293 */ "xx_mcall_expr ::= xx_common_expr ARROW IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 294 */ "xx_mcall_expr ::= xx_common_expr ARROW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 295 */ "xx_mcall_expr ::= xx_common_expr ARROW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 296 */ "xx_mcall_expr ::= xx_common_expr ARROW BRACKET_OPEN STRING BRACKET_CLOSE PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 297 */ "xx_mcall_expr ::= xx_common_expr ARROW BRACKET_OPEN STRING BRACKET_CLOSE PARENTHESES_OPEN PARENTHESES_CLOSE",
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
// 1001 "parser.lemon"
{
	/*if ((yypminor->yy0)) {
		if ((yypminor->yy0)->free_flag) {
			efree((yypminor->yy0)->token);
		}
		efree((yypminor->yy0));
	}*/
}
// 3132 "parser.c"
      break;
    case 106:
// 1014 "parser.lemon"
{ json_object_put((yypminor->yy115)); }
// 3137 "parser.c"
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
// 1010 "parser.lemon"
{
	status->ret = yymsp[0].minor.yy115;
}
// 3681 "parser.c"
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
// 1016 "parser.lemon"
{
	yygotominor.yy115 = yymsp[0].minor.yy115;
}
// 3721 "parser.c"
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
// 1020 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_list(yymsp[-1].minor.yy115, yymsp[0].minor.yy115);
}
// 3736 "parser.c"
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
// 1024 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_list(NULL, yymsp[0].minor.yy115);
}
// 3760 "parser.c"
        break;
      case 8:
// 1044 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_namespace(yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(39,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3769 "parser.c"
        break;
      case 9:
// 1048 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_interface(yymsp[-1].minor.yy0, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(42,&yymsp[-2].minor);
}
// 3777 "parser.c"
        break;
      case 10:
// 1052 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_interface(yymsp[-3].minor.yy0, yymsp[0].minor.yy115, yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(42,&yymsp[-4].minor);
  yy_destructor(43,&yymsp[-2].minor);
}
// 3786 "parser.c"
        break;
      case 11:
// 1056 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class(yymsp[-1].minor.yy0, yymsp[0].minor.yy115, 0, 0, NULL, NULL, status->scanner_state);
  yy_destructor(44,&yymsp[-2].minor);
}
// 3794 "parser.c"
        break;
      case 12:
// 1060 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy115, 0, 0, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(44,&yymsp[-4].minor);
  yy_destructor(43,&yymsp[-2].minor);
}
// 3803 "parser.c"
        break;
      case 13:
// 1064 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy115, 0, 0, NULL, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(44,&yymsp[-4].minor);
  yy_destructor(45,&yymsp[-2].minor);
}
// 3812 "parser.c"
        break;
      case 14:
// 1068 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class(yymsp[-5].minor.yy0, yymsp[0].minor.yy115, 0, 0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(44,&yymsp[-6].minor);
  yy_destructor(43,&yymsp[-4].minor);
  yy_destructor(45,&yymsp[-2].minor);
}
// 3822 "parser.c"
        break;
      case 15:
// 1072 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class(yymsp[-1].minor.yy0, yymsp[0].minor.yy115, 1, 0, NULL, NULL, status->scanner_state);
  yy_destructor(46,&yymsp[-3].minor);
  yy_destructor(44,&yymsp[-2].minor);
}
// 3831 "parser.c"
        break;
      case 16:
// 1076 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy115, 1, 0, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(46,&yymsp[-5].minor);
  yy_destructor(44,&yymsp[-4].minor);
  yy_destructor(43,&yymsp[-2].minor);
}
// 3841 "parser.c"
        break;
      case 17:
// 1080 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy115, 1, 0, NULL, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(46,&yymsp[-5].minor);
  yy_destructor(44,&yymsp[-4].minor);
  yy_destructor(45,&yymsp[-2].minor);
}
// 3851 "parser.c"
        break;
      case 18:
// 1084 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class(yymsp[-1].minor.yy0, yymsp[0].minor.yy115, 0, 1, NULL, NULL, status->scanner_state);
  yy_destructor(47,&yymsp[-3].minor);
  yy_destructor(44,&yymsp[-2].minor);
}
// 3860 "parser.c"
        break;
      case 19:
// 1088 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy115, 0, 1, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(47,&yymsp[-5].minor);
  yy_destructor(44,&yymsp[-4].minor);
  yy_destructor(43,&yymsp[-2].minor);
}
// 3870 "parser.c"
        break;
      case 20:
      case 25:
      case 46:
// 1092 "parser.lemon"
{
	yygotominor.yy115 = NULL;
  yy_destructor(48,&yymsp[-1].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 3881 "parser.c"
        break;
      case 21:
      case 26:
      case 47:
// 1096 "parser.lemon"
{
	yygotominor.yy115 = yymsp[-1].minor.yy115;
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 3892 "parser.c"
        break;
      case 22:
      case 48:
      case 102:
      case 174:
      case 202:
      case 224:
      case 301:
      case 304:
// 1100 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_list(yymsp[-2].minor.yy115, yymsp[0].minor.yy115);
  yy_destructor(6,&yymsp[-1].minor);
}
// 3907 "parser.c"
        break;
      case 24:
      case 265:
      case 308:
// 1108 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state);
}
// 3916 "parser.c"
        break;
      case 27:
// 1120 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_definition(yymsp[0].minor.yy115, NULL, NULL, status->scanner_state);
}
// 3923 "parser.c"
        break;
      case 28:
// 1124 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_definition(NULL, NULL, yymsp[0].minor.yy115, status->scanner_state);
}
// 3930 "parser.c"
        break;
      case 29:
// 1128 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_definition(NULL, yymsp[0].minor.yy115, NULL, status->scanner_state);
}
// 3937 "parser.c"
        break;
      case 30:
// 1132 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_definition(yymsp[-1].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
}
// 3944 "parser.c"
        break;
      case 31:
// 1136 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_definition(yymsp[-1].minor.yy115, NULL, yymsp[0].minor.yy115, status->scanner_state);
}
// 3951 "parser.c"
        break;
      case 32:
// 1140 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_definition(yymsp[0].minor.yy115, NULL, yymsp[-1].minor.yy115, status->scanner_state);
}
// 3958 "parser.c"
        break;
      case 33:
// 1144 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_definition(NULL, yymsp[0].minor.yy115, yymsp[-1].minor.yy115, status->scanner_state);
}
// 3965 "parser.c"
        break;
      case 34:
// 1148 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_definition(yymsp[-2].minor.yy115, yymsp[0].minor.yy115, yymsp[-1].minor.yy115, status->scanner_state);
}
// 3972 "parser.c"
        break;
      case 35:
// 1152 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_definition(yymsp[-1].minor.yy115, yymsp[0].minor.yy115, yymsp[-2].minor.yy115, status->scanner_state);
}
// 3979 "parser.c"
        break;
      case 38:
// 1169 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_property(yymsp[-2].minor.yy115, yymsp[-1].minor.yy0, NULL, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(41,&yymsp[0].minor);
}
// 3987 "parser.c"
        break;
      case 39:
// 1173 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_property(yymsp[-2].minor.yy115, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(41,&yymsp[0].minor);
}
// 3995 "parser.c"
        break;
      case 40:
// 1177 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_property(yymsp[-4].minor.yy115, yymsp[-3].minor.yy0, yymsp[-1].minor.yy115, yymsp[-5].minor.yy0, NULL, status->scanner_state);
  yy_destructor(51,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 4004 "parser.c"
        break;
      case 41:
// 1181 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_property(yymsp[-4].minor.yy115, yymsp[-3].minor.yy0, yymsp[-1].minor.yy115, NULL, NULL, status->scanner_state);
  yy_destructor(51,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 4013 "parser.c"
        break;
      case 42:
// 1185 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_property(yymsp[-3].minor.yy115, yymsp[-2].minor.yy0, NULL, yymsp[-4].minor.yy0, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(41,&yymsp[0].minor);
}
// 4021 "parser.c"
        break;
      case 43:
// 1189 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_property(yymsp[-3].minor.yy115, yymsp[-2].minor.yy0, NULL, NULL, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(41,&yymsp[0].minor);
}
// 4029 "parser.c"
        break;
      case 44:
// 1193 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_property(yymsp[-5].minor.yy115, yymsp[-4].minor.yy0, yymsp[-2].minor.yy115, yymsp[-6].minor.yy0, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(51,&yymsp[-3].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 4038 "parser.c"
        break;
      case 45:
// 1197 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_property(yymsp[-5].minor.yy115, yymsp[-4].minor.yy0, yymsp[-2].minor.yy115, NULL, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(51,&yymsp[-3].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 4047 "parser.c"
        break;
      case 50:
// 1217 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_property_shortcut(NULL, yymsp[0].minor.yy0, status->scanner_state);
}
// 4054 "parser.c"
        break;
      case 51:
// 1221 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_property_shortcut(yymsp[-1].minor.yy0, yymsp[0].minor.yy0, status->scanner_state);
}
// 4061 "parser.c"
        break;
      case 58:
// 1250 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy115, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(52,&yymsp[-4].minor);
  yy_destructor(51,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 4071 "parser.c"
        break;
      case 59:
// 1254 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy115, NULL, status->scanner_state);
  yy_destructor(52,&yymsp[-4].minor);
  yy_destructor(51,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 4081 "parser.c"
        break;
      case 60:
// 1259 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_method(yymsp[-6].minor.yy115, yymsp[-4].minor.yy0, NULL, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(54,&yymsp[-5].minor);
  yy_destructor(55,&yymsp[-3].minor);
  yy_destructor(36,&yymsp[-2].minor);
  yy_destructor(48,&yymsp[-1].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4093 "parser.c"
        break;
      case 61:
// 1263 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_method(yymsp[-7].minor.yy115, yymsp[-5].minor.yy0, yymsp[-3].minor.yy115, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(54,&yymsp[-6].minor);
  yy_destructor(55,&yymsp[-4].minor);
  yy_destructor(36,&yymsp[-2].minor);
  yy_destructor(48,&yymsp[-1].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4105 "parser.c"
        break;
      case 62:
// 1267 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_method(yymsp[-7].minor.yy115, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy115, NULL, NULL, status->scanner_state);
  yy_destructor(54,&yymsp[-6].minor);
  yy_destructor(55,&yymsp[-4].minor);
  yy_destructor(36,&yymsp[-3].minor);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4117 "parser.c"
        break;
      case 63:
// 1271 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_method(yymsp[-8].minor.yy115, yymsp[-6].minor.yy0, yymsp[-4].minor.yy115, yymsp[-1].minor.yy115, NULL, NULL, status->scanner_state);
  yy_destructor(54,&yymsp[-7].minor);
  yy_destructor(55,&yymsp[-5].minor);
  yy_destructor(36,&yymsp[-3].minor);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4129 "parser.c"
        break;
      case 64:
// 1275 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_method(yymsp[-6].minor.yy115, yymsp[-4].minor.yy0, NULL, NULL, yymsp[-7].minor.yy0, NULL, status->scanner_state);
  yy_destructor(54,&yymsp[-5].minor);
  yy_destructor(55,&yymsp[-3].minor);
  yy_destructor(36,&yymsp[-2].minor);
  yy_destructor(48,&yymsp[-1].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4141 "parser.c"
        break;
      case 65:
// 1279 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_method(yymsp[-7].minor.yy115, yymsp[-5].minor.yy0, yymsp[-3].minor.yy115, NULL, yymsp[-8].minor.yy0, NULL, status->scanner_state);
  yy_destructor(54,&yymsp[-6].minor);
  yy_destructor(55,&yymsp[-4].minor);
  yy_destructor(36,&yymsp[-2].minor);
  yy_destructor(48,&yymsp[-1].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4153 "parser.c"
        break;
      case 66:
// 1283 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_method(yymsp[-7].minor.yy115, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy115, yymsp[-8].minor.yy0, NULL, status->scanner_state);
  yy_destructor(54,&yymsp[-6].minor);
  yy_destructor(55,&yymsp[-4].minor);
  yy_destructor(36,&yymsp[-3].minor);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4165 "parser.c"
        break;
      case 67:
// 1287 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_method(yymsp[-8].minor.yy115, yymsp[-6].minor.yy0, yymsp[-4].minor.yy115, yymsp[-1].minor.yy115, yymsp[-9].minor.yy0, NULL, status->scanner_state);
  yy_destructor(54,&yymsp[-7].minor);
  yy_destructor(55,&yymsp[-5].minor);
  yy_destructor(36,&yymsp[-3].minor);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4177 "parser.c"
        break;
      case 68:
// 1291 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_method(yymsp[-8].minor.yy115, yymsp[-6].minor.yy0, NULL, NULL, NULL, yymsp[-2].minor.yy115, status->scanner_state);
  yy_destructor(54,&yymsp[-7].minor);
  yy_destructor(55,&yymsp[-5].minor);
  yy_destructor(36,&yymsp[-4].minor);
  yy_destructor(38,&yymsp[-3].minor);
  yy_destructor(48,&yymsp[-1].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4190 "parser.c"
        break;
      case 69:
// 1295 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_method(yymsp[-9].minor.yy115, yymsp[-7].minor.yy0, yymsp[-5].minor.yy115, NULL, NULL, yymsp[-2].minor.yy115, status->scanner_state);
  yy_destructor(54,&yymsp[-8].minor);
  yy_destructor(55,&yymsp[-6].minor);
  yy_destructor(36,&yymsp[-4].minor);
  yy_destructor(38,&yymsp[-3].minor);
  yy_destructor(48,&yymsp[-1].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4203 "parser.c"
        break;
      case 70:
// 1299 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_method(yymsp[-9].minor.yy115, yymsp[-7].minor.yy0, NULL, yymsp[-1].minor.yy115, NULL, yymsp[-3].minor.yy115, status->scanner_state);
  yy_destructor(54,&yymsp[-8].minor);
  yy_destructor(55,&yymsp[-6].minor);
  yy_destructor(36,&yymsp[-5].minor);
  yy_destructor(38,&yymsp[-4].minor);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4216 "parser.c"
        break;
      case 71:
// 1303 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_method(yymsp[-10].minor.yy115, yymsp[-8].minor.yy0, yymsp[-6].minor.yy115, yymsp[-1].minor.yy115, NULL, yymsp[-3].minor.yy115, status->scanner_state);
  yy_destructor(54,&yymsp[-9].minor);
  yy_destructor(55,&yymsp[-7].minor);
  yy_destructor(36,&yymsp[-5].minor);
  yy_destructor(38,&yymsp[-4].minor);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4229 "parser.c"
        break;
      case 72:
// 1307 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_method(yymsp[-8].minor.yy115, yymsp[-6].minor.yy0, NULL, NULL, yymsp[-9].minor.yy0, yymsp[-2].minor.yy115, status->scanner_state);
  yy_destructor(54,&yymsp[-7].minor);
  yy_destructor(55,&yymsp[-5].minor);
  yy_destructor(36,&yymsp[-4].minor);
  yy_destructor(38,&yymsp[-3].minor);
  yy_destructor(48,&yymsp[-1].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4242 "parser.c"
        break;
      case 73:
// 1311 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_method(yymsp[-9].minor.yy115, yymsp[-7].minor.yy0, yymsp[-5].minor.yy115, NULL, yymsp[-10].minor.yy0, yymsp[-2].minor.yy115, status->scanner_state);
  yy_destructor(54,&yymsp[-8].minor);
  yy_destructor(55,&yymsp[-6].minor);
  yy_destructor(36,&yymsp[-4].minor);
  yy_destructor(38,&yymsp[-3].minor);
  yy_destructor(48,&yymsp[-1].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4255 "parser.c"
        break;
      case 74:
// 1315 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_method(yymsp[-9].minor.yy115, yymsp[-7].minor.yy0, NULL, yymsp[-1].minor.yy115, yymsp[-10].minor.yy0, yymsp[-3].minor.yy115, status->scanner_state);
  yy_destructor(54,&yymsp[-8].minor);
  yy_destructor(55,&yymsp[-6].minor);
  yy_destructor(36,&yymsp[-5].minor);
  yy_destructor(38,&yymsp[-4].minor);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4268 "parser.c"
        break;
      case 75:
// 1319 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_method(yymsp[-10].minor.yy115, yymsp[-8].minor.yy0, yymsp[-6].minor.yy115, yymsp[-1].minor.yy115, yymsp[-11].minor.yy0, yymsp[-3].minor.yy115, status->scanner_state);
  yy_destructor(54,&yymsp[-9].minor);
  yy_destructor(55,&yymsp[-7].minor);
  yy_destructor(36,&yymsp[-5].minor);
  yy_destructor(38,&yymsp[-4].minor);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4281 "parser.c"
        break;
      case 76:
// 1324 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_method(yymsp[-7].minor.yy115, yymsp[-5].minor.yy0, NULL, NULL, NULL, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(54,&yymsp[-6].minor);
  yy_destructor(55,&yymsp[-4].minor);
  yy_destructor(36,&yymsp[-3].minor);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 4293 "parser.c"
        break;
      case 77:
// 1328 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_method(yymsp[-8].minor.yy115, yymsp[-6].minor.yy0, yymsp[-4].minor.yy115, NULL, NULL, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(54,&yymsp[-7].minor);
  yy_destructor(55,&yymsp[-5].minor);
  yy_destructor(36,&yymsp[-3].minor);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 4305 "parser.c"
        break;
      case 78:
// 1332 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_method(yymsp[-7].minor.yy115, yymsp[-5].minor.yy0, NULL, NULL, yymsp[-8].minor.yy0, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(54,&yymsp[-6].minor);
  yy_destructor(55,&yymsp[-4].minor);
  yy_destructor(36,&yymsp[-3].minor);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 4317 "parser.c"
        break;
      case 79:
// 1336 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_method(yymsp[-8].minor.yy115, yymsp[-6].minor.yy0, yymsp[-4].minor.yy115, NULL, yymsp[-9].minor.yy0, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(54,&yymsp[-7].minor);
  yy_destructor(55,&yymsp[-5].minor);
  yy_destructor(36,&yymsp[-3].minor);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 4329 "parser.c"
        break;
      case 80:
// 1340 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_method(yymsp[-5].minor.yy115, yymsp[-3].minor.yy0, NULL, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(54,&yymsp[-4].minor);
  yy_destructor(55,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[-1].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 4340 "parser.c"
        break;
      case 81:
// 1344 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_method(yymsp[-6].minor.yy115, yymsp[-4].minor.yy0, yymsp[-2].minor.yy115, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(54,&yymsp[-5].minor);
  yy_destructor(55,&yymsp[-3].minor);
  yy_destructor(36,&yymsp[-1].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 4351 "parser.c"
        break;
      case 82:
// 1348 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_method(yymsp[-5].minor.yy115, yymsp[-3].minor.yy0, NULL, NULL, yymsp[-6].minor.yy0, NULL, status->scanner_state);
  yy_destructor(54,&yymsp[-4].minor);
  yy_destructor(55,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[-1].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 4362 "parser.c"
        break;
      case 83:
// 1352 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_method(yymsp[-6].minor.yy115, yymsp[-4].minor.yy0, yymsp[-2].minor.yy115, NULL, yymsp[-7].minor.yy0, NULL, status->scanner_state);
  yy_destructor(54,&yymsp[-5].minor);
  yy_destructor(55,&yymsp[-3].minor);
  yy_destructor(36,&yymsp[-1].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 4373 "parser.c"
        break;
      case 86:
// 1365 "parser.lemon"
{
	yygotominor.yy115 = json_object_new_string("public");
  yy_destructor(1,&yymsp[0].minor);
}
// 4381 "parser.c"
        break;
      case 87:
// 1369 "parser.lemon"
{
	yygotominor.yy115 = json_object_new_string("protected");
  yy_destructor(2,&yymsp[0].minor);
}
// 4389 "parser.c"
        break;
      case 88:
// 1373 "parser.lemon"
{
	yygotominor.yy115 = json_object_new_string("private");
  yy_destructor(4,&yymsp[0].minor);
}
// 4397 "parser.c"
        break;
      case 89:
// 1377 "parser.lemon"
{
	yygotominor.yy115 = json_object_new_string("static");
  yy_destructor(3,&yymsp[0].minor);
}
// 4405 "parser.c"
        break;
      case 90:
// 1381 "parser.lemon"
{
	yygotominor.yy115 = json_object_new_string("scoped");
  yy_destructor(5,&yymsp[0].minor);
}
// 4413 "parser.c"
        break;
      case 91:
// 1385 "parser.lemon"
{
	yygotominor.yy115 = json_object_new_string("inline");
  yy_destructor(56,&yymsp[0].minor);
}
// 4421 "parser.c"
        break;
      case 92:
// 1389 "parser.lemon"
{
	yygotominor.yy115 = json_object_new_string("abstract");
  yy_destructor(46,&yymsp[0].minor);
}
// 4429 "parser.c"
        break;
      case 93:
// 1393 "parser.lemon"
{
	yygotominor.yy115 = json_object_new_string("final");
  yy_destructor(47,&yymsp[0].minor);
}
// 4437 "parser.c"
        break;
      case 94:
// 1398 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_return_type(1, NULL, status->scanner_state);
  yy_destructor(57,&yymsp[0].minor);
}
// 4445 "parser.c"
        break;
      case 95:
// 1402 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_return_type(0, yymsp[0].minor.yy115, status->scanner_state);
}
// 4452 "parser.c"
        break;
      case 96:
// 1406 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_list(yymsp[-2].minor.yy115, yymsp[0].minor.yy115);
  yy_destructor(15,&yymsp[-1].minor);
}
// 4460 "parser.c"
        break;
      case 98:
// 1414 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_return_type_item(yymsp[0].minor.yy115, NULL, 0, 0, status->scanner_state);
}
// 4467 "parser.c"
        break;
      case 99:
// 1418 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_return_type_item(yymsp[-1].minor.yy115, NULL, 1, 0, status->scanner_state);
  yy_destructor(35,&yymsp[0].minor);
}
// 4475 "parser.c"
        break;
      case 100:
// 1422 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_return_type_item(NULL, yymsp[0].minor.yy115, 0, 0, status->scanner_state);
}
// 4482 "parser.c"
        break;
      case 101:
// 1426 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_return_type_item(NULL, yymsp[0].minor.yy115, 0, 1, status->scanner_state);
}
// 4489 "parser.c"
        break;
      case 104:
// 1439 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_parameter(NULL, NULL, yymsp[0].minor.yy0, NULL, 0, status->scanner_state);
}
// 4496 "parser.c"
        break;
      case 105:
// 1443 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_parameter(yymsp[-1].minor.yy115, NULL, yymsp[0].minor.yy0, NULL, 0, status->scanner_state);
}
// 4503 "parser.c"
        break;
      case 106:
// 1447 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_parameter(yymsp[-2].minor.yy115, NULL, yymsp[0].minor.yy0, NULL, 1, status->scanner_state);
  yy_destructor(35,&yymsp[-1].minor);
}
// 4511 "parser.c"
        break;
      case 107:
// 1451 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_parameter(NULL, yymsp[-1].minor.yy115, yymsp[0].minor.yy0, NULL, 0, status->scanner_state);
}
// 4518 "parser.c"
        break;
      case 108:
// 1455 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_parameter(NULL, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy115, 0, status->scanner_state);
  yy_destructor(51,&yymsp[-1].minor);
}
// 4526 "parser.c"
        break;
      case 109:
// 1459 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_parameter(yymsp[-3].minor.yy115, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy115, 0, status->scanner_state);
  yy_destructor(51,&yymsp[-1].minor);
}
// 4534 "parser.c"
        break;
      case 110:
// 1463 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_parameter(yymsp[-4].minor.yy115, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy115, 1, status->scanner_state);
  yy_destructor(35,&yymsp[-3].minor);
  yy_destructor(51,&yymsp[-1].minor);
}
// 4543 "parser.c"
        break;
      case 111:
// 1467 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_parameter(NULL, yymsp[-3].minor.yy115, yymsp[-2].minor.yy0, yymsp[0].minor.yy115, 0, status->scanner_state);
  yy_destructor(51,&yymsp[-1].minor);
}
// 4551 "parser.c"
        break;
      case 112:
// 1471 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(19,&yymsp[-2].minor);
  yy_destructor(20,&yymsp[0].minor);
}
// 4560 "parser.c"
        break;
      case 113:
// 1475 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(19,&yymsp[-4].minor);
  yy_destructor(37,&yymsp[-2].minor);
  yy_destructor(58,&yymsp[-1].minor);
  yy_destructor(20,&yymsp[0].minor);
}
// 4571 "parser.c"
        break;
      case 114:
// 1479 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_type(XX_TYPE_INTEGER);
  yy_destructor(59,&yymsp[0].minor);
}
// 4579 "parser.c"
        break;
      case 115:
// 1483 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_type(XX_TYPE_UINTEGER);
  yy_destructor(60,&yymsp[0].minor);
}
// 4587 "parser.c"
        break;
      case 116:
// 1487 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_type(XX_TYPE_LONG);
  yy_destructor(61,&yymsp[0].minor);
}
// 4595 "parser.c"
        break;
      case 117:
// 1491 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_type(XX_TYPE_ULONG);
  yy_destructor(62,&yymsp[0].minor);
}
// 4603 "parser.c"
        break;
      case 118:
// 1495 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_type(XX_TYPE_CHAR);
  yy_destructor(63,&yymsp[0].minor);
}
// 4611 "parser.c"
        break;
      case 119:
// 1499 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_type(XX_TYPE_UCHAR);
  yy_destructor(64,&yymsp[0].minor);
}
// 4619 "parser.c"
        break;
      case 120:
// 1503 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_type(XX_TYPE_DOUBLE);
  yy_destructor(65,&yymsp[0].minor);
}
// 4627 "parser.c"
        break;
      case 121:
// 1507 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_type(XX_TYPE_BOOL);
  yy_destructor(66,&yymsp[0].minor);
}
// 4635 "parser.c"
        break;
      case 122:
// 1511 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_type(XX_TYPE_STRING);
  yy_destructor(67,&yymsp[0].minor);
}
// 4643 "parser.c"
        break;
      case 123:
// 1515 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_type(XX_TYPE_ARRAY);
  yy_destructor(68,&yymsp[0].minor);
}
// 4651 "parser.c"
        break;
      case 124:
// 1519 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_type(XX_TYPE_VAR);
  yy_destructor(69,&yymsp[0].minor);
}
// 4659 "parser.c"
        break;
      case 147:
// 1611 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_empty_statement(status->scanner_state);
  yy_destructor(41,&yymsp[0].minor);
}
// 4667 "parser.c"
        break;
      case 148:
// 1615 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_break_statement(status->scanner_state);
  yy_destructor(70,&yymsp[-1].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 4676 "parser.c"
        break;
      case 149:
// 1619 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_continue_statement(status->scanner_state);
  yy_destructor(71,&yymsp[-1].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 4685 "parser.c"
        break;
      case 150:
// 1623 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_if_statement(yymsp[-2].minor.yy115, NULL, NULL, status->scanner_state);
  yy_destructor(72,&yymsp[-3].minor);
  yy_destructor(48,&yymsp[-1].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4695 "parser.c"
        break;
      case 151:
// 1627 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_if_statement(yymsp[-5].minor.yy115, NULL, NULL, status->scanner_state);
  yy_destructor(72,&yymsp[-6].minor);
  yy_destructor(48,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-3].minor);
  yy_destructor(73,&yymsp[-2].minor);
  yy_destructor(48,&yymsp[-1].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4708 "parser.c"
        break;
      case 152:
// 1631 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_if_statement(yymsp[-3].minor.yy115, yymsp[-1].minor.yy115, NULL, status->scanner_state);
  yy_destructor(72,&yymsp[-4].minor);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4718 "parser.c"
        break;
      case 153:
// 1635 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_if_statement(yymsp[-7].minor.yy115, yymsp[-5].minor.yy115, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(72,&yymsp[-8].minor);
  yy_destructor(48,&yymsp[-6].minor);
  yy_destructor(49,&yymsp[-4].minor);
  yy_destructor(73,&yymsp[-3].minor);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4731 "parser.c"
        break;
      case 154:
// 1639 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_if_statement(yymsp[-6].minor.yy115, yymsp[-4].minor.yy115, NULL, status->scanner_state);
  yy_destructor(72,&yymsp[-7].minor);
  yy_destructor(48,&yymsp[-5].minor);
  yy_destructor(49,&yymsp[-3].minor);
  yy_destructor(73,&yymsp[-2].minor);
  yy_destructor(48,&yymsp[-1].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4744 "parser.c"
        break;
      case 155:
// 1643 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_if_statement(yymsp[-6].minor.yy115, NULL, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(72,&yymsp[-7].minor);
  yy_destructor(48,&yymsp[-5].minor);
  yy_destructor(49,&yymsp[-4].minor);
  yy_destructor(73,&yymsp[-3].minor);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4757 "parser.c"
        break;
      case 156:
// 1647 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_switch_statement(yymsp[-2].minor.yy115, NULL, status->scanner_state);
  yy_destructor(74,&yymsp[-3].minor);
  yy_destructor(48,&yymsp[-1].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4767 "parser.c"
        break;
      case 157:
// 1651 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_switch_statement(yymsp[-3].minor.yy115, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(74,&yymsp[-4].minor);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4777 "parser.c"
        break;
      case 160:
// 1663 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_case_clause(yymsp[-1].minor.yy115, NULL, status->scanner_state);
  yy_destructor(75,&yymsp[-2].minor);
  yy_destructor(76,&yymsp[0].minor);
}
// 4786 "parser.c"
        break;
      case 161:
// 1667 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_case_clause(yymsp[-2].minor.yy115, yymsp[0].minor.yy115, status->scanner_state);
  yy_destructor(75,&yymsp[-3].minor);
  yy_destructor(76,&yymsp[-1].minor);
}
// 4795 "parser.c"
        break;
      case 162:
// 1671 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_case_clause(NULL, yymsp[0].minor.yy115, status->scanner_state);
  yy_destructor(77,&yymsp[-2].minor);
  yy_destructor(76,&yymsp[-1].minor);
}
// 4804 "parser.c"
        break;
      case 163:
// 1675 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_loop_statement(NULL, status->scanner_state);
  yy_destructor(78,&yymsp[-2].minor);
  yy_destructor(48,&yymsp[-1].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4814 "parser.c"
        break;
      case 164:
// 1679 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_loop_statement(yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(78,&yymsp[-3].minor);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4824 "parser.c"
        break;
      case 165:
// 1683 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_while_statement(yymsp[-2].minor.yy115, NULL, status->scanner_state);
  yy_destructor(79,&yymsp[-3].minor);
  yy_destructor(48,&yymsp[-1].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4834 "parser.c"
        break;
      case 166:
// 1687 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_while_statement(yymsp[-3].minor.yy115, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(79,&yymsp[-4].minor);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4844 "parser.c"
        break;
      case 167:
// 1691 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_do_while_statement(yymsp[-1].minor.yy115, NULL, status->scanner_state);
  yy_destructor(80,&yymsp[-5].minor);
  yy_destructor(48,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-3].minor);
  yy_destructor(79,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 4856 "parser.c"
        break;
      case 168:
// 1695 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_do_while_statement(yymsp[-1].minor.yy115, yymsp[-4].minor.yy115, status->scanner_state);
  yy_destructor(80,&yymsp[-6].minor);
  yy_destructor(48,&yymsp[-5].minor);
  yy_destructor(49,&yymsp[-3].minor);
  yy_destructor(79,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 4868 "parser.c"
        break;
      case 169:
// 1699 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_for_statement(yymsp[-3].minor.yy115, NULL, yymsp[-5].minor.yy0, 0, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(81,&yymsp[-6].minor);
  yy_destructor(82,&yymsp[-4].minor);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4879 "parser.c"
        break;
      case 170:
// 1703 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_for_statement(yymsp[-3].minor.yy115, NULL, yymsp[-6].minor.yy0, 1, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(81,&yymsp[-7].minor);
  yy_destructor(82,&yymsp[-5].minor);
  yy_destructor(83,&yymsp[-4].minor);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4891 "parser.c"
        break;
      case 171:
// 1707 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_for_statement(yymsp[-3].minor.yy115, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, 0, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(81,&yymsp[-8].minor);
  yy_destructor(6,&yymsp[-6].minor);
  yy_destructor(82,&yymsp[-4].minor);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4903 "parser.c"
        break;
      case 172:
// 1711 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_for_statement(yymsp[-3].minor.yy115, yymsp[-8].minor.yy0, yymsp[-6].minor.yy0, 1, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(81,&yymsp[-9].minor);
  yy_destructor(6,&yymsp[-7].minor);
  yy_destructor(82,&yymsp[-5].minor);
  yy_destructor(83,&yymsp[-4].minor);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4916 "parser.c"
        break;
      case 173:
// 1715 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_let_statement(yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(84,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 4925 "parser.c"
        break;
      case 176:
// 1728 "parser.lemon"
{
	yygotominor.yy115 = json_object_new_string("assign");
  yy_destructor(51,&yymsp[0].minor);
}
// 4933 "parser.c"
        break;
      case 177:
// 1733 "parser.lemon"
{
	yygotominor.yy115 = json_object_new_string("add-assign");
  yy_destructor(85,&yymsp[0].minor);
}
// 4941 "parser.c"
        break;
      case 178:
// 1738 "parser.lemon"
{
	yygotominor.yy115 = json_object_new_string("sub-assign");
  yy_destructor(86,&yymsp[0].minor);
}
// 4949 "parser.c"
        break;
      case 179:
// 1742 "parser.lemon"
{
	yygotominor.yy115 = json_object_new_string("mult-assign");
  yy_destructor(87,&yymsp[0].minor);
}
// 4957 "parser.c"
        break;
      case 180:
// 1746 "parser.lemon"
{
	yygotominor.yy115 = json_object_new_string("div-assign");
  yy_destructor(88,&yymsp[0].minor);
}
// 4965 "parser.c"
        break;
      case 181:
// 1750 "parser.lemon"
{
	yygotominor.yy115 = json_object_new_string("concat-assign");
  yy_destructor(89,&yymsp[0].minor);
}
// 4973 "parser.c"
        break;
      case 182:
// 1755 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_let_assignment("variable", yymsp[-1].minor.yy115, yymsp[-2].minor.yy0, NULL, NULL, yymsp[0].minor.yy115, status->scanner_state);
}
// 4980 "parser.c"
        break;
      case 183:
// 1760 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_let_assignment("object-property", yymsp[-1].minor.yy115, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, yymsp[0].minor.yy115, status->scanner_state);
  yy_destructor(38,&yymsp[-3].minor);
}
// 4988 "parser.c"
        break;
      case 184:
// 1765 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_let_assignment("variable-dynamic-object-property", yymsp[-1].minor.yy115, yymsp[-6].minor.yy0, yymsp[-3].minor.yy0, NULL, yymsp[0].minor.yy115, status->scanner_state);
  yy_destructor(38,&yymsp[-5].minor);
  yy_destructor(48,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
}
// 4998 "parser.c"
        break;
      case 185:
// 1770 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_let_assignment("string-dynamic-object-property", yymsp[-1].minor.yy115, yymsp[-6].minor.yy0, yymsp[-3].minor.yy0, NULL, yymsp[0].minor.yy115, status->scanner_state);
  yy_destructor(38,&yymsp[-5].minor);
  yy_destructor(48,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
}
// 5008 "parser.c"
        break;
      case 186:
// 1775 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_let_assignment("object-property-append", yymsp[-1].minor.yy115, yymsp[-6].minor.yy0, yymsp[-4].minor.yy0, NULL, yymsp[0].minor.yy115, status->scanner_state);
  yy_destructor(38,&yymsp[-5].minor);
  yy_destructor(37,&yymsp[-3].minor);
  yy_destructor(58,&yymsp[-2].minor);
}
// 5018 "parser.c"
        break;
      case 187:
// 1780 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_let_assignment("object-property-array-index", yymsp[-1].minor.yy115, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, yymsp[-3].minor.yy115, yymsp[0].minor.yy115, status->scanner_state);
  yy_destructor(38,&yymsp[-6].minor);
  yy_destructor(37,&yymsp[-4].minor);
  yy_destructor(58,&yymsp[-2].minor);
}
// 5028 "parser.c"
        break;
      case 188:
// 1785 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_let_assignment("static-property", yymsp[-1].minor.yy115, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, yymsp[0].minor.yy115, status->scanner_state);
  yy_destructor(91,&yymsp[-3].minor);
}
// 5036 "parser.c"
        break;
      case 189:
// 1790 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_let_assignment("static-property-append", yymsp[-1].minor.yy115, yymsp[-6].minor.yy0, yymsp[-4].minor.yy0, NULL, yymsp[0].minor.yy115, status->scanner_state);
  yy_destructor(91,&yymsp[-5].minor);
  yy_destructor(37,&yymsp[-3].minor);
  yy_destructor(58,&yymsp[-2].minor);
}
// 5046 "parser.c"
        break;
      case 190:
// 1795 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_let_assignment("static-property-array-index", yymsp[-1].minor.yy115, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, yymsp[-3].minor.yy115, yymsp[0].minor.yy115, status->scanner_state);
  yy_destructor(91,&yymsp[-6].minor);
  yy_destructor(37,&yymsp[-4].minor);
  yy_destructor(58,&yymsp[-2].minor);
}
// 5056 "parser.c"
        break;
      case 191:
// 1800 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_let_assignment("variable-append", yymsp[-1].minor.yy115, yymsp[-4].minor.yy0, NULL, NULL, yymsp[0].minor.yy115, status->scanner_state);
  yy_destructor(37,&yymsp[-3].minor);
  yy_destructor(58,&yymsp[-2].minor);
}
// 5065 "parser.c"
        break;
      case 192:
// 1805 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_let_assignment("array-index", yymsp[-1].minor.yy115, yymsp[-3].minor.yy0, NULL, yymsp[-2].minor.yy115, yymsp[0].minor.yy115, status->scanner_state);
}
// 5072 "parser.c"
        break;
      case 195:
// 1817 "parser.lemon"
{
	yygotominor.yy115 = yymsp[-1].minor.yy115;
  yy_destructor(37,&yymsp[-2].minor);
  yy_destructor(58,&yymsp[0].minor);
}
// 5081 "parser.c"
        break;
      case 196:
// 1822 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_let_assignment("incr", NULL, yymsp[-3].minor.yy0, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(92,&yymsp[0].minor);
}
// 5090 "parser.c"
        break;
      case 197:
// 1827 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_let_assignment("decr", NULL, yymsp[-3].minor.yy0, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(93,&yymsp[0].minor);
}
// 5099 "parser.c"
        break;
      case 198:
// 1832 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_let_assignment("incr", NULL, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(92,&yymsp[0].minor);
}
// 5107 "parser.c"
        break;
      case 199:
// 1837 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_let_assignment("decr", NULL, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(93,&yymsp[0].minor);
}
// 5115 "parser.c"
        break;
      case 201:
// 1845 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_echo_statement(yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(94,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 5124 "parser.c"
        break;
      case 204:
// 1857 "parser.lemon"
{
	yygotominor.yy115 = yymsp[0].minor.yy115;;
}
// 5131 "parser.c"
        break;
      case 205:
// 1862 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_mcall_statement(yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(41,&yymsp[0].minor);
}
// 5139 "parser.c"
        break;
      case 206:
// 1867 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_fcall_statement(yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(41,&yymsp[0].minor);
}
// 5147 "parser.c"
        break;
      case 207:
// 1872 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_scall_statement(yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(41,&yymsp[0].minor);
}
// 5155 "parser.c"
        break;
      case 208:
// 1877 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_fetch_statement(yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(41,&yymsp[0].minor);
}
// 5163 "parser.c"
        break;
      case 209:
// 1882 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_return_statement(yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(95,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 5172 "parser.c"
        break;
      case 210:
// 1887 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_return_statement(NULL, status->scanner_state);
  yy_destructor(95,&yymsp[-1].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 5181 "parser.c"
        break;
      case 211:
// 1892 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(96,&yymsp[-4].minor);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 5195 "parser.c"
        break;
      case 212:
// 1901 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state),
		yymsp[-2].minor.yy115,
		status->scanner_state
	);
  yy_destructor(96,&yymsp[-5].minor);
  yy_destructor(37,&yymsp[-3].minor);
  yy_destructor(58,&yymsp[-1].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 5210 "parser.c"
        break;
      case 213:
// 1910 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_throw_exception(yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(97,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 5219 "parser.c"
        break;
      case 214:
// 1914 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_declare_statement(XX_T_TYPE_INTEGER, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(59,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 5228 "parser.c"
        break;
      case 215:
// 1918 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_declare_statement(XX_T_TYPE_UINTEGER, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(60,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 5237 "parser.c"
        break;
      case 216:
// 1922 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_declare_statement(XX_T_TYPE_CHAR, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(63,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 5246 "parser.c"
        break;
      case 217:
// 1926 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_declare_statement(XX_T_TYPE_UCHAR, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(64,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 5255 "parser.c"
        break;
      case 218:
// 1930 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_declare_statement(XX_T_TYPE_LONG, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(61,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 5264 "parser.c"
        break;
      case 219:
// 1934 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_declare_statement(XX_T_TYPE_ULONG, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(62,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 5273 "parser.c"
        break;
      case 220:
// 1938 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_declare_statement(XX_T_TYPE_DOUBLE, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(65,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 5282 "parser.c"
        break;
      case 221:
// 1942 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_declare_statement(XX_T_TYPE_STRING, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(67,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 5291 "parser.c"
        break;
      case 222:
// 1946 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_declare_statement(XX_T_TYPE_BOOL, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(66,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 5300 "parser.c"
        break;
      case 223:
// 1950 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_declare_statement(XX_T_TYPE_VAR, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(69,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 5309 "parser.c"
        break;
      case 226:
// 1962 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_declare_variable(yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 5316 "parser.c"
        break;
      case 227:
// 1966 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_declare_variable(yymsp[-2].minor.yy0, yymsp[0].minor.yy115, status->scanner_state);
  yy_destructor(51,&yymsp[-1].minor);
}
// 5324 "parser.c"
        break;
      case 229:
// 1974 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("not", yymsp[0].minor.yy115, NULL, NULL, status->scanner_state);
  yy_destructor(35,&yymsp[-1].minor);
}
// 5332 "parser.c"
        break;
      case 230:
// 1978 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("isset", yymsp[0].minor.yy115, NULL, NULL, status->scanner_state);
  yy_destructor(31,&yymsp[-1].minor);
}
// 5340 "parser.c"
        break;
      case 231:
// 1982 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("require", yymsp[0].minor.yy115, NULL, NULL, status->scanner_state);
  yy_destructor(7,&yymsp[-1].minor);
}
// 5348 "parser.c"
        break;
      case 232:
// 1986 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("clone", yymsp[0].minor.yy115, NULL, NULL, status->scanner_state);
  yy_destructor(10,&yymsp[-1].minor);
}
// 5356 "parser.c"
        break;
      case 233:
// 1990 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("empty", yymsp[0].minor.yy115, NULL, NULL, status->scanner_state);
  yy_destructor(11,&yymsp[-1].minor);
}
// 5364 "parser.c"
        break;
      case 234:
// 1994 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("likely", yymsp[0].minor.yy115, NULL, NULL, status->scanner_state);
  yy_destructor(8,&yymsp[-1].minor);
}
// 5372 "parser.c"
        break;
      case 235:
// 1998 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("unlikely", yymsp[0].minor.yy115, NULL, NULL, status->scanner_state);
  yy_destructor(9,&yymsp[-1].minor);
}
// 5380 "parser.c"
        break;
      case 236:
// 2002 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("equals", yymsp[-2].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(17,&yymsp[-1].minor);
}
// 5388 "parser.c"
        break;
      case 237:
// 2006 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("not-equals", yymsp[-2].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(24,&yymsp[-1].minor);
}
// 5396 "parser.c"
        break;
      case 238:
// 2010 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("identical", yymsp[-2].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(18,&yymsp[-1].minor);
}
// 5404 "parser.c"
        break;
      case 239:
// 2014 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("not-identical", yymsp[-2].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(23,&yymsp[-1].minor);
}
// 5412 "parser.c"
        break;
      case 240:
// 2018 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("less", yymsp[-2].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(19,&yymsp[-1].minor);
}
// 5420 "parser.c"
        break;
      case 241:
// 2022 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("greater", yymsp[-2].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(20,&yymsp[-1].minor);
}
// 5428 "parser.c"
        break;
      case 242:
// 2026 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("less-equal", yymsp[-2].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(21,&yymsp[-1].minor);
}
// 5436 "parser.c"
        break;
      case 243:
// 2030 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("greater-equal", yymsp[-2].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(22,&yymsp[-1].minor);
}
// 5444 "parser.c"
        break;
      case 244:
// 2034 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("list", yymsp[-1].minor.yy115, NULL, NULL, status->scanner_state);
  yy_destructor(55,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5453 "parser.c"
        break;
      case 245:
// 2038 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("cast", yymsp[-2].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(55,&yymsp[-3].minor);
  yy_destructor(36,&yymsp[-1].minor);
}
// 5462 "parser.c"
        break;
      case 246:
// 2042 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("type-hint", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(19,&yymsp[-3].minor);
  yy_destructor(20,&yymsp[-1].minor);
}
// 5471 "parser.c"
        break;
      case 247:
// 2046 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("property-access", yymsp[-2].minor.yy115, xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-1].minor);
}
// 5479 "parser.c"
        break;
      case 248:
// 2050 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("property-dynamic-access", yymsp[-4].minor.yy115, xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-3].minor);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 5489 "parser.c"
        break;
      case 249:
// 2054 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("property-string-access", yymsp[-4].minor.yy115, xx_ret_literal(XX_T_STRING, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-3].minor);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 5499 "parser.c"
        break;
      case 250:
// 2058 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("static-property-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(91,&yymsp[-1].minor);
}
// 5507 "parser.c"
        break;
      case 251:
      case 319:
// 2062 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("static-constant-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(91,&yymsp[-1].minor);
}
// 5516 "parser.c"
        break;
      case 252:
// 2071 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("array-access", yymsp[-3].minor.yy115, yymsp[-1].minor.yy115, NULL, status->scanner_state);
  yy_destructor(37,&yymsp[-2].minor);
  yy_destructor(58,&yymsp[0].minor);
}
// 5525 "parser.c"
        break;
      case 253:
// 2076 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("add", yymsp[-2].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(25,&yymsp[-1].minor);
}
// 5533 "parser.c"
        break;
      case 254:
// 2081 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("sub", yymsp[-2].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(26,&yymsp[-1].minor);
}
// 5541 "parser.c"
        break;
      case 255:
// 2086 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("mul", yymsp[-2].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(28,&yymsp[-1].minor);
}
// 5549 "parser.c"
        break;
      case 256:
// 2091 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("div", yymsp[-2].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(29,&yymsp[-1].minor);
}
// 5557 "parser.c"
        break;
      case 257:
// 2096 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("mod", yymsp[-2].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(30,&yymsp[-1].minor);
}
// 5565 "parser.c"
        break;
      case 258:
// 2101 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("concat", yymsp[-2].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(27,&yymsp[-1].minor);
}
// 5573 "parser.c"
        break;
      case 259:
// 2106 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("and", yymsp[-2].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(13,&yymsp[-1].minor);
}
// 5581 "parser.c"
        break;
      case 260:
// 2111 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("or", yymsp[-2].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(14,&yymsp[-1].minor);
}
// 5589 "parser.c"
        break;
      case 261:
// 2116 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("instanceof", yymsp[-2].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(12,&yymsp[-1].minor);
}
// 5597 "parser.c"
        break;
      case 262:
// 2121 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("fetch", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(32,&yymsp[-3].minor);
  yy_destructor(6,&yymsp[-1].minor);
}
// 5606 "parser.c"
        break;
      case 264:
// 2131 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("typeof", yymsp[0].minor.yy115, NULL, NULL, status->scanner_state);
  yy_destructor(33,&yymsp[-1].minor);
}
// 5614 "parser.c"
        break;
      case 266:
      case 310:
      case 312:
// 2141 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_literal(XX_T_INTEGER, yymsp[0].minor.yy0, status->scanner_state);
}
// 5623 "parser.c"
        break;
      case 267:
      case 309:
      case 314:
// 2146 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_literal(XX_T_STRING, yymsp[0].minor.yy0, status->scanner_state);
}
// 5632 "parser.c"
        break;
      case 268:
      case 313:
// 2151 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_literal(XX_T_CHAR, yymsp[0].minor.yy0, status->scanner_state);
}
// 5640 "parser.c"
        break;
      case 269:
      case 315:
// 2156 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_literal(XX_T_DOUBLE, yymsp[0].minor.yy0, status->scanner_state);
}
// 5648 "parser.c"
        break;
      case 270:
      case 316:
// 2161 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_literal(XX_T_NULL, NULL, status->scanner_state);
  yy_destructor(101,&yymsp[0].minor);
}
// 5657 "parser.c"
        break;
      case 271:
      case 318:
// 2166 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_literal(XX_T_TRUE, NULL, status->scanner_state);
  yy_destructor(102,&yymsp[0].minor);
}
// 5666 "parser.c"
        break;
      case 272:
      case 317:
// 2171 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_literal(XX_T_FALSE, NULL, status->scanner_state);
  yy_destructor(103,&yymsp[0].minor);
}
// 5675 "parser.c"
        break;
      case 273:
      case 320:
// 2176 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_literal(XX_T_CONSTANT, yymsp[0].minor.yy0, status->scanner_state);
}
// 5683 "parser.c"
        break;
      case 274:
// 2181 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("empty-array", NULL, NULL, NULL, status->scanner_state);
  yy_destructor(37,&yymsp[-1].minor);
  yy_destructor(58,&yymsp[0].minor);
}
// 5692 "parser.c"
        break;
      case 275:
// 2186 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("array", yymsp[-1].minor.yy115, NULL, NULL, status->scanner_state);
  yy_destructor(37,&yymsp[-2].minor);
  yy_destructor(58,&yymsp[0].minor);
}
// 5701 "parser.c"
        break;
      case 276:
// 2191 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_new_instance(0, yymsp[0].minor.yy0, NULL, status->scanner_state);
  yy_destructor(34,&yymsp[-1].minor);
}
// 5709 "parser.c"
        break;
      case 277:
// 2196 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_new_instance(0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(34,&yymsp[-3].minor);
  yy_destructor(55,&yymsp[-1].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5719 "parser.c"
        break;
      case 278:
// 2201 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_new_instance(0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(34,&yymsp[-4].minor);
  yy_destructor(55,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5729 "parser.c"
        break;
      case 279:
// 2206 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_new_instance(1, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(34,&yymsp[-3].minor);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 5739 "parser.c"
        break;
      case 280:
// 2211 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_new_instance(1, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(34,&yymsp[-5].minor);
  yy_destructor(48,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(55,&yymsp[-1].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5751 "parser.c"
        break;
      case 281:
// 2216 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_new_instance(1, yymsp[-4].minor.yy0, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(34,&yymsp[-6].minor);
  yy_destructor(48,&yymsp[-5].minor);
  yy_destructor(49,&yymsp[-3].minor);
  yy_destructor(55,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5763 "parser.c"
        break;
      case 282:
// 2221 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_fcall(1, yymsp[-3].minor.yy0, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(55,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5772 "parser.c"
        break;
      case 283:
// 2226 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_fcall(1, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(55,&yymsp[-1].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5781 "parser.c"
        break;
      case 284:
// 2231 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_fcall(2, yymsp[-4].minor.yy0, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(48,&yymsp[-5].minor);
  yy_destructor(49,&yymsp[-3].minor);
  yy_destructor(55,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5792 "parser.c"
        break;
      case 285:
// 2236 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_fcall(2, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(48,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(55,&yymsp[-1].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5803 "parser.c"
        break;
      case 286:
// 2241 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_scall(0, yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(91,&yymsp[-4].minor);
  yy_destructor(55,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5813 "parser.c"
        break;
      case 287:
// 2246 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_scall(0, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(91,&yymsp[-3].minor);
  yy_destructor(55,&yymsp[-1].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5823 "parser.c"
        break;
      case 288:
// 2251 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_scall(1, yymsp[-6].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(48,&yymsp[-7].minor);
  yy_destructor(49,&yymsp[-5].minor);
  yy_destructor(91,&yymsp[-4].minor);
  yy_destructor(55,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5835 "parser.c"
        break;
      case 289:
// 2256 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_scall(1, yymsp[-5].minor.yy0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(48,&yymsp[-6].minor);
  yy_destructor(49,&yymsp[-4].minor);
  yy_destructor(91,&yymsp[-3].minor);
  yy_destructor(55,&yymsp[-1].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5847 "parser.c"
        break;
      case 290:
// 2261 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_scall(1, yymsp[-8].minor.yy0, yymsp[-4].minor.yy0, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(48,&yymsp[-9].minor);
  yy_destructor(49,&yymsp[-7].minor);
  yy_destructor(91,&yymsp[-6].minor);
  yy_destructor(48,&yymsp[-5].minor);
  yy_destructor(49,&yymsp[-3].minor);
  yy_destructor(55,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5861 "parser.c"
        break;
      case 291:
// 2266 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_scall(1, yymsp[-7].minor.yy0, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(48,&yymsp[-8].minor);
  yy_destructor(49,&yymsp[-6].minor);
  yy_destructor(91,&yymsp[-5].minor);
  yy_destructor(48,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(55,&yymsp[-1].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5875 "parser.c"
        break;
      case 292:
// 2271 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_mcall(1, yymsp[-5].minor.yy115, yymsp[-3].minor.yy0, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(38,&yymsp[-4].minor);
  yy_destructor(55,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5885 "parser.c"
        break;
      case 293:
// 2276 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_mcall(1, yymsp[-4].minor.yy115, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-3].minor);
  yy_destructor(55,&yymsp[-1].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5895 "parser.c"
        break;
      case 294:
// 2281 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_mcall(2, yymsp[-7].minor.yy115, yymsp[-4].minor.yy0, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(38,&yymsp[-6].minor);
  yy_destructor(48,&yymsp[-5].minor);
  yy_destructor(49,&yymsp[-3].minor);
  yy_destructor(55,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5907 "parser.c"
        break;
      case 295:
// 2286 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_mcall(2, yymsp[-6].minor.yy115, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-5].minor);
  yy_destructor(48,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(55,&yymsp[-1].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5919 "parser.c"
        break;
      case 296:
// 2291 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_mcall(3, yymsp[-7].minor.yy115, yymsp[-4].minor.yy0, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(38,&yymsp[-6].minor);
  yy_destructor(48,&yymsp[-5].minor);
  yy_destructor(49,&yymsp[-3].minor);
  yy_destructor(55,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5931 "parser.c"
        break;
      case 297:
// 2296 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_mcall(3, yymsp[-6].minor.yy115, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-5].minor);
  yy_destructor(48,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(55,&yymsp[-1].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5943 "parser.c"
        break;
      case 306:
// 2335 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_array_item(yymsp[-2].minor.yy115, yymsp[0].minor.yy115, status->scanner_state);
  yy_destructor(76,&yymsp[-1].minor);
}
// 5951 "parser.c"
        break;
      case 307:
// 2339 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_array_item(NULL, yymsp[0].minor.yy115, status->scanner_state);
}
// 5958 "parser.c"
        break;
      case 321:
// 2396 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), yymsp[-1].minor.yy115, NULL, status->scanner_state);
  yy_destructor(37,&yymsp[-2].minor);
  yy_destructor(58,&yymsp[0].minor);
}
// 5967 "parser.c"
        break;
      case 322:
// 2401 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("property-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-1].minor);
}
// 5975 "parser.c"
        break;
      case 323:
// 2406 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("property-dynamic-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-3].minor);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 5985 "parser.c"
        break;
      case 324:
// 2411 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("property-string-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_STRING, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-3].minor);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 5995 "parser.c"
        break;
      case 326:
// 2419 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_comment(yymsp[0].minor.yy0, status->scanner_state);
}
// 6002 "parser.c"
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
// 967 "parser.lemon"


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

// 6078 "parser.c"
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
