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
#define YYNSTATE 689
#define YYNRULE 323
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
 /*     0 */   214,  223,  226,  217,  220,   60,  605,   31,   55,   34,
 /*    10 */    36,   37,  258, 1013,    1,    2,  688,    4,    5,    6,
 /*    20 */     7,  512,  489,  420,  210,  267,  272,  288,  207,  131,
 /*    30 */   394,  262,  133,  657,  663,  662,  646,  523,  407,  295,
 /*    40 */     9,  656,  612,  669,  274,  667,  252,  118,   78,   86,
 /*    50 */   282,   64,   44,   45,   46,   47,   48,   49,   50,   51,
 /*    60 */    52,   53,   54,  659,  624,  646,  395,  396,  397,  398,
 /*    70 */   399,  612,  267,  416,  417,  597,  118,  307,  517,  166,
 /*    80 */   453,   78,   86,  276,   92,  312,  295,  596,  527,  525,
 /*    90 */   255,  275,  277,  278,  279,  280,  281,  389,  449,  481,
 /*   100 */   490,  499,  502,  493,  496,  505,  511,  508,   61,  514,
 /*   110 */   167,  169,  171,  452,  180,  375,  267,  253,  191,  195,
 /*   120 */   200,  381,  517,  166,  388,  302,  304,  303,  271,  183,
 /*   130 */   295,  533,  527,  638,  450,  465,  469,  478,    3,    4,
 /*   140 */     5,    6,    7,  481,  490,  499,  502,  493,  496,  505,
 /*   150 */   511,  508,  394,  514,  167,  169,  171,  187,  180,  552,
 /*   160 */   431,   19,  191,  195,  200,  381,   22,  377,  388,  267,
 /*   170 */   238,  240,  242,  119,  637,  517,  166,  118,  450,  465,
 /*   180 */   469,  478,   63,  295,  546,  527,   93,  651,  395,  396,
 /*   190 */   397,  398,  399,  679,  634,  667,  481,  490,  499,  502,
 /*   200 */   493,  496,  505,  511,  508,  685,  514,  167,  169,  171,
 /*   210 */   129,  180,  570,  267,   13,  191,  195,  200,  381,  517,
 /*   220 */   166,  388,  354,   33,   34,   36,   37,  295,  550,  527,
 /*   230 */   664,  450,  465,  469,  478,  482,  489,  184,  555,  640,
 /*   240 */   481,  490,  499,  502,  493,  496,  505,  511,  508,  641,
 /*   250 */   514,  167,  169,  171,  658,  180,  661,  662,  646,  191,
 /*   260 */   195,  200,  381,  611,  612,  388,  267,  503,  489,  118,
 /*   270 */   129,  419,  517,  166,  360,  450,  465,  469,  478,  455,
 /*   280 */   295,  540,  527,  138,  404,  451,  456,  302,  304,  303,
 /*   290 */   271,  582,  418,  481,  490,  499,  502,  493,  496,  505,
 /*   300 */   511,  508,  118,  514,  167,  169,  171,  127,  180,  667,
 /*   310 */   267,  332,  191,  195,  200,  381,  517,  166,  388,  302,
 /*   320 */   304,  303,  271,  423,  295,  557,  527,  571,  450,  465,
 /*   330 */   469,  478,  484,  334,  520,  575,  486,  481,  490,  499,
 /*   340 */   502,  493,  496,  505,  511,  508,  394,  514,  167,  169,
 /*   350 */   171,   63,  180,  610,  534,  136,  191,  195,  200,  381,
 /*   360 */   611,  612,  388,  267,  684,  501,  118,  500,  489,  517,
 /*   370 */   166,  518,  450,  465,  469,  478,  185,  295,  561,  527,
 /*   380 */   413,  113,  395,  396,  397,  398,  399,  488,  626,  290,
 /*   390 */   481,  490,  499,  502,  493,  496,  505,  511,  508,  118,
 /*   400 */   514,  167,  169,  171,  187,  180,  552,  267,  239,  191,
 /*   410 */   195,  200,  381,  517,  166,  388,  302,  304,  303,  271,
 /*   420 */   298,  295,  574,  527,  373,  450,  465,  469,  478,  484,
 /*   430 */   383,  484,  374,  192,  481,  490,  499,  502,  493,  496,
 /*   440 */   505,  511,  508,  294,  514,  167,  169,  171,  294,  180,
 /*   450 */    22,  491,  489,  191,  195,  200,  381,   21,  306,  388,
 /*   460 */   267,  118,  492,  123,  510,  668,  517,  166,  129,  450,
 /*   470 */   465,  469,  478,  358,  295,  562,  527,  521,  317,  234,
 /*   480 */   236,  244,  238,  240,  242,  522,   62,  481,  490,  499,
 /*   490 */   502,  493,  496,  505,  511,  508,  343,  514,  167,  169,
 /*   500 */   171,   59,  180,  412,  267,  369,  191,  195,  200,  381,
 /*   510 */   517,  166,  388,  302,  304,  303,  271,  615,  295,  175,
 /*   520 */   527,  484,  450,  465,  469,  478,  509,  489,  484,  294,
 /*   530 */    56,  481,  490,  499,  502,  493,  496,  505,  511,  508,
 /*   540 */   294,  514,  167,  169,  171,   27,  180,  111,  342,  136,
 /*   550 */   191,  195,  200,  381,  498,  135,  388,  267,  631,  326,
 /*   560 */   118,  516,  108,  517,  166,  634,  450,  465,  469,  478,
 /*   570 */   293,  295,  179,  527,  109,  599,   31,   55,   34,   36,
 /*   580 */    37,  497,  489,  603,  481,  490,  499,  502,  493,  496,
 /*   590 */   505,  511,  508,  294,  514,  167,  169,  171,  616,  180,
 /*   600 */    40,  267,  379,  191,  195,  200,  381,  517,  166,  388,
 /*   610 */   302,  304,  303,  271,  294,  295,  203,  527,  484,  450,
 /*   620 */   465,  469,  478,  310,  515,  489,  636,  643,  481,  490,
 /*   630 */   499,  502,  493,  496,  505,  511,  508,   41,  514,  167,
 /*   640 */   169,  171,   63,  180,  364,  628,  409,  191,  195,  200,
 /*   650 */   381,  507,  634,  388,  267,  629,  617,  675,  618,  678,
 /*   660 */   517,  166,  129,  450,  465,  469,  478,  402,  295,  545,
 /*   670 */   527,  401,   58,  506,  489,  302,  304,  303,  271,  103,
 /*   680 */   117,  481,  490,  499,  502,  493,  496,  505,  511,  508,
 /*   690 */   118,  514,  167,  169,  171,  114,  180,  620,  267,  241,
 /*   700 */   191,  195,  200,  381,  517,  166,  388,  302,  304,  303,
 /*   710 */   271,   63,  295,  199,  527,  622,  450,  465,  469,  478,
 /*   720 */   621,  286,   25,  294,  623,  481,  490,  499,  502,  493,
 /*   730 */   496,  505,  511,  508,  330,  514,  167,  169,  171,  640,
 /*   740 */   180,  598,  125,  484,  191,  195,  200,  381,  635,  641,
 /*   750 */   388,  267,  996,  328,   14,  494,  489,  517,  166,   17,
 /*   760 */   450,  465,  469,  478,  402,  295,  568,  527,  406,  289,
 /*   770 */   995,  405,  302,  304,  303,  271,  483,  320,  481,  490,
 /*   780 */   499,  502,  493,  496,  505,  511,  508,  484,  514,  167,
 /*   790 */   169,  171,   89,  180,  487,  267,  218,  191,  195,  200,
 /*   800 */   381,  517,  166,  388,  302,  304,  303,  271,  294,  295,
 /*   810 */   594,  527,  484,  450,  465,  469,  478,  644,  372,  391,
 /*   820 */   495,  213,  481,  490,  499,  502,  493,  496,  505,  511,
 /*   830 */   508,  630,  514,  167,  169,  171,  632,  180,  300,  648,
 /*   840 */   633,  191,  195,  200,  381,  504,  634,  388,  267,  649,
 /*   850 */    82,  197,  390,   79,  517,  166,  484,  450,  465,  469,
 /*   860 */   478,  415,  295,  606,  527,  264,  411,  287,  110,  302,
 /*   870 */   304,  303,  271,  353,  735,  481,  490,  499,  502,  493,
 /*   880 */   496,  505,  511,  508,  294,  514,  167,  169,  171,  513,
 /*   890 */   180,   12,  267,  475,  191,  195,  200,  381,  517,  166,
 /*   900 */   388,  302,  304,  303,  271,  294,  295,  608,  527,  587,
 /*   910 */   450,  465,  469,  478,  336,  471,  736,  591,  474,  481,
 /*   920 */   490,  499,  502,  493,  496,  505,  511,  508,  141,  514,
 /*   930 */   167,  169,  171,  346,  180,  351,  563,  639,  191,  195,
 /*   940 */   200,  381,  666,  126,  388,  267,  129,  642,  201,  400,
 /*   950 */   645,  517,  166,  392,  450,  465,  469,  478,  402,  295,
 /*   960 */   387,  527,  410,  393,  654,  204,  302,  304,  303,  271,
 /*   970 */   128,  650,  481,  490,  499,  502,  493,  496,  505,  511,
 /*   980 */   508,  653,  514,  167,  169,  171,  355,  180,  206,  267,
 /*   990 */   208,  191,  195,  200,  381,  517,  166,  388,  302,  304,
 /*  1000 */   303,  271,  655,  295,  566,  527,  652,  450,  465,  469,
 /*  1010 */   478,  356,   85,   88,  382,   35,  481,  490,  499,  502,
 /*  1020 */   493,  496,  505,  511,  508,  665,  514,  167,  169,  171,
 /*  1030 */   378,  180,  361,  670,  362,  191,  195,  200,  381,  485,
 /*  1040 */   376,  388,  267,  672,   32,  673,  105,  674,  517,  166,
 /*  1050 */   106,  450,  465,  469,  478,  402,  295,  590,  527,  414,
 /*  1060 */   212,  211,  676,  302,  304,  303,  271,  129,  677,  481,
 /*  1070 */   490,  499,  502,  493,  496,  505,  511,  508,   87,  514,
 /*  1080 */   167,  169,  171,  132,  180,  680,  267,  249,  191,  195,
 /*  1090 */   200,  381,  517,  166,  388,  302,  304,  303,  271,   66,
 /*  1100 */   295,  142,  527,  170,  450,  465,  469,  478,  265,   17,
 /*  1110 */   470,   39,  547,  481,  490,  499,  502,  493,  496,  505,
 /*  1120 */   511,  508,  464,  514,  167,  169,  171,  472,  180,  682,
 /*  1130 */   549,   80,  191,  195,  200,  381,  173,  268,  388,  267,
 /*  1140 */   473,  462,  553,  260,  460,  517,  166,  308,  450,  465,
 /*  1150 */   469,  478,  259,  295,  602,  527,  604,  458,   31,   55,
 /*  1160 */    34,   36,   37,  269,   24,  176,  481,  490,  499,  502,
 /*  1170 */   493,  496,  505,  511,  508,  559,  514,  167,  169,  171,
 /*  1180 */   177,  180,  558,  267,  370,  191,  195,  200,  381,  517,
 /*  1190 */   166,  388,  302,  304,  303,  271,   43,  295,  588,  527,
 /*  1200 */   116,  450,  465,  469,  478,  256,  536,   40,  313,  535,
 /*  1210 */   481,  490,  499,  502,  493,  496,  505,  511,  508,   67,
 /*  1220 */   514,  167,  169,  171,  565,  180,  314,  445,  477,  191,
 /*  1230 */   195,  200,  381,  315,  270,  388,  267,   29,  441,  444,
 /*  1240 */    16,  168,  517,  166,   81,  450,  465,  469,  478,  402,
 /*  1250 */   295,  572,  527,  422,   91,  297,   10,  302,  304,  303,
 /*  1260 */   271,  296,  526,  481,  490,  499,  502,  493,  496,  505,
 /*  1270 */   511,  508,  577,  514,  167,  169,  171,  437,  180,  182,
 /*  1280 */   267,  261,  191,  195,  200,  381,  517,  166,  388,  302,
 /*  1290 */   304,  303,  271,  436,  295,  556,  527,  121,  450,  465,
 /*  1300 */   469,  478,  321,  433,  689,  584,  585,  481,  490,  499,
 /*  1310 */   502,  493,  496,  505,  511,  508,  322,  514,  167,  169,
 /*  1320 */   171,   15,  180,  524,  323,  683,  191,  195,  200,  381,
 /*  1330 */    77,  429,  388,  267,  428,  687,  593,  186,  122,  517,
 /*  1340 */   166,   83,  450,  465,  469,  478,  402,  295,  551,  527,
 /*  1350 */   426,  425,  424,  124,  302,  304,  303,  271,   42,  686,
 /*  1360 */   481,  490,  499,  502,  493,  496,  505,  511,  508,  188,
 /*  1370 */   514,  167,  169,  171,  421,  180,  519,  267,  237,  191,
 /*  1380 */   195,  200,  381,  517,  166,  388,  302,  304,  303,  271,
 /*  1390 */    84,  295,  580,  527,  189,  450,  465,  469,  478,  139,
 /*  1400 */    76,  510,  510,  510,  481,  490,  499,  502,  493,  496,
 /*  1410 */   505,  511,  508,  510,  514,  167,  169,  171,  510,  180,
 /*  1420 */   510,  510,  510,  191,  195,  200,  381,  510,  510,  388,
 /*  1430 */   267,  510,  510,  510,  510,  510,  517,  166,  510,  450,
 /*  1440 */   465,  469,  478,  510,  295,  194,  527,  592,  510,   31,
 /*  1450 */    55,   34,   36,   37,  510,  510,  510,  481,  490,  499,
 /*  1460 */   502,  493,  496,  505,  511,  508,  510,  514,  167,  169,
 /*  1470 */   171,  510,  180,  510,  267,  247,  191,  195,  200,  381,
 /*  1480 */   517,  166,  388,  302,  304,  303,  271,  510,  295,  600,
 /*  1490 */   527,  510,  450,  465,  469,  478,  510,  510,  510,  510,
 /*  1500 */   510,  481,  490,  499,  502,  493,  496,  505,  511,  508,
 /*  1510 */   510,  514,  167,  169,  171,  510,  180,  510,  510,  510,
 /*  1520 */   191,  195,  200,  381,  510,  510,  388,  267,  510,  510,
 /*  1530 */   510,  510,  510,  517,  166,  510,  450,  465,  469,  478,
 /*  1540 */   402,  295,  578,  527,  430,  510,  510,  510,  302,  304,
 /*  1550 */   303,  271,  510,  510,  481,  490,  499,  502,  493,  496,
 /*  1560 */   505,  511,  508,  510,  514,  167,  169,  171,  510,  180,
 /*  1570 */   510,  267,  542,  191,  195,  200,  381,  517,  166,  388,
 /*  1580 */   302,  304,  303,  271,  510,  295,  144,  527,  510,  450,
 /*  1590 */   465,  469,  478,  510,  510,  510,  510,  510,  481,  490,
 /*  1600 */   499,  502,  493,  496,  505,  511,  508,  510,  514,  167,
 /*  1610 */   169,  171,  510,  180,  510,  510,  510,  191,  195,  200,
 /*  1620 */   381,  510,  510,  388,  267,   94,   95,   97,   96,   98,
 /*  1630 */   517,  166,  510,  450,  465,  469,  478,  510,  295,    8,
 /*  1640 */   527,  510,   11,  510,  120,  510,  671,  681,  510,  510,
 /*  1650 */   527,  481,  490,  499,  502,  493,  496,  505,  511,  508,
 /*  1660 */   510,  514,  167,  169,  171,  510,  180,  510,  100,  101,
 /*  1670 */   191,  195,  200,  381,  164,  510,  388,  510,  576,   99,
 /*  1680 */    31,   55,   34,   36,   37,  510,  450,  465,  469,  478,
 /*  1690 */   510,  510,  510,  510,  174,  510,   94,   95,   97,   96,
 /*  1700 */    98,  510,  510,  528,  146,  147,  148,  149,  150,  151,
 /*  1710 */   152,  153,  154,  155,  156,  157,  158,  159,  160,  161,
 /*  1720 */   162,  163,  165,  164,  564,  510,   31,   55,   34,   36,
 /*  1730 */    37,  510,  510,  627,  457,  459,  461,  463,  510,  100,
 /*  1740 */   101,  334,  263,  601,  510,  331,  510,  583,  510,  510,
 /*  1750 */    99,  510,  528,  146,  147,  148,  149,  150,  151,  152,
 /*  1760 */   153,  154,  155,  156,  157,  158,  159,  160,  161,  162,
 /*  1770 */   163,  165,  164,  609,  134,  624,  646,  510,  510,  285,
 /*  1780 */   510,  656,  612,  457,  459,  461,  463,  118,  510,  510,
 /*  1790 */   510,  510,  595,  510,   94,   95,   97,   96,   98,  510,
 /*  1800 */   510,  528,  146,  147,  148,  149,  150,  151,  152,  153,
 /*  1810 */   154,  155,  156,  157,  158,  159,  160,  161,  162,  163,
 /*  1820 */   165,  164,  115,  510,   31,   55,   34,   36,   37,  510,
 /*  1830 */   510,  647,  457,  459,  461,  463,  510,  100,  101,  510,
 /*  1840 */   510,  386,  510,  510,  510,  137,  510,  510,   99,  510,
 /*  1850 */   528,  146,  147,  148,  149,  150,  151,  152,  153,  154,
 /*  1860 */   155,  156,  157,  158,  159,  160,  161,  162,  163,  165,
 /*  1870 */   164,  510,  510,  510,  510,  510,  510,  283,  510,  510,
 /*  1880 */   510,  457,  459,  461,  463,  302,  304,  303,  271,  510,
 /*  1890 */   589,  510,  341,  510,  510,  349,  345,  348,  510,  528,
 /*  1900 */   146,  147,  148,  149,  150,  151,  152,  153,  154,  155,
 /*  1910 */   156,  157,  158,  159,  160,  161,  162,  163,  165,  164,
 /*  1920 */   510,  510,  510,  205,  510,  510,  380,  510,  510,  510,
 /*  1930 */   457,  459,  461,  463,  302,  304,  303,  271,  510,  573,
 /*  1940 */   510,   94,   95,   97,   96,   98,  510,  510,  528,  146,
 /*  1950 */   147,  148,  149,  150,  151,  152,  153,  154,  155,  156,
 /*  1960 */   157,  158,  159,  160,  161,  162,  163,  165,  164,   28,
 /*  1970 */   510,   31,   55,   34,   36,   37,  510,  510,  510,  457,
 /*  1980 */   459,  461,  463,  402,  100,  101,  510,  434,  579,  510,
 /*  1990 */   614,  302,  304,  303,  271,   99,  510,  528,  146,  147,
 /*  2000 */   148,  149,  150,  151,  152,  153,  154,  155,  156,  157,
 /*  2010 */   158,  159,  160,  161,  162,  163,  165,  164,  510,  510,
 /*  2020 */   510,  510,  510,  510,  283,  510,  510,  510,  457,  459,
 /*  2030 */   461,  463,  302,  304,  303,  271,  510,  532,  510,   94,
 /*  2040 */    95,   97,   96,   98,  347,  510,  528,  146,  147,  148,
 /*  2050 */   149,  150,  151,  152,  153,  154,  155,  156,  157,  158,
 /*  2060 */   159,  160,  161,  162,  163,  165,  164,  112,  510,  510,
 /*  2070 */   510,  510,  510,   78,   86,  510,   92,  457,  459,  461,
 /*  2080 */   463,  510,  100,  101,  510,  510,  202,  510,  510,  510,
 /*  2090 */   583,  510,  510,   99,  510,  528,  146,  147,  148,  149,
 /*  2100 */   150,  151,  152,  153,  154,  155,  156,  157,  158,  159,
 /*  2110 */   160,  161,  162,  163,  165,  164,  510,  510,  510,  196,
 /*  2120 */   510,  510,  380,  510,  510,  510,  457,  459,  461,  463,
 /*  2130 */   302,  304,  303,  271,  510,  554,  510,   94,   95,   97,
 /*  2140 */    96,   98,  510,  510,  528,  146,  147,  148,  149,  150,
 /*  2150 */   151,  152,  153,  154,  155,  156,  157,  158,  159,  160,
 /*  2160 */   161,  162,  163,  165,  164,   57,  510,  510,  510,  510,
 /*  2170 */   510,   78,   86,  510,   92,  457,  459,  461,  463,  510,
 /*  2180 */   100,  101,  510,  510,  198,  510,  510,  510,   23,  510,
 /*  2190 */   510,   99,  510,  528,  146,  147,  148,  149,  150,  151,
 /*  2200 */   152,  153,  154,  155,  156,  157,  158,  159,  160,  161,
 /*  2210 */   162,  163,  165,  164,  510,  510,  510,  181,  510,  510,
 /*  2220 */   380,  510,  510,  510,  457,  459,  461,  463,  302,  304,
 /*  2230 */   303,  271,  510,  560,  510,   94,   95,   97,   96,   98,
 /*  2240 */   510,  510,  528,  146,  147,  148,  149,  150,  151,  152,
 /*  2250 */   153,  154,  155,  156,  157,  158,  159,  160,  161,  162,
 /*  2260 */   163,  165,  164,  569,  510,  510,  510,  510,  510,   78,
 /*  2270 */    86,  510,   92,  457,  459,  461,  463,  510,  100,  101,
 /*  2280 */   510,  510,  539,  510,  510,  510,  137,  510,  510,   99,
 /*  2290 */   510,  528,  146,  147,  148,  149,  150,  151,  152,  153,
 /*  2300 */   154,  155,  156,  157,  158,  159,  160,  161,  162,  163,
 /*  2310 */   165,  164,  510,  510,  510,  510,  510,  510,  510,  455,
 /*  2320 */   510,  510,  457,  459,  461,  463,  454,  302,  304,  303,
 /*  2330 */   271,  193,  510,   94,   95,   97,   96,   98,  510,  510,
 /*  2340 */   528,  146,  147,  148,  149,  150,  151,  152,  153,  154,
 /*  2350 */   155,  156,  157,  158,  159,  160,  161,  162,  163,  165,
 /*  2360 */   164,  510,  510,  510,  510,  510,  510,  510,  510,  510,
 /*  2370 */   510,  457,  459,  461,  463,  510,  100,  101,  510,  510,
 /*  2380 */   178,  510,  510,  510,  104,  510,  510,   99,  510,  528,
 /*  2390 */   146,  147,  148,  149,  150,  151,  152,  153,  154,  155,
 /*  2400 */   156,  157,  158,  159,  160,  161,  162,  163,  165,  164,
 /*  2410 */   510,  510,  510,  510,  510,  510,  283,  510,  510,  510,
 /*  2420 */   457,  459,  461,  463,  302,  304,  303,  271,  510,  143,
 /*  2430 */   510,  510,  510,  510,  344,  345,  348,  510,  528,  146,
 /*  2440 */   147,  148,  149,  150,  151,  152,  153,  154,  155,  156,
 /*  2450 */   157,  158,  159,  160,  161,  162,  163,  165,  164,  510,
 /*  2460 */   510,  510,  510,  510,  510,  266,  510,  510,  510,  457,
 /*  2470 */   459,  461,  463,  302,  304,  303,  271,  510,  567,  510,
 /*  2480 */   510,  316,  305,  510,  510,  510,  510,  528,  146,  147,
 /*  2490 */   148,  149,  150,  151,  152,  153,  154,  155,  156,  157,
 /*  2500 */   158,  159,  160,  161,  162,  163,  165,  164,  510,  510,
 /*  2510 */   510,  510,  510,  510,  266,  510,  510,  510,  457,  459,
 /*  2520 */   461,  463,  302,  304,  303,  271,  510,  190,  510,  510,
 /*  2530 */   350,  305,  510,  510,  510,  510,  528,  146,  147,  148,
 /*  2540 */   149,  150,  151,  152,  153,  154,  155,  156,  157,  158,
 /*  2550 */   159,  160,  161,  162,  163,  165,  164,  510,  510,  510,
 /*  2560 */   510,  510,  510,  266,  510,  510,  510,  457,  459,  461,
 /*  2570 */   463,  302,  304,  303,  271,  510,  544,  510,  510,  327,
 /*  2580 */   305,  510,  510,  510,  510,  528,  146,  147,  148,  149,
 /*  2590 */   150,  151,  152,  153,  154,  155,  156,  157,  158,  159,
 /*  2600 */   160,  161,  162,  163,  165,  164,  510,  510,  510,  510,
 /*  2610 */   510,  510,  266,  510,  510,  510,  457,  459,  461,  463,
 /*  2620 */   302,  304,  303,  271,  510,  607,  510,  510,  292,  305,
 /*  2630 */   510,  510,  510,  510,  528,  146,  147,  148,  149,  150,
 /*  2640 */   151,  152,  153,  154,  155,  156,  157,  158,  159,  160,
 /*  2650 */   161,  162,  163,  165,  164,  510,  510,  510,  510,  510,
 /*  2660 */   510,  510,  510,  510,  510,  457,  459,  461,  463,  209,
 /*  2670 */   219,  225,  228,  230,  232,  222,  216,  234,  236,  244,
 /*  2680 */   238,  240,  242,  145,  146,  147,  148,  149,  150,  151,
 /*  2690 */   152,  153,  154,  155,  156,  157,  158,  159,  160,  161,
 /*  2700 */   162,  163,  165,  510,  510,  510,  214,  223,  226,  217,
 /*  2710 */   220,  266,  510,  510,  457,  459,  461,  463,  258,  302,
 /*  2720 */   304,  303,  271,  510,  510,  510,  510,  299,  305,  510,
 /*  2730 */   210,  267,  272,  288,  207,  311,  510,  262,   90,  334,
 /*  2740 */   263,  510,  510,  331,  510,  295,  510,  510,  510,  510,
 /*  2750 */   274,  510,  252,  510,  510,  586,  282,   65,  250,  246,
 /*  2760 */   248,  510,  994,  209,  219,  225,  228,  230,  232,  222,
 /*  2770 */   216,  234,  236,  244,  238,  240,  242,  285,   44,   45,
 /*  2780 */    46,   47,   48,   49,   50,   51,   52,   53,   54,  276,
 /*  2790 */   510,  510,  385,  510,  510,  510,  510,  275,  277,  278,
 /*  2800 */   279,  280,  281,  266,  510,  214,  223,  226,  217,  220,
 /*  2810 */   510,  302,  304,  303,  271,  368,  510,  258,  510,  309,
 /*  2820 */   305,  510,  510,  302,  304,  303,  271,  510,  510,  210,
 /*  2830 */   267,  272,  288,  207,  510,  510,  262,  266,  510,  510,
 /*  2840 */   510,  510,  510,  510,  295,  302,  304,  303,  271,  274,
 /*  2850 */   510,  252,  510,  325,  305,  282,  510,  250,  246,  248,
 /*  2860 */   510,  510,  209,  219,  225,  228,  230,  232,  222,  216,
 /*  2870 */   234,  236,  244,  238,  240,  242,  402,   38,  510,  510,
 /*  2880 */   438,  529,  510,  510,  302,  304,  303,  271,  276,  510,
 /*  2890 */    94,   95,   97,   96,   98,  510,  275,  277,  278,  279,
 /*  2900 */   280,  281,  257,  886,  214,  223,  226,  217,  220,  510,
 /*  2910 */   302,  304,  303,  271,   30,  510,  258,   44,   45,   46,
 /*  2920 */    47,   48,   49,   50,   51,   52,   53,   54,  210,  267,
 /*  2930 */   272,  288,  207,  100,  101,  262,  716,  625,  510,  619,
 /*  2940 */   510,  510,  510,  295,   99,  510,  510,  510,  274,  510,
 /*  2950 */   252,  510,  510,  510,  282,  440,  250,  246,  248,  510,
 /*  2960 */   510,  209,  219,  225,  228,  230,  232,  222,  216,  234,
 /*  2970 */   236,  244,  238,  240,  242,  510,   38,  510,  510,  510,
 /*  2980 */   254,  510,  402,  510,  510,  510,  442,  276,  510,  510,
 /*  2990 */   302,  304,  303,  271,  510,  275,  277,  278,  279,  280,
 /*  3000 */   281,  221,  510,  214,  223,  226,  217,  220,  510,  302,
 /*  3010 */   304,  303,  271,  510,  510,  258,   44,   45,   46,   47,
 /*  3020 */    48,   49,   50,   51,   52,   53,   54,  210,  267,  272,
 /*  3030 */   288,  207,  291,  510,  262,   90,  510,  510,  510,  510,
 /*  3040 */   510,  510,  295,  510,  510,  510,  510,  274,  510,  252,
 /*  3050 */   510,  510,   26,  282,   65,  250,  246,  248,   75,  510,
 /*  3060 */   209,  219,  225,  228,  230,  232,  222,  216,  234,  236,
 /*  3070 */   244,  238,  240,  242,  510,   44,   45,   46,   47,   48,
 /*  3080 */    49,   50,   51,   52,   53,   54,  276,  510,   94,   95,
 /*  3090 */    97,   96,   98,  510,  275,  277,  278,  279,  280,  281,
 /*  3100 */   510,  371,  214,  223,  226,  217,  220,  510,  510,  510,
 /*  3110 */    70,  510,  510,  510,  258,  510,  510,  510,   68,   69,
 /*  3120 */    71,   72,   74,   73,  510,  510,  210,  267,  272,  288,
 /*  3130 */   207,  100,  101,  262,  130,  625,  510,  619,  510,  394,
 /*  3140 */   510,  295,   99,  510,  403,  510,  274,  439,  252,  510,
 /*  3150 */   510,  510,  282,  408,  250,  246,  248,  510,  510,  209,
 /*  3160 */   219,  225,  228,  230,  232,  222,  216,  234,  236,  244,
 /*  3170 */   238,  240,  242,  510,  510,  395,  396,  397,  398,  399,
 /*  3180 */   510,  427,  447,  448,  510,  276,  510,  510,  531,  510,
 /*  3190 */   510,  510,  510,  275,  277,  278,  279,  280,  281,  266,
 /*  3200 */   510,  214,  223,  226,  217,  220,  510,  302,  304,  303,
 /*  3210 */   271,  510,  415,  258,  510,  335,  305,  435,  510,  510,
 /*  3220 */   302,  304,  303,  271,  510,  210,  267,  272,  288,  207,
 /*  3230 */   510,  510,  262,  468,  510,   94,   95,   97,   96,   98,
 /*  3240 */   295,  510,  510,  510,  510,  274,  510,  252,  510,  510,
 /*  3250 */   510,  282,  510,  250,  246,  248,  510,  510,  209,  219,
 /*  3260 */   225,  228,  230,  232,  222,  216,  234,  236,  244,  238,
 /*  3270 */   240,  242,  510,  510,  510,  510,  510,  510,  100,  101,
 /*  3280 */   480,  721,  660,  510,  276,  510,  510,  510,  510,   99,
 /*  3290 */   510,  510,  275,  277,  278,  279,  280,  281,  266,  510,
 /*  3300 */   214,  223,  226,  217,  220,  510,  302,  304,  303,  271,
 /*  3310 */   510,  415,  258,  510,  357,  305,  443,  510,  510,  302,
 /*  3320 */   304,  303,  271,  510,  210,  267,  272,  288,  207,  359,
 /*  3330 */   510,  262,   90,  510,  510,  510,  510,  510,  510,  295,
 /*  3340 */   510,  510,  510,  510,  274,  510,  252,  510,  510,  140,
 /*  3350 */   282,   65,  250,  246,  248,  510,  510,  209,  219,  225,
 /*  3360 */   228,  230,  232,  222,  216,  234,  236,  244,  238,  240,
 /*  3370 */   242,  510,   44,   45,   46,   47,   48,   49,   50,   51,
 /*  3380 */    52,   53,   54,  276,  510,  510,  510,  510,  510,  510,
 /*  3390 */   510,  275,  277,  278,  279,  280,  281,  266,  476,  214,
 /*  3400 */   223,  226,  217,  220,  510,  302,  304,  303,  271,  510,
 /*  3410 */   384,  258,  510,  363,  305,  510,  510,  510,  302,  304,
 /*  3420 */   303,  271,  510,  210,  267,  272,  288,  207,  352,  510,
 /*  3430 */   262,   90,  510,  510,  510,  510,  510,  510,  295,  510,
 /*  3440 */   510,  510,  510,  274,  510,  252,  510,  510,  107,  282,
 /*  3450 */    65,  250,  246,  248,  510,  510,  209,  219,  225,  228,
 /*  3460 */   230,  232,  222,  216,  234,  236,  244,  238,  240,  242,
 /*  3470 */   510,   44,   45,   46,   47,   48,   49,   50,   51,   52,
 /*  3480 */    53,   54,  276,  510,  510,  538,  510,  510,  510,  510,
 /*  3490 */   275,  277,  278,  279,  280,  281,  266,  510,  214,  223,
 /*  3500 */   226,  217,  220,  510,  302,  304,  303,  271,  510,  367,
 /*  3510 */   258,  510,  510,  319,  510,  510,  510,  302,  304,  303,
 /*  3520 */   271,  510,  210,  267,  272,  288,  207,  318,  510,  262,
 /*  3530 */    90,  510,  510,  510,  510,  510,  510,  295,  510,  510,
 /*  3540 */   510,  510,  274,  510,  252,  510,  510,  510,  282,   65,
 /*  3550 */   250,  246,  248,  510,  510,  209,  219,  225,  228,  230,
 /*  3560 */   232,  222,  216,  234,  236,  244,  238,  240,  242,  510,
 /*  3570 */    44,   45,   46,   47,   48,   49,   50,   51,   52,   53,
 /*  3580 */    54,  276,  510,  510,  543,  510,  510,  510,  510,  275,
 /*  3590 */   277,  278,  279,  280,  281,  273,  510,  214,  223,  226,
 /*  3600 */   217,  220,  510,  302,  304,  303,  271,  510,  402,  258,
 /*  3610 */   510,  510,  446,  510,  510,  510,  302,  304,  303,  271,
 /*  3620 */   510,  210,  267,  272,  288,  207,  548,  510,  262,  380,
 /*  3630 */   510,  510,  510,  510,  510,  510,  295,  302,  304,  303,
 /*  3640 */   271,  274,  510,  252,  510,  510,  510,  282,  432,  250,
 /*  3650 */   246,  248,  510,  510,  209,  219,  225,  228,  230,  232,
 /*  3660 */   222,  216,  234,  236,  244,  238,  240,  242,  510,  510,
 /*  3670 */   510,  510,  510,  510,  224,  510,  467,  510,  510,  510,
 /*  3680 */   276,  510,  302,  304,  303,  271,  510,  510,  275,  277,
 /*  3690 */   278,  279,  280,  281,  510,  510,  214,  223,  226,  217,
 /*  3700 */   220,  510,  510,  510,  172,  510,  510,  380,  258,  510,
 /*  3710 */   510,  510,  510,  510,  510,  302,  304,  303,  271,  510,
 /*  3720 */   210,  267,  272,  288,  207,  337,  510,  262,  366,   94,
 /*  3730 */    95,   97,   96,   98,  510,  295,  302,  304,  303,  271,
 /*  3740 */   274,  510,  252,  510,  510,  510,  282,  510,  250,  246,
 /*  3750 */   248,  510,  510,  209,  219,  225,  228,  230,  232,  222,
 /*  3760 */   216,  234,  236,  244,  238,  240,  242,  510,  510,  510,
 /*  3770 */   510,  510,  100,  101,  510,  720,  613,  510,  619,  276,
 /*  3780 */   510,  510,  510,   99,  510,  510,  510,  275,  277,  278,
 /*  3790 */   279,  280,  281,  243,  333,  214,  223,  226,  217,  220,
 /*  3800 */   510,  302,  304,  303,  271,  479,  510,  258,  510,  510,
 /*  3810 */   510,  510,  510,  302,  304,  303,  271,  510,  510,  210,
 /*  3820 */   267,  272,  288,  207,  324,  510,  262,  227,   94,   95,
 /*  3830 */    97,   96,   98,  510,  295,  302,  304,  303,  271,  274,
 /*  3840 */   510,  252,  510,  510,  510,  282,  510,  250,  246,  248,
 /*  3850 */   510,  510,  209,  219,  225,  228,  230,  232,  222,  216,
 /*  3860 */   234,  236,  244,  238,  240,  242,  510,  510,  510,  510,
 /*  3870 */   510,  100,  101,  510,  717,  625,  510,  619,  276,  510,
 /*  3880 */   510,  510,   99,  510,  510,  510,  275,  277,  278,  279,
 /*  3890 */   280,  281,  510,  229,  214,  223,  226,  217,  220,  510,
 /*  3900 */   510,  302,  304,  303,  271,  251,  258,  510,  510,  510,
 /*  3910 */   510,  510,  510,  302,  304,  303,  271,  510,  210,  267,
 /*  3920 */   272,  288,  207,  329,  510,  262,  231,   94,   95,   97,
 /*  3930 */    96,   98,  510,  295,  302,  304,  303,  271,  274,  510,
 /*  3940 */   252,  510,  510,  510,  282,  510,  510,  246,  248,  510,
 /*  3950 */   510,  209,  219,  225,  228,  230,  232,  222,  216,  234,
 /*  3960 */   236,  244,  238,  240,  242,  510,  510,  510,  510,  510,
 /*  3970 */   100,  101,  510,  724,  581,  510,  510,  276,  510,  510,
 /*  3980 */   510,   99,  510,  510,  510,  275,  277,  278,  279,  280,
 /*  3990 */   281,  510,  510,  214,  223,  226,  217,  220,   94,   95,
 /*  4000 */    97,   96,   98,  510,  510,  258,  510,   94,   95,   97,
 /*  4010 */    96,   98,   94,   95,   97,   96,   98,  210,  267,  272,
 /*  4020 */   288,  207,  365,  690,  262,  466,   94,   95,   97,   96,
 /*  4030 */    98,  510,  295,  302,  304,  303,  271,  274,  510,  252,
 /*  4040 */   510,  100,  101,  282,  719,  581,  510,  510,  510,  510,
 /*  4050 */   100,  101,   99,  722,  581,  100,  101,  510,  718,  581,
 /*  4060 */     8,   99,  510,   11,  510,  120,   99,  671,  681,  100,
 /*  4070 */   101,  527,   18,  102,  510,  510,  276,  510,  510,  510,
 /*  4080 */    99,  510,  510,  510,  275,  277,  278,  279,  280,  281,
 /*  4090 */   510,  233,  214,  223,  226,  217,  220,  510,  510,  302,
 /*  4100 */   304,  303,  271,  510,  258,  510,   94,   95,   97,   96,
 /*  4110 */    98,   94,   95,   97,   96,   98,  210,  267,  272,  288,
 /*  4120 */   207,  510,  510,  284,  215,  510,  510,  510,  510,  510,
 /*  4130 */   510,  295,  302,  304,  303,  271,  274,  510,  252,  510,
 /*  4140 */   510,  510,  282,  340,  510,  510,  510,  510,  510,  100,
 /*  4150 */   101,  510,  723,  581,  100,  101,  510,   20,  102,  245,
 /*  4160 */    99,  510,  510,  530,  510,   99,  510,  302,  304,  303,
 /*  4170 */   271,  302,  304,  303,  271,  339,  510,  510,  510,  510,
 /*  4180 */   510,  510,  510,  338,  277,  278,  279,  280,  281,  510,
 /*  4190 */   235,  214,  223,  226,  217,  220,  510,  537,  302,  304,
 /*  4200 */   303,  271,  510,  258,  510,  302,  304,  303,  271,  510,
 /*  4210 */   510,  510,  510,  510,  510,  210,  267,  272,  288,  207,
 /*  4220 */   301,  510,  262,  510,  510,  510,  510,  510,  510,  510,
 /*  4230 */   295,  510,  510,  510,  510,  274,  510,  252,  510,  510,
 /*  4240 */   510,  282,  510,  510,  510,  510,  510,  510,  510,  510,
 /*  4250 */   510,  510,  510,  510,  510,  510,  510,  510,  510,  510,
 /*  4260 */   510,  510,  510,  510,  510,  510,  510,  510,  510,  510,
 /*  4270 */   510,  510,  510,  510,  276,  510,  510,  510,  510,  510,
 /*  4280 */   510,  510,  275,  277,  278,  279,  280,  281,  510,  510,
 /*  4290 */   214,  223,  226,  217,  220,  510,  510,  510,  510,  510,
 /*  4300 */   510,  510,  258,  510,  510,  510,  510,  510,  510,  510,
 /*  4310 */   510,  510,  510,  510,  210,  267,  272,  288,  207,  510,
 /*  4320 */   510,  262,  510,  510,  510,  510,  510,  510,  510,  295,
 /*  4330 */   510,  510,  510,  510,  274,  510,  252,  510,  510,  510,
 /*  4340 */   282,  510,  510,  510,  510,  510,  510,  510,  510,  510,
 /*  4350 */   510,  510,  510,  510,  510,  510,  510,  510,  510,  510,
 /*  4360 */   510,  510,  510,  510,  510,  510,  541,  510,  510,  510,
 /*  4370 */   510,  510,  510,  276,  510,  510,  510,  510,  510,  510,
 /*  4380 */   510,  275,  277,  278,  279,  280,  281,  510,  510,  214,
 /*  4390 */   223,  226,  217,  220,  510,  510,  510,  510,  510,  510,
 /*  4400 */   510,  258,  510,  510,  510,  510,  510,  510,  510,  510,
 /*  4410 */   510,  510,  510,  210,  267,  272,  288,  207,  510,  510,
 /*  4420 */   262,  510,  510,  510,  510,  510,  510,  510,  295,  510,
 /*  4430 */   510,  510,  510,  274,  510,  252,  510,  510,  510,  282,
 /*  4440 */   510,  510,  510,  510,  510,  510,  510,  510,  510,  510,
 /*  4450 */   510,  510,  510,  510,  510,  510,  510,  510,  510,  510,
 /*  4460 */   510,  510,  510,  510,  510,  510,  510,  510,  510,  510,
 /*  4470 */   510,  510,  276,  510,  510,  510,  510,  510,  510,  510,
 /*  4480 */   275,  277,  278,  279,  280,  281,  510,  510,  214,  223,
 /*  4490 */   226,  217,  220,  510,  510,  510,  510,  510,  510,  510,
 /*  4500 */   258,  510,  510,  510,  510,  510,  510,  510,  510,  510,
 /*  4510 */   510,  510,  210,  267,  272,  288,  207,  510,  510,  284,
 /*  4520 */   510,  510,  510,  510,  510,  510,  510,  295,  510,  510,
 /*  4530 */   510,  510,  274,  510,  252,  510,  510,  510,  282,  510,
 /*  4540 */   510,  510,  510,  510,  510,  510,  510,  510,  510,  510,
 /*  4550 */   510,  510,  510,  510,  510,  510,  510,  510,  510,  510,
 /*  4560 */   510,  510,  510,  510,  510,  510,  510,  510,  510,  510,
 /*  4570 */   510,  339,  510,  510,  510,  510,  510,  510,  510,  338,
 /*  4580 */   277,  278,  279,  280,  281,
};
static YYCODETYPE yy_lookahead[] = {
 /*     0 */     7,    8,    9,   10,   11,  133,   46,  135,  136,  137,
 /*    10 */   138,  139,   19,  105,  106,  107,  108,  109,  110,  111,
 /*    20 */   112,  176,  177,   47,   31,   32,   33,   34,   35,  116,
 /*    30 */    49,   38,  119,  120,  121,  122,  123,   38,   57,   46,
 /*    40 */    38,  128,  129,  115,   51,  117,   53,  134,  137,  138,
 /*    50 */    57,  140,   59,   60,   61,   62,   63,   64,   65,   66,
 /*    60 */    67,   68,   69,  121,  122,  123,   85,   86,   87,   88,
 /*    70 */    89,  129,   32,   92,   93,  131,  134,   38,   38,   39,
 /*    80 */     6,  137,  138,   90,  140,   46,   46,   47,   48,   90,
 /*    90 */   137,   98,   99,  100,  101,  102,  103,  165,  166,   59,
 /*   100 */    60,   61,   62,   63,   64,   65,   66,   67,   39,   69,
 /*   110 */    70,   71,   72,   39,   74,   38,   32,  164,   78,   79,
 /*   120 */    80,   81,   38,   39,   84,  172,  173,  174,  175,   47,
 /*   130 */    46,   47,   48,    6,   94,   95,   96,   97,  108,  109,
 /*   140 */   110,  111,  112,   59,   60,   61,   62,   63,   64,   65,
 /*   150 */    66,   67,   49,   69,   70,   71,   72,   75,   74,   77,
 /*   160 */    57,  118,   78,   79,   80,   81,  123,   90,   84,   32,
 /*   170 */    28,   29,   30,  130,   47,   38,   39,  134,   94,   95,
 /*   180 */    96,   97,    6,   46,   47,   48,  134,   39,   85,   86,
 /*   190 */    87,   88,   89,  115,   46,  117,   59,   60,   61,   62,
 /*   200 */    63,   64,   65,   66,   67,   41,   69,   70,   71,   72,
 /*   210 */    46,   74,   36,   32,  113,   78,   79,   80,   81,   38,
 /*   220 */    39,   84,   38,  136,  137,  138,  139,   46,   47,   48,
 /*   230 */     6,   94,   95,   96,   97,  176,  177,  162,  163,   38,
 /*   240 */    59,   60,   61,   62,   63,   64,   65,   66,   67,   48,
 /*   250 */    69,   70,   71,   72,  119,   74,  121,  122,  123,   78,
 /*   260 */    79,   80,   81,  128,  129,   84,   32,  176,  177,  134,
 /*   270 */    46,   38,   38,   39,   90,   94,   95,   96,   97,  164,
 /*   280 */    46,   47,   48,   38,   38,  170,  171,  172,  173,  174,
 /*   290 */   175,  123,   46,   59,   60,   61,   62,   63,   64,   65,
 /*   300 */    66,   67,  134,   69,   70,   71,   72,  115,   74,  117,
 /*   310 */    32,  164,   78,   79,   80,   81,   38,   39,   84,  172,
 /*   320 */   173,  174,  175,   90,   46,   47,   48,   46,   94,   95,
 /*   330 */    96,   97,    6,   53,   54,   54,   38,   59,   60,   61,
 /*   340 */    62,   63,   64,   65,   66,   67,   49,   69,   70,   71,
 /*   350 */    72,    6,   74,  121,    6,  123,   78,   79,   80,   81,
 /*   360 */   128,  129,   84,   32,  114,   39,  134,  176,  177,   38,
 /*   370 */    39,   91,   94,   95,   96,   97,   47,   46,   47,   48,
 /*   380 */   167,   36,   85,   86,   87,   88,   89,  124,  123,   53,
 /*   390 */    59,   60,   61,   62,   63,   64,   65,   66,   67,  134,
 /*   400 */    69,   70,   71,   72,   75,   74,   77,   32,  164,   78,
 /*   410 */    79,   80,   81,   38,   39,   84,  172,  173,  174,  175,
 /*   420 */    53,   46,   47,   48,   38,   94,   95,   96,   97,    6,
 /*   430 */    82,    6,   46,   46,   59,   60,   61,   62,   63,   64,
 /*   440 */    65,   66,   67,    6,   69,   70,   71,   72,    6,   74,
 /*   450 */   123,  176,  177,   78,   79,   80,   81,  130,   91,   84,
 /*   460 */    32,  134,   39,   41,   39,   43,   38,   39,   46,   94,
 /*   470 */    95,   96,   97,   36,   46,   47,   48,   38,   36,   25,
 /*   480 */    26,   27,   28,   29,   30,   46,   39,   59,   60,   61,
 /*   490 */    62,   63,   64,   65,   66,   67,    6,   69,   70,   71,
 /*   500 */    72,   54,   74,   58,   32,  164,   78,   79,   80,   81,
 /*   510 */    38,   39,   84,  172,  173,  174,  175,   51,   46,   47,
 /*   520 */    48,    6,   94,   95,   96,   97,  176,  177,    6,    6,
 /*   530 */    39,   59,   60,   61,   62,   63,   64,   65,   66,   67,
 /*   540 */     6,   69,   70,   71,   72,   54,   74,   39,   58,  123,
 /*   550 */    78,   79,   80,   81,   39,  129,   84,   32,   39,   36,
 /*   560 */   134,   39,   54,   38,   39,   46,   94,   95,   96,   97,
 /*   570 */    36,   46,   47,   48,  133,   46,  135,  136,  137,  138,
 /*   580 */   139,  176,  177,   54,   59,   60,   61,   62,   63,   64,
 /*   590 */    65,   66,   67,    6,   69,   70,   71,   72,   49,   74,
 /*   600 */    20,   32,  164,   78,   79,   80,   81,   38,   39,   84,
 /*   610 */   172,  173,  174,  175,    6,   46,   47,   48,    6,   94,
 /*   620 */    95,   96,   97,   36,  176,  177,  126,  127,   59,   60,
 /*   630 */    61,   62,   63,   64,   65,   66,   67,   57,   69,   70,
 /*   640 */    71,   72,    6,   74,   36,   39,  167,   78,   79,   80,
 /*   650 */    81,   39,   46,   84,   32,   49,  124,   41,   39,   43,
 /*   660 */    38,   39,   46,   94,   95,   96,   97,  164,   46,   47,
 /*   670 */    48,  168,   36,  176,  177,  172,  173,  174,  175,  123,
 /*   680 */    39,   59,   60,   61,   62,   63,   64,   65,   66,   67,
 /*   690 */   134,   69,   70,   71,   72,   54,   74,   51,   32,  164,
 /*   700 */    78,   79,   80,   81,   38,   39,   84,  172,  173,  174,
 /*   710 */   175,    6,   46,   47,   48,  124,   94,   95,   96,   97,
 /*   720 */    49,   38,   53,    6,   39,   59,   60,   61,   62,   63,
 /*   730 */    64,   65,   66,   67,   51,   69,   70,   71,   72,   38,
 /*   740 */    74,   36,  114,    6,   78,   79,   80,   81,   47,   48,
 /*   750 */    84,   32,   76,   36,   41,  176,  177,   38,   39,   46,
 /*   760 */    94,   95,   96,   97,  164,   46,   47,   48,  168,   38,
 /*   770 */    76,  167,  172,  173,  174,  175,   39,   46,   59,   60,
 /*   780 */    61,   62,   63,   64,   65,   66,   67,    6,   69,   70,
 /*   790 */    71,   72,  124,   74,   49,   32,  164,   78,   79,   80,
 /*   800 */    81,   38,   39,   84,  172,  173,  174,  175,    6,   46,
 /*   810 */    47,   48,    6,   94,   95,   96,   97,  125,   54,    6,
 /*   820 */    39,   57,   59,   60,   61,   62,   63,   64,   65,   66,
 /*   830 */    67,  124,   69,   70,   71,   72,  125,   74,   36,   39,
 /*   840 */    39,   78,   79,   80,   81,   39,   46,   84,   32,   49,
 /*   850 */    35,   46,   39,   38,   38,   39,    6,   94,   95,   96,
 /*   860 */    97,  164,   46,   47,   48,   38,  169,   53,   39,  172,
 /*   870 */   173,  174,  175,   46,   39,   59,   60,   61,   62,   63,
 /*   880 */    64,   65,   66,   67,    6,   69,   70,   71,   72,   39,
 /*   890 */    74,   38,   32,  164,   78,   79,   80,   81,   38,   39,
 /*   900 */    84,  172,  173,  174,  175,    6,   46,   47,   48,   46,
 /*   910 */    94,   95,   96,   97,   36,   54,   39,   54,   57,   59,
 /*   920 */    60,   61,   62,   63,   64,   65,   66,   67,   46,   69,
 /*   930 */    70,   71,   72,   76,   74,   36,   54,  127,   78,   79,
 /*   940 */    80,   81,   38,   43,   84,   32,   46,   38,   46,  167,
 /*   950 */    39,   38,   39,  166,   94,   95,   96,   97,  164,   46,
 /*   960 */    47,   48,  168,   38,  125,   79,  172,  173,  174,  175,
 /*   970 */   114,  124,   59,   60,   61,   62,   63,   64,   65,   66,
 /*   980 */    67,   39,   69,   70,   71,   72,   47,   74,   39,   32,
 /*   990 */   164,   78,   79,   80,   81,   38,   39,   84,  172,  173,
 /*  1000 */   174,  175,   39,   46,   47,   48,  125,   94,   95,   96,
 /*  1010 */    97,   53,  124,   49,   38,   35,   59,   60,   61,   62,
 /*  1020 */    63,   64,   65,   66,   67,  117,   69,   70,   71,   72,
 /*  1030 */    47,   74,   47,  114,   53,   78,   79,   80,   81,  177,
 /*  1040 */    47,   84,   32,   42,   15,   38,   38,  114,   38,   39,
 /*  1050 */    53,   94,   95,   96,   97,  164,   46,   47,   48,  168,
 /*  1060 */    38,  178,   38,  172,  173,  174,  175,   46,  114,   59,
 /*  1070 */    60,   61,   62,   63,   64,   65,   66,   67,   38,   69,
 /*  1080 */    70,   71,   72,   47,   74,  114,   32,  164,   78,   79,
 /*  1090 */    80,   81,   38,   39,   84,  172,  173,  174,  175,   49,
 /*  1100 */    46,   47,   48,   39,   94,   95,   96,   97,   53,   46,
 /*  1110 */    38,   38,   79,   59,   60,   61,   62,   63,   64,   65,
 /*  1120 */    66,   67,   39,   69,   70,   71,   72,   38,   74,   42,
 /*  1130 */    39,   49,   78,   79,   80,   81,   46,   38,   84,   32,
 /*  1140 */    39,   39,   76,   20,   39,   38,   39,   53,   94,   95,
 /*  1150 */    96,   97,   38,   46,   47,   48,  133,   39,  135,  136,
 /*  1160 */   137,  138,  139,    6,   38,   73,   59,   60,   61,   62,
 /*  1170 */    63,   64,   65,   66,   67,   46,   69,   70,   71,   72,
 /*  1180 */    46,   74,   73,   32,  164,   78,   79,   80,   81,   38,
 /*  1190 */    39,   84,  172,  173,  174,  175,   20,   46,   47,   48,
 /*  1200 */    39,   94,   95,   96,   97,   36,   82,   20,   38,   38,
 /*  1210 */    59,   60,   61,   62,   63,   64,   65,   66,   67,  124,
 /*  1220 */    69,   70,   71,   72,   46,   74,   47,  167,   39,   78,
 /*  1230 */    79,   80,   81,   53,  178,   84,   32,   39,  167,   58,
 /*  1240 */   113,   39,   38,   39,  124,   94,   95,   96,   97,  164,
 /*  1250 */    46,   47,   48,  168,   38,   47,   39,  172,  173,  174,
 /*  1260 */   175,   38,   47,   59,   60,   61,   62,   63,   64,   65,
 /*  1270 */    66,   67,   46,   69,   70,   71,   72,  167,   74,   46,
 /*  1280 */    32,  164,   78,   79,   80,   81,   38,   39,   84,  172,
 /*  1290 */   173,  174,  175,   58,   46,   47,   48,   38,   94,   95,
 /*  1300 */    96,   97,   38,  167,    0,   38,   53,   59,   60,   61,
 /*  1310 */    62,   63,   64,   65,   66,   67,   47,   69,   70,   71,
 /*  1320 */    72,   38,   74,   47,   53,   38,   78,   79,   80,   81,
 /*  1330 */    51,  167,   84,   32,   38,  114,   46,  163,  114,   38,
 /*  1340 */    39,   38,   94,   95,   96,   97,  164,   46,   47,   48,
 /*  1350 */   168,  167,   47,   38,  172,  173,  174,  175,   58,   38,
 /*  1360 */    59,   60,   61,   62,   63,   64,   65,   66,   67,  124,
 /*  1370 */    69,   70,   71,   72,  167,   74,   38,   32,  164,   78,
 /*  1380 */    79,   80,   81,   38,   39,   84,  172,  173,  174,  175,
 /*  1390 */    49,   46,   47,   48,   76,   94,   95,   96,   97,   53,
 /*  1400 */    91,  185,  185,  185,   59,   60,   61,   62,   63,   64,
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
 /*  1540 */   164,   46,   47,   48,  168,  185,  185,  185,  172,  173,
 /*  1550 */   174,  175,  185,  185,   59,   60,   61,   62,   63,   64,
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
 /*  1820 */   160,  112,  133,  185,  135,  136,  137,  138,  139,  185,
 /*  1830 */   185,   38,  172,  173,  174,  175,  185,   44,   45,  185,
 /*  1840 */   185,  132,  185,  185,  185,   52,  185,  185,   55,  185,
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
 /*  1960 */   153,  154,  155,  156,  157,  158,  159,  160,  112,  133,
 /*  1970 */   185,  135,  136,  137,  138,  139,  185,  185,  185,  172,
 /*  1980 */   173,  174,  175,  164,   44,   45,  185,  168,  132,  185,
 /*  1990 */    50,  172,  173,  174,  175,   55,  185,  141,  142,  143,
 /*  2000 */   144,  145,  146,  147,  148,  149,  150,  151,  152,  153,
 /*  2010 */   154,  155,  156,  157,  158,  159,  160,  112,  185,  185,
 /*  2020 */   185,  185,  185,  185,  164,  185,  185,  185,  172,  173,
 /*  2030 */   174,  175,  172,  173,  174,  175,  185,  132,  185,    1,
 /*  2040 */     2,    3,    4,    5,  184,  185,  141,  142,  143,  144,
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
 /*  2260 */   159,  160,  112,  131,  185,  185,  185,  185,  185,  137,
 /*  2270 */   138,  185,  140,  172,  173,  174,  175,  185,   44,   45,
 /*  2280 */   185,  185,  132,  185,  185,  185,   52,  185,  185,   55,
 /*  2290 */   185,  141,  142,  143,  144,  145,  146,  147,  148,  149,
 /*  2300 */   150,  151,  152,  153,  154,  155,  156,  157,  158,  159,
 /*  2310 */   160,  112,  185,  185,  185,  185,  185,  185,  185,  164,
 /*  2320 */   185,  185,  172,  173,  174,  175,  171,  172,  173,  174,
 /*  2330 */   175,  132,  185,    1,    2,    3,    4,    5,  185,  185,
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
 /*  2790 */   185,  185,   46,  185,  185,  185,  185,   98,   99,  100,
 /*  2800 */   101,  102,  103,  164,  185,    7,    8,    9,   10,   11,
 /*  2810 */   185,  172,  173,  174,  175,  164,  185,   19,  185,  180,
 /*  2820 */   181,  185,  185,  172,  173,  174,  175,  185,  185,   31,
 /*  2830 */    32,   33,   34,   35,  185,  185,   38,  164,  185,  185,
 /*  2840 */   185,  185,  185,  185,   46,  172,  173,  174,  175,   51,
 /*  2850 */   185,   53,  185,  180,  181,   57,  185,   12,   13,   14,
 /*  2860 */   185,  185,   17,   18,   19,   20,   21,   22,   23,   24,
 /*  2870 */    25,   26,   27,   28,   29,   30,  164,   19,  185,  185,
 /*  2880 */   168,   83,  185,  185,  172,  173,  174,  175,   90,  185,
 /*  2890 */     1,    2,    3,    4,    5,  185,   98,   99,  100,  101,
 /*  2900 */   102,  103,  164,   58,    7,    8,    9,   10,   11,  185,
 /*  2910 */   172,  173,  174,  175,   56,  185,   19,   59,   60,   61,
 /*  2920 */    62,   63,   64,   65,   66,   67,   68,   69,   31,   32,
 /*  2930 */    33,   34,   35,   44,   45,   38,   47,   48,  185,   50,
 /*  2940 */   185,  185,  185,   46,   55,  185,  185,  185,   51,  185,
 /*  2950 */    53,  185,  185,  185,   57,   58,   12,   13,   14,  185,
 /*  2960 */   185,   17,   18,   19,   20,   21,   22,   23,   24,   25,
 /*  2970 */    26,   27,   28,   29,   30,  185,   19,  185,  185,  185,
 /*  2980 */    36,  185,  164,  185,  185,  185,  168,   90,  185,  185,
 /*  2990 */   172,  173,  174,  175,  185,   98,   99,  100,  101,  102,
 /*  3000 */   103,  164,  185,    7,    8,    9,   10,   11,  185,  172,
 /*  3010 */   173,  174,  175,  185,  185,   19,   59,   60,   61,   62,
 /*  3020 */    63,   64,   65,   66,   67,   68,   69,   31,   32,   33,
 /*  3030 */    34,   35,   36,  185,   38,   19,  185,  185,  185,  185,
 /*  3040 */   185,  185,   46,  185,  185,  185,  185,   51,  185,   53,
 /*  3050 */   185,  185,   36,   57,   38,   12,   13,   14,   38,  185,
 /*  3060 */    17,   18,   19,   20,   21,   22,   23,   24,   25,   26,
 /*  3070 */    27,   28,   29,   30,  185,   59,   60,   61,   62,   63,
 /*  3080 */    64,   65,   66,   67,   68,   69,   90,  185,    1,    2,
 /*  3090 */     3,    4,    5,  185,   98,   99,  100,  101,  102,  103,
 /*  3100 */   185,   58,    7,    8,    9,   10,   11,  185,  185,  185,
 /*  3110 */    90,  185,  185,  185,   19,  185,  185,  185,   98,   99,
 /*  3120 */   100,  101,  102,  103,  185,  185,   31,   32,   33,   34,
 /*  3130 */    35,   44,   45,   38,   47,   48,  185,   50,  185,   49,
 /*  3140 */   185,   46,   55,  185,   54,  185,   51,   57,   53,  185,
 /*  3150 */   185,  185,   57,   58,   12,   13,   14,  185,  185,   17,
 /*  3160 */    18,   19,   20,   21,   22,   23,   24,   25,   26,   27,
 /*  3170 */    28,   29,   30,  185,  185,   85,   86,   87,   88,   89,
 /*  3180 */   185,   91,   92,   93,  185,   90,  185,  185,   46,  185,
 /*  3190 */   185,  185,  185,   98,   99,  100,  101,  102,  103,  164,
 /*  3200 */   185,    7,    8,    9,   10,   11,  185,  172,  173,  174,
 /*  3210 */   175,  185,  164,   19,  185,  180,  181,  169,  185,  185,
 /*  3220 */   172,  173,  174,  175,  185,   31,   32,   33,   34,   35,
 /*  3230 */   185,  185,   38,   39,  185,    1,    2,    3,    4,    5,
 /*  3240 */    46,  185,  185,  185,  185,   51,  185,   53,  185,  185,
 /*  3250 */   185,   57,  185,   12,   13,   14,  185,  185,   17,   18,
 /*  3260 */    19,   20,   21,   22,   23,   24,   25,   26,   27,   28,
 /*  3270 */    29,   30,  185,  185,  185,  185,  185,  185,   44,   45,
 /*  3280 */    39,   47,   48,  185,   90,  185,  185,  185,  185,   55,
 /*  3290 */   185,  185,   98,   99,  100,  101,  102,  103,  164,  185,
 /*  3300 */     7,    8,    9,   10,   11,  185,  172,  173,  174,  175,
 /*  3310 */   185,  164,   19,  185,  180,  181,  169,  185,  185,  172,
 /*  3320 */   173,  174,  175,  185,   31,   32,   33,   34,   35,   36,
 /*  3330 */   185,   38,   19,  185,  185,  185,  185,  185,  185,   46,
 /*  3340 */   185,  185,  185,  185,   51,  185,   53,  185,  185,   36,
 /*  3350 */    57,   38,   12,   13,   14,  185,  185,   17,   18,   19,
 /*  3360 */    20,   21,   22,   23,   24,   25,   26,   27,   28,   29,
 /*  3370 */    30,  185,   59,   60,   61,   62,   63,   64,   65,   66,
 /*  3380 */    67,   68,   69,   90,  185,  185,  185,  185,  185,  185,
 /*  3390 */   185,   98,   99,  100,  101,  102,  103,  164,   58,    7,
 /*  3400 */     8,    9,   10,   11,  185,  172,  173,  174,  175,  185,
 /*  3410 */   164,   19,  185,  180,  181,  185,  185,  185,  172,  173,
 /*  3420 */   174,  175,  185,   31,   32,   33,   34,   35,   36,  185,
 /*  3430 */    38,   19,  185,  185,  185,  185,  185,  185,   46,  185,
 /*  3440 */   185,  185,  185,   51,  185,   53,  185,  185,   36,   57,
 /*  3450 */    38,   12,   13,   14,  185,  185,   17,   18,   19,   20,
 /*  3460 */    21,   22,   23,   24,   25,   26,   27,   28,   29,   30,
 /*  3470 */   185,   59,   60,   61,   62,   63,   64,   65,   66,   67,
 /*  3480 */    68,   69,   90,  185,  185,   46,  185,  185,  185,  185,
 /*  3490 */    98,   99,  100,  101,  102,  103,  164,  185,    7,    8,
 /*  3500 */     9,   10,   11,  185,  172,  173,  174,  175,  185,  164,
 /*  3510 */    19,  185,  185,  181,  185,  185,  185,  172,  173,  174,
 /*  3520 */   175,  185,   31,   32,   33,   34,   35,   36,  185,   38,
 /*  3530 */    19,  185,  185,  185,  185,  185,  185,   46,  185,  185,
 /*  3540 */   185,  185,   51,  185,   53,  185,  185,  185,   57,   38,
 /*  3550 */    12,   13,   14,  185,  185,   17,   18,   19,   20,   21,
 /*  3560 */    22,   23,   24,   25,   26,   27,   28,   29,   30,  185,
 /*  3570 */    59,   60,   61,   62,   63,   64,   65,   66,   67,   68,
 /*  3580 */    69,   90,  185,  185,   46,  185,  185,  185,  185,   98,
 /*  3590 */    99,  100,  101,  102,  103,  164,  185,    7,    8,    9,
 /*  3600 */    10,   11,  185,  172,  173,  174,  175,  185,  164,   19,
 /*  3610 */   185,  185,  168,  185,  185,  185,  172,  173,  174,  175,
 /*  3620 */   185,   31,   32,   33,   34,   35,  161,  185,   38,  164,
 /*  3630 */   185,  185,  185,  185,  185,  185,   46,  172,  173,  174,
 /*  3640 */   175,   51,  185,   53,  185,  185,  185,   57,   58,   12,
 /*  3650 */    13,   14,  185,  185,   17,   18,   19,   20,   21,   22,
 /*  3660 */    23,   24,   25,   26,   27,   28,   29,   30,  185,  185,
 /*  3670 */   185,  185,  185,  185,  164,  185,   39,  185,  185,  185,
 /*  3680 */    90,  185,  172,  173,  174,  175,  185,  185,   98,   99,
 /*  3690 */   100,  101,  102,  103,  185,  185,    7,    8,    9,   10,
 /*  3700 */    11,  185,  185,  185,  161,  185,  185,  164,   19,  185,
 /*  3710 */   185,  185,  185,  185,  185,  172,  173,  174,  175,  185,
 /*  3720 */    31,   32,   33,   34,   35,   36,  185,   38,  164,    1,
 /*  3730 */     2,    3,    4,    5,  185,   46,  172,  173,  174,  175,
 /*  3740 */    51,  185,   53,  185,  185,  185,   57,  185,   12,   13,
 /*  3750 */    14,  185,  185,   17,   18,   19,   20,   21,   22,   23,
 /*  3760 */    24,   25,   26,   27,   28,   29,   30,  185,  185,  185,
 /*  3770 */   185,  185,   44,   45,  185,   47,   48,  185,   50,   90,
 /*  3780 */   185,  185,  185,   55,  185,  185,  185,   98,   99,  100,
 /*  3790 */   101,  102,  103,  164,   58,    7,    8,    9,   10,   11,
 /*  3800 */   185,  172,  173,  174,  175,  164,  185,   19,  185,  185,
 /*  3810 */   185,  185,  185,  172,  173,  174,  175,  185,  185,   31,
 /*  3820 */    32,   33,   34,   35,   36,  185,   38,  164,    1,    2,
 /*  3830 */     3,    4,    5,  185,   46,  172,  173,  174,  175,   51,
 /*  3840 */   185,   53,  185,  185,  185,   57,  185,   12,   13,   14,
 /*  3850 */   185,  185,   17,   18,   19,   20,   21,   22,   23,   24,
 /*  3860 */    25,   26,   27,   28,   29,   30,  185,  185,  185,  185,
 /*  3870 */   185,   44,   45,  185,   47,   48,  185,   50,   90,  185,
 /*  3880 */   185,  185,   55,  185,  185,  185,   98,   99,  100,  101,
 /*  3890 */   102,  103,  185,  164,    7,    8,    9,   10,   11,  185,
 /*  3900 */   185,  172,  173,  174,  175,  164,   19,  185,  185,  185,
 /*  3910 */   185,  185,  185,  172,  173,  174,  175,  185,   31,   32,
 /*  3920 */    33,   34,   35,   36,  185,   38,  164,    1,    2,    3,
 /*  3930 */     4,    5,  185,   46,  172,  173,  174,  175,   51,  185,
 /*  3940 */    53,  185,  185,  185,   57,  185,  185,   13,   14,  185,
 /*  3950 */   185,   17,   18,   19,   20,   21,   22,   23,   24,   25,
 /*  3960 */    26,   27,   28,   29,   30,  185,  185,  185,  185,  185,
 /*  3970 */    44,   45,  185,   47,   48,  185,  185,   90,  185,  185,
 /*  3980 */   185,   55,  185,  185,  185,   98,   99,  100,  101,  102,
 /*  3990 */   103,  185,  185,    7,    8,    9,   10,   11,    1,    2,
 /*  4000 */     3,    4,    5,  185,  185,   19,  185,    1,    2,    3,
 /*  4010 */     4,    5,    1,    2,    3,    4,    5,   31,   32,   33,
 /*  4020 */    34,   35,   36,    0,   38,  164,    1,    2,    3,    4,
 /*  4030 */     5,  185,   46,  172,  173,  174,  175,   51,  185,   53,
 /*  4040 */   185,   44,   45,   57,   47,   48,  185,  185,  185,  185,
 /*  4050 */    44,   45,   55,   47,   48,   44,   45,  185,   47,   48,
 /*  4060 */    37,   55,  185,   40,  185,   42,   55,   44,   45,   44,
 /*  4070 */    45,   48,   47,   48,  185,  185,   90,  185,  185,  185,
 /*  4080 */    55,  185,  185,  185,   98,   99,  100,  101,  102,  103,
 /*  4090 */   185,  164,    7,    8,    9,   10,   11,  185,  185,  172,
 /*  4100 */   173,  174,  175,  185,   19,  185,    1,    2,    3,    4,
 /*  4110 */     5,    1,    2,    3,    4,    5,   31,   32,   33,   34,
 /*  4120 */    35,  185,  185,   38,  164,  185,  185,  185,  185,  185,
 /*  4130 */   185,   46,  172,  173,  174,  175,   51,  185,   53,  185,
 /*  4140 */   185,  185,   57,   58,  185,  185,  185,  185,  185,   44,
 /*  4150 */    45,  185,   47,   48,   44,   45,  185,   47,   48,  164,
 /*  4160 */    55,  185,  185,  164,  185,   55,  185,  172,  173,  174,
 /*  4170 */   175,  172,  173,  174,  175,   90,  185,  185,  185,  185,
 /*  4180 */   185,  185,  185,   98,   99,  100,  101,  102,  103,  185,
 /*  4190 */   164,    7,    8,    9,   10,   11,  185,  164,  172,  173,
 /*  4200 */   174,  175,  185,   19,  185,  172,  173,  174,  175,  185,
 /*  4210 */   185,  185,  185,  185,  185,   31,   32,   33,   34,   35,
 /*  4220 */    36,  185,   38,  185,  185,  185,  185,  185,  185,  185,
 /*  4230 */    46,  185,  185,  185,  185,   51,  185,   53,  185,  185,
 /*  4240 */   185,   57,  185,  185,  185,  185,  185,  185,  185,  185,
 /*  4250 */   185,  185,  185,  185,  185,  185,  185,  185,  185,  185,
 /*  4260 */   185,  185,  185,  185,  185,  185,  185,  185,  185,  185,
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
#define YY_SHIFT_USE_DFLT (-41)
static short yy_shift_ofst[] = {
 /*     0 */  1602, 1304, 4023,  -41,  -41,  -41,  -41,  -41,    2, 1217,
 /*    10 */   -41,  853,  713,  -41, 1283, 1063,  -41, 4025,  -41, 4110,
 /*    20 */   -41,  -41, 2136, 1126,  669, 3016,  491, 2858, 1198,  -41,
 /*    30 */   -41, 1029, 2957,  -41,  980,  -41,  -41,  -41, 1073,  580,
 /*    40 */   -41, 1300, 1176,  -41,  -41,  -41,  -41,  -41,  -41,  -41,
 /*    50 */   -41,  -41,  -41,  -41,  -41,  -41,  -41,  636,  447, 2858,
 /*    60 */    69,  -41,  -41, 3511,  -41, 1050, 3020,  -41,  -41,  -41,
 /*    70 */   -41,  -41,  -41,  -41,  -41, 1309, 1279,  -41,  815, 1082,
 /*    80 */  3020,  -41, 1303, 1341, 3020,  -41, 1040,  964, 3020,  -41,
 /*    90 */  1216, 1187,  -41,  -41,  -41,  -41,  -41,  -41,  -41,  -41,
 /*   100 */   -41,  -41, 1624, 2332, 1008,  997, 3412,  508, 2858,  829,
 /*   110 */   -41,  -41,  345,  641, 2858, 1161,  -41,  -41,  -41,  -41,
 /*   120 */  1259,  422,  -41, 1315,  900,  -41,  904,  224,  -41, 3087,
 /*   130 */   -41, 1036,  -41, 2889, 3997,  -41, 2234,  245, 1346, 3313,
 /*   140 */   882, 1054,  -41, 1539,  -41,  -41,  -41,  -41,  -41,  -41,
 /*   150 */   -41,  -41,  -41,  -41,  -41,  -41,  -41,  -41,  -41,  -41,
 /*   160 */   -41,  -41,  -41,  -41,  -41,  -41,  -41, 1202,  -41, 1064,
 /*   170 */   -41, 4382, 1090,  278,  472, 1092, 1134, 1248,  525,  -41,
 /*   180 */  4382, 1233,   82,  -41,  329,  -41,  -41, 3020, 1318, 1592,
 /*   190 */  1592,  387, 1301, 1398,  -41, 4382,  805,  181,  666,  -41,
 /*   200 */   902,  137,  569,  886, 4382,  949,  -41, 4382,  -41, 4382,
 /*   210 */  1022,  -41,  764, 4382, 4382, 3835, 4382, 4382, 3835, 4382,
 /*   220 */  4382, 3835, 4382, 4382, 3835, 4382, 4382, 3835, 4382,  454,
 /*   230 */  4382,  454, 4382,  454, 4382,  142, 4382,  142, 4382,  -41,
 /*   240 */  4382,  -41, 4382,  -41, 4382,  142, 4382, 2652, 4382, 2652,
 /*   250 */  4382, 3934,   -7, 2944,  -41, 1169, 4382,  -41, 1114, 1123,
 /*   260 */  4382,  454, 1688,  827, 1055, 3392, 3835, 1099, 1157, 1022,
 /*   270 */   -41,  -41, 4382,  -41,  -41,  -41,  -41,  -41,  -41,  -41,
 /*   280 */   -41,  -41, 4085, 3835, 2686,  683,  814, 3887,  731,  336,
 /*   290 */  2996,  -41,  534,  -41, 4382, 1223, 1208,  367, 4184,  802,
 /*   300 */   -41,  -41,  -41,  -41,  -41,  -41,   39, 1094, 2699,  587,
 /*   310 */   -41,  -41, 1170, 1179, 1180, 3491,  442,  -41,  -41,  -41,
 /*   320 */  1264, 1269, 1271, 3788,  -41,  523,  -41,  717,  -41,  -41,
 /*   330 */   -41, 4382, 3736,  -41, 3689,  878,  -41,  -41,  676,  694,
 /*   340 */   -41,  490,  -41, 4481,  -41,  857, 4382,  -41,  -41,  -41,
 /*   350 */   899,  -41,  -41,  184,  939,  958, 3293,  437,  -41,  -41,
 /*   360 */   985,  981, 3986,  608,  -41,  -41,  454,  454,  454,  454,
 /*   370 */  3043,  -41,  386,  -41,   77,  993,  -41,  983,  -41,  454,
 /*   380 */  3835,  976,  348, 2798, 2746, 1592,  913,  -41,  925,  813,
 /*   390 */   -41,  925,  -41, 3090,  -41,  -41,  -41,  -41,  -41,  -41,
 /*   400 */  4382,  -41, 3835,  246,  -19, 4382,  -41, 3095,  297, 4382,
 /*   410 */   -41,  445,  297, 4382,  -41, 2845,  -41,  -41,  233,  -24,
 /*   420 */   297, 4382,  -41, 1305,  297, 4382,  -41, 1296,  103, 4382,
 /*   430 */   -41, 3590,  297, 4382,  -41, 1235,  297, 4382,  -41, 2897,
 /*   440 */   297, 4382,  -41, 1181,  297, 4382,  -41,  -41,  -41,  -41,
 /*   450 */  4382,   74,  -41, 4382,  -41, 3835,  -41, 1118,  -41, 1105,
 /*   460 */   -41, 1102,  -41, 1083,  -41, 3194, 3637,  -41,  -41, 1072,
 /*   470 */   861, 1089, 1101,  -41, 4382, 3340, 1189,  -41, 4382, 3241,
 /*   480 */   -41,  298,  737,  -41,  298,  -41,  745, 3020,  -41,  -41,
 /*   490 */   298,  423,  -41,  298,  781,  -41,  298,  515,  -41,  298,
 /*   500 */   326,  -41,  298,  806,  -41,  298,  612,  -41,  298,  425,
 /*   510 */   -41,  298,  850,  -41,  298,  522,  -41,  280, 1338,  814,
 /*   520 */   439, 1055,   -1, 1276,  958, 1215,  981,  -41,  -41, 4382,
 /*   530 */  3142, 1592,   84,  -41, 1171, 1124, 4283, 3439, 1592,  234,
 /*   540 */   -41, 4382, 3538, 1592,  622,  -41, 1033, 4382, 1091,  -41,
 /*   550 */   -41,  -41, 1066, 1592, 1592,  -41,  -41, 1109, 1129,  428,
 /*   560 */   331,  -41,  -41, 2858, 1178,  957,  -41,  719,  -41,  176,
 /*   570 */   281, 1204,  -41,  375,  -41, 2858, 1226, 1495,  -41, 1345,
 /*   580 */   -41, 1624, 2038, 1267, 1253, 2719,  863, 1151,  -41, 1010,
 /*   590 */   -41, 2858, 1290,  763,  -41,   40,  -41,  705,  529, 1442,
 /*   600 */   -41, 1107,  -41, 2858,  -40,  816,  -41,  860,  -41, 3728,
 /*   610 */  4105,  -41,  -41, 1940,  466,  549, 3020,  619,  -41,  646,
 /*   620 */   671, 3020,  685,  -41,  -41, 1940, 1695,  606,  -41, 3020,
 /*   630 */   519,  -41,  801,  -41,  701,  835,  127,  877,  201,  -41,
 /*   640 */   -41,  909,  -41,  -41,  911,  -41, 1793,  800,  -41, 3020,
 /*   650 */   148,  -41,  942,  -41,  963,  -41,  -41, 3827, 3234, 3926,
 /*   660 */  1624, 4006,  -41, 4011,  904,  -41,  -41,  -41,  904,  224,
 /*   670 */   -41, 1001, 1007,  616,  -41, 1024, 1021,  -41,  904,  224,
 /*   680 */   -41, 1087, 1287,  164,  -41, 1321, 1021,  -41,  -41,
};
#define YY_REDUCE_USE_DFLT (-156)
static short yy_reduce_ofst[] = {
 /*     0 */   -92, -156,   30, -156, -156, -156, -156, -156, -156, -156,
 /*    10 */  -156, -156,  101, -156, -156, 1127, -156,   43, -156,  327,
 /*    20 */  -156, -156,   52, -156, -156, 2034, -156, 1836, -156, -156,
 /*    30 */  -156, -156,   87, -156, -156, -156, -156, -156, -156, -156,
 /*    40 */  -156, -156, -156, -156, -156, -156, -156, -156, -156, -156,
 /*    50 */  -156, -156, -156, -156, -156, -156, -156, -156, -156, -128,
 /*    60 */  -156, -156, -156,  -89, -156, -156, 1095, -156, -156, -156,
 /*    70 */  -156, -156, -156, -156, -156, -156, -156, -156, -156, -156,
 /*    80 */  1120, -156, -156, -156,  888, -156, -156, -156,  668, -156,
 /*    90 */  -156, -156, -156, -156, -156, -156, -156, -156, -156, -156,
 /*   100 */  -156, -156,  556,   52, -156, -156, 1936, -156,  441, -156,
 /*   110 */  -156, -156, -156, -156, 1689, -156, -156, -156, -156, -156,
 /*   120 */  -156, 1224, -156, -156,  628, -156,  192,  856, -156,  -87,
 /*   130 */  -156, -156, -156, 1653,  426, -156,   52, -156, -156, 2132,
 /*   140 */  -156, 2297, -156, 2542, -156, -156, -156, -156, -156, -156,
 /*   150 */  -156, -156, -156, -156, -156, -156, -156, -156, -156, -156,
 /*   160 */  -156, -156, -156, -156, -156, -156, -156, -156, -156, -156,
 /*   170 */  -156, 3543, -156, 1562, 2542, -156, -156, 2248, 2542, -156,
 /*   180 */  2056, -156,   75, -156, 1174, -156, -156, 1245, -156, 2395,
 /*   190 */  2542, -156, 2199, 2542, -156, 1958, -156, 2052, 2542, -156,
 /*   200 */  -156, 1954, 2542, -156, 1762, -156, -156,  826, -156,  438,
 /*   210 */   883, -156, -156, 1020, 3960, -156,  341,  632, -156, 2651,
 /*   220 */  2837, -156, 3345, 3510, -156, 3564, 3663, -156, 3729, -156,
 /*   230 */  3762, -156, 3927, -156, 4026, -156, 1214, -156,  244, -156,
 /*   240 */   535, -156, 3629, -156, 3995, -156, 1311, -156,  923, -156,
 /*   250 */  3741, -156,  -47, -156, -156, -156, 2738, -156, -156, -156,
 /*   260 */  1117, -156, -156, -156, -156, 2350, -156, -156, -156, 1056,
 /*   270 */  -156, -156, 3431, -156, -156, -156, -156, -156, -156, -156,
 /*   280 */  -156, -156, 1713, -156, -156, -156, -156, 2399, -156, -156,
 /*   290 */  2448, -156, -156, -156, 3332, -156, -156, -156, 2547, -156,
 /*   300 */  -156, -156, -156, -156, -156, -156, -156, -156, 2639, -156,
 /*   310 */  -156, -156, -156, -156, -156, 2301, -156, -156, -156, -156,
 /*   320 */  -156, -156, -156, 2673, -156, -156, -156, -156, -156, -156,
 /*   330 */  -156,  147, -156, -156, 3035, -156, -156, -156, -156, -156,
 /*   340 */  -156, -156, -156, 2252, -156, -156, 1860, -156, -156, -156,
 /*   350 */  -156, -156, -156, -156, -156, -156, 3134, -156, -156, -156,
 /*   360 */  -156, -156, 3233, -156, -156, -156, -156, -156, -156, -156,
 /*   370 */  -156, -156, -156, -156, -156, -156, -156, -156, -156, -156,
 /*   380 */  -156, -156, -156, 3246, -156, 1709, 2542, -156,  -68, -156,
 /*   390 */  -156,  787, -156,  782, -156, -156, -156, -156, -156, -156,
 /*   400 */   503, -156, -156, -156,  604,  600, -156,  697,  479,  794,
 /*   410 */  -156, -156,  213,  891, -156, -156, -156, -156, -156, -156,
 /*   420 */  1207, 1085, -156, -156, 1184, 1182, -156, -156, 1164, 1376,
 /*   430 */  -156, 3048, 1136, 1819, -156, -156, 1110, 2712, -156, 3147,
 /*   440 */  1071, 2818, -156, -156, 1060, 3444, -156, -156, -156, -156,
 /*   450 */   115, -156, -156, 2155, -156, -156, -156, -156, -156, -156,
 /*   460 */  -156, -156, -156, -156, -156, 3861, -156, -156, -156, -156,
 /*   470 */  -156, -156, -156, -156,  729, -156, -156, -156, 3641, -156,
 /*   480 */  -156,   59, -156, -156,  862, -156, -156,  263, -156, -156,
 /*   490 */   275, -156, -156,  579, -156, -156,  405, -156, -156,  191,
 /*   500 */  -156, -156,   91, -156, -156,  497, -156, -156,  350, -156,
 /*   510 */  -156, -155, -156, -156,  448, -156, -156, -156, -156, -156,
 /*   520 */  -156, -156, -156, -156, -156, -156, -156, -156, -156, 3999,
 /*   530 */  -156, 1905, 2542, -156, -156, -156, 4033, -156, 2150, 2542,
 /*   540 */  -156, 1408, -156, 2444, 2542, -156, -156, 3465, -156, -156,
 /*   550 */  -156, -156, -156, 2003, 2542, -156, -156, -156, -156, 2101,
 /*   560 */  2542, -156, -156, 1591, -156, 2346, -156, 2542, -156, -156,
 /*   570 */  -156, 1807, -156, 2542, -156, 1545, -156, 1856, -156, 2542,
 /*   580 */  -156,  168,   52, -156, -156,  -56, -156, 1758, -156, 2542,
 /*   590 */  -156, 1314, -156, 1660, -156, 2542, -156, -156, -156, 1611,
 /*   600 */  -156, 2542, -156, 1023, -156, 2493, -156, 2542, -156,  232,
 /*   610 */   426, -156, -156,  168, -156, -156,  532, -156, -156, -156,
 /*   620 */  -156,  591, -156, -156, -156,  265,   52,  692, -156,  707,
 /*   630 */   711, -156, -156, -156,  500, -156, -156, -156,  810, -156,
 /*   640 */  -156, -156, -156, -156, -156, -156,   52,  839, -156,  847,
 /*   650 */   881, -156, -156, -156, -156, -156, -156,  135,  -58,  426,
 /*   660 */   265,  426, -156,  426,  908, -156, -156, -156,  -72,  919,
 /*   670 */  -156, -156, -156,  933, -156, -156,  954, -156,   78,  971,
 /*   680 */  -156, -156, -156,  250, -156, -156, 1221, -156, -156,
};
static YYACTIONTYPE yy_default[] = {
 /*     0 */  1012, 1012, 1012,  691,  693,  694,  695,  696, 1012, 1012,
 /*    10 */   697, 1012, 1012,  698, 1012, 1012,  699, 1012,  714, 1012,
 /*    20 */   715,  745, 1012, 1012, 1012, 1012, 1012, 1012, 1012,  765,
 /*    30 */   783,  784, 1012,  785,  787,  788,  789,  790, 1012, 1012,
 /*    40 */   801, 1012, 1012,  802,  803,  804,  805,  806,  807,  808,
 /*    50 */   809,  810,  811,  812,  813,  786,  769, 1012, 1012, 1012,
 /*    60 */  1012,  766,  770, 1012,  791,  793, 1012,  797,  998,  999,
 /*    70 */  1000, 1001, 1002, 1003, 1004, 1012, 1012, 1005, 1012,  794,
 /*    80 */  1012,  798, 1012,  795, 1012,  799, 1012,  796, 1012,  800,
 /*    90 */  1012, 1012,  792,  773,  775,  776,  777,  778,  779,  780,
 /*   100 */   781,  782, 1012, 1012, 1012, 1012, 1012, 1012, 1012, 1012,
 /*   110 */   767,  771, 1012, 1012, 1012, 1012,  768,  772,  774,  746,
 /*   120 */  1012, 1012,  700, 1012, 1012,  701, 1012, 1012,  703, 1012,
 /*   130 */   709, 1012,  710, 1012, 1012,  743, 1012, 1012, 1012, 1012,
 /*   140 */  1012, 1012,  749, 1012,  751,  814,  816,  817,  818,  819,
 /*   150 */   820,  821,  822,  823,  824,  825,  826,  827,  828,  829,
 /*   160 */   830,  831,  832,  833,  834,  835,  836, 1012,  837, 1012,
 /*   170 */   838, 1012, 1012, 1012, 1012,  841, 1012, 1012, 1012,  842,
 /*   180 */  1012, 1012, 1012,  845, 1012,  846,  847, 1012, 1012,  849,
 /*   190 */   850, 1012, 1012, 1012,  853, 1012, 1012, 1012, 1012,  855,
 /*   200 */  1012, 1012, 1012, 1012, 1012, 1012,  857, 1012,  915, 1012,
 /*   210 */  1012,  916, 1012, 1012, 1012,  917, 1012, 1012,  918, 1012,
 /*   220 */  1012,  919, 1012, 1012,  920, 1012, 1012,  921, 1012,  927,
 /*   230 */  1012,  928, 1012,  929, 1012,  939, 1012,  940, 1012,  941,
 /*   240 */  1012,  942, 1012,  943, 1012,  944, 1012,  945, 1012,  946,
 /*   250 */  1012,  947, 1012, 1012,  930, 1012, 1012,  931, 1012, 1012,
 /*   260 */  1012,  932,  952, 1012,  933, 1012,  989, 1012, 1012, 1012,
 /*   270 */   948,  949, 1012,  950,  951,  953,  954,  955,  956,  957,
 /*   280 */   958,  959, 1012,  997,  952, 1012,  936, 1012, 1012,  962,
 /*   290 */  1012,  963, 1012,  964, 1012, 1012, 1012, 1012, 1012, 1012,
 /*   300 */   970,  971,  984,  985,  986,  988, 1012, 1012, 1012, 1012,
 /*   310 */   974,  975, 1012, 1012, 1012, 1012, 1012,  976,  977,  987,
 /*   320 */  1012, 1012,  965, 1012,  966, 1012,  967, 1012,  972,  973,
 /*   330 */   937, 1012, 1012,  938, 1012, 1012,  968,  969,  953,  954,
 /*   340 */   960, 1012,  961, 1012,  990, 1012, 1012,  992,  993,  991,
 /*   350 */  1012,  978,  979, 1012, 1012,  934, 1012, 1012,  980,  981,
 /*   360 */  1012,  935, 1012, 1012,  982,  983,  926,  925,  924,  923,
 /*   370 */  1012, 1006, 1012, 1007, 1012, 1012, 1008, 1012, 1009,  922,
 /*   380 */  1010, 1012, 1012, 1012, 1012, 1012, 1012,  858, 1012, 1012,
 /*   390 */   862, 1012,  863, 1012,  865,  866,  867,  868,  869,  870,
 /*   400 */  1012,  871,  914, 1012, 1012, 1012,  872, 1012, 1012, 1012,
 /*   410 */   875, 1012, 1012, 1012,  876, 1012,  882,  883, 1012, 1012,
 /*   420 */  1012, 1012,  873, 1012, 1012, 1012,  874, 1012, 1012, 1012,
 /*   430 */   877, 1012, 1012, 1012,  878, 1012, 1012, 1012,  879, 1012,
 /*   440 */  1012, 1012,  880, 1012, 1012, 1012,  881,  884,  885,  864,
 /*   450 */  1012, 1012,  887, 1012,  888,  890,  889, 1012,  891, 1012,
 /*   460 */   892, 1012,  893, 1012,  894, 1012, 1012,  895,  896, 1012,
 /*   470 */  1012, 1012, 1012,  897, 1012, 1012, 1012,  898, 1012, 1012,
 /*   480 */   899, 1012, 1012,  900, 1012,  910,  912, 1012,  913,  911,
 /*   490 */  1012, 1012,  901, 1012, 1012,  902, 1012, 1012,  903, 1012,
 /*   500 */  1012,  904, 1012, 1012,  905, 1012, 1012,  906, 1012, 1012,
 /*   510 */   907, 1012, 1012,  908, 1012, 1012,  909, 1012, 1012, 1012,
 /*   520 */  1012, 1012, 1012, 1012, 1012, 1012, 1012, 1011,  815, 1012,
 /*   530 */  1012, 1012, 1012,  859, 1012, 1012, 1012, 1012, 1012, 1012,
 /*   540 */   860, 1012, 1012, 1012, 1012,  861, 1012, 1012, 1012,  856,
 /*   550 */   854,  852, 1012, 1012,  851,  848,  843,  839, 1012, 1012,
 /*   560 */  1012,  844,  840, 1012, 1012, 1012,  757, 1012,  759, 1012,
 /*   570 */  1012, 1012,  750, 1012,  752, 1012, 1012, 1012,  758, 1012,
 /*   580 */   760, 1012, 1012, 1012, 1012, 1012, 1012, 1012,  753, 1012,
 /*   590 */   755, 1012, 1012, 1012,  761, 1012,  763, 1012, 1012, 1012,
 /*   600 */   754, 1012,  756, 1012, 1012, 1012,  762, 1012,  764, 1012,
 /*   610 */  1012,  741,  744, 1012, 1012, 1012, 1012, 1012,  747, 1012,
 /*   620 */  1012, 1012, 1012,  748,  725, 1012, 1012, 1012,  727, 1012,
 /*   630 */  1012,  729, 1012,  733, 1012, 1012, 1012, 1012, 1012,  737,
 /*   640 */   739, 1012,  740,  738, 1012,  731, 1012, 1012,  728, 1012,
 /*   650 */  1012,  730, 1012,  734, 1012,  732,  742, 1012, 1012, 1012,
 /*   660 */  1012, 1012,  726, 1012, 1012,  711,  713,  712, 1012, 1012,
 /*   670 */   702, 1012, 1012, 1012,  704, 1012, 1012,  705, 1012, 1012,
 /*   680 */   706, 1012, 1012, 1012,  707, 1012, 1012,  708,  692,
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
 /* 262 */ "xx_common_expr ::= CONSTANT",
 /* 263 */ "xx_common_expr ::= IDENTIFIER",
 /* 264 */ "xx_common_expr ::= INTEGER",
 /* 265 */ "xx_common_expr ::= STRING",
 /* 266 */ "xx_common_expr ::= CHAR",
 /* 267 */ "xx_common_expr ::= DOUBLE",
 /* 268 */ "xx_common_expr ::= NULL",
 /* 269 */ "xx_common_expr ::= TRUE",
 /* 270 */ "xx_common_expr ::= FALSE",
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
 /* 317 */ "xx_isset_expr ::= IDENTIFIER SBRACKET_OPEN xx_common_expr SBRACKET_CLOSE",
 /* 318 */ "xx_isset_expr ::= IDENTIFIER ARROW IDENTIFIER",
 /* 319 */ "xx_isset_expr ::= IDENTIFIER ARROW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE",
 /* 320 */ "xx_isset_expr ::= IDENTIFIER ARROW BRACKET_OPEN STRING BRACKET_CLOSE",
 /* 321 */ "xx_eval_expr ::= xx_common_expr",
 /* 322 */ "xx_comment ::= COMMENT",
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
// 2808 "parser.c"
      break;
    case 106:
// 1012 "parser.lemon"
{ json_object_put((yypminor->yy235)); }
// 2813 "parser.c"
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
// 3353 "parser.c"
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
      case 321:
// 1014 "parser.lemon"
{
	yygotominor.yy235 = yymsp[0].minor.yy235;
}
// 3393 "parser.c"
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
// 3407 "parser.c"
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
// 3430 "parser.c"
        break;
      case 8:
// 1042 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_namespace(yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(37,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 3439 "parser.c"
        break;
      case 9:
// 1046 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_interface(yymsp[-1].minor.yy0, yymsp[0].minor.yy235, NULL, status->scanner_state);
  yy_destructor(40,&yymsp[-2].minor);
}
// 3447 "parser.c"
        break;
      case 10:
// 1050 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_interface(yymsp[-3].minor.yy0, yymsp[0].minor.yy235, yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(40,&yymsp[-4].minor);
  yy_destructor(41,&yymsp[-2].minor);
}
// 3456 "parser.c"
        break;
      case 11:
// 1054 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class(yymsp[-1].minor.yy0, yymsp[0].minor.yy235, 0, 0, NULL, NULL, status->scanner_state);
  yy_destructor(42,&yymsp[-2].minor);
}
// 3464 "parser.c"
        break;
      case 12:
// 1058 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy235, 0, 0, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(42,&yymsp[-4].minor);
  yy_destructor(41,&yymsp[-2].minor);
}
// 3473 "parser.c"
        break;
      case 13:
// 1062 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy235, 0, 0, NULL, yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(42,&yymsp[-4].minor);
  yy_destructor(43,&yymsp[-2].minor);
}
// 3482 "parser.c"
        break;
      case 14:
// 1066 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class(yymsp[-5].minor.yy0, yymsp[0].minor.yy235, 0, 0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(42,&yymsp[-6].minor);
  yy_destructor(41,&yymsp[-4].minor);
  yy_destructor(43,&yymsp[-2].minor);
}
// 3492 "parser.c"
        break;
      case 15:
// 1070 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class(yymsp[-1].minor.yy0, yymsp[0].minor.yy235, 1, 0, NULL, NULL, status->scanner_state);
  yy_destructor(44,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-2].minor);
}
// 3501 "parser.c"
        break;
      case 16:
// 1074 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy235, 1, 0, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(44,&yymsp[-5].minor);
  yy_destructor(42,&yymsp[-4].minor);
  yy_destructor(41,&yymsp[-2].minor);
}
// 3511 "parser.c"
        break;
      case 17:
// 1078 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy235, 1, 0, NULL, yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(44,&yymsp[-5].minor);
  yy_destructor(42,&yymsp[-4].minor);
  yy_destructor(43,&yymsp[-2].minor);
}
// 3521 "parser.c"
        break;
      case 18:
// 1082 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class(yymsp[-1].minor.yy0, yymsp[0].minor.yy235, 0, 1, NULL, NULL, status->scanner_state);
  yy_destructor(45,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-2].minor);
}
// 3530 "parser.c"
        break;
      case 19:
// 1086 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy235, 0, 1, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(45,&yymsp[-5].minor);
  yy_destructor(42,&yymsp[-4].minor);
  yy_destructor(41,&yymsp[-2].minor);
}
// 3540 "parser.c"
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
// 3551 "parser.c"
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
// 3562 "parser.c"
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
// 3577 "parser.c"
        break;
      case 24:
      case 263:
      case 305:
// 1106 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state);
}
// 3586 "parser.c"
        break;
      case 27:
// 1118 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class_definition(yymsp[0].minor.yy235, NULL, NULL, status->scanner_state);
}
// 3593 "parser.c"
        break;
      case 28:
// 1122 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class_definition(NULL, NULL, yymsp[0].minor.yy235, status->scanner_state);
}
// 3600 "parser.c"
        break;
      case 29:
// 1126 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class_definition(NULL, yymsp[0].minor.yy235, NULL, status->scanner_state);
}
// 3607 "parser.c"
        break;
      case 30:
// 1130 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class_definition(yymsp[-1].minor.yy235, yymsp[0].minor.yy235, NULL, status->scanner_state);
}
// 3614 "parser.c"
        break;
      case 31:
// 1134 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class_definition(yymsp[-1].minor.yy235, NULL, yymsp[0].minor.yy235, status->scanner_state);
}
// 3621 "parser.c"
        break;
      case 32:
// 1138 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class_definition(yymsp[0].minor.yy235, NULL, yymsp[-1].minor.yy235, status->scanner_state);
}
// 3628 "parser.c"
        break;
      case 33:
// 1142 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class_definition(NULL, yymsp[0].minor.yy235, yymsp[-1].minor.yy235, status->scanner_state);
}
// 3635 "parser.c"
        break;
      case 34:
// 1146 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class_definition(yymsp[-2].minor.yy235, yymsp[0].minor.yy235, yymsp[-1].minor.yy235, status->scanner_state);
}
// 3642 "parser.c"
        break;
      case 35:
// 1150 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class_definition(yymsp[-1].minor.yy235, yymsp[0].minor.yy235, yymsp[-2].minor.yy235, status->scanner_state);
}
// 3649 "parser.c"
        break;
      case 38:
// 1167 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class_property(yymsp[-2].minor.yy235, yymsp[-1].minor.yy0, NULL, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(39,&yymsp[0].minor);
}
// 3657 "parser.c"
        break;
      case 39:
// 1171 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class_property(yymsp[-2].minor.yy235, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(39,&yymsp[0].minor);
}
// 3665 "parser.c"
        break;
      case 40:
// 1175 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class_property(yymsp[-4].minor.yy235, yymsp[-3].minor.yy0, yymsp[-1].minor.yy235, yymsp[-5].minor.yy0, NULL, status->scanner_state);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 3674 "parser.c"
        break;
      case 41:
// 1179 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class_property(yymsp[-4].minor.yy235, yymsp[-3].minor.yy0, yymsp[-1].minor.yy235, NULL, NULL, status->scanner_state);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 3683 "parser.c"
        break;
      case 42:
// 1183 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class_property(yymsp[-3].minor.yy235, yymsp[-2].minor.yy0, NULL, yymsp[-4].minor.yy0, yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(39,&yymsp[0].minor);
}
// 3691 "parser.c"
        break;
      case 43:
// 1187 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class_property(yymsp[-3].minor.yy235, yymsp[-2].minor.yy0, NULL, NULL, yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(39,&yymsp[0].minor);
}
// 3699 "parser.c"
        break;
      case 44:
// 1191 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class_property(yymsp[-5].minor.yy235, yymsp[-4].minor.yy0, yymsp[-2].minor.yy235, yymsp[-6].minor.yy0, yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(49,&yymsp[-3].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 3708 "parser.c"
        break;
      case 45:
// 1195 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class_property(yymsp[-5].minor.yy235, yymsp[-4].minor.yy0, yymsp[-2].minor.yy235, NULL, yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(49,&yymsp[-3].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 3717 "parser.c"
        break;
      case 50:
// 1215 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_property_shortcut(NULL, yymsp[0].minor.yy0, status->scanner_state);
}
// 3724 "parser.c"
        break;
      case 51:
// 1219 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_property_shortcut(yymsp[-1].minor.yy0, yymsp[0].minor.yy0, status->scanner_state);
}
// 3731 "parser.c"
        break;
      case 58:
// 1248 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy235, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(50,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 3741 "parser.c"
        break;
      case 59:
// 1252 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy235, NULL, status->scanner_state);
  yy_destructor(50,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 3751 "parser.c"
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
// 3763 "parser.c"
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
// 3775 "parser.c"
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
// 3787 "parser.c"
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
// 3799 "parser.c"
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
// 3811 "parser.c"
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
// 3823 "parser.c"
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
// 3835 "parser.c"
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
// 3847 "parser.c"
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
// 3860 "parser.c"
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
// 3873 "parser.c"
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
// 3886 "parser.c"
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
// 3899 "parser.c"
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
// 3912 "parser.c"
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
// 3925 "parser.c"
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
// 3938 "parser.c"
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
// 3951 "parser.c"
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
// 3963 "parser.c"
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
// 3975 "parser.c"
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
// 3987 "parser.c"
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
// 3999 "parser.c"
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
// 4010 "parser.c"
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
// 4021 "parser.c"
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
// 4032 "parser.c"
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
// 4043 "parser.c"
        break;
      case 86:
// 1363 "parser.lemon"
{
	yygotominor.yy235 = json_object_new_string("public");
  yy_destructor(1,&yymsp[0].minor);
}
// 4051 "parser.c"
        break;
      case 87:
// 1367 "parser.lemon"
{
	yygotominor.yy235 = json_object_new_string("protected");
  yy_destructor(2,&yymsp[0].minor);
}
// 4059 "parser.c"
        break;
      case 88:
// 1371 "parser.lemon"
{
	yygotominor.yy235 = json_object_new_string("private");
  yy_destructor(4,&yymsp[0].minor);
}
// 4067 "parser.c"
        break;
      case 89:
// 1375 "parser.lemon"
{
	yygotominor.yy235 = json_object_new_string("static");
  yy_destructor(3,&yymsp[0].minor);
}
// 4075 "parser.c"
        break;
      case 90:
// 1379 "parser.lemon"
{
	yygotominor.yy235 = json_object_new_string("scoped");
  yy_destructor(5,&yymsp[0].minor);
}
// 4083 "parser.c"
        break;
      case 91:
// 1383 "parser.lemon"
{
	yygotominor.yy235 = json_object_new_string("inline");
  yy_destructor(55,&yymsp[0].minor);
}
// 4091 "parser.c"
        break;
      case 92:
// 1387 "parser.lemon"
{
	yygotominor.yy235 = json_object_new_string("abstract");
  yy_destructor(44,&yymsp[0].minor);
}
// 4099 "parser.c"
        break;
      case 93:
// 1391 "parser.lemon"
{
	yygotominor.yy235 = json_object_new_string("final");
  yy_destructor(45,&yymsp[0].minor);
}
// 4107 "parser.c"
        break;
      case 94:
// 1396 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_return_type(1, NULL, status->scanner_state);
  yy_destructor(56,&yymsp[0].minor);
}
// 4115 "parser.c"
        break;
      case 95:
// 1400 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_return_type(0, yymsp[0].minor.yy235, status->scanner_state);
}
// 4122 "parser.c"
        break;
      case 96:
// 1404 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_list(yymsp[-2].minor.yy235, yymsp[0].minor.yy235);
  yy_destructor(15,&yymsp[-1].minor);
}
// 4130 "parser.c"
        break;
      case 98:
// 1412 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_return_type_item(yymsp[0].minor.yy235, NULL, 0, 0, status->scanner_state);
}
// 4137 "parser.c"
        break;
      case 99:
// 1416 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_return_type_item(yymsp[-1].minor.yy235, NULL, 1, 0, status->scanner_state);
  yy_destructor(35,&yymsp[0].minor);
}
// 4145 "parser.c"
        break;
      case 100:
// 1420 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_return_type_item(NULL, yymsp[0].minor.yy235, 0, 0, status->scanner_state);
}
// 4152 "parser.c"
        break;
      case 101:
// 1424 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_return_type_item(NULL, yymsp[0].minor.yy235, 0, 1, status->scanner_state);
}
// 4159 "parser.c"
        break;
      case 104:
// 1437 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_parameter(NULL, NULL, yymsp[0].minor.yy0, NULL, 0, status->scanner_state);
}
// 4166 "parser.c"
        break;
      case 105:
// 1441 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_parameter(yymsp[-1].minor.yy235, NULL, yymsp[0].minor.yy0, NULL, 0, status->scanner_state);
}
// 4173 "parser.c"
        break;
      case 106:
// 1445 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_parameter(yymsp[-2].minor.yy235, NULL, yymsp[0].minor.yy0, NULL, 1, status->scanner_state);
  yy_destructor(35,&yymsp[-1].minor);
}
// 4181 "parser.c"
        break;
      case 107:
// 1449 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_parameter(NULL, yymsp[-1].minor.yy235, yymsp[0].minor.yy0, NULL, 0, status->scanner_state);
}
// 4188 "parser.c"
        break;
      case 108:
// 1453 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_parameter(NULL, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy235, 0, status->scanner_state);
  yy_destructor(49,&yymsp[-1].minor);
}
// 4196 "parser.c"
        break;
      case 109:
// 1457 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_parameter(yymsp[-3].minor.yy235, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy235, 0, status->scanner_state);
  yy_destructor(49,&yymsp[-1].minor);
}
// 4204 "parser.c"
        break;
      case 110:
// 1461 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_parameter(yymsp[-4].minor.yy235, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy235, 1, status->scanner_state);
  yy_destructor(35,&yymsp[-3].minor);
  yy_destructor(49,&yymsp[-1].minor);
}
// 4213 "parser.c"
        break;
      case 111:
// 1465 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_parameter(NULL, yymsp[-3].minor.yy235, yymsp[-2].minor.yy0, yymsp[0].minor.yy235, 0, status->scanner_state);
  yy_destructor(49,&yymsp[-1].minor);
}
// 4221 "parser.c"
        break;
      case 112:
// 1469 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(19,&yymsp[-2].minor);
  yy_destructor(20,&yymsp[0].minor);
}
// 4230 "parser.c"
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
// 4241 "parser.c"
        break;
      case 114:
// 1477 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_type(XX_TYPE_INTEGER);
  yy_destructor(59,&yymsp[0].minor);
}
// 4249 "parser.c"
        break;
      case 115:
// 1481 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_type(XX_TYPE_UINTEGER);
  yy_destructor(60,&yymsp[0].minor);
}
// 4257 "parser.c"
        break;
      case 116:
// 1485 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_type(XX_TYPE_LONG);
  yy_destructor(61,&yymsp[0].minor);
}
// 4265 "parser.c"
        break;
      case 117:
// 1489 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_type(XX_TYPE_ULONG);
  yy_destructor(62,&yymsp[0].minor);
}
// 4273 "parser.c"
        break;
      case 118:
// 1493 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_type(XX_TYPE_CHAR);
  yy_destructor(63,&yymsp[0].minor);
}
// 4281 "parser.c"
        break;
      case 119:
// 1497 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_type(XX_TYPE_UCHAR);
  yy_destructor(64,&yymsp[0].minor);
}
// 4289 "parser.c"
        break;
      case 120:
// 1501 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_type(XX_TYPE_DOUBLE);
  yy_destructor(65,&yymsp[0].minor);
}
// 4297 "parser.c"
        break;
      case 121:
// 1505 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_type(XX_TYPE_BOOL);
  yy_destructor(66,&yymsp[0].minor);
}
// 4305 "parser.c"
        break;
      case 122:
// 1509 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_type(XX_TYPE_STRING);
  yy_destructor(67,&yymsp[0].minor);
}
// 4313 "parser.c"
        break;
      case 123:
// 1513 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_type(XX_TYPE_ARRAY);
  yy_destructor(68,&yymsp[0].minor);
}
// 4321 "parser.c"
        break;
      case 124:
// 1517 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_type(XX_TYPE_VAR);
  yy_destructor(69,&yymsp[0].minor);
}
// 4329 "parser.c"
        break;
      case 147:
// 1609 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_empty_statement(status->scanner_state);
  yy_destructor(39,&yymsp[0].minor);
}
// 4337 "parser.c"
        break;
      case 148:
// 1613 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_break_statement(status->scanner_state);
  yy_destructor(70,&yymsp[-1].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 4346 "parser.c"
        break;
      case 149:
// 1617 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_continue_statement(status->scanner_state);
  yy_destructor(71,&yymsp[-1].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 4355 "parser.c"
        break;
      case 150:
// 1621 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_if_statement(yymsp[-2].minor.yy235, NULL, NULL, status->scanner_state);
  yy_destructor(72,&yymsp[-3].minor);
  yy_destructor(46,&yymsp[-1].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 4365 "parser.c"
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
// 4378 "parser.c"
        break;
      case 152:
// 1629 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_if_statement(yymsp[-3].minor.yy235, yymsp[-1].minor.yy235, NULL, status->scanner_state);
  yy_destructor(72,&yymsp[-4].minor);
  yy_destructor(46,&yymsp[-2].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 4388 "parser.c"
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
// 4401 "parser.c"
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
// 4414 "parser.c"
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
// 4427 "parser.c"
        break;
      case 156:
// 1645 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_switch_statement(yymsp[-2].minor.yy235, NULL, status->scanner_state);
  yy_destructor(74,&yymsp[-3].minor);
  yy_destructor(46,&yymsp[-1].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 4437 "parser.c"
        break;
      case 157:
// 1649 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_switch_statement(yymsp[-3].minor.yy235, yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(74,&yymsp[-4].minor);
  yy_destructor(46,&yymsp[-2].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 4447 "parser.c"
        break;
      case 160:
// 1661 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_case_clause(yymsp[-1].minor.yy235, NULL, status->scanner_state);
  yy_destructor(75,&yymsp[-2].minor);
  yy_destructor(76,&yymsp[0].minor);
}
// 4456 "parser.c"
        break;
      case 161:
// 1665 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_case_clause(yymsp[-2].minor.yy235, yymsp[0].minor.yy235, status->scanner_state);
  yy_destructor(75,&yymsp[-3].minor);
  yy_destructor(76,&yymsp[-1].minor);
}
// 4465 "parser.c"
        break;
      case 162:
// 1669 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_case_clause(NULL, yymsp[0].minor.yy235, status->scanner_state);
  yy_destructor(77,&yymsp[-2].minor);
  yy_destructor(76,&yymsp[-1].minor);
}
// 4474 "parser.c"
        break;
      case 163:
// 1673 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_loop_statement(NULL, status->scanner_state);
  yy_destructor(78,&yymsp[-2].minor);
  yy_destructor(46,&yymsp[-1].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 4484 "parser.c"
        break;
      case 164:
// 1677 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_loop_statement(yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(78,&yymsp[-3].minor);
  yy_destructor(46,&yymsp[-2].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 4494 "parser.c"
        break;
      case 165:
// 1681 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_while_statement(yymsp[-2].minor.yy235, NULL, status->scanner_state);
  yy_destructor(79,&yymsp[-3].minor);
  yy_destructor(46,&yymsp[-1].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 4504 "parser.c"
        break;
      case 166:
// 1685 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_while_statement(yymsp[-3].minor.yy235, yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(79,&yymsp[-4].minor);
  yy_destructor(46,&yymsp[-2].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 4514 "parser.c"
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
// 4526 "parser.c"
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
// 4538 "parser.c"
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
// 4549 "parser.c"
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
// 4561 "parser.c"
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
// 4573 "parser.c"
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
// 4586 "parser.c"
        break;
      case 173:
// 1713 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_let_statement(yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(84,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 4595 "parser.c"
        break;
      case 176:
// 1726 "parser.lemon"
{
	yygotominor.yy235 = json_object_new_string("assign");
  yy_destructor(49,&yymsp[0].minor);
}
// 4603 "parser.c"
        break;
      case 177:
// 1731 "parser.lemon"
{
	yygotominor.yy235 = json_object_new_string("add-assign");
  yy_destructor(85,&yymsp[0].minor);
}
// 4611 "parser.c"
        break;
      case 178:
// 1736 "parser.lemon"
{
	yygotominor.yy235 = json_object_new_string("sub-assign");
  yy_destructor(86,&yymsp[0].minor);
}
// 4619 "parser.c"
        break;
      case 179:
// 1740 "parser.lemon"
{
	yygotominor.yy235 = json_object_new_string("mult-assign");
  yy_destructor(87,&yymsp[0].minor);
}
// 4627 "parser.c"
        break;
      case 180:
// 1744 "parser.lemon"
{
	yygotominor.yy235 = json_object_new_string("div-assign");
  yy_destructor(88,&yymsp[0].minor);
}
// 4635 "parser.c"
        break;
      case 181:
// 1748 "parser.lemon"
{
	yygotominor.yy235 = json_object_new_string("concat-assign");
  yy_destructor(89,&yymsp[0].minor);
}
// 4643 "parser.c"
        break;
      case 182:
// 1753 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_let_assignment("variable", yymsp[-1].minor.yy235, yymsp[-2].minor.yy0, NULL, NULL, yymsp[0].minor.yy235, status->scanner_state);
}
// 4650 "parser.c"
        break;
      case 183:
// 1758 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_let_assignment("object-property", yymsp[-1].minor.yy235, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, yymsp[0].minor.yy235, status->scanner_state);
  yy_destructor(54,&yymsp[-3].minor);
}
// 4658 "parser.c"
        break;
      case 184:
// 1763 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_let_assignment("variable-dynamic-object-property", yymsp[-1].minor.yy235, yymsp[-6].minor.yy0, yymsp[-3].minor.yy0, NULL, yymsp[0].minor.yy235, status->scanner_state);
  yy_destructor(54,&yymsp[-5].minor);
  yy_destructor(46,&yymsp[-4].minor);
  yy_destructor(47,&yymsp[-2].minor);
}
// 4668 "parser.c"
        break;
      case 185:
// 1768 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_let_assignment("string-dynamic-object-property", yymsp[-1].minor.yy235, yymsp[-6].minor.yy0, yymsp[-3].minor.yy0, NULL, yymsp[0].minor.yy235, status->scanner_state);
  yy_destructor(54,&yymsp[-5].minor);
  yy_destructor(46,&yymsp[-4].minor);
  yy_destructor(47,&yymsp[-2].minor);
}
// 4678 "parser.c"
        break;
      case 186:
// 1773 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_let_assignment("object-property-append", yymsp[-1].minor.yy235, yymsp[-6].minor.yy0, yymsp[-4].minor.yy0, NULL, yymsp[0].minor.yy235, status->scanner_state);
  yy_destructor(54,&yymsp[-5].minor);
  yy_destructor(57,&yymsp[-3].minor);
  yy_destructor(58,&yymsp[-2].minor);
}
// 4688 "parser.c"
        break;
      case 187:
// 1778 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_let_assignment("object-property-array-index", yymsp[-1].minor.yy235, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, yymsp[-3].minor.yy235, yymsp[0].minor.yy235, status->scanner_state);
  yy_destructor(54,&yymsp[-6].minor);
  yy_destructor(57,&yymsp[-4].minor);
  yy_destructor(58,&yymsp[-2].minor);
}
// 4698 "parser.c"
        break;
      case 188:
// 1783 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_let_assignment("static-property", yymsp[-1].minor.yy235, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, yymsp[0].minor.yy235, status->scanner_state);
  yy_destructor(91,&yymsp[-3].minor);
}
// 4706 "parser.c"
        break;
      case 189:
// 1788 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_let_assignment("static-property-append", yymsp[-1].minor.yy235, yymsp[-6].minor.yy0, yymsp[-4].minor.yy0, NULL, yymsp[0].minor.yy235, status->scanner_state);
  yy_destructor(91,&yymsp[-5].minor);
  yy_destructor(57,&yymsp[-3].minor);
  yy_destructor(58,&yymsp[-2].minor);
}
// 4716 "parser.c"
        break;
      case 190:
// 1793 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_let_assignment("static-property-array-index", yymsp[-1].minor.yy235, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, yymsp[-3].minor.yy235, yymsp[0].minor.yy235, status->scanner_state);
  yy_destructor(91,&yymsp[-6].minor);
  yy_destructor(57,&yymsp[-4].minor);
  yy_destructor(58,&yymsp[-2].minor);
}
// 4726 "parser.c"
        break;
      case 191:
// 1798 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_let_assignment("variable-append", yymsp[-1].minor.yy235, yymsp[-4].minor.yy0, NULL, NULL, yymsp[0].minor.yy235, status->scanner_state);
  yy_destructor(57,&yymsp[-3].minor);
  yy_destructor(58,&yymsp[-2].minor);
}
// 4735 "parser.c"
        break;
      case 192:
// 1803 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_let_assignment("array-index", yymsp[-1].minor.yy235, yymsp[-5].minor.yy0, NULL, yymsp[-3].minor.yy235, yymsp[0].minor.yy235, status->scanner_state);
  yy_destructor(57,&yymsp[-4].minor);
  yy_destructor(58,&yymsp[-2].minor);
}
// 4744 "parser.c"
        break;
      case 193:
// 1808 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_let_assignment("incr", NULL, yymsp[-3].minor.yy0, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(92,&yymsp[0].minor);
}
// 4753 "parser.c"
        break;
      case 194:
// 1813 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_let_assignment("decr", NULL, yymsp[-3].minor.yy0, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(93,&yymsp[0].minor);
}
// 4762 "parser.c"
        break;
      case 195:
// 1818 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_let_assignment("incr", NULL, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(92,&yymsp[0].minor);
}
// 4770 "parser.c"
        break;
      case 196:
// 1823 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_let_assignment("decr", NULL, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(93,&yymsp[0].minor);
}
// 4778 "parser.c"
        break;
      case 198:
// 1831 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_echo_statement(yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(94,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 4787 "parser.c"
        break;
      case 201:
// 1843 "parser.lemon"
{
	yygotominor.yy235 = yymsp[0].minor.yy235;;
}
// 4794 "parser.c"
        break;
      case 202:
// 1848 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_mcall_statement(yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(39,&yymsp[0].minor);
}
// 4802 "parser.c"
        break;
      case 203:
// 1853 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_fcall_statement(yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(39,&yymsp[0].minor);
}
// 4810 "parser.c"
        break;
      case 204:
// 1858 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_scall_statement(yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(39,&yymsp[0].minor);
}
// 4818 "parser.c"
        break;
      case 205:
// 1863 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_fetch_statement(yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(39,&yymsp[0].minor);
}
// 4826 "parser.c"
        break;
      case 206:
// 1868 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_return_statement(yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(95,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 4835 "parser.c"
        break;
      case 207:
// 1873 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_return_statement(NULL, status->scanner_state);
  yy_destructor(95,&yymsp[-1].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 4844 "parser.c"
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
// 4858 "parser.c"
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
// 4873 "parser.c"
        break;
      case 210:
// 1896 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_throw_exception(yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(97,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 4882 "parser.c"
        break;
      case 211:
// 1900 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_declare_statement(XX_T_TYPE_INTEGER, yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(59,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 4891 "parser.c"
        break;
      case 212:
// 1904 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_declare_statement(XX_T_TYPE_UINTEGER, yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(60,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 4900 "parser.c"
        break;
      case 213:
// 1908 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_declare_statement(XX_T_TYPE_CHAR, yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(63,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 4909 "parser.c"
        break;
      case 214:
// 1912 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_declare_statement(XX_T_TYPE_UCHAR, yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(64,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 4918 "parser.c"
        break;
      case 215:
// 1916 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_declare_statement(XX_T_TYPE_LONG, yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(61,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 4927 "parser.c"
        break;
      case 216:
// 1920 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_declare_statement(XX_T_TYPE_ULONG, yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(62,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 4936 "parser.c"
        break;
      case 217:
// 1924 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_declare_statement(XX_T_TYPE_DOUBLE, yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(65,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 4945 "parser.c"
        break;
      case 218:
// 1928 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_declare_statement(XX_T_TYPE_STRING, yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(67,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 4954 "parser.c"
        break;
      case 219:
// 1932 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_declare_statement(XX_T_TYPE_BOOL, yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(66,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 4963 "parser.c"
        break;
      case 220:
// 1936 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_declare_statement(XX_T_TYPE_VAR, yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(69,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 4972 "parser.c"
        break;
      case 223:
// 1948 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_declare_variable(yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 4979 "parser.c"
        break;
      case 224:
// 1952 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_declare_variable(yymsp[-2].minor.yy0, yymsp[0].minor.yy235, status->scanner_state);
  yy_destructor(49,&yymsp[-1].minor);
}
// 4987 "parser.c"
        break;
      case 226:
// 1960 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_expr("not", yymsp[0].minor.yy235, NULL, NULL, status->scanner_state);
  yy_destructor(35,&yymsp[-1].minor);
}
// 4995 "parser.c"
        break;
      case 227:
// 1964 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_expr("isset", yymsp[0].minor.yy235, NULL, NULL, status->scanner_state);
  yy_destructor(31,&yymsp[-1].minor);
}
// 5003 "parser.c"
        break;
      case 228:
// 1968 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_expr("require", yymsp[0].minor.yy235, NULL, NULL, status->scanner_state);
  yy_destructor(7,&yymsp[-1].minor);
}
// 5011 "parser.c"
        break;
      case 229:
// 1972 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_expr("clone", yymsp[0].minor.yy235, NULL, NULL, status->scanner_state);
  yy_destructor(10,&yymsp[-1].minor);
}
// 5019 "parser.c"
        break;
      case 230:
// 1976 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_expr("empty", yymsp[0].minor.yy235, NULL, NULL, status->scanner_state);
  yy_destructor(11,&yymsp[-1].minor);
}
// 5027 "parser.c"
        break;
      case 231:
// 1980 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_expr("likely", yymsp[0].minor.yy235, NULL, NULL, status->scanner_state);
  yy_destructor(8,&yymsp[-1].minor);
}
// 5035 "parser.c"
        break;
      case 232:
// 1984 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_expr("unlikely", yymsp[0].minor.yy235, NULL, NULL, status->scanner_state);
  yy_destructor(9,&yymsp[-1].minor);
}
// 5043 "parser.c"
        break;
      case 233:
// 1988 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_expr("equals", yymsp[-2].minor.yy235, yymsp[0].minor.yy235, NULL, status->scanner_state);
  yy_destructor(17,&yymsp[-1].minor);
}
// 5051 "parser.c"
        break;
      case 234:
// 1992 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_expr("not-equals", yymsp[-2].minor.yy235, yymsp[0].minor.yy235, NULL, status->scanner_state);
  yy_destructor(24,&yymsp[-1].minor);
}
// 5059 "parser.c"
        break;
      case 235:
// 1996 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_expr("identical", yymsp[-2].minor.yy235, yymsp[0].minor.yy235, NULL, status->scanner_state);
  yy_destructor(18,&yymsp[-1].minor);
}
// 5067 "parser.c"
        break;
      case 236:
// 2000 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_expr("not-identical", yymsp[-2].minor.yy235, yymsp[0].minor.yy235, NULL, status->scanner_state);
  yy_destructor(23,&yymsp[-1].minor);
}
// 5075 "parser.c"
        break;
      case 237:
// 2004 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_expr("less", yymsp[-2].minor.yy235, yymsp[0].minor.yy235, NULL, status->scanner_state);
  yy_destructor(19,&yymsp[-1].minor);
}
// 5083 "parser.c"
        break;
      case 238:
// 2008 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_expr("greater", yymsp[-2].minor.yy235, yymsp[0].minor.yy235, NULL, status->scanner_state);
  yy_destructor(20,&yymsp[-1].minor);
}
// 5091 "parser.c"
        break;
      case 239:
// 2012 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_expr("less-equal", yymsp[-2].minor.yy235, yymsp[0].minor.yy235, NULL, status->scanner_state);
  yy_destructor(21,&yymsp[-1].minor);
}
// 5099 "parser.c"
        break;
      case 240:
// 2016 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_expr("greater-equal", yymsp[-2].minor.yy235, yymsp[0].minor.yy235, NULL, status->scanner_state);
  yy_destructor(22,&yymsp[-1].minor);
}
// 5107 "parser.c"
        break;
      case 241:
// 2020 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_expr("list", yymsp[-1].minor.yy235, NULL, NULL, status->scanner_state);
  yy_destructor(53,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5116 "parser.c"
        break;
      case 242:
// 2024 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_expr("cast", yymsp[-2].minor.yy235, yymsp[0].minor.yy235, NULL, status->scanner_state);
  yy_destructor(53,&yymsp[-3].minor);
  yy_destructor(36,&yymsp[-1].minor);
}
// 5125 "parser.c"
        break;
      case 243:
// 2028 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_expr("type-hint", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), yymsp[0].minor.yy235, NULL, status->scanner_state);
  yy_destructor(19,&yymsp[-3].minor);
  yy_destructor(20,&yymsp[-1].minor);
}
// 5134 "parser.c"
        break;
      case 244:
      case 318:
// 2032 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_expr("property-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(54,&yymsp[-1].minor);
}
// 5143 "parser.c"
        break;
      case 245:
      case 319:
// 2036 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_expr("property-dynamic-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(54,&yymsp[-3].minor);
  yy_destructor(46,&yymsp[-2].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 5154 "parser.c"
        break;
      case 246:
      case 320:
// 2040 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_expr("property-string-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_STRING, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(54,&yymsp[-3].minor);
  yy_destructor(46,&yymsp[-2].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 5165 "parser.c"
        break;
      case 247:
// 2044 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_expr("static-property-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(91,&yymsp[-1].minor);
}
// 5173 "parser.c"
        break;
      case 248:
      case 316:
// 2048 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_expr("static-constant-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(91,&yymsp[-1].minor);
}
// 5182 "parser.c"
        break;
      case 249:
      case 317:
// 2053 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), yymsp[-1].minor.yy235, NULL, status->scanner_state);
  yy_destructor(57,&yymsp[-2].minor);
  yy_destructor(58,&yymsp[0].minor);
}
// 5192 "parser.c"
        break;
      case 250:
// 2058 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_expr("add", yymsp[-2].minor.yy235, yymsp[0].minor.yy235, NULL, status->scanner_state);
  yy_destructor(25,&yymsp[-1].minor);
}
// 5200 "parser.c"
        break;
      case 251:
// 2063 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_expr("sub", yymsp[-2].minor.yy235, yymsp[0].minor.yy235, NULL, status->scanner_state);
  yy_destructor(26,&yymsp[-1].minor);
}
// 5208 "parser.c"
        break;
      case 252:
// 2068 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_expr("mul", yymsp[-2].minor.yy235, yymsp[0].minor.yy235, NULL, status->scanner_state);
  yy_destructor(28,&yymsp[-1].minor);
}
// 5216 "parser.c"
        break;
      case 253:
// 2073 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_expr("div", yymsp[-2].minor.yy235, yymsp[0].minor.yy235, NULL, status->scanner_state);
  yy_destructor(29,&yymsp[-1].minor);
}
// 5224 "parser.c"
        break;
      case 254:
// 2078 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_expr("mod", yymsp[-2].minor.yy235, yymsp[0].minor.yy235, NULL, status->scanner_state);
  yy_destructor(30,&yymsp[-1].minor);
}
// 5232 "parser.c"
        break;
      case 255:
// 2083 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_expr("concat", yymsp[-2].minor.yy235, yymsp[0].minor.yy235, NULL, status->scanner_state);
  yy_destructor(27,&yymsp[-1].minor);
}
// 5240 "parser.c"
        break;
      case 256:
// 2088 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_expr("and", yymsp[-2].minor.yy235, yymsp[0].minor.yy235, NULL, status->scanner_state);
  yy_destructor(13,&yymsp[-1].minor);
}
// 5248 "parser.c"
        break;
      case 257:
// 2093 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_expr("or", yymsp[-2].minor.yy235, yymsp[0].minor.yy235, NULL, status->scanner_state);
  yy_destructor(14,&yymsp[-1].minor);
}
// 5256 "parser.c"
        break;
      case 258:
// 2098 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_expr("instanceof", yymsp[-2].minor.yy235, yymsp[0].minor.yy235, NULL, status->scanner_state);
  yy_destructor(12,&yymsp[-1].minor);
}
// 5264 "parser.c"
        break;
      case 259:
// 2103 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_expr("fetch", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), yymsp[0].minor.yy235, NULL, status->scanner_state);
  yy_destructor(32,&yymsp[-3].minor);
  yy_destructor(6,&yymsp[-1].minor);
}
// 5273 "parser.c"
        break;
      case 261:
// 2113 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_expr("typeof", yymsp[0].minor.yy235, NULL, NULL, status->scanner_state);
  yy_destructor(33,&yymsp[-1].minor);
}
// 5281 "parser.c"
        break;
      case 262:
// 2118 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_literal(XX_T_CONSTANT, yymsp[0].minor.yy0, status->scanner_state);
}
// 5288 "parser.c"
        break;
      case 264:
      case 307:
      case 309:
// 2128 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_literal(XX_T_INTEGER, yymsp[0].minor.yy0, status->scanner_state);
}
// 5297 "parser.c"
        break;
      case 265:
      case 306:
      case 311:
// 2133 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_literal(XX_T_STRING, yymsp[0].minor.yy0, status->scanner_state);
}
// 5306 "parser.c"
        break;
      case 266:
      case 310:
// 2138 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_literal(XX_T_CHAR, yymsp[0].minor.yy0, status->scanner_state);
}
// 5314 "parser.c"
        break;
      case 267:
      case 312:
// 2143 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_literal(XX_T_DOUBLE, yymsp[0].minor.yy0, status->scanner_state);
}
// 5322 "parser.c"
        break;
      case 268:
      case 313:
// 2148 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_literal(XX_T_NULL, NULL, status->scanner_state);
  yy_destructor(101,&yymsp[0].minor);
}
// 5331 "parser.c"
        break;
      case 269:
      case 315:
// 2153 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_literal(XX_T_TRUE, NULL, status->scanner_state);
  yy_destructor(102,&yymsp[0].minor);
}
// 5340 "parser.c"
        break;
      case 270:
      case 314:
// 2158 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_literal(XX_T_FALSE, NULL, status->scanner_state);
  yy_destructor(103,&yymsp[0].minor);
}
// 5349 "parser.c"
        break;
      case 271:
// 2163 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_expr("empty-array", NULL, NULL, NULL, status->scanner_state);
  yy_destructor(57,&yymsp[-1].minor);
  yy_destructor(58,&yymsp[0].minor);
}
// 5358 "parser.c"
        break;
      case 272:
// 2168 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_expr("array", yymsp[-1].minor.yy235, NULL, NULL, status->scanner_state);
  yy_destructor(57,&yymsp[-2].minor);
  yy_destructor(58,&yymsp[0].minor);
}
// 5367 "parser.c"
        break;
      case 273:
// 2173 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_new_instance(0, yymsp[0].minor.yy0, NULL, status->scanner_state);
  yy_destructor(34,&yymsp[-1].minor);
}
// 5375 "parser.c"
        break;
      case 274:
// 2178 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_new_instance(0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(34,&yymsp[-3].minor);
  yy_destructor(53,&yymsp[-1].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5385 "parser.c"
        break;
      case 275:
// 2183 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_new_instance(0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(34,&yymsp[-4].minor);
  yy_destructor(53,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5395 "parser.c"
        break;
      case 276:
// 2188 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_new_instance(1, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(34,&yymsp[-3].minor);
  yy_destructor(46,&yymsp[-2].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 5405 "parser.c"
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
// 5417 "parser.c"
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
// 5429 "parser.c"
        break;
      case 279:
// 2203 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_fcall(1, yymsp[-3].minor.yy0, yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(53,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5438 "parser.c"
        break;
      case 280:
// 2208 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_fcall(1, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(53,&yymsp[-1].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5447 "parser.c"
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
// 5458 "parser.c"
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
// 5469 "parser.c"
        break;
      case 283:
// 2223 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_scall(0, yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(91,&yymsp[-4].minor);
  yy_destructor(53,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5479 "parser.c"
        break;
      case 284:
// 2228 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_scall(0, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(91,&yymsp[-3].minor);
  yy_destructor(53,&yymsp[-1].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5489 "parser.c"
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
// 5501 "parser.c"
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
// 5513 "parser.c"
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
// 5527 "parser.c"
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
// 5541 "parser.c"
        break;
      case 289:
// 2253 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_mcall(1, yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(54,&yymsp[-4].minor);
  yy_destructor(53,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5551 "parser.c"
        break;
      case 290:
// 2258 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_mcall(1, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(54,&yymsp[-3].minor);
  yy_destructor(53,&yymsp[-1].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5561 "parser.c"
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
// 5573 "parser.c"
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
// 5585 "parser.c"
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
// 5597 "parser.c"
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
// 5609 "parser.c"
        break;
      case 303:
// 2317 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_array_item(yymsp[-2].minor.yy235, yymsp[0].minor.yy235, status->scanner_state);
  yy_destructor(76,&yymsp[-1].minor);
}
// 5617 "parser.c"
        break;
      case 304:
// 2321 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_array_item(NULL, yymsp[0].minor.yy235, status->scanner_state);
}
// 5624 "parser.c"
        break;
      case 322:
// 2397 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_comment(yymsp[0].minor.yy0, status->scanner_state);
}
// 5631 "parser.c"
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

// 5707 "parser.c"
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
