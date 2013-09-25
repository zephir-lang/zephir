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


// 861 "parser.c"
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
#define YYNOCODE 176
#define YYACTIONTYPE unsigned short int
#define xx_TOKENTYPE xx_parser_token*
typedef union {
  xx_TOKENTYPE yy0;
  json_object* yy175;
  int yy351;
} YYMINORTYPE;
#define YYSTACKDEPTH 100
#define xx_ARG_SDECL xx_parser_status *status;
#define xx_ARG_PDECL ,xx_parser_status *status
#define xx_ARG_FETCH xx_parser_status *status = yypParser->status
#define xx_ARG_STORE yypParser->status = status
#define YYNSTATE 635
#define YYNRULE 298
#define YYERRORSYMBOL 99
#define YYERRSYMDT yy351
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
 /*     0 */   197,  200,  203,    3,    4,    5,    6,    7,  239,  934,
 /*    10 */     1,    2,  634,    4,    5,    6,    7,   68,   72,   54,
 /*    20 */   193,  248,  252,  268,  190,  115,  472,  243,  117,  603,
 /*    30 */   609,  608,  592,  120,  313,  275,   14,  602,  558,  119,
 /*    40 */   254,   17,  233,  102,  102,   35,   36,   37,   38,   39,
 /*    50 */    40,   41,   42,   43,   44,   45,  231,  227,  229,  192,
 /*    60 */   202,  207,  209,  211,  213,  205,  199,  215,  217,  225,
 /*    70 */   219,  221,  223,  586,  515,  610,  256,  262,   68,   72,
 /*    80 */    76,  431,  581,  587,  468,  149,  255,  257,  258,  259,
 /*    90 */   260,  261,  275,  536,  473,   47,  219,  221,  223,   68,
 /*   100 */    72,   76,  432,  441,  450,  453,  444,  447,  456,  462,
 /*   110 */   459,  113,  465,  150,  152,  154,  522,  163,   30,   31,
 /*   120 */   528,  174,  178,  183,  334,  468,  149,  341,  355,  466,
 /*   130 */   440,  102,  363,  275,  340,  473,  282,  284,  283,  403,
 /*   140 */   416,  420,  429,  432,  441,  450,  453,  444,  447,  456,
 /*   150 */   462,  459,  251,  465,  150,  152,  154,  604,  163,  607,
 /*   160 */   608,  592,  174,  178,  183,  334,  557,  558,  341,  468,
 /*   170 */   149,  422,  102,  494,  463,  440,  333,  275,  162,  473,
 /*   180 */   403,  416,  420,  429,  282,  284,  283,  432,  441,  450,
 /*   190 */   453,  444,  447,  456,  462,  459,  346,  465,  150,  152,
 /*   200 */   154,   99,  163,   30,   31,  425,  174,  178,  183,  334,
 /*   210 */   294,  244,  341,  468,  149,  215,  217,  225,  219,  221,
 /*   220 */   223,  275,  534,  473,  403,  416,  420,  429,  274,  112,
 /*   230 */   917,  432,  441,  450,  453,  444,  447,  456,  462,  459,
 /*   240 */    52,  465,  150,  152,  154,  291,  163,  265,  460,  440,
 /*   250 */   174,  178,  183,  334,  280,   49,  341,  468,  149,  331,
 /*   260 */   435,  188,  294,  471,  333,  275,  520,  473,  403,  416,
 /*   270 */   420,  429,  282,  284,  283,  432,  441,  450,  453,  444,
 /*   280 */   447,  456,  462,  459,  517,  465,  150,  152,  154,  467,
 /*   290 */   163,  166,  521,  586,  174,  178,  183,  334,  408,  469,
 /*   300 */   341,  468,  149,  587,  404,  409,  282,  284,  283,  275,
 /*   310 */   507,  473,  403,  416,  420,  429,  170,   87,  498,  432,
 /*   320 */   441,  450,  453,  444,  447,  456,  462,  459,  102,  465,
 /*   330 */   150,  152,  154,  618,  163,  556,   53,  120,  174,  178,
 /*   340 */   183,  334,  557,  558,  341,  468,  149,  266,  102,   50,
 /*   350 */   572,   30,   31,  275,  518,  473,  403,  416,  420,  429,
 /*   360 */   290,  102,  516,  432,  441,  450,  453,  444,  447,  456,
 /*   370 */   462,  459,  101,  465,  150,  152,  154,  110,  163,  168,
 /*   380 */   113,  303,  174,  178,  183,  334,  368,   98,  341,  468,
 /*   390 */   149,  396,  435,   75,  282,  284,  283,  275,  512,  473,
 /*   400 */   403,  416,  420,  429,  170,  236,  498,  432,  441,  450,
 /*   410 */   453,  444,  447,  456,  462,  459,  314,  465,  150,  152,
 /*   420 */   154,  464,  163,  605,  570,  592,  174,  178,  183,  334,
 /*   430 */   234,  558,  341,  468,  149,  111,  102,  613,  282,  284,
 /*   440 */   283,  275,  479,  473,  403,  416,  420,  429,  457,  440,
 /*   450 */   437,  432,  441,  450,  453,  444,  447,  456,  462,  459,
 /*   460 */   302,  465,  150,  152,  154,  320,  163,  584,   53,  250,
 /*   470 */   174,  178,  183,  334,  294,  244,  341,  468,  149,  107,
 /*   480 */    28,  614,   30,   31,  113,  275,  542,  473,  403,  416,
 /*   490 */   420,  429,  167,  501,   48,  432,  441,  450,  453,  444,
 /*   500 */   447,  456,  462,  459,  583,  465,  150,  152,  154,  291,
 /*   510 */   163,  265,  274,   71,  174,  178,  183,  334,  574,  435,
 /*   520 */   341,  468,  149,  480,  550,  580,   30,   31,  575,  275,
 /*   530 */   514,  473,  403,  416,  420,  429,   53,  551,  288,  432,
 /*   540 */   441,  450,  453,  444,  447,  456,  462,  459,  458,  465,
 /*   550 */   150,  152,  154,  435,  163,  454,  440,  919,  174,  178,
 /*   560 */   183,  334,   97,   96,  341,  468,  149,   68,   72,   76,
 /*   570 */   435,  342,  402,  275,  540,  473,  403,  416,  420,  429,
 /*   580 */   451,  440,  461,  432,  441,  450,  453,  444,  447,  456,
 /*   590 */   462,  459,  336,  465,  150,  152,  154,  435,  163,  455,
 /*   600 */    22,  372,  174,  178,  183,  334,  577,   21,  341,  468,
 /*   610 */   149,  102,   93,  580,   30,   31,  533,  275,  508,  473,
 /*   620 */   403,  416,  420,  429,  537,  499,  452,  432,  441,  450,
 /*   630 */   453,  444,  447,  456,  462,  459,  249,  465,  150,  152,
 /*   640 */   154,  435,  163,  448,  440,  171,  174,  178,  183,  334,
 /*   650 */   376,  435,  341,  468,  149,  621,  538,  624,   30,   31,
 /*   660 */   113,  275,  502,  473,  403,  416,  420,  429,   53,  918,
 /*   670 */   449,  432,  441,  450,  453,  444,  447,  456,  462,  459,
 /*   680 */   446,  465,  150,  152,  154,  619,  163,   77,  274,   95,
 /*   690 */   174,  178,  183,  334,  544,  543,  341,  468,  149,   68,
 /*   700 */    72,   76,  445,  440,   92,  275,  496,  473,  403,  416,
 /*   710 */   420,  429,  442,  440,  296,  432,  441,  450,  453,  444,
 /*   720 */   447,  456,  462,  459,  172,  465,  150,  152,  154,  510,
 /*   730 */   163,   30,   31,   13,  174,  178,  183,  334,  545,  435,
 /*   740 */   341,  468,  149,  615,  435,  613,  549,  439,  245,  275,
 /*   750 */   526,  473,  403,  416,  420,  429,  313,  582,  589,  432,
 /*   760 */   441,  450,  453,  444,  447,  456,  462,  459,  443,  465,
 /*   770 */   150,  152,  154,  434,  163,  433,  440,   46,  174,  178,
 /*   780 */   183,  334,  597,  406,  341,  468,  149,  330,  625,  580,
 /*   790 */   613,  274,   27,  275,  524,  473,  403,  416,  420,  429,
 /*   800 */   274,  438,  344,  432,  441,  450,  453,  444,  447,  456,
 /*   810 */   462,  459,  405,  465,  150,  152,  154,  311,  163,  274,
 /*   820 */   267,  196,  174,  178,  183,  334,  318,  357,  341,  468,
 /*   830 */   149,  343,  246,  276,  436,  371,  620,  275,  126,  473,
 /*   840 */   403,  416,  420,  429,  122,  324,   90,  432,  441,  450,
 /*   850 */   453,  444,  447,  456,  462,  459,  125,  465,  150,  152,
 /*   860 */   154,  562,  163,  274,  509,  631,  174,  178,  183,  334,
 /*   870 */   113,  561,  341,  468,  149,  353,  622,  563,  564,  175,
 /*   880 */   635,  275,  497,  473,  403,  416,  420,  429,  566,  273,
 /*   890 */   306,  432,  441,  450,  453,  444,  447,  456,  462,  459,
 /*   900 */   567,  465,  150,  152,  154,   89,  163,  241,  568,  569,
 /*   910 */   174,  178,  183,  334,  428,  240,  341,  468,  149,  424,
 /*   920 */   423,  623,  590,  421,  113,  275,  182,  473,  403,  416,
 /*   930 */   420,  429,  505,  237,   16,  432,  441,  450,  453,  444,
 /*   940 */   447,  456,  462,  459,  576,  465,  150,  152,  154,  345,
 /*   950 */   163,  578,   29,  195,  174,  178,  183,  334,  579,  151,
 /*   960 */   341,  468,  149,  180,   10,  504,  612,  106,  681,  275,
 /*   970 */   177,  473,  403,  416,  420,  429,  415,  413,  682,  432,
 /*   980 */   441,  450,  453,  444,  447,  456,  462,  459,  585,  465,
 /*   990 */   150,  152,  154,   17,  163,  315,  626,  411,  174,  178,
 /*  1000 */   183,  334,  316,  588,  341,  468,  149,  116,  335,  156,
 /*  1010 */   628,  591,  194,  275,  492,  473,  403,  416,  420,  429,
 /*  1020 */   184,  398,  397,  432,  441,  450,  453,  444,  447,  456,
 /*  1030 */   462,  459,   25,  465,  150,  152,  154,  278,  163,  321,
 /*  1040 */   600,   57,  174,  178,  183,  334,  394,  596,  341,  468,
 /*  1050 */   149,  495,  390,  322,  598,  599,  481,  275,  486,  473,
 /*  1060 */   403,  416,  420,  429,  389,  386,  601,  432,  441,  450,
 /*  1070 */   453,  444,  447,  456,  462,  459,  629,  465,  150,  152,
 /*  1080 */   154,  187,  163,   69,  159,   34,  174,  178,  183,  334,
 /*  1090 */   153,  108,  341,  468,  149,  160,   74,  277,  382,  482,
 /*  1100 */   381,  275,  554,  473,  403,  416,  420,  429,  100,   70,
 /*  1110 */   377,  432,  441,  450,  453,  444,  447,  456,  462,  459,
 /*  1120 */   373,  465,  150,  152,  154,  511,  163,  523,  378,  269,
 /*  1130 */   174,  178,  183,  334,  109,  374,  341,  468,  149,   24,
 /*  1140 */    73,   67,  630,   33,  165,  275,  128,  473,  403,  416,
 /*  1150 */   420,  429,  189,  270,  530,  432,  441,  450,  453,  444,
 /*  1160 */   447,  456,  462,  459,  366,  465,  150,  152,  154,  365,
 /*  1170 */   163,  531,  470,  105,  174,  178,  183,  334,   66,  633,
 /*  1180 */   341,  468,  149,    9,   12,  362,  611,   15,  358,  275,
 /*  1190 */   186,  473,  403,  416,  420,  429,   94,  539,   51,  432,
 /*  1200 */   441,  450,  453,  444,  447,  456,  462,  459,  123,  465,
 /*  1210 */   150,  152,  154,   56,  163,  169,  493,  632,  174,  178,
 /*  1220 */   183,  334,  616,  475,  341,  468,  149,  475,  475,  475,
 /*  1230 */   475,  475,  475,  275,  491,  473,  403,  416,  420,  429,
 /*  1240 */   475,  475,  475,  432,  441,  450,  453,  444,  447,  456,
 /*  1250 */   462,  459,  475,  465,  150,  152,  154,  475,  163,  475,
 /*  1260 */   475,  475,  174,  178,  183,  334,  475,  475,  341,  468,
 /*  1270 */   149,  475,  475,  475,  475,  475,  475,  275,  552,  473,
 /*  1280 */   403,  416,  420,  429,  475,  475,  475,  432,  441,  450,
 /*  1290 */   453,  444,  447,  456,  462,  459,  475,  465,  150,  152,
 /*  1300 */   154,  475,  163,  475,  475,  475,  174,  178,  183,  334,
 /*  1310 */   475,  475,  341,  468,  149,  475,  475,  475,  475,  475,
 /*  1320 */   475,  275,  548,  473,  403,  416,  420,  429,  475,  475,
 /*  1330 */   475,  432,  441,  450,  453,  444,  447,  456,  462,  459,
 /*  1340 */   475,  465,  150,  152,  154,  475,  163,  475,  475,  475,
 /*  1350 */   174,  178,  183,  334,  475,  475,  341,  468,  149,  475,
 /*  1360 */   475,  475,  475,  475,  475,  275,  546,  473,  403,  416,
 /*  1370 */   420,  429,  475,  475,  475,  432,  441,  450,  453,  444,
 /*  1380 */   447,  456,  462,  459,  475,  465,  150,  152,  154,  475,
 /*  1390 */   163,  475,  475,  475,  174,  178,  183,  334,  475,  475,
 /*  1400 */   341,  468,  149,  475,  475,  475,  475,  475,  475,  275,
 /*  1410 */   158,  473,  403,  416,  420,  429,  475,  475,  475,  432,
 /*  1420 */   441,  450,  453,  444,  447,  456,  462,  459,  475,  465,
 /*  1430 */   150,  152,  154,  475,  163,  475,  475,  475,  174,  178,
 /*  1440 */   183,  334,  475,  475,  341,  468,  149,  475,  475,  475,
 /*  1450 */   475,  475,  475,  275,  503,  473,  403,  416,  420,  429,
 /*  1460 */   475,  475,  475,  432,  441,  450,  453,  444,  447,  456,
 /*  1470 */   462,  459,  475,  465,  150,  152,  154,  475,  163,  475,
 /*  1480 */   475,  475,  174,  178,  183,  334,  475,  475,  341,  468,
 /*  1490 */   149,   78,   79,   81,   80,   82,  475,  275,  475,  473,
 /*  1500 */   403,  416,  420,  429,  475,  475,  475,  432,  441,  450,
 /*  1510 */   453,  444,  447,  456,  462,  459,  475,  465,  150,  152,
 /*  1520 */   154,  475,  163,  147,  475,  475,  174,  178,  183,  334,
 /*  1530 */    84,   85,  341,  666,  559,  475,  565,  475,  475,  347,
 /*  1540 */   475,   83,  475,  478,  403,  416,  420,  429,  475,  474,
 /*  1550 */   130,  131,  132,  133,  134,  135,  136,  137,  138,  139,
 /*  1560 */   140,  141,  142,  143,  144,  145,  146,  148,  475,  475,
 /*  1570 */   475,  147,  348,  349,  350,  351,  352,  475,  384,  410,
 /*  1580 */   412,  414,   78,   79,   81,   80,   82,  475,  475,  475,
 /*  1590 */   475,  127,  475,  475,  475,  475,  475,  474,  130,  131,
 /*  1600 */   132,  133,  134,  135,  136,  137,  138,  139,  140,  141,
 /*  1610 */   142,  143,  144,  145,  146,  148,  147,  475,  475,  475,
 /*  1620 */   475,   84,   85,  475,   18,   86,  475,  410,  412,  414,
 /*  1630 */   347,  355,   83,  475,  475,  395,  181,  475,  475,  282,
 /*  1640 */   284,  283,  474,  130,  131,  132,  133,  134,  135,  136,
 /*  1650 */   137,  138,  139,  140,  141,  142,  143,  144,  145,  146,
 /*  1660 */   148,  147,  475,  348,  349,  350,  351,  352,  475,  475,
 /*  1670 */   475,  475,  410,  412,  414,   78,   79,   81,   80,   82,
 /*  1680 */   475,  157,  475,  475,  475,  475,  475,  474,  130,  131,
 /*  1690 */   132,  133,  134,  135,  136,  137,  138,  139,  140,  141,
 /*  1700 */   142,  143,  144,  145,  146,  148,  147,  475,  573,  475,
 /*  1710 */   475,   19,  475,  475,   84,   85,   22,  410,  412,  414,
 /*  1720 */   355,  475,  529,  103,  399,   83,  185,  102,  282,  284,
 /*  1730 */   283,  475,  474,  130,  131,  132,  133,  134,  135,  136,
 /*  1740 */   137,  138,  139,  140,  141,  142,  143,  144,  145,  146,
 /*  1750 */   148,  147,  475,  475,  475,  475,  475,  475,  475,  475,
 /*  1760 */   475,  475,  410,  412,  414,   78,   79,   81,   80,   82,
 /*  1770 */   475,  339,  475,  475,  475,  475,  475,  474,  130,  131,
 /*  1780 */   132,  133,  134,  135,  136,  137,  138,  139,  140,  141,
 /*  1790 */   142,  143,  144,  145,  146,  148,  147,  475,  593,  475,
 /*  1800 */   475,  475,  475,  475,   84,   85,  475,  410,  412,  414,
 /*  1810 */   355,  475,  121,  475,  379,   83,  490,  475,  282,  284,
 /*  1820 */   283,  475,  474,  130,  131,  132,  133,  134,  135,  136,
 /*  1830 */   137,  138,  139,  140,  141,  142,  143,  144,  145,  146,
 /*  1840 */   148,  147,  475,  475,  475,  475,  475,  475,  475,  475,
 /*  1850 */   475,  475,  410,  412,  414,   78,   79,   81,   80,   82,
 /*  1860 */   475,  513,  475,  475,  475,  475,  475,  474,  130,  131,
 /*  1870 */   132,  133,  134,  135,  136,  137,  138,  139,  140,  141,
 /*  1880 */   142,  143,  144,  145,  146,  148,  147,  475,  475,  475,
 /*  1890 */   475,  475,  475,  475,   84,   85,  475,  410,  412,  414,
 /*  1900 */   355,  475,   23,  475,  359,   83,  506,  475,  282,  284,
 /*  1910 */   283,  475,  474,  130,  131,  132,  133,  134,  135,  136,
 /*  1920 */   137,  138,  139,  140,  141,  142,  143,  144,  145,  146,
 /*  1930 */   148,  147,  475,  475,  475,  475,  475,  475,  475,  475,
 /*  1940 */   475,  475,  410,  412,  414,   78,   79,   81,   80,   82,
 /*  1950 */   475,  519,  475,  475,  475,  475,  475,  474,  130,  131,
 /*  1960 */   132,  133,  134,  135,  136,  137,  138,  139,  140,  141,
 /*  1970 */   142,  143,  144,  145,  146,  148,  147,  475,  475,  475,
 /*  1980 */   475,  475,  475,  475,   84,   85,  475,  410,  412,  414,
 /*  1990 */   355,  475,  121,  475,  383,   83,  161,  475,  282,  284,
 /*  2000 */   283,  475,  474,  130,  131,  132,  133,  134,  135,  136,
 /*  2010 */   137,  138,  139,  140,  141,  142,  143,  144,  145,  146,
 /*  2020 */   148,  147,  475,  475,  475,  475,  475,  475,  475,  475,
 /*  2030 */   475,  475,  410,  412,  414,   78,   79,   81,   80,   82,
 /*  2040 */   475,  525,  475,  475,  475,  475,  475,  474,  130,  131,
 /*  2050 */   132,  133,  134,  135,  136,  137,  138,  139,  140,  141,
 /*  2060 */   142,  143,  144,  145,  146,  148,  147,  475,  475,  475,
 /*  2070 */   475,  475,  475,  475,   84,   85,  475,  410,  412,  414,
 /*  2080 */   355,  475,   88,  475,  391,   83,  535,  475,  282,  284,
 /*  2090 */   283,  475,  474,  130,  131,  132,  133,  134,  135,  136,
 /*  2100 */   137,  138,  139,  140,  141,  142,  143,  144,  145,  146,
 /*  2110 */   148,  147,  475,  475,  475,  475,  475,  475,  475,  475,
 /*  2120 */   475,  475,  410,  412,  414,   78,   79,   81,   80,   82,
 /*  2130 */   475,  541,  475,  475,  475,  475,  475,  474,  130,  131,
 /*  2140 */   132,  133,  134,  135,  136,  137,  138,  139,  140,  141,
 /*  2150 */   142,  143,  144,  145,  146,  148,  147,  475,  475,  475,
 /*  2160 */   475,  475,  475,  247,   84,   85,  475,  410,  412,  414,
 /*  2170 */   560,  282,  284,  283,  475,   83,  547,  475,  272,  285,
 /*  2180 */   475,  475,  474,  130,  131,  132,  133,  134,  135,  136,
 /*  2190 */   137,  138,  139,  140,  141,  142,  143,  144,  145,  146,
 /*  2200 */   148,  147,  475,  475,  475,  475,  475,  475,  475,  475,
 /*  2210 */   475,  475,  410,  412,  414,   78,   79,   81,   80,   82,
 /*  2220 */   475,  553,  475,  475,  475,  475,  475,  474,  130,  131,
 /*  2230 */   132,  133,  134,  135,  136,  137,  138,  139,  140,  141,
 /*  2240 */   142,  143,  144,  145,  146,  148,  147,  475,  475,  475,
 /*  2250 */   475,  475,  475,  475,   84,   85,  475,  410,  412,  414,
 /*  2260 */   408,  475,  529,  475,  475,   83,  485,  407,  282,  284,
 /*  2270 */   283,  475,  474,  130,  131,  132,  133,  134,  135,  136,
 /*  2280 */   137,  138,  139,  140,  141,  142,  143,  144,  145,  146,
 /*  2290 */   148,  147,  475,  475,  475,  475,  475,  475,  475,  475,
 /*  2300 */   475,  475,  410,  412,  414,   78,   79,   81,   80,   82,
 /*  2310 */   475,  173,  475,  475,  475,  475,  475,  474,  130,  131,
 /*  2320 */   132,  133,  134,  135,  136,  137,  138,  139,  140,  141,
 /*  2330 */   142,  143,  144,  145,  146,  148,  147,  475,  475,  475,
 /*  2340 */   475,  475,  263,  475,   84,   85,  475,  410,  412,  414,
 /*  2350 */   282,  284,  283,  475,  475,   83,  500,  475,  475,  304,
 /*  2360 */   305,  308,  474,  130,  131,  132,  133,  134,  135,  136,
 /*  2370 */   137,  138,  139,  140,  141,  142,  143,  144,  145,  146,
 /*  2380 */   148,  147,   65,  475,  475,  475,  475,  475,  555,  118,
 /*  2390 */   570,  592,  410,  412,  414,  368,  602,  558,  475,  475,
 /*  2400 */   364,  176,  102,  282,  284,  283,  475,  474,  130,  131,
 /*  2410 */   132,  133,  134,  135,  136,  137,  138,  139,  140,  141,
 /*  2420 */   142,  143,  144,  145,  146,  148,  147,  475,  475,  475,
 /*  2430 */   475,   60,  475,  475,  475,  475,  475,  410,  412,  414,
 /*  2440 */   232,   58,   59,   61,   62,   64,   63,  475,  282,  284,
 /*  2450 */   283,  475,  129,  130,  131,  132,  133,  134,  135,  136,
 /*  2460 */   137,  138,  139,  140,  141,  142,  143,  144,  145,  146,
 /*  2470 */   148,  197,  200,  203,  475,  475,  475,  475,  475,  239,
 /*  2480 */   475,  475,  410,  412,  414,   78,   79,   81,   80,   82,
 /*  2490 */   475,  193,  248,  252,  268,  190,  475,  475,  243,  197,
 /*  2500 */   200,  203,  475,  475,  475,  475,  275,  239,  355,  475,
 /*  2510 */   475,  254,  387,  233,  475,  475,  282,  284,  283,  193,
 /*  2520 */   248,  252,  268,  190,   84,   85,  243,   20,   86,  197,
 /*  2530 */   200,  203,  475,  475,  275,   83,  263,  239,  475,  254,
 /*  2540 */   475,  233,  475,  475,  282,  284,  283,  256,  262,  193,
 /*  2550 */   248,  252,  268,  190,  475,  307,  243,  255,  257,  258,
 /*  2560 */   259,  260,  261,  475,  275,  475,  475,  475,  475,  254,
 /*  2570 */   475,  233,  475,  368,  475,  256,  262,  361,  388,  475,
 /*  2580 */   475,  282,  284,  283,  475,  255,  257,  258,  259,  260,
 /*  2590 */   261,  192,  202,  207,  209,  211,  213,  205,  199,  215,
 /*  2600 */   217,  225,  219,  221,  223,  256,  262,  393,  475,  475,
 /*  2610 */   475,  475,  475,  475,  475,  255,  257,  258,  259,  260,
 /*  2620 */   261,  475,  475,  197,  200,  203,  247,  475,  475,  355,
 /*  2630 */   475,  239,  475,  367,  282,  284,  283,  282,  284,  283,
 /*  2640 */   475,  323,  285,  193,  248,  252,  268,  190,  289,  475,
 /*  2650 */   243,  197,  200,  203,  636,  475,  475,  475,  275,  239,
 /*  2660 */   594,  475,  475,  254,  475,  233,  475,  580,  475,  475,
 /*  2670 */   595,  193,  248,  252,  268,  190,  297,  475,  243,  475,
 /*  2680 */   475,  197,  200,  203,  475,  475,  275,    8,  475,  239,
 /*  2690 */    11,  254,  104,  233,  617,  627,  475,  475,  473,  256,
 /*  2700 */   262,  193,  248,  252,  268,  190,  281,  475,  243,  255,
 /*  2710 */   257,  258,  259,  260,  261,  475,  275,  475,  475,  475,
 /*  2720 */   475,  254,  475,  233,  197,  200,  203,  256,  262,  475,
 /*  2730 */   475,  475,  239,  475,  475,  475,  475,  255,  257,  258,
 /*  2740 */   259,  260,  261,  475,  193,  248,  252,  268,  190,  475,
 /*  2750 */   475,  264,  475,  197,  200,  203,  475,  256,  262,  275,
 /*  2760 */   475,  239,  475,  475,  254,  475,  233,  255,  257,  258,
 /*  2770 */   259,  260,  261,  193,  248,  252,  268,  190,  475,  475,
 /*  2780 */   243,  475,  247,  475,  347,  475,  475,  475,  275,  356,
 /*  2790 */   282,  284,  283,  254,  475,  233,  475,  310,  285,  475,
 /*  2800 */   299,  262,  300,  475,  475,  475,  475,  475,  475,  475,
 /*  2810 */   298,  257,  258,  259,  260,  261,  475,  348,  349,  350,
 /*  2820 */   351,  352,  475,  392,  475,  380,  400,  401,  355,  256,
 /*  2830 */   262,  385,  354,  475,  475,  475,  282,  284,  283,  255,
 /*  2840 */   257,  258,  259,  260,  261,  475,  475,  197,  200,  203,
 /*  2850 */   247,  164,  475,  475,  333,  239,  475,  475,  282,  284,
 /*  2860 */   283,  475,  282,  284,  283,  287,  285,  193,  248,  252,
 /*  2870 */   268,  190,  325,  475,  243,  197,  200,  203,  475,  475,
 /*  2880 */   475,  155,  275,  239,  333,  475,  475,  254,  475,  233,
 /*  2890 */   475,  475,  282,  284,  283,  193,  248,  252,  268,  190,
 /*  2900 */   271,  475,  243,  475,  475,  197,  200,  203,  475,  475,
 /*  2910 */   275,    8,  475,  239,   11,  254,  104,  233,  617,  627,
 /*  2920 */   475,  475,  473,  256,  262,  193,  248,  252,  268,  190,
 /*  2930 */   312,  475,  243,  255,  257,  258,  259,  260,  261,  475,
 /*  2940 */   275,  475,  475,  475,  475,  254,  475,  233,  197,  200,
 /*  2950 */   203,  256,  262,  475,  475,  475,  239,  475,  475,  475,
 /*  2960 */   475,  255,  257,  258,  259,  260,  261,  475,  193,  248,
 /*  2970 */   252,  268,  190,  319,  475,  243,  475,  197,  200,  203,
 /*  2980 */   475,  256,  262,  275,  475,  239,  475,  475,  254,  475,
 /*  2990 */   233,  255,  257,  258,  259,  260,  261,  193,  248,  252,
 /*  3000 */   268,  190,  475,  475,  243,  419,  197,  200,  203,  475,
 /*  3010 */   475,  475,  275,  475,  239,  475,  475,  254,  475,  233,
 /*  3020 */   475,  475,  475,  475,  256,  262,  193,  248,  252,  268,
 /*  3030 */   190,  475,  475,  243,  255,  257,  258,  259,  260,  261,
 /*  3040 */   475,  275,  475,  475,  475,  475,  254,  475,  233,  197,
 /*  3050 */   200,  203,  355,  256,  262,  475,  375,  239,  475,  475,
 /*  3060 */   282,  284,  283,  255,  257,  258,  259,  260,  261,  193,
 /*  3070 */   248,  252,  268,  190,  475,  487,  243,  475,  197,  200,
 /*  3080 */   203,  475,  256,  262,  275,  475,  239,  475,  475,  254,
 /*  3090 */   475,  233,  255,  257,  258,  259,  260,  261,  193,  248,
 /*  3100 */   252,  268,  190,  475,  475,  264,  247,   78,   79,   81,
 /*  3110 */    80,   82,  475,  275,  282,  284,  283,  475,  254,  328,
 /*  3120 */   233,  295,  285,  475,  475,  256,  262,  282,  284,  283,
 /*  3130 */   179,  475,  475,  333,  475,  255,  257,  258,  259,  260,
 /*  3140 */   261,  282,  284,  283,  475,  475,   84,   85,  475,  662,
 /*  3150 */   571,  198,  565,  475,  299,  262,  475,   83,  475,  282,
 /*  3160 */   284,  283,  475,  475,  298,  257,  258,  259,  260,  261,
 /*  3170 */    78,   79,   81,   80,   82,  231,  227,  229,  192,  202,
 /*  3180 */   207,  209,  211,  213,  205,  199,  215,  217,  225,  219,
 /*  3190 */   221,  223,  231,  227,  229,  192,  202,  207,  209,  211,
 /*  3200 */   213,  205,  199,  215,  217,  225,  219,  221,  223,   84,
 /*  3210 */    85,  475,  663,  571,  475,  565,  475,  475,  475,  263,
 /*  3220 */    83,  475,  475,  475,  477,  475,  475,  282,  284,  283,
 /*  3230 */   475,  247,  475,  301,  475,  247,  309,  305,  308,  282,
 /*  3240 */   284,  283,  475,  282,  284,  283,  279,  285,  327,  475,
 /*  3250 */   427,  286,  475,  475,  475,  475,  282,  284,  283,  475,
 /*  3260 */   475,  231,  227,  229,  192,  202,  207,  209,  211,  213,
 /*  3270 */   205,  199,  215,  217,  225,  219,  221,  223,  231,  227,
 /*  3280 */   229,  192,  202,  207,  209,  211,  213,  205,  199,  215,
 /*  3290 */   217,  225,  219,  221,  223,   78,   79,   81,   80,   82,
 /*  3300 */   475,  475,  475,  247,  475,  475,  201,  475,  475,  475,
 /*  3310 */   484,  282,  284,  283,  282,  284,  283,  326,  317,  285,
 /*  3320 */   475,  204,  475,  475,  475,  282,  284,  283,  475,  282,
 /*  3330 */   284,  283,  475,  206,   84,   85,  329,  114,  571,  475,
 /*  3340 */   565,  282,  284,  283,  475,   83,  475,  231,  227,  229,
 /*  3350 */   192,  202,  207,  209,  211,  213,  205,  199,  215,  217,
 /*  3360 */   225,  219,  221,  223,  231,  227,  229,  192,  202,  207,
 /*  3370 */   209,  211,  213,  205,  199,  215,  217,  225,  219,  221,
 /*  3380 */   223,   78,   79,   81,   80,   82,  475,  475,  475,  208,
 /*  3390 */   475,  475,  210,  475,  475,  212,  338,  282,  284,  283,
 /*  3400 */   282,  284,  283,  282,  284,  283,  475,  214,  475,  475,
 /*  3410 */   475,  475,  475,  475,  475,  282,  284,  283,  216,  475,
 /*  3420 */    84,   85,  822,  665,  527,  475,  282,  284,  283,  475,
 /*  3430 */   475,   83,  475,  231,  227,  229,  192,  202,  207,  209,
 /*  3440 */   211,  213,  205,  199,  215,  217,  225,  219,  221,  223,
 /*  3450 */   475,  475,  475,  475,  475,  235,  231,  227,  229,  192,
 /*  3460 */   202,  207,  209,  211,  213,  205,  199,  215,  217,  225,
 /*  3470 */   219,  221,  223,  475,  475,  475,  475,  475,  475,   32,
 /*  3480 */   475,  418,  231,  227,  229,  192,  202,  207,  209,  211,
 /*  3490 */   213,  205,  199,  215,  217,  225,  219,  221,  223,  231,
 /*  3500 */   227,  229,  192,  202,  207,  209,  211,  213,  205,  199,
 /*  3510 */   215,  217,  225,  219,  221,  223,   35,   36,   37,   38,
 /*  3520 */    39,   40,   41,   42,   43,   44,   45,  218,  475,  475,
 /*  3530 */   220,  489,  475,  222,  475,  282,  284,  283,  282,  284,
 /*  3540 */   283,  282,  284,  283,  475,  224,  475,  475,  475,  475,
 /*  3550 */   475,  226,  475,  282,  284,  283,  228,  293,  475,  282,
 /*  3560 */   284,  283,  475,  475,  282,  284,  283,  475,  231,  227,
 /*  3570 */   229,  192,  202,  207,  209,  211,  213,  205,  199,  215,
 /*  3580 */   217,  225,  219,  221,  223,  227,  229,  192,  202,  207,
 /*  3590 */   209,  211,  213,  205,  199,  215,  217,  225,  219,  221,
 /*  3600 */   223,   32,  475,  475,  475,  230,  475,  475,  475,  475,
 /*  3610 */   475,  475,   32,  282,  284,  283,  475,  475,  124,  475,
 /*  3620 */    55,  475,   78,   79,   81,   80,   82,  475,  475,   26,
 /*  3630 */   475,   55,  475,  475,  475,  475,  475,  475,   35,   36,
 /*  3640 */    37,   38,   39,   40,   41,   42,   43,   44,   45,   35,
 /*  3650 */    36,   37,   38,   39,   40,   41,   42,   43,   44,   45,
 /*  3660 */    32,   84,   85,  475,  669,  527,  417,  475,  475,  475,
 /*  3670 */   475,   32,   83,  475,  282,  284,  283,   91,  475,   55,
 /*  3680 */   475,   78,   79,   81,   80,   82,  238,  475,  532,  475,
 /*  3690 */    55,  475,  475,  475,  282,  284,  283,   35,   36,   37,
 /*  3700 */    38,   39,   40,   41,   42,   43,   44,   45,   35,   36,
 /*  3710 */    37,   38,   39,   40,   41,   42,   43,   44,   45,   32,
 /*  3720 */    84,   85,  475,  667,  606,  475,  475,  475,  475,  475,
 /*  3730 */   475,   83,  475,   78,   79,   81,   80,   82,   55,   78,
 /*  3740 */    79,   81,   80,   82,  426,   78,   79,   81,   80,   82,
 /*  3750 */   475,  475,  282,  284,  283,  475,   35,   36,   37,   38,
 /*  3760 */    39,   40,   41,   42,   43,   44,   45,  347,  475,  475,
 /*  3770 */   475,  475,   84,   85,  475,  668,  527,  475,   84,   85,
 /*  3780 */   475,  664,  527,   83,   84,   85,  475,  670,  527,   83,
 /*  3790 */   475,  475,  475,  475,  475,   83,  475,  475,  475,  475,
 /*  3800 */   348,  349,  350,  351,  352,  430,  360,  475,  242,  369,
 /*  3810 */   370,  475,  253,  282,  284,  283,  282,  284,  283,  475,
 /*  3820 */   282,  284,  283,  475,  475,  475,  475,  476,  475,  475,
 /*  3830 */   483,  475,  475,  488,  475,  282,  284,  283,  282,  284,
 /*  3840 */   283,  282,  284,  283,  292,  475,  475,  475,  475,  475,
 /*  3850 */   191,  475,  282,  284,  283,  332,  475,  475,  282,  284,
 /*  3860 */   283,  337,  475,  282,  284,  283,  475,  475,  475,  282,
 /*  3870 */   284,  283,
};
static YYCODETYPE yy_lookahead[] = {
 /*     0 */     7,    8,    9,  103,  104,  105,  106,  107,   15,  100,
 /*    10 */   101,  102,  103,  104,  105,  106,  107,  130,  131,  132,
 /*    20 */    27,   28,   29,   30,   31,  111,   34,   34,  114,  115,
 /*    30 */   116,  117,  118,  118,   42,   42,   37,  123,  124,  124,
 /*    40 */    47,   42,   49,  129,  129,   52,   53,   54,   55,   56,
 /*    50 */    57,   58,   59,   60,   61,   62,   10,   11,   12,   13,
 /*    60 */    14,   15,   16,   17,   18,   19,   20,   21,   22,   23,
 /*    70 */    24,   25,   26,   34,  126,    6,   83,   84,  130,  131,
 /*    80 */   132,   35,   43,   44,   34,   35,   93,   94,   95,   96,
 /*    90 */    97,   98,   42,   43,   44,  126,   24,   25,   26,  130,
 /*   100 */   131,  132,   52,   53,   54,   55,   56,   57,   58,   59,
 /*   110 */    60,   42,   62,   63,   64,   65,  128,   67,  130,  131,
 /*   120 */   118,   71,   72,   73,   74,   34,   35,   77,  155,  166,
 /*   130 */   167,  129,  159,   42,   43,   44,  163,  164,  165,   89,
 /*   140 */    90,   91,   92,   52,   53,   54,   55,   56,   57,   58,
 /*   150 */    59,   60,  168,   62,   63,   64,   65,  114,   67,  116,
 /*   160 */   117,  118,   71,   72,   73,   74,  123,  124,   77,   34,
 /*   170 */    35,   50,  129,  152,  166,  167,  155,   42,   43,   44,
 /*   180 */    89,   90,   91,   92,  163,  164,  165,   52,   53,   54,
 /*   190 */    55,   56,   57,   58,   59,   60,   34,   62,   63,   64,
 /*   200 */    65,  128,   67,  130,  131,   84,   71,   72,   73,   74,
 /*   210 */    49,   50,   77,   34,   35,   21,   22,   23,   24,   25,
 /*   220 */    26,   42,   43,   44,   89,   90,   91,   92,    6,  109,
 /*   230 */    69,   52,   53,   54,   55,   56,   57,   58,   59,   60,
 /*   240 */    35,   62,   63,   64,   65,   84,   67,   86,  166,  167,
 /*   250 */    71,   72,   73,   74,   32,   50,   77,   34,   35,   34,
 /*   260 */     6,  152,   49,   50,  155,   42,   43,   44,   89,   90,
 /*   270 */    91,   92,  163,  164,  165,   52,   53,   54,   55,   56,
 /*   280 */    57,   58,   59,   60,   42,   62,   63,   64,   65,   35,
 /*   290 */    67,   43,   50,   34,   71,   72,   73,   74,  155,   86,
 /*   300 */    77,   34,   35,   44,  161,  162,  163,  164,  165,   42,
 /*   310 */    43,   44,   89,   90,   91,   92,   68,  118,   70,   52,
 /*   320 */    53,   54,   55,   56,   57,   58,   59,   60,  129,   62,
 /*   330 */    63,   64,   65,   38,   67,  116,    6,  118,   71,   72,
 /*   340 */    73,   74,  123,  124,   77,   34,   35,   34,  129,  128,
 /*   350 */   118,  130,  131,   42,   43,   44,   89,   90,   91,   92,
 /*   360 */    47,  129,   32,   52,   53,   54,   55,   56,   57,   58,
 /*   370 */    59,   60,   35,   62,   63,   64,   65,   39,   67,   43,
 /*   380 */    42,    6,   71,   72,   73,   74,  155,   50,   77,   34,
 /*   390 */    35,  160,    6,  119,  163,  164,  165,   42,   43,   44,
 /*   400 */    89,   90,   91,   92,   68,  130,   70,   52,   53,   54,
 /*   410 */    55,   56,   57,   58,   59,   60,   34,   62,   63,   64,
 /*   420 */    65,   35,   67,  116,  117,  118,   71,   72,   73,   74,
 /*   430 */   155,  124,   77,   34,   35,  110,  129,  112,  163,  164,
 /*   440 */   165,   42,   43,   44,   89,   90,   91,   92,  166,  167,
 /*   450 */    34,   52,   53,   54,   55,   56,   57,   58,   59,   60,
 /*   460 */    85,   62,   63,   64,   65,   83,   67,    6,    6,    6,
 /*   470 */    71,   72,   73,   74,   49,   50,   77,   34,   35,   37,
 /*   480 */   128,   39,  130,  131,   42,   42,   43,   44,   89,   90,
 /*   490 */    91,   92,  153,  154,   32,   52,   53,   54,   55,   56,
 /*   500 */    57,   58,   59,   60,   43,   62,   63,   64,   65,   84,
 /*   510 */    67,   86,    6,  119,   71,   72,   73,   74,   35,    6,
 /*   520 */    77,   34,   35,    6,  128,   42,  130,  131,   45,   42,
 /*   530 */    43,   44,   89,   90,   91,   92,    6,   42,   32,   52,
 /*   540 */    53,   54,   55,   56,   57,   58,   59,   60,   35,   62,
 /*   550 */    63,   64,   65,    6,   67,  166,  167,   69,   71,   72,
 /*   560 */    73,   74,   32,  126,   77,   34,   35,  130,  131,  132,
 /*   570 */     6,  156,  157,   42,   43,   44,   89,   90,   91,   92,
 /*   580 */   166,  167,   35,   52,   53,   54,   55,   56,   57,   58,
 /*   590 */    59,   60,   75,   62,   63,   64,   65,    6,   67,   35,
 /*   600 */   118,   34,   71,   72,   73,   74,   35,  125,   77,   34,
 /*   610 */    35,  129,  128,   42,  130,  131,   42,   42,   43,   44,
 /*   620 */    89,   90,   91,   92,   50,   69,   35,   52,   53,   54,
 /*   630 */    55,   56,   57,   58,   59,   60,   34,   62,   63,   64,
 /*   640 */    65,    6,   67,  166,  167,  119,   71,   72,   73,   74,
 /*   650 */    83,    6,   77,   34,   35,   37,  128,   39,  130,  131,
 /*   660 */    42,   42,   43,   44,   89,   90,   91,   92,    6,   69,
 /*   670 */    35,   52,   53,   54,   55,   56,   57,   58,   59,   60,
 /*   680 */    35,   62,   63,   64,   65,   34,   67,  129,    6,   35,
 /*   690 */    71,   72,   73,   74,   32,  126,   77,   34,   35,  130,
 /*   700 */   131,  132,  166,  167,   50,   42,   43,   44,   89,   90,
 /*   710 */    91,   92,  166,  167,   32,   52,   53,   54,   55,   56,
 /*   720 */    57,   58,   59,   60,   69,   62,   63,   64,   65,  128,
 /*   730 */    67,  130,  131,  108,   71,   72,   73,   74,   42,    6,
 /*   740 */    77,   34,   35,  110,    6,  112,   50,  119,   34,   42,
 /*   750 */    43,   44,   89,   90,   91,   92,   42,  121,  122,   52,
 /*   760 */    53,   54,   55,   56,   57,   58,   59,   60,   35,   62,
 /*   770 */    63,   64,   65,   35,   67,  166,  167,   35,   71,   72,
 /*   780 */    73,   74,   35,    6,   77,   34,   35,   50,  110,   42,
 /*   790 */   112,    6,   50,   42,   43,   44,   89,   90,   91,   92,
 /*   800 */     6,   45,    6,   52,   53,   54,   55,   56,   57,   58,
 /*   810 */    59,   60,   35,   62,   63,   64,   65,   32,   67,    6,
 /*   820 */    49,   84,   71,   72,   73,   74,   32,   34,   77,   34,
 /*   830 */    35,   35,   49,   34,  167,   42,  109,   42,   43,   44,
 /*   840 */    89,   90,   91,   92,   34,   32,   49,   52,   53,   54,
 /*   850 */    55,   56,   57,   58,   59,   60,   42,   62,   63,   64,
 /*   860 */    65,   45,   67,    6,   50,   37,   71,   72,   73,   74,
 /*   870 */    42,   47,   77,   34,   35,  158,   34,  119,   35,   42,
 /*   880 */     0,   42,   43,   44,   89,   90,   91,   92,   47,   32,
 /*   890 */    69,   52,   53,   54,   55,   56,   57,   58,   59,   60,
 /*   900 */    45,   62,   63,   64,   65,   34,   67,   16,  119,   35,
 /*   910 */    71,   72,   73,   74,   35,   34,   77,   34,   35,   35,
 /*   920 */    34,  109,  120,   34,   42,   42,   43,   44,   89,   90,
 /*   930 */    91,   92,   42,   32,  108,   52,   53,   54,   55,   56,
 /*   940 */    57,   58,   59,   60,  119,   62,   63,   64,   65,  157,
 /*   950 */    67,  120,   35,   34,   71,   72,   73,   74,   35,   35,
 /*   960 */    77,   34,   35,   42,   35,   66,   34,  109,   35,   42,
 /*   970 */    43,   44,   89,   90,   91,   92,   35,   35,   35,   52,
 /*   980 */    53,   54,   55,   56,   57,   58,   59,   60,  122,   62,
 /*   990 */    63,   64,   65,   42,   67,   43,  109,   35,   71,   72,
 /*  1000 */    73,   74,   49,   34,   77,   34,   35,   43,   34,   42,
 /*  1010 */    38,   35,  168,   42,   43,   44,   89,   90,   91,   92,
 /*  1020 */    42,  158,   85,   52,   53,   54,   55,   56,   57,   58,
 /*  1030 */    59,   60,   49,   62,   63,   64,   65,   49,   67,   43,
 /*  1040 */   120,  119,   71,   72,   73,   74,  158,  119,   77,   34,
 /*  1050 */    35,   35,  158,   49,  120,   35,   34,   42,   43,   44,
 /*  1060 */    89,   90,   91,   92,   85,  158,   35,   52,   53,   54,
 /*  1070 */    55,   56,   57,   58,   59,   60,   34,   62,   63,   64,
 /*  1080 */    65,   72,   67,   34,   66,   16,   71,   72,   73,   74,
 /*  1090 */    35,   34,   77,   34,   35,   42,   45,   43,  158,   75,
 /*  1100 */    34,   42,   43,   44,   89,   90,   91,   92,   35,   45,
 /*  1110 */    43,   52,   53,   54,   55,   56,   57,   58,   59,   60,
 /*  1120 */    43,   62,   63,   64,   65,   42,   67,   42,  158,   34,
 /*  1130 */    71,   72,   73,   74,  109,  158,   77,   34,   35,   34,
 /*  1140 */    34,   47,  109,   34,   42,   42,   43,   44,   89,   90,
 /*  1150 */    91,   92,   35,   49,   34,   52,   53,   54,   55,   56,
 /*  1160 */    57,   58,   59,   60,  158,   62,   63,   64,   65,   85,
 /*  1170 */    67,   49,   34,   34,   71,   72,   73,   74,   86,  109,
 /*  1180 */    77,   34,   35,   34,   34,  158,  112,   34,  158,   42,
 /*  1190 */    43,   44,   89,   90,   91,   92,   35,   42,   35,   52,
 /*  1200 */    53,   54,   55,   56,   57,   58,   59,   60,   49,   62,
 /*  1210 */    63,   64,   65,   45,   67,  154,   72,   34,   71,   72,
 /*  1220 */    73,   74,  109,  175,   77,   34,   35,  175,  175,  175,
 /*  1230 */   175,  175,  175,   42,   43,   44,   89,   90,   91,   92,
 /*  1240 */   175,  175,  175,   52,   53,   54,   55,   56,   57,   58,
 /*  1250 */    59,   60,  175,   62,   63,   64,   65,  175,   67,  175,
 /*  1260 */   175,  175,   71,   72,   73,   74,  175,  175,   77,   34,
 /*  1270 */    35,  175,  175,  175,  175,  175,  175,   42,   43,   44,
 /*  1280 */    89,   90,   91,   92,  175,  175,  175,   52,   53,   54,
 /*  1290 */    55,   56,   57,   58,   59,   60,  175,   62,   63,   64,
 /*  1300 */    65,  175,   67,  175,  175,  175,   71,   72,   73,   74,
 /*  1310 */   175,  175,   77,   34,   35,  175,  175,  175,  175,  175,
 /*  1320 */   175,   42,   43,   44,   89,   90,   91,   92,  175,  175,
 /*  1330 */   175,   52,   53,   54,   55,   56,   57,   58,   59,   60,
 /*  1340 */   175,   62,   63,   64,   65,  175,   67,  175,  175,  175,
 /*  1350 */    71,   72,   73,   74,  175,  175,   77,   34,   35,  175,
 /*  1360 */   175,  175,  175,  175,  175,   42,   43,   44,   89,   90,
 /*  1370 */    91,   92,  175,  175,  175,   52,   53,   54,   55,   56,
 /*  1380 */    57,   58,   59,   60,  175,   62,   63,   64,   65,  175,
 /*  1390 */    67,  175,  175,  175,   71,   72,   73,   74,  175,  175,
 /*  1400 */    77,   34,   35,  175,  175,  175,  175,  175,  175,   42,
 /*  1410 */    43,   44,   89,   90,   91,   92,  175,  175,  175,   52,
 /*  1420 */    53,   54,   55,   56,   57,   58,   59,   60,  175,   62,
 /*  1430 */    63,   64,   65,  175,   67,  175,  175,  175,   71,   72,
 /*  1440 */    73,   74,  175,  175,   77,   34,   35,  175,  175,  175,
 /*  1450 */   175,  175,  175,   42,   43,   44,   89,   90,   91,   92,
 /*  1460 */   175,  175,  175,   52,   53,   54,   55,   56,   57,   58,
 /*  1470 */    59,   60,  175,   62,   63,   64,   65,  175,   67,  175,
 /*  1480 */   175,  175,   71,   72,   73,   74,  175,  175,   77,   34,
 /*  1490 */    35,    1,    2,    3,    4,    5,  175,   42,  175,   44,
 /*  1500 */    89,   90,   91,   92,  175,  175,  175,   52,   53,   54,
 /*  1510 */    55,   56,   57,   58,   59,   60,  175,   62,   63,   64,
 /*  1520 */    65,  175,   67,  107,  175,  175,   71,   72,   73,   74,
 /*  1530 */    40,   41,   77,   43,   44,  175,   46,  175,  175,   45,
 /*  1540 */   175,   51,  175,  127,   89,   90,   91,   92,  175,  133,
 /*  1550 */   134,  135,  136,  137,  138,  139,  140,  141,  142,  143,
 /*  1560 */   144,  145,  146,  147,  148,  149,  150,  151,  175,  175,
 /*  1570 */   175,  107,   78,   79,   80,   81,   82,  175,   84,  163,
 /*  1580 */   164,  165,    1,    2,    3,    4,    5,  175,  175,  175,
 /*  1590 */   175,  127,  175,  175,  175,  175,  175,  133,  134,  135,
 /*  1600 */   136,  137,  138,  139,  140,  141,  142,  143,  144,  145,
 /*  1610 */   146,  147,  148,  149,  150,  151,  107,  175,  175,  175,
 /*  1620 */   175,   40,   41,  175,   43,   44,  175,  163,  164,  165,
 /*  1630 */    45,  155,   51,  175,  175,  159,  127,  175,  175,  163,
 /*  1640 */   164,  165,  133,  134,  135,  136,  137,  138,  139,  140,
 /*  1650 */   141,  142,  143,  144,  145,  146,  147,  148,  149,  150,
 /*  1660 */   151,  107,  175,   78,   79,   80,   81,   82,  175,  175,
 /*  1670 */   175,  175,  163,  164,  165,    1,    2,    3,    4,    5,
 /*  1680 */   175,  127,  175,  175,  175,  175,  175,  133,  134,  135,
 /*  1690 */   136,  137,  138,  139,  140,  141,  142,  143,  144,  145,
 /*  1700 */   146,  147,  148,  149,  150,  151,  107,  175,   34,  175,
 /*  1710 */   175,  113,  175,  175,   40,   41,  118,  163,  164,  165,
 /*  1720 */   155,  175,   48,  125,  159,   51,  127,  129,  163,  164,
 /*  1730 */   165,  175,  133,  134,  135,  136,  137,  138,  139,  140,
 /*  1740 */   141,  142,  143,  144,  145,  146,  147,  148,  149,  150,
 /*  1750 */   151,  107,  175,  175,  175,  175,  175,  175,  175,  175,
 /*  1760 */   175,  175,  163,  164,  165,    1,    2,    3,    4,    5,
 /*  1770 */   175,  127,  175,  175,  175,  175,  175,  133,  134,  135,
 /*  1780 */   136,  137,  138,  139,  140,  141,  142,  143,  144,  145,
 /*  1790 */   146,  147,  148,  149,  150,  151,  107,  175,   34,  175,
 /*  1800 */   175,  175,  175,  175,   40,   41,  175,  163,  164,  165,
 /*  1810 */   155,  175,   48,  175,  159,   51,  127,  175,  163,  164,
 /*  1820 */   165,  175,  133,  134,  135,  136,  137,  138,  139,  140,
 /*  1830 */   141,  142,  143,  144,  145,  146,  147,  148,  149,  150,
 /*  1840 */   151,  107,  175,  175,  175,  175,  175,  175,  175,  175,
 /*  1850 */   175,  175,  163,  164,  165,    1,    2,    3,    4,    5,
 /*  1860 */   175,  127,  175,  175,  175,  175,  175,  133,  134,  135,
 /*  1870 */   136,  137,  138,  139,  140,  141,  142,  143,  144,  145,
 /*  1880 */   146,  147,  148,  149,  150,  151,  107,  175,  175,  175,
 /*  1890 */   175,  175,  175,  175,   40,   41,  175,  163,  164,  165,
 /*  1900 */   155,  175,   48,  175,  159,   51,  127,  175,  163,  164,
 /*  1910 */   165,  175,  133,  134,  135,  136,  137,  138,  139,  140,
 /*  1920 */   141,  142,  143,  144,  145,  146,  147,  148,  149,  150,
 /*  1930 */   151,  107,  175,  175,  175,  175,  175,  175,  175,  175,
 /*  1940 */   175,  175,  163,  164,  165,    1,    2,    3,    4,    5,
 /*  1950 */   175,  127,  175,  175,  175,  175,  175,  133,  134,  135,
 /*  1960 */   136,  137,  138,  139,  140,  141,  142,  143,  144,  145,
 /*  1970 */   146,  147,  148,  149,  150,  151,  107,  175,  175,  175,
 /*  1980 */   175,  175,  175,  175,   40,   41,  175,  163,  164,  165,
 /*  1990 */   155,  175,   48,  175,  159,   51,  127,  175,  163,  164,
 /*  2000 */   165,  175,  133,  134,  135,  136,  137,  138,  139,  140,
 /*  2010 */   141,  142,  143,  144,  145,  146,  147,  148,  149,  150,
 /*  2020 */   151,  107,  175,  175,  175,  175,  175,  175,  175,  175,
 /*  2030 */   175,  175,  163,  164,  165,    1,    2,    3,    4,    5,
 /*  2040 */   175,  127,  175,  175,  175,  175,  175,  133,  134,  135,
 /*  2050 */   136,  137,  138,  139,  140,  141,  142,  143,  144,  145,
 /*  2060 */   146,  147,  148,  149,  150,  151,  107,  175,  175,  175,
 /*  2070 */   175,  175,  175,  175,   40,   41,  175,  163,  164,  165,
 /*  2080 */   155,  175,   48,  175,  159,   51,  127,  175,  163,  164,
 /*  2090 */   165,  175,  133,  134,  135,  136,  137,  138,  139,  140,
 /*  2100 */   141,  142,  143,  144,  145,  146,  147,  148,  149,  150,
 /*  2110 */   151,  107,  175,  175,  175,  175,  175,  175,  175,  175,
 /*  2120 */   175,  175,  163,  164,  165,    1,    2,    3,    4,    5,
 /*  2130 */   175,  127,  175,  175,  175,  175,  175,  133,  134,  135,
 /*  2140 */   136,  137,  138,  139,  140,  141,  142,  143,  144,  145,
 /*  2150 */   146,  147,  148,  149,  150,  151,  107,  175,  175,  175,
 /*  2160 */   175,  175,  175,  155,   40,   41,  175,  163,  164,  165,
 /*  2170 */    46,  163,  164,  165,  175,   51,  127,  175,  170,  171,
 /*  2180 */   175,  175,  133,  134,  135,  136,  137,  138,  139,  140,
 /*  2190 */   141,  142,  143,  144,  145,  146,  147,  148,  149,  150,
 /*  2200 */   151,  107,  175,  175,  175,  175,  175,  175,  175,  175,
 /*  2210 */   175,  175,  163,  164,  165,    1,    2,    3,    4,    5,
 /*  2220 */   175,  127,  175,  175,  175,  175,  175,  133,  134,  135,
 /*  2230 */   136,  137,  138,  139,  140,  141,  142,  143,  144,  145,
 /*  2240 */   146,  147,  148,  149,  150,  151,  107,  175,  175,  175,
 /*  2250 */   175,  175,  175,  175,   40,   41,  175,  163,  164,  165,
 /*  2260 */   155,  175,   48,  175,  175,   51,  127,  162,  163,  164,
 /*  2270 */   165,  175,  133,  134,  135,  136,  137,  138,  139,  140,
 /*  2280 */   141,  142,  143,  144,  145,  146,  147,  148,  149,  150,
 /*  2290 */   151,  107,  175,  175,  175,  175,  175,  175,  175,  175,
 /*  2300 */   175,  175,  163,  164,  165,    1,    2,    3,    4,    5,
 /*  2310 */   175,  127,  175,  175,  175,  175,  175,  133,  134,  135,
 /*  2320 */   136,  137,  138,  139,  140,  141,  142,  143,  144,  145,
 /*  2330 */   146,  147,  148,  149,  150,  151,  107,  175,  175,  175,
 /*  2340 */   175,  175,  155,  175,   40,   41,  175,  163,  164,  165,
 /*  2350 */   163,  164,  165,  175,  175,   51,  127,  175,  175,  172,
 /*  2360 */   173,  174,  133,  134,  135,  136,  137,  138,  139,  140,
 /*  2370 */   141,  142,  143,  144,  145,  146,  147,  148,  149,  150,
 /*  2380 */   151,  107,   34,  175,  175,  175,  175,  175,  115,  116,
 /*  2390 */   117,  118,  163,  164,  165,  155,  123,  124,  175,  175,
 /*  2400 */   160,  127,  129,  163,  164,  165,  175,  133,  134,  135,
 /*  2410 */   136,  137,  138,  139,  140,  141,  142,  143,  144,  145,
 /*  2420 */   146,  147,  148,  149,  150,  151,  107,  175,  175,  175,
 /*  2430 */   175,   83,  175,  175,  175,  175,  175,  163,  164,  165,
 /*  2440 */   155,   93,   94,   95,   96,   97,   98,  175,  163,  164,
 /*  2450 */   165,  175,  133,  134,  135,  136,  137,  138,  139,  140,
 /*  2460 */   141,  142,  143,  144,  145,  146,  147,  148,  149,  150,
 /*  2470 */   151,    7,    8,    9,  175,  175,  175,  175,  175,   15,
 /*  2480 */   175,  175,  163,  164,  165,    1,    2,    3,    4,    5,
 /*  2490 */   175,   27,   28,   29,   30,   31,  175,  175,   34,    7,
 /*  2500 */     8,    9,  175,  175,  175,  175,   42,   15,  155,  175,
 /*  2510 */   175,   47,  159,   49,  175,  175,  163,  164,  165,   27,
 /*  2520 */    28,   29,   30,   31,   40,   41,   34,   43,   44,    7,
 /*  2530 */     8,    9,  175,  175,   42,   51,  155,   15,  175,   47,
 /*  2540 */    76,   49,  175,  175,  163,  164,  165,   83,   84,   27,
 /*  2550 */    28,   29,   30,   31,  175,  174,   34,   93,   94,   95,
 /*  2560 */    96,   97,   98,  175,   42,  175,  175,  175,  175,   47,
 /*  2570 */   175,   49,  175,  155,  175,   83,   84,   85,  160,  175,
 /*  2580 */   175,  163,  164,  165,  175,   93,   94,   95,   96,   97,
 /*  2590 */    98,   13,   14,   15,   16,   17,   18,   19,   20,   21,
 /*  2600 */    22,   23,   24,   25,   26,   83,   84,   85,  175,  175,
 /*  2610 */   175,  175,  175,  175,  175,   93,   94,   95,   96,   97,
 /*  2620 */    98,  175,  175,    7,    8,    9,  155,  175,  175,  155,
 /*  2630 */   175,   15,  175,  159,  163,  164,  165,  163,  164,  165,
 /*  2640 */   175,  170,  171,   27,   28,   29,   30,   31,   32,  175,
 /*  2650 */    34,    7,    8,    9,    0,  175,  175,  175,   42,   15,
 /*  2660 */    35,  175,  175,   47,  175,   49,  175,   42,  175,  175,
 /*  2670 */    45,   27,   28,   29,   30,   31,   32,  175,   34,  175,
 /*  2680 */   175,    7,    8,    9,  175,  175,   42,   33,  175,   15,
 /*  2690 */    36,   47,   38,   49,   40,   41,  175,  175,   44,   83,
 /*  2700 */    84,   27,   28,   29,   30,   31,   32,  175,   34,   93,
 /*  2710 */    94,   95,   96,   97,   98,  175,   42,  175,  175,  175,
 /*  2720 */   175,   47,  175,   49,    7,    8,    9,   83,   84,  175,
 /*  2730 */   175,  175,   15,  175,  175,  175,  175,   93,   94,   95,
 /*  2740 */    96,   97,   98,  175,   27,   28,   29,   30,   31,  175,
 /*  2750 */   175,   34,  175,    7,    8,    9,  175,   83,   84,   42,
 /*  2760 */   175,   15,  175,  175,   47,  175,   49,   93,   94,   95,
 /*  2770 */    96,   97,   98,   27,   28,   29,   30,   31,  175,  175,
 /*  2780 */    34,  175,  155,  175,   45,  175,  175,  175,   42,   50,
 /*  2790 */   163,  164,  165,   47,  175,   49,  175,  170,  171,  175,
 /*  2800 */    83,   84,   85,  175,  175,  175,  175,  175,  175,  175,
 /*  2810 */    93,   94,   95,   96,   97,   98,  175,   78,   79,   80,
 /*  2820 */    81,   82,  175,   84,  175,   86,   87,   88,  155,   83,
 /*  2830 */    84,   85,  159,  175,  175,  175,  163,  164,  165,   93,
 /*  2840 */    94,   95,   96,   97,   98,  175,  175,    7,    8,    9,
 /*  2850 */   155,  152,  175,  175,  155,   15,  175,  175,  163,  164,
 /*  2860 */   165,  175,  163,  164,  165,  170,  171,   27,   28,   29,
 /*  2870 */    30,   31,   32,  175,   34,    7,    8,    9,  175,  175,
 /*  2880 */   175,  152,   42,   15,  155,  175,  175,   47,  175,   49,
 /*  2890 */   175,  175,  163,  164,  165,   27,   28,   29,   30,   31,
 /*  2900 */    32,  175,   34,  175,  175,    7,    8,    9,  175,  175,
 /*  2910 */    42,   33,  175,   15,   36,   47,   38,   49,   40,   41,
 /*  2920 */   175,  175,   44,   83,   84,   27,   28,   29,   30,   31,
 /*  2930 */    32,  175,   34,   93,   94,   95,   96,   97,   98,  175,
 /*  2940 */    42,  175,  175,  175,  175,   47,  175,   49,    7,    8,
 /*  2950 */     9,   83,   84,  175,  175,  175,   15,  175,  175,  175,
 /*  2960 */   175,   93,   94,   95,   96,   97,   98,  175,   27,   28,
 /*  2970 */    29,   30,   31,   32,  175,   34,  175,    7,    8,    9,
 /*  2980 */   175,   83,   84,   42,  175,   15,  175,  175,   47,  175,
 /*  2990 */    49,   93,   94,   95,   96,   97,   98,   27,   28,   29,
 /*  3000 */    30,   31,  175,  175,   34,   35,    7,    8,    9,  175,
 /*  3010 */   175,  175,   42,  175,   15,  175,  175,   47,  175,   49,
 /*  3020 */   175,  175,  175,  175,   83,   84,   27,   28,   29,   30,
 /*  3030 */    31,  175,  175,   34,   93,   94,   95,   96,   97,   98,
 /*  3040 */   175,   42,  175,  175,  175,  175,   47,  175,   49,    7,
 /*  3050 */     8,    9,  155,   83,   84,  175,  159,   15,  175,  175,
 /*  3060 */   163,  164,  165,   93,   94,   95,   96,   97,   98,   27,
 /*  3070 */    28,   29,   30,   31,  175,   76,   34,  175,    7,    8,
 /*  3080 */     9,  175,   83,   84,   42,  175,   15,  175,  175,   47,
 /*  3090 */   175,   49,   93,   94,   95,   96,   97,   98,   27,   28,
 /*  3100 */    29,   30,   31,  175,  175,   34,  155,    1,    2,    3,
 /*  3110 */     4,    5,  175,   42,  163,  164,  165,  175,   47,  155,
 /*  3120 */    49,  170,  171,  175,  175,   83,   84,  163,  164,  165,
 /*  3130 */   152,  175,  175,  155,  175,   93,   94,   95,   96,   97,
 /*  3140 */    98,  163,  164,  165,  175,  175,   40,   41,  175,   43,
 /*  3150 */    44,  155,   46,  175,   83,   84,  175,   51,  175,  163,
 /*  3160 */   164,  165,  175,  175,   93,   94,   95,   96,   97,   98,
 /*  3170 */     1,    2,    3,    4,    5,   10,   11,   12,   13,   14,
 /*  3180 */    15,   16,   17,   18,   19,   20,   21,   22,   23,   24,
 /*  3190 */    25,   26,   10,   11,   12,   13,   14,   15,   16,   17,
 /*  3200 */    18,   19,   20,   21,   22,   23,   24,   25,   26,   40,
 /*  3210 */    41,  175,   43,   44,  175,   46,  175,  175,  175,  155,
 /*  3220 */    51,  175,  175,  175,   42,  175,  175,  163,  164,  165,
 /*  3230 */   175,  155,  175,  169,  175,  155,  172,  173,  174,  163,
 /*  3240 */   164,  165,  175,  163,  164,  165,  170,  171,  155,  175,
 /*  3250 */    85,  171,  175,  175,  175,  175,  163,  164,  165,  175,
 /*  3260 */   175,   10,   11,   12,   13,   14,   15,   16,   17,   18,
 /*  3270 */    19,   20,   21,   22,   23,   24,   25,   26,   10,   11,
 /*  3280 */    12,   13,   14,   15,   16,   17,   18,   19,   20,   21,
 /*  3290 */    22,   23,   24,   25,   26,    1,    2,    3,    4,    5,
 /*  3300 */   175,  175,  175,  155,  175,  175,  155,  175,  175,  175,
 /*  3310 */    42,  163,  164,  165,  163,  164,  165,  155,  170,  171,
 /*  3320 */   175,  155,  175,  175,  175,  163,  164,  165,  175,  163,
 /*  3330 */   164,  165,  175,  155,   40,   41,   85,   43,   44,  175,
 /*  3340 */    46,  163,  164,  165,  175,   51,  175,   10,   11,   12,
 /*  3350 */    13,   14,   15,   16,   17,   18,   19,   20,   21,   22,
 /*  3360 */    23,   24,   25,   26,   10,   11,   12,   13,   14,   15,
 /*  3370 */    16,   17,   18,   19,   20,   21,   22,   23,   24,   25,
 /*  3380 */    26,    1,    2,    3,    4,    5,  175,  175,  175,  155,
 /*  3390 */   175,  175,  155,  175,  175,  155,   42,  163,  164,  165,
 /*  3400 */   163,  164,  165,  163,  164,  165,  175,  155,  175,  175,
 /*  3410 */   175,  175,  175,  175,  175,  163,  164,  165,  155,  175,
 /*  3420 */    40,   41,   85,   43,   44,  175,  163,  164,  165,  175,
 /*  3430 */   175,   51,  175,   10,   11,   12,   13,   14,   15,   16,
 /*  3440 */    17,   18,   19,   20,   21,   22,   23,   24,   25,   26,
 /*  3450 */   175,  175,  175,  175,  175,   32,   10,   11,   12,   13,
 /*  3460 */    14,   15,   16,   17,   18,   19,   20,   21,   22,   23,
 /*  3470 */    24,   25,   26,  175,  175,  175,  175,  175,  175,   15,
 /*  3480 */   175,   35,   10,   11,   12,   13,   14,   15,   16,   17,
 /*  3490 */    18,   19,   20,   21,   22,   23,   24,   25,   26,   10,
 /*  3500 */    11,   12,   13,   14,   15,   16,   17,   18,   19,   20,
 /*  3510 */    21,   22,   23,   24,   25,   26,   52,   53,   54,   55,
 /*  3520 */    56,   57,   58,   59,   60,   61,   62,  155,  175,  175,
 /*  3530 */   155,   42,  175,  155,  175,  163,  164,  165,  163,  164,
 /*  3540 */   165,  163,  164,  165,  175,  155,  175,  175,  175,  175,
 /*  3550 */   175,  155,  175,  163,  164,  165,  155,   85,  175,  163,
 /*  3560 */   164,  165,  175,  175,  163,  164,  165,  175,   10,   11,
 /*  3570 */    12,   13,   14,   15,   16,   17,   18,   19,   20,   21,
 /*  3580 */    22,   23,   24,   25,   26,   11,   12,   13,   14,   15,
 /*  3590 */    16,   17,   18,   19,   20,   21,   22,   23,   24,   25,
 /*  3600 */    26,   15,  175,  175,  175,  155,  175,  175,  175,  175,
 /*  3610 */   175,  175,   15,  163,  164,  165,  175,  175,   32,  175,
 /*  3620 */    34,  175,    1,    2,    3,    4,    5,  175,  175,   32,
 /*  3630 */   175,   34,  175,  175,  175,  175,  175,  175,   52,   53,
 /*  3640 */    54,   55,   56,   57,   58,   59,   60,   61,   62,   52,
 /*  3650 */    53,   54,   55,   56,   57,   58,   59,   60,   61,   62,
 /*  3660 */    15,   40,   41,  175,   43,   44,  155,  175,  175,  175,
 /*  3670 */   175,   15,   51,  175,  163,  164,  165,   32,  175,   34,
 /*  3680 */   175,    1,    2,    3,    4,    5,  155,  175,   32,  175,
 /*  3690 */    34,  175,  175,  175,  163,  164,  165,   52,   53,   54,
 /*  3700 */    55,   56,   57,   58,   59,   60,   61,   62,   52,   53,
 /*  3710 */    54,   55,   56,   57,   58,   59,   60,   61,   62,   15,
 /*  3720 */    40,   41,  175,   43,   44,  175,  175,  175,  175,  175,
 /*  3730 */   175,   51,  175,    1,    2,    3,    4,    5,   34,    1,
 /*  3740 */     2,    3,    4,    5,  155,    1,    2,    3,    4,    5,
 /*  3750 */   175,  175,  163,  164,  165,  175,   52,   53,   54,   55,
 /*  3760 */    56,   57,   58,   59,   60,   61,   62,   45,  175,  175,
 /*  3770 */   175,  175,   40,   41,  175,   43,   44,  175,   40,   41,
 /*  3780 */   175,   43,   44,   51,   40,   41,  175,   43,   44,   51,
 /*  3790 */   175,  175,  175,  175,  175,   51,  175,  175,  175,  175,
 /*  3800 */    78,   79,   80,   81,   82,  155,   84,  175,  155,   87,
 /*  3810 */    88,  175,  155,  163,  164,  165,  163,  164,  165,  175,
 /*  3820 */   163,  164,  165,  175,  175,  175,  175,  155,  175,  175,
 /*  3830 */   155,  175,  175,  155,  175,  163,  164,  165,  163,  164,
 /*  3840 */   165,  163,  164,  165,  155,  175,  175,  175,  175,  175,
 /*  3850 */   155,  175,  163,  164,  165,  155,  175,  175,  163,  164,
 /*  3860 */   165,  155,  175,  163,  164,  165,  175,  175,  175,  163,
 /*  3870 */   164,  165,
};
#define YY_SHIFT_USE_DFLT (-9)
static short yy_shift_ofst[] = {
 /*     0 */  2878,  880, 2654,   -9,   -9,   -9,   -9,   -9, 1149,  929,
 /*    10 */    -9, 1150,   -1,   -9, 1153,  951,   -9, 1581,   -9, 2484,
 /*    20 */    -9,   -9, 1854, 1105,  983, 3597,  742, 3464,  917,   -9,
 /*    30 */    -9,   -9, 1109, 1069,   -9,   -9,   -9,   -9,   -9,   -9,
 /*    40 */    -9,   -9,   -9,   -9,   -9,   -9,   -9,  462,  205, 3464,
 /*    50 */  1163,   -9,   -9, 3704,   -9, 1168, 2348,   -9,   -9,   -9,
 /*    60 */    -9,   -9,   -9,   -9,   -9, 1092, 1094,   -9, 1049, 1064,
 /*    70 */  2348,   -9, 1106, 1051, 2348,   -9,   -9,   -9,   -9,   -9,
 /*    80 */    -9,   -9,   -9,   -9,   -9,   -9, 2304, 2034,  871,  797,
 /*    90 */  3645,  654, 3464, 1161,   -9,   -9,  530,  337, 3464, 1073,
 /*   100 */    -9,   -9,   -9,   -9, 1139,  442,   -9, 1057,  338,   -9,
 /*   110 */   932,   69,   -9, 3294,   -9,  964,   -9, 3106, 3380,   -9,
 /*   120 */  1944,  810, 1159, 3586,  814,  795,   -9, 1103,   -9,   -9,
 /*   130 */    -9,   -9,   -9,   -9,   -9,   -9,   -9,   -9,   -9,   -9,
 /*   140 */    -9,   -9,   -9,   -9,   -9,   -9,   -9,   -9,   -9,   -9,
 /*   150 */   924,   -9, 1055,   -9, 3042,  967, 1411, 1367, 1018, 1053,
 /*   160 */   619,  135,   -9, 3042, 1102,  248,   -9,  336,   -9,   -9,
 /*   170 */  2348,  655, 1455, 1455,  837,  839,  927,   -9, 3042,  921,
 /*   180 */   663,  883,   -9,  978,  971, 1147, 1009, 3042, 1117,   -9,
 /*   190 */  3042,   -9, 3042,  919,   -9,  737, 3042, 3042, 3558, 3042,
 /*   200 */  3042, 3558, 3042, 3042, 3558, 3042,  194, 3042,  194, 3042,
 /*   210 */   194, 3042,  194, 3042,  194, 3042,   72, 3042,   72, 3042,
 /*   220 */    -9, 3042,   -9, 3042,   -9, 3042,   72, 3042, 2578, 3042,
 /*   230 */  2578, 3042, 3574,   -7, 3423,   -9,  901, 3042,   -9,  881,
 /*   240 */   891, 3042,  194,  425,  714,  783, 2898, 3558,  602,  463,
 /*   250 */   919,   -9, 3042,   -9,   -9,   -9,   -9,   -9,   -9,   -9,
 /*   260 */    -9,   -9, 2717, 3558,  161,  313,  771, 2616, 1095, 1104,
 /*   270 */  2868,   -9,  857,   -9, 3042,  799, 1054,  988, 2674,  222,
 /*   280 */    -9,   -9,   -9,   -9,   -9,   -9,   -9,  506,   -9,   -9,
 /*   290 */    -9, 3042, 3472,   -9, 2644,  682,   -9,   -9,  488,  600,
 /*   300 */    -9,  375,   -9, 3071,   -9,  821, 3042,   -9,   -9,   -9,
 /*   310 */   785,   -9,   -9,  382,  952,  953, 2941,  794,   -9,   -9,
 /*   320 */   996, 1004, 2840,  813,   -9,   -9,  194,  194, 3251,   -9,
 /*   330 */   225,   -9,  194, 3558,  974,  517, 2464, 3354, 1455,   91,
 /*   340 */    -9,  162,  796,   -9,  162,   -9, 2739,   -9,   -9,   -9,
 /*   350 */    -9,   -9,   -9, 3042,   -9, 3558,  793, 3722, 3042,   -9,
 /*   360 */  2492, 1585, 3042,   -9, 1084, 1585, 3042,   -9, 3337,   -9,
 /*   370 */    -9,  567, 1077, 1585, 3042,   -9, 1067, 1585, 3042,   -9,
 /*   380 */  1066, 1494, 3042,   -9, 2746, 1585, 3042,   -9,  979, 1585,
 /*   390 */  3042,   -9, 2522, 1585, 3042,   -9,  937, 1585, 3042,   -9,
 /*   400 */    -9,   -9,   -9, 3042,  777,   -9, 3042,   -9, 3558,   -9,
 /*   410 */   962,   -9,  942,   -9,  941,   -9, 2970, 3446,   -9,   -9,
 /*   420 */   889,  121,  886,  884,   -9, 3042, 3165,  879,   -9, 3042,
 /*   430 */    46,   -9,  416,  738,   -9,  416,   -9,  756, 2348,   -9,
 /*   440 */    -9,  416,  733,   -9,  416,  645,   -9,  416,  635,   -9,
 /*   450 */   416,  591,   -9,  416,  564,   -9,  416,  513,   -9,  416,
 /*   460 */   547,   -9,  416,  386,   -9,  416,  254,   -9,  213, 1138,
 /*   470 */   771,   -8,  783,   -9,   -9, 3042, 3182, 1455,  399,   -9,
 /*   480 */  1022, 1024, 2999, 3268, 1455, 1015,   -9, 3042, 3489, 1455,
 /*   490 */  1191,   -9, 1144, 3042, 1016,   -9,   -9,   -9,  556, 1455,
 /*   500 */  1455,   -9,   -9,  899,  890,  575,  267,   -9,   -9, 3464,
 /*   510 */  1083,  355,   -9,  487,   -9,  330,  242,  311,   -9,  223,
 /*   520 */    -9, 3464, 1085,  751,   -9,  707,   -9, 2304, 2214, 1120,
 /*   530 */  1122, 3656,  574,  179,   -9,   50,   -9, 3464, 1155,  531,
 /*   540 */    -9,  443,   -9,  662,  696, 1323,   -9, 1279,   -9, 3464,
 /*   550 */   495, 1235,   -9, 1059,   -9, 1490, 3621,   -9,   -9, 2124,
 /*   560 */   824,  816, 2348,  843,   -9,  841,  855, 2348,  874,   -9,
 /*   570 */    -9, 2124, 1674,  483,   -9, 2348,  571,   -9,  923,   -9,
 /*   580 */    39,  933,  461,  943,  259,   -9,   -9,  969,   -9,   -9,
 /*   590 */   976,   -9, 1764, 2625,   -9, 2348,  747,   -9, 1020,   -9,
 /*   600 */  1031,   -9,   -9, 3169, 3680, 3744, 2304, 3732,   -9, 3738,
 /*   610 */   932,   -9,   -9,   -9,  932,   69,   -9,  295,  651,  618,
 /*   620 */    -9,  842,  882,   -9,  932,   69,   -9,  972, 1042,  828,
 /*   630 */    -9, 1183,  882,   -9,   -9,
};
#define YY_REDUCE_USE_DFLT (-114)
static short yy_reduce_ofst[] = {
 /*     0 */   -91, -114, -100, -114, -114, -114, -114, -114, -114, -114,
 /*    10 */  -114, -114,  625, -114, -114,  826, -114, 1598, -114,  482,
 /*    20 */  -114, -114,  558, -114, -114,  -31, -114,  352, -114, -114,
 /*    30 */  -114, -114, -114, -114, -114, -114, -114, -114, -114, -114,
 /*    40 */  -114, -114, -114, -114, -114, -114, -114, -114, -114,  221,
 /*    50 */  -114, -114, -114, -113, -114, -114,  922, -114, -114, -114,
 /*    60 */  -114, -114, -114, -114, -114, -114, -114, -114, -114, -114,
 /*    70 */   394, -114, -114, -114,  274, -114, -114, -114, -114, -114,
 /*    80 */  -114, -114, -114, -114, -114, -114,  199,  558, -114, -114,
 /*    90 */   437, -114,  484, -114, -114, -114, -114, -114,   73, -114,
 /*   100 */  -114, -114, -114, -114, -114,  858, -114, -114, 1025, -114,
 /*   110 */   325,  120, -114,  -86, -114, -114, -114, 2273,  -85, -114,
 /*   120 */   558, -114, -114,  -52, -114, 1464, -114, 2319, -114, -114,
 /*   130 */  -114, -114, -114, -114, -114, -114, -114, -114, -114, -114,
 /*   140 */  -114, -114, -114, -114, -114, -114, -114, -114, -114, -114,
 /*   150 */  -114, -114, -114, -114, 2729, -114, 1554, 2319, -114, -114,
 /*   160 */  1869, 2319, -114, 2699, -114,  339, -114, 1061, -114, -114,
 /*   170 */   526, -114, 2184, 2319, -114, 2274, 2319, -114, 2978, -114,
 /*   180 */  1509, 2319, -114, -114, 1599, 2319, -114,  109, -114, -114,
 /*   190 */  3695, -114, 3700,  844, -114, -114, 2964, 2996, -114, 3093,
 /*   200 */  3151, -114, 3162, 3166, -114, 3178, -114, 3234, -114, 3237,
 /*   210 */  -114, 3240, -114, 3252, -114, 3263, -114, 3372, -114, 3375,
 /*   220 */  -114, 3378, -114, 3390, -114, 3396, -114, 3401, -114, 3450,
 /*   230 */  -114, 2285, -114,  275, -114, -114, -114, 3531, -114, -114,
 /*   240 */  -114, 3653, -114, -114, -114, -114, 2627, -114, -114, -114,
 /*   250 */   -16, -114, 3657, -114, -114, -114, -114, -114, -114, -114,
 /*   260 */  -114, -114, 3064, -114, -114, -114, -114, 2695, -114, -114,
 /*   270 */  2008, -114, -114, -114, 3080, -114, -114, -114, 3076, -114,
 /*   280 */  -114, -114, -114, -114, -114, -114, -114, -114, -114, -114,
 /*   290 */  -114, 3689, -114, -114, 2951, -114, -114, -114, -114, -114,
 /*   300 */  -114, -114, -114, 2187, -114, -114, 2381, -114, -114, -114,
 /*   310 */  -114, -114, -114, -114, -114, -114, 3148, -114, -114, -114,
 /*   320 */  -114, -114, 2471, -114, -114, -114, -114, -114, -114, -114,
 /*   330 */  -114, -114, -114, -114, -114, -114, 3706, -114, 1644, 2319,
 /*   340 */  -114,  415, -114, -114,  792, -114,  717, -114, -114, -114,
 /*   350 */  -114, -114, -114, 2673, -114, -114, -114, 1030, 1745, -114,
 /*   360 */  2240, 1027,  -27, -114, -114, 1006, 2474, -114, -114, -114,
 /*   370 */  -114, -114, -114,  977, 2897, -114, -114,  970, 1655, -114,
 /*   380 */  -114,  940, 1835, -114, 2418,  907, 2353, -114, -114,  894,
 /*   390 */  1925, -114,  231,  888, 1476, -114, -114,  863, 1565, -114,
 /*   400 */  -114, -114, -114,  143, -114, -114, 2105, -114, -114, -114,
 /*   410 */  -114, -114, -114, -114, -114, -114, 3511, -114, -114, -114,
 /*   420 */  -114, -114, -114, -114, -114, 3589, -114, -114, -114, 3650,
 /*   430 */  -114, -114,  609, -114, -114,  667, -114, -114,  628, -114,
 /*   440 */  -114,  546, -114, -114,  536, -114, -114,  477, -114, -114,
 /*   450 */   414, -114, -114,  389, -114, -114,  282, -114, -114,   82,
 /*   460 */  -114, -114,    8, -114, -114,  -37, -114, -114, -114, -114,
 /*   470 */  -114, -114, -114, -114, -114, 3672, -114, 1416, 2319, -114,
 /*   480 */  -114, -114, 3675, -114, 2139, 2319, -114, 3678, -114, 1689,
 /*   490 */  2319, -114, -114,   21, -114, -114, -114, -114, -114, 2229,
 /*   500 */  2319, -114, -114, -114, -114, 1779, 2319, -114, -114,  601,
 /*   510 */  -114, 1734, -114, 2319, -114, -114, -114, 1824, -114, 2319,
 /*   520 */  -114,  -12, -114, 1914, -114, 2319, -114,    2,  558, -114,
 /*   530 */  -114,  569, -114, 1959, -114, 2319, -114,  528, -114, 2004,
 /*   540 */  -114, 2319, -114, -114, -114, 2049, -114, 2319, -114,  396,
 /*   550 */  -114, 2094, -114, 2319, -114,  219,  -85, -114, -114,    2,
 /*   560 */  -114, -114,  758, -114, -114, -114, -114,  789, -114, -114,
 /*   570 */  -114,  232,  558,  802, -114,  825,  831, -114, -114, -114,
 /*   580 */   636, -114, -114, -114,  866, -114, -114, -114, -114, -114,
 /*   590 */  -114, -114,  558,  920, -114,  928,  934, -114, -114, -114,
 /*   600 */  -114, -114, -114,   43,  307,  -85,  232,  -85, -114,  -85,
 /*   610 */  1074, -114, -114, -114,  633, 1113, -114, -114, -114,  727,
 /*   620 */  -114, -114,  812, -114,  678,  887, -114, -114, -114, 1033,
 /*   630 */  -114, -114, 1070, -114, -114,
};
static YYACTIONTYPE yy_default[] = {
 /*     0 */   933,  933,  933,  637,  639,  640,  641,  642,  933,  933,
 /*    10 */   643,  933,  933,  644,  933,  933,  645,  933,  660,  933,
 /*    20 */   661,  691,  933,  933,  933,  933,  933,  933,  933,  711,
 /*    30 */   729,  730,  933,  933,  739,  740,  741,  742,  743,  744,
 /*    40 */   745,  746,  747,  748,  749,  750,  715,  933,  933,  933,
 /*    50 */   933,  712,  716,  933,  731,  733,  933,  736,  921,  922,
 /*    60 */   923,  924,  925,  926,  927,  933,  933,  928,  933,  734,
 /*    70 */   933,  737,  933,  735,  933,  738,  732,  719,  721,  722,
 /*    80 */   723,  724,  725,  726,  727,  728,  933,  933,  933,  933,
 /*    90 */   933,  933,  933,  933,  713,  717,  933,  933,  933,  933,
 /*   100 */   714,  718,  720,  692,  933,  933,  646,  933,  933,  647,
 /*   110 */   933,  933,  649,  933,  655,  933,  656,  933,  933,  689,
 /*   120 */   933,  933,  933,  933,  933,  933,  695,  933,  697,  751,
 /*   130 */   753,  754,  755,  756,  757,  758,  759,  760,  761,  762,
 /*   140 */   763,  764,  765,  766,  767,  768,  769,  770,  771,  772,
 /*   150 */   933,  773,  933,  774,  933,  933,  933,  933,  777,  933,
 /*   160 */   933,  933,  778,  933,  933,  933,  781,  933,  782,  783,
 /*   170 */   933,  933,  785,  786,  933,  933,  933,  789,  933,  933,
 /*   180 */   933,  933,  791,  933,  933,  933,  933,  933,  933,  793,
 /*   190 */   933,  850,  933,  933,  851,  933,  933,  933,  852,  933,
 /*   200 */   933,  853,  933,  933,  854,  933,  858,  933,  859,  933,
 /*   210 */   860,  933,  861,  933,  862,  933,  870,  933,  871,  933,
 /*   220 */   872,  933,  873,  933,  874,  933,  875,  933,  876,  933,
 /*   230 */   877,  933,  878,  933,  933,  863,  933,  933,  864,  933,
 /*   240 */   933,  933,  865,  882,  933,  866,  933,  912,  933,  933,
 /*   250 */   933,  879,  933,  880,  881,  883,  884,  885,  886,  887,
 /*   260 */   888,  889,  933,  920,  882,  933,  867,  933,  933,  892,
 /*   270 */   933,  893,  933,  894,  933,  933,  933,  933,  933,  933,
 /*   280 */   897,  898,  907,  908,  909,  911,  910,  933,  899,  900,
 /*   290 */   868,  933,  933,  869,  933,  933,  895,  896,  883,  884,
 /*   300 */   890,  933,  891,  933,  913,  933,  933,  915,  916,  914,
 /*   310 */   933,  901,  902,  933,  933,  933,  933,  933,  903,  904,
 /*   320 */   933,  933,  933,  933,  905,  906,  857,  856,  933,  929,
 /*   330 */   933,  930,  855,  931,  933,  933,  933,  933,  933,  933,
 /*   340 */   794,  933,  933,  798,  933,  799,  933,  801,  802,  803,
 /*   350 */   804,  805,  806,  933,  807,  849,  933,  933,  933,  808,
 /*   360 */   933,  933,  933,  811,  933,  933,  933,  812,  933,  818,
 /*   370 */   819,  933,  933,  933,  933,  809,  933,  933,  933,  810,
 /*   380 */   933,  933,  933,  813,  933,  933,  933,  814,  933,  933,
 /*   390 */   933,  815,  933,  933,  933,  816,  933,  933,  933,  817,
 /*   400 */   820,  821,  800,  933,  933,  823,  933,  824,  826,  825,
 /*   410 */   933,  827,  933,  828,  933,  829,  933,  933,  830,  831,
 /*   420 */   933,  933,  933,  933,  832,  933,  933,  933,  833,  933,
 /*   430 */   933,  834,  933,  933,  835,  933,  845,  847,  933,  848,
 /*   440 */   846,  933,  933,  836,  933,  933,  837,  933,  933,  838,
 /*   450 */   933,  933,  839,  933,  933,  840,  933,  933,  841,  933,
 /*   460 */   933,  842,  933,  933,  843,  933,  933,  844,  933,  933,
 /*   470 */   933,  933,  933,  932,  752,  933,  933,  933,  933,  795,
 /*   480 */   933,  933,  933,  933,  933,  933,  796,  933,  933,  933,
 /*   490 */   933,  797,  933,  933,  933,  792,  790,  788,  933,  933,
 /*   500 */   787,  784,  779,  775,  933,  933,  933,  780,  776,  933,
 /*   510 */   933,  933,  703,  933,  705,  933,  933,  933,  696,  933,
 /*   520 */   698,  933,  933,  933,  704,  933,  706,  933,  933,  933,
 /*   530 */   933,  933,  933,  933,  699,  933,  701,  933,  933,  933,
 /*   540 */   707,  933,  709,  933,  933,  933,  700,  933,  702,  933,
 /*   550 */   933,  933,  708,  933,  710,  933,  933,  687,  690,  933,
 /*   560 */   933,  933,  933,  933,  693,  933,  933,  933,  933,  694,
 /*   570 */   671,  933,  933,  933,  673,  933,  933,  675,  933,  679,
 /*   580 */   933,  933,  933,  933,  933,  683,  685,  933,  686,  684,
 /*   590 */   933,  677,  933,  933,  674,  933,  933,  676,  933,  680,
 /*   600 */   933,  678,  688,  933,  933,  933,  933,  933,  672,  933,
 /*   610 */   933,  657,  659,  658,  933,  933,  648,  933,  933,  933,
 /*   620 */   650,  933,  933,  651,  933,  933,  652,  933,  933,  933,
 /*   630 */   653,  933,  933,  654,  638,
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
  "xx_loop_statement",  "xx_echo_statement",  "xx_return_statement",  "xx_fcall_statement",
  "xx_mcall_statement",  "xx_scall_statement",  "xx_unset_statement",  "xx_throw_statement",
  "xx_declare_statement",  "xx_break_statement",  "xx_continue_statement",  "xx_while_statement",
  "xx_do_while_statement",  "xx_switch_statement",  "xx_for_statement",  "xx_empty_statement",
  "xx_eval_expr",  "xx_case_clauses",  "xx_case_clause",  "xx_common_expr",
  "xx_let_assignments",  "xx_let_assignment",  "xx_assignment_operator",  "xx_assign_expr",
  "xx_index_expr",  "xx_echo_expressions",  "xx_echo_expression",  "xx_mcall_expr",
  "xx_fcall_expr",  "xx_scall_expr",  "xx_declare_variable_list",  "xx_declare_variable",
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
 /* 123 */ "xx_statement ::= xx_fcall_statement",
 /* 124 */ "xx_statement ::= xx_mcall_statement",
 /* 125 */ "xx_statement ::= xx_scall_statement",
 /* 126 */ "xx_statement ::= xx_unset_statement",
 /* 127 */ "xx_statement ::= xx_throw_statement",
 /* 128 */ "xx_statement ::= xx_declare_statement",
 /* 129 */ "xx_statement ::= xx_break_statement",
 /* 130 */ "xx_statement ::= xx_continue_statement",
 /* 131 */ "xx_statement ::= xx_while_statement",
 /* 132 */ "xx_statement ::= xx_do_while_statement",
 /* 133 */ "xx_statement ::= xx_switch_statement",
 /* 134 */ "xx_statement ::= xx_for_statement",
 /* 135 */ "xx_statement ::= xx_comment",
 /* 136 */ "xx_statement ::= xx_empty_statement",
 /* 137 */ "xx_empty_statement ::= DOTCOMMA",
 /* 138 */ "xx_break_statement ::= BREAK DOTCOMMA",
 /* 139 */ "xx_continue_statement ::= CONTINUE DOTCOMMA",
 /* 140 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN BRACKET_CLOSE",
 /* 141 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN BRACKET_CLOSE ELSE BRACKET_OPEN BRACKET_CLOSE",
 /* 142 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 143 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE ELSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 144 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE ELSE BRACKET_OPEN BRACKET_CLOSE",
 /* 145 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN BRACKET_CLOSE ELSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 146 */ "xx_switch_statement ::= SWITCH xx_eval_expr BRACKET_OPEN BRACKET_CLOSE",
 /* 147 */ "xx_switch_statement ::= SWITCH xx_eval_expr BRACKET_OPEN xx_case_clauses BRACKET_CLOSE",
 /* 148 */ "xx_case_clauses ::= xx_case_clauses xx_case_clause",
 /* 149 */ "xx_case_clauses ::= xx_case_clause",
 /* 150 */ "xx_case_clause ::= CASE xx_literal_expr COLON",
 /* 151 */ "xx_case_clause ::= CASE xx_literal_expr COLON xx_statement_list",
 /* 152 */ "xx_case_clause ::= DEFAULT COLON xx_statement_list",
 /* 153 */ "xx_loop_statement ::= LOOP BRACKET_OPEN BRACKET_CLOSE",
 /* 154 */ "xx_loop_statement ::= LOOP BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 155 */ "xx_while_statement ::= WHILE xx_eval_expr BRACKET_OPEN BRACKET_CLOSE",
 /* 156 */ "xx_while_statement ::= WHILE xx_eval_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 157 */ "xx_do_while_statement ::= DO BRACKET_OPEN BRACKET_CLOSE WHILE xx_eval_expr DOTCOMMA",
 /* 158 */ "xx_do_while_statement ::= DO BRACKET_OPEN xx_statement_list BRACKET_CLOSE WHILE xx_eval_expr DOTCOMMA",
 /* 159 */ "xx_for_statement ::= FOR IDENTIFIER IN xx_common_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 160 */ "xx_for_statement ::= FOR IDENTIFIER IN REVERSE xx_common_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 161 */ "xx_for_statement ::= FOR IDENTIFIER COMMA IDENTIFIER IN xx_common_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 162 */ "xx_for_statement ::= FOR IDENTIFIER COMMA IDENTIFIER IN REVERSE xx_common_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 163 */ "xx_let_statement ::= LET xx_let_assignments DOTCOMMA",
 /* 164 */ "xx_let_assignments ::= xx_let_assignments COMMA xx_let_assignment",
 /* 165 */ "xx_let_assignments ::= xx_let_assignment",
 /* 166 */ "xx_assignment_operator ::= ASSIGN",
 /* 167 */ "xx_assignment_operator ::= ADDASSIGN",
 /* 168 */ "xx_assignment_operator ::= SUBASSIGN",
 /* 169 */ "xx_assignment_operator ::= MULASSIGN",
 /* 170 */ "xx_assignment_operator ::= DIVASSIGN",
 /* 171 */ "xx_assignment_operator ::= CONCATASSIGN",
 /* 172 */ "xx_let_assignment ::= IDENTIFIER xx_assignment_operator xx_assign_expr",
 /* 173 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER xx_assignment_operator xx_assign_expr",
 /* 174 */ "xx_let_assignment ::= IDENTIFIER ARROW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 175 */ "xx_let_assignment ::= IDENTIFIER ARROW BRACKET_OPEN STRING BRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 176 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER SBRACKET_OPEN SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 177 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER SBRACKET_OPEN xx_index_expr SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 178 */ "xx_let_assignment ::= IDENTIFIER DOUBLECOLON IDENTIFIER xx_assignment_operator xx_assign_expr",
 /* 179 */ "xx_let_assignment ::= IDENTIFIER DOUBLECOLON IDENTIFIER SBRACKET_OPEN SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 180 */ "xx_let_assignment ::= IDENTIFIER DOUBLECOLON IDENTIFIER SBRACKET_OPEN xx_index_expr SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 181 */ "xx_let_assignment ::= IDENTIFIER SBRACKET_OPEN SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 182 */ "xx_let_assignment ::= IDENTIFIER SBRACKET_OPEN xx_index_expr SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 183 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER INCR",
 /* 184 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER DECR",
 /* 185 */ "xx_let_assignment ::= IDENTIFIER INCR",
 /* 186 */ "xx_let_assignment ::= IDENTIFIER DECR",
 /* 187 */ "xx_index_expr ::= xx_common_expr",
 /* 188 */ "xx_echo_statement ::= ECHO xx_echo_expressions DOTCOMMA",
 /* 189 */ "xx_echo_expressions ::= xx_echo_expressions COMMA xx_echo_expression",
 /* 190 */ "xx_echo_expressions ::= xx_echo_expression",
 /* 191 */ "xx_echo_expression ::= xx_common_expr",
 /* 192 */ "xx_mcall_statement ::= xx_mcall_expr DOTCOMMA",
 /* 193 */ "xx_fcall_statement ::= xx_fcall_expr DOTCOMMA",
 /* 194 */ "xx_scall_statement ::= xx_scall_expr DOTCOMMA",
 /* 195 */ "xx_return_statement ::= RETURN xx_common_expr DOTCOMMA",
 /* 196 */ "xx_return_statement ::= RETURN DOTCOMMA",
 /* 197 */ "xx_unset_statement ::= UNSET IDENTIFIER ARROW IDENTIFIER DOTCOMMA",
 /* 198 */ "xx_unset_statement ::= UNSET IDENTIFIER SBRACKET_OPEN xx_common_expr SBRACKET_CLOSE DOTCOMMA",
 /* 199 */ "xx_throw_statement ::= THROW xx_common_expr DOTCOMMA",
 /* 200 */ "xx_declare_statement ::= TYPE_INTEGER xx_declare_variable_list DOTCOMMA",
 /* 201 */ "xx_declare_statement ::= TYPE_UINTEGER xx_declare_variable_list DOTCOMMA",
 /* 202 */ "xx_declare_statement ::= TYPE_CHAR xx_declare_variable_list DOTCOMMA",
 /* 203 */ "xx_declare_statement ::= TYPE_UCHAR xx_declare_variable_list DOTCOMMA",
 /* 204 */ "xx_declare_statement ::= TYPE_LONG xx_declare_variable_list DOTCOMMA",
 /* 205 */ "xx_declare_statement ::= TYPE_ULONG xx_declare_variable_list DOTCOMMA",
 /* 206 */ "xx_declare_statement ::= TYPE_DOUBLE xx_declare_variable_list DOTCOMMA",
 /* 207 */ "xx_declare_statement ::= TYPE_STRING xx_declare_variable_list DOTCOMMA",
 /* 208 */ "xx_declare_statement ::= TYPE_BOOL xx_declare_variable_list DOTCOMMA",
 /* 209 */ "xx_declare_statement ::= TYPE_VAR xx_declare_variable_list DOTCOMMA",
 /* 210 */ "xx_declare_variable_list ::= xx_declare_variable_list COMMA xx_declare_variable",
 /* 211 */ "xx_declare_variable_list ::= xx_declare_variable",
 /* 212 */ "xx_declare_variable ::= IDENTIFIER",
 /* 213 */ "xx_declare_variable ::= IDENTIFIER ASSIGN xx_literal_expr",
 /* 214 */ "xx_assign_expr ::= xx_common_expr",
 /* 215 */ "xx_common_expr ::= NOT xx_common_expr",
 /* 216 */ "xx_common_expr ::= ISSET xx_isset_expr",
 /* 217 */ "xx_common_expr ::= REQUIRE xx_common_expr",
 /* 218 */ "xx_common_expr ::= CLONE xx_common_expr",
 /* 219 */ "xx_common_expr ::= EMPTY xx_common_expr",
 /* 220 */ "xx_common_expr ::= xx_common_expr EQUALS xx_common_expr",
 /* 221 */ "xx_common_expr ::= xx_common_expr NOTEQUALS xx_common_expr",
 /* 222 */ "xx_common_expr ::= xx_common_expr IDENTICAL xx_common_expr",
 /* 223 */ "xx_common_expr ::= xx_common_expr NOTIDENTICAL xx_common_expr",
 /* 224 */ "xx_common_expr ::= xx_common_expr LESS xx_common_expr",
 /* 225 */ "xx_common_expr ::= xx_common_expr GREATER xx_common_expr",
 /* 226 */ "xx_common_expr ::= xx_common_expr LESSEQUAL xx_common_expr",
 /* 227 */ "xx_common_expr ::= xx_common_expr GREATEREQUAL xx_common_expr",
 /* 228 */ "xx_common_expr ::= PARENTHESES_OPEN xx_common_expr PARENTHESES_CLOSE",
 /* 229 */ "xx_common_expr ::= PARENTHESES_OPEN xx_parameter_type PARENTHESES_CLOSE xx_common_expr",
 /* 230 */ "xx_common_expr ::= LESS IDENTIFIER GREATER xx_common_expr",
 /* 231 */ "xx_common_expr ::= IDENTIFIER ARROW IDENTIFIER",
 /* 232 */ "xx_common_expr ::= IDENTIFIER DOUBLECOLON IDENTIFIER",
 /* 233 */ "xx_common_expr ::= IDENTIFIER DOUBLECOLON CONSTANT",
 /* 234 */ "xx_common_expr ::= IDENTIFIER SBRACKET_OPEN xx_common_expr SBRACKET_CLOSE",
 /* 235 */ "xx_common_expr ::= xx_common_expr ADD xx_common_expr",
 /* 236 */ "xx_common_expr ::= xx_common_expr SUB xx_common_expr",
 /* 237 */ "xx_common_expr ::= xx_common_expr MUL xx_common_expr",
 /* 238 */ "xx_common_expr ::= xx_common_expr DIV xx_common_expr",
 /* 239 */ "xx_common_expr ::= xx_common_expr MOD xx_common_expr",
 /* 240 */ "xx_common_expr ::= xx_common_expr CONCAT xx_common_expr",
 /* 241 */ "xx_common_expr ::= xx_common_expr AND xx_common_expr",
 /* 242 */ "xx_common_expr ::= xx_common_expr OR xx_common_expr",
 /* 243 */ "xx_common_expr ::= xx_common_expr INSTANCEOF xx_common_expr",
 /* 244 */ "xx_common_expr ::= FETCH IDENTIFIER COMMA xx_isset_expr",
 /* 245 */ "xx_common_expr ::= TYPEOF xx_common_expr",
 /* 246 */ "xx_common_expr ::= CONSTANT",
 /* 247 */ "xx_common_expr ::= IDENTIFIER",
 /* 248 */ "xx_common_expr ::= INTEGER",
 /* 249 */ "xx_common_expr ::= STRING",
 /* 250 */ "xx_common_expr ::= CHAR",
 /* 251 */ "xx_common_expr ::= DOUBLE",
 /* 252 */ "xx_common_expr ::= NULL",
 /* 253 */ "xx_common_expr ::= TRUE",
 /* 254 */ "xx_common_expr ::= FALSE",
 /* 255 */ "xx_common_expr ::= SBRACKET_OPEN SBRACKET_CLOSE",
 /* 256 */ "xx_common_expr ::= SBRACKET_OPEN xx_array_list SBRACKET_CLOSE",
 /* 257 */ "xx_common_expr ::= NEW IDENTIFIER",
 /* 258 */ "xx_common_expr ::= NEW IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 259 */ "xx_common_expr ::= NEW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 260 */ "xx_fcall_expr ::= IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 261 */ "xx_fcall_expr ::= IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 262 */ "xx_fcall_expr ::= BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 263 */ "xx_fcall_expr ::= BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 264 */ "xx_scall_expr ::= IDENTIFIER DOUBLECOLON IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 265 */ "xx_scall_expr ::= IDENTIFIER DOUBLECOLON IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 266 */ "xx_mcall_expr ::= IDENTIFIER ARROW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 267 */ "xx_mcall_expr ::= IDENTIFIER ARROW IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 268 */ "xx_mcall_expr ::= IDENTIFIER ARROW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 269 */ "xx_mcall_expr ::= IDENTIFIER ARROW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 270 */ "xx_mcall_expr ::= IDENTIFIER ARROW BRACKET_OPEN STRING BRACKET_CLOSE PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 271 */ "xx_mcall_expr ::= IDENTIFIER ARROW BRACKET_OPEN STRING BRACKET_CLOSE PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 272 */ "xx_common_expr ::= xx_mcall_expr",
 /* 273 */ "xx_common_expr ::= xx_scall_expr",
 /* 274 */ "xx_common_expr ::= xx_fcall_expr",
 /* 275 */ "xx_call_parameters ::= xx_call_parameters COMMA xx_call_parameter",
 /* 276 */ "xx_call_parameters ::= xx_call_parameter",
 /* 277 */ "xx_call_parameter ::= xx_common_expr",
 /* 278 */ "xx_array_list ::= xx_array_list COMMA xx_array_item",
 /* 279 */ "xx_array_list ::= xx_array_item",
 /* 280 */ "xx_array_item ::= xx_array_key COLON xx_array_value",
 /* 281 */ "xx_array_item ::= xx_array_value",
 /* 282 */ "xx_array_key ::= IDENTIFIER",
 /* 283 */ "xx_array_key ::= STRING",
 /* 284 */ "xx_array_key ::= INTEGER",
 /* 285 */ "xx_array_value ::= xx_common_expr",
 /* 286 */ "xx_literal_expr ::= INTEGER",
 /* 287 */ "xx_literal_expr ::= CHAR",
 /* 288 */ "xx_literal_expr ::= STRING",
 /* 289 */ "xx_literal_expr ::= DOUBLE",
 /* 290 */ "xx_literal_expr ::= NULL",
 /* 291 */ "xx_literal_expr ::= FALSE",
 /* 292 */ "xx_literal_expr ::= TRUE",
 /* 293 */ "xx_literal_expr ::= IDENTIFIER DOUBLECOLON CONSTANT",
 /* 294 */ "xx_isset_expr ::= IDENTIFIER SBRACKET_OPEN xx_common_expr SBRACKET_CLOSE",
 /* 295 */ "xx_isset_expr ::= IDENTIFIER ARROW IDENTIFIER",
 /* 296 */ "xx_eval_expr ::= xx_common_expr",
 /* 297 */ "xx_comment ::= COMMENT",
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
// 936 "parser.lemon"
{
	/*if ((yypminor->yy0)) {
		if ((yypminor->yy0)->free_flag) {
			efree((yypminor->yy0)->token);
		}
		efree((yypminor->yy0));
	}*/
}
// 2556 "parser.c"
      break;
    case 101:
// 949 "parser.lemon"
{ json_object_put((yypminor->yy175)); }
// 2561 "parser.c"
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
  { 151, 1 },
  { 145, 2 },
  { 146, 2 },
  { 135, 4 },
  { 135, 7 },
  { 135, 5 },
  { 135, 9 },
  { 135, 8 },
  { 135, 8 },
  { 149, 4 },
  { 149, 5 },
  { 153, 2 },
  { 153, 1 },
  { 154, 3 },
  { 154, 4 },
  { 154, 3 },
  { 136, 3 },
  { 136, 4 },
  { 147, 4 },
  { 147, 5 },
  { 148, 6 },
  { 148, 7 },
  { 150, 7 },
  { 150, 8 },
  { 150, 9 },
  { 150, 10 },
  { 134, 3 },
  { 156, 3 },
  { 156, 1 },
  { 158, 1 },
  { 158, 1 },
  { 158, 1 },
  { 158, 1 },
  { 158, 1 },
  { 158, 1 },
  { 157, 3 },
  { 157, 5 },
  { 157, 7 },
  { 157, 7 },
  { 157, 7 },
  { 157, 8 },
  { 157, 5 },
  { 157, 7 },
  { 157, 8 },
  { 157, 5 },
  { 157, 6 },
  { 157, 4 },
  { 157, 4 },
  { 157, 2 },
  { 157, 2 },
  { 160, 1 },
  { 137, 3 },
  { 161, 3 },
  { 161, 1 },
  { 162, 1 },
  { 140, 2 },
  { 139, 2 },
  { 141, 2 },
  { 138, 3 },
  { 138, 2 },
  { 142, 5 },
  { 142, 6 },
  { 143, 3 },
  { 144, 3 },
  { 144, 3 },
  { 144, 3 },
  { 144, 3 },
  { 144, 3 },
  { 144, 3 },
  { 144, 3 },
  { 144, 3 },
  { 144, 3 },
  { 144, 3 },
  { 166, 3 },
  { 166, 1 },
  { 167, 1 },
  { 167, 3 },
  { 159, 1 },
  { 155, 2 },
  { 155, 2 },
  { 155, 2 },
  { 155, 2 },
  { 155, 2 },
  { 155, 3 },
  { 155, 3 },
  { 155, 3 },
  { 155, 3 },
  { 155, 3 },
  { 155, 3 },
  { 155, 3 },
  { 155, 3 },
  { 155, 3 },
  { 155, 4 },
  { 155, 4 },
  { 155, 3 },
  { 155, 3 },
  { 155, 3 },
  { 155, 4 },
  { 155, 3 },
  { 155, 3 },
  { 155, 3 },
  { 155, 3 },
  { 155, 3 },
  { 155, 3 },
  { 155, 3 },
  { 155, 3 },
  { 155, 3 },
  { 155, 4 },
  { 155, 2 },
  { 155, 1 },
  { 155, 1 },
  { 155, 1 },
  { 155, 1 },
  { 155, 1 },
  { 155, 1 },
  { 155, 1 },
  { 155, 1 },
  { 155, 1 },
  { 155, 2 },
  { 155, 3 },
  { 155, 2 },
  { 155, 4 },
  { 155, 5 },
  { 164, 4 },
  { 164, 3 },
  { 164, 6 },
  { 164, 5 },
  { 165, 6 },
  { 165, 5 },
  { 163, 6 },
  { 163, 5 },
  { 163, 8 },
  { 163, 7 },
  { 163, 8 },
  { 163, 7 },
  { 155, 1 },
  { 155, 1 },
  { 155, 1 },
  { 170, 3 },
  { 170, 1 },
  { 171, 1 },
  { 169, 3 },
  { 169, 1 },
  { 172, 3 },
  { 172, 1 },
  { 173, 1 },
  { 173, 1 },
  { 173, 1 },
  { 174, 1 },
  { 119, 1 },
  { 119, 1 },
  { 119, 1 },
  { 119, 1 },
  { 119, 1 },
  { 119, 1 },
  { 119, 1 },
  { 119, 3 },
  { 168, 4 },
  { 168, 3 },
  { 152, 1 },
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
// 945 "parser.lemon"
{
	status->ret = yymsp[0].minor.yy175;
}
// 3076 "parser.c"
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
      case 187:
      case 214:
      case 272:
      case 273:
      case 274:
      case 277:
      case 285:
      case 296:
// 951 "parser.lemon"
{
	yygotominor.yy175 = yymsp[0].minor.yy175;
}
// 3116 "parser.c"
        break;
      case 2:
      case 36:
      case 52:
      case 54:
      case 56:
      case 84:
      case 116:
      case 148:
// 955 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_list(yymsp[-1].minor.yy175, yymsp[0].minor.yy175);
}
// 3130 "parser.c"
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
      case 149:
      case 165:
      case 190:
      case 211:
      case 276:
      case 279:
// 959 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_list(NULL, yymsp[0].minor.yy175);
}
// 3152 "parser.c"
        break;
      case 8:
// 979 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_namespace(yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(33,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3161 "parser.c"
        break;
      case 9:
// 983 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_interface(yymsp[-1].minor.yy0, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(36,&yymsp[-2].minor);
}
// 3169 "parser.c"
        break;
      case 10:
// 987 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_interface(yymsp[-3].minor.yy0, yymsp[0].minor.yy175, yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(36,&yymsp[-4].minor);
  yy_destructor(37,&yymsp[-2].minor);
}
// 3178 "parser.c"
        break;
      case 11:
// 991 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class(yymsp[-1].minor.yy0, yymsp[0].minor.yy175, 0, 0, NULL, NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-2].minor);
}
// 3186 "parser.c"
        break;
      case 12:
// 995 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy175, 0, 0, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-4].minor);
  yy_destructor(37,&yymsp[-2].minor);
}
// 3195 "parser.c"
        break;
      case 13:
// 999 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy175, 0, 0, NULL, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(38,&yymsp[-4].minor);
  yy_destructor(39,&yymsp[-2].minor);
}
// 3204 "parser.c"
        break;
      case 14:
// 1003 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class(yymsp[-5].minor.yy0, yymsp[0].minor.yy175, 0, 0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(38,&yymsp[-6].minor);
  yy_destructor(37,&yymsp[-4].minor);
  yy_destructor(39,&yymsp[-2].minor);
}
// 3214 "parser.c"
        break;
      case 15:
// 1007 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class(yymsp[-1].minor.yy0, yymsp[0].minor.yy175, 1, 0, NULL, NULL, status->scanner_state);
  yy_destructor(40,&yymsp[-3].minor);
  yy_destructor(38,&yymsp[-2].minor);
}
// 3223 "parser.c"
        break;
      case 16:
// 1011 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy175, 1, 0, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(40,&yymsp[-5].minor);
  yy_destructor(38,&yymsp[-4].minor);
  yy_destructor(37,&yymsp[-2].minor);
}
// 3233 "parser.c"
        break;
      case 17:
// 1015 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy175, 1, 0, NULL, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(40,&yymsp[-5].minor);
  yy_destructor(38,&yymsp[-4].minor);
  yy_destructor(39,&yymsp[-2].minor);
}
// 3243 "parser.c"
        break;
      case 18:
// 1019 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class(yymsp[-1].minor.yy0, yymsp[0].minor.yy175, 0, 1, NULL, NULL, status->scanner_state);
  yy_destructor(41,&yymsp[-3].minor);
  yy_destructor(38,&yymsp[-2].minor);
}
// 3252 "parser.c"
        break;
      case 19:
// 1023 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy175, 0, 1, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(41,&yymsp[-5].minor);
  yy_destructor(38,&yymsp[-4].minor);
  yy_destructor(37,&yymsp[-2].minor);
}
// 3262 "parser.c"
        break;
      case 20:
      case 25:
      case 46:
// 1027 "parser.lemon"
{
	yygotominor.yy175 = NULL;
  yy_destructor(42,&yymsp[-1].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3273 "parser.c"
        break;
      case 21:
      case 26:
      case 47:
// 1031 "parser.lemon"
{
	yygotominor.yy175 = yymsp[-1].minor.yy175;
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3284 "parser.c"
        break;
      case 22:
      case 48:
      case 96:
      case 164:
      case 189:
      case 210:
      case 275:
      case 278:
// 1035 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_list(yymsp[-2].minor.yy175, yymsp[0].minor.yy175);
  yy_destructor(6,&yymsp[-1].minor);
}
// 3299 "parser.c"
        break;
      case 24:
      case 247:
      case 282:
// 1043 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state);
}
// 3308 "parser.c"
        break;
      case 27:
// 1055 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_definition(yymsp[0].minor.yy175, NULL, NULL, status->scanner_state);
}
// 3315 "parser.c"
        break;
      case 28:
// 1059 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_definition(NULL, NULL, yymsp[0].minor.yy175, status->scanner_state);
}
// 3322 "parser.c"
        break;
      case 29:
// 1063 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_definition(NULL, yymsp[0].minor.yy175, NULL, status->scanner_state);
}
// 3329 "parser.c"
        break;
      case 30:
// 1067 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_definition(yymsp[-1].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
}
// 3336 "parser.c"
        break;
      case 31:
// 1071 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_definition(yymsp[-1].minor.yy175, NULL, yymsp[0].minor.yy175, status->scanner_state);
}
// 3343 "parser.c"
        break;
      case 32:
// 1075 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_definition(yymsp[0].minor.yy175, NULL, yymsp[-1].minor.yy175, status->scanner_state);
}
// 3350 "parser.c"
        break;
      case 33:
// 1079 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_definition(NULL, yymsp[0].minor.yy175, yymsp[-1].minor.yy175, status->scanner_state);
}
// 3357 "parser.c"
        break;
      case 34:
// 1083 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_definition(yymsp[-2].minor.yy175, yymsp[0].minor.yy175, yymsp[-1].minor.yy175, status->scanner_state);
}
// 3364 "parser.c"
        break;
      case 35:
// 1087 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_definition(yymsp[-1].minor.yy175, yymsp[0].minor.yy175, yymsp[-2].minor.yy175, status->scanner_state);
}
// 3371 "parser.c"
        break;
      case 38:
// 1104 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_property(yymsp[-2].minor.yy175, yymsp[-1].minor.yy0, NULL, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(35,&yymsp[0].minor);
}
// 3379 "parser.c"
        break;
      case 39:
// 1108 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_property(yymsp[-2].minor.yy175, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(35,&yymsp[0].minor);
}
// 3387 "parser.c"
        break;
      case 40:
// 1112 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_property(yymsp[-4].minor.yy175, yymsp[-3].minor.yy0, yymsp[-1].minor.yy175, yymsp[-5].minor.yy0, NULL, status->scanner_state);
  yy_destructor(45,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3396 "parser.c"
        break;
      case 41:
// 1116 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_property(yymsp[-4].minor.yy175, yymsp[-3].minor.yy0, yymsp[-1].minor.yy175, NULL, NULL, status->scanner_state);
  yy_destructor(45,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3405 "parser.c"
        break;
      case 42:
// 1120 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_property(yymsp[-3].minor.yy175, yymsp[-2].minor.yy0, NULL, yymsp[-4].minor.yy0, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(35,&yymsp[0].minor);
}
// 3413 "parser.c"
        break;
      case 43:
// 1124 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_property(yymsp[-3].minor.yy175, yymsp[-2].minor.yy0, NULL, NULL, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(35,&yymsp[0].minor);
}
// 3421 "parser.c"
        break;
      case 44:
// 1128 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_property(yymsp[-5].minor.yy175, yymsp[-4].minor.yy0, yymsp[-2].minor.yy175, yymsp[-6].minor.yy0, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(45,&yymsp[-3].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3430 "parser.c"
        break;
      case 45:
// 1132 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_property(yymsp[-5].minor.yy175, yymsp[-4].minor.yy0, yymsp[-2].minor.yy175, NULL, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(45,&yymsp[-3].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3439 "parser.c"
        break;
      case 50:
// 1152 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_property_shortcut(NULL, yymsp[0].minor.yy0, status->scanner_state);
}
// 3446 "parser.c"
        break;
      case 51:
// 1156 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_property_shortcut(yymsp[-1].minor.yy0, yymsp[0].minor.yy0, status->scanner_state);
}
// 3453 "parser.c"
        break;
      case 58:
// 1185 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy175, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(46,&yymsp[-4].minor);
  yy_destructor(45,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3463 "parser.c"
        break;
      case 59:
// 1189 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy175, NULL, status->scanner_state);
  yy_destructor(46,&yymsp[-4].minor);
  yy_destructor(45,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3473 "parser.c"
        break;
      case 60:
// 1194 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(yymsp[-6].minor.yy175, yymsp[-4].minor.yy0, NULL, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(48,&yymsp[-5].minor);
  yy_destructor(49,&yymsp[-3].minor);
  yy_destructor(32,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[-1].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3485 "parser.c"
        break;
      case 61:
// 1198 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(yymsp[-7].minor.yy175, yymsp[-5].minor.yy0, yymsp[-3].minor.yy175, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(48,&yymsp[-6].minor);
  yy_destructor(49,&yymsp[-4].minor);
  yy_destructor(32,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[-1].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3497 "parser.c"
        break;
      case 62:
// 1202 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(yymsp[-7].minor.yy175, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy175, NULL, NULL, status->scanner_state);
  yy_destructor(48,&yymsp[-6].minor);
  yy_destructor(49,&yymsp[-4].minor);
  yy_destructor(32,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3509 "parser.c"
        break;
      case 63:
// 1206 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(yymsp[-8].minor.yy175, yymsp[-6].minor.yy0, yymsp[-4].minor.yy175, yymsp[-1].minor.yy175, NULL, NULL, status->scanner_state);
  yy_destructor(48,&yymsp[-7].minor);
  yy_destructor(49,&yymsp[-5].minor);
  yy_destructor(32,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3521 "parser.c"
        break;
      case 64:
// 1210 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(yymsp[-6].minor.yy175, yymsp[-4].minor.yy0, NULL, NULL, yymsp[-7].minor.yy0, NULL, status->scanner_state);
  yy_destructor(48,&yymsp[-5].minor);
  yy_destructor(49,&yymsp[-3].minor);
  yy_destructor(32,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[-1].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3533 "parser.c"
        break;
      case 65:
// 1214 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(yymsp[-7].minor.yy175, yymsp[-5].minor.yy0, yymsp[-3].minor.yy175, NULL, yymsp[-8].minor.yy0, NULL, status->scanner_state);
  yy_destructor(48,&yymsp[-6].minor);
  yy_destructor(49,&yymsp[-4].minor);
  yy_destructor(32,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[-1].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3545 "parser.c"
        break;
      case 66:
// 1218 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(yymsp[-7].minor.yy175, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy175, yymsp[-8].minor.yy0, NULL, status->scanner_state);
  yy_destructor(48,&yymsp[-6].minor);
  yy_destructor(49,&yymsp[-4].minor);
  yy_destructor(32,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3557 "parser.c"
        break;
      case 67:
// 1222 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(yymsp[-8].minor.yy175, yymsp[-6].minor.yy0, yymsp[-4].minor.yy175, yymsp[-1].minor.yy175, yymsp[-9].minor.yy0, NULL, status->scanner_state);
  yy_destructor(48,&yymsp[-7].minor);
  yy_destructor(49,&yymsp[-5].minor);
  yy_destructor(32,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3569 "parser.c"
        break;
      case 68:
// 1226 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(yymsp[-8].minor.yy175, yymsp[-6].minor.yy0, NULL, NULL, NULL, yymsp[-2].minor.yy175, status->scanner_state);
  yy_destructor(48,&yymsp[-7].minor);
  yy_destructor(49,&yymsp[-5].minor);
  yy_destructor(32,&yymsp[-4].minor);
  yy_destructor(50,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-1].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3582 "parser.c"
        break;
      case 69:
// 1230 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(yymsp[-9].minor.yy175, yymsp[-7].minor.yy0, yymsp[-5].minor.yy175, NULL, NULL, yymsp[-2].minor.yy175, status->scanner_state);
  yy_destructor(48,&yymsp[-8].minor);
  yy_destructor(49,&yymsp[-6].minor);
  yy_destructor(32,&yymsp[-4].minor);
  yy_destructor(50,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-1].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3595 "parser.c"
        break;
      case 70:
// 1234 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(yymsp[-9].minor.yy175, yymsp[-7].minor.yy0, NULL, yymsp[-1].minor.yy175, NULL, yymsp[-3].minor.yy175, status->scanner_state);
  yy_destructor(48,&yymsp[-8].minor);
  yy_destructor(49,&yymsp[-6].minor);
  yy_destructor(32,&yymsp[-5].minor);
  yy_destructor(50,&yymsp[-4].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3608 "parser.c"
        break;
      case 71:
// 1238 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(yymsp[-10].minor.yy175, yymsp[-8].minor.yy0, yymsp[-6].minor.yy175, yymsp[-1].minor.yy175, NULL, yymsp[-3].minor.yy175, status->scanner_state);
  yy_destructor(48,&yymsp[-9].minor);
  yy_destructor(49,&yymsp[-7].minor);
  yy_destructor(32,&yymsp[-5].minor);
  yy_destructor(50,&yymsp[-4].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3621 "parser.c"
        break;
      case 72:
// 1242 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(yymsp[-8].minor.yy175, yymsp[-6].minor.yy0, NULL, NULL, yymsp[-9].minor.yy0, yymsp[-2].minor.yy175, status->scanner_state);
  yy_destructor(48,&yymsp[-7].minor);
  yy_destructor(49,&yymsp[-5].minor);
  yy_destructor(32,&yymsp[-4].minor);
  yy_destructor(50,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-1].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3634 "parser.c"
        break;
      case 73:
// 1246 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(yymsp[-9].minor.yy175, yymsp[-7].minor.yy0, yymsp[-5].minor.yy175, NULL, yymsp[-10].minor.yy0, yymsp[-2].minor.yy175, status->scanner_state);
  yy_destructor(48,&yymsp[-8].minor);
  yy_destructor(49,&yymsp[-6].minor);
  yy_destructor(32,&yymsp[-4].minor);
  yy_destructor(50,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-1].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3647 "parser.c"
        break;
      case 74:
// 1250 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(yymsp[-9].minor.yy175, yymsp[-7].minor.yy0, NULL, yymsp[-1].minor.yy175, yymsp[-10].minor.yy0, yymsp[-3].minor.yy175, status->scanner_state);
  yy_destructor(48,&yymsp[-8].minor);
  yy_destructor(49,&yymsp[-6].minor);
  yy_destructor(32,&yymsp[-5].minor);
  yy_destructor(50,&yymsp[-4].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3660 "parser.c"
        break;
      case 75:
// 1254 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(yymsp[-10].minor.yy175, yymsp[-8].minor.yy0, yymsp[-6].minor.yy175, yymsp[-1].minor.yy175, yymsp[-11].minor.yy0, yymsp[-3].minor.yy175, status->scanner_state);
  yy_destructor(48,&yymsp[-9].minor);
  yy_destructor(49,&yymsp[-7].minor);
  yy_destructor(32,&yymsp[-5].minor);
  yy_destructor(50,&yymsp[-4].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3673 "parser.c"
        break;
      case 76:
// 1258 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(yymsp[-7].minor.yy175, yymsp[-5].minor.yy0, NULL, NULL, NULL, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(48,&yymsp[-6].minor);
  yy_destructor(49,&yymsp[-4].minor);
  yy_destructor(32,&yymsp[-3].minor);
  yy_destructor(50,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3685 "parser.c"
        break;
      case 77:
// 1262 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(yymsp[-8].minor.yy175, yymsp[-6].minor.yy0, yymsp[-4].minor.yy175, NULL, NULL, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(48,&yymsp[-7].minor);
  yy_destructor(49,&yymsp[-5].minor);
  yy_destructor(32,&yymsp[-3].minor);
  yy_destructor(50,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3697 "parser.c"
        break;
      case 78:
// 1266 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(yymsp[-7].minor.yy175, yymsp[-5].minor.yy0, NULL, NULL, yymsp[-8].minor.yy0, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(48,&yymsp[-6].minor);
  yy_destructor(49,&yymsp[-4].minor);
  yy_destructor(32,&yymsp[-3].minor);
  yy_destructor(50,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3709 "parser.c"
        break;
      case 79:
// 1270 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(yymsp[-8].minor.yy175, yymsp[-6].minor.yy0, yymsp[-4].minor.yy175, NULL, yymsp[-9].minor.yy0, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(48,&yymsp[-7].minor);
  yy_destructor(49,&yymsp[-5].minor);
  yy_destructor(32,&yymsp[-3].minor);
  yy_destructor(50,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3721 "parser.c"
        break;
      case 80:
// 1275 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(yymsp[-5].minor.yy175, yymsp[-3].minor.yy0, NULL, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(48,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[-1].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3732 "parser.c"
        break;
      case 81:
// 1279 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(yymsp[-6].minor.yy175, yymsp[-4].minor.yy0, yymsp[-2].minor.yy175, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(48,&yymsp[-5].minor);
  yy_destructor(49,&yymsp[-3].minor);
  yy_destructor(32,&yymsp[-1].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3743 "parser.c"
        break;
      case 82:
// 1283 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(yymsp[-5].minor.yy175, yymsp[-3].minor.yy0, NULL, NULL, yymsp[-6].minor.yy0, NULL, status->scanner_state);
  yy_destructor(48,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[-1].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3754 "parser.c"
        break;
      case 83:
// 1287 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(yymsp[-6].minor.yy175, yymsp[-4].minor.yy0, yymsp[-2].minor.yy175, NULL, yymsp[-7].minor.yy0, NULL, status->scanner_state);
  yy_destructor(48,&yymsp[-5].minor);
  yy_destructor(49,&yymsp[-3].minor);
  yy_destructor(32,&yymsp[-1].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3765 "parser.c"
        break;
      case 86:
// 1300 "parser.lemon"
{
	yygotominor.yy175 = json_object_new_string("public");
  yy_destructor(1,&yymsp[0].minor);
}
// 3773 "parser.c"
        break;
      case 87:
// 1304 "parser.lemon"
{
	yygotominor.yy175 = json_object_new_string("protected");
  yy_destructor(2,&yymsp[0].minor);
}
// 3781 "parser.c"
        break;
      case 88:
// 1308 "parser.lemon"
{
	yygotominor.yy175 = json_object_new_string("private");
  yy_destructor(4,&yymsp[0].minor);
}
// 3789 "parser.c"
        break;
      case 89:
// 1312 "parser.lemon"
{
	yygotominor.yy175 = json_object_new_string("static");
  yy_destructor(3,&yymsp[0].minor);
}
// 3797 "parser.c"
        break;
      case 90:
// 1316 "parser.lemon"
{
	yygotominor.yy175 = json_object_new_string("scoped");
  yy_destructor(5,&yymsp[0].minor);
}
// 3805 "parser.c"
        break;
      case 91:
// 1320 "parser.lemon"
{
	yygotominor.yy175 = json_object_new_string("inline");
  yy_destructor(51,&yymsp[0].minor);
}
// 3813 "parser.c"
        break;
      case 92:
// 1324 "parser.lemon"
{
	yygotominor.yy175 = json_object_new_string("abstract");
  yy_destructor(40,&yymsp[0].minor);
}
// 3821 "parser.c"
        break;
      case 93:
// 1328 "parser.lemon"
{
	yygotominor.yy175 = json_object_new_string("final");
  yy_destructor(41,&yymsp[0].minor);
}
// 3829 "parser.c"
        break;
      case 98:
// 1350 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_parameter(NULL, NULL, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 3836 "parser.c"
        break;
      case 99:
// 1354 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_parameter(yymsp[-1].minor.yy175, NULL, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 3843 "parser.c"
        break;
      case 100:
// 1358 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_parameter(NULL, yymsp[-1].minor.yy175, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 3850 "parser.c"
        break;
      case 101:
// 1362 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_parameter(NULL, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(45,&yymsp[-1].minor);
}
// 3858 "parser.c"
        break;
      case 102:
// 1366 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_parameter(yymsp[-3].minor.yy175, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(45,&yymsp[-1].minor);
}
// 3866 "parser.c"
        break;
      case 103:
// 1370 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_parameter(NULL, yymsp[-3].minor.yy175, yymsp[-2].minor.yy0, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(45,&yymsp[-1].minor);
}
// 3874 "parser.c"
        break;
      case 104:
// 1374 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(15,&yymsp[-2].minor);
  yy_destructor(16,&yymsp[0].minor);
}
// 3883 "parser.c"
        break;
      case 105:
// 1378 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_type(XX_TYPE_INTEGER);
  yy_destructor(52,&yymsp[0].minor);
}
// 3891 "parser.c"
        break;
      case 106:
// 1382 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_type(XX_TYPE_UINTEGER);
  yy_destructor(53,&yymsp[0].minor);
}
// 3899 "parser.c"
        break;
      case 107:
// 1386 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_type(XX_TYPE_LONG);
  yy_destructor(54,&yymsp[0].minor);
}
// 3907 "parser.c"
        break;
      case 108:
// 1390 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_type(XX_TYPE_ULONG);
  yy_destructor(55,&yymsp[0].minor);
}
// 3915 "parser.c"
        break;
      case 109:
// 1394 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_type(XX_TYPE_CHAR);
  yy_destructor(56,&yymsp[0].minor);
}
// 3923 "parser.c"
        break;
      case 110:
// 1398 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_type(XX_TYPE_UCHAR);
  yy_destructor(57,&yymsp[0].minor);
}
// 3931 "parser.c"
        break;
      case 111:
// 1402 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_type(XX_TYPE_DOUBLE);
  yy_destructor(58,&yymsp[0].minor);
}
// 3939 "parser.c"
        break;
      case 112:
// 1406 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_type(XX_TYPE_BOOL);
  yy_destructor(59,&yymsp[0].minor);
}
// 3947 "parser.c"
        break;
      case 113:
// 1410 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_type(XX_TYPE_STRING);
  yy_destructor(60,&yymsp[0].minor);
}
// 3955 "parser.c"
        break;
      case 114:
// 1414 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_type(XX_TYPE_ARRAY);
  yy_destructor(61,&yymsp[0].minor);
}
// 3963 "parser.c"
        break;
      case 115:
// 1418 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_type(XX_TYPE_VAR);
  yy_destructor(62,&yymsp[0].minor);
}
// 3971 "parser.c"
        break;
      case 137:
// 1506 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_empty_statement(status->scanner_state);
  yy_destructor(35,&yymsp[0].minor);
}
// 3979 "parser.c"
        break;
      case 138:
// 1510 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_break_statement(status->scanner_state);
  yy_destructor(63,&yymsp[-1].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3988 "parser.c"
        break;
      case 139:
// 1514 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_continue_statement(status->scanner_state);
  yy_destructor(64,&yymsp[-1].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3997 "parser.c"
        break;
      case 140:
// 1518 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_if_statement(yymsp[-2].minor.yy175, NULL, NULL, status->scanner_state);
  yy_destructor(65,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-1].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 4007 "parser.c"
        break;
      case 141:
// 1522 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_if_statement(yymsp[-5].minor.yy175, NULL, NULL, status->scanner_state);
  yy_destructor(65,&yymsp[-6].minor);
  yy_destructor(42,&yymsp[-4].minor);
  yy_destructor(43,&yymsp[-3].minor);
  yy_destructor(66,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[-1].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 4020 "parser.c"
        break;
      case 142:
// 1526 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_if_statement(yymsp[-3].minor.yy175, yymsp[-1].minor.yy175, NULL, status->scanner_state);
  yy_destructor(65,&yymsp[-4].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 4030 "parser.c"
        break;
      case 143:
// 1530 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_if_statement(yymsp[-7].minor.yy175, yymsp[-5].minor.yy175, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(65,&yymsp[-8].minor);
  yy_destructor(42,&yymsp[-6].minor);
  yy_destructor(43,&yymsp[-4].minor);
  yy_destructor(66,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 4043 "parser.c"
        break;
      case 144:
// 1534 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_if_statement(yymsp[-6].minor.yy175, yymsp[-4].minor.yy175, NULL, status->scanner_state);
  yy_destructor(65,&yymsp[-7].minor);
  yy_destructor(42,&yymsp[-5].minor);
  yy_destructor(43,&yymsp[-3].minor);
  yy_destructor(66,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[-1].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 4056 "parser.c"
        break;
      case 145:
// 1538 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_if_statement(yymsp[-6].minor.yy175, NULL, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(65,&yymsp[-7].minor);
  yy_destructor(42,&yymsp[-5].minor);
  yy_destructor(43,&yymsp[-4].minor);
  yy_destructor(66,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 4069 "parser.c"
        break;
      case 146:
// 1542 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_switch_statement(yymsp[-2].minor.yy175, NULL, status->scanner_state);
  yy_destructor(67,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-1].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 4079 "parser.c"
        break;
      case 147:
// 1546 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_switch_statement(yymsp[-3].minor.yy175, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(67,&yymsp[-4].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 4089 "parser.c"
        break;
      case 150:
// 1558 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_case_clause(yymsp[-1].minor.yy175, NULL, status->scanner_state);
  yy_destructor(68,&yymsp[-2].minor);
  yy_destructor(69,&yymsp[0].minor);
}
// 4098 "parser.c"
        break;
      case 151:
// 1562 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_case_clause(yymsp[-2].minor.yy175, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(68,&yymsp[-3].minor);
  yy_destructor(69,&yymsp[-1].minor);
}
// 4107 "parser.c"
        break;
      case 152:
// 1566 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_case_clause(NULL, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(70,&yymsp[-2].minor);
  yy_destructor(69,&yymsp[-1].minor);
}
// 4116 "parser.c"
        break;
      case 153:
// 1570 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_loop_statement(NULL, status->scanner_state);
  yy_destructor(71,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[-1].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 4126 "parser.c"
        break;
      case 154:
// 1574 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_loop_statement(yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(71,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 4136 "parser.c"
        break;
      case 155:
// 1578 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_while_statement(yymsp[-2].minor.yy175, NULL, status->scanner_state);
  yy_destructor(72,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-1].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 4146 "parser.c"
        break;
      case 156:
// 1582 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_while_statement(yymsp[-3].minor.yy175, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(72,&yymsp[-4].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 4156 "parser.c"
        break;
      case 157:
// 1586 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_do_while_statement(yymsp[-1].minor.yy175, NULL, status->scanner_state);
  yy_destructor(73,&yymsp[-5].minor);
  yy_destructor(42,&yymsp[-4].minor);
  yy_destructor(43,&yymsp[-3].minor);
  yy_destructor(72,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4168 "parser.c"
        break;
      case 158:
// 1590 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_do_while_statement(yymsp[-1].minor.yy175, yymsp[-4].minor.yy175, status->scanner_state);
  yy_destructor(73,&yymsp[-6].minor);
  yy_destructor(42,&yymsp[-5].minor);
  yy_destructor(43,&yymsp[-3].minor);
  yy_destructor(72,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4180 "parser.c"
        break;
      case 159:
// 1594 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_for_statement(yymsp[-3].minor.yy175, NULL, yymsp[-5].minor.yy0, 0, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(74,&yymsp[-6].minor);
  yy_destructor(75,&yymsp[-4].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 4191 "parser.c"
        break;
      case 160:
// 1598 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_for_statement(yymsp[-3].minor.yy175, NULL, yymsp[-6].minor.yy0, 1, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(74,&yymsp[-7].minor);
  yy_destructor(75,&yymsp[-5].minor);
  yy_destructor(76,&yymsp[-4].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 4203 "parser.c"
        break;
      case 161:
// 1602 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_for_statement(yymsp[-3].minor.yy175, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, 0, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(74,&yymsp[-8].minor);
  yy_destructor(6,&yymsp[-6].minor);
  yy_destructor(75,&yymsp[-4].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 4215 "parser.c"
        break;
      case 162:
// 1606 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_for_statement(yymsp[-3].minor.yy175, yymsp[-8].minor.yy0, yymsp[-6].minor.yy0, 1, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(74,&yymsp[-9].minor);
  yy_destructor(6,&yymsp[-7].minor);
  yy_destructor(75,&yymsp[-5].minor);
  yy_destructor(76,&yymsp[-4].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 4228 "parser.c"
        break;
      case 163:
// 1610 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_statement(yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(77,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4237 "parser.c"
        break;
      case 166:
// 1623 "parser.lemon"
{
	yygotominor.yy175 = json_object_new_string("assign");
  yy_destructor(45,&yymsp[0].minor);
}
// 4245 "parser.c"
        break;
      case 167:
// 1628 "parser.lemon"
{
	yygotominor.yy175 = json_object_new_string("add-assign");
  yy_destructor(78,&yymsp[0].minor);
}
// 4253 "parser.c"
        break;
      case 168:
// 1633 "parser.lemon"
{
	yygotominor.yy175 = json_object_new_string("sub-assign");
  yy_destructor(79,&yymsp[0].minor);
}
// 4261 "parser.c"
        break;
      case 169:
// 1637 "parser.lemon"
{
	yygotominor.yy175 = json_object_new_string("mult-assign");
  yy_destructor(80,&yymsp[0].minor);
}
// 4269 "parser.c"
        break;
      case 170:
// 1641 "parser.lemon"
{
	yygotominor.yy175 = json_object_new_string("div-assign");
  yy_destructor(81,&yymsp[0].minor);
}
// 4277 "parser.c"
        break;
      case 171:
// 1645 "parser.lemon"
{
	yygotominor.yy175 = json_object_new_string("concat-assign");
  yy_destructor(82,&yymsp[0].minor);
}
// 4285 "parser.c"
        break;
      case 172:
// 1650 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("variable", yymsp[-1].minor.yy175, yymsp[-2].minor.yy0, NULL, NULL, yymsp[0].minor.yy175, status->scanner_state);
}
// 4292 "parser.c"
        break;
      case 173:
// 1655 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("object-property", yymsp[-1].minor.yy175, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(50,&yymsp[-3].minor);
}
// 4300 "parser.c"
        break;
      case 174:
// 1660 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("variable-dynamic-object-property", yymsp[-1].minor.yy175, yymsp[-6].minor.yy0, yymsp[-3].minor.yy0, NULL, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(50,&yymsp[-5].minor);
  yy_destructor(42,&yymsp[-4].minor);
  yy_destructor(43,&yymsp[-2].minor);
}
// 4310 "parser.c"
        break;
      case 175:
// 1665 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("string-dynamic-object-property", yymsp[-1].minor.yy175, yymsp[-6].minor.yy0, yymsp[-3].minor.yy0, NULL, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(50,&yymsp[-5].minor);
  yy_destructor(42,&yymsp[-4].minor);
  yy_destructor(43,&yymsp[-2].minor);
}
// 4320 "parser.c"
        break;
      case 176:
// 1670 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("object-property-append", yymsp[-1].minor.yy175, yymsp[-6].minor.yy0, yymsp[-4].minor.yy0, NULL, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(50,&yymsp[-5].minor);
  yy_destructor(84,&yymsp[-3].minor);
  yy_destructor(85,&yymsp[-2].minor);
}
// 4330 "parser.c"
        break;
      case 177:
// 1675 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("object-property-array-index", yymsp[-1].minor.yy175, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, yymsp[-3].minor.yy175, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(50,&yymsp[-6].minor);
  yy_destructor(84,&yymsp[-4].minor);
  yy_destructor(85,&yymsp[-2].minor);
}
// 4340 "parser.c"
        break;
      case 178:
// 1680 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("static-property", yymsp[-1].minor.yy175, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(86,&yymsp[-3].minor);
}
// 4348 "parser.c"
        break;
      case 179:
// 1685 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("static-property-append", yymsp[-1].minor.yy175, yymsp[-6].minor.yy0, yymsp[-4].minor.yy0, NULL, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(86,&yymsp[-5].minor);
  yy_destructor(84,&yymsp[-3].minor);
  yy_destructor(85,&yymsp[-2].minor);
}
// 4358 "parser.c"
        break;
      case 180:
// 1690 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("static-property-array-index", yymsp[-1].minor.yy175, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, yymsp[-3].minor.yy175, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(86,&yymsp[-6].minor);
  yy_destructor(84,&yymsp[-4].minor);
  yy_destructor(85,&yymsp[-2].minor);
}
// 4368 "parser.c"
        break;
      case 181:
// 1695 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("variable-append", yymsp[-1].minor.yy175, yymsp[-4].minor.yy0, NULL, NULL, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(84,&yymsp[-3].minor);
  yy_destructor(85,&yymsp[-2].minor);
}
// 4377 "parser.c"
        break;
      case 182:
// 1700 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("array-index", yymsp[-1].minor.yy175, yymsp[-5].minor.yy0, NULL, yymsp[-3].minor.yy175, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(84,&yymsp[-4].minor);
  yy_destructor(85,&yymsp[-2].minor);
}
// 4386 "parser.c"
        break;
      case 183:
// 1705 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("incr", NULL, yymsp[-3].minor.yy0, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(50,&yymsp[-2].minor);
  yy_destructor(87,&yymsp[0].minor);
}
// 4395 "parser.c"
        break;
      case 184:
// 1710 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("decr", NULL, yymsp[-3].minor.yy0, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(50,&yymsp[-2].minor);
  yy_destructor(88,&yymsp[0].minor);
}
// 4404 "parser.c"
        break;
      case 185:
// 1715 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("incr", NULL, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(87,&yymsp[0].minor);
}
// 4412 "parser.c"
        break;
      case 186:
// 1720 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("decr", NULL, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(88,&yymsp[0].minor);
}
// 4420 "parser.c"
        break;
      case 188:
// 1728 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_echo_statement(yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(89,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4429 "parser.c"
        break;
      case 191:
// 1740 "parser.lemon"
{
	yygotominor.yy175 = yymsp[0].minor.yy175;;
}
// 4436 "parser.c"
        break;
      case 192:
// 1745 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_mcall_statement(yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(35,&yymsp[0].minor);
}
// 4444 "parser.c"
        break;
      case 193:
// 1750 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_fcall_statement(yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(35,&yymsp[0].minor);
}
// 4452 "parser.c"
        break;
      case 194:
// 1755 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_scall_statement(yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(35,&yymsp[0].minor);
}
// 4460 "parser.c"
        break;
      case 195:
// 1760 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_return_statement(yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(90,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4469 "parser.c"
        break;
      case 196:
// 1765 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_return_statement(NULL, status->scanner_state);
  yy_destructor(90,&yymsp[-1].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4478 "parser.c"
        break;
      case 197:
// 1770 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(91,&yymsp[-4].minor);
  yy_destructor(50,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4492 "parser.c"
        break;
      case 198:
// 1779 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state),
		yymsp[-2].minor.yy175,
		status->scanner_state
	);
  yy_destructor(91,&yymsp[-5].minor);
  yy_destructor(84,&yymsp[-3].minor);
  yy_destructor(85,&yymsp[-1].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4507 "parser.c"
        break;
      case 199:
// 1788 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_throw_exception(yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(92,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4516 "parser.c"
        break;
      case 200:
// 1792 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_statement(XX_T_TYPE_INTEGER, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(52,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4525 "parser.c"
        break;
      case 201:
// 1796 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_statement(XX_T_TYPE_UINTEGER, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(53,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4534 "parser.c"
        break;
      case 202:
// 1800 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_statement(XX_T_TYPE_CHAR, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(56,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4543 "parser.c"
        break;
      case 203:
// 1804 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_statement(XX_T_TYPE_UCHAR, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(57,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4552 "parser.c"
        break;
      case 204:
// 1808 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_statement(XX_T_TYPE_LONG, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4561 "parser.c"
        break;
      case 205:
// 1812 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_statement(XX_T_TYPE_ULONG, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(55,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4570 "parser.c"
        break;
      case 206:
// 1816 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_statement(XX_T_TYPE_DOUBLE, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(58,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4579 "parser.c"
        break;
      case 207:
// 1820 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_statement(XX_T_TYPE_STRING, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(60,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4588 "parser.c"
        break;
      case 208:
// 1824 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_statement(XX_T_TYPE_BOOL, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(59,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4597 "parser.c"
        break;
      case 209:
// 1828 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_statement(XX_T_TYPE_VAR, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(62,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4606 "parser.c"
        break;
      case 212:
// 1840 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_variable(yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 4613 "parser.c"
        break;
      case 213:
// 1844 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_variable(yymsp[-2].minor.yy0, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(45,&yymsp[-1].minor);
}
// 4621 "parser.c"
        break;
      case 215:
// 1852 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("not", yymsp[0].minor.yy175, NULL, NULL, status->scanner_state);
  yy_destructor(31,&yymsp[-1].minor);
}
// 4629 "parser.c"
        break;
      case 216:
// 1856 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("isset", yymsp[0].minor.yy175, NULL, NULL, status->scanner_state);
  yy_destructor(27,&yymsp[-1].minor);
}
// 4637 "parser.c"
        break;
      case 217:
// 1860 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("require", yymsp[0].minor.yy175, NULL, NULL, status->scanner_state);
  yy_destructor(7,&yymsp[-1].minor);
}
// 4645 "parser.c"
        break;
      case 218:
// 1864 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("clone", yymsp[0].minor.yy175, NULL, NULL, status->scanner_state);
  yy_destructor(8,&yymsp[-1].minor);
}
// 4653 "parser.c"
        break;
      case 219:
// 1868 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("empty", yymsp[0].minor.yy175, NULL, NULL, status->scanner_state);
  yy_destructor(9,&yymsp[-1].minor);
}
// 4661 "parser.c"
        break;
      case 220:
// 1872 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("equals", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(13,&yymsp[-1].minor);
}
// 4669 "parser.c"
        break;
      case 221:
// 1876 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("not-equals", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(20,&yymsp[-1].minor);
}
// 4677 "parser.c"
        break;
      case 222:
// 1880 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("identical", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(14,&yymsp[-1].minor);
}
// 4685 "parser.c"
        break;
      case 223:
// 1884 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("not-identical", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(19,&yymsp[-1].minor);
}
// 4693 "parser.c"
        break;
      case 224:
// 1888 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("less", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(15,&yymsp[-1].minor);
}
// 4701 "parser.c"
        break;
      case 225:
// 1892 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("greater", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(16,&yymsp[-1].minor);
}
// 4709 "parser.c"
        break;
      case 226:
// 1896 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("less-equal", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(17,&yymsp[-1].minor);
}
// 4717 "parser.c"
        break;
      case 227:
// 1900 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("greater-equal", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(18,&yymsp[-1].minor);
}
// 4725 "parser.c"
        break;
      case 228:
// 1904 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("list", yymsp[-1].minor.yy175, NULL, NULL, status->scanner_state);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 4734 "parser.c"
        break;
      case 229:
// 1908 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("cast", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(49,&yymsp[-3].minor);
  yy_destructor(32,&yymsp[-1].minor);
}
// 4743 "parser.c"
        break;
      case 230:
// 1912 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("type-hint", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(15,&yymsp[-3].minor);
  yy_destructor(16,&yymsp[-1].minor);
}
// 4752 "parser.c"
        break;
      case 231:
      case 295:
// 1916 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("property-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(50,&yymsp[-1].minor);
}
// 4761 "parser.c"
        break;
      case 232:
// 1920 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("static-property-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(86,&yymsp[-1].minor);
}
// 4769 "parser.c"
        break;
      case 233:
      case 293:
// 1924 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("static-constant-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(86,&yymsp[-1].minor);
}
// 4778 "parser.c"
        break;
      case 234:
      case 294:
// 1929 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), yymsp[-1].minor.yy175, NULL, status->scanner_state);
  yy_destructor(84,&yymsp[-2].minor);
  yy_destructor(85,&yymsp[0].minor);
}
// 4788 "parser.c"
        break;
      case 235:
// 1934 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("add", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(21,&yymsp[-1].minor);
}
// 4796 "parser.c"
        break;
      case 236:
// 1939 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("sub", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(22,&yymsp[-1].minor);
}
// 4804 "parser.c"
        break;
      case 237:
// 1944 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("mul", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(24,&yymsp[-1].minor);
}
// 4812 "parser.c"
        break;
      case 238:
// 1949 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("div", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(25,&yymsp[-1].minor);
}
// 4820 "parser.c"
        break;
      case 239:
// 1954 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("mod", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(26,&yymsp[-1].minor);
}
// 4828 "parser.c"
        break;
      case 240:
// 1959 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("concat", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(23,&yymsp[-1].minor);
}
// 4836 "parser.c"
        break;
      case 241:
// 1964 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("and", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(11,&yymsp[-1].minor);
}
// 4844 "parser.c"
        break;
      case 242:
// 1969 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("or", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(12,&yymsp[-1].minor);
}
// 4852 "parser.c"
        break;
      case 243:
// 1974 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("instanceof", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(10,&yymsp[-1].minor);
}
// 4860 "parser.c"
        break;
      case 244:
// 1979 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("fetch", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(28,&yymsp[-3].minor);
  yy_destructor(6,&yymsp[-1].minor);
}
// 4869 "parser.c"
        break;
      case 245:
// 1984 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("typeof", yymsp[0].minor.yy175, NULL, NULL, status->scanner_state);
  yy_destructor(29,&yymsp[-1].minor);
}
// 4877 "parser.c"
        break;
      case 246:
// 1989 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_literal(XX_T_CONSTANT, yymsp[0].minor.yy0, status->scanner_state);
}
// 4884 "parser.c"
        break;
      case 248:
      case 284:
      case 286:
// 1999 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_literal(XX_T_INTEGER, yymsp[0].minor.yy0, status->scanner_state);
}
// 4893 "parser.c"
        break;
      case 249:
      case 283:
      case 288:
// 2004 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_literal(XX_T_STRING, yymsp[0].minor.yy0, status->scanner_state);
}
// 4902 "parser.c"
        break;
      case 250:
      case 287:
// 2009 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_literal(XX_T_CHAR, yymsp[0].minor.yy0, status->scanner_state);
}
// 4910 "parser.c"
        break;
      case 251:
      case 289:
// 2014 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_literal(XX_T_DOUBLE, yymsp[0].minor.yy0, status->scanner_state);
}
// 4918 "parser.c"
        break;
      case 252:
      case 290:
// 2019 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_literal(XX_T_NULL, NULL, status->scanner_state);
  yy_destructor(96,&yymsp[0].minor);
}
// 4927 "parser.c"
        break;
      case 253:
      case 292:
// 2024 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_literal(XX_T_TRUE, NULL, status->scanner_state);
  yy_destructor(97,&yymsp[0].minor);
}
// 4936 "parser.c"
        break;
      case 254:
      case 291:
// 2029 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_literal(XX_T_FALSE, NULL, status->scanner_state);
  yy_destructor(98,&yymsp[0].minor);
}
// 4945 "parser.c"
        break;
      case 255:
// 2034 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("empty-array", NULL, NULL, NULL, status->scanner_state);
  yy_destructor(84,&yymsp[-1].minor);
  yy_destructor(85,&yymsp[0].minor);
}
// 4954 "parser.c"
        break;
      case 256:
// 2039 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("array", yymsp[-1].minor.yy175, NULL, NULL, status->scanner_state);
  yy_destructor(84,&yymsp[-2].minor);
  yy_destructor(85,&yymsp[0].minor);
}
// 4963 "parser.c"
        break;
      case 257:
// 2044 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_new_instance(yymsp[0].minor.yy0, NULL, status->scanner_state);
  yy_destructor(30,&yymsp[-1].minor);
}
// 4971 "parser.c"
        break;
      case 258:
// 2049 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_new_instance(yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(30,&yymsp[-3].minor);
  yy_destructor(49,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 4981 "parser.c"
        break;
      case 259:
// 2054 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_new_instance(yymsp[-3].minor.yy0, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(30,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 4991 "parser.c"
        break;
      case 260:
// 2059 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_fcall(1, yymsp[-3].minor.yy0, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 5000 "parser.c"
        break;
      case 261:
// 2064 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_fcall(1, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(49,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 5009 "parser.c"
        break;
      case 262:
// 2069 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_fcall(2, yymsp[-4].minor.yy0, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(42,&yymsp[-5].minor);
  yy_destructor(43,&yymsp[-3].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 5020 "parser.c"
        break;
      case 263:
// 2074 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_fcall(2, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(42,&yymsp[-4].minor);
  yy_destructor(43,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 5031 "parser.c"
        break;
      case 264:
// 2079 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_scall(yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(86,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 5041 "parser.c"
        break;
      case 265:
// 2084 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_scall(yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(86,&yymsp[-3].minor);
  yy_destructor(49,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 5051 "parser.c"
        break;
      case 266:
// 2089 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_mcall(1, yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(50,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 5061 "parser.c"
        break;
      case 267:
// 2094 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_mcall(1, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(50,&yymsp[-3].minor);
  yy_destructor(49,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 5071 "parser.c"
        break;
      case 268:
// 2099 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_mcall(2, yymsp[-7].minor.yy0, yymsp[-4].minor.yy0, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(50,&yymsp[-6].minor);
  yy_destructor(42,&yymsp[-5].minor);
  yy_destructor(43,&yymsp[-3].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 5083 "parser.c"
        break;
      case 269:
// 2104 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_mcall(2, yymsp[-6].minor.yy0, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(50,&yymsp[-5].minor);
  yy_destructor(42,&yymsp[-4].minor);
  yy_destructor(43,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 5095 "parser.c"
        break;
      case 270:
// 2109 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_mcall(3, yymsp[-7].minor.yy0, yymsp[-4].minor.yy0, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(50,&yymsp[-6].minor);
  yy_destructor(42,&yymsp[-5].minor);
  yy_destructor(43,&yymsp[-3].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 5107 "parser.c"
        break;
      case 271:
// 2114 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_mcall(3, yymsp[-6].minor.yy0, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(50,&yymsp[-5].minor);
  yy_destructor(42,&yymsp[-4].minor);
  yy_destructor(43,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 5119 "parser.c"
        break;
      case 280:
// 2153 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_array_item(yymsp[-2].minor.yy175, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(69,&yymsp[-1].minor);
}
// 5127 "parser.c"
        break;
      case 281:
// 2157 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_array_item(NULL, yymsp[0].minor.yy175, status->scanner_state);
}
// 5134 "parser.c"
        break;
      case 297:
// 2222 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_comment(yymsp[0].minor.yy0, status->scanner_state);
}
// 5141 "parser.c"
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
// 902 "parser.lemon"


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

// 5217 "parser.c"
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
