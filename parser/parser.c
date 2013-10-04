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

static json_object *xx_ret_return_type(json_object *type, json_object *cast, int mandatory, int is_void, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("return-type"));

	if (type) {
		json_object_object_add(ret, "data-type", type);
		json_object_object_add(ret, "mandatory", json_object_new_int(mandatory));
	}
	if (cast) {
		json_object_object_add(ret, "cast", cast);
	}

	json_object_object_add(ret, "void", json_object_new_int(is_void));

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


// 906 "parser.c"
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
#define YYNOCODE 181
#define YYACTIONTYPE unsigned short int
#define xx_TOKENTYPE xx_parser_token*
typedef union {
  xx_TOKENTYPE yy0;
  json_object* yy295;
  int yy361;
} YYMINORTYPE;
#define YYSTACKDEPTH 100
#define xx_ARG_SDECL xx_parser_status *status;
#define xx_ARG_PDECL ,xx_parser_status *status
#define xx_ARG_FETCH xx_parser_status *status = yypParser->status
#define xx_ARG_STORE yypParser->status = status
#define YYNSTATE 679
#define YYNRULE 318
#define YYERRORSYMBOL 102
#define YYERRSYMDT yy361
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
 /*     0 */   204,  213,  216,  207,  210,    3,    4,    5,    6,    7,
 /*    10 */   248,  998,    1,    2,  678,    4,    5,    6,    7,   70,
 /*    20 */    78,   56,  200,  257,  262,  278,  197,  121,  474,  252,
 /*    30 */   123,  647,  653,  652,  636,  126,   22,  285,  336,  646,
 /*    40 */   602,  125,  264,   21,  242,  108,  108,  108,   37,   38,
 /*    50 */    39,   40,   41,   42,   43,   44,   45,   46,   47,  494,
 /*    60 */   622,  240,  236,  238,  199,  209,  215,  218,  220,  222,
 /*    70 */   212,  206,  224,  226,  234,  228,  230,  232,  363,  266,
 /*    80 */   272,  257,  435,  244,  324,  253,  364,  507,  156,  265,
 /*    90 */   267,  268,  269,  270,  271,  285,  580,  517,  224,  226,
 /*   100 */   234,  228,  230,  232,  623,  979,  471,  480,  489,  492,
 /*   110 */   483,  486,  495,  501,  498,  409,  504,  157,  159,  161,
 /*   120 */   321,  170,  275,  257,  654,  181,  185,  190,  371,  507,
 /*   130 */   156,  378,  648,   67,  651,  652,  636,  285,  546,  517,
 /*   140 */   434,  601,  602,  440,  455,  459,  468,  108,  471,  480,
 /*   150 */   489,  492,  483,  486,  495,  501,  498,  431,  504,  157,
 /*   160 */   159,  161,  119,  170,  600,  413,  126,  181,  185,  190,
 /*   170 */   371,  601,  602,  378,  362,  257,  254,  108,  324,  253,
 /*   180 */   725,  507,  156,   62,  343,  440,  455,  459,  468,  285,
 /*   190 */   592,  517,  194,   60,   61,   63,   64,   66,   65,  122,
 /*   200 */   471,  480,  489,  492,  483,  486,  495,  501,  498,  203,
 /*   210 */   504,  157,  159,  161,  321,  170,  275,  257,  131,  181,
 /*   220 */   185,  190,  371,  507,  156,  378,  553,  599,  124,  614,
 /*   230 */   636,  285,  596,  517,  474,  646,  602,  440,  455,  459,
 /*   240 */   468,  108,  471,  480,  489,  492,  483,  486,  495,  501,
 /*   250 */   498,  675,  504,  157,  159,  161,  119,  170,  649,  614,
 /*   260 */   636,  181,  185,  190,  371,  482,  602,  378,  659,  257,
 /*   270 */   657,  108,  228,  230,  232,  507,  156,  381,  211,  440,
 /*   280 */   455,  459,  468,  285,  530,  517,  292,  294,  293,  261,
 /*   290 */   594,  726,   31,   33,  471,  480,  489,  492,  483,  486,
 /*   300 */   495,  501,  498,  333,  504,  157,  159,  161,  380,  170,
 /*   310 */   427,  257,  256,  181,  185,  190,  371,  507,  156,  378,
 /*   320 */   292,  294,  293,  261,  173,  285,  598,  517,  299,  295,
 /*   330 */   384,  440,  455,  459,  468,  630,  471,  480,  489,  492,
 /*   340 */   483,  486,  495,  501,  498,  631,  504,  157,  159,  161,
 /*   350 */   177,  170,  542,  481,  479,  181,  185,  190,  371,  324,
 /*   360 */   510,  378,  629,  257,  385,  386,  387,  388,  389,  507,
 /*   370 */   156,  443,  357,  440,  455,  459,  468,  285,  540,  517,
 /*   380 */   292,  294,  293,  261,   28,  332,   31,   33,  471,  480,
 /*   390 */   489,  492,  483,  486,  495,  501,  498,  508,  504,  157,
 /*   400 */   159,  161,  442,  170,  426,  257,  256,  181,  185,  190,
 /*   410 */   371,  507,  156,  378,  292,  294,  293,  261,  284,  285,
 /*   420 */   189,  517,  347,  295,  107,  440,  455,  459,  468,  365,
 /*   430 */   471,  480,  489,  492,  483,  486,  495,  501,  498,  104,
 /*   440 */   504,  157,  159,  161,  630,  170,  348,  572,  196,  181,
 /*   450 */   185,  190,  371,  625,  631,  378,  587,  257,  108,  628,
 /*   460 */    70,   78,   82,  507,  156,  423,  214,  440,  455,  459,
 /*   470 */   468,  285,  552,  517,  292,  294,  293,  261,   99,  367,
 /*   480 */    31,   33,  471,  480,  489,  492,  483,  486,  495,  501,
 /*   490 */   498,   14,  504,  157,  159,  161,   17,  170,  627,  257,
 /*   500 */   256,  181,  185,  190,  371,  507,  156,  378,  292,  294,
 /*   510 */   293,  261,  616,  285,  536,  517,  289,  295,   54,  440,
 /*   520 */   455,  459,  468,  108,  471,  480,  489,  492,  483,  486,
 /*   530 */   495,  501,  498,   51,  504,  157,  159,  161,  582,  170,
 /*   540 */    31,   33,  474,  181,  185,  190,  371,  493,  479,  378,
 /*   550 */    49,  257,  499,  479,   70,   78,   82,  507,  156,  632,
 /*   560 */   356,  440,  455,  459,  468,  285,  193,  517,  292,  294,
 /*   570 */   293,  261,  566,  491,   31,   33,  471,  480,  489,  492,
 /*   580 */   483,  486,  495,  501,  498,   81,  504,  157,  159,  161,
 /*   590 */   669,  170,  657,  257,  256,  181,  185,  190,  371,  507,
 /*   600 */   156,  378,  292,  294,  293,  261,   93,  285,  551,  517,
 /*   610 */   317,  295,  524,  440,  455,  459,  468,  108,  471,  480,
 /*   620 */   489,  492,  483,  486,  495,  501,  498,   48,  504,  157,
 /*   630 */   159,  161,   52,  170,   31,   33,   24,  181,  185,  190,
 /*   640 */   371,  175,   27,  378,  559,  257,  174,  545,   70,   78,
 /*   650 */    82,  507,  156,  419,  374,  440,  455,  459,  468,  285,
 /*   660 */   556,  517,  292,  294,  293,  261,  676,  177,  288,  542,
 /*   670 */   471,  480,  489,  492,  483,  486,  495,  501,  498,  621,
 /*   680 */   504,  157,  159,  161,  373,  170,  624,  257,  256,  181,
 /*   690 */   185,  190,  371,  507,  156,  378,  292,  294,  293,  261,
 /*   700 */   635,  285,  165,  517,  353,  295,  296,  440,  455,  459,
 /*   710 */   468,  513,  471,  480,  489,  492,  483,  486,  495,  501,
 /*   720 */   498,   59,  504,  157,  159,  161,  554,  170,   31,   33,
 /*   730 */   344,  181,  185,  190,  371,  379,  439,  378,  102,  257,
 /*   740 */   490,  479,   70,   78,   82,  507,  156,  474,  217,  440,
 /*   750 */   455,  459,  468,  285,  558,  517,  292,  294,  293,  261,
 /*   760 */   105,  515,   31,   33,  471,  480,  489,  492,  483,  486,
 /*   770 */   495,  501,  498,  345,  504,  157,  159,  161,  497,  170,
 /*   780 */   350,  257,  256,  181,  185,  190,  371,  507,  156,  378,
 /*   790 */   292,  294,  293,  261,  418,  285,  562,  517,  340,  295,
 /*   800 */   101,  440,  455,  459,  468,  415,  471,  480,  489,  492,
 /*   810 */   483,  486,  495,  501,  498,   98,  504,  157,  159,  161,
 /*   820 */    74,  170,  618,   71,   83,  181,  185,  190,  371,  624,
 /*   830 */   644,  378,  619,  257,  116,  346,  113,  119,  658,  507,
 /*   840 */   156,  119,  219,  440,  455,  459,  468,  285,  564,  517,
 /*   850 */   292,  294,  293,  261,  276,  496,  479,  474,  471,  480,
 /*   860 */   489,  492,  483,  486,  495,  501,  498,  320,  504,  157,
 /*   870 */   159,  161,  117,  170,  657,  257,  256,  181,  185,  190,
 /*   880 */   371,  507,  156,  378,  292,  294,  293,  261,  473,  285,
 /*   890 */   568,  517,  306,  295,  414,  440,  455,  459,  468,  642,
 /*   900 */   471,  480,  489,  492,  483,  486,  495,  501,  498,  577,
 /*   910 */   504,  157,  159,  161,  640,  170,  638,  581,  411,  181,
 /*   920 */   185,  190,  371,  624,  641,  378,  639,  257,  626,  633,
 /*   930 */   665,  624,  668,  507,  156,  119,  221,  440,  455,  459,
 /*   940 */   468,  285,  523,  517,  292,  294,  293,  261,  410,  472,
 /*   950 */   479,  474,  471,  480,  489,  492,  483,  486,  495,  501,
 /*   960 */   498,  643,  504,  157,  159,  161,  368,  170,  202,  257,
 /*   970 */   256,  181,  185,  190,  371,  507,  156,  378,  292,  294,
 /*   980 */   293,  261,  506,  285,  570,  517,  315,  295,  461,  440,
 /*   990 */   455,  459,  468,  474,  471,  480,  489,  492,  483,  486,
 /*  1000 */   495,  501,  498,  589,  504,  157,  159,  161,  645,  170,
 /*  1010 */   394,  593,  511,  181,  185,  190,  371,  284,  408,  378,
 /*  1020 */   512,  257,  351,  464,  503,  502,  479,  507,  156,  201,
 /*  1030 */   223,  440,  455,  459,  468,  285,  578,  517,  292,  294,
 /*  1040 */   293,  261,  403,  505,  479,  354,  471,  480,  489,  492,
 /*  1050 */   483,  486,  495,  501,  498,  402,  504,  157,  159,  161,
 /*  1060 */    58,  170,  119,  257,  256,  181,  185,  190,  371,  507,
 /*  1070 */   156,  378,  292,  294,  293,  261,  284,  285,  132,  517,
 /*  1080 */   325,  295,  399,  440,  455,  459,  468,  474,  471,  480,
 /*  1090 */   489,  492,  483,  486,  495,  501,  498,  561,  504,  157,
 /*  1100 */   159,  161,  395,  170,  307,  565,  474,  181,  185,  190,
 /*  1110 */   371,  284,  352,  378,  656,  257,  655,   25,  488,  487,
 /*  1120 */   479,  507,  156,  390,  225,  440,  455,  459,  468,  285,
 /*  1130 */   584,  517,  292,  294,  293,  261,  677,  500,  660,  316,
 /*  1140 */   471,  480,  489,  492,  483,  486,  495,  501,  498,  366,
 /*  1150 */   504,  157,  159,  161,  129,  170,  662,  257,  256,  181,
 /*  1160 */   185,  190,  371,  507,  156,  378,  292,  294,  293,  261,
 /*  1170 */    55,  285,  586,  517,  282,  295,  382,  440,  455,  459,
 /*  1180 */   468,  474,  471,  480,  489,  492,  483,  486,  495,  501,
 /*  1190 */   498,  663,  504,  157,  159,  161,  664,  170,   50,  484,
 /*  1200 */   479,  181,  185,  190,  371,   55,  297,  378,  383,  257,
 /*  1210 */    15,  667,  485,  666,  302,  507,  156,  673,  227,  440,
 /*  1220 */   455,  459,  468,  285,  547,  517,  292,  294,  293,  261,
 /*  1230 */   670,  672,  372,  103,  471,  480,  489,  492,  483,  486,
 /*  1240 */   495,  501,  498,  516,  504,  157,  159,  161,  163,  170,
 /*  1250 */    76,  257,  256,  181,  185,  190,  371,  507,  156,  378,
 /*  1260 */   292,  294,  293,  261,  284,  285,  134,  517,  514,  309,
 /*  1270 */   525,  440,  455,  459,  468,  166,  471,  480,  489,  492,
 /*  1280 */   483,  486,  495,  501,  498,  526,  504,  157,  159,  161,
 /*  1290 */   111,  170,  283,  284,  284,  181,  185,  190,  371,   55,
 /*  1300 */   279,  378,  255,  257,  112,  287,  277,  167,  310,  507,
 /*  1310 */   156,   53,  286,  440,  455,  459,  468,  285,  541,  517,
 /*  1320 */   245,  318,  290,  674,  537,  509,   75,  560,  471,  480,
 /*  1330 */   489,  492,  483,  486,  495,  501,  498,  160,  504,  157,
 /*  1340 */   159,  161,  476,  170,   36,  257,  243,  181,  185,  190,
 /*  1350 */   371,  507,  156,  378,  292,  294,  293,  261,   55,  285,
 /*  1360 */   169,  517,  298,  543,  539,  440,  455,  459,  468,  158,
 /*  1370 */   471,  480,  489,  492,  483,  486,  495,  501,  498,  114,
 /*  1380 */   504,  157,  159,  161,   73,  170,  588,  280,  172,  181,
 /*  1390 */   185,  190,  371,  284,  303,  378,  304,  257,  548,   77,
 /*  1400 */   549,  305,   72,  507,  156,  555,  229,  440,  455,  459,
 /*  1410 */   468,  285,  184,  517,  292,  294,  293,  261,   10,  115,
 /*  1420 */    13,  341,  471,  480,  489,  492,  483,  486,  495,  501,
 /*  1430 */   498,  311,  504,  157,  159,  161,  176,  170,  312,  257,
 /*  1440 */   231,  181,  185,  190,  371,  507,  156,  378,  292,  294,
 /*  1450 */   293,  261,  284,  285,  590,  517,  106,  567,  313,  440,
 /*  1460 */   455,  459,  468,  574,  471,  480,  489,  492,  483,  486,
 /*  1470 */   495,  501,  498,  179,  504,  157,  159,  161,   35,  170,
 /*  1480 */   300,  178,  575,  181,  185,  190,  371,  284,  478,  378,
 /*  1490 */   477,  257,  475,   69,  583,   68,  100,  507,  156,  182,
 /*  1500 */   233,  440,  455,  459,  468,  285,  377,  517,  292,  294,
 /*  1510 */   293,  261,  118,  467,    9,  326,  471,  480,  489,  492,
 /*  1520 */   483,  486,  495,  501,  498,  128,  504,  157,  159,  161,
 /*  1530 */    96,  170,  463,  257,  235,  181,  185,  190,  371,  507,
 /*  1540 */   156,  378,  292,  294,  293,  261,   12,  285,  535,  517,
 /*  1550 */   462,   79,   95,  440,  455,  459,  468,  595,  471,  480,
 /*  1560 */   489,  492,  483,  486,  495,  501,  498,  260,  504,  157,
 /*  1570 */   159,  161,  460,  170,  259,  258,  187,  181,  185,  190,
 /*  1580 */   371,  454,  452,  378,   32,  257,  605,  606,  607,  981,
 /*  1590 */   450,  507,  156,  608,  980,  440,  455,  459,  468,  285,
 /*  1600 */   448,  517,  250,   84,   85,   87,   86,   88,  610,  679,
 /*  1610 */   471,  480,  489,  492,  483,  486,  495,  501,  498,  611,
 /*  1620 */   504,  157,  159,  161,  249,  170,  612,  613,   80,  181,
 /*  1630 */   185,  190,  371,  154,   29,  378,  191,   16,  637,  246,
 /*  1640 */    17,  634,  620,  500,   90,   91,  384,  440,  455,  459,
 /*  1650 */   468,  500,  127,  192,  500,   89,  500,  500,  500,  518,
 /*  1660 */   136,  137,  138,  139,  140,  141,  142,  143,  144,  145,
 /*  1670 */   146,  147,  148,  149,  150,  151,  152,  153,  155,  154,
 /*  1680 */   385,  386,  387,  388,  389,  500,  421,  445,  500,  500,
 /*  1690 */   447,  449,  451,  453,  444,  292,  294,  293,  261,  376,
 /*  1700 */   500,  500,  500,  500,  500,  518,  136,  137,  138,  139,
 /*  1710 */   140,  141,  142,  143,  144,  145,  146,  147,  148,  149,
 /*  1720 */   150,  151,  152,  153,  155,  154,  500,  500,  500,  500,
 /*  1730 */   162,  500,  500,  370,  500,  500,  447,  449,  451,  453,
 /*  1740 */   500,  292,  294,  293,  261,  529,  500,  500,  500,  500,
 /*  1750 */   500,  518,  136,  137,  138,  139,  140,  141,  142,  143,
 /*  1760 */   144,  145,  146,  147,  148,  149,  150,  151,  152,  153,
 /*  1770 */   155,  154,  500,  500,  500,  186,  500,  500,  370,  500,
 /*  1780 */   500,  500,  447,  449,  451,  453,  292,  294,  293,  261,
 /*  1790 */   500,  557,  500,  500,  500,  500,  500,  518,  136,  137,
 /*  1800 */   138,  139,  140,  141,  142,  143,  144,  145,  146,  147,
 /*  1810 */   148,  149,  150,  151,  152,  153,  155,  154,  500,  500,
 /*  1820 */   500,  195,  500,  500,  370,  500,  500,  500,  447,  449,
 /*  1830 */   451,  453,  292,  294,  293,  261,  500,  133,  500,  500,
 /*  1840 */   500,  500,  500,  518,  136,  137,  138,  139,  140,  141,
 /*  1850 */   142,  143,  144,  145,  146,  147,  148,  149,  150,  151,
 /*  1860 */   152,  153,  155,  154,  500,  500,  500,  171,  500,  500,
 /*  1870 */   370,  500,  500,  500,  447,  449,  451,  453,  292,  294,
 /*  1880 */   293,  261,  500,  164,  500,  500,  500,  500,  500,  518,
 /*  1890 */   136,  137,  138,  139,  140,  141,  142,  143,  144,  145,
 /*  1900 */   146,  147,  148,  149,  150,  151,  152,  153,  155,  154,
 /*  1910 */   500,  500,  500,  538,  500,  500,  370,  500,  500,  500,
 /*  1920 */   447,  449,  451,  453,  292,  294,  293,  261,  500,  585,
 /*  1930 */   500,  500,  500,  500,  500,  518,  136,  137,  138,  139,
 /*  1940 */   140,  141,  142,  143,  144,  145,  146,  147,  148,  149,
 /*  1950 */   150,  151,  152,  153,  155,  154,  500,  500,  500,  500,
 /*  1960 */   500,  500,  237,  500,  500,  500,  447,  449,  451,  453,
 /*  1970 */   292,  294,  293,  261,  500,  563,  500,  500,  500,  500,
 /*  1980 */   500,  518,  136,  137,  138,  139,  140,  141,  142,  143,
 /*  1990 */   144,  145,  146,  147,  148,  149,  150,  151,  152,  153,
 /*  2000 */   155,  154,  500,  500,  500,  500,  500,  500,  239,  500,
 /*  2010 */   500,  500,  447,  449,  451,  453,  292,  294,  293,  261,
 /*  2020 */   500,  183,  500,  500,  500,  500,  500,  518,  136,  137,
 /*  2030 */   138,  139,  140,  141,  142,  143,  144,  145,  146,  147,
 /*  2040 */   148,  149,  150,  151,  152,  153,  155,  154,  500,  500,
 /*  2050 */   500,  500,  500,  500,  241,  500,  500,  500,  447,  449,
 /*  2060 */   451,  453,  292,  294,  293,  261,  500,  569,  500,  500,
 /*  2070 */   500,  500,  500,  518,  136,  137,  138,  139,  140,  141,
 /*  2080 */   142,  143,  144,  145,  146,  147,  148,  149,  150,  151,
 /*  2090 */   152,  153,  155,  154,  500,  500,  500,  500,  500,  500,
 /*  2100 */   247,  500,  500,  500,  447,  449,  451,  453,  292,  294,
 /*  2110 */   293,  261,  500,  534,  500,  500,  500,  500,  500,  518,
 /*  2120 */   136,  137,  138,  139,  140,  141,  142,  143,  144,  145,
 /*  2130 */   146,  147,  148,  149,  150,  151,  152,  153,  155,  154,
 /*  2140 */   500,  500,  500,  500,  500,  500,  251,  500,  500,  500,
 /*  2150 */   447,  449,  451,  453,  292,  294,  293,  261,  500,  591,
 /*  2160 */   500,  500,  500,  500,  500,  518,  136,  137,  138,  139,
 /*  2170 */   140,  141,  142,  143,  144,  145,  146,  147,  148,  149,
 /*  2180 */   150,  151,  152,  153,  155,  154,  500,  500,  500,  500,
 /*  2190 */   500,  500,  456,  500,  500,  500,  447,  449,  451,  453,
 /*  2200 */   292,  294,  293,  261,  500,  579,  500,  500,  500,  500,
 /*  2210 */   500,  518,  136,  137,  138,  139,  140,  141,  142,  143,
 /*  2220 */   144,  145,  146,  147,  148,  149,  150,  151,  152,  153,
 /*  2230 */   155,  154,  500,  500,  500,  500,  500,  500,  465,  500,
 /*  2240 */   500,  500,  447,  449,  451,  453,  292,  294,  293,  261,
 /*  2250 */   500,  522,  500,  500,  500,  500,  500,  518,  136,  137,
 /*  2260 */   138,  139,  140,  141,  142,  143,  144,  145,  146,  147,
 /*  2270 */   148,  149,  150,  151,  152,  153,  155,  154,  500,  500,
 /*  2280 */   500,  500,  500,  500,  263,  500,  500,  500,  447,  449,
 /*  2290 */   451,  453,  292,  294,  293,  261,  500,  597,  500,  500,
 /*  2300 */   500,  500,  500,  518,  136,  137,  138,  139,  140,  141,
 /*  2310 */   142,  143,  144,  145,  146,  147,  148,  149,  150,  151,
 /*  2320 */   152,  153,  155,  154,  500,  500,  500,  500,  500,  500,
 /*  2330 */   469,  500,  500,  500,  447,  449,  451,  453,  292,  294,
 /*  2340 */   293,  261,  500,  180,  500,  500,  500,  500,  500,  518,
 /*  2350 */   136,  137,  138,  139,  140,  141,  142,  143,  144,  145,
 /*  2360 */   146,  147,  148,  149,  150,  151,  152,  153,  155,  154,
 /*  2370 */   500,  500,  500,  500,  500,  500,  520,  500,  500,  500,
 /*  2380 */   447,  449,  451,  453,  292,  294,  293,  261,  500,  544,
 /*  2390 */   500,  500,  500,  500,  500,  518,  136,  137,  138,  139,
 /*  2400 */   140,  141,  142,  143,  144,  145,  146,  147,  148,  149,
 /*  2410 */   150,  151,  152,  153,  155,  154,  500,  500,  500,  500,
 /*  2420 */   500,  500,  527,  500,  500,  500,  447,  449,  451,  453,
 /*  2430 */   292,  294,  293,  261,  500,  188,  500,  500,  500,  500,
 /*  2440 */   500,  518,  136,  137,  138,  139,  140,  141,  142,  143,
 /*  2450 */   144,  145,  146,  147,  148,  149,  150,  151,  152,  153,
 /*  2460 */   155,  154,  500,  500,  500,  500,  500,  500,  532,  500,
 /*  2470 */   500,  500,  447,  449,  451,  453,  292,  294,  293,  261,
 /*  2480 */   500,  168,  500,  500,  500,  500,  500,  518,  136,  137,
 /*  2490 */   138,  139,  140,  141,  142,  143,  144,  145,  146,  147,
 /*  2500 */   148,  149,  150,  151,  152,  153,  155,  154,  500,  500,
 /*  2510 */   500,  500,  500,  500,  322,  500,  500,  500,  447,  449,
 /*  2520 */   451,  453,  292,  294,  293,  261,  500,  550,  500,  500,
 /*  2530 */   500,  500,  500,  518,  136,  137,  138,  139,  140,  141,
 /*  2540 */   142,  143,  144,  145,  146,  147,  148,  149,  150,  151,
 /*  2550 */   152,  153,  155,  154,  500,  500,  500,  500,  500,  500,
 /*  2560 */   500,  500,  445,  500,  447,  449,  451,  453,  441,  446,
 /*  2570 */   292,  294,  293,  261,  500,  500,  500,  500,  500,  135,
 /*  2580 */   136,  137,  138,  139,  140,  141,  142,  143,  144,  145,
 /*  2590 */   146,  147,  148,  149,  150,  151,  152,  153,  155,  204,
 /*  2600 */   213,  216,  207,  210,  500,  500,  273,  500,  500,  248,
 /*  2610 */   447,  449,  451,  453,  292,  294,  293,  261,  500,  500,
 /*  2620 */   500,  200,  257,  262,  278,  197,  337,  500,  274,  204,
 /*  2630 */   213,  216,  207,  210,  500,  500,  285,    8,  500,  248,
 /*  2640 */    11,  264,  110,  242,  661,  671,  500,  500,  517,  500,
 /*  2650 */   500,  200,  257,  262,  278,  197,  314,   34,  252,  392,
 /*  2660 */   500,  500,  500,  432,  500,  500,  285,  292,  294,  293,
 /*  2670 */   261,  264,  500,  242,   26,  500,   57,  500,  329,  272,
 /*  2680 */   330,  500,  500,  500,  500,  500,  500,  500,  328,  267,
 /*  2690 */   268,  269,  270,  271,  500,   37,   38,   39,   40,   41,
 /*  2700 */    42,   43,   44,   45,   46,   47,  392,  500,  266,  272,
 /*  2710 */   420,  500,  500,  500,  292,  294,  293,  261,  265,  267,
 /*  2720 */   268,  269,  270,  271,  500,  500,  204,  213,  216,  207,
 /*  2730 */   210,   84,   85,   87,   86,   88,  248,  500,  500,  500,
 /*  2740 */   500,  500,  500,  500,  500,  500,  500,  500,  200,  257,
 /*  2750 */   262,  278,  197,  291,  500,  252,  204,  213,  216,  207,
 /*  2760 */   210,  680,  500,  285,  500,  500,  248,  500,  264,  500,
 /*  2770 */   242,  500,   90,   91,  500,   18,   92,  500,  200,  257,
 /*  2780 */   262,  278,  197,   89,  500,  252,  500,  500,  500,  500,
 /*  2790 */   500,  500,  500,  285,  500,  500,    8,  500,  264,   11,
 /*  2800 */   242,  110,  198,  661,  671,  266,  272,  517,  500,  500,
 /*  2810 */   292,  294,  293,  261,  500,  265,  267,  268,  269,  270,
 /*  2820 */   271,  199,  209,  215,  218,  220,  222,  212,  206,  224,
 /*  2830 */   226,  234,  228,  230,  232,  266,  272,  398,  500,  500,
 /*  2840 */   500,  500,  500,  500,  500,  265,  267,  268,  269,  270,
 /*  2850 */   271,  273,  500,  204,  213,  216,  207,  210,  500,  292,
 /*  2860 */   294,  293,  261,  248,  500,  500,  331,  500,  500,  339,
 /*  2870 */   335,  338,  500,  500,  500,  200,  257,  262,  278,  197,
 /*  2880 */   319,  500,  252,  204,  213,  216,  207,  210,  500,  500,
 /*  2890 */   285,  405,  500,  248,  500,  264,  425,  242,  500,  292,
 /*  2900 */   294,  293,  261,  500,  500,  200,  257,  262,  278,  197,
 /*  2910 */   281,   34,  252,  405,  500,  500,  500,  500,  433,  500,
 /*  2920 */   285,  292,  294,  293,  261,  264,  405,  242,  130,  500,
 /*  2930 */    57,  401,  266,  272,  292,  294,  293,  261,  500,  500,
 /*  2940 */   500,  500,  265,  267,  268,  269,  270,  271,  500,   37,
 /*  2950 */    38,   39,   40,   41,   42,   43,   44,   45,   46,   47,
 /*  2960 */   392,  500,  266,  272,  404,  500,  500,  500,  292,  294,
 /*  2970 */   293,  261,  265,  267,  268,  269,  270,  271,  273,  500,
 /*  2980 */   204,  213,  216,  207,  210,  500,  292,  294,  293,  261,
 /*  2990 */   248,  500,  500,  500,  500,  500,  334,  335,  338,  500,
 /*  3000 */   500,  500,  200,  257,  262,  278,  197,  500,  500,  252,
 /*  3010 */   204,  213,  216,  207,  210,  500,  500,  285,  500,  392,
 /*  3020 */   248,  500,  264,  424,  242,  500,  500,  292,  294,  293,
 /*  3030 */   261,  500,  200,  257,  262,  278,  197,  327,   34,  252,
 /*  3040 */   392,  500,  500,  500,  400,  500,  500,  285,  292,  294,
 /*  3050 */   293,  261,  264,  500,  242,   97,  369,   57,  500,  266,
 /*  3060 */   272,  430,  500,  500,  292,  294,  293,  261,  500,  265,
 /*  3070 */   267,  268,  269,  270,  271,  500,   37,   38,   39,   40,
 /*  3080 */    41,   42,   43,   44,   45,   46,   47,  392,  500,  266,
 /*  3090 */   272,  396,  500,  500,  500,  292,  294,  293,  261,  265,
 /*  3100 */   267,  268,  269,  270,  271,  500,  500,  204,  213,  216,
 /*  3110 */   207,  210,  392,  500,  500,  500,  391,  248,  500,  500,
 /*  3120 */   292,  294,  293,  261,  500,  500,  500,  500,  500,  200,
 /*  3130 */   257,  262,  278,  197,  384,  500,  252,  500,  204,  213,
 /*  3140 */   216,  207,  210,  500,  285,  500,  392,  500,  248,  264,
 /*  3150 */   428,  242,  500,  500,  292,  294,  293,  261,  500,  500,
 /*  3160 */   200,  257,  262,  278,  197,  500,  500,  252,  385,  386,
 /*  3170 */   387,  388,  389,  500,  397,  285,  500,  406,  407,  531,
 /*  3180 */   264,  500,  242,  500,  392,  500,  266,  272,  412,  500,
 /*  3190 */   500,  500,  292,  294,  293,  261,  265,  267,  268,  269,
 /*  3200 */   270,  271,   84,   85,   87,   86,   88,  500,  500,  500,
 /*  3210 */   519,  392,  500,  500,  500,  436,  500,  266,  272,  292,
 /*  3220 */   294,  293,  261,  500,  500,  500,  500,  265,  267,  268,
 /*  3230 */   269,  270,  271,  500,  500,  204,  213,  216,  207,  210,
 /*  3240 */   500,  500,  360,   90,   91,  248,  707,  615,  500,  609,
 /*  3250 */   292,  294,  293,  261,   89,  500,  500,  200,  257,  262,
 /*  3260 */   278,  197,  500,  500,  252,  204,  213,  216,  207,  210,
 /*  3270 */   500,  500,  285,  500,  500,  248,  500,  264,  500,  242,
 /*  3280 */   500,  500,  500,  500,  500,  500,  500,  200,  257,  262,
 /*  3290 */   278,  197,  308,   34,  252,  500,  500,  500,  500,  500,
 /*  3300 */   500,   19,  285,  500,  500,  500,   22,  264,  500,  242,
 /*  3310 */   576,  205,   57,  109,  266,  272,  422,  108,  500,  292,
 /*  3320 */   294,  293,  261,  500,  265,  267,  268,  269,  270,  271,
 /*  3330 */   500,   37,   38,   39,   40,   41,   42,   43,   44,   45,
 /*  3340 */    46,   47,  392,  500,  266,  272,  416,  500,  500,  500,
 /*  3350 */   292,  294,  293,  261,  265,  267,  268,  269,  270,  271,
 /*  3360 */   500,  500,  204,  213,  216,  207,  210,  500,  500,  500,
 /*  3370 */   359,  500,  248,  500,  500,  500,  500,  500,  292,  294,
 /*  3380 */   293,  261,  500,  500,  200,  257,  262,  278,  197,  301,
 /*  3390 */   500,  252,  204,  213,  216,  207,  210,  500,  500,  285,
 /*  3400 */   500,  500,  248,  500,  264,  500,  242,  500,  500,  500,
 /*  3410 */   500,  500,  500,  500,  200,  257,  262,  278,  197,  355,
 /*  3420 */    34,  252,  500,  500,  500,  500,  500,  500,  500,  285,
 /*  3430 */   500,  500,  500,  500,  264,  500,  242,  208,  500,  500,
 /*  3440 */   500,  266,  272,  500,  500,  292,  294,  293,  261,  500,
 /*  3450 */   500,  265,  267,  268,  269,  270,  271,   30,   37,   38,
 /*  3460 */    39,   40,   41,   42,   43,   44,   45,   46,   47,  358,
 /*  3470 */   500,  266,  272,  500,  500,  500,  500,  292,  294,  293,
 /*  3480 */   261,  265,  267,  268,  269,  270,  271,  500,  500,  204,
 /*  3490 */   213,  216,  207,  210,  500,  500,  500,  500,  500,  248,
 /*  3500 */   500,  500,  500,  500,  500,  500,  500,  500,  500,  500,
 /*  3510 */   500,  200,  257,  262,  278,  197,  500,  500,  252,  458,
 /*  3520 */   204,  213,  216,  207,  210,  500,  285,  500,  500,  500,
 /*  3530 */   248,  264,  500,  242,  500,  500,  500,  500,  500,  500,
 /*  3540 */   500,  500,  200,  257,  262,  278,  197,  342,   34,  252,
 /*  3550 */   500,  500,  500,  500,  500,  500,  500,  285,  500,  500,
 /*  3560 */   500,  500,  264,  500,  242,  500,  500,   57,  266,  272,
 /*  3570 */   500,  500,  500,  500,  500,  500,  500,  500,  265,  267,
 /*  3580 */   268,  269,  270,  271,  500,  500,   37,   38,   39,   40,
 /*  3590 */    41,   42,   43,   44,   45,   46,   47,  500,  500,  266,
 /*  3600 */   272,  500,  500,  500,  500,  500,  500,  500,  500,  265,
 /*  3610 */   267,  268,  269,  270,  271,  500,  500,  204,  213,  216,
 /*  3620 */   207,  210,   84,   85,   87,   86,   88,  248,  500,  500,
 /*  3630 */   500,  500,  500,  500,  500,  500,  500,  500,  500,  200,
 /*  3640 */   257,  262,  278,  197,  349,  500,  252,  204,  213,  216,
 /*  3650 */   207,  210,  500,  500,  285,  500,  500,  248,  500,  264,
 /*  3660 */   500,  242,  500,   90,   91,  500,  713,  571,  500,  200,
 /*  3670 */   257,  262,  278,  197,   89,  500,  252,  500,   84,   85,
 /*  3680 */    87,   86,   88,  500,  285,  500,  500,  500,  500,  264,
 /*  3690 */   500,  242,  500,  500,  500,  500,  266,  272,  500,  500,
 /*  3700 */   500,  500,  500,  500,  500,  500,  265,  267,  268,  269,
 /*  3710 */   270,  271,  500,  500,  500,  500,  500,  500,  500,   90,
 /*  3720 */    91,  500,  710,  603,  500,  609,  266,  272,  500,  500,
 /*  3730 */    89,  500,  500,  500,  500,  500,  265,  267,  268,  269,
 /*  3740 */   270,  271,  500,  500,  204,  213,  216,  207,  210,  500,
 /*  3750 */   500,  500,  500,  500,  248,  500,  500,  500,  500,  500,
 /*  3760 */   500,  500,  500,  500,  500,  500,  200,  257,  262,  278,
 /*  3770 */   197,  500,  500,  274,  500,  500,  500,  500,  384,  500,
 /*  3780 */   500,  285,  500,  393,  500,  500,  264,  500,  242,  500,
 /*  3790 */   500,  500,  500,  500,  240,  236,  238,  199,  209,  215,
 /*  3800 */   218,  220,  222,  212,  206,  224,  226,  234,  228,  230,
 /*  3810 */   232,  500,  385,  386,  387,  388,  389,  500,  429,  500,
 /*  3820 */   417,  437,  438,  329,  272,  500,  500,  500,  500,  500,
 /*  3830 */   500,  500,  500,  328,  267,  268,  269,  270,  271,  500,
 /*  3840 */   500,  500,  500,  500,  500,  500,  240,  236,  238,  199,
 /*  3850 */   209,  215,  218,  220,  222,  212,  206,  224,  226,  234,
 /*  3860 */   228,  230,  232,  500,   84,   85,   87,   86,   88,  500,
 /*  3870 */   323,  500,  500,  500,  500,  500,  500,  500,  533,  500,
 /*  3880 */   500,  500,  500,  240,  236,  238,  199,  209,  215,  218,
 /*  3890 */   220,  222,  212,  206,  224,  226,  234,  228,  230,  232,
 /*  3900 */   500,  500,  500,  500,  500,   90,   91,  500,  120,  615,
 /*  3910 */   500,  609,  500,  500,  500,  521,   89,  500,  500,  500,
 /*  3920 */   500,  500,  500,  500,  500,  500,  500,  500,  240,  236,
 /*  3930 */   238,  199,  209,  215,  218,  220,  222,  212,  206,  224,
 /*  3940 */   226,  234,  228,  230,  232,  500,  500,  500,  500,  500,
 /*  3950 */   500,  500,  500,  500,  500,  500,  500,  500,  500,  500,
 /*  3960 */   528,  500,  500,  500,  500,  500,  500,  500,  500,  500,
 /*  3970 */   500,  500,  500,  240,  236,  238,  199,  209,  215,  218,
 /*  3980 */   220,  222,  212,  206,  224,  226,  234,  228,  230,  232,
 /*  3990 */   500,  500,  500,  500,  500,  500,  500,  500,  500,  500,
 /*  4000 */   500,  500,  500,  500,  500,  375,  500,  500,  500,  500,
 /*  4010 */   500,  500,  500,  500,  500,  500,  500,  500,  240,  236,
 /*  4020 */   238,  199,  209,  215,  218,  220,  222,  212,  206,  224,
 /*  4030 */   226,  234,  228,  230,  232,  240,  236,  238,  199,  209,
 /*  4040 */   215,  218,  220,  222,  212,  206,  224,  226,  234,  228,
 /*  4050 */   230,  232,  500,  500,  500,  500,  500,  500,  500,  500,
 /*  4060 */   457,  240,  236,  238,  199,  209,  215,  218,  220,  222,
 /*  4070 */   212,  206,  224,  226,  234,  228,  230,  232,  500,  500,
 /*  4080 */   500,  500,  500,  500,  500,  500,  470,  500,  500,  500,
 /*  4090 */   500,  500,  500,  500,  871,  500,  500,  500,  500,  240,
 /*  4100 */   236,  238,  199,  209,  215,  218,  220,  222,  212,  206,
 /*  4110 */   224,  226,  234,  228,  230,  232,  240,  236,  238,  199,
 /*  4120 */   209,  215,  218,  220,  222,  212,  206,  224,  226,  234,
 /*  4130 */   228,  230,  232,  240,  236,  238,  199,  209,  215,  218,
 /*  4140 */   220,  222,  212,  206,  224,  226,  234,  228,  230,  232,
 /*  4150 */   236,  238,  199,  209,  215,  218,  220,  222,  212,  206,
 /*  4160 */   224,  226,  234,  228,  230,  232,   84,   85,   87,   86,
 /*  4170 */    88,  500,  500,  500,  500,  361,  500,  500,  500,  500,
 /*  4180 */   500,  500,  500,  500,  500,  500,  500,  500,  500,  500,
 /*  4190 */   500,  500,  466,  500,   84,   85,   87,   86,   88,   84,
 /*  4200 */    85,   87,   86,   88,  500,  500,  500,   90,   91,  500,
 /*  4210 */   706,  615,  500,  609,  500,  500,  500,  500,   89,  500,
 /*  4220 */    84,   85,   87,   86,   88,   84,   85,   87,   86,   88,
 /*  4230 */   500,  500,  500,  500,  500,   90,   91,  500,   20,   92,
 /*  4240 */    90,   91,  500,  709,  571,  500,   89,  500,  500,  500,
 /*  4250 */   500,   89,  500,   84,   85,   87,   86,   88,  500,  500,
 /*  4260 */   500,   90,   91,  500,  711,  650,   90,   91,  500,  714,
 /*  4270 */   571,  500,   89,  500,  500,  500,  500,   89,  500,   84,
 /*  4280 */    85,   87,   86,   88,   84,   85,   87,   86,   88,  500,
 /*  4290 */   500,  500,  500,  500,   90,   91,  500,  712,  571,   84,
 /*  4300 */    85,   87,   86,   88,  500,   89,  500,  500,  500,  500,
 /*  4310 */   500,  500,  500,  500,  500,  500,  500,  500,  500,  617,
 /*  4320 */    90,   91,  500,  708,  571,   90,   91,  500,  500,  500,
 /*  4330 */   500,   89,  500,  573,  500,  500,   89,  500,  500,  500,
 /*  4340 */    90,   91,   84,   85,   87,   86,   88,  500,  127,  500,
 /*  4350 */   500,   89,  500,  500,  500,  500,  500,  500,  500,  500,
 /*  4360 */   500,  500,  500,  500,  500,  500,  500,  500,  500,  500,
 /*  4370 */   500,  500,  500,  500,  500,  500,  500,  500,  500,  500,
 /*  4380 */   500,  500,  500,   90,   91,   84,   85,   87,   86,   88,
 /*  4390 */   500,  573,  500,  500,   89,  500,  500,  500,  500,  500,
 /*  4400 */   500,  500,  500,  500,  500,  500,  500,  500,  500,  500,
 /*  4410 */   500,  500,  500,  500,  500,  500,  500,  500,  500,  500,
 /*  4420 */   500,  500,  500,  500,  500,  500,   90,   91,  500,  500,
 /*  4430 */   500,  500,  604,  500,  500,  500,  500,   89,  500,   84,
 /*  4440 */    85,   87,   86,   88,   84,   85,   87,   86,   88,  500,
 /*  4450 */   500,  500,  500,  500,  500,  500,  500,  500,  500,  500,
 /*  4460 */   500,  500,  500,  500,  500,  500,  500,  500,  500,  500,
 /*  4470 */   500,  500,  500,  500,  500,  500,  500,  500,  500,  500,
 /*  4480 */    90,   91,  500,  500,  500,   90,   91,  500,   94,  500,
 /*  4490 */   500,   89,  500,   23,  500,  500,   89,  500,   84,   85,
 /*  4500 */    87,   86,   88,  500,  500,  500,  500,  500,  500,  500,
 /*  4510 */   500,  500,  500,  500,  500,  500,  500,  500,  500,  500,
 /*  4520 */   500,  500,  500,  500,  500,  500,  500,  500,  500,  500,
 /*  4530 */   500,  500,  500,  500,  500,  500,  500,  500,  500,   90,
 /*  4540 */    91,  500,  500,  500,  500,  500,  500,  500,  500,  500,
 /*  4550 */    89,
};
static YYCODETYPE yy_lookahead[] = {
 /*     0 */     7,    8,    9,   10,   11,  106,  107,  108,  109,  110,
 /*    10 */    17,  103,  104,  105,  106,  107,  108,  109,  110,  133,
 /*    20 */   134,  135,   29,   30,   31,   32,   33,  114,    6,   36,
 /*    30 */   117,  118,  119,  120,  121,  121,  121,   44,   72,  126,
 /*    40 */   127,  127,   49,  128,   51,  132,  132,  132,   55,   56,
 /*    50 */    57,   58,   59,   60,   61,   62,   63,   64,   65,   37,
 /*    60 */   123,   12,   13,   14,   15,   16,   17,   18,   19,   20,
 /*    70 */    21,   22,   23,   24,   25,   26,   27,   28,   36,   86,
 /*    80 */    87,   30,  162,   34,   51,   52,   44,   36,   37,   96,
 /*    90 */    97,   98,   99,  100,  101,   44,   45,   46,   23,   24,
 /*   100 */    25,   26,   27,   28,   37,   72,   55,   56,   57,   58,
 /*   110 */    59,   60,   61,   62,   63,   36,   65,   66,   67,   68,
 /*   120 */    87,   70,   89,   30,    6,   74,   75,   76,   77,   36,
 /*   130 */    37,   80,  117,   36,  119,  120,  121,   44,   45,   46,
 /*   140 */    88,  126,  127,   92,   93,   94,   95,  132,   55,   56,
 /*   150 */    57,   58,   59,   60,   61,   62,   63,  162,   65,   66,
 /*   160 */    67,   68,   44,   70,  119,   86,  121,   74,   75,   76,
 /*   170 */    77,  126,  127,   80,   52,   30,   36,  132,   51,   52,
 /*   180 */    37,   36,   37,   86,   44,   92,   93,   94,   95,   44,
 /*   190 */    45,   46,   75,   96,   97,   98,   99,  100,  101,   45,
 /*   200 */    55,   56,   57,   58,   59,   60,   61,   62,   63,   87,
 /*   210 */    65,   66,   67,   68,   87,   70,   89,   30,   44,   74,
 /*   220 */    75,   76,   77,   36,   37,   80,   52,  118,  119,  120,
 /*   230 */   121,   44,   45,   46,    6,  126,  127,   92,   93,   94,
 /*   240 */    95,  132,   55,   56,   57,   58,   59,   60,   61,   62,
 /*   250 */    63,   39,   65,   66,   67,   68,   44,   70,  119,  120,
 /*   260 */   121,   74,   75,   76,   77,   37,  127,   80,  113,   30,
 /*   270 */   115,  132,   26,   27,   28,   36,   37,    6,  159,   92,
 /*   280 */    93,   94,   95,   44,   45,   46,  167,  168,  169,  170,
 /*   290 */   131,   37,  133,  134,   55,   56,   57,   58,   59,   60,
 /*   300 */    61,   62,   63,    6,   65,   66,   67,   68,   37,   70,
 /*   310 */   162,   30,  159,   74,   75,   76,   77,   36,   37,   80,
 /*   320 */   167,  168,  169,  170,   45,   44,   45,   46,  175,  176,
 /*   330 */    47,   92,   93,   94,   95,   36,   55,   56,   57,   58,
 /*   340 */    59,   60,   61,   62,   63,   46,   65,   66,   67,   68,
 /*   350 */    71,   70,   73,  171,  172,   74,   75,   76,   77,   51,
 /*   360 */    52,   80,  125,   30,   81,   82,   83,   84,   85,   36,
 /*   370 */    37,    6,  159,   92,   93,   94,   95,   44,   45,   46,
 /*   380 */   167,  168,  169,  170,  131,   88,  133,  134,   55,   56,
 /*   390 */    57,   58,   59,   60,   61,   62,   63,   89,   65,   66,
 /*   400 */    67,   68,   37,   70,   88,   30,  159,   74,   75,   76,
 /*   410 */    77,   36,   37,   80,  167,  168,  169,  170,    6,   44,
 /*   420 */    45,   46,  175,  176,   37,   92,   93,   94,   95,   36,
 /*   430 */    55,   56,   57,   58,   59,   60,   61,   62,   63,   52,
 /*   440 */    65,   66,   67,   68,   36,   70,   34,  121,   37,   74,
 /*   450 */    75,   76,   77,   45,   46,   80,  129,   30,  132,    6,
 /*   460 */   133,  134,  135,   36,   37,  162,  159,   92,   93,   94,
 /*   470 */    95,   44,   45,   46,  167,  168,  169,  170,  131,   86,
 /*   480 */   133,  134,   55,   56,   57,   58,   59,   60,   61,   62,
 /*   490 */    63,   39,   65,   66,   67,   68,   44,   70,   45,   30,
 /*   500 */   159,   74,   75,   76,   77,   36,   37,   80,  167,  168,
 /*   510 */   169,  170,  121,   44,   45,   46,  175,  176,   37,   92,
 /*   520 */    93,   94,   95,  132,   55,   56,   57,   58,   59,   60,
 /*   530 */    61,   62,   63,   52,   65,   66,   67,   68,  131,   70,
 /*   540 */   133,  134,    6,   74,   75,   76,   77,  171,  172,   80,
 /*   550 */   129,   30,  171,  172,  133,  134,  135,   36,   37,   36,
 /*   560 */   159,   92,   93,   94,   95,   44,   45,   46,  167,  168,
 /*   570 */   169,  170,  131,   37,  133,  134,   55,   56,   57,   58,
 /*   580 */    59,   60,   61,   62,   63,  122,   65,   66,   67,   68,
 /*   590 */   113,   70,  115,   30,  159,   74,   75,   76,   77,   36,
 /*   600 */    37,   80,  167,  168,  169,  170,  121,   44,   45,   46,
 /*   610 */   175,  176,    6,   92,   93,   94,   95,  132,   55,   56,
 /*   620 */    57,   58,   59,   60,   61,   62,   63,   37,   65,   66,
 /*   630 */    67,   68,  131,   70,  133,  134,   36,   74,   75,   76,
 /*   640 */    77,   45,   52,   80,  129,   30,  157,  158,  133,  134,
 /*   650 */   135,   36,   37,  162,  159,   92,   93,   94,   95,   44,
 /*   660 */    45,   46,  167,  168,  169,  170,   36,   71,   51,   73,
 /*   670 */    55,   56,   57,   58,   59,   60,   61,   62,   63,   37,
 /*   680 */    65,   66,   67,   68,   78,   70,   44,   30,  159,   74,
 /*   690 */    75,   76,   77,   36,   37,   80,  167,  168,  169,  170,
 /*   700 */    37,   44,   45,   46,  175,  176,   89,   92,   93,   94,
 /*   710 */    95,   36,   55,   56,   57,   58,   59,   60,   61,   62,
 /*   720 */    63,  122,   65,   66,   67,   68,  131,   70,  133,  134,
 /*   730 */    36,   74,   75,   76,   77,  160,  161,   80,  129,   30,
 /*   740 */   171,  172,  133,  134,  135,   36,   37,    6,  159,   92,
 /*   750 */    93,   94,   95,   44,   45,   46,  167,  168,  169,  170,
 /*   760 */   131,   86,  133,  134,   55,   56,   57,   58,   59,   60,
 /*   770 */    61,   62,   63,   45,   65,   66,   67,   68,   37,   70,
 /*   780 */    86,   30,  159,   74,   75,   76,   77,   36,   37,   80,
 /*   790 */   167,  168,  169,  170,   36,   44,   45,   46,  175,  176,
 /*   800 */    37,   92,   93,   94,   95,  162,   55,   56,   57,   58,
 /*   810 */    59,   60,   61,   62,   63,   52,   65,   66,   67,   68,
 /*   820 */    33,   70,   37,   36,  132,   74,   75,   76,   77,   44,
 /*   830 */   123,   80,   47,   30,   41,   51,   39,   44,   41,   36,
 /*   840 */    37,   44,  159,   92,   93,   94,   95,   44,   45,   46,
 /*   850 */   167,  168,  169,  170,   36,  171,  172,    6,   55,   56,
 /*   860 */    57,   58,   59,   60,   61,   62,   63,   49,   65,   66,
 /*   870 */    67,   68,  113,   70,  115,   30,  159,   74,   75,   76,
 /*   880 */    77,   36,   37,   80,  167,  168,  169,  170,   37,   44,
 /*   890 */    45,   46,  175,  176,   45,   92,   93,   94,   95,  123,
 /*   900 */    55,   56,   57,   58,   59,   60,   61,   62,   63,   44,
 /*   910 */    65,   66,   67,   68,  122,   70,   37,   52,  162,   74,
 /*   920 */    75,   76,   77,   44,   37,   80,   47,   30,  124,  125,
 /*   930 */    39,   44,   41,   36,   37,   44,  159,   92,   93,   94,
 /*   940 */    95,   44,   45,   46,  167,  168,  169,  170,   45,  171,
 /*   950 */   172,    6,   55,   56,   57,   58,   59,   60,   61,   62,
 /*   960 */    63,   37,   65,   66,   67,   68,   45,   70,   36,   30,
 /*   970 */   159,   74,   75,   76,   77,   36,   37,   80,  167,  168,
 /*   980 */   169,  170,   37,   44,   45,   46,  175,  176,   52,   92,
 /*   990 */    93,   94,   95,    6,   55,   56,   57,   58,   59,   60,
 /*  1000 */    61,   62,   63,   44,   65,   66,   67,   68,   37,   70,
 /*  1010 */    36,   52,   36,   74,   75,   76,   77,    6,   44,   80,
 /*  1020 */    44,   30,   45,   87,   37,  171,  172,   36,   37,  173,
 /*  1030 */   159,   92,   93,   94,   95,   44,   45,   46,  167,  168,
 /*  1040 */   169,  170,  162,  171,  172,   34,   55,   56,   57,   58,
 /*  1050 */    59,   60,   61,   62,   63,   88,   65,   66,   67,   68,
 /*  1060 */    47,   70,   44,   30,  159,   74,   75,   76,   77,   36,
 /*  1070 */    37,   80,  167,  168,  169,  170,    6,   44,   45,   46,
 /*  1080 */   175,  176,  162,   92,   93,   94,   95,    6,   55,   56,
 /*  1090 */    57,   58,   59,   60,   61,   62,   63,   44,   65,   66,
 /*  1100 */    67,   68,  162,   70,   34,   52,    6,   74,   75,   76,
 /*  1110 */    77,    6,   51,   80,   36,   30,  115,   51,   37,  171,
 /*  1120 */   172,   36,   37,  162,  159,   92,   93,   94,   95,   44,
 /*  1130 */    45,   46,  167,  168,  169,  170,  112,   37,  112,   34,
 /*  1140 */    55,   56,   57,   58,   59,   60,   61,   62,   63,   45,
 /*  1150 */    65,   66,   67,   68,   51,   70,   40,   30,  159,   74,
 /*  1160 */    75,   76,   77,   36,   37,   80,  167,  168,  169,  170,
 /*  1170 */     6,   44,   45,   46,  175,  176,  161,   92,   93,   94,
 /*  1180 */    95,    6,   55,   56,   57,   58,   59,   60,   61,   62,
 /*  1190 */    63,   36,   65,   66,   67,   68,  112,   70,   34,  171,
 /*  1200 */   172,   74,   75,   76,   77,    6,   36,   80,   36,   30,
 /*  1210 */    36,  112,   37,   36,   44,   36,   37,   36,  159,   92,
 /*  1220 */    93,   94,   95,   44,   45,   46,  167,  168,  169,  170,
 /*  1230 */   112,   40,   36,   34,   55,   56,   57,   58,   59,   60,
 /*  1240 */    61,   62,   63,   45,   65,   66,   67,   68,   44,   70,
 /*  1250 */    47,   30,  159,   74,   75,   76,   77,   36,   37,   80,
 /*  1260 */   167,  168,  169,  170,    6,   44,   45,   46,   45,  176,
 /*  1270 */    36,   92,   93,   94,   95,   69,   55,   56,   57,   58,
 /*  1280 */    59,   60,   61,   62,   63,   78,   65,   66,   67,   68,
 /*  1290 */    36,   70,   34,    6,    6,   74,   75,   76,   77,    6,
 /*  1300 */    36,   80,   51,   30,  112,   45,   51,   44,   44,   36,
 /*  1310 */    37,   37,   36,   92,   93,   94,   95,   44,   45,   46,
 /*  1320 */   133,   34,   34,  112,   75,   36,   36,   34,   55,   56,
 /*  1330 */    57,   58,   59,   60,   61,   62,   63,   37,   65,   66,
 /*  1340 */    67,   68,   36,   70,   18,   30,  159,   74,   75,   76,
 /*  1350 */    77,   36,   37,   80,  167,  168,  169,  170,    6,   44,
 /*  1360 */    45,   46,   51,   72,   37,   92,   93,   94,   95,   37,
 /*  1370 */    55,   56,   57,   58,   59,   60,   61,   62,   63,   36,
 /*  1380 */    65,   66,   67,   68,  122,   70,   34,   51,   44,   74,
 /*  1390 */    75,   76,   77,    6,   36,   80,   45,   30,   69,  122,
 /*  1400 */    44,   51,   47,   36,   37,   44,  159,   92,   93,   94,
 /*  1410 */    95,   44,   45,   46,  167,  168,  169,  170,   37,  112,
 /*  1420 */   111,   34,   55,   56,   57,   58,   59,   60,   61,   62,
 /*  1430 */    63,   36,   65,   66,   67,   68,  158,   70,   45,   30,
 /*  1440 */   159,   74,   75,   76,   77,   36,   37,   80,  167,  168,
 /*  1450 */   169,  170,    6,   44,   45,   46,   37,   44,   51,   92,
 /*  1460 */    93,   94,   95,   36,   55,   56,   57,   58,   59,   60,
 /*  1470 */    61,   62,   63,   72,   65,   66,   67,   68,   36,   70,
 /*  1480 */    34,  122,   51,   74,   75,   76,   77,    6,  122,   80,
 /*  1490 */    47,   30,  172,   49,   44,   89,   37,   36,   37,   44,
 /*  1500 */   159,   92,   93,   94,   95,   44,   45,   46,  167,  168,
 /*  1510 */   169,  170,  112,   37,   36,   34,   55,   56,   57,   58,
 /*  1520 */    59,   60,   61,   62,   63,   36,   65,   66,   67,   68,
 /*  1530 */    51,   70,   37,   30,  159,   74,   75,   76,   77,   36,
 /*  1540 */    37,   80,  167,  168,  169,  170,   36,   44,   45,   46,
 /*  1550 */    36,   36,   36,   92,   93,   94,   95,   44,   55,   56,
 /*  1560 */    57,   58,   59,   60,   61,   62,   63,  173,   65,   66,
 /*  1570 */    67,   68,   36,   70,    6,   36,   44,   74,   75,   76,
 /*  1580 */    77,   37,   37,   80,   33,   30,   49,   47,  122,   72,
 /*  1590 */    37,   36,   37,   37,   72,   92,   93,   94,   95,   44,
 /*  1600 */    37,   46,   18,    1,    2,    3,    4,    5,   49,    0,
 /*  1610 */    55,   56,   57,   58,   59,   60,   61,   62,   63,   47,
 /*  1620 */    65,   66,   67,   68,   36,   70,  122,   37,   47,   74,
 /*  1630 */    75,   76,   77,  110,   37,   80,   44,  111,   36,   34,
 /*  1640 */    44,  123,  122,  180,   42,   43,   47,   92,   93,   94,
 /*  1650 */    95,  180,   50,  130,  180,   53,  180,  180,  180,  136,
 /*  1660 */   137,  138,  139,  140,  141,  142,  143,  144,  145,  146,
 /*  1670 */   147,  148,  149,  150,  151,  152,  153,  154,  155,  110,
 /*  1680 */    81,   82,   83,   84,   85,  180,   87,  159,  180,  180,
 /*  1690 */   167,  168,  169,  170,  166,  167,  168,  169,  170,  130,
 /*  1700 */   180,  180,  180,  180,  180,  136,  137,  138,  139,  140,
 /*  1710 */   141,  142,  143,  144,  145,  146,  147,  148,  149,  150,
 /*  1720 */   151,  152,  153,  154,  155,  110,  180,  180,  180,  180,
 /*  1730 */   156,  180,  180,  159,  180,  180,  167,  168,  169,  170,
 /*  1740 */   180,  167,  168,  169,  170,  130,  180,  180,  180,  180,
 /*  1750 */   180,  136,  137,  138,  139,  140,  141,  142,  143,  144,
 /*  1760 */   145,  146,  147,  148,  149,  150,  151,  152,  153,  154,
 /*  1770 */   155,  110,  180,  180,  180,  156,  180,  180,  159,  180,
 /*  1780 */   180,  180,  167,  168,  169,  170,  167,  168,  169,  170,
 /*  1790 */   180,  130,  180,  180,  180,  180,  180,  136,  137,  138,
 /*  1800 */   139,  140,  141,  142,  143,  144,  145,  146,  147,  148,
 /*  1810 */   149,  150,  151,  152,  153,  154,  155,  110,  180,  180,
 /*  1820 */   180,  156,  180,  180,  159,  180,  180,  180,  167,  168,
 /*  1830 */   169,  170,  167,  168,  169,  170,  180,  130,  180,  180,
 /*  1840 */   180,  180,  180,  136,  137,  138,  139,  140,  141,  142,
 /*  1850 */   143,  144,  145,  146,  147,  148,  149,  150,  151,  152,
 /*  1860 */   153,  154,  155,  110,  180,  180,  180,  156,  180,  180,
 /*  1870 */   159,  180,  180,  180,  167,  168,  169,  170,  167,  168,
 /*  1880 */   169,  170,  180,  130,  180,  180,  180,  180,  180,  136,
 /*  1890 */   137,  138,  139,  140,  141,  142,  143,  144,  145,  146,
 /*  1900 */   147,  148,  149,  150,  151,  152,  153,  154,  155,  110,
 /*  1910 */   180,  180,  180,  156,  180,  180,  159,  180,  180,  180,
 /*  1920 */   167,  168,  169,  170,  167,  168,  169,  170,  180,  130,
 /*  1930 */   180,  180,  180,  180,  180,  136,  137,  138,  139,  140,
 /*  1940 */   141,  142,  143,  144,  145,  146,  147,  148,  149,  150,
 /*  1950 */   151,  152,  153,  154,  155,  110,  180,  180,  180,  180,
 /*  1960 */   180,  180,  159,  180,  180,  180,  167,  168,  169,  170,
 /*  1970 */   167,  168,  169,  170,  180,  130,  180,  180,  180,  180,
 /*  1980 */   180,  136,  137,  138,  139,  140,  141,  142,  143,  144,
 /*  1990 */   145,  146,  147,  148,  149,  150,  151,  152,  153,  154,
 /*  2000 */   155,  110,  180,  180,  180,  180,  180,  180,  159,  180,
 /*  2010 */   180,  180,  167,  168,  169,  170,  167,  168,  169,  170,
 /*  2020 */   180,  130,  180,  180,  180,  180,  180,  136,  137,  138,
 /*  2030 */   139,  140,  141,  142,  143,  144,  145,  146,  147,  148,
 /*  2040 */   149,  150,  151,  152,  153,  154,  155,  110,  180,  180,
 /*  2050 */   180,  180,  180,  180,  159,  180,  180,  180,  167,  168,
 /*  2060 */   169,  170,  167,  168,  169,  170,  180,  130,  180,  180,
 /*  2070 */   180,  180,  180,  136,  137,  138,  139,  140,  141,  142,
 /*  2080 */   143,  144,  145,  146,  147,  148,  149,  150,  151,  152,
 /*  2090 */   153,  154,  155,  110,  180,  180,  180,  180,  180,  180,
 /*  2100 */   159,  180,  180,  180,  167,  168,  169,  170,  167,  168,
 /*  2110 */   169,  170,  180,  130,  180,  180,  180,  180,  180,  136,
 /*  2120 */   137,  138,  139,  140,  141,  142,  143,  144,  145,  146,
 /*  2130 */   147,  148,  149,  150,  151,  152,  153,  154,  155,  110,
 /*  2140 */   180,  180,  180,  180,  180,  180,  159,  180,  180,  180,
 /*  2150 */   167,  168,  169,  170,  167,  168,  169,  170,  180,  130,
 /*  2160 */   180,  180,  180,  180,  180,  136,  137,  138,  139,  140,
 /*  2170 */   141,  142,  143,  144,  145,  146,  147,  148,  149,  150,
 /*  2180 */   151,  152,  153,  154,  155,  110,  180,  180,  180,  180,
 /*  2190 */   180,  180,  159,  180,  180,  180,  167,  168,  169,  170,
 /*  2200 */   167,  168,  169,  170,  180,  130,  180,  180,  180,  180,
 /*  2210 */   180,  136,  137,  138,  139,  140,  141,  142,  143,  144,
 /*  2220 */   145,  146,  147,  148,  149,  150,  151,  152,  153,  154,
 /*  2230 */   155,  110,  180,  180,  180,  180,  180,  180,  159,  180,
 /*  2240 */   180,  180,  167,  168,  169,  170,  167,  168,  169,  170,
 /*  2250 */   180,  130,  180,  180,  180,  180,  180,  136,  137,  138,
 /*  2260 */   139,  140,  141,  142,  143,  144,  145,  146,  147,  148,
 /*  2270 */   149,  150,  151,  152,  153,  154,  155,  110,  180,  180,
 /*  2280 */   180,  180,  180,  180,  159,  180,  180,  180,  167,  168,
 /*  2290 */   169,  170,  167,  168,  169,  170,  180,  130,  180,  180,
 /*  2300 */   180,  180,  180,  136,  137,  138,  139,  140,  141,  142,
 /*  2310 */   143,  144,  145,  146,  147,  148,  149,  150,  151,  152,
 /*  2320 */   153,  154,  155,  110,  180,  180,  180,  180,  180,  180,
 /*  2330 */   159,  180,  180,  180,  167,  168,  169,  170,  167,  168,
 /*  2340 */   169,  170,  180,  130,  180,  180,  180,  180,  180,  136,
 /*  2350 */   137,  138,  139,  140,  141,  142,  143,  144,  145,  146,
 /*  2360 */   147,  148,  149,  150,  151,  152,  153,  154,  155,  110,
 /*  2370 */   180,  180,  180,  180,  180,  180,  159,  180,  180,  180,
 /*  2380 */   167,  168,  169,  170,  167,  168,  169,  170,  180,  130,
 /*  2390 */   180,  180,  180,  180,  180,  136,  137,  138,  139,  140,
 /*  2400 */   141,  142,  143,  144,  145,  146,  147,  148,  149,  150,
 /*  2410 */   151,  152,  153,  154,  155,  110,  180,  180,  180,  180,
 /*  2420 */   180,  180,  159,  180,  180,  180,  167,  168,  169,  170,
 /*  2430 */   167,  168,  169,  170,  180,  130,  180,  180,  180,  180,
 /*  2440 */   180,  136,  137,  138,  139,  140,  141,  142,  143,  144,
 /*  2450 */   145,  146,  147,  148,  149,  150,  151,  152,  153,  154,
 /*  2460 */   155,  110,  180,  180,  180,  180,  180,  180,  159,  180,
 /*  2470 */   180,  180,  167,  168,  169,  170,  167,  168,  169,  170,
 /*  2480 */   180,  130,  180,  180,  180,  180,  180,  136,  137,  138,
 /*  2490 */   139,  140,  141,  142,  143,  144,  145,  146,  147,  148,
 /*  2500 */   149,  150,  151,  152,  153,  154,  155,  110,  180,  180,
 /*  2510 */   180,  180,  180,  180,  159,  180,  180,  180,  167,  168,
 /*  2520 */   169,  170,  167,  168,  169,  170,  180,  130,  180,  180,
 /*  2530 */   180,  180,  180,  136,  137,  138,  139,  140,  141,  142,
 /*  2540 */   143,  144,  145,  146,  147,  148,  149,  150,  151,  152,
 /*  2550 */   153,  154,  155,  110,  180,  180,  180,  180,  180,  180,
 /*  2560 */   180,  180,  159,  180,  167,  168,  169,  170,  165,  166,
 /*  2570 */   167,  168,  169,  170,  180,  180,  180,  180,  180,  136,
 /*  2580 */   137,  138,  139,  140,  141,  142,  143,  144,  145,  146,
 /*  2590 */   147,  148,  149,  150,  151,  152,  153,  154,  155,    7,
 /*  2600 */     8,    9,   10,   11,  180,  180,  159,  180,  180,   17,
 /*  2610 */   167,  168,  169,  170,  167,  168,  169,  170,  180,  180,
 /*  2620 */   180,   29,   30,   31,   32,   33,  179,  180,   36,    7,
 /*  2630 */     8,    9,   10,   11,  180,  180,   44,   35,  180,   17,
 /*  2640 */    38,   49,   40,   51,   42,   43,  180,  180,   46,  180,
 /*  2650 */   180,   29,   30,   31,   32,   33,   34,   17,   36,  159,
 /*  2660 */   180,  180,  180,  163,  180,  180,   44,  167,  168,  169,
 /*  2670 */   170,   49,  180,   51,   34,  180,   36,  180,   86,   87,
 /*  2680 */    88,  180,  180,  180,  180,  180,  180,  180,   96,   97,
 /*  2690 */    98,   99,  100,  101,  180,   55,   56,   57,   58,   59,
 /*  2700 */    60,   61,   62,   63,   64,   65,  159,  180,   86,   87,
 /*  2710 */   163,  180,  180,  180,  167,  168,  169,  170,   96,   97,
 /*  2720 */    98,   99,  100,  101,  180,  180,    7,    8,    9,   10,
 /*  2730 */    11,    1,    2,    3,    4,    5,   17,  180,  180,  180,
 /*  2740 */   180,  180,  180,  180,  180,  180,  180,  180,   29,   30,
 /*  2750 */    31,   32,   33,   34,  180,   36,    7,    8,    9,   10,
 /*  2760 */    11,    0,  180,   44,  180,  180,   17,  180,   49,  180,
 /*  2770 */    51,  180,   42,   43,  180,   45,   46,  180,   29,   30,
 /*  2780 */    31,   32,   33,   53,  180,   36,  180,  180,  180,  180,
 /*  2790 */   180,  180,  180,   44,  180,  180,   35,  180,   49,   38,
 /*  2800 */    51,   40,  159,   42,   43,   86,   87,   46,  180,  180,
 /*  2810 */   167,  168,  169,  170,  180,   96,   97,   98,   99,  100,
 /*  2820 */   101,   15,   16,   17,   18,   19,   20,   21,   22,   23,
 /*  2830 */    24,   25,   26,   27,   28,   86,   87,   88,  180,  180,
 /*  2840 */   180,  180,  180,  180,  180,   96,   97,   98,   99,  100,
 /*  2850 */   101,  159,  180,    7,    8,    9,   10,   11,  180,  167,
 /*  2860 */   168,  169,  170,   17,  180,  180,  174,  180,  180,  177,
 /*  2870 */   178,  179,  180,  180,  180,   29,   30,   31,   32,   33,
 /*  2880 */    34,  180,   36,    7,    8,    9,   10,   11,  180,  180,
 /*  2890 */    44,  159,  180,   17,  180,   49,  164,   51,  180,  167,
 /*  2900 */   168,  169,  170,  180,  180,   29,   30,   31,   32,   33,
 /*  2910 */    34,   17,   36,  159,  180,  180,  180,  180,  164,  180,
 /*  2920 */    44,  167,  168,  169,  170,   49,  159,   51,   34,  180,
 /*  2930 */    36,  164,   86,   87,  167,  168,  169,  170,  180,  180,
 /*  2940 */   180,  180,   96,   97,   98,   99,  100,  101,  180,   55,
 /*  2950 */    56,   57,   58,   59,   60,   61,   62,   63,   64,   65,
 /*  2960 */   159,  180,   86,   87,  163,  180,  180,  180,  167,  168,
 /*  2970 */   169,  170,   96,   97,   98,   99,  100,  101,  159,  180,
 /*  2980 */     7,    8,    9,   10,   11,  180,  167,  168,  169,  170,
 /*  2990 */    17,  180,  180,  180,  180,  180,  177,  178,  179,  180,
 /*  3000 */   180,  180,   29,   30,   31,   32,   33,  180,  180,   36,
 /*  3010 */     7,    8,    9,   10,   11,  180,  180,   44,  180,  159,
 /*  3020 */    17,  180,   49,  163,   51,  180,  180,  167,  168,  169,
 /*  3030 */   170,  180,   29,   30,   31,   32,   33,   34,   17,   36,
 /*  3040 */   159,  180,  180,  180,  163,  180,  180,   44,  167,  168,
 /*  3050 */   169,  170,   49,  180,   51,   34,  159,   36,  180,   86,
 /*  3060 */    87,   88,  180,  180,  167,  168,  169,  170,  180,   96,
 /*  3070 */    97,   98,   99,  100,  101,  180,   55,   56,   57,   58,
 /*  3080 */    59,   60,   61,   62,   63,   64,   65,  159,  180,   86,
 /*  3090 */    87,  163,  180,  180,  180,  167,  168,  169,  170,   96,
 /*  3100 */    97,   98,   99,  100,  101,  180,  180,    7,    8,    9,
 /*  3110 */    10,   11,  159,  180,  180,  180,  163,   17,  180,  180,
 /*  3120 */   167,  168,  169,  170,  180,  180,  180,  180,  180,   29,
 /*  3130 */    30,   31,   32,   33,   47,  180,   36,  180,    7,    8,
 /*  3140 */     9,   10,   11,  180,   44,  180,  159,  180,   17,   49,
 /*  3150 */   163,   51,  180,  180,  167,  168,  169,  170,  180,  180,
 /*  3160 */    29,   30,   31,   32,   33,  180,  180,   36,   81,   82,
 /*  3170 */    83,   84,   85,  180,   87,   44,  180,   90,   91,   79,
 /*  3180 */    49,  180,   51,  180,  159,  180,   86,   87,  163,  180,
 /*  3190 */   180,  180,  167,  168,  169,  170,   96,   97,   98,   99,
 /*  3200 */   100,  101,    1,    2,    3,    4,    5,  180,  180,  180,
 /*  3210 */    79,  159,  180,  180,  180,  163,  180,   86,   87,  167,
 /*  3220 */   168,  169,  170,  180,  180,  180,  180,   96,   97,   98,
 /*  3230 */    99,  100,  101,  180,  180,    7,    8,    9,   10,   11,
 /*  3240 */   180,  180,  159,   42,   43,   17,   45,   46,  180,   48,
 /*  3250 */   167,  168,  169,  170,   53,  180,  180,   29,   30,   31,
 /*  3260 */    32,   33,  180,  180,   36,    7,    8,    9,   10,   11,
 /*  3270 */   180,  180,   44,  180,  180,   17,  180,   49,  180,   51,
 /*  3280 */   180,  180,  180,  180,  180,  180,  180,   29,   30,   31,
 /*  3290 */    32,   33,   34,   17,   36,  180,  180,  180,  180,  180,
 /*  3300 */   180,  116,   44,  180,  180,  180,  121,   49,  180,   51,
 /*  3310 */    34,  159,   36,  128,   86,   87,   88,  132,  180,  167,
 /*  3320 */   168,  169,  170,  180,   96,   97,   98,   99,  100,  101,
 /*  3330 */   180,   55,   56,   57,   58,   59,   60,   61,   62,   63,
 /*  3340 */    64,   65,  159,  180,   86,   87,  163,  180,  180,  180,
 /*  3350 */   167,  168,  169,  170,   96,   97,   98,   99,  100,  101,
 /*  3360 */   180,  180,    7,    8,    9,   10,   11,  180,  180,  180,
 /*  3370 */   159,  180,   17,  180,  180,  180,  180,  180,  167,  168,
 /*  3380 */   169,  170,  180,  180,   29,   30,   31,   32,   33,   34,
 /*  3390 */   180,   36,    7,    8,    9,   10,   11,  180,  180,   44,
 /*  3400 */   180,  180,   17,  180,   49,  180,   51,  180,  180,  180,
 /*  3410 */   180,  180,  180,  180,   29,   30,   31,   32,   33,   34,
 /*  3420 */    17,   36,  180,  180,  180,  180,  180,  180,  180,   44,
 /*  3430 */   180,  180,  180,  180,   49,  180,   51,  159,  180,  180,
 /*  3440 */   180,   86,   87,  180,  180,  167,  168,  169,  170,  180,
 /*  3450 */   180,   96,   97,   98,   99,  100,  101,   54,   55,   56,
 /*  3460 */    57,   58,   59,   60,   61,   62,   63,   64,   65,  159,
 /*  3470 */   180,   86,   87,  180,  180,  180,  180,  167,  168,  169,
 /*  3480 */   170,   96,   97,   98,   99,  100,  101,  180,  180,    7,
 /*  3490 */     8,    9,   10,   11,  180,  180,  180,  180,  180,   17,
 /*  3500 */   180,  180,  180,  180,  180,  180,  180,  180,  180,  180,
 /*  3510 */   180,   29,   30,   31,   32,   33,  180,  180,   36,   37,
 /*  3520 */     7,    8,    9,   10,   11,  180,   44,  180,  180,  180,
 /*  3530 */    17,   49,  180,   51,  180,  180,  180,  180,  180,  180,
 /*  3540 */   180,  180,   29,   30,   31,   32,   33,   34,   17,   36,
 /*  3550 */   180,  180,  180,  180,  180,  180,  180,   44,  180,  180,
 /*  3560 */   180,  180,   49,  180,   51,  180,  180,   36,   86,   87,
 /*  3570 */   180,  180,  180,  180,  180,  180,  180,  180,   96,   97,
 /*  3580 */    98,   99,  100,  101,  180,  180,   55,   56,   57,   58,
 /*  3590 */    59,   60,   61,   62,   63,   64,   65,  180,  180,   86,
 /*  3600 */    87,  180,  180,  180,  180,  180,  180,  180,  180,   96,
 /*  3610 */    97,   98,   99,  100,  101,  180,  180,    7,    8,    9,
 /*  3620 */    10,   11,    1,    2,    3,    4,    5,   17,  180,  180,
 /*  3630 */   180,  180,  180,  180,  180,  180,  180,  180,  180,   29,
 /*  3640 */    30,   31,   32,   33,   34,  180,   36,    7,    8,    9,
 /*  3650 */    10,   11,  180,  180,   44,  180,  180,   17,  180,   49,
 /*  3660 */   180,   51,  180,   42,   43,  180,   45,   46,  180,   29,
 /*  3670 */    30,   31,   32,   33,   53,  180,   36,  180,    1,    2,
 /*  3680 */     3,    4,    5,  180,   44,  180,  180,  180,  180,   49,
 /*  3690 */   180,   51,  180,  180,  180,  180,   86,   87,  180,  180,
 /*  3700 */   180,  180,  180,  180,  180,  180,   96,   97,   98,   99,
 /*  3710 */   100,  101,  180,  180,  180,  180,  180,  180,  180,   42,
 /*  3720 */    43,  180,   45,   46,  180,   48,   86,   87,  180,  180,
 /*  3730 */    53,  180,  180,  180,  180,  180,   96,   97,   98,   99,
 /*  3740 */   100,  101,  180,  180,    7,    8,    9,   10,   11,  180,
 /*  3750 */   180,  180,  180,  180,   17,  180,  180,  180,  180,  180,
 /*  3760 */   180,  180,  180,  180,  180,  180,   29,   30,   31,   32,
 /*  3770 */    33,  180,  180,   36,  180,  180,  180,  180,   47,  180,
 /*  3780 */   180,   44,  180,   52,  180,  180,   49,  180,   51,  180,
 /*  3790 */   180,  180,  180,  180,   12,   13,   14,   15,   16,   17,
 /*  3800 */    18,   19,   20,   21,   22,   23,   24,   25,   26,   27,
 /*  3810 */    28,  180,   81,   82,   83,   84,   85,  180,   87,  180,
 /*  3820 */    89,   90,   91,   86,   87,  180,  180,  180,  180,  180,
 /*  3830 */   180,  180,  180,   96,   97,   98,   99,  100,  101,  180,
 /*  3840 */   180,  180,  180,  180,  180,  180,   12,   13,   14,   15,
 /*  3850 */    16,   17,   18,   19,   20,   21,   22,   23,   24,   25,
 /*  3860 */    26,   27,   28,  180,    1,    2,    3,    4,    5,  180,
 /*  3870 */    88,  180,  180,  180,  180,  180,  180,  180,   44,  180,
 /*  3880 */   180,  180,  180,   12,   13,   14,   15,   16,   17,   18,
 /*  3890 */    19,   20,   21,   22,   23,   24,   25,   26,   27,   28,
 /*  3900 */   180,  180,  180,  180,  180,   42,   43,  180,   45,   46,
 /*  3910 */   180,   48,  180,  180,  180,   44,   53,  180,  180,  180,
 /*  3920 */   180,  180,  180,  180,  180,  180,  180,  180,   12,   13,
 /*  3930 */    14,   15,   16,   17,   18,   19,   20,   21,   22,   23,
 /*  3940 */    24,   25,   26,   27,   28,  180,  180,  180,  180,  180,
 /*  3950 */   180,  180,  180,  180,  180,  180,  180,  180,  180,  180,
 /*  3960 */    44,  180,  180,  180,  180,  180,  180,  180,  180,  180,
 /*  3970 */   180,  180,  180,   12,   13,   14,   15,   16,   17,   18,
 /*  3980 */    19,   20,   21,   22,   23,   24,   25,   26,   27,   28,
 /*  3990 */   180,  180,  180,  180,  180,  180,  180,  180,  180,  180,
 /*  4000 */   180,  180,  180,  180,  180,   44,  180,  180,  180,  180,
 /*  4010 */   180,  180,  180,  180,  180,  180,  180,  180,   12,   13,
 /*  4020 */    14,   15,   16,   17,   18,   19,   20,   21,   22,   23,
 /*  4030 */    24,   25,   26,   27,   28,   12,   13,   14,   15,   16,
 /*  4040 */    17,   18,   19,   20,   21,   22,   23,   24,   25,   26,
 /*  4050 */    27,   28,  180,  180,  180,  180,  180,  180,  180,  180,
 /*  4060 */    37,   12,   13,   14,   15,   16,   17,   18,   19,   20,
 /*  4070 */    21,   22,   23,   24,   25,   26,   27,   28,  180,  180,
 /*  4080 */   180,  180,  180,  180,  180,  180,   37,  180,  180,  180,
 /*  4090 */   180,  180,  180,  180,   88,  180,  180,  180,  180,   12,
 /*  4100 */    13,   14,   15,   16,   17,   18,   19,   20,   21,   22,
 /*  4110 */    23,   24,   25,   26,   27,   28,   12,   13,   14,   15,
 /*  4120 */    16,   17,   18,   19,   20,   21,   22,   23,   24,   25,
 /*  4130 */    26,   27,   28,   12,   13,   14,   15,   16,   17,   18,
 /*  4140 */    19,   20,   21,   22,   23,   24,   25,   26,   27,   28,
 /*  4150 */    13,   14,   15,   16,   17,   18,   19,   20,   21,   22,
 /*  4160 */    23,   24,   25,   26,   27,   28,    1,    2,    3,    4,
 /*  4170 */     5,  180,  180,  180,  180,   88,  180,  180,  180,  180,
 /*  4180 */   180,  180,  180,  180,  180,  180,  180,  180,  180,  180,
 /*  4190 */   180,  180,   88,  180,    1,    2,    3,    4,    5,    1,
 /*  4200 */     2,    3,    4,    5,  180,  180,  180,   42,   43,  180,
 /*  4210 */    45,   46,  180,   48,  180,  180,  180,  180,   53,  180,
 /*  4220 */     1,    2,    3,    4,    5,    1,    2,    3,    4,    5,
 /*  4230 */   180,  180,  180,  180,  180,   42,   43,  180,   45,   46,
 /*  4240 */    42,   43,  180,   45,   46,  180,   53,  180,  180,  180,
 /*  4250 */   180,   53,  180,    1,    2,    3,    4,    5,  180,  180,
 /*  4260 */   180,   42,   43,  180,   45,   46,   42,   43,  180,   45,
 /*  4270 */    46,  180,   53,  180,  180,  180,  180,   53,  180,    1,
 /*  4280 */     2,    3,    4,    5,    1,    2,    3,    4,    5,  180,
 /*  4290 */   180,  180,  180,  180,   42,   43,  180,   45,   46,    1,
 /*  4300 */     2,    3,    4,    5,  180,   53,  180,  180,  180,  180,
 /*  4310 */   180,  180,  180,  180,  180,  180,  180,  180,  180,   36,
 /*  4320 */    42,   43,  180,   45,   46,   42,   43,  180,  180,  180,
 /*  4330 */   180,   53,  180,   50,  180,  180,   53,  180,  180,  180,
 /*  4340 */    42,   43,    1,    2,    3,    4,    5,  180,   50,  180,
 /*  4350 */   180,   53,  180,  180,  180,  180,  180,  180,  180,  180,
 /*  4360 */   180,  180,  180,  180,  180,  180,  180,  180,  180,  180,
 /*  4370 */   180,  180,  180,  180,  180,  180,  180,  180,  180,  180,
 /*  4380 */   180,  180,  180,   42,   43,    1,    2,    3,    4,    5,
 /*  4390 */   180,   50,  180,  180,   53,  180,  180,  180,  180,  180,
 /*  4400 */   180,  180,  180,  180,  180,  180,  180,  180,  180,  180,
 /*  4410 */   180,  180,  180,  180,  180,  180,  180,  180,  180,  180,
 /*  4420 */   180,  180,  180,  180,  180,  180,   42,   43,  180,  180,
 /*  4430 */   180,  180,   48,  180,  180,  180,  180,   53,  180,    1,
 /*  4440 */     2,    3,    4,    5,    1,    2,    3,    4,    5,  180,
 /*  4450 */   180,  180,  180,  180,  180,  180,  180,  180,  180,  180,
 /*  4460 */   180,  180,  180,  180,  180,  180,  180,  180,  180,  180,
 /*  4470 */   180,  180,  180,  180,  180,  180,  180,  180,  180,  180,
 /*  4480 */    42,   43,  180,  180,  180,   42,   43,  180,   50,  180,
 /*  4490 */   180,   53,  180,   50,  180,  180,   53,  180,    1,    2,
 /*  4500 */     3,    4,    5,  180,  180,  180,  180,  180,  180,  180,
 /*  4510 */   180,  180,  180,  180,  180,  180,  180,  180,  180,  180,
 /*  4520 */   180,  180,  180,  180,  180,  180,  180,  180,  180,  180,
 /*  4530 */   180,  180,  180,  180,  180,  180,  180,  180,  180,   42,
 /*  4540 */    43,  180,  180,  180,  180,  180,  180,  180,  180,  180,
 /*  4550 */    53,
};
#define YY_SHIFT_USE_DFLT (-35)
static short yy_shift_ofst[] = {
 /*     0 */  2602, 1609, 2761,  -35,  -35,  -35,  -35,  -35, 1478, 1381,
 /*    10 */   -35, 1510,  452,  -35, 1174, 1596,  -35, 2730,  -35, 4193,
 /*    20 */   -35,  -35, 4443,  600, 1066, 2640,  590, 3403, 1597,  -35,
 /*    30 */   -35, 1551,  -35,  -35, 1442, 1326,  -35,  -35,  -35,  -35,
 /*    40 */   -35,  -35,  -35,  -35,  -35,  -35,  -35,  -35,  -35, 1164,
 /*    50 */   481, 3403, 1274,  -35,  -35, 3531,  -35, 1013,   97,  -35,
 /*    60 */   -35,  -35,  -35,  -35,  -35,  -35,  -35, 1406, 1444,  -35,
 /*    70 */   787, 1355,   97,  -35, 1290, 1203,   97,  -35, 1515, 1581,
 /*    80 */    97,  -35,  -35,  -35,  -35,  -35,  -35,  -35,  -35,  -35,
 /*    90 */   -35,  -35, 4497, 4438, 1516, 1479, 3021,  763, 3403, 1459,
 /*   100 */   -35,  -35, 1199,  387, 3403, 1419,  -35,  -35,  -35,  -35,
 /*   110 */  1254,  797,  -35, 1343,  793,  -35, 1078,  118,  -35, 3863,
 /*   120 */   -35,  154,  -35, 4165, 4198,  -35, 4298, 1489, 1103, 2894,
 /*   130 */   174, 1033,  -35, 1221,  -35,  -35,  -35,  -35,  -35,  -35,
 /*   140 */   -35,  -35,  -35,  -35,  -35,  -35,  -35,  -35,  -35,  -35,
 /*   150 */   -35,  -35,  -35,  -35,  -35,  -35,  -35, 1332,  -35, 1300,
 /*   160 */   -35, 3640, 1204, 1179,  657, 1206, 1263,   93, 1315,  -35,
 /*   170 */  3640, 1344,  279,  -35,  596,  -35,  -35,   97, 1401, 1555,
 /*   180 */  1555, 1455, 1273, 1367,  -35, 3640, 1532,  333,  375,  -35,
 /*   190 */  1592,  469,  521,  117, 3640,  411,  -35, 3640,  -35, 3640,
 /*   200 */   932,  -35,  122, 3640, 3640, 4121, 3640, 3640, 4121, 3640,
 /*   210 */  3640, 4121, 3640, 3640, 4121, 3640, 3640, 4121, 3640,   75,
 /*   220 */  3640,   75, 3640,   75, 3640,  246, 3640,  246, 3640,  -35,
 /*   230 */  3640,  -35, 3640,  -35, 3640,  246, 3640, 2806, 3640, 2806,
 /*   240 */  3640, 4137,   -7,   49,  -35, 1605, 3640,  -35, 1588, 1584,
 /*   250 */  3640,   75,  127,  140, 1251, 3513, 4121, 1539, 1568,  932,
 /*   260 */   -35,  -35, 3640,  -35,  -35,  -35,  -35,  -35,  -35,  -35,
 /*   270 */   -35,  -35, 2592, 4121,   33,  818, 1255, 2846, 1264, 1336,
 /*   280 */  2876,  -35, 1258,  -35, 3640, 1276, 1260,  617, 2719, 1288,
 /*   290 */   -35,  -35,  -35,  -35,  -35,  -35, 1170, 1311, 3355, 1446,
 /*   300 */   -35,  -35, 1358, 1351, 1350, 3258, 1070,  -35,  -35,  -35,
 /*   310 */  1395, 1393, 1407, 2622,  -35, 1105,  -35, 1287,  -35,  -35,
 /*   320 */   -35, 3640, 3782,  -35, 3003, 1481,  -35,  -35, 1517, 1522,
 /*   330 */   -35,  297,  -35, 3737,  -35,  -34, 3640,  -35,  -35,  -35,
 /*   340 */  1387,  -35,  -35,  694,  728,  784, 3610,  412,  -35,  -35,
 /*   350 */   977, 1061, 3385, 1011,  -35,  -35,   75,   75,   75,   75,
 /*   360 */  4087,  -35,   42,  -35,  393, 1104,  -35,  921,  -35,   75,
 /*   370 */  4121, 1196,  606, 3131, 3961, 1555, 1461,  -35, 1172,  271,
 /*   380 */   -35, 1172,  -35, 3731,  -35,  -35,  -35,  -35,  -35,  -35,
 /*   390 */  3640,  -35, 4121,  974, 3087, 3640,  -35, 2749,  283, 3640,
 /*   400 */   -35,  967,  283, 3640,  -35, 4006,  -35,  -35,   79,  903,
 /*   410 */   283, 3640,  -35,  849,  283, 3640,  -35,  758, 1599, 3640,
 /*   420 */   -35, 3228,  283, 3640,  -35,  316,  283, 3640,  -35, 2973,
 /*   430 */   283, 3640,  -35,   52,  283, 3640,  -35,  -35,  -35,  -35,
 /*   440 */  3640,  365,  -35, 3640,  -35, 4121,  -35, 1563,  -35, 1553,
 /*   450 */   -35, 1545,  -35, 1544,  -35, 3482, 4023,  -35,  -35, 1536,
 /*   460 */   936, 1514, 1495,  -35, 3640, 4104, 1476,  -35, 3640, 4049,
 /*   470 */   -35, 1306,  851,  -35, 1306,  -35, 1443,   97,  -35,  -35,
 /*   480 */  1306,  228,  -35, 1306, 1175,  -35, 1306, 1081,  -35, 1306,
 /*   490 */   536,  -35, 1306,   22,  -35, 1306,  741,  -35, 1306, 1100,
 /*   500 */   -35, 1306,  987,  -35, 1306,  945,  -35,  308, 1289, 1255,
 /*   510 */   976, 1251,  675, 1223,  784, 1198, 1061,  -35,  -35, 3640,
 /*   520 */  3871, 1555,  897,  -35, 1234, 1207, 3100, 3916, 1555,  239,
 /*   530 */   -35, 3640, 3834, 1555, 1503,  -35, 1249, 3640, 1327,  -35,
 /*   540 */   -35,  -35, 1291, 1555, 1555,  -35,  -35, 1329, 1356,  427,
 /*   550 */   563,  -35,  -35, 3403, 1361,  615,  -35,  709,  -35, 1293,
 /*   560 */  1053,  751,  -35,  803,  -35, 3403, 1413,  845,  -35,  939,
 /*   570 */   -35, 4497, 4341, 1427, 1431, 3276,  865,  991,  -35,   51,
 /*   580 */   -35, 3403, 1450, 1085,  -35, 1127,  -35, 1352,  959, 1409,
 /*   590 */   -35,  145,  -35, 3403, 1513,  187,  -35,  281,  -35, 3677,
 /*   600 */  3621,  -35,  -35, 4384, 1537, 1540,   97, 1556,  -35, 1559,
 /*   610 */  1572,   97, 1590,  -35,  -35, 4384, 4283,  785,  -35,   97,
 /*   620 */   642,  -35,   67,  -35,  408,  143,  453,  254,  299,  -35,
 /*   630 */   -35,  523,  -35,  -35,  663,  -35, 1602,  879,  -35,   97,
 /*   640 */   887,  -35,  924,  -35,  971,  -35,  -35, 3201, 4219, 4224,
 /*   650 */  4497, 4252,  -35, 4278, 1078,  -35,  -35,  -35, 1078,  118,
 /*   660 */   -35, 1116, 1155,  891,  -35, 1177, 1018,  -35, 1078,  118,
 /*   670 */   -35, 1191, 1181,  212,  -35,  630, 1018,  -35,  -35,
};
#define YY_REDUCE_USE_DFLT (-115)
static short yy_reduce_ofst[] = {
 /*     0 */   -92, -115, -101, -115, -115, -115, -115, -115, -115, -115,
 /*    10 */  -115, -115, 1309, -115, -115, 1526, -115, 3185, -115,  -85,
 /*    20 */  -115, -115,  692, -115, -115,  421, -115,  253, -115, -115,
 /*    30 */  -115, -115, -115, -115, -115, -115, -115, -115, -115, -115,
 /*    40 */  -115, -115, -115, -115, -115, -115, -115, -115, -115, -115,
 /*    50 */  -115,  501, -115, -115, -115, -114, -115, -115,  599, -115,
 /*    60 */  -115, -115, -115, -115, -115, -115, -115, -115, -115, -115,
 /*    70 */  -115, -115, 1262, -115, -115, -115, 1277, -115, -115, -115,
 /*    80 */   463, -115, -115, -115, -115, -115, -115, -115, -115, -115,
 /*    90 */  -115, -115,  485,  692, -115, -115,  609, -115,  347, -115,
 /*   100 */  -115, -115, -115, -115,  629, -115, -115, -115, -115, -115,
 /*   110 */  -115, 1192, -115, -115, 1307, -115,  759, 1400, -115,  -87,
 /*   120 */  -115, -115, -115,  109,  -86, -115,  692, -115, -115,  515,
 /*   130 */  -115, 1707, -115, 2443, -115, -115, -115, -115, -115, -115,
 /*   140 */  -115, -115, -115, -115, -115, -115, -115, -115, -115, -115,
 /*   150 */  -115, -115, -115, -115, -115, -115, -115, -115, -115, -115,
 /*   160 */  -115, 1574, -115, 1753, 2443, -115, -115, 2351, 2443, -115,
 /*   170 */  1711, -115,  489, -115, 1278, -115, -115, 1359, -115, 2213,
 /*   180 */  2443, -115, 1891, 2443, -115, 1619, -115, 2305, 2443, -115,
 /*   190 */  -115, 1523, 2443, -115, 1665, -115, -115, 2643, -115, 2897,
 /*   200 */   856, -115, -115, 3083, 3152, -115, 3211, 3278, -115, 3310,
 /*   210 */   119, -115,  213,  307, -115,  401,  589, -115,  683, -115,
 /*   220 */   777, -115,  871, -115,  965, -115, 1059, -115, 1247, -115,
 /*   230 */  1281, -115, 1341, -115, 1375, -115, 1803, -115, 1849, -115,
 /*   240 */  1895, -115, 1187, -115, -115, -115, 1941, -115, -115, -115,
 /*   250 */  1987, -115, -115, -115, -115,  623, -115, -115, -115, 1394,
 /*   260 */  -115, -115, 2125, -115, -115, -115, -115, -115, -115, -115,
 /*   270 */  -115, -115, 2692, -115, -115, -115, -115,  435, -115, -115,
 /*   280 */   999, -115, -115, -115, 1093, -115, -115, -115,  341, -115,
 /*   290 */  -115, -115, -115, -115, -115, -115, -115, -115,  153, -115,
 /*   300 */  -115, -115, -115, -115, -115,  717, -115, -115, -115, -115,
 /*   310 */  -115, -115, -115,  811, -115, -115, -115, -115, -115, -115,
 /*   320 */  -115, 2355, -115, -115,  905, -115, -115, -115, -115, -115,
 /*   330 */  -115, -115, -115, 2819, -115, -115, 2447, -115, -115, -115,
 /*   340 */  -115, -115, -115, -115, -115, -115,  247, -115, -115, -115,
 /*   350 */  -115, -115,  529, -115, -115, -115, -115, -115, -115, -115,
 /*   360 */  -115, -115, -115, -115, -115, -115, -115, -115, -115, -115,
 /*   370 */  -115, -115, -115,  495, -115, 1569, 2443, -115,  575, -115,
 /*   380 */  -115, 1015, -115,  961, -115, -115, -115, -115, -115, -115,
 /*   390 */  2953, -115, -115, -115,  940, 2928, -115, 2767,  920, 2881,
 /*   400 */  -115, -115,  880, 2801, -115, -115, -115, -115, -115, -115,
 /*   410 */   756, 3025, -115, -115,  643, 3183, -115, -115,  491, 2547,
 /*   420 */  -115, 2732,  303, 2860, -115, -115,  148, 2987, -115, 2754,
 /*   430 */    -5, 2500, -115, -115,  -80, 3052, -115, -115, -115, -115,
 /*   440 */  2403, -115, -115, 1528, -115, -115, -115, -115, -115, -115,
 /*   450 */  -115, -115, -115, -115, -115, 2033, -115, -115, -115, -115,
 /*   460 */  -115, -115, -115, -115, 2079, -115, -115, -115, 2171, -115,
 /*   470 */  -115,  778, -115, -115, 1320, -115, -115, 1366, -115, -115,
 /*   480 */   182, -115, -115, 1028, -115, -115,  948, -115, -115,  569,
 /*   490 */  -115, -115,  376, -115, -115,  684, -115, -115,  381, -115,
 /*   500 */  -115,  854, -115, -115,  872, -115, -115, -115, -115, -115,
 /*   510 */  -115, -115, -115, -115, -115, -115, -115, -115, -115, 2217,
 /*   520 */  -115, 2121, 2443, -115, -115, -115, 2263, -115, 1615, 2443,
 /*   530 */  -115, 2309, -115, 1983, 2443, -115, -115, 1757, -115, -115,
 /*   540 */  -115, -115, -115, 2259, 2443, -115, -115, -115, -115, 2397,
 /*   550 */  2443, -115, -115,  595, -115, 1661, -115, 2443, -115, -115,
 /*   560 */  -115, 1845, -115, 2443, -115,  441, -115, 1937, -115, 2443,
 /*   570 */  -115,  326,  692, -115, -115,  327, -115, 2075, -115, 2443,
 /*   580 */  -115,  407, -115, 1799, -115, 2443, -115, -115, -115, 2029,
 /*   590 */  -115, 2443, -115,  159, -115, 2167, -115, 2443, -115,   45,
 /*   600 */   -86, -115, -115,  326, -115, -115, 1466, -115, -115, -115,
 /*   610 */  -115, 1504, -115, -115, -115,  391,  692, 1518, -115, 1520,
 /*   620 */   -63, -115, -115, -115,  804, -115, -115, -115,  237, -115,
 /*   630 */  -115, -115, -115, -115, -115, -115,  692,  707, -115,  792,
 /*   640 */   776, -115, -115, -115, -115, -115, -115,   15,  139,  -86,
 /*   650 */   391,  -86, -115,  -86, 1001, -115, -115, -115,  155, 1026,
 /*   660 */  -115, -115, -115, 1084, -115, -115, 1099, -115,  477, 1118,
 /*   670 */  -115, -115, -115, 1211, -115, -115, 1024, -115, -115,
};
static YYACTIONTYPE yy_default[] = {
 /*     0 */   997,  997,  997,  681,  683,  684,  685,  686,  997,  997,
 /*    10 */   687,  997,  997,  688,  997,  997,  689,  997,  704,  997,
 /*    20 */   705,  735,  997,  997,  997,  997,  997,  997,  997,  755,
 /*    30 */   773,  774,  775,  776,  997,  997,  787,  788,  789,  790,
 /*    40 */   791,  792,  793,  794,  795,  796,  797,  798,  759,  997,
 /*    50 */   997,  997,  997,  756,  760,  997,  777,  779,  997,  783,
 /*    60 */   983,  984,  985,  986,  987,  988,  989,  997,  997,  990,
 /*    70 */   997,  780,  997,  784,  997,  781,  997,  785,  997,  782,
 /*    80 */   997,  786,  778,  763,  765,  766,  767,  768,  769,  770,
 /*    90 */   771,  772,  997,  997,  997,  997,  997,  997,  997,  997,
 /*   100 */   757,  761,  997,  997,  997,  997,  758,  762,  764,  736,
 /*   110 */   997,  997,  690,  997,  997,  691,  997,  997,  693,  997,
 /*   120 */   699,  997,  700,  997,  997,  733,  997,  997,  997,  997,
 /*   130 */   997,  997,  739,  997,  741,  799,  801,  802,  803,  804,
 /*   140 */   805,  806,  807,  808,  809,  810,  811,  812,  813,  814,
 /*   150 */   815,  816,  817,  818,  819,  820,  821,  997,  822,  997,
 /*   160 */   823,  997,  997,  997,  997,  826,  997,  997,  997,  827,
 /*   170 */   997,  997,  997,  830,  997,  831,  832,  997,  997,  834,
 /*   180 */   835,  997,  997,  997,  838,  997,  997,  997,  997,  840,
 /*   190 */   997,  997,  997,  997,  997,  997,  842,  997,  900,  997,
 /*   200 */   997,  901,  997,  997,  997,  902,  997,  997,  903,  997,
 /*   210 */   997,  904,  997,  997,  905,  997,  997,  906,  997,  912,
 /*   220 */   997,  913,  997,  914,  997,  924,  997,  925,  997,  926,
 /*   230 */   997,  927,  997,  928,  997,  929,  997,  930,  997,  931,
 /*   240 */   997,  932,  997,  997,  915,  997,  997,  916,  997,  997,
 /*   250 */   997,  917,  937,  997,  918,  997,  974,  997,  997,  997,
 /*   260 */   933,  934,  997,  935,  936,  938,  939,  940,  941,  942,
 /*   270 */   943,  944,  997,  982,  937,  997,  921,  997,  997,  947,
 /*   280 */   997,  948,  997,  949,  997,  997,  997,  997,  997,  997,
 /*   290 */   955,  956,  969,  970,  971,  973,  997,  997,  997,  997,
 /*   300 */   959,  960,  997,  997,  997,  997,  997,  961,  962,  972,
 /*   310 */   997,  997,  950,  997,  951,  997,  952,  997,  957,  958,
 /*   320 */   922,  997,  997,  923,  997,  997,  953,  954,  938,  939,
 /*   330 */   945,  997,  946,  997,  975,  997,  997,  977,  978,  976,
 /*   340 */   997,  963,  964,  997,  997,  919,  997,  997,  965,  966,
 /*   350 */   997,  920,  997,  997,  967,  968,  911,  910,  909,  908,
 /*   360 */   997,  991,  997,  992,  997,  997,  993,  997,  994,  907,
 /*   370 */   995,  997,  997,  997,  997,  997,  997,  843,  997,  997,
 /*   380 */   847,  997,  848,  997,  850,  851,  852,  853,  854,  855,
 /*   390 */   997,  856,  899,  997,  997,  997,  857,  997,  997,  997,
 /*   400 */   860,  997,  997,  997,  861,  997,  867,  868,  997,  997,
 /*   410 */   997,  997,  858,  997,  997,  997,  859,  997,  997,  997,
 /*   420 */   862,  997,  997,  997,  863,  997,  997,  997,  864,  997,
 /*   430 */   997,  997,  865,  997,  997,  997,  866,  869,  870,  849,
 /*   440 */   997,  997,  872,  997,  873,  875,  874,  997,  876,  997,
 /*   450 */   877,  997,  878,  997,  879,  997,  997,  880,  881,  997,
 /*   460 */   997,  997,  997,  882,  997,  997,  997,  883,  997,  997,
 /*   470 */   884,  997,  997,  885,  997,  895,  897,  997,  898,  896,
 /*   480 */   997,  997,  886,  997,  997,  887,  997,  997,  888,  997,
 /*   490 */   997,  889,  997,  997,  890,  997,  997,  891,  997,  997,
 /*   500 */   892,  997,  997,  893,  997,  997,  894,  997,  997,  997,
 /*   510 */   997,  997,  997,  997,  997,  997,  997,  996,  800,  997,
 /*   520 */   997,  997,  997,  844,  997,  997,  997,  997,  997,  997,
 /*   530 */   845,  997,  997,  997,  997,  846,  997,  997,  997,  841,
 /*   540 */   839,  837,  997,  997,  836,  833,  828,  824,  997,  997,
 /*   550 */   997,  829,  825,  997,  997,  997,  747,  997,  749,  997,
 /*   560 */   997,  997,  740,  997,  742,  997,  997,  997,  748,  997,
 /*   570 */   750,  997,  997,  997,  997,  997,  997,  997,  743,  997,
 /*   580 */   745,  997,  997,  997,  751,  997,  753,  997,  997,  997,
 /*   590 */   744,  997,  746,  997,  997,  997,  752,  997,  754,  997,
 /*   600 */   997,  731,  734,  997,  997,  997,  997,  997,  737,  997,
 /*   610 */   997,  997,  997,  738,  715,  997,  997,  997,  717,  997,
 /*   620 */   997,  719,  997,  723,  997,  997,  997,  997,  997,  727,
 /*   630 */   729,  997,  730,  728,  997,  721,  997,  997,  718,  997,
 /*   640 */   997,  720,  997,  724,  997,  722,  732,  997,  997,  997,
 /*   650 */   997,  997,  716,  997,  997,  701,  703,  702,  997,  997,
 /*   660 */   692,  997,  997,  997,  694,  997,  997,  695,  997,  997,
 /*   670 */   696,  997,  997,  997,  697,  997,  997,  698,  682,
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
  "INSTANCEOF",    "AND",           "OR",            "EQUALS",      
  "IDENTICAL",     "LESS",          "GREATER",       "LESSEQUAL",   
  "GREATEREQUAL",  "NOTIDENTICAL",  "NOTEQUALS",     "ADD",         
  "SUB",           "CONCAT",        "MUL",           "DIV",         
  "MOD",           "ISSET",         "FETCH",         "TYPEOF",      
  "NEW",           "NOT",           "PARENTHESES_CLOSE",  "NAMESPACE",   
  "IDENTIFIER",    "DOTCOMMA",      "INTERFACE",     "EXTENDS",     
  "CLASS",         "IMPLEMENTS",    "ABSTRACT",      "FINAL",       
  "BRACKET_OPEN",  "BRACKET_CLOSE",  "COMMENT",       "ASSIGN",      
  "CONST",         "CONSTANT",      "FUNCTION",      "PARENTHESES_OPEN",
  "ARROW",         "INLINE",        "VOID",          "TYPE_INTEGER",
  "TYPE_UINTEGER",  "TYPE_LONG",     "TYPE_ULONG",    "TYPE_CHAR",   
  "TYPE_UCHAR",    "TYPE_DOUBLE",   "TYPE_BOOL",     "TYPE_STRING", 
  "TYPE_ARRAY",    "TYPE_VAR",      "BREAK",         "CONTINUE",    
  "IF",            "ELSE",          "SWITCH",        "CASE",        
  "COLON",         "DEFAULT",       "LOOP",          "WHILE",       
  "DO",            "FOR",           "IN",            "REVERSE",     
  "LET",           "ADDASSIGN",     "SUBASSIGN",     "MULASSIGN",   
  "DIVASSIGN",     "CONCATASSIGN",  "STRING",        "SBRACKET_OPEN",
  "SBRACKET_CLOSE",  "DOUBLECOLON",   "INCR",          "DECR",        
  "ECHO",          "RETURN",        "UNSET",         "THROW",       
  "INTEGER",       "CHAR",          "DOUBLE",        "NULL",        
  "TRUE",          "FALSE",         "error",         "program",     
  "xx_language",   "xx_top_statement_list",  "xx_top_statement",  "xx_namespace_def",
  "xx_class_def",  "xx_interface_def",  "xx_comment",    "xx_interface_body",
  "xx_class_body",  "xx_implements_list",  "xx_class_definition",  "xx_implements",
  "xx_interface_methods_definition",  "xx_class_properties_definition",  "xx_class_consts_definition",  "xx_class_methods_definition",
  "xx_class_property_definition",  "xx_visibility_list",  "xx_literal_expr",  "xx_class_property_shortcuts",
  "xx_class_property_shortcuts_list",  "xx_class_property_shortcut",  "xx_class_const_definition",  "xx_class_method_definition",
  "xx_interface_method_definition",  "xx_parameter_list",  "xx_statement_list",  "xx_method_return_type",
  "xx_visibility",  "xx_parameter_type",  "xx_parameter_cast",  "xx_parameter",
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
 /*  95 */ "xx_method_return_type ::= xx_parameter_type",
 /*  96 */ "xx_method_return_type ::= xx_parameter_type NOT",
 /*  97 */ "xx_method_return_type ::= xx_parameter_cast",
 /*  98 */ "xx_parameter_list ::= xx_parameter_list COMMA xx_parameter",
 /*  99 */ "xx_parameter_list ::= xx_parameter",
 /* 100 */ "xx_parameter ::= IDENTIFIER",
 /* 101 */ "xx_parameter ::= xx_parameter_type IDENTIFIER",
 /* 102 */ "xx_parameter ::= xx_parameter_type NOT IDENTIFIER",
 /* 103 */ "xx_parameter ::= xx_parameter_cast IDENTIFIER",
 /* 104 */ "xx_parameter ::= IDENTIFIER ASSIGN xx_literal_expr",
 /* 105 */ "xx_parameter ::= xx_parameter_type IDENTIFIER ASSIGN xx_literal_expr",
 /* 106 */ "xx_parameter ::= xx_parameter_type NOT IDENTIFIER ASSIGN xx_literal_expr",
 /* 107 */ "xx_parameter ::= xx_parameter_cast IDENTIFIER ASSIGN xx_literal_expr",
 /* 108 */ "xx_parameter_cast ::= LESS IDENTIFIER GREATER",
 /* 109 */ "xx_parameter_type ::= TYPE_INTEGER",
 /* 110 */ "xx_parameter_type ::= TYPE_UINTEGER",
 /* 111 */ "xx_parameter_type ::= TYPE_LONG",
 /* 112 */ "xx_parameter_type ::= TYPE_ULONG",
 /* 113 */ "xx_parameter_type ::= TYPE_CHAR",
 /* 114 */ "xx_parameter_type ::= TYPE_UCHAR",
 /* 115 */ "xx_parameter_type ::= TYPE_DOUBLE",
 /* 116 */ "xx_parameter_type ::= TYPE_BOOL",
 /* 117 */ "xx_parameter_type ::= TYPE_STRING",
 /* 118 */ "xx_parameter_type ::= TYPE_ARRAY",
 /* 119 */ "xx_parameter_type ::= TYPE_VAR",
 /* 120 */ "xx_statement_list ::= xx_statement_list xx_statement",
 /* 121 */ "xx_statement_list ::= xx_statement",
 /* 122 */ "xx_statement ::= xx_let_statement",
 /* 123 */ "xx_statement ::= xx_if_statement",
 /* 124 */ "xx_statement ::= xx_loop_statement",
 /* 125 */ "xx_statement ::= xx_echo_statement",
 /* 126 */ "xx_statement ::= xx_return_statement",
 /* 127 */ "xx_statement ::= xx_fetch_statement",
 /* 128 */ "xx_statement ::= xx_fcall_statement",
 /* 129 */ "xx_statement ::= xx_mcall_statement",
 /* 130 */ "xx_statement ::= xx_scall_statement",
 /* 131 */ "xx_statement ::= xx_unset_statement",
 /* 132 */ "xx_statement ::= xx_throw_statement",
 /* 133 */ "xx_statement ::= xx_declare_statement",
 /* 134 */ "xx_statement ::= xx_break_statement",
 /* 135 */ "xx_statement ::= xx_continue_statement",
 /* 136 */ "xx_statement ::= xx_while_statement",
 /* 137 */ "xx_statement ::= xx_do_while_statement",
 /* 138 */ "xx_statement ::= xx_switch_statement",
 /* 139 */ "xx_statement ::= xx_for_statement",
 /* 140 */ "xx_statement ::= xx_comment",
 /* 141 */ "xx_statement ::= xx_empty_statement",
 /* 142 */ "xx_empty_statement ::= DOTCOMMA",
 /* 143 */ "xx_break_statement ::= BREAK DOTCOMMA",
 /* 144 */ "xx_continue_statement ::= CONTINUE DOTCOMMA",
 /* 145 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN BRACKET_CLOSE",
 /* 146 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN BRACKET_CLOSE ELSE BRACKET_OPEN BRACKET_CLOSE",
 /* 147 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 148 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE ELSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 149 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE ELSE BRACKET_OPEN BRACKET_CLOSE",
 /* 150 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN BRACKET_CLOSE ELSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 151 */ "xx_switch_statement ::= SWITCH xx_eval_expr BRACKET_OPEN BRACKET_CLOSE",
 /* 152 */ "xx_switch_statement ::= SWITCH xx_eval_expr BRACKET_OPEN xx_case_clauses BRACKET_CLOSE",
 /* 153 */ "xx_case_clauses ::= xx_case_clauses xx_case_clause",
 /* 154 */ "xx_case_clauses ::= xx_case_clause",
 /* 155 */ "xx_case_clause ::= CASE xx_literal_expr COLON",
 /* 156 */ "xx_case_clause ::= CASE xx_literal_expr COLON xx_statement_list",
 /* 157 */ "xx_case_clause ::= DEFAULT COLON xx_statement_list",
 /* 158 */ "xx_loop_statement ::= LOOP BRACKET_OPEN BRACKET_CLOSE",
 /* 159 */ "xx_loop_statement ::= LOOP BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 160 */ "xx_while_statement ::= WHILE xx_eval_expr BRACKET_OPEN BRACKET_CLOSE",
 /* 161 */ "xx_while_statement ::= WHILE xx_eval_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 162 */ "xx_do_while_statement ::= DO BRACKET_OPEN BRACKET_CLOSE WHILE xx_eval_expr DOTCOMMA",
 /* 163 */ "xx_do_while_statement ::= DO BRACKET_OPEN xx_statement_list BRACKET_CLOSE WHILE xx_eval_expr DOTCOMMA",
 /* 164 */ "xx_for_statement ::= FOR IDENTIFIER IN xx_common_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 165 */ "xx_for_statement ::= FOR IDENTIFIER IN REVERSE xx_common_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 166 */ "xx_for_statement ::= FOR IDENTIFIER COMMA IDENTIFIER IN xx_common_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 167 */ "xx_for_statement ::= FOR IDENTIFIER COMMA IDENTIFIER IN REVERSE xx_common_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 168 */ "xx_let_statement ::= LET xx_let_assignments DOTCOMMA",
 /* 169 */ "xx_let_assignments ::= xx_let_assignments COMMA xx_let_assignment",
 /* 170 */ "xx_let_assignments ::= xx_let_assignment",
 /* 171 */ "xx_assignment_operator ::= ASSIGN",
 /* 172 */ "xx_assignment_operator ::= ADDASSIGN",
 /* 173 */ "xx_assignment_operator ::= SUBASSIGN",
 /* 174 */ "xx_assignment_operator ::= MULASSIGN",
 /* 175 */ "xx_assignment_operator ::= DIVASSIGN",
 /* 176 */ "xx_assignment_operator ::= CONCATASSIGN",
 /* 177 */ "xx_let_assignment ::= IDENTIFIER xx_assignment_operator xx_assign_expr",
 /* 178 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER xx_assignment_operator xx_assign_expr",
 /* 179 */ "xx_let_assignment ::= IDENTIFIER ARROW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 180 */ "xx_let_assignment ::= IDENTIFIER ARROW BRACKET_OPEN STRING BRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 181 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER SBRACKET_OPEN SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 182 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER SBRACKET_OPEN xx_index_expr SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 183 */ "xx_let_assignment ::= IDENTIFIER DOUBLECOLON IDENTIFIER xx_assignment_operator xx_assign_expr",
 /* 184 */ "xx_let_assignment ::= IDENTIFIER DOUBLECOLON IDENTIFIER SBRACKET_OPEN SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 185 */ "xx_let_assignment ::= IDENTIFIER DOUBLECOLON IDENTIFIER SBRACKET_OPEN xx_index_expr SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 186 */ "xx_let_assignment ::= IDENTIFIER SBRACKET_OPEN SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 187 */ "xx_let_assignment ::= IDENTIFIER SBRACKET_OPEN xx_index_expr SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 188 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER INCR",
 /* 189 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER DECR",
 /* 190 */ "xx_let_assignment ::= IDENTIFIER INCR",
 /* 191 */ "xx_let_assignment ::= IDENTIFIER DECR",
 /* 192 */ "xx_index_expr ::= xx_common_expr",
 /* 193 */ "xx_echo_statement ::= ECHO xx_echo_expressions DOTCOMMA",
 /* 194 */ "xx_echo_expressions ::= xx_echo_expressions COMMA xx_echo_expression",
 /* 195 */ "xx_echo_expressions ::= xx_echo_expression",
 /* 196 */ "xx_echo_expression ::= xx_common_expr",
 /* 197 */ "xx_mcall_statement ::= xx_mcall_expr DOTCOMMA",
 /* 198 */ "xx_fcall_statement ::= xx_fcall_expr DOTCOMMA",
 /* 199 */ "xx_scall_statement ::= xx_scall_expr DOTCOMMA",
 /* 200 */ "xx_fetch_statement ::= xx_fetch_expr DOTCOMMA",
 /* 201 */ "xx_return_statement ::= RETURN xx_common_expr DOTCOMMA",
 /* 202 */ "xx_return_statement ::= RETURN DOTCOMMA",
 /* 203 */ "xx_unset_statement ::= UNSET IDENTIFIER ARROW IDENTIFIER DOTCOMMA",
 /* 204 */ "xx_unset_statement ::= UNSET IDENTIFIER SBRACKET_OPEN xx_common_expr SBRACKET_CLOSE DOTCOMMA",
 /* 205 */ "xx_throw_statement ::= THROW xx_common_expr DOTCOMMA",
 /* 206 */ "xx_declare_statement ::= TYPE_INTEGER xx_declare_variable_list DOTCOMMA",
 /* 207 */ "xx_declare_statement ::= TYPE_UINTEGER xx_declare_variable_list DOTCOMMA",
 /* 208 */ "xx_declare_statement ::= TYPE_CHAR xx_declare_variable_list DOTCOMMA",
 /* 209 */ "xx_declare_statement ::= TYPE_UCHAR xx_declare_variable_list DOTCOMMA",
 /* 210 */ "xx_declare_statement ::= TYPE_LONG xx_declare_variable_list DOTCOMMA",
 /* 211 */ "xx_declare_statement ::= TYPE_ULONG xx_declare_variable_list DOTCOMMA",
 /* 212 */ "xx_declare_statement ::= TYPE_DOUBLE xx_declare_variable_list DOTCOMMA",
 /* 213 */ "xx_declare_statement ::= TYPE_STRING xx_declare_variable_list DOTCOMMA",
 /* 214 */ "xx_declare_statement ::= TYPE_BOOL xx_declare_variable_list DOTCOMMA",
 /* 215 */ "xx_declare_statement ::= TYPE_VAR xx_declare_variable_list DOTCOMMA",
 /* 216 */ "xx_declare_variable_list ::= xx_declare_variable_list COMMA xx_declare_variable",
 /* 217 */ "xx_declare_variable_list ::= xx_declare_variable",
 /* 218 */ "xx_declare_variable ::= IDENTIFIER",
 /* 219 */ "xx_declare_variable ::= IDENTIFIER ASSIGN xx_literal_expr",
 /* 220 */ "xx_assign_expr ::= xx_common_expr",
 /* 221 */ "xx_common_expr ::= NOT xx_common_expr",
 /* 222 */ "xx_common_expr ::= ISSET xx_isset_expr",
 /* 223 */ "xx_common_expr ::= REQUIRE xx_common_expr",
 /* 224 */ "xx_common_expr ::= CLONE xx_common_expr",
 /* 225 */ "xx_common_expr ::= EMPTY xx_common_expr",
 /* 226 */ "xx_common_expr ::= LIKELY xx_common_expr",
 /* 227 */ "xx_common_expr ::= UNLIKELY xx_common_expr",
 /* 228 */ "xx_common_expr ::= xx_common_expr EQUALS xx_common_expr",
 /* 229 */ "xx_common_expr ::= xx_common_expr NOTEQUALS xx_common_expr",
 /* 230 */ "xx_common_expr ::= xx_common_expr IDENTICAL xx_common_expr",
 /* 231 */ "xx_common_expr ::= xx_common_expr NOTIDENTICAL xx_common_expr",
 /* 232 */ "xx_common_expr ::= xx_common_expr LESS xx_common_expr",
 /* 233 */ "xx_common_expr ::= xx_common_expr GREATER xx_common_expr",
 /* 234 */ "xx_common_expr ::= xx_common_expr LESSEQUAL xx_common_expr",
 /* 235 */ "xx_common_expr ::= xx_common_expr GREATEREQUAL xx_common_expr",
 /* 236 */ "xx_common_expr ::= PARENTHESES_OPEN xx_common_expr PARENTHESES_CLOSE",
 /* 237 */ "xx_common_expr ::= PARENTHESES_OPEN xx_parameter_type PARENTHESES_CLOSE xx_common_expr",
 /* 238 */ "xx_common_expr ::= LESS IDENTIFIER GREATER xx_common_expr",
 /* 239 */ "xx_common_expr ::= IDENTIFIER ARROW IDENTIFIER",
 /* 240 */ "xx_common_expr ::= IDENTIFIER ARROW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE",
 /* 241 */ "xx_common_expr ::= IDENTIFIER ARROW BRACKET_OPEN STRING BRACKET_CLOSE",
 /* 242 */ "xx_common_expr ::= IDENTIFIER DOUBLECOLON IDENTIFIER",
 /* 243 */ "xx_common_expr ::= IDENTIFIER DOUBLECOLON CONSTANT",
 /* 244 */ "xx_common_expr ::= IDENTIFIER SBRACKET_OPEN xx_common_expr SBRACKET_CLOSE",
 /* 245 */ "xx_common_expr ::= xx_common_expr ADD xx_common_expr",
 /* 246 */ "xx_common_expr ::= xx_common_expr SUB xx_common_expr",
 /* 247 */ "xx_common_expr ::= xx_common_expr MUL xx_common_expr",
 /* 248 */ "xx_common_expr ::= xx_common_expr DIV xx_common_expr",
 /* 249 */ "xx_common_expr ::= xx_common_expr MOD xx_common_expr",
 /* 250 */ "xx_common_expr ::= xx_common_expr CONCAT xx_common_expr",
 /* 251 */ "xx_common_expr ::= xx_common_expr AND xx_common_expr",
 /* 252 */ "xx_common_expr ::= xx_common_expr OR xx_common_expr",
 /* 253 */ "xx_common_expr ::= xx_common_expr INSTANCEOF xx_common_expr",
 /* 254 */ "xx_fetch_expr ::= FETCH IDENTIFIER COMMA xx_isset_expr",
 /* 255 */ "xx_common_expr ::= xx_fetch_expr",
 /* 256 */ "xx_common_expr ::= TYPEOF xx_common_expr",
 /* 257 */ "xx_common_expr ::= CONSTANT",
 /* 258 */ "xx_common_expr ::= IDENTIFIER",
 /* 259 */ "xx_common_expr ::= INTEGER",
 /* 260 */ "xx_common_expr ::= STRING",
 /* 261 */ "xx_common_expr ::= CHAR",
 /* 262 */ "xx_common_expr ::= DOUBLE",
 /* 263 */ "xx_common_expr ::= NULL",
 /* 264 */ "xx_common_expr ::= TRUE",
 /* 265 */ "xx_common_expr ::= FALSE",
 /* 266 */ "xx_common_expr ::= SBRACKET_OPEN SBRACKET_CLOSE",
 /* 267 */ "xx_common_expr ::= SBRACKET_OPEN xx_array_list SBRACKET_CLOSE",
 /* 268 */ "xx_common_expr ::= NEW IDENTIFIER",
 /* 269 */ "xx_common_expr ::= NEW IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 270 */ "xx_common_expr ::= NEW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 271 */ "xx_common_expr ::= NEW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE",
 /* 272 */ "xx_common_expr ::= NEW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 273 */ "xx_common_expr ::= NEW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 274 */ "xx_fcall_expr ::= IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 275 */ "xx_fcall_expr ::= IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 276 */ "xx_fcall_expr ::= BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 277 */ "xx_fcall_expr ::= BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 278 */ "xx_scall_expr ::= IDENTIFIER DOUBLECOLON IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 279 */ "xx_scall_expr ::= IDENTIFIER DOUBLECOLON IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 280 */ "xx_scall_expr ::= BRACKET_OPEN IDENTIFIER BRACKET_CLOSE DOUBLECOLON IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 281 */ "xx_scall_expr ::= BRACKET_OPEN IDENTIFIER BRACKET_CLOSE DOUBLECOLON IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 282 */ "xx_scall_expr ::= BRACKET_OPEN IDENTIFIER BRACKET_CLOSE DOUBLECOLON BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 283 */ "xx_scall_expr ::= BRACKET_OPEN IDENTIFIER BRACKET_CLOSE DOUBLECOLON BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 284 */ "xx_mcall_expr ::= IDENTIFIER ARROW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 285 */ "xx_mcall_expr ::= IDENTIFIER ARROW IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 286 */ "xx_mcall_expr ::= IDENTIFIER ARROW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 287 */ "xx_mcall_expr ::= IDENTIFIER ARROW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 288 */ "xx_mcall_expr ::= IDENTIFIER ARROW BRACKET_OPEN STRING BRACKET_CLOSE PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 289 */ "xx_mcall_expr ::= IDENTIFIER ARROW BRACKET_OPEN STRING BRACKET_CLOSE PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 290 */ "xx_common_expr ::= xx_mcall_expr",
 /* 291 */ "xx_common_expr ::= xx_scall_expr",
 /* 292 */ "xx_common_expr ::= xx_fcall_expr",
 /* 293 */ "xx_call_parameters ::= xx_call_parameters COMMA xx_call_parameter",
 /* 294 */ "xx_call_parameters ::= xx_call_parameter",
 /* 295 */ "xx_call_parameter ::= xx_common_expr",
 /* 296 */ "xx_array_list ::= xx_array_list COMMA xx_array_item",
 /* 297 */ "xx_array_list ::= xx_array_item",
 /* 298 */ "xx_array_item ::= xx_array_key COLON xx_array_value",
 /* 299 */ "xx_array_item ::= xx_array_value",
 /* 300 */ "xx_array_key ::= IDENTIFIER",
 /* 301 */ "xx_array_key ::= STRING",
 /* 302 */ "xx_array_key ::= INTEGER",
 /* 303 */ "xx_array_value ::= xx_common_expr",
 /* 304 */ "xx_literal_expr ::= INTEGER",
 /* 305 */ "xx_literal_expr ::= CHAR",
 /* 306 */ "xx_literal_expr ::= STRING",
 /* 307 */ "xx_literal_expr ::= DOUBLE",
 /* 308 */ "xx_literal_expr ::= NULL",
 /* 309 */ "xx_literal_expr ::= FALSE",
 /* 310 */ "xx_literal_expr ::= TRUE",
 /* 311 */ "xx_literal_expr ::= IDENTIFIER DOUBLECOLON CONSTANT",
 /* 312 */ "xx_isset_expr ::= IDENTIFIER SBRACKET_OPEN xx_common_expr SBRACKET_CLOSE",
 /* 313 */ "xx_isset_expr ::= IDENTIFIER ARROW IDENTIFIER",
 /* 314 */ "xx_isset_expr ::= IDENTIFIER ARROW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE",
 /* 315 */ "xx_isset_expr ::= IDENTIFIER ARROW BRACKET_OPEN STRING BRACKET_CLOSE",
 /* 316 */ "xx_eval_expr ::= xx_common_expr",
 /* 317 */ "xx_comment ::= COMMENT",
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
// 982 "parser.lemon"
{
	/*if ((yypminor->yy0)) {
		if ((yypminor->yy0)->free_flag) {
			efree((yypminor->yy0)->token);
		}
		efree((yypminor->yy0));
	}*/
}
// 2773 "parser.c"
      break;
    case 104:
// 995 "parser.lemon"
{ json_object_put((yypminor->yy295)); }
// 2778 "parser.c"
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
  { 103, 1 },
  { 104, 1 },
  { 105, 2 },
  { 105, 1 },
  { 106, 1 },
  { 106, 1 },
  { 106, 1 },
  { 106, 1 },
  { 107, 3 },
  { 109, 3 },
  { 109, 5 },
  { 108, 3 },
  { 108, 5 },
  { 108, 5 },
  { 108, 7 },
  { 108, 4 },
  { 108, 6 },
  { 108, 6 },
  { 108, 4 },
  { 108, 6 },
  { 112, 2 },
  { 112, 3 },
  { 113, 3 },
  { 113, 1 },
  { 115, 1 },
  { 111, 2 },
  { 111, 3 },
  { 114, 1 },
  { 114, 1 },
  { 114, 1 },
  { 114, 2 },
  { 114, 2 },
  { 114, 2 },
  { 114, 2 },
  { 114, 3 },
  { 114, 3 },
  { 117, 2 },
  { 117, 1 },
  { 120, 4 },
  { 120, 3 },
  { 120, 6 },
  { 120, 5 },
  { 120, 5 },
  { 120, 4 },
  { 120, 7 },
  { 120, 6 },
  { 123, 2 },
  { 123, 3 },
  { 124, 3 },
  { 124, 1 },
  { 125, 1 },
  { 125, 2 },
  { 118, 2 },
  { 118, 1 },
  { 119, 2 },
  { 119, 1 },
  { 116, 2 },
  { 116, 1 },
  { 126, 6 },
  { 126, 5 },
  { 127, 7 },
  { 127, 8 },
  { 127, 8 },
  { 127, 9 },
  { 127, 8 },
  { 127, 9 },
  { 127, 9 },
  { 127, 10 },
  { 127, 9 },
  { 127, 10 },
  { 127, 10 },
  { 127, 11 },
  { 127, 10 },
  { 127, 11 },
  { 127, 11 },
  { 127, 12 },
  { 128, 8 },
  { 128, 9 },
  { 128, 9 },
  { 128, 10 },
  { 128, 6 },
  { 128, 7 },
  { 128, 7 },
  { 128, 8 },
  { 121, 2 },
  { 121, 1 },
  { 132, 1 },
  { 132, 1 },
  { 132, 1 },
  { 132, 1 },
  { 132, 1 },
  { 132, 1 },
  { 132, 1 },
  { 132, 1 },
  { 131, 1 },
  { 131, 1 },
  { 131, 2 },
  { 131, 1 },
  { 129, 3 },
  { 129, 1 },
  { 135, 1 },
  { 135, 2 },
  { 135, 3 },
  { 135, 2 },
  { 135, 3 },
  { 135, 4 },
  { 135, 5 },
  { 135, 4 },
  { 134, 3 },
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
  { 130, 2 },
  { 130, 1 },
  { 136, 1 },
  { 136, 1 },
  { 136, 1 },
  { 136, 1 },
  { 136, 1 },
  { 136, 1 },
  { 136, 1 },
  { 136, 1 },
  { 136, 1 },
  { 136, 1 },
  { 136, 1 },
  { 136, 1 },
  { 136, 1 },
  { 136, 1 },
  { 136, 1 },
  { 136, 1 },
  { 136, 1 },
  { 136, 1 },
  { 136, 1 },
  { 136, 1 },
  { 155, 1 },
  { 149, 2 },
  { 150, 2 },
  { 138, 4 },
  { 138, 7 },
  { 138, 5 },
  { 138, 9 },
  { 138, 8 },
  { 138, 8 },
  { 153, 4 },
  { 153, 5 },
  { 157, 2 },
  { 157, 1 },
  { 158, 3 },
  { 158, 4 },
  { 158, 3 },
  { 139, 3 },
  { 139, 4 },
  { 151, 4 },
  { 151, 5 },
  { 152, 6 },
  { 152, 7 },
  { 154, 7 },
  { 154, 8 },
  { 154, 9 },
  { 154, 10 },
  { 137, 3 },
  { 160, 3 },
  { 160, 1 },
  { 162, 1 },
  { 162, 1 },
  { 162, 1 },
  { 162, 1 },
  { 162, 1 },
  { 162, 1 },
  { 161, 3 },
  { 161, 5 },
  { 161, 7 },
  { 161, 7 },
  { 161, 7 },
  { 161, 8 },
  { 161, 5 },
  { 161, 7 },
  { 161, 8 },
  { 161, 5 },
  { 161, 6 },
  { 161, 4 },
  { 161, 4 },
  { 161, 2 },
  { 161, 2 },
  { 164, 1 },
  { 140, 3 },
  { 165, 3 },
  { 165, 1 },
  { 166, 1 },
  { 144, 2 },
  { 143, 2 },
  { 145, 2 },
  { 142, 2 },
  { 141, 3 },
  { 141, 2 },
  { 146, 5 },
  { 146, 6 },
  { 147, 3 },
  { 148, 3 },
  { 148, 3 },
  { 148, 3 },
  { 148, 3 },
  { 148, 3 },
  { 148, 3 },
  { 148, 3 },
  { 148, 3 },
  { 148, 3 },
  { 148, 3 },
  { 171, 3 },
  { 171, 1 },
  { 172, 1 },
  { 172, 3 },
  { 163, 1 },
  { 159, 2 },
  { 159, 2 },
  { 159, 2 },
  { 159, 2 },
  { 159, 2 },
  { 159, 2 },
  { 159, 2 },
  { 159, 3 },
  { 159, 3 },
  { 159, 3 },
  { 159, 3 },
  { 159, 3 },
  { 159, 3 },
  { 159, 3 },
  { 159, 3 },
  { 159, 3 },
  { 159, 4 },
  { 159, 4 },
  { 159, 3 },
  { 159, 5 },
  { 159, 5 },
  { 159, 3 },
  { 159, 3 },
  { 159, 4 },
  { 159, 3 },
  { 159, 3 },
  { 159, 3 },
  { 159, 3 },
  { 159, 3 },
  { 159, 3 },
  { 159, 3 },
  { 159, 3 },
  { 159, 3 },
  { 170, 4 },
  { 159, 1 },
  { 159, 2 },
  { 159, 1 },
  { 159, 1 },
  { 159, 1 },
  { 159, 1 },
  { 159, 1 },
  { 159, 1 },
  { 159, 1 },
  { 159, 1 },
  { 159, 1 },
  { 159, 2 },
  { 159, 3 },
  { 159, 2 },
  { 159, 4 },
  { 159, 5 },
  { 159, 4 },
  { 159, 6 },
  { 159, 7 },
  { 168, 4 },
  { 168, 3 },
  { 168, 6 },
  { 168, 5 },
  { 169, 6 },
  { 169, 5 },
  { 169, 8 },
  { 169, 7 },
  { 169, 10 },
  { 169, 9 },
  { 167, 6 },
  { 167, 5 },
  { 167, 8 },
  { 167, 7 },
  { 167, 8 },
  { 167, 7 },
  { 159, 1 },
  { 159, 1 },
  { 159, 1 },
  { 175, 3 },
  { 175, 1 },
  { 176, 1 },
  { 174, 3 },
  { 174, 1 },
  { 177, 3 },
  { 177, 1 },
  { 178, 1 },
  { 178, 1 },
  { 178, 1 },
  { 179, 1 },
  { 122, 1 },
  { 122, 1 },
  { 122, 1 },
  { 122, 1 },
  { 122, 1 },
  { 122, 1 },
  { 122, 1 },
  { 122, 3 },
  { 173, 4 },
  { 173, 3 },
  { 173, 5 },
  { 173, 5 },
  { 156, 1 },
  { 110, 1 },
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
// 991 "parser.lemon"
{
	status->ret = yymsp[0].minor.yy295;
}
// 3313 "parser.c"
        break;
      case 1:
      case 4:
      case 5:
      case 6:
      case 7:
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
      case 138:
      case 139:
      case 140:
      case 141:
      case 192:
      case 220:
      case 255:
      case 290:
      case 291:
      case 292:
      case 295:
      case 303:
      case 316:
// 997 "parser.lemon"
{
	yygotominor.yy295 = yymsp[0].minor.yy295;
}
// 3353 "parser.c"
        break;
      case 2:
      case 36:
      case 52:
      case 54:
      case 56:
      case 84:
      case 120:
      case 153:
// 1001 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_list(yymsp[-1].minor.yy295, yymsp[0].minor.yy295);
}
// 3367 "parser.c"
        break;
      case 3:
      case 23:
      case 37:
      case 49:
      case 53:
      case 55:
      case 57:
      case 85:
      case 99:
      case 121:
      case 154:
      case 170:
      case 195:
      case 217:
      case 294:
      case 297:
// 1005 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_list(NULL, yymsp[0].minor.yy295);
}
// 3389 "parser.c"
        break;
      case 8:
// 1025 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_namespace(yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(35,&yymsp[-2].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 3398 "parser.c"
        break;
      case 9:
// 1029 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_interface(yymsp[-1].minor.yy0, yymsp[0].minor.yy295, NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-2].minor);
}
// 3406 "parser.c"
        break;
      case 10:
// 1033 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_interface(yymsp[-3].minor.yy0, yymsp[0].minor.yy295, yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(38,&yymsp[-4].minor);
  yy_destructor(39,&yymsp[-2].minor);
}
// 3415 "parser.c"
        break;
      case 11:
// 1037 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_class(yymsp[-1].minor.yy0, yymsp[0].minor.yy295, 0, 0, NULL, NULL, status->scanner_state);
  yy_destructor(40,&yymsp[-2].minor);
}
// 3423 "parser.c"
        break;
      case 12:
// 1041 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy295, 0, 0, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(40,&yymsp[-4].minor);
  yy_destructor(39,&yymsp[-2].minor);
}
// 3432 "parser.c"
        break;
      case 13:
// 1045 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy295, 0, 0, NULL, yymsp[-1].minor.yy295, status->scanner_state);
  yy_destructor(40,&yymsp[-4].minor);
  yy_destructor(41,&yymsp[-2].minor);
}
// 3441 "parser.c"
        break;
      case 14:
// 1049 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_class(yymsp[-5].minor.yy0, yymsp[0].minor.yy295, 0, 0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy295, status->scanner_state);
  yy_destructor(40,&yymsp[-6].minor);
  yy_destructor(39,&yymsp[-4].minor);
  yy_destructor(41,&yymsp[-2].minor);
}
// 3451 "parser.c"
        break;
      case 15:
// 1053 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_class(yymsp[-1].minor.yy0, yymsp[0].minor.yy295, 1, 0, NULL, NULL, status->scanner_state);
  yy_destructor(42,&yymsp[-3].minor);
  yy_destructor(40,&yymsp[-2].minor);
}
// 3460 "parser.c"
        break;
      case 16:
// 1057 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy295, 1, 0, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(42,&yymsp[-5].minor);
  yy_destructor(40,&yymsp[-4].minor);
  yy_destructor(39,&yymsp[-2].minor);
}
// 3470 "parser.c"
        break;
      case 17:
// 1061 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy295, 1, 0, NULL, yymsp[-1].minor.yy295, status->scanner_state);
  yy_destructor(42,&yymsp[-5].minor);
  yy_destructor(40,&yymsp[-4].minor);
  yy_destructor(41,&yymsp[-2].minor);
}
// 3480 "parser.c"
        break;
      case 18:
// 1065 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_class(yymsp[-1].minor.yy0, yymsp[0].minor.yy295, 0, 1, NULL, NULL, status->scanner_state);
  yy_destructor(43,&yymsp[-3].minor);
  yy_destructor(40,&yymsp[-2].minor);
}
// 3489 "parser.c"
        break;
      case 19:
// 1069 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy295, 0, 1, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(43,&yymsp[-5].minor);
  yy_destructor(40,&yymsp[-4].minor);
  yy_destructor(39,&yymsp[-2].minor);
}
// 3499 "parser.c"
        break;
      case 20:
      case 25:
      case 46:
// 1073 "parser.lemon"
{
	yygotominor.yy295 = NULL;
  yy_destructor(44,&yymsp[-1].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 3510 "parser.c"
        break;
      case 21:
      case 26:
      case 47:
// 1077 "parser.lemon"
{
	yygotominor.yy295 = yymsp[-1].minor.yy295;
  yy_destructor(44,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 3521 "parser.c"
        break;
      case 22:
      case 48:
      case 98:
      case 169:
      case 194:
      case 216:
      case 293:
      case 296:
// 1081 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_list(yymsp[-2].minor.yy295, yymsp[0].minor.yy295);
  yy_destructor(6,&yymsp[-1].minor);
}
// 3536 "parser.c"
        break;
      case 24:
      case 258:
      case 300:
// 1089 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state);
}
// 3545 "parser.c"
        break;
      case 27:
// 1101 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_class_definition(yymsp[0].minor.yy295, NULL, NULL, status->scanner_state);
}
// 3552 "parser.c"
        break;
      case 28:
// 1105 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_class_definition(NULL, NULL, yymsp[0].minor.yy295, status->scanner_state);
}
// 3559 "parser.c"
        break;
      case 29:
// 1109 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_class_definition(NULL, yymsp[0].minor.yy295, NULL, status->scanner_state);
}
// 3566 "parser.c"
        break;
      case 30:
// 1113 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_class_definition(yymsp[-1].minor.yy295, yymsp[0].minor.yy295, NULL, status->scanner_state);
}
// 3573 "parser.c"
        break;
      case 31:
// 1117 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_class_definition(yymsp[-1].minor.yy295, NULL, yymsp[0].minor.yy295, status->scanner_state);
}
// 3580 "parser.c"
        break;
      case 32:
// 1121 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_class_definition(yymsp[0].minor.yy295, NULL, yymsp[-1].minor.yy295, status->scanner_state);
}
// 3587 "parser.c"
        break;
      case 33:
// 1125 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_class_definition(NULL, yymsp[0].minor.yy295, yymsp[-1].minor.yy295, status->scanner_state);
}
// 3594 "parser.c"
        break;
      case 34:
// 1129 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_class_definition(yymsp[-2].minor.yy295, yymsp[0].minor.yy295, yymsp[-1].minor.yy295, status->scanner_state);
}
// 3601 "parser.c"
        break;
      case 35:
// 1133 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_class_definition(yymsp[-1].minor.yy295, yymsp[0].minor.yy295, yymsp[-2].minor.yy295, status->scanner_state);
}
// 3608 "parser.c"
        break;
      case 38:
// 1150 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_class_property(yymsp[-2].minor.yy295, yymsp[-1].minor.yy0, NULL, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(37,&yymsp[0].minor);
}
// 3616 "parser.c"
        break;
      case 39:
// 1154 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_class_property(yymsp[-2].minor.yy295, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(37,&yymsp[0].minor);
}
// 3624 "parser.c"
        break;
      case 40:
// 1158 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_class_property(yymsp[-4].minor.yy295, yymsp[-3].minor.yy0, yymsp[-1].minor.yy295, yymsp[-5].minor.yy0, NULL, status->scanner_state);
  yy_destructor(47,&yymsp[-2].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 3633 "parser.c"
        break;
      case 41:
// 1162 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_class_property(yymsp[-4].minor.yy295, yymsp[-3].minor.yy0, yymsp[-1].minor.yy295, NULL, NULL, status->scanner_state);
  yy_destructor(47,&yymsp[-2].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 3642 "parser.c"
        break;
      case 42:
// 1166 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_class_property(yymsp[-3].minor.yy295, yymsp[-2].minor.yy0, NULL, yymsp[-4].minor.yy0, yymsp[-1].minor.yy295, status->scanner_state);
  yy_destructor(37,&yymsp[0].minor);
}
// 3650 "parser.c"
        break;
      case 43:
// 1170 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_class_property(yymsp[-3].minor.yy295, yymsp[-2].minor.yy0, NULL, NULL, yymsp[-1].minor.yy295, status->scanner_state);
  yy_destructor(37,&yymsp[0].minor);
}
// 3658 "parser.c"
        break;
      case 44:
// 1174 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_class_property(yymsp[-5].minor.yy295, yymsp[-4].minor.yy0, yymsp[-2].minor.yy295, yymsp[-6].minor.yy0, yymsp[-1].minor.yy295, status->scanner_state);
  yy_destructor(47,&yymsp[-3].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 3667 "parser.c"
        break;
      case 45:
// 1178 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_class_property(yymsp[-5].minor.yy295, yymsp[-4].minor.yy0, yymsp[-2].minor.yy295, NULL, yymsp[-1].minor.yy295, status->scanner_state);
  yy_destructor(47,&yymsp[-3].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 3676 "parser.c"
        break;
      case 50:
// 1198 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_property_shortcut(NULL, yymsp[0].minor.yy0, status->scanner_state);
}
// 3683 "parser.c"
        break;
      case 51:
// 1202 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_property_shortcut(yymsp[-1].minor.yy0, yymsp[0].minor.yy0, status->scanner_state);
}
// 3690 "parser.c"
        break;
      case 58:
// 1231 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy295, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(48,&yymsp[-4].minor);
  yy_destructor(47,&yymsp[-2].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 3700 "parser.c"
        break;
      case 59:
// 1235 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy295, NULL, status->scanner_state);
  yy_destructor(48,&yymsp[-4].minor);
  yy_destructor(47,&yymsp[-2].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 3710 "parser.c"
        break;
      case 60:
// 1240 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_class_method(yymsp[-6].minor.yy295, yymsp[-4].minor.yy0, NULL, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(50,&yymsp[-5].minor);
  yy_destructor(51,&yymsp[-3].minor);
  yy_destructor(34,&yymsp[-2].minor);
  yy_destructor(44,&yymsp[-1].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 3722 "parser.c"
        break;
      case 61:
// 1244 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_class_method(yymsp[-7].minor.yy295, yymsp[-5].minor.yy0, yymsp[-3].minor.yy295, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(50,&yymsp[-6].minor);
  yy_destructor(51,&yymsp[-4].minor);
  yy_destructor(34,&yymsp[-2].minor);
  yy_destructor(44,&yymsp[-1].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 3734 "parser.c"
        break;
      case 62:
// 1248 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_class_method(yymsp[-7].minor.yy295, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy295, NULL, NULL, status->scanner_state);
  yy_destructor(50,&yymsp[-6].minor);
  yy_destructor(51,&yymsp[-4].minor);
  yy_destructor(34,&yymsp[-3].minor);
  yy_destructor(44,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 3746 "parser.c"
        break;
      case 63:
// 1252 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_class_method(yymsp[-8].minor.yy295, yymsp[-6].minor.yy0, yymsp[-4].minor.yy295, yymsp[-1].minor.yy295, NULL, NULL, status->scanner_state);
  yy_destructor(50,&yymsp[-7].minor);
  yy_destructor(51,&yymsp[-5].minor);
  yy_destructor(34,&yymsp[-3].minor);
  yy_destructor(44,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 3758 "parser.c"
        break;
      case 64:
// 1256 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_class_method(yymsp[-6].minor.yy295, yymsp[-4].minor.yy0, NULL, NULL, yymsp[-7].minor.yy0, NULL, status->scanner_state);
  yy_destructor(50,&yymsp[-5].minor);
  yy_destructor(51,&yymsp[-3].minor);
  yy_destructor(34,&yymsp[-2].minor);
  yy_destructor(44,&yymsp[-1].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 3770 "parser.c"
        break;
      case 65:
// 1260 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_class_method(yymsp[-7].minor.yy295, yymsp[-5].minor.yy0, yymsp[-3].minor.yy295, NULL, yymsp[-8].minor.yy0, NULL, status->scanner_state);
  yy_destructor(50,&yymsp[-6].minor);
  yy_destructor(51,&yymsp[-4].minor);
  yy_destructor(34,&yymsp[-2].minor);
  yy_destructor(44,&yymsp[-1].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 3782 "parser.c"
        break;
      case 66:
// 1264 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_class_method(yymsp[-7].minor.yy295, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy295, yymsp[-8].minor.yy0, NULL, status->scanner_state);
  yy_destructor(50,&yymsp[-6].minor);
  yy_destructor(51,&yymsp[-4].minor);
  yy_destructor(34,&yymsp[-3].minor);
  yy_destructor(44,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 3794 "parser.c"
        break;
      case 67:
// 1268 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_class_method(yymsp[-8].minor.yy295, yymsp[-6].minor.yy0, yymsp[-4].minor.yy295, yymsp[-1].minor.yy295, yymsp[-9].minor.yy0, NULL, status->scanner_state);
  yy_destructor(50,&yymsp[-7].minor);
  yy_destructor(51,&yymsp[-5].minor);
  yy_destructor(34,&yymsp[-3].minor);
  yy_destructor(44,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 3806 "parser.c"
        break;
      case 68:
// 1272 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_class_method(yymsp[-8].minor.yy295, yymsp[-6].minor.yy0, NULL, NULL, NULL, yymsp[-2].minor.yy295, status->scanner_state);
  yy_destructor(50,&yymsp[-7].minor);
  yy_destructor(51,&yymsp[-5].minor);
  yy_destructor(34,&yymsp[-4].minor);
  yy_destructor(52,&yymsp[-3].minor);
  yy_destructor(44,&yymsp[-1].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 3819 "parser.c"
        break;
      case 69:
// 1276 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_class_method(yymsp[-9].minor.yy295, yymsp[-7].minor.yy0, yymsp[-5].minor.yy295, NULL, NULL, yymsp[-2].minor.yy295, status->scanner_state);
  yy_destructor(50,&yymsp[-8].minor);
  yy_destructor(51,&yymsp[-6].minor);
  yy_destructor(34,&yymsp[-4].minor);
  yy_destructor(52,&yymsp[-3].minor);
  yy_destructor(44,&yymsp[-1].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 3832 "parser.c"
        break;
      case 70:
// 1280 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_class_method(yymsp[-9].minor.yy295, yymsp[-7].minor.yy0, NULL, yymsp[-1].minor.yy295, NULL, yymsp[-3].minor.yy295, status->scanner_state);
  yy_destructor(50,&yymsp[-8].minor);
  yy_destructor(51,&yymsp[-6].minor);
  yy_destructor(34,&yymsp[-5].minor);
  yy_destructor(52,&yymsp[-4].minor);
  yy_destructor(44,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 3845 "parser.c"
        break;
      case 71:
// 1284 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_class_method(yymsp[-10].minor.yy295, yymsp[-8].minor.yy0, yymsp[-6].minor.yy295, yymsp[-1].minor.yy295, NULL, yymsp[-3].minor.yy295, status->scanner_state);
  yy_destructor(50,&yymsp[-9].minor);
  yy_destructor(51,&yymsp[-7].minor);
  yy_destructor(34,&yymsp[-5].minor);
  yy_destructor(52,&yymsp[-4].minor);
  yy_destructor(44,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 3858 "parser.c"
        break;
      case 72:
// 1288 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_class_method(yymsp[-8].minor.yy295, yymsp[-6].minor.yy0, NULL, NULL, yymsp[-9].minor.yy0, yymsp[-2].minor.yy295, status->scanner_state);
  yy_destructor(50,&yymsp[-7].minor);
  yy_destructor(51,&yymsp[-5].minor);
  yy_destructor(34,&yymsp[-4].minor);
  yy_destructor(52,&yymsp[-3].minor);
  yy_destructor(44,&yymsp[-1].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 3871 "parser.c"
        break;
      case 73:
// 1292 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_class_method(yymsp[-9].minor.yy295, yymsp[-7].minor.yy0, yymsp[-5].minor.yy295, NULL, yymsp[-10].minor.yy0, yymsp[-2].minor.yy295, status->scanner_state);
  yy_destructor(50,&yymsp[-8].minor);
  yy_destructor(51,&yymsp[-6].minor);
  yy_destructor(34,&yymsp[-4].minor);
  yy_destructor(52,&yymsp[-3].minor);
  yy_destructor(44,&yymsp[-1].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 3884 "parser.c"
        break;
      case 74:
// 1296 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_class_method(yymsp[-9].minor.yy295, yymsp[-7].minor.yy0, NULL, yymsp[-1].minor.yy295, yymsp[-10].minor.yy0, yymsp[-3].minor.yy295, status->scanner_state);
  yy_destructor(50,&yymsp[-8].minor);
  yy_destructor(51,&yymsp[-6].minor);
  yy_destructor(34,&yymsp[-5].minor);
  yy_destructor(52,&yymsp[-4].minor);
  yy_destructor(44,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 3897 "parser.c"
        break;
      case 75:
// 1300 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_class_method(yymsp[-10].minor.yy295, yymsp[-8].minor.yy0, yymsp[-6].minor.yy295, yymsp[-1].minor.yy295, yymsp[-11].minor.yy0, yymsp[-3].minor.yy295, status->scanner_state);
  yy_destructor(50,&yymsp[-9].minor);
  yy_destructor(51,&yymsp[-7].minor);
  yy_destructor(34,&yymsp[-5].minor);
  yy_destructor(52,&yymsp[-4].minor);
  yy_destructor(44,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 3910 "parser.c"
        break;
      case 76:
// 1305 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_class_method(yymsp[-7].minor.yy295, yymsp[-5].minor.yy0, NULL, NULL, NULL, yymsp[-1].minor.yy295, status->scanner_state);
  yy_destructor(50,&yymsp[-6].minor);
  yy_destructor(51,&yymsp[-4].minor);
  yy_destructor(34,&yymsp[-3].minor);
  yy_destructor(52,&yymsp[-2].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 3922 "parser.c"
        break;
      case 77:
// 1309 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_class_method(yymsp[-8].minor.yy295, yymsp[-6].minor.yy0, yymsp[-4].minor.yy295, NULL, NULL, yymsp[-1].minor.yy295, status->scanner_state);
  yy_destructor(50,&yymsp[-7].minor);
  yy_destructor(51,&yymsp[-5].minor);
  yy_destructor(34,&yymsp[-3].minor);
  yy_destructor(52,&yymsp[-2].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 3934 "parser.c"
        break;
      case 78:
// 1313 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_class_method(yymsp[-7].minor.yy295, yymsp[-5].minor.yy0, NULL, NULL, yymsp[-8].minor.yy0, yymsp[-1].minor.yy295, status->scanner_state);
  yy_destructor(50,&yymsp[-6].minor);
  yy_destructor(51,&yymsp[-4].minor);
  yy_destructor(34,&yymsp[-3].minor);
  yy_destructor(52,&yymsp[-2].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 3946 "parser.c"
        break;
      case 79:
// 1317 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_class_method(yymsp[-8].minor.yy295, yymsp[-6].minor.yy0, yymsp[-4].minor.yy295, NULL, yymsp[-9].minor.yy0, yymsp[-1].minor.yy295, status->scanner_state);
  yy_destructor(50,&yymsp[-7].minor);
  yy_destructor(51,&yymsp[-5].minor);
  yy_destructor(34,&yymsp[-3].minor);
  yy_destructor(52,&yymsp[-2].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 3958 "parser.c"
        break;
      case 80:
// 1321 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_class_method(yymsp[-5].minor.yy295, yymsp[-3].minor.yy0, NULL, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(50,&yymsp[-4].minor);
  yy_destructor(51,&yymsp[-2].minor);
  yy_destructor(34,&yymsp[-1].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 3969 "parser.c"
        break;
      case 81:
// 1325 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_class_method(yymsp[-6].minor.yy295, yymsp[-4].minor.yy0, yymsp[-2].minor.yy295, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(50,&yymsp[-5].minor);
  yy_destructor(51,&yymsp[-3].minor);
  yy_destructor(34,&yymsp[-1].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 3980 "parser.c"
        break;
      case 82:
// 1329 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_class_method(yymsp[-5].minor.yy295, yymsp[-3].minor.yy0, NULL, NULL, yymsp[-6].minor.yy0, NULL, status->scanner_state);
  yy_destructor(50,&yymsp[-4].minor);
  yy_destructor(51,&yymsp[-2].minor);
  yy_destructor(34,&yymsp[-1].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 3991 "parser.c"
        break;
      case 83:
// 1333 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_class_method(yymsp[-6].minor.yy295, yymsp[-4].minor.yy0, yymsp[-2].minor.yy295, NULL, yymsp[-7].minor.yy0, NULL, status->scanner_state);
  yy_destructor(50,&yymsp[-5].minor);
  yy_destructor(51,&yymsp[-3].minor);
  yy_destructor(34,&yymsp[-1].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 4002 "parser.c"
        break;
      case 86:
// 1346 "parser.lemon"
{
	yygotominor.yy295 = json_object_new_string("public");
  yy_destructor(1,&yymsp[0].minor);
}
// 4010 "parser.c"
        break;
      case 87:
// 1350 "parser.lemon"
{
	yygotominor.yy295 = json_object_new_string("protected");
  yy_destructor(2,&yymsp[0].minor);
}
// 4018 "parser.c"
        break;
      case 88:
// 1354 "parser.lemon"
{
	yygotominor.yy295 = json_object_new_string("private");
  yy_destructor(4,&yymsp[0].minor);
}
// 4026 "parser.c"
        break;
      case 89:
// 1358 "parser.lemon"
{
	yygotominor.yy295 = json_object_new_string("static");
  yy_destructor(3,&yymsp[0].minor);
}
// 4034 "parser.c"
        break;
      case 90:
// 1362 "parser.lemon"
{
	yygotominor.yy295 = json_object_new_string("scoped");
  yy_destructor(5,&yymsp[0].minor);
}
// 4042 "parser.c"
        break;
      case 91:
// 1366 "parser.lemon"
{
	yygotominor.yy295 = json_object_new_string("inline");
  yy_destructor(53,&yymsp[0].minor);
}
// 4050 "parser.c"
        break;
      case 92:
// 1370 "parser.lemon"
{
	yygotominor.yy295 = json_object_new_string("abstract");
  yy_destructor(42,&yymsp[0].minor);
}
// 4058 "parser.c"
        break;
      case 93:
// 1374 "parser.lemon"
{
	yygotominor.yy295 = json_object_new_string("final");
  yy_destructor(43,&yymsp[0].minor);
}
// 4066 "parser.c"
        break;
      case 94:
// 1379 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_return_type(NULL, NULL, 0, 1, status->scanner_state);
  yy_destructor(54,&yymsp[0].minor);
}
// 4074 "parser.c"
        break;
      case 95:
// 1383 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_return_type(yymsp[0].minor.yy295, NULL, 0, 0, status->scanner_state);
}
// 4081 "parser.c"
        break;
      case 96:
// 1387 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_return_type(yymsp[-1].minor.yy295, NULL, 1, 0, status->scanner_state);
  yy_destructor(33,&yymsp[0].minor);
}
// 4089 "parser.c"
        break;
      case 97:
// 1391 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_return_type(NULL, yymsp[0].minor.yy295, 0, 0, status->scanner_state);
}
// 4096 "parser.c"
        break;
      case 100:
// 1404 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_parameter(NULL, NULL, yymsp[0].minor.yy0, NULL, 0, status->scanner_state);
}
// 4103 "parser.c"
        break;
      case 101:
// 1408 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_parameter(yymsp[-1].minor.yy295, NULL, yymsp[0].minor.yy0, NULL, 0, status->scanner_state);
}
// 4110 "parser.c"
        break;
      case 102:
// 1412 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_parameter(yymsp[-2].minor.yy295, NULL, yymsp[0].minor.yy0, NULL, 1, status->scanner_state);
  yy_destructor(33,&yymsp[-1].minor);
}
// 4118 "parser.c"
        break;
      case 103:
// 1416 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_parameter(NULL, yymsp[-1].minor.yy295, yymsp[0].minor.yy0, NULL, 0, status->scanner_state);
}
// 4125 "parser.c"
        break;
      case 104:
// 1420 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_parameter(NULL, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy295, 0, status->scanner_state);
  yy_destructor(47,&yymsp[-1].minor);
}
// 4133 "parser.c"
        break;
      case 105:
// 1424 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_parameter(yymsp[-3].minor.yy295, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy295, 0, status->scanner_state);
  yy_destructor(47,&yymsp[-1].minor);
}
// 4141 "parser.c"
        break;
      case 106:
// 1428 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_parameter(yymsp[-4].minor.yy295, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy295, 1, status->scanner_state);
  yy_destructor(33,&yymsp[-3].minor);
  yy_destructor(47,&yymsp[-1].minor);
}
// 4150 "parser.c"
        break;
      case 107:
// 1432 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_parameter(NULL, yymsp[-3].minor.yy295, yymsp[-2].minor.yy0, yymsp[0].minor.yy295, 0, status->scanner_state);
  yy_destructor(47,&yymsp[-1].minor);
}
// 4158 "parser.c"
        break;
      case 108:
// 1436 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(17,&yymsp[-2].minor);
  yy_destructor(18,&yymsp[0].minor);
}
// 4167 "parser.c"
        break;
      case 109:
// 1440 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_type(XX_TYPE_INTEGER);
  yy_destructor(55,&yymsp[0].minor);
}
// 4175 "parser.c"
        break;
      case 110:
// 1444 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_type(XX_TYPE_UINTEGER);
  yy_destructor(56,&yymsp[0].minor);
}
// 4183 "parser.c"
        break;
      case 111:
// 1448 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_type(XX_TYPE_LONG);
  yy_destructor(57,&yymsp[0].minor);
}
// 4191 "parser.c"
        break;
      case 112:
// 1452 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_type(XX_TYPE_ULONG);
  yy_destructor(58,&yymsp[0].minor);
}
// 4199 "parser.c"
        break;
      case 113:
// 1456 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_type(XX_TYPE_CHAR);
  yy_destructor(59,&yymsp[0].minor);
}
// 4207 "parser.c"
        break;
      case 114:
// 1460 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_type(XX_TYPE_UCHAR);
  yy_destructor(60,&yymsp[0].minor);
}
// 4215 "parser.c"
        break;
      case 115:
// 1464 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_type(XX_TYPE_DOUBLE);
  yy_destructor(61,&yymsp[0].minor);
}
// 4223 "parser.c"
        break;
      case 116:
// 1468 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_type(XX_TYPE_BOOL);
  yy_destructor(62,&yymsp[0].minor);
}
// 4231 "parser.c"
        break;
      case 117:
// 1472 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_type(XX_TYPE_STRING);
  yy_destructor(63,&yymsp[0].minor);
}
// 4239 "parser.c"
        break;
      case 118:
// 1476 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_type(XX_TYPE_ARRAY);
  yy_destructor(64,&yymsp[0].minor);
}
// 4247 "parser.c"
        break;
      case 119:
// 1480 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_type(XX_TYPE_VAR);
  yy_destructor(65,&yymsp[0].minor);
}
// 4255 "parser.c"
        break;
      case 142:
// 1572 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_empty_statement(status->scanner_state);
  yy_destructor(37,&yymsp[0].minor);
}
// 4263 "parser.c"
        break;
      case 143:
// 1576 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_break_statement(status->scanner_state);
  yy_destructor(66,&yymsp[-1].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 4272 "parser.c"
        break;
      case 144:
// 1580 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_continue_statement(status->scanner_state);
  yy_destructor(67,&yymsp[-1].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 4281 "parser.c"
        break;
      case 145:
// 1584 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_if_statement(yymsp[-2].minor.yy295, NULL, NULL, status->scanner_state);
  yy_destructor(68,&yymsp[-3].minor);
  yy_destructor(44,&yymsp[-1].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 4291 "parser.c"
        break;
      case 146:
// 1588 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_if_statement(yymsp[-5].minor.yy295, NULL, NULL, status->scanner_state);
  yy_destructor(68,&yymsp[-6].minor);
  yy_destructor(44,&yymsp[-4].minor);
  yy_destructor(45,&yymsp[-3].minor);
  yy_destructor(69,&yymsp[-2].minor);
  yy_destructor(44,&yymsp[-1].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 4304 "parser.c"
        break;
      case 147:
// 1592 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_if_statement(yymsp[-3].minor.yy295, yymsp[-1].minor.yy295, NULL, status->scanner_state);
  yy_destructor(68,&yymsp[-4].minor);
  yy_destructor(44,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 4314 "parser.c"
        break;
      case 148:
// 1596 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_if_statement(yymsp[-7].minor.yy295, yymsp[-5].minor.yy295, yymsp[-1].minor.yy295, status->scanner_state);
  yy_destructor(68,&yymsp[-8].minor);
  yy_destructor(44,&yymsp[-6].minor);
  yy_destructor(45,&yymsp[-4].minor);
  yy_destructor(69,&yymsp[-3].minor);
  yy_destructor(44,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 4327 "parser.c"
        break;
      case 149:
// 1600 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_if_statement(yymsp[-6].minor.yy295, yymsp[-4].minor.yy295, NULL, status->scanner_state);
  yy_destructor(68,&yymsp[-7].minor);
  yy_destructor(44,&yymsp[-5].minor);
  yy_destructor(45,&yymsp[-3].minor);
  yy_destructor(69,&yymsp[-2].minor);
  yy_destructor(44,&yymsp[-1].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 4340 "parser.c"
        break;
      case 150:
// 1604 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_if_statement(yymsp[-6].minor.yy295, NULL, yymsp[-1].minor.yy295, status->scanner_state);
  yy_destructor(68,&yymsp[-7].minor);
  yy_destructor(44,&yymsp[-5].minor);
  yy_destructor(45,&yymsp[-4].minor);
  yy_destructor(69,&yymsp[-3].minor);
  yy_destructor(44,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 4353 "parser.c"
        break;
      case 151:
// 1608 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_switch_statement(yymsp[-2].minor.yy295, NULL, status->scanner_state);
  yy_destructor(70,&yymsp[-3].minor);
  yy_destructor(44,&yymsp[-1].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 4363 "parser.c"
        break;
      case 152:
// 1612 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_switch_statement(yymsp[-3].minor.yy295, yymsp[-1].minor.yy295, status->scanner_state);
  yy_destructor(70,&yymsp[-4].minor);
  yy_destructor(44,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 4373 "parser.c"
        break;
      case 155:
// 1624 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_case_clause(yymsp[-1].minor.yy295, NULL, status->scanner_state);
  yy_destructor(71,&yymsp[-2].minor);
  yy_destructor(72,&yymsp[0].minor);
}
// 4382 "parser.c"
        break;
      case 156:
// 1628 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_case_clause(yymsp[-2].minor.yy295, yymsp[0].minor.yy295, status->scanner_state);
  yy_destructor(71,&yymsp[-3].minor);
  yy_destructor(72,&yymsp[-1].minor);
}
// 4391 "parser.c"
        break;
      case 157:
// 1632 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_case_clause(NULL, yymsp[0].minor.yy295, status->scanner_state);
  yy_destructor(73,&yymsp[-2].minor);
  yy_destructor(72,&yymsp[-1].minor);
}
// 4400 "parser.c"
        break;
      case 158:
// 1636 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_loop_statement(NULL, status->scanner_state);
  yy_destructor(74,&yymsp[-2].minor);
  yy_destructor(44,&yymsp[-1].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 4410 "parser.c"
        break;
      case 159:
// 1640 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_loop_statement(yymsp[-1].minor.yy295, status->scanner_state);
  yy_destructor(74,&yymsp[-3].minor);
  yy_destructor(44,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 4420 "parser.c"
        break;
      case 160:
// 1644 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_while_statement(yymsp[-2].minor.yy295, NULL, status->scanner_state);
  yy_destructor(75,&yymsp[-3].minor);
  yy_destructor(44,&yymsp[-1].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 4430 "parser.c"
        break;
      case 161:
// 1648 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_while_statement(yymsp[-3].minor.yy295, yymsp[-1].minor.yy295, status->scanner_state);
  yy_destructor(75,&yymsp[-4].minor);
  yy_destructor(44,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 4440 "parser.c"
        break;
      case 162:
// 1652 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_do_while_statement(yymsp[-1].minor.yy295, NULL, status->scanner_state);
  yy_destructor(76,&yymsp[-5].minor);
  yy_destructor(44,&yymsp[-4].minor);
  yy_destructor(45,&yymsp[-3].minor);
  yy_destructor(75,&yymsp[-2].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 4452 "parser.c"
        break;
      case 163:
// 1656 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_do_while_statement(yymsp[-1].minor.yy295, yymsp[-4].minor.yy295, status->scanner_state);
  yy_destructor(76,&yymsp[-6].minor);
  yy_destructor(44,&yymsp[-5].minor);
  yy_destructor(45,&yymsp[-3].minor);
  yy_destructor(75,&yymsp[-2].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 4464 "parser.c"
        break;
      case 164:
// 1660 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_for_statement(yymsp[-3].minor.yy295, NULL, yymsp[-5].minor.yy0, 0, yymsp[-1].minor.yy295, status->scanner_state);
  yy_destructor(77,&yymsp[-6].minor);
  yy_destructor(78,&yymsp[-4].minor);
  yy_destructor(44,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 4475 "parser.c"
        break;
      case 165:
// 1664 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_for_statement(yymsp[-3].minor.yy295, NULL, yymsp[-6].minor.yy0, 1, yymsp[-1].minor.yy295, status->scanner_state);
  yy_destructor(77,&yymsp[-7].minor);
  yy_destructor(78,&yymsp[-5].minor);
  yy_destructor(79,&yymsp[-4].minor);
  yy_destructor(44,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 4487 "parser.c"
        break;
      case 166:
// 1668 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_for_statement(yymsp[-3].minor.yy295, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, 0, yymsp[-1].minor.yy295, status->scanner_state);
  yy_destructor(77,&yymsp[-8].minor);
  yy_destructor(6,&yymsp[-6].minor);
  yy_destructor(78,&yymsp[-4].minor);
  yy_destructor(44,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 4499 "parser.c"
        break;
      case 167:
// 1672 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_for_statement(yymsp[-3].minor.yy295, yymsp[-8].minor.yy0, yymsp[-6].minor.yy0, 1, yymsp[-1].minor.yy295, status->scanner_state);
  yy_destructor(77,&yymsp[-9].minor);
  yy_destructor(6,&yymsp[-7].minor);
  yy_destructor(78,&yymsp[-5].minor);
  yy_destructor(79,&yymsp[-4].minor);
  yy_destructor(44,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 4512 "parser.c"
        break;
      case 168:
// 1676 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_let_statement(yymsp[-1].minor.yy295, status->scanner_state);
  yy_destructor(80,&yymsp[-2].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 4521 "parser.c"
        break;
      case 171:
// 1689 "parser.lemon"
{
	yygotominor.yy295 = json_object_new_string("assign");
  yy_destructor(47,&yymsp[0].minor);
}
// 4529 "parser.c"
        break;
      case 172:
// 1694 "parser.lemon"
{
	yygotominor.yy295 = json_object_new_string("add-assign");
  yy_destructor(81,&yymsp[0].minor);
}
// 4537 "parser.c"
        break;
      case 173:
// 1699 "parser.lemon"
{
	yygotominor.yy295 = json_object_new_string("sub-assign");
  yy_destructor(82,&yymsp[0].minor);
}
// 4545 "parser.c"
        break;
      case 174:
// 1703 "parser.lemon"
{
	yygotominor.yy295 = json_object_new_string("mult-assign");
  yy_destructor(83,&yymsp[0].minor);
}
// 4553 "parser.c"
        break;
      case 175:
// 1707 "parser.lemon"
{
	yygotominor.yy295 = json_object_new_string("div-assign");
  yy_destructor(84,&yymsp[0].minor);
}
// 4561 "parser.c"
        break;
      case 176:
// 1711 "parser.lemon"
{
	yygotominor.yy295 = json_object_new_string("concat-assign");
  yy_destructor(85,&yymsp[0].minor);
}
// 4569 "parser.c"
        break;
      case 177:
// 1716 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_let_assignment("variable", yymsp[-1].minor.yy295, yymsp[-2].minor.yy0, NULL, NULL, yymsp[0].minor.yy295, status->scanner_state);
}
// 4576 "parser.c"
        break;
      case 178:
// 1721 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_let_assignment("object-property", yymsp[-1].minor.yy295, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, yymsp[0].minor.yy295, status->scanner_state);
  yy_destructor(52,&yymsp[-3].minor);
}
// 4584 "parser.c"
        break;
      case 179:
// 1726 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_let_assignment("variable-dynamic-object-property", yymsp[-1].minor.yy295, yymsp[-6].minor.yy0, yymsp[-3].minor.yy0, NULL, yymsp[0].minor.yy295, status->scanner_state);
  yy_destructor(52,&yymsp[-5].minor);
  yy_destructor(44,&yymsp[-4].minor);
  yy_destructor(45,&yymsp[-2].minor);
}
// 4594 "parser.c"
        break;
      case 180:
// 1731 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_let_assignment("string-dynamic-object-property", yymsp[-1].minor.yy295, yymsp[-6].minor.yy0, yymsp[-3].minor.yy0, NULL, yymsp[0].minor.yy295, status->scanner_state);
  yy_destructor(52,&yymsp[-5].minor);
  yy_destructor(44,&yymsp[-4].minor);
  yy_destructor(45,&yymsp[-2].minor);
}
// 4604 "parser.c"
        break;
      case 181:
// 1736 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_let_assignment("object-property-append", yymsp[-1].minor.yy295, yymsp[-6].minor.yy0, yymsp[-4].minor.yy0, NULL, yymsp[0].minor.yy295, status->scanner_state);
  yy_destructor(52,&yymsp[-5].minor);
  yy_destructor(87,&yymsp[-3].minor);
  yy_destructor(88,&yymsp[-2].minor);
}
// 4614 "parser.c"
        break;
      case 182:
// 1741 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_let_assignment("object-property-array-index", yymsp[-1].minor.yy295, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, yymsp[-3].minor.yy295, yymsp[0].minor.yy295, status->scanner_state);
  yy_destructor(52,&yymsp[-6].minor);
  yy_destructor(87,&yymsp[-4].minor);
  yy_destructor(88,&yymsp[-2].minor);
}
// 4624 "parser.c"
        break;
      case 183:
// 1746 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_let_assignment("static-property", yymsp[-1].minor.yy295, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, yymsp[0].minor.yy295, status->scanner_state);
  yy_destructor(89,&yymsp[-3].minor);
}
// 4632 "parser.c"
        break;
      case 184:
// 1751 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_let_assignment("static-property-append", yymsp[-1].minor.yy295, yymsp[-6].minor.yy0, yymsp[-4].minor.yy0, NULL, yymsp[0].minor.yy295, status->scanner_state);
  yy_destructor(89,&yymsp[-5].minor);
  yy_destructor(87,&yymsp[-3].minor);
  yy_destructor(88,&yymsp[-2].minor);
}
// 4642 "parser.c"
        break;
      case 185:
// 1756 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_let_assignment("static-property-array-index", yymsp[-1].minor.yy295, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, yymsp[-3].minor.yy295, yymsp[0].minor.yy295, status->scanner_state);
  yy_destructor(89,&yymsp[-6].minor);
  yy_destructor(87,&yymsp[-4].minor);
  yy_destructor(88,&yymsp[-2].minor);
}
// 4652 "parser.c"
        break;
      case 186:
// 1761 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_let_assignment("variable-append", yymsp[-1].minor.yy295, yymsp[-4].minor.yy0, NULL, NULL, yymsp[0].minor.yy295, status->scanner_state);
  yy_destructor(87,&yymsp[-3].minor);
  yy_destructor(88,&yymsp[-2].minor);
}
// 4661 "parser.c"
        break;
      case 187:
// 1766 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_let_assignment("array-index", yymsp[-1].minor.yy295, yymsp[-5].minor.yy0, NULL, yymsp[-3].minor.yy295, yymsp[0].minor.yy295, status->scanner_state);
  yy_destructor(87,&yymsp[-4].minor);
  yy_destructor(88,&yymsp[-2].minor);
}
// 4670 "parser.c"
        break;
      case 188:
// 1771 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_let_assignment("incr", NULL, yymsp[-3].minor.yy0, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(52,&yymsp[-2].minor);
  yy_destructor(90,&yymsp[0].minor);
}
// 4679 "parser.c"
        break;
      case 189:
// 1776 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_let_assignment("decr", NULL, yymsp[-3].minor.yy0, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(52,&yymsp[-2].minor);
  yy_destructor(91,&yymsp[0].minor);
}
// 4688 "parser.c"
        break;
      case 190:
// 1781 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_let_assignment("incr", NULL, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(90,&yymsp[0].minor);
}
// 4696 "parser.c"
        break;
      case 191:
// 1786 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_let_assignment("decr", NULL, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(91,&yymsp[0].minor);
}
// 4704 "parser.c"
        break;
      case 193:
// 1794 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_echo_statement(yymsp[-1].minor.yy295, status->scanner_state);
  yy_destructor(92,&yymsp[-2].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 4713 "parser.c"
        break;
      case 196:
// 1806 "parser.lemon"
{
	yygotominor.yy295 = yymsp[0].minor.yy295;;
}
// 4720 "parser.c"
        break;
      case 197:
// 1811 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_mcall_statement(yymsp[-1].minor.yy295, status->scanner_state);
  yy_destructor(37,&yymsp[0].minor);
}
// 4728 "parser.c"
        break;
      case 198:
// 1816 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_fcall_statement(yymsp[-1].minor.yy295, status->scanner_state);
  yy_destructor(37,&yymsp[0].minor);
}
// 4736 "parser.c"
        break;
      case 199:
// 1821 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_scall_statement(yymsp[-1].minor.yy295, status->scanner_state);
  yy_destructor(37,&yymsp[0].minor);
}
// 4744 "parser.c"
        break;
      case 200:
// 1826 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_fetch_statement(yymsp[-1].minor.yy295, status->scanner_state);
  yy_destructor(37,&yymsp[0].minor);
}
// 4752 "parser.c"
        break;
      case 201:
// 1831 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_return_statement(yymsp[-1].minor.yy295, status->scanner_state);
  yy_destructor(93,&yymsp[-2].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 4761 "parser.c"
        break;
      case 202:
// 1836 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_return_statement(NULL, status->scanner_state);
  yy_destructor(93,&yymsp[-1].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 4770 "parser.c"
        break;
      case 203:
// 1841 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(94,&yymsp[-4].minor);
  yy_destructor(52,&yymsp[-2].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 4784 "parser.c"
        break;
      case 204:
// 1850 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state),
		yymsp[-2].minor.yy295,
		status->scanner_state
	);
  yy_destructor(94,&yymsp[-5].minor);
  yy_destructor(87,&yymsp[-3].minor);
  yy_destructor(88,&yymsp[-1].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 4799 "parser.c"
        break;
      case 205:
// 1859 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_throw_exception(yymsp[-1].minor.yy295, status->scanner_state);
  yy_destructor(95,&yymsp[-2].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 4808 "parser.c"
        break;
      case 206:
// 1863 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_declare_statement(XX_T_TYPE_INTEGER, yymsp[-1].minor.yy295, status->scanner_state);
  yy_destructor(55,&yymsp[-2].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 4817 "parser.c"
        break;
      case 207:
// 1867 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_declare_statement(XX_T_TYPE_UINTEGER, yymsp[-1].minor.yy295, status->scanner_state);
  yy_destructor(56,&yymsp[-2].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 4826 "parser.c"
        break;
      case 208:
// 1871 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_declare_statement(XX_T_TYPE_CHAR, yymsp[-1].minor.yy295, status->scanner_state);
  yy_destructor(59,&yymsp[-2].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 4835 "parser.c"
        break;
      case 209:
// 1875 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_declare_statement(XX_T_TYPE_UCHAR, yymsp[-1].minor.yy295, status->scanner_state);
  yy_destructor(60,&yymsp[-2].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 4844 "parser.c"
        break;
      case 210:
// 1879 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_declare_statement(XX_T_TYPE_LONG, yymsp[-1].minor.yy295, status->scanner_state);
  yy_destructor(57,&yymsp[-2].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 4853 "parser.c"
        break;
      case 211:
// 1883 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_declare_statement(XX_T_TYPE_ULONG, yymsp[-1].minor.yy295, status->scanner_state);
  yy_destructor(58,&yymsp[-2].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 4862 "parser.c"
        break;
      case 212:
// 1887 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_declare_statement(XX_T_TYPE_DOUBLE, yymsp[-1].minor.yy295, status->scanner_state);
  yy_destructor(61,&yymsp[-2].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 4871 "parser.c"
        break;
      case 213:
// 1891 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_declare_statement(XX_T_TYPE_STRING, yymsp[-1].minor.yy295, status->scanner_state);
  yy_destructor(63,&yymsp[-2].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 4880 "parser.c"
        break;
      case 214:
// 1895 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_declare_statement(XX_T_TYPE_BOOL, yymsp[-1].minor.yy295, status->scanner_state);
  yy_destructor(62,&yymsp[-2].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 4889 "parser.c"
        break;
      case 215:
// 1899 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_declare_statement(XX_T_TYPE_VAR, yymsp[-1].minor.yy295, status->scanner_state);
  yy_destructor(65,&yymsp[-2].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 4898 "parser.c"
        break;
      case 218:
// 1911 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_declare_variable(yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 4905 "parser.c"
        break;
      case 219:
// 1915 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_declare_variable(yymsp[-2].minor.yy0, yymsp[0].minor.yy295, status->scanner_state);
  yy_destructor(47,&yymsp[-1].minor);
}
// 4913 "parser.c"
        break;
      case 221:
// 1923 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_expr("not", yymsp[0].minor.yy295, NULL, NULL, status->scanner_state);
  yy_destructor(33,&yymsp[-1].minor);
}
// 4921 "parser.c"
        break;
      case 222:
// 1927 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_expr("isset", yymsp[0].minor.yy295, NULL, NULL, status->scanner_state);
  yy_destructor(29,&yymsp[-1].minor);
}
// 4929 "parser.c"
        break;
      case 223:
// 1931 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_expr("require", yymsp[0].minor.yy295, NULL, NULL, status->scanner_state);
  yy_destructor(7,&yymsp[-1].minor);
}
// 4937 "parser.c"
        break;
      case 224:
// 1935 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_expr("clone", yymsp[0].minor.yy295, NULL, NULL, status->scanner_state);
  yy_destructor(10,&yymsp[-1].minor);
}
// 4945 "parser.c"
        break;
      case 225:
// 1939 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_expr("empty", yymsp[0].minor.yy295, NULL, NULL, status->scanner_state);
  yy_destructor(11,&yymsp[-1].minor);
}
// 4953 "parser.c"
        break;
      case 226:
// 1943 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_expr("likely", yymsp[0].minor.yy295, NULL, NULL, status->scanner_state);
  yy_destructor(8,&yymsp[-1].minor);
}
// 4961 "parser.c"
        break;
      case 227:
// 1947 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_expr("unlikely", yymsp[0].minor.yy295, NULL, NULL, status->scanner_state);
  yy_destructor(9,&yymsp[-1].minor);
}
// 4969 "parser.c"
        break;
      case 228:
// 1951 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_expr("equals", yymsp[-2].minor.yy295, yymsp[0].minor.yy295, NULL, status->scanner_state);
  yy_destructor(15,&yymsp[-1].minor);
}
// 4977 "parser.c"
        break;
      case 229:
// 1955 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_expr("not-equals", yymsp[-2].minor.yy295, yymsp[0].minor.yy295, NULL, status->scanner_state);
  yy_destructor(22,&yymsp[-1].minor);
}
// 4985 "parser.c"
        break;
      case 230:
// 1959 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_expr("identical", yymsp[-2].minor.yy295, yymsp[0].minor.yy295, NULL, status->scanner_state);
  yy_destructor(16,&yymsp[-1].minor);
}
// 4993 "parser.c"
        break;
      case 231:
// 1963 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_expr("not-identical", yymsp[-2].minor.yy295, yymsp[0].minor.yy295, NULL, status->scanner_state);
  yy_destructor(21,&yymsp[-1].minor);
}
// 5001 "parser.c"
        break;
      case 232:
// 1967 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_expr("less", yymsp[-2].minor.yy295, yymsp[0].minor.yy295, NULL, status->scanner_state);
  yy_destructor(17,&yymsp[-1].minor);
}
// 5009 "parser.c"
        break;
      case 233:
// 1971 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_expr("greater", yymsp[-2].minor.yy295, yymsp[0].minor.yy295, NULL, status->scanner_state);
  yy_destructor(18,&yymsp[-1].minor);
}
// 5017 "parser.c"
        break;
      case 234:
// 1975 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_expr("less-equal", yymsp[-2].minor.yy295, yymsp[0].minor.yy295, NULL, status->scanner_state);
  yy_destructor(19,&yymsp[-1].minor);
}
// 5025 "parser.c"
        break;
      case 235:
// 1979 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_expr("greater-equal", yymsp[-2].minor.yy295, yymsp[0].minor.yy295, NULL, status->scanner_state);
  yy_destructor(20,&yymsp[-1].minor);
}
// 5033 "parser.c"
        break;
      case 236:
// 1983 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_expr("list", yymsp[-1].minor.yy295, NULL, NULL, status->scanner_state);
  yy_destructor(51,&yymsp[-2].minor);
  yy_destructor(34,&yymsp[0].minor);
}
// 5042 "parser.c"
        break;
      case 237:
// 1987 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_expr("cast", yymsp[-2].minor.yy295, yymsp[0].minor.yy295, NULL, status->scanner_state);
  yy_destructor(51,&yymsp[-3].minor);
  yy_destructor(34,&yymsp[-1].minor);
}
// 5051 "parser.c"
        break;
      case 238:
// 1991 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_expr("type-hint", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), yymsp[0].minor.yy295, NULL, status->scanner_state);
  yy_destructor(17,&yymsp[-3].minor);
  yy_destructor(18,&yymsp[-1].minor);
}
// 5060 "parser.c"
        break;
      case 239:
      case 313:
// 1995 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_expr("property-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(52,&yymsp[-1].minor);
}
// 5069 "parser.c"
        break;
      case 240:
      case 314:
// 1999 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_expr("property-dynamic-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(52,&yymsp[-3].minor);
  yy_destructor(44,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 5080 "parser.c"
        break;
      case 241:
      case 315:
// 2003 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_expr("property-string-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_STRING, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(52,&yymsp[-3].minor);
  yy_destructor(44,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 5091 "parser.c"
        break;
      case 242:
// 2007 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_expr("static-property-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(89,&yymsp[-1].minor);
}
// 5099 "parser.c"
        break;
      case 243:
      case 311:
// 2011 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_expr("static-constant-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(89,&yymsp[-1].minor);
}
// 5108 "parser.c"
        break;
      case 244:
      case 312:
// 2016 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), yymsp[-1].minor.yy295, NULL, status->scanner_state);
  yy_destructor(87,&yymsp[-2].minor);
  yy_destructor(88,&yymsp[0].minor);
}
// 5118 "parser.c"
        break;
      case 245:
// 2021 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_expr("add", yymsp[-2].minor.yy295, yymsp[0].minor.yy295, NULL, status->scanner_state);
  yy_destructor(23,&yymsp[-1].minor);
}
// 5126 "parser.c"
        break;
      case 246:
// 2026 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_expr("sub", yymsp[-2].minor.yy295, yymsp[0].minor.yy295, NULL, status->scanner_state);
  yy_destructor(24,&yymsp[-1].minor);
}
// 5134 "parser.c"
        break;
      case 247:
// 2031 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_expr("mul", yymsp[-2].minor.yy295, yymsp[0].minor.yy295, NULL, status->scanner_state);
  yy_destructor(26,&yymsp[-1].minor);
}
// 5142 "parser.c"
        break;
      case 248:
// 2036 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_expr("div", yymsp[-2].minor.yy295, yymsp[0].minor.yy295, NULL, status->scanner_state);
  yy_destructor(27,&yymsp[-1].minor);
}
// 5150 "parser.c"
        break;
      case 249:
// 2041 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_expr("mod", yymsp[-2].minor.yy295, yymsp[0].minor.yy295, NULL, status->scanner_state);
  yy_destructor(28,&yymsp[-1].minor);
}
// 5158 "parser.c"
        break;
      case 250:
// 2046 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_expr("concat", yymsp[-2].minor.yy295, yymsp[0].minor.yy295, NULL, status->scanner_state);
  yy_destructor(25,&yymsp[-1].minor);
}
// 5166 "parser.c"
        break;
      case 251:
// 2051 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_expr("and", yymsp[-2].minor.yy295, yymsp[0].minor.yy295, NULL, status->scanner_state);
  yy_destructor(13,&yymsp[-1].minor);
}
// 5174 "parser.c"
        break;
      case 252:
// 2056 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_expr("or", yymsp[-2].minor.yy295, yymsp[0].minor.yy295, NULL, status->scanner_state);
  yy_destructor(14,&yymsp[-1].minor);
}
// 5182 "parser.c"
        break;
      case 253:
// 2061 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_expr("instanceof", yymsp[-2].minor.yy295, yymsp[0].minor.yy295, NULL, status->scanner_state);
  yy_destructor(12,&yymsp[-1].minor);
}
// 5190 "parser.c"
        break;
      case 254:
// 2066 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_expr("fetch", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), yymsp[0].minor.yy295, NULL, status->scanner_state);
  yy_destructor(30,&yymsp[-3].minor);
  yy_destructor(6,&yymsp[-1].minor);
}
// 5199 "parser.c"
        break;
      case 256:
// 2076 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_expr("typeof", yymsp[0].minor.yy295, NULL, NULL, status->scanner_state);
  yy_destructor(31,&yymsp[-1].minor);
}
// 5207 "parser.c"
        break;
      case 257:
// 2081 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_literal(XX_T_CONSTANT, yymsp[0].minor.yy0, status->scanner_state);
}
// 5214 "parser.c"
        break;
      case 259:
      case 302:
      case 304:
// 2091 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_literal(XX_T_INTEGER, yymsp[0].minor.yy0, status->scanner_state);
}
// 5223 "parser.c"
        break;
      case 260:
      case 301:
      case 306:
// 2096 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_literal(XX_T_STRING, yymsp[0].minor.yy0, status->scanner_state);
}
// 5232 "parser.c"
        break;
      case 261:
      case 305:
// 2101 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_literal(XX_T_CHAR, yymsp[0].minor.yy0, status->scanner_state);
}
// 5240 "parser.c"
        break;
      case 262:
      case 307:
// 2106 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_literal(XX_T_DOUBLE, yymsp[0].minor.yy0, status->scanner_state);
}
// 5248 "parser.c"
        break;
      case 263:
      case 308:
// 2111 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_literal(XX_T_NULL, NULL, status->scanner_state);
  yy_destructor(99,&yymsp[0].minor);
}
// 5257 "parser.c"
        break;
      case 264:
      case 310:
// 2116 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_literal(XX_T_TRUE, NULL, status->scanner_state);
  yy_destructor(100,&yymsp[0].minor);
}
// 5266 "parser.c"
        break;
      case 265:
      case 309:
// 2121 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_literal(XX_T_FALSE, NULL, status->scanner_state);
  yy_destructor(101,&yymsp[0].minor);
}
// 5275 "parser.c"
        break;
      case 266:
// 2126 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_expr("empty-array", NULL, NULL, NULL, status->scanner_state);
  yy_destructor(87,&yymsp[-1].minor);
  yy_destructor(88,&yymsp[0].minor);
}
// 5284 "parser.c"
        break;
      case 267:
// 2131 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_expr("array", yymsp[-1].minor.yy295, NULL, NULL, status->scanner_state);
  yy_destructor(87,&yymsp[-2].minor);
  yy_destructor(88,&yymsp[0].minor);
}
// 5293 "parser.c"
        break;
      case 268:
// 2136 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_new_instance(0, yymsp[0].minor.yy0, NULL, status->scanner_state);
  yy_destructor(32,&yymsp[-1].minor);
}
// 5301 "parser.c"
        break;
      case 269:
// 2141 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_new_instance(0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(32,&yymsp[-3].minor);
  yy_destructor(51,&yymsp[-1].minor);
  yy_destructor(34,&yymsp[0].minor);
}
// 5311 "parser.c"
        break;
      case 270:
// 2146 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_new_instance(0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy295, status->scanner_state);
  yy_destructor(32,&yymsp[-4].minor);
  yy_destructor(51,&yymsp[-2].minor);
  yy_destructor(34,&yymsp[0].minor);
}
// 5321 "parser.c"
        break;
      case 271:
// 2151 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_new_instance(1, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(32,&yymsp[-3].minor);
  yy_destructor(44,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 5331 "parser.c"
        break;
      case 272:
// 2156 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_new_instance(1, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(32,&yymsp[-5].minor);
  yy_destructor(44,&yymsp[-4].minor);
  yy_destructor(45,&yymsp[-2].minor);
  yy_destructor(51,&yymsp[-1].minor);
  yy_destructor(34,&yymsp[0].minor);
}
// 5343 "parser.c"
        break;
      case 273:
// 2161 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_new_instance(1, yymsp[-4].minor.yy0, yymsp[-1].minor.yy295, status->scanner_state);
  yy_destructor(32,&yymsp[-6].minor);
  yy_destructor(44,&yymsp[-5].minor);
  yy_destructor(45,&yymsp[-3].minor);
  yy_destructor(51,&yymsp[-2].minor);
  yy_destructor(34,&yymsp[0].minor);
}
// 5355 "parser.c"
        break;
      case 274:
// 2166 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_fcall(1, yymsp[-3].minor.yy0, yymsp[-1].minor.yy295, status->scanner_state);
  yy_destructor(51,&yymsp[-2].minor);
  yy_destructor(34,&yymsp[0].minor);
}
// 5364 "parser.c"
        break;
      case 275:
// 2171 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_fcall(1, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(51,&yymsp[-1].minor);
  yy_destructor(34,&yymsp[0].minor);
}
// 5373 "parser.c"
        break;
      case 276:
// 2176 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_fcall(2, yymsp[-4].minor.yy0, yymsp[-1].minor.yy295, status->scanner_state);
  yy_destructor(44,&yymsp[-5].minor);
  yy_destructor(45,&yymsp[-3].minor);
  yy_destructor(51,&yymsp[-2].minor);
  yy_destructor(34,&yymsp[0].minor);
}
// 5384 "parser.c"
        break;
      case 277:
// 2181 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_fcall(2, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(44,&yymsp[-4].minor);
  yy_destructor(45,&yymsp[-2].minor);
  yy_destructor(51,&yymsp[-1].minor);
  yy_destructor(34,&yymsp[0].minor);
}
// 5395 "parser.c"
        break;
      case 278:
// 2186 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_scall(0, yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy295, status->scanner_state);
  yy_destructor(89,&yymsp[-4].minor);
  yy_destructor(51,&yymsp[-2].minor);
  yy_destructor(34,&yymsp[0].minor);
}
// 5405 "parser.c"
        break;
      case 279:
// 2191 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_scall(0, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(89,&yymsp[-3].minor);
  yy_destructor(51,&yymsp[-1].minor);
  yy_destructor(34,&yymsp[0].minor);
}
// 5415 "parser.c"
        break;
      case 280:
// 2196 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_scall(1, yymsp[-6].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy295, status->scanner_state);
  yy_destructor(44,&yymsp[-7].minor);
  yy_destructor(45,&yymsp[-5].minor);
  yy_destructor(89,&yymsp[-4].minor);
  yy_destructor(51,&yymsp[-2].minor);
  yy_destructor(34,&yymsp[0].minor);
}
// 5427 "parser.c"
        break;
      case 281:
// 2201 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_scall(1, yymsp[-5].minor.yy0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(44,&yymsp[-6].minor);
  yy_destructor(45,&yymsp[-4].minor);
  yy_destructor(89,&yymsp[-3].minor);
  yy_destructor(51,&yymsp[-1].minor);
  yy_destructor(34,&yymsp[0].minor);
}
// 5439 "parser.c"
        break;
      case 282:
// 2206 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_scall(1, yymsp[-8].minor.yy0, yymsp[-4].minor.yy0, yymsp[-1].minor.yy295, status->scanner_state);
  yy_destructor(44,&yymsp[-9].minor);
  yy_destructor(45,&yymsp[-7].minor);
  yy_destructor(89,&yymsp[-6].minor);
  yy_destructor(44,&yymsp[-5].minor);
  yy_destructor(45,&yymsp[-3].minor);
  yy_destructor(51,&yymsp[-2].minor);
  yy_destructor(34,&yymsp[0].minor);
}
// 5453 "parser.c"
        break;
      case 283:
// 2211 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_scall(1, yymsp[-7].minor.yy0, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(44,&yymsp[-8].minor);
  yy_destructor(45,&yymsp[-6].minor);
  yy_destructor(89,&yymsp[-5].minor);
  yy_destructor(44,&yymsp[-4].minor);
  yy_destructor(45,&yymsp[-2].minor);
  yy_destructor(51,&yymsp[-1].minor);
  yy_destructor(34,&yymsp[0].minor);
}
// 5467 "parser.c"
        break;
      case 284:
// 2216 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_mcall(1, yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy295, status->scanner_state);
  yy_destructor(52,&yymsp[-4].minor);
  yy_destructor(51,&yymsp[-2].minor);
  yy_destructor(34,&yymsp[0].minor);
}
// 5477 "parser.c"
        break;
      case 285:
// 2221 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_mcall(1, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(52,&yymsp[-3].minor);
  yy_destructor(51,&yymsp[-1].minor);
  yy_destructor(34,&yymsp[0].minor);
}
// 5487 "parser.c"
        break;
      case 286:
// 2226 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_mcall(2, yymsp[-7].minor.yy0, yymsp[-4].minor.yy0, yymsp[-1].minor.yy295, status->scanner_state);
  yy_destructor(52,&yymsp[-6].minor);
  yy_destructor(44,&yymsp[-5].minor);
  yy_destructor(45,&yymsp[-3].minor);
  yy_destructor(51,&yymsp[-2].minor);
  yy_destructor(34,&yymsp[0].minor);
}
// 5499 "parser.c"
        break;
      case 287:
// 2231 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_mcall(2, yymsp[-6].minor.yy0, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(52,&yymsp[-5].minor);
  yy_destructor(44,&yymsp[-4].minor);
  yy_destructor(45,&yymsp[-2].minor);
  yy_destructor(51,&yymsp[-1].minor);
  yy_destructor(34,&yymsp[0].minor);
}
// 5511 "parser.c"
        break;
      case 288:
// 2236 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_mcall(3, yymsp[-7].minor.yy0, yymsp[-4].minor.yy0, yymsp[-1].minor.yy295, status->scanner_state);
  yy_destructor(52,&yymsp[-6].minor);
  yy_destructor(44,&yymsp[-5].minor);
  yy_destructor(45,&yymsp[-3].minor);
  yy_destructor(51,&yymsp[-2].minor);
  yy_destructor(34,&yymsp[0].minor);
}
// 5523 "parser.c"
        break;
      case 289:
// 2241 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_mcall(3, yymsp[-6].minor.yy0, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(52,&yymsp[-5].minor);
  yy_destructor(44,&yymsp[-4].minor);
  yy_destructor(45,&yymsp[-2].minor);
  yy_destructor(51,&yymsp[-1].minor);
  yy_destructor(34,&yymsp[0].minor);
}
// 5535 "parser.c"
        break;
      case 298:
// 2280 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_array_item(yymsp[-2].minor.yy295, yymsp[0].minor.yy295, status->scanner_state);
  yy_destructor(72,&yymsp[-1].minor);
}
// 5543 "parser.c"
        break;
      case 299:
// 2284 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_array_item(NULL, yymsp[0].minor.yy295, status->scanner_state);
}
// 5550 "parser.c"
        break;
      case 317:
// 2360 "parser.lemon"
{
	yygotominor.yy295 = xx_ret_comment(yymsp[0].minor.yy0, status->scanner_state);
}
// 5557 "parser.c"
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
// 948 "parser.lemon"


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

// 5633 "parser.c"
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
