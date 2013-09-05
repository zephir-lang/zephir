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
#define YYNSTATE 539
#define YYNRULE 260
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
 /*     0 */   175,  195,  197,  199,  499,  213,  800,    1,    2,  538,
 /*    10 */     4,    5,    6,    7,  500,  146,  507,  171,  224,  228,
 /*    20 */   222,  244,  168,   95,  508,  217,   97,  510,  516,  515,
 /*    30 */   503,   82,  509,  483,   65,   43,   47,   85,  230,  148,
 /*    40 */   207,  447,   54,   55,   56,   57,   58,   59,   60,   61,
 /*    50 */    62,   63,   64,  203,  205,  170,  179,  183,  185,  187,
 /*    60 */   189,  181,  177,  191,  193,  201,  195,  197,  199,  511,
 /*    70 */   465,  514,  515,  503,  238,  482,  483,   85,  380,   14,
 /*    80 */    85,  417,  231,  232,   17,  233,  234,  235,  236,  237,
 /*    90 */   440,  422,   67,   68,   70,   69,   71,  505,  381,  390,
 /*   100 */   399,  402,  393,  396,  405,  411,  408,  506,  414,  128,
 /*   110 */   130,  132,  314,  141,   32,   43,   47,  152,  156,  161,
 /*   120 */   286,  497,  474,  293,  417,   65,   43,   47,   85,   73,
 /*   130 */    74,  144,   48,  160,  422,  346,  359,  363,  162,   72,
 /*   140 */   378,  381,  390,  399,  402,  393,  396,  405,  411,  408,
 /*   150 */   103,  414,  128,  130,  132,  148,  141,  447,  394,  389,
 /*   160 */   152,  156,  161,  286,  310,  263,  293,  417,   36,   38,
 /*   170 */   309,   37,   39,   40,   42,   41,  106,  422,  346,  359,
 /*   180 */   363,  406,  389,  378,  381,  390,  399,  402,  393,  396,
 /*   190 */   405,  411,  408,  369,  414,  128,  130,  132,  420,  141,
 /*   200 */    50,  418,  165,  152,  156,  161,  286,  294,  345,  293,
 /*   210 */   417,  191,  193,  201,  195,  197,  199,  250,  384,  445,
 /*   220 */   422,  346,  359,  363,  299,  521,  378,  381,  390,  399,
 /*   230 */   402,  393,  396,  405,  411,  408,  497,  414,  128,  130,
 /*   240 */   132,  265,  141,   85,  250,  407,  152,  156,  161,  286,
 /*   250 */   372,  375,  293,  417,  415,  389,  300,  301,  302,  303,
 /*   260 */   304,  384,  136,  422,  346,  359,  363,  384,  249,  378,
 /*   270 */   381,  390,  399,  402,  393,  396,  405,  411,  408,  522,
 /*   280 */   414,  128,  130,  132,  242,  141,  382,  389,  416,  152,
 /*   290 */   156,  161,  286,  351,  398,  293,  417,  259,  272,  347,
 /*   300 */   352,  251,  253,  252,  384,  452,  422,  346,  359,  363,
 /*   310 */   365,  368,  378,  381,  390,  399,  402,  393,  396,  405,
 /*   320 */   411,  408,  528,  414,  128,  130,  132,   93,  141,  403,
 /*   330 */   389,  383,  152,  156,  161,  286,  307,  172,  293,  417,
 /*   340 */   319,  145,  450,  384,  251,  253,  252,  519,  457,  422,
 /*   350 */   346,  359,  363,  409,  389,  378,  381,  390,  399,  402,
 /*   360 */   393,  396,  405,  411,  408,   76,  414,  128,  130,  132,
 /*   370 */   410,  141,   85,   31,  271,  152,  156,  161,  286,  351,
 /*   380 */   458,  293,  417,   65,   43,   47,  350,  251,  253,  252,
 /*   390 */   429,  428,  422,  346,  359,  363,  386,  459,  378,  381,
 /*   400 */   390,  399,  402,  393,  396,  405,  411,  408,   12,  414,
 /*   410 */   128,  130,  132,  518,  141,  523,  250,  520,  152,  156,
 /*   420 */   161,  286,  305,  221,  293,  417,    3,    4,    5,    6,
 /*   430 */     7,  251,  253,  252,  471,  422,  346,  359,  363,  254,
 /*   440 */   280,  378,  381,  390,  399,  402,  393,  396,  405,  411,
 /*   450 */   408,   93,  414,  128,  130,  132,  288,  141,  525,  250,
 /*   460 */   167,  152,  156,  161,  286,  320,  535,  293,  417,  100,
 /*   470 */   316,   93,   99,  251,  253,  252,   85,  461,  422,  346,
 /*   480 */   359,  363,  384,  257,  378,  381,  390,  399,  402,  393,
 /*   490 */   396,  405,  411,  408,  526,  414,  128,  130,  132,  296,
 /*   500 */   141,  391,  389,  173,  152,  156,  161,  286,  320,  392,
 /*   510 */   293,  417,  527,  339,  287,  384,  251,  253,  252,  529,
 /*   520 */   108,  422,  346,  359,  363,  384,  295,  378,  381,  390,
 /*   530 */   399,  402,  393,  396,  405,  411,  408,   93,  414,  128,
 /*   540 */   130,  132,  404,  141,  530,   31,  105,  152,  156,  161,
 /*   550 */   286,  307,  401,  293,  417,  334,   31,  400,  389,  251,
 /*   560 */   253,  252,  349,  164,  422,  346,  359,  363,  384,   83,
 /*   570 */   378,  381,  390,  399,  402,  393,  396,  405,  411,  408,
 /*   580 */   475,  414,  128,  130,  132,  384,  141,  412,  389,  348,
 /*   590 */   152,  156,  161,  286,  320,  413,  293,  417,  533,  331,
 /*   600 */   397,  389,  251,  253,  252,  532,  473,  422,  346,  359,
 /*   610 */   363,  534,  395,  378,  381,  390,  399,  402,  393,  396,
 /*   620 */   405,  411,  408,  297,  414,  128,  130,  132,   49,  141,
 /*   630 */   536,   31,  298,  152,  156,  161,  286,  307,  537,  293,
 /*   640 */   417,  342,  225,  226,  174,  251,  253,  252,   88,  463,
 /*   650 */   422,  346,  359,  363,   89,   29,  378,  381,  390,  399,
 /*   660 */   402,  393,  396,  405,  411,  408,  227,  414,  128,  130,
 /*   670 */   132,   27,  141,   17,  129,   91,  152,  156,  161,  286,
 /*   680 */   307,  131,  293,  417,  315,   92,   16,  539,  251,  253,
 /*   690 */   252,   44,  477,  422,  346,  359,  363,   25,  134,  378,
 /*   700 */   381,  390,  399,  402,  393,  396,  405,  411,  408,  419,
 /*   710 */   414,  128,  130,  132,  243,  141,  421,  220,   15,  152,
 /*   720 */   156,  161,  286,  307,  388,  293,  417,  311,   24,   66,
 /*   730 */   385,  251,  253,  252,  387,  441,  422,  346,  359,  363,
 /*   740 */   219,  137,  378,  381,  390,  399,  402,  393,  396,  405,
 /*   750 */   411,  408,  430,  414,  128,  130,  132,  431,  141,  138,
 /*   760 */    84,   10,  152,  156,  161,  286,  307,  215,  293,  417,
 /*   770 */   330,  377,  245,  214,  251,  253,  252,  376,  479,  422,
 /*   780 */   346,  359,  363,  263,  374,  378,  381,  390,  399,  402,
 /*   790 */   393,  396,  405,  411,  408,  246,  414,  128,  130,  132,
 /*   800 */   442,  141,  785,   13,  373,  152,  156,  161,  286,  444,
 /*   810 */    28,  293,  417,   65,   43,   47,  218,  260,  211,  241,
 /*   820 */   371,  140,  422,  346,  359,  363,   81,   45,  378,  381,
 /*   830 */   390,  399,  402,  393,  396,  405,  411,  408,  448,  414,
 /*   840 */   128,  130,  132,   96,  141,  370,  367,  366,  152,  156,
 /*   850 */   161,  286,  307,  364,  293,  417,  326,  453,  143,  454,
 /*   860 */   251,  253,  252,   35,  435,  422,  346,  359,  363,   79,
 /*   870 */    46,  378,  381,  390,  399,  402,  393,  396,  405,  411,
 /*   880 */   408,  460,  414,  128,  130,  132,  147,  141,  358,   78,
 /*   890 */   467,  152,  156,  161,  286,  307,  356,  293,  417,  306,
 /*   900 */   354,  468,  470,  251,  253,  252,  149,  446,  422,  346,
 /*   910 */   359,  363,  150,   34,  378,  381,  390,  399,  402,  393,
 /*   920 */   396,  405,  411,  408,   30,  414,  128,  130,  132,  476,
 /*   930 */   141,  617,  341,  340,  152,  156,  161,  286,  307,   53,
 /*   940 */   293,  417,  338,  337,  153,  486,  251,  253,  252,  333,
 /*   950 */   292,  422,  346,  359,  363,  487,  787,  378,  381,  390,
 /*   960 */   399,  402,  393,  396,  405,  411,  408,  332,  414,  128,
 /*   970 */   130,  132,  489,  141,  786,  491,  488,  152,  156,  161,
 /*   980 */   286,  282,  329,  293,  417,  492,    9,  325,  493,  251,
 /*   990 */   253,  252,  494,  155,  422,  346,  359,  363,  324,   52,
 /*  1000 */   378,  381,  390,  399,  402,  393,  396,  405,  411,  408,
 /*  1010 */   158,  414,  128,  130,  132,  102,  141,  502,  501,  275,
 /*  1020 */   152,  156,  161,  286,  178,  318,  293,  417,  317,  430,
 /*  1030 */   430,  430,  251,  253,  252,  430,  456,  422,  346,  359,
 /*  1040 */   363,  430,  430,  378,  381,  390,  399,  402,  393,  396,
 /*  1050 */   405,  411,  408,  430,  414,  128,  130,  132,  430,  141,
 /*  1060 */   430,  430,  430,  152,  156,  161,  286,  176,  430,  293,
 /*  1070 */   417,  430,  430,  430,  430,  251,  253,  252,  430,  451,
 /*  1080 */   422,  346,  359,  363,  430,  430,  378,  381,  390,  399,
 /*  1090 */   402,  393,  396,  405,  411,  408,  430,  414,  128,  130,
 /*  1100 */   132,  430,  141,  430,  430,   19,  152,  156,  161,  286,
 /*  1110 */    22,  430,  293,  417,   86,  430,  430,   85,  430,   51,
 /*  1120 */   430,  430,  430,  422,  346,  359,  363,  430,  430,  378,
 /*  1130 */   381,  390,  399,  402,  393,  396,  405,  411,  408,   33,
 /*  1140 */   414,  128,  130,  132,  430,  141,  430,  430,  430,  152,
 /*  1150 */   156,  161,  286,  127,  430,  293,   54,   55,   56,   57,
 /*  1160 */    58,   59,   60,   61,   62,   63,   64,  346,  359,  363,
 /*  1170 */   455,  430,  378,  430,  430,  423,  110,  111,  112,  113,
 /*  1180 */   114,  115,  116,  117,  118,  119,  120,  121,  122,  123,
 /*  1190 */   124,  125,  126,  127,  430,  430,  430,  480,   98,  495,
 /*  1200 */   503,  430,  509,  483,  353,  355,  357,   85,  430,  430,
 /*  1210 */   135,  430,  430,  430,  430,  423,  110,  111,  112,  113,
 /*  1220 */   114,  115,  116,  117,  118,  119,  120,  121,  122,  123,
 /*  1230 */   124,  125,  126,  430,  481,  127,  100,  430,  482,  483,
 /*  1240 */   430,  430,  430,   85,  353,  355,  357,   67,   68,   70,
 /*  1250 */    69,   71,  154,  430,  430,  430,  430,  423,  110,  111,
 /*  1260 */   112,  113,  114,  115,  116,  117,  118,  119,  120,  121,
 /*  1270 */   122,  123,  124,  125,  126,  127,   22,  430,  430,  430,
 /*  1280 */    21,  430,  430,   85,   73,   74,  353,  355,  357,  430,
 /*  1290 */   485,  430,  472,  430,   72,  430,  430,  423,  110,  111,
 /*  1300 */   112,  113,  114,  115,  116,  117,  118,  119,  120,  121,
 /*  1310 */   122,  123,  124,  125,  126,  221,  127,  430,  430,  430,
 /*  1320 */   430,  430,  430,  251,  253,  252,  353,  355,  357,  430,
 /*  1330 */   279,  255,  430,  449,  430,  430,  430,  430,  423,  110,
 /*  1340 */   111,  112,  113,  114,  115,  116,  117,  118,  119,  120,
 /*  1350 */   121,  122,  123,  124,  125,  126,  221,  430,  127,  430,
 /*  1360 */   430,  430,  430,  430,  251,  253,  252,  353,  355,  357,
 /*  1370 */   299,  256,  255,  430,   90,  291,  517,  430,  430,   93,
 /*  1380 */   423,  110,  111,  112,  113,  114,  115,  116,  117,  118,
 /*  1390 */   119,  120,  121,  122,  123,  124,  125,  126,  127,  430,
 /*  1400 */   430,  430,  300,  301,  302,  303,  304,  430,  327,  353,
 /*  1410 */   355,  357,  430,  430,  430,  427,  430,  430,  430,  430,
 /*  1420 */   423,  110,  111,  112,  113,  114,  115,  116,  117,  118,
 /*  1430 */   119,  120,  121,  122,  123,  124,  125,  126,  430,  127,
 /*  1440 */   430,  430,  430,  512,  495,  503,  430,  430,  483,  353,
 /*  1450 */   355,  357,   85,  430,  430,  430,  163,  430,  430,  430,
 /*  1460 */   430,  423,  110,  111,  112,  113,  114,  115,  116,  117,
 /*  1470 */   118,  119,  120,  121,  122,  123,  124,  125,  126,  221,
 /*  1480 */   430,  127,  430,  430,  430,  430,  430,  251,  253,  252,
 /*  1490 */   353,  355,  357,  263,  248,  255,  430,  430,  139,  430,
 /*  1500 */   430,  430,  430,  423,  110,  111,  112,  113,  114,  115,
 /*  1510 */   116,  117,  118,  119,  120,  121,  122,  123,  124,  125,
 /*  1520 */   126,  127,  430,  430,  430,  430,  218,  260,  133,  241,
 /*  1530 */   430,  285,  353,  355,  357,  430,  430,  430,  462,  251,
 /*  1540 */   253,  252,  430,  423,  110,  111,  112,  113,  114,  115,
 /*  1550 */   116,  117,  118,  119,  120,  121,  122,  123,  124,  125,
 /*  1560 */   126,  430,  127,  430,  430,  430,  430,  430,  430,  430,
 /*  1570 */   430,  430,  353,  355,  357,  430,  430,  430,  430,  439,
 /*  1580 */   430,  430,  430,  430,  423,  110,  111,  112,  113,  114,
 /*  1590 */   115,  116,  117,  118,  119,  120,  121,  122,  123,  124,
 /*  1600 */   125,  126,  430,  430,  127,  430,  430,  142,  430,  430,
 /*  1610 */   285,  430,  430,  353,  355,  357,  430,  430,  251,  253,
 /*  1620 */   252,  434,  430,  430,  430,  430,  423,  110,  111,  112,
 /*  1630 */   113,  114,  115,  116,  117,  118,  119,  120,  121,  122,
 /*  1640 */   123,  124,  125,  126,  127,  430,  430,  430,  430,  430,
 /*  1650 */   430,  166,  430,  430,  285,  353,  355,  357,  430,  430,
 /*  1660 */   430,  478,  251,  253,  252,  430,  423,  110,  111,  112,
 /*  1670 */   113,  114,  115,  116,  117,  118,  119,  120,  121,  122,
 /*  1680 */   123,  124,  125,  126,  430,  127,  430,  157,  430,  430,
 /*  1690 */   285,  430,  430,  430,  430,  353,  355,  357,  251,  253,
 /*  1700 */   252,  430,  159,  430,  430,  430,  430,  423,  110,  111,
 /*  1710 */   112,  113,  114,  115,  116,  117,  118,  119,  120,  121,
 /*  1720 */   122,  123,  124,  125,  126,  430,  430,  127,  430,  430,
 /*  1730 */   443,  430,  430,  285,  430,  430,  353,  355,  357,  430,
 /*  1740 */   430,  251,  253,  252,  107,  430,  430,  430,  430,  423,
 /*  1750 */   110,  111,  112,  113,  114,  115,  116,  117,  118,  119,
 /*  1760 */   120,  121,  122,  123,  124,  125,  126,  127,  430,  430,
 /*  1770 */   430,  430,  430,  430,  430,  430,  430,  425,  353,  355,
 /*  1780 */   357,  430,  430,  430,  151,  251,  253,  252,  430,  423,
 /*  1790 */   110,  111,  112,  113,  114,  115,  116,  117,  118,  119,
 /*  1800 */   120,  121,  122,  123,  124,  125,  126,  430,  127,  430,
 /*  1810 */   430,  430,  430,  200,  430,  430,  437,  430,  353,  355,
 /*  1820 */   357,  251,  253,  252,  251,  253,  252,  430,  430,  430,
 /*  1830 */   109,  110,  111,  112,  113,  114,  115,  116,  117,  118,
 /*  1840 */   119,  120,  121,  122,  123,  124,  125,  126,  175,  430,
 /*  1850 */   430,  430,  430,  213,  430,  430,  430,  430,  430,  353,
 /*  1860 */   355,  357,  430,  430,  430,  171,  224,  228,  222,  244,
 /*  1870 */   168,  430,  430,  240,  175,  299,  430,  430,  430,  213,
 /*  1880 */   430,   67,   68,   70,   69,   71,  230,  430,  207,  430,
 /*  1890 */   430,  171,  224,  228,  222,  244,  168,  430,  430,  217,
 /*  1900 */   430,  430,  430,  430,  430,  430,  430,  300,  301,  302,
 /*  1910 */   303,  304,  230,  312,  207,  430,  321,  322,   73,   74,
 /*  1920 */   210,  430,  238,  269,  430,  430,  466,  430,   72,  430,
 /*  1930 */   267,  268,  430,  233,  234,  235,  236,  237,  299,    8,
 /*  1940 */   430,  430,   11,  208,   87,  430,  524,  531,  238,  313,
 /*  1950 */   422,  251,  253,  252,  430,  430,  231,  232,  430,  233,
 /*  1960 */   234,  235,  236,  237,  175,  430,  430,  430,  430,  213,
 /*  1970 */   300,  301,  302,  303,  304,  308,  335,  430,  323,  343,
 /*  1980 */   344,  171,  224,  228,  222,  244,  168,  540,  430,  217,
 /*  1990 */   175,  430,  430,  430,  430,  213,  430,   67,   68,   70,
 /*  2000 */    69,   71,  230,  430,  207,  430,  430,  171,  224,  228,
 /*  2010 */   222,  244,  168,  281,  430,  217,  430,  430,    8,  430,
 /*  2020 */   430,   11,  430,   87,  430,  524,  531,  430,  230,  422,
 /*  2030 */   207,  430,  430,  430,   73,   74,  430,  430,  238,  328,
 /*  2040 */   430,  430,   77,  430,   72,  430,  231,  232,  239,  233,
 /*  2050 */   234,  235,  236,  237,  430,  430,  251,  253,  252,  430,
 /*  2060 */   169,  430,  270,  430,  238,  278,  274,  277,  251,  253,
 /*  2070 */   252,  430,  231,  232,  430,  233,  234,  235,  236,  237,
 /*  2080 */   175,  430,  198,  430,  430,  213,  229,  430,  430,  430,
 /*  2090 */   251,  253,  252,  430,  251,  253,  252,  171,  224,  228,
 /*  2100 */   222,  244,  168,  258,  430,  217,  175,  430,  430,  430,
 /*  2110 */   430,  213,  430,   67,   68,   70,   69,   71,  230,  430,
 /*  2120 */   207,  430,  430,  171,  224,  228,  222,  244,  168,  247,
 /*  2130 */   239,  217,  430,  239,  430,  430,  430,  430,  251,  253,
 /*  2140 */   252,  251,  253,  252,  230,  430,  207,  273,  274,  277,
 /*  2150 */    73,   74,  276,  430,  238,  430,  430,  430,  101,  430,
 /*  2160 */    72,  430,  231,  232,  186,  233,  234,  235,  236,  237,
 /*  2170 */   430,  430,  251,  253,  252,  430,  430,  430,  430,  430,
 /*  2180 */   238,  430,   67,   68,   70,   69,   71,  430,  231,  232,
 /*  2190 */   430,  233,  234,  235,  236,  237,  175,  196,  430,  430,
 /*  2200 */   284,  213,  430,  430,  430,  251,  253,  252,  251,  253,
 /*  2210 */   252,  430,  430,  171,  224,  228,  222,  244,  168,   73,
 /*  2220 */    74,  217,  221,  175,  430,  430,  430,   23,  213,   72,
 /*  2230 */   251,  253,  252,  430,  230,  430,  207,  264,  255,  430,
 /*  2240 */   171,  224,  228,  222,  244,  168,  289,  430,  217,  212,
 /*  2250 */   430,  430,  430,  430,  251,  253,  252,  251,  253,  252,
 /*  2260 */   430,  230,  424,  207,  261,  430,  430,  190,  430,  430,
 /*  2270 */   238,  430,  251,  253,  252,  251,  253,  252,  231,  232,
 /*  2280 */   430,  233,  234,  235,  236,  237,  430,  430,  430,  436,
 /*  2290 */   432,  430,  430,  194,  430,  430,  430,  238,  251,  253,
 /*  2300 */   252,  251,  253,  252,  430,  231,  232,  184,  233,  234,
 /*  2310 */   235,  236,  237,  175,  206,  251,  253,  252,  213,  430,
 /*  2320 */   430,  430,  251,  253,  252,  430,  430,  430,  430,  430,
 /*  2330 */   171,  224,  228,  222,  244,  168,  430,  430,  217,  175,
 /*  2340 */   192,  430,  430,  430,  213,  430,  430,  430,  251,  253,
 /*  2350 */   252,  230,  430,  207,  430,  430,  171,  224,  228,  222,
 /*  2360 */   244,  168,  266,  204,  217,  430,  182,  430,  430,  430,
 /*  2370 */   430,  251,  253,  252,  251,  253,  252,  230,  430,  207,
 /*  2380 */   430,  223,  430,  430,  430,  430,  430,  238,  336,  251,
 /*  2390 */   253,  252,  430,  430,  430,  231,  232,  216,  233,  234,
 /*  2400 */   235,  236,  237,  430,  430,  251,  253,  252,  360,  430,
 /*  2410 */   430,  430,  430,  238,  430,  430,  251,  253,  252,  430,
 /*  2420 */   430,  231,  232,  188,  233,  234,  235,  236,  237,  175,
 /*  2430 */   180,  251,  253,  252,  213,  430,  430,  430,  251,  253,
 /*  2440 */   252,  430,  430,  430,  430,  430,  171,  224,  228,  222,
 /*  2450 */   244,  168,  430,  430,  217,  362,  175,  430,  430,  430,
 /*  2460 */   430,  213,  379,  430,  430,  430,  430,  230,  430,  207,
 /*  2470 */   251,  253,  252,  171,  224,  228,  222,  244,  168,  202,
 /*  2480 */   430,  217,   51,  430,  430,  430,  430,  251,  253,  252,
 /*  2490 */   430,  430,  430,  430,  230,  430,  207,  430,  430,  430,
 /*  2500 */   104,  430,   33,  238,  430,  430,  430,  430,  430,  430,
 /*  2510 */   430,  231,  232,  430,  233,  234,  235,  236,  237,   54,
 /*  2520 */    55,   56,   57,   58,   59,   60,   61,   62,   63,   64,
 /*  2530 */   238,  430,  430,  430,  430,   51,  430,  430,  231,  232,
 /*  2540 */   430,  233,  234,  235,  236,  237,  175,  430,  430,  430,
 /*  2550 */   430,  213,  430,  469,  430,   33,  430,  430,  430,  430,
 /*  2560 */   430,  430,  430,  171,  224,  228,  222,  244,  168,  430,
 /*  2570 */   430,  240,   54,   55,   56,   57,   58,   59,   60,   61,
 /*  2580 */    62,   63,   64,  430,  230,  430,  207,  430,  430,  430,
 /*  2590 */   430,  430,  430,  203,  205,  170,  179,  183,  185,  187,
 /*  2600 */   189,  181,  177,  191,  193,  201,  195,  197,  199,  430,
 /*  2610 */   430,  430,  430,  430,  430,  430,  430,  430,  430,  430,
 /*  2620 */   238,  430,  430,  430,  430,  438,  430,  430,  267,  268,
 /*  2630 */   430,  233,  234,  235,  236,  237,  430,  203,  205,  170,
 /*  2640 */   179,  183,  185,  187,  189,  181,  177,  191,  193,  201,
 /*  2650 */   195,  197,  199,  203,  205,  170,  179,  183,  185,  187,
 /*  2660 */   189,  181,  177,  191,  193,  201,  195,  197,  199,  290,
 /*  2670 */   170,  179,  183,  185,  187,  189,  181,  177,  191,  193,
 /*  2680 */   201,  195,  197,  199,  430,  433,  430,  430,  430,  430,
 /*  2690 */   430,  430,  430,  430,  203,  205,  170,  179,  183,  185,
 /*  2700 */   187,  189,  181,  177,  191,  193,  201,  195,  197,  199,
 /*  2710 */   203,  205,  170,  179,  183,  185,  187,  189,  181,  177,
 /*  2720 */   191,  193,  201,  195,  197,  199,  203,  205,  170,  179,
 /*  2730 */   183,  185,  187,  189,  181,  177,  191,  193,  201,  195,
 /*  2740 */   197,  199,  430,   67,   68,   70,   69,   71,  209,  203,
 /*  2750 */   205,  170,  179,  183,  185,  187,  189,  181,  177,  191,
 /*  2760 */   193,  201,  195,  197,  199,  430,  430,  430,  696,  430,
 /*  2770 */   430,  430,  430,  430,  498,  430,  430,  430,  430,  430,
 /*  2780 */    73,   74,  430,  430,  283,  430,  430,  430,  466,  430,
 /*  2790 */    72,  430,  430,  203,  205,  170,  179,  183,  185,  187,
 /*  2800 */   189,  181,  177,  191,  193,  201,  195,  197,  199,  430,
 /*  2810 */   430,  430,  430,  430,  430,  430,  430,  430,  361,  430,
 /*  2820 */   430,  430,  430,  262,  430,  430,  430,  203,  205,  170,
 /*  2830 */   179,  183,  185,  187,  189,  181,  177,  191,  193,  201,
 /*  2840 */   195,  197,  199,  203,  205,  170,  179,  183,  185,  187,
 /*  2850 */   189,  181,  177,  191,  193,  201,  195,  197,  199,  426,
 /*  2860 */    51,  430,  430,  430,  430,  430,  430,  430,  430,  430,
 /*  2870 */   430,   51,  430,  430,  430,  430,  430,  430,   26,  430,
 /*  2880 */    33,  430,   67,   68,   70,   69,   71,  430,  430,   80,
 /*  2890 */   430,   33,  430,  430,  430,  430,  430,   54,   55,   56,
 /*  2900 */    57,   58,   59,   60,   61,   62,   63,   64,   54,   55,
 /*  2910 */    56,   57,   58,   59,   60,   61,   62,   63,   64,   73,
 /*  2920 */    74,  430,  565,  496,  430,  490,  430,  430,  430,   72,
 /*  2930 */   430,   67,   68,   70,   69,   71,  430,  430,  430,  430,
 /*  2940 */   430,  430,  430,  430,  430,  430,  430,  430,  430,  430,
 /*  2950 */   430,  430,  430,  430,  430,  430,  430,  430,  430,  430,
 /*  2960 */   430,  430,  430,  430,  430,  430,  430,  430,   73,   74,
 /*  2970 */   430,   94,  496,  430,  490,  430,  430,  430,   72,  430,
 /*  2980 */    67,   68,   70,   69,   71,  430,  430,  430,  430,  430,
 /*  2990 */   430,  430,  430,  430,  430,  430,  430,  430,  430,  430,
 /*  3000 */   430,  430,  430,  430,  430,  430,  430,  430,  430,  430,
 /*  3010 */   430,  430,  430,  430,  430,  430,  430,   73,   74,  430,
 /*  3020 */   568,  484,  430,  490,  430,  430,  430,   72,  430,   67,
 /*  3030 */    68,   70,   69,   71,  430,  430,  430,  430,  430,  430,
 /*  3040 */   430,  430,  430,  430,  430,  430,  430,  430,  430,  430,
 /*  3050 */   430,  430,  430,  430,  430,  430,  430,  430,  430,  430,
 /*  3060 */   430,  430,  430,  430,  430,  430,   73,   74,  430,  564,
 /*  3070 */   496,  430,  490,  430,  430,  430,   72,  430,   67,   68,
 /*  3080 */    70,   69,   71,   67,   68,   70,   69,   71,  430,  430,
 /*  3090 */   430,  430,  430,  430,  430,  430,  430,  430,  430,  430,
 /*  3100 */   430,  430,  430,  430,  430,  430,  430,  430,  430,  430,
 /*  3110 */   430,  430,  430,  430,  430,   73,   74,  430,  572,  464,
 /*  3120 */    73,   74,  430,  569,  513,   72,  430,  430,  430,  430,
 /*  3130 */    72,  430,   67,   68,   70,   69,   71,  430,  430,  430,
 /*  3140 */   430,  430,  430,  430,  430,  430,  430,  430,  430,  430,
 /*  3150 */   430,  430,  430,  430,  430,  430,  430,  430,  430,  430,
 /*  3160 */   430,  430,  430,  504,  430,  430,  430,  430,  430,   73,
 /*  3170 */    74,   67,   68,   70,   69,   71,  430,  101,  430,   72,
 /*  3180 */   430,  430,  430,  430,  430,  430,  430,  430,  430,  430,
 /*  3190 */   430,  430,  430,  430,  430,  430,  430,  430,  430,  430,
 /*  3200 */   430,  430,  430,  430,  430,  430,  430,  430,   73,   74,
 /*  3210 */   430,   20,   75,   67,   68,   70,   69,   71,   72,  430,
 /*  3220 */   430,  430,  430,  430,  430,  430,  430,  430,  430,  430,
 /*  3230 */   430,  430,  430,  430,  430,  430,  430,  430,  430,  430,
 /*  3240 */   430,  430,  430,  430,  430,  430,  430,  430,  430,  430,
 /*  3250 */    73,   74,  430,  571,  464,   67,   68,   70,   69,   71,
 /*  3260 */    72,  430,  430,  430,  430,  430,  430,  430,  430,  430,
 /*  3270 */   430,  430,  430,  430,  430,  430,  430,  430,  430,  430,
 /*  3280 */   430,  430,  430,  430,  430,  430,  430,  430,  430,  430,
 /*  3290 */   430,  430,   73,   74,  430,  570,  464,   67,   68,   70,
 /*  3300 */    69,   71,   72,  430,  430,  430,  430,  430,  430,  430,
 /*  3310 */   430,  430,  430,  430,  430,  430,  430,  430,  430,  430,
 /*  3320 */   430,  430,  430,  430,  430,  430,  430,  430,  430,  430,
 /*  3330 */   430,  430,  430,  430,   73,   74,  430,   18,   75,   67,
 /*  3340 */    68,   70,   69,   71,   72,  430,  430,  430,  430,  430,
 /*  3350 */   430,  430,  430,  430,  430,  430,  430,  430,  430,  430,
 /*  3360 */   430,  430,  430,  430,  430,  430,  430,  430,  430,  430,
 /*  3370 */   430,  430,  430,  430,  430,  430,   73,   74,  430,  566,
 /*  3380 */   464,   67,   68,   70,   69,   71,   72,  430,  430,  430,
 /*  3390 */   430,  430,  430,  430,  430,  430,  430,  430,  430,  430,
 /*  3400 */   430,  430,  430,  430,  430,  430,  430,  430,  430,  430,
 /*  3410 */   430,  430,  430,  430,  430,  430,  430,  430,   73,   74,
 /*  3420 */   430,  567,  464,  430,  430,  430,  430,  430,   72,
};
static YYCODETYPE yy_lookahead[] = {
 /*     0 */     7,   21,   22,   23,   33,   12,   98,   99,  100,  101,
 /*    10 */   102,  103,  104,  105,   43,   41,  117,   24,   25,   26,
 /*    20 */    27,   28,   29,  109,   33,   32,  112,  113,  114,  115,
 /*    30 */   116,  121,  118,  119,  124,  125,  126,  123,   45,   65,
 /*    40 */    47,   67,   49,   50,   51,   52,   53,   54,   55,   56,
 /*    50 */    57,   58,   59,    8,    9,   10,   11,   12,   13,   14,
 /*    60 */    15,   16,   17,   18,   19,   20,   21,   22,   23,  112,
 /*    70 */   116,  114,  115,  116,   81,  118,  119,  123,   33,   35,
 /*    80 */   123,   32,   89,   90,   40,   92,   93,   94,   95,   96,
 /*    90 */    41,   42,    1,    2,    3,    4,    5,   33,   49,   50,
 /*   100 */    51,   52,   53,   54,   55,   56,   57,   43,   59,   60,
 /*   110 */    61,   62,  151,   64,  124,  125,  126,   68,   69,   70,
 /*   120 */    71,  116,  121,   74,   32,  124,  125,  126,  123,   38,
 /*   130 */    39,   41,   32,   41,   42,   86,   87,   88,   40,   48,
 /*   140 */    91,   49,   50,   51,   52,   53,   54,   55,   56,   57,
 /*   150 */    47,   59,   60,   61,   62,   65,   64,   67,  159,  160,
 /*   160 */    68,   69,   70,   71,  151,   47,   74,   32,   89,   90,
 /*   170 */    32,   92,   93,   94,   95,   96,   41,   42,   86,   87,
 /*   180 */    88,  159,  160,   91,   49,   50,   51,   52,   53,   54,
 /*   190 */    55,   56,   57,   32,   59,   60,   61,   62,   80,   64,
 /*   200 */   117,   83,   69,   68,   69,   70,   71,  149,  150,   74,
 /*   210 */    32,   18,   19,   20,   21,   22,   23,    6,    6,   41,
 /*   220 */    42,   86,   87,   88,   43,  110,   91,   49,   50,   51,
 /*   230 */    52,   53,   54,   55,   56,   57,  116,   59,   60,   61,
 /*   240 */    62,   30,   64,  123,    6,   33,   68,   69,   70,   71,
 /*   250 */    89,   90,   74,   32,  159,  160,   75,   76,   77,   78,
 /*   260 */    79,    6,   41,   42,   86,   87,   88,    6,   30,   91,
 /*   270 */    49,   50,   51,   52,   53,   54,   55,   56,   57,   32,
 /*   280 */    59,   60,   61,   62,   32,   64,  159,  160,   33,   68,
 /*   290 */    69,   70,   71,  148,   33,   74,   32,   45,    6,  154,
 /*   300 */   155,  156,  157,  158,    6,   41,   42,   86,   87,   88,
 /*   310 */    80,   81,   91,   49,   50,   51,   52,   53,   54,   55,
 /*   320 */    56,   57,   35,   59,   60,   61,   62,   40,   64,  159,
 /*   330 */   160,   33,   68,   69,   70,   71,  148,  161,   74,   32,
 /*   340 */   152,  146,  147,    6,  156,  157,  158,  107,   41,   42,
 /*   350 */    86,   87,   88,  159,  160,   91,   49,   50,   51,   52,
 /*   360 */    53,   54,   55,   56,   57,  116,   59,   60,   61,   62,
 /*   370 */    33,   64,  123,    6,   82,   68,   69,   70,   71,  148,
 /*   380 */   121,   74,   32,  124,  125,  126,  155,  156,  157,  158,
 /*   390 */     6,   41,   42,   86,   87,   88,   32,   30,   91,   49,
 /*   400 */    50,   51,   52,   53,   54,   55,   56,   57,   32,   59,
 /*   410 */    60,   61,   62,  108,   64,  110,    6,    6,   68,   69,
 /*   420 */    70,   71,  151,  148,   74,   32,  101,  102,  103,  104,
 /*   430 */   105,  156,  157,  158,   41,   42,   86,   87,   88,  164,
 /*   440 */    30,   91,   49,   50,   51,   52,   53,   54,   55,   56,
 /*   450 */    57,   40,   59,   60,   61,   62,   72,   64,   36,    6,
 /*   460 */    33,   68,   69,   70,   71,  148,   35,   74,   32,  116,
 /*   470 */   153,   40,  119,  156,  157,  158,  123,   41,   42,   86,
 /*   480 */    87,   88,    6,   30,   91,   49,   50,   51,   52,   53,
 /*   490 */    54,   55,   56,   57,   32,   59,   60,   61,   62,    6,
 /*   500 */    64,  159,  160,   32,   68,   69,   70,   71,  148,   33,
 /*   510 */    74,   32,  107,  153,   32,    6,  156,  157,  158,   32,
 /*   520 */    41,   42,   86,   87,   88,    6,   33,   91,   49,   50,
 /*   530 */    51,   52,   53,   54,   55,   56,   57,   40,   59,   60,
 /*   540 */    61,   62,   33,   64,  107,    6,   40,   68,   69,   70,
 /*   550 */    71,  148,   33,   74,   32,  152,    6,  159,  160,  156,
 /*   560 */   157,  158,    6,   41,   42,   86,   87,   88,    6,   30,
 /*   570 */    91,   49,   50,   51,   52,   53,   54,   55,   56,   57,
 /*   580 */    30,   59,   60,   61,   62,    6,   64,  159,  160,   33,
 /*   590 */    68,   69,   70,   71,  148,   33,   74,   32,   32,  153,
 /*   600 */   159,  160,  156,  157,  158,   36,   41,   42,   86,   87,
 /*   610 */    88,  107,   33,   91,   49,   50,   51,   52,   53,   54,
 /*   620 */    55,   56,   57,  150,   59,   60,   61,   62,   43,   64,
 /*   630 */    32,    6,   32,   68,   69,   70,   71,  148,  107,   74,
 /*   640 */    32,  152,   32,    6,   81,  156,  157,  158,   32,   41,
 /*   650 */    42,   86,   87,   88,  107,   30,   91,   49,   50,   51,
 /*   660 */    52,   53,   54,   55,   56,   57,  161,   59,   60,   61,
 /*   670 */    62,   33,   64,   40,   33,   32,   68,   69,   70,   71,
 /*   680 */   148,   33,   74,   32,  152,  107,  106,    0,  156,  157,
 /*   690 */   158,   32,   41,   42,   86,   87,   88,   47,   40,   91,
 /*   700 */    49,   50,   51,   52,   53,   54,   55,   56,   57,   32,
 /*   710 */    59,   60,   61,   62,   47,   64,   32,   47,   32,   68,
 /*   720 */    69,   70,   71,  148,  117,   74,   32,  152,   32,  123,
 /*   730 */   160,  156,  157,  158,   43,   41,   42,   86,   87,   88,
 /*   740 */    32,   63,   91,   49,   50,   51,   52,   53,   54,   55,
 /*   750 */    56,   57,   32,   59,   60,   61,   62,   72,   64,   40,
 /*   760 */    33,   33,   68,   69,   70,   71,  148,   13,   74,   32,
 /*   770 */   152,   33,   32,   32,  156,  157,  158,   82,   41,   42,
 /*   780 */    86,   87,   88,   47,   33,   91,   49,   50,   51,   52,
 /*   790 */    53,   54,   55,   56,   57,   47,   59,   60,   61,   62,
 /*   800 */    69,   64,   66,  106,   82,   68,   69,   70,   71,   33,
 /*   810 */   121,   74,   32,  124,  125,  126,   80,   81,   30,   83,
 /*   820 */    33,   41,   42,   86,   87,   88,   33,   43,   91,   49,
 /*   830 */    50,   51,   52,   53,   54,   55,   56,   57,   66,   59,
 /*   840 */    60,   61,   62,   41,   64,   82,   33,   32,   68,   69,
 /*   850 */    70,   71,  148,   32,   74,   32,  152,   63,   40,   40,
 /*   860 */   156,  157,  158,  117,   41,   42,   86,   87,   88,   47,
 /*   870 */   117,   91,   49,   50,   51,   52,   53,   54,   55,   56,
 /*   880 */    57,   40,   59,   60,   61,   62,  147,   64,   33,   32,
 /*   890 */    32,   68,   69,   70,   71,  148,   33,   74,   32,  152,
 /*   900 */    33,   47,   40,  156,  157,  158,  117,   41,   42,   86,
 /*   910 */    87,   88,   66,   43,   91,   49,   50,   51,   52,   53,
 /*   920 */    54,   55,   56,   57,   33,   59,   60,   61,   62,   40,
 /*   930 */    64,   32,  151,   82,   68,   69,   70,   71,  148,   13,
 /*   940 */    74,   32,  152,  151,   40,   45,  156,  157,  158,  151,
 /*   950 */    41,   42,   86,   87,   88,   43,   66,   91,   49,   50,
 /*   960 */    51,   52,   53,   54,   55,   56,   57,   82,   59,   60,
 /*   970 */    61,   62,   33,   64,   66,   45,  117,   68,   69,   70,
 /*   980 */    71,  148,  151,   74,   32,   43,   32,  151,  117,  156,
 /*   990 */   157,  158,   33,   41,   42,   86,   87,   88,   32,   32,
 /*  1000 */    91,   49,   50,   51,   52,   53,   54,   55,   56,   57,
 /*  1010 */    40,   59,   60,   61,   62,   32,   64,   33,  117,   66,
 /*  1020 */    68,   69,   70,   71,  148,  151,   74,   32,   82,  168,
 /*  1030 */   168,  168,  156,  157,  158,  168,   41,   42,   86,   87,
 /*  1040 */    88,  168,  168,   91,   49,   50,   51,   52,   53,   54,
 /*  1050 */    55,   56,   57,  168,   59,   60,   61,   62,  168,   64,
 /*  1060 */   168,  168,  168,   68,   69,   70,   71,  148,  168,   74,
 /*  1070 */    32,  168,  168,  168,  168,  156,  157,  158,  168,   41,
 /*  1080 */    42,   86,   87,   88,  168,  168,   91,   49,   50,   51,
 /*  1090 */    52,   53,   54,   55,   56,   57,  168,   59,   60,   61,
 /*  1100 */    62,  168,   64,  168,  168,  111,   68,   69,   70,   71,
 /*  1110 */   116,  168,   74,   32,  120,  168,  168,  123,  168,   12,
 /*  1120 */   168,  168,  168,   42,   86,   87,   88,  168,  168,   91,
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
 /*  1310 */   140,  141,  142,  143,  144,  148,  105,  168,  168,  168,
 /*  1320 */   168,  168,  168,  156,  157,  158,  156,  157,  158,  168,
 /*  1330 */   163,  164,  168,  122,  168,  168,  168,  168,  127,  128,
 /*  1340 */   129,  130,  131,  132,  133,  134,  135,  136,  137,  138,
 /*  1350 */   139,  140,  141,  142,  143,  144,  148,  168,  105,  168,
 /*  1360 */   168,  168,  168,  168,  156,  157,  158,  156,  157,  158,
 /*  1370 */    43,  163,  164,  168,   35,  122,   37,  168,  168,   40,
 /*  1380 */   127,  128,  129,  130,  131,  132,  133,  134,  135,  136,
 /*  1390 */   137,  138,  139,  140,  141,  142,  143,  144,  105,  168,
 /*  1400 */   168,  168,   75,   76,   77,   78,   79,  168,   81,  156,
 /*  1410 */   157,  158,  168,  168,  168,  122,  168,  168,  168,  168,
 /*  1420 */   127,  128,  129,  130,  131,  132,  133,  134,  135,  136,
 /*  1430 */   137,  138,  139,  140,  141,  142,  143,  144,  168,  105,
 /*  1440 */   168,  168,  168,  114,  115,  116,  168,  168,  119,  156,
 /*  1450 */   157,  158,  123,  168,  168,  168,  122,  168,  168,  168,
 /*  1460 */   168,  127,  128,  129,  130,  131,  132,  133,  134,  135,
 /*  1470 */   136,  137,  138,  139,  140,  141,  142,  143,  144,  148,
 /*  1480 */   168,  105,  168,  168,  168,  168,  168,  156,  157,  158,
 /*  1490 */   156,  157,  158,   47,  163,  164,  168,  168,  122,  168,
 /*  1500 */   168,  168,  168,  127,  128,  129,  130,  131,  132,  133,
 /*  1510 */   134,  135,  136,  137,  138,  139,  140,  141,  142,  143,
 /*  1520 */   144,  105,  168,  168,  168,  168,   80,   81,  145,   83,
 /*  1530 */   168,  148,  156,  157,  158,  168,  168,  168,  122,  156,
 /*  1540 */   157,  158,  168,  127,  128,  129,  130,  131,  132,  133,
 /*  1550 */   134,  135,  136,  137,  138,  139,  140,  141,  142,  143,
 /*  1560 */   144,  168,  105,  168,  168,  168,  168,  168,  168,  168,
 /*  1570 */   168,  168,  156,  157,  158,  168,  168,  168,  168,  122,
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
 /*  1730 */   145,  168,  168,  148,  168,  168,  156,  157,  158,  168,
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
 /*  1850 */   168,  168,  168,   12,  168,  168,  168,  168,  168,  156,
 /*  1860 */   157,  158,  168,  168,  168,   24,   25,   26,   27,   28,
 /*  1870 */    29,  168,  168,   32,    7,   43,  168,  168,  168,   12,
 /*  1880 */   168,    1,    2,    3,    4,    5,   45,  168,   47,  168,
 /*  1890 */   168,   24,   25,   26,   27,   28,   29,  168,  168,   32,
 /*  1900 */   168,  168,  168,  168,  168,  168,  168,   75,   76,   77,
 /*  1910 */    78,   79,   45,   81,   47,  168,   84,   85,   38,   39,
 /*  1920 */   125,  168,   81,   82,  168,  168,   46,  168,   48,  168,
 /*  1930 */    89,   90,  168,   92,   93,   94,   95,   96,   43,   31,
 /*  1940 */   168,  168,   34,  148,   36,  168,   38,   39,   81,   82,
 /*  1950 */    42,  156,  157,  158,  168,  168,   89,   90,  168,   92,
 /*  1960 */    93,   94,   95,   96,    7,  168,  168,  168,  168,   12,
 /*  1970 */    75,   76,   77,   78,   79,   80,   81,  168,   83,   84,
 /*  1980 */    85,   24,   25,   26,   27,   28,   29,    0,  168,   32,
 /*  1990 */     7,  168,  168,  168,  168,   12,  168,    1,    2,    3,
 /*  2000 */     4,    5,   45,  168,   47,  168,  168,   24,   25,   26,
 /*  2010 */    27,   28,   29,   30,  168,   32,  168,  168,   31,  168,
 /*  2020 */   168,   34,  168,   36,  168,   38,   39,  168,   45,   42,
 /*  2030 */    47,  168,  168,  168,   38,   39,  168,  168,   81,   82,
 /*  2040 */   168,  168,   46,  168,   48,  168,   89,   90,  148,   92,
 /*  2050 */    93,   94,   95,   96,  168,  168,  156,  157,  158,  168,
 /*  2060 */   148,  168,  162,  168,   81,  165,  166,  167,  156,  157,
 /*  2070 */   158,  168,   89,   90,  168,   92,   93,   94,   95,   96,
 /*  2080 */     7,  168,  148,  168,  168,   12,  148,  168,  168,  168,
 /*  2090 */   156,  157,  158,  168,  156,  157,  158,   24,   25,   26,
 /*  2100 */    27,   28,   29,   30,  168,   32,    7,  168,  168,  168,
 /*  2110 */   168,   12,  168,    1,    2,    3,    4,    5,   45,  168,
 /*  2120 */    47,  168,  168,   24,   25,   26,   27,   28,   29,   30,
 /*  2130 */   148,   32,  168,  148,  168,  168,  168,  168,  156,  157,
 /*  2140 */   158,  156,  157,  158,   45,  168,   47,  165,  166,  167,
 /*  2150 */    38,   39,  167,  168,   81,  168,  168,  168,   46,  168,
 /*  2160 */    48,  168,   89,   90,  148,   92,   93,   94,   95,   96,
 /*  2170 */   168,  168,  156,  157,  158,  168,  168,  168,  168,  168,
 /*  2180 */    81,  168,    1,    2,    3,    4,    5,  168,   89,   90,
 /*  2190 */   168,   92,   93,   94,   95,   96,    7,  148,  168,  168,
 /*  2200 */   148,   12,  168,  168,  168,  156,  157,  158,  156,  157,
 /*  2210 */   158,  168,  168,   24,   25,   26,   27,   28,   29,   38,
 /*  2220 */    39,   32,  148,    7,  168,  168,  168,   46,   12,   48,
 /*  2230 */   156,  157,  158,  168,   45,  168,   47,  163,  164,  168,
 /*  2240 */    24,   25,   26,   27,   28,   29,  148,  168,   32,  148,
 /*  2250 */   168,  168,  168,  168,  156,  157,  158,  156,  157,  158,
 /*  2260 */   168,   45,   73,   47,  148,  168,  168,  148,  168,  168,
 /*  2270 */    81,  168,  156,  157,  158,  156,  157,  158,   89,   90,
 /*  2280 */   168,   92,   93,   94,   95,   96,  168,  168,  168,   73,
 /*  2290 */   148,  168,  168,  148,  168,  168,  168,   81,  156,  157,
 /*  2300 */   158,  156,  157,  158,  168,   89,   90,  148,   92,   93,
 /*  2310 */    94,   95,   96,    7,  148,  156,  157,  158,   12,  168,
 /*  2320 */   168,  168,  156,  157,  158,  168,  168,  168,  168,  168,
 /*  2330 */    24,   25,   26,   27,   28,   29,  168,  168,   32,    7,
 /*  2340 */   148,  168,  168,  168,   12,  168,  168,  168,  156,  157,
 /*  2350 */   158,   45,  168,   47,  168,  168,   24,   25,   26,   27,
 /*  2360 */    28,   29,   30,  148,   32,  168,  148,  168,  168,  168,
 /*  2370 */   168,  156,  157,  158,  156,  157,  158,   45,  168,   47,
 /*  2380 */   168,  148,  168,  168,  168,  168,  168,   81,   82,  156,
 /*  2390 */   157,  158,  168,  168,  168,   89,   90,  148,   92,   93,
 /*  2400 */    94,   95,   96,  168,  168,  156,  157,  158,  148,  168,
 /*  2410 */   168,  168,  168,   81,  168,  168,  156,  157,  158,  168,
 /*  2420 */   168,   89,   90,  148,   92,   93,   94,   95,   96,    7,
 /*  2430 */   148,  156,  157,  158,   12,  168,  168,  168,  156,  157,
 /*  2440 */   158,  168,  168,  168,  168,  168,   24,   25,   26,   27,
 /*  2450 */    28,   29,  168,  168,   32,   33,    7,  168,  168,  168,
 /*  2460 */   168,   12,  148,  168,  168,  168,  168,   45,  168,   47,
 /*  2470 */   156,  157,  158,   24,   25,   26,   27,   28,   29,  148,
 /*  2480 */   168,   32,   12,  168,  168,  168,  168,  156,  157,  158,
 /*  2490 */   168,  168,  168,  168,   45,  168,   47,  168,  168,  168,
 /*  2500 */    30,  168,   32,   81,  168,  168,  168,  168,  168,  168,
 /*  2510 */   168,   89,   90,  168,   92,   93,   94,   95,   96,   49,
 /*  2520 */    50,   51,   52,   53,   54,   55,   56,   57,   58,   59,
 /*  2530 */    81,  168,  168,  168,  168,   12,  168,  168,   89,   90,
 /*  2540 */   168,   92,   93,   94,   95,   96,    7,  168,  168,  168,
 /*  2550 */   168,   12,  168,   30,  168,   32,  168,  168,  168,  168,
 /*  2560 */   168,  168,  168,   24,   25,   26,   27,   28,   29,  168,
 /*  2570 */   168,   32,   49,   50,   51,   52,   53,   54,   55,   56,
 /*  2580 */    57,   58,   59,  168,   45,  168,   47,  168,  168,  168,
 /*  2590 */   168,  168,  168,    8,    9,   10,   11,   12,   13,   14,
 /*  2600 */    15,   16,   17,   18,   19,   20,   21,   22,   23,  168,
 /*  2610 */   168,  168,  168,  168,  168,  168,  168,  168,  168,  168,
 /*  2620 */    81,  168,  168,  168,  168,   40,  168,  168,   89,   90,
 /*  2630 */   168,   92,   93,   94,   95,   96,  168,    8,    9,   10,
 /*  2640 */    11,   12,   13,   14,   15,   16,   17,   18,   19,   20,
 /*  2650 */    21,   22,   23,    8,    9,   10,   11,   12,   13,   14,
 /*  2660 */    15,   16,   17,   18,   19,   20,   21,   22,   23,   40,
 /*  2670 */    10,   11,   12,   13,   14,   15,   16,   17,   18,   19,
 /*  2680 */    20,   21,   22,   23,  168,   40,  168,  168,  168,  168,
 /*  2690 */   168,  168,  168,  168,    8,    9,   10,   11,   12,   13,
 /*  2700 */    14,   15,   16,   17,   18,   19,   20,   21,   22,   23,
 /*  2710 */     8,    9,   10,   11,   12,   13,   14,   15,   16,   17,
 /*  2720 */    18,   19,   20,   21,   22,   23,    8,    9,   10,   11,
 /*  2730 */    12,   13,   14,   15,   16,   17,   18,   19,   20,   21,
 /*  2740 */    22,   23,  168,    1,    2,    3,    4,    5,   30,    8,
 /*  2750 */     9,   10,   11,   12,   13,   14,   15,   16,   17,   18,
 /*  2760 */    19,   20,   21,   22,   23,  168,  168,  168,   82,  168,
 /*  2770 */   168,  168,  168,  168,   32,  168,  168,  168,  168,  168,
 /*  2780 */    38,   39,  168,  168,   82,  168,  168,  168,   46,  168,
 /*  2790 */    48,  168,  168,    8,    9,   10,   11,   12,   13,   14,
 /*  2800 */    15,   16,   17,   18,   19,   20,   21,   22,   23,  168,
 /*  2810 */   168,  168,  168,  168,  168,  168,  168,  168,   33,  168,
 /*  2820 */   168,  168,  168,   82,  168,  168,  168,    8,    9,   10,
 /*  2830 */    11,   12,   13,   14,   15,   16,   17,   18,   19,   20,
 /*  2840 */    21,   22,   23,    8,    9,   10,   11,   12,   13,   14,
 /*  2850 */    15,   16,   17,   18,   19,   20,   21,   22,   23,   40,
 /*  2860 */    12,  168,  168,  168,  168,  168,  168,  168,  168,  168,
 /*  2870 */   168,   12,  168,  168,  168,  168,  168,  168,   30,  168,
 /*  2880 */    32,  168,    1,    2,    3,    4,    5,  168,  168,   30,
 /*  2890 */   168,   32,  168,  168,  168,  168,  168,   49,   50,   51,
 /*  2900 */    52,   53,   54,   55,   56,   57,   58,   59,   49,   50,
 /*  2910 */    51,   52,   53,   54,   55,   56,   57,   58,   59,   38,
 /*  2920 */    39,  168,   41,   42,  168,   44,  168,  168,  168,   48,
 /*  2930 */   168,    1,    2,    3,    4,    5,  168,  168,  168,  168,
 /*  2940 */   168,  168,  168,  168,  168,  168,  168,  168,  168,  168,
 /*  2950 */   168,  168,  168,  168,  168,  168,  168,  168,  168,  168,
 /*  2960 */   168,  168,  168,  168,  168,  168,  168,  168,   38,   39,
 /*  2970 */   168,   41,   42,  168,   44,  168,  168,  168,   48,  168,
 /*  2980 */     1,    2,    3,    4,    5,  168,  168,  168,  168,  168,
 /*  2990 */   168,  168,  168,  168,  168,  168,  168,  168,  168,  168,
 /*  3000 */   168,  168,  168,  168,  168,  168,  168,  168,  168,  168,
 /*  3010 */   168,  168,  168,  168,  168,  168,  168,   38,   39,  168,
 /*  3020 */    41,   42,  168,   44,  168,  168,  168,   48,  168,    1,
 /*  3030 */     2,    3,    4,    5,  168,  168,  168,  168,  168,  168,
 /*  3040 */   168,  168,  168,  168,  168,  168,  168,  168,  168,  168,
 /*  3050 */   168,  168,  168,  168,  168,  168,  168,  168,  168,  168,
 /*  3060 */   168,  168,  168,  168,  168,  168,   38,   39,  168,   41,
 /*  3070 */    42,  168,   44,  168,  168,  168,   48,  168,    1,    2,
 /*  3080 */     3,    4,    5,    1,    2,    3,    4,    5,  168,  168,
 /*  3090 */   168,  168,  168,  168,  168,  168,  168,  168,  168,  168,
 /*  3100 */   168,  168,  168,  168,  168,  168,  168,  168,  168,  168,
 /*  3110 */   168,  168,  168,  168,  168,   38,   39,  168,   41,   42,
 /*  3120 */    38,   39,  168,   41,   42,   48,  168,  168,  168,  168,
 /*  3130 */    48,  168,    1,    2,    3,    4,    5,  168,  168,  168,
 /*  3140 */   168,  168,  168,  168,  168,  168,  168,  168,  168,  168,
 /*  3150 */   168,  168,  168,  168,  168,  168,  168,  168,  168,  168,
 /*  3160 */   168,  168,  168,   32,  168,  168,  168,  168,  168,   38,
 /*  3170 */    39,    1,    2,    3,    4,    5,  168,   46,  168,   48,
 /*  3180 */   168,  168,  168,  168,  168,  168,  168,  168,  168,  168,
 /*  3190 */   168,  168,  168,  168,  168,  168,  168,  168,  168,  168,
 /*  3200 */   168,  168,  168,  168,  168,  168,  168,  168,   38,   39,
 /*  3210 */   168,   41,   42,    1,    2,    3,    4,    5,   48,  168,
 /*  3220 */   168,  168,  168,  168,  168,  168,  168,  168,  168,  168,
 /*  3230 */   168,  168,  168,  168,  168,  168,  168,  168,  168,  168,
 /*  3240 */   168,  168,  168,  168,  168,  168,  168,  168,  168,  168,
 /*  3250 */    38,   39,  168,   41,   42,    1,    2,    3,    4,    5,
 /*  3260 */    48,  168,  168,  168,  168,  168,  168,  168,  168,  168,
 /*  3270 */   168,  168,  168,  168,  168,  168,  168,  168,  168,  168,
 /*  3280 */   168,  168,  168,  168,  168,  168,  168,  168,  168,  168,
 /*  3290 */   168,  168,   38,   39,  168,   41,   42,    1,    2,    3,
 /*  3300 */     4,    5,   48,  168,  168,  168,  168,  168,  168,  168,
 /*  3310 */   168,  168,  168,  168,  168,  168,  168,  168,  168,  168,
 /*  3320 */   168,  168,  168,  168,  168,  168,  168,  168,  168,  168,
 /*  3330 */   168,  168,  168,  168,   38,   39,  168,   41,   42,    1,
 /*  3340 */     2,    3,    4,    5,   48,  168,  168,  168,  168,  168,
 /*  3350 */   168,  168,  168,  168,  168,  168,  168,  168,  168,  168,
 /*  3360 */   168,  168,  168,  168,  168,  168,  168,  168,  168,  168,
 /*  3370 */   168,  168,  168,  168,  168,  168,   38,   39,  168,   41,
 /*  3380 */    42,    1,    2,    3,    4,    5,   48,  168,  168,  168,
 /*  3390 */   168,  168,  168,  168,  168,  168,  168,  168,  168,  168,
 /*  3400 */   168,  168,  168,  168,  168,  168,  168,  168,  168,  168,
 /*  3410 */   168,  168,  168,  168,  168,  168,  168,  168,   38,   39,
 /*  3420 */   168,   41,   42,  168,  168,  168,  168,  168,   48,
};
#define YY_SHIFT_USE_DFLT (-30)
static short yy_shift_ofst[] = {
 /*     0 */  1908,  687, 1987,  -30,  -30,  -30,  -30,  -30,  954,  728,
 /*    10 */   -30,  376,   44,  -30,  686,  633,  -30, 3296,  -30, 3170,
 /*    20 */   -30,  -30, 2181,  696,  650, 2848,  638,  -30,  625,  891,
 /*    30 */   -30, 1107,  -30,  870,   79,  -30,  -30,  -30,  -30,  -30,
 /*    40 */   -30,  -30,  -30,  659,  784,   79,  -30,  100,  585,   79,
 /*    50 */   -30,  967,  926,  899,  -30,  -30,  -30,  -30,  -30,  -30,
 /*    60 */   -30,  -30,  -30,  -30,  -30,  -30,  -30,  -30,  -30,  -30,
 /*    70 */   -30,  -30,  -30,  -30,  -30,   91, 1996,  857,  822, 2859,
 /*    80 */   793,  -30,  539,  727,  -30,  -30,  -30,  616, 1339,  -30,
 /*    90 */   643,  497,  -30, 2930,  -30,  802,  -30, 3028, 3380,  -30,
 /*   100 */  2112,  983,  103, 2470,  506,  135,  -30,  479,  -30,  -30,
 /*   110 */   -30,  -30,  -30,  -30,  -30,  -30,  -30,  -30,  -30,  -30,
 /*   120 */   -30,  -30,  -30,  -30,  -30,  -30,  -30,  -30,  641,  -30,
 /*   130 */   648,  -30, 2449,  658,  264,  221,  678,  719, 1038,  780,
 /*   140 */   -30, 2449,  818,   90,  -30,  -26,  -30,  -30,   79,  846,
 /*   150 */  1081, 1081,  904,  866,  952,  -30, 2449,  970,  178,   92,
 /*   160 */   -30,   98,  694,  522,  133, 2449,  427,  -30, 2449,  -30,
 /*   170 */  2449,  471,  -30,  563, 2449, 2449, 2835, 2449,  193, 2449,
 /*   180 */   193, 2449,  193, 2449,  193, 2449,  193, 2449,  193, 2449,
 /*   190 */   193, 2449,  -20, 2449,  -20, 2449,  -30, 2449,  -30, 2449,
 /*   200 */   -30, 2449,  -20, 2449, 2660, 2449, 2660,   -7, 2718,  -30,
 /*   210 */   788, 2449,  -30,  741,  754, 2449,  193, 1446,  708,  670,
 /*   220 */  1983, 2835, 2449,  -30,  610,  637,  471,  -30, 2449,  -30,
 /*   230 */   -30,  -30,  -30,  -30,  -30,  -30,  -30,  -30, 1841, 2835,
 /*   240 */   736,  252,  667, 2073,  740,  748, 2099,  -30,  238,  -30,
 /*   250 */  2449,  -30,  -30,  -30,  -30,  -30,  453,  -30,  -30,  -30,
 /*   260 */  2449, 2741,  -30, 2332,  211,  -30,  -30,  890,  908,  -30,
 /*   270 */   292,  -30, 2539,  -30,  953, 2449,  -30,  -30,  -30,  410,
 /*   280 */   -30,  -30, 2702,  -30,  193, 2835,  482,  384, 2189, 2629,
 /*   290 */  1081,  909,  -30,  600,  493,  -30,  600,  -30, 1895,  -30,
 /*   300 */   -30,  -30,  -30,  -30,  -30, 2449,  -30, 2835,  138, 1832,
 /*   310 */  2449,  -30, 1867,  181, 2449,  -30,  946,  181, 2449,  -30,
 /*   320 */  2686,  -30,  -30,  966, 1327, 2449,  -30, 1957,  181, 2449,
 /*   330 */   -30,  885,  181, 2449,  -30, 2306,  181, 2449,  -30,  851,
 /*   340 */   181, 2449,  -30,  -30,  -30,  -30, 2449,  556,  -30, 2449,
 /*   350 */   -30, 2835,  -30,  867,  -30,  863,  -30,  855,  -30, 2422,
 /*   360 */  2785,  -30,  -30,  821,  230,  815,  813,  -30,  161,  763,
 /*   370 */   787,  -30,  722,  751,  -30,  695,  738,  -30, 2449,   45,
 /*   380 */   -30,  364,  298,  -30,  364,  -30,  691,   79,  -30,  -30,
 /*   390 */   364,  476,  -30,  364,  579,  -30,  364,  261,  -30,  364,
 /*   400 */   519,  -30,  364,  509,  -30,  364,  212,  -30,  364,  337,
 /*   410 */   -30,  364,  562,  -30,  364,  255,  -30,  118,  677,  667,
 /*   420 */   684,  670,  -30,  -30, 2449, 2819, 1081,  350,  -30,  720,
 /*   430 */   685, 2216, 2645, 1081,  823,  -30, 2449, 2585, 1081,   49,
 /*   440 */   -30,  731, 2449,  776,  -30,  -30,  -30,  772, 1081, 1081,
 /*   450 */   -30,  -30,  794,  819,  307,  995,  -30,  -30,  367,  841,
 /*   460 */   436,  -30,  608,  -30,   91, 1880,  858,  854, 2523,  862,
 /*   470 */   393,  -30,  565,  -30,  550,  889,  651,  -30,  737,  -30,
 /*   480 */  2979, 3212,  -30,  -30, 1246,  900,  912,   79,  939,  -30,
 /*   490 */   930,  942,   79,  959,  -30,  -30, 1246, 2742,  -29,  -30,
 /*   500 */    79,  984,  -30, 3131,   64,  -30,   79,   -9,  -30,  -30,
 /*   510 */  2881, 3082, 3077,   91, 3254,  -30, 3338,  247,  411,  -30,
 /*   520 */   247,  -30,  -30,  -30,  422,  462,  287,  -30,  487,  497,
 /*   530 */   -30,  569,  566,  431,  -30,  598,  497,  -30,  -30,
};
#define YY_REDUCE_USE_DFLT (-102)
static short yy_reduce_ofst[] = {
 /*     0 */   -92, -102,  325, -102, -102, -102, -102, -102, -102, -102,
 /*    10 */  -102, -102,  697, -102, -102,  580, -102,  994, -102, 1160,
 /*    20 */  -102, -102,  606, -102, -102,  689, -102, -102, -102, -102,
 /*    30 */  -102,  -10, -102, -102,  746, -102, -102, -102, -102, -102,
 /*    40 */  -102, -102, -102, -102, -102,  753, -102, -102, -102,   83,
 /*    50 */  -102, -102, -102, -102, -102, -102, -102, -102, -102, -102,
 /*    60 */  -102, -102, -102, -102, -102, -102, -102, -102, -102, -102,
 /*    70 */  -102, -102, -102, -102, -102,  249,  606, -102, -102,  -90,
 /*    80 */  -102, -102, -102, -102, -102, -102, -102, -102,  547, -102,
 /*    90 */  -102,  578, -102,  -86, -102, -102, -102, 1084,  353, -102,
 /*   100 */   606, -102, -102,  259, -102, 1622, -102, 1703, -102, -102,
 /*   110 */  -102, -102, -102, -102, -102, -102, -102, -102, -102, -102,
 /*   120 */  -102, -102, -102, -102, -102, -102, -102, -102, -102, -102,
 /*   130 */  -102, -102, 1383, -102, 1088, 1703, -102, -102, 1376, 1703,
 /*   140 */  -102, 1462, -102,  195, -102,  739, -102, -102,  789, -102,
 /*   150 */  1662, 1703, -102, 1130, 1703, -102, 1542, -102, 1580, 1703,
 /*   160 */  -102, -102, 1334, 1703, -102, 1506, -102, -102, 1912, -102,
 /*   170 */  2052,  176, -102, -102,  833,  919, -102,  876, -102, 2282,
 /*   180 */  -102, 2218, -102, 2159, -102, 2016, -102, 2275, -102, 2119,
 /*   190 */  -102, 2192, -102, 2145, -102, 2049, -102, 1934, -102, 1665,
 /*   200 */  -102, 2331, -102, 2215, -102, 2166, -102, 1795, -102, -102,
 /*   210 */  -102, 2101, -102, -102, -102, 2249, -102, -102, -102, -102,
 /*   220 */  1167, -102, 2233, -102, -102, -102,  505, -102, 1938, -102,
 /*   230 */  -102, -102, -102, -102, -102, -102, -102, -102, 1900, -102,
 /*   240 */  -102, -102, -102, 1208, -102, -102, 1331, -102, -102, -102,
 /*   250 */   275, -102, -102, -102, -102, -102, -102, -102, -102, -102,
 /*   260 */  2116, -102, -102, 2074, -102, -102, -102, -102, -102, -102,
 /*   270 */  -102, -102, 1982, -102, -102, 1985, -102, -102, -102, -102,
 /*   280 */  -102, -102, -102, -102, -102, -102, -102, -102, 2098, -102,
 /*   290 */  1253, 1703, -102,   58, -102, -102,  473, -102,  271, -102,
 /*   300 */  -102, -102, -102, -102, -102,  747, -102, -102, -102,   13,
 /*   310 */   575, -102,  317,  -39,  532, -102, -102,  874,  188, -102,
 /*   320 */  -102, -102, -102, -102,  836,  704, -102,  446,  831,  618,
 /*   330 */  -102, -102,  798,  403, -102,  360,  792,  790, -102, -102,
 /*   340 */   781,  489, -102, -102, -102, -102,  145, -102, -102,  231,
 /*   350 */  -102, -102, -102, -102, -102, -102, -102, -102, -102, 2260,
 /*   360 */  -102, -102, -102, -102, -102, -102, -102, -102, -102, -102,
 /*   370 */  -102, -102, -102, -102, -102, -102, -102, -102, 2314, -102,
 /*   380 */  -102,  127, -102, -102,  570, -102, -102,  607, -102, -102,
 /*   390 */   342, -102, -102,   -1, -102, -102,  441, -102, -102,  398,
 /*   400 */  -102, -102,  170, -102, -102,   22, -102, -102,  194, -102,
 /*   410 */  -102,  428, -102, -102,   95, -102, -102, -102, -102, -102,
 /*   420 */  -102, -102, -102, -102, 1629, -102, 1293, 1703, -102, -102,
 /*   430 */  -102, 2142, -102, 1499, 1703, -102, 1668, -102, 1457, 1703,
 /*   440 */  -102, -102, 1585, -102, -102, -102, -102, -102, 1211, 1703,
 /*   450 */  -102, -102, -102, -102, 1048, 1703, -102, -102, -102, -102,
 /*   460 */  1416, -102, 1703, -102,  -46,  606, -102, -102,    1, -102,
 /*   470 */  1170, -102, 1703, -102, -102, -102, 1539, -102, 1703, -102,
 /*   480 */  1120,  353, -102, -102,  -46, -102, -102,  859, -102, -102,
 /*   490 */  -102, -102,  871, -102, -102, -102,  120,  606, -102, -102,
 /*   500 */   901, -102, -102,  606, -102, -102, -101, -102, -102, -102,
 /*   510 */   -43, 1329,  353,    5,  353, -102,  353,  305,  240, -102,
 /*   520 */   115, -102, -102, -102, -102, -102,  405, -102, -102,  437,
 /*   530 */  -102, -102, -102,  504, -102, -102,  531, -102, -102,
};
static YYACTIONTYPE yy_default[] = {
 /*     0 */   799,  799,  799,  541,  543,  544,  545,  546,  799,  799,
 /*    10 */   547,  799,  799,  548,  799,  799,  549,  799,  562,  799,
 /*    20 */   563,  583,  799,  799,  799,  799,  799,  595,  799,  799,
 /*    30 */   596,  799,  609,  611,  799,  614,  789,  790,  791,  792,
 /*    40 */   793,  794,  795,  799,  612,  799,  615,  799,  613,  799,
 /*    50 */   616,  799,  799,  799,  618,  619,  620,  621,  622,  623,
 /*    60 */   624,  625,  626,  627,  628,  610,  599,  601,  602,  603,
 /*    70 */   604,  605,  606,  607,  608,  799,  799,  799,  799,  799,
 /*    80 */   799,  597,  799,  799,  598,  600,  584,  799,  799,  550,
 /*    90 */   799,  799,  551,  799,  557,  799,  558,  799,  799,  581,
 /*   100 */   799,  799,  799,  799,  799,  799,  587,  799,  589,  629,
 /*   110 */   631,  632,  633,  634,  635,  636,  637,  638,  639,  640,
 /*   120 */   641,  642,  643,  644,  645,  646,  647,  648,  799,  649,
 /*   130 */   799,  650,  799,  799,  799,  799,  653,  799,  799,  799,
 /*   140 */   654,  799,  799,  799,  657,  799,  658,  659,  799,  799,
 /*   150 */   661,  662,  799,  799,  799,  665,  799,  799,  799,  799,
 /*   160 */   667,  799,  799,  799,  799,  799,  799,  669,  799,  726,
 /*   170 */   799,  799,  727,  799,  799,  799,  728,  799,  730,  799,
 /*   180 */   731,  799,  732,  799,  733,  799,  734,  799,  735,  799,
 /*   190 */   736,  799,  744,  799,  745,  799,  746,  799,  747,  799,
 /*   200 */   748,  799,  749,  799,  750,  799,  751,  799,  799,  737,
 /*   210 */   799,  799,  738,  799,  799,  799,  739,  756,  799,  740,
 /*   220 */   799,  780,  799,  752,  799,  799,  799,  753,  799,  754,
 /*   230 */   755,  757,  758,  759,  760,  761,  762,  763,  799,  788,
 /*   240 */   756,  799,  741,  799,  799,  766,  799,  767,  799,  768,
 /*   250 */   799,  775,  776,  777,  778,  779,  799,  771,  772,  742,
 /*   260 */   799,  799,  743,  799,  799,  769,  770,  757,  758,  764,
 /*   270 */   799,  765,  799,  781,  799,  799,  783,  784,  782,  799,
 /*   280 */   773,  774,  799,  796,  729,  797,  799,  799,  799,  799,
 /*   290 */   799,  799,  670,  799,  799,  674,  799,  675,  799,  677,
 /*   300 */   678,  679,  680,  681,  682,  799,  683,  725,  799,  799,
 /*   310 */   799,  684,  799,  799,  799,  685,  799,  799,  799,  686,
 /*   320 */   799,  692,  693,  799,  799,  799,  687,  799,  799,  799,
 /*   330 */   688,  799,  799,  799,  689,  799,  799,  799,  690,  799,
 /*   340 */   799,  799,  691,  694,  695,  676,  799,  799,  697,  799,
 /*   350 */   698,  700,  699,  799,  701,  799,  702,  799,  703,  799,
 /*   360 */   799,  704,  705,  799,  799,  799,  799,  706,  799,  799,
 /*   370 */   799,  707,  799,  799,  708,  799,  799,  709,  799,  799,
 /*   380 */   710,  799,  799,  711,  799,  721,  723,  799,  724,  722,
 /*   390 */   799,  799,  712,  799,  799,  713,  799,  799,  714,  799,
 /*   400 */   799,  715,  799,  799,  716,  799,  799,  717,  799,  799,
 /*   410 */   718,  799,  799,  719,  799,  799,  720,  799,  799,  799,
 /*   420 */   799,  799,  798,  630,  799,  799,  799,  799,  671,  799,
 /*   430 */   799,  799,  799,  799,  799,  672,  799,  799,  799,  799,
 /*   440 */   673,  799,  799,  799,  668,  666,  664,  799,  799,  663,
 /*   450 */   660,  655,  651,  799,  799,  799,  656,  652,  799,  799,
 /*   460 */   799,  588,  799,  590,  799,  799,  799,  799,  799,  799,
 /*   470 */   799,  591,  799,  593,  799,  799,  799,  592,  799,  594,
 /*   480 */   799,  799,  579,  582,  799,  799,  799,  799,  799,  585,
 /*   490 */   799,  799,  799,  799,  586,  573,  799,  799,  799,  575,
 /*   500 */   799,  799,  577,  799,  799,  576,  799,  799,  578,  580,
 /*   510 */   799,  799,  799,  799,  799,  574,  799,  799,  799,  552,
 /*   520 */   799,  559,  561,  560,  799,  799,  799,  553,  799,  799,
 /*   530 */   554,  799,  799,  799,  555,  799,  799,  556,  542,
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
 /* 200 */ "xx_common_expr ::= LESS IDENTIFIER GREATER xx_common_expr",
 /* 201 */ "xx_common_expr ::= IDENTIFIER ARROW IDENTIFIER",
 /* 202 */ "xx_common_expr ::= IDENTIFIER DOUBLECOLON IDENTIFIER",
 /* 203 */ "xx_common_expr ::= IDENTIFIER DOUBLECOLON CONSTANT",
 /* 204 */ "xx_common_expr ::= IDENTIFIER SBRACKET_OPEN xx_common_expr SBRACKET_CLOSE",
 /* 205 */ "xx_common_expr ::= xx_common_expr ADD xx_common_expr",
 /* 206 */ "xx_common_expr ::= xx_common_expr SUB xx_common_expr",
 /* 207 */ "xx_common_expr ::= xx_common_expr MUL xx_common_expr",
 /* 208 */ "xx_common_expr ::= xx_common_expr DIV xx_common_expr",
 /* 209 */ "xx_common_expr ::= xx_common_expr MOD xx_common_expr",
 /* 210 */ "xx_common_expr ::= xx_common_expr CONCAT xx_common_expr",
 /* 211 */ "xx_common_expr ::= xx_common_expr AND xx_common_expr",
 /* 212 */ "xx_common_expr ::= xx_common_expr OR xx_common_expr",
 /* 213 */ "xx_common_expr ::= INSTANCEOF xx_common_expr",
 /* 214 */ "xx_common_expr ::= FETCH IDENTIFIER COMMA xx_isset_expr",
 /* 215 */ "xx_common_expr ::= TYPEOF xx_common_expr",
 /* 216 */ "xx_common_expr ::= CONSTANT",
 /* 217 */ "xx_common_expr ::= IDENTIFIER",
 /* 218 */ "xx_common_expr ::= INTEGER",
 /* 219 */ "xx_common_expr ::= STRING",
 /* 220 */ "xx_common_expr ::= CHAR",
 /* 221 */ "xx_common_expr ::= DOUBLE",
 /* 222 */ "xx_common_expr ::= NULL",
 /* 223 */ "xx_common_expr ::= TRUE",
 /* 224 */ "xx_common_expr ::= FALSE",
 /* 225 */ "xx_common_expr ::= SBRACKET_OPEN SBRACKET_CLOSE",
 /* 226 */ "xx_common_expr ::= SBRACKET_OPEN xx_array_list SBRACKET_CLOSE",
 /* 227 */ "xx_common_expr ::= NEW IDENTIFIER",
 /* 228 */ "xx_common_expr ::= NEW IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 229 */ "xx_common_expr ::= NEW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 230 */ "xx_fcall_expr ::= IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 231 */ "xx_fcall_expr ::= IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 232 */ "xx_scall_expr ::= IDENTIFIER DOUBLECOLON IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 233 */ "xx_scall_expr ::= IDENTIFIER DOUBLECOLON IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 234 */ "xx_mcall_expr ::= IDENTIFIER ARROW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 235 */ "xx_mcall_expr ::= IDENTIFIER ARROW IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 236 */ "xx_common_expr ::= xx_mcall_expr",
 /* 237 */ "xx_common_expr ::= xx_scall_expr",
 /* 238 */ "xx_common_expr ::= xx_fcall_expr",
 /* 239 */ "xx_call_parameters ::= xx_call_parameters COMMA xx_call_parameter",
 /* 240 */ "xx_call_parameters ::= xx_call_parameter",
 /* 241 */ "xx_call_parameter ::= xx_common_expr",
 /* 242 */ "xx_array_list ::= xx_array_list COMMA xx_array_item",
 /* 243 */ "xx_array_list ::= xx_array_item",
 /* 244 */ "xx_array_item ::= xx_array_key COLON xx_array_value",
 /* 245 */ "xx_array_item ::= xx_array_value",
 /* 246 */ "xx_array_key ::= IDENTIFIER",
 /* 247 */ "xx_array_key ::= STRING",
 /* 248 */ "xx_array_key ::= INTEGER",
 /* 249 */ "xx_array_value ::= xx_common_expr",
 /* 250 */ "xx_literal_expr ::= INTEGER",
 /* 251 */ "xx_literal_expr ::= CHAR",
 /* 252 */ "xx_literal_expr ::= STRING",
 /* 253 */ "xx_literal_expr ::= DOUBLE",
 /* 254 */ "xx_literal_expr ::= NULL",
 /* 255 */ "xx_literal_expr ::= FALSE",
 /* 256 */ "xx_literal_expr ::= TRUE",
 /* 257 */ "xx_isset_expr ::= IDENTIFIER SBRACKET_OPEN xx_common_expr SBRACKET_CLOSE",
 /* 258 */ "xx_eval_expr ::= xx_common_expr",
 /* 259 */ "xx_comment ::= COMMENT",
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
// 2357 "parser.c"
      break;
    case 99:
// 910 "parser.lemon"
{ json_object_put((yypminor->yy175)); }
// 2362 "parser.c"
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
// 2839 "parser.c"
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
      case 236:
      case 237:
      case 238:
      case 241:
      case 249:
      case 258:
// 912 "parser.lemon"
{
	yygotominor.yy175 = yymsp[0].minor.yy175;
}
// 2876 "parser.c"
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
// 2890 "parser.c"
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
      case 240:
      case 243:
// 920 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_list(NULL, yymsp[0].minor.yy175);
}
// 2911 "parser.c"
        break;
      case 8:
// 940 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_namespace(yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(31,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 2920 "parser.c"
        break;
      case 9:
// 944 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_interface(yymsp[-1].minor.yy0, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(34,&yymsp[-2].minor);
}
// 2928 "parser.c"
        break;
      case 10:
// 948 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_interface(yymsp[-3].minor.yy0, yymsp[0].minor.yy175, yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(34,&yymsp[-4].minor);
  yy_destructor(35,&yymsp[-2].minor);
}
// 2937 "parser.c"
        break;
      case 11:
// 952 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class(yymsp[-1].minor.yy0, yymsp[0].minor.yy175, 0, 0, NULL, NULL, status->scanner_state);
  yy_destructor(36,&yymsp[-2].minor);
}
// 2945 "parser.c"
        break;
      case 12:
// 956 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy175, 0, 0, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(36,&yymsp[-4].minor);
  yy_destructor(35,&yymsp[-2].minor);
}
// 2954 "parser.c"
        break;
      case 13:
// 960 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy175, 0, 0, NULL, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(36,&yymsp[-4].minor);
  yy_destructor(37,&yymsp[-2].minor);
}
// 2963 "parser.c"
        break;
      case 14:
// 964 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class(yymsp[-1].minor.yy0, yymsp[0].minor.yy175, 1, 0, NULL, NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-3].minor);
  yy_destructor(36,&yymsp[-2].minor);
}
// 2972 "parser.c"
        break;
      case 15:
// 968 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy175, 1, 0, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-5].minor);
  yy_destructor(36,&yymsp[-4].minor);
  yy_destructor(35,&yymsp[-2].minor);
}
// 2982 "parser.c"
        break;
      case 16:
// 972 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class(yymsp[-1].minor.yy0, yymsp[0].minor.yy175, 0, 1, NULL, NULL, status->scanner_state);
  yy_destructor(39,&yymsp[-3].minor);
  yy_destructor(36,&yymsp[-2].minor);
}
// 2991 "parser.c"
        break;
      case 17:
// 976 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy175, 0, 1, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(39,&yymsp[-5].minor);
  yy_destructor(36,&yymsp[-4].minor);
  yy_destructor(35,&yymsp[-2].minor);
}
// 3001 "parser.c"
        break;
      case 18:
      case 23:
// 980 "parser.lemon"
{
	yygotominor.yy175 = NULL;
  yy_destructor(40,&yymsp[-1].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3011 "parser.c"
        break;
      case 19:
      case 24:
// 984 "parser.lemon"
{
	yygotominor.yy175 = yymsp[-1].minor.yy175;
  yy_destructor(40,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3021 "parser.c"
        break;
      case 20:
      case 70:
      case 136:
      case 159:
      case 182:
      case 239:
      case 242:
// 988 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_list(yymsp[-2].minor.yy175, yymsp[0].minor.yy175);
  yy_destructor(6,&yymsp[-1].minor);
}
// 3035 "parser.c"
        break;
      case 22:
      case 217:
      case 246:
// 996 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state);
}
// 3044 "parser.c"
        break;
      case 25:
// 1008 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_definition(yymsp[0].minor.yy175, NULL, NULL, status->scanner_state);
}
// 3051 "parser.c"
        break;
      case 26:
// 1012 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_definition(NULL, NULL, yymsp[0].minor.yy175, status->scanner_state);
}
// 3058 "parser.c"
        break;
      case 27:
// 1016 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_definition(NULL, yymsp[0].minor.yy175, NULL, status->scanner_state);
}
// 3065 "parser.c"
        break;
      case 28:
// 1020 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_definition(yymsp[-1].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
}
// 3072 "parser.c"
        break;
      case 29:
// 1024 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_definition(yymsp[-1].minor.yy175, NULL, yymsp[0].minor.yy175, status->scanner_state);
}
// 3079 "parser.c"
        break;
      case 30:
// 1028 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_definition(yymsp[0].minor.yy175, NULL, yymsp[-1].minor.yy175, status->scanner_state);
}
// 3086 "parser.c"
        break;
      case 31:
// 1032 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_definition(NULL, yymsp[0].minor.yy175, yymsp[-1].minor.yy175, status->scanner_state);
}
// 3093 "parser.c"
        break;
      case 32:
// 1036 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_definition(yymsp[-2].minor.yy175, yymsp[0].minor.yy175, yymsp[-1].minor.yy175, status->scanner_state);
}
// 3100 "parser.c"
        break;
      case 33:
// 1040 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_definition(yymsp[-1].minor.yy175, yymsp[0].minor.yy175, yymsp[-2].minor.yy175, status->scanner_state);
}
// 3107 "parser.c"
        break;
      case 36:
// 1056 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_property(yymsp[-2].minor.yy175, yymsp[-1].minor.yy0, NULL, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(33,&yymsp[0].minor);
}
// 3115 "parser.c"
        break;
      case 37:
// 1060 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_property(yymsp[-2].minor.yy175, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(33,&yymsp[0].minor);
}
// 3123 "parser.c"
        break;
      case 38:
// 1064 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_property(yymsp[-4].minor.yy175, yymsp[-3].minor.yy0, yymsp[-1].minor.yy175, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(43,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 3132 "parser.c"
        break;
      case 39:
// 1068 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_property(yymsp[-4].minor.yy175, yymsp[-3].minor.yy0, yymsp[-1].minor.yy175, NULL, status->scanner_state);
  yy_destructor(43,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 3141 "parser.c"
        break;
      case 46:
// 1096 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy175, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(44,&yymsp[-4].minor);
  yy_destructor(43,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 3151 "parser.c"
        break;
      case 47:
// 1100 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy175, NULL, status->scanner_state);
  yy_destructor(44,&yymsp[-4].minor);
  yy_destructor(43,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 3161 "parser.c"
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
// 3173 "parser.c"
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
// 3185 "parser.c"
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
// 3197 "parser.c"
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
// 3209 "parser.c"
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
// 3221 "parser.c"
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
// 3233 "parser.c"
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
// 3245 "parser.c"
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
// 3257 "parser.c"
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
// 3268 "parser.c"
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
// 3279 "parser.c"
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
// 3290 "parser.c"
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
// 3301 "parser.c"
        break;
      case 62:
// 1160 "parser.lemon"
{
	yygotominor.yy175 = json_object_new_string("public");
  yy_destructor(1,&yymsp[0].minor);
}
// 3309 "parser.c"
        break;
      case 63:
// 1164 "parser.lemon"
{
	yygotominor.yy175 = json_object_new_string("protected");
  yy_destructor(2,&yymsp[0].minor);
}
// 3317 "parser.c"
        break;
      case 64:
// 1168 "parser.lemon"
{
	yygotominor.yy175 = json_object_new_string("private");
  yy_destructor(4,&yymsp[0].minor);
}
// 3325 "parser.c"
        break;
      case 65:
// 1172 "parser.lemon"
{
	yygotominor.yy175 = json_object_new_string("static");
  yy_destructor(3,&yymsp[0].minor);
}
// 3333 "parser.c"
        break;
      case 66:
// 1176 "parser.lemon"
{
	yygotominor.yy175 = json_object_new_string("scoped");
  yy_destructor(5,&yymsp[0].minor);
}
// 3341 "parser.c"
        break;
      case 67:
// 1180 "parser.lemon"
{
	yygotominor.yy175 = json_object_new_string("inline");
  yy_destructor(48,&yymsp[0].minor);
}
// 3349 "parser.c"
        break;
      case 68:
// 1184 "parser.lemon"
{
	yygotominor.yy175 = json_object_new_string("abstract");
  yy_destructor(38,&yymsp[0].minor);
}
// 3357 "parser.c"
        break;
      case 69:
// 1188 "parser.lemon"
{
	yygotominor.yy175 = json_object_new_string("final");
  yy_destructor(39,&yymsp[0].minor);
}
// 3365 "parser.c"
        break;
      case 72:
// 1200 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_parameter(NULL, NULL, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 3372 "parser.c"
        break;
      case 73:
// 1204 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_parameter(yymsp[-1].minor.yy175, NULL, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 3379 "parser.c"
        break;
      case 74:
// 1208 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_parameter(NULL, yymsp[-1].minor.yy175, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 3386 "parser.c"
        break;
      case 75:
// 1212 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_parameter(NULL, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(43,&yymsp[-1].minor);
}
// 3394 "parser.c"
        break;
      case 76:
// 1216 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_parameter(yymsp[-3].minor.yy175, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(43,&yymsp[-1].minor);
}
// 3402 "parser.c"
        break;
      case 77:
// 1220 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_parameter(NULL, yymsp[-3].minor.yy175, yymsp[-2].minor.yy0, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(43,&yymsp[-1].minor);
}
// 3410 "parser.c"
        break;
      case 78:
// 1224 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(12,&yymsp[-2].minor);
  yy_destructor(13,&yymsp[0].minor);
}
// 3419 "parser.c"
        break;
      case 79:
// 1228 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_type(XX_TYPE_INTEGER);
  yy_destructor(49,&yymsp[0].minor);
}
// 3427 "parser.c"
        break;
      case 80:
// 1232 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_type(XX_TYPE_UINTEGER);
  yy_destructor(50,&yymsp[0].minor);
}
// 3435 "parser.c"
        break;
      case 81:
// 1236 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_type(XX_TYPE_LONG);
  yy_destructor(51,&yymsp[0].minor);
}
// 3443 "parser.c"
        break;
      case 82:
// 1240 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_type(XX_TYPE_ULONG);
  yy_destructor(52,&yymsp[0].minor);
}
// 3451 "parser.c"
        break;
      case 83:
// 1244 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_type(XX_TYPE_CHAR);
  yy_destructor(53,&yymsp[0].minor);
}
// 3459 "parser.c"
        break;
      case 84:
// 1248 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_type(XX_TYPE_UCHAR);
  yy_destructor(54,&yymsp[0].minor);
}
// 3467 "parser.c"
        break;
      case 85:
// 1252 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_type(XX_TYPE_DOUBLE);
  yy_destructor(55,&yymsp[0].minor);
}
// 3475 "parser.c"
        break;
      case 86:
// 1256 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_type(XX_TYPE_BOOL);
  yy_destructor(56,&yymsp[0].minor);
}
// 3483 "parser.c"
        break;
      case 87:
// 1260 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_type(XX_TYPE_STRING);
  yy_destructor(57,&yymsp[0].minor);
}
// 3491 "parser.c"
        break;
      case 88:
// 1264 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_type(XX_TYPE_ARRAY);
  yy_destructor(58,&yymsp[0].minor);
}
// 3499 "parser.c"
        break;
      case 89:
// 1268 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_type(XX_TYPE_VAR);
  yy_destructor(59,&yymsp[0].minor);
}
// 3507 "parser.c"
        break;
      case 110:
// 1352 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_break_statement(status->scanner_state);
  yy_destructor(60,&yymsp[-1].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 3516 "parser.c"
        break;
      case 111:
// 1356 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_continue_statement(status->scanner_state);
  yy_destructor(61,&yymsp[-1].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 3525 "parser.c"
        break;
      case 112:
// 1360 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_if_statement(yymsp[-2].minor.yy175, NULL, NULL, status->scanner_state);
  yy_destructor(62,&yymsp[-3].minor);
  yy_destructor(40,&yymsp[-1].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3535 "parser.c"
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
// 3548 "parser.c"
        break;
      case 114:
// 1368 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_if_statement(yymsp[-3].minor.yy175, yymsp[-1].minor.yy175, NULL, status->scanner_state);
  yy_destructor(62,&yymsp[-4].minor);
  yy_destructor(40,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3558 "parser.c"
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
// 3571 "parser.c"
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
// 3584 "parser.c"
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
// 3597 "parser.c"
        break;
      case 118:
// 1384 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_switch_statement(yymsp[-2].minor.yy175, NULL, status->scanner_state);
  yy_destructor(64,&yymsp[-3].minor);
  yy_destructor(40,&yymsp[-1].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3607 "parser.c"
        break;
      case 119:
// 1388 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_switch_statement(yymsp[-3].minor.yy175, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(64,&yymsp[-4].minor);
  yy_destructor(40,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3617 "parser.c"
        break;
      case 122:
// 1400 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_case_clause(yymsp[-1].minor.yy175, NULL, status->scanner_state);
  yy_destructor(65,&yymsp[-2].minor);
  yy_destructor(66,&yymsp[0].minor);
}
// 3626 "parser.c"
        break;
      case 123:
// 1404 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_case_clause(yymsp[-2].minor.yy175, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(65,&yymsp[-3].minor);
  yy_destructor(66,&yymsp[-1].minor);
}
// 3635 "parser.c"
        break;
      case 124:
// 1408 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_case_clause(NULL, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(67,&yymsp[-2].minor);
  yy_destructor(66,&yymsp[-1].minor);
}
// 3644 "parser.c"
        break;
      case 125:
// 1412 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_loop_statement(NULL, status->scanner_state);
  yy_destructor(68,&yymsp[-2].minor);
  yy_destructor(40,&yymsp[-1].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3654 "parser.c"
        break;
      case 126:
// 1416 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_loop_statement(yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(68,&yymsp[-3].minor);
  yy_destructor(40,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3664 "parser.c"
        break;
      case 127:
// 1420 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_while_statement(yymsp[-2].minor.yy175, NULL, status->scanner_state);
  yy_destructor(69,&yymsp[-3].minor);
  yy_destructor(40,&yymsp[-1].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3674 "parser.c"
        break;
      case 128:
// 1424 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_while_statement(yymsp[-3].minor.yy175, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(69,&yymsp[-4].minor);
  yy_destructor(40,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3684 "parser.c"
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
// 3696 "parser.c"
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
// 3708 "parser.c"
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
// 3719 "parser.c"
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
// 3731 "parser.c"
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
// 3743 "parser.c"
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
// 3756 "parser.c"
        break;
      case 135:
// 1452 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_statement(yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(74,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 3765 "parser.c"
        break;
      case 138:
// 1465 "parser.lemon"
{
	yygotominor.yy175 = json_object_new_string("assign");
  yy_destructor(43,&yymsp[0].minor);
}
// 3773 "parser.c"
        break;
      case 139:
// 1470 "parser.lemon"
{
	yygotominor.yy175 = json_object_new_string("add-assign");
  yy_destructor(75,&yymsp[0].minor);
}
// 3781 "parser.c"
        break;
      case 140:
// 1475 "parser.lemon"
{
	yygotominor.yy175 = json_object_new_string("sub-assign");
  yy_destructor(76,&yymsp[0].minor);
}
// 3789 "parser.c"
        break;
      case 141:
// 1479 "parser.lemon"
{
	yygotominor.yy175 = json_object_new_string("mult-assign");
  yy_destructor(77,&yymsp[0].minor);
}
// 3797 "parser.c"
        break;
      case 142:
// 1483 "parser.lemon"
{
	yygotominor.yy175 = json_object_new_string("div-assign");
  yy_destructor(78,&yymsp[0].minor);
}
// 3805 "parser.c"
        break;
      case 143:
// 1487 "parser.lemon"
{
	yygotominor.yy175 = json_object_new_string("concat-assign");
  yy_destructor(79,&yymsp[0].minor);
}
// 3813 "parser.c"
        break;
      case 144:
// 1492 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("variable", yymsp[-1].minor.yy175, yymsp[-2].minor.yy0, NULL, NULL, yymsp[0].minor.yy175, status->scanner_state);
}
// 3820 "parser.c"
        break;
      case 145:
// 1497 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("object-property", yymsp[-1].minor.yy175, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(80,&yymsp[-3].minor);
}
// 3828 "parser.c"
        break;
      case 146:
// 1502 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("object-property-append", yymsp[-1].minor.yy175, yymsp[-6].minor.yy0, yymsp[-4].minor.yy0, NULL, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(80,&yymsp[-5].minor);
  yy_destructor(81,&yymsp[-3].minor);
  yy_destructor(82,&yymsp[-2].minor);
}
// 3838 "parser.c"
        break;
      case 147:
// 1507 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("object-property-array-index", yymsp[-1].minor.yy175, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, yymsp[-3].minor.yy175, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(80,&yymsp[-6].minor);
  yy_destructor(81,&yymsp[-4].minor);
  yy_destructor(82,&yymsp[-2].minor);
}
// 3848 "parser.c"
        break;
      case 148:
// 1512 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("static-property", yymsp[-1].minor.yy175, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(83,&yymsp[-3].minor);
}
// 3856 "parser.c"
        break;
      case 149:
// 1517 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("static-property-append", yymsp[-1].minor.yy175, yymsp[-6].minor.yy0, yymsp[-4].minor.yy0, NULL, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(83,&yymsp[-5].minor);
  yy_destructor(81,&yymsp[-3].minor);
  yy_destructor(82,&yymsp[-2].minor);
}
// 3866 "parser.c"
        break;
      case 150:
// 1522 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("static-property-array-index", yymsp[-1].minor.yy175, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, yymsp[-3].minor.yy175, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(83,&yymsp[-6].minor);
  yy_destructor(81,&yymsp[-4].minor);
  yy_destructor(82,&yymsp[-2].minor);
}
// 3876 "parser.c"
        break;
      case 151:
// 1527 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("variable-append", yymsp[-1].minor.yy175, yymsp[-4].minor.yy0, NULL, NULL, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(81,&yymsp[-3].minor);
  yy_destructor(82,&yymsp[-2].minor);
}
// 3885 "parser.c"
        break;
      case 152:
// 1532 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("array-index", yymsp[-1].minor.yy175, yymsp[-5].minor.yy0, NULL, yymsp[-3].minor.yy175, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(81,&yymsp[-4].minor);
  yy_destructor(82,&yymsp[-2].minor);
}
// 3894 "parser.c"
        break;
      case 153:
// 1537 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("incr", NULL, yymsp[-3].minor.yy0, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(80,&yymsp[-2].minor);
  yy_destructor(84,&yymsp[0].minor);
}
// 3903 "parser.c"
        break;
      case 154:
// 1542 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("decr", NULL, yymsp[-3].minor.yy0, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(80,&yymsp[-2].minor);
  yy_destructor(85,&yymsp[0].minor);
}
// 3912 "parser.c"
        break;
      case 155:
// 1547 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("incr", NULL, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(84,&yymsp[0].minor);
}
// 3920 "parser.c"
        break;
      case 156:
// 1552 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("decr", NULL, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(85,&yymsp[0].minor);
}
// 3928 "parser.c"
        break;
      case 158:
// 1560 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_echo_statement(yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(86,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 3937 "parser.c"
        break;
      case 161:
// 1572 "parser.lemon"
{
	yygotominor.yy175 = yymsp[0].minor.yy175;;
}
// 3944 "parser.c"
        break;
      case 162:
// 1577 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_mcall_statement(yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(33,&yymsp[0].minor);
}
// 3952 "parser.c"
        break;
      case 163:
// 1582 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_fcall_statement(yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(33,&yymsp[0].minor);
}
// 3960 "parser.c"
        break;
      case 164:
// 1587 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_scall_statement(yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(33,&yymsp[0].minor);
}
// 3968 "parser.c"
        break;
      case 165:
// 1592 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_return_statement(yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(87,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 3977 "parser.c"
        break;
      case 166:
// 1597 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_return_statement(NULL, status->scanner_state);
  yy_destructor(87,&yymsp[-1].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 3986 "parser.c"
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
// 4000 "parser.c"
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
// 4015 "parser.c"
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
// 4030 "parser.c"
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
// 4045 "parser.c"
        break;
      case 171:
// 1638 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_throw_exception(yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(91,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 4054 "parser.c"
        break;
      case 172:
// 1642 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_statement(XX_T_TYPE_INTEGER, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 4063 "parser.c"
        break;
      case 173:
// 1646 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_statement(XX_T_TYPE_UINTEGER, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(50,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 4072 "parser.c"
        break;
      case 174:
// 1650 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_statement(XX_T_TYPE_CHAR, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(53,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 4081 "parser.c"
        break;
      case 175:
// 1654 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_statement(XX_T_TYPE_UCHAR, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 4090 "parser.c"
        break;
      case 176:
// 1658 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_statement(XX_T_TYPE_LONG, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(51,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 4099 "parser.c"
        break;
      case 177:
// 1662 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_statement(XX_T_TYPE_ULONG, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(52,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 4108 "parser.c"
        break;
      case 178:
// 1666 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_statement(XX_T_TYPE_DOUBLE, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(55,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 4117 "parser.c"
        break;
      case 179:
// 1670 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_statement(XX_T_TYPE_STRING, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(57,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 4126 "parser.c"
        break;
      case 180:
// 1674 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_statement(XX_T_TYPE_BOOL, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(56,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 4135 "parser.c"
        break;
      case 181:
// 1678 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_statement(XX_T_TYPE_VAR, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(59,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 4144 "parser.c"
        break;
      case 184:
// 1690 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_variable(yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 4151 "parser.c"
        break;
      case 185:
// 1694 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_variable(yymsp[-2].minor.yy0, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(43,&yymsp[-1].minor);
}
// 4159 "parser.c"
        break;
      case 187:
// 1702 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("not", yymsp[0].minor.yy175, NULL, NULL, status->scanner_state);
  yy_destructor(29,&yymsp[-1].minor);
}
// 4167 "parser.c"
        break;
      case 188:
// 1706 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("isset", yymsp[0].minor.yy175, NULL, NULL, status->scanner_state);
  yy_destructor(24,&yymsp[-1].minor);
}
// 4175 "parser.c"
        break;
      case 189:
// 1710 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("require", yymsp[0].minor.yy175, NULL, NULL, status->scanner_state);
  yy_destructor(7,&yymsp[-1].minor);
}
// 4183 "parser.c"
        break;
      case 190:
// 1714 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("equals", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(10,&yymsp[-1].minor);
}
// 4191 "parser.c"
        break;
      case 191:
// 1718 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("not-equals", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(17,&yymsp[-1].minor);
}
// 4199 "parser.c"
        break;
      case 192:
// 1722 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("identical", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(11,&yymsp[-1].minor);
}
// 4207 "parser.c"
        break;
      case 193:
// 1726 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("not-identical", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(16,&yymsp[-1].minor);
}
// 4215 "parser.c"
        break;
      case 194:
// 1730 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("less", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(12,&yymsp[-1].minor);
}
// 4223 "parser.c"
        break;
      case 195:
// 1734 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("greater", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(13,&yymsp[-1].minor);
}
// 4231 "parser.c"
        break;
      case 196:
// 1738 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("less-equal", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(14,&yymsp[-1].minor);
}
// 4239 "parser.c"
        break;
      case 197:
// 1742 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("greater-equal", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(15,&yymsp[-1].minor);
}
// 4247 "parser.c"
        break;
      case 198:
// 1746 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("list", yymsp[-1].minor.yy175, NULL, NULL, status->scanner_state);
  yy_destructor(47,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 4256 "parser.c"
        break;
      case 199:
// 1750 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("cast", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(47,&yymsp[-3].minor);
  yy_destructor(30,&yymsp[-1].minor);
}
// 4265 "parser.c"
        break;
      case 200:
// 1754 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("type-hint", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(12,&yymsp[-3].minor);
  yy_destructor(13,&yymsp[-1].minor);
}
// 4274 "parser.c"
        break;
      case 201:
// 1758 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("property-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(80,&yymsp[-1].minor);
}
// 4282 "parser.c"
        break;
      case 202:
// 1762 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("static-property-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(83,&yymsp[-1].minor);
}
// 4290 "parser.c"
        break;
      case 203:
// 1766 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("static-constant-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(83,&yymsp[-1].minor);
}
// 4298 "parser.c"
        break;
      case 204:
      case 257:
// 1771 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), yymsp[-1].minor.yy175, NULL, status->scanner_state);
  yy_destructor(81,&yymsp[-2].minor);
  yy_destructor(82,&yymsp[0].minor);
}
// 4308 "parser.c"
        break;
      case 205:
// 1776 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("add", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(18,&yymsp[-1].minor);
}
// 4316 "parser.c"
        break;
      case 206:
// 1781 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("sub", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(19,&yymsp[-1].minor);
}
// 4324 "parser.c"
        break;
      case 207:
// 1786 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("mul", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(21,&yymsp[-1].minor);
}
// 4332 "parser.c"
        break;
      case 208:
// 1791 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("div", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(22,&yymsp[-1].minor);
}
// 4340 "parser.c"
        break;
      case 209:
// 1796 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("mod", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(23,&yymsp[-1].minor);
}
// 4348 "parser.c"
        break;
      case 210:
// 1801 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("concat", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(20,&yymsp[-1].minor);
}
// 4356 "parser.c"
        break;
      case 211:
// 1806 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("and", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(8,&yymsp[-1].minor);
}
// 4364 "parser.c"
        break;
      case 212:
// 1811 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("or", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(9,&yymsp[-1].minor);
}
// 4372 "parser.c"
        break;
      case 213:
// 1816 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("instanceof", yymsp[0].minor.yy175, NULL, NULL, status->scanner_state);
  yy_destructor(27,&yymsp[-1].minor);
}
// 4380 "parser.c"
        break;
      case 214:
// 1821 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("fetch", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(25,&yymsp[-3].minor);
  yy_destructor(6,&yymsp[-1].minor);
}
// 4389 "parser.c"
        break;
      case 215:
// 1826 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("typeof", yymsp[0].minor.yy175, NULL, NULL, status->scanner_state);
  yy_destructor(26,&yymsp[-1].minor);
}
// 4397 "parser.c"
        break;
      case 216:
// 1831 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_literal(XX_T_CONSTANT, yymsp[0].minor.yy0, status->scanner_state);
}
// 4404 "parser.c"
        break;
      case 218:
      case 248:
      case 250:
// 1841 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_literal(XX_T_INTEGER, yymsp[0].minor.yy0, status->scanner_state);
}
// 4413 "parser.c"
        break;
      case 219:
      case 247:
      case 252:
// 1846 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_literal(XX_T_STRING, yymsp[0].minor.yy0, status->scanner_state);
}
// 4422 "parser.c"
        break;
      case 220:
      case 251:
// 1851 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_literal(XX_T_CHAR, yymsp[0].minor.yy0, status->scanner_state);
}
// 4430 "parser.c"
        break;
      case 221:
      case 253:
// 1856 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_literal(XX_T_DOUBLE, yymsp[0].minor.yy0, status->scanner_state);
}
// 4438 "parser.c"
        break;
      case 222:
      case 254:
// 1861 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_literal(XX_T_NULL, NULL, status->scanner_state);
  yy_destructor(94,&yymsp[0].minor);
}
// 4447 "parser.c"
        break;
      case 223:
      case 256:
// 1866 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_literal(XX_T_TRUE, NULL, status->scanner_state);
  yy_destructor(95,&yymsp[0].minor);
}
// 4456 "parser.c"
        break;
      case 224:
      case 255:
// 1871 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_literal(XX_T_FALSE, NULL, status->scanner_state);
  yy_destructor(96,&yymsp[0].minor);
}
// 4465 "parser.c"
        break;
      case 225:
// 1876 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("empty-array", NULL, NULL, NULL, status->scanner_state);
  yy_destructor(81,&yymsp[-1].minor);
  yy_destructor(82,&yymsp[0].minor);
}
// 4474 "parser.c"
        break;
      case 226:
// 1881 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("array", yymsp[-1].minor.yy175, NULL, NULL, status->scanner_state);
  yy_destructor(81,&yymsp[-2].minor);
  yy_destructor(82,&yymsp[0].minor);
}
// 4483 "parser.c"
        break;
      case 227:
// 1886 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_new_instance(yymsp[0].minor.yy0, NULL, status->scanner_state);
  yy_destructor(28,&yymsp[-1].minor);
}
// 4491 "parser.c"
        break;
      case 228:
// 1891 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_new_instance(yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(28,&yymsp[-3].minor);
  yy_destructor(47,&yymsp[-1].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 4501 "parser.c"
        break;
      case 229:
// 1896 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_new_instance(yymsp[-3].minor.yy0, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(28,&yymsp[-4].minor);
  yy_destructor(47,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 4511 "parser.c"
        break;
      case 230:
// 1901 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_fcall(yymsp[-3].minor.yy0, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(47,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 4520 "parser.c"
        break;
      case 231:
// 1906 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_fcall(yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(47,&yymsp[-1].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 4529 "parser.c"
        break;
      case 232:
// 1911 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_scall(yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(83,&yymsp[-4].minor);
  yy_destructor(47,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 4539 "parser.c"
        break;
      case 233:
// 1916 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_scall(yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(83,&yymsp[-3].minor);
  yy_destructor(47,&yymsp[-1].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 4549 "parser.c"
        break;
      case 234:
// 1921 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_mcall(yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(80,&yymsp[-4].minor);
  yy_destructor(47,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 4559 "parser.c"
        break;
      case 235:
// 1926 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_mcall(yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(80,&yymsp[-3].minor);
  yy_destructor(47,&yymsp[-1].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 4569 "parser.c"
        break;
      case 244:
// 1965 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_array_item(yymsp[-2].minor.yy175, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(66,&yymsp[-1].minor);
}
// 4577 "parser.c"
        break;
      case 245:
// 1969 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_array_item(NULL, yymsp[0].minor.yy175, status->scanner_state);
}
// 4584 "parser.c"
        break;
      case 259:
// 2026 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_comment(yymsp[0].minor.yy0, status->scanner_state);
}
// 4591 "parser.c"
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

// 4667 "parser.c"
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
