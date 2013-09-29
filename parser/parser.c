/** The author disclaims copyright to this source code.
*/
/* First off, code is include which follows the "include" declaration
** in the input file. */
#include <stdio.h>
// 48 "parser.lemon"


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

	json_object_object_add(ret, "abtract", json_object_new_int(is_abstract));
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

static json_object *xx_ret_parameter(json_object *type, json_object *cast, xx_parser_token *N, json_object *default_value, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("parameter"));
	json_object_object_add(ret, "name", json_object_new_string(N->token));

	if (type) {
		json_object_object_add(ret, "data-type", type);
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

static json_object *xx_ret_new_instance(xx_parser_token *T, json_object *parameters, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("new"));
	json_object_object_add(ret, "class", json_object_new_string(T->token));

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

static json_object *xx_ret_scall(xx_parser_token *O, xx_parser_token *M, json_object *parameters, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("scall"));
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


// 879 "parser.c"
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
#define YYNOCODE 178
#define YYACTIONTYPE unsigned short int
#define xx_TOKENTYPE xx_parser_token*
typedef union {
  xx_TOKENTYPE yy0;
  json_object* yy277;
  int yy355;
} YYMINORTYPE;
#define YYSTACKDEPTH 100
#define xx_ARG_SDECL xx_parser_status *status;
#define xx_ARG_PDECL ,xx_parser_status *status
#define xx_ARG_FETCH xx_parser_status *status = yypParser->status
#define xx_ARG_STORE yypParser->status = status
#define YYNSTATE 644
#define YYNRULE 303
#define YYERRORSYMBOL 99
#define YYERRSYMDT yy355
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
 /*     0 */   198,  201,  204,    3,    4,    5,    6,    7,  240,  948,
 /*    10 */     1,    2,  643,    4,    5,    6,    7,   68,   72,   54,
 /*    20 */   194,  249,  254,  270,  191,  115,  481,  244,  117,  612,
 /*    30 */   618,  617,  601,  120,  315,  277,  640,  611,  567,  119,
 /*    40 */   256,  113,  234,  102,  102,   35,   36,   37,   38,   39,
 /*    50 */    40,   41,   42,   43,   44,   45,  232,  228,  230,  193,
 /*    60 */   203,  208,  210,  212,  214,  206,  200,  216,  218,  226,
 /*    70 */   220,  222,  224,  220,  222,  224,  258,  264,  249,  593,
 /*    80 */    99,  440,   30,   31,  477,  150,  257,  259,  260,  261,
 /*    90 */   262,  263,  277,  347,  482,  524,  460,  449,  431,   68,
 /*   100 */    72,   76,  441,  450,  459,  462,  453,  456,  465,  471,
 /*   110 */   468,  586,  474,  151,  153,  155,  592,  164,  589,  249,
 /*   120 */   316,  175,  179,  184,  341,  477,  150,  348,  547,   65,
 /*   130 */    30,   31,  434,  277,  505,  482,  296,  245,  379,  410,
 /*   140 */   425,  429,  438,  441,  450,  459,  462,  453,  456,  465,
 /*   150 */   471,  468,  372,  474,  151,  153,  155,  613,  164,  616,
 /*   160 */   617,  601,  175,  179,  184,  341,  566,  567,  348,  322,
 /*   170 */   249,  293,  102,  267,  444,  610,  477,  150,   60,  444,
 /*   180 */   410,  425,  429,  438,  277,  128,  482,  383,   58,   59,
 /*   190 */    61,   62,   64,   63,  441,  450,  459,  462,  453,  456,
 /*   200 */   465,  471,  468,  461,  474,  151,  153,  155,  476,  164,
 /*   210 */   369,  249,  248,  175,  179,  184,  341,  477,  150,  348,
 /*   220 */   284,  286,  285,  253,  169,  277,  183,  482,  325,  287,
 /*   230 */   354,  410,  425,  429,  438,  441,  450,  459,  462,  453,
 /*   240 */   456,  465,  471,  468,  554,  474,  151,  153,  155,  171,
 /*   250 */   164,  507,  558,  489,  175,  179,  184,  341,  110,  276,
 /*   260 */   348,  113,  249,  355,  356,  357,  358,  359,  477,  150,
 /*   270 */    24,  444,  410,  425,  429,  438,  277,  495,  482,  216,
 /*   280 */   218,  226,  220,  222,  224,  313,  441,  450,  459,  462,
 /*   290 */   453,  456,  465,  471,  468,  606,  474,  151,  153,  155,
 /*   300 */   458,  164,  589,  249,  248,  175,  179,  184,  341,  477,
 /*   310 */   150,  348,  284,  286,  285,  253,  365,  277,  511,  482,
 /*   320 */   297,  287,  343,  410,  425,  429,  438,  441,  450,  459,
 /*   330 */   462,  453,  456,  465,  471,  468,  276,  474,  151,  153,
 /*   340 */   155,  332,  164,  614,  579,  601,  175,  179,  184,  341,
 /*   350 */   276,  567,  348,  624,  249,  622,  102,  296,  245,  335,
 /*   360 */   477,  150,  298,  192,  410,  425,  429,  438,  277,  517,
 /*   370 */   482,  284,  286,  285,  253,  197,  320,  929,  441,  450,
 /*   380 */   459,  462,  453,  456,  465,  471,  468,  621,  474,  151,
 /*   390 */   153,  155,  293,  164,  267,  249,  248,  175,  179,  184,
 /*   400 */   341,  477,  150,  348,  284,  286,  285,  253,  337,  277,
 /*   410 */   488,  482,  312,  287,  620,  410,  425,  429,  438,  441,
 /*   420 */   450,  459,  462,  453,  456,  465,  471,  468,   53,  474,
 /*   430 */   151,  153,  155,  196,  164,  565,  537,  120,  175,  179,
 /*   440 */   184,  341,  566,  567,  348,  336,  249,  102,  102,  296,
 /*   450 */   480,  195,  477,  150,   97,  211,  410,  425,  429,  438,
 /*   460 */   277,  516,  482,  284,  286,  285,  253,   77,  457,  449,
 /*   470 */   441,  450,  459,  462,  453,  456,  465,  471,  468,   14,
 /*   480 */   474,  151,  153,  155,   17,  164,  478,  249,  248,  175,
 /*   490 */   179,  184,  341,  477,  150,  348,  284,  286,  285,  253,
 /*   500 */   444,  277,  512,  482,  319,  287,  122,  410,  425,  429,
 /*   510 */   438,  441,  450,  459,  462,  453,  456,  465,  471,  468,
 /*   520 */   338,  474,  151,  153,  155,   93,  164,   30,   31,  443,
 /*   530 */   175,  179,  184,  341,  167,  581,  348,   96,  249,  442,
 /*   540 */   449,   68,   72,   76,  477,  150,  102,  485,  410,  425,
 /*   550 */   429,  438,  277,  163,  482,  284,  286,  285,  253,  171,
 /*   560 */   625,  507,  441,  450,  459,  462,  453,  456,  465,  471,
 /*   570 */   468,  627,  474,  151,  153,  155,  619,  164,  628,  249,
 /*   580 */   248,  175,  179,  184,  341,  477,  150,  348,  284,  286,
 /*   590 */   285,  253,  444,  277,  500,  482,  274,  287,   56,  410,
 /*   600 */   425,  429,  438,  441,  450,  459,  462,  453,  456,  465,
 /*   610 */   471,  468,  113,  474,  151,  153,  155,  531,  164,   30,
 /*   620 */    31,  464,  175,  179,  184,  341,  475,  449,  348,  552,
 /*   630 */   249,  463,  449,   68,   72,   76,  477,  150,   52,  209,
 /*   640 */   410,  425,  429,  438,  277,  521,  482,  284,  286,  285,
 /*   650 */   253,  591,  598,   49,  441,  450,  459,  462,  453,  456,
 /*   660 */   465,  471,  468,  360,  474,  151,  153,  155,  634,  164,
 /*   670 */   622,  249,  248,  175,  179,  184,  341,  477,  150,  348,
 /*   680 */   284,  286,  285,  253,  444,  277,  523,  482,  289,  287,
 /*   690 */   629,  410,  425,  429,  438,  441,  450,  459,  462,  453,
 /*   700 */   456,  465,  471,  468,  631,  474,  151,  153,  155,  101,
 /*   710 */   164,  113,   22,  455,  175,  179,  184,  341,  632,   21,
 /*   720 */   348,   47,  249,  102,   98,   68,   72,   76,  477,  150,
 /*   730 */    95,  207,  410,  425,  429,  438,  277,  527,  482,  284,
 /*   740 */   286,  285,  253,  352,   87,   92,  441,  450,  459,  462,
 /*   750 */   453,  456,  465,  471,  468,  102,  474,  151,  153,  155,
 /*   760 */   111,  164,  622,  249,  248,  175,  179,  184,  341,  477,
 /*   770 */   150,  348,  284,  286,  285,  253,  351,  277,  529,  482,
 /*   780 */   281,  287,  342,  410,  425,  429,  438,  441,  450,  459,
 /*   790 */   462,  453,  456,  465,  471,  468,  276,  474,  151,  153,
 /*   800 */   155,  559,  164,   30,   31,  350,  175,  179,  184,  341,
 /*   810 */   635,  107,  348,  623,  249,   50,  113,   30,   31,  353,
 /*   820 */   477,  150,  326,  339,  410,  425,  429,  438,  277,  533,
 /*   830 */   482,  284,  286,  285,  253,  637,  472,  449,  441,  450,
 /*   840 */   459,  462,  453,  456,  465,  471,  468,  638,  474,  151,
 /*   850 */   153,  155,  123,  164,  639,  249,  248,  175,  179,  184,
 /*   860 */   341,  477,  150,  348,  284,  286,  285,  253,  444,  277,
 /*   870 */   535,  482,  479,  288,  269,  410,  425,  429,  438,  441,
 /*   880 */   450,  459,  462,  453,  456,  465,  471,  468,   53,  474,
 /*   890 */   151,  153,  155,  519,  164,   30,   31,  470,  175,  179,
 /*   900 */   184,  341,  276,  630,  348,  633,  249,   28,  113,   30,
 /*   910 */    31,  271,  477,  150,  553,  205,  410,  425,  429,  438,
 /*   920 */   277,  543,  482,  284,  286,  285,  253,  305,  290,  595,
 /*   930 */   441,  450,  459,  462,  453,  456,  465,  471,  468,  596,
 /*   940 */   474,  151,  153,  155,  106,  164,  247,  249,  239,  175,
 /*   950 */   179,  184,  341,  477,  150,  348,  284,  286,  285,  253,
 /*   960 */   389,  277,  126,  482,  454,  449,  154,  410,  425,  429,
 /*   970 */   438,  441,  450,  459,  462,  453,  456,  465,  471,  468,
 /*   980 */   542,  474,  151,  153,  155,  595,  164,  446,  546,  246,
 /*   990 */   175,  179,  184,  341,  590,  596,  348,  315,  249,  451,
 /*  1000 */   449,   73,  444,  152,  477,  150,  304,  492,  410,  425,
 /*  1010 */   429,  438,  277,  545,  482,  284,  286,  285,  253,  105,
 /*  1020 */   349,  409,  441,  450,  459,  462,  453,  456,  465,  471,
 /*  1030 */   468,  452,  474,  151,  153,  155,   51,  164,  490,  249,
 /*  1040 */   223,  175,  179,  184,  341,  477,  150,  348,  284,  286,
 /*  1050 */   285,  253,  444,  277,  549,  482,  491,   46,  108,  410,
 /*  1060 */   425,  429,  438,  441,  450,  459,  462,  453,  456,  465,
 /*  1070 */   471,  468,   27,  474,  151,  153,  155,   15,  164,  603,
 /*  1080 */    71,  473,  175,  179,  184,  341,  589,  333,  348,  604,
 /*  1090 */   249,  469,  449,  466,  449,  334,  477,  150,  268,  221,
 /*  1100 */   410,  425,  429,  438,  277,  551,  482,  284,  286,  285,
 /*  1110 */   253,  292,  168,  510,  441,  450,  459,  462,  453,  456,
 /*  1120 */   465,  471,  468,  276,  474,  151,  153,  155,  278,  164,
 /*  1130 */    70,  249,  225,  175,  179,  184,  341,  477,  150,  348,
 /*  1140 */   284,  286,  285,  253,  444,  277,  555,  482,  157,  275,
 /*  1150 */   279,  410,  425,  429,  438,  441,  450,  459,  462,  453,
 /*  1160 */   456,  465,  471,  468,  364,  474,  151,  153,  155,  502,
 /*  1170 */   164,  583,  378,  467,  175,  179,  184,  341,  589,  125,
 /*  1180 */   348,  584,  249,  641,   74,  504,   69,  518,  477,  150,
 /*  1190 */   280,  217,  410,  425,  429,  438,  277,  159,  482,  284,
 /*  1200 */   286,  285,  253,   34,  508,  100,  441,  450,  459,  462,
 /*  1210 */   453,  456,  465,  471,  468,   53,  474,  151,  153,  155,
 /*  1220 */   161,  164,  160,  249,  233,  175,  179,  184,  341,  477,
 /*  1230 */   150,  348,  284,  286,  285,  253,  413,  277,  557,  482,
 /*  1240 */   109,  525,  513,  410,  425,  429,  438,  441,  450,  459,
 /*  1250 */   462,  453,  456,  465,  471,  468,  526,  474,  151,  153,
 /*  1260 */   155,   33,  164,  514,  530,  412,  175,  179,  184,  341,
 /*  1270 */    67,   53,  348,  520,  249,   66,    9,   12,  166,  447,
 /*  1280 */   477,  150,  448,  243,  410,  425,  429,  438,  277,  561,
 /*  1290 */   482,  284,  286,  285,  253,   94,  252,   48,  441,  450,
 /*  1300 */   459,  462,  453,  456,  465,  471,  468,  276,  474,  151,
 /*  1310 */   153,  155,  445,  164,  532,  249,  330,  175,  179,  184,
 /*  1320 */   341,  477,  150,  348,  284,  286,  285,  253,  170,  277,
 /*  1330 */   187,  482,  251,  282,  112,  410,  425,  429,  438,  441,
 /*  1340 */   450,  459,  462,  453,  456,  465,  471,  468,   75,  474,
 /*  1350 */   151,  153,  155,  539,  164,  250,  540,  437,  175,  179,
 /*  1360 */   184,  341,   13,  433,  348,  432,  249,  172,  173,  548,
 /*  1370 */   430,  931,  477,  150,   90,  329,  410,  425,  429,  438,
 /*  1380 */   277,  501,  482,  284,  286,  285,  253,  930,  242,  644,
 /*  1390 */   441,  450,  459,  462,  453,  456,  465,  471,  468,  241,
 /*  1400 */   474,  151,  153,  155,   89,  164,  424,  249,  328,  175,
 /*  1410 */   179,  184,  341,  477,  150,  348,  284,  286,  285,  253,
 /*  1420 */   176,  277,  506,  482,  238,  422,  560,  410,  425,  429,
 /*  1430 */   438,  441,  450,  459,  462,  453,  456,  465,  471,  468,
 /*  1440 */   420,  474,  151,  153,  155,   16,  164,  418,   29,  308,
 /*  1450 */   175,  179,  184,  341,  405,  570,  348,   17,  249,   10,
 /*  1460 */   116,  571,  404,  573,  477,  150,  572,  231,  410,  425,
 /*  1470 */   429,  438,  277,  563,  482,  284,  286,  285,  253,  181,
 /*  1480 */   575,  576,  441,  450,  459,  462,  453,  456,  465,  471,
 /*  1490 */   468,  642,  474,  151,  153,  155,  578,  164,  401,  249,
 /*  1500 */   229,  175,  179,  184,  341,  477,  150,  348,  284,  286,
 /*  1510 */   285,  253,  577,  277,  178,  482,  397,  396,  393,  410,
 /*  1520 */   425,  429,  438,  441,  450,  459,  462,  453,  456,  465,
 /*  1530 */   471,  468,  317,  474,  151,  153,  155,  272,  164,  599,
 /*  1540 */   388,  585,  175,  179,  184,  341,  587,  318,  348,  588,
 /*  1550 */   249,  185,  690,  385,  384,  691,  477,  150,  381,  199,
 /*  1560 */   410,  425,  429,  438,  277,   25,  482,  284,  286,  285,
 /*  1570 */   253,  594,  237,  323,  441,  450,  459,  462,  453,  456,
 /*  1580 */   465,  471,  468,  597,  474,  151,  153,  155,  324,  164,
 /*  1590 */   148,  188,   57,  175,  179,  184,  341,  380,  235,  348,
 /*  1600 */   600,   78,   79,   81,   80,   82,  284,  286,  285,  253,
 /*  1610 */   346,  410,  425,  429,  438,  609,  483,  130,  131,  132,
 /*  1620 */   133,  134,  135,  136,  137,  138,  139,  140,  141,  142,
 /*  1630 */   143,  144,  145,  146,  147,  149,  148,  190,  373,  608,
 /*  1640 */    84,   85,  605,  675,  568,  607,  574,  417,  419,  421,
 /*  1650 */   423,   83,  482,  482,  482,  482,  522,  482,  482,  482,
 /*  1660 */   482,  482,  483,  130,  131,  132,  133,  134,  135,  136,
 /*  1670 */   137,  138,  139,  140,  141,  142,  143,  144,  145,  146,
 /*  1680 */   147,  149,  148,  482,  482,  482,  482,  482,  482,  482,
 /*  1690 */   482,  482,  482,  417,  419,  421,  423,   78,   79,   81,
 /*  1700 */    80,   82,  556,  482,  482,  482,  482,  482,  483,  130,
 /*  1710 */   131,  132,  133,  134,  135,  136,  137,  138,  139,  140,
 /*  1720 */   141,  142,  143,  144,  145,  146,  147,  149,  148,  482,
 /*  1730 */   582,  482,  482,   19,  482,  482,   84,   85,   22,  417,
 /*  1740 */   419,  421,  423,  354,  538,  103,  482,   83,  528,  102,
 /*  1750 */   482,  482,  482,  482,  483,  130,  131,  132,  133,  134,
 /*  1760 */   135,  136,  137,  138,  139,  140,  141,  142,  143,  144,
 /*  1770 */   145,  146,  147,  149,  148,  482,  355,  356,  357,  358,
 /*  1780 */   359,  482,  391,  482,  482,  417,  419,  421,  423,   78,
 /*  1790 */    79,   81,   80,   82,  177,  482,  482,  482,  482,  482,
 /*  1800 */   483,  130,  131,  132,  133,  134,  135,  136,  137,  138,
 /*  1810 */   139,  140,  141,  142,  143,  144,  145,  146,  147,  149,
 /*  1820 */   148,  482,  602,  482,  482,  482,  482,  482,   84,   85,
 /*  1830 */   482,  417,  419,  421,  423,  482,  121,  482,  482,   83,
 /*  1840 */   499,  482,  482,  482,  482,  482,  483,  130,  131,  132,
 /*  1850 */   133,  134,  135,  136,  137,  138,  139,  140,  141,  142,
 /*  1860 */   143,  144,  145,  146,  147,  149,  148,  482,  482,  482,
 /*  1870 */   482,  482,  482,  482,  482,  482,  482,  417,  419,  421,
 /*  1880 */   423,   78,   79,   81,   80,   82,  534,  482,  482,  482,
 /*  1890 */   482,  482,  483,  130,  131,  132,  133,  134,  135,  136,
 /*  1900 */   137,  138,  139,  140,  141,  142,  143,  144,  145,  146,
 /*  1910 */   147,  149,  148,  482,  482,  482,  482,  482,  482,  482,
 /*  1920 */    84,   85,  482,  417,  419,  421,  423,  482,   88,  482,
 /*  1930 */   482,   83,  544,  482,  482,  482,  482,  482,  483,  130,
 /*  1940 */   131,  132,  133,  134,  135,  136,  137,  138,  139,  140,
 /*  1950 */   141,  142,  143,  144,  145,  146,  147,  149,  148,  482,
 /*  1960 */   482,  482,  482,  482,  482,  482,  482,  482,  482,  417,
 /*  1970 */   419,  421,  423,   78,   79,   81,   80,   82,  182,  482,
 /*  1980 */   482,  482,  482,  482,  483,  130,  131,  132,  133,  134,
 /*  1990 */   135,  136,  137,  138,  139,  140,  141,  142,  143,  144,
 /*  2000 */   145,  146,  147,  149,  148,  482,  482,  482,  482,  482,
 /*  2010 */   482,  482,   84,   85,  482,  417,  419,  421,  423,  482,
 /*  2020 */    23,  482,  482,   83,  509,  482,  482,  482,  482,  482,
 /*  2030 */   483,  130,  131,  132,  133,  134,  135,  136,  137,  138,
 /*  2040 */   139,  140,  141,  142,  143,  144,  145,  146,  147,  149,
 /*  2050 */   148,  482,  482,  482,  482,  482,  482,  482,  482,  482,
 /*  2060 */   482,  417,  419,  421,  423,   78,   79,   81,   80,   82,
 /*  2070 */   562,  482,  482,  482,  482,  482,  483,  130,  131,  132,
 /*  2080 */   133,  134,  135,  136,  137,  138,  139,  140,  141,  142,
 /*  2090 */   143,  144,  145,  146,  147,  149,  148,  482,  482,  482,
 /*  2100 */   482,  482,  482,  482,   84,   85,  482,  417,  419,  421,
 /*  2110 */   423,  482,  538,  482,  482,   83,  494,  482,  482,  482,
 /*  2120 */   482,  482,  483,  130,  131,  132,  133,  134,  135,  136,
 /*  2130 */   137,  138,  139,  140,  141,  142,  143,  144,  145,  146,
 /*  2140 */   147,  149,  148,  482,  482,  482,  482,  482,  482,  482,
 /*  2150 */   482,  482,  482,  417,  419,  421,  423,   78,   79,   81,
 /*  2160 */    80,   82,  158,  482,  482,  482,  482,  482,  483,  130,
 /*  2170 */   131,  132,  133,  134,  135,  136,  137,  138,  139,  140,
 /*  2180 */   141,  142,  143,  144,  145,  146,  147,  149,  148,  482,
 /*  2190 */   482,  482,  482,  482,  482,  482,   84,   85,  482,  417,
 /*  2200 */   419,  421,  423,  482,  121,  482,  482,   83,  550,  482,
 /*  2210 */   482,  482,  482,  482,  483,  130,  131,  132,  133,  134,
 /*  2220 */   135,  136,  137,  138,  139,  140,  141,  142,  143,  144,
 /*  2230 */   145,  146,  147,  149,  148,  482,  482,  482,  482,  482,
 /*  2240 */   482,  482,  482,  482,  482,  417,  419,  421,  423,   78,
 /*  2250 */    79,   81,   80,   82,  162,  482,  482,  482,  482,  482,
 /*  2260 */   483,  130,  131,  132,  133,  134,  135,  136,  137,  138,
 /*  2270 */   139,  140,  141,  142,  143,  144,  145,  146,  147,  149,
 /*  2280 */   148,  482,  482,  482,  156,  482,  482,  340,   84,   85,
 /*  2290 */   482,  417,  419,  421,  423,  284,  286,  285,  253,   83,
 /*  2300 */   487,  482,  482,  482,  482,  482,  483,  130,  131,  132,
 /*  2310 */   133,  134,  135,  136,  137,  138,  139,  140,  141,  142,
 /*  2320 */   143,  144,  145,  146,  147,  149,  148,  482,  482,  482,
 /*  2330 */   482,  482,  482,  564,  118,  579,  601,  417,  419,  421,
 /*  2340 */   423,  611,  567,  482,  482,  482,  515,  102,  482,  482,
 /*  2350 */   482,  482,  483,  130,  131,  132,  133,  134,  135,  136,
 /*  2360 */   137,  138,  139,  140,  141,  142,  143,  144,  145,  146,
 /*  2370 */   147,  149,  148,  482,  482,  482,  482,  482,  482,  482,
 /*  2380 */   415,  482,  482,  417,  419,  421,  423,  414,  284,  286,
 /*  2390 */   285,  253,  174,  482,  482,  482,  482,  482,  483,  130,
 /*  2400 */   131,  132,  133,  134,  135,  136,  137,  138,  139,  140,
 /*  2410 */   141,  142,  143,  144,  145,  146,  147,  149,  148,  482,
 /*  2420 */   482,  482,  503,  482,  482,  340,  482,  482,  482,  417,
 /*  2430 */   419,  421,  423,  284,  286,  285,  253,  482,  127,  482,
 /*  2440 */   482,  482,  482,  482,  483,  130,  131,  132,  133,  134,
 /*  2450 */   135,  136,  137,  138,  139,  140,  141,  142,  143,  144,
 /*  2460 */   145,  146,  147,  149,  148,  482,  482,  482,  165,  482,
 /*  2470 */   482,  340,  482,  482,  482,  417,  419,  421,  423,  284,
 /*  2480 */   286,  285,  253,  482,  186,  482,  482,  482,  482,  482,
 /*  2490 */   483,  130,  131,  132,  133,  134,  135,  136,  137,  138,
 /*  2500 */   139,  140,  141,  142,  143,  144,  145,  146,  147,  149,
 /*  2510 */   148,  482,  482,  482,  482,  482,  482,  482,  482,  415,
 /*  2520 */   482,  417,  419,  421,  423,  411,  416,  284,  286,  285,
 /*  2530 */   253,  482,  482,  482,  482,  482,  129,  130,  131,  132,
 /*  2540 */   133,  134,  135,  136,  137,  138,  139,  140,  141,  142,
 /*  2550 */   143,  144,  145,  146,  147,  149,  198,  201,  204,  482,
 /*  2560 */   180,  482,  482,  340,  240,  482,  482,  417,  419,  421,
 /*  2570 */   423,  284,  286,  285,  253,  482,  194,  249,  254,  270,
 /*  2580 */   191,  273,  482,  244,  198,  201,  204,  645,  482,  482,
 /*  2590 */   482,  277,  240,  482,  482,  482,  256,  482,  234,  482,
 /*  2600 */   482,  482,  482,  482,  194,  249,  254,  270,  191,  482,
 /*  2610 */   482,  266,  482,  482,  198,  201,  204,  482,  482,  277,
 /*  2620 */     8,  482,  240,   11,  256,  104,  234,  626,  636,  482,
 /*  2630 */   482,  482,  258,  264,  194,  249,  254,  270,  191,  482,
 /*  2640 */   482,  244,  257,  259,  260,  261,  262,  263,  482,  277,
 /*  2650 */   482,  482,  482,  482,  256,  482,  234,  198,  201,  204,
 /*  2660 */   301,  264,  302,  482,  482,  240,  482,  482,  482,  482,
 /*  2670 */   300,  259,  260,  261,  262,  263,  482,  194,  249,  254,
 /*  2680 */   270,  191,  291,  496,  244,  482,  198,  201,  204,  482,
 /*  2690 */   258,  264,  277,  482,  240,  482,  482,  256,  482,  234,
 /*  2700 */   257,  259,  260,  261,  262,  263,  194,  249,  254,  270,
 /*  2710 */   191,  482,  482,  244,  482,  198,  201,  204,  482,  482,
 /*  2720 */   482,  277,  482,  240,  482,  482,  256,  482,  234,  482,
 /*  2730 */   482,  482,  482,  258,  264,  194,  249,  254,  270,  191,
 /*  2740 */   482,  482,  244,  257,  259,  260,  261,  262,  263,  482,
 /*  2750 */   277,  482,  482,  482,  482,  256,  482,  234,  198,  201,
 /*  2760 */   204,  482,  258,  264,  368,  482,  240,  482,  482,  482,
 /*  2770 */   482,  482,  257,  259,  260,  261,  262,  263,  194,  249,
 /*  2780 */   254,  270,  191,  327,  484,  244,  482,  198,  201,  204,
 /*  2790 */   482,  258,  264,  277,  482,  240,  482,  482,  256,  482,
 /*  2800 */   234,  257,  259,  260,  261,  262,  263,  194,  249,  254,
 /*  2810 */   270,  191,  321,  482,  244,  482,  198,  201,  204,  482,
 /*  2820 */   482,  482,  277,  482,  240,  482,  482,  256,  482,  234,
 /*  2830 */   482,  482,  482,  482,  258,  264,  194,  249,  254,  270,
 /*  2840 */   191,  482,  482,  244,  257,  259,  260,  261,  262,  263,
 /*  2850 */   482,  277,  482,  482,  482,  482,  256,  362,  234,  482,
 /*  2860 */   482,  361,  482,  258,  264,  284,  286,  285,  253,  482,
 /*  2870 */   482,  482,  482,  257,  259,  260,  261,  262,  263,    8,
 /*  2880 */   354,  482,   11,  482,  104,  363,  626,  636,  482,  482,
 /*  2890 */   482,  482,  258,  264,  392,  482,  482,  482,  482,  482,
 /*  2900 */   482,  482,  257,  259,  260,  261,  262,  263,  482,  482,
 /*  2910 */   198,  201,  204,  355,  356,  357,  358,  359,  240,  399,
 /*  2920 */   482,  387,  407,  408,  482,  482,  482,  482,  482,  482,
 /*  2930 */   194,  249,  254,  270,  191,  482,  482,  244,  198,  201,
 /*  2940 */   204,  482,  482,  482,  482,  277,  240,  482,  482,  482,
 /*  2950 */   256,  482,  234,  482,  482,  482,  482,  482,  194,  249,
 /*  2960 */   254,  270,  191,  482,  482,  244,  428,   78,   79,   81,
 /*  2970 */    80,   82,  482,  277,  265,  482,  482,  482,  256,  482,
 /*  2980 */   234,  482,  284,  286,  285,  253,  258,  264,  400,  303,
 /*  2990 */   482,  482,  311,  307,  310,  482,  257,  259,  260,  261,
 /*  3000 */   262,  263,  482,  482,   32,  482,   84,   85,  482,  114,
 /*  3010 */   580,  482,  574,  482,  258,  264,  482,   83,  482,  482,
 /*  3020 */   482,   91,  482,   55,  257,  259,  260,  261,  262,  263,
 /*  3030 */   482,  482,  198,  201,  204,  482,  482,  482,  482,  482,
 /*  3040 */   240,   35,   36,   37,   38,   39,   40,   41,   42,   43,
 /*  3050 */    44,   45,  194,  249,  254,  270,  191,  314,  482,  244,
 /*  3060 */   198,  201,  204,  482,  482,  482,  482,  277,  240,  482,
 /*  3070 */   482,  482,  256,  482,  234,  482,  482,  482,  482,  482,
 /*  3080 */   194,  249,  254,  270,  191,  299,  482,  244,  482,  482,
 /*  3090 */   198,  201,  204,  482,  482,  277,  375,  482,  240,  482,
 /*  3100 */   256,  371,  234,  482,  284,  286,  285,  253,  258,  264,
 /*  3110 */   194,  249,  254,  270,  191,  283,  482,  244,  257,  259,
 /*  3120 */   260,  261,  262,  263,  482,  277,  482,  482,  482,  482,
 /*  3130 */   256,  482,  234,  198,  201,  204,  258,  264,  482,  482,
 /*  3140 */   482,  240,  482,  482,  482,  482,  257,  259,  260,  261,
 /*  3150 */   262,  263,  482,  194,  249,  254,  270,  191,  482,  482,
 /*  3160 */   244,  482,  198,  201,  204,  482,  258,  264,  277,  482,
 /*  3170 */   240,  482,  482,  256,  482,  234,  257,  259,  260,  261,
 /*  3180 */   262,  263,  194,  249,  254,  270,  191,  189,  482,  266,
 /*  3190 */   340,   78,   79,   81,   80,   82,  482,  277,  284,  286,
 /*  3200 */   285,  253,  256,  375,  234,  482,  482,  265,  395,  258,
 /*  3210 */   264,  284,  286,  285,  253,  284,  286,  285,  253,  257,
 /*  3220 */   259,  260,  261,  262,  263,  306,  307,  310,  482,  482,
 /*  3230 */    84,   85,  482,  671,  580,  482,  574,  482,  301,  264,
 /*  3240 */   482,   83,  482,  482,  482,  482,  482,  215,  300,  259,
 /*  3250 */   260,  261,  262,  263,  482,  284,  286,  285,  253,  232,
 /*  3260 */   228,  230,  193,  203,  208,  210,  212,  214,  206,  200,
 /*  3270 */   216,  218,  226,  220,  222,  224,  193,  203,  208,  210,
 /*  3280 */   212,  214,  206,  200,  216,  218,  226,  220,  222,  224,
 /*  3290 */   362,  498,  482,  482,  366,  482,  482,  482,  284,  286,
 /*  3300 */   285,  253,  232,  228,  230,  193,  203,  208,  210,  212,
 /*  3310 */   214,  206,  200,  216,  218,  226,  220,  222,  224,  232,
 /*  3320 */   228,  230,  193,  203,  208,  210,  212,  214,  206,  200,
 /*  3330 */   216,  218,  226,  220,  222,  224,  482,  482,  482,  482,
 /*  3340 */   482,  482,  482,  482,  427,  232,  228,  230,  193,  203,
 /*  3350 */   208,  210,  212,  214,  206,  200,  216,  218,  226,  220,
 /*  3360 */   222,  224,  482,  482,  362,  482,  482,  482,  370,  482,
 /*  3370 */   482,  482,  284,  286,  285,  253,  362,  832,  482,  354,
 /*  3380 */   374,  482,  482,  482,  284,  286,  285,  253,  232,  228,
 /*  3390 */   230,  193,  203,  208,  210,  212,  214,  206,  200,  216,
 /*  3400 */   218,  226,  220,  222,  224,  482,  482,  482,  482,  482,
 /*  3410 */   236,   32,  355,  356,  357,  358,  359,  482,  367,  482,
 /*  3420 */   295,  376,  377,  482,  482,  482,  482,  482,  124,  482,
 /*  3430 */    55,  232,  228,  230,  193,  203,  208,  210,  212,  214,
 /*  3440 */   206,  200,  216,  218,  226,  220,  222,  224,   35,   36,
 /*  3450 */    37,   38,   39,   40,   41,   42,   43,   44,   45,  482,
 /*  3460 */   362,  482,  482,  486,  382,  482,  482,  482,  284,  286,
 /*  3470 */   285,  253,  482,  482,  232,  228,  230,  193,  203,  208,
 /*  3480 */   210,  212,  214,  206,  200,  216,  218,  226,  220,  222,
 /*  3490 */   224,  482,  362,  482,  482,  482,  386,   32,  482,  482,
 /*  3500 */   284,  286,  285,  253,  482,  482,  345,  482,  482,  482,
 /*  3510 */   482,  482,  482,  482,  541,  482,   55,  232,  228,  230,
 /*  3520 */   193,  203,  208,  210,  212,  214,  206,  200,  216,  218,
 /*  3530 */   226,  220,  222,  224,   35,   36,   37,   38,   39,   40,
 /*  3540 */    41,   42,   43,   44,   45,  482,  362,  482,  482,  493,
 /*  3550 */   390,  482,  482,  482,  284,  286,  285,  253,  482,  482,
 /*  3560 */   232,  228,  230,  193,  203,  208,  210,  212,  214,  206,
 /*  3570 */   200,  216,  218,  226,  220,  222,  224,  232,  228,  230,
 /*  3580 */   193,  203,  208,  210,  212,  214,  206,  200,  216,  218,
 /*  3590 */   226,  220,  222,  224,  232,  228,  230,  193,  203,  208,
 /*  3600 */   210,  212,  214,  206,  200,  216,  218,  226,  220,  222,
 /*  3610 */   224,  228,  230,  193,  203,  208,  210,  212,  214,  206,
 /*  3620 */   200,  216,  218,  226,  220,  222,  224,   32,  482,  482,
 /*  3630 */   482,  482,  362,  482,  482,  331,  394,  482,   32,  482,
 /*  3640 */   284,  286,  285,  253,   26,  362,   55,  482,  482,  398,
 /*  3650 */   482,   32,  436,  284,  286,  285,  253,   55,  482,  482,
 /*  3660 */   482,  482,  482,  482,   35,   36,   37,   38,   39,   40,
 /*  3670 */    41,   42,   43,   44,   45,   35,   36,   37,   38,   39,
 /*  3680 */    40,   41,   42,   43,   44,   45,  482,  482,   35,   36,
 /*  3690 */    37,   38,   39,   40,   41,   42,   43,   44,   45,   78,
 /*  3700 */    79,   81,   80,   82,  482,  482,  482,  375,  482,  482,
 /*  3710 */   482,  482,  403,  482,  482,  284,  286,  285,  253,  362,
 /*  3720 */   482,  482,  482,  402,  482,  265,  482,  284,  286,  285,
 /*  3730 */   253,  482,  482,  284,  286,  285,  253,  482,   84,   85,
 /*  3740 */   482,  672,  580,  482,  574,  309,  482,  482,  482,   83,
 /*  3750 */   482,   78,   79,   81,   80,   82,   78,   79,   81,   80,
 /*  3760 */    82,  362,  482,  482,  482,  406,  435,  482,  482,  284,
 /*  3770 */   286,  285,  253,  202,  284,  286,  285,  253,  227,  482,
 /*  3780 */   482,  284,  286,  285,  253,  482,  284,  286,  285,  253,
 /*  3790 */    84,   85,  482,  676,  615,   84,   85,  482,  679,  536,
 /*  3800 */   482,   83,  482,  482,  482,  482,   83,  482,   78,   79,
 /*  3810 */    81,   80,   82,   78,   79,   81,   80,   82,  482,  482,
 /*  3820 */   482,  426,  482,  482,  482,  344,  482,  482,  482,  284,
 /*  3830 */   286,  285,  253,  284,  286,  285,  253,  482,  482,  482,
 /*  3840 */   482,  482,  482,  482,  482,  482,  482,   84,   85,  482,
 /*  3850 */   678,  536,   84,   85,  482,   18,   86,  482,   83,  482,
 /*  3860 */   482,  482,  482,   83,  482,   78,   79,   81,   80,   82,
 /*  3870 */    78,   79,   81,   80,   82,  439,  482,  482,  482,  482,
 /*  3880 */   255,  482,  482,  284,  286,  285,  253,  294,  284,  286,
 /*  3890 */   285,  253,  213,  482,  482,  284,  286,  285,  253,  482,
 /*  3900 */   284,  286,  285,  253,   84,   85,  482,  677,  536,   84,
 /*  3910 */    85,  482,  673,  536,  482,   83,  482,  482,  482,  482,
 /*  3920 */    83,  482,   78,   79,   81,   80,   82,   78,   79,   81,
 /*  3930 */    80,   82,  497,  482,  482,  482,  482,  482,  219,  482,
 /*  3940 */   284,  286,  285,  253,  482,  482,  284,  286,  285,  253,
 /*  3950 */   482,  482,  482,  482,  482,  482,  482,  482,  482,  482,
 /*  3960 */   482,   84,   85,  482,  674,  536,   84,   85,  482,   20,
 /*  3970 */    86,  482,   83,  482,  482,  482,  482,   83,  482,   78,
 /*  3980 */    79,   81,   80,   82,  482,  482,  482,  482,  482,  482,
 /*  3990 */   482,  482,  482,  482,  482,  482,  482,  482,  482,  482,
 /*  4000 */   482,  482,  482,  482,  482,  482,  482,  482,  482,  482,
 /*  4010 */   482,  482,  482,  482,  482,  482,  482,  482,   84,   85,
 /*  4020 */   482,  482,  482,  482,  569,  482,  482,  482,  482,   83,
};
static YYCODETYPE yy_lookahead[] = {
 /*     0 */     7,    8,    9,  103,  104,  105,  106,  107,   15,  100,
 /*    10 */   101,  102,  103,  104,  105,  106,  107,  130,  131,  132,
 /*    20 */    27,   28,   29,   30,   31,  111,   34,   34,  114,  115,
 /*    30 */   116,  117,  118,  118,   42,   42,   37,  123,  124,  124,
 /*    40 */    47,   42,   49,  129,  129,   52,   53,   54,   55,   56,
 /*    50 */    57,   58,   59,   60,   61,   62,   10,   11,   12,   13,
 /*    60 */    14,   15,   16,   17,   18,   19,   20,   21,   22,   23,
 /*    70 */    24,   25,   26,   24,   25,   26,   83,   84,   28,    6,
 /*    80 */   128,   35,  130,  131,   34,   35,   93,   94,   95,   96,
 /*    90 */    97,   98,   42,   43,   44,  126,  168,  169,   50,  130,
 /*   100 */   131,  132,   52,   53,   54,   55,   56,   57,   58,   59,
 /*   110 */    60,   35,   62,   63,   64,   65,   43,   67,   42,   28,
 /*   120 */    34,   71,   72,   73,   74,   34,   35,   77,  128,   34,
 /*   130 */   130,  131,   84,   42,   43,   44,   49,   50,   34,   89,
 /*   140 */    90,   91,   92,   52,   53,   54,   55,   56,   57,   58,
 /*   150 */    59,   60,   85,   62,   63,   64,   65,  114,   67,  116,
 /*   160 */   117,  118,   71,   72,   73,   74,  123,  124,   77,   83,
 /*   170 */    28,   84,  129,   86,    6,   35,   34,   35,   83,    6,
 /*   180 */    89,   90,   91,   92,   42,   43,   44,   83,   93,   94,
 /*   190 */    95,   96,   97,   98,   52,   53,   54,   55,   56,   57,
 /*   200 */    58,   59,   60,   35,   62,   63,   64,   65,   35,   67,
 /*   210 */   159,   28,  156,   71,   72,   73,   74,   34,   35,   77,
 /*   220 */   164,  165,  166,  167,   43,   42,   43,   44,  172,  173,
 /*   230 */    45,   89,   90,   91,   92,   52,   53,   54,   55,   56,
 /*   240 */    57,   58,   59,   60,   42,   62,   63,   64,   65,   68,
 /*   250 */    67,   70,   50,    6,   71,   72,   73,   74,   39,    6,
 /*   260 */    77,   42,   28,   78,   79,   80,   81,   82,   34,   35,
 /*   270 */    34,    6,   89,   90,   91,   92,   42,   43,   44,   21,
 /*   280 */    22,   23,   24,   25,   26,   32,   52,   53,   54,   55,
 /*   290 */    56,   57,   58,   59,   60,   35,   62,   63,   64,   65,
 /*   300 */    35,   67,   42,   28,  156,   71,   72,   73,   74,   34,
 /*   310 */    35,   77,  164,  165,  166,  167,  159,   42,   43,   44,
 /*   320 */   172,  173,   75,   89,   90,   91,   92,   52,   53,   54,
 /*   330 */    55,   56,   57,   58,   59,   60,    6,   62,   63,   64,
 /*   340 */    65,   50,   67,  116,  117,  118,   71,   72,   73,   74,
 /*   350 */     6,  124,   77,  110,   28,  112,  129,   49,   50,   34,
 /*   360 */    34,   35,   32,  156,   89,   90,   91,   92,   42,   43,
 /*   370 */    44,  164,  165,  166,  167,   84,   32,   69,   52,   53,
 /*   380 */    54,   55,   56,   57,   58,   59,   60,   34,   62,   63,
 /*   390 */    64,   65,   84,   67,   86,   28,  156,   71,   72,   73,
 /*   400 */    74,   34,   35,   77,  164,  165,  166,  167,   83,   42,
 /*   410 */    43,   44,  172,  173,  112,   89,   90,   91,   92,   52,
 /*   420 */    53,   54,   55,   56,   57,   58,   59,   60,    6,   62,
 /*   430 */    63,   64,   65,   34,   67,  116,  118,  118,   71,   72,
 /*   440 */    73,   74,  123,  124,   77,   43,   28,  129,  129,   49,
 /*   450 */    50,  170,   34,   35,   32,  156,   89,   90,   91,   92,
 /*   460 */    42,   43,   44,  164,  165,  166,  167,  129,  168,  169,
 /*   470 */    52,   53,   54,   55,   56,   57,   58,   59,   60,   37,
 /*   480 */    62,   63,   64,   65,   42,   67,   86,   28,  156,   71,
 /*   490 */    72,   73,   74,   34,   35,   77,  164,  165,  166,  167,
 /*   500 */     6,   42,   43,   44,  172,  173,   34,   89,   90,   91,
 /*   510 */    92,   52,   53,   54,   55,   56,   57,   58,   59,   60,
 /*   520 */    43,   62,   63,   64,   65,  128,   67,  130,  131,   35,
 /*   530 */    71,   72,   73,   74,   43,  118,   77,  126,   28,  168,
 /*   540 */   169,  130,  131,  132,   34,   35,  129,  156,   89,   90,
 /*   550 */    91,   92,   42,   43,   44,  164,  165,  166,  167,   68,
 /*   560 */   109,   70,   52,   53,   54,   55,   56,   57,   58,   59,
 /*   570 */    60,   38,   62,   63,   64,   65,    6,   67,   34,   28,
 /*   580 */   156,   71,   72,   73,   74,   34,   35,   77,  164,  165,
 /*   590 */   166,  167,    6,   42,   43,   44,  172,  173,   45,   89,
 /*   600 */    90,   91,   92,   52,   53,   54,   55,   56,   57,   58,
 /*   610 */    59,   60,   42,   62,   63,   64,   65,  128,   67,  130,
 /*   620 */   131,   35,   71,   72,   73,   74,  168,  169,   77,  126,
 /*   630 */    28,  168,  169,  130,  131,  132,   34,   35,   35,  156,
 /*   640 */    89,   90,   91,   92,   42,   43,   44,  164,  165,  166,
 /*   650 */   167,  121,  122,   50,   52,   53,   54,   55,   56,   57,
 /*   660 */    58,   59,   60,  159,   62,   63,   64,   65,  110,   67,
 /*   670 */   112,   28,  156,   71,   72,   73,   74,   34,   35,   77,
 /*   680 */   164,  165,  166,  167,    6,   42,   43,   44,  172,  173,
 /*   690 */   109,   89,   90,   91,   92,   52,   53,   54,   55,   56,
 /*   700 */    57,   58,   59,   60,   34,   62,   63,   64,   65,   35,
 /*   710 */    67,   42,  118,   35,   71,   72,   73,   74,  109,  125,
 /*   720 */    77,  126,   28,  129,   50,  130,  131,  132,   34,   35,
 /*   730 */    35,  156,   89,   90,   91,   92,   42,   43,   44,  164,
 /*   740 */   165,  166,  167,  158,  118,   50,   52,   53,   54,   55,
 /*   750 */    56,   57,   58,   59,   60,  129,   62,   63,   64,   65,
 /*   760 */   110,   67,  112,   28,  156,   71,   72,   73,   74,   34,
 /*   770 */    35,   77,  164,  165,  166,  167,    6,   42,   43,   44,
 /*   780 */   172,  173,   34,   89,   90,   91,   92,   52,   53,   54,
 /*   790 */    55,   56,   57,   58,   59,   60,    6,   62,   63,   64,
 /*   800 */    65,  128,   67,  130,  131,   35,   71,   72,   73,   74,
 /*   810 */   109,   37,   77,   39,   28,  128,   42,  130,  131,   34,
 /*   820 */    34,   35,   32,  156,   89,   90,   91,   92,   42,   43,
 /*   830 */    44,  164,  165,  166,  167,   38,  168,  169,   52,   53,
 /*   840 */    54,   55,   56,   57,   58,   59,   60,   34,   62,   63,
 /*   850 */    64,   65,   49,   67,  109,   28,  156,   71,   72,   73,
 /*   860 */    74,   34,   35,   77,  164,  165,  166,  167,    6,   42,
 /*   870 */    43,   44,   34,  173,   49,   89,   90,   91,   92,   52,
 /*   880 */    53,   54,   55,   56,   57,   58,   59,   60,    6,   62,
 /*   890 */    63,   64,   65,  128,   67,  130,  131,   35,   71,   72,
 /*   900 */    73,   74,    6,   37,   77,   39,   28,  128,   42,  130,
 /*   910 */   131,   34,   34,   35,   32,  156,   89,   90,   91,   92,
 /*   920 */    42,   43,   44,  164,  165,  166,  167,    6,   32,   34,
 /*   930 */    52,   53,   54,   55,   56,   57,   58,   59,   60,   44,
 /*   940 */    62,   63,   64,   65,  109,   67,   49,   28,  156,   71,
 /*   950 */    72,   73,   74,   34,   35,   77,  164,  165,  166,  167,
 /*   960 */   159,   42,   43,   44,  168,  169,   35,   89,   90,   91,
 /*   970 */    92,   52,   53,   54,   55,   56,   57,   58,   59,   60,
 /*   980 */    42,   62,   63,   64,   65,   34,   67,   34,   50,   34,
 /*   990 */    71,   72,   73,   74,   43,   44,   77,   42,   28,  168,
 /*  1000 */   169,   34,    6,   35,   34,   35,   85,  156,   89,   90,
 /*  1010 */    91,   92,   42,   43,   44,  164,  165,  166,  167,   34,
 /*  1020 */   157,  158,   52,   53,   54,   55,   56,   57,   58,   59,
 /*  1030 */    60,   35,   62,   63,   64,   65,   35,   67,   34,   28,
 /*  1040 */   156,   71,   72,   73,   74,   34,   35,   77,  164,  165,
 /*  1050 */   166,  167,    6,   42,   43,   44,   75,   35,   34,   89,
 /*  1060 */    90,   91,   92,   52,   53,   54,   55,   56,   57,   58,
 /*  1070 */    59,   60,   50,   62,   63,   64,   65,   34,   67,   35,
 /*  1080 */   119,   35,   71,   72,   73,   74,   42,   34,   77,   45,
 /*  1090 */    28,  168,  169,  168,  169,   42,   34,   35,   34,  156,
 /*  1100 */    89,   90,   91,   92,   42,   43,   44,  164,  165,  166,
 /*  1110 */   167,   47,  154,  155,   52,   53,   54,   55,   56,   57,
 /*  1120 */    58,   59,   60,    6,   62,   63,   64,   65,   34,   67,
 /*  1130 */    45,   28,  156,   71,   72,   73,   74,   34,   35,   77,
 /*  1140 */   164,  165,  166,  167,    6,   42,   43,   44,   42,   32,
 /*  1150 */    43,   89,   90,   91,   92,   52,   53,   54,   55,   56,
 /*  1160 */    57,   58,   59,   60,   34,   62,   63,   64,   65,   72,
 /*  1170 */    67,   35,   42,   35,   71,   72,   73,   74,   42,   42,
 /*  1180 */    77,   45,   28,   34,   45,   35,   34,   50,   34,   35,
 /*  1190 */    49,  156,   89,   90,   91,   92,   42,   43,   44,  164,
 /*  1200 */   165,  166,  167,   16,   69,   35,   52,   53,   54,   55,
 /*  1210 */    56,   57,   58,   59,   60,    6,   62,   63,   64,   65,
 /*  1220 */    42,   67,   66,   28,  156,   71,   72,   73,   74,   34,
 /*  1230 */    35,   77,  164,  165,  166,  167,    6,   42,   43,   44,
 /*  1240 */   109,   32,   66,   89,   90,   91,   92,   52,   53,   54,
 /*  1250 */    55,   56,   57,   58,   59,   60,   42,   62,   63,   64,
 /*  1260 */    65,   34,   67,   42,   50,   35,   71,   72,   73,   74,
 /*  1270 */    47,    6,   77,   42,   28,   86,   34,   34,   42,   45,
 /*  1280 */    34,   35,  119,  156,   89,   90,   91,   92,   42,   43,
 /*  1290 */    44,  164,  165,  166,  167,   35,  170,   32,   52,   53,
 /*  1300 */    54,   55,   56,   57,   58,   59,   60,    6,   62,   63,
 /*  1310 */    64,   65,  169,   67,   42,   28,  156,   71,   72,   73,
 /*  1320 */    74,   34,   35,   77,  164,  165,  166,  167,  155,   42,
 /*  1330 */    43,   44,    6,   32,  109,   89,   90,   91,   92,   52,
 /*  1340 */    53,   54,   55,   56,   57,   58,   59,   60,  119,   62,
 /*  1350 */    63,   64,   65,   34,   67,   34,   49,   35,   71,   72,
 /*  1360 */    73,   74,  108,   35,   77,   34,   28,  119,   69,   42,
 /*  1370 */    34,   69,   34,   35,   49,  156,   89,   90,   91,   92,
 /*  1380 */    42,   43,   44,  164,  165,  166,  167,   69,   16,    0,
 /*  1390 */    52,   53,   54,   55,   56,   57,   58,   59,   60,   34,
 /*  1400 */    62,   63,   64,   65,   34,   67,   35,   28,  156,   71,
 /*  1410 */    72,   73,   74,   34,   35,   77,  164,  165,  166,  167,
 /*  1420 */    42,   42,   43,   44,   32,   35,   42,   89,   90,   91,
 /*  1430 */    92,   52,   53,   54,   55,   56,   57,   58,   59,   60,
 /*  1440 */    35,   62,   63,   64,   65,  108,   67,   35,   35,   69,
 /*  1450 */    71,   72,   73,   74,  159,   47,   77,   42,   28,   35,
 /*  1460 */    43,   45,   85,   35,   34,   35,  119,  156,   89,   90,
 /*  1470 */    91,   92,   42,   43,   44,  164,  165,  166,  167,   42,
 /*  1480 */    47,   45,   52,   53,   54,   55,   56,   57,   58,   59,
 /*  1490 */    60,  109,   62,   63,   64,   65,   35,   67,  159,   28,
 /*  1500 */   156,   71,   72,   73,   74,   34,   35,   77,  164,  165,
 /*  1510 */   166,  167,  119,   42,   43,   44,  159,   85,  159,   89,
 /*  1520 */    90,   91,   92,   52,   53,   54,   55,   56,   57,   58,
 /*  1530 */    59,   60,   43,   62,   63,   64,   65,   49,   67,  120,
 /*  1540 */    34,  119,   71,   72,   73,   74,  120,   49,   77,   35,
 /*  1550 */    28,   42,   35,  159,   43,   35,   34,   35,  159,  156,
 /*  1560 */    89,   90,   91,   92,   42,   49,   44,  164,  165,  166,
 /*  1570 */   167,  122,  130,   43,   52,   53,   54,   55,   56,   57,
 /*  1580 */    58,   59,   60,   34,   62,   63,   64,   65,   49,   67,
 /*  1590 */   107,   72,  119,   71,   72,   73,   74,   43,  156,   77,
 /*  1600 */    35,    1,    2,    3,    4,    5,  164,  165,  166,  167,
 /*  1610 */   127,   89,   90,   91,   92,  120,  133,  134,  135,  136,
 /*  1620 */   137,  138,  139,  140,  141,  142,  143,  144,  145,  146,
 /*  1630 */   147,  148,  149,  150,  151,  152,  107,   35,  159,   35,
 /*  1640 */    40,   41,  119,   43,   44,  120,   46,  164,  165,  166,
 /*  1650 */   167,   51,  177,  177,  177,  177,  127,  177,  177,  177,
 /*  1660 */   177,  177,  133,  134,  135,  136,  137,  138,  139,  140,
 /*  1670 */   141,  142,  143,  144,  145,  146,  147,  148,  149,  150,
 /*  1680 */   151,  152,  107,  177,  177,  177,  177,  177,  177,  177,
 /*  1690 */   177,  177,  177,  164,  165,  166,  167,    1,    2,    3,
 /*  1700 */     4,    5,  127,  177,  177,  177,  177,  177,  133,  134,
 /*  1710 */   135,  136,  137,  138,  139,  140,  141,  142,  143,  144,
 /*  1720 */   145,  146,  147,  148,  149,  150,  151,  152,  107,  177,
 /*  1730 */    34,  177,  177,  113,  177,  177,   40,   41,  118,  164,
 /*  1740 */   165,  166,  167,   45,   48,  125,  177,   51,  127,  129,
 /*  1750 */   177,  177,  177,  177,  133,  134,  135,  136,  137,  138,
 /*  1760 */   139,  140,  141,  142,  143,  144,  145,  146,  147,  148,
 /*  1770 */   149,  150,  151,  152,  107,  177,   78,   79,   80,   81,
 /*  1780 */    82,  177,   84,  177,  177,  164,  165,  166,  167,    1,
 /*  1790 */     2,    3,    4,    5,  127,  177,  177,  177,  177,  177,
 /*  1800 */   133,  134,  135,  136,  137,  138,  139,  140,  141,  142,
 /*  1810 */   143,  144,  145,  146,  147,  148,  149,  150,  151,  152,
 /*  1820 */   107,  177,   34,  177,  177,  177,  177,  177,   40,   41,
 /*  1830 */   177,  164,  165,  166,  167,  177,   48,  177,  177,   51,
 /*  1840 */   127,  177,  177,  177,  177,  177,  133,  134,  135,  136,
 /*  1850 */   137,  138,  139,  140,  141,  142,  143,  144,  145,  146,
 /*  1860 */   147,  148,  149,  150,  151,  152,  107,  177,  177,  177,
 /*  1870 */   177,  177,  177,  177,  177,  177,  177,  164,  165,  166,
 /*  1880 */   167,    1,    2,    3,    4,    5,  127,  177,  177,  177,
 /*  1890 */   177,  177,  133,  134,  135,  136,  137,  138,  139,  140,
 /*  1900 */   141,  142,  143,  144,  145,  146,  147,  148,  149,  150,
 /*  1910 */   151,  152,  107,  177,  177,  177,  177,  177,  177,  177,
 /*  1920 */    40,   41,  177,  164,  165,  166,  167,  177,   48,  177,
 /*  1930 */   177,   51,  127,  177,  177,  177,  177,  177,  133,  134,
 /*  1940 */   135,  136,  137,  138,  139,  140,  141,  142,  143,  144,
 /*  1950 */   145,  146,  147,  148,  149,  150,  151,  152,  107,  177,
 /*  1960 */   177,  177,  177,  177,  177,  177,  177,  177,  177,  164,
 /*  1970 */   165,  166,  167,    1,    2,    3,    4,    5,  127,  177,
 /*  1980 */   177,  177,  177,  177,  133,  134,  135,  136,  137,  138,
 /*  1990 */   139,  140,  141,  142,  143,  144,  145,  146,  147,  148,
 /*  2000 */   149,  150,  151,  152,  107,  177,  177,  177,  177,  177,
 /*  2010 */   177,  177,   40,   41,  177,  164,  165,  166,  167,  177,
 /*  2020 */    48,  177,  177,   51,  127,  177,  177,  177,  177,  177,
 /*  2030 */   133,  134,  135,  136,  137,  138,  139,  140,  141,  142,
 /*  2040 */   143,  144,  145,  146,  147,  148,  149,  150,  151,  152,
 /*  2050 */   107,  177,  177,  177,  177,  177,  177,  177,  177,  177,
 /*  2060 */   177,  164,  165,  166,  167,    1,    2,    3,    4,    5,
 /*  2070 */   127,  177,  177,  177,  177,  177,  133,  134,  135,  136,
 /*  2080 */   137,  138,  139,  140,  141,  142,  143,  144,  145,  146,
 /*  2090 */   147,  148,  149,  150,  151,  152,  107,  177,  177,  177,
 /*  2100 */   177,  177,  177,  177,   40,   41,  177,  164,  165,  166,
 /*  2110 */   167,  177,   48,  177,  177,   51,  127,  177,  177,  177,
 /*  2120 */   177,  177,  133,  134,  135,  136,  137,  138,  139,  140,
 /*  2130 */   141,  142,  143,  144,  145,  146,  147,  148,  149,  150,
 /*  2140 */   151,  152,  107,  177,  177,  177,  177,  177,  177,  177,
 /*  2150 */   177,  177,  177,  164,  165,  166,  167,    1,    2,    3,
 /*  2160 */     4,    5,  127,  177,  177,  177,  177,  177,  133,  134,
 /*  2170 */   135,  136,  137,  138,  139,  140,  141,  142,  143,  144,
 /*  2180 */   145,  146,  147,  148,  149,  150,  151,  152,  107,  177,
 /*  2190 */   177,  177,  177,  177,  177,  177,   40,   41,  177,  164,
 /*  2200 */   165,  166,  167,  177,   48,  177,  177,   51,  127,  177,
 /*  2210 */   177,  177,  177,  177,  133,  134,  135,  136,  137,  138,
 /*  2220 */   139,  140,  141,  142,  143,  144,  145,  146,  147,  148,
 /*  2230 */   149,  150,  151,  152,  107,  177,  177,  177,  177,  177,
 /*  2240 */   177,  177,  177,  177,  177,  164,  165,  166,  167,    1,
 /*  2250 */     2,    3,    4,    5,  127,  177,  177,  177,  177,  177,
 /*  2260 */   133,  134,  135,  136,  137,  138,  139,  140,  141,  142,
 /*  2270 */   143,  144,  145,  146,  147,  148,  149,  150,  151,  152,
 /*  2280 */   107,  177,  177,  177,  153,  177,  177,  156,   40,   41,
 /*  2290 */   177,  164,  165,  166,  167,  164,  165,  166,  167,   51,
 /*  2300 */   127,  177,  177,  177,  177,  177,  133,  134,  135,  136,
 /*  2310 */   137,  138,  139,  140,  141,  142,  143,  144,  145,  146,
 /*  2320 */   147,  148,  149,  150,  151,  152,  107,  177,  177,  177,
 /*  2330 */   177,  177,  177,  115,  116,  117,  118,  164,  165,  166,
 /*  2340 */   167,  123,  124,  177,  177,  177,  127,  129,  177,  177,
 /*  2350 */   177,  177,  133,  134,  135,  136,  137,  138,  139,  140,
 /*  2360 */   141,  142,  143,  144,  145,  146,  147,  148,  149,  150,
 /*  2370 */   151,  152,  107,  177,  177,  177,  177,  177,  177,  177,
 /*  2380 */   156,  177,  177,  164,  165,  166,  167,  163,  164,  165,
 /*  2390 */   166,  167,  127,  177,  177,  177,  177,  177,  133,  134,
 /*  2400 */   135,  136,  137,  138,  139,  140,  141,  142,  143,  144,
 /*  2410 */   145,  146,  147,  148,  149,  150,  151,  152,  107,  177,
 /*  2420 */   177,  177,  153,  177,  177,  156,  177,  177,  177,  164,
 /*  2430 */   165,  166,  167,  164,  165,  166,  167,  177,  127,  177,
 /*  2440 */   177,  177,  177,  177,  133,  134,  135,  136,  137,  138,
 /*  2450 */   139,  140,  141,  142,  143,  144,  145,  146,  147,  148,
 /*  2460 */   149,  150,  151,  152,  107,  177,  177,  177,  153,  177,
 /*  2470 */   177,  156,  177,  177,  177,  164,  165,  166,  167,  164,
 /*  2480 */   165,  166,  167,  177,  127,  177,  177,  177,  177,  177,
 /*  2490 */   133,  134,  135,  136,  137,  138,  139,  140,  141,  142,
 /*  2500 */   143,  144,  145,  146,  147,  148,  149,  150,  151,  152,
 /*  2510 */   107,  177,  177,  177,  177,  177,  177,  177,  177,  156,
 /*  2520 */   177,  164,  165,  166,  167,  162,  163,  164,  165,  166,
 /*  2530 */   167,  177,  177,  177,  177,  177,  133,  134,  135,  136,
 /*  2540 */   137,  138,  139,  140,  141,  142,  143,  144,  145,  146,
 /*  2550 */   147,  148,  149,  150,  151,  152,    7,    8,    9,  177,
 /*  2560 */   153,  177,  177,  156,   15,  177,  177,  164,  165,  166,
 /*  2570 */   167,  164,  165,  166,  167,  177,   27,   28,   29,   30,
 /*  2580 */    31,   32,  177,   34,    7,    8,    9,    0,  177,  177,
 /*  2590 */   177,   42,   15,  177,  177,  177,   47,  177,   49,  177,
 /*  2600 */   177,  177,  177,  177,   27,   28,   29,   30,   31,  177,
 /*  2610 */   177,   34,  177,  177,    7,    8,    9,  177,  177,   42,
 /*  2620 */    33,  177,   15,   36,   47,   38,   49,   40,   41,  177,
 /*  2630 */   177,   44,   83,   84,   27,   28,   29,   30,   31,  177,
 /*  2640 */   177,   34,   93,   94,   95,   96,   97,   98,  177,   42,
 /*  2650 */   177,  177,  177,  177,   47,  177,   49,    7,    8,    9,
 /*  2660 */    83,   84,   85,  177,  177,   15,  177,  177,  177,  177,
 /*  2670 */    93,   94,   95,   96,   97,   98,  177,   27,   28,   29,
 /*  2680 */    30,   31,   32,   76,   34,  177,    7,    8,    9,  177,
 /*  2690 */    83,   84,   42,  177,   15,  177,  177,   47,  177,   49,
 /*  2700 */    93,   94,   95,   96,   97,   98,   27,   28,   29,   30,
 /*  2710 */    31,  177,  177,   34,  177,    7,    8,    9,  177,  177,
 /*  2720 */   177,   42,  177,   15,  177,  177,   47,  177,   49,  177,
 /*  2730 */   177,  177,  177,   83,   84,   27,   28,   29,   30,   31,
 /*  2740 */   177,  177,   34,   93,   94,   95,   96,   97,   98,  177,
 /*  2750 */    42,  177,  177,  177,  177,   47,  177,   49,    7,    8,
 /*  2760 */     9,  177,   83,   84,   85,  177,   15,  177,  177,  177,
 /*  2770 */   177,  177,   93,   94,   95,   96,   97,   98,   27,   28,
 /*  2780 */    29,   30,   31,   32,   76,   34,  177,    7,    8,    9,
 /*  2790 */   177,   83,   84,   42,  177,   15,  177,  177,   47,  177,
 /*  2800 */    49,   93,   94,   95,   96,   97,   98,   27,   28,   29,
 /*  2810 */    30,   31,   32,  177,   34,  177,    7,    8,    9,  177,
 /*  2820 */   177,  177,   42,  177,   15,  177,  177,   47,  177,   49,
 /*  2830 */   177,  177,  177,  177,   83,   84,   27,   28,   29,   30,
 /*  2840 */    31,  177,  177,   34,   93,   94,   95,   96,   97,   98,
 /*  2850 */   177,   42,  177,  177,  177,  177,   47,  156,   49,  177,
 /*  2860 */   177,  160,  177,   83,   84,  164,  165,  166,  167,  177,
 /*  2870 */   177,  177,  177,   93,   94,   95,   96,   97,   98,   33,
 /*  2880 */    45,  177,   36,  177,   38,   50,   40,   41,  177,  177,
 /*  2890 */    44,  177,   83,   84,   85,  177,  177,  177,  177,  177,
 /*  2900 */   177,  177,   93,   94,   95,   96,   97,   98,  177,  177,
 /*  2910 */     7,    8,    9,   78,   79,   80,   81,   82,   15,   84,
 /*  2920 */   177,   86,   87,   88,  177,  177,  177,  177,  177,  177,
 /*  2930 */    27,   28,   29,   30,   31,  177,  177,   34,    7,    8,
 /*  2940 */     9,  177,  177,  177,  177,   42,   15,  177,  177,  177,
 /*  2950 */    47,  177,   49,  177,  177,  177,  177,  177,   27,   28,
 /*  2960 */    29,   30,   31,  177,  177,   34,   35,    1,    2,    3,
 /*  2970 */     4,    5,  177,   42,  156,  177,  177,  177,   47,  177,
 /*  2980 */    49,  177,  164,  165,  166,  167,   83,   84,   85,  171,
 /*  2990 */   177,  177,  174,  175,  176,  177,   93,   94,   95,   96,
 /*  3000 */    97,   98,  177,  177,   15,  177,   40,   41,  177,   43,
 /*  3010 */    44,  177,   46,  177,   83,   84,  177,   51,  177,  177,
 /*  3020 */   177,   32,  177,   34,   93,   94,   95,   96,   97,   98,
 /*  3030 */   177,  177,    7,    8,    9,  177,  177,  177,  177,  177,
 /*  3040 */    15,   52,   53,   54,   55,   56,   57,   58,   59,   60,
 /*  3050 */    61,   62,   27,   28,   29,   30,   31,   32,  177,   34,
 /*  3060 */     7,    8,    9,  177,  177,  177,  177,   42,   15,  177,
 /*  3070 */   177,  177,   47,  177,   49,  177,  177,  177,  177,  177,
 /*  3080 */    27,   28,   29,   30,   31,   32,  177,   34,  177,  177,
 /*  3090 */     7,    8,    9,  177,  177,   42,  156,  177,   15,  177,
 /*  3100 */    47,  161,   49,  177,  164,  165,  166,  167,   83,   84,
 /*  3110 */    27,   28,   29,   30,   31,   32,  177,   34,   93,   94,
 /*  3120 */    95,   96,   97,   98,  177,   42,  177,  177,  177,  177,
 /*  3130 */    47,  177,   49,    7,    8,    9,   83,   84,  177,  177,
 /*  3140 */   177,   15,  177,  177,  177,  177,   93,   94,   95,   96,
 /*  3150 */    97,   98,  177,   27,   28,   29,   30,   31,  177,  177,
 /*  3160 */    34,  177,    7,    8,    9,  177,   83,   84,   42,  177,
 /*  3170 */    15,  177,  177,   47,  177,   49,   93,   94,   95,   96,
 /*  3180 */    97,   98,   27,   28,   29,   30,   31,  153,  177,   34,
 /*  3190 */   156,    1,    2,    3,    4,    5,  177,   42,  164,  165,
 /*  3200 */   166,  167,   47,  156,   49,  177,  177,  156,  161,   83,
 /*  3210 */    84,  164,  165,  166,  167,  164,  165,  166,  167,   93,
 /*  3220 */    94,   95,   96,   97,   98,  174,  175,  176,  177,  177,
 /*  3230 */    40,   41,  177,   43,   44,  177,   46,  177,   83,   84,
 /*  3240 */   177,   51,  177,  177,  177,  177,  177,  156,   93,   94,
 /*  3250 */    95,   96,   97,   98,  177,  164,  165,  166,  167,   10,
 /*  3260 */    11,   12,   13,   14,   15,   16,   17,   18,   19,   20,
 /*  3270 */    21,   22,   23,   24,   25,   26,   13,   14,   15,   16,
 /*  3280 */    17,   18,   19,   20,   21,   22,   23,   24,   25,   26,
 /*  3290 */   156,   42,  177,  177,  160,  177,  177,  177,  164,  165,
 /*  3300 */   166,  167,   10,   11,   12,   13,   14,   15,   16,   17,
 /*  3310 */    18,   19,   20,   21,   22,   23,   24,   25,   26,   10,
 /*  3320 */    11,   12,   13,   14,   15,   16,   17,   18,   19,   20,
 /*  3330 */    21,   22,   23,   24,   25,   26,  177,  177,  177,  177,
 /*  3340 */   177,  177,  177,  177,   35,   10,   11,   12,   13,   14,
 /*  3350 */    15,   16,   17,   18,   19,   20,   21,   22,   23,   24,
 /*  3360 */    25,   26,  177,  177,  156,  177,  177,  177,  160,  177,
 /*  3370 */   177,  177,  164,  165,  166,  167,  156,   85,  177,   45,
 /*  3380 */   160,  177,  177,  177,  164,  165,  166,  167,   10,   11,
 /*  3390 */    12,   13,   14,   15,   16,   17,   18,   19,   20,   21,
 /*  3400 */    22,   23,   24,   25,   26,  177,  177,  177,  177,  177,
 /*  3410 */    32,   15,   78,   79,   80,   81,   82,  177,   84,  177,
 /*  3420 */    85,   87,   88,  177,  177,  177,  177,  177,   32,  177,
 /*  3430 */    34,   10,   11,   12,   13,   14,   15,   16,   17,   18,
 /*  3440 */    19,   20,   21,   22,   23,   24,   25,   26,   52,   53,
 /*  3450 */    54,   55,   56,   57,   58,   59,   60,   61,   62,  177,
 /*  3460 */   156,  177,  177,   42,  160,  177,  177,  177,  164,  165,
 /*  3470 */   166,  167,  177,  177,   10,   11,   12,   13,   14,   15,
 /*  3480 */    16,   17,   18,   19,   20,   21,   22,   23,   24,   25,
 /*  3490 */    26,  177,  156,  177,  177,  177,  160,   15,  177,  177,
 /*  3500 */   164,  165,  166,  167,  177,  177,   42,  177,  177,  177,
 /*  3510 */   177,  177,  177,  177,   32,  177,   34,   10,   11,   12,
 /*  3520 */    13,   14,   15,   16,   17,   18,   19,   20,   21,   22,
 /*  3530 */    23,   24,   25,   26,   52,   53,   54,   55,   56,   57,
 /*  3540 */    58,   59,   60,   61,   62,  177,  156,  177,  177,   42,
 /*  3550 */   160,  177,  177,  177,  164,  165,  166,  167,  177,  177,
 /*  3560 */    10,   11,   12,   13,   14,   15,   16,   17,   18,   19,
 /*  3570 */    20,   21,   22,   23,   24,   25,   26,   10,   11,   12,
 /*  3580 */    13,   14,   15,   16,   17,   18,   19,   20,   21,   22,
 /*  3590 */    23,   24,   25,   26,   10,   11,   12,   13,   14,   15,
 /*  3600 */    16,   17,   18,   19,   20,   21,   22,   23,   24,   25,
 /*  3610 */    26,   11,   12,   13,   14,   15,   16,   17,   18,   19,
 /*  3620 */    20,   21,   22,   23,   24,   25,   26,   15,  177,  177,
 /*  3630 */   177,  177,  156,  177,  177,   85,  160,  177,   15,  177,
 /*  3640 */   164,  165,  166,  167,   32,  156,   34,  177,  177,  160,
 /*  3650 */   177,   15,   85,  164,  165,  166,  167,   34,  177,  177,
 /*  3660 */   177,  177,  177,  177,   52,   53,   54,   55,   56,   57,
 /*  3670 */    58,   59,   60,   61,   62,   52,   53,   54,   55,   56,
 /*  3680 */    57,   58,   59,   60,   61,   62,  177,  177,   52,   53,
 /*  3690 */    54,   55,   56,   57,   58,   59,   60,   61,   62,    1,
 /*  3700 */     2,    3,    4,    5,  177,  177,  177,  156,  177,  177,
 /*  3710 */   177,  177,  161,  177,  177,  164,  165,  166,  167,  156,
 /*  3720 */   177,  177,  177,  160,  177,  156,  177,  164,  165,  166,
 /*  3730 */   167,  177,  177,  164,  165,  166,  167,  177,   40,   41,
 /*  3740 */   177,   43,   44,  177,   46,  176,  177,  177,  177,   51,
 /*  3750 */   177,    1,    2,    3,    4,    5,    1,    2,    3,    4,
 /*  3760 */     5,  156,  177,  177,  177,  160,  156,  177,  177,  164,
 /*  3770 */   165,  166,  167,  156,  164,  165,  166,  167,  156,  177,
 /*  3780 */   177,  164,  165,  166,  167,  177,  164,  165,  166,  167,
 /*  3790 */    40,   41,  177,   43,   44,   40,   41,  177,   43,   44,
 /*  3800 */   177,   51,  177,  177,  177,  177,   51,  177,    1,    2,
 /*  3810 */     3,    4,    5,    1,    2,    3,    4,    5,  177,  177,
 /*  3820 */   177,  156,  177,  177,  177,  156,  177,  177,  177,  164,
 /*  3830 */   165,  166,  167,  164,  165,  166,  167,  177,  177,  177,
 /*  3840 */   177,  177,  177,  177,  177,  177,  177,   40,   41,  177,
 /*  3850 */    43,   44,   40,   41,  177,   43,   44,  177,   51,  177,
 /*  3860 */   177,  177,  177,   51,  177,    1,    2,    3,    4,    5,
 /*  3870 */     1,    2,    3,    4,    5,  156,  177,  177,  177,  177,
 /*  3880 */   156,  177,  177,  164,  165,  166,  167,  156,  164,  165,
 /*  3890 */   166,  167,  156,  177,  177,  164,  165,  166,  167,  177,
 /*  3900 */   164,  165,  166,  167,   40,   41,  177,   43,   44,   40,
 /*  3910 */    41,  177,   43,   44,  177,   51,  177,  177,  177,  177,
 /*  3920 */    51,  177,    1,    2,    3,    4,    5,    1,    2,    3,
 /*  3930 */     4,    5,  156,  177,  177,  177,  177,  177,  156,  177,
 /*  3940 */   164,  165,  166,  167,  177,  177,  164,  165,  166,  167,
 /*  3950 */   177,  177,  177,  177,  177,  177,  177,  177,  177,  177,
 /*  3960 */   177,   40,   41,  177,   43,   44,   40,   41,  177,   43,
 /*  3970 */    44,  177,   51,  177,  177,  177,  177,   51,  177,    1,
 /*  3980 */     2,    3,    4,    5,  177,  177,  177,  177,  177,  177,
 /*  3990 */   177,  177,  177,  177,  177,  177,  177,  177,  177,  177,
 /*  4000 */   177,  177,  177,  177,  177,  177,  177,  177,  177,  177,
 /*  4010 */   177,  177,  177,  177,  177,  177,  177,  177,   40,   41,
 /*  4020 */   177,  177,  177,  177,   46,  177,  177,  177,  177,   51,
};
#define YY_SHIFT_USE_DFLT (-9)
static short yy_shift_ofst[] = {
 /*     0 */  2846, 1389, 2587,   -9,   -9,   -9,   -9,   -9, 1242, 1424,
 /*    10 */    -9, 1243,  442,   -9, 1043, 1415,   -9, 3812,   -9, 3926,
 /*    20 */    -9,   -9, 1972,  236, 1516, 3612, 1022, 3636, 1413,   -9,
 /*    30 */    -9,   -9, 1227, 1187,   -9,   -9,   -9,   -9,   -9,   -9,
 /*    40 */    -9,   -9,   -9,   -9,   -9,   -9,   -9, 1265,  603, 3636,
 /*    50 */  1001,   -9,   -9, 3623,   -9,  553,   95,   -9,   -9,   -9,
 /*    60 */    -9,   -9,   -9,   -9,   -9, 1189, 1223,   -9, 1152, 1085,
 /*    70 */    95,   -9,  967, 1139,   95,   -9,   -9,   -9,   -9,   -9,
 /*    80 */    -9,   -9,   -9,   -9,   -9,   -9, 2248, 1880, 1370, 1325,
 /*    90 */  2989,  695, 3636, 1260,   -9,   -9,  422,  674, 3636, 1170,
 /*   100 */    -9,   -9,   -9,   -9,  985,  774,   -9, 1024,  219,   -9,
 /*   110 */   353,  570,   -9, 2966,   -9, 1417,   -9, 3190, 3921,   -9,
 /*   120 */  2156,  472,  803, 3396, 1137,  919,   -9,  142,   -9,   -9,
 /*   130 */    -9,   -9,   -9,   -9,   -9,   -9,   -9,   -9,   -9,   -9,
 /*   140 */    -9,   -9,   -9,   -9,   -9,   -9,   -9,   -9,   -9,   -9,
 /*   150 */    -9,  968,   -9,  931,   -9, 3126, 1106,  459, 1154, 1156,
 /*   160 */  1178,  275,  510,   -9, 3126, 1236,  491,   -9,  181,   -9,
 /*   170 */    -9,   95, 1299, 1522, 1522, 1378, 1379, 1471,   -9, 3126,
 /*   180 */  1437,   91,  183,   -9, 1509, 1338, 1287, 1519, 3126, 1602,
 /*   190 */    -9, 3126,   -9, 3126,  399,   -9,  291, 3126, 3126, 3584,
 /*   200 */  3126, 3126, 3584, 3126, 3126, 3584, 3126,  258, 3126,  258,
 /*   210 */  3126,  258, 3126,  258, 3126,  258, 3126,   49, 3126,   49,
 /*   220 */  3126,   -9, 3126,   -9, 3126,   -9, 3126,   49, 3126, 3263,
 /*   230 */  3126, 3263, 3126, 3600,   -7, 3378,   -9, 1392, 3126,   -9,
 /*   240 */  1365, 1372, 3126,  258,   87,  955,  897, 3025, 3584, 1321,
 /*   250 */  1326,  399,   -9,   -9, 3126,   -9,   -9,   -9,   -9,   -9,
 /*   260 */    -9,   -9,   -9,   -9, 2577, 3584,  308, 1064,  825, 2650,
 /*   270 */   877, 1488, 2549,   -9, 1117,   -9, 3126, 1094, 1107, 1141,
 /*   280 */  3083, 1301,   -9,   -9,   -9,   -9,   -9,   -9,   -9,  896,
 /*   290 */    -9,   -9,   -9, 3126, 3335,   -9, 3053,  330,   -9,   -9,
 /*   300 */  1302, 1318,   -9,  921,   -9, 3155,   -9, 1380, 3126,   -9,
 /*   310 */    -9,   -9,  253,   -9,   -9,   86, 1489, 1498, 2780,  344,
 /*   320 */    -9,   -9, 1530, 1539, 2751,  790,   -9,   -9,  258,  258,
 /*   330 */  3550,   -9, 1053,   -9,  325,  402,   -9,  477,   -9,  258,
 /*   340 */  3584,  748,  247, 2708, 3464, 1522,   50,   -9,  785,  770,
 /*   350 */    -9,  785,   -9, 2835,   -9,   -9,   -9,   -9,   -9,   -9,
 /*   360 */  3126,   -9, 3584, 1130, 3334, 3126,   -9, 2679,  185, 3126,
 /*   370 */    -9,   67,  185, 3126,   -9, 3292,   -9,   -9,  104, 1554,
 /*   380 */   185, 3126,   -9, 1511,  185, 3126,   -9, 1506, 1698, 3126,
 /*   390 */    -9, 2809,  185, 3126,   -9, 1432,  185, 3126,   -9, 2903,
 /*   400 */   185, 3126,   -9, 1377,  185, 3126,   -9,   -9,   -9,   -9,
 /*   410 */  3126, 1230,   -9, 3126,   -9, 3584,   -9, 1412,   -9, 1405,
 /*   420 */    -9, 1390,   -9, 1371,   -9, 2931, 3309,   -9,   -9, 1336,
 /*   430 */    48, 1331, 1328,   -9, 3126, 3567, 1322,   -9, 3126,   46,
 /*   440 */    -9,  953,  494,   -9,  953,   -9, 1234,   95,   -9,   -9,
 /*   450 */   953,  996,   -9,  953,  678,   -9,  953,  265,   -9,  953,
 /*   460 */   168,   -9,  953,  586,   -9,  953, 1138,   -9,  953,  862,
 /*   470 */    -9,  953, 1046,   -9,  953,  173,   -9,  400,  838,  825,
 /*   480 */    -8,  897,   -9,   -9, 3126, 3421, 1522,  367,   -9, 1004,
 /*   490 */   981, 2607, 3507, 1522,  234,   -9, 3126, 3249, 1522,  551,
 /*   500 */    -9, 1097, 3126, 1150,   -9,   -9,   -9, 1135, 1522, 1522,
 /*   510 */    -9,   -9, 1176, 1221,  326,  418,   -9,   -9, 3636, 1231,
 /*   520 */   602,   -9,  643,   -9, 1209, 1214,  694,   -9,  735,   -9,
 /*   530 */  3636, 1272,  786,   -9,  827,   -9, 2248, 2064, 1319, 1307,
 /*   540 */  3482,  938,  878,   -9,  970,   -9, 3636, 1327, 1011,   -9,
 /*   550 */  1062,   -9,  882,  202, 1103,   -9, 1195,   -9, 3636, 1384,
 /*   560 */  1246,   -9, 1430,   -9, 1600, 3807,   -9,   -9, 3978, 1408,
 /*   570 */  1416,   95, 1428,   -9, 1433, 1436,   95, 1461,   -9,   -9,
 /*   580 */  3978, 1696, 1136,   -9,   95,   76,   -9, 1514,   -9,  951,
 /*   590 */  1517,   73, 1520,  895,   -9,   -9, 1549,   -9,   -9, 1565,
 /*   600 */    -9, 1788, 1044,   -9,   95,  260,   -9, 1604,   -9,  140,
 /*   610 */    -9,   -9, 3698, 3750, 3755, 2248, 3864,   -9, 3869,  353,
 /*   620 */    -9,   -9,   -9,  353,  570,   -9,  533,  544,  866,   -9,
 /*   630 */   670,  669,   -9,  353,  570,   -9,  797,  813,   -1,   -9,
 /*   640 */  1149,  669,   -9,   -9,
};
#define YY_REDUCE_USE_DFLT (-114)
static short yy_reduce_ofst[] = {
 /*     0 */   -91, -114, -100, -114, -114, -114, -114, -114, -114, -114,
 /*    10 */  -114, -114, 1254, -114, -114, 1337, -114, 1620, -114,  594,
 /*    20 */  -114, -114,  338, -114, -114,  595, -114,  779, -114, -114,
 /*    30 */  -114, -114, -114, -114, -114, -114, -114, -114, -114, -114,
 /*    40 */  -114, -114, -114, -114, -114, -114, -114, -114, -114,  687,
 /*    50 */  -114, -114, -114, -113, -114, -114, 1473, -114, -114, -114,
 /*    60 */  -114, -114, -114, -114, -114, -114, -114, -114, -114, -114,
 /*    70 */   961, -114, -114, -114, 1229, -114, -114, -114, -114, -114,
 /*    80 */  -114, -114, -114, -114, -114, -114,  626,  338, -114, -114,
 /*    90 */   411, -114,  397, -114, -114, -114, -114, -114,  -48, -114,
 /*   100 */  -114, -114, -114, -114, -114,  835, -114, -114, 1131, -114,
 /*   110 */   650, 1225, -114,  -86, -114, -114, -114, 2218,  -85, -114,
 /*   120 */   338, -114, -114,  -31, -114, 2311, -114, 2403, -114, -114,
 /*   130 */  -114, -114, -114, -114, -114, -114, -114, -114, -114, -114,
 /*   140 */  -114, -114, -114, -114, -114, -114, -114, -114, -114, -114,
 /*   150 */  -114, -114, -114, -114, -114, 2131, -114, 2035, 2403, -114,
 /*   160 */  -114, 2127, 2403, -114, 2315, -114,  958, -114, 1173, -114,
 /*   170 */  -114, 1248, -114, 2265, 2403, -114, 1667, 2403, -114, 2407,
 /*   180 */  -114, 1851, 2403, -114, -114, 2357, 2403, -114, 3034, -114,
 /*   190 */  -114,  207, -114,  667,  281, -114, -114, 1160, 1403, -114,
 /*   200 */  1219, 3617, -114, 1252,  759, -114,  575, -114,  483, -114,
 /*   210 */   299, -114, 3736, -114, 3091, -114, 1035, -114, 3782, -114,
 /*   220 */   943, -114,  884, -114,  976, -114, 3622, -114, 1344, -114,
 /*   230 */  1311, -114, 1068, -114, 1442, -114, -114, -114,  792, -114,
 /*   240 */  -114, -114, 1127, -114, -114, -114, -114,  240, -114, -114,
 /*   250 */  -114, 1126, -114, -114, 3724, -114, -114, -114, -114, -114,
 /*   260 */  -114, -114, -114, -114, 2818, -114, -114, -114, -114,  516,
 /*   270 */  -114, -114,  424, -114, -114, -114,  700, -114, -114, -114,
 /*   280 */   608, -114, -114, -114, -114, -114, -114, -114, -114, -114,
 /*   290 */  -114, -114, -114, 3731, -114, -114,  148, -114, -114, -114,
 /*   300 */  -114, -114, -114, -114, -114, 3051, -114, -114, 3569, -114,
 /*   310 */  -114, -114, -114, -114, -114, -114, -114, -114,  332, -114,
 /*   320 */  -114, -114, -114, -114,   56, -114, -114, -114, -114, -114,
 /*   330 */  -114, -114, -114, -114, -114, -114, -114, -114, -114, -114,
 /*   340 */  -114, -114, -114, 3669, -114, 1483, 2403, -114,  863, -114,
 /*   350 */  -114,  585, -114,  504, -114, -114, -114, -114, -114, -114,
 /*   360 */  2701, -114, -114, -114,  157, 3134, -114, 2940,   51, 3208,
 /*   370 */  -114, -114, 1479, 3220, -114, -114, -114, -114, -114, -114,
 /*   380 */  1399, 3304, -114, -114, 1394, 3336, -114, -114,  801, 3390,
 /*   390 */  -114, 3047, 1359, 3476, -114, -114, 1357, 3489, -114, 3551,
 /*   400 */  1339, 3563, -114, -114, 1295, 3605, -114, -114, -114, -114,
 /*   410 */  2363, -114, -114, 2224, -114, -114, -114, -114, -114, -114,
 /*   420 */  -114, -114, -114, -114, -114, 3665, -114, -114, -114, -114,
 /*   430 */  -114, -114, -114, -114, 3610, -114, -114, -114, 3719, -114,
 /*   440 */  -114,  371, -114, -114, 1143, -114, -114, 1163, -114, -114,
 /*   450 */   831, -114, -114,  796, -114, -114,  300, -114, -114,  -72,
 /*   460 */  -114, -114,  463, -114, -114,  925, -114, -114,  923, -114,
 /*   470 */  -114,  668, -114, -114,  458, -114, -114, -114, -114, -114,
 /*   480 */  -114, -114, -114, -114,  391, -114, 2173, 2403, -114, -114,
 /*   490 */  -114,  851, -114, 1989, 2403, -114, 3776, -114, 1713, 2403,
 /*   500 */  -114, -114, 2269, -114, -114, -114, -114, -114, 1897, 2403,
 /*   510 */  -114, -114, -114, -114, 2219, 2403, -114, -114,  765, -114,
 /*   520 */  1529, -114, 2403, -114, -114, -114, 1621, -114, 2403, -114,
 /*   530 */   489, -114, 1759, -114, 2403, -114,  318,  338, -114, -114,
 /*   540 */   503, -114, 1805, -114, 2403, -114,    0, -114, 2081, -114,
 /*   550 */  2403, -114, -114, -114, 1575, -114, 2403, -114,  673, -114,
 /*   560 */  1943, -114, 2403, -114,  319,  -85, -114, -114,  318, -114,
 /*   570 */  -114, 1347, -114, -114, -114, -114, 1393, -114, -114, -114,
 /*   580 */   417,  338, 1419, -114, 1422, 1426, -114, -114, -114,  530,
 /*   590 */  -114, -114, -114, 1449, -114, -114, -114, -114, -114, -114,
 /*   600 */  -114,  338, 1495, -114, 1523, 1525, -114, -114, -114, -114,
 /*   610 */  -114, -114,   43,  227,  -85,  417,  -85, -114,  -85,  302,
 /*   620 */  -114, -114, -114,  243,  451, -114, -114, -114,  581, -114,
 /*   630 */  -114,  609, -114,  558,  701, -114, -114, -114,  745, -114,
 /*   640 */  -114, 1382, -114, -114,
};
static YYACTIONTYPE yy_default[] = {
 /*     0 */   947,  947,  947,  646,  648,  649,  650,  651,  947,  947,
 /*    10 */   652,  947,  947,  653,  947,  947,  654,  947,  669,  947,
 /*    20 */   670,  700,  947,  947,  947,  947,  947,  947,  947,  720,
 /*    30 */   738,  739,  947,  947,  748,  749,  750,  751,  752,  753,
 /*    40 */   754,  755,  756,  757,  758,  759,  724,  947,  947,  947,
 /*    50 */   947,  721,  725,  947,  740,  742,  947,  745,  933,  934,
 /*    60 */   935,  936,  937,  938,  939,  947,  947,  940,  947,  743,
 /*    70 */   947,  746,  947,  744,  947,  747,  741,  728,  730,  731,
 /*    80 */   732,  733,  734,  735,  736,  737,  947,  947,  947,  947,
 /*    90 */   947,  947,  947,  947,  722,  726,  947,  947,  947,  947,
 /*   100 */   723,  727,  729,  701,  947,  947,  655,  947,  947,  656,
 /*   110 */   947,  947,  658,  947,  664,  947,  665,  947,  947,  698,
 /*   120 */   947,  947,  947,  947,  947,  947,  704,  947,  706,  760,
 /*   130 */   762,  763,  764,  765,  766,  767,  768,  769,  770,  771,
 /*   140 */   772,  773,  774,  775,  776,  777,  778,  779,  780,  781,
 /*   150 */   782,  947,  783,  947,  784,  947,  947,  947,  947,  787,
 /*   160 */   947,  947,  947,  788,  947,  947,  947,  791,  947,  792,
 /*   170 */   793,  947,  947,  795,  796,  947,  947,  947,  799,  947,
 /*   180 */   947,  947,  947,  801,  947,  947,  947,  947,  947,  947,
 /*   190 */   803,  947,  861,  947,  947,  862,  947,  947,  947,  863,
 /*   200 */   947,  947,  864,  947,  947,  865,  947,  869,  947,  870,
 /*   210 */   947,  871,  947,  872,  947,  873,  947,  881,  947,  882,
 /*   220 */   947,  883,  947,  884,  947,  885,  947,  886,  947,  887,
 /*   230 */   947,  888,  947,  889,  947,  947,  874,  947,  947,  875,
 /*   240 */   947,  947,  947,  876,  894,  947,  877,  947,  924,  947,
 /*   250 */   947,  947,  890,  891,  947,  892,  893,  895,  896,  897,
 /*   260 */   898,  899,  900,  901,  947,  932,  894,  947,  878,  947,
 /*   270 */   947,  904,  947,  905,  947,  906,  947,  947,  947,  947,
 /*   280 */   947,  947,  909,  910,  919,  920,  921,  923,  922,  947,
 /*   290 */   911,  912,  879,  947,  947,  880,  947,  947,  907,  908,
 /*   300 */   895,  896,  902,  947,  903,  947,  925,  947,  947,  927,
 /*   310 */   928,  926,  947,  913,  914,  947,  947,  947,  947,  947,
 /*   320 */   915,  916,  947,  947,  947,  947,  917,  918,  868,  867,
 /*   330 */   947,  941,  947,  942,  947,  947,  943,  947,  944,  866,
 /*   340 */   945,  947,  947,  947,  947,  947,  947,  804,  947,  947,
 /*   350 */   808,  947,  809,  947,  811,  812,  813,  814,  815,  816,
 /*   360 */   947,  817,  860,  947,  947,  947,  818,  947,  947,  947,
 /*   370 */   821,  947,  947,  947,  822,  947,  828,  829,  947,  947,
 /*   380 */   947,  947,  819,  947,  947,  947,  820,  947,  947,  947,
 /*   390 */   823,  947,  947,  947,  824,  947,  947,  947,  825,  947,
 /*   400 */   947,  947,  826,  947,  947,  947,  827,  830,  831,  810,
 /*   410 */   947,  947,  833,  947,  834,  836,  835,  947,  837,  947,
 /*   420 */   838,  947,  839,  947,  840,  947,  947,  841,  842,  947,
 /*   430 */   947,  947,  947,  843,  947,  947,  947,  844,  947,  947,
 /*   440 */   845,  947,  947,  846,  947,  856,  858,  947,  859,  857,
 /*   450 */   947,  947,  847,  947,  947,  848,  947,  947,  849,  947,
 /*   460 */   947,  850,  947,  947,  851,  947,  947,  852,  947,  947,
 /*   470 */   853,  947,  947,  854,  947,  947,  855,  947,  947,  947,
 /*   480 */   947,  947,  946,  761,  947,  947,  947,  947,  805,  947,
 /*   490 */   947,  947,  947,  947,  947,  806,  947,  947,  947,  947,
 /*   500 */   807,  947,  947,  947,  802,  800,  798,  947,  947,  797,
 /*   510 */   794,  789,  785,  947,  947,  947,  790,  786,  947,  947,
 /*   520 */   947,  712,  947,  714,  947,  947,  947,  705,  947,  707,
 /*   530 */   947,  947,  947,  713,  947,  715,  947,  947,  947,  947,
 /*   540 */   947,  947,  947,  708,  947,  710,  947,  947,  947,  716,
 /*   550 */   947,  718,  947,  947,  947,  709,  947,  711,  947,  947,
 /*   560 */   947,  717,  947,  719,  947,  947,  696,  699,  947,  947,
 /*   570 */   947,  947,  947,  702,  947,  947,  947,  947,  703,  680,
 /*   580 */   947,  947,  947,  682,  947,  947,  684,  947,  688,  947,
 /*   590 */   947,  947,  947,  947,  692,  694,  947,  695,  693,  947,
 /*   600 */   686,  947,  947,  683,  947,  947,  685,  947,  689,  947,
 /*   610 */   687,  697,  947,  947,  947,  947,  947,  681,  947,  947,
 /*   620 */   666,  668,  667,  947,  947,  657,  947,  947,  947,  659,
 /*   630 */   947,  947,  660,  947,  947,  661,  947,  947,  947,  662,
 /*   640 */   947,  947,  663,  647,
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
  "CLONE",         "EMPTY",         "INSTANCEOF",    "AND",         
  "OR",            "EQUALS",        "IDENTICAL",     "LESS",        
  "GREATER",       "LESSEQUAL",     "GREATEREQUAL",  "NOTIDENTICAL",
  "NOTEQUALS",     "ADD",           "SUB",           "CONCAT",      
  "MUL",           "DIV",           "MOD",           "ISSET",       
  "FETCH",         "TYPEOF",        "NEW",           "NOT",         
  "PARENTHESES_CLOSE",  "NAMESPACE",     "IDENTIFIER",    "DOTCOMMA",    
  "INTERFACE",     "EXTENDS",       "CLASS",         "IMPLEMENTS",  
  "ABSTRACT",      "FINAL",         "BRACKET_OPEN",  "BRACKET_CLOSE",
  "COMMENT",       "ASSIGN",        "CONST",         "CONSTANT",    
  "FUNCTION",      "PARENTHESES_OPEN",  "ARROW",         "INLINE",      
  "TYPE_INTEGER",  "TYPE_UINTEGER",  "TYPE_LONG",     "TYPE_ULONG",  
  "TYPE_CHAR",     "TYPE_UCHAR",    "TYPE_DOUBLE",   "TYPE_BOOL",   
  "TYPE_STRING",   "TYPE_ARRAY",    "TYPE_VAR",      "BREAK",       
  "CONTINUE",      "IF",            "ELSE",          "SWITCH",      
  "CASE",          "COLON",         "DEFAULT",       "LOOP",        
  "WHILE",         "DO",            "FOR",           "IN",          
  "REVERSE",       "LET",           "ADDASSIGN",     "SUBASSIGN",   
  "MULASSIGN",     "DIVASSIGN",     "CONCATASSIGN",  "STRING",      
  "SBRACKET_OPEN",  "SBRACKET_CLOSE",  "DOUBLECOLON",   "INCR",        
  "DECR",          "ECHO",          "RETURN",        "UNSET",       
  "THROW",         "INTEGER",       "CHAR",          "DOUBLE",      
  "NULL",          "TRUE",          "FALSE",         "error",       
  "program",       "xx_language",   "xx_top_statement_list",  "xx_top_statement",
  "xx_namespace_def",  "xx_class_def",  "xx_interface_def",  "xx_comment",  
  "xx_interface_body",  "xx_class_body",  "xx_implements_list",  "xx_class_definition",
  "xx_implements",  "xx_interface_methods_definition",  "xx_class_properties_definition",  "xx_class_consts_definition",
  "xx_class_methods_definition",  "xx_class_property_definition",  "xx_visibility_list",  "xx_literal_expr",
  "xx_class_property_shortcuts",  "xx_class_property_shortcuts_list",  "xx_class_property_shortcut",  "xx_class_const_definition",
  "xx_class_method_definition",  "xx_interface_method_definition",  "xx_parameter_list",  "xx_statement_list",
  "xx_method_return_type",  "xx_visibility",  "xx_parameter_type",  "xx_parameter_cast",
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
 /*  94 */ "xx_method_return_type ::= xx_parameter_type",
 /*  95 */ "xx_method_return_type ::= xx_parameter_cast",
 /*  96 */ "xx_parameter_list ::= xx_parameter_list COMMA xx_parameter",
 /*  97 */ "xx_parameter_list ::= xx_parameter",
 /*  98 */ "xx_parameter ::= IDENTIFIER",
 /*  99 */ "xx_parameter ::= xx_parameter_type IDENTIFIER",
 /* 100 */ "xx_parameter ::= xx_parameter_cast IDENTIFIER",
 /* 101 */ "xx_parameter ::= IDENTIFIER ASSIGN xx_literal_expr",
 /* 102 */ "xx_parameter ::= xx_parameter_type IDENTIFIER ASSIGN xx_literal_expr",
 /* 103 */ "xx_parameter ::= xx_parameter_cast IDENTIFIER ASSIGN xx_literal_expr",
 /* 104 */ "xx_parameter_cast ::= LESS IDENTIFIER GREATER",
 /* 105 */ "xx_parameter_type ::= TYPE_INTEGER",
 /* 106 */ "xx_parameter_type ::= TYPE_UINTEGER",
 /* 107 */ "xx_parameter_type ::= TYPE_LONG",
 /* 108 */ "xx_parameter_type ::= TYPE_ULONG",
 /* 109 */ "xx_parameter_type ::= TYPE_CHAR",
 /* 110 */ "xx_parameter_type ::= TYPE_UCHAR",
 /* 111 */ "xx_parameter_type ::= TYPE_DOUBLE",
 /* 112 */ "xx_parameter_type ::= TYPE_BOOL",
 /* 113 */ "xx_parameter_type ::= TYPE_STRING",
 /* 114 */ "xx_parameter_type ::= TYPE_ARRAY",
 /* 115 */ "xx_parameter_type ::= TYPE_VAR",
 /* 116 */ "xx_statement_list ::= xx_statement_list xx_statement",
 /* 117 */ "xx_statement_list ::= xx_statement",
 /* 118 */ "xx_statement ::= xx_let_statement",
 /* 119 */ "xx_statement ::= xx_if_statement",
 /* 120 */ "xx_statement ::= xx_loop_statement",
 /* 121 */ "xx_statement ::= xx_echo_statement",
 /* 122 */ "xx_statement ::= xx_return_statement",
 /* 123 */ "xx_statement ::= xx_fetch_statement",
 /* 124 */ "xx_statement ::= xx_fcall_statement",
 /* 125 */ "xx_statement ::= xx_mcall_statement",
 /* 126 */ "xx_statement ::= xx_scall_statement",
 /* 127 */ "xx_statement ::= xx_unset_statement",
 /* 128 */ "xx_statement ::= xx_throw_statement",
 /* 129 */ "xx_statement ::= xx_declare_statement",
 /* 130 */ "xx_statement ::= xx_break_statement",
 /* 131 */ "xx_statement ::= xx_continue_statement",
 /* 132 */ "xx_statement ::= xx_while_statement",
 /* 133 */ "xx_statement ::= xx_do_while_statement",
 /* 134 */ "xx_statement ::= xx_switch_statement",
 /* 135 */ "xx_statement ::= xx_for_statement",
 /* 136 */ "xx_statement ::= xx_comment",
 /* 137 */ "xx_statement ::= xx_empty_statement",
 /* 138 */ "xx_empty_statement ::= DOTCOMMA",
 /* 139 */ "xx_break_statement ::= BREAK DOTCOMMA",
 /* 140 */ "xx_continue_statement ::= CONTINUE DOTCOMMA",
 /* 141 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN BRACKET_CLOSE",
 /* 142 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN BRACKET_CLOSE ELSE BRACKET_OPEN BRACKET_CLOSE",
 /* 143 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 144 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE ELSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 145 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE ELSE BRACKET_OPEN BRACKET_CLOSE",
 /* 146 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN BRACKET_CLOSE ELSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 147 */ "xx_switch_statement ::= SWITCH xx_eval_expr BRACKET_OPEN BRACKET_CLOSE",
 /* 148 */ "xx_switch_statement ::= SWITCH xx_eval_expr BRACKET_OPEN xx_case_clauses BRACKET_CLOSE",
 /* 149 */ "xx_case_clauses ::= xx_case_clauses xx_case_clause",
 /* 150 */ "xx_case_clauses ::= xx_case_clause",
 /* 151 */ "xx_case_clause ::= CASE xx_literal_expr COLON",
 /* 152 */ "xx_case_clause ::= CASE xx_literal_expr COLON xx_statement_list",
 /* 153 */ "xx_case_clause ::= DEFAULT COLON xx_statement_list",
 /* 154 */ "xx_loop_statement ::= LOOP BRACKET_OPEN BRACKET_CLOSE",
 /* 155 */ "xx_loop_statement ::= LOOP BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 156 */ "xx_while_statement ::= WHILE xx_eval_expr BRACKET_OPEN BRACKET_CLOSE",
 /* 157 */ "xx_while_statement ::= WHILE xx_eval_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 158 */ "xx_do_while_statement ::= DO BRACKET_OPEN BRACKET_CLOSE WHILE xx_eval_expr DOTCOMMA",
 /* 159 */ "xx_do_while_statement ::= DO BRACKET_OPEN xx_statement_list BRACKET_CLOSE WHILE xx_eval_expr DOTCOMMA",
 /* 160 */ "xx_for_statement ::= FOR IDENTIFIER IN xx_common_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 161 */ "xx_for_statement ::= FOR IDENTIFIER IN REVERSE xx_common_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 162 */ "xx_for_statement ::= FOR IDENTIFIER COMMA IDENTIFIER IN xx_common_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 163 */ "xx_for_statement ::= FOR IDENTIFIER COMMA IDENTIFIER IN REVERSE xx_common_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 164 */ "xx_let_statement ::= LET xx_let_assignments DOTCOMMA",
 /* 165 */ "xx_let_assignments ::= xx_let_assignments COMMA xx_let_assignment",
 /* 166 */ "xx_let_assignments ::= xx_let_assignment",
 /* 167 */ "xx_assignment_operator ::= ASSIGN",
 /* 168 */ "xx_assignment_operator ::= ADDASSIGN",
 /* 169 */ "xx_assignment_operator ::= SUBASSIGN",
 /* 170 */ "xx_assignment_operator ::= MULASSIGN",
 /* 171 */ "xx_assignment_operator ::= DIVASSIGN",
 /* 172 */ "xx_assignment_operator ::= CONCATASSIGN",
 /* 173 */ "xx_let_assignment ::= IDENTIFIER xx_assignment_operator xx_assign_expr",
 /* 174 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER xx_assignment_operator xx_assign_expr",
 /* 175 */ "xx_let_assignment ::= IDENTIFIER ARROW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 176 */ "xx_let_assignment ::= IDENTIFIER ARROW BRACKET_OPEN STRING BRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 177 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER SBRACKET_OPEN SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 178 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER SBRACKET_OPEN xx_index_expr SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 179 */ "xx_let_assignment ::= IDENTIFIER DOUBLECOLON IDENTIFIER xx_assignment_operator xx_assign_expr",
 /* 180 */ "xx_let_assignment ::= IDENTIFIER DOUBLECOLON IDENTIFIER SBRACKET_OPEN SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 181 */ "xx_let_assignment ::= IDENTIFIER DOUBLECOLON IDENTIFIER SBRACKET_OPEN xx_index_expr SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 182 */ "xx_let_assignment ::= IDENTIFIER SBRACKET_OPEN SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 183 */ "xx_let_assignment ::= IDENTIFIER SBRACKET_OPEN xx_index_expr SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 184 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER INCR",
 /* 185 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER DECR",
 /* 186 */ "xx_let_assignment ::= IDENTIFIER INCR",
 /* 187 */ "xx_let_assignment ::= IDENTIFIER DECR",
 /* 188 */ "xx_index_expr ::= xx_common_expr",
 /* 189 */ "xx_echo_statement ::= ECHO xx_echo_expressions DOTCOMMA",
 /* 190 */ "xx_echo_expressions ::= xx_echo_expressions COMMA xx_echo_expression",
 /* 191 */ "xx_echo_expressions ::= xx_echo_expression",
 /* 192 */ "xx_echo_expression ::= xx_common_expr",
 /* 193 */ "xx_mcall_statement ::= xx_mcall_expr DOTCOMMA",
 /* 194 */ "xx_fcall_statement ::= xx_fcall_expr DOTCOMMA",
 /* 195 */ "xx_scall_statement ::= xx_scall_expr DOTCOMMA",
 /* 196 */ "xx_fetch_statement ::= xx_fetch_expr DOTCOMMA",
 /* 197 */ "xx_return_statement ::= RETURN xx_common_expr DOTCOMMA",
 /* 198 */ "xx_return_statement ::= RETURN DOTCOMMA",
 /* 199 */ "xx_unset_statement ::= UNSET IDENTIFIER ARROW IDENTIFIER DOTCOMMA",
 /* 200 */ "xx_unset_statement ::= UNSET IDENTIFIER SBRACKET_OPEN xx_common_expr SBRACKET_CLOSE DOTCOMMA",
 /* 201 */ "xx_throw_statement ::= THROW xx_common_expr DOTCOMMA",
 /* 202 */ "xx_declare_statement ::= TYPE_INTEGER xx_declare_variable_list DOTCOMMA",
 /* 203 */ "xx_declare_statement ::= TYPE_UINTEGER xx_declare_variable_list DOTCOMMA",
 /* 204 */ "xx_declare_statement ::= TYPE_CHAR xx_declare_variable_list DOTCOMMA",
 /* 205 */ "xx_declare_statement ::= TYPE_UCHAR xx_declare_variable_list DOTCOMMA",
 /* 206 */ "xx_declare_statement ::= TYPE_LONG xx_declare_variable_list DOTCOMMA",
 /* 207 */ "xx_declare_statement ::= TYPE_ULONG xx_declare_variable_list DOTCOMMA",
 /* 208 */ "xx_declare_statement ::= TYPE_DOUBLE xx_declare_variable_list DOTCOMMA",
 /* 209 */ "xx_declare_statement ::= TYPE_STRING xx_declare_variable_list DOTCOMMA",
 /* 210 */ "xx_declare_statement ::= TYPE_BOOL xx_declare_variable_list DOTCOMMA",
 /* 211 */ "xx_declare_statement ::= TYPE_VAR xx_declare_variable_list DOTCOMMA",
 /* 212 */ "xx_declare_variable_list ::= xx_declare_variable_list COMMA xx_declare_variable",
 /* 213 */ "xx_declare_variable_list ::= xx_declare_variable",
 /* 214 */ "xx_declare_variable ::= IDENTIFIER",
 /* 215 */ "xx_declare_variable ::= IDENTIFIER ASSIGN xx_literal_expr",
 /* 216 */ "xx_assign_expr ::= xx_common_expr",
 /* 217 */ "xx_common_expr ::= NOT xx_common_expr",
 /* 218 */ "xx_common_expr ::= ISSET xx_isset_expr",
 /* 219 */ "xx_common_expr ::= REQUIRE xx_common_expr",
 /* 220 */ "xx_common_expr ::= CLONE xx_common_expr",
 /* 221 */ "xx_common_expr ::= EMPTY xx_common_expr",
 /* 222 */ "xx_common_expr ::= xx_common_expr EQUALS xx_common_expr",
 /* 223 */ "xx_common_expr ::= xx_common_expr NOTEQUALS xx_common_expr",
 /* 224 */ "xx_common_expr ::= xx_common_expr IDENTICAL xx_common_expr",
 /* 225 */ "xx_common_expr ::= xx_common_expr NOTIDENTICAL xx_common_expr",
 /* 226 */ "xx_common_expr ::= xx_common_expr LESS xx_common_expr",
 /* 227 */ "xx_common_expr ::= xx_common_expr GREATER xx_common_expr",
 /* 228 */ "xx_common_expr ::= xx_common_expr LESSEQUAL xx_common_expr",
 /* 229 */ "xx_common_expr ::= xx_common_expr GREATEREQUAL xx_common_expr",
 /* 230 */ "xx_common_expr ::= PARENTHESES_OPEN xx_common_expr PARENTHESES_CLOSE",
 /* 231 */ "xx_common_expr ::= PARENTHESES_OPEN xx_parameter_type PARENTHESES_CLOSE xx_common_expr",
 /* 232 */ "xx_common_expr ::= LESS IDENTIFIER GREATER xx_common_expr",
 /* 233 */ "xx_common_expr ::= IDENTIFIER ARROW IDENTIFIER",
 /* 234 */ "xx_common_expr ::= IDENTIFIER DOUBLECOLON IDENTIFIER",
 /* 235 */ "xx_common_expr ::= IDENTIFIER DOUBLECOLON CONSTANT",
 /* 236 */ "xx_common_expr ::= IDENTIFIER SBRACKET_OPEN xx_common_expr SBRACKET_CLOSE",
 /* 237 */ "xx_common_expr ::= xx_common_expr ADD xx_common_expr",
 /* 238 */ "xx_common_expr ::= xx_common_expr SUB xx_common_expr",
 /* 239 */ "xx_common_expr ::= xx_common_expr MUL xx_common_expr",
 /* 240 */ "xx_common_expr ::= xx_common_expr DIV xx_common_expr",
 /* 241 */ "xx_common_expr ::= xx_common_expr MOD xx_common_expr",
 /* 242 */ "xx_common_expr ::= xx_common_expr CONCAT xx_common_expr",
 /* 243 */ "xx_common_expr ::= xx_common_expr AND xx_common_expr",
 /* 244 */ "xx_common_expr ::= xx_common_expr OR xx_common_expr",
 /* 245 */ "xx_common_expr ::= xx_common_expr INSTANCEOF xx_common_expr",
 /* 246 */ "xx_fetch_expr ::= FETCH IDENTIFIER COMMA xx_isset_expr",
 /* 247 */ "xx_common_expr ::= xx_fetch_expr",
 /* 248 */ "xx_common_expr ::= TYPEOF xx_common_expr",
 /* 249 */ "xx_common_expr ::= CONSTANT",
 /* 250 */ "xx_common_expr ::= IDENTIFIER",
 /* 251 */ "xx_common_expr ::= INTEGER",
 /* 252 */ "xx_common_expr ::= STRING",
 /* 253 */ "xx_common_expr ::= CHAR",
 /* 254 */ "xx_common_expr ::= DOUBLE",
 /* 255 */ "xx_common_expr ::= NULL",
 /* 256 */ "xx_common_expr ::= TRUE",
 /* 257 */ "xx_common_expr ::= FALSE",
 /* 258 */ "xx_common_expr ::= SBRACKET_OPEN SBRACKET_CLOSE",
 /* 259 */ "xx_common_expr ::= SBRACKET_OPEN xx_array_list SBRACKET_CLOSE",
 /* 260 */ "xx_common_expr ::= NEW IDENTIFIER",
 /* 261 */ "xx_common_expr ::= NEW IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 262 */ "xx_common_expr ::= NEW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 263 */ "xx_fcall_expr ::= IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 264 */ "xx_fcall_expr ::= IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 265 */ "xx_fcall_expr ::= BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 266 */ "xx_fcall_expr ::= BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 267 */ "xx_scall_expr ::= IDENTIFIER DOUBLECOLON IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 268 */ "xx_scall_expr ::= IDENTIFIER DOUBLECOLON IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 269 */ "xx_mcall_expr ::= IDENTIFIER ARROW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 270 */ "xx_mcall_expr ::= IDENTIFIER ARROW IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 271 */ "xx_mcall_expr ::= IDENTIFIER ARROW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 272 */ "xx_mcall_expr ::= IDENTIFIER ARROW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 273 */ "xx_mcall_expr ::= IDENTIFIER ARROW BRACKET_OPEN STRING BRACKET_CLOSE PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 274 */ "xx_mcall_expr ::= IDENTIFIER ARROW BRACKET_OPEN STRING BRACKET_CLOSE PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 275 */ "xx_common_expr ::= xx_mcall_expr",
 /* 276 */ "xx_common_expr ::= xx_scall_expr",
 /* 277 */ "xx_common_expr ::= xx_fcall_expr",
 /* 278 */ "xx_call_parameters ::= xx_call_parameters COMMA xx_call_parameter",
 /* 279 */ "xx_call_parameters ::= xx_call_parameter",
 /* 280 */ "xx_call_parameter ::= xx_common_expr",
 /* 281 */ "xx_array_list ::= xx_array_list COMMA xx_array_item",
 /* 282 */ "xx_array_list ::= xx_array_item",
 /* 283 */ "xx_array_item ::= xx_array_key COLON xx_array_value",
 /* 284 */ "xx_array_item ::= xx_array_value",
 /* 285 */ "xx_array_key ::= IDENTIFIER",
 /* 286 */ "xx_array_key ::= STRING",
 /* 287 */ "xx_array_key ::= INTEGER",
 /* 288 */ "xx_array_value ::= xx_common_expr",
 /* 289 */ "xx_literal_expr ::= INTEGER",
 /* 290 */ "xx_literal_expr ::= CHAR",
 /* 291 */ "xx_literal_expr ::= STRING",
 /* 292 */ "xx_literal_expr ::= DOUBLE",
 /* 293 */ "xx_literal_expr ::= NULL",
 /* 294 */ "xx_literal_expr ::= FALSE",
 /* 295 */ "xx_literal_expr ::= TRUE",
 /* 296 */ "xx_literal_expr ::= IDENTIFIER DOUBLECOLON CONSTANT",
 /* 297 */ "xx_isset_expr ::= IDENTIFIER SBRACKET_OPEN xx_common_expr SBRACKET_CLOSE",
 /* 298 */ "xx_isset_expr ::= IDENTIFIER ARROW IDENTIFIER",
 /* 299 */ "xx_isset_expr ::= IDENTIFIER ARROW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE",
 /* 300 */ "xx_isset_expr ::= IDENTIFIER ARROW BRACKET_OPEN STRING BRACKET_CLOSE",
 /* 301 */ "xx_eval_expr ::= xx_common_expr",
 /* 302 */ "xx_comment ::= COMMENT",
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
// 954 "parser.lemon"
{
	/*if ((yypminor->yy0)) {
		if ((yypminor->yy0)->free_flag) {
			efree((yypminor->yy0)->token);
		}
		efree((yypminor->yy0));
	}*/
}
// 2613 "parser.c"
      break;
    case 101:
// 967 "parser.lemon"
{ json_object_put((yypminor->yy277)); }
// 2618 "parser.c"
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
  { 100, 1 },
  { 101, 1 },
  { 102, 2 },
  { 102, 1 },
  { 103, 1 },
  { 103, 1 },
  { 103, 1 },
  { 103, 1 },
  { 104, 3 },
  { 106, 3 },
  { 106, 5 },
  { 105, 3 },
  { 105, 5 },
  { 105, 5 },
  { 105, 7 },
  { 105, 4 },
  { 105, 6 },
  { 105, 6 },
  { 105, 4 },
  { 105, 6 },
  { 109, 2 },
  { 109, 3 },
  { 110, 3 },
  { 110, 1 },
  { 112, 1 },
  { 108, 2 },
  { 108, 3 },
  { 111, 1 },
  { 111, 1 },
  { 111, 1 },
  { 111, 2 },
  { 111, 2 },
  { 111, 2 },
  { 111, 2 },
  { 111, 3 },
  { 111, 3 },
  { 114, 2 },
  { 114, 1 },
  { 117, 4 },
  { 117, 3 },
  { 117, 6 },
  { 117, 5 },
  { 117, 5 },
  { 117, 4 },
  { 117, 7 },
  { 117, 6 },
  { 120, 2 },
  { 120, 3 },
  { 121, 3 },
  { 121, 1 },
  { 122, 1 },
  { 122, 2 },
  { 115, 2 },
  { 115, 1 },
  { 116, 2 },
  { 116, 1 },
  { 113, 2 },
  { 113, 1 },
  { 123, 6 },
  { 123, 5 },
  { 124, 7 },
  { 124, 8 },
  { 124, 8 },
  { 124, 9 },
  { 124, 8 },
  { 124, 9 },
  { 124, 9 },
  { 124, 10 },
  { 124, 9 },
  { 124, 10 },
  { 124, 10 },
  { 124, 11 },
  { 124, 10 },
  { 124, 11 },
  { 124, 11 },
  { 124, 12 },
  { 125, 8 },
  { 125, 9 },
  { 125, 9 },
  { 125, 10 },
  { 125, 6 },
  { 125, 7 },
  { 125, 7 },
  { 125, 8 },
  { 118, 2 },
  { 118, 1 },
  { 129, 1 },
  { 129, 1 },
  { 129, 1 },
  { 129, 1 },
  { 129, 1 },
  { 129, 1 },
  { 129, 1 },
  { 129, 1 },
  { 128, 1 },
  { 128, 1 },
  { 126, 3 },
  { 126, 1 },
  { 132, 1 },
  { 132, 2 },
  { 132, 2 },
  { 132, 3 },
  { 132, 4 },
  { 132, 4 },
  { 131, 3 },
  { 130, 1 },
  { 130, 1 },
  { 130, 1 },
  { 130, 1 },
  { 130, 1 },
  { 130, 1 },
  { 130, 1 },
  { 130, 1 },
  { 130, 1 },
  { 130, 1 },
  { 130, 1 },
  { 127, 2 },
  { 127, 1 },
  { 133, 1 },
  { 133, 1 },
  { 133, 1 },
  { 133, 1 },
  { 133, 1 },
  { 133, 1 },
  { 133, 1 },
  { 133, 1 },
  { 133, 1 },
  { 133, 1 },
  { 133, 1 },
  { 133, 1 },
  { 133, 1 },
  { 133, 1 },
  { 133, 1 },
  { 133, 1 },
  { 133, 1 },
  { 133, 1 },
  { 133, 1 },
  { 133, 1 },
  { 152, 1 },
  { 146, 2 },
  { 147, 2 },
  { 135, 4 },
  { 135, 7 },
  { 135, 5 },
  { 135, 9 },
  { 135, 8 },
  { 135, 8 },
  { 150, 4 },
  { 150, 5 },
  { 154, 2 },
  { 154, 1 },
  { 155, 3 },
  { 155, 4 },
  { 155, 3 },
  { 136, 3 },
  { 136, 4 },
  { 148, 4 },
  { 148, 5 },
  { 149, 6 },
  { 149, 7 },
  { 151, 7 },
  { 151, 8 },
  { 151, 9 },
  { 151, 10 },
  { 134, 3 },
  { 157, 3 },
  { 157, 1 },
  { 159, 1 },
  { 159, 1 },
  { 159, 1 },
  { 159, 1 },
  { 159, 1 },
  { 159, 1 },
  { 158, 3 },
  { 158, 5 },
  { 158, 7 },
  { 158, 7 },
  { 158, 7 },
  { 158, 8 },
  { 158, 5 },
  { 158, 7 },
  { 158, 8 },
  { 158, 5 },
  { 158, 6 },
  { 158, 4 },
  { 158, 4 },
  { 158, 2 },
  { 158, 2 },
  { 161, 1 },
  { 137, 3 },
  { 162, 3 },
  { 162, 1 },
  { 163, 1 },
  { 141, 2 },
  { 140, 2 },
  { 142, 2 },
  { 139, 2 },
  { 138, 3 },
  { 138, 2 },
  { 143, 5 },
  { 143, 6 },
  { 144, 3 },
  { 145, 3 },
  { 145, 3 },
  { 145, 3 },
  { 145, 3 },
  { 145, 3 },
  { 145, 3 },
  { 145, 3 },
  { 145, 3 },
  { 145, 3 },
  { 145, 3 },
  { 168, 3 },
  { 168, 1 },
  { 169, 1 },
  { 169, 3 },
  { 160, 1 },
  { 156, 2 },
  { 156, 2 },
  { 156, 2 },
  { 156, 2 },
  { 156, 2 },
  { 156, 3 },
  { 156, 3 },
  { 156, 3 },
  { 156, 3 },
  { 156, 3 },
  { 156, 3 },
  { 156, 3 },
  { 156, 3 },
  { 156, 3 },
  { 156, 4 },
  { 156, 4 },
  { 156, 3 },
  { 156, 3 },
  { 156, 3 },
  { 156, 4 },
  { 156, 3 },
  { 156, 3 },
  { 156, 3 },
  { 156, 3 },
  { 156, 3 },
  { 156, 3 },
  { 156, 3 },
  { 156, 3 },
  { 156, 3 },
  { 167, 4 },
  { 156, 1 },
  { 156, 2 },
  { 156, 1 },
  { 156, 1 },
  { 156, 1 },
  { 156, 1 },
  { 156, 1 },
  { 156, 1 },
  { 156, 1 },
  { 156, 1 },
  { 156, 1 },
  { 156, 2 },
  { 156, 3 },
  { 156, 2 },
  { 156, 4 },
  { 156, 5 },
  { 165, 4 },
  { 165, 3 },
  { 165, 6 },
  { 165, 5 },
  { 166, 6 },
  { 166, 5 },
  { 164, 6 },
  { 164, 5 },
  { 164, 8 },
  { 164, 7 },
  { 164, 8 },
  { 164, 7 },
  { 156, 1 },
  { 156, 1 },
  { 156, 1 },
  { 172, 3 },
  { 172, 1 },
  { 173, 1 },
  { 171, 3 },
  { 171, 1 },
  { 174, 3 },
  { 174, 1 },
  { 175, 1 },
  { 175, 1 },
  { 175, 1 },
  { 176, 1 },
  { 119, 1 },
  { 119, 1 },
  { 119, 1 },
  { 119, 1 },
  { 119, 1 },
  { 119, 1 },
  { 119, 1 },
  { 119, 3 },
  { 170, 4 },
  { 170, 3 },
  { 170, 5 },
  { 170, 5 },
  { 153, 1 },
  { 107, 1 },
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
// 963 "parser.lemon"
{
	status->ret = yymsp[0].minor.yy277;
}
// 3138 "parser.c"
        break;
      case 1:
      case 4:
      case 5:
      case 6:
      case 7:
      case 94:
      case 95:
      case 118:
      case 119:
      case 120:
      case 121:
      case 122:
      case 123:
      case 124:
      case 125:
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
      case 188:
      case 216:
      case 247:
      case 275:
      case 276:
      case 277:
      case 280:
      case 288:
      case 301:
// 969 "parser.lemon"
{
	yygotominor.yy277 = yymsp[0].minor.yy277;
}
// 3180 "parser.c"
        break;
      case 2:
      case 36:
      case 52:
      case 54:
      case 56:
      case 84:
      case 116:
      case 149:
// 973 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_list(yymsp[-1].minor.yy277, yymsp[0].minor.yy277);
}
// 3194 "parser.c"
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
      case 117:
      case 150:
      case 166:
      case 191:
      case 213:
      case 279:
      case 282:
// 977 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_list(NULL, yymsp[0].minor.yy277);
}
// 3216 "parser.c"
        break;
      case 8:
// 997 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_namespace(yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(33,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3225 "parser.c"
        break;
      case 9:
// 1001 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_interface(yymsp[-1].minor.yy0, yymsp[0].minor.yy277, NULL, status->scanner_state);
  yy_destructor(36,&yymsp[-2].minor);
}
// 3233 "parser.c"
        break;
      case 10:
// 1005 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_interface(yymsp[-3].minor.yy0, yymsp[0].minor.yy277, yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(36,&yymsp[-4].minor);
  yy_destructor(37,&yymsp[-2].minor);
}
// 3242 "parser.c"
        break;
      case 11:
// 1009 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class(yymsp[-1].minor.yy0, yymsp[0].minor.yy277, 0, 0, NULL, NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-2].minor);
}
// 3250 "parser.c"
        break;
      case 12:
// 1013 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy277, 0, 0, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-4].minor);
  yy_destructor(37,&yymsp[-2].minor);
}
// 3259 "parser.c"
        break;
      case 13:
// 1017 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy277, 0, 0, NULL, yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(38,&yymsp[-4].minor);
  yy_destructor(39,&yymsp[-2].minor);
}
// 3268 "parser.c"
        break;
      case 14:
// 1021 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class(yymsp[-5].minor.yy0, yymsp[0].minor.yy277, 0, 0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(38,&yymsp[-6].minor);
  yy_destructor(37,&yymsp[-4].minor);
  yy_destructor(39,&yymsp[-2].minor);
}
// 3278 "parser.c"
        break;
      case 15:
// 1025 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class(yymsp[-1].minor.yy0, yymsp[0].minor.yy277, 1, 0, NULL, NULL, status->scanner_state);
  yy_destructor(40,&yymsp[-3].minor);
  yy_destructor(38,&yymsp[-2].minor);
}
// 3287 "parser.c"
        break;
      case 16:
// 1029 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy277, 1, 0, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(40,&yymsp[-5].minor);
  yy_destructor(38,&yymsp[-4].minor);
  yy_destructor(37,&yymsp[-2].minor);
}
// 3297 "parser.c"
        break;
      case 17:
// 1033 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy277, 1, 0, NULL, yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(40,&yymsp[-5].minor);
  yy_destructor(38,&yymsp[-4].minor);
  yy_destructor(39,&yymsp[-2].minor);
}
// 3307 "parser.c"
        break;
      case 18:
// 1037 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class(yymsp[-1].minor.yy0, yymsp[0].minor.yy277, 0, 1, NULL, NULL, status->scanner_state);
  yy_destructor(41,&yymsp[-3].minor);
  yy_destructor(38,&yymsp[-2].minor);
}
// 3316 "parser.c"
        break;
      case 19:
// 1041 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy277, 0, 1, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(41,&yymsp[-5].minor);
  yy_destructor(38,&yymsp[-4].minor);
  yy_destructor(37,&yymsp[-2].minor);
}
// 3326 "parser.c"
        break;
      case 20:
      case 25:
      case 46:
// 1045 "parser.lemon"
{
	yygotominor.yy277 = NULL;
  yy_destructor(42,&yymsp[-1].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3337 "parser.c"
        break;
      case 21:
      case 26:
      case 47:
// 1049 "parser.lemon"
{
	yygotominor.yy277 = yymsp[-1].minor.yy277;
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3348 "parser.c"
        break;
      case 22:
      case 48:
      case 96:
      case 165:
      case 190:
      case 212:
      case 278:
      case 281:
// 1053 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_list(yymsp[-2].minor.yy277, yymsp[0].minor.yy277);
  yy_destructor(6,&yymsp[-1].minor);
}
// 3363 "parser.c"
        break;
      case 24:
      case 250:
      case 285:
// 1061 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state);
}
// 3372 "parser.c"
        break;
      case 27:
// 1073 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class_definition(yymsp[0].minor.yy277, NULL, NULL, status->scanner_state);
}
// 3379 "parser.c"
        break;
      case 28:
// 1077 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class_definition(NULL, NULL, yymsp[0].minor.yy277, status->scanner_state);
}
// 3386 "parser.c"
        break;
      case 29:
// 1081 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class_definition(NULL, yymsp[0].minor.yy277, NULL, status->scanner_state);
}
// 3393 "parser.c"
        break;
      case 30:
// 1085 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class_definition(yymsp[-1].minor.yy277, yymsp[0].minor.yy277, NULL, status->scanner_state);
}
// 3400 "parser.c"
        break;
      case 31:
// 1089 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class_definition(yymsp[-1].minor.yy277, NULL, yymsp[0].minor.yy277, status->scanner_state);
}
// 3407 "parser.c"
        break;
      case 32:
// 1093 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class_definition(yymsp[0].minor.yy277, NULL, yymsp[-1].minor.yy277, status->scanner_state);
}
// 3414 "parser.c"
        break;
      case 33:
// 1097 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class_definition(NULL, yymsp[0].minor.yy277, yymsp[-1].minor.yy277, status->scanner_state);
}
// 3421 "parser.c"
        break;
      case 34:
// 1101 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class_definition(yymsp[-2].minor.yy277, yymsp[0].minor.yy277, yymsp[-1].minor.yy277, status->scanner_state);
}
// 3428 "parser.c"
        break;
      case 35:
// 1105 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class_definition(yymsp[-1].minor.yy277, yymsp[0].minor.yy277, yymsp[-2].minor.yy277, status->scanner_state);
}
// 3435 "parser.c"
        break;
      case 38:
// 1122 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class_property(yymsp[-2].minor.yy277, yymsp[-1].minor.yy0, NULL, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(35,&yymsp[0].minor);
}
// 3443 "parser.c"
        break;
      case 39:
// 1126 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class_property(yymsp[-2].minor.yy277, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(35,&yymsp[0].minor);
}
// 3451 "parser.c"
        break;
      case 40:
// 1130 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class_property(yymsp[-4].minor.yy277, yymsp[-3].minor.yy0, yymsp[-1].minor.yy277, yymsp[-5].minor.yy0, NULL, status->scanner_state);
  yy_destructor(45,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3460 "parser.c"
        break;
      case 41:
// 1134 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class_property(yymsp[-4].minor.yy277, yymsp[-3].minor.yy0, yymsp[-1].minor.yy277, NULL, NULL, status->scanner_state);
  yy_destructor(45,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3469 "parser.c"
        break;
      case 42:
// 1138 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class_property(yymsp[-3].minor.yy277, yymsp[-2].minor.yy0, NULL, yymsp[-4].minor.yy0, yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(35,&yymsp[0].minor);
}
// 3477 "parser.c"
        break;
      case 43:
// 1142 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class_property(yymsp[-3].minor.yy277, yymsp[-2].minor.yy0, NULL, NULL, yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(35,&yymsp[0].minor);
}
// 3485 "parser.c"
        break;
      case 44:
// 1146 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class_property(yymsp[-5].minor.yy277, yymsp[-4].minor.yy0, yymsp[-2].minor.yy277, yymsp[-6].minor.yy0, yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(45,&yymsp[-3].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3494 "parser.c"
        break;
      case 45:
// 1150 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class_property(yymsp[-5].minor.yy277, yymsp[-4].minor.yy0, yymsp[-2].minor.yy277, NULL, yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(45,&yymsp[-3].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3503 "parser.c"
        break;
      case 50:
// 1170 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_property_shortcut(NULL, yymsp[0].minor.yy0, status->scanner_state);
}
// 3510 "parser.c"
        break;
      case 51:
// 1174 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_property_shortcut(yymsp[-1].minor.yy0, yymsp[0].minor.yy0, status->scanner_state);
}
// 3517 "parser.c"
        break;
      case 58:
// 1203 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy277, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(46,&yymsp[-4].minor);
  yy_destructor(45,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3527 "parser.c"
        break;
      case 59:
// 1207 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy277, NULL, status->scanner_state);
  yy_destructor(46,&yymsp[-4].minor);
  yy_destructor(45,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3537 "parser.c"
        break;
      case 60:
// 1212 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class_method(yymsp[-6].minor.yy277, yymsp[-4].minor.yy0, NULL, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(48,&yymsp[-5].minor);
  yy_destructor(49,&yymsp[-3].minor);
  yy_destructor(32,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[-1].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3549 "parser.c"
        break;
      case 61:
// 1216 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class_method(yymsp[-7].minor.yy277, yymsp[-5].minor.yy0, yymsp[-3].minor.yy277, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(48,&yymsp[-6].minor);
  yy_destructor(49,&yymsp[-4].minor);
  yy_destructor(32,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[-1].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3561 "parser.c"
        break;
      case 62:
// 1220 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class_method(yymsp[-7].minor.yy277, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy277, NULL, NULL, status->scanner_state);
  yy_destructor(48,&yymsp[-6].minor);
  yy_destructor(49,&yymsp[-4].minor);
  yy_destructor(32,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3573 "parser.c"
        break;
      case 63:
// 1224 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class_method(yymsp[-8].minor.yy277, yymsp[-6].minor.yy0, yymsp[-4].minor.yy277, yymsp[-1].minor.yy277, NULL, NULL, status->scanner_state);
  yy_destructor(48,&yymsp[-7].minor);
  yy_destructor(49,&yymsp[-5].minor);
  yy_destructor(32,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3585 "parser.c"
        break;
      case 64:
// 1228 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class_method(yymsp[-6].minor.yy277, yymsp[-4].minor.yy0, NULL, NULL, yymsp[-7].minor.yy0, NULL, status->scanner_state);
  yy_destructor(48,&yymsp[-5].minor);
  yy_destructor(49,&yymsp[-3].minor);
  yy_destructor(32,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[-1].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3597 "parser.c"
        break;
      case 65:
// 1232 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class_method(yymsp[-7].minor.yy277, yymsp[-5].minor.yy0, yymsp[-3].minor.yy277, NULL, yymsp[-8].minor.yy0, NULL, status->scanner_state);
  yy_destructor(48,&yymsp[-6].minor);
  yy_destructor(49,&yymsp[-4].minor);
  yy_destructor(32,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[-1].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3609 "parser.c"
        break;
      case 66:
// 1236 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class_method(yymsp[-7].minor.yy277, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy277, yymsp[-8].minor.yy0, NULL, status->scanner_state);
  yy_destructor(48,&yymsp[-6].minor);
  yy_destructor(49,&yymsp[-4].minor);
  yy_destructor(32,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3621 "parser.c"
        break;
      case 67:
// 1240 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class_method(yymsp[-8].minor.yy277, yymsp[-6].minor.yy0, yymsp[-4].minor.yy277, yymsp[-1].minor.yy277, yymsp[-9].minor.yy0, NULL, status->scanner_state);
  yy_destructor(48,&yymsp[-7].minor);
  yy_destructor(49,&yymsp[-5].minor);
  yy_destructor(32,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3633 "parser.c"
        break;
      case 68:
// 1244 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class_method(yymsp[-8].minor.yy277, yymsp[-6].minor.yy0, NULL, NULL, NULL, yymsp[-2].minor.yy277, status->scanner_state);
  yy_destructor(48,&yymsp[-7].minor);
  yy_destructor(49,&yymsp[-5].minor);
  yy_destructor(32,&yymsp[-4].minor);
  yy_destructor(50,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-1].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3646 "parser.c"
        break;
      case 69:
// 1248 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class_method(yymsp[-9].minor.yy277, yymsp[-7].minor.yy0, yymsp[-5].minor.yy277, NULL, NULL, yymsp[-2].minor.yy277, status->scanner_state);
  yy_destructor(48,&yymsp[-8].minor);
  yy_destructor(49,&yymsp[-6].minor);
  yy_destructor(32,&yymsp[-4].minor);
  yy_destructor(50,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-1].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3659 "parser.c"
        break;
      case 70:
// 1252 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class_method(yymsp[-9].minor.yy277, yymsp[-7].minor.yy0, NULL, yymsp[-1].minor.yy277, NULL, yymsp[-3].minor.yy277, status->scanner_state);
  yy_destructor(48,&yymsp[-8].minor);
  yy_destructor(49,&yymsp[-6].minor);
  yy_destructor(32,&yymsp[-5].minor);
  yy_destructor(50,&yymsp[-4].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3672 "parser.c"
        break;
      case 71:
// 1256 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class_method(yymsp[-10].minor.yy277, yymsp[-8].minor.yy0, yymsp[-6].minor.yy277, yymsp[-1].minor.yy277, NULL, yymsp[-3].minor.yy277, status->scanner_state);
  yy_destructor(48,&yymsp[-9].minor);
  yy_destructor(49,&yymsp[-7].minor);
  yy_destructor(32,&yymsp[-5].minor);
  yy_destructor(50,&yymsp[-4].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3685 "parser.c"
        break;
      case 72:
// 1260 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class_method(yymsp[-8].minor.yy277, yymsp[-6].minor.yy0, NULL, NULL, yymsp[-9].minor.yy0, yymsp[-2].minor.yy277, status->scanner_state);
  yy_destructor(48,&yymsp[-7].minor);
  yy_destructor(49,&yymsp[-5].minor);
  yy_destructor(32,&yymsp[-4].minor);
  yy_destructor(50,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-1].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3698 "parser.c"
        break;
      case 73:
// 1264 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class_method(yymsp[-9].minor.yy277, yymsp[-7].minor.yy0, yymsp[-5].minor.yy277, NULL, yymsp[-10].minor.yy0, yymsp[-2].minor.yy277, status->scanner_state);
  yy_destructor(48,&yymsp[-8].minor);
  yy_destructor(49,&yymsp[-6].minor);
  yy_destructor(32,&yymsp[-4].minor);
  yy_destructor(50,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-1].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3711 "parser.c"
        break;
      case 74:
// 1268 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class_method(yymsp[-9].minor.yy277, yymsp[-7].minor.yy0, NULL, yymsp[-1].minor.yy277, yymsp[-10].minor.yy0, yymsp[-3].minor.yy277, status->scanner_state);
  yy_destructor(48,&yymsp[-8].minor);
  yy_destructor(49,&yymsp[-6].minor);
  yy_destructor(32,&yymsp[-5].minor);
  yy_destructor(50,&yymsp[-4].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3724 "parser.c"
        break;
      case 75:
// 1272 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class_method(yymsp[-10].minor.yy277, yymsp[-8].minor.yy0, yymsp[-6].minor.yy277, yymsp[-1].minor.yy277, yymsp[-11].minor.yy0, yymsp[-3].minor.yy277, status->scanner_state);
  yy_destructor(48,&yymsp[-9].minor);
  yy_destructor(49,&yymsp[-7].minor);
  yy_destructor(32,&yymsp[-5].minor);
  yy_destructor(50,&yymsp[-4].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3737 "parser.c"
        break;
      case 76:
// 1276 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class_method(yymsp[-7].minor.yy277, yymsp[-5].minor.yy0, NULL, NULL, NULL, yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(48,&yymsp[-6].minor);
  yy_destructor(49,&yymsp[-4].minor);
  yy_destructor(32,&yymsp[-3].minor);
  yy_destructor(50,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3749 "parser.c"
        break;
      case 77:
// 1280 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class_method(yymsp[-8].minor.yy277, yymsp[-6].minor.yy0, yymsp[-4].minor.yy277, NULL, NULL, yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(48,&yymsp[-7].minor);
  yy_destructor(49,&yymsp[-5].minor);
  yy_destructor(32,&yymsp[-3].minor);
  yy_destructor(50,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3761 "parser.c"
        break;
      case 78:
// 1284 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class_method(yymsp[-7].minor.yy277, yymsp[-5].minor.yy0, NULL, NULL, yymsp[-8].minor.yy0, yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(48,&yymsp[-6].minor);
  yy_destructor(49,&yymsp[-4].minor);
  yy_destructor(32,&yymsp[-3].minor);
  yy_destructor(50,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3773 "parser.c"
        break;
      case 79:
// 1288 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class_method(yymsp[-8].minor.yy277, yymsp[-6].minor.yy0, yymsp[-4].minor.yy277, NULL, yymsp[-9].minor.yy0, yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(48,&yymsp[-7].minor);
  yy_destructor(49,&yymsp[-5].minor);
  yy_destructor(32,&yymsp[-3].minor);
  yy_destructor(50,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3785 "parser.c"
        break;
      case 80:
// 1293 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class_method(yymsp[-5].minor.yy277, yymsp[-3].minor.yy0, NULL, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(48,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[-1].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3796 "parser.c"
        break;
      case 81:
// 1297 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class_method(yymsp[-6].minor.yy277, yymsp[-4].minor.yy0, yymsp[-2].minor.yy277, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(48,&yymsp[-5].minor);
  yy_destructor(49,&yymsp[-3].minor);
  yy_destructor(32,&yymsp[-1].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3807 "parser.c"
        break;
      case 82:
// 1301 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class_method(yymsp[-5].minor.yy277, yymsp[-3].minor.yy0, NULL, NULL, yymsp[-6].minor.yy0, NULL, status->scanner_state);
  yy_destructor(48,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[-1].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3818 "parser.c"
        break;
      case 83:
// 1305 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class_method(yymsp[-6].minor.yy277, yymsp[-4].minor.yy0, yymsp[-2].minor.yy277, NULL, yymsp[-7].minor.yy0, NULL, status->scanner_state);
  yy_destructor(48,&yymsp[-5].minor);
  yy_destructor(49,&yymsp[-3].minor);
  yy_destructor(32,&yymsp[-1].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3829 "parser.c"
        break;
      case 86:
// 1318 "parser.lemon"
{
	yygotominor.yy277 = json_object_new_string("public");
  yy_destructor(1,&yymsp[0].minor);
}
// 3837 "parser.c"
        break;
      case 87:
// 1322 "parser.lemon"
{
	yygotominor.yy277 = json_object_new_string("protected");
  yy_destructor(2,&yymsp[0].minor);
}
// 3845 "parser.c"
        break;
      case 88:
// 1326 "parser.lemon"
{
	yygotominor.yy277 = json_object_new_string("private");
  yy_destructor(4,&yymsp[0].minor);
}
// 3853 "parser.c"
        break;
      case 89:
// 1330 "parser.lemon"
{
	yygotominor.yy277 = json_object_new_string("static");
  yy_destructor(3,&yymsp[0].minor);
}
// 3861 "parser.c"
        break;
      case 90:
// 1334 "parser.lemon"
{
	yygotominor.yy277 = json_object_new_string("scoped");
  yy_destructor(5,&yymsp[0].minor);
}
// 3869 "parser.c"
        break;
      case 91:
// 1338 "parser.lemon"
{
	yygotominor.yy277 = json_object_new_string("inline");
  yy_destructor(51,&yymsp[0].minor);
}
// 3877 "parser.c"
        break;
      case 92:
// 1342 "parser.lemon"
{
	yygotominor.yy277 = json_object_new_string("abstract");
  yy_destructor(40,&yymsp[0].minor);
}
// 3885 "parser.c"
        break;
      case 93:
// 1346 "parser.lemon"
{
	yygotominor.yy277 = json_object_new_string("final");
  yy_destructor(41,&yymsp[0].minor);
}
// 3893 "parser.c"
        break;
      case 98:
// 1368 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_parameter(NULL, NULL, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 3900 "parser.c"
        break;
      case 99:
// 1372 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_parameter(yymsp[-1].minor.yy277, NULL, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 3907 "parser.c"
        break;
      case 100:
// 1376 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_parameter(NULL, yymsp[-1].minor.yy277, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 3914 "parser.c"
        break;
      case 101:
// 1380 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_parameter(NULL, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy277, status->scanner_state);
  yy_destructor(45,&yymsp[-1].minor);
}
// 3922 "parser.c"
        break;
      case 102:
// 1384 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_parameter(yymsp[-3].minor.yy277, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy277, status->scanner_state);
  yy_destructor(45,&yymsp[-1].minor);
}
// 3930 "parser.c"
        break;
      case 103:
// 1388 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_parameter(NULL, yymsp[-3].minor.yy277, yymsp[-2].minor.yy0, yymsp[0].minor.yy277, status->scanner_state);
  yy_destructor(45,&yymsp[-1].minor);
}
// 3938 "parser.c"
        break;
      case 104:
// 1392 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(15,&yymsp[-2].minor);
  yy_destructor(16,&yymsp[0].minor);
}
// 3947 "parser.c"
        break;
      case 105:
// 1396 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_type(XX_TYPE_INTEGER);
  yy_destructor(52,&yymsp[0].minor);
}
// 3955 "parser.c"
        break;
      case 106:
// 1400 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_type(XX_TYPE_UINTEGER);
  yy_destructor(53,&yymsp[0].minor);
}
// 3963 "parser.c"
        break;
      case 107:
// 1404 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_type(XX_TYPE_LONG);
  yy_destructor(54,&yymsp[0].minor);
}
// 3971 "parser.c"
        break;
      case 108:
// 1408 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_type(XX_TYPE_ULONG);
  yy_destructor(55,&yymsp[0].minor);
}
// 3979 "parser.c"
        break;
      case 109:
// 1412 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_type(XX_TYPE_CHAR);
  yy_destructor(56,&yymsp[0].minor);
}
// 3987 "parser.c"
        break;
      case 110:
// 1416 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_type(XX_TYPE_UCHAR);
  yy_destructor(57,&yymsp[0].minor);
}
// 3995 "parser.c"
        break;
      case 111:
// 1420 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_type(XX_TYPE_DOUBLE);
  yy_destructor(58,&yymsp[0].minor);
}
// 4003 "parser.c"
        break;
      case 112:
// 1424 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_type(XX_TYPE_BOOL);
  yy_destructor(59,&yymsp[0].minor);
}
// 4011 "parser.c"
        break;
      case 113:
// 1428 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_type(XX_TYPE_STRING);
  yy_destructor(60,&yymsp[0].minor);
}
// 4019 "parser.c"
        break;
      case 114:
// 1432 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_type(XX_TYPE_ARRAY);
  yy_destructor(61,&yymsp[0].minor);
}
// 4027 "parser.c"
        break;
      case 115:
// 1436 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_type(XX_TYPE_VAR);
  yy_destructor(62,&yymsp[0].minor);
}
// 4035 "parser.c"
        break;
      case 138:
// 1528 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_empty_statement(status->scanner_state);
  yy_destructor(35,&yymsp[0].minor);
}
// 4043 "parser.c"
        break;
      case 139:
// 1532 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_break_statement(status->scanner_state);
  yy_destructor(63,&yymsp[-1].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4052 "parser.c"
        break;
      case 140:
// 1536 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_continue_statement(status->scanner_state);
  yy_destructor(64,&yymsp[-1].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4061 "parser.c"
        break;
      case 141:
// 1540 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_if_statement(yymsp[-2].minor.yy277, NULL, NULL, status->scanner_state);
  yy_destructor(65,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-1].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 4071 "parser.c"
        break;
      case 142:
// 1544 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_if_statement(yymsp[-5].minor.yy277, NULL, NULL, status->scanner_state);
  yy_destructor(65,&yymsp[-6].minor);
  yy_destructor(42,&yymsp[-4].minor);
  yy_destructor(43,&yymsp[-3].minor);
  yy_destructor(66,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[-1].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 4084 "parser.c"
        break;
      case 143:
// 1548 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_if_statement(yymsp[-3].minor.yy277, yymsp[-1].minor.yy277, NULL, status->scanner_state);
  yy_destructor(65,&yymsp[-4].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 4094 "parser.c"
        break;
      case 144:
// 1552 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_if_statement(yymsp[-7].minor.yy277, yymsp[-5].minor.yy277, yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(65,&yymsp[-8].minor);
  yy_destructor(42,&yymsp[-6].minor);
  yy_destructor(43,&yymsp[-4].minor);
  yy_destructor(66,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 4107 "parser.c"
        break;
      case 145:
// 1556 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_if_statement(yymsp[-6].minor.yy277, yymsp[-4].minor.yy277, NULL, status->scanner_state);
  yy_destructor(65,&yymsp[-7].minor);
  yy_destructor(42,&yymsp[-5].minor);
  yy_destructor(43,&yymsp[-3].minor);
  yy_destructor(66,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[-1].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 4120 "parser.c"
        break;
      case 146:
// 1560 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_if_statement(yymsp[-6].minor.yy277, NULL, yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(65,&yymsp[-7].minor);
  yy_destructor(42,&yymsp[-5].minor);
  yy_destructor(43,&yymsp[-4].minor);
  yy_destructor(66,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 4133 "parser.c"
        break;
      case 147:
// 1564 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_switch_statement(yymsp[-2].minor.yy277, NULL, status->scanner_state);
  yy_destructor(67,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-1].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 4143 "parser.c"
        break;
      case 148:
// 1568 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_switch_statement(yymsp[-3].minor.yy277, yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(67,&yymsp[-4].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 4153 "parser.c"
        break;
      case 151:
// 1580 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_case_clause(yymsp[-1].minor.yy277, NULL, status->scanner_state);
  yy_destructor(68,&yymsp[-2].minor);
  yy_destructor(69,&yymsp[0].minor);
}
// 4162 "parser.c"
        break;
      case 152:
// 1584 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_case_clause(yymsp[-2].minor.yy277, yymsp[0].minor.yy277, status->scanner_state);
  yy_destructor(68,&yymsp[-3].minor);
  yy_destructor(69,&yymsp[-1].minor);
}
// 4171 "parser.c"
        break;
      case 153:
// 1588 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_case_clause(NULL, yymsp[0].minor.yy277, status->scanner_state);
  yy_destructor(70,&yymsp[-2].minor);
  yy_destructor(69,&yymsp[-1].minor);
}
// 4180 "parser.c"
        break;
      case 154:
// 1592 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_loop_statement(NULL, status->scanner_state);
  yy_destructor(71,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[-1].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 4190 "parser.c"
        break;
      case 155:
// 1596 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_loop_statement(yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(71,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 4200 "parser.c"
        break;
      case 156:
// 1600 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_while_statement(yymsp[-2].minor.yy277, NULL, status->scanner_state);
  yy_destructor(72,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-1].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 4210 "parser.c"
        break;
      case 157:
// 1604 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_while_statement(yymsp[-3].minor.yy277, yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(72,&yymsp[-4].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 4220 "parser.c"
        break;
      case 158:
// 1608 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_do_while_statement(yymsp[-1].minor.yy277, NULL, status->scanner_state);
  yy_destructor(73,&yymsp[-5].minor);
  yy_destructor(42,&yymsp[-4].minor);
  yy_destructor(43,&yymsp[-3].minor);
  yy_destructor(72,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4232 "parser.c"
        break;
      case 159:
// 1612 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_do_while_statement(yymsp[-1].minor.yy277, yymsp[-4].minor.yy277, status->scanner_state);
  yy_destructor(73,&yymsp[-6].minor);
  yy_destructor(42,&yymsp[-5].minor);
  yy_destructor(43,&yymsp[-3].minor);
  yy_destructor(72,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4244 "parser.c"
        break;
      case 160:
// 1616 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_for_statement(yymsp[-3].minor.yy277, NULL, yymsp[-5].minor.yy0, 0, yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(74,&yymsp[-6].minor);
  yy_destructor(75,&yymsp[-4].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 4255 "parser.c"
        break;
      case 161:
// 1620 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_for_statement(yymsp[-3].minor.yy277, NULL, yymsp[-6].minor.yy0, 1, yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(74,&yymsp[-7].minor);
  yy_destructor(75,&yymsp[-5].minor);
  yy_destructor(76,&yymsp[-4].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 4267 "parser.c"
        break;
      case 162:
// 1624 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_for_statement(yymsp[-3].minor.yy277, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, 0, yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(74,&yymsp[-8].minor);
  yy_destructor(6,&yymsp[-6].minor);
  yy_destructor(75,&yymsp[-4].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 4279 "parser.c"
        break;
      case 163:
// 1628 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_for_statement(yymsp[-3].minor.yy277, yymsp[-8].minor.yy0, yymsp[-6].minor.yy0, 1, yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(74,&yymsp[-9].minor);
  yy_destructor(6,&yymsp[-7].minor);
  yy_destructor(75,&yymsp[-5].minor);
  yy_destructor(76,&yymsp[-4].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 4292 "parser.c"
        break;
      case 164:
// 1632 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_let_statement(yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(77,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4301 "parser.c"
        break;
      case 167:
// 1645 "parser.lemon"
{
	yygotominor.yy277 = json_object_new_string("assign");
  yy_destructor(45,&yymsp[0].minor);
}
// 4309 "parser.c"
        break;
      case 168:
// 1650 "parser.lemon"
{
	yygotominor.yy277 = json_object_new_string("add-assign");
  yy_destructor(78,&yymsp[0].minor);
}
// 4317 "parser.c"
        break;
      case 169:
// 1655 "parser.lemon"
{
	yygotominor.yy277 = json_object_new_string("sub-assign");
  yy_destructor(79,&yymsp[0].minor);
}
// 4325 "parser.c"
        break;
      case 170:
// 1659 "parser.lemon"
{
	yygotominor.yy277 = json_object_new_string("mult-assign");
  yy_destructor(80,&yymsp[0].minor);
}
// 4333 "parser.c"
        break;
      case 171:
// 1663 "parser.lemon"
{
	yygotominor.yy277 = json_object_new_string("div-assign");
  yy_destructor(81,&yymsp[0].minor);
}
// 4341 "parser.c"
        break;
      case 172:
// 1667 "parser.lemon"
{
	yygotominor.yy277 = json_object_new_string("concat-assign");
  yy_destructor(82,&yymsp[0].minor);
}
// 4349 "parser.c"
        break;
      case 173:
// 1672 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_let_assignment("variable", yymsp[-1].minor.yy277, yymsp[-2].minor.yy0, NULL, NULL, yymsp[0].minor.yy277, status->scanner_state);
}
// 4356 "parser.c"
        break;
      case 174:
// 1677 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_let_assignment("object-property", yymsp[-1].minor.yy277, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, yymsp[0].minor.yy277, status->scanner_state);
  yy_destructor(50,&yymsp[-3].minor);
}
// 4364 "parser.c"
        break;
      case 175:
// 1682 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_let_assignment("variable-dynamic-object-property", yymsp[-1].minor.yy277, yymsp[-6].minor.yy0, yymsp[-3].minor.yy0, NULL, yymsp[0].minor.yy277, status->scanner_state);
  yy_destructor(50,&yymsp[-5].minor);
  yy_destructor(42,&yymsp[-4].minor);
  yy_destructor(43,&yymsp[-2].minor);
}
// 4374 "parser.c"
        break;
      case 176:
// 1687 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_let_assignment("string-dynamic-object-property", yymsp[-1].minor.yy277, yymsp[-6].minor.yy0, yymsp[-3].minor.yy0, NULL, yymsp[0].minor.yy277, status->scanner_state);
  yy_destructor(50,&yymsp[-5].minor);
  yy_destructor(42,&yymsp[-4].minor);
  yy_destructor(43,&yymsp[-2].minor);
}
// 4384 "parser.c"
        break;
      case 177:
// 1692 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_let_assignment("object-property-append", yymsp[-1].minor.yy277, yymsp[-6].minor.yy0, yymsp[-4].minor.yy0, NULL, yymsp[0].minor.yy277, status->scanner_state);
  yy_destructor(50,&yymsp[-5].minor);
  yy_destructor(84,&yymsp[-3].minor);
  yy_destructor(85,&yymsp[-2].minor);
}
// 4394 "parser.c"
        break;
      case 178:
// 1697 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_let_assignment("object-property-array-index", yymsp[-1].minor.yy277, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, yymsp[-3].minor.yy277, yymsp[0].minor.yy277, status->scanner_state);
  yy_destructor(50,&yymsp[-6].minor);
  yy_destructor(84,&yymsp[-4].minor);
  yy_destructor(85,&yymsp[-2].minor);
}
// 4404 "parser.c"
        break;
      case 179:
// 1702 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_let_assignment("static-property", yymsp[-1].minor.yy277, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, yymsp[0].minor.yy277, status->scanner_state);
  yy_destructor(86,&yymsp[-3].minor);
}
// 4412 "parser.c"
        break;
      case 180:
// 1707 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_let_assignment("static-property-append", yymsp[-1].minor.yy277, yymsp[-6].minor.yy0, yymsp[-4].minor.yy0, NULL, yymsp[0].minor.yy277, status->scanner_state);
  yy_destructor(86,&yymsp[-5].minor);
  yy_destructor(84,&yymsp[-3].minor);
  yy_destructor(85,&yymsp[-2].minor);
}
// 4422 "parser.c"
        break;
      case 181:
// 1712 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_let_assignment("static-property-array-index", yymsp[-1].minor.yy277, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, yymsp[-3].minor.yy277, yymsp[0].minor.yy277, status->scanner_state);
  yy_destructor(86,&yymsp[-6].minor);
  yy_destructor(84,&yymsp[-4].minor);
  yy_destructor(85,&yymsp[-2].minor);
}
// 4432 "parser.c"
        break;
      case 182:
// 1717 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_let_assignment("variable-append", yymsp[-1].minor.yy277, yymsp[-4].minor.yy0, NULL, NULL, yymsp[0].minor.yy277, status->scanner_state);
  yy_destructor(84,&yymsp[-3].minor);
  yy_destructor(85,&yymsp[-2].minor);
}
// 4441 "parser.c"
        break;
      case 183:
// 1722 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_let_assignment("array-index", yymsp[-1].minor.yy277, yymsp[-5].minor.yy0, NULL, yymsp[-3].minor.yy277, yymsp[0].minor.yy277, status->scanner_state);
  yy_destructor(84,&yymsp[-4].minor);
  yy_destructor(85,&yymsp[-2].minor);
}
// 4450 "parser.c"
        break;
      case 184:
// 1727 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_let_assignment("incr", NULL, yymsp[-3].minor.yy0, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(50,&yymsp[-2].minor);
  yy_destructor(87,&yymsp[0].minor);
}
// 4459 "parser.c"
        break;
      case 185:
// 1732 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_let_assignment("decr", NULL, yymsp[-3].minor.yy0, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(50,&yymsp[-2].minor);
  yy_destructor(88,&yymsp[0].minor);
}
// 4468 "parser.c"
        break;
      case 186:
// 1737 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_let_assignment("incr", NULL, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(87,&yymsp[0].minor);
}
// 4476 "parser.c"
        break;
      case 187:
// 1742 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_let_assignment("decr", NULL, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(88,&yymsp[0].minor);
}
// 4484 "parser.c"
        break;
      case 189:
// 1750 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_echo_statement(yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(89,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4493 "parser.c"
        break;
      case 192:
// 1762 "parser.lemon"
{
	yygotominor.yy277 = yymsp[0].minor.yy277;;
}
// 4500 "parser.c"
        break;
      case 193:
// 1767 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_mcall_statement(yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(35,&yymsp[0].minor);
}
// 4508 "parser.c"
        break;
      case 194:
// 1772 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_fcall_statement(yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(35,&yymsp[0].minor);
}
// 4516 "parser.c"
        break;
      case 195:
// 1777 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_scall_statement(yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(35,&yymsp[0].minor);
}
// 4524 "parser.c"
        break;
      case 196:
// 1782 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_fetch_statement(yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(35,&yymsp[0].minor);
}
// 4532 "parser.c"
        break;
      case 197:
// 1787 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_return_statement(yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(90,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4541 "parser.c"
        break;
      case 198:
// 1792 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_return_statement(NULL, status->scanner_state);
  yy_destructor(90,&yymsp[-1].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4550 "parser.c"
        break;
      case 199:
// 1797 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(91,&yymsp[-4].minor);
  yy_destructor(50,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4564 "parser.c"
        break;
      case 200:
// 1806 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state),
		yymsp[-2].minor.yy277,
		status->scanner_state
	);
  yy_destructor(91,&yymsp[-5].minor);
  yy_destructor(84,&yymsp[-3].minor);
  yy_destructor(85,&yymsp[-1].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4579 "parser.c"
        break;
      case 201:
// 1815 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_throw_exception(yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(92,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4588 "parser.c"
        break;
      case 202:
// 1819 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_declare_statement(XX_T_TYPE_INTEGER, yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(52,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4597 "parser.c"
        break;
      case 203:
// 1823 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_declare_statement(XX_T_TYPE_UINTEGER, yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(53,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4606 "parser.c"
        break;
      case 204:
// 1827 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_declare_statement(XX_T_TYPE_CHAR, yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(56,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4615 "parser.c"
        break;
      case 205:
// 1831 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_declare_statement(XX_T_TYPE_UCHAR, yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(57,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4624 "parser.c"
        break;
      case 206:
// 1835 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_declare_statement(XX_T_TYPE_LONG, yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4633 "parser.c"
        break;
      case 207:
// 1839 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_declare_statement(XX_T_TYPE_ULONG, yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(55,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4642 "parser.c"
        break;
      case 208:
// 1843 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_declare_statement(XX_T_TYPE_DOUBLE, yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(58,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4651 "parser.c"
        break;
      case 209:
// 1847 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_declare_statement(XX_T_TYPE_STRING, yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(60,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4660 "parser.c"
        break;
      case 210:
// 1851 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_declare_statement(XX_T_TYPE_BOOL, yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(59,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4669 "parser.c"
        break;
      case 211:
// 1855 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_declare_statement(XX_T_TYPE_VAR, yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(62,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4678 "parser.c"
        break;
      case 214:
// 1867 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_declare_variable(yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 4685 "parser.c"
        break;
      case 215:
// 1871 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_declare_variable(yymsp[-2].minor.yy0, yymsp[0].minor.yy277, status->scanner_state);
  yy_destructor(45,&yymsp[-1].minor);
}
// 4693 "parser.c"
        break;
      case 217:
// 1879 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_expr("not", yymsp[0].minor.yy277, NULL, NULL, status->scanner_state);
  yy_destructor(31,&yymsp[-1].minor);
}
// 4701 "parser.c"
        break;
      case 218:
// 1883 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_expr("isset", yymsp[0].minor.yy277, NULL, NULL, status->scanner_state);
  yy_destructor(27,&yymsp[-1].minor);
}
// 4709 "parser.c"
        break;
      case 219:
// 1887 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_expr("require", yymsp[0].minor.yy277, NULL, NULL, status->scanner_state);
  yy_destructor(7,&yymsp[-1].minor);
}
// 4717 "parser.c"
        break;
      case 220:
// 1891 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_expr("clone", yymsp[0].minor.yy277, NULL, NULL, status->scanner_state);
  yy_destructor(8,&yymsp[-1].minor);
}
// 4725 "parser.c"
        break;
      case 221:
// 1895 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_expr("empty", yymsp[0].minor.yy277, NULL, NULL, status->scanner_state);
  yy_destructor(9,&yymsp[-1].minor);
}
// 4733 "parser.c"
        break;
      case 222:
// 1899 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_expr("equals", yymsp[-2].minor.yy277, yymsp[0].minor.yy277, NULL, status->scanner_state);
  yy_destructor(13,&yymsp[-1].minor);
}
// 4741 "parser.c"
        break;
      case 223:
// 1903 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_expr("not-equals", yymsp[-2].minor.yy277, yymsp[0].minor.yy277, NULL, status->scanner_state);
  yy_destructor(20,&yymsp[-1].minor);
}
// 4749 "parser.c"
        break;
      case 224:
// 1907 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_expr("identical", yymsp[-2].minor.yy277, yymsp[0].minor.yy277, NULL, status->scanner_state);
  yy_destructor(14,&yymsp[-1].minor);
}
// 4757 "parser.c"
        break;
      case 225:
// 1911 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_expr("not-identical", yymsp[-2].minor.yy277, yymsp[0].minor.yy277, NULL, status->scanner_state);
  yy_destructor(19,&yymsp[-1].minor);
}
// 4765 "parser.c"
        break;
      case 226:
// 1915 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_expr("less", yymsp[-2].minor.yy277, yymsp[0].minor.yy277, NULL, status->scanner_state);
  yy_destructor(15,&yymsp[-1].minor);
}
// 4773 "parser.c"
        break;
      case 227:
// 1919 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_expr("greater", yymsp[-2].minor.yy277, yymsp[0].minor.yy277, NULL, status->scanner_state);
  yy_destructor(16,&yymsp[-1].minor);
}
// 4781 "parser.c"
        break;
      case 228:
// 1923 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_expr("less-equal", yymsp[-2].minor.yy277, yymsp[0].minor.yy277, NULL, status->scanner_state);
  yy_destructor(17,&yymsp[-1].minor);
}
// 4789 "parser.c"
        break;
      case 229:
// 1927 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_expr("greater-equal", yymsp[-2].minor.yy277, yymsp[0].minor.yy277, NULL, status->scanner_state);
  yy_destructor(18,&yymsp[-1].minor);
}
// 4797 "parser.c"
        break;
      case 230:
// 1931 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_expr("list", yymsp[-1].minor.yy277, NULL, NULL, status->scanner_state);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 4806 "parser.c"
        break;
      case 231:
// 1935 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_expr("cast", yymsp[-2].minor.yy277, yymsp[0].minor.yy277, NULL, status->scanner_state);
  yy_destructor(49,&yymsp[-3].minor);
  yy_destructor(32,&yymsp[-1].minor);
}
// 4815 "parser.c"
        break;
      case 232:
// 1939 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_expr("type-hint", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), yymsp[0].minor.yy277, NULL, status->scanner_state);
  yy_destructor(15,&yymsp[-3].minor);
  yy_destructor(16,&yymsp[-1].minor);
}
// 4824 "parser.c"
        break;
      case 233:
      case 298:
// 1943 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_expr("property-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(50,&yymsp[-1].minor);
}
// 4833 "parser.c"
        break;
      case 234:
// 1947 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_expr("static-property-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(86,&yymsp[-1].minor);
}
// 4841 "parser.c"
        break;
      case 235:
      case 296:
// 1951 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_expr("static-constant-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(86,&yymsp[-1].minor);
}
// 4850 "parser.c"
        break;
      case 236:
      case 297:
// 1956 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), yymsp[-1].minor.yy277, NULL, status->scanner_state);
  yy_destructor(84,&yymsp[-2].minor);
  yy_destructor(85,&yymsp[0].minor);
}
// 4860 "parser.c"
        break;
      case 237:
// 1961 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_expr("add", yymsp[-2].minor.yy277, yymsp[0].minor.yy277, NULL, status->scanner_state);
  yy_destructor(21,&yymsp[-1].minor);
}
// 4868 "parser.c"
        break;
      case 238:
// 1966 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_expr("sub", yymsp[-2].minor.yy277, yymsp[0].minor.yy277, NULL, status->scanner_state);
  yy_destructor(22,&yymsp[-1].minor);
}
// 4876 "parser.c"
        break;
      case 239:
// 1971 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_expr("mul", yymsp[-2].minor.yy277, yymsp[0].minor.yy277, NULL, status->scanner_state);
  yy_destructor(24,&yymsp[-1].minor);
}
// 4884 "parser.c"
        break;
      case 240:
// 1976 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_expr("div", yymsp[-2].minor.yy277, yymsp[0].minor.yy277, NULL, status->scanner_state);
  yy_destructor(25,&yymsp[-1].minor);
}
// 4892 "parser.c"
        break;
      case 241:
// 1981 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_expr("mod", yymsp[-2].minor.yy277, yymsp[0].minor.yy277, NULL, status->scanner_state);
  yy_destructor(26,&yymsp[-1].minor);
}
// 4900 "parser.c"
        break;
      case 242:
// 1986 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_expr("concat", yymsp[-2].minor.yy277, yymsp[0].minor.yy277, NULL, status->scanner_state);
  yy_destructor(23,&yymsp[-1].minor);
}
// 4908 "parser.c"
        break;
      case 243:
// 1991 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_expr("and", yymsp[-2].minor.yy277, yymsp[0].minor.yy277, NULL, status->scanner_state);
  yy_destructor(11,&yymsp[-1].minor);
}
// 4916 "parser.c"
        break;
      case 244:
// 1996 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_expr("or", yymsp[-2].minor.yy277, yymsp[0].minor.yy277, NULL, status->scanner_state);
  yy_destructor(12,&yymsp[-1].minor);
}
// 4924 "parser.c"
        break;
      case 245:
// 2001 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_expr("instanceof", yymsp[-2].minor.yy277, yymsp[0].minor.yy277, NULL, status->scanner_state);
  yy_destructor(10,&yymsp[-1].minor);
}
// 4932 "parser.c"
        break;
      case 246:
// 2006 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_expr("fetch", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), yymsp[0].minor.yy277, NULL, status->scanner_state);
  yy_destructor(28,&yymsp[-3].minor);
  yy_destructor(6,&yymsp[-1].minor);
}
// 4941 "parser.c"
        break;
      case 248:
// 2016 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_expr("typeof", yymsp[0].minor.yy277, NULL, NULL, status->scanner_state);
  yy_destructor(29,&yymsp[-1].minor);
}
// 4949 "parser.c"
        break;
      case 249:
// 2021 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_literal(XX_T_CONSTANT, yymsp[0].minor.yy0, status->scanner_state);
}
// 4956 "parser.c"
        break;
      case 251:
      case 287:
      case 289:
// 2031 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_literal(XX_T_INTEGER, yymsp[0].minor.yy0, status->scanner_state);
}
// 4965 "parser.c"
        break;
      case 252:
      case 286:
      case 291:
// 2036 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_literal(XX_T_STRING, yymsp[0].minor.yy0, status->scanner_state);
}
// 4974 "parser.c"
        break;
      case 253:
      case 290:
// 2041 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_literal(XX_T_CHAR, yymsp[0].minor.yy0, status->scanner_state);
}
// 4982 "parser.c"
        break;
      case 254:
      case 292:
// 2046 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_literal(XX_T_DOUBLE, yymsp[0].minor.yy0, status->scanner_state);
}
// 4990 "parser.c"
        break;
      case 255:
      case 293:
// 2051 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_literal(XX_T_NULL, NULL, status->scanner_state);
  yy_destructor(96,&yymsp[0].minor);
}
// 4999 "parser.c"
        break;
      case 256:
      case 295:
// 2056 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_literal(XX_T_TRUE, NULL, status->scanner_state);
  yy_destructor(97,&yymsp[0].minor);
}
// 5008 "parser.c"
        break;
      case 257:
      case 294:
// 2061 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_literal(XX_T_FALSE, NULL, status->scanner_state);
  yy_destructor(98,&yymsp[0].minor);
}
// 5017 "parser.c"
        break;
      case 258:
// 2066 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_expr("empty-array", NULL, NULL, NULL, status->scanner_state);
  yy_destructor(84,&yymsp[-1].minor);
  yy_destructor(85,&yymsp[0].minor);
}
// 5026 "parser.c"
        break;
      case 259:
// 2071 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_expr("array", yymsp[-1].minor.yy277, NULL, NULL, status->scanner_state);
  yy_destructor(84,&yymsp[-2].minor);
  yy_destructor(85,&yymsp[0].minor);
}
// 5035 "parser.c"
        break;
      case 260:
// 2076 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_new_instance(yymsp[0].minor.yy0, NULL, status->scanner_state);
  yy_destructor(30,&yymsp[-1].minor);
}
// 5043 "parser.c"
        break;
      case 261:
// 2081 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_new_instance(yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(30,&yymsp[-3].minor);
  yy_destructor(49,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 5053 "parser.c"
        break;
      case 262:
// 2086 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_new_instance(yymsp[-3].minor.yy0, yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(30,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 5063 "parser.c"
        break;
      case 263:
// 2091 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_fcall(1, yymsp[-3].minor.yy0, yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 5072 "parser.c"
        break;
      case 264:
// 2096 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_fcall(1, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(49,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 5081 "parser.c"
        break;
      case 265:
// 2101 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_fcall(2, yymsp[-4].minor.yy0, yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(42,&yymsp[-5].minor);
  yy_destructor(43,&yymsp[-3].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 5092 "parser.c"
        break;
      case 266:
// 2106 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_fcall(2, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(42,&yymsp[-4].minor);
  yy_destructor(43,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 5103 "parser.c"
        break;
      case 267:
// 2111 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_scall(yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(86,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 5113 "parser.c"
        break;
      case 268:
// 2116 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_scall(yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(86,&yymsp[-3].minor);
  yy_destructor(49,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 5123 "parser.c"
        break;
      case 269:
// 2121 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_mcall(1, yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(50,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 5133 "parser.c"
        break;
      case 270:
// 2126 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_mcall(1, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(50,&yymsp[-3].minor);
  yy_destructor(49,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 5143 "parser.c"
        break;
      case 271:
// 2131 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_mcall(2, yymsp[-7].minor.yy0, yymsp[-4].minor.yy0, yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(50,&yymsp[-6].minor);
  yy_destructor(42,&yymsp[-5].minor);
  yy_destructor(43,&yymsp[-3].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 5155 "parser.c"
        break;
      case 272:
// 2136 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_mcall(2, yymsp[-6].minor.yy0, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(50,&yymsp[-5].minor);
  yy_destructor(42,&yymsp[-4].minor);
  yy_destructor(43,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 5167 "parser.c"
        break;
      case 273:
// 2141 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_mcall(3, yymsp[-7].minor.yy0, yymsp[-4].minor.yy0, yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(50,&yymsp[-6].minor);
  yy_destructor(42,&yymsp[-5].minor);
  yy_destructor(43,&yymsp[-3].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 5179 "parser.c"
        break;
      case 274:
// 2146 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_mcall(3, yymsp[-6].minor.yy0, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(50,&yymsp[-5].minor);
  yy_destructor(42,&yymsp[-4].minor);
  yy_destructor(43,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 5191 "parser.c"
        break;
      case 283:
// 2185 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_array_item(yymsp[-2].minor.yy277, yymsp[0].minor.yy277, status->scanner_state);
  yy_destructor(69,&yymsp[-1].minor);
}
// 5199 "parser.c"
        break;
      case 284:
// 2189 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_array_item(NULL, yymsp[0].minor.yy277, status->scanner_state);
}
// 5206 "parser.c"
        break;
      case 299:
// 2252 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_expr("property-dynamic-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(50,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 5216 "parser.c"
        break;
      case 300:
// 2257 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_expr("property-string-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_STRING, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(50,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 5226 "parser.c"
        break;
      case 302:
// 2265 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_comment(yymsp[0].minor.yy0, status->scanner_state);
}
// 5233 "parser.c"
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
// 920 "parser.lemon"


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

// 5309 "parser.c"
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
 +----------------------------------------------------------------------+
 | Zephir Language                                                      |
 +----------------------------------------------------------------------+
 | Copyright (c) 2013 Zephir Team                                       |
 +----------------------------------------------------------------------+
 | This source file is subject to version 1.0 of the Zephir license,    |
 | that is bundled with this package in the file LICENSE, and is        |
 | available through the world-wide-web at the following url:           |
 | http://www.zephir-lang.com/license                                   |
 |                                                                      |
 | If you did not receive a copy of the Zephir license and are unable   |
 | to obtain it through the world-wide-web, please send a note to       |
 | license@zephir-lang.com so we can mail you a copy immediately.       |
 +----------------------------------------------------------------------+
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
