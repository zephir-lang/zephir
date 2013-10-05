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
#define YYNOCODE 185
#define YYACTIONTYPE unsigned short int
#define xx_TOKENTYPE xx_parser_token*
typedef union {
  xx_TOKENTYPE yy0;
  json_object* yy239;
  int yy369;
} YYMINORTYPE;
#define YYSTACKDEPTH 100
#define xx_ARG_SDECL xx_parser_status *status;
#define xx_ARG_PDECL ,xx_parser_status *status
#define xx_ARG_FETCH xx_parser_status *status = yypParser->status
#define xx_ARG_STORE yypParser->status = status
#define YYNSTATE 685
#define YYNRULE 322
#define YYERRORSYMBOL 104
#define YYERRSYMDT yy369
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
 /*     0 */   210,  219,  222,  213,  216,  230,  232,  240,  234,  236,
 /*    10 */   238,  660,  254, 1008,    1,    2,  684,    4,    5,    6,
 /*    20 */     7,   76,   84,   62,  206,  263,  268,  284,  203,  127,
 /*    30 */   390,  258,  129,  653,  659,  658,  642,  132,  403,  291,
 /*    40 */   634,  652,  608,  131,  270,  415,  248,  114,  114,  172,
 /*    50 */   278,  125,   42,   43,   44,   45,   46,   47,   48,   49,
 /*    60 */    50,   51,   52,  655,  620,  642,  391,  392,  393,  394,
 /*    70 */   395,  608,  263,  412,  413,  624,  114,  371,  513,  162,
 /*    80 */   636,  633,  630,  272,    9,  625,  291,  171,  523,  631,
 /*    90 */   637,  271,  273,  274,  275,  276,  277,  419,  636,  477,
 /*   100 */   486,  495,  498,  489,  492,  501,  507,  504,  637,  510,
 /*   110 */   163,  165,  167,  665,  176,  663,  263,  227,  187,  191,
 /*   120 */   196,  377,  513,  162,  384,  298,  300,  299,  267,  373,
 /*   130 */   291,  529,  523,  294,  446,  461,  465,  474,    3,    4,
 /*   140 */     5,    6,    7,  477,  486,  495,  498,  489,  492,  501,
 /*   150 */   507,  504,   14,  510,  163,  165,  167,   17,  176,  234,
 /*   160 */   236,  238,  187,  191,  196,  377,  565,  122,  384,  263,
 /*   170 */   125,  302,   76,   84,   88,  513,  162,  449,  446,  461,
 /*   180 */   465,  474,  451,  291,  576,  523,  137,  400,  447,  452,
 /*   190 */   298,  300,  299,  267,  559,  414,  477,  486,  495,  498,
 /*   200 */   489,  492,  501,  507,  504,  290,  510,  163,  165,  167,
 /*   210 */   448,  176,   10,  263,  235,  187,  191,  196,  377,  513,
 /*   220 */   162,  384,  298,  300,  299,  267,  685,  291,  590,  523,
 /*   230 */    13,  446,  461,  465,  474,  289,  330,  516,  478,  485,
 /*   240 */   477,  486,  495,  498,  489,  492,  501,  507,  504,   61,
 /*   250 */   510,  163,  165,  167,  654,  176,  657,  658,  642,  187,
 /*   260 */   191,  196,  377,  607,  608,  384,  263,   12,  467,  114,
 /*   270 */    17,  470,  513,  162,  514,  446,  461,  465,  474,  109,
 /*   280 */   291,  547,  523,   16,  560,  251,   31,   53,   34,   36,
 /*   290 */    33,   34,   36,  477,  486,  495,  498,  489,  492,  501,
 /*   300 */   507,  504,  480,  510,  163,  165,  167,  123,  176,  663,
 /*   310 */   263,  249,  187,  191,  196,  377,  513,  162,  384,  298,
 /*   320 */   300,  299,  267,   15,  291,  383,  523,   89,  446,  461,
 /*   330 */   465,  474,  675,  368,  663,  479,  209,  477,  486,  495,
 /*   340 */   498,  489,  492,  501,  507,  504,  390,  510,  163,  165,
 /*   350 */   167,   24,  176,  606,  427,  132,  187,  191,  196,  377,
 /*   360 */   607,  608,  384,  263,  385,  445,  114,  180,  551,  513,
 /*   370 */   162,   25,  446,  461,  465,  474,  179,  291,  553,  523,
 /*   380 */   487,  485,  391,  392,  393,  394,  395,  490,  485,   29,
 /*   390 */   477,  486,  495,  498,  489,  492,  501,  507,  504,  387,
 /*   400 */   510,  163,  165,  167,  183,  176,  548,  263,  471,  187,
 /*   410 */   191,  196,  377,  513,  162,  384,  298,  300,  299,  267,
 /*   420 */   181,  291,  558,  523,  647,  446,  461,  465,  474,  493,
 /*   430 */   485,  630,  386,   32,  477,  486,  495,  498,  489,  492,
 /*   440 */   501,  507,  504,  390,  510,  163,  165,  167,  183,  176,
 /*   450 */   548,  644,  530,  187,  191,  196,  377,  593,  630,  384,
 /*   460 */   263,  645,   99,   76,   84,   88,  513,  162,  480,  446,
 /*   470 */   461,  465,  474,  114,  291,  541,  523,   35,   55,  391,
 /*   480 */   392,  393,  394,  395,   76,   84,   88,  477,  486,  495,
 /*   490 */   498,  489,  492,  501,  507,  504,  480,  510,  163,  165,
 /*   500 */   167,  488,  176,   38,  263,  533,  187,  191,  196,  377,
 /*   510 */   513,  162,  384,  298,  300,  299,  267,   37,  291,  570,
 /*   520 */   523,   40,  446,  461,  465,  474,  480,   41,  379,  491,
 /*   530 */   107,  477,  486,  495,  498,  489,  492,  501,  507,  504,
 /*   540 */   369,  510,  163,  165,  167,  104,  176,  285,  370,  480,
 /*   550 */   187,  191,  196,  377,  108,  316,  384,  263,  627,  494,
 /*   560 */    76,   84,   88,  513,  162,  630,  446,  461,  465,  474,
 /*   570 */   290,  291,  140,  523,  572,  350,   31,   53,   34,   36,
 /*   580 */   496,  485,  497,   54,  477,  486,  495,  498,  489,  492,
 /*   590 */   501,  507,  504,   59,  510,  163,  165,  167,   27,  176,
 /*   600 */   360,  263,  237,  187,  191,  196,  377,  513,  162,  384,
 /*   610 */   298,  300,  299,  267,   64,  291,  574,  523,   65,  446,
 /*   620 */   461,  465,  474,  499,  485,  519,  290,  356,  477,  486,
 /*   630 */   495,  498,  489,  492,  501,  507,  504,  290,  510,  163,
 /*   640 */   165,  167,   74,  176,   22,  480,  480,  187,  191,  196,
 /*   650 */   377,   21,   78,  384,  263,  114,  354,  671,   75,  674,
 /*   660 */   513,  162,  125,  446,  461,  465,  474,  347,  291,  190,
 /*   670 */   523,  588,  517,   31,   53,   34,   36,  521,  500,  503,
 /*   680 */   518,  477,  486,  495,  498,  489,  492,  501,  507,  504,
 /*   690 */   681,  510,  163,  165,  167,  125,  176,   79,  263,  231,
 /*   700 */   187,  191,  196,  377,  513,  162,  384,  298,  300,  299,
 /*   710 */   267,  290,  291,  552,  523,  480,  446,  461,  465,  474,
 /*   720 */   502,  485,  505,  485,   81,  477,  486,  495,  498,  489,
 /*   730 */   492,  501,  507,  504,  290,  510,  163,  165,  167,  282,
 /*   740 */   176,  332,  508,  485,  187,  191,  196,  377,  506,   82,
 /*   750 */   384,  263,  326,   83,  119,   85,  664,  513,  162,  125,
 /*   760 */   446,  461,  465,  474,  324,  291,  586,  523,  111,   86,
 /*   770 */    31,   53,   34,   36,   87,  480,  511,  485,  477,  486,
 /*   780 */   495,  498,  489,  492,  501,  507,  504,  480,  510,  163,
 /*   790 */   165,  167,  101,  176,  339,  263,  214,  187,  191,  196,
 /*   800 */   377,  513,  162,  384,  298,  300,  299,  267,  509,  291,
 /*   810 */   542,  523,  102,  446,  461,  465,  474,  632,  639,  106,
 /*   820 */   512,  290,  477,  486,  495,  498,  489,  492,  501,  507,
 /*   830 */   504,   61,  510,  163,  165,  167,   80,  176,  578,   77,
 /*   840 */   117,  187,  191,  196,  377,  567,  338,  384,  263,  114,
 /*   850 */   112,  322,  118,  571,  513,  162,   60,  446,  461,  465,
 /*   860 */   474,  566,  291,  175,  523,  105,  303,   31,   53,   34,
 /*   870 */    36,   57,  120,  662,  308,  477,  486,  495,  498,  489,
 /*   880 */   492,  501,  507,  504,   61,  510,  163,  165,  167,  121,
 /*   890 */   176,  124,  263,  366,  187,  191,  196,  377,  513,  162,
 /*   900 */   384,  298,  300,  299,  267,  290,  291,  199,  523,  583,
 /*   910 */   446,  461,  465,  474,   56,  128,   61,  587,  134,  477,
 /*   920 */   486,  495,  498,  489,  492,  501,  507,  504,  595,  510,
 /*   930 */   163,  165,  167,  290,  176,  313,  599,  622,  187,  191,
 /*   940 */   196,  377,  260,  135,  384,  263,  594,  164,  114,  169,
 /*   950 */   349,  513,  162,  113,  446,  461,  465,  474,  290,  291,
 /*   960 */   598,  523,  600,  306,   31,   53,   34,   36,  110,  166,
 /*   970 */   182,  173,  477,  486,  495,  498,  489,  492,  501,  507,
 /*   980 */   504,  178,  510,  163,  165,  167,  184,  176,  296,  263,
 /*   990 */   362,  187,  191,  196,  377,  513,  162,  384,  298,  300,
 /*  1000 */   299,  267,  188,  291,  584,  523,  193,  446,  461,  465,
 /*  1010 */   474,  197,  185,  200,  202,  208,  477,  486,  495,  498,
 /*  1020 */   489,  492,  501,  507,  504,  207,  510,  163,  165,  167,
 /*  1030 */   252,  176,  256,  255,  261,  187,  191,  196,  377,  264,
 /*  1040 */   265,  384,  263,  266,  283,  286,  292,  293,  513,  162,
 /*  1050 */   304,  446,  461,  465,  474,  309,  291,  602,  523,   28,
 /*  1060 */   310,   31,   53,   34,   36,  317,  318,  311,  319,  477,
 /*  1070 */   486,  495,  498,  489,  492,  501,  507,  504,  991,  510,
 /*  1080 */   163,  165,  167,  990,  176,  342,  263,  365,  187,  191,
 /*  1090 */   196,  377,  513,  162,  384,  298,  300,  299,  267,  351,
 /*  1100 */   291,  536,  523,  352,  446,  461,  465,  474,  357,  358,
 /*  1110 */   372,  374,  378,  477,  486,  495,  498,  489,  492,  501,
 /*  1120 */   507,  504,  389,  510,  163,  165,  167,  388,  176,  396,
 /*  1130 */   401,  408,  187,  191,  196,  377,  405,  409,  384,  263,
 /*  1140 */   416,  420,  417,  424,  421,  513,  162,  432,  446,  461,
 /*  1150 */   465,  474,  425,  291,  568,  523,   58,  440,   31,   53,
 /*  1160 */    34,   36,  429,  454,  433,  437,  477,  486,  495,  498,
 /*  1170 */   489,  492,  501,  507,  504,  456,  510,  163,  165,  167,
 /*  1180 */   458,  176,  441,  263,  211,  187,  191,  196,  377,  513,
 /*  1190 */   162,  384,  298,  300,  299,  267,  460,  291,  546,  523,
 /*  1200 */   466,  446,  461,  465,  474,  468,  481,  469,  473,  482,
 /*  1210 */   477,  486,  495,  498,  489,  492,  501,  507,  504,  483,
 /*  1220 */   510,  163,  165,  167,  484,  176,  515,  520,  522,  187,
 /*  1230 */   191,  196,  377,  531,  532,  384,  263,  543,  545,  549,
 /*  1240 */   555,  580,  513,  162,  561,  446,  461,  465,  474,  554,
 /*  1250 */   291,  604,  523,  573,  581,  330,  259,  589,  601,  327,
 /*  1260 */   611,  613,  612,  477,  486,  495,  498,  489,  492,  501,
 /*  1270 */   507,  504,  614,  510,  163,  165,  167,  616,  176,  617,
 /*  1280 */   263,  328,  187,  191,  196,  377,  513,  162,  384,  298,
 /*  1290 */   300,  299,  267,  281,  291,  564,  523,  618,  446,  461,
 /*  1300 */   465,  474,  619,  640,  628,  629,  626,  477,  486,  495,
 /*  1310 */   498,  489,  492,  501,  507,  504,  731,  510,  163,  165,
 /*  1320 */   167,  732,  176,  635,  638,  650,  187,  191,  196,  377,
 /*  1330 */   641,  648,  384,  263,  646,  649,  651,  661,  666,  513,
 /*  1340 */   162,   19,  446,  461,  465,  474,   22,  291,  592,  523,
 /*  1350 */   668,  669,  670,  115,  672,  125,  673,  114,  676,  678,
 /*  1360 */   477,  486,  495,  498,  489,  492,  501,  507,  504,  679,
 /*  1370 */   510,  163,  165,  167,  680,  176,  682,  263,  462,  187,
 /*  1380 */   191,  196,  377,  513,  162,  384,  298,  300,  299,  267,
 /*  1390 */   683,  291,  596,  523,  508,  446,  461,  465,  474,  508,
 /*  1400 */   508,  508,  508,  508,  477,  486,  495,  498,  489,  492,
 /*  1410 */   501,  507,  504,  508,  510,  163,  165,  167,  508,  176,
 /*  1420 */   508,  508,  508,  187,  191,  196,  377,  508,  508,  384,
 /*  1430 */   263,  508,  508,  508,  508,  508,  513,  162,  508,  446,
 /*  1440 */   461,  465,  474,  508,  291,  562,  523,  508,  508,  508,
 /*  1450 */   508,  508,  508,  508,  508,  508,  508,  477,  486,  495,
 /*  1460 */   498,  489,  492,  501,  507,  504,  508,  510,  163,  165,
 /*  1470 */   167,  508,  176,  508,  263,  380,  187,  191,  196,  377,
 /*  1480 */   513,  162,  384,  298,  300,  299,  267,  508,  291,  557,
 /*  1490 */   523,  508,  446,  461,  465,  474,  508,  508,  508,  508,
 /*  1500 */   508,  477,  486,  495,  498,  489,  492,  501,  507,  504,
 /*  1510 */   508,  510,  163,  165,  167,  508,  176,  508,  508,  508,
 /*  1520 */   187,  191,  196,  377,  508,  508,  384,  263,  508,  508,
 /*  1530 */   508,  508,  508,  513,  162,  508,  446,  461,  465,  474,
 /*  1540 */   508,  291,  195,  523,  508,  508,  508,  508,  508,  508,
 /*  1550 */   508,  508,  508,  508,  477,  486,  495,  498,  489,  492,
 /*  1560 */   501,  507,  504,  508,  510,  163,  165,  167,  508,  176,
 /*  1570 */   508,  263,  269,  187,  191,  196,  377,  513,  162,  384,
 /*  1580 */   298,  300,  299,  267,  508,  291,  138,  523,  508,  446,
 /*  1590 */   461,  465,  474,  508,  508,  508,  508,  508,  477,  486,
 /*  1600 */   495,  498,  489,  492,  501,  507,  504,  508,  510,  163,
 /*  1610 */   165,  167,  508,  176,  508,  508,  508,  187,  191,  196,
 /*  1620 */   377,  508,  508,  384,  263,   90,   91,   93,   92,   94,
 /*  1630 */   513,  162,  508,  446,  461,  465,  474,  508,  291,    8,
 /*  1640 */   523,  508,   11,  508,  116,  508,  667,  677,  508,  508,
 /*  1650 */   523,  477,  486,  495,  498,  489,  492,  501,  507,  504,
 /*  1660 */   508,  510,  163,  165,  167,  508,  176,  508,   96,   97,
 /*  1670 */   187,  191,  196,  377,  160,  508,  384,  508,  508,   95,
 /*  1680 */   508,  508,  508,  508,  508,  508,  446,  461,  465,  474,
 /*  1690 */   451,  508,  508,  508,  535,  508,  508,  450,  298,  300,
 /*  1700 */   299,  267,  524,  142,  143,  144,  145,  146,  147,  148,
 /*  1710 */   149,  150,  151,  152,  153,  154,  155,  156,  157,  158,
 /*  1720 */   159,  161,  160,  508,  508,  508,  508,  508,  508,  279,
 /*  1730 */   508,  508,  508,  453,  455,  457,  459,  298,  300,  299,
 /*  1740 */   267,  508,  597,  508,  337,  508,  508,  345,  341,  344,
 /*  1750 */   524,  142,  143,  144,  145,  146,  147,  148,  149,  150,
 /*  1760 */   151,  152,  153,  154,  155,  156,  157,  158,  159,  161,
 /*  1770 */   160,  508,  508,  508,  508,  508,  508,  279,  508,  508,
 /*  1780 */   508,  453,  455,  457,  459,  298,  300,  299,  267,  508,
 /*  1790 */   540,  508,  508,  508,  508,  340,  341,  344,  524,  142,
 /*  1800 */   143,  144,  145,  146,  147,  148,  149,  150,  151,  152,
 /*  1810 */   153,  154,  155,  156,  157,  158,  159,  161,  160,  508,
 /*  1820 */   508,  508,  508,  508,  508,  262,  508,  508,  508,  453,
 /*  1830 */   455,  457,  459,  298,  300,  299,  267,  508,  591,  508,
 /*  1840 */   508,  346,  301,  508,  508,  508,  524,  142,  143,  144,
 /*  1850 */   145,  146,  147,  148,  149,  150,  151,  152,  153,  154,
 /*  1860 */   155,  156,  157,  158,  159,  161,  160,  508,  508,  508,
 /*  1870 */   508,  508,  508,  262,  508,  508,  508,  453,  455,  457,
 /*  1880 */   459,  298,  300,  299,  267,  508,  139,  508,  508,  305,
 /*  1890 */   301,  508,  508,  508,  524,  142,  143,  144,  145,  146,
 /*  1900 */   147,  148,  149,  150,  151,  152,  153,  154,  155,  156,
 /*  1910 */   157,  158,  159,  161,  160,  508,  508,  508,  508,  508,
 /*  1920 */   508,  605,  130,  620,  642,  453,  455,  457,  459,  652,
 /*  1930 */   608,  508,  508,  508,  585,  114,  508,  508,  508,  508,
 /*  1940 */   508,  508,  524,  142,  143,  144,  145,  146,  147,  148,
 /*  1950 */   149,  150,  151,  152,  153,  154,  155,  156,  157,  158,
 /*  1960 */   159,  161,  160,  508,  508,  508,  508,  508,  508,  262,
 /*  1970 */   508,  508,  508,  453,  455,  457,  459,  298,  300,  299,
 /*  1980 */   267,  508,  186,  508,  508,  312,  301,  508,  508,  508,
 /*  1990 */   524,  142,  143,  144,  145,  146,  147,  148,  149,  150,
 /*  2000 */   151,  152,  153,  154,  155,  156,  157,  158,  159,  161,
 /*  2010 */   160,  508,  508,  508,  508,  508,  508,  262,  508,  508,
 /*  2020 */   508,  453,  455,  457,  459,  298,  300,  299,  267,  508,
 /*  2030 */   556,  508,  508,  359,  301,  508,  508,  508,  524,  142,
 /*  2040 */   143,  144,  145,  146,  147,  148,  149,  150,  151,  152,
 /*  2050 */   153,  154,  155,  156,  157,  158,  159,  161,  160,  508,
 /*  2060 */   508,  508,  508,  508,  508,  262,  508,  508,  508,  453,
 /*  2070 */   455,  457,  459,  298,  300,  299,  267,  508,  563,  508,
 /*  2080 */   508,  295,  301,  508,  508,  508,  524,  142,  143,  144,
 /*  2090 */   145,  146,  147,  148,  149,  150,  151,  152,  153,  154,
 /*  2100 */   155,  156,  157,  158,  159,  161,  160,  508,  508,  508,
 /*  2110 */   508,  508,  508,  262,  508,  508,  508,  453,  455,  457,
 /*  2120 */   459,  298,  300,  299,  267,  508,  550,  508,  508,  288,
 /*  2130 */   301,  508,  508,  508,  524,  142,  143,  144,  145,  146,
 /*  2140 */   147,  148,  149,  150,  151,  152,  153,  154,  155,  156,
 /*  2150 */   157,  158,  159,  161,  160,  508,  508,  508,  508,  508,
 /*  2160 */   508,  262,  508,  508,  508,  453,  455,  457,  459,  298,
 /*  2170 */   300,  299,  267,  508,  170,  508,  508,  321,  301,  508,
 /*  2180 */   508,  508,  524,  142,  143,  144,  145,  146,  147,  148,
 /*  2190 */   149,  150,  151,  152,  153,  154,  155,  156,  157,  158,
 /*  2200 */   159,  161,  160,  508,  508,  508,  508,  508,  508,  262,
 /*  2210 */   508,  508,  508,  453,  455,  457,  459,  298,  300,  299,
 /*  2220 */   267,  508,  382,  508,  508,  323,  301,  508,  508,  508,
 /*  2230 */   524,  142,  143,  144,  145,  146,  147,  148,  149,  150,
 /*  2240 */   151,  152,  153,  154,  155,  156,  157,  158,  159,  161,
 /*  2250 */   160,  508,  508,  508,  508,  508,  508,  262,  508,  508,
 /*  2260 */   508,  453,  455,  457,  459,  298,  300,  299,  267,  508,
 /*  2270 */   575,  508,  508,  331,  301,  508,  508,  508,  524,  142,
 /*  2280 */   143,  144,  145,  146,  147,  148,  149,  150,  151,  152,
 /*  2290 */   153,  154,  155,  156,  157,  158,  159,  161,  160,  508,
 /*  2300 */   508,  508,  508,  508,  508,  262,  508,  508,  508,  453,
 /*  2310 */   455,  457,  459,  298,  300,  299,  267,  508,  174,  508,
 /*  2320 */   508,  353,  301,  508,  508,  508,  524,  142,  143,  144,
 /*  2330 */   145,  146,  147,  148,  149,  150,  151,  152,  153,  154,
 /*  2340 */   155,  156,  157,  158,  159,  161,  160,  508,  508,  508,
 /*  2350 */   192,  508,  508,  376,  508,  508,  508,  453,  455,  457,
 /*  2360 */   459,  298,  300,  299,  267,  508,  198,  508,  508,  508,
 /*  2370 */   508,  508,  508,  508,  524,  142,  143,  144,  145,  146,
 /*  2380 */   147,  148,  149,  150,  151,  152,  153,  154,  155,  156,
 /*  2390 */   157,  158,  159,  161,  160,  508,  508,  508,  544,  508,
 /*  2400 */   508,  376,  508,  508,  508,  453,  455,  457,  459,  298,
 /*  2410 */   300,  299,  267,  508,  528,  508,  508,  508,  508,  508,
 /*  2420 */   508,  508,  524,  142,  143,  144,  145,  146,  147,  148,
 /*  2430 */   149,  150,  151,  152,  153,  154,  155,  156,  157,  158,
 /*  2440 */   159,  161,  160,  508,  508,  508,  508,  508,  508,  508,
 /*  2450 */   508,  508,  508,  453,  455,  457,  459,  398,  508,  508,
 /*  2460 */   508,  442,  189,  508,  508,  298,  300,  299,  267,  508,
 /*  2470 */   524,  142,  143,  144,  145,  146,  147,  148,  149,  150,
 /*  2480 */   151,  152,  153,  154,  155,  156,  157,  158,  159,  161,
 /*  2490 */   160,  508,  508,  508,  508,  508,  508,  508,  508,  508,
 /*  2500 */   508,  453,  455,  457,  459,  398,  508,  508,  508,  438,
 /*  2510 */   569,  508,  508,  298,  300,  299,  267,  508,  524,  142,
 /*  2520 */   143,  144,  145,  146,  147,  148,  149,  150,  151,  152,
 /*  2530 */   153,  154,  155,  156,  157,  158,  159,  161,  160,  508,
 /*  2540 */   508,  508,  508,  508,  508,  508,  508,  508,  508,  453,
 /*  2550 */   455,  457,  459,  508,  411,  508,  508,  508,  603,  439,
 /*  2560 */   508,  508,  298,  300,  299,  267,  524,  142,  143,  144,
 /*  2570 */   145,  146,  147,  148,  149,  150,  151,  152,  153,  154,
 /*  2580 */   155,  156,  157,  158,  159,  161,  160,  508,  508,   73,
 /*  2590 */   508,  508,  508,  508,  508,  508,  508,  453,  455,  457,
 /*  2600 */   459,  398,  508,  508,  508,  434,  194,  508,  508,  298,
 /*  2610 */   300,  299,  267,  508,  524,  142,  143,  144,  145,  146,
 /*  2620 */   147,  148,  149,  150,  151,  152,  153,  154,  155,  156,
 /*  2630 */   157,  158,  159,  161,  160,  508,  508,  508,  508,  508,
 /*  2640 */   508,   68,  508,  508,  508,  453,  455,  457,  459,   66,
 /*  2650 */    67,   69,   70,   72,   71,  508,  508,  508,  508,  508,
 /*  2660 */   508,  508,  141,  142,  143,  144,  145,  146,  147,  148,
 /*  2670 */   149,  150,  151,  152,  153,  154,  155,  156,  157,  158,
 /*  2680 */   159,  161,  508,  508,  508,  210,  219,  222,  213,  216,
 /*  2690 */   262,  508,  508,  453,  455,  457,  459,  254,  298,  300,
 /*  2700 */   299,  267,  330,  259,  508,  508,  327,  315,  508,  206,
 /*  2710 */   263,  268,  284,  203,  361,  508,  258,   39,  508,  508,
 /*  2720 */   508,  508,  508,  508,  291,  989,  508,  508,  508,  270,
 /*  2730 */   508,  248,  508,  508,  103,  278,   63,  246,  242,  244,
 /*  2740 */   281,  508,  205,  215,  221,  224,  226,  228,  218,  212,
 /*  2750 */   230,  232,  240,  234,  236,  238,  508,   42,   43,   44,
 /*  2760 */    45,   46,   47,   48,   49,   50,   51,   52,  272,  508,
 /*  2770 */   508,  508,  508,  508,  508,  508,  271,  273,  274,  275,
 /*  2780 */   276,  277,  508,  472,  210,  219,  222,  213,  216,  398,
 /*  2790 */   508,  508,  508,  430,  279,  508,  254,  298,  300,  299,
 /*  2800 */   267,  508,  298,  300,  299,  267,  508,  508,  206,  263,
 /*  2810 */   268,  284,  203,  320,  343,  258,   39,  508,  508,  508,
 /*  2820 */   508,  508,  508,  291,  508,  508,  508,  508,  270,  508,
 /*  2830 */   248,  508,  508,  582,  278,   63,  246,  242,  244,  508,
 /*  2840 */   508,  205,  215,  221,  224,  226,  228,  218,  212,  230,
 /*  2850 */   232,  240,  234,  236,  238,  508,   42,   43,   44,   45,
 /*  2860 */    46,   47,   48,   49,   50,   51,   52,  272,  508,  508,
 /*  2870 */   527,  508,  508,  508,  508,  271,  273,  274,  275,  276,
 /*  2880 */   277,  223,  508,  210,  219,  222,  213,  216,  508,  298,
 /*  2890 */   300,  299,  267,  411,  508,  254,  508,  508,  431,  508,
 /*  2900 */   508,  298,  300,  299,  267,  508,  508,  206,  263,  268,
 /*  2910 */   284,  203,  168,  508,  258,  376,   90,   91,   93,   92,
 /*  2920 */    94,  508,  291,  298,  300,  299,  267,  270,  508,  248,
 /*  2930 */   508,  508,  508,  278,  428,  246,  242,  244,  508,  508,
 /*  2940 */   205,  215,  221,  224,  226,  228,  218,  212,  230,  232,
 /*  2950 */   240,  234,  236,  238,  508,   39,  508,  508,  508,   96,
 /*  2960 */    97,  508,  463,  508,  508,  508,  272,  133,  508,  508,
 /*  2970 */    95,  508,  508,  508,  271,  273,  274,  275,  276,  277,
 /*  2980 */   538,  508,  210,  219,  222,  213,  216,  508,  298,  300,
 /*  2990 */   299,  267,   30,  508,  254,   42,   43,   44,   45,   46,
 /*  3000 */    47,   48,   49,   50,   51,   52,  206,  263,  268,  284,
 /*  3010 */   203,  355,  508,  258,  398,  508,  508,  508,  426,  508,
 /*  3020 */   508,  291,  298,  300,  299,  267,  270,  508,  248,  508,
 /*  3030 */   508,  508,  278,  508,  246,  242,  244,  508,  508,  205,
 /*  3040 */   215,  221,  224,  226,  228,  218,  212,  230,  232,  240,
 /*  3050 */   234,  236,  238,  508,   39,  508,  508,  508,  250,  398,
 /*  3060 */   508,  508,  508,  422,  508,  272,  508,  298,  300,  299,
 /*  3070 */   267,  508,  508,  271,  273,  274,  275,  276,  277,  225,
 /*  3080 */   508,  210,  219,  222,  213,  216,  508,  298,  300,  299,
 /*  3090 */   267,  508,  508,  254,   42,   43,   44,   45,   46,   47,
 /*  3100 */    48,   49,   50,   51,   52,  206,  263,  268,  284,  203,
 /*  3110 */   348,  508,  258,   39,  508,  508,  508,  508,  508,  508,
 /*  3120 */   291,  508,  508,  508,  508,  270,  508,  248,  508,  508,
 /*  3130 */   136,  278,   63,  246,  242,  244,  508,  508,  205,  215,
 /*  3140 */   221,  224,  226,  228,  218,  212,  230,  232,  240,  234,
 /*  3150 */   236,  238,  508,   42,   43,   44,   45,   46,   47,   48,
 /*  3160 */    49,   50,   51,   52,  272,  508,  508,  381,  508,  508,
 /*  3170 */   508,  508,  271,  273,  274,  275,  276,  277,  508,  508,
 /*  3180 */   210,  219,  222,  213,  216,  508,  398,  508,  508,  508,
 /*  3190 */   418,  508,  254,  508,  298,  300,  299,  267,  508,  508,
 /*  3200 */   508,  508,  508,  508,  206,  263,  268,  284,  203,  508,
 /*  3210 */   508,  258,  464,   90,   91,   93,   92,   94,  508,  291,
 /*  3220 */   508,  508,  508,  508,  270,  508,  248,  508,  508,  508,
 /*  3230 */   278,  508,  246,  242,  244,  508,  508,  205,  215,  221,
 /*  3240 */   224,  226,  228,  218,  212,  230,  232,  240,  234,  236,
 /*  3250 */   238,  508,  508,  508,  508,  508,   96,   97,  508,  713,
 /*  3260 */   621,  508,  615,  272,  508,  508,  539,   95,  508,  508,
 /*  3270 */   508,  271,  273,  274,  275,  276,  277,  233,  508,  210,
 /*  3280 */   219,  222,  213,  216,  508,  298,  300,  299,  267,  398,
 /*  3290 */   508,  254,  508,  410,  508,  508,  508,  298,  300,  299,
 /*  3300 */   267,  508,  508,  206,  263,  268,  284,  203,  297,  508,
 /*  3310 */   258,   39,  508,  508,  508,  508,  508,  508,  291,  508,
 /*  3320 */   508,  508,  508,  270,  508,  248,  508,  508,   26,  278,
 /*  3330 */    63,  246,  242,  244,  508,  508,  205,  215,  221,  224,
 /*  3340 */   226,  228,  218,  212,  230,  232,  240,  234,  236,  238,
 /*  3350 */   508,   42,   43,   44,   45,   46,   47,   48,   49,   50,
 /*  3360 */    51,   52,  272,  508,  508,  508,  508,  508,  508,  508,
 /*  3370 */   271,  273,  274,  275,  276,  277,  364,  329,  210,  219,
 /*  3380 */   222,  213,  216,  508,  298,  300,  299,  267,  398,  508,
 /*  3390 */   254,  508,  406,  508,  508,  508,  298,  300,  299,  267,
 /*  3400 */   508,  508,  206,  263,  268,  284,  203,  325,  508,  258,
 /*  3410 */    39,  508,  508,  508,  508,  508,  508,  291,  508,  508,
 /*  3420 */   508,  508,  270,  508,  248,  508,  508,  508,  278,   63,
 /*  3430 */   246,  242,  244,  508,  508,  205,  215,  221,  224,  226,
 /*  3440 */   228,  218,  212,  230,  232,  240,  234,  236,  238,  508,
 /*  3450 */    42,   43,   44,   45,   46,   47,   48,   49,   50,   51,
 /*  3460 */    52,  272,  508,  508,  534,  508,  508,  508,  508,  271,
 /*  3470 */   273,  274,  275,  276,  277,  217,  508,  210,  219,  222,
 /*  3480 */   213,  216,  508,  298,  300,  299,  267,  411,  508,  254,
 /*  3490 */   508,  508,  407,  508,  508,  298,  300,  299,  267,  508,
 /*  3500 */   508,  206,  263,  268,  284,  203,  177,  508,  258,  376,
 /*  3510 */    90,   91,   93,   92,   94,  508,  291,  298,  300,  299,
 /*  3520 */   267,  270,  508,  248,  508,  508,  508,  278,  436,  246,
 /*  3530 */   242,  244,  508,  508,  205,  215,  221,  224,  226,  228,
 /*  3540 */   218,  212,  230,  232,  240,  234,  236,  238,  508,  508,
 /*  3550 */   508,  508,  508,   96,   97,  508,  716,  609,  508,  615,
 /*  3560 */   272,  508,  508,  508,   95,  508,  508,  508,  271,  273,
 /*  3570 */   274,  275,  276,  277,  253,  367,  210,  219,  222,  213,
 /*  3580 */   216,  508,  298,  300,  299,  267,  398,  508,  254,  508,
 /*  3590 */   402,  508,  508,  508,  298,  300,  299,  267,  508,  508,
 /*  3600 */   206,  263,  268,  284,  203,  287,  508,  258,  398,  508,
 /*  3610 */   508,  508,  397,  508,  508,  291,  298,  300,  299,  267,
 /*  3620 */   270,  508,  248,  508,  508,  508,  278,  508,  246,  242,
 /*  3630 */   244,  508,  508,  205,  215,  221,  224,  226,  228,  218,
 /*  3640 */   212,  230,  232,  240,  234,  236,  238,  508,  508,  508,
 /*  3650 */   508,  508,  390,  508,  508,  476,  508,  399,  508,  272,
 /*  3660 */   435,   90,   91,   93,   92,   94,  508,  271,  273,  274,
 /*  3670 */   275,  276,  277,  247,  508,  210,  219,  222,  213,  216,
 /*  3680 */   508,  298,  300,  299,  267,  508,  508,  254,  391,  392,
 /*  3690 */   393,  394,  395,  508,  423,  443,  444,  508,  508,  206,
 /*  3700 */   263,  268,  284,  203,   96,   97,  258,  126,  621,  508,
 /*  3710 */   615,  508,  508,  508,  291,   95,  508,  508,  508,  270,
 /*  3720 */   508,  248,  508,  508,  508,  278,  508,  246,  242,  244,
 /*  3730 */   508,  508,  205,  215,  221,  224,  226,  228,  218,  212,
 /*  3740 */   230,  232,  240,  234,  236,  238,  245,  508,  508,  508,
 /*  3750 */   508,  525,  508,  508,  298,  300,  299,  267,  272,  508,
 /*  3760 */    90,   91,   93,   92,   94,  508,  271,  273,  274,  275,
 /*  3770 */   276,  277,  508,  881,  210,  219,  222,  213,  216,  201,
 /*  3780 */   508,  508,  376,  508,  508,  508,  254,  508,  508,  508,
 /*  3790 */   298,  300,  299,  267,  508,  508,  508,  508,  206,  263,
 /*  3800 */   268,  284,  203,   96,   97,  280,  712,  621,  686,  615,
 /*  3810 */   508,  508,  508,  291,   95,  508,  508,  508,  270,  508,
 /*  3820 */   248,  508,  508,  508,  278,  336,  246,  242,  244,  508,
 /*  3830 */   508,  205,  215,  221,  224,  226,  228,  218,  212,  230,
 /*  3840 */   232,  240,  234,  236,  238,    8,  508,  508,   11,  508,
 /*  3850 */   116,  508,  667,  677,  508,  508,  523,  335,  508,   90,
 /*  3860 */    91,   93,   92,   94,  508,  334,  273,  274,  275,  276,
 /*  3870 */   277,  257,  508,  210,  219,  222,  213,  216,  508,  298,
 /*  3880 */   300,  299,  267,  526,  508,  254,  508,  508,  508,  508,
 /*  3890 */   508,  298,  300,  299,  267,  508,  508,  206,  263,  268,
 /*  3900 */   284,  203,   96,   97,  258,  715,  577,   90,   91,   93,
 /*  3910 */    92,   94,  291,   95,  508,  508,  508,  270,  508,  248,
 /*  3920 */   508,  508,  508,  278,  404,  508,  242,  244,  508,  508,
 /*  3930 */   205,  215,  221,  224,  226,  228,  218,  212,  230,  232,
 /*  3940 */   240,  234,  236,  238,  508,  508,  508,  508,  508,  508,
 /*  3950 */    96,   97,  508,   20,   98,  508,  272,  508,  508,  508,
 /*  3960 */   508,   95,  508,  508,  271,  273,  274,  275,  276,  277,
 /*  3970 */   243,  508,  210,  219,  222,  213,  216,  508,  298,  300,
 /*  3980 */   299,  267,  220,  508,  254,  508,  508,  508,  508,  508,
 /*  3990 */   298,  300,  299,  267,  508,  508,  206,  263,  268,  284,
 /*  4000 */   203,  307,  508,  258,  241,   90,   91,   93,   92,   94,
 /*  4010 */   508,  291,  298,  300,  299,  267,  270,  229,  248,  508,
 /*  4020 */   508,  508,  278,  508,  508,  298,  300,  299,  267,  205,
 /*  4030 */   215,  221,  224,  226,  228,  218,  212,  230,  232,  240,
 /*  4040 */   234,  236,  238,  508,  508,  508,  508,  508,   96,   97,
 /*  4050 */   508,  718,  577,  508,  508,  272,  508,  508,  508,   95,
 /*  4060 */   508,  508,  508,  271,  273,  274,  275,  276,  277,  508,
 /*  4070 */   508,  210,  219,  222,  213,  216,   90,   91,   93,   92,
 /*  4080 */    94,  204,  508,  254,  508,  508,  508,  508,  508,  298,
 /*  4090 */   300,  299,  267,  508,  508,  206,  263,  268,  284,  203,
 /*  4100 */   508,  508,  258,  475,  508,  508,  508,  508,  508,  508,
 /*  4110 */   291,  298,  300,  299,  267,  270,  508,  248,  363,   96,
 /*  4120 */    97,  278,  717,  656,  508,  508,  298,  300,  299,  267,
 /*  4130 */    95,  239,   90,   91,   93,   92,   94,  508,  508,  298,
 /*  4140 */   300,  299,  267,  508,  508,  508,  508,  537,  375,  508,
 /*  4150 */   508,  508,  508,  508,  272,  508,  298,  300,  299,  267,
 /*  4160 */   508,  508,  271,  273,  274,  275,  276,  277,  508,  643,
 /*  4170 */   210,  219,  222,  213,  216,   96,   97,  508,  508,  508,
 /*  4180 */   508,  508,  254,  133,  508,  508,   95,  508,   90,   91,
 /*  4190 */    93,   92,   94,  508,  206,  263,  268,  284,  203,  333,
 /*  4200 */   508,  258,  508,   90,   91,   93,   92,   94,  508,  291,
 /*  4210 */   508,  508,  508,  508,  270,  508,  248,  508,  508,  508,
 /*  4220 */   278,  508,  508,  508,  508,  508,  508,  508,  508,  508,
 /*  4230 */   508,   96,   97,  508,   18,   98,   90,   91,   93,   92,
 /*  4240 */    94,  508,   95,  508,  508,  508,   96,   97,  508,  714,
 /*  4250 */   577,  508,  508,  272,  508,  508,  508,   95,  508,  508,
 /*  4260 */   508,  271,  273,  274,  275,  276,  277,  508,  508,  210,
 /*  4270 */   219,  222,  213,  216,   90,   91,   93,   92,   94,   96,
 /*  4280 */    97,  254,  719,  577,  508,  508,  508,  508,  508,  508,
 /*  4290 */    95,  508,  508,  206,  263,  268,  284,  203,  314,  508,
 /*  4300 */   258,  508,   90,   91,   93,   92,   94,  508,  291,  508,
 /*  4310 */   508,  508,  508,  270,  508,  248,  508,   96,   97,  278,
 /*  4320 */   720,  577,  508,  508,  508,  508,  508,  508,   95,  508,
 /*  4330 */    90,   91,   93,   92,   94,  508,  508,  508,  508,  623,
 /*  4340 */   508,  508,  508,  508,  508,   96,   97,   90,   91,   93,
 /*  4350 */    92,   94,  272,  579,  508,  508,   95,  508,  508,  508,
 /*  4360 */   271,  273,  274,  275,  276,  277,  508,  508,  210,  219,
 /*  4370 */   222,  213,  216,   96,   97,  508,  508,  508,  508,  610,
 /*  4380 */   254,  508,  508,  508,   95,  508,  508,  508,  508,  508,
 /*  4390 */    96,   97,  206,  263,  268,  284,  203,  508,   23,  258,
 /*  4400 */   508,   95,  508,  508,  508,  508,  508,  291,  508,  508,
 /*  4410 */   508,  508,  270,  508,  248,  508,  508,  508,  278,  508,
 /*  4420 */   508,  508,  508,  508,  508,  508,  508,  508,  508,  508,
 /*  4430 */   508,  508,  508,  508,  508,  508,  508,  508,  508,  508,
 /*  4440 */   508,  508,  508,  508,  508,  508,  508,  508,  508,  508,
 /*  4450 */   508,  272,  508,   90,   91,   93,   92,   94,  508,  271,
 /*  4460 */   273,  274,  275,  276,  277,  508,  508,  210,  219,  222,
 /*  4470 */   213,  216,   90,   91,   93,   92,   94,  508,  508,  254,
 /*  4480 */   508,  508,  508,  508,  508,  508,  508,  508,  508,  508,
 /*  4490 */   508,  206,  263,  268,  284,  203,   96,   97,  280,  508,
 /*  4500 */   508,  508,  508,  508,  100,  508,  291,   95,  508,  508,
 /*  4510 */   508,  270,  508,  248,  508,   96,   97,  278,  508,  508,
 /*  4520 */   508,  508,  508,  579,  508,  508,   95,  508,  508,  508,
 /*  4530 */   508,  508,  508,  508,  508,  508,  508,  508,  508,  508,
 /*  4540 */   508,  508,  508,  508,  508,  508,  508,  508,  508,  508,
 /*  4550 */   335,  508,  508,  508,  508,  508,  508,  508,  334,  273,
 /*  4560 */   274,  275,  276,  277,
};
static YYCODETYPE yy_lookahead[] = {
 /*     0 */     7,    8,    9,   10,   11,   25,   26,   27,   28,   29,
 /*    10 */    30,    6,   19,  105,  106,  107,  108,  109,  110,  111,
 /*    20 */   112,  137,  138,  139,   31,   32,   33,   34,   35,  116,
 /*    30 */    49,   38,  119,  120,  121,  122,  123,  123,   57,   46,
 /*    40 */     6,  128,  129,  129,   51,   38,   53,  134,  134,   73,
 /*    50 */    57,   46,   59,   60,   61,   62,   63,   64,   65,   66,
 /*    60 */    67,   68,   69,  121,  122,  123,   85,   86,   87,   88,
 /*    70 */    89,  129,   32,   92,   93,   39,  134,   38,   38,   39,
 /*    80 */    38,   47,   46,   90,   38,   49,   46,   47,   48,   47,
 /*    90 */    48,   98,   99,  100,  101,  102,  103,   90,   38,   59,
 /*   100 */    60,   61,   62,   63,   64,   65,   66,   67,   48,   69,
 /*   110 */    70,   71,   72,  115,   74,  117,   32,  163,   78,   79,
 /*   120 */    80,   81,   38,   39,   84,  171,  172,  173,  174,   90,
 /*   130 */    46,   47,   48,   53,   94,   95,   96,   97,  108,  109,
 /*   140 */   110,  111,  112,   59,   60,   61,   62,   63,   64,   65,
 /*   150 */    66,   67,   41,   69,   70,   71,   72,   46,   74,   28,
 /*   160 */    29,   30,   78,   79,   80,   81,  131,   43,   84,   32,
 /*   170 */    46,   91,  137,  138,  139,   38,   39,    6,   94,   95,
 /*   180 */    96,   97,  163,   46,   47,   48,   46,   38,  169,  170,
 /*   190 */   171,  172,  173,  174,   54,   46,   59,   60,   61,   62,
 /*   200 */    63,   64,   65,   66,   67,    6,   69,   70,   71,   72,
 /*   210 */    39,   74,   39,   32,  163,   78,   79,   80,   81,   38,
 /*   220 */    39,   84,  171,  172,  173,  174,    0,   46,   47,   48,
 /*   230 */   113,   94,   95,   96,   97,   36,   53,   54,  175,  176,
 /*   240 */    59,   60,   61,   62,   63,   64,   65,   66,   67,    6,
 /*   250 */    69,   70,   71,   72,  119,   74,  121,  122,  123,   78,
 /*   260 */    79,   80,   81,  128,  129,   84,   32,   38,   54,  134,
 /*   270 */    46,   57,   38,   39,   91,   94,   95,   96,   97,   36,
 /*   280 */    46,   47,   48,  113,  133,  137,  135,  136,  137,  138,
 /*   290 */   136,  137,  138,   59,   60,   61,   62,   63,   64,   65,
 /*   300 */    66,   67,    6,   69,   70,   71,   72,  115,   74,  117,
 /*   310 */    32,  163,   78,   79,   80,   81,   38,   39,   84,  171,
 /*   320 */   172,  173,  174,   38,   46,   47,   48,  134,   94,   95,
 /*   330 */    96,   97,  115,   54,  117,   39,   57,   59,   60,   61,
 /*   340 */    62,   63,   64,   65,   66,   67,   49,   69,   70,   71,
 /*   350 */    72,   38,   74,  121,   57,  123,   78,   79,   80,   81,
 /*   360 */   128,  129,   84,   32,  164,  165,  134,  161,  162,   38,
 /*   370 */    39,   53,   94,   95,   96,   97,   47,   46,   47,   48,
 /*   380 */   175,  176,   85,   86,   87,   88,   89,  175,  176,   39,
 /*   390 */    59,   60,   61,   62,   63,   64,   65,   66,   67,    6,
 /*   400 */    69,   70,   71,   72,   75,   74,   77,   32,  163,   78,
 /*   410 */    79,   80,   81,   38,   39,   84,  171,  172,  173,  174,
 /*   420 */    47,   46,   47,   48,   39,   94,   95,   96,   97,  175,
 /*   430 */   176,   46,   39,   15,   59,   60,   61,   62,   63,   64,
 /*   440 */    65,   66,   67,   49,   69,   70,   71,   72,   75,   74,
 /*   450 */    77,   39,    6,   78,   79,   80,   81,  131,   46,   84,
 /*   460 */    32,   49,  123,  137,  138,  139,   38,   39,    6,   94,
 /*   470 */    95,   96,   97,  134,   46,   47,   48,   35,  131,   85,
 /*   480 */    86,   87,   88,   89,  137,  138,  139,   59,   60,   61,
 /*   490 */    62,   63,   64,   65,   66,   67,    6,   69,   70,   71,
 /*   500 */    72,   39,   74,   58,   32,  163,   78,   79,   80,   81,
 /*   510 */    38,   39,   84,  171,  172,  173,  174,   57,   46,   47,
 /*   520 */    48,   38,   94,   95,   96,   97,    6,   20,   82,   39,
 /*   530 */    39,   59,   60,   61,   62,   63,   64,   65,   66,   67,
 /*   540 */    38,   69,   70,   71,   72,   54,   74,   38,   46,    6,
 /*   550 */    78,   79,   80,   81,  131,   46,   84,   32,   39,   39,
 /*   560 */   137,  138,  139,   38,   39,   46,   94,   95,   96,   97,
 /*   570 */     6,   46,   47,   48,  133,   38,  135,  136,  137,  138,
 /*   580 */   175,  176,   39,   39,   59,   60,   61,   62,   63,   64,
 /*   590 */    65,   66,   67,   39,   69,   70,   71,   72,   54,   74,
 /*   600 */    36,   32,  163,   78,   79,   80,   81,   38,   39,   84,
 /*   610 */   171,  172,  173,  174,   49,   46,   47,   48,  124,   94,
 /*   620 */    95,   96,   97,  175,  176,   38,    6,   90,   59,   60,
 /*   630 */    61,   62,   63,   64,   65,   66,   67,    6,   69,   70,
 /*   640 */    71,   72,   91,   74,  123,    6,    6,   78,   79,   80,
 /*   650 */    81,  130,   49,   84,   32,  134,   36,   41,   51,   43,
 /*   660 */    38,   39,   46,   94,   95,   96,   97,   36,   46,   47,
 /*   670 */    48,  133,   38,  135,  136,  137,  138,   90,   39,   39,
 /*   680 */    46,   59,   60,   61,   62,   63,   64,   65,   66,   67,
 /*   690 */    41,   69,   70,   71,   72,   46,   74,  124,   32,  163,
 /*   700 */    78,   79,   80,   81,   38,   39,   84,  171,  172,  173,
 /*   710 */   174,    6,   46,   47,   48,    6,   94,   95,   96,   97,
 /*   720 */   175,  176,  175,  176,   38,   59,   60,   61,   62,   63,
 /*   730 */    64,   65,   66,   67,    6,   69,   70,   71,   72,   38,
 /*   740 */    74,   36,  175,  176,   78,   79,   80,   81,   39,   49,
 /*   750 */    84,   32,   51,  124,   41,   38,   43,   38,   39,   46,
 /*   760 */    94,   95,   96,   97,   36,   46,   47,   48,  133,   49,
 /*   770 */   135,  136,  137,  138,  124,    6,  175,  176,   59,   60,
 /*   780 */    61,   62,   63,   64,   65,   66,   67,    6,   69,   70,
 /*   790 */    71,   72,   38,   74,    6,   32,  163,   78,   79,   80,
 /*   800 */    81,   38,   39,   84,  171,  172,  173,  174,   39,   46,
 /*   810 */    47,   48,   53,   94,   95,   96,   97,  126,  127,   39,
 /*   820 */    39,    6,   59,   60,   61,   62,   63,   64,   65,   66,
 /*   830 */    67,    6,   69,   70,   71,   72,   35,   74,  123,   38,
 /*   840 */    38,   78,   79,   80,   81,   46,   58,   84,   32,  134,
 /*   850 */    39,   36,  114,   54,   38,   39,   39,   94,   95,   96,
 /*   860 */    97,   36,   46,   47,   48,  133,   38,  135,  136,  137,
 /*   870 */   138,   54,   38,   38,   46,   59,   60,   61,   62,   63,
 /*   880 */    64,   65,   66,   67,    6,   69,   70,   71,   72,  114,
 /*   890 */    74,  114,   32,  163,   78,   79,   80,   81,   38,   39,
 /*   900 */    84,  171,  172,  173,  174,    6,   46,   47,   48,   46,
 /*   910 */    94,   95,   96,   97,   36,   47,    6,   54,   38,   59,
 /*   920 */    60,   61,   62,   63,   64,   65,   66,   67,   46,   69,
 /*   930 */    70,   71,   72,    6,   74,   36,   54,  123,   78,   79,
 /*   940 */    80,   81,   38,   53,   84,   32,   36,   39,  134,   46,
 /*   950 */    46,   38,   39,   39,   94,   95,   96,   97,    6,   46,
 /*   960 */    47,   48,  133,   36,  135,  136,  137,  138,   54,   39,
 /*   970 */   162,   46,   59,   60,   61,   62,   63,   64,   65,   66,
 /*   980 */    67,   46,   69,   70,   71,   72,  124,   74,   36,   32,
 /*   990 */   163,   78,   79,   80,   81,   38,   39,   84,  171,  172,
 /*  1000 */   173,  174,   46,   46,   47,   48,   46,   94,   95,   96,
 /*  1010 */    97,   46,   76,   79,   39,   38,   59,   60,   61,   62,
 /*  1020 */    63,   64,   65,   66,   67,  177,   69,   70,   71,   72,
 /*  1030 */    36,   74,   20,   38,   53,   78,   79,   80,   81,   38,
 /*  1040 */     6,   84,   32,  177,   53,   53,   38,   47,   38,   39,
 /*  1050 */    53,   94,   95,   96,   97,   38,   46,   47,   48,  133,
 /*  1060 */    47,  135,  136,  137,  138,   38,   47,   53,   53,   59,
 /*  1070 */    60,   61,   62,   63,   64,   65,   66,   67,   76,   69,
 /*  1080 */    70,   71,   72,   76,   74,   76,   32,  163,   78,   79,
 /*  1090 */    80,   81,   38,   39,   84,  171,  172,  173,  174,   47,
 /*  1100 */    46,   47,   48,   53,   94,   95,   96,   97,   47,   53,
 /*  1110 */    47,   47,   38,   59,   60,   61,   62,   63,   64,   65,
 /*  1120 */    66,   67,   38,   69,   70,   71,   72,  165,   74,  166,
 /*  1130 */   166,   58,   78,   79,   80,   81,  166,  166,   84,   32,
 /*  1140 */    47,   47,  166,   38,  166,   38,   39,   58,   94,   95,
 /*  1150 */    96,   97,  166,   46,   47,   48,  133,   58,  135,  136,
 /*  1160 */   137,  138,  166,   39,  166,  166,   59,   60,   61,   62,
 /*  1170 */    63,   64,   65,   66,   67,   39,   69,   70,   71,   72,
 /*  1180 */    39,   74,  166,   32,  163,   78,   79,   80,   81,   38,
 /*  1190 */    39,   84,  171,  172,  173,  174,   39,   46,   47,   48,
 /*  1200 */    38,   94,   95,   96,   97,   38,  176,   39,   39,   38,
 /*  1210 */    59,   60,   61,   62,   63,   64,   65,   66,   67,   49,
 /*  1220 */    69,   70,   71,   72,  124,   74,   38,   47,   47,   78,
 /*  1230 */    79,   80,   81,   38,   82,   84,   32,   79,   39,   76,
 /*  1240 */    46,   38,   38,   39,   46,   94,   95,   96,   97,   73,
 /*  1250 */    46,   47,   48,   46,   53,   53,   54,   46,   46,   57,
 /*  1260 */    51,  124,   49,   59,   60,   61,   62,   63,   64,   65,
 /*  1270 */    66,   67,   39,   69,   70,   71,   72,   51,   74,   49,
 /*  1280 */    32,  163,   78,   79,   80,   81,   38,   39,   84,  171,
 /*  1290 */   172,  173,  174,   91,   46,   47,   48,  124,   94,   95,
 /*  1300 */    96,   97,   39,  125,  125,   39,  124,   59,   60,   61,
 /*  1310 */    62,   63,   64,   65,   66,   67,   39,   69,   70,   71,
 /*  1320 */    72,   39,   74,  127,   38,  125,   78,   79,   80,   81,
 /*  1330 */    39,  125,   84,   32,  124,   39,   39,  117,  114,   38,
 /*  1340 */    39,  118,   94,   95,   96,   97,  123,   46,   47,   48,
 /*  1350 */    42,   38,  114,  130,   38,   46,  114,  134,  114,   42,
 /*  1360 */    59,   60,   61,   62,   63,   64,   65,   66,   67,   38,
 /*  1370 */    69,   70,   71,   72,  114,   74,   38,   32,  163,   78,
 /*  1380 */    79,   80,   81,   38,   39,   84,  171,  172,  173,  174,
 /*  1390 */   114,   46,   47,   48,  184,   94,   95,   96,   97,  184,
 /*  1400 */   184,  184,  184,  184,   59,   60,   61,   62,   63,   64,
 /*  1410 */    65,   66,   67,  184,   69,   70,   71,   72,  184,   74,
 /*  1420 */   184,  184,  184,   78,   79,   80,   81,  184,  184,   84,
 /*  1430 */    32,  184,  184,  184,  184,  184,   38,   39,  184,   94,
 /*  1440 */    95,   96,   97,  184,   46,   47,   48,  184,  184,  184,
 /*  1450 */   184,  184,  184,  184,  184,  184,  184,   59,   60,   61,
 /*  1460 */    62,   63,   64,   65,   66,   67,  184,   69,   70,   71,
 /*  1470 */    72,  184,   74,  184,   32,  163,   78,   79,   80,   81,
 /*  1480 */    38,   39,   84,  171,  172,  173,  174,  184,   46,   47,
 /*  1490 */    48,  184,   94,   95,   96,   97,  184,  184,  184,  184,
 /*  1500 */   184,   59,   60,   61,   62,   63,   64,   65,   66,   67,
 /*  1510 */   184,   69,   70,   71,   72,  184,   74,  184,  184,  184,
 /*  1520 */    78,   79,   80,   81,  184,  184,   84,   32,  184,  184,
 /*  1530 */   184,  184,  184,   38,   39,  184,   94,   95,   96,   97,
 /*  1540 */   184,   46,   47,   48,  184,  184,  184,  184,  184,  184,
 /*  1550 */   184,  184,  184,  184,   59,   60,   61,   62,   63,   64,
 /*  1560 */    65,   66,   67,  184,   69,   70,   71,   72,  184,   74,
 /*  1570 */   184,   32,  163,   78,   79,   80,   81,   38,   39,   84,
 /*  1580 */   171,  172,  173,  174,  184,   46,   47,   48,  184,   94,
 /*  1590 */    95,   96,   97,  184,  184,  184,  184,  184,   59,   60,
 /*  1600 */    61,   62,   63,   64,   65,   66,   67,  184,   69,   70,
 /*  1610 */    71,   72,  184,   74,  184,  184,  184,   78,   79,   80,
 /*  1620 */    81,  184,  184,   84,   32,    1,    2,    3,    4,    5,
 /*  1630 */    38,   39,  184,   94,   95,   96,   97,  184,   46,   37,
 /*  1640 */    48,  184,   40,  184,   42,  184,   44,   45,  184,  184,
 /*  1650 */    48,   59,   60,   61,   62,   63,   64,   65,   66,   67,
 /*  1660 */   184,   69,   70,   71,   72,  184,   74,  184,   44,   45,
 /*  1670 */    78,   79,   80,   81,  112,  184,   84,  184,  184,   55,
 /*  1680 */   184,  184,  184,  184,  184,  184,   94,   95,   96,   97,
 /*  1690 */   163,  184,  184,  184,  132,  184,  184,  170,  171,  172,
 /*  1700 */   173,  174,  140,  141,  142,  143,  144,  145,  146,  147,
 /*  1710 */   148,  149,  150,  151,  152,  153,  154,  155,  156,  157,
 /*  1720 */   158,  159,  112,  184,  184,  184,  184,  184,  184,  163,
 /*  1730 */   184,  184,  184,  171,  172,  173,  174,  171,  172,  173,
 /*  1740 */   174,  184,  132,  184,  178,  184,  184,  181,  182,  183,
 /*  1750 */   140,  141,  142,  143,  144,  145,  146,  147,  148,  149,
 /*  1760 */   150,  151,  152,  153,  154,  155,  156,  157,  158,  159,
 /*  1770 */   112,  184,  184,  184,  184,  184,  184,  163,  184,  184,
 /*  1780 */   184,  171,  172,  173,  174,  171,  172,  173,  174,  184,
 /*  1790 */   132,  184,  184,  184,  184,  181,  182,  183,  140,  141,
 /*  1800 */   142,  143,  144,  145,  146,  147,  148,  149,  150,  151,
 /*  1810 */   152,  153,  154,  155,  156,  157,  158,  159,  112,  184,
 /*  1820 */   184,  184,  184,  184,  184,  163,  184,  184,  184,  171,
 /*  1830 */   172,  173,  174,  171,  172,  173,  174,  184,  132,  184,
 /*  1840 */   184,  179,  180,  184,  184,  184,  140,  141,  142,  143,
 /*  1850 */   144,  145,  146,  147,  148,  149,  150,  151,  152,  153,
 /*  1860 */   154,  155,  156,  157,  158,  159,  112,  184,  184,  184,
 /*  1870 */   184,  184,  184,  163,  184,  184,  184,  171,  172,  173,
 /*  1880 */   174,  171,  172,  173,  174,  184,  132,  184,  184,  179,
 /*  1890 */   180,  184,  184,  184,  140,  141,  142,  143,  144,  145,
 /*  1900 */   146,  147,  148,  149,  150,  151,  152,  153,  154,  155,
 /*  1910 */   156,  157,  158,  159,  112,  184,  184,  184,  184,  184,
 /*  1920 */   184,  120,  121,  122,  123,  171,  172,  173,  174,  128,
 /*  1930 */   129,  184,  184,  184,  132,  134,  184,  184,  184,  184,
 /*  1940 */   184,  184,  140,  141,  142,  143,  144,  145,  146,  147,
 /*  1950 */   148,  149,  150,  151,  152,  153,  154,  155,  156,  157,
 /*  1960 */   158,  159,  112,  184,  184,  184,  184,  184,  184,  163,
 /*  1970 */   184,  184,  184,  171,  172,  173,  174,  171,  172,  173,
 /*  1980 */   174,  184,  132,  184,  184,  179,  180,  184,  184,  184,
 /*  1990 */   140,  141,  142,  143,  144,  145,  146,  147,  148,  149,
 /*  2000 */   150,  151,  152,  153,  154,  155,  156,  157,  158,  159,
 /*  2010 */   112,  184,  184,  184,  184,  184,  184,  163,  184,  184,
 /*  2020 */   184,  171,  172,  173,  174,  171,  172,  173,  174,  184,
 /*  2030 */   132,  184,  184,  179,  180,  184,  184,  184,  140,  141,
 /*  2040 */   142,  143,  144,  145,  146,  147,  148,  149,  150,  151,
 /*  2050 */   152,  153,  154,  155,  156,  157,  158,  159,  112,  184,
 /*  2060 */   184,  184,  184,  184,  184,  163,  184,  184,  184,  171,
 /*  2070 */   172,  173,  174,  171,  172,  173,  174,  184,  132,  184,
 /*  2080 */   184,  179,  180,  184,  184,  184,  140,  141,  142,  143,
 /*  2090 */   144,  145,  146,  147,  148,  149,  150,  151,  152,  153,
 /*  2100 */   154,  155,  156,  157,  158,  159,  112,  184,  184,  184,
 /*  2110 */   184,  184,  184,  163,  184,  184,  184,  171,  172,  173,
 /*  2120 */   174,  171,  172,  173,  174,  184,  132,  184,  184,  179,
 /*  2130 */   180,  184,  184,  184,  140,  141,  142,  143,  144,  145,
 /*  2140 */   146,  147,  148,  149,  150,  151,  152,  153,  154,  155,
 /*  2150 */   156,  157,  158,  159,  112,  184,  184,  184,  184,  184,
 /*  2160 */   184,  163,  184,  184,  184,  171,  172,  173,  174,  171,
 /*  2170 */   172,  173,  174,  184,  132,  184,  184,  179,  180,  184,
 /*  2180 */   184,  184,  140,  141,  142,  143,  144,  145,  146,  147,
 /*  2190 */   148,  149,  150,  151,  152,  153,  154,  155,  156,  157,
 /*  2200 */   158,  159,  112,  184,  184,  184,  184,  184,  184,  163,
 /*  2210 */   184,  184,  184,  171,  172,  173,  174,  171,  172,  173,
 /*  2220 */   174,  184,  132,  184,  184,  179,  180,  184,  184,  184,
 /*  2230 */   140,  141,  142,  143,  144,  145,  146,  147,  148,  149,
 /*  2240 */   150,  151,  152,  153,  154,  155,  156,  157,  158,  159,
 /*  2250 */   112,  184,  184,  184,  184,  184,  184,  163,  184,  184,
 /*  2260 */   184,  171,  172,  173,  174,  171,  172,  173,  174,  184,
 /*  2270 */   132,  184,  184,  179,  180,  184,  184,  184,  140,  141,
 /*  2280 */   142,  143,  144,  145,  146,  147,  148,  149,  150,  151,
 /*  2290 */   152,  153,  154,  155,  156,  157,  158,  159,  112,  184,
 /*  2300 */   184,  184,  184,  184,  184,  163,  184,  184,  184,  171,
 /*  2310 */   172,  173,  174,  171,  172,  173,  174,  184,  132,  184,
 /*  2320 */   184,  179,  180,  184,  184,  184,  140,  141,  142,  143,
 /*  2330 */   144,  145,  146,  147,  148,  149,  150,  151,  152,  153,
 /*  2340 */   154,  155,  156,  157,  158,  159,  112,  184,  184,  184,
 /*  2350 */   160,  184,  184,  163,  184,  184,  184,  171,  172,  173,
 /*  2360 */   174,  171,  172,  173,  174,  184,  132,  184,  184,  184,
 /*  2370 */   184,  184,  184,  184,  140,  141,  142,  143,  144,  145,
 /*  2380 */   146,  147,  148,  149,  150,  151,  152,  153,  154,  155,
 /*  2390 */   156,  157,  158,  159,  112,  184,  184,  184,  160,  184,
 /*  2400 */   184,  163,  184,  184,  184,  171,  172,  173,  174,  171,
 /*  2410 */   172,  173,  174,  184,  132,  184,  184,  184,  184,  184,
 /*  2420 */   184,  184,  140,  141,  142,  143,  144,  145,  146,  147,
 /*  2430 */   148,  149,  150,  151,  152,  153,  154,  155,  156,  157,
 /*  2440 */   158,  159,  112,  184,  184,  184,  184,  184,  184,  184,
 /*  2450 */   184,  184,  184,  171,  172,  173,  174,  163,  184,  184,
 /*  2460 */   184,  167,  132,  184,  184,  171,  172,  173,  174,  184,
 /*  2470 */   140,  141,  142,  143,  144,  145,  146,  147,  148,  149,
 /*  2480 */   150,  151,  152,  153,  154,  155,  156,  157,  158,  159,
 /*  2490 */   112,  184,  184,  184,  184,  184,  184,  184,  184,  184,
 /*  2500 */   184,  171,  172,  173,  174,  163,  184,  184,  184,  167,
 /*  2510 */   132,  184,  184,  171,  172,  173,  174,  184,  140,  141,
 /*  2520 */   142,  143,  144,  145,  146,  147,  148,  149,  150,  151,
 /*  2530 */   152,  153,  154,  155,  156,  157,  158,  159,  112,  184,
 /*  2540 */   184,  184,  184,  184,  184,  184,  184,  184,  184,  171,
 /*  2550 */   172,  173,  174,  184,  163,  184,  184,  184,  132,  168,
 /*  2560 */   184,  184,  171,  172,  173,  174,  140,  141,  142,  143,
 /*  2570 */   144,  145,  146,  147,  148,  149,  150,  151,  152,  153,
 /*  2580 */   154,  155,  156,  157,  158,  159,  112,  184,  184,   38,
 /*  2590 */   184,  184,  184,  184,  184,  184,  184,  171,  172,  173,
 /*  2600 */   174,  163,  184,  184,  184,  167,  132,  184,  184,  171,
 /*  2610 */   172,  173,  174,  184,  140,  141,  142,  143,  144,  145,
 /*  2620 */   146,  147,  148,  149,  150,  151,  152,  153,  154,  155,
 /*  2630 */   156,  157,  158,  159,  112,  184,  184,  184,  184,  184,
 /*  2640 */   184,   90,  184,  184,  184,  171,  172,  173,  174,   98,
 /*  2650 */    99,  100,  101,  102,  103,  184,  184,  184,  184,  184,
 /*  2660 */   184,  184,  140,  141,  142,  143,  144,  145,  146,  147,
 /*  2670 */   148,  149,  150,  151,  152,  153,  154,  155,  156,  157,
 /*  2680 */   158,  159,  184,  184,  184,    7,    8,    9,   10,   11,
 /*  2690 */   163,  184,  184,  171,  172,  173,  174,   19,  171,  172,
 /*  2700 */   173,  174,   53,   54,  184,  184,   57,  180,  184,   31,
 /*  2710 */    32,   33,   34,   35,   36,  184,   38,   19,  184,  184,
 /*  2720 */   184,  184,  184,  184,   46,   76,  184,  184,  184,   51,
 /*  2730 */   184,   53,  184,  184,   36,   57,   38,   12,   13,   14,
 /*  2740 */    91,  184,   17,   18,   19,   20,   21,   22,   23,   24,
 /*  2750 */    25,   26,   27,   28,   29,   30,  184,   59,   60,   61,
 /*  2760 */    62,   63,   64,   65,   66,   67,   68,   69,   90,  184,
 /*  2770 */   184,  184,  184,  184,  184,  184,   98,   99,  100,  101,
 /*  2780 */   102,  103,  184,   58,    7,    8,    9,   10,   11,  163,
 /*  2790 */   184,  184,  184,  167,  163,  184,   19,  171,  172,  173,
 /*  2800 */   174,  184,  171,  172,  173,  174,  184,  184,   31,   32,
 /*  2810 */    33,   34,   35,   36,  183,   38,   19,  184,  184,  184,
 /*  2820 */   184,  184,  184,   46,  184,  184,  184,  184,   51,  184,
 /*  2830 */    53,  184,  184,   36,   57,   38,   12,   13,   14,  184,
 /*  2840 */   184,   17,   18,   19,   20,   21,   22,   23,   24,   25,
 /*  2850 */    26,   27,   28,   29,   30,  184,   59,   60,   61,   62,
 /*  2860 */    63,   64,   65,   66,   67,   68,   69,   90,  184,  184,
 /*  2870 */    46,  184,  184,  184,  184,   98,   99,  100,  101,  102,
 /*  2880 */   103,  163,  184,    7,    8,    9,   10,   11,  184,  171,
 /*  2890 */   172,  173,  174,  163,  184,   19,  184,  184,  168,  184,
 /*  2900 */   184,  171,  172,  173,  174,  184,  184,   31,   32,   33,
 /*  2910 */    34,   35,  160,  184,   38,  163,    1,    2,    3,    4,
 /*  2920 */     5,  184,   46,  171,  172,  173,  174,   51,  184,   53,
 /*  2930 */   184,  184,  184,   57,   58,   12,   13,   14,  184,  184,
 /*  2940 */    17,   18,   19,   20,   21,   22,   23,   24,   25,   26,
 /*  2950 */    27,   28,   29,   30,  184,   19,  184,  184,  184,   44,
 /*  2960 */    45,  184,   39,  184,  184,  184,   90,   52,  184,  184,
 /*  2970 */    55,  184,  184,  184,   98,   99,  100,  101,  102,  103,
 /*  2980 */   163,  184,    7,    8,    9,   10,   11,  184,  171,  172,
 /*  2990 */   173,  174,   56,  184,   19,   59,   60,   61,   62,   63,
 /*  3000 */    64,   65,   66,   67,   68,   69,   31,   32,   33,   34,
 /*  3010 */    35,   36,  184,   38,  163,  184,  184,  184,  167,  184,
 /*  3020 */   184,   46,  171,  172,  173,  174,   51,  184,   53,  184,
 /*  3030 */   184,  184,   57,  184,   12,   13,   14,  184,  184,   17,
 /*  3040 */    18,   19,   20,   21,   22,   23,   24,   25,   26,   27,
 /*  3050 */    28,   29,   30,  184,   19,  184,  184,  184,   36,  163,
 /*  3060 */   184,  184,  184,  167,  184,   90,  184,  171,  172,  173,
 /*  3070 */   174,  184,  184,   98,   99,  100,  101,  102,  103,  163,
 /*  3080 */   184,    7,    8,    9,   10,   11,  184,  171,  172,  173,
 /*  3090 */   174,  184,  184,   19,   59,   60,   61,   62,   63,   64,
 /*  3100 */    65,   66,   67,   68,   69,   31,   32,   33,   34,   35,
 /*  3110 */    36,  184,   38,   19,  184,  184,  184,  184,  184,  184,
 /*  3120 */    46,  184,  184,  184,  184,   51,  184,   53,  184,  184,
 /*  3130 */    36,   57,   38,   12,   13,   14,  184,  184,   17,   18,
 /*  3140 */    19,   20,   21,   22,   23,   24,   25,   26,   27,   28,
 /*  3150 */    29,   30,  184,   59,   60,   61,   62,   63,   64,   65,
 /*  3160 */    66,   67,   68,   69,   90,  184,  184,   46,  184,  184,
 /*  3170 */   184,  184,   98,   99,  100,  101,  102,  103,  184,  184,
 /*  3180 */     7,    8,    9,   10,   11,  184,  163,  184,  184,  184,
 /*  3190 */   167,  184,   19,  184,  171,  172,  173,  174,  184,  184,
 /*  3200 */   184,  184,  184,  184,   31,   32,   33,   34,   35,  184,
 /*  3210 */   184,   38,   39,    1,    2,    3,    4,    5,  184,   46,
 /*  3220 */   184,  184,  184,  184,   51,  184,   53,  184,  184,  184,
 /*  3230 */    57,  184,   12,   13,   14,  184,  184,   17,   18,   19,
 /*  3240 */    20,   21,   22,   23,   24,   25,   26,   27,   28,   29,
 /*  3250 */    30,  184,  184,  184,  184,  184,   44,   45,  184,   47,
 /*  3260 */    48,  184,   50,   90,  184,  184,   46,   55,  184,  184,
 /*  3270 */   184,   98,   99,  100,  101,  102,  103,  163,  184,    7,
 /*  3280 */     8,    9,   10,   11,  184,  171,  172,  173,  174,  163,
 /*  3290 */   184,   19,  184,  167,  184,  184,  184,  171,  172,  173,
 /*  3300 */   174,  184,  184,   31,   32,   33,   34,   35,   36,  184,
 /*  3310 */    38,   19,  184,  184,  184,  184,  184,  184,   46,  184,
 /*  3320 */   184,  184,  184,   51,  184,   53,  184,  184,   36,   57,
 /*  3330 */    38,   12,   13,   14,  184,  184,   17,   18,   19,   20,
 /*  3340 */    21,   22,   23,   24,   25,   26,   27,   28,   29,   30,
 /*  3350 */   184,   59,   60,   61,   62,   63,   64,   65,   66,   67,
 /*  3360 */    68,   69,   90,  184,  184,  184,  184,  184,  184,  184,
 /*  3370 */    98,   99,  100,  101,  102,  103,  163,   58,    7,    8,
 /*  3380 */     9,   10,   11,  184,  171,  172,  173,  174,  163,  184,
 /*  3390 */    19,  184,  167,  184,  184,  184,  171,  172,  173,  174,
 /*  3400 */   184,  184,   31,   32,   33,   34,   35,   36,  184,   38,
 /*  3410 */    19,  184,  184,  184,  184,  184,  184,   46,  184,  184,
 /*  3420 */   184,  184,   51,  184,   53,  184,  184,  184,   57,   38,
 /*  3430 */    12,   13,   14,  184,  184,   17,   18,   19,   20,   21,
 /*  3440 */    22,   23,   24,   25,   26,   27,   28,   29,   30,  184,
 /*  3450 */    59,   60,   61,   62,   63,   64,   65,   66,   67,   68,
 /*  3460 */    69,   90,  184,  184,   46,  184,  184,  184,  184,   98,
 /*  3470 */    99,  100,  101,  102,  103,  163,  184,    7,    8,    9,
 /*  3480 */    10,   11,  184,  171,  172,  173,  174,  163,  184,   19,
 /*  3490 */   184,  184,  168,  184,  184,  171,  172,  173,  174,  184,
 /*  3500 */   184,   31,   32,   33,   34,   35,  160,  184,   38,  163,
 /*  3510 */     1,    2,    3,    4,    5,  184,   46,  171,  172,  173,
 /*  3520 */   174,   51,  184,   53,  184,  184,  184,   57,   58,   12,
 /*  3530 */    13,   14,  184,  184,   17,   18,   19,   20,   21,   22,
 /*  3540 */    23,   24,   25,   26,   27,   28,   29,   30,  184,  184,
 /*  3550 */   184,  184,  184,   44,   45,  184,   47,   48,  184,   50,
 /*  3560 */    90,  184,  184,  184,   55,  184,  184,  184,   98,   99,
 /*  3570 */   100,  101,  102,  103,  163,   58,    7,    8,    9,   10,
 /*  3580 */    11,  184,  171,  172,  173,  174,  163,  184,   19,  184,
 /*  3590 */   167,  184,  184,  184,  171,  172,  173,  174,  184,  184,
 /*  3600 */    31,   32,   33,   34,   35,   36,  184,   38,  163,  184,
 /*  3610 */   184,  184,  167,  184,  184,   46,  171,  172,  173,  174,
 /*  3620 */    51,  184,   53,  184,  184,  184,   57,  184,   12,   13,
 /*  3630 */    14,  184,  184,   17,   18,   19,   20,   21,   22,   23,
 /*  3640 */    24,   25,   26,   27,   28,   29,   30,  184,  184,  184,
 /*  3650 */   184,  184,   49,  184,  184,   39,  184,   54,  184,   90,
 /*  3660 */    57,    1,    2,    3,    4,    5,  184,   98,   99,  100,
 /*  3670 */   101,  102,  103,  163,  184,    7,    8,    9,   10,   11,
 /*  3680 */   184,  171,  172,  173,  174,  184,  184,   19,   85,   86,
 /*  3690 */    87,   88,   89,  184,   91,   92,   93,  184,  184,   31,
 /*  3700 */    32,   33,   34,   35,   44,   45,   38,   47,   48,  184,
 /*  3710 */    50,  184,  184,  184,   46,   55,  184,  184,  184,   51,
 /*  3720 */   184,   53,  184,  184,  184,   57,  184,   12,   13,   14,
 /*  3730 */   184,  184,   17,   18,   19,   20,   21,   22,   23,   24,
 /*  3740 */    25,   26,   27,   28,   29,   30,  163,  184,  184,  184,
 /*  3750 */   184,   83,  184,  184,  171,  172,  173,  174,   90,  184,
 /*  3760 */     1,    2,    3,    4,    5,  184,   98,   99,  100,  101,
 /*  3770 */   102,  103,  184,   58,    7,    8,    9,   10,   11,  160,
 /*  3780 */   184,  184,  163,  184,  184,  184,   19,  184,  184,  184,
 /*  3790 */   171,  172,  173,  174,  184,  184,  184,  184,   31,   32,
 /*  3800 */    33,   34,   35,   44,   45,   38,   47,   48,    0,   50,
 /*  3810 */   184,  184,  184,   46,   55,  184,  184,  184,   51,  184,
 /*  3820 */    53,  184,  184,  184,   57,   58,   12,   13,   14,  184,
 /*  3830 */   184,   17,   18,   19,   20,   21,   22,   23,   24,   25,
 /*  3840 */    26,   27,   28,   29,   30,   37,  184,  184,   40,  184,
 /*  3850 */    42,  184,   44,   45,  184,  184,   48,   90,  184,    1,
 /*  3860 */     2,    3,    4,    5,  184,   98,   99,  100,  101,  102,
 /*  3870 */   103,  163,  184,    7,    8,    9,   10,   11,  184,  171,
 /*  3880 */   172,  173,  174,  163,  184,   19,  184,  184,  184,  184,
 /*  3890 */   184,  171,  172,  173,  174,  184,  184,   31,   32,   33,
 /*  3900 */    34,   35,   44,   45,   38,   47,   48,    1,    2,    3,
 /*  3910 */     4,    5,   46,   55,  184,  184,  184,   51,  184,   53,
 /*  3920 */   184,  184,  184,   57,   58,  184,   13,   14,  184,  184,
 /*  3930 */    17,   18,   19,   20,   21,   22,   23,   24,   25,   26,
 /*  3940 */    27,   28,   29,   30,  184,  184,  184,  184,  184,  184,
 /*  3950 */    44,   45,  184,   47,   48,  184,   90,  184,  184,  184,
 /*  3960 */   184,   55,  184,  184,   98,   99,  100,  101,  102,  103,
 /*  3970 */   163,  184,    7,    8,    9,   10,   11,  184,  171,  172,
 /*  3980 */   173,  174,  163,  184,   19,  184,  184,  184,  184,  184,
 /*  3990 */   171,  172,  173,  174,  184,  184,   31,   32,   33,   34,
 /*  4000 */    35,   36,  184,   38,  163,    1,    2,    3,    4,    5,
 /*  4010 */   184,   46,  171,  172,  173,  174,   51,  163,   53,  184,
 /*  4020 */   184,  184,   57,  184,  184,  171,  172,  173,  174,   17,
 /*  4030 */    18,   19,   20,   21,   22,   23,   24,   25,   26,   27,
 /*  4040 */    28,   29,   30,  184,  184,  184,  184,  184,   44,   45,
 /*  4050 */   184,   47,   48,  184,  184,   90,  184,  184,  184,   55,
 /*  4060 */   184,  184,  184,   98,   99,  100,  101,  102,  103,  184,
 /*  4070 */   184,    7,    8,    9,   10,   11,    1,    2,    3,    4,
 /*  4080 */     5,  163,  184,   19,  184,  184,  184,  184,  184,  171,
 /*  4090 */   172,  173,  174,  184,  184,   31,   32,   33,   34,   35,
 /*  4100 */   184,  184,   38,  163,  184,  184,  184,  184,  184,  184,
 /*  4110 */    46,  171,  172,  173,  174,   51,  184,   53,  163,   44,
 /*  4120 */    45,   57,   47,   48,  184,  184,  171,  172,  173,  174,
 /*  4130 */    55,  163,    1,    2,    3,    4,    5,  184,  184,  171,
 /*  4140 */   172,  173,  174,  184,  184,  184,  184,   83,  163,  184,
 /*  4150 */   184,  184,  184,  184,   90,  184,  171,  172,  173,  174,
 /*  4160 */   184,  184,   98,   99,  100,  101,  102,  103,  184,   38,
 /*  4170 */     7,    8,    9,   10,   11,   44,   45,  184,  184,  184,
 /*  4180 */   184,  184,   19,   52,  184,  184,   55,  184,    1,    2,
 /*  4190 */     3,    4,    5,  184,   31,   32,   33,   34,   35,   36,
 /*  4200 */   184,   38,  184,    1,    2,    3,    4,    5,  184,   46,
 /*  4210 */   184,  184,  184,  184,   51,  184,   53,  184,  184,  184,
 /*  4220 */    57,  184,  184,  184,  184,  184,  184,  184,  184,  184,
 /*  4230 */   184,   44,   45,  184,   47,   48,    1,    2,    3,    4,
 /*  4240 */     5,  184,   55,  184,  184,  184,   44,   45,  184,   47,
 /*  4250 */    48,  184,  184,   90,  184,  184,  184,   55,  184,  184,
 /*  4260 */   184,   98,   99,  100,  101,  102,  103,  184,  184,    7,
 /*  4270 */     8,    9,   10,   11,    1,    2,    3,    4,    5,   44,
 /*  4280 */    45,   19,   47,   48,  184,  184,  184,  184,  184,  184,
 /*  4290 */    55,  184,  184,   31,   32,   33,   34,   35,   36,  184,
 /*  4300 */    38,  184,    1,    2,    3,    4,    5,  184,   46,  184,
 /*  4310 */   184,  184,  184,   51,  184,   53,  184,   44,   45,   57,
 /*  4320 */    47,   48,  184,  184,  184,  184,  184,  184,   55,  184,
 /*  4330 */     1,    2,    3,    4,    5,  184,  184,  184,  184,   38,
 /*  4340 */   184,  184,  184,  184,  184,   44,   45,    1,    2,    3,
 /*  4350 */     4,    5,   90,   52,  184,  184,   55,  184,  184,  184,
 /*  4360 */    98,   99,  100,  101,  102,  103,  184,  184,    7,    8,
 /*  4370 */     9,   10,   11,   44,   45,  184,  184,  184,  184,   50,
 /*  4380 */    19,  184,  184,  184,   55,  184,  184,  184,  184,  184,
 /*  4390 */    44,   45,   31,   32,   33,   34,   35,  184,   52,   38,
 /*  4400 */   184,   55,  184,  184,  184,  184,  184,   46,  184,  184,
 /*  4410 */   184,  184,   51,  184,   53,  184,  184,  184,   57,  184,
 /*  4420 */   184,  184,  184,  184,  184,  184,  184,  184,  184,  184,
 /*  4430 */   184,  184,  184,  184,  184,  184,  184,  184,  184,  184,
 /*  4440 */   184,  184,  184,  184,  184,  184,  184,  184,  184,  184,
 /*  4450 */   184,   90,  184,    1,    2,    3,    4,    5,  184,   98,
 /*  4460 */    99,  100,  101,  102,  103,  184,  184,    7,    8,    9,
 /*  4470 */    10,   11,    1,    2,    3,    4,    5,  184,  184,   19,
 /*  4480 */   184,  184,  184,  184,  184,  184,  184,  184,  184,  184,
 /*  4490 */   184,   31,   32,   33,   34,   35,   44,   45,   38,  184,
 /*  4500 */   184,  184,  184,  184,   52,  184,   46,   55,  184,  184,
 /*  4510 */   184,   51,  184,   53,  184,   44,   45,   57,  184,  184,
 /*  4520 */   184,  184,  184,   52,  184,  184,   55,  184,  184,  184,
 /*  4530 */   184,  184,  184,  184,  184,  184,  184,  184,  184,  184,
 /*  4540 */   184,  184,  184,  184,  184,  184,  184,  184,  184,  184,
 /*  4550 */    90,  184,  184,  184,  184,  184,  184,  184,   98,   99,
 /*  4560 */   100,  101,  102,  103,
};
#define YY_SHIFT_USE_DFLT (-25)
static short yy_shift_ofst[] = {
 /*     0 */  1602,  226, 3808,  -25,  -25,  -25,  -25,  -25,   46,  173,
 /*    10 */   -25,  229,  111,  -25,  285,  224,  -25, 4187,  -25, 3906,
 /*    20 */   -25,  -25, 4346,  313,  318, 3292,  544, 2936,  350,  -25,
 /*    30 */   -25,  418, 3035,  -25,  442,  -25,  460,  445,  -25,  483,
 /*    40 */   507,  -25,  -25,  -25,  -25,  -25,  -25,  -25,  -25,  -25,
 /*    50 */   -25,  -25,  -25,  -25,  -25,  878,  817, 2936,  554,  -25,
 /*    60 */   -25, 3391,  -25,  565, 2551,  -25,  -25,  -25,  -25,  -25,
 /*    70 */   -25,  -25,  -25,  551,  607,  -25,  801,  603, 2551,  -25,
 /*    80 */   686,  700, 2551,  -25,  717,  720, 2551,  -25,  -25,  -25,
 /*    90 */   -25,  -25,  -25,  -25,  -25,  -25,  -25,  -25, 1624, 4452,
 /*   100 */   754,  759, 2698,  491, 2936,  780,  -25,  -25,  243,  914,
 /*   110 */  2936,  811,  -25,  -25,  -25,  -25,  802,  713,  -25,  834,
 /*   120 */   124,  -25,  835,    5,  -25, 3660,  -25,  868,  -25, 3759,
 /*   130 */  3858,  -25, 2915,  880,  890, 3094,  140, 1539,  -25,  525,
 /*   140 */   -25,  -25,  -25,  -25,  -25,  -25,  -25,  -25,  -25,  -25,
 /*   150 */   -25,  -25,  -25,  -25,  -25,  -25,  -25,  -25,  -25,  -25,
 /*   160 */   -25,  -25,  -25,  908,  -25,  930,  -25, 4361,  903,  331,
 /*   170 */    40,  -24,  925,  666,  816,  -25, 4361,  935,  329,  -25,
 /*   180 */   373,  -25,  -25, 2551,  936, 1592, 1592,  956,  234,  622,
 /*   190 */   -25, 4361,  960, 1151, 1495,  -25,  965,  763,  860,  934,
 /*   200 */  4361,  975,  -25, 4361,  -25, 4361,  977,  -25,  279, 4361,
 /*   210 */  4361, 3814, 4361, 4361, 3814, 4361, 4361, 3814, 4361, 4361,
 /*   220 */  3814, 4361, 4361, 3814, 4361,  -20, 4361,  -20, 4361,  -20,
 /*   230 */  4361,  131, 4361,  131, 4361,  -25, 4361,  -25, 4361,  -25,
 /*   240 */  4361,  131, 4361, 4012, 4361, 4012, 4361, 3913,   -7, 3022,
 /*   250 */   -25,  994, 4361,  -25,  995, 1012, 4361,  -20, 1202,  904,
 /*   260 */   981, 3074, 3814, 1001, 1034,  977,  -25,  -25, 4361,  -25,
 /*   270 */   -25,  -25,  -25,  -25,  -25,  -25,  -25,  -25, 3767, 3814,
 /*   280 */  2649,  701,  991, 3371,  509,  992, 3569,  -25,  199,  -25,
 /*   290 */  4361, 1008, 1000,   80, 3272,  952,  -25,  -25,  -25,  -25,
 /*   300 */   -25,  -25,  828,  997, 3965,  927,  -25,  -25, 1017, 1013,
 /*   310 */  1014, 4262,  899,  -25,  -25,  -25, 1027, 1019, 1015, 2777,
 /*   320 */   -25,  815,  -25,  728,  -25,  -25,  -25, 4361, 3319,  -25,
 /*   330 */  4163,  705,  -25,  -25, 1002, 1007,  -25,  788,  -25, 4460,
 /*   340 */   -25, 1009, 4361,  -25,  -25,  -25,  631,  -25,  -25,  537,
 /*   350 */  1052, 1050, 2975,  620,  -25,  -25, 1061, 1056, 2678,  564,
 /*   360 */   -25,  -25,  -20,  -20,  -20,  -20, 3517,  -25,  502,  -25,
 /*   370 */    39, 1063,  -25, 1064,  -25,  -20, 3814, 1074,  446, 3668,
 /*   380 */  3121, 1592,  278,  -25, 1084,  393,  -25, 1084,  -25, 3603,
 /*   390 */   -25,  -25,  -25,  -25,  -25,  -25, 4361,  -25, 3814,  149,
 /*   400 */   -19, 4361,  -25, 3866,  394, 4361,  -25, 1073,  394, 4361,
 /*   410 */   -25, 3715,  -25,  -25,    7, 1093,  394, 4361,  -25, 1094,
 /*   420 */   394, 4361,  -25, 1105,  297, 4361,  -25, 2876,  394, 4361,
 /*   430 */   -25, 1089,  394, 4361,  -25, 3470,  394, 4361,  -25, 1099,
 /*   440 */   394, 4361,  -25,  -25,  -25,  -25, 4361,  171,  -25, 4361,
 /*   450 */   -25, 3814,  -25, 1124,  -25, 1136,  -25, 1141,  -25, 1157,
 /*   460 */   -25, 3173, 2923,  -25,  -25, 1162,  214, 1167, 1168,  -25,
 /*   470 */  4361, 2725, 1169,  -25, 4361, 3616,  -25, 1171,  296,  -25,
 /*   480 */  1171,  -25, 1170, 2551,  -25,  -25, 1171,  462,  -25, 1171,
 /*   490 */   490,  -25, 1171,  520,  -25, 1171,  543,  -25, 1171,  639,
 /*   500 */   -25, 1171,  640,  -25, 1171,  709,  -25, 1171,  769,  -25,
 /*   510 */  1171,  781,  -25,  183, 1188,  991,  634,  981,  587, 1180,
 /*   520 */  1050, 1181, 1056,  -25,  -25, 4361, 2824, 1592,   84,  -25,
 /*   530 */  1195, 1152, 4064, 3418, 1592, 1054,  -25, 4361, 3220, 1592,
 /*   540 */   428,  -25, 1158, 4361, 1199,  -25,  -25,  -25, 1163, 1592,
 /*   550 */  1592,  -25,  -25, 1176, 1194,  375, 1442,  -25,  -25, 2936,
 /*   560 */  1198, 1398,  -25, 1248,  -25,  825,  799, 1107,  -25,  472,
 /*   570 */   -25, 2936, 1207,  569,  -25,  137,  -25, 1624, 4471, 1203,
 /*   580 */  1201, 2797,  863,  957,  -25,  719,  -25, 2936, 1211,  181,
 /*   590 */   -25, 1301,  -25,  910,  882, 1345,  -25,  913,  -25, 2936,
 /*   600 */  1212, 1010,  -25, 1204,  -25, 3509, 4235,  -25,  -25, 4329,
 /*   610 */  1209, 1213, 2551, 1233,  -25, 1226, 1230, 2551, 1263,  -25,
 /*   620 */   -25, 4329, 4301,   36,  -25, 2551,  519,  -25, 1266,  -25,
 /*   630 */    42, 1277,   34, 1282,   60,  -25,  -25, 1286,  -25,  -25,
 /*   640 */  1291,  -25, 4131,  412,  -25, 2551,  385,  -25, 1296,  -25,
 /*   650 */  1297,  -25,  -25, 3212, 4075, 4273, 1624, 4004,  -25, 4202,
 /*   660 */   835,  -25,  -25,  -25,  835,    5,  -25, 1308, 1313,  616,
 /*   670 */   -25, 1316, 1309,  -25,  835,    5,  -25, 1317, 1331,  649,
 /*   680 */   -25, 1338, 1309,  -25,  -25,
};
#define YY_REDUCE_USE_DFLT (-117)
static short yy_reduce_ofst[] = {
 /*     0 */   -92, -117,   30, -117, -117, -117, -117, -117, -117, -117,
 /*    10 */  -117, -117,  117, -117, -117,  170, -117, 1223, -117,  521,
 /*    20 */  -117, -117,  193, -117, -117,  347, -117,  926, -117, -117,
 /*    30 */  -117, -117,  154, -117, -117, -117, -117, -117, -117, -117,
 /*    40 */  -117, -117, -117, -117, -117, -117, -117, -117, -117, -117,
 /*    50 */  -117, -117, -117, -117, -117, -117, -117, 1023, -117, -117,
 /*    60 */  -117, -116, -117, -117,  494, -117, -117, -117, -117, -117,
 /*    70 */  -117, -117, -117, -117, -117, -117, -117, -117,  573, -117,
 /*    80 */  -117, -117,  629, -117, -117, -117,  650, -117, -117, -117,
 /*    90 */  -117, -117, -117, -117, -117, -117, -117, -117,  339,  193,
 /*   100 */  -117, -117,  423, -117,  732, -117, -117, -117, -117, -117,
 /*   110 */   635, -117, -117, -117, -117, -117, -117,  738, -117, -117,
 /*   120 */   775, -117,  192,  777, -117,  -87, -117, -117, -117, 1801,
 /*   130 */   -86, -117,  193, -117, -117,   35, -117, 1754, -117, 2522,
 /*   140 */  -117, -117, -117, -117, -117, -117, -117, -117, -117, -117,
 /*   150 */  -117, -117, -117, -117, -117, -117, -117, -117, -117, -117,
 /*   160 */  -117, -117, -117, -117, -117, -117, -117, 2752, -117, 2042,
 /*   170 */  2522, -117, -117, 2186, 2522, -117, 3346, -117,  206, -117,
 /*   180 */   808, -117, -117,  862, -117, 1850, 2522, -117, 2330, 2522,
 /*   190 */  -117, 2190, -117, 2474, 2522, -117, -117, 2234, 2522, -117,
 /*   200 */  3619, -117, -117, 3918, -117, 3985,  848, -117, -117,  730,
 /*   210 */  1021, -117,  924,  633, -117, 3213, 3312, -117, 3955, 3819,
 /*   220 */  -117,  827, 2718, -117, 2916, -117,  -46, -117, 3854, -117,
 /*   230 */   536, -117, 3114, -117,   51, -117,  439, -117, 3968, -117,
 /*   240 */  3841, -117, 3807, -117, 3583, -117, 3510, -117,  148, -117,
 /*   250 */  -117, -117, 3411, -117, -117, -117, 3708, -117, -117, -117,
 /*   260 */  -117, 1662, -117, -117, -117,  866, -117, -117, 1409, -117,
 /*   270 */  -117, -117, -117, -117, -117, -117, -117, -117, 1566, -117,
 /*   280 */  -117, -117, -117, 2046, -117, -117, 1950, -117, -117, -117,
 /*   290 */  2527, -117, -117, -117, 1902, -117, -117, -117, -117, -117,
 /*   300 */  -117, -117, -117, -117, 1710, -117, -117, -117, -117, -117,
 /*   310 */  -117, 1806, -117, -117, -117, -117, -117, -117, -117, 1998,
 /*   320 */  -117, -117, -117, -117, -117, -117, -117, 1118, -117, -117,
 /*   330 */  2094, -117, -117, -117, -117, -117, -117, -117, -117, 1614,
 /*   340 */  -117, -117, 2631, -117, -117, -117, -117, -117, -117, -117,
 /*   350 */  -117, -117, 2142, -117, -117, -117, -117, -117, 1854, -117,
 /*   360 */  -117, -117, -117, -117, -117, -117, -117, -117, -117, -117,
 /*   370 */  -117, -117, -117, -117, -117, -117, -117, -117, -117, 1312,
 /*   380 */  -117, 2090, 2522, -117,  200, -117, -117,  962, -117,  963,
 /*   390 */  -117, -117, -117, -117, -117, -117, 3445, -117, -117, -117,
 /*   400 */   964, 3423, -117, 3324,  970, 3225, -117, -117,  971, 3126,
 /*   410 */  -117, -117, -117, -117, -117, -117,  976, 3023, -117, -117,
 /*   420 */   978, 2896, -117, -117,  986, 2851, -117, 2730,  996, 2626,
 /*   430 */  -117, -117,  998, 2438, -117, 2391,  999, 2342, -117, -117,
 /*   440 */  1016, 2294, -117, -117, -117, -117,   19, -117, -117, 1527,
 /*   450 */  -117, -117, -117, -117, -117, -117, -117, -117, -117, -117,
 /*   460 */  -117, 1215, -117, -117, -117, -117, -117, -117, -117, -117,
 /*   470 */   245, -117, -117, -117, 3940, -117, -117,   63, -117, -117,
 /*   480 */  1030, -117, -117, 1100, -117, -117,  205, -117, -117,  212,
 /*   490 */  -117, -117,  254, -117, -117,  405, -117, -117,  448, -117,
 /*   500 */  -117,  545, -117, -117,  547, -117, -117,  567, -117, -117,
 /*   510 */   601, -117, -117, -117, -117, -117, -117, -117, -117, -117,
 /*   520 */  -117, -117, -117, -117, -117, 3720, -117, 2282, 2522, -117,
 /*   530 */  -117, -117,  342, -117, 1562, 2522, -117, 2817, -117, 1658,
 /*   540 */  2522, -117, -117, 2238, -117, -117, -117, -117, -117, 1994,
 /*   550 */  2522, -117, -117, -117, -117, 1898, 2522, -117, -117,  151,
 /*   560 */  -117, 1946, -117, 2522, -117, -117, -117, 2378, -117, 2522,
 /*   570 */  -117,  441, -117, 2138, -117, 2522, -117,  715,  193, -117,
 /*   580 */  -117,  326, -117, 1802, -117, 2522, -117,  538, -117, 1706,
 /*   590 */  -117, 2522, -117, -117, -117, 1610, -117, 2522, -117,  829,
 /*   600 */  -117, 2426, -117, 2522, -117,  232,  -86, -117, -117,  715,
 /*   610 */  -117, -117, 1137, -117, -117, -117, -117, 1173, -117, -117,
 /*   620 */  -117,  814,  193, 1178, -117, 1182, 1179, -117, -117, -117,
 /*   630 */   691, -117, -117, -117, 1196, -117, -117, -117, -117, -117,
 /*   640 */  -117, -117,  193, 1200, -117, 1210, 1206, -117, -117, -117,
 /*   650 */  -117, -117, -117,  135,  -58,  -86,  814,  -86, -117,  -86,
 /*   660 */  1220, -117, -117, -117,   -2, 1224, -117, -117, -117, 1238,
 /*   670 */  -117, -117, 1242, -117,  217, 1244, -117, -117, -117, 1260,
 /*   680 */  -117, -117, 1276, -117, -117,
};
static YYACTIONTYPE yy_default[] = {
 /*     0 */  1007, 1007, 1007,  687,  689,  690,  691,  692, 1007, 1007,
 /*    10 */   693, 1007, 1007,  694, 1007, 1007,  695, 1007,  710, 1007,
 /*    20 */   711,  741, 1007, 1007, 1007, 1007, 1007, 1007, 1007,  761,
 /*    30 */   779,  780, 1007,  781,  783,  784,  785, 1007,  786, 1007,
 /*    40 */  1007,  797,  798,  799,  800,  801,  802,  803,  804,  805,
 /*    50 */   806,  807,  808,  782,  765, 1007, 1007, 1007, 1007,  762,
 /*    60 */   766, 1007,  787,  789, 1007,  793,  993,  994,  995,  996,
 /*    70 */   997,  998,  999, 1007, 1007, 1000, 1007,  790, 1007,  794,
 /*    80 */  1007,  791, 1007,  795, 1007,  792, 1007,  796,  788,  769,
 /*    90 */   771,  772,  773,  774,  775,  776,  777,  778, 1007, 1007,
 /*   100 */  1007, 1007, 1007, 1007, 1007, 1007,  763,  767, 1007, 1007,
 /*   110 */  1007, 1007,  764,  768,  770,  742, 1007, 1007,  696, 1007,
 /*   120 */  1007,  697, 1007, 1007,  699, 1007,  705, 1007,  706, 1007,
 /*   130 */  1007,  739, 1007, 1007, 1007, 1007, 1007, 1007,  745, 1007,
 /*   140 */   747,  809,  811,  812,  813,  814,  815,  816,  817,  818,
 /*   150 */   819,  820,  821,  822,  823,  824,  825,  826,  827,  828,
 /*   160 */   829,  830,  831, 1007,  832, 1007,  833, 1007, 1007, 1007,
 /*   170 */  1007,  836, 1007, 1007, 1007,  837, 1007, 1007, 1007,  840,
 /*   180 */  1007,  841,  842, 1007, 1007,  844,  845, 1007, 1007, 1007,
 /*   190 */   848, 1007, 1007, 1007, 1007,  850, 1007, 1007, 1007, 1007,
 /*   200 */  1007, 1007,  852, 1007,  910, 1007, 1007,  911, 1007, 1007,
 /*   210 */  1007,  912, 1007, 1007,  913, 1007, 1007,  914, 1007, 1007,
 /*   220 */   915, 1007, 1007,  916, 1007,  922, 1007,  923, 1007,  924,
 /*   230 */  1007,  934, 1007,  935, 1007,  936, 1007,  937, 1007,  938,
 /*   240 */  1007,  939, 1007,  940, 1007,  941, 1007,  942, 1007, 1007,
 /*   250 */   925, 1007, 1007,  926, 1007, 1007, 1007,  927,  947, 1007,
 /*   260 */   928, 1007,  984, 1007, 1007, 1007,  943,  944, 1007,  945,
 /*   270 */   946,  948,  949,  950,  951,  952,  953,  954, 1007,  992,
 /*   280 */   947, 1007,  931, 1007, 1007,  957, 1007,  958, 1007,  959,
 /*   290 */  1007, 1007, 1007, 1007, 1007, 1007,  965,  966,  979,  980,
 /*   300 */   981,  983, 1007, 1007, 1007, 1007,  969,  970, 1007, 1007,
 /*   310 */  1007, 1007, 1007,  971,  972,  982, 1007, 1007,  960, 1007,
 /*   320 */   961, 1007,  962, 1007,  967,  968,  932, 1007, 1007,  933,
 /*   330 */  1007, 1007,  963,  964,  948,  949,  955, 1007,  956, 1007,
 /*   340 */   985, 1007, 1007,  987,  988,  986, 1007,  973,  974, 1007,
 /*   350 */  1007,  929, 1007, 1007,  975,  976, 1007,  930, 1007, 1007,
 /*   360 */   977,  978,  921,  920,  919,  918, 1007, 1001, 1007, 1002,
 /*   370 */  1007, 1007, 1003, 1007, 1004,  917, 1005, 1007, 1007, 1007,
 /*   380 */  1007, 1007, 1007,  853, 1007, 1007,  857, 1007,  858, 1007,
 /*   390 */   860,  861,  862,  863,  864,  865, 1007,  866,  909, 1007,
 /*   400 */  1007, 1007,  867, 1007, 1007, 1007,  870, 1007, 1007, 1007,
 /*   410 */   871, 1007,  877,  878, 1007, 1007, 1007, 1007,  868, 1007,
 /*   420 */  1007, 1007,  869, 1007, 1007, 1007,  872, 1007, 1007, 1007,
 /*   430 */   873, 1007, 1007, 1007,  874, 1007, 1007, 1007,  875, 1007,
 /*   440 */  1007, 1007,  876,  879,  880,  859, 1007, 1007,  882, 1007,
 /*   450 */   883,  885,  884, 1007,  886, 1007,  887, 1007,  888, 1007,
 /*   460 */   889, 1007, 1007,  890,  891, 1007, 1007, 1007, 1007,  892,
 /*   470 */  1007, 1007, 1007,  893, 1007, 1007,  894, 1007, 1007,  895,
 /*   480 */  1007,  905,  907, 1007,  908,  906, 1007, 1007,  896, 1007,
 /*   490 */  1007,  897, 1007, 1007,  898, 1007, 1007,  899, 1007, 1007,
 /*   500 */   900, 1007, 1007,  901, 1007, 1007,  902, 1007, 1007,  903,
 /*   510 */  1007, 1007,  904, 1007, 1007, 1007, 1007, 1007, 1007, 1007,
 /*   520 */  1007, 1007, 1007, 1006,  810, 1007, 1007, 1007, 1007,  854,
 /*   530 */  1007, 1007, 1007, 1007, 1007, 1007,  855, 1007, 1007, 1007,
 /*   540 */  1007,  856, 1007, 1007, 1007,  851,  849,  847, 1007, 1007,
 /*   550 */   846,  843,  838,  834, 1007, 1007, 1007,  839,  835, 1007,
 /*   560 */  1007, 1007,  753, 1007,  755, 1007, 1007, 1007,  746, 1007,
 /*   570 */   748, 1007, 1007, 1007,  754, 1007,  756, 1007, 1007, 1007,
 /*   580 */  1007, 1007, 1007, 1007,  749, 1007,  751, 1007, 1007, 1007,
 /*   590 */   757, 1007,  759, 1007, 1007, 1007,  750, 1007,  752, 1007,
 /*   600 */  1007, 1007,  758, 1007,  760, 1007, 1007,  737,  740, 1007,
 /*   610 */  1007, 1007, 1007, 1007,  743, 1007, 1007, 1007, 1007,  744,
 /*   620 */   721, 1007, 1007, 1007,  723, 1007, 1007,  725, 1007,  729,
 /*   630 */  1007, 1007, 1007, 1007, 1007,  733,  735, 1007,  736,  734,
 /*   640 */  1007,  727, 1007, 1007,  724, 1007, 1007,  726, 1007,  730,
 /*   650 */  1007,  728,  738, 1007, 1007, 1007, 1007, 1007,  722, 1007,
 /*   660 */  1007,  707,  709,  708, 1007, 1007,  698, 1007, 1007, 1007,
 /*   670 */   700, 1007, 1007,  701, 1007, 1007,  702, 1007, 1007, 1007,
 /*   680 */   703, 1007, 1007,  704,  688,
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
  "xx_method_return_type_item",  "xx_parameter_type",  "xx_parameter_cast",  "xx_parameter",
  "xx_statement",  "xx_let_statement",  "xx_if_statement",  "xx_loop_statement",
  "xx_echo_statement",  "xx_return_statement",  "xx_fetch_statement",  "xx_fcall_statement",
  "xx_mcall_statement",  "xx_scall_statement",  "xx_unset_statement",  "xx_throw_statement",
  "xx_declare_statement",  "xx_break_statement",  "xx_continue_statement",  "xx_while_statement",
  "xx_do_while_statement",  "xx_switch_statement",  "xx_for_statement",  "xx_empty_statement",
  "xx_eval_expr",  "xx_case_clauses",  "xx_case_clause",  "xx_common_expr",
  "xx_let_assignments",  "xx_let_assignment",  "xx_assignment_operator",  "xx_assign_expr",
  "xx_index_expr",  "xx_echo_expressions",  "xx_echo_expression",  "xx_mcall_expr",
  "xx_fcall_expr",  "xx_scall_expr",  "xx_fetch_expr",  "xx_declare_variable_list",
  "xx_declare_variable",  "xx_isset_expr",  "xx_array_list",  "xx_call_parameters",
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
 /* 101 */ "xx_method_return_type_item ::= xx_parameter_cast SBRACKET_OPEN SBRACKET_CLOSE",
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
 /* 113 */ "xx_parameter_type ::= TYPE_INTEGER",
 /* 114 */ "xx_parameter_type ::= TYPE_UINTEGER",
 /* 115 */ "xx_parameter_type ::= TYPE_LONG",
 /* 116 */ "xx_parameter_type ::= TYPE_ULONG",
 /* 117 */ "xx_parameter_type ::= TYPE_CHAR",
 /* 118 */ "xx_parameter_type ::= TYPE_UCHAR",
 /* 119 */ "xx_parameter_type ::= TYPE_DOUBLE",
 /* 120 */ "xx_parameter_type ::= TYPE_BOOL",
 /* 121 */ "xx_parameter_type ::= TYPE_STRING",
 /* 122 */ "xx_parameter_type ::= TYPE_ARRAY",
 /* 123 */ "xx_parameter_type ::= TYPE_VAR",
 /* 124 */ "xx_statement_list ::= xx_statement_list xx_statement",
 /* 125 */ "xx_statement_list ::= xx_statement",
 /* 126 */ "xx_statement ::= xx_let_statement",
 /* 127 */ "xx_statement ::= xx_if_statement",
 /* 128 */ "xx_statement ::= xx_loop_statement",
 /* 129 */ "xx_statement ::= xx_echo_statement",
 /* 130 */ "xx_statement ::= xx_return_statement",
 /* 131 */ "xx_statement ::= xx_fetch_statement",
 /* 132 */ "xx_statement ::= xx_fcall_statement",
 /* 133 */ "xx_statement ::= xx_mcall_statement",
 /* 134 */ "xx_statement ::= xx_scall_statement",
 /* 135 */ "xx_statement ::= xx_unset_statement",
 /* 136 */ "xx_statement ::= xx_throw_statement",
 /* 137 */ "xx_statement ::= xx_declare_statement",
 /* 138 */ "xx_statement ::= xx_break_statement",
 /* 139 */ "xx_statement ::= xx_continue_statement",
 /* 140 */ "xx_statement ::= xx_while_statement",
 /* 141 */ "xx_statement ::= xx_do_while_statement",
 /* 142 */ "xx_statement ::= xx_switch_statement",
 /* 143 */ "xx_statement ::= xx_for_statement",
 /* 144 */ "xx_statement ::= xx_comment",
 /* 145 */ "xx_statement ::= xx_empty_statement",
 /* 146 */ "xx_empty_statement ::= DOTCOMMA",
 /* 147 */ "xx_break_statement ::= BREAK DOTCOMMA",
 /* 148 */ "xx_continue_statement ::= CONTINUE DOTCOMMA",
 /* 149 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN BRACKET_CLOSE",
 /* 150 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN BRACKET_CLOSE ELSE BRACKET_OPEN BRACKET_CLOSE",
 /* 151 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 152 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE ELSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 153 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE ELSE BRACKET_OPEN BRACKET_CLOSE",
 /* 154 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN BRACKET_CLOSE ELSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 155 */ "xx_switch_statement ::= SWITCH xx_eval_expr BRACKET_OPEN BRACKET_CLOSE",
 /* 156 */ "xx_switch_statement ::= SWITCH xx_eval_expr BRACKET_OPEN xx_case_clauses BRACKET_CLOSE",
 /* 157 */ "xx_case_clauses ::= xx_case_clauses xx_case_clause",
 /* 158 */ "xx_case_clauses ::= xx_case_clause",
 /* 159 */ "xx_case_clause ::= CASE xx_literal_expr COLON",
 /* 160 */ "xx_case_clause ::= CASE xx_literal_expr COLON xx_statement_list",
 /* 161 */ "xx_case_clause ::= DEFAULT COLON xx_statement_list",
 /* 162 */ "xx_loop_statement ::= LOOP BRACKET_OPEN BRACKET_CLOSE",
 /* 163 */ "xx_loop_statement ::= LOOP BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 164 */ "xx_while_statement ::= WHILE xx_eval_expr BRACKET_OPEN BRACKET_CLOSE",
 /* 165 */ "xx_while_statement ::= WHILE xx_eval_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 166 */ "xx_do_while_statement ::= DO BRACKET_OPEN BRACKET_CLOSE WHILE xx_eval_expr DOTCOMMA",
 /* 167 */ "xx_do_while_statement ::= DO BRACKET_OPEN xx_statement_list BRACKET_CLOSE WHILE xx_eval_expr DOTCOMMA",
 /* 168 */ "xx_for_statement ::= FOR IDENTIFIER IN xx_common_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 169 */ "xx_for_statement ::= FOR IDENTIFIER IN REVERSE xx_common_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 170 */ "xx_for_statement ::= FOR IDENTIFIER COMMA IDENTIFIER IN xx_common_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 171 */ "xx_for_statement ::= FOR IDENTIFIER COMMA IDENTIFIER IN REVERSE xx_common_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 172 */ "xx_let_statement ::= LET xx_let_assignments DOTCOMMA",
 /* 173 */ "xx_let_assignments ::= xx_let_assignments COMMA xx_let_assignment",
 /* 174 */ "xx_let_assignments ::= xx_let_assignment",
 /* 175 */ "xx_assignment_operator ::= ASSIGN",
 /* 176 */ "xx_assignment_operator ::= ADDASSIGN",
 /* 177 */ "xx_assignment_operator ::= SUBASSIGN",
 /* 178 */ "xx_assignment_operator ::= MULASSIGN",
 /* 179 */ "xx_assignment_operator ::= DIVASSIGN",
 /* 180 */ "xx_assignment_operator ::= CONCATASSIGN",
 /* 181 */ "xx_let_assignment ::= IDENTIFIER xx_assignment_operator xx_assign_expr",
 /* 182 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER xx_assignment_operator xx_assign_expr",
 /* 183 */ "xx_let_assignment ::= IDENTIFIER ARROW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 184 */ "xx_let_assignment ::= IDENTIFIER ARROW BRACKET_OPEN STRING BRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 185 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER SBRACKET_OPEN SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 186 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER SBRACKET_OPEN xx_index_expr SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 187 */ "xx_let_assignment ::= IDENTIFIER DOUBLECOLON IDENTIFIER xx_assignment_operator xx_assign_expr",
 /* 188 */ "xx_let_assignment ::= IDENTIFIER DOUBLECOLON IDENTIFIER SBRACKET_OPEN SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 189 */ "xx_let_assignment ::= IDENTIFIER DOUBLECOLON IDENTIFIER SBRACKET_OPEN xx_index_expr SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 190 */ "xx_let_assignment ::= IDENTIFIER SBRACKET_OPEN SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 191 */ "xx_let_assignment ::= IDENTIFIER SBRACKET_OPEN xx_index_expr SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 192 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER INCR",
 /* 193 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER DECR",
 /* 194 */ "xx_let_assignment ::= IDENTIFIER INCR",
 /* 195 */ "xx_let_assignment ::= IDENTIFIER DECR",
 /* 196 */ "xx_index_expr ::= xx_common_expr",
 /* 197 */ "xx_echo_statement ::= ECHO xx_echo_expressions DOTCOMMA",
 /* 198 */ "xx_echo_expressions ::= xx_echo_expressions COMMA xx_echo_expression",
 /* 199 */ "xx_echo_expressions ::= xx_echo_expression",
 /* 200 */ "xx_echo_expression ::= xx_common_expr",
 /* 201 */ "xx_mcall_statement ::= xx_mcall_expr DOTCOMMA",
 /* 202 */ "xx_fcall_statement ::= xx_fcall_expr DOTCOMMA",
 /* 203 */ "xx_scall_statement ::= xx_scall_expr DOTCOMMA",
 /* 204 */ "xx_fetch_statement ::= xx_fetch_expr DOTCOMMA",
 /* 205 */ "xx_return_statement ::= RETURN xx_common_expr DOTCOMMA",
 /* 206 */ "xx_return_statement ::= RETURN DOTCOMMA",
 /* 207 */ "xx_unset_statement ::= UNSET IDENTIFIER ARROW IDENTIFIER DOTCOMMA",
 /* 208 */ "xx_unset_statement ::= UNSET IDENTIFIER SBRACKET_OPEN xx_common_expr SBRACKET_CLOSE DOTCOMMA",
 /* 209 */ "xx_throw_statement ::= THROW xx_common_expr DOTCOMMA",
 /* 210 */ "xx_declare_statement ::= TYPE_INTEGER xx_declare_variable_list DOTCOMMA",
 /* 211 */ "xx_declare_statement ::= TYPE_UINTEGER xx_declare_variable_list DOTCOMMA",
 /* 212 */ "xx_declare_statement ::= TYPE_CHAR xx_declare_variable_list DOTCOMMA",
 /* 213 */ "xx_declare_statement ::= TYPE_UCHAR xx_declare_variable_list DOTCOMMA",
 /* 214 */ "xx_declare_statement ::= TYPE_LONG xx_declare_variable_list DOTCOMMA",
 /* 215 */ "xx_declare_statement ::= TYPE_ULONG xx_declare_variable_list DOTCOMMA",
 /* 216 */ "xx_declare_statement ::= TYPE_DOUBLE xx_declare_variable_list DOTCOMMA",
 /* 217 */ "xx_declare_statement ::= TYPE_STRING xx_declare_variable_list DOTCOMMA",
 /* 218 */ "xx_declare_statement ::= TYPE_BOOL xx_declare_variable_list DOTCOMMA",
 /* 219 */ "xx_declare_statement ::= TYPE_VAR xx_declare_variable_list DOTCOMMA",
 /* 220 */ "xx_declare_variable_list ::= xx_declare_variable_list COMMA xx_declare_variable",
 /* 221 */ "xx_declare_variable_list ::= xx_declare_variable",
 /* 222 */ "xx_declare_variable ::= IDENTIFIER",
 /* 223 */ "xx_declare_variable ::= IDENTIFIER ASSIGN xx_literal_expr",
 /* 224 */ "xx_assign_expr ::= xx_common_expr",
 /* 225 */ "xx_common_expr ::= NOT xx_common_expr",
 /* 226 */ "xx_common_expr ::= ISSET xx_isset_expr",
 /* 227 */ "xx_common_expr ::= REQUIRE xx_common_expr",
 /* 228 */ "xx_common_expr ::= CLONE xx_common_expr",
 /* 229 */ "xx_common_expr ::= EMPTY xx_common_expr",
 /* 230 */ "xx_common_expr ::= LIKELY xx_common_expr",
 /* 231 */ "xx_common_expr ::= UNLIKELY xx_common_expr",
 /* 232 */ "xx_common_expr ::= xx_common_expr EQUALS xx_common_expr",
 /* 233 */ "xx_common_expr ::= xx_common_expr NOTEQUALS xx_common_expr",
 /* 234 */ "xx_common_expr ::= xx_common_expr IDENTICAL xx_common_expr",
 /* 235 */ "xx_common_expr ::= xx_common_expr NOTIDENTICAL xx_common_expr",
 /* 236 */ "xx_common_expr ::= xx_common_expr LESS xx_common_expr",
 /* 237 */ "xx_common_expr ::= xx_common_expr GREATER xx_common_expr",
 /* 238 */ "xx_common_expr ::= xx_common_expr LESSEQUAL xx_common_expr",
 /* 239 */ "xx_common_expr ::= xx_common_expr GREATEREQUAL xx_common_expr",
 /* 240 */ "xx_common_expr ::= PARENTHESES_OPEN xx_common_expr PARENTHESES_CLOSE",
 /* 241 */ "xx_common_expr ::= PARENTHESES_OPEN xx_parameter_type PARENTHESES_CLOSE xx_common_expr",
 /* 242 */ "xx_common_expr ::= LESS IDENTIFIER GREATER xx_common_expr",
 /* 243 */ "xx_common_expr ::= IDENTIFIER ARROW IDENTIFIER",
 /* 244 */ "xx_common_expr ::= IDENTIFIER ARROW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE",
 /* 245 */ "xx_common_expr ::= IDENTIFIER ARROW BRACKET_OPEN STRING BRACKET_CLOSE",
 /* 246 */ "xx_common_expr ::= IDENTIFIER DOUBLECOLON IDENTIFIER",
 /* 247 */ "xx_common_expr ::= IDENTIFIER DOUBLECOLON CONSTANT",
 /* 248 */ "xx_common_expr ::= IDENTIFIER SBRACKET_OPEN xx_common_expr SBRACKET_CLOSE",
 /* 249 */ "xx_common_expr ::= xx_common_expr ADD xx_common_expr",
 /* 250 */ "xx_common_expr ::= xx_common_expr SUB xx_common_expr",
 /* 251 */ "xx_common_expr ::= xx_common_expr MUL xx_common_expr",
 /* 252 */ "xx_common_expr ::= xx_common_expr DIV xx_common_expr",
 /* 253 */ "xx_common_expr ::= xx_common_expr MOD xx_common_expr",
 /* 254 */ "xx_common_expr ::= xx_common_expr CONCAT xx_common_expr",
 /* 255 */ "xx_common_expr ::= xx_common_expr AND xx_common_expr",
 /* 256 */ "xx_common_expr ::= xx_common_expr OR xx_common_expr",
 /* 257 */ "xx_common_expr ::= xx_common_expr INSTANCEOF xx_common_expr",
 /* 258 */ "xx_fetch_expr ::= FETCH IDENTIFIER COMMA xx_isset_expr",
 /* 259 */ "xx_common_expr ::= xx_fetch_expr",
 /* 260 */ "xx_common_expr ::= TYPEOF xx_common_expr",
 /* 261 */ "xx_common_expr ::= CONSTANT",
 /* 262 */ "xx_common_expr ::= IDENTIFIER",
 /* 263 */ "xx_common_expr ::= INTEGER",
 /* 264 */ "xx_common_expr ::= STRING",
 /* 265 */ "xx_common_expr ::= CHAR",
 /* 266 */ "xx_common_expr ::= DOUBLE",
 /* 267 */ "xx_common_expr ::= NULL",
 /* 268 */ "xx_common_expr ::= TRUE",
 /* 269 */ "xx_common_expr ::= FALSE",
 /* 270 */ "xx_common_expr ::= SBRACKET_OPEN SBRACKET_CLOSE",
 /* 271 */ "xx_common_expr ::= SBRACKET_OPEN xx_array_list SBRACKET_CLOSE",
 /* 272 */ "xx_common_expr ::= NEW IDENTIFIER",
 /* 273 */ "xx_common_expr ::= NEW IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 274 */ "xx_common_expr ::= NEW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 275 */ "xx_common_expr ::= NEW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE",
 /* 276 */ "xx_common_expr ::= NEW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 277 */ "xx_common_expr ::= NEW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 278 */ "xx_fcall_expr ::= IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 279 */ "xx_fcall_expr ::= IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 280 */ "xx_fcall_expr ::= BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 281 */ "xx_fcall_expr ::= BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 282 */ "xx_scall_expr ::= IDENTIFIER DOUBLECOLON IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 283 */ "xx_scall_expr ::= IDENTIFIER DOUBLECOLON IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 284 */ "xx_scall_expr ::= BRACKET_OPEN IDENTIFIER BRACKET_CLOSE DOUBLECOLON IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 285 */ "xx_scall_expr ::= BRACKET_OPEN IDENTIFIER BRACKET_CLOSE DOUBLECOLON IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 286 */ "xx_scall_expr ::= BRACKET_OPEN IDENTIFIER BRACKET_CLOSE DOUBLECOLON BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 287 */ "xx_scall_expr ::= BRACKET_OPEN IDENTIFIER BRACKET_CLOSE DOUBLECOLON BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 288 */ "xx_mcall_expr ::= IDENTIFIER ARROW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 289 */ "xx_mcall_expr ::= IDENTIFIER ARROW IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 290 */ "xx_mcall_expr ::= IDENTIFIER ARROW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 291 */ "xx_mcall_expr ::= IDENTIFIER ARROW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 292 */ "xx_mcall_expr ::= IDENTIFIER ARROW BRACKET_OPEN STRING BRACKET_CLOSE PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 293 */ "xx_mcall_expr ::= IDENTIFIER ARROW BRACKET_OPEN STRING BRACKET_CLOSE PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 294 */ "xx_common_expr ::= xx_mcall_expr",
 /* 295 */ "xx_common_expr ::= xx_scall_expr",
 /* 296 */ "xx_common_expr ::= xx_fcall_expr",
 /* 297 */ "xx_call_parameters ::= xx_call_parameters COMMA xx_call_parameter",
 /* 298 */ "xx_call_parameters ::= xx_call_parameter",
 /* 299 */ "xx_call_parameter ::= xx_common_expr",
 /* 300 */ "xx_array_list ::= xx_array_list COMMA xx_array_item",
 /* 301 */ "xx_array_list ::= xx_array_item",
 /* 302 */ "xx_array_item ::= xx_array_key COLON xx_array_value",
 /* 303 */ "xx_array_item ::= xx_array_value",
 /* 304 */ "xx_array_key ::= IDENTIFIER",
 /* 305 */ "xx_array_key ::= STRING",
 /* 306 */ "xx_array_key ::= INTEGER",
 /* 307 */ "xx_array_value ::= xx_common_expr",
 /* 308 */ "xx_literal_expr ::= INTEGER",
 /* 309 */ "xx_literal_expr ::= CHAR",
 /* 310 */ "xx_literal_expr ::= STRING",
 /* 311 */ "xx_literal_expr ::= DOUBLE",
 /* 312 */ "xx_literal_expr ::= NULL",
 /* 313 */ "xx_literal_expr ::= FALSE",
 /* 314 */ "xx_literal_expr ::= TRUE",
 /* 315 */ "xx_literal_expr ::= IDENTIFIER DOUBLECOLON CONSTANT",
 /* 316 */ "xx_isset_expr ::= IDENTIFIER SBRACKET_OPEN xx_common_expr SBRACKET_CLOSE",
 /* 317 */ "xx_isset_expr ::= IDENTIFIER ARROW IDENTIFIER",
 /* 318 */ "xx_isset_expr ::= IDENTIFIER ARROW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE",
 /* 319 */ "xx_isset_expr ::= IDENTIFIER ARROW BRACKET_OPEN STRING BRACKET_CLOSE",
 /* 320 */ "xx_eval_expr ::= xx_common_expr",
 /* 321 */ "xx_comment ::= COMMENT",
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
// 2802 "parser.c"
      break;
    case 106:
// 1012 "parser.lemon"
{ json_object_put((yypminor->yy239)); }
// 2807 "parser.c"
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
  { 136, 3 },
  { 131, 3 },
  { 131, 1 },
  { 139, 1 },
  { 139, 2 },
  { 139, 3 },
  { 139, 2 },
  { 139, 3 },
  { 139, 4 },
  { 139, 5 },
  { 139, 4 },
  { 138, 3 },
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
  { 140, 1 },
  { 140, 1 },
  { 140, 1 },
  { 140, 1 },
  { 140, 1 },
  { 140, 1 },
  { 140, 1 },
  { 140, 1 },
  { 140, 1 },
  { 140, 1 },
  { 140, 1 },
  { 140, 1 },
  { 140, 1 },
  { 140, 1 },
  { 140, 1 },
  { 140, 1 },
  { 140, 1 },
  { 140, 1 },
  { 140, 1 },
  { 140, 1 },
  { 159, 1 },
  { 153, 2 },
  { 154, 2 },
  { 142, 4 },
  { 142, 7 },
  { 142, 5 },
  { 142, 9 },
  { 142, 8 },
  { 142, 8 },
  { 157, 4 },
  { 157, 5 },
  { 161, 2 },
  { 161, 1 },
  { 162, 3 },
  { 162, 4 },
  { 162, 3 },
  { 143, 3 },
  { 143, 4 },
  { 155, 4 },
  { 155, 5 },
  { 156, 6 },
  { 156, 7 },
  { 158, 7 },
  { 158, 8 },
  { 158, 9 },
  { 158, 10 },
  { 141, 3 },
  { 164, 3 },
  { 164, 1 },
  { 166, 1 },
  { 166, 1 },
  { 166, 1 },
  { 166, 1 },
  { 166, 1 },
  { 166, 1 },
  { 165, 3 },
  { 165, 5 },
  { 165, 7 },
  { 165, 7 },
  { 165, 7 },
  { 165, 8 },
  { 165, 5 },
  { 165, 7 },
  { 165, 8 },
  { 165, 5 },
  { 165, 6 },
  { 165, 4 },
  { 165, 4 },
  { 165, 2 },
  { 165, 2 },
  { 168, 1 },
  { 144, 3 },
  { 169, 3 },
  { 169, 1 },
  { 170, 1 },
  { 148, 2 },
  { 147, 2 },
  { 149, 2 },
  { 146, 2 },
  { 145, 3 },
  { 145, 2 },
  { 150, 5 },
  { 150, 6 },
  { 151, 3 },
  { 152, 3 },
  { 152, 3 },
  { 152, 3 },
  { 152, 3 },
  { 152, 3 },
  { 152, 3 },
  { 152, 3 },
  { 152, 3 },
  { 152, 3 },
  { 152, 3 },
  { 175, 3 },
  { 175, 1 },
  { 176, 1 },
  { 176, 3 },
  { 167, 1 },
  { 163, 2 },
  { 163, 2 },
  { 163, 2 },
  { 163, 2 },
  { 163, 2 },
  { 163, 2 },
  { 163, 2 },
  { 163, 3 },
  { 163, 3 },
  { 163, 3 },
  { 163, 3 },
  { 163, 3 },
  { 163, 3 },
  { 163, 3 },
  { 163, 3 },
  { 163, 3 },
  { 163, 4 },
  { 163, 4 },
  { 163, 3 },
  { 163, 5 },
  { 163, 5 },
  { 163, 3 },
  { 163, 3 },
  { 163, 4 },
  { 163, 3 },
  { 163, 3 },
  { 163, 3 },
  { 163, 3 },
  { 163, 3 },
  { 163, 3 },
  { 163, 3 },
  { 163, 3 },
  { 163, 3 },
  { 174, 4 },
  { 163, 1 },
  { 163, 2 },
  { 163, 1 },
  { 163, 1 },
  { 163, 1 },
  { 163, 1 },
  { 163, 1 },
  { 163, 1 },
  { 163, 1 },
  { 163, 1 },
  { 163, 1 },
  { 163, 2 },
  { 163, 3 },
  { 163, 2 },
  { 163, 4 },
  { 163, 5 },
  { 163, 4 },
  { 163, 6 },
  { 163, 7 },
  { 172, 4 },
  { 172, 3 },
  { 172, 6 },
  { 172, 5 },
  { 173, 6 },
  { 173, 5 },
  { 173, 8 },
  { 173, 7 },
  { 173, 10 },
  { 173, 9 },
  { 171, 6 },
  { 171, 5 },
  { 171, 8 },
  { 171, 7 },
  { 171, 8 },
  { 171, 7 },
  { 163, 1 },
  { 163, 1 },
  { 163, 1 },
  { 179, 3 },
  { 179, 1 },
  { 180, 1 },
  { 178, 3 },
  { 178, 1 },
  { 181, 3 },
  { 181, 1 },
  { 182, 1 },
  { 182, 1 },
  { 182, 1 },
  { 183, 1 },
  { 124, 1 },
  { 124, 1 },
  { 124, 1 },
  { 124, 1 },
  { 124, 1 },
  { 124, 1 },
  { 124, 1 },
  { 124, 3 },
  { 177, 4 },
  { 177, 3 },
  { 177, 5 },
  { 177, 5 },
  { 160, 1 },
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
	status->ret = yymsp[0].minor.yy239;
}
// 3346 "parser.c"
        break;
      case 1:
      case 4:
      case 5:
      case 6:
      case 7:
      case 126:
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
      case 196:
      case 224:
      case 259:
      case 294:
      case 295:
      case 296:
      case 299:
      case 307:
      case 320:
// 1014 "parser.lemon"
{
	yygotominor.yy239 = yymsp[0].minor.yy239;
}
// 3386 "parser.c"
        break;
      case 2:
      case 36:
      case 52:
      case 54:
      case 56:
      case 84:
      case 124:
      case 157:
// 1018 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_list(yymsp[-1].minor.yy239, yymsp[0].minor.yy239);
}
// 3400 "parser.c"
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
      case 125:
      case 158:
      case 174:
      case 199:
      case 221:
      case 298:
      case 301:
// 1022 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_list(NULL, yymsp[0].minor.yy239);
}
// 3423 "parser.c"
        break;
      case 8:
// 1042 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_namespace(yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(37,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 3432 "parser.c"
        break;
      case 9:
// 1046 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_interface(yymsp[-1].minor.yy0, yymsp[0].minor.yy239, NULL, status->scanner_state);
  yy_destructor(40,&yymsp[-2].minor);
}
// 3440 "parser.c"
        break;
      case 10:
// 1050 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_interface(yymsp[-3].minor.yy0, yymsp[0].minor.yy239, yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(40,&yymsp[-4].minor);
  yy_destructor(41,&yymsp[-2].minor);
}
// 3449 "parser.c"
        break;
      case 11:
// 1054 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_class(yymsp[-1].minor.yy0, yymsp[0].minor.yy239, 0, 0, NULL, NULL, status->scanner_state);
  yy_destructor(42,&yymsp[-2].minor);
}
// 3457 "parser.c"
        break;
      case 12:
// 1058 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy239, 0, 0, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(42,&yymsp[-4].minor);
  yy_destructor(41,&yymsp[-2].minor);
}
// 3466 "parser.c"
        break;
      case 13:
// 1062 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy239, 0, 0, NULL, yymsp[-1].minor.yy239, status->scanner_state);
  yy_destructor(42,&yymsp[-4].minor);
  yy_destructor(43,&yymsp[-2].minor);
}
// 3475 "parser.c"
        break;
      case 14:
// 1066 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_class(yymsp[-5].minor.yy0, yymsp[0].minor.yy239, 0, 0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy239, status->scanner_state);
  yy_destructor(42,&yymsp[-6].minor);
  yy_destructor(41,&yymsp[-4].minor);
  yy_destructor(43,&yymsp[-2].minor);
}
// 3485 "parser.c"
        break;
      case 15:
// 1070 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_class(yymsp[-1].minor.yy0, yymsp[0].minor.yy239, 1, 0, NULL, NULL, status->scanner_state);
  yy_destructor(44,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-2].minor);
}
// 3494 "parser.c"
        break;
      case 16:
// 1074 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy239, 1, 0, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(44,&yymsp[-5].minor);
  yy_destructor(42,&yymsp[-4].minor);
  yy_destructor(41,&yymsp[-2].minor);
}
// 3504 "parser.c"
        break;
      case 17:
// 1078 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy239, 1, 0, NULL, yymsp[-1].minor.yy239, status->scanner_state);
  yy_destructor(44,&yymsp[-5].minor);
  yy_destructor(42,&yymsp[-4].minor);
  yy_destructor(43,&yymsp[-2].minor);
}
// 3514 "parser.c"
        break;
      case 18:
// 1082 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_class(yymsp[-1].minor.yy0, yymsp[0].minor.yy239, 0, 1, NULL, NULL, status->scanner_state);
  yy_destructor(45,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-2].minor);
}
// 3523 "parser.c"
        break;
      case 19:
// 1086 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy239, 0, 1, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(45,&yymsp[-5].minor);
  yy_destructor(42,&yymsp[-4].minor);
  yy_destructor(41,&yymsp[-2].minor);
}
// 3533 "parser.c"
        break;
      case 20:
      case 25:
      case 46:
// 1090 "parser.lemon"
{
	yygotominor.yy239 = NULL;
  yy_destructor(46,&yymsp[-1].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 3544 "parser.c"
        break;
      case 21:
      case 26:
      case 47:
// 1094 "parser.lemon"
{
	yygotominor.yy239 = yymsp[-1].minor.yy239;
  yy_destructor(46,&yymsp[-2].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 3555 "parser.c"
        break;
      case 22:
      case 48:
      case 102:
      case 173:
      case 198:
      case 220:
      case 297:
      case 300:
// 1098 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_list(yymsp[-2].minor.yy239, yymsp[0].minor.yy239);
  yy_destructor(6,&yymsp[-1].minor);
}
// 3570 "parser.c"
        break;
      case 24:
      case 262:
      case 304:
// 1106 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state);
}
// 3579 "parser.c"
        break;
      case 27:
// 1118 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_class_definition(yymsp[0].minor.yy239, NULL, NULL, status->scanner_state);
}
// 3586 "parser.c"
        break;
      case 28:
// 1122 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_class_definition(NULL, NULL, yymsp[0].minor.yy239, status->scanner_state);
}
// 3593 "parser.c"
        break;
      case 29:
// 1126 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_class_definition(NULL, yymsp[0].minor.yy239, NULL, status->scanner_state);
}
// 3600 "parser.c"
        break;
      case 30:
// 1130 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_class_definition(yymsp[-1].minor.yy239, yymsp[0].minor.yy239, NULL, status->scanner_state);
}
// 3607 "parser.c"
        break;
      case 31:
// 1134 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_class_definition(yymsp[-1].minor.yy239, NULL, yymsp[0].minor.yy239, status->scanner_state);
}
// 3614 "parser.c"
        break;
      case 32:
// 1138 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_class_definition(yymsp[0].minor.yy239, NULL, yymsp[-1].minor.yy239, status->scanner_state);
}
// 3621 "parser.c"
        break;
      case 33:
// 1142 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_class_definition(NULL, yymsp[0].minor.yy239, yymsp[-1].minor.yy239, status->scanner_state);
}
// 3628 "parser.c"
        break;
      case 34:
// 1146 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_class_definition(yymsp[-2].minor.yy239, yymsp[0].minor.yy239, yymsp[-1].minor.yy239, status->scanner_state);
}
// 3635 "parser.c"
        break;
      case 35:
// 1150 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_class_definition(yymsp[-1].minor.yy239, yymsp[0].minor.yy239, yymsp[-2].minor.yy239, status->scanner_state);
}
// 3642 "parser.c"
        break;
      case 38:
// 1167 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_class_property(yymsp[-2].minor.yy239, yymsp[-1].minor.yy0, NULL, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(39,&yymsp[0].minor);
}
// 3650 "parser.c"
        break;
      case 39:
// 1171 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_class_property(yymsp[-2].minor.yy239, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(39,&yymsp[0].minor);
}
// 3658 "parser.c"
        break;
      case 40:
// 1175 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_class_property(yymsp[-4].minor.yy239, yymsp[-3].minor.yy0, yymsp[-1].minor.yy239, yymsp[-5].minor.yy0, NULL, status->scanner_state);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 3667 "parser.c"
        break;
      case 41:
// 1179 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_class_property(yymsp[-4].minor.yy239, yymsp[-3].minor.yy0, yymsp[-1].minor.yy239, NULL, NULL, status->scanner_state);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 3676 "parser.c"
        break;
      case 42:
// 1183 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_class_property(yymsp[-3].minor.yy239, yymsp[-2].minor.yy0, NULL, yymsp[-4].minor.yy0, yymsp[-1].minor.yy239, status->scanner_state);
  yy_destructor(39,&yymsp[0].minor);
}
// 3684 "parser.c"
        break;
      case 43:
// 1187 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_class_property(yymsp[-3].minor.yy239, yymsp[-2].minor.yy0, NULL, NULL, yymsp[-1].minor.yy239, status->scanner_state);
  yy_destructor(39,&yymsp[0].minor);
}
// 3692 "parser.c"
        break;
      case 44:
// 1191 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_class_property(yymsp[-5].minor.yy239, yymsp[-4].minor.yy0, yymsp[-2].minor.yy239, yymsp[-6].minor.yy0, yymsp[-1].minor.yy239, status->scanner_state);
  yy_destructor(49,&yymsp[-3].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 3701 "parser.c"
        break;
      case 45:
// 1195 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_class_property(yymsp[-5].minor.yy239, yymsp[-4].minor.yy0, yymsp[-2].minor.yy239, NULL, yymsp[-1].minor.yy239, status->scanner_state);
  yy_destructor(49,&yymsp[-3].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 3710 "parser.c"
        break;
      case 50:
// 1215 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_property_shortcut(NULL, yymsp[0].minor.yy0, status->scanner_state);
}
// 3717 "parser.c"
        break;
      case 51:
// 1219 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_property_shortcut(yymsp[-1].minor.yy0, yymsp[0].minor.yy0, status->scanner_state);
}
// 3724 "parser.c"
        break;
      case 58:
// 1248 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy239, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(50,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 3734 "parser.c"
        break;
      case 59:
// 1252 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy239, NULL, status->scanner_state);
  yy_destructor(50,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 3744 "parser.c"
        break;
      case 60:
// 1257 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_class_method(yymsp[-6].minor.yy239, yymsp[-4].minor.yy0, NULL, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(52,&yymsp[-5].minor);
  yy_destructor(53,&yymsp[-3].minor);
  yy_destructor(36,&yymsp[-2].minor);
  yy_destructor(46,&yymsp[-1].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 3756 "parser.c"
        break;
      case 61:
// 1261 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_class_method(yymsp[-7].minor.yy239, yymsp[-5].minor.yy0, yymsp[-3].minor.yy239, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(52,&yymsp[-6].minor);
  yy_destructor(53,&yymsp[-4].minor);
  yy_destructor(36,&yymsp[-2].minor);
  yy_destructor(46,&yymsp[-1].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 3768 "parser.c"
        break;
      case 62:
// 1265 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_class_method(yymsp[-7].minor.yy239, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy239, NULL, NULL, status->scanner_state);
  yy_destructor(52,&yymsp[-6].minor);
  yy_destructor(53,&yymsp[-4].minor);
  yy_destructor(36,&yymsp[-3].minor);
  yy_destructor(46,&yymsp[-2].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 3780 "parser.c"
        break;
      case 63:
// 1269 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_class_method(yymsp[-8].minor.yy239, yymsp[-6].minor.yy0, yymsp[-4].minor.yy239, yymsp[-1].minor.yy239, NULL, NULL, status->scanner_state);
  yy_destructor(52,&yymsp[-7].minor);
  yy_destructor(53,&yymsp[-5].minor);
  yy_destructor(36,&yymsp[-3].minor);
  yy_destructor(46,&yymsp[-2].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 3792 "parser.c"
        break;
      case 64:
// 1273 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_class_method(yymsp[-6].minor.yy239, yymsp[-4].minor.yy0, NULL, NULL, yymsp[-7].minor.yy0, NULL, status->scanner_state);
  yy_destructor(52,&yymsp[-5].minor);
  yy_destructor(53,&yymsp[-3].minor);
  yy_destructor(36,&yymsp[-2].minor);
  yy_destructor(46,&yymsp[-1].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 3804 "parser.c"
        break;
      case 65:
// 1277 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_class_method(yymsp[-7].minor.yy239, yymsp[-5].minor.yy0, yymsp[-3].minor.yy239, NULL, yymsp[-8].minor.yy0, NULL, status->scanner_state);
  yy_destructor(52,&yymsp[-6].minor);
  yy_destructor(53,&yymsp[-4].minor);
  yy_destructor(36,&yymsp[-2].minor);
  yy_destructor(46,&yymsp[-1].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 3816 "parser.c"
        break;
      case 66:
// 1281 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_class_method(yymsp[-7].minor.yy239, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy239, yymsp[-8].minor.yy0, NULL, status->scanner_state);
  yy_destructor(52,&yymsp[-6].minor);
  yy_destructor(53,&yymsp[-4].minor);
  yy_destructor(36,&yymsp[-3].minor);
  yy_destructor(46,&yymsp[-2].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 3828 "parser.c"
        break;
      case 67:
// 1285 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_class_method(yymsp[-8].minor.yy239, yymsp[-6].minor.yy0, yymsp[-4].minor.yy239, yymsp[-1].minor.yy239, yymsp[-9].minor.yy0, NULL, status->scanner_state);
  yy_destructor(52,&yymsp[-7].minor);
  yy_destructor(53,&yymsp[-5].minor);
  yy_destructor(36,&yymsp[-3].minor);
  yy_destructor(46,&yymsp[-2].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 3840 "parser.c"
        break;
      case 68:
// 1289 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_class_method(yymsp[-8].minor.yy239, yymsp[-6].minor.yy0, NULL, NULL, NULL, yymsp[-2].minor.yy239, status->scanner_state);
  yy_destructor(52,&yymsp[-7].minor);
  yy_destructor(53,&yymsp[-5].minor);
  yy_destructor(36,&yymsp[-4].minor);
  yy_destructor(54,&yymsp[-3].minor);
  yy_destructor(46,&yymsp[-1].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 3853 "parser.c"
        break;
      case 69:
// 1293 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_class_method(yymsp[-9].minor.yy239, yymsp[-7].minor.yy0, yymsp[-5].minor.yy239, NULL, NULL, yymsp[-2].minor.yy239, status->scanner_state);
  yy_destructor(52,&yymsp[-8].minor);
  yy_destructor(53,&yymsp[-6].minor);
  yy_destructor(36,&yymsp[-4].minor);
  yy_destructor(54,&yymsp[-3].minor);
  yy_destructor(46,&yymsp[-1].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 3866 "parser.c"
        break;
      case 70:
// 1297 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_class_method(yymsp[-9].minor.yy239, yymsp[-7].minor.yy0, NULL, yymsp[-1].minor.yy239, NULL, yymsp[-3].minor.yy239, status->scanner_state);
  yy_destructor(52,&yymsp[-8].minor);
  yy_destructor(53,&yymsp[-6].minor);
  yy_destructor(36,&yymsp[-5].minor);
  yy_destructor(54,&yymsp[-4].minor);
  yy_destructor(46,&yymsp[-2].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 3879 "parser.c"
        break;
      case 71:
// 1301 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_class_method(yymsp[-10].minor.yy239, yymsp[-8].minor.yy0, yymsp[-6].minor.yy239, yymsp[-1].minor.yy239, NULL, yymsp[-3].minor.yy239, status->scanner_state);
  yy_destructor(52,&yymsp[-9].minor);
  yy_destructor(53,&yymsp[-7].minor);
  yy_destructor(36,&yymsp[-5].minor);
  yy_destructor(54,&yymsp[-4].minor);
  yy_destructor(46,&yymsp[-2].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 3892 "parser.c"
        break;
      case 72:
// 1305 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_class_method(yymsp[-8].minor.yy239, yymsp[-6].minor.yy0, NULL, NULL, yymsp[-9].minor.yy0, yymsp[-2].minor.yy239, status->scanner_state);
  yy_destructor(52,&yymsp[-7].minor);
  yy_destructor(53,&yymsp[-5].minor);
  yy_destructor(36,&yymsp[-4].minor);
  yy_destructor(54,&yymsp[-3].minor);
  yy_destructor(46,&yymsp[-1].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 3905 "parser.c"
        break;
      case 73:
// 1309 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_class_method(yymsp[-9].minor.yy239, yymsp[-7].minor.yy0, yymsp[-5].minor.yy239, NULL, yymsp[-10].minor.yy0, yymsp[-2].minor.yy239, status->scanner_state);
  yy_destructor(52,&yymsp[-8].minor);
  yy_destructor(53,&yymsp[-6].minor);
  yy_destructor(36,&yymsp[-4].minor);
  yy_destructor(54,&yymsp[-3].minor);
  yy_destructor(46,&yymsp[-1].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 3918 "parser.c"
        break;
      case 74:
// 1313 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_class_method(yymsp[-9].minor.yy239, yymsp[-7].minor.yy0, NULL, yymsp[-1].minor.yy239, yymsp[-10].minor.yy0, yymsp[-3].minor.yy239, status->scanner_state);
  yy_destructor(52,&yymsp[-8].minor);
  yy_destructor(53,&yymsp[-6].minor);
  yy_destructor(36,&yymsp[-5].minor);
  yy_destructor(54,&yymsp[-4].minor);
  yy_destructor(46,&yymsp[-2].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 3931 "parser.c"
        break;
      case 75:
// 1317 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_class_method(yymsp[-10].minor.yy239, yymsp[-8].minor.yy0, yymsp[-6].minor.yy239, yymsp[-1].minor.yy239, yymsp[-11].minor.yy0, yymsp[-3].minor.yy239, status->scanner_state);
  yy_destructor(52,&yymsp[-9].minor);
  yy_destructor(53,&yymsp[-7].minor);
  yy_destructor(36,&yymsp[-5].minor);
  yy_destructor(54,&yymsp[-4].minor);
  yy_destructor(46,&yymsp[-2].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 3944 "parser.c"
        break;
      case 76:
// 1322 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_class_method(yymsp[-7].minor.yy239, yymsp[-5].minor.yy0, NULL, NULL, NULL, yymsp[-1].minor.yy239, status->scanner_state);
  yy_destructor(52,&yymsp[-6].minor);
  yy_destructor(53,&yymsp[-4].minor);
  yy_destructor(36,&yymsp[-3].minor);
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 3956 "parser.c"
        break;
      case 77:
// 1326 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_class_method(yymsp[-8].minor.yy239, yymsp[-6].minor.yy0, yymsp[-4].minor.yy239, NULL, NULL, yymsp[-1].minor.yy239, status->scanner_state);
  yy_destructor(52,&yymsp[-7].minor);
  yy_destructor(53,&yymsp[-5].minor);
  yy_destructor(36,&yymsp[-3].minor);
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 3968 "parser.c"
        break;
      case 78:
// 1330 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_class_method(yymsp[-7].minor.yy239, yymsp[-5].minor.yy0, NULL, NULL, yymsp[-8].minor.yy0, yymsp[-1].minor.yy239, status->scanner_state);
  yy_destructor(52,&yymsp[-6].minor);
  yy_destructor(53,&yymsp[-4].minor);
  yy_destructor(36,&yymsp[-3].minor);
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 3980 "parser.c"
        break;
      case 79:
// 1334 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_class_method(yymsp[-8].minor.yy239, yymsp[-6].minor.yy0, yymsp[-4].minor.yy239, NULL, yymsp[-9].minor.yy0, yymsp[-1].minor.yy239, status->scanner_state);
  yy_destructor(52,&yymsp[-7].minor);
  yy_destructor(53,&yymsp[-5].minor);
  yy_destructor(36,&yymsp[-3].minor);
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 3992 "parser.c"
        break;
      case 80:
// 1338 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_class_method(yymsp[-5].minor.yy239, yymsp[-3].minor.yy0, NULL, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(52,&yymsp[-4].minor);
  yy_destructor(53,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[-1].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 4003 "parser.c"
        break;
      case 81:
// 1342 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_class_method(yymsp[-6].minor.yy239, yymsp[-4].minor.yy0, yymsp[-2].minor.yy239, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(52,&yymsp[-5].minor);
  yy_destructor(53,&yymsp[-3].minor);
  yy_destructor(36,&yymsp[-1].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 4014 "parser.c"
        break;
      case 82:
// 1346 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_class_method(yymsp[-5].minor.yy239, yymsp[-3].minor.yy0, NULL, NULL, yymsp[-6].minor.yy0, NULL, status->scanner_state);
  yy_destructor(52,&yymsp[-4].minor);
  yy_destructor(53,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[-1].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 4025 "parser.c"
        break;
      case 83:
// 1350 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_class_method(yymsp[-6].minor.yy239, yymsp[-4].minor.yy0, yymsp[-2].minor.yy239, NULL, yymsp[-7].minor.yy0, NULL, status->scanner_state);
  yy_destructor(52,&yymsp[-5].minor);
  yy_destructor(53,&yymsp[-3].minor);
  yy_destructor(36,&yymsp[-1].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 4036 "parser.c"
        break;
      case 86:
// 1363 "parser.lemon"
{
	yygotominor.yy239 = json_object_new_string("public");
  yy_destructor(1,&yymsp[0].minor);
}
// 4044 "parser.c"
        break;
      case 87:
// 1367 "parser.lemon"
{
	yygotominor.yy239 = json_object_new_string("protected");
  yy_destructor(2,&yymsp[0].minor);
}
// 4052 "parser.c"
        break;
      case 88:
// 1371 "parser.lemon"
{
	yygotominor.yy239 = json_object_new_string("private");
  yy_destructor(4,&yymsp[0].minor);
}
// 4060 "parser.c"
        break;
      case 89:
// 1375 "parser.lemon"
{
	yygotominor.yy239 = json_object_new_string("static");
  yy_destructor(3,&yymsp[0].minor);
}
// 4068 "parser.c"
        break;
      case 90:
// 1379 "parser.lemon"
{
	yygotominor.yy239 = json_object_new_string("scoped");
  yy_destructor(5,&yymsp[0].minor);
}
// 4076 "parser.c"
        break;
      case 91:
// 1383 "parser.lemon"
{
	yygotominor.yy239 = json_object_new_string("inline");
  yy_destructor(55,&yymsp[0].minor);
}
// 4084 "parser.c"
        break;
      case 92:
// 1387 "parser.lemon"
{
	yygotominor.yy239 = json_object_new_string("abstract");
  yy_destructor(44,&yymsp[0].minor);
}
// 4092 "parser.c"
        break;
      case 93:
// 1391 "parser.lemon"
{
	yygotominor.yy239 = json_object_new_string("final");
  yy_destructor(45,&yymsp[0].minor);
}
// 4100 "parser.c"
        break;
      case 94:
// 1396 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_return_type(1, NULL, status->scanner_state);
  yy_destructor(56,&yymsp[0].minor);
}
// 4108 "parser.c"
        break;
      case 95:
// 1400 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_return_type(0, yymsp[0].minor.yy239, status->scanner_state);
}
// 4115 "parser.c"
        break;
      case 96:
// 1404 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_list(yymsp[-2].minor.yy239, yymsp[0].minor.yy239);
  yy_destructor(15,&yymsp[-1].minor);
}
// 4123 "parser.c"
        break;
      case 98:
// 1412 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_return_type_item(yymsp[0].minor.yy239, NULL, 0, 0, status->scanner_state);
}
// 4130 "parser.c"
        break;
      case 99:
// 1416 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_return_type_item(yymsp[-1].minor.yy239, NULL, 1, 0, status->scanner_state);
  yy_destructor(35,&yymsp[0].minor);
}
// 4138 "parser.c"
        break;
      case 100:
// 1420 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_return_type_item(NULL, yymsp[0].minor.yy239, 0, 0, status->scanner_state);
}
// 4145 "parser.c"
        break;
      case 101:
// 1424 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_return_type_item(NULL, yymsp[-2].minor.yy239, 0, 1, status->scanner_state);
  yy_destructor(57,&yymsp[-1].minor);
  yy_destructor(58,&yymsp[0].minor);
}
// 4154 "parser.c"
        break;
      case 104:
// 1437 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_parameter(NULL, NULL, yymsp[0].minor.yy0, NULL, 0, status->scanner_state);
}
// 4161 "parser.c"
        break;
      case 105:
// 1441 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_parameter(yymsp[-1].minor.yy239, NULL, yymsp[0].minor.yy0, NULL, 0, status->scanner_state);
}
// 4168 "parser.c"
        break;
      case 106:
// 1445 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_parameter(yymsp[-2].minor.yy239, NULL, yymsp[0].minor.yy0, NULL, 1, status->scanner_state);
  yy_destructor(35,&yymsp[-1].minor);
}
// 4176 "parser.c"
        break;
      case 107:
// 1449 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_parameter(NULL, yymsp[-1].minor.yy239, yymsp[0].minor.yy0, NULL, 0, status->scanner_state);
}
// 4183 "parser.c"
        break;
      case 108:
// 1453 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_parameter(NULL, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy239, 0, status->scanner_state);
  yy_destructor(49,&yymsp[-1].minor);
}
// 4191 "parser.c"
        break;
      case 109:
// 1457 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_parameter(yymsp[-3].minor.yy239, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy239, 0, status->scanner_state);
  yy_destructor(49,&yymsp[-1].minor);
}
// 4199 "parser.c"
        break;
      case 110:
// 1461 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_parameter(yymsp[-4].minor.yy239, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy239, 1, status->scanner_state);
  yy_destructor(35,&yymsp[-3].minor);
  yy_destructor(49,&yymsp[-1].minor);
}
// 4208 "parser.c"
        break;
      case 111:
// 1465 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_parameter(NULL, yymsp[-3].minor.yy239, yymsp[-2].minor.yy0, yymsp[0].minor.yy239, 0, status->scanner_state);
  yy_destructor(49,&yymsp[-1].minor);
}
// 4216 "parser.c"
        break;
      case 112:
// 1469 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(19,&yymsp[-2].minor);
  yy_destructor(20,&yymsp[0].minor);
}
// 4225 "parser.c"
        break;
      case 113:
// 1473 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_type(XX_TYPE_INTEGER);
  yy_destructor(59,&yymsp[0].minor);
}
// 4233 "parser.c"
        break;
      case 114:
// 1477 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_type(XX_TYPE_UINTEGER);
  yy_destructor(60,&yymsp[0].minor);
}
// 4241 "parser.c"
        break;
      case 115:
// 1481 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_type(XX_TYPE_LONG);
  yy_destructor(61,&yymsp[0].minor);
}
// 4249 "parser.c"
        break;
      case 116:
// 1485 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_type(XX_TYPE_ULONG);
  yy_destructor(62,&yymsp[0].minor);
}
// 4257 "parser.c"
        break;
      case 117:
// 1489 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_type(XX_TYPE_CHAR);
  yy_destructor(63,&yymsp[0].minor);
}
// 4265 "parser.c"
        break;
      case 118:
// 1493 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_type(XX_TYPE_UCHAR);
  yy_destructor(64,&yymsp[0].minor);
}
// 4273 "parser.c"
        break;
      case 119:
// 1497 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_type(XX_TYPE_DOUBLE);
  yy_destructor(65,&yymsp[0].minor);
}
// 4281 "parser.c"
        break;
      case 120:
// 1501 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_type(XX_TYPE_BOOL);
  yy_destructor(66,&yymsp[0].minor);
}
// 4289 "parser.c"
        break;
      case 121:
// 1505 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_type(XX_TYPE_STRING);
  yy_destructor(67,&yymsp[0].minor);
}
// 4297 "parser.c"
        break;
      case 122:
// 1509 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_type(XX_TYPE_ARRAY);
  yy_destructor(68,&yymsp[0].minor);
}
// 4305 "parser.c"
        break;
      case 123:
// 1513 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_type(XX_TYPE_VAR);
  yy_destructor(69,&yymsp[0].minor);
}
// 4313 "parser.c"
        break;
      case 146:
// 1605 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_empty_statement(status->scanner_state);
  yy_destructor(39,&yymsp[0].minor);
}
// 4321 "parser.c"
        break;
      case 147:
// 1609 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_break_statement(status->scanner_state);
  yy_destructor(70,&yymsp[-1].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 4330 "parser.c"
        break;
      case 148:
// 1613 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_continue_statement(status->scanner_state);
  yy_destructor(71,&yymsp[-1].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 4339 "parser.c"
        break;
      case 149:
// 1617 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_if_statement(yymsp[-2].minor.yy239, NULL, NULL, status->scanner_state);
  yy_destructor(72,&yymsp[-3].minor);
  yy_destructor(46,&yymsp[-1].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 4349 "parser.c"
        break;
      case 150:
// 1621 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_if_statement(yymsp[-5].minor.yy239, NULL, NULL, status->scanner_state);
  yy_destructor(72,&yymsp[-6].minor);
  yy_destructor(46,&yymsp[-4].minor);
  yy_destructor(47,&yymsp[-3].minor);
  yy_destructor(73,&yymsp[-2].minor);
  yy_destructor(46,&yymsp[-1].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 4362 "parser.c"
        break;
      case 151:
// 1625 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_if_statement(yymsp[-3].minor.yy239, yymsp[-1].minor.yy239, NULL, status->scanner_state);
  yy_destructor(72,&yymsp[-4].minor);
  yy_destructor(46,&yymsp[-2].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 4372 "parser.c"
        break;
      case 152:
// 1629 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_if_statement(yymsp[-7].minor.yy239, yymsp[-5].minor.yy239, yymsp[-1].minor.yy239, status->scanner_state);
  yy_destructor(72,&yymsp[-8].minor);
  yy_destructor(46,&yymsp[-6].minor);
  yy_destructor(47,&yymsp[-4].minor);
  yy_destructor(73,&yymsp[-3].minor);
  yy_destructor(46,&yymsp[-2].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 4385 "parser.c"
        break;
      case 153:
// 1633 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_if_statement(yymsp[-6].minor.yy239, yymsp[-4].minor.yy239, NULL, status->scanner_state);
  yy_destructor(72,&yymsp[-7].minor);
  yy_destructor(46,&yymsp[-5].minor);
  yy_destructor(47,&yymsp[-3].minor);
  yy_destructor(73,&yymsp[-2].minor);
  yy_destructor(46,&yymsp[-1].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 4398 "parser.c"
        break;
      case 154:
// 1637 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_if_statement(yymsp[-6].minor.yy239, NULL, yymsp[-1].minor.yy239, status->scanner_state);
  yy_destructor(72,&yymsp[-7].minor);
  yy_destructor(46,&yymsp[-5].minor);
  yy_destructor(47,&yymsp[-4].minor);
  yy_destructor(73,&yymsp[-3].minor);
  yy_destructor(46,&yymsp[-2].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 4411 "parser.c"
        break;
      case 155:
// 1641 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_switch_statement(yymsp[-2].minor.yy239, NULL, status->scanner_state);
  yy_destructor(74,&yymsp[-3].minor);
  yy_destructor(46,&yymsp[-1].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 4421 "parser.c"
        break;
      case 156:
// 1645 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_switch_statement(yymsp[-3].minor.yy239, yymsp[-1].minor.yy239, status->scanner_state);
  yy_destructor(74,&yymsp[-4].minor);
  yy_destructor(46,&yymsp[-2].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 4431 "parser.c"
        break;
      case 159:
// 1657 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_case_clause(yymsp[-1].minor.yy239, NULL, status->scanner_state);
  yy_destructor(75,&yymsp[-2].minor);
  yy_destructor(76,&yymsp[0].minor);
}
// 4440 "parser.c"
        break;
      case 160:
// 1661 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_case_clause(yymsp[-2].minor.yy239, yymsp[0].minor.yy239, status->scanner_state);
  yy_destructor(75,&yymsp[-3].minor);
  yy_destructor(76,&yymsp[-1].minor);
}
// 4449 "parser.c"
        break;
      case 161:
// 1665 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_case_clause(NULL, yymsp[0].minor.yy239, status->scanner_state);
  yy_destructor(77,&yymsp[-2].minor);
  yy_destructor(76,&yymsp[-1].minor);
}
// 4458 "parser.c"
        break;
      case 162:
// 1669 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_loop_statement(NULL, status->scanner_state);
  yy_destructor(78,&yymsp[-2].minor);
  yy_destructor(46,&yymsp[-1].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 4468 "parser.c"
        break;
      case 163:
// 1673 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_loop_statement(yymsp[-1].minor.yy239, status->scanner_state);
  yy_destructor(78,&yymsp[-3].minor);
  yy_destructor(46,&yymsp[-2].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 4478 "parser.c"
        break;
      case 164:
// 1677 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_while_statement(yymsp[-2].minor.yy239, NULL, status->scanner_state);
  yy_destructor(79,&yymsp[-3].minor);
  yy_destructor(46,&yymsp[-1].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 4488 "parser.c"
        break;
      case 165:
// 1681 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_while_statement(yymsp[-3].minor.yy239, yymsp[-1].minor.yy239, status->scanner_state);
  yy_destructor(79,&yymsp[-4].minor);
  yy_destructor(46,&yymsp[-2].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 4498 "parser.c"
        break;
      case 166:
// 1685 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_do_while_statement(yymsp[-1].minor.yy239, NULL, status->scanner_state);
  yy_destructor(80,&yymsp[-5].minor);
  yy_destructor(46,&yymsp[-4].minor);
  yy_destructor(47,&yymsp[-3].minor);
  yy_destructor(79,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 4510 "parser.c"
        break;
      case 167:
// 1689 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_do_while_statement(yymsp[-1].minor.yy239, yymsp[-4].minor.yy239, status->scanner_state);
  yy_destructor(80,&yymsp[-6].minor);
  yy_destructor(46,&yymsp[-5].minor);
  yy_destructor(47,&yymsp[-3].minor);
  yy_destructor(79,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 4522 "parser.c"
        break;
      case 168:
// 1693 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_for_statement(yymsp[-3].minor.yy239, NULL, yymsp[-5].minor.yy0, 0, yymsp[-1].minor.yy239, status->scanner_state);
  yy_destructor(81,&yymsp[-6].minor);
  yy_destructor(82,&yymsp[-4].minor);
  yy_destructor(46,&yymsp[-2].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 4533 "parser.c"
        break;
      case 169:
// 1697 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_for_statement(yymsp[-3].minor.yy239, NULL, yymsp[-6].minor.yy0, 1, yymsp[-1].minor.yy239, status->scanner_state);
  yy_destructor(81,&yymsp[-7].minor);
  yy_destructor(82,&yymsp[-5].minor);
  yy_destructor(83,&yymsp[-4].minor);
  yy_destructor(46,&yymsp[-2].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 4545 "parser.c"
        break;
      case 170:
// 1701 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_for_statement(yymsp[-3].minor.yy239, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, 0, yymsp[-1].minor.yy239, status->scanner_state);
  yy_destructor(81,&yymsp[-8].minor);
  yy_destructor(6,&yymsp[-6].minor);
  yy_destructor(82,&yymsp[-4].minor);
  yy_destructor(46,&yymsp[-2].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 4557 "parser.c"
        break;
      case 171:
// 1705 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_for_statement(yymsp[-3].minor.yy239, yymsp[-8].minor.yy0, yymsp[-6].minor.yy0, 1, yymsp[-1].minor.yy239, status->scanner_state);
  yy_destructor(81,&yymsp[-9].minor);
  yy_destructor(6,&yymsp[-7].minor);
  yy_destructor(82,&yymsp[-5].minor);
  yy_destructor(83,&yymsp[-4].minor);
  yy_destructor(46,&yymsp[-2].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 4570 "parser.c"
        break;
      case 172:
// 1709 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_let_statement(yymsp[-1].minor.yy239, status->scanner_state);
  yy_destructor(84,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 4579 "parser.c"
        break;
      case 175:
// 1722 "parser.lemon"
{
	yygotominor.yy239 = json_object_new_string("assign");
  yy_destructor(49,&yymsp[0].minor);
}
// 4587 "parser.c"
        break;
      case 176:
// 1727 "parser.lemon"
{
	yygotominor.yy239 = json_object_new_string("add-assign");
  yy_destructor(85,&yymsp[0].minor);
}
// 4595 "parser.c"
        break;
      case 177:
// 1732 "parser.lemon"
{
	yygotominor.yy239 = json_object_new_string("sub-assign");
  yy_destructor(86,&yymsp[0].minor);
}
// 4603 "parser.c"
        break;
      case 178:
// 1736 "parser.lemon"
{
	yygotominor.yy239 = json_object_new_string("mult-assign");
  yy_destructor(87,&yymsp[0].minor);
}
// 4611 "parser.c"
        break;
      case 179:
// 1740 "parser.lemon"
{
	yygotominor.yy239 = json_object_new_string("div-assign");
  yy_destructor(88,&yymsp[0].minor);
}
// 4619 "parser.c"
        break;
      case 180:
// 1744 "parser.lemon"
{
	yygotominor.yy239 = json_object_new_string("concat-assign");
  yy_destructor(89,&yymsp[0].minor);
}
// 4627 "parser.c"
        break;
      case 181:
// 1749 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_let_assignment("variable", yymsp[-1].minor.yy239, yymsp[-2].minor.yy0, NULL, NULL, yymsp[0].minor.yy239, status->scanner_state);
}
// 4634 "parser.c"
        break;
      case 182:
// 1754 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_let_assignment("object-property", yymsp[-1].minor.yy239, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, yymsp[0].minor.yy239, status->scanner_state);
  yy_destructor(54,&yymsp[-3].minor);
}
// 4642 "parser.c"
        break;
      case 183:
// 1759 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_let_assignment("variable-dynamic-object-property", yymsp[-1].minor.yy239, yymsp[-6].minor.yy0, yymsp[-3].minor.yy0, NULL, yymsp[0].minor.yy239, status->scanner_state);
  yy_destructor(54,&yymsp[-5].minor);
  yy_destructor(46,&yymsp[-4].minor);
  yy_destructor(47,&yymsp[-2].minor);
}
// 4652 "parser.c"
        break;
      case 184:
// 1764 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_let_assignment("string-dynamic-object-property", yymsp[-1].minor.yy239, yymsp[-6].minor.yy0, yymsp[-3].minor.yy0, NULL, yymsp[0].minor.yy239, status->scanner_state);
  yy_destructor(54,&yymsp[-5].minor);
  yy_destructor(46,&yymsp[-4].minor);
  yy_destructor(47,&yymsp[-2].minor);
}
// 4662 "parser.c"
        break;
      case 185:
// 1769 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_let_assignment("object-property-append", yymsp[-1].minor.yy239, yymsp[-6].minor.yy0, yymsp[-4].minor.yy0, NULL, yymsp[0].minor.yy239, status->scanner_state);
  yy_destructor(54,&yymsp[-5].minor);
  yy_destructor(57,&yymsp[-3].minor);
  yy_destructor(58,&yymsp[-2].minor);
}
// 4672 "parser.c"
        break;
      case 186:
// 1774 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_let_assignment("object-property-array-index", yymsp[-1].minor.yy239, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, yymsp[-3].minor.yy239, yymsp[0].minor.yy239, status->scanner_state);
  yy_destructor(54,&yymsp[-6].minor);
  yy_destructor(57,&yymsp[-4].minor);
  yy_destructor(58,&yymsp[-2].minor);
}
// 4682 "parser.c"
        break;
      case 187:
// 1779 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_let_assignment("static-property", yymsp[-1].minor.yy239, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, yymsp[0].minor.yy239, status->scanner_state);
  yy_destructor(91,&yymsp[-3].minor);
}
// 4690 "parser.c"
        break;
      case 188:
// 1784 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_let_assignment("static-property-append", yymsp[-1].minor.yy239, yymsp[-6].minor.yy0, yymsp[-4].minor.yy0, NULL, yymsp[0].minor.yy239, status->scanner_state);
  yy_destructor(91,&yymsp[-5].minor);
  yy_destructor(57,&yymsp[-3].minor);
  yy_destructor(58,&yymsp[-2].minor);
}
// 4700 "parser.c"
        break;
      case 189:
// 1789 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_let_assignment("static-property-array-index", yymsp[-1].minor.yy239, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, yymsp[-3].minor.yy239, yymsp[0].minor.yy239, status->scanner_state);
  yy_destructor(91,&yymsp[-6].minor);
  yy_destructor(57,&yymsp[-4].minor);
  yy_destructor(58,&yymsp[-2].minor);
}
// 4710 "parser.c"
        break;
      case 190:
// 1794 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_let_assignment("variable-append", yymsp[-1].minor.yy239, yymsp[-4].minor.yy0, NULL, NULL, yymsp[0].minor.yy239, status->scanner_state);
  yy_destructor(57,&yymsp[-3].minor);
  yy_destructor(58,&yymsp[-2].minor);
}
// 4719 "parser.c"
        break;
      case 191:
// 1799 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_let_assignment("array-index", yymsp[-1].minor.yy239, yymsp[-5].minor.yy0, NULL, yymsp[-3].minor.yy239, yymsp[0].minor.yy239, status->scanner_state);
  yy_destructor(57,&yymsp[-4].minor);
  yy_destructor(58,&yymsp[-2].minor);
}
// 4728 "parser.c"
        break;
      case 192:
// 1804 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_let_assignment("incr", NULL, yymsp[-3].minor.yy0, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(92,&yymsp[0].minor);
}
// 4737 "parser.c"
        break;
      case 193:
// 1809 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_let_assignment("decr", NULL, yymsp[-3].minor.yy0, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(93,&yymsp[0].minor);
}
// 4746 "parser.c"
        break;
      case 194:
// 1814 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_let_assignment("incr", NULL, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(92,&yymsp[0].minor);
}
// 4754 "parser.c"
        break;
      case 195:
// 1819 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_let_assignment("decr", NULL, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(93,&yymsp[0].minor);
}
// 4762 "parser.c"
        break;
      case 197:
// 1827 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_echo_statement(yymsp[-1].minor.yy239, status->scanner_state);
  yy_destructor(94,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 4771 "parser.c"
        break;
      case 200:
// 1839 "parser.lemon"
{
	yygotominor.yy239 = yymsp[0].minor.yy239;;
}
// 4778 "parser.c"
        break;
      case 201:
// 1844 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_mcall_statement(yymsp[-1].minor.yy239, status->scanner_state);
  yy_destructor(39,&yymsp[0].minor);
}
// 4786 "parser.c"
        break;
      case 202:
// 1849 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_fcall_statement(yymsp[-1].minor.yy239, status->scanner_state);
  yy_destructor(39,&yymsp[0].minor);
}
// 4794 "parser.c"
        break;
      case 203:
// 1854 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_scall_statement(yymsp[-1].minor.yy239, status->scanner_state);
  yy_destructor(39,&yymsp[0].minor);
}
// 4802 "parser.c"
        break;
      case 204:
// 1859 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_fetch_statement(yymsp[-1].minor.yy239, status->scanner_state);
  yy_destructor(39,&yymsp[0].minor);
}
// 4810 "parser.c"
        break;
      case 205:
// 1864 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_return_statement(yymsp[-1].minor.yy239, status->scanner_state);
  yy_destructor(95,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 4819 "parser.c"
        break;
      case 206:
// 1869 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_return_statement(NULL, status->scanner_state);
  yy_destructor(95,&yymsp[-1].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 4828 "parser.c"
        break;
      case 207:
// 1874 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(96,&yymsp[-4].minor);
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 4842 "parser.c"
        break;
      case 208:
// 1883 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state),
		yymsp[-2].minor.yy239,
		status->scanner_state
	);
  yy_destructor(96,&yymsp[-5].minor);
  yy_destructor(57,&yymsp[-3].minor);
  yy_destructor(58,&yymsp[-1].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 4857 "parser.c"
        break;
      case 209:
// 1892 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_throw_exception(yymsp[-1].minor.yy239, status->scanner_state);
  yy_destructor(97,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 4866 "parser.c"
        break;
      case 210:
// 1896 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_declare_statement(XX_T_TYPE_INTEGER, yymsp[-1].minor.yy239, status->scanner_state);
  yy_destructor(59,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 4875 "parser.c"
        break;
      case 211:
// 1900 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_declare_statement(XX_T_TYPE_UINTEGER, yymsp[-1].minor.yy239, status->scanner_state);
  yy_destructor(60,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 4884 "parser.c"
        break;
      case 212:
// 1904 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_declare_statement(XX_T_TYPE_CHAR, yymsp[-1].minor.yy239, status->scanner_state);
  yy_destructor(63,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 4893 "parser.c"
        break;
      case 213:
// 1908 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_declare_statement(XX_T_TYPE_UCHAR, yymsp[-1].minor.yy239, status->scanner_state);
  yy_destructor(64,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 4902 "parser.c"
        break;
      case 214:
// 1912 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_declare_statement(XX_T_TYPE_LONG, yymsp[-1].minor.yy239, status->scanner_state);
  yy_destructor(61,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 4911 "parser.c"
        break;
      case 215:
// 1916 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_declare_statement(XX_T_TYPE_ULONG, yymsp[-1].minor.yy239, status->scanner_state);
  yy_destructor(62,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 4920 "parser.c"
        break;
      case 216:
// 1920 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_declare_statement(XX_T_TYPE_DOUBLE, yymsp[-1].minor.yy239, status->scanner_state);
  yy_destructor(65,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 4929 "parser.c"
        break;
      case 217:
// 1924 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_declare_statement(XX_T_TYPE_STRING, yymsp[-1].minor.yy239, status->scanner_state);
  yy_destructor(67,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 4938 "parser.c"
        break;
      case 218:
// 1928 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_declare_statement(XX_T_TYPE_BOOL, yymsp[-1].minor.yy239, status->scanner_state);
  yy_destructor(66,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 4947 "parser.c"
        break;
      case 219:
// 1932 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_declare_statement(XX_T_TYPE_VAR, yymsp[-1].minor.yy239, status->scanner_state);
  yy_destructor(69,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 4956 "parser.c"
        break;
      case 222:
// 1944 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_declare_variable(yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 4963 "parser.c"
        break;
      case 223:
// 1948 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_declare_variable(yymsp[-2].minor.yy0, yymsp[0].minor.yy239, status->scanner_state);
  yy_destructor(49,&yymsp[-1].minor);
}
// 4971 "parser.c"
        break;
      case 225:
// 1956 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_expr("not", yymsp[0].minor.yy239, NULL, NULL, status->scanner_state);
  yy_destructor(35,&yymsp[-1].minor);
}
// 4979 "parser.c"
        break;
      case 226:
// 1960 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_expr("isset", yymsp[0].minor.yy239, NULL, NULL, status->scanner_state);
  yy_destructor(31,&yymsp[-1].minor);
}
// 4987 "parser.c"
        break;
      case 227:
// 1964 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_expr("require", yymsp[0].minor.yy239, NULL, NULL, status->scanner_state);
  yy_destructor(7,&yymsp[-1].minor);
}
// 4995 "parser.c"
        break;
      case 228:
// 1968 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_expr("clone", yymsp[0].minor.yy239, NULL, NULL, status->scanner_state);
  yy_destructor(10,&yymsp[-1].minor);
}
// 5003 "parser.c"
        break;
      case 229:
// 1972 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_expr("empty", yymsp[0].minor.yy239, NULL, NULL, status->scanner_state);
  yy_destructor(11,&yymsp[-1].minor);
}
// 5011 "parser.c"
        break;
      case 230:
// 1976 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_expr("likely", yymsp[0].minor.yy239, NULL, NULL, status->scanner_state);
  yy_destructor(8,&yymsp[-1].minor);
}
// 5019 "parser.c"
        break;
      case 231:
// 1980 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_expr("unlikely", yymsp[0].minor.yy239, NULL, NULL, status->scanner_state);
  yy_destructor(9,&yymsp[-1].minor);
}
// 5027 "parser.c"
        break;
      case 232:
// 1984 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_expr("equals", yymsp[-2].minor.yy239, yymsp[0].minor.yy239, NULL, status->scanner_state);
  yy_destructor(17,&yymsp[-1].minor);
}
// 5035 "parser.c"
        break;
      case 233:
// 1988 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_expr("not-equals", yymsp[-2].minor.yy239, yymsp[0].minor.yy239, NULL, status->scanner_state);
  yy_destructor(24,&yymsp[-1].minor);
}
// 5043 "parser.c"
        break;
      case 234:
// 1992 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_expr("identical", yymsp[-2].minor.yy239, yymsp[0].minor.yy239, NULL, status->scanner_state);
  yy_destructor(18,&yymsp[-1].minor);
}
// 5051 "parser.c"
        break;
      case 235:
// 1996 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_expr("not-identical", yymsp[-2].minor.yy239, yymsp[0].minor.yy239, NULL, status->scanner_state);
  yy_destructor(23,&yymsp[-1].minor);
}
// 5059 "parser.c"
        break;
      case 236:
// 2000 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_expr("less", yymsp[-2].minor.yy239, yymsp[0].minor.yy239, NULL, status->scanner_state);
  yy_destructor(19,&yymsp[-1].minor);
}
// 5067 "parser.c"
        break;
      case 237:
// 2004 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_expr("greater", yymsp[-2].minor.yy239, yymsp[0].minor.yy239, NULL, status->scanner_state);
  yy_destructor(20,&yymsp[-1].minor);
}
// 5075 "parser.c"
        break;
      case 238:
// 2008 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_expr("less-equal", yymsp[-2].minor.yy239, yymsp[0].minor.yy239, NULL, status->scanner_state);
  yy_destructor(21,&yymsp[-1].minor);
}
// 5083 "parser.c"
        break;
      case 239:
// 2012 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_expr("greater-equal", yymsp[-2].minor.yy239, yymsp[0].minor.yy239, NULL, status->scanner_state);
  yy_destructor(22,&yymsp[-1].minor);
}
// 5091 "parser.c"
        break;
      case 240:
// 2016 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_expr("list", yymsp[-1].minor.yy239, NULL, NULL, status->scanner_state);
  yy_destructor(53,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5100 "parser.c"
        break;
      case 241:
// 2020 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_expr("cast", yymsp[-2].minor.yy239, yymsp[0].minor.yy239, NULL, status->scanner_state);
  yy_destructor(53,&yymsp[-3].minor);
  yy_destructor(36,&yymsp[-1].minor);
}
// 5109 "parser.c"
        break;
      case 242:
// 2024 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_expr("type-hint", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), yymsp[0].minor.yy239, NULL, status->scanner_state);
  yy_destructor(19,&yymsp[-3].minor);
  yy_destructor(20,&yymsp[-1].minor);
}
// 5118 "parser.c"
        break;
      case 243:
      case 317:
// 2028 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_expr("property-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(54,&yymsp[-1].minor);
}
// 5127 "parser.c"
        break;
      case 244:
      case 318:
// 2032 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_expr("property-dynamic-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(54,&yymsp[-3].minor);
  yy_destructor(46,&yymsp[-2].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 5138 "parser.c"
        break;
      case 245:
      case 319:
// 2036 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_expr("property-string-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_STRING, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(54,&yymsp[-3].minor);
  yy_destructor(46,&yymsp[-2].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 5149 "parser.c"
        break;
      case 246:
// 2040 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_expr("static-property-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(91,&yymsp[-1].minor);
}
// 5157 "parser.c"
        break;
      case 247:
      case 315:
// 2044 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_expr("static-constant-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(91,&yymsp[-1].minor);
}
// 5166 "parser.c"
        break;
      case 248:
      case 316:
// 2049 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), yymsp[-1].minor.yy239, NULL, status->scanner_state);
  yy_destructor(57,&yymsp[-2].minor);
  yy_destructor(58,&yymsp[0].minor);
}
// 5176 "parser.c"
        break;
      case 249:
// 2054 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_expr("add", yymsp[-2].minor.yy239, yymsp[0].minor.yy239, NULL, status->scanner_state);
  yy_destructor(25,&yymsp[-1].minor);
}
// 5184 "parser.c"
        break;
      case 250:
// 2059 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_expr("sub", yymsp[-2].minor.yy239, yymsp[0].minor.yy239, NULL, status->scanner_state);
  yy_destructor(26,&yymsp[-1].minor);
}
// 5192 "parser.c"
        break;
      case 251:
// 2064 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_expr("mul", yymsp[-2].minor.yy239, yymsp[0].minor.yy239, NULL, status->scanner_state);
  yy_destructor(28,&yymsp[-1].minor);
}
// 5200 "parser.c"
        break;
      case 252:
// 2069 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_expr("div", yymsp[-2].minor.yy239, yymsp[0].minor.yy239, NULL, status->scanner_state);
  yy_destructor(29,&yymsp[-1].minor);
}
// 5208 "parser.c"
        break;
      case 253:
// 2074 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_expr("mod", yymsp[-2].minor.yy239, yymsp[0].minor.yy239, NULL, status->scanner_state);
  yy_destructor(30,&yymsp[-1].minor);
}
// 5216 "parser.c"
        break;
      case 254:
// 2079 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_expr("concat", yymsp[-2].minor.yy239, yymsp[0].minor.yy239, NULL, status->scanner_state);
  yy_destructor(27,&yymsp[-1].minor);
}
// 5224 "parser.c"
        break;
      case 255:
// 2084 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_expr("and", yymsp[-2].minor.yy239, yymsp[0].minor.yy239, NULL, status->scanner_state);
  yy_destructor(13,&yymsp[-1].minor);
}
// 5232 "parser.c"
        break;
      case 256:
// 2089 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_expr("or", yymsp[-2].minor.yy239, yymsp[0].minor.yy239, NULL, status->scanner_state);
  yy_destructor(14,&yymsp[-1].minor);
}
// 5240 "parser.c"
        break;
      case 257:
// 2094 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_expr("instanceof", yymsp[-2].minor.yy239, yymsp[0].minor.yy239, NULL, status->scanner_state);
  yy_destructor(12,&yymsp[-1].minor);
}
// 5248 "parser.c"
        break;
      case 258:
// 2099 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_expr("fetch", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), yymsp[0].minor.yy239, NULL, status->scanner_state);
  yy_destructor(32,&yymsp[-3].minor);
  yy_destructor(6,&yymsp[-1].minor);
}
// 5257 "parser.c"
        break;
      case 260:
// 2109 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_expr("typeof", yymsp[0].minor.yy239, NULL, NULL, status->scanner_state);
  yy_destructor(33,&yymsp[-1].minor);
}
// 5265 "parser.c"
        break;
      case 261:
// 2114 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_literal(XX_T_CONSTANT, yymsp[0].minor.yy0, status->scanner_state);
}
// 5272 "parser.c"
        break;
      case 263:
      case 306:
      case 308:
// 2124 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_literal(XX_T_INTEGER, yymsp[0].minor.yy0, status->scanner_state);
}
// 5281 "parser.c"
        break;
      case 264:
      case 305:
      case 310:
// 2129 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_literal(XX_T_STRING, yymsp[0].minor.yy0, status->scanner_state);
}
// 5290 "parser.c"
        break;
      case 265:
      case 309:
// 2134 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_literal(XX_T_CHAR, yymsp[0].minor.yy0, status->scanner_state);
}
// 5298 "parser.c"
        break;
      case 266:
      case 311:
// 2139 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_literal(XX_T_DOUBLE, yymsp[0].minor.yy0, status->scanner_state);
}
// 5306 "parser.c"
        break;
      case 267:
      case 312:
// 2144 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_literal(XX_T_NULL, NULL, status->scanner_state);
  yy_destructor(101,&yymsp[0].minor);
}
// 5315 "parser.c"
        break;
      case 268:
      case 314:
// 2149 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_literal(XX_T_TRUE, NULL, status->scanner_state);
  yy_destructor(102,&yymsp[0].minor);
}
// 5324 "parser.c"
        break;
      case 269:
      case 313:
// 2154 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_literal(XX_T_FALSE, NULL, status->scanner_state);
  yy_destructor(103,&yymsp[0].minor);
}
// 5333 "parser.c"
        break;
      case 270:
// 2159 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_expr("empty-array", NULL, NULL, NULL, status->scanner_state);
  yy_destructor(57,&yymsp[-1].minor);
  yy_destructor(58,&yymsp[0].minor);
}
// 5342 "parser.c"
        break;
      case 271:
// 2164 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_expr("array", yymsp[-1].minor.yy239, NULL, NULL, status->scanner_state);
  yy_destructor(57,&yymsp[-2].minor);
  yy_destructor(58,&yymsp[0].minor);
}
// 5351 "parser.c"
        break;
      case 272:
// 2169 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_new_instance(0, yymsp[0].minor.yy0, NULL, status->scanner_state);
  yy_destructor(34,&yymsp[-1].minor);
}
// 5359 "parser.c"
        break;
      case 273:
// 2174 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_new_instance(0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(34,&yymsp[-3].minor);
  yy_destructor(53,&yymsp[-1].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5369 "parser.c"
        break;
      case 274:
// 2179 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_new_instance(0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy239, status->scanner_state);
  yy_destructor(34,&yymsp[-4].minor);
  yy_destructor(53,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5379 "parser.c"
        break;
      case 275:
// 2184 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_new_instance(1, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(34,&yymsp[-3].minor);
  yy_destructor(46,&yymsp[-2].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 5389 "parser.c"
        break;
      case 276:
// 2189 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_new_instance(1, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(34,&yymsp[-5].minor);
  yy_destructor(46,&yymsp[-4].minor);
  yy_destructor(47,&yymsp[-2].minor);
  yy_destructor(53,&yymsp[-1].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5401 "parser.c"
        break;
      case 277:
// 2194 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_new_instance(1, yymsp[-4].minor.yy0, yymsp[-1].minor.yy239, status->scanner_state);
  yy_destructor(34,&yymsp[-6].minor);
  yy_destructor(46,&yymsp[-5].minor);
  yy_destructor(47,&yymsp[-3].minor);
  yy_destructor(53,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5413 "parser.c"
        break;
      case 278:
// 2199 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_fcall(1, yymsp[-3].minor.yy0, yymsp[-1].minor.yy239, status->scanner_state);
  yy_destructor(53,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5422 "parser.c"
        break;
      case 279:
// 2204 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_fcall(1, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(53,&yymsp[-1].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5431 "parser.c"
        break;
      case 280:
// 2209 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_fcall(2, yymsp[-4].minor.yy0, yymsp[-1].minor.yy239, status->scanner_state);
  yy_destructor(46,&yymsp[-5].minor);
  yy_destructor(47,&yymsp[-3].minor);
  yy_destructor(53,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5442 "parser.c"
        break;
      case 281:
// 2214 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_fcall(2, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(46,&yymsp[-4].minor);
  yy_destructor(47,&yymsp[-2].minor);
  yy_destructor(53,&yymsp[-1].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5453 "parser.c"
        break;
      case 282:
// 2219 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_scall(0, yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy239, status->scanner_state);
  yy_destructor(91,&yymsp[-4].minor);
  yy_destructor(53,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5463 "parser.c"
        break;
      case 283:
// 2224 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_scall(0, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(91,&yymsp[-3].minor);
  yy_destructor(53,&yymsp[-1].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5473 "parser.c"
        break;
      case 284:
// 2229 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_scall(1, yymsp[-6].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy239, status->scanner_state);
  yy_destructor(46,&yymsp[-7].minor);
  yy_destructor(47,&yymsp[-5].minor);
  yy_destructor(91,&yymsp[-4].minor);
  yy_destructor(53,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5485 "parser.c"
        break;
      case 285:
// 2234 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_scall(1, yymsp[-5].minor.yy0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(46,&yymsp[-6].minor);
  yy_destructor(47,&yymsp[-4].minor);
  yy_destructor(91,&yymsp[-3].minor);
  yy_destructor(53,&yymsp[-1].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5497 "parser.c"
        break;
      case 286:
// 2239 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_scall(1, yymsp[-8].minor.yy0, yymsp[-4].minor.yy0, yymsp[-1].minor.yy239, status->scanner_state);
  yy_destructor(46,&yymsp[-9].minor);
  yy_destructor(47,&yymsp[-7].minor);
  yy_destructor(91,&yymsp[-6].minor);
  yy_destructor(46,&yymsp[-5].minor);
  yy_destructor(47,&yymsp[-3].minor);
  yy_destructor(53,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5511 "parser.c"
        break;
      case 287:
// 2244 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_scall(1, yymsp[-7].minor.yy0, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(46,&yymsp[-8].minor);
  yy_destructor(47,&yymsp[-6].minor);
  yy_destructor(91,&yymsp[-5].minor);
  yy_destructor(46,&yymsp[-4].minor);
  yy_destructor(47,&yymsp[-2].minor);
  yy_destructor(53,&yymsp[-1].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5525 "parser.c"
        break;
      case 288:
// 2249 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_mcall(1, yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy239, status->scanner_state);
  yy_destructor(54,&yymsp[-4].minor);
  yy_destructor(53,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5535 "parser.c"
        break;
      case 289:
// 2254 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_mcall(1, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(54,&yymsp[-3].minor);
  yy_destructor(53,&yymsp[-1].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5545 "parser.c"
        break;
      case 290:
// 2259 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_mcall(2, yymsp[-7].minor.yy0, yymsp[-4].minor.yy0, yymsp[-1].minor.yy239, status->scanner_state);
  yy_destructor(54,&yymsp[-6].minor);
  yy_destructor(46,&yymsp[-5].minor);
  yy_destructor(47,&yymsp[-3].minor);
  yy_destructor(53,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5557 "parser.c"
        break;
      case 291:
// 2264 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_mcall(2, yymsp[-6].minor.yy0, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(54,&yymsp[-5].minor);
  yy_destructor(46,&yymsp[-4].minor);
  yy_destructor(47,&yymsp[-2].minor);
  yy_destructor(53,&yymsp[-1].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5569 "parser.c"
        break;
      case 292:
// 2269 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_mcall(3, yymsp[-7].minor.yy0, yymsp[-4].minor.yy0, yymsp[-1].minor.yy239, status->scanner_state);
  yy_destructor(54,&yymsp[-6].minor);
  yy_destructor(46,&yymsp[-5].minor);
  yy_destructor(47,&yymsp[-3].minor);
  yy_destructor(53,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5581 "parser.c"
        break;
      case 293:
// 2274 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_mcall(3, yymsp[-6].minor.yy0, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(54,&yymsp[-5].minor);
  yy_destructor(46,&yymsp[-4].minor);
  yy_destructor(47,&yymsp[-2].minor);
  yy_destructor(53,&yymsp[-1].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5593 "parser.c"
        break;
      case 302:
// 2313 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_array_item(yymsp[-2].minor.yy239, yymsp[0].minor.yy239, status->scanner_state);
  yy_destructor(76,&yymsp[-1].minor);
}
// 5601 "parser.c"
        break;
      case 303:
// 2317 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_array_item(NULL, yymsp[0].minor.yy239, status->scanner_state);
}
// 5608 "parser.c"
        break;
      case 321:
// 2393 "parser.lemon"
{
	yygotominor.yy239 = xx_ret_comment(yymsp[0].minor.yy0, status->scanner_state);
}
// 5615 "parser.c"
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

// 5691 "parser.c"
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
