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
 /*     0 */   215,  224,  227,  218,  221,  235,  237,  245,  239,  241,
 /*    10 */   243,  694,  259, 1022,    1,    2,  693,    4,    5,    6,
 /*    20 */     7,  184,  508,  494,  211,  268,  273,  289,  208,  132,
 /*    30 */   395,  263,  134,  662,  668,  667,  651,  137,  408,  296,
 /*    40 */   690,  661,  617,  136,  282,  130,  253,  119,  119,  188,
 /*    50 */   283,  557,   44,   45,   46,   47,   48,   49,   50,   51,
 /*    60 */    52,   53,   54,  664,  629,  651,  396,  397,  398,  399,
 /*    70 */   400,  617,  268,  417,  418,  113,  119,  265,  522,  167,
 /*    80 */   645,   79,   87,  276,   93,  354,  296,  200,  532,  640,
 /*    90 */   646,  275,  277,  278,  279,  280,  281,  502,  494,  486,
 /*   100 */   495,  504,  507,  498,  501,  510,  516,  513,  295,  519,
 /*   110 */   168,  170,  172,  174,  181,  385,  268,  669,  192,  196,
 /*   120 */   201,  382,  522,  167,  389,  303,  305,  304,  272,  403,
 /*   130 */   296,  561,  532,  415,  455,  470,  474,  483,  365,  303,
 /*   140 */   305,  304,  272,  486,  495,  504,  507,  498,  501,  510,
 /*   150 */   516,  513,    9,  519,  168,  170,  172,  130,  181,  186,
 /*   160 */   653,  416,  192,  196,  201,  382,  412,  639,  389,  268,
 /*   170 */   654,  303,  305,  304,  272,  522,  167,  489,  455,  470,
 /*   180 */   474,  483,  460,  296,  180,  532,  128,  188,  672,  557,
 /*   190 */   456,  461,  303,  305,  304,  272,  486,  495,  504,  507,
 /*   200 */   498,  501,  510,  516,  513,  295,  519,  168,  170,  172,
 /*   210 */   509,  181,  547,  268,   10,  192,  196,  201,  382,  522,
 /*   220 */   167,  389,  303,  305,  304,  272,  403,  296,  595,  532,
 /*   230 */   411,  455,  470,  474,  483,  294,  303,  305,  304,  272,
 /*   240 */   486,  495,  504,  507,  498,  501,  510,  516,  513,  395,
 /*   250 */   519,  168,  170,  172,  663,  181,  666,  667,  651,  192,
 /*   260 */   196,  201,  382,  616,  617,  389,  268,  401,   56,  119,
 /*   270 */   446,  451,  522,  167,  489,  455,  470,  474,  483,   14,
 /*   280 */   296,  550,  532,   27,   17,  396,  397,  398,  399,  400,
 /*   290 */   239,  241,  243,  486,  495,  504,  507,  498,  501,  510,
 /*   300 */   516,  513,  295,  519,  168,  170,  172,  512,  181,  219,
 /*   310 */   268,   12,  192,  196,  201,  382,  522,  167,  389,  303,
 /*   320 */   305,  304,  272,  403,  296,  585,  532,  407,  455,  470,
 /*   330 */   474,  483,  329,  303,  305,  304,  272,  486,  495,  504,
 /*   340 */   507,  498,  501,  510,  516,  513,  395,  519,  168,  170,
 /*   350 */   172,  299,  181,  374,  450,  416,  192,  196,  201,  382,
 /*   360 */   444,  375,  389,  268,  656,  303,  305,  304,  272,  522,
 /*   370 */   167,  639,  455,  470,  474,  483,   13,  296,  599,  532,
 /*   380 */   185,  560,  396,  397,  398,  399,  400,  511,  494,  307,
 /*   390 */   486,  495,  504,  507,  498,  501,  510,  516,  513,  295,
 /*   400 */   519,  168,  170,  172,  447,  181,  262,  268,  449,  192,
 /*   410 */   196,  201,  382,  522,  167,  389,  303,  305,  304,  272,
 /*   420 */   403,  296,  583,  532,  431,  455,  470,  474,  483,  327,
 /*   430 */   303,  305,  304,  272,  486,  495,  504,  507,  498,  501,
 /*   440 */   510,  516,  513,  395,  519,  168,  170,  172,  645,  181,
 /*   450 */   539,  432,  460,  192,  196,  201,  382,  631,  646,  389,
 /*   460 */   268,  459,  303,  305,  304,  272,  522,  167,  119,  455,
 /*   470 */   470,  474,  483,  295,  296,  566,  532,  335,  525,  396,
 /*   480 */   397,  398,  399,  400,  514,  494,   62,  486,  495,  504,
 /*   490 */   507,  498,  501,  510,  516,  513,   15,  519,  168,  170,
 /*   500 */   172,   59,  181,  318,  268,   17,  192,  196,  201,  382,
 /*   510 */   522,  167,  389,  308,  206,  523,  489,  381,  296,  601,
 /*   520 */   532,  313,  455,  470,  474,  483,  384,  303,  305,  304,
 /*   530 */   272,  486,  495,  504,  507,  498,  501,  510,  516,  513,
 /*   540 */    16,  519,  168,  170,  172,  684,  181,  672,  284,  500,
 /*   550 */   192,  196,  201,  382,  499,  494,  389,  268,  303,  305,
 /*   560 */   304,  272,  197,  522,  167,  381,  455,  470,  474,  483,
 /*   570 */   348,  296,  605,  532,   94,  303,  305,  304,  272,    3,
 /*   580 */     4,    5,    6,    7,  486,  495,  504,  507,  498,  501,
 /*   590 */   510,  516,  513,  295,  519,  168,  170,  172,   24,  181,
 /*   600 */   376,  268,  405,  192,  196,  201,  382,  522,  167,  389,
 /*   610 */   419,  553,   63,  489,  381,  296,  579,  532,  256,  455,
 /*   620 */   470,  474,  483,  311,  303,  305,  304,  272,  486,  495,
 /*   630 */   504,  507,  498,  501,  510,  516,  513,  636,  519,  168,
 /*   640 */   170,  172,  603,  181,  639,  254,  515,  192,  196,  201,
 /*   650 */   382,   29,  378,  389,  268,  303,  305,  304,  272,  182,
 /*   660 */   522,  167,  381,  455,  470,  474,  483,  403,  296,  545,
 /*   670 */   532,  402,  303,  305,  304,  272,   25,  303,  305,  304,
 /*   680 */   272,  486,  495,  504,  507,  498,  501,  510,  516,  513,
 /*   690 */   295,  519,  168,  170,  172,   32,  181,  371,  268,   35,
 /*   700 */   192,  196,  201,  382,  522,  167,  389,  303,  305,  304,
 /*   710 */   272,  403,  296,  577,  532,  448,  455,  470,  474,  483,
 /*   720 */   301,  303,  305,  304,  272,  486,  495,  504,  507,  498,
 /*   730 */   501,  510,  516,  513,   63,  519,  168,  170,  172,  173,
 /*   740 */   181,  674,  381,  672,  192,  196,  201,  382,   42,  633,
 /*   750 */   389,  268,  303,  305,  304,  272,  639,  522,  167,  634,
 /*   760 */   455,  470,  474,  483,  575,  296,  571,  532,  110,  290,
 /*   770 */    31,   55,   34,   36,   37,  517,  494,  321,  486,  495,
 /*   780 */   504,  507,  498,  501,  510,  516,  513,   63,  519,  168,
 /*   790 */   170,  172,   39,  181,  244,  268,   43,  192,  196,  201,
 /*   800 */   382,  522,  167,  389,  303,  305,  304,  272,  403,  296,
 /*   810 */   607,  532,  435,  455,  470,  474,  483,   58,  303,  305,
 /*   820 */   304,  272,  486,  495,  504,  507,  498,  501,  510,  516,
 /*   830 */   513,   63,  519,  168,  170,  172,  295,  181,   61,  489,
 /*   840 */   416,  192,  196,  201,  382,  436,  373,  389,  268,  214,
 /*   850 */   303,  305,  304,  272,  522,  167,  489,  455,  470,  474,
 /*   860 */   483,  114,  296,  573,  532,  597,  337,   31,   55,   34,
 /*   870 */    36,   37,  497,  496,  494,  486,  495,  504,  507,  498,
 /*   880 */   501,  510,  516,  513,  295,  519,  168,  170,  172,  518,
 /*   890 */   181,  246,  268,   66,  192,  196,  201,  382,  522,  167,
 /*   900 */   389,  303,  305,  304,  272,  403,  296,  611,  532,  427,
 /*   910 */   455,  470,  474,  483,  352,  303,  305,  304,  272,  486,
 /*   920 */   495,  504,  507,  498,  501,  510,  516,  513,   67,  519,
 /*   930 */   168,  170,  172,   76,  181,  112,  258,  489,  192,  196,
 /*   940 */   201,  382,  390,  454,  389,  268,  303,  305,  304,  272,
 /*   950 */   109,  522,  167,  392,  455,  470,  474,  483,  104,  296,
 /*   960 */   204,  532,  581,  142,   31,   55,   34,   36,   37,  119,
 /*   970 */   488,  568,  486,  495,  504,  507,  498,  501,  510,  516,
 /*   980 */   513,  295,  519,  168,  170,  172,  391,  181,  370,  268,
 /*   990 */    77,  192,  196,  201,  382,  522,  167,  389,  303,  305,
 /*  1000 */   304,  272,  403,  296,  613,  532,  439,  455,  470,  474,
 /*  1010 */   483,  359,  303,  305,  304,  272,  486,  495,  504,  507,
 /*  1020 */   498,  501,  510,  516,  513,   81,  519,  168,  170,  172,
 /*  1030 */    82,  181,   84,  471,   85,  192,  196,  201,  382,  487,
 /*  1040 */   494,  389,  268,  303,  305,  304,  272,  576,  522,  167,
 /*  1050 */    40,  455,  470,  474,  483,  580,  296,  195,  532,   28,
 /*  1060 */   526,   31,   55,   34,   36,   37,  641,  648,  527,  486,
 /*  1070 */   495,  504,  507,  498,  501,  510,  516,  513,   86,  519,
 /*  1080 */   168,  170,  172,   88,  181,  209,  268,   41,  192,  196,
 /*  1090 */   201,  382,  522,  167,  389,  303,  305,  304,  272,  403,
 /*  1100 */   296,  538,  532,  423,  455,  470,  474,  483,   89,  303,
 /*  1110 */   305,  304,  272,  486,  495,  504,  507,  498,  501,  510,
 /*  1120 */   516,  513,   90,  519,  168,  170,  172,   92,  181,   40,
 /*  1130 */   369,  489,  192,  196,  201,  382,  520,  494,  389,  268,
 /*  1140 */   303,  305,  304,  272,  127,  522,  167,  130,  455,  470,
 /*  1150 */   474,  483,  587,  296,  551,  532,  569,  592,   31,   55,
 /*  1160 */    34,   36,   37,  119,  521,  596,  486,  495,  504,  507,
 /*  1170 */   498,  501,  510,  516,  513,  106,  519,  168,  170,  172,
 /*  1180 */   107,  181,  535,  268,  111,  192,  196,  201,  382,  522,
 /*  1190 */   167,  389,  303,  305,  304,  272,  403,  296,  143,  532,
 /*  1200 */   443,  455,  470,  474,  483,  117,  303,  305,  304,  272,
 /*  1210 */   486,  495,  504,  507,  498,  501,  510,  516,  513,  643,
 /*  1220 */   519,  168,  170,  172,  122,  181,  123,  380,  458,  192,
 /*  1230 */   196,  201,  382,  126,  118,  389,  268,  303,  305,  304,
 /*  1240 */   272,  287,  522,  167,  489,  455,  470,  474,  483,  115,
 /*  1250 */   296,  556,  532,  609,  331,   31,   55,   34,   36,   37,
 /*  1260 */   642,  457,  125,  486,  495,  504,  507,  498,  501,  510,
 /*  1270 */   516,  513,  671,  519,  168,  170,  172,  506,  181,  480,
 /*  1280 */   268,  129,  192,  196,  201,  382,  522,  167,  389,  303,
 /*  1290 */   305,  304,  272,  368,  296,  145,  532,  133,  455,  470,
 /*  1300 */   474,  483,  139,  303,  305,  304,  272,  486,  495,  504,
 /*  1310 */   507,  498,  501,  510,  516,  513,  140,  519,  168,  170,
 /*  1320 */   172,  169,  181,  178,  252,  489,  192,  196,  201,  382,
 /*  1330 */   505,  494,  389,  268,  303,  305,  304,  272,  476,  522,
 /*  1340 */   167,  479,  455,  470,  474,  483,  171,  296,  593,  532,
 /*  1350 */    60,  604,   31,   55,   34,   36,   37,  183,  503,  608,
 /*  1360 */   486,  495,  504,  507,  498,  501,  510,  516,  513,  177,
 /*  1370 */   519,  168,  170,  172,  187,  181,  225,  268,  189,  192,
 /*  1380 */   196,  201,  382,  522,  167,  389,  303,  305,  304,  272,
 /*  1390 */   367,  296,  388,  532,  190,  455,  470,  474,  483,  193,
 /*  1400 */   303,  305,  304,  272,  486,  495,  504,  507,  498,  501,
 /*  1410 */   510,  516,  513,  198,  519,  168,  170,  172,  202,  181,
 /*  1420 */   205,  228,  207,  192,  196,  201,  382,  213,  257,  389,
 /*  1430 */   268,  303,  305,  304,  272,  212,  522,  167,  260,  455,
 /*  1440 */   470,  474,  483,  261,  296,  567,  532,  116,  266,   31,
 /*  1450 */    55,   34,   36,   37,  269,  270,  271,  486,  495,  504,
 /*  1460 */   507,  498,  501,  510,  516,  513,  288,  519,  168,  170,
 /*  1470 */   172,  291,  181,  230,  268,  297,  192,  196,  201,  382,
 /*  1480 */   522,  167,  389,  303,  305,  304,  272,  484,  296,  555,
 /*  1490 */   532,  298,  455,  470,  474,  483,  309,  303,  305,  304,
 /*  1500 */   272,  486,  495,  504,  507,  498,  501,  510,  516,  513,
 /*  1510 */   314,  519,  168,  170,  172,  315,  181,  316,  274,  322,
 /*  1520 */   192,  196,  201,  382,  323,  356,  389,  268,  303,  305,
 /*  1530 */   304,  272,  324,  522,  167,  232,  455,  470,  474,  483,
 /*  1540 */   250,  296,  176,  532,  357,  303,  305,  304,  272, 1004,
 /*  1550 */   303,  305,  304,  272,  486,  495,  504,  507,  498,  501,
 /*  1560 */   510,  516,  513, 1003,  519,  168,  170,  172,  347,  181,
 /*  1570 */   222,  268,  362,  192,  196,  201,  382,  522,  167,  389,
 /*  1580 */   303,  305,  304,  272,  234,  296,  562,  532,  363,  455,
 /*  1590 */   470,  474,  483,  420,  303,  305,  304,  272,  486,  495,
 /*  1600 */   504,  507,  498,  501,  510,  516,  513,  377,  519,  168,
 /*  1610 */   170,  172,  379,  181,  383,  528,  394,  192,  196,  201,
 /*  1620 */   382,  393,  413,  389,  268,   95,   96,   98,   97,   99,
 /*  1630 */   522,  167,  406,  455,  470,  474,  483,  410,  296,    8,
 /*  1640 */   532,  414,   11,  421,  121,  424,  676,  686,  355,  422,
 /*  1650 */   532,  486,  495,  504,  507,  498,  501,  510,  516,  513,
 /*  1660 */   425,  519,  168,  170,  172,  426,  181,  530,  101,  102,
 /*  1670 */   192,  196,  201,  382,  165,   83,  389,  429,   80,  100,
 /*  1680 */    33,   34,   36,   37,   38,  236,  455,  470,  474,  483,
 /*  1690 */    79,   87,  437,   64,  584,  303,  305,  304,  272,  430,
 /*  1700 */   361,  434,  445,  533,  147,  148,  149,  150,  151,  152,
 /*  1710 */   153,  154,  155,  156,  157,  158,  159,  160,  161,  162,
 /*  1720 */   163,  164,  166,  165,   44,   45,   46,   47,   48,   49,
 /*  1730 */    50,   51,   52,   53,   54,  542,  462,  464,  466,  468,
 /*  1740 */   335,  264,  438,  600,  332,  303,  305,  304,  272,  442,
 /*  1750 */   463,  465,  533,  147,  148,  149,  150,  151,  152,  153,
 /*  1760 */   154,  155,  156,  157,  158,  159,  160,  161,  162,  163,
 /*  1770 */   164,  166,  165,  467,  574,  492,  469,  680,  286,  683,
 /*  1780 */    79,   87,  130,   93,   57,  462,  464,  466,  468,  395,
 /*  1790 */    79,   87,  144,   93,  404,  475,  344,  440,  477,  490,
 /*  1800 */   478,  533,  147,  148,  149,  150,  151,  152,  153,  154,
 /*  1810 */   155,  156,  157,  158,  159,  160,  161,  162,  163,  164,
 /*  1820 */   166,  165,  482,  491,  493,  396,  397,  398,  399,  400,
 /*  1830 */   524,  428,  452,  453,  462,  464,  466,  468,  602,  540,
 /*  1840 */   529,  194,  531,  554,   79,   87,  541,   93,  343,  552,
 /*  1850 */   533,  147,  148,  149,  150,  151,  152,  153,  154,  155,
 /*  1860 */   156,  157,  158,  159,  160,  161,  162,  163,  164,  166,
 /*  1870 */   165,  563,  558,  614,  135,  629,  651,  564,  570,  582,
 /*  1880 */   589,  661,  617,  462,  464,  466,  468,  119,  590,  124,
 /*  1890 */   594,  673,  620,  598,  130,  610,  622,  623,  627,  533,
 /*  1900 */   147,  148,  149,  150,  151,  152,  153,  154,  155,  156,
 /*  1910 */   157,  158,  159,  160,  161,  162,  163,  164,  166,  165,
 /*  1920 */   621,  625,  626,  628,  649,  637,  635,  638,  740,  741,
 /*  1930 */   238,  647,  462,  464,  466,  468,  644,  650,  659,  175,
 /*  1940 */   303,  305,  304,  272,  655,  658,  657,  660,  533,  147,
 /*  1950 */   148,  149,  150,  151,  152,  153,  154,  155,  156,  157,
 /*  1960 */   158,  159,  160,  161,  162,  163,  164,  166,  165,   22,
 /*  1970 */   670,  675,  677,  678,  679,  681,   21,  130,  687,  216,
 /*  1980 */   119,  462,  464,  466,  468,  688,  682,  691,  549,  303,
 /*  1990 */   305,  304,  272,  685,  689,  692,  516,  533,  147,  148,
 /*  2000 */   149,  150,  151,  152,  153,  154,  155,  156,  157,  158,
 /*  2010 */   159,  160,  161,  162,  163,  164,  166,  165,  516,  516,
 /*  2020 */   516,  516,  516,  516,  516,  516,  516,  516,  333,  516,
 /*  2030 */   462,  464,  466,  468,  516,  516,  516,  559,  303,  305,
 /*  2040 */   304,  272,  516,  516,  516,  516,  533,  147,  148,  149,
 /*  2050 */   150,  151,  152,  153,  154,  155,  156,  157,  158,  159,
 /*  2060 */   160,  161,  162,  163,  164,  166,  165,  516,  516,  516,
 /*  2070 */   516,  516,  516,  516,  516,  516,  516,  240,  516,  462,
 /*  2080 */   464,  466,  468,  516,  516,  516,  578,  303,  305,  304,
 /*  2090 */   272,  516,  516,  516,  516,  533,  147,  148,  149,  150,
 /*  2100 */   151,  152,  153,  154,  155,  156,  157,  158,  159,  160,
 /*  2110 */   161,  162,  163,  164,  166,  165,  516,  516,  516,  516,
 /*  2120 */   516,  516,  516,  516,  516,  516,  248,  516,  462,  464,
 /*  2130 */   466,  468,  516,  516,  516,  199,  303,  305,  304,  272,
 /*  2140 */   516,  516,  516,  516,  533,  147,  148,  149,  150,  151,
 /*  2150 */   152,  153,  154,  155,  156,  157,  158,  159,  160,  161,
 /*  2160 */   162,  163,  164,  166,  165,  516,  516,  516,  516,  516,
 /*  2170 */   516,  516,  516,  516,  516,  242,  516,  462,  464,  466,
 /*  2180 */   468,  516,  516,  516,  606,  303,  305,  304,  272,  516,
 /*  2190 */   516,  516,  516,  533,  147,  148,  149,  150,  151,  152,
 /*  2200 */   153,  154,  155,  156,  157,  158,  159,  160,  161,  162,
 /*  2210 */   163,  164,  166,  165,  516,  516,  516,  615,  516,  137,
 /*  2220 */   516,  516,  516,  516,  616,  617,  462,  464,  466,  468,
 /*  2230 */   119,  516,  516,  572,  516,  516,  516,  516,  516,  516,
 /*  2240 */   516,  516,  533,  147,  148,  149,  150,  151,  152,  153,
 /*  2250 */   154,  155,  156,  157,  158,  159,  160,  161,  162,  163,
 /*  2260 */   164,  166,  165,  516,  516,  516,  516,   19,  516,  516,
 /*  2270 */   516,  516,   22,  516,  516,  462,  464,  466,  468,  120,
 /*  2280 */   516,  516,  544,  119,  516,  516,  516,  516,  516,  516,
 /*  2290 */   516,  533,  147,  148,  149,  150,  151,  152,  153,  154,
 /*  2300 */   155,  156,  157,  158,  159,  160,  161,  162,  163,  164,
 /*  2310 */   166,  165,  516,  516,  516,  516,  516,  516,  516,  516,
 /*  2320 */   516,  516,  516,  516,  462,  464,  466,  468,  516,  516,
 /*  2330 */   516,  565,  516,  516,  516,  516,  516,  516,  516,  516,
 /*  2340 */   533,  147,  148,  149,  150,  151,  152,  153,  154,  155,
 /*  2350 */   156,  157,  158,  159,  160,  161,  162,  163,  164,  166,
 /*  2360 */   165,  516,  516,  516,  516,  516,  516,  516,  516,  516,
 /*  2370 */   516,  516,  516,  462,  464,  466,  468,  516,  516,  516,
 /*  2380 */   387,  516,  516,  516,  516,  516,  516,  516,  516,  533,
 /*  2390 */   147,  148,  149,  150,  151,  152,  153,  154,  155,  156,
 /*  2400 */   157,  158,  159,  160,  161,  162,  163,  164,  166,  165,
 /*  2410 */   516,  516,  516,  516,  516,  516,  516,  516,  516,  516,
 /*  2420 */   516,  516,  462,  464,  466,  468,  516,  516,  516,  191,
 /*  2430 */   516,  516,  516,  516,  516,  516,  516,  516,  533,  147,
 /*  2440 */   148,  149,  150,  151,  152,  153,  154,  155,  156,  157,
 /*  2450 */   158,  159,  160,  161,  162,  163,  164,  166,  165,  516,
 /*  2460 */   516,  516,  516,  516,  516,  516,  516,  516,  516,  516,
 /*  2470 */   516,  462,  464,  466,  468,  516,  516,  516,  179,  516,
 /*  2480 */   516,  516,  516,  516,  516,  516,  516,  533,  147,  148,
 /*  2490 */   149,  150,  151,  152,  153,  154,  155,  156,  157,  158,
 /*  2500 */   159,  160,  161,  162,  163,  164,  166,  165,  516,  516,
 /*  2510 */   516,  516,  516,  516,  516,  516,  516,  516,  516,  516,
 /*  2520 */   462,  464,  466,  468,  516,  516,  516,  612,  516,  516,
 /*  2530 */   516,  516,  516,  516,  516,  516,  533,  147,  148,  149,
 /*  2540 */   150,  151,  152,  153,  154,  155,  156,  157,  158,  159,
 /*  2550 */   160,  161,  162,  163,  164,  166,  165,  516,  516,  516,
 /*  2560 */   516,  516,  516,  516,  516,  516,  516,  516,  516,  462,
 /*  2570 */   464,  466,  468,  516,  516,  516,  203,  516,  516,  516,
 /*  2580 */   516,  516,  516,  516,  516,  533,  147,  148,  149,  150,
 /*  2590 */   151,  152,  153,  154,  155,  156,  157,  158,  159,  160,
 /*  2600 */   161,  162,  163,  164,  166,  165,  516,  516,  516,  516,
 /*  2610 */   516,   75,  516,  516,  516,  516,  516,  516,  462,  464,
 /*  2620 */   466,  468,  516,  516,   78,  537,  516,  516,  516,  516,
 /*  2630 */   516,  516,  516,  516,  533,  147,  148,  149,  150,  151,
 /*  2640 */   152,  153,  154,  155,  156,  157,  158,  159,  160,  161,
 /*  2650 */   162,  163,  164,  166,  165,  516,  516,  516,  516,  516,
 /*  2660 */   516,  516,  516,   70,  516,  516,  516,  462,  464,  466,
 /*  2670 */   468,   68,   69,   71,   72,   74,   73,  516,  516,  516,
 /*  2680 */   516,  516,  516,  146,  147,  148,  149,  150,  151,  152,
 /*  2690 */   153,  154,  155,  156,  157,  158,  159,  160,  161,  162,
 /*  2700 */   163,  164,  166,  215,  224,  227,  218,  221,  516,  516,
 /*  2710 */   516,  516,  516,  516,  516,  259,  462,  464,  466,  468,
 /*  2720 */   516,  335,  264,  516,  516,  332,  516,  211,  268,  273,
 /*  2730 */   289,  208,  366,  516,  263,   91,  516,  516,  516,  516,
 /*  2740 */   516,  516,  296,  516, 1002,  516,  516,  282,  516,  253,
 /*  2750 */   516,  516,  591,  283,   65,  251,  247,  249,  516,  286,
 /*  2760 */   210,  220,  226,  229,  231,  233,  223,  217,  235,  237,
 /*  2770 */   245,  239,  241,  243,  516,   44,   45,   46,   47,   48,
 /*  2780 */    49,   50,   51,   52,   53,   54,  276,  516,  516,  516,
 /*  2790 */   516,  516,  516,  516,  275,  277,  278,  279,  280,  281,
 /*  2800 */   284,  372,  215,  224,  227,  218,  221,  516,  516,  516,
 /*  2810 */   303,  305,  304,  272,  259,  516,  516,  342,  516,  516,
 /*  2820 */   350,  346,  349,  516,  516,  516,  211,  268,  273,  289,
 /*  2830 */   208,  360,  516,  263,   91,  516,  516,  516,  516,  516,
 /*  2840 */   516,  296,  516,  516,  516,  516,  282,  516,  253,  516,
 /*  2850 */   516,  141,  283,   65,  251,  247,  249,  516,  516,  210,
 /*  2860 */   220,  226,  229,  231,  233,  223,  217,  235,  237,  245,
 /*  2870 */   239,  241,  243,  516,   44,   45,   46,   47,   48,   49,
 /*  2880 */    50,   51,   52,   53,   54,  276,  516,  516,  543,  516,
 /*  2890 */   516,  516,  516,  275,  277,  278,  279,  280,  281,  284,
 /*  2900 */   516,  215,  224,  227,  218,  221,  516,  516,  516,  303,
 /*  2910 */   305,  304,  272,  259,  516,  516,  516,  516,  516,  345,
 /*  2920 */   346,  349,  516,  516,  516,  211,  268,  273,  289,  208,
 /*  2930 */   516,  267,  263,  516,   95,   96,   98,   97,   99,  516,
 /*  2940 */   296,  303,  305,  304,  272,  282,  516,  253,  516,  293,
 /*  2950 */   306,  283,  441,  251,  247,  249,  516,  516,  210,  220,
 /*  2960 */   226,  229,  231,  233,  223,  217,  235,  237,  245,  239,
 /*  2970 */   241,  243,  516,   38,  516,  516,  516,  101,  102,  516,
 /*  2980 */   721,  630,  516,  624,  276,  516,  516,  536,  100,  516,
 /*  2990 */   516,  516,  275,  277,  278,  279,  280,  281,  516,  516,
 /*  3000 */   215,  224,  227,  218,  221,  516,  516,  516,  516,  516,
 /*  3010 */    30,  516,  259,   44,   45,   46,   47,   48,   49,   50,
 /*  3020 */    51,   52,   53,   54,  211,  268,  273,  289,  208,  516,
 /*  3030 */   267,  263,  473,  516,  516,  516,  516,  516,  516,  296,
 /*  3040 */   303,  305,  304,  272,  282,  516,  253,  516,  328,  306,
 /*  3050 */   283,  516,  251,  247,  249,  516,  516,  210,  220,  226,
 /*  3060 */   229,  231,  233,  223,  217,  235,  237,  245,  239,  241,
 /*  3070 */   243,  516,  516,  516,  516,  516,  516,  516,  516,  472,
 /*  3080 */   516,  516,  516,  276,  516,   95,   96,   98,   97,   99,
 /*  3090 */   516,  275,  277,  278,  279,  280,  281,  516,  516,  215,
 /*  3100 */   224,  227,  218,  221,  516,  516,  516,  516,  516,  516,
 /*  3110 */   516,  259,  516,  516,  516,  516,  516,  516,  516,  516,
 /*  3120 */   516,  516,  516,  211,  268,  273,  289,  208,  101,  102,
 /*  3130 */   263,  722,  630,  695,  624,  516,  516,  516,  296,  100,
 /*  3140 */   516,  516,  516,  282,  516,  253,  516,  516,  516,  283,
 /*  3150 */   433,  251,  247,  249,  516,  516,  210,  220,  226,  229,
 /*  3160 */   231,  233,  223,  217,  235,  237,  245,  239,  241,  243,
 /*  3170 */     8,  516,  516,   11,  516,  121,  516,  676,  686,  516,
 /*  3180 */   516,  532,  276,  516,  516,  548,  516,  516,  516,  516,
 /*  3190 */   275,  277,  278,  279,  280,  281,  267,  516,  215,  224,
 /*  3200 */   227,  218,  221,  516,  516,  516,  303,  305,  304,  272,
 /*  3210 */   259,  516,  516,  516,  364,  306,  516,  516,  516,  516,
 /*  3220 */   516,  516,  211,  268,  273,  289,  208,  330,  267,  263,
 /*  3230 */   516,   95,   96,   98,   97,   99,  516,  296,  303,  305,
 /*  3240 */   304,  272,  282,  516,  253,  516,  358,  306,  283,  516,
 /*  3250 */   251,  247,  249,  516,  516,  210,  220,  226,  229,  231,
 /*  3260 */   233,  223,  217,  235,  237,  245,  239,  241,  243,  516,
 /*  3270 */   516,  516,  516,  516,  101,  102,  516,  485,  516,  516,
 /*  3280 */   619,  276,  516,  516,  516,  100,  516,  516,  516,  275,
 /*  3290 */   277,  278,  279,  280,  281,  267,  516,  215,  224,  227,
 /*  3300 */   218,  221,  516,  516,  516,  303,  305,  304,  272,  259,
 /*  3310 */   516,  516,  516,  336,  306,  516,  516,  516,  516,  516,
 /*  3320 */   516,  211,  268,  273,  289,  208,  312,  516,  263,   91,
 /*  3330 */   516,  516,  516,  516,  516,  516,  296,  516,  516,  516,
 /*  3340 */   516,  282,  516,  253,  516,  516,  108,  283,   65,  251,
 /*  3350 */   247,  249,  516,  516,  210,  220,  226,  229,  231,  233,
 /*  3360 */   223,  217,  235,  237,  245,  239,  241,  243,  516,   44,
 /*  3370 */    45,   46,   47,   48,   49,   50,   51,   52,   53,   54,
 /*  3380 */   276,  516,   95,   96,   98,   97,   99,  516,  275,  277,
 /*  3390 */   278,  279,  280,  281,  516,  334,  215,  224,  227,  218,
 /*  3400 */   221,  516,  516,  516,  516,  516,  516,  516,  259,  516,
 /*  3410 */   516,  516,  516,  516,  516,  516,  516,  516,  516,  516,
 /*  3420 */   211,  268,  273,  289,  208,  101,  102,  285,  725,  618,
 /*  3430 */   516,  624,  516,  516,  516,  296,  100,  516,  516,  516,
 /*  3440 */   282,  516,  253,  516,  516,  516,  283,  341,  251,  247,
 /*  3450 */   249,  516,  516,  210,  220,  226,  229,  231,  233,  223,
 /*  3460 */   217,  235,  237,  245,  239,  241,  243,  516,  516,  516,
 /*  3470 */   516,  516,  255,  516,   95,   96,   98,   97,   99,  340,
 /*  3480 */   516,  516,  516,  516,  516,  516,  516,  339,  277,  278,
 /*  3490 */   279,  280,  281,  267,  516,  215,  224,  227,  218,  221,
 /*  3500 */   516,  516,  516,  303,  305,  304,  272,  259,  516,  516,
 /*  3510 */   516,  632,  320,  516,  516,  516,  516,  101,  102,  211,
 /*  3520 */   268,  273,  289,  208,  325,  588,  263,   91,  100,  516,
 /*  3530 */   516,  516,  516,  516,  296,  516,  516,  516,  516,  282,
 /*  3540 */   516,  253,  516,  516,   26,  283,   65,  251,  247,  249,
 /*  3550 */   516,  516,  210,  220,  226,  229,  231,  233,  223,  217,
 /*  3560 */   235,  237,  245,  239,  241,  243,  516,   44,   45,   46,
 /*  3570 */    47,   48,   49,   50,   51,   52,   53,   54,  276,  516,
 /*  3580 */    95,   96,   98,   97,   99,  516,  275,  277,  278,  279,
 /*  3590 */   280,  281,  267,  481,  215,  224,  227,  218,  221,  516,
 /*  3600 */   516,  516,  303,  305,  304,  272,  259,  516,  516,  516,
 /*  3610 */   326,  306,  516,  516,  516,  516,  516,  516,  211,  268,
 /*  3620 */   273,  289,  208,  101,  102,  263,  131,  630,  516,  624,
 /*  3630 */   516,  516,  516,  296,  100,  516,  516,  516,  282,  516,
 /*  3640 */   253,  516,  516,  516,  283,  409,  251,  247,  249,  516,
 /*  3650 */   516,  210,  220,  226,  229,  231,  233,  223,  217,  235,
 /*  3660 */   237,  245,  239,  241,  243,  516,  516,  516,  516,  516,
 /*  3670 */   516,  516,  516,  516,  516,  516,  516,  276,  516,  516,
 /*  3680 */   516,  516,  516,  516,  516,  275,  277,  278,  279,  280,
 /*  3690 */   281,  267,  894,  215,  224,  227,  218,  221,  516,  516,
 /*  3700 */   516,  303,  305,  304,  272,  259,  516,  516,  516,  317,
 /*  3710 */   306,  516,  516,  516,  516,  516,  516,  211,  268,  273,
 /*  3720 */   289,  208,  319,  267,  263,   91,  516,  516,  516,  516,
 /*  3730 */   516,  516,  296,  303,  305,  304,  272,  282,  516,  253,
 /*  3740 */   516,  351,  306,  283,   65,  251,  247,  249,  516,  516,
 /*  3750 */   210,  220,  226,  229,  231,  233,  223,  217,  235,  237,
 /*  3760 */   245,  239,  241,  243,  516,   44,   45,   46,   47,   48,
 /*  3770 */    49,   50,   51,   52,   53,   54,  276,  516,  516,  386,
 /*  3780 */   516,  516,  516,  516,  275,  277,  278,  279,  280,  281,
 /*  3790 */   267,  516,  215,  224,  227,  218,  221,  516,  516,  516,
 /*  3800 */   303,  305,  304,  272,  259,  516,  516,  516,  310,  306,
 /*  3810 */   516,  516,  516,  516,  516,  516,  211,  268,  273,  289,
 /*  3820 */   208,  292,  267,  263,  516,   95,   96,   98,   97,   99,
 /*  3830 */   516,  296,  303,  305,  304,  272,  282,  516,  253,  516,
 /*  3840 */   300,  306,  283,  516,  251,  247,  249,  516,  516,  210,
 /*  3850 */   220,  226,  229,  231,  233,  223,  217,  235,  237,  245,
 /*  3860 */   239,  241,  243,  516,  516,  516,  516,  516,  101,  102,
 /*  3870 */   516,  724,  586,  516,  516,  276,  516,  516,  516,  100,
 /*  3880 */   516,  516,  516,  275,  277,  278,  279,  280,  281,  516,
 /*  3890 */   516,  215,  224,  227,  218,  221,  516,  516,  516,  516,
 /*  3900 */   516,  516,  516,  259,  516,  516,  516,  516,  516,  516,
 /*  3910 */   516,  516,  516,  516,  516,  211,  268,  273,  289,  208,
 /*  3920 */   516,  516,  263,  516,  516,  516,  516,  516,  516,  516,
 /*  3930 */   296,  516,  516,  516,  516,  282,  516,  253,  516,  516,
 /*  3940 */   516,  283,  516,  516,  247,  249,  516,  516,  210,  220,
 /*  3950 */   226,  229,  231,  233,  223,  217,  235,  237,  245,  239,
 /*  3960 */   241,  243,  516,  516,  516,  516,  516,  546,  516,  516,
 /*  3970 */   516,  516,  516,  516,  276,  516,  516,  516,  516,  516,
 /*  3980 */   516,  516,  275,  277,  278,  279,  280,  281,  516,  516,
 /*  3990 */   215,  224,  227,  218,  221,  516,  516,  516,  516,  516,
 /*  4000 */   516,  516,  259,  516,  516,  516,  516,  516,  516,  516,
 /*  4010 */   516,  516,  516,  516,  211,  268,  273,  289,  208,  353,
 /*  4020 */   516,  263,  516,   95,   96,   98,   97,   99,  516,  296,
 /*  4030 */   516,  516,  516,  516,  282,  516,  253,  516,  516,  516,
 /*  4040 */   283,  516,  516,  516,  516,  516,  516,  210,  220,  226,
 /*  4050 */   229,  231,  233,  223,  217,  235,  237,  245,  239,  241,
 /*  4060 */   243,  516,  516,  516,  516,  516,  101,  102,  516,  726,
 /*  4070 */   665,  516,  516,  276,  516,  516,  516,  100,  516,  516,
 /*  4080 */   516,  275,  277,  278,  279,  280,  281,  516,  516,  215,
 /*  4090 */   224,  227,  218,  221,   95,   96,   98,   97,   99,  516,
 /*  4100 */   516,  259,  516,   95,   96,   98,   97,   99,   95,   96,
 /*  4110 */    98,   97,   99,  211,  268,  273,  289,  208,  516,  516,
 /*  4120 */   263,  516,  516,  516,  516,  516,  516,  516,  296,  516,
 /*  4130 */   516,  516,  516,  282,  516,  253,  516,  101,  102,  283,
 /*  4140 */   727,  586,  516,  516,  516,  516,  101,  102,  100,  729,
 /*  4150 */   586,  101,  102,  516,  723,  586,  516,  100,  516,  516,
 /*  4160 */   516,  516,  100,  516,  516,  534,  516,  516,  516,  516,
 /*  4170 */   516,  516,  276,  516,  516,  516,  516,  516,  516,  516,
 /*  4180 */   275,  277,  278,  279,  280,  281,  516,  516,  215,  224,
 /*  4190 */   227,  218,  221,   95,   96,   98,   97,   99,  516,  516,
 /*  4200 */   259,  516,   95,   96,   98,   97,   99,   95,   96,   98,
 /*  4210 */    97,   99,  211,  268,  273,  289,  208,  338,  516,  263,
 /*  4220 */   516,   95,   96,   98,   97,   99,  516,  296,  516,  516,
 /*  4230 */   516,  516,  282,  516,  253,  516,  101,  102,  283,   18,
 /*  4240 */   103,  516,  516,  516,  652,  101,  102,  100,   20,  103,
 /*  4250 */   101,  102,  516,  516,  516,  516,  100,  516,  138,  516,
 /*  4260 */   516,  100,  516,  516,  101,  102,  516,  728,  586,  516,
 /*  4270 */   516,  276,  516,  516,  516,  100,  516,  516,  516,  275,
 /*  4280 */   277,  278,  279,  280,  281,  516,  516,  215,  224,  227,
 /*  4290 */   218,  221,   95,   96,   98,   97,   99,  516,  516,  259,
 /*  4300 */   516,   95,   96,   98,   97,   99,   95,   96,   98,   97,
 /*  4310 */    99,  211,  268,  273,  289,  208,  302,  516,  263,  516,
 /*  4320 */    95,   96,   98,   97,   99,  516,  296,  516,  516,  516,
 /*  4330 */   516,  282,  516,  253,  516,  101,  102,  283,  516,  516,
 /*  4340 */   516,  516,  516,  105,  101,  102,  100,  516,  516,  101,
 /*  4350 */   102,  516,   23,  516,  516,  100,  516,  138,  516,  516,
 /*  4360 */   100,  516,  516,  101,  102,  516,  516,  516,  516,  516,
 /*  4370 */   276,  588,  516,  516,  100,  516,  516,  516,  275,  277,
 /*  4380 */   278,  279,  280,  281,  516,  516,  215,  224,  227,  218,
 /*  4390 */   221,  516,  516,  516,  516,  516,  516,  516,  259,  516,
 /*  4400 */   516,  516,  516,  516,  516,  516,  516,  516,  516,  516,
 /*  4410 */   211,  268,  273,  289,  208,  516,  516,  263,  516,  516,
 /*  4420 */   516,  516,  516,  516,  516,  296,  516,  516,  516,  516,
 /*  4430 */   282,  516,  253,  516,  516,  516,  283,  516,  516,  516,
 /*  4440 */   516,  516,  516,  516,  516,  516,  516,  516,  516,  516,
 /*  4450 */   516,  516,  516,  516,  516,  516,  516,  516,  516,  516,
 /*  4460 */   516,  516,  516,  516,  516,  516,  516,  516,  516,  276,
 /*  4470 */   516,  516,  516,  516,  516,  516,  516,  275,  277,  278,
 /*  4480 */   279,  280,  281,  516,  516,  215,  224,  227,  218,  221,
 /*  4490 */   516,  516,  516,  516,  516,  516,  516,  259,  516,  516,
 /*  4500 */   516,  516,  516,  516,  516,  516,  516,  516,  516,  211,
 /*  4510 */   268,  273,  289,  208,  516,  516,  285,  516,  516,  516,
 /*  4520 */   516,  516,  516,  516,  296,  516,  516,  516,  516,  282,
 /*  4530 */   516,  253,  516,  516,  516,  283,  516,  516,  516,  516,
 /*  4540 */   516,  516,  516,  516,  516,  516,  516,  516,  516,  516,
 /*  4550 */   516,  516,  516,  516,  516,  516,  516,  516,  516,  516,
 /*  4560 */   516,  516,  516,  516,  516,  516,  516,  516,  340,  516,
 /*  4570 */   516,  516,  516,  516,  516,  516,  339,  277,  278,  279,
 /*  4580 */   280,  281,
};
static YYCODETYPE yy_lookahead[] = {
 /*     0 */     7,    8,    9,   10,   11,   25,   26,   27,   28,   29,
 /*    10 */    30,    0,   19,  105,  106,  107,  108,  109,  110,  111,
 /*    20 */   112,   47,  178,  179,   31,   32,   33,   34,   35,  116,
 /*    30 */    49,   38,  119,  120,  121,  122,  123,  123,   57,   46,
 /*    40 */    41,  128,  129,  129,   51,   46,   53,  134,  134,   75,
 /*    50 */    57,   77,   59,   60,   61,   62,   63,   64,   65,   66,
 /*    60 */    67,   68,   69,  121,  122,  123,   85,   86,   87,   88,
 /*    70 */    89,  129,   32,   92,   93,  131,  134,   38,   38,   39,
 /*    80 */    38,  137,  138,   90,  140,   46,   46,   47,   48,   47,
 /*    90 */    48,   98,   99,  100,  101,  102,  103,  178,  179,   59,
 /*   100 */    60,   61,   62,   63,   64,   65,   66,   67,    6,   69,
 /*   110 */    70,   71,   72,   46,   74,  164,   32,    6,   78,   79,
 /*   120 */    80,   81,   38,   39,   84,  174,  175,  176,  177,  164,
 /*   130 */    46,   47,   48,  168,   94,   95,   96,   97,   36,  174,
 /*   140 */   175,  176,  177,   59,   60,   61,   62,   63,   64,   65,
 /*   150 */    66,   67,   38,   69,   70,   71,   72,   46,   74,   47,
 /*   160 */    39,  164,   78,   79,   80,   81,  169,   46,   84,   32,
 /*   170 */    49,  174,  175,  176,  177,   38,   39,    6,   94,   95,
 /*   180 */    96,   97,  164,   46,   47,   48,  115,   75,  117,   77,
 /*   190 */   172,  173,  174,  175,  176,  177,   59,   60,   61,   62,
 /*   200 */    63,   64,   65,   66,   67,    6,   69,   70,   71,   72,
 /*   210 */    39,   74,  164,   32,   39,   78,   79,   80,   81,   38,
 /*   220 */    39,   84,  174,  175,  176,  177,  164,   46,   47,   48,
 /*   230 */   168,   94,   95,   96,   97,   36,  174,  175,  176,  177,
 /*   240 */    59,   60,   61,   62,   63,   64,   65,   66,   67,   49,
 /*   250 */    69,   70,   71,   72,  119,   74,  121,  122,  123,   78,
 /*   260 */    79,   80,   81,  128,  129,   84,   32,  167,   39,  134,
 /*   270 */   170,  171,   38,   39,    6,   94,   95,   96,   97,   41,
 /*   280 */    46,   47,   48,   54,   46,   85,   86,   87,   88,   89,
 /*   290 */    28,   29,   30,   59,   60,   61,   62,   63,   64,   65,
 /*   300 */    66,   67,    6,   69,   70,   71,   72,   39,   74,  164,
 /*   310 */    32,   38,   78,   79,   80,   81,   38,   39,   84,  174,
 /*   320 */   175,  176,  177,  164,   46,   47,   48,  168,   94,   95,
 /*   330 */    96,   97,   36,  174,  175,  176,  177,   59,   60,   61,
 /*   340 */    62,   63,   64,   65,   66,   67,   49,   69,   70,   71,
 /*   350 */    72,   53,   74,   38,   57,  164,   78,   79,   80,   81,
 /*   360 */   169,   46,   84,   32,   39,  174,  175,  176,  177,   38,
 /*   370 */    39,   46,   94,   95,   96,   97,  113,   46,   47,   48,
 /*   380 */   162,  163,   85,   86,   87,   88,   89,  178,  179,   91,
 /*   390 */    59,   60,   61,   62,   63,   64,   65,   66,   67,    6,
 /*   400 */    69,   70,   71,   72,  167,   74,  164,   32,  171,   78,
 /*   410 */    79,   80,   81,   38,   39,   84,  174,  175,  176,  177,
 /*   420 */   164,   46,   47,   48,  168,   94,   95,   96,   97,   36,
 /*   430 */   174,  175,  176,  177,   59,   60,   61,   62,   63,   64,
 /*   440 */    65,   66,   67,   49,   69,   70,   71,   72,   38,   74,
 /*   450 */     6,   57,  164,   78,   79,   80,   81,  123,   48,   84,
 /*   460 */    32,  173,  174,  175,  176,  177,   38,   39,  134,   94,
 /*   470 */    95,   96,   97,    6,   46,   47,   48,   53,   54,   85,
 /*   480 */    86,   87,   88,   89,  178,  179,   39,   59,   60,   61,
 /*   490 */    62,   63,   64,   65,   66,   67,   38,   69,   70,   71,
 /*   500 */    72,   54,   74,   36,   32,   46,   78,   79,   80,   81,
 /*   510 */    38,   39,   84,   38,  161,   91,    6,  164,   46,   47,
 /*   520 */    48,   46,   94,   95,   96,   97,   82,  174,  175,  176,
 /*   530 */   177,   59,   60,   61,   62,   63,   64,   65,   66,   67,
 /*   540 */   113,   69,   70,   71,   72,  115,   74,  117,  164,   39,
 /*   550 */    78,   79,   80,   81,  178,  179,   84,   32,  174,  175,
 /*   560 */   176,  177,  161,   38,   39,  164,   94,   95,   96,   97,
 /*   570 */   186,   46,   47,   48,  134,  174,  175,  176,  177,  108,
 /*   580 */   109,  110,  111,  112,   59,   60,   61,   62,   63,   64,
 /*   590 */    65,   66,   67,    6,   69,   70,   71,   72,   38,   74,
 /*   600 */    38,   32,   38,   78,   79,   80,   81,   38,   39,   84,
 /*   610 */    46,  161,    6,    6,  164,   46,   47,   48,  137,   94,
 /*   620 */    95,   96,   97,   36,  174,  175,  176,  177,   59,   60,
 /*   630 */    61,   62,   63,   64,   65,   66,   67,   39,   69,   70,
 /*   640 */    71,   72,   36,   74,   46,  164,   39,   78,   79,   80,
 /*   650 */    81,   39,   90,   84,   32,  174,  175,  176,  177,  161,
 /*   660 */    38,   39,  164,   94,   95,   96,   97,  164,   46,   47,
 /*   670 */    48,  168,  174,  175,  176,  177,   53,  174,  175,  176,
 /*   680 */   177,   59,   60,   61,   62,   63,   64,   65,   66,   67,
 /*   690 */     6,   69,   70,   71,   72,   15,   74,  164,   32,   35,
 /*   700 */    78,   79,   80,   81,   38,   39,   84,  174,  175,  176,
 /*   710 */   177,  164,   46,   47,   48,  168,   94,   95,   96,   97,
 /*   720 */    36,  174,  175,  176,  177,   59,   60,   61,   62,   63,
 /*   730 */    64,   65,   66,   67,    6,   69,   70,   71,   72,  161,
 /*   740 */    74,  115,  164,  117,   78,   79,   80,   81,   58,   39,
 /*   750 */    84,   32,  174,  175,  176,  177,   46,   38,   39,   49,
 /*   760 */    94,   95,   96,   97,   36,   46,   47,   48,  133,   38,
 /*   770 */   135,  136,  137,  138,  139,  178,  179,   46,   59,   60,
 /*   780 */    61,   62,   63,   64,   65,   66,   67,    6,   69,   70,
 /*   790 */    71,   72,   38,   74,  164,   32,   20,   78,   79,   80,
 /*   800 */    81,   38,   39,   84,  174,  175,  176,  177,  164,   46,
 /*   810 */    47,   48,  168,   94,   95,   96,   97,   36,  174,  175,
 /*   820 */   176,  177,   59,   60,   61,   62,   63,   64,   65,   66,
 /*   830 */    67,    6,   69,   70,   71,   72,    6,   74,   39,    6,
 /*   840 */   164,   78,   79,   80,   81,  169,   54,   84,   32,   57,
 /*   850 */   174,  175,  176,  177,   38,   39,    6,   94,   95,   96,
 /*   860 */    97,   36,   46,   47,   48,  133,   36,  135,  136,  137,
 /*   870 */   138,  139,   39,  178,  179,   59,   60,   61,   62,   63,
 /*   880 */    64,   65,   66,   67,    6,   69,   70,   71,   72,   39,
 /*   890 */    74,  164,   32,   49,   78,   79,   80,   81,   38,   39,
 /*   900 */    84,  174,  175,  176,  177,  164,   46,   47,   48,  168,
 /*   910 */    94,   95,   96,   97,   36,  174,  175,  176,  177,   59,
 /*   920 */    60,   61,   62,   63,   64,   65,   66,   67,  124,   69,
 /*   930 */    70,   71,   72,   91,   74,   39,  164,    6,   78,   79,
 /*   940 */    80,   81,  165,  166,   84,   32,  174,  175,  176,  177,
 /*   950 */    54,   38,   39,    6,   94,   95,   96,   97,  123,   46,
 /*   960 */    47,   48,  133,   46,  135,  136,  137,  138,  139,  134,
 /*   970 */    39,   54,   59,   60,   61,   62,   63,   64,   65,   66,
 /*   980 */    67,    6,   69,   70,   71,   72,   39,   74,  164,   32,
 /*   990 */    51,   78,   79,   80,   81,   38,   39,   84,  174,  175,
 /*  1000 */   176,  177,  164,   46,   47,   48,  168,   94,   95,   96,
 /*  1010 */    97,   36,  174,  175,  176,  177,   59,   60,   61,   62,
 /*  1020 */    63,   64,   65,   66,   67,   49,   69,   70,   71,   72,
 /*  1030 */   124,   74,   38,  164,   49,   78,   79,   80,   81,  178,
 /*  1040 */   179,   84,   32,  174,  175,  176,  177,   46,   38,   39,
 /*  1050 */    20,   94,   95,   96,   97,   54,   46,   47,   48,  133,
 /*  1060 */    38,  135,  136,  137,  138,  139,  126,  127,   46,   59,
 /*  1070 */    60,   61,   62,   63,   64,   65,   66,   67,  124,   69,
 /*  1080 */    70,   71,   72,   38,   74,  164,   32,   57,   78,   79,
 /*  1090 */    80,   81,   38,   39,   84,  174,  175,  176,  177,  164,
 /*  1100 */    46,   47,   48,  168,   94,   95,   96,   97,   49,  174,
 /*  1110 */   175,  176,  177,   59,   60,   61,   62,   63,   64,   65,
 /*  1120 */    66,   67,  124,   69,   70,   71,   72,   38,   74,   20,
 /*  1130 */   164,    6,   78,   79,   80,   81,  178,  179,   84,   32,
 /*  1140 */   174,  175,  176,  177,   43,   38,   39,   46,   94,   95,
 /*  1150 */    96,   97,  123,   46,   47,   48,  133,   46,  135,  136,
 /*  1160 */   137,  138,  139,  134,   39,   54,   59,   60,   61,   62,
 /*  1170 */    63,   64,   65,   66,   67,   38,   69,   70,   71,   72,
 /*  1180 */    53,   74,  164,   32,   39,   78,   79,   80,   81,   38,
 /*  1190 */    39,   84,  174,  175,  176,  177,  164,   46,   47,   48,
 /*  1200 */   168,   94,   95,   96,   97,   39,  174,  175,  176,  177,
 /*  1210 */    59,   60,   61,   62,   63,   64,   65,   66,   67,    6,
 /*  1220 */    69,   70,   71,   72,   38,   74,  114,  164,    6,   78,
 /*  1230 */    79,   80,   81,  114,   39,   84,   32,  174,  175,  176,
 /*  1240 */   177,   38,   38,   39,    6,   94,   95,   96,   97,   54,
 /*  1250 */    46,   47,   48,  133,   51,  135,  136,  137,  138,  139,
 /*  1260 */    47,   39,   38,   59,   60,   61,   62,   63,   64,   65,
 /*  1270 */    66,   67,   38,   69,   70,   71,   72,   39,   74,  164,
 /*  1280 */    32,  114,   78,   79,   80,   81,   38,   39,   84,  174,
 /*  1290 */   175,  176,  177,  164,   46,   47,   48,   47,   94,   95,
 /*  1300 */    96,   97,   38,  174,  175,  176,  177,   59,   60,   61,
 /*  1310 */    62,   63,   64,   65,   66,   67,   53,   69,   70,   71,
 /*  1320 */    72,   39,   74,   46,  164,    6,   78,   79,   80,   81,
 /*  1330 */   178,  179,   84,   32,  174,  175,  176,  177,   54,   38,
 /*  1340 */    39,   57,   94,   95,   96,   97,   39,   46,   47,   48,
 /*  1350 */   133,   46,  135,  136,  137,  138,  139,   46,   39,   54,
 /*  1360 */    59,   60,   61,   62,   63,   64,   65,   66,   67,   73,
 /*  1370 */    69,   70,   71,   72,  163,   74,  164,   32,  124,   78,
 /*  1380 */    79,   80,   81,   38,   39,   84,  174,  175,  176,  177,
 /*  1390 */   164,   46,   47,   48,   76,   94,   95,   96,   97,   46,
 /*  1400 */   174,  175,  176,  177,   59,   60,   61,   62,   63,   64,
 /*  1410 */    65,   66,   67,   46,   69,   70,   71,   72,   46,   74,
 /*  1420 */    79,  164,   39,   78,   79,   80,   81,   38,   36,   84,
 /*  1430 */    32,  174,  175,  176,  177,  180,   38,   39,   38,   94,
 /*  1440 */    95,   96,   97,   20,   46,   47,   48,  133,   53,  135,
 /*  1450 */   136,  137,  138,  139,   38,    6,  180,   59,   60,   61,
 /*  1460 */    62,   63,   64,   65,   66,   67,   53,   69,   70,   71,
 /*  1470 */    72,   53,   74,  164,   32,   38,   78,   79,   80,   81,
 /*  1480 */    38,   39,   84,  174,  175,  176,  177,  164,   46,   47,
 /*  1490 */    48,   47,   94,   95,   96,   97,   53,  174,  175,  176,
 /*  1500 */   177,   59,   60,   61,   62,   63,   64,   65,   66,   67,
 /*  1510 */    38,   69,   70,   71,   72,   47,   74,   53,  164,   38,
 /*  1520 */    78,   79,   80,   81,   47,   47,   84,   32,  174,  175,
 /*  1530 */   176,  177,   53,   38,   39,  164,   94,   95,   96,   97,
 /*  1540 */   164,   46,   47,   48,   53,  174,  175,  176,  177,   76,
 /*  1550 */   174,  175,  176,  177,   59,   60,   61,   62,   63,   64,
 /*  1560 */    65,   66,   67,   76,   69,   70,   71,   72,   76,   74,
 /*  1570 */   164,   32,   47,   78,   79,   80,   81,   38,   39,   84,
 /*  1580 */   174,  175,  176,  177,  164,   46,   47,   48,   53,   94,
 /*  1590 */    95,   96,   97,   38,  174,  175,  176,  177,   59,   60,
 /*  1600 */    61,   62,   63,   64,   65,   66,   67,   47,   69,   70,
 /*  1610 */    71,   72,   47,   74,   38,   38,   38,   78,   79,   80,
 /*  1620 */    81,  166,   58,   84,   32,    1,    2,    3,    4,    5,
 /*  1630 */    38,   39,  167,   94,   95,   96,   97,  167,   46,   37,
 /*  1640 */    48,  167,   40,   47,   42,   90,   44,   45,   38,  167,
 /*  1650 */    48,   59,   60,   61,   62,   63,   64,   65,   66,   67,
 /*  1660 */    47,   69,   70,   71,   72,  167,   74,   90,   44,   45,
 /*  1670 */    78,   79,   80,   81,  112,   35,   84,   38,   38,   55,
 /*  1680 */   136,  137,  138,  139,   19,  164,   94,   95,   96,   97,
 /*  1690 */   137,  138,   58,  140,  132,  174,  175,  176,  177,  167,
 /*  1700 */    90,  167,   58,  141,  142,  143,  144,  145,  146,  147,
 /*  1710 */   148,  149,  150,  151,  152,  153,  154,  155,  156,  157,
 /*  1720 */   158,  159,  160,  112,   59,   60,   61,   62,   63,   64,
 /*  1730 */    65,   66,   67,   68,   69,  164,  174,  175,  176,  177,
 /*  1740 */    53,   54,  167,  132,   57,  174,  175,  176,  177,  167,
 /*  1750 */    39,   39,  141,  142,  143,  144,  145,  146,  147,  148,
 /*  1760 */   149,  150,  151,  152,  153,  154,  155,  156,  157,  158,
 /*  1770 */   159,  160,  112,   39,  131,   49,   39,   41,   91,   43,
 /*  1780 */   137,  138,   46,  140,  131,  174,  175,  176,  177,   49,
 /*  1790 */   137,  138,  132,  140,   54,   38,    6,   57,   38,  179,
 /*  1800 */    39,  141,  142,  143,  144,  145,  146,  147,  148,  149,
 /*  1810 */   150,  151,  152,  153,  154,  155,  156,  157,  158,  159,
 /*  1820 */   160,  112,   39,   38,  124,   85,   86,   87,   88,   89,
 /*  1830 */    38,   91,   92,   93,  174,  175,  176,  177,  131,   38,
 /*  1840 */    47,  132,   47,   39,  137,  138,   82,  140,   58,   79,
 /*  1850 */   141,  142,  143,  144,  145,  146,  147,  148,  149,  150,
 /*  1860 */   151,  152,  153,  154,  155,  156,  157,  158,  159,  160,
 /*  1870 */   112,   73,   76,  120,  121,  122,  123,   46,   46,   46,
 /*  1880 */    38,  128,  129,  174,  175,  176,  177,  134,   53,   41,
 /*  1890 */   132,   43,   51,   46,   46,   46,  124,   39,  124,  141,
 /*  1900 */   142,  143,  144,  145,  146,  147,  148,  149,  150,  151,
 /*  1910 */   152,  153,  154,  155,  156,  157,  158,  159,  160,  112,
 /*  1920 */    49,   51,   49,   39,  125,  125,  124,   39,   39,   39,
 /*  1930 */   164,   38,  174,  175,  176,  177,  127,   39,  125,  132,
 /*  1940 */   174,  175,  176,  177,  124,   39,  125,   39,  141,  142,
 /*  1950 */   143,  144,  145,  146,  147,  148,  149,  150,  151,  152,
 /*  1960 */   153,  154,  155,  156,  157,  158,  159,  160,  112,  123,
 /*  1970 */   117,  114,   42,   38,  114,   38,  130,   46,   42,  164,
 /*  1980 */   134,  174,  175,  176,  177,   38,  114,   38,  132,  174,
 /*  1990 */   175,  176,  177,  114,  114,  114,  187,  141,  142,  143,
 /*  2000 */   144,  145,  146,  147,  148,  149,  150,  151,  152,  153,
 /*  2010 */   154,  155,  156,  157,  158,  159,  160,  112,  187,  187,
 /*  2020 */   187,  187,  187,  187,  187,  187,  187,  187,  164,  187,
 /*  2030 */   174,  175,  176,  177,  187,  187,  187,  132,  174,  175,
 /*  2040 */   176,  177,  187,  187,  187,  187,  141,  142,  143,  144,
 /*  2050 */   145,  146,  147,  148,  149,  150,  151,  152,  153,  154,
 /*  2060 */   155,  156,  157,  158,  159,  160,  112,  187,  187,  187,
 /*  2070 */   187,  187,  187,  187,  187,  187,  187,  164,  187,  174,
 /*  2080 */   175,  176,  177,  187,  187,  187,  132,  174,  175,  176,
 /*  2090 */   177,  187,  187,  187,  187,  141,  142,  143,  144,  145,
 /*  2100 */   146,  147,  148,  149,  150,  151,  152,  153,  154,  155,
 /*  2110 */   156,  157,  158,  159,  160,  112,  187,  187,  187,  187,
 /*  2120 */   187,  187,  187,  187,  187,  187,  164,  187,  174,  175,
 /*  2130 */   176,  177,  187,  187,  187,  132,  174,  175,  176,  177,
 /*  2140 */   187,  187,  187,  187,  141,  142,  143,  144,  145,  146,
 /*  2150 */   147,  148,  149,  150,  151,  152,  153,  154,  155,  156,
 /*  2160 */   157,  158,  159,  160,  112,  187,  187,  187,  187,  187,
 /*  2170 */   187,  187,  187,  187,  187,  164,  187,  174,  175,  176,
 /*  2180 */   177,  187,  187,  187,  132,  174,  175,  176,  177,  187,
 /*  2190 */   187,  187,  187,  141,  142,  143,  144,  145,  146,  147,
 /*  2200 */   148,  149,  150,  151,  152,  153,  154,  155,  156,  157,
 /*  2210 */   158,  159,  160,  112,  187,  187,  187,  121,  187,  123,
 /*  2220 */   187,  187,  187,  187,  128,  129,  174,  175,  176,  177,
 /*  2230 */   134,  187,  187,  132,  187,  187,  187,  187,  187,  187,
 /*  2240 */   187,  187,  141,  142,  143,  144,  145,  146,  147,  148,
 /*  2250 */   149,  150,  151,  152,  153,  154,  155,  156,  157,  158,
 /*  2260 */   159,  160,  112,  187,  187,  187,  187,  118,  187,  187,
 /*  2270 */   187,  187,  123,  187,  187,  174,  175,  176,  177,  130,
 /*  2280 */   187,  187,  132,  134,  187,  187,  187,  187,  187,  187,
 /*  2290 */   187,  141,  142,  143,  144,  145,  146,  147,  148,  149,
 /*  2300 */   150,  151,  152,  153,  154,  155,  156,  157,  158,  159,
 /*  2310 */   160,  112,  187,  187,  187,  187,  187,  187,  187,  187,
 /*  2320 */   187,  187,  187,  187,  174,  175,  176,  177,  187,  187,
 /*  2330 */   187,  132,  187,  187,  187,  187,  187,  187,  187,  187,
 /*  2340 */   141,  142,  143,  144,  145,  146,  147,  148,  149,  150,
 /*  2350 */   151,  152,  153,  154,  155,  156,  157,  158,  159,  160,
 /*  2360 */   112,  187,  187,  187,  187,  187,  187,  187,  187,  187,
 /*  2370 */   187,  187,  187,  174,  175,  176,  177,  187,  187,  187,
 /*  2380 */   132,  187,  187,  187,  187,  187,  187,  187,  187,  141,
 /*  2390 */   142,  143,  144,  145,  146,  147,  148,  149,  150,  151,
 /*  2400 */   152,  153,  154,  155,  156,  157,  158,  159,  160,  112,
 /*  2410 */   187,  187,  187,  187,  187,  187,  187,  187,  187,  187,
 /*  2420 */   187,  187,  174,  175,  176,  177,  187,  187,  187,  132,
 /*  2430 */   187,  187,  187,  187,  187,  187,  187,  187,  141,  142,
 /*  2440 */   143,  144,  145,  146,  147,  148,  149,  150,  151,  152,
 /*  2450 */   153,  154,  155,  156,  157,  158,  159,  160,  112,  187,
 /*  2460 */   187,  187,  187,  187,  187,  187,  187,  187,  187,  187,
 /*  2470 */   187,  174,  175,  176,  177,  187,  187,  187,  132,  187,
 /*  2480 */   187,  187,  187,  187,  187,  187,  187,  141,  142,  143,
 /*  2490 */   144,  145,  146,  147,  148,  149,  150,  151,  152,  153,
 /*  2500 */   154,  155,  156,  157,  158,  159,  160,  112,  187,  187,
 /*  2510 */   187,  187,  187,  187,  187,  187,  187,  187,  187,  187,
 /*  2520 */   174,  175,  176,  177,  187,  187,  187,  132,  187,  187,
 /*  2530 */   187,  187,  187,  187,  187,  187,  141,  142,  143,  144,
 /*  2540 */   145,  146,  147,  148,  149,  150,  151,  152,  153,  154,
 /*  2550 */   155,  156,  157,  158,  159,  160,  112,  187,  187,  187,
 /*  2560 */   187,  187,  187,  187,  187,  187,  187,  187,  187,  174,
 /*  2570 */   175,  176,  177,  187,  187,  187,  132,  187,  187,  187,
 /*  2580 */   187,  187,  187,  187,  187,  141,  142,  143,  144,  145,
 /*  2590 */   146,  147,  148,  149,  150,  151,  152,  153,  154,  155,
 /*  2600 */   156,  157,  158,  159,  160,  112,  187,  187,  187,  187,
 /*  2610 */   187,   38,  187,  187,  187,  187,  187,  187,  174,  175,
 /*  2620 */   176,  177,  187,  187,   51,  132,  187,  187,  187,  187,
 /*  2630 */   187,  187,  187,  187,  141,  142,  143,  144,  145,  146,
 /*  2640 */   147,  148,  149,  150,  151,  152,  153,  154,  155,  156,
 /*  2650 */   157,  158,  159,  160,  112,  187,  187,  187,  187,  187,
 /*  2660 */   187,  187,  187,   90,  187,  187,  187,  174,  175,  176,
 /*  2670 */   177,   98,   99,  100,  101,  102,  103,  187,  187,  187,
 /*  2680 */   187,  187,  187,  141,  142,  143,  144,  145,  146,  147,
 /*  2690 */   148,  149,  150,  151,  152,  153,  154,  155,  156,  157,
 /*  2700 */   158,  159,  160,    7,    8,    9,   10,   11,  187,  187,
 /*  2710 */   187,  187,  187,  187,  187,   19,  174,  175,  176,  177,
 /*  2720 */   187,   53,   54,  187,  187,   57,  187,   31,   32,   33,
 /*  2730 */    34,   35,   36,  187,   38,   19,  187,  187,  187,  187,
 /*  2740 */   187,  187,   46,  187,   76,  187,  187,   51,  187,   53,
 /*  2750 */   187,  187,   36,   57,   38,   12,   13,   14,  187,   91,
 /*  2760 */    17,   18,   19,   20,   21,   22,   23,   24,   25,   26,
 /*  2770 */    27,   28,   29,   30,  187,   59,   60,   61,   62,   63,
 /*  2780 */    64,   65,   66,   67,   68,   69,   90,  187,  187,  187,
 /*  2790 */   187,  187,  187,  187,   98,   99,  100,  101,  102,  103,
 /*  2800 */   164,   58,    7,    8,    9,   10,   11,  187,  187,  187,
 /*  2810 */   174,  175,  176,  177,   19,  187,  187,  181,  187,  187,
 /*  2820 */   184,  185,  186,  187,  187,  187,   31,   32,   33,   34,
 /*  2830 */    35,   36,  187,   38,   19,  187,  187,  187,  187,  187,
 /*  2840 */   187,   46,  187,  187,  187,  187,   51,  187,   53,  187,
 /*  2850 */   187,   36,   57,   38,   12,   13,   14,  187,  187,   17,
 /*  2860 */    18,   19,   20,   21,   22,   23,   24,   25,   26,   27,
 /*  2870 */    28,   29,   30,  187,   59,   60,   61,   62,   63,   64,
 /*  2880 */    65,   66,   67,   68,   69,   90,  187,  187,   46,  187,
 /*  2890 */   187,  187,  187,   98,   99,  100,  101,  102,  103,  164,
 /*  2900 */   187,    7,    8,    9,   10,   11,  187,  187,  187,  174,
 /*  2910 */   175,  176,  177,   19,  187,  187,  187,  187,  187,  184,
 /*  2920 */   185,  186,  187,  187,  187,   31,   32,   33,   34,   35,
 /*  2930 */   187,  164,   38,  187,    1,    2,    3,    4,    5,  187,
 /*  2940 */    46,  174,  175,  176,  177,   51,  187,   53,  187,  182,
 /*  2950 */   183,   57,   58,   12,   13,   14,  187,  187,   17,   18,
 /*  2960 */    19,   20,   21,   22,   23,   24,   25,   26,   27,   28,
 /*  2970 */    29,   30,  187,   19,  187,  187,  187,   44,   45,  187,
 /*  2980 */    47,   48,  187,   50,   90,  187,  187,   46,   55,  187,
 /*  2990 */   187,  187,   98,   99,  100,  101,  102,  103,  187,  187,
 /*  3000 */     7,    8,    9,   10,   11,  187,  187,  187,  187,  187,
 /*  3010 */    56,  187,   19,   59,   60,   61,   62,   63,   64,   65,
 /*  3020 */    66,   67,   68,   69,   31,   32,   33,   34,   35,  187,
 /*  3030 */   164,   38,   39,  187,  187,  187,  187,  187,  187,   46,
 /*  3040 */   174,  175,  176,  177,   51,  187,   53,  187,  182,  183,
 /*  3050 */    57,  187,   12,   13,   14,  187,  187,   17,   18,   19,
 /*  3060 */    20,   21,   22,   23,   24,   25,   26,   27,   28,   29,
 /*  3070 */    30,  187,  187,  187,  187,  187,  187,  187,  187,   39,
 /*  3080 */   187,  187,  187,   90,  187,    1,    2,    3,    4,    5,
 /*  3090 */   187,   98,   99,  100,  101,  102,  103,  187,  187,    7,
 /*  3100 */     8,    9,   10,   11,  187,  187,  187,  187,  187,  187,
 /*  3110 */   187,   19,  187,  187,  187,  187,  187,  187,  187,  187,
 /*  3120 */   187,  187,  187,   31,   32,   33,   34,   35,   44,   45,
 /*  3130 */    38,   47,   48,    0,   50,  187,  187,  187,   46,   55,
 /*  3140 */   187,  187,  187,   51,  187,   53,  187,  187,  187,   57,
 /*  3150 */    58,   12,   13,   14,  187,  187,   17,   18,   19,   20,
 /*  3160 */    21,   22,   23,   24,   25,   26,   27,   28,   29,   30,
 /*  3170 */    37,  187,  187,   40,  187,   42,  187,   44,   45,  187,
 /*  3180 */   187,   48,   90,  187,  187,   46,  187,  187,  187,  187,
 /*  3190 */    98,   99,  100,  101,  102,  103,  164,  187,    7,    8,
 /*  3200 */     9,   10,   11,  187,  187,  187,  174,  175,  176,  177,
 /*  3210 */    19,  187,  187,  187,  182,  183,  187,  187,  187,  187,
 /*  3220 */   187,  187,   31,   32,   33,   34,   35,   36,  164,   38,
 /*  3230 */   187,    1,    2,    3,    4,    5,  187,   46,  174,  175,
 /*  3240 */   176,  177,   51,  187,   53,  187,  182,  183,   57,  187,
 /*  3250 */    12,   13,   14,  187,  187,   17,   18,   19,   20,   21,
 /*  3260 */    22,   23,   24,   25,   26,   27,   28,   29,   30,  187,
 /*  3270 */   187,  187,  187,  187,   44,   45,  187,   39,  187,  187,
 /*  3280 */    50,   90,  187,  187,  187,   55,  187,  187,  187,   98,
 /*  3290 */    99,  100,  101,  102,  103,  164,  187,    7,    8,    9,
 /*  3300 */    10,   11,  187,  187,  187,  174,  175,  176,  177,   19,
 /*  3310 */   187,  187,  187,  182,  183,  187,  187,  187,  187,  187,
 /*  3320 */   187,   31,   32,   33,   34,   35,   36,  187,   38,   19,
 /*  3330 */   187,  187,  187,  187,  187,  187,   46,  187,  187,  187,
 /*  3340 */   187,   51,  187,   53,  187,  187,   36,   57,   38,   12,
 /*  3350 */    13,   14,  187,  187,   17,   18,   19,   20,   21,   22,
 /*  3360 */    23,   24,   25,   26,   27,   28,   29,   30,  187,   59,
 /*  3370 */    60,   61,   62,   63,   64,   65,   66,   67,   68,   69,
 /*  3380 */    90,  187,    1,    2,    3,    4,    5,  187,   98,   99,
 /*  3390 */   100,  101,  102,  103,  187,   58,    7,    8,    9,   10,
 /*  3400 */    11,  187,  187,  187,  187,  187,  187,  187,   19,  187,
 /*  3410 */   187,  187,  187,  187,  187,  187,  187,  187,  187,  187,
 /*  3420 */    31,   32,   33,   34,   35,   44,   45,   38,   47,   48,
 /*  3430 */   187,   50,  187,  187,  187,   46,   55,  187,  187,  187,
 /*  3440 */    51,  187,   53,  187,  187,  187,   57,   58,   12,   13,
 /*  3450 */    14,  187,  187,   17,   18,   19,   20,   21,   22,   23,
 /*  3460 */    24,   25,   26,   27,   28,   29,   30,  187,  187,  187,
 /*  3470 */   187,  187,   36,  187,    1,    2,    3,    4,    5,   90,
 /*  3480 */   187,  187,  187,  187,  187,  187,  187,   98,   99,  100,
 /*  3490 */   101,  102,  103,  164,  187,    7,    8,    9,   10,   11,
 /*  3500 */   187,  187,  187,  174,  175,  176,  177,   19,  187,  187,
 /*  3510 */   187,   38,  183,  187,  187,  187,  187,   44,   45,   31,
 /*  3520 */    32,   33,   34,   35,   36,   52,   38,   19,   55,  187,
 /*  3530 */   187,  187,  187,  187,   46,  187,  187,  187,  187,   51,
 /*  3540 */   187,   53,  187,  187,   36,   57,   38,   12,   13,   14,
 /*  3550 */   187,  187,   17,   18,   19,   20,   21,   22,   23,   24,
 /*  3560 */    25,   26,   27,   28,   29,   30,  187,   59,   60,   61,
 /*  3570 */    62,   63,   64,   65,   66,   67,   68,   69,   90,  187,
 /*  3580 */     1,    2,    3,    4,    5,  187,   98,   99,  100,  101,
 /*  3590 */   102,  103,  164,   58,    7,    8,    9,   10,   11,  187,
 /*  3600 */   187,  187,  174,  175,  176,  177,   19,  187,  187,  187,
 /*  3610 */   182,  183,  187,  187,  187,  187,  187,  187,   31,   32,
 /*  3620 */    33,   34,   35,   44,   45,   38,   47,   48,  187,   50,
 /*  3630 */   187,  187,  187,   46,   55,  187,  187,  187,   51,  187,
 /*  3640 */    53,  187,  187,  187,   57,   58,   12,   13,   14,  187,
 /*  3650 */   187,   17,   18,   19,   20,   21,   22,   23,   24,   25,
 /*  3660 */    26,   27,   28,   29,   30,  187,  187,  187,  187,  187,
 /*  3670 */   187,  187,  187,  187,  187,  187,  187,   90,  187,  187,
 /*  3680 */   187,  187,  187,  187,  187,   98,   99,  100,  101,  102,
 /*  3690 */   103,  164,   58,    7,    8,    9,   10,   11,  187,  187,
 /*  3700 */   187,  174,  175,  176,  177,   19,  187,  187,  187,  182,
 /*  3710 */   183,  187,  187,  187,  187,  187,  187,   31,   32,   33,
 /*  3720 */    34,   35,   36,  164,   38,   19,  187,  187,  187,  187,
 /*  3730 */   187,  187,   46,  174,  175,  176,  177,   51,  187,   53,
 /*  3740 */   187,  182,  183,   57,   38,   12,   13,   14,  187,  187,
 /*  3750 */    17,   18,   19,   20,   21,   22,   23,   24,   25,   26,
 /*  3760 */    27,   28,   29,   30,  187,   59,   60,   61,   62,   63,
 /*  3770 */    64,   65,   66,   67,   68,   69,   90,  187,  187,   46,
 /*  3780 */   187,  187,  187,  187,   98,   99,  100,  101,  102,  103,
 /*  3790 */   164,  187,    7,    8,    9,   10,   11,  187,  187,  187,
 /*  3800 */   174,  175,  176,  177,   19,  187,  187,  187,  182,  183,
 /*  3810 */   187,  187,  187,  187,  187,  187,   31,   32,   33,   34,
 /*  3820 */    35,   36,  164,   38,  187,    1,    2,    3,    4,    5,
 /*  3830 */   187,   46,  174,  175,  176,  177,   51,  187,   53,  187,
 /*  3840 */   182,  183,   57,  187,   12,   13,   14,  187,  187,   17,
 /*  3850 */    18,   19,   20,   21,   22,   23,   24,   25,   26,   27,
 /*  3860 */    28,   29,   30,  187,  187,  187,  187,  187,   44,   45,
 /*  3870 */   187,   47,   48,  187,  187,   90,  187,  187,  187,   55,
 /*  3880 */   187,  187,  187,   98,   99,  100,  101,  102,  103,  187,
 /*  3890 */   187,    7,    8,    9,   10,   11,  187,  187,  187,  187,
 /*  3900 */   187,  187,  187,   19,  187,  187,  187,  187,  187,  187,
 /*  3910 */   187,  187,  187,  187,  187,   31,   32,   33,   34,   35,
 /*  3920 */   187,  187,   38,  187,  187,  187,  187,  187,  187,  187,
 /*  3930 */    46,  187,  187,  187,  187,   51,  187,   53,  187,  187,
 /*  3940 */   187,   57,  187,  187,   13,   14,  187,  187,   17,   18,
 /*  3950 */    19,   20,   21,   22,   23,   24,   25,   26,   27,   28,
 /*  3960 */    29,   30,  187,  187,  187,  187,  187,   83,  187,  187,
 /*  3970 */   187,  187,  187,  187,   90,  187,  187,  187,  187,  187,
 /*  3980 */   187,  187,   98,   99,  100,  101,  102,  103,  187,  187,
 /*  3990 */     7,    8,    9,   10,   11,  187,  187,  187,  187,  187,
 /*  4000 */   187,  187,   19,  187,  187,  187,  187,  187,  187,  187,
 /*  4010 */   187,  187,  187,  187,   31,   32,   33,   34,   35,   36,
 /*  4020 */   187,   38,  187,    1,    2,    3,    4,    5,  187,   46,
 /*  4030 */   187,  187,  187,  187,   51,  187,   53,  187,  187,  187,
 /*  4040 */    57,  187,  187,  187,  187,  187,  187,   17,   18,   19,
 /*  4050 */    20,   21,   22,   23,   24,   25,   26,   27,   28,   29,
 /*  4060 */    30,  187,  187,  187,  187,  187,   44,   45,  187,   47,
 /*  4070 */    48,  187,  187,   90,  187,  187,  187,   55,  187,  187,
 /*  4080 */   187,   98,   99,  100,  101,  102,  103,  187,  187,    7,
 /*  4090 */     8,    9,   10,   11,    1,    2,    3,    4,    5,  187,
 /*  4100 */   187,   19,  187,    1,    2,    3,    4,    5,    1,    2,
 /*  4110 */     3,    4,    5,   31,   32,   33,   34,   35,  187,  187,
 /*  4120 */    38,  187,  187,  187,  187,  187,  187,  187,   46,  187,
 /*  4130 */   187,  187,  187,   51,  187,   53,  187,   44,   45,   57,
 /*  4140 */    47,   48,  187,  187,  187,  187,   44,   45,   55,   47,
 /*  4150 */    48,   44,   45,  187,   47,   48,  187,   55,  187,  187,
 /*  4160 */   187,  187,   55,  187,  187,   83,  187,  187,  187,  187,
 /*  4170 */   187,  187,   90,  187,  187,  187,  187,  187,  187,  187,
 /*  4180 */    98,   99,  100,  101,  102,  103,  187,  187,    7,    8,
 /*  4190 */     9,   10,   11,    1,    2,    3,    4,    5,  187,  187,
 /*  4200 */    19,  187,    1,    2,    3,    4,    5,    1,    2,    3,
 /*  4210 */     4,    5,   31,   32,   33,   34,   35,   36,  187,   38,
 /*  4220 */   187,    1,    2,    3,    4,    5,  187,   46,  187,  187,
 /*  4230 */   187,  187,   51,  187,   53,  187,   44,   45,   57,   47,
 /*  4240 */    48,  187,  187,  187,   38,   44,   45,   55,   47,   48,
 /*  4250 */    44,   45,  187,  187,  187,  187,   55,  187,   52,  187,
 /*  4260 */   187,   55,  187,  187,   44,   45,  187,   47,   48,  187,
 /*  4270 */   187,   90,  187,  187,  187,   55,  187,  187,  187,   98,
 /*  4280 */    99,  100,  101,  102,  103,  187,  187,    7,    8,    9,
 /*  4290 */    10,   11,    1,    2,    3,    4,    5,  187,  187,   19,
 /*  4300 */   187,    1,    2,    3,    4,    5,    1,    2,    3,    4,
 /*  4310 */     5,   31,   32,   33,   34,   35,   36,  187,   38,  187,
 /*  4320 */     1,    2,    3,    4,    5,  187,   46,  187,  187,  187,
 /*  4330 */   187,   51,  187,   53,  187,   44,   45,   57,  187,  187,
 /*  4340 */   187,  187,  187,   52,   44,   45,   55,  187,  187,   44,
 /*  4350 */    45,  187,   52,  187,  187,   55,  187,   52,  187,  187,
 /*  4360 */    55,  187,  187,   44,   45,  187,  187,  187,  187,  187,
 /*  4370 */    90,   52,  187,  187,   55,  187,  187,  187,   98,   99,
 /*  4380 */   100,  101,  102,  103,  187,  187,    7,    8,    9,   10,
 /*  4390 */    11,  187,  187,  187,  187,  187,  187,  187,   19,  187,
 /*  4400 */   187,  187,  187,  187,  187,  187,  187,  187,  187,  187,
 /*  4410 */    31,   32,   33,   34,   35,  187,  187,   38,  187,  187,
 /*  4420 */   187,  187,  187,  187,  187,   46,  187,  187,  187,  187,
 /*  4430 */    51,  187,   53,  187,  187,  187,   57,  187,  187,  187,
 /*  4440 */   187,  187,  187,  187,  187,  187,  187,  187,  187,  187,
 /*  4450 */   187,  187,  187,  187,  187,  187,  187,  187,  187,  187,
 /*  4460 */   187,  187,  187,  187,  187,  187,  187,  187,  187,   90,
 /*  4470 */   187,  187,  187,  187,  187,  187,  187,   98,   99,  100,
 /*  4480 */   101,  102,  103,  187,  187,    7,    8,    9,   10,   11,
 /*  4490 */   187,  187,  187,  187,  187,  187,  187,   19,  187,  187,
 /*  4500 */   187,  187,  187,  187,  187,  187,  187,  187,  187,   31,
 /*  4510 */    32,   33,   34,   35,  187,  187,   38,  187,  187,  187,
 /*  4520 */   187,  187,  187,  187,   46,  187,  187,  187,  187,   51,
 /*  4530 */   187,   53,  187,  187,  187,   57,  187,  187,  187,  187,
 /*  4540 */   187,  187,  187,  187,  187,  187,  187,  187,  187,  187,
 /*  4550 */   187,  187,  187,  187,  187,  187,  187,  187,  187,  187,
 /*  4560 */   187,  187,  187,  187,  187,  187,  187,  187,   90,  187,
 /*  4570 */   187,  187,  187,  187,  187,  187,   98,   99,  100,  101,
 /*  4580 */   102,  103,
};
#define YY_SHIFT_USE_DFLT (-27)
static short yy_shift_ofst[] = {
 /*     0 */  1602,   11, 3133,  -27,  -27,  -27,  -27,  -27,  114,  175,
 /*    10 */   -27,  273,  238,  -27,  458,  459,  -27, 4192,  -27, 4201,
 /*    20 */   -27,  -27, 4300,  560,  623, 3508,  229, 2954,  612,  -27,
 /*    30 */   -27,  680, 1665,  -27,  664,  -27,  -27,  -27,  754, 1030,
 /*    40 */   -27,  690,  776,  -27,  -27,  -27,  -27,  -27,  -27,  -27,
 /*    50 */   -27,  -27,  -27,  -27,  -27,  -27,  -27,  781,  447, 2954,
 /*    60 */   799,  -27,  -27, 3706,  -27,  844, 2573,  -27,  -27,  -27,
 /*    70 */   -27,  -27,  -27,  -27,  -27,  842,  939,  -27,  -27, 1640,
 /*    80 */   976, 2573,  -27,  994,  985, 2573,  -27, 1045, 1059, 2573,
 /*    90 */   -27, 1089, 1109,  -27,  -27,  -27,  -27,  -27,  -27,  -27,
 /*   100 */   -27,  -27,  -27, 1624, 4291, 1137, 1127, 3310,  896, 2954,
 /*   110 */  1145,  -27,  -27,  825, 1195, 2954, 1166,  -27,  -27,  -27,
 /*   120 */   -27, 1186, 1848,  -27, 1224, 1101,  -27, 1234,  111,  -27,
 /*   130 */  3579,  -27, 1250,  -27, 2933, 3824,  -27, 4305, 1264, 1263,
 /*   140 */  2815,  917, 1151,  -27, 1248,  -27,  -27,  -27,  -27,  -27,
 /*   150 */   -27,  -27,  -27,  -27,  -27,  -27,  -27,  -27,  -27,  -27,
 /*   160 */   -27,  -27,  -27,  -27,  -27,  -27,  -27,  -27, 1282,  -27,
 /*   170 */  1307,  -27, 4379,   67, 1539, 1495, 1296, 1277,   84,  137,
 /*   180 */   -27, 4379, 1311,  -26,  -27,  112,  -27,  -27, 2573, 1318,
 /*   190 */  1592, 1592, 1353, 1204, 1010,  -27, 4379, 1367, 1442,   40,
 /*   200 */   -27, 1372, 1107,  913, 1341, 4379, 1383,  -27, 4379,  -27,
 /*   210 */  4379, 1389,  -27,  792, 4379, 4379, 3832, 4379, 4379, 3832,
 /*   220 */  4379, 4379, 3832, 4379, 4379, 3832, 4379, 4379, 3832, 4379,
 /*   230 */   -20, 4379,  -20, 4379,  -20, 4379,  262, 4379,  262, 4379,
 /*   240 */   -27, 4379,  -27, 4379,  -27, 4379,  262, 4379, 4030, 4379,
 /*   250 */  4030, 4379, 3931,   -7, 3436,  -27, 1392, 4379,  -27, 1400,
 /*   260 */  1423, 4379,  -20, 1687,   39, 1395, 3983, 3832, 1416, 1449,
 /*   270 */  1389,  -27,  -27, 4379,  -27,  -27,  -27,  -27,  -27,  -27,
 /*   280 */   -27,  -27,  -27, 3389, 3832, 2668, 1203, 1413, 3191,  731,
 /*   290 */  1418, 3785,  -27,  199,  -27, 4379, 1437, 1444,  298, 4280,
 /*   300 */   684,  -27,  -27,  -27,  -27,  -27,  -27,  475, 1443, 3290,
 /*   310 */   587,  -27,  -27, 1472, 1468, 1464, 3686,  467,  -27,  -27,
 /*   320 */   -27, 1481, 1477, 1479, 3488,  -27,  393,  -27,  296,  -27,
 /*   330 */   -27,  -27, 4379, 3337,  -27, 4181,  830,  -27,  -27, 1473,
 /*   340 */  1487,  -27, 1790,  -27, 4478,  -27, 1492, 4379,  -27,  -27,
 /*   350 */   -27,  878,  -27,  -27, 1610, 1478, 1491, 2795,  975,  -27,
 /*   360 */   -27, 1525, 1535, 2696,  102,  -27,  -27,  -20,  -20,  -20,
 /*   370 */   -20, 2743,  -27,  315,  -27,  562, 1560,  -27, 1565,  -27,
 /*   380 */   -20, 3832, 1576,  444, 4082, 3733, 1592, 1345,  -27, 1578,
 /*   390 */   947,  -27, 1578,  -27, 1740,  -27,  -27,  -27,  -27,  -27,
 /*   400 */   -27, 4379,  -27, 3832,  564,  -19, 4379,  -27, 3587,  200,
 /*   410 */  4379,  -27, 1564,  200, 4379,  -27, 3634,  -27,  -27, 1555,
 /*   420 */  1596,  200, 4379,  -27, 1613,  200, 4379,  -27, 1639,  394,
 /*   430 */  4379,  -27, 3092,  200, 4379,  -27, 1634,  200, 4379,  -27,
 /*   440 */  2894,  200, 4379,  -27, 1644,  -27,  297, 4379,  -27,  -27,
 /*   450 */  4379,  -27,  -27,  -27,  -27, 4379, 1222,  -27, 4379,  -27,
 /*   460 */  3832,  -27, 1711,  -27, 1712,  -27, 1734,  -27, 1737,  -27,
 /*   470 */  2993, 3040,  -27,  -27, 1757, 1284, 1760, 1761,  -27, 4379,
 /*   480 */  3535, 1783,  -27, 4379, 3238,  -27, 1785,  931,  -27, 1785,
 /*   490 */   -27, 1726, 2573,  -27,  -27, 1785,  833,  -27, 1785,  510,
 /*   500 */   -27, 1785, 1319,  -27, 1785, 1238,  -27, 1785,  171,  -27,
 /*   510 */  1785,  268,  -27, 1785,  607,  -27, 1785,  850,  -27, 1785,
 /*   520 */  1125,  -27,  424, 1792, 1413, 1022, 1395, 1577, 1793, 1491,
 /*   530 */  1795, 1535,  -27,  -27, 4379, 2941, 1592, 1054,  -27, 1801,
 /*   540 */  1764, 3884, 2842, 1592,  622,  -27, 4379, 3139, 1592,  234,
 /*   550 */   -27, 1770, 4379, 1804,  -27,  -27,  -27, 1796, 1592, 1592,
 /*   560 */   -27,  -27, 1798, 1831, 1398,  428,  -27,  -27, 2954, 1832,
 /*   570 */   719,  -27,  816,  -27,  728, 1001,  666,  -27,  569,  -27,
 /*   580 */  2954, 1833,  375,  -27,  278,  -27, 1624, 4319, 1842, 1835,
 /*   590 */  2716, 1111, 1301,  -27,  181,  -27, 2954, 1847,  331,  -27,
 /*   600 */   472,  -27,  606, 1305,  525,  -27,  763,  -27, 2954, 1849,
 /*   610 */   860,  -27,  957,  -27, 3381, 4220,  -27,  -27, 3230, 1841,
 /*   620 */  1871, 2573, 1858,  -27, 1870, 1873, 2573, 1884,  -27,  -27,
 /*   630 */  3230, 3473,  710,  -27, 2573,  598,  -27, 1888,  -27,   42,
 /*   640 */  1889, 1213, 1890,  410,  -27,  -27, 1893,  -27,  -27, 1898,
 /*   650 */   -27, 4206,  121,  -27, 2573,  325,  -27, 1906,  -27, 1908,
 /*   660 */   -27,  -27, 3084, 4022, 4102, 1624, 4093,  -27, 4107, 1234,
 /*   670 */   -27,  -27,  -27, 1234,  111,  -27, 1930, 1935, 1736,  -27,
 /*   680 */  1937, 1931,  -27, 1234,  111,  -27, 1936, 1947,   -1,  -27,
 /*   690 */  1949, 1931,  -27,  -27,
};
#define YY_REDUCE_USE_DFLT (-157)
static short yy_reduce_ofst[] = {
 /*     0 */   -92, -157,  471, -157, -157, -157, -157, -157, -157, -157,
 /*    10 */  -157, -157,  263, -157, -157,  427, -157, 2149, -157, 1846,
 /*    20 */  -157, -157,  440, -157, -157, 1653, -157,  926, -157, -157,
 /*    30 */  -157, -157, 1544, -157, -157, -157, -157, -157, -157, -157,
 /*    40 */  -157, -157, -157, -157, -157, -157, -157, -157, -157, -157,
 /*    50 */  -157, -157, -157, -157, -157, -157, -157, -157, -157, 1217,
 /*    60 */  -157, -157, -157, 1553, -157, -157,  804, -157, -157, -157,
 /*    70 */  -157, -157, -157, -157, -157, -157, -157, -157, -157, -157,
 /*    80 */  -157,  906, -157, -157, -157,  954, -157, -157, -157,  998,
 /*    90 */  -157, -157, -157, -157, -157, -157, -157, -157, -157, -157,
 /*   100 */  -157, -157, -157,  835,  440, -157, -157,  -56, -157,  635,
 /*   110 */  -157, -157, -157, -157, -157, 1314, -157, -157, -157, -157,
 /*   120 */  -157, -157, 1112, -157, -157, 1119, -157,   71, 1167, -157,
 /*   130 */   -87, -157, -157, -157, 1753,  -86, -157,  440, -157, -157,
 /*   140 */  1643, -157, 1660, -157, 2542, -157, -157, -157, -157, -157,
 /*   150 */  -157, -157, -157, -157, -157, -157, -157, -157, -157, -157,
 /*   160 */  -157, -157, -157, -157, -157, -157, -157, -157, -157, -157,
 /*   170 */  -157, -157,  578, -157, 1807, 2542, -157, -157, 2346, 2542,
 /*   180 */  -157,  498, -157,  218, -157, 1211, -157, -157, 1254, -157,
 /*   190 */  2297, 2542, -157, 1709, 2542, -157,  401, -157, 2003, 2542,
 /*   200 */  -157, -157, 2444, 2542, -157,  353, -157, -157,  921, -157,
 /*   210 */  1063, 1255, -157, -157,  533, 1815, -157,  824,  145, -157,
 /*   220 */   966, 1406, -157, 1129, 1212, -157, 1226, 1257, -157, 1309,
 /*   230 */  -157, 1371, -157, 1420, -157, 1521, -157, 1766, -157, 1913,
 /*   240 */  -157, 2011, -157,  630, -157,  727, -157, 1962, -157, 1376,
 /*   250 */  -157, 1160, -157,  481, -157, -157, -157,  772, -157, -157,
 /*   260 */  -157,  242, -157, -157, -157, -157, 3559, -157, -157, -157,
 /*   270 */  1276, -157, -157, 1354, -157, -157, -157, -157, -157, -157,
 /*   280 */  -157, -157, -157, 2636, -157, -157, -157, -157, 2866, -157,
 /*   290 */  -157, 2767, -157, -157, -157, 3329, -157, -157, -157, 3658,
 /*   300 */  -157, -157, -157, -157, -157, -157, -157, -157, -157, 3626,
 /*   310 */  -157, -157, -157, -157, -157, -157, 3527, -157, -157, -157,
 /*   320 */  -157, -157, -157, -157, 3428, -157, -157, -157, -157, -157,
 /*   330 */  -157, -157, 1864, -157, -157, 3131, -157, -157, -157, -157,
 /*   340 */  -157, -157, -157, -157, 2735, -157, -157,  384, -157, -157,
 /*   350 */  -157, -157, -157, -157, -157, -157, -157, 3064, -157, -157,
 /*   360 */  -157, -157, -157, 3032, -157, -157, -157, -157, -157, -157,
 /*   370 */  -157, -157, -157, -157, -157, -157, -157, -157, -157, -157,
 /*   380 */  -157, -157, -157, -157,  -49, -157, 2248, 2542, -157,  777,
 /*   390 */  -157, -157, 1455, -157,  100, -157, -157, -157, -157, -157,
 /*   400 */  -157,  503, -157, -157, -157, 1465,  159, -157,   -3, 1470,
 /*   410 */    62, -157, -157, 1474,  -35, -157, -157, -157, -157, -157,
 /*   420 */  -157, 1482,  935, -157, -157, 1498,  741, -157, -157, 1532,
 /*   430 */   256, -157,  676, 1534,  644, -157, -157, 1575,  838, -157,
 /*   440 */   191, 1582, 1032, -157, -157, -157,  237,  547, -157, -157,
 /*   450 */   191, -157, -157, -157, -157,   18, -157, -157,  288, -157,
 /*   460 */  -157, -157, -157, -157, -157, -157, -157, -157, -157, -157,
 /*   470 */   869, -157, -157, -157, -157, -157, -157, -157, -157, 1115,
 /*   480 */  -157, -157, -157, 1323, -157, -157,  861, -157, -157, 1620,
 /*   490 */  -157, -157, 1700, -157, -157,  695, -157, -157,  376, -157,
 /*   500 */  -157,  -81, -157, -157, 1152, -157, -157, -156, -157, -157,
 /*   510 */   209, -157, -157,  306, -157, -157,  597, -157, -157,  958,
 /*   520 */  -157, -157, -157, -157, -157, -157, -157, -157, -157, -157,
 /*   530 */  -157, -157, -157, -157, 1018, -157, 2493, 2542, -157, -157,
 /*   540 */  -157, 1571, -157, 2150, 2542, -157,   48, -157, 1856, 2542,
 /*   550 */  -157, -157,  450, -157, -157, -157, -157, -157, 1905, 2542,
 /*   560 */  -157, -157, -157, -157, 2199, 2542, -157, -157, 1023, -157,
 /*   570 */  2101, -157, 2542, -157, -157, -157, 1954, -157, 2542, -157,
 /*   580 */   829, -157, 1562, -157, 2542, -157, 1029,  440, -157, -157,
 /*   590 */  1707, -157, 1758, -157, 2542, -157,  732, -157, 1611, -157,
 /*   600 */  2542, -157, -157, -157, 2052, -157, 2542, -157, 1120, -157,
 /*   610 */  2395, -157, 2542, -157, 2096,  -86, -157, -157, 1029, -157,
 /*   620 */  -157, 1772, -157, -157, -157, -157, 1774, -157, -157, -157,
 /*   630 */   334,  440, 1799, -157, 1802, 1800, -157, -157, -157,  940,
 /*   640 */  -157, -157, -157, 1809, -157, -157, -157, -157, -157, -157,
 /*   650 */  -157,  440, 1813, -157, 1820, 1821, -157, -157, -157, -157,
 /*   660 */  -157, -157,  135,  -58,  -86,  334,  -86, -157,  -86, 1853,
 /*   670 */  -157, -157, -157,  626, 1857, -157, -157, -157, 1860, -157,
 /*   680 */  -157, 1872, -157,  430, 1879, -157, -157, -157, 1880, -157,
 /*   690 */  -157, 1881, -157, -157,
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
 /*   230 */   935, 1021,  936, 1021,  937, 1021,  947, 1021,  948, 1021,
 /*   240 */   949, 1021,  950, 1021,  951, 1021,  952, 1021,  953, 1021,
 /*   250 */   954, 1021,  955, 1021, 1021,  938, 1021, 1021,  939, 1021,
 /*   260 */  1021, 1021,  940,  959, 1021,  941, 1021,  997, 1021, 1021,
 /*   270 */  1021,  956,  957, 1021,  958,  960,  961,  962,  963,  964,
 /*   280 */   965,  966,  967, 1021, 1005,  959, 1021,  944, 1021, 1021,
 /*   290 */   970, 1021,  971, 1021,  972, 1021, 1021, 1021, 1021, 1021,
 /*   300 */  1021,  978,  979,  992,  993,  994,  996, 1021, 1021, 1021,
 /*   310 */  1021,  982,  983, 1021, 1021, 1021, 1021, 1021,  984,  985,
 /*   320 */   995, 1021, 1021,  973, 1021,  974, 1021,  975, 1021,  980,
 /*   330 */   981,  945, 1021, 1021,  946, 1021, 1021,  976,  977,  960,
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
 /* 252 */ "xx_common_expr ::= IDENTIFIER SBRACKET_OPEN xx_common_expr SBRACKET_CLOSE",
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
// 999 "parser.lemon"
{
	/*if ((yypminor->yy0)) {
		if ((yypminor->yy0)->free_flag) {
			efree((yypminor->yy0)->token);
		}
		efree((yypminor->yy0));
	}*/
}
// 2815 "parser.c"
      break;
    case 106:
// 1012 "parser.lemon"
{ json_object_put((yypminor->yy115)); }
// 2820 "parser.c"
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
// 1008 "parser.lemon"
{
	status->ret = yymsp[0].minor.yy115;
}
// 3364 "parser.c"
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
// 1014 "parser.lemon"
{
	yygotominor.yy115 = yymsp[0].minor.yy115;
}
// 3404 "parser.c"
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
// 1018 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_list(yymsp[-1].minor.yy115, yymsp[0].minor.yy115);
}
// 3419 "parser.c"
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
// 1022 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_list(NULL, yymsp[0].minor.yy115);
}
// 3443 "parser.c"
        break;
      case 8:
// 1042 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_namespace(yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(37,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 3452 "parser.c"
        break;
      case 9:
// 1046 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_interface(yymsp[-1].minor.yy0, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(40,&yymsp[-2].minor);
}
// 3460 "parser.c"
        break;
      case 10:
// 1050 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_interface(yymsp[-3].minor.yy0, yymsp[0].minor.yy115, yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(40,&yymsp[-4].minor);
  yy_destructor(41,&yymsp[-2].minor);
}
// 3469 "parser.c"
        break;
      case 11:
// 1054 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class(yymsp[-1].minor.yy0, yymsp[0].minor.yy115, 0, 0, NULL, NULL, status->scanner_state);
  yy_destructor(42,&yymsp[-2].minor);
}
// 3477 "parser.c"
        break;
      case 12:
// 1058 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy115, 0, 0, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(42,&yymsp[-4].minor);
  yy_destructor(41,&yymsp[-2].minor);
}
// 3486 "parser.c"
        break;
      case 13:
// 1062 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy115, 0, 0, NULL, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(42,&yymsp[-4].minor);
  yy_destructor(43,&yymsp[-2].minor);
}
// 3495 "parser.c"
        break;
      case 14:
// 1066 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class(yymsp[-5].minor.yy0, yymsp[0].minor.yy115, 0, 0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(42,&yymsp[-6].minor);
  yy_destructor(41,&yymsp[-4].minor);
  yy_destructor(43,&yymsp[-2].minor);
}
// 3505 "parser.c"
        break;
      case 15:
// 1070 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class(yymsp[-1].minor.yy0, yymsp[0].minor.yy115, 1, 0, NULL, NULL, status->scanner_state);
  yy_destructor(44,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-2].minor);
}
// 3514 "parser.c"
        break;
      case 16:
// 1074 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy115, 1, 0, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(44,&yymsp[-5].minor);
  yy_destructor(42,&yymsp[-4].minor);
  yy_destructor(41,&yymsp[-2].minor);
}
// 3524 "parser.c"
        break;
      case 17:
// 1078 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy115, 1, 0, NULL, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(44,&yymsp[-5].minor);
  yy_destructor(42,&yymsp[-4].minor);
  yy_destructor(43,&yymsp[-2].minor);
}
// 3534 "parser.c"
        break;
      case 18:
// 1082 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class(yymsp[-1].minor.yy0, yymsp[0].minor.yy115, 0, 1, NULL, NULL, status->scanner_state);
  yy_destructor(45,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-2].minor);
}
// 3543 "parser.c"
        break;
      case 19:
// 1086 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy115, 0, 1, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(45,&yymsp[-5].minor);
  yy_destructor(42,&yymsp[-4].minor);
  yy_destructor(41,&yymsp[-2].minor);
}
// 3553 "parser.c"
        break;
      case 20:
      case 25:
      case 46:
// 1090 "parser.lemon"
{
	yygotominor.yy115 = NULL;
  yy_destructor(46,&yymsp[-1].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 3564 "parser.c"
        break;
      case 21:
      case 26:
      case 47:
// 1094 "parser.lemon"
{
	yygotominor.yy115 = yymsp[-1].minor.yy115;
  yy_destructor(46,&yymsp[-2].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 3575 "parser.c"
        break;
      case 22:
      case 48:
      case 102:
      case 174:
      case 202:
      case 224:
      case 301:
      case 304:
// 1098 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_list(yymsp[-2].minor.yy115, yymsp[0].minor.yy115);
  yy_destructor(6,&yymsp[-1].minor);
}
// 3590 "parser.c"
        break;
      case 24:
      case 265:
      case 308:
// 1106 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state);
}
// 3599 "parser.c"
        break;
      case 27:
// 1118 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_definition(yymsp[0].minor.yy115, NULL, NULL, status->scanner_state);
}
// 3606 "parser.c"
        break;
      case 28:
// 1122 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_definition(NULL, NULL, yymsp[0].minor.yy115, status->scanner_state);
}
// 3613 "parser.c"
        break;
      case 29:
// 1126 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_definition(NULL, yymsp[0].minor.yy115, NULL, status->scanner_state);
}
// 3620 "parser.c"
        break;
      case 30:
// 1130 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_definition(yymsp[-1].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
}
// 3627 "parser.c"
        break;
      case 31:
// 1134 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_definition(yymsp[-1].minor.yy115, NULL, yymsp[0].minor.yy115, status->scanner_state);
}
// 3634 "parser.c"
        break;
      case 32:
// 1138 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_definition(yymsp[0].minor.yy115, NULL, yymsp[-1].minor.yy115, status->scanner_state);
}
// 3641 "parser.c"
        break;
      case 33:
// 1142 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_definition(NULL, yymsp[0].minor.yy115, yymsp[-1].minor.yy115, status->scanner_state);
}
// 3648 "parser.c"
        break;
      case 34:
// 1146 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_definition(yymsp[-2].minor.yy115, yymsp[0].minor.yy115, yymsp[-1].minor.yy115, status->scanner_state);
}
// 3655 "parser.c"
        break;
      case 35:
// 1150 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_definition(yymsp[-1].minor.yy115, yymsp[0].minor.yy115, yymsp[-2].minor.yy115, status->scanner_state);
}
// 3662 "parser.c"
        break;
      case 38:
// 1167 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_property(yymsp[-2].minor.yy115, yymsp[-1].minor.yy0, NULL, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(39,&yymsp[0].minor);
}
// 3670 "parser.c"
        break;
      case 39:
// 1171 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_property(yymsp[-2].minor.yy115, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(39,&yymsp[0].minor);
}
// 3678 "parser.c"
        break;
      case 40:
// 1175 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_property(yymsp[-4].minor.yy115, yymsp[-3].minor.yy0, yymsp[-1].minor.yy115, yymsp[-5].minor.yy0, NULL, status->scanner_state);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 3687 "parser.c"
        break;
      case 41:
// 1179 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_property(yymsp[-4].minor.yy115, yymsp[-3].minor.yy0, yymsp[-1].minor.yy115, NULL, NULL, status->scanner_state);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 3696 "parser.c"
        break;
      case 42:
// 1183 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_property(yymsp[-3].minor.yy115, yymsp[-2].minor.yy0, NULL, yymsp[-4].minor.yy0, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(39,&yymsp[0].minor);
}
// 3704 "parser.c"
        break;
      case 43:
// 1187 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_property(yymsp[-3].minor.yy115, yymsp[-2].minor.yy0, NULL, NULL, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(39,&yymsp[0].minor);
}
// 3712 "parser.c"
        break;
      case 44:
// 1191 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_property(yymsp[-5].minor.yy115, yymsp[-4].minor.yy0, yymsp[-2].minor.yy115, yymsp[-6].minor.yy0, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(49,&yymsp[-3].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 3721 "parser.c"
        break;
      case 45:
// 1195 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_property(yymsp[-5].minor.yy115, yymsp[-4].minor.yy0, yymsp[-2].minor.yy115, NULL, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(49,&yymsp[-3].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 3730 "parser.c"
        break;
      case 50:
// 1215 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_property_shortcut(NULL, yymsp[0].minor.yy0, status->scanner_state);
}
// 3737 "parser.c"
        break;
      case 51:
// 1219 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_property_shortcut(yymsp[-1].minor.yy0, yymsp[0].minor.yy0, status->scanner_state);
}
// 3744 "parser.c"
        break;
      case 58:
// 1248 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy115, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(50,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 3754 "parser.c"
        break;
      case 59:
// 1252 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy115, NULL, status->scanner_state);
  yy_destructor(50,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 3764 "parser.c"
        break;
      case 60:
// 1257 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_method(yymsp[-6].minor.yy115, yymsp[-4].minor.yy0, NULL, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(52,&yymsp[-5].minor);
  yy_destructor(53,&yymsp[-3].minor);
  yy_destructor(36,&yymsp[-2].minor);
  yy_destructor(46,&yymsp[-1].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 3776 "parser.c"
        break;
      case 61:
// 1261 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_method(yymsp[-7].minor.yy115, yymsp[-5].minor.yy0, yymsp[-3].minor.yy115, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(52,&yymsp[-6].minor);
  yy_destructor(53,&yymsp[-4].minor);
  yy_destructor(36,&yymsp[-2].minor);
  yy_destructor(46,&yymsp[-1].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 3788 "parser.c"
        break;
      case 62:
// 1265 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_method(yymsp[-7].minor.yy115, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy115, NULL, NULL, status->scanner_state);
  yy_destructor(52,&yymsp[-6].minor);
  yy_destructor(53,&yymsp[-4].minor);
  yy_destructor(36,&yymsp[-3].minor);
  yy_destructor(46,&yymsp[-2].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 3800 "parser.c"
        break;
      case 63:
// 1269 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_method(yymsp[-8].minor.yy115, yymsp[-6].minor.yy0, yymsp[-4].minor.yy115, yymsp[-1].minor.yy115, NULL, NULL, status->scanner_state);
  yy_destructor(52,&yymsp[-7].minor);
  yy_destructor(53,&yymsp[-5].minor);
  yy_destructor(36,&yymsp[-3].minor);
  yy_destructor(46,&yymsp[-2].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 3812 "parser.c"
        break;
      case 64:
// 1273 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_method(yymsp[-6].minor.yy115, yymsp[-4].minor.yy0, NULL, NULL, yymsp[-7].minor.yy0, NULL, status->scanner_state);
  yy_destructor(52,&yymsp[-5].minor);
  yy_destructor(53,&yymsp[-3].minor);
  yy_destructor(36,&yymsp[-2].minor);
  yy_destructor(46,&yymsp[-1].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 3824 "parser.c"
        break;
      case 65:
// 1277 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_method(yymsp[-7].minor.yy115, yymsp[-5].minor.yy0, yymsp[-3].minor.yy115, NULL, yymsp[-8].minor.yy0, NULL, status->scanner_state);
  yy_destructor(52,&yymsp[-6].minor);
  yy_destructor(53,&yymsp[-4].minor);
  yy_destructor(36,&yymsp[-2].minor);
  yy_destructor(46,&yymsp[-1].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 3836 "parser.c"
        break;
      case 66:
// 1281 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_method(yymsp[-7].minor.yy115, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy115, yymsp[-8].minor.yy0, NULL, status->scanner_state);
  yy_destructor(52,&yymsp[-6].minor);
  yy_destructor(53,&yymsp[-4].minor);
  yy_destructor(36,&yymsp[-3].minor);
  yy_destructor(46,&yymsp[-2].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 3848 "parser.c"
        break;
      case 67:
// 1285 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_method(yymsp[-8].minor.yy115, yymsp[-6].minor.yy0, yymsp[-4].minor.yy115, yymsp[-1].minor.yy115, yymsp[-9].minor.yy0, NULL, status->scanner_state);
  yy_destructor(52,&yymsp[-7].minor);
  yy_destructor(53,&yymsp[-5].minor);
  yy_destructor(36,&yymsp[-3].minor);
  yy_destructor(46,&yymsp[-2].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 3860 "parser.c"
        break;
      case 68:
// 1289 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_method(yymsp[-8].minor.yy115, yymsp[-6].minor.yy0, NULL, NULL, NULL, yymsp[-2].minor.yy115, status->scanner_state);
  yy_destructor(52,&yymsp[-7].minor);
  yy_destructor(53,&yymsp[-5].minor);
  yy_destructor(36,&yymsp[-4].minor);
  yy_destructor(54,&yymsp[-3].minor);
  yy_destructor(46,&yymsp[-1].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 3873 "parser.c"
        break;
      case 69:
// 1293 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_method(yymsp[-9].minor.yy115, yymsp[-7].minor.yy0, yymsp[-5].minor.yy115, NULL, NULL, yymsp[-2].minor.yy115, status->scanner_state);
  yy_destructor(52,&yymsp[-8].minor);
  yy_destructor(53,&yymsp[-6].minor);
  yy_destructor(36,&yymsp[-4].minor);
  yy_destructor(54,&yymsp[-3].minor);
  yy_destructor(46,&yymsp[-1].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 3886 "parser.c"
        break;
      case 70:
// 1297 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_method(yymsp[-9].minor.yy115, yymsp[-7].minor.yy0, NULL, yymsp[-1].minor.yy115, NULL, yymsp[-3].minor.yy115, status->scanner_state);
  yy_destructor(52,&yymsp[-8].minor);
  yy_destructor(53,&yymsp[-6].minor);
  yy_destructor(36,&yymsp[-5].minor);
  yy_destructor(54,&yymsp[-4].minor);
  yy_destructor(46,&yymsp[-2].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 3899 "parser.c"
        break;
      case 71:
// 1301 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_method(yymsp[-10].minor.yy115, yymsp[-8].minor.yy0, yymsp[-6].minor.yy115, yymsp[-1].minor.yy115, NULL, yymsp[-3].minor.yy115, status->scanner_state);
  yy_destructor(52,&yymsp[-9].minor);
  yy_destructor(53,&yymsp[-7].minor);
  yy_destructor(36,&yymsp[-5].minor);
  yy_destructor(54,&yymsp[-4].minor);
  yy_destructor(46,&yymsp[-2].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 3912 "parser.c"
        break;
      case 72:
// 1305 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_method(yymsp[-8].minor.yy115, yymsp[-6].minor.yy0, NULL, NULL, yymsp[-9].minor.yy0, yymsp[-2].minor.yy115, status->scanner_state);
  yy_destructor(52,&yymsp[-7].minor);
  yy_destructor(53,&yymsp[-5].minor);
  yy_destructor(36,&yymsp[-4].minor);
  yy_destructor(54,&yymsp[-3].minor);
  yy_destructor(46,&yymsp[-1].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 3925 "parser.c"
        break;
      case 73:
// 1309 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_method(yymsp[-9].minor.yy115, yymsp[-7].minor.yy0, yymsp[-5].minor.yy115, NULL, yymsp[-10].minor.yy0, yymsp[-2].minor.yy115, status->scanner_state);
  yy_destructor(52,&yymsp[-8].minor);
  yy_destructor(53,&yymsp[-6].minor);
  yy_destructor(36,&yymsp[-4].minor);
  yy_destructor(54,&yymsp[-3].minor);
  yy_destructor(46,&yymsp[-1].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 3938 "parser.c"
        break;
      case 74:
// 1313 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_method(yymsp[-9].minor.yy115, yymsp[-7].minor.yy0, NULL, yymsp[-1].minor.yy115, yymsp[-10].minor.yy0, yymsp[-3].minor.yy115, status->scanner_state);
  yy_destructor(52,&yymsp[-8].minor);
  yy_destructor(53,&yymsp[-6].minor);
  yy_destructor(36,&yymsp[-5].minor);
  yy_destructor(54,&yymsp[-4].minor);
  yy_destructor(46,&yymsp[-2].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 3951 "parser.c"
        break;
      case 75:
// 1317 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_method(yymsp[-10].minor.yy115, yymsp[-8].minor.yy0, yymsp[-6].minor.yy115, yymsp[-1].minor.yy115, yymsp[-11].minor.yy0, yymsp[-3].minor.yy115, status->scanner_state);
  yy_destructor(52,&yymsp[-9].minor);
  yy_destructor(53,&yymsp[-7].minor);
  yy_destructor(36,&yymsp[-5].minor);
  yy_destructor(54,&yymsp[-4].minor);
  yy_destructor(46,&yymsp[-2].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 3964 "parser.c"
        break;
      case 76:
// 1322 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_method(yymsp[-7].minor.yy115, yymsp[-5].minor.yy0, NULL, NULL, NULL, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(52,&yymsp[-6].minor);
  yy_destructor(53,&yymsp[-4].minor);
  yy_destructor(36,&yymsp[-3].minor);
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 3976 "parser.c"
        break;
      case 77:
// 1326 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_method(yymsp[-8].minor.yy115, yymsp[-6].minor.yy0, yymsp[-4].minor.yy115, NULL, NULL, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(52,&yymsp[-7].minor);
  yy_destructor(53,&yymsp[-5].minor);
  yy_destructor(36,&yymsp[-3].minor);
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 3988 "parser.c"
        break;
      case 78:
// 1330 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_method(yymsp[-7].minor.yy115, yymsp[-5].minor.yy0, NULL, NULL, yymsp[-8].minor.yy0, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(52,&yymsp[-6].minor);
  yy_destructor(53,&yymsp[-4].minor);
  yy_destructor(36,&yymsp[-3].minor);
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 4000 "parser.c"
        break;
      case 79:
// 1334 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_method(yymsp[-8].minor.yy115, yymsp[-6].minor.yy0, yymsp[-4].minor.yy115, NULL, yymsp[-9].minor.yy0, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(52,&yymsp[-7].minor);
  yy_destructor(53,&yymsp[-5].minor);
  yy_destructor(36,&yymsp[-3].minor);
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 4012 "parser.c"
        break;
      case 80:
// 1338 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_method(yymsp[-5].minor.yy115, yymsp[-3].minor.yy0, NULL, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(52,&yymsp[-4].minor);
  yy_destructor(53,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[-1].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 4023 "parser.c"
        break;
      case 81:
// 1342 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_method(yymsp[-6].minor.yy115, yymsp[-4].minor.yy0, yymsp[-2].minor.yy115, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(52,&yymsp[-5].minor);
  yy_destructor(53,&yymsp[-3].minor);
  yy_destructor(36,&yymsp[-1].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 4034 "parser.c"
        break;
      case 82:
// 1346 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_method(yymsp[-5].minor.yy115, yymsp[-3].minor.yy0, NULL, NULL, yymsp[-6].minor.yy0, NULL, status->scanner_state);
  yy_destructor(52,&yymsp[-4].minor);
  yy_destructor(53,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[-1].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 4045 "parser.c"
        break;
      case 83:
// 1350 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_method(yymsp[-6].minor.yy115, yymsp[-4].minor.yy0, yymsp[-2].minor.yy115, NULL, yymsp[-7].minor.yy0, NULL, status->scanner_state);
  yy_destructor(52,&yymsp[-5].minor);
  yy_destructor(53,&yymsp[-3].minor);
  yy_destructor(36,&yymsp[-1].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 4056 "parser.c"
        break;
      case 86:
// 1363 "parser.lemon"
{
	yygotominor.yy115 = json_object_new_string("public");
  yy_destructor(1,&yymsp[0].minor);
}
// 4064 "parser.c"
        break;
      case 87:
// 1367 "parser.lemon"
{
	yygotominor.yy115 = json_object_new_string("protected");
  yy_destructor(2,&yymsp[0].minor);
}
// 4072 "parser.c"
        break;
      case 88:
// 1371 "parser.lemon"
{
	yygotominor.yy115 = json_object_new_string("private");
  yy_destructor(4,&yymsp[0].minor);
}
// 4080 "parser.c"
        break;
      case 89:
// 1375 "parser.lemon"
{
	yygotominor.yy115 = json_object_new_string("static");
  yy_destructor(3,&yymsp[0].minor);
}
// 4088 "parser.c"
        break;
      case 90:
// 1379 "parser.lemon"
{
	yygotominor.yy115 = json_object_new_string("scoped");
  yy_destructor(5,&yymsp[0].minor);
}
// 4096 "parser.c"
        break;
      case 91:
// 1383 "parser.lemon"
{
	yygotominor.yy115 = json_object_new_string("inline");
  yy_destructor(55,&yymsp[0].minor);
}
// 4104 "parser.c"
        break;
      case 92:
// 1387 "parser.lemon"
{
	yygotominor.yy115 = json_object_new_string("abstract");
  yy_destructor(44,&yymsp[0].minor);
}
// 4112 "parser.c"
        break;
      case 93:
// 1391 "parser.lemon"
{
	yygotominor.yy115 = json_object_new_string("final");
  yy_destructor(45,&yymsp[0].minor);
}
// 4120 "parser.c"
        break;
      case 94:
// 1396 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_return_type(1, NULL, status->scanner_state);
  yy_destructor(56,&yymsp[0].minor);
}
// 4128 "parser.c"
        break;
      case 95:
// 1400 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_return_type(0, yymsp[0].minor.yy115, status->scanner_state);
}
// 4135 "parser.c"
        break;
      case 96:
// 1404 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_list(yymsp[-2].minor.yy115, yymsp[0].minor.yy115);
  yy_destructor(15,&yymsp[-1].minor);
}
// 4143 "parser.c"
        break;
      case 98:
// 1412 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_return_type_item(yymsp[0].minor.yy115, NULL, 0, 0, status->scanner_state);
}
// 4150 "parser.c"
        break;
      case 99:
// 1416 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_return_type_item(yymsp[-1].minor.yy115, NULL, 1, 0, status->scanner_state);
  yy_destructor(35,&yymsp[0].minor);
}
// 4158 "parser.c"
        break;
      case 100:
// 1420 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_return_type_item(NULL, yymsp[0].minor.yy115, 0, 0, status->scanner_state);
}
// 4165 "parser.c"
        break;
      case 101:
// 1424 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_return_type_item(NULL, yymsp[0].minor.yy115, 0, 1, status->scanner_state);
}
// 4172 "parser.c"
        break;
      case 104:
// 1437 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_parameter(NULL, NULL, yymsp[0].minor.yy0, NULL, 0, status->scanner_state);
}
// 4179 "parser.c"
        break;
      case 105:
// 1441 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_parameter(yymsp[-1].minor.yy115, NULL, yymsp[0].minor.yy0, NULL, 0, status->scanner_state);
}
// 4186 "parser.c"
        break;
      case 106:
// 1445 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_parameter(yymsp[-2].minor.yy115, NULL, yymsp[0].minor.yy0, NULL, 1, status->scanner_state);
  yy_destructor(35,&yymsp[-1].minor);
}
// 4194 "parser.c"
        break;
      case 107:
// 1449 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_parameter(NULL, yymsp[-1].minor.yy115, yymsp[0].minor.yy0, NULL, 0, status->scanner_state);
}
// 4201 "parser.c"
        break;
      case 108:
// 1453 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_parameter(NULL, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy115, 0, status->scanner_state);
  yy_destructor(49,&yymsp[-1].minor);
}
// 4209 "parser.c"
        break;
      case 109:
// 1457 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_parameter(yymsp[-3].minor.yy115, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy115, 0, status->scanner_state);
  yy_destructor(49,&yymsp[-1].minor);
}
// 4217 "parser.c"
        break;
      case 110:
// 1461 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_parameter(yymsp[-4].minor.yy115, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy115, 1, status->scanner_state);
  yy_destructor(35,&yymsp[-3].minor);
  yy_destructor(49,&yymsp[-1].minor);
}
// 4226 "parser.c"
        break;
      case 111:
// 1465 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_parameter(NULL, yymsp[-3].minor.yy115, yymsp[-2].minor.yy0, yymsp[0].minor.yy115, 0, status->scanner_state);
  yy_destructor(49,&yymsp[-1].minor);
}
// 4234 "parser.c"
        break;
      case 112:
// 1469 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(19,&yymsp[-2].minor);
  yy_destructor(20,&yymsp[0].minor);
}
// 4243 "parser.c"
        break;
      case 113:
// 1473 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(19,&yymsp[-4].minor);
  yy_destructor(57,&yymsp[-2].minor);
  yy_destructor(58,&yymsp[-1].minor);
  yy_destructor(20,&yymsp[0].minor);
}
// 4254 "parser.c"
        break;
      case 114:
// 1477 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_type(XX_TYPE_INTEGER);
  yy_destructor(59,&yymsp[0].minor);
}
// 4262 "parser.c"
        break;
      case 115:
// 1481 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_type(XX_TYPE_UINTEGER);
  yy_destructor(60,&yymsp[0].minor);
}
// 4270 "parser.c"
        break;
      case 116:
// 1485 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_type(XX_TYPE_LONG);
  yy_destructor(61,&yymsp[0].minor);
}
// 4278 "parser.c"
        break;
      case 117:
// 1489 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_type(XX_TYPE_ULONG);
  yy_destructor(62,&yymsp[0].minor);
}
// 4286 "parser.c"
        break;
      case 118:
// 1493 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_type(XX_TYPE_CHAR);
  yy_destructor(63,&yymsp[0].minor);
}
// 4294 "parser.c"
        break;
      case 119:
// 1497 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_type(XX_TYPE_UCHAR);
  yy_destructor(64,&yymsp[0].minor);
}
// 4302 "parser.c"
        break;
      case 120:
// 1501 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_type(XX_TYPE_DOUBLE);
  yy_destructor(65,&yymsp[0].minor);
}
// 4310 "parser.c"
        break;
      case 121:
// 1505 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_type(XX_TYPE_BOOL);
  yy_destructor(66,&yymsp[0].minor);
}
// 4318 "parser.c"
        break;
      case 122:
// 1509 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_type(XX_TYPE_STRING);
  yy_destructor(67,&yymsp[0].minor);
}
// 4326 "parser.c"
        break;
      case 123:
// 1513 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_type(XX_TYPE_ARRAY);
  yy_destructor(68,&yymsp[0].minor);
}
// 4334 "parser.c"
        break;
      case 124:
// 1517 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_type(XX_TYPE_VAR);
  yy_destructor(69,&yymsp[0].minor);
}
// 4342 "parser.c"
        break;
      case 147:
// 1609 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_empty_statement(status->scanner_state);
  yy_destructor(39,&yymsp[0].minor);
}
// 4350 "parser.c"
        break;
      case 148:
// 1613 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_break_statement(status->scanner_state);
  yy_destructor(70,&yymsp[-1].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 4359 "parser.c"
        break;
      case 149:
// 1617 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_continue_statement(status->scanner_state);
  yy_destructor(71,&yymsp[-1].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 4368 "parser.c"
        break;
      case 150:
// 1621 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_if_statement(yymsp[-2].minor.yy115, NULL, NULL, status->scanner_state);
  yy_destructor(72,&yymsp[-3].minor);
  yy_destructor(46,&yymsp[-1].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 4378 "parser.c"
        break;
      case 151:
// 1625 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_if_statement(yymsp[-5].minor.yy115, NULL, NULL, status->scanner_state);
  yy_destructor(72,&yymsp[-6].minor);
  yy_destructor(46,&yymsp[-4].minor);
  yy_destructor(47,&yymsp[-3].minor);
  yy_destructor(73,&yymsp[-2].minor);
  yy_destructor(46,&yymsp[-1].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 4391 "parser.c"
        break;
      case 152:
// 1629 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_if_statement(yymsp[-3].minor.yy115, yymsp[-1].minor.yy115, NULL, status->scanner_state);
  yy_destructor(72,&yymsp[-4].minor);
  yy_destructor(46,&yymsp[-2].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 4401 "parser.c"
        break;
      case 153:
// 1633 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_if_statement(yymsp[-7].minor.yy115, yymsp[-5].minor.yy115, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(72,&yymsp[-8].minor);
  yy_destructor(46,&yymsp[-6].minor);
  yy_destructor(47,&yymsp[-4].minor);
  yy_destructor(73,&yymsp[-3].minor);
  yy_destructor(46,&yymsp[-2].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 4414 "parser.c"
        break;
      case 154:
// 1637 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_if_statement(yymsp[-6].minor.yy115, yymsp[-4].minor.yy115, NULL, status->scanner_state);
  yy_destructor(72,&yymsp[-7].minor);
  yy_destructor(46,&yymsp[-5].minor);
  yy_destructor(47,&yymsp[-3].minor);
  yy_destructor(73,&yymsp[-2].minor);
  yy_destructor(46,&yymsp[-1].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 4427 "parser.c"
        break;
      case 155:
// 1641 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_if_statement(yymsp[-6].minor.yy115, NULL, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(72,&yymsp[-7].minor);
  yy_destructor(46,&yymsp[-5].minor);
  yy_destructor(47,&yymsp[-4].minor);
  yy_destructor(73,&yymsp[-3].minor);
  yy_destructor(46,&yymsp[-2].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 4440 "parser.c"
        break;
      case 156:
// 1645 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_switch_statement(yymsp[-2].minor.yy115, NULL, status->scanner_state);
  yy_destructor(74,&yymsp[-3].minor);
  yy_destructor(46,&yymsp[-1].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 4450 "parser.c"
        break;
      case 157:
// 1649 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_switch_statement(yymsp[-3].minor.yy115, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(74,&yymsp[-4].minor);
  yy_destructor(46,&yymsp[-2].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 4460 "parser.c"
        break;
      case 160:
// 1661 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_case_clause(yymsp[-1].minor.yy115, NULL, status->scanner_state);
  yy_destructor(75,&yymsp[-2].minor);
  yy_destructor(76,&yymsp[0].minor);
}
// 4469 "parser.c"
        break;
      case 161:
// 1665 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_case_clause(yymsp[-2].minor.yy115, yymsp[0].minor.yy115, status->scanner_state);
  yy_destructor(75,&yymsp[-3].minor);
  yy_destructor(76,&yymsp[-1].minor);
}
// 4478 "parser.c"
        break;
      case 162:
// 1669 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_case_clause(NULL, yymsp[0].minor.yy115, status->scanner_state);
  yy_destructor(77,&yymsp[-2].minor);
  yy_destructor(76,&yymsp[-1].minor);
}
// 4487 "parser.c"
        break;
      case 163:
// 1673 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_loop_statement(NULL, status->scanner_state);
  yy_destructor(78,&yymsp[-2].minor);
  yy_destructor(46,&yymsp[-1].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 4497 "parser.c"
        break;
      case 164:
// 1677 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_loop_statement(yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(78,&yymsp[-3].minor);
  yy_destructor(46,&yymsp[-2].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 4507 "parser.c"
        break;
      case 165:
// 1681 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_while_statement(yymsp[-2].minor.yy115, NULL, status->scanner_state);
  yy_destructor(79,&yymsp[-3].minor);
  yy_destructor(46,&yymsp[-1].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 4517 "parser.c"
        break;
      case 166:
// 1685 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_while_statement(yymsp[-3].minor.yy115, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(79,&yymsp[-4].minor);
  yy_destructor(46,&yymsp[-2].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 4527 "parser.c"
        break;
      case 167:
// 1689 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_do_while_statement(yymsp[-1].minor.yy115, NULL, status->scanner_state);
  yy_destructor(80,&yymsp[-5].minor);
  yy_destructor(46,&yymsp[-4].minor);
  yy_destructor(47,&yymsp[-3].minor);
  yy_destructor(79,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 4539 "parser.c"
        break;
      case 168:
// 1693 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_do_while_statement(yymsp[-1].minor.yy115, yymsp[-4].minor.yy115, status->scanner_state);
  yy_destructor(80,&yymsp[-6].minor);
  yy_destructor(46,&yymsp[-5].minor);
  yy_destructor(47,&yymsp[-3].minor);
  yy_destructor(79,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 4551 "parser.c"
        break;
      case 169:
// 1697 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_for_statement(yymsp[-3].minor.yy115, NULL, yymsp[-5].minor.yy0, 0, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(81,&yymsp[-6].minor);
  yy_destructor(82,&yymsp[-4].minor);
  yy_destructor(46,&yymsp[-2].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 4562 "parser.c"
        break;
      case 170:
// 1701 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_for_statement(yymsp[-3].minor.yy115, NULL, yymsp[-6].minor.yy0, 1, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(81,&yymsp[-7].minor);
  yy_destructor(82,&yymsp[-5].minor);
  yy_destructor(83,&yymsp[-4].minor);
  yy_destructor(46,&yymsp[-2].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 4574 "parser.c"
        break;
      case 171:
// 1705 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_for_statement(yymsp[-3].minor.yy115, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, 0, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(81,&yymsp[-8].minor);
  yy_destructor(6,&yymsp[-6].minor);
  yy_destructor(82,&yymsp[-4].minor);
  yy_destructor(46,&yymsp[-2].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 4586 "parser.c"
        break;
      case 172:
// 1709 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_for_statement(yymsp[-3].minor.yy115, yymsp[-8].minor.yy0, yymsp[-6].minor.yy0, 1, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(81,&yymsp[-9].minor);
  yy_destructor(6,&yymsp[-7].minor);
  yy_destructor(82,&yymsp[-5].minor);
  yy_destructor(83,&yymsp[-4].minor);
  yy_destructor(46,&yymsp[-2].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 4599 "parser.c"
        break;
      case 173:
// 1713 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_let_statement(yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(84,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 4608 "parser.c"
        break;
      case 176:
// 1726 "parser.lemon"
{
	yygotominor.yy115 = json_object_new_string("assign");
  yy_destructor(49,&yymsp[0].minor);
}
// 4616 "parser.c"
        break;
      case 177:
// 1731 "parser.lemon"
{
	yygotominor.yy115 = json_object_new_string("add-assign");
  yy_destructor(85,&yymsp[0].minor);
}
// 4624 "parser.c"
        break;
      case 178:
// 1736 "parser.lemon"
{
	yygotominor.yy115 = json_object_new_string("sub-assign");
  yy_destructor(86,&yymsp[0].minor);
}
// 4632 "parser.c"
        break;
      case 179:
// 1740 "parser.lemon"
{
	yygotominor.yy115 = json_object_new_string("mult-assign");
  yy_destructor(87,&yymsp[0].minor);
}
// 4640 "parser.c"
        break;
      case 180:
// 1744 "parser.lemon"
{
	yygotominor.yy115 = json_object_new_string("div-assign");
  yy_destructor(88,&yymsp[0].minor);
}
// 4648 "parser.c"
        break;
      case 181:
// 1748 "parser.lemon"
{
	yygotominor.yy115 = json_object_new_string("concat-assign");
  yy_destructor(89,&yymsp[0].minor);
}
// 4656 "parser.c"
        break;
      case 182:
// 1753 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_let_assignment("variable", yymsp[-1].minor.yy115, yymsp[-2].minor.yy0, NULL, NULL, yymsp[0].minor.yy115, status->scanner_state);
}
// 4663 "parser.c"
        break;
      case 183:
// 1758 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_let_assignment("object-property", yymsp[-1].minor.yy115, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, yymsp[0].minor.yy115, status->scanner_state);
  yy_destructor(54,&yymsp[-3].minor);
}
// 4671 "parser.c"
        break;
      case 184:
// 1763 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_let_assignment("variable-dynamic-object-property", yymsp[-1].minor.yy115, yymsp[-6].minor.yy0, yymsp[-3].minor.yy0, NULL, yymsp[0].minor.yy115, status->scanner_state);
  yy_destructor(54,&yymsp[-5].minor);
  yy_destructor(46,&yymsp[-4].minor);
  yy_destructor(47,&yymsp[-2].minor);
}
// 4681 "parser.c"
        break;
      case 185:
// 1768 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_let_assignment("string-dynamic-object-property", yymsp[-1].minor.yy115, yymsp[-6].minor.yy0, yymsp[-3].minor.yy0, NULL, yymsp[0].minor.yy115, status->scanner_state);
  yy_destructor(54,&yymsp[-5].minor);
  yy_destructor(46,&yymsp[-4].minor);
  yy_destructor(47,&yymsp[-2].minor);
}
// 4691 "parser.c"
        break;
      case 186:
// 1773 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_let_assignment("object-property-append", yymsp[-1].minor.yy115, yymsp[-6].minor.yy0, yymsp[-4].minor.yy0, NULL, yymsp[0].minor.yy115, status->scanner_state);
  yy_destructor(54,&yymsp[-5].minor);
  yy_destructor(57,&yymsp[-3].minor);
  yy_destructor(58,&yymsp[-2].minor);
}
// 4701 "parser.c"
        break;
      case 187:
// 1778 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_let_assignment("object-property-array-index", yymsp[-1].minor.yy115, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, yymsp[-3].minor.yy115, yymsp[0].minor.yy115, status->scanner_state);
  yy_destructor(54,&yymsp[-6].minor);
  yy_destructor(57,&yymsp[-4].minor);
  yy_destructor(58,&yymsp[-2].minor);
}
// 4711 "parser.c"
        break;
      case 188:
// 1783 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_let_assignment("static-property", yymsp[-1].minor.yy115, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, yymsp[0].minor.yy115, status->scanner_state);
  yy_destructor(91,&yymsp[-3].minor);
}
// 4719 "parser.c"
        break;
      case 189:
// 1788 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_let_assignment("static-property-append", yymsp[-1].minor.yy115, yymsp[-6].minor.yy0, yymsp[-4].minor.yy0, NULL, yymsp[0].minor.yy115, status->scanner_state);
  yy_destructor(91,&yymsp[-5].minor);
  yy_destructor(57,&yymsp[-3].minor);
  yy_destructor(58,&yymsp[-2].minor);
}
// 4729 "parser.c"
        break;
      case 190:
// 1793 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_let_assignment("static-property-array-index", yymsp[-1].minor.yy115, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, yymsp[-3].minor.yy115, yymsp[0].minor.yy115, status->scanner_state);
  yy_destructor(91,&yymsp[-6].minor);
  yy_destructor(57,&yymsp[-4].minor);
  yy_destructor(58,&yymsp[-2].minor);
}
// 4739 "parser.c"
        break;
      case 191:
// 1798 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_let_assignment("variable-append", yymsp[-1].minor.yy115, yymsp[-4].minor.yy0, NULL, NULL, yymsp[0].minor.yy115, status->scanner_state);
  yy_destructor(57,&yymsp[-3].minor);
  yy_destructor(58,&yymsp[-2].minor);
}
// 4748 "parser.c"
        break;
      case 192:
// 1803 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_let_assignment("array-index", yymsp[-1].minor.yy115, yymsp[-3].minor.yy0, NULL, yymsp[-2].minor.yy115, yymsp[0].minor.yy115, status->scanner_state);
}
// 4755 "parser.c"
        break;
      case 195:
// 1815 "parser.lemon"
{
	yygotominor.yy115 = yymsp[-1].minor.yy115;
  yy_destructor(57,&yymsp[-2].minor);
  yy_destructor(58,&yymsp[0].minor);
}
// 4764 "parser.c"
        break;
      case 196:
// 1820 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_let_assignment("incr", NULL, yymsp[-3].minor.yy0, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(92,&yymsp[0].minor);
}
// 4773 "parser.c"
        break;
      case 197:
// 1825 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_let_assignment("decr", NULL, yymsp[-3].minor.yy0, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(93,&yymsp[0].minor);
}
// 4782 "parser.c"
        break;
      case 198:
// 1830 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_let_assignment("incr", NULL, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(92,&yymsp[0].minor);
}
// 4790 "parser.c"
        break;
      case 199:
// 1835 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_let_assignment("decr", NULL, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(93,&yymsp[0].minor);
}
// 4798 "parser.c"
        break;
      case 201:
// 1843 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_echo_statement(yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(94,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 4807 "parser.c"
        break;
      case 204:
// 1855 "parser.lemon"
{
	yygotominor.yy115 = yymsp[0].minor.yy115;;
}
// 4814 "parser.c"
        break;
      case 205:
// 1860 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_mcall_statement(yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(39,&yymsp[0].minor);
}
// 4822 "parser.c"
        break;
      case 206:
// 1865 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_fcall_statement(yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(39,&yymsp[0].minor);
}
// 4830 "parser.c"
        break;
      case 207:
// 1870 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_scall_statement(yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(39,&yymsp[0].minor);
}
// 4838 "parser.c"
        break;
      case 208:
// 1875 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_fetch_statement(yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(39,&yymsp[0].minor);
}
// 4846 "parser.c"
        break;
      case 209:
// 1880 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_return_statement(yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(95,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 4855 "parser.c"
        break;
      case 210:
// 1885 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_return_statement(NULL, status->scanner_state);
  yy_destructor(95,&yymsp[-1].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 4864 "parser.c"
        break;
      case 211:
// 1890 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(96,&yymsp[-4].minor);
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 4878 "parser.c"
        break;
      case 212:
// 1899 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state),
		yymsp[-2].minor.yy115,
		status->scanner_state
	);
  yy_destructor(96,&yymsp[-5].minor);
  yy_destructor(57,&yymsp[-3].minor);
  yy_destructor(58,&yymsp[-1].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 4893 "parser.c"
        break;
      case 213:
// 1908 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_throw_exception(yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(97,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 4902 "parser.c"
        break;
      case 214:
// 1912 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_declare_statement(XX_T_TYPE_INTEGER, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(59,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 4911 "parser.c"
        break;
      case 215:
// 1916 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_declare_statement(XX_T_TYPE_UINTEGER, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(60,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 4920 "parser.c"
        break;
      case 216:
// 1920 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_declare_statement(XX_T_TYPE_CHAR, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(63,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 4929 "parser.c"
        break;
      case 217:
// 1924 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_declare_statement(XX_T_TYPE_UCHAR, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(64,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 4938 "parser.c"
        break;
      case 218:
// 1928 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_declare_statement(XX_T_TYPE_LONG, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(61,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 4947 "parser.c"
        break;
      case 219:
// 1932 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_declare_statement(XX_T_TYPE_ULONG, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(62,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 4956 "parser.c"
        break;
      case 220:
// 1936 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_declare_statement(XX_T_TYPE_DOUBLE, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(65,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 4965 "parser.c"
        break;
      case 221:
// 1940 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_declare_statement(XX_T_TYPE_STRING, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(67,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 4974 "parser.c"
        break;
      case 222:
// 1944 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_declare_statement(XX_T_TYPE_BOOL, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(66,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 4983 "parser.c"
        break;
      case 223:
// 1948 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_declare_statement(XX_T_TYPE_VAR, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(69,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 4992 "parser.c"
        break;
      case 226:
// 1960 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_declare_variable(yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 4999 "parser.c"
        break;
      case 227:
// 1964 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_declare_variable(yymsp[-2].minor.yy0, yymsp[0].minor.yy115, status->scanner_state);
  yy_destructor(49,&yymsp[-1].minor);
}
// 5007 "parser.c"
        break;
      case 229:
// 1972 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("not", yymsp[0].minor.yy115, NULL, NULL, status->scanner_state);
  yy_destructor(35,&yymsp[-1].minor);
}
// 5015 "parser.c"
        break;
      case 230:
// 1976 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("isset", yymsp[0].minor.yy115, NULL, NULL, status->scanner_state);
  yy_destructor(31,&yymsp[-1].minor);
}
// 5023 "parser.c"
        break;
      case 231:
// 1980 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("require", yymsp[0].minor.yy115, NULL, NULL, status->scanner_state);
  yy_destructor(7,&yymsp[-1].minor);
}
// 5031 "parser.c"
        break;
      case 232:
// 1984 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("clone", yymsp[0].minor.yy115, NULL, NULL, status->scanner_state);
  yy_destructor(10,&yymsp[-1].minor);
}
// 5039 "parser.c"
        break;
      case 233:
// 1988 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("empty", yymsp[0].minor.yy115, NULL, NULL, status->scanner_state);
  yy_destructor(11,&yymsp[-1].minor);
}
// 5047 "parser.c"
        break;
      case 234:
// 1992 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("likely", yymsp[0].minor.yy115, NULL, NULL, status->scanner_state);
  yy_destructor(8,&yymsp[-1].minor);
}
// 5055 "parser.c"
        break;
      case 235:
// 1996 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("unlikely", yymsp[0].minor.yy115, NULL, NULL, status->scanner_state);
  yy_destructor(9,&yymsp[-1].minor);
}
// 5063 "parser.c"
        break;
      case 236:
// 2000 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("equals", yymsp[-2].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(17,&yymsp[-1].minor);
}
// 5071 "parser.c"
        break;
      case 237:
// 2004 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("not-equals", yymsp[-2].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(24,&yymsp[-1].minor);
}
// 5079 "parser.c"
        break;
      case 238:
// 2008 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("identical", yymsp[-2].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(18,&yymsp[-1].minor);
}
// 5087 "parser.c"
        break;
      case 239:
// 2012 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("not-identical", yymsp[-2].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(23,&yymsp[-1].minor);
}
// 5095 "parser.c"
        break;
      case 240:
// 2016 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("less", yymsp[-2].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(19,&yymsp[-1].minor);
}
// 5103 "parser.c"
        break;
      case 241:
// 2020 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("greater", yymsp[-2].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(20,&yymsp[-1].minor);
}
// 5111 "parser.c"
        break;
      case 242:
// 2024 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("less-equal", yymsp[-2].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(21,&yymsp[-1].minor);
}
// 5119 "parser.c"
        break;
      case 243:
// 2028 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("greater-equal", yymsp[-2].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(22,&yymsp[-1].minor);
}
// 5127 "parser.c"
        break;
      case 244:
// 2032 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("list", yymsp[-1].minor.yy115, NULL, NULL, status->scanner_state);
  yy_destructor(53,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5136 "parser.c"
        break;
      case 245:
// 2036 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("cast", yymsp[-2].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(53,&yymsp[-3].minor);
  yy_destructor(36,&yymsp[-1].minor);
}
// 5145 "parser.c"
        break;
      case 246:
// 2040 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("type-hint", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(19,&yymsp[-3].minor);
  yy_destructor(20,&yymsp[-1].minor);
}
// 5154 "parser.c"
        break;
      case 247:
      case 322:
// 2044 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("property-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(54,&yymsp[-1].minor);
}
// 5163 "parser.c"
        break;
      case 248:
      case 323:
// 2048 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("property-dynamic-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(54,&yymsp[-3].minor);
  yy_destructor(46,&yymsp[-2].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 5174 "parser.c"
        break;
      case 249:
      case 324:
// 2052 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("property-string-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_STRING, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(54,&yymsp[-3].minor);
  yy_destructor(46,&yymsp[-2].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 5185 "parser.c"
        break;
      case 250:
// 2056 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("static-property-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(91,&yymsp[-1].minor);
}
// 5193 "parser.c"
        break;
      case 251:
      case 319:
// 2060 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("static-constant-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(91,&yymsp[-1].minor);
}
// 5202 "parser.c"
        break;
      case 252:
      case 321:
// 2065 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), yymsp[-1].minor.yy115, NULL, status->scanner_state);
  yy_destructor(57,&yymsp[-2].minor);
  yy_destructor(58,&yymsp[0].minor);
}
// 5212 "parser.c"
        break;
      case 253:
// 2070 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("add", yymsp[-2].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(25,&yymsp[-1].minor);
}
// 5220 "parser.c"
        break;
      case 254:
// 2075 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("sub", yymsp[-2].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(26,&yymsp[-1].minor);
}
// 5228 "parser.c"
        break;
      case 255:
// 2080 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("mul", yymsp[-2].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(28,&yymsp[-1].minor);
}
// 5236 "parser.c"
        break;
      case 256:
// 2085 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("div", yymsp[-2].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(29,&yymsp[-1].minor);
}
// 5244 "parser.c"
        break;
      case 257:
// 2090 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("mod", yymsp[-2].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(30,&yymsp[-1].minor);
}
// 5252 "parser.c"
        break;
      case 258:
// 2095 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("concat", yymsp[-2].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(27,&yymsp[-1].minor);
}
// 5260 "parser.c"
        break;
      case 259:
// 2100 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("and", yymsp[-2].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(13,&yymsp[-1].minor);
}
// 5268 "parser.c"
        break;
      case 260:
// 2105 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("or", yymsp[-2].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(14,&yymsp[-1].minor);
}
// 5276 "parser.c"
        break;
      case 261:
// 2110 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("instanceof", yymsp[-2].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(12,&yymsp[-1].minor);
}
// 5284 "parser.c"
        break;
      case 262:
// 2115 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("fetch", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(32,&yymsp[-3].minor);
  yy_destructor(6,&yymsp[-1].minor);
}
// 5293 "parser.c"
        break;
      case 264:
// 2125 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("typeof", yymsp[0].minor.yy115, NULL, NULL, status->scanner_state);
  yy_destructor(33,&yymsp[-1].minor);
}
// 5301 "parser.c"
        break;
      case 266:
      case 310:
      case 312:
// 2135 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_literal(XX_T_INTEGER, yymsp[0].minor.yy0, status->scanner_state);
}
// 5310 "parser.c"
        break;
      case 267:
      case 309:
      case 314:
// 2140 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_literal(XX_T_STRING, yymsp[0].minor.yy0, status->scanner_state);
}
// 5319 "parser.c"
        break;
      case 268:
      case 313:
// 2145 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_literal(XX_T_CHAR, yymsp[0].minor.yy0, status->scanner_state);
}
// 5327 "parser.c"
        break;
      case 269:
      case 315:
// 2150 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_literal(XX_T_DOUBLE, yymsp[0].minor.yy0, status->scanner_state);
}
// 5335 "parser.c"
        break;
      case 270:
      case 316:
// 2155 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_literal(XX_T_NULL, NULL, status->scanner_state);
  yy_destructor(101,&yymsp[0].minor);
}
// 5344 "parser.c"
        break;
      case 271:
      case 318:
// 2160 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_literal(XX_T_TRUE, NULL, status->scanner_state);
  yy_destructor(102,&yymsp[0].minor);
}
// 5353 "parser.c"
        break;
      case 272:
      case 317:
// 2165 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_literal(XX_T_FALSE, NULL, status->scanner_state);
  yy_destructor(103,&yymsp[0].minor);
}
// 5362 "parser.c"
        break;
      case 273:
      case 320:
// 2170 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_literal(XX_T_CONSTANT, yymsp[0].minor.yy0, status->scanner_state);
}
// 5370 "parser.c"
        break;
      case 274:
// 2175 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("empty-array", NULL, NULL, NULL, status->scanner_state);
  yy_destructor(57,&yymsp[-1].minor);
  yy_destructor(58,&yymsp[0].minor);
}
// 5379 "parser.c"
        break;
      case 275:
// 2180 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("array", yymsp[-1].minor.yy115, NULL, NULL, status->scanner_state);
  yy_destructor(57,&yymsp[-2].minor);
  yy_destructor(58,&yymsp[0].minor);
}
// 5388 "parser.c"
        break;
      case 276:
// 2185 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_new_instance(0, yymsp[0].minor.yy0, NULL, status->scanner_state);
  yy_destructor(34,&yymsp[-1].minor);
}
// 5396 "parser.c"
        break;
      case 277:
// 2190 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_new_instance(0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(34,&yymsp[-3].minor);
  yy_destructor(53,&yymsp[-1].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5406 "parser.c"
        break;
      case 278:
// 2195 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_new_instance(0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(34,&yymsp[-4].minor);
  yy_destructor(53,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5416 "parser.c"
        break;
      case 279:
// 2200 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_new_instance(1, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(34,&yymsp[-3].minor);
  yy_destructor(46,&yymsp[-2].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 5426 "parser.c"
        break;
      case 280:
// 2205 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_new_instance(1, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(34,&yymsp[-5].minor);
  yy_destructor(46,&yymsp[-4].minor);
  yy_destructor(47,&yymsp[-2].minor);
  yy_destructor(53,&yymsp[-1].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5438 "parser.c"
        break;
      case 281:
// 2210 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_new_instance(1, yymsp[-4].minor.yy0, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(34,&yymsp[-6].minor);
  yy_destructor(46,&yymsp[-5].minor);
  yy_destructor(47,&yymsp[-3].minor);
  yy_destructor(53,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5450 "parser.c"
        break;
      case 282:
// 2215 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_fcall(1, yymsp[-3].minor.yy0, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(53,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5459 "parser.c"
        break;
      case 283:
// 2220 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_fcall(1, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(53,&yymsp[-1].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5468 "parser.c"
        break;
      case 284:
// 2225 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_fcall(2, yymsp[-4].minor.yy0, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(46,&yymsp[-5].minor);
  yy_destructor(47,&yymsp[-3].minor);
  yy_destructor(53,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5479 "parser.c"
        break;
      case 285:
// 2230 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_fcall(2, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(46,&yymsp[-4].minor);
  yy_destructor(47,&yymsp[-2].minor);
  yy_destructor(53,&yymsp[-1].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5490 "parser.c"
        break;
      case 286:
// 2235 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_scall(0, yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(91,&yymsp[-4].minor);
  yy_destructor(53,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5500 "parser.c"
        break;
      case 287:
// 2240 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_scall(0, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(91,&yymsp[-3].minor);
  yy_destructor(53,&yymsp[-1].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5510 "parser.c"
        break;
      case 288:
// 2245 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_scall(1, yymsp[-6].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(46,&yymsp[-7].minor);
  yy_destructor(47,&yymsp[-5].minor);
  yy_destructor(91,&yymsp[-4].minor);
  yy_destructor(53,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5522 "parser.c"
        break;
      case 289:
// 2250 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_scall(1, yymsp[-5].minor.yy0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(46,&yymsp[-6].minor);
  yy_destructor(47,&yymsp[-4].minor);
  yy_destructor(91,&yymsp[-3].minor);
  yy_destructor(53,&yymsp[-1].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5534 "parser.c"
        break;
      case 290:
// 2255 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_scall(1, yymsp[-8].minor.yy0, yymsp[-4].minor.yy0, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(46,&yymsp[-9].minor);
  yy_destructor(47,&yymsp[-7].minor);
  yy_destructor(91,&yymsp[-6].minor);
  yy_destructor(46,&yymsp[-5].minor);
  yy_destructor(47,&yymsp[-3].minor);
  yy_destructor(53,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5548 "parser.c"
        break;
      case 291:
// 2260 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_scall(1, yymsp[-7].minor.yy0, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(46,&yymsp[-8].minor);
  yy_destructor(47,&yymsp[-6].minor);
  yy_destructor(91,&yymsp[-5].minor);
  yy_destructor(46,&yymsp[-4].minor);
  yy_destructor(47,&yymsp[-2].minor);
  yy_destructor(53,&yymsp[-1].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5562 "parser.c"
        break;
      case 292:
// 2265 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_mcall(1, yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(54,&yymsp[-4].minor);
  yy_destructor(53,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5572 "parser.c"
        break;
      case 293:
// 2270 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_mcall(1, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(54,&yymsp[-3].minor);
  yy_destructor(53,&yymsp[-1].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5582 "parser.c"
        break;
      case 294:
// 2275 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_mcall(2, yymsp[-7].minor.yy0, yymsp[-4].minor.yy0, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(54,&yymsp[-6].minor);
  yy_destructor(46,&yymsp[-5].minor);
  yy_destructor(47,&yymsp[-3].minor);
  yy_destructor(53,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5594 "parser.c"
        break;
      case 295:
// 2280 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_mcall(2, yymsp[-6].minor.yy0, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(54,&yymsp[-5].minor);
  yy_destructor(46,&yymsp[-4].minor);
  yy_destructor(47,&yymsp[-2].minor);
  yy_destructor(53,&yymsp[-1].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5606 "parser.c"
        break;
      case 296:
// 2285 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_mcall(3, yymsp[-7].minor.yy0, yymsp[-4].minor.yy0, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(54,&yymsp[-6].minor);
  yy_destructor(46,&yymsp[-5].minor);
  yy_destructor(47,&yymsp[-3].minor);
  yy_destructor(53,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5618 "parser.c"
        break;
      case 297:
// 2290 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_mcall(3, yymsp[-6].minor.yy0, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(54,&yymsp[-5].minor);
  yy_destructor(46,&yymsp[-4].minor);
  yy_destructor(47,&yymsp[-2].minor);
  yy_destructor(53,&yymsp[-1].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5630 "parser.c"
        break;
      case 306:
// 2329 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_array_item(yymsp[-2].minor.yy115, yymsp[0].minor.yy115, status->scanner_state);
  yy_destructor(76,&yymsp[-1].minor);
}
// 5638 "parser.c"
        break;
      case 307:
// 2333 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_array_item(NULL, yymsp[0].minor.yy115, status->scanner_state);
}
// 5645 "parser.c"
        break;
      case 326:
// 2413 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_comment(yymsp[0].minor.yy0, status->scanner_state);
}
// 5652 "parser.c"
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

// 5728 "parser.c"
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
