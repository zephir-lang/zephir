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
#define YYNSTATE 554
#define YYNRULE 264
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
 /*     0 */   178,  231,    3,    4,    5,    6,    7,  218,  819,    1,
 /*    10 */     2,  553,    4,    5,    6,    7,   32,   43,   47,  174,
 /*    20 */   227,  247,  171,   98,  381,  222,  100,  522,  528,  527,
 /*    30 */   515,  312,  521,  495,  198,  200,  202,   85,  233,  326,
 /*    40 */   212,  278,   54,   55,   56,   57,   58,   59,   60,   61,
 /*    50 */    62,   63,   64,  210,  206,  208,  173,  182,  186,  188,
 /*    60 */   190,  192,  184,  180,  194,  196,  204,  198,  200,  202,
 /*    70 */   363,  394,  401,  387,  235,  241,  359,  364,  254,  256,
 /*    80 */   255,  429,  384,  234,  441,  236,  237,  238,  239,  240,
 /*    90 */   447,  434,   67,   68,   70,   69,   71,  517,  393,  402,
 /*   100 */   411,  414,  405,  408,  417,  423,  420,  518,  426,  131,
 /*   110 */   133,  135,   94,  144,  532,  327,   19,  155,  159,  164,
 /*   120 */   289,   22,  266,  296,  429,   86,  554,  715,   85,   73,
 /*   130 */    74,  529,    9,  139,  434,  497,  358,  371,  375,   72,
 /*   140 */   390,  393,  402,  411,  414,  405,  408,  417,  423,  420,
 /*   150 */   291,  426,  131,  133,  135,  223,  144,  263,   31,  244,
 /*   160 */   155,  159,  164,  289,  331,   96,  296,  429,  492,  101,
 /*   170 */   507,  515,   10,  521,  495,  266,  464,  434,   85,  358,
 /*   180 */   371,  375,   83,  390,  393,  402,  411,  414,  405,  408,
 /*   190 */   417,  423,  420,  509,  426,  131,  133,  135,   93,  144,
 /*   200 */    85,   96,   38,  155,  159,  164,  289,   12,  432,  296,
 /*   210 */   429,   36,  430,   37,   39,   40,   42,   41,  396,  485,
 /*   220 */   434,   13,  358,  371,  375,  302,  390,  393,  402,  411,
 /*   230 */   414,  405,  408,  417,  423,  420,   15,  426,  131,  133,
 /*   240 */   135,  299,  144,  297,  357,  425,  155,  159,  164,  289,
 /*   250 */    17,   82,  296,  429,   65,   43,   47,  303,  304,  305,
 /*   260 */   306,  307,  167,  434,  339,  358,  371,  375,  298,  390,
 /*   270 */   393,  402,  411,  414,  405,  408,  417,  423,  420,   16,
 /*   280 */   426,  131,  133,  135,  544,  144,  532,   76,  275,  155,
 /*   290 */   159,  164,  289,   66,   85,  296,  429,  194,  196,  204,
 /*   300 */   198,  200,  202,  147,  361,  453,  434,   24,  358,  371,
 /*   310 */   375,  302,  390,  393,  402,  411,  414,  405,  408,  417,
 /*   320 */   423,  420,   34,  426,  131,  133,  135,  151,  144,  459,
 /*   330 */   253,  360,  155,  159,  164,  289,   25,  470,  296,  429,
 /*   340 */    65,   43,   47,  303,  304,  305,  306,  307,  489,  434,
 /*   350 */   253,  358,  371,  375,  283,  390,  393,  402,  411,  414,
 /*   360 */   405,  408,  417,  423,  420,  274,  426,  131,  133,  135,
 /*   370 */   377,  144,  380,  477,  268,  155,  159,  164,  289,  310,
 /*   380 */    85,  296,  429,  354,  403,  401,  396,  254,  256,  255,
 /*   390 */   103,  468,  434,  102,  358,  371,  375,   85,  390,  393,
 /*   400 */   402,  411,  414,  405,  408,  417,  423,  420,   27,  426,
 /*   410 */   131,  133,  135,  395,  144,  406,  401,   30,  155,  159,
 /*   420 */   164,  289,  409,  401,  296,  429,   22,  493,   35,  103,
 /*   430 */    21,  494,  495,   85,  163,  434,   85,  358,  371,  375,
 /*   440 */    44,  390,  393,  402,  411,  414,  405,  408,  417,  423,
 /*   450 */   420,   46,  426,  131,  133,  135,  396,  144,  412,  401,
 /*   460 */    45,  155,  159,  164,  289,  415,  401,  296,  429,   48,
 /*   470 */   524,  507,  515,   49,  396,  495,   50,  158,  434,   85,
 /*   480 */   358,  371,  375,  404,  390,  393,  402,  411,  414,  405,
 /*   490 */   408,  417,  423,  420,   52,  426,  131,  133,  135,  396,
 /*   500 */   144,  407,  418,  401,  155,  159,  164,  289,  363,  486,
 /*   510 */   296,  429,   65,   43,   47,  362,  254,  256,  255,  396,
 /*   520 */   469,  434,   53,  358,  371,  375,  410,  390,  393,  402,
 /*   530 */   411,  414,  405,  408,  417,  423,  420,  634,  426,  131,
 /*   540 */   133,  135,  534,  144,  532,  253,  413,  155,  159,  164,
 /*   550 */   289,  310,  245,  296,  429,  350,  421,  401,  396,  254,
 /*   560 */   256,  255,  396,  473,  434,  262,  358,  371,  375,  260,
 /*   570 */   390,  393,  402,  411,  414,  405,  408,  417,  423,  420,
 /*   580 */    78,  426,  131,  133,  135,  416,  144,  424,  401,  419,
 /*   590 */   155,  159,  164,  289,  323,   14,  296,  429,   79,  351,
 /*   600 */    17,  396,  254,  256,  255,   81,  457,  434,  253,  358,
 /*   610 */   371,  375,   84,  390,  393,  402,  411,  414,  405,  408,
 /*   620 */   417,  423,  420,   88,  426,  131,  133,  135,  422,  144,
 /*   630 */   148,  462,  252,  155,  159,  164,  289,  310,  511,  296,
 /*   640 */   429,  346,  427,  401,  396,  254,  256,  255,  512,  143,
 /*   650 */   434,   31,  358,  371,  375,   91,  390,  393,  402,  411,
 /*   660 */   414,  405,  408,  417,  423,  420,   89,  426,  131,  133,
 /*   670 */   135,  428,  144,   92,   31,  471,  155,  159,  164,  289,
 /*   680 */   310,  550,  296,  429,  342,   31,   96,  531,  254,  256,
 /*   690 */   255,   95,  458,  434,   99,  358,  371,  375,  487,  390,
 /*   700 */   393,  402,  411,  414,  405,  408,  417,  423,  420,   29,
 /*   710 */   426,  131,  133,  135,  105,  144,  106,  108,  132,  155,
 /*   720 */   159,  164,  289,  323,  134,  296,  429,  137,  343,  140,
 /*   730 */   141,  254,  256,  255,  150,  109,  434,  146,  358,  371,
 /*   740 */   375,  152,  390,  393,  402,  411,  414,  405,  408,  417,
 /*   750 */   423,  420,  153,  426,  131,  133,  135,  156,  144,  161,
 /*   760 */   165,  168,  155,  159,  164,  289,  310,  170,  296,  429,
 /*   770 */   338,  176,  175,  216,  254,  256,  255,  177,  440,  434,
 /*   780 */   219,  358,  371,  375,  220,  390,  393,  402,  411,  414,
 /*   790 */   405,  408,  417,  423,  420,  224,  426,  131,  133,  135,
 /*   800 */   225,  144,  228,  229,  230,  155,  159,  164,  289,  310,
 /*   810 */   248,  296,  429,  334,  246,  249,  806,  254,  256,  255,
 /*   820 */   805,  463,  434,  290,  358,  371,  375,  301,  390,  393,
 /*   830 */   402,  411,  414,  405,  408,  417,  423,  420,  300,  426,
 /*   840 */   131,  133,  135,  320,  144,  328,  332,  308,  155,  159,
 /*   850 */   164,  289,  310,  313,  296,  429,  330,  317,  321,  329,
 /*   860 */   254,  256,  255,  333,  295,  434,  336,  358,  371,  375,
 /*   870 */   344,  390,  393,  402,  411,  414,  405,  408,  417,  423,
 /*   880 */   420,  337,  426,  131,  133,  135,  341,  144,  366,  345,
 /*   890 */   352,  155,  159,  164,  289,  310,  349,  296,  429,  322,
 /*   900 */   353,  368,  370,  254,  256,  255,  376,  475,  434,  378,
 /*   910 */   358,  371,  375,  379,  390,  393,  402,  411,  414,  405,
 /*   920 */   408,  417,  423,  420,  382,  426,  131,  133,  135,  383,
 /*   930 */   144,  385,  386,  388,  155,  159,  164,  289,  310,  389,
 /*   940 */   296,  429,  318,  398,  397,  399,  254,  256,  255,  400,
 /*   950 */   111,  434,  431,  358,  371,  375,  433,  390,  393,  402,
 /*   960 */   411,  414,  405,  408,  417,  423,  420,  442,  426,  131,
 /*   970 */   133,  135,  443,  144,  454,  456,  460,  155,  159,  164,
 /*   980 */   289,  323,  479,  296,  429,  465,  319,  466,  472,  254,
 /*   990 */   256,  255,  480,  452,  434,  482,  358,  371,  375,  488,
 /*  1000 */   390,  393,  402,  411,  414,  405,  408,  417,  423,  420,
 /*  1010 */   498,  426,  131,  133,  135,  499,  144,  501,  500,  503,
 /*  1020 */   155,  159,  164,  289,  310,  504,  296,  429,  314,  505,
 /*  1030 */   506,  513,  254,  256,  255,  514,  483,  434,  519,  358,
 /*  1040 */   371,  375,  520,  390,  393,  402,  411,  414,  405,  408,
 /*  1050 */   417,  423,  420,  530,  426,  131,  133,  135,  535,  144,
 /*  1060 */   537,  538,  541,  155,  159,  164,  289,  310,  539,  296,
 /*  1070 */   429,  309,   96,  542,  545,  254,  256,  255,  547,  491,
 /*  1080 */   434,  434,  358,  371,  375,  548,  390,  393,  402,  411,
 /*  1090 */   414,  405,  408,  417,  423,  420,  549,  426,  131,  133,
 /*  1100 */   135,  551,  144,  552,  434,  434,  155,  159,  164,  289,
 /*  1110 */   434,    8,  296,  429,   11,  434,   87,  434,  536,  546,
 /*  1120 */   434,   51,  434,  434,  434,  358,  371,  375,  434,  390,
 /*  1130 */   393,  402,  411,  414,  405,  408,  417,  423,  420,   33,
 /*  1140 */   426,  131,  133,  135,  434,  144,  434,  434,  434,  155,
 /*  1150 */   159,  164,  289,  434,  130,  296,   54,   55,   56,   57,
 /*  1160 */    58,   59,   60,   61,   62,   63,   64,  434,  358,  371,
 /*  1170 */   375,  166,  390,  434,  434,  434,  435,  113,  114,  115,
 /*  1180 */   116,  117,  118,  119,  120,  121,  122,  123,  124,  125,
 /*  1190 */   126,  127,  128,  129,  130,  434,  434,  523,  434,  526,
 /*  1200 */   527,  515,  434,  494,  495,  365,  367,  369,   85,   28,
 /*  1210 */   434,  138,   65,   43,   47,  434,  435,  113,  114,  115,
 /*  1220 */   116,  117,  118,  119,  120,  121,  122,  123,  124,  125,
 /*  1230 */   126,  127,  128,  129,  434,  130,   90,  540,  533,  543,
 /*  1240 */   226,   96,   96,  434,  434,  365,  367,  369,  254,  256,
 /*  1250 */   255,  434,  484,  434,  434,  259,  258,  435,  113,  114,
 /*  1260 */   115,  116,  117,  118,  119,  120,  121,  122,  123,  124,
 /*  1270 */   125,  126,  127,  128,  129,  130,  434,  434,  455,  434,
 /*  1280 */   434,  288,  136,  434,  434,  288,  365,  367,  369,  254,
 /*  1290 */   256,  255,  110,  254,  256,  255,  434,  435,  113,  114,
 /*  1300 */   115,  116,  117,  118,  119,  120,  121,  122,  123,  124,
 /*  1310 */   125,  126,  127,  128,  129,  197,  434,  130,  434,  434,
 /*  1320 */   434,  434,  434,  254,  256,  255,  365,  367,  369,   67,
 /*  1330 */    68,   70,   69,   71,  461,  149,  434,  434,  434,  435,
 /*  1340 */   113,  114,  115,  116,  117,  118,  119,  120,  121,  122,
 /*  1350 */   123,  124,  125,  126,  127,  128,  129,  434,  130,  151,
 /*  1360 */   510,  459,  434,  232,  434,  434,   73,   74,  365,  367,
 /*  1370 */   369,  254,  256,  255,  478,  467,   72,  434,  434,  434,
 /*  1380 */   435,  113,  114,  115,  116,  117,  118,  119,  120,  121,
 /*  1390 */   122,  123,  124,  125,  126,  127,  128,  129,  130,  434,
 /*  1400 */   434,  145,  434,  434,  288,  160,  434,  434,  288,  365,
 /*  1410 */   367,  369,  254,  256,  255,  154,  254,  256,  255,  434,
 /*  1420 */   435,  113,  114,  115,  116,  117,  118,  119,  120,  121,
 /*  1430 */   122,  123,  124,  125,  126,  127,  128,  129,  207,  434,
 /*  1440 */   130,  434,  434,  434,  434,  434,  254,  256,  255,  365,
 /*  1450 */   367,  369,   67,   68,   70,   69,   71,  142,  434,  434,
 /*  1460 */   434,  434,  435,  113,  114,  115,  116,  117,  118,  119,
 /*  1470 */   120,  121,  122,  123,  124,  125,  126,  127,  128,  129,
 /*  1480 */   434,  130,  434,  516,  434,  434,  434,  434,  434,   73,
 /*  1490 */    74,  365,  367,  369,  434,  434,  434,  104,  474,   72,
 /*  1500 */   434,  434,  434,  435,  113,  114,  115,  116,  117,  118,
 /*  1510 */   119,  120,  121,  122,  123,  124,  125,  126,  127,  128,
 /*  1520 */   129,  130,  434,  434,  434,  434,  434,  434,  169,  434,
 /*  1530 */   434,  288,  365,  367,  369,  434,  434,  434,  451,  254,
 /*  1540 */   256,  255,  434,  435,  113,  114,  115,  116,  117,  118,
 /*  1550 */   119,  120,  121,  122,  123,  124,  125,  126,  127,  128,
 /*  1560 */   129,  195,  434,  130,  434,  434,  434,  434,  434,  254,
 /*  1570 */   256,  255,  365,  367,  369,   67,   68,   70,   69,   71,
 /*  1580 */   490,  434,  434,  434,  434,  435,  113,  114,  115,  116,
 /*  1590 */   117,  118,  119,  120,  121,  122,  123,  124,  125,  126,
 /*  1600 */   127,  128,  129,  434,  130,  434,  434,  434,  434,  434,
 /*  1610 */   434,  434,   73,   74,  365,  367,  369,  434,  434,  434,
 /*  1620 */    77,  294,   72,  434,  434,  434,  435,  113,  114,  115,
 /*  1630 */   116,  117,  118,  119,  120,  121,  122,  123,  124,  125,
 /*  1640 */   126,  127,  128,  129,  130,  434,  434,  434,  434,  434,
 /*  1650 */   434,  434,  434,  434,  193,  365,  367,  369,  434,  434,
 /*  1660 */   434,  439,  254,  256,  255,  434,  435,  113,  114,  115,
 /*  1670 */   116,  117,  118,  119,  120,  121,  122,  123,  124,  125,
 /*  1680 */   126,  127,  128,  129,  434,  434,  130,  434,  434,  434,
 /*  1690 */   434,  434,  434,  434,  434,  365,  367,  369,   67,   68,
 /*  1700 */    70,   69,   71,  157,  434,  434,  434,  434,  435,  113,
 /*  1710 */   114,  115,  116,  117,  118,  119,  120,  121,  122,  123,
 /*  1720 */   124,  125,  126,  127,  128,  129,  434,  130,  434,  434,
 /*  1730 */   434,  434,  434,  434,  434,   73,   74,  365,  367,  369,
 /*  1740 */   434,  434,  434,   23,  162,   72,  434,  434,  434,  435,
 /*  1750 */   113,  114,  115,  116,  117,  118,  119,  120,  121,  122,
 /*  1760 */   123,  124,  125,  126,  127,  128,  129,  130,  434,  434,
 /*  1770 */   434,  434,  434,  434,  434,  434,  434,  191,  365,  367,
 /*  1780 */   369,  434,  434,  434,  446,  254,  256,  255,  434,  435,
 /*  1790 */   113,  114,  115,  116,  117,  118,  119,  120,  121,  122,
 /*  1800 */   123,  124,  125,  126,  127,  128,  129,  434,  434,  130,
 /*  1810 */   434,  434,  434,  209,  434,  434,  189,  434,  365,  367,
 /*  1820 */   369,  254,  256,  255,  254,  256,  255,  434,  434,  434,
 /*  1830 */   434,  112,  113,  114,  115,  116,  117,  118,  119,  120,
 /*  1840 */   121,  122,  123,  124,  125,  126,  127,  128,  129,  178,
 /*  1850 */   231,  434,  434,  434,  434,  434,  218,  226,  434,  434,
 /*  1860 */   365,  367,  369,  434,  434,  254,  256,  255,  174,  227,
 /*  1870 */   247,  171,  282,  258,  222,  374,  178,  231,  434,  434,
 /*  1880 */   434,  434,  434,  218,  242,  434,  434,  233,  434,  212,
 /*  1890 */   434,  434,  254,  256,  255,  174,  227,  247,  171,  434,
 /*  1900 */   242,  222,  434,  279,  434,  434,  178,  231,  254,  256,
 /*  1910 */   255,  434,  187,  218,  233,  434,  212,  276,  277,  280,
 /*  1920 */   254,  256,  255,  235,  241,  174,  227,  247,  171,  250,
 /*  1930 */   434,  222,  234,  434,  236,  237,  238,  239,  240,  242,
 /*  1940 */   302,  434,  436,  434,  233,  434,  212,  254,  256,  255,
 /*  1950 */   235,  241,  434,  273,  434,  434,  281,  277,  280,  234,
 /*  1960 */   434,  236,  237,  238,  239,  240,  434,  434,  434,  434,
 /*  1970 */   434,  434,  303,  304,  305,  306,  307,  434,  434,  315,
 /*  1980 */   235,  241,  324,  325,  434,  434,  434,  434,  434,  234,
 /*  1990 */   434,  236,  237,  238,  239,  240,  434,  178,  231,  226,
 /*  2000 */   434,  434,  434,  434,  218,  434,  434,  254,  256,  255,
 /*  2010 */   434,  434,  434,  434,  267,  258,  174,  227,  247,  171,
 /*  2020 */   226,  434,  222,  434,  178,  231,  434,  434,  254,  256,
 /*  2030 */   255,  218,  434,  434,  434,  233,  257,  212,  434,  434,
 /*  2040 */   434,  434,  266,  174,  227,  247,  171,  284,  434,  222,
 /*  2050 */   226,  434,   51,  434,  434,  434,  434,  434,  254,  256,
 /*  2060 */   255,  804,  233,  434,  212,  251,  258,  434,  481,  434,
 /*  2070 */    33,  235,  241,  316,  434,  223,  434,  263,  434,  244,
 /*  2080 */   234,  434,  236,  237,  238,  239,  240,   54,   55,   56,
 /*  2090 */    57,   58,   59,   60,   61,   62,   63,   64,  235,  241,
 /*  2100 */   215,   67,   68,   70,   69,   71,  434,  234,  434,  236,
 /*  2110 */   237,  238,  239,  240,  434,  178,  231,  434,  434,  434,
 /*  2120 */   434,  434,  218,  213,  434,  434,  434,  434,  434,  434,
 /*  2130 */   434,  254,  256,  255,  174,  227,  247,  171,   73,   74,
 /*  2140 */   222,  434,  178,  231,  434,  434,  478,  434,   72,  218,
 /*  2150 */   434,  434,  434,  233,  434,  212,  434,  434,  434,  434,
 /*  2160 */   434,  174,  227,  247,  171,  434,  185,  222,  434,  434,
 /*  2170 */   434,  434,  178,  231,  254,  256,  255,  211,  434,  218,
 /*  2180 */   233,  448,  212,  434,  434,  254,  256,  255,  434,  235,
 /*  2190 */   241,  174,  227,  247,  171,  269,  434,  222,  234,  434,
 /*  2200 */   236,  237,  238,  239,  240,   67,   68,   70,   69,   71,
 /*  2210 */   233,  391,  212,  434,  183,  434,  235,  241,  348,  254,
 /*  2220 */   256,  255,  254,  256,  255,  234,  181,  236,  237,  238,
 /*  2230 */   239,  240,  434,  434,  254,  256,  255,  434,  434,  434,
 /*  2240 */   434,  434,   73,   74,  434,  434,  235,  241,  434,  434,
 /*  2250 */   104,  434,   72,  434,  434,  234,  434,  236,  237,  238,
 /*  2260 */   239,  240,  434,  178,  231,  437,  434,  434,  434,  434,
 /*  2270 */   218,  221,  434,  254,  256,  255,  434,  434,  434,  254,
 /*  2280 */   256,  255,  174,  227,  247,  171,  285,  434,  222,  434,
 /*  2290 */   178,  231,  434,  434,  254,  256,  255,  218,  444,  434,
 /*  2300 */   434,  233,  434,  212,  434,  434,  254,  256,  255,  174,
 /*  2310 */   227,  247,  171,  261,  434,  222,  287,  434,   51,  434,
 /*  2320 */   434,  434,  434,  449,  254,  256,  255,  434,  233,  434,
 /*  2330 */   212,  254,  256,  255,   26,  264,   33,  235,  241,  340,
 /*  2340 */   434,  434,  434,  254,  256,  255,  234,  434,  236,  237,
 /*  2350 */   238,  239,  240,   54,   55,   56,   57,   58,   59,   60,
 /*  2360 */    61,   62,   63,   64,  235,  241,  434,   67,   68,   70,
 /*  2370 */    69,   71,  434,  234,  434,  236,  237,  238,  239,  240,
 /*  2380 */   434,  178,  231,  372,  434,  434,  434,  434,  218,  434,
 /*  2390 */   434,  254,  256,  255,  434,  434,  434,  434,  434,  434,
 /*  2400 */   174,  227,  247,  171,   73,   74,  243,  434,  178,  231,
 /*  2410 */   434,  434,  434,  434,   72,  218,  434,  434,  434,  233,
 /*  2420 */   434,  212,  434,  434,  434,  434,  434,  174,  227,  247,
 /*  2430 */   171,  434,  179,  222,  434,  292,   51,  434,  434,  434,
 /*  2440 */   254,  256,  255,  254,  256,  255,  233,  434,  212,  434,
 /*  2450 */   172,  434,   80,  434,   33,  271,  241,  272,  254,  256,
 /*  2460 */   255,  434,  434,  434,  270,  434,  236,  237,  238,  239,
 /*  2470 */   240,   54,   55,   56,   57,   58,   59,   60,   61,   62,
 /*  2480 */    63,   64,  235,  241,  434,  434,  434,  434,  434,  434,
 /*  2490 */   434,  234,  434,  236,  237,  238,  239,  240,  434,  434,
 /*  2500 */   434,  210,  206,  208,  173,  182,  186,  188,  190,  192,
 /*  2510 */   184,  180,  194,  196,  204,  198,  200,  202,  434,  434,
 /*  2520 */   434,  434,  214,  210,  206,  208,  173,  182,  186,  188,
 /*  2530 */   190,  192,  184,  180,  194,  196,  204,  198,  200,  202,
 /*  2540 */   173,  182,  186,  188,  190,  192,  184,  180,  194,  196,
 /*  2550 */   204,  198,  200,  202,  450,  302,  434,  434,  434,  434,
 /*  2560 */   434,  434,  434,  434,  210,  206,  208,  173,  182,  186,
 /*  2570 */   188,  190,  192,  184,  180,  194,  196,  204,  198,  200,
 /*  2580 */   202,  434,  434,  434,  434,  434,  434,  303,  304,  305,
 /*  2590 */   306,  307,  311,  555,  347,  445,  335,  355,  356,  434,
 /*  2600 */   434,  434,  434,  434,  434,  210,  206,  208,  173,  182,
 /*  2610 */   186,  188,  190,  192,  184,  180,  194,  196,  204,  198,
 /*  2620 */   200,  202,  434,  434,    8,  434,  434,   11,  434,   87,
 /*  2630 */   203,  536,  546,  434,  434,  434,  438,  434,  254,  256,
 /*  2640 */   255,  434,  434,  434,  434,  434,  210,  206,  208,  173,
 /*  2650 */   182,  186,  188,  190,  192,  184,  180,  194,  196,  204,
 /*  2660 */   198,  200,  202,  210,  206,  208,  173,  182,  186,  188,
 /*  2670 */   190,  192,  184,  180,  194,  196,  204,  198,  200,  202,
 /*  2680 */   434,  178,  231,  201,  434,  434,  205,  392,  218,  217,
 /*  2690 */   434,  254,  256,  255,  254,  256,  255,  254,  256,  255,
 /*  2700 */   174,  227,  247,  171,  199,  434,  243,  434,  434,  434,
 /*  2710 */   434,  434,  254,  256,  255,  434,  434,  434,  434,  233,
 /*  2720 */   265,  212,  434,  434,  434,  434,  434,  434,  434,  210,
 /*  2730 */   206,  208,  173,  182,  186,  188,  190,  192,  184,  180,
 /*  2740 */   194,  196,  204,  198,  200,  202,  434,  434,  434,  434,
 /*  2750 */   434,  434,  434,  434,  434,  271,  241,  434,  434,  434,
 /*  2760 */   434,  434,  434,  434,  270,  434,  236,  237,  238,  239,
 /*  2770 */   240,  434,  434,  434,  210,  206,  208,  173,  182,  186,
 /*  2780 */   188,  190,  192,  184,  180,  194,  196,  204,  198,  200,
 /*  2790 */   202,   67,   68,   70,   69,   71,  434,  434,  373,  434,
 /*  2800 */   434,  434,  434,  286,  434,  434,  434,  434,  210,  206,
 /*  2810 */   208,  173,  182,  186,  188,  190,  192,  184,  180,  194,
 /*  2820 */   196,  204,  198,  200,  202,  434,  434,  434,   73,   74,
 /*  2830 */   434,  582,  508,  434,  502,  434,  434,  434,   72,  293,
 /*  2840 */    67,   68,   70,   69,   71,  434,  434,  434,  434,  210,
 /*  2850 */   206,  208,  173,  182,  186,  188,  190,  192,  184,  180,
 /*  2860 */   194,  196,  204,  198,  200,  202,   51,  434,  434,  434,
 /*  2870 */   434,  434,  434,  434,  434,  434,  434,   73,   74,  434,
 /*  2880 */    97,  508,  107,  502,   33,  434,  434,   72,  434,   67,
 /*  2890 */    68,   70,   69,   71,  434,  434,  434,  434,  434,  434,
 /*  2900 */   434,   54,   55,   56,   57,   58,   59,   60,   61,   62,
 /*  2910 */    63,   64,   67,   68,   70,   69,   71,  434,  434,  434,
 /*  2920 */   434,  434,  434,  434,  434,  434,   73,   74,  434,  581,
 /*  2930 */   508,  434,  502,  434,  434,  434,   72,  434,   67,   68,
 /*  2940 */    70,   69,   71,   67,   68,   70,   69,   71,  434,   73,
 /*  2950 */    74,  434,  585,  496,  434,  502,  434,  434,  434,   72,
 /*  2960 */   434,   67,   68,   70,   69,   71,   67,   68,   70,   69,
 /*  2970 */    71,  434,  434,  434,  434,   73,   74,  434,  583,  476,
 /*  2980 */    73,   74,  434,   20,   75,   72,  434,  434,  434,  434,
 /*  2990 */    72,  434,   67,   68,   70,   69,   71,  434,   73,   74,
 /*  3000 */   434,  588,  476,   73,   74,  434,  584,  476,   72,  434,
 /*  3010 */   434,  434,  434,   72,  434,   67,   68,   70,   69,   71,
 /*  3020 */    67,   68,   70,   69,   71,  434,  434,  434,  434,   73,
 /*  3030 */    74,  434,  586,  525,   67,   68,   70,   69,   71,   72,
 /*  3040 */   434,  434,  434,  434,  434,  434,  434,  434,  434,  434,
 /*  3050 */   434,  434,   73,   74,  434,  589,  476,   73,   74,  434,
 /*  3060 */   587,  476,   72,  434,  434,  434,  434,   72,  434,  434,
 /*  3070 */   434,   73,   74,  434,   18,   75,  434,  434,  434,  434,
 /*  3080 */   434,   72,
};
static YYCODETYPE yy_lookahead[] = {
 /*     0 */     7,    8,  101,  102,  103,  104,  105,   14,   98,   99,
 /*    10 */   100,  101,  102,  103,  104,  105,  124,  125,  126,   26,
 /*    20 */    27,   28,   29,  109,   32,   32,  112,  113,  114,  115,
 /*    30 */   116,   32,  118,  119,   23,   24,   25,  123,   45,   40,
 /*    40 */    47,   66,   49,   50,   51,   52,   53,   54,   55,   56,
 /*    50 */    57,   58,   59,    9,   10,   11,   12,   13,   14,   15,
 /*    60 */    16,   17,   18,   19,   20,   21,   22,   23,   24,   25,
 /*    70 */   148,  159,  160,   81,   81,   82,  154,  155,  156,  157,
 /*    80 */   158,   32,   90,   90,    6,   92,   93,   94,   95,   96,
 /*    90 */    41,   42,    1,    2,    3,    4,    5,   33,   49,   50,
 /*   100 */    51,   52,   53,   54,   55,   56,   57,   43,   59,   60,
 /*   110 */    61,   62,  108,   64,  110,   32,  111,   68,   69,   70,
 /*   120 */    71,  116,   47,   74,   32,  120,    0,   83,  123,   38,
 /*   130 */    39,    6,   32,   41,   42,   44,   87,   88,   89,   48,
 /*   140 */    91,   49,   50,   51,   52,   53,   54,   55,   56,   57,
 /*   150 */    72,   59,   60,   61,   62,   80,   64,   82,    6,   84,
 /*   160 */    68,   69,   70,   71,   81,   40,   74,   32,  113,  114,
 /*   170 */   115,  116,   33,  118,  119,   47,   41,   42,  123,   87,
 /*   180 */    88,   89,   30,   91,   49,   50,   51,   52,   53,   54,
 /*   190 */    55,   56,   57,  116,   59,   60,   61,   62,   37,   64,
 /*   200 */   123,   40,   81,   68,   69,   70,   71,   32,   80,   74,
 /*   210 */    32,   90,   84,   92,   93,   94,   95,   96,    6,   41,
 /*   220 */    42,  106,   87,   88,   89,   43,   91,   49,   50,   51,
 /*   230 */    52,   53,   54,   55,   56,   57,   32,   59,   60,   61,
 /*   240 */    62,    6,   64,  149,  150,   33,   68,   69,   70,   71,
 /*   250 */    40,  121,   74,   32,  124,  125,  126,   75,   76,   77,
 /*   260 */    78,   79,   41,   42,   82,   87,   88,   89,   33,   91,
 /*   270 */    49,   50,   51,   52,   53,   54,   55,   56,   57,  106,
 /*   280 */    59,   60,   61,   62,  108,   64,  110,  116,    6,   68,
 /*   290 */    69,   70,   71,  123,  123,   74,   32,   20,   21,   22,
 /*   300 */    23,   24,   25,   41,    6,   41,   42,   32,   87,   88,
 /*   310 */    89,   43,   91,   49,   50,   51,   52,   53,   54,   55,
 /*   320 */    56,   57,   43,   59,   60,   61,   62,   65,   64,   67,
 /*   330 */     6,   33,   68,   69,   70,   71,   47,  121,   74,   32,
 /*   340 */   124,  125,  126,   75,   76,   77,   78,   79,   41,   42,
 /*   350 */     6,   87,   88,   89,   30,   91,   49,   50,   51,   52,
 /*   360 */    53,   54,   55,   56,   57,   83,   59,   60,   61,   62,
 /*   370 */    80,   64,   82,  116,   30,   68,   69,   70,   71,  148,
 /*   380 */   123,   74,   32,  152,  159,  160,    6,  156,  157,  158,
 /*   390 */   116,   41,   42,  119,   87,   88,   89,  123,   91,   49,
 /*   400 */    50,   51,   52,   53,   54,   55,   56,   57,   33,   59,
 /*   410 */    60,   61,   62,   33,   64,  159,  160,   33,   68,   69,
 /*   420 */    70,   71,  159,  160,   74,   32,  116,  114,  117,  116,
 /*   430 */   120,  118,  119,  123,   41,   42,  123,   87,   88,   89,
 /*   440 */    32,   91,   49,   50,   51,   52,   53,   54,   55,   56,
 /*   450 */    57,  117,   59,   60,   61,   62,    6,   64,  159,  160,
 /*   460 */    43,   68,   69,   70,   71,  159,  160,   74,   32,   32,
 /*   470 */   114,  115,  116,   43,    6,  119,  117,   41,   42,  123,
 /*   480 */    87,   88,   89,   33,   91,   49,   50,   51,   52,   53,
 /*   490 */    54,   55,   56,   57,   32,   59,   60,   61,   62,    6,
 /*   500 */    64,   33,  159,  160,   68,   69,   70,   71,  148,  121,
 /*   510 */    74,   32,  124,  125,  126,  155,  156,  157,  158,    6,
 /*   520 */    41,   42,   15,   87,   88,   89,   33,   91,   49,   50,
 /*   530 */    51,   52,   53,   54,   55,   56,   57,   32,   59,   60,
 /*   540 */    61,   62,  108,   64,  110,    6,   33,   68,   69,   70,
 /*   550 */    71,  148,   32,   74,   32,  152,  159,  160,    6,  156,
 /*   560 */   157,  158,    6,   41,   42,   45,   87,   88,   89,   30,
 /*   570 */    91,   49,   50,   51,   52,   53,   54,   55,   56,   57,
 /*   580 */    32,   59,   60,   61,   62,   33,   64,  159,  160,   33,
 /*   590 */    68,   69,   70,   71,  148,   35,   74,   32,   47,  153,
 /*   600 */    40,    6,  156,  157,  158,   33,   41,   42,    6,   87,
 /*   610 */    88,   89,   33,   91,   49,   50,   51,   52,   53,   54,
 /*   620 */    55,   56,   57,   32,   59,   60,   61,   62,   33,   64,
 /*   630 */   146,  147,   30,   68,   69,   70,   71,  148,   33,   74,
 /*   640 */    32,  152,  159,  160,    6,  156,  157,  158,   43,   41,
 /*   650 */    42,    6,   87,   88,   89,   32,   91,   49,   50,   51,
 /*   660 */    52,   53,   54,   55,   56,   57,  107,   59,   60,   61,
 /*   670 */    62,   33,   64,  107,    6,   30,   68,   69,   70,   71,
 /*   680 */   148,   35,   74,   32,  152,    6,   40,   32,  156,  157,
 /*   690 */   158,  107,   41,   42,   41,   87,   88,   89,   30,   91,
 /*   700 */    49,   50,   51,   52,   53,   54,   55,   56,   57,   30,
 /*   710 */    59,   60,   61,   62,   32,   64,   47,   40,   33,   68,
 /*   720 */    69,   70,   71,  148,   33,   74,   32,   40,  153,   63,
 /*   730 */    40,  156,  157,  158,  147,   41,   42,   40,   87,   88,
 /*   740 */    89,  117,   91,   49,   50,   51,   52,   53,   54,   55,
 /*   750 */    56,   57,   66,   59,   60,   61,   62,   40,   64,   40,
 /*   760 */    40,   69,   68,   69,   70,   71,  148,   33,   74,   32,
 /*   770 */   152,   32,  161,   30,  156,  157,  158,   82,   41,   42,
 /*   780 */    32,   87,   88,   89,   15,   91,   49,   50,   51,   52,
 /*   790 */    53,   54,   55,   56,   57,   32,   59,   60,   61,   62,
 /*   800 */    47,   64,   32,    6,  161,   68,   69,   70,   71,  148,
 /*   810 */    32,   74,   32,  152,   47,   47,   66,  156,  157,  158,
 /*   820 */    66,   41,   42,   32,   87,   88,   89,   32,   91,   49,
 /*   830 */    50,   51,   52,   53,   54,   55,   56,   57,  150,   59,
 /*   840 */    60,   61,   62,   83,   64,   41,   41,  151,   68,   69,
 /*   850 */    70,   71,  148,  151,   74,   32,  152,  151,  151,  151,
 /*   860 */   156,  157,  158,  151,   41,   42,   32,   87,   88,   89,
 /*   870 */    83,   91,   49,   50,   51,   52,   53,   54,   55,   56,
 /*   880 */    57,  151,   59,   60,   61,   62,  151,   64,   33,  151,
 /*   890 */    83,   68,   69,   70,   71,  148,  151,   74,   32,  152,
 /*   900 */   151,   33,   33,  156,  157,  158,   32,   41,   42,   32,
 /*   910 */    87,   88,   89,   33,   91,   49,   50,   51,   52,   53,
 /*   920 */    54,   55,   56,   57,   83,   59,   60,   61,   62,   33,
 /*   930 */    64,   83,   33,   83,   68,   69,   70,   71,  148,   33,
 /*   940 */    74,   32,  152,   32,  160,   43,  156,  157,  158,  117,
 /*   950 */    41,   42,   32,   87,   88,   89,   32,   91,   49,   50,
 /*   960 */    51,   52,   53,   54,   55,   56,   57,   32,   59,   60,
 /*   970 */    61,   62,   72,   64,   69,   33,   66,   68,   69,   70,
 /*   980 */    71,  148,   32,   74,   32,   63,  153,   40,   40,  156,
 /*   990 */   157,  158,   47,   41,   42,   40,   87,   88,   89,   40,
 /*  1000 */    91,   49,   50,   51,   52,   53,   54,   55,   56,   57,
 /*  1010 */    45,   59,   60,   61,   62,   43,   64,   33,  117,   45,
 /*  1020 */    68,   69,   70,   71,  148,   43,   74,   32,  152,  117,
 /*  1030 */    33,  117,  156,  157,  158,   33,   41,   42,  117,   87,
 /*  1040 */    88,   89,   33,   91,   49,   50,   51,   52,   53,   54,
 /*  1050 */    55,   56,   57,  110,   59,   60,   61,   62,  107,   64,
 /*  1060 */    36,   32,   32,   68,   69,   70,   71,  148,  107,   74,
 /*  1070 */    32,  152,   40,  107,  107,  156,  157,  158,   36,   41,
 /*  1080 */    42,  168,   87,   88,   89,   32,   91,   49,   50,   51,
 /*  1090 */    52,   53,   54,   55,   56,   57,  107,   59,   60,   61,
 /*  1100 */    62,   32,   64,  107,  168,  168,   68,   69,   70,   71,
 /*  1110 */   168,   31,   74,   32,   34,  168,   36,  168,   38,   39,
 /*  1120 */   168,   14,   42,   42,  168,   87,   88,   89,  168,   91,
 /*  1130 */    49,   50,   51,   52,   53,   54,   55,   56,   57,   32,
 /*  1140 */    59,   60,   61,   62,  168,   64,  168,  168,  168,   68,
 /*  1150 */    69,   70,   71,  168,  105,   74,   49,   50,   51,   52,
 /*  1160 */    53,   54,   55,   56,   57,   58,   59,  168,   87,   88,
 /*  1170 */    89,  122,   91,  168,  168,  168,  127,  128,  129,  130,
 /*  1180 */   131,  132,  133,  134,  135,  136,  137,  138,  139,  140,
 /*  1190 */   141,  142,  143,  144,  105,  168,  168,  112,  168,  114,
 /*  1200 */   115,  116,  168,  118,  119,  156,  157,  158,  123,  121,
 /*  1210 */   168,  122,  124,  125,  126,  168,  127,  128,  129,  130,
 /*  1220 */   131,  132,  133,  134,  135,  136,  137,  138,  139,  140,
 /*  1230 */   141,  142,  143,  144,  168,  105,   35,   35,   37,   37,
 /*  1240 */   148,   40,   40,  168,  168,  156,  157,  158,  156,  157,
 /*  1250 */   158,  168,  122,  168,  168,  163,  164,  127,  128,  129,
 /*  1260 */   130,  131,  132,  133,  134,  135,  136,  137,  138,  139,
 /*  1270 */   140,  141,  142,  143,  144,  105,  168,  168,  145,  168,
 /*  1280 */   168,  148,  145,  168,  168,  148,  156,  157,  158,  156,
 /*  1290 */   157,  158,  122,  156,  157,  158,  168,  127,  128,  129,
 /*  1300 */   130,  131,  132,  133,  134,  135,  136,  137,  138,  139,
 /*  1310 */   140,  141,  142,  143,  144,  148,  168,  105,  168,  168,
 /*  1320 */   168,  168,  168,  156,  157,  158,  156,  157,  158,    1,
 /*  1330 */     2,    3,    4,    5,  122,   41,  168,  168,  168,  127,
 /*  1340 */   128,  129,  130,  131,  132,  133,  134,  135,  136,  137,
 /*  1350 */   138,  139,  140,  141,  142,  143,  144,  168,  105,   65,
 /*  1360 */    32,   67,  168,  148,  168,  168,   38,   39,  156,  157,
 /*  1370 */   158,  156,  157,  158,   46,  122,   48,  168,  168,  168,
 /*  1380 */   127,  128,  129,  130,  131,  132,  133,  134,  135,  136,
 /*  1390 */   137,  138,  139,  140,  141,  142,  143,  144,  105,  168,
 /*  1400 */   168,  145,  168,  168,  148,  145,  168,  168,  148,  156,
 /*  1410 */   157,  158,  156,  157,  158,  122,  156,  157,  158,  168,
 /*  1420 */   127,  128,  129,  130,  131,  132,  133,  134,  135,  136,
 /*  1430 */   137,  138,  139,  140,  141,  142,  143,  144,  148,  168,
 /*  1440 */   105,  168,  168,  168,  168,  168,  156,  157,  158,  156,
 /*  1450 */   157,  158,    1,    2,    3,    4,    5,  122,  168,  168,
 /*  1460 */   168,  168,  127,  128,  129,  130,  131,  132,  133,  134,
 /*  1470 */   135,  136,  137,  138,  139,  140,  141,  142,  143,  144,
 /*  1480 */   168,  105,  168,   32,  168,  168,  168,  168,  168,   38,
 /*  1490 */    39,  156,  157,  158,  168,  168,  168,   46,  122,   48,
 /*  1500 */   168,  168,  168,  127,  128,  129,  130,  131,  132,  133,
 /*  1510 */   134,  135,  136,  137,  138,  139,  140,  141,  142,  143,
 /*  1520 */   144,  105,  168,  168,  168,  168,  168,  168,  145,  168,
 /*  1530 */   168,  148,  156,  157,  158,  168,  168,  168,  122,  156,
 /*  1540 */   157,  158,  168,  127,  128,  129,  130,  131,  132,  133,
 /*  1550 */   134,  135,  136,  137,  138,  139,  140,  141,  142,  143,
 /*  1560 */   144,  148,  168,  105,  168,  168,  168,  168,  168,  156,
 /*  1570 */   157,  158,  156,  157,  158,    1,    2,    3,    4,    5,
 /*  1580 */   122,  168,  168,  168,  168,  127,  128,  129,  130,  131,
 /*  1590 */   132,  133,  134,  135,  136,  137,  138,  139,  140,  141,
 /*  1600 */   142,  143,  144,  168,  105,  168,  168,  168,  168,  168,
 /*  1610 */   168,  168,   38,   39,  156,  157,  158,  168,  168,  168,
 /*  1620 */    46,  122,   48,  168,  168,  168,  127,  128,  129,  130,
 /*  1630 */   131,  132,  133,  134,  135,  136,  137,  138,  139,  140,
 /*  1640 */   141,  142,  143,  144,  105,  168,  168,  168,  168,  168,
 /*  1650 */   168,  168,  168,  168,  148,  156,  157,  158,  168,  168,
 /*  1660 */   168,  122,  156,  157,  158,  168,  127,  128,  129,  130,
 /*  1670 */   131,  132,  133,  134,  135,  136,  137,  138,  139,  140,
 /*  1680 */   141,  142,  143,  144,  168,  168,  105,  168,  168,  168,
 /*  1690 */   168,  168,  168,  168,  168,  156,  157,  158,    1,    2,
 /*  1700 */     3,    4,    5,  122,  168,  168,  168,  168,  127,  128,
 /*  1710 */   129,  130,  131,  132,  133,  134,  135,  136,  137,  138,
 /*  1720 */   139,  140,  141,  142,  143,  144,  168,  105,  168,  168,
 /*  1730 */   168,  168,  168,  168,  168,   38,   39,  156,  157,  158,
 /*  1740 */   168,  168,  168,   46,  122,   48,  168,  168,  168,  127,
 /*  1750 */   128,  129,  130,  131,  132,  133,  134,  135,  136,  137,
 /*  1760 */   138,  139,  140,  141,  142,  143,  144,  105,  168,  168,
 /*  1770 */   168,  168,  168,  168,  168,  168,  168,  148,  156,  157,
 /*  1780 */   158,  168,  168,  168,  122,  156,  157,  158,  168,  127,
 /*  1790 */   128,  129,  130,  131,  132,  133,  134,  135,  136,  137,
 /*  1800 */   138,  139,  140,  141,  142,  143,  144,  168,  168,  105,
 /*  1810 */   168,  168,  168,  148,  168,  168,  148,  168,  156,  157,
 /*  1820 */   158,  156,  157,  158,  156,  157,  158,  168,  168,  168,
 /*  1830 */   168,  127,  128,  129,  130,  131,  132,  133,  134,  135,
 /*  1840 */   136,  137,  138,  139,  140,  141,  142,  143,  144,    7,
 /*  1850 */     8,  168,  168,  168,  168,  168,   14,  148,  168,  168,
 /*  1860 */   156,  157,  158,  168,  168,  156,  157,  158,   26,   27,
 /*  1870 */    28,   29,  163,  164,   32,   33,    7,    8,  168,  168,
 /*  1880 */   168,  168,  168,   14,  148,  168,  168,   45,  168,   47,
 /*  1890 */   168,  168,  156,  157,  158,   26,   27,   28,   29,  168,
 /*  1900 */   148,   32,  168,  167,  168,  168,    7,    8,  156,  157,
 /*  1910 */   158,  168,  148,   14,   45,  168,   47,  165,  166,  167,
 /*  1920 */   156,  157,  158,   81,   82,   26,   27,   28,   29,   30,
 /*  1930 */   168,   32,   90,  168,   92,   93,   94,   95,   96,  148,
 /*  1940 */    43,  168,   73,  168,   45,  168,   47,  156,  157,  158,
 /*  1950 */    81,   82,  168,  162,  168,  168,  165,  166,  167,   90,
 /*  1960 */   168,   92,   93,   94,   95,   96,  168,  168,  168,  168,
 /*  1970 */   168,  168,   75,   76,   77,   78,   79,  168,  168,   82,
 /*  1980 */    81,   82,   85,   86,  168,  168,  168,  168,  168,   90,
 /*  1990 */   168,   92,   93,   94,   95,   96,  168,    7,    8,  148,
 /*  2000 */   168,  168,  168,  168,   14,  168,  168,  156,  157,  158,
 /*  2010 */   168,  168,  168,  168,  163,  164,   26,   27,   28,   29,
 /*  2020 */   148,  168,   32,  168,    7,    8,  168,  168,  156,  157,
 /*  2030 */   158,   14,  168,  168,  168,   45,  164,   47,  168,  168,
 /*  2040 */   168,  168,   47,   26,   27,   28,   29,   30,  168,   32,
 /*  2050 */   148,  168,   14,  168,  168,  168,  168,  168,  156,  157,
 /*  2060 */   158,   66,   45,  168,   47,  163,  164,  168,   30,  168,
 /*  2070 */    32,   81,   82,   83,  168,   80,  168,   82,  168,   84,
 /*  2080 */    90,  168,   92,   93,   94,   95,   96,   49,   50,   51,
 /*  2090 */    52,   53,   54,   55,   56,   57,   58,   59,   81,   82,
 /*  2100 */   125,    1,    2,    3,    4,    5,  168,   90,  168,   92,
 /*  2110 */    93,   94,   95,   96,  168,    7,    8,  168,  168,  168,
 /*  2120 */   168,  168,   14,  148,  168,  168,  168,  168,  168,  168,
 /*  2130 */   168,  156,  157,  158,   26,   27,   28,   29,   38,   39,
 /*  2140 */    32,  168,    7,    8,  168,  168,   46,  168,   48,   14,
 /*  2150 */   168,  168,  168,   45,  168,   47,  168,  168,  168,  168,
 /*  2160 */   168,   26,   27,   28,   29,  168,  148,   32,  168,  168,
 /*  2170 */   168,  168,    7,    8,  156,  157,  158,  148,  168,   14,
 /*  2180 */    45,   73,   47,  168,  168,  156,  157,  158,  168,   81,
 /*  2190 */    82,   26,   27,   28,   29,   30,  168,   32,   90,  168,
 /*  2200 */    92,   93,   94,   95,   96,    1,    2,    3,    4,    5,
 /*  2210 */    45,  148,   47,  168,  148,  168,   81,   82,   83,  156,
 /*  2220 */   157,  158,  156,  157,  158,   90,  148,   92,   93,   94,
 /*  2230 */    95,   96,  168,  168,  156,  157,  158,  168,  168,  168,
 /*  2240 */   168,  168,   38,   39,  168,  168,   81,   82,  168,  168,
 /*  2250 */    46,  168,   48,  168,  168,   90,  168,   92,   93,   94,
 /*  2260 */    95,   96,  168,    7,    8,  148,  168,  168,  168,  168,
 /*  2270 */    14,  148,  168,  156,  157,  158,  168,  168,  168,  156,
 /*  2280 */   157,  158,   26,   27,   28,   29,  148,  168,   32,  168,
 /*  2290 */     7,    8,  168,  168,  156,  157,  158,   14,  148,  168,
 /*  2300 */   168,   45,  168,   47,  168,  168,  156,  157,  158,   26,
 /*  2310 */    27,   28,   29,   30,  168,   32,  148,  168,   14,  168,
 /*  2320 */   168,  168,  168,  148,  156,  157,  158,  168,   45,  168,
 /*  2330 */    47,  156,  157,  158,   30,  148,   32,   81,   82,   83,
 /*  2340 */   168,  168,  168,  156,  157,  158,   90,  168,   92,   93,
 /*  2350 */    94,   95,   96,   49,   50,   51,   52,   53,   54,   55,
 /*  2360 */    56,   57,   58,   59,   81,   82,  168,    1,    2,    3,
 /*  2370 */     4,    5,  168,   90,  168,   92,   93,   94,   95,   96,
 /*  2380 */   168,    7,    8,  148,  168,  168,  168,  168,   14,  168,
 /*  2390 */   168,  156,  157,  158,  168,  168,  168,  168,  168,  168,
 /*  2400 */    26,   27,   28,   29,   38,   39,   32,  168,    7,    8,
 /*  2410 */   168,  168,  168,  168,   48,   14,  168,  168,  168,   45,
 /*  2420 */   168,   47,  168,  168,  168,  168,  168,   26,   27,   28,
 /*  2430 */    29,  168,  148,   32,  168,  148,   14,  168,  168,  168,
 /*  2440 */   156,  157,  158,  156,  157,  158,   45,  168,   47,  168,
 /*  2450 */   148,  168,   30,  168,   32,   81,   82,   83,  156,  157,
 /*  2460 */   158,  168,  168,  168,   90,  168,   92,   93,   94,   95,
 /*  2470 */    96,   49,   50,   51,   52,   53,   54,   55,   56,   57,
 /*  2480 */    58,   59,   81,   82,  168,  168,  168,  168,  168,  168,
 /*  2490 */   168,   90,  168,   92,   93,   94,   95,   96,  168,  168,
 /*  2500 */   168,    9,   10,   11,   12,   13,   14,   15,   16,   17,
 /*  2510 */    18,   19,   20,   21,   22,   23,   24,   25,  168,  168,
 /*  2520 */   168,  168,   30,    9,   10,   11,   12,   13,   14,   15,
 /*  2530 */    16,   17,   18,   19,   20,   21,   22,   23,   24,   25,
 /*  2540 */    12,   13,   14,   15,   16,   17,   18,   19,   20,   21,
 /*  2550 */    22,   23,   24,   25,   40,   43,  168,  168,  168,  168,
 /*  2560 */   168,  168,  168,  168,    9,   10,   11,   12,   13,   14,
 /*  2570 */    15,   16,   17,   18,   19,   20,   21,   22,   23,   24,
 /*  2580 */    25,  168,  168,  168,  168,  168,  168,   75,   76,   77,
 /*  2590 */    78,   79,   80,    0,   82,   40,   84,   85,   86,  168,
 /*  2600 */   168,  168,  168,  168,  168,    9,   10,   11,   12,   13,
 /*  2610 */    14,   15,   16,   17,   18,   19,   20,   21,   22,   23,
 /*  2620 */    24,   25,  168,  168,   31,  168,  168,   34,  168,   36,
 /*  2630 */   148,   38,   39,  168,  168,   42,   40,  168,  156,  157,
 /*  2640 */   158,  168,  168,  168,  168,  168,    9,   10,   11,   12,
 /*  2650 */    13,   14,   15,   16,   17,   18,   19,   20,   21,   22,
 /*  2660 */    23,   24,   25,    9,   10,   11,   12,   13,   14,   15,
 /*  2670 */    16,   17,   18,   19,   20,   21,   22,   23,   24,   25,
 /*  2680 */   168,    7,    8,  148,  168,  168,  148,   33,   14,  148,
 /*  2690 */   168,  156,  157,  158,  156,  157,  158,  156,  157,  158,
 /*  2700 */    26,   27,   28,   29,  148,  168,   32,  168,  168,  168,
 /*  2710 */   168,  168,  156,  157,  158,  168,  168,  168,  168,   45,
 /*  2720 */    83,   47,  168,  168,  168,  168,  168,  168,  168,    9,
 /*  2730 */    10,   11,   12,   13,   14,   15,   16,   17,   18,   19,
 /*  2740 */    20,   21,   22,   23,   24,   25,  168,  168,  168,  168,
 /*  2750 */   168,  168,  168,  168,  168,   81,   82,  168,  168,  168,
 /*  2760 */   168,  168,  168,  168,   90,  168,   92,   93,   94,   95,
 /*  2770 */    96,  168,  168,  168,    9,   10,   11,   12,   13,   14,
 /*  2780 */    15,   16,   17,   18,   19,   20,   21,   22,   23,   24,
 /*  2790 */    25,    1,    2,    3,    4,    5,  168,  168,   33,  168,
 /*  2800 */   168,  168,  168,   83,  168,  168,  168,  168,    9,   10,
 /*  2810 */    11,   12,   13,   14,   15,   16,   17,   18,   19,   20,
 /*  2820 */    21,   22,   23,   24,   25,  168,  168,  168,   38,   39,
 /*  2830 */   168,   41,   42,  168,   44,  168,  168,  168,   48,   40,
 /*  2840 */     1,    2,    3,    4,    5,  168,  168,  168,  168,    9,
 /*  2850 */    10,   11,   12,   13,   14,   15,   16,   17,   18,   19,
 /*  2860 */    20,   21,   22,   23,   24,   25,   14,  168,  168,  168,
 /*  2870 */   168,  168,  168,  168,  168,  168,  168,   38,   39,  168,
 /*  2880 */    41,   42,   30,   44,   32,  168,  168,   48,  168,    1,
 /*  2890 */     2,    3,    4,    5,  168,  168,  168,  168,  168,  168,
 /*  2900 */   168,   49,   50,   51,   52,   53,   54,   55,   56,   57,
 /*  2910 */    58,   59,    1,    2,    3,    4,    5,  168,  168,  168,
 /*  2920 */   168,  168,  168,  168,  168,  168,   38,   39,  168,   41,
 /*  2930 */    42,  168,   44,  168,  168,  168,   48,  168,    1,    2,
 /*  2940 */     3,    4,    5,    1,    2,    3,    4,    5,  168,   38,
 /*  2950 */    39,  168,   41,   42,  168,   44,  168,  168,  168,   48,
 /*  2960 */   168,    1,    2,    3,    4,    5,    1,    2,    3,    4,
 /*  2970 */     5,  168,  168,  168,  168,   38,   39,  168,   41,   42,
 /*  2980 */    38,   39,  168,   41,   42,   48,  168,  168,  168,  168,
 /*  2990 */    48,  168,    1,    2,    3,    4,    5,  168,   38,   39,
 /*  3000 */   168,   41,   42,   38,   39,  168,   41,   42,   48,  168,
 /*  3010 */   168,  168,  168,   48,  168,    1,    2,    3,    4,    5,
 /*  3020 */     1,    2,    3,    4,    5,  168,  168,  168,  168,   38,
 /*  3030 */    39,  168,   41,   42,    1,    2,    3,    4,    5,   48,
 /*  3040 */   168,  168,  168,  168,  168,  168,  168,  168,  168,  168,
 /*  3050 */   168,  168,   38,   39,  168,   41,   42,   38,   39,  168,
 /*  3060 */    41,   42,   48,  168,  168,  168,  168,   48,  168,  168,
 /*  3070 */   168,   38,   39,  168,   41,   42,  168,  168,  168,  168,
 /*  3080 */   168,   48,
};
#define YY_SHIFT_USE_DFLT (-26)
static short yy_shift_ofst[] = {
 /*     0 */  1080,  126, 2593,  -26,  -26,  -26,  -26,  -26,  100,  139,
 /*    10 */   -26,  175,  560,  -26,  204,  210,  -26, 3033,  -26, 2942,
 /*    20 */   -26,  -26, 1697,  275,  289, 2304,  375,  -26,  679,  384,
 /*    30 */   -26, 1107,  -26,  279,  121,  -26,  -26,  -26,  -26,  -26,
 /*    40 */   -26,  -26,  -26,  408,  417,  121,  -26,  437,  430,  121,
 /*    50 */   -26,  462,  507,  505,  -26,  -26,  -26,  -26,  -26,  -26,
 /*    60 */   -26,  -26,  -26,  -26,  -26,  -26,  -26,  -26,  -26,  -26,
 /*    70 */   -26,  -26,  -26,  -26,  -26, 2366, 1574,  548,  551, 2422,
 /*    80 */   572,  -26,  152,  579,  -26,  -26,  -26,  591, 1201,  -26,
 /*    90 */   623,  161,  -26,  655,  125,  -26, 2839,  -26,  653,  -26,
 /*   100 */  2888, 2965,  -26, 2204,  682,  669, 2852,  677,  694,  -26,
 /*   110 */   909,  -26,  -26,  -26,  -26,  -26,  -26,  -26,  -26,  -26,
 /*   120 */   -26,  -26,  -26,  -26,  -26,  -26,  -26,  -26,  -26,  -26,
 /*   130 */   -26,  685,  -26,  691,  -26, 2401,  687,  135,   92,  666,
 /*   140 */   690,  780,  608,  -26, 2401,  697,  262,  -26, 1294,  -26,
 /*   150 */   -26,  121,  686, 1081, 1081,  717,  651,  436,  -26, 2401,
 /*   160 */   719,  565,  393,  -26,  720,  264,  221,  692, 2401,  734,
 /*   170 */   -26, 2401,  -26, 2401,  739,  -26,  695, 2401, 2401, 2840,
 /*   180 */  2401,  277, 2401,  277, 2401,  277, 2401,  277, 2401,  277,
 /*   190 */  2401,  277, 2401,  277, 2401,   11, 2401,   11, 2401,  -26,
 /*   200 */  2401,  -26, 2401,  -26, 2401,   11, 2401, 2528, 2401, 2528,
 /*   210 */  2401, 2840,   -7, 2492,  -26,  743, 2401,  -26,  748,  769,
 /*   220 */  2401,  277,   75,  763,  753, 2017, 2840,  770,  797,  739,
 /*   230 */   -26, 2401, 2840,  -26,  -26,  -26,  -26,  -26,  -26,  -26,
 /*   240 */   -26, 2374, 2840, 1995,  520,  767, 2283,  778,  768, 1899,
 /*   250 */   -26,  602,  -26, 2401,  -26,  -26,  -26,  -26,  -26,  539,
 /*   260 */   -26,  -26,  -26, 2401, 2637,  -26, 2165,  344,  -26,  -26,
 /*   270 */   750,  754,  -26,  282,  -26, 2674,  -26,  -25, 2401,  -26,
 /*   280 */   -26,  -26,  324,  -26,  -26, 2720,  -26,  277, 2840,  791,
 /*   290 */    78, 1869, 2799, 1081,  823,  -26,  795,  235,  -26,  795,
 /*   300 */   -26, 2512,  -26,  -26,  -26,  -26,  -26,  -26, 2401,  -26,
 /*   310 */  2840,   -1, 1897, 2401,  -26, 1990,  268, 2401,  -26,  760,
 /*   320 */   268, 2401,  -26,   44,  -26,  -26,   83,  804,  268, 2401,
 /*   330 */   -26,  805,  268, 2401,  -26,  834,  182, 2401,  -26, 2256,
 /*   340 */   268, 2401,  -26,  787,  268, 2401,  -26, 2135,  268, 2401,
 /*   350 */   -26,  807,  268, 2401,  -26,  -26,  -26,  -26, 2401,  298,
 /*   360 */   -26, 2401,  -26, 2840,  -26,  855,  -26,  868,  -26,  869,
 /*   370 */   -26, 1842, 2765,  -26,  -26,  874,  290,  877,  880,  -26,
 /*   380 */    -8,  841,  896,  -26,  848,  899,  -26,  850,  906,  -26,
 /*   390 */  2401, 2654,  -26,  911,  380,  -26,  911,  -26,  902,  121,
 /*   400 */   -26,  -26,  911,  450,  -26,  911,  468,  -26,  911,  493,
 /*   410 */   -26,  911,  513,  -26,  911,  552,  -26,  911,  556,  -26,
 /*   420 */   911,  595,  -26,  911,  212,  -26,  911,  638,  -26,  128,
 /*   430 */   920,  767,  924,  753,  -26,  -26, 2401, 2596, 1081,  737,
 /*   440 */   -26,  935,  900, 2108, 2555, 1081,   49,  -26, 2401, 2514,
 /*   450 */  1081,  952,  -26,  905, 2401,  942,  -26,  -26,  -26,  910,
 /*   460 */  1081, 1081,  -26,  -26,  922,  947,  479,  350,  -26,  -26,
 /*   470 */   645,  948,  522,  -26,  866,  -26, 2366, 2100,  950,  945,
 /*   480 */  2038,  955,  995,  -26,  178,  -26,  668,  959,  307,  -26,
 /*   490 */  1038,  -26, 2911, 2960,  -26,  -26,   91,  965,  972,  121,
 /*   500 */   984,  -26,  974,  982,  121,  997,  -26,  -26,   91, 1328,
 /*   510 */   605,  -26,  121, 1002,  -26, 1451,   64,  -26,  121, 1009,
 /*   520 */   -26,  -26, 2790, 2991, 3014, 2366, 3019,  -26, 2937,  655,
 /*   530 */   -26,  -26,  -26,  655,  125,  -26, 1024, 1029, 1202,  -26,
 /*   540 */  1030, 1032,  -26,  655,  125,  -26, 1042, 1053,  646,  -26,
 /*   550 */  1069, 1032,  -26,  -26,
};
#define YY_REDUCE_USE_DFLT (-109)
static short yy_reduce_ofst[] = {
 /*     0 */   -90, -109,  -99, -109, -109, -109, -109, -109, -109, -109,
 /*    10 */  -109, -109,  115, -109, -109,  173, -109,    5, -109,  310,
 /*    20 */  -109, -109,  170, -109, -109, 1088, -109, -109, -109, -109,
 /*    30 */  -109, -108, -109, -109,  311, -109, -109, -109, -109, -109,
 /*    40 */  -109, -109, -109, -109, -109,  334, -109, -109, -109,  359,
 /*    50 */  -109, -109, -109, -109, -109, -109, -109, -109, -109, -109,
 /*    60 */  -109, -109, -109, -109, -109, -109, -109, -109, -109, -109,
 /*    70 */  -109, -109, -109, -109, -109,  171,  170, -109, -109,  130,
 /*    80 */  -109, -109, -109, -109, -109, -109, -109, -109,  559, -109,
 /*    90 */  -109,  566, -109,    4,  584, -109,  -86, -109, -109, -109,
 /*   100 */    55,  274, -109,  170, -109, -109,  216, -109, 1170, -109,
 /*   110 */  1704, -109, -109, -109, -109, -109, -109, -109, -109, -109,
 /*   120 */  -109, -109, -109, -109, -109, -109, -109, -109, -109, -109,
 /*   130 */  -109, -109, -109, -109, -109, 1137, -109, 1089, 1704, -109,
 /*   140 */  -109, 1335, 1704, -109, 1256, -109,  484, -109,  587, -109,
 /*   150 */  -109,  624, -109, 1293, 1704, -109, 1581, 1704, -109, 1260,
 /*   160 */  -109, 1622, 1704, -109, -109, 1049, 1704, -109, 1383, -109,
 /*   170 */  -109, 2302, -109, 2168,  611, -109, -109, 2138, 2284, -109,
 /*   180 */  2078, -109, 2066, -109, 2018, -109, 1764, -109, 1668, -109,
 /*   190 */  1629, -109, 1506, -109, 1413, -109, 1167, -109, 2556, -109,
 /*   200 */  2535, -109, 2482, -109, 2538, -109, 1290, -109, 1665, -109,
 /*   210 */  2029, -109, 1975, -109, -109, -109, 2541, -109, -109, -109,
 /*   220 */  2123, -109, -109, -109, -109, 1709, -109, -109, -109,  643,
 /*   230 */  -109, 1215, -109, -109, -109, -109, -109, -109, -109, -109,
 /*   240 */  -109, 1791, -109, -109, -109, -109, 1092, -109, -109, 1902,
 /*   250 */  -109, -109, -109, 1872, -109, -109, -109, -109, -109, -109,
 /*   260 */  -109, -109, -109, 2187, -109, -109, 1851, -109, -109, -109,
 /*   270 */  -109, -109, -109, -109, -109, 1752, -109, -109, 1736, -109,
 /*   280 */  -109, -109, -109, -109, -109, -109, -109, -109, -109, -109,
 /*   290 */  -109, 2287, -109, 1499, 1704, -109,   94, -109, -109,  688,
 /*   300 */  -109,  696, -109, -109, -109, -109, -109, -109,  919, -109,
 /*   310 */  -109, -109,  702,  876, -109,  833,  706,  790, -109, -109,
 /*   320 */   707,  747, -109, -109, -109, -109, -109, -109,  708,  704,
 /*   330 */  -109, -109,  712,  661, -109, -109,  730,  618, -109,  575,
 /*   340 */   735,  532, -109, -109,  738,  489, -109,  446,  745,  403,
 /*   350 */  -109, -109,  749,  231, -109, -109, -109, -109,  -78, -109,
 /*   360 */  -109,  360, -109, -109, -109, -109, -109, -109, -109, -109,
 /*   370 */  -109, 2235, -109, -109, -109, -109, -109, -109, -109, -109,
 /*   380 */  -109, -109, -109, -109, -109, -109, -109, -109, -109, -109,
 /*   390 */  2063, -109, -109,  -88, -109, -109,  784, -109, -109,  832,
 /*   400 */  -109, -109,  225, -109, -109,  256, -109, -109,  263, -109,
 /*   410 */  -109,  299, -109, -109,  306, -109, -109,  343, -109, -109,
 /*   420 */   397, -109, -109,  428, -109, -109,  483, -109, -109, -109,
 /*   430 */  -109, -109, -109, -109, -109, -109, 2117, -109, 1539, 1704,
 /*   440 */  -109, -109, -109, 2150, -109, 1662, 1704, -109, 2175, -109,
 /*   450 */  1416, 1704, -109, -109, 1133, -109, -109, -109, -109, -109,
 /*   460 */  1212, 1704, -109, -109, -109, -109, 1253, 1704, -109, -109,
 /*   470 */  -109, -109, 1376, -109, 1704, -109,  257,  170, -109, -109,
 /*   480 */   388, -109, 1130, -109, 1704, -109, -109, -109, 1458, -109,
 /*   490 */  1704, -109,  313,  274, -109, -109,  257, -109, -109,  901,
 /*   500 */  -109, -109, -109, -109,  912, -109, -109, -109,   77,  170,
 /*   510 */  -109, -109,  914, -109, -109,  170, -109, -109,  921, -109,
 /*   520 */  -109, -109, 1085,  356,  274,   77,  274, -109,  274,  943,
 /*   530 */  -109, -109, -109,  434,  951, -109, -109, -109,  961, -109,
 /*   540 */  -109,  966, -109,  176,  967, -109, -109, -109,  989, -109,
 /*   550 */  -109,  996, -109, -109,
};
static YYACTIONTYPE yy_default[] = {
 /*     0 */   818,  818,  818,  556,  558,  559,  560,  561,  818,  818,
 /*    10 */   562,  818,  818,  563,  818,  818,  564,  818,  579,  818,
 /*    20 */   580,  600,  818,  818,  818,  818,  818,  612,  818,  818,
 /*    30 */   613,  818,  626,  628,  818,  631,  808,  809,  810,  811,
 /*    40 */   812,  813,  814,  818,  629,  818,  632,  818,  630,  818,
 /*    50 */   633,  818,  818,  818,  635,  636,  637,  638,  639,  640,
 /*    60 */   641,  642,  643,  644,  645,  627,  616,  618,  619,  620,
 /*    70 */   621,  622,  623,  624,  625,  818,  818,  818,  818,  818,
 /*    80 */   818,  614,  818,  818,  615,  617,  601,  818,  818,  565,
 /*    90 */   818,  818,  566,  818,  818,  568,  818,  574,  818,  575,
 /*   100 */   818,  818,  598,  818,  818,  818,  818,  818,  818,  604,
 /*   110 */   818,  606,  646,  648,  649,  650,  651,  652,  653,  654,
 /*   120 */   655,  656,  657,  658,  659,  660,  661,  662,  663,  664,
 /*   130 */   665,  818,  666,  818,  667,  818,  818,  818,  818,  670,
 /*   140 */   818,  818,  818,  671,  818,  818,  818,  674,  818,  675,
 /*   150 */   676,  818,  818,  678,  679,  818,  818,  818,  682,  818,
 /*   160 */   818,  818,  818,  684,  818,  818,  818,  818,  818,  818,
 /*   170 */   686,  818,  745,  818,  818,  746,  818,  818,  818,  747,
 /*   180 */   818,  749,  818,  750,  818,  751,  818,  752,  818,  753,
 /*   190 */   818,  754,  818,  755,  818,  763,  818,  764,  818,  765,
 /*   200 */   818,  766,  818,  767,  818,  768,  818,  769,  818,  770,
 /*   210 */   818,  771,  818,  818,  756,  818,  818,  757,  818,  818,
 /*   220 */   818,  758,  775,  818,  759,  818,  799,  818,  818,  818,
 /*   230 */   772,  818,  773,  774,  776,  777,  778,  779,  780,  781,
 /*   240 */   782,  818,  807,  775,  818,  760,  818,  818,  785,  818,
 /*   250 */   786,  818,  787,  818,  794,  795,  796,  797,  798,  818,
 /*   260 */   790,  791,  761,  818,  818,  762,  818,  818,  788,  789,
 /*   270 */   776,  777,  783,  818,  784,  818,  800,  818,  818,  802,
 /*   280 */   803,  801,  818,  792,  793,  818,  815,  748,  816,  818,
 /*   290 */   818,  818,  818,  818,  818,  687,  818,  818,  691,  818,
 /*   300 */   692,  818,  694,  695,  696,  697,  698,  699,  818,  700,
 /*   310 */   744,  818,  818,  818,  701,  818,  818,  818,  704,  818,
 /*   320 */   818,  818,  705,  818,  711,  712,  818,  818,  818,  818,
 /*   330 */   702,  818,  818,  818,  703,  818,  818,  818,  706,  818,
 /*   340 */   818,  818,  707,  818,  818,  818,  708,  818,  818,  818,
 /*   350 */   709,  818,  818,  818,  710,  713,  714,  693,  818,  818,
 /*   360 */   716,  818,  717,  719,  718,  818,  720,  818,  721,  818,
 /*   370 */   722,  818,  818,  723,  724,  818,  818,  818,  818,  725,
 /*   380 */   818,  818,  818,  726,  818,  818,  727,  818,  818,  728,
 /*   390 */   818,  818,  729,  818,  818,  730,  818,  740,  742,  818,
 /*   400 */   743,  741,  818,  818,  731,  818,  818,  732,  818,  818,
 /*   410 */   733,  818,  818,  734,  818,  818,  735,  818,  818,  736,
 /*   420 */   818,  818,  737,  818,  818,  738,  818,  818,  739,  818,
 /*   430 */   818,  818,  818,  818,  817,  647,  818,  818,  818,  818,
 /*   440 */   688,  818,  818,  818,  818,  818,  818,  689,  818,  818,
 /*   450 */   818,  818,  690,  818,  818,  818,  685,  683,  681,  818,
 /*   460 */   818,  680,  677,  672,  668,  818,  818,  818,  673,  669,
 /*   470 */   818,  818,  818,  605,  818,  607,  818,  818,  818,  818,
 /*   480 */   818,  818,  818,  608,  818,  610,  818,  818,  818,  609,
 /*   490 */   818,  611,  818,  818,  596,  599,  818,  818,  818,  818,
 /*   500 */   818,  602,  818,  818,  818,  818,  603,  590,  818,  818,
 /*   510 */   818,  592,  818,  818,  594,  818,  818,  593,  818,  818,
 /*   520 */   595,  597,  818,  818,  818,  818,  818,  591,  818,  818,
 /*   530 */   576,  578,  577,  818,  818,  567,  818,  818,  818,  569,
 /*   540 */   818,  818,  570,  818,  818,  571,  818,  818,  818,  572,
 /*   550 */   818,  818,  573,  557,
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
  "TYPEOF",        "INSTANCEOF",    "AND",           "OR",          
  "EQUALS",        "IDENTICAL",     "LESS",          "GREATER",     
  "LESSEQUAL",     "GREATEREQUAL",  "NOTIDENTICAL",  "NOTEQUALS",   
  "ADD",           "SUB",           "CONCAT",        "MUL",         
  "DIV",           "MOD",           "ISSET",         "FETCH",       
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
  "ARROW",         "STRING",        "SBRACKET_OPEN",  "SBRACKET_CLOSE",
  "DOUBLECOLON",   "INCR",          "DECR",          "ECHO",        
  "RETURN",        "UNSET",         "INTEGER",       "THROW",       
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
 /* 172 */ "xx_unset_statement ::= UNSET IDENTIFIER SBRACKET_OPEN IDENTIFIER SBRACKET_CLOSE DOTCOMMA",
 /* 173 */ "xx_unset_statement ::= UNSET IDENTIFIER SBRACKET_OPEN INTEGER SBRACKET_CLOSE DOTCOMMA",
 /* 174 */ "xx_unset_statement ::= UNSET IDENTIFIER SBRACKET_OPEN STRING SBRACKET_CLOSE DOTCOMMA",
 /* 175 */ "xx_throw_statement ::= THROW xx_common_expr DOTCOMMA",
 /* 176 */ "xx_declare_statement ::= TYPE_INTEGER xx_declare_variable_list DOTCOMMA",
 /* 177 */ "xx_declare_statement ::= TYPE_UINTEGER xx_declare_variable_list DOTCOMMA",
 /* 178 */ "xx_declare_statement ::= TYPE_CHAR xx_declare_variable_list DOTCOMMA",
 /* 179 */ "xx_declare_statement ::= TYPE_UCHAR xx_declare_variable_list DOTCOMMA",
 /* 180 */ "xx_declare_statement ::= TYPE_LONG xx_declare_variable_list DOTCOMMA",
 /* 181 */ "xx_declare_statement ::= TYPE_ULONG xx_declare_variable_list DOTCOMMA",
 /* 182 */ "xx_declare_statement ::= TYPE_DOUBLE xx_declare_variable_list DOTCOMMA",
 /* 183 */ "xx_declare_statement ::= TYPE_STRING xx_declare_variable_list DOTCOMMA",
 /* 184 */ "xx_declare_statement ::= TYPE_BOOL xx_declare_variable_list DOTCOMMA",
 /* 185 */ "xx_declare_statement ::= TYPE_VAR xx_declare_variable_list DOTCOMMA",
 /* 186 */ "xx_declare_variable_list ::= xx_declare_variable_list COMMA xx_declare_variable",
 /* 187 */ "xx_declare_variable_list ::= xx_declare_variable",
 /* 188 */ "xx_declare_variable ::= IDENTIFIER",
 /* 189 */ "xx_declare_variable ::= IDENTIFIER ASSIGN xx_literal_expr",
 /* 190 */ "xx_assign_expr ::= xx_common_expr",
 /* 191 */ "xx_common_expr ::= NOT xx_common_expr",
 /* 192 */ "xx_common_expr ::= ISSET xx_isset_expr",
 /* 193 */ "xx_common_expr ::= REQUIRE xx_common_expr",
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
 /* 240 */ "xx_common_expr ::= xx_mcall_expr",
 /* 241 */ "xx_common_expr ::= xx_scall_expr",
 /* 242 */ "xx_common_expr ::= xx_fcall_expr",
 /* 243 */ "xx_call_parameters ::= xx_call_parameters COMMA xx_call_parameter",
 /* 244 */ "xx_call_parameters ::= xx_call_parameter",
 /* 245 */ "xx_call_parameter ::= xx_common_expr",
 /* 246 */ "xx_array_list ::= xx_array_list COMMA xx_array_item",
 /* 247 */ "xx_array_list ::= xx_array_item",
 /* 248 */ "xx_array_item ::= xx_array_key COLON xx_array_value",
 /* 249 */ "xx_array_item ::= xx_array_value",
 /* 250 */ "xx_array_key ::= IDENTIFIER",
 /* 251 */ "xx_array_key ::= STRING",
 /* 252 */ "xx_array_key ::= INTEGER",
 /* 253 */ "xx_array_value ::= xx_common_expr",
 /* 254 */ "xx_literal_expr ::= INTEGER",
 /* 255 */ "xx_literal_expr ::= CHAR",
 /* 256 */ "xx_literal_expr ::= STRING",
 /* 257 */ "xx_literal_expr ::= DOUBLE",
 /* 258 */ "xx_literal_expr ::= NULL",
 /* 259 */ "xx_literal_expr ::= FALSE",
 /* 260 */ "xx_literal_expr ::= TRUE",
 /* 261 */ "xx_isset_expr ::= IDENTIFIER SBRACKET_OPEN xx_common_expr SBRACKET_CLOSE",
 /* 262 */ "xx_eval_expr ::= xx_common_expr",
 /* 263 */ "xx_comment ::= COMMENT",
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
// 2299 "parser.c"
      break;
    case 99:
// 910 "parser.lemon"
{ json_object_put((yypminor->yy175)); }
// 2304 "parser.c"
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
  { 103, 7 },
  { 103, 4 },
  { 103, 6 },
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
  { 150, 7 },
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
  { 148, 3 },
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
// 2785 "parser.c"
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
      case 190:
      case 240:
      case 241:
      case 242:
      case 245:
      case 253:
      case 262:
// 912 "parser.lemon"
{
	yygotominor.yy175 = yymsp[0].minor.yy175;
}
// 2822 "parser.c"
        break;
      case 2:
      case 36:
      case 42:
      case 44:
      case 46:
      case 62:
      case 92:
      case 122:
// 916 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_list(yymsp[-1].minor.yy175, yymsp[0].minor.yy175);
}
// 2836 "parser.c"
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
      case 187:
      case 244:
      case 247:
// 920 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_list(NULL, yymsp[0].minor.yy175);
}
// 2857 "parser.c"
        break;
      case 8:
// 940 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_namespace(yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(31,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 2866 "parser.c"
        break;
      case 9:
// 944 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_interface(yymsp[-1].minor.yy0, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(34,&yymsp[-2].minor);
}
// 2874 "parser.c"
        break;
      case 10:
// 948 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_interface(yymsp[-3].minor.yy0, yymsp[0].minor.yy175, yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(34,&yymsp[-4].minor);
  yy_destructor(35,&yymsp[-2].minor);
}
// 2883 "parser.c"
        break;
      case 11:
// 952 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class(yymsp[-1].minor.yy0, yymsp[0].minor.yy175, 0, 0, NULL, NULL, status->scanner_state);
  yy_destructor(36,&yymsp[-2].minor);
}
// 2891 "parser.c"
        break;
      case 12:
// 956 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy175, 0, 0, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(36,&yymsp[-4].minor);
  yy_destructor(35,&yymsp[-2].minor);
}
// 2900 "parser.c"
        break;
      case 13:
// 960 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy175, 0, 0, NULL, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(36,&yymsp[-4].minor);
  yy_destructor(37,&yymsp[-2].minor);
}
// 2909 "parser.c"
        break;
      case 14:
// 964 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class(yymsp[-5].minor.yy0, yymsp[0].minor.yy175, 0, 0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(36,&yymsp[-6].minor);
  yy_destructor(35,&yymsp[-4].minor);
  yy_destructor(37,&yymsp[-2].minor);
}
// 2919 "parser.c"
        break;
      case 15:
// 968 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class(yymsp[-1].minor.yy0, yymsp[0].minor.yy175, 1, 0, NULL, NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-3].minor);
  yy_destructor(36,&yymsp[-2].minor);
}
// 2928 "parser.c"
        break;
      case 16:
// 972 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy175, 1, 0, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-5].minor);
  yy_destructor(36,&yymsp[-4].minor);
  yy_destructor(35,&yymsp[-2].minor);
}
// 2938 "parser.c"
        break;
      case 17:
// 976 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy175, 1, 0, NULL, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(38,&yymsp[-5].minor);
  yy_destructor(36,&yymsp[-4].minor);
  yy_destructor(37,&yymsp[-2].minor);
}
// 2948 "parser.c"
        break;
      case 18:
// 980 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class(yymsp[-1].minor.yy0, yymsp[0].minor.yy175, 0, 1, NULL, NULL, status->scanner_state);
  yy_destructor(39,&yymsp[-3].minor);
  yy_destructor(36,&yymsp[-2].minor);
}
// 2957 "parser.c"
        break;
      case 19:
// 984 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy175, 0, 1, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(39,&yymsp[-5].minor);
  yy_destructor(36,&yymsp[-4].minor);
  yy_destructor(35,&yymsp[-2].minor);
}
// 2967 "parser.c"
        break;
      case 20:
      case 25:
// 988 "parser.lemon"
{
	yygotominor.yy175 = NULL;
  yy_destructor(40,&yymsp[-1].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 2977 "parser.c"
        break;
      case 21:
      case 26:
// 992 "parser.lemon"
{
	yygotominor.yy175 = yymsp[-1].minor.yy175;
  yy_destructor(40,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 2987 "parser.c"
        break;
      case 22:
      case 72:
      case 138:
      case 163:
      case 186:
      case 243:
      case 246:
// 996 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_list(yymsp[-2].minor.yy175, yymsp[0].minor.yy175);
  yy_destructor(6,&yymsp[-1].minor);
}
// 3001 "parser.c"
        break;
      case 24:
      case 221:
      case 250:
// 1004 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state);
}
// 3010 "parser.c"
        break;
      case 27:
// 1016 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_definition(yymsp[0].minor.yy175, NULL, NULL, status->scanner_state);
}
// 3017 "parser.c"
        break;
      case 28:
// 1020 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_definition(NULL, NULL, yymsp[0].minor.yy175, status->scanner_state);
}
// 3024 "parser.c"
        break;
      case 29:
// 1024 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_definition(NULL, yymsp[0].minor.yy175, NULL, status->scanner_state);
}
// 3031 "parser.c"
        break;
      case 30:
// 1028 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_definition(yymsp[-1].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
}
// 3038 "parser.c"
        break;
      case 31:
// 1032 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_definition(yymsp[-1].minor.yy175, NULL, yymsp[0].minor.yy175, status->scanner_state);
}
// 3045 "parser.c"
        break;
      case 32:
// 1036 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_definition(yymsp[0].minor.yy175, NULL, yymsp[-1].minor.yy175, status->scanner_state);
}
// 3052 "parser.c"
        break;
      case 33:
// 1040 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_definition(NULL, yymsp[0].minor.yy175, yymsp[-1].minor.yy175, status->scanner_state);
}
// 3059 "parser.c"
        break;
      case 34:
// 1044 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_definition(yymsp[-2].minor.yy175, yymsp[0].minor.yy175, yymsp[-1].minor.yy175, status->scanner_state);
}
// 3066 "parser.c"
        break;
      case 35:
// 1048 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_definition(yymsp[-1].minor.yy175, yymsp[0].minor.yy175, yymsp[-2].minor.yy175, status->scanner_state);
}
// 3073 "parser.c"
        break;
      case 38:
// 1064 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_property(yymsp[-2].minor.yy175, yymsp[-1].minor.yy0, NULL, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(33,&yymsp[0].minor);
}
// 3081 "parser.c"
        break;
      case 39:
// 1068 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_property(yymsp[-2].minor.yy175, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(33,&yymsp[0].minor);
}
// 3089 "parser.c"
        break;
      case 40:
// 1072 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_property(yymsp[-4].minor.yy175, yymsp[-3].minor.yy0, yymsp[-1].minor.yy175, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(43,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 3098 "parser.c"
        break;
      case 41:
// 1076 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_property(yymsp[-4].minor.yy175, yymsp[-3].minor.yy0, yymsp[-1].minor.yy175, NULL, status->scanner_state);
  yy_destructor(43,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 3107 "parser.c"
        break;
      case 48:
// 1104 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy175, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(44,&yymsp[-4].minor);
  yy_destructor(43,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 3117 "parser.c"
        break;
      case 49:
// 1108 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy175, NULL, status->scanner_state);
  yy_destructor(44,&yymsp[-4].minor);
  yy_destructor(43,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 3127 "parser.c"
        break;
      case 50:
// 1112 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(yymsp[-6].minor.yy175, yymsp[-4].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(46,&yymsp[-5].minor);
  yy_destructor(47,&yymsp[-3].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(40,&yymsp[-1].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3139 "parser.c"
        break;
      case 51:
// 1116 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(yymsp[-7].minor.yy175, yymsp[-5].minor.yy0, yymsp[-3].minor.yy175, NULL, NULL, status->scanner_state);
  yy_destructor(46,&yymsp[-6].minor);
  yy_destructor(47,&yymsp[-4].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(40,&yymsp[-1].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3151 "parser.c"
        break;
      case 52:
// 1120 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(yymsp[-7].minor.yy175, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy175, NULL, status->scanner_state);
  yy_destructor(46,&yymsp[-6].minor);
  yy_destructor(47,&yymsp[-4].minor);
  yy_destructor(30,&yymsp[-3].minor);
  yy_destructor(40,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3163 "parser.c"
        break;
      case 53:
// 1124 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(yymsp[-8].minor.yy175, yymsp[-6].minor.yy0, yymsp[-4].minor.yy175, yymsp[-1].minor.yy175, NULL, status->scanner_state);
  yy_destructor(46,&yymsp[-7].minor);
  yy_destructor(47,&yymsp[-5].minor);
  yy_destructor(30,&yymsp[-3].minor);
  yy_destructor(40,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3175 "parser.c"
        break;
      case 54:
// 1128 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(yymsp[-6].minor.yy175, yymsp[-4].minor.yy0, NULL, NULL, yymsp[-7].minor.yy0, status->scanner_state);
  yy_destructor(46,&yymsp[-5].minor);
  yy_destructor(47,&yymsp[-3].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(40,&yymsp[-1].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3187 "parser.c"
        break;
      case 55:
// 1132 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(yymsp[-7].minor.yy175, yymsp[-5].minor.yy0, yymsp[-3].minor.yy175, NULL, yymsp[-8].minor.yy0, status->scanner_state);
  yy_destructor(46,&yymsp[-6].minor);
  yy_destructor(47,&yymsp[-4].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(40,&yymsp[-1].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3199 "parser.c"
        break;
      case 56:
// 1136 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(yymsp[-7].minor.yy175, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy175, yymsp[-8].minor.yy0, status->scanner_state);
  yy_destructor(46,&yymsp[-6].minor);
  yy_destructor(47,&yymsp[-4].minor);
  yy_destructor(30,&yymsp[-3].minor);
  yy_destructor(40,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3211 "parser.c"
        break;
      case 57:
// 1140 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(yymsp[-8].minor.yy175, yymsp[-6].minor.yy0, yymsp[-4].minor.yy175, yymsp[-1].minor.yy175, yymsp[-9].minor.yy0, status->scanner_state);
  yy_destructor(46,&yymsp[-7].minor);
  yy_destructor(47,&yymsp[-5].minor);
  yy_destructor(30,&yymsp[-3].minor);
  yy_destructor(40,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3223 "parser.c"
        break;
      case 58:
// 1144 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(yymsp[-5].minor.yy175, yymsp[-3].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(46,&yymsp[-4].minor);
  yy_destructor(47,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[-1].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 3234 "parser.c"
        break;
      case 59:
// 1148 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(yymsp[-6].minor.yy175, yymsp[-4].minor.yy0, yymsp[-2].minor.yy175, NULL, NULL, status->scanner_state);
  yy_destructor(46,&yymsp[-5].minor);
  yy_destructor(47,&yymsp[-3].minor);
  yy_destructor(30,&yymsp[-1].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 3245 "parser.c"
        break;
      case 60:
// 1152 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(yymsp[-5].minor.yy175, yymsp[-3].minor.yy0, NULL, NULL, yymsp[-6].minor.yy0, status->scanner_state);
  yy_destructor(46,&yymsp[-4].minor);
  yy_destructor(47,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[-1].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 3256 "parser.c"
        break;
      case 61:
// 1156 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(yymsp[-6].minor.yy175, yymsp[-4].minor.yy0, yymsp[-2].minor.yy175, NULL, yymsp[-7].minor.yy0, status->scanner_state);
  yy_destructor(46,&yymsp[-5].minor);
  yy_destructor(47,&yymsp[-3].minor);
  yy_destructor(30,&yymsp[-1].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 3267 "parser.c"
        break;
      case 64:
// 1168 "parser.lemon"
{
	yygotominor.yy175 = json_object_new_string("public");
  yy_destructor(1,&yymsp[0].minor);
}
// 3275 "parser.c"
        break;
      case 65:
// 1172 "parser.lemon"
{
	yygotominor.yy175 = json_object_new_string("protected");
  yy_destructor(2,&yymsp[0].minor);
}
// 3283 "parser.c"
        break;
      case 66:
// 1176 "parser.lemon"
{
	yygotominor.yy175 = json_object_new_string("private");
  yy_destructor(4,&yymsp[0].minor);
}
// 3291 "parser.c"
        break;
      case 67:
// 1180 "parser.lemon"
{
	yygotominor.yy175 = json_object_new_string("static");
  yy_destructor(3,&yymsp[0].minor);
}
// 3299 "parser.c"
        break;
      case 68:
// 1184 "parser.lemon"
{
	yygotominor.yy175 = json_object_new_string("scoped");
  yy_destructor(5,&yymsp[0].minor);
}
// 3307 "parser.c"
        break;
      case 69:
// 1188 "parser.lemon"
{
	yygotominor.yy175 = json_object_new_string("inline");
  yy_destructor(48,&yymsp[0].minor);
}
// 3315 "parser.c"
        break;
      case 70:
// 1192 "parser.lemon"
{
	yygotominor.yy175 = json_object_new_string("abstract");
  yy_destructor(38,&yymsp[0].minor);
}
// 3323 "parser.c"
        break;
      case 71:
// 1196 "parser.lemon"
{
	yygotominor.yy175 = json_object_new_string("final");
  yy_destructor(39,&yymsp[0].minor);
}
// 3331 "parser.c"
        break;
      case 74:
// 1208 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_parameter(NULL, NULL, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 3338 "parser.c"
        break;
      case 75:
// 1212 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_parameter(yymsp[-1].minor.yy175, NULL, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 3345 "parser.c"
        break;
      case 76:
// 1216 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_parameter(NULL, yymsp[-1].minor.yy175, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 3352 "parser.c"
        break;
      case 77:
// 1220 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_parameter(NULL, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(43,&yymsp[-1].minor);
}
// 3360 "parser.c"
        break;
      case 78:
// 1224 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_parameter(yymsp[-3].minor.yy175, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(43,&yymsp[-1].minor);
}
// 3368 "parser.c"
        break;
      case 79:
// 1228 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_parameter(NULL, yymsp[-3].minor.yy175, yymsp[-2].minor.yy0, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(43,&yymsp[-1].minor);
}
// 3376 "parser.c"
        break;
      case 80:
// 1232 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(14,&yymsp[-2].minor);
  yy_destructor(15,&yymsp[0].minor);
}
// 3385 "parser.c"
        break;
      case 81:
// 1236 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_type(XX_TYPE_INTEGER);
  yy_destructor(49,&yymsp[0].minor);
}
// 3393 "parser.c"
        break;
      case 82:
// 1240 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_type(XX_TYPE_UINTEGER);
  yy_destructor(50,&yymsp[0].minor);
}
// 3401 "parser.c"
        break;
      case 83:
// 1244 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_type(XX_TYPE_LONG);
  yy_destructor(51,&yymsp[0].minor);
}
// 3409 "parser.c"
        break;
      case 84:
// 1248 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_type(XX_TYPE_ULONG);
  yy_destructor(52,&yymsp[0].minor);
}
// 3417 "parser.c"
        break;
      case 85:
// 1252 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_type(XX_TYPE_CHAR);
  yy_destructor(53,&yymsp[0].minor);
}
// 3425 "parser.c"
        break;
      case 86:
// 1256 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_type(XX_TYPE_UCHAR);
  yy_destructor(54,&yymsp[0].minor);
}
// 3433 "parser.c"
        break;
      case 87:
// 1260 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_type(XX_TYPE_DOUBLE);
  yy_destructor(55,&yymsp[0].minor);
}
// 3441 "parser.c"
        break;
      case 88:
// 1264 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_type(XX_TYPE_BOOL);
  yy_destructor(56,&yymsp[0].minor);
}
// 3449 "parser.c"
        break;
      case 89:
// 1268 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_type(XX_TYPE_STRING);
  yy_destructor(57,&yymsp[0].minor);
}
// 3457 "parser.c"
        break;
      case 90:
// 1272 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_type(XX_TYPE_ARRAY);
  yy_destructor(58,&yymsp[0].minor);
}
// 3465 "parser.c"
        break;
      case 91:
// 1276 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_type(XX_TYPE_VAR);
  yy_destructor(59,&yymsp[0].minor);
}
// 3473 "parser.c"
        break;
      case 112:
// 1360 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_break_statement(status->scanner_state);
  yy_destructor(60,&yymsp[-1].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 3482 "parser.c"
        break;
      case 113:
// 1364 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_continue_statement(status->scanner_state);
  yy_destructor(61,&yymsp[-1].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 3491 "parser.c"
        break;
      case 114:
// 1368 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_if_statement(yymsp[-2].minor.yy175, NULL, NULL, status->scanner_state);
  yy_destructor(62,&yymsp[-3].minor);
  yy_destructor(40,&yymsp[-1].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3501 "parser.c"
        break;
      case 115:
// 1372 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_if_statement(yymsp[-5].minor.yy175, NULL, NULL, status->scanner_state);
  yy_destructor(62,&yymsp[-6].minor);
  yy_destructor(40,&yymsp[-4].minor);
  yy_destructor(41,&yymsp[-3].minor);
  yy_destructor(63,&yymsp[-2].minor);
  yy_destructor(40,&yymsp[-1].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3514 "parser.c"
        break;
      case 116:
// 1376 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_if_statement(yymsp[-3].minor.yy175, yymsp[-1].minor.yy175, NULL, status->scanner_state);
  yy_destructor(62,&yymsp[-4].minor);
  yy_destructor(40,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3524 "parser.c"
        break;
      case 117:
// 1380 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_if_statement(yymsp[-7].minor.yy175, yymsp[-5].minor.yy175, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(62,&yymsp[-8].minor);
  yy_destructor(40,&yymsp[-6].minor);
  yy_destructor(41,&yymsp[-4].minor);
  yy_destructor(63,&yymsp[-3].minor);
  yy_destructor(40,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3537 "parser.c"
        break;
      case 118:
// 1384 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_if_statement(yymsp[-6].minor.yy175, yymsp[-4].minor.yy175, NULL, status->scanner_state);
  yy_destructor(62,&yymsp[-7].minor);
  yy_destructor(40,&yymsp[-5].minor);
  yy_destructor(41,&yymsp[-3].minor);
  yy_destructor(63,&yymsp[-2].minor);
  yy_destructor(40,&yymsp[-1].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3550 "parser.c"
        break;
      case 119:
// 1388 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_if_statement(yymsp[-6].minor.yy175, NULL, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(62,&yymsp[-7].minor);
  yy_destructor(40,&yymsp[-5].minor);
  yy_destructor(41,&yymsp[-4].minor);
  yy_destructor(63,&yymsp[-3].minor);
  yy_destructor(40,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3563 "parser.c"
        break;
      case 120:
// 1392 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_switch_statement(yymsp[-2].minor.yy175, NULL, status->scanner_state);
  yy_destructor(64,&yymsp[-3].minor);
  yy_destructor(40,&yymsp[-1].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3573 "parser.c"
        break;
      case 121:
// 1396 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_switch_statement(yymsp[-3].minor.yy175, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(64,&yymsp[-4].minor);
  yy_destructor(40,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3583 "parser.c"
        break;
      case 124:
// 1408 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_case_clause(yymsp[-1].minor.yy175, NULL, status->scanner_state);
  yy_destructor(65,&yymsp[-2].minor);
  yy_destructor(66,&yymsp[0].minor);
}
// 3592 "parser.c"
        break;
      case 125:
// 1412 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_case_clause(yymsp[-2].minor.yy175, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(65,&yymsp[-3].minor);
  yy_destructor(66,&yymsp[-1].minor);
}
// 3601 "parser.c"
        break;
      case 126:
// 1416 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_case_clause(NULL, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(67,&yymsp[-2].minor);
  yy_destructor(66,&yymsp[-1].minor);
}
// 3610 "parser.c"
        break;
      case 127:
// 1420 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_loop_statement(NULL, status->scanner_state);
  yy_destructor(68,&yymsp[-2].minor);
  yy_destructor(40,&yymsp[-1].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3620 "parser.c"
        break;
      case 128:
// 1424 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_loop_statement(yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(68,&yymsp[-3].minor);
  yy_destructor(40,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3630 "parser.c"
        break;
      case 129:
// 1428 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_while_statement(yymsp[-2].minor.yy175, NULL, status->scanner_state);
  yy_destructor(69,&yymsp[-3].minor);
  yy_destructor(40,&yymsp[-1].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3640 "parser.c"
        break;
      case 130:
// 1432 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_while_statement(yymsp[-3].minor.yy175, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(69,&yymsp[-4].minor);
  yy_destructor(40,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3650 "parser.c"
        break;
      case 131:
// 1436 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_do_while_statement(yymsp[-1].minor.yy175, NULL, status->scanner_state);
  yy_destructor(70,&yymsp[-5].minor);
  yy_destructor(40,&yymsp[-4].minor);
  yy_destructor(41,&yymsp[-3].minor);
  yy_destructor(69,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 3662 "parser.c"
        break;
      case 132:
// 1440 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_do_while_statement(yymsp[-1].minor.yy175, yymsp[-4].minor.yy175, status->scanner_state);
  yy_destructor(70,&yymsp[-6].minor);
  yy_destructor(40,&yymsp[-5].minor);
  yy_destructor(41,&yymsp[-3].minor);
  yy_destructor(69,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 3674 "parser.c"
        break;
      case 133:
// 1444 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_for_statement(yymsp[-3].minor.yy175, NULL, yymsp[-5].minor.yy0, 0, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(71,&yymsp[-6].minor);
  yy_destructor(72,&yymsp[-4].minor);
  yy_destructor(40,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3685 "parser.c"
        break;
      case 134:
// 1448 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_for_statement(yymsp[-3].minor.yy175, NULL, yymsp[-6].minor.yy0, 1, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(71,&yymsp[-7].minor);
  yy_destructor(72,&yymsp[-5].minor);
  yy_destructor(73,&yymsp[-4].minor);
  yy_destructor(40,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3697 "parser.c"
        break;
      case 135:
// 1452 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_for_statement(yymsp[-3].minor.yy175, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, 0, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(71,&yymsp[-8].minor);
  yy_destructor(6,&yymsp[-6].minor);
  yy_destructor(72,&yymsp[-4].minor);
  yy_destructor(40,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3709 "parser.c"
        break;
      case 136:
// 1456 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_for_statement(yymsp[-3].minor.yy175, yymsp[-8].minor.yy0, yymsp[-6].minor.yy0, 1, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(71,&yymsp[-9].minor);
  yy_destructor(6,&yymsp[-7].minor);
  yy_destructor(72,&yymsp[-5].minor);
  yy_destructor(73,&yymsp[-4].minor);
  yy_destructor(40,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3722 "parser.c"
        break;
      case 137:
// 1460 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_statement(yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(74,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 3731 "parser.c"
        break;
      case 140:
// 1473 "parser.lemon"
{
	yygotominor.yy175 = json_object_new_string("assign");
  yy_destructor(43,&yymsp[0].minor);
}
// 3739 "parser.c"
        break;
      case 141:
// 1478 "parser.lemon"
{
	yygotominor.yy175 = json_object_new_string("add-assign");
  yy_destructor(75,&yymsp[0].minor);
}
// 3747 "parser.c"
        break;
      case 142:
// 1483 "parser.lemon"
{
	yygotominor.yy175 = json_object_new_string("sub-assign");
  yy_destructor(76,&yymsp[0].minor);
}
// 3755 "parser.c"
        break;
      case 143:
// 1487 "parser.lemon"
{
	yygotominor.yy175 = json_object_new_string("mult-assign");
  yy_destructor(77,&yymsp[0].minor);
}
// 3763 "parser.c"
        break;
      case 144:
// 1491 "parser.lemon"
{
	yygotominor.yy175 = json_object_new_string("div-assign");
  yy_destructor(78,&yymsp[0].minor);
}
// 3771 "parser.c"
        break;
      case 145:
// 1495 "parser.lemon"
{
	yygotominor.yy175 = json_object_new_string("concat-assign");
  yy_destructor(79,&yymsp[0].minor);
}
// 3779 "parser.c"
        break;
      case 146:
// 1500 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("variable", yymsp[-1].minor.yy175, yymsp[-2].minor.yy0, NULL, NULL, yymsp[0].minor.yy175, status->scanner_state);
}
// 3786 "parser.c"
        break;
      case 147:
// 1505 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("object-property", yymsp[-1].minor.yy175, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(80,&yymsp[-3].minor);
}
// 3794 "parser.c"
        break;
      case 148:
// 1510 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("variable-dynamic-object-property", yymsp[-1].minor.yy175, yymsp[-6].minor.yy0, yymsp[-3].minor.yy0, NULL, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(80,&yymsp[-5].minor);
  yy_destructor(40,&yymsp[-4].minor);
  yy_destructor(41,&yymsp[-2].minor);
}
// 3804 "parser.c"
        break;
      case 149:
// 1515 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("string-dynamic-object-property", yymsp[-1].minor.yy175, yymsp[-6].minor.yy0, yymsp[-3].minor.yy0, NULL, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(80,&yymsp[-5].minor);
  yy_destructor(40,&yymsp[-4].minor);
  yy_destructor(41,&yymsp[-2].minor);
}
// 3814 "parser.c"
        break;
      case 150:
// 1520 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("object-property-append", yymsp[-1].minor.yy175, yymsp[-6].minor.yy0, yymsp[-4].minor.yy0, NULL, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(80,&yymsp[-5].minor);
  yy_destructor(82,&yymsp[-3].minor);
  yy_destructor(83,&yymsp[-2].minor);
}
// 3824 "parser.c"
        break;
      case 151:
// 1525 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("object-property-array-index", yymsp[-1].minor.yy175, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, yymsp[-3].minor.yy175, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(80,&yymsp[-6].minor);
  yy_destructor(82,&yymsp[-4].minor);
  yy_destructor(83,&yymsp[-2].minor);
}
// 3834 "parser.c"
        break;
      case 152:
// 1530 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("static-property", yymsp[-1].minor.yy175, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(84,&yymsp[-3].minor);
}
// 3842 "parser.c"
        break;
      case 153:
// 1535 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("static-property-append", yymsp[-1].minor.yy175, yymsp[-6].minor.yy0, yymsp[-4].minor.yy0, NULL, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(84,&yymsp[-5].minor);
  yy_destructor(82,&yymsp[-3].minor);
  yy_destructor(83,&yymsp[-2].minor);
}
// 3852 "parser.c"
        break;
      case 154:
// 1540 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("static-property-array-index", yymsp[-1].minor.yy175, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, yymsp[-3].minor.yy175, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(84,&yymsp[-6].minor);
  yy_destructor(82,&yymsp[-4].minor);
  yy_destructor(83,&yymsp[-2].minor);
}
// 3862 "parser.c"
        break;
      case 155:
// 1545 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("variable-append", yymsp[-1].minor.yy175, yymsp[-4].minor.yy0, NULL, NULL, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(82,&yymsp[-3].minor);
  yy_destructor(83,&yymsp[-2].minor);
}
// 3871 "parser.c"
        break;
      case 156:
// 1550 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("array-index", yymsp[-1].minor.yy175, yymsp[-5].minor.yy0, NULL, yymsp[-3].minor.yy175, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(82,&yymsp[-4].minor);
  yy_destructor(83,&yymsp[-2].minor);
}
// 3880 "parser.c"
        break;
      case 157:
// 1555 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("incr", NULL, yymsp[-3].minor.yy0, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(80,&yymsp[-2].minor);
  yy_destructor(85,&yymsp[0].minor);
}
// 3889 "parser.c"
        break;
      case 158:
// 1560 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("decr", NULL, yymsp[-3].minor.yy0, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(80,&yymsp[-2].minor);
  yy_destructor(86,&yymsp[0].minor);
}
// 3898 "parser.c"
        break;
      case 159:
// 1565 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("incr", NULL, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(85,&yymsp[0].minor);
}
// 3906 "parser.c"
        break;
      case 160:
// 1570 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("decr", NULL, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(86,&yymsp[0].minor);
}
// 3914 "parser.c"
        break;
      case 162:
// 1578 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_echo_statement(yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(87,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 3923 "parser.c"
        break;
      case 165:
// 1590 "parser.lemon"
{
	yygotominor.yy175 = yymsp[0].minor.yy175;;
}
// 3930 "parser.c"
        break;
      case 166:
// 1595 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_mcall_statement(yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(33,&yymsp[0].minor);
}
// 3938 "parser.c"
        break;
      case 167:
// 1600 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_fcall_statement(yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(33,&yymsp[0].minor);
}
// 3946 "parser.c"
        break;
      case 168:
// 1605 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_scall_statement(yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(33,&yymsp[0].minor);
}
// 3954 "parser.c"
        break;
      case 169:
// 1610 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_return_statement(yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(88,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 3963 "parser.c"
        break;
      case 170:
// 1615 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_return_statement(NULL, status->scanner_state);
  yy_destructor(88,&yymsp[-1].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 3972 "parser.c"
        break;
      case 171:
// 1620 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(89,&yymsp[-4].minor);
  yy_destructor(80,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 3986 "parser.c"
        break;
      case 172:
// 1629 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(89,&yymsp[-5].minor);
  yy_destructor(82,&yymsp[-3].minor);
  yy_destructor(83,&yymsp[-1].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 4001 "parser.c"
        break;
      case 173:
// 1638 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_INTEGER, yymsp[-2].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(89,&yymsp[-5].minor);
  yy_destructor(82,&yymsp[-3].minor);
  yy_destructor(83,&yymsp[-1].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 4016 "parser.c"
        break;
      case 174:
// 1647 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_STRING, yymsp[-2].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(89,&yymsp[-5].minor);
  yy_destructor(82,&yymsp[-3].minor);
  yy_destructor(83,&yymsp[-1].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 4031 "parser.c"
        break;
      case 175:
// 1656 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_throw_exception(yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(91,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 4040 "parser.c"
        break;
      case 176:
// 1660 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_statement(XX_T_TYPE_INTEGER, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 4049 "parser.c"
        break;
      case 177:
// 1664 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_statement(XX_T_TYPE_UINTEGER, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(50,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 4058 "parser.c"
        break;
      case 178:
// 1668 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_statement(XX_T_TYPE_CHAR, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(53,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 4067 "parser.c"
        break;
      case 179:
// 1672 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_statement(XX_T_TYPE_UCHAR, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 4076 "parser.c"
        break;
      case 180:
// 1676 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_statement(XX_T_TYPE_LONG, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(51,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 4085 "parser.c"
        break;
      case 181:
// 1680 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_statement(XX_T_TYPE_ULONG, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(52,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 4094 "parser.c"
        break;
      case 182:
// 1684 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_statement(XX_T_TYPE_DOUBLE, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(55,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 4103 "parser.c"
        break;
      case 183:
// 1688 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_statement(XX_T_TYPE_STRING, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(57,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 4112 "parser.c"
        break;
      case 184:
// 1692 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_statement(XX_T_TYPE_BOOL, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(56,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 4121 "parser.c"
        break;
      case 185:
// 1696 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_statement(XX_T_TYPE_VAR, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(59,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 4130 "parser.c"
        break;
      case 188:
// 1708 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_variable(yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 4137 "parser.c"
        break;
      case 189:
// 1712 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_variable(yymsp[-2].minor.yy0, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(43,&yymsp[-1].minor);
}
// 4145 "parser.c"
        break;
      case 191:
// 1720 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("not", yymsp[0].minor.yy175, NULL, NULL, status->scanner_state);
  yy_destructor(29,&yymsp[-1].minor);
}
// 4153 "parser.c"
        break;
      case 192:
// 1724 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("isset", yymsp[0].minor.yy175, NULL, NULL, status->scanner_state);
  yy_destructor(26,&yymsp[-1].minor);
}
// 4161 "parser.c"
        break;
      case 193:
// 1728 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("require", yymsp[0].minor.yy175, NULL, NULL, status->scanner_state);
  yy_destructor(7,&yymsp[-1].minor);
}
// 4169 "parser.c"
        break;
      case 194:
// 1732 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("equals", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(12,&yymsp[-1].minor);
}
// 4177 "parser.c"
        break;
      case 195:
// 1736 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("not-equals", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(19,&yymsp[-1].minor);
}
// 4185 "parser.c"
        break;
      case 196:
// 1740 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("identical", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(13,&yymsp[-1].minor);
}
// 4193 "parser.c"
        break;
      case 197:
// 1744 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("not-identical", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(18,&yymsp[-1].minor);
}
// 4201 "parser.c"
        break;
      case 198:
// 1748 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("less", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(14,&yymsp[-1].minor);
}
// 4209 "parser.c"
        break;
      case 199:
// 1752 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("greater", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(15,&yymsp[-1].minor);
}
// 4217 "parser.c"
        break;
      case 200:
// 1756 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("less-equal", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(16,&yymsp[-1].minor);
}
// 4225 "parser.c"
        break;
      case 201:
// 1760 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("greater-equal", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(17,&yymsp[-1].minor);
}
// 4233 "parser.c"
        break;
      case 202:
// 1764 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("list", yymsp[-1].minor.yy175, NULL, NULL, status->scanner_state);
  yy_destructor(47,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 4242 "parser.c"
        break;
      case 203:
// 1768 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("cast", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(47,&yymsp[-3].minor);
  yy_destructor(30,&yymsp[-1].minor);
}
// 4251 "parser.c"
        break;
      case 204:
// 1772 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("type-hint", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(14,&yymsp[-3].minor);
  yy_destructor(15,&yymsp[-1].minor);
}
// 4260 "parser.c"
        break;
      case 205:
// 1776 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("property-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(80,&yymsp[-1].minor);
}
// 4268 "parser.c"
        break;
      case 206:
// 1780 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("static-property-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(84,&yymsp[-1].minor);
}
// 4276 "parser.c"
        break;
      case 207:
// 1784 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("static-constant-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(84,&yymsp[-1].minor);
}
// 4284 "parser.c"
        break;
      case 208:
      case 261:
// 1789 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), yymsp[-1].minor.yy175, NULL, status->scanner_state);
  yy_destructor(82,&yymsp[-2].minor);
  yy_destructor(83,&yymsp[0].minor);
}
// 4294 "parser.c"
        break;
      case 209:
// 1794 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("add", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(20,&yymsp[-1].minor);
}
// 4302 "parser.c"
        break;
      case 210:
// 1799 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("sub", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(21,&yymsp[-1].minor);
}
// 4310 "parser.c"
        break;
      case 211:
// 1804 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("mul", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(23,&yymsp[-1].minor);
}
// 4318 "parser.c"
        break;
      case 212:
// 1809 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("div", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(24,&yymsp[-1].minor);
}
// 4326 "parser.c"
        break;
      case 213:
// 1814 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("mod", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(25,&yymsp[-1].minor);
}
// 4334 "parser.c"
        break;
      case 214:
// 1819 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("concat", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(22,&yymsp[-1].minor);
}
// 4342 "parser.c"
        break;
      case 215:
// 1824 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("and", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(10,&yymsp[-1].minor);
}
// 4350 "parser.c"
        break;
      case 216:
// 1829 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("or", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(11,&yymsp[-1].minor);
}
// 4358 "parser.c"
        break;
      case 217:
// 1834 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("instanceof", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(9,&yymsp[-1].minor);
}
// 4366 "parser.c"
        break;
      case 218:
// 1839 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("fetch", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(27,&yymsp[-3].minor);
  yy_destructor(6,&yymsp[-1].minor);
}
// 4375 "parser.c"
        break;
      case 219:
// 1844 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("typeof", yymsp[0].minor.yy175, NULL, NULL, status->scanner_state);
  yy_destructor(8,&yymsp[-1].minor);
}
// 4383 "parser.c"
        break;
      case 220:
// 1849 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_literal(XX_T_CONSTANT, yymsp[0].minor.yy0, status->scanner_state);
}
// 4390 "parser.c"
        break;
      case 222:
      case 252:
      case 254:
// 1859 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_literal(XX_T_INTEGER, yymsp[0].minor.yy0, status->scanner_state);
}
// 4399 "parser.c"
        break;
      case 223:
      case 251:
      case 256:
// 1864 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_literal(XX_T_STRING, yymsp[0].minor.yy0, status->scanner_state);
}
// 4408 "parser.c"
        break;
      case 224:
      case 255:
// 1869 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_literal(XX_T_CHAR, yymsp[0].minor.yy0, status->scanner_state);
}
// 4416 "parser.c"
        break;
      case 225:
      case 257:
// 1874 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_literal(XX_T_DOUBLE, yymsp[0].minor.yy0, status->scanner_state);
}
// 4424 "parser.c"
        break;
      case 226:
      case 258:
// 1879 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_literal(XX_T_NULL, NULL, status->scanner_state);
  yy_destructor(94,&yymsp[0].minor);
}
// 4433 "parser.c"
        break;
      case 227:
      case 260:
// 1884 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_literal(XX_T_TRUE, NULL, status->scanner_state);
  yy_destructor(95,&yymsp[0].minor);
}
// 4442 "parser.c"
        break;
      case 228:
      case 259:
// 1889 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_literal(XX_T_FALSE, NULL, status->scanner_state);
  yy_destructor(96,&yymsp[0].minor);
}
// 4451 "parser.c"
        break;
      case 229:
// 1894 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("empty-array", NULL, NULL, NULL, status->scanner_state);
  yy_destructor(82,&yymsp[-1].minor);
  yy_destructor(83,&yymsp[0].minor);
}
// 4460 "parser.c"
        break;
      case 230:
// 1899 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("array", yymsp[-1].minor.yy175, NULL, NULL, status->scanner_state);
  yy_destructor(82,&yymsp[-2].minor);
  yy_destructor(83,&yymsp[0].minor);
}
// 4469 "parser.c"
        break;
      case 231:
// 1904 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_new_instance(yymsp[0].minor.yy0, NULL, status->scanner_state);
  yy_destructor(28,&yymsp[-1].minor);
}
// 4477 "parser.c"
        break;
      case 232:
// 1909 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_new_instance(yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(28,&yymsp[-3].minor);
  yy_destructor(47,&yymsp[-1].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 4487 "parser.c"
        break;
      case 233:
// 1914 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_new_instance(yymsp[-3].minor.yy0, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(28,&yymsp[-4].minor);
  yy_destructor(47,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 4497 "parser.c"
        break;
      case 234:
// 1919 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_fcall(yymsp[-3].minor.yy0, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(47,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 4506 "parser.c"
        break;
      case 235:
// 1924 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_fcall(yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(47,&yymsp[-1].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 4515 "parser.c"
        break;
      case 236:
// 1929 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_scall(yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(84,&yymsp[-4].minor);
  yy_destructor(47,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 4525 "parser.c"
        break;
      case 237:
// 1934 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_scall(yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(84,&yymsp[-3].minor);
  yy_destructor(47,&yymsp[-1].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 4535 "parser.c"
        break;
      case 238:
// 1939 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_mcall(yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(80,&yymsp[-4].minor);
  yy_destructor(47,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 4545 "parser.c"
        break;
      case 239:
// 1944 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_mcall(yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(80,&yymsp[-3].minor);
  yy_destructor(47,&yymsp[-1].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 4555 "parser.c"
        break;
      case 248:
// 1983 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_array_item(yymsp[-2].minor.yy175, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(66,&yymsp[-1].minor);
}
// 4563 "parser.c"
        break;
      case 249:
// 1987 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_array_item(NULL, yymsp[0].minor.yy175, status->scanner_state);
}
// 4570 "parser.c"
        break;
      case 263:
// 2044 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_comment(yymsp[0].minor.yy0, status->scanner_state);
}
// 4577 "parser.c"
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

// 4653 "parser.c"
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
