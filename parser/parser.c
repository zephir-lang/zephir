/** The author disclaims copyright to this source code.
*/
/* First off, code is include which follows the "include" declaration
** in the input file. */
#include <stdio.h>
// 49 "parser.lemon"


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
#define YYNOCODE 186
#define YYACTIONTYPE unsigned short int
#define xx_TOKENTYPE xx_parser_token*
typedef union {
  xx_TOKENTYPE yy0;
  json_object* yy235;
  int yy371;
} YYMINORTYPE;
#define YYSTACKDEPTH 100
#define xx_ARG_SDECL xx_parser_status *status;
#define xx_ARG_PDECL ,xx_parser_status *status
#define xx_ARG_FETCH xx_parser_status *status = yypParser->status
#define xx_ARG_STORE yypParser->status = status
#define YYNSTATE 690
#define YYNRULE 324
#define YYERRORSYMBOL 104
#define YYERRSYMDT yy371
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
 /*     0 */   215,  224,  227,  218,  221,  565,   35,   31,   55,   34,
 /*    10 */    36,   37,  259, 1015,    1,    2,  689,    4,    5,    6,
 /*    20 */     7,  186,  504,  490,  211,  268,  273,  289,  208,  132,
 /*    30 */   395,  263,  134,  658,  664,  663,  647,  137,  408,  296,
 /*    40 */   686,  657,  613,  136,  282,  130,  253,  119,  119,  188,
 /*    50 */   283,  553,   44,   45,   46,   47,   48,   49,   50,   51,
 /*    60 */    52,   53,   54,  660,  625,  647,  396,  397,  398,  399,
 /*    70 */   400,  613,  268,  417,  418,  598,  119,   83,  518,  167,
 /*    80 */    80,   79,   87,  276,   93,  184,  296,  176,  528,  308,
 /*    90 */   256,  275,  277,  278,  279,  280,  281,  313,  641,  482,
 /*   100 */   491,  500,  503,  494,  497,  506,  512,  509,  642,  515,
 /*   110 */   168,  170,  172,  188,  181,  553,  268,  254,  192,  196,
 /*   120 */   201,  382,  518,  167,  389,  303,  305,  304,  272,  295,
 /*   130 */   296,  558,  528,  485,  451,  466,  470,  479,    3,    4,
 /*   140 */     5,    6,    7,  482,  491,  500,  503,  494,  497,  506,
 /*   150 */   512,  509,  395,  515,  168,  170,  172,  106,  181,  318,
 /*   160 */   432,  107,  192,  196,  201,  382,  496,  629,  389,  268,
 /*   170 */    33,   34,   36,   37,  635,  518,  167,  630,  451,  466,
 /*   180 */   470,  479,  295,  296,  603,  528,  335,  521,  396,  397,
 /*   190 */   398,  399,  400,  239,  241,  243,  482,  491,  500,  503,
 /*   200 */   494,  497,  506,  512,  509,   14,  515,  168,  170,  172,
 /*   210 */    17,  181,  301,  268,  486,  192,  196,  201,  382,  518,
 /*   220 */   167,  389,   79,   87,  519,   64,  641,  296,  607,  528,
 /*   230 */   290,  451,  466,  470,  479,  636,  642,   63,  321,  524,
 /*   240 */   482,  491,  500,  503,  494,  497,  506,  512,  509,  487,
 /*   250 */   515,  168,  170,  172,  659,  181,  662,  663,  647,  192,
 /*   260 */   196,  201,  382,  612,  613,  389,  268,  114,  670,  119,
 /*   270 */   668,  355,  518,  167,  485,  451,  466,  470,  479,  456,
 /*   280 */   296,  569,  528,  420,  374,  452,  457,  303,  305,  304,
 /*   290 */   272,  526,  375,  482,  491,  500,  503,  494,  497,  506,
 /*   300 */   512,  509,   63,  515,  168,  170,  172,  505,  181,   12,
 /*   310 */   268,  274,  192,  196,  201,  382,  518,  167,  389,  303,
 /*   320 */   305,  304,  272,  361,  296,  180,  528,   40,  451,  466,
 /*   330 */   470,  479,   58,  495,  490,  424,   32,  482,  491,  500,
 /*   340 */   503,  494,  497,  506,  512,  509,  295,  515,  168,  170,
 /*   350 */   172,  295,  181,  611,  535,  137,  192,  196,  201,  382,
 /*   360 */   612,  613,  389,  268,   41,  111,  119,  498,  490,  518,
 /*   370 */   167,  288,  451,  466,  470,  479,  359,  296,  601,  528,
 /*   380 */   522,  294,  235,  237,  245,  239,  241,  243,  523,  665,
 /*   390 */   482,  491,  500,  503,  494,  497,  506,  512,  509,   13,
 /*   400 */   515,  168,  170,  172,   76,  181,  376,  268,  467,  192,
 /*   410 */   196,  201,  382,  518,  167,  389,  303,  305,  304,  272,
 /*   420 */   627,  296,  609,  528,  600,  451,  466,  470,  479,  130,
 /*   430 */   384,  119,  604,   56,  482,  491,  500,  503,  494,  497,
 /*   440 */   506,  512,  509,  291,  515,  168,  170,  172,   27,  181,
 /*   450 */    22,  104,  485,  192,  196,  201,  382,   21,  378,  389,
 /*   460 */   268,  119,  119,  676,  687,  679,  518,  167,  130,  451,
 /*   470 */   466,  470,  479,  403,  296,  597,  528,  402,  142,  516,
 /*   480 */   490,  303,  305,  304,  272,  517,  564,  482,  491,  500,
 /*   490 */   503,  494,  497,  506,  512,  509,  295,  515,  168,  170,
 /*   500 */   172,  680,  181,  668,  268,  244,  192,  196,  201,  382,
 /*   510 */   518,  167,  389,  303,  305,  304,  272,  295,  296,  143,
 /*   520 */   528,  485,  451,  466,  470,  479,  329,  185,  556,   63,
 /*   530 */    77,  482,  491,  500,  503,  494,  497,  506,  512,  509,
 /*   540 */   395,  515,  168,  170,  172,  520,  181,  365,  649,   10,
 /*   550 */   192,  196,  201,  382,  493,  635,  389,  268,  650,  599,
 /*   560 */   124,  266,  669,  518,  167,  130,  451,  466,  470,  479,
 /*   570 */   583,  296,  567,  528,  492,  490,  396,  397,  398,  399,
 /*   580 */   400,  119,  510,  490,  482,  491,  500,  503,  494,  497,
 /*   590 */   506,  512,  509,  295,  515,  168,  170,  172,  128,  181,
 /*   600 */   668,  268,  242,  192,  196,  201,  382,  518,  167,  389,
 /*   610 */   303,  305,  304,  272,  299,  296,  595,  528,  485,  451,
 /*   620 */   466,  470,  479,  311,  483,  490,  485,  112,  482,  491,
 /*   630 */   500,  503,  494,  497,  506,  512,  509,  525,  515,  168,
 /*   640 */   170,  172,  109,  181,  501,  490,  357,  192,  196,  201,
 /*   650 */   382,  511,  307,  389,  268,  632,  637,  644,   62,  499,
 /*   660 */   518,  167,  635,  451,  466,  470,  479,  416,  296,  557,
 /*   670 */   528,  405,  412,   59,  527,  303,  305,  304,  272,  419,
 /*   680 */   297,  482,  491,  500,  503,  494,  497,  506,  512,  509,
 /*   690 */   344,  515,  168,  170,  172,  298,  181,  363,  268,  262,
 /*   700 */   192,  196,  201,  382,  518,  167,  389,  303,  305,  304,
 /*   710 */   272,  295,  296,  388,  528,  485,  451,  466,  470,  479,
 /*   720 */   127,  472,  454,  130,  475,  482,  491,  500,  503,  494,
 /*   730 */   497,  506,  512,  509,  118,  515,  168,  170,  172,  287,
 /*   740 */   181,  337,  343,  485,  192,  196,  201,  382,  484,  115,
 /*   750 */   389,  268,  331,  639,  485,  453,  373,  518,  167,  214,
 /*   760 */   451,  466,  470,  479,  456,  296,  552,  528,  507,  490,
 /*   770 */   652,  455,  303,  305,  304,  272,  508,  635,  482,  491,
 /*   780 */   500,  503,  494,  497,  506,  512,  509,  502,  515,  168,
 /*   790 */   170,  172,   94,  181,  638,  268,  240,  192,  196,  201,
 /*   800 */   382,  518,  167,  389,  303,  305,  304,  272,  295,  296,
 /*   810 */   195,  528,  265,  451,  466,  470,  479,  513,  490,  485,
 /*   820 */   354,   63,  482,  491,  500,  503,  494,  497,  506,  512,
 /*   830 */   509,  588,  515,  168,  170,  172,  295,  181,  352,  592,
 /*   840 */   392,  192,  196,  201,  382,  390,  450,  389,  268,  169,
 /*   850 */   690,  571,  514,  271,  518,  167,  478,  451,  466,  470,
 /*   860 */   479,  403,  296,  145,  528,  407,  327,  213,   16,  303,
 /*   870 */   305,  304,  272,  391,  270,  482,  491,  500,  503,  494,
 /*   880 */   497,  506,  512,  509,  536,  515,  168,  170,  172,  537,
 /*   890 */   181,  117,  268,  333,  192,  196,  201,  382,  518,  167,
 /*   900 */   389,  303,  305,  304,  272,   29,  296,  591,  528,  572,
 /*   910 */   451,  466,  470,  479,  474,   39,  269,  576,  473,  482,
 /*   920 */   491,  500,  503,  494,  497,  506,  512,  509,   67,  515,
 /*   930 */   168,  170,  172,  171,  181,   24,  471,   66,  192,  196,
 /*   940 */   201,  382,  465,  548,  389,  268,  550,  174,   81,  463,
 /*   950 */   554,  518,  167,  309,  451,  466,  470,  479,  403,  296,
 /*   960 */   541,  528,  435,   17,  261,  260,  303,  305,  304,  272,
 /*   970 */   461,  459,  482,  491,  500,  503,  494,  497,  506,  512,
 /*   980 */   509,  130,  515,  168,  170,  172,   15,  181,  177,  268,
 /*   990 */   238,  192,  196,  201,  382,  518,  167,  389,  303,  305,
 /*  1000 */   304,  272,  559,  296,  562,  528,  560,  451,  466,  470,
 /*  1010 */   479,  178,  257,   40,  566,  314,  482,  491,  500,  503,
 /*  1020 */   494,  497,  506,  512,  509,  315,  515,  168,  170,  172,
 /*  1030 */   445,  181,  446,  316,  442,  192,  196,  201,  382,   92,
 /*  1040 */    82,  389,  268,  438,  578,  183,  122,  437,  518,  167,
 /*  1050 */   322,  451,  466,  470,  479,  585,  296,  563,  528,   60,
 /*  1060 */   434,   31,   55,   34,   36,   37,  323,  586,   25,  482,
 /*  1070 */   491,  500,  503,  494,  497,  506,  512,  509,  324,  515,
 /*  1080 */   168,  170,  172,  688,  181,  430,  268,  371,  192,  196,
 /*  1090 */   201,  382,  518,  167,  389,  303,  305,  304,  272,  429,
 /*  1100 */   296,  589,  528,  594,  451,  466,  470,  479,  187,  123,
 /*  1110 */   426,   84,  125,  482,  491,  500,  503,  494,  497,  506,
 /*  1120 */   512,  509,  425,  515,  168,  170,  172,  189,  181,   43,
 /*  1130 */   422,  190,  192,  196,  201,  382,   85,  606,  389,  268,
 /*  1140 */   421,  140,  139,  193,  413,  518,  167,  616,  451,  466,
 /*  1150 */   470,  479,  414,  296,  573,  528,   28,  617,   31,   55,
 /*  1160 */    34,   36,   37,  410,  618,  619,  482,  491,  500,  503,
 /*  1170 */   494,  497,  506,  512,  509,  621,  515,  168,  170,  172,
 /*  1180 */   622,  181,  623,  268,  236,  192,  196,  201,  382,  518,
 /*  1190 */   167,  389,  303,  305,  304,  272,  624,  296,  551,  528,
 /*  1200 */   406,  451,  466,  470,  479,  997,  126,   90,  996,  645,
 /*  1210 */   482,  491,  500,  503,  494,  497,  506,  512,  509,  631,
 /*  1220 */   515,  168,  170,  172,  633,  181,  198,  634,  736,  192,
 /*  1230 */   196,  201,  382,  737,  347,  389,  268,  640,  667,  643,
 /*  1240 */   394,  202,  518,  167,  401,  451,  466,  470,  479,  646,
 /*  1250 */   296,  200,  528,  577,  205,   31,   55,   34,   36,   37,
 /*  1260 */   393,  655,  129,  482,  491,  500,  503,  494,  497,  506,
 /*  1270 */   512,  509,  653,  515,  168,  170,  172,  651,  181,  654,
 /*  1280 */   268,  480,  192,  196,  201,  382,  518,  167,  389,  303,
 /*  1290 */   305,  304,  272,  356,  296,  575,  528,  207,  451,  466,
 /*  1300 */   470,  479,  656,   86,   89,  383,  379,  482,  491,  500,
 /*  1310 */   503,  494,  497,  506,  512,  509,  666,  515,  168,  170,
 /*  1320 */   172,  362,  181,   42,  377,  671,  192,  196,  201,  382,
 /*  1330 */   673,  674,  389,  268,  675,  677,  678,  212,   88,  518,
 /*  1340 */   167,  133,  451,  466,  470,  479,  403,  296,  534,  528,
 /*  1350 */   423,  681,  683,  684,  303,  305,  304,  272,   61,  685,
 /*  1360 */   482,  491,  500,  503,  494,  497,  506,  512,  509,    9,
 /*  1370 */   515,  168,  170,  172,  489,  181,  488,  268,  258,  192,
 /*  1380 */   196,  201,  382,  518,  167,  389,  303,  305,  304,  272,
 /*  1390 */   511,  296,  546,  528,  511,  451,  466,  470,  479,  511,
 /*  1400 */   511,  511,  511,  511,  482,  491,  500,  503,  494,  497,
 /*  1410 */   506,  512,  509,  511,  515,  168,  170,  172,  511,  181,
 /*  1420 */   511,  511,  511,  192,  196,  201,  382,  511,  511,  389,
 /*  1430 */   268,  511,  511,  511,  511,  511,  518,  167,  511,  451,
 /*  1440 */   466,  470,  479,  511,  296,  581,  528,  116,  511,   31,
 /*  1450 */    55,   34,   36,   37,  511,  511,  511,  482,  491,  500,
 /*  1460 */   503,  494,  497,  506,  512,  509,  511,  515,  168,  170,
 /*  1470 */   172,  511,  181,  511,  268,  531,  192,  196,  201,  382,
 /*  1480 */   518,  167,  389,  303,  305,  304,  272,  511,  296,  547,
 /*  1490 */   528,  511,  451,  466,  470,  479,  511,  511,  511,  511,
 /*  1500 */   511,  482,  491,  500,  503,  494,  497,  506,  512,  509,
 /*  1510 */   511,  515,  168,  170,  172,  511,  181,  511,  511,  511,
 /*  1520 */   192,  196,  201,  382,  511,  511,  389,  268,  511,  511,
 /*  1530 */   511,  511,  511,  518,  167,  511,  451,  466,  470,  479,
 /*  1540 */   511,  296,  204,  528,  605,  511,   31,   55,   34,   36,
 /*  1550 */    37,  511,  511,  511,  482,  491,  500,  503,  494,  497,
 /*  1560 */   506,  512,  509,  511,  515,  168,  170,  172,  511,  181,
 /*  1570 */   511,  268,  234,  192,  196,  201,  382,  518,  167,  389,
 /*  1580 */   303,  305,  304,  272,  511,  296,  579,  528,  511,  451,
 /*  1590 */   466,  470,  479,  511,  511,  511,  511,  511,  482,  491,
 /*  1600 */   500,  503,  494,  497,  506,  512,  509,  511,  515,  168,
 /*  1610 */   170,  172,  511,  181,  511,  511,  511,  192,  196,  201,
 /*  1620 */   382,  511,  511,  389,  268,   95,   96,   98,   97,   99,
 /*  1630 */   518,  167,  511,  451,  466,  470,  479,  511,  296,    8,
 /*  1640 */   528,  511,   11,  511,  121,  511,  672,  682,  511,  511,
 /*  1650 */   528,  482,  491,  500,  503,  494,  497,  506,  512,  509,
 /*  1660 */   511,  515,  168,  170,  172,  511,  181,  511,  101,  102,
 /*  1670 */   192,  196,  201,  382,  165,  511,  389,  511,  110,  100,
 /*  1680 */    31,   55,   34,   36,   37,  511,  451,  466,  470,  479,
 /*  1690 */   511,  511,  511,  511,  555,  511,   95,   96,   98,   97,
 /*  1700 */    99,  511,  511,  529,  147,  148,  149,  150,  151,  152,
 /*  1710 */   153,  154,  155,  156,  157,  158,  159,  160,  161,  162,
 /*  1720 */   163,  164,  166,  165,  593,  511,   31,   55,   34,   36,
 /*  1730 */    37,  511,  511,  628,  458,  460,  462,  464,  511,  101,
 /*  1740 */   102,  335,  264,  561,  511,  332,  511,  584,  511,  511,
 /*  1750 */   100,  511,  529,  147,  148,  149,  150,  151,  152,  153,
 /*  1760 */   154,  155,  156,  157,  158,  159,  160,  161,  162,  163,
 /*  1770 */   164,  166,  165,  610,  135,  625,  647,  511,  511,  286,
 /*  1780 */   511,  657,  613,  458,  460,  462,  464,  119,  511,  511,
 /*  1790 */   511,  511,  596,  511,   95,   96,   98,   97,   99,  511,
 /*  1800 */   511,  529,  147,  148,  149,  150,  151,  152,  153,  154,
 /*  1810 */   155,  156,  157,  158,  159,  160,  161,  162,  163,  164,
 /*  1820 */   166,  165,  511,  511,   19,  511,  511,  511,  511,   22,
 /*  1830 */   511,  648,  458,  460,  462,  464,  120,  101,  102,  511,
 /*  1840 */   119,  590,  511,  511,  511,  138,  511,  511,  100,  511,
 /*  1850 */   529,  147,  148,  149,  150,  151,  152,  153,  154,  155,
 /*  1860 */   156,  157,  158,  159,  160,  161,  162,  163,  164,  166,
 /*  1870 */   165,  511,  511,  511,  511,  511,  511,  284,  511,  511,
 /*  1880 */   511,  458,  460,  462,  464,  303,  305,  304,  272,  511,
 /*  1890 */   540,  511,  342,  511,  511,  350,  346,  349,  511,  529,
 /*  1900 */   147,  148,  149,  150,  151,  152,  153,  154,  155,  156,
 /*  1910 */   157,  158,  159,  160,  161,  162,  163,  164,  166,  165,
 /*  1920 */   511,  511,  511,  197,  511,  511,  381,  511,  511,  511,
 /*  1930 */   458,  460,  462,  464,  303,  305,  304,  272,  511,  545,
 /*  1940 */   511,   95,   96,   98,   97,   99,  511,  511,  529,  147,
 /*  1950 */   148,  149,  150,  151,  152,  153,  154,  155,  156,  157,
 /*  1960 */   158,  159,  160,  161,  162,  163,  164,  166,  165,  113,
 /*  1970 */   511,  511,  511,  511,  511,   79,   87,  511,   93,  458,
 /*  1980 */   460,  462,  464,  403,  101,  102,  511,  439,  602,  511,
 /*  1990 */   615,  303,  305,  304,  272,  100,  511,  529,  147,  148,
 /*  2000 */   149,  150,  151,  152,  153,  154,  155,  156,  157,  158,
 /*  2010 */   159,  160,  161,  162,  163,  164,  166,  165,  511,  511,
 /*  2020 */   511,  173,  511,  511,  381,  511,  511,  511,  458,  460,
 /*  2030 */   462,  464,  303,  305,  304,  272,  511,  387,  511,   95,
 /*  2040 */    96,   98,   97,   99,  511,  511,  529,  147,  148,  149,
 /*  2050 */   150,  151,  152,  153,  154,  155,  156,  157,  158,  159,
 /*  2060 */   160,  161,  162,  163,  164,  166,  165,   57,  511,  511,
 /*  2070 */   511,  511,  511,   79,   87,  511,   93,  458,  460,  462,
 /*  2080 */   464,  511,  101,  102,  511,  511,  580,  511,  511,  511,
 /*  2090 */    23,  511,  511,  100,  511,  529,  147,  148,  149,  150,
 /*  2100 */   151,  152,  153,  154,  155,  156,  157,  158,  159,  160,
 /*  2110 */   161,  162,  163,  164,  166,  165,  511,  511,  511,  549,
 /*  2120 */   511,  511,  381,  511,  511,  511,  458,  460,  462,  464,
 /*  2130 */   303,  305,  304,  272,  511,  175,  511,   95,   96,   98,
 /*  2140 */    97,   99,  511,  511,  529,  147,  148,  149,  150,  151,
 /*  2150 */   152,  153,  154,  155,  156,  157,  158,  159,  160,  161,
 /*  2160 */   162,  163,  164,  166,  165,  570,  511,  511,  511,  511,
 /*  2170 */   511,   79,   87,  511,   93,  458,  460,  462,  464,  511,
 /*  2180 */   101,  102,  511,  511,  144,  511,  511,  511,  105,  511,
 /*  2190 */   511,  100,  511,  529,  147,  148,  149,  150,  151,  152,
 /*  2200 */   153,  154,  155,  156,  157,  158,  159,  160,  161,  162,
 /*  2210 */   163,  164,  166,  165,  511,  511,  511,  182,  511,  511,
 /*  2220 */   381,  511,  511,  511,  458,  460,  462,  464,  303,  305,
 /*  2230 */   304,  272,  511,  199,  511,   95,   96,   98,   97,   99,
 /*  2240 */   511,  511,  529,  147,  148,  149,  150,  151,  152,  153,
 /*  2250 */   154,  155,  156,  157,  158,  159,  160,  161,  162,  163,
 /*  2260 */   164,  166,  165,  511,  511,  511,  511,  511,  511,  511,
 /*  2270 */   511,  511,  511,  458,  460,  462,  464,  511,  101,  102,
 /*  2280 */   511,  511,  194,  511,  511,  511,  138,  511,  511,  100,
 /*  2290 */   511,  529,  147,  148,  149,  150,  151,  152,  153,  154,
 /*  2300 */   155,  156,  157,  158,  159,  160,  161,  162,  163,  164,
 /*  2310 */   166,  165,  511,  511,  511,  206,  511,  511,  381,  511,
 /*  2320 */   511,  511,  458,  460,  462,  464,  303,  305,  304,  272,
 /*  2330 */   511,  533,  511,   95,   96,   98,   97,   99,  511,  511,
 /*  2340 */   529,  147,  148,  149,  150,  151,  152,  153,  154,  155,
 /*  2350 */   156,  157,  158,  159,  160,  161,  162,  163,  164,  166,
 /*  2360 */   165,  511,  511,  511,  511,  511,  511,  511,  511,  511,
 /*  2370 */   511,  458,  460,  462,  464,  511,  101,  102,  511,  511,
 /*  2380 */   568,  511,  511,  511,  584,  511,  511,  100,  511,  529,
 /*  2390 */   147,  148,  149,  150,  151,  152,  153,  154,  155,  156,
 /*  2400 */   157,  158,  159,  160,  161,  162,  163,  164,  166,  165,
 /*  2410 */   511,  511,  511,  511,  511,  511,  284,  511,  511,  511,
 /*  2420 */   458,  460,  462,  464,  303,  305,  304,  272,  511,  191,
 /*  2430 */   511,  511,  511,  511,  345,  346,  349,  511,  529,  147,
 /*  2440 */   148,  149,  150,  151,  152,  153,  154,  155,  156,  157,
 /*  2450 */   158,  159,  160,  161,  162,  163,  164,  166,  165,  511,
 /*  2460 */   511,  511,  511,  511,  511,  267,  511,  511,  511,  458,
 /*  2470 */   460,  462,  464,  303,  305,  304,  272,  511,  179,  511,
 /*  2480 */   511,  317,  306,  511,  511,  511,  511,  529,  147,  148,
 /*  2490 */   149,  150,  151,  152,  153,  154,  155,  156,  157,  158,
 /*  2500 */   159,  160,  161,  162,  163,  164,  166,  165,  511,  511,
 /*  2510 */   511,  511,  511,  511,  267,  511,  511,  511,  458,  460,
 /*  2520 */   462,  464,  303,  305,  304,  272,  511,  608,  511,  511,
 /*  2530 */   351,  306,  511,  511,  511,  511,  529,  147,  148,  149,
 /*  2540 */   150,  151,  152,  153,  154,  155,  156,  157,  158,  159,
 /*  2550 */   160,  161,  162,  163,  164,  166,  165,  511,  511,  511,
 /*  2560 */   511,  511,  511,  267,  511,  511,  511,  458,  460,  462,
 /*  2570 */   464,  303,  305,  304,  272,  511,  574,  511,  511,  328,
 /*  2580 */   306,  511,  511,  511,  511,  529,  147,  148,  149,  150,
 /*  2590 */   151,  152,  153,  154,  155,  156,  157,  158,  159,  160,
 /*  2600 */   161,  162,  163,  164,  166,  165,  511,  511,  511,  511,
 /*  2610 */   511,  511,  267,  511,  511,  511,  458,  460,  462,  464,
 /*  2620 */   303,  305,  304,  272,  511,  203,  511,  511,  293,  306,
 /*  2630 */   511,  511,  511,  511,  529,  147,  148,  149,  150,  151,
 /*  2640 */   152,  153,  154,  155,  156,  157,  158,  159,  160,  161,
 /*  2650 */   162,  163,  164,  166,  165,  511,  511,  511,  511,  511,
 /*  2660 */   511,  511,  511,  511,  511,  458,  460,  462,  464,  210,
 /*  2670 */   220,  226,  229,  231,  233,  223,  217,  235,  237,  245,
 /*  2680 */   239,  241,  243,  146,  147,  148,  149,  150,  151,  152,
 /*  2690 */   153,  154,  155,  156,  157,  158,  159,  160,  161,  162,
 /*  2700 */   163,  164,  166,  511,  511,  511,  215,  224,  227,  218,
 /*  2710 */   221,  267,  511,  511,  458,  460,  462,  464,  259,  303,
 /*  2720 */   305,  304,  272,  511,  511,  511,  511,  300,  306,  511,
 /*  2730 */   211,  268,  273,  289,  208,  353,  511,  263,   91,  335,
 /*  2740 */   264,  511,  511,  332,  511,  296,  511,  511,  511,  511,
 /*  2750 */   282,  511,  253,  511,  511,   26,  283,   65,  251,  247,
 /*  2760 */   249,  511,  995,  210,  220,  226,  229,  231,  233,  223,
 /*  2770 */   217,  235,  237,  245,  239,  241,  243,  286,   44,   45,
 /*  2780 */    46,   47,   48,   49,   50,   51,   52,   53,   54,  276,
 /*  2790 */   511,  511,  511,  511,  511,  511,  511,  275,  277,  278,
 /*  2800 */   279,  280,  281,  267,  334,  215,  224,  227,  218,  221,
 /*  2810 */   511,  303,  305,  304,  272,  284,  511,  259,  511,  310,
 /*  2820 */   306,  511,  511,  303,  305,  304,  272,  511,  511,  211,
 /*  2830 */   268,  273,  289,  208,  302,  348,  263,   91,  511,  511,
 /*  2840 */   511,  511,  511,  511,  296,  511,  511,  511,  511,  282,
 /*  2850 */   511,  253,  511,  511,  141,  283,   65,  251,  247,  249,
 /*  2860 */   511,  511,  210,  220,  226,  229,  231,  233,  223,  217,
 /*  2870 */   235,  237,  245,  239,  241,  243,  511,   44,   45,   46,
 /*  2880 */    47,   48,   49,   50,   51,   52,   53,   54,  276,  511,
 /*  2890 */   511,  544,  511,  511,  511,  511,  275,  277,  278,  279,
 /*  2900 */   280,  281,  267,  511,  215,  224,  227,  218,  221,  511,
 /*  2910 */   303,  305,  304,  272,  511,  416,  259,  511,  326,  306,
 /*  2920 */   436,  511,  511,  303,  305,  304,  272,  511,  211,  268,
 /*  2930 */   273,  289,  208,  511,  511,  263,  469,  511,  511,  511,
 /*  2940 */   511,  511,  511,  296,  511,  511,  511,  511,  282,  511,
 /*  2950 */   253,  511,  511,  511,  283,  511,  251,  247,  249,  511,
 /*  2960 */   511,  210,  220,  226,  229,  231,  233,  223,  217,  235,
 /*  2970 */   237,  245,  239,  241,  243,  511,   38,  511,  511,  511,
 /*  2980 */   511,  511,  511,  481,  511,  511,  511,  276,  511,   95,
 /*  2990 */    96,   98,   97,   99,  511,  275,  277,  278,  279,  280,
 /*  3000 */   281,  232,  511,  215,  224,  227,  218,  221,  511,  303,
 /*  3010 */   305,  304,  272,   30,  511,  259,   44,   45,   46,   47,
 /*  3020 */    48,   49,   50,   51,   52,   53,   54,  211,  268,  273,
 /*  3030 */   289,  208,  101,  102,  285,  717,  626,  511,  620,  511,
 /*  3040 */   511,  511,  296,  100,  511,  511,  511,  282,  511,  253,
 /*  3050 */   511,  511,  511,  283,  341,  251,  247,  249,  511,  511,
 /*  3060 */   210,  220,  226,  229,  231,  233,  223,  217,  235,  237,
 /*  3070 */   245,  239,  241,  243,  511,   38,  511,  511,  511,  255,
 /*  3080 */   416,  511,  511,  511,  511,  444,  340,  511,  303,  305,
 /*  3090 */   304,  272,  511,  511,  339,  277,  278,  279,  280,  281,
 /*  3100 */   230,  511,  215,  224,  227,  218,  221,  511,  303,  305,
 /*  3110 */   304,  272,  511,  511,  259,   44,   45,   46,   47,   48,
 /*  3120 */    49,   50,   51,   52,   53,   54,  211,  268,  273,  289,
 /*  3130 */   208,  360,  511,  263,   91,  511,  511,  511,  511,  511,
 /*  3140 */   511,  296,  511,  511,  511,  511,  282,  511,  253,  511,
 /*  3150 */   511,  587,  283,   65,  251,  247,  249,  511,  511,  210,
 /*  3160 */   220,  226,  229,  231,  233,  223,  217,  235,  237,  245,
 /*  3170 */   239,  241,  243,  511,   44,   45,   46,   47,   48,   49,
 /*  3180 */    50,   51,   52,   53,   54,  276,  511,  511,  511,  511,
 /*  3190 */   511,  511,  511,  275,  277,  278,  279,  280,  281,  267,
 /*  3200 */   477,  215,  224,  227,  218,  221,  511,  303,  305,  304,
 /*  3210 */   272,  511,  228,  259,  511,  336,  306,  511,  511,  511,
 /*  3220 */   303,  305,  304,  272,  511,  211,  268,  273,  289,  208,
 /*  3230 */   366,  511,  263,   91,  511,  511,  511,  511,  511,  511,
 /*  3240 */   296,  511,  511,  511,  511,  282,  511,  253,  511,  511,
 /*  3250 */   108,  283,   65,  251,  247,  249,  511,  511,  210,  220,
 /*  3260 */   226,  229,  231,  233,  223,  217,  235,  237,  245,  239,
 /*  3270 */   241,  243,  511,   44,   45,   46,   47,   48,   49,   50,
 /*  3280 */    51,   52,   53,   54,  276,  511,  511,  532,  511,  511,
 /*  3290 */   511,  511,  275,  277,  278,  279,  280,  281,  267,  511,
 /*  3300 */   215,  224,  227,  218,  221,  511,  303,  305,  304,  272,
 /*  3310 */   511,  252,  259,  511,  358,  306,  511,  511,  511,  303,
 /*  3320 */   305,  304,  272,  511,  211,  268,  273,  289,  208,  511,
 /*  3330 */   511,  263,  267,  511,  511,  511,  511,  511,  511,  296,
 /*  3340 */   303,  305,  304,  272,  282,  511,  253,  511,  364,  306,
 /*  3350 */   283,  511,  251,  247,  249,  511,  511,  210,  220,  226,
 /*  3360 */   229,  231,  233,  223,  217,  235,  237,  245,  239,  241,
 /*  3370 */   243,  511,  511,  511,  511,  511,  542,  395,  511,  468,
 /*  3380 */   511,  511,  404,  276,  511,  440,  511,  511,  511,  511,
 /*  3390 */   511,  275,  277,  278,  279,  280,  281,  476,  511,  215,
 /*  3400 */   224,  227,  218,  221,  511,  303,  305,  304,  272,  511,
 /*  3410 */   511,  259,  511,  396,  397,  398,  399,  400,  511,  428,
 /*  3420 */   448,  449,  511,  211,  268,  273,  289,  208,  312,  511,
 /*  3430 */   263,   91,  511,  511,  511,  511,  511,  511,  296,  511,
 /*  3440 */   511,  511,  511,  282,  511,  253,  511,  511,  511,  283,
 /*  3450 */    65,  251,  247,  249,  511,  511,  210,  220,  226,  229,
 /*  3460 */   231,  233,  223,  217,  235,  237,  245,  239,  241,  243,
 /*  3470 */   511,   44,   45,   46,   47,   48,   49,   50,   51,   52,
 /*  3480 */    53,   54,  276,  511,   95,   96,   98,   97,   99,  511,
 /*  3490 */   275,  277,  278,  279,  280,  281,  267,  372,  215,  224,
 /*  3500 */   227,  218,  221,  511,  303,  305,  304,  272,  403,  511,
 /*  3510 */   259,  511,  427,  320,  511,  511,  303,  305,  304,  272,
 /*  3520 */   511,  511,  211,  268,  273,  289,  208,  101,  102,  263,
 /*  3530 */   718,  626,  691,  620,  511,  511,  511,  296,  100,  511,
 /*  3540 */   511,  511,  282,  511,  253,  511,  511,  511,  283,  433,
 /*  3550 */   251,  247,  249,  511,  511,  210,  220,  226,  229,  231,
 /*  3560 */   233,  223,  217,  235,  237,  245,  239,  241,  243,    8,
 /*  3570 */   511,  511,   11,  511,  121,  511,  672,  682,  511,  511,
 /*  3580 */   528,  276,  511,  511,  539,  511,  511,  511,  511,  275,
 /*  3590 */   277,  278,  279,  280,  281,  367,  511,  215,  224,  227,
 /*  3600 */   218,  221,  511,  303,  305,  304,  272,  511,  403,  259,
 /*  3610 */   511,  511,  415,  511,  511,  511,  303,  305,  304,  272,
 /*  3620 */   511,  211,  268,  273,  289,  208,  325,  511,  263,  250,
 /*  3630 */    95,   96,   98,   97,   99,  511,  296,  303,  305,  304,
 /*  3640 */   272,  282,  511,  253,  511,  511,  511,  283,  511,  251,
 /*  3650 */   247,  249,  511,  511,  210,  220,  226,  229,  231,  233,
 /*  3660 */   223,  217,  235,  237,  245,  239,  241,  243,  511,  511,
 /*  3670 */   511,  511,  511,  101,  102,  511,  131,  626,  511,  620,
 /*  3680 */   276,  511,  511,  511,  100,  511,  511,  511,  275,  277,
 /*  3690 */   278,  279,  280,  281,  511,  887,  215,  224,  227,  218,
 /*  3700 */   221,  511,  511,  403,  511,  511,  511,  443,  259,  511,
 /*  3710 */   511,  303,  305,  304,  272,  511,  511,  511,  511,  511,
 /*  3720 */   211,  268,  273,  289,  208,  292,  511,  263,  216,   95,
 /*  3730 */    96,   98,   97,   99,  511,  296,  303,  305,  304,  272,
 /*  3740 */   282,  511,  253,  511,  511,  511,  283,  511,  251,  247,
 /*  3750 */   249,  511,  511,  210,  220,  226,  229,  231,  233,  223,
 /*  3760 */   217,  235,  237,  245,  239,  241,  243,  511,  511,  511,
 /*  3770 */   511,  511,  101,  102,  511,  721,  614,  511,  620,  276,
 /*  3780 */   511,  511,  386,  100,  511,  511,  511,  275,  277,  278,
 /*  3790 */   279,  280,  281,  511,  225,  215,  224,  227,  218,  221,
 /*  3800 */   511,  511,  303,  305,  304,  272,  403,  259,  511,  511,
 /*  3810 */   447,  511,  511,  511,  303,  305,  304,  272,  511,  211,
 /*  3820 */   268,  273,  289,  208,  511,  511,  263,  368,   95,   96,
 /*  3830 */    98,   97,   99,  511,  296,  303,  305,  304,  272,  282,
 /*  3840 */   511,  253,  511,  511,  511,  283,  409,  251,  247,  249,
 /*  3850 */   511,  511,  210,  220,  226,  229,  231,  233,  223,  217,
 /*  3860 */   235,  237,  245,  239,  241,  243,  511,  511,  511,  511,
 /*  3870 */   511,  101,  102,  511,  722,  661,  511,  511,  276,  511,
 /*  3880 */   511,  511,  100,  511,  511,  511,  275,  277,  278,  279,
 /*  3890 */   280,  281,  511,  385,  215,  224,  227,  218,  221,  511,
 /*  3900 */   511,  303,  305,  304,  272,  403,  259,  511,  511,  411,
 /*  3910 */   511,  511,  511,  303,  305,  304,  272,  511,  211,  268,
 /*  3920 */   273,  289,  208,  319,  511,  263,  222,   95,   96,   98,
 /*  3930 */    97,   99,  511,  296,  303,  305,  304,  272,  282,  511,
 /*  3940 */   253,  511,  511,  511,  283,  511,  511,  247,  249,  511,
 /*  3950 */   511,  210,  220,  226,  229,  231,  233,  223,  217,  235,
 /*  3960 */   237,  245,  239,  241,  243,  511,  511,  511,  511,  511,
 /*  3970 */   101,  102,  511,  724,  582,  511,  511,  276,  511,  511,
 /*  3980 */   511,  100,  511,  511,  511,  275,  277,  278,  279,  280,
 /*  3990 */   281,  511,  369,  215,  224,  227,  218,  221,  511,  511,
 /*  4000 */   303,  305,  304,  272,  511,  259,  511,   95,   96,   98,
 /*  4010 */    97,   99,   95,   96,   98,   97,   99,  211,  268,  273,
 /*  4020 */   289,  208,  511,  511,  263,  209,   95,   96,   98,   97,
 /*  4030 */    99,  511,  296,  303,  305,  304,  272,  282,  511,  253,
 /*  4040 */   511,  511,  511,  283,  441,  511,  511,  511,  511,  511,
 /*  4050 */   101,  102,  511,   18,  103,  101,  102,  511,  720,  582,
 /*  4060 */   511,  100,  511,  511,  511,  511,  100,  511,  511,  101,
 /*  4070 */   102,  511,  725,  582,  511,  511,  276,  511,  511,  511,
 /*  4080 */   100,  511,  511,  511,  275,  277,  278,  279,  280,  281,
 /*  4090 */   511,  511,  215,  224,  227,  218,  221,   95,   96,   98,
 /*  4100 */    97,   99,  511,  511,  259,  511,   95,   96,   98,   97,
 /*  4110 */    99,   95,   96,   98,   97,   99,  211,  268,  273,  289,
 /*  4120 */   208,  338,  511,  263,  403,  511,  511,  511,  431,  511,
 /*  4130 */   511,  296,  303,  305,  304,  272,  282,  511,  253,  511,
 /*  4140 */   101,  102,  283,   20,  103,  511,   75,  511,  511,  101,
 /*  4150 */   102,  100,  723,  582,  101,  102,  511,  719,  582,   78,
 /*  4160 */   100,  219,  511,  511,  511,  100,  511,  511,  248,  303,
 /*  4170 */   305,  304,  272,  511,  511,  276,  303,  305,  304,  272,
 /*  4180 */   511,  511,  511,  275,  277,  278,  279,  280,  281,  511,
 /*  4190 */   511,  215,  224,  227,  218,  221,  511,  511,   70,  511,
 /*  4200 */   511,  511,  511,  259,  511,  511,   68,   69,   71,   72,
 /*  4210 */    74,   73,  511,  511,  511,  211,  268,  273,  289,  208,
 /*  4220 */   330,  511,  263,  538,  511,  511,  511,  511,  511,  511,
 /*  4230 */   296,  303,  305,  304,  272,  282,  511,  253,  380,  511,
 /*  4240 */   511,  283,  511,  511,  511,  370,  303,  305,  304,  272,
 /*  4250 */   246,  511,  511,  303,  305,  304,  272,  543,  303,  305,
 /*  4260 */   304,  272,  511,  511,  511,  303,  305,  304,  272,  511,
 /*  4270 */   511,  511,  511,  511,  276,  511,  511,  511,  511,  511,
 /*  4280 */   511,  511,  275,  277,  278,  279,  280,  281,  511,  511,
 /*  4290 */   215,  224,  227,  218,  221,  511,  511,  511,  511,  511,
 /*  4300 */   511,  511,  259,  511,  511,  511,  511,  511,  511,  511,
 /*  4310 */   511,  511,  511,  511,  211,  268,  273,  289,  208,  511,
 /*  4320 */   511,  263,  511,  511,  511,  511,  511,  511,  511,  296,
 /*  4330 */   511,  511,  511,  511,  282,  511,  253,  511,  511,  511,
 /*  4340 */   283,  511,  511,  511,  511,  511,  511,  511,  511,  511,
 /*  4350 */   511,  511,  511,  511,  511,  511,  511,  511,  511,  511,
 /*  4360 */   511,  511,  511,  511,  511,  511,  530,  511,  511,  511,
 /*  4370 */   511,  511,  511,  276,  511,  511,  511,  511,  511,  511,
 /*  4380 */   511,  275,  277,  278,  279,  280,  281,  511,  511,  215,
 /*  4390 */   224,  227,  218,  221,  511,  511,  511,  511,  511,  511,
 /*  4400 */   511,  259,  511,  511,  511,  511,  511,  511,  511,  511,
 /*  4410 */   511,  511,  511,  211,  268,  273,  289,  208,  511,  511,
 /*  4420 */   263,  511,  511,  511,  511,  511,  511,  511,  296,  511,
 /*  4430 */   511,  511,  511,  282,  511,  253,  511,  511,  511,  283,
 /*  4440 */   511,  511,  511,  511,  511,  511,  511,  511,  511,  511,
 /*  4450 */   511,  511,  511,  511,  511,  511,  511,  511,  511,  511,
 /*  4460 */   511,  511,  511,  511,  511,  511,  511,  511,  511,  511,
 /*  4470 */   511,  511,  276,  511,  511,  511,  511,  511,  511,  511,
 /*  4480 */   275,  277,  278,  279,  280,  281,  511,  511,  215,  224,
 /*  4490 */   227,  218,  221,  511,  511,  511,  511,  511,  511,  511,
 /*  4500 */   259,  511,  511,  511,  511,  511,  511,  511,  511,  511,
 /*  4510 */   511,  511,  211,  268,  273,  289,  208,  511,  511,  285,
 /*  4520 */   511,  511,  511,  511,  511,  511,  511,  296,  511,  511,
 /*  4530 */   511,  511,  282,  511,  253,  511,  511,  511,  283,  511,
 /*  4540 */   511,  511,  511,  511,  511,  511,  511,  511,  511,  511,
 /*  4550 */   511,  511,  511,  511,  511,  511,  511,  511,  511,  511,
 /*  4560 */   511,  511,  511,  511,  511,  511,  511,  511,  511,  511,
 /*  4570 */   511,  340,  511,  511,  511,  511,  511,  511,  511,  339,
 /*  4580 */   277,  278,  279,  280,  281,
};
static YYCODETYPE yy_lookahead[] = {
 /*     0 */     7,    8,    9,   10,   11,  133,   35,  135,  136,  137,
 /*    10 */   138,  139,   19,  105,  106,  107,  108,  109,  110,  111,
 /*    20 */   112,   47,  176,  177,   31,   32,   33,   34,   35,  116,
 /*    30 */    49,   38,  119,  120,  121,  122,  123,  123,   57,   46,
 /*    40 */    41,  128,  129,  129,   51,   46,   53,  134,  134,   75,
 /*    50 */    57,   77,   59,   60,   61,   62,   63,   64,   65,   66,
 /*    60 */    67,   68,   69,  121,  122,  123,   85,   86,   87,   88,
 /*    70 */    89,  129,   32,   92,   93,  131,  134,   35,   38,   39,
 /*    80 */    38,  137,  138,   90,  140,   47,   46,   47,   48,   38,
 /*    90 */   137,   98,   99,  100,  101,  102,  103,   46,   38,   59,
 /*   100 */    60,   61,   62,   63,   64,   65,   66,   67,   48,   69,
 /*   110 */    70,   71,   72,   75,   74,   77,   32,  164,   78,   79,
 /*   120 */    80,   81,   38,   39,   84,  172,  173,  174,  175,    6,
 /*   130 */    46,   47,   48,    6,   94,   95,   96,   97,  108,  109,
 /*   140 */   110,  111,  112,   59,   60,   61,   62,   63,   64,   65,
 /*   150 */    66,   67,   49,   69,   70,   71,   72,   38,   74,   36,
 /*   160 */    57,   53,   78,   79,   80,   81,   39,   39,   84,   32,
 /*   170 */   136,  137,  138,  139,   46,   38,   39,   49,   94,   95,
 /*   180 */    96,   97,    6,   46,   47,   48,   53,   54,   85,   86,
 /*   190 */    87,   88,   89,   28,   29,   30,   59,   60,   61,   62,
 /*   200 */    63,   64,   65,   66,   67,   41,   69,   70,   71,   72,
 /*   210 */    46,   74,   36,   32,  177,   78,   79,   80,   81,   38,
 /*   220 */    39,   84,  137,  138,   91,  140,   38,   46,   47,   48,
 /*   230 */    38,   94,   95,   96,   97,   47,   48,    6,   46,   38,
 /*   240 */    59,   60,   61,   62,   63,   64,   65,   66,   67,   38,
 /*   250 */    69,   70,   71,   72,  119,   74,  121,  122,  123,   78,
 /*   260 */    79,   80,   81,  128,  129,   84,   32,   36,  115,  134,
 /*   270 */   117,   38,   38,   39,    6,   94,   95,   96,   97,  164,
 /*   280 */    46,   47,   48,   38,   38,  170,  171,  172,  173,  174,
 /*   290 */   175,   90,   46,   59,   60,   61,   62,   63,   64,   65,
 /*   300 */    66,   67,    6,   69,   70,   71,   72,   39,   74,   38,
 /*   310 */    32,  164,   78,   79,   80,   81,   38,   39,   84,  172,
 /*   320 */   173,  174,  175,   90,   46,   47,   48,   20,   94,   95,
 /*   330 */    96,   97,   36,  176,  177,   90,   15,   59,   60,   61,
 /*   340 */    62,   63,   64,   65,   66,   67,    6,   69,   70,   71,
 /*   350 */    72,    6,   74,  121,    6,  123,   78,   79,   80,   81,
 /*   360 */   128,  129,   84,   32,   57,   39,  134,  176,  177,   38,
 /*   370 */    39,   53,   94,   95,   96,   97,   36,   46,   47,   48,
 /*   380 */    38,   36,   25,   26,   27,   28,   29,   30,   46,    6,
 /*   390 */    59,   60,   61,   62,   63,   64,   65,   66,   67,  113,
 /*   400 */    69,   70,   71,   72,   91,   74,   38,   32,  164,   78,
 /*   410 */    79,   80,   81,   38,   39,   84,  172,  173,  174,  175,
 /*   420 */   123,   46,   47,   48,   46,   94,   95,   96,   97,   46,
 /*   430 */    82,  134,   54,   39,   59,   60,   61,   62,   63,   64,
 /*   440 */    65,   66,   67,   53,   69,   70,   71,   72,   54,   74,
 /*   450 */   123,  123,    6,   78,   79,   80,   81,  130,   90,   84,
 /*   460 */    32,  134,  134,   41,   38,   43,   38,   39,   46,   94,
 /*   470 */    95,   96,   97,  164,   46,   47,   48,  168,   46,  176,
 /*   480 */   177,  172,  173,  174,  175,   39,   54,   59,   60,   61,
 /*   490 */    62,   63,   64,   65,   66,   67,    6,   69,   70,   71,
 /*   500 */    72,  115,   74,  117,   32,  164,   78,   79,   80,   81,
 /*   510 */    38,   39,   84,  172,  173,  174,  175,    6,   46,   47,
 /*   520 */    48,    6,   94,   95,   96,   97,   36,  162,  163,    6,
 /*   530 */    51,   59,   60,   61,   62,   63,   64,   65,   66,   67,
 /*   540 */    49,   69,   70,   71,   72,   38,   74,   36,   39,   39,
 /*   550 */    78,   79,   80,   81,   39,   46,   84,   32,   49,   36,
 /*   560 */    41,   53,   43,   38,   39,   46,   94,   95,   96,   97,
 /*   570 */   123,   46,   47,   48,  176,  177,   85,   86,   87,   88,
 /*   580 */    89,  134,  176,  177,   59,   60,   61,   62,   63,   64,
 /*   590 */    65,   66,   67,    6,   69,   70,   71,   72,  115,   74,
 /*   600 */   117,   32,  164,   78,   79,   80,   81,   38,   39,   84,
 /*   610 */   172,  173,  174,  175,   53,   46,   47,   48,    6,   94,
 /*   620 */    95,   96,   97,   36,  176,  177,    6,   39,   59,   60,
 /*   630 */    61,   62,   63,   64,   65,   66,   67,   47,   69,   70,
 /*   640 */    71,   72,   54,   74,  176,  177,   53,   78,   79,   80,
 /*   650 */    81,   39,   91,   84,   32,   39,  126,  127,   39,   39,
 /*   660 */    38,   39,   46,   94,   95,   96,   97,  164,   46,   47,
 /*   670 */    48,   38,  169,   54,   47,  172,  173,  174,  175,   46,
 /*   680 */    38,   59,   60,   61,   62,   63,   64,   65,   66,   67,
 /*   690 */     6,   69,   70,   71,   72,   47,   74,   53,   32,  164,
 /*   700 */    78,   79,   80,   81,   38,   39,   84,  172,  173,  174,
 /*   710 */   175,    6,   46,   47,   48,    6,   94,   95,   96,   97,
 /*   720 */    43,   54,    6,   46,   57,   59,   60,   61,   62,   63,
 /*   730 */    64,   65,   66,   67,   39,   69,   70,   71,   72,   38,
 /*   740 */    74,   36,   58,    6,   78,   79,   80,   81,   39,   54,
 /*   750 */    84,   32,   51,    6,    6,   39,   54,   38,   39,   57,
 /*   760 */    94,   95,   96,   97,  164,   46,   47,   48,  176,  177,
 /*   770 */    39,  171,  172,  173,  174,  175,   39,   46,   59,   60,
 /*   780 */    61,   62,   63,   64,   65,   66,   67,   39,   69,   70,
 /*   790 */    71,   72,  134,   74,   47,   32,  164,   78,   79,   80,
 /*   800 */    81,   38,   39,   84,  172,  173,  174,  175,    6,   46,
 /*   810 */    47,   48,   38,   94,   95,   96,   97,  176,  177,    6,
 /*   820 */    46,    6,   59,   60,   61,   62,   63,   64,   65,   66,
 /*   830 */    67,   46,   69,   70,   71,   72,    6,   74,   36,   54,
 /*   840 */     6,   78,   79,   80,   81,  165,  166,   84,   32,   39,
 /*   850 */     0,   36,   39,  178,   38,   39,   39,   94,   95,   96,
 /*   860 */    97,  164,   46,   47,   48,  168,   36,   38,  113,  172,
 /*   870 */   173,  174,  175,   39,    6,   59,   60,   61,   62,   63,
 /*   880 */    64,   65,   66,   67,   38,   69,   70,   71,   72,   82,
 /*   890 */    74,   39,   32,  164,   78,   79,   80,   81,   38,   39,
 /*   900 */    84,  172,  173,  174,  175,   39,   46,   47,   48,   46,
 /*   910 */    94,   95,   96,   97,   39,   38,   38,   54,   38,   59,
 /*   920 */    60,   61,   62,   63,   64,   65,   66,   67,  124,   69,
 /*   930 */    70,   71,   72,   39,   74,   38,   38,   49,   78,   79,
 /*   940 */    80,   81,   39,   79,   84,   32,   39,   46,   49,   39,
 /*   950 */    76,   38,   39,   53,   94,   95,   96,   97,  164,   46,
 /*   960 */    47,   48,  168,   46,   20,   38,  172,  173,  174,  175,
 /*   970 */    39,   39,   59,   60,   61,   62,   63,   64,   65,   66,
 /*   980 */    67,   46,   69,   70,   71,   72,   38,   74,   73,   32,
 /*   990 */   164,   78,   79,   80,   81,   38,   39,   84,  172,  173,
 /*  1000 */   174,  175,   73,   46,   47,   48,   46,   94,   95,   96,
 /*  1010 */    97,   46,   36,   20,   46,   38,   59,   60,   61,   62,
 /*  1020 */    63,   64,   65,   66,   67,   47,   69,   70,   71,   72,
 /*  1030 */    58,   74,  167,   53,  167,   78,   79,   80,   81,   38,
 /*  1040 */   124,   84,   32,  167,   46,   46,   38,   58,   38,   39,
 /*  1050 */    38,   94,   95,   96,   97,   38,   46,   47,   48,  133,
 /*  1060 */   167,  135,  136,  137,  138,  139,   47,   53,   53,   59,
 /*  1070 */    60,   61,   62,   63,   64,   65,   66,   67,   53,   69,
 /*  1080 */    70,   71,   72,  114,   74,  167,   32,  164,   78,   79,
 /*  1090 */    80,   81,   38,   39,   84,  172,  173,  174,  175,   38,
 /*  1100 */    46,   47,   48,   46,   94,   95,   96,   97,  163,  114,
 /*  1110 */   167,   38,   38,   59,   60,   61,   62,   63,   64,   65,
 /*  1120 */    66,   67,   47,   69,   70,   71,   72,  124,   74,   20,
 /*  1130 */   167,   76,   78,   79,   80,   81,   49,   46,   84,   32,
 /*  1140 */    47,   53,   38,   46,   58,   38,   39,   51,   94,   95,
 /*  1150 */    96,   97,  167,   46,   47,   48,  133,   49,  135,  136,
 /*  1160 */   137,  138,  139,  167,  124,   39,   59,   60,   61,   62,
 /*  1170 */    63,   64,   65,   66,   67,   51,   69,   70,   71,   72,
 /*  1180 */    49,   74,  124,   32,  164,   78,   79,   80,   81,   38,
 /*  1190 */    39,   84,  172,  173,  174,  175,   39,   46,   47,   48,
 /*  1200 */   167,   94,   95,   96,   97,   76,  114,  124,   76,  125,
 /*  1210 */    59,   60,   61,   62,   63,   64,   65,   66,   67,  124,
 /*  1220 */    69,   70,   71,   72,  125,   74,   46,   39,   39,   78,
 /*  1230 */    79,   80,   81,   39,   76,   84,   32,  127,   38,   38,
 /*  1240 */    38,   46,   38,   39,  167,   94,   95,   96,   97,   39,
 /*  1250 */    46,   47,   48,  133,   79,  135,  136,  137,  138,  139,
 /*  1260 */   166,  125,  114,   59,   60,   61,   62,   63,   64,   65,
 /*  1270 */    66,   67,  125,   69,   70,   71,   72,  124,   74,   39,
 /*  1280 */    32,  164,   78,   79,   80,   81,   38,   39,   84,  172,
 /*  1290 */   173,  174,  175,   47,   46,   47,   48,   39,   94,   95,
 /*  1300 */    96,   97,   39,  124,   49,   38,   47,   59,   60,   61,
 /*  1310 */    62,   63,   64,   65,   66,   67,  117,   69,   70,   71,
 /*  1320 */    72,   47,   74,   58,   47,  114,   78,   79,   80,   81,
 /*  1330 */    42,   38,   84,   32,  114,   38,  114,  178,   38,   38,
 /*  1340 */    39,   47,   94,   95,   96,   97,  164,   46,   47,   48,
 /*  1350 */   168,  114,   42,   38,  172,  173,  174,  175,   39,  114,
 /*  1360 */    59,   60,   61,   62,   63,   64,   65,   66,   67,   38,
 /*  1370 */    69,   70,   71,   72,  124,   74,   49,   32,  164,   78,
 /*  1380 */    79,   80,   81,   38,   39,   84,  172,  173,  174,  175,
 /*  1390 */   185,   46,   47,   48,  185,   94,   95,   96,   97,  185,
 /*  1400 */   185,  185,  185,  185,   59,   60,   61,   62,   63,   64,
 /*  1410 */    65,   66,   67,  185,   69,   70,   71,   72,  185,   74,
 /*  1420 */   185,  185,  185,   78,   79,   80,   81,  185,  185,   84,
 /*  1430 */    32,  185,  185,  185,  185,  185,   38,   39,  185,   94,
 /*  1440 */    95,   96,   97,  185,   46,   47,   48,  133,  185,  135,
 /*  1450 */   136,  137,  138,  139,  185,  185,  185,   59,   60,   61,
 /*  1460 */    62,   63,   64,   65,   66,   67,  185,   69,   70,   71,
 /*  1470 */    72,  185,   74,  185,   32,  164,   78,   79,   80,   81,
 /*  1480 */    38,   39,   84,  172,  173,  174,  175,  185,   46,   47,
 /*  1490 */    48,  185,   94,   95,   96,   97,  185,  185,  185,  185,
 /*  1500 */   185,   59,   60,   61,   62,   63,   64,   65,   66,   67,
 /*  1510 */   185,   69,   70,   71,   72,  185,   74,  185,  185,  185,
 /*  1520 */    78,   79,   80,   81,  185,  185,   84,   32,  185,  185,
 /*  1530 */   185,  185,  185,   38,   39,  185,   94,   95,   96,   97,
 /*  1540 */   185,   46,   47,   48,  133,  185,  135,  136,  137,  138,
 /*  1550 */   139,  185,  185,  185,   59,   60,   61,   62,   63,   64,
 /*  1560 */    65,   66,   67,  185,   69,   70,   71,   72,  185,   74,
 /*  1570 */   185,   32,  164,   78,   79,   80,   81,   38,   39,   84,
 /*  1580 */   172,  173,  174,  175,  185,   46,   47,   48,  185,   94,
 /*  1590 */    95,   96,   97,  185,  185,  185,  185,  185,   59,   60,
 /*  1600 */    61,   62,   63,   64,   65,   66,   67,  185,   69,   70,
 /*  1610 */    71,   72,  185,   74,  185,  185,  185,   78,   79,   80,
 /*  1620 */    81,  185,  185,   84,   32,    1,    2,    3,    4,    5,
 /*  1630 */    38,   39,  185,   94,   95,   96,   97,  185,   46,   37,
 /*  1640 */    48,  185,   40,  185,   42,  185,   44,   45,  185,  185,
 /*  1650 */    48,   59,   60,   61,   62,   63,   64,   65,   66,   67,
 /*  1660 */   185,   69,   70,   71,   72,  185,   74,  185,   44,   45,
 /*  1670 */    78,   79,   80,   81,  112,  185,   84,  185,  133,   55,
 /*  1680 */   135,  136,  137,  138,  139,  185,   94,   95,   96,   97,
 /*  1690 */   185,  185,  185,  185,  132,  185,    1,    2,    3,    4,
 /*  1700 */     5,  185,  185,  141,  142,  143,  144,  145,  146,  147,
 /*  1710 */   148,  149,  150,  151,  152,  153,  154,  155,  156,  157,
 /*  1720 */   158,  159,  160,  112,  133,  185,  135,  136,  137,  138,
 /*  1730 */   139,  185,  185,   38,  172,  173,  174,  175,  185,   44,
 /*  1740 */    45,   53,   54,  132,  185,   57,  185,   52,  185,  185,
 /*  1750 */    55,  185,  141,  142,  143,  144,  145,  146,  147,  148,
 /*  1760 */   149,  150,  151,  152,  153,  154,  155,  156,  157,  158,
 /*  1770 */   159,  160,  112,  120,  121,  122,  123,  185,  185,   91,
 /*  1780 */   185,  128,  129,  172,  173,  174,  175,  134,  185,  185,
 /*  1790 */   185,  185,  132,  185,    1,    2,    3,    4,    5,  185,
 /*  1800 */   185,  141,  142,  143,  144,  145,  146,  147,  148,  149,
 /*  1810 */   150,  151,  152,  153,  154,  155,  156,  157,  158,  159,
 /*  1820 */   160,  112,  185,  185,  118,  185,  185,  185,  185,  123,
 /*  1830 */   185,   38,  172,  173,  174,  175,  130,   44,   45,  185,
 /*  1840 */   134,  132,  185,  185,  185,   52,  185,  185,   55,  185,
 /*  1850 */   141,  142,  143,  144,  145,  146,  147,  148,  149,  150,
 /*  1860 */   151,  152,  153,  154,  155,  156,  157,  158,  159,  160,
 /*  1870 */   112,  185,  185,  185,  185,  185,  185,  164,  185,  185,
 /*  1880 */   185,  172,  173,  174,  175,  172,  173,  174,  175,  185,
 /*  1890 */   132,  185,  179,  185,  185,  182,  183,  184,  185,  141,
 /*  1900 */   142,  143,  144,  145,  146,  147,  148,  149,  150,  151,
 /*  1910 */   152,  153,  154,  155,  156,  157,  158,  159,  160,  112,
 /*  1920 */   185,  185,  185,  161,  185,  185,  164,  185,  185,  185,
 /*  1930 */   172,  173,  174,  175,  172,  173,  174,  175,  185,  132,
 /*  1940 */   185,    1,    2,    3,    4,    5,  185,  185,  141,  142,
 /*  1950 */   143,  144,  145,  146,  147,  148,  149,  150,  151,  152,
 /*  1960 */   153,  154,  155,  156,  157,  158,  159,  160,  112,  131,
 /*  1970 */   185,  185,  185,  185,  185,  137,  138,  185,  140,  172,
 /*  1980 */   173,  174,  175,  164,   44,   45,  185,  168,  132,  185,
 /*  1990 */    50,  172,  173,  174,  175,   55,  185,  141,  142,  143,
 /*  2000 */   144,  145,  146,  147,  148,  149,  150,  151,  152,  153,
 /*  2010 */   154,  155,  156,  157,  158,  159,  160,  112,  185,  185,
 /*  2020 */   185,  161,  185,  185,  164,  185,  185,  185,  172,  173,
 /*  2030 */   174,  175,  172,  173,  174,  175,  185,  132,  185,    1,
 /*  2040 */     2,    3,    4,    5,  185,  185,  141,  142,  143,  144,
 /*  2050 */   145,  146,  147,  148,  149,  150,  151,  152,  153,  154,
 /*  2060 */   155,  156,  157,  158,  159,  160,  112,  131,  185,  185,
 /*  2070 */   185,  185,  185,  137,  138,  185,  140,  172,  173,  174,
 /*  2080 */   175,  185,   44,   45,  185,  185,  132,  185,  185,  185,
 /*  2090 */    52,  185,  185,   55,  185,  141,  142,  143,  144,  145,
 /*  2100 */   146,  147,  148,  149,  150,  151,  152,  153,  154,  155,
 /*  2110 */   156,  157,  158,  159,  160,  112,  185,  185,  185,  161,
 /*  2120 */   185,  185,  164,  185,  185,  185,  172,  173,  174,  175,
 /*  2130 */   172,  173,  174,  175,  185,  132,  185,    1,    2,    3,
 /*  2140 */     4,    5,  185,  185,  141,  142,  143,  144,  145,  146,
 /*  2150 */   147,  148,  149,  150,  151,  152,  153,  154,  155,  156,
 /*  2160 */   157,  158,  159,  160,  112,  131,  185,  185,  185,  185,
 /*  2170 */   185,  137,  138,  185,  140,  172,  173,  174,  175,  185,
 /*  2180 */    44,   45,  185,  185,  132,  185,  185,  185,   52,  185,
 /*  2190 */   185,   55,  185,  141,  142,  143,  144,  145,  146,  147,
 /*  2200 */   148,  149,  150,  151,  152,  153,  154,  155,  156,  157,
 /*  2210 */   158,  159,  160,  112,  185,  185,  185,  161,  185,  185,
 /*  2220 */   164,  185,  185,  185,  172,  173,  174,  175,  172,  173,
 /*  2230 */   174,  175,  185,  132,  185,    1,    2,    3,    4,    5,
 /*  2240 */   185,  185,  141,  142,  143,  144,  145,  146,  147,  148,
 /*  2250 */   149,  150,  151,  152,  153,  154,  155,  156,  157,  158,
 /*  2260 */   159,  160,  112,  185,  185,  185,  185,  185,  185,  185,
 /*  2270 */   185,  185,  185,  172,  173,  174,  175,  185,   44,   45,
 /*  2280 */   185,  185,  132,  185,  185,  185,   52,  185,  185,   55,
 /*  2290 */   185,  141,  142,  143,  144,  145,  146,  147,  148,  149,
 /*  2300 */   150,  151,  152,  153,  154,  155,  156,  157,  158,  159,
 /*  2310 */   160,  112,  185,  185,  185,  161,  185,  185,  164,  185,
 /*  2320 */   185,  185,  172,  173,  174,  175,  172,  173,  174,  175,
 /*  2330 */   185,  132,  185,    1,    2,    3,    4,    5,  185,  185,
 /*  2340 */   141,  142,  143,  144,  145,  146,  147,  148,  149,  150,
 /*  2350 */   151,  152,  153,  154,  155,  156,  157,  158,  159,  160,
 /*  2360 */   112,  185,  185,  185,  185,  185,  185,  185,  185,  185,
 /*  2370 */   185,  172,  173,  174,  175,  185,   44,   45,  185,  185,
 /*  2380 */   132,  185,  185,  185,   52,  185,  185,   55,  185,  141,
 /*  2390 */   142,  143,  144,  145,  146,  147,  148,  149,  150,  151,
 /*  2400 */   152,  153,  154,  155,  156,  157,  158,  159,  160,  112,
 /*  2410 */   185,  185,  185,  185,  185,  185,  164,  185,  185,  185,
 /*  2420 */   172,  173,  174,  175,  172,  173,  174,  175,  185,  132,
 /*  2430 */   185,  185,  185,  185,  182,  183,  184,  185,  141,  142,
 /*  2440 */   143,  144,  145,  146,  147,  148,  149,  150,  151,  152,
 /*  2450 */   153,  154,  155,  156,  157,  158,  159,  160,  112,  185,
 /*  2460 */   185,  185,  185,  185,  185,  164,  185,  185,  185,  172,
 /*  2470 */   173,  174,  175,  172,  173,  174,  175,  185,  132,  185,
 /*  2480 */   185,  180,  181,  185,  185,  185,  185,  141,  142,  143,
 /*  2490 */   144,  145,  146,  147,  148,  149,  150,  151,  152,  153,
 /*  2500 */   154,  155,  156,  157,  158,  159,  160,  112,  185,  185,
 /*  2510 */   185,  185,  185,  185,  164,  185,  185,  185,  172,  173,
 /*  2520 */   174,  175,  172,  173,  174,  175,  185,  132,  185,  185,
 /*  2530 */   180,  181,  185,  185,  185,  185,  141,  142,  143,  144,
 /*  2540 */   145,  146,  147,  148,  149,  150,  151,  152,  153,  154,
 /*  2550 */   155,  156,  157,  158,  159,  160,  112,  185,  185,  185,
 /*  2560 */   185,  185,  185,  164,  185,  185,  185,  172,  173,  174,
 /*  2570 */   175,  172,  173,  174,  175,  185,  132,  185,  185,  180,
 /*  2580 */   181,  185,  185,  185,  185,  141,  142,  143,  144,  145,
 /*  2590 */   146,  147,  148,  149,  150,  151,  152,  153,  154,  155,
 /*  2600 */   156,  157,  158,  159,  160,  112,  185,  185,  185,  185,
 /*  2610 */   185,  185,  164,  185,  185,  185,  172,  173,  174,  175,
 /*  2620 */   172,  173,  174,  175,  185,  132,  185,  185,  180,  181,
 /*  2630 */   185,  185,  185,  185,  141,  142,  143,  144,  145,  146,
 /*  2640 */   147,  148,  149,  150,  151,  152,  153,  154,  155,  156,
 /*  2650 */   157,  158,  159,  160,  112,  185,  185,  185,  185,  185,
 /*  2660 */   185,  185,  185,  185,  185,  172,  173,  174,  175,   17,
 /*  2670 */    18,   19,   20,   21,   22,   23,   24,   25,   26,   27,
 /*  2680 */    28,   29,   30,  141,  142,  143,  144,  145,  146,  147,
 /*  2690 */   148,  149,  150,  151,  152,  153,  154,  155,  156,  157,
 /*  2700 */   158,  159,  160,  185,  185,  185,    7,    8,    9,   10,
 /*  2710 */    11,  164,  185,  185,  172,  173,  174,  175,   19,  172,
 /*  2720 */   173,  174,  175,  185,  185,  185,  185,  180,  181,  185,
 /*  2730 */    31,   32,   33,   34,   35,   36,  185,   38,   19,   53,
 /*  2740 */    54,  185,  185,   57,  185,   46,  185,  185,  185,  185,
 /*  2750 */    51,  185,   53,  185,  185,   36,   57,   38,   12,   13,
 /*  2760 */    14,  185,   76,   17,   18,   19,   20,   21,   22,   23,
 /*  2770 */    24,   25,   26,   27,   28,   29,   30,   91,   59,   60,
 /*  2780 */    61,   62,   63,   64,   65,   66,   67,   68,   69,   90,
 /*  2790 */   185,  185,  185,  185,  185,  185,  185,   98,   99,  100,
 /*  2800 */   101,  102,  103,  164,   58,    7,    8,    9,   10,   11,
 /*  2810 */   185,  172,  173,  174,  175,  164,  185,   19,  185,  180,
 /*  2820 */   181,  185,  185,  172,  173,  174,  175,  185,  185,   31,
 /*  2830 */    32,   33,   34,   35,   36,  184,   38,   19,  185,  185,
 /*  2840 */   185,  185,  185,  185,   46,  185,  185,  185,  185,   51,
 /*  2850 */   185,   53,  185,  185,   36,   57,   38,   12,   13,   14,
 /*  2860 */   185,  185,   17,   18,   19,   20,   21,   22,   23,   24,
 /*  2870 */    25,   26,   27,   28,   29,   30,  185,   59,   60,   61,
 /*  2880 */    62,   63,   64,   65,   66,   67,   68,   69,   90,  185,
 /*  2890 */   185,   46,  185,  185,  185,  185,   98,   99,  100,  101,
 /*  2900 */   102,  103,  164,  185,    7,    8,    9,   10,   11,  185,
 /*  2910 */   172,  173,  174,  175,  185,  164,   19,  185,  180,  181,
 /*  2920 */   169,  185,  185,  172,  173,  174,  175,  185,   31,   32,
 /*  2930 */    33,   34,   35,  185,  185,   38,   39,  185,  185,  185,
 /*  2940 */   185,  185,  185,   46,  185,  185,  185,  185,   51,  185,
 /*  2950 */    53,  185,  185,  185,   57,  185,   12,   13,   14,  185,
 /*  2960 */   185,   17,   18,   19,   20,   21,   22,   23,   24,   25,
 /*  2970 */    26,   27,   28,   29,   30,  185,   19,  185,  185,  185,
 /*  2980 */   185,  185,  185,   39,  185,  185,  185,   90,  185,    1,
 /*  2990 */     2,    3,    4,    5,  185,   98,   99,  100,  101,  102,
 /*  3000 */   103,  164,  185,    7,    8,    9,   10,   11,  185,  172,
 /*  3010 */   173,  174,  175,   56,  185,   19,   59,   60,   61,   62,
 /*  3020 */    63,   64,   65,   66,   67,   68,   69,   31,   32,   33,
 /*  3030 */    34,   35,   44,   45,   38,   47,   48,  185,   50,  185,
 /*  3040 */   185,  185,   46,   55,  185,  185,  185,   51,  185,   53,
 /*  3050 */   185,  185,  185,   57,   58,   12,   13,   14,  185,  185,
 /*  3060 */    17,   18,   19,   20,   21,   22,   23,   24,   25,   26,
 /*  3070 */    27,   28,   29,   30,  185,   19,  185,  185,  185,   36,
 /*  3080 */   164,  185,  185,  185,  185,  169,   90,  185,  172,  173,
 /*  3090 */   174,  175,  185,  185,   98,   99,  100,  101,  102,  103,
 /*  3100 */   164,  185,    7,    8,    9,   10,   11,  185,  172,  173,
 /*  3110 */   174,  175,  185,  185,   19,   59,   60,   61,   62,   63,
 /*  3120 */    64,   65,   66,   67,   68,   69,   31,   32,   33,   34,
 /*  3130 */    35,   36,  185,   38,   19,  185,  185,  185,  185,  185,
 /*  3140 */   185,   46,  185,  185,  185,  185,   51,  185,   53,  185,
 /*  3150 */   185,   36,   57,   38,   12,   13,   14,  185,  185,   17,
 /*  3160 */    18,   19,   20,   21,   22,   23,   24,   25,   26,   27,
 /*  3170 */    28,   29,   30,  185,   59,   60,   61,   62,   63,   64,
 /*  3180 */    65,   66,   67,   68,   69,   90,  185,  185,  185,  185,
 /*  3190 */   185,  185,  185,   98,   99,  100,  101,  102,  103,  164,
 /*  3200 */    58,    7,    8,    9,   10,   11,  185,  172,  173,  174,
 /*  3210 */   175,  185,  164,   19,  185,  180,  181,  185,  185,  185,
 /*  3220 */   172,  173,  174,  175,  185,   31,   32,   33,   34,   35,
 /*  3230 */    36,  185,   38,   19,  185,  185,  185,  185,  185,  185,
 /*  3240 */    46,  185,  185,  185,  185,   51,  185,   53,  185,  185,
 /*  3250 */    36,   57,   38,   12,   13,   14,  185,  185,   17,   18,
 /*  3260 */    19,   20,   21,   22,   23,   24,   25,   26,   27,   28,
 /*  3270 */    29,   30,  185,   59,   60,   61,   62,   63,   64,   65,
 /*  3280 */    66,   67,   68,   69,   90,  185,  185,   46,  185,  185,
 /*  3290 */   185,  185,   98,   99,  100,  101,  102,  103,  164,  185,
 /*  3300 */     7,    8,    9,   10,   11,  185,  172,  173,  174,  175,
 /*  3310 */   185,  164,   19,  185,  180,  181,  185,  185,  185,  172,
 /*  3320 */   173,  174,  175,  185,   31,   32,   33,   34,   35,  185,
 /*  3330 */   185,   38,  164,  185,  185,  185,  185,  185,  185,   46,
 /*  3340 */   172,  173,  174,  175,   51,  185,   53,  185,  180,  181,
 /*  3350 */    57,  185,   12,   13,   14,  185,  185,   17,   18,   19,
 /*  3360 */    20,   21,   22,   23,   24,   25,   26,   27,   28,   29,
 /*  3370 */    30,  185,  185,  185,  185,  185,   83,   49,  185,   39,
 /*  3380 */   185,  185,   54,   90,  185,   57,  185,  185,  185,  185,
 /*  3390 */   185,   98,   99,  100,  101,  102,  103,  164,  185,    7,
 /*  3400 */     8,    9,   10,   11,  185,  172,  173,  174,  175,  185,
 /*  3410 */   185,   19,  185,   85,   86,   87,   88,   89,  185,   91,
 /*  3420 */    92,   93,  185,   31,   32,   33,   34,   35,   36,  185,
 /*  3430 */    38,   19,  185,  185,  185,  185,  185,  185,   46,  185,
 /*  3440 */   185,  185,  185,   51,  185,   53,  185,  185,  185,   57,
 /*  3450 */    38,   12,   13,   14,  185,  185,   17,   18,   19,   20,
 /*  3460 */    21,   22,   23,   24,   25,   26,   27,   28,   29,   30,
 /*  3470 */   185,   59,   60,   61,   62,   63,   64,   65,   66,   67,
 /*  3480 */    68,   69,   90,  185,    1,    2,    3,    4,    5,  185,
 /*  3490 */    98,   99,  100,  101,  102,  103,  164,   58,    7,    8,
 /*  3500 */     9,   10,   11,  185,  172,  173,  174,  175,  164,  185,
 /*  3510 */    19,  185,  168,  181,  185,  185,  172,  173,  174,  175,
 /*  3520 */   185,  185,   31,   32,   33,   34,   35,   44,   45,   38,
 /*  3530 */    47,   48,    0,   50,  185,  185,  185,   46,   55,  185,
 /*  3540 */   185,  185,   51,  185,   53,  185,  185,  185,   57,   58,
 /*  3550 */    12,   13,   14,  185,  185,   17,   18,   19,   20,   21,
 /*  3560 */    22,   23,   24,   25,   26,   27,   28,   29,   30,   37,
 /*  3570 */   185,  185,   40,  185,   42,  185,   44,   45,  185,  185,
 /*  3580 */    48,   90,  185,  185,   46,  185,  185,  185,  185,   98,
 /*  3590 */    99,  100,  101,  102,  103,  164,  185,    7,    8,    9,
 /*  3600 */    10,   11,  185,  172,  173,  174,  175,  185,  164,   19,
 /*  3610 */   185,  185,  168,  185,  185,  185,  172,  173,  174,  175,
 /*  3620 */   185,   31,   32,   33,   34,   35,   36,  185,   38,  164,
 /*  3630 */     1,    2,    3,    4,    5,  185,   46,  172,  173,  174,
 /*  3640 */   175,   51,  185,   53,  185,  185,  185,   57,  185,   12,
 /*  3650 */    13,   14,  185,  185,   17,   18,   19,   20,   21,   22,
 /*  3660 */    23,   24,   25,   26,   27,   28,   29,   30,  185,  185,
 /*  3670 */   185,  185,  185,   44,   45,  185,   47,   48,  185,   50,
 /*  3680 */    90,  185,  185,  185,   55,  185,  185,  185,   98,   99,
 /*  3690 */   100,  101,  102,  103,  185,   58,    7,    8,    9,   10,
 /*  3700 */    11,  185,  185,  164,  185,  185,  185,  168,   19,  185,
 /*  3710 */   185,  172,  173,  174,  175,  185,  185,  185,  185,  185,
 /*  3720 */    31,   32,   33,   34,   35,   36,  185,   38,  164,    1,
 /*  3730 */     2,    3,    4,    5,  185,   46,  172,  173,  174,  175,
 /*  3740 */    51,  185,   53,  185,  185,  185,   57,  185,   12,   13,
 /*  3750 */    14,  185,  185,   17,   18,   19,   20,   21,   22,   23,
 /*  3760 */    24,   25,   26,   27,   28,   29,   30,  185,  185,  185,
 /*  3770 */   185,  185,   44,   45,  185,   47,   48,  185,   50,   90,
 /*  3780 */   185,  185,   46,   55,  185,  185,  185,   98,   99,  100,
 /*  3790 */   101,  102,  103,  185,  164,    7,    8,    9,   10,   11,
 /*  3800 */   185,  185,  172,  173,  174,  175,  164,   19,  185,  185,
 /*  3810 */   168,  185,  185,  185,  172,  173,  174,  175,  185,   31,
 /*  3820 */    32,   33,   34,   35,  185,  185,   38,  164,    1,    2,
 /*  3830 */     3,    4,    5,  185,   46,  172,  173,  174,  175,   51,
 /*  3840 */   185,   53,  185,  185,  185,   57,   58,   12,   13,   14,
 /*  3850 */   185,  185,   17,   18,   19,   20,   21,   22,   23,   24,
 /*  3860 */    25,   26,   27,   28,   29,   30,  185,  185,  185,  185,
 /*  3870 */   185,   44,   45,  185,   47,   48,  185,  185,   90,  185,
 /*  3880 */   185,  185,   55,  185,  185,  185,   98,   99,  100,  101,
 /*  3890 */   102,  103,  185,  164,    7,    8,    9,   10,   11,  185,
 /*  3900 */   185,  172,  173,  174,  175,  164,   19,  185,  185,  168,
 /*  3910 */   185,  185,  185,  172,  173,  174,  175,  185,   31,   32,
 /*  3920 */    33,   34,   35,   36,  185,   38,  164,    1,    2,    3,
 /*  3930 */     4,    5,  185,   46,  172,  173,  174,  175,   51,  185,
 /*  3940 */    53,  185,  185,  185,   57,  185,  185,   13,   14,  185,
 /*  3950 */   185,   17,   18,   19,   20,   21,   22,   23,   24,   25,
 /*  3960 */    26,   27,   28,   29,   30,  185,  185,  185,  185,  185,
 /*  3970 */    44,   45,  185,   47,   48,  185,  185,   90,  185,  185,
 /*  3980 */   185,   55,  185,  185,  185,   98,   99,  100,  101,  102,
 /*  3990 */   103,  185,  164,    7,    8,    9,   10,   11,  185,  185,
 /*  4000 */   172,  173,  174,  175,  185,   19,  185,    1,    2,    3,
 /*  4010 */     4,    5,    1,    2,    3,    4,    5,   31,   32,   33,
 /*  4020 */    34,   35,  185,  185,   38,  164,    1,    2,    3,    4,
 /*  4030 */     5,  185,   46,  172,  173,  174,  175,   51,  185,   53,
 /*  4040 */   185,  185,  185,   57,   58,  185,  185,  185,  185,  185,
 /*  4050 */    44,   45,  185,   47,   48,   44,   45,  185,   47,   48,
 /*  4060 */   185,   55,  185,  185,  185,  185,   55,  185,  185,   44,
 /*  4070 */    45,  185,   47,   48,  185,  185,   90,  185,  185,  185,
 /*  4080 */    55,  185,  185,  185,   98,   99,  100,  101,  102,  103,
 /*  4090 */   185,  185,    7,    8,    9,   10,   11,    1,    2,    3,
 /*  4100 */     4,    5,  185,  185,   19,  185,    1,    2,    3,    4,
 /*  4110 */     5,    1,    2,    3,    4,    5,   31,   32,   33,   34,
 /*  4120 */    35,   36,  185,   38,  164,  185,  185,  185,  168,  185,
 /*  4130 */   185,   46,  172,  173,  174,  175,   51,  185,   53,  185,
 /*  4140 */    44,   45,   57,   47,   48,  185,   38,  185,  185,   44,
 /*  4150 */    45,   55,   47,   48,   44,   45,  185,   47,   48,   51,
 /*  4160 */    55,  164,  185,  185,  185,   55,  185,  185,  164,  172,
 /*  4170 */   173,  174,  175,  185,  185,   90,  172,  173,  174,  175,
 /*  4180 */   185,  185,  185,   98,   99,  100,  101,  102,  103,  185,
 /*  4190 */   185,    7,    8,    9,   10,   11,  185,  185,   90,  185,
 /*  4200 */   185,  185,  185,   19,  185,  185,   98,   99,  100,  101,
 /*  4210 */   102,  103,  185,  185,  185,   31,   32,   33,   34,   35,
 /*  4220 */    36,  185,   38,  164,  185,  185,  185,  185,  185,  185,
 /*  4230 */    46,  172,  173,  174,  175,   51,  185,   53,  164,  185,
 /*  4240 */   185,   57,  185,  185,  185,  164,  172,  173,  174,  175,
 /*  4250 */   164,  185,  185,  172,  173,  174,  175,  164,  172,  173,
 /*  4260 */   174,  175,  185,  185,  185,  172,  173,  174,  175,  185,
 /*  4270 */   185,  185,  185,  185,   90,  185,  185,  185,  185,  185,
 /*  4280 */   185,  185,   98,   99,  100,  101,  102,  103,  185,  185,
 /*  4290 */     7,    8,    9,   10,   11,  185,  185,  185,  185,  185,
 /*  4300 */   185,  185,   19,  185,  185,  185,  185,  185,  185,  185,
 /*  4310 */   185,  185,  185,  185,   31,   32,   33,   34,   35,  185,
 /*  4320 */   185,   38,  185,  185,  185,  185,  185,  185,  185,   46,
 /*  4330 */   185,  185,  185,  185,   51,  185,   53,  185,  185,  185,
 /*  4340 */    57,  185,  185,  185,  185,  185,  185,  185,  185,  185,
 /*  4350 */   185,  185,  185,  185,  185,  185,  185,  185,  185,  185,
 /*  4360 */   185,  185,  185,  185,  185,  185,   83,  185,  185,  185,
 /*  4370 */   185,  185,  185,   90,  185,  185,  185,  185,  185,  185,
 /*  4380 */   185,   98,   99,  100,  101,  102,  103,  185,  185,    7,
 /*  4390 */     8,    9,   10,   11,  185,  185,  185,  185,  185,  185,
 /*  4400 */   185,   19,  185,  185,  185,  185,  185,  185,  185,  185,
 /*  4410 */   185,  185,  185,   31,   32,   33,   34,   35,  185,  185,
 /*  4420 */    38,  185,  185,  185,  185,  185,  185,  185,   46,  185,
 /*  4430 */   185,  185,  185,   51,  185,   53,  185,  185,  185,   57,
 /*  4440 */   185,  185,  185,  185,  185,  185,  185,  185,  185,  185,
 /*  4450 */   185,  185,  185,  185,  185,  185,  185,  185,  185,  185,
 /*  4460 */   185,  185,  185,  185,  185,  185,  185,  185,  185,  185,
 /*  4470 */   185,  185,   90,  185,  185,  185,  185,  185,  185,  185,
 /*  4480 */    98,   99,  100,  101,  102,  103,  185,  185,    7,    8,
 /*  4490 */     9,   10,   11,  185,  185,  185,  185,  185,  185,  185,
 /*  4500 */    19,  185,  185,  185,  185,  185,  185,  185,  185,  185,
 /*  4510 */   185,  185,   31,   32,   33,   34,   35,  185,  185,   38,
 /*  4520 */   185,  185,  185,  185,  185,  185,  185,   46,  185,  185,
 /*  4530 */   185,  185,   51,  185,   53,  185,  185,  185,   57,  185,
 /*  4540 */   185,  185,  185,  185,  185,  185,  185,  185,  185,  185,
 /*  4550 */   185,  185,  185,  185,  185,  185,  185,  185,  185,  185,
 /*  4560 */   185,  185,  185,  185,  185,  185,  185,  185,  185,  185,
 /*  4570 */   185,   90,  185,  185,  185,  185,  185,  185,  185,   98,
 /*  4580 */    99,  100,  101,  102,  103,
};
#define YY_SHIFT_USE_DFLT (-30)
static short yy_shift_ofst[] = {
 /*     0 */  1602,  850, 3532,  -30,  -30,  -30,  -30,  -30, 1331,  510,
 /*    10 */   -30,  271,  164,  -30,  948,  917,  -30, 4006,  -30, 4096,
 /*    20 */   -30,  -30, 2038,  897, 1015, 2719,  394, 2957,  866,  -30,
 /*    30 */   -30,  321, 3056,  -30,  -29,  -30,  -30,  -30,  877,  307,
 /*    40 */   -30, 1265, 1109,  -30,  -30,  -30,  -30,  -30,  -30,  -30,
 /*    50 */   -30,  -30,  -30,  -30,  -30,  -30,  -30,  296,  619, 2957,
 /*    60 */  1319,  -30,  -30, 3412,  -30,  888, 4108,  -30,  -30,  -30,
 /*    70 */   -30,  -30,  -30,  -30,  -30,  313,  479,  -30,  -30,   42,
 /*    80 */   899, 4108,  -30, 1073, 1087, 4108,  -30, 1300, 1255, 4108,
 /*    90 */   -30, 1001,  993,  -30,  -30,  -30,  -30,  -30,  -30,  -30,
 /*   100 */   -30,  -30,  -30, 1624, 2136,  119,  108, 3214,  588, 2957,
 /*   110 */   326,  -30,  -30,  231,  695, 2957,  852,  -30,  -30,  -30,
 /*   120 */   -30, 1008,  519,  -30, 1074,  677,  -30, 1200,  383,  -30,
 /*   130 */  3629,  -30, 1294,  -30, 2988, 4011,  -30, 2234, 1104, 1088,
 /*   140 */  2818,  432,  472,  -30,  816,  -30,  -30,  -30,  -30,  -30,
 /*   150 */   -30,  -30,  -30,  -30,  -30,  -30,  -30,  -30,  -30,  -30,
 /*   160 */   -30,  -30,  -30,  -30,  -30,  -30,  -30,  -30,  810,  -30,
 /*   170 */   894,  -30, 4382,  901,   84,   40,  915,  965,  622,  278,
 /*   180 */   -30, 4382,  999,   38,  -30,  -26,  -30,  -30, 4108, 1055,
 /*   190 */  1592, 1592, 1097,  719,  763,  -30, 4382, 1180, 1151, 1204,
 /*   200 */   -30, 1195, 1442, 1495, 1175, 4382, 1258,  -30, 4382,  -30,
 /*   210 */  4382,  829,  -30,  702, 4382, 4382, 3835, 4382, 4382, 3835,
 /*   220 */  4382, 4382, 3835, 4382, 4382, 3835, 4382, 4382, 3835, 4382,
 /*   230 */   357, 4382,  357, 4382,  357, 4382,  165, 4382,  165, 4382,
 /*   240 */   -30, 4382,  -30, 4382,  -30, 4382,  165, 4382, 2652, 4382,
 /*   250 */  2652, 4382, 3934,   -7, 3043,  -30,  976, 4382,  -30,  927,
 /*   260 */   944, 4382,  357, 1688,  774,  508, 2699, 3835,  878,  868,
 /*   270 */   829,  -30,  -30, 4382,  -30,  -30,  -30,  -30,  -30,  -30,
 /*   280 */   -30,  -30,  -30, 2996, 3835, 2686,  701,  318, 4184,  192,
 /*   290 */   390, 3689,  -30,  345,  -30, 4382,  642,  648,  561, 2798,
 /*   300 */   176,  -30,  -30,  -30,  -30,  -30,  -30,   51,  900, 3392,
 /*   310 */   587,  -30,  -30,  977,  978,  980, 3887,  123,  -30,  -30,
 /*   320 */   -30, 1012, 1019, 1025, 3590,  -30,  830,  -30,  490,  -30,
 /*   330 */   -30,  -30, 4382, 2746,  -30, 4085,  705,  -30,  -30, 1129,
 /*   340 */  1132,  -30,  684,  -30, 4481,  -30, 1158, 4382,  -30,  -30,
 /*   350 */   -30,  802,  -30,  -30,  233, 1246,  593, 3095,  340,  -30,
 /*   360 */   -30, 1274,  644, 3194,  511,  -30,  -30,  357,  357,  357,
 /*   370 */   357, 3439,  -30,  246,  -30,  368, 1277,  -30, 1259,  -30,
 /*   380 */   357, 3835, 1267,  348, 4283, 3736, 1592,  666,  -30, 1202,
 /*   390 */   834,  -30, 1202,  -30, 3328,  -30,  -30,  -30,  -30,  -30,
 /*   400 */   -30, 4382,  -30, 3835,  633,  -19, 4382,  -30, 3788,  491,
 /*   410 */  4382,  -30, 1086,  491, 4382,  -30, 3637,  -30,  -30,  245,
 /*   420 */  1093,  491, 4382,  -30, 1075,  491, 4382,  -30, 1061,  103,
 /*   430 */  4382,  -30, 3491,  491, 4382,  -30,  989,  491, 4382,  -30,
 /*   440 */  3986,  491, 4382,  -30,  972,  491, 4382,  -30,  -30,  -30,
 /*   450 */   -30, 4382,  716,  -30, 4382,  -30, 3835,  -30,  932,  -30,
 /*   460 */   931,  -30,  910,  -30,  903,  -30, 2897, 3340,  -30,  -30,
 /*   470 */   898,  667,  880,  875,  -30, 4382, 3142,  817,  -30, 4382,
 /*   480 */  2944,  -30,  211,  709,  -30,  211,  -30, 1327, 4108,  -30,
 /*   490 */   -30,  211,  515,  -30,  211,  127,  -30,  211,  620,  -30,
 /*   500 */   211,  748,  -30,  211,  268,  -30,  211,  737,  -30,  211,
 /*   510 */   612,  -30,  211,  813,  -30,  211,  446,  -30,  133,  507,
 /*   520 */   318,  342,  508,  201,  590,  593,  627,  644,  -30,  -30,
 /*   530 */  4382, 3241, 1592, 1301,  -30,  846,  807, 3293, 3538, 1592,
 /*   540 */   913,  -30, 4382, 2845, 1592, 1345,  -30,  864, 4382,  907,
 /*   550 */   -30,  -30,  -30,  874, 1592, 1592,  -30,  -30,  929,  960,
 /*   560 */  1010,  957,  -30,  -30, 2957,  968,  525,  -30,  234,  -30,
 /*   570 */   815,  863, 1107,  -30, 1248,  -30, 2957,  998, 1539,  -30,
 /*   580 */  1398,  -30, 1624, 2332, 1017, 1014, 3115,  785, 1054,  -30,
 /*   590 */   860,  -30, 2957, 1057,  569,  -30,  428,  -30,  523,  378,
 /*   600 */   331,  -30,  137,  -30, 2957, 1091,  181,  -30,  375,  -30,
 /*   610 */  3728, 3926,  -30,  -30, 1940, 1096, 1108, 4108, 1126,  -30,
 /*   620 */  1124, 1131, 4108, 1157,  -30,  -30, 1940, 1695,  128,  -30,
 /*   630 */  4108,  616,  -30, 1188,  -30,  188, 1189,  747, 1194,   60,
 /*   640 */   -30,  -30, 1201,  -30,  -30, 1210,  -30, 1793,  509,  -30,
 /*   650 */  4108,  731,  -30, 1240,  -30, 1263,  -30,  -30, 3483, 3827,
 /*   660 */  4025, 1624, 4105,  -30, 4110, 1200,  -30,  -30,  -30, 1200,
 /*   670 */   383,  -30, 1288, 1293,  422,  -30, 1297,  935,  -30, 1200,
 /*   680 */   383,  -30, 1310, 1315,   -1,  -30,  426,  935,  -30,  -30,
};
#define YY_REDUCE_USE_DFLT (-155)
static short yy_reduce_ofst[] = {
 /*     0 */   -92, -155,   30, -155, -155, -155, -155, -155, -155, -155,
 /*    10 */  -155, -155,  286, -155, -155,  755, -155, 1706, -155,  327,
 /*    20 */  -155, -155,  658, -155, -155, 1936, -155, 1023, -155, -155,
 /*    30 */  -155, -155,   34, -155, -155, -155, -155, -155, -155, -155,
 /*    40 */  -155, -155, -155, -155, -155, -155, -155, -155, -155, -155,
 /*    50 */  -155, -155, -155, -155, -155, -155, -155, -155, -155,  926,
 /*    60 */  -155, -155, -155,   85, -155, -155,  804, -155, -155, -155,
 /*    70 */  -155, -155, -155, -155, -155, -155, -155, -155, -155, -155,
 /*    80 */  -155,  916, -155, -155, -155, 1179, -155, -155, -155, 1083,
 /*    90 */  -155, -155, -155, -155, -155, -155, -155, -155, -155, -155,
 /*   100 */  -155, -155, -155,  328,  658, -155, -155, 1838, -155, 1545,
 /*   110 */  -155, -155, -155, -155, -155, 1314, -155, -155, -155, -155,
 /*   120 */  -155, -155,  995, -155, -155, 1092, -155,  483, 1148, -155,
 /*   130 */   -87, -155, -155, -155, 1653,  -86, -155,  658, -155, -155,
 /*   140 */  2034, -155, 2052, -155, 2542, -155, -155, -155, -155, -155,
 /*   150 */  -155, -155, -155, -155, -155, -155, -155, -155, -155, -155,
 /*   160 */  -155, -155, -155, -155, -155, -155, -155, -155, -155, -155,
 /*   170 */  -155, -155, 1860, -155, 2003, 2542, -155, -155, 2346, 2542,
 /*   180 */  -155, 2056, -155,  365, -155,  945, -155, -155, 1003, -155,
 /*   190 */  2297, 2542, -155, 2150, 2542, -155, 1762, -155, 2101, 2542,
 /*   200 */  -155, -155, 2493, 2542, -155, 2154, -155, -155, 3861, -155,
 /*   210 */  4074, 1159, -155, -155,  923, 3564, -155, 4081, 3997, -155,
 /*   220 */  3828, 3762, -155, 3663, 3630, -155, 3431, 3048, -155, 2936,
 /*   230 */  -155, 2837, -155, 1408, -155, 1020, -155,  826, -155,  632,
 /*   240 */  -155,  438, -155,  341, -155, 4086, -155, 4004, -155, 3465,
 /*   250 */  -155, 3147, -155,  -47, -155, -155, -155, 1214, -155, -155,
 /*   260 */  -155,  535, -155, -155, -155, -155, 2350, -155, -155, -155,
 /*   270 */   675, -155, -155,  147, -155, -155, -155, -155, -155, -155,
 /*   280 */  -155, -155, -155, 1713, -155, -155, -155, -155, 2399, -155,
 /*   290 */  -155, 2448, -155, -155, -155, 3332, -155, -155, -155, 2547,
 /*   300 */  -155, -155, -155, -155, -155, -155, -155, -155, -155, 2639,
 /*   310 */  -155, -155, -155, -155, -155, -155, 2301, -155, -155, -155,
 /*   320 */  -155, -155, -155, -155, 2738, -155, -155, -155, -155, -155,
 /*   330 */  -155, -155,  729, -155, -155, 3035, -155, -155, -155, -155,
 /*   340 */  -155, -155, -155, -155, 2252, -155, -155, 2651, -155, -155,
 /*   350 */  -155, -155, -155, -155, -155, -155, -155, 3134, -155, -155,
 /*   360 */  -155, -155, -155, 3168, -155, -155, -155, -155, -155, -155,
 /*   370 */  -155, -155, -155, -155, -155, -155, -155, -155, -155, -155,
 /*   380 */  -155, -155, -155, -155, 3729, -155, 1905, 2542, -155,  680,
 /*   390 */  -155, -155, 1094, -155, 1077, -155, -155, -155, -155, -155,
 /*   400 */  -155,  309, -155, -155, -155, 1033,  697, -155,  503,  996,
 /*   410 */  3741, -155, -155,  985, 3444, -155, -155, -155, -155, -155,
 /*   420 */  -155,  963, 1182, -155, -155,  943, 3344, -155, -155,  918,
 /*   430 */  3960, -155, 2751,  893,  794, -155, -155,  876, 1819, -155,
 /*   440 */  2916,  867, 3539, -155, -155,  865, 3642, -155, -155, -155,
 /*   450 */  -155,  115, -155, -155,  600, -155, -155, -155, -155, -155,
 /*   460 */  -155, -155, -155, -155, -155, -155,  244, -155, -155, -155,
 /*   470 */  -155, -155, -155, -155, -155, 3233, -155, -155, -155, 1117,
 /*   480 */  -155, -155,  448, -155, -155,   37, -155, -155, 1250, -155,
 /*   490 */  -155,  398, -155, -155,  157, -155, -155,  191, -155, -155,
 /*   500 */   468, -155, -155, -154, -155, -155,  592, -155, -155,  406,
 /*   510 */  -155, -155,  641, -155, -155,  303, -155, -155, -155, -155,
 /*   520 */  -155, -155, -155, -155, -155, -155, -155, -155, -155, -155,
 /*   530 */  1311, -155, 2199, 2542, -155, -155, -155, 4059, -155, 1758,
 /*   540 */  2542, -155, 4093, -155, 1807, 2542, -155, -155, 1958, -155,
 /*   550 */  -155, -155, -155, -155, 1562, 2542, -155, -155, -155, -155,
 /*   560 */  1611, 2542, -155, -155, -128, -155, 2248, -155, 2542, -155,
 /*   570 */  -155, -155, 2444, -155, 2542, -155, 1120, -155, 1954, -155,
 /*   580 */  2542, -155,  447,  658, -155, -155,  -56, -155, 1709, -155,
 /*   590 */  2542, -155, 1591, -155, 1660, -155, 2542, -155, -155, -155,
 /*   600 */  1856, -155, 2542, -155, 1411, -155, 2395, -155, 2542, -155,
 /*   610 */   232,  -86, -155, -155,  447, -155, -155, 1040, -155, -155,
 /*   620 */  -155, -155, 1058, -155, -155, -155,  297,  658, 1084, -155,
 /*   630 */  1095, 1099, -155, -155, -155,  530, -155, -155, -155, 1110,
 /*   640 */  -155, -155, -155, -155, -155, -155, -155,  658, 1136, -155,
 /*   650 */  1153, 1147, -155, -155, -155, -155, -155, -155,  135,  -58,
 /*   660 */   -86,  297,  -86, -155,  -86, 1199, -155, -155, -155,  153,
 /*   670 */  1211, -155, -155, -155, 1220, -155, -155, 1222, -155,  386,
 /*   680 */  1237, -155, -155, -155, 1245, -155, -155,  969, -155, -155,
};
static YYACTIONTYPE yy_default[] = {
 /*     0 */  1014, 1014, 1014,  692,  694,  695,  696,  697, 1014, 1014,
 /*    10 */   698, 1014, 1014,  699, 1014, 1014,  700, 1014,  715, 1014,
 /*    20 */   716,  746, 1014, 1014, 1014, 1014, 1014, 1014, 1014,  766,
 /*    30 */   784,  785, 1014,  786,  788,  789,  790,  791, 1014, 1014,
 /*    40 */   802, 1014, 1014,  803,  804,  805,  806,  807,  808,  809,
 /*    50 */   810,  811,  812,  813,  814,  787,  770, 1014, 1014, 1014,
 /*    60 */  1014,  767,  771, 1014,  792,  794, 1014,  798,  999, 1000,
 /*    70 */  1001, 1002, 1003, 1004, 1005, 1014, 1014, 1006, 1007, 1014,
 /*    80 */   795, 1014,  799, 1014,  796, 1014,  800, 1014,  797, 1014,
 /*    90 */   801, 1014, 1014,  793,  774,  776,  777,  778,  779,  780,
 /*   100 */   781,  782,  783, 1014, 1014, 1014, 1014, 1014, 1014, 1014,
 /*   110 */  1014,  768,  772, 1014, 1014, 1014, 1014,  769,  773,  775,
 /*   120 */   747, 1014, 1014,  701, 1014, 1014,  702, 1014, 1014,  704,
 /*   130 */  1014,  710, 1014,  711, 1014, 1014,  744, 1014, 1014, 1014,
 /*   140 */  1014, 1014, 1014,  750, 1014,  752,  815,  817,  818,  819,
 /*   150 */   820,  821,  822,  823,  824,  825,  826,  827,  828,  829,
 /*   160 */   830,  831,  832,  833,  834,  835,  836,  837, 1014,  838,
 /*   170 */  1014,  839, 1014, 1014, 1014, 1014,  842, 1014, 1014, 1014,
 /*   180 */   843, 1014, 1014, 1014,  846, 1014,  847,  848, 1014, 1014,
 /*   190 */   850,  851, 1014, 1014, 1014,  854, 1014, 1014, 1014, 1014,
 /*   200 */   856, 1014, 1014, 1014, 1014, 1014, 1014,  858, 1014,  916,
 /*   210 */  1014, 1014,  917, 1014, 1014, 1014,  918, 1014, 1014,  919,
 /*   220 */  1014, 1014,  920, 1014, 1014,  921, 1014, 1014,  922, 1014,
 /*   230 */   928, 1014,  929, 1014,  930, 1014,  940, 1014,  941, 1014,
 /*   240 */   942, 1014,  943, 1014,  944, 1014,  945, 1014,  946, 1014,
 /*   250 */   947, 1014,  948, 1014, 1014,  931, 1014, 1014,  932, 1014,
 /*   260 */  1014, 1014,  933,  952, 1014,  934, 1014,  990, 1014, 1014,
 /*   270 */  1014,  949,  950, 1014,  951,  953,  954,  955,  956,  957,
 /*   280 */   958,  959,  960, 1014,  998,  952, 1014,  937, 1014, 1014,
 /*   290 */   963, 1014,  964, 1014,  965, 1014, 1014, 1014, 1014, 1014,
 /*   300 */  1014,  971,  972,  985,  986,  987,  989, 1014, 1014, 1014,
 /*   310 */  1014,  975,  976, 1014, 1014, 1014, 1014, 1014,  977,  978,
 /*   320 */   988, 1014, 1014,  966, 1014,  967, 1014,  968, 1014,  973,
 /*   330 */   974,  938, 1014, 1014,  939, 1014, 1014,  969,  970,  953,
 /*   340 */   954,  961, 1014,  962, 1014,  991, 1014, 1014,  993,  994,
 /*   350 */   992, 1014,  979,  980, 1014, 1014,  935, 1014, 1014,  981,
 /*   360 */   982, 1014,  936, 1014, 1014,  983,  984,  927,  926,  925,
 /*   370 */   924, 1014, 1008, 1014, 1009, 1014, 1014, 1010, 1014, 1011,
 /*   380 */   923, 1012, 1014, 1014, 1014, 1014, 1014, 1014,  859, 1014,
 /*   390 */  1014,  863, 1014,  864, 1014,  866,  867,  868,  869,  870,
 /*   400 */   871, 1014,  872,  915, 1014, 1014, 1014,  873, 1014, 1014,
 /*   410 */  1014,  876, 1014, 1014, 1014,  877, 1014,  883,  884, 1014,
 /*   420 */  1014, 1014, 1014,  874, 1014, 1014, 1014,  875, 1014, 1014,
 /*   430 */  1014,  878, 1014, 1014, 1014,  879, 1014, 1014, 1014,  880,
 /*   440 */  1014, 1014, 1014,  881, 1014, 1014, 1014,  882,  885,  886,
 /*   450 */   865, 1014, 1014,  888, 1014,  889,  891,  890, 1014,  892,
 /*   460 */  1014,  893, 1014,  894, 1014,  895, 1014, 1014,  896,  897,
 /*   470 */  1014, 1014, 1014, 1014,  898, 1014, 1014, 1014,  899, 1014,
 /*   480 */  1014,  900, 1014, 1014,  901, 1014,  911,  913, 1014,  914,
 /*   490 */   912, 1014, 1014,  902, 1014, 1014,  903, 1014, 1014,  904,
 /*   500 */  1014, 1014,  905, 1014, 1014,  906, 1014, 1014,  907, 1014,
 /*   510 */  1014,  908, 1014, 1014,  909, 1014, 1014,  910, 1014, 1014,
 /*   520 */  1014, 1014, 1014, 1014, 1014, 1014, 1014, 1014, 1013,  816,
 /*   530 */  1014, 1014, 1014, 1014,  860, 1014, 1014, 1014, 1014, 1014,
 /*   540 */  1014,  861, 1014, 1014, 1014, 1014,  862, 1014, 1014, 1014,
 /*   550 */   857,  855,  853, 1014, 1014,  852,  849,  844,  840, 1014,
 /*   560 */  1014, 1014,  845,  841, 1014, 1014, 1014,  758, 1014,  760,
 /*   570 */  1014, 1014, 1014,  751, 1014,  753, 1014, 1014, 1014,  759,
 /*   580 */  1014,  761, 1014, 1014, 1014, 1014, 1014, 1014, 1014,  754,
 /*   590 */  1014,  756, 1014, 1014, 1014,  762, 1014,  764, 1014, 1014,
 /*   600 */  1014,  755, 1014,  757, 1014, 1014, 1014,  763, 1014,  765,
 /*   610 */  1014, 1014,  742,  745, 1014, 1014, 1014, 1014, 1014,  748,
 /*   620 */  1014, 1014, 1014, 1014,  749,  726, 1014, 1014, 1014,  728,
 /*   630 */  1014, 1014,  730, 1014,  734, 1014, 1014, 1014, 1014, 1014,
 /*   640 */   738,  740, 1014,  741,  739, 1014,  732, 1014, 1014,  729,
 /*   650 */  1014, 1014,  731, 1014,  735, 1014,  733,  743, 1014, 1014,
 /*   660 */  1014, 1014, 1014,  727, 1014, 1014,  712,  714,  713, 1014,
 /*   670 */  1014,  703, 1014, 1014, 1014,  705, 1014, 1014,  706, 1014,
 /*   680 */  1014,  707, 1014, 1014, 1014,  708, 1014, 1014,  709,  693,
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
  "PARENTHESES_CLOSE",  "NAMESPACE",     "IDENTIFIER",    "DOTCOMMA",    
  "INTERFACE",     "EXTENDS",       "CLASS",         "IMPLEMENTS",  
  "ABSTRACT",      "FINAL",         "BRACKET_OPEN",  "BRACKET_CLOSE",
  "COMMENT",       "ASSIGN",        "CONST",         "CONSTANT",    
  "FUNCTION",      "PARENTHESES_OPEN",  "ARROW",         "INLINE",      
  "VOID",          "SBRACKET_OPEN",  "SBRACKET_CLOSE",  "TYPE_INTEGER",
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
  "xx_assign_expr",  "xx_index_expr",  "xx_echo_expressions",  "xx_echo_expression",
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
 /* 192 */ "xx_let_assignment ::= IDENTIFIER SBRACKET_OPEN xx_index_expr SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 193 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER INCR",
 /* 194 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER DECR",
 /* 195 */ "xx_let_assignment ::= IDENTIFIER INCR",
 /* 196 */ "xx_let_assignment ::= IDENTIFIER DECR",
 /* 197 */ "xx_index_expr ::= xx_common_expr",
 /* 198 */ "xx_echo_statement ::= ECHO xx_echo_expressions DOTCOMMA",
 /* 199 */ "xx_echo_expressions ::= xx_echo_expressions COMMA xx_echo_expression",
 /* 200 */ "xx_echo_expressions ::= xx_echo_expression",
 /* 201 */ "xx_echo_expression ::= xx_common_expr",
 /* 202 */ "xx_mcall_statement ::= xx_mcall_expr DOTCOMMA",
 /* 203 */ "xx_fcall_statement ::= xx_fcall_expr DOTCOMMA",
 /* 204 */ "xx_scall_statement ::= xx_scall_expr DOTCOMMA",
 /* 205 */ "xx_fetch_statement ::= xx_fetch_expr DOTCOMMA",
 /* 206 */ "xx_return_statement ::= RETURN xx_common_expr DOTCOMMA",
 /* 207 */ "xx_return_statement ::= RETURN DOTCOMMA",
 /* 208 */ "xx_unset_statement ::= UNSET IDENTIFIER ARROW IDENTIFIER DOTCOMMA",
 /* 209 */ "xx_unset_statement ::= UNSET IDENTIFIER SBRACKET_OPEN xx_common_expr SBRACKET_CLOSE DOTCOMMA",
 /* 210 */ "xx_throw_statement ::= THROW xx_common_expr DOTCOMMA",
 /* 211 */ "xx_declare_statement ::= TYPE_INTEGER xx_declare_variable_list DOTCOMMA",
 /* 212 */ "xx_declare_statement ::= TYPE_UINTEGER xx_declare_variable_list DOTCOMMA",
 /* 213 */ "xx_declare_statement ::= TYPE_CHAR xx_declare_variable_list DOTCOMMA",
 /* 214 */ "xx_declare_statement ::= TYPE_UCHAR xx_declare_variable_list DOTCOMMA",
 /* 215 */ "xx_declare_statement ::= TYPE_LONG xx_declare_variable_list DOTCOMMA",
 /* 216 */ "xx_declare_statement ::= TYPE_ULONG xx_declare_variable_list DOTCOMMA",
 /* 217 */ "xx_declare_statement ::= TYPE_DOUBLE xx_declare_variable_list DOTCOMMA",
 /* 218 */ "xx_declare_statement ::= TYPE_STRING xx_declare_variable_list DOTCOMMA",
 /* 219 */ "xx_declare_statement ::= TYPE_BOOL xx_declare_variable_list DOTCOMMA",
 /* 220 */ "xx_declare_statement ::= TYPE_VAR xx_declare_variable_list DOTCOMMA",
 /* 221 */ "xx_declare_variable_list ::= xx_declare_variable_list COMMA xx_declare_variable",
 /* 222 */ "xx_declare_variable_list ::= xx_declare_variable",
 /* 223 */ "xx_declare_variable ::= IDENTIFIER",
 /* 224 */ "xx_declare_variable ::= IDENTIFIER ASSIGN xx_literal_expr",
 /* 225 */ "xx_assign_expr ::= xx_common_expr",
 /* 226 */ "xx_common_expr ::= NOT xx_common_expr",
 /* 227 */ "xx_common_expr ::= ISSET xx_isset_expr",
 /* 228 */ "xx_common_expr ::= REQUIRE xx_common_expr",
 /* 229 */ "xx_common_expr ::= CLONE xx_common_expr",
 /* 230 */ "xx_common_expr ::= EMPTY xx_common_expr",
 /* 231 */ "xx_common_expr ::= LIKELY xx_common_expr",
 /* 232 */ "xx_common_expr ::= UNLIKELY xx_common_expr",
 /* 233 */ "xx_common_expr ::= xx_common_expr EQUALS xx_common_expr",
 /* 234 */ "xx_common_expr ::= xx_common_expr NOTEQUALS xx_common_expr",
 /* 235 */ "xx_common_expr ::= xx_common_expr IDENTICAL xx_common_expr",
 /* 236 */ "xx_common_expr ::= xx_common_expr NOTIDENTICAL xx_common_expr",
 /* 237 */ "xx_common_expr ::= xx_common_expr LESS xx_common_expr",
 /* 238 */ "xx_common_expr ::= xx_common_expr GREATER xx_common_expr",
 /* 239 */ "xx_common_expr ::= xx_common_expr LESSEQUAL xx_common_expr",
 /* 240 */ "xx_common_expr ::= xx_common_expr GREATEREQUAL xx_common_expr",
 /* 241 */ "xx_common_expr ::= PARENTHESES_OPEN xx_common_expr PARENTHESES_CLOSE",
 /* 242 */ "xx_common_expr ::= PARENTHESES_OPEN xx_parameter_type PARENTHESES_CLOSE xx_common_expr",
 /* 243 */ "xx_common_expr ::= LESS IDENTIFIER GREATER xx_common_expr",
 /* 244 */ "xx_common_expr ::= IDENTIFIER ARROW IDENTIFIER",
 /* 245 */ "xx_common_expr ::= IDENTIFIER ARROW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE",
 /* 246 */ "xx_common_expr ::= IDENTIFIER ARROW BRACKET_OPEN STRING BRACKET_CLOSE",
 /* 247 */ "xx_common_expr ::= IDENTIFIER DOUBLECOLON IDENTIFIER",
 /* 248 */ "xx_common_expr ::= IDENTIFIER DOUBLECOLON CONSTANT",
 /* 249 */ "xx_common_expr ::= IDENTIFIER SBRACKET_OPEN xx_common_expr SBRACKET_CLOSE",
 /* 250 */ "xx_common_expr ::= xx_common_expr ADD xx_common_expr",
 /* 251 */ "xx_common_expr ::= xx_common_expr SUB xx_common_expr",
 /* 252 */ "xx_common_expr ::= xx_common_expr MUL xx_common_expr",
 /* 253 */ "xx_common_expr ::= xx_common_expr DIV xx_common_expr",
 /* 254 */ "xx_common_expr ::= xx_common_expr MOD xx_common_expr",
 /* 255 */ "xx_common_expr ::= xx_common_expr CONCAT xx_common_expr",
 /* 256 */ "xx_common_expr ::= xx_common_expr AND xx_common_expr",
 /* 257 */ "xx_common_expr ::= xx_common_expr OR xx_common_expr",
 /* 258 */ "xx_common_expr ::= xx_common_expr INSTANCEOF xx_common_expr",
 /* 259 */ "xx_fetch_expr ::= FETCH IDENTIFIER COMMA xx_isset_expr",
 /* 260 */ "xx_common_expr ::= xx_fetch_expr",
 /* 261 */ "xx_common_expr ::= TYPEOF xx_common_expr",
 /* 262 */ "xx_common_expr ::= IDENTIFIER",
 /* 263 */ "xx_common_expr ::= INTEGER",
 /* 264 */ "xx_common_expr ::= STRING",
 /* 265 */ "xx_common_expr ::= CHAR",
 /* 266 */ "xx_common_expr ::= DOUBLE",
 /* 267 */ "xx_common_expr ::= NULL",
 /* 268 */ "xx_common_expr ::= TRUE",
 /* 269 */ "xx_common_expr ::= FALSE",
 /* 270 */ "xx_common_expr ::= CONSTANT",
 /* 271 */ "xx_common_expr ::= SBRACKET_OPEN SBRACKET_CLOSE",
 /* 272 */ "xx_common_expr ::= SBRACKET_OPEN xx_array_list SBRACKET_CLOSE",
 /* 273 */ "xx_common_expr ::= NEW IDENTIFIER",
 /* 274 */ "xx_common_expr ::= NEW IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 275 */ "xx_common_expr ::= NEW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 276 */ "xx_common_expr ::= NEW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE",
 /* 277 */ "xx_common_expr ::= NEW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 278 */ "xx_common_expr ::= NEW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 279 */ "xx_fcall_expr ::= IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 280 */ "xx_fcall_expr ::= IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 281 */ "xx_fcall_expr ::= BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 282 */ "xx_fcall_expr ::= BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 283 */ "xx_scall_expr ::= IDENTIFIER DOUBLECOLON IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 284 */ "xx_scall_expr ::= IDENTIFIER DOUBLECOLON IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 285 */ "xx_scall_expr ::= BRACKET_OPEN IDENTIFIER BRACKET_CLOSE DOUBLECOLON IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 286 */ "xx_scall_expr ::= BRACKET_OPEN IDENTIFIER BRACKET_CLOSE DOUBLECOLON IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 287 */ "xx_scall_expr ::= BRACKET_OPEN IDENTIFIER BRACKET_CLOSE DOUBLECOLON BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 288 */ "xx_scall_expr ::= BRACKET_OPEN IDENTIFIER BRACKET_CLOSE DOUBLECOLON BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 289 */ "xx_mcall_expr ::= IDENTIFIER ARROW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 290 */ "xx_mcall_expr ::= IDENTIFIER ARROW IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 291 */ "xx_mcall_expr ::= IDENTIFIER ARROW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 292 */ "xx_mcall_expr ::= IDENTIFIER ARROW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 293 */ "xx_mcall_expr ::= IDENTIFIER ARROW BRACKET_OPEN STRING BRACKET_CLOSE PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 294 */ "xx_mcall_expr ::= IDENTIFIER ARROW BRACKET_OPEN STRING BRACKET_CLOSE PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 295 */ "xx_common_expr ::= xx_mcall_expr",
 /* 296 */ "xx_common_expr ::= xx_scall_expr",
 /* 297 */ "xx_common_expr ::= xx_fcall_expr",
 /* 298 */ "xx_call_parameters ::= xx_call_parameters COMMA xx_call_parameter",
 /* 299 */ "xx_call_parameters ::= xx_call_parameter",
 /* 300 */ "xx_call_parameter ::= xx_common_expr",
 /* 301 */ "xx_array_list ::= xx_array_list COMMA xx_array_item",
 /* 302 */ "xx_array_list ::= xx_array_item",
 /* 303 */ "xx_array_item ::= xx_array_key COLON xx_array_value",
 /* 304 */ "xx_array_item ::= xx_array_value",
 /* 305 */ "xx_array_key ::= IDENTIFIER",
 /* 306 */ "xx_array_key ::= STRING",
 /* 307 */ "xx_array_key ::= INTEGER",
 /* 308 */ "xx_array_value ::= xx_common_expr",
 /* 309 */ "xx_literal_expr ::= INTEGER",
 /* 310 */ "xx_literal_expr ::= CHAR",
 /* 311 */ "xx_literal_expr ::= STRING",
 /* 312 */ "xx_literal_expr ::= DOUBLE",
 /* 313 */ "xx_literal_expr ::= NULL",
 /* 314 */ "xx_literal_expr ::= FALSE",
 /* 315 */ "xx_literal_expr ::= TRUE",
 /* 316 */ "xx_literal_expr ::= IDENTIFIER DOUBLECOLON CONSTANT",
 /* 317 */ "xx_literal_expr ::= CONSTANT",
 /* 318 */ "xx_isset_expr ::= IDENTIFIER SBRACKET_OPEN xx_common_expr SBRACKET_CLOSE",
 /* 319 */ "xx_isset_expr ::= IDENTIFIER ARROW IDENTIFIER",
 /* 320 */ "xx_isset_expr ::= IDENTIFIER ARROW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE",
 /* 321 */ "xx_isset_expr ::= IDENTIFIER ARROW BRACKET_OPEN STRING BRACKET_CLOSE",
 /* 322 */ "xx_eval_expr ::= xx_common_expr",
 /* 323 */ "xx_comment ::= COMMENT",
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
// 999 "parser.lemon"
{
	/*if ((yypminor->yy0)) {
		if ((yypminor->yy0)->free_flag) {
			efree((yypminor->yy0)->token);
		}
		efree((yypminor->yy0));
	}*/
}
// 2809 "parser.c"
      break;
    case 106:
// 1012 "parser.lemon"
{ json_object_put((yypminor->yy235)); }
// 2814 "parser.c"
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
  { 166, 6 },
  { 166, 4 },
  { 166, 4 },
  { 166, 2 },
  { 166, 2 },
  { 169, 1 },
  { 145, 3 },
  { 170, 3 },
  { 170, 1 },
  { 171, 1 },
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
  { 176, 3 },
  { 176, 1 },
  { 177, 1 },
  { 177, 3 },
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
  { 175, 4 },
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
  { 173, 4 },
  { 173, 3 },
  { 173, 6 },
  { 173, 5 },
  { 174, 6 },
  { 174, 5 },
  { 174, 8 },
  { 174, 7 },
  { 174, 10 },
  { 174, 9 },
  { 172, 6 },
  { 172, 5 },
  { 172, 8 },
  { 172, 7 },
  { 172, 8 },
  { 172, 7 },
  { 164, 1 },
  { 164, 1 },
  { 164, 1 },
  { 180, 3 },
  { 180, 1 },
  { 181, 1 },
  { 179, 3 },
  { 179, 1 },
  { 182, 3 },
  { 182, 1 },
  { 183, 1 },
  { 183, 1 },
  { 183, 1 },
  { 184, 1 },
  { 124, 1 },
  { 124, 1 },
  { 124, 1 },
  { 124, 1 },
  { 124, 1 },
  { 124, 1 },
  { 124, 1 },
  { 124, 3 },
  { 124, 1 },
  { 178, 4 },
  { 178, 3 },
  { 178, 5 },
  { 178, 5 },
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
// 1008 "parser.lemon"
{
	status->ret = yymsp[0].minor.yy235;
}
// 3355 "parser.c"
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
      case 197:
      case 225:
      case 260:
      case 295:
      case 296:
      case 297:
      case 300:
      case 308:
      case 322:
// 1014 "parser.lemon"
{
	yygotominor.yy235 = yymsp[0].minor.yy235;
}
// 3395 "parser.c"
        break;
      case 2:
      case 36:
      case 52:
      case 54:
      case 56:
      case 84:
      case 125:
      case 158:
// 1018 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_list(yymsp[-1].minor.yy235, yymsp[0].minor.yy235);
}
// 3409 "parser.c"
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
      case 200:
      case 222:
      case 299:
      case 302:
// 1022 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_list(NULL, yymsp[0].minor.yy235);
}
// 3432 "parser.c"
        break;
      case 8:
// 1042 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_namespace(yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(37,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 3441 "parser.c"
        break;
      case 9:
// 1046 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_interface(yymsp[-1].minor.yy0, yymsp[0].minor.yy235, NULL, status->scanner_state);
  yy_destructor(40,&yymsp[-2].minor);
}
// 3449 "parser.c"
        break;
      case 10:
// 1050 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_interface(yymsp[-3].minor.yy0, yymsp[0].minor.yy235, yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(40,&yymsp[-4].minor);
  yy_destructor(41,&yymsp[-2].minor);
}
// 3458 "parser.c"
        break;
      case 11:
// 1054 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class(yymsp[-1].minor.yy0, yymsp[0].minor.yy235, 0, 0, NULL, NULL, status->scanner_state);
  yy_destructor(42,&yymsp[-2].minor);
}
// 3466 "parser.c"
        break;
      case 12:
// 1058 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy235, 0, 0, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(42,&yymsp[-4].minor);
  yy_destructor(41,&yymsp[-2].minor);
}
// 3475 "parser.c"
        break;
      case 13:
// 1062 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy235, 0, 0, NULL, yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(42,&yymsp[-4].minor);
  yy_destructor(43,&yymsp[-2].minor);
}
// 3484 "parser.c"
        break;
      case 14:
// 1066 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class(yymsp[-5].minor.yy0, yymsp[0].minor.yy235, 0, 0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(42,&yymsp[-6].minor);
  yy_destructor(41,&yymsp[-4].minor);
  yy_destructor(43,&yymsp[-2].minor);
}
// 3494 "parser.c"
        break;
      case 15:
// 1070 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class(yymsp[-1].minor.yy0, yymsp[0].minor.yy235, 1, 0, NULL, NULL, status->scanner_state);
  yy_destructor(44,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-2].minor);
}
// 3503 "parser.c"
        break;
      case 16:
// 1074 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy235, 1, 0, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(44,&yymsp[-5].minor);
  yy_destructor(42,&yymsp[-4].minor);
  yy_destructor(41,&yymsp[-2].minor);
}
// 3513 "parser.c"
        break;
      case 17:
// 1078 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy235, 1, 0, NULL, yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(44,&yymsp[-5].minor);
  yy_destructor(42,&yymsp[-4].minor);
  yy_destructor(43,&yymsp[-2].minor);
}
// 3523 "parser.c"
        break;
      case 18:
// 1082 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class(yymsp[-1].minor.yy0, yymsp[0].minor.yy235, 0, 1, NULL, NULL, status->scanner_state);
  yy_destructor(45,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-2].minor);
}
// 3532 "parser.c"
        break;
      case 19:
// 1086 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy235, 0, 1, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(45,&yymsp[-5].minor);
  yy_destructor(42,&yymsp[-4].minor);
  yy_destructor(41,&yymsp[-2].minor);
}
// 3542 "parser.c"
        break;
      case 20:
      case 25:
      case 46:
// 1090 "parser.lemon"
{
	yygotominor.yy235 = NULL;
  yy_destructor(46,&yymsp[-1].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 3553 "parser.c"
        break;
      case 21:
      case 26:
      case 47:
// 1094 "parser.lemon"
{
	yygotominor.yy235 = yymsp[-1].minor.yy235;
  yy_destructor(46,&yymsp[-2].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 3564 "parser.c"
        break;
      case 22:
      case 48:
      case 102:
      case 174:
      case 199:
      case 221:
      case 298:
      case 301:
// 1098 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_list(yymsp[-2].minor.yy235, yymsp[0].minor.yy235);
  yy_destructor(6,&yymsp[-1].minor);
}
// 3579 "parser.c"
        break;
      case 24:
      case 262:
      case 305:
// 1106 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state);
}
// 3588 "parser.c"
        break;
      case 27:
// 1118 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class_definition(yymsp[0].minor.yy235, NULL, NULL, status->scanner_state);
}
// 3595 "parser.c"
        break;
      case 28:
// 1122 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class_definition(NULL, NULL, yymsp[0].minor.yy235, status->scanner_state);
}
// 3602 "parser.c"
        break;
      case 29:
// 1126 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class_definition(NULL, yymsp[0].minor.yy235, NULL, status->scanner_state);
}
// 3609 "parser.c"
        break;
      case 30:
// 1130 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class_definition(yymsp[-1].minor.yy235, yymsp[0].minor.yy235, NULL, status->scanner_state);
}
// 3616 "parser.c"
        break;
      case 31:
// 1134 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class_definition(yymsp[-1].minor.yy235, NULL, yymsp[0].minor.yy235, status->scanner_state);
}
// 3623 "parser.c"
        break;
      case 32:
// 1138 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class_definition(yymsp[0].minor.yy235, NULL, yymsp[-1].minor.yy235, status->scanner_state);
}
// 3630 "parser.c"
        break;
      case 33:
// 1142 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class_definition(NULL, yymsp[0].minor.yy235, yymsp[-1].minor.yy235, status->scanner_state);
}
// 3637 "parser.c"
        break;
      case 34:
// 1146 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class_definition(yymsp[-2].minor.yy235, yymsp[0].minor.yy235, yymsp[-1].minor.yy235, status->scanner_state);
}
// 3644 "parser.c"
        break;
      case 35:
// 1150 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class_definition(yymsp[-1].minor.yy235, yymsp[0].minor.yy235, yymsp[-2].minor.yy235, status->scanner_state);
}
// 3651 "parser.c"
        break;
      case 38:
// 1167 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class_property(yymsp[-2].minor.yy235, yymsp[-1].minor.yy0, NULL, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(39,&yymsp[0].minor);
}
// 3659 "parser.c"
        break;
      case 39:
// 1171 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class_property(yymsp[-2].minor.yy235, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(39,&yymsp[0].minor);
}
// 3667 "parser.c"
        break;
      case 40:
// 1175 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class_property(yymsp[-4].minor.yy235, yymsp[-3].minor.yy0, yymsp[-1].minor.yy235, yymsp[-5].minor.yy0, NULL, status->scanner_state);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 3676 "parser.c"
        break;
      case 41:
// 1179 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class_property(yymsp[-4].minor.yy235, yymsp[-3].minor.yy0, yymsp[-1].minor.yy235, NULL, NULL, status->scanner_state);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 3685 "parser.c"
        break;
      case 42:
// 1183 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class_property(yymsp[-3].minor.yy235, yymsp[-2].minor.yy0, NULL, yymsp[-4].minor.yy0, yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(39,&yymsp[0].minor);
}
// 3693 "parser.c"
        break;
      case 43:
// 1187 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class_property(yymsp[-3].minor.yy235, yymsp[-2].minor.yy0, NULL, NULL, yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(39,&yymsp[0].minor);
}
// 3701 "parser.c"
        break;
      case 44:
// 1191 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class_property(yymsp[-5].minor.yy235, yymsp[-4].minor.yy0, yymsp[-2].minor.yy235, yymsp[-6].minor.yy0, yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(49,&yymsp[-3].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 3710 "parser.c"
        break;
      case 45:
// 1195 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class_property(yymsp[-5].minor.yy235, yymsp[-4].minor.yy0, yymsp[-2].minor.yy235, NULL, yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(49,&yymsp[-3].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 3719 "parser.c"
        break;
      case 50:
// 1215 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_property_shortcut(NULL, yymsp[0].minor.yy0, status->scanner_state);
}
// 3726 "parser.c"
        break;
      case 51:
// 1219 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_property_shortcut(yymsp[-1].minor.yy0, yymsp[0].minor.yy0, status->scanner_state);
}
// 3733 "parser.c"
        break;
      case 58:
// 1248 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy235, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(50,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 3743 "parser.c"
        break;
      case 59:
// 1252 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy235, NULL, status->scanner_state);
  yy_destructor(50,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 3753 "parser.c"
        break;
      case 60:
// 1257 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class_method(yymsp[-6].minor.yy235, yymsp[-4].minor.yy0, NULL, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(52,&yymsp[-5].minor);
  yy_destructor(53,&yymsp[-3].minor);
  yy_destructor(36,&yymsp[-2].minor);
  yy_destructor(46,&yymsp[-1].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 3765 "parser.c"
        break;
      case 61:
// 1261 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class_method(yymsp[-7].minor.yy235, yymsp[-5].minor.yy0, yymsp[-3].minor.yy235, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(52,&yymsp[-6].minor);
  yy_destructor(53,&yymsp[-4].minor);
  yy_destructor(36,&yymsp[-2].minor);
  yy_destructor(46,&yymsp[-1].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 3777 "parser.c"
        break;
      case 62:
// 1265 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class_method(yymsp[-7].minor.yy235, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy235, NULL, NULL, status->scanner_state);
  yy_destructor(52,&yymsp[-6].minor);
  yy_destructor(53,&yymsp[-4].minor);
  yy_destructor(36,&yymsp[-3].minor);
  yy_destructor(46,&yymsp[-2].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 3789 "parser.c"
        break;
      case 63:
// 1269 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class_method(yymsp[-8].minor.yy235, yymsp[-6].minor.yy0, yymsp[-4].minor.yy235, yymsp[-1].minor.yy235, NULL, NULL, status->scanner_state);
  yy_destructor(52,&yymsp[-7].minor);
  yy_destructor(53,&yymsp[-5].minor);
  yy_destructor(36,&yymsp[-3].minor);
  yy_destructor(46,&yymsp[-2].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 3801 "parser.c"
        break;
      case 64:
// 1273 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class_method(yymsp[-6].minor.yy235, yymsp[-4].minor.yy0, NULL, NULL, yymsp[-7].minor.yy0, NULL, status->scanner_state);
  yy_destructor(52,&yymsp[-5].minor);
  yy_destructor(53,&yymsp[-3].minor);
  yy_destructor(36,&yymsp[-2].minor);
  yy_destructor(46,&yymsp[-1].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 3813 "parser.c"
        break;
      case 65:
// 1277 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class_method(yymsp[-7].minor.yy235, yymsp[-5].minor.yy0, yymsp[-3].minor.yy235, NULL, yymsp[-8].minor.yy0, NULL, status->scanner_state);
  yy_destructor(52,&yymsp[-6].minor);
  yy_destructor(53,&yymsp[-4].minor);
  yy_destructor(36,&yymsp[-2].minor);
  yy_destructor(46,&yymsp[-1].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 3825 "parser.c"
        break;
      case 66:
// 1281 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class_method(yymsp[-7].minor.yy235, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy235, yymsp[-8].minor.yy0, NULL, status->scanner_state);
  yy_destructor(52,&yymsp[-6].minor);
  yy_destructor(53,&yymsp[-4].minor);
  yy_destructor(36,&yymsp[-3].minor);
  yy_destructor(46,&yymsp[-2].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 3837 "parser.c"
        break;
      case 67:
// 1285 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class_method(yymsp[-8].minor.yy235, yymsp[-6].minor.yy0, yymsp[-4].minor.yy235, yymsp[-1].minor.yy235, yymsp[-9].minor.yy0, NULL, status->scanner_state);
  yy_destructor(52,&yymsp[-7].minor);
  yy_destructor(53,&yymsp[-5].minor);
  yy_destructor(36,&yymsp[-3].minor);
  yy_destructor(46,&yymsp[-2].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 3849 "parser.c"
        break;
      case 68:
// 1289 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class_method(yymsp[-8].minor.yy235, yymsp[-6].minor.yy0, NULL, NULL, NULL, yymsp[-2].minor.yy235, status->scanner_state);
  yy_destructor(52,&yymsp[-7].minor);
  yy_destructor(53,&yymsp[-5].minor);
  yy_destructor(36,&yymsp[-4].minor);
  yy_destructor(54,&yymsp[-3].minor);
  yy_destructor(46,&yymsp[-1].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 3862 "parser.c"
        break;
      case 69:
// 1293 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class_method(yymsp[-9].minor.yy235, yymsp[-7].minor.yy0, yymsp[-5].minor.yy235, NULL, NULL, yymsp[-2].minor.yy235, status->scanner_state);
  yy_destructor(52,&yymsp[-8].minor);
  yy_destructor(53,&yymsp[-6].minor);
  yy_destructor(36,&yymsp[-4].minor);
  yy_destructor(54,&yymsp[-3].minor);
  yy_destructor(46,&yymsp[-1].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 3875 "parser.c"
        break;
      case 70:
// 1297 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class_method(yymsp[-9].minor.yy235, yymsp[-7].minor.yy0, NULL, yymsp[-1].minor.yy235, NULL, yymsp[-3].minor.yy235, status->scanner_state);
  yy_destructor(52,&yymsp[-8].minor);
  yy_destructor(53,&yymsp[-6].minor);
  yy_destructor(36,&yymsp[-5].minor);
  yy_destructor(54,&yymsp[-4].minor);
  yy_destructor(46,&yymsp[-2].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 3888 "parser.c"
        break;
      case 71:
// 1301 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class_method(yymsp[-10].minor.yy235, yymsp[-8].minor.yy0, yymsp[-6].minor.yy235, yymsp[-1].minor.yy235, NULL, yymsp[-3].minor.yy235, status->scanner_state);
  yy_destructor(52,&yymsp[-9].minor);
  yy_destructor(53,&yymsp[-7].minor);
  yy_destructor(36,&yymsp[-5].minor);
  yy_destructor(54,&yymsp[-4].minor);
  yy_destructor(46,&yymsp[-2].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 3901 "parser.c"
        break;
      case 72:
// 1305 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class_method(yymsp[-8].minor.yy235, yymsp[-6].minor.yy0, NULL, NULL, yymsp[-9].minor.yy0, yymsp[-2].minor.yy235, status->scanner_state);
  yy_destructor(52,&yymsp[-7].minor);
  yy_destructor(53,&yymsp[-5].minor);
  yy_destructor(36,&yymsp[-4].minor);
  yy_destructor(54,&yymsp[-3].minor);
  yy_destructor(46,&yymsp[-1].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 3914 "parser.c"
        break;
      case 73:
// 1309 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class_method(yymsp[-9].minor.yy235, yymsp[-7].minor.yy0, yymsp[-5].minor.yy235, NULL, yymsp[-10].minor.yy0, yymsp[-2].minor.yy235, status->scanner_state);
  yy_destructor(52,&yymsp[-8].minor);
  yy_destructor(53,&yymsp[-6].minor);
  yy_destructor(36,&yymsp[-4].minor);
  yy_destructor(54,&yymsp[-3].minor);
  yy_destructor(46,&yymsp[-1].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 3927 "parser.c"
        break;
      case 74:
// 1313 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class_method(yymsp[-9].minor.yy235, yymsp[-7].minor.yy0, NULL, yymsp[-1].minor.yy235, yymsp[-10].minor.yy0, yymsp[-3].minor.yy235, status->scanner_state);
  yy_destructor(52,&yymsp[-8].minor);
  yy_destructor(53,&yymsp[-6].minor);
  yy_destructor(36,&yymsp[-5].minor);
  yy_destructor(54,&yymsp[-4].minor);
  yy_destructor(46,&yymsp[-2].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 3940 "parser.c"
        break;
      case 75:
// 1317 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class_method(yymsp[-10].minor.yy235, yymsp[-8].minor.yy0, yymsp[-6].minor.yy235, yymsp[-1].minor.yy235, yymsp[-11].minor.yy0, yymsp[-3].minor.yy235, status->scanner_state);
  yy_destructor(52,&yymsp[-9].minor);
  yy_destructor(53,&yymsp[-7].minor);
  yy_destructor(36,&yymsp[-5].minor);
  yy_destructor(54,&yymsp[-4].minor);
  yy_destructor(46,&yymsp[-2].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 3953 "parser.c"
        break;
      case 76:
// 1322 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class_method(yymsp[-7].minor.yy235, yymsp[-5].minor.yy0, NULL, NULL, NULL, yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(52,&yymsp[-6].minor);
  yy_destructor(53,&yymsp[-4].minor);
  yy_destructor(36,&yymsp[-3].minor);
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 3965 "parser.c"
        break;
      case 77:
// 1326 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class_method(yymsp[-8].minor.yy235, yymsp[-6].minor.yy0, yymsp[-4].minor.yy235, NULL, NULL, yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(52,&yymsp[-7].minor);
  yy_destructor(53,&yymsp[-5].minor);
  yy_destructor(36,&yymsp[-3].minor);
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 3977 "parser.c"
        break;
      case 78:
// 1330 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class_method(yymsp[-7].minor.yy235, yymsp[-5].minor.yy0, NULL, NULL, yymsp[-8].minor.yy0, yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(52,&yymsp[-6].minor);
  yy_destructor(53,&yymsp[-4].minor);
  yy_destructor(36,&yymsp[-3].minor);
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 3989 "parser.c"
        break;
      case 79:
// 1334 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class_method(yymsp[-8].minor.yy235, yymsp[-6].minor.yy0, yymsp[-4].minor.yy235, NULL, yymsp[-9].minor.yy0, yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(52,&yymsp[-7].minor);
  yy_destructor(53,&yymsp[-5].minor);
  yy_destructor(36,&yymsp[-3].minor);
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 4001 "parser.c"
        break;
      case 80:
// 1338 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class_method(yymsp[-5].minor.yy235, yymsp[-3].minor.yy0, NULL, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(52,&yymsp[-4].minor);
  yy_destructor(53,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[-1].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 4012 "parser.c"
        break;
      case 81:
// 1342 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class_method(yymsp[-6].minor.yy235, yymsp[-4].minor.yy0, yymsp[-2].minor.yy235, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(52,&yymsp[-5].minor);
  yy_destructor(53,&yymsp[-3].minor);
  yy_destructor(36,&yymsp[-1].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 4023 "parser.c"
        break;
      case 82:
// 1346 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class_method(yymsp[-5].minor.yy235, yymsp[-3].minor.yy0, NULL, NULL, yymsp[-6].minor.yy0, NULL, status->scanner_state);
  yy_destructor(52,&yymsp[-4].minor);
  yy_destructor(53,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[-1].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 4034 "parser.c"
        break;
      case 83:
// 1350 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class_method(yymsp[-6].minor.yy235, yymsp[-4].minor.yy0, yymsp[-2].minor.yy235, NULL, yymsp[-7].minor.yy0, NULL, status->scanner_state);
  yy_destructor(52,&yymsp[-5].minor);
  yy_destructor(53,&yymsp[-3].minor);
  yy_destructor(36,&yymsp[-1].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 4045 "parser.c"
        break;
      case 86:
// 1363 "parser.lemon"
{
	yygotominor.yy235 = json_object_new_string("public");
  yy_destructor(1,&yymsp[0].minor);
}
// 4053 "parser.c"
        break;
      case 87:
// 1367 "parser.lemon"
{
	yygotominor.yy235 = json_object_new_string("protected");
  yy_destructor(2,&yymsp[0].minor);
}
// 4061 "parser.c"
        break;
      case 88:
// 1371 "parser.lemon"
{
	yygotominor.yy235 = json_object_new_string("private");
  yy_destructor(4,&yymsp[0].minor);
}
// 4069 "parser.c"
        break;
      case 89:
// 1375 "parser.lemon"
{
	yygotominor.yy235 = json_object_new_string("static");
  yy_destructor(3,&yymsp[0].minor);
}
// 4077 "parser.c"
        break;
      case 90:
// 1379 "parser.lemon"
{
	yygotominor.yy235 = json_object_new_string("scoped");
  yy_destructor(5,&yymsp[0].minor);
}
// 4085 "parser.c"
        break;
      case 91:
// 1383 "parser.lemon"
{
	yygotominor.yy235 = json_object_new_string("inline");
  yy_destructor(55,&yymsp[0].minor);
}
// 4093 "parser.c"
        break;
      case 92:
// 1387 "parser.lemon"
{
	yygotominor.yy235 = json_object_new_string("abstract");
  yy_destructor(44,&yymsp[0].minor);
}
// 4101 "parser.c"
        break;
      case 93:
// 1391 "parser.lemon"
{
	yygotominor.yy235 = json_object_new_string("final");
  yy_destructor(45,&yymsp[0].minor);
}
// 4109 "parser.c"
        break;
      case 94:
// 1396 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_return_type(1, NULL, status->scanner_state);
  yy_destructor(56,&yymsp[0].minor);
}
// 4117 "parser.c"
        break;
      case 95:
// 1400 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_return_type(0, yymsp[0].minor.yy235, status->scanner_state);
}
// 4124 "parser.c"
        break;
      case 96:
// 1404 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_list(yymsp[-2].minor.yy235, yymsp[0].minor.yy235);
  yy_destructor(15,&yymsp[-1].minor);
}
// 4132 "parser.c"
        break;
      case 98:
// 1412 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_return_type_item(yymsp[0].minor.yy235, NULL, 0, 0, status->scanner_state);
}
// 4139 "parser.c"
        break;
      case 99:
// 1416 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_return_type_item(yymsp[-1].minor.yy235, NULL, 1, 0, status->scanner_state);
  yy_destructor(35,&yymsp[0].minor);
}
// 4147 "parser.c"
        break;
      case 100:
// 1420 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_return_type_item(NULL, yymsp[0].minor.yy235, 0, 0, status->scanner_state);
}
// 4154 "parser.c"
        break;
      case 101:
// 1424 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_return_type_item(NULL, yymsp[0].minor.yy235, 0, 1, status->scanner_state);
}
// 4161 "parser.c"
        break;
      case 104:
// 1437 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_parameter(NULL, NULL, yymsp[0].minor.yy0, NULL, 0, status->scanner_state);
}
// 4168 "parser.c"
        break;
      case 105:
// 1441 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_parameter(yymsp[-1].minor.yy235, NULL, yymsp[0].minor.yy0, NULL, 0, status->scanner_state);
}
// 4175 "parser.c"
        break;
      case 106:
// 1445 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_parameter(yymsp[-2].minor.yy235, NULL, yymsp[0].minor.yy0, NULL, 1, status->scanner_state);
  yy_destructor(35,&yymsp[-1].minor);
}
// 4183 "parser.c"
        break;
      case 107:
// 1449 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_parameter(NULL, yymsp[-1].minor.yy235, yymsp[0].minor.yy0, NULL, 0, status->scanner_state);
}
// 4190 "parser.c"
        break;
      case 108:
// 1453 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_parameter(NULL, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy235, 0, status->scanner_state);
  yy_destructor(49,&yymsp[-1].minor);
}
// 4198 "parser.c"
        break;
      case 109:
// 1457 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_parameter(yymsp[-3].minor.yy235, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy235, 0, status->scanner_state);
  yy_destructor(49,&yymsp[-1].minor);
}
// 4206 "parser.c"
        break;
      case 110:
// 1461 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_parameter(yymsp[-4].minor.yy235, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy235, 1, status->scanner_state);
  yy_destructor(35,&yymsp[-3].minor);
  yy_destructor(49,&yymsp[-1].minor);
}
// 4215 "parser.c"
        break;
      case 111:
// 1465 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_parameter(NULL, yymsp[-3].minor.yy235, yymsp[-2].minor.yy0, yymsp[0].minor.yy235, 0, status->scanner_state);
  yy_destructor(49,&yymsp[-1].minor);
}
// 4223 "parser.c"
        break;
      case 112:
// 1469 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(19,&yymsp[-2].minor);
  yy_destructor(20,&yymsp[0].minor);
}
// 4232 "parser.c"
        break;
      case 113:
// 1473 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(19,&yymsp[-4].minor);
  yy_destructor(57,&yymsp[-2].minor);
  yy_destructor(58,&yymsp[-1].minor);
  yy_destructor(20,&yymsp[0].minor);
}
// 4243 "parser.c"
        break;
      case 114:
// 1477 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_type(XX_TYPE_INTEGER);
  yy_destructor(59,&yymsp[0].minor);
}
// 4251 "parser.c"
        break;
      case 115:
// 1481 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_type(XX_TYPE_UINTEGER);
  yy_destructor(60,&yymsp[0].minor);
}
// 4259 "parser.c"
        break;
      case 116:
// 1485 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_type(XX_TYPE_LONG);
  yy_destructor(61,&yymsp[0].minor);
}
// 4267 "parser.c"
        break;
      case 117:
// 1489 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_type(XX_TYPE_ULONG);
  yy_destructor(62,&yymsp[0].minor);
}
// 4275 "parser.c"
        break;
      case 118:
// 1493 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_type(XX_TYPE_CHAR);
  yy_destructor(63,&yymsp[0].minor);
}
// 4283 "parser.c"
        break;
      case 119:
// 1497 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_type(XX_TYPE_UCHAR);
  yy_destructor(64,&yymsp[0].minor);
}
// 4291 "parser.c"
        break;
      case 120:
// 1501 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_type(XX_TYPE_DOUBLE);
  yy_destructor(65,&yymsp[0].minor);
}
// 4299 "parser.c"
        break;
      case 121:
// 1505 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_type(XX_TYPE_BOOL);
  yy_destructor(66,&yymsp[0].minor);
}
// 4307 "parser.c"
        break;
      case 122:
// 1509 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_type(XX_TYPE_STRING);
  yy_destructor(67,&yymsp[0].minor);
}
// 4315 "parser.c"
        break;
      case 123:
// 1513 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_type(XX_TYPE_ARRAY);
  yy_destructor(68,&yymsp[0].minor);
}
// 4323 "parser.c"
        break;
      case 124:
// 1517 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_type(XX_TYPE_VAR);
  yy_destructor(69,&yymsp[0].minor);
}
// 4331 "parser.c"
        break;
      case 147:
// 1609 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_empty_statement(status->scanner_state);
  yy_destructor(39,&yymsp[0].minor);
}
// 4339 "parser.c"
        break;
      case 148:
// 1613 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_break_statement(status->scanner_state);
  yy_destructor(70,&yymsp[-1].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 4348 "parser.c"
        break;
      case 149:
// 1617 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_continue_statement(status->scanner_state);
  yy_destructor(71,&yymsp[-1].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 4357 "parser.c"
        break;
      case 150:
// 1621 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_if_statement(yymsp[-2].minor.yy235, NULL, NULL, status->scanner_state);
  yy_destructor(72,&yymsp[-3].minor);
  yy_destructor(46,&yymsp[-1].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 4367 "parser.c"
        break;
      case 151:
// 1625 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_if_statement(yymsp[-5].minor.yy235, NULL, NULL, status->scanner_state);
  yy_destructor(72,&yymsp[-6].minor);
  yy_destructor(46,&yymsp[-4].minor);
  yy_destructor(47,&yymsp[-3].minor);
  yy_destructor(73,&yymsp[-2].minor);
  yy_destructor(46,&yymsp[-1].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 4380 "parser.c"
        break;
      case 152:
// 1629 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_if_statement(yymsp[-3].minor.yy235, yymsp[-1].minor.yy235, NULL, status->scanner_state);
  yy_destructor(72,&yymsp[-4].minor);
  yy_destructor(46,&yymsp[-2].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 4390 "parser.c"
        break;
      case 153:
// 1633 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_if_statement(yymsp[-7].minor.yy235, yymsp[-5].minor.yy235, yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(72,&yymsp[-8].minor);
  yy_destructor(46,&yymsp[-6].minor);
  yy_destructor(47,&yymsp[-4].minor);
  yy_destructor(73,&yymsp[-3].minor);
  yy_destructor(46,&yymsp[-2].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 4403 "parser.c"
        break;
      case 154:
// 1637 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_if_statement(yymsp[-6].minor.yy235, yymsp[-4].minor.yy235, NULL, status->scanner_state);
  yy_destructor(72,&yymsp[-7].minor);
  yy_destructor(46,&yymsp[-5].minor);
  yy_destructor(47,&yymsp[-3].minor);
  yy_destructor(73,&yymsp[-2].minor);
  yy_destructor(46,&yymsp[-1].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 4416 "parser.c"
        break;
      case 155:
// 1641 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_if_statement(yymsp[-6].minor.yy235, NULL, yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(72,&yymsp[-7].minor);
  yy_destructor(46,&yymsp[-5].minor);
  yy_destructor(47,&yymsp[-4].minor);
  yy_destructor(73,&yymsp[-3].minor);
  yy_destructor(46,&yymsp[-2].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 4429 "parser.c"
        break;
      case 156:
// 1645 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_switch_statement(yymsp[-2].minor.yy235, NULL, status->scanner_state);
  yy_destructor(74,&yymsp[-3].minor);
  yy_destructor(46,&yymsp[-1].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 4439 "parser.c"
        break;
      case 157:
// 1649 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_switch_statement(yymsp[-3].minor.yy235, yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(74,&yymsp[-4].minor);
  yy_destructor(46,&yymsp[-2].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 4449 "parser.c"
        break;
      case 160:
// 1661 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_case_clause(yymsp[-1].minor.yy235, NULL, status->scanner_state);
  yy_destructor(75,&yymsp[-2].minor);
  yy_destructor(76,&yymsp[0].minor);
}
// 4458 "parser.c"
        break;
      case 161:
// 1665 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_case_clause(yymsp[-2].minor.yy235, yymsp[0].minor.yy235, status->scanner_state);
  yy_destructor(75,&yymsp[-3].minor);
  yy_destructor(76,&yymsp[-1].minor);
}
// 4467 "parser.c"
        break;
      case 162:
// 1669 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_case_clause(NULL, yymsp[0].minor.yy235, status->scanner_state);
  yy_destructor(77,&yymsp[-2].minor);
  yy_destructor(76,&yymsp[-1].minor);
}
// 4476 "parser.c"
        break;
      case 163:
// 1673 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_loop_statement(NULL, status->scanner_state);
  yy_destructor(78,&yymsp[-2].minor);
  yy_destructor(46,&yymsp[-1].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 4486 "parser.c"
        break;
      case 164:
// 1677 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_loop_statement(yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(78,&yymsp[-3].minor);
  yy_destructor(46,&yymsp[-2].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 4496 "parser.c"
        break;
      case 165:
// 1681 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_while_statement(yymsp[-2].minor.yy235, NULL, status->scanner_state);
  yy_destructor(79,&yymsp[-3].minor);
  yy_destructor(46,&yymsp[-1].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 4506 "parser.c"
        break;
      case 166:
// 1685 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_while_statement(yymsp[-3].minor.yy235, yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(79,&yymsp[-4].minor);
  yy_destructor(46,&yymsp[-2].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 4516 "parser.c"
        break;
      case 167:
// 1689 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_do_while_statement(yymsp[-1].minor.yy235, NULL, status->scanner_state);
  yy_destructor(80,&yymsp[-5].minor);
  yy_destructor(46,&yymsp[-4].minor);
  yy_destructor(47,&yymsp[-3].minor);
  yy_destructor(79,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 4528 "parser.c"
        break;
      case 168:
// 1693 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_do_while_statement(yymsp[-1].minor.yy235, yymsp[-4].minor.yy235, status->scanner_state);
  yy_destructor(80,&yymsp[-6].minor);
  yy_destructor(46,&yymsp[-5].minor);
  yy_destructor(47,&yymsp[-3].minor);
  yy_destructor(79,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 4540 "parser.c"
        break;
      case 169:
// 1697 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_for_statement(yymsp[-3].minor.yy235, NULL, yymsp[-5].minor.yy0, 0, yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(81,&yymsp[-6].minor);
  yy_destructor(82,&yymsp[-4].minor);
  yy_destructor(46,&yymsp[-2].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 4551 "parser.c"
        break;
      case 170:
// 1701 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_for_statement(yymsp[-3].minor.yy235, NULL, yymsp[-6].minor.yy0, 1, yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(81,&yymsp[-7].minor);
  yy_destructor(82,&yymsp[-5].minor);
  yy_destructor(83,&yymsp[-4].minor);
  yy_destructor(46,&yymsp[-2].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 4563 "parser.c"
        break;
      case 171:
// 1705 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_for_statement(yymsp[-3].minor.yy235, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, 0, yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(81,&yymsp[-8].minor);
  yy_destructor(6,&yymsp[-6].minor);
  yy_destructor(82,&yymsp[-4].minor);
  yy_destructor(46,&yymsp[-2].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 4575 "parser.c"
        break;
      case 172:
// 1709 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_for_statement(yymsp[-3].minor.yy235, yymsp[-8].minor.yy0, yymsp[-6].minor.yy0, 1, yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(81,&yymsp[-9].minor);
  yy_destructor(6,&yymsp[-7].minor);
  yy_destructor(82,&yymsp[-5].minor);
  yy_destructor(83,&yymsp[-4].minor);
  yy_destructor(46,&yymsp[-2].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 4588 "parser.c"
        break;
      case 173:
// 1713 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_let_statement(yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(84,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 4597 "parser.c"
        break;
      case 176:
// 1726 "parser.lemon"
{
	yygotominor.yy235 = json_object_new_string("assign");
  yy_destructor(49,&yymsp[0].minor);
}
// 4605 "parser.c"
        break;
      case 177:
// 1731 "parser.lemon"
{
	yygotominor.yy235 = json_object_new_string("add-assign");
  yy_destructor(85,&yymsp[0].minor);
}
// 4613 "parser.c"
        break;
      case 178:
// 1736 "parser.lemon"
{
	yygotominor.yy235 = json_object_new_string("sub-assign");
  yy_destructor(86,&yymsp[0].minor);
}
// 4621 "parser.c"
        break;
      case 179:
// 1740 "parser.lemon"
{
	yygotominor.yy235 = json_object_new_string("mult-assign");
  yy_destructor(87,&yymsp[0].minor);
}
// 4629 "parser.c"
        break;
      case 180:
// 1744 "parser.lemon"
{
	yygotominor.yy235 = json_object_new_string("div-assign");
  yy_destructor(88,&yymsp[0].minor);
}
// 4637 "parser.c"
        break;
      case 181:
// 1748 "parser.lemon"
{
	yygotominor.yy235 = json_object_new_string("concat-assign");
  yy_destructor(89,&yymsp[0].minor);
}
// 4645 "parser.c"
        break;
      case 182:
// 1753 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_let_assignment("variable", yymsp[-1].minor.yy235, yymsp[-2].minor.yy0, NULL, NULL, yymsp[0].minor.yy235, status->scanner_state);
}
// 4652 "parser.c"
        break;
      case 183:
// 1758 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_let_assignment("object-property", yymsp[-1].minor.yy235, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, yymsp[0].minor.yy235, status->scanner_state);
  yy_destructor(54,&yymsp[-3].minor);
}
// 4660 "parser.c"
        break;
      case 184:
// 1763 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_let_assignment("variable-dynamic-object-property", yymsp[-1].minor.yy235, yymsp[-6].minor.yy0, yymsp[-3].minor.yy0, NULL, yymsp[0].minor.yy235, status->scanner_state);
  yy_destructor(54,&yymsp[-5].minor);
  yy_destructor(46,&yymsp[-4].minor);
  yy_destructor(47,&yymsp[-2].minor);
}
// 4670 "parser.c"
        break;
      case 185:
// 1768 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_let_assignment("string-dynamic-object-property", yymsp[-1].minor.yy235, yymsp[-6].minor.yy0, yymsp[-3].minor.yy0, NULL, yymsp[0].minor.yy235, status->scanner_state);
  yy_destructor(54,&yymsp[-5].minor);
  yy_destructor(46,&yymsp[-4].minor);
  yy_destructor(47,&yymsp[-2].minor);
}
// 4680 "parser.c"
        break;
      case 186:
// 1773 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_let_assignment("object-property-append", yymsp[-1].minor.yy235, yymsp[-6].minor.yy0, yymsp[-4].minor.yy0, NULL, yymsp[0].minor.yy235, status->scanner_state);
  yy_destructor(54,&yymsp[-5].minor);
  yy_destructor(57,&yymsp[-3].minor);
  yy_destructor(58,&yymsp[-2].minor);
}
// 4690 "parser.c"
        break;
      case 187:
// 1778 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_let_assignment("object-property-array-index", yymsp[-1].minor.yy235, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, yymsp[-3].minor.yy235, yymsp[0].minor.yy235, status->scanner_state);
  yy_destructor(54,&yymsp[-6].minor);
  yy_destructor(57,&yymsp[-4].minor);
  yy_destructor(58,&yymsp[-2].minor);
}
// 4700 "parser.c"
        break;
      case 188:
// 1783 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_let_assignment("static-property", yymsp[-1].minor.yy235, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, yymsp[0].minor.yy235, status->scanner_state);
  yy_destructor(91,&yymsp[-3].minor);
}
// 4708 "parser.c"
        break;
      case 189:
// 1788 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_let_assignment("static-property-append", yymsp[-1].minor.yy235, yymsp[-6].minor.yy0, yymsp[-4].minor.yy0, NULL, yymsp[0].minor.yy235, status->scanner_state);
  yy_destructor(91,&yymsp[-5].minor);
  yy_destructor(57,&yymsp[-3].minor);
  yy_destructor(58,&yymsp[-2].minor);
}
// 4718 "parser.c"
        break;
      case 190:
// 1793 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_let_assignment("static-property-array-index", yymsp[-1].minor.yy235, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, yymsp[-3].minor.yy235, yymsp[0].minor.yy235, status->scanner_state);
  yy_destructor(91,&yymsp[-6].minor);
  yy_destructor(57,&yymsp[-4].minor);
  yy_destructor(58,&yymsp[-2].minor);
}
// 4728 "parser.c"
        break;
      case 191:
// 1798 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_let_assignment("variable-append", yymsp[-1].minor.yy235, yymsp[-4].minor.yy0, NULL, NULL, yymsp[0].minor.yy235, status->scanner_state);
  yy_destructor(57,&yymsp[-3].minor);
  yy_destructor(58,&yymsp[-2].minor);
}
// 4737 "parser.c"
        break;
      case 192:
// 1803 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_let_assignment("array-index", yymsp[-1].minor.yy235, yymsp[-5].minor.yy0, NULL, yymsp[-3].minor.yy235, yymsp[0].minor.yy235, status->scanner_state);
  yy_destructor(57,&yymsp[-4].minor);
  yy_destructor(58,&yymsp[-2].minor);
}
// 4746 "parser.c"
        break;
      case 193:
// 1808 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_let_assignment("incr", NULL, yymsp[-3].minor.yy0, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(92,&yymsp[0].minor);
}
// 4755 "parser.c"
        break;
      case 194:
// 1813 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_let_assignment("decr", NULL, yymsp[-3].minor.yy0, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(93,&yymsp[0].minor);
}
// 4764 "parser.c"
        break;
      case 195:
// 1818 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_let_assignment("incr", NULL, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(92,&yymsp[0].minor);
}
// 4772 "parser.c"
        break;
      case 196:
// 1823 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_let_assignment("decr", NULL, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(93,&yymsp[0].minor);
}
// 4780 "parser.c"
        break;
      case 198:
// 1831 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_echo_statement(yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(94,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 4789 "parser.c"
        break;
      case 201:
// 1843 "parser.lemon"
{
	yygotominor.yy235 = yymsp[0].minor.yy235;;
}
// 4796 "parser.c"
        break;
      case 202:
// 1848 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_mcall_statement(yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(39,&yymsp[0].minor);
}
// 4804 "parser.c"
        break;
      case 203:
// 1853 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_fcall_statement(yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(39,&yymsp[0].minor);
}
// 4812 "parser.c"
        break;
      case 204:
// 1858 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_scall_statement(yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(39,&yymsp[0].minor);
}
// 4820 "parser.c"
        break;
      case 205:
// 1863 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_fetch_statement(yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(39,&yymsp[0].minor);
}
// 4828 "parser.c"
        break;
      case 206:
// 1868 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_return_statement(yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(95,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 4837 "parser.c"
        break;
      case 207:
// 1873 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_return_statement(NULL, status->scanner_state);
  yy_destructor(95,&yymsp[-1].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 4846 "parser.c"
        break;
      case 208:
// 1878 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(96,&yymsp[-4].minor);
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 4860 "parser.c"
        break;
      case 209:
// 1887 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state),
		yymsp[-2].minor.yy235,
		status->scanner_state
	);
  yy_destructor(96,&yymsp[-5].minor);
  yy_destructor(57,&yymsp[-3].minor);
  yy_destructor(58,&yymsp[-1].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 4875 "parser.c"
        break;
      case 210:
// 1896 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_throw_exception(yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(97,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 4884 "parser.c"
        break;
      case 211:
// 1900 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_declare_statement(XX_T_TYPE_INTEGER, yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(59,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 4893 "parser.c"
        break;
      case 212:
// 1904 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_declare_statement(XX_T_TYPE_UINTEGER, yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(60,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 4902 "parser.c"
        break;
      case 213:
// 1908 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_declare_statement(XX_T_TYPE_CHAR, yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(63,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 4911 "parser.c"
        break;
      case 214:
// 1912 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_declare_statement(XX_T_TYPE_UCHAR, yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(64,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 4920 "parser.c"
        break;
      case 215:
// 1916 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_declare_statement(XX_T_TYPE_LONG, yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(61,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 4929 "parser.c"
        break;
      case 216:
// 1920 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_declare_statement(XX_T_TYPE_ULONG, yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(62,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 4938 "parser.c"
        break;
      case 217:
// 1924 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_declare_statement(XX_T_TYPE_DOUBLE, yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(65,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 4947 "parser.c"
        break;
      case 218:
// 1928 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_declare_statement(XX_T_TYPE_STRING, yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(67,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 4956 "parser.c"
        break;
      case 219:
// 1932 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_declare_statement(XX_T_TYPE_BOOL, yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(66,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 4965 "parser.c"
        break;
      case 220:
// 1936 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_declare_statement(XX_T_TYPE_VAR, yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(69,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 4974 "parser.c"
        break;
      case 223:
// 1948 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_declare_variable(yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 4981 "parser.c"
        break;
      case 224:
// 1952 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_declare_variable(yymsp[-2].minor.yy0, yymsp[0].minor.yy235, status->scanner_state);
  yy_destructor(49,&yymsp[-1].minor);
}
// 4989 "parser.c"
        break;
      case 226:
// 1960 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_expr("not", yymsp[0].minor.yy235, NULL, NULL, status->scanner_state);
  yy_destructor(35,&yymsp[-1].minor);
}
// 4997 "parser.c"
        break;
      case 227:
// 1964 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_expr("isset", yymsp[0].minor.yy235, NULL, NULL, status->scanner_state);
  yy_destructor(31,&yymsp[-1].minor);
}
// 5005 "parser.c"
        break;
      case 228:
// 1968 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_expr("require", yymsp[0].minor.yy235, NULL, NULL, status->scanner_state);
  yy_destructor(7,&yymsp[-1].minor);
}
// 5013 "parser.c"
        break;
      case 229:
// 1972 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_expr("clone", yymsp[0].minor.yy235, NULL, NULL, status->scanner_state);
  yy_destructor(10,&yymsp[-1].minor);
}
// 5021 "parser.c"
        break;
      case 230:
// 1976 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_expr("empty", yymsp[0].minor.yy235, NULL, NULL, status->scanner_state);
  yy_destructor(11,&yymsp[-1].minor);
}
// 5029 "parser.c"
        break;
      case 231:
// 1980 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_expr("likely", yymsp[0].minor.yy235, NULL, NULL, status->scanner_state);
  yy_destructor(8,&yymsp[-1].minor);
}
// 5037 "parser.c"
        break;
      case 232:
// 1984 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_expr("unlikely", yymsp[0].minor.yy235, NULL, NULL, status->scanner_state);
  yy_destructor(9,&yymsp[-1].minor);
}
// 5045 "parser.c"
        break;
      case 233:
// 1988 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_expr("equals", yymsp[-2].minor.yy235, yymsp[0].minor.yy235, NULL, status->scanner_state);
  yy_destructor(17,&yymsp[-1].minor);
}
// 5053 "parser.c"
        break;
      case 234:
// 1992 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_expr("not-equals", yymsp[-2].minor.yy235, yymsp[0].minor.yy235, NULL, status->scanner_state);
  yy_destructor(24,&yymsp[-1].minor);
}
// 5061 "parser.c"
        break;
      case 235:
// 1996 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_expr("identical", yymsp[-2].minor.yy235, yymsp[0].minor.yy235, NULL, status->scanner_state);
  yy_destructor(18,&yymsp[-1].minor);
}
// 5069 "parser.c"
        break;
      case 236:
// 2000 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_expr("not-identical", yymsp[-2].minor.yy235, yymsp[0].minor.yy235, NULL, status->scanner_state);
  yy_destructor(23,&yymsp[-1].minor);
}
// 5077 "parser.c"
        break;
      case 237:
// 2004 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_expr("less", yymsp[-2].minor.yy235, yymsp[0].minor.yy235, NULL, status->scanner_state);
  yy_destructor(19,&yymsp[-1].minor);
}
// 5085 "parser.c"
        break;
      case 238:
// 2008 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_expr("greater", yymsp[-2].minor.yy235, yymsp[0].minor.yy235, NULL, status->scanner_state);
  yy_destructor(20,&yymsp[-1].minor);
}
// 5093 "parser.c"
        break;
      case 239:
// 2012 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_expr("less-equal", yymsp[-2].minor.yy235, yymsp[0].minor.yy235, NULL, status->scanner_state);
  yy_destructor(21,&yymsp[-1].minor);
}
// 5101 "parser.c"
        break;
      case 240:
// 2016 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_expr("greater-equal", yymsp[-2].minor.yy235, yymsp[0].minor.yy235, NULL, status->scanner_state);
  yy_destructor(22,&yymsp[-1].minor);
}
// 5109 "parser.c"
        break;
      case 241:
// 2020 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_expr("list", yymsp[-1].minor.yy235, NULL, NULL, status->scanner_state);
  yy_destructor(53,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5118 "parser.c"
        break;
      case 242:
// 2024 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_expr("cast", yymsp[-2].minor.yy235, yymsp[0].minor.yy235, NULL, status->scanner_state);
  yy_destructor(53,&yymsp[-3].minor);
  yy_destructor(36,&yymsp[-1].minor);
}
// 5127 "parser.c"
        break;
      case 243:
// 2028 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_expr("type-hint", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), yymsp[0].minor.yy235, NULL, status->scanner_state);
  yy_destructor(19,&yymsp[-3].minor);
  yy_destructor(20,&yymsp[-1].minor);
}
// 5136 "parser.c"
        break;
      case 244:
      case 319:
// 2032 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_expr("property-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(54,&yymsp[-1].minor);
}
// 5145 "parser.c"
        break;
      case 245:
      case 320:
// 2036 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_expr("property-dynamic-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(54,&yymsp[-3].minor);
  yy_destructor(46,&yymsp[-2].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 5156 "parser.c"
        break;
      case 246:
      case 321:
// 2040 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_expr("property-string-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_STRING, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(54,&yymsp[-3].minor);
  yy_destructor(46,&yymsp[-2].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 5167 "parser.c"
        break;
      case 247:
// 2044 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_expr("static-property-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(91,&yymsp[-1].minor);
}
// 5175 "parser.c"
        break;
      case 248:
      case 316:
// 2048 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_expr("static-constant-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(91,&yymsp[-1].minor);
}
// 5184 "parser.c"
        break;
      case 249:
      case 318:
// 2053 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), yymsp[-1].minor.yy235, NULL, status->scanner_state);
  yy_destructor(57,&yymsp[-2].minor);
  yy_destructor(58,&yymsp[0].minor);
}
// 5194 "parser.c"
        break;
      case 250:
// 2058 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_expr("add", yymsp[-2].minor.yy235, yymsp[0].minor.yy235, NULL, status->scanner_state);
  yy_destructor(25,&yymsp[-1].minor);
}
// 5202 "parser.c"
        break;
      case 251:
// 2063 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_expr("sub", yymsp[-2].minor.yy235, yymsp[0].minor.yy235, NULL, status->scanner_state);
  yy_destructor(26,&yymsp[-1].minor);
}
// 5210 "parser.c"
        break;
      case 252:
// 2068 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_expr("mul", yymsp[-2].minor.yy235, yymsp[0].minor.yy235, NULL, status->scanner_state);
  yy_destructor(28,&yymsp[-1].minor);
}
// 5218 "parser.c"
        break;
      case 253:
// 2073 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_expr("div", yymsp[-2].minor.yy235, yymsp[0].minor.yy235, NULL, status->scanner_state);
  yy_destructor(29,&yymsp[-1].minor);
}
// 5226 "parser.c"
        break;
      case 254:
// 2078 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_expr("mod", yymsp[-2].minor.yy235, yymsp[0].minor.yy235, NULL, status->scanner_state);
  yy_destructor(30,&yymsp[-1].minor);
}
// 5234 "parser.c"
        break;
      case 255:
// 2083 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_expr("concat", yymsp[-2].minor.yy235, yymsp[0].minor.yy235, NULL, status->scanner_state);
  yy_destructor(27,&yymsp[-1].minor);
}
// 5242 "parser.c"
        break;
      case 256:
// 2088 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_expr("and", yymsp[-2].minor.yy235, yymsp[0].minor.yy235, NULL, status->scanner_state);
  yy_destructor(13,&yymsp[-1].minor);
}
// 5250 "parser.c"
        break;
      case 257:
// 2093 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_expr("or", yymsp[-2].minor.yy235, yymsp[0].minor.yy235, NULL, status->scanner_state);
  yy_destructor(14,&yymsp[-1].minor);
}
// 5258 "parser.c"
        break;
      case 258:
// 2098 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_expr("instanceof", yymsp[-2].minor.yy235, yymsp[0].minor.yy235, NULL, status->scanner_state);
  yy_destructor(12,&yymsp[-1].minor);
}
// 5266 "parser.c"
        break;
      case 259:
// 2103 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_expr("fetch", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), yymsp[0].minor.yy235, NULL, status->scanner_state);
  yy_destructor(32,&yymsp[-3].minor);
  yy_destructor(6,&yymsp[-1].minor);
}
// 5275 "parser.c"
        break;
      case 261:
// 2113 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_expr("typeof", yymsp[0].minor.yy235, NULL, NULL, status->scanner_state);
  yy_destructor(33,&yymsp[-1].minor);
}
// 5283 "parser.c"
        break;
      case 263:
      case 307:
      case 309:
// 2123 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_literal(XX_T_INTEGER, yymsp[0].minor.yy0, status->scanner_state);
}
// 5292 "parser.c"
        break;
      case 264:
      case 306:
      case 311:
// 2128 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_literal(XX_T_STRING, yymsp[0].minor.yy0, status->scanner_state);
}
// 5301 "parser.c"
        break;
      case 265:
      case 310:
// 2133 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_literal(XX_T_CHAR, yymsp[0].minor.yy0, status->scanner_state);
}
// 5309 "parser.c"
        break;
      case 266:
      case 312:
// 2138 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_literal(XX_T_DOUBLE, yymsp[0].minor.yy0, status->scanner_state);
}
// 5317 "parser.c"
        break;
      case 267:
      case 313:
// 2143 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_literal(XX_T_NULL, NULL, status->scanner_state);
  yy_destructor(101,&yymsp[0].minor);
}
// 5326 "parser.c"
        break;
      case 268:
      case 315:
// 2148 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_literal(XX_T_TRUE, NULL, status->scanner_state);
  yy_destructor(102,&yymsp[0].minor);
}
// 5335 "parser.c"
        break;
      case 269:
      case 314:
// 2153 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_literal(XX_T_FALSE, NULL, status->scanner_state);
  yy_destructor(103,&yymsp[0].minor);
}
// 5344 "parser.c"
        break;
      case 270:
      case 317:
// 2158 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_literal(XX_T_CONSTANT, yymsp[0].minor.yy0, status->scanner_state);
}
// 5352 "parser.c"
        break;
      case 271:
// 2163 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_expr("empty-array", NULL, NULL, NULL, status->scanner_state);
  yy_destructor(57,&yymsp[-1].minor);
  yy_destructor(58,&yymsp[0].minor);
}
// 5361 "parser.c"
        break;
      case 272:
// 2168 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_expr("array", yymsp[-1].minor.yy235, NULL, NULL, status->scanner_state);
  yy_destructor(57,&yymsp[-2].minor);
  yy_destructor(58,&yymsp[0].minor);
}
// 5370 "parser.c"
        break;
      case 273:
// 2173 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_new_instance(0, yymsp[0].minor.yy0, NULL, status->scanner_state);
  yy_destructor(34,&yymsp[-1].minor);
}
// 5378 "parser.c"
        break;
      case 274:
// 2178 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_new_instance(0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(34,&yymsp[-3].minor);
  yy_destructor(53,&yymsp[-1].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5388 "parser.c"
        break;
      case 275:
// 2183 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_new_instance(0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(34,&yymsp[-4].minor);
  yy_destructor(53,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5398 "parser.c"
        break;
      case 276:
// 2188 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_new_instance(1, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(34,&yymsp[-3].minor);
  yy_destructor(46,&yymsp[-2].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 5408 "parser.c"
        break;
      case 277:
// 2193 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_new_instance(1, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(34,&yymsp[-5].minor);
  yy_destructor(46,&yymsp[-4].minor);
  yy_destructor(47,&yymsp[-2].minor);
  yy_destructor(53,&yymsp[-1].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5420 "parser.c"
        break;
      case 278:
// 2198 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_new_instance(1, yymsp[-4].minor.yy0, yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(34,&yymsp[-6].minor);
  yy_destructor(46,&yymsp[-5].minor);
  yy_destructor(47,&yymsp[-3].minor);
  yy_destructor(53,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5432 "parser.c"
        break;
      case 279:
// 2203 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_fcall(1, yymsp[-3].minor.yy0, yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(53,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5441 "parser.c"
        break;
      case 280:
// 2208 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_fcall(1, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(53,&yymsp[-1].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5450 "parser.c"
        break;
      case 281:
// 2213 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_fcall(2, yymsp[-4].minor.yy0, yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(46,&yymsp[-5].minor);
  yy_destructor(47,&yymsp[-3].minor);
  yy_destructor(53,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5461 "parser.c"
        break;
      case 282:
// 2218 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_fcall(2, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(46,&yymsp[-4].minor);
  yy_destructor(47,&yymsp[-2].minor);
  yy_destructor(53,&yymsp[-1].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5472 "parser.c"
        break;
      case 283:
// 2223 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_scall(0, yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(91,&yymsp[-4].minor);
  yy_destructor(53,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5482 "parser.c"
        break;
      case 284:
// 2228 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_scall(0, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(91,&yymsp[-3].minor);
  yy_destructor(53,&yymsp[-1].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5492 "parser.c"
        break;
      case 285:
// 2233 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_scall(1, yymsp[-6].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(46,&yymsp[-7].minor);
  yy_destructor(47,&yymsp[-5].minor);
  yy_destructor(91,&yymsp[-4].minor);
  yy_destructor(53,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5504 "parser.c"
        break;
      case 286:
// 2238 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_scall(1, yymsp[-5].minor.yy0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(46,&yymsp[-6].minor);
  yy_destructor(47,&yymsp[-4].minor);
  yy_destructor(91,&yymsp[-3].minor);
  yy_destructor(53,&yymsp[-1].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5516 "parser.c"
        break;
      case 287:
// 2243 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_scall(1, yymsp[-8].minor.yy0, yymsp[-4].minor.yy0, yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(46,&yymsp[-9].minor);
  yy_destructor(47,&yymsp[-7].minor);
  yy_destructor(91,&yymsp[-6].minor);
  yy_destructor(46,&yymsp[-5].minor);
  yy_destructor(47,&yymsp[-3].minor);
  yy_destructor(53,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5530 "parser.c"
        break;
      case 288:
// 2248 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_scall(1, yymsp[-7].minor.yy0, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(46,&yymsp[-8].minor);
  yy_destructor(47,&yymsp[-6].minor);
  yy_destructor(91,&yymsp[-5].minor);
  yy_destructor(46,&yymsp[-4].minor);
  yy_destructor(47,&yymsp[-2].minor);
  yy_destructor(53,&yymsp[-1].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5544 "parser.c"
        break;
      case 289:
// 2253 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_mcall(1, yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(54,&yymsp[-4].minor);
  yy_destructor(53,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5554 "parser.c"
        break;
      case 290:
// 2258 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_mcall(1, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(54,&yymsp[-3].minor);
  yy_destructor(53,&yymsp[-1].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5564 "parser.c"
        break;
      case 291:
// 2263 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_mcall(2, yymsp[-7].minor.yy0, yymsp[-4].minor.yy0, yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(54,&yymsp[-6].minor);
  yy_destructor(46,&yymsp[-5].minor);
  yy_destructor(47,&yymsp[-3].minor);
  yy_destructor(53,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5576 "parser.c"
        break;
      case 292:
// 2268 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_mcall(2, yymsp[-6].minor.yy0, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(54,&yymsp[-5].minor);
  yy_destructor(46,&yymsp[-4].minor);
  yy_destructor(47,&yymsp[-2].minor);
  yy_destructor(53,&yymsp[-1].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5588 "parser.c"
        break;
      case 293:
// 2273 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_mcall(3, yymsp[-7].minor.yy0, yymsp[-4].minor.yy0, yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(54,&yymsp[-6].minor);
  yy_destructor(46,&yymsp[-5].minor);
  yy_destructor(47,&yymsp[-3].minor);
  yy_destructor(53,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5600 "parser.c"
        break;
      case 294:
// 2278 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_mcall(3, yymsp[-6].minor.yy0, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(54,&yymsp[-5].minor);
  yy_destructor(46,&yymsp[-4].minor);
  yy_destructor(47,&yymsp[-2].minor);
  yy_destructor(53,&yymsp[-1].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5612 "parser.c"
        break;
      case 303:
// 2317 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_array_item(yymsp[-2].minor.yy235, yymsp[0].minor.yy235, status->scanner_state);
  yy_destructor(76,&yymsp[-1].minor);
}
// 5620 "parser.c"
        break;
      case 304:
// 2321 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_array_item(NULL, yymsp[0].minor.yy235, status->scanner_state);
}
// 5627 "parser.c"
        break;
      case 323:
// 2401 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_comment(yymsp[0].minor.yy0, status->scanner_state);
}
// 5634 "parser.c"
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
// 965 "parser.lemon"


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

// 5710 "parser.c"
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
