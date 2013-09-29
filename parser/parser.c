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
#define YYNSTATE 649
#define YYNRULE 305
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
 /*     0 */   198,  201,  204,    3,    4,    5,    6,    7,  240,  955,
 /*    10 */     1,    2,  648,    4,    5,    6,    7,   68,   72,   54,
 /*    20 */   194,  249,  254,  270,  191,  115,  588,  244,  117,  617,
 /*    30 */   623,  622,  606,  594,  483,  277,  589,  616,  572,  107,
 /*    40 */   256,  628,  234,  102,  113,   35,   36,   37,   38,   39,
 /*    50 */    40,   41,   42,   43,   44,   45,  232,  228,  230,  193,
 /*    60 */   203,  208,  210,  212,  214,  206,  200,  216,  218,  226,
 /*    70 */   220,  222,  224,  220,  222,  224,  258,  264,  249,  349,
 /*    80 */   409,  296,  245,  485,  477,  150,  257,  259,  260,  261,
 /*    90 */   262,  263,  277,  517,  487,   50,  305,   30,   31,  610,
 /*   100 */   542,  936,  441,  450,  459,  462,  453,  456,  465,  471,
 /*   110 */   468,  102,  474,  151,  153,  155,  293,  164,  267,  249,
 /*   120 */   481,  175,  179,  184,  341,  477,  150,  348,  482,   65,
 /*   130 */   612,  331,   47,  277,  528,  487,   68,   72,   76,  410,
 /*   140 */   425,  429,  438,  441,  450,  459,  462,  453,  456,  465,
 /*   150 */   471,  468,  613,  474,  151,  153,  155,  618,  164,  621,
 /*   160 */   622,  606,  175,  179,  184,  341,  571,  572,  348,  332,
 /*   170 */   249,  333,  102,  296,  245,  304,  477,  150,   60,  334,
 /*   180 */   410,  425,  429,  438,  277,  532,  487,  446,   58,   59,
 /*   190 */    61,   62,   64,   63,  441,  450,  459,  462,  453,  456,
 /*   200 */   465,  471,  468,  197,  474,  151,  153,  155,  293,  164,
 /*   210 */   267,  249,  248,  175,  179,  184,  341,  477,  150,  348,
 /*   220 */   284,  286,  285,  253,  169,  277,  540,  487,  274,  287,
 /*   230 */   354,  410,  425,  429,  438,  441,  450,  459,  462,  453,
 /*   240 */   456,  465,  471,  468,  276,  474,  151,  153,  155,  171,
 /*   250 */   164,  512,  442,  449,  175,  179,  184,  341,  296,  480,
 /*   260 */   348,  615,  249,  355,  356,  357,  358,  359,  477,  150,
 /*   270 */   313,  632,  410,  425,  429,  438,  277,  538,  487,  216,
 /*   280 */   218,  226,  220,  222,  224,  598,  441,  450,  459,  462,
 /*   290 */   453,  456,  465,  471,  468,  478,  474,  151,  153,  155,
 /*   300 */   624,  164,   69,  249,  248,  175,  179,  184,  341,  477,
 /*   310 */   150,  348,  284,  286,  285,  253,  247,  277,  506,  487,
 /*   320 */   281,  287,  597,  410,  425,  429,  438,  441,  450,  459,
 /*   330 */   462,  453,  456,  465,  471,  468,  113,  474,  151,  153,
 /*   340 */   155,  649,  164,  619,  584,  606,  175,  179,  184,  341,
 /*   350 */   167,  572,  348,  635,  249,  638,  102,  626,  113,  110,
 /*   360 */   477,  150,  113,  192,  410,  425,  429,  438,  277,  534,
 /*   370 */   487,  284,  286,  285,  253,  171,  494,  512,  441,  450,
 /*   380 */   459,  462,  453,  456,  465,  471,  468,  645,  474,  151,
 /*   390 */   153,  155,  113,  164,  625,  249,  248,  175,  179,  184,
 /*   400 */   341,  477,  150,  348,  284,  286,  285,  253,  365,  277,
 /*   410 */   526,  487,  312,  287,   89,  410,  425,  429,  438,  441,
 /*   420 */   450,  459,  462,  453,  456,  465,  471,  468,  276,  474,
 /*   430 */   151,  153,  155,  116,  164,  570,   95,  120,  175,  179,
 /*   440 */   184,  341,  571,  572,  348,  343,  249,  629,  102,  627,
 /*   450 */   351,   92,  477,  150,  320,  213,  410,  425,  429,  438,
 /*   460 */   277,  126,  487,  284,  286,  285,  253,  596,  603,  185,
 /*   470 */   441,  450,  459,  462,  453,  456,  465,  471,  468,  350,
 /*   480 */   474,  151,  153,  155,  639,  164,  627,  249,  248,  175,
 /*   490 */   179,  184,  341,  477,  150,  348,  284,  286,  285,  253,
 /*   500 */   444,  277,  554,  487,  297,  287,  172,  410,  425,  429,
 /*   510 */   438,  441,  450,  459,  462,  453,  456,  465,  471,  468,
 /*   520 */    53,  474,  151,  153,  155,   28,  164,   30,   31,  443,
 /*   530 */   175,  179,  184,  341,   77,   53,  348,  557,  249,  451,
 /*   540 */   449,   68,   72,   76,  477,  150,   48,  344,  410,  425,
 /*   550 */   429,  438,  277,  511,  487,  284,  286,  285,  253,  122,
 /*   560 */   586,   97,  441,  450,  459,  462,  453,  456,  465,  471,
 /*   570 */   468,  102,  474,  151,  153,  155,  111,  164,  627,  249,
 /*   580 */   248,  175,  179,  184,  341,  477,  150,  348,  284,  286,
 /*   590 */   285,  253,  938,  277,  522,  487,  319,  287,  431,  410,
 /*   600 */   425,  429,  438,  441,  450,  459,  462,  453,  456,  465,
 /*   610 */   471,  468,  276,  474,  151,  153,  155,  123,  164,  591,
 /*   620 */    22,  335,  175,  179,  184,  341,  594,   21,  348,  529,
 /*   630 */   249,  102,  434,   68,   72,   76,  477,  150,  275,  330,
 /*   640 */   410,  425,  429,  438,  277,  550,  487,  284,  286,  285,
 /*   650 */   253,  168,  515,  600,  441,  450,  459,  462,  453,  456,
 /*   660 */   465,  471,  468,  601,  474,  151,  153,  155,  112,  164,
 /*   670 */   337,  249,  248,  175,  179,  184,  341,  477,  150,  348,
 /*   680 */   284,  286,  285,  253,   57,  277,  128,  487,  325,  287,
 /*   690 */   444,  410,  425,  429,  438,  441,  450,  459,  462,  453,
 /*   700 */   456,  465,  471,  468,  276,  474,  151,  153,  155,  524,
 /*   710 */   164,   30,   31,  316,  175,  179,  184,  341,  269,  452,
 /*   720 */   348,   96,  249,  454,  449,   68,   72,   76,  477,  150,
 /*   730 */   290,  502,  410,  425,  429,  438,  277,  521,  487,  284,
 /*   740 */   286,  285,  253,  479,   87,  101,  441,  450,  459,  462,
 /*   750 */   453,  456,  465,  471,  468,  102,  474,  151,  153,  155,
 /*   760 */    98,  164,  322,  249,  248,  175,  179,  184,  341,  477,
 /*   770 */   150,  348,  284,  286,  285,  253,  369,  277,  548,  487,
 /*   780 */   289,  287,  250,  410,  425,  429,  438,  441,  450,  459,
 /*   790 */   462,  453,  456,  465,  471,  468,  937,  474,  151,  153,
 /*   800 */   155,  564,  164,   30,   31,  120,  175,  179,  184,  341,
 /*   810 */   547,  119,  348,  372,  249,  611,  102,  640,  551,  484,
 /*   820 */   477,  150,  594,  227,  410,  425,  429,  438,  277,  178,
 /*   830 */   487,  284,  286,  285,  253,  373,  457,  449,  441,  450,
 /*   840 */   459,  462,  453,  456,  465,  471,  468,   14,  474,  151,
 /*   850 */   153,  155,   17,  164,  251,  249,  248,  175,  179,  184,
 /*   860 */   341,  477,  150,  348,  284,  286,  285,  253,  444,  277,
 /*   870 */   493,  487,  318,  288,  444,  410,  425,  429,  438,  441,
 /*   880 */   450,  459,  462,  453,  456,  465,  471,  468,   53,  474,
 /*   890 */   151,  153,  155,  552,  164,   30,   31,  455,  175,  179,
 /*   900 */   184,  341,  486,  458,  348,  324,  249,   93,  196,   30,
 /*   910 */    31,  379,  477,  150,  558,  229,  410,  425,  429,  438,
 /*   920 */   277,  163,  487,  284,  286,  285,  253,  252,  460,  449,
 /*   930 */   441,  450,  459,  462,  453,  456,  465,  471,  468,   56,
 /*   940 */   474,  151,  153,  155,  633,  164,  380,  249,  255,  175,
 /*   950 */   179,  184,  341,  477,  150,  348,  284,  286,  285,  253,
 /*   960 */   383,  277,  183,  487,  463,  449,  173,  410,  425,  429,
 /*   970 */   438,  441,  450,  459,  462,  453,  456,  465,  471,  468,
 /*   980 */   559,  474,  151,  153,  155,  600,  164,  268,  563,  246,
 /*   990 */   175,  179,  184,  341,  595,  601,  348,  315,  249,  536,
 /*  1000 */   292,   30,   31,  308,  477,  150,  381,  329,  410,  425,
 /*  1010 */   429,  438,  277,  516,  487,  284,  286,  285,  253,  495,
 /*  1020 */   466,  449,  441,  450,  459,  462,  453,  456,  465,  471,
 /*  1030 */   468,  276,  474,  151,  153,  155,   67,  164,  642,  249,
 /*  1040 */   497,  175,  179,  184,  341,  477,  150,  348,  284,  286,
 /*  1050 */   285,  253,  444,  277,  500,  487,  496,  326,  444,  410,
 /*  1060 */   425,  429,  438,  441,  450,  459,  462,  453,  456,  465,
 /*  1070 */   471,  468,  364,  474,  151,  153,  155,   46,  164,  608,
 /*  1080 */   378,  461,  175,  179,  184,  341,  594,  464,  348,  609,
 /*  1090 */   249,   99,   27,   30,   31,  636,  477,  150,  384,  215,
 /*  1100 */   410,  425,  429,  438,  277,  347,  487,  284,  286,  285,
 /*  1110 */   253,  385,  469,  449,  441,  450,  459,  462,  453,  456,
 /*  1120 */   465,  471,  468,   53,  474,  151,  153,  155,  643,  164,
 /*  1130 */   188,  249,  490,  175,  179,  184,  341,  477,  150,  348,
 /*  1140 */   284,  286,  285,  253,  444,  277,  510,  487,  109,  530,
 /*  1150 */   388,  410,  425,  429,  438,  441,  450,  459,  462,  453,
 /*  1160 */   456,  465,  471,  468,  531,  474,  151,  153,  155,  389,
 /*  1170 */   164,  105,  535,  467,  175,  179,  184,  341,  507,  276,
 /*  1180 */   348,  644,  249,  472,  449,  509,  444,  393,  477,  150,
 /*  1190 */   317,  221,  410,  425,  429,  438,  277,  568,  487,  284,
 /*  1200 */   286,  285,  253,  190,  396,  298,  441,  450,  459,  462,
 /*  1210 */   453,  456,  465,  471,  468,  470,  474,  151,  153,  155,
 /*  1220 */   513,  164,  106,  249,  202,  175,  179,  184,  341,  477,
 /*  1230 */   150,  348,  284,  286,  285,  253,  444,  277,  159,  487,
 /*  1240 */   397,   52,  444,  410,  425,  429,  438,  441,  450,  459,
 /*  1250 */   462,  453,  456,  465,  471,  468,   49,  474,  151,  153,
 /*  1260 */   155,   34,  164,  475,  449,  473,  175,  179,  184,  341,
 /*  1270 */   125,  476,  348,   33,  249,  646,   66,  176,  523,  518,
 /*  1280 */   477,  150,  519,  239,  410,  425,  429,  438,  277,  566,
 /*  1290 */   487,  284,  286,  285,  253,  630,  113,   51,  441,  450,
 /*  1300 */   459,  462,  453,  456,  465,  471,  468,  276,  474,  151,
 /*  1310 */   153,  155,  401,  164,  647,  249,  217,  175,  179,  184,
 /*  1320 */   341,  477,  150,  348,  284,  286,  285,  253,  413,  277,
 /*  1330 */   505,  487,  166,  282,  404,  410,  425,  429,  438,  441,
 /*  1340 */   450,  459,  462,  453,  456,  465,  471,  468,   75,  474,
 /*  1350 */   151,  153,  155,  323,  164,  405,  525,  412,  175,  179,
 /*  1360 */   184,  341,  160,   29,  348,   74,  249,   25,  537,  271,
 /*  1370 */    24,   73,  477,  150,  272,  339,  410,  425,  429,  438,
 /*  1380 */   277,  562,  487,  284,  286,  285,  253,  544,  545,  418,
 /*  1390 */   441,  450,  459,  462,  453,  456,  465,  471,  468,  420,
 /*  1400 */   474,  151,  153,  155,  195,  164,  336,  249,  435,  175,
 /*  1410 */   179,  184,  341,  477,  150,  348,  284,  286,  285,  253,
 /*  1420 */   422,  277,  187,  487,  161,  553,  424,  410,  425,  429,
 /*  1430 */   438,  441,  450,  459,  462,  453,  456,  465,  471,  468,
 /*  1440 */    16,  474,  151,  153,  155,  338,  164,   17,  278,  170,
 /*  1450 */   175,  179,  184,  341,  637,  279,  348,   15,  249,  280,
 /*  1460 */   342,  181,  430,   13,  477,  150,  565,  205,  410,  425,
 /*  1470 */   429,  438,  277,  560,  487,  284,  286,  285,  253,   71,
 /*  1480 */    12,  432,  441,  450,  459,  462,  453,  456,  465,  471,
 /*  1490 */   468,  433,  474,  151,  153,  155,   94,  164,   10,  249,
 /*  1500 */   294,  175,  179,  184,  341,  477,  150,  348,  284,  286,
 /*  1510 */   285,  253,  157,  277,  556,  487,  634,  437,  576,  410,
 /*  1520 */   425,  429,  438,  441,  450,  459,  462,  453,  456,  465,
 /*  1530 */   471,  468,  575,  474,  151,  153,  155,  154,  164,    9,
 /*  1540 */   577,  578,  175,  179,  184,  341,  353,  580,  348,  581,
 /*  1550 */   249,  238,  582,  583,  352,  604,  477,  150,  445,  328,
 /*  1560 */   410,  425,  429,  438,  277,  108,  487,  284,  286,  285,
 /*  1570 */   253,  590,  237,  592,  441,  450,  459,  462,  453,  456,
 /*  1580 */   465,  471,  468,  447,  474,  151,  153,  155,  593,  164,
 /*  1590 */   148,  360,  695,  175,  179,  184,  341,  448,  235,  348,
 /*  1600 */   241,   78,   79,   81,   80,   82,  284,  286,  285,  253,
 /*  1610 */   346,  410,  425,  429,  438,  696,  488,  130,  131,  132,
 /*  1620 */   133,  134,  135,  136,  137,  138,  139,  140,  141,  142,
 /*  1630 */   143,  144,  145,  146,  147,  149,  148,  100,  599,  242,
 /*  1640 */    84,   85,  602,  676,  585,   90,  579,  417,  419,  421,
 /*  1650 */   423,   83,  605,  152,  614,   70,  186,  484,  484,  484,
 /*  1660 */   484,  484,  488,  130,  131,  132,  133,  134,  135,  136,
 /*  1670 */   137,  138,  139,  140,  141,  142,  143,  144,  145,  146,
 /*  1680 */   147,  149,  148,  484,  484,  484,  484,  484,  484,  484,
 /*  1690 */   484,  484,  484,  417,  419,  421,  423,   78,   79,   81,
 /*  1700 */    80,   82,  127,  484,  484,  484,  484,  484,  488,  130,
 /*  1710 */   131,  132,  133,  134,  135,  136,  137,  138,  139,  140,
 /*  1720 */   141,  142,  143,  144,  145,  146,  147,  149,  148,  484,
 /*  1730 */   607,  484,  484,   19,  484,  484,   84,   85,   22,  417,
 /*  1740 */   419,  421,  423,  354,  121,  103,  484,   83,  177,  102,
 /*  1750 */   484,  484,  484,  484,  488,  130,  131,  132,  133,  134,
 /*  1760 */   135,  136,  137,  138,  139,  140,  141,  142,  143,  144,
 /*  1770 */   145,  146,  147,  149,  148,  484,  355,  356,  357,  358,
 /*  1780 */   359,  484,  391,  484,  484,  417,  419,  421,  423,   78,
 /*  1790 */    79,   81,   80,   82,  567,  484,  484,  484,  484,  484,
 /*  1800 */   488,  130,  131,  132,  133,  134,  135,  136,  137,  138,
 /*  1810 */   139,  140,  141,  142,  143,  144,  145,  146,  147,  149,
 /*  1820 */   148,  484,  587,  484,  484,  484,  484,  484,   84,   85,
 /*  1830 */   484,  417,  419,  421,  423,  484,  543,  484,  484,   83,
 /*  1840 */   561,  484,  484,  484,  484,  484,  488,  130,  131,  132,
 /*  1850 */   133,  134,  135,  136,  137,  138,  139,  140,  141,  142,
 /*  1860 */   143,  144,  145,  146,  147,  149,  148,  484,  484,  484,
 /*  1870 */   484,  484,  484,  484,  484,  484,  484,  417,  419,  421,
 /*  1880 */   423,   78,   79,   81,   80,   82,  555,  484,  484,  484,
 /*  1890 */   484,  484,  488,  130,  131,  132,  133,  134,  135,  136,
 /*  1900 */   137,  138,  139,  140,  141,  142,  143,  144,  145,  146,
 /*  1910 */   147,  149,  148,  484,  484,  484,  484,  484,  484,  484,
 /*  1920 */    84,   85,  484,  417,  419,  421,  423,  484,   88,  484,
 /*  1930 */   484,   83,  549,  484,  484,  484,  484,  484,  488,  130,
 /*  1940 */   131,  132,  133,  134,  135,  136,  137,  138,  139,  140,
 /*  1950 */   141,  142,  143,  144,  145,  146,  147,  149,  148,  484,
 /*  1960 */   484,  484,  484,  484,  484,  484,  484,  484,  484,  417,
 /*  1970 */   419,  421,  423,   78,   79,   81,   80,   82,  539,  484,
 /*  1980 */   484,  484,  484,  484,  488,  130,  131,  132,  133,  134,
 /*  1990 */   135,  136,  137,  138,  139,  140,  141,  142,  143,  144,
 /*  2000 */   145,  146,  147,  149,  148,  484,  484,  484,  484,  484,
 /*  2010 */   484,  484,   84,   85,  484,  417,  419,  421,  423,  484,
 /*  2020 */   543,  484,  484,   83,  174,  484,  484,  484,  484,  484,
 /*  2030 */   488,  130,  131,  132,  133,  134,  135,  136,  137,  138,
 /*  2040 */   139,  140,  141,  142,  143,  144,  145,  146,  147,  149,
 /*  2050 */   148,  484,  484,  484,  484,  484,  484,  484,  484,  484,
 /*  2060 */   484,  417,  419,  421,  423,   78,   79,   81,   80,   82,
 /*  2070 */   533,  484,  484,  484,  484,  484,  488,  130,  131,  132,
 /*  2080 */   133,  134,  135,  136,  137,  138,  139,  140,  141,  142,
 /*  2090 */   143,  144,  145,  146,  147,  149,  148,  484,  484,  484,
 /*  2100 */   484,  484,  484,  484,   84,   85,  484,  417,  419,  421,
 /*  2110 */   423,  484,   23,  484,  484,   83,  527,  484,  484,  484,
 /*  2120 */   484,  484,  488,  130,  131,  132,  133,  134,  135,  136,
 /*  2130 */   137,  138,  139,  140,  141,  142,  143,  144,  145,  146,
 /*  2140 */   147,  149,  148,  484,  484,  484,  484,  484,  484,  484,
 /*  2150 */   484,  484,  484,  417,  419,  421,  423,   78,   79,   81,
 /*  2160 */    80,   82,  520,  484,  484,  484,  484,  484,  488,  130,
 /*  2170 */   131,  132,  133,  134,  135,  136,  137,  138,  139,  140,
 /*  2180 */   141,  142,  143,  144,  145,  146,  147,  149,  148,  484,
 /*  2190 */   484,  484,  484,  484,  484,  484,   84,   85,  484,  417,
 /*  2200 */   419,  421,  423,  484,  121,  484,  484,   83,  514,  484,
 /*  2210 */   484,  484,  484,  484,  488,  130,  131,  132,  133,  134,
 /*  2220 */   135,  136,  137,  138,  139,  140,  141,  142,  143,  144,
 /*  2230 */   145,  146,  147,  149,  148,  484,  484,  484,  484,  484,
 /*  2240 */   484,  484,  484,  484,  484,  417,  419,  421,  423,   78,
 /*  2250 */    79,   81,   80,   82,  182,  484,  484,  484,  484,  484,
 /*  2260 */   488,  130,  131,  132,  133,  134,  135,  136,  137,  138,
 /*  2270 */   139,  140,  141,  142,  143,  144,  145,  146,  147,  149,
 /*  2280 */   148,  484,  484,  484,  156,  484,  484,  340,   84,   85,
 /*  2290 */   484,  417,  419,  421,  423,  284,  286,  285,  253,   83,
 /*  2300 */   504,  484,  484,  484,  484,  484,  488,  130,  131,  132,
 /*  2310 */   133,  134,  135,  136,  137,  138,  139,  140,  141,  142,
 /*  2320 */   143,  144,  145,  146,  147,  149,  148,  484,  484,  484,
 /*  2330 */   484,  484,  484,  569,  118,  584,  606,  417,  419,  421,
 /*  2340 */   423,  616,  572,  484,  484,  484,  499,  102,  484,  484,
 /*  2350 */   484,  484,  488,  130,  131,  132,  133,  134,  135,  136,
 /*  2360 */   137,  138,  139,  140,  141,  142,  143,  144,  145,  146,
 /*  2370 */   147,  149,  148,  484,  484,  484,  165,  484,  484,  340,
 /*  2380 */   484,  484,  484,  417,  419,  421,  423,  284,  286,  285,
 /*  2390 */   253,  484,  492,  484,  484,  484,  484,  484,  488,  130,
 /*  2400 */   131,  132,  133,  134,  135,  136,  137,  138,  139,  140,
 /*  2410 */   141,  142,  143,  144,  145,  146,  147,  149,  148,  484,
 /*  2420 */   484,  484,  180,  484,  484,  340,  484,  484,  484,  417,
 /*  2430 */   419,  421,  423,  284,  286,  285,  253,  484,  158,  484,
 /*  2440 */   484,  484,  484,  484,  488,  130,  131,  132,  133,  134,
 /*  2450 */   135,  136,  137,  138,  139,  140,  141,  142,  143,  144,
 /*  2460 */   145,  146,  147,  149,  148,  484,  484,  484,  189,  484,
 /*  2470 */   484,  340,  484,  484,  484,  417,  419,  421,  423,  284,
 /*  2480 */   286,  285,  253,  484,  162,  484,  484,  484,  484,  484,
 /*  2490 */   488,  130,  131,  132,  133,  134,  135,  136,  137,  138,
 /*  2500 */   139,  140,  141,  142,  143,  144,  145,  146,  147,  149,
 /*  2510 */   148,  484,  484,  484,  484,  484,  484,  484,  484,  415,
 /*  2520 */   484,  417,  419,  421,  423,  411,  416,  284,  286,  285,
 /*  2530 */   253,  484,  484,  484,  484,  484,  129,  130,  131,  132,
 /*  2540 */   133,  134,  135,  136,  137,  138,  139,  140,  141,  142,
 /*  2550 */   143,  144,  145,  146,  147,  149,  198,  201,  204,  484,
 /*  2560 */   508,  484,  484,  340,  240,  484,  484,  417,  419,  421,
 /*  2570 */   423,  284,  286,  285,  253,  484,  194,  249,  254,  270,
 /*  2580 */   191,  283,  484,  244,  198,  201,  204,  650,  484,  484,
 /*  2590 */   484,  277,  240,  484,  484,  484,  256,  484,  234,  484,
 /*  2600 */   484,  484,  484,  484,  194,  249,  254,  270,  191,  314,
 /*  2610 */   484,  244,  484,  484,  198,  201,  204,  484,  484,  277,
 /*  2620 */     8,  484,  240,   11,  256,  104,  234,  631,  641,  484,
 /*  2630 */   484,  487,  258,  264,  194,  249,  254,  270,  191,  484,
 /*  2640 */    32,  244,  257,  259,  260,  261,  262,  263,  484,  277,
 /*  2650 */   484,  484,  484,  484,  256,  484,  234,  124,  362,   55,
 /*  2660 */   258,  264,  361,  484,  484,  484,  284,  286,  285,  253,
 /*  2670 */   257,  259,  260,  261,  262,  263,  484,   35,   36,   37,
 /*  2680 */    38,   39,   40,   41,   42,   43,   44,   45,  484,  484,
 /*  2690 */   258,  264,  392,  484,  484,  484,  484,  484,  484,  484,
 /*  2700 */   257,  259,  260,  261,  262,  263,  484,  484,  198,  201,
 /*  2710 */   204,  484,    8,  484,  484,   11,  240,  104,  484,  631,
 /*  2720 */   641,  484,  484,  487,  484,  484,  484,  484,  194,  249,
 /*  2730 */   254,  270,  191,  291,  484,  244,  198,  201,  204,  484,
 /*  2740 */   484,  484,  484,  277,  240,  484,  484,  484,  256,  484,
 /*  2750 */   234,  484,  484,  484,  484,  484,  194,  249,  254,  270,
 /*  2760 */   191,  484,  484,  244,  484,  484,  198,  201,  204,  484,
 /*  2770 */   484,  277,  375,  484,  240,  484,  256,  371,  234,  484,
 /*  2780 */   284,  286,  285,  253,  258,  264,  194,  249,  254,  270,
 /*  2790 */   191,  484,   32,  244,  257,  259,  260,  261,  262,  263,
 /*  2800 */   484,  277,  484,  484,  484,  484,  256,  484,  234,  546,
 /*  2810 */   484,   55,  258,  264,  400,  484,  484,  484,  484,  484,
 /*  2820 */   484,  484,  257,  259,  260,  261,  262,  263,  484,   35,
 /*  2830 */    36,   37,   38,   39,   40,   41,   42,   43,   44,   45,
 /*  2840 */   375,  484,  258,  264,  368,  395,  484,  484,  284,  286,
 /*  2850 */   285,  253,  257,  259,  260,  261,  262,  263,  484,  484,
 /*  2860 */   198,  201,  204,  484,  484,  484,  265,  484,  240,  484,
 /*  2870 */   484,  484,  484,  484,  284,  286,  285,  253,  484,  484,
 /*  2880 */   194,  249,  254,  270,  191,  299,  309,  244,  198,  201,
 /*  2890 */   204,  484,  484,  484,  484,  277,  240,  484,  484,  484,
 /*  2900 */   256,  484,  234,  484,  484,  484,  484,  484,  194,  249,
 /*  2910 */   254,  270,  191,  484,  484,  244,  484,  484,  198,  201,
 /*  2920 */   204,  484,  484,  277,  375,  484,  240,  484,  256,  403,
 /*  2930 */   234,  484,  284,  286,  285,  253,  258,  264,  194,  249,
 /*  2940 */   254,  270,  191,  273,  484,  244,  257,  259,  260,  261,
 /*  2950 */   262,  263,  484,  277,  484,  484,  484,  501,  256,  484,
 /*  2960 */   234,  198,  201,  204,  258,  264,  484,  484,  484,  240,
 /*  2970 */   484,  484,  484,  484,  257,  259,  260,  261,  262,  263,
 /*  2980 */   484,  194,  249,  254,  270,  191,  484,  484,  244,  484,
 /*  2990 */   484,  198,  201,  204,  258,  264,  277,  484,  484,  240,
 /*  3000 */   484,  256,  484,  234,  257,  259,  260,  261,  262,  263,
 /*  3010 */   484,  194,  249,  254,  270,  191,  484,  484,  266,  198,
 /*  3020 */   201,  204,  484,  484,  484,  484,  277,  240,  484,  484,
 /*  3030 */   489,  256,  484,  234,  484,  484,  484,  258,  264,  194,
 /*  3040 */   249,  254,  270,  191,  321,  484,  244,  257,  259,  260,
 /*  3050 */   261,  262,  263,  484,  277,  354,  484,  484,  484,  256,
 /*  3060 */   484,  234,  484,  484,  484,  415,  484,  301,  264,  302,
 /*  3070 */   484,  484,  414,  284,  286,  285,  253,  300,  259,  260,
 /*  3080 */   261,  262,  263,  484,  484,   32,  484,  484,  355,  356,
 /*  3090 */   357,  358,  359,  484,  367,  258,  264,  376,  377,  484,
 /*  3100 */   484,  484,   26,  484,   55,  257,  259,  260,  261,  262,
 /*  3110 */   263,  484,  484,  198,  201,  204,  484,  484,  484,  484,
 /*  3120 */   484,  240,   35,   36,   37,   38,   39,   40,   41,   42,
 /*  3130 */    43,   44,   45,  194,  249,  254,  270,  191,  327,  484,
 /*  3140 */   244,  198,  201,  204,  484,  484,  484,  484,  277,  240,
 /*  3150 */   484,  484,  484,  256,  484,  234,  484,  484,  484,  484,
 /*  3160 */   484,  194,  249,  254,  270,  191,  484,  484,  244,  428,
 /*  3170 */   484,  198,  201,  204,  484,  484,  277,  426,  484,  240,
 /*  3180 */   484,  256,  484,  234,  484,  284,  286,  285,  253,  258,
 /*  3190 */   264,  194,  249,  254,  270,  191,  484,  484,  244,  257,
 /*  3200 */   259,  260,  261,  262,  263,  484,  277,  484,  484,  484,
 /*  3210 */   484,  256,  484,  234,  198,  201,  204,  258,  264,  484,
 /*  3220 */   484,  484,  240,  484,  484,  484,  484,  257,  259,  260,
 /*  3230 */   261,  262,  263,  484,  194,  249,  254,  270,  191,  484,
 /*  3240 */   484,  266,  362,  484,  484,  484,  366,  258,  264,  277,
 /*  3250 */   284,  286,  285,  253,  256,  484,  234,  257,  259,  260,
 /*  3260 */   261,  262,  263,  484,  484,  232,  228,  230,  193,  203,
 /*  3270 */   208,  210,  212,  214,  206,  200,  216,  218,  226,  220,
 /*  3280 */   222,  224,  484,  484,  484,  484,  484,  484,  484,  484,
 /*  3290 */   301,  264,  484,  354,  484,  484,  484,  491,  363,  484,
 /*  3300 */   300,  259,  260,  261,  262,  263,  484,  484,  232,  228,
 /*  3310 */   230,  193,  203,  208,  210,  212,  214,  206,  200,  216,
 /*  3320 */   218,  226,  220,  222,  224,  484,  355,  356,  357,  358,
 /*  3330 */   359,   32,  399,  362,  387,  407,  408,  370,  484,  484,
 /*  3340 */   498,  284,  286,  285,  253,  484,  484,  484,   91,  484,
 /*  3350 */    55,  232,  228,  230,  193,  203,  208,  210,  212,  214,
 /*  3360 */   206,  200,  216,  218,  226,  220,  222,  224,   35,   36,
 /*  3370 */    37,   38,   39,   40,   41,   42,   43,   44,   45,  223,
 /*  3380 */   484,  484,  484,  503,  484,  484,  484,  284,  286,  285,
 /*  3390 */   253,  484,  484,  484,  232,  228,  230,  193,  203,  208,
 /*  3400 */   210,  212,  214,  206,  200,  216,  218,  226,  220,  222,
 /*  3410 */   224,  232,  228,  230,  193,  203,  208,  210,  212,  214,
 /*  3420 */   206,  200,  216,  218,  226,  220,  222,  224,  484,  484,
 /*  3430 */   484,  484,  484,  484,  484,  484,  440,  232,  228,  230,
 /*  3440 */   193,  203,  208,  210,  212,  214,  206,  200,  216,  218,
 /*  3450 */   226,  220,  222,  224,  193,  203,  208,  210,  212,  214,
 /*  3460 */   206,  200,  216,  218,  226,  220,  222,  224,  362,  837,
 /*  3470 */   484,  484,  374,  484,  484,  484,  284,  286,  285,  253,
 /*  3480 */   232,  228,  230,  193,  203,  208,  210,  212,  214,  206,
 /*  3490 */   200,  216,  218,  226,  220,  222,  224,  484,  362,  484,
 /*  3500 */   484,  484,  382,   32,  484,  427,  284,  286,  285,  253,
 /*  3510 */   362,  484,  436,  484,  386,  484,  484,  484,  284,  286,
 /*  3520 */   285,  253,   55,  232,  228,  230,  193,  203,  208,  210,
 /*  3530 */   212,  214,  206,  200,  216,  218,  226,  220,  222,  224,
 /*  3540 */    35,   36,   37,   38,   39,   40,   41,   42,   43,   44,
 /*  3550 */    45,  484,  362,  484,  484,  345,  390,  484,  484,  484,
 /*  3560 */   284,  286,  285,  253,  484,  484,  232,  228,  230,  193,
 /*  3570 */   203,  208,  210,  212,  214,  206,  200,  216,  218,  226,
 /*  3580 */   220,  222,  224,  232,  228,  230,  193,  203,  208,  210,
 /*  3590 */   212,  214,  206,  200,  216,  218,  226,  220,  222,  224,
 /*  3600 */   484,  484,  484,  484,  484,  236,  232,  228,  230,  193,
 /*  3610 */   203,  208,  210,  212,  214,  206,  200,  216,  218,  226,
 /*  3620 */   220,  222,  224,  228,  230,  193,  203,  208,  210,  212,
 /*  3630 */   214,  206,  200,  216,  218,  226,  220,  222,  224,   32,
 /*  3640 */   265,  295,  484,  484,  484,  484,  484,  484,  284,  286,
 /*  3650 */   285,  253,  484,  265,  484,  303,  484,  484,  311,  307,
 /*  3660 */   310,  284,  286,  285,  253,  484,  484,  484,  484,  484,
 /*  3670 */   484,  306,  307,  310,  484,  484,   35,   36,   37,   38,
 /*  3680 */    39,   40,   41,   42,   43,   44,   45,   78,   79,   81,
 /*  3690 */    80,   82,  362,  484,  484,  484,  394,  484,  484,  484,
 /*  3700 */   284,  286,  285,  253,  362,  484,  484,  484,  398,  439,
 /*  3710 */   484,  484,  284,  286,  285,  253,  484,  284,  286,  285,
 /*  3720 */   253,  484,  484,  484,  484,  225,   84,   85,  484,  114,
 /*  3730 */   585,  484,  579,  284,  286,  285,  253,   83,  484,   78,
 /*  3740 */    79,   81,   80,   82,  362,  484,  484,  484,  402,  484,
 /*  3750 */   484,  484,  284,  286,  285,  253,  362,  484,  484,  484,
 /*  3760 */   406,  484,  211,  484,  284,  286,  285,  253,  484,  484,
 /*  3770 */   284,  286,  285,  253,  484,  484,  484,  219,   84,   85,
 /*  3780 */   484,  680,  573,  484,  579,  284,  286,  285,  253,   83,
 /*  3790 */   209,   78,   79,   81,   80,   82,  243,  484,  284,  286,
 /*  3800 */   285,  253,  484,  484,  284,  286,  285,  253,  207,  484,
 /*  3810 */   484,  484,  199,  484,  484,  484,  284,  286,  285,  253,
 /*  3820 */   284,  286,  285,  253,  484,  484,  484,  484,  484,  231,
 /*  3830 */    84,   85,  484,  677,  585,  484,  579,  284,  286,  285,
 /*  3840 */   253,   83,  484,   78,   79,   81,   80,   82,   78,   79,
 /*  3850 */    81,   80,   82,  233,  484,  484,  484,  484,  484,  484,
 /*  3860 */   484,  284,  286,  285,  253,  484,  484,  484,  484,  484,
 /*  3870 */   484,  484,  484,  484,  484,  484,  484,  484,  484,  484,
 /*  3880 */   484,  484,   84,   85,  484,  679,  541,   84,   85,  484,
 /*  3890 */   683,  541,  484,   83,  484,  484,  484,  484,   83,  484,
 /*  3900 */    78,   79,   81,   80,   82,   78,   79,   81,   80,   82,
 /*  3910 */   484,  484,  484,  484,  484,  484,  484,  484,  484,  484,
 /*  3920 */   484,  484,  484,  484,  484,  484,  484,  484,  484,  484,
 /*  3930 */   484,  484,  484,  484,  484,  484,  484,  484,  484,   84,
 /*  3940 */    85,  484,   20,   86,   84,   85,  484,  682,  541,  484,
 /*  3950 */    83,  484,  484,  484,  484,   83,  484,   78,   79,   81,
 /*  3960 */    80,   82,   78,   79,   81,   80,   82,  484,  484,  484,
 /*  3970 */   484,  484,  484,  484,  484,  484,  484,  484,  484,  484,
 /*  3980 */   484,  484,  484,  484,  484,  484,  484,  484,  484,  484,
 /*  3990 */   484,  484,  484,  484,  484,  484,   84,   85,  484,  678,
 /*  4000 */   541,   84,   85,  484,   18,   86,  484,   83,  484,  484,
 /*  4010 */   484,  484,   83,  484,   78,   79,   81,   80,   82,   78,
 /*  4020 */    79,   81,   80,   82,  484,  484,  484,  484,  484,  484,
 /*  4030 */   484,  484,  484,  484,  484,  484,  484,  484,  484,  484,
 /*  4040 */   484,  484,  484,  484,  484,  484,  484,  484,  484,  484,
 /*  4050 */   484,  484,  484,   84,   85,  484,  681,  620,   84,   85,
 /*  4060 */   484,  684,  541,  484,   83,  484,  484,  484,  484,   83,
 /*  4070 */   484,   78,   79,   81,   80,   82,  484,  484,  484,  484,
 /*  4080 */   484,  484,  484,  484,  484,  484,  484,  484,  484,  484,
 /*  4090 */   484,  484,  484,  484,  484,  484,  484,  484,  484,  484,
 /*  4100 */   484,  484,  484,  484,  484,  484,  484,  484,  484,  484,
 /*  4110 */    84,   85,  484,  484,  484,  484,  574,  484,  484,  484,
 /*  4120 */   484,   83,
};
static YYCODETYPE yy_lookahead[] = {
 /*     0 */     7,    8,    9,  103,  104,  105,  106,  107,   15,  100,
 /*    10 */   101,  102,  103,  104,  105,  106,  107,  130,  131,  132,
 /*    20 */    27,   28,   29,   30,   31,  111,   35,   34,  114,  115,
 /*    30 */   116,  117,  118,   42,   34,   42,   45,  123,  124,   37,
 /*    40 */    47,   39,   49,  129,   42,   52,   53,   54,   55,   56,
 /*    50 */    57,   58,   59,   60,   61,   62,   10,   11,   12,   13,
 /*    60 */    14,   15,   16,   17,   18,   19,   20,   21,   22,   23,
 /*    70 */    24,   25,   26,   24,   25,   26,   83,   84,   28,  157,
 /*    80 */   158,   49,   50,   83,   34,   35,   93,   94,   95,   96,
 /*    90 */    97,   98,   42,   43,   44,  128,    6,  130,  131,  119,
 /*   100 */   118,   69,   52,   53,   54,   55,   56,   57,   58,   59,
 /*   110 */    60,  129,   62,   63,   64,   65,   84,   67,   86,   28,
 /*   120 */    34,   71,   72,   73,   74,   34,   35,   77,   42,   34,
 /*   130 */   120,   85,  126,   42,   43,   44,  130,  131,  132,   89,
 /*   140 */    90,   91,   92,   52,   53,   54,   55,   56,   57,   58,
 /*   150 */    59,   60,   35,   62,   63,   64,   65,  114,   67,  116,
 /*   160 */   117,  118,   71,   72,   73,   74,  123,  124,   77,   50,
 /*   170 */    28,   34,  129,   49,   50,   85,   34,   35,   83,   42,
 /*   180 */    89,   90,   91,   92,   42,   43,   44,   34,   93,   94,
 /*   190 */    95,   96,   97,   98,   52,   53,   54,   55,   56,   57,
 /*   200 */    58,   59,   60,   84,   62,   63,   64,   65,   84,   67,
 /*   210 */    86,   28,  156,   71,   72,   73,   74,   34,   35,   77,
 /*   220 */   164,  165,  166,  167,   43,   42,   43,   44,  172,  173,
 /*   230 */    45,   89,   90,   91,   92,   52,   53,   54,   55,   56,
 /*   240 */    57,   58,   59,   60,    6,   62,   63,   64,   65,   68,
 /*   250 */    67,   70,  168,  169,   71,   72,   73,   74,   49,   50,
 /*   260 */    77,   35,   28,   78,   79,   80,   81,   82,   34,   35,
 /*   270 */    32,   38,   89,   90,   91,   92,   42,   43,   44,   21,
 /*   280 */    22,   23,   24,   25,   26,    6,   52,   53,   54,   55,
 /*   290 */    56,   57,   58,   59,   60,   86,   62,   63,   64,   65,
 /*   300 */     6,   67,   34,   28,  156,   71,   72,   73,   74,   34,
 /*   310 */    35,   77,  164,  165,  166,  167,   49,   42,   43,   44,
 /*   320 */   172,  173,   43,   89,   90,   91,   92,   52,   53,   54,
 /*   330 */    55,   56,   57,   58,   59,   60,   42,   62,   63,   64,
 /*   340 */    65,    0,   67,  116,  117,  118,   71,   72,   73,   74,
 /*   350 */    43,  124,   77,   37,   28,   39,  129,   34,   42,   39,
 /*   360 */    34,   35,   42,  156,   89,   90,   91,   92,   42,   43,
 /*   370 */    44,  164,  165,  166,  167,   68,    6,   70,   52,   53,
 /*   380 */    54,   55,   56,   57,   58,   59,   60,   37,   62,   63,
 /*   390 */    64,   65,   42,   67,  112,   28,  156,   71,   72,   73,
 /*   400 */    74,   34,   35,   77,  164,  165,  166,  167,  159,   42,
 /*   410 */    43,   44,  172,  173,   34,   89,   90,   91,   92,   52,
 /*   420 */    53,   54,   55,   56,   57,   58,   59,   60,    6,   62,
 /*   430 */    63,   64,   65,   43,   67,  116,   35,  118,   71,   72,
 /*   440 */    73,   74,  123,  124,   77,   75,   28,  110,  129,  112,
 /*   450 */     6,   50,   34,   35,   32,  156,   89,   90,   91,   92,
 /*   460 */    42,   43,   44,  164,  165,  166,  167,  121,  122,   42,
 /*   470 */    52,   53,   54,   55,   56,   57,   58,   59,   60,   35,
 /*   480 */    62,   63,   64,   65,  110,   67,  112,   28,  156,   71,
 /*   490 */    72,   73,   74,   34,   35,   77,  164,  165,  166,  167,
 /*   500 */     6,   42,   43,   44,  172,  173,  119,   89,   90,   91,
 /*   510 */    92,   52,   53,   54,   55,   56,   57,   58,   59,   60,
 /*   520 */     6,   62,   63,   64,   65,  128,   67,  130,  131,   35,
 /*   530 */    71,   72,   73,   74,  129,    6,   77,  126,   28,  168,
 /*   540 */   169,  130,  131,  132,   34,   35,   32,  156,   89,   90,
 /*   550 */    91,   92,   42,   43,   44,  164,  165,  166,  167,   34,
 /*   560 */   118,   32,   52,   53,   54,   55,   56,   57,   58,   59,
 /*   570 */    60,  129,   62,   63,   64,   65,  110,   67,  112,   28,
 /*   580 */   156,   71,   72,   73,   74,   34,   35,   77,  164,  165,
 /*   590 */   166,  167,   69,   42,   43,   44,  172,  173,   50,   89,
 /*   600 */    90,   91,   92,   52,   53,   54,   55,   56,   57,   58,
 /*   610 */    59,   60,    6,   62,   63,   64,   65,   49,   67,   35,
 /*   620 */   118,   34,   71,   72,   73,   74,   42,  125,   77,  126,
 /*   630 */    28,  129,   84,  130,  131,  132,   34,   35,   32,  156,
 /*   640 */    89,   90,   91,   92,   42,   43,   44,  164,  165,  166,
 /*   650 */   167,  154,  155,   34,   52,   53,   54,   55,   56,   57,
 /*   660 */    58,   59,   60,   44,   62,   63,   64,   65,  109,   67,
 /*   670 */    83,   28,  156,   71,   72,   73,   74,   34,   35,   77,
 /*   680 */   164,  165,  166,  167,  119,   42,   43,   44,  172,  173,
 /*   690 */     6,   89,   90,   91,   92,   52,   53,   54,   55,   56,
 /*   700 */    57,   58,   59,   60,    6,   62,   63,   64,   65,  128,
 /*   710 */    67,  130,  131,   34,   71,   72,   73,   74,   49,   35,
 /*   720 */    77,  126,   28,  168,  169,  130,  131,  132,   34,   35,
 /*   730 */    32,  156,   89,   90,   91,   92,   42,   43,   44,  164,
 /*   740 */   165,  166,  167,   34,  118,   35,   52,   53,   54,   55,
 /*   750 */    56,   57,   58,   59,   60,  129,   62,   63,   64,   65,
 /*   760 */    50,   67,   83,   28,  156,   71,   72,   73,   74,   34,
 /*   770 */    35,   77,  164,  165,  166,  167,  159,   42,   43,   44,
 /*   780 */   172,  173,   34,   89,   90,   91,   92,   52,   53,   54,
 /*   790 */    55,   56,   57,   58,   59,   60,   69,   62,   63,   64,
 /*   800 */    65,  128,   67,  130,  131,  118,   71,   72,   73,   74,
 /*   810 */    42,  124,   77,   85,   28,   35,  129,  109,   50,   43,
 /*   820 */    34,   35,   42,  156,   89,   90,   91,   92,   42,   43,
 /*   830 */    44,  164,  165,  166,  167,  159,  168,  169,   52,   53,
 /*   840 */    54,   55,   56,   57,   58,   59,   60,   37,   62,   63,
 /*   850 */    64,   65,   42,   67,    6,   28,  156,   71,   72,   73,
 /*   860 */    74,   34,   35,   77,  164,  165,  166,  167,    6,   42,
 /*   870 */    43,   44,   49,  173,    6,   89,   90,   91,   92,   52,
 /*   880 */    53,   54,   55,   56,   57,   58,   59,   60,    6,   62,
 /*   890 */    63,   64,   65,  128,   67,  130,  131,   35,   71,   72,
 /*   900 */    73,   74,   43,   35,   77,   49,   28,  128,   34,  130,
 /*   910 */   131,   34,   34,   35,   32,  156,   89,   90,   91,   92,
 /*   920 */    42,   43,   44,  164,  165,  166,  167,  170,  168,  169,
 /*   930 */    52,   53,   54,   55,   56,   57,   58,   59,   60,   45,
 /*   940 */    62,   63,   64,   65,   34,   67,   43,   28,  156,   71,
 /*   950 */    72,   73,   74,   34,   35,   77,  164,  165,  166,  167,
 /*   960 */    83,   42,   43,   44,  168,  169,   69,   89,   90,   91,
 /*   970 */    92,   52,   53,   54,   55,   56,   57,   58,   59,   60,
 /*   980 */    42,   62,   63,   64,   65,   34,   67,   34,   50,   34,
 /*   990 */    71,   72,   73,   74,   43,   44,   77,   42,   28,  128,
 /*  1000 */    47,  130,  131,   69,   34,   35,  159,  156,   89,   90,
 /*  1010 */    91,   92,   42,   43,   44,  164,  165,  166,  167,   34,
 /*  1020 */   168,  169,   52,   53,   54,   55,   56,   57,   58,   59,
 /*  1030 */    60,    6,   62,   63,   64,   65,   47,   67,   38,   28,
 /*  1040 */   156,   71,   72,   73,   74,   34,   35,   77,  164,  165,
 /*  1050 */   166,  167,    6,   42,   43,   44,   75,   32,    6,   89,
 /*  1060 */    90,   91,   92,   52,   53,   54,   55,   56,   57,   58,
 /*  1070 */    59,   60,   34,   62,   63,   64,   65,   35,   67,   35,
 /*  1080 */    42,   35,   71,   72,   73,   74,   42,   35,   77,   45,
 /*  1090 */    28,  128,   50,  130,  131,   34,   34,   35,   43,  156,
 /*  1100 */    89,   90,   91,   92,   42,   43,   44,  164,  165,  166,
 /*  1110 */   167,  159,  168,  169,   52,   53,   54,   55,   56,   57,
 /*  1120 */    58,   59,   60,    6,   62,   63,   64,   65,   34,   67,
 /*  1130 */    72,   28,  156,   71,   72,   73,   74,   34,   35,   77,
 /*  1140 */   164,  165,  166,  167,    6,   42,   43,   44,  109,   32,
 /*  1150 */    34,   89,   90,   91,   92,   52,   53,   54,   55,   56,
 /*  1160 */    57,   58,   59,   60,   42,   62,   63,   64,   65,  159,
 /*  1170 */    67,   34,   50,   35,   71,   72,   73,   74,   72,    6,
 /*  1180 */    77,  109,   28,  168,  169,   35,    6,  159,   34,   35,
 /*  1190 */    43,  156,   89,   90,   91,   92,   42,   43,   44,  164,
 /*  1200 */   165,  166,  167,   35,   85,   32,   52,   53,   54,   55,
 /*  1210 */    56,   57,   58,   59,   60,   35,   62,   63,   64,   65,
 /*  1220 */    69,   67,  109,   28,  156,   71,   72,   73,   74,   34,
 /*  1230 */    35,   77,  164,  165,  166,  167,    6,   42,   43,   44,
 /*  1240 */   159,   35,    6,   89,   90,   91,   92,   52,   53,   54,
 /*  1250 */    55,   56,   57,   58,   59,   60,   50,   62,   63,   64,
 /*  1260 */    65,   16,   67,  168,  169,   35,   71,   72,   73,   74,
 /*  1270 */    42,   35,   77,   34,   28,   34,   86,   42,   50,   66,
 /*  1280 */    34,   35,   42,  156,   89,   90,   91,   92,   42,   43,
 /*  1290 */    44,  164,  165,  166,  167,  109,   42,   35,   52,   53,
 /*  1300 */    54,   55,   56,   57,   58,   59,   60,    6,   62,   63,
 /*  1310 */    64,   65,  159,   67,  109,   28,  156,   71,   72,   73,
 /*  1320 */    74,   34,   35,   77,  164,  165,  166,  167,    6,   42,
 /*  1330 */    43,   44,   42,   32,   85,   89,   90,   91,   92,   52,
 /*  1340 */    53,   54,   55,   56,   57,   58,   59,   60,  119,   62,
 /*  1350 */    63,   64,   65,   43,   67,  159,   42,   35,   71,   72,
 /*  1360 */    73,   74,   66,   35,   77,   45,   28,   49,   42,   34,
 /*  1370 */    34,   34,   34,   35,   49,  156,   89,   90,   91,   92,
 /*  1380 */    42,   43,   44,  164,  165,  166,  167,   34,   49,   35,
 /*  1390 */    52,   53,   54,   55,   56,   57,   58,   59,   60,   35,
 /*  1400 */    62,   63,   64,   65,  170,   67,   43,   28,  156,   71,
 /*  1410 */    72,   73,   74,   34,   35,   77,  164,  165,  166,  167,
 /*  1420 */    35,   42,   43,   44,   42,   42,   35,   89,   90,   91,
 /*  1430 */    92,   52,   53,   54,   55,   56,   57,   58,   59,   60,
 /*  1440 */   108,   62,   63,   64,   65,   43,   67,   42,   34,  155,
 /*  1450 */    71,   72,   73,   74,  109,   43,   77,   34,   28,   49,
 /*  1460 */    34,   42,   34,  108,   34,   35,   42,  156,   89,   90,
 /*  1470 */    91,   92,   42,   43,   44,  164,  165,  166,  167,  119,
 /*  1480 */    34,   34,   52,   53,   54,   55,   56,   57,   58,   59,
 /*  1490 */    60,   35,   62,   63,   64,   65,   35,   67,   35,   28,
 /*  1500 */   156,   71,   72,   73,   74,   34,   35,   77,  164,  165,
 /*  1510 */   166,  167,   42,   42,   43,   44,  109,   35,   45,   89,
 /*  1520 */    90,   91,   92,   52,   53,   54,   55,   56,   57,   58,
 /*  1530 */    59,   60,   47,   62,   63,   64,   65,   35,   67,   34,
 /*  1540 */   119,   35,   71,   72,   73,   74,   34,   47,   77,   45,
 /*  1550 */    28,   32,  119,   35,  158,  120,   34,   35,  169,  156,
 /*  1560 */    89,   90,   91,   92,   42,   34,   44,  164,  165,  166,
 /*  1570 */   167,  119,  130,  120,   52,   53,   54,   55,   56,   57,
 /*  1580 */    58,   59,   60,   45,   62,   63,   64,   65,   35,   67,
 /*  1590 */   107,  159,   35,   71,   72,   73,   74,  119,  156,   77,
 /*  1600 */    34,    1,    2,    3,    4,    5,  164,  165,  166,  167,
 /*  1610 */   127,   89,   90,   91,   92,   35,  133,  134,  135,  136,
 /*  1620 */   137,  138,  139,  140,  141,  142,  143,  144,  145,  146,
 /*  1630 */   147,  148,  149,  150,  151,  152,  107,   35,  122,   16,
 /*  1640 */    40,   41,   34,   43,   44,   49,   46,  164,  165,  166,
 /*  1650 */   167,   51,   35,   35,  120,   45,  127,  177,  177,  177,
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
 /*  2370 */   151,  152,  107,  177,  177,  177,  153,  177,  177,  156,
 /*  2380 */   177,  177,  177,  164,  165,  166,  167,  164,  165,  166,
 /*  2390 */   167,  177,  127,  177,  177,  177,  177,  177,  133,  134,
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
 /*  2600 */   177,  177,  177,  177,   27,   28,   29,   30,   31,   32,
 /*  2610 */   177,   34,  177,  177,    7,    8,    9,  177,  177,   42,
 /*  2620 */    33,  177,   15,   36,   47,   38,   49,   40,   41,  177,
 /*  2630 */   177,   44,   83,   84,   27,   28,   29,   30,   31,  177,
 /*  2640 */    15,   34,   93,   94,   95,   96,   97,   98,  177,   42,
 /*  2650 */   177,  177,  177,  177,   47,  177,   49,   32,  156,   34,
 /*  2660 */    83,   84,  160,  177,  177,  177,  164,  165,  166,  167,
 /*  2670 */    93,   94,   95,   96,   97,   98,  177,   52,   53,   54,
 /*  2680 */    55,   56,   57,   58,   59,   60,   61,   62,  177,  177,
 /*  2690 */    83,   84,   85,  177,  177,  177,  177,  177,  177,  177,
 /*  2700 */    93,   94,   95,   96,   97,   98,  177,  177,    7,    8,
 /*  2710 */     9,  177,   33,  177,  177,   36,   15,   38,  177,   40,
 /*  2720 */    41,  177,  177,   44,  177,  177,  177,  177,   27,   28,
 /*  2730 */    29,   30,   31,   32,  177,   34,    7,    8,    9,  177,
 /*  2740 */   177,  177,  177,   42,   15,  177,  177,  177,   47,  177,
 /*  2750 */    49,  177,  177,  177,  177,  177,   27,   28,   29,   30,
 /*  2760 */    31,  177,  177,   34,  177,  177,    7,    8,    9,  177,
 /*  2770 */   177,   42,  156,  177,   15,  177,   47,  161,   49,  177,
 /*  2780 */   164,  165,  166,  167,   83,   84,   27,   28,   29,   30,
 /*  2790 */    31,  177,   15,   34,   93,   94,   95,   96,   97,   98,
 /*  2800 */   177,   42,  177,  177,  177,  177,   47,  177,   49,   32,
 /*  2810 */   177,   34,   83,   84,   85,  177,  177,  177,  177,  177,
 /*  2820 */   177,  177,   93,   94,   95,   96,   97,   98,  177,   52,
 /*  2830 */    53,   54,   55,   56,   57,   58,   59,   60,   61,   62,
 /*  2840 */   156,  177,   83,   84,   85,  161,  177,  177,  164,  165,
 /*  2850 */   166,  167,   93,   94,   95,   96,   97,   98,  177,  177,
 /*  2860 */     7,    8,    9,  177,  177,  177,  156,  177,   15,  177,
 /*  2870 */   177,  177,  177,  177,  164,  165,  166,  167,  177,  177,
 /*  2880 */    27,   28,   29,   30,   31,   32,  176,   34,    7,    8,
 /*  2890 */     9,  177,  177,  177,  177,   42,   15,  177,  177,  177,
 /*  2900 */    47,  177,   49,  177,  177,  177,  177,  177,   27,   28,
 /*  2910 */    29,   30,   31,  177,  177,   34,  177,  177,    7,    8,
 /*  2920 */     9,  177,  177,   42,  156,  177,   15,  177,   47,  161,
 /*  2930 */    49,  177,  164,  165,  166,  167,   83,   84,   27,   28,
 /*  2940 */    29,   30,   31,   32,  177,   34,   93,   94,   95,   96,
 /*  2950 */    97,   98,  177,   42,  177,  177,  177,   76,   47,  177,
 /*  2960 */    49,    7,    8,    9,   83,   84,  177,  177,  177,   15,
 /*  2970 */   177,  177,  177,  177,   93,   94,   95,   96,   97,   98,
 /*  2980 */   177,   27,   28,   29,   30,   31,  177,  177,   34,  177,
 /*  2990 */   177,    7,    8,    9,   83,   84,   42,  177,  177,   15,
 /*  3000 */   177,   47,  177,   49,   93,   94,   95,   96,   97,   98,
 /*  3010 */   177,   27,   28,   29,   30,   31,  177,  177,   34,    7,
 /*  3020 */     8,    9,  177,  177,  177,  177,   42,   15,  177,  177,
 /*  3030 */    76,   47,  177,   49,  177,  177,  177,   83,   84,   27,
 /*  3040 */    28,   29,   30,   31,   32,  177,   34,   93,   94,   95,
 /*  3050 */    96,   97,   98,  177,   42,   45,  177,  177,  177,   47,
 /*  3060 */   177,   49,  177,  177,  177,  156,  177,   83,   84,   85,
 /*  3070 */   177,  177,  163,  164,  165,  166,  167,   93,   94,   95,
 /*  3080 */    96,   97,   98,  177,  177,   15,  177,  177,   78,   79,
 /*  3090 */    80,   81,   82,  177,   84,   83,   84,   87,   88,  177,
 /*  3100 */   177,  177,   32,  177,   34,   93,   94,   95,   96,   97,
 /*  3110 */    98,  177,  177,    7,    8,    9,  177,  177,  177,  177,
 /*  3120 */   177,   15,   52,   53,   54,   55,   56,   57,   58,   59,
 /*  3130 */    60,   61,   62,   27,   28,   29,   30,   31,   32,  177,
 /*  3140 */    34,    7,    8,    9,  177,  177,  177,  177,   42,   15,
 /*  3150 */   177,  177,  177,   47,  177,   49,  177,  177,  177,  177,
 /*  3160 */   177,   27,   28,   29,   30,   31,  177,  177,   34,   35,
 /*  3170 */   177,    7,    8,    9,  177,  177,   42,  156,  177,   15,
 /*  3180 */   177,   47,  177,   49,  177,  164,  165,  166,  167,   83,
 /*  3190 */    84,   27,   28,   29,   30,   31,  177,  177,   34,   93,
 /*  3200 */    94,   95,   96,   97,   98,  177,   42,  177,  177,  177,
 /*  3210 */   177,   47,  177,   49,    7,    8,    9,   83,   84,  177,
 /*  3220 */   177,  177,   15,  177,  177,  177,  177,   93,   94,   95,
 /*  3230 */    96,   97,   98,  177,   27,   28,   29,   30,   31,  177,
 /*  3240 */   177,   34,  156,  177,  177,  177,  160,   83,   84,   42,
 /*  3250 */   164,  165,  166,  167,   47,  177,   49,   93,   94,   95,
 /*  3260 */    96,   97,   98,  177,  177,   10,   11,   12,   13,   14,
 /*  3270 */    15,   16,   17,   18,   19,   20,   21,   22,   23,   24,
 /*  3280 */    25,   26,  177,  177,  177,  177,  177,  177,  177,  177,
 /*  3290 */    83,   84,  177,   45,  177,  177,  177,   42,   50,  177,
 /*  3300 */    93,   94,   95,   96,   97,   98,  177,  177,   10,   11,
 /*  3310 */    12,   13,   14,   15,   16,   17,   18,   19,   20,   21,
 /*  3320 */    22,   23,   24,   25,   26,  177,   78,   79,   80,   81,
 /*  3330 */    82,   15,   84,  156,   86,   87,   88,  160,  177,  177,
 /*  3340 */    42,  164,  165,  166,  167,  177,  177,  177,   32,  177,
 /*  3350 */    34,   10,   11,   12,   13,   14,   15,   16,   17,   18,
 /*  3360 */    19,   20,   21,   22,   23,   24,   25,   26,   52,   53,
 /*  3370 */    54,   55,   56,   57,   58,   59,   60,   61,   62,  156,
 /*  3380 */   177,  177,  177,   42,  177,  177,  177,  164,  165,  166,
 /*  3390 */   167,  177,  177,  177,   10,   11,   12,   13,   14,   15,
 /*  3400 */    16,   17,   18,   19,   20,   21,   22,   23,   24,   25,
 /*  3410 */    26,   10,   11,   12,   13,   14,   15,   16,   17,   18,
 /*  3420 */    19,   20,   21,   22,   23,   24,   25,   26,  177,  177,
 /*  3430 */   177,  177,  177,  177,  177,  177,   35,   10,   11,   12,
 /*  3440 */    13,   14,   15,   16,   17,   18,   19,   20,   21,   22,
 /*  3450 */    23,   24,   25,   26,   13,   14,   15,   16,   17,   18,
 /*  3460 */    19,   20,   21,   22,   23,   24,   25,   26,  156,   85,
 /*  3470 */   177,  177,  160,  177,  177,  177,  164,  165,  166,  167,
 /*  3480 */    10,   11,   12,   13,   14,   15,   16,   17,   18,   19,
 /*  3490 */    20,   21,   22,   23,   24,   25,   26,  177,  156,  177,
 /*  3500 */   177,  177,  160,   15,  177,   35,  164,  165,  166,  167,
 /*  3510 */   156,  177,   85,  177,  160,  177,  177,  177,  164,  165,
 /*  3520 */   166,  167,   34,   10,   11,   12,   13,   14,   15,   16,
 /*  3530 */    17,   18,   19,   20,   21,   22,   23,   24,   25,   26,
 /*  3540 */    52,   53,   54,   55,   56,   57,   58,   59,   60,   61,
 /*  3550 */    62,  177,  156,  177,  177,   42,  160,  177,  177,  177,
 /*  3560 */   164,  165,  166,  167,  177,  177,   10,   11,   12,   13,
 /*  3570 */    14,   15,   16,   17,   18,   19,   20,   21,   22,   23,
 /*  3580 */    24,   25,   26,   10,   11,   12,   13,   14,   15,   16,
 /*  3590 */    17,   18,   19,   20,   21,   22,   23,   24,   25,   26,
 /*  3600 */   177,  177,  177,  177,  177,   32,   10,   11,   12,   13,
 /*  3610 */    14,   15,   16,   17,   18,   19,   20,   21,   22,   23,
 /*  3620 */    24,   25,   26,   11,   12,   13,   14,   15,   16,   17,
 /*  3630 */    18,   19,   20,   21,   22,   23,   24,   25,   26,   15,
 /*  3640 */   156,   85,  177,  177,  177,  177,  177,  177,  164,  165,
 /*  3650 */   166,  167,  177,  156,  177,  171,  177,  177,  174,  175,
 /*  3660 */   176,  164,  165,  166,  167,  177,  177,  177,  177,  177,
 /*  3670 */   177,  174,  175,  176,  177,  177,   52,   53,   54,   55,
 /*  3680 */    56,   57,   58,   59,   60,   61,   62,    1,    2,    3,
 /*  3690 */     4,    5,  156,  177,  177,  177,  160,  177,  177,  177,
 /*  3700 */   164,  165,  166,  167,  156,  177,  177,  177,  160,  156,
 /*  3710 */   177,  177,  164,  165,  166,  167,  177,  164,  165,  166,
 /*  3720 */   167,  177,  177,  177,  177,  156,   40,   41,  177,   43,
 /*  3730 */    44,  177,   46,  164,  165,  166,  167,   51,  177,    1,
 /*  3740 */     2,    3,    4,    5,  156,  177,  177,  177,  160,  177,
 /*  3750 */   177,  177,  164,  165,  166,  167,  156,  177,  177,  177,
 /*  3760 */   160,  177,  156,  177,  164,  165,  166,  167,  177,  177,
 /*  3770 */   164,  165,  166,  167,  177,  177,  177,  156,   40,   41,
 /*  3780 */   177,   43,   44,  177,   46,  164,  165,  166,  167,   51,
 /*  3790 */   156,    1,    2,    3,    4,    5,  156,  177,  164,  165,
 /*  3800 */   166,  167,  177,  177,  164,  165,  166,  167,  156,  177,
 /*  3810 */   177,  177,  156,  177,  177,  177,  164,  165,  166,  167,
 /*  3820 */   164,  165,  166,  167,  177,  177,  177,  177,  177,  156,
 /*  3830 */    40,   41,  177,   43,   44,  177,   46,  164,  165,  166,
 /*  3840 */   167,   51,  177,    1,    2,    3,    4,    5,    1,    2,
 /*  3850 */     3,    4,    5,  156,  177,  177,  177,  177,  177,  177,
 /*  3860 */   177,  164,  165,  166,  167,  177,  177,  177,  177,  177,
 /*  3870 */   177,  177,  177,  177,  177,  177,  177,  177,  177,  177,
 /*  3880 */   177,  177,   40,   41,  177,   43,   44,   40,   41,  177,
 /*  3890 */    43,   44,  177,   51,  177,  177,  177,  177,   51,  177,
 /*  3900 */     1,    2,    3,    4,    5,    1,    2,    3,    4,    5,
 /*  3910 */   177,  177,  177,  177,  177,  177,  177,  177,  177,  177,
 /*  3920 */   177,  177,  177,  177,  177,  177,  177,  177,  177,  177,
 /*  3930 */   177,  177,  177,  177,  177,  177,  177,  177,  177,   40,
 /*  3940 */    41,  177,   43,   44,   40,   41,  177,   43,   44,  177,
 /*  3950 */    51,  177,  177,  177,  177,   51,  177,    1,    2,    3,
 /*  3960 */     4,    5,    1,    2,    3,    4,    5,  177,  177,  177,
 /*  3970 */   177,  177,  177,  177,  177,  177,  177,  177,  177,  177,
 /*  3980 */   177,  177,  177,  177,  177,  177,  177,  177,  177,  177,
 /*  3990 */   177,  177,  177,  177,  177,  177,   40,   41,  177,   43,
 /*  4000 */    44,   40,   41,  177,   43,   44,  177,   51,  177,  177,
 /*  4010 */   177,  177,   51,  177,    1,    2,    3,    4,    5,    1,
 /*  4020 */     2,    3,    4,    5,  177,  177,  177,  177,  177,  177,
 /*  4030 */   177,  177,  177,  177,  177,  177,  177,  177,  177,  177,
 /*  4040 */   177,  177,  177,  177,  177,  177,  177,  177,  177,  177,
 /*  4050 */   177,  177,  177,   40,   41,  177,   43,   44,   40,   41,
 /*  4060 */   177,   43,   44,  177,   51,  177,  177,  177,  177,   51,
 /*  4070 */   177,    1,    2,    3,    4,    5,  177,  177,  177,  177,
 /*  4080 */   177,  177,  177,  177,  177,  177,  177,  177,  177,  177,
 /*  4090 */   177,  177,  177,  177,  177,  177,  177,  177,  177,  177,
 /*  4100 */   177,  177,  177,  177,  177,  177,  177,  177,  177,  177,
 /*  4110 */    40,   41,  177,  177,  177,  177,   46,  177,  177,  177,
 /*  4120 */   177,   51,
};
#define YY_SHIFT_USE_DFLT (-10)
static short yy_shift_ofst[] = {
 /*     0 */  2679,  341, 2587,  -10,  -10,  -10,  -10,  -10, 1505, 1463,
 /*    10 */   -10, 1446,  810,  -10, 1423, 1405,  -10, 3961,  -10, 3899,
 /*    20 */   -10,  -10, 2064, 1336, 1318, 3070, 1042, 3624, 1328,  -10,
 /*    30 */   -10,  -10, 1239, 1245,  -10,  -10,  -10,  -10,  -10,  -10,
 /*    40 */   -10,  -10,  -10,  -10,  -10,  -10,  -10,  514, 1206, 3624,
 /*    50 */  1262,  -10,  -10, 3488,  -10,  894,   95,  -10,  -10,  -10,
 /*    60 */   -10,  -10,  -10,  -10,  -10, 1190,  989,  -10,  268, 1610,
 /*    70 */    95,  -10, 1337, 1320,   95,  -10,  -10,  -10,  -10,  -10,
 /*    80 */   -10,  -10,  -10,  -10,  -10,  -10, 2248, 1880,  380, 1596,
 /*    90 */  3316,  401, 3624, 1461,  -10,  -10,  529,  710, 3624, 1602,
 /*   100 */   -10,  -10,  -10,  -10, 1137,    2,  -10, 1531,  320,  -10,
 /*   110 */   323,  294,  -10, 3686,  -10,  390,  -10, 1600, 3842,  -10,
 /*   120 */  2156,  525,  568, 2625, 1228,  418,  -10,  643,  -10,  -10,
 /*   130 */   -10,  -10,  -10,  -10,  -10,  -10,  -10,  -10,  -10,  -10,
 /*   140 */   -10,  -10,  -10,  -10,  -10,  -10,  -10,  -10,  -10,  -10,
 /*   150 */   -10, 1618,  -10, 1502,  -10, 3164, 1470,   50, 1195, 1296,
 /*   160 */  1382,  970,  878,  -10, 3164, 1290,  307,  -10,  181,  -10,
 /*   170 */   -10,   95,  897, 1522, 1522, 1235,  510,  786,  -10, 3164,
 /*   180 */  1419, 1103,  919,  -10,  427,  275, 1379, 1058, 3164, 1168,
 /*   190 */   -10, 3164,  -10, 3164,  874,  -10,  119, 3164, 3164, 3596,
 /*   200 */  3164, 3164, 3596, 3164, 3164, 3596, 3164,  258, 3164,  258,
 /*   210 */  3164,  258, 3164,  258, 3164,  258, 3164,   49, 3164,   49,
 /*   220 */  3164,  -10, 3164,  -10, 3164,  -10, 3164,   49, 3164, 3441,
 /*   230 */  3164, 3441, 3164, 3612,   -7, 3573,  -10, 1519, 3164,  -10,
 /*   240 */  1566, 1623, 3164,  258,  124,  955,  267, 2577, 3596,  748,
 /*   250 */   848,  874,  -10,  -10, 3164,  -10,  -10,  -10,  -10,  -10,
 /*   260 */   -10,  -10,  -10,  -10, 2984, 3596,   32,  953,  669, 2701,
 /*   270 */  1335, 1325, 2911,  -10,  606,  -10, 3164, 1414, 1412, 1410,
 /*   280 */  2549, 1301,  -10,  -10,  -10,  -10,  -10,  -10,  -10,  698,
 /*   290 */   -10,  -10,  -10, 3164, 3556,  -10, 2853, 1173,  -10,  -10,
 /*   300 */   523,  727,  -10,   90,  -10, 3207,  -10,  934, 3164,  -10,
 /*   310 */   -10,  -10,  238,  -10,  -10,  679, 1147,  823, 3012,  422,
 /*   320 */   -10,  -10, 1310,  856, 3106, 1025,  -10,  -10,  258,  258,
 /*   330 */    46,  -10,  137,  -10,  587, 1363,  -10, 1402,  -10,  258,
 /*   340 */  3596, 1426,  370, 2954, 3513, 1522, 1062,  -10, 1512,  444,
 /*   350 */   -10, 1512,  -10, 3248,  -10,  -10,  -10,  -10,  -10,  -10,
 /*   360 */  3164,  -10, 3596, 1038, 3010, 3164,  -10, 2759,  185, 3164,
 /*   370 */   -10,  728,  185, 3164,  -10, 3384,  -10,  -10,  877,  903,
 /*   380 */   185, 3164,  -10, 1055,  185, 3164,  -10, 1116, 1698, 3164,
 /*   390 */   -10, 2607,  185, 3164,  -10, 1119,  185, 3164,  -10, 2729,
 /*   400 */   185, 3164,  -10, 1249,  185, 3164,  -10,  -10,  -10,  -10,
 /*   410 */  3164, 1322,  -10, 3164,  -10, 3596,  -10, 1354,  -10, 1364,
 /*   420 */   -10, 1385,  -10, 1391,  -10, 3134, 3470,  -10,  -10, 1428,
 /*   430 */   548, 1447, 1456,  -10, 3164, 3427, 1482,  -10, 3164, 3401,
 /*   440 */   -10,  153,  494,  -10,  153,  -10, 1538,   95,  -10,  -10,
 /*   450 */   153,  684,  -10,  153,  862,  -10,  153,  868,  -10,  153,
 /*   460 */  1046,  -10,  153, 1052,  -10,  153, 1138,  -10,  153, 1180,
 /*   470 */   -10,  153, 1230,  -10,  153, 1236,  -10,  209,  709,  669,
 /*   480 */    86,  267,    0,  776,  823,  859,  856,  -10,  -10, 3164,
 /*   490 */  3255, 1522,  827,  -10,  985,  981, 2881, 3298, 1522, 1011,
 /*   500 */   -10, 3164, 3341, 1522, 1287,  -10, 1106, 3164, 1150,  -10,
 /*   510 */   -10,  -10, 1151, 1522, 1522,  -10,  -10, 1213, 1240,  551,
 /*   520 */   694,  -10,  -10, 3624, 1314,  367,  -10,   91,  -10, 1117,
 /*   530 */  1122,  142,  -10,  326,  -10, 3624, 1326,  234,  -10,  183,
 /*   540 */   -10, 2248, 1972, 1353, 1339, 2777,  768,  735,  -10,  602,
 /*   550 */   -10, 3624, 1383,  459,  -10, 1471,  -10,  882,  938, 1430,
 /*   560 */   -10, 1338,  -10, 3624, 1424, 1246,  -10, 1154,  -10, 3738,
 /*   570 */  3847,  -10,  -10, 4070, 1485, 1473,   95, 1506,  -10, 1500,
 /*   580 */  1504,   95, 1518,  -10,  -10, 4070, 1788,   -9,  -10,   95,
 /*   590 */   584,  -10, 1553,  -10,  951, 1557,  279, 1580,  619,  -10,
 /*   600 */   -10, 1608,  -10,  -10, 1617,  -10, 1696, 1044,  -10,   95,
 /*   610 */   780,  -10,  117,  -10,  226,  -10,  -10, 3790, 4013, 4018,
 /*   620 */  2248, 3904,  -10, 3956,  323,  -10,  -10,  -10,  323,  294,
 /*   630 */   -10,  233,  910,  316,  -10, 1061, 1254,  -10,  323,  294,
 /*   640 */   -10, 1000, 1094,  350,  -10, 1241, 1254,  -10,  -10,
};
#define YY_REDUCE_USE_DFLT (-114)
static short yy_reduce_ofst[] = {
 /*     0 */   -91, -114, -100, -114, -114, -114, -114, -114, -114, -114,
 /*    10 */  -114, -114, 1355, -114, -114, 1332, -114, 1620, -114,  502,
 /*    20 */  -114, -114,  405, -114, -114,    6, -114,  397, -114, -114,
 /*    30 */  -114, -114, -114, -114, -114, -114, -114, -114, -114, -114,
 /*    40 */  -114, -114, -114, -114, -114, -114, -114, -114, -114,  -33,
 /*    50 */  -114, -114, -114, -113, -114, -114,  565, -114, -114, -114,
 /*    60 */  -114, -114, -114, -114, -114, -114, -114, -114, -114, -114,
 /*    70 */  1360, -114, -114, -114, 1229, -114, -114, -114, -114, -114,
 /*    80 */  -114, -114, -114, -114, -114, -114,  626,  405, -114, -114,
 /*    90 */   595, -114,  779, -114, -114, -114, -114, -114,  963, -114,
 /*   100 */  -114, -114, -114, -114, -114, 1113, -114, -114, 1039, -114,
 /*   110 */   466,  559, -114,  -86, -114, -114, -114, 2218,  687, -114,
 /*   120 */   405, -114, -114,  503, -114, 1575, -114, 2403, -114, -114,
 /*   130 */  -114, -114, -114, -114, -114, -114, -114, -114, -114, -114,
 /*   140 */  -114, -114, -114, -114, -114, -114, -114, -114, -114, -114,
 /*   150 */  -114, -114, -114, -114, -114, 2131, -114, 2311, 2403, -114,
 /*   160 */  -114, 2357, 2403, -114, 2223, -114,  497, -114, 1294, -114,
 /*   170 */  -114,  387, -114, 1897, 2403, -114, 1621, 2403, -114, 2269,
 /*   180 */  -114, 2127, 2403, -114, -114, 1529, 2403, -114, 2315, -114,
 /*   190 */  -114,  207, -114, 1219, 1234, -114, -114,  483, 3656, -114,
 /*   200 */   851, 1068, -114, 1403, 1311, -114, 3652, -114, 3634, -114,
 /*   210 */  3606, -114,  299, -114,  943, -114, 1160, -114, 3621, -114,
 /*   220 */  1035, -114, 3223, -114, 3569, -114,  667, -114,  759, -114,
 /*   230 */  3673, -114, 3697, -114, 1442, -114, -114, -114, 1127, -114,
 /*   240 */  -114, -114, 3640, -114, -114, -114, -114,  240, -114, -114,
 /*   250 */  -114,  757, -114, -114,  792, -114, -114, -114, -114, -114,
 /*   260 */  -114, -114, -114, -114, 3484, -114, -114, -114, -114,  608,
 /*   270 */  -114, -114,   56, -114, -114, -114,  700, -114, -114, -114,
 /*   280 */   148, -114, -114, -114, -114, -114, -114, -114, -114, -114,
 /*   290 */  -114, -114, -114, 1344, -114, -114,  332, -114, -114, -114,
 /*   300 */  -114, -114, -114, -114, -114, 3497, -114, -114, 2710, -114,
 /*   310 */  -114, -114, -114, -114, -114, -114, -114, -114,  424, -114,
 /*   320 */  -114, -114, -114, -114,  516, -114, -114, -114, -114, -114,
 /*   330 */  -114, -114, -114, -114, -114, -114, -114, -114, -114, -114,
 /*   340 */  -114, -114, -114,  391, -114, 1483, 2403, -114,  -78, -114,
 /*   350 */  -114, 1396, -114, 1432, -114, -114, -114, -114, -114, -114,
 /*   360 */  2502, -114, -114, -114,  249, 3086, -114, 2616,  617, 3177,
 /*   370 */  -114, -114,  676, 3312, -114, -114, -114, -114, -114, -114,
 /*   380 */   847, 3342, -114, -114,  952, 3354, -114, -114, 1010, 3396,
 /*   390 */  -114, 2684, 1028, 3536, -114, -114, 1081, 3548, -114, 2768,
 /*   400 */  1153, 3588, -114, -114, 1196, 3600, -114, -114, -114, -114,
 /*   410 */  2363, -114, -114, 2909, -114, -114, -114, -114, -114, -114,
 /*   420 */  -114, -114, -114, -114, -114, 3021, -114, -114, -114, -114,
 /*   430 */  -114, -114, -114, -114, 1252, -114, -114, -114, 3553, -114,
 /*   440 */  -114,   84, -114, -114, 1389, -114, -114, 1478, -114, -114,
 /*   450 */   371, -114, -114,  555, -114, -114,  668, -114, -114,  760,
 /*   460 */  -114, -114,  796, -114, -114,  852, -114, -114,  944, -114,
 /*   470 */  -114, 1015, -114, -114, 1095, -114, -114, -114, -114, -114,
 /*   480 */  -114, -114, -114, -114, -114, -114, -114, -114, -114,  976,
 /*   490 */  -114, 2265, 2403, -114, -114, -114,  884, -114, 2219, 2403,
 /*   500 */  -114,  575, -114, 2173, 2403, -114, -114, 2407, -114, -114,
 /*   510 */  -114, -114, -114, 2081, 2403, -114, -114, -114, -114, 2035,
 /*   520 */  2403, -114, -114,  581, -114, 1989, -114, 2403, -114, -114,
 /*   530 */  -114, 1943, -114, 2403, -114,  871, -114, 1851, -114, 2403,
 /*   540 */  -114,  -18,  405, -114, -114,  411, -114, 1805, -114, 2403,
 /*   550 */  -114,  765, -114, 1759, -114, 2403, -114, -114, -114, 1713,
 /*   560 */  -114, 2403, -114,  673, -114, 1667, -114, 2403, -114,  319,
 /*   570 */   687, -114, -114,  -18, -114, -114, 1421, -114, -114, -114,
 /*   580 */  -114, 1433, -114, -114, -114,  442,  405, 1435, -114, 1452,
 /*   590 */  1453, -114, -114, -114,  346, -114, -114, -114, 1516, -114,
 /*   600 */  -114, -114, -114, -114, -114, -114,  405, 1534, -114,  -20,
 /*   610 */    10, -114, -114, -114, -114, -114, -114,   43,  227,  687,
 /*   620 */   442,  687, -114,  687,  282, -114, -114, -114,  337, 1186,
 /*   630 */  -114, -114, -114, 1407, -114, -114, 1345, -114,  374,  708,
 /*   640 */  -114, -114, -114, 1072, -114, -114, 1205, -114, -114,
};
static YYACTIONTYPE yy_default[] = {
 /*     0 */   954,  954,  954,  651,  653,  654,  655,  656,  954,  954,
 /*    10 */   657,  954,  954,  658,  954,  954,  659,  954,  674,  954,
 /*    20 */   675,  705,  954,  954,  954,  954,  954,  954,  954,  725,
 /*    30 */   743,  744,  954,  954,  753,  754,  755,  756,  757,  758,
 /*    40 */   759,  760,  761,  762,  763,  764,  729,  954,  954,  954,
 /*    50 */   954,  726,  730,  954,  745,  747,  954,  750,  940,  941,
 /*    60 */   942,  943,  944,  945,  946,  954,  954,  947,  954,  748,
 /*    70 */   954,  751,  954,  749,  954,  752,  746,  733,  735,  736,
 /*    80 */   737,  738,  739,  740,  741,  742,  954,  954,  954,  954,
 /*    90 */   954,  954,  954,  954,  727,  731,  954,  954,  954,  954,
 /*   100 */   728,  732,  734,  706,  954,  954,  660,  954,  954,  661,
 /*   110 */   954,  954,  663,  954,  669,  954,  670,  954,  954,  703,
 /*   120 */   954,  954,  954,  954,  954,  954,  709,  954,  711,  765,
 /*   130 */   767,  768,  769,  770,  771,  772,  773,  774,  775,  776,
 /*   140 */   777,  778,  779,  780,  781,  782,  783,  784,  785,  786,
 /*   150 */   787,  954,  788,  954,  789,  954,  954,  954,  954,  792,
 /*   160 */   954,  954,  954,  793,  954,  954,  954,  796,  954,  797,
 /*   170 */   798,  954,  954,  800,  801,  954,  954,  954,  804,  954,
 /*   180 */   954,  954,  954,  806,  954,  954,  954,  954,  954,  954,
 /*   190 */   808,  954,  866,  954,  954,  867,  954,  954,  954,  868,
 /*   200 */   954,  954,  869,  954,  954,  870,  954,  874,  954,  875,
 /*   210 */   954,  876,  954,  877,  954,  878,  954,  888,  954,  889,
 /*   220 */   954,  890,  954,  891,  954,  892,  954,  893,  954,  894,
 /*   230 */   954,  895,  954,  896,  954,  954,  879,  954,  954,  880,
 /*   240 */   954,  954,  954,  881,  901,  954,  882,  954,  931,  954,
 /*   250 */   954,  954,  897,  898,  954,  899,  900,  902,  903,  904,
 /*   260 */   905,  906,  907,  908,  954,  939,  901,  954,  885,  954,
 /*   270 */   954,  911,  954,  912,  954,  913,  954,  954,  954,  954,
 /*   280 */   954,  954,  916,  917,  926,  927,  928,  930,  929,  954,
 /*   290 */   918,  919,  886,  954,  954,  887,  954,  954,  914,  915,
 /*   300 */   902,  903,  909,  954,  910,  954,  932,  954,  954,  934,
 /*   310 */   935,  933,  954,  920,  921,  954,  954,  883,  954,  954,
 /*   320 */   922,  923,  954,  884,  954,  954,  924,  925,  873,  872,
 /*   330 */   954,  948,  954,  949,  954,  954,  950,  954,  951,  871,
 /*   340 */   952,  954,  954,  954,  954,  954,  954,  809,  954,  954,
 /*   350 */   813,  954,  814,  954,  816,  817,  818,  819,  820,  821,
 /*   360 */   954,  822,  865,  954,  954,  954,  823,  954,  954,  954,
 /*   370 */   826,  954,  954,  954,  827,  954,  833,  834,  954,  954,
 /*   380 */   954,  954,  824,  954,  954,  954,  825,  954,  954,  954,
 /*   390 */   828,  954,  954,  954,  829,  954,  954,  954,  830,  954,
 /*   400 */   954,  954,  831,  954,  954,  954,  832,  835,  836,  815,
 /*   410 */   954,  954,  838,  954,  839,  841,  840,  954,  842,  954,
 /*   420 */   843,  954,  844,  954,  845,  954,  954,  846,  847,  954,
 /*   430 */   954,  954,  954,  848,  954,  954,  954,  849,  954,  954,
 /*   440 */   850,  954,  954,  851,  954,  861,  863,  954,  864,  862,
 /*   450 */   954,  954,  852,  954,  954,  853,  954,  954,  854,  954,
 /*   460 */   954,  855,  954,  954,  856,  954,  954,  857,  954,  954,
 /*   470 */   858,  954,  954,  859,  954,  954,  860,  954,  954,  954,
 /*   480 */   954,  954,  954,  954,  954,  954,  954,  953,  766,  954,
 /*   490 */   954,  954,  954,  810,  954,  954,  954,  954,  954,  954,
 /*   500 */   811,  954,  954,  954,  954,  812,  954,  954,  954,  807,
 /*   510 */   805,  803,  954,  954,  802,  799,  794,  790,  954,  954,
 /*   520 */   954,  795,  791,  954,  954,  954,  717,  954,  719,  954,
 /*   530 */   954,  954,  710,  954,  712,  954,  954,  954,  718,  954,
 /*   540 */   720,  954,  954,  954,  954,  954,  954,  954,  713,  954,
 /*   550 */   715,  954,  954,  954,  721,  954,  723,  954,  954,  954,
 /*   560 */   714,  954,  716,  954,  954,  954,  722,  954,  724,  954,
 /*   570 */   954,  701,  704,  954,  954,  954,  954,  954,  707,  954,
 /*   580 */   954,  954,  954,  708,  685,  954,  954,  954,  687,  954,
 /*   590 */   954,  689,  954,  693,  954,  954,  954,  954,  954,  697,
 /*   600 */   699,  954,  700,  698,  954,  691,  954,  954,  688,  954,
 /*   610 */   954,  690,  954,  694,  954,  692,  702,  954,  954,  954,
 /*   620 */   954,  954,  686,  954,  954,  671,  673,  672,  954,  954,
 /*   630 */   662,  954,  954,  954,  664,  954,  954,  665,  954,  954,
 /*   640 */   666,  954,  954,  954,  667,  954,  954,  668,  652,
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
 /* 234 */ "xx_common_expr ::= IDENTIFIER ARROW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE",
 /* 235 */ "xx_common_expr ::= IDENTIFIER ARROW BRACKET_OPEN STRING BRACKET_CLOSE",
 /* 236 */ "xx_common_expr ::= IDENTIFIER DOUBLECOLON IDENTIFIER",
 /* 237 */ "xx_common_expr ::= IDENTIFIER DOUBLECOLON CONSTANT",
 /* 238 */ "xx_common_expr ::= IDENTIFIER SBRACKET_OPEN xx_common_expr SBRACKET_CLOSE",
 /* 239 */ "xx_common_expr ::= xx_common_expr ADD xx_common_expr",
 /* 240 */ "xx_common_expr ::= xx_common_expr SUB xx_common_expr",
 /* 241 */ "xx_common_expr ::= xx_common_expr MUL xx_common_expr",
 /* 242 */ "xx_common_expr ::= xx_common_expr DIV xx_common_expr",
 /* 243 */ "xx_common_expr ::= xx_common_expr MOD xx_common_expr",
 /* 244 */ "xx_common_expr ::= xx_common_expr CONCAT xx_common_expr",
 /* 245 */ "xx_common_expr ::= xx_common_expr AND xx_common_expr",
 /* 246 */ "xx_common_expr ::= xx_common_expr OR xx_common_expr",
 /* 247 */ "xx_common_expr ::= xx_common_expr INSTANCEOF xx_common_expr",
 /* 248 */ "xx_fetch_expr ::= FETCH IDENTIFIER COMMA xx_isset_expr",
 /* 249 */ "xx_common_expr ::= xx_fetch_expr",
 /* 250 */ "xx_common_expr ::= TYPEOF xx_common_expr",
 /* 251 */ "xx_common_expr ::= CONSTANT",
 /* 252 */ "xx_common_expr ::= IDENTIFIER",
 /* 253 */ "xx_common_expr ::= INTEGER",
 /* 254 */ "xx_common_expr ::= STRING",
 /* 255 */ "xx_common_expr ::= CHAR",
 /* 256 */ "xx_common_expr ::= DOUBLE",
 /* 257 */ "xx_common_expr ::= NULL",
 /* 258 */ "xx_common_expr ::= TRUE",
 /* 259 */ "xx_common_expr ::= FALSE",
 /* 260 */ "xx_common_expr ::= SBRACKET_OPEN SBRACKET_CLOSE",
 /* 261 */ "xx_common_expr ::= SBRACKET_OPEN xx_array_list SBRACKET_CLOSE",
 /* 262 */ "xx_common_expr ::= NEW IDENTIFIER",
 /* 263 */ "xx_common_expr ::= NEW IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 264 */ "xx_common_expr ::= NEW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 265 */ "xx_fcall_expr ::= IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 266 */ "xx_fcall_expr ::= IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 267 */ "xx_fcall_expr ::= BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 268 */ "xx_fcall_expr ::= BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 269 */ "xx_scall_expr ::= IDENTIFIER DOUBLECOLON IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 270 */ "xx_scall_expr ::= IDENTIFIER DOUBLECOLON IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 271 */ "xx_mcall_expr ::= IDENTIFIER ARROW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 272 */ "xx_mcall_expr ::= IDENTIFIER ARROW IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 273 */ "xx_mcall_expr ::= IDENTIFIER ARROW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 274 */ "xx_mcall_expr ::= IDENTIFIER ARROW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 275 */ "xx_mcall_expr ::= IDENTIFIER ARROW BRACKET_OPEN STRING BRACKET_CLOSE PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 276 */ "xx_mcall_expr ::= IDENTIFIER ARROW BRACKET_OPEN STRING BRACKET_CLOSE PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 277 */ "xx_common_expr ::= xx_mcall_expr",
 /* 278 */ "xx_common_expr ::= xx_scall_expr",
 /* 279 */ "xx_common_expr ::= xx_fcall_expr",
 /* 280 */ "xx_call_parameters ::= xx_call_parameters COMMA xx_call_parameter",
 /* 281 */ "xx_call_parameters ::= xx_call_parameter",
 /* 282 */ "xx_call_parameter ::= xx_common_expr",
 /* 283 */ "xx_array_list ::= xx_array_list COMMA xx_array_item",
 /* 284 */ "xx_array_list ::= xx_array_item",
 /* 285 */ "xx_array_item ::= xx_array_key COLON xx_array_value",
 /* 286 */ "xx_array_item ::= xx_array_value",
 /* 287 */ "xx_array_key ::= IDENTIFIER",
 /* 288 */ "xx_array_key ::= STRING",
 /* 289 */ "xx_array_key ::= INTEGER",
 /* 290 */ "xx_array_value ::= xx_common_expr",
 /* 291 */ "xx_literal_expr ::= INTEGER",
 /* 292 */ "xx_literal_expr ::= CHAR",
 /* 293 */ "xx_literal_expr ::= STRING",
 /* 294 */ "xx_literal_expr ::= DOUBLE",
 /* 295 */ "xx_literal_expr ::= NULL",
 /* 296 */ "xx_literal_expr ::= FALSE",
 /* 297 */ "xx_literal_expr ::= TRUE",
 /* 298 */ "xx_literal_expr ::= IDENTIFIER DOUBLECOLON CONSTANT",
 /* 299 */ "xx_isset_expr ::= IDENTIFIER SBRACKET_OPEN xx_common_expr SBRACKET_CLOSE",
 /* 300 */ "xx_isset_expr ::= IDENTIFIER ARROW IDENTIFIER",
 /* 301 */ "xx_isset_expr ::= IDENTIFIER ARROW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE",
 /* 302 */ "xx_isset_expr ::= IDENTIFIER ARROW BRACKET_OPEN STRING BRACKET_CLOSE",
 /* 303 */ "xx_eval_expr ::= xx_common_expr",
 /* 304 */ "xx_comment ::= COMMENT",
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
// 2635 "parser.c"
      break;
    case 101:
// 967 "parser.lemon"
{ json_object_put((yypminor->yy277)); }
// 2640 "parser.c"
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
  { 156, 5 },
  { 156, 5 },
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
// 3162 "parser.c"
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
      case 249:
      case 277:
      case 278:
      case 279:
      case 282:
      case 290:
      case 303:
// 969 "parser.lemon"
{
	yygotominor.yy277 = yymsp[0].minor.yy277;
}
// 3204 "parser.c"
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
// 3218 "parser.c"
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
      case 281:
      case 284:
// 977 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_list(NULL, yymsp[0].minor.yy277);
}
// 3240 "parser.c"
        break;
      case 8:
// 997 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_namespace(yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(33,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3249 "parser.c"
        break;
      case 9:
// 1001 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_interface(yymsp[-1].minor.yy0, yymsp[0].minor.yy277, NULL, status->scanner_state);
  yy_destructor(36,&yymsp[-2].minor);
}
// 3257 "parser.c"
        break;
      case 10:
// 1005 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_interface(yymsp[-3].minor.yy0, yymsp[0].minor.yy277, yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(36,&yymsp[-4].minor);
  yy_destructor(37,&yymsp[-2].minor);
}
// 3266 "parser.c"
        break;
      case 11:
// 1009 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class(yymsp[-1].minor.yy0, yymsp[0].minor.yy277, 0, 0, NULL, NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-2].minor);
}
// 3274 "parser.c"
        break;
      case 12:
// 1013 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy277, 0, 0, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-4].minor);
  yy_destructor(37,&yymsp[-2].minor);
}
// 3283 "parser.c"
        break;
      case 13:
// 1017 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy277, 0, 0, NULL, yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(38,&yymsp[-4].minor);
  yy_destructor(39,&yymsp[-2].minor);
}
// 3292 "parser.c"
        break;
      case 14:
// 1021 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class(yymsp[-5].minor.yy0, yymsp[0].minor.yy277, 0, 0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(38,&yymsp[-6].minor);
  yy_destructor(37,&yymsp[-4].minor);
  yy_destructor(39,&yymsp[-2].minor);
}
// 3302 "parser.c"
        break;
      case 15:
// 1025 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class(yymsp[-1].minor.yy0, yymsp[0].minor.yy277, 1, 0, NULL, NULL, status->scanner_state);
  yy_destructor(40,&yymsp[-3].minor);
  yy_destructor(38,&yymsp[-2].minor);
}
// 3311 "parser.c"
        break;
      case 16:
// 1029 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy277, 1, 0, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(40,&yymsp[-5].minor);
  yy_destructor(38,&yymsp[-4].minor);
  yy_destructor(37,&yymsp[-2].minor);
}
// 3321 "parser.c"
        break;
      case 17:
// 1033 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy277, 1, 0, NULL, yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(40,&yymsp[-5].minor);
  yy_destructor(38,&yymsp[-4].minor);
  yy_destructor(39,&yymsp[-2].minor);
}
// 3331 "parser.c"
        break;
      case 18:
// 1037 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class(yymsp[-1].minor.yy0, yymsp[0].minor.yy277, 0, 1, NULL, NULL, status->scanner_state);
  yy_destructor(41,&yymsp[-3].minor);
  yy_destructor(38,&yymsp[-2].minor);
}
// 3340 "parser.c"
        break;
      case 19:
// 1041 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy277, 0, 1, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(41,&yymsp[-5].minor);
  yy_destructor(38,&yymsp[-4].minor);
  yy_destructor(37,&yymsp[-2].minor);
}
// 3350 "parser.c"
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
// 3361 "parser.c"
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
// 3372 "parser.c"
        break;
      case 22:
      case 48:
      case 96:
      case 165:
      case 190:
      case 212:
      case 280:
      case 283:
// 1053 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_list(yymsp[-2].minor.yy277, yymsp[0].minor.yy277);
  yy_destructor(6,&yymsp[-1].minor);
}
// 3387 "parser.c"
        break;
      case 24:
      case 252:
      case 287:
// 1061 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state);
}
// 3396 "parser.c"
        break;
      case 27:
// 1073 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class_definition(yymsp[0].minor.yy277, NULL, NULL, status->scanner_state);
}
// 3403 "parser.c"
        break;
      case 28:
// 1077 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class_definition(NULL, NULL, yymsp[0].minor.yy277, status->scanner_state);
}
// 3410 "parser.c"
        break;
      case 29:
// 1081 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class_definition(NULL, yymsp[0].minor.yy277, NULL, status->scanner_state);
}
// 3417 "parser.c"
        break;
      case 30:
// 1085 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class_definition(yymsp[-1].minor.yy277, yymsp[0].minor.yy277, NULL, status->scanner_state);
}
// 3424 "parser.c"
        break;
      case 31:
// 1089 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class_definition(yymsp[-1].minor.yy277, NULL, yymsp[0].minor.yy277, status->scanner_state);
}
// 3431 "parser.c"
        break;
      case 32:
// 1093 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class_definition(yymsp[0].minor.yy277, NULL, yymsp[-1].minor.yy277, status->scanner_state);
}
// 3438 "parser.c"
        break;
      case 33:
// 1097 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class_definition(NULL, yymsp[0].minor.yy277, yymsp[-1].minor.yy277, status->scanner_state);
}
// 3445 "parser.c"
        break;
      case 34:
// 1101 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class_definition(yymsp[-2].minor.yy277, yymsp[0].minor.yy277, yymsp[-1].minor.yy277, status->scanner_state);
}
// 3452 "parser.c"
        break;
      case 35:
// 1105 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class_definition(yymsp[-1].minor.yy277, yymsp[0].minor.yy277, yymsp[-2].minor.yy277, status->scanner_state);
}
// 3459 "parser.c"
        break;
      case 38:
// 1122 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class_property(yymsp[-2].minor.yy277, yymsp[-1].minor.yy0, NULL, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(35,&yymsp[0].minor);
}
// 3467 "parser.c"
        break;
      case 39:
// 1126 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class_property(yymsp[-2].minor.yy277, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(35,&yymsp[0].minor);
}
// 3475 "parser.c"
        break;
      case 40:
// 1130 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class_property(yymsp[-4].minor.yy277, yymsp[-3].minor.yy0, yymsp[-1].minor.yy277, yymsp[-5].minor.yy0, NULL, status->scanner_state);
  yy_destructor(45,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3484 "parser.c"
        break;
      case 41:
// 1134 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class_property(yymsp[-4].minor.yy277, yymsp[-3].minor.yy0, yymsp[-1].minor.yy277, NULL, NULL, status->scanner_state);
  yy_destructor(45,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3493 "parser.c"
        break;
      case 42:
// 1138 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class_property(yymsp[-3].minor.yy277, yymsp[-2].minor.yy0, NULL, yymsp[-4].minor.yy0, yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(35,&yymsp[0].minor);
}
// 3501 "parser.c"
        break;
      case 43:
// 1142 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class_property(yymsp[-3].minor.yy277, yymsp[-2].minor.yy0, NULL, NULL, yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(35,&yymsp[0].minor);
}
// 3509 "parser.c"
        break;
      case 44:
// 1146 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class_property(yymsp[-5].minor.yy277, yymsp[-4].minor.yy0, yymsp[-2].minor.yy277, yymsp[-6].minor.yy0, yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(45,&yymsp[-3].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3518 "parser.c"
        break;
      case 45:
// 1150 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class_property(yymsp[-5].minor.yy277, yymsp[-4].minor.yy0, yymsp[-2].minor.yy277, NULL, yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(45,&yymsp[-3].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3527 "parser.c"
        break;
      case 50:
// 1170 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_property_shortcut(NULL, yymsp[0].minor.yy0, status->scanner_state);
}
// 3534 "parser.c"
        break;
      case 51:
// 1174 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_property_shortcut(yymsp[-1].minor.yy0, yymsp[0].minor.yy0, status->scanner_state);
}
// 3541 "parser.c"
        break;
      case 58:
// 1203 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy277, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(46,&yymsp[-4].minor);
  yy_destructor(45,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3551 "parser.c"
        break;
      case 59:
// 1207 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy277, NULL, status->scanner_state);
  yy_destructor(46,&yymsp[-4].minor);
  yy_destructor(45,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3561 "parser.c"
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
// 3573 "parser.c"
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
// 3585 "parser.c"
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
// 3597 "parser.c"
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
// 3609 "parser.c"
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
// 3621 "parser.c"
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
// 3633 "parser.c"
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
// 3645 "parser.c"
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
// 3657 "parser.c"
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
// 3670 "parser.c"
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
// 3683 "parser.c"
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
// 3696 "parser.c"
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
// 3709 "parser.c"
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
// 3722 "parser.c"
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
// 3735 "parser.c"
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
// 3748 "parser.c"
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
// 3761 "parser.c"
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
// 3773 "parser.c"
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
// 3785 "parser.c"
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
// 3797 "parser.c"
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
// 3809 "parser.c"
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
// 3820 "parser.c"
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
// 3831 "parser.c"
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
// 3842 "parser.c"
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
// 3853 "parser.c"
        break;
      case 86:
// 1318 "parser.lemon"
{
	yygotominor.yy277 = json_object_new_string("public");
  yy_destructor(1,&yymsp[0].minor);
}
// 3861 "parser.c"
        break;
      case 87:
// 1322 "parser.lemon"
{
	yygotominor.yy277 = json_object_new_string("protected");
  yy_destructor(2,&yymsp[0].minor);
}
// 3869 "parser.c"
        break;
      case 88:
// 1326 "parser.lemon"
{
	yygotominor.yy277 = json_object_new_string("private");
  yy_destructor(4,&yymsp[0].minor);
}
// 3877 "parser.c"
        break;
      case 89:
// 1330 "parser.lemon"
{
	yygotominor.yy277 = json_object_new_string("static");
  yy_destructor(3,&yymsp[0].minor);
}
// 3885 "parser.c"
        break;
      case 90:
// 1334 "parser.lemon"
{
	yygotominor.yy277 = json_object_new_string("scoped");
  yy_destructor(5,&yymsp[0].minor);
}
// 3893 "parser.c"
        break;
      case 91:
// 1338 "parser.lemon"
{
	yygotominor.yy277 = json_object_new_string("inline");
  yy_destructor(51,&yymsp[0].minor);
}
// 3901 "parser.c"
        break;
      case 92:
// 1342 "parser.lemon"
{
	yygotominor.yy277 = json_object_new_string("abstract");
  yy_destructor(40,&yymsp[0].minor);
}
// 3909 "parser.c"
        break;
      case 93:
// 1346 "parser.lemon"
{
	yygotominor.yy277 = json_object_new_string("final");
  yy_destructor(41,&yymsp[0].minor);
}
// 3917 "parser.c"
        break;
      case 98:
// 1368 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_parameter(NULL, NULL, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 3924 "parser.c"
        break;
      case 99:
// 1372 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_parameter(yymsp[-1].minor.yy277, NULL, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 3931 "parser.c"
        break;
      case 100:
// 1376 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_parameter(NULL, yymsp[-1].minor.yy277, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 3938 "parser.c"
        break;
      case 101:
// 1380 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_parameter(NULL, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy277, status->scanner_state);
  yy_destructor(45,&yymsp[-1].minor);
}
// 3946 "parser.c"
        break;
      case 102:
// 1384 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_parameter(yymsp[-3].minor.yy277, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy277, status->scanner_state);
  yy_destructor(45,&yymsp[-1].minor);
}
// 3954 "parser.c"
        break;
      case 103:
// 1388 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_parameter(NULL, yymsp[-3].minor.yy277, yymsp[-2].minor.yy0, yymsp[0].minor.yy277, status->scanner_state);
  yy_destructor(45,&yymsp[-1].minor);
}
// 3962 "parser.c"
        break;
      case 104:
// 1392 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(15,&yymsp[-2].minor);
  yy_destructor(16,&yymsp[0].minor);
}
// 3971 "parser.c"
        break;
      case 105:
// 1396 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_type(XX_TYPE_INTEGER);
  yy_destructor(52,&yymsp[0].minor);
}
// 3979 "parser.c"
        break;
      case 106:
// 1400 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_type(XX_TYPE_UINTEGER);
  yy_destructor(53,&yymsp[0].minor);
}
// 3987 "parser.c"
        break;
      case 107:
// 1404 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_type(XX_TYPE_LONG);
  yy_destructor(54,&yymsp[0].minor);
}
// 3995 "parser.c"
        break;
      case 108:
// 1408 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_type(XX_TYPE_ULONG);
  yy_destructor(55,&yymsp[0].minor);
}
// 4003 "parser.c"
        break;
      case 109:
// 1412 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_type(XX_TYPE_CHAR);
  yy_destructor(56,&yymsp[0].minor);
}
// 4011 "parser.c"
        break;
      case 110:
// 1416 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_type(XX_TYPE_UCHAR);
  yy_destructor(57,&yymsp[0].minor);
}
// 4019 "parser.c"
        break;
      case 111:
// 1420 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_type(XX_TYPE_DOUBLE);
  yy_destructor(58,&yymsp[0].minor);
}
// 4027 "parser.c"
        break;
      case 112:
// 1424 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_type(XX_TYPE_BOOL);
  yy_destructor(59,&yymsp[0].minor);
}
// 4035 "parser.c"
        break;
      case 113:
// 1428 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_type(XX_TYPE_STRING);
  yy_destructor(60,&yymsp[0].minor);
}
// 4043 "parser.c"
        break;
      case 114:
// 1432 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_type(XX_TYPE_ARRAY);
  yy_destructor(61,&yymsp[0].minor);
}
// 4051 "parser.c"
        break;
      case 115:
// 1436 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_type(XX_TYPE_VAR);
  yy_destructor(62,&yymsp[0].minor);
}
// 4059 "parser.c"
        break;
      case 138:
// 1528 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_empty_statement(status->scanner_state);
  yy_destructor(35,&yymsp[0].minor);
}
// 4067 "parser.c"
        break;
      case 139:
// 1532 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_break_statement(status->scanner_state);
  yy_destructor(63,&yymsp[-1].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4076 "parser.c"
        break;
      case 140:
// 1536 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_continue_statement(status->scanner_state);
  yy_destructor(64,&yymsp[-1].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4085 "parser.c"
        break;
      case 141:
// 1540 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_if_statement(yymsp[-2].minor.yy277, NULL, NULL, status->scanner_state);
  yy_destructor(65,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-1].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 4095 "parser.c"
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
// 4108 "parser.c"
        break;
      case 143:
// 1548 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_if_statement(yymsp[-3].minor.yy277, yymsp[-1].minor.yy277, NULL, status->scanner_state);
  yy_destructor(65,&yymsp[-4].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 4118 "parser.c"
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
// 4131 "parser.c"
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
// 4144 "parser.c"
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
// 4157 "parser.c"
        break;
      case 147:
// 1564 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_switch_statement(yymsp[-2].minor.yy277, NULL, status->scanner_state);
  yy_destructor(67,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-1].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 4167 "parser.c"
        break;
      case 148:
// 1568 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_switch_statement(yymsp[-3].minor.yy277, yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(67,&yymsp[-4].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 4177 "parser.c"
        break;
      case 151:
// 1580 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_case_clause(yymsp[-1].minor.yy277, NULL, status->scanner_state);
  yy_destructor(68,&yymsp[-2].minor);
  yy_destructor(69,&yymsp[0].minor);
}
// 4186 "parser.c"
        break;
      case 152:
// 1584 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_case_clause(yymsp[-2].minor.yy277, yymsp[0].minor.yy277, status->scanner_state);
  yy_destructor(68,&yymsp[-3].minor);
  yy_destructor(69,&yymsp[-1].minor);
}
// 4195 "parser.c"
        break;
      case 153:
// 1588 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_case_clause(NULL, yymsp[0].minor.yy277, status->scanner_state);
  yy_destructor(70,&yymsp[-2].minor);
  yy_destructor(69,&yymsp[-1].minor);
}
// 4204 "parser.c"
        break;
      case 154:
// 1592 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_loop_statement(NULL, status->scanner_state);
  yy_destructor(71,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[-1].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 4214 "parser.c"
        break;
      case 155:
// 1596 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_loop_statement(yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(71,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 4224 "parser.c"
        break;
      case 156:
// 1600 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_while_statement(yymsp[-2].minor.yy277, NULL, status->scanner_state);
  yy_destructor(72,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-1].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 4234 "parser.c"
        break;
      case 157:
// 1604 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_while_statement(yymsp[-3].minor.yy277, yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(72,&yymsp[-4].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 4244 "parser.c"
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
// 4256 "parser.c"
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
// 4268 "parser.c"
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
// 4279 "parser.c"
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
// 4291 "parser.c"
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
// 4303 "parser.c"
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
// 4316 "parser.c"
        break;
      case 164:
// 1632 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_let_statement(yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(77,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4325 "parser.c"
        break;
      case 167:
// 1645 "parser.lemon"
{
	yygotominor.yy277 = json_object_new_string("assign");
  yy_destructor(45,&yymsp[0].minor);
}
// 4333 "parser.c"
        break;
      case 168:
// 1650 "parser.lemon"
{
	yygotominor.yy277 = json_object_new_string("add-assign");
  yy_destructor(78,&yymsp[0].minor);
}
// 4341 "parser.c"
        break;
      case 169:
// 1655 "parser.lemon"
{
	yygotominor.yy277 = json_object_new_string("sub-assign");
  yy_destructor(79,&yymsp[0].minor);
}
// 4349 "parser.c"
        break;
      case 170:
// 1659 "parser.lemon"
{
	yygotominor.yy277 = json_object_new_string("mult-assign");
  yy_destructor(80,&yymsp[0].minor);
}
// 4357 "parser.c"
        break;
      case 171:
// 1663 "parser.lemon"
{
	yygotominor.yy277 = json_object_new_string("div-assign");
  yy_destructor(81,&yymsp[0].minor);
}
// 4365 "parser.c"
        break;
      case 172:
// 1667 "parser.lemon"
{
	yygotominor.yy277 = json_object_new_string("concat-assign");
  yy_destructor(82,&yymsp[0].minor);
}
// 4373 "parser.c"
        break;
      case 173:
// 1672 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_let_assignment("variable", yymsp[-1].minor.yy277, yymsp[-2].minor.yy0, NULL, NULL, yymsp[0].minor.yy277, status->scanner_state);
}
// 4380 "parser.c"
        break;
      case 174:
// 1677 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_let_assignment("object-property", yymsp[-1].minor.yy277, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, yymsp[0].minor.yy277, status->scanner_state);
  yy_destructor(50,&yymsp[-3].minor);
}
// 4388 "parser.c"
        break;
      case 175:
// 1682 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_let_assignment("variable-dynamic-object-property", yymsp[-1].minor.yy277, yymsp[-6].minor.yy0, yymsp[-3].minor.yy0, NULL, yymsp[0].minor.yy277, status->scanner_state);
  yy_destructor(50,&yymsp[-5].minor);
  yy_destructor(42,&yymsp[-4].minor);
  yy_destructor(43,&yymsp[-2].minor);
}
// 4398 "parser.c"
        break;
      case 176:
// 1687 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_let_assignment("string-dynamic-object-property", yymsp[-1].minor.yy277, yymsp[-6].minor.yy0, yymsp[-3].minor.yy0, NULL, yymsp[0].minor.yy277, status->scanner_state);
  yy_destructor(50,&yymsp[-5].minor);
  yy_destructor(42,&yymsp[-4].minor);
  yy_destructor(43,&yymsp[-2].minor);
}
// 4408 "parser.c"
        break;
      case 177:
// 1692 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_let_assignment("object-property-append", yymsp[-1].minor.yy277, yymsp[-6].minor.yy0, yymsp[-4].minor.yy0, NULL, yymsp[0].minor.yy277, status->scanner_state);
  yy_destructor(50,&yymsp[-5].minor);
  yy_destructor(84,&yymsp[-3].minor);
  yy_destructor(85,&yymsp[-2].minor);
}
// 4418 "parser.c"
        break;
      case 178:
// 1697 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_let_assignment("object-property-array-index", yymsp[-1].minor.yy277, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, yymsp[-3].minor.yy277, yymsp[0].minor.yy277, status->scanner_state);
  yy_destructor(50,&yymsp[-6].minor);
  yy_destructor(84,&yymsp[-4].minor);
  yy_destructor(85,&yymsp[-2].minor);
}
// 4428 "parser.c"
        break;
      case 179:
// 1702 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_let_assignment("static-property", yymsp[-1].minor.yy277, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, yymsp[0].minor.yy277, status->scanner_state);
  yy_destructor(86,&yymsp[-3].minor);
}
// 4436 "parser.c"
        break;
      case 180:
// 1707 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_let_assignment("static-property-append", yymsp[-1].minor.yy277, yymsp[-6].minor.yy0, yymsp[-4].minor.yy0, NULL, yymsp[0].minor.yy277, status->scanner_state);
  yy_destructor(86,&yymsp[-5].minor);
  yy_destructor(84,&yymsp[-3].minor);
  yy_destructor(85,&yymsp[-2].minor);
}
// 4446 "parser.c"
        break;
      case 181:
// 1712 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_let_assignment("static-property-array-index", yymsp[-1].minor.yy277, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, yymsp[-3].minor.yy277, yymsp[0].minor.yy277, status->scanner_state);
  yy_destructor(86,&yymsp[-6].minor);
  yy_destructor(84,&yymsp[-4].minor);
  yy_destructor(85,&yymsp[-2].minor);
}
// 4456 "parser.c"
        break;
      case 182:
// 1717 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_let_assignment("variable-append", yymsp[-1].minor.yy277, yymsp[-4].minor.yy0, NULL, NULL, yymsp[0].minor.yy277, status->scanner_state);
  yy_destructor(84,&yymsp[-3].minor);
  yy_destructor(85,&yymsp[-2].minor);
}
// 4465 "parser.c"
        break;
      case 183:
// 1722 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_let_assignment("array-index", yymsp[-1].minor.yy277, yymsp[-5].minor.yy0, NULL, yymsp[-3].minor.yy277, yymsp[0].minor.yy277, status->scanner_state);
  yy_destructor(84,&yymsp[-4].minor);
  yy_destructor(85,&yymsp[-2].minor);
}
// 4474 "parser.c"
        break;
      case 184:
// 1727 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_let_assignment("incr", NULL, yymsp[-3].minor.yy0, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(50,&yymsp[-2].minor);
  yy_destructor(87,&yymsp[0].minor);
}
// 4483 "parser.c"
        break;
      case 185:
// 1732 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_let_assignment("decr", NULL, yymsp[-3].minor.yy0, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(50,&yymsp[-2].minor);
  yy_destructor(88,&yymsp[0].minor);
}
// 4492 "parser.c"
        break;
      case 186:
// 1737 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_let_assignment("incr", NULL, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(87,&yymsp[0].minor);
}
// 4500 "parser.c"
        break;
      case 187:
// 1742 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_let_assignment("decr", NULL, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(88,&yymsp[0].minor);
}
// 4508 "parser.c"
        break;
      case 189:
// 1750 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_echo_statement(yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(89,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4517 "parser.c"
        break;
      case 192:
// 1762 "parser.lemon"
{
	yygotominor.yy277 = yymsp[0].minor.yy277;;
}
// 4524 "parser.c"
        break;
      case 193:
// 1767 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_mcall_statement(yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(35,&yymsp[0].minor);
}
// 4532 "parser.c"
        break;
      case 194:
// 1772 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_fcall_statement(yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(35,&yymsp[0].minor);
}
// 4540 "parser.c"
        break;
      case 195:
// 1777 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_scall_statement(yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(35,&yymsp[0].minor);
}
// 4548 "parser.c"
        break;
      case 196:
// 1782 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_fetch_statement(yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(35,&yymsp[0].minor);
}
// 4556 "parser.c"
        break;
      case 197:
// 1787 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_return_statement(yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(90,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4565 "parser.c"
        break;
      case 198:
// 1792 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_return_statement(NULL, status->scanner_state);
  yy_destructor(90,&yymsp[-1].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4574 "parser.c"
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
// 4588 "parser.c"
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
// 4603 "parser.c"
        break;
      case 201:
// 1815 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_throw_exception(yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(92,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4612 "parser.c"
        break;
      case 202:
// 1819 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_declare_statement(XX_T_TYPE_INTEGER, yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(52,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4621 "parser.c"
        break;
      case 203:
// 1823 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_declare_statement(XX_T_TYPE_UINTEGER, yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(53,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4630 "parser.c"
        break;
      case 204:
// 1827 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_declare_statement(XX_T_TYPE_CHAR, yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(56,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4639 "parser.c"
        break;
      case 205:
// 1831 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_declare_statement(XX_T_TYPE_UCHAR, yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(57,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4648 "parser.c"
        break;
      case 206:
// 1835 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_declare_statement(XX_T_TYPE_LONG, yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4657 "parser.c"
        break;
      case 207:
// 1839 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_declare_statement(XX_T_TYPE_ULONG, yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(55,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4666 "parser.c"
        break;
      case 208:
// 1843 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_declare_statement(XX_T_TYPE_DOUBLE, yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(58,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4675 "parser.c"
        break;
      case 209:
// 1847 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_declare_statement(XX_T_TYPE_STRING, yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(60,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4684 "parser.c"
        break;
      case 210:
// 1851 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_declare_statement(XX_T_TYPE_BOOL, yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(59,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4693 "parser.c"
        break;
      case 211:
// 1855 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_declare_statement(XX_T_TYPE_VAR, yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(62,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4702 "parser.c"
        break;
      case 214:
// 1867 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_declare_variable(yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 4709 "parser.c"
        break;
      case 215:
// 1871 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_declare_variable(yymsp[-2].minor.yy0, yymsp[0].minor.yy277, status->scanner_state);
  yy_destructor(45,&yymsp[-1].minor);
}
// 4717 "parser.c"
        break;
      case 217:
// 1879 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_expr("not", yymsp[0].minor.yy277, NULL, NULL, status->scanner_state);
  yy_destructor(31,&yymsp[-1].minor);
}
// 4725 "parser.c"
        break;
      case 218:
// 1883 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_expr("isset", yymsp[0].minor.yy277, NULL, NULL, status->scanner_state);
  yy_destructor(27,&yymsp[-1].minor);
}
// 4733 "parser.c"
        break;
      case 219:
// 1887 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_expr("require", yymsp[0].minor.yy277, NULL, NULL, status->scanner_state);
  yy_destructor(7,&yymsp[-1].minor);
}
// 4741 "parser.c"
        break;
      case 220:
// 1891 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_expr("clone", yymsp[0].minor.yy277, NULL, NULL, status->scanner_state);
  yy_destructor(8,&yymsp[-1].minor);
}
// 4749 "parser.c"
        break;
      case 221:
// 1895 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_expr("empty", yymsp[0].minor.yy277, NULL, NULL, status->scanner_state);
  yy_destructor(9,&yymsp[-1].minor);
}
// 4757 "parser.c"
        break;
      case 222:
// 1899 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_expr("equals", yymsp[-2].minor.yy277, yymsp[0].minor.yy277, NULL, status->scanner_state);
  yy_destructor(13,&yymsp[-1].minor);
}
// 4765 "parser.c"
        break;
      case 223:
// 1903 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_expr("not-equals", yymsp[-2].minor.yy277, yymsp[0].minor.yy277, NULL, status->scanner_state);
  yy_destructor(20,&yymsp[-1].minor);
}
// 4773 "parser.c"
        break;
      case 224:
// 1907 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_expr("identical", yymsp[-2].minor.yy277, yymsp[0].minor.yy277, NULL, status->scanner_state);
  yy_destructor(14,&yymsp[-1].minor);
}
// 4781 "parser.c"
        break;
      case 225:
// 1911 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_expr("not-identical", yymsp[-2].minor.yy277, yymsp[0].minor.yy277, NULL, status->scanner_state);
  yy_destructor(19,&yymsp[-1].minor);
}
// 4789 "parser.c"
        break;
      case 226:
// 1915 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_expr("less", yymsp[-2].minor.yy277, yymsp[0].minor.yy277, NULL, status->scanner_state);
  yy_destructor(15,&yymsp[-1].minor);
}
// 4797 "parser.c"
        break;
      case 227:
// 1919 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_expr("greater", yymsp[-2].minor.yy277, yymsp[0].minor.yy277, NULL, status->scanner_state);
  yy_destructor(16,&yymsp[-1].minor);
}
// 4805 "parser.c"
        break;
      case 228:
// 1923 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_expr("less-equal", yymsp[-2].minor.yy277, yymsp[0].minor.yy277, NULL, status->scanner_state);
  yy_destructor(17,&yymsp[-1].minor);
}
// 4813 "parser.c"
        break;
      case 229:
// 1927 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_expr("greater-equal", yymsp[-2].minor.yy277, yymsp[0].minor.yy277, NULL, status->scanner_state);
  yy_destructor(18,&yymsp[-1].minor);
}
// 4821 "parser.c"
        break;
      case 230:
// 1931 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_expr("list", yymsp[-1].minor.yy277, NULL, NULL, status->scanner_state);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 4830 "parser.c"
        break;
      case 231:
// 1935 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_expr("cast", yymsp[-2].minor.yy277, yymsp[0].minor.yy277, NULL, status->scanner_state);
  yy_destructor(49,&yymsp[-3].minor);
  yy_destructor(32,&yymsp[-1].minor);
}
// 4839 "parser.c"
        break;
      case 232:
// 1939 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_expr("type-hint", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), yymsp[0].minor.yy277, NULL, status->scanner_state);
  yy_destructor(15,&yymsp[-3].minor);
  yy_destructor(16,&yymsp[-1].minor);
}
// 4848 "parser.c"
        break;
      case 233:
      case 300:
// 1943 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_expr("property-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(50,&yymsp[-1].minor);
}
// 4857 "parser.c"
        break;
      case 234:
      case 301:
// 1947 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_expr("property-dynamic-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(50,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 4868 "parser.c"
        break;
      case 235:
      case 302:
// 1951 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_expr("property-string-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_STRING, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(50,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 4879 "parser.c"
        break;
      case 236:
// 1955 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_expr("static-property-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(86,&yymsp[-1].minor);
}
// 4887 "parser.c"
        break;
      case 237:
      case 298:
// 1959 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_expr("static-constant-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(86,&yymsp[-1].minor);
}
// 4896 "parser.c"
        break;
      case 238:
      case 299:
// 1964 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), yymsp[-1].minor.yy277, NULL, status->scanner_state);
  yy_destructor(84,&yymsp[-2].minor);
  yy_destructor(85,&yymsp[0].minor);
}
// 4906 "parser.c"
        break;
      case 239:
// 1969 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_expr("add", yymsp[-2].minor.yy277, yymsp[0].minor.yy277, NULL, status->scanner_state);
  yy_destructor(21,&yymsp[-1].minor);
}
// 4914 "parser.c"
        break;
      case 240:
// 1974 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_expr("sub", yymsp[-2].minor.yy277, yymsp[0].minor.yy277, NULL, status->scanner_state);
  yy_destructor(22,&yymsp[-1].minor);
}
// 4922 "parser.c"
        break;
      case 241:
// 1979 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_expr("mul", yymsp[-2].minor.yy277, yymsp[0].minor.yy277, NULL, status->scanner_state);
  yy_destructor(24,&yymsp[-1].minor);
}
// 4930 "parser.c"
        break;
      case 242:
// 1984 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_expr("div", yymsp[-2].minor.yy277, yymsp[0].minor.yy277, NULL, status->scanner_state);
  yy_destructor(25,&yymsp[-1].minor);
}
// 4938 "parser.c"
        break;
      case 243:
// 1989 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_expr("mod", yymsp[-2].minor.yy277, yymsp[0].minor.yy277, NULL, status->scanner_state);
  yy_destructor(26,&yymsp[-1].minor);
}
// 4946 "parser.c"
        break;
      case 244:
// 1994 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_expr("concat", yymsp[-2].minor.yy277, yymsp[0].minor.yy277, NULL, status->scanner_state);
  yy_destructor(23,&yymsp[-1].minor);
}
// 4954 "parser.c"
        break;
      case 245:
// 1999 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_expr("and", yymsp[-2].minor.yy277, yymsp[0].minor.yy277, NULL, status->scanner_state);
  yy_destructor(11,&yymsp[-1].minor);
}
// 4962 "parser.c"
        break;
      case 246:
// 2004 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_expr("or", yymsp[-2].minor.yy277, yymsp[0].minor.yy277, NULL, status->scanner_state);
  yy_destructor(12,&yymsp[-1].minor);
}
// 4970 "parser.c"
        break;
      case 247:
// 2009 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_expr("instanceof", yymsp[-2].minor.yy277, yymsp[0].minor.yy277, NULL, status->scanner_state);
  yy_destructor(10,&yymsp[-1].minor);
}
// 4978 "parser.c"
        break;
      case 248:
// 2014 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_expr("fetch", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), yymsp[0].minor.yy277, NULL, status->scanner_state);
  yy_destructor(28,&yymsp[-3].minor);
  yy_destructor(6,&yymsp[-1].minor);
}
// 4987 "parser.c"
        break;
      case 250:
// 2024 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_expr("typeof", yymsp[0].minor.yy277, NULL, NULL, status->scanner_state);
  yy_destructor(29,&yymsp[-1].minor);
}
// 4995 "parser.c"
        break;
      case 251:
// 2029 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_literal(XX_T_CONSTANT, yymsp[0].minor.yy0, status->scanner_state);
}
// 5002 "parser.c"
        break;
      case 253:
      case 289:
      case 291:
// 2039 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_literal(XX_T_INTEGER, yymsp[0].minor.yy0, status->scanner_state);
}
// 5011 "parser.c"
        break;
      case 254:
      case 288:
      case 293:
// 2044 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_literal(XX_T_STRING, yymsp[0].minor.yy0, status->scanner_state);
}
// 5020 "parser.c"
        break;
      case 255:
      case 292:
// 2049 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_literal(XX_T_CHAR, yymsp[0].minor.yy0, status->scanner_state);
}
// 5028 "parser.c"
        break;
      case 256:
      case 294:
// 2054 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_literal(XX_T_DOUBLE, yymsp[0].minor.yy0, status->scanner_state);
}
// 5036 "parser.c"
        break;
      case 257:
      case 295:
// 2059 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_literal(XX_T_NULL, NULL, status->scanner_state);
  yy_destructor(96,&yymsp[0].minor);
}
// 5045 "parser.c"
        break;
      case 258:
      case 297:
// 2064 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_literal(XX_T_TRUE, NULL, status->scanner_state);
  yy_destructor(97,&yymsp[0].minor);
}
// 5054 "parser.c"
        break;
      case 259:
      case 296:
// 2069 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_literal(XX_T_FALSE, NULL, status->scanner_state);
  yy_destructor(98,&yymsp[0].minor);
}
// 5063 "parser.c"
        break;
      case 260:
// 2074 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_expr("empty-array", NULL, NULL, NULL, status->scanner_state);
  yy_destructor(84,&yymsp[-1].minor);
  yy_destructor(85,&yymsp[0].minor);
}
// 5072 "parser.c"
        break;
      case 261:
// 2079 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_expr("array", yymsp[-1].minor.yy277, NULL, NULL, status->scanner_state);
  yy_destructor(84,&yymsp[-2].minor);
  yy_destructor(85,&yymsp[0].minor);
}
// 5081 "parser.c"
        break;
      case 262:
// 2084 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_new_instance(yymsp[0].minor.yy0, NULL, status->scanner_state);
  yy_destructor(30,&yymsp[-1].minor);
}
// 5089 "parser.c"
        break;
      case 263:
// 2089 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_new_instance(yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(30,&yymsp[-3].minor);
  yy_destructor(49,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 5099 "parser.c"
        break;
      case 264:
// 2094 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_new_instance(yymsp[-3].minor.yy0, yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(30,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 5109 "parser.c"
        break;
      case 265:
// 2099 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_fcall(1, yymsp[-3].minor.yy0, yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 5118 "parser.c"
        break;
      case 266:
// 2104 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_fcall(1, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(49,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 5127 "parser.c"
        break;
      case 267:
// 2109 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_fcall(2, yymsp[-4].minor.yy0, yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(42,&yymsp[-5].minor);
  yy_destructor(43,&yymsp[-3].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 5138 "parser.c"
        break;
      case 268:
// 2114 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_fcall(2, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(42,&yymsp[-4].minor);
  yy_destructor(43,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 5149 "parser.c"
        break;
      case 269:
// 2119 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_scall(yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(86,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 5159 "parser.c"
        break;
      case 270:
// 2124 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_scall(yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(86,&yymsp[-3].minor);
  yy_destructor(49,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 5169 "parser.c"
        break;
      case 271:
// 2129 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_mcall(1, yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(50,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 5179 "parser.c"
        break;
      case 272:
// 2134 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_mcall(1, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(50,&yymsp[-3].minor);
  yy_destructor(49,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 5189 "parser.c"
        break;
      case 273:
// 2139 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_mcall(2, yymsp[-7].minor.yy0, yymsp[-4].minor.yy0, yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(50,&yymsp[-6].minor);
  yy_destructor(42,&yymsp[-5].minor);
  yy_destructor(43,&yymsp[-3].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 5201 "parser.c"
        break;
      case 274:
// 2144 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_mcall(2, yymsp[-6].minor.yy0, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(50,&yymsp[-5].minor);
  yy_destructor(42,&yymsp[-4].minor);
  yy_destructor(43,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 5213 "parser.c"
        break;
      case 275:
// 2149 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_mcall(3, yymsp[-7].minor.yy0, yymsp[-4].minor.yy0, yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(50,&yymsp[-6].minor);
  yy_destructor(42,&yymsp[-5].minor);
  yy_destructor(43,&yymsp[-3].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 5225 "parser.c"
        break;
      case 276:
// 2154 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_mcall(3, yymsp[-6].minor.yy0, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(50,&yymsp[-5].minor);
  yy_destructor(42,&yymsp[-4].minor);
  yy_destructor(43,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 5237 "parser.c"
        break;
      case 285:
// 2193 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_array_item(yymsp[-2].minor.yy277, yymsp[0].minor.yy277, status->scanner_state);
  yy_destructor(69,&yymsp[-1].minor);
}
// 5245 "parser.c"
        break;
      case 286:
// 2197 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_array_item(NULL, yymsp[0].minor.yy277, status->scanner_state);
}
// 5252 "parser.c"
        break;
      case 304:
// 2273 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_comment(yymsp[0].minor.yy0, status->scanner_state);
}
// 5259 "parser.c"
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

// 5335 "parser.c"
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
