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


// 881 "parser.c"
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
#define YYNSTATE 669
#define YYNRULE 312
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
 /*     0 */   198,  201,  204,    3,    4,    5,    6,    7,  240,  982,
 /*    10 */     1,    2,  668,    4,    5,    6,    7,   68,   72,   54,
 /*    20 */   194,  249,  254,  270,  191,  115,  101,  244,  117,  637,
 /*    30 */   643,  642,  626,  120,  111,  277,  647,  636,  592,  119,
 /*    40 */   256,   98,  234,  102,  102,   35,   36,   37,   38,   39,
 /*    50 */    40,   41,   42,   43,   44,   45,  232,  228,  230,  193,
 /*    60 */   203,  208,  210,  212,  214,  206,  200,  216,  218,  226,
 /*    70 */   220,  222,  224,  220,  222,  224,  258,  264,  249,  280,
 /*    80 */   584,  447,   30,   31,  497,  150,  257,  259,  260,  261,
 /*    90 */   262,  263,  277,  530,  507,  549,   52,  462,  469,   68,
 /*   100 */    72,   76,  461,  470,  479,  482,  473,  476,  485,  491,
 /*   110 */   488,   49,  494,  151,  153,  155,  288,  164,  352,  249,
 /*   120 */   503,  175,  179,  184,  361,  497,  150,  368,  556,   65,
 /*   130 */    30,   31,  538,  277,  576,  507,  316,  245,  399,  430,
 /*   140 */   445,  449,  458,  461,  470,  479,  482,  473,  476,  485,
 /*   150 */   491,  488,  197,  494,  151,  153,  155,  638,  164,  641,
 /*   160 */   642,  626,  175,  179,  184,  361,  591,  592,  368,  505,
 /*   170 */   249,  313,  102,  267,  464,  181,  497,  150,   60,  464,
 /*   180 */   430,  445,  449,  458,  277,  580,  507,  403,   58,   59,
 /*   190 */    61,   62,   64,   63,  461,  470,  479,  482,  473,  476,
 /*   200 */   485,  491,  488,  463,  494,  151,  153,  155,  472,  164,
 /*   210 */   112,  249,  248,  175,  179,  184,  361,  497,  150,  368,
 /*   220 */   284,  286,  285,  253,  464,  277,  574,  507,  307,  287,
 /*   230 */   374,  430,  445,  449,  458,  461,  470,  479,  482,  473,
 /*   240 */   476,  485,  491,  488,   53,  494,  151,  153,  155,  620,
 /*   250 */   164,  471,  469,  475,  175,  179,  184,  361,  615,  621,
 /*   260 */   368,   34,  249,  375,  376,  377,  378,  379,  497,  150,
 /*   270 */    97,  464,  430,  445,  449,  458,  277,  513,  507,  216,
 /*   280 */   218,  226,  220,  222,  224,  618,  461,  470,  479,  482,
 /*   290 */   473,  476,  485,  491,  488,  611,  494,  151,  153,  155,
 /*   300 */   478,  164,  614,  249,  248,  175,  179,  184,  361,  497,
 /*   310 */   150,  368,  284,  286,  285,  253,  421,  277,  367,  507,
 /*   320 */   339,  287,  617,  430,  445,  449,  458,  461,  470,  479,
 /*   330 */   482,  473,  476,  485,  491,  488,   53,  494,  151,  153,
 /*   340 */   155,  451,  164,  639,  604,  626,  175,  179,  184,  361,
 /*   350 */   276,  592,  368,  649,  249,  647,  102,  316,  245,  336,
 /*   360 */   497,  150,   48,  233,  430,  445,  449,  458,  277,  537,
 /*   370 */   507,  284,  286,  285,  253,  454,  275,  963,  461,  470,
 /*   380 */   479,  482,  473,  476,  485,  491,  488,  392,  494,  151,
 /*   390 */   153,  155,  313,  164,  267,  249,  248,  175,  179,  184,
 /*   400 */   361,  497,  150,  368,  284,  286,  285,  253,  342,  277,
 /*   410 */   520,  507,  281,  287,   12,  430,  445,  449,  458,  461,
 /*   420 */   470,  479,  482,  473,  476,  485,  491,  488,  271,  494,
 /*   430 */   151,  153,  155,  238,  164,  590,  302,  120,  175,  179,
 /*   440 */   184,  361,  591,  592,  368,  107,  249,  648,  102,  655,
 /*   450 */   113,  658,  497,  150,  113,  202,  430,  445,  449,  458,
 /*   460 */   277,  525,  507,  284,  286,  285,  253,  656,  474,  469,
 /*   470 */   461,  470,  479,  482,  473,  476,  485,  491,  488,   14,
 /*   480 */   494,  151,  153,  155,   17,  164,  170,  249,  248,  175,
 /*   490 */   179,  184,  361,  497,  150,  368,  284,  286,  285,  253,
 /*   500 */   464,  277,  126,  507,  274,  287,  464,  430,  445,  449,
 /*   510 */   458,  461,  470,  479,  482,  473,  476,  485,  491,  488,
 /*   520 */   276,  494,  151,  153,  155,  572,  164,   30,   31,  481,
 /*   530 */   175,  179,  184,  361,  169,  484,  368,   96,  249,  316,
 /*   540 */   500,   68,   72,   76,  497,  150,  282,  446,  430,  445,
 /*   550 */   449,  458,  277,  542,  507,  284,  286,  285,  253,  171,
 /*   560 */   297,  532,  461,  470,  479,  482,  473,  476,  485,  491,
 /*   570 */   488,  631,  494,  151,  153,  155,  498,  164,  614,  249,
 /*   580 */   248,  175,  179,  184,  361,  497,  150,  368,  284,  286,
 /*   590 */   285,  253,  464,  277,  541,  507,  332,  287,  464,  430,
 /*   600 */   445,  449,  458,  461,  470,  479,  482,  473,  476,  485,
 /*   610 */   491,  488,  276,  494,  151,  153,  155,   50,  164,   30,
 /*   620 */    31,  487,  175,  179,  184,  361,  167,  490,  368,  577,
 /*   630 */   249,  477,  469,   68,   72,   76,  497,  150,  292,  207,
 /*   640 */   430,  445,  449,  458,  277,  546,  507,  284,  286,  285,
 /*   650 */   253,  171,   87,  532,  461,  470,  479,  482,  473,  476,
 /*   660 */   485,  491,  488,  102,  494,  151,  153,  155,  644,  164,
 /*   670 */   533,  249,  248,  175,  179,  184,  361,  497,  150,  368,
 /*   680 */   284,  286,  285,  253,  464,  277,  548,  507,  309,  287,
 /*   690 */   450,  430,  445,  449,  458,  461,  470,  479,  482,  473,
 /*   700 */   476,  485,  491,  488,  113,  494,  151,  153,  155,   28,
 /*   710 */   164,   30,   31,  493,  175,  179,  184,  361,  480,  469,
 /*   720 */   368,   47,  249,  483,  469,   68,   72,   76,  497,  150,
 /*   730 */   268,  217,  430,  445,  449,  458,  277,  552,  507,  284,
 /*   740 */   286,  285,  253,  312,  486,  469,  461,  470,  479,  482,
 /*   750 */   473,  476,  485,  491,  488,  389,  494,  151,  153,  155,
 /*   760 */   659,  164,  647,  249,  248,  175,  179,  184,  361,  497,
 /*   770 */   150,  368,  284,  286,  285,  253,  296,  277,  187,  507,
 /*   780 */   345,  287,  464,  430,  445,  449,  458,  461,  470,  479,
 /*   790 */   482,  473,  476,  485,  491,  488,  276,  494,  151,  153,
 /*   800 */   155,   95,  164,  608,  489,  469,  175,  179,  184,  361,
 /*   810 */   614,  496,  368,  609,  249,   99,   92,   30,   31,  295,
 /*   820 */   497,  150,  299,  459,  430,  445,  449,  458,  277,  526,
 /*   830 */   507,  284,  286,  285,  253,  529,  492,  469,  461,  470,
 /*   840 */   479,  482,  473,  476,  485,  491,  488,  665,  494,  151,
 /*   850 */   153,  155,  113,  164,  385,  249,  248,  175,  179,  184,
 /*   860 */   361,  497,  150,  368,  284,  286,  285,  253,  433,  277,
 /*   870 */   128,  507,  291,  287,  527,  430,  445,  449,  458,  461,
 /*   880 */   470,  479,  482,  473,  476,  485,  491,  488,   53,  494,
 /*   890 */   151,  153,  155,  646,  164,  628,   22,  432,  175,  179,
 /*   900 */   184,  361,  614,   21,  368,  629,  249,  102,   93,  585,
 /*   910 */    30,   31,  497,  150,  550,  209,  430,  445,  449,  458,
 /*   920 */   277,  159,  507,  284,  286,  285,  253,  166,  495,  469,
 /*   930 */   461,  470,  479,  482,  473,  476,  485,  491,  488,  424,
 /*   940 */   494,  151,  153,  155,  965,  164,  113,  249,  248,  175,
 /*   950 */   179,  184,  361,  497,  150,  368,  284,  286,  285,  253,
 /*   960 */   425,  277,  536,  507,  317,  287,   77,  430,  445,  449,
 /*   970 */   458,  461,  470,  479,  482,  473,  476,  485,  491,  488,
 /*   980 */   276,  494,  151,  153,  155,  544,  164,   30,   31,  501,
 /*   990 */   175,  179,  184,  361,  168,  535,  368,  502,  249,  110,
 /*  1000 */   616,  623,  113,  355,  497,  150,  308,  219,  430,  445,
 /*  1010 */   449,  458,  277,  554,  507,  284,  286,  285,  253,  325,
 /*  1020 */   562,   46,  461,  470,  479,  482,  473,  476,  485,  491,
 /*  1030 */   488,  102,  494,  151,  153,  155,   27,  164,   89,  249,
 /*  1040 */   248,  175,  179,  184,  361,  497,  150,  368,  284,  286,
 /*  1050 */   285,  253,  357,  277,  588,  507,  298,  287,  185,  430,
 /*  1060 */   445,  449,  458,  461,  470,  479,  482,  473,  476,  485,
 /*  1070 */   491,  488,  246,  494,  151,  153,  155,  964,  164,  289,
 /*  1080 */   335,  384,  175,  179,  184,  361,  551,  294,  368,  398,
 /*  1090 */   249,  369,  429,  241,  555,  669,  497,  150,  324,  510,
 /*  1100 */   430,  445,  449,  458,  277,  586,  507,  284,  286,  285,
 /*  1110 */   253,  516,  606,  242,  461,  470,  479,  482,  473,  476,
 /*  1120 */   485,  491,  488,  102,  494,  151,  153,  155,   90,  164,
 /*  1130 */   290,  249,  248,  175,  179,  184,  361,  497,  150,  368,
 /*  1140 */   284,  286,  285,  253,  515,  277,  558,  507,   25,  301,
 /*  1150 */   657,  430,  445,  449,  458,  461,  470,  479,  482,  473,
 /*  1160 */   476,  485,  491,  488,  276,  494,  151,  153,  155,   66,
 /*  1170 */   164,  514,  595,  620,  175,  179,  184,  361,  567,  125,
 /*  1180 */   368,  247,  249,  621,  438,  596,  571,  543,  497,  150,
 /*  1190 */   310,  349,  430,  445,  449,  458,  277,  183,  507,  284,
 /*  1200 */   286,  285,  253,  109,  597,  598,  461,  470,  479,  482,
 /*  1210 */   473,  476,  485,  491,  488,   53,  494,  151,  153,  155,
 /*  1220 */   188,  164,  344,  249,  314,  175,  179,  184,  361,  497,
 /*  1230 */   150,  368,  284,  286,  285,  253,  371,  277,  531,  507,
 /*  1240 */   363,  578,   57,  430,  445,  449,  458,  461,  470,  479,
 /*  1250 */   482,  473,  476,  485,  491,  488,  579,  494,  151,  153,
 /*  1260 */   155,  506,  164,  600,  583,  370,  175,  179,  184,  361,
 /*  1270 */   353,  276,  368,  161,  249,  338,  602,  601,  354,  603,
 /*  1280 */   497,  150,  328,  211,  430,  445,  449,  458,  277,  560,
 /*  1290 */   507,  284,  286,  285,  253,  160,  504,  318,  461,  470,
 /*  1300 */   479,  482,  473,  476,  485,  491,  488,  276,  494,  151,
 /*  1310 */   153,  155,  440,  164,  190,  249,  243,  175,  179,  184,
 /*  1320 */   361,  497,  150,  368,  284,  286,  285,  253,  100,  277,
 /*  1330 */   163,  507,  250,  333,  269,  430,  445,  449,  458,  461,
 /*  1340 */   470,  479,  482,  473,  476,  485,  491,  488,  276,  494,
 /*  1350 */   151,  153,  155,  624,  164,  499,  251,  442,  175,  179,
 /*  1360 */   184,  361,  610,  380,  368,  612,  249,  613,   74,  453,
 /*  1370 */    24,  466,  497,  150,  340,  221,  430,  445,  449,  458,
 /*  1380 */   277,  582,  507,  284,  286,  285,  253,   33,  279,  715,
 /*  1390 */   461,  470,  479,  482,  473,  476,  485,  491,  488,  276,
 /*  1400 */   494,  151,  153,  155,  660,  164,  157,  249,  231,  175,
 /*  1410 */   179,  184,  361,  497,  150,  368,  284,  286,  285,  253,
 /*  1420 */   716,  277,  178,  507,  278,  346,  356,  430,  445,  449,
 /*  1430 */   458,  461,  470,  479,  482,  473,  476,  485,  491,  488,
 /*  1440 */   619,  494,  151,  153,  155,  196,  164,  457,  195,  337,
 /*  1450 */   175,  179,  184,  361,  622,  122,  368,  625,  249,   56,
 /*  1460 */   444,  662,  108,  634,  497,  150,   51,  517,  430,  445,
 /*  1470 */   449,  458,  277,  568,  507,  284,  286,  285,  253,   73,
 /*  1480 */   630,  663,  461,  470,  479,  482,  473,  476,  485,  491,
 /*  1490 */   488,  632,  494,  151,  153,  155,  343,  164,  372,  249,
 /*  1500 */   229,  175,  179,  184,  361,  497,  150,  368,  284,  286,
 /*  1510 */   285,  253,  633,  277,  570,  507,  373,  123,    9,  430,
 /*  1520 */   445,  449,  458,  461,  470,  479,  482,  473,  476,  485,
 /*  1530 */   491,  488,  635,  494,  151,  153,  155,  154,  164,  452,
 /*  1540 */   272,  106,  175,  179,  184,  361,  252,  664,  368,  152,
 /*  1550 */   249,  105,  358,   94,  468,  645,  497,  150,  467,  223,
 /*  1560 */   430,  445,  449,  458,  277,  667,  507,  284,  286,  285,
 /*  1570 */   253,   71,  237,   15,  461,  470,  479,  482,  473,  476,
 /*  1580 */   485,  491,  488,  362,  494,  151,  153,  155,  465,  164,
 /*  1590 */   148,  666,   70,  175,  179,  184,  361,  650,  235,  368,
 /*  1600 */   652,   78,   79,   81,   80,   82,  284,  286,  285,  253,
 /*  1610 */   186,  430,  445,  449,  458,  176,  508,  130,  131,  132,
 /*  1620 */   133,  134,  135,  136,  137,  138,  139,  140,  141,  142,
 /*  1630 */   143,  144,  145,  146,  147,  149,  148,  557,  408,  409,
 /*  1640 */    84,   85,   29,  114,  605,  405,  599,  437,  439,  441,
 /*  1650 */   443,   83,  413,  404,  305,  416,  519,  653,  564,  304,
 /*  1660 */   565,   16,  508,  130,  131,  132,  133,  134,  135,  136,
 /*  1670 */   137,  138,  139,  140,  141,  142,  143,  144,  145,  146,
 /*  1680 */   147,  149,  148,  401,  303,   67,   10,  545,   69,  400,
 /*  1690 */   116,  173,  417,  437,  439,  441,  443,   78,   79,   81,
 /*  1700 */    80,   82,  569,   17,  172,  654,   13,  573,  508,  130,
 /*  1710 */   131,  132,  133,  134,  135,  136,  137,  138,  139,  140,
 /*  1720 */   141,  142,  143,  144,  145,  146,  147,  149,  148,  539,
 /*  1730 */   607,   75,  491,   19,  491,  393,   84,   85,   22,  437,
 /*  1740 */   439,  441,  443,  374,  563,  103,  491,   83,  587,  102,
 /*  1750 */   491,  491,  491,  491,  508,  130,  131,  132,  133,  134,
 /*  1760 */   135,  136,  137,  138,  139,  140,  141,  142,  143,  144,
 /*  1770 */   145,  146,  147,  149,  148,  491,  375,  376,  377,  378,
 /*  1780 */   379,  491,  411,  491,  491,  437,  439,  441,  443,   78,
 /*  1790 */    79,   81,   80,   82,  182,  491,  491,  491,  491,  491,
 /*  1800 */   508,  130,  131,  132,  133,  134,  135,  136,  137,  138,
 /*  1810 */   139,  140,  141,  142,  143,  144,  145,  146,  147,  149,
 /*  1820 */   148,  491,  627,  491,  491,  491,  491,  491,   84,   85,
 /*  1830 */   491,  437,  439,  441,  443,  491,  121,  491,  491,   83,
 /*  1840 */   534,  491,  491,  491,  491,  491,  508,  130,  131,  132,
 /*  1850 */   133,  134,  135,  136,  137,  138,  139,  140,  141,  142,
 /*  1860 */   143,  144,  145,  146,  147,  149,  148,  491,  491,  491,
 /*  1870 */   491,  491,  491,  491,  491,  491,  491,  437,  439,  441,
 /*  1880 */   443,   78,   79,   81,   80,   82,  553,  491,  491,  491,
 /*  1890 */   491,  491,  508,  130,  131,  132,  133,  134,  135,  136,
 /*  1900 */   137,  138,  139,  140,  141,  142,  143,  144,  145,  146,
 /*  1910 */   147,  149,  148,  491,  491,  491,  491,  491,  491,  491,
 /*  1920 */    84,   85,  491,  437,  439,  441,  443,  491,   88,  491,
 /*  1930 */   491,   83,  540,  491,  491,  491,  491,  491,  508,  130,
 /*  1940 */   131,  132,  133,  134,  135,  136,  137,  138,  139,  140,
 /*  1950 */   141,  142,  143,  144,  145,  146,  147,  149,  148,  491,
 /*  1960 */   491,  491,  491,  491,  491,  491,  491,  491,  491,  437,
 /*  1970 */   439,  441,  443,   78,   79,   81,   80,   82,  581,  491,
 /*  1980 */   491,  491,  491,  491,  508,  130,  131,  132,  133,  134,
 /*  1990 */   135,  136,  137,  138,  139,  140,  141,  142,  143,  144,
 /*  2000 */   145,  146,  147,  149,  148,  491,  491,  491,  491,  491,
 /*  2010 */   491,  491,   84,   85,  491,  437,  439,  441,  443,  491,
 /*  2020 */   563,  491,  491,   83,  162,  491,  491,  491,  491,  491,
 /*  2030 */   508,  130,  131,  132,  133,  134,  135,  136,  137,  138,
 /*  2040 */   139,  140,  141,  142,  143,  144,  145,  146,  147,  149,
 /*  2050 */   148,  491,  491,  491,  491,  491,  491,  491,  491,  491,
 /*  2060 */   491,  437,  439,  441,  443,   78,   79,   81,   80,   82,
 /*  2070 */   127,  491,  491,  491,  491,  491,  508,  130,  131,  132,
 /*  2080 */   133,  134,  135,  136,  137,  138,  139,  140,  141,  142,
 /*  2090 */   143,  144,  145,  146,  147,  149,  148,  491,  491,  491,
 /*  2100 */   491,  491,  491,  491,   84,   85,  491,  437,  439,  441,
 /*  2110 */   443,  491,  121,  491,  491,   83,  547,  491,  491,  491,
 /*  2120 */   491,  491,  508,  130,  131,  132,  133,  134,  135,  136,
 /*  2130 */   137,  138,  139,  140,  141,  142,  143,  144,  145,  146,
 /*  2140 */   147,  149,  148,  491,  491,  491,  491,  491,  491,  491,
 /*  2150 */   491,  491,  491,  437,  439,  441,  443,   78,   79,   81,
 /*  2160 */    80,   82,  524,  491,  491,  491,  491,  491,  508,  130,
 /*  2170 */   131,  132,  133,  134,  135,  136,  137,  138,  139,  140,
 /*  2180 */   141,  142,  143,  144,  145,  146,  147,  149,  148,  491,
 /*  2190 */   491,  491,  491,  491,  491,  491,   84,   85,  491,  437,
 /*  2200 */   439,  441,  443,  491,   23,  491,  491,   83,  174,  491,
 /*  2210 */   491,  491,  491,  491,  508,  130,  131,  132,  133,  134,
 /*  2220 */   135,  136,  137,  138,  139,  140,  141,  142,  143,  144,
 /*  2230 */   145,  146,  147,  149,  148,  491,  491,  491,  491,  491,
 /*  2240 */   491,  491,  491,  491,  491,  437,  439,  441,  443,   78,
 /*  2250 */    79,   81,   80,   82,  575,  491,  491,  491,  491,  491,
 /*  2260 */   508,  130,  131,  132,  133,  134,  135,  136,  137,  138,
 /*  2270 */   139,  140,  141,  142,  143,  144,  145,  146,  147,  149,
 /*  2280 */   148,  491,  491,  491,  180,  491,  491,  360,   84,   85,
 /*  2290 */   491,  437,  439,  441,  443,  284,  286,  285,  253,   83,
 /*  2300 */   559,  491,  491,  491,  491,  491,  508,  130,  131,  132,
 /*  2310 */   133,  134,  135,  136,  137,  138,  139,  140,  141,  142,
 /*  2320 */   143,  144,  145,  146,  147,  149,  148,  491,  491,  491,
 /*  2330 */   491,  491,  491,  589,  118,  604,  626,  437,  439,  441,
 /*  2340 */   443,  636,  592,  491,  491,  491,  512,  102,  491,  491,
 /*  2350 */   491,  491,  508,  130,  131,  132,  133,  134,  135,  136,
 /*  2360 */   137,  138,  139,  140,  141,  142,  143,  144,  145,  146,
 /*  2370 */   147,  149,  148,  491,  491,  491,  156,  491,  491,  360,
 /*  2380 */   491,  491,  491,  437,  439,  441,  443,  284,  286,  285,
 /*  2390 */   253,  491,  158,  491,  491,  491,  491,  491,  508,  130,
 /*  2400 */   131,  132,  133,  134,  135,  136,  137,  138,  139,  140,
 /*  2410 */   141,  142,  143,  144,  145,  146,  147,  149,  148,  491,
 /*  2420 */   491,  491,  189,  491,  491,  360,  491,  491,  491,  437,
 /*  2430 */   439,  441,  443,  284,  286,  285,  253,  491,  366,  491,
 /*  2440 */   491,  491,  491,  491,  508,  130,  131,  132,  133,  134,
 /*  2450 */   135,  136,  137,  138,  139,  140,  141,  142,  143,  144,
 /*  2460 */   145,  146,  147,  149,  148,  491,  491,  491,  491,  491,
 /*  2470 */   491,  491,  435,  491,  491,  437,  439,  441,  443,  434,
 /*  2480 */   284,  286,  285,  253,  177,  491,  491,  491,  491,  491,
 /*  2490 */   508,  130,  131,  132,  133,  134,  135,  136,  137,  138,
 /*  2500 */   139,  140,  141,  142,  143,  144,  145,  146,  147,  149,
 /*  2510 */   148,  491,  491,  491,  491,  491,  491,  491,  491,  435,
 /*  2520 */   491,  437,  439,  441,  443,  431,  436,  284,  286,  285,
 /*  2530 */   253,  491,  491,  491,  491,  491,  129,  130,  131,  132,
 /*  2540 */   133,  134,  135,  136,  137,  138,  139,  140,  141,  142,
 /*  2550 */   143,  144,  145,  146,  147,  149,  198,  201,  204,  491,
 /*  2560 */   165,  491,  491,  360,  240,  491,  491,  437,  439,  441,
 /*  2570 */   443,  284,  286,  285,  253,  491,  194,  249,  254,  270,
 /*  2580 */   191,  491,  491,  244,  198,  201,  204,  491,  491,  491,
 /*  2590 */   491,  277,  240,  491,  491,  491,  256,  491,  234,  491,
 /*  2600 */   491,  491,  491,  491,  194,  249,  254,  270,  191,  491,
 /*  2610 */   491,  244,  491,  491,  198,  201,  204,  491,  491,  277,
 /*  2620 */   265,  491,  240,  491,  256,  509,  234,  491,  284,  286,
 /*  2630 */   285,  253,  258,  264,  194,  249,  254,  270,  191,  293,
 /*  2640 */   329,  244,  257,  259,  260,  261,  262,  263,  491,  277,
 /*  2650 */   491,  491,  491,  521,  256,  491,  234,  198,  201,  204,
 /*  2660 */   258,  264,  491,  491,  491,  240,  491,  491,  491,  491,
 /*  2670 */   257,  259,  260,  261,  262,  263,  491,  194,  249,  254,
 /*  2680 */   270,  191,  347,  491,  244,  491,  198,  201,  204,  491,
 /*  2690 */   258,  264,  277,  491,  240,  491,  491,  256,  491,  234,
 /*  2700 */   257,  259,  260,  261,  262,  263,  194,  249,  254,  270,
 /*  2710 */   191,  319,  491,  244,  491,  198,  201,  204,  491,  491,
 /*  2720 */   491,  277,  491,  240,  491,  491,  256,  491,  234,  491,
 /*  2730 */   491,  491,  491,  258,  264,  194,  249,  254,  270,  191,
 /*  2740 */   491,  491,  244,  257,  259,  260,  261,  262,  263,  491,
 /*  2750 */   277,  491,  491,  491,  491,  256,  491,  234,  491,  491,
 /*  2760 */   491,  491,  258,  264,  491,  491,  491,  491,  491,  491,
 /*  2770 */   491,  491,  257,  259,  260,  261,  262,  263,  491,  374,
 /*  2780 */   491,  491,  491,  491,  383,  491,  491,  491,  491,  395,
 /*  2790 */   491,  258,  264,  388,  423,  491,  491,  284,  286,  285,
 /*  2800 */   253,  257,  259,  260,  261,  262,  263,  491,  491,  198,
 /*  2810 */   201,  204,  375,  376,  377,  378,  379,  240,  419,  491,
 /*  2820 */   407,  427,  428,  491,  491,  491,  491,  491,  491,  194,
 /*  2830 */   249,  254,  270,  191,  283,  491,  244,  198,  201,  204,
 /*  2840 */   670,  491,  491,  491,  277,  240,  491,  491,  491,  256,
 /*  2850 */   491,  234,  491,  491,  491,  491,  491,  194,  249,  254,
 /*  2860 */   270,  191,  491,  491,  244,  448,  491,  198,  201,  204,
 /*  2870 */   491,  491,  277,    8,  491,  240,   11,  256,  104,  234,
 /*  2880 */   651,  661,  491,  491,  507,  258,  264,  194,  249,  254,
 /*  2890 */   270,  191,  334,  491,  244,  257,  259,  260,  261,  262,
 /*  2900 */   263,  491,  277,  491,  491,  491,  491,  256,  491,  234,
 /*  2910 */   198,  201,  204,  258,  264,  491,  491,  491,  240,  491,
 /*  2920 */   491,  491,  491,  257,  259,  260,  261,  262,  263,  491,
 /*  2930 */   194,  249,  254,  270,  191,  491,  491,  244,  491,  198,
 /*  2940 */   201,  204,  491,  258,  264,  277,  491,  240,  491,  491,
 /*  2950 */   256,  491,  234,  257,  259,  260,  261,  262,  263,  194,
 /*  2960 */   249,  254,  270,  191,  341,  491,  244,  382,  491,  491,
 /*  2970 */   491,  422,  491,  491,  277,  284,  286,  285,  253,  256,
 /*  2980 */   491,  234,  198,  201,  204,  491,  258,  264,  412,  491,
 /*  2990 */   240,  491,  491,  491,  491,  491,  257,  259,  260,  261,
 /*  3000 */   262,  263,  194,  249,  254,  270,  191,  273,  491,  244,
 /*  3010 */   491,  198,  201,  204,  491,  258,  264,  277,  491,  240,
 /*  3020 */   491,  491,  256,  491,  234,  257,  259,  260,  261,  262,
 /*  3030 */   263,  194,  249,  254,  270,  191,  491,  491,  244,  491,
 /*  3040 */   198,  201,  204,  491,  491,  491,  277,  491,  240,  491,
 /*  3050 */   491,  256,  491,  234,  491,  491,  491,  491,  258,  264,
 /*  3060 */   194,  249,  254,  270,  191,  300,  491,  244,  257,  259,
 /*  3070 */   260,  261,  262,  263,  491,  277,  491,  491,  491,  491,
 /*  3080 */   256,  491,  234,  198,  201,  204,  491,  258,  264,  420,
 /*  3090 */   491,  240,  491,  491,  491,  491,  491,  257,  259,  260,
 /*  3100 */   261,  262,  263,  194,  249,  254,  270,  191,  491,  491,
 /*  3110 */   266,  491,  198,  201,  204,  491,  258,  264,  277,  491,
 /*  3120 */   240,  491,  491,  256,  491,  234,  257,  259,  260,  261,
 /*  3130 */   262,  263,  194,  249,  254,  270,  191,  306,  491,  244,
 /*  3140 */   382,  491,  491,  491,  394,  491,  491,  277,  284,  286,
 /*  3150 */   285,  253,  256,  491,  234,  198,  201,  204,  491,  321,
 /*  3160 */   264,  322,  491,  240,  491,  491,  491,  491,  491,  320,
 /*  3170 */   259,  260,  261,  262,  263,  194,  249,  254,  270,  191,
 /*  3180 */   311,  491,  244,  491,  198,  201,  204,  491,  258,  264,
 /*  3190 */   277,  491,  240,  491,  491,  256,  491,  234,  257,  259,
 /*  3200 */   260,  261,  262,  263,  194,  249,  254,  270,  191,  491,
 /*  3210 */   491,  244,  491,  198,  201,  204,  491,  491,  491,  277,
 /*  3220 */   491,  240,  491,  491,  256,  491,  234,  491,  491,  491,
 /*  3230 */   491,  258,  264,  194,  249,  254,  270,  191,  491,  491,
 /*  3240 */   266,  257,  259,  260,  261,  262,  263,  491,  277,  491,
 /*  3250 */   491,  491,  491,  256,  395,  234,  491,  491,  265,  415,
 /*  3260 */   258,  264,  284,  286,  285,  253,  284,  286,  285,  253,
 /*  3270 */   257,  259,  260,  261,  262,  263,  326,  327,  330,  491,
 /*  3280 */   491,  491,  491,  491,  491,  491,  491,  491,  491,  321,
 /*  3290 */   264,   78,   79,   81,   80,   82,  491,  522,  491,  320,
 /*  3300 */   259,  260,  261,  262,  263,  284,  286,  285,  253,  491,
 /*  3310 */   232,  228,  230,  193,  203,  208,  210,  212,  214,  206,
 /*  3320 */   200,  216,  218,  226,  220,  222,  224,  491,  491,  265,
 /*  3330 */    84,   85,  491,   32,  491,  491,  594,  284,  286,  285,
 /*  3340 */   253,   83,  518,  491,  323,  491,  491,  331,  327,  330,
 /*  3350 */   566,  491,   55,  232,  228,  230,  193,  203,  208,  210,
 /*  3360 */   212,  214,  206,  200,  216,  218,  226,  220,  222,  224,
 /*  3370 */    35,   36,   37,   38,   39,   40,   41,   42,   43,   44,
 /*  3380 */    45,  491,  491,  382,  491,  511,  491,  418,  491,  491,
 /*  3390 */   491,  284,  286,  285,  253,  491,  232,  228,  230,  193,
 /*  3400 */   203,  208,  210,  212,  214,  206,  200,  216,  218,  226,
 /*  3410 */   220,  222,  224,  232,  228,  230,  193,  203,  208,  210,
 /*  3420 */   212,  214,  206,  200,  216,  218,  226,  220,  222,  224,
 /*  3430 */   232,  228,  230,  193,  203,  208,  210,  212,  214,  206,
 /*  3440 */   200,  216,  218,  226,  220,  222,  224,  374,  491,  491,
 /*  3450 */   491,  491,  236,  232,  228,  230,  193,  203,  208,  210,
 /*  3460 */   212,  214,  206,  200,  216,  218,  226,  220,  222,  224,
 /*  3470 */   491,  315,  491,  491,  491,  491,  491,  491,  491,  491,
 /*  3480 */   375,  376,  377,  378,  379,  523,  387,  491,  857,  396,
 /*  3490 */   397,  491,  491,  491,  491,  491,  232,  228,  230,  193,
 /*  3500 */   203,  208,  210,  212,  214,  206,  200,  216,  218,  226,
 /*  3510 */   220,  222,  224,  193,  203,  208,  210,  212,  214,  206,
 /*  3520 */   200,  216,  218,  226,  220,  222,  224,  382,  365,  491,
 /*  3530 */   491,  402,  491,  491,  491,  284,  286,  285,  253,  232,
 /*  3540 */   228,  230,  193,  203,  208,  210,  212,  214,  206,  200,
 /*  3550 */   216,  218,  226,  220,  222,  224,  491,  491,  491,  491,
 /*  3560 */   491,  491,  491,  491,  460,  232,  228,  230,  193,  203,
 /*  3570 */   208,  210,  212,  214,  206,  200,  216,  218,  226,  220,
 /*  3580 */   222,  224,  232,  228,  230,  193,  203,  208,  210,  212,
 /*  3590 */   214,  206,  200,  216,  218,  226,  220,  222,  224,  232,
 /*  3600 */   228,  230,  193,  203,  208,  210,  212,  214,  206,  200,
 /*  3610 */   216,  218,  226,  220,  222,  224,  228,  230,  193,  203,
 /*  3620 */   208,  210,  212,  214,  206,  200,  216,  218,  226,  220,
 /*  3630 */   222,  224,   32,  491,  491,  491,  491,  382,  491,  491,
 /*  3640 */   351,  414,  491,   32,  491,  284,  286,  285,  253,  124,
 /*  3650 */   491,   55,  491,  491,  491,  491,  491,  456,  491,  491,
 /*  3660 */    26,  491,   55,  491,   78,   79,   81,   80,   82,   35,
 /*  3670 */    36,   37,   38,   39,   40,   41,   42,   43,   44,   45,
 /*  3680 */    35,   36,   37,   38,   39,   40,   41,   42,   43,   44,
 /*  3690 */    45,   32,  491,  491,  491,  491,  491,  491,  491,  491,
 /*  3700 */   491,  491,   32,   84,   85,  491,  700,  593,   91,  599,
 /*  3710 */    55,  491,    8,  491,   83,   11,  491,  104,   32,  651,
 /*  3720 */   661,   55,  491,  507,  491,  491,  491,  491,   35,   36,
 /*  3730 */    37,   38,   39,   40,   41,   42,   43,   44,   45,   35,
 /*  3740 */    36,   37,   38,   39,   40,   41,   42,   43,   44,   45,
 /*  3750 */    78,   79,   81,   80,   82,   35,   36,   37,   38,   39,
 /*  3760 */    40,   41,   42,   43,   44,   45,   78,   79,   81,   80,
 /*  3770 */    82,  382,  491,  528,  491,  410,  360,  491,  491,  284,
 /*  3780 */   286,  285,  253,  491,  284,  286,  285,  253,  239,   84,
 /*  3790 */    85,  491,  697,  605,  491,  599,  284,  286,  285,  253,
 /*  3800 */    83,  491,  491,  491,  227,   84,   85,  491,  696,  605,
 /*  3810 */   491,  599,  284,  286,  285,  253,   83,  491,   78,   79,
 /*  3820 */    81,   80,   82,   78,   79,   81,   80,   82,  382,  491,
 /*  3830 */   491,  491,  406,  491,  491,  491,  284,  286,  285,  253,
 /*  3840 */   382,  491,  491,  491,  381,  225,  491,  491,  284,  286,
 /*  3850 */   285,  253,  491,  284,  286,  285,  253,   84,   85,  491,
 /*  3860 */   703,  561,   84,   85,  491,   20,   86,  491,   83,  491,
 /*  3870 */   491,  491,  491,   83,  491,   78,   79,   81,   80,   82,
 /*  3880 */    78,   79,   81,   80,   82,  382,  491,  491,  491,  426,
 /*  3890 */   491,  491,  491,  284,  286,  285,  253,  382,  491,  491,
 /*  3900 */   491,  386,  348,  491,  491,  284,  286,  285,  253,  491,
 /*  3910 */   284,  286,  285,  253,   84,   85,  491,  701,  640,   84,
 /*  3920 */    85,  491,  699,  561,  491,   83,  491,  491,  491,  491,
 /*  3930 */    83,  491,   78,   79,   81,   80,   82,   78,   79,   81,
 /*  3940 */    80,   82,  395,  491,  491,  491,  491,  391,  491,  491,
 /*  3950 */   284,  286,  285,  253,  491,  491,  491,  382,  491,  491,
 /*  3960 */   491,  390,  491,  491,  491,  284,  286,  285,  253,  491,
 /*  3970 */   491,   84,   85,  491,   18,   86,   84,   85,  491,  698,
 /*  3980 */   561,  491,   83,  491,  491,  491,  491,   83,  491,   78,
 /*  3990 */    79,   81,   80,   82,   78,   79,   81,   80,   82,  491,
 /*  4000 */   491,  359,  491,  491,  491,  455,  491,  491,  491,  284,
 /*  4010 */   286,  285,  253,  284,  286,  285,  253,  491,  491,  491,
 /*  4020 */   491,  491,  491,  491,  491,  491,  491,  491,   84,   85,
 /*  4030 */   491,  704,  561,   84,   85,  491,  702,  561,  255,   83,
 /*  4040 */   491,  491,  213,  491,   83,  491,  284,  286,  285,  253,
 /*  4050 */   284,  286,  285,  253,  192,  491,  491,  491,  364,  491,
 /*  4060 */   491,  491,  284,  286,  285,  253,  284,  286,  285,  253,
 /*  4070 */   215,  491,  491,  491,  350,  491,  491,  491,  284,  286,
 /*  4080 */   285,  253,  284,  286,  285,  253,  491,  491,  491,  199,
 /*  4090 */   491,  491,  491,  205,  491,  491,  491,  284,  286,  285,
 /*  4100 */   253,  284,  286,  285,  253,
};
static YYCODETYPE yy_lookahead[] = {
 /*     0 */     7,    8,    9,  103,  104,  105,  106,  107,   15,  100,
 /*    10 */   101,  102,  103,  104,  105,  106,  107,  130,  131,  132,
 /*    20 */    27,   28,   29,   30,   31,  111,   35,   34,  114,  115,
 /*    30 */   116,  117,  118,  118,  110,   42,  112,  123,  124,  124,
 /*    40 */    47,   50,   49,  129,  129,   52,   53,   54,   55,   56,
 /*    50 */    57,   58,   59,   60,   61,   62,   10,   11,   12,   13,
 /*    60 */    14,   15,   16,   17,   18,   19,   20,   21,   22,   23,
 /*    70 */    24,   25,   26,   24,   25,   26,   83,   84,   28,   49,
 /*    80 */   128,   35,  130,  131,   34,   35,   93,   94,   95,   96,
 /*    90 */    97,   98,   42,   43,   44,  126,   35,  168,  169,  130,
 /*   100 */   131,  132,   52,   53,   54,   55,   56,   57,   58,   59,
 /*   110 */    60,   50,   62,   63,   64,   65,   86,   67,   50,   28,
 /*   120 */    34,   71,   72,   73,   74,   34,   35,   77,  128,   34,
 /*   130 */   130,  131,   66,   42,   43,   44,   49,   50,   34,   89,
 /*   140 */    90,   91,   92,   52,   53,   54,   55,   56,   57,   58,
 /*   150 */    59,   60,   84,   62,   63,   64,   65,  114,   67,  116,
 /*   160 */   117,  118,   71,   72,   73,   74,  123,  124,   77,   83,
 /*   170 */    28,   84,  129,   86,    6,   42,   34,   35,   83,    6,
 /*   180 */    89,   90,   91,   92,   42,   43,   44,   83,   93,   94,
 /*   190 */    95,   96,   97,   98,   52,   53,   54,   55,   56,   57,
 /*   200 */    58,   59,   60,   35,   62,   63,   64,   65,   35,   67,
 /*   210 */   109,   28,  156,   71,   72,   73,   74,   34,   35,   77,
 /*   220 */   164,  165,  166,  167,    6,   42,   43,   44,  172,  173,
 /*   230 */    45,   89,   90,   91,   92,   52,   53,   54,   55,   56,
 /*   240 */    57,   58,   59,   60,    6,   62,   63,   64,   65,   34,
 /*   250 */    67,  168,  169,   35,   71,   72,   73,   74,   43,   44,
 /*   260 */    77,   16,   28,   78,   79,   80,   81,   82,   34,   35,
 /*   270 */    32,    6,   89,   90,   91,   92,   42,   43,   44,   21,
 /*   280 */    22,   23,   24,   25,   26,    6,   52,   53,   54,   55,
 /*   290 */    56,   57,   58,   59,   60,   35,   62,   63,   64,   65,
 /*   300 */    35,   67,   42,   28,  156,   71,   72,   73,   74,   34,
 /*   310 */    35,   77,  164,  165,  166,  167,  159,   42,   43,   44,
 /*   320 */   172,  173,   43,   89,   90,   91,   92,   52,   53,   54,
 /*   330 */    55,   56,   57,   58,   59,   60,    6,   62,   63,   64,
 /*   340 */    65,   50,   67,  116,  117,  118,   71,   72,   73,   74,
 /*   350 */     6,  124,   77,  110,   28,  112,  129,   49,   50,   34,
 /*   360 */    34,   35,   32,  156,   89,   90,   91,   92,   42,   43,
 /*   370 */    44,  164,  165,  166,  167,   84,   32,   69,   52,   53,
 /*   380 */    54,   55,   56,   57,   58,   59,   60,   85,   62,   63,
 /*   390 */    64,   65,   84,   67,   86,   28,  156,   71,   72,   73,
 /*   400 */    74,   34,   35,   77,  164,  165,  166,  167,   83,   42,
 /*   410 */    43,   44,  172,  173,   34,   89,   90,   91,   92,   52,
 /*   420 */    53,   54,   55,   56,   57,   58,   59,   60,   34,   62,
 /*   430 */    63,   64,   65,   32,   67,  116,   42,  118,   71,   72,
 /*   440 */    73,   74,  123,  124,   77,   37,   28,   39,  129,   37,
 /*   450 */    42,   39,   34,   35,   42,  156,   89,   90,   91,   92,
 /*   460 */    42,   43,   44,  164,  165,  166,  167,   34,  168,  169,
 /*   470 */    52,   53,   54,   55,   56,   57,   58,   59,   60,   37,
 /*   480 */    62,   63,   64,   65,   42,   67,  155,   28,  156,   71,
 /*   490 */    72,   73,   74,   34,   35,   77,  164,  165,  166,  167,
 /*   500 */     6,   42,   43,   44,  172,  173,    6,   89,   90,   91,
 /*   510 */    92,   52,   53,   54,   55,   56,   57,   58,   59,   60,
 /*   520 */     6,   62,   63,   64,   65,  128,   67,  130,  131,   35,
 /*   530 */    71,   72,   73,   74,   43,   35,   77,  126,   28,   49,
 /*   540 */    50,  130,  131,  132,   34,   35,   32,  156,   89,   90,
 /*   550 */    91,   92,   42,   43,   44,  164,  165,  166,  167,   68,
 /*   560 */    49,   70,   52,   53,   54,   55,   56,   57,   58,   59,
 /*   570 */    60,   35,   62,   63,   64,   65,   86,   67,   42,   28,
 /*   580 */   156,   71,   72,   73,   74,   34,   35,   77,  164,  165,
 /*   590 */   166,  167,    6,   42,   43,   44,  172,  173,    6,   89,
 /*   600 */    90,   91,   92,   52,   53,   54,   55,   56,   57,   58,
 /*   610 */    59,   60,    6,   62,   63,   64,   65,  128,   67,  130,
 /*   620 */   131,   35,   71,   72,   73,   74,   43,   35,   77,  126,
 /*   630 */    28,  168,  169,  130,  131,  132,   34,   35,   32,  156,
 /*   640 */    89,   90,   91,   92,   42,   43,   44,  164,  165,  166,
 /*   650 */   167,   68,  118,   70,   52,   53,   54,   55,   56,   57,
 /*   660 */    58,   59,   60,  129,   62,   63,   64,   65,    6,   67,
 /*   670 */    69,   28,  156,   71,   72,   73,   74,   34,   35,   77,
 /*   680 */   164,  165,  166,  167,    6,   42,   43,   44,  172,  173,
 /*   690 */    34,   89,   90,   91,   92,   52,   53,   54,   55,   56,
 /*   700 */    57,   58,   59,   60,   42,   62,   63,   64,   65,  128,
 /*   710 */    67,  130,  131,   35,   71,   72,   73,   74,  168,  169,
 /*   720 */    77,  126,   28,  168,  169,  130,  131,  132,   34,   35,
 /*   730 */    34,  156,   89,   90,   91,   92,   42,   43,   44,  164,
 /*   740 */   165,  166,  167,   47,  168,  169,   52,   53,   54,   55,
 /*   750 */    56,   57,   58,   59,   60,  159,   62,   63,   64,   65,
 /*   760 */   110,   67,  112,   28,  156,   71,   72,   73,   74,   34,
 /*   770 */    35,   77,  164,  165,  166,  167,   43,   42,   43,   44,
 /*   780 */   172,  173,    6,   89,   90,   91,   92,   52,   53,   54,
 /*   790 */    55,   56,   57,   58,   59,   60,    6,   62,   63,   64,
 /*   800 */    65,   35,   67,   35,  168,  169,   71,   72,   73,   74,
 /*   810 */    42,   35,   77,   45,   28,  128,   50,  130,  131,   34,
 /*   820 */    34,   35,   32,  156,   89,   90,   91,   92,   42,   43,
 /*   830 */    44,  164,  165,  166,  167,   35,  168,  169,   52,   53,
 /*   840 */    54,   55,   56,   57,   58,   59,   60,   37,   62,   63,
 /*   850 */    64,   65,   42,   67,  159,   28,  156,   71,   72,   73,
 /*   860 */    74,   34,   35,   77,  164,  165,  166,  167,    6,   42,
 /*   870 */    43,   44,  172,  173,   72,   89,   90,   91,   92,   52,
 /*   880 */    53,   54,   55,   56,   57,   58,   59,   60,    6,   62,
 /*   890 */    63,   64,   65,   34,   67,   35,  118,   35,   71,   72,
 /*   900 */    73,   74,   42,  125,   77,   45,   28,  129,  128,   42,
 /*   910 */   130,  131,   34,   35,   32,  156,   89,   90,   91,   92,
 /*   920 */    42,   43,   44,  164,  165,  166,  167,   42,  168,  169,
 /*   930 */    52,   53,   54,   55,   56,   57,   58,   59,   60,   85,
 /*   940 */    62,   63,   64,   65,   69,   67,   42,   28,  156,   71,
 /*   950 */    72,   73,   74,   34,   35,   77,  164,  165,  166,  167,
 /*   960 */   159,   42,   43,   44,  172,  173,  129,   89,   90,   91,
 /*   970 */    92,   52,   53,   54,   55,   56,   57,   58,   59,   60,
 /*   980 */     6,   62,   63,   64,   65,  128,   67,  130,  131,   34,
 /*   990 */    71,   72,   73,   74,  154,  155,   77,   42,   28,   39,
 /*  1000 */   121,  122,   42,   34,   34,   35,   32,  156,   89,   90,
 /*  1010 */    91,   92,   42,   43,   44,  164,  165,  166,  167,    6,
 /*  1020 */   118,   35,   52,   53,   54,   55,   56,   57,   58,   59,
 /*  1030 */    60,  129,   62,   63,   64,   65,   50,   67,   34,   28,
 /*  1040 */   156,   71,   72,   73,   74,   34,   35,   77,  164,  165,
 /*  1050 */   166,  167,   83,   42,   43,   44,  172,  173,   42,   89,
 /*  1060 */    90,   91,   92,   52,   53,   54,   55,   56,   57,   58,
 /*  1070 */    59,   60,   34,   62,   63,   64,   65,   69,   67,   34,
 /*  1080 */    42,   34,   71,   72,   73,   74,   42,   42,   77,   42,
 /*  1090 */    28,  157,  158,   34,   50,    0,   34,   35,   85,  156,
 /*  1100 */    89,   90,   91,   92,   42,   43,   44,  164,  165,  166,
 /*  1110 */   167,   75,  118,   16,   52,   53,   54,   55,   56,   57,
 /*  1120 */    58,   59,   60,  129,   62,   63,   64,   65,   49,   67,
 /*  1130 */    49,   28,  156,   71,   72,   73,   74,   34,   35,   77,
 /*  1140 */   164,  165,  166,  167,   34,   42,   43,   44,   49,  173,
 /*  1150 */   109,   89,   90,   91,   92,   52,   53,   54,   55,   56,
 /*  1160 */    57,   58,   59,   60,    6,   62,   63,   64,   65,   86,
 /*  1170 */    67,    6,   47,   34,   71,   72,   73,   74,   42,   42,
 /*  1180 */    77,   49,   28,   44,   35,   45,   50,   50,   34,   35,
 /*  1190 */    32,  156,   89,   90,   91,   92,   42,   43,   44,  164,
 /*  1200 */   165,  166,  167,  109,  119,   35,   52,   53,   54,   55,
 /*  1210 */    56,   57,   58,   59,   60,    6,   62,   63,   64,   65,
 /*  1220 */    72,   67,   49,   28,  156,   71,   72,   73,   74,   34,
 /*  1230 */    35,   77,  164,  165,  166,  167,    6,   42,   43,   44,
 /*  1240 */    75,   32,  119,   89,   90,   91,   92,   52,   53,   54,
 /*  1250 */    55,   56,   57,   58,   59,   60,   42,   62,   63,   64,
 /*  1260 */    65,   43,   67,   47,   50,   35,   71,   72,   73,   74,
 /*  1270 */    34,    6,   77,   42,   28,   49,  119,   45,   42,   35,
 /*  1280 */    34,   35,   69,  156,   89,   90,   91,   92,   42,   43,
 /*  1290 */    44,  164,  165,  166,  167,   66,   43,   32,   52,   53,
 /*  1300 */    54,   55,   56,   57,   58,   59,   60,    6,   62,   63,
 /*  1310 */    64,   65,   35,   67,   35,   28,  156,   71,   72,   73,
 /*  1320 */    74,   34,   35,   77,  164,  165,  166,  167,   35,   42,
 /*  1330 */    43,   44,   34,   32,   49,   89,   90,   91,   92,   52,
 /*  1340 */    53,   54,   55,   56,   57,   58,   59,   60,    6,   62,
 /*  1350 */    63,   64,   65,  120,   67,   34,    6,   35,   71,   72,
 /*  1360 */    73,   74,  119,  159,   77,  120,   28,   35,   45,   35,
 /*  1370 */    34,   34,   34,   35,   32,  156,   89,   90,   91,   92,
 /*  1380 */    42,   43,   44,  164,  165,  166,  167,   34,   43,   35,
 /*  1390 */    52,   53,   54,   55,   56,   57,   58,   59,   60,    6,
 /*  1400 */    62,   63,   64,   65,  109,   67,   42,   28,  156,   71,
 /*  1410 */    72,   73,   74,   34,   35,   77,  164,  165,  166,  167,
 /*  1420 */    35,   42,   43,   44,   34,   32,   43,   89,   90,   91,
 /*  1430 */    92,   52,   53,   54,   55,   56,   57,   58,   59,   60,
 /*  1440 */   122,   62,   63,   64,   65,   34,   67,   35,  170,   43,
 /*  1450 */    71,   72,   73,   74,   34,   34,   77,   35,   28,   45,
 /*  1460 */    35,   38,   34,  120,   34,   35,   35,  156,   89,   90,
 /*  1470 */    91,   92,   42,   43,   44,  164,  165,  166,  167,   34,
 /*  1480 */   119,   34,   52,   53,   54,   55,   56,   57,   58,   59,
 /*  1490 */    60,  120,   62,   63,   64,   65,   43,   67,  158,   28,
 /*  1500 */   156,   71,   72,   73,   74,   34,   35,   77,  164,  165,
 /*  1510 */   166,  167,   35,   42,   43,   44,   34,   49,   34,   89,
 /*  1520 */    90,   91,   92,   52,   53,   54,   55,   56,   57,   58,
 /*  1530 */    59,   60,   35,   62,   63,   64,   65,   35,   67,   34,
 /*  1540 */    49,  109,   71,   72,   73,   74,  170,  109,   77,   35,
 /*  1550 */    28,   34,   43,   35,  119,  112,   34,   35,   45,  156,
 /*  1560 */    89,   90,   91,   92,   42,  109,   44,  164,  165,  166,
 /*  1570 */   167,  119,  130,   34,   52,   53,   54,   55,   56,   57,
 /*  1580 */    58,   59,   60,   34,   62,   63,   64,   65,  169,   67,
 /*  1590 */   107,   34,   45,   71,   72,   73,   74,  109,  156,   77,
 /*  1600 */    38,    1,    2,    3,    4,    5,  164,  165,  166,  167,
 /*  1610 */   127,   89,   90,   91,   92,   42,  133,  134,  135,  136,
 /*  1620 */   137,  138,  139,  140,  141,  142,  143,  144,  145,  146,
 /*  1630 */   147,  148,  149,  150,  151,  152,  107,   42,   34,  159,
 /*  1640 */    40,   41,   35,   43,   44,  159,   46,  164,  165,  166,
 /*  1650 */   167,   51,  159,   43,   49,   85,  127,   34,   34,   43,
 /*  1660 */    49,  108,  133,  134,  135,  136,  137,  138,  139,  140,
 /*  1670 */   141,  142,  143,  144,  145,  146,  147,  148,  149,  150,
 /*  1680 */   151,  152,  107,  159,   34,   47,   35,   42,   34,   43,
 /*  1690 */    43,   69,  159,  164,  165,  166,  167,    1,    2,    3,
 /*  1700 */     4,    5,  127,   42,  119,  109,  108,   42,  133,  134,
 /*  1710 */   135,  136,  137,  138,  139,  140,  141,  142,  143,  144,
 /*  1720 */   145,  146,  147,  148,  149,  150,  151,  152,  107,   42,
 /*  1730 */    34,  119,  177,  113,  177,  159,   40,   41,  118,  164,
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
 /*  2460 */   149,  150,  151,  152,  107,  177,  177,  177,  177,  177,
 /*  2470 */   177,  177,  156,  177,  177,  164,  165,  166,  167,  163,
 /*  2480 */   164,  165,  166,  167,  127,  177,  177,  177,  177,  177,
 /*  2490 */   133,  134,  135,  136,  137,  138,  139,  140,  141,  142,
 /*  2500 */   143,  144,  145,  146,  147,  148,  149,  150,  151,  152,
 /*  2510 */   107,  177,  177,  177,  177,  177,  177,  177,  177,  156,
 /*  2520 */   177,  164,  165,  166,  167,  162,  163,  164,  165,  166,
 /*  2530 */   167,  177,  177,  177,  177,  177,  133,  134,  135,  136,
 /*  2540 */   137,  138,  139,  140,  141,  142,  143,  144,  145,  146,
 /*  2550 */   147,  148,  149,  150,  151,  152,    7,    8,    9,  177,
 /*  2560 */   153,  177,  177,  156,   15,  177,  177,  164,  165,  166,
 /*  2570 */   167,  164,  165,  166,  167,  177,   27,   28,   29,   30,
 /*  2580 */    31,  177,  177,   34,    7,    8,    9,  177,  177,  177,
 /*  2590 */   177,   42,   15,  177,  177,  177,   47,  177,   49,  177,
 /*  2600 */   177,  177,  177,  177,   27,   28,   29,   30,   31,  177,
 /*  2610 */   177,   34,  177,  177,    7,    8,    9,  177,  177,   42,
 /*  2620 */   156,  177,   15,  177,   47,   76,   49,  177,  164,  165,
 /*  2630 */   166,  167,   83,   84,   27,   28,   29,   30,   31,   32,
 /*  2640 */   176,   34,   93,   94,   95,   96,   97,   98,  177,   42,
 /*  2650 */   177,  177,  177,   76,   47,  177,   49,    7,    8,    9,
 /*  2660 */    83,   84,  177,  177,  177,   15,  177,  177,  177,  177,
 /*  2670 */    93,   94,   95,   96,   97,   98,  177,   27,   28,   29,
 /*  2680 */    30,   31,   32,  177,   34,  177,    7,    8,    9,  177,
 /*  2690 */    83,   84,   42,  177,   15,  177,  177,   47,  177,   49,
 /*  2700 */    93,   94,   95,   96,   97,   98,   27,   28,   29,   30,
 /*  2710 */    31,   32,  177,   34,  177,    7,    8,    9,  177,  177,
 /*  2720 */   177,   42,  177,   15,  177,  177,   47,  177,   49,  177,
 /*  2730 */   177,  177,  177,   83,   84,   27,   28,   29,   30,   31,
 /*  2740 */   177,  177,   34,   93,   94,   95,   96,   97,   98,  177,
 /*  2750 */    42,  177,  177,  177,  177,   47,  177,   49,  177,  177,
 /*  2760 */   177,  177,   83,   84,  177,  177,  177,  177,  177,  177,
 /*  2770 */   177,  177,   93,   94,   95,   96,   97,   98,  177,   45,
 /*  2780 */   177,  177,  177,  177,   50,  177,  177,  177,  177,  156,
 /*  2790 */   177,   83,   84,   85,  161,  177,  177,  164,  165,  166,
 /*  2800 */   167,   93,   94,   95,   96,   97,   98,  177,  177,    7,
 /*  2810 */     8,    9,   78,   79,   80,   81,   82,   15,   84,  177,
 /*  2820 */    86,   87,   88,  177,  177,  177,  177,  177,  177,   27,
 /*  2830 */    28,   29,   30,   31,   32,  177,   34,    7,    8,    9,
 /*  2840 */     0,  177,  177,  177,   42,   15,  177,  177,  177,   47,
 /*  2850 */   177,   49,  177,  177,  177,  177,  177,   27,   28,   29,
 /*  2860 */    30,   31,  177,  177,   34,   35,  177,    7,    8,    9,
 /*  2870 */   177,  177,   42,   33,  177,   15,   36,   47,   38,   49,
 /*  2880 */    40,   41,  177,  177,   44,   83,   84,   27,   28,   29,
 /*  2890 */    30,   31,   32,  177,   34,   93,   94,   95,   96,   97,
 /*  2900 */    98,  177,   42,  177,  177,  177,  177,   47,  177,   49,
 /*  2910 */     7,    8,    9,   83,   84,  177,  177,  177,   15,  177,
 /*  2920 */   177,  177,  177,   93,   94,   95,   96,   97,   98,  177,
 /*  2930 */    27,   28,   29,   30,   31,  177,  177,   34,  177,    7,
 /*  2940 */     8,    9,  177,   83,   84,   42,  177,   15,  177,  177,
 /*  2950 */    47,  177,   49,   93,   94,   95,   96,   97,   98,   27,
 /*  2960 */    28,   29,   30,   31,   32,  177,   34,  156,  177,  177,
 /*  2970 */   177,  160,  177,  177,   42,  164,  165,  166,  167,   47,
 /*  2980 */   177,   49,    7,    8,    9,  177,   83,   84,   85,  177,
 /*  2990 */    15,  177,  177,  177,  177,  177,   93,   94,   95,   96,
 /*  3000 */    97,   98,   27,   28,   29,   30,   31,   32,  177,   34,
 /*  3010 */   177,    7,    8,    9,  177,   83,   84,   42,  177,   15,
 /*  3020 */   177,  177,   47,  177,   49,   93,   94,   95,   96,   97,
 /*  3030 */    98,   27,   28,   29,   30,   31,  177,  177,   34,  177,
 /*  3040 */     7,    8,    9,  177,  177,  177,   42,  177,   15,  177,
 /*  3050 */   177,   47,  177,   49,  177,  177,  177,  177,   83,   84,
 /*  3060 */    27,   28,   29,   30,   31,   32,  177,   34,   93,   94,
 /*  3070 */    95,   96,   97,   98,  177,   42,  177,  177,  177,  177,
 /*  3080 */    47,  177,   49,    7,    8,    9,  177,   83,   84,   85,
 /*  3090 */   177,   15,  177,  177,  177,  177,  177,   93,   94,   95,
 /*  3100 */    96,   97,   98,   27,   28,   29,   30,   31,  177,  177,
 /*  3110 */    34,  177,    7,    8,    9,  177,   83,   84,   42,  177,
 /*  3120 */    15,  177,  177,   47,  177,   49,   93,   94,   95,   96,
 /*  3130 */    97,   98,   27,   28,   29,   30,   31,   32,  177,   34,
 /*  3140 */   156,  177,  177,  177,  160,  177,  177,   42,  164,  165,
 /*  3150 */   166,  167,   47,  177,   49,    7,    8,    9,  177,   83,
 /*  3160 */    84,   85,  177,   15,  177,  177,  177,  177,  177,   93,
 /*  3170 */    94,   95,   96,   97,   98,   27,   28,   29,   30,   31,
 /*  3180 */    32,  177,   34,  177,    7,    8,    9,  177,   83,   84,
 /*  3190 */    42,  177,   15,  177,  177,   47,  177,   49,   93,   94,
 /*  3200 */    95,   96,   97,   98,   27,   28,   29,   30,   31,  177,
 /*  3210 */   177,   34,  177,    7,    8,    9,  177,  177,  177,   42,
 /*  3220 */   177,   15,  177,  177,   47,  177,   49,  177,  177,  177,
 /*  3230 */   177,   83,   84,   27,   28,   29,   30,   31,  177,  177,
 /*  3240 */    34,   93,   94,   95,   96,   97,   98,  177,   42,  177,
 /*  3250 */   177,  177,  177,   47,  156,   49,  177,  177,  156,  161,
 /*  3260 */    83,   84,  164,  165,  166,  167,  164,  165,  166,  167,
 /*  3270 */    93,   94,   95,   96,   97,   98,  174,  175,  176,  177,
 /*  3280 */   177,  177,  177,  177,  177,  177,  177,  177,  177,   83,
 /*  3290 */    84,    1,    2,    3,    4,    5,  177,  156,  177,   93,
 /*  3300 */    94,   95,   96,   97,   98,  164,  165,  166,  167,  177,
 /*  3310 */    10,   11,   12,   13,   14,   15,   16,   17,   18,   19,
 /*  3320 */    20,   21,   22,   23,   24,   25,   26,  177,  177,  156,
 /*  3330 */    40,   41,  177,   15,  177,  177,   46,  164,  165,  166,
 /*  3340 */   167,   51,   42,  177,  171,  177,  177,  174,  175,  176,
 /*  3350 */    32,  177,   34,   10,   11,   12,   13,   14,   15,   16,
 /*  3360 */    17,   18,   19,   20,   21,   22,   23,   24,   25,   26,
 /*  3370 */    52,   53,   54,   55,   56,   57,   58,   59,   60,   61,
 /*  3380 */    62,  177,  177,  156,  177,   42,  177,  160,  177,  177,
 /*  3390 */   177,  164,  165,  166,  167,  177,   10,   11,   12,   13,
 /*  3400 */    14,   15,   16,   17,   18,   19,   20,   21,   22,   23,
 /*  3410 */    24,   25,   26,   10,   11,   12,   13,   14,   15,   16,
 /*  3420 */    17,   18,   19,   20,   21,   22,   23,   24,   25,   26,
 /*  3430 */    10,   11,   12,   13,   14,   15,   16,   17,   18,   19,
 /*  3440 */    20,   21,   22,   23,   24,   25,   26,   45,  177,  177,
 /*  3450 */   177,  177,   32,   10,   11,   12,   13,   14,   15,   16,
 /*  3460 */    17,   18,   19,   20,   21,   22,   23,   24,   25,   26,
 /*  3470 */   177,   85,  177,  177,  177,  177,  177,  177,  177,  177,
 /*  3480 */    78,   79,   80,   81,   82,   42,   84,  177,   85,   87,
 /*  3490 */    88,  177,  177,  177,  177,  177,   10,   11,   12,   13,
 /*  3500 */    14,   15,   16,   17,   18,   19,   20,   21,   22,   23,
 /*  3510 */    24,   25,   26,   13,   14,   15,   16,   17,   18,   19,
 /*  3520 */    20,   21,   22,   23,   24,   25,   26,  156,   42,  177,
 /*  3530 */   177,  160,  177,  177,  177,  164,  165,  166,  167,   10,
 /*  3540 */    11,   12,   13,   14,   15,   16,   17,   18,   19,   20,
 /*  3550 */    21,   22,   23,   24,   25,   26,  177,  177,  177,  177,
 /*  3560 */   177,  177,  177,  177,   35,   10,   11,   12,   13,   14,
 /*  3570 */    15,   16,   17,   18,   19,   20,   21,   22,   23,   24,
 /*  3580 */    25,   26,   10,   11,   12,   13,   14,   15,   16,   17,
 /*  3590 */    18,   19,   20,   21,   22,   23,   24,   25,   26,   10,
 /*  3600 */    11,   12,   13,   14,   15,   16,   17,   18,   19,   20,
 /*  3610 */    21,   22,   23,   24,   25,   26,   11,   12,   13,   14,
 /*  3620 */    15,   16,   17,   18,   19,   20,   21,   22,   23,   24,
 /*  3630 */    25,   26,   15,  177,  177,  177,  177,  156,  177,  177,
 /*  3640 */    85,  160,  177,   15,  177,  164,  165,  166,  167,   32,
 /*  3650 */   177,   34,  177,  177,  177,  177,  177,   85,  177,  177,
 /*  3660 */    32,  177,   34,  177,    1,    2,    3,    4,    5,   52,
 /*  3670 */    53,   54,   55,   56,   57,   58,   59,   60,   61,   62,
 /*  3680 */    52,   53,   54,   55,   56,   57,   58,   59,   60,   61,
 /*  3690 */    62,   15,  177,  177,  177,  177,  177,  177,  177,  177,
 /*  3700 */   177,  177,   15,   40,   41,  177,   43,   44,   32,   46,
 /*  3710 */    34,  177,   33,  177,   51,   36,  177,   38,   15,   40,
 /*  3720 */    41,   34,  177,   44,  177,  177,  177,  177,   52,   53,
 /*  3730 */    54,   55,   56,   57,   58,   59,   60,   61,   62,   52,
 /*  3740 */    53,   54,   55,   56,   57,   58,   59,   60,   61,   62,
 /*  3750 */     1,    2,    3,    4,    5,   52,   53,   54,   55,   56,
 /*  3760 */    57,   58,   59,   60,   61,   62,    1,    2,    3,    4,
 /*  3770 */     5,  156,  177,  153,  177,  160,  156,  177,  177,  164,
 /*  3780 */   165,  166,  167,  177,  164,  165,  166,  167,  156,   40,
 /*  3790 */    41,  177,   43,   44,  177,   46,  164,  165,  166,  167,
 /*  3800 */    51,  177,  177,  177,  156,   40,   41,  177,   43,   44,
 /*  3810 */   177,   46,  164,  165,  166,  167,   51,  177,    1,    2,
 /*  3820 */     3,    4,    5,    1,    2,    3,    4,    5,  156,  177,
 /*  3830 */   177,  177,  160,  177,  177,  177,  164,  165,  166,  167,
 /*  3840 */   156,  177,  177,  177,  160,  156,  177,  177,  164,  165,
 /*  3850 */   166,  167,  177,  164,  165,  166,  167,   40,   41,  177,
 /*  3860 */    43,   44,   40,   41,  177,   43,   44,  177,   51,  177,
 /*  3870 */   177,  177,  177,   51,  177,    1,    2,    3,    4,    5,
 /*  3880 */     1,    2,    3,    4,    5,  156,  177,  177,  177,  160,
 /*  3890 */   177,  177,  177,  164,  165,  166,  167,  156,  177,  177,
 /*  3900 */   177,  160,  156,  177,  177,  164,  165,  166,  167,  177,
 /*  3910 */   164,  165,  166,  167,   40,   41,  177,   43,   44,   40,
 /*  3920 */    41,  177,   43,   44,  177,   51,  177,  177,  177,  177,
 /*  3930 */    51,  177,    1,    2,    3,    4,    5,    1,    2,    3,
 /*  3940 */     4,    5,  156,  177,  177,  177,  177,  161,  177,  177,
 /*  3950 */   164,  165,  166,  167,  177,  177,  177,  156,  177,  177,
 /*  3960 */   177,  160,  177,  177,  177,  164,  165,  166,  167,  177,
 /*  3970 */   177,   40,   41,  177,   43,   44,   40,   41,  177,   43,
 /*  3980 */    44,  177,   51,  177,  177,  177,  177,   51,  177,    1,
 /*  3990 */     2,    3,    4,    5,    1,    2,    3,    4,    5,  177,
 /*  4000 */   177,  156,  177,  177,  177,  156,  177,  177,  177,  164,
 /*  4010 */   165,  166,  167,  164,  165,  166,  167,  177,  177,  177,
 /*  4020 */   177,  177,  177,  177,  177,  177,  177,  177,   40,   41,
 /*  4030 */   177,   43,   44,   40,   41,  177,   43,   44,  156,   51,
 /*  4040 */   177,  177,  156,  177,   51,  177,  164,  165,  166,  167,
 /*  4050 */   164,  165,  166,  167,  156,  177,  177,  177,  156,  177,
 /*  4060 */   177,  177,  164,  165,  166,  167,  164,  165,  166,  167,
 /*  4070 */   156,  177,  177,  177,  156,  177,  177,  177,  164,  165,
 /*  4080 */   166,  167,  164,  165,  166,  167,  177,  177,  177,  156,
 /*  4090 */   177,  177,  177,  156,  177,  177,  177,  164,  165,  166,
 /*  4100 */   167,  164,  165,  166,  167,
};
#define YY_SHIFT_USE_DFLT (-10)
static short yy_shift_ofst[] = {
 /*     0 */  3679, 1095, 2840,  -10,  -10,  -10,  -10,  -10, 1484, 1651,
 /*    10 */   -10,  380,  442,  -10, 1539, 1661,  -10, 3931,  -10, 3822,
 /*    20 */   -10,  -10, 2156, 1336, 1099, 3628,  986, 3703, 1607,  -10,
 /*    30 */   -10,  -10, 1353,  245,  -10,  -10,  -10,  -10,  -10,  -10,
 /*    40 */   -10,  -10,  -10,  -10,  -10,  -10,  -10,  330,   61, 3703,
 /*    50 */  1431,  -10,  -10, 3687,  -10, 1414,   95,  -10,  -10,  -10,
 /*    60 */   -10,  -10,  -10,  -10,  -10, 1083, 1638,  -10, 1654, 1547,
 /*    70 */    95,  -10, 1445, 1323,   95,  -10,  -10,  -10,  -10,  -10,
 /*    80 */   -10,  -10,  -10,  -10,  -10,  -10, 2248, 1880, 1004, 1079,
 /*    90 */  3676,  766, 3703, 1518,  -10,  -10,  238,   -9, 3703, 1293,
 /*   100 */   -10,  -10,  -10,  -10, 1517,  408,  -10, 1428,  960,  -10,
 /*   110 */   859,  662,  -10, 1600,  -10, 1647,  -10, 3765, 3879,  -10,
 /*   120 */  2064, 1421, 1468, 3617, 1137,  459,  -10,  827,  -10,  -10,
 /*   130 */   -10,  -10,  -10,  -10,  -10,  -10,  -10,  -10,  -10,  -10,
 /*   140 */   -10,  -10,  -10,  -10,  -10,  -10,  -10,  -10,  -10,  -10,
 /*   150 */   -10, 1514,  -10, 1502,  -10, 3177, 1364,  326,  878, 1229,
 /*   160 */  1231,  919, 1287,  -10, 3177,  885,  583,  -10,  491,  -10,
 /*   170 */   -10,   95, 1622, 1522, 1522, 1573, 1195, 1379,  -10, 3177,
 /*   180 */   133,   50, 1154,  -10, 1016,  786,  735, 1148, 3177, 1279,
 /*   190 */   -10, 3177,  -10, 3177, 1411,  -10,   68, 3177, 3177, 3589,
 /*   200 */  3177, 3177, 3589, 3177, 3177, 3589, 3177,  258, 3177,  258,
 /*   210 */  3177,  258, 3177,  258, 3177,  258, 3177,   49, 3177,   49,
 /*   220 */  3177,  -10, 3177,  -10, 3177,  -10, 3177,   49, 3177, 3500,
 /*   230 */  3177, 3500, 3177, 3605,   -7, 3420,  -10,  401, 3177,  -10,
 /*   240 */  1059, 1097, 3177,  258,   87, 1038, 1132, 2860, 3589, 1298,
 /*   250 */  1350, 1411,  -10,  -10, 3177,  -10,  -10,  -10,  -10,  -10,
 /*   260 */   -10,  -10,  -10,  -10, 3076, 3589,  308,  696, 1285, 3148,
 /*   270 */   394, 1491, 2975,  -10,  344,  -10, 3177, 1390, 1345,   30,
 /*   280 */  2802,  514,  -10,  -10,  -10,  -10,  -10,  -10, 1045, 1081,
 /*   290 */  2607,  606,  -10,  -10,  785,  733,  511, 3033,  790,  -10,
 /*   300 */   -10,  -10, 1650, 1616, 1605, 3105,  -10,  974,  -10, 1158,
 /*   310 */   -10,  -10,  -10, 3177, 3386,  -10, 2679, 1265,  -10,  -10,
 /*   320 */   875, 1008,  -10, 1013,  -10, 3206,  -10, 1213, 3177,  -10,
 /*   330 */   -10,  -10, 1301,  -10,  -10,  325, 1406, 1226, 2932, 1342,
 /*   340 */   -10,  -10, 1453, 1173, 2650, 1393,  -10,  -10,  258,  258,
 /*   350 */  3555,  -10, 1236,  -10,  969, 1383,  -10, 1509,  -10,  258,
 /*   360 */  3589, 1549, 1165, 2549, 3486, 1522,  275,  -10, 1482, 1230,
 /*   370 */   -10, 1482,  -10, 2734,  -10,  -10,  -10,  -10,  -10,  -10,
 /*   380 */  3177,  -10, 3589, 1047, 3402, 3177,  -10, 2708,  185, 3177,
 /*   390 */   -10,  302,  185, 3177,  -10, 3403,  -10,  -10,  104, 1646,
 /*   400 */   185, 3177,  -10, 1610,  185, 3177,  -10, 1604, 1698, 3177,
 /*   410 */   -10, 2903,  185, 3177,  -10, 1570,  185, 3177,  -10, 3004,
 /*   420 */   185, 3177,  -10,  854,  185, 3177,  -10,  -10,  -10,  -10,
 /*   430 */  3177,  862,  -10, 3177,  -10, 3589,  -10, 1149,  -10, 1277,
 /*   440 */   -10, 1322,  -10, 1425,  -10, 2830,   46,  -10,  -10,  656,
 /*   450 */   291, 1505, 1334,  -10, 3177, 3572, 1412,  -10, 3177, 3529,
 /*   460 */   -10, 1337,  168,  -10, 1337,  -10, 1513,   95,  -10,  -10,
 /*   470 */  1337,  173,  -10, 1337,  218,  -10, 1337,  265,  -10, 1337,
 /*   480 */   494,  -10, 1337,  500,  -10, 1337,  586,  -10, 1337,  592,
 /*   490 */   -10, 1337,  678,  -10, 1337,  776,  -10,  490, 1321, 1285,
 /*   500 */   955, 1132,   86, 1253, 1226, 1218, 1173,  -10,  -10, 3177,
 /*   510 */  3343, 1522,  234,  -10, 1110, 1036, 2577, 3300, 1522,  367,
 /*   520 */   -10, 3177, 3443, 1522,  418,  -10,  802, 3177,  800,  -10,
 /*   530 */   -10,  -10,  601, 1522, 1522,  -10,  -10,   66, 1687,  510,
 /*   540 */   551,  -10,  -10, 3703, 1645,  602,  -10,  643,  -10,  882,
 /*   550 */  1044,  694,  -10,  970,  -10, 3703, 1595, 1103,  -10, 1246,
 /*   560 */   -10, 2248, 1972, 1624, 1611, 3318, 1136, 1430,  -10, 1471,
 /*   570 */   -10, 3703, 1665,  183,  -10,   91,  -10, 1209, 1214,  142,
 /*   580 */   -10, 1338,  -10, 3703,  867, 1062,  -10, 1011,  -10, 3663,
 /*   590 */  3817,  -10,  -10, 3290, 1125, 1140,   95, 1170,  -10, 1216,
 /*   600 */  1232,   95, 1244,  -10,  -10, 3290, 1696,  768,  -10,   95,
 /*   610 */   260,  -10, 1332,  -10,  215, 1354,  279, 1385, 1139,  -10,
 /*   620 */   -10, 1420,  -10,  -10, 1422,  -10, 1788,  860,  -10,   95,
 /*   630 */   536,  -10, 1477,  -10, 1497,  -10,  -10, 3749, 3874, 3988,
 /*   640 */  2248, 3993,  -10, 3936,  859,  -10,  -10,  -10,  859,  662,
 /*   650 */   -10, 1562, 1623,  412,  -10,  433,  904,  -10,  859,  662,
 /*   660 */   -10, 1423, 1447,  810,  -10, 1557,  904,  -10,  -10,
};
#define YY_REDUCE_USE_DFLT (-114)
static short yy_reduce_ofst[] = {
 /*     0 */   -91, -114, -100, -114, -114, -114, -114, -114, -114, -114,
 /*    10 */  -114, -114, 1598, -114, -114, 1553, -114, 1620, -114,  778,
 /*    20 */  -114, -114,  837, -114, -114,  595, -114,  581, -114, -114,
 /*    30 */  -114, -114, -114, -114, -114, -114, -114, -114, -114, -114,
 /*    40 */  -114, -114, -114, -114, -114, -114, -114, -114, -114,  489,
 /*    50 */  -114, -114, -114, -113, -114, -114, 1123, -114, -114, -114,
 /*    60 */  -114, -114, -114, -114, -114, -114, -114, -114, -114, -114,
 /*    70 */  1452, -114, -114, -114, 1612, -114, -114, -114, -114, -114,
 /*    80 */  -114, -114, -114, -114, -114, -114,  534,  837, -114, -114,
 /*    90 */   411, -114,  780, -114, -114, -114, -114, -114,  687, -114,
 /*   100 */  -114, -114, -114, -114, -114, 1432, -114, -114, 1094, -114,
 /*   110 */   -76,  101, -114,  -86, -114, -114, -114, 2218,  -85, -114,
 /*   120 */   837, -114, -114,  -31, -114, 1943, -114, 2403, -114, -114,
 /*   130 */  -114, -114, -114, -114, -114, -114, -114, -114, -114, -114,
 /*   140 */  -114, -114, -114, -114, -114, -114, -114, -114, -114, -114,
 /*   150 */  -114, -114, -114, -114, -114, 2223, -114, 2265, 2403, -114,
 /*   160 */  -114, 1897, 2403, -114, 2407, -114,  840, -114,  331, -114,
 /*   170 */  -114, 1585, -114, 2081, 2403, -114, 2357, 2403, -114, 2131,
 /*   180 */  -114, 1667, 2403, -114, -114, 1483, 2403, -114, 2269, -114,
 /*   190 */  -114, 3898, -114, 3845, 1278, -114, -114, 3918, 3933, -114,
 /*   200 */  1035,  299, -114, 3746, 3937, -114,  483, -114,  759, -114,
 /*   210 */  1127, -114, 3886, -114, 3914, -114,  575, -114,  851, -114,
 /*   220 */  1219, -114, 1403, -114, 3689, -114, 3648, -114, 1344, -114,
 /*   230 */  1252, -114,  207, -114, 1442, -114, -114, -114, 3632, -114,
 /*   240 */  -114, -114, 1160, -114, -114, -114, -114,  424, -114, -114,
 /*   250 */  -114, 1376, -114, -114, 3882, -114, -114, -114, -114, -114,
 /*   260 */  -114, -114, -114, -114, 3173, -114, -114, -114, -114,  516,
 /*   270 */  -114, -114,  332, -114, -114, -114,  976, -114, -114, -114,
 /*   280 */   240, -114, -114, -114, -114, -114, -114, -114, -114, -114,
 /*   290 */   700, -114, -114, -114, -114, -114, -114,  884, -114, -114,
 /*   300 */  -114, -114, -114, -114, -114,   56, -114, -114, -114, -114,
 /*   310 */  -114, -114, -114, 1068, -114, -114,  792, -114, -114, -114,
 /*   320 */  -114, -114, -114, -114, -114, 3102, -114, -114, 2464, -114,
 /*   330 */  -114, -114, -114, -114, -114, -114, -114, -114,  148, -114,
 /*   340 */  -114, -114, -114, -114,  608, -114, -114, -114, -114, -114,
 /*   350 */  -114, -114, -114, -114, -114, -114, -114, -114, -114, -114,
 /*   360 */  -114, -114, -114, 3902, -114, 2311, 2403, -114,  934, -114,
 /*   370 */  -114, 1340, -114, 1204, -114, -114, -114, -114, -114, -114,
 /*   380 */  3684, -114, -114, -114,  695, 3741, -114, 3786,  596, 3801,
 /*   390 */  -114, -114, 1576, 2984, -114, -114, -114, -114, -114, -114,
 /*   400 */  1524, 3371, -114, -114, 1486, 3672, -114, -114, 1480, 3615,
 /*   410 */  -114, 3098, 1493, 3481, -114, -114, 1533, 3227, -114, 2633,
 /*   420 */   157, 2811, -114, -114,  801, 3729, -114, -114, -114, -114,
 /*   430 */  2363, -114, -114, 2316, -114, -114, -114, -114, -114, -114,
 /*   440 */  -114, -114, -114, -114, -114,  391, -114, -114, -114, -114,
 /*   450 */  -114, -114, -114, -114, 3849, -114, -114, -114,  667, -114,
 /*   460 */  -114,  -71, -114, -114, 1419, -114, -114, 1435, -114, -114,
 /*   470 */    83, -114, -114,  300, -114, -114,  463, -114, -114,  550,
 /*   480 */  -114, -114,  555, -114, -114,  576, -114, -114,  636, -114,
 /*   490 */  -114,  668, -114, -114,  760, -114, -114, -114, -114, -114,
 /*   500 */  -114, -114, -114, -114, -114, -114, -114, -114, -114,  943,
 /*   510 */  -114, 2219, 2403, -114, -114, -114, 1311, -114, 1529, 2403,
 /*   520 */  -114, 3141, -114, 2035, 2403, -114, -114, 3620, -114, -114,
 /*   530 */  -114, -114, -114, 1713, 2403, -114, -114, -114, -114, 1805,
 /*   540 */  2403, -114, -114,  857, -114, 1989, -114, 2403, -114, -114,
 /*   550 */  -114, 1759, -114, 2403, -114,    0, -114, 2173, -114, 2403,
 /*   560 */  -114,  902,  837, -114, -114,  503, -114, 1575, -114, 2403,
 /*   570 */  -114,  397, -114, 2127, -114, 2403, -114, -114, -114, 1851,
 /*   580 */  -114, 2403, -114,  -48, -114, 1621, -114, 2403, -114,  319,
 /*   590 */   -85, -114, -114,  902, -114, -114, 1085, -114, -114, -114,
 /*   600 */  -114, 1157, -114, -114, -114,  994,  837, 1233, -114, 1243,
 /*   610 */  1245, -114, -114, -114,  879, -114, -114, -114, 1318, -114,
 /*   620 */  -114, -114, -114, -114, -114, -114,  837, 1343, -114, 1361,
 /*   630 */  1371, -114, -114, -114, -114, -114, -114,   43,  227,  -85,
 /*   640 */   994,  -85, -114,  -85, 1443, -114, -114, -114,  243, 1488,
 /*   650 */  -114, -114, -114, 1596, -114, -114, 1041, -114,  650, 1295,
 /*   660 */  -114, -114, -114, 1438, -114, -114, 1456, -114, -114,
};
static YYACTIONTYPE yy_default[] = {
 /*     0 */   981,  981,  981,  671,  673,  674,  675,  676,  981,  981,
 /*    10 */   677,  981,  981,  678,  981,  981,  679,  981,  694,  981,
 /*    20 */   695,  725,  981,  981,  981,  981,  981,  981,  981,  745,
 /*    30 */   763,  764,  981,  981,  773,  774,  775,  776,  777,  778,
 /*    40 */   779,  780,  781,  782,  783,  784,  749,  981,  981,  981,
 /*    50 */   981,  746,  750,  981,  765,  767,  981,  770,  967,  968,
 /*    60 */   969,  970,  971,  972,  973,  981,  981,  974,  981,  768,
 /*    70 */   981,  771,  981,  769,  981,  772,  766,  753,  755,  756,
 /*    80 */   757,  758,  759,  760,  761,  762,  981,  981,  981,  981,
 /*    90 */   981,  981,  981,  981,  747,  751,  981,  981,  981,  981,
 /*   100 */   748,  752,  754,  726,  981,  981,  680,  981,  981,  681,
 /*   110 */   981,  981,  683,  981,  689,  981,  690,  981,  981,  723,
 /*   120 */   981,  981,  981,  981,  981,  981,  729,  981,  731,  785,
 /*   130 */   787,  788,  789,  790,  791,  792,  793,  794,  795,  796,
 /*   140 */   797,  798,  799,  800,  801,  802,  803,  804,  805,  806,
 /*   150 */   807,  981,  808,  981,  809,  981,  981,  981,  981,  812,
 /*   160 */   981,  981,  981,  813,  981,  981,  981,  816,  981,  817,
 /*   170 */   818,  981,  981,  820,  821,  981,  981,  981,  824,  981,
 /*   180 */   981,  981,  981,  826,  981,  981,  981,  981,  981,  981,
 /*   190 */   828,  981,  886,  981,  981,  887,  981,  981,  981,  888,
 /*   200 */   981,  981,  889,  981,  981,  890,  981,  894,  981,  895,
 /*   210 */   981,  896,  981,  897,  981,  898,  981,  908,  981,  909,
 /*   220 */   981,  910,  981,  911,  981,  912,  981,  913,  981,  914,
 /*   230 */   981,  915,  981,  916,  981,  981,  899,  981,  981,  900,
 /*   240 */   981,  981,  981,  901,  921,  981,  902,  981,  958,  981,
 /*   250 */   981,  981,  917,  918,  981,  919,  920,  922,  923,  924,
 /*   260 */   925,  926,  927,  928,  981,  966,  921,  981,  905,  981,
 /*   270 */   981,  931,  981,  932,  981,  933,  981,  981,  981,  981,
 /*   280 */   981,  981,  939,  940,  953,  954,  955,  957,  981,  981,
 /*   290 */   981,  981,  943,  944,  981,  981,  981,  981,  981,  945,
 /*   300 */   946,  956,  981,  981,  934,  981,  935,  981,  936,  981,
 /*   310 */   941,  942,  906,  981,  981,  907,  981,  981,  937,  938,
 /*   320 */   922,  923,  929,  981,  930,  981,  959,  981,  981,  961,
 /*   330 */   962,  960,  981,  947,  948,  981,  981,  903,  981,  981,
 /*   340 */   949,  950,  981,  904,  981,  981,  951,  952,  893,  892,
 /*   350 */   981,  975,  981,  976,  981,  981,  977,  981,  978,  891,
 /*   360 */   979,  981,  981,  981,  981,  981,  981,  829,  981,  981,
 /*   370 */   833,  981,  834,  981,  836,  837,  838,  839,  840,  841,
 /*   380 */   981,  842,  885,  981,  981,  981,  843,  981,  981,  981,
 /*   390 */   846,  981,  981,  981,  847,  981,  853,  854,  981,  981,
 /*   400 */   981,  981,  844,  981,  981,  981,  845,  981,  981,  981,
 /*   410 */   848,  981,  981,  981,  849,  981,  981,  981,  850,  981,
 /*   420 */   981,  981,  851,  981,  981,  981,  852,  855,  856,  835,
 /*   430 */   981,  981,  858,  981,  859,  861,  860,  981,  862,  981,
 /*   440 */   863,  981,  864,  981,  865,  981,  981,  866,  867,  981,
 /*   450 */   981,  981,  981,  868,  981,  981,  981,  869,  981,  981,
 /*   460 */   870,  981,  981,  871,  981,  881,  883,  981,  884,  882,
 /*   470 */   981,  981,  872,  981,  981,  873,  981,  981,  874,  981,
 /*   480 */   981,  875,  981,  981,  876,  981,  981,  877,  981,  981,
 /*   490 */   878,  981,  981,  879,  981,  981,  880,  981,  981,  981,
 /*   500 */   981,  981,  981,  981,  981,  981,  981,  980,  786,  981,
 /*   510 */   981,  981,  981,  830,  981,  981,  981,  981,  981,  981,
 /*   520 */   831,  981,  981,  981,  981,  832,  981,  981,  981,  827,
 /*   530 */   825,  823,  981,  981,  822,  819,  814,  810,  981,  981,
 /*   540 */   981,  815,  811,  981,  981,  981,  737,  981,  739,  981,
 /*   550 */   981,  981,  730,  981,  732,  981,  981,  981,  738,  981,
 /*   560 */   740,  981,  981,  981,  981,  981,  981,  981,  733,  981,
 /*   570 */   735,  981,  981,  981,  741,  981,  743,  981,  981,  981,
 /*   580 */   734,  981,  736,  981,  981,  981,  742,  981,  744,  981,
 /*   590 */   981,  721,  724,  981,  981,  981,  981,  981,  727,  981,
 /*   600 */   981,  981,  981,  728,  705,  981,  981,  981,  707,  981,
 /*   610 */   981,  709,  981,  713,  981,  981,  981,  981,  981,  717,
 /*   620 */   719,  981,  720,  718,  981,  711,  981,  981,  708,  981,
 /*   630 */   981,  710,  981,  714,  981,  712,  722,  981,  981,  981,
 /*   640 */   981,  981,  706,  981,  981,  691,  693,  692,  981,  981,
 /*   650 */   682,  981,  981,  981,  684,  981,  981,  685,  981,  981,
 /*   660 */   686,  981,  981,  981,  687,  981,  981,  688,  672,
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
 /* 265 */ "xx_common_expr ::= NEW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE",
 /* 266 */ "xx_common_expr ::= NEW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 267 */ "xx_common_expr ::= NEW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 268 */ "xx_fcall_expr ::= IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 269 */ "xx_fcall_expr ::= IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 270 */ "xx_fcall_expr ::= BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 271 */ "xx_fcall_expr ::= BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 272 */ "xx_scall_expr ::= IDENTIFIER DOUBLECOLON IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 273 */ "xx_scall_expr ::= IDENTIFIER DOUBLECOLON IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 274 */ "xx_scall_expr ::= BRACKET_OPEN IDENTIFIER BRACKET_CLOSE DOUBLECOLON IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 275 */ "xx_scall_expr ::= BRACKET_OPEN IDENTIFIER BRACKET_CLOSE DOUBLECOLON IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 276 */ "xx_scall_expr ::= BRACKET_OPEN IDENTIFIER BRACKET_CLOSE DOUBLECOLON BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 277 */ "xx_scall_expr ::= BRACKET_OPEN IDENTIFIER BRACKET_CLOSE DOUBLECOLON BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 278 */ "xx_mcall_expr ::= IDENTIFIER ARROW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 279 */ "xx_mcall_expr ::= IDENTIFIER ARROW IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 280 */ "xx_mcall_expr ::= IDENTIFIER ARROW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 281 */ "xx_mcall_expr ::= IDENTIFIER ARROW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 282 */ "xx_mcall_expr ::= IDENTIFIER ARROW BRACKET_OPEN STRING BRACKET_CLOSE PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 283 */ "xx_mcall_expr ::= IDENTIFIER ARROW BRACKET_OPEN STRING BRACKET_CLOSE PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 284 */ "xx_common_expr ::= xx_mcall_expr",
 /* 285 */ "xx_common_expr ::= xx_scall_expr",
 /* 286 */ "xx_common_expr ::= xx_fcall_expr",
 /* 287 */ "xx_call_parameters ::= xx_call_parameters COMMA xx_call_parameter",
 /* 288 */ "xx_call_parameters ::= xx_call_parameter",
 /* 289 */ "xx_call_parameter ::= xx_common_expr",
 /* 290 */ "xx_array_list ::= xx_array_list COMMA xx_array_item",
 /* 291 */ "xx_array_list ::= xx_array_item",
 /* 292 */ "xx_array_item ::= xx_array_key COLON xx_array_value",
 /* 293 */ "xx_array_item ::= xx_array_value",
 /* 294 */ "xx_array_key ::= IDENTIFIER",
 /* 295 */ "xx_array_key ::= STRING",
 /* 296 */ "xx_array_key ::= INTEGER",
 /* 297 */ "xx_array_value ::= xx_common_expr",
 /* 298 */ "xx_literal_expr ::= INTEGER",
 /* 299 */ "xx_literal_expr ::= CHAR",
 /* 300 */ "xx_literal_expr ::= STRING",
 /* 301 */ "xx_literal_expr ::= DOUBLE",
 /* 302 */ "xx_literal_expr ::= NULL",
 /* 303 */ "xx_literal_expr ::= FALSE",
 /* 304 */ "xx_literal_expr ::= TRUE",
 /* 305 */ "xx_literal_expr ::= IDENTIFIER DOUBLECOLON CONSTANT",
 /* 306 */ "xx_isset_expr ::= IDENTIFIER SBRACKET_OPEN xx_common_expr SBRACKET_CLOSE",
 /* 307 */ "xx_isset_expr ::= IDENTIFIER ARROW IDENTIFIER",
 /* 308 */ "xx_isset_expr ::= IDENTIFIER ARROW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE",
 /* 309 */ "xx_isset_expr ::= IDENTIFIER ARROW BRACKET_OPEN STRING BRACKET_CLOSE",
 /* 310 */ "xx_eval_expr ::= xx_common_expr",
 /* 311 */ "xx_comment ::= COMMENT",
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
// 956 "parser.lemon"
{
	/*if ((yypminor->yy0)) {
		if ((yypminor->yy0)->free_flag) {
			efree((yypminor->yy0)->token);
		}
		efree((yypminor->yy0));
	}*/
}
// 2646 "parser.c"
      break;
    case 101:
// 969 "parser.lemon"
{ json_object_put((yypminor->yy277)); }
// 2651 "parser.c"
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
  { 156, 4 },
  { 156, 6 },
  { 156, 7 },
  { 165, 4 },
  { 165, 3 },
  { 165, 6 },
  { 165, 5 },
  { 166, 6 },
  { 166, 5 },
  { 166, 8 },
  { 166, 7 },
  { 166, 10 },
  { 166, 9 },
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
// 965 "parser.lemon"
{
	status->ret = yymsp[0].minor.yy277;
}
// 3180 "parser.c"
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
      case 284:
      case 285:
      case 286:
      case 289:
      case 297:
      case 310:
// 971 "parser.lemon"
{
	yygotominor.yy277 = yymsp[0].minor.yy277;
}
// 3222 "parser.c"
        break;
      case 2:
      case 36:
      case 52:
      case 54:
      case 56:
      case 84:
      case 116:
      case 149:
// 975 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_list(yymsp[-1].minor.yy277, yymsp[0].minor.yy277);
}
// 3236 "parser.c"
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
      case 288:
      case 291:
// 979 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_list(NULL, yymsp[0].minor.yy277);
}
// 3258 "parser.c"
        break;
      case 8:
// 999 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_namespace(yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(33,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3267 "parser.c"
        break;
      case 9:
// 1003 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_interface(yymsp[-1].minor.yy0, yymsp[0].minor.yy277, NULL, status->scanner_state);
  yy_destructor(36,&yymsp[-2].minor);
}
// 3275 "parser.c"
        break;
      case 10:
// 1007 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_interface(yymsp[-3].minor.yy0, yymsp[0].minor.yy277, yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(36,&yymsp[-4].minor);
  yy_destructor(37,&yymsp[-2].minor);
}
// 3284 "parser.c"
        break;
      case 11:
// 1011 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class(yymsp[-1].minor.yy0, yymsp[0].minor.yy277, 0, 0, NULL, NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-2].minor);
}
// 3292 "parser.c"
        break;
      case 12:
// 1015 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy277, 0, 0, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-4].minor);
  yy_destructor(37,&yymsp[-2].minor);
}
// 3301 "parser.c"
        break;
      case 13:
// 1019 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy277, 0, 0, NULL, yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(38,&yymsp[-4].minor);
  yy_destructor(39,&yymsp[-2].minor);
}
// 3310 "parser.c"
        break;
      case 14:
// 1023 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class(yymsp[-5].minor.yy0, yymsp[0].minor.yy277, 0, 0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(38,&yymsp[-6].minor);
  yy_destructor(37,&yymsp[-4].minor);
  yy_destructor(39,&yymsp[-2].minor);
}
// 3320 "parser.c"
        break;
      case 15:
// 1027 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class(yymsp[-1].minor.yy0, yymsp[0].minor.yy277, 1, 0, NULL, NULL, status->scanner_state);
  yy_destructor(40,&yymsp[-3].minor);
  yy_destructor(38,&yymsp[-2].minor);
}
// 3329 "parser.c"
        break;
      case 16:
// 1031 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy277, 1, 0, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(40,&yymsp[-5].minor);
  yy_destructor(38,&yymsp[-4].minor);
  yy_destructor(37,&yymsp[-2].minor);
}
// 3339 "parser.c"
        break;
      case 17:
// 1035 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy277, 1, 0, NULL, yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(40,&yymsp[-5].minor);
  yy_destructor(38,&yymsp[-4].minor);
  yy_destructor(39,&yymsp[-2].minor);
}
// 3349 "parser.c"
        break;
      case 18:
// 1039 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class(yymsp[-1].minor.yy0, yymsp[0].minor.yy277, 0, 1, NULL, NULL, status->scanner_state);
  yy_destructor(41,&yymsp[-3].minor);
  yy_destructor(38,&yymsp[-2].minor);
}
// 3358 "parser.c"
        break;
      case 19:
// 1043 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy277, 0, 1, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(41,&yymsp[-5].minor);
  yy_destructor(38,&yymsp[-4].minor);
  yy_destructor(37,&yymsp[-2].minor);
}
// 3368 "parser.c"
        break;
      case 20:
      case 25:
      case 46:
// 1047 "parser.lemon"
{
	yygotominor.yy277 = NULL;
  yy_destructor(42,&yymsp[-1].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3379 "parser.c"
        break;
      case 21:
      case 26:
      case 47:
// 1051 "parser.lemon"
{
	yygotominor.yy277 = yymsp[-1].minor.yy277;
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3390 "parser.c"
        break;
      case 22:
      case 48:
      case 96:
      case 165:
      case 190:
      case 212:
      case 287:
      case 290:
// 1055 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_list(yymsp[-2].minor.yy277, yymsp[0].minor.yy277);
  yy_destructor(6,&yymsp[-1].minor);
}
// 3405 "parser.c"
        break;
      case 24:
      case 252:
      case 294:
// 1063 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state);
}
// 3414 "parser.c"
        break;
      case 27:
// 1075 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class_definition(yymsp[0].minor.yy277, NULL, NULL, status->scanner_state);
}
// 3421 "parser.c"
        break;
      case 28:
// 1079 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class_definition(NULL, NULL, yymsp[0].minor.yy277, status->scanner_state);
}
// 3428 "parser.c"
        break;
      case 29:
// 1083 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class_definition(NULL, yymsp[0].minor.yy277, NULL, status->scanner_state);
}
// 3435 "parser.c"
        break;
      case 30:
// 1087 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class_definition(yymsp[-1].minor.yy277, yymsp[0].minor.yy277, NULL, status->scanner_state);
}
// 3442 "parser.c"
        break;
      case 31:
// 1091 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class_definition(yymsp[-1].minor.yy277, NULL, yymsp[0].minor.yy277, status->scanner_state);
}
// 3449 "parser.c"
        break;
      case 32:
// 1095 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class_definition(yymsp[0].minor.yy277, NULL, yymsp[-1].minor.yy277, status->scanner_state);
}
// 3456 "parser.c"
        break;
      case 33:
// 1099 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class_definition(NULL, yymsp[0].minor.yy277, yymsp[-1].minor.yy277, status->scanner_state);
}
// 3463 "parser.c"
        break;
      case 34:
// 1103 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class_definition(yymsp[-2].minor.yy277, yymsp[0].minor.yy277, yymsp[-1].minor.yy277, status->scanner_state);
}
// 3470 "parser.c"
        break;
      case 35:
// 1107 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class_definition(yymsp[-1].minor.yy277, yymsp[0].minor.yy277, yymsp[-2].minor.yy277, status->scanner_state);
}
// 3477 "parser.c"
        break;
      case 38:
// 1124 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class_property(yymsp[-2].minor.yy277, yymsp[-1].minor.yy0, NULL, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(35,&yymsp[0].minor);
}
// 3485 "parser.c"
        break;
      case 39:
// 1128 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class_property(yymsp[-2].minor.yy277, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(35,&yymsp[0].minor);
}
// 3493 "parser.c"
        break;
      case 40:
// 1132 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class_property(yymsp[-4].minor.yy277, yymsp[-3].minor.yy0, yymsp[-1].minor.yy277, yymsp[-5].minor.yy0, NULL, status->scanner_state);
  yy_destructor(45,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3502 "parser.c"
        break;
      case 41:
// 1136 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class_property(yymsp[-4].minor.yy277, yymsp[-3].minor.yy0, yymsp[-1].minor.yy277, NULL, NULL, status->scanner_state);
  yy_destructor(45,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3511 "parser.c"
        break;
      case 42:
// 1140 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class_property(yymsp[-3].minor.yy277, yymsp[-2].minor.yy0, NULL, yymsp[-4].minor.yy0, yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(35,&yymsp[0].minor);
}
// 3519 "parser.c"
        break;
      case 43:
// 1144 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class_property(yymsp[-3].minor.yy277, yymsp[-2].minor.yy0, NULL, NULL, yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(35,&yymsp[0].minor);
}
// 3527 "parser.c"
        break;
      case 44:
// 1148 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class_property(yymsp[-5].minor.yy277, yymsp[-4].minor.yy0, yymsp[-2].minor.yy277, yymsp[-6].minor.yy0, yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(45,&yymsp[-3].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3536 "parser.c"
        break;
      case 45:
// 1152 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class_property(yymsp[-5].minor.yy277, yymsp[-4].minor.yy0, yymsp[-2].minor.yy277, NULL, yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(45,&yymsp[-3].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3545 "parser.c"
        break;
      case 50:
// 1172 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_property_shortcut(NULL, yymsp[0].minor.yy0, status->scanner_state);
}
// 3552 "parser.c"
        break;
      case 51:
// 1176 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_property_shortcut(yymsp[-1].minor.yy0, yymsp[0].minor.yy0, status->scanner_state);
}
// 3559 "parser.c"
        break;
      case 58:
// 1205 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy277, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(46,&yymsp[-4].minor);
  yy_destructor(45,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3569 "parser.c"
        break;
      case 59:
// 1209 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy277, NULL, status->scanner_state);
  yy_destructor(46,&yymsp[-4].minor);
  yy_destructor(45,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3579 "parser.c"
        break;
      case 60:
// 1214 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class_method(yymsp[-6].minor.yy277, yymsp[-4].minor.yy0, NULL, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(48,&yymsp[-5].minor);
  yy_destructor(49,&yymsp[-3].minor);
  yy_destructor(32,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[-1].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3591 "parser.c"
        break;
      case 61:
// 1218 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class_method(yymsp[-7].minor.yy277, yymsp[-5].minor.yy0, yymsp[-3].minor.yy277, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(48,&yymsp[-6].minor);
  yy_destructor(49,&yymsp[-4].minor);
  yy_destructor(32,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[-1].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3603 "parser.c"
        break;
      case 62:
// 1222 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class_method(yymsp[-7].minor.yy277, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy277, NULL, NULL, status->scanner_state);
  yy_destructor(48,&yymsp[-6].minor);
  yy_destructor(49,&yymsp[-4].minor);
  yy_destructor(32,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3615 "parser.c"
        break;
      case 63:
// 1226 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class_method(yymsp[-8].minor.yy277, yymsp[-6].minor.yy0, yymsp[-4].minor.yy277, yymsp[-1].minor.yy277, NULL, NULL, status->scanner_state);
  yy_destructor(48,&yymsp[-7].minor);
  yy_destructor(49,&yymsp[-5].minor);
  yy_destructor(32,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3627 "parser.c"
        break;
      case 64:
// 1230 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class_method(yymsp[-6].minor.yy277, yymsp[-4].minor.yy0, NULL, NULL, yymsp[-7].minor.yy0, NULL, status->scanner_state);
  yy_destructor(48,&yymsp[-5].minor);
  yy_destructor(49,&yymsp[-3].minor);
  yy_destructor(32,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[-1].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3639 "parser.c"
        break;
      case 65:
// 1234 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class_method(yymsp[-7].minor.yy277, yymsp[-5].minor.yy0, yymsp[-3].minor.yy277, NULL, yymsp[-8].minor.yy0, NULL, status->scanner_state);
  yy_destructor(48,&yymsp[-6].minor);
  yy_destructor(49,&yymsp[-4].minor);
  yy_destructor(32,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[-1].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3651 "parser.c"
        break;
      case 66:
// 1238 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class_method(yymsp[-7].minor.yy277, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy277, yymsp[-8].minor.yy0, NULL, status->scanner_state);
  yy_destructor(48,&yymsp[-6].minor);
  yy_destructor(49,&yymsp[-4].minor);
  yy_destructor(32,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3663 "parser.c"
        break;
      case 67:
// 1242 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class_method(yymsp[-8].minor.yy277, yymsp[-6].minor.yy0, yymsp[-4].minor.yy277, yymsp[-1].minor.yy277, yymsp[-9].minor.yy0, NULL, status->scanner_state);
  yy_destructor(48,&yymsp[-7].minor);
  yy_destructor(49,&yymsp[-5].minor);
  yy_destructor(32,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3675 "parser.c"
        break;
      case 68:
// 1246 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class_method(yymsp[-8].minor.yy277, yymsp[-6].minor.yy0, NULL, NULL, NULL, yymsp[-2].minor.yy277, status->scanner_state);
  yy_destructor(48,&yymsp[-7].minor);
  yy_destructor(49,&yymsp[-5].minor);
  yy_destructor(32,&yymsp[-4].minor);
  yy_destructor(50,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-1].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3688 "parser.c"
        break;
      case 69:
// 1250 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class_method(yymsp[-9].minor.yy277, yymsp[-7].minor.yy0, yymsp[-5].minor.yy277, NULL, NULL, yymsp[-2].minor.yy277, status->scanner_state);
  yy_destructor(48,&yymsp[-8].minor);
  yy_destructor(49,&yymsp[-6].minor);
  yy_destructor(32,&yymsp[-4].minor);
  yy_destructor(50,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-1].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3701 "parser.c"
        break;
      case 70:
// 1254 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class_method(yymsp[-9].minor.yy277, yymsp[-7].minor.yy0, NULL, yymsp[-1].minor.yy277, NULL, yymsp[-3].minor.yy277, status->scanner_state);
  yy_destructor(48,&yymsp[-8].minor);
  yy_destructor(49,&yymsp[-6].minor);
  yy_destructor(32,&yymsp[-5].minor);
  yy_destructor(50,&yymsp[-4].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3714 "parser.c"
        break;
      case 71:
// 1258 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class_method(yymsp[-10].minor.yy277, yymsp[-8].minor.yy0, yymsp[-6].minor.yy277, yymsp[-1].minor.yy277, NULL, yymsp[-3].minor.yy277, status->scanner_state);
  yy_destructor(48,&yymsp[-9].minor);
  yy_destructor(49,&yymsp[-7].minor);
  yy_destructor(32,&yymsp[-5].minor);
  yy_destructor(50,&yymsp[-4].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3727 "parser.c"
        break;
      case 72:
// 1262 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class_method(yymsp[-8].minor.yy277, yymsp[-6].minor.yy0, NULL, NULL, yymsp[-9].minor.yy0, yymsp[-2].minor.yy277, status->scanner_state);
  yy_destructor(48,&yymsp[-7].minor);
  yy_destructor(49,&yymsp[-5].minor);
  yy_destructor(32,&yymsp[-4].minor);
  yy_destructor(50,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-1].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3740 "parser.c"
        break;
      case 73:
// 1266 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class_method(yymsp[-9].minor.yy277, yymsp[-7].minor.yy0, yymsp[-5].minor.yy277, NULL, yymsp[-10].minor.yy0, yymsp[-2].minor.yy277, status->scanner_state);
  yy_destructor(48,&yymsp[-8].minor);
  yy_destructor(49,&yymsp[-6].minor);
  yy_destructor(32,&yymsp[-4].minor);
  yy_destructor(50,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-1].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3753 "parser.c"
        break;
      case 74:
// 1270 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class_method(yymsp[-9].minor.yy277, yymsp[-7].minor.yy0, NULL, yymsp[-1].minor.yy277, yymsp[-10].minor.yy0, yymsp[-3].minor.yy277, status->scanner_state);
  yy_destructor(48,&yymsp[-8].minor);
  yy_destructor(49,&yymsp[-6].minor);
  yy_destructor(32,&yymsp[-5].minor);
  yy_destructor(50,&yymsp[-4].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3766 "parser.c"
        break;
      case 75:
// 1274 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class_method(yymsp[-10].minor.yy277, yymsp[-8].minor.yy0, yymsp[-6].minor.yy277, yymsp[-1].minor.yy277, yymsp[-11].minor.yy0, yymsp[-3].minor.yy277, status->scanner_state);
  yy_destructor(48,&yymsp[-9].minor);
  yy_destructor(49,&yymsp[-7].minor);
  yy_destructor(32,&yymsp[-5].minor);
  yy_destructor(50,&yymsp[-4].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3779 "parser.c"
        break;
      case 76:
// 1278 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class_method(yymsp[-7].minor.yy277, yymsp[-5].minor.yy0, NULL, NULL, NULL, yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(48,&yymsp[-6].minor);
  yy_destructor(49,&yymsp[-4].minor);
  yy_destructor(32,&yymsp[-3].minor);
  yy_destructor(50,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3791 "parser.c"
        break;
      case 77:
// 1282 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class_method(yymsp[-8].minor.yy277, yymsp[-6].minor.yy0, yymsp[-4].minor.yy277, NULL, NULL, yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(48,&yymsp[-7].minor);
  yy_destructor(49,&yymsp[-5].minor);
  yy_destructor(32,&yymsp[-3].minor);
  yy_destructor(50,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3803 "parser.c"
        break;
      case 78:
// 1286 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class_method(yymsp[-7].minor.yy277, yymsp[-5].minor.yy0, NULL, NULL, yymsp[-8].minor.yy0, yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(48,&yymsp[-6].minor);
  yy_destructor(49,&yymsp[-4].minor);
  yy_destructor(32,&yymsp[-3].minor);
  yy_destructor(50,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3815 "parser.c"
        break;
      case 79:
// 1290 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class_method(yymsp[-8].minor.yy277, yymsp[-6].minor.yy0, yymsp[-4].minor.yy277, NULL, yymsp[-9].minor.yy0, yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(48,&yymsp[-7].minor);
  yy_destructor(49,&yymsp[-5].minor);
  yy_destructor(32,&yymsp[-3].minor);
  yy_destructor(50,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3827 "parser.c"
        break;
      case 80:
// 1295 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class_method(yymsp[-5].minor.yy277, yymsp[-3].minor.yy0, NULL, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(48,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[-1].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3838 "parser.c"
        break;
      case 81:
// 1299 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class_method(yymsp[-6].minor.yy277, yymsp[-4].minor.yy0, yymsp[-2].minor.yy277, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(48,&yymsp[-5].minor);
  yy_destructor(49,&yymsp[-3].minor);
  yy_destructor(32,&yymsp[-1].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3849 "parser.c"
        break;
      case 82:
// 1303 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class_method(yymsp[-5].minor.yy277, yymsp[-3].minor.yy0, NULL, NULL, yymsp[-6].minor.yy0, NULL, status->scanner_state);
  yy_destructor(48,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[-1].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3860 "parser.c"
        break;
      case 83:
// 1307 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_class_method(yymsp[-6].minor.yy277, yymsp[-4].minor.yy0, yymsp[-2].minor.yy277, NULL, yymsp[-7].minor.yy0, NULL, status->scanner_state);
  yy_destructor(48,&yymsp[-5].minor);
  yy_destructor(49,&yymsp[-3].minor);
  yy_destructor(32,&yymsp[-1].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3871 "parser.c"
        break;
      case 86:
// 1320 "parser.lemon"
{
	yygotominor.yy277 = json_object_new_string("public");
  yy_destructor(1,&yymsp[0].minor);
}
// 3879 "parser.c"
        break;
      case 87:
// 1324 "parser.lemon"
{
	yygotominor.yy277 = json_object_new_string("protected");
  yy_destructor(2,&yymsp[0].minor);
}
// 3887 "parser.c"
        break;
      case 88:
// 1328 "parser.lemon"
{
	yygotominor.yy277 = json_object_new_string("private");
  yy_destructor(4,&yymsp[0].minor);
}
// 3895 "parser.c"
        break;
      case 89:
// 1332 "parser.lemon"
{
	yygotominor.yy277 = json_object_new_string("static");
  yy_destructor(3,&yymsp[0].minor);
}
// 3903 "parser.c"
        break;
      case 90:
// 1336 "parser.lemon"
{
	yygotominor.yy277 = json_object_new_string("scoped");
  yy_destructor(5,&yymsp[0].minor);
}
// 3911 "parser.c"
        break;
      case 91:
// 1340 "parser.lemon"
{
	yygotominor.yy277 = json_object_new_string("inline");
  yy_destructor(51,&yymsp[0].minor);
}
// 3919 "parser.c"
        break;
      case 92:
// 1344 "parser.lemon"
{
	yygotominor.yy277 = json_object_new_string("abstract");
  yy_destructor(40,&yymsp[0].minor);
}
// 3927 "parser.c"
        break;
      case 93:
// 1348 "parser.lemon"
{
	yygotominor.yy277 = json_object_new_string("final");
  yy_destructor(41,&yymsp[0].minor);
}
// 3935 "parser.c"
        break;
      case 98:
// 1370 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_parameter(NULL, NULL, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 3942 "parser.c"
        break;
      case 99:
// 1374 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_parameter(yymsp[-1].minor.yy277, NULL, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 3949 "parser.c"
        break;
      case 100:
// 1378 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_parameter(NULL, yymsp[-1].minor.yy277, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 3956 "parser.c"
        break;
      case 101:
// 1382 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_parameter(NULL, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy277, status->scanner_state);
  yy_destructor(45,&yymsp[-1].minor);
}
// 3964 "parser.c"
        break;
      case 102:
// 1386 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_parameter(yymsp[-3].minor.yy277, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy277, status->scanner_state);
  yy_destructor(45,&yymsp[-1].minor);
}
// 3972 "parser.c"
        break;
      case 103:
// 1390 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_parameter(NULL, yymsp[-3].minor.yy277, yymsp[-2].minor.yy0, yymsp[0].minor.yy277, status->scanner_state);
  yy_destructor(45,&yymsp[-1].minor);
}
// 3980 "parser.c"
        break;
      case 104:
// 1394 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(15,&yymsp[-2].minor);
  yy_destructor(16,&yymsp[0].minor);
}
// 3989 "parser.c"
        break;
      case 105:
// 1398 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_type(XX_TYPE_INTEGER);
  yy_destructor(52,&yymsp[0].minor);
}
// 3997 "parser.c"
        break;
      case 106:
// 1402 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_type(XX_TYPE_UINTEGER);
  yy_destructor(53,&yymsp[0].minor);
}
// 4005 "parser.c"
        break;
      case 107:
// 1406 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_type(XX_TYPE_LONG);
  yy_destructor(54,&yymsp[0].minor);
}
// 4013 "parser.c"
        break;
      case 108:
// 1410 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_type(XX_TYPE_ULONG);
  yy_destructor(55,&yymsp[0].minor);
}
// 4021 "parser.c"
        break;
      case 109:
// 1414 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_type(XX_TYPE_CHAR);
  yy_destructor(56,&yymsp[0].minor);
}
// 4029 "parser.c"
        break;
      case 110:
// 1418 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_type(XX_TYPE_UCHAR);
  yy_destructor(57,&yymsp[0].minor);
}
// 4037 "parser.c"
        break;
      case 111:
// 1422 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_type(XX_TYPE_DOUBLE);
  yy_destructor(58,&yymsp[0].minor);
}
// 4045 "parser.c"
        break;
      case 112:
// 1426 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_type(XX_TYPE_BOOL);
  yy_destructor(59,&yymsp[0].minor);
}
// 4053 "parser.c"
        break;
      case 113:
// 1430 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_type(XX_TYPE_STRING);
  yy_destructor(60,&yymsp[0].minor);
}
// 4061 "parser.c"
        break;
      case 114:
// 1434 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_type(XX_TYPE_ARRAY);
  yy_destructor(61,&yymsp[0].minor);
}
// 4069 "parser.c"
        break;
      case 115:
// 1438 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_type(XX_TYPE_VAR);
  yy_destructor(62,&yymsp[0].minor);
}
// 4077 "parser.c"
        break;
      case 138:
// 1530 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_empty_statement(status->scanner_state);
  yy_destructor(35,&yymsp[0].minor);
}
// 4085 "parser.c"
        break;
      case 139:
// 1534 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_break_statement(status->scanner_state);
  yy_destructor(63,&yymsp[-1].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4094 "parser.c"
        break;
      case 140:
// 1538 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_continue_statement(status->scanner_state);
  yy_destructor(64,&yymsp[-1].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4103 "parser.c"
        break;
      case 141:
// 1542 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_if_statement(yymsp[-2].minor.yy277, NULL, NULL, status->scanner_state);
  yy_destructor(65,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-1].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 4113 "parser.c"
        break;
      case 142:
// 1546 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_if_statement(yymsp[-5].minor.yy277, NULL, NULL, status->scanner_state);
  yy_destructor(65,&yymsp[-6].minor);
  yy_destructor(42,&yymsp[-4].minor);
  yy_destructor(43,&yymsp[-3].minor);
  yy_destructor(66,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[-1].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 4126 "parser.c"
        break;
      case 143:
// 1550 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_if_statement(yymsp[-3].minor.yy277, yymsp[-1].minor.yy277, NULL, status->scanner_state);
  yy_destructor(65,&yymsp[-4].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 4136 "parser.c"
        break;
      case 144:
// 1554 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_if_statement(yymsp[-7].minor.yy277, yymsp[-5].minor.yy277, yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(65,&yymsp[-8].minor);
  yy_destructor(42,&yymsp[-6].minor);
  yy_destructor(43,&yymsp[-4].minor);
  yy_destructor(66,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 4149 "parser.c"
        break;
      case 145:
// 1558 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_if_statement(yymsp[-6].minor.yy277, yymsp[-4].minor.yy277, NULL, status->scanner_state);
  yy_destructor(65,&yymsp[-7].minor);
  yy_destructor(42,&yymsp[-5].minor);
  yy_destructor(43,&yymsp[-3].minor);
  yy_destructor(66,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[-1].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 4162 "parser.c"
        break;
      case 146:
// 1562 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_if_statement(yymsp[-6].minor.yy277, NULL, yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(65,&yymsp[-7].minor);
  yy_destructor(42,&yymsp[-5].minor);
  yy_destructor(43,&yymsp[-4].minor);
  yy_destructor(66,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 4175 "parser.c"
        break;
      case 147:
// 1566 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_switch_statement(yymsp[-2].minor.yy277, NULL, status->scanner_state);
  yy_destructor(67,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-1].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 4185 "parser.c"
        break;
      case 148:
// 1570 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_switch_statement(yymsp[-3].minor.yy277, yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(67,&yymsp[-4].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 4195 "parser.c"
        break;
      case 151:
// 1582 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_case_clause(yymsp[-1].minor.yy277, NULL, status->scanner_state);
  yy_destructor(68,&yymsp[-2].minor);
  yy_destructor(69,&yymsp[0].minor);
}
// 4204 "parser.c"
        break;
      case 152:
// 1586 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_case_clause(yymsp[-2].minor.yy277, yymsp[0].minor.yy277, status->scanner_state);
  yy_destructor(68,&yymsp[-3].minor);
  yy_destructor(69,&yymsp[-1].minor);
}
// 4213 "parser.c"
        break;
      case 153:
// 1590 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_case_clause(NULL, yymsp[0].minor.yy277, status->scanner_state);
  yy_destructor(70,&yymsp[-2].minor);
  yy_destructor(69,&yymsp[-1].minor);
}
// 4222 "parser.c"
        break;
      case 154:
// 1594 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_loop_statement(NULL, status->scanner_state);
  yy_destructor(71,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[-1].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 4232 "parser.c"
        break;
      case 155:
// 1598 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_loop_statement(yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(71,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 4242 "parser.c"
        break;
      case 156:
// 1602 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_while_statement(yymsp[-2].minor.yy277, NULL, status->scanner_state);
  yy_destructor(72,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-1].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 4252 "parser.c"
        break;
      case 157:
// 1606 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_while_statement(yymsp[-3].minor.yy277, yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(72,&yymsp[-4].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 4262 "parser.c"
        break;
      case 158:
// 1610 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_do_while_statement(yymsp[-1].minor.yy277, NULL, status->scanner_state);
  yy_destructor(73,&yymsp[-5].minor);
  yy_destructor(42,&yymsp[-4].minor);
  yy_destructor(43,&yymsp[-3].minor);
  yy_destructor(72,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4274 "parser.c"
        break;
      case 159:
// 1614 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_do_while_statement(yymsp[-1].minor.yy277, yymsp[-4].minor.yy277, status->scanner_state);
  yy_destructor(73,&yymsp[-6].minor);
  yy_destructor(42,&yymsp[-5].minor);
  yy_destructor(43,&yymsp[-3].minor);
  yy_destructor(72,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4286 "parser.c"
        break;
      case 160:
// 1618 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_for_statement(yymsp[-3].minor.yy277, NULL, yymsp[-5].minor.yy0, 0, yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(74,&yymsp[-6].minor);
  yy_destructor(75,&yymsp[-4].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 4297 "parser.c"
        break;
      case 161:
// 1622 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_for_statement(yymsp[-3].minor.yy277, NULL, yymsp[-6].minor.yy0, 1, yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(74,&yymsp[-7].minor);
  yy_destructor(75,&yymsp[-5].minor);
  yy_destructor(76,&yymsp[-4].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 4309 "parser.c"
        break;
      case 162:
// 1626 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_for_statement(yymsp[-3].minor.yy277, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, 0, yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(74,&yymsp[-8].minor);
  yy_destructor(6,&yymsp[-6].minor);
  yy_destructor(75,&yymsp[-4].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 4321 "parser.c"
        break;
      case 163:
// 1630 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_for_statement(yymsp[-3].minor.yy277, yymsp[-8].minor.yy0, yymsp[-6].minor.yy0, 1, yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(74,&yymsp[-9].minor);
  yy_destructor(6,&yymsp[-7].minor);
  yy_destructor(75,&yymsp[-5].minor);
  yy_destructor(76,&yymsp[-4].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 4334 "parser.c"
        break;
      case 164:
// 1634 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_let_statement(yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(77,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4343 "parser.c"
        break;
      case 167:
// 1647 "parser.lemon"
{
	yygotominor.yy277 = json_object_new_string("assign");
  yy_destructor(45,&yymsp[0].minor);
}
// 4351 "parser.c"
        break;
      case 168:
// 1652 "parser.lemon"
{
	yygotominor.yy277 = json_object_new_string("add-assign");
  yy_destructor(78,&yymsp[0].minor);
}
// 4359 "parser.c"
        break;
      case 169:
// 1657 "parser.lemon"
{
	yygotominor.yy277 = json_object_new_string("sub-assign");
  yy_destructor(79,&yymsp[0].minor);
}
// 4367 "parser.c"
        break;
      case 170:
// 1661 "parser.lemon"
{
	yygotominor.yy277 = json_object_new_string("mult-assign");
  yy_destructor(80,&yymsp[0].minor);
}
// 4375 "parser.c"
        break;
      case 171:
// 1665 "parser.lemon"
{
	yygotominor.yy277 = json_object_new_string("div-assign");
  yy_destructor(81,&yymsp[0].minor);
}
// 4383 "parser.c"
        break;
      case 172:
// 1669 "parser.lemon"
{
	yygotominor.yy277 = json_object_new_string("concat-assign");
  yy_destructor(82,&yymsp[0].minor);
}
// 4391 "parser.c"
        break;
      case 173:
// 1674 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_let_assignment("variable", yymsp[-1].minor.yy277, yymsp[-2].minor.yy0, NULL, NULL, yymsp[0].minor.yy277, status->scanner_state);
}
// 4398 "parser.c"
        break;
      case 174:
// 1679 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_let_assignment("object-property", yymsp[-1].minor.yy277, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, yymsp[0].minor.yy277, status->scanner_state);
  yy_destructor(50,&yymsp[-3].minor);
}
// 4406 "parser.c"
        break;
      case 175:
// 1684 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_let_assignment("variable-dynamic-object-property", yymsp[-1].minor.yy277, yymsp[-6].minor.yy0, yymsp[-3].minor.yy0, NULL, yymsp[0].minor.yy277, status->scanner_state);
  yy_destructor(50,&yymsp[-5].minor);
  yy_destructor(42,&yymsp[-4].minor);
  yy_destructor(43,&yymsp[-2].minor);
}
// 4416 "parser.c"
        break;
      case 176:
// 1689 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_let_assignment("string-dynamic-object-property", yymsp[-1].minor.yy277, yymsp[-6].minor.yy0, yymsp[-3].minor.yy0, NULL, yymsp[0].minor.yy277, status->scanner_state);
  yy_destructor(50,&yymsp[-5].minor);
  yy_destructor(42,&yymsp[-4].minor);
  yy_destructor(43,&yymsp[-2].minor);
}
// 4426 "parser.c"
        break;
      case 177:
// 1694 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_let_assignment("object-property-append", yymsp[-1].minor.yy277, yymsp[-6].minor.yy0, yymsp[-4].minor.yy0, NULL, yymsp[0].minor.yy277, status->scanner_state);
  yy_destructor(50,&yymsp[-5].minor);
  yy_destructor(84,&yymsp[-3].minor);
  yy_destructor(85,&yymsp[-2].minor);
}
// 4436 "parser.c"
        break;
      case 178:
// 1699 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_let_assignment("object-property-array-index", yymsp[-1].minor.yy277, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, yymsp[-3].minor.yy277, yymsp[0].minor.yy277, status->scanner_state);
  yy_destructor(50,&yymsp[-6].minor);
  yy_destructor(84,&yymsp[-4].minor);
  yy_destructor(85,&yymsp[-2].minor);
}
// 4446 "parser.c"
        break;
      case 179:
// 1704 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_let_assignment("static-property", yymsp[-1].minor.yy277, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, yymsp[0].minor.yy277, status->scanner_state);
  yy_destructor(86,&yymsp[-3].minor);
}
// 4454 "parser.c"
        break;
      case 180:
// 1709 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_let_assignment("static-property-append", yymsp[-1].minor.yy277, yymsp[-6].minor.yy0, yymsp[-4].minor.yy0, NULL, yymsp[0].minor.yy277, status->scanner_state);
  yy_destructor(86,&yymsp[-5].minor);
  yy_destructor(84,&yymsp[-3].minor);
  yy_destructor(85,&yymsp[-2].minor);
}
// 4464 "parser.c"
        break;
      case 181:
// 1714 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_let_assignment("static-property-array-index", yymsp[-1].minor.yy277, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, yymsp[-3].minor.yy277, yymsp[0].minor.yy277, status->scanner_state);
  yy_destructor(86,&yymsp[-6].minor);
  yy_destructor(84,&yymsp[-4].minor);
  yy_destructor(85,&yymsp[-2].minor);
}
// 4474 "parser.c"
        break;
      case 182:
// 1719 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_let_assignment("variable-append", yymsp[-1].minor.yy277, yymsp[-4].minor.yy0, NULL, NULL, yymsp[0].minor.yy277, status->scanner_state);
  yy_destructor(84,&yymsp[-3].minor);
  yy_destructor(85,&yymsp[-2].minor);
}
// 4483 "parser.c"
        break;
      case 183:
// 1724 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_let_assignment("array-index", yymsp[-1].minor.yy277, yymsp[-5].minor.yy0, NULL, yymsp[-3].minor.yy277, yymsp[0].minor.yy277, status->scanner_state);
  yy_destructor(84,&yymsp[-4].minor);
  yy_destructor(85,&yymsp[-2].minor);
}
// 4492 "parser.c"
        break;
      case 184:
// 1729 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_let_assignment("incr", NULL, yymsp[-3].minor.yy0, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(50,&yymsp[-2].minor);
  yy_destructor(87,&yymsp[0].minor);
}
// 4501 "parser.c"
        break;
      case 185:
// 1734 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_let_assignment("decr", NULL, yymsp[-3].minor.yy0, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(50,&yymsp[-2].minor);
  yy_destructor(88,&yymsp[0].minor);
}
// 4510 "parser.c"
        break;
      case 186:
// 1739 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_let_assignment("incr", NULL, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(87,&yymsp[0].minor);
}
// 4518 "parser.c"
        break;
      case 187:
// 1744 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_let_assignment("decr", NULL, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(88,&yymsp[0].minor);
}
// 4526 "parser.c"
        break;
      case 189:
// 1752 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_echo_statement(yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(89,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4535 "parser.c"
        break;
      case 192:
// 1764 "parser.lemon"
{
	yygotominor.yy277 = yymsp[0].minor.yy277;;
}
// 4542 "parser.c"
        break;
      case 193:
// 1769 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_mcall_statement(yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(35,&yymsp[0].minor);
}
// 4550 "parser.c"
        break;
      case 194:
// 1774 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_fcall_statement(yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(35,&yymsp[0].minor);
}
// 4558 "parser.c"
        break;
      case 195:
// 1779 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_scall_statement(yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(35,&yymsp[0].minor);
}
// 4566 "parser.c"
        break;
      case 196:
// 1784 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_fetch_statement(yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(35,&yymsp[0].minor);
}
// 4574 "parser.c"
        break;
      case 197:
// 1789 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_return_statement(yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(90,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4583 "parser.c"
        break;
      case 198:
// 1794 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_return_statement(NULL, status->scanner_state);
  yy_destructor(90,&yymsp[-1].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4592 "parser.c"
        break;
      case 199:
// 1799 "parser.lemon"
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
// 4606 "parser.c"
        break;
      case 200:
// 1808 "parser.lemon"
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
// 4621 "parser.c"
        break;
      case 201:
// 1817 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_throw_exception(yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(92,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4630 "parser.c"
        break;
      case 202:
// 1821 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_declare_statement(XX_T_TYPE_INTEGER, yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(52,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4639 "parser.c"
        break;
      case 203:
// 1825 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_declare_statement(XX_T_TYPE_UINTEGER, yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(53,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4648 "parser.c"
        break;
      case 204:
// 1829 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_declare_statement(XX_T_TYPE_CHAR, yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(56,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4657 "parser.c"
        break;
      case 205:
// 1833 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_declare_statement(XX_T_TYPE_UCHAR, yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(57,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4666 "parser.c"
        break;
      case 206:
// 1837 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_declare_statement(XX_T_TYPE_LONG, yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4675 "parser.c"
        break;
      case 207:
// 1841 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_declare_statement(XX_T_TYPE_ULONG, yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(55,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4684 "parser.c"
        break;
      case 208:
// 1845 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_declare_statement(XX_T_TYPE_DOUBLE, yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(58,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4693 "parser.c"
        break;
      case 209:
// 1849 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_declare_statement(XX_T_TYPE_STRING, yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(60,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4702 "parser.c"
        break;
      case 210:
// 1853 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_declare_statement(XX_T_TYPE_BOOL, yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(59,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4711 "parser.c"
        break;
      case 211:
// 1857 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_declare_statement(XX_T_TYPE_VAR, yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(62,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4720 "parser.c"
        break;
      case 214:
// 1869 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_declare_variable(yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 4727 "parser.c"
        break;
      case 215:
// 1873 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_declare_variable(yymsp[-2].minor.yy0, yymsp[0].minor.yy277, status->scanner_state);
  yy_destructor(45,&yymsp[-1].minor);
}
// 4735 "parser.c"
        break;
      case 217:
// 1881 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_expr("not", yymsp[0].minor.yy277, NULL, NULL, status->scanner_state);
  yy_destructor(31,&yymsp[-1].minor);
}
// 4743 "parser.c"
        break;
      case 218:
// 1885 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_expr("isset", yymsp[0].minor.yy277, NULL, NULL, status->scanner_state);
  yy_destructor(27,&yymsp[-1].minor);
}
// 4751 "parser.c"
        break;
      case 219:
// 1889 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_expr("require", yymsp[0].minor.yy277, NULL, NULL, status->scanner_state);
  yy_destructor(7,&yymsp[-1].minor);
}
// 4759 "parser.c"
        break;
      case 220:
// 1893 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_expr("clone", yymsp[0].minor.yy277, NULL, NULL, status->scanner_state);
  yy_destructor(8,&yymsp[-1].minor);
}
// 4767 "parser.c"
        break;
      case 221:
// 1897 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_expr("empty", yymsp[0].minor.yy277, NULL, NULL, status->scanner_state);
  yy_destructor(9,&yymsp[-1].minor);
}
// 4775 "parser.c"
        break;
      case 222:
// 1901 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_expr("equals", yymsp[-2].minor.yy277, yymsp[0].minor.yy277, NULL, status->scanner_state);
  yy_destructor(13,&yymsp[-1].minor);
}
// 4783 "parser.c"
        break;
      case 223:
// 1905 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_expr("not-equals", yymsp[-2].minor.yy277, yymsp[0].minor.yy277, NULL, status->scanner_state);
  yy_destructor(20,&yymsp[-1].minor);
}
// 4791 "parser.c"
        break;
      case 224:
// 1909 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_expr("identical", yymsp[-2].minor.yy277, yymsp[0].minor.yy277, NULL, status->scanner_state);
  yy_destructor(14,&yymsp[-1].minor);
}
// 4799 "parser.c"
        break;
      case 225:
// 1913 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_expr("not-identical", yymsp[-2].minor.yy277, yymsp[0].minor.yy277, NULL, status->scanner_state);
  yy_destructor(19,&yymsp[-1].minor);
}
// 4807 "parser.c"
        break;
      case 226:
// 1917 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_expr("less", yymsp[-2].minor.yy277, yymsp[0].minor.yy277, NULL, status->scanner_state);
  yy_destructor(15,&yymsp[-1].minor);
}
// 4815 "parser.c"
        break;
      case 227:
// 1921 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_expr("greater", yymsp[-2].minor.yy277, yymsp[0].minor.yy277, NULL, status->scanner_state);
  yy_destructor(16,&yymsp[-1].minor);
}
// 4823 "parser.c"
        break;
      case 228:
// 1925 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_expr("less-equal", yymsp[-2].minor.yy277, yymsp[0].minor.yy277, NULL, status->scanner_state);
  yy_destructor(17,&yymsp[-1].minor);
}
// 4831 "parser.c"
        break;
      case 229:
// 1929 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_expr("greater-equal", yymsp[-2].minor.yy277, yymsp[0].minor.yy277, NULL, status->scanner_state);
  yy_destructor(18,&yymsp[-1].minor);
}
// 4839 "parser.c"
        break;
      case 230:
// 1933 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_expr("list", yymsp[-1].minor.yy277, NULL, NULL, status->scanner_state);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 4848 "parser.c"
        break;
      case 231:
// 1937 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_expr("cast", yymsp[-2].minor.yy277, yymsp[0].minor.yy277, NULL, status->scanner_state);
  yy_destructor(49,&yymsp[-3].minor);
  yy_destructor(32,&yymsp[-1].minor);
}
// 4857 "parser.c"
        break;
      case 232:
// 1941 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_expr("type-hint", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), yymsp[0].minor.yy277, NULL, status->scanner_state);
  yy_destructor(15,&yymsp[-3].minor);
  yy_destructor(16,&yymsp[-1].minor);
}
// 4866 "parser.c"
        break;
      case 233:
      case 307:
// 1945 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_expr("property-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(50,&yymsp[-1].minor);
}
// 4875 "parser.c"
        break;
      case 234:
      case 308:
// 1949 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_expr("property-dynamic-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(50,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 4886 "parser.c"
        break;
      case 235:
      case 309:
// 1953 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_expr("property-string-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_STRING, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(50,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 4897 "parser.c"
        break;
      case 236:
// 1957 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_expr("static-property-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(86,&yymsp[-1].minor);
}
// 4905 "parser.c"
        break;
      case 237:
      case 305:
// 1961 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_expr("static-constant-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(86,&yymsp[-1].minor);
}
// 4914 "parser.c"
        break;
      case 238:
      case 306:
// 1966 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), yymsp[-1].minor.yy277, NULL, status->scanner_state);
  yy_destructor(84,&yymsp[-2].minor);
  yy_destructor(85,&yymsp[0].minor);
}
// 4924 "parser.c"
        break;
      case 239:
// 1971 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_expr("add", yymsp[-2].minor.yy277, yymsp[0].minor.yy277, NULL, status->scanner_state);
  yy_destructor(21,&yymsp[-1].minor);
}
// 4932 "parser.c"
        break;
      case 240:
// 1976 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_expr("sub", yymsp[-2].minor.yy277, yymsp[0].minor.yy277, NULL, status->scanner_state);
  yy_destructor(22,&yymsp[-1].minor);
}
// 4940 "parser.c"
        break;
      case 241:
// 1981 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_expr("mul", yymsp[-2].minor.yy277, yymsp[0].minor.yy277, NULL, status->scanner_state);
  yy_destructor(24,&yymsp[-1].minor);
}
// 4948 "parser.c"
        break;
      case 242:
// 1986 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_expr("div", yymsp[-2].minor.yy277, yymsp[0].minor.yy277, NULL, status->scanner_state);
  yy_destructor(25,&yymsp[-1].minor);
}
// 4956 "parser.c"
        break;
      case 243:
// 1991 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_expr("mod", yymsp[-2].minor.yy277, yymsp[0].minor.yy277, NULL, status->scanner_state);
  yy_destructor(26,&yymsp[-1].minor);
}
// 4964 "parser.c"
        break;
      case 244:
// 1996 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_expr("concat", yymsp[-2].minor.yy277, yymsp[0].minor.yy277, NULL, status->scanner_state);
  yy_destructor(23,&yymsp[-1].minor);
}
// 4972 "parser.c"
        break;
      case 245:
// 2001 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_expr("and", yymsp[-2].minor.yy277, yymsp[0].minor.yy277, NULL, status->scanner_state);
  yy_destructor(11,&yymsp[-1].minor);
}
// 4980 "parser.c"
        break;
      case 246:
// 2006 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_expr("or", yymsp[-2].minor.yy277, yymsp[0].minor.yy277, NULL, status->scanner_state);
  yy_destructor(12,&yymsp[-1].minor);
}
// 4988 "parser.c"
        break;
      case 247:
// 2011 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_expr("instanceof", yymsp[-2].minor.yy277, yymsp[0].minor.yy277, NULL, status->scanner_state);
  yy_destructor(10,&yymsp[-1].minor);
}
// 4996 "parser.c"
        break;
      case 248:
// 2016 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_expr("fetch", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), yymsp[0].minor.yy277, NULL, status->scanner_state);
  yy_destructor(28,&yymsp[-3].minor);
  yy_destructor(6,&yymsp[-1].minor);
}
// 5005 "parser.c"
        break;
      case 250:
// 2026 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_expr("typeof", yymsp[0].minor.yy277, NULL, NULL, status->scanner_state);
  yy_destructor(29,&yymsp[-1].minor);
}
// 5013 "parser.c"
        break;
      case 251:
// 2031 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_literal(XX_T_CONSTANT, yymsp[0].minor.yy0, status->scanner_state);
}
// 5020 "parser.c"
        break;
      case 253:
      case 296:
      case 298:
// 2041 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_literal(XX_T_INTEGER, yymsp[0].minor.yy0, status->scanner_state);
}
// 5029 "parser.c"
        break;
      case 254:
      case 295:
      case 300:
// 2046 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_literal(XX_T_STRING, yymsp[0].minor.yy0, status->scanner_state);
}
// 5038 "parser.c"
        break;
      case 255:
      case 299:
// 2051 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_literal(XX_T_CHAR, yymsp[0].minor.yy0, status->scanner_state);
}
// 5046 "parser.c"
        break;
      case 256:
      case 301:
// 2056 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_literal(XX_T_DOUBLE, yymsp[0].minor.yy0, status->scanner_state);
}
// 5054 "parser.c"
        break;
      case 257:
      case 302:
// 2061 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_literal(XX_T_NULL, NULL, status->scanner_state);
  yy_destructor(96,&yymsp[0].minor);
}
// 5063 "parser.c"
        break;
      case 258:
      case 304:
// 2066 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_literal(XX_T_TRUE, NULL, status->scanner_state);
  yy_destructor(97,&yymsp[0].minor);
}
// 5072 "parser.c"
        break;
      case 259:
      case 303:
// 2071 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_literal(XX_T_FALSE, NULL, status->scanner_state);
  yy_destructor(98,&yymsp[0].minor);
}
// 5081 "parser.c"
        break;
      case 260:
// 2076 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_expr("empty-array", NULL, NULL, NULL, status->scanner_state);
  yy_destructor(84,&yymsp[-1].minor);
  yy_destructor(85,&yymsp[0].minor);
}
// 5090 "parser.c"
        break;
      case 261:
// 2081 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_expr("array", yymsp[-1].minor.yy277, NULL, NULL, status->scanner_state);
  yy_destructor(84,&yymsp[-2].minor);
  yy_destructor(85,&yymsp[0].minor);
}
// 5099 "parser.c"
        break;
      case 262:
// 2086 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_new_instance(0, yymsp[0].minor.yy0, NULL, status->scanner_state);
  yy_destructor(30,&yymsp[-1].minor);
}
// 5107 "parser.c"
        break;
      case 263:
// 2091 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_new_instance(0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(30,&yymsp[-3].minor);
  yy_destructor(49,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 5117 "parser.c"
        break;
      case 264:
// 2096 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_new_instance(0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(30,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 5127 "parser.c"
        break;
      case 265:
// 2101 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_new_instance(1, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(30,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 5137 "parser.c"
        break;
      case 266:
// 2106 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_new_instance(1, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(30,&yymsp[-5].minor);
  yy_destructor(42,&yymsp[-4].minor);
  yy_destructor(43,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 5149 "parser.c"
        break;
      case 267:
// 2111 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_new_instance(1, yymsp[-4].minor.yy0, yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(30,&yymsp[-6].minor);
  yy_destructor(42,&yymsp[-5].minor);
  yy_destructor(43,&yymsp[-3].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 5161 "parser.c"
        break;
      case 268:
// 2116 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_fcall(1, yymsp[-3].minor.yy0, yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 5170 "parser.c"
        break;
      case 269:
// 2121 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_fcall(1, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(49,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 5179 "parser.c"
        break;
      case 270:
// 2126 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_fcall(2, yymsp[-4].minor.yy0, yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(42,&yymsp[-5].minor);
  yy_destructor(43,&yymsp[-3].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 5190 "parser.c"
        break;
      case 271:
// 2131 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_fcall(2, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(42,&yymsp[-4].minor);
  yy_destructor(43,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 5201 "parser.c"
        break;
      case 272:
// 2136 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_scall(0, yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(86,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 5211 "parser.c"
        break;
      case 273:
// 2141 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_scall(0, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(86,&yymsp[-3].minor);
  yy_destructor(49,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 5221 "parser.c"
        break;
      case 274:
// 2146 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_scall(1, yymsp[-6].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(42,&yymsp[-7].minor);
  yy_destructor(43,&yymsp[-5].minor);
  yy_destructor(86,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 5233 "parser.c"
        break;
      case 275:
// 2151 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_scall(1, yymsp[-5].minor.yy0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(42,&yymsp[-6].minor);
  yy_destructor(43,&yymsp[-4].minor);
  yy_destructor(86,&yymsp[-3].minor);
  yy_destructor(49,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 5245 "parser.c"
        break;
      case 276:
// 2156 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_scall(1, yymsp[-8].minor.yy0, yymsp[-4].minor.yy0, yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(42,&yymsp[-9].minor);
  yy_destructor(43,&yymsp[-7].minor);
  yy_destructor(86,&yymsp[-6].minor);
  yy_destructor(42,&yymsp[-5].minor);
  yy_destructor(43,&yymsp[-3].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 5259 "parser.c"
        break;
      case 277:
// 2161 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_scall(1, yymsp[-7].minor.yy0, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(42,&yymsp[-8].minor);
  yy_destructor(43,&yymsp[-6].minor);
  yy_destructor(86,&yymsp[-5].minor);
  yy_destructor(42,&yymsp[-4].minor);
  yy_destructor(43,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 5273 "parser.c"
        break;
      case 278:
// 2166 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_mcall(1, yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(50,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 5283 "parser.c"
        break;
      case 279:
// 2171 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_mcall(1, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(50,&yymsp[-3].minor);
  yy_destructor(49,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 5293 "parser.c"
        break;
      case 280:
// 2176 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_mcall(2, yymsp[-7].minor.yy0, yymsp[-4].minor.yy0, yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(50,&yymsp[-6].minor);
  yy_destructor(42,&yymsp[-5].minor);
  yy_destructor(43,&yymsp[-3].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 5305 "parser.c"
        break;
      case 281:
// 2181 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_mcall(2, yymsp[-6].minor.yy0, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(50,&yymsp[-5].minor);
  yy_destructor(42,&yymsp[-4].minor);
  yy_destructor(43,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 5317 "parser.c"
        break;
      case 282:
// 2186 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_mcall(3, yymsp[-7].minor.yy0, yymsp[-4].minor.yy0, yymsp[-1].minor.yy277, status->scanner_state);
  yy_destructor(50,&yymsp[-6].minor);
  yy_destructor(42,&yymsp[-5].minor);
  yy_destructor(43,&yymsp[-3].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 5329 "parser.c"
        break;
      case 283:
// 2191 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_mcall(3, yymsp[-6].minor.yy0, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(50,&yymsp[-5].minor);
  yy_destructor(42,&yymsp[-4].minor);
  yy_destructor(43,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 5341 "parser.c"
        break;
      case 292:
// 2230 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_array_item(yymsp[-2].minor.yy277, yymsp[0].minor.yy277, status->scanner_state);
  yy_destructor(69,&yymsp[-1].minor);
}
// 5349 "parser.c"
        break;
      case 293:
// 2234 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_array_item(NULL, yymsp[0].minor.yy277, status->scanner_state);
}
// 5356 "parser.c"
        break;
      case 311:
// 2310 "parser.lemon"
{
	yygotominor.yy277 = xx_ret_comment(yymsp[0].minor.yy0, status->scanner_state);
}
// 5363 "parser.c"
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
// 922 "parser.lemon"


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

// 5439 "parser.c"
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
