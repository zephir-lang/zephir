/** The author disclaims copyright to this source code.
*/
/* First off, code is include which follows the "include" declaration
** in the input file. */
#include <stdio.h>
// 46 "parser.lemon"


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

static json_object *xx_ret_interface(xx_parser_token *T, json_object *interface_definition, xx_parser_token *E, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("interface"));
	json_object_object_add(ret, "name", json_object_new_string(T->token));

	if (E) {
		json_object_object_add(ret, "extends", json_object_new_string(E->token));
	}

	if (interface_definition) {
		json_object_object_add(ret, "definition", interface_definition);
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

static json_object *xx_ret_mcall(xx_parser_token *O, xx_parser_token *M, json_object *parameters, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("mcall"));
	json_object_object_add(ret, "variable", json_object_new_string(O->token));
	json_object_object_add(ret, "name", json_object_new_string(M->token));

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


// 824 "parser.c"
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
#define YYNOCODE 169
#define YYACTIONTYPE unsigned short int
#define xx_TOKENTYPE xx_parser_token*
typedef union {
  xx_TOKENTYPE yy0;
  json_object* yy175;
  int yy337;
} YYMINORTYPE;
#define YYSTACKDEPTH 100
#define xx_ARG_SDECL xx_parser_status *status;
#define xx_ARG_PDECL ,xx_parser_status *status
#define xx_ARG_FETCH xx_parser_status *status = yypParser->status
#define xx_ARG_STORE yypParser->status = status
#define YYNSTATE 535
#define YYNRULE 259
#define YYERRORSYMBOL 97
#define YYERRSYMDT yy337
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
 /*     0 */   175,  170,  179,  183,  185,  187,  189,  181,  177,  191,
 /*    10 */   193,  201,  195,  197,  199,  146,  134,  171,  220,  224,
 /*    20 */   218,  240,  168,  408,  385,  213,  795,    1,    2,  534,
 /*    30 */     4,    5,    6,    7,  195,  197,  199,  295,  226,  148,
 /*    40 */   207,  443,   54,   55,   56,   57,   58,   59,   60,   61,
 /*    50 */    62,   63,   64,  203,  205,  170,  179,  183,  185,  187,
 /*    60 */   189,  181,  177,  191,  193,  201,  195,  197,  199,  296,
 /*    70 */   297,  298,  299,  300,  234,  308,    9,  100,  317,  318,
 /*    80 */    99,  413,  227,  228,   85,  229,  230,  231,  232,  233,
 /*    90 */   288,  418,   67,   68,   70,   69,   71,  535,  377,  386,
 /*   100 */   395,  398,  389,  392,  401,  407,  404,  365,  410,  128,
 /*   110 */   130,  132,   12,  141,  145,  446,   19,  152,  156,  161,
 /*   120 */   282,   22,  501,  289,  413,   86,   31,  692,   85,   73,
 /*   130 */    74,  144,  502,  447,  418,  342,  355,  359,   10,   72,
 /*   140 */   374,  377,  386,  395,  398,  389,  392,  401,  407,  404,
 /*   150 */   455,  410,  128,  130,  132,  148,  141,  443,  361,  364,
 /*   160 */   152,  156,  161,  282,  368,  371,  289,  413,   36,   38,
 /*   170 */   425,   37,   39,   40,   42,   41,  164,  418,  342,  355,
 /*   180 */   359,  295,   15,  374,  377,  386,  395,  398,  389,  392,
 /*   190 */   401,  407,  404,   13,  410,  128,  130,  132,  380,  141,
 /*   200 */    32,   43,   47,  152,  156,  161,  282,   17,  259,  289,
 /*   210 */   413,   76,  268,  296,  297,  298,  299,  300,   85,  106,
 /*   220 */   418,  342,  355,  359,  380,  409,  374,  377,  386,  395,
 /*   230 */   398,  389,  392,  401,  407,  404,  284,  410,  128,  130,
 /*   240 */   132,  416,  141,   16,  414,  531,  152,  156,  161,  282,
 /*   250 */    93,  379,  289,  413,  191,  193,  201,  195,  197,  199,
 /*   260 */   378,  385,  457,  418,  342,  355,  359,  387,  385,  374,
 /*   270 */   377,  386,  395,  398,  389,  392,  401,  407,  404,   66,
 /*   280 */   410,  128,  130,  132,  380,  141,  411,  385,  267,  152,
 /*   290 */   156,  161,  282,  347,   24,  289,  413,  461,  246,  343,
 /*   300 */   348,  247,  249,  248,   85,  108,  418,  342,  355,  359,
 /*   310 */   380,  388,  374,  377,  386,  395,  398,  389,  392,  401,
 /*   320 */   407,  404,  261,  410,  128,  130,  132,  380,  141,  390,
 /*   330 */   385,  516,  152,  156,  161,  282,  303,  391,  289,  413,
 /*   340 */   330,  393,  385,  380,  247,  249,  248,   25,  475,  418,
 /*   350 */   342,  355,  359,  259,  394,  374,  377,  386,  395,  398,
 /*   360 */   389,  392,  401,  407,  404,   93,  410,  128,  130,  132,
 /*   370 */   397,  141,  780,  396,  385,  152,  156,  161,  282,   27,
 /*   380 */    82,  289,  413,   65,   43,   47,  214,  256,   30,  237,
 /*   390 */   380,  473,  418,  342,  355,  359,  380,   35,  374,  377,
 /*   400 */   386,  395,  398,  389,  392,  401,  407,  404,   14,  410,
 /*   410 */   128,  130,  132,   17,  141,  399,  385,  400,  152,  156,
 /*   420 */   161,  282,  316,  403,  289,  413,  493,  335,  402,  385,
 /*   430 */   247,  249,  248,   85,  436,  418,  342,  355,  359,  290,
 /*   440 */   341,  374,  377,  386,  395,  398,  389,  392,  401,  407,
 /*   450 */   404,   34,  410,  128,  130,  132,  495,  141,  238,  246,
 /*   460 */    44,  152,  156,  161,  282,  303,  496,  289,  413,  334,
 /*   470 */   246,  255,  292,  247,  249,  248,  380,  442,  418,  342,
 /*   480 */   355,  359,  345,  276,  374,  377,  386,  395,  398,  389,
 /*   490 */   392,  401,  407,  404,  253,  410,  128,  130,  132,  291,
 /*   500 */   141,  246,   31,  412,  152,  156,  161,  282,  303,  344,
 /*   510 */   289,  413,  315,  405,  385,   45,  247,  249,  248,  380,
 /*   520 */   459,  418,  342,  355,  359,  245,   29,  374,  377,  386,
 /*   530 */   395,  398,  389,  392,  401,  407,  404,   46,  410,  128,
 /*   540 */   130,  132,  514,  141,  519,   31,  406,  152,  156,  161,
 /*   550 */   282,  303,  524,  289,  413,  302,   31,   93,   48,  247,
 /*   560 */   249,  248,   49,  452,  418,  342,  355,  359,   50,  471,
 /*   570 */   374,  377,  386,  395,  398,  389,  392,  401,  407,  404,
 /*   580 */    83,  410,  128,  130,  132,   52,  141,   53,  613,   78,
 /*   590 */   152,  156,  161,  282,  303,   79,  289,  413,  338,   81,
 /*   600 */    84,   88,  247,  249,  248,   89,  424,  418,  342,  355,
 /*   610 */   359,   91,   93,  374,  377,  386,  395,  398,  389,  392,
 /*   620 */   401,  407,  404,   92,  410,  128,  130,  132,   96,  141,
 /*   630 */   102,  103,  105,  152,  156,  161,  282,  303,  129,  289,
 /*   640 */   413,  311,  131,  137,  138,  247,  249,  248,  147,  453,
 /*   650 */   418,  342,  355,  359,  143,  149,  374,  377,  386,  395,
 /*   660 */   398,  389,  392,  401,  407,  404,  150,  410,  128,  130,
 /*   670 */   132,  153,  141,  158,  162,  165,  152,  156,  161,  282,
 /*   680 */   316,  167,  289,  413,  173,  312,  172,  211,  247,  249,
 /*   690 */   248,  174,  160,  418,  342,  355,  359,  215,  221,  374,
 /*   700 */   377,  386,  395,  398,  389,  392,  401,  407,  404,  216,
 /*   710 */   410,  128,  130,  132,  222,  141,  223,  239,  241,  152,
 /*   720 */   156,  161,  282,  347,   28,  289,  413,   65,   43,   47,
 /*   730 */   346,  247,  249,  248,  242,  140,  418,  342,  355,  359,
 /*   740 */   782,  781,  374,  377,  386,  395,  398,  389,  392,  401,
 /*   750 */   407,  404,  283,  410,  128,  130,  132,  271,  141,  294,
 /*   760 */   305,  293,  152,  156,  161,  282,  303,  301,  289,  413,
 /*   770 */   307,  306,  310,  314,  247,  249,  248,  313,  155,  418,
 /*   780 */   342,  355,  359,  320,  328,  374,  377,  386,  395,  398,
 /*   790 */   389,  392,  401,  407,  404,  321,  410,  128,  130,  132,
 /*   800 */   325,  141,  350,  336,  329,  152,  156,  161,  282,  316,
 /*   810 */   333,  289,  413,  360,  327,  337,  352,  247,  249,  248,
 /*   820 */   354,  441,  418,  342,  355,  359,  362,  366,  374,  377,
 /*   830 */   386,  395,  398,  389,  392,  401,  407,  404,  363,  410,
 /*   840 */   128,  130,  132,  367,  141,  369,  370,  372,  152,  156,
 /*   850 */   161,  282,  373,  217,  289,  413,    3,    4,    5,    6,
 /*   860 */     7,  247,  249,  248,  136,  418,  342,  355,  359,  250,
 /*   870 */   382,  374,  377,  386,  395,  398,  389,  392,  401,  407,
 /*   880 */   404,  384,  410,  128,  130,  132,  381,  141,  383,  415,
 /*   890 */   417,  152,  156,  161,  282,  303,  426,  289,  413,  322,
 /*   900 */   427,  438,  440,  247,  249,  248,  444,  431,  418,  342,
 /*   910 */   355,  359,  449,  450,  374,  377,  386,  395,  398,  389,
 /*   920 */   392,  401,  407,  404,  456,  410,  128,  130,  132,  463,
 /*   930 */   141,  466,  464,  472,  152,  156,  161,  282,  303,  482,
 /*   940 */   289,  413,  326,  483,  484,  485,  247,  249,  248,  488,
 /*   950 */   448,  418,  342,  355,  359,  487,  489,  374,  377,  386,
 /*   960 */   395,  398,  389,  392,  401,  407,  404,  490,  410,  128,
 /*   970 */   130,  132,  497,  141,  498,  504,  503,  152,  156,  161,
 /*   980 */   282,  196,  454,  289,  413,   65,   43,   47,  518,  247,
 /*   990 */   249,  248,  517,  467,  418,  342,  355,  359,  521,  515,
 /*  1000 */   374,  377,  386,  395,  398,  389,  392,  401,  407,  404,
 /*  1010 */   522,  410,  128,  130,  132,  523,  141,  525,  526,  528,
 /*  1020 */   152,  156,  161,  282,  421,  470,  289,  413,   65,   43,
 /*  1030 */    47,  529,  247,  249,  248,  530,  437,  418,  342,  355,
 /*  1040 */   359,  532,  533,  374,  377,  386,  395,  398,  389,  392,
 /*  1050 */   401,  407,  404,  429,  410,  128,  130,  132,  429,  141,
 /*  1060 */   429,  429,  429,  152,  156,  161,  282,  278,  429,  289,
 /*  1070 */   413,  429,  429,  429,  429,  247,  249,  248,  429,  469,
 /*  1080 */   418,  342,  355,  359,  259,  429,  374,  377,  386,  395,
 /*  1090 */   398,  389,  392,  401,  407,  404,  429,  410,  128,  130,
 /*  1100 */   132,  429,  141,  429,  429,  429,  152,  156,  161,  282,
 /*  1110 */   429,  429,  289,  413,  429,  429,  429,  214,  256,   51,
 /*  1120 */   237,  429,  429,  418,  342,  355,  359,  429,  429,  374,
 /*  1130 */   377,  386,  395,  398,  389,  392,  401,  407,  404,   33,
 /*  1140 */   410,  128,  130,  132,  429,  141,  429,  429,  429,  152,
 /*  1150 */   156,  161,  282,  127,  429,  289,   54,   55,   56,   57,
 /*  1160 */    58,   59,   60,   61,   62,   63,   64,  342,  355,  359,
 /*  1170 */   435,  429,  374,  429,  429,  419,  110,  111,  112,  113,
 /*  1180 */   114,  115,  116,  117,  118,  119,  120,  121,  122,  123,
 /*  1190 */   124,  125,  126,  127,  429,  429,  429,  476,   98,  491,
 /*  1200 */   499,  429,  505,  479,  349,  351,  353,   85,  429,  429,
 /*  1210 */   445,  429,  429,  429,  429,  419,  110,  111,  112,  113,
 /*  1220 */   114,  115,  116,  117,  118,  119,  120,  121,  122,  123,
 /*  1230 */   124,  125,  126,  429,  477,  127,  100,  429,  478,  479,
 /*  1240 */   429,  429,  429,   85,  349,  351,  353,   67,   68,   70,
 /*  1250 */    69,   71,  159,  429,  429,  429,  429,  419,  110,  111,
 /*  1260 */   112,  113,  114,  115,  116,  117,  118,  119,  120,  121,
 /*  1270 */   122,  123,  124,  125,  126,  127,   22,  429,  429,  429,
 /*  1280 */    21,  429,  429,   85,   73,   74,  349,  351,  353,  429,
 /*  1290 */   481,  429,  430,  429,   72,  429,  429,  419,  110,  111,
 /*  1300 */   112,  113,  114,  115,  116,  117,  118,  119,  120,  121,
 /*  1310 */   122,  123,  124,  125,  126,   95,  127,  429,   97,  506,
 /*  1320 */   512,  511,  499,  429,  505,  479,  349,  351,  353,   85,
 /*  1330 */    90,  429,  513,  468,  429,   93,  429,  429,  419,  110,
 /*  1340 */   111,  112,  113,  114,  115,  116,  117,  118,  119,  120,
 /*  1350 */   121,  122,  123,  124,  125,  126,  217,  429,  127,  429,
 /*  1360 */   429,  429,  429,  429,  247,  249,  248,  349,  351,  353,
 /*  1370 */   295,  260,  251,  429,  429,  287,  429,  429,  429,  429,
 /*  1380 */   419,  110,  111,  112,  113,  114,  115,  116,  117,  118,
 /*  1390 */   119,  120,  121,  122,  123,  124,  125,  126,  127,  429,
 /*  1400 */   429,  429,  296,  297,  298,  299,  300,  429,  323,  349,
 /*  1410 */   351,  353,  429,  429,  429,  139,  429,  429,  429,  429,
 /*  1420 */   419,  110,  111,  112,  113,  114,  115,  116,  117,  118,
 /*  1430 */   119,  120,  121,  122,  123,  124,  125,  126,  429,  127,
 /*  1440 */   429,  507,  429,  510,  511,  499,  429,  478,  479,  349,
 /*  1450 */   351,  353,   85,  429,  429,  429,  451,  429,  429,  429,
 /*  1460 */   429,  419,  110,  111,  112,  113,  114,  115,  116,  117,
 /*  1470 */   118,  119,  120,  121,  122,  123,  124,  125,  126,  429,
 /*  1480 */   429,  127,  429,  429,  133,  429,  429,  281,  429,  429,
 /*  1490 */   349,  351,  353,  429,  429,  247,  249,  248,  135,  429,
 /*  1500 */   429,  429,  429,  419,  110,  111,  112,  113,  114,  115,
 /*  1510 */   116,  117,  118,  119,  120,  121,  122,  123,  124,  125,
 /*  1520 */   126,  127,  429,  429,  429,  429,  429,  429,  166,  429,
 /*  1530 */   429,  281,  349,  351,  353,  429,  429,  429,  151,  247,
 /*  1540 */   249,  248,  429,  419,  110,  111,  112,  113,  114,  115,
 /*  1550 */   116,  117,  118,  119,  120,  121,  122,  123,  124,  125,
 /*  1560 */   126,  429,  127,  429,  429,  429,  508,  491,  499,  429,
 /*  1570 */   429,  479,  349,  351,  353,   85,  429,  429,  429,  154,
 /*  1580 */   429,  429,  429,  429,  419,  110,  111,  112,  113,  114,
 /*  1590 */   115,  116,  117,  118,  119,  120,  121,  122,  123,  124,
 /*  1600 */   125,  126,  429,  429,  127,  429,  429,  439,  429,  429,
 /*  1610 */   281,  429,  429,  349,  351,  353,  429,  429,  247,  249,
 /*  1620 */   248,  458,  429,  429,  429,  429,  419,  110,  111,  112,
 /*  1630 */   113,  114,  115,  116,  117,  118,  119,  120,  121,  122,
 /*  1640 */   123,  124,  125,  126,  127,  429,  429,  429,  429,  429,
 /*  1650 */   429,  142,  429,  429,  281,  349,  351,  353,  429,  429,
 /*  1660 */   429,  163,  247,  249,  248,  429,  419,  110,  111,  112,
 /*  1670 */   113,  114,  115,  116,  117,  118,  119,  120,  121,  122,
 /*  1680 */   123,  124,  125,  126,  429,  127,  429,  157,  429,  429,
 /*  1690 */   281,  429,  429,  429,  429,  349,  351,  353,  247,  249,
 /*  1700 */   248,  429,  423,  429,  429,  429,  429,  419,  110,  111,
 /*  1710 */   112,  113,  114,  115,  116,  117,  118,  119,  120,  121,
 /*  1720 */   122,  123,  124,  125,  126,  429,  429,  127,  429,  429,
 /*  1730 */   429,  429,  429,  375,  429,  429,  349,  351,  353,  429,
 /*  1740 */   429,  247,  249,  248,  474,  429,  429,  429,  429,  419,
 /*  1750 */   110,  111,  112,  113,  114,  115,  116,  117,  118,  119,
 /*  1760 */   120,  121,  122,  123,  124,  125,  126,  127,  429,  429,
 /*  1770 */   429,  429,  429,  429,  429,  429,  429,  200,  349,  351,
 /*  1780 */   353,  429,  429,  429,  107,  247,  249,  248,  429,  419,
 /*  1790 */   110,  111,  112,  113,  114,  115,  116,  117,  118,  119,
 /*  1800 */   120,  121,  122,  123,  124,  125,  126,  429,  127,  429,
 /*  1810 */   429,  429,  429,  356,  429,  429,  202,  429,  349,  351,
 /*  1820 */   353,  247,  249,  248,  247,  249,  248,  429,  429,  429,
 /*  1830 */   109,  110,  111,  112,  113,  114,  115,  116,  117,  118,
 /*  1840 */   119,  120,  121,  122,  123,  124,  125,  126,  175,  429,
 /*  1850 */   429,  429,  429,  429,  429,  429,  429,  429,  429,  349,
 /*  1860 */   351,  353,  429,  429,  429,  171,  220,  224,  218,  240,
 /*  1870 */   168,  429,  429,  213,  175,  429,    8,  429,  429,   11,
 /*  1880 */   429,   87,  429,  520,  527,  429,  226,  418,  207,  429,
 /*  1890 */   429,  171,  220,  224,  218,  240,  168,  429,  429,  213,
 /*  1900 */   429,  235,  429,  429,  429,  429,  429,  175,  429,  247,
 /*  1910 */   249,  248,  226,  429,  207,  266,  429,  429,  274,  270,
 /*  1920 */   273,  210,  234,  324,  171,  220,  224,  218,  240,  168,
 /*  1930 */   227,  228,  213,  229,  230,  231,  232,  233,  429,   67,
 /*  1940 */    68,   70,   69,   71,  208,  226,  429,  207,  234,  332,
 /*  1950 */   429,  429,  247,  249,  248,  429,  227,  228,  429,  229,
 /*  1960 */   230,  231,  232,  233,   67,   68,   70,   69,   71,  429,
 /*  1970 */   429,  429,  429,  420,  429,  212,   73,   74,  429,  560,
 /*  1980 */   492,  234,  486,  247,  249,  248,   72,  429,  429,  227,
 /*  1990 */   228,  429,  229,  230,  231,  232,  233,  175,  429,  429,
 /*  2000 */   429,   73,   74,  429,   94,  492,  429,  486,  429,  429,
 /*  2010 */   429,   72,  429,  429,  171,  220,  224,  218,  240,  168,
 /*  2020 */   429,  429,  213,  358,  175,  429,  429,  429,  429,  429,
 /*  2030 */   280,  429,  429,  429,  429,  226,  429,  207,  247,  249,
 /*  2040 */   248,  171,  220,  224,  218,  240,  168,  262,  429,  213,
 /*  2050 */    51,  429,  429,  429,  429,  429,  429,  429,  429,  429,
 /*  2060 */   429,  429,  226,  429,  207,  429,  429,  192,   26,  429,
 /*  2070 */    33,  234,  429,  429,  429,  247,  249,  248,  429,  227,
 /*  2080 */   228,   51,  229,  230,  231,  232,  233,   54,   55,   56,
 /*  2090 */    57,   58,   59,   60,   61,   62,   63,   64,  234,   80,
 /*  2100 */   429,   33,  429,  429,  429,  429,  227,  228,  429,  229,
 /*  2110 */   230,  231,  232,  233,  175,  429,  429,  429,   54,   55,
 /*  2120 */    56,   57,   58,   59,   60,   61,   62,   63,   64,  429,
 /*  2130 */   429,  171,  220,  224,  218,  240,  168,  243,  429,  213,
 /*  2140 */   175,  190,  429,  429,  429,  429,  429,  429,  429,  247,
 /*  2150 */   249,  248,  226,  429,  207,  429,  429,  171,  220,  224,
 /*  2160 */   218,  240,  168,  254,  429,  213,  235,  429,  429,  429,
 /*  2170 */   429,  429,  429,  175,  247,  249,  248,  429,  226,  429,
 /*  2180 */   207,  429,  429,  269,  270,  273,  429,  429,  234,  429,
 /*  2190 */   171,  220,  224,  218,  240,  168,  227,  228,  236,  229,
 /*  2200 */   230,  231,  232,  233,  429,  429,  429,  429,  429,  429,
 /*  2210 */   429,  226,  429,  207,  234,  429,  429,  429,  429,  429,
 /*  2220 */   429,  429,  227,  228,  429,  229,  230,  231,  232,  233,
 /*  2230 */   429,  203,  205,  170,  179,  183,  185,  187,  189,  181,
 /*  2240 */   177,  191,  193,  201,  195,  197,  199,  234,  265,  429,
 /*  2250 */   429,  429,  429,  209,  429,  263,  264,  429,  229,  230,
 /*  2260 */   231,  232,  233,  175,  429,  429,  429,  429,  235,  429,
 /*  2270 */   429,  429,  429,  429,  429,  429,  247,  249,  248,  429,
 /*  2280 */   171,  220,  224,  218,  240,  168,  277,  272,  213,  175,
 /*  2290 */   194,  429,  429,  429,  429,  429,  429,  429,  247,  249,
 /*  2300 */   248,  226,  429,  207,  429,  429,  171,  220,  224,  218,
 /*  2310 */   240,  168,  429,  429,  213,  217,  429,  429,  429,  429,
 /*  2320 */   429,  429,  175,  247,  249,  248,  429,  226,  429,  207,
 /*  2330 */   275,  251,  429,  429,  429,  429,  429,  234,  429,  171,
 /*  2340 */   220,  224,  218,  240,  168,  227,  228,  213,  229,  230,
 /*  2350 */   231,  232,  233,  429,   67,   68,   70,   69,   71,  429,
 /*  2360 */   226,  429,  207,  234,  309,  429,  429,  429,  429,  429,
 /*  2370 */   429,  227,  228,  429,  229,  230,  231,  232,  233,  429,
 /*  2380 */   429,  295,  429,  429,  429,  429,  429,  429,  432,  429,
 /*  2390 */   285,   73,   74,  429,  564,  480,  234,  486,  247,  249,
 /*  2400 */   248,   72,  429,  429,  227,  228,  429,  229,  230,  231,
 /*  2410 */   232,  233,  175,  296,  297,  298,  299,  300,  304,  331,
 /*  2420 */   429,  319,  339,  340,   67,   68,   70,   69,   71,  171,
 /*  2430 */   220,  224,  218,  240,  168,  188,  429,  213,  175,  257,
 /*  2440 */   429,  429,  429,  247,  249,  248,  429,  247,  249,  248,
 /*  2450 */   226,  536,  207,  429,  429,  171,  220,  224,  218,  240,
 /*  2460 */   168,   73,   74,  236,  561,  492,  429,  486,  429,  429,
 /*  2470 */   429,   72,  429,  429,  429,  429,  226,  429,  207,  429,
 /*  2480 */   429,  429,    8,  429,  429,   11,  234,   87,  429,  520,
 /*  2490 */   527,  429,  429,  418,  227,  228,  169,  229,  230,  231,
 /*  2500 */   232,  233,  429,  429,  247,  249,  248,  186,  429,  429,
 /*  2510 */   429,  429,  234,  429,  429,  247,  249,  248,  429,  429,
 /*  2520 */   263,  264,  429,  229,  230,  231,  232,  233,  429,  203,
 /*  2530 */   205,  170,  179,  183,  185,  187,  189,  181,  177,  191,
 /*  2540 */   193,  201,  195,  197,  199,  203,  205,  170,  179,  183,
 /*  2550 */   185,  187,  189,  181,  177,  191,  193,  201,  195,  197,
 /*  2560 */   199,  434,  429,  206,  429,  429,  184,  429,  429,  429,
 /*  2570 */   357,  247,  249,  248,  247,  249,  248,  429,  429,  203,
 /*  2580 */   205,  170,  179,  183,  185,  187,  189,  181,  177,  191,
 /*  2590 */   193,  201,  195,  197,  199,  203,  205,  170,  179,  183,
 /*  2600 */   185,  187,  189,  181,  177,  191,  193,  201,  195,  197,
 /*  2610 */   199,  203,  205,  170,  179,  183,  185,  187,  189,  181,
 /*  2620 */   177,  191,  193,  201,  195,  197,  199,  286,  429,  429,
 /*  2630 */   429,  217,  429,  429,  429,  429,  429,  429,  429,  247,
 /*  2640 */   249,  248,  217,  422,  429,  429,  252,  251,  429,  429,
 /*  2650 */   247,  249,  248,  279,  429,  429,  429,  244,  251,  429,
 /*  2660 */   429,  429,  203,  205,  170,  179,  183,  185,  187,  189,
 /*  2670 */   181,  177,  191,  193,  201,  195,  197,  199,  203,  205,
 /*  2680 */   170,  179,  183,  185,  187,  189,  181,  177,  191,  193,
 /*  2690 */   201,  195,  197,  199,  203,  205,  170,  179,  183,  185,
 /*  2700 */   187,  189,  181,  177,  191,  193,  201,  195,  197,  199,
 /*  2710 */   429,  429,  182,  429,  429,  204,  429,  429,  429,  376,
 /*  2720 */   247,  249,  248,  247,  249,  248,  429,  429,  429,  180,
 /*  2730 */   429,  429,  225,  429,  429,  429,  258,  247,  249,  248,
 /*  2740 */   247,  249,  248,  429,  429,  203,  205,  170,  179,  183,
 /*  2750 */   185,  187,  189,  181,  177,  191,  193,  201,  195,  197,
 /*  2760 */   199,   51,  429,  429,  429,  219,  429,  429,  198,  429,
 /*  2770 */   429,  429,   51,  247,  249,  248,  247,  249,  248,  465,
 /*  2780 */   429,   33,  429,   67,   68,   70,   69,   71,  429,  429,
 /*  2790 */   104,  429,   33,  429,  429,  429,  429,  429,   54,   55,
 /*  2800 */    56,   57,   58,   59,   60,   61,   62,   63,   64,   54,
 /*  2810 */    55,   56,   57,   58,   59,   60,   61,   62,   63,   64,
 /*  2820 */    73,   74,  429,  565,  509,   67,   68,   70,   69,   71,
 /*  2830 */    72,  178,  429,  429,  429,  176,  429,  429,  433,  247,
 /*  2840 */   249,  248,  428,  247,  249,  248,  247,  249,  248,  429,
 /*  2850 */   247,  249,  248,  429,  429,  429,  429,  429,  429,  429,
 /*  2860 */   429,  429,   73,   74,  429,  567,  460,   67,   68,   70,
 /*  2870 */    69,   71,   72,  429,  429,  429,  429,  429,  429,  429,
 /*  2880 */   429,  429,  429,  429,  429,  429,  429,  429,  429,  429,
 /*  2890 */   429,  429,  429,  429,  429,  429,  429,  429,  429,  429,
 /*  2900 */   429,  429,  429,  429,   73,   74,  429,  568,  460,   67,
 /*  2910 */    68,   70,   69,   71,   72,  429,  429,  429,  429,  429,
 /*  2920 */   429,  429,  429,  429,  429,  429,  429,  429,  429,  429,
 /*  2930 */   429,  429,  429,  429,  429,  429,  429,  429,  429,  429,
 /*  2940 */   429,  429,  429,  429,  429,  429,   73,   74,  429,   18,
 /*  2950 */    75,   67,   68,   70,   69,   71,   72,  429,  429,  429,
 /*  2960 */   429,  429,  429,  429,  429,  429,  429,  429,  429,  429,
 /*  2970 */   429,  429,  429,  429,  429,  429,  429,  429,  429,  429,
 /*  2980 */   429,  429,  500,  429,  429,  429,  429,  429,   73,   74,
 /*  2990 */    67,   68,   70,   69,   71,  429,  101,  429,   72,  429,
 /*  3000 */   429,  429,  429,  429,  429,  429,  429,  429,  429,  429,
 /*  3010 */   429,  429,  429,  429,  429,  429,  429,  429,  429,  429,
 /*  3020 */   429,  494,  429,  429,  429,  429,  429,   73,   74,   67,
 /*  3030 */    68,   70,   69,   71,  429,  462,  429,   72,  429,  429,
 /*  3040 */   429,  429,  429,  429,  429,  429,  429,  429,  429,  429,
 /*  3050 */   429,  429,  429,  429,  429,  429,  429,  429,  429,  429,
 /*  3060 */   429,  429,  429,  429,  429,  429,   73,   74,  429,  562,
 /*  3070 */   460,   67,   68,   70,   69,   71,   72,  429,  429,  429,
 /*  3080 */   429,  429,  429,  429,  429,  429,  429,  429,  429,  429,
 /*  3090 */   429,  429,  429,  429,  429,  429,  429,  429,  429,  429,
 /*  3100 */   429,  429,  429,  429,  429,  429,  429,  429,   73,   74,
 /*  3110 */   429,   20,   75,   67,   68,   70,   69,   71,   72,  429,
 /*  3120 */   429,  429,  429,  429,  429,  429,  429,  429,  429,  429,
 /*  3130 */   429,  429,  429,  429,  429,  429,  429,  429,  429,  429,
 /*  3140 */   429,  429,  429,  429,  429,  429,  429,  429,  429,  429,
 /*  3150 */    73,   74,  429,  566,  460,   67,   68,   70,   69,   71,
 /*  3160 */    72,   67,   68,   70,   69,   71,   67,   68,   70,   69,
 /*  3170 */    71,  429,   67,   68,   70,   69,   71,   67,   68,   70,
 /*  3180 */    69,   71,  429,  429,  429,  429,  429,  429,  429,  429,
 /*  3190 */   429,  429,   73,   74,  429,  563,  460,  429,   73,   74,
 /*  3200 */   429,  429,   72,   73,   74,  429,  462,  429,   72,   73,
 /*  3210 */    74,   77,  429,   72,   73,   74,  429,   23,  429,   72,
 /*  3220 */   429,  429,  101,  429,   72,
};
static YYCODETYPE yy_lookahead[] = {
 /*     0 */     7,   10,   11,   12,   13,   14,   15,   16,   17,   18,
 /*    10 */    19,   20,   21,   22,   23,   41,   40,   24,   25,   26,
 /*    20 */    27,   28,   29,  159,  160,   32,   98,   99,  100,  101,
 /*    30 */   102,  103,  104,  105,   21,   22,   23,   43,   45,   65,
 /*    40 */    47,   67,   49,   50,   51,   52,   53,   54,   55,   56,
 /*    50 */    57,   58,   59,    8,    9,   10,   11,   12,   13,   14,
 /*    60 */    15,   16,   17,   18,   19,   20,   21,   22,   23,   75,
 /*    70 */    76,   77,   78,   79,   81,   81,   32,  116,   84,   85,
 /*    80 */   119,   32,   89,   90,  123,   92,   93,   94,   95,   96,
 /*    90 */    41,   42,    1,    2,    3,    4,    5,    0,   49,   50,
 /*   100 */    51,   52,   53,   54,   55,   56,   57,   32,   59,   60,
 /*   110 */    61,   62,   32,   64,  146,  147,  111,   68,   69,   70,
 /*   120 */    71,  116,   33,   74,   32,  120,    6,   82,  123,   38,
 /*   130 */    39,   41,   43,   41,   42,   86,   87,   88,   33,   48,
 /*   140 */    91,   49,   50,   51,   52,   53,   54,   55,   56,   57,
 /*   150 */    30,   59,   60,   61,   62,   65,   64,   67,   80,   81,
 /*   160 */    68,   69,   70,   71,   89,   90,   74,   32,   89,   90,
 /*   170 */     6,   92,   93,   94,   95,   96,   41,   42,   86,   87,
 /*   180 */    88,   43,   32,   91,   49,   50,   51,   52,   53,   54,
 /*   190 */    55,   56,   57,  106,   59,   60,   61,   62,    6,   64,
 /*   200 */   124,  125,  126,   68,   69,   70,   71,   40,   47,   74,
 /*   210 */    32,  116,    6,   75,   76,   77,   78,   79,  123,   41,
 /*   220 */    42,   86,   87,   88,    6,   33,   91,   49,   50,   51,
 /*   230 */    52,   53,   54,   55,   56,   57,   72,   59,   60,   61,
 /*   240 */    62,   80,   64,  106,   83,   35,   68,   69,   70,   71,
 /*   250 */    40,   33,   74,   32,   18,   19,   20,   21,   22,   23,
 /*   260 */   159,  160,   41,   42,   86,   87,   88,  159,  160,   91,
 /*   270 */    49,   50,   51,   52,   53,   54,   55,   56,   57,  123,
 /*   280 */    59,   60,   61,   62,    6,   64,  159,  160,   82,   68,
 /*   290 */    69,   70,   71,  148,   32,   74,   32,  116,    6,  154,
 /*   300 */   155,  156,  157,  158,  123,   41,   42,   86,   87,   88,
 /*   310 */     6,   33,   91,   49,   50,   51,   52,   53,   54,   55,
 /*   320 */    56,   57,   30,   59,   60,   61,   62,    6,   64,  159,
 /*   330 */   160,    6,   68,   69,   70,   71,  148,   33,   74,   32,
 /*   340 */   152,  159,  160,    6,  156,  157,  158,   47,   41,   42,
 /*   350 */    86,   87,   88,   47,   33,   91,   49,   50,   51,   52,
 /*   360 */    53,   54,   55,   56,   57,   40,   59,   60,   61,   62,
 /*   370 */    33,   64,   66,  159,  160,   68,   69,   70,   71,   33,
 /*   380 */   121,   74,   32,  124,  125,  126,   80,   81,   33,   83,
 /*   390 */     6,   41,   42,   86,   87,   88,    6,  117,   91,   49,
 /*   400 */    50,   51,   52,   53,   54,   55,   56,   57,   35,   59,
 /*   410 */    60,   61,   62,   40,   64,  159,  160,   33,   68,   69,
 /*   420 */    70,   71,  148,   33,   74,   32,  116,  153,  159,  160,
 /*   430 */   156,  157,  158,  123,   41,   42,   86,   87,   88,  149,
 /*   440 */   150,   91,   49,   50,   51,   52,   53,   54,   55,   56,
 /*   450 */    57,   43,   59,   60,   61,   62,   33,   64,   32,    6,
 /*   460 */    32,   68,   69,   70,   71,  148,   43,   74,   32,  152,
 /*   470 */     6,   45,    6,  156,  157,  158,    6,   41,   42,   86,
 /*   480 */    87,   88,    6,   30,   91,   49,   50,   51,   52,   53,
 /*   490 */    54,   55,   56,   57,   30,   59,   60,   61,   62,   33,
 /*   500 */    64,    6,    6,   33,   68,   69,   70,   71,  148,   33,
 /*   510 */    74,   32,  152,  159,  160,   43,  156,  157,  158,    6,
 /*   520 */    41,   42,   86,   87,   88,   30,   30,   91,   49,   50,
 /*   530 */    51,   52,   53,   54,   55,   56,   57,  117,   59,   60,
 /*   540 */    61,   62,  108,   64,  110,    6,   33,   68,   69,   70,
 /*   550 */    71,  148,   35,   74,   32,  152,    6,   40,   32,  156,
 /*   560 */   157,  158,   43,   41,   42,   86,   87,   88,  117,   30,
 /*   570 */    91,   49,   50,   51,   52,   53,   54,   55,   56,   57,
 /*   580 */    30,   59,   60,   61,   62,   32,   64,   13,   32,   32,
 /*   590 */    68,   69,   70,   71,  148,   47,   74,   32,  152,   33,
 /*   600 */    33,   32,  156,  157,  158,  107,   41,   42,   86,   87,
 /*   610 */    88,   32,   40,   91,   49,   50,   51,   52,   53,   54,
 /*   620 */    55,   56,   57,  107,   59,   60,   61,   62,   41,   64,
 /*   630 */    32,   47,   40,   68,   69,   70,   71,  148,   33,   74,
 /*   640 */    32,  152,   33,   63,   40,  156,  157,  158,  147,   41,
 /*   650 */    42,   86,   87,   88,   40,  117,   91,   49,   50,   51,
 /*   660 */    52,   53,   54,   55,   56,   57,   66,   59,   60,   61,
 /*   670 */    62,   40,   64,   40,   40,   69,   68,   69,   70,   71,
 /*   680 */   148,   33,   74,   32,   32,  153,  161,   30,  156,  157,
 /*   690 */   158,   81,   41,   42,   86,   87,   88,   32,   32,   91,
 /*   700 */    49,   50,   51,   52,   53,   54,   55,   56,   57,   47,
 /*   710 */    59,   60,   61,   62,    6,   64,  161,   47,   32,   68,
 /*   720 */    69,   70,   71,  148,  121,   74,   32,  124,  125,  126,
 /*   730 */   155,  156,  157,  158,   47,   41,   42,   86,   87,   88,
 /*   740 */    66,   66,   91,   49,   50,   51,   52,   53,   54,   55,
 /*   750 */    56,   57,   32,   59,   60,   61,   62,   66,   64,   32,
 /*   760 */    32,  150,   68,   69,   70,   71,  148,  151,   74,   32,
 /*   770 */   152,  151,  151,  151,  156,  157,  158,   82,   41,   42,
 /*   780 */    86,   87,   88,   32,   82,   91,   49,   50,   51,   52,
 /*   790 */    53,   54,   55,   56,   57,  151,   59,   60,   61,   62,
 /*   800 */   151,   64,   33,   82,  151,   68,   69,   70,   71,  148,
 /*   810 */   151,   74,   32,   32,  153,  151,   33,  156,  157,  158,
 /*   820 */    33,   41,   42,   86,   87,   88,   32,   82,   91,   49,
 /*   830 */    50,   51,   52,   53,   54,   55,   56,   57,   33,   59,
 /*   840 */    60,   61,   62,   33,   64,   82,   33,   82,   68,   69,
 /*   850 */    70,   71,   33,  148,   74,   32,  101,  102,  103,  104,
 /*   860 */   105,  156,  157,  158,   41,   42,   86,   87,   88,  164,
 /*   870 */    32,   91,   49,   50,   51,   52,   53,   54,   55,   56,
 /*   880 */    57,  117,   59,   60,   61,   62,  160,   64,   43,   32,
 /*   890 */    32,   68,   69,   70,   71,  148,   32,   74,   32,  152,
 /*   900 */    72,   69,   33,  156,  157,  158,   66,   41,   42,   86,
 /*   910 */    87,   88,   63,   40,   91,   49,   50,   51,   52,   53,
 /*   920 */    54,   55,   56,   57,   40,   59,   60,   61,   62,   32,
 /*   930 */    64,   40,   47,   40,   68,   69,   70,   71,  148,   45,
 /*   940 */    74,   32,  152,   43,  117,   33,  156,  157,  158,   43,
 /*   950 */    41,   42,   86,   87,   88,   45,  117,   91,   49,   50,
 /*   960 */    51,   52,   53,   54,   55,   56,   57,   33,   59,   60,
 /*   970 */    61,   62,  117,   64,   33,   33,  117,   68,   69,   70,
 /*   980 */    71,  148,  121,   74,   32,  124,  125,  126,   32,  156,
 /*   990 */   157,  158,  110,   41,   42,   86,   87,   88,   36,  107,
 /*  1000 */    91,   49,   50,   51,   52,   53,   54,   55,   56,   57,
 /*  1010 */    32,   59,   60,   61,   62,  107,   64,   32,  107,   36,
 /*  1020 */    68,   69,   70,   71,  148,  121,   74,   32,  124,  125,
 /*  1030 */   126,   32,  156,  157,  158,  107,   41,   42,   86,   87,
 /*  1040 */    88,   32,  107,   91,   49,   50,   51,   52,   53,   54,
 /*  1050 */    55,   56,   57,  168,   59,   60,   61,   62,  168,   64,
 /*  1060 */   168,  168,  168,   68,   69,   70,   71,  148,  168,   74,
 /*  1070 */    32,  168,  168,  168,  168,  156,  157,  158,  168,   41,
 /*  1080 */    42,   86,   87,   88,   47,  168,   91,   49,   50,   51,
 /*  1090 */    52,   53,   54,   55,   56,   57,  168,   59,   60,   61,
 /*  1100 */    62,  168,   64,  168,  168,  168,   68,   69,   70,   71,
 /*  1110 */   168,  168,   74,   32,  168,  168,  168,   80,   81,   12,
 /*  1120 */    83,  168,  168,   42,   86,   87,   88,  168,  168,   91,
 /*  1130 */    49,   50,   51,   52,   53,   54,   55,   56,   57,   32,
 /*  1140 */    59,   60,   61,   62,  168,   64,  168,  168,  168,   68,
 /*  1150 */    69,   70,   71,  105,  168,   74,   49,   50,   51,   52,
 /*  1160 */    53,   54,   55,   56,   57,   58,   59,   86,   87,   88,
 /*  1170 */   122,  168,   91,  168,  168,  127,  128,  129,  130,  131,
 /*  1180 */   132,  133,  134,  135,  136,  137,  138,  139,  140,  141,
 /*  1190 */   142,  143,  144,  105,  168,  168,  168,  113,  114,  115,
 /*  1200 */   116,  168,  118,  119,  156,  157,  158,  123,  168,  168,
 /*  1210 */   122,  168,  168,  168,  168,  127,  128,  129,  130,  131,
 /*  1220 */   132,  133,  134,  135,  136,  137,  138,  139,  140,  141,
 /*  1230 */   142,  143,  144,  168,  114,  105,  116,  168,  118,  119,
 /*  1240 */   168,  168,  168,  123,  156,  157,  158,    1,    2,    3,
 /*  1250 */     4,    5,  122,  168,  168,  168,  168,  127,  128,  129,
 /*  1260 */   130,  131,  132,  133,  134,  135,  136,  137,  138,  139,
 /*  1270 */   140,  141,  142,  143,  144,  105,  116,  168,  168,  168,
 /*  1280 */   120,  168,  168,  123,   38,   39,  156,  157,  158,  168,
 /*  1290 */    44,  168,  122,  168,   48,  168,  168,  127,  128,  129,
 /*  1300 */   130,  131,  132,  133,  134,  135,  136,  137,  138,  139,
 /*  1310 */   140,  141,  142,  143,  144,  109,  105,  168,  112,  113,
 /*  1320 */   114,  115,  116,  168,  118,  119,  156,  157,  158,  123,
 /*  1330 */    35,  168,   37,  122,  168,   40,  168,  168,  127,  128,
 /*  1340 */   129,  130,  131,  132,  133,  134,  135,  136,  137,  138,
 /*  1350 */   139,  140,  141,  142,  143,  144,  148,  168,  105,  168,
 /*  1360 */   168,  168,  168,  168,  156,  157,  158,  156,  157,  158,
 /*  1370 */    43,  163,  164,  168,  168,  122,  168,  168,  168,  168,
 /*  1380 */   127,  128,  129,  130,  131,  132,  133,  134,  135,  136,
 /*  1390 */   137,  138,  139,  140,  141,  142,  143,  144,  105,  168,
 /*  1400 */   168,  168,   75,   76,   77,   78,   79,  168,   81,  156,
 /*  1410 */   157,  158,  168,  168,  168,  122,  168,  168,  168,  168,
 /*  1420 */   127,  128,  129,  130,  131,  132,  133,  134,  135,  136,
 /*  1430 */   137,  138,  139,  140,  141,  142,  143,  144,  168,  105,
 /*  1440 */   168,  112,  168,  114,  115,  116,  168,  118,  119,  156,
 /*  1450 */   157,  158,  123,  168,  168,  168,  122,  168,  168,  168,
 /*  1460 */   168,  127,  128,  129,  130,  131,  132,  133,  134,  135,
 /*  1470 */   136,  137,  138,  139,  140,  141,  142,  143,  144,  168,
 /*  1480 */   168,  105,  168,  168,  145,  168,  168,  148,  168,  168,
 /*  1490 */   156,  157,  158,  168,  168,  156,  157,  158,  122,  168,
 /*  1500 */   168,  168,  168,  127,  128,  129,  130,  131,  132,  133,
 /*  1510 */   134,  135,  136,  137,  138,  139,  140,  141,  142,  143,
 /*  1520 */   144,  105,  168,  168,  168,  168,  168,  168,  145,  168,
 /*  1530 */   168,  148,  156,  157,  158,  168,  168,  168,  122,  156,
 /*  1540 */   157,  158,  168,  127,  128,  129,  130,  131,  132,  133,
 /*  1550 */   134,  135,  136,  137,  138,  139,  140,  141,  142,  143,
 /*  1560 */   144,  168,  105,  168,  168,  168,  114,  115,  116,  168,
 /*  1570 */   168,  119,  156,  157,  158,  123,  168,  168,  168,  122,
 /*  1580 */   168,  168,  168,  168,  127,  128,  129,  130,  131,  132,
 /*  1590 */   133,  134,  135,  136,  137,  138,  139,  140,  141,  142,
 /*  1600 */   143,  144,  168,  168,  105,  168,  168,  145,  168,  168,
 /*  1610 */   148,  168,  168,  156,  157,  158,  168,  168,  156,  157,
 /*  1620 */   158,  122,  168,  168,  168,  168,  127,  128,  129,  130,
 /*  1630 */   131,  132,  133,  134,  135,  136,  137,  138,  139,  140,
 /*  1640 */   141,  142,  143,  144,  105,  168,  168,  168,  168,  168,
 /*  1650 */   168,  145,  168,  168,  148,  156,  157,  158,  168,  168,
 /*  1660 */   168,  122,  156,  157,  158,  168,  127,  128,  129,  130,
 /*  1670 */   131,  132,  133,  134,  135,  136,  137,  138,  139,  140,
 /*  1680 */   141,  142,  143,  144,  168,  105,  168,  145,  168,  168,
 /*  1690 */   148,  168,  168,  168,  168,  156,  157,  158,  156,  157,
 /*  1700 */   158,  168,  122,  168,  168,  168,  168,  127,  128,  129,
 /*  1710 */   130,  131,  132,  133,  134,  135,  136,  137,  138,  139,
 /*  1720 */   140,  141,  142,  143,  144,  168,  168,  105,  168,  168,
 /*  1730 */   168,  168,  168,  148,  168,  168,  156,  157,  158,  168,
 /*  1740 */   168,  156,  157,  158,  122,  168,  168,  168,  168,  127,
 /*  1750 */   128,  129,  130,  131,  132,  133,  134,  135,  136,  137,
 /*  1760 */   138,  139,  140,  141,  142,  143,  144,  105,  168,  168,
 /*  1770 */   168,  168,  168,  168,  168,  168,  168,  148,  156,  157,
 /*  1780 */   158,  168,  168,  168,  122,  156,  157,  158,  168,  127,
 /*  1790 */   128,  129,  130,  131,  132,  133,  134,  135,  136,  137,
 /*  1800 */   138,  139,  140,  141,  142,  143,  144,  168,  105,  168,
 /*  1810 */   168,  168,  168,  148,  168,  168,  148,  168,  156,  157,
 /*  1820 */   158,  156,  157,  158,  156,  157,  158,  168,  168,  168,
 /*  1830 */   127,  128,  129,  130,  131,  132,  133,  134,  135,  136,
 /*  1840 */   137,  138,  139,  140,  141,  142,  143,  144,    7,  168,
 /*  1850 */   168,  168,  168,  168,  168,  168,  168,  168,  168,  156,
 /*  1860 */   157,  158,  168,  168,  168,   24,   25,   26,   27,   28,
 /*  1870 */    29,  168,  168,   32,    7,  168,   31,  168,  168,   34,
 /*  1880 */   168,   36,  168,   38,   39,  168,   45,   42,   47,  168,
 /*  1890 */   168,   24,   25,   26,   27,   28,   29,  168,  168,   32,
 /*  1900 */   168,  148,  168,  168,  168,  168,  168,    7,  168,  156,
 /*  1910 */   157,  158,   45,  168,   47,  162,  168,  168,  165,  166,
 /*  1920 */   167,  125,   81,   82,   24,   25,   26,   27,   28,   29,
 /*  1930 */    89,   90,   32,   92,   93,   94,   95,   96,  168,    1,
 /*  1940 */     2,    3,    4,    5,  148,   45,  168,   47,   81,   82,
 /*  1950 */   168,  168,  156,  157,  158,  168,   89,   90,  168,   92,
 /*  1960 */    93,   94,   95,   96,    1,    2,    3,    4,    5,  168,
 /*  1970 */   168,  168,  168,   73,  168,  148,   38,   39,  168,   41,
 /*  1980 */    42,   81,   44,  156,  157,  158,   48,  168,  168,   89,
 /*  1990 */    90,  168,   92,   93,   94,   95,   96,    7,  168,  168,
 /*  2000 */   168,   38,   39,  168,   41,   42,  168,   44,  168,  168,
 /*  2010 */   168,   48,  168,  168,   24,   25,   26,   27,   28,   29,
 /*  2020 */   168,  168,   32,   33,    7,  168,  168,  168,  168,  168,
 /*  2030 */   148,  168,  168,  168,  168,   45,  168,   47,  156,  157,
 /*  2040 */   158,   24,   25,   26,   27,   28,   29,   30,  168,   32,
 /*  2050 */    12,  168,  168,  168,  168,  168,  168,  168,  168,  168,
 /*  2060 */   168,  168,   45,  168,   47,  168,  168,  148,   30,  168,
 /*  2070 */    32,   81,  168,  168,  168,  156,  157,  158,  168,   89,
 /*  2080 */    90,   12,   92,   93,   94,   95,   96,   49,   50,   51,
 /*  2090 */    52,   53,   54,   55,   56,   57,   58,   59,   81,   30,
 /*  2100 */   168,   32,  168,  168,  168,  168,   89,   90,  168,   92,
 /*  2110 */    93,   94,   95,   96,    7,  168,  168,  168,   49,   50,
 /*  2120 */    51,   52,   53,   54,   55,   56,   57,   58,   59,  168,
 /*  2130 */   168,   24,   25,   26,   27,   28,   29,   30,  168,   32,
 /*  2140 */     7,  148,  168,  168,  168,  168,  168,  168,  168,  156,
 /*  2150 */   157,  158,   45,  168,   47,  168,  168,   24,   25,   26,
 /*  2160 */    27,   28,   29,   30,  168,   32,  148,  168,  168,  168,
 /*  2170 */   168,  168,  168,    7,  156,  157,  158,  168,   45,  168,
 /*  2180 */    47,  168,  168,  165,  166,  167,  168,  168,   81,  168,
 /*  2190 */    24,   25,   26,   27,   28,   29,   89,   90,   32,   92,
 /*  2200 */    93,   94,   95,   96,  168,  168,  168,  168,  168,  168,
 /*  2210 */   168,   45,  168,   47,   81,  168,  168,  168,  168,  168,
 /*  2220 */   168,  168,   89,   90,  168,   92,   93,   94,   95,   96,
 /*  2230 */   168,    8,    9,   10,   11,   12,   13,   14,   15,   16,
 /*  2240 */    17,   18,   19,   20,   21,   22,   23,   81,   82,  168,
 /*  2250 */   168,  168,  168,   30,  168,   89,   90,  168,   92,   93,
 /*  2260 */    94,   95,   96,    7,  168,  168,  168,  168,  148,  168,
 /*  2270 */   168,  168,  168,  168,  168,  168,  156,  157,  158,  168,
 /*  2280 */    24,   25,   26,   27,   28,   29,   30,  167,   32,    7,
 /*  2290 */   148,  168,  168,  168,  168,  168,  168,  168,  156,  157,
 /*  2300 */   158,   45,  168,   47,  168,  168,   24,   25,   26,   27,
 /*  2310 */    28,   29,  168,  168,   32,  148,  168,  168,  168,  168,
 /*  2320 */   168,  168,    7,  156,  157,  158,  168,   45,  168,   47,
 /*  2330 */   163,  164,  168,  168,  168,  168,  168,   81,  168,   24,
 /*  2340 */    25,   26,   27,   28,   29,   89,   90,   32,   92,   93,
 /*  2350 */    94,   95,   96,  168,    1,    2,    3,    4,    5,  168,
 /*  2360 */    45,  168,   47,   81,   82,  168,  168,  168,  168,  168,
 /*  2370 */   168,   89,   90,  168,   92,   93,   94,   95,   96,  168,
 /*  2380 */   168,   43,  168,  168,  168,  168,  168,  168,   73,  168,
 /*  2390 */   148,   38,   39,  168,   41,   42,   81,   44,  156,  157,
 /*  2400 */   158,   48,  168,  168,   89,   90,  168,   92,   93,   94,
 /*  2410 */    95,   96,    7,   75,   76,   77,   78,   79,   80,   81,
 /*  2420 */   168,   83,   84,   85,    1,    2,    3,    4,    5,   24,
 /*  2430 */    25,   26,   27,   28,   29,  148,  168,   32,    7,  148,
 /*  2440 */   168,  168,  168,  156,  157,  158,  168,  156,  157,  158,
 /*  2450 */    45,    0,   47,  168,  168,   24,   25,   26,   27,   28,
 /*  2460 */    29,   38,   39,   32,   41,   42,  168,   44,  168,  168,
 /*  2470 */   168,   48,  168,  168,  168,  168,   45,  168,   47,  168,
 /*  2480 */   168,  168,   31,  168,  168,   34,   81,   36,  168,   38,
 /*  2490 */    39,  168,  168,   42,   89,   90,  148,   92,   93,   94,
 /*  2500 */    95,   96,  168,  168,  156,  157,  158,  148,  168,  168,
 /*  2510 */   168,  168,   81,  168,  168,  156,  157,  158,  168,  168,
 /*  2520 */    89,   90,  168,   92,   93,   94,   95,   96,  168,    8,
 /*  2530 */     9,   10,   11,   12,   13,   14,   15,   16,   17,   18,
 /*  2540 */    19,   20,   21,   22,   23,    8,    9,   10,   11,   12,
 /*  2550 */    13,   14,   15,   16,   17,   18,   19,   20,   21,   22,
 /*  2560 */    23,   40,  168,  148,  168,  168,  148,  168,  168,  168,
 /*  2570 */    33,  156,  157,  158,  156,  157,  158,  168,  168,    8,
 /*  2580 */     9,   10,   11,   12,   13,   14,   15,   16,   17,   18,
 /*  2590 */    19,   20,   21,   22,   23,    8,    9,   10,   11,   12,
 /*  2600 */    13,   14,   15,   16,   17,   18,   19,   20,   21,   22,
 /*  2610 */    23,    8,    9,   10,   11,   12,   13,   14,   15,   16,
 /*  2620 */    17,   18,   19,   20,   21,   22,   23,   40,  168,  168,
 /*  2630 */   168,  148,  168,  168,  168,  168,  168,  168,  168,  156,
 /*  2640 */   157,  158,  148,   40,  168,  168,  163,  164,  168,  168,
 /*  2650 */   156,  157,  158,   82,  168,  168,  168,  163,  164,  168,
 /*  2660 */   168,  168,    8,    9,   10,   11,   12,   13,   14,   15,
 /*  2670 */    16,   17,   18,   19,   20,   21,   22,   23,    8,    9,
 /*  2680 */    10,   11,   12,   13,   14,   15,   16,   17,   18,   19,
 /*  2690 */    20,   21,   22,   23,    8,    9,   10,   11,   12,   13,
 /*  2700 */    14,   15,   16,   17,   18,   19,   20,   21,   22,   23,
 /*  2710 */    40,  168,  148,  168,  168,  148,  168,  168,  168,   33,
 /*  2720 */   156,  157,  158,  156,  157,  158,  168,  168,  168,  148,
 /*  2730 */   168,  168,  148,  168,  168,  168,   82,  156,  157,  158,
 /*  2740 */   156,  157,  158,  168,  168,    8,    9,   10,   11,   12,
 /*  2750 */    13,   14,   15,   16,   17,   18,   19,   20,   21,   22,
 /*  2760 */    23,   12,  168,  168,  168,  148,  168,  168,  148,  168,
 /*  2770 */   168,  168,   12,  156,  157,  158,  156,  157,  158,   30,
 /*  2780 */   168,   32,  168,    1,    2,    3,    4,    5,  168,  168,
 /*  2790 */    30,  168,   32,  168,  168,  168,  168,  168,   49,   50,
 /*  2800 */    51,   52,   53,   54,   55,   56,   57,   58,   59,   49,
 /*  2810 */    50,   51,   52,   53,   54,   55,   56,   57,   58,   59,
 /*  2820 */    38,   39,  168,   41,   42,    1,    2,    3,    4,    5,
 /*  2830 */    48,  148,  168,  168,  168,  148,  168,  168,  148,  156,
 /*  2840 */   157,  158,  148,  156,  157,  158,  156,  157,  158,  168,
 /*  2850 */   156,  157,  158,  168,  168,  168,  168,  168,  168,  168,
 /*  2860 */   168,  168,   38,   39,  168,   41,   42,    1,    2,    3,
 /*  2870 */     4,    5,   48,  168,  168,  168,  168,  168,  168,  168,
 /*  2880 */   168,  168,  168,  168,  168,  168,  168,  168,  168,  168,
 /*  2890 */   168,  168,  168,  168,  168,  168,  168,  168,  168,  168,
 /*  2900 */   168,  168,  168,  168,   38,   39,  168,   41,   42,    1,
 /*  2910 */     2,    3,    4,    5,   48,  168,  168,  168,  168,  168,
 /*  2920 */   168,  168,  168,  168,  168,  168,  168,  168,  168,  168,
 /*  2930 */   168,  168,  168,  168,  168,  168,  168,  168,  168,  168,
 /*  2940 */   168,  168,  168,  168,  168,  168,   38,   39,  168,   41,
 /*  2950 */    42,    1,    2,    3,    4,    5,   48,  168,  168,  168,
 /*  2960 */   168,  168,  168,  168,  168,  168,  168,  168,  168,  168,
 /*  2970 */   168,  168,  168,  168,  168,  168,  168,  168,  168,  168,
 /*  2980 */   168,  168,   32,  168,  168,  168,  168,  168,   38,   39,
 /*  2990 */     1,    2,    3,    4,    5,  168,   46,  168,   48,  168,
 /*  3000 */   168,  168,  168,  168,  168,  168,  168,  168,  168,  168,
 /*  3010 */   168,  168,  168,  168,  168,  168,  168,  168,  168,  168,
 /*  3020 */   168,   32,  168,  168,  168,  168,  168,   38,   39,    1,
 /*  3030 */     2,    3,    4,    5,  168,   46,  168,   48,  168,  168,
 /*  3040 */   168,  168,  168,  168,  168,  168,  168,  168,  168,  168,
 /*  3050 */   168,  168,  168,  168,  168,  168,  168,  168,  168,  168,
 /*  3060 */   168,  168,  168,  168,  168,  168,   38,   39,  168,   41,
 /*  3070 */    42,    1,    2,    3,    4,    5,   48,  168,  168,  168,
 /*  3080 */   168,  168,  168,  168,  168,  168,  168,  168,  168,  168,
 /*  3090 */   168,  168,  168,  168,  168,  168,  168,  168,  168,  168,
 /*  3100 */   168,  168,  168,  168,  168,  168,  168,  168,   38,   39,
 /*  3110 */   168,   41,   42,    1,    2,    3,    4,    5,   48,  168,
 /*  3120 */   168,  168,  168,  168,  168,  168,  168,  168,  168,  168,
 /*  3130 */   168,  168,  168,  168,  168,  168,  168,  168,  168,  168,
 /*  3140 */   168,  168,  168,  168,  168,  168,  168,  168,  168,  168,
 /*  3150 */    38,   39,  168,   41,   42,    1,    2,    3,    4,    5,
 /*  3160 */    48,    1,    2,    3,    4,    5,    1,    2,    3,    4,
 /*  3170 */     5,  168,    1,    2,    3,    4,    5,    1,    2,    3,
 /*  3180 */     4,    5,  168,  168,  168,  168,  168,  168,  168,  168,
 /*  3190 */   168,  168,   38,   39,  168,   41,   42,  168,   38,   39,
 /*  3200 */   168,  168,   48,   38,   39,  168,   46,  168,   48,   38,
 /*  3210 */    39,   46,  168,   48,   38,   39,  168,   46,  168,   48,
 /*  3220 */   168,  168,   46,  168,   48,
};
#define YY_SHIFT_USE_DFLT (-27)
static short yy_shift_ofst[] = {
 /*     0 */  1845,   97, 2451,  -27,  -27,  -27,  -27,  -27,   44,  105,
 /*    10 */   -27,   80,  373,  -27,  150,  167,  -27, 2908,  -27, 3070,
 /*    20 */   -27,  -27, 3171,  262,  300, 2038,  346,  -27,  496,  355,
 /*    30 */   -27, 1107,  -27,  408,   79,  -27,  -27,  -27,  -27,  -27,
 /*    40 */   -27,  -27,  -27,  428,  472,   79,  -27,  526,  519,   79,
 /*    50 */   -27,  553,  574,  556,  -27,  -27,  -27,  -27,  -27,  -27,
 /*    60 */   -27,  -27,  -27,  -27,  -27,  -27,  -27,  -27,  -27,  -27,
 /*    70 */   -27,  -27,  -27,  -27,  -27,   91, 3165,  557,  548, 2069,
 /*    80 */   566,  -27,  550,  567,  -27,  -27,  -27,  569, 1295,  -27,
 /*    90 */   579,  572,  -27, 1963,  -27,  587,  -27, 1938, 3154,  -27,
 /*   100 */  3176,  598,  584, 2760,  592,  178,  -27,  264,  -27,  -27,
 /*   110 */   -27,  -27,  -27,  -27,  -27,  -27,  -27,  -27,  -27,  -27,
 /*   120 */   -27,  -27,  -27,  -27,  -27,  -27,  -27,  -27,  605,  -27,
 /*   130 */   609,  -27, 2405,  -24,  909,  823,  580,  604,   92,  694,
 /*   140 */   -27, 2405,  614,   90,  -27,  -26,  -27,  -27,   79,  600,
 /*   150 */  1081, 1081,  631,  436,  737,  -27, 2405,  633,  780,  651,
 /*   160 */   -27,  634,  995,  135,  606, 2405,  648,  -27, 2405,  -27,
 /*   170 */  2405,  652,  -27,  610, 2405, 2405, 2737, 2405,  236, 2405,
 /*   180 */   236, 2405,  236, 2405,  236, 2405,  236, 2405,  236, 2405,
 /*   190 */   236, 2405,   13, 2405,   13, 2405,  -27, 2405,  -27, 2405,
 /*   200 */   -27, 2405,   13, 2405,   -9, 2405,   -9,   -7, 2223,  -27,
 /*   210 */   657, 2405,  -27, 1037,  665,  662, 2256, 2737, 2405,  -27,
 /*   220 */   666,  708,  652,  -27, 2405,  -27,  -27,  -27,  -27,  -27,
 /*   230 */   -27,  -27,  -27,  -27, 2166, 2737,  306,  426,  670, 2133,
 /*   240 */   686,  687, 2107,  -27,  495,  -27, 2405,  -27,  -27,  -27,
 /*   250 */   -27,  -27,  464,  -27,  -27,  -27, 2405, 2654,  -27, 2017,
 /*   260 */   292,  -27,  -27,  674,  675,  -27,  206,  -27, 2431,  -27,
 /*   270 */   691, 2405,  -27,  -27,  -27,  453,  -27,  -27, 2571,  -27,
 /*   280 */   236, 2737,  720,  164, 1900, 2587, 1081,   49,  -27,  727,
 /*   290 */   466,  -27,  727,  -27, 2338,  -27,  -27,  -27,  -27,  -27,
 /*   300 */   -27, 2405,  -27, 2737,  728,   -6, 2405,  -27, 2282,  138,
 /*   310 */  2405,  -27,  695,  138, 2405,  -27,   45,  -27,  -27,  751,
 /*   320 */  1327, 2405,  -27, 1841,  138, 2405,  -27,  702,  138, 2405,
 /*   330 */   -27, 1867,  138, 2405,  -27,  721,  138, 2405,  -27,  -27,
 /*   340 */   -27,  -27, 2405,  476,  -27, 2405,  -27, 2737,  -27,  769,
 /*   350 */   -27,  783,  -27,  787,  -27, 1990, 2537,  -27,  -27,  781,
 /*   360 */    78,  794,  805,  -27,   75,  745,  810,  -27,  763,  813,
 /*   370 */   -27,  765,  819,  -27, 2405, 2686,  -27,  838,  218,  -27,
 /*   380 */   838,  -27,  845,   79,  -27,  -27,  838,  278,  -27,  838,
 /*   390 */   304,  -27,  838,  321,  -27,  838,  337,  -27,  838,  384,
 /*   400 */   -27,  838,  390,  -27,  838,  513,  -27,  838,  192,  -27,
 /*   410 */   838,  470,  -27,  161,  857,  670,  858,  662,  -27,  -27,
 /*   420 */  2405, 2603, 1081,  565,  -27,  864,  828, 2315, 2670, 1081,
 /*   430 */   866,  -27, 2405, 2521, 1081,  393,  -27,  832, 2405,  869,
 /*   440 */   -27,  -27,  -27,  840, 1081, 1081,  -27,  -27,  849,  873,
 /*   450 */   608,  522,  -27,  -27,  120,  884,  221,  -27,  479,  -27,
 /*   460 */    91, 3160,  897,  885, 2749,  891,  952,  -27, 1038,  -27,
 /*   470 */   539,  893,  350,  -27,  307,  -27, 2353, 2824,  -27,  -27,
 /*   480 */  1246,  894,  900,   79,  912,  -27,  910,  906,   79,  934,
 /*   490 */   -27,  -27, 1246, 2989,  423,  -27,   79,  941,  -27, 2950,
 /*   500 */    89,  -27,   79,  942,  -27,  -27, 2423, 2782, 2866,   91,
 /*   510 */  3112,  -27, 3028,  956,  325,  -27,  956,  -27,  -27,  -27,
 /*   520 */   962,  978,  517,  -27,  985,  572,  -27,  983,  999,  210,
 /*   530 */   -27, 1009,  572,  -27,  -27,
};
#define YY_REDUCE_USE_DFLT (-137)
static short yy_reduce_ofst[] = {
 /*     0 */   -72, -137,  755, -137, -137, -137, -137, -137, -137, -137,
 /*    10 */  -137, -137,   87, -137, -137,  137, -137,    5, -137, 1160,
 /*    20 */  -137, -137,  156, -137, -137,  603, -137, -137, -137, -137,
 /*    30 */  -137,   76, -137, -137,  280, -137, -137, -137, -137, -137,
 /*    40 */  -137, -137, -137, -137, -137,  420, -137, -137, -137,  451,
 /*    50 */  -137, -137, -137, -137, -137, -137, -137, -137, -137, -137,
 /*    60 */  -137, -137, -137, -137, -137, -137, -137, -137, -137, -137,
 /*    70 */  -137, -137, -137, -137, -137,   95,  156, -137, -137,  259,
 /*    80 */  -137, -137, -137, -137, -137, -137, -137, -137,  498, -137,
 /*    90 */  -137,  516, -137, 1206, -137, -137, -137, 1084,  -39, -137,
 /*   100 */   156, -137, -137,  861, -137, 1662, -137, 1703, -137, -137,
 /*   110 */  -137, -137, -137, -137, -137, -137, -137, -137, -137, -137,
 /*   120 */  -137, -137, -137, -137, -137, -137, -137, -137, -137, -137,
 /*   130 */  -137, -137, 1339, -137, 1376, 1703, -137, -137, 1293, 1703,
 /*   140 */  -137, 1506, -137,  -32, -137,  501, -137, -137,  538, -137,
 /*   150 */  1416, 1703, -137, 1457, 1703, -137, 1542, -137, 1130, 1703,
 /*   160 */  -137, -137, 1539, 1703, -137, 1383, -137, -137, 2348, -137,
 /*   170 */  1882,  525, -137, -137,  919, 2687, -137, 2683, -137, 2581,
 /*   180 */  -137, 2564, -137, 2418, -137, 2359, -137, 2287, -137, 1993,
 /*   190 */  -137, 1919, -137, 2142, -137,  833, -137, 2620, -137, 1629,
 /*   200 */  -137, 1668, -137, 2567, -137, 2415, -137, 1796, -137, -137,
 /*   210 */  -137, 1827, -137, -137, -137, -137, 2167, -137, 2617, -137,
 /*   220 */  -137, -137,  555, -137, 2584, -137, -137, -137, -137, -137,
 /*   230 */  -137, -137, -137, -137, 1753, -137, -137, -137, -137, 2483,
 /*   240 */  -137, -137, 2494, -137, -137, -137,  705, -137, -137, -137,
 /*   250 */  -137, -137, -137, -137, -137, -137, 2291, -137, -137, 1208,
 /*   260 */  -137, -137, -137, -137, -137, -137, -137, -137, 2018, -137,
 /*   270 */  -137, 2120, -137, -137, -137, -137, -137, -137, -137, -137,
 /*   280 */  -137, -137, -137, -137, 2242, -137, 1253, 1703, -137,  290,
 /*   290 */  -137, -137,  611, -137,  616, -137, -137, -137, -137, -137,
 /*   300 */  -137,  403, -137, -137, -137,  620,  618, -137,  532,  621,
 /*   310 */   489, -137, -137,  622,  360, -137, -137, -137, -137, -137,
 /*   320 */   644,  747, -137,  661,  649,  790, -137, -137,  653,  188,
 /*   330 */  -137,  274,  659,  317, -137, -137,  664,  446, -137, -137,
 /*   340 */  -137, -137,  145, -137, -137,  575, -137, -137, -137, -137,
 /*   350 */  -137, -137, -137, -137, -137, 1665, -137, -137, -137, -137,
 /*   360 */  -137, -137, -137, -137, -137, -137, -137, -137, -137, -137,
 /*   370 */  -137, -137, -137, -137, 1585, -137, -137,  101, -137, -137,
 /*   380 */   726, -137, -137,  764, -137, -137,  108, -137, -137,  170,
 /*   390 */  -137, -137,  182, -137, -137,  214, -137, -137,  256, -137,
 /*   400 */  -137,  269, -137, -137,  354, -137, -137, -136, -137, -137,
 /*   410 */   127, -137, -137, -137, -137, -137, -137, -137, -137, -137,
 /*   420 */   876, -137, 1580, 1703, -137, -137, -137, 2694, -137, 1170,
 /*   430 */  1703, -137, 2690, -137, 1048, 1703, -137, -137, 1462, -137,
 /*   440 */  -137, -137, -137, -137, 1088, 1703, -137, -137, -137, -137,
 /*   450 */  1334, 1703, -137, -137, -137, -137, 1499, -137, 1703, -137,
 /*   460 */   181,  156, -137, -137,  904, -137, 1211, -137, 1703, -137,
 /*   470 */  -137, -137, 1622, -137, 1703, -137, 1120,  -39, -137, -137,
 /*   480 */   181, -137, -137,  827, -137, -137, -137, -137,  839, -137,
 /*   490 */  -137, -137,  310,  156, -137, -137,  855, -137, -137,  156,
 /*   500 */  -137, -137,  859, -137, -137, -137, 1329, 1452,  -39,  310,
 /*   510 */   -39, -137,  -39,  434,  892, -137,  882, -137, -137, -137,
 /*   520 */  -137, -137,  908, -137, -137,  911, -137, -137, -137,  928,
 /*   530 */  -137, -137,  935, -137, -137,
};
static YYACTIONTYPE yy_default[] = {
 /*     0 */   794,  794,  794,  537,  539,  540,  541,  542,  794,  794,
 /*    10 */   543,  794,  794,  544,  794,  794,  545,  794,  558,  794,
 /*    20 */   559,  579,  794,  794,  794,  794,  794,  591,  794,  794,
 /*    30 */   592,  794,  605,  607,  794,  610,  784,  785,  786,  787,
 /*    40 */   788,  789,  790,  794,  608,  794,  611,  794,  609,  794,
 /*    50 */   612,  794,  794,  794,  614,  615,  616,  617,  618,  619,
 /*    60 */   620,  621,  622,  623,  624,  606,  595,  597,  598,  599,
 /*    70 */   600,  601,  602,  603,  604,  794,  794,  794,  794,  794,
 /*    80 */   794,  593,  794,  794,  594,  596,  580,  794,  794,  546,
 /*    90 */   794,  794,  547,  794,  553,  794,  554,  794,  794,  577,
 /*   100 */   794,  794,  794,  794,  794,  794,  583,  794,  585,  625,
 /*   110 */   627,  628,  629,  630,  631,  632,  633,  634,  635,  636,
 /*   120 */   637,  638,  639,  640,  641,  642,  643,  644,  794,  645,
 /*   130 */   794,  646,  794,  794,  794,  794,  649,  794,  794,  794,
 /*   140 */   650,  794,  794,  794,  653,  794,  654,  655,  794,  794,
 /*   150 */   657,  658,  794,  794,  794,  661,  794,  794,  794,  794,
 /*   160 */   663,  794,  794,  794,  794,  794,  794,  665,  794,  722,
 /*   170 */   794,  794,  723,  794,  794,  794,  724,  794,  726,  794,
 /*   180 */   727,  794,  728,  794,  729,  794,  730,  794,  731,  794,
 /*   190 */   732,  794,  739,  794,  740,  794,  741,  794,  742,  794,
 /*   200 */   743,  794,  744,  794,  745,  794,  746,  794,  794,  733,
 /*   210 */   794,  794,  734,  751,  794,  735,  794,  775,  794,  747,
 /*   220 */   794,  794,  794,  748,  794,  749,  750,  752,  753,  754,
 /*   230 */   755,  756,  757,  758,  794,  783,  751,  794,  736,  794,
 /*   240 */   794,  761,  794,  762,  794,  763,  794,  770,  771,  772,
 /*   250 */   773,  774,  794,  766,  767,  737,  794,  794,  738,  794,
 /*   260 */   794,  764,  765,  752,  753,  759,  794,  760,  794,  776,
 /*   270 */   794,  794,  778,  779,  777,  794,  768,  769,  794,  791,
 /*   280 */   725,  792,  794,  794,  794,  794,  794,  794,  666,  794,
 /*   290 */   794,  670,  794,  671,  794,  673,  674,  675,  676,  677,
 /*   300 */   678,  794,  679,  721,  794,  794,  794,  680,  794,  794,
 /*   310 */   794,  681,  794,  794,  794,  682,  794,  688,  689,  794,
 /*   320 */   794,  794,  683,  794,  794,  794,  684,  794,  794,  794,
 /*   330 */   685,  794,  794,  794,  686,  794,  794,  794,  687,  690,
 /*   340 */   691,  672,  794,  794,  693,  794,  694,  696,  695,  794,
 /*   350 */   697,  794,  698,  794,  699,  794,  794,  700,  701,  794,
 /*   360 */   794,  794,  794,  702,  794,  794,  794,  703,  794,  794,
 /*   370 */   704,  794,  794,  705,  794,  794,  706,  794,  794,  707,
 /*   380 */   794,  717,  719,  794,  720,  718,  794,  794,  708,  794,
 /*   390 */   794,  709,  794,  794,  710,  794,  794,  711,  794,  794,
 /*   400 */   712,  794,  794,  713,  794,  794,  714,  794,  794,  715,
 /*   410 */   794,  794,  716,  794,  794,  794,  794,  794,  793,  626,
 /*   420 */   794,  794,  794,  794,  667,  794,  794,  794,  794,  794,
 /*   430 */   794,  668,  794,  794,  794,  794,  669,  794,  794,  794,
 /*   440 */   664,  662,  660,  794,  794,  659,  656,  651,  647,  794,
 /*   450 */   794,  794,  652,  648,  794,  794,  794,  584,  794,  586,
 /*   460 */   794,  794,  794,  794,  794,  794,  794,  587,  794,  589,
 /*   470 */   794,  794,  794,  588,  794,  590,  794,  794,  575,  578,
 /*   480 */   794,  794,  794,  794,  794,  581,  794,  794,  794,  794,
 /*   490 */   582,  569,  794,  794,  794,  571,  794,  794,  573,  794,
 /*   500 */   794,  572,  794,  794,  574,  576,  794,  794,  794,  794,
 /*   510 */   794,  570,  794,  794,  794,  548,  794,  555,  557,  556,
 /*   520 */   794,  794,  794,  549,  794,  794,  550,  794,  794,  794,
 /*   530 */   551,  794,  794,  552,  538,
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
  "AND",           "OR",            "EQUALS",        "IDENTICAL",   
  "LESS",          "GREATER",       "LESSEQUAL",     "GREATEREQUAL",
  "NOTIDENTICAL",  "NOTEQUALS",     "ADD",           "SUB",         
  "CONCAT",        "MUL",           "DIV",           "MOD",         
  "ISSET",         "FETCH",         "TYPEOF",        "INSTANCEOF",  
  "NEW",           "NOT",           "PARENTHESES_CLOSE",  "NAMESPACE",   
  "IDENTIFIER",    "DOTCOMMA",      "INTERFACE",     "EXTENDS",     
  "CLASS",         "IMPLEMENTS",    "ABSTRACT",      "FINAL",       
  "BRACKET_OPEN",  "BRACKET_CLOSE",  "COMMENT",       "ASSIGN",      
  "CONST",         "CONSTANT",      "FUNCTION",      "PARENTHESES_OPEN",
  "INLINE",        "TYPE_INTEGER",  "TYPE_UINTEGER",  "TYPE_LONG",   
  "TYPE_ULONG",    "TYPE_CHAR",     "TYPE_UCHAR",    "TYPE_DOUBLE", 
  "TYPE_BOOL",     "TYPE_STRING",   "TYPE_ARRAY",    "TYPE_VAR",    
  "BREAK",         "CONTINUE",      "IF",            "ELSE",        
  "SWITCH",        "CASE",          "COLON",         "DEFAULT",     
  "LOOP",          "WHILE",         "DO",            "FOR",         
  "IN",            "REVERSE",       "LET",           "ADDASSIGN",   
  "SUBASSIGN",     "MULASSIGN",     "DIVASSIGN",     "CONCATASSIGN",
  "ARROW",         "SBRACKET_OPEN",  "SBRACKET_CLOSE",  "DOUBLECOLON", 
  "INCR",          "DECR",          "ECHO",          "RETURN",      
  "UNSET",         "INTEGER",       "STRING",        "THROW",       
  "CHAR",          "DOUBLE",        "NULL",          "TRUE",        
  "FALSE",         "error",         "program",       "xx_language", 
  "xx_top_statement_list",  "xx_top_statement",  "xx_namespace_def",  "xx_class_def",
  "xx_interface_def",  "xx_comment",    "xx_interface_body",  "xx_class_body",
  "xx_implements_list",  "xx_class_definition",  "xx_implements",  "xx_interface_methods_definition",
  "xx_class_properties_definition",  "xx_class_consts_definition",  "xx_class_methods_definition",  "xx_class_property_definition",
  "xx_visibility_list",  "xx_literal_expr",  "xx_class_const_definition",  "xx_class_method_definition",
  "xx_interface_method_definition",  "xx_parameter_list",  "xx_statement_list",  "xx_visibility",
  "xx_parameter",  "xx_parameter_type",  "xx_parameter_cast",  "xx_statement",
  "xx_let_statement",  "xx_if_statement",  "xx_loop_statement",  "xx_echo_statement",
  "xx_return_statement",  "xx_fcall_statement",  "xx_mcall_statement",  "xx_scall_statement",
  "xx_unset_statement",  "xx_throw_statement",  "xx_declare_statement",  "xx_break_statement",
  "xx_continue_statement",  "xx_while_statement",  "xx_do_while_statement",  "xx_switch_statement",
  "xx_for_statement",  "xx_eval_expr",  "xx_case_clauses",  "xx_case_clause",
  "xx_common_expr",  "xx_let_assignments",  "xx_let_assignment",  "xx_assignment_operator",
  "xx_assign_expr",  "xx_index_expr",  "xx_echo_expressions",  "xx_echo_expression",
  "xx_mcall_expr",  "xx_fcall_expr",  "xx_scall_expr",  "xx_declare_variable_list",
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
 /*  14 */ "xx_class_def ::= ABSTRACT CLASS IDENTIFIER xx_class_body",
 /*  15 */ "xx_class_def ::= ABSTRACT CLASS IDENTIFIER EXTENDS IDENTIFIER xx_class_body",
 /*  16 */ "xx_class_def ::= FINAL CLASS IDENTIFIER xx_class_body",
 /*  17 */ "xx_class_def ::= FINAL CLASS IDENTIFIER EXTENDS IDENTIFIER xx_class_body",
 /*  18 */ "xx_class_body ::= BRACKET_OPEN BRACKET_CLOSE",
 /*  19 */ "xx_class_body ::= BRACKET_OPEN xx_class_definition BRACKET_CLOSE",
 /*  20 */ "xx_implements_list ::= xx_implements_list COMMA xx_implements",
 /*  21 */ "xx_implements_list ::= xx_implements",
 /*  22 */ "xx_implements ::= IDENTIFIER",
 /*  23 */ "xx_interface_body ::= BRACKET_OPEN BRACKET_CLOSE",
 /*  24 */ "xx_interface_body ::= BRACKET_OPEN xx_interface_methods_definition BRACKET_CLOSE",
 /*  25 */ "xx_class_definition ::= xx_class_properties_definition",
 /*  26 */ "xx_class_definition ::= xx_class_consts_definition",
 /*  27 */ "xx_class_definition ::= xx_class_methods_definition",
 /*  28 */ "xx_class_definition ::= xx_class_properties_definition xx_class_methods_definition",
 /*  29 */ "xx_class_definition ::= xx_class_properties_definition xx_class_consts_definition",
 /*  30 */ "xx_class_definition ::= xx_class_consts_definition xx_class_properties_definition",
 /*  31 */ "xx_class_definition ::= xx_class_consts_definition xx_class_methods_definition",
 /*  32 */ "xx_class_definition ::= xx_class_properties_definition xx_class_consts_definition xx_class_methods_definition",
 /*  33 */ "xx_class_definition ::= xx_class_consts_definition xx_class_properties_definition xx_class_methods_definition",
 /*  34 */ "xx_class_properties_definition ::= xx_class_properties_definition xx_class_property_definition",
 /*  35 */ "xx_class_properties_definition ::= xx_class_property_definition",
 /*  36 */ "xx_class_property_definition ::= COMMENT xx_visibility_list IDENTIFIER DOTCOMMA",
 /*  37 */ "xx_class_property_definition ::= xx_visibility_list IDENTIFIER DOTCOMMA",
 /*  38 */ "xx_class_property_definition ::= COMMENT xx_visibility_list IDENTIFIER ASSIGN xx_literal_expr DOTCOMMA",
 /*  39 */ "xx_class_property_definition ::= xx_visibility_list IDENTIFIER ASSIGN xx_literal_expr DOTCOMMA",
 /*  40 */ "xx_class_consts_definition ::= xx_class_consts_definition xx_class_const_definition",
 /*  41 */ "xx_class_consts_definition ::= xx_class_const_definition",
 /*  42 */ "xx_class_methods_definition ::= xx_class_methods_definition xx_class_method_definition",
 /*  43 */ "xx_class_methods_definition ::= xx_class_method_definition",
 /*  44 */ "xx_interface_methods_definition ::= xx_interface_methods_definition xx_interface_method_definition",
 /*  45 */ "xx_interface_methods_definition ::= xx_interface_method_definition",
 /*  46 */ "xx_class_const_definition ::= COMMENT CONST CONSTANT ASSIGN xx_literal_expr DOTCOMMA",
 /*  47 */ "xx_class_const_definition ::= CONST CONSTANT ASSIGN xx_literal_expr DOTCOMMA",
 /*  48 */ "xx_class_method_definition ::= xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE BRACKET_OPEN BRACKET_CLOSE",
 /*  49 */ "xx_class_method_definition ::= xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE BRACKET_OPEN BRACKET_CLOSE",
 /*  50 */ "xx_class_method_definition ::= xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  51 */ "xx_class_method_definition ::= xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  52 */ "xx_class_method_definition ::= COMMENT xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE BRACKET_OPEN BRACKET_CLOSE",
 /*  53 */ "xx_class_method_definition ::= COMMENT xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE BRACKET_OPEN BRACKET_CLOSE",
 /*  54 */ "xx_class_method_definition ::= COMMENT xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  55 */ "xx_class_method_definition ::= COMMENT xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  56 */ "xx_interface_method_definition ::= xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE DOTCOMMA",
 /*  57 */ "xx_interface_method_definition ::= xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE DOTCOMMA",
 /*  58 */ "xx_interface_method_definition ::= COMMENT xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE DOTCOMMA",
 /*  59 */ "xx_interface_method_definition ::= COMMENT xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE DOTCOMMA",
 /*  60 */ "xx_visibility_list ::= xx_visibility_list xx_visibility",
 /*  61 */ "xx_visibility_list ::= xx_visibility",
 /*  62 */ "xx_visibility ::= PUBLIC",
 /*  63 */ "xx_visibility ::= PROTECTED",
 /*  64 */ "xx_visibility ::= PRIVATE",
 /*  65 */ "xx_visibility ::= STATIC",
 /*  66 */ "xx_visibility ::= SCOPED",
 /*  67 */ "xx_visibility ::= INLINE",
 /*  68 */ "xx_visibility ::= ABSTRACT",
 /*  69 */ "xx_visibility ::= FINAL",
 /*  70 */ "xx_parameter_list ::= xx_parameter_list COMMA xx_parameter",
 /*  71 */ "xx_parameter_list ::= xx_parameter",
 /*  72 */ "xx_parameter ::= IDENTIFIER",
 /*  73 */ "xx_parameter ::= xx_parameter_type IDENTIFIER",
 /*  74 */ "xx_parameter ::= xx_parameter_cast IDENTIFIER",
 /*  75 */ "xx_parameter ::= IDENTIFIER ASSIGN xx_literal_expr",
 /*  76 */ "xx_parameter ::= xx_parameter_type IDENTIFIER ASSIGN xx_literal_expr",
 /*  77 */ "xx_parameter ::= xx_parameter_cast IDENTIFIER ASSIGN xx_literal_expr",
 /*  78 */ "xx_parameter_cast ::= LESS IDENTIFIER GREATER",
 /*  79 */ "xx_parameter_type ::= TYPE_INTEGER",
 /*  80 */ "xx_parameter_type ::= TYPE_UINTEGER",
 /*  81 */ "xx_parameter_type ::= TYPE_LONG",
 /*  82 */ "xx_parameter_type ::= TYPE_ULONG",
 /*  83 */ "xx_parameter_type ::= TYPE_CHAR",
 /*  84 */ "xx_parameter_type ::= TYPE_UCHAR",
 /*  85 */ "xx_parameter_type ::= TYPE_DOUBLE",
 /*  86 */ "xx_parameter_type ::= TYPE_BOOL",
 /*  87 */ "xx_parameter_type ::= TYPE_STRING",
 /*  88 */ "xx_parameter_type ::= TYPE_ARRAY",
 /*  89 */ "xx_parameter_type ::= TYPE_VAR",
 /*  90 */ "xx_statement_list ::= xx_statement_list xx_statement",
 /*  91 */ "xx_statement_list ::= xx_statement",
 /*  92 */ "xx_statement ::= xx_let_statement",
 /*  93 */ "xx_statement ::= xx_if_statement",
 /*  94 */ "xx_statement ::= xx_loop_statement",
 /*  95 */ "xx_statement ::= xx_echo_statement",
 /*  96 */ "xx_statement ::= xx_return_statement",
 /*  97 */ "xx_statement ::= xx_fcall_statement",
 /*  98 */ "xx_statement ::= xx_mcall_statement",
 /*  99 */ "xx_statement ::= xx_scall_statement",
 /* 100 */ "xx_statement ::= xx_unset_statement",
 /* 101 */ "xx_statement ::= xx_throw_statement",
 /* 102 */ "xx_statement ::= xx_declare_statement",
 /* 103 */ "xx_statement ::= xx_break_statement",
 /* 104 */ "xx_statement ::= xx_continue_statement",
 /* 105 */ "xx_statement ::= xx_while_statement",
 /* 106 */ "xx_statement ::= xx_do_while_statement",
 /* 107 */ "xx_statement ::= xx_switch_statement",
 /* 108 */ "xx_statement ::= xx_for_statement",
 /* 109 */ "xx_statement ::= xx_comment",
 /* 110 */ "xx_break_statement ::= BREAK DOTCOMMA",
 /* 111 */ "xx_continue_statement ::= CONTINUE DOTCOMMA",
 /* 112 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN BRACKET_CLOSE",
 /* 113 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN BRACKET_CLOSE ELSE BRACKET_OPEN BRACKET_CLOSE",
 /* 114 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 115 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE ELSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 116 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE ELSE BRACKET_OPEN BRACKET_CLOSE",
 /* 117 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN BRACKET_CLOSE ELSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 118 */ "xx_switch_statement ::= SWITCH xx_eval_expr BRACKET_OPEN BRACKET_CLOSE",
 /* 119 */ "xx_switch_statement ::= SWITCH xx_eval_expr BRACKET_OPEN xx_case_clauses BRACKET_CLOSE",
 /* 120 */ "xx_case_clauses ::= xx_case_clauses xx_case_clause",
 /* 121 */ "xx_case_clauses ::= xx_case_clause",
 /* 122 */ "xx_case_clause ::= CASE xx_literal_expr COLON",
 /* 123 */ "xx_case_clause ::= CASE xx_literal_expr COLON xx_statement_list",
 /* 124 */ "xx_case_clause ::= DEFAULT COLON xx_statement_list",
 /* 125 */ "xx_loop_statement ::= LOOP BRACKET_OPEN BRACKET_CLOSE",
 /* 126 */ "xx_loop_statement ::= LOOP BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 127 */ "xx_while_statement ::= WHILE xx_eval_expr BRACKET_OPEN BRACKET_CLOSE",
 /* 128 */ "xx_while_statement ::= WHILE xx_eval_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 129 */ "xx_do_while_statement ::= DO BRACKET_OPEN BRACKET_CLOSE WHILE xx_eval_expr DOTCOMMA",
 /* 130 */ "xx_do_while_statement ::= DO BRACKET_OPEN xx_statement_list BRACKET_CLOSE WHILE xx_eval_expr DOTCOMMA",
 /* 131 */ "xx_for_statement ::= FOR IDENTIFIER IN xx_common_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 132 */ "xx_for_statement ::= FOR IDENTIFIER IN REVERSE xx_common_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 133 */ "xx_for_statement ::= FOR IDENTIFIER COMMA IDENTIFIER IN xx_common_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 134 */ "xx_for_statement ::= FOR IDENTIFIER COMMA IDENTIFIER IN REVERSE xx_common_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 135 */ "xx_let_statement ::= LET xx_let_assignments DOTCOMMA",
 /* 136 */ "xx_let_assignments ::= xx_let_assignments COMMA xx_let_assignment",
 /* 137 */ "xx_let_assignments ::= xx_let_assignment",
 /* 138 */ "xx_assignment_operator ::= ASSIGN",
 /* 139 */ "xx_assignment_operator ::= ADDASSIGN",
 /* 140 */ "xx_assignment_operator ::= SUBASSIGN",
 /* 141 */ "xx_assignment_operator ::= MULASSIGN",
 /* 142 */ "xx_assignment_operator ::= DIVASSIGN",
 /* 143 */ "xx_assignment_operator ::= CONCATASSIGN",
 /* 144 */ "xx_let_assignment ::= IDENTIFIER xx_assignment_operator xx_assign_expr",
 /* 145 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER xx_assignment_operator xx_assign_expr",
 /* 146 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER SBRACKET_OPEN SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 147 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER SBRACKET_OPEN xx_index_expr SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 148 */ "xx_let_assignment ::= IDENTIFIER DOUBLECOLON IDENTIFIER xx_assignment_operator xx_assign_expr",
 /* 149 */ "xx_let_assignment ::= IDENTIFIER DOUBLECOLON IDENTIFIER SBRACKET_OPEN SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 150 */ "xx_let_assignment ::= IDENTIFIER DOUBLECOLON IDENTIFIER SBRACKET_OPEN xx_index_expr SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 151 */ "xx_let_assignment ::= IDENTIFIER SBRACKET_OPEN SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 152 */ "xx_let_assignment ::= IDENTIFIER SBRACKET_OPEN xx_index_expr SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 153 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER INCR",
 /* 154 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER DECR",
 /* 155 */ "xx_let_assignment ::= IDENTIFIER INCR",
 /* 156 */ "xx_let_assignment ::= IDENTIFIER DECR",
 /* 157 */ "xx_index_expr ::= xx_common_expr",
 /* 158 */ "xx_echo_statement ::= ECHO xx_echo_expressions DOTCOMMA",
 /* 159 */ "xx_echo_expressions ::= xx_echo_expressions COMMA xx_echo_expression",
 /* 160 */ "xx_echo_expressions ::= xx_echo_expression",
 /* 161 */ "xx_echo_expression ::= xx_common_expr",
 /* 162 */ "xx_mcall_statement ::= xx_mcall_expr DOTCOMMA",
 /* 163 */ "xx_fcall_statement ::= xx_fcall_expr DOTCOMMA",
 /* 164 */ "xx_scall_statement ::= xx_scall_expr DOTCOMMA",
 /* 165 */ "xx_return_statement ::= RETURN xx_common_expr DOTCOMMA",
 /* 166 */ "xx_return_statement ::= RETURN DOTCOMMA",
 /* 167 */ "xx_unset_statement ::= UNSET IDENTIFIER ARROW IDENTIFIER DOTCOMMA",
 /* 168 */ "xx_unset_statement ::= UNSET IDENTIFIER SBRACKET_OPEN IDENTIFIER SBRACKET_CLOSE DOTCOMMA",
 /* 169 */ "xx_unset_statement ::= UNSET IDENTIFIER SBRACKET_OPEN INTEGER SBRACKET_CLOSE DOTCOMMA",
 /* 170 */ "xx_unset_statement ::= UNSET IDENTIFIER SBRACKET_OPEN STRING SBRACKET_CLOSE DOTCOMMA",
 /* 171 */ "xx_throw_statement ::= THROW xx_common_expr DOTCOMMA",
 /* 172 */ "xx_declare_statement ::= TYPE_INTEGER xx_declare_variable_list DOTCOMMA",
 /* 173 */ "xx_declare_statement ::= TYPE_UINTEGER xx_declare_variable_list DOTCOMMA",
 /* 174 */ "xx_declare_statement ::= TYPE_CHAR xx_declare_variable_list DOTCOMMA",
 /* 175 */ "xx_declare_statement ::= TYPE_UCHAR xx_declare_variable_list DOTCOMMA",
 /* 176 */ "xx_declare_statement ::= TYPE_LONG xx_declare_variable_list DOTCOMMA",
 /* 177 */ "xx_declare_statement ::= TYPE_ULONG xx_declare_variable_list DOTCOMMA",
 /* 178 */ "xx_declare_statement ::= TYPE_DOUBLE xx_declare_variable_list DOTCOMMA",
 /* 179 */ "xx_declare_statement ::= TYPE_STRING xx_declare_variable_list DOTCOMMA",
 /* 180 */ "xx_declare_statement ::= TYPE_BOOL xx_declare_variable_list DOTCOMMA",
 /* 181 */ "xx_declare_statement ::= TYPE_VAR xx_declare_variable_list DOTCOMMA",
 /* 182 */ "xx_declare_variable_list ::= xx_declare_variable_list COMMA xx_declare_variable",
 /* 183 */ "xx_declare_variable_list ::= xx_declare_variable",
 /* 184 */ "xx_declare_variable ::= IDENTIFIER",
 /* 185 */ "xx_declare_variable ::= IDENTIFIER ASSIGN xx_literal_expr",
 /* 186 */ "xx_assign_expr ::= xx_common_expr",
 /* 187 */ "xx_common_expr ::= NOT xx_common_expr",
 /* 188 */ "xx_common_expr ::= ISSET xx_isset_expr",
 /* 189 */ "xx_common_expr ::= REQUIRE xx_common_expr",
 /* 190 */ "xx_common_expr ::= xx_common_expr EQUALS xx_common_expr",
 /* 191 */ "xx_common_expr ::= xx_common_expr NOTEQUALS xx_common_expr",
 /* 192 */ "xx_common_expr ::= xx_common_expr IDENTICAL xx_common_expr",
 /* 193 */ "xx_common_expr ::= xx_common_expr NOTIDENTICAL xx_common_expr",
 /* 194 */ "xx_common_expr ::= xx_common_expr LESS xx_common_expr",
 /* 195 */ "xx_common_expr ::= xx_common_expr GREATER xx_common_expr",
 /* 196 */ "xx_common_expr ::= xx_common_expr LESSEQUAL xx_common_expr",
 /* 197 */ "xx_common_expr ::= xx_common_expr GREATEREQUAL xx_common_expr",
 /* 198 */ "xx_common_expr ::= PARENTHESES_OPEN xx_common_expr PARENTHESES_CLOSE",
 /* 199 */ "xx_common_expr ::= PARENTHESES_OPEN xx_parameter_type PARENTHESES_CLOSE xx_common_expr",
 /* 200 */ "xx_common_expr ::= IDENTIFIER ARROW IDENTIFIER",
 /* 201 */ "xx_common_expr ::= IDENTIFIER DOUBLECOLON IDENTIFIER",
 /* 202 */ "xx_common_expr ::= IDENTIFIER DOUBLECOLON CONSTANT",
 /* 203 */ "xx_common_expr ::= IDENTIFIER SBRACKET_OPEN xx_common_expr SBRACKET_CLOSE",
 /* 204 */ "xx_common_expr ::= xx_common_expr ADD xx_common_expr",
 /* 205 */ "xx_common_expr ::= xx_common_expr SUB xx_common_expr",
 /* 206 */ "xx_common_expr ::= xx_common_expr MUL xx_common_expr",
 /* 207 */ "xx_common_expr ::= xx_common_expr DIV xx_common_expr",
 /* 208 */ "xx_common_expr ::= xx_common_expr MOD xx_common_expr",
 /* 209 */ "xx_common_expr ::= xx_common_expr CONCAT xx_common_expr",
 /* 210 */ "xx_common_expr ::= xx_common_expr AND xx_common_expr",
 /* 211 */ "xx_common_expr ::= xx_common_expr OR xx_common_expr",
 /* 212 */ "xx_common_expr ::= INSTANCEOF xx_common_expr",
 /* 213 */ "xx_common_expr ::= FETCH IDENTIFIER COMMA xx_isset_expr",
 /* 214 */ "xx_common_expr ::= TYPEOF xx_common_expr",
 /* 215 */ "xx_common_expr ::= CONSTANT",
 /* 216 */ "xx_common_expr ::= IDENTIFIER",
 /* 217 */ "xx_common_expr ::= INTEGER",
 /* 218 */ "xx_common_expr ::= STRING",
 /* 219 */ "xx_common_expr ::= CHAR",
 /* 220 */ "xx_common_expr ::= DOUBLE",
 /* 221 */ "xx_common_expr ::= NULL",
 /* 222 */ "xx_common_expr ::= TRUE",
 /* 223 */ "xx_common_expr ::= FALSE",
 /* 224 */ "xx_common_expr ::= SBRACKET_OPEN SBRACKET_CLOSE",
 /* 225 */ "xx_common_expr ::= SBRACKET_OPEN xx_array_list SBRACKET_CLOSE",
 /* 226 */ "xx_common_expr ::= NEW IDENTIFIER",
 /* 227 */ "xx_common_expr ::= NEW IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 228 */ "xx_common_expr ::= NEW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 229 */ "xx_fcall_expr ::= IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 230 */ "xx_fcall_expr ::= IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 231 */ "xx_scall_expr ::= IDENTIFIER DOUBLECOLON IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 232 */ "xx_scall_expr ::= IDENTIFIER DOUBLECOLON IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 233 */ "xx_mcall_expr ::= IDENTIFIER ARROW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 234 */ "xx_mcall_expr ::= IDENTIFIER ARROW IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 235 */ "xx_common_expr ::= xx_mcall_expr",
 /* 236 */ "xx_common_expr ::= xx_scall_expr",
 /* 237 */ "xx_common_expr ::= xx_fcall_expr",
 /* 238 */ "xx_call_parameters ::= xx_call_parameters COMMA xx_call_parameter",
 /* 239 */ "xx_call_parameters ::= xx_call_parameter",
 /* 240 */ "xx_call_parameter ::= xx_common_expr",
 /* 241 */ "xx_array_list ::= xx_array_list COMMA xx_array_item",
 /* 242 */ "xx_array_list ::= xx_array_item",
 /* 243 */ "xx_array_item ::= xx_array_key COLON xx_array_value",
 /* 244 */ "xx_array_item ::= xx_array_value",
 /* 245 */ "xx_array_key ::= IDENTIFIER",
 /* 246 */ "xx_array_key ::= STRING",
 /* 247 */ "xx_array_key ::= INTEGER",
 /* 248 */ "xx_array_value ::= xx_common_expr",
 /* 249 */ "xx_literal_expr ::= INTEGER",
 /* 250 */ "xx_literal_expr ::= CHAR",
 /* 251 */ "xx_literal_expr ::= STRING",
 /* 252 */ "xx_literal_expr ::= DOUBLE",
 /* 253 */ "xx_literal_expr ::= NULL",
 /* 254 */ "xx_literal_expr ::= FALSE",
 /* 255 */ "xx_literal_expr ::= TRUE",
 /* 256 */ "xx_isset_expr ::= IDENTIFIER SBRACKET_OPEN xx_common_expr SBRACKET_CLOSE",
 /* 257 */ "xx_eval_expr ::= xx_common_expr",
 /* 258 */ "xx_comment ::= COMMENT",
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
// 897 "parser.lemon"
{
	/*if ((yypminor->yy0)) {
		if ((yypminor->yy0)->free_flag) {
			efree((yypminor->yy0)->token);
		}
		efree((yypminor->yy0));
	}*/
}
// 2316 "parser.c"
      break;
    case 99:
// 910 "parser.lemon"
{ json_object_put((yypminor->yy175)); }
// 2321 "parser.c"
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
  { 98, 1 },
  { 99, 1 },
  { 100, 2 },
  { 100, 1 },
  { 101, 1 },
  { 101, 1 },
  { 101, 1 },
  { 101, 1 },
  { 102, 3 },
  { 104, 3 },
  { 104, 5 },
  { 103, 3 },
  { 103, 5 },
  { 103, 5 },
  { 103, 4 },
  { 103, 6 },
  { 103, 4 },
  { 103, 6 },
  { 107, 2 },
  { 107, 3 },
  { 108, 3 },
  { 108, 1 },
  { 110, 1 },
  { 106, 2 },
  { 106, 3 },
  { 109, 1 },
  { 109, 1 },
  { 109, 1 },
  { 109, 2 },
  { 109, 2 },
  { 109, 2 },
  { 109, 2 },
  { 109, 3 },
  { 109, 3 },
  { 112, 2 },
  { 112, 1 },
  { 115, 4 },
  { 115, 3 },
  { 115, 6 },
  { 115, 5 },
  { 113, 2 },
  { 113, 1 },
  { 114, 2 },
  { 114, 1 },
  { 111, 2 },
  { 111, 1 },
  { 118, 6 },
  { 118, 5 },
  { 119, 7 },
  { 119, 8 },
  { 119, 8 },
  { 119, 9 },
  { 119, 8 },
  { 119, 9 },
  { 119, 9 },
  { 119, 10 },
  { 120, 6 },
  { 120, 7 },
  { 120, 7 },
  { 120, 8 },
  { 116, 2 },
  { 116, 1 },
  { 123, 1 },
  { 123, 1 },
  { 123, 1 },
  { 123, 1 },
  { 123, 1 },
  { 123, 1 },
  { 123, 1 },
  { 123, 1 },
  { 121, 3 },
  { 121, 1 },
  { 124, 1 },
  { 124, 2 },
  { 124, 2 },
  { 124, 3 },
  { 124, 4 },
  { 124, 4 },
  { 126, 3 },
  { 125, 1 },
  { 125, 1 },
  { 125, 1 },
  { 125, 1 },
  { 125, 1 },
  { 125, 1 },
  { 125, 1 },
  { 125, 1 },
  { 125, 1 },
  { 125, 1 },
  { 125, 1 },
  { 122, 2 },
  { 122, 1 },
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
  { 127, 1 },
  { 127, 1 },
  { 127, 1 },
  { 127, 1 },
  { 127, 1 },
  { 127, 1 },
  { 127, 1 },
  { 139, 2 },
  { 140, 2 },
  { 129, 4 },
  { 129, 7 },
  { 129, 5 },
  { 129, 9 },
  { 129, 8 },
  { 129, 8 },
  { 143, 4 },
  { 143, 5 },
  { 146, 2 },
  { 146, 1 },
  { 147, 3 },
  { 147, 4 },
  { 147, 3 },
  { 130, 3 },
  { 130, 4 },
  { 141, 4 },
  { 141, 5 },
  { 142, 6 },
  { 142, 7 },
  { 144, 7 },
  { 144, 8 },
  { 144, 9 },
  { 144, 10 },
  { 128, 3 },
  { 149, 3 },
  { 149, 1 },
  { 151, 1 },
  { 151, 1 },
  { 151, 1 },
  { 151, 1 },
  { 151, 1 },
  { 151, 1 },
  { 150, 3 },
  { 150, 5 },
  { 150, 7 },
  { 150, 8 },
  { 150, 5 },
  { 150, 7 },
  { 150, 8 },
  { 150, 5 },
  { 150, 6 },
  { 150, 4 },
  { 150, 4 },
  { 150, 2 },
  { 150, 2 },
  { 153, 1 },
  { 131, 3 },
  { 154, 3 },
  { 154, 1 },
  { 155, 1 },
  { 134, 2 },
  { 133, 2 },
  { 135, 2 },
  { 132, 3 },
  { 132, 2 },
  { 136, 5 },
  { 136, 6 },
  { 136, 6 },
  { 136, 6 },
  { 137, 3 },
  { 138, 3 },
  { 138, 3 },
  { 138, 3 },
  { 138, 3 },
  { 138, 3 },
  { 138, 3 },
  { 138, 3 },
  { 138, 3 },
  { 138, 3 },
  { 138, 3 },
  { 159, 3 },
  { 159, 1 },
  { 160, 1 },
  { 160, 3 },
  { 152, 1 },
  { 148, 2 },
  { 148, 2 },
  { 148, 2 },
  { 148, 3 },
  { 148, 3 },
  { 148, 3 },
  { 148, 3 },
  { 148, 3 },
  { 148, 3 },
  { 148, 3 },
  { 148, 3 },
  { 148, 3 },
  { 148, 4 },
  { 148, 3 },
  { 148, 3 },
  { 148, 3 },
  { 148, 4 },
  { 148, 3 },
  { 148, 3 },
  { 148, 3 },
  { 148, 3 },
  { 148, 3 },
  { 148, 3 },
  { 148, 3 },
  { 148, 3 },
  { 148, 2 },
  { 148, 4 },
  { 148, 2 },
  { 148, 1 },
  { 148, 1 },
  { 148, 1 },
  { 148, 1 },
  { 148, 1 },
  { 148, 1 },
  { 148, 1 },
  { 148, 1 },
  { 148, 1 },
  { 148, 2 },
  { 148, 3 },
  { 148, 2 },
  { 148, 4 },
  { 148, 5 },
  { 157, 4 },
  { 157, 3 },
  { 158, 6 },
  { 158, 5 },
  { 156, 6 },
  { 156, 5 },
  { 148, 1 },
  { 148, 1 },
  { 148, 1 },
  { 163, 3 },
  { 163, 1 },
  { 164, 1 },
  { 162, 3 },
  { 162, 1 },
  { 165, 3 },
  { 165, 1 },
  { 166, 1 },
  { 166, 1 },
  { 166, 1 },
  { 167, 1 },
  { 117, 1 },
  { 117, 1 },
  { 117, 1 },
  { 117, 1 },
  { 117, 1 },
  { 117, 1 },
  { 117, 1 },
  { 161, 4 },
  { 145, 1 },
  { 105, 1 },
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
// 906 "parser.lemon"
{
	status->ret = yymsp[0].minor.yy175;
}
// 2797 "parser.c"
        break;
      case 1:
      case 4:
      case 5:
      case 6:
      case 7:
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
      case 104:
      case 105:
      case 106:
      case 107:
      case 108:
      case 109:
      case 157:
      case 186:
      case 235:
      case 236:
      case 237:
      case 240:
      case 248:
      case 257:
// 912 "parser.lemon"
{
	yygotominor.yy175 = yymsp[0].minor.yy175;
}
// 2834 "parser.c"
        break;
      case 2:
      case 34:
      case 40:
      case 42:
      case 44:
      case 60:
      case 90:
      case 120:
// 916 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_list(yymsp[-1].minor.yy175, yymsp[0].minor.yy175);
}
// 2848 "parser.c"
        break;
      case 3:
      case 21:
      case 35:
      case 41:
      case 43:
      case 45:
      case 61:
      case 71:
      case 91:
      case 121:
      case 137:
      case 160:
      case 183:
      case 239:
      case 242:
// 920 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_list(NULL, yymsp[0].minor.yy175);
}
// 2869 "parser.c"
        break;
      case 8:
// 940 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_namespace(yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(31,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 2878 "parser.c"
        break;
      case 9:
// 944 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_interface(yymsp[-1].minor.yy0, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(34,&yymsp[-2].minor);
}
// 2886 "parser.c"
        break;
      case 10:
// 948 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_interface(yymsp[-3].minor.yy0, yymsp[0].minor.yy175, yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(34,&yymsp[-4].minor);
  yy_destructor(35,&yymsp[-2].minor);
}
// 2895 "parser.c"
        break;
      case 11:
// 952 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class(yymsp[-1].minor.yy0, yymsp[0].minor.yy175, 0, 0, NULL, NULL, status->scanner_state);
  yy_destructor(36,&yymsp[-2].minor);
}
// 2903 "parser.c"
        break;
      case 12:
// 956 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy175, 0, 0, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(36,&yymsp[-4].minor);
  yy_destructor(35,&yymsp[-2].minor);
}
// 2912 "parser.c"
        break;
      case 13:
// 960 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy175, 0, 0, NULL, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(36,&yymsp[-4].minor);
  yy_destructor(37,&yymsp[-2].minor);
}
// 2921 "parser.c"
        break;
      case 14:
// 964 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class(yymsp[-1].minor.yy0, yymsp[0].minor.yy175, 1, 0, NULL, NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-3].minor);
  yy_destructor(36,&yymsp[-2].minor);
}
// 2930 "parser.c"
        break;
      case 15:
// 968 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy175, 1, 0, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-5].minor);
  yy_destructor(36,&yymsp[-4].minor);
  yy_destructor(35,&yymsp[-2].minor);
}
// 2940 "parser.c"
        break;
      case 16:
// 972 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class(yymsp[-1].minor.yy0, yymsp[0].minor.yy175, 0, 1, NULL, NULL, status->scanner_state);
  yy_destructor(39,&yymsp[-3].minor);
  yy_destructor(36,&yymsp[-2].minor);
}
// 2949 "parser.c"
        break;
      case 17:
// 976 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy175, 0, 1, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(39,&yymsp[-5].minor);
  yy_destructor(36,&yymsp[-4].minor);
  yy_destructor(35,&yymsp[-2].minor);
}
// 2959 "parser.c"
        break;
      case 18:
      case 23:
// 980 "parser.lemon"
{
	yygotominor.yy175 = NULL;
  yy_destructor(40,&yymsp[-1].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 2969 "parser.c"
        break;
      case 19:
      case 24:
// 984 "parser.lemon"
{
	yygotominor.yy175 = yymsp[-1].minor.yy175;
  yy_destructor(40,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 2979 "parser.c"
        break;
      case 20:
      case 70:
      case 136:
      case 159:
      case 182:
      case 238:
      case 241:
// 988 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_list(yymsp[-2].minor.yy175, yymsp[0].minor.yy175);
  yy_destructor(6,&yymsp[-1].minor);
}
// 2993 "parser.c"
        break;
      case 22:
      case 216:
      case 245:
// 996 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state);
}
// 3002 "parser.c"
        break;
      case 25:
// 1008 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_definition(yymsp[0].minor.yy175, NULL, NULL, status->scanner_state);
}
// 3009 "parser.c"
        break;
      case 26:
// 1012 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_definition(NULL, NULL, yymsp[0].minor.yy175, status->scanner_state);
}
// 3016 "parser.c"
        break;
      case 27:
// 1016 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_definition(NULL, yymsp[0].minor.yy175, NULL, status->scanner_state);
}
// 3023 "parser.c"
        break;
      case 28:
// 1020 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_definition(yymsp[-1].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
}
// 3030 "parser.c"
        break;
      case 29:
// 1024 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_definition(yymsp[-1].minor.yy175, NULL, yymsp[0].minor.yy175, status->scanner_state);
}
// 3037 "parser.c"
        break;
      case 30:
// 1028 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_definition(yymsp[0].minor.yy175, NULL, yymsp[-1].minor.yy175, status->scanner_state);
}
// 3044 "parser.c"
        break;
      case 31:
// 1032 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_definition(NULL, yymsp[0].minor.yy175, yymsp[-1].minor.yy175, status->scanner_state);
}
// 3051 "parser.c"
        break;
      case 32:
// 1036 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_definition(yymsp[-2].minor.yy175, yymsp[0].minor.yy175, yymsp[-1].minor.yy175, status->scanner_state);
}
// 3058 "parser.c"
        break;
      case 33:
// 1040 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_definition(yymsp[-1].minor.yy175, yymsp[0].minor.yy175, yymsp[-2].minor.yy175, status->scanner_state);
}
// 3065 "parser.c"
        break;
      case 36:
// 1056 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_property(yymsp[-2].minor.yy175, yymsp[-1].minor.yy0, NULL, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(33,&yymsp[0].minor);
}
// 3073 "parser.c"
        break;
      case 37:
// 1060 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_property(yymsp[-2].minor.yy175, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(33,&yymsp[0].minor);
}
// 3081 "parser.c"
        break;
      case 38:
// 1064 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_property(yymsp[-4].minor.yy175, yymsp[-3].minor.yy0, yymsp[-1].minor.yy175, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(43,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 3090 "parser.c"
        break;
      case 39:
// 1068 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_property(yymsp[-4].minor.yy175, yymsp[-3].minor.yy0, yymsp[-1].minor.yy175, NULL, status->scanner_state);
  yy_destructor(43,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 3099 "parser.c"
        break;
      case 46:
// 1096 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy175, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(44,&yymsp[-4].minor);
  yy_destructor(43,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 3109 "parser.c"
        break;
      case 47:
// 1100 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy175, NULL, status->scanner_state);
  yy_destructor(44,&yymsp[-4].minor);
  yy_destructor(43,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 3119 "parser.c"
        break;
      case 48:
// 1104 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(yymsp[-6].minor.yy175, yymsp[-4].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(46,&yymsp[-5].minor);
  yy_destructor(47,&yymsp[-3].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(40,&yymsp[-1].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3131 "parser.c"
        break;
      case 49:
// 1108 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(yymsp[-7].minor.yy175, yymsp[-5].minor.yy0, yymsp[-3].minor.yy175, NULL, NULL, status->scanner_state);
  yy_destructor(46,&yymsp[-6].minor);
  yy_destructor(47,&yymsp[-4].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(40,&yymsp[-1].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3143 "parser.c"
        break;
      case 50:
// 1112 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(yymsp[-7].minor.yy175, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy175, NULL, status->scanner_state);
  yy_destructor(46,&yymsp[-6].minor);
  yy_destructor(47,&yymsp[-4].minor);
  yy_destructor(30,&yymsp[-3].minor);
  yy_destructor(40,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3155 "parser.c"
        break;
      case 51:
// 1116 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(yymsp[-8].minor.yy175, yymsp[-6].minor.yy0, yymsp[-4].minor.yy175, yymsp[-1].minor.yy175, NULL, status->scanner_state);
  yy_destructor(46,&yymsp[-7].minor);
  yy_destructor(47,&yymsp[-5].minor);
  yy_destructor(30,&yymsp[-3].minor);
  yy_destructor(40,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3167 "parser.c"
        break;
      case 52:
// 1120 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(yymsp[-6].minor.yy175, yymsp[-4].minor.yy0, NULL, NULL, yymsp[-7].minor.yy0, status->scanner_state);
  yy_destructor(46,&yymsp[-5].minor);
  yy_destructor(47,&yymsp[-3].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(40,&yymsp[-1].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3179 "parser.c"
        break;
      case 53:
// 1124 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(yymsp[-7].minor.yy175, yymsp[-5].minor.yy0, yymsp[-3].minor.yy175, NULL, yymsp[-8].minor.yy0, status->scanner_state);
  yy_destructor(46,&yymsp[-6].minor);
  yy_destructor(47,&yymsp[-4].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(40,&yymsp[-1].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3191 "parser.c"
        break;
      case 54:
// 1128 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(yymsp[-7].minor.yy175, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy175, yymsp[-8].minor.yy0, status->scanner_state);
  yy_destructor(46,&yymsp[-6].minor);
  yy_destructor(47,&yymsp[-4].minor);
  yy_destructor(30,&yymsp[-3].minor);
  yy_destructor(40,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3203 "parser.c"
        break;
      case 55:
// 1132 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(yymsp[-8].minor.yy175, yymsp[-6].minor.yy0, yymsp[-4].minor.yy175, yymsp[-1].minor.yy175, yymsp[-9].minor.yy0, status->scanner_state);
  yy_destructor(46,&yymsp[-7].minor);
  yy_destructor(47,&yymsp[-5].minor);
  yy_destructor(30,&yymsp[-3].minor);
  yy_destructor(40,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3215 "parser.c"
        break;
      case 56:
// 1136 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(yymsp[-5].minor.yy175, yymsp[-3].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(46,&yymsp[-4].minor);
  yy_destructor(47,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[-1].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 3226 "parser.c"
        break;
      case 57:
// 1140 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(yymsp[-6].minor.yy175, yymsp[-4].minor.yy0, yymsp[-2].minor.yy175, NULL, NULL, status->scanner_state);
  yy_destructor(46,&yymsp[-5].minor);
  yy_destructor(47,&yymsp[-3].minor);
  yy_destructor(30,&yymsp[-1].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 3237 "parser.c"
        break;
      case 58:
// 1144 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(yymsp[-5].minor.yy175, yymsp[-3].minor.yy0, NULL, NULL, yymsp[-6].minor.yy0, status->scanner_state);
  yy_destructor(46,&yymsp[-4].minor);
  yy_destructor(47,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[-1].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 3248 "parser.c"
        break;
      case 59:
// 1148 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(yymsp[-6].minor.yy175, yymsp[-4].minor.yy0, yymsp[-2].minor.yy175, NULL, yymsp[-7].minor.yy0, status->scanner_state);
  yy_destructor(46,&yymsp[-5].minor);
  yy_destructor(47,&yymsp[-3].minor);
  yy_destructor(30,&yymsp[-1].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 3259 "parser.c"
        break;
      case 62:
// 1160 "parser.lemon"
{
	yygotominor.yy175 = json_object_new_string("public");
  yy_destructor(1,&yymsp[0].minor);
}
// 3267 "parser.c"
        break;
      case 63:
// 1164 "parser.lemon"
{
	yygotominor.yy175 = json_object_new_string("protected");
  yy_destructor(2,&yymsp[0].minor);
}
// 3275 "parser.c"
        break;
      case 64:
// 1168 "parser.lemon"
{
	yygotominor.yy175 = json_object_new_string("private");
  yy_destructor(4,&yymsp[0].minor);
}
// 3283 "parser.c"
        break;
      case 65:
// 1172 "parser.lemon"
{
	yygotominor.yy175 = json_object_new_string("static");
  yy_destructor(3,&yymsp[0].minor);
}
// 3291 "parser.c"
        break;
      case 66:
// 1176 "parser.lemon"
{
	yygotominor.yy175 = json_object_new_string("scoped");
  yy_destructor(5,&yymsp[0].minor);
}
// 3299 "parser.c"
        break;
      case 67:
// 1180 "parser.lemon"
{
	yygotominor.yy175 = json_object_new_string("inline");
  yy_destructor(48,&yymsp[0].minor);
}
// 3307 "parser.c"
        break;
      case 68:
// 1184 "parser.lemon"
{
	yygotominor.yy175 = json_object_new_string("abstract");
  yy_destructor(38,&yymsp[0].minor);
}
// 3315 "parser.c"
        break;
      case 69:
// 1188 "parser.lemon"
{
	yygotominor.yy175 = json_object_new_string("final");
  yy_destructor(39,&yymsp[0].minor);
}
// 3323 "parser.c"
        break;
      case 72:
// 1200 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_parameter(NULL, NULL, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 3330 "parser.c"
        break;
      case 73:
// 1204 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_parameter(yymsp[-1].minor.yy175, NULL, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 3337 "parser.c"
        break;
      case 74:
// 1208 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_parameter(NULL, yymsp[-1].minor.yy175, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 3344 "parser.c"
        break;
      case 75:
// 1212 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_parameter(NULL, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(43,&yymsp[-1].minor);
}
// 3352 "parser.c"
        break;
      case 76:
// 1216 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_parameter(yymsp[-3].minor.yy175, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(43,&yymsp[-1].minor);
}
// 3360 "parser.c"
        break;
      case 77:
// 1220 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_parameter(NULL, yymsp[-3].minor.yy175, yymsp[-2].minor.yy0, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(43,&yymsp[-1].minor);
}
// 3368 "parser.c"
        break;
      case 78:
// 1224 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(12,&yymsp[-2].minor);
  yy_destructor(13,&yymsp[0].minor);
}
// 3377 "parser.c"
        break;
      case 79:
// 1228 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_type(XX_TYPE_INTEGER);
  yy_destructor(49,&yymsp[0].minor);
}
// 3385 "parser.c"
        break;
      case 80:
// 1232 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_type(XX_TYPE_UINTEGER);
  yy_destructor(50,&yymsp[0].minor);
}
// 3393 "parser.c"
        break;
      case 81:
// 1236 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_type(XX_TYPE_LONG);
  yy_destructor(51,&yymsp[0].minor);
}
// 3401 "parser.c"
        break;
      case 82:
// 1240 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_type(XX_TYPE_ULONG);
  yy_destructor(52,&yymsp[0].minor);
}
// 3409 "parser.c"
        break;
      case 83:
// 1244 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_type(XX_TYPE_CHAR);
  yy_destructor(53,&yymsp[0].minor);
}
// 3417 "parser.c"
        break;
      case 84:
// 1248 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_type(XX_TYPE_UCHAR);
  yy_destructor(54,&yymsp[0].minor);
}
// 3425 "parser.c"
        break;
      case 85:
// 1252 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_type(XX_TYPE_DOUBLE);
  yy_destructor(55,&yymsp[0].minor);
}
// 3433 "parser.c"
        break;
      case 86:
// 1256 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_type(XX_TYPE_BOOL);
  yy_destructor(56,&yymsp[0].minor);
}
// 3441 "parser.c"
        break;
      case 87:
// 1260 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_type(XX_TYPE_STRING);
  yy_destructor(57,&yymsp[0].minor);
}
// 3449 "parser.c"
        break;
      case 88:
// 1264 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_type(XX_TYPE_ARRAY);
  yy_destructor(58,&yymsp[0].minor);
}
// 3457 "parser.c"
        break;
      case 89:
// 1268 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_type(XX_TYPE_VAR);
  yy_destructor(59,&yymsp[0].minor);
}
// 3465 "parser.c"
        break;
      case 110:
// 1352 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_break_statement(status->scanner_state);
  yy_destructor(60,&yymsp[-1].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 3474 "parser.c"
        break;
      case 111:
// 1356 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_continue_statement(status->scanner_state);
  yy_destructor(61,&yymsp[-1].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 3483 "parser.c"
        break;
      case 112:
// 1360 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_if_statement(yymsp[-2].minor.yy175, NULL, NULL, status->scanner_state);
  yy_destructor(62,&yymsp[-3].minor);
  yy_destructor(40,&yymsp[-1].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3493 "parser.c"
        break;
      case 113:
// 1364 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_if_statement(yymsp[-5].minor.yy175, NULL, NULL, status->scanner_state);
  yy_destructor(62,&yymsp[-6].minor);
  yy_destructor(40,&yymsp[-4].minor);
  yy_destructor(41,&yymsp[-3].minor);
  yy_destructor(63,&yymsp[-2].minor);
  yy_destructor(40,&yymsp[-1].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3506 "parser.c"
        break;
      case 114:
// 1368 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_if_statement(yymsp[-3].minor.yy175, yymsp[-1].minor.yy175, NULL, status->scanner_state);
  yy_destructor(62,&yymsp[-4].minor);
  yy_destructor(40,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3516 "parser.c"
        break;
      case 115:
// 1372 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_if_statement(yymsp[-7].minor.yy175, yymsp[-5].minor.yy175, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(62,&yymsp[-8].minor);
  yy_destructor(40,&yymsp[-6].minor);
  yy_destructor(41,&yymsp[-4].minor);
  yy_destructor(63,&yymsp[-3].minor);
  yy_destructor(40,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3529 "parser.c"
        break;
      case 116:
// 1376 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_if_statement(yymsp[-6].minor.yy175, yymsp[-4].minor.yy175, NULL, status->scanner_state);
  yy_destructor(62,&yymsp[-7].minor);
  yy_destructor(40,&yymsp[-5].minor);
  yy_destructor(41,&yymsp[-3].minor);
  yy_destructor(63,&yymsp[-2].minor);
  yy_destructor(40,&yymsp[-1].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3542 "parser.c"
        break;
      case 117:
// 1380 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_if_statement(yymsp[-6].minor.yy175, NULL, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(62,&yymsp[-7].minor);
  yy_destructor(40,&yymsp[-5].minor);
  yy_destructor(41,&yymsp[-4].minor);
  yy_destructor(63,&yymsp[-3].minor);
  yy_destructor(40,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3555 "parser.c"
        break;
      case 118:
// 1384 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_switch_statement(yymsp[-2].minor.yy175, NULL, status->scanner_state);
  yy_destructor(64,&yymsp[-3].minor);
  yy_destructor(40,&yymsp[-1].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3565 "parser.c"
        break;
      case 119:
// 1388 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_switch_statement(yymsp[-3].minor.yy175, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(64,&yymsp[-4].minor);
  yy_destructor(40,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3575 "parser.c"
        break;
      case 122:
// 1400 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_case_clause(yymsp[-1].minor.yy175, NULL, status->scanner_state);
  yy_destructor(65,&yymsp[-2].minor);
  yy_destructor(66,&yymsp[0].minor);
}
// 3584 "parser.c"
        break;
      case 123:
// 1404 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_case_clause(yymsp[-2].minor.yy175, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(65,&yymsp[-3].minor);
  yy_destructor(66,&yymsp[-1].minor);
}
// 3593 "parser.c"
        break;
      case 124:
// 1408 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_case_clause(NULL, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(67,&yymsp[-2].minor);
  yy_destructor(66,&yymsp[-1].minor);
}
// 3602 "parser.c"
        break;
      case 125:
// 1412 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_loop_statement(NULL, status->scanner_state);
  yy_destructor(68,&yymsp[-2].minor);
  yy_destructor(40,&yymsp[-1].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3612 "parser.c"
        break;
      case 126:
// 1416 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_loop_statement(yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(68,&yymsp[-3].minor);
  yy_destructor(40,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3622 "parser.c"
        break;
      case 127:
// 1420 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_while_statement(yymsp[-2].minor.yy175, NULL, status->scanner_state);
  yy_destructor(69,&yymsp[-3].minor);
  yy_destructor(40,&yymsp[-1].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3632 "parser.c"
        break;
      case 128:
// 1424 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_while_statement(yymsp[-3].minor.yy175, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(69,&yymsp[-4].minor);
  yy_destructor(40,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3642 "parser.c"
        break;
      case 129:
// 1428 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_do_while_statement(yymsp[-1].minor.yy175, NULL, status->scanner_state);
  yy_destructor(70,&yymsp[-5].minor);
  yy_destructor(40,&yymsp[-4].minor);
  yy_destructor(41,&yymsp[-3].minor);
  yy_destructor(69,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 3654 "parser.c"
        break;
      case 130:
// 1432 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_do_while_statement(yymsp[-1].minor.yy175, yymsp[-4].minor.yy175, status->scanner_state);
  yy_destructor(70,&yymsp[-6].minor);
  yy_destructor(40,&yymsp[-5].minor);
  yy_destructor(41,&yymsp[-3].minor);
  yy_destructor(69,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 3666 "parser.c"
        break;
      case 131:
// 1436 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_for_statement(yymsp[-3].minor.yy175, NULL, yymsp[-5].minor.yy0, 0, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(71,&yymsp[-6].minor);
  yy_destructor(72,&yymsp[-4].minor);
  yy_destructor(40,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3677 "parser.c"
        break;
      case 132:
// 1440 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_for_statement(yymsp[-3].minor.yy175, NULL, yymsp[-6].minor.yy0, 1, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(71,&yymsp[-7].minor);
  yy_destructor(72,&yymsp[-5].minor);
  yy_destructor(73,&yymsp[-4].minor);
  yy_destructor(40,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3689 "parser.c"
        break;
      case 133:
// 1444 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_for_statement(yymsp[-3].minor.yy175, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, 0, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(71,&yymsp[-8].minor);
  yy_destructor(6,&yymsp[-6].minor);
  yy_destructor(72,&yymsp[-4].minor);
  yy_destructor(40,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3701 "parser.c"
        break;
      case 134:
// 1448 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_for_statement(yymsp[-3].minor.yy175, yymsp[-8].minor.yy0, yymsp[-6].minor.yy0, 1, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(71,&yymsp[-9].minor);
  yy_destructor(6,&yymsp[-7].minor);
  yy_destructor(72,&yymsp[-5].minor);
  yy_destructor(73,&yymsp[-4].minor);
  yy_destructor(40,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3714 "parser.c"
        break;
      case 135:
// 1452 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_statement(yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(74,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 3723 "parser.c"
        break;
      case 138:
// 1465 "parser.lemon"
{
	yygotominor.yy175 = json_object_new_string("assign");
  yy_destructor(43,&yymsp[0].minor);
}
// 3731 "parser.c"
        break;
      case 139:
// 1470 "parser.lemon"
{
	yygotominor.yy175 = json_object_new_string("add-assign");
  yy_destructor(75,&yymsp[0].minor);
}
// 3739 "parser.c"
        break;
      case 140:
// 1475 "parser.lemon"
{
	yygotominor.yy175 = json_object_new_string("sub-assign");
  yy_destructor(76,&yymsp[0].minor);
}
// 3747 "parser.c"
        break;
      case 141:
// 1479 "parser.lemon"
{
	yygotominor.yy175 = json_object_new_string("mult-assign");
  yy_destructor(77,&yymsp[0].minor);
}
// 3755 "parser.c"
        break;
      case 142:
// 1483 "parser.lemon"
{
	yygotominor.yy175 = json_object_new_string("div-assign");
  yy_destructor(78,&yymsp[0].minor);
}
// 3763 "parser.c"
        break;
      case 143:
// 1487 "parser.lemon"
{
	yygotominor.yy175 = json_object_new_string("concat-assign");
  yy_destructor(79,&yymsp[0].minor);
}
// 3771 "parser.c"
        break;
      case 144:
// 1492 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("variable", yymsp[-1].minor.yy175, yymsp[-2].minor.yy0, NULL, NULL, yymsp[0].minor.yy175, status->scanner_state);
}
// 3778 "parser.c"
        break;
      case 145:
// 1497 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("object-property", yymsp[-1].minor.yy175, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(80,&yymsp[-3].minor);
}
// 3786 "parser.c"
        break;
      case 146:
// 1502 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("object-property-append", yymsp[-1].minor.yy175, yymsp[-6].minor.yy0, yymsp[-4].minor.yy0, NULL, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(80,&yymsp[-5].minor);
  yy_destructor(81,&yymsp[-3].minor);
  yy_destructor(82,&yymsp[-2].minor);
}
// 3796 "parser.c"
        break;
      case 147:
// 1507 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("object-property-array-index", yymsp[-1].minor.yy175, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, yymsp[-3].minor.yy175, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(80,&yymsp[-6].minor);
  yy_destructor(81,&yymsp[-4].minor);
  yy_destructor(82,&yymsp[-2].minor);
}
// 3806 "parser.c"
        break;
      case 148:
// 1512 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("static-property", yymsp[-1].minor.yy175, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(83,&yymsp[-3].minor);
}
// 3814 "parser.c"
        break;
      case 149:
// 1517 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("static-property-append", yymsp[-1].minor.yy175, yymsp[-6].minor.yy0, yymsp[-4].minor.yy0, NULL, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(83,&yymsp[-5].minor);
  yy_destructor(81,&yymsp[-3].minor);
  yy_destructor(82,&yymsp[-2].minor);
}
// 3824 "parser.c"
        break;
      case 150:
// 1522 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("static-property-array-index", yymsp[-1].minor.yy175, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, yymsp[-3].minor.yy175, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(83,&yymsp[-6].minor);
  yy_destructor(81,&yymsp[-4].minor);
  yy_destructor(82,&yymsp[-2].minor);
}
// 3834 "parser.c"
        break;
      case 151:
// 1527 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("variable-append", yymsp[-1].minor.yy175, yymsp[-4].minor.yy0, NULL, NULL, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(81,&yymsp[-3].minor);
  yy_destructor(82,&yymsp[-2].minor);
}
// 3843 "parser.c"
        break;
      case 152:
// 1532 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("array-index", yymsp[-1].minor.yy175, yymsp[-5].minor.yy0, NULL, yymsp[-3].minor.yy175, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(81,&yymsp[-4].minor);
  yy_destructor(82,&yymsp[-2].minor);
}
// 3852 "parser.c"
        break;
      case 153:
// 1537 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("incr", NULL, yymsp[-3].minor.yy0, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(80,&yymsp[-2].minor);
  yy_destructor(84,&yymsp[0].minor);
}
// 3861 "parser.c"
        break;
      case 154:
// 1542 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("decr", NULL, yymsp[-3].minor.yy0, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(80,&yymsp[-2].minor);
  yy_destructor(85,&yymsp[0].minor);
}
// 3870 "parser.c"
        break;
      case 155:
// 1547 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("incr", NULL, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(84,&yymsp[0].minor);
}
// 3878 "parser.c"
        break;
      case 156:
// 1552 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("decr", NULL, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(85,&yymsp[0].minor);
}
// 3886 "parser.c"
        break;
      case 158:
// 1560 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_echo_statement(yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(86,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 3895 "parser.c"
        break;
      case 161:
// 1572 "parser.lemon"
{
	yygotominor.yy175 = yymsp[0].minor.yy175;;
}
// 3902 "parser.c"
        break;
      case 162:
// 1577 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_mcall_statement(yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(33,&yymsp[0].minor);
}
// 3910 "parser.c"
        break;
      case 163:
// 1582 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_fcall_statement(yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(33,&yymsp[0].minor);
}
// 3918 "parser.c"
        break;
      case 164:
// 1587 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_scall_statement(yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(33,&yymsp[0].minor);
}
// 3926 "parser.c"
        break;
      case 165:
// 1592 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_return_statement(yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(87,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 3935 "parser.c"
        break;
      case 166:
// 1597 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_return_statement(NULL, status->scanner_state);
  yy_destructor(87,&yymsp[-1].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 3944 "parser.c"
        break;
      case 167:
// 1602 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(88,&yymsp[-4].minor);
  yy_destructor(80,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 3958 "parser.c"
        break;
      case 168:
// 1611 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(88,&yymsp[-5].minor);
  yy_destructor(81,&yymsp[-3].minor);
  yy_destructor(82,&yymsp[-1].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 3973 "parser.c"
        break;
      case 169:
// 1620 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_INTEGER, yymsp[-2].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(88,&yymsp[-5].minor);
  yy_destructor(81,&yymsp[-3].minor);
  yy_destructor(82,&yymsp[-1].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 3988 "parser.c"
        break;
      case 170:
// 1629 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_STRING, yymsp[-2].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(88,&yymsp[-5].minor);
  yy_destructor(81,&yymsp[-3].minor);
  yy_destructor(82,&yymsp[-1].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 4003 "parser.c"
        break;
      case 171:
// 1638 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_throw_exception(yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(91,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 4012 "parser.c"
        break;
      case 172:
// 1642 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_statement(XX_T_TYPE_INTEGER, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 4021 "parser.c"
        break;
      case 173:
// 1646 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_statement(XX_T_TYPE_UINTEGER, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(50,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 4030 "parser.c"
        break;
      case 174:
// 1650 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_statement(XX_T_TYPE_CHAR, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(53,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 4039 "parser.c"
        break;
      case 175:
// 1654 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_statement(XX_T_TYPE_UCHAR, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 4048 "parser.c"
        break;
      case 176:
// 1658 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_statement(XX_T_TYPE_LONG, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(51,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 4057 "parser.c"
        break;
      case 177:
// 1662 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_statement(XX_T_TYPE_ULONG, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(52,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 4066 "parser.c"
        break;
      case 178:
// 1666 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_statement(XX_T_TYPE_DOUBLE, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(55,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 4075 "parser.c"
        break;
      case 179:
// 1670 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_statement(XX_T_TYPE_STRING, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(57,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 4084 "parser.c"
        break;
      case 180:
// 1674 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_statement(XX_T_TYPE_BOOL, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(56,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 4093 "parser.c"
        break;
      case 181:
// 1678 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_statement(XX_T_TYPE_VAR, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(59,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 4102 "parser.c"
        break;
      case 184:
// 1690 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_variable(yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 4109 "parser.c"
        break;
      case 185:
// 1694 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_variable(yymsp[-2].minor.yy0, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(43,&yymsp[-1].minor);
}
// 4117 "parser.c"
        break;
      case 187:
// 1702 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("not", yymsp[0].minor.yy175, NULL, NULL, status->scanner_state);
  yy_destructor(29,&yymsp[-1].minor);
}
// 4125 "parser.c"
        break;
      case 188:
// 1706 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("isset", yymsp[0].minor.yy175, NULL, NULL, status->scanner_state);
  yy_destructor(24,&yymsp[-1].minor);
}
// 4133 "parser.c"
        break;
      case 189:
// 1710 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("require", yymsp[0].minor.yy175, NULL, NULL, status->scanner_state);
  yy_destructor(7,&yymsp[-1].minor);
}
// 4141 "parser.c"
        break;
      case 190:
// 1714 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("equals", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(10,&yymsp[-1].minor);
}
// 4149 "parser.c"
        break;
      case 191:
// 1718 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("not-equals", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(17,&yymsp[-1].minor);
}
// 4157 "parser.c"
        break;
      case 192:
// 1722 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("identical", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(11,&yymsp[-1].minor);
}
// 4165 "parser.c"
        break;
      case 193:
// 1726 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("not-identical", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(16,&yymsp[-1].minor);
}
// 4173 "parser.c"
        break;
      case 194:
// 1730 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("less", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(12,&yymsp[-1].minor);
}
// 4181 "parser.c"
        break;
      case 195:
// 1734 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("greater", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(13,&yymsp[-1].minor);
}
// 4189 "parser.c"
        break;
      case 196:
// 1738 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("less-equal", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(14,&yymsp[-1].minor);
}
// 4197 "parser.c"
        break;
      case 197:
// 1742 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("greater-equal", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(15,&yymsp[-1].minor);
}
// 4205 "parser.c"
        break;
      case 198:
// 1746 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("list", yymsp[-1].minor.yy175, NULL, NULL, status->scanner_state);
  yy_destructor(47,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 4214 "parser.c"
        break;
      case 199:
// 1750 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("cast", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(47,&yymsp[-3].minor);
  yy_destructor(30,&yymsp[-1].minor);
}
// 4223 "parser.c"
        break;
      case 200:
// 1754 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("property-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(80,&yymsp[-1].minor);
}
// 4231 "parser.c"
        break;
      case 201:
// 1758 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("static-property-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(83,&yymsp[-1].minor);
}
// 4239 "parser.c"
        break;
      case 202:
// 1762 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("static-constant-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(83,&yymsp[-1].minor);
}
// 4247 "parser.c"
        break;
      case 203:
      case 256:
// 1767 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), yymsp[-1].minor.yy175, NULL, status->scanner_state);
  yy_destructor(81,&yymsp[-2].minor);
  yy_destructor(82,&yymsp[0].minor);
}
// 4257 "parser.c"
        break;
      case 204:
// 1772 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("add", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(18,&yymsp[-1].minor);
}
// 4265 "parser.c"
        break;
      case 205:
// 1777 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("sub", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(19,&yymsp[-1].minor);
}
// 4273 "parser.c"
        break;
      case 206:
// 1782 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("mul", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(21,&yymsp[-1].minor);
}
// 4281 "parser.c"
        break;
      case 207:
// 1787 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("div", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(22,&yymsp[-1].minor);
}
// 4289 "parser.c"
        break;
      case 208:
// 1792 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("mod", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(23,&yymsp[-1].minor);
}
// 4297 "parser.c"
        break;
      case 209:
// 1797 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("concat", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(20,&yymsp[-1].minor);
}
// 4305 "parser.c"
        break;
      case 210:
// 1802 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("and", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(8,&yymsp[-1].minor);
}
// 4313 "parser.c"
        break;
      case 211:
// 1807 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("or", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(9,&yymsp[-1].minor);
}
// 4321 "parser.c"
        break;
      case 212:
// 1812 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("instanceof", yymsp[0].minor.yy175, NULL, NULL, status->scanner_state);
  yy_destructor(27,&yymsp[-1].minor);
}
// 4329 "parser.c"
        break;
      case 213:
// 1817 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("fetch", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(25,&yymsp[-3].minor);
  yy_destructor(6,&yymsp[-1].minor);
}
// 4338 "parser.c"
        break;
      case 214:
// 1822 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("typeof", yymsp[0].minor.yy175, NULL, NULL, status->scanner_state);
  yy_destructor(26,&yymsp[-1].minor);
}
// 4346 "parser.c"
        break;
      case 215:
// 1827 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_literal(XX_T_CONSTANT, yymsp[0].minor.yy0, status->scanner_state);
}
// 4353 "parser.c"
        break;
      case 217:
      case 247:
      case 249:
// 1837 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_literal(XX_T_INTEGER, yymsp[0].minor.yy0, status->scanner_state);
}
// 4362 "parser.c"
        break;
      case 218:
      case 246:
      case 251:
// 1842 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_literal(XX_T_STRING, yymsp[0].minor.yy0, status->scanner_state);
}
// 4371 "parser.c"
        break;
      case 219:
      case 250:
// 1847 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_literal(XX_T_CHAR, yymsp[0].minor.yy0, status->scanner_state);
}
// 4379 "parser.c"
        break;
      case 220:
      case 252:
// 1852 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_literal(XX_T_DOUBLE, yymsp[0].minor.yy0, status->scanner_state);
}
// 4387 "parser.c"
        break;
      case 221:
      case 253:
// 1857 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_literal(XX_T_NULL, NULL, status->scanner_state);
  yy_destructor(94,&yymsp[0].minor);
}
// 4396 "parser.c"
        break;
      case 222:
      case 255:
// 1862 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_literal(XX_T_TRUE, NULL, status->scanner_state);
  yy_destructor(95,&yymsp[0].minor);
}
// 4405 "parser.c"
        break;
      case 223:
      case 254:
// 1867 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_literal(XX_T_FALSE, NULL, status->scanner_state);
  yy_destructor(96,&yymsp[0].minor);
}
// 4414 "parser.c"
        break;
      case 224:
// 1872 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("empty-array", NULL, NULL, NULL, status->scanner_state);
  yy_destructor(81,&yymsp[-1].minor);
  yy_destructor(82,&yymsp[0].minor);
}
// 4423 "parser.c"
        break;
      case 225:
// 1877 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("array", yymsp[-1].minor.yy175, NULL, NULL, status->scanner_state);
  yy_destructor(81,&yymsp[-2].minor);
  yy_destructor(82,&yymsp[0].minor);
}
// 4432 "parser.c"
        break;
      case 226:
// 1882 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_new_instance(yymsp[0].minor.yy0, NULL, status->scanner_state);
  yy_destructor(28,&yymsp[-1].minor);
}
// 4440 "parser.c"
        break;
      case 227:
// 1887 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_new_instance(yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(28,&yymsp[-3].minor);
  yy_destructor(47,&yymsp[-1].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 4450 "parser.c"
        break;
      case 228:
// 1892 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_new_instance(yymsp[-3].minor.yy0, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(28,&yymsp[-4].minor);
  yy_destructor(47,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 4460 "parser.c"
        break;
      case 229:
// 1897 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_fcall(yymsp[-3].minor.yy0, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(47,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 4469 "parser.c"
        break;
      case 230:
// 1902 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_fcall(yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(47,&yymsp[-1].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 4478 "parser.c"
        break;
      case 231:
// 1907 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_scall(yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(83,&yymsp[-4].minor);
  yy_destructor(47,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 4488 "parser.c"
        break;
      case 232:
// 1912 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_scall(yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(83,&yymsp[-3].minor);
  yy_destructor(47,&yymsp[-1].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 4498 "parser.c"
        break;
      case 233:
// 1917 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_mcall(yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(80,&yymsp[-4].minor);
  yy_destructor(47,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 4508 "parser.c"
        break;
      case 234:
// 1922 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_mcall(yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(80,&yymsp[-3].minor);
  yy_destructor(47,&yymsp[-1].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 4518 "parser.c"
        break;
      case 243:
// 1961 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_array_item(yymsp[-2].minor.yy175, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(66,&yymsp[-1].minor);
}
// 4526 "parser.c"
        break;
      case 244:
// 1965 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_array_item(NULL, yymsp[0].minor.yy175, status->scanner_state);
}
// 4533 "parser.c"
        break;
      case 258:
// 2022 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_comment(yymsp[0].minor.yy0, status->scanner_state);
}
// 4540 "parser.c"
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
// 863 "parser.lemon"


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

// 4616 "parser.c"
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
