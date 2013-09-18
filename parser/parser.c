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

static json_object *xx_ret_class_property(json_object *visibility, xx_parser_token *T, json_object *default_value, xx_parser_token *D, xx_scanner_state *state)
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
	json_object *statements, xx_parser_token *D, xx_scanner_state *state)
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

static json_object *xx_ret_fcall(xx_parser_token *F, json_object *parameters, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("fcall"));
	json_object_object_add(ret, "name", json_object_new_string(F->token));

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


// 825 "parser.c"
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
#define YYNOCODE 171
#define YYACTIONTYPE unsigned short int
#define xx_TOKENTYPE xx_parser_token*
typedef union {
  xx_TOKENTYPE yy0;
  json_object* yy115;
  int yy341;
} YYMINORTYPE;
#define YYSTACKDEPTH 100
#define xx_ARG_SDECL xx_parser_status *status;
#define xx_ARG_PDECL ,xx_parser_status *status
#define xx_ARG_FETCH xx_parser_status *status = yypParser->status
#define xx_ARG_STORE yypParser->status = status
#define YYNSTATE 570
#define YYNRULE 270
#define YYERRORSYMBOL 99
#define YYERRSYMDT yy341
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
 /*     0 */   181,  184,  187,    3,    4,    5,    6,    7,  223,  841,
 /*    10 */     1,    2,  569,    4,    5,    6,    7,  150,  143,   79,
 /*    20 */   177,  232,  236,  252,  174,  101,   88,  227,  103,  538,
 /*    30 */   544,  543,  531,  502,  537,  511,   68,   46,   50,   88,
 /*    40 */   238,  154,  217,  475,   57,   58,   59,   60,   61,   62,
 /*    50 */    63,   64,   65,   66,   67,  215,  211,  213,  176,  186,
 /*    60 */   191,  193,  195,  197,  189,  183,  199,  201,  209,  203,
 /*    70 */   205,  207,  383,  345,  570,  445,  240,  246,  373,  545,
 /*    80 */   408,  259,  261,  260,  501,  450,  239,  241,  242,  243,
 /*    90 */   244,  245,  409,  418,  427,  430,  421,  424,  433,  439,
 /*   100 */   436,  382,  442,  134,  136,  138,  533,  147,  203,  205,
 /*   110 */   207,  158,  162,  167,  311,   99,  534,  318,   10,  445,
 /*   120 */   199,  201,  209,  203,  205,  207,    9,  271,  505,  450,
 /*   130 */   380,  393,  397,  406,  324,   12,  409,  418,  427,  430,
 /*   140 */   421,  424,  433,  439,  436,   14,  442,  134,  136,  138,
 /*   150 */    17,  147,   32,   46,   50,  158,  162,  167,  311,   13,
 /*   160 */   228,  318,  268,  445,  249,  412,  325,  326,  327,  328,
 /*   170 */   329,  271,  507,  450,  380,  393,  397,  406,  410,  417,
 /*   180 */   409,  418,  427,  430,  421,  424,  433,  439,  436,  258,
 /*   190 */   442,  134,  136,  138,  411,  147,   31,  419,  417,  158,
 /*   200 */   162,  167,  311,  385,  448,  318,  449,  445,  446,  381,
 /*   210 */   386,  259,  261,  260,  290,  257,  146,  450,  380,  393,
 /*   220 */   397,  406,   86,  527,  409,  418,  427,  430,  421,  424,
 /*   230 */   433,  439,  436,  528,  442,  134,  136,  138,  560,  147,
 /*   240 */   548,  422,  417,  158,  162,  167,  311,    8,  412,  318,
 /*   250 */    11,  445,   90,   15,  552,  562,  412,  307,  450,  180,
 /*   260 */   112,  450,  380,  393,  397,  406,  425,  417,  409,  418,
 /*   270 */   427,  430,  421,  424,  433,  439,  436,  423,  442,  134,
 /*   280 */   136,  138,  399,  147,  402,  420,  566,  158,  162,  167,
 /*   290 */   311,   99,  332,  318,   85,  445,  340,   68,   46,   50,
 /*   300 */   259,  261,  260,  280,  489,  450,  380,  393,  397,  406,
 /*   310 */   428,  417,  409,  418,  427,  430,  421,  424,  433,  439,
 /*   320 */   436,   31,  442,  134,  136,  138,  412,  147,  431,  417,
 /*   330 */   334,  158,  162,  167,  311,   17,  332,  318,  348,  445,
 /*   340 */   368,  412,   16,  412,  259,  261,  260,  487,  114,  450,
 /*   350 */   380,  393,  397,  406,   27,  426,  409,  418,  427,  430,
 /*   360 */   421,  424,  433,  439,  436,   31,  442,  134,  136,  138,
 /*   370 */   429,  147,  432,  434,  417,  158,  162,  167,  311,  247,
 /*   380 */   412,  318,  279,  445,  437,  417,  250,  259,  261,  260,
 /*   390 */    69,  503,  491,  450,  380,  393,  397,  406,  284,  267,
 /*   400 */   409,  418,  427,  430,  421,  424,  433,  439,  436,  435,
 /*   410 */   442,  134,  136,  138,  412,  147,  440,  417,  291,  158,
 /*   420 */   162,  167,  311,   24,  332,  318,  486,  445,  364,   68,
 /*   430 */    46,   50,  259,  261,  260,  493,  499,  450,  380,  393,
 /*   440 */   397,  406,   88,  438,  409,  418,  427,  430,  421,  424,
 /*   450 */   433,  439,  436,   25,  442,  134,  136,  138,  412,  147,
 /*   460 */   443,  417,  349,  158,  162,  167,  311,  297,  332,  318,
 /*   470 */    28,  445,  331,   68,   46,   50,  259,  261,  260,  525,
 /*   480 */   142,  450,  380,  393,  397,  406,   88,  441,  409,  418,
 /*   490 */   427,  430,  421,  424,  433,  439,  436,   31,  442,  134,
 /*   500 */   136,  138,  412,  147,  258,   30,  457,  158,  162,  167,
 /*   510 */   311,  353,  332,  318,   34,  445,  352,  550,   35,  548,
 /*   520 */   259,  261,  260,   29,  170,  450,  380,  393,  397,  406,
 /*   530 */   288,  444,  409,  418,  427,  430,  421,  424,  433,  439,
 /*   540 */   436,  258,  442,  134,  136,  138,   97,  147,  548,  258,
 /*   550 */   229,  158,  162,  167,  311,  345,  321,  318,  290,  445,
 /*   560 */   365,  319,  379,  259,  261,  260,  152,  295,  480,  450,
 /*   570 */   380,  393,  397,  406,  313,  301,  409,  418,  427,  430,
 /*   580 */   421,  424,  433,  439,  436,  320,  442,  134,  136,  138,
 /*   590 */   154,  147,  475,  258,   44,  158,  162,  167,  311,  345,
 /*   600 */    45,  318,   47,  445,  341,  151,  478,  259,  261,  260,
 /*   610 */   258,   48,  469,  450,  380,  393,  397,  406,   49,  273,
 /*   620 */   409,  418,  427,  430,  421,  424,  433,  439,  436,   51,
 /*   630 */   442,  134,  136,  138,   52,  147,  265,   53,   56,  158,
 /*   640 */   162,  167,  311,   55,  332,  318,   96,  445,  372,   99,
 /*   650 */   650,   81,  259,  261,  260,   82,  456,  450,  380,  393,
 /*   660 */   397,  406,   84,   87,  409,  418,  427,  430,  421,  424,
 /*   670 */   433,  439,  436,   91,  442,  134,  136,  138,   94,  147,
 /*   680 */    92,   95,  547,  158,  162,  167,  311,   98,  332,  318,
 /*   690 */   102,  445,  376,  108,  109,  111,  259,  261,  260,  135,
 /*   700 */   166,  450,  380,  393,  397,  406,  137,  140,  409,  418,
 /*   710 */   427,  430,  421,  424,  433,  439,  436,  144,  442,  134,
 /*   720 */   136,  138,  149,  147,  153,  159,  156,  158,  162,  167,
 /*   730 */   311,  155,  332,  318,  164,  445,  360,  168,  171,  173,
 /*   740 */   259,  261,  260,  179,  317,  450,  380,  393,  397,  406,
 /*   750 */   221,  178,  409,  418,  427,  430,  421,  424,  433,  439,
 /*   760 */   436,  224,  442,  134,  136,  138,  225,  147,  230,  233,
 /*   770 */   234,  158,  162,  167,  311,  251,  509,  318,  106,  445,
 /*   780 */   510,  511,  235,  253,  826,   88,  254,  293,  463,  450,
 /*   790 */   380,  393,  397,  406,  292,  298,  409,  418,  427,  430,
 /*   800 */   421,  424,  433,  439,  436,  299,  442,  134,  136,  138,
 /*   810 */   825,  147,  283,  308,  312,  158,  162,  167,  311,  323,
 /*   820 */   332,  318,  330,  445,  336,  322,  342,  335,  259,  261,
 /*   830 */   260,  339,  473,  450,  380,  393,  397,  406,  343,  350,
 /*   840 */   409,  418,  427,  430,  421,  424,  433,  439,  436,  351,
 /*   850 */   442,  134,  136,  138,  354,  147,  355,  358,  359,  158,
 /*   860 */   162,  167,  311,  385,  363,  318,  366,  445,  367,  374,
 /*   870 */   384,  259,  261,  260,  371,  375,  479,  450,  380,  393,
 /*   880 */   397,  406,  388,  390,  409,  418,  427,  430,  421,  424,
 /*   890 */   433,  439,  436,  392,  442,  134,  136,  138,  398,  147,
 /*   900 */   400,  413,  401,  158,  162,  167,  311,  405,  332,  318,
 /*   910 */   414,  445,  344,  415,  416,  447,  259,  261,  260,  458,
 /*   920 */   468,  450,  380,  393,  397,  406,  459,  470,  409,  418,
 /*   930 */   427,  430,  421,  424,  433,  439,  436,  472,  442,  134,
 /*   940 */   136,  138,  476,  147,  481,  482,  488,  158,  162,  167,
 /*   950 */   311,  495,  332,  318,  496,  445,  356,  498,  504,  514,
 /*   960 */   259,  261,  260,  515,  161,  450,  380,  393,  397,  406,
 /*   970 */   516,  517,  409,  418,  427,  430,  421,  424,  433,  439,
 /*   980 */   436,  519,  442,  134,  136,  138,  520,  147,  521,  522,
 /*   990 */   529,  158,  162,  167,  311,  182,  530,  318,  536,  445,
 /*  1000 */   546,  535,  551,  259,  261,  260,  553,  554,  485,  450,
 /*  1010 */   380,  393,  397,  406,   99,  555,  409,  418,  427,  430,
 /*  1020 */   421,  424,  433,  439,  436,  557,  442,  134,  136,  138,
 /*  1030 */   563,  147,  558,  561,  564,  158,  162,  167,  311,  304,
 /*  1040 */   565,  318,  567,  445,  568,  442,  442,  259,  261,  260,
 /*  1050 */   442,  442,  474,  450,  380,  393,  397,  406,  442,  442,
 /*  1060 */   409,  418,  427,  430,  421,  424,  433,  439,  436,  442,
 /*  1070 */   442,  134,  136,  138,  442,  147,  442,  442,  442,  158,
 /*  1080 */   162,  167,  311,  185,  442,  318,  442,  445,  442,  442,
 /*  1090 */   442,  259,  261,  260,  442,  442,  484,  450,  380,  393,
 /*  1100 */   397,  406,  442,  442,  409,  418,  427,  430,  421,  424,
 /*  1110 */   433,  439,  436,  442,  442,  134,  136,  138,  442,  147,
 /*  1120 */   442,  442,  442,  158,  162,  167,  311,  139,   22,  318,
 /*  1130 */   310,  445,   21,  442,  442,   88,  442,  442,  259,  261,
 /*  1140 */   260,  450,  380,  393,  397,  406,  324,  442,  409,  418,
 /*  1150 */   427,  430,  421,  424,  433,  439,  436,  271,  442,  134,
 /*  1160 */   136,  138,  442,  147,  442,  442,  442,  158,  162,  167,
 /*  1170 */   311,  442,  442,  318,  133,  442,  824,  442,  325,  326,
 /*  1180 */   327,  328,  329,  442,  442,  361,  380,  393,  397,  406,
 /*  1190 */   228,  160,  268,  442,  249,  442,  451,  116,  117,  118,
 /*  1200 */   119,  120,  121,  122,  123,  124,  125,  126,  127,  128,
 /*  1210 */   129,  130,  131,  132,  133,   70,   71,   73,   72,   74,
 /*  1220 */   442,  163,  442,  442,  310,  387,  389,  391,  442,  442,
 /*  1230 */   442,  169,  259,  261,  260,  442,  451,  116,  117,  118,
 /*  1240 */   119,  120,  121,  122,  123,  124,  125,  126,  127,  128,
 /*  1250 */   129,  130,  131,  132,   76,   77,  133,  100,  524,  442,
 /*  1260 */   518,  231,  442,   19,   75,  387,  389,  391,   22,  259,
 /*  1270 */   261,  260,   89,  145,  442,   88,  300,  263,  451,  116,
 /*  1280 */   117,  118,  119,  120,  121,  122,  123,  124,  125,  126,
 /*  1290 */   127,  128,  129,  130,  131,  132,  133,   70,   71,   73,
 /*  1300 */    72,   74,   70,   71,   73,   72,   74,  387,  389,  391,
 /*  1310 */   556,  442,  559,  113,  442,   99,  442,  442,  451,  116,
 /*  1320 */   117,  118,  119,  120,  121,  122,  123,  124,  125,  126,
 /*  1330 */   127,  128,  129,  130,  131,  132,   76,   77,  442,  133,
 /*  1340 */   442,   76,   77,  442,  494,  226,   75,  387,  389,  391,
 /*  1350 */   303,   75,  442,  259,  261,  260,  141,  442,  259,  261,
 /*  1360 */   260,  451,  116,  117,  118,  119,  120,  121,  122,  123,
 /*  1370 */   124,  125,  126,  127,  128,  129,  130,  131,  132,  442,
 /*  1380 */   442,  133,  539,  442,  542,  543,  531,  188,  510,  511,
 /*  1390 */   387,  389,  391,   88,  442,  259,  261,  260,  506,  442,
 /*  1400 */   442,  442,  442,  451,  116,  117,  118,  119,  120,  121,
 /*  1410 */   122,  123,  124,  125,  126,  127,  128,  129,  130,  131,
 /*  1420 */   132,  133,   70,   71,   73,   72,   74,  190,  471,  442,
 /*  1430 */   442,  310,  387,  389,  391,  259,  261,  260,  157,  259,
 /*  1440 */   261,  260,  442,  451,  116,  117,  118,  119,  120,  121,
 /*  1450 */   122,  123,  124,  125,  126,  127,  128,  129,  130,  131,
 /*  1460 */   132,   76,   77,  106,  133,  442,  105,  513,  442,  231,
 /*  1470 */    88,   75,  387,  389,  391,  442,  442,  259,  261,  260,
 /*  1480 */   442,  500,  442,  442,  287,  263,  451,  116,  117,  118,
 /*  1490 */   119,  120,  121,  122,  123,  124,  125,  126,  127,  128,
 /*  1500 */   129,  130,  131,  132,  442,  442,  133,  442,  508,  104,
 /*  1510 */   523,  531,  222,  537,  511,  387,  389,  391,   88,  442,
 /*  1520 */   259,  261,  260,  490,  442,  442,  442,  442,  451,  116,
 /*  1530 */   117,  118,  119,  120,  121,  122,  123,  124,  125,  126,
 /*  1540 */   127,  128,  129,  130,  131,  132,  133,   70,   71,   73,
 /*  1550 */    72,   74,  442,  148,  442,  442,  310,  387,  389,  391,
 /*  1560 */   442,  442,  442,  483,  259,  261,  260,  442,  451,  116,
 /*  1570 */   117,  118,  119,  120,  121,  122,  123,  124,  125,  126,
 /*  1580 */   127,  128,  129,  130,  131,  132,   76,   77,   93,  133,
 /*  1590 */   549,  442,  172,   99,   80,  310,   75,  387,  389,  391,
 /*  1600 */   192,  442,  442,  259,  261,  260,  477,  442,  259,  261,
 /*  1610 */   260,  451,  116,  117,  118,  119,  120,  121,  122,  123,
 /*  1620 */   124,  125,  126,  127,  128,  129,  130,  131,  132,  231,
 /*  1630 */   442,  133,  442,  442,  442,  442,  442,  259,  261,  260,
 /*  1640 */   387,  389,  391,  442,  256,  263,  442,  442,  467,  442,
 /*  1650 */   442,  442,  442,  451,  116,  117,  118,  119,  120,  121,
 /*  1660 */   122,  123,  124,  125,  126,  127,  128,  129,  130,  131,
 /*  1670 */   132,  133,   70,   71,   73,   72,   74,  442,  442,  442,
 /*  1680 */   442,  194,  387,  389,  391,  442,  442,  442,  316,  259,
 /*  1690 */   261,  260,  442,  451,  116,  117,  118,  119,  120,  121,
 /*  1700 */   122,  123,  124,  125,  126,  127,  128,  129,  130,  131,
 /*  1710 */   132,   76,   77,  442,  133,  442,  442,  231,  442,   23,
 /*  1720 */   196,   75,  387,  389,  391,  259,  261,  260,  259,  261,
 /*  1730 */   260,  165,  442,  262,  442,  442,  451,  116,  117,  118,
 /*  1740 */   119,  120,  121,  122,  123,  124,  125,  126,  127,  128,
 /*  1750 */   129,  130,  131,  132,  442,  442,  133,  442,  442,  540,
 /*  1760 */   523,  531,  198,  442,  511,  387,  389,  391,   88,  442,
 /*  1770 */   259,  261,  260,  462,  442,  442,  442,  442,  451,  116,
 /*  1780 */   117,  118,  119,  120,  121,  122,  123,  124,  125,  126,
 /*  1790 */   127,  128,  129,  130,  131,  132,  133,   70,   71,   73,
 /*  1800 */    72,   74,  442,  442,  442,  442,  200,  387,  389,  391,
 /*  1810 */   442,  442,  442,  455,  259,  261,  260,  442,  451,  116,
 /*  1820 */   117,  118,  119,  120,  121,  122,  123,  124,  125,  126,
 /*  1830 */   127,  128,  129,  130,  131,  132,   76,   77,  442,  133,
 /*  1840 */   442,  442,  202,  442,  107,  216,   75,  387,  389,  391,
 /*  1850 */   259,  261,  260,  259,  261,  260,  442,  442,  442,  442,
 /*  1860 */   442,  115,  116,  117,  118,  119,  120,  121,  122,  123,
 /*  1870 */   124,  125,  126,  127,  128,  129,  130,  131,  132,  181,
 /*  1880 */   184,  187,  571,  442,  442,  442,  442,  223,  204,  442,
 /*  1890 */   387,  389,  391,  442,  442,  442,  259,  261,  260,  177,
 /*  1900 */   232,  236,  252,  174,  206,  442,  227,  442,  181,  184,
 /*  1910 */   187,  442,  259,  261,  260,    8,  223,  442,   11,  238,
 /*  1920 */    90,  217,  552,  562,  442,  442,  450,   43,  177,  232,
 /*  1930 */   236,  252,  174,  255,  247,  227,  442,  181,  184,  187,
 /*  1940 */   442,  442,  259,  261,  260,  223,  442,  452,  238,  220,
 /*  1950 */   217,  281,  282,  285,  442,  240,  246,  177,  232,  236,
 /*  1960 */   252,  174,  442,  442,  227,  239,  241,  242,  243,  244,
 /*  1970 */   245,  442,  218,  442,  442,  442,   38,  238,  442,  217,
 /*  1980 */   259,  261,  260,  442,  240,  246,   36,   37,   39,   40,
 /*  1990 */    42,   41,  442,  442,  239,  241,  242,  243,  244,  245,
 /*  2000 */   214,   70,   71,   73,   72,   74,  442,  442,  259,  261,
 /*  2010 */   260,  442,  208,  240,  246,  362,  442,  442,  442,  442,
 /*  2020 */   259,  261,  260,  239,  241,  242,  243,  244,  245,  442,
 /*  2030 */   442,  181,  184,  187,  442,  442,  442,  442,  442,  223,
 /*  2040 */    76,   77,  442,  598,  524,  442,  518,  442,  442,  442,
 /*  2050 */    75,  177,  232,  236,  252,  174,  296,  442,  227,  181,
 /*  2060 */   184,  187,  442,  442,  442,  442,  212,  223,  442,  442,
 /*  2070 */   442,  238,  442,  217,  259,  261,  260,  442,  442,  177,
 /*  2080 */   232,  236,  252,  174,  274,  210,  227,  442,  442,  181,
 /*  2090 */   184,  187,  442,  259,  261,  260,  465,  223,  442,  238,
 /*  2100 */   442,  217,  442,  442,  259,  261,  260,  240,  246,  177,
 /*  2110 */   232,  236,  252,  174,  266,   54,  227,  239,  241,  242,
 /*  2120 */   243,  244,  245,  442,  442,  442,  442,  442,  442,  238,
 /*  2130 */   460,  217,   83,  453,   33,  240,  246,  442,  259,  261,
 /*  2140 */   260,  259,  261,  260,  442,  239,  241,  242,  243,  244,
 /*  2150 */   245,   57,   58,   59,   60,   61,   62,   63,   64,   65,
 /*  2160 */    66,   67,  442,  407,  442,  240,  246,  442,  442,  442,
 /*  2170 */   442,  259,  261,  260,  442,  239,  241,  242,  243,  244,
 /*  2180 */   245,  442,  442,  181,  184,  187,  442,  442,  442,  442,
 /*  2190 */   403,  223,  442,  442,  442,  442,  442,  442,  259,  261,
 /*  2200 */   260,  442,  442,  177,  232,  236,  252,  174,  289,  442,
 /*  2210 */   227,  181,  184,  187,  442,  442,  442,  442,  394,  223,
 /*  2220 */   442,  442,  442,  238,  442,  217,  259,  261,  260,  442,
 /*  2230 */   442,  177,  232,  236,  252,  174,  314,  442,  227,  442,
 /*  2240 */   442,  181,  184,  187,  259,  261,  260,  269,  442,  223,
 /*  2250 */   442,  238,  442,  217,  442,  259,  261,  260,  442,  240,
 /*  2260 */   246,  177,  232,  236,  252,  174,  442,  237,  227,  239,
 /*  2270 */   241,  242,  243,  244,  245,  259,  261,  260,  442,  442,
 /*  2280 */   442,  238,  175,  217,  442,  309,  442,  240,  246,  338,
 /*  2290 */   259,  261,  260,  259,  261,  260,  442,  239,  241,  242,
 /*  2300 */   243,  244,  245,  176,  186,  191,  193,  195,  197,  189,
 /*  2310 */   183,  199,  201,  209,  203,  205,  207,  240,  246,  370,
 /*  2320 */   442,   70,   71,   73,   72,   74,  442,  239,  241,  242,
 /*  2330 */   243,  244,  245,  442,  442,  181,  184,  187,  442,  442,
 /*  2340 */   442,  305,  442,  223,  442,  442,  442,  442,  442,  259,
 /*  2350 */   261,  260,  442,  442,  532,  177,  232,  236,  252,  174,
 /*  2360 */    76,   77,  227,  396,  181,  184,  187,  442,  107,  442,
 /*  2370 */    75,  442,  223,  442,  442,  238,  442,  217,  442,  442,
 /*  2380 */   442,  442,  442,  442,  177,  232,  236,  252,  174,  442,
 /*  2390 */   442,  227,  442,  181,  184,  187,  442,  442,  442,  442,
 /*  2400 */   442,  223,  442,  442,  238,  442,  217,  442,  442,  442,
 /*  2410 */   442,  240,  246,  177,  232,  236,  252,  174,  442,  442,
 /*  2420 */   248,  239,  241,  242,  243,  244,  245,  442,  442,  442,
 /*  2430 */   442,  442,  464,  238,  442,  217,  442,  442,  442,  442,
 /*  2440 */   240,  246,  442,  442,  442,  442,  442,  442,  442,  442,
 /*  2450 */   239,  241,  242,  243,  244,  245,  442,   70,   71,   73,
 /*  2460 */    72,   74,  442,  442,  442,  442,  442,  442,  442,  276,
 /*  2470 */   246,  277,  442,  442,  442,  442,  442,  442,  442,  275,
 /*  2480 */   241,  242,  243,  244,  245,  442,  442,  181,  184,  187,
 /*  2490 */   442,  442,  442,  442,  442,  223,   76,   77,  442,  601,
 /*  2500 */   512,  442,  518,  442,  442,  442,   75,  177,  232,  236,
 /*  2510 */   252,  174,  302,  442,  227,  181,  184,  187,  442,  442,
 /*  2520 */   442,  442,  442,  223,  442,  442,  442,  238,  442,  217,
 /*  2530 */   442,  442,  442,  442,  442,  177,  232,  236,  252,  174,
 /*  2540 */   442,  442,  227,  442,  442,  181,  184,  187,  442,  442,
 /*  2550 */   442,  442,  442,  223,  442,  238,  442,  217,  442,  442,
 /*  2560 */   442,  442,  442,  240,  246,  177,  232,  236,  252,  174,
 /*  2570 */   442,   54,  248,  239,  241,  242,  243,  244,  245,  442,
 /*  2580 */   442,  442,  442,  442,  442,  238,  442,  217,  110,  442,
 /*  2590 */    33,  240,  246,  442,  442,  442,  442,  442,  442,  442,
 /*  2600 */   442,  239,  241,  242,  243,  244,  245,   57,   58,   59,
 /*  2610 */    60,   61,   62,   63,   64,   65,   66,   67,  442,  442,
 /*  2620 */   442,  276,  246,   70,   71,   73,   72,   74,  442,  442,
 /*  2630 */   442,  275,  241,  242,  243,  244,  245,  442,  442,  442,
 /*  2640 */   442,  442,  215,  211,  213,  176,  186,  191,  193,  195,
 /*  2650 */   197,  189,  183,  199,  201,  209,  203,  205,  207,  442,
 /*  2660 */   442,  231,   76,   77,  442,  597,  524,  442,  518,  259,
 /*  2670 */   261,  260,   75,  442,  454,  442,  294,  263,  442,  324,
 /*  2680 */   442,  442,  442,  442,  442,  215,  211,  213,  176,  186,
 /*  2690 */   191,  193,  195,  197,  189,  183,  199,  201,  209,  203,
 /*  2700 */   205,  207,  442,  442,  442,  442,  442,  442,  442,  442,
 /*  2710 */   442,  325,  326,  327,  328,  329,  333,  315,  369,  442,
 /*  2720 */   357,  377,  378,   70,   71,   73,   72,   74,  215,  211,
 /*  2730 */   213,  176,  186,  191,  193,  195,  197,  189,  183,  199,
 /*  2740 */   201,  209,  203,  205,  207,  215,  211,  213,  176,  186,
 /*  2750 */   191,  193,  195,  197,  189,  183,  199,  201,  209,  203,
 /*  2760 */   205,  207,   76,   77,  442,  599,  492,  442,  442,  442,
 /*  2770 */   442,  442,   75,  247,  442,  442,  442,  466,  442,  442,
 /*  2780 */   442,  259,  261,  260,  442,  231,  442,  278,  442,  442,
 /*  2790 */   286,  282,  285,  259,  261,  260,  231,  442,  442,  324,
 /*  2800 */   272,  263,  442,  306,  259,  261,  260,  442,  442,  442,
 /*  2810 */   442,  264,  263,  442,  215,  211,  213,  176,  186,  191,
 /*  2820 */   193,  195,  197,  189,  183,  199,  201,  209,  203,  205,
 /*  2830 */   207,  325,  326,  327,  328,  329,  442,  442,  337,  442,
 /*  2840 */   442,  346,  347,  442,  442,  442,  461,  442,  442,  442,
 /*  2850 */   442,  442,  442,  442,  442,  442,  442,  215,  211,  213,
 /*  2860 */   176,  186,  191,  193,  195,  197,  189,  183,  199,  201,
 /*  2870 */   209,  203,  205,  207,  442,  442,  442,  442,  442,  219,
 /*  2880 */   215,  211,  213,  176,  186,  191,  193,  195,  197,  189,
 /*  2890 */   183,  199,  201,  209,  203,  205,  207,  215,  211,  213,
 /*  2900 */   176,  186,  191,  193,  195,  197,  189,  183,  199,  201,
 /*  2910 */   209,  203,  205,  207,  442,  442,  442,  442,  442,  442,
 /*  2920 */   442,  442,  395,  215,  211,  213,  176,  186,  191,  193,
 /*  2930 */   195,  197,  189,  183,  199,  201,  209,  203,  205,  207,
 /*  2940 */   442,  442,  442,  442,  442,  442,  442,  442,  442,  442,
 /*  2950 */   442,  442,  442,  442,  442,  404,  442,  442,  442,  442,
 /*  2960 */   442,  442,  442,  442,  442,  442,  215,  211,  213,  176,
 /*  2970 */   186,  191,  193,  195,  197,  189,  183,  199,  201,  209,
 /*  2980 */   203,  205,  207,  442,  442,  442,  442,  442,  442,  442,
 /*  2990 */   442,  442,  442,  442,  442,  442,  442,  442,  270,  442,
 /*  3000 */   442,  442,  442,  442,  442,  442,  442,  442,  442,  215,
 /*  3010 */   211,  213,  176,  186,  191,  193,  195,  197,  189,  183,
 /*  3020 */   199,  201,  209,  203,  205,  207,  442,  442,  442,  442,
 /*  3030 */   442,  442,  442,   54,  442,  442,  442,  442,  442,  442,
 /*  3040 */   442,  731,  442,  442,  442,  442,  442,  442,  442,  442,
 /*  3050 */    26,  442,   33,  211,  213,  176,  186,  191,  193,  195,
 /*  3060 */   197,  189,  183,  199,  201,  209,  203,  205,  207,   57,
 /*  3070 */    58,   59,   60,   61,   62,   63,   64,   65,   66,   67,
 /*  3080 */    54,  442,  442,  442,  442,  442,  442,  442,  442,  442,
 /*  3090 */   442,   54,  442,  442,  442,  442,  442,  497,  442,   33,
 /*  3100 */   442,   70,   71,   73,   72,   74,  442,  442,  442,  442,
 /*  3110 */    33,  442,  442,  442,  442,  442,   57,   58,   59,   60,
 /*  3120 */    61,   62,   63,   64,   65,   66,   67,   57,   58,   59,
 /*  3130 */    60,   61,   62,   63,   64,   65,   66,   67,  442,  442,
 /*  3140 */    76,   77,  442,  602,  541,   70,   71,   73,   72,   74,
 /*  3150 */    75,  442,  442,  442,  442,  442,  442,  442,  442,  442,
 /*  3160 */   442,  442,  442,  442,  442,  442,  442,  442,  442,  442,
 /*  3170 */   442,  442,  442,  442,  442,  442,  442,  442,  442,  442,
 /*  3180 */   442,  442,  442,  442,   76,   77,  442,  605,  492,   70,
 /*  3190 */    71,   73,   72,   74,   75,  442,  442,  442,  442,  442,
 /*  3200 */   442,  442,  442,  442,  442,  442,  442,  442,  442,  442,
 /*  3210 */   442,  442,  442,  442,  442,  442,  442,  442,  442,  442,
 /*  3220 */   442,  442,  442,  442,  442,  442,  442,  442,   76,   77,
 /*  3230 */   442,  603,  492,   70,   71,   73,   72,   74,   75,  442,
 /*  3240 */   442,  442,  442,  442,  442,  442,  442,  442,  442,  442,
 /*  3250 */   442,  442,  442,  442,  442,  442,  442,  442,  442,  442,
 /*  3260 */   442,  442,  442,  442,  442,  442,  442,  442,  442,  442,
 /*  3270 */   442,  442,   76,   77,  442,  600,  492,   70,   71,   73,
 /*  3280 */    72,   74,   75,  442,  442,  442,  442,  442,  442,  442,
 /*  3290 */   442,  442,  442,  442,  442,  442,  442,  442,  442,  442,
 /*  3300 */   442,  442,  442,  442,  442,  442,  442,  442,  442,  442,
 /*  3310 */   442,  442,  442,  442,  442,  442,   76,   77,  442,  604,
 /*  3320 */   492,   70,   71,   73,   72,   74,   75,  442,  442,  442,
 /*  3330 */   442,  442,  442,  442,  442,  442,  442,  442,  442,  442,
 /*  3340 */   442,  442,  442,  442,  442,  442,  442,  442,  442,  442,
 /*  3350 */   442,  442,  442,  442,  442,  442,  442,  442,  442,  442,
 /*  3360 */    76,   77,  442,   18,   78,   70,   71,   73,   72,   74,
 /*  3370 */    75,  442,  442,  442,  442,  442,  442,  442,  442,  442,
 /*  3380 */   442,  442,  442,  442,  442,  442,  442,  442,  442,  442,
 /*  3390 */   442,  442,  442,  442,  442,  442,  442,  442,  526,  442,
 /*  3400 */   442,  442,  442,  442,   76,   77,   70,   71,   73,   72,
 /*  3410 */    74,  442,  494,  442,   75,  442,  442,  442,  442,  442,
 /*  3420 */   442,  442,  442,  442,  442,  442,  442,  442,  442,  442,
 /*  3430 */   442,  442,  442,  442,  442,  442,  442,  442,  442,  442,
 /*  3440 */   442,  442,  442,  442,  442,   76,   77,  442,   20,   78,
 /*  3450 */   442,  442,  442,  442,  442,   75,
};
static YYCODETYPE yy_lookahead[] = {
 /*     0 */     7,    8,    9,  103,  104,  105,  106,  107,   15,  100,
 /*    10 */   101,  102,  103,  104,  105,  106,  107,   43,   65,  118,
 /*    20 */    27,   28,   29,   30,   31,  111,  125,   34,  114,  115,
 /*    30 */   116,  117,  118,  123,  120,  121,  126,  127,  128,  125,
 /*    40 */    47,   67,   49,   69,   51,   52,   53,   54,   55,   56,
 /*    50 */    57,   58,   59,   60,   61,   10,   11,   12,   13,   14,
 /*    60 */    15,   16,   17,   18,   19,   20,   21,   22,   23,   24,
 /*    70 */    25,   26,    6,  150,    0,   34,   83,   84,  155,    6,
 /*    80 */    35,  158,  159,  160,   43,   44,   93,   94,   95,   96,
 /*    90 */    97,   98,   51,   52,   53,   54,   55,   56,   57,   58,
 /*   100 */    59,   35,   61,   62,   63,   64,   35,   66,   24,   25,
 /*   110 */    26,   70,   71,   72,   73,   42,   45,   76,   35,   34,
 /*   120 */    21,   22,   23,   24,   25,   26,   34,   49,   43,   44,
 /*   130 */    89,   90,   91,   92,   45,   34,   51,   52,   53,   54,
 /*   140 */    55,   56,   57,   58,   59,   37,   61,   62,   63,   64,
 /*   150 */    42,   66,  126,  127,  128,   70,   71,   72,   73,  108,
 /*   160 */    82,   76,   84,   34,   86,    6,   77,   78,   79,   80,
 /*   170 */    81,   49,   43,   44,   89,   90,   91,   92,  161,  162,
 /*   180 */    51,   52,   53,   54,   55,   56,   57,   58,   59,    6,
 /*   190 */    61,   62,   63,   64,   35,   66,    6,  161,  162,   70,
 /*   200 */    71,   72,   73,  150,   82,   76,   34,   34,   86,  156,
 /*   210 */   157,  158,  159,  160,   42,   32,   43,   44,   89,   90,
 /*   220 */    91,   92,   32,   35,   51,   52,   53,   54,   55,   56,
 /*   230 */    57,   58,   59,   45,   61,   62,   63,   64,  110,   66,
 /*   240 */   112,  161,  162,   70,   71,   72,   73,   33,    6,   76,
 /*   250 */    36,   34,   38,   34,   40,   41,    6,   82,   44,   84,
 /*   260 */    43,   44,   89,   90,   91,   92,  161,  162,   51,   52,
 /*   270 */    53,   54,   55,   56,   57,   58,   59,   35,   61,   62,
 /*   280 */    63,   64,   82,   66,   84,   35,   37,   70,   71,   72,
 /*   290 */    73,   42,  150,   76,  123,   34,  154,  126,  127,  128,
 /*   300 */   158,  159,  160,    6,   43,   44,   89,   90,   91,   92,
 /*   310 */   161,  162,   51,   52,   53,   54,   55,   56,   57,   58,
 /*   320 */    59,    6,   61,   62,   63,   64,    6,   66,  161,  162,
 /*   330 */    34,   70,   71,   72,   73,   42,  150,   76,   42,   34,
 /*   340 */   154,    6,  108,    6,  158,  159,  160,   32,   43,   44,
 /*   350 */    89,   90,   91,   92,   35,   35,   51,   52,   53,   54,
 /*   360 */    55,   56,   57,   58,   59,    6,   61,   62,   63,   64,
 /*   370 */    35,   66,   35,  161,  162,   70,   71,   72,   73,  150,
 /*   380 */     6,   76,   85,   34,  161,  162,   34,  158,  159,  160,
 /*   390 */   125,   32,   43,   44,   89,   90,   91,   92,  169,   47,
 /*   400 */    51,   52,   53,   54,   55,   56,   57,   58,   59,   35,
 /*   410 */    61,   62,   63,   64,    6,   66,  161,  162,   34,   70,
 /*   420 */    71,   72,   73,   34,  150,   76,  123,   34,  154,  126,
 /*   430 */   127,  128,  158,  159,  160,  118,   43,   44,   89,   90,
 /*   440 */    91,   92,  125,   35,   51,   52,   53,   54,   55,   56,
 /*   450 */    57,   58,   59,   49,   61,   62,   63,   64,    6,   66,
 /*   460 */   161,  162,   34,   70,   71,   72,   73,   83,  150,   76,
 /*   470 */   123,   34,  154,  126,  127,  128,  158,  159,  160,  118,
 /*   480 */    43,   44,   89,   90,   91,   92,  125,   35,   51,   52,
 /*   490 */    53,   54,   55,   56,   57,   58,   59,    6,   61,   62,
 /*   500 */    63,   64,    6,   66,    6,   35,    6,   70,   71,   72,
 /*   510 */    73,   83,  150,   76,   45,   34,  154,  110,  119,  112,
 /*   520 */   158,  159,  160,   32,   43,   44,   89,   90,   91,   92,
 /*   530 */    32,   35,   51,   52,   53,   54,   55,   56,   57,   58,
 /*   540 */    59,    6,   61,   62,   63,   64,  110,   66,  112,    6,
 /*   550 */    34,   70,   71,   72,   73,  150,    6,   76,   42,   34,
 /*   560 */   155,  151,  152,  158,  159,  160,   43,   32,   43,   44,
 /*   570 */    89,   90,   91,   92,   74,   32,   51,   52,   53,   54,
 /*   580 */    55,   56,   57,   58,   59,   35,   61,   62,   63,   64,
 /*   590 */    67,   66,   69,    6,   86,   70,   71,   72,   73,  150,
 /*   600 */    47,   76,   34,   34,  155,  148,  149,  158,  159,  160,
 /*   610 */     6,   45,   43,   44,   89,   90,   91,   92,  119,   32,
 /*   620 */    51,   52,   53,   54,   55,   56,   57,   58,   59,   34,
 /*   630 */    61,   62,   63,   64,   45,   66,   32,  119,   16,   70,
 /*   640 */    71,   72,   73,   34,  150,   76,   39,   34,  154,   42,
 /*   650 */    34,   34,  158,  159,  160,   49,   43,   44,   89,   90,
 /*   660 */    91,   92,   35,   35,   51,   52,   53,   54,   55,   56,
 /*   670 */    57,   58,   59,   34,   61,   62,   63,   64,   34,   66,
 /*   680 */   109,  109,   34,   70,   71,   72,   73,  109,  150,   76,
 /*   690 */    43,   34,  154,   34,   49,   42,  158,  159,  160,   35,
 /*   700 */    43,   44,   89,   90,   91,   92,   35,   42,   51,   52,
 /*   710 */    53,   54,   55,   56,   57,   58,   59,   42,   61,   62,
 /*   720 */    63,   64,   42,   66,  149,   42,   68,   70,   71,   72,
 /*   730 */    73,  119,  150,   76,   42,   34,  154,   42,   71,   35,
 /*   740 */   158,  159,  160,   34,   43,   44,   89,   90,   91,   92,
 /*   750 */    32,  163,   51,   52,   53,   54,   55,   56,   57,   58,
 /*   760 */    59,   34,   61,   62,   63,   64,   16,   66,   49,   34,
 /*   770 */     6,   70,   71,   72,   73,   49,  116,   76,  118,   34,
 /*   780 */   120,  121,  163,   34,   68,  125,   49,   49,   43,   44,
 /*   790 */    89,   90,   91,   92,   43,   43,   51,   52,   53,   54,
 /*   800 */    55,   56,   57,   58,   59,   49,   61,   62,   63,   64,
 /*   810 */    68,   66,   68,   34,   34,   70,   71,   72,   73,   34,
 /*   820 */   150,   76,  153,   34,  154,  152,   85,  153,  158,  159,
 /*   830 */   160,  153,   43,   44,   89,   90,   91,   92,  153,   43,
 /*   840 */    51,   52,   53,   54,   55,   56,   57,   58,   59,  153,
 /*   850 */    61,   62,   63,   64,   43,   66,  153,   34,  153,   70,
 /*   860 */    71,   72,   73,  150,  153,   76,   85,   34,  153,   85,
 /*   870 */   157,  158,  159,  160,  153,  153,   43,   44,   89,   90,
 /*   880 */    91,   92,   35,   35,   51,   52,   53,   54,   55,   56,
 /*   890 */    57,   58,   59,   35,   61,   62,   63,   64,   34,   66,
 /*   900 */    34,  162,   35,   70,   71,   72,   73,   35,  150,   76,
 /*   910 */    34,   34,  154,   45,  119,   34,  158,  159,  160,   34,
 /*   920 */    43,   44,   89,   90,   91,   92,   74,   71,   51,   52,
 /*   930 */    53,   54,   55,   56,   57,   58,   59,   35,   61,   62,
 /*   940 */    63,   64,   68,   66,   65,   42,   42,   70,   71,   72,
 /*   950 */    73,   34,  150,   76,   49,   34,  154,   42,   42,   47,
 /*   960 */   158,  159,  160,   45,   43,   44,   89,   90,   91,   92,
 /*   970 */   119,   35,   51,   52,   53,   54,   55,   56,   57,   58,
 /*   980 */    59,   47,   61,   62,   63,   64,   45,   66,  119,   35,
 /*   990 */   119,   70,   71,   72,   73,  150,   35,   76,   35,   34,
 /*  1000 */   112,  119,  109,  158,  159,  160,   38,   34,   43,   44,
 /*  1010 */    89,   90,   91,   92,   42,  109,   51,   52,   53,   54,
 /*  1020 */    55,   56,   57,   58,   59,   34,   61,   62,   63,   64,
 /*  1030 */    38,   66,  109,  109,   34,   70,   71,   72,   73,  150,
 /*  1040 */   109,   76,   34,   34,  109,  170,  170,  158,  159,  160,
 /*  1050 */   170,  170,   43,   44,   89,   90,   91,   92,  170,  170,
 /*  1060 */    51,   52,   53,   54,   55,   56,   57,   58,   59,  170,
 /*  1070 */    61,   62,   63,   64,  170,   66,  170,  170,  170,   70,
 /*  1080 */    71,   72,   73,  150,  170,   76,  170,   34,  170,  170,
 /*  1090 */   170,  158,  159,  160,  170,  170,   43,   44,   89,   90,
 /*  1100 */    91,   92,  170,  170,   51,   52,   53,   54,   55,   56,
 /*  1110 */    57,   58,   59,  170,   61,   62,   63,   64,  170,   66,
 /*  1120 */   170,  170,  170,   70,   71,   72,   73,  147,  118,   76,
 /*  1130 */   150,   34,  122,  170,  170,  125,  170,  170,  158,  159,
 /*  1140 */   160,   44,   89,   90,   91,   92,   45,  170,   51,   52,
 /*  1150 */    53,   54,   55,   56,   57,   58,   59,   49,   61,   62,
 /*  1160 */    63,   64,  170,   66,  170,  170,  170,   70,   71,   72,
 /*  1170 */    73,  170,  170,   76,  107,  170,   68,  170,   77,   78,
 /*  1180 */    79,   80,   81,  170,  170,   84,   89,   90,   91,   92,
 /*  1190 */    82,  124,   84,  170,   86,  170,  129,  130,  131,  132,
 /*  1200 */   133,  134,  135,  136,  137,  138,  139,  140,  141,  142,
 /*  1210 */   143,  144,  145,  146,  107,    1,    2,    3,    4,    5,
 /*  1220 */   170,  147,  170,  170,  150,  158,  159,  160,  170,  170,
 /*  1230 */   170,  124,  158,  159,  160,  170,  129,  130,  131,  132,
 /*  1240 */   133,  134,  135,  136,  137,  138,  139,  140,  141,  142,
 /*  1250 */   143,  144,  145,  146,   40,   41,  107,   43,   44,  170,
 /*  1260 */    46,  150,  170,  113,   50,  158,  159,  160,  118,  158,
 /*  1270 */   159,  160,  122,  124,  170,  125,  165,  166,  129,  130,
 /*  1280 */   131,  132,  133,  134,  135,  136,  137,  138,  139,  140,
 /*  1290 */   141,  142,  143,  144,  145,  146,  107,    1,    2,    3,
 /*  1300 */     4,    5,    1,    2,    3,    4,    5,  158,  159,  160,
 /*  1310 */    37,  170,   39,  124,  170,   42,  170,  170,  129,  130,
 /*  1320 */   131,  132,  133,  134,  135,  136,  137,  138,  139,  140,
 /*  1330 */   141,  142,  143,  144,  145,  146,   40,   41,  170,  107,
 /*  1340 */   170,   40,   41,  170,   48,  150,   50,  158,  159,  160,
 /*  1350 */   150,   50,  170,  158,  159,  160,  124,  170,  158,  159,
 /*  1360 */   160,  129,  130,  131,  132,  133,  134,  135,  136,  137,
 /*  1370 */   138,  139,  140,  141,  142,  143,  144,  145,  146,  170,
 /*  1380 */   170,  107,  114,  170,  116,  117,  118,  150,  120,  121,
 /*  1390 */   158,  159,  160,  125,  170,  158,  159,  160,  124,  170,
 /*  1400 */   170,  170,  170,  129,  130,  131,  132,  133,  134,  135,
 /*  1410 */   136,  137,  138,  139,  140,  141,  142,  143,  144,  145,
 /*  1420 */   146,  107,    1,    2,    3,    4,    5,  150,  147,  170,
 /*  1430 */   170,  150,  158,  159,  160,  158,  159,  160,  124,  158,
 /*  1440 */   159,  160,  170,  129,  130,  131,  132,  133,  134,  135,
 /*  1450 */   136,  137,  138,  139,  140,  141,  142,  143,  144,  145,
 /*  1460 */   146,   40,   41,  118,  107,  170,  121,   46,  170,  150,
 /*  1470 */   125,   50,  158,  159,  160,  170,  170,  158,  159,  160,
 /*  1480 */   170,  124,  170,  170,  165,  166,  129,  130,  131,  132,
 /*  1490 */   133,  134,  135,  136,  137,  138,  139,  140,  141,  142,
 /*  1500 */   143,  144,  145,  146,  170,  170,  107,  170,  115,  116,
 /*  1510 */   117,  118,  150,  120,  121,  158,  159,  160,  125,  170,
 /*  1520 */   158,  159,  160,  124,  170,  170,  170,  170,  129,  130,
 /*  1530 */   131,  132,  133,  134,  135,  136,  137,  138,  139,  140,
 /*  1540 */   141,  142,  143,  144,  145,  146,  107,    1,    2,    3,
 /*  1550 */     4,    5,  170,  147,  170,  170,  150,  158,  159,  160,
 /*  1560 */   170,  170,  170,  124,  158,  159,  160,  170,  129,  130,
 /*  1570 */   131,  132,  133,  134,  135,  136,  137,  138,  139,  140,
 /*  1580 */   141,  142,  143,  144,  145,  146,   40,   41,   37,  107,
 /*  1590 */    39,  170,  147,   42,   48,  150,   50,  158,  159,  160,
 /*  1600 */   150,  170,  170,  158,  159,  160,  124,  170,  158,  159,
 /*  1610 */   160,  129,  130,  131,  132,  133,  134,  135,  136,  137,
 /*  1620 */   138,  139,  140,  141,  142,  143,  144,  145,  146,  150,
 /*  1630 */   170,  107,  170,  170,  170,  170,  170,  158,  159,  160,
 /*  1640 */   158,  159,  160,  170,  165,  166,  170,  170,  124,  170,
 /*  1650 */   170,  170,  170,  129,  130,  131,  132,  133,  134,  135,
 /*  1660 */   136,  137,  138,  139,  140,  141,  142,  143,  144,  145,
 /*  1670 */   146,  107,    1,    2,    3,    4,    5,  170,  170,  170,
 /*  1680 */   170,  150,  158,  159,  160,  170,  170,  170,  124,  158,
 /*  1690 */   159,  160,  170,  129,  130,  131,  132,  133,  134,  135,
 /*  1700 */   136,  137,  138,  139,  140,  141,  142,  143,  144,  145,
 /*  1710 */   146,   40,   41,  170,  107,  170,  170,  150,  170,   48,
 /*  1720 */   150,   50,  158,  159,  160,  158,  159,  160,  158,  159,
 /*  1730 */   160,  124,  170,  166,  170,  170,  129,  130,  131,  132,
 /*  1740 */   133,  134,  135,  136,  137,  138,  139,  140,  141,  142,
 /*  1750 */   143,  144,  145,  146,  170,  170,  107,  170,  170,  116,
 /*  1760 */   117,  118,  150,  170,  121,  158,  159,  160,  125,  170,
 /*  1770 */   158,  159,  160,  124,  170,  170,  170,  170,  129,  130,
 /*  1780 */   131,  132,  133,  134,  135,  136,  137,  138,  139,  140,
 /*  1790 */   141,  142,  143,  144,  145,  146,  107,    1,    2,    3,
 /*  1800 */     4,    5,  170,  170,  170,  170,  150,  158,  159,  160,
 /*  1810 */   170,  170,  170,  124,  158,  159,  160,  170,  129,  130,
 /*  1820 */   131,  132,  133,  134,  135,  136,  137,  138,  139,  140,
 /*  1830 */   141,  142,  143,  144,  145,  146,   40,   41,  170,  107,
 /*  1840 */   170,  170,  150,  170,   48,  150,   50,  158,  159,  160,
 /*  1850 */   158,  159,  160,  158,  159,  160,  170,  170,  170,  170,
 /*  1860 */   170,  129,  130,  131,  132,  133,  134,  135,  136,  137,
 /*  1870 */   138,  139,  140,  141,  142,  143,  144,  145,  146,    7,
 /*  1880 */     8,    9,    0,  170,  170,  170,  170,   15,  150,  170,
 /*  1890 */   158,  159,  160,  170,  170,  170,  158,  159,  160,   27,
 /*  1900 */    28,   29,   30,   31,  150,  170,   34,  170,    7,    8,
 /*  1910 */     9,  170,  158,  159,  160,   33,   15,  170,   36,   47,
 /*  1920 */    38,   49,   40,   41,  170,  170,   44,   34,   27,   28,
 /*  1930 */    29,   30,   31,   32,  150,   34,  170,    7,    8,    9,
 /*  1940 */   170,  170,  158,  159,  160,   15,  170,   75,   47,  127,
 /*  1950 */    49,  167,  168,  169,  170,   83,   84,   27,   28,   29,
 /*  1960 */    30,   31,  170,  170,   34,   93,   94,   95,   96,   97,
 /*  1970 */    98,  170,  150,  170,  170,  170,   83,   47,  170,   49,
 /*  1980 */   158,  159,  160,  170,   83,   84,   93,   94,   95,   96,
 /*  1990 */    97,   98,  170,  170,   93,   94,   95,   96,   97,   98,
 /*  2000 */   150,    1,    2,    3,    4,    5,  170,  170,  158,  159,
 /*  2010 */   160,  170,  150,   83,   84,   85,  170,  170,  170,  170,
 /*  2020 */   158,  159,  160,   93,   94,   95,   96,   97,   98,  170,
 /*  2030 */   170,    7,    8,    9,  170,  170,  170,  170,  170,   15,
 /*  2040 */    40,   41,  170,   43,   44,  170,   46,  170,  170,  170,
 /*  2050 */    50,   27,   28,   29,   30,   31,   32,  170,   34,    7,
 /*  2060 */     8,    9,  170,  170,  170,  170,  150,   15,  170,  170,
 /*  2070 */   170,   47,  170,   49,  158,  159,  160,  170,  170,   27,
 /*  2080 */    28,   29,   30,   31,   32,  150,   34,  170,  170,    7,
 /*  2090 */     8,    9,  170,  158,  159,  160,  150,   15,  170,   47,
 /*  2100 */   170,   49,  170,  170,  158,  159,  160,   83,   84,   27,
 /*  2110 */    28,   29,   30,   31,   32,   15,   34,   93,   94,   95,
 /*  2120 */    96,   97,   98,  170,  170,  170,  170,  170,  170,   47,
 /*  2130 */   150,   49,   32,  150,   34,   83,   84,  170,  158,  159,
 /*  2140 */   160,  158,  159,  160,  170,   93,   94,   95,   96,   97,
 /*  2150 */    98,   51,   52,   53,   54,   55,   56,   57,   58,   59,
 /*  2160 */    60,   61,  170,  150,  170,   83,   84,  170,  170,  170,
 /*  2170 */   170,  158,  159,  160,  170,   93,   94,   95,   96,   97,
 /*  2180 */    98,  170,  170,    7,    8,    9,  170,  170,  170,  170,
 /*  2190 */   150,   15,  170,  170,  170,  170,  170,  170,  158,  159,
 /*  2200 */   160,  170,  170,   27,   28,   29,   30,   31,   32,  170,
 /*  2210 */    34,    7,    8,    9,  170,  170,  170,  170,  150,   15,
 /*  2220 */   170,  170,  170,   47,  170,   49,  158,  159,  160,  170,
 /*  2230 */   170,   27,   28,   29,   30,   31,  150,  170,   34,  170,
 /*  2240 */   170,    7,    8,    9,  158,  159,  160,  150,  170,   15,
 /*  2250 */   170,   47,  170,   49,  170,  158,  159,  160,  170,   83,
 /*  2260 */    84,   27,   28,   29,   30,   31,  170,  150,   34,   93,
 /*  2270 */    94,   95,   96,   97,   98,  158,  159,  160,  170,  170,
 /*  2280 */   170,   47,  150,   49,  170,  150,  170,   83,   84,   85,
 /*  2290 */   158,  159,  160,  158,  159,  160,  170,   93,   94,   95,
 /*  2300 */    96,   97,   98,   13,   14,   15,   16,   17,   18,   19,
 /*  2310 */    20,   21,   22,   23,   24,   25,   26,   83,   84,   85,
 /*  2320 */   170,    1,    2,    3,    4,    5,  170,   93,   94,   95,
 /*  2330 */    96,   97,   98,  170,  170,    7,    8,    9,  170,  170,
 /*  2340 */   170,  150,  170,   15,  170,  170,  170,  170,  170,  158,
 /*  2350 */   159,  160,  170,  170,   34,   27,   28,   29,   30,   31,
 /*  2360 */    40,   41,   34,   35,    7,    8,    9,  170,   48,  170,
 /*  2370 */    50,  170,   15,  170,  170,   47,  170,   49,  170,  170,
 /*  2380 */   170,  170,  170,  170,   27,   28,   29,   30,   31,  170,
 /*  2390 */   170,   34,  170,    7,    8,    9,  170,  170,  170,  170,
 /*  2400 */   170,   15,  170,  170,   47,  170,   49,  170,  170,  170,
 /*  2410 */   170,   83,   84,   27,   28,   29,   30,   31,  170,  170,
 /*  2420 */    34,   93,   94,   95,   96,   97,   98,  170,  170,  170,
 /*  2430 */   170,  170,   75,   47,  170,   49,  170,  170,  170,  170,
 /*  2440 */    83,   84,  170,  170,  170,  170,  170,  170,  170,  170,
 /*  2450 */    93,   94,   95,   96,   97,   98,  170,    1,    2,    3,
 /*  2460 */     4,    5,  170,  170,  170,  170,  170,  170,  170,   83,
 /*  2470 */    84,   85,  170,  170,  170,  170,  170,  170,  170,   93,
 /*  2480 */    94,   95,   96,   97,   98,  170,  170,    7,    8,    9,
 /*  2490 */   170,  170,  170,  170,  170,   15,   40,   41,  170,   43,
 /*  2500 */    44,  170,   46,  170,  170,  170,   50,   27,   28,   29,
 /*  2510 */    30,   31,   32,  170,   34,    7,    8,    9,  170,  170,
 /*  2520 */   170,  170,  170,   15,  170,  170,  170,   47,  170,   49,
 /*  2530 */   170,  170,  170,  170,  170,   27,   28,   29,   30,   31,
 /*  2540 */   170,  170,   34,  170,  170,    7,    8,    9,  170,  170,
 /*  2550 */   170,  170,  170,   15,  170,   47,  170,   49,  170,  170,
 /*  2560 */   170,  170,  170,   83,   84,   27,   28,   29,   30,   31,
 /*  2570 */   170,   15,   34,   93,   94,   95,   96,   97,   98,  170,
 /*  2580 */   170,  170,  170,  170,  170,   47,  170,   49,   32,  170,
 /*  2590 */    34,   83,   84,  170,  170,  170,  170,  170,  170,  170,
 /*  2600 */   170,   93,   94,   95,   96,   97,   98,   51,   52,   53,
 /*  2610 */    54,   55,   56,   57,   58,   59,   60,   61,  170,  170,
 /*  2620 */   170,   83,   84,    1,    2,    3,    4,    5,  170,  170,
 /*  2630 */   170,   93,   94,   95,   96,   97,   98,  170,  170,  170,
 /*  2640 */   170,  170,   10,   11,   12,   13,   14,   15,   16,   17,
 /*  2650 */    18,   19,   20,   21,   22,   23,   24,   25,   26,  170,
 /*  2660 */   170,  150,   40,   41,  170,   43,   44,  170,   46,  158,
 /*  2670 */   159,  160,   50,  170,   42,  170,  165,  166,  170,   45,
 /*  2680 */   170,  170,  170,  170,  170,   10,   11,   12,   13,   14,
 /*  2690 */    15,   16,   17,   18,   19,   20,   21,   22,   23,   24,
 /*  2700 */    25,   26,  170,  170,  170,  170,  170,  170,  170,  170,
 /*  2710 */   170,   77,   78,   79,   80,   81,   82,   42,   84,  170,
 /*  2720 */    86,   87,   88,    1,    2,    3,    4,    5,   10,   11,
 /*  2730 */    12,   13,   14,   15,   16,   17,   18,   19,   20,   21,
 /*  2740 */    22,   23,   24,   25,   26,   10,   11,   12,   13,   14,
 /*  2750 */    15,   16,   17,   18,   19,   20,   21,   22,   23,   24,
 /*  2760 */    25,   26,   40,   41,  170,   43,   44,  170,  170,  170,
 /*  2770 */   170,  170,   50,  150,  170,  170,  170,   42,  170,  170,
 /*  2780 */   170,  158,  159,  160,  170,  150,  170,  164,  170,  170,
 /*  2790 */   167,  168,  169,  158,  159,  160,  150,  170,  170,   45,
 /*  2800 */   165,  166,  170,   85,  158,  159,  160,  170,  170,  170,
 /*  2810 */   170,  165,  166,  170,   10,   11,   12,   13,   14,   15,
 /*  2820 */    16,   17,   18,   19,   20,   21,   22,   23,   24,   25,
 /*  2830 */    26,   77,   78,   79,   80,   81,  170,  170,   84,  170,
 /*  2840 */   170,   87,   88,  170,  170,  170,   42,  170,  170,  170,
 /*  2850 */   170,  170,  170,  170,  170,  170,  170,   10,   11,   12,
 /*  2860 */    13,   14,   15,   16,   17,   18,   19,   20,   21,   22,
 /*  2870 */    23,   24,   25,   26,  170,  170,  170,  170,  170,   32,
 /*  2880 */    10,   11,   12,   13,   14,   15,   16,   17,   18,   19,
 /*  2890 */    20,   21,   22,   23,   24,   25,   26,   10,   11,   12,
 /*  2900 */    13,   14,   15,   16,   17,   18,   19,   20,   21,   22,
 /*  2910 */    23,   24,   25,   26,  170,  170,  170,  170,  170,  170,
 /*  2920 */   170,  170,   35,   10,   11,   12,   13,   14,   15,   16,
 /*  2930 */    17,   18,   19,   20,   21,   22,   23,   24,   25,   26,
 /*  2940 */   170,  170,  170,  170,  170,  170,  170,  170,  170,  170,
 /*  2950 */   170,  170,  170,  170,  170,   85,  170,  170,  170,  170,
 /*  2960 */   170,  170,  170,  170,  170,  170,   10,   11,   12,   13,
 /*  2970 */    14,   15,   16,   17,   18,   19,   20,   21,   22,   23,
 /*  2980 */    24,   25,   26,  170,  170,  170,  170,  170,  170,  170,
 /*  2990 */   170,  170,  170,  170,  170,  170,  170,  170,   85,  170,
 /*  3000 */   170,  170,  170,  170,  170,  170,  170,  170,  170,   10,
 /*  3010 */    11,   12,   13,   14,   15,   16,   17,   18,   19,   20,
 /*  3020 */    21,   22,   23,   24,   25,   26,  170,  170,  170,  170,
 /*  3030 */   170,  170,  170,   15,  170,  170,  170,  170,  170,  170,
 /*  3040 */   170,   85,  170,  170,  170,  170,  170,  170,  170,  170,
 /*  3050 */    32,  170,   34,   11,   12,   13,   14,   15,   16,   17,
 /*  3060 */    18,   19,   20,   21,   22,   23,   24,   25,   26,   51,
 /*  3070 */    52,   53,   54,   55,   56,   57,   58,   59,   60,   61,
 /*  3080 */    15,  170,  170,  170,  170,  170,  170,  170,  170,  170,
 /*  3090 */   170,   15,  170,  170,  170,  170,  170,   32,  170,   34,
 /*  3100 */   170,    1,    2,    3,    4,    5,  170,  170,  170,  170,
 /*  3110 */    34,  170,  170,  170,  170,  170,   51,   52,   53,   54,
 /*  3120 */    55,   56,   57,   58,   59,   60,   61,   51,   52,   53,
 /*  3130 */    54,   55,   56,   57,   58,   59,   60,   61,  170,  170,
 /*  3140 */    40,   41,  170,   43,   44,    1,    2,    3,    4,    5,
 /*  3150 */    50,  170,  170,  170,  170,  170,  170,  170,  170,  170,
 /*  3160 */   170,  170,  170,  170,  170,  170,  170,  170,  170,  170,
 /*  3170 */   170,  170,  170,  170,  170,  170,  170,  170,  170,  170,
 /*  3180 */   170,  170,  170,  170,   40,   41,  170,   43,   44,    1,
 /*  3190 */     2,    3,    4,    5,   50,  170,  170,  170,  170,  170,
 /*  3200 */   170,  170,  170,  170,  170,  170,  170,  170,  170,  170,
 /*  3210 */   170,  170,  170,  170,  170,  170,  170,  170,  170,  170,
 /*  3220 */   170,  170,  170,  170,  170,  170,  170,  170,   40,   41,
 /*  3230 */   170,   43,   44,    1,    2,    3,    4,    5,   50,  170,
 /*  3240 */   170,  170,  170,  170,  170,  170,  170,  170,  170,  170,
 /*  3250 */   170,  170,  170,  170,  170,  170,  170,  170,  170,  170,
 /*  3260 */   170,  170,  170,  170,  170,  170,  170,  170,  170,  170,
 /*  3270 */   170,  170,   40,   41,  170,   43,   44,    1,    2,    3,
 /*  3280 */     4,    5,   50,  170,  170,  170,  170,  170,  170,  170,
 /*  3290 */   170,  170,  170,  170,  170,  170,  170,  170,  170,  170,
 /*  3300 */   170,  170,  170,  170,  170,  170,  170,  170,  170,  170,
 /*  3310 */   170,  170,  170,  170,  170,  170,   40,   41,  170,   43,
 /*  3320 */    44,    1,    2,    3,    4,    5,   50,  170,  170,  170,
 /*  3330 */   170,  170,  170,  170,  170,  170,  170,  170,  170,  170,
 /*  3340 */   170,  170,  170,  170,  170,  170,  170,  170,  170,  170,
 /*  3350 */   170,  170,  170,  170,  170,  170,  170,  170,  170,  170,
 /*  3360 */    40,   41,  170,   43,   44,    1,    2,    3,    4,    5,
 /*  3370 */    50,  170,  170,  170,  170,  170,  170,  170,  170,  170,
 /*  3380 */   170,  170,  170,  170,  170,  170,  170,  170,  170,  170,
 /*  3390 */   170,  170,  170,  170,  170,  170,  170,  170,   34,  170,
 /*  3400 */   170,  170,  170,  170,   40,   41,    1,    2,    3,    4,
 /*  3410 */     5,  170,   48,  170,   50,  170,  170,  170,  170,  170,
 /*  3420 */   170,  170,  170,  170,  170,  170,  170,  170,  170,  170,
 /*  3430 */   170,  170,  170,  170,  170,  170,  170,  170,  170,  170,
 /*  3440 */   170,  170,  170,  170,  170,   40,   41,  170,   43,   44,
 /*  3450 */   170,  170,  170,  170,  170,   50,
};
#define YY_SHIFT_USE_DFLT (-48)
static short yy_shift_ofst[] = {
 /*     0 */   214,   74, 1882,  -48,  -48,  -48,  -48,  -48,   92,   83,
 /*    10 */   -48,  101,  108,  -48,  219,  293,  -48, 3320,  -48, 3405,
 /*    20 */   -48,  -48, 1671,  389,  404, 3018,  319,  -48,  491,  470,
 /*    30 */   -48, 3076,  -48,  469, 1893,  -48,  -48,  -48,  -48,  -48,
 /*    40 */   -48,  -48,  -48,  508,  553,  -48,  568,  566, 1893,  -48,
 /*    50 */   595,  589, 1893,  -48,  609,  622,  616,  -48,  -48,  -48,
 /*    60 */   -48,  -48,  -48,  -48,  -48,  -48,  -48,  -48,  -48,  -48,
 /*    70 */   -48,  -48,  -48,  -48,  -48,  -48,  -48,  -48, 1301, 1546,
 /*    80 */   617,  606, 2100,  627,  -48,  190,  628,  -48,  -48,  -48,
 /*    90 */   639, 1551,  -48,  644,  607,  -48,  648,   73,  -48, 1214,
 /*   100 */   -48,  647,  -48, 2622, 3232,  -48, 1796,  659,  645, 2556,
 /*   110 */   653,  217,  -48,  305,  -48,  -48,  -48,  -48,  -48,  -48,
 /*   120 */   -48,  -48,  -48,  -48,  -48,  -48,  -48,  -48,  -48,  -48,
 /*   130 */   -48,  -48,  -48,  -48,  664,  -48,  671,  -48, 2508,  665,
 /*   140 */   525,  437,  -47,  675,  833,  173,  -48, 2508,  680,  -26,
 /*   150 */   -48,  523,  -48,  -48, 1893,  658, 1097, 1097,  683, 1009,
 /*   160 */   921,  -48, 2508,  692,  789,  657,  -48,  695,  569,  481,
 /*   170 */   667, 2508,  704,  -48, 2508,  -48, 2508,  709,  -48,  175,
 /*   180 */  2508, 2508, 2999, 2508, 2508, 2999, 2508, 2508, 2999, 2508,
 /*   190 */    99, 2508,   99, 2508,   99, 2508,   99, 2508,   99, 2508,
 /*   200 */    84, 2508,   84, 2508,  -48, 2508,  -48, 2508,  -48, 2508,
 /*   210 */    84, 2508, 2290, 2508, 2290, 2508, 3042,   -7, 2847,  -48,
 /*   220 */   718, 2508,  -48,  727,  750, 2508,   99,   78,  516,  719,
 /*   230 */  2176, 2999,  735,  764,  709,  -48, 2508,  -48,  -48,  -48,
 /*   240 */   -48,  -48,  -48,  -48,  -48,  -48, 2386, 2999, 1108,  352,
 /*   250 */   726, 2082,  749,  737, 1901,  -48,  183,  -48, 2508,  -48,
 /*   260 */   -48,  -48,  -48,  -48,  604,  -48,  -48,  -48, 2508, 2913,
 /*   270 */   -48, 2052,  587,  -48,  -48,  716,  742,  -48,  297,  -48,
 /*   280 */  2538,  -48,  744, 2508,  -48,  -48,  -48,  498,  -48,  -48,
 /*   290 */   384,  751,  738, 2024,  535,  -48,  -48,  752,  756, 2480,
 /*   300 */   543,  -48,  -48,   99,   99, 2718,  -48,  779,  -48,   99,
 /*   310 */  2999,  780,  500, 1872, 2675, 1097,  701,  -48,  785,  550,
 /*   320 */   -48,  785,  -48, 2634,  -48,  -48,  -48,  -48,  -48,  -48,
 /*   330 */  2508,  -48, 2999,  296, 2754, 2508,  -48, 2204,   89, 2508,
 /*   340 */   -48,  741,   89, 2508,  -48, 2956,  -48,  -48,  428,  796,
 /*   350 */    89, 2508,  -48,  811,   89, 2508,  -48,  823, 1101, 2508,
 /*   360 */   -48, 1930,   89, 2508,  -48,  781,   89, 2508,  -48, 2234,
 /*   370 */    89, 2508,  -48,  784,   89, 2508,  -48,  -48,  -48,  -48,
 /*   380 */  2508,   66,  -48, 2508,  -48, 2999,  -48,  847,  -48,  848,
 /*   390 */   -48,  858,  -48, 2328, 2887,  -48,  -48,  864,  200,  866,
 /*   400 */   867,  -48, 2508, 2870,  872,  -48, 2508,   45,  -48,  876,
 /*   410 */   159,  -48,  876,  -48,  868, 1893,  -48,  -48,  876,  250,
 /*   420 */   -48,  876,  242,  -48,  876,  320,  -48,  876,  335,  -48,
 /*   430 */   876,  337,  -48,  876,  374,  -48,  876,  408,  -48,  876,
 /*   440 */   452,  -48,  876,  496,  -48,  122,  881,  726,  172,  719,
 /*   450 */   -48,  -48, 2508, 2632, 1097,  613,  -48,  885,  852, 2357,
 /*   460 */  2804, 1097,  745,  -48, 2508, 2735, 1097,  877,  -48,  856,
 /*   470 */  2508,  902,  -48,  -48,  -48,  874, 1097, 1097,  -48,  -48,
 /*   480 */   879,  903,  965, 1053,  -48,  -48,  315,  904,  261,  -48,
 /*   490 */   349,  -48, 1301, 1296,  917,  905, 3065,  915,  393,  -48,
 /*   500 */    41,  -48,  359,  916,   85,  -48,  129,  -48, 2456, 3276,
 /*   510 */   -48,  -48, 1421,  912,  918, 1893,  936,  -48,  934,  941,
 /*   520 */  1893,  954,  -48,  -48, 1421, 3364,  188,  -48, 1893,  961,
 /*   530 */   -48, 2320,   71,  -48, 1893,  963,  -48,  -48, 2000, 3100,
 /*   540 */  3144, 1301, 3188,  -48, 2722,  648,  -48,  -48,  -48,  648,
 /*   550 */    73,  -48,  968,  973, 1273,  -48,  991,  972,  -48,  648,
 /*   560 */    73,  -48,  992, 1000,  249,  -48, 1008,  972,  -48,  -48,
};
#define YY_REDUCE_USE_DFLT (-101)
static short yy_reduce_ofst[] = {
 /*     0 */   -91, -101, -100, -101, -101, -101, -101, -101, -101, -101,
 /*    10 */  -101, -101,   51, -101, -101,  234, -101, 1150, -101, 1010,
 /*    20 */  -101, -101,  265, -101, -101,  347, -101, -101, -101, -101,
 /*    30 */  -101,   26, -101, -101,  399, -101, -101, -101, -101, -101,
 /*    40 */  -101, -101, -101, -101, -101, -101, -101, -101,  499, -101,
 /*    50 */  -101, -101,  518, -101, -101, -101, -101, -101, -101, -101,
 /*    60 */  -101, -101, -101, -101, -101, -101, -101, -101, -101, -101,
 /*    70 */  -101, -101, -101, -101, -101, -101, -101, -101,  -99,  265,
 /*    80 */  -101, -101,  171, -101, -101, -101, -101, -101, -101, -101,
 /*    90 */  -101,  571, -101, -101,  572, -101,  436,  578, -101,  -86,
 /*   100 */  -101, -101, -101, 1393, 1345, -101,  265, -101, -101,  303,
 /*   110 */  -101, 1189, -101, 1732, -101, -101, -101, -101, -101, -101,
 /*   120 */  -101, -101, -101, -101, -101, -101, -101, -101, -101, -101,
 /*   130 */  -101, -101, -101, -101, -101, -101, -101, -101,  980, -101,
 /*   140 */  1232, 1732, -101, -101, 1149, 1732, -101, 1406, -101,  457,
 /*   150 */  -101,  575, -101, -101,  612, -101, 1314, 1732, -101, 1067,
 /*   160 */  1732, -101, 1074, -101, 1607, 1732, -101, -101, 1107, 1732,
 /*   170 */  -101, 1445, -101, -101, 2132, -101, 2135,  588, -101, -101,
 /*   180 */  2191,  845, -101,  889,  933, -101, 1200, 1237, -101, 1277,
 /*   190 */  -101, 1450, -101, 1531, -101, 1570, -101, 1612, -101, 1656,
 /*   200 */  -101, 1692, -101, 1738, -101, 1754, -101, 1862, -101, 1935,
 /*   210 */  -101, 1916, -101, 1850, -101, 1695, -101, 1822, -101, -101,
 /*   220 */  -101, 1362, -101, -101, -101, 1195, -101, -101, -101, -101,
 /*   230 */  1319, -101, -101, -101,  619, -101, 2117, -101, -101, -101,
 /*   240 */  -101, -101, -101, -101, -101, -101, 2623, -101, -101, -101,
 /*   250 */  -101, 2646, -101, -101, 1479, -101, -101, -101, 1567, -101,
 /*   260 */  -101, -101, -101, -101, -101, -101, -101, -101, 2097, -101,
 /*   270 */  -101, 2635, -101, -101, -101, -101, -101, -101, -101, -101,
 /*   280 */  1784, -101, -101,  229, -101, -101, -101, -101, -101, -101,
 /*   290 */  -101, -101, -101, 2511, -101, -101, -101, -101, -101, 1111,
 /*   300 */  -101, -101, -101, -101, -101, -101, -101, -101, -101, -101,
 /*   310 */  -101, -101, -101, 2086, -101, 1564, 1732, -101,  410, -101,
 /*   320 */  -101,  673, -101,  669, -101, -101, -101, -101, -101, -101,
 /*   330 */   318, -101, -101, -101,  674,  670, -101,  449,  678,  142,
 /*   340 */  -101, -101,  685,  758, -101, -101, -101, -101, -101, -101,
 /*   350 */   696,  362, -101, -101,  703,  802, -101, -101,  705,  582,
 /*   360 */  -101,  405,  711,  274, -101, -101,  715,  186, -101,  -77,
 /*   370 */   721,  494, -101, -101,  722,  538, -101, -101, -101, -101,
 /*   380 */    53, -101, -101,  713, -101, -101, -101, -101, -101, -101,
 /*   390 */  -101, -101, -101, 2068, -101, -101, -101, -101, -101, -101,
 /*   400 */  -101, -101, 2040, -101, -101, -101, 2013, -101, -101,   17,
 /*   410 */  -101, -101,  739, -101, -101,  795, -101, -101,   36, -101,
 /*   420 */  -101,   80, -101, -101,  105, -101, -101,  149, -101, -101,
 /*   430 */   167, -101, -101,  212, -101, -101,  223, -101, -101,  255,
 /*   440 */  -101, -101,  299, -101, -101, -101, -101, -101, -101, -101,
 /*   450 */  -101, -101, 1983, -101, 1689, 1732, -101, -101, -101, 1980,
 /*   460 */  -101, 1649, 1732, -101, 1946, -101, 1524, 1732, -101, -101,
 /*   470 */  1281, -101, -101, -101, -101, -101, 1482, 1732, -101, -101,
 /*   480 */  -101, -101, 1439, 1732, -101, -101, -101, -101, 1399, -101,
 /*   490 */  1732, -101,  317,  265, -101, -101,  -90, -101, 1357, -101,
 /*   500 */  1732, -101, -101, -101, 1274, -101, 1732, -101,  660, 1345,
 /*   510 */  -101, -101,  317, -101, -101,  851, -101, -101, -101, -101,
 /*   520 */   869, -101, -101, -101,  361,  265, -101, -101,  871, -101,
 /*   530 */  -101,  265, -101, -101,  882, -101, -101, -101, 1268, 1643,
 /*   540 */  1345,  361, 1345, -101, 1345,  888, -101, -101, -101,  407,
 /*   550 */   893, -101, -101, -101,  906, -101, -101,  923, -101,  128,
 /*   560 */   924, -101, -101, -101,  931, -101, -101,  935, -101, -101,
};
static YYACTIONTYPE yy_default[] = {
 /*     0 */   840,  840,  840,  572,  574,  575,  576,  577,  840,  840,
 /*    10 */   578,  840,  840,  579,  840,  840,  580,  840,  595,  840,
 /*    20 */   596,  616,  840,  840,  840,  840,  840,  628,  840,  840,
 /*    30 */   629,  840,  642,  644,  840,  647,  828,  829,  830,  831,
 /*    40 */   832,  833,  834,  840,  840,  835,  840,  645,  840,  648,
 /*    50 */   840,  646,  840,  649,  840,  840,  840,  651,  652,  653,
 /*    60 */   654,  655,  656,  657,  658,  659,  660,  661,  643,  632,
 /*    70 */   634,  635,  636,  637,  638,  639,  640,  641,  840,  840,
 /*    80 */   840,  840,  840,  840,  630,  840,  840,  631,  633,  617,
 /*    90 */   840,  840,  581,  840,  840,  582,  840,  840,  584,  840,
 /*   100 */   590,  840,  591,  840,  840,  614,  840,  840,  840,  840,
 /*   110 */   840,  840,  620,  840,  622,  662,  664,  665,  666,  667,
 /*   120 */   668,  669,  670,  671,  672,  673,  674,  675,  676,  677,
 /*   130 */   678,  679,  680,  681,  840,  682,  840,  683,  840,  840,
 /*   140 */   840,  840,  686,  840,  840,  840,  687,  840,  840,  840,
 /*   150 */   690,  840,  691,  692,  840,  840,  694,  695,  840,  840,
 /*   160 */   840,  698,  840,  840,  840,  840,  700,  840,  840,  840,
 /*   170 */   840,  840,  840,  702,  840,  759,  840,  840,  760,  840,
 /*   180 */   840,  840,  761,  840,  840,  762,  840,  840,  763,  840,
 /*   190 */   767,  840,  768,  840,  769,  840,  770,  840,  771,  840,
 /*   200 */   779,  840,  780,  840,  781,  840,  782,  840,  783,  840,
 /*   210 */   784,  840,  785,  840,  786,  840,  787,  840,  840,  772,
 /*   220 */   840,  840,  773,  840,  840,  840,  774,  791,  840,  775,
 /*   230 */   840,  819,  840,  840,  840,  788,  840,  789,  790,  792,
 /*   240 */   793,  794,  795,  796,  797,  798,  840,  827,  791,  840,
 /*   250 */   776,  840,  840,  801,  840,  802,  840,  803,  840,  814,
 /*   260 */   815,  816,  817,  818,  840,  806,  807,  777,  840,  840,
 /*   270 */   778,  840,  840,  804,  805,  792,  793,  799,  840,  800,
 /*   280 */   840,  820,  840,  840,  822,  823,  821,  840,  808,  809,
 /*   290 */   840,  840,  840,  840,  840,  810,  811,  840,  840,  840,
 /*   300 */   840,  812,  813,  766,  765,  840,  836,  840,  837,  764,
 /*   310 */   838,  840,  840,  840,  840,  840,  840,  703,  840,  840,
 /*   320 */   707,  840,  708,  840,  710,  711,  712,  713,  714,  715,
 /*   330 */   840,  716,  758,  840,  840,  840,  717,  840,  840,  840,
 /*   340 */   720,  840,  840,  840,  721,  840,  727,  728,  840,  840,
 /*   350 */   840,  840,  718,  840,  840,  840,  719,  840,  840,  840,
 /*   360 */   722,  840,  840,  840,  723,  840,  840,  840,  724,  840,
 /*   370 */   840,  840,  725,  840,  840,  840,  726,  729,  730,  709,
 /*   380 */   840,  840,  732,  840,  733,  735,  734,  840,  736,  840,
 /*   390 */   737,  840,  738,  840,  840,  739,  740,  840,  840,  840,
 /*   400 */   840,  741,  840,  840,  840,  742,  840,  840,  743,  840,
 /*   410 */   840,  744,  840,  754,  756,  840,  757,  755,  840,  840,
 /*   420 */   745,  840,  840,  746,  840,  840,  747,  840,  840,  748,
 /*   430 */   840,  840,  749,  840,  840,  750,  840,  840,  751,  840,
 /*   440 */   840,  752,  840,  840,  753,  840,  840,  840,  840,  840,
 /*   450 */   839,  663,  840,  840,  840,  840,  704,  840,  840,  840,
 /*   460 */   840,  840,  840,  705,  840,  840,  840,  840,  706,  840,
 /*   470 */   840,  840,  701,  699,  697,  840,  840,  696,  693,  688,
 /*   480 */   684,  840,  840,  840,  689,  685,  840,  840,  840,  621,
 /*   490 */   840,  623,  840,  840,  840,  840,  840,  840,  840,  624,
 /*   500 */   840,  626,  840,  840,  840,  625,  840,  627,  840,  840,
 /*   510 */   612,  615,  840,  840,  840,  840,  840,  618,  840,  840,
 /*   520 */   840,  840,  619,  606,  840,  840,  840,  608,  840,  840,
 /*   530 */   610,  840,  840,  609,  840,  840,  611,  613,  840,  840,
 /*   540 */   840,  840,  840,  607,  840,  840,  592,  594,  593,  840,
 /*   550 */   840,  583,  840,  840,  840,  585,  840,  840,  586,  840,
 /*   560 */   840,  587,  840,  840,  840,  588,  840,  840,  589,  573,
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
  "FUNCTION",      "PARENTHESES_OPEN",  "INLINE",        "TYPE_INTEGER",
  "TYPE_UINTEGER",  "TYPE_LONG",     "TYPE_ULONG",    "TYPE_CHAR",   
  "TYPE_UCHAR",    "TYPE_DOUBLE",   "TYPE_BOOL",     "TYPE_STRING", 
  "TYPE_ARRAY",    "TYPE_VAR",      "BREAK",         "CONTINUE",    
  "IF",            "ELSE",          "SWITCH",        "CASE",        
  "COLON",         "DEFAULT",       "LOOP",          "WHILE",       
  "DO",            "FOR",           "IN",            "REVERSE",     
  "LET",           "ADDASSIGN",     "SUBASSIGN",     "MULASSIGN",   
  "DIVASSIGN",     "CONCATASSIGN",  "ARROW",         "STRING",      
  "SBRACKET_OPEN",  "SBRACKET_CLOSE",  "DOUBLECOLON",   "INCR",        
  "DECR",          "ECHO",          "RETURN",        "UNSET",       
  "THROW",         "INTEGER",       "CHAR",          "DOUBLE",      
  "NULL",          "TRUE",          "FALSE",         "error",       
  "program",       "xx_language",   "xx_top_statement_list",  "xx_top_statement",
  "xx_namespace_def",  "xx_class_def",  "xx_interface_def",  "xx_comment",  
  "xx_interface_body",  "xx_class_body",  "xx_implements_list",  "xx_class_definition",
  "xx_implements",  "xx_interface_methods_definition",  "xx_class_properties_definition",  "xx_class_consts_definition",
  "xx_class_methods_definition",  "xx_class_property_definition",  "xx_visibility_list",  "xx_literal_expr",
  "xx_class_const_definition",  "xx_class_method_definition",  "xx_interface_method_definition",  "xx_parameter_list",
  "xx_statement_list",  "xx_visibility",  "xx_parameter",  "xx_parameter_type",
  "xx_parameter_cast",  "xx_statement",  "xx_let_statement",  "xx_if_statement",
  "xx_loop_statement",  "xx_echo_statement",  "xx_return_statement",  "xx_fcall_statement",
  "xx_mcall_statement",  "xx_scall_statement",  "xx_unset_statement",  "xx_throw_statement",
  "xx_declare_statement",  "xx_break_statement",  "xx_continue_statement",  "xx_while_statement",
  "xx_do_while_statement",  "xx_switch_statement",  "xx_for_statement",  "xx_eval_expr",
  "xx_case_clauses",  "xx_case_clause",  "xx_common_expr",  "xx_let_assignments",
  "xx_let_assignment",  "xx_assignment_operator",  "xx_assign_expr",  "xx_index_expr",
  "xx_echo_expressions",  "xx_echo_expression",  "xx_mcall_expr",  "xx_fcall_expr",
  "xx_scall_expr",  "xx_declare_variable_list",  "xx_declare_variable",  "xx_isset_expr",
  "xx_array_list",  "xx_call_parameters",  "xx_call_parameter",  "xx_array_item",
  "xx_array_key",  "xx_array_value",
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
 /*  42 */ "xx_class_consts_definition ::= xx_class_consts_definition xx_class_const_definition",
 /*  43 */ "xx_class_consts_definition ::= xx_class_const_definition",
 /*  44 */ "xx_class_methods_definition ::= xx_class_methods_definition xx_class_method_definition",
 /*  45 */ "xx_class_methods_definition ::= xx_class_method_definition",
 /*  46 */ "xx_interface_methods_definition ::= xx_interface_methods_definition xx_interface_method_definition",
 /*  47 */ "xx_interface_methods_definition ::= xx_interface_method_definition",
 /*  48 */ "xx_class_const_definition ::= COMMENT CONST CONSTANT ASSIGN xx_literal_expr DOTCOMMA",
 /*  49 */ "xx_class_const_definition ::= CONST CONSTANT ASSIGN xx_literal_expr DOTCOMMA",
 /*  50 */ "xx_class_method_definition ::= xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE BRACKET_OPEN BRACKET_CLOSE",
 /*  51 */ "xx_class_method_definition ::= xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE BRACKET_OPEN BRACKET_CLOSE",
 /*  52 */ "xx_class_method_definition ::= xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  53 */ "xx_class_method_definition ::= xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  54 */ "xx_class_method_definition ::= COMMENT xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE BRACKET_OPEN BRACKET_CLOSE",
 /*  55 */ "xx_class_method_definition ::= COMMENT xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE BRACKET_OPEN BRACKET_CLOSE",
 /*  56 */ "xx_class_method_definition ::= COMMENT xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  57 */ "xx_class_method_definition ::= COMMENT xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  58 */ "xx_interface_method_definition ::= xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE DOTCOMMA",
 /*  59 */ "xx_interface_method_definition ::= xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE DOTCOMMA",
 /*  60 */ "xx_interface_method_definition ::= COMMENT xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE DOTCOMMA",
 /*  61 */ "xx_interface_method_definition ::= COMMENT xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE DOTCOMMA",
 /*  62 */ "xx_visibility_list ::= xx_visibility_list xx_visibility",
 /*  63 */ "xx_visibility_list ::= xx_visibility",
 /*  64 */ "xx_visibility ::= PUBLIC",
 /*  65 */ "xx_visibility ::= PROTECTED",
 /*  66 */ "xx_visibility ::= PRIVATE",
 /*  67 */ "xx_visibility ::= STATIC",
 /*  68 */ "xx_visibility ::= SCOPED",
 /*  69 */ "xx_visibility ::= INLINE",
 /*  70 */ "xx_visibility ::= ABSTRACT",
 /*  71 */ "xx_visibility ::= FINAL",
 /*  72 */ "xx_parameter_list ::= xx_parameter_list COMMA xx_parameter",
 /*  73 */ "xx_parameter_list ::= xx_parameter",
 /*  74 */ "xx_parameter ::= IDENTIFIER",
 /*  75 */ "xx_parameter ::= xx_parameter_type IDENTIFIER",
 /*  76 */ "xx_parameter ::= xx_parameter_cast IDENTIFIER",
 /*  77 */ "xx_parameter ::= IDENTIFIER ASSIGN xx_literal_expr",
 /*  78 */ "xx_parameter ::= xx_parameter_type IDENTIFIER ASSIGN xx_literal_expr",
 /*  79 */ "xx_parameter ::= xx_parameter_cast IDENTIFIER ASSIGN xx_literal_expr",
 /*  80 */ "xx_parameter_cast ::= LESS IDENTIFIER GREATER",
 /*  81 */ "xx_parameter_type ::= TYPE_INTEGER",
 /*  82 */ "xx_parameter_type ::= TYPE_UINTEGER",
 /*  83 */ "xx_parameter_type ::= TYPE_LONG",
 /*  84 */ "xx_parameter_type ::= TYPE_ULONG",
 /*  85 */ "xx_parameter_type ::= TYPE_CHAR",
 /*  86 */ "xx_parameter_type ::= TYPE_UCHAR",
 /*  87 */ "xx_parameter_type ::= TYPE_DOUBLE",
 /*  88 */ "xx_parameter_type ::= TYPE_BOOL",
 /*  89 */ "xx_parameter_type ::= TYPE_STRING",
 /*  90 */ "xx_parameter_type ::= TYPE_ARRAY",
 /*  91 */ "xx_parameter_type ::= TYPE_VAR",
 /*  92 */ "xx_statement_list ::= xx_statement_list xx_statement",
 /*  93 */ "xx_statement_list ::= xx_statement",
 /*  94 */ "xx_statement ::= xx_let_statement",
 /*  95 */ "xx_statement ::= xx_if_statement",
 /*  96 */ "xx_statement ::= xx_loop_statement",
 /*  97 */ "xx_statement ::= xx_echo_statement",
 /*  98 */ "xx_statement ::= xx_return_statement",
 /*  99 */ "xx_statement ::= xx_fcall_statement",
 /* 100 */ "xx_statement ::= xx_mcall_statement",
 /* 101 */ "xx_statement ::= xx_scall_statement",
 /* 102 */ "xx_statement ::= xx_unset_statement",
 /* 103 */ "xx_statement ::= xx_throw_statement",
 /* 104 */ "xx_statement ::= xx_declare_statement",
 /* 105 */ "xx_statement ::= xx_break_statement",
 /* 106 */ "xx_statement ::= xx_continue_statement",
 /* 107 */ "xx_statement ::= xx_while_statement",
 /* 108 */ "xx_statement ::= xx_do_while_statement",
 /* 109 */ "xx_statement ::= xx_switch_statement",
 /* 110 */ "xx_statement ::= xx_for_statement",
 /* 111 */ "xx_statement ::= xx_comment",
 /* 112 */ "xx_break_statement ::= BREAK DOTCOMMA",
 /* 113 */ "xx_continue_statement ::= CONTINUE DOTCOMMA",
 /* 114 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN BRACKET_CLOSE",
 /* 115 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN BRACKET_CLOSE ELSE BRACKET_OPEN BRACKET_CLOSE",
 /* 116 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 117 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE ELSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 118 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE ELSE BRACKET_OPEN BRACKET_CLOSE",
 /* 119 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN BRACKET_CLOSE ELSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 120 */ "xx_switch_statement ::= SWITCH xx_eval_expr BRACKET_OPEN BRACKET_CLOSE",
 /* 121 */ "xx_switch_statement ::= SWITCH xx_eval_expr BRACKET_OPEN xx_case_clauses BRACKET_CLOSE",
 /* 122 */ "xx_case_clauses ::= xx_case_clauses xx_case_clause",
 /* 123 */ "xx_case_clauses ::= xx_case_clause",
 /* 124 */ "xx_case_clause ::= CASE xx_literal_expr COLON",
 /* 125 */ "xx_case_clause ::= CASE xx_literal_expr COLON xx_statement_list",
 /* 126 */ "xx_case_clause ::= DEFAULT COLON xx_statement_list",
 /* 127 */ "xx_loop_statement ::= LOOP BRACKET_OPEN BRACKET_CLOSE",
 /* 128 */ "xx_loop_statement ::= LOOP BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 129 */ "xx_while_statement ::= WHILE xx_eval_expr BRACKET_OPEN BRACKET_CLOSE",
 /* 130 */ "xx_while_statement ::= WHILE xx_eval_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 131 */ "xx_do_while_statement ::= DO BRACKET_OPEN BRACKET_CLOSE WHILE xx_eval_expr DOTCOMMA",
 /* 132 */ "xx_do_while_statement ::= DO BRACKET_OPEN xx_statement_list BRACKET_CLOSE WHILE xx_eval_expr DOTCOMMA",
 /* 133 */ "xx_for_statement ::= FOR IDENTIFIER IN xx_common_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 134 */ "xx_for_statement ::= FOR IDENTIFIER IN REVERSE xx_common_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 135 */ "xx_for_statement ::= FOR IDENTIFIER COMMA IDENTIFIER IN xx_common_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 136 */ "xx_for_statement ::= FOR IDENTIFIER COMMA IDENTIFIER IN REVERSE xx_common_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 137 */ "xx_let_statement ::= LET xx_let_assignments DOTCOMMA",
 /* 138 */ "xx_let_assignments ::= xx_let_assignments COMMA xx_let_assignment",
 /* 139 */ "xx_let_assignments ::= xx_let_assignment",
 /* 140 */ "xx_assignment_operator ::= ASSIGN",
 /* 141 */ "xx_assignment_operator ::= ADDASSIGN",
 /* 142 */ "xx_assignment_operator ::= SUBASSIGN",
 /* 143 */ "xx_assignment_operator ::= MULASSIGN",
 /* 144 */ "xx_assignment_operator ::= DIVASSIGN",
 /* 145 */ "xx_assignment_operator ::= CONCATASSIGN",
 /* 146 */ "xx_let_assignment ::= IDENTIFIER xx_assignment_operator xx_assign_expr",
 /* 147 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER xx_assignment_operator xx_assign_expr",
 /* 148 */ "xx_let_assignment ::= IDENTIFIER ARROW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 149 */ "xx_let_assignment ::= IDENTIFIER ARROW BRACKET_OPEN STRING BRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 150 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER SBRACKET_OPEN SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 151 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER SBRACKET_OPEN xx_index_expr SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 152 */ "xx_let_assignment ::= IDENTIFIER DOUBLECOLON IDENTIFIER xx_assignment_operator xx_assign_expr",
 /* 153 */ "xx_let_assignment ::= IDENTIFIER DOUBLECOLON IDENTIFIER SBRACKET_OPEN SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 154 */ "xx_let_assignment ::= IDENTIFIER DOUBLECOLON IDENTIFIER SBRACKET_OPEN xx_index_expr SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 155 */ "xx_let_assignment ::= IDENTIFIER SBRACKET_OPEN SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 156 */ "xx_let_assignment ::= IDENTIFIER SBRACKET_OPEN xx_index_expr SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 157 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER INCR",
 /* 158 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER DECR",
 /* 159 */ "xx_let_assignment ::= IDENTIFIER INCR",
 /* 160 */ "xx_let_assignment ::= IDENTIFIER DECR",
 /* 161 */ "xx_index_expr ::= xx_common_expr",
 /* 162 */ "xx_echo_statement ::= ECHO xx_echo_expressions DOTCOMMA",
 /* 163 */ "xx_echo_expressions ::= xx_echo_expressions COMMA xx_echo_expression",
 /* 164 */ "xx_echo_expressions ::= xx_echo_expression",
 /* 165 */ "xx_echo_expression ::= xx_common_expr",
 /* 166 */ "xx_mcall_statement ::= xx_mcall_expr DOTCOMMA",
 /* 167 */ "xx_fcall_statement ::= xx_fcall_expr DOTCOMMA",
 /* 168 */ "xx_scall_statement ::= xx_scall_expr DOTCOMMA",
 /* 169 */ "xx_return_statement ::= RETURN xx_common_expr DOTCOMMA",
 /* 170 */ "xx_return_statement ::= RETURN DOTCOMMA",
 /* 171 */ "xx_unset_statement ::= UNSET IDENTIFIER ARROW IDENTIFIER DOTCOMMA",
 /* 172 */ "xx_unset_statement ::= UNSET IDENTIFIER SBRACKET_OPEN xx_common_expr SBRACKET_CLOSE DOTCOMMA",
 /* 173 */ "xx_throw_statement ::= THROW xx_common_expr DOTCOMMA",
 /* 174 */ "xx_declare_statement ::= TYPE_INTEGER xx_declare_variable_list DOTCOMMA",
 /* 175 */ "xx_declare_statement ::= TYPE_UINTEGER xx_declare_variable_list DOTCOMMA",
 /* 176 */ "xx_declare_statement ::= TYPE_CHAR xx_declare_variable_list DOTCOMMA",
 /* 177 */ "xx_declare_statement ::= TYPE_UCHAR xx_declare_variable_list DOTCOMMA",
 /* 178 */ "xx_declare_statement ::= TYPE_LONG xx_declare_variable_list DOTCOMMA",
 /* 179 */ "xx_declare_statement ::= TYPE_ULONG xx_declare_variable_list DOTCOMMA",
 /* 180 */ "xx_declare_statement ::= TYPE_DOUBLE xx_declare_variable_list DOTCOMMA",
 /* 181 */ "xx_declare_statement ::= TYPE_STRING xx_declare_variable_list DOTCOMMA",
 /* 182 */ "xx_declare_statement ::= TYPE_BOOL xx_declare_variable_list DOTCOMMA",
 /* 183 */ "xx_declare_statement ::= TYPE_VAR xx_declare_variable_list DOTCOMMA",
 /* 184 */ "xx_declare_variable_list ::= xx_declare_variable_list COMMA xx_declare_variable",
 /* 185 */ "xx_declare_variable_list ::= xx_declare_variable",
 /* 186 */ "xx_declare_variable ::= IDENTIFIER",
 /* 187 */ "xx_declare_variable ::= IDENTIFIER ASSIGN xx_literal_expr",
 /* 188 */ "xx_assign_expr ::= xx_common_expr",
 /* 189 */ "xx_common_expr ::= NOT xx_common_expr",
 /* 190 */ "xx_common_expr ::= ISSET xx_isset_expr",
 /* 191 */ "xx_common_expr ::= REQUIRE xx_common_expr",
 /* 192 */ "xx_common_expr ::= CLONE xx_common_expr",
 /* 193 */ "xx_common_expr ::= EMPTY xx_common_expr",
 /* 194 */ "xx_common_expr ::= xx_common_expr EQUALS xx_common_expr",
 /* 195 */ "xx_common_expr ::= xx_common_expr NOTEQUALS xx_common_expr",
 /* 196 */ "xx_common_expr ::= xx_common_expr IDENTICAL xx_common_expr",
 /* 197 */ "xx_common_expr ::= xx_common_expr NOTIDENTICAL xx_common_expr",
 /* 198 */ "xx_common_expr ::= xx_common_expr LESS xx_common_expr",
 /* 199 */ "xx_common_expr ::= xx_common_expr GREATER xx_common_expr",
 /* 200 */ "xx_common_expr ::= xx_common_expr LESSEQUAL xx_common_expr",
 /* 201 */ "xx_common_expr ::= xx_common_expr GREATEREQUAL xx_common_expr",
 /* 202 */ "xx_common_expr ::= PARENTHESES_OPEN xx_common_expr PARENTHESES_CLOSE",
 /* 203 */ "xx_common_expr ::= PARENTHESES_OPEN xx_parameter_type PARENTHESES_CLOSE xx_common_expr",
 /* 204 */ "xx_common_expr ::= LESS IDENTIFIER GREATER xx_common_expr",
 /* 205 */ "xx_common_expr ::= IDENTIFIER ARROW IDENTIFIER",
 /* 206 */ "xx_common_expr ::= IDENTIFIER DOUBLECOLON IDENTIFIER",
 /* 207 */ "xx_common_expr ::= IDENTIFIER DOUBLECOLON CONSTANT",
 /* 208 */ "xx_common_expr ::= IDENTIFIER SBRACKET_OPEN xx_common_expr SBRACKET_CLOSE",
 /* 209 */ "xx_common_expr ::= xx_common_expr ADD xx_common_expr",
 /* 210 */ "xx_common_expr ::= xx_common_expr SUB xx_common_expr",
 /* 211 */ "xx_common_expr ::= xx_common_expr MUL xx_common_expr",
 /* 212 */ "xx_common_expr ::= xx_common_expr DIV xx_common_expr",
 /* 213 */ "xx_common_expr ::= xx_common_expr MOD xx_common_expr",
 /* 214 */ "xx_common_expr ::= xx_common_expr CONCAT xx_common_expr",
 /* 215 */ "xx_common_expr ::= xx_common_expr AND xx_common_expr",
 /* 216 */ "xx_common_expr ::= xx_common_expr OR xx_common_expr",
 /* 217 */ "xx_common_expr ::= xx_common_expr INSTANCEOF xx_common_expr",
 /* 218 */ "xx_common_expr ::= FETCH IDENTIFIER COMMA xx_isset_expr",
 /* 219 */ "xx_common_expr ::= TYPEOF xx_common_expr",
 /* 220 */ "xx_common_expr ::= CONSTANT",
 /* 221 */ "xx_common_expr ::= IDENTIFIER",
 /* 222 */ "xx_common_expr ::= INTEGER",
 /* 223 */ "xx_common_expr ::= STRING",
 /* 224 */ "xx_common_expr ::= CHAR",
 /* 225 */ "xx_common_expr ::= DOUBLE",
 /* 226 */ "xx_common_expr ::= NULL",
 /* 227 */ "xx_common_expr ::= TRUE",
 /* 228 */ "xx_common_expr ::= FALSE",
 /* 229 */ "xx_common_expr ::= SBRACKET_OPEN SBRACKET_CLOSE",
 /* 230 */ "xx_common_expr ::= SBRACKET_OPEN xx_array_list SBRACKET_CLOSE",
 /* 231 */ "xx_common_expr ::= NEW IDENTIFIER",
 /* 232 */ "xx_common_expr ::= NEW IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 233 */ "xx_common_expr ::= NEW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 234 */ "xx_fcall_expr ::= IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 235 */ "xx_fcall_expr ::= IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 236 */ "xx_scall_expr ::= IDENTIFIER DOUBLECOLON IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 237 */ "xx_scall_expr ::= IDENTIFIER DOUBLECOLON IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 238 */ "xx_mcall_expr ::= IDENTIFIER ARROW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 239 */ "xx_mcall_expr ::= IDENTIFIER ARROW IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 240 */ "xx_mcall_expr ::= IDENTIFIER ARROW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 241 */ "xx_mcall_expr ::= IDENTIFIER ARROW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 242 */ "xx_mcall_expr ::= IDENTIFIER ARROW BRACKET_OPEN STRING BRACKET_CLOSE PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 243 */ "xx_mcall_expr ::= IDENTIFIER ARROW BRACKET_OPEN STRING BRACKET_CLOSE PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 244 */ "xx_common_expr ::= xx_mcall_expr",
 /* 245 */ "xx_common_expr ::= xx_scall_expr",
 /* 246 */ "xx_common_expr ::= xx_fcall_expr",
 /* 247 */ "xx_call_parameters ::= xx_call_parameters COMMA xx_call_parameter",
 /* 248 */ "xx_call_parameters ::= xx_call_parameter",
 /* 249 */ "xx_call_parameter ::= xx_common_expr",
 /* 250 */ "xx_array_list ::= xx_array_list COMMA xx_array_item",
 /* 251 */ "xx_array_list ::= xx_array_item",
 /* 252 */ "xx_array_item ::= xx_array_key COLON xx_array_value",
 /* 253 */ "xx_array_item ::= xx_array_value",
 /* 254 */ "xx_array_key ::= IDENTIFIER",
 /* 255 */ "xx_array_key ::= STRING",
 /* 256 */ "xx_array_key ::= INTEGER",
 /* 257 */ "xx_array_value ::= xx_common_expr",
 /* 258 */ "xx_literal_expr ::= INTEGER",
 /* 259 */ "xx_literal_expr ::= CHAR",
 /* 260 */ "xx_literal_expr ::= STRING",
 /* 261 */ "xx_literal_expr ::= DOUBLE",
 /* 262 */ "xx_literal_expr ::= NULL",
 /* 263 */ "xx_literal_expr ::= FALSE",
 /* 264 */ "xx_literal_expr ::= TRUE",
 /* 265 */ "xx_literal_expr ::= IDENTIFIER DOUBLECOLON CONSTANT",
 /* 266 */ "xx_isset_expr ::= IDENTIFIER SBRACKET_OPEN xx_common_expr SBRACKET_CLOSE",
 /* 267 */ "xx_isset_expr ::= IDENTIFIER ARROW IDENTIFIER",
 /* 268 */ "xx_eval_expr ::= xx_common_expr",
 /* 269 */ "xx_comment ::= COMMENT",
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
// 900 "parser.lemon"
{
	/*if ((yypminor->yy0)) {
		if ((yypminor->yy0)->free_flag) {
			efree((yypminor->yy0)->token);
		}
		efree((yypminor->yy0));
	}*/
}
// 2386 "parser.c"
      break;
    case 101:
// 913 "parser.lemon"
{ json_object_put((yypminor->yy115)); }
// 2391 "parser.c"
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
  { 115, 2 },
  { 115, 1 },
  { 116, 2 },
  { 116, 1 },
  { 113, 2 },
  { 113, 1 },
  { 120, 6 },
  { 120, 5 },
  { 121, 7 },
  { 121, 8 },
  { 121, 8 },
  { 121, 9 },
  { 121, 8 },
  { 121, 9 },
  { 121, 9 },
  { 121, 10 },
  { 122, 6 },
  { 122, 7 },
  { 122, 7 },
  { 122, 8 },
  { 118, 2 },
  { 118, 1 },
  { 125, 1 },
  { 125, 1 },
  { 125, 1 },
  { 125, 1 },
  { 125, 1 },
  { 125, 1 },
  { 125, 1 },
  { 125, 1 },
  { 123, 3 },
  { 123, 1 },
  { 126, 1 },
  { 126, 2 },
  { 126, 2 },
  { 126, 3 },
  { 126, 4 },
  { 126, 4 },
  { 128, 3 },
  { 127, 1 },
  { 127, 1 },
  { 127, 1 },
  { 127, 1 },
  { 127, 1 },
  { 127, 1 },
  { 127, 1 },
  { 127, 1 },
  { 127, 1 },
  { 127, 1 },
  { 127, 1 },
  { 124, 2 },
  { 124, 1 },
  { 129, 1 },
  { 129, 1 },
  { 129, 1 },
  { 129, 1 },
  { 129, 1 },
  { 129, 1 },
  { 129, 1 },
  { 129, 1 },
  { 129, 1 },
  { 129, 1 },
  { 129, 1 },
  { 129, 1 },
  { 129, 1 },
  { 129, 1 },
  { 129, 1 },
  { 129, 1 },
  { 129, 1 },
  { 129, 1 },
  { 141, 2 },
  { 142, 2 },
  { 131, 4 },
  { 131, 7 },
  { 131, 5 },
  { 131, 9 },
  { 131, 8 },
  { 131, 8 },
  { 145, 4 },
  { 145, 5 },
  { 148, 2 },
  { 148, 1 },
  { 149, 3 },
  { 149, 4 },
  { 149, 3 },
  { 132, 3 },
  { 132, 4 },
  { 143, 4 },
  { 143, 5 },
  { 144, 6 },
  { 144, 7 },
  { 146, 7 },
  { 146, 8 },
  { 146, 9 },
  { 146, 10 },
  { 130, 3 },
  { 151, 3 },
  { 151, 1 },
  { 153, 1 },
  { 153, 1 },
  { 153, 1 },
  { 153, 1 },
  { 153, 1 },
  { 153, 1 },
  { 152, 3 },
  { 152, 5 },
  { 152, 7 },
  { 152, 7 },
  { 152, 7 },
  { 152, 8 },
  { 152, 5 },
  { 152, 7 },
  { 152, 8 },
  { 152, 5 },
  { 152, 6 },
  { 152, 4 },
  { 152, 4 },
  { 152, 2 },
  { 152, 2 },
  { 155, 1 },
  { 133, 3 },
  { 156, 3 },
  { 156, 1 },
  { 157, 1 },
  { 136, 2 },
  { 135, 2 },
  { 137, 2 },
  { 134, 3 },
  { 134, 2 },
  { 138, 5 },
  { 138, 6 },
  { 139, 3 },
  { 140, 3 },
  { 140, 3 },
  { 140, 3 },
  { 140, 3 },
  { 140, 3 },
  { 140, 3 },
  { 140, 3 },
  { 140, 3 },
  { 140, 3 },
  { 140, 3 },
  { 161, 3 },
  { 161, 1 },
  { 162, 1 },
  { 162, 3 },
  { 154, 1 },
  { 150, 2 },
  { 150, 2 },
  { 150, 2 },
  { 150, 2 },
  { 150, 2 },
  { 150, 3 },
  { 150, 3 },
  { 150, 3 },
  { 150, 3 },
  { 150, 3 },
  { 150, 3 },
  { 150, 3 },
  { 150, 3 },
  { 150, 3 },
  { 150, 4 },
  { 150, 4 },
  { 150, 3 },
  { 150, 3 },
  { 150, 3 },
  { 150, 4 },
  { 150, 3 },
  { 150, 3 },
  { 150, 3 },
  { 150, 3 },
  { 150, 3 },
  { 150, 3 },
  { 150, 3 },
  { 150, 3 },
  { 150, 3 },
  { 150, 4 },
  { 150, 2 },
  { 150, 1 },
  { 150, 1 },
  { 150, 1 },
  { 150, 1 },
  { 150, 1 },
  { 150, 1 },
  { 150, 1 },
  { 150, 1 },
  { 150, 1 },
  { 150, 2 },
  { 150, 3 },
  { 150, 2 },
  { 150, 4 },
  { 150, 5 },
  { 159, 4 },
  { 159, 3 },
  { 160, 6 },
  { 160, 5 },
  { 158, 6 },
  { 158, 5 },
  { 158, 8 },
  { 158, 7 },
  { 158, 8 },
  { 158, 7 },
  { 150, 1 },
  { 150, 1 },
  { 150, 1 },
  { 165, 3 },
  { 165, 1 },
  { 166, 1 },
  { 164, 3 },
  { 164, 1 },
  { 167, 3 },
  { 167, 1 },
  { 168, 1 },
  { 168, 1 },
  { 168, 1 },
  { 169, 1 },
  { 119, 1 },
  { 119, 1 },
  { 119, 1 },
  { 119, 1 },
  { 119, 1 },
  { 119, 1 },
  { 119, 1 },
  { 119, 3 },
  { 163, 4 },
  { 163, 3 },
  { 147, 1 },
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
// 909 "parser.lemon"
{
	status->ret = yymsp[0].minor.yy115;
}
// 2878 "parser.c"
        break;
      case 1:
      case 4:
      case 5:
      case 6:
      case 7:
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
      case 105:
      case 106:
      case 107:
      case 108:
      case 109:
      case 110:
      case 111:
      case 161:
      case 188:
      case 244:
      case 245:
      case 246:
      case 249:
      case 257:
      case 268:
// 915 "parser.lemon"
{
	yygotominor.yy115 = yymsp[0].minor.yy115;
}
// 2915 "parser.c"
        break;
      case 2:
      case 36:
      case 42:
      case 44:
      case 46:
      case 62:
      case 92:
      case 122:
// 919 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_list(yymsp[-1].minor.yy115, yymsp[0].minor.yy115);
}
// 2929 "parser.c"
        break;
      case 3:
      case 23:
      case 37:
      case 43:
      case 45:
      case 47:
      case 63:
      case 73:
      case 93:
      case 123:
      case 139:
      case 164:
      case 185:
      case 248:
      case 251:
// 923 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_list(NULL, yymsp[0].minor.yy115);
}
// 2950 "parser.c"
        break;
      case 8:
// 943 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_namespace(yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(33,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 2959 "parser.c"
        break;
      case 9:
// 947 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_interface(yymsp[-1].minor.yy0, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(36,&yymsp[-2].minor);
}
// 2967 "parser.c"
        break;
      case 10:
// 951 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_interface(yymsp[-3].minor.yy0, yymsp[0].minor.yy115, yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(36,&yymsp[-4].minor);
  yy_destructor(37,&yymsp[-2].minor);
}
// 2976 "parser.c"
        break;
      case 11:
// 955 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class(yymsp[-1].minor.yy0, yymsp[0].minor.yy115, 0, 0, NULL, NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-2].minor);
}
// 2984 "parser.c"
        break;
      case 12:
// 959 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy115, 0, 0, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-4].minor);
  yy_destructor(37,&yymsp[-2].minor);
}
// 2993 "parser.c"
        break;
      case 13:
// 963 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy115, 0, 0, NULL, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(38,&yymsp[-4].minor);
  yy_destructor(39,&yymsp[-2].minor);
}
// 3002 "parser.c"
        break;
      case 14:
// 967 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class(yymsp[-5].minor.yy0, yymsp[0].minor.yy115, 0, 0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(38,&yymsp[-6].minor);
  yy_destructor(37,&yymsp[-4].minor);
  yy_destructor(39,&yymsp[-2].minor);
}
// 3012 "parser.c"
        break;
      case 15:
// 971 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class(yymsp[-1].minor.yy0, yymsp[0].minor.yy115, 1, 0, NULL, NULL, status->scanner_state);
  yy_destructor(40,&yymsp[-3].minor);
  yy_destructor(38,&yymsp[-2].minor);
}
// 3021 "parser.c"
        break;
      case 16:
// 975 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy115, 1, 0, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(40,&yymsp[-5].minor);
  yy_destructor(38,&yymsp[-4].minor);
  yy_destructor(37,&yymsp[-2].minor);
}
// 3031 "parser.c"
        break;
      case 17:
// 979 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy115, 1, 0, NULL, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(40,&yymsp[-5].minor);
  yy_destructor(38,&yymsp[-4].minor);
  yy_destructor(39,&yymsp[-2].minor);
}
// 3041 "parser.c"
        break;
      case 18:
// 983 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class(yymsp[-1].minor.yy0, yymsp[0].minor.yy115, 0, 1, NULL, NULL, status->scanner_state);
  yy_destructor(41,&yymsp[-3].minor);
  yy_destructor(38,&yymsp[-2].minor);
}
// 3050 "parser.c"
        break;
      case 19:
// 987 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy115, 0, 1, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(41,&yymsp[-5].minor);
  yy_destructor(38,&yymsp[-4].minor);
  yy_destructor(37,&yymsp[-2].minor);
}
// 3060 "parser.c"
        break;
      case 20:
      case 25:
// 991 "parser.lemon"
{
	yygotominor.yy115 = NULL;
  yy_destructor(42,&yymsp[-1].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3070 "parser.c"
        break;
      case 21:
      case 26:
// 995 "parser.lemon"
{
	yygotominor.yy115 = yymsp[-1].minor.yy115;
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3080 "parser.c"
        break;
      case 22:
      case 72:
      case 138:
      case 163:
      case 184:
      case 247:
      case 250:
// 999 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_list(yymsp[-2].minor.yy115, yymsp[0].minor.yy115);
  yy_destructor(6,&yymsp[-1].minor);
}
// 3094 "parser.c"
        break;
      case 24:
      case 221:
      case 254:
// 1007 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state);
}
// 3103 "parser.c"
        break;
      case 27:
// 1019 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_definition(yymsp[0].minor.yy115, NULL, NULL, status->scanner_state);
}
// 3110 "parser.c"
        break;
      case 28:
// 1023 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_definition(NULL, NULL, yymsp[0].minor.yy115, status->scanner_state);
}
// 3117 "parser.c"
        break;
      case 29:
// 1027 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_definition(NULL, yymsp[0].minor.yy115, NULL, status->scanner_state);
}
// 3124 "parser.c"
        break;
      case 30:
// 1031 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_definition(yymsp[-1].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
}
// 3131 "parser.c"
        break;
      case 31:
// 1035 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_definition(yymsp[-1].minor.yy115, NULL, yymsp[0].minor.yy115, status->scanner_state);
}
// 3138 "parser.c"
        break;
      case 32:
// 1039 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_definition(yymsp[0].minor.yy115, NULL, yymsp[-1].minor.yy115, status->scanner_state);
}
// 3145 "parser.c"
        break;
      case 33:
// 1043 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_definition(NULL, yymsp[0].minor.yy115, yymsp[-1].minor.yy115, status->scanner_state);
}
// 3152 "parser.c"
        break;
      case 34:
// 1047 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_definition(yymsp[-2].minor.yy115, yymsp[0].minor.yy115, yymsp[-1].minor.yy115, status->scanner_state);
}
// 3159 "parser.c"
        break;
      case 35:
// 1051 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_definition(yymsp[-1].minor.yy115, yymsp[0].minor.yy115, yymsp[-2].minor.yy115, status->scanner_state);
}
// 3166 "parser.c"
        break;
      case 38:
// 1067 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_property(yymsp[-2].minor.yy115, yymsp[-1].minor.yy0, NULL, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(35,&yymsp[0].minor);
}
// 3174 "parser.c"
        break;
      case 39:
// 1071 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_property(yymsp[-2].minor.yy115, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(35,&yymsp[0].minor);
}
// 3182 "parser.c"
        break;
      case 40:
// 1075 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_property(yymsp[-4].minor.yy115, yymsp[-3].minor.yy0, yymsp[-1].minor.yy115, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(45,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3191 "parser.c"
        break;
      case 41:
// 1079 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_property(yymsp[-4].minor.yy115, yymsp[-3].minor.yy0, yymsp[-1].minor.yy115, NULL, status->scanner_state);
  yy_destructor(45,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3200 "parser.c"
        break;
      case 48:
// 1107 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy115, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(46,&yymsp[-4].minor);
  yy_destructor(45,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3210 "parser.c"
        break;
      case 49:
// 1111 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy115, NULL, status->scanner_state);
  yy_destructor(46,&yymsp[-4].minor);
  yy_destructor(45,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3220 "parser.c"
        break;
      case 50:
// 1115 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_method(yymsp[-6].minor.yy115, yymsp[-4].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(48,&yymsp[-5].minor);
  yy_destructor(49,&yymsp[-3].minor);
  yy_destructor(32,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[-1].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3232 "parser.c"
        break;
      case 51:
// 1119 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_method(yymsp[-7].minor.yy115, yymsp[-5].minor.yy0, yymsp[-3].minor.yy115, NULL, NULL, status->scanner_state);
  yy_destructor(48,&yymsp[-6].minor);
  yy_destructor(49,&yymsp[-4].minor);
  yy_destructor(32,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[-1].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3244 "parser.c"
        break;
      case 52:
// 1123 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_method(yymsp[-7].minor.yy115, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy115, NULL, status->scanner_state);
  yy_destructor(48,&yymsp[-6].minor);
  yy_destructor(49,&yymsp[-4].minor);
  yy_destructor(32,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3256 "parser.c"
        break;
      case 53:
// 1127 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_method(yymsp[-8].minor.yy115, yymsp[-6].minor.yy0, yymsp[-4].minor.yy115, yymsp[-1].minor.yy115, NULL, status->scanner_state);
  yy_destructor(48,&yymsp[-7].minor);
  yy_destructor(49,&yymsp[-5].minor);
  yy_destructor(32,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3268 "parser.c"
        break;
      case 54:
// 1131 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_method(yymsp[-6].minor.yy115, yymsp[-4].minor.yy0, NULL, NULL, yymsp[-7].minor.yy0, status->scanner_state);
  yy_destructor(48,&yymsp[-5].minor);
  yy_destructor(49,&yymsp[-3].minor);
  yy_destructor(32,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[-1].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3280 "parser.c"
        break;
      case 55:
// 1135 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_method(yymsp[-7].minor.yy115, yymsp[-5].minor.yy0, yymsp[-3].minor.yy115, NULL, yymsp[-8].minor.yy0, status->scanner_state);
  yy_destructor(48,&yymsp[-6].minor);
  yy_destructor(49,&yymsp[-4].minor);
  yy_destructor(32,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[-1].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3292 "parser.c"
        break;
      case 56:
// 1139 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_method(yymsp[-7].minor.yy115, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy115, yymsp[-8].minor.yy0, status->scanner_state);
  yy_destructor(48,&yymsp[-6].minor);
  yy_destructor(49,&yymsp[-4].minor);
  yy_destructor(32,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3304 "parser.c"
        break;
      case 57:
// 1143 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_method(yymsp[-8].minor.yy115, yymsp[-6].minor.yy0, yymsp[-4].minor.yy115, yymsp[-1].minor.yy115, yymsp[-9].minor.yy0, status->scanner_state);
  yy_destructor(48,&yymsp[-7].minor);
  yy_destructor(49,&yymsp[-5].minor);
  yy_destructor(32,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3316 "parser.c"
        break;
      case 58:
// 1147 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_method(yymsp[-5].minor.yy115, yymsp[-3].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(48,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[-1].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3327 "parser.c"
        break;
      case 59:
// 1151 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_method(yymsp[-6].minor.yy115, yymsp[-4].minor.yy0, yymsp[-2].minor.yy115, NULL, NULL, status->scanner_state);
  yy_destructor(48,&yymsp[-5].minor);
  yy_destructor(49,&yymsp[-3].minor);
  yy_destructor(32,&yymsp[-1].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3338 "parser.c"
        break;
      case 60:
// 1155 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_method(yymsp[-5].minor.yy115, yymsp[-3].minor.yy0, NULL, NULL, yymsp[-6].minor.yy0, status->scanner_state);
  yy_destructor(48,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[-1].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3349 "parser.c"
        break;
      case 61:
// 1159 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_method(yymsp[-6].minor.yy115, yymsp[-4].minor.yy0, yymsp[-2].minor.yy115, NULL, yymsp[-7].minor.yy0, status->scanner_state);
  yy_destructor(48,&yymsp[-5].minor);
  yy_destructor(49,&yymsp[-3].minor);
  yy_destructor(32,&yymsp[-1].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3360 "parser.c"
        break;
      case 64:
// 1171 "parser.lemon"
{
	yygotominor.yy115 = json_object_new_string("public");
  yy_destructor(1,&yymsp[0].minor);
}
// 3368 "parser.c"
        break;
      case 65:
// 1175 "parser.lemon"
{
	yygotominor.yy115 = json_object_new_string("protected");
  yy_destructor(2,&yymsp[0].minor);
}
// 3376 "parser.c"
        break;
      case 66:
// 1179 "parser.lemon"
{
	yygotominor.yy115 = json_object_new_string("private");
  yy_destructor(4,&yymsp[0].minor);
}
// 3384 "parser.c"
        break;
      case 67:
// 1183 "parser.lemon"
{
	yygotominor.yy115 = json_object_new_string("static");
  yy_destructor(3,&yymsp[0].minor);
}
// 3392 "parser.c"
        break;
      case 68:
// 1187 "parser.lemon"
{
	yygotominor.yy115 = json_object_new_string("scoped");
  yy_destructor(5,&yymsp[0].minor);
}
// 3400 "parser.c"
        break;
      case 69:
// 1191 "parser.lemon"
{
	yygotominor.yy115 = json_object_new_string("inline");
  yy_destructor(50,&yymsp[0].minor);
}
// 3408 "parser.c"
        break;
      case 70:
// 1195 "parser.lemon"
{
	yygotominor.yy115 = json_object_new_string("abstract");
  yy_destructor(40,&yymsp[0].minor);
}
// 3416 "parser.c"
        break;
      case 71:
// 1199 "parser.lemon"
{
	yygotominor.yy115 = json_object_new_string("final");
  yy_destructor(41,&yymsp[0].minor);
}
// 3424 "parser.c"
        break;
      case 74:
// 1211 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_parameter(NULL, NULL, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 3431 "parser.c"
        break;
      case 75:
// 1215 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_parameter(yymsp[-1].minor.yy115, NULL, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 3438 "parser.c"
        break;
      case 76:
// 1219 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_parameter(NULL, yymsp[-1].minor.yy115, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 3445 "parser.c"
        break;
      case 77:
// 1223 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_parameter(NULL, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy115, status->scanner_state);
  yy_destructor(45,&yymsp[-1].minor);
}
// 3453 "parser.c"
        break;
      case 78:
// 1227 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_parameter(yymsp[-3].minor.yy115, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy115, status->scanner_state);
  yy_destructor(45,&yymsp[-1].minor);
}
// 3461 "parser.c"
        break;
      case 79:
// 1231 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_parameter(NULL, yymsp[-3].minor.yy115, yymsp[-2].minor.yy0, yymsp[0].minor.yy115, status->scanner_state);
  yy_destructor(45,&yymsp[-1].minor);
}
// 3469 "parser.c"
        break;
      case 80:
// 1235 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(15,&yymsp[-2].minor);
  yy_destructor(16,&yymsp[0].minor);
}
// 3478 "parser.c"
        break;
      case 81:
// 1239 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_type(XX_TYPE_INTEGER);
  yy_destructor(51,&yymsp[0].minor);
}
// 3486 "parser.c"
        break;
      case 82:
// 1243 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_type(XX_TYPE_UINTEGER);
  yy_destructor(52,&yymsp[0].minor);
}
// 3494 "parser.c"
        break;
      case 83:
// 1247 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_type(XX_TYPE_LONG);
  yy_destructor(53,&yymsp[0].minor);
}
// 3502 "parser.c"
        break;
      case 84:
// 1251 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_type(XX_TYPE_ULONG);
  yy_destructor(54,&yymsp[0].minor);
}
// 3510 "parser.c"
        break;
      case 85:
// 1255 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_type(XX_TYPE_CHAR);
  yy_destructor(55,&yymsp[0].minor);
}
// 3518 "parser.c"
        break;
      case 86:
// 1259 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_type(XX_TYPE_UCHAR);
  yy_destructor(56,&yymsp[0].minor);
}
// 3526 "parser.c"
        break;
      case 87:
// 1263 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_type(XX_TYPE_DOUBLE);
  yy_destructor(57,&yymsp[0].minor);
}
// 3534 "parser.c"
        break;
      case 88:
// 1267 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_type(XX_TYPE_BOOL);
  yy_destructor(58,&yymsp[0].minor);
}
// 3542 "parser.c"
        break;
      case 89:
// 1271 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_type(XX_TYPE_STRING);
  yy_destructor(59,&yymsp[0].minor);
}
// 3550 "parser.c"
        break;
      case 90:
// 1275 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_type(XX_TYPE_ARRAY);
  yy_destructor(60,&yymsp[0].minor);
}
// 3558 "parser.c"
        break;
      case 91:
// 1279 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_type(XX_TYPE_VAR);
  yy_destructor(61,&yymsp[0].minor);
}
// 3566 "parser.c"
        break;
      case 112:
// 1363 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_break_statement(status->scanner_state);
  yy_destructor(62,&yymsp[-1].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3575 "parser.c"
        break;
      case 113:
// 1367 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_continue_statement(status->scanner_state);
  yy_destructor(63,&yymsp[-1].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3584 "parser.c"
        break;
      case 114:
// 1371 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_if_statement(yymsp[-2].minor.yy115, NULL, NULL, status->scanner_state);
  yy_destructor(64,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-1].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3594 "parser.c"
        break;
      case 115:
// 1375 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_if_statement(yymsp[-5].minor.yy115, NULL, NULL, status->scanner_state);
  yy_destructor(64,&yymsp[-6].minor);
  yy_destructor(42,&yymsp[-4].minor);
  yy_destructor(43,&yymsp[-3].minor);
  yy_destructor(65,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[-1].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3607 "parser.c"
        break;
      case 116:
// 1379 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_if_statement(yymsp[-3].minor.yy115, yymsp[-1].minor.yy115, NULL, status->scanner_state);
  yy_destructor(64,&yymsp[-4].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3617 "parser.c"
        break;
      case 117:
// 1383 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_if_statement(yymsp[-7].minor.yy115, yymsp[-5].minor.yy115, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(64,&yymsp[-8].minor);
  yy_destructor(42,&yymsp[-6].minor);
  yy_destructor(43,&yymsp[-4].minor);
  yy_destructor(65,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3630 "parser.c"
        break;
      case 118:
// 1387 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_if_statement(yymsp[-6].minor.yy115, yymsp[-4].minor.yy115, NULL, status->scanner_state);
  yy_destructor(64,&yymsp[-7].minor);
  yy_destructor(42,&yymsp[-5].minor);
  yy_destructor(43,&yymsp[-3].minor);
  yy_destructor(65,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[-1].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3643 "parser.c"
        break;
      case 119:
// 1391 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_if_statement(yymsp[-6].minor.yy115, NULL, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(64,&yymsp[-7].minor);
  yy_destructor(42,&yymsp[-5].minor);
  yy_destructor(43,&yymsp[-4].minor);
  yy_destructor(65,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3656 "parser.c"
        break;
      case 120:
// 1395 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_switch_statement(yymsp[-2].minor.yy115, NULL, status->scanner_state);
  yy_destructor(66,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-1].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3666 "parser.c"
        break;
      case 121:
// 1399 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_switch_statement(yymsp[-3].minor.yy115, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(66,&yymsp[-4].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3676 "parser.c"
        break;
      case 124:
// 1411 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_case_clause(yymsp[-1].minor.yy115, NULL, status->scanner_state);
  yy_destructor(67,&yymsp[-2].minor);
  yy_destructor(68,&yymsp[0].minor);
}
// 3685 "parser.c"
        break;
      case 125:
// 1415 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_case_clause(yymsp[-2].minor.yy115, yymsp[0].minor.yy115, status->scanner_state);
  yy_destructor(67,&yymsp[-3].minor);
  yy_destructor(68,&yymsp[-1].minor);
}
// 3694 "parser.c"
        break;
      case 126:
// 1419 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_case_clause(NULL, yymsp[0].minor.yy115, status->scanner_state);
  yy_destructor(69,&yymsp[-2].minor);
  yy_destructor(68,&yymsp[-1].minor);
}
// 3703 "parser.c"
        break;
      case 127:
// 1423 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_loop_statement(NULL, status->scanner_state);
  yy_destructor(70,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[-1].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3713 "parser.c"
        break;
      case 128:
// 1427 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_loop_statement(yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(70,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3723 "parser.c"
        break;
      case 129:
// 1431 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_while_statement(yymsp[-2].minor.yy115, NULL, status->scanner_state);
  yy_destructor(71,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-1].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3733 "parser.c"
        break;
      case 130:
// 1435 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_while_statement(yymsp[-3].minor.yy115, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(71,&yymsp[-4].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3743 "parser.c"
        break;
      case 131:
// 1439 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_do_while_statement(yymsp[-1].minor.yy115, NULL, status->scanner_state);
  yy_destructor(72,&yymsp[-5].minor);
  yy_destructor(42,&yymsp[-4].minor);
  yy_destructor(43,&yymsp[-3].minor);
  yy_destructor(71,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3755 "parser.c"
        break;
      case 132:
// 1443 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_do_while_statement(yymsp[-1].minor.yy115, yymsp[-4].minor.yy115, status->scanner_state);
  yy_destructor(72,&yymsp[-6].minor);
  yy_destructor(42,&yymsp[-5].minor);
  yy_destructor(43,&yymsp[-3].minor);
  yy_destructor(71,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3767 "parser.c"
        break;
      case 133:
// 1447 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_for_statement(yymsp[-3].minor.yy115, NULL, yymsp[-5].minor.yy0, 0, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(73,&yymsp[-6].minor);
  yy_destructor(74,&yymsp[-4].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3778 "parser.c"
        break;
      case 134:
// 1451 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_for_statement(yymsp[-3].minor.yy115, NULL, yymsp[-6].minor.yy0, 1, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(73,&yymsp[-7].minor);
  yy_destructor(74,&yymsp[-5].minor);
  yy_destructor(75,&yymsp[-4].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3790 "parser.c"
        break;
      case 135:
// 1455 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_for_statement(yymsp[-3].minor.yy115, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, 0, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(73,&yymsp[-8].minor);
  yy_destructor(6,&yymsp[-6].minor);
  yy_destructor(74,&yymsp[-4].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3802 "parser.c"
        break;
      case 136:
// 1459 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_for_statement(yymsp[-3].minor.yy115, yymsp[-8].minor.yy0, yymsp[-6].minor.yy0, 1, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(73,&yymsp[-9].minor);
  yy_destructor(6,&yymsp[-7].minor);
  yy_destructor(74,&yymsp[-5].minor);
  yy_destructor(75,&yymsp[-4].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3815 "parser.c"
        break;
      case 137:
// 1463 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_let_statement(yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(76,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3824 "parser.c"
        break;
      case 140:
// 1476 "parser.lemon"
{
	yygotominor.yy115 = json_object_new_string("assign");
  yy_destructor(45,&yymsp[0].minor);
}
// 3832 "parser.c"
        break;
      case 141:
// 1481 "parser.lemon"
{
	yygotominor.yy115 = json_object_new_string("add-assign");
  yy_destructor(77,&yymsp[0].minor);
}
// 3840 "parser.c"
        break;
      case 142:
// 1486 "parser.lemon"
{
	yygotominor.yy115 = json_object_new_string("sub-assign");
  yy_destructor(78,&yymsp[0].minor);
}
// 3848 "parser.c"
        break;
      case 143:
// 1490 "parser.lemon"
{
	yygotominor.yy115 = json_object_new_string("mult-assign");
  yy_destructor(79,&yymsp[0].minor);
}
// 3856 "parser.c"
        break;
      case 144:
// 1494 "parser.lemon"
{
	yygotominor.yy115 = json_object_new_string("div-assign");
  yy_destructor(80,&yymsp[0].minor);
}
// 3864 "parser.c"
        break;
      case 145:
// 1498 "parser.lemon"
{
	yygotominor.yy115 = json_object_new_string("concat-assign");
  yy_destructor(81,&yymsp[0].minor);
}
// 3872 "parser.c"
        break;
      case 146:
// 1503 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_let_assignment("variable", yymsp[-1].minor.yy115, yymsp[-2].minor.yy0, NULL, NULL, yymsp[0].minor.yy115, status->scanner_state);
}
// 3879 "parser.c"
        break;
      case 147:
// 1508 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_let_assignment("object-property", yymsp[-1].minor.yy115, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, yymsp[0].minor.yy115, status->scanner_state);
  yy_destructor(82,&yymsp[-3].minor);
}
// 3887 "parser.c"
        break;
      case 148:
// 1513 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_let_assignment("variable-dynamic-object-property", yymsp[-1].minor.yy115, yymsp[-6].minor.yy0, yymsp[-3].minor.yy0, NULL, yymsp[0].minor.yy115, status->scanner_state);
  yy_destructor(82,&yymsp[-5].minor);
  yy_destructor(42,&yymsp[-4].minor);
  yy_destructor(43,&yymsp[-2].minor);
}
// 3897 "parser.c"
        break;
      case 149:
// 1518 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_let_assignment("string-dynamic-object-property", yymsp[-1].minor.yy115, yymsp[-6].minor.yy0, yymsp[-3].minor.yy0, NULL, yymsp[0].minor.yy115, status->scanner_state);
  yy_destructor(82,&yymsp[-5].minor);
  yy_destructor(42,&yymsp[-4].minor);
  yy_destructor(43,&yymsp[-2].minor);
}
// 3907 "parser.c"
        break;
      case 150:
// 1523 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_let_assignment("object-property-append", yymsp[-1].minor.yy115, yymsp[-6].minor.yy0, yymsp[-4].minor.yy0, NULL, yymsp[0].minor.yy115, status->scanner_state);
  yy_destructor(82,&yymsp[-5].minor);
  yy_destructor(84,&yymsp[-3].minor);
  yy_destructor(85,&yymsp[-2].minor);
}
// 3917 "parser.c"
        break;
      case 151:
// 1528 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_let_assignment("object-property-array-index", yymsp[-1].minor.yy115, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, yymsp[-3].minor.yy115, yymsp[0].minor.yy115, status->scanner_state);
  yy_destructor(82,&yymsp[-6].minor);
  yy_destructor(84,&yymsp[-4].minor);
  yy_destructor(85,&yymsp[-2].minor);
}
// 3927 "parser.c"
        break;
      case 152:
// 1533 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_let_assignment("static-property", yymsp[-1].minor.yy115, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, yymsp[0].minor.yy115, status->scanner_state);
  yy_destructor(86,&yymsp[-3].minor);
}
// 3935 "parser.c"
        break;
      case 153:
// 1538 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_let_assignment("static-property-append", yymsp[-1].minor.yy115, yymsp[-6].minor.yy0, yymsp[-4].minor.yy0, NULL, yymsp[0].minor.yy115, status->scanner_state);
  yy_destructor(86,&yymsp[-5].minor);
  yy_destructor(84,&yymsp[-3].minor);
  yy_destructor(85,&yymsp[-2].minor);
}
// 3945 "parser.c"
        break;
      case 154:
// 1543 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_let_assignment("static-property-array-index", yymsp[-1].minor.yy115, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, yymsp[-3].minor.yy115, yymsp[0].minor.yy115, status->scanner_state);
  yy_destructor(86,&yymsp[-6].minor);
  yy_destructor(84,&yymsp[-4].minor);
  yy_destructor(85,&yymsp[-2].minor);
}
// 3955 "parser.c"
        break;
      case 155:
// 1548 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_let_assignment("variable-append", yymsp[-1].minor.yy115, yymsp[-4].minor.yy0, NULL, NULL, yymsp[0].minor.yy115, status->scanner_state);
  yy_destructor(84,&yymsp[-3].minor);
  yy_destructor(85,&yymsp[-2].minor);
}
// 3964 "parser.c"
        break;
      case 156:
// 1553 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_let_assignment("array-index", yymsp[-1].minor.yy115, yymsp[-5].minor.yy0, NULL, yymsp[-3].minor.yy115, yymsp[0].minor.yy115, status->scanner_state);
  yy_destructor(84,&yymsp[-4].minor);
  yy_destructor(85,&yymsp[-2].minor);
}
// 3973 "parser.c"
        break;
      case 157:
// 1558 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_let_assignment("incr", NULL, yymsp[-3].minor.yy0, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(82,&yymsp[-2].minor);
  yy_destructor(87,&yymsp[0].minor);
}
// 3982 "parser.c"
        break;
      case 158:
// 1563 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_let_assignment("decr", NULL, yymsp[-3].minor.yy0, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(82,&yymsp[-2].minor);
  yy_destructor(88,&yymsp[0].minor);
}
// 3991 "parser.c"
        break;
      case 159:
// 1568 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_let_assignment("incr", NULL, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(87,&yymsp[0].minor);
}
// 3999 "parser.c"
        break;
      case 160:
// 1573 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_let_assignment("decr", NULL, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(88,&yymsp[0].minor);
}
// 4007 "parser.c"
        break;
      case 162:
// 1581 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_echo_statement(yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(89,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4016 "parser.c"
        break;
      case 165:
// 1593 "parser.lemon"
{
	yygotominor.yy115 = yymsp[0].minor.yy115;;
}
// 4023 "parser.c"
        break;
      case 166:
// 1598 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_mcall_statement(yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(35,&yymsp[0].minor);
}
// 4031 "parser.c"
        break;
      case 167:
// 1603 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_fcall_statement(yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(35,&yymsp[0].minor);
}
// 4039 "parser.c"
        break;
      case 168:
// 1608 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_scall_statement(yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(35,&yymsp[0].minor);
}
// 4047 "parser.c"
        break;
      case 169:
// 1613 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_return_statement(yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(90,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4056 "parser.c"
        break;
      case 170:
// 1618 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_return_statement(NULL, status->scanner_state);
  yy_destructor(90,&yymsp[-1].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4065 "parser.c"
        break;
      case 171:
// 1623 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(91,&yymsp[-4].minor);
  yy_destructor(82,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4079 "parser.c"
        break;
      case 172:
// 1632 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state),
		yymsp[-2].minor.yy115,
		status->scanner_state
	);
  yy_destructor(91,&yymsp[-5].minor);
  yy_destructor(84,&yymsp[-3].minor);
  yy_destructor(85,&yymsp[-1].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4094 "parser.c"
        break;
      case 173:
// 1641 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_throw_exception(yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(92,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4103 "parser.c"
        break;
      case 174:
// 1645 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_declare_statement(XX_T_TYPE_INTEGER, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(51,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4112 "parser.c"
        break;
      case 175:
// 1649 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_declare_statement(XX_T_TYPE_UINTEGER, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(52,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4121 "parser.c"
        break;
      case 176:
// 1653 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_declare_statement(XX_T_TYPE_CHAR, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(55,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4130 "parser.c"
        break;
      case 177:
// 1657 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_declare_statement(XX_T_TYPE_UCHAR, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(56,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4139 "parser.c"
        break;
      case 178:
// 1661 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_declare_statement(XX_T_TYPE_LONG, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(53,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4148 "parser.c"
        break;
      case 179:
// 1665 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_declare_statement(XX_T_TYPE_ULONG, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4157 "parser.c"
        break;
      case 180:
// 1669 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_declare_statement(XX_T_TYPE_DOUBLE, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(57,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4166 "parser.c"
        break;
      case 181:
// 1673 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_declare_statement(XX_T_TYPE_STRING, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(59,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4175 "parser.c"
        break;
      case 182:
// 1677 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_declare_statement(XX_T_TYPE_BOOL, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(58,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4184 "parser.c"
        break;
      case 183:
// 1681 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_declare_statement(XX_T_TYPE_VAR, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(61,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4193 "parser.c"
        break;
      case 186:
// 1693 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_declare_variable(yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 4200 "parser.c"
        break;
      case 187:
// 1697 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_declare_variable(yymsp[-2].minor.yy0, yymsp[0].minor.yy115, status->scanner_state);
  yy_destructor(45,&yymsp[-1].minor);
}
// 4208 "parser.c"
        break;
      case 189:
// 1705 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("not", yymsp[0].minor.yy115, NULL, NULL, status->scanner_state);
  yy_destructor(31,&yymsp[-1].minor);
}
// 4216 "parser.c"
        break;
      case 190:
// 1709 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("isset", yymsp[0].minor.yy115, NULL, NULL, status->scanner_state);
  yy_destructor(27,&yymsp[-1].minor);
}
// 4224 "parser.c"
        break;
      case 191:
// 1713 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("require", yymsp[0].minor.yy115, NULL, NULL, status->scanner_state);
  yy_destructor(7,&yymsp[-1].minor);
}
// 4232 "parser.c"
        break;
      case 192:
// 1717 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("clone", yymsp[0].minor.yy115, NULL, NULL, status->scanner_state);
  yy_destructor(8,&yymsp[-1].minor);
}
// 4240 "parser.c"
        break;
      case 193:
// 1721 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("empty", yymsp[0].minor.yy115, NULL, NULL, status->scanner_state);
  yy_destructor(9,&yymsp[-1].minor);
}
// 4248 "parser.c"
        break;
      case 194:
// 1725 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("equals", yymsp[-2].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(13,&yymsp[-1].minor);
}
// 4256 "parser.c"
        break;
      case 195:
// 1729 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("not-equals", yymsp[-2].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(20,&yymsp[-1].minor);
}
// 4264 "parser.c"
        break;
      case 196:
// 1733 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("identical", yymsp[-2].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(14,&yymsp[-1].minor);
}
// 4272 "parser.c"
        break;
      case 197:
// 1737 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("not-identical", yymsp[-2].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(19,&yymsp[-1].minor);
}
// 4280 "parser.c"
        break;
      case 198:
// 1741 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("less", yymsp[-2].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(15,&yymsp[-1].minor);
}
// 4288 "parser.c"
        break;
      case 199:
// 1745 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("greater", yymsp[-2].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(16,&yymsp[-1].minor);
}
// 4296 "parser.c"
        break;
      case 200:
// 1749 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("less-equal", yymsp[-2].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(17,&yymsp[-1].minor);
}
// 4304 "parser.c"
        break;
      case 201:
// 1753 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("greater-equal", yymsp[-2].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(18,&yymsp[-1].minor);
}
// 4312 "parser.c"
        break;
      case 202:
// 1757 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("list", yymsp[-1].minor.yy115, NULL, NULL, status->scanner_state);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 4321 "parser.c"
        break;
      case 203:
// 1761 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("cast", yymsp[-2].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(49,&yymsp[-3].minor);
  yy_destructor(32,&yymsp[-1].minor);
}
// 4330 "parser.c"
        break;
      case 204:
// 1765 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("type-hint", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(15,&yymsp[-3].minor);
  yy_destructor(16,&yymsp[-1].minor);
}
// 4339 "parser.c"
        break;
      case 205:
      case 267:
// 1769 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("property-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(82,&yymsp[-1].minor);
}
// 4348 "parser.c"
        break;
      case 206:
// 1773 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("static-property-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(86,&yymsp[-1].minor);
}
// 4356 "parser.c"
        break;
      case 207:
      case 265:
// 1777 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("static-constant-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(86,&yymsp[-1].minor);
}
// 4365 "parser.c"
        break;
      case 208:
      case 266:
// 1782 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), yymsp[-1].minor.yy115, NULL, status->scanner_state);
  yy_destructor(84,&yymsp[-2].minor);
  yy_destructor(85,&yymsp[0].minor);
}
// 4375 "parser.c"
        break;
      case 209:
// 1787 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("add", yymsp[-2].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(21,&yymsp[-1].minor);
}
// 4383 "parser.c"
        break;
      case 210:
// 1792 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("sub", yymsp[-2].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(22,&yymsp[-1].minor);
}
// 4391 "parser.c"
        break;
      case 211:
// 1797 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("mul", yymsp[-2].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(24,&yymsp[-1].minor);
}
// 4399 "parser.c"
        break;
      case 212:
// 1802 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("div", yymsp[-2].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(25,&yymsp[-1].minor);
}
// 4407 "parser.c"
        break;
      case 213:
// 1807 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("mod", yymsp[-2].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(26,&yymsp[-1].minor);
}
// 4415 "parser.c"
        break;
      case 214:
// 1812 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("concat", yymsp[-2].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(23,&yymsp[-1].minor);
}
// 4423 "parser.c"
        break;
      case 215:
// 1817 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("and", yymsp[-2].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(11,&yymsp[-1].minor);
}
// 4431 "parser.c"
        break;
      case 216:
// 1822 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("or", yymsp[-2].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(12,&yymsp[-1].minor);
}
// 4439 "parser.c"
        break;
      case 217:
// 1827 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("instanceof", yymsp[-2].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(10,&yymsp[-1].minor);
}
// 4447 "parser.c"
        break;
      case 218:
// 1832 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("fetch", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(28,&yymsp[-3].minor);
  yy_destructor(6,&yymsp[-1].minor);
}
// 4456 "parser.c"
        break;
      case 219:
// 1837 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("typeof", yymsp[0].minor.yy115, NULL, NULL, status->scanner_state);
  yy_destructor(29,&yymsp[-1].minor);
}
// 4464 "parser.c"
        break;
      case 220:
// 1842 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_literal(XX_T_CONSTANT, yymsp[0].minor.yy0, status->scanner_state);
}
// 4471 "parser.c"
        break;
      case 222:
      case 256:
      case 258:
// 1852 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_literal(XX_T_INTEGER, yymsp[0].minor.yy0, status->scanner_state);
}
// 4480 "parser.c"
        break;
      case 223:
      case 255:
      case 260:
// 1857 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_literal(XX_T_STRING, yymsp[0].minor.yy0, status->scanner_state);
}
// 4489 "parser.c"
        break;
      case 224:
      case 259:
// 1862 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_literal(XX_T_CHAR, yymsp[0].minor.yy0, status->scanner_state);
}
// 4497 "parser.c"
        break;
      case 225:
      case 261:
// 1867 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_literal(XX_T_DOUBLE, yymsp[0].minor.yy0, status->scanner_state);
}
// 4505 "parser.c"
        break;
      case 226:
      case 262:
// 1872 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_literal(XX_T_NULL, NULL, status->scanner_state);
  yy_destructor(96,&yymsp[0].minor);
}
// 4514 "parser.c"
        break;
      case 227:
      case 264:
// 1877 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_literal(XX_T_TRUE, NULL, status->scanner_state);
  yy_destructor(97,&yymsp[0].minor);
}
// 4523 "parser.c"
        break;
      case 228:
      case 263:
// 1882 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_literal(XX_T_FALSE, NULL, status->scanner_state);
  yy_destructor(98,&yymsp[0].minor);
}
// 4532 "parser.c"
        break;
      case 229:
// 1887 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("empty-array", NULL, NULL, NULL, status->scanner_state);
  yy_destructor(84,&yymsp[-1].minor);
  yy_destructor(85,&yymsp[0].minor);
}
// 4541 "parser.c"
        break;
      case 230:
// 1892 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("array", yymsp[-1].minor.yy115, NULL, NULL, status->scanner_state);
  yy_destructor(84,&yymsp[-2].minor);
  yy_destructor(85,&yymsp[0].minor);
}
// 4550 "parser.c"
        break;
      case 231:
// 1897 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_new_instance(yymsp[0].minor.yy0, NULL, status->scanner_state);
  yy_destructor(30,&yymsp[-1].minor);
}
// 4558 "parser.c"
        break;
      case 232:
// 1902 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_new_instance(yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(30,&yymsp[-3].minor);
  yy_destructor(49,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 4568 "parser.c"
        break;
      case 233:
// 1907 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_new_instance(yymsp[-3].minor.yy0, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(30,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 4578 "parser.c"
        break;
      case 234:
// 1912 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_fcall(yymsp[-3].minor.yy0, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 4587 "parser.c"
        break;
      case 235:
// 1917 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_fcall(yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(49,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 4596 "parser.c"
        break;
      case 236:
// 1922 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_scall(yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(86,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 4606 "parser.c"
        break;
      case 237:
// 1927 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_scall(yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(86,&yymsp[-3].minor);
  yy_destructor(49,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 4616 "parser.c"
        break;
      case 238:
// 1932 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_mcall(1, yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(82,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 4626 "parser.c"
        break;
      case 239:
// 1937 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_mcall(1, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(82,&yymsp[-3].minor);
  yy_destructor(49,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 4636 "parser.c"
        break;
      case 240:
// 1942 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_mcall(2, yymsp[-7].minor.yy0, yymsp[-4].minor.yy0, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(82,&yymsp[-6].minor);
  yy_destructor(42,&yymsp[-5].minor);
  yy_destructor(43,&yymsp[-3].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 4648 "parser.c"
        break;
      case 241:
// 1947 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_mcall(2, yymsp[-6].minor.yy0, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(82,&yymsp[-5].minor);
  yy_destructor(42,&yymsp[-4].minor);
  yy_destructor(43,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 4660 "parser.c"
        break;
      case 242:
// 1952 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_mcall(3, yymsp[-7].minor.yy0, yymsp[-4].minor.yy0, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(82,&yymsp[-6].minor);
  yy_destructor(42,&yymsp[-5].minor);
  yy_destructor(43,&yymsp[-3].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 4672 "parser.c"
        break;
      case 243:
// 1957 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_mcall(3, yymsp[-6].minor.yy0, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(82,&yymsp[-5].minor);
  yy_destructor(42,&yymsp[-4].minor);
  yy_destructor(43,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 4684 "parser.c"
        break;
      case 252:
// 1996 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_array_item(yymsp[-2].minor.yy115, yymsp[0].minor.yy115, status->scanner_state);
  yy_destructor(68,&yymsp[-1].minor);
}
// 4692 "parser.c"
        break;
      case 253:
// 2000 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_array_item(NULL, yymsp[0].minor.yy115, status->scanner_state);
}
// 4699 "parser.c"
        break;
      case 269:
// 2065 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_comment(yymsp[0].minor.yy0, status->scanner_state);
}
// 4706 "parser.c"
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
// 866 "parser.lemon"


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

// 4782 "parser.c"
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
