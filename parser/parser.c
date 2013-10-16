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
#define YYNSTATE 686
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
 /*     0 */   217,  226,  229,  220,  223,    3,    4,    5,    6,    7,
 /*    10 */     8,  378,  268, 1017,    1,    2,  685,    4,    5,    6,
 /*    20 */     7,    8,   26,  186,  213,  276,  281,  295,  210,  105,
 /*    30 */   291,  546,  133,  272,  169,  135,  654,  660,  659,  643,
 /*    40 */   120,  302,  182,  523,  653,  609,  290,  303,  262,  190,
 /*    50 */   120,  549,  486,  495,  504,  507,  498,  501,  510,  516,
 /*    60 */   513,  380,  519,  170,  172,  174,  655,  183,  658,  659,
 /*    70 */   643,  194,  198,  203,  384,  608,  609,  391,  544,  216,
 /*    80 */   375,  120,  403,  284,  449,  422,  188,  455,  470,  474,
 /*    90 */   483,  283,  285,  286,  287,  288,  289,  524,  217,  226,
 /*   100 */   229,  220,  223,  244,  246,  254,  248,  250,  252,  294,
 /*   110 */   268,  114,  190,  376,  549,  241,  237,   80,   88,  560,
 /*   120 */    94,  377,  213,  276,  281,  295,  210,  637,  291,  143,
 /*   130 */   996,  272,  169,  248,  250,  252,  632,  638,  572,  302,
 /*   140 */   591,  523,  241,  237,  290,  273,  262,   11,  568,  394,
 /*   150 */   486,  495,  504,  507,  498,  501,  510,  516,  513,  328,
 /*   160 */   519,  170,  172,  174,  421,  183,  656,  621,  643,  194,
 /*   170 */   198,  203,  384,  625,  609,  391,  392,  454,  397,  120,
 /*   180 */   631,  284,   78,  626,  393,  455,  470,  474,  483,  283,
 /*   190 */   285,  286,  287,  288,  289,  524,  217,  226,  229,  220,
 /*   200 */   223,  561,  407,   32,   56,   35,   37,   38,  268,  238,
 /*   210 */   425,  458,  398,  399,  400,  401,  402,  356,  241,  237,
 /*   220 */   213,  276,  281,  295,  210,  408,  291,  417,  422,  272,
 /*   230 */   169,   20,   34,   35,   37,   38,   23,  302,  146,  523,
 /*   240 */   531,  533,  290,  121,  262,  329,  457,  120,  486,  495,
 /*   250 */   504,  507,  498,  501,  510,  516,  513,   41,  519,  170,
 /*   260 */   172,  174,  438,  183,  607,  397,  138,  194,  198,  203,
 /*   270 */   384,  608,  609,  391,   42,  426,  397,  120,  436,  284,
 /*   280 */   442,  422,  489,  455,  470,  474,  483,  283,  285,  286,
 /*   290 */   287,  288,  289,  524,  217,  226,  229,  220,  223,  398,
 /*   300 */   399,  400,  401,  402,   80,   88,  268,   65,  479,  476,
 /*   310 */   398,  399,  400,  401,  402,  185,  386,  488,  213,  276,
 /*   320 */   281,  295,  210,  594,  291,  430,  686,  272,  169,   80,
 /*   330 */    88,  645,   94,  138,  294,  302,  565,  523,  631,  137,
 /*   340 */   290,  646,  262,  532,  120,  489,  486,  495,  504,  507,
 /*   350 */   498,  501,  510,  516,  513,   15,  519,  170,  172,  174,
 /*   360 */    18,  183,  628,  460,  602,  194,  198,  203,  384,  631,
 /*   370 */   273,  391,  459,  309,  311,  310,  280,  284,  487,  494,
 /*   380 */   497,  455,  470,  474,  483,  283,  285,  286,  287,  288,
 /*   390 */   289,  524,  217,  226,  229,  220,  223,  460,  125,  418,
 /*   400 */   665,  301,  420,  131,  268,  456,  461,  309,  311,  310,
 /*   410 */   280,   28,  443,   77,   57,  420,  213,  276,  281,  295,
 /*   420 */   210,   58,  291,  274,  305,  272,  169,   80,   88,  450,
 /*   430 */    94,  300,  420,  302,  548,  523,  352,  672,  290,  675,
 /*   440 */   262,  112,  131,  489,  486,  495,  504,  507,  498,  501,
 /*   450 */   510,  516,  513,  635,  519,  170,  172,  174,  545,  183,
 /*   460 */   313,  383,  297,  194,  198,  203,  384,  110,  489,  391,
 /*   470 */   113,  309,  311,  310,  280,  284,  496,  494,  500,  455,
 /*   480 */   470,  474,  483,  283,  285,  286,  287,  288,  289,  524,
 /*   490 */   217,  226,  229,  220,  223,  405,  634,  499,  494,  448,
 /*   500 */   502,  494,  268,  503,  489,  309,  311,  310,  280,  661,
 /*   510 */   505,  494,  508,  494,  213,  276,  281,  295,  210,  566,
 /*   520 */   291,  511,  494,  272,  169,   80,   88,  129,   94,  664,
 /*   530 */    23,  302,  547,  523,  514,  494,  290,   22,  262,  506,
 /*   540 */   489,  120,  486,  495,  504,  507,  498,  501,  510,  516,
 /*   550 */   513,  131,  519,  170,  172,  174,  175,  183,   14,  383,
 /*   560 */   489,  194,  198,  203,  384,  116,  489,  391,  119,  309,
 /*   570 */   311,  310,  280,  284,  489,  509,  330,  455,  470,  474,
 /*   580 */   483,  283,  285,  286,  287,  288,  289,  524,  217,  226,
 /*   590 */   229,  220,  223,  405,   64,  512,  301,  451,  517,  494,
 /*   600 */   268,  515,  489,  309,  311,  310,  280,  637,   64,  518,
 /*   610 */   520,  494,  213,  276,  281,  295,  210,  638,  291,  296,
 /*   620 */    84,  272,  169,  491,  115,   81,  307,  327,  301,  302,
 /*   630 */   543,  523,  187,  552,  290,  108,  262,  521,  567,  588,
 /*   640 */   486,  495,  504,  507,  498,  501,  510,  516,  513,  584,
 /*   650 */   519,  170,  172,  174,  600,  183,  416,  107,  317,  194,
 /*   660 */   198,  203,  384,  414,  596,  391,  309,  311,  310,  280,
 /*   670 */    60,  284,  666,   63,  664,  455,  470,  474,  483,  283,
 /*   680 */   285,  286,  287,  288,  289,  524,  217,  226,  229,  220,
 /*   690 */   223,  405,  314,   95,  579,  444,   64,  623,  268,   36,
 /*   700 */   319,  309,  311,  310,  280,  120,  128,  301,  120,  131,
 /*   710 */   213,  276,  281,  295,  210,  364,  291,   64,  342,  272,
 /*   720 */   169,  633,  640,  648,  301,  124,   59,  302,  554,  523,
 /*   730 */   631,  301,  290,  131,  262,   85,  674,  324,  486,  495,
 /*   740 */   504,  507,  498,  501,  510,  516,  513,  595,  519,  170,
 /*   750 */   172,  174,  208,  183,  333,  383,  612,  194,  198,  203,
 /*   760 */   384,  335,  676,  391,  664,  309,  311,  310,  280,  284,
 /*   770 */   341,  357,  189,  455,  470,  474,  483,  283,  285,  286,
 /*   780 */   287,  288,  289,  524,  217,  226,  229,  220,  223,   61,
 /*   790 */   301,   32,   56,   35,   37,   38,  268,  589,  301,   32,
 /*   800 */    56,   35,   37,   38,  301,  301,  613,   10,  213,  276,
 /*   810 */   281,  295,  210,  615,  291,  614,  365,  272,  169,  682,
 /*   820 */   350,  363,  385,  126,  131,  302,  569,  523,  354,  663,
 /*   830 */   290,  617,  262,  619,  361,  367,  486,  495,  504,  507,
 /*   840 */   498,  501,  510,  516,  513,   86,  519,  170,  172,  174,
 /*   850 */   184,  183,  618,  383,  620,  194,  198,  203,  384,  191,
 /*   860 */    13,  391,  998,  309,  311,  310,  280,  284,  684,  997,
 /*   870 */    33,  455,  470,  474,  483,  283,  285,  286,  287,  288,
 /*   880 */   289,  524,  217,  226,  229,  220,  223,  405,  192,  641,
 /*   890 */   127,  441,  279,  215,  268,  627,  677,  309,  311,  310,
 /*   900 */   280,  630,  629,  278,  277,  493,  213,  276,  281,  295,
 /*   910 */   210,  490,  291,  492,  381,  272,  169,  733,  195,   43,
 /*   920 */   734,   17,  679,  302,  599,  523,  636,  345,  290,  275,
 /*   930 */   262,   30,  639,   68,  486,  495,  504,  507,  498,  501,
 /*   940 */   510,  516,  513,  482,  519,  170,  172,  174,  199,  183,
 /*   950 */   478,  383,  270,  194,  198,  203,  384,  642,  477,  391,
 /*   960 */   379,  309,  311,  310,  280,  284,   67,  269,  475,  455,
 /*   970 */   470,  474,  483,  283,  285,  286,  287,  288,  289,  524,
 /*   980 */   217,  226,  229,  220,  223,  405,  651,  200,  680,  437,
 /*   990 */    18,   89,  268,  647,  266,  309,  311,  310,  280,  649,
 /*  1000 */   469,  650,  467,  465,  213,  276,  281,  295,  210,  214,
 /*  1010 */   291,  652,  681,  272,  169,  130,  204,   87,  463,   41,
 /*  1020 */   431,  302,  542,  523,   93,  447,  290,  440,  262,  435,
 /*  1030 */    90,  427,  486,  495,  504,  507,  498,  501,  510,  516,
 /*  1040 */   513,   62,  519,  170,  172,  174,  432,  183,  261,  428,
 /*  1050 */    44,  194,  198,  203,  384,  140,  141,  391,  309,  311,
 /*  1060 */   310,  280,  662,  284,  239,  358,  359,  455,  470,  474,
 /*  1070 */   483,  283,  285,  286,  287,  288,  289,  524,  217,  226,
 /*  1080 */   229,  220,  223,  405,  415,  683,  134,  433,  412,  207,
 /*  1090 */   268,  667,  669,  309,  311,  310,  280,  670,   91,  395,
 /*  1100 */   320,  581,  213,  276,  281,  295,  210,  180,  291,   83,
 /*  1110 */   582,  272,  169,  321,  179,  396,  574,  315,  671,  302,
 /*  1120 */   571,  523,  322,   16,  290,  176,  262,  562,  173,  556,
 /*  1130 */   486,  495,  504,  507,  498,  501,  510,  516,  513,   82,
 /*  1140 */   519,  170,  172,  174,   25,  183,  232,  590,   40,  194,
 /*  1150 */   198,  203,  384,  555,  304,  391,  309,  311,  310,  280,
 /*  1160 */   171,  284,  118,  123,  673,  455,  470,  474,  483,  283,
 /*  1170 */   285,  286,  287,  288,  289,  524,  217,  226,  229,  220,
 /*  1180 */   223,  111,  550,   32,   56,   35,   37,   38,  268,  601,
 /*  1190 */   209,   32,   56,   35,   37,   38,  521,  521,  521,  521,
 /*  1200 */   213,  276,  281,  295,  210,  521,  291,  521,  521,  272,
 /*  1210 */   169,  521,  521,  521,  521,  521,  521,  302,  202,  523,
 /*  1220 */   521,  521,  290,  521,  262,  521,  521,  521,  486,  495,
 /*  1230 */   504,  507,  498,  501,  510,  516,  513,  521,  519,  170,
 /*  1240 */   172,  174,  521,  183,  259,  521,  521,  194,  198,  203,
 /*  1250 */   384,  521,  521,  391,  309,  311,  310,  280,  521,  284,
 /*  1260 */   521,  521,  521,  455,  470,  474,  483,  283,  285,  286,
 /*  1270 */   287,  288,  289,  524,  217,  226,  229,  220,  223,  405,
 /*  1280 */   521,  521,  521,  429,  521,  521,  268,  521,  521,  309,
 /*  1290 */   311,  310,  280,  521,  521,  521,  521,  521,  213,  276,
 /*  1300 */   281,  295,  210,  521,  291,  521,  521,  272,  169,  521,
 /*  1310 */   521,  521,  521,  521,  521,  302,  597,  523,  521,  521,
 /*  1320 */   290,  521,  262,  521,  521,  521,  486,  495,  504,  507,
 /*  1330 */   498,  501,  510,  516,  513,  521,  519,  170,  172,  174,
 /*  1340 */   521,  183,  236,  521,  521,  194,  198,  203,  384,  521,
 /*  1350 */   521,  391,  309,  311,  310,  280,  521,  284,  521,  521,
 /*  1360 */   521,  455,  470,  474,  483,  283,  285,  286,  287,  288,
 /*  1370 */   289,  524,  217,  226,  229,  220,  223,  405,  521,  521,
 /*  1380 */   521,  419,  521,  521,  268,  521,  521,  309,  311,  310,
 /*  1390 */   280,  521,  521,  521,  521,  521,  213,  276,  281,  295,
 /*  1400 */   210,  521,  291,  521,  521,  272,  169,  521,  521,  521,
 /*  1410 */   521,  521,  521,  302,  585,  523,  521,  521,  290,  521,
 /*  1420 */   262,  521,  521,  521,  486,  495,  504,  507,  498,  501,
 /*  1430 */   510,  516,  513,  521,  519,  170,  172,  174,  521,  183,
 /*  1440 */   372,  521,  521,  194,  198,  203,  384,  521,  521,  391,
 /*  1450 */   309,  311,  310,  280,  521,  284,  521,  521,  521,  455,
 /*  1460 */   470,  474,  483,  283,  285,  286,  287,  288,  289,  524,
 /*  1470 */   217,  226,  229,  220,  223,  405,  521,  521,  521,  413,
 /*  1480 */   521,  521,  268,  521,  521,  309,  311,  310,  280,  521,
 /*  1490 */   521,  521,  521,  521,  213,  276,  281,  295,  210,  521,
 /*  1500 */   291,  521,  521,  272,  169,  521,  521,  521,  521,  521,
 /*  1510 */   521,  302,  178,  523,  521,  521,  290,  521,  262,  521,
 /*  1520 */   521,  521,  486,  495,  504,  507,  498,  501,  510,  516,
 /*  1530 */   513,  521,  519,  170,  172,  174,  521,  183,  534,  521,
 /*  1540 */   521,  194,  198,  203,  384,  521,  521,  391,  309,  311,
 /*  1550 */   310,  280,  521,  284,  521,  521,  521,  455,  470,  474,
 /*  1560 */   483,  283,  285,  286,  287,  288,  289,  524,  217,  226,
 /*  1570 */   229,  220,  223,  405,  521,  521,  521,  409,  521,  521,
 /*  1580 */   268,  521,  521,  309,  311,  310,  280,  521,  521,  521,
 /*  1590 */   521,  521,  213,  276,  281,  295,  210,  521,  291,  521,
 /*  1600 */   521,  272,  169,  521,  521,  521,  521,  521,  521,  302,
 /*  1610 */   575,  523,  521,  521,  290,  521,  262,  521,  521,  521,
 /*  1620 */   486,  495,  504,  507,  498,  501,  510,  516,  513,  521,
 /*  1630 */   519,  170,  172,  174,  521,  183,  218,  521,  521,  194,
 /*  1640 */   198,  203,  384,  521,  521,  391,  309,  311,  310,  280,
 /*  1650 */   521,  284,  521,  521,  521,  455,  470,  474,  483,  283,
 /*  1660 */   285,  286,  287,  288,  289,  524,  217,  226,  229,  220,
 /*  1670 */   223,  405,  521,  521,  521,  404,  521,  521,  268,  521,
 /*  1680 */   521,  309,  311,  310,  280,  521,  521,  521,  521,  521,
 /*  1690 */   213,  276,  281,  295,  210,  521,  291,  521,  521,  272,
 /*  1700 */   169,  521,  521,  521,  521,  521,  521,  302,  577,  523,
 /*  1710 */   521,  521,  290,  521,  262,  521,  521,  521,  486,  495,
 /*  1720 */   504,  507,  498,  501,  510,  516,  513,  521,  519,  170,
 /*  1730 */   172,  174,  521,  183,  267,  521,  521,  194,  198,  203,
 /*  1740 */   384,  521,  521,  391,  309,  311,  310,  280,  521,  284,
 /*  1750 */   521,  521,  521,  455,  470,  474,  483,  283,  285,  286,
 /*  1760 */   287,  288,  289,  524,  217,  226,  229,  220,  223,  117,
 /*  1770 */   521,   32,   56,   35,   37,   38,  268,   29,  521,   32,
 /*  1780 */    56,   35,   37,   38,  521,  521,  521,  521,  213,  276,
 /*  1790 */   281,  295,  210,  521,  291,  521,  521,  272,  169,  521,
 /*  1800 */   521,  521,  521,  521,  521,  302,  144,  523,  521,  521,
 /*  1810 */   290,  521,  262,  521,  521,  521,  486,  495,  504,  507,
 /*  1820 */   498,  501,  510,  516,  513,  521,  519,  170,  172,  174,
 /*  1830 */   521,  183,  249,  521,  521,  194,  198,  203,  384,  521,
 /*  1840 */   521,  391,  309,  311,  310,  280,  521,  284,  521,  521,
 /*  1850 */   521,  455,  470,  474,  483,  283,  285,  286,  287,  288,
 /*  1860 */   289,  524,  217,  226,  229,  220,  223,  573,  521,   32,
 /*  1870 */    56,   35,   37,   38,  268,  521,  521,  521,  521,  521,
 /*  1880 */   521,  521,  521,  521,  521,  521,  213,  276,  281,  295,
 /*  1890 */   210,  521,  291,  521,  521,  272,  169,  521,  521,  521,
 /*  1900 */   521,  521,  521,  302,  206,  523,  521,  521,  290,  521,
 /*  1910 */   262,  521,  521,  521,  486,  495,  504,  507,  498,  501,
 /*  1920 */   510,  516,  513,  521,  519,  170,  172,  174,  521,  183,
 /*  1930 */   234,  521,  521,  194,  198,  203,  384,  521,  265,  391,
 /*  1940 */   309,  311,  310,  280,  521,  284,  521,  521,  521,  455,
 /*  1950 */   470,  474,  483,  283,  285,  286,  287,  288,  289,  524,
 /*  1960 */   217,  226,  229,  220,  223,  263,  521,  521,  521,  521,
 /*  1970 */   521,  521,  268,  521,  521,  309,  311,  310,  280,  521,
 /*  1980 */   521,  521,  521,  521,  213,  276,  281,  295,  210,  521,
 /*  1990 */   291,  521,  521,  272,  169,  521,  521,  521,  521,  521,
 /*  2000 */   521,  302,  603,  523,  521,  521,  290,  521,  262,  521,
 /*  2010 */   521,  521,  486,  495,  504,  507,  498,  501,  510,  516,
 /*  2020 */   513,  521,  519,  170,  172,  174,  521,  183,  370,  521,
 /*  2030 */   521,  194,  198,  203,  384,  521,  521,  391,  309,  311,
 /*  2040 */   310,  280,  521,  284,  521,  521,  521,  455,  470,  474,
 /*  2050 */   483,  283,  285,  286,  287,  288,  289,  524,  217,  226,
 /*  2060 */   229,  220,  223,  387,  521,  521,  521,  521,  521,  521,
 /*  2070 */   268,  521,  521,  309,  311,  310,  280,  521,  521,  521,
 /*  2080 */   521,  521,  213,  276,  281,  295,  210,  521,  291,  521,
 /*  2090 */   521,  272,  169,  521,  521,  521,  521,  521,  521,  302,
 /*  2100 */   587,  523,  521,  521,  290,  521,  262,  521,  521,  521,
 /*  2110 */   486,  495,  504,  507,  498,  501,  510,  516,  513,  521,
 /*  2120 */   519,  170,  172,  174,  521,  183,  371,  521,  521,  194,
 /*  2130 */   198,  203,  384,  521,  521,  391,  309,  311,  310,  280,
 /*  2140 */   521,  284,  521,  521,  521,  455,  470,  474,  483,  283,
 /*  2150 */   285,  286,  287,  288,  289,  524,  217,  226,  229,  220,
 /*  2160 */   223,  271,  521,  521,  521,  521,  521,  521,  268,  521,
 /*  2170 */   521,  309,  311,  310,  280,  521,  521,  521,  521,  521,
 /*  2180 */   213,  276,  281,  295,  210,  521,  291,  521,  521,  272,
 /*  2190 */   169,  521,  521,  521,  521,  521,  521,  302,  558,  523,
 /*  2200 */   521,  521,  290,  521,  262,  521,  521,  521,  486,  495,
 /*  2210 */   504,  507,  498,  501,  510,  516,  513,  521,  519,  170,
 /*  2220 */   172,  174,  521,  183,  471,  521,  521,  194,  198,  203,
 /*  2230 */   384,  521,  521,  391,  309,  311,  310,  280,  521,  284,
 /*  2240 */   521,  521,  521,  455,  470,  474,  483,  283,  285,  286,
 /*  2250 */   287,  288,  289,  524,  217,  226,  229,  220,  223,  382,
 /*  2260 */   521,  521,  521,  521,  521,  521,  268,  521,  521,  309,
 /*  2270 */   311,  310,  280,  521,  521,  521,  521,  521,  213,  276,
 /*  2280 */   281,  295,  210,  521,  291,  521,  521,  272,  169,  521,
 /*  2290 */   521,  521,  521,  521,  521,  302,  537,  523,  521,  521,
 /*  2300 */   290,  521,  262,  521,  521,  521,  486,  495,  504,  507,
 /*  2310 */   498,  501,  510,  516,  513,  521,  519,  170,  172,  174,
 /*  2320 */   521,  183,  539,  521,  521,  194,  198,  203,  384,  521,
 /*  2330 */   521,  391,  309,  311,  310,  280,  521,  284,  521,  521,
 /*  2340 */   521,  455,  470,  474,  483,  283,  285,  286,  287,  288,
 /*  2350 */   289,  524,  217,  226,  229,  220,  223,  282,  521,  521,
 /*  2360 */   521,  521,  521,  521,  268,  521,  521,  309,  311,  310,
 /*  2370 */   280,  521,  521,  521,  521,  521,  213,  276,  281,  295,
 /*  2380 */   210,  521,  291,  521,  521,  272,  169,  521,  521,  521,
 /*  2390 */   521,  521,  521,  302,  390,  523,  521,  521,  290,  521,
 /*  2400 */   262,  521,  521,  521,  486,  495,  504,  507,  498,  501,
 /*  2410 */   510,  516,  513,  521,  519,  170,  172,  174,  521,  183,
 /*  2420 */   247,  521,  521,  194,  198,  203,  384,  521,  521,  391,
 /*  2430 */   309,  311,  310,  280,  521,  284,  521,  521,  521,  455,
 /*  2440 */   470,  474,  483,  283,  285,  286,  287,  288,  289,  524,
 /*  2450 */   217,  226,  229,  220,  223,  257,  521,  521,  521,  521,
 /*  2460 */   521,  521,  268,  521,  521,  309,  311,  310,  280,  521,
 /*  2470 */   521,  521,  521,  521,  213,  276,  281,  295,  210,  521,
 /*  2480 */   291,  521,  521,  272,  169,  521,  521,  521,  521,  521,
 /*  2490 */   521,  302,  530,  523,  521,  521,  290,  521,  262,  521,
 /*  2500 */   521,  521,  486,  495,  504,  507,  498,  501,  510,  516,
 /*  2510 */   513,  521,  519,  170,  172,  174,  521,  183,  224,  521,
 /*  2520 */   521,  194,  198,  203,  384,  521,  521,  391,  309,  311,
 /*  2530 */   310,  280,  521,  284,  521,  521,  521,  455,  470,  474,
 /*  2540 */   483,  283,  285,  286,  287,  288,  289,  524,  217,  226,
 /*  2550 */   229,  220,  223,  480,  521,  521,  521,  521,  521,  521,
 /*  2560 */   268,  521,  521,  309,  311,  310,  280,  521,  521,  521,
 /*  2570 */   521,  521,  213,  276,  281,  295,  210,  521,  291,  521,
 /*  2580 */   521,  272,  169,  521,  521,  521,  521,  521,  521,  302,
 /*  2590 */   559,  523,  521,  521,  290,  521,  262,  521,  521,  521,
 /*  2600 */   486,  495,  504,  507,  498,  501,  510,  516,  513,  521,
 /*  2610 */   519,  170,  172,  174,  521,  183,  251,  521,  521,  194,
 /*  2620 */   198,  203,  384,  521,  521,  391,  309,  311,  310,  280,
 /*  2630 */   521,  284,  521,  521,  521,  455,  470,  474,  483,  283,
 /*  2640 */   285,  286,  287,  288,  289,  524,  217,  226,  229,  220,
 /*  2650 */   223,  221,  521,  521,  521,  521,  521,  521,  268,  521,
 /*  2660 */   521,  309,  311,  310,  280,  521,  521,  521,  521,  521,
 /*  2670 */   213,  276,  281,  295,  210,  521,  291,  521,  521,  272,
 /*  2680 */   169,  521,  521,  521,  521,  521,  521,  302,  605,  523,
 /*  2690 */   521,  521,  290,  521,  262,  521,  521,  521,  486,  495,
 /*  2700 */   504,  507,  498,  501,  510,  516,  513,  521,  519,  170,
 /*  2710 */   172,  174,  521,  183,  245,  521,  521,  194,  198,  203,
 /*  2720 */   384,  521,  521,  391,  309,  311,  310,  280,  521,  284,
 /*  2730 */   521,  521,  521,  455,  470,  474,  483,  283,  285,  286,
 /*  2740 */   287,  288,  289,  524,  217,  226,  229,  220,  223,  230,
 /*  2750 */   521,  521,  521,  521,  521,  521,  268,  521,  521,  309,
 /*  2760 */   311,  310,  280,  521,  521,  521,  521,  521,  213,  276,
 /*  2770 */   281,  295,  210,  521,  291,  521,  521,  272,  169,  521,
 /*  2780 */   521,  521,  521,  521,  521,  302,  563,  523,  521,  521,
 /*  2790 */   290,  521,  262,  521,  521,  521,  486,  495,  504,  507,
 /*  2800 */   498,  501,  510,  516,  513,  521,  519,  170,  172,  174,
 /*  2810 */   521,  183,  369,  521,  521,  194,  198,  203,  384,  521,
 /*  2820 */   521,  391,  309,  311,  310,  280,  521,  284,  521,  521,
 /*  2830 */   521,  455,  470,  474,  483,  283,  285,  286,  287,  288,
 /*  2840 */   289,  524,  217,  226,  229,  220,  223,  373,  521,  521,
 /*  2850 */   521,  521,  521,  521,  268,  521,  521,  309,  311,  310,
 /*  2860 */   280,  521,  521,  521,  521,  521,  213,  276,  281,  295,
 /*  2870 */   210,  521,  291,  521,  521,  272,  169,  521,  521,  521,
 /*  2880 */   521,  521,  521,  302,  593,  523,  521,  521,  290,  521,
 /*  2890 */   262,  521,  521,  521,  486,  495,  504,  507,  498,  501,
 /*  2900 */   510,  516,  513,  521,  519,  170,  172,  174,  521,  183,
 /*  2910 */   227,  521,  521,  194,  198,  203,  384,  521,  521,  391,
 /*  2920 */   309,  311,  310,  280,  521,  284,  521,  521,  521,  455,
 /*  2930 */   470,  474,  483,  283,  285,  286,  287,  288,  289,  524,
 /*  2940 */   217,  226,  229,  220,  223,  242,  521,  521,  521,  521,
 /*  2950 */   521,  521,  268,  521,  521,  309,  311,  310,  280,  521,
 /*  2960 */   521,  521,  521,  521,  213,  276,  281,  295,  210,  521,
 /*  2970 */   291,  521,  521,  272,  169,  521,  521,  521,  521,  521,
 /*  2980 */   521,  302,  197,  523,  521,  521,  290,  521,  262,  521,
 /*  2990 */   521,  521,  486,  495,  504,  507,  498,  501,  510,  516,
 /*  3000 */   513,  521,  519,  170,  172,  174,  521,  183,  527,  521,
 /*  3010 */   521,  194,  198,  203,  384,  521,  521,  391,  309,  311,
 /*  3020 */   310,  280,  521,  284,  521,  521,  521,  455,  470,  474,
 /*  3030 */   483,  283,  285,  286,  287,  288,  289,  524,  217,  226,
 /*  3040 */   229,  220,  223,  484,  521,  521,  521,  521,  521,  521,
 /*  3050 */   268,  521,  521,  309,  311,  310,  280,  521,  521,  521,
 /*  3060 */   521,  521,  213,  276,  281,  295,  210,  521,  291,  521,
 /*  3070 */   521,  272,  169,  521,  521,  521,  521,  521,  521,  302,
 /*  3080 */   553,  523,  521,  521,  290,  521,  262,  521,  521,  521,
 /*  3090 */   486,  495,  504,  507,  498,  501,  510,  516,  513,  521,
 /*  3100 */   519,  170,  172,  174,  521,  183,  253,  521,  521,  194,
 /*  3110 */   198,  203,  384,  521,  521,  391,  309,  311,  310,  280,
 /*  3120 */   521,  284,  521,  521,  521,  455,  470,  474,  483,  283,
 /*  3130 */   285,  286,  287,  288,  289,  524,  217,  226,  229,  220,
 /*  3140 */   223,  255,  521,  521,  521,  521,  521,  521,  268,  521,
 /*  3150 */   521,  309,  311,  310,  280,  521,  521,  521,  521,  521,
 /*  3160 */   213,  276,  281,  295,  210,  521,  291,  521,  521,  272,
 /*  3170 */   169,  521,  521,  521,  521,  521,  521,  302,  521,  523,
 /*  3180 */   521,  521,  290,  521,  262,  521,  521,  521,  486,  495,
 /*  3190 */   504,  507,  498,  501,  510,  516,  513,  521,  519,  170,
 /*  3200 */   172,  174,  521,  183,  211,  521,  521,  194,  198,  203,
 /*  3210 */   384,  521,  521,  391,  309,  311,  310,  280,  521,  284,
 /*  3220 */   521,  521,  521,  455,  470,  474,  483,  283,  285,  286,
 /*  3230 */   287,  288,  289,  524,  217,  226,  229,  220,  223,  521,
 /*  3240 */     9,  521,  521,   12,  521,  122,  268,  668,  678,  521,
 /*  3250 */   521,  523,  521,  521,  521,  521,  521,  521,  213,  276,
 /*  3260 */   281,  295,  210,  521,  291,  521,   39,  272,  521,  521,
 /*  3270 */   521,  521,  521,  521,  521,  302,  521,  521,  521,  521,
 /*  3280 */   290,  521,  262,  521,  521,  521,   45,   46,   47,   48,
 /*  3290 */    49,   50,   51,   52,   53,   54,   55,  521,  521,  521,
 /*  3300 */   521,  521,  167,  148,   31,  524,   45,   46,   47,   48,
 /*  3310 */    49,   50,   51,   52,   53,   54,   55,  284,  521,  521,
 /*  3320 */   521,  521,  521,  145,  521,  283,  285,  286,  287,  288,
 /*  3330 */   289,  521,  525,  149,  150,  151,  152,  153,  154,  155,
 /*  3340 */   156,  157,  158,  159,  160,  161,  162,  163,  164,  165,
 /*  3350 */   166,  168,  167,  148,  521,  522,  521,  521,  521,  292,
 /*  3360 */   521,  521,  521,  521,  521,  462,  464,  466,  468,  309,
 /*  3370 */   311,  310,  280,  529,  521,   96,   97,   99,   98,  100,
 /*  3380 */   521,  346,  525,  149,  150,  151,  152,  153,  154,  155,
 /*  3390 */   156,  157,  158,  159,  160,  161,  162,  163,  164,  165,
 /*  3400 */   166,  168,  167,  148,  521,  522,  521,  521,  521,  521,
 /*  3410 */   521,  521,  521,  521,  624,  462,  464,  466,  468,  521,
 /*  3420 */   102,  103,  521,  205,  521,  521,  521,  521,  580,  521,
 /*  3430 */   101,  521,  525,  149,  150,  151,  152,  153,  154,  155,
 /*  3440 */   156,  157,  158,  159,  160,  161,  162,  163,  164,  165,
 /*  3450 */   166,  168,  167,  148,  521,  522,  521,  521,  521,  521,
 /*  3460 */   521,  521,  521,  521,  521,  462,  464,  466,  468,  521,
 /*  3470 */   521,  521,  521,  389,  521,   96,   97,   99,   98,  100,
 /*  3480 */   521,  521,  525,  149,  150,  151,  152,  153,  154,  155,
 /*  3490 */   156,  157,  158,  159,  160,  161,  162,  163,  164,  165,
 /*  3500 */   166,  168,  167,  148,  521,  522,  521,  521,  521,  521,
 /*  3510 */   521,  521,  521,  521,  644,  462,  464,  466,  468,  521,
 /*  3520 */   102,  103,  521,  604,  521,  521,  521,  521,  139,  521,
 /*  3530 */   101,  521,  525,  149,  150,  151,  152,  153,  154,  155,
 /*  3540 */   156,  157,  158,  159,  160,  161,  162,  163,  164,  165,
 /*  3550 */   166,  168,  167,  148,  521,  522,  521,  521,  410,  521,
 /*  3560 */   521,  521,  521,  521,  521,  462,  464,  466,  468,  521,
 /*  3570 */   521,  521,  397,  564,  521,   96,   97,   99,   98,  100,
 /*  3580 */   521,  521,  525,  149,  150,  151,  152,  153,  154,  155,
 /*  3590 */   156,  157,  158,  159,  160,  161,  162,  163,  164,  165,
 /*  3600 */   166,  168,  167,  148,  521,  522,  398,  399,  400,  401,
 /*  3610 */   402,  521,  521,  423,  424,  462,  464,  466,  468,  521,
 /*  3620 */   102,  103,  521,  586,  521,  521,  521,  521,  580,  521,
 /*  3630 */   101,  521,  525,  149,  150,  151,  152,  153,  154,  155,
 /*  3640 */   156,  157,  158,  159,  160,  161,  162,  163,  164,  165,
 /*  3650 */   166,  168,  167,  148,  521,  522,  521,  521,  521,  521,
 /*  3660 */   521,  521,  521,  521,  521,  462,  464,  466,  468,  521,
 /*  3670 */   521,  521,  521,  570,  521,   96,   97,   99,   98,  100,
 /*  3680 */   521,  521,  525,  149,  150,  151,  152,  153,  154,  155,
 /*  3690 */   156,  157,  158,  159,  160,  161,  162,  163,  164,  165,
 /*  3700 */   166,  168,  167,  148,  521,  522,  521,  521,  521,  521,
 /*  3710 */   521,  521,  521,  521,  521,  462,  464,  466,  468,  521,
 /*  3720 */   102,  103,  521,  177,  521,  521,  521,  521,  139,  521,
 /*  3730 */   101,  521,  525,  149,  150,  151,  152,  153,  154,  155,
 /*  3740 */   156,  157,  158,  159,  160,  161,  162,  163,  164,  165,
 /*  3750 */   166,  168,  167,  148,  521,  522,  521,  521,  521,  521,
 /*  3760 */   521,  521,  521,  521,  521,  462,  464,  466,  468,  521,
 /*  3770 */   521,  521,  521,  196,  521,   96,   97,   99,   98,  100,
 /*  3780 */   521,  521,  525,  149,  150,  151,  152,  153,  154,  155,
 /*  3790 */   156,  157,  158,  159,  160,  161,  162,  163,  164,  165,
 /*  3800 */   166,  168,  167,  148,  521,  522,  521,  521,  521,  521,
 /*  3810 */   521,  521,  521,  521,  521,  462,  464,  466,  468,  521,
 /*  3820 */   102,  103,  521,  576,  521,  521,  611,  521,  521,  521,
 /*  3830 */   101,  521,  525,  149,  150,  151,  152,  153,  154,  155,
 /*  3840 */   156,  157,  158,  159,  160,  161,  162,  163,  164,  165,
 /*  3850 */   166,  168,  167,  148,  521,  522,  521,  521,  521,  521,
 /*  3860 */   521,  521,  521,  521,  521,  462,  464,  466,  468,  521,
 /*  3870 */   521,  521,  521,  598,  521,   96,   97,   99,   98,  100,
 /*  3880 */   521,  521,  525,  149,  150,  151,  152,  153,  154,  155,
 /*  3890 */   156,  157,  158,  159,  160,  161,  162,  163,  164,  165,
 /*  3900 */   166,  168,  167,  148,  521,  522,  521,  521,  521,  521,
 /*  3910 */   521,  521,  521,  521,  521,  462,  464,  466,  468,  521,
 /*  3920 */   102,  103,  521,  551,  521,  521,  521,  521,  106,  521,
 /*  3930 */   101,  521,  525,  149,  150,  151,  152,  153,  154,  155,
 /*  3940 */   156,  157,  158,  159,  160,  161,  162,  163,  164,  165,
 /*  3950 */   166,  168,  167,  148,  521,  522,  521,  521,  521,  292,
 /*  3960 */   521,  521,  521,  521,  521,  462,  464,  466,  468,  309,
 /*  3970 */   311,  310,  280,  181,  521,  521,  340,  521,  521,  348,
 /*  3980 */   344,  347,  525,  149,  150,  151,  152,  153,  154,  155,
 /*  3990 */   156,  157,  158,  159,  160,  161,  162,  163,  164,  165,
 /*  4000 */   166,  168,  167,  148,  521,  522,  521,  521,  521,  521,
 /*  4010 */   521,  521,  521,  521,  521,  462,  464,  466,  468,  521,
 /*  4020 */   521,  521,  521,  592,  521,   96,   97,   99,   98,  100,
 /*  4030 */   521,  521,  525,  149,  150,  151,  152,  153,  154,  155,
 /*  4040 */   156,  157,  158,  159,  160,  161,  162,  163,  164,  165,
 /*  4050 */   166,  168,  167,  148,  521,  522,  521,  521,  521,  521,
 /*  4060 */   521,  521,  521,  521,  521,  462,  464,  466,  468,  521,
 /*  4070 */   102,  103,  521,  541,  521,  521,  521,  521,   24,  521,
 /*  4080 */   101,  521,  525,  149,  150,  151,  152,  153,  154,  155,
 /*  4090 */   156,  157,  158,  159,  160,  161,  162,  163,  164,  165,
 /*  4100 */   166,  168,  167,  148,  521,  522,  521,  521,  521,  521,
 /*  4110 */   521,  521,  521,  521,  521,  462,  464,  466,  468,  521,
 /*  4120 */   521,  521,  521,  201,  521,   96,   97,   99,   98,  100,
 /*  4130 */   521,  521,  525,  149,  150,  151,  152,  153,  154,  155,
 /*  4140 */   156,  157,  158,  159,  160,  161,  162,  163,  164,  165,
 /*  4150 */   166,  168,  167,  148,  521,  522,  521,  521,  521,  521,
 /*  4160 */   521,  521,  521,  521,  521,  462,  464,  466,  468,  521,
 /*  4170 */   102,  103,  521,  536,  521,  521,  521,  521,  521,  521,
 /*  4180 */   101,  521,  525,  149,  150,  151,  152,  153,  154,  155,
 /*  4190 */   156,  157,  158,  159,  160,  161,  162,  163,  164,  165,
 /*  4200 */   166,  168,  167,  148,  521,  522,  521,  521,  521,  292,
 /*  4210 */   521,  521,  521,  521,  521,  462,  464,  466,  468,  309,
 /*  4220 */   311,  310,  280,  557,  521,  521,  521,  521,  521,  343,
 /*  4230 */   344,  347,  525,  149,  150,  151,  152,  153,  154,  155,
 /*  4240 */   156,  157,  158,  159,  160,  161,  162,  163,  164,  165,
 /*  4250 */   166,  168,  167,  148,  521,  522,  521,  521,  521,  521,
 /*  4260 */   521,  521,  521,  521,  521,  462,  464,  466,  468,  521,
 /*  4270 */   521,  521,  521,  193,  521,   96,   97,   99,   98,  100,
 /*  4280 */   521,  521,  525,  149,  150,  151,  152,  153,  154,  155,
 /*  4290 */   156,  157,  158,  159,  160,  161,  162,  163,  164,  165,
 /*  4300 */   166,  168,  167,  148,  521,  522,  521,  521,  521,  521,
 /*  4310 */   521,  521,  521,  521,  521,  462,  464,  466,  468,  521,
 /*  4320 */   102,  103,  521,  718,  610,  521,  616,  521,  521,  521,
 /*  4330 */   101,  521,  147,  149,  150,  151,  152,  153,  154,  155,
 /*  4340 */   156,  157,  158,  159,  160,  161,  162,  163,  164,  165,
 /*  4350 */   166,  168,  521,  521,  521,  522,  521,  217,  226,  229,
 /*  4360 */   220,  223,  687,  521,  521,  462,  464,  466,  468,  268,
 /*  4370 */   521,  521,  521,  521,  521,  521,  521,  521,  521,  521,
 /*  4380 */   521,  213,  276,  281,  295,  210,  331,  291,  521,  521,
 /*  4390 */   272,  521,  521,  521,  521,  521,  521,  521,  302,  521,
 /*  4400 */   521,    9,  521,  290,   12,  262,  122,  521,  668,  678,
 /*  4410 */   521,  521,  523,  521,  521,  521,  260,  256,  258,  521,
 /*  4420 */   521,  212,  222,  228,  231,  233,  235,  225,  219,  244,
 /*  4430 */   246,  254,  248,  250,  252,  521,  521,  521,  521,  521,
 /*  4440 */   284,  241,  237,  521,  521,  485,  521,  521,  283,  285,
 /*  4450 */   286,  287,  288,  289,  521,  521,  217,  226,  229,  220,
 /*  4460 */   223,  606,  136,  621,  643,  521,  524,  521,  268,  653,
 /*  4470 */   609,  521,  521,  521,  521,  120,  521,  521,  521,  521,
 /*  4480 */   213,  276,  281,  295,  210,  325,  291,  521,  240,  272,
 /*  4490 */   521,  521,  521,  521,  521,  521,  521,  302,  309,  311,
 /*  4500 */   310,  280,  290,  521,  262,  521,  366,  312,  521,  521,
 /*  4510 */   521,  521,  521,  521,  521,  260,  256,  258,  521,  521,
 /*  4520 */   212,  222,  228,  231,  233,  235,  225,  219,  244,  246,
 /*  4530 */   254,  248,  250,  252,  521,  521,  521,  521,  521,  284,
 /*  4540 */   241,  237,  521,  521,  521,  521,  521,  283,  285,  286,
 /*  4550 */   287,  288,  289,  240,  521,  217,  226,  229,  220,  223,
 /*  4560 */   521,  481,  521,  309,  311,  310,  280,  268,  521,  521,
 /*  4570 */   521,  306,  312,  521,  521,  521,  521,  521,  521,  213,
 /*  4580 */   276,  281,  295,  210,  308,  291,  521,  521,  272,  521,
 /*  4590 */   521,  521,  521,  521,  240,  521,  302,  521,  521,  521,
 /*  4600 */   521,  290,  521,  262,  309,  311,  310,  280,  521,  260,
 /*  4610 */   256,  258,  353,  312,  212,  222,  228,  231,  233,  235,
 /*  4620 */   225,  219,  244,  246,  254,  248,  250,  252,   96,   97,
 /*  4630 */    99,   98,  100,  521,  241,  237,  521,  521,  284,  521,
 /*  4640 */   521,  521,  521,  521,  521,  540,  283,  285,  286,  287,
 /*  4650 */   288,  289,  240,  521,  217,  226,  229,  220,  223,  521,
 /*  4660 */   521,  521,  309,  311,  310,  280,  268,  521,  521,  521,
 /*  4670 */   360,  312,  521,  102,  103,  521,  722,  578,  213,  276,
 /*  4680 */   281,  295,  210,  101,  291,  521,  521,  272,  521,  521,
 /*  4690 */   521,  521,  521,  521,  521,  302,  521,  521,  521,  521,
 /*  4700 */   290,  521,  262,  521,  521,  446,  521,  521,  260,  256,
 /*  4710 */   258,  521,  521,  212,  222,  228,  231,  233,  235,  225,
 /*  4720 */   219,  244,  246,  254,  248,  250,  252,  521,  521,  521,
 /*  4730 */   521,  521,  521,  241,  237,  521,  521,  284,  521,  521,
 /*  4740 */   521,  521,  521,  521,  388,  283,  285,  286,  287,  288,
 /*  4750 */   289,  240,  521,  217,  226,  229,  220,  223,  521,  521,
 /*  4760 */   521,  309,  311,  310,  280,  268,  521,  521,  521,  332,
 /*  4770 */   312,  521,  521,  521,  521,  521,  521,  213,  276,  281,
 /*  4780 */   295,  210,  336,  291,  521,  240,  272,  521,  521,  521,
 /*  4790 */   521,  521,  521,  521,  302,  309,  311,  310,  280,  290,
 /*  4800 */   521,  262,  521,  316,  312,  521,  521,  521,  521,  521,
 /*  4810 */   521,  521,  260,  256,  258,  521,  521,  212,  222,  228,
 /*  4820 */   231,  233,  235,  225,  219,  244,  246,  254,  248,  250,
 /*  4830 */   252,  521,  521,  521,  521,  521,  284,  241,  237,  521,
 /*  4840 */   521,  521,  521,  521,  283,  285,  286,  287,  288,  289,
 /*  4850 */   240,  521,  217,  226,  229,  220,  223,  521,  374,  521,
 /*  4860 */   309,  311,  310,  280,  268,  521,  521,  521,  323,  312,
 /*  4870 */   521,  521,  521,  521,  521,  521,  213,  276,  281,  295,
 /*  4880 */   210,  355,  291,  521,  240,  272,  521,  521,  521,  521,
 /*  4890 */   521,  521,  521,  302,  309,  311,  310,  280,  290,  521,
 /*  4900 */   262,  521,  334,  312,  521,  521,  260,  256,  258,  521,
 /*  4910 */   521,  212,  222,  228,  231,  233,  235,  225,  219,  244,
 /*  4920 */   246,  254,  248,  250,  252,   96,   97,   99,   98,  100,
 /*  4930 */   264,  241,  237,  521,  521,  284,  521,  521,  521,  521,
 /*  4940 */   521,  521,  521,  283,  285,  286,  287,  288,  289,  240,
 /*  4950 */   521,  217,  226,  229,  220,  223,  521,  521,  521,  309,
 /*  4960 */   311,  310,  280,  268,  521,  521,  521,  349,  312,  521,
 /*  4970 */   102,  103,  521,  717,  578,  213,  276,  281,  295,  210,
 /*  4980 */   101,  291,  521,  521,  272,  521,  521,  521,  521,  521,
 /*  4990 */   521,  521,  302,  521,  521,  240,  521,  290,  521,  262,
 /*  5000 */   521,  521,  411,  521,  521,  309,  311,  310,  280,  521,
 /*  5010 */   260,  256,  258,  299,  312,  212,  222,  228,  231,  233,
 /*  5020 */   235,  225,  219,  244,  246,  254,  248,  250,  252,  521,
 /*  5030 */   521,  521,  521,  521,  284,  241,  237,  521,  521,  472,
 /*  5040 */   521,  521,  283,  285,  286,  287,  288,  289,  240,  521,
 /*  5050 */   217,  226,  229,  220,  223,  521,  521,  521,  309,  311,
 /*  5060 */   310,  280,  268,  521,  521,  521,  521,  326,  521,  521,
 /*  5070 */   521,  521,  521,  521,  213,  276,  281,  295,  210,  521,
 /*  5080 */   291,  521,  521,  293,  521,  521,  521,  521,  521,  521,
 /*  5090 */   521,  302,  521,  521,  521,  521,  290,  521,  262,  521,
 /*  5100 */   521,  339,  521,  521,  521,  521,  521,  521,  521,  260,
 /*  5110 */   256,  258,  521,  521,  212,  222,  228,  231,  233,  235,
 /*  5120 */   225,  219,  244,  246,  254,  248,  250,  252,  521,  521,
 /*  5130 */   521,  521,  521,  338,  241,  237,  521,  521,  521,  521,
 /*  5140 */   521,  337,  285,  286,  287,  288,  289,  521,  521,  217,
 /*  5150 */   226,  229,  220,  223,  521,  243,  521,  521,  521,  521,
 /*  5160 */   521,  268,  521,  521,  521,  521,  521,  521,  521,  521,
 /*  5170 */   521,  521,  521,  213,  276,  281,  295,  210,  362,  291,
 /*  5180 */   521,  521,  272,  521,  521,  521,  521,  521,  521,  521,
 /*  5190 */   302,  521,  521,  521,  521,  290,  521,  262,  521,  521,
 /*  5200 */   521,  521,  521,  521,  521,  521,  521,  521,  260,  256,
 /*  5210 */   258,  521,  521,  212,  222,  228,  231,  233,  235,  225,
 /*  5220 */   219,  244,  246,  254,  248,  250,  252,  521,  521,  521,
 /*  5230 */   521,  521,  284,  241,  237,  521,  521,  521,  521,  521,
 /*  5240 */   283,  285,  286,  287,  288,  289,  521,  521,  217,  226,
 /*  5250 */   229,  220,  223,  521,  888,  521,  521,  521,  521,  521,
 /*  5260 */   268,  521,  521,  521,  521,  521,  521,  521,  521,  521,
 /*  5270 */   521,  521,  213,  276,  281,  295,  210,  351,  291,  521,
 /*  5280 */   521,  272,  521,  521,  521,  521,  521,  521,  521,  302,
 /*  5290 */   521,  521,  521,  521,  290,  521,  262,  521,  521,  521,
 /*  5300 */   521,  521,  260,  256,  258,  521,  521,  212,  222,  228,
 /*  5310 */   231,  233,  235,  225,  219,  244,  246,  254,  248,  250,
 /*  5320 */   252,   96,   97,   99,   98,  100,  521,  241,  237,  521,
 /*  5330 */   521,  284,  521,  521,  521,  521,  521,  521,  528,  283,
 /*  5340 */   285,  286,  287,  288,  289,  521,  521,  217,  226,  229,
 /*  5350 */   220,  223,  521,  521,  521,  521,  521,  521,  521,  268,
 /*  5360 */   521,  521,  521,  521,  521,  521,  102,  103,  521,  719,
 /*  5370 */   657,  213,  276,  281,  295,  210,  101,  291,  521,  521,
 /*  5380 */   272,  521,  521,  521,  521,  521,  521,  521,  302,  521,
 /*  5390 */   521,  521,  521,  290,  521,  262,  521,  521,  439,  521,
 /*  5400 */   521,  260,  256,  258,  521,  521,  212,  222,  228,  231,
 /*  5410 */   233,  235,  225,  219,  244,  246,  254,  248,  250,  252,
 /*  5420 */    96,   97,   99,   98,  100,  521,  241,  237,  521,  521,
 /*  5430 */   284,  521,  521,  521,  521,  521,  521,  535,  283,  285,
 /*  5440 */   286,  287,  288,  289,  521,  521,  217,  226,  229,  220,
 /*  5450 */   223,  521,  521,  521,  521,  521,  521,  521,  268,  521,
 /*  5460 */   521,  521,  521,  521,  521,  102,  103,  521,   19,  104,
 /*  5470 */   213,  276,  281,  295,  210,  101,  291,  521,  521,  272,
 /*  5480 */   521,  521,  521,  521,  521,  521,  521,  302,  521,  521,
 /*  5490 */   521,  521,  290,  521,  262,  521,  521,  521,  521,  521,
 /*  5500 */   260,  256,  258,  521,  521,  212,  222,  228,  231,  233,
 /*  5510 */   235,  225,  219,  244,  246,  254,  248,  250,  252,  521,
 /*  5520 */   521,  521,  538,  521,  521,  241,  237,  521,  521,  284,
 /*  5530 */   521,  521,  521,  521,  521,  521,  521,  283,  285,  286,
 /*  5540 */   287,  288,  289,  521,  521,  217,  226,  229,  220,  223,
 /*  5550 */   521,  521,  521,  521,  521,  521,  521,  268,  521,  521,
 /*  5560 */   521,  521,  521,  521,  521,  521,  521,  521,  521,  213,
 /*  5570 */   276,  281,  295,  210,  521,  291,  521,  521,  272,  473,
 /*  5580 */   521,  521,  521,  521,  521,  521,  302,  521,  521,  521,
 /*  5590 */   521,  290,  521,  262,  521,  521,  521,  521,  521,  521,
 /*  5600 */   256,  258,  521,  521,  212,  222,  228,  231,  233,  235,
 /*  5610 */   225,  219,  244,  246,  254,  248,  250,  252,  521,  521,
 /*  5620 */   521,  521,  521,  521,  241,  237,  521,  521,  284,  521,
 /*  5630 */   521,  521,  521,  521,  521,  521,  283,  285,  286,  287,
 /*  5640 */   288,  289,  521,  521,  217,  226,  229,  220,  223,  521,
 /*  5650 */   521,  521,  521,  521,  521,  521,  268,  521,  521,  521,
 /*  5660 */   521,  521,  521,  521,  521,  521,  521,  521,  213,  276,
 /*  5670 */   281,  295,  210,  298,  291,  521,  521,  272,  521,  521,
 /*  5680 */   521,  521,  521,  521,  521,  302,  521,  521,  521,  521,
 /*  5690 */   290,  521,  262,  521,  521,  212,  222,  228,  231,  233,
 /*  5700 */   235,  225,  219,  244,  246,  254,  248,  250,  252,  521,
 /*  5710 */   445,  406,  521,  521,  521,  241,  237,  521,  521,  521,
 /*  5720 */   521,  521,  521,  521,  397,  521,  521,  284,  521,  521,
 /*  5730 */   521,  521,  521,  521,  521,  283,  285,  286,  287,  288,
 /*  5740 */   289,  521,  521,  217,  226,  229,  220,  223,  521,  521,
 /*  5750 */   521,  521,  521,  521,  521,  268,  521,   92,  398,  399,
 /*  5760 */   400,  401,  402,  521,  434,  452,  453,  213,  276,  281,
 /*  5770 */   295,  210,  521,  291,  109,  521,  272,  521,   66,  521,
 /*  5780 */   521,  521,  521,  521,  302,  521,  521,  521,  521,  290,
 /*  5790 */   521,  262,  521,  521,  521,  521,  521,   45,   46,   47,
 /*  5800 */    48,   49,   50,   51,   52,   53,   54,   55,  521,  521,
 /*  5810 */   521,  521,  521,  521,  521,  521,  521,  521,  521,  526,
 /*  5820 */   521,  521,  521,  521,  521,  521,  284,  521,  521,  521,
 /*  5830 */   521,  521,  521,  521,  283,  285,  286,  287,  288,  289,
 /*  5840 */   521,  521,  217,  226,  229,  220,  223,  521,  521,  521,
 /*  5850 */   521,  521,  521,  521,  268,  521,   92,  521,  521,  521,
 /*  5860 */   521,  521,  521,  521,  521,  521,  213,  276,  281,  295,
 /*  5870 */   210,  318,  291,  583,   92,  272,  521,   66,  521,  521,
 /*  5880 */   521,  521,  521,  302,  521,  521,  521,  521,  290,  521,
 /*  5890 */   262,  142,  521,  521,  521,   66,   45,   46,   47,   48,
 /*  5900 */    49,   50,   51,   52,   53,   54,   55,  521,  521,  521,
 /*  5910 */   521,  521,  521,  521,   45,   46,   47,   48,   49,   50,
 /*  5920 */    51,   52,   53,   54,   55,  284,  521,  521,  521,  521,
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
 /*  6150 */   103,  268,  714,  622,  521,  616,  521,  521,  521,  101,
 /*  6160 */   521,  521,  521,  213,  276,  281,  295,  210,  521,  291,
 /*  6170 */   521,  521,  293,  521,  521,  521,  521,  521,  521,  521,
 /*  6180 */   302,  521,  521,  521,  521,  290,  521,  262,  521,  102,
 /*  6190 */   103,  521,  715,  622,  521,  616,  521,  521,  521,  101,
 /*  6200 */   521,   96,   97,   99,   98,  100,  521,  521,  521,  521,
 /*  6210 */   521,  521,  521,  521,  521,  521,  521,  521,  521,  521,
 /*  6220 */   521,  521,  338,  521,   96,   97,   99,   98,  100,  521,
 /*  6230 */   337,  285,  286,  287,  288,  289,   96,   97,   99,   98,
 /*  6240 */   100,   96,   97,   99,   98,  100,  102,  103,  521,  132,
 /*  6250 */   622,  521,  616,  521,  521,  521,  101,  521,   96,   97,
 /*  6260 */    99,   98,  100,  521,  521,  521,  521,  521,  521,  102,
 /*  6270 */   103,  521,  716,  578,  521,  521,  521,  521,  521,  101,
 /*  6280 */   521,  102,  103,  521,  721,  578,  102,  103,  521,   21,
 /*  6290 */   104,  101,  521,  521,  521,  521,  101,  521,  521,  521,
 /*  6300 */   521,  521,  521,  102,  103,  521,  720,  578,  521,  521,
 /*  6310 */   521,  521,  521,  101,
};
static YYCODETYPE yy_lookahead[] = {
 /*     0 */     7,    8,    9,   10,   11,  109,  110,  111,  112,  113,
 /*    10 */   114,   40,   19,  106,  107,  108,  109,  110,  111,  112,
 /*    20 */   113,  114,   55,   49,   31,   32,   33,   34,   35,  125,
 /*    30 */    37,   41,  118,   40,   41,  121,  122,  123,  124,  125,
 /*    40 */   136,   48,   49,   50,  130,  131,   53,   40,   55,   75,
 /*    50 */   136,   77,   59,   60,   61,   62,   63,   64,   65,   66,
 /*    60 */    67,   90,   69,   70,   71,   72,  121,   74,  123,  124,
 /*    70 */   125,   78,   79,   80,   81,  130,  131,   84,   79,   37,
 /*    80 */    38,  136,  169,   90,  171,  172,   49,   94,   95,   96,
 /*    90 */    97,   98,   99,  100,  101,  102,  103,  104,    7,    8,
 /*   100 */     9,   10,   11,   25,   26,   27,   28,   29,   30,   55,
 /*   110 */    19,  133,   75,   40,   77,   37,   38,  139,  140,   38,
 /*   120 */   142,   48,   31,   32,   33,   34,   35,   40,   37,   48,
 /*   130 */    76,   40,   41,   28,   29,   30,   49,   50,   38,   48,
 /*   140 */    49,   50,   37,   38,   53,   91,   55,   41,   48,    6,
 /*   150 */    59,   60,   61,   62,   63,   64,   65,   66,   67,   40,
 /*   160 */    69,   70,   71,   72,   37,   74,  123,  124,  125,   78,
 /*   170 */    79,   80,   81,   41,  131,   84,  167,  168,   51,  136,
 /*   180 */    48,   90,   53,   51,   41,   94,   95,   96,   97,   98,
 /*   190 */    99,  100,  101,  102,  103,  104,    7,    8,    9,   10,
 /*   200 */    11,  135,   40,  137,  138,  139,  140,  141,   19,   40,
 /*   210 */    48,    6,   85,   86,   87,   88,   89,   48,   37,   38,
 /*   220 */    31,   32,   33,   34,   35,  169,   37,  171,  172,   40,
 /*   230 */    41,  120,  138,  139,  140,  141,  125,   48,   49,   50,
 /*   240 */     6,   82,   53,  132,   55,   49,   41,  136,   59,   60,
 /*   250 */    61,   62,   63,   64,   65,   66,   67,   20,   69,   70,
 /*   260 */    71,   72,   37,   74,  123,   51,  125,   78,   79,   80,
 /*   270 */    81,  130,  131,   84,   37,   40,   51,  136,  169,   90,
 /*   280 */   171,  172,    6,   94,   95,   96,   97,   98,   99,  100,
 /*   290 */   101,  102,  103,  104,    7,    8,    9,   10,   11,   85,
 /*   300 */    86,   87,   88,   89,  139,  140,   19,  142,   37,   38,
 /*   310 */    85,   86,   87,   88,   89,   48,   82,   41,   31,   32,
 /*   320 */    33,   34,   35,  133,   37,   90,    0,   40,   41,  139,
 /*   330 */   140,   41,  142,  125,   55,   48,   49,   50,   48,  131,
 /*   340 */    53,   51,   55,   40,  136,    6,   59,   60,   61,   62,
 /*   350 */    63,   64,   65,   66,   67,   43,   69,   70,   71,   72,
 /*   360 */    48,   74,   41,  166,   48,   78,   79,   80,   81,   48,
 /*   370 */    91,   84,  175,  176,  177,  178,  179,   90,  180,  181,
 /*   380 */    41,   94,   95,   96,   97,   98,   99,  100,  101,  102,
 /*   390 */   103,  104,    7,    8,    9,   10,   11,  166,   43,  169,
 /*   400 */    45,    6,  172,   48,   19,  174,  175,  176,  177,  178,
 /*   410 */   179,   38,  169,   91,   41,  172,   31,   32,   33,   34,
 /*   420 */    35,  133,   37,   40,   55,   40,   41,  139,  140,  169,
 /*   430 */   142,   36,  172,   48,   49,   50,   53,   43,   53,   45,
 /*   440 */    55,   41,   48,    6,   59,   60,   61,   62,   63,   64,
 /*   450 */    65,   66,   67,    6,   69,   70,   71,   72,  163,   74,
 /*   460 */    91,  166,   55,   78,   79,   80,   81,   38,    6,   84,
 /*   470 */    41,  176,  177,  178,  179,   90,  180,  181,   41,   94,
 /*   480 */    95,   96,   97,   98,   99,  100,  101,  102,  103,  104,
 /*   490 */     7,    8,    9,   10,   11,  166,   49,  180,  181,  170,
 /*   500 */   180,  181,   19,   41,    6,  176,  177,  178,  179,    6,
 /*   510 */   180,  181,  180,  181,   31,   32,   33,   34,   35,  133,
 /*   520 */    37,  180,  181,   40,   41,  139,  140,  117,  142,  119,
 /*   530 */   125,   48,   49,   50,  180,  181,   53,  132,   55,   41,
 /*   540 */     6,  136,   59,   60,   61,   62,   63,   64,   65,   66,
 /*   550 */    67,   48,   69,   70,   71,   72,  163,   74,  115,  166,
 /*   560 */     6,   78,   79,   80,   81,   38,    6,   84,   41,  176,
 /*   570 */   177,  178,  179,   90,    6,   41,   55,   94,   95,   96,
 /*   580 */    97,   98,   99,  100,  101,  102,  103,  104,    7,    8,
 /*   590 */     9,   10,   11,  166,    6,   41,    6,  170,  180,  181,
 /*   600 */    19,   41,    6,  176,  177,  178,  179,   40,    6,   41,
 /*   610 */   180,  181,   31,   32,   33,   34,   35,   50,   37,   40,
 /*   620 */    35,   40,   41,   40,   36,   40,   36,   48,    6,   48,
 /*   630 */    49,   50,  164,  165,   53,   55,   55,   41,   36,   38,
 /*   640 */    59,   60,   61,   62,   63,   64,   65,   66,   67,   48,
 /*   650 */    69,   70,   71,   72,   38,   74,  166,   40,   36,   78,
 /*   660 */    79,   80,   81,  173,   48,   84,  176,  177,  178,  179,
 /*   670 */    38,   90,  117,   41,  119,   94,   95,   96,   97,   98,
 /*   680 */    99,  100,  101,  102,  103,  104,    7,    8,    9,   10,
 /*   690 */    11,  166,   40,  136,  125,  170,    6,  125,   19,   35,
 /*   700 */    48,  176,  177,  178,  179,  136,   45,    6,  136,   48,
 /*   710 */    31,   32,   33,   34,   35,   49,   37,    6,    6,   40,
 /*   720 */    41,  128,  129,   41,    6,  116,   36,   48,   49,   50,
 /*   730 */    48,    6,   53,   48,   55,   40,  116,   36,   59,   60,
 /*   740 */    61,   62,   63,   64,   65,   66,   67,   36,   69,   70,
 /*   750 */    71,   72,  163,   74,   36,  166,   53,   78,   79,   80,
 /*   760 */    81,   36,  117,   84,  119,  176,  177,  178,  179,   90,
 /*   770 */    58,   40,  165,   94,   95,   96,   97,   98,   99,  100,
 /*   780 */   101,  102,  103,  104,    7,    8,    9,   10,   11,  135,
 /*   790 */     6,  137,  138,  139,  140,  141,   19,  135,    6,  137,
 /*   800 */   138,  139,  140,  141,    6,    6,   51,   40,   31,   32,
 /*   810 */    33,   34,   35,   41,   37,  126,   55,   40,   41,   43,
 /*   820 */    36,   90,   40,   40,   48,   48,   49,   50,   36,   40,
 /*   830 */    53,   53,   55,  126,   36,   36,   59,   60,   61,   62,
 /*   840 */    63,   64,   65,   66,   67,   51,   69,   70,   71,   72,
 /*   850 */   163,   74,   51,  166,   41,   78,   79,   80,   81,  126,
 /*   860 */    40,   84,   76,  176,  177,  178,  179,   90,  116,   76,
 /*   870 */    15,   94,   95,   96,   97,   98,   99,  100,  101,  102,
 /*   880 */   103,  104,    7,    8,    9,   10,   11,  166,   76,  127,
 /*   890 */   116,  170,  182,   40,   19,  126,  116,  176,  177,  178,
 /*   900 */   179,   41,  127,    6,   40,  126,   31,   32,   33,   34,
 /*   910 */    35,  181,   37,   51,   49,   40,   41,   41,   48,   58,
 /*   920 */    41,  115,   44,   48,   49,   50,  129,   76,   53,   55,
 /*   930 */    55,   41,   40,  126,   59,   60,   61,   62,   63,   64,
 /*   940 */    65,   66,   67,   41,   69,   70,   71,   72,  163,   74,
 /*   950 */    41,  166,   20,   78,   79,   80,   81,   41,   40,   84,
 /*   960 */    49,  176,  177,  178,  179,   90,   51,   40,   40,   94,
 /*   970 */    95,   96,   97,   98,   99,  100,  101,  102,  103,  104,
 /*   980 */     7,    8,    9,   10,   11,  166,  127,   48,   40,  170,
 /*   990 */    48,   40,   19,  126,   36,  176,  177,  178,  179,  127,
 /*  1000 */    41,   41,   41,   41,   31,   32,   33,   34,   35,  182,
 /*  1010 */    37,   41,  116,   40,   41,  116,   48,  126,   41,   20,
 /*  1020 */    49,   48,   49,   50,   40,  169,   53,  169,   55,   40,
 /*  1030 */    51,   49,   59,   60,   61,   62,   63,   64,   65,   66,
 /*  1040 */    67,   41,   69,   70,   71,   72,  169,   74,  166,  169,
 /*  1050 */    20,   78,   79,   80,   81,   40,   55,   84,  176,  177,
 /*  1060 */   178,  179,  119,   90,   55,   49,   55,   94,   95,   96,
 /*  1070 */    97,   98,   99,  100,  101,  102,  103,  104,    7,    8,
 /*  1080 */     9,   10,   11,  166,   58,   40,   49,  170,  169,   79,
 /*  1090 */    19,  116,   44,  176,  177,  178,  179,   40,  126,  168,
 /*  1100 */    40,   40,   31,   32,   33,   34,   35,   48,   37,  126,
 /*  1110 */    55,   40,   41,   49,   73,   40,   48,   55,  116,   48,
 /*  1120 */    49,   50,   55,   40,   53,   48,   55,   48,   41,   48,
 /*  1130 */    59,   60,   61,   62,   63,   64,   65,   66,   67,   51,
 /*  1140 */    69,   70,   71,   72,   40,   74,  166,   48,   40,   78,
 /*  1150 */    79,   80,   81,   73,   49,   84,  176,  177,  178,  179,
 /*  1160 */    41,   90,   41,   40,   40,   94,   95,   96,   97,   98,
 /*  1170 */    99,  100,  101,  102,  103,  104,    7,    8,    9,   10,
 /*  1180 */    11,  135,   76,  137,  138,  139,  140,  141,   19,  135,
 /*  1190 */    41,  137,  138,  139,  140,  141,  189,  189,  189,  189,
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
 /*  1570 */     9,   10,   11,  166,  189,  189,  189,  170,  189,  189,
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
 /*  1760 */   101,  102,  103,  104,    7,    8,    9,   10,   11,  135,
 /*  1770 */   189,  137,  138,  139,  140,  141,   19,  135,  189,  137,
 /*  1780 */   138,  139,  140,  141,  189,  189,  189,  189,   31,   32,
 /*  1790 */    33,   34,   35,  189,   37,  189,  189,   40,   41,  189,
 /*  1800 */   189,  189,  189,  189,  189,   48,   49,   50,  189,  189,
 /*  1810 */    53,  189,   55,  189,  189,  189,   59,   60,   61,   62,
 /*  1820 */    63,   64,   65,   66,   67,  189,   69,   70,   71,   72,
 /*  1830 */   189,   74,  166,  189,  189,   78,   79,   80,   81,  189,
 /*  1840 */   189,   84,  176,  177,  178,  179,  189,   90,  189,  189,
 /*  1850 */   189,   94,   95,   96,   97,   98,   99,  100,  101,  102,
 /*  1860 */   103,  104,    7,    8,    9,   10,   11,  135,  189,  137,
 /*  1870 */   138,  139,  140,  141,   19,  189,  189,  189,  189,  189,
 /*  1880 */   189,  189,  189,  189,  189,  189,   31,   32,   33,   34,
 /*  1890 */    35,  189,   37,  189,  189,   40,   41,  189,  189,  189,
 /*  1900 */   189,  189,  189,   48,   49,   50,  189,  189,   53,  189,
 /*  1910 */    55,  189,  189,  189,   59,   60,   61,   62,   63,   64,
 /*  1920 */    65,   66,   67,  189,   69,   70,   71,   72,  189,   74,
 /*  1930 */   166,  189,  189,   78,   79,   80,   81,  189,  139,   84,
 /*  1940 */   176,  177,  178,  179,  189,   90,  189,  189,  189,   94,
 /*  1950 */    95,   96,   97,   98,   99,  100,  101,  102,  103,  104,
 /*  1960 */     7,    8,    9,   10,   11,  166,  189,  189,  189,  189,
 /*  1970 */   189,  189,   19,  189,  189,  176,  177,  178,  179,  189,
 /*  1980 */   189,  189,  189,  189,   31,   32,   33,   34,   35,  189,
 /*  1990 */    37,  189,  189,   40,   41,  189,  189,  189,  189,  189,
 /*  2000 */   189,   48,   49,   50,  189,  189,   53,  189,   55,  189,
 /*  2010 */   189,  189,   59,   60,   61,   62,   63,   64,   65,   66,
 /*  2020 */    67,  189,   69,   70,   71,   72,  189,   74,  166,  189,
 /*  2030 */   189,   78,   79,   80,   81,  189,  189,   84,  176,  177,
 /*  2040 */   178,  179,  189,   90,  189,  189,  189,   94,   95,   96,
 /*  2050 */    97,   98,   99,  100,  101,  102,  103,  104,    7,    8,
 /*  2060 */     9,   10,   11,  166,  189,  189,  189,  189,  189,  189,
 /*  2070 */    19,  189,  189,  176,  177,  178,  179,  189,  189,  189,
 /*  2080 */   189,  189,   31,   32,   33,   34,   35,  189,   37,  189,
 /*  2090 */   189,   40,   41,  189,  189,  189,  189,  189,  189,   48,
 /*  2100 */    49,   50,  189,  189,   53,  189,   55,  189,  189,  189,
 /*  2110 */    59,   60,   61,   62,   63,   64,   65,   66,   67,  189,
 /*  2120 */    69,   70,   71,   72,  189,   74,  166,  189,  189,   78,
 /*  2130 */    79,   80,   81,  189,  189,   84,  176,  177,  178,  179,
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
 /*  2260 */   189,  189,  189,  189,  189,  189,   19,  189,  189,  176,
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
 /*  3450 */   161,  162,  113,  114,  189,  166,  189,  189,  189,  189,
 /*  3460 */   189,  189,  189,  189,  189,  176,  177,  178,  179,  189,
 /*  3470 */   189,  189,  189,  134,  189,    1,    2,    3,    4,    5,
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
 /*  3650 */   161,  162,  113,  114,  189,  166,  189,  189,  189,  189,
 /*  3660 */   189,  189,  189,  189,  189,  176,  177,  178,  179,  189,
 /*  3670 */   189,  189,  189,  134,  189,    1,    2,    3,    4,    5,
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
 /*  3820 */    46,   47,  189,  134,  189,  189,   52,  189,  189,  189,
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
 /*  3950 */   161,  162,  113,  114,  189,  166,  189,  189,  189,  166,
 /*  3960 */   189,  189,  189,  189,  189,  176,  177,  178,  179,  176,
 /*  3970 */   177,  178,  179,  134,  189,  189,  183,  189,  189,  186,
 /*  3980 */   187,  188,  143,  144,  145,  146,  147,  148,  149,  150,
 /*  3990 */   151,  152,  153,  154,  155,  156,  157,  158,  159,  160,
 /*  4000 */   161,  162,  113,  114,  189,  166,  189,  189,  189,  189,
 /*  4010 */   189,  189,  189,  189,  189,  176,  177,  178,  179,  189,
 /*  4020 */   189,  189,  189,  134,  189,    1,    2,    3,    4,    5,
 /*  4030 */   189,  189,  143,  144,  145,  146,  147,  148,  149,  150,
 /*  4040 */   151,  152,  153,  154,  155,  156,  157,  158,  159,  160,
 /*  4050 */   161,  162,  113,  114,  189,  166,  189,  189,  189,  189,
 /*  4060 */   189,  189,  189,  189,  189,  176,  177,  178,  179,  189,
 /*  4070 */    46,   47,  189,  134,  189,  189,  189,  189,   54,  189,
 /*  4080 */    56,  189,  143,  144,  145,  146,  147,  148,  149,  150,
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
 /*  4360 */    10,   11,    0,  189,  189,  176,  177,  178,  179,   19,
 /*  4370 */   189,  189,  189,  189,  189,  189,  189,  189,  189,  189,
 /*  4380 */   189,   31,   32,   33,   34,   35,   36,   37,  189,  189,
 /*  4390 */    40,  189,  189,  189,  189,  189,  189,  189,   48,  189,
 /*  4400 */   189,   39,  189,   53,   42,   55,   44,  189,   46,   47,
 /*  4410 */   189,  189,   50,  189,  189,  189,   12,   13,   14,  189,
 /*  4420 */   189,   17,   18,   19,   20,   21,   22,   23,   24,   25,
 /*  4430 */    26,   27,   28,   29,   30,  189,  189,  189,  189,  189,
 /*  4440 */    90,   37,   38,  189,  189,   41,  189,  189,   98,   99,
 /*  4450 */   100,  101,  102,  103,  189,  189,    7,    8,    9,   10,
 /*  4460 */    11,  122,  123,  124,  125,  189,  104,  189,   19,  130,
 /*  4470 */   131,  189,  189,  189,  189,  136,  189,  189,  189,  189,
 /*  4480 */    31,   32,   33,   34,   35,   36,   37,  189,  166,   40,
 /*  4490 */   189,  189,  189,  189,  189,  189,  189,   48,  176,  177,
 /*  4500 */   178,  179,   53,  189,   55,  189,  184,  185,  189,  189,
 /*  4510 */   189,  189,  189,  189,  189,   12,   13,   14,  189,  189,
 /*  4520 */    17,   18,   19,   20,   21,   22,   23,   24,   25,   26,
 /*  4530 */    27,   28,   29,   30,  189,  189,  189,  189,  189,   90,
 /*  4540 */    37,   38,  189,  189,  189,  189,  189,   98,   99,  100,
 /*  4550 */   101,  102,  103,  166,  189,    7,    8,    9,   10,   11,
 /*  4560 */   189,   58,  189,  176,  177,  178,  179,   19,  189,  189,
 /*  4570 */   189,  184,  185,  189,  189,  189,  189,  189,  189,   31,
 /*  4580 */    32,   33,   34,   35,   36,   37,  189,  189,   40,  189,
 /*  4590 */   189,  189,  189,  189,  166,  189,   48,  189,  189,  189,
 /*  4600 */   189,   53,  189,   55,  176,  177,  178,  179,  189,   12,
 /*  4610 */    13,   14,  184,  185,   17,   18,   19,   20,   21,   22,
 /*  4620 */    23,   24,   25,   26,   27,   28,   29,   30,    1,    2,
 /*  4630 */     3,    4,    5,  189,   37,   38,  189,  189,   90,  189,
 /*  4640 */   189,  189,  189,  189,  189,   48,   98,   99,  100,  101,
 /*  4650 */   102,  103,  166,  189,    7,    8,    9,   10,   11,  189,
 /*  4660 */   189,  189,  176,  177,  178,  179,   19,  189,  189,  189,
 /*  4670 */   184,  185,  189,   46,   47,  189,   49,   50,   31,   32,
 /*  4680 */    33,   34,   35,   56,   37,  189,  189,   40,  189,  189,
 /*  4690 */   189,  189,  189,  189,  189,   48,  189,  189,  189,  189,
 /*  4700 */    53,  189,   55,  189,  189,   58,  189,  189,   12,   13,
 /*  4710 */    14,  189,  189,   17,   18,   19,   20,   21,   22,   23,
 /*  4720 */    24,   25,   26,   27,   28,   29,   30,  189,  189,  189,
 /*  4730 */   189,  189,  189,   37,   38,  189,  189,   90,  189,  189,
 /*  4740 */   189,  189,  189,  189,   48,   98,   99,  100,  101,  102,
 /*  4750 */   103,  166,  189,    7,    8,    9,   10,   11,  189,  189,
 /*  4760 */   189,  176,  177,  178,  179,   19,  189,  189,  189,  184,
 /*  4770 */   185,  189,  189,  189,  189,  189,  189,   31,   32,   33,
 /*  4780 */    34,   35,   36,   37,  189,  166,   40,  189,  189,  189,
 /*  4790 */   189,  189,  189,  189,   48,  176,  177,  178,  179,   53,
 /*  4800 */   189,   55,  189,  184,  185,  189,  189,  189,  189,  189,
 /*  4810 */   189,  189,   12,   13,   14,  189,  189,   17,   18,   19,
 /*  4820 */    20,   21,   22,   23,   24,   25,   26,   27,   28,   29,
 /*  4830 */    30,  189,  189,  189,  189,  189,   90,   37,   38,  189,
 /*  4840 */   189,  189,  189,  189,   98,   99,  100,  101,  102,  103,
 /*  4850 */   166,  189,    7,    8,    9,   10,   11,  189,   58,  189,
 /*  4860 */   176,  177,  178,  179,   19,  189,  189,  189,  184,  185,
 /*  4870 */   189,  189,  189,  189,  189,  189,   31,   32,   33,   34,
 /*  4880 */    35,   36,   37,  189,  166,   40,  189,  189,  189,  189,
 /*  4890 */   189,  189,  189,   48,  176,  177,  178,  179,   53,  189,
 /*  4900 */    55,  189,  184,  185,  189,  189,   12,   13,   14,  189,
 /*  4910 */   189,   17,   18,   19,   20,   21,   22,   23,   24,   25,
 /*  4920 */    26,   27,   28,   29,   30,    1,    2,    3,    4,    5,
 /*  4930 */    36,   37,   38,  189,  189,   90,  189,  189,  189,  189,
 /*  4940 */   189,  189,  189,   98,   99,  100,  101,  102,  103,  166,
 /*  4950 */   189,    7,    8,    9,   10,   11,  189,  189,  189,  176,
 /*  4960 */   177,  178,  179,   19,  189,  189,  189,  184,  185,  189,
 /*  4970 */    46,   47,  189,   49,   50,   31,   32,   33,   34,   35,
 /*  4980 */    56,   37,  189,  189,   40,  189,  189,  189,  189,  189,
 /*  4990 */   189,  189,   48,  189,  189,  166,  189,   53,  189,   55,
 /*  5000 */   189,  189,   58,  189,  189,  176,  177,  178,  179,  189,
 /*  5010 */    12,   13,   14,  184,  185,   17,   18,   19,   20,   21,
 /*  5020 */    22,   23,   24,   25,   26,   27,   28,   29,   30,  189,
 /*  5030 */   189,  189,  189,  189,   90,   37,   38,  189,  189,   41,
 /*  5040 */   189,  189,   98,   99,  100,  101,  102,  103,  166,  189,
 /*  5050 */     7,    8,    9,   10,   11,  189,  189,  189,  176,  177,
 /*  5060 */   178,  179,   19,  189,  189,  189,  189,  185,  189,  189,
 /*  5070 */   189,  189,  189,  189,   31,   32,   33,   34,   35,  189,
 /*  5080 */    37,  189,  189,   40,  189,  189,  189,  189,  189,  189,
 /*  5090 */   189,   48,  189,  189,  189,  189,   53,  189,   55,  189,
 /*  5100 */   189,   58,  189,  189,  189,  189,  189,  189,  189,   12,
 /*  5110 */    13,   14,  189,  189,   17,   18,   19,   20,   21,   22,
 /*  5120 */    23,   24,   25,   26,   27,   28,   29,   30,  189,  189,
 /*  5130 */   189,  189,  189,   90,   37,   38,  189,  189,  189,  189,
 /*  5140 */   189,   98,   99,  100,  101,  102,  103,  189,  189,    7,
 /*  5150 */     8,    9,   10,   11,  189,   58,  189,  189,  189,  189,
 /*  5160 */   189,   19,  189,  189,  189,  189,  189,  189,  189,  189,
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
 /*  5300 */   189,  189,   12,   13,   14,  189,  189,   17,   18,   19,
 /*  5310 */    20,   21,   22,   23,   24,   25,   26,   27,   28,   29,
 /*  5320 */    30,    1,    2,    3,    4,    5,  189,   37,   38,  189,
 /*  5330 */   189,   90,  189,  189,  189,  189,  189,  189,   48,   98,
 /*  5340 */    99,  100,  101,  102,  103,  189,  189,    7,    8,    9,
 /*  5350 */    10,   11,  189,  189,  189,  189,  189,  189,  189,   19,
 /*  5360 */   189,  189,  189,  189,  189,  189,   46,   47,  189,   49,
 /*  5370 */    50,   31,   32,   33,   34,   35,   56,   37,  189,  189,
 /*  5380 */    40,  189,  189,  189,  189,  189,  189,  189,   48,  189,
 /*  5390 */   189,  189,  189,   53,  189,   55,  189,  189,   58,  189,
 /*  5400 */   189,   12,   13,   14,  189,  189,   17,   18,   19,   20,
 /*  5410 */    21,   22,   23,   24,   25,   26,   27,   28,   29,   30,
 /*  5420 */     1,    2,    3,    4,    5,  189,   37,   38,  189,  189,
 /*  5430 */    90,  189,  189,  189,  189,  189,  189,   48,   98,   99,
 /*  5440 */   100,  101,  102,  103,  189,  189,    7,    8,    9,   10,
 /*  5450 */    11,  189,  189,  189,  189,  189,  189,  189,   19,  189,
 /*  5460 */   189,  189,  189,  189,  189,   46,   47,  189,   49,   50,
 /*  5470 */    31,   32,   33,   34,   35,   56,   37,  189,  189,   40,
 /*  5480 */   189,  189,  189,  189,  189,  189,  189,   48,  189,  189,
 /*  5490 */   189,  189,   53,  189,   55,  189,  189,  189,  189,  189,
 /*  5500 */    12,   13,   14,  189,  189,   17,   18,   19,   20,   21,
 /*  5510 */    22,   23,   24,   25,   26,   27,   28,   29,   30,  189,
 /*  5520 */   189,  189,   83,  189,  189,   37,   38,  189,  189,   90,
 /*  5530 */   189,  189,  189,  189,  189,  189,  189,   98,   99,  100,
 /*  5540 */   101,  102,  103,  189,  189,    7,    8,    9,   10,   11,
 /*  5550 */   189,  189,  189,  189,  189,  189,  189,   19,  189,  189,
 /*  5560 */   189,  189,  189,  189,  189,  189,  189,  189,  189,   31,
 /*  5570 */    32,   33,   34,   35,  189,   37,  189,  189,   40,   41,
 /*  5580 */   189,  189,  189,  189,  189,  189,   48,  189,  189,  189,
 /*  5590 */   189,   53,  189,   55,  189,  189,  189,  189,  189,  189,
 /*  5600 */    13,   14,  189,  189,   17,   18,   19,   20,   21,   22,
 /*  5610 */    23,   24,   25,   26,   27,   28,   29,   30,  189,  189,
 /*  5620 */   189,  189,  189,  189,   37,   38,  189,  189,   90,  189,
 /*  5630 */   189,  189,  189,  189,  189,  189,   98,   99,  100,  101,
 /*  5640 */   102,  103,  189,  189,    7,    8,    9,   10,   11,  189,
 /*  5650 */   189,  189,  189,  189,  189,  189,   19,  189,  189,  189,
 /*  5660 */   189,  189,  189,  189,  189,  189,  189,  189,   31,   32,
 /*  5670 */    33,   34,   35,   36,   37,  189,  189,   40,  189,  189,
 /*  5680 */   189,  189,  189,  189,  189,   48,  189,  189,  189,  189,
 /*  5690 */    53,  189,   55,  189,  189,   17,   18,   19,   20,   21,
 /*  5700 */    22,   23,   24,   25,   26,   27,   28,   29,   30,  189,
 /*  5710 */    37,   38,  189,  189,  189,   37,   38,  189,  189,  189,
 /*  5720 */   189,  189,  189,  189,   51,  189,  189,   90,  189,  189,
 /*  5730 */   189,  189,  189,  189,  189,   98,   99,  100,  101,  102,
 /*  5740 */   103,  189,  189,    7,    8,    9,   10,   11,  189,  189,
 /*  5750 */   189,  189,  189,  189,  189,   19,  189,   19,   85,   86,
 /*  5760 */    87,   88,   89,  189,   91,   92,   93,   31,   32,   33,
 /*  5770 */    34,   35,  189,   37,   36,  189,   40,  189,   40,  189,
 /*  5780 */   189,  189,  189,  189,   48,  189,  189,  189,  189,   53,
 /*  5790 */   189,   55,  189,  189,  189,  189,  189,   59,   60,   61,
 /*  5800 */    62,   63,   64,   65,   66,   67,   68,   69,  189,  189,
 /*  5810 */   189,  189,  189,  189,  189,  189,  189,  189,  189,   83,
 /*  5820 */   189,  189,  189,  189,  189,  189,   90,  189,  189,  189,
 /*  5830 */   189,  189,  189,  189,   98,   99,  100,  101,  102,  103,
 /*  5840 */   189,  189,    7,    8,    9,   10,   11,  189,  189,  189,
 /*  5850 */   189,  189,  189,  189,   19,  189,   19,  189,  189,  189,
 /*  5860 */   189,  189,  189,  189,  189,  189,   31,   32,   33,   34,
 /*  5870 */    35,   36,   37,   36,   19,   40,  189,   40,  189,  189,
 /*  5880 */   189,  189,  189,   48,  189,  189,  189,  189,   53,  189,
 /*  5890 */    55,   36,  189,  189,  189,   40,   59,   60,   61,   62,
 /*  5900 */    63,   64,   65,   66,   67,   68,   69,  189,  189,  189,
 /*  5910 */   189,  189,  189,  189,   59,   60,   61,   62,   63,   64,
 /*  5920 */    65,   66,   67,   68,   69,   90,  189,  189,  189,  189,
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
 /*  6260 */     3,    4,    5,  189,  189,  189,  189,  189,  189,   46,
 /*  6270 */    47,  189,   49,   50,  189,  189,  189,  189,  189,   56,
 /*  6280 */   189,   46,   47,  189,   49,   50,   46,   47,  189,   49,
 /*  6290 */    50,   56,  189,  189,  189,  189,   56,  189,  189,  189,
 /*  6300 */   189,  189,  189,   46,   47,  189,   49,   50,  189,  189,
 /*  6310 */   189,  189,  189,   56,
};
#define YY_SHIFT_USE_DFLT (-34)
static short yy_shift_ofst[] = {
 /*     0 */  3201,  326, 4362,  -34,  -34,  -34,  -34,  -34,  -34,  767,
 /*    10 */   106,  -34,  820,  312,  -34, 1083,  942,  -34, 5419,  -34,
 /*    20 */  6240,  -34,  -34, 4024, 1104,  -33, 5936,  373, 3247,  890,
 /*    30 */   -34,  -34,  855, 6034,  -34,  664,  -34,  -34,  -34, 1108,
 /*    40 */   237,  -34,  861, 1030,  -34,  -34,  -34,  -34,  -34,  -34,
 /*    50 */   -34,  -34,  -34,  -34,  -34,  -34,  -34,  -34,  690,  632,
 /*    60 */  3247, 1000,  -34,  -34, 5954,  -34,  915, 6019,  -34,  -34,
 /*    70 */   -34,  -34,  -34,  -34,  -34,  -34,  322,  129,  -34,  -34,
 /*    80 */   585, 1088, 6019,  -34,  695,  794, 6019,  -34,  951,  979,
 /*    90 */  6019,  -34,  984,  999,  -34,  -34,  -34,  -34,  -34,  -34,
 /*   100 */   -34,  -34,  -34,  -34, 4124, 3874,  617,  580, 5738,  429,
 /*   110 */  3247,  400,  -34,  -34,  588,  527, 3247, 1121,  -34,  -34,
 /*   120 */   -34,  -34, 1123,  355,  -34,  783,  661,  -34,  789,  503,
 /*   130 */   -34, 6200,  -34, 1037,  -34, 6103, 4924,  -34, 3674, 1015,
 /*   140 */  1001, 5855,   81, 1757,  -34,  189,  -34,  -34,  -34,  -34,
 /*   150 */   -34,  -34,  -34,  -34,  -34,  -34,  -34,  -34,  -34,  -34,
 /*   160 */   -34,  -34,  -34,  -34,  -34,  -34,  -34,  -34,  -34,  -34,
 /*   170 */  1119,  -34, 1087,  -34, 6033, 1077,  679, 1463, 1041, 1059,
 /*   180 */  3031,   -7,  -34, 6033,  267,  -26,  -34,   37,  -34,  -34,
 /*   190 */  6019,  812, 3129, 3129,  870,  385, 2933,  -34, 6033,  939,
 /*   200 */   483, 1169,  -34,  968,  581, 1855, 1010, 6033, 1149,  -34,
 /*   210 */  6033,  181, 6033,  853,  -34,   42, 6033, 6033, 5488, 6033,
 /*   220 */  6033, 5488, 6033, 6033, 5488, 6033, 6033, 5488, 6033, 6033,
 /*   230 */  5488, 6033,   78, 6033,   78, 6033,   78,  169, 1009, 4845,
 /*   240 */  5488, 6033, 5097,  -34, 6033,  105, 6033,  105, 6033,  181,
 /*   250 */  6033,  181, 6033,  181, 6033,  105, 6033, 5678, 6033, 5678,
 /*   260 */  6033, 5587, 3227, 4894,  -34,  958, 6033,  181,  927,  932,
 /*   270 */  6033,   78,  279,  383,  874, 5241,  864,  897,  853,  -34,
 /*   280 */   -34, 6033,  181,  -34,  -34,  -34,  -34,  -34,  -34,  -34,
 /*   290 */   -34, 5043, 5488,   54, 4746,  579,  407, 5637,  -34,  395,
 /*   300 */   -34, 6033,    7, 1105,  369, 4548,  590,  -34,  -34,  -34,
 /*   310 */   -34,  -34,  -34,  652, 1062, 5835,  622,  -34,  -34, 1060,
 /*   320 */  1064, 1067, 4449,  701,  -34,  -34,  -34,  119,  196,  521,
 /*   330 */  4350,  -34,  718,  -34,  725,  -34,  -34,  786,  793,  -34,
 /*   340 */   712,  -34, 6132,  -34,  851, 6033,  -34,  -34,  -34,  784,
 /*   350 */   -34,  -34,  -34,  792,  -34,  -34,  731, 1016, 1011, 5142,
 /*   360 */   798,  -34,  -34,  666,  761, 5934,  799,  -34,  -34,   78,
 /*   370 */    78,   78,   78, 4800,  -34,   73,  -34,  -29,  911,  -34,
 /*   380 */   865,  -34,   78, 5488,  782,  234, 5736, 4696, 3129, 2345,
 /*   390 */   -34, 1075,  143,  -34, 1075,  -34, 5673,  -34,  -34,  -34,
 /*   400 */   -34,  -34,  -34, 6033,  -34, 5488,  162, 3521, 6033,  -34,
 /*   410 */  4944,  214, 6033,  -34, 1026,  -34, 5196,  127, 6033,  -34,
 /*   420 */   -34, 6033,  -34,  -34,  -34,  235,  982,  214, 6033,  -34,
 /*   430 */   971,  214, 6033,  -34,  989,  225, 6033,  -34, 5340,  214,
 /*   440 */  6033,  -34,  127, 6033,  -34, 4647,  214, 6033,  -34,  127,
 /*   450 */  6033,  -34,  -34,  -34,  -34, 6033,  205,  -34, 6033,  -34,
 /*   460 */  5488,  -34,  977,  -34,  962,  -34,  961,  -34,  959,  -34,
 /*   470 */  5538, 4998,  -34,  -34,  928,  271,  918,  909,  -34, 6033,
 /*   480 */  4503,  902,  -34, 6033, 4404,  -34,  583,  276,  -34,  583,
 /*   490 */   -34,  862, 6019,  -34,  -34,  583,  339,  -34,  583,  437,
 /*   500 */   -34,  583,  462,  -34,  583,  498,  -34,  583,  534,  -34,
 /*   510 */   583,  554,  -34,  583,  560,  -34,  583,  568,  -34,  583,
 /*   520 */   596,  -34, 5488,  -34,  -34,  -34, 6033, 5290, 3129, 2443,
 /*   530 */   -34,  303,  159, 5439, 5389, 3129, 2247,  -34, 6033, 4597,
 /*   540 */  3129,  973,  -34,   -1, 6033,  -10,  -34,  -34,  -34, 1106,
 /*   550 */  3129, 3129,  -34,  -34, 1080, 1081, 2541, 2149,  -34,  -34,
 /*   560 */  3247, 1079, 2737,  -34,  287,  -34,  602,  100,  777,  -34,
 /*   570 */  1071,  -34, 3247, 1068, 1561,  -34, 1659,  -34, 4124, 3574,
 /*   580 */  1061, 1055, 5837,  601, 1365,  -34, 2051,  -34, 3247, 1099,
 /*   590 */    91,  -34, 2835,  -34,  711,  616, 1267,  -34,  875,  -34,
 /*   600 */  3247,  316, 1953,  -34, 2639,  -34, 4274, 6235,  -34,  -34,
 /*   610 */  3774,  703,  755, 6019,  772,  -34,  778,  801, 6019,  813,
 /*   620 */   -34,  -34, 3774, 3374,  132,  -34, 6019,  321,  -34,  860,
 /*   630 */   -34,   87,  876,  447,  879,  567,  -34,  -34,  892,  -34,
 /*   640 */   -34,  916,  -34, 3474,  290,  -34, 6019,  682,  -34,  960,
 /*   650 */   -34,  970,  -34,  -34, 6143, 5320, 4627, 4124, 6257,  -34,
 /*   660 */  6223,  789,  -34,  -34,  -34,  789,  503,  -34, 1048, 1057,
 /*   670 */   394,  -34, 1124,  685,  -34,  789,  503,  -34,  878,  948,
 /*   680 */   776,  -34, 1045,  685,  -34,  -34,
};
#define YY_REDUCE_USE_DFLT (-105)
static short yy_reduce_ofst[] = {
 /*     0 */   -93, -105, -104, -105, -105, -105, -105, -105, -105, -105,
 /*    10 */  -105, -105, -105,  443, -105, -105,  806, -105,  111, -105,
 /*    20 */   405, -105, -105,  557, -105, -105,  288, -105, 1642, -105,
 /*    30 */  -105, -105, -105,   94, -105, -105, -105, -105, -105, -105,
 /*    40 */  -105, -105, -105, -105, -105, -105, -105, -105, -105, -105,
 /*    50 */  -105, -105, -105, -105, -105, -105, -105, -105, -105, -105,
 /*    60 */   654, -105, -105, -105,  165, -105, -105,  807, -105, -105,
 /*    70 */  -105, -105, -105, -105, -105, -105, -105, -105, -105, -105,
 /*    80 */  -105, -105,  983, -105, -105, -105,  891, -105, -105, -105,
 /*    90 */   972, -105, -105, -105, -105, -105, -105, -105, -105, -105,
 /*   100 */  -105, -105, -105, -105,  -96,  557, -105, -105,  -22, -105,
 /*   110 */  1046, -105, -105, -105, -105, -105, 1634, -105, -105, -105,
 /*   120 */  -105, -105, -105,  609, -105, -105,  774, -105,  410,  899,
 /*   130 */  -105,  -86, -105, -105, -105, 4339,  208, -105,  557, -105,
 /*   140 */  -105,  386, -105, 3189, -105, 4189, -105, -105, -105, -105,
 /*   150 */  -105, -105, -105, -105, -105, -105, -105, -105, -105, -105,
 /*   160 */  -105, -105, -105, -105, -105, -105, -105, -105, -105, -105,
 /*   170 */  -105, -105, -105, -105,  393, -105, 3589, 4189, -105, -105,
 /*   180 */  3839, 4189, -105,  687, -105,  468, -105,  607, -105, -105,
 /*   190 */   733, -105, 4139, 4189, -105, 3639, 4189, -105,  785, -105,
 /*   200 */  3989, 4189, -105, -105, 3289, 4189, -105,  589, -105, -105,
 /*   210 */  3038, -105, 2093,  827, -105, -105, 2681, 1470, -105, 1274,
 /*   220 */  2485, -105, 1960, 2352, -105, 1862, 2744, -105, 2646, 2583,
 /*   230 */  -105,  980, -105, 1764, -105, 1176, -105, -105, -105, 4428,
 /*   240 */  -105, 2779, -105, -105, 2548, -105, 2254, -105, 1666, -105,
 /*   250 */  2450, -105, 2940, -105, 2975, -105, 2289, -105, 1078, -105,
 /*   260 */   882, -105, 1799, -105, -105, -105, 1568, -105, -105, -105,
 /*   270 */  1995, -105, -105, -105, -105, 4783, -105, -105,  710, -105,
 /*   280 */  -105, 2191, -105, -105, -105, -105, -105, -105, -105, -105,
 /*   290 */  -105, 3793, -105, -105, 4718, -105, -105, 4829, -105, -105,
 /*   300 */  -105, 4882, -105, -105, -105, 4387, -105, -105, -105, -105,
 /*   310 */  -105, -105, -105, -105, -105, 4619, -105, -105, -105, -105,
 /*   320 */  -105, -105, 4684, -105, -105, -105, -105, -105, -105, -105,
 /*   330 */  4585, -105, -105, -105, -105, -105, -105, -105, -105, -105,
 /*   340 */  -105, -105, 4043, -105, -105, 3193, -105, -105, -105, -105,
 /*   350 */  -105, -105, -105, -105, -105, -105, -105, -105, -105, 4486,
 /*   360 */  -105, -105, -105, -105, -105, 4322, -105, -105, -105, -105,
 /*   370 */  -105, -105, -105, -105, -105, -105, -105, -105, -105, -105,
 /*   380 */  -105, -105, -105, -105, -105, -105, 1897, -105, 3339, 4189,
 /*   390 */  -105,    9, -105, -105,  931, -105,  -87, -105, -105, -105,
 /*   400 */  -105, -105, -105, 1505, -105, -105, -105,   56, 1407, -105,
 /*   410 */   490,  919, 1309, -105, -105, -105, -105,  230, 1211, -105,
 /*   420 */  -105,  490, -105, -105, -105, -105, -105,  880, 1113, -105,
 /*   430 */  -105,  877,  917, -105, -105,  109,  819, -105,  490,  858,
 /*   440 */   721, -105,  243,  525, -105,  490,  856,  329, -105,  260,
 /*   450 */   427, -105, -105, -105, -105,  231, -105, -105,  197, -105,
 /*   460 */  -105, -105, -105, -105, -105, -105, -105, -105, -105, -105,
 /*   470 */  2058, -105, -105, -105, -105, -105, -105, -105, -105, 2387,
 /*   480 */  -105, -105, -105, 2877, -105, -105,  198, -105, -105,  730,
 /*   490 */  -105, -105,  779, -105, -105,  296, -105, -105,  317, -105,
 /*   500 */  -105,  320, -105, -105,  330, -105, -105,  332, -105, -105,
 /*   510 */   341, -105, -105,  354, -105, -105,  418, -105, -105,  430,
 /*   520 */  -105, -105, -105, -105, -105, -105, 2842, -105, 3239, 4189,
 /*   530 */  -105, -105, -105, 1372, -105, 4039, 4189, -105, 2156, -105,
 /*   540 */  3939, 4189, -105, -105,  295, -105, -105, -105, -105, -105,
 /*   550 */  3789, 4189, -105, -105, -105, -105, 4089, 4189, -105, -105,
 /*   560 */    66, -105, 3439, -105, 4189, -105, -105, -105, 3539, -105,
 /*   570 */  4189, -105, 1732, -105, 3689, -105, 4189, -105,  569,  557,
 /*   580 */  -105, -105,  190, -105, 3489, -105, 4189, -105,  662, -105,
 /*   590 */  3889, -105, 4189, -105, -105, -105, 3739, -105, 4189, -105,
 /*   600 */  1054, -105, 3389, -105, 4189, -105,  141,  208, -105, -105,
 /*   610 */   569, -105, -105,  689, -105, -105, -105, -105,  707, -105,
 /*   620 */  -105, -105,  572,  557,  762, -105,  769,  775, -105, -105,
 /*   630 */  -105,  593, -105, -105, -105,  797, -105, -105, -105, -105,
 /*   640 */  -105, -105, -105,  557,  859, -105,  867,  872, -105, -105,
 /*   650 */  -105, -105, -105, -105,  -55,   43,  208,  572,  208, -105,
 /*   660 */   208,  943, -105, -105, -105,  555,  975, -105, -105, -105,
 /*   670 */  1002, -105, -105,  620, -105,  645,  780, -105, -105, -105,
 /*   680 */   896, -105, -105,  752, -105, -105,
};
static YYACTIONTYPE yy_default[] = {
 /*     0 */  1016, 1016, 1016,  688,  690,  691,  692,  693,  694, 1016,
 /*    10 */  1016,  695, 1016, 1016,  696, 1016, 1016,  697, 1016,  712,
 /*    20 */  1016,  713,  743, 1016, 1016, 1016, 1016, 1016, 1016, 1016,
 /*    30 */   763,  781,  782, 1016,  783,  785,  786,  787,  788, 1016,
 /*    40 */  1016,  799, 1016, 1016,  800,  801,  802,  803,  804,  805,
 /*    50 */   806,  807,  808,  809,  810,  811,  784,  767, 1016, 1016,
 /*    60 */  1016, 1016,  764,  768, 1016,  789,  791, 1016,  795, 1000,
 /*    70 */  1001, 1002, 1003, 1004, 1005, 1006, 1016, 1016, 1007, 1008,
 /*    80 */  1016,  792, 1016,  796, 1016,  793, 1016,  797, 1016,  794,
 /*    90 */  1016,  798, 1016, 1016,  790,  771,  773,  774,  775,  776,
 /*   100 */   777,  778,  779,  780, 1016, 1016, 1016, 1016, 1016, 1016,
 /*   110 */  1016, 1016,  765,  769, 1016, 1016, 1016, 1016,  766,  770,
 /*   120 */   772,  744, 1016, 1016,  698, 1016, 1016,  699, 1016, 1016,
 /*   130 */   701, 1016,  707, 1016,  708, 1016, 1016,  741, 1016, 1016,
 /*   140 */  1016, 1016, 1016, 1016,  747, 1016,  749,  812,  814,  815,
 /*   150 */   816,  817,  818,  819,  820,  821,  822,  823,  824,  825,
 /*   160 */   826,  827,  828,  829,  830,  831,  832,  833,  834,  835,
 /*   170 */  1016,  836, 1016,  837, 1016, 1016, 1016, 1016,  840, 1016,
 /*   180 */  1016, 1016,  841, 1016, 1016, 1016,  844, 1016,  845,  846,
 /*   190 */  1016, 1016,  848,  849, 1016, 1016, 1016,  852, 1016, 1016,
 /*   200 */  1016, 1016,  854, 1016, 1016, 1016, 1016, 1016, 1016,  856,
 /*   210 */  1016,  917, 1016, 1016,  918, 1016, 1016, 1016,  919, 1016,
 /*   220 */  1016,  920, 1016, 1016,  921, 1016, 1016,  922, 1016, 1016,
 /*   230 */   923, 1016,  929, 1016,  930, 1016,  931, 1016,  935, 1016,
 /*   240 */   991, 1016, 1016,  940, 1016,  941, 1016,  942, 1016,  943,
 /*   250 */  1016,  944, 1016,  945, 1016,  946, 1016,  947, 1016,  948,
 /*   260 */  1016,  949, 1016, 1016,  932, 1016, 1016,  933, 1016, 1016,
 /*   270 */  1016,  934,  953, 1016,  938, 1016, 1016, 1016, 1016,  950,
 /*   280 */   951, 1016,  952,  954,  955,  956,  957,  958,  959,  960,
 /*   290 */   961, 1016,  999,  953, 1016, 1016,  964, 1016,  965, 1016,
 /*   300 */   966, 1016, 1016, 1016, 1016, 1016, 1016,  972,  973,  986,
 /*   310 */   987,  988,  990, 1016, 1016, 1016, 1016,  976,  977, 1016,
 /*   320 */  1016, 1016, 1016, 1016,  978,  979,  989, 1016, 1016,  967,
 /*   330 */  1016,  968, 1016,  969, 1016,  970,  971,  954,  955,  962,
 /*   340 */  1016,  963, 1016,  992, 1016, 1016,  994,  995,  993, 1016,
 /*   350 */   974,  975,  939, 1016,  980,  981, 1016, 1016,  936, 1016,
 /*   360 */  1016,  982,  983, 1016,  937, 1016, 1016,  984,  985,  928,
 /*   370 */   927,  926,  925, 1016, 1009, 1016, 1010, 1016, 1016, 1011,
 /*   380 */  1016, 1012,  924, 1013, 1016, 1016, 1016, 1016, 1016, 1016,
 /*   390 */   857, 1016, 1016,  861, 1016,  862, 1016,  864,  865,  866,
 /*   400 */   867,  868,  869, 1016,  870,  916, 1016, 1016, 1016,  871,
 /*   410 */  1016, 1016, 1016,  874, 1016,  883, 1016, 1016, 1016,  875,
 /*   420 */   881, 1016,  882,  884,  885, 1016, 1016, 1016, 1016,  872,
 /*   430 */  1016, 1016, 1016,  873, 1016, 1016, 1016,  876, 1016, 1016,
 /*   440 */  1016,  877, 1016, 1016,  878, 1016, 1016, 1016,  879, 1016,
 /*   450 */  1016,  880,  886,  887,  863, 1016, 1016,  889, 1016,  890,
 /*   460 */   892,  891,  986,  893,  988,  894,  987,  895,  951,  896,
 /*   470 */  1016, 1016,  897,  898, 1016, 1016, 1016, 1016,  899, 1016,
 /*   480 */  1016, 1016,  900, 1016, 1016,  901, 1016, 1016,  902, 1016,
 /*   490 */   912,  914, 1016,  915,  913, 1016, 1016,  903, 1016, 1016,
 /*   500 */   904, 1016, 1016,  905, 1016, 1016,  906, 1016, 1016,  907,
 /*   510 */  1016, 1016,  908, 1016, 1016,  909, 1016, 1016,  910, 1016,
 /*   520 */  1016,  911, 1016, 1014, 1015,  813, 1016, 1016, 1016, 1016,
 /*   530 */   858, 1016, 1016, 1016, 1016, 1016, 1016,  859, 1016, 1016,
 /*   540 */  1016, 1016,  860, 1016, 1016, 1016,  855,  853,  851, 1016,
 /*   550 */  1016,  850,  847,  842,  838, 1016, 1016, 1016,  843,  839,
 /*   560 */  1016, 1016, 1016,  755, 1016,  757, 1016, 1016, 1016,  748,
 /*   570 */  1016,  750, 1016, 1016, 1016,  756, 1016,  758, 1016, 1016,
 /*   580 */  1016, 1016, 1016, 1016, 1016,  751, 1016,  753, 1016, 1016,
 /*   590 */  1016,  759, 1016,  761, 1016, 1016, 1016,  752, 1016,  754,
 /*   600 */  1016, 1016, 1016,  760, 1016,  762, 1016, 1016,  739,  742,
 /*   610 */  1016, 1016, 1016, 1016, 1016,  745, 1016, 1016, 1016, 1016,
 /*   620 */   746,  723, 1016, 1016, 1016,  725, 1016, 1016,  727, 1016,
 /*   630 */   731, 1016, 1016, 1016, 1016, 1016,  735,  737, 1016,  738,
 /*   640 */   736, 1016,  729, 1016, 1016,  726, 1016, 1016,  728, 1016,
 /*   650 */   732, 1016,  730,  740, 1016, 1016, 1016, 1016, 1016,  724,
 /*   660 */  1016, 1016,  709,  711,  710, 1016, 1016,  700, 1016, 1016,
 /*   670 */  1016,  702, 1016, 1016,  703, 1016, 1016,  704, 1016, 1016,
 /*   680 */  1016,  705, 1016, 1016,  706,  689,
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
// 3177 "parser.c"
      break;
    case 107:
// 1028 "parser.lemon"
{ json_object_put((yypminor->yy259)); }
// 3182 "parser.c"
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
// 3729 "parser.c"
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
// 3771 "parser.c"
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
// 3786 "parser.c"
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
// 3810 "parser.c"
        break;
      case 9:
// 1063 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_namespace(yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(39,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3819 "parser.c"
        break;
      case 10:
// 1067 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_interface(yymsp[-1].minor.yy0, yymsp[0].minor.yy259, NULL, status->scanner_state);
  yy_destructor(42,&yymsp[-2].minor);
}
// 3827 "parser.c"
        break;
      case 11:
// 1071 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_interface(yymsp[-3].minor.yy0, yymsp[0].minor.yy259, yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(42,&yymsp[-4].minor);
  yy_destructor(43,&yymsp[-2].minor);
}
// 3836 "parser.c"
        break;
      case 12:
// 1075 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class(yymsp[-1].minor.yy0, yymsp[0].minor.yy259, 0, 0, NULL, NULL, status->scanner_state);
  yy_destructor(44,&yymsp[-2].minor);
}
// 3844 "parser.c"
        break;
      case 13:
// 1079 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy259, 0, 0, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(44,&yymsp[-4].minor);
  yy_destructor(43,&yymsp[-2].minor);
}
// 3853 "parser.c"
        break;
      case 14:
// 1083 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy259, 0, 0, NULL, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(44,&yymsp[-4].minor);
  yy_destructor(45,&yymsp[-2].minor);
}
// 3862 "parser.c"
        break;
      case 15:
// 1087 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class(yymsp[-5].minor.yy0, yymsp[0].minor.yy259, 0, 0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(44,&yymsp[-6].minor);
  yy_destructor(43,&yymsp[-4].minor);
  yy_destructor(45,&yymsp[-2].minor);
}
// 3872 "parser.c"
        break;
      case 16:
// 1091 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class(yymsp[-1].minor.yy0, yymsp[0].minor.yy259, 1, 0, NULL, NULL, status->scanner_state);
  yy_destructor(46,&yymsp[-3].minor);
  yy_destructor(44,&yymsp[-2].minor);
}
// 3881 "parser.c"
        break;
      case 17:
// 1095 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy259, 1, 0, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(46,&yymsp[-5].minor);
  yy_destructor(44,&yymsp[-4].minor);
  yy_destructor(43,&yymsp[-2].minor);
}
// 3891 "parser.c"
        break;
      case 18:
// 1099 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy259, 1, 0, NULL, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(46,&yymsp[-5].minor);
  yy_destructor(44,&yymsp[-4].minor);
  yy_destructor(45,&yymsp[-2].minor);
}
// 3901 "parser.c"
        break;
      case 19:
// 1103 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class(yymsp[-1].minor.yy0, yymsp[0].minor.yy259, 0, 1, NULL, NULL, status->scanner_state);
  yy_destructor(47,&yymsp[-3].minor);
  yy_destructor(44,&yymsp[-2].minor);
}
// 3910 "parser.c"
        break;
      case 20:
// 1107 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy259, 0, 1, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(47,&yymsp[-5].minor);
  yy_destructor(44,&yymsp[-4].minor);
  yy_destructor(43,&yymsp[-2].minor);
}
// 3920 "parser.c"
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
// 3931 "parser.c"
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
// 3942 "parser.c"
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
// 3957 "parser.c"
        break;
      case 25:
      case 267:
      case 310:
// 1127 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state);
}
// 3966 "parser.c"
        break;
      case 28:
// 1139 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_definition(yymsp[0].minor.yy259, NULL, NULL, status->scanner_state);
}
// 3973 "parser.c"
        break;
      case 29:
// 1143 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_definition(NULL, NULL, yymsp[0].minor.yy259, status->scanner_state);
}
// 3980 "parser.c"
        break;
      case 30:
// 1147 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_definition(NULL, yymsp[0].minor.yy259, NULL, status->scanner_state);
}
// 3987 "parser.c"
        break;
      case 31:
// 1151 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_definition(yymsp[-1].minor.yy259, yymsp[0].minor.yy259, NULL, status->scanner_state);
}
// 3994 "parser.c"
        break;
      case 32:
// 1155 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_definition(yymsp[-1].minor.yy259, NULL, yymsp[0].minor.yy259, status->scanner_state);
}
// 4001 "parser.c"
        break;
      case 33:
// 1159 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_definition(yymsp[0].minor.yy259, NULL, yymsp[-1].minor.yy259, status->scanner_state);
}
// 4008 "parser.c"
        break;
      case 34:
// 1163 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_definition(NULL, yymsp[0].minor.yy259, yymsp[-1].minor.yy259, status->scanner_state);
}
// 4015 "parser.c"
        break;
      case 35:
// 1167 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_definition(yymsp[-2].minor.yy259, yymsp[0].minor.yy259, yymsp[-1].minor.yy259, status->scanner_state);
}
// 4022 "parser.c"
        break;
      case 36:
// 1171 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_definition(yymsp[-1].minor.yy259, yymsp[0].minor.yy259, yymsp[-2].minor.yy259, status->scanner_state);
}
// 4029 "parser.c"
        break;
      case 39:
// 1188 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_property(yymsp[-2].minor.yy259, yymsp[-1].minor.yy0, NULL, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(41,&yymsp[0].minor);
}
// 4037 "parser.c"
        break;
      case 40:
// 1192 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_property(yymsp[-2].minor.yy259, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(41,&yymsp[0].minor);
}
// 4045 "parser.c"
        break;
      case 41:
// 1196 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_property(yymsp[-4].minor.yy259, yymsp[-3].minor.yy0, yymsp[-1].minor.yy259, yymsp[-5].minor.yy0, NULL, status->scanner_state);
  yy_destructor(51,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 4054 "parser.c"
        break;
      case 42:
// 1200 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_property(yymsp[-4].minor.yy259, yymsp[-3].minor.yy0, yymsp[-1].minor.yy259, NULL, NULL, status->scanner_state);
  yy_destructor(51,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 4063 "parser.c"
        break;
      case 43:
// 1204 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_property(yymsp[-3].minor.yy259, yymsp[-2].minor.yy0, NULL, yymsp[-4].minor.yy0, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(41,&yymsp[0].minor);
}
// 4071 "parser.c"
        break;
      case 44:
// 1208 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_property(yymsp[-3].minor.yy259, yymsp[-2].minor.yy0, NULL, NULL, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(41,&yymsp[0].minor);
}
// 4079 "parser.c"
        break;
      case 45:
// 1212 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_property(yymsp[-5].minor.yy259, yymsp[-4].minor.yy0, yymsp[-2].minor.yy259, yymsp[-6].minor.yy0, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(51,&yymsp[-3].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 4088 "parser.c"
        break;
      case 46:
// 1216 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_property(yymsp[-5].minor.yy259, yymsp[-4].minor.yy0, yymsp[-2].minor.yy259, NULL, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(51,&yymsp[-3].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 4097 "parser.c"
        break;
      case 51:
// 1236 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_property_shortcut(NULL, yymsp[0].minor.yy0, status->scanner_state);
}
// 4104 "parser.c"
        break;
      case 52:
// 1240 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_property_shortcut(yymsp[-1].minor.yy0, yymsp[0].minor.yy0, status->scanner_state);
}
// 4111 "parser.c"
        break;
      case 59:
// 1269 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy259, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(52,&yymsp[-4].minor);
  yy_destructor(51,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 4121 "parser.c"
        break;
      case 60:
// 1273 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy259, NULL, status->scanner_state);
  yy_destructor(52,&yymsp[-4].minor);
  yy_destructor(51,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 4131 "parser.c"
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
// 4143 "parser.c"
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
// 4155 "parser.c"
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
// 4167 "parser.c"
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
// 4179 "parser.c"
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
// 4191 "parser.c"
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
// 4203 "parser.c"
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
// 4215 "parser.c"
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
// 4227 "parser.c"
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
// 4240 "parser.c"
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
// 4253 "parser.c"
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
// 4266 "parser.c"
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
// 4279 "parser.c"
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
// 4292 "parser.c"
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
// 4305 "parser.c"
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
// 4318 "parser.c"
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
// 4331 "parser.c"
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
// 4343 "parser.c"
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
// 4355 "parser.c"
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
// 4367 "parser.c"
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
// 4379 "parser.c"
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
// 4390 "parser.c"
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
// 4401 "parser.c"
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
// 4412 "parser.c"
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
// 4423 "parser.c"
        break;
      case 87:
// 1384 "parser.lemon"
{
	yygotominor.yy259 = json_object_new_string("public");
  yy_destructor(1,&yymsp[0].minor);
}
// 4431 "parser.c"
        break;
      case 88:
// 1388 "parser.lemon"
{
	yygotominor.yy259 = json_object_new_string("protected");
  yy_destructor(2,&yymsp[0].minor);
}
// 4439 "parser.c"
        break;
      case 89:
// 1392 "parser.lemon"
{
	yygotominor.yy259 = json_object_new_string("private");
  yy_destructor(4,&yymsp[0].minor);
}
// 4447 "parser.c"
        break;
      case 90:
// 1396 "parser.lemon"
{
	yygotominor.yy259 = json_object_new_string("static");
  yy_destructor(3,&yymsp[0].minor);
}
// 4455 "parser.c"
        break;
      case 91:
// 1400 "parser.lemon"
{
	yygotominor.yy259 = json_object_new_string("scoped");
  yy_destructor(5,&yymsp[0].minor);
}
// 4463 "parser.c"
        break;
      case 92:
// 1404 "parser.lemon"
{
	yygotominor.yy259 = json_object_new_string("inline");
  yy_destructor(56,&yymsp[0].minor);
}
// 4471 "parser.c"
        break;
      case 93:
// 1408 "parser.lemon"
{
	yygotominor.yy259 = json_object_new_string("abstract");
  yy_destructor(46,&yymsp[0].minor);
}
// 4479 "parser.c"
        break;
      case 94:
// 1412 "parser.lemon"
{
	yygotominor.yy259 = json_object_new_string("final");
  yy_destructor(47,&yymsp[0].minor);
}
// 4487 "parser.c"
        break;
      case 95:
// 1417 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_return_type(1, NULL, status->scanner_state);
  yy_destructor(57,&yymsp[0].minor);
}
// 4495 "parser.c"
        break;
      case 96:
// 1421 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_return_type(0, yymsp[0].minor.yy259, status->scanner_state);
}
// 4502 "parser.c"
        break;
      case 97:
// 1425 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_list(yymsp[-2].minor.yy259, yymsp[0].minor.yy259);
  yy_destructor(15,&yymsp[-1].minor);
}
// 4510 "parser.c"
        break;
      case 99:
// 1433 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_return_type_item(yymsp[0].minor.yy259, NULL, 0, 0, status->scanner_state);
}
// 4517 "parser.c"
        break;
      case 100:
// 1437 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_return_type_item(yymsp[-1].minor.yy259, NULL, 1, 0, status->scanner_state);
  yy_destructor(35,&yymsp[0].minor);
}
// 4525 "parser.c"
        break;
      case 101:
// 1441 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_return_type_item(NULL, yymsp[0].minor.yy259, 0, 0, status->scanner_state);
}
// 4532 "parser.c"
        break;
      case 102:
// 1445 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_return_type_item(NULL, yymsp[0].minor.yy259, 0, 1, status->scanner_state);
}
// 4539 "parser.c"
        break;
      case 105:
// 1458 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_parameter(NULL, NULL, yymsp[0].minor.yy0, NULL, 0, status->scanner_state);
}
// 4546 "parser.c"
        break;
      case 106:
// 1462 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_parameter(yymsp[-1].minor.yy259, NULL, yymsp[0].minor.yy0, NULL, 0, status->scanner_state);
}
// 4553 "parser.c"
        break;
      case 107:
// 1466 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_parameter(yymsp[-2].minor.yy259, NULL, yymsp[0].minor.yy0, NULL, 1, status->scanner_state);
  yy_destructor(35,&yymsp[-1].minor);
}
// 4561 "parser.c"
        break;
      case 108:
// 1470 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_parameter(NULL, yymsp[-1].minor.yy259, yymsp[0].minor.yy0, NULL, 0, status->scanner_state);
}
// 4568 "parser.c"
        break;
      case 109:
// 1474 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_parameter(NULL, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy259, 0, status->scanner_state);
  yy_destructor(51,&yymsp[-1].minor);
}
// 4576 "parser.c"
        break;
      case 110:
// 1478 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_parameter(yymsp[-3].minor.yy259, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy259, 0, status->scanner_state);
  yy_destructor(51,&yymsp[-1].minor);
}
// 4584 "parser.c"
        break;
      case 111:
// 1482 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_parameter(yymsp[-4].minor.yy259, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy259, 1, status->scanner_state);
  yy_destructor(35,&yymsp[-3].minor);
  yy_destructor(51,&yymsp[-1].minor);
}
// 4593 "parser.c"
        break;
      case 112:
// 1486 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_parameter(NULL, yymsp[-3].minor.yy259, yymsp[-2].minor.yy0, yymsp[0].minor.yy259, 0, status->scanner_state);
  yy_destructor(51,&yymsp[-1].minor);
}
// 4601 "parser.c"
        break;
      case 113:
// 1490 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(19,&yymsp[-2].minor);
  yy_destructor(20,&yymsp[0].minor);
}
// 4610 "parser.c"
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
// 4621 "parser.c"
        break;
      case 115:
// 1498 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_type(XX_TYPE_INTEGER);
  yy_destructor(59,&yymsp[0].minor);
}
// 4629 "parser.c"
        break;
      case 116:
// 1502 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_type(XX_TYPE_UINTEGER);
  yy_destructor(60,&yymsp[0].minor);
}
// 4637 "parser.c"
        break;
      case 117:
// 1506 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_type(XX_TYPE_LONG);
  yy_destructor(61,&yymsp[0].minor);
}
// 4645 "parser.c"
        break;
      case 118:
// 1510 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_type(XX_TYPE_ULONG);
  yy_destructor(62,&yymsp[0].minor);
}
// 4653 "parser.c"
        break;
      case 119:
// 1514 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_type(XX_TYPE_CHAR);
  yy_destructor(63,&yymsp[0].minor);
}
// 4661 "parser.c"
        break;
      case 120:
// 1518 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_type(XX_TYPE_UCHAR);
  yy_destructor(64,&yymsp[0].minor);
}
// 4669 "parser.c"
        break;
      case 121:
// 1522 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_type(XX_TYPE_DOUBLE);
  yy_destructor(65,&yymsp[0].minor);
}
// 4677 "parser.c"
        break;
      case 122:
// 1526 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_type(XX_TYPE_BOOL);
  yy_destructor(66,&yymsp[0].minor);
}
// 4685 "parser.c"
        break;
      case 123:
// 1530 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_type(XX_TYPE_STRING);
  yy_destructor(67,&yymsp[0].minor);
}
// 4693 "parser.c"
        break;
      case 124:
// 1534 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_type(XX_TYPE_ARRAY);
  yy_destructor(68,&yymsp[0].minor);
}
// 4701 "parser.c"
        break;
      case 125:
// 1538 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_type(XX_TYPE_VAR);
  yy_destructor(69,&yymsp[0].minor);
}
// 4709 "parser.c"
        break;
      case 149:
// 1636 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_empty_statement(status->scanner_state);
  yy_destructor(41,&yymsp[0].minor);
}
// 4717 "parser.c"
        break;
      case 150:
// 1640 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_break_statement(status->scanner_state);
  yy_destructor(70,&yymsp[-1].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 4726 "parser.c"
        break;
      case 151:
// 1644 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_continue_statement(status->scanner_state);
  yy_destructor(71,&yymsp[-1].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 4735 "parser.c"
        break;
      case 152:
// 1648 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_if_statement(yymsp[-2].minor.yy259, NULL, NULL, status->scanner_state);
  yy_destructor(72,&yymsp[-3].minor);
  yy_destructor(48,&yymsp[-1].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4745 "parser.c"
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
// 4758 "parser.c"
        break;
      case 154:
// 1656 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_if_statement(yymsp[-3].minor.yy259, yymsp[-1].minor.yy259, NULL, status->scanner_state);
  yy_destructor(72,&yymsp[-4].minor);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4768 "parser.c"
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
// 4781 "parser.c"
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
// 4794 "parser.c"
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
// 4807 "parser.c"
        break;
      case 158:
// 1672 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_switch_statement(yymsp[-2].minor.yy259, NULL, status->scanner_state);
  yy_destructor(74,&yymsp[-3].minor);
  yy_destructor(48,&yymsp[-1].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4817 "parser.c"
        break;
      case 159:
// 1676 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_switch_statement(yymsp[-3].minor.yy259, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(74,&yymsp[-4].minor);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4827 "parser.c"
        break;
      case 162:
// 1688 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_case_clause(yymsp[-1].minor.yy259, NULL, status->scanner_state);
  yy_destructor(75,&yymsp[-2].minor);
  yy_destructor(76,&yymsp[0].minor);
}
// 4836 "parser.c"
        break;
      case 163:
// 1692 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_case_clause(yymsp[-2].minor.yy259, yymsp[0].minor.yy259, status->scanner_state);
  yy_destructor(75,&yymsp[-3].minor);
  yy_destructor(76,&yymsp[-1].minor);
}
// 4845 "parser.c"
        break;
      case 164:
// 1696 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_case_clause(NULL, yymsp[0].minor.yy259, status->scanner_state);
  yy_destructor(77,&yymsp[-2].minor);
  yy_destructor(76,&yymsp[-1].minor);
}
// 4854 "parser.c"
        break;
      case 165:
// 1700 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_loop_statement(NULL, status->scanner_state);
  yy_destructor(78,&yymsp[-2].minor);
  yy_destructor(48,&yymsp[-1].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4864 "parser.c"
        break;
      case 166:
// 1704 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_loop_statement(yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(78,&yymsp[-3].minor);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4874 "parser.c"
        break;
      case 167:
// 1708 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_while_statement(yymsp[-2].minor.yy259, NULL, status->scanner_state);
  yy_destructor(79,&yymsp[-3].minor);
  yy_destructor(48,&yymsp[-1].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4884 "parser.c"
        break;
      case 168:
// 1712 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_while_statement(yymsp[-3].minor.yy259, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(79,&yymsp[-4].minor);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4894 "parser.c"
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
// 4906 "parser.c"
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
// 4918 "parser.c"
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
// 4929 "parser.c"
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
// 4941 "parser.c"
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
// 4953 "parser.c"
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
// 4966 "parser.c"
        break;
      case 175:
// 1740 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_let_statement(yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(84,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 4975 "parser.c"
        break;
      case 178:
// 1753 "parser.lemon"
{
	yygotominor.yy259 = json_object_new_string("assign");
  yy_destructor(51,&yymsp[0].minor);
}
// 4983 "parser.c"
        break;
      case 179:
// 1758 "parser.lemon"
{
	yygotominor.yy259 = json_object_new_string("add-assign");
  yy_destructor(85,&yymsp[0].minor);
}
// 4991 "parser.c"
        break;
      case 180:
// 1763 "parser.lemon"
{
	yygotominor.yy259 = json_object_new_string("sub-assign");
  yy_destructor(86,&yymsp[0].minor);
}
// 4999 "parser.c"
        break;
      case 181:
// 1767 "parser.lemon"
{
	yygotominor.yy259 = json_object_new_string("mult-assign");
  yy_destructor(87,&yymsp[0].minor);
}
// 5007 "parser.c"
        break;
      case 182:
// 1771 "parser.lemon"
{
	yygotominor.yy259 = json_object_new_string("div-assign");
  yy_destructor(88,&yymsp[0].minor);
}
// 5015 "parser.c"
        break;
      case 183:
// 1775 "parser.lemon"
{
	yygotominor.yy259 = json_object_new_string("concat-assign");
  yy_destructor(89,&yymsp[0].minor);
}
// 5023 "parser.c"
        break;
      case 184:
// 1780 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_let_assignment("variable", yymsp[-1].minor.yy259, yymsp[-2].minor.yy0, NULL, NULL, yymsp[0].minor.yy259, status->scanner_state);
}
// 5030 "parser.c"
        break;
      case 185:
// 1785 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_let_assignment("object-property", yymsp[-1].minor.yy259, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, yymsp[0].minor.yy259, status->scanner_state);
  yy_destructor(38,&yymsp[-3].minor);
}
// 5038 "parser.c"
        break;
      case 186:
// 1790 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_let_assignment("variable-dynamic-object-property", yymsp[-1].minor.yy259, yymsp[-6].minor.yy0, yymsp[-3].minor.yy0, NULL, yymsp[0].minor.yy259, status->scanner_state);
  yy_destructor(38,&yymsp[-5].minor);
  yy_destructor(48,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
}
// 5048 "parser.c"
        break;
      case 187:
// 1795 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_let_assignment("string-dynamic-object-property", yymsp[-1].minor.yy259, yymsp[-6].minor.yy0, yymsp[-3].minor.yy0, NULL, yymsp[0].minor.yy259, status->scanner_state);
  yy_destructor(38,&yymsp[-5].minor);
  yy_destructor(48,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
}
// 5058 "parser.c"
        break;
      case 188:
// 1800 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_let_assignment("object-property-append", yymsp[-1].minor.yy259, yymsp[-6].minor.yy0, yymsp[-4].minor.yy0, NULL, yymsp[0].minor.yy259, status->scanner_state);
  yy_destructor(38,&yymsp[-5].minor);
  yy_destructor(37,&yymsp[-3].minor);
  yy_destructor(58,&yymsp[-2].minor);
}
// 5068 "parser.c"
        break;
      case 189:
// 1805 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_let_assignment("object-property-array-index", yymsp[-1].minor.yy259, yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-2].minor.yy259, yymsp[0].minor.yy259, status->scanner_state);
  yy_destructor(38,&yymsp[-4].minor);
}
// 5076 "parser.c"
        break;
      case 190:
// 1810 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_let_assignment("static-property", yymsp[-1].minor.yy259, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, yymsp[0].minor.yy259, status->scanner_state);
  yy_destructor(91,&yymsp[-3].minor);
}
// 5084 "parser.c"
        break;
      case 191:
// 1815 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_let_assignment("static-property-append", yymsp[-1].minor.yy259, yymsp[-6].minor.yy0, yymsp[-4].minor.yy0, NULL, yymsp[0].minor.yy259, status->scanner_state);
  yy_destructor(91,&yymsp[-5].minor);
  yy_destructor(37,&yymsp[-3].minor);
  yy_destructor(58,&yymsp[-2].minor);
}
// 5094 "parser.c"
        break;
      case 192:
// 1820 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_let_assignment("static-property-array-index", yymsp[-1].minor.yy259, yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-2].minor.yy259, yymsp[0].minor.yy259, status->scanner_state);
  yy_destructor(91,&yymsp[-4].minor);
}
// 5102 "parser.c"
        break;
      case 193:
// 1825 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_let_assignment("variable-append", yymsp[-1].minor.yy259, yymsp[-4].minor.yy0, NULL, NULL, yymsp[0].minor.yy259, status->scanner_state);
  yy_destructor(37,&yymsp[-3].minor);
  yy_destructor(58,&yymsp[-2].minor);
}
// 5111 "parser.c"
        break;
      case 194:
// 1830 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_let_assignment("array-index", yymsp[-1].minor.yy259, yymsp[-3].minor.yy0, NULL, yymsp[-2].minor.yy259, yymsp[0].minor.yy259, status->scanner_state);
}
// 5118 "parser.c"
        break;
      case 197:
// 1842 "parser.lemon"
{
	yygotominor.yy259 = yymsp[-1].minor.yy259;
  yy_destructor(37,&yymsp[-2].minor);
  yy_destructor(58,&yymsp[0].minor);
}
// 5127 "parser.c"
        break;
      case 198:
// 1847 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_let_assignment("object-property-incr", NULL, yymsp[-3].minor.yy0, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(92,&yymsp[0].minor);
}
// 5136 "parser.c"
        break;
      case 199:
// 1852 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_let_assignment("object-property-decr", NULL, yymsp[-3].minor.yy0, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(93,&yymsp[0].minor);
}
// 5145 "parser.c"
        break;
      case 200:
// 1857 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_let_assignment("incr", NULL, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(92,&yymsp[0].minor);
}
// 5153 "parser.c"
        break;
      case 201:
// 1862 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_let_assignment("decr", NULL, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(93,&yymsp[0].minor);
}
// 5161 "parser.c"
        break;
      case 203:
// 1870 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_echo_statement(yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(94,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 5170 "parser.c"
        break;
      case 206:
// 1882 "parser.lemon"
{
	yygotominor.yy259 = yymsp[0].minor.yy259;;
}
// 5177 "parser.c"
        break;
      case 207:
// 1887 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_mcall_statement(yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(41,&yymsp[0].minor);
}
// 5185 "parser.c"
        break;
      case 208:
// 1892 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_fcall_statement(yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(41,&yymsp[0].minor);
}
// 5193 "parser.c"
        break;
      case 209:
// 1897 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_scall_statement(yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(41,&yymsp[0].minor);
}
// 5201 "parser.c"
        break;
      case 210:
// 1902 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_fetch_statement(yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(41,&yymsp[0].minor);
}
// 5209 "parser.c"
        break;
      case 211:
// 1907 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_return_statement(yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(95,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 5218 "parser.c"
        break;
      case 212:
// 1912 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_return_statement(NULL, status->scanner_state);
  yy_destructor(95,&yymsp[-1].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 5227 "parser.c"
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
// 5241 "parser.c"
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
// 5256 "parser.c"
        break;
      case 215:
// 1935 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_throw_exception(yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(97,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 5265 "parser.c"
        break;
      case 216:
// 1939 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_declare_statement(XX_T_TYPE_INTEGER, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(59,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 5274 "parser.c"
        break;
      case 217:
// 1943 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_declare_statement(XX_T_TYPE_UINTEGER, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(60,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 5283 "parser.c"
        break;
      case 218:
// 1947 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_declare_statement(XX_T_TYPE_CHAR, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(63,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 5292 "parser.c"
        break;
      case 219:
// 1951 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_declare_statement(XX_T_TYPE_UCHAR, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(64,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 5301 "parser.c"
        break;
      case 220:
// 1955 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_declare_statement(XX_T_TYPE_LONG, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(61,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 5310 "parser.c"
        break;
      case 221:
// 1959 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_declare_statement(XX_T_TYPE_ULONG, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(62,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 5319 "parser.c"
        break;
      case 222:
// 1963 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_declare_statement(XX_T_TYPE_DOUBLE, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(65,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 5328 "parser.c"
        break;
      case 223:
// 1967 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_declare_statement(XX_T_TYPE_STRING, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(67,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 5337 "parser.c"
        break;
      case 224:
// 1971 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_declare_statement(XX_T_TYPE_BOOL, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(66,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 5346 "parser.c"
        break;
      case 225:
// 1975 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_declare_statement(XX_T_TYPE_VAR, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(69,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 5355 "parser.c"
        break;
      case 228:
// 1987 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_declare_variable(yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 5362 "parser.c"
        break;
      case 229:
// 1991 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_declare_variable(yymsp[-2].minor.yy0, yymsp[0].minor.yy259, status->scanner_state);
  yy_destructor(51,&yymsp[-1].minor);
}
// 5370 "parser.c"
        break;
      case 231:
// 1999 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("not", yymsp[0].minor.yy259, NULL, NULL, status->scanner_state);
  yy_destructor(35,&yymsp[-1].minor);
}
// 5378 "parser.c"
        break;
      case 232:
// 2003 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("isset", yymsp[0].minor.yy259, NULL, NULL, status->scanner_state);
  yy_destructor(31,&yymsp[-1].minor);
}
// 5386 "parser.c"
        break;
      case 233:
// 2007 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("require", yymsp[0].minor.yy259, NULL, NULL, status->scanner_state);
  yy_destructor(7,&yymsp[-1].minor);
}
// 5394 "parser.c"
        break;
      case 234:
// 2011 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("clone", yymsp[0].minor.yy259, NULL, NULL, status->scanner_state);
  yy_destructor(10,&yymsp[-1].minor);
}
// 5402 "parser.c"
        break;
      case 235:
// 2015 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("empty", yymsp[0].minor.yy259, NULL, NULL, status->scanner_state);
  yy_destructor(11,&yymsp[-1].minor);
}
// 5410 "parser.c"
        break;
      case 236:
// 2019 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("likely", yymsp[0].minor.yy259, NULL, NULL, status->scanner_state);
  yy_destructor(8,&yymsp[-1].minor);
}
// 5418 "parser.c"
        break;
      case 237:
// 2023 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("unlikely", yymsp[0].minor.yy259, NULL, NULL, status->scanner_state);
  yy_destructor(9,&yymsp[-1].minor);
}
// 5426 "parser.c"
        break;
      case 238:
// 2027 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("equals", yymsp[-2].minor.yy259, yymsp[0].minor.yy259, NULL, status->scanner_state);
  yy_destructor(17,&yymsp[-1].minor);
}
// 5434 "parser.c"
        break;
      case 239:
// 2031 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("not-equals", yymsp[-2].minor.yy259, yymsp[0].minor.yy259, NULL, status->scanner_state);
  yy_destructor(24,&yymsp[-1].minor);
}
// 5442 "parser.c"
        break;
      case 240:
// 2035 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("identical", yymsp[-2].minor.yy259, yymsp[0].minor.yy259, NULL, status->scanner_state);
  yy_destructor(18,&yymsp[-1].minor);
}
// 5450 "parser.c"
        break;
      case 241:
// 2039 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("not-identical", yymsp[-2].minor.yy259, yymsp[0].minor.yy259, NULL, status->scanner_state);
  yy_destructor(23,&yymsp[-1].minor);
}
// 5458 "parser.c"
        break;
      case 242:
// 2043 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("less", yymsp[-2].minor.yy259, yymsp[0].minor.yy259, NULL, status->scanner_state);
  yy_destructor(19,&yymsp[-1].minor);
}
// 5466 "parser.c"
        break;
      case 243:
// 2047 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("greater", yymsp[-2].minor.yy259, yymsp[0].minor.yy259, NULL, status->scanner_state);
  yy_destructor(20,&yymsp[-1].minor);
}
// 5474 "parser.c"
        break;
      case 244:
// 2051 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("less-equal", yymsp[-2].minor.yy259, yymsp[0].minor.yy259, NULL, status->scanner_state);
  yy_destructor(21,&yymsp[-1].minor);
}
// 5482 "parser.c"
        break;
      case 245:
// 2055 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("greater-equal", yymsp[-2].minor.yy259, yymsp[0].minor.yy259, NULL, status->scanner_state);
  yy_destructor(22,&yymsp[-1].minor);
}
// 5490 "parser.c"
        break;
      case 246:
// 2059 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("list", yymsp[-1].minor.yy259, NULL, NULL, status->scanner_state);
  yy_destructor(55,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5499 "parser.c"
        break;
      case 247:
// 2063 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("cast", yymsp[-2].minor.yy259, yymsp[0].minor.yy259, NULL, status->scanner_state);
  yy_destructor(55,&yymsp[-3].minor);
  yy_destructor(36,&yymsp[-1].minor);
}
// 5508 "parser.c"
        break;
      case 248:
// 2067 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("type-hint", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), yymsp[0].minor.yy259, NULL, status->scanner_state);
  yy_destructor(19,&yymsp[-3].minor);
  yy_destructor(20,&yymsp[-1].minor);
}
// 5517 "parser.c"
        break;
      case 249:
// 2071 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("property-access", yymsp[-2].minor.yy259, xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-1].minor);
}
// 5525 "parser.c"
        break;
      case 250:
// 2075 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("property-dynamic-access", yymsp[-4].minor.yy259, xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-3].minor);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 5535 "parser.c"
        break;
      case 251:
// 2079 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("property-string-access", yymsp[-4].minor.yy259, xx_ret_literal(XX_T_STRING, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-3].minor);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 5545 "parser.c"
        break;
      case 252:
// 2083 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("static-property-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(91,&yymsp[-1].minor);
}
// 5553 "parser.c"
        break;
      case 253:
      case 321:
// 2087 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("static-constant-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(91,&yymsp[-1].minor);
}
// 5562 "parser.c"
        break;
      case 254:
// 2096 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("array-access", yymsp[-3].minor.yy259, yymsp[-1].minor.yy259, NULL, status->scanner_state);
  yy_destructor(37,&yymsp[-2].minor);
  yy_destructor(58,&yymsp[0].minor);
}
// 5571 "parser.c"
        break;
      case 255:
// 2101 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("add", yymsp[-2].minor.yy259, yymsp[0].minor.yy259, NULL, status->scanner_state);
  yy_destructor(25,&yymsp[-1].minor);
}
// 5579 "parser.c"
        break;
      case 256:
// 2106 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("sub", yymsp[-2].minor.yy259, yymsp[0].minor.yy259, NULL, status->scanner_state);
  yy_destructor(26,&yymsp[-1].minor);
}
// 5587 "parser.c"
        break;
      case 257:
// 2111 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("mul", yymsp[-2].minor.yy259, yymsp[0].minor.yy259, NULL, status->scanner_state);
  yy_destructor(28,&yymsp[-1].minor);
}
// 5595 "parser.c"
        break;
      case 258:
// 2116 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("div", yymsp[-2].minor.yy259, yymsp[0].minor.yy259, NULL, status->scanner_state);
  yy_destructor(29,&yymsp[-1].minor);
}
// 5603 "parser.c"
        break;
      case 259:
// 2121 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("mod", yymsp[-2].minor.yy259, yymsp[0].minor.yy259, NULL, status->scanner_state);
  yy_destructor(30,&yymsp[-1].minor);
}
// 5611 "parser.c"
        break;
      case 260:
// 2126 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("concat", yymsp[-2].minor.yy259, yymsp[0].minor.yy259, NULL, status->scanner_state);
  yy_destructor(27,&yymsp[-1].minor);
}
// 5619 "parser.c"
        break;
      case 261:
// 2131 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("and", yymsp[-2].minor.yy259, yymsp[0].minor.yy259, NULL, status->scanner_state);
  yy_destructor(13,&yymsp[-1].minor);
}
// 5627 "parser.c"
        break;
      case 262:
// 2136 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("or", yymsp[-2].minor.yy259, yymsp[0].minor.yy259, NULL, status->scanner_state);
  yy_destructor(14,&yymsp[-1].minor);
}
// 5635 "parser.c"
        break;
      case 263:
// 2141 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("instanceof", yymsp[-2].minor.yy259, yymsp[0].minor.yy259, NULL, status->scanner_state);
  yy_destructor(12,&yymsp[-1].minor);
}
// 5643 "parser.c"
        break;
      case 264:
// 2146 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("fetch", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), yymsp[0].minor.yy259, NULL, status->scanner_state);
  yy_destructor(32,&yymsp[-3].minor);
  yy_destructor(6,&yymsp[-1].minor);
}
// 5652 "parser.c"
        break;
      case 266:
// 2156 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("typeof", yymsp[0].minor.yy259, NULL, NULL, status->scanner_state);
  yy_destructor(33,&yymsp[-1].minor);
}
// 5660 "parser.c"
        break;
      case 268:
      case 312:
      case 314:
// 2166 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_literal(XX_T_INTEGER, yymsp[0].minor.yy0, status->scanner_state);
}
// 5669 "parser.c"
        break;
      case 269:
      case 311:
      case 316:
// 2171 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_literal(XX_T_STRING, yymsp[0].minor.yy0, status->scanner_state);
}
// 5678 "parser.c"
        break;
      case 270:
      case 315:
// 2176 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_literal(XX_T_CHAR, yymsp[0].minor.yy0, status->scanner_state);
}
// 5686 "parser.c"
        break;
      case 271:
      case 317:
// 2181 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_literal(XX_T_DOUBLE, yymsp[0].minor.yy0, status->scanner_state);
}
// 5694 "parser.c"
        break;
      case 272:
      case 318:
// 2186 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_literal(XX_T_NULL, NULL, status->scanner_state);
  yy_destructor(101,&yymsp[0].minor);
}
// 5703 "parser.c"
        break;
      case 273:
      case 320:
// 2191 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_literal(XX_T_TRUE, NULL, status->scanner_state);
  yy_destructor(102,&yymsp[0].minor);
}
// 5712 "parser.c"
        break;
      case 274:
      case 319:
// 2196 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_literal(XX_T_FALSE, NULL, status->scanner_state);
  yy_destructor(103,&yymsp[0].minor);
}
// 5721 "parser.c"
        break;
      case 275:
      case 322:
// 2201 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_literal(XX_T_CONSTANT, yymsp[0].minor.yy0, status->scanner_state);
}
// 5729 "parser.c"
        break;
      case 276:
// 2206 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("empty-array", NULL, NULL, NULL, status->scanner_state);
  yy_destructor(37,&yymsp[-1].minor);
  yy_destructor(58,&yymsp[0].minor);
}
// 5738 "parser.c"
        break;
      case 277:
// 2211 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("array", yymsp[-1].minor.yy259, NULL, NULL, status->scanner_state);
  yy_destructor(37,&yymsp[-2].minor);
  yy_destructor(58,&yymsp[0].minor);
}
// 5747 "parser.c"
        break;
      case 278:
// 2216 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_new_instance(0, yymsp[0].minor.yy0, NULL, status->scanner_state);
  yy_destructor(34,&yymsp[-1].minor);
}
// 5755 "parser.c"
        break;
      case 279:
// 2221 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_new_instance(0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(34,&yymsp[-3].minor);
  yy_destructor(55,&yymsp[-1].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5765 "parser.c"
        break;
      case 280:
// 2226 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_new_instance(0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(34,&yymsp[-4].minor);
  yy_destructor(55,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5775 "parser.c"
        break;
      case 281:
// 2231 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_new_instance(1, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(34,&yymsp[-3].minor);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 5785 "parser.c"
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
// 5797 "parser.c"
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
// 5809 "parser.c"
        break;
      case 284:
// 2246 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_fcall(1, yymsp[-3].minor.yy0, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(55,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5818 "parser.c"
        break;
      case 285:
// 2251 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_fcall(1, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(55,&yymsp[-1].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5827 "parser.c"
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
// 5838 "parser.c"
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
// 5849 "parser.c"
        break;
      case 288:
// 2266 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_scall(0, yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(91,&yymsp[-4].minor);
  yy_destructor(55,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5859 "parser.c"
        break;
      case 289:
// 2271 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_scall(0, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(91,&yymsp[-3].minor);
  yy_destructor(55,&yymsp[-1].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5869 "parser.c"
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
// 5881 "parser.c"
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
// 5893 "parser.c"
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
// 5907 "parser.c"
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
// 5921 "parser.c"
        break;
      case 294:
// 2296 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_mcall(1, yymsp[-5].minor.yy259, yymsp[-3].minor.yy0, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(38,&yymsp[-4].minor);
  yy_destructor(55,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5931 "parser.c"
        break;
      case 295:
// 2301 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_mcall(1, yymsp[-4].minor.yy259, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-3].minor);
  yy_destructor(55,&yymsp[-1].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5941 "parser.c"
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
// 5953 "parser.c"
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
// 5965 "parser.c"
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
// 5977 "parser.c"
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
// 5989 "parser.c"
        break;
      case 308:
// 2360 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_array_item(yymsp[-2].minor.yy259, yymsp[0].minor.yy259, status->scanner_state);
  yy_destructor(76,&yymsp[-1].minor);
}
// 5997 "parser.c"
        break;
      case 309:
// 2364 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_array_item(NULL, yymsp[0].minor.yy259, status->scanner_state);
}
// 6004 "parser.c"
        break;
      case 323:
// 2421 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), yymsp[-1].minor.yy259, NULL, status->scanner_state);
  yy_destructor(37,&yymsp[-2].minor);
  yy_destructor(58,&yymsp[0].minor);
}
// 6013 "parser.c"
        break;
      case 324:
// 2426 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("property-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-1].minor);
}
// 6021 "parser.c"
        break;
      case 325:
// 2431 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("property-dynamic-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-3].minor);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 6031 "parser.c"
        break;
      case 326:
// 2436 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("property-string-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_STRING, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-3].minor);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 6041 "parser.c"
        break;
      case 328:
// 2444 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_comment(yymsp[0].minor.yy0, status->scanner_state);
}
// 6048 "parser.c"
        break;
      case 329:
// 2448 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_cblock(yymsp[0].minor.yy0, status->scanner_state);
}
// 6055 "parser.c"
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

// 6131 "parser.c"
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
