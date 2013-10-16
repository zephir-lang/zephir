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

static json_object *xx_ret_unset_statement(json_object *expr, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("unset"));

	json_object_object_add(ret, "expr", expr);

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


// 936 "parser.c"
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
#define YYNOCODE 189
#define YYACTIONTYPE unsigned short int
#define xx_TOKENTYPE xx_parser_token*
typedef union {
  xx_TOKENTYPE yy0;
  json_object* yy119;
  int yy377;
} YYMINORTYPE;
#define YYSTACKDEPTH 100
#define xx_ARG_SDECL xx_parser_status *status;
#define xx_ARG_PDECL ,xx_parser_status *status
#define xx_ARG_FETCH xx_parser_status *status = yypParser->status
#define xx_ARG_STORE yypParser->status = status
#define YYNSTATE 669
#define YYNRULE 325
#define YYERRORSYMBOL 105
#define YYERRSYMDT yy377
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
 /*     0 */   216,  225,  228,  219,  222,   61,  653,   32,   56,   35,
 /*    10 */    37,   38,  265,  995,    1,    2,  668,    4,    5,    6,
 /*    20 */     7,    8,  654,  188,  213,  273,  278,  292,  210,  562,
 /*    30 */   288,  298,  133,  269,  169,  135,  637,  643,  642,  626,
 /*    40 */   120,  299,  379,  506,  636,  592,  287,   89,  259,  190,
 /*    50 */   120,  532,  469,  478,  487,  490,  481,  484,  493,  499,
 /*    60 */   496,  351,  502,  170,  172,  174,  620,  183,  237,  238,
 /*    70 */   234,  194,  198,  203,  373,  615,  621,  380,  306,  308,
 /*    80 */   307,  277,  291,  281,  656,  296,  309,  444,  459,  463,
 /*    90 */   466,  280,  282,  283,  284,  285,  286,  507,  216,  225,
 /*   100 */   228,  219,  222,  241,  243,  251,  245,  247,  249,  354,
 /*   110 */   265,   34,   35,   37,   38,  238,  234,  392,  270,  438,
 /*   120 */   411,  131,  213,  273,  278,  292,  210,  425,  288,  431,
 /*   130 */   411,  269,  169,   20,  639,  604,  626,  138,   23,  299,
 /*   140 */   541,  506,  592,  137,  287,  121,  259,  120,  120,  120,
 /*   150 */   469,  478,  487,  490,  481,  484,  493,  499,  496,  360,
 /*   160 */   502,  170,  172,  174,  638,  183,  641,  642,  626,  194,
 /*   170 */   198,  203,  373,  591,  592,  380,  470,  477,  125,  120,
 /*   180 */   648,  281,  659,  131,  647,  444,  459,  463,  466,  280,
 /*   190 */   282,  283,  284,  285,  286,  507,  216,  225,  228,  219,
 /*   200 */   222,  237,    3,    4,    5,    6,    7,    8,  265,  646,
 /*   210 */    15,  306,  308,  307,  277,   18,  657,  186,  303,  309,
 /*   220 */   213,  273,  278,  292,  210,  397,  288,  406,  411,  269,
 /*   230 */   169,  245,  247,  249,  500,  477,  644,  299,  525,  506,
 /*   240 */   238,  234,  287,  190,  259,  532,  472,  620,  469,  478,
 /*   250 */   487,  490,  481,  484,  493,  499,  496,  621,  502,  170,
 /*   260 */   172,  174,  410,  183,  590,  386,  138,  194,  198,  203,
 /*   270 */   373,  591,  592,  380,  293,  660,  386,  120,  131,  281,
 /*   280 */   472,  471,  324,  444,  459,  463,  466,  280,  282,  283,
 /*   290 */   284,  285,  286,  507,  216,  225,  228,  219,  222,  387,
 /*   300 */   388,  389,  390,  391,   80,   88,  265,   65,  302,   64,
 /*   310 */   387,  388,  389,  390,  391,  501,  555,  130,  213,  273,
 /*   320 */   278,  292,  210,   58,  288,   64,  551,  269,  169,   80,
 /*   330 */    88,  611,   94,  209,   23,  299,  197,  506,  614,  115,
 /*   340 */   287,   22,  259,  472,  310,  120,  469,  478,  487,  490,
 /*   350 */   481,  484,  493,  499,  496,  550,  502,  170,  172,  174,
 /*   360 */    28,  183,  237,   57,  662,  194,  198,  203,  373,  298,
 /*   370 */   472,  380,  306,  308,  307,  277,  298,  281,  504,  329,
 /*   380 */   309,  444,  459,  463,  466,  280,  282,  283,  284,  285,
 /*   390 */   286,  507,  216,  225,  228,  219,  222,  237,  655,  304,
 /*   400 */   658,  503,  477,  131,  265,  498,  321,  306,  308,  307,
 /*   410 */   277,  415,  491,  477,  331,  309,  213,  273,  278,  292,
 /*   420 */   210,  577,  288,   64,  235,  269,  169,   80,   88,  628,
 /*   430 */    94,  618,  353,  299,  554,  506,  614,  298,  287,  629,
 /*   440 */   259,  472,  479,  477,  469,  478,  487,  490,  481,  484,
 /*   450 */   493,  499,  496,   59,  502,  170,  172,  174,  355,  183,
 /*   460 */   237,  419,  663,  194,  198,  203,  373,  364,  472,  380,
 /*   470 */   306,  308,  307,  277,  617,  281,  480,  363,  309,  444,
 /*   480 */   459,  463,  466,  280,  282,  283,  284,  285,  286,  507,
 /*   490 */   216,  225,  228,  219,  222,  237,  116,  407,  606,  119,
 /*   500 */   409,  204,  265,  489,  472,  306,  308,  307,  277,  120,
 /*   510 */   488,  477,  357,  309,  213,  273,  278,  292,  210,  549,
 /*   520 */   288,  298,  396,  269,  169,   80,   88,  608,   94,  105,
 /*   530 */   414,  299,  202,  506,  614,  298,  287,  609,  259,  495,
 /*   540 */   120,  474,  469,  478,  487,  490,  481,  484,  493,  499,
 /*   550 */   496,  358,  502,  170,  172,  174,   33,  183,  237,  187,
 /*   560 */   535,  194,  198,  203,  373,  297,  298,  380,  306,  308,
 /*   570 */   307,  277,  472,  281,  291,  313,  309,  444,  459,  463,
 /*   580 */   466,  280,  282,  283,  284,  285,  286,  507,  216,  225,
 /*   590 */   228,  219,  222,  449,   41,  978,  332,  298,  616,  623,
 /*   600 */   265,  445,  450,  306,  308,  307,  277,  486,  485,  477,
 /*   610 */   270,   42,  213,  273,  278,  292,  210,  114,  288,   13,
 /*   620 */    84,  269,  169,   80,   88,   81,   94,  330,  298,  299,
 /*   630 */   546,  506,  494,  477,  287,   60,  259,  624,   63,  571,
 /*   640 */   469,  478,  487,  490,  481,  484,  493,  499,  496,  567,
 /*   650 */   502,  170,  172,  174,  427,  183,  271,  543,  314,  194,
 /*   660 */   198,  203,  373,  128,  311,  380,  131,  143,  386,  349,
 /*   670 */   339,  281,  316,  472,  447,  444,  459,  463,  466,  280,
 /*   680 */   282,  283,  284,  285,  286,  507,  216,  225,  228,  219,
 /*   690 */   222,   29,  631,   32,   56,   35,   37,   38,  265,  614,
 /*   700 */   497,  477,  387,  388,  389,  390,  391,   64,  483,  446,
 /*   710 */   213,  273,  278,  292,  210,  110,  288,  383,  113,  269,
 /*   720 */   169,  583,  338,  482,  477,  381,  443,  299,  531,  506,
 /*   730 */   439,  579,  287,  409,  259,  472,   10,  578,  469,  478,
 /*   740 */   487,  490,  481,  484,  493,  499,  496,  514,  502,  170,
 /*   750 */   172,  174,  382,  183,  237,   17,   95,  194,  198,  203,
 /*   760 */   373,  298,   30,  380,  306,  308,  307,  277,  107,  281,
 /*   770 */   492,  346,  309,  444,  459,  463,  466,  280,  282,  283,
 /*   780 */   284,  285,  286,  507,  216,  225,  228,  219,  222,  237,
 /*   790 */   665,  347,  649,   36,  647,  131,  265,  515,  108,  306,
 /*   800 */   308,  307,  277,  275,  516,  272,  350,  309,  213,  273,
 /*   810 */   278,  292,  210,  432,  288,   68,  409,  269,  169,  129,
 /*   820 */   274,  647,   67,  375,  527,  299,  182,  506,  529,  294,
 /*   830 */   287,   18,  259,  267,  476,  533,  469,  478,  487,  490,
 /*   840 */   481,  484,  493,  499,  496,  112,  502,  170,  172,  174,
 /*   850 */   664,  183,  237,   14,  538,  194,  198,  203,  373,   77,
 /*   860 */   539,  380,  306,  308,  307,  277,  669,  281,  475,  320,
 /*   870 */   309,  444,  459,  463,  466,  280,  282,  283,  284,  285,
 /*   880 */   286,  507,  216,  225,  228,  219,  222,  589,  136,  604,
 /*   890 */   626,  266,  356,  473,  265,  636,  592,  300,   78,   87,
 /*   900 */   545,  120,  301,   11,  263,   41,  213,  273,  278,  292,
 /*   910 */   210,  458,  288,  666,   40,  269,  169,  557,  456,  118,
 /*   920 */   171,  454,  565,  299,  513,  506,   25,   93,  287,  564,
 /*   930 */   259,  173,  452,   16,  469,  478,  487,  490,  481,  484,
 /*   940 */   493,  499,  496,   82,  502,  170,  172,  174,  208,  183,
 /*   950 */   312,  372,  573,  194,  198,  203,  373,   44,  667,  380,
 /*   960 */   176,  306,  308,  307,  277,  281,  141,  436,  317,  444,
 /*   970 */   459,  463,  466,  280,  282,  283,  284,  285,  286,  507,
 /*   980 */   216,  225,  228,  219,  222,  394,  140,  429,  362,  393,
 /*   990 */   585,  424,  265,  318,  319,  306,  308,  307,  277,  421,
 /*  1000 */   179,  420,  417,  416,  213,  273,  278,  292,  210,   91,
 /*  1010 */   288,  180,  595,  269,  169,  596,  236,  598,  600,  325,
 /*  1020 */    83,  299,  576,  506,  597,  601,  287,  123,  259,  602,
 /*  1030 */   603,  326,  469,  478,  487,  490,  481,  484,  493,  499,
 /*  1040 */   496,   26,  502,  170,  172,  174,  327,  183,  405,  610,
 /*  1050 */   404,  194,  198,  203,  373,  403,  612,  380,  306,  308,
 /*  1060 */   307,  277,  185,  281,  613,  401,  716,  444,  459,  463,
 /*  1070 */   466,  280,  282,  283,  284,  285,  286,  507,  216,  225,
 /*  1080 */   228,  219,  222,  394,  124,  717,   85,  398,  980,  619,
 /*  1090 */   265,  189,  622,  306,  308,  307,  277,  126,  979,   86,
 /*  1100 */   625,  191,  213,  273,  278,  292,  210,  192,  288,  127,
 /*  1110 */   634,  269,  169,   43,  630,   90,  633,  632,  342,  299,
 /*  1120 */   548,  506,  635,  361,  287,   62,  259,  385,  195,  645,
 /*  1130 */   469,  478,  487,  490,  481,  484,  493,  499,  496,  384,
 /*  1140 */   502,  170,  172,  174,  134,  183,  374,  449,  650,  194,
 /*  1150 */   198,  203,  373,  200,  652,  380,  448,  306,  308,  307,
 /*  1160 */   277,  281,  207,  515,  515,  444,  459,  463,  466,  280,
 /*  1170 */   282,  283,  284,  285,  286,  507,  216,  225,  228,  219,
 /*  1180 */   222,  394,  515,  515,  515,  402,  515,  515,  265,  515,
 /*  1190 */   515,  306,  308,  307,  277,  515,  515,  515,  515,  515,
 /*  1200 */   213,  273,  278,  292,  210,  515,  288,  515,  515,  269,
 /*  1210 */   169,  515,  515,  515,  515,  515,  515,  299,  537,  506,
 /*  1220 */   515,  515,  287,  515,  259,  515,  515,  515,  469,  478,
 /*  1230 */   487,  490,  481,  484,  493,  499,  496,  515,  502,  170,
 /*  1240 */   172,  174,  528,  183,  515,  372,  515,  194,  198,  203,
 /*  1250 */   373,  515,  515,  380,  515,  306,  308,  307,  277,  281,
 /*  1260 */   515,  515,  515,  444,  459,  463,  466,  280,  282,  283,
 /*  1270 */   284,  285,  286,  507,  216,  225,  228,  219,  222,  394,
 /*  1280 */   515,  515,  515,  408,  515,  515,  265,  515,  515,  306,
 /*  1290 */   308,  307,  277,  515,  515,  515,  515,  515,  213,  273,
 /*  1300 */   278,  292,  210,  515,  288,  515,  515,  269,  169,  515,
 /*  1310 */   515,  515,  515,  515,  515,  299,  552,  506,  515,  515,
 /*  1320 */   287,  515,  259,  515,  515,  515,  469,  478,  487,  490,
 /*  1330 */   481,  484,  493,  499,  496,  515,  502,  170,  172,  174,
 /*  1340 */   515,  183,  237,  515,  515,  194,  198,  203,  373,  515,
 /*  1350 */   515,  380,  306,  308,  307,  277,  515,  281,  515,  515,
 /*  1360 */   323,  444,  459,  463,  466,  280,  282,  283,  284,  285,
 /*  1370 */   286,  507,  216,  225,  228,  219,  222,  394,  515,  515,
 /*  1380 */   515,  418,  515,  515,  265,  515,  515,  306,  308,  307,
 /*  1390 */   277,  515,  515,  515,  515,  515,  213,  273,  278,  292,
 /*  1400 */   210,  515,  288,  515,  515,  269,  169,  515,  515,  515,
 /*  1410 */   515,  515,  515,  299,  146,  506,  515,  515,  287,  515,
 /*  1420 */   259,  515,  515,  515,  469,  478,  487,  490,  481,  484,
 /*  1430 */   493,  499,  496,  515,  502,  170,  172,  174,  175,  183,
 /*  1440 */   515,  372,  515,  194,  198,  203,  373,  515,  515,  380,
 /*  1450 */   515,  306,  308,  307,  277,  281,  515,  515,  515,  444,
 /*  1460 */   459,  463,  466,  280,  282,  283,  284,  285,  286,  507,
 /*  1470 */   216,  225,  228,  219,  222,  394,  515,  515,  515,  422,
 /*  1480 */   515,  515,  265,  515,  515,  306,  308,  307,  277,  515,
 /*  1490 */   515,  515,  515,  515,  213,  273,  278,  292,  210,  515,
 /*  1500 */   288,  515,  515,  269,  169,  515,  515,  515,  515,  515,
 /*  1510 */   515,  299,  580,  506,  515,  515,  287,  515,  259,  515,
 /*  1520 */   515,  515,  469,  478,  487,  490,  481,  484,  493,  499,
 /*  1530 */   496,  515,  502,  170,  172,  174,  184,  183,  515,  372,
 /*  1540 */   515,  194,  198,  203,  373,  515,  515,  380,  515,  306,
 /*  1550 */   308,  307,  277,  281,  515,  515,  515,  444,  459,  463,
 /*  1560 */   466,  280,  282,  283,  284,  285,  286,  507,  216,  225,
 /*  1570 */   228,  219,  222,  394,  515,  515,  515,  426,  515,  515,
 /*  1580 */   265,  515,  515,  306,  308,  307,  277,  515,  515,  515,
 /*  1590 */   515,  515,  213,  273,  278,  292,  210,  515,  288,  515,
 /*  1600 */   515,  269,  169,  515,  515,  515,  515,  515,  515,  299,
 /*  1610 */   530,  506,  515,  515,  287,  515,  259,  515,  515,  515,
 /*  1620 */   469,  478,  487,  490,  481,  484,  493,  499,  496,  515,
 /*  1630 */   502,  170,  172,  174,  199,  183,  515,  372,  515,  194,
 /*  1640 */   198,  203,  373,  515,  515,  380,  515,  306,  308,  307,
 /*  1650 */   277,  281,  515,  515,  515,  444,  459,  463,  466,  280,
 /*  1660 */   282,  283,  284,  285,  286,  507,  216,  225,  228,  219,
 /*  1670 */   222,  394,  515,  515,  515,  430,  515,  515,  265,  515,
 /*  1680 */   515,  306,  308,  307,  277,  515,  515,  515,  515,  515,
 /*  1690 */   213,  273,  278,  292,  210,  515,  288,  515,  515,  269,
 /*  1700 */   169,  515,  515,  515,  515,  515,  515,  299,  582,  506,
 /*  1710 */   515,  515,  287,  515,  259,  515,  515,  515,  469,  478,
 /*  1720 */   487,  490,  481,  484,  493,  499,  496,  515,  502,  170,
 /*  1730 */   172,  174,  515,  183,  229,  515,  515,  194,  198,  203,
 /*  1740 */   373,  515,  515,  380,  306,  308,  307,  277,  515,  281,
 /*  1750 */   515,  515,  515,  444,  459,  463,  466,  280,  282,  283,
 /*  1760 */   284,  285,  286,  507,  216,  225,  228,  219,  222,  394,
 /*  1770 */   515,  515,  515,  433,  515,  515,  265,  515,  515,  306,
 /*  1780 */   308,  307,  277,  515,  515,  515,  515,  515,  213,  273,
 /*  1790 */   278,  292,  210,  515,  288,  515,  515,  269,  169,  515,
 /*  1800 */   515,  515,  515,  515,  515,  299,  536,  506,  515,  515,
 /*  1810 */   287,  515,  259,  515,  515,  515,  469,  478,  487,  490,
 /*  1820 */   481,  484,  493,  499,  496,  515,  502,  170,  172,  174,
 /*  1830 */   515,  183,  366,  515,  515,  194,  198,  203,  373,  515,
 /*  1840 */   515,  380,  306,  308,  307,  277,  515,  281,  515,  515,
 /*  1850 */   515,  444,  459,  463,  466,  280,  282,  283,  284,  285,
 /*  1860 */   286,  507,  216,  225,  228,  219,  222,  394,  515,  515,
 /*  1870 */   515,  437,  515,  515,  265,  515,  515,  306,  308,  307,
 /*  1880 */   277,  515,  515,  515,  515,  515,  213,  273,  278,  292,
 /*  1890 */   210,  515,  288,  515,  515,  269,  169,  515,  515,  515,
 /*  1900 */   515,  515,  515,  299,  520,  506,  515,  515,  287,  515,
 /*  1910 */   259,  515,  515,  515,  469,  478,  487,  490,  481,  484,
 /*  1920 */   493,  499,  496,  515,  502,  170,  172,  174,  515,  183,
 /*  1930 */   217,  515,  515,  194,  198,  203,  373,  515,  515,  380,
 /*  1940 */   306,  308,  307,  277,  515,  281,  515,  515,  515,  444,
 /*  1950 */   459,  463,  466,  280,  282,  283,  284,  285,  286,  507,
 /*  1960 */   216,  225,  228,  219,  222,  394,  515,  515,  515,  440,
 /*  1970 */   515,  515,  265,  515,  515,  306,  308,  307,  277,  515,
 /*  1980 */   515,  515,  515,  515,  213,  273,  278,  292,  210,  515,
 /*  1990 */   288,  515,  515,  269,  169,  515,  515,  515,  515,  515,
 /*  2000 */   515,  299,  178,  506,  515,  515,  287,  515,  259,  515,
 /*  2010 */   515,  515,  469,  478,  487,  490,  481,  484,  493,  499,
 /*  2020 */   496,  515,  502,  170,  172,  174,  515,  183,  231,  515,
 /*  2030 */   515,  194,  198,  203,  373,  515,  262,  380,  306,  308,
 /*  2040 */   307,  277,  515,  281,  515,  515,  515,  444,  459,  463,
 /*  2050 */   466,  280,  282,  283,  284,  285,  286,  507,  216,  225,
 /*  2060 */   228,  219,  222,  260,  515,  515,  515,  515,  515,  515,
 /*  2070 */   265,  515,  515,  306,  308,  307,  277,  515,  515,  515,
 /*  2080 */   515,  515,  213,  273,  278,  292,  210,  515,  288,  515,
 /*  2090 */   515,  269,  169,  515,  515,  515,  515,  515,  515,  299,
 /*  2100 */   574,  506,  515,  515,  287,  515,  259,  515,  515,  515,
 /*  2110 */   469,  478,  487,  490,  481,  484,  493,  499,  496,  515,
 /*  2120 */   502,  170,  172,  174,  515,  183,  258,  515,  515,  194,
 /*  2130 */   198,  203,  373,  515,  515,  380,  306,  308,  307,  277,
 /*  2140 */   515,  281,  515,  515,  515,  444,  459,  463,  466,  280,
 /*  2150 */   282,  283,  284,  285,  286,  507,  216,  225,  228,  219,
 /*  2160 */   222,  111,  515,   32,   56,   35,   37,   38,  265,  544,
 /*  2170 */   515,   32,   56,   35,   37,   38,  515,  515,  515,  515,
 /*  2180 */   213,  273,  278,  292,  210,  515,  288,  515,  515,  269,
 /*  2190 */   169,  515,  515,  515,  515,  515,  515,  299,  570,  506,
 /*  2200 */   515,  515,  287,  515,  259,  515,  515,  515,  469,  478,
 /*  2210 */   487,  490,  481,  484,  493,  499,  496,  515,  502,  170,
 /*  2220 */   172,  174,  515,  183,  256,  515,  515,  194,  198,  203,
 /*  2230 */   373,  515,  515,  380,  306,  308,  307,  277,  515,  281,
 /*  2240 */   515,  515,  515,  444,  459,  463,  466,  280,  282,  283,
 /*  2250 */   284,  285,  286,  507,  216,  225,  228,  219,  222,  556,
 /*  2260 */   515,   32,   56,   35,   37,   38,  265,  117,  515,   32,
 /*  2270 */    56,   35,   37,   38,  515,  515,  515,  515,  213,  273,
 /*  2280 */   278,  292,  210,  515,  288,  515,  515,  269,  169,  515,
 /*  2290 */   515,  515,  515,  515,  515,  299,  526,  506,  515,  515,
 /*  2300 */   287,  515,  259,  515,  515,  515,  469,  478,  487,  490,
 /*  2310 */   481,  484,  493,  499,  496,  515,  502,  170,  172,  174,
 /*  2320 */   515,  183,  517,  515,  515,  194,  198,  203,  373,  515,
 /*  2330 */   515,  380,  306,  308,  307,  277,  515,  281,  515,  515,
 /*  2340 */   515,  444,  459,  463,  466,  280,  282,  283,  284,  285,
 /*  2350 */   286,  507,  216,  225,  228,  219,  222,  572,  515,   32,
 /*  2360 */    56,   35,   37,   38,  265,  584,  515,   32,   56,   35,
 /*  2370 */    37,   38,  515,  515,  515,  515,  213,  273,  278,  292,
 /*  2380 */   210,  515,  288,  515,  515,  269,  169,  515,  515,  515,
 /*  2390 */   515,  515,  515,  299,  542,  506,  515,  515,  287,  515,
 /*  2400 */   259,  515,  515,  515,  469,  478,  487,  490,  481,  484,
 /*  2410 */   493,  499,  496,  515,  502,  170,  172,  174,  515,  183,
 /*  2420 */   226,  515,  515,  194,  198,  203,  373,  515,  515,  380,
 /*  2430 */   306,  308,  307,  277,  515,  281,  515,  515,  515,  444,
 /*  2440 */   459,  463,  466,  280,  282,  283,  284,  285,  286,  507,
 /*  2450 */   216,  225,  228,  219,  222,  268,  515,  515,  515,  515,
 /*  2460 */   515,  515,  265,  515,  515,  306,  308,  307,  277,  515,
 /*  2470 */   515,  515,  515,  515,  213,  273,  278,  292,  210,  515,
 /*  2480 */   288,  515,  515,  269,  169,  515,  515,  515,  515,  515,
 /*  2490 */   515,  299,  206,  506,  515,  515,  287,  515,  259,  515,
 /*  2500 */   515,  515,  469,  478,  487,  490,  481,  484,  493,  499,
 /*  2510 */   496,  515,  502,  170,  172,  174,  515,  183,  264,  515,
 /*  2520 */   515,  194,  198,  203,  373,  515,  515,  380,  306,  308,
 /*  2530 */   307,  277,  515,  281,  515,  515,  515,  444,  459,  463,
 /*  2540 */   466,  280,  282,  283,  284,  285,  286,  507,  216,  225,
 /*  2550 */   228,  219,  222,  276,  515,  515,  515,  515,  515,  515,
 /*  2560 */   265,  515,  515,  306,  308,  307,  277,  515,  515,  515,
 /*  2570 */   515,  515,  213,  273,  278,  292,  210,  515,  288,  515,
 /*  2580 */   515,  269,  169,  515,  515,  515,  515,  515,  515,  299,
 /*  2590 */   586,  506,  515,  515,  287,  515,  259,  515,  515,  515,
 /*  2600 */   469,  478,  487,  490,  481,  484,  493,  499,  496,  515,
 /*  2610 */   502,  170,  172,  174,  515,  183,  250,  515,  515,  194,
 /*  2620 */   198,  203,  373,  515,  515,  380,  306,  308,  307,  277,
 /*  2630 */   515,  281,  515,  515,  515,  444,  459,  463,  466,  280,
 /*  2640 */   282,  283,  284,  285,  286,  507,  216,  225,  228,  219,
 /*  2650 */   222,  367,  515,  515,  515,  515,  515,  515,  265,  515,
 /*  2660 */   515,  306,  308,  307,  277,  515,  515,  515,  515,  515,
 /*  2670 */   213,  273,  278,  292,  210,  515,  288,  515,  515,  269,
 /*  2680 */   169,  515,  515,  515,  515,  515,  515,  299,  568,  506,
 /*  2690 */   515,  515,  287,  515,  259,  515,  515,  515,  469,  478,
 /*  2700 */   487,  490,  481,  484,  493,  499,  496,  515,  502,  170,
 /*  2710 */   172,  174,  515,  183,  211,  515,  515,  194,  198,  203,
 /*  2720 */   373,  515,  515,  380,  306,  308,  307,  277,  515,  281,
 /*  2730 */   515,  515,  515,  444,  459,  463,  466,  280,  282,  283,
 /*  2740 */   284,  285,  286,  507,  216,  225,  228,  219,  222,  522,
 /*  2750 */   515,  515,  515,  515,  515,  515,  265,  515,  515,  306,
 /*  2760 */   308,  307,  277,  515,  515,  515,  515,  515,  213,  273,
 /*  2770 */   278,  292,  210,  515,  288,  515,  515,  269,  169,  515,
 /*  2780 */   515,  515,  515,  515,  515,  299,  560,  506,  515,  515,
 /*  2790 */   287,  515,  259,  515,  515,  515,  469,  478,  487,  490,
 /*  2800 */   481,  484,  493,  499,  496,  515,  502,  170,  172,  174,
 /*  2810 */   515,  183,  223,  515,  515,  194,  198,  203,  373,  515,
 /*  2820 */   515,  380,  306,  308,  307,  277,  515,  281,  515,  515,
 /*  2830 */   515,  444,  459,  463,  466,  280,  282,  283,  284,  285,
 /*  2840 */   286,  507,  216,  225,  228,  219,  222,  239,  515,  515,
 /*  2850 */   515,  515,  515,  515,  265,  515,  515,  306,  308,  307,
 /*  2860 */   277,  515,  515,  515,  515,  515,  213,  273,  278,  292,
 /*  2870 */   210,  515,  288,  515,  515,  269,  169,  515,  515,  515,
 /*  2880 */   515,  515,  515,  299,  144,  506,  515,  515,  287,  515,
 /*  2890 */   259,  515,  515,  515,  469,  478,  487,  490,  481,  484,
 /*  2900 */   493,  499,  496,  515,  502,  170,  172,  174,  515,  183,
 /*  2910 */   368,  515,  515,  194,  198,  203,  373,  515,  515,  380,
 /*  2920 */   306,  308,  307,  277,  515,  281,  515,  515,  515,  444,
 /*  2930 */   459,  463,  466,  280,  282,  283,  284,  285,  286,  507,
 /*  2940 */   216,  225,  228,  219,  222,  248,  515,  515,  515,  515,
 /*  2950 */   515,  515,  265,  515,  515,  306,  308,  307,  277,  515,
 /*  2960 */   515,  515,  515,  515,  213,  273,  278,  292,  210,  515,
 /*  2970 */   288,  515,  515,  269,  169,  515,  515,  515,  515,  515,
 /*  2980 */   515,  299,  558,  506,  515,  515,  287,  515,  259,  515,
 /*  2990 */   515,  515,  469,  478,  487,  490,  481,  484,  493,  499,
 /*  3000 */   496,  515,  502,  170,  172,  174,  515,  183,  220,  515,
 /*  3010 */   515,  194,  198,  203,  373,  515,  515,  380,  306,  308,
 /*  3020 */   307,  277,  515,  281,  515,  515,  515,  444,  459,  463,
 /*  3030 */   466,  280,  282,  283,  284,  285,  286,  507,  216,  225,
 /*  3040 */   228,  219,  222,  279,  515,  515,  515,  515,  515,  515,
 /*  3050 */   265,  515,  515,  306,  308,  307,  277,  515,  515,  515,
 /*  3060 */   515,  515,  213,  273,  278,  292,  210,  515,  288,  515,
 /*  3070 */   515,  269,  169,  515,  515,  515,  515,  515,  515,  299,
 /*  3080 */   588,  506,  515,  515,  287,  515,  259,  515,  515,  515,
 /*  3090 */   469,  478,  487,  490,  481,  484,  493,  499,  496,  515,
 /*  3100 */   502,  170,  172,  174,  515,  183,  369,  515,  515,  194,
 /*  3110 */   198,  203,  373,  515,  515,  380,  306,  308,  307,  277,
 /*  3120 */   515,  281,  515,  515,  515,  444,  459,  463,  466,  280,
 /*  3130 */   282,  283,  284,  285,  286,  507,  216,  225,  228,  219,
 /*  3140 */   222,  246,  515,  515,  515,  515,  515,  515,  265,  515,
 /*  3150 */   515,  306,  308,  307,  277,  515,  515,  515,  515,  515,
 /*  3160 */   213,  273,  278,  292,  210,  515,  288,  515,  515,  269,
 /*  3170 */   169,  515,  515,  515,  515,  515,  515,  299,  515,  506,
 /*  3180 */   515,  515,  287,  515,  259,  515,  515,  515,  469,  478,
 /*  3190 */   487,  490,  481,  484,  493,  499,  496,  515,  502,  170,
 /*  3200 */   172,  174,  515,  183,  254,  515,  515,  194,  198,  203,
 /*  3210 */   373,  515,  515,  380,  306,  308,  307,  277,  515,  281,
 /*  3220 */   515,  515,  515,  444,  459,  463,  466,  280,  282,  283,
 /*  3230 */   284,  285,  286,  507,  216,  225,  228,  219,  222,  515,
 /*  3240 */     9,  515,  515,   12,  515,  122,  265,  651,  661,  515,
 /*  3250 */   515,  506,  515,  515,  515,  515,  515,  515,  213,  273,
 /*  3260 */   278,  292,  210,  515,  288,  515,   39,  269,  515,  515,
 /*  3270 */   515,  515,  515,  515,  515,  299,  515,  515,  515,  515,
 /*  3280 */   287,  515,  259,  515,  515,  515,   45,   46,   47,   48,
 /*  3290 */    49,   50,   51,   52,   53,   54,   55,  515,  515,  515,
 /*  3300 */   515,  515,  167,  148,   31,  507,   45,   46,   47,   48,
 /*  3310 */    49,   50,   51,   52,   53,   54,   55,  281,  515,  515,
 /*  3320 */   515,  515,  515,  581,  515,  280,  282,  283,  284,  285,
 /*  3330 */   286,  515,  508,  149,  150,  151,  152,  153,  154,  155,
 /*  3340 */   156,  157,  158,  159,  160,  161,  162,  163,  164,  165,
 /*  3350 */   166,  168,  167,  148,  515,  505,  515,  515,  515,  289,
 /*  3360 */   515,  515,  515,  515,  515,  451,  453,  455,  457,  306,
 /*  3370 */   308,  307,  277,  205,  515,   96,   97,   99,   98,  100,
 /*  3380 */   343,  515,  508,  149,  150,  151,  152,  153,  154,  155,
 /*  3390 */   156,  157,  158,  159,  160,  161,  162,  163,  164,  165,
 /*  3400 */   166,  168,  167,  148,  515,  505,  515,  515,  515,  515,
 /*  3410 */   515,  515,  515,  515,  627,  451,  453,  455,  457,  515,
 /*  3420 */   102,  103,  515,  519,  515,  515,  515,  515,  139,  515,
 /*  3430 */   101,  515,  508,  149,  150,  151,  152,  153,  154,  155,
 /*  3440 */   156,  157,  158,  159,  160,  161,  162,  163,  164,  165,
 /*  3450 */   166,  168,  167,  148,  515,  505,  515,  515,  515,  467,
 /*  3460 */   515,  515,  515,  515,  515,  451,  453,  455,  457,  306,
 /*  3470 */   308,  307,  277,  181,  515,   96,   97,   99,   98,  100,
 /*  3480 */   515,  515,  508,  149,  150,  151,  152,  153,  154,  155,
 /*  3490 */   156,  157,  158,  159,  160,  161,  162,  163,  164,  165,
 /*  3500 */   166,  168,  167,  148,  515,  505,  515,  515,  515,  515,
 /*  3510 */   515,  515,  515,  515,  607,  451,  453,  455,  457,  515,
 /*  3520 */   102,  103,  515,  177,  515,  515,  515,  515,  563,  515,
 /*  3530 */   101,  515,  508,  149,  150,  151,  152,  153,  154,  155,
 /*  3540 */   156,  157,  158,  159,  160,  161,  162,  163,  164,  165,
 /*  3550 */   166,  168,  167,  148,  515,  505,  515,  515,  399,  515,
 /*  3560 */   515,  515,  515,  515,  515,  451,  453,  455,  457,  515,
 /*  3570 */   515,  515,  386,  540,  515,   96,   97,   99,   98,  100,
 /*  3580 */   515,  515,  508,  149,  150,  151,  152,  153,  154,  155,
 /*  3590 */   156,  157,  158,  159,  160,  161,  162,  163,  164,  165,
 /*  3600 */   166,  168,  167,  148,  515,  505,  387,  388,  389,  390,
 /*  3610 */   391,  515,  515,  412,  413,  451,  453,  455,  457,  515,
 /*  3620 */   102,  103,  515,  587,  515,  515,  594,  515,  515,  515,
 /*  3630 */   101,  515,  508,  149,  150,  151,  152,  153,  154,  155,
 /*  3640 */   156,  157,  158,  159,  160,  161,  162,  163,  164,  165,
 /*  3650 */   166,  168,  167,  148,  515,  505,  515,  515,  515,  233,
 /*  3660 */   515,  515,  515,  515,  515,  451,  453,  455,  457,  306,
 /*  3670 */   308,  307,  277,  196,  515,   96,   97,   99,   98,  100,
 /*  3680 */   515,  515,  508,  149,  150,  151,  152,  153,  154,  155,
 /*  3690 */   156,  157,  158,  159,  160,  161,  162,  163,  164,  165,
 /*  3700 */   166,  168,  167,  148,  515,  505,  515,  515,  515,  515,
 /*  3710 */   515,  515,  515,  515,  464,  451,  453,  455,  457,  515,
 /*  3720 */   102,  103,  515,  378,  306,  308,  307,  277,  106,  515,
 /*  3730 */   101,  515,  508,  149,  150,  151,  152,  153,  154,  155,
 /*  3740 */   156,  157,  158,  159,  160,  161,  162,  163,  164,  165,
 /*  3750 */   166,  168,  167,  148,  515,  505,  515,  515,  515,  370,
 /*  3760 */   515,  515,  515,  515,  515,  451,  453,  455,  457,  306,
 /*  3770 */   308,  307,  277,  547,  515,   96,   97,   99,   98,  100,
 /*  3780 */   515,  515,  508,  149,  150,  151,  152,  153,  154,  155,
 /*  3790 */   156,  157,  158,  159,  160,  161,  162,  163,  164,  165,
 /*  3800 */   166,  168,  167,  148,  515,  505,  515,  515,  515,  515,
 /*  3810 */   515,  515,  515,  515,  252,  451,  453,  455,  457,  515,
 /*  3820 */   102,  103,  515,  524,  306,  308,  307,  277,  563,  515,
 /*  3830 */   101,  515,  508,  149,  150,  151,  152,  153,  154,  155,
 /*  3840 */   156,  157,  158,  159,  160,  161,  162,  163,  164,  165,
 /*  3850 */   166,  168,  167,  148,  515,  505,  515,  515,  515,  376,
 /*  3860 */   515,  515,  515,  515,  515,  451,  453,  455,  457,  306,
 /*  3870 */   308,  307,  277,  553,  515,   96,   97,   99,   98,  100,
 /*  3880 */   515,  515,  508,  149,  150,  151,  152,  153,  154,  155,
 /*  3890 */   156,  157,  158,  159,  160,  161,  162,  163,  164,  165,
 /*  3900 */   166,  168,  167,  148,  515,  505,  515,  515,  515,  515,
 /*  3910 */   515,  515,  515,  515,  244,  451,  453,  455,  457,  515,
 /*  3920 */   102,  103,  515,  193,  306,  308,  307,  277,   24,  515,
 /*  3930 */   101,  515,  508,  149,  150,  151,  152,  153,  154,  155,
 /*  3940 */   156,  157,  158,  159,  160,  161,  162,  163,  164,  165,
 /*  3950 */   166,  168,  167,  148,  515,  505,  515,  515,  515,  289,
 /*  3960 */   515,  515,  515,  515,  515,  451,  453,  455,  457,  306,
 /*  3970 */   308,  307,  277,  512,  515,  337,  515,  515,  345,  341,
 /*  3980 */   344,  515,  508,  149,  150,  151,  152,  153,  154,  155,
 /*  3990 */   156,  157,  158,  159,  160,  161,  162,  163,  164,  165,
 /*  4000 */   166,  168,  167,  148,  515,  505,  515,  515,  515,  214,
 /*  4010 */   515,  515,  515,  515,  515,  451,  453,  455,  457,  306,
 /*  4020 */   308,  307,  277,  145,  515,   96,   97,   99,   98,  100,
 /*  4030 */   515,  515,  508,  149,  150,  151,  152,  153,  154,  155,
 /*  4040 */   156,  157,  158,  159,  160,  161,  162,  163,  164,  165,
 /*  4050 */   166,  168,  167,  148,  515,  505,  515,  515,  515,  515,
 /*  4060 */   515,  515,  515,  515,  510,  451,  453,  455,  457,  515,
 /*  4070 */   102,  103,  515,  559,  306,  308,  307,  277,  139,  515,
 /*  4080 */   101,  515,  508,  149,  150,  151,  152,  153,  154,  155,
 /*  4090 */   156,  157,  158,  159,  160,  161,  162,  163,  164,  165,
 /*  4100 */   166,  168,  167,  148,  515,  505,  515,  515,  515,  371,
 /*  4110 */   515,  515,  515,  515,  515,  451,  453,  455,  457,  306,
 /*  4120 */   308,  307,  277,  569,  515,   96,   97,   99,   98,  100,
 /*  4130 */   515,  515,  508,  149,  150,  151,  152,  153,  154,  155,
 /*  4140 */   156,  157,  158,  159,  160,  161,  162,  163,  164,  165,
 /*  4150 */   166,  168,  167,  148,  515,  505,  515,  515,  515,  515,
 /*  4160 */   515,  515,  515,  515,  460,  451,  453,  455,  457,  515,
 /*  4170 */   102,  103,  515,  575,  306,  308,  307,  277,  515,  515,
 /*  4180 */   101,  515,  508,  149,  150,  151,  152,  153,  154,  155,
 /*  4190 */   156,  157,  158,  159,  160,  161,  162,  163,  164,  165,
 /*  4200 */   166,  168,  167,  148,  515,  505,  515,  515,  515,  289,
 /*  4210 */   515,  515,  515,  515,  242,  451,  453,  455,  457,  306,
 /*  4220 */   308,  307,  277,  534,  306,  308,  307,  277,  340,  341,
 /*  4230 */   344,  515,  508,  149,  150,  151,  152,  153,  154,  155,
 /*  4240 */   156,  157,  158,  159,  160,  161,  162,  163,  164,  165,
 /*  4250 */   166,  168,  167,  148,  515,  505,  515,  515,  515,  515,
 /*  4260 */   515,  515,  515,  515,  515,  451,  453,  455,  457,  515,
 /*  4270 */   515,  515,  515,  201,  515,   96,   97,   99,   98,  100,
 /*  4280 */   515,  515,  508,  149,  150,  151,  152,  153,  154,  155,
 /*  4290 */   156,  157,  158,  159,  160,  161,  162,  163,  164,  165,
 /*  4300 */   166,  168,  167,  148,  515,  505,  515,  515,  515,  515,
 /*  4310 */   515,  515,  515,  515,  515,  451,  453,  455,  457,  515,
 /*  4320 */   102,  103,  515,  701,  593,  515,  599,  515,  515,  515,
 /*  4330 */   101,  515,  147,  149,  150,  151,  152,  153,  154,  155,
 /*  4340 */   156,  157,  158,  159,  160,  161,  162,  163,  164,  165,
 /*  4350 */   166,  168,  515,  515,  515,  505,  515,  216,  225,  228,
 /*  4360 */   219,  222,  670,  515,  515,  451,  453,  455,  457,  265,
 /*  4370 */   515,  515,  515,  515,  515,  515,  515,  515,  515,  515,
 /*  4380 */   515,  213,  273,  278,  292,  210,  333,  288,  515,  515,
 /*  4390 */   269,  515,  515,  515,  515,  515,  515,  515,  299,  515,
 /*  4400 */   515,    9,  515,  287,   12,  259,  122,  515,  651,  661,
 /*  4410 */   515,  515,  506,  515,  515,  515,  257,  253,  255,  515,
 /*  4420 */   515,  212,  218,  224,  227,  230,  232,  221,  215,  241,
 /*  4430 */   243,  251,  245,  247,  249,  515,  515,  515,  515,  515,
 /*  4440 */   281,  238,  234,  515,  515,  515,  515,  515,  280,  282,
 /*  4450 */   283,  284,  285,  286,  515,  515,  216,  225,  228,  219,
 /*  4460 */   222,  515,  871,  515,  515,  515,  507,  515,  265,  515,
 /*  4470 */   515,  515,  515,  515,  515,  515,  515,  515,  515,  515,
 /*  4480 */   213,  273,  278,  292,  210,  515,  288,  515,  515,  269,
 /*  4490 */   515,  515,  515,  515,  515,  515,  515,  299,  515,  515,
 /*  4500 */   515,  515,  287,  515,  259,  515,  515,  428,  515,  515,
 /*  4510 */   515,  515,  515,  515,  515,  257,  253,  255,  515,  515,
 /*  4520 */   212,  218,  224,  227,  230,  232,  221,  215,  241,  243,
 /*  4530 */   251,  245,  247,  249,  515,  515,  515,  515,  515,  281,
 /*  4540 */   238,  234,  515,  515,  468,  515,  515,  280,  282,  283,
 /*  4550 */   284,  285,  286,  515,  515,  216,  225,  228,  219,  222,
 /*  4560 */   515,  515,  515,  515,  515,  515,  515,  265,  515,  515,
 /*  4570 */   515,  515,  515,  515,  515,  515,  515,  515,  515,  213,
 /*  4580 */   273,  278,  292,  210,  359,  288,  515,  515,  269,  515,
 /*  4590 */   515,  515,  515,  515,  515,  515,  299,  515,  515,  515,
 /*  4600 */   515,  287,  515,  259,  515,  515,  515,  515,  515,  515,
 /*  4610 */   515,  515,  515,  515,  257,  253,  255,  515,  515,  212,
 /*  4620 */   218,  224,  227,  230,  232,  221,  215,  241,  243,  251,
 /*  4630 */   245,  247,  249,  515,  515,  515,  515,  515,  281,  238,
 /*  4640 */   234,  515,  515,  465,  515,  515,  280,  282,  283,  284,
 /*  4650 */   285,  286,  515,  515,  216,  225,  228,  219,  222,  515,
 /*  4660 */   515,  515,  515,  515,  515,  515,  265,  515,  515,  515,
 /*  4670 */   515,  515,  515,  515,  515,  515,  515,  515,  213,  273,
 /*  4680 */   278,  292,  210,  515,  288,  515,  515,  269,  462,  515,
 /*  4690 */   515,  515,  515,  515,  515,  299,  515,  515,  515,  515,
 /*  4700 */   287,  515,  259,  515,  515,  515,  515,  515,  257,  253,
 /*  4710 */   255,  515,  515,  212,  218,  224,  227,  230,  232,  221,
 /*  4720 */   215,  241,  243,  251,  245,  247,  249,  515,  515,  515,
 /*  4730 */   515,  515,  515,  238,  234,  515,  515,  281,  515,  515,
 /*  4740 */   515,  515,  515,  515,  511,  280,  282,  283,  284,  285,
 /*  4750 */   286,  515,  515,  216,  225,  228,  219,  222,  515,  515,
 /*  4760 */   515,  515,  515,  515,  515,  265,  515,  515,  515,  515,
 /*  4770 */   515,  515,  515,  515,  515,  515,  515,  213,  273,  278,
 /*  4780 */   292,  210,  295,  288,  515,  515,  269,  515,  515,  515,
 /*  4790 */   515,  515,  515,  515,  299,  515,  515,  515,  515,  287,
 /*  4800 */   515,  259,  515,  515,  515,  515,  515,  257,  253,  255,
 /*  4810 */   515,  515,  212,  218,  224,  227,  230,  232,  221,  215,
 /*  4820 */   241,  243,  251,  245,  247,  249,   96,   97,   99,   98,
 /*  4830 */   100,  261,  238,  234,  515,  515,  281,  515,  515,  515,
 /*  4840 */   515,  515,  515,  515,  280,  282,  283,  284,  285,  286,
 /*  4850 */   515,  515,  216,  225,  228,  219,  222,  515,  515,  515,
 /*  4860 */   515,  515,  515,  515,  265,  515,  515,  515,  515,  515,
 /*  4870 */   515,  102,  103,  515,  705,  561,  213,  273,  278,  292,
 /*  4880 */   210,  101,  288,  515,  515,  269,  515,  515,  515,  515,
 /*  4890 */   515,  515,  515,  299,  515,  515,  515,  515,  287,  515,
 /*  4900 */   259,  515,  515,  400,  515,  515,  257,  253,  255,  515,
 /*  4910 */   515,  212,  218,  224,  227,  230,  232,  221,  215,  241,
 /*  4920 */   243,  251,  245,  247,  249,   96,   97,   99,   98,  100,
 /*  4930 */   515,  238,  234,  515,  515,  281,  515,  515,  515,  515,
 /*  4940 */   515,  515,  518,  280,  282,  283,  284,  285,  286,  515,
 /*  4950 */   515,  216,  225,  228,  219,  222,  515,  515,  515,  515,
 /*  4960 */   515,  515,  515,  265,  515,  515,  515,  515,  515,  515,
 /*  4970 */   102,  103,  515,  704,  561,  213,  273,  278,  292,  210,
 /*  4980 */   101,  288,  515,  515,  290,  515,  515,  515,  515,  515,
 /*  4990 */   515,  515,  299,  515,  515,  515,  515,  287,  515,  259,
 /*  5000 */   515,  515,  336,  515,  515,  257,  253,  255,  515,  515,
 /*  5010 */   212,  218,  224,  227,  230,  232,  221,  215,  241,  243,
 /*  5020 */   251,  245,  247,  249,   96,   97,   99,   98,  100,  515,
 /*  5030 */   238,  234,  515,  515,  335,  515,  515,  515,  515,  515,
 /*  5040 */   515,  377,  334,  282,  283,  284,  285,  286,  515,  515,
 /*  5050 */   216,  225,  228,  219,  222,  515,  515,  515,  515,  515,
 /*  5060 */   515,  515,  265,  515,  515,  515,  515,  515,  515,  102,
 /*  5070 */   103,  515,  703,  561,  213,  273,  278,  292,  210,  101,
 /*  5080 */   288,  515,  515,  269,  515,  515,  515,  515,  515,  515,
 /*  5090 */   515,  299,  515,  515,  515,  515,  287,  515,  259,  515,
 /*  5100 */   515,  435,  515,  515,  515,  515,  515,  515,  515,  257,
 /*  5110 */   253,  255,  515,  515,  212,  218,  224,  227,  230,  232,
 /*  5120 */   221,  215,  241,  243,  251,  245,  247,  249,  515,  515,
 /*  5130 */   515,  515,  515,  281,  238,  234,  515,  515,  515,  515,
 /*  5140 */   515,  280,  282,  283,  284,  285,  286,  515,  515,  216,
 /*  5150 */   225,  228,  219,  222,  515,  240,  515,  515,  515,  515,
 /*  5160 */   515,  265,  515,  515,  515,  515,  515,  515,  515,  515,
 /*  5170 */   515,  515,  515,  213,  273,  278,  292,  210,  352,  288,
 /*  5180 */   515,  515,  269,  515,  515,  515,  515,  515,  515,  515,
 /*  5190 */   299,  515,  515,  515,  515,  287,  515,  259,  515,  515,
 /*  5200 */   515,  515,  515,  515,  515,  515,  515,  515,  257,  253,
 /*  5210 */   255,  515,  515,  212,  218,  224,  227,  230,  232,  221,
 /*  5220 */   215,  241,  243,  251,  245,  247,  249,  515,  515,  515,
 /*  5230 */   515,  515,  281,  238,  234,  515,  515,  461,  515,  515,
 /*  5240 */   280,  282,  283,  284,  285,  286,  515,  515,  216,  225,
 /*  5250 */   228,  219,  222,  515,  515,  515,  515,  515,  515,  515,
 /*  5260 */   265,  515,  515,  515,  515,  515,  515,  515,  515,  515,
 /*  5270 */   515,  515,  213,  273,  278,  292,  210,  328,  288,  515,
 /*  5280 */   515,  269,  515,  515,  515,  515,  515,  515,  515,  299,
 /*  5290 */   515,  515,  515,  515,  287,  515,  259,  515,  515,  515,
 /*  5300 */   515,  515,  257,  253,  255,  515,  515,  212,  218,  224,
 /*  5310 */   227,  230,  232,  221,  215,  241,  243,  251,  245,  247,
 /*  5320 */   249,  515,  515,  515,  515,  515,  515,  238,  234,  515,
 /*  5330 */   515,  281,  515,  515,  515,  515,  515,  515,  523,  280,
 /*  5340 */   282,  283,  284,  285,  286,  515,  515,  216,  225,  228,
 /*  5350 */   219,  222,  515,  515,  515,  515,  515,  515,  515,  265,
 /*  5360 */   515,  515,  515,  515,  515,  515,  515,  515,  515,  515,
 /*  5370 */   515,  213,  273,  278,  292,  210,  305,  288,  515,  515,
 /*  5380 */   269,  515,  515,  515,  515,  515,  515,  515,  299,  515,
 /*  5390 */   515,  515,  515,  287,  515,  259,  515,  515,  515,  515,
 /*  5400 */   515,  257,  253,  255,  515,  515,  212,  218,  224,  227,
 /*  5410 */   230,  232,  221,  215,  241,  243,  251,  245,  247,  249,
 /*  5420 */   515,  515,  515,  515,  515,  515,  238,  234,  515,  515,
 /*  5430 */   281,  515,  515,  515,  515,  515,  515,  515,  280,  282,
 /*  5440 */   283,  284,  285,  286,  515,  515,  216,  225,  228,  219,
 /*  5450 */   222,  515,  515,  515,  515,  515,  515,  515,  265,  515,
 /*  5460 */   515,  515,  515,  515,  515,  515,  515,  515,  515,  515,
 /*  5470 */   213,  273,  278,  292,  210,  365,  288,  515,  515,  269,
 /*  5480 */   515,  515,  515,  515,  515,  515,  515,  299,  515,  515,
 /*  5490 */   515,  515,  287,  515,  259,  515,  515,  515,  515,  515,
 /*  5500 */   515,  253,  255,  515,  515,  212,  218,  224,  227,  230,
 /*  5510 */   232,  221,  215,  241,  243,  251,  245,  247,  249,  515,
 /*  5520 */   515,  515,  515,  515,  515,  238,  234,  515,  515,  281,
 /*  5530 */   515,  515,  515,  515,  515,  515,  515,  280,  282,  283,
 /*  5540 */   284,  285,  286,  515,  515,  216,  225,  228,  219,  222,
 /*  5550 */   515,  515,  515,  515,  515,  515,  515,  265,  515,  515,
 /*  5560 */   515,  515,  515,  515,  515,  515,  515,  515,  515,  213,
 /*  5570 */   273,  278,  292,  210,  315,  288,  515,  515,  269,  515,
 /*  5580 */   515,  515,  515,  515,  515,  515,  299,  515,  515,  515,
 /*  5590 */   515,  287,  515,  259,  515,  515,  212,  218,  224,  227,
 /*  5600 */   230,  232,  221,  215,  241,  243,  251,  245,  247,  249,
 /*  5610 */   515,  434,  395,  515,  515,  515,  238,  234,  515,  515,
 /*  5620 */   515,  515,  515,  515,  515,  386,  515,  515,  281,  515,
 /*  5630 */   515,  515,  515,  515,  515,  515,  280,  282,  283,  284,
 /*  5640 */   285,  286,  515,  515,  216,  225,  228,  219,  222,  515,
 /*  5650 */   515,  515,  515,  515,  515,  515,  265,  515,   92,  387,
 /*  5660 */   388,  389,  390,  391,  515,  423,  441,  442,  213,  273,
 /*  5670 */   278,  292,  210,  348,  288,  109,   92,  269,  515,   66,
 /*  5680 */   515,  515,  515,  515,  515,  299,  515,  515,  515,  515,
 /*  5690 */   287,  515,  259,   27,  515,  515,  515,   66,   45,   46,
 /*  5700 */    47,   48,   49,   50,   51,   52,   53,   54,   55,  515,
 /*  5710 */   515,  515,  515,  515,  515,  515,   45,   46,   47,   48,
 /*  5720 */    49,   50,   51,   52,   53,   54,   55,  281,  515,  515,
 /*  5730 */   515,  515,  515,  515,  515,  280,  282,  283,  284,  285,
 /*  5740 */   286,  515,  515,  216,  225,  228,  219,  222,  515,  515,
 /*  5750 */   515,  515,  515,  515,  515,  265,  515,   92,  515,  515,
 /*  5760 */   515,  515,  515,  515,  515,  515,  515,  213,  273,  278,
 /*  5770 */   292,  210,  322,  288,  566,   92,  269,  515,   66,  515,
 /*  5780 */   515,  515,  515,  515,  299,  515,  515,  515,  515,  287,
 /*  5790 */   515,  259,  142,  515,  515,  515,   66,   45,   46,   47,
 /*  5800 */    48,   49,   50,   51,   52,   53,   54,   55,  515,  515,
 /*  5810 */   515,  515,  515,  515,  515,   45,   46,   47,   48,   49,
 /*  5820 */    50,   51,   52,   53,   54,   55,  281,  515,  515,  515,
 /*  5830 */   515,  515,  515,  515,  280,  282,  283,  284,  285,  286,
 /*  5840 */   515,  515,  216,  225,  228,  219,  222,  515,  515,  515,
 /*  5850 */   515,  515,  515,  515,  265,   92,  515,  515,  515,  515,
 /*  5860 */   515,   76,  515,  515,  515,  515,  213,  273,  278,  292,
 /*  5870 */   210,  515,  288,  515,   79,  269,   66,  515,  515,  515,
 /*  5880 */   515,  515,  515,  299,  515,  515,  515,  515,  287,  515,
 /*  5890 */   259,  515,  515,  515,  515,   45,   46,   47,   48,   49,
 /*  5900 */    50,   51,   52,   53,   54,   55,  515,  515,  515,  515,
 /*  5910 */   515,   71,  515,  515,  515,  515,  515,  515,  521,   69,
 /*  5920 */    70,   72,   73,   75,   74,  281,  515,  515,  515,  515,
 /*  5930 */   515,  515,  515,  280,  282,  283,  284,  285,  286,  515,
 /*  5940 */   515,  216,  225,  228,  219,  222,  515,  515,  515,  515,
 /*  5950 */   515,  515,  515,  265,   39,  515,  515,  515,  515,  515,
 /*  5960 */   515,  515,  515,  515,  515,  213,  273,  278,  292,  210,
 /*  5970 */   515,  288,  515,  515,  269,  515,  515,  515,  515,  515,
 /*  5980 */   515,  515,  299,  515,  515,  515,  515,  287,  515,  259,
 /*  5990 */   515,  515,  515,  515,   45,   46,   47,   48,   49,   50,
 /*  6000 */    51,   52,   53,   54,   55,   96,   97,   99,   98,  100,
 /*  6010 */   515,  515,  515,  515,  515,  515,  515,  509,  515,  515,
 /*  6020 */   515,  515,  515,  515,  281,  515,  515,  515,  515,  515,
 /*  6030 */   515,  515,  280,  282,  283,  284,  285,  286,  515,  515,
 /*  6040 */   216,  225,  228,  219,  222,   96,   97,   99,   98,  100,
 /*  6050 */   102,  103,  265,  132,  605,  515,  599,  515,  515,  515,
 /*  6060 */   101,  515,  515,  515,  213,  273,  278,  292,  210,  515,
 /*  6070 */   288,  515,  515,  269,  515,  515,  515,  515,  515,  515,
 /*  6080 */   515,  299,  515,  515,  515,  515,  287,  515,  259,  515,
 /*  6090 */   102,  103,  515,  698,  605,  515,  599,  515,  515,  515,
 /*  6100 */   101,  515,  515,  515,  515,  515,  515,  515,  515,  515,
 /*  6110 */   515,  515,  515,  515,  515,  515,  515,  515,  515,  515,
 /*  6120 */   515,  515,  515,  281,  515,  515,  515,  515,  515,  515,
 /*  6130 */   515,  280,  282,  283,  284,  285,  286,  515,  515,  216,
 /*  6140 */   225,  228,  219,  222,   96,   97,   99,   98,  100,  515,
 /*  6150 */   515,  265,  515,  515,  515,  515,  515,  515,  515,  515,
 /*  6160 */   515,  515,  515,  213,  273,  278,  292,  210,  515,  288,
 /*  6170 */   515,  515,  290,  515,  515,  515,  515,  515,  515,  515,
 /*  6180 */   299,  515,  515,  515,  515,  287,  515,  259,  515,  102,
 /*  6190 */   103,  515,  697,  605,  515,  599,  515,  515,  515,  101,
 /*  6200 */   515,   96,   97,   99,   98,  100,   96,   97,   99,   98,
 /*  6210 */   100,  515,  515,  515,  515,  515,  515,  515,  515,  515,
 /*  6220 */   515,  515,  335,  515,   96,   97,   99,   98,  100,  515,
 /*  6230 */   334,  282,  283,  284,  285,  286,   96,   97,   99,   98,
 /*  6240 */   100,   96,   97,   99,   98,  100,  102,  103,  515,  700,
 /*  6250 */   561,  102,  103,  515,   19,  104,  101,  515,  515,  515,
 /*  6260 */   515,  101,  515,  515,  515,  515,  515,  515,  515,  102,
 /*  6270 */   103,  515,  702,  640,  515,  515,  515,  515,  515,  101,
 /*  6280 */   515,  102,  103,  515,   21,  104,  102,  103,  515,  699,
 /*  6290 */   561,  101,  515,  515,  515,  515,  101,
};
static YYCODETYPE yy_lookahead[] = {
 /*     0 */     7,    8,    9,   10,   11,  135,   40,  137,  138,  139,
 /*    10 */   140,  141,   19,  106,  107,  108,  109,  110,  111,  112,
 /*    20 */   113,  114,  116,   49,   31,   32,   33,   34,   35,  125,
 /*    30 */    37,    6,  118,   40,   41,  121,  122,  123,  124,  125,
 /*    40 */   136,   48,   49,   50,  130,  131,   53,   40,   55,   75,
 /*    50 */   136,   77,   59,   60,   61,   62,   63,   64,   65,   66,
 /*    60 */    67,   36,   69,   70,   71,   72,   40,   74,  166,   37,
 /*    70 */    38,   78,   79,   80,   81,   49,   50,   84,  176,  177,
 /*    80 */   178,  179,   55,   90,   40,  183,  184,   94,   95,   96,
 /*    90 */    97,   98,   99,  100,  101,  102,  103,  104,    7,    8,
 /*   100 */     9,   10,   11,   25,   26,   27,   28,   29,   30,   40,
 /*   110 */    19,  138,  139,  140,  141,   37,   38,  169,   91,  171,
 /*   120 */   172,   48,   31,   32,   33,   34,   35,  169,   37,  171,
 /*   130 */   172,   40,   41,  120,  123,  124,  125,  125,  125,   48,
 /*   140 */    49,   50,  131,  131,   53,  132,   55,  136,  136,  136,
 /*   150 */    59,   60,   61,   62,   63,   64,   65,   66,   67,   90,
 /*   160 */    69,   70,   71,   72,  121,   74,  123,  124,  125,   78,
 /*   170 */    79,   80,   81,  130,  131,   84,  180,  181,   43,  136,
 /*   180 */    45,   90,  117,   48,  119,   94,   95,   96,   97,   98,
 /*   190 */    99,  100,  101,  102,  103,  104,    7,    8,    9,   10,
 /*   200 */    11,  166,  109,  110,  111,  112,  113,  114,   19,   40,
 /*   210 */    43,  176,  177,  178,  179,   48,  116,   49,  183,  184,
 /*   220 */    31,   32,   33,   34,   35,  169,   37,  171,  172,   40,
 /*   230 */    41,   28,   29,   30,  180,  181,    6,   48,   49,   50,
 /*   240 */    37,   38,   53,   75,   55,   77,    6,   40,   59,   60,
 /*   250 */    61,   62,   63,   64,   65,   66,   67,   50,   69,   70,
 /*   260 */    71,   72,   37,   74,  123,   51,  125,   78,   79,   80,
 /*   270 */    81,  130,  131,   84,   40,  116,   51,  136,   48,   90,
 /*   280 */     6,   41,   48,   94,   95,   96,   97,   98,   99,  100,
 /*   290 */   101,  102,  103,  104,    7,    8,    9,   10,   11,   85,
 /*   300 */    86,   87,   88,   89,  139,  140,   19,  142,   55,    6,
 /*   310 */    85,   86,   87,   88,   89,   41,   38,  116,   31,   32,
 /*   320 */    33,   34,   35,  133,   37,    6,   48,   40,   41,  139,
 /*   330 */   140,   41,  142,   41,  125,   48,   49,   50,   48,   36,
 /*   340 */    53,  132,   55,    6,   91,  136,   59,   60,   61,   62,
 /*   350 */    63,   64,   65,   66,   67,   36,   69,   70,   71,   72,
 /*   360 */    38,   74,  166,   41,   44,   78,   79,   80,   81,    6,
 /*   370 */     6,   84,  176,  177,  178,  179,    6,   90,   41,  183,
 /*   380 */   184,   94,   95,   96,   97,   98,   99,  100,  101,  102,
 /*   390 */   103,  104,    7,    8,    9,   10,   11,  166,   43,   36,
 /*   400 */    45,  180,  181,   48,   19,   41,   36,  176,  177,  178,
 /*   410 */   179,   40,  180,  181,  183,  184,   31,   32,   33,   34,
 /*   420 */    35,  133,   37,    6,   40,   40,   41,  139,  140,   41,
 /*   430 */   142,    6,   48,   48,   49,   50,   48,    6,   53,   51,
 /*   440 */    55,    6,  180,  181,   59,   60,   61,   62,   63,   64,
 /*   450 */    65,   66,   67,   36,   69,   70,   71,   72,   49,   74,
 /*   460 */   166,   90,   40,   78,   79,   80,   81,   36,    6,   84,
 /*   470 */   176,  177,  178,  179,   49,   90,   41,  183,  184,   94,
 /*   480 */    95,   96,   97,   98,   99,  100,  101,  102,  103,  104,
 /*   490 */     7,    8,    9,   10,   11,  166,   38,  169,  125,   41,
 /*   500 */   172,   48,   19,   41,    6,  176,  177,  178,  179,  136,
 /*   510 */   180,  181,  183,  184,   31,   32,   33,   34,   35,  133,
 /*   520 */    37,    6,   40,   40,   41,  139,  140,   41,  142,  125,
 /*   530 */    48,   48,   49,   50,   48,    6,   53,   51,   55,   41,
 /*   540 */   136,   40,   59,   60,   61,   62,   63,   64,   65,   66,
 /*   550 */    67,   36,   69,   70,   71,   72,   15,   74,  166,  164,
 /*   560 */   165,   78,   79,   80,   81,   36,    6,   84,  176,  177,
 /*   570 */   178,  179,    6,   90,   55,  183,  184,   94,   95,   96,
 /*   580 */    97,   98,   99,  100,  101,  102,  103,  104,    7,    8,
 /*   590 */     9,   10,   11,  166,   20,   76,   36,    6,  128,  129,
 /*   600 */    19,  174,  175,  176,  177,  178,  179,   41,  180,  181,
 /*   610 */    91,   37,   31,   32,   33,   34,   35,  133,   37,   40,
 /*   620 */    35,   40,   41,  139,  140,   40,  142,   36,    6,   48,
 /*   630 */    49,   50,  180,  181,   53,   38,   55,  127,   41,   38,
 /*   640 */    59,   60,   61,   62,   63,   64,   65,   66,   67,   48,
 /*   650 */    69,   70,   71,   72,   37,   74,   40,   38,   36,   78,
 /*   660 */    79,   80,   81,   45,   40,   84,   48,   48,   51,   53,
 /*   670 */     6,   90,   48,    6,    6,   94,   95,   96,   97,   98,
 /*   680 */    99,  100,  101,  102,  103,  104,    7,    8,    9,   10,
 /*   690 */    11,  135,   41,  137,  138,  139,  140,  141,   19,   48,
 /*   700 */   180,  181,   85,   86,   87,   88,   89,    6,   41,   41,
 /*   710 */    31,   32,   33,   34,   35,   38,   37,    6,   41,   40,
 /*   720 */    41,   38,   58,  180,  181,  167,  168,   48,   49,   50,
 /*   730 */   169,   48,   53,  172,   55,    6,   40,   36,   59,   60,
 /*   740 */    61,   62,   63,   64,   65,   66,   67,    6,   69,   70,
 /*   750 */    71,   72,   41,   74,  166,  115,  136,   78,   79,   80,
 /*   760 */    81,    6,   41,   84,  176,  177,  178,  179,   40,   90,
 /*   770 */    41,  183,  184,   94,   95,   96,   97,   98,   99,  100,
 /*   780 */   101,  102,  103,  104,    7,    8,    9,   10,   11,  166,
 /*   790 */    43,   36,  117,   35,  119,   48,   19,   40,   55,  176,
 /*   800 */   177,  178,  179,    6,   82,   55,  183,  184,   31,   32,
 /*   810 */    33,   34,   35,  169,   37,  126,  172,   40,   41,  117,
 /*   820 */    40,  119,   51,   82,   79,   48,   49,   50,   41,   55,
 /*   830 */    53,   48,   55,   20,  126,   76,   59,   60,   61,   62,
 /*   840 */    63,   64,   65,   66,   67,   41,   69,   70,   71,   72,
 /*   850 */   116,   74,  166,  115,   73,   78,   79,   80,   81,   91,
 /*   860 */    48,   84,  176,  177,  178,  179,    0,   90,   51,  183,
 /*   870 */   184,   94,   95,   96,   97,   98,   99,  100,  101,  102,
 /*   880 */   103,  104,    7,    8,    9,   10,   11,  122,  123,  124,
 /*   890 */   125,   40,   55,  181,   19,  130,  131,   40,   53,  126,
 /*   900 */    48,  136,   49,   41,   36,   20,   31,   32,   33,   34,
 /*   910 */    35,   41,   37,   40,   40,   40,   41,   48,   41,   41,
 /*   920 */    41,   41,   55,   48,   49,   50,   40,   40,   53,   40,
 /*   930 */    55,   41,   41,   40,   59,   60,   61,   62,   63,   64,
 /*   940 */    65,   66,   67,   51,   69,   70,   71,   72,  163,   74,
 /*   950 */    55,  166,   48,   78,   79,   80,   81,   20,  116,   84,
 /*   960 */    48,  176,  177,  178,  179,   90,   55,  169,   40,   94,
 /*   970 */    95,   96,   97,   98,   99,  100,  101,  102,  103,  104,
 /*   980 */     7,    8,    9,   10,   11,  166,   40,  169,   55,  170,
 /*   990 */    48,   40,   19,   49,   55,  176,  177,  178,  179,  169,
 /*  1000 */    73,   49,  169,   49,   31,   32,   33,   34,   35,  126,
 /*  1010 */    37,   48,   53,   40,   41,   51,   55,   41,   53,   40,
 /*  1020 */   126,   48,   49,   50,  126,   51,   53,   40,   55,  126,
 /*  1030 */    41,   49,   59,   60,   61,   62,   63,   64,   65,   66,
 /*  1040 */    67,   55,   69,   70,   71,   72,   55,   74,  166,  126,
 /*  1050 */    58,   78,   79,   80,   81,  173,  127,   84,  176,  177,
 /*  1060 */   178,  179,   48,   90,   41,  169,   41,   94,   95,   96,
 /*  1070 */    97,   98,   99,  100,  101,  102,  103,  104,    7,    8,
 /*  1080 */     9,   10,   11,  166,  116,   41,   40,  170,   76,  129,
 /*  1090 */    19,  165,   40,  176,  177,  178,  179,   40,   76,   51,
 /*  1100 */    41,  126,   31,   32,   33,   34,   35,   76,   37,  116,
 /*  1110 */   127,   40,   41,   58,  126,   51,   41,  127,   76,   48,
 /*  1120 */    49,   50,   41,   49,   53,   41,   55,   40,   48,  119,
 /*  1130 */    59,   60,   61,   62,   63,   64,   65,   66,   67,  168,
 /*  1140 */    69,   70,   71,   72,   49,   74,   40,  166,  116,   78,
 /*  1150 */    79,   80,   81,   48,   44,   84,  175,  176,  177,  178,
 /*  1160 */   179,   90,   79,  188,  188,   94,   95,   96,   97,   98,
 /*  1170 */    99,  100,  101,  102,  103,  104,    7,    8,    9,   10,
 /*  1180 */    11,  166,  188,  188,  188,  170,  188,  188,   19,  188,
 /*  1190 */   188,  176,  177,  178,  179,  188,  188,  188,  188,  188,
 /*  1200 */    31,   32,   33,   34,   35,  188,   37,  188,  188,   40,
 /*  1210 */    41,  188,  188,  188,  188,  188,  188,   48,   49,   50,
 /*  1220 */   188,  188,   53,  188,   55,  188,  188,  188,   59,   60,
 /*  1230 */    61,   62,   63,   64,   65,   66,   67,  188,   69,   70,
 /*  1240 */    71,   72,  163,   74,  188,  166,  188,   78,   79,   80,
 /*  1250 */    81,  188,  188,   84,  188,  176,  177,  178,  179,   90,
 /*  1260 */   188,  188,  188,   94,   95,   96,   97,   98,   99,  100,
 /*  1270 */   101,  102,  103,  104,    7,    8,    9,   10,   11,  166,
 /*  1280 */   188,  188,  188,  170,  188,  188,   19,  188,  188,  176,
 /*  1290 */   177,  178,  179,  188,  188,  188,  188,  188,   31,   32,
 /*  1300 */    33,   34,   35,  188,   37,  188,  188,   40,   41,  188,
 /*  1310 */   188,  188,  188,  188,  188,   48,   49,   50,  188,  188,
 /*  1320 */    53,  188,   55,  188,  188,  188,   59,   60,   61,   62,
 /*  1330 */    63,   64,   65,   66,   67,  188,   69,   70,   71,   72,
 /*  1340 */   188,   74,  166,  188,  188,   78,   79,   80,   81,  188,
 /*  1350 */   188,   84,  176,  177,  178,  179,  188,   90,  188,  188,
 /*  1360 */   184,   94,   95,   96,   97,   98,   99,  100,  101,  102,
 /*  1370 */   103,  104,    7,    8,    9,   10,   11,  166,  188,  188,
 /*  1380 */   188,  170,  188,  188,   19,  188,  188,  176,  177,  178,
 /*  1390 */   179,  188,  188,  188,  188,  188,   31,   32,   33,   34,
 /*  1400 */    35,  188,   37,  188,  188,   40,   41,  188,  188,  188,
 /*  1410 */   188,  188,  188,   48,   49,   50,  188,  188,   53,  188,
 /*  1420 */    55,  188,  188,  188,   59,   60,   61,   62,   63,   64,
 /*  1430 */    65,   66,   67,  188,   69,   70,   71,   72,  163,   74,
 /*  1440 */   188,  166,  188,   78,   79,   80,   81,  188,  188,   84,
 /*  1450 */   188,  176,  177,  178,  179,   90,  188,  188,  188,   94,
 /*  1460 */    95,   96,   97,   98,   99,  100,  101,  102,  103,  104,
 /*  1470 */     7,    8,    9,   10,   11,  166,  188,  188,  188,  170,
 /*  1480 */   188,  188,   19,  188,  188,  176,  177,  178,  179,  188,
 /*  1490 */   188,  188,  188,  188,   31,   32,   33,   34,   35,  188,
 /*  1500 */    37,  188,  188,   40,   41,  188,  188,  188,  188,  188,
 /*  1510 */   188,   48,   49,   50,  188,  188,   53,  188,   55,  188,
 /*  1520 */   188,  188,   59,   60,   61,   62,   63,   64,   65,   66,
 /*  1530 */    67,  188,   69,   70,   71,   72,  163,   74,  188,  166,
 /*  1540 */   188,   78,   79,   80,   81,  188,  188,   84,  188,  176,
 /*  1550 */   177,  178,  179,   90,  188,  188,  188,   94,   95,   96,
 /*  1560 */    97,   98,   99,  100,  101,  102,  103,  104,    7,    8,
 /*  1570 */     9,   10,   11,  166,  188,  188,  188,  170,  188,  188,
 /*  1580 */    19,  188,  188,  176,  177,  178,  179,  188,  188,  188,
 /*  1590 */   188,  188,   31,   32,   33,   34,   35,  188,   37,  188,
 /*  1600 */   188,   40,   41,  188,  188,  188,  188,  188,  188,   48,
 /*  1610 */    49,   50,  188,  188,   53,  188,   55,  188,  188,  188,
 /*  1620 */    59,   60,   61,   62,   63,   64,   65,   66,   67,  188,
 /*  1630 */    69,   70,   71,   72,  163,   74,  188,  166,  188,   78,
 /*  1640 */    79,   80,   81,  188,  188,   84,  188,  176,  177,  178,
 /*  1650 */   179,   90,  188,  188,  188,   94,   95,   96,   97,   98,
 /*  1660 */    99,  100,  101,  102,  103,  104,    7,    8,    9,   10,
 /*  1670 */    11,  166,  188,  188,  188,  170,  188,  188,   19,  188,
 /*  1680 */   188,  176,  177,  178,  179,  188,  188,  188,  188,  188,
 /*  1690 */    31,   32,   33,   34,   35,  188,   37,  188,  188,   40,
 /*  1700 */    41,  188,  188,  188,  188,  188,  188,   48,   49,   50,
 /*  1710 */   188,  188,   53,  188,   55,  188,  188,  188,   59,   60,
 /*  1720 */    61,   62,   63,   64,   65,   66,   67,  188,   69,   70,
 /*  1730 */    71,   72,  188,   74,  166,  188,  188,   78,   79,   80,
 /*  1740 */    81,  188,  188,   84,  176,  177,  178,  179,  188,   90,
 /*  1750 */   188,  188,  188,   94,   95,   96,   97,   98,   99,  100,
 /*  1760 */   101,  102,  103,  104,    7,    8,    9,   10,   11,  166,
 /*  1770 */   188,  188,  188,  170,  188,  188,   19,  188,  188,  176,
 /*  1780 */   177,  178,  179,  188,  188,  188,  188,  188,   31,   32,
 /*  1790 */    33,   34,   35,  188,   37,  188,  188,   40,   41,  188,
 /*  1800 */   188,  188,  188,  188,  188,   48,   49,   50,  188,  188,
 /*  1810 */    53,  188,   55,  188,  188,  188,   59,   60,   61,   62,
 /*  1820 */    63,   64,   65,   66,   67,  188,   69,   70,   71,   72,
 /*  1830 */   188,   74,  166,  188,  188,   78,   79,   80,   81,  188,
 /*  1840 */   188,   84,  176,  177,  178,  179,  188,   90,  188,  188,
 /*  1850 */   188,   94,   95,   96,   97,   98,   99,  100,  101,  102,
 /*  1860 */   103,  104,    7,    8,    9,   10,   11,  166,  188,  188,
 /*  1870 */   188,  170,  188,  188,   19,  188,  188,  176,  177,  178,
 /*  1880 */   179,  188,  188,  188,  188,  188,   31,   32,   33,   34,
 /*  1890 */    35,  188,   37,  188,  188,   40,   41,  188,  188,  188,
 /*  1900 */   188,  188,  188,   48,   49,   50,  188,  188,   53,  188,
 /*  1910 */    55,  188,  188,  188,   59,   60,   61,   62,   63,   64,
 /*  1920 */    65,   66,   67,  188,   69,   70,   71,   72,  188,   74,
 /*  1930 */   166,  188,  188,   78,   79,   80,   81,  188,  188,   84,
 /*  1940 */   176,  177,  178,  179,  188,   90,  188,  188,  188,   94,
 /*  1950 */    95,   96,   97,   98,   99,  100,  101,  102,  103,  104,
 /*  1960 */     7,    8,    9,   10,   11,  166,  188,  188,  188,  170,
 /*  1970 */   188,  188,   19,  188,  188,  176,  177,  178,  179,  188,
 /*  1980 */   188,  188,  188,  188,   31,   32,   33,   34,   35,  188,
 /*  1990 */    37,  188,  188,   40,   41,  188,  188,  188,  188,  188,
 /*  2000 */   188,   48,   49,   50,  188,  188,   53,  188,   55,  188,
 /*  2010 */   188,  188,   59,   60,   61,   62,   63,   64,   65,   66,
 /*  2020 */    67,  188,   69,   70,   71,   72,  188,   74,  166,  188,
 /*  2030 */   188,   78,   79,   80,   81,  188,  139,   84,  176,  177,
 /*  2040 */   178,  179,  188,   90,  188,  188,  188,   94,   95,   96,
 /*  2050 */    97,   98,   99,  100,  101,  102,  103,  104,    7,    8,
 /*  2060 */     9,   10,   11,  166,  188,  188,  188,  188,  188,  188,
 /*  2070 */    19,  188,  188,  176,  177,  178,  179,  188,  188,  188,
 /*  2080 */   188,  188,   31,   32,   33,   34,   35,  188,   37,  188,
 /*  2090 */   188,   40,   41,  188,  188,  188,  188,  188,  188,   48,
 /*  2100 */    49,   50,  188,  188,   53,  188,   55,  188,  188,  188,
 /*  2110 */    59,   60,   61,   62,   63,   64,   65,   66,   67,  188,
 /*  2120 */    69,   70,   71,   72,  188,   74,  166,  188,  188,   78,
 /*  2130 */    79,   80,   81,  188,  188,   84,  176,  177,  178,  179,
 /*  2140 */   188,   90,  188,  188,  188,   94,   95,   96,   97,   98,
 /*  2150 */    99,  100,  101,  102,  103,  104,    7,    8,    9,   10,
 /*  2160 */    11,  135,  188,  137,  138,  139,  140,  141,   19,  135,
 /*  2170 */   188,  137,  138,  139,  140,  141,  188,  188,  188,  188,
 /*  2180 */    31,   32,   33,   34,   35,  188,   37,  188,  188,   40,
 /*  2190 */    41,  188,  188,  188,  188,  188,  188,   48,   49,   50,
 /*  2200 */   188,  188,   53,  188,   55,  188,  188,  188,   59,   60,
 /*  2210 */    61,   62,   63,   64,   65,   66,   67,  188,   69,   70,
 /*  2220 */    71,   72,  188,   74,  166,  188,  188,   78,   79,   80,
 /*  2230 */    81,  188,  188,   84,  176,  177,  178,  179,  188,   90,
 /*  2240 */   188,  188,  188,   94,   95,   96,   97,   98,   99,  100,
 /*  2250 */   101,  102,  103,  104,    7,    8,    9,   10,   11,  135,
 /*  2260 */   188,  137,  138,  139,  140,  141,   19,  135,  188,  137,
 /*  2270 */   138,  139,  140,  141,  188,  188,  188,  188,   31,   32,
 /*  2280 */    33,   34,   35,  188,   37,  188,  188,   40,   41,  188,
 /*  2290 */   188,  188,  188,  188,  188,   48,   49,   50,  188,  188,
 /*  2300 */    53,  188,   55,  188,  188,  188,   59,   60,   61,   62,
 /*  2310 */    63,   64,   65,   66,   67,  188,   69,   70,   71,   72,
 /*  2320 */   188,   74,  166,  188,  188,   78,   79,   80,   81,  188,
 /*  2330 */   188,   84,  176,  177,  178,  179,  188,   90,  188,  188,
 /*  2340 */   188,   94,   95,   96,   97,   98,   99,  100,  101,  102,
 /*  2350 */   103,  104,    7,    8,    9,   10,   11,  135,  188,  137,
 /*  2360 */   138,  139,  140,  141,   19,  135,  188,  137,  138,  139,
 /*  2370 */   140,  141,  188,  188,  188,  188,   31,   32,   33,   34,
 /*  2380 */    35,  188,   37,  188,  188,   40,   41,  188,  188,  188,
 /*  2390 */   188,  188,  188,   48,   49,   50,  188,  188,   53,  188,
 /*  2400 */    55,  188,  188,  188,   59,   60,   61,   62,   63,   64,
 /*  2410 */    65,   66,   67,  188,   69,   70,   71,   72,  188,   74,
 /*  2420 */   166,  188,  188,   78,   79,   80,   81,  188,  188,   84,
 /*  2430 */   176,  177,  178,  179,  188,   90,  188,  188,  188,   94,
 /*  2440 */    95,   96,   97,   98,   99,  100,  101,  102,  103,  104,
 /*  2450 */     7,    8,    9,   10,   11,  166,  188,  188,  188,  188,
 /*  2460 */   188,  188,   19,  188,  188,  176,  177,  178,  179,  188,
 /*  2470 */   188,  188,  188,  188,   31,   32,   33,   34,   35,  188,
 /*  2480 */    37,  188,  188,   40,   41,  188,  188,  188,  188,  188,
 /*  2490 */   188,   48,   49,   50,  188,  188,   53,  188,   55,  188,
 /*  2500 */   188,  188,   59,   60,   61,   62,   63,   64,   65,   66,
 /*  2510 */    67,  188,   69,   70,   71,   72,  188,   74,  166,  188,
 /*  2520 */   188,   78,   79,   80,   81,  188,  188,   84,  176,  177,
 /*  2530 */   178,  179,  188,   90,  188,  188,  188,   94,   95,   96,
 /*  2540 */    97,   98,   99,  100,  101,  102,  103,  104,    7,    8,
 /*  2550 */     9,   10,   11,  166,  188,  188,  188,  188,  188,  188,
 /*  2560 */    19,  188,  188,  176,  177,  178,  179,  188,  188,  188,
 /*  2570 */   188,  188,   31,   32,   33,   34,   35,  188,   37,  188,
 /*  2580 */   188,   40,   41,  188,  188,  188,  188,  188,  188,   48,
 /*  2590 */    49,   50,  188,  188,   53,  188,   55,  188,  188,  188,
 /*  2600 */    59,   60,   61,   62,   63,   64,   65,   66,   67,  188,
 /*  2610 */    69,   70,   71,   72,  188,   74,  166,  188,  188,   78,
 /*  2620 */    79,   80,   81,  188,  188,   84,  176,  177,  178,  179,
 /*  2630 */   188,   90,  188,  188,  188,   94,   95,   96,   97,   98,
 /*  2640 */    99,  100,  101,  102,  103,  104,    7,    8,    9,   10,
 /*  2650 */    11,  166,  188,  188,  188,  188,  188,  188,   19,  188,
 /*  2660 */   188,  176,  177,  178,  179,  188,  188,  188,  188,  188,
 /*  2670 */    31,   32,   33,   34,   35,  188,   37,  188,  188,   40,
 /*  2680 */    41,  188,  188,  188,  188,  188,  188,   48,   49,   50,
 /*  2690 */   188,  188,   53,  188,   55,  188,  188,  188,   59,   60,
 /*  2700 */    61,   62,   63,   64,   65,   66,   67,  188,   69,   70,
 /*  2710 */    71,   72,  188,   74,  166,  188,  188,   78,   79,   80,
 /*  2720 */    81,  188,  188,   84,  176,  177,  178,  179,  188,   90,
 /*  2730 */   188,  188,  188,   94,   95,   96,   97,   98,   99,  100,
 /*  2740 */   101,  102,  103,  104,    7,    8,    9,   10,   11,  166,
 /*  2750 */   188,  188,  188,  188,  188,  188,   19,  188,  188,  176,
 /*  2760 */   177,  178,  179,  188,  188,  188,  188,  188,   31,   32,
 /*  2770 */    33,   34,   35,  188,   37,  188,  188,   40,   41,  188,
 /*  2780 */   188,  188,  188,  188,  188,   48,   49,   50,  188,  188,
 /*  2790 */    53,  188,   55,  188,  188,  188,   59,   60,   61,   62,
 /*  2800 */    63,   64,   65,   66,   67,  188,   69,   70,   71,   72,
 /*  2810 */   188,   74,  166,  188,  188,   78,   79,   80,   81,  188,
 /*  2820 */   188,   84,  176,  177,  178,  179,  188,   90,  188,  188,
 /*  2830 */   188,   94,   95,   96,   97,   98,   99,  100,  101,  102,
 /*  2840 */   103,  104,    7,    8,    9,   10,   11,  166,  188,  188,
 /*  2850 */   188,  188,  188,  188,   19,  188,  188,  176,  177,  178,
 /*  2860 */   179,  188,  188,  188,  188,  188,   31,   32,   33,   34,
 /*  2870 */    35,  188,   37,  188,  188,   40,   41,  188,  188,  188,
 /*  2880 */   188,  188,  188,   48,   49,   50,  188,  188,   53,  188,
 /*  2890 */    55,  188,  188,  188,   59,   60,   61,   62,   63,   64,
 /*  2900 */    65,   66,   67,  188,   69,   70,   71,   72,  188,   74,
 /*  2910 */   166,  188,  188,   78,   79,   80,   81,  188,  188,   84,
 /*  2920 */   176,  177,  178,  179,  188,   90,  188,  188,  188,   94,
 /*  2930 */    95,   96,   97,   98,   99,  100,  101,  102,  103,  104,
 /*  2940 */     7,    8,    9,   10,   11,  166,  188,  188,  188,  188,
 /*  2950 */   188,  188,   19,  188,  188,  176,  177,  178,  179,  188,
 /*  2960 */   188,  188,  188,  188,   31,   32,   33,   34,   35,  188,
 /*  2970 */    37,  188,  188,   40,   41,  188,  188,  188,  188,  188,
 /*  2980 */   188,   48,   49,   50,  188,  188,   53,  188,   55,  188,
 /*  2990 */   188,  188,   59,   60,   61,   62,   63,   64,   65,   66,
 /*  3000 */    67,  188,   69,   70,   71,   72,  188,   74,  166,  188,
 /*  3010 */   188,   78,   79,   80,   81,  188,  188,   84,  176,  177,
 /*  3020 */   178,  179,  188,   90,  188,  188,  188,   94,   95,   96,
 /*  3030 */    97,   98,   99,  100,  101,  102,  103,  104,    7,    8,
 /*  3040 */     9,   10,   11,  166,  188,  188,  188,  188,  188,  188,
 /*  3050 */    19,  188,  188,  176,  177,  178,  179,  188,  188,  188,
 /*  3060 */   188,  188,   31,   32,   33,   34,   35,  188,   37,  188,
 /*  3070 */   188,   40,   41,  188,  188,  188,  188,  188,  188,   48,
 /*  3080 */    49,   50,  188,  188,   53,  188,   55,  188,  188,  188,
 /*  3090 */    59,   60,   61,   62,   63,   64,   65,   66,   67,  188,
 /*  3100 */    69,   70,   71,   72,  188,   74,  166,  188,  188,   78,
 /*  3110 */    79,   80,   81,  188,  188,   84,  176,  177,  178,  179,
 /*  3120 */   188,   90,  188,  188,  188,   94,   95,   96,   97,   98,
 /*  3130 */    99,  100,  101,  102,  103,  104,    7,    8,    9,   10,
 /*  3140 */    11,  166,  188,  188,  188,  188,  188,  188,   19,  188,
 /*  3150 */   188,  176,  177,  178,  179,  188,  188,  188,  188,  188,
 /*  3160 */    31,   32,   33,   34,   35,  188,   37,  188,  188,   40,
 /*  3170 */    41,  188,  188,  188,  188,  188,  188,   48,  188,   50,
 /*  3180 */   188,  188,   53,  188,   55,  188,  188,  188,   59,   60,
 /*  3190 */    61,   62,   63,   64,   65,   66,   67,  188,   69,   70,
 /*  3200 */    71,   72,  188,   74,  166,  188,  188,   78,   79,   80,
 /*  3210 */    81,  188,  188,   84,  176,  177,  178,  179,  188,   90,
 /*  3220 */   188,  188,  188,   94,   95,   96,   97,   98,   99,  100,
 /*  3230 */   101,  102,  103,  104,    7,    8,    9,   10,   11,  188,
 /*  3240 */    39,  188,  188,   42,  188,   44,   19,   46,   47,  188,
 /*  3250 */   188,   50,  188,  188,  188,  188,  188,  188,   31,   32,
 /*  3260 */    33,   34,   35,  188,   37,  188,   19,   40,  188,  188,
 /*  3270 */   188,  188,  188,  188,  188,   48,  188,  188,  188,  188,
 /*  3280 */    53,  188,   55,  188,  188,  188,   59,   60,   61,   62,
 /*  3290 */    63,   64,   65,   66,   67,   68,   69,  188,  188,  188,
 /*  3300 */   188,  188,  113,  114,   57,  104,   59,   60,   61,   62,
 /*  3310 */    63,   64,   65,   66,   67,   68,   69,   90,  188,  188,
 /*  3320 */   188,  188,  188,  134,  188,   98,   99,  100,  101,  102,
 /*  3330 */   103,  188,  143,  144,  145,  146,  147,  148,  149,  150,
 /*  3340 */   151,  152,  153,  154,  155,  156,  157,  158,  159,  160,
 /*  3350 */   161,  162,  113,  114,  188,  166,  188,  188,  188,  166,
 /*  3360 */   188,  188,  188,  188,  188,  176,  177,  178,  179,  176,
 /*  3370 */   177,  178,  179,  134,  188,    1,    2,    3,    4,    5,
 /*  3380 */   187,  188,  143,  144,  145,  146,  147,  148,  149,  150,
 /*  3390 */   151,  152,  153,  154,  155,  156,  157,  158,  159,  160,
 /*  3400 */   161,  162,  113,  114,  188,  166,  188,  188,  188,  188,
 /*  3410 */   188,  188,  188,  188,   40,  176,  177,  178,  179,  188,
 /*  3420 */    46,   47,  188,  134,  188,  188,  188,  188,   54,  188,
 /*  3430 */    56,  188,  143,  144,  145,  146,  147,  148,  149,  150,
 /*  3440 */   151,  152,  153,  154,  155,  156,  157,  158,  159,  160,
 /*  3450 */   161,  162,  113,  114,  188,  166,  188,  188,  188,  166,
 /*  3460 */   188,  188,  188,  188,  188,  176,  177,  178,  179,  176,
 /*  3470 */   177,  178,  179,  134,  188,    1,    2,    3,    4,    5,
 /*  3480 */   188,  188,  143,  144,  145,  146,  147,  148,  149,  150,
 /*  3490 */   151,  152,  153,  154,  155,  156,  157,  158,  159,  160,
 /*  3500 */   161,  162,  113,  114,  188,  166,  188,  188,  188,  188,
 /*  3510 */   188,  188,  188,  188,   40,  176,  177,  178,  179,  188,
 /*  3520 */    46,   47,  188,  134,  188,  188,  188,  188,   54,  188,
 /*  3530 */    56,  188,  143,  144,  145,  146,  147,  148,  149,  150,
 /*  3540 */   151,  152,  153,  154,  155,  156,  157,  158,  159,  160,
 /*  3550 */   161,  162,  113,  114,  188,  166,  188,  188,   37,  188,
 /*  3560 */   188,  188,  188,  188,  188,  176,  177,  178,  179,  188,
 /*  3570 */   188,  188,   51,  134,  188,    1,    2,    3,    4,    5,
 /*  3580 */   188,  188,  143,  144,  145,  146,  147,  148,  149,  150,
 /*  3590 */   151,  152,  153,  154,  155,  156,  157,  158,  159,  160,
 /*  3600 */   161,  162,  113,  114,  188,  166,   85,   86,   87,   88,
 /*  3610 */    89,  188,  188,   92,   93,  176,  177,  178,  179,  188,
 /*  3620 */    46,   47,  188,  134,  188,  188,   52,  188,  188,  188,
 /*  3630 */    56,  188,  143,  144,  145,  146,  147,  148,  149,  150,
 /*  3640 */   151,  152,  153,  154,  155,  156,  157,  158,  159,  160,
 /*  3650 */   161,  162,  113,  114,  188,  166,  188,  188,  188,  166,
 /*  3660 */   188,  188,  188,  188,  188,  176,  177,  178,  179,  176,
 /*  3670 */   177,  178,  179,  134,  188,    1,    2,    3,    4,    5,
 /*  3680 */   188,  188,  143,  144,  145,  146,  147,  148,  149,  150,
 /*  3690 */   151,  152,  153,  154,  155,  156,  157,  158,  159,  160,
 /*  3700 */   161,  162,  113,  114,  188,  166,  188,  188,  188,  188,
 /*  3710 */   188,  188,  188,  188,  166,  176,  177,  178,  179,  188,
 /*  3720 */    46,   47,  188,  134,  176,  177,  178,  179,   54,  188,
 /*  3730 */    56,  188,  143,  144,  145,  146,  147,  148,  149,  150,
 /*  3740 */   151,  152,  153,  154,  155,  156,  157,  158,  159,  160,
 /*  3750 */   161,  162,  113,  114,  188,  166,  188,  188,  188,  166,
 /*  3760 */   188,  188,  188,  188,  188,  176,  177,  178,  179,  176,
 /*  3770 */   177,  178,  179,  134,  188,    1,    2,    3,    4,    5,
 /*  3780 */   188,  188,  143,  144,  145,  146,  147,  148,  149,  150,
 /*  3790 */   151,  152,  153,  154,  155,  156,  157,  158,  159,  160,
 /*  3800 */   161,  162,  113,  114,  188,  166,  188,  188,  188,  188,
 /*  3810 */   188,  188,  188,  188,  166,  176,  177,  178,  179,  188,
 /*  3820 */    46,   47,  188,  134,  176,  177,  178,  179,   54,  188,
 /*  3830 */    56,  188,  143,  144,  145,  146,  147,  148,  149,  150,
 /*  3840 */   151,  152,  153,  154,  155,  156,  157,  158,  159,  160,
 /*  3850 */   161,  162,  113,  114,  188,  166,  188,  188,  188,  166,
 /*  3860 */   188,  188,  188,  188,  188,  176,  177,  178,  179,  176,
 /*  3870 */   177,  178,  179,  134,  188,    1,    2,    3,    4,    5,
 /*  3880 */   188,  188,  143,  144,  145,  146,  147,  148,  149,  150,
 /*  3890 */   151,  152,  153,  154,  155,  156,  157,  158,  159,  160,
 /*  3900 */   161,  162,  113,  114,  188,  166,  188,  188,  188,  188,
 /*  3910 */   188,  188,  188,  188,  166,  176,  177,  178,  179,  188,
 /*  3920 */    46,   47,  188,  134,  176,  177,  178,  179,   54,  188,
 /*  3930 */    56,  188,  143,  144,  145,  146,  147,  148,  149,  150,
 /*  3940 */   151,  152,  153,  154,  155,  156,  157,  158,  159,  160,
 /*  3950 */   161,  162,  113,  114,  188,  166,  188,  188,  188,  166,
 /*  3960 */   188,  188,  188,  188,  188,  176,  177,  178,  179,  176,
 /*  3970 */   177,  178,  179,  134,  188,  182,  188,  188,  185,  186,
 /*  3980 */   187,  188,  143,  144,  145,  146,  147,  148,  149,  150,
 /*  3990 */   151,  152,  153,  154,  155,  156,  157,  158,  159,  160,
 /*  4000 */   161,  162,  113,  114,  188,  166,  188,  188,  188,  166,
 /*  4010 */   188,  188,  188,  188,  188,  176,  177,  178,  179,  176,
 /*  4020 */   177,  178,  179,  134,  188,    1,    2,    3,    4,    5,
 /*  4030 */   188,  188,  143,  144,  145,  146,  147,  148,  149,  150,
 /*  4040 */   151,  152,  153,  154,  155,  156,  157,  158,  159,  160,
 /*  4050 */   161,  162,  113,  114,  188,  166,  188,  188,  188,  188,
 /*  4060 */   188,  188,  188,  188,  166,  176,  177,  178,  179,  188,
 /*  4070 */    46,   47,  188,  134,  176,  177,  178,  179,   54,  188,
 /*  4080 */    56,  188,  143,  144,  145,  146,  147,  148,  149,  150,
 /*  4090 */   151,  152,  153,  154,  155,  156,  157,  158,  159,  160,
 /*  4100 */   161,  162,  113,  114,  188,  166,  188,  188,  188,  166,
 /*  4110 */   188,  188,  188,  188,  188,  176,  177,  178,  179,  176,
 /*  4120 */   177,  178,  179,  134,  188,    1,    2,    3,    4,    5,
 /*  4130 */   188,  188,  143,  144,  145,  146,  147,  148,  149,  150,
 /*  4140 */   151,  152,  153,  154,  155,  156,  157,  158,  159,  160,
 /*  4150 */   161,  162,  113,  114,  188,  166,  188,  188,  188,  188,
 /*  4160 */   188,  188,  188,  188,  166,  176,  177,  178,  179,  188,
 /*  4170 */    46,   47,  188,  134,  176,  177,  178,  179,  188,  188,
 /*  4180 */    56,  188,  143,  144,  145,  146,  147,  148,  149,  150,
 /*  4190 */   151,  152,  153,  154,  155,  156,  157,  158,  159,  160,
 /*  4200 */   161,  162,  113,  114,  188,  166,  188,  188,  188,  166,
 /*  4210 */   188,  188,  188,  188,  166,  176,  177,  178,  179,  176,
 /*  4220 */   177,  178,  179,  134,  176,  177,  178,  179,  185,  186,
 /*  4230 */   187,  188,  143,  144,  145,  146,  147,  148,  149,  150,
 /*  4240 */   151,  152,  153,  154,  155,  156,  157,  158,  159,  160,
 /*  4250 */   161,  162,  113,  114,  188,  166,  188,  188,  188,  188,
 /*  4260 */   188,  188,  188,  188,  188,  176,  177,  178,  179,  188,
 /*  4270 */   188,  188,  188,  134,  188,    1,    2,    3,    4,    5,
 /*  4280 */   188,  188,  143,  144,  145,  146,  147,  148,  149,  150,
 /*  4290 */   151,  152,  153,  154,  155,  156,  157,  158,  159,  160,
 /*  4300 */   161,  162,  113,  114,  188,  166,  188,  188,  188,  188,
 /*  4310 */   188,  188,  188,  188,  188,  176,  177,  178,  179,  188,
 /*  4320 */    46,   47,  188,   49,   50,  188,   52,  188,  188,  188,
 /*  4330 */    56,  188,  143,  144,  145,  146,  147,  148,  149,  150,
 /*  4340 */   151,  152,  153,  154,  155,  156,  157,  158,  159,  160,
 /*  4350 */   161,  162,  188,  188,  188,  166,  188,    7,    8,    9,
 /*  4360 */    10,   11,    0,  188,  188,  176,  177,  178,  179,   19,
 /*  4370 */   188,  188,  188,  188,  188,  188,  188,  188,  188,  188,
 /*  4380 */   188,   31,   32,   33,   34,   35,   36,   37,  188,  188,
 /*  4390 */    40,  188,  188,  188,  188,  188,  188,  188,   48,  188,
 /*  4400 */   188,   39,  188,   53,   42,   55,   44,  188,   46,   47,
 /*  4410 */   188,  188,   50,  188,  188,  188,   12,   13,   14,  188,
 /*  4420 */   188,   17,   18,   19,   20,   21,   22,   23,   24,   25,
 /*  4430 */    26,   27,   28,   29,   30,  188,  188,  188,  188,  188,
 /*  4440 */    90,   37,   38,  188,  188,  188,  188,  188,   98,   99,
 /*  4450 */   100,  101,  102,  103,  188,  188,    7,    8,    9,   10,
 /*  4460 */    11,  188,   58,  188,  188,  188,  104,  188,   19,  188,
 /*  4470 */   188,  188,  188,  188,  188,  188,  188,  188,  188,  188,
 /*  4480 */    31,   32,   33,   34,   35,  188,   37,  188,  188,   40,
 /*  4490 */   188,  188,  188,  188,  188,  188,  188,   48,  188,  188,
 /*  4500 */   188,  188,   53,  188,   55,  188,  188,   58,  188,  188,
 /*  4510 */   188,  188,  188,  188,  188,   12,   13,   14,  188,  188,
 /*  4520 */    17,   18,   19,   20,   21,   22,   23,   24,   25,   26,
 /*  4530 */    27,   28,   29,   30,  188,  188,  188,  188,  188,   90,
 /*  4540 */    37,   38,  188,  188,   41,  188,  188,   98,   99,  100,
 /*  4550 */   101,  102,  103,  188,  188,    7,    8,    9,   10,   11,
 /*  4560 */   188,  188,  188,  188,  188,  188,  188,   19,  188,  188,
 /*  4570 */   188,  188,  188,  188,  188,  188,  188,  188,  188,   31,
 /*  4580 */    32,   33,   34,   35,   36,   37,  188,  188,   40,  188,
 /*  4590 */   188,  188,  188,  188,  188,  188,   48,  188,  188,  188,
 /*  4600 */   188,   53,  188,   55,  188,  188,  188,  188,  188,  188,
 /*  4610 */   188,  188,  188,  188,   12,   13,   14,  188,  188,   17,
 /*  4620 */    18,   19,   20,   21,   22,   23,   24,   25,   26,   27,
 /*  4630 */    28,   29,   30,  188,  188,  188,  188,  188,   90,   37,
 /*  4640 */    38,  188,  188,   41,  188,  188,   98,   99,  100,  101,
 /*  4650 */   102,  103,  188,  188,    7,    8,    9,   10,   11,  188,
 /*  4660 */   188,  188,  188,  188,  188,  188,   19,  188,  188,  188,
 /*  4670 */   188,  188,  188,  188,  188,  188,  188,  188,   31,   32,
 /*  4680 */    33,   34,   35,  188,   37,  188,  188,   40,   41,  188,
 /*  4690 */   188,  188,  188,  188,  188,   48,  188,  188,  188,  188,
 /*  4700 */    53,  188,   55,  188,  188,  188,  188,  188,   12,   13,
 /*  4710 */    14,  188,  188,   17,   18,   19,   20,   21,   22,   23,
 /*  4720 */    24,   25,   26,   27,   28,   29,   30,  188,  188,  188,
 /*  4730 */   188,  188,  188,   37,   38,  188,  188,   90,  188,  188,
 /*  4740 */   188,  188,  188,  188,   48,   98,   99,  100,  101,  102,
 /*  4750 */   103,  188,  188,    7,    8,    9,   10,   11,  188,  188,
 /*  4760 */   188,  188,  188,  188,  188,   19,  188,  188,  188,  188,
 /*  4770 */   188,  188,  188,  188,  188,  188,  188,   31,   32,   33,
 /*  4780 */    34,   35,   36,   37,  188,  188,   40,  188,  188,  188,
 /*  4790 */   188,  188,  188,  188,   48,  188,  188,  188,  188,   53,
 /*  4800 */   188,   55,  188,  188,  188,  188,  188,   12,   13,   14,
 /*  4810 */   188,  188,   17,   18,   19,   20,   21,   22,   23,   24,
 /*  4820 */    25,   26,   27,   28,   29,   30,    1,    2,    3,    4,
 /*  4830 */     5,   36,   37,   38,  188,  188,   90,  188,  188,  188,
 /*  4840 */   188,  188,  188,  188,   98,   99,  100,  101,  102,  103,
 /*  4850 */   188,  188,    7,    8,    9,   10,   11,  188,  188,  188,
 /*  4860 */   188,  188,  188,  188,   19,  188,  188,  188,  188,  188,
 /*  4870 */   188,   46,   47,  188,   49,   50,   31,   32,   33,   34,
 /*  4880 */    35,   56,   37,  188,  188,   40,  188,  188,  188,  188,
 /*  4890 */   188,  188,  188,   48,  188,  188,  188,  188,   53,  188,
 /*  4900 */    55,  188,  188,   58,  188,  188,   12,   13,   14,  188,
 /*  4910 */   188,   17,   18,   19,   20,   21,   22,   23,   24,   25,
 /*  4920 */    26,   27,   28,   29,   30,    1,    2,    3,    4,    5,
 /*  4930 */   188,   37,   38,  188,  188,   90,  188,  188,  188,  188,
 /*  4940 */   188,  188,   48,   98,   99,  100,  101,  102,  103,  188,
 /*  4950 */   188,    7,    8,    9,   10,   11,  188,  188,  188,  188,
 /*  4960 */   188,  188,  188,   19,  188,  188,  188,  188,  188,  188,
 /*  4970 */    46,   47,  188,   49,   50,   31,   32,   33,   34,   35,
 /*  4980 */    56,   37,  188,  188,   40,  188,  188,  188,  188,  188,
 /*  4990 */   188,  188,   48,  188,  188,  188,  188,   53,  188,   55,
 /*  5000 */   188,  188,   58,  188,  188,   12,   13,   14,  188,  188,
 /*  5010 */    17,   18,   19,   20,   21,   22,   23,   24,   25,   26,
 /*  5020 */    27,   28,   29,   30,    1,    2,    3,    4,    5,  188,
 /*  5030 */    37,   38,  188,  188,   90,  188,  188,  188,  188,  188,
 /*  5040 */   188,   48,   98,   99,  100,  101,  102,  103,  188,  188,
 /*  5050 */     7,    8,    9,   10,   11,  188,  188,  188,  188,  188,
 /*  5060 */   188,  188,   19,  188,  188,  188,  188,  188,  188,   46,
 /*  5070 */    47,  188,   49,   50,   31,   32,   33,   34,   35,   56,
 /*  5080 */    37,  188,  188,   40,  188,  188,  188,  188,  188,  188,
 /*  5090 */   188,   48,  188,  188,  188,  188,   53,  188,   55,  188,
 /*  5100 */   188,   58,  188,  188,  188,  188,  188,  188,  188,   12,
 /*  5110 */    13,   14,  188,  188,   17,   18,   19,   20,   21,   22,
 /*  5120 */    23,   24,   25,   26,   27,   28,   29,   30,  188,  188,
 /*  5130 */   188,  188,  188,   90,   37,   38,  188,  188,  188,  188,
 /*  5140 */   188,   98,   99,  100,  101,  102,  103,  188,  188,    7,
 /*  5150 */     8,    9,   10,   11,  188,   58,  188,  188,  188,  188,
 /*  5160 */   188,   19,  188,  188,  188,  188,  188,  188,  188,  188,
 /*  5170 */   188,  188,  188,   31,   32,   33,   34,   35,   36,   37,
 /*  5180 */   188,  188,   40,  188,  188,  188,  188,  188,  188,  188,
 /*  5190 */    48,  188,  188,  188,  188,   53,  188,   55,  188,  188,
 /*  5200 */   188,  188,  188,  188,  188,  188,  188,  188,   12,   13,
 /*  5210 */    14,  188,  188,   17,   18,   19,   20,   21,   22,   23,
 /*  5220 */    24,   25,   26,   27,   28,   29,   30,  188,  188,  188,
 /*  5230 */   188,  188,   90,   37,   38,  188,  188,   41,  188,  188,
 /*  5240 */    98,   99,  100,  101,  102,  103,  188,  188,    7,    8,
 /*  5250 */     9,   10,   11,  188,  188,  188,  188,  188,  188,  188,
 /*  5260 */    19,  188,  188,  188,  188,  188,  188,  188,  188,  188,
 /*  5270 */   188,  188,   31,   32,   33,   34,   35,   36,   37,  188,
 /*  5280 */   188,   40,  188,  188,  188,  188,  188,  188,  188,   48,
 /*  5290 */   188,  188,  188,  188,   53,  188,   55,  188,  188,  188,
 /*  5300 */   188,  188,   12,   13,   14,  188,  188,   17,   18,   19,
 /*  5310 */    20,   21,   22,   23,   24,   25,   26,   27,   28,   29,
 /*  5320 */    30,  188,  188,  188,  188,  188,  188,   37,   38,  188,
 /*  5330 */   188,   90,  188,  188,  188,  188,  188,  188,   48,   98,
 /*  5340 */    99,  100,  101,  102,  103,  188,  188,    7,    8,    9,
 /*  5350 */    10,   11,  188,  188,  188,  188,  188,  188,  188,   19,
 /*  5360 */   188,  188,  188,  188,  188,  188,  188,  188,  188,  188,
 /*  5370 */   188,   31,   32,   33,   34,   35,   36,   37,  188,  188,
 /*  5380 */    40,  188,  188,  188,  188,  188,  188,  188,   48,  188,
 /*  5390 */   188,  188,  188,   53,  188,   55,  188,  188,  188,  188,
 /*  5400 */   188,   12,   13,   14,  188,  188,   17,   18,   19,   20,
 /*  5410 */    21,   22,   23,   24,   25,   26,   27,   28,   29,   30,
 /*  5420 */   188,  188,  188,  188,  188,  188,   37,   38,  188,  188,
 /*  5430 */    90,  188,  188,  188,  188,  188,  188,  188,   98,   99,
 /*  5440 */   100,  101,  102,  103,  188,  188,    7,    8,    9,   10,
 /*  5450 */    11,  188,  188,  188,  188,  188,  188,  188,   19,  188,
 /*  5460 */   188,  188,  188,  188,  188,  188,  188,  188,  188,  188,
 /*  5470 */    31,   32,   33,   34,   35,   36,   37,  188,  188,   40,
 /*  5480 */   188,  188,  188,  188,  188,  188,  188,   48,  188,  188,
 /*  5490 */   188,  188,   53,  188,   55,  188,  188,  188,  188,  188,
 /*  5500 */   188,   13,   14,  188,  188,   17,   18,   19,   20,   21,
 /*  5510 */    22,   23,   24,   25,   26,   27,   28,   29,   30,  188,
 /*  5520 */   188,  188,  188,  188,  188,   37,   38,  188,  188,   90,
 /*  5530 */   188,  188,  188,  188,  188,  188,  188,   98,   99,  100,
 /*  5540 */   101,  102,  103,  188,  188,    7,    8,    9,   10,   11,
 /*  5550 */   188,  188,  188,  188,  188,  188,  188,   19,  188,  188,
 /*  5560 */   188,  188,  188,  188,  188,  188,  188,  188,  188,   31,
 /*  5570 */    32,   33,   34,   35,   36,   37,  188,  188,   40,  188,
 /*  5580 */   188,  188,  188,  188,  188,  188,   48,  188,  188,  188,
 /*  5590 */   188,   53,  188,   55,  188,  188,   17,   18,   19,   20,
 /*  5600 */    21,   22,   23,   24,   25,   26,   27,   28,   29,   30,
 /*  5610 */   188,   37,   38,  188,  188,  188,   37,   38,  188,  188,
 /*  5620 */   188,  188,  188,  188,  188,   51,  188,  188,   90,  188,
 /*  5630 */   188,  188,  188,  188,  188,  188,   98,   99,  100,  101,
 /*  5640 */   102,  103,  188,  188,    7,    8,    9,   10,   11,  188,
 /*  5650 */   188,  188,  188,  188,  188,  188,   19,  188,   19,   85,
 /*  5660 */    86,   87,   88,   89,  188,   91,   92,   93,   31,   32,
 /*  5670 */    33,   34,   35,   36,   37,   36,   19,   40,  188,   40,
 /*  5680 */   188,  188,  188,  188,  188,   48,  188,  188,  188,  188,
 /*  5690 */    53,  188,   55,   36,  188,  188,  188,   40,   59,   60,
 /*  5700 */    61,   62,   63,   64,   65,   66,   67,   68,   69,  188,
 /*  5710 */   188,  188,  188,  188,  188,  188,   59,   60,   61,   62,
 /*  5720 */    63,   64,   65,   66,   67,   68,   69,   90,  188,  188,
 /*  5730 */   188,  188,  188,  188,  188,   98,   99,  100,  101,  102,
 /*  5740 */   103,  188,  188,    7,    8,    9,   10,   11,  188,  188,
 /*  5750 */   188,  188,  188,  188,  188,   19,  188,   19,  188,  188,
 /*  5760 */   188,  188,  188,  188,  188,  188,  188,   31,   32,   33,
 /*  5770 */    34,   35,   36,   37,   36,   19,   40,  188,   40,  188,
 /*  5780 */   188,  188,  188,  188,   48,  188,  188,  188,  188,   53,
 /*  5790 */   188,   55,   36,  188,  188,  188,   40,   59,   60,   61,
 /*  5800 */    62,   63,   64,   65,   66,   67,   68,   69,  188,  188,
 /*  5810 */   188,  188,  188,  188,  188,   59,   60,   61,   62,   63,
 /*  5820 */    64,   65,   66,   67,   68,   69,   90,  188,  188,  188,
 /*  5830 */   188,  188,  188,  188,   98,   99,  100,  101,  102,  103,
 /*  5840 */   188,  188,    7,    8,    9,   10,   11,  188,  188,  188,
 /*  5850 */   188,  188,  188,  188,   19,   19,  188,  188,  188,  188,
 /*  5860 */   188,   40,  188,  188,  188,  188,   31,   32,   33,   34,
 /*  5870 */    35,  188,   37,  188,   53,   40,   40,  188,  188,  188,
 /*  5880 */   188,  188,  188,   48,  188,  188,  188,  188,   53,  188,
 /*  5890 */    55,  188,  188,  188,  188,   59,   60,   61,   62,   63,
 /*  5900 */    64,   65,   66,   67,   68,   69,  188,  188,  188,  188,
 /*  5910 */   188,   90,  188,  188,  188,  188,  188,  188,   83,   98,
 /*  5920 */    99,  100,  101,  102,  103,   90,  188,  188,  188,  188,
 /*  5930 */   188,  188,  188,   98,   99,  100,  101,  102,  103,  188,
 /*  5940 */   188,    7,    8,    9,   10,   11,  188,  188,  188,  188,
 /*  5950 */   188,  188,  188,   19,   19,  188,  188,  188,  188,  188,
 /*  5960 */   188,  188,  188,  188,  188,   31,   32,   33,   34,   35,
 /*  5970 */   188,   37,  188,  188,   40,  188,  188,  188,  188,  188,
 /*  5980 */   188,  188,   48,  188,  188,  188,  188,   53,  188,   55,
 /*  5990 */   188,  188,  188,  188,   59,   60,   61,   62,   63,   64,
 /*  6000 */    65,   66,   67,   68,   69,    1,    2,    3,    4,    5,
 /*  6010 */   188,  188,  188,  188,  188,  188,  188,   83,  188,  188,
 /*  6020 */   188,  188,  188,  188,   90,  188,  188,  188,  188,  188,
 /*  6030 */   188,  188,   98,   99,  100,  101,  102,  103,  188,  188,
 /*  6040 */     7,    8,    9,   10,   11,    1,    2,    3,    4,    5,
 /*  6050 */    46,   47,   19,   49,   50,  188,   52,  188,  188,  188,
 /*  6060 */    56,  188,  188,  188,   31,   32,   33,   34,   35,  188,
 /*  6070 */    37,  188,  188,   40,  188,  188,  188,  188,  188,  188,
 /*  6080 */   188,   48,  188,  188,  188,  188,   53,  188,   55,  188,
 /*  6090 */    46,   47,  188,   49,   50,  188,   52,  188,  188,  188,
 /*  6100 */    56,  188,  188,  188,  188,  188,  188,  188,  188,  188,
 /*  6110 */   188,  188,  188,  188,  188,  188,  188,  188,  188,  188,
 /*  6120 */   188,  188,  188,   90,  188,  188,  188,  188,  188,  188,
 /*  6130 */   188,   98,   99,  100,  101,  102,  103,  188,  188,    7,
 /*  6140 */     8,    9,   10,   11,    1,    2,    3,    4,    5,  188,
 /*  6150 */   188,   19,  188,  188,  188,  188,  188,  188,  188,  188,
 /*  6160 */   188,  188,  188,   31,   32,   33,   34,   35,  188,   37,
 /*  6170 */   188,  188,   40,  188,  188,  188,  188,  188,  188,  188,
 /*  6180 */    48,  188,  188,  188,  188,   53,  188,   55,  188,   46,
 /*  6190 */    47,  188,   49,   50,  188,   52,  188,  188,  188,   56,
 /*  6200 */   188,    1,    2,    3,    4,    5,    1,    2,    3,    4,
 /*  6210 */     5,  188,  188,  188,  188,  188,  188,  188,  188,  188,
 /*  6220 */   188,  188,   90,  188,    1,    2,    3,    4,    5,  188,
 /*  6230 */    98,   99,  100,  101,  102,  103,    1,    2,    3,    4,
 /*  6240 */     5,    1,    2,    3,    4,    5,   46,   47,  188,   49,
 /*  6250 */    50,   46,   47,  188,   49,   50,   56,  188,  188,  188,
 /*  6260 */   188,   56,  188,  188,  188,  188,  188,  188,  188,   46,
 /*  6270 */    47,  188,   49,   50,  188,  188,  188,  188,  188,   56,
 /*  6280 */   188,   46,   47,  188,   49,   50,   46,   47,  188,   49,
 /*  6290 */    50,   56,  188,  188,  188,  188,   56,
};
#define YY_SHIFT_USE_DFLT (-35)
static short yy_shift_ofst[] = {
 /*     0 */  3201,  866, 4362,  -35,  -35,  -35,  -35,  -35,  -35,  696,
 /*    10 */   862,  -35,  579,  167,  -35,  893,  783,  -35, 6205,  -35,
 /*    20 */  6235,  -35,  -35, 3874,  886,  986, 5657,  322, 3247,  721,
 /*    30 */   -35,  -35,  541, 5935,  -35,  758,  -35,  -35,  -35,  874,
 /*    40 */   574,  -35, 1055,  937,  -35,  -35,  -35,  -35,  -35,  -35,
 /*    50 */   -35,  -35,  -35,  -35,  -35,  -35,  -35,  -35,  417,  597,
 /*    60 */  3247, 1084,  -35,  -35, 5836,  -35,  771, 5821,  -35,  -35,
 /*    70 */   -35,  -35,  -35,  -35,  -35,  -35,  768,  845,  -35,  -35,
 /*    80 */   585,  892, 5821,  -35, 1046, 1048, 5821,  -35,    7, 1064,
 /*    90 */  5821,  -35,  887,  885,  -35,  -35,  -35,  -35,  -35,  -35,
 /*   100 */   -35,  -35,  -35,  -35, 4124, 3674,  728,  743, 5639,  677,
 /*   110 */  3247,  804,  -35,  -35,  303,  458, 3247,  878,  -35,  -35,
 /*   120 */   -35,  -35,  987,  135,  -35, 1057,  618,  -35,  169,  230,
 /*   130 */   -35, 6004,  -35, 1095,  -35, 6143, 6200,  -35, 4024,  946,
 /*   140 */   911, 5756,  619, 2835,  -35, 1365,  -35,  -35,  -35,  -35,
 /*   150 */   -35,  -35,  -35,  -35,  -35,  -35,  -35,  -35,  -35,  -35,
 /*   160 */   -35,  -35,  -35,  -35,  -35,  -35,  -35,  -35,  -35,  -35,
 /*   170 */   879,  -35,  890,  -35, 6033,  912, 1169, 1953,  927,  963,
 /*   180 */  1757,  777,  -35, 6033, 1014,  168,  -35,  -26,  -35,  -35,
 /*   190 */  5821, 1031, 3129, 3129, 1080,  679,  287,  -35, 6033, 1105,
 /*   200 */  1561,  483,  -35,  453, 2247, 2443, 1083, 6033,  292,  -35,
 /*   210 */  6033,   32, 6033, 6033,   32, 6033, 6033, 5389, 6033, 6033,
 /*   220 */  5389, 6033, 6033, 5389, 6033, 6033, 5389, 6033, 6033, 5389,
 /*   230 */  6033,   78, 6033,   78,  384,  961, 5142, 5389, 6033, 5097,
 /*   240 */   -35, 6033,  203, 6033,  203, 6033,   32, 6033,   32, 6033,
 /*   250 */    32, 6033,  203, 6033, 5579, 6033, 5579, 6033, 5488, 3227,
 /*   260 */  4795,  -35,  868, 6033,   32,  851,  813, 6033,   78,   27,
 /*   270 */   616,  750, 5637,  780,  797, 6033,   32,  -35, 6033,   32,
 /*   280 */   -35,  -35,  -35,  -35,  -35,  -35,  -35,  -35, 4944, 5389,
 /*   290 */   519, 4350,  234,  774, 4746,  -35,  529,  -35, 6033,  857,
 /*   300 */   853,  253, 5340,  363,  -35,  -35,  -35,  -35,  -35,  -35,
 /*   310 */   624,  895, 5538,  622,  -35,  -35,  928,  944,  939, 5736,
 /*   320 */   370,  -35,  -35,  -35,  979,  982,  991, 5241,  -35,  591,
 /*   330 */   -35,  560,  -35,  -35, 1012, 1022,  -35,  664,  -35, 6132,
 /*   340 */   -35, 1042, 6033,  -35,  -35,  -35,  755,  -35,  -35,  -35,
 /*   350 */    25,  -35,  -35,   69,  409,  837, 4548,  515,  -35,  -35,
 /*   360 */  1074,  933, 5439,  431,  -35,  -35,   78,   78,   78,   78,
 /*   370 */    78,   78, 5389, 1106,  741, 5934, 4993, 3129,   -7,  -35,
 /*   380 */  1087,  711,  -35, 1087,  -35, 5574,  -35,  -35,  -35,  -35,
 /*   390 */   -35,  -35, 6033,  -35, 5389,  482, 3521, 6033,  -35, 4845,
 /*   400 */   214, 6033,  -35,  992,  -35, 4404,  225, 6033,  -35,  -35,
 /*   410 */  6033,  -35,  -35,  -35,  371,  954,  214, 6033,  -35,  952,
 /*   420 */   214, 6033,  -35,  951,  617, 6033,  -35, 4449,  214, 6033,
 /*   430 */   -35,  225, 6033,  -35, 5043,  214, 6033,  -35,  225, 6033,
 /*   440 */   -35,  -35,  -35,  -35, 6033,  668,  -35, 6033,  -35, 5389,
 /*   450 */   -35,  891,  -35,  880,  -35,  877,  -35,  870,  -35, 4647,
 /*   460 */  5196,  -35,  -35, 6033, 4602,  -35, 6033, 4503,  -35,  501,
 /*   470 */   240,  -35,  501,  -35,  817, 5821,  -35,  -35,  501,  435,
 /*   480 */   -35,  501,  667,  -35,  501,  566,  -35,  501,  462,  -35,
 /*   490 */   501,  729,  -35,  501,  498,  -35,  501,  364,  -35,  501,
 /*   500 */   274,  -35,  501,  337,  -35, 5389,  -35,  -35,  -35, 6033,
 /*   510 */  4696, 3129,  875,  -35,  757,  722, 5835, 4894, 3129, 1855,
 /*   520 */   -35, 6033, 5290, 3129,  189,  -35,  745, 6033,  787,  -35,
 /*   530 */   -35,  -35,  759, 3129, 3129,  -35,  -35,  781,  812, 2345,
 /*   540 */    91,  -35,  -35, 3247,  852,  581,  -35, 1071,  -35,  319,
 /*   550 */   278, 1267,  -35,  385,  -35, 3247,  869, 2933,  -35, 2737,
 /*   560 */   -35, 4124, 3774,  889,  867, 5738,  601, 2639,  -35, 2149,
 /*   570 */   -35, 3247,  904, 2051,  -35,  973,  -35,  701,  683, 1463,
 /*   580 */   -35, 1659,  -35, 3247,  942, 2541,  -35, 3031,  -35, 4274,
 /*   590 */  4924,  -35,  -35, 3574,  959,  964, 5821,  976,  -35,  965,
 /*   600 */   974, 5821,  989,  -35,  -35, 3574, 3474,  486,  -35, 5821,
 /*   610 */   290,  -35, 1023,  -35,   26, 1025,  425, 1044,  207,  -35,
 /*   620 */   -35, 1052,  -35,  -35, 1059,  -35, 3374,  388,  -35, 5821,
 /*   630 */   651,  -35, 1075,  -35, 1081,  -35,  -35, 6044, 6223, 4825,
 /*   640 */  4124, 5023,  -35, 6240,  169,  -35,  -35,  -35,  169,  230,
 /*   650 */   -35, 1110,  -34,  355,  -35,   44,   73,  -35,  169,  230,
 /*   660 */   -35,  320,  422,  747,  -35,  873,   73,  -35,  -35,
};
#define YY_REDUCE_USE_DFLT (-131)
static short yy_reduce_ofst[] = {
 /*     0 */   -93, -131,   93, -131, -131, -131, -131, -131, -131, -131,
 /*    10 */  -131, -131, -131,  738, -131, -131,  640, -131,   13, -131,
 /*    20 */   209, -131, -131,  620, -131, -131,  190, -131,  556, -131,
 /*    30 */  -131, -131, -131,  -27, -131, -131, -131, -131, -131, -131,
 /*    40 */  -131, -131, -131, -131, -131, -131, -131, -131, -131, -131,
 /*    50 */  -131, -131, -131, -131, -131, -131, -131, -131, -131, -131,
 /*    60 */  -130, -131, -131, -131,  165, -131, -131,  689, -131, -131,
 /*    70 */  -131, -131, -131, -131, -131, -131, -131, -131, -131, -131,
 /*    80 */  -131, -131,  894, -131, -131, -131,  773, -131, -131, -131,
 /*    90 */   883, -131, -131, -131, -131, -131, -131, -131, -131, -131,
 /*   100 */  -131, -131, -131, -131,  404,  620, -131, -131,  484, -131,
 /*   110 */  2026, -131, -131, -131, -131, -131, 2132, -131, -131, -131,
 /*   120 */  -131, -131, -131,  968, -131, -131,  993, -131,  702,  201,
 /*   130 */  -131,  -86, -131, -131, -131,  765,   12, -131,  620, -131,
 /*   140 */  -131,  386, -131, 3889, -131, 4189, -131, -131, -131, -131,
 /*   150 */  -131, -131, -131, -131, -131, -131, -131, -131, -131, -131,
 /*   160 */  -131, -131, -131, -131, -131, -131, -131, -131, -131, -131,
 /*   170 */  -131, -131, -131, -131, 1275, -131, 3389, 4189, -131, -131,
 /*   180 */  3339, 4189, -131, 1373, -131,  395, -131,  926, -131, -131,
 /*   190 */   975, -131, 3789, 4189, -131, 3539, 4189, -131, 1471, -131,
 /*   200 */  4139, 4189, -131, -131, 3239, 4189, -131,  785, -131, -131,
 /*   210 */  2548, -131, 3943, 3843, -131, 3593, 1764, -131, 2940, 2842,
 /*   220 */  -131, 2744, 2646, -131, 2485, 2254, -131, 1666, 1568, -131,
 /*   230 */  1862, -131, 3493, -131, -131, -131,  623, -131, 2681, -131,
 /*   240 */  -131, 4048, -131, 3748, -131, 2975, -131, 2779, -131, 2450,
 /*   250 */  -131, 3648, -131, 3038, -131, 2058, -131, 1960, -131, 1897,
 /*   260 */  -131, -131, -131, 2352, -131, -131, -131, 2289, -131, -131,
 /*   270 */  -131, -131,  588, -131, -131, 2387, -131, -131, 2877, -131,
 /*   280 */  -131, -131, -131, -131, -131, -131, -131, -131, 3793, -131,
 /*   290 */  -131,  231, -131, -131,  -98, -131, -131, -131, 1176, -131,
 /*   300 */  -131, -131,   35, -131, -131, -131, -131, -131, -131, -131,
 /*   310 */  -131, -131,  392, -131, -131, -131, -131, -131, -131,  686,
 /*   320 */  -131, -131, -131, -131, -131, -131, -131,  196, -131, -131,
 /*   330 */  -131, -131, -131, -131, -131, -131, -131, -131, -131, 4043,
 /*   340 */  -131, -131, 3193, -131, -131, -131, -131, -131, -131, -131,
 /*   350 */  -131, -131, -131, -131, -131, -131,  329, -131, -131, -131,
 /*   360 */  -131, -131,  294, -131, -131, -131, -131, -131, -131, -131,
 /*   370 */  -131, -131, -131, -131, -131, 3693, -131, 3589, 4189, -131,
 /*   380 */   558, -131, -131,  971, -131,  -52, -131, -131, -131, -131,
 /*   390 */  -131, -131,  819, -131, -131, -131,   56,  917, -131,  882,
 /*   400 */   896, 1015, -131, -131, -131, -131,  328, 1113, -131, -131,
 /*   410 */   882, -131, -131, -131, -131, -131,  833, 1211, -131, -131,
 /*   420 */   830, 1309, -131, -131,  -42, 1407, -131,  882,  818, 1505,
 /*   430 */  -131,  644, 1603, -131,  882,  798, 1701, -131,  561, 1799,
 /*   440 */  -131, -131, -131, -131,  427, -131, -131,  981, -131, -131,
 /*   450 */  -131, -131, -131, -131, -131, -131, -131, -131, -131, 3998,
 /*   460 */  -131, -131, -131, 3548, -131, -131, 3293, -131, -131,   -4,
 /*   470 */  -131, -131,  712, -131, -131,  708, -131, -131,  262, -131,
 /*   480 */  -131,  543, -131, -131,  428, -131, -131,  330, -131, -131,
 /*   490 */   232, -131, -131,  452, -131, -131,  520, -131, -131,   54,
 /*   500 */  -131, -131,  221, -131, -131, -131, -131, -131, -131, 3898,
 /*   510 */  -131, 3839, 4189, -131, -131, -131, 2156, -131, 3289, 4189,
 /*   520 */  -131, 2583, -131, 3689, 4189, -131, -131, 1079, -131, -131,
 /*   530 */  -131, -131, -131, 4089, 4189, -131, -131, -131, -131, 3439,
 /*   540 */  4189, -131, -131, 2034, -131, 3639, -131, 4189, -131, -131,
 /*   550 */  -131, 3739, -131, 4189, -131, 2124, -131, 3939, -131, 4189,
 /*   560 */  -131,  -96,  620, -131, -131,  288, -131, 3989, -131, 4189,
 /*   570 */  -131, 2222, -131, 4039, -131, 4189, -131, -131, -131, 3189,
 /*   580 */  -131, 4189, -131, 2230, -131, 3489, -131, 4189, -131,  141,
 /*   590 */    12, -131, -131,  -96, -131, -131,  898, -131, -131, -131,
 /*   600 */  -131,  903, -131, -131, -131,  373,  620,  510, -131,  923,
 /*   610 */   929, -131, -131, -131,  470, -131, -131, -131,  960, -131,
 /*   620 */  -131, -131, -131, -131, -131, -131,  620,  983, -131,  988,
 /*   630 */   990, -131, -131, -131, -131, -131, -131,   43,   11,   12,
 /*   640 */   373,   12, -131,   12, 1010, -131, -131, -131,  675, 1032,
 /*   650 */  -131, -131, -131,  -94, -131, -131,  100, -131,   65,  159,
 /*   660 */  -131, -131, -131,  734, -131, -131,  842, -131, -131,
};
static YYACTIONTYPE yy_default[] = {
 /*     0 */   994,  994,  994,  671,  673,  674,  675,  676,  677,  994,
 /*    10 */   994,  678,  994,  994,  679,  994,  994,  680,  994,  695,
 /*    20 */   994,  696,  726,  994,  994,  994,  994,  994,  994,  994,
 /*    30 */   746,  764,  765,  994,  766,  768,  769,  770,  771,  994,
 /*    40 */   994,  782,  994,  994,  783,  784,  785,  786,  787,  788,
 /*    50 */   789,  790,  791,  792,  793,  794,  767,  750,  994,  994,
 /*    60 */   994,  994,  747,  751,  994,  772,  774,  994,  778,  982,
 /*    70 */   983,  984,  985,  986,  987,  988,  994,  994,  989,  990,
 /*    80 */   994,  775,  994,  779,  994,  776,  994,  780,  994,  777,
 /*    90 */   994,  781,  994,  994,  773,  754,  756,  757,  758,  759,
 /*   100 */   760,  761,  762,  763,  994,  994,  994,  994,  994,  994,
 /*   110 */   994,  994,  748,  752,  994,  994,  994,  994,  749,  753,
 /*   120 */   755,  727,  994,  994,  681,  994,  994,  682,  994,  994,
 /*   130 */   684,  994,  690,  994,  691,  994,  994,  724,  994,  994,
 /*   140 */   994,  994,  994,  994,  730,  994,  732,  795,  797,  798,
 /*   150 */   799,  800,  801,  802,  803,  804,  805,  806,  807,  808,
 /*   160 */   809,  810,  811,  812,  813,  814,  815,  816,  817,  818,
 /*   170 */   994,  819,  994,  820,  994,  994,  994,  994,  823,  994,
 /*   180 */   994,  994,  824,  994,  994,  994,  827,  994,  828,  829,
 /*   190 */   994,  994,  831,  832,  994,  994,  994,  835,  994,  994,
 /*   200 */   994,  994,  837,  994,  994,  994,  994,  994,  994,  839,
 /*   210 */   994,  899,  994,  994,  900,  994,  994,  901,  994,  994,
 /*   220 */   902,  994,  994,  903,  994,  994,  904,  994,  994,  905,
 /*   230 */   994,  912,  994,  913,  994,  917,  994,  973,  994,  994,
 /*   240 */   922,  994,  923,  994,  924,  994,  925,  994,  926,  994,
 /*   250 */   927,  994,  928,  994,  929,  994,  930,  994,  931,  994,
 /*   260 */   994,  914,  994,  994,  915,  994,  994,  994,  916,  935,
 /*   270 */   994,  920,  994,  994,  994,  994,  932,  933,  994,  934,
 /*   280 */   936,  937,  938,  939,  940,  941,  942,  943,  994,  981,
 /*   290 */   935,  994,  994,  946,  994,  947,  994,  948,  994,  994,
 /*   300 */   994,  994,  994,  994,  954,  955,  968,  969,  970,  972,
 /*   310 */   994,  994,  994,  994,  958,  959,  994,  994,  994,  994,
 /*   320 */   994,  960,  961,  971,  994,  994,  949,  994,  950,  994,
 /*   330 */   951,  994,  952,  953,  936,  937,  944,  994,  945,  994,
 /*   340 */   974,  994,  994,  976,  977,  975,  994,  956,  957,  921,
 /*   350 */   994,  962,  963,  994,  994,  918,  994,  994,  964,  965,
 /*   360 */   994,  919,  994,  994,  966,  967,  911,  910,  909,  908,
 /*   370 */   907,  906,  991,  994,  994,  994,  994,  994,  994,  840,
 /*   380 */   994,  994,  844,  994,  845,  994,  847,  848,  849,  850,
 /*   390 */   851,  852,  994,  853,  898,  994,  994,  994,  854,  994,
 /*   400 */   994,  994,  857,  994,  866,  994,  994,  994,  858,  864,
 /*   410 */   994,  865,  867,  868,  994,  994,  994,  994,  855,  994,
 /*   420 */   994,  994,  856,  994,  994,  994,  859,  994,  994,  994,
 /*   430 */   860,  994,  994,  861,  994,  994,  994,  862,  994,  994,
 /*   440 */   863,  869,  870,  846,  994,  994,  872,  994,  873,  875,
 /*   450 */   874,  968,  876,  970,  877,  969,  878,  933,  879,  994,
 /*   460 */   994,  880,  881,  994,  994,  882,  994,  994,  883,  994,
 /*   470 */   994,  884,  994,  894,  896,  994,  897,  895,  994,  994,
 /*   480 */   885,  994,  994,  886,  994,  994,  887,  994,  994,  888,
 /*   490 */   994,  994,  889,  994,  994,  890,  994,  994,  891,  994,
 /*   500 */   994,  892,  994,  994,  893,  994,  992,  993,  796,  994,
 /*   510 */   994,  994,  994,  841,  994,  994,  994,  994,  994,  994,
 /*   520 */   842,  994,  994,  994,  994,  843,  994,  994,  994,  838,
 /*   530 */   836,  834,  994,  994,  833,  830,  825,  821,  994,  994,
 /*   540 */   994,  826,  822,  994,  994,  994,  738,  994,  740,  994,
 /*   550 */   994,  994,  731,  994,  733,  994,  994,  994,  739,  994,
 /*   560 */   741,  994,  994,  994,  994,  994,  994,  994,  734,  994,
 /*   570 */   736,  994,  994,  994,  742,  994,  744,  994,  994,  994,
 /*   580 */   735,  994,  737,  994,  994,  994,  743,  994,  745,  994,
 /*   590 */   994,  722,  725,  994,  994,  994,  994,  994,  728,  994,
 /*   600 */   994,  994,  994,  729,  706,  994,  994,  994,  708,  994,
 /*   610 */   994,  710,  994,  714,  994,  994,  994,  994,  994,  718,
 /*   620 */   720,  994,  721,  719,  994,  712,  994,  994,  709,  994,
 /*   630 */   994,  711,  994,  715,  994,  713,  723,  994,  994,  994,
 /*   640 */   994,  994,  707,  994,  994,  692,  694,  693,  994,  994,
 /*   650 */   683,  994,  994,  994,  685,  994,  994,  686,  994,  994,
 /*   660 */   687,  994,  994,  994,  688,  994,  994,  689,  672,
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
  "xx_let_assignment",  "xx_assignment_operator",  "xx_assign_expr",  "xx_array_offset_list",
  "xx_array_offset",  "xx_index_expr",  "xx_echo_expressions",  "xx_echo_expression",
  "xx_mcall_expr",  "xx_fcall_expr",  "xx_scall_expr",  "xx_fetch_expr",
  "xx_declare_variable_list",  "xx_declare_variable",  "xx_array_list",  "xx_call_parameters",
  "xx_call_parameter",  "xx_array_item",  "xx_array_key",  "xx_array_value",
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
 /* 189 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER xx_array_offset_list xx_assignment_operator xx_assign_expr",
 /* 190 */ "xx_let_assignment ::= IDENTIFIER DOUBLECOLON IDENTIFIER xx_assignment_operator xx_assign_expr",
 /* 191 */ "xx_let_assignment ::= IDENTIFIER DOUBLECOLON IDENTIFIER SBRACKET_OPEN SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 192 */ "xx_let_assignment ::= IDENTIFIER DOUBLECOLON IDENTIFIER xx_array_offset_list xx_assignment_operator xx_assign_expr",
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
 /* 213 */ "xx_unset_statement ::= UNSET xx_common_expr DOTCOMMA",
 /* 214 */ "xx_throw_statement ::= THROW xx_common_expr DOTCOMMA",
 /* 215 */ "xx_declare_statement ::= TYPE_INTEGER xx_declare_variable_list DOTCOMMA",
 /* 216 */ "xx_declare_statement ::= TYPE_UINTEGER xx_declare_variable_list DOTCOMMA",
 /* 217 */ "xx_declare_statement ::= TYPE_CHAR xx_declare_variable_list DOTCOMMA",
 /* 218 */ "xx_declare_statement ::= TYPE_UCHAR xx_declare_variable_list DOTCOMMA",
 /* 219 */ "xx_declare_statement ::= TYPE_LONG xx_declare_variable_list DOTCOMMA",
 /* 220 */ "xx_declare_statement ::= TYPE_ULONG xx_declare_variable_list DOTCOMMA",
 /* 221 */ "xx_declare_statement ::= TYPE_DOUBLE xx_declare_variable_list DOTCOMMA",
 /* 222 */ "xx_declare_statement ::= TYPE_STRING xx_declare_variable_list DOTCOMMA",
 /* 223 */ "xx_declare_statement ::= TYPE_BOOL xx_declare_variable_list DOTCOMMA",
 /* 224 */ "xx_declare_statement ::= TYPE_VAR xx_declare_variable_list DOTCOMMA",
 /* 225 */ "xx_declare_variable_list ::= xx_declare_variable_list COMMA xx_declare_variable",
 /* 226 */ "xx_declare_variable_list ::= xx_declare_variable",
 /* 227 */ "xx_declare_variable ::= IDENTIFIER",
 /* 228 */ "xx_declare_variable ::= IDENTIFIER ASSIGN xx_literal_expr",
 /* 229 */ "xx_assign_expr ::= xx_common_expr",
 /* 230 */ "xx_common_expr ::= NOT xx_common_expr",
 /* 231 */ "xx_common_expr ::= ISSET xx_common_expr",
 /* 232 */ "xx_common_expr ::= REQUIRE xx_common_expr",
 /* 233 */ "xx_common_expr ::= CLONE xx_common_expr",
 /* 234 */ "xx_common_expr ::= EMPTY xx_common_expr",
 /* 235 */ "xx_common_expr ::= LIKELY xx_common_expr",
 /* 236 */ "xx_common_expr ::= UNLIKELY xx_common_expr",
 /* 237 */ "xx_common_expr ::= xx_common_expr EQUALS xx_common_expr",
 /* 238 */ "xx_common_expr ::= xx_common_expr NOTEQUALS xx_common_expr",
 /* 239 */ "xx_common_expr ::= xx_common_expr IDENTICAL xx_common_expr",
 /* 240 */ "xx_common_expr ::= xx_common_expr NOTIDENTICAL xx_common_expr",
 /* 241 */ "xx_common_expr ::= xx_common_expr LESS xx_common_expr",
 /* 242 */ "xx_common_expr ::= xx_common_expr GREATER xx_common_expr",
 /* 243 */ "xx_common_expr ::= xx_common_expr LESSEQUAL xx_common_expr",
 /* 244 */ "xx_common_expr ::= xx_common_expr GREATEREQUAL xx_common_expr",
 /* 245 */ "xx_common_expr ::= PARENTHESES_OPEN xx_common_expr PARENTHESES_CLOSE",
 /* 246 */ "xx_common_expr ::= PARENTHESES_OPEN xx_parameter_type PARENTHESES_CLOSE xx_common_expr",
 /* 247 */ "xx_common_expr ::= LESS IDENTIFIER GREATER xx_common_expr",
 /* 248 */ "xx_common_expr ::= xx_common_expr ARROW IDENTIFIER",
 /* 249 */ "xx_common_expr ::= xx_common_expr ARROW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE",
 /* 250 */ "xx_common_expr ::= xx_common_expr ARROW BRACKET_OPEN STRING BRACKET_CLOSE",
 /* 251 */ "xx_common_expr ::= IDENTIFIER DOUBLECOLON IDENTIFIER",
 /* 252 */ "xx_common_expr ::= IDENTIFIER DOUBLECOLON CONSTANT",
 /* 253 */ "xx_common_expr ::= xx_common_expr SBRACKET_OPEN xx_common_expr SBRACKET_CLOSE",
 /* 254 */ "xx_common_expr ::= xx_common_expr ADD xx_common_expr",
 /* 255 */ "xx_common_expr ::= xx_common_expr SUB xx_common_expr",
 /* 256 */ "xx_common_expr ::= xx_common_expr MUL xx_common_expr",
 /* 257 */ "xx_common_expr ::= xx_common_expr DIV xx_common_expr",
 /* 258 */ "xx_common_expr ::= xx_common_expr MOD xx_common_expr",
 /* 259 */ "xx_common_expr ::= xx_common_expr CONCAT xx_common_expr",
 /* 260 */ "xx_common_expr ::= xx_common_expr AND xx_common_expr",
 /* 261 */ "xx_common_expr ::= xx_common_expr OR xx_common_expr",
 /* 262 */ "xx_common_expr ::= xx_common_expr INSTANCEOF xx_common_expr",
 /* 263 */ "xx_fetch_expr ::= FETCH IDENTIFIER COMMA xx_common_expr",
 /* 264 */ "xx_common_expr ::= xx_fetch_expr",
 /* 265 */ "xx_common_expr ::= TYPEOF xx_common_expr",
 /* 266 */ "xx_common_expr ::= IDENTIFIER",
 /* 267 */ "xx_common_expr ::= INTEGER",
 /* 268 */ "xx_common_expr ::= STRING",
 /* 269 */ "xx_common_expr ::= CHAR",
 /* 270 */ "xx_common_expr ::= DOUBLE",
 /* 271 */ "xx_common_expr ::= NULL",
 /* 272 */ "xx_common_expr ::= TRUE",
 /* 273 */ "xx_common_expr ::= FALSE",
 /* 274 */ "xx_common_expr ::= CONSTANT",
 /* 275 */ "xx_common_expr ::= SBRACKET_OPEN SBRACKET_CLOSE",
 /* 276 */ "xx_common_expr ::= SBRACKET_OPEN xx_array_list SBRACKET_CLOSE",
 /* 277 */ "xx_common_expr ::= NEW IDENTIFIER",
 /* 278 */ "xx_common_expr ::= NEW IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 279 */ "xx_common_expr ::= NEW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 280 */ "xx_common_expr ::= NEW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE",
 /* 281 */ "xx_common_expr ::= NEW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 282 */ "xx_common_expr ::= NEW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 283 */ "xx_fcall_expr ::= IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 284 */ "xx_fcall_expr ::= IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 285 */ "xx_fcall_expr ::= BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 286 */ "xx_fcall_expr ::= BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 287 */ "xx_scall_expr ::= IDENTIFIER DOUBLECOLON IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 288 */ "xx_scall_expr ::= IDENTIFIER DOUBLECOLON IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 289 */ "xx_scall_expr ::= BRACKET_OPEN IDENTIFIER BRACKET_CLOSE DOUBLECOLON IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 290 */ "xx_scall_expr ::= BRACKET_OPEN IDENTIFIER BRACKET_CLOSE DOUBLECOLON IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 291 */ "xx_scall_expr ::= BRACKET_OPEN IDENTIFIER BRACKET_CLOSE DOUBLECOLON BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 292 */ "xx_scall_expr ::= BRACKET_OPEN IDENTIFIER BRACKET_CLOSE DOUBLECOLON BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 293 */ "xx_mcall_expr ::= xx_common_expr ARROW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 294 */ "xx_mcall_expr ::= xx_common_expr ARROW IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 295 */ "xx_mcall_expr ::= xx_common_expr ARROW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 296 */ "xx_mcall_expr ::= xx_common_expr ARROW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 297 */ "xx_mcall_expr ::= xx_common_expr ARROW BRACKET_OPEN STRING BRACKET_CLOSE PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 298 */ "xx_mcall_expr ::= xx_common_expr ARROW BRACKET_OPEN STRING BRACKET_CLOSE PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 299 */ "xx_common_expr ::= xx_mcall_expr",
 /* 300 */ "xx_common_expr ::= xx_scall_expr",
 /* 301 */ "xx_common_expr ::= xx_fcall_expr",
 /* 302 */ "xx_call_parameters ::= xx_call_parameters COMMA xx_call_parameter",
 /* 303 */ "xx_call_parameters ::= xx_call_parameter",
 /* 304 */ "xx_call_parameter ::= xx_common_expr",
 /* 305 */ "xx_array_list ::= xx_array_list COMMA xx_array_item",
 /* 306 */ "xx_array_list ::= xx_array_item",
 /* 307 */ "xx_array_item ::= xx_array_key COLON xx_array_value",
 /* 308 */ "xx_array_item ::= xx_array_value",
 /* 309 */ "xx_array_key ::= IDENTIFIER",
 /* 310 */ "xx_array_key ::= STRING",
 /* 311 */ "xx_array_key ::= INTEGER",
 /* 312 */ "xx_array_value ::= xx_common_expr",
 /* 313 */ "xx_literal_expr ::= INTEGER",
 /* 314 */ "xx_literal_expr ::= CHAR",
 /* 315 */ "xx_literal_expr ::= STRING",
 /* 316 */ "xx_literal_expr ::= DOUBLE",
 /* 317 */ "xx_literal_expr ::= NULL",
 /* 318 */ "xx_literal_expr ::= FALSE",
 /* 319 */ "xx_literal_expr ::= TRUE",
 /* 320 */ "xx_literal_expr ::= IDENTIFIER DOUBLECOLON CONSTANT",
 /* 321 */ "xx_literal_expr ::= CONSTANT",
 /* 322 */ "xx_eval_expr ::= xx_common_expr",
 /* 323 */ "xx_comment ::= COMMENT",
 /* 324 */ "xx_cblock ::= CBLOCK",
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
// 1014 "parser.lemon"
{
	/*if ((yypminor->yy0)) {
		if ((yypminor->yy0)->free_flag) {
			efree((yypminor->yy0)->token);
		}
		efree((yypminor->yy0));
	}*/
}
// 3160 "parser.c"
      break;
    case 107:
// 1027 "parser.lemon"
{ json_object_put((yypminor->yy119)); }
// 3165 "parser.c"
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
  { 168, 6 },
  { 168, 5 },
  { 168, 7 },
  { 168, 6 },
  { 168, 5 },
  { 168, 4 },
  { 171, 2 },
  { 171, 1 },
  { 172, 3 },
  { 168, 4 },
  { 168, 4 },
  { 168, 2 },
  { 168, 2 },
  { 173, 1 },
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
  { 153, 3 },
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
  { 183, 3 },
  { 183, 1 },
  { 184, 1 },
  { 182, 3 },
  { 182, 1 },
  { 185, 3 },
  { 185, 1 },
  { 186, 1 },
  { 186, 1 },
  { 186, 1 },
  { 187, 1 },
  { 126, 1 },
  { 126, 1 },
  { 126, 1 },
  { 126, 1 },
  { 126, 1 },
  { 126, 1 },
  { 126, 1 },
  { 126, 3 },
  { 126, 1 },
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
// 1023 "parser.lemon"
{
	status->ret = yymsp[0].minor.yy119;
}
// 3707 "parser.c"
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
      case 229:
      case 264:
      case 299:
      case 300:
      case 301:
      case 304:
      case 312:
      case 322:
// 1029 "parser.lemon"
{
	yygotominor.yy119 = yymsp[0].minor.yy119;
}
// 3749 "parser.c"
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
// 1033 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_list(yymsp[-1].minor.yy119, yymsp[0].minor.yy119);
}
// 3764 "parser.c"
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
      case 226:
      case 303:
      case 306:
// 1037 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_list(NULL, yymsp[0].minor.yy119);
}
// 3788 "parser.c"
        break;
      case 9:
// 1062 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_namespace(yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(39,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3797 "parser.c"
        break;
      case 10:
// 1066 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_interface(yymsp[-1].minor.yy0, yymsp[0].minor.yy119, NULL, status->scanner_state);
  yy_destructor(42,&yymsp[-2].minor);
}
// 3805 "parser.c"
        break;
      case 11:
// 1070 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_interface(yymsp[-3].minor.yy0, yymsp[0].minor.yy119, yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(42,&yymsp[-4].minor);
  yy_destructor(43,&yymsp[-2].minor);
}
// 3814 "parser.c"
        break;
      case 12:
// 1074 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_class(yymsp[-1].minor.yy0, yymsp[0].minor.yy119, 0, 0, NULL, NULL, status->scanner_state);
  yy_destructor(44,&yymsp[-2].minor);
}
// 3822 "parser.c"
        break;
      case 13:
// 1078 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy119, 0, 0, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(44,&yymsp[-4].minor);
  yy_destructor(43,&yymsp[-2].minor);
}
// 3831 "parser.c"
        break;
      case 14:
// 1082 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy119, 0, 0, NULL, yymsp[-1].minor.yy119, status->scanner_state);
  yy_destructor(44,&yymsp[-4].minor);
  yy_destructor(45,&yymsp[-2].minor);
}
// 3840 "parser.c"
        break;
      case 15:
// 1086 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_class(yymsp[-5].minor.yy0, yymsp[0].minor.yy119, 0, 0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy119, status->scanner_state);
  yy_destructor(44,&yymsp[-6].minor);
  yy_destructor(43,&yymsp[-4].minor);
  yy_destructor(45,&yymsp[-2].minor);
}
// 3850 "parser.c"
        break;
      case 16:
// 1090 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_class(yymsp[-1].minor.yy0, yymsp[0].minor.yy119, 1, 0, NULL, NULL, status->scanner_state);
  yy_destructor(46,&yymsp[-3].minor);
  yy_destructor(44,&yymsp[-2].minor);
}
// 3859 "parser.c"
        break;
      case 17:
// 1094 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy119, 1, 0, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(46,&yymsp[-5].minor);
  yy_destructor(44,&yymsp[-4].minor);
  yy_destructor(43,&yymsp[-2].minor);
}
// 3869 "parser.c"
        break;
      case 18:
// 1098 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy119, 1, 0, NULL, yymsp[-1].minor.yy119, status->scanner_state);
  yy_destructor(46,&yymsp[-5].minor);
  yy_destructor(44,&yymsp[-4].minor);
  yy_destructor(45,&yymsp[-2].minor);
}
// 3879 "parser.c"
        break;
      case 19:
// 1102 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_class(yymsp[-1].minor.yy0, yymsp[0].minor.yy119, 0, 1, NULL, NULL, status->scanner_state);
  yy_destructor(47,&yymsp[-3].minor);
  yy_destructor(44,&yymsp[-2].minor);
}
// 3888 "parser.c"
        break;
      case 20:
// 1106 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy119, 0, 1, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(47,&yymsp[-5].minor);
  yy_destructor(44,&yymsp[-4].minor);
  yy_destructor(43,&yymsp[-2].minor);
}
// 3898 "parser.c"
        break;
      case 21:
      case 26:
      case 47:
// 1110 "parser.lemon"
{
	yygotominor.yy119 = NULL;
  yy_destructor(48,&yymsp[-1].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 3909 "parser.c"
        break;
      case 22:
      case 27:
      case 48:
// 1114 "parser.lemon"
{
	yygotominor.yy119 = yymsp[-1].minor.yy119;
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 3920 "parser.c"
        break;
      case 23:
      case 49:
      case 103:
      case 176:
      case 204:
      case 225:
      case 302:
      case 305:
// 1118 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_list(yymsp[-2].minor.yy119, yymsp[0].minor.yy119);
  yy_destructor(6,&yymsp[-1].minor);
}
// 3935 "parser.c"
        break;
      case 25:
      case 266:
      case 309:
// 1126 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state);
}
// 3944 "parser.c"
        break;
      case 28:
// 1138 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_class_definition(yymsp[0].minor.yy119, NULL, NULL, status->scanner_state);
}
// 3951 "parser.c"
        break;
      case 29:
// 1142 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_class_definition(NULL, NULL, yymsp[0].minor.yy119, status->scanner_state);
}
// 3958 "parser.c"
        break;
      case 30:
// 1146 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_class_definition(NULL, yymsp[0].minor.yy119, NULL, status->scanner_state);
}
// 3965 "parser.c"
        break;
      case 31:
// 1150 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_class_definition(yymsp[-1].minor.yy119, yymsp[0].minor.yy119, NULL, status->scanner_state);
}
// 3972 "parser.c"
        break;
      case 32:
// 1154 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_class_definition(yymsp[-1].minor.yy119, NULL, yymsp[0].minor.yy119, status->scanner_state);
}
// 3979 "parser.c"
        break;
      case 33:
// 1158 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_class_definition(yymsp[0].minor.yy119, NULL, yymsp[-1].minor.yy119, status->scanner_state);
}
// 3986 "parser.c"
        break;
      case 34:
// 1162 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_class_definition(NULL, yymsp[0].minor.yy119, yymsp[-1].minor.yy119, status->scanner_state);
}
// 3993 "parser.c"
        break;
      case 35:
// 1166 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_class_definition(yymsp[-2].minor.yy119, yymsp[0].minor.yy119, yymsp[-1].minor.yy119, status->scanner_state);
}
// 4000 "parser.c"
        break;
      case 36:
// 1170 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_class_definition(yymsp[-1].minor.yy119, yymsp[0].minor.yy119, yymsp[-2].minor.yy119, status->scanner_state);
}
// 4007 "parser.c"
        break;
      case 39:
// 1187 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_class_property(yymsp[-2].minor.yy119, yymsp[-1].minor.yy0, NULL, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(41,&yymsp[0].minor);
}
// 4015 "parser.c"
        break;
      case 40:
// 1191 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_class_property(yymsp[-2].minor.yy119, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(41,&yymsp[0].minor);
}
// 4023 "parser.c"
        break;
      case 41:
// 1195 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_class_property(yymsp[-4].minor.yy119, yymsp[-3].minor.yy0, yymsp[-1].minor.yy119, yymsp[-5].minor.yy0, NULL, status->scanner_state);
  yy_destructor(51,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 4032 "parser.c"
        break;
      case 42:
// 1199 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_class_property(yymsp[-4].minor.yy119, yymsp[-3].minor.yy0, yymsp[-1].minor.yy119, NULL, NULL, status->scanner_state);
  yy_destructor(51,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 4041 "parser.c"
        break;
      case 43:
// 1203 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_class_property(yymsp[-3].minor.yy119, yymsp[-2].minor.yy0, NULL, yymsp[-4].minor.yy0, yymsp[-1].minor.yy119, status->scanner_state);
  yy_destructor(41,&yymsp[0].minor);
}
// 4049 "parser.c"
        break;
      case 44:
// 1207 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_class_property(yymsp[-3].minor.yy119, yymsp[-2].minor.yy0, NULL, NULL, yymsp[-1].minor.yy119, status->scanner_state);
  yy_destructor(41,&yymsp[0].minor);
}
// 4057 "parser.c"
        break;
      case 45:
// 1211 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_class_property(yymsp[-5].minor.yy119, yymsp[-4].minor.yy0, yymsp[-2].minor.yy119, yymsp[-6].minor.yy0, yymsp[-1].minor.yy119, status->scanner_state);
  yy_destructor(51,&yymsp[-3].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 4066 "parser.c"
        break;
      case 46:
// 1215 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_class_property(yymsp[-5].minor.yy119, yymsp[-4].minor.yy0, yymsp[-2].minor.yy119, NULL, yymsp[-1].minor.yy119, status->scanner_state);
  yy_destructor(51,&yymsp[-3].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 4075 "parser.c"
        break;
      case 51:
// 1235 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_property_shortcut(NULL, yymsp[0].minor.yy0, status->scanner_state);
}
// 4082 "parser.c"
        break;
      case 52:
// 1239 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_property_shortcut(yymsp[-1].minor.yy0, yymsp[0].minor.yy0, status->scanner_state);
}
// 4089 "parser.c"
        break;
      case 59:
// 1268 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy119, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(52,&yymsp[-4].minor);
  yy_destructor(51,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 4099 "parser.c"
        break;
      case 60:
// 1272 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy119, NULL, status->scanner_state);
  yy_destructor(52,&yymsp[-4].minor);
  yy_destructor(51,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 4109 "parser.c"
        break;
      case 61:
// 1277 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_class_method(yymsp[-6].minor.yy119, yymsp[-4].minor.yy0, NULL, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(54,&yymsp[-5].minor);
  yy_destructor(55,&yymsp[-3].minor);
  yy_destructor(36,&yymsp[-2].minor);
  yy_destructor(48,&yymsp[-1].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4121 "parser.c"
        break;
      case 62:
// 1281 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_class_method(yymsp[-7].minor.yy119, yymsp[-5].minor.yy0, yymsp[-3].minor.yy119, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(54,&yymsp[-6].minor);
  yy_destructor(55,&yymsp[-4].minor);
  yy_destructor(36,&yymsp[-2].minor);
  yy_destructor(48,&yymsp[-1].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4133 "parser.c"
        break;
      case 63:
// 1285 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_class_method(yymsp[-7].minor.yy119, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy119, NULL, NULL, status->scanner_state);
  yy_destructor(54,&yymsp[-6].minor);
  yy_destructor(55,&yymsp[-4].minor);
  yy_destructor(36,&yymsp[-3].minor);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4145 "parser.c"
        break;
      case 64:
// 1289 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_class_method(yymsp[-8].minor.yy119, yymsp[-6].minor.yy0, yymsp[-4].minor.yy119, yymsp[-1].minor.yy119, NULL, NULL, status->scanner_state);
  yy_destructor(54,&yymsp[-7].minor);
  yy_destructor(55,&yymsp[-5].minor);
  yy_destructor(36,&yymsp[-3].minor);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4157 "parser.c"
        break;
      case 65:
// 1293 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_class_method(yymsp[-6].minor.yy119, yymsp[-4].minor.yy0, NULL, NULL, yymsp[-7].minor.yy0, NULL, status->scanner_state);
  yy_destructor(54,&yymsp[-5].minor);
  yy_destructor(55,&yymsp[-3].minor);
  yy_destructor(36,&yymsp[-2].minor);
  yy_destructor(48,&yymsp[-1].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4169 "parser.c"
        break;
      case 66:
// 1297 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_class_method(yymsp[-7].minor.yy119, yymsp[-5].minor.yy0, yymsp[-3].minor.yy119, NULL, yymsp[-8].minor.yy0, NULL, status->scanner_state);
  yy_destructor(54,&yymsp[-6].minor);
  yy_destructor(55,&yymsp[-4].minor);
  yy_destructor(36,&yymsp[-2].minor);
  yy_destructor(48,&yymsp[-1].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4181 "parser.c"
        break;
      case 67:
// 1301 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_class_method(yymsp[-7].minor.yy119, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy119, yymsp[-8].minor.yy0, NULL, status->scanner_state);
  yy_destructor(54,&yymsp[-6].minor);
  yy_destructor(55,&yymsp[-4].minor);
  yy_destructor(36,&yymsp[-3].minor);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4193 "parser.c"
        break;
      case 68:
// 1305 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_class_method(yymsp[-8].minor.yy119, yymsp[-6].minor.yy0, yymsp[-4].minor.yy119, yymsp[-1].minor.yy119, yymsp[-9].minor.yy0, NULL, status->scanner_state);
  yy_destructor(54,&yymsp[-7].minor);
  yy_destructor(55,&yymsp[-5].minor);
  yy_destructor(36,&yymsp[-3].minor);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4205 "parser.c"
        break;
      case 69:
// 1309 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_class_method(yymsp[-8].minor.yy119, yymsp[-6].minor.yy0, NULL, NULL, NULL, yymsp[-2].minor.yy119, status->scanner_state);
  yy_destructor(54,&yymsp[-7].minor);
  yy_destructor(55,&yymsp[-5].minor);
  yy_destructor(36,&yymsp[-4].minor);
  yy_destructor(38,&yymsp[-3].minor);
  yy_destructor(48,&yymsp[-1].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4218 "parser.c"
        break;
      case 70:
// 1313 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_class_method(yymsp[-9].minor.yy119, yymsp[-7].minor.yy0, yymsp[-5].minor.yy119, NULL, NULL, yymsp[-2].minor.yy119, status->scanner_state);
  yy_destructor(54,&yymsp[-8].minor);
  yy_destructor(55,&yymsp[-6].minor);
  yy_destructor(36,&yymsp[-4].minor);
  yy_destructor(38,&yymsp[-3].minor);
  yy_destructor(48,&yymsp[-1].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4231 "parser.c"
        break;
      case 71:
// 1317 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_class_method(yymsp[-9].minor.yy119, yymsp[-7].minor.yy0, NULL, yymsp[-1].minor.yy119, NULL, yymsp[-3].minor.yy119, status->scanner_state);
  yy_destructor(54,&yymsp[-8].minor);
  yy_destructor(55,&yymsp[-6].minor);
  yy_destructor(36,&yymsp[-5].minor);
  yy_destructor(38,&yymsp[-4].minor);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4244 "parser.c"
        break;
      case 72:
// 1321 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_class_method(yymsp[-10].minor.yy119, yymsp[-8].minor.yy0, yymsp[-6].minor.yy119, yymsp[-1].minor.yy119, NULL, yymsp[-3].minor.yy119, status->scanner_state);
  yy_destructor(54,&yymsp[-9].minor);
  yy_destructor(55,&yymsp[-7].minor);
  yy_destructor(36,&yymsp[-5].minor);
  yy_destructor(38,&yymsp[-4].minor);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4257 "parser.c"
        break;
      case 73:
// 1325 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_class_method(yymsp[-8].minor.yy119, yymsp[-6].minor.yy0, NULL, NULL, yymsp[-9].minor.yy0, yymsp[-2].minor.yy119, status->scanner_state);
  yy_destructor(54,&yymsp[-7].minor);
  yy_destructor(55,&yymsp[-5].minor);
  yy_destructor(36,&yymsp[-4].minor);
  yy_destructor(38,&yymsp[-3].minor);
  yy_destructor(48,&yymsp[-1].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4270 "parser.c"
        break;
      case 74:
// 1329 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_class_method(yymsp[-9].minor.yy119, yymsp[-7].minor.yy0, yymsp[-5].minor.yy119, NULL, yymsp[-10].minor.yy0, yymsp[-2].minor.yy119, status->scanner_state);
  yy_destructor(54,&yymsp[-8].minor);
  yy_destructor(55,&yymsp[-6].minor);
  yy_destructor(36,&yymsp[-4].minor);
  yy_destructor(38,&yymsp[-3].minor);
  yy_destructor(48,&yymsp[-1].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4283 "parser.c"
        break;
      case 75:
// 1333 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_class_method(yymsp[-9].minor.yy119, yymsp[-7].minor.yy0, NULL, yymsp[-1].minor.yy119, yymsp[-10].minor.yy0, yymsp[-3].minor.yy119, status->scanner_state);
  yy_destructor(54,&yymsp[-8].minor);
  yy_destructor(55,&yymsp[-6].minor);
  yy_destructor(36,&yymsp[-5].minor);
  yy_destructor(38,&yymsp[-4].minor);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4296 "parser.c"
        break;
      case 76:
// 1337 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_class_method(yymsp[-10].minor.yy119, yymsp[-8].minor.yy0, yymsp[-6].minor.yy119, yymsp[-1].minor.yy119, yymsp[-11].minor.yy0, yymsp[-3].minor.yy119, status->scanner_state);
  yy_destructor(54,&yymsp[-9].minor);
  yy_destructor(55,&yymsp[-7].minor);
  yy_destructor(36,&yymsp[-5].minor);
  yy_destructor(38,&yymsp[-4].minor);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4309 "parser.c"
        break;
      case 77:
// 1342 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_class_method(yymsp[-7].minor.yy119, yymsp[-5].minor.yy0, NULL, NULL, NULL, yymsp[-1].minor.yy119, status->scanner_state);
  yy_destructor(54,&yymsp[-6].minor);
  yy_destructor(55,&yymsp[-4].minor);
  yy_destructor(36,&yymsp[-3].minor);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 4321 "parser.c"
        break;
      case 78:
// 1346 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_class_method(yymsp[-8].minor.yy119, yymsp[-6].minor.yy0, yymsp[-4].minor.yy119, NULL, NULL, yymsp[-1].minor.yy119, status->scanner_state);
  yy_destructor(54,&yymsp[-7].minor);
  yy_destructor(55,&yymsp[-5].minor);
  yy_destructor(36,&yymsp[-3].minor);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 4333 "parser.c"
        break;
      case 79:
// 1350 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_class_method(yymsp[-7].minor.yy119, yymsp[-5].minor.yy0, NULL, NULL, yymsp[-8].minor.yy0, yymsp[-1].minor.yy119, status->scanner_state);
  yy_destructor(54,&yymsp[-6].minor);
  yy_destructor(55,&yymsp[-4].minor);
  yy_destructor(36,&yymsp[-3].minor);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 4345 "parser.c"
        break;
      case 80:
// 1354 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_class_method(yymsp[-8].minor.yy119, yymsp[-6].minor.yy0, yymsp[-4].minor.yy119, NULL, yymsp[-9].minor.yy0, yymsp[-1].minor.yy119, status->scanner_state);
  yy_destructor(54,&yymsp[-7].minor);
  yy_destructor(55,&yymsp[-5].minor);
  yy_destructor(36,&yymsp[-3].minor);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 4357 "parser.c"
        break;
      case 81:
// 1358 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_class_method(yymsp[-5].minor.yy119, yymsp[-3].minor.yy0, NULL, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(54,&yymsp[-4].minor);
  yy_destructor(55,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[-1].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 4368 "parser.c"
        break;
      case 82:
// 1362 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_class_method(yymsp[-6].minor.yy119, yymsp[-4].minor.yy0, yymsp[-2].minor.yy119, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(54,&yymsp[-5].minor);
  yy_destructor(55,&yymsp[-3].minor);
  yy_destructor(36,&yymsp[-1].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 4379 "parser.c"
        break;
      case 83:
// 1366 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_class_method(yymsp[-5].minor.yy119, yymsp[-3].minor.yy0, NULL, NULL, yymsp[-6].minor.yy0, NULL, status->scanner_state);
  yy_destructor(54,&yymsp[-4].minor);
  yy_destructor(55,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[-1].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 4390 "parser.c"
        break;
      case 84:
// 1370 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_class_method(yymsp[-6].minor.yy119, yymsp[-4].minor.yy0, yymsp[-2].minor.yy119, NULL, yymsp[-7].minor.yy0, NULL, status->scanner_state);
  yy_destructor(54,&yymsp[-5].minor);
  yy_destructor(55,&yymsp[-3].minor);
  yy_destructor(36,&yymsp[-1].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 4401 "parser.c"
        break;
      case 87:
// 1383 "parser.lemon"
{
	yygotominor.yy119 = json_object_new_string("public");
  yy_destructor(1,&yymsp[0].minor);
}
// 4409 "parser.c"
        break;
      case 88:
// 1387 "parser.lemon"
{
	yygotominor.yy119 = json_object_new_string("protected");
  yy_destructor(2,&yymsp[0].minor);
}
// 4417 "parser.c"
        break;
      case 89:
// 1391 "parser.lemon"
{
	yygotominor.yy119 = json_object_new_string("private");
  yy_destructor(4,&yymsp[0].minor);
}
// 4425 "parser.c"
        break;
      case 90:
// 1395 "parser.lemon"
{
	yygotominor.yy119 = json_object_new_string("static");
  yy_destructor(3,&yymsp[0].minor);
}
// 4433 "parser.c"
        break;
      case 91:
// 1399 "parser.lemon"
{
	yygotominor.yy119 = json_object_new_string("scoped");
  yy_destructor(5,&yymsp[0].minor);
}
// 4441 "parser.c"
        break;
      case 92:
// 1403 "parser.lemon"
{
	yygotominor.yy119 = json_object_new_string("inline");
  yy_destructor(56,&yymsp[0].minor);
}
// 4449 "parser.c"
        break;
      case 93:
// 1407 "parser.lemon"
{
	yygotominor.yy119 = json_object_new_string("abstract");
  yy_destructor(46,&yymsp[0].minor);
}
// 4457 "parser.c"
        break;
      case 94:
// 1411 "parser.lemon"
{
	yygotominor.yy119 = json_object_new_string("final");
  yy_destructor(47,&yymsp[0].minor);
}
// 4465 "parser.c"
        break;
      case 95:
// 1416 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_return_type(1, NULL, status->scanner_state);
  yy_destructor(57,&yymsp[0].minor);
}
// 4473 "parser.c"
        break;
      case 96:
// 1420 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_return_type(0, yymsp[0].minor.yy119, status->scanner_state);
}
// 4480 "parser.c"
        break;
      case 97:
// 1424 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_list(yymsp[-2].minor.yy119, yymsp[0].minor.yy119);
  yy_destructor(15,&yymsp[-1].minor);
}
// 4488 "parser.c"
        break;
      case 99:
// 1432 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_return_type_item(yymsp[0].minor.yy119, NULL, 0, 0, status->scanner_state);
}
// 4495 "parser.c"
        break;
      case 100:
// 1436 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_return_type_item(yymsp[-1].minor.yy119, NULL, 1, 0, status->scanner_state);
  yy_destructor(35,&yymsp[0].minor);
}
// 4503 "parser.c"
        break;
      case 101:
// 1440 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_return_type_item(NULL, yymsp[0].minor.yy119, 0, 0, status->scanner_state);
}
// 4510 "parser.c"
        break;
      case 102:
// 1444 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_return_type_item(NULL, yymsp[0].minor.yy119, 0, 1, status->scanner_state);
}
// 4517 "parser.c"
        break;
      case 105:
// 1457 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_parameter(NULL, NULL, yymsp[0].minor.yy0, NULL, 0, status->scanner_state);
}
// 4524 "parser.c"
        break;
      case 106:
// 1461 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_parameter(yymsp[-1].minor.yy119, NULL, yymsp[0].minor.yy0, NULL, 0, status->scanner_state);
}
// 4531 "parser.c"
        break;
      case 107:
// 1465 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_parameter(yymsp[-2].minor.yy119, NULL, yymsp[0].minor.yy0, NULL, 1, status->scanner_state);
  yy_destructor(35,&yymsp[-1].minor);
}
// 4539 "parser.c"
        break;
      case 108:
// 1469 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_parameter(NULL, yymsp[-1].minor.yy119, yymsp[0].minor.yy0, NULL, 0, status->scanner_state);
}
// 4546 "parser.c"
        break;
      case 109:
// 1473 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_parameter(NULL, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy119, 0, status->scanner_state);
  yy_destructor(51,&yymsp[-1].minor);
}
// 4554 "parser.c"
        break;
      case 110:
// 1477 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_parameter(yymsp[-3].minor.yy119, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy119, 0, status->scanner_state);
  yy_destructor(51,&yymsp[-1].minor);
}
// 4562 "parser.c"
        break;
      case 111:
// 1481 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_parameter(yymsp[-4].minor.yy119, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy119, 1, status->scanner_state);
  yy_destructor(35,&yymsp[-3].minor);
  yy_destructor(51,&yymsp[-1].minor);
}
// 4571 "parser.c"
        break;
      case 112:
// 1485 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_parameter(NULL, yymsp[-3].minor.yy119, yymsp[-2].minor.yy0, yymsp[0].minor.yy119, 0, status->scanner_state);
  yy_destructor(51,&yymsp[-1].minor);
}
// 4579 "parser.c"
        break;
      case 113:
// 1489 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(19,&yymsp[-2].minor);
  yy_destructor(20,&yymsp[0].minor);
}
// 4588 "parser.c"
        break;
      case 114:
// 1493 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(19,&yymsp[-4].minor);
  yy_destructor(37,&yymsp[-2].minor);
  yy_destructor(58,&yymsp[-1].minor);
  yy_destructor(20,&yymsp[0].minor);
}
// 4599 "parser.c"
        break;
      case 115:
// 1497 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_type(XX_TYPE_INTEGER);
  yy_destructor(59,&yymsp[0].minor);
}
// 4607 "parser.c"
        break;
      case 116:
// 1501 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_type(XX_TYPE_UINTEGER);
  yy_destructor(60,&yymsp[0].minor);
}
// 4615 "parser.c"
        break;
      case 117:
// 1505 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_type(XX_TYPE_LONG);
  yy_destructor(61,&yymsp[0].minor);
}
// 4623 "parser.c"
        break;
      case 118:
// 1509 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_type(XX_TYPE_ULONG);
  yy_destructor(62,&yymsp[0].minor);
}
// 4631 "parser.c"
        break;
      case 119:
// 1513 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_type(XX_TYPE_CHAR);
  yy_destructor(63,&yymsp[0].minor);
}
// 4639 "parser.c"
        break;
      case 120:
// 1517 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_type(XX_TYPE_UCHAR);
  yy_destructor(64,&yymsp[0].minor);
}
// 4647 "parser.c"
        break;
      case 121:
// 1521 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_type(XX_TYPE_DOUBLE);
  yy_destructor(65,&yymsp[0].minor);
}
// 4655 "parser.c"
        break;
      case 122:
// 1525 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_type(XX_TYPE_BOOL);
  yy_destructor(66,&yymsp[0].minor);
}
// 4663 "parser.c"
        break;
      case 123:
// 1529 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_type(XX_TYPE_STRING);
  yy_destructor(67,&yymsp[0].minor);
}
// 4671 "parser.c"
        break;
      case 124:
// 1533 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_type(XX_TYPE_ARRAY);
  yy_destructor(68,&yymsp[0].minor);
}
// 4679 "parser.c"
        break;
      case 125:
// 1537 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_type(XX_TYPE_VAR);
  yy_destructor(69,&yymsp[0].minor);
}
// 4687 "parser.c"
        break;
      case 149:
// 1635 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_empty_statement(status->scanner_state);
  yy_destructor(41,&yymsp[0].minor);
}
// 4695 "parser.c"
        break;
      case 150:
// 1639 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_break_statement(status->scanner_state);
  yy_destructor(70,&yymsp[-1].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 4704 "parser.c"
        break;
      case 151:
// 1643 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_continue_statement(status->scanner_state);
  yy_destructor(71,&yymsp[-1].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 4713 "parser.c"
        break;
      case 152:
// 1647 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_if_statement(yymsp[-2].minor.yy119, NULL, NULL, status->scanner_state);
  yy_destructor(72,&yymsp[-3].minor);
  yy_destructor(48,&yymsp[-1].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4723 "parser.c"
        break;
      case 153:
// 1651 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_if_statement(yymsp[-5].minor.yy119, NULL, NULL, status->scanner_state);
  yy_destructor(72,&yymsp[-6].minor);
  yy_destructor(48,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-3].minor);
  yy_destructor(73,&yymsp[-2].minor);
  yy_destructor(48,&yymsp[-1].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4736 "parser.c"
        break;
      case 154:
// 1655 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_if_statement(yymsp[-3].minor.yy119, yymsp[-1].minor.yy119, NULL, status->scanner_state);
  yy_destructor(72,&yymsp[-4].minor);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4746 "parser.c"
        break;
      case 155:
// 1659 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_if_statement(yymsp[-7].minor.yy119, yymsp[-5].minor.yy119, yymsp[-1].minor.yy119, status->scanner_state);
  yy_destructor(72,&yymsp[-8].minor);
  yy_destructor(48,&yymsp[-6].minor);
  yy_destructor(49,&yymsp[-4].minor);
  yy_destructor(73,&yymsp[-3].minor);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4759 "parser.c"
        break;
      case 156:
// 1663 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_if_statement(yymsp[-6].minor.yy119, yymsp[-4].minor.yy119, NULL, status->scanner_state);
  yy_destructor(72,&yymsp[-7].minor);
  yy_destructor(48,&yymsp[-5].minor);
  yy_destructor(49,&yymsp[-3].minor);
  yy_destructor(73,&yymsp[-2].minor);
  yy_destructor(48,&yymsp[-1].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4772 "parser.c"
        break;
      case 157:
// 1667 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_if_statement(yymsp[-6].minor.yy119, NULL, yymsp[-1].minor.yy119, status->scanner_state);
  yy_destructor(72,&yymsp[-7].minor);
  yy_destructor(48,&yymsp[-5].minor);
  yy_destructor(49,&yymsp[-4].minor);
  yy_destructor(73,&yymsp[-3].minor);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4785 "parser.c"
        break;
      case 158:
// 1671 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_switch_statement(yymsp[-2].minor.yy119, NULL, status->scanner_state);
  yy_destructor(74,&yymsp[-3].minor);
  yy_destructor(48,&yymsp[-1].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4795 "parser.c"
        break;
      case 159:
// 1675 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_switch_statement(yymsp[-3].minor.yy119, yymsp[-1].minor.yy119, status->scanner_state);
  yy_destructor(74,&yymsp[-4].minor);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4805 "parser.c"
        break;
      case 162:
// 1687 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_case_clause(yymsp[-1].minor.yy119, NULL, status->scanner_state);
  yy_destructor(75,&yymsp[-2].minor);
  yy_destructor(76,&yymsp[0].minor);
}
// 4814 "parser.c"
        break;
      case 163:
// 1691 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_case_clause(yymsp[-2].minor.yy119, yymsp[0].minor.yy119, status->scanner_state);
  yy_destructor(75,&yymsp[-3].minor);
  yy_destructor(76,&yymsp[-1].minor);
}
// 4823 "parser.c"
        break;
      case 164:
// 1695 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_case_clause(NULL, yymsp[0].minor.yy119, status->scanner_state);
  yy_destructor(77,&yymsp[-2].minor);
  yy_destructor(76,&yymsp[-1].minor);
}
// 4832 "parser.c"
        break;
      case 165:
// 1699 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_loop_statement(NULL, status->scanner_state);
  yy_destructor(78,&yymsp[-2].minor);
  yy_destructor(48,&yymsp[-1].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4842 "parser.c"
        break;
      case 166:
// 1703 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_loop_statement(yymsp[-1].minor.yy119, status->scanner_state);
  yy_destructor(78,&yymsp[-3].minor);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4852 "parser.c"
        break;
      case 167:
// 1707 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_while_statement(yymsp[-2].minor.yy119, NULL, status->scanner_state);
  yy_destructor(79,&yymsp[-3].minor);
  yy_destructor(48,&yymsp[-1].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4862 "parser.c"
        break;
      case 168:
// 1711 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_while_statement(yymsp[-3].minor.yy119, yymsp[-1].minor.yy119, status->scanner_state);
  yy_destructor(79,&yymsp[-4].minor);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4872 "parser.c"
        break;
      case 169:
// 1715 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_do_while_statement(yymsp[-1].minor.yy119, NULL, status->scanner_state);
  yy_destructor(80,&yymsp[-5].minor);
  yy_destructor(48,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-3].minor);
  yy_destructor(79,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 4884 "parser.c"
        break;
      case 170:
// 1719 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_do_while_statement(yymsp[-1].minor.yy119, yymsp[-4].minor.yy119, status->scanner_state);
  yy_destructor(80,&yymsp[-6].minor);
  yy_destructor(48,&yymsp[-5].minor);
  yy_destructor(49,&yymsp[-3].minor);
  yy_destructor(79,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 4896 "parser.c"
        break;
      case 171:
// 1723 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_for_statement(yymsp[-3].minor.yy119, NULL, yymsp[-5].minor.yy0, 0, yymsp[-1].minor.yy119, status->scanner_state);
  yy_destructor(81,&yymsp[-6].minor);
  yy_destructor(82,&yymsp[-4].minor);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4907 "parser.c"
        break;
      case 172:
// 1727 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_for_statement(yymsp[-3].minor.yy119, NULL, yymsp[-6].minor.yy0, 1, yymsp[-1].minor.yy119, status->scanner_state);
  yy_destructor(81,&yymsp[-7].minor);
  yy_destructor(82,&yymsp[-5].minor);
  yy_destructor(83,&yymsp[-4].minor);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4919 "parser.c"
        break;
      case 173:
// 1731 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_for_statement(yymsp[-3].minor.yy119, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, 0, yymsp[-1].minor.yy119, status->scanner_state);
  yy_destructor(81,&yymsp[-8].minor);
  yy_destructor(6,&yymsp[-6].minor);
  yy_destructor(82,&yymsp[-4].minor);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4931 "parser.c"
        break;
      case 174:
// 1735 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_for_statement(yymsp[-3].minor.yy119, yymsp[-8].minor.yy0, yymsp[-6].minor.yy0, 1, yymsp[-1].minor.yy119, status->scanner_state);
  yy_destructor(81,&yymsp[-9].minor);
  yy_destructor(6,&yymsp[-7].minor);
  yy_destructor(82,&yymsp[-5].minor);
  yy_destructor(83,&yymsp[-4].minor);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4944 "parser.c"
        break;
      case 175:
// 1739 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_let_statement(yymsp[-1].minor.yy119, status->scanner_state);
  yy_destructor(84,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 4953 "parser.c"
        break;
      case 178:
// 1752 "parser.lemon"
{
	yygotominor.yy119 = json_object_new_string("assign");
  yy_destructor(51,&yymsp[0].minor);
}
// 4961 "parser.c"
        break;
      case 179:
// 1757 "parser.lemon"
{
	yygotominor.yy119 = json_object_new_string("add-assign");
  yy_destructor(85,&yymsp[0].minor);
}
// 4969 "parser.c"
        break;
      case 180:
// 1762 "parser.lemon"
{
	yygotominor.yy119 = json_object_new_string("sub-assign");
  yy_destructor(86,&yymsp[0].minor);
}
// 4977 "parser.c"
        break;
      case 181:
// 1766 "parser.lemon"
{
	yygotominor.yy119 = json_object_new_string("mult-assign");
  yy_destructor(87,&yymsp[0].minor);
}
// 4985 "parser.c"
        break;
      case 182:
// 1770 "parser.lemon"
{
	yygotominor.yy119 = json_object_new_string("div-assign");
  yy_destructor(88,&yymsp[0].minor);
}
// 4993 "parser.c"
        break;
      case 183:
// 1774 "parser.lemon"
{
	yygotominor.yy119 = json_object_new_string("concat-assign");
  yy_destructor(89,&yymsp[0].minor);
}
// 5001 "parser.c"
        break;
      case 184:
// 1779 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_let_assignment("variable", yymsp[-1].minor.yy119, yymsp[-2].minor.yy0, NULL, NULL, yymsp[0].minor.yy119, status->scanner_state);
}
// 5008 "parser.c"
        break;
      case 185:
// 1784 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_let_assignment("object-property", yymsp[-1].minor.yy119, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, yymsp[0].minor.yy119, status->scanner_state);
  yy_destructor(38,&yymsp[-3].minor);
}
// 5016 "parser.c"
        break;
      case 186:
// 1789 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_let_assignment("variable-dynamic-object-property", yymsp[-1].minor.yy119, yymsp[-6].minor.yy0, yymsp[-3].minor.yy0, NULL, yymsp[0].minor.yy119, status->scanner_state);
  yy_destructor(38,&yymsp[-5].minor);
  yy_destructor(48,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
}
// 5026 "parser.c"
        break;
      case 187:
// 1794 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_let_assignment("string-dynamic-object-property", yymsp[-1].minor.yy119, yymsp[-6].minor.yy0, yymsp[-3].minor.yy0, NULL, yymsp[0].minor.yy119, status->scanner_state);
  yy_destructor(38,&yymsp[-5].minor);
  yy_destructor(48,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
}
// 5036 "parser.c"
        break;
      case 188:
// 1799 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_let_assignment("object-property-append", yymsp[-1].minor.yy119, yymsp[-6].minor.yy0, yymsp[-4].minor.yy0, NULL, yymsp[0].minor.yy119, status->scanner_state);
  yy_destructor(38,&yymsp[-5].minor);
  yy_destructor(37,&yymsp[-3].minor);
  yy_destructor(58,&yymsp[-2].minor);
}
// 5046 "parser.c"
        break;
      case 189:
// 1804 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_let_assignment("object-property-array-index", yymsp[-1].minor.yy119, yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-2].minor.yy119, yymsp[0].minor.yy119, status->scanner_state);
  yy_destructor(38,&yymsp[-4].minor);
}
// 5054 "parser.c"
        break;
      case 190:
// 1809 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_let_assignment("static-property", yymsp[-1].minor.yy119, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, yymsp[0].minor.yy119, status->scanner_state);
  yy_destructor(91,&yymsp[-3].minor);
}
// 5062 "parser.c"
        break;
      case 191:
// 1814 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_let_assignment("static-property-append", yymsp[-1].minor.yy119, yymsp[-6].minor.yy0, yymsp[-4].minor.yy0, NULL, yymsp[0].minor.yy119, status->scanner_state);
  yy_destructor(91,&yymsp[-5].minor);
  yy_destructor(37,&yymsp[-3].minor);
  yy_destructor(58,&yymsp[-2].minor);
}
// 5072 "parser.c"
        break;
      case 192:
// 1819 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_let_assignment("static-property-array-index", yymsp[-1].minor.yy119, yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-2].minor.yy119, yymsp[0].minor.yy119, status->scanner_state);
  yy_destructor(91,&yymsp[-4].minor);
}
// 5080 "parser.c"
        break;
      case 193:
// 1824 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_let_assignment("variable-append", yymsp[-1].minor.yy119, yymsp[-4].minor.yy0, NULL, NULL, yymsp[0].minor.yy119, status->scanner_state);
  yy_destructor(37,&yymsp[-3].minor);
  yy_destructor(58,&yymsp[-2].minor);
}
// 5089 "parser.c"
        break;
      case 194:
// 1829 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_let_assignment("array-index", yymsp[-1].minor.yy119, yymsp[-3].minor.yy0, NULL, yymsp[-2].minor.yy119, yymsp[0].minor.yy119, status->scanner_state);
}
// 5096 "parser.c"
        break;
      case 197:
// 1841 "parser.lemon"
{
	yygotominor.yy119 = yymsp[-1].minor.yy119;
  yy_destructor(37,&yymsp[-2].minor);
  yy_destructor(58,&yymsp[0].minor);
}
// 5105 "parser.c"
        break;
      case 198:
// 1846 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_let_assignment("object-property-incr", NULL, yymsp[-3].minor.yy0, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(92,&yymsp[0].minor);
}
// 5114 "parser.c"
        break;
      case 199:
// 1851 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_let_assignment("object-property-decr", NULL, yymsp[-3].minor.yy0, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(93,&yymsp[0].minor);
}
// 5123 "parser.c"
        break;
      case 200:
// 1856 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_let_assignment("incr", NULL, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(92,&yymsp[0].minor);
}
// 5131 "parser.c"
        break;
      case 201:
// 1861 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_let_assignment("decr", NULL, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(93,&yymsp[0].minor);
}
// 5139 "parser.c"
        break;
      case 203:
// 1869 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_echo_statement(yymsp[-1].minor.yy119, status->scanner_state);
  yy_destructor(94,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 5148 "parser.c"
        break;
      case 206:
// 1881 "parser.lemon"
{
	yygotominor.yy119 = yymsp[0].minor.yy119;;
}
// 5155 "parser.c"
        break;
      case 207:
// 1886 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_mcall_statement(yymsp[-1].minor.yy119, status->scanner_state);
  yy_destructor(41,&yymsp[0].minor);
}
// 5163 "parser.c"
        break;
      case 208:
// 1891 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_fcall_statement(yymsp[-1].minor.yy119, status->scanner_state);
  yy_destructor(41,&yymsp[0].minor);
}
// 5171 "parser.c"
        break;
      case 209:
// 1896 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_scall_statement(yymsp[-1].minor.yy119, status->scanner_state);
  yy_destructor(41,&yymsp[0].minor);
}
// 5179 "parser.c"
        break;
      case 210:
// 1901 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_fetch_statement(yymsp[-1].minor.yy119, status->scanner_state);
  yy_destructor(41,&yymsp[0].minor);
}
// 5187 "parser.c"
        break;
      case 211:
// 1906 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_return_statement(yymsp[-1].minor.yy119, status->scanner_state);
  yy_destructor(95,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 5196 "parser.c"
        break;
      case 212:
// 1911 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_return_statement(NULL, status->scanner_state);
  yy_destructor(95,&yymsp[-1].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 5205 "parser.c"
        break;
      case 213:
// 1916 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_unset_statement(yymsp[-1].minor.yy119, status->scanner_state);
  yy_destructor(96,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 5214 "parser.c"
        break;
      case 214:
// 1921 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_throw_exception(yymsp[-1].minor.yy119, status->scanner_state);
  yy_destructor(97,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 5223 "parser.c"
        break;
      case 215:
// 1925 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_declare_statement(XX_T_TYPE_INTEGER, yymsp[-1].minor.yy119, status->scanner_state);
  yy_destructor(59,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 5232 "parser.c"
        break;
      case 216:
// 1929 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_declare_statement(XX_T_TYPE_UINTEGER, yymsp[-1].minor.yy119, status->scanner_state);
  yy_destructor(60,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 5241 "parser.c"
        break;
      case 217:
// 1933 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_declare_statement(XX_T_TYPE_CHAR, yymsp[-1].minor.yy119, status->scanner_state);
  yy_destructor(63,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 5250 "parser.c"
        break;
      case 218:
// 1937 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_declare_statement(XX_T_TYPE_UCHAR, yymsp[-1].minor.yy119, status->scanner_state);
  yy_destructor(64,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 5259 "parser.c"
        break;
      case 219:
// 1941 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_declare_statement(XX_T_TYPE_LONG, yymsp[-1].minor.yy119, status->scanner_state);
  yy_destructor(61,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 5268 "parser.c"
        break;
      case 220:
// 1945 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_declare_statement(XX_T_TYPE_ULONG, yymsp[-1].minor.yy119, status->scanner_state);
  yy_destructor(62,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 5277 "parser.c"
        break;
      case 221:
// 1949 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_declare_statement(XX_T_TYPE_DOUBLE, yymsp[-1].minor.yy119, status->scanner_state);
  yy_destructor(65,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 5286 "parser.c"
        break;
      case 222:
// 1953 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_declare_statement(XX_T_TYPE_STRING, yymsp[-1].minor.yy119, status->scanner_state);
  yy_destructor(67,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 5295 "parser.c"
        break;
      case 223:
// 1957 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_declare_statement(XX_T_TYPE_BOOL, yymsp[-1].minor.yy119, status->scanner_state);
  yy_destructor(66,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 5304 "parser.c"
        break;
      case 224:
// 1961 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_declare_statement(XX_T_TYPE_VAR, yymsp[-1].minor.yy119, status->scanner_state);
  yy_destructor(69,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 5313 "parser.c"
        break;
      case 227:
// 1973 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_declare_variable(yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 5320 "parser.c"
        break;
      case 228:
// 1977 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_declare_variable(yymsp[-2].minor.yy0, yymsp[0].minor.yy119, status->scanner_state);
  yy_destructor(51,&yymsp[-1].minor);
}
// 5328 "parser.c"
        break;
      case 230:
// 1985 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_expr("not", yymsp[0].minor.yy119, NULL, NULL, status->scanner_state);
  yy_destructor(35,&yymsp[-1].minor);
}
// 5336 "parser.c"
        break;
      case 231:
// 1989 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_expr("isset", yymsp[0].minor.yy119, NULL, NULL, status->scanner_state);
  yy_destructor(31,&yymsp[-1].minor);
}
// 5344 "parser.c"
        break;
      case 232:
// 1993 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_expr("require", yymsp[0].minor.yy119, NULL, NULL, status->scanner_state);
  yy_destructor(7,&yymsp[-1].minor);
}
// 5352 "parser.c"
        break;
      case 233:
// 1997 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_expr("clone", yymsp[0].minor.yy119, NULL, NULL, status->scanner_state);
  yy_destructor(10,&yymsp[-1].minor);
}
// 5360 "parser.c"
        break;
      case 234:
// 2001 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_expr("empty", yymsp[0].minor.yy119, NULL, NULL, status->scanner_state);
  yy_destructor(11,&yymsp[-1].minor);
}
// 5368 "parser.c"
        break;
      case 235:
// 2005 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_expr("likely", yymsp[0].minor.yy119, NULL, NULL, status->scanner_state);
  yy_destructor(8,&yymsp[-1].minor);
}
// 5376 "parser.c"
        break;
      case 236:
// 2009 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_expr("unlikely", yymsp[0].minor.yy119, NULL, NULL, status->scanner_state);
  yy_destructor(9,&yymsp[-1].minor);
}
// 5384 "parser.c"
        break;
      case 237:
// 2013 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_expr("equals", yymsp[-2].minor.yy119, yymsp[0].minor.yy119, NULL, status->scanner_state);
  yy_destructor(17,&yymsp[-1].minor);
}
// 5392 "parser.c"
        break;
      case 238:
// 2017 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_expr("not-equals", yymsp[-2].minor.yy119, yymsp[0].minor.yy119, NULL, status->scanner_state);
  yy_destructor(24,&yymsp[-1].minor);
}
// 5400 "parser.c"
        break;
      case 239:
// 2021 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_expr("identical", yymsp[-2].minor.yy119, yymsp[0].minor.yy119, NULL, status->scanner_state);
  yy_destructor(18,&yymsp[-1].minor);
}
// 5408 "parser.c"
        break;
      case 240:
// 2025 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_expr("not-identical", yymsp[-2].minor.yy119, yymsp[0].minor.yy119, NULL, status->scanner_state);
  yy_destructor(23,&yymsp[-1].minor);
}
// 5416 "parser.c"
        break;
      case 241:
// 2029 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_expr("less", yymsp[-2].minor.yy119, yymsp[0].minor.yy119, NULL, status->scanner_state);
  yy_destructor(19,&yymsp[-1].minor);
}
// 5424 "parser.c"
        break;
      case 242:
// 2033 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_expr("greater", yymsp[-2].minor.yy119, yymsp[0].minor.yy119, NULL, status->scanner_state);
  yy_destructor(20,&yymsp[-1].minor);
}
// 5432 "parser.c"
        break;
      case 243:
// 2037 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_expr("less-equal", yymsp[-2].minor.yy119, yymsp[0].minor.yy119, NULL, status->scanner_state);
  yy_destructor(21,&yymsp[-1].minor);
}
// 5440 "parser.c"
        break;
      case 244:
// 2041 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_expr("greater-equal", yymsp[-2].minor.yy119, yymsp[0].minor.yy119, NULL, status->scanner_state);
  yy_destructor(22,&yymsp[-1].minor);
}
// 5448 "parser.c"
        break;
      case 245:
// 2045 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_expr("list", yymsp[-1].minor.yy119, NULL, NULL, status->scanner_state);
  yy_destructor(55,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5457 "parser.c"
        break;
      case 246:
// 2049 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_expr("cast", yymsp[-2].minor.yy119, yymsp[0].minor.yy119, NULL, status->scanner_state);
  yy_destructor(55,&yymsp[-3].minor);
  yy_destructor(36,&yymsp[-1].minor);
}
// 5466 "parser.c"
        break;
      case 247:
// 2053 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_expr("type-hint", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), yymsp[0].minor.yy119, NULL, status->scanner_state);
  yy_destructor(19,&yymsp[-3].minor);
  yy_destructor(20,&yymsp[-1].minor);
}
// 5475 "parser.c"
        break;
      case 248:
// 2057 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_expr("property-access", yymsp[-2].minor.yy119, xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-1].minor);
}
// 5483 "parser.c"
        break;
      case 249:
// 2061 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_expr("property-dynamic-access", yymsp[-4].minor.yy119, xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-3].minor);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 5493 "parser.c"
        break;
      case 250:
// 2065 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_expr("property-string-access", yymsp[-4].minor.yy119, xx_ret_literal(XX_T_STRING, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-3].minor);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 5503 "parser.c"
        break;
      case 251:
// 2069 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_expr("static-property-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(91,&yymsp[-1].minor);
}
// 5511 "parser.c"
        break;
      case 252:
      case 320:
// 2073 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_expr("static-constant-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(91,&yymsp[-1].minor);
}
// 5520 "parser.c"
        break;
      case 253:
// 2082 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_expr("array-access", yymsp[-3].minor.yy119, yymsp[-1].minor.yy119, NULL, status->scanner_state);
  yy_destructor(37,&yymsp[-2].minor);
  yy_destructor(58,&yymsp[0].minor);
}
// 5529 "parser.c"
        break;
      case 254:
// 2087 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_expr("add", yymsp[-2].minor.yy119, yymsp[0].minor.yy119, NULL, status->scanner_state);
  yy_destructor(25,&yymsp[-1].minor);
}
// 5537 "parser.c"
        break;
      case 255:
// 2092 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_expr("sub", yymsp[-2].minor.yy119, yymsp[0].minor.yy119, NULL, status->scanner_state);
  yy_destructor(26,&yymsp[-1].minor);
}
// 5545 "parser.c"
        break;
      case 256:
// 2097 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_expr("mul", yymsp[-2].minor.yy119, yymsp[0].minor.yy119, NULL, status->scanner_state);
  yy_destructor(28,&yymsp[-1].minor);
}
// 5553 "parser.c"
        break;
      case 257:
// 2102 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_expr("div", yymsp[-2].minor.yy119, yymsp[0].minor.yy119, NULL, status->scanner_state);
  yy_destructor(29,&yymsp[-1].minor);
}
// 5561 "parser.c"
        break;
      case 258:
// 2107 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_expr("mod", yymsp[-2].minor.yy119, yymsp[0].minor.yy119, NULL, status->scanner_state);
  yy_destructor(30,&yymsp[-1].minor);
}
// 5569 "parser.c"
        break;
      case 259:
// 2112 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_expr("concat", yymsp[-2].minor.yy119, yymsp[0].minor.yy119, NULL, status->scanner_state);
  yy_destructor(27,&yymsp[-1].minor);
}
// 5577 "parser.c"
        break;
      case 260:
// 2117 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_expr("and", yymsp[-2].minor.yy119, yymsp[0].minor.yy119, NULL, status->scanner_state);
  yy_destructor(13,&yymsp[-1].minor);
}
// 5585 "parser.c"
        break;
      case 261:
// 2122 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_expr("or", yymsp[-2].minor.yy119, yymsp[0].minor.yy119, NULL, status->scanner_state);
  yy_destructor(14,&yymsp[-1].minor);
}
// 5593 "parser.c"
        break;
      case 262:
// 2127 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_expr("instanceof", yymsp[-2].minor.yy119, yymsp[0].minor.yy119, NULL, status->scanner_state);
  yy_destructor(12,&yymsp[-1].minor);
}
// 5601 "parser.c"
        break;
      case 263:
// 2132 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_expr("fetch", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), yymsp[0].minor.yy119, NULL, status->scanner_state);
  yy_destructor(32,&yymsp[-3].minor);
  yy_destructor(6,&yymsp[-1].minor);
}
// 5610 "parser.c"
        break;
      case 265:
// 2142 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_expr("typeof", yymsp[0].minor.yy119, NULL, NULL, status->scanner_state);
  yy_destructor(33,&yymsp[-1].minor);
}
// 5618 "parser.c"
        break;
      case 267:
      case 311:
      case 313:
// 2152 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_literal(XX_T_INTEGER, yymsp[0].minor.yy0, status->scanner_state);
}
// 5627 "parser.c"
        break;
      case 268:
      case 310:
      case 315:
// 2157 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_literal(XX_T_STRING, yymsp[0].minor.yy0, status->scanner_state);
}
// 5636 "parser.c"
        break;
      case 269:
      case 314:
// 2162 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_literal(XX_T_CHAR, yymsp[0].minor.yy0, status->scanner_state);
}
// 5644 "parser.c"
        break;
      case 270:
      case 316:
// 2167 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_literal(XX_T_DOUBLE, yymsp[0].minor.yy0, status->scanner_state);
}
// 5652 "parser.c"
        break;
      case 271:
      case 317:
// 2172 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_literal(XX_T_NULL, NULL, status->scanner_state);
  yy_destructor(101,&yymsp[0].minor);
}
// 5661 "parser.c"
        break;
      case 272:
      case 319:
// 2177 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_literal(XX_T_TRUE, NULL, status->scanner_state);
  yy_destructor(102,&yymsp[0].minor);
}
// 5670 "parser.c"
        break;
      case 273:
      case 318:
// 2182 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_literal(XX_T_FALSE, NULL, status->scanner_state);
  yy_destructor(103,&yymsp[0].minor);
}
// 5679 "parser.c"
        break;
      case 274:
      case 321:
// 2187 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_literal(XX_T_CONSTANT, yymsp[0].minor.yy0, status->scanner_state);
}
// 5687 "parser.c"
        break;
      case 275:
// 2192 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_expr("empty-array", NULL, NULL, NULL, status->scanner_state);
  yy_destructor(37,&yymsp[-1].minor);
  yy_destructor(58,&yymsp[0].minor);
}
// 5696 "parser.c"
        break;
      case 276:
// 2197 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_expr("array", yymsp[-1].minor.yy119, NULL, NULL, status->scanner_state);
  yy_destructor(37,&yymsp[-2].minor);
  yy_destructor(58,&yymsp[0].minor);
}
// 5705 "parser.c"
        break;
      case 277:
// 2202 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_new_instance(0, yymsp[0].minor.yy0, NULL, status->scanner_state);
  yy_destructor(34,&yymsp[-1].minor);
}
// 5713 "parser.c"
        break;
      case 278:
// 2207 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_new_instance(0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(34,&yymsp[-3].minor);
  yy_destructor(55,&yymsp[-1].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5723 "parser.c"
        break;
      case 279:
// 2212 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_new_instance(0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy119, status->scanner_state);
  yy_destructor(34,&yymsp[-4].minor);
  yy_destructor(55,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5733 "parser.c"
        break;
      case 280:
// 2217 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_new_instance(1, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(34,&yymsp[-3].minor);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 5743 "parser.c"
        break;
      case 281:
// 2222 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_new_instance(1, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(34,&yymsp[-5].minor);
  yy_destructor(48,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(55,&yymsp[-1].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5755 "parser.c"
        break;
      case 282:
// 2227 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_new_instance(1, yymsp[-4].minor.yy0, yymsp[-1].minor.yy119, status->scanner_state);
  yy_destructor(34,&yymsp[-6].minor);
  yy_destructor(48,&yymsp[-5].minor);
  yy_destructor(49,&yymsp[-3].minor);
  yy_destructor(55,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5767 "parser.c"
        break;
      case 283:
// 2232 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_fcall(1, yymsp[-3].minor.yy0, yymsp[-1].minor.yy119, status->scanner_state);
  yy_destructor(55,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5776 "parser.c"
        break;
      case 284:
// 2237 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_fcall(1, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(55,&yymsp[-1].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5785 "parser.c"
        break;
      case 285:
// 2242 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_fcall(2, yymsp[-4].minor.yy0, yymsp[-1].minor.yy119, status->scanner_state);
  yy_destructor(48,&yymsp[-5].minor);
  yy_destructor(49,&yymsp[-3].minor);
  yy_destructor(55,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5796 "parser.c"
        break;
      case 286:
// 2247 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_fcall(2, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(48,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(55,&yymsp[-1].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5807 "parser.c"
        break;
      case 287:
// 2252 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_scall(0, yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy119, status->scanner_state);
  yy_destructor(91,&yymsp[-4].minor);
  yy_destructor(55,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5817 "parser.c"
        break;
      case 288:
// 2257 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_scall(0, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(91,&yymsp[-3].minor);
  yy_destructor(55,&yymsp[-1].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5827 "parser.c"
        break;
      case 289:
// 2262 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_scall(1, yymsp[-6].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy119, status->scanner_state);
  yy_destructor(48,&yymsp[-7].minor);
  yy_destructor(49,&yymsp[-5].minor);
  yy_destructor(91,&yymsp[-4].minor);
  yy_destructor(55,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5839 "parser.c"
        break;
      case 290:
// 2267 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_scall(1, yymsp[-5].minor.yy0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(48,&yymsp[-6].minor);
  yy_destructor(49,&yymsp[-4].minor);
  yy_destructor(91,&yymsp[-3].minor);
  yy_destructor(55,&yymsp[-1].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5851 "parser.c"
        break;
      case 291:
// 2272 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_scall(1, yymsp[-8].minor.yy0, yymsp[-4].minor.yy0, yymsp[-1].minor.yy119, status->scanner_state);
  yy_destructor(48,&yymsp[-9].minor);
  yy_destructor(49,&yymsp[-7].minor);
  yy_destructor(91,&yymsp[-6].minor);
  yy_destructor(48,&yymsp[-5].minor);
  yy_destructor(49,&yymsp[-3].minor);
  yy_destructor(55,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5865 "parser.c"
        break;
      case 292:
// 2277 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_scall(1, yymsp[-7].minor.yy0, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(48,&yymsp[-8].minor);
  yy_destructor(49,&yymsp[-6].minor);
  yy_destructor(91,&yymsp[-5].minor);
  yy_destructor(48,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(55,&yymsp[-1].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5879 "parser.c"
        break;
      case 293:
// 2282 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_mcall(1, yymsp[-5].minor.yy119, yymsp[-3].minor.yy0, yymsp[-1].minor.yy119, status->scanner_state);
  yy_destructor(38,&yymsp[-4].minor);
  yy_destructor(55,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5889 "parser.c"
        break;
      case 294:
// 2287 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_mcall(1, yymsp[-4].minor.yy119, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-3].minor);
  yy_destructor(55,&yymsp[-1].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5899 "parser.c"
        break;
      case 295:
// 2292 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_mcall(2, yymsp[-7].minor.yy119, yymsp[-4].minor.yy0, yymsp[-1].minor.yy119, status->scanner_state);
  yy_destructor(38,&yymsp[-6].minor);
  yy_destructor(48,&yymsp[-5].minor);
  yy_destructor(49,&yymsp[-3].minor);
  yy_destructor(55,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5911 "parser.c"
        break;
      case 296:
// 2297 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_mcall(2, yymsp[-6].minor.yy119, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-5].minor);
  yy_destructor(48,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(55,&yymsp[-1].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5923 "parser.c"
        break;
      case 297:
// 2302 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_mcall(3, yymsp[-7].minor.yy119, yymsp[-4].minor.yy0, yymsp[-1].minor.yy119, status->scanner_state);
  yy_destructor(38,&yymsp[-6].minor);
  yy_destructor(48,&yymsp[-5].minor);
  yy_destructor(49,&yymsp[-3].minor);
  yy_destructor(55,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5935 "parser.c"
        break;
      case 298:
// 2307 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_mcall(3, yymsp[-6].minor.yy119, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-5].minor);
  yy_destructor(48,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(55,&yymsp[-1].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5947 "parser.c"
        break;
      case 307:
// 2346 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_array_item(yymsp[-2].minor.yy119, yymsp[0].minor.yy119, status->scanner_state);
  yy_destructor(76,&yymsp[-1].minor);
}
// 5955 "parser.c"
        break;
      case 308:
// 2350 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_array_item(NULL, yymsp[0].minor.yy119, status->scanner_state);
}
// 5962 "parser.c"
        break;
      case 323:
// 2410 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_comment(yymsp[0].minor.yy0, status->scanner_state);
}
// 5969 "parser.c"
        break;
      case 324:
// 2414 "parser.lemon"
{
	yygotominor.yy119 = xx_ret_cblock(yymsp[0].minor.yy0, status->scanner_state);
}
// 5976 "parser.c"
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
// 980 "parser.lemon"


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

// 6052 "parser.c"
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
