/** The author disclaims copyright to this source code.
*/
/* First off, code is include which follows the "include" declaration
** in the input file. */
#include <stdio.h>
// 47 "parser.lemon"


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
#define YYNSTATE 563
#define YYNRULE 267
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
 /*     0 */   178,    3,    4,    5,    6,    7,  218,  831,    1,    2,
 /*    10 */   562,    4,    5,    6,    7,  147,  427,  410,  174,  227,
 /*    20 */   231,  247,  171,   98,  816,  222,  100,  531,  537,  536,
 /*    30 */   524,   82,  530,  504,   65,   43,   47,   85,  233,  151,
 /*    40 */   212,  468,   54,   55,   56,   57,   58,   59,   60,   61,
 /*    50 */    62,   63,   64,  391,  210,  206,  208,  173,  182,  186,
 /*    60 */   188,  190,  192,  184,  180,  194,  196,  204,  198,  200,
 /*    70 */   202,  378,  219,  438,  235,  241,  214,  374,  379,  254,
 /*    80 */   256,  255,  477,  443,  234,  236,  237,  238,  239,  240,
 /*    90 */   402,  411,  420,  423,  414,  417,  426,  432,  429,  253,
 /*   100 */   435,  131,  133,  135,   94,  144,  541,   38,   13,  155,
 /*   110 */   159,  164,  304,  156,  266,  311,  438,   36,   37,   39,
 /*   120 */    40,   42,   41,  268,  327,  482,  443,  216,  373,  386,
 /*   130 */   390,  399,  341,  402,  411,  420,  423,  414,  417,  426,
 /*   140 */   432,  429,  224,  435,  131,  133,  135,  223,  144,  263,
 /*   150 */   285,  244,  155,  159,  164,  304,  412,  410,  311,  438,
 /*   160 */   501,  101,  516,  524,   81,  530,  504,  385,  456,  443,
 /*   170 */    85,  373,  386,  390,  399,  253,  402,  411,  420,  423,
 /*   180 */   414,  417,  426,  432,  429,  559,  435,  131,  133,  135,
 /*   190 */    96,  144,   32,   43,   47,  155,  159,  164,  304,  290,
 /*   200 */   266,  311,  438,  194,  196,  204,  198,  200,  202,  405,
 /*   210 */    93,  467,  443,   96,  373,  386,  390,  399,   35,  402,
 /*   220 */   411,  420,  423,  414,  417,  426,  432,  429,   76,  435,
 /*   230 */   131,  133,  135,  441,  144,   85,  422,  439,  155,  159,
 /*   240 */   164,  304,  325,  520,  311,  438,  333,  198,  200,  202,
 /*   250 */   254,  256,  255,  521,  466,  443,   14,  373,  386,  390,
 /*   260 */   399,   17,  402,  411,  420,  423,  414,  417,  426,  432,
 /*   270 */   429,  383,  435,  131,  133,  135,  381,  144,  421,  410,
 /*   280 */   538,  155,  159,  164,  304,  325,  245,  311,  438,  337,
 /*   290 */   543,  450,  541,  254,  256,  255,  405,  473,  443,  262,
 /*   300 */   373,  386,  390,  399,   79,  402,  411,  420,  423,  414,
 /*   310 */   417,  426,  432,  429,   96,  435,  131,  133,  135,  526,
 /*   320 */   144,  430,  410,  431,  155,  159,  164,  304,  338,  527,
 /*   330 */   311,  438,  103,  334,  507,  102,  254,  256,  255,   85,
 /*   340 */   143,  443,   31,  373,  386,  390,  399,   78,  402,  411,
 /*   350 */   420,  423,  414,  417,  426,  432,  429,  306,  435,  131,
 /*   360 */   133,  135,  508,  144,  433,  410,   29,  155,  159,  164,
 /*   370 */   304,  325,   66,  311,  438,  329,  312,  372,  405,  254,
 /*   380 */   256,  255,  314,  484,  443,  509,  373,  386,  390,  399,
 /*   390 */   510,  402,  411,  420,  423,  414,  417,  426,  432,  429,
 /*   400 */   518,  435,  131,  133,  135,  434,  144,   85,  486,  313,
 /*   410 */   155,  159,  164,  304,  325,   85,  311,  438,  345,   31,
 /*   420 */   442,  405,  254,  256,  255,  278,  109,  443,  285,  373,
 /*   430 */   386,  390,  399,  368,  402,  411,  420,  423,  414,  417,
 /*   440 */   426,  432,  429,  496,  435,  131,  133,  135,  419,  144,
 /*   450 */   418,  410,  253,  155,  159,  164,  304,  325,  367,  311,
 /*   460 */   438,  353,   31,  424,  410,  254,  256,  255,  512,  167,
 /*   470 */   443,  514,  373,  386,  390,  399,  296,  402,  411,  420,
 /*   480 */   423,  414,  417,  426,  432,  429,  480,  435,  131,  133,
 /*   490 */   135,  553,  144,  541,  513,   31,  155,  159,  164,  304,
 /*   500 */   338,  364,  311,  438,  392,  358,  395,  405,  254,  256,
 /*   510 */   255,  515,  472,  443,  360,  373,  386,  390,  399,   83,
 /*   520 */   402,  411,  420,  423,  414,  417,  426,  432,  429,   48,
 /*   530 */   435,  131,  133,  135,  425,  144,  436,  410,  253,  155,
 /*   540 */   159,  164,  304,  325,  161,  311,  438,  357,  300,  359,
 /*   550 */   177,  254,  256,  255,  356,  449,  443,  253,  373,  386,
 /*   560 */   390,  399,  283,  402,  411,  420,  423,  414,  417,  426,
 /*   570 */   432,  429,   34,  435,  131,  133,  135,  643,  144,  415,
 /*   580 */   410,  252,  155,  159,  164,  304,  325,  352,  311,  438,
 /*   590 */   369,  286,  522,  405,  254,  256,  255,  405,  478,  443,
 /*   600 */   275,  373,  386,  390,  399,  523,  402,  411,  420,  423,
 /*   610 */   414,  417,  426,  432,  429,  351,  435,  131,  133,  135,
 /*   620 */   416,  144,  403,  410,  437,  155,  159,  164,  304,  378,
 /*   630 */    28,  311,  438,   65,   43,   47,  377,  254,  256,  255,
 /*   640 */   292,  111,  443,  348,  373,  386,  390,  399,  301,  402,
 /*   650 */   411,  420,  423,  414,  417,  426,  432,  429,  347,  435,
 /*   660 */   131,  133,  135,  165,  144,  148,  471,  342,  155,  159,
 /*   670 */   164,  304,  325,  344,  311,  438,  361,  274,  528,  405,
 /*   680 */   254,  256,  255,  405,  500,  443,  287,  373,  386,  390,
 /*   690 */   399,  529,  402,  411,  420,  423,  414,  417,  426,  432,
 /*   700 */   429,   53,  435,  131,  133,  135,  428,  144,  288,  343,
 /*   710 */   404,  155,  159,  164,  304,  325,  346,  311,  438,  324,
 /*   720 */   253,    9,  376,  254,  256,  255,  405,  139,  443,   52,
 /*   730 */   373,  386,  390,  399,  335,  402,  411,  420,  423,  414,
 /*   740 */   417,  426,  432,  429,  260,  435,  131,  133,  135,  375,
 /*   750 */   144,  540,  336,  413,  155,  159,  164,  304,  338,  332,
 /*   760 */   311,  438,  328,  366,  539,   50,  254,  256,  255,  323,
 /*   770 */   158,  443,  168,  373,  386,  390,  399,  105,  402,  411,
 /*   780 */   420,  423,  414,  417,  426,  432,  429,  315,  435,  131,
 /*   790 */   133,  135,  544,  144,  293,  316,  546,  155,  159,  164,
 /*   800 */   304,  242,  495,  311,  438,   65,   43,   47,  294,  254,
 /*   810 */   256,  255,  547,  494,  443,  548,  373,  386,  390,  399,
 /*   820 */   279,  402,  411,  420,  423,  414,  417,  426,  432,  429,
 /*   830 */   170,  435,  131,  133,  135,  550,  144,  551,   96,   49,
 /*   840 */   155,  159,  164,  304,  325,  554,  311,  438,  365,  106,
 /*   850 */    12,  108,  254,  256,  255,  556,  310,  443,  557,  373,
 /*   860 */   386,  390,  399,  558,  402,  411,  420,  423,  414,  417,
 /*   870 */   426,  432,  429,  440,  435,  131,  133,  135,  246,  144,
 /*   880 */   225,  248,  137,  155,  159,  164,  304,  325,  407,  311,
 /*   890 */   438,  349,  249,  176,  451,  254,  256,  255,  140,  461,
 /*   900 */   443,  452,  373,  386,  390,  399,   95,  402,  411,  420,
 /*   910 */   423,  414,  417,  426,  432,  429,   44,  435,  131,  133,
 /*   920 */   135,  560,  144,  134,  563,  141,  155,  159,  164,  304,
 /*   930 */   400,  479,  311,  438,   65,   43,   47,   22,  254,  256,
 /*   940 */   255,   21,  163,  443,   85,  373,  386,  390,  399,  305,
 /*   950 */   402,  411,  420,  423,  414,  417,  426,  432,  429,   92,
 /*   960 */   435,  131,  133,  135,  132,  144,   16,   91,  463,  155,
 /*   970 */   159,  164,  304,  307,   17,  311,  438,   89,  465,   27,
 /*   980 */    45,  254,  256,  255,  469,  492,  443,  146,  373,  386,
 /*   990 */   390,  399,   88,  402,  411,  420,  423,  414,  417,  426,
 /*  1000 */   432,  429,  474,  435,  131,  133,  135,  475,  144,  230,
 /*  1010 */    25,  150,  155,  159,  164,  304,  298,   99,  311,  438,
 /*  1020 */    46,  409,  481,  229,  254,  256,  255,  408,  462,  443,
 /*  1030 */    15,  373,  386,  390,  399,   30,  402,  411,  420,  423,
 /*  1040 */   414,  417,  426,  432,  429,  228,  435,  131,  133,  135,
 /*  1050 */    24,  144,  406,  488,  489,  155,  159,  164,  304,  179,
 /*  1060 */   561,  311,  438,  491,   84,  152,  175,  254,  256,  255,
 /*  1070 */   153,  498,  443,  398,  373,  386,  390,  399,  317,  402,
 /*  1080 */   411,  420,  423,  414,  417,  426,  432,  429,   10,  435,
 /*  1090 */   131,  133,  135,  394,  144,  393,  497,   19,  155,  159,
 /*  1100 */   164,  304,   22,  817,  311,  438,   86,  220,  437,   85,
 /*  1110 */   318,  319,  320,  321,  322,  443,  437,  373,  386,  390,
 /*  1120 */   399,  437,  402,  411,  420,  423,  414,  417,  426,  432,
 /*  1130 */   429,  437,  435,  131,  133,  135,  437,  144,  437,  437,
 /*  1140 */   437,  155,  159,  164,  304,  181,  437,  311,  130,  242,
 /*  1150 */   437,  437,  437,  254,  256,  255,  437,  254,  256,  255,
 /*  1160 */   373,  386,  390,  399,  437,  460,  276,  277,  280,  149,
 /*  1170 */   444,  113,  114,  115,  116,  117,  118,  119,  120,  121,
 /*  1180 */   122,  123,  124,  125,  126,  127,  128,  129,  130,  533,
 /*  1190 */   516,  524,  437,  151,  504,  468,  437,  437,   85,  380,
 /*  1200 */   382,  384,   90,  437,  542,  166,  437,   96,  437,  437,
 /*  1210 */   444,  113,  114,  115,  116,  117,  118,  119,  120,  121,
 /*  1220 */   122,  123,  124,  125,  126,  127,  128,  129,  130,  437,
 /*  1230 */   437,  532,  437,  535,  536,  524,  437,  503,  504,  380,
 /*  1240 */   382,  384,   85,  437,  437,  157,  437,  437,  437,  437,
 /*  1250 */   444,  113,  114,  115,  116,  117,  118,  119,  120,  121,
 /*  1260 */   122,  123,  124,  125,  126,  127,  128,  129,  130,  437,
 /*  1270 */   437,  464,  437,  437,  303,  136,  437,  437,  303,  380,
 /*  1280 */   382,  384,  254,  256,  255,  142,  254,  256,  255,  437,
 /*  1290 */   444,  113,  114,  115,  116,  117,  118,  119,  120,  121,
 /*  1300 */   122,  123,  124,  125,  126,  127,  128,  129,  183,  437,
 /*  1310 */   549,  130,  552,  437,  437,   96,  254,  256,  255,  380,
 /*  1320 */   382,  384,   67,   68,   70,   69,   71,  437,  309,  437,
 /*  1330 */   437,  437,  437,  444,  113,  114,  115,  116,  117,  118,
 /*  1340 */   119,  120,  121,  122,  123,  124,  125,  126,  127,  128,
 /*  1350 */   129,  130,  502,  525,  103,  437,  503,  504,  437,   73,
 /*  1360 */    74,   85,  380,  382,  384,  437,  437,  104,  138,   72,
 /*  1370 */   437,  437,  437,  444,  113,  114,  115,  116,  117,  118,
 /*  1380 */   119,  120,  121,  122,  123,  124,  125,  126,  127,  128,
 /*  1390 */   129,  130,  437,  437,  437,  437,  437,  437,  145,  437,
 /*  1400 */   437,  303,  380,  382,  384,  437,  437,  437,  162,  254,
 /*  1410 */   256,  255,  437,  444,  113,  114,  115,  116,  117,  118,
 /*  1420 */   119,  120,  121,  122,  123,  124,  125,  126,  127,  128,
 /*  1430 */   129,  185,  437,  437,  130,  437,  437,  437,  437,  254,
 /*  1440 */   256,  255,  380,  382,  384,   67,   68,   70,   69,   71,
 /*  1450 */   437,  470,  437,  437,  437,  437,  444,  113,  114,  115,
 /*  1460 */   116,  117,  118,  119,  120,  121,  122,  123,  124,  125,
 /*  1470 */   126,  127,  128,  129,  130,  437,  519,  437,  437,  437,
 /*  1480 */   437,  437,   73,   74,  437,  380,  382,  384,  437,  437,
 /*  1490 */   487,  476,   72,  437,  437,  437,  444,  113,  114,  115,
 /*  1500 */   116,  117,  118,  119,  120,  121,  122,  123,  124,  125,
 /*  1510 */   126,  127,  128,  129,  130,  437,  437,  437,  437,  437,
 /*  1520 */   437,  160,  437,  437,  303,  380,  382,  384,  437,  437,
 /*  1530 */   437,  448,  254,  256,  255,  437,  444,  113,  114,  115,
 /*  1540 */   116,  117,  118,  119,  120,  121,  122,  123,  124,  125,
 /*  1550 */   126,  127,  128,  129,  437,  437,  437,  130,  437,  437,
 /*  1560 */   437,  437,  437,  437,  437,  380,  382,  384,   67,   68,
 /*  1570 */    70,   69,   71,  437,  483,  437,  437,  437,  437,  444,
 /*  1580 */   113,  114,  115,  116,  117,  118,  119,  120,  121,  122,
 /*  1590 */   123,  124,  125,  126,  127,  128,  129,  130,  437,  437,
 /*  1600 */   437,  437,  437,  437,  437,   73,   74,  437,  380,  382,
 /*  1610 */   384,  437,  437,  104,  493,   72,  437,  437,  437,  444,
 /*  1620 */   113,  114,  115,  116,  117,  118,  119,  120,  121,  122,
 /*  1630 */   123,  124,  125,  126,  127,  128,  129,  130,  437,  437,
 /*  1640 */   437,  437,  437,  437,  169,  437,  437,  303,  380,  382,
 /*  1650 */   384,  437,  437,  437,  110,  254,  256,  255,  437,  444,
 /*  1660 */   113,  114,  115,  116,  117,  118,  119,  120,  121,  122,
 /*  1670 */   123,  124,  125,  126,  127,  128,  129,  437,  437,  437,
 /*  1680 */   130,  437,  437,  437,  437,  437,  437,  437,  380,  382,
 /*  1690 */   384,   67,   68,   70,   69,   71,  437,  154,  437,  437,
 /*  1700 */   437,  437,  444,  113,  114,  115,  116,  117,  118,  119,
 /*  1710 */   120,  121,  122,  123,  124,  125,  126,  127,  128,  129,
 /*  1720 */   130,  437,  437,  437,  437,  437,  437,  437,   73,   74,
 /*  1730 */   437,  380,  382,  384,  506,  437,  437,  455,   72,  437,
 /*  1740 */   437,  437,  444,  113,  114,  115,  116,  117,  118,  119,
 /*  1750 */   120,  121,  122,  123,  124,  125,  126,  127,  128,  129,
 /*  1760 */   130,  437,  437,  437,  437,  437,  437,  437,  437,  437,
 /*  1770 */   187,  380,  382,  384,  437,  437,  437,  499,  254,  256,
 /*  1780 */   255,  437,  444,  113,  114,  115,  116,  117,  118,  119,
 /*  1790 */   120,  121,  122,  123,  124,  125,  126,  127,  128,  129,
 /*  1800 */   437,  437,  437,  130,  437,  437,  437,  437,  226,  437,
 /*  1810 */   437,  380,  382,  384,  437,  437,  254,  256,  255,  437,
 /*  1820 */   437,  437,  437,  289,  258,  112,  113,  114,  115,  116,
 /*  1830 */   117,  118,  119,  120,  121,  122,  123,  124,  125,  126,
 /*  1840 */   127,  128,  129,  178,  437,  437,  437,  437,  437,  218,
 /*  1850 */   437,  437,  437,  437,  380,  382,  384,  437,  437,  437,
 /*  1860 */   437,  174,  227,  231,  247,  171,  250,  437,  222,  178,
 /*  1870 */   437,    8,  437,  437,   11,  218,   87,  437,  545,  555,
 /*  1880 */   317,  233,  443,  212,  437,  437,  437,  174,  227,  231,
 /*  1890 */   247,  171,  226,  437,  222,  226,  437,  437,  437,  437,
 /*  1900 */   254,  256,  255,  254,  256,  255,  317,  233,  257,  212,
 /*  1910 */   295,  258,  318,  319,  320,  321,  322,  235,  241,  330,
 /*  1920 */   437,  437,  339,  340,  437,  437,  437,  234,  236,  237,
 /*  1930 */   238,  239,  240,  437,  437,  445,  437,  437,  318,  319,
 /*  1940 */   320,  321,  322,  235,  241,  354,  437,  437,  437,  437,
 /*  1950 */   437,  437,  437,  234,  236,  237,  238,  239,  240,  437,
 /*  1960 */   437,  178,  226,  437,  437,  437,  437,  218,  437,  178,
 /*  1970 */   254,  256,  255,  437,  437,  218,  437,  282,  258,  174,
 /*  1980 */   227,  231,  247,  171,  437,  437,  222,  174,  227,  231,
 /*  1990 */   247,  171,  437,  437,  222,  389,  437,  437,  437,  233,
 /*  2000 */   437,  212,  178,  226,  437,  437,  437,  233,  218,  212,
 /*  2010 */   437,  254,  256,  255,  437,  437,  437,  437,  259,  258,
 /*  2020 */   174,  227,  231,  247,  171,  269,  437,  222,  437,  437,
 /*  2030 */   437,  437,  437,  437,  437,  235,  241,  355,  437,  437,
 /*  2040 */   233,  437,  212,  235,  241,  234,  236,  237,  238,  239,
 /*  2050 */   240,  437,  437,  234,  236,  237,  238,  239,  240,  210,
 /*  2060 */   206,  208,  173,  182,  186,  188,  190,  192,  184,  180,
 /*  2070 */   194,  196,  204,  198,  200,  202,  235,  241,  437,  437,
 /*  2080 */   437,  437,  437,  437,  388,  437,  234,  236,  237,  238,
 /*  2090 */   239,  240,  437,  226,  178,  437,  437,  437,  437,  437,
 /*  2100 */   218,  254,  256,  255,  437,  437,  437,  437,  251,  258,
 /*  2110 */   437,  437,  174,  227,  231,  247,  171,  291,  437,  222,
 /*  2120 */   178,  226,  437,  437,  266,  437,  218,  437,  437,  254,
 /*  2130 */   256,  255,  233,  437,  212,  437,  267,  258,  174,  227,
 /*  2140 */   231,  247,  171,  815,  437,  222,  437,  437,  437,  437,
 /*  2150 */   437,  437,  437,  215,  437,  437,  242,  223,  233,  263,
 /*  2160 */   212,  244,  437,  437,  254,  256,  255,  437,  235,  241,
 /*  2170 */   273,  437,  437,  281,  277,  280,  213,  437,  234,  236,
 /*  2180 */   237,  238,  239,  240,  254,  256,  255,  437,  437,  189,
 /*  2190 */   437,  437,  191,  437,  235,  241,  331,  254,  256,  255,
 /*  2200 */   254,  256,  255,  437,  234,  236,  237,  238,  239,  240,
 /*  2210 */   437,  193,  178,  437,  195,  437,  437,  197,  218,  254,
 /*  2220 */   256,  255,  254,  256,  255,  254,  256,  255,  437,  437,
 /*  2230 */   174,  227,  231,  247,  171,  284,  437,  222,  178,  199,
 /*  2240 */   437,  437,  437,  437,  218,  437,  437,  254,  256,  255,
 /*  2250 */   233,  437,  212,  437,  437,  437,  174,  227,  231,  247,
 /*  2260 */   171,  437,  201,  222,  437,  203,  437,  437,  437,  437,
 /*  2270 */   254,  256,  255,  254,  256,  255,  233,  437,  212,  437,
 /*  2280 */   205,  437,  437,  207,  437,  437,  235,  241,  254,  256,
 /*  2290 */   255,  254,  256,  255,  209,  437,  234,  236,  237,  238,
 /*  2300 */   239,  240,  254,  256,  255,  437,  437,  437,  437,  437,
 /*  2310 */   437,  437,  235,  241,  363,  437,   67,   68,   70,   69,
 /*  2320 */    71,  437,  234,  236,  237,  238,  239,  240,  437,  211,
 /*  2330 */   178,  437,  387,  437,  437,  217,  218,  254,  256,  255,
 /*  2340 */   254,  256,  255,  254,  256,  255,  437,  437,  174,  227,
 /*  2350 */   231,  247,  171,   73,   74,  243,  178,  221,  437,  437,
 /*  2360 */   437,   23,  218,   72,  437,  254,  256,  255,  233,  437,
 /*  2370 */   212,  437,  437,  437,  174,  227,  231,  247,  171,  437,
 /*  2380 */   396,  222,  437,  232,  437,  437,  437,  437,  254,  256,
 /*  2390 */   255,  254,  256,  255,  233,  437,  212,  437,  437,  446,
 /*  2400 */   437,  437,  437,  437,  271,  241,  272,  254,  256,  255,
 /*  2410 */   437,  437,  437,  437,  270,  236,  237,  238,  239,  240,
 /*  2420 */   437,  437,  457,  437,  453,  437,  437,  458,  437,  437,
 /*  2430 */   235,  241,  254,  256,  255,  254,  256,  255,  437,  437,
 /*  2440 */   234,  236,  237,  238,  239,  240,  437,  437,  178,  264,
 /*  2450 */   437,  437,  437,  437,  218,  437,  178,  254,  256,  255,
 /*  2460 */   437,  437,  218,  437,  437,  437,  174,  227,  231,  247,
 /*  2470 */   171,  261,  437,  222,  174,  227,  231,  247,  171,  297,
 /*  2480 */   437,  222,  437,  437,  437,  437,  233,  437,  212,  437,
 /*  2490 */   437,  437,  437,  437,  233,  437,  212,  210,  206,  208,
 /*  2500 */   173,  182,  186,  188,  190,  192,  184,  180,  194,  196,
 /*  2510 */   204,  198,  200,  202,  437,  437,  172,  437,  437,   51,
 /*  2520 */   437,  437,  235,  241,  254,  256,  255,  437,  437,  459,
 /*  2530 */   235,  241,  234,  236,  237,  238,  239,  240,   33,  437,
 /*  2540 */   234,  236,  237,  238,  239,  240,  437,  437,  178,  437,
 /*  2550 */   437,  437,  437,  437,  218,   54,   55,   56,   57,   58,
 /*  2560 */    59,   60,   61,   62,   63,   64,  174,  227,  231,  247,
 /*  2570 */   171,  302,  437,  222,  437,  437,  437,  437,  437,  254,
 /*  2580 */   256,  255,  437,  437,  437,  437,  233,  437,  212,  437,
 /*  2590 */   437,  437,  437,  437,  437,  210,  206,  208,  173,  182,
 /*  2600 */   186,  188,  190,  192,  184,  180,  194,  196,  204,  198,
 /*  2610 */   200,  202,  437,  437,  437,  437,  437,  437,  437,  437,
 /*  2620 */   437,  437,  235,  241,  437,  437,  437,  454,  437,  437,
 /*  2630 */   437,  437,  234,  236,  237,  238,  239,  240,  437,  437,
 /*  2640 */   437,  210,  206,  208,  173,  182,  186,  188,  190,  192,
 /*  2650 */   184,  180,  194,  196,  204,  198,  200,  202,  173,  182,
 /*  2660 */   186,  188,  190,  192,  184,  180,  194,  196,  204,  198,
 /*  2670 */   200,  202,  437,  447,  437,  437,  437,  437,  437,  437,
 /*  2680 */   437,  437,  210,  206,  208,  173,  182,  186,  188,  190,
 /*  2690 */   192,  184,  180,  194,  196,  204,  198,  200,  202,  210,
 /*  2700 */   206,  208,  173,  182,  186,  188,  190,  192,  184,  180,
 /*  2710 */   194,  196,  204,  198,  200,  202,  210,  206,  208,  173,
 /*  2720 */   182,  186,  188,  190,  192,  184,  180,  194,  196,  204,
 /*  2730 */   198,  200,  202,  178,  437,  437,  437,  437,  437,  218,
 /*  2740 */   437,  437,  437,  437,  437,  437,  437,  437,  308,  437,
 /*  2750 */   437,  174,  227,  231,  247,  171,  437,  299,  243,  437,
 /*  2760 */   437,  437,  437,  437,  437,  437,  437,  437,  437,  437,
 /*  2770 */   437,  233,  437,  212,  724,  437,  437,  437,  437,  437,
 /*  2780 */   210,  206,  208,  173,  182,  186,  188,  190,  192,  184,
 /*  2790 */   180,  194,  196,  204,  198,  200,  202,  437,  437,  437,
 /*  2800 */   437,  437,  437,  437,  437,  437,  437,  271,  241,   67,
 /*  2810 */    68,   70,   69,   71,  437,  437,  437,  270,  236,  237,
 /*  2820 */   238,  239,  240,  437,  437,  437,  210,  206,  208,  173,
 /*  2830 */   182,  186,  188,  190,  192,  184,  180,  194,  196,  204,
 /*  2840 */   198,  200,  202,  437,  437,  437,   73,   74,  437,  591,
 /*  2850 */   517,  401,  511,  437,  437,  397,   72,  437,  437,  437,
 /*  2860 */   210,  206,  208,  173,  182,  186,  188,  190,  192,  184,
 /*  2870 */   180,  194,  196,  204,  198,  200,  202,  210,  206,  208,
 /*  2880 */   173,  182,  186,  188,  190,  192,  184,  180,  194,  196,
 /*  2890 */   204,  198,  200,  202,  206,  208,  173,  182,  186,  188,
 /*  2900 */   190,  192,  184,  180,  194,  196,  204,  198,  200,  202,
 /*  2910 */    51,  437,  437,  437,  437,  437,  437,  437,  437,  437,
 /*  2920 */   437,   51,  437,  437,  437,  437,  437,  107,  437,   33,
 /*  2930 */   437,  437,  437,  437,  437,  265,  437,  437,   80,  317,
 /*  2940 */    33,  437,  437,  437,  437,  437,   54,   55,   56,   57,
 /*  2950 */    58,   59,   60,   61,   62,   63,   64,   54,   55,   56,
 /*  2960 */    57,   58,   59,   60,   61,   62,   63,   64,   51,  437,
 /*  2970 */   437,  318,  319,  320,  321,  322,  326,  437,  362,   51,
 /*  2980 */   350,  370,  371,  437,  437,  490,  437,   33,  437,   67,
 /*  2990 */    68,   70,   69,   71,  437,  437,   26,  437,   33,  437,
 /*  3000 */   437,  437,  437,  437,   54,   55,   56,   57,   58,   59,
 /*  3010 */    60,   61,   62,   63,   64,   54,   55,   56,   57,   58,
 /*  3020 */    59,   60,   61,   62,   63,   64,   73,   74,  564,  590,
 /*  3030 */   517,  437,  511,  437,  437,  437,   72,  437,   67,   68,
 /*  3040 */    70,   69,   71,  437,  437,  437,  437,  437,  437,  437,
 /*  3050 */   437,  437,  437,  437,  437,  437,  437,  437,  437,    8,
 /*  3060 */   437,  437,   11,  437,   87,  437,  545,  555,  437,  437,
 /*  3070 */   443,  437,  437,  437,  437,   73,   74,  437,   97,  517,
 /*  3080 */   437,  511,  437,  437,  437,   72,  437,   67,   68,   70,
 /*  3090 */    69,   71,  437,  437,  437,  437,  437,  437,  437,  437,
 /*  3100 */   437,  437,  437,  437,  437,  437,  437,  437,  437,  437,
 /*  3110 */   437,  437,  437,  437,  437,  437,  437,  437,  437,  437,
 /*  3120 */   437,  437,  437,  437,   73,   74,  437,  594,  505,  437,
 /*  3130 */   511,  437,  437,  437,   72,  437,   67,   68,   70,   69,
 /*  3140 */    71,   67,   68,   70,   69,   71,  437,  437,  437,  437,
 /*  3150 */   437,  437,  437,  437,  437,  437,  437,  437,  437,  437,
 /*  3160 */   437,  437,  437,  437,  437,  437,  437,  437,  437,  437,
 /*  3170 */   437,  437,  437,   73,   74,  437,  596,  485,   73,   74,
 /*  3180 */   437,  598,  485,   72,  437,  437,  437,  437,   72,  437,
 /*  3190 */    67,   68,   70,   69,   71,   67,   68,   70,   69,   71,
 /*  3200 */   437,  437,  437,  437,  437,  437,  437,  437,  437,  437,
 /*  3210 */   437,  437,  437,  437,  437,  437,  437,  437,  437,  437,
 /*  3220 */   437,  437,  437,  437,  437,  437,  437,   73,   74,  437,
 /*  3230 */    20,   75,   73,   74,  437,  592,  485,   72,  437,  437,
 /*  3240 */   437,  437,   72,  437,   67,   68,   70,   69,   71,   67,
 /*  3250 */    68,   70,   69,   71,  437,  437,  437,  437,  437,  437,
 /*  3260 */   437,  437,  437,  437,  437,  437,  437,  437,  437,  437,
 /*  3270 */   437,  437,  437,  437,  437,  437,  437,  437,  437,  437,
 /*  3280 */   437,   73,   74,  437,  597,  485,   73,   74,  437,   18,
 /*  3290 */    75,   72,  437,  437,  437,  437,   72,  437,   67,   68,
 /*  3300 */    70,   69,   71,   67,   68,   70,   69,   71,  437,   67,
 /*  3310 */    68,   70,   69,   71,   67,   68,   70,   69,   71,  437,
 /*  3320 */    67,   68,   70,   69,   71,  437,  437,  437,  437,  437,
 /*  3330 */   437,  437,  437,  437,  437,   73,   74,  437,  593,  485,
 /*  3340 */    73,   74,  437,  595,  534,   72,   73,   74,  437,  437,
 /*  3350 */    72,   73,   74,  437,   77,  437,   72,   73,   74,  487,
 /*  3360 */   437,   72,  437,  437,  437,  437,  437,   72,
};
static YYCODETYPE yy_lookahead[] = {
 /*     0 */     7,  101,  102,  103,  104,  105,   13,   98,   99,  100,
 /*    10 */   101,  102,  103,  104,  105,   41,  159,  160,   25,   26,
 /*    20 */    27,   28,   29,  109,   66,   32,  112,  113,  114,  115,
 /*    30 */   116,  121,  118,  119,  124,  125,  126,  123,   45,   65,
 /*    40 */    47,   67,   49,   50,   51,   52,   53,   54,   55,   56,
 /*    50 */    57,   58,   59,   32,    8,    9,   10,   11,   12,   13,
 /*    60 */    14,   15,   16,   17,   18,   19,   20,   21,   22,   23,
 /*    70 */    24,  148,   32,   32,   81,   82,   30,  154,  155,  156,
 /*    80 */   157,  158,   41,   42,   91,   92,   93,   94,   95,   96,
 /*    90 */    49,   50,   51,   52,   53,   54,   55,   56,   57,    6,
 /*   100 */    59,   60,   61,   62,  108,   64,  110,   81,  106,   68,
 /*   110 */    69,   70,   71,   40,   47,   74,   32,   91,   92,   93,
 /*   120 */    94,   95,   96,   30,   32,   41,   42,   30,   87,   88,
 /*   130 */    89,   90,   40,   49,   50,   51,   52,   53,   54,   55,
 /*   140 */    56,   57,   32,   59,   60,   61,   62,   80,   64,   82,
 /*   150 */    40,   84,   68,   69,   70,   71,  159,  160,   74,   32,
 /*   160 */   113,  114,  115,  116,   33,  118,  119,   33,   41,   42,
 /*   170 */   123,   87,   88,   89,   90,    6,   49,   50,   51,   52,
 /*   180 */    53,   54,   55,   56,   57,   35,   59,   60,   61,   62,
 /*   190 */    40,   64,  124,  125,  126,   68,   69,   70,   71,   30,
 /*   200 */    47,   74,   32,   19,   20,   21,   22,   23,   24,    6,
 /*   210 */    37,   41,   42,   40,   87,   88,   89,   90,  117,   49,
 /*   220 */    50,   51,   52,   53,   54,   55,   56,   57,  116,   59,
 /*   230 */    60,   61,   62,   80,   64,  123,   33,   84,   68,   69,
 /*   240 */    70,   71,  148,   33,   74,   32,  152,   22,   23,   24,
 /*   250 */   156,  157,  158,   43,   41,   42,   35,   87,   88,   89,
 /*   260 */    90,   40,   49,   50,   51,   52,   53,   54,   55,   56,
 /*   270 */    57,   33,   59,   60,   61,   62,   33,   64,  159,  160,
 /*   280 */     6,   68,   69,   70,   71,  148,   32,   74,   32,  152,
 /*   290 */   108,    6,  110,  156,  157,  158,    6,   41,   42,   45,
 /*   300 */    87,   88,   89,   90,   47,   49,   50,   51,   52,   53,
 /*   310 */    54,   55,   56,   57,   40,   59,   60,   61,   62,   33,
 /*   320 */    64,  159,  160,   33,   68,   69,   70,   71,  148,   43,
 /*   330 */    74,   32,  116,  153,   45,  119,  156,  157,  158,  123,
 /*   340 */    41,   42,    6,   87,   88,   89,   90,   32,   49,   50,
 /*   350 */    51,   52,   53,   54,   55,   56,   57,   72,   59,   60,
 /*   360 */    61,   62,   43,   64,  159,  160,   30,   68,   69,   70,
 /*   370 */    71,  148,  123,   74,   32,  152,  149,  150,    6,  156,
 /*   380 */   157,  158,    6,   41,   42,  117,   87,   88,   89,   90,
 /*   390 */    33,   49,   50,   51,   52,   53,   54,   55,   56,   57,
 /*   400 */   116,   59,   60,   61,   62,   33,   64,  123,  116,   33,
 /*   410 */    68,   69,   70,   71,  148,  123,   74,   32,  152,    6,
 /*   420 */    32,    6,  156,  157,  158,   66,   41,   42,   40,   87,
 /*   430 */    88,   89,   90,  151,   49,   50,   51,   52,   53,   54,
 /*   440 */    55,   56,   57,   30,   59,   60,   61,   62,   33,   64,
 /*   450 */   159,  160,    6,   68,   69,   70,   71,  148,   83,   74,
 /*   460 */    32,  152,    6,  159,  160,  156,  157,  158,   45,   41,
 /*   470 */    42,  117,   87,   88,   89,   90,   30,   49,   50,   51,
 /*   480 */    52,   53,   54,   55,   56,   57,   30,   59,   60,   61,
 /*   490 */    62,  108,   64,  110,   43,    6,   68,   69,   70,   71,
 /*   500 */   148,  151,   74,   32,   80,  153,   82,    6,  156,  157,
 /*   510 */   158,   33,   41,   42,  151,   87,   88,   89,   90,   30,
 /*   520 */    49,   50,   51,   52,   53,   54,   55,   56,   57,   32,
 /*   530 */    59,   60,   61,   62,   33,   64,  159,  160,    6,   68,
 /*   540 */    69,   70,   71,  148,   40,   74,   32,  152,   80,   83,
 /*   550 */    82,  156,  157,  158,  151,   41,   42,    6,   87,   88,
 /*   560 */    89,   90,   30,   49,   50,   51,   52,   53,   54,   55,
 /*   570 */    56,   57,   43,   59,   60,   61,   62,   32,   64,  159,
 /*   580 */   160,   30,   68,   69,   70,   71,  148,  151,   74,   32,
 /*   590 */   152,   32,  117,    6,  156,  157,  158,    6,   41,   42,
 /*   600 */     6,   87,   88,   89,   90,   33,   49,   50,   51,   52,
 /*   610 */    53,   54,   55,   56,   57,   32,   59,   60,   61,   62,
 /*   620 */    33,   64,  159,  160,   33,   68,   69,   70,   71,  148,
 /*   630 */   121,   74,   32,  124,  125,  126,  155,  156,  157,  158,
 /*   640 */    81,   41,   42,  151,   87,   88,   89,   90,   32,   49,
 /*   650 */    50,   51,   52,   53,   54,   55,   56,   57,   41,   59,
 /*   660 */    60,   61,   62,   40,   64,  146,  147,   32,   68,   69,
 /*   670 */    70,   71,  148,  151,   74,   32,  152,   83,  117,    6,
 /*   680 */   156,  157,  158,    6,   41,   42,   41,   87,   88,   89,
 /*   690 */    90,   33,   49,   50,   51,   52,   53,   54,   55,   56,
 /*   700 */    57,   14,   59,   60,   61,   62,   33,   64,   47,   41,
 /*   710 */    33,   68,   69,   70,   71,  148,   81,   74,   32,  152,
 /*   720 */     6,   32,    6,  156,  157,  158,    6,   41,   42,   32,
 /*   730 */    87,   88,   89,   90,   83,   49,   50,   51,   52,   53,
 /*   740 */    54,   55,   56,   57,   30,   59,   60,   61,   62,   33,
 /*   750 */    64,   32,  151,   33,   68,   69,   70,   71,  148,  151,
 /*   760 */    74,   32,  151,  153,  110,  117,  156,  157,  158,  151,
 /*   770 */    41,   42,   69,   87,   88,   89,   90,   32,   49,   50,
 /*   780 */    51,   52,   53,   54,   55,   56,   57,  150,   59,   60,
 /*   790 */    61,   62,  107,   64,   41,   32,   36,   68,   69,   70,
 /*   800 */    71,  148,  121,   74,   32,  124,  125,  126,   47,  156,
 /*   810 */   157,  158,   32,   41,   42,  107,   87,   88,   89,   90,
 /*   820 */   167,   49,   50,   51,   52,   53,   54,   55,   56,   57,
 /*   830 */    33,   59,   60,   61,   62,   32,   64,  107,   40,   43,
 /*   840 */    68,   69,   70,   71,  148,  107,   74,   32,  152,   47,
 /*   850 */    32,   40,  156,  157,  158,   36,   41,   42,   32,   87,
 /*   860 */    88,   89,   90,  107,   49,   50,   51,   52,   53,   54,
 /*   870 */    55,   56,   57,   32,   59,   60,   61,   62,   47,   64,
 /*   880 */    47,   32,   40,   68,   69,   70,   71,  148,   32,   74,
 /*   890 */    32,  152,   47,   32,   32,  156,  157,  158,   63,   41,
 /*   900 */    42,   72,   87,   88,   89,   90,  107,   49,   50,   51,
 /*   910 */    52,   53,   54,   55,   56,   57,   32,   59,   60,   61,
 /*   920 */    62,   32,   64,   33,    0,   40,   68,   69,   70,   71,
 /*   930 */   148,  121,   74,   32,  124,  125,  126,  116,  156,  157,
 /*   940 */   158,  120,   41,   42,  123,   87,   88,   89,   90,   32,
 /*   950 */    49,   50,   51,   52,   53,   54,   55,   56,   57,  107,
 /*   960 */    59,   60,   61,   62,   33,   64,  106,   32,   69,   68,
 /*   970 */    69,   70,   71,  148,   40,   74,   32,  107,   33,   33,
 /*   980 */    43,  156,  157,  158,   66,   41,   42,   40,   87,   88,
 /*   990 */    89,   90,   32,   49,   50,   51,   52,   53,   54,   55,
 /*  1000 */    56,   57,   63,   59,   60,   61,   62,   40,   64,  161,
 /*  1010 */    47,  147,   68,   69,   70,   71,  148,   41,   74,   32,
 /*  1020 */   117,  117,   40,    6,  156,  157,  158,   43,   41,   42,
 /*  1030 */    32,   87,   88,   89,   90,   33,   49,   50,   51,   52,
 /*  1040 */    53,   54,   55,   56,   57,   32,   59,   60,   61,   62,
 /*  1050 */    32,   64,  160,   32,   47,   68,   69,   70,   71,  148,
 /*  1060 */   107,   74,   32,   40,   33,  117,  161,  156,  157,  158,
 /*  1070 */    66,   41,   42,   33,   87,   88,   89,   90,   43,   49,
 /*  1080 */    50,   51,   52,   53,   54,   55,   56,   57,   33,   59,
 /*  1090 */    60,   61,   62,   33,   64,   32,   40,  111,   68,   69,
 /*  1100 */    70,   71,  116,   66,   74,   32,  120,   14,  168,  123,
 /*  1110 */    75,   76,   77,   78,   79,   42,  168,   87,   88,   89,
 /*  1120 */    90,  168,   49,   50,   51,   52,   53,   54,   55,   56,
 /*  1130 */    57,  168,   59,   60,   61,   62,  168,   64,  168,  168,
 /*  1140 */   168,   68,   69,   70,   71,  148,  168,   74,  105,  148,
 /*  1150 */   168,  168,  168,  156,  157,  158,  168,  156,  157,  158,
 /*  1160 */    87,   88,   89,   90,  168,  122,  165,  166,  167,   41,
 /*  1170 */   127,  128,  129,  130,  131,  132,  133,  134,  135,  136,
 /*  1180 */   137,  138,  139,  140,  141,  142,  143,  144,  105,  114,
 /*  1190 */   115,  116,  168,   65,  119,   67,  168,  168,  123,  156,
 /*  1200 */   157,  158,   35,  168,   37,  122,  168,   40,  168,  168,
 /*  1210 */   127,  128,  129,  130,  131,  132,  133,  134,  135,  136,
 /*  1220 */   137,  138,  139,  140,  141,  142,  143,  144,  105,  168,
 /*  1230 */   168,  112,  168,  114,  115,  116,  168,  118,  119,  156,
 /*  1240 */   157,  158,  123,  168,  168,  122,  168,  168,  168,  168,
 /*  1250 */   127,  128,  129,  130,  131,  132,  133,  134,  135,  136,
 /*  1260 */   137,  138,  139,  140,  141,  142,  143,  144,  105,  168,
 /*  1270 */   168,  145,  168,  168,  148,  145,  168,  168,  148,  156,
 /*  1280 */   157,  158,  156,  157,  158,  122,  156,  157,  158,  168,
 /*  1290 */   127,  128,  129,  130,  131,  132,  133,  134,  135,  136,
 /*  1300 */   137,  138,  139,  140,  141,  142,  143,  144,  148,  168,
 /*  1310 */    35,  105,   37,  168,  168,   40,  156,  157,  158,  156,
 /*  1320 */   157,  158,    1,    2,    3,    4,    5,  168,  122,  168,
 /*  1330 */   168,  168,  168,  127,  128,  129,  130,  131,  132,  133,
 /*  1340 */   134,  135,  136,  137,  138,  139,  140,  141,  142,  143,
 /*  1350 */   144,  105,  114,   32,  116,  168,  118,  119,  168,   38,
 /*  1360 */    39,  123,  156,  157,  158,  168,  168,   46,  122,   48,
 /*  1370 */   168,  168,  168,  127,  128,  129,  130,  131,  132,  133,
 /*  1380 */   134,  135,  136,  137,  138,  139,  140,  141,  142,  143,
 /*  1390 */   144,  105,  168,  168,  168,  168,  168,  168,  145,  168,
 /*  1400 */   168,  148,  156,  157,  158,  168,  168,  168,  122,  156,
 /*  1410 */   157,  158,  168,  127,  128,  129,  130,  131,  132,  133,
 /*  1420 */   134,  135,  136,  137,  138,  139,  140,  141,  142,  143,
 /*  1430 */   144,  148,  168,  168,  105,  168,  168,  168,  168,  156,
 /*  1440 */   157,  158,  156,  157,  158,    1,    2,    3,    4,    5,
 /*  1450 */   168,  122,  168,  168,  168,  168,  127,  128,  129,  130,
 /*  1460 */   131,  132,  133,  134,  135,  136,  137,  138,  139,  140,
 /*  1470 */   141,  142,  143,  144,  105,  168,   32,  168,  168,  168,
 /*  1480 */   168,  168,   38,   39,  168,  156,  157,  158,  168,  168,
 /*  1490 */    46,  122,   48,  168,  168,  168,  127,  128,  129,  130,
 /*  1500 */   131,  132,  133,  134,  135,  136,  137,  138,  139,  140,
 /*  1510 */   141,  142,  143,  144,  105,  168,  168,  168,  168,  168,
 /*  1520 */   168,  145,  168,  168,  148,  156,  157,  158,  168,  168,
 /*  1530 */   168,  122,  156,  157,  158,  168,  127,  128,  129,  130,
 /*  1540 */   131,  132,  133,  134,  135,  136,  137,  138,  139,  140,
 /*  1550 */   141,  142,  143,  144,  168,  168,  168,  105,  168,  168,
 /*  1560 */   168,  168,  168,  168,  168,  156,  157,  158,    1,    2,
 /*  1570 */     3,    4,    5,  168,  122,  168,  168,  168,  168,  127,
 /*  1580 */   128,  129,  130,  131,  132,  133,  134,  135,  136,  137,
 /*  1590 */   138,  139,  140,  141,  142,  143,  144,  105,  168,  168,
 /*  1600 */   168,  168,  168,  168,  168,   38,   39,  168,  156,  157,
 /*  1610 */   158,  168,  168,   46,  122,   48,  168,  168,  168,  127,
 /*  1620 */   128,  129,  130,  131,  132,  133,  134,  135,  136,  137,
 /*  1630 */   138,  139,  140,  141,  142,  143,  144,  105,  168,  168,
 /*  1640 */   168,  168,  168,  168,  145,  168,  168,  148,  156,  157,
 /*  1650 */   158,  168,  168,  168,  122,  156,  157,  158,  168,  127,
 /*  1660 */   128,  129,  130,  131,  132,  133,  134,  135,  136,  137,
 /*  1670 */   138,  139,  140,  141,  142,  143,  144,  168,  168,  168,
 /*  1680 */   105,  168,  168,  168,  168,  168,  168,  168,  156,  157,
 /*  1690 */   158,    1,    2,    3,    4,    5,  168,  122,  168,  168,
 /*  1700 */   168,  168,  127,  128,  129,  130,  131,  132,  133,  134,
 /*  1710 */   135,  136,  137,  138,  139,  140,  141,  142,  143,  144,
 /*  1720 */   105,  168,  168,  168,  168,  168,  168,  168,   38,   39,
 /*  1730 */   168,  156,  157,  158,   44,  168,  168,  122,   48,  168,
 /*  1740 */   168,  168,  127,  128,  129,  130,  131,  132,  133,  134,
 /*  1750 */   135,  136,  137,  138,  139,  140,  141,  142,  143,  144,
 /*  1760 */   105,  168,  168,  168,  168,  168,  168,  168,  168,  168,
 /*  1770 */   148,  156,  157,  158,  168,  168,  168,  122,  156,  157,
 /*  1780 */   158,  168,  127,  128,  129,  130,  131,  132,  133,  134,
 /*  1790 */   135,  136,  137,  138,  139,  140,  141,  142,  143,  144,
 /*  1800 */   168,  168,  168,  105,  168,  168,  168,  168,  148,  168,
 /*  1810 */   168,  156,  157,  158,  168,  168,  156,  157,  158,  168,
 /*  1820 */   168,  168,  168,  163,  164,  127,  128,  129,  130,  131,
 /*  1830 */   132,  133,  134,  135,  136,  137,  138,  139,  140,  141,
 /*  1840 */   142,  143,  144,    7,  168,  168,  168,  168,  168,   13,
 /*  1850 */   168,  168,  168,  168,  156,  157,  158,  168,  168,  168,
 /*  1860 */   168,   25,   26,   27,   28,   29,   30,  168,   32,    7,
 /*  1870 */   168,   31,  168,  168,   34,   13,   36,  168,   38,   39,
 /*  1880 */    43,   45,   42,   47,  168,  168,  168,   25,   26,   27,
 /*  1890 */    28,   29,  148,  168,   32,  148,  168,  168,  168,  168,
 /*  1900 */   156,  157,  158,  156,  157,  158,   43,   45,  164,   47,
 /*  1910 */   163,  164,   75,   76,   77,   78,   79,   81,   82,   82,
 /*  1920 */   168,  168,   85,   86,  168,  168,  168,   91,   92,   93,
 /*  1930 */    94,   95,   96,  168,  168,   73,  168,  168,   75,   76,
 /*  1940 */    77,   78,   79,   81,   82,   82,  168,  168,  168,  168,
 /*  1950 */   168,  168,  168,   91,   92,   93,   94,   95,   96,  168,
 /*  1960 */   168,    7,  148,  168,  168,  168,  168,   13,  168,    7,
 /*  1970 */   156,  157,  158,  168,  168,   13,  168,  163,  164,   25,
 /*  1980 */    26,   27,   28,   29,  168,  168,   32,   25,   26,   27,
 /*  1990 */    28,   29,  168,  168,   32,   33,  168,  168,  168,   45,
 /*  2000 */   168,   47,    7,  148,  168,  168,  168,   45,   13,   47,
 /*  2010 */   168,  156,  157,  158,  168,  168,  168,  168,  163,  164,
 /*  2020 */    25,   26,   27,   28,   29,   30,  168,   32,  168,  168,
 /*  2030 */   168,  168,  168,  168,  168,   81,   82,   83,  168,  168,
 /*  2040 */    45,  168,   47,   81,   82,   91,   92,   93,   94,   95,
 /*  2050 */    96,  168,  168,   91,   92,   93,   94,   95,   96,    8,
 /*  2060 */     9,   10,   11,   12,   13,   14,   15,   16,   17,   18,
 /*  2070 */    19,   20,   21,   22,   23,   24,   81,   82,  168,  168,
 /*  2080 */   168,  168,  168,  168,   33,  168,   91,   92,   93,   94,
 /*  2090 */    95,   96,  168,  148,    7,  168,  168,  168,  168,  168,
 /*  2100 */    13,  156,  157,  158,  168,  168,  168,  168,  163,  164,
 /*  2110 */   168,  168,   25,   26,   27,   28,   29,   30,  168,   32,
 /*  2120 */     7,  148,  168,  168,   47,  168,   13,  168,  168,  156,
 /*  2130 */   157,  158,   45,  168,   47,  168,  163,  164,   25,   26,
 /*  2140 */    27,   28,   29,   66,  168,   32,  168,  168,  168,  168,
 /*  2150 */   168,  168,  168,  125,  168,  168,  148,   80,   45,   82,
 /*  2160 */    47,   84,  168,  168,  156,  157,  158,  168,   81,   82,
 /*  2170 */   162,  168,  168,  165,  166,  167,  148,  168,   91,   92,
 /*  2180 */    93,   94,   95,   96,  156,  157,  158,  168,  168,  148,
 /*  2190 */   168,  168,  148,  168,   81,   82,   83,  156,  157,  158,
 /*  2200 */   156,  157,  158,  168,   91,   92,   93,   94,   95,   96,
 /*  2210 */   168,  148,    7,  168,  148,  168,  168,  148,   13,  156,
 /*  2220 */   157,  158,  156,  157,  158,  156,  157,  158,  168,  168,
 /*  2230 */    25,   26,   27,   28,   29,   30,  168,   32,    7,  148,
 /*  2240 */   168,  168,  168,  168,   13,  168,  168,  156,  157,  158,
 /*  2250 */    45,  168,   47,  168,  168,  168,   25,   26,   27,   28,
 /*  2260 */    29,  168,  148,   32,  168,  148,  168,  168,  168,  168,
 /*  2270 */   156,  157,  158,  156,  157,  158,   45,  168,   47,  168,
 /*  2280 */   148,  168,  168,  148,  168,  168,   81,   82,  156,  157,
 /*  2290 */   158,  156,  157,  158,  148,  168,   91,   92,   93,   94,
 /*  2300 */    95,   96,  156,  157,  158,  168,  168,  168,  168,  168,
 /*  2310 */   168,  168,   81,   82,   83,  168,    1,    2,    3,    4,
 /*  2320 */     5,  168,   91,   92,   93,   94,   95,   96,  168,  148,
 /*  2330 */     7,  168,  148,  168,  168,  148,   13,  156,  157,  158,
 /*  2340 */   156,  157,  158,  156,  157,  158,  168,  168,   25,   26,
 /*  2350 */    27,   28,   29,   38,   39,   32,    7,  148,  168,  168,
 /*  2360 */   168,   46,   13,   48,  168,  156,  157,  158,   45,  168,
 /*  2370 */    47,  168,  168,  168,   25,   26,   27,   28,   29,  168,
 /*  2380 */   148,   32,  168,  148,  168,  168,  168,  168,  156,  157,
 /*  2390 */   158,  156,  157,  158,   45,  168,   47,  168,  168,  148,
 /*  2400 */   168,  168,  168,  168,   81,   82,   83,  156,  157,  158,
 /*  2410 */   168,  168,  168,  168,   91,   92,   93,   94,   95,   96,
 /*  2420 */   168,  168,   73,  168,  148,  168,  168,  148,  168,  168,
 /*  2430 */    81,   82,  156,  157,  158,  156,  157,  158,  168,  168,
 /*  2440 */    91,   92,   93,   94,   95,   96,  168,  168,    7,  148,
 /*  2450 */   168,  168,  168,  168,   13,  168,    7,  156,  157,  158,
 /*  2460 */   168,  168,   13,  168,  168,  168,   25,   26,   27,   28,
 /*  2470 */    29,   30,  168,   32,   25,   26,   27,   28,   29,   30,
 /*  2480 */   168,   32,  168,  168,  168,  168,   45,  168,   47,  168,
 /*  2490 */   168,  168,  168,  168,   45,  168,   47,    8,    9,   10,
 /*  2500 */    11,   12,   13,   14,   15,   16,   17,   18,   19,   20,
 /*  2510 */    21,   22,   23,   24,  168,  168,  148,  168,  168,   13,
 /*  2520 */   168,  168,   81,   82,  156,  157,  158,  168,  168,   40,
 /*  2530 */    81,   82,   91,   92,   93,   94,   95,   96,   32,  168,
 /*  2540 */    91,   92,   93,   94,   95,   96,  168,  168,    7,  168,
 /*  2550 */   168,  168,  168,  168,   13,   49,   50,   51,   52,   53,
 /*  2560 */    54,   55,   56,   57,   58,   59,   25,   26,   27,   28,
 /*  2570 */    29,  148,  168,   32,  168,  168,  168,  168,  168,  156,
 /*  2580 */   157,  158,  168,  168,  168,  168,   45,  168,   47,  168,
 /*  2590 */   168,  168,  168,  168,  168,    8,    9,   10,   11,   12,
 /*  2600 */    13,   14,   15,   16,   17,   18,   19,   20,   21,   22,
 /*  2610 */    23,   24,  168,  168,  168,  168,  168,  168,  168,  168,
 /*  2620 */   168,  168,   81,   82,  168,  168,  168,   40,  168,  168,
 /*  2630 */   168,  168,   91,   92,   93,   94,   95,   96,  168,  168,
 /*  2640 */   168,    8,    9,   10,   11,   12,   13,   14,   15,   16,
 /*  2650 */    17,   18,   19,   20,   21,   22,   23,   24,   11,   12,
 /*  2660 */    13,   14,   15,   16,   17,   18,   19,   20,   21,   22,
 /*  2670 */    23,   24,  168,   40,  168,  168,  168,  168,  168,  168,
 /*  2680 */   168,  168,    8,    9,   10,   11,   12,   13,   14,   15,
 /*  2690 */    16,   17,   18,   19,   20,   21,   22,   23,   24,    8,
 /*  2700 */     9,   10,   11,   12,   13,   14,   15,   16,   17,   18,
 /*  2710 */    19,   20,   21,   22,   23,   24,    8,    9,   10,   11,
 /*  2720 */    12,   13,   14,   15,   16,   17,   18,   19,   20,   21,
 /*  2730 */    22,   23,   24,    7,  168,  168,  168,  168,  168,   13,
 /*  2740 */   168,  168,  168,  168,  168,  168,  168,  168,   40,  168,
 /*  2750 */   168,   25,   26,   27,   28,   29,  168,   83,   32,  168,
 /*  2760 */   168,  168,  168,  168,  168,  168,  168,  168,  168,  168,
 /*  2770 */   168,   45,  168,   47,   83,  168,  168,  168,  168,  168,
 /*  2780 */     8,    9,   10,   11,   12,   13,   14,   15,   16,   17,
 /*  2790 */    18,   19,   20,   21,   22,   23,   24,  168,  168,  168,
 /*  2800 */   168,  168,  168,  168,  168,  168,  168,   81,   82,    1,
 /*  2810 */     2,    3,    4,    5,  168,  168,  168,   91,   92,   93,
 /*  2820 */    94,   95,   96,  168,  168,  168,    8,    9,   10,   11,
 /*  2830 */    12,   13,   14,   15,   16,   17,   18,   19,   20,   21,
 /*  2840 */    22,   23,   24,  168,  168,  168,   38,   39,  168,   41,
 /*  2850 */    42,   33,   44,  168,  168,   83,   48,  168,  168,  168,
 /*  2860 */     8,    9,   10,   11,   12,   13,   14,   15,   16,   17,
 /*  2870 */    18,   19,   20,   21,   22,   23,   24,    8,    9,   10,
 /*  2880 */    11,   12,   13,   14,   15,   16,   17,   18,   19,   20,
 /*  2890 */    21,   22,   23,   24,    9,   10,   11,   12,   13,   14,
 /*  2900 */    15,   16,   17,   18,   19,   20,   21,   22,   23,   24,
 /*  2910 */    13,  168,  168,  168,  168,  168,  168,  168,  168,  168,
 /*  2920 */   168,   13,  168,  168,  168,  168,  168,   30,  168,   32,
 /*  2930 */   168,  168,  168,  168,  168,   83,  168,  168,   30,   43,
 /*  2940 */    32,  168,  168,  168,  168,  168,   49,   50,   51,   52,
 /*  2950 */    53,   54,   55,   56,   57,   58,   59,   49,   50,   51,
 /*  2960 */    52,   53,   54,   55,   56,   57,   58,   59,   13,  168,
 /*  2970 */   168,   75,   76,   77,   78,   79,   80,  168,   82,   13,
 /*  2980 */    84,   85,   86,  168,  168,   30,  168,   32,  168,    1,
 /*  2990 */     2,    3,    4,    5,  168,  168,   30,  168,   32,  168,
 /*  3000 */   168,  168,  168,  168,   49,   50,   51,   52,   53,   54,
 /*  3010 */    55,   56,   57,   58,   59,   49,   50,   51,   52,   53,
 /*  3020 */    54,   55,   56,   57,   58,   59,   38,   39,    0,   41,
 /*  3030 */    42,  168,   44,  168,  168,  168,   48,  168,    1,    2,
 /*  3040 */     3,    4,    5,  168,  168,  168,  168,  168,  168,  168,
 /*  3050 */   168,  168,  168,  168,  168,  168,  168,  168,  168,   31,
 /*  3060 */   168,  168,   34,  168,   36,  168,   38,   39,  168,  168,
 /*  3070 */    42,  168,  168,  168,  168,   38,   39,  168,   41,   42,
 /*  3080 */   168,   44,  168,  168,  168,   48,  168,    1,    2,    3,
 /*  3090 */     4,    5,  168,  168,  168,  168,  168,  168,  168,  168,
 /*  3100 */   168,  168,  168,  168,  168,  168,  168,  168,  168,  168,
 /*  3110 */   168,  168,  168,  168,  168,  168,  168,  168,  168,  168,
 /*  3120 */   168,  168,  168,  168,   38,   39,  168,   41,   42,  168,
 /*  3130 */    44,  168,  168,  168,   48,  168,    1,    2,    3,    4,
 /*  3140 */     5,    1,    2,    3,    4,    5,  168,  168,  168,  168,
 /*  3150 */   168,  168,  168,  168,  168,  168,  168,  168,  168,  168,
 /*  3160 */   168,  168,  168,  168,  168,  168,  168,  168,  168,  168,
 /*  3170 */   168,  168,  168,   38,   39,  168,   41,   42,   38,   39,
 /*  3180 */   168,   41,   42,   48,  168,  168,  168,  168,   48,  168,
 /*  3190 */     1,    2,    3,    4,    5,    1,    2,    3,    4,    5,
 /*  3200 */   168,  168,  168,  168,  168,  168,  168,  168,  168,  168,
 /*  3210 */   168,  168,  168,  168,  168,  168,  168,  168,  168,  168,
 /*  3220 */   168,  168,  168,  168,  168,  168,  168,   38,   39,  168,
 /*  3230 */    41,   42,   38,   39,  168,   41,   42,   48,  168,  168,
 /*  3240 */   168,  168,   48,  168,    1,    2,    3,    4,    5,    1,
 /*  3250 */     2,    3,    4,    5,  168,  168,  168,  168,  168,  168,
 /*  3260 */   168,  168,  168,  168,  168,  168,  168,  168,  168,  168,
 /*  3270 */   168,  168,  168,  168,  168,  168,  168,  168,  168,  168,
 /*  3280 */   168,   38,   39,  168,   41,   42,   38,   39,  168,   41,
 /*  3290 */    42,   48,  168,  168,  168,  168,   48,  168,    1,    2,
 /*  3300 */     3,    4,    5,    1,    2,    3,    4,    5,  168,    1,
 /*  3310 */     2,    3,    4,    5,    1,    2,    3,    4,    5,  168,
 /*  3320 */     1,    2,    3,    4,    5,  168,  168,  168,  168,  168,
 /*  3330 */   168,  168,  168,  168,  168,   38,   39,  168,   41,   42,
 /*  3340 */    38,   39,  168,   41,   42,   48,   38,   39,  168,  168,
 /*  3350 */    48,   38,   39,  168,   46,  168,   48,   38,   39,   46,
 /*  3360 */   168,   48,  168,  168,  168,  168,  168,   48,
};
#define YY_SHIFT_USE_DFLT (-43)
static short yy_shift_ofst[] = {
 /*     0 */  1840,  924, 3028,  -43,  -43,  -43,  -43,  -43,  689, 1055,
 /*    10 */   -43,  818,  221,  -43,  998,  934,  -43, 3248,  -43, 3189,
 /*    20 */   -43,  -43, 2315, 1018,  963, 2966,  946,  -43,  336, 1002,
 /*    30 */   -43, 2506,  -43,  529,   26,  -43,  -43,  -43,  -43,  -43,
 /*    40 */   -43,  -43,  -43,  884,  937,   26,  -43,  497,  796,   26,
 /*    50 */   -43,  697,  687,  545,  -43,  -43,  -43,  -43,  -43,  -43,
 /*    60 */   -43,  -43,  -43,  -43,  -43,  -43,  -43,  -43,  -43,  -43,
 /*    70 */   -43,  -43,  -43,  -43,  -43, 3319, 3308,  315,  257, 2908,
 /*    80 */   131,  -43,  489, 1031,  -43,  -43,  -43,  960, 1167,  -43,
 /*    90 */   935,  173,  -43,  719,  274,  -43, 3037,  -43,  976,  -43,
 /*   100 */  2988, 3297,  -43, 1567,  745,  802, 2897,  811,  385,  -43,
 /*   110 */   600,  -43,  -43,  -43,  -43,  -43,  -43,  -43,  -43,  -43,
 /*   120 */   -43,  -43,  -43,  -43,  -43,  -43,  -43,  -43,  -43,  -43,
 /*   130 */   -43,  931,  -43,  890,  -43, 2541,  842,  256,  686,  835,
 /*   140 */   885,  471,  299,  -43, 2541,  947,  -26,  -43, 1128,  -43,
 /*   150 */   -43,   26, 1004, 1073, 1073,   73,  170,  729,  -43, 2541,
 /*   160 */   504,  213,  901,  -43,  623,  987,  428,  703, 2541,  797,
 /*   170 */   -43, 2541,  -43, 2541,  861,  -43,  468, 2541, 2541, 2869,
 /*   180 */  2541,  184, 2541,  184, 2541,  184, 2541,  184, 2541,  184,
 /*   190 */  2541,  184, 2541,  184, 2541,  225, 2541,  225, 2541,  -43,
 /*   200 */  2541,  -43, 2541,  -43, 2541,  225, 2541, 2647, 2541, 2647,
 /*   210 */  2541, 2885,   -7,   46,  -43,   97, 2541,  -43,   40, 1093,
 /*   220 */  2541,  184,   67,  110,  833, 2205, 2869, 1013, 1017,  861,
 /*   230 */   -43, 2541,  -43,  -43,  -43,  -43,  -43,  -43,  -43,  -43,
 /*   240 */   -43, 2323, 2869, 2077,  254,  831, 2441,  849,  845, 1836,
 /*   250 */   -43,  551,  -43, 2541,  -43,  -43,  -43,  -43,  -43,  714,
 /*   260 */   -43,  -43,  -43, 2541, 2852,  -43, 1995,   93,  -43,  -43,
 /*   270 */  1037,  -42,  -43,  594,  -43, 2726,  -43,  359, 2541,  -43,
 /*   280 */   -43,  -43,  532,  -43,  -43,  559,  645,  661, 2087,  169,
 /*   290 */   -43,  -43,  753,  761, 2449,  446,  -43,  -43, 2674,  -43,
 /*   300 */   616,  -43,  184, 2869,  917,  285, 1862, 2708, 1073,  815,
 /*   310 */   -43,  763,  376,  -43,  763,  -43, 2896,  -43,  -43,  -43,
 /*   320 */   -43,  -43,  -43, 2541,  -43, 2869,   92, 1837, 2541,  -43,
 /*   330 */  2113, 1035, 2541,  -43,  651, 1035, 2541,  -43, 2691,  -43,
 /*   340 */   -43,  635,  668, 1035, 2541,  -43,  617, 1035, 2541,  -43,
 /*   350 */   583, 1863, 2541,  -43, 1954, 1035, 2541,  -43,  466, 1035,
 /*   360 */  2541,  -43, 2231, 1035, 2541,  -43,  375, 1035, 2541,  -43,
 /*   370 */   -43,  -43,  -43, 2541,  716,  -43, 2541,  -43, 2869,  -43,
 /*   380 */   243,  -43,  238,  -43,  134,  -43, 1962, 2051,  -43,  -43,
 /*   390 */    21,  424, 1063, 1060,  -43, 2541, 2772, 1040,  -43, 2541,
 /*   400 */  2818,  -43,  856,  677,  -43,  856,  -43,  984,   26,  -43,
 /*   410 */   -43,  856,  720,  -43,  856,  587,  -43,  856,  415,  -43,
 /*   420 */   856,  203,  -43,  856,  501,  -43,  856,  673,  -43,  856,
 /*   430 */   290,  -43,  856,  372,  -43,  856,  591,  -43,  153,  841,
 /*   440 */   831,  388,  833,  -43,  -43, 2541, 2633, 1073,  514,  -43,
 /*   450 */   862,  829, 2349, 2587, 1073,  127,  -43, 2541, 2489, 1073,
 /*   460 */   858,  -43,  899, 2541,  945,  -43,  -43,  -43,  918, 1073,
 /*   470 */  1073,  -43,  -43,  939,  967,  557,   41,  -43,  -43,  456,
 /*   480 */   982,   84,  -43,  342,  -43, 3319, 3313, 1021, 1007, 2955,
 /*   490 */  1023,  944,  -43,  772,  -43,  413, 1056, 1030,  -43,  643,
 /*   500 */   -43, 3086, 3243,  -43,  -43, 1690,  289,  319,   26,  357,
 /*   510 */   -43,  423,  451,   26,  478,  -43,  -43, 1690, 1444,  210,
 /*   520 */   -43,   26,  572,  -43, 1321,  286,  -43,   26,  658,  -43,
 /*   530 */   -43, 2808, 3302, 3140, 3319, 3135,  -43, 3194,  719,  -43,
 /*   540 */   -43,  -43,  719,  274,  -43,  760,  780, 1275,  -43,  803,
 /*   550 */   798,  -43,  719,  274,  -43,  819,  826,  150,  -43,  889,
 /*   560 */   798,  -43,  -43,
};
#define YY_REDUCE_USE_DFLT (-144)
static short yy_reduce_ofst[] = {
 /*     0 */   -91, -144, -100, -144, -144, -144, -144, -144, -144, -144,
 /*    10 */  -144, -144,    2, -144, -144,  860, -144,  986, -144,  821,
 /*    20 */  -144, -144,  249, -144, -144,  509, -144, -144, -144, -144,
 /*    30 */  -144,   68, -144, -144,  101, -144, -144, -144, -144, -144,
 /*    40 */  -144, -144, -144, -144, -144,  903, -144, -144, -144,  648,
 /*    50 */  -144, -144, -144, -144, -144, -144, -144, -144, -144, -144,
 /*    60 */  -144, -144, -144, -144, -144, -144, -144, -144, -144, -144,
 /*    70 */  -144, -144, -144, -144, -144,  112,  249, -144, -144,  -90,
 /*    80 */  -144, -144, -144, -144, -144, -144, -144, -144,  870, -144,
 /*    90 */  -144,  852, -144,   -4,  799, -144,  -86, -144, -144, -144,
 /*   100 */    47,  216, -144,  249, -144, -144,  810, -144, 1532, -144,
 /*   110 */  1698, -144, -144, -144, -144, -144, -144, -144, -144, -144,
 /*   120 */  -144, -144, -144, -144, -144, -144, -144, -144, -144, -144,
 /*   130 */  -144, -144, -144, -144, -144, 1130, -144, 1246, 1698, -144,
 /*   140 */  -144, 1163, 1698, -144, 1253, -144,  519, -144,  864, -144,
 /*   150 */  -144,  948, -144, 1575, 1698, -144, 1123, 1698, -144, 1376,
 /*   160 */  -144, 1286, 1698, -144, -144, 1083, 1698, -144, 1499, -144,
 /*   170 */  -144, 2368, -144, 2423,  905, -144, -144,  868,  911, -144,
 /*   180 */   997, -144, 1160, -144, 1283, -144, 1622, -144, 2041, -144,
 /*   190 */  2044, -144, 2063, -144, 2066, -144, 2069, -144, 2091, -144,
 /*   200 */  2114, -144, 2117, -144, 2132, -144, 2135, -144, 2146, -144,
 /*   210 */  2181, -144, 2028, -144, -144, -144, 2187, -144, -144, -144,
 /*   220 */  2209, -144, -144, -144, -144, 1814, -144, -144, -144,  848,
 /*   230 */  -144, 2235, -144, -144, -144, -144, -144, -144, -144, -144,
 /*   240 */  -144, 2008, -144, -144, -144, -144, 1855, -144, -144, 1945,
 /*   250 */  -144, -144, -144, 1744, -144, -144, -144, -144, -144, -144,
 /*   260 */  -144, -144, -144, 2301, -144, -144, 1973, -144, -144, -144,
 /*   270 */  -144, -144, -144, -144, -144, 1001, -144, -144,  653, -144,
 /*   280 */  -144, -144, -144, -144, -144, -144, -144, -144, 1660, -144,
 /*   290 */  -144, -144, -144, -144, 1747, -144, -144, -144, -144, -144,
 /*   300 */  -144, -144, -144, -144, -144, -144,  825, -144, 1206, 1698,
 /*   310 */  -144,  227, -144, -144,  637, -144,  618, -144, -144, -144,
 /*   320 */  -144, -144, -144,  567, -144, -144, -144,  611,  223, -144,
 /*   330 */   180,  608,   94, -144, -144,  601,  137, -144, -144, -144,
 /*   340 */  -144, -144, -144,  522,  266, -144, -144,  492,  739, -144,
 /*   350 */  -144,  436,  309, -144,  352,  403,  395, -144, -144,  363,
 /*   360 */   524, -144,  610,  350,  696, -144, -144,  282,  438, -144,
 /*   370 */  -144, -144, -144,  -77, -144, -144,  481, -144, -144, -144,
 /*   380 */  -144, -144, -144, -144, -144, -144, 2184, -144, -144, -144,
 /*   390 */  -144, -144, -144, -144, -144, 2232, -144, -144, -144,  782,
 /*   400 */  -144, -144,  463, -144, -144,  892, -144, -144,  904, -144,
 /*   410 */  -144,   -3, -144, -144,  420, -144, -144,  291, -144, -144,
 /*   420 */   119, -144, -144,  304, -144, -144, -143, -144, -144,  162,
 /*   430 */  -144, -144,  205, -144, -144,  377, -144, -144, -144, -144,
 /*   440 */  -144, -144, -144, -144, -144, 2251, -144, 1409, 1698, -144,
 /*   450 */  -144, -144, 2276, -144, 1615, 1698, -144, 2279, -144, 1043,
 /*   460 */  1698, -144, -144, 1126, -144, -144, -144, -144, -144, 1329,
 /*   470 */  1698, -144, -144, -144, -144, 1369, 1698, -144, -144, -144,
 /*   480 */  -144, 1452, -144, 1698, -144,  292,  249, -144, -144,  681,
 /*   490 */  -144, 1492, -144, 1698, -144, -144, -144, 1655, -144, 1698,
 /*   500 */  -144, 1238,  216, -144, -144,  292, -144, -144,  268, -144,
 /*   510 */  -144, -144, -144,  354, -144, -144, -144,  284,  249, -144,
 /*   520 */  -144,  475, -144, -144,  249, -144, -144,  561, -144, -144,
 /*   530 */  -144, 1119, 1075,  216,  284,  216, -144,  216,  654, -144,
 /*   540 */  -144, -144,  182,  685, -144, -144, -144,  708, -144, -144,
 /*   550 */   730, -144,  383,  738, -144, -144, -144,  756, -144, -144,
 /*   560 */   953, -144, -144,
};
static YYACTIONTYPE yy_default[] = {
 /*     0 */   830,  830,  830,  565,  567,  568,  569,  570,  830,  830,
 /*    10 */   571,  830,  830,  572,  830,  830,  573,  830,  588,  830,
 /*    20 */   589,  609,  830,  830,  830,  830,  830,  621,  830,  830,
 /*    30 */   622,  830,  635,  637,  830,  640,  819,  820,  821,  822,
 /*    40 */   823,  824,  825,  830,  638,  830,  641,  830,  639,  830,
 /*    50 */   642,  830,  830,  830,  644,  645,  646,  647,  648,  649,
 /*    60 */   650,  651,  652,  653,  654,  636,  625,  627,  628,  629,
 /*    70 */   630,  631,  632,  633,  634,  830,  830,  830,  830,  830,
 /*    80 */   830,  623,  830,  830,  624,  626,  610,  830,  830,  574,
 /*    90 */   830,  830,  575,  830,  830,  577,  830,  583,  830,  584,
 /*   100 */   830,  830,  607,  830,  830,  830,  830,  830,  830,  613,
 /*   110 */   830,  615,  655,  657,  658,  659,  660,  661,  662,  663,
 /*   120 */   664,  665,  666,  667,  668,  669,  670,  671,  672,  673,
 /*   130 */   674,  830,  675,  830,  676,  830,  830,  830,  830,  679,
 /*   140 */   830,  830,  830,  680,  830,  830,  830,  683,  830,  684,
 /*   150 */   685,  830,  830,  687,  688,  830,  830,  830,  691,  830,
 /*   160 */   830,  830,  830,  693,  830,  830,  830,  830,  830,  830,
 /*   170 */   695,  830,  752,  830,  830,  753,  830,  830,  830,  754,
 /*   180 */   830,  756,  830,  757,  830,  758,  830,  759,  830,  760,
 /*   190 */   830,  761,  830,  762,  830,  770,  830,  771,  830,  772,
 /*   200 */   830,  773,  830,  774,  830,  775,  830,  776,  830,  777,
 /*   210 */   830,  778,  830,  830,  763,  830,  830,  764,  830,  830,
 /*   220 */   830,  765,  782,  830,  766,  830,  810,  830,  830,  830,
 /*   230 */   779,  830,  780,  781,  783,  784,  785,  786,  787,  788,
 /*   240 */   789,  830,  818,  782,  830,  767,  830,  830,  792,  830,
 /*   250 */   793,  830,  794,  830,  805,  806,  807,  808,  809,  830,
 /*   260 */   797,  798,  768,  830,  830,  769,  830,  830,  795,  796,
 /*   270 */   783,  784,  790,  830,  791,  830,  811,  830,  830,  813,
 /*   280 */   814,  812,  830,  799,  800,  830,  830,  830,  830,  830,
 /*   290 */   801,  802,  830,  830,  830,  830,  803,  804,  830,  826,
 /*   300 */   830,  827,  755,  828,  830,  830,  830,  830,  830,  830,
 /*   310 */   696,  830,  830,  700,  830,  701,  830,  703,  704,  705,
 /*   320 */   706,  707,  708,  830,  709,  751,  830,  830,  830,  710,
 /*   330 */   830,  830,  830,  713,  830,  830,  830,  714,  830,  720,
 /*   340 */   721,  830,  830,  830,  830,  711,  830,  830,  830,  712,
 /*   350 */   830,  830,  830,  715,  830,  830,  830,  716,  830,  830,
 /*   360 */   830,  717,  830,  830,  830,  718,  830,  830,  830,  719,
 /*   370 */   722,  723,  702,  830,  830,  725,  830,  726,  728,  727,
 /*   380 */   830,  729,  830,  730,  830,  731,  830,  830,  732,  733,
 /*   390 */   830,  830,  830,  830,  734,  830,  830,  830,  735,  830,
 /*   400 */   830,  736,  830,  830,  737,  830,  747,  749,  830,  750,
 /*   410 */   748,  830,  830,  738,  830,  830,  739,  830,  830,  740,
 /*   420 */   830,  830,  741,  830,  830,  742,  830,  830,  743,  830,
 /*   430 */   830,  744,  830,  830,  745,  830,  830,  746,  830,  830,
 /*   440 */   830,  830,  830,  829,  656,  830,  830,  830,  830,  697,
 /*   450 */   830,  830,  830,  830,  830,  830,  698,  830,  830,  830,
 /*   460 */   830,  699,  830,  830,  830,  694,  692,  690,  830,  830,
 /*   470 */   689,  686,  681,  677,  830,  830,  830,  682,  678,  830,
 /*   480 */   830,  830,  614,  830,  616,  830,  830,  830,  830,  830,
 /*   490 */   830,  830,  617,  830,  619,  830,  830,  830,  618,  830,
 /*   500 */   620,  830,  830,  605,  608,  830,  830,  830,  830,  830,
 /*   510 */   611,  830,  830,  830,  830,  612,  599,  830,  830,  830,
 /*   520 */   601,  830,  830,  603,  830,  830,  602,  830,  830,  604,
 /*   530 */   606,  830,  830,  830,  830,  830,  600,  830,  830,  585,
 /*   540 */   587,  586,  830,  830,  576,  830,  830,  830,  578,  830,
 /*   550 */   830,  579,  830,  830,  580,  830,  830,  830,  581,  830,
 /*   560 */   830,  582,  566,
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
  "RETURN",        "UNSET",         "THROW",         "INTEGER",     
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
 /* 192 */ "xx_common_expr ::= xx_common_expr EQUALS xx_common_expr",
 /* 193 */ "xx_common_expr ::= xx_common_expr NOTEQUALS xx_common_expr",
 /* 194 */ "xx_common_expr ::= xx_common_expr IDENTICAL xx_common_expr",
 /* 195 */ "xx_common_expr ::= xx_common_expr NOTIDENTICAL xx_common_expr",
 /* 196 */ "xx_common_expr ::= xx_common_expr LESS xx_common_expr",
 /* 197 */ "xx_common_expr ::= xx_common_expr GREATER xx_common_expr",
 /* 198 */ "xx_common_expr ::= xx_common_expr LESSEQUAL xx_common_expr",
 /* 199 */ "xx_common_expr ::= xx_common_expr GREATEREQUAL xx_common_expr",
 /* 200 */ "xx_common_expr ::= PARENTHESES_OPEN xx_common_expr PARENTHESES_CLOSE",
 /* 201 */ "xx_common_expr ::= PARENTHESES_OPEN xx_parameter_type PARENTHESES_CLOSE xx_common_expr",
 /* 202 */ "xx_common_expr ::= LESS IDENTIFIER GREATER xx_common_expr",
 /* 203 */ "xx_common_expr ::= IDENTIFIER ARROW IDENTIFIER",
 /* 204 */ "xx_common_expr ::= IDENTIFIER DOUBLECOLON IDENTIFIER",
 /* 205 */ "xx_common_expr ::= IDENTIFIER DOUBLECOLON CONSTANT",
 /* 206 */ "xx_common_expr ::= IDENTIFIER SBRACKET_OPEN xx_common_expr SBRACKET_CLOSE",
 /* 207 */ "xx_common_expr ::= xx_common_expr ADD xx_common_expr",
 /* 208 */ "xx_common_expr ::= xx_common_expr SUB xx_common_expr",
 /* 209 */ "xx_common_expr ::= xx_common_expr MUL xx_common_expr",
 /* 210 */ "xx_common_expr ::= xx_common_expr DIV xx_common_expr",
 /* 211 */ "xx_common_expr ::= xx_common_expr MOD xx_common_expr",
 /* 212 */ "xx_common_expr ::= xx_common_expr CONCAT xx_common_expr",
 /* 213 */ "xx_common_expr ::= xx_common_expr AND xx_common_expr",
 /* 214 */ "xx_common_expr ::= xx_common_expr OR xx_common_expr",
 /* 215 */ "xx_common_expr ::= xx_common_expr INSTANCEOF xx_common_expr",
 /* 216 */ "xx_common_expr ::= FETCH IDENTIFIER COMMA xx_isset_expr",
 /* 217 */ "xx_common_expr ::= TYPEOF xx_common_expr",
 /* 218 */ "xx_common_expr ::= CONSTANT",
 /* 219 */ "xx_common_expr ::= IDENTIFIER",
 /* 220 */ "xx_common_expr ::= INTEGER",
 /* 221 */ "xx_common_expr ::= STRING",
 /* 222 */ "xx_common_expr ::= CHAR",
 /* 223 */ "xx_common_expr ::= DOUBLE",
 /* 224 */ "xx_common_expr ::= NULL",
 /* 225 */ "xx_common_expr ::= TRUE",
 /* 226 */ "xx_common_expr ::= FALSE",
 /* 227 */ "xx_common_expr ::= SBRACKET_OPEN SBRACKET_CLOSE",
 /* 228 */ "xx_common_expr ::= SBRACKET_OPEN xx_array_list SBRACKET_CLOSE",
 /* 229 */ "xx_common_expr ::= NEW IDENTIFIER",
 /* 230 */ "xx_common_expr ::= NEW IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 231 */ "xx_common_expr ::= NEW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 232 */ "xx_fcall_expr ::= IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 233 */ "xx_fcall_expr ::= IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 234 */ "xx_scall_expr ::= IDENTIFIER DOUBLECOLON IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 235 */ "xx_scall_expr ::= IDENTIFIER DOUBLECOLON IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 236 */ "xx_mcall_expr ::= IDENTIFIER ARROW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 237 */ "xx_mcall_expr ::= IDENTIFIER ARROW IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 238 */ "xx_mcall_expr ::= IDENTIFIER ARROW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 239 */ "xx_mcall_expr ::= IDENTIFIER ARROW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 240 */ "xx_mcall_expr ::= IDENTIFIER ARROW BRACKET_OPEN STRING BRACKET_CLOSE PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 241 */ "xx_mcall_expr ::= IDENTIFIER ARROW BRACKET_OPEN STRING BRACKET_CLOSE PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 242 */ "xx_common_expr ::= xx_mcall_expr",
 /* 243 */ "xx_common_expr ::= xx_scall_expr",
 /* 244 */ "xx_common_expr ::= xx_fcall_expr",
 /* 245 */ "xx_call_parameters ::= xx_call_parameters COMMA xx_call_parameter",
 /* 246 */ "xx_call_parameters ::= xx_call_parameter",
 /* 247 */ "xx_call_parameter ::= xx_common_expr",
 /* 248 */ "xx_array_list ::= xx_array_list COMMA xx_array_item",
 /* 249 */ "xx_array_list ::= xx_array_item",
 /* 250 */ "xx_array_item ::= xx_array_key COLON xx_array_value",
 /* 251 */ "xx_array_item ::= xx_array_value",
 /* 252 */ "xx_array_key ::= IDENTIFIER",
 /* 253 */ "xx_array_key ::= STRING",
 /* 254 */ "xx_array_key ::= INTEGER",
 /* 255 */ "xx_array_value ::= xx_common_expr",
 /* 256 */ "xx_literal_expr ::= INTEGER",
 /* 257 */ "xx_literal_expr ::= CHAR",
 /* 258 */ "xx_literal_expr ::= STRING",
 /* 259 */ "xx_literal_expr ::= DOUBLE",
 /* 260 */ "xx_literal_expr ::= NULL",
 /* 261 */ "xx_literal_expr ::= FALSE",
 /* 262 */ "xx_literal_expr ::= TRUE",
 /* 263 */ "xx_isset_expr ::= IDENTIFIER SBRACKET_OPEN xx_common_expr SBRACKET_CLOSE",
 /* 264 */ "xx_isset_expr ::= IDENTIFIER ARROW IDENTIFIER",
 /* 265 */ "xx_eval_expr ::= xx_common_expr",
 /* 266 */ "xx_comment ::= COMMENT",
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
// 899 "parser.lemon"
{
	/*if ((yypminor->yy0)) {
		if ((yypminor->yy0)->free_flag) {
			efree((yypminor->yy0)->token);
		}
		efree((yypminor->yy0));
	}*/
}
// 2362 "parser.c"
      break;
    case 99:
// 912 "parser.lemon"
{ json_object_put((yypminor->yy175)); }
// 2367 "parser.c"
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
  { 156, 8 },
  { 156, 7 },
  { 156, 8 },
  { 156, 7 },
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
  { 161, 3 },
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
// 908 "parser.lemon"
{
	status->ret = yymsp[0].minor.yy175;
}
// 2851 "parser.c"
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
      case 242:
      case 243:
      case 244:
      case 247:
      case 255:
      case 265:
// 914 "parser.lemon"
{
	yygotominor.yy175 = yymsp[0].minor.yy175;
}
// 2888 "parser.c"
        break;
      case 2:
      case 36:
      case 42:
      case 44:
      case 46:
      case 62:
      case 92:
      case 122:
// 918 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_list(yymsp[-1].minor.yy175, yymsp[0].minor.yy175);
}
// 2902 "parser.c"
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
      case 246:
      case 249:
// 922 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_list(NULL, yymsp[0].minor.yy175);
}
// 2923 "parser.c"
        break;
      case 8:
// 942 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_namespace(yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(31,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 2932 "parser.c"
        break;
      case 9:
// 946 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_interface(yymsp[-1].minor.yy0, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(34,&yymsp[-2].minor);
}
// 2940 "parser.c"
        break;
      case 10:
// 950 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_interface(yymsp[-3].minor.yy0, yymsp[0].minor.yy175, yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(34,&yymsp[-4].minor);
  yy_destructor(35,&yymsp[-2].minor);
}
// 2949 "parser.c"
        break;
      case 11:
// 954 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class(yymsp[-1].minor.yy0, yymsp[0].minor.yy175, 0, 0, NULL, NULL, status->scanner_state);
  yy_destructor(36,&yymsp[-2].minor);
}
// 2957 "parser.c"
        break;
      case 12:
// 958 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy175, 0, 0, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(36,&yymsp[-4].minor);
  yy_destructor(35,&yymsp[-2].minor);
}
// 2966 "parser.c"
        break;
      case 13:
// 962 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy175, 0, 0, NULL, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(36,&yymsp[-4].minor);
  yy_destructor(37,&yymsp[-2].minor);
}
// 2975 "parser.c"
        break;
      case 14:
// 966 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class(yymsp[-5].minor.yy0, yymsp[0].minor.yy175, 0, 0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(36,&yymsp[-6].minor);
  yy_destructor(35,&yymsp[-4].minor);
  yy_destructor(37,&yymsp[-2].minor);
}
// 2985 "parser.c"
        break;
      case 15:
// 970 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class(yymsp[-1].minor.yy0, yymsp[0].minor.yy175, 1, 0, NULL, NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-3].minor);
  yy_destructor(36,&yymsp[-2].minor);
}
// 2994 "parser.c"
        break;
      case 16:
// 974 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy175, 1, 0, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-5].minor);
  yy_destructor(36,&yymsp[-4].minor);
  yy_destructor(35,&yymsp[-2].minor);
}
// 3004 "parser.c"
        break;
      case 17:
// 978 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy175, 1, 0, NULL, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(38,&yymsp[-5].minor);
  yy_destructor(36,&yymsp[-4].minor);
  yy_destructor(37,&yymsp[-2].minor);
}
// 3014 "parser.c"
        break;
      case 18:
// 982 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class(yymsp[-1].minor.yy0, yymsp[0].minor.yy175, 0, 1, NULL, NULL, status->scanner_state);
  yy_destructor(39,&yymsp[-3].minor);
  yy_destructor(36,&yymsp[-2].minor);
}
// 3023 "parser.c"
        break;
      case 19:
// 986 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy175, 0, 1, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(39,&yymsp[-5].minor);
  yy_destructor(36,&yymsp[-4].minor);
  yy_destructor(35,&yymsp[-2].minor);
}
// 3033 "parser.c"
        break;
      case 20:
      case 25:
// 990 "parser.lemon"
{
	yygotominor.yy175 = NULL;
  yy_destructor(40,&yymsp[-1].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3043 "parser.c"
        break;
      case 21:
      case 26:
// 994 "parser.lemon"
{
	yygotominor.yy175 = yymsp[-1].minor.yy175;
  yy_destructor(40,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3053 "parser.c"
        break;
      case 22:
      case 72:
      case 138:
      case 163:
      case 184:
      case 245:
      case 248:
// 998 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_list(yymsp[-2].minor.yy175, yymsp[0].minor.yy175);
  yy_destructor(6,&yymsp[-1].minor);
}
// 3067 "parser.c"
        break;
      case 24:
      case 219:
      case 252:
// 1006 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state);
}
// 3076 "parser.c"
        break;
      case 27:
// 1018 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_definition(yymsp[0].minor.yy175, NULL, NULL, status->scanner_state);
}
// 3083 "parser.c"
        break;
      case 28:
// 1022 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_definition(NULL, NULL, yymsp[0].minor.yy175, status->scanner_state);
}
// 3090 "parser.c"
        break;
      case 29:
// 1026 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_definition(NULL, yymsp[0].minor.yy175, NULL, status->scanner_state);
}
// 3097 "parser.c"
        break;
      case 30:
// 1030 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_definition(yymsp[-1].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
}
// 3104 "parser.c"
        break;
      case 31:
// 1034 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_definition(yymsp[-1].minor.yy175, NULL, yymsp[0].minor.yy175, status->scanner_state);
}
// 3111 "parser.c"
        break;
      case 32:
// 1038 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_definition(yymsp[0].minor.yy175, NULL, yymsp[-1].minor.yy175, status->scanner_state);
}
// 3118 "parser.c"
        break;
      case 33:
// 1042 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_definition(NULL, yymsp[0].minor.yy175, yymsp[-1].minor.yy175, status->scanner_state);
}
// 3125 "parser.c"
        break;
      case 34:
// 1046 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_definition(yymsp[-2].minor.yy175, yymsp[0].minor.yy175, yymsp[-1].minor.yy175, status->scanner_state);
}
// 3132 "parser.c"
        break;
      case 35:
// 1050 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_definition(yymsp[-1].minor.yy175, yymsp[0].minor.yy175, yymsp[-2].minor.yy175, status->scanner_state);
}
// 3139 "parser.c"
        break;
      case 38:
// 1066 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_property(yymsp[-2].minor.yy175, yymsp[-1].minor.yy0, NULL, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(33,&yymsp[0].minor);
}
// 3147 "parser.c"
        break;
      case 39:
// 1070 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_property(yymsp[-2].minor.yy175, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(33,&yymsp[0].minor);
}
// 3155 "parser.c"
        break;
      case 40:
// 1074 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_property(yymsp[-4].minor.yy175, yymsp[-3].minor.yy0, yymsp[-1].minor.yy175, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(43,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 3164 "parser.c"
        break;
      case 41:
// 1078 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_property(yymsp[-4].minor.yy175, yymsp[-3].minor.yy0, yymsp[-1].minor.yy175, NULL, status->scanner_state);
  yy_destructor(43,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 3173 "parser.c"
        break;
      case 48:
// 1106 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy175, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(44,&yymsp[-4].minor);
  yy_destructor(43,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 3183 "parser.c"
        break;
      case 49:
// 1110 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy175, NULL, status->scanner_state);
  yy_destructor(44,&yymsp[-4].minor);
  yy_destructor(43,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 3193 "parser.c"
        break;
      case 50:
// 1114 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(yymsp[-6].minor.yy175, yymsp[-4].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(46,&yymsp[-5].minor);
  yy_destructor(47,&yymsp[-3].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(40,&yymsp[-1].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3205 "parser.c"
        break;
      case 51:
// 1118 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(yymsp[-7].minor.yy175, yymsp[-5].minor.yy0, yymsp[-3].minor.yy175, NULL, NULL, status->scanner_state);
  yy_destructor(46,&yymsp[-6].minor);
  yy_destructor(47,&yymsp[-4].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(40,&yymsp[-1].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3217 "parser.c"
        break;
      case 52:
// 1122 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(yymsp[-7].minor.yy175, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy175, NULL, status->scanner_state);
  yy_destructor(46,&yymsp[-6].minor);
  yy_destructor(47,&yymsp[-4].minor);
  yy_destructor(30,&yymsp[-3].minor);
  yy_destructor(40,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3229 "parser.c"
        break;
      case 53:
// 1126 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(yymsp[-8].minor.yy175, yymsp[-6].minor.yy0, yymsp[-4].minor.yy175, yymsp[-1].minor.yy175, NULL, status->scanner_state);
  yy_destructor(46,&yymsp[-7].minor);
  yy_destructor(47,&yymsp[-5].minor);
  yy_destructor(30,&yymsp[-3].minor);
  yy_destructor(40,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3241 "parser.c"
        break;
      case 54:
// 1130 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(yymsp[-6].minor.yy175, yymsp[-4].minor.yy0, NULL, NULL, yymsp[-7].minor.yy0, status->scanner_state);
  yy_destructor(46,&yymsp[-5].minor);
  yy_destructor(47,&yymsp[-3].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(40,&yymsp[-1].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3253 "parser.c"
        break;
      case 55:
// 1134 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(yymsp[-7].minor.yy175, yymsp[-5].minor.yy0, yymsp[-3].minor.yy175, NULL, yymsp[-8].minor.yy0, status->scanner_state);
  yy_destructor(46,&yymsp[-6].minor);
  yy_destructor(47,&yymsp[-4].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(40,&yymsp[-1].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3265 "parser.c"
        break;
      case 56:
// 1138 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(yymsp[-7].minor.yy175, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy175, yymsp[-8].minor.yy0, status->scanner_state);
  yy_destructor(46,&yymsp[-6].minor);
  yy_destructor(47,&yymsp[-4].minor);
  yy_destructor(30,&yymsp[-3].minor);
  yy_destructor(40,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3277 "parser.c"
        break;
      case 57:
// 1142 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(yymsp[-8].minor.yy175, yymsp[-6].minor.yy0, yymsp[-4].minor.yy175, yymsp[-1].minor.yy175, yymsp[-9].minor.yy0, status->scanner_state);
  yy_destructor(46,&yymsp[-7].minor);
  yy_destructor(47,&yymsp[-5].minor);
  yy_destructor(30,&yymsp[-3].minor);
  yy_destructor(40,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3289 "parser.c"
        break;
      case 58:
// 1146 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(yymsp[-5].minor.yy175, yymsp[-3].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(46,&yymsp[-4].minor);
  yy_destructor(47,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[-1].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 3300 "parser.c"
        break;
      case 59:
// 1150 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(yymsp[-6].minor.yy175, yymsp[-4].minor.yy0, yymsp[-2].minor.yy175, NULL, NULL, status->scanner_state);
  yy_destructor(46,&yymsp[-5].minor);
  yy_destructor(47,&yymsp[-3].minor);
  yy_destructor(30,&yymsp[-1].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 3311 "parser.c"
        break;
      case 60:
// 1154 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(yymsp[-5].minor.yy175, yymsp[-3].minor.yy0, NULL, NULL, yymsp[-6].minor.yy0, status->scanner_state);
  yy_destructor(46,&yymsp[-4].minor);
  yy_destructor(47,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[-1].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 3322 "parser.c"
        break;
      case 61:
// 1158 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(yymsp[-6].minor.yy175, yymsp[-4].minor.yy0, yymsp[-2].minor.yy175, NULL, yymsp[-7].minor.yy0, status->scanner_state);
  yy_destructor(46,&yymsp[-5].minor);
  yy_destructor(47,&yymsp[-3].minor);
  yy_destructor(30,&yymsp[-1].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 3333 "parser.c"
        break;
      case 64:
// 1170 "parser.lemon"
{
	yygotominor.yy175 = json_object_new_string("public");
  yy_destructor(1,&yymsp[0].minor);
}
// 3341 "parser.c"
        break;
      case 65:
// 1174 "parser.lemon"
{
	yygotominor.yy175 = json_object_new_string("protected");
  yy_destructor(2,&yymsp[0].minor);
}
// 3349 "parser.c"
        break;
      case 66:
// 1178 "parser.lemon"
{
	yygotominor.yy175 = json_object_new_string("private");
  yy_destructor(4,&yymsp[0].minor);
}
// 3357 "parser.c"
        break;
      case 67:
// 1182 "parser.lemon"
{
	yygotominor.yy175 = json_object_new_string("static");
  yy_destructor(3,&yymsp[0].minor);
}
// 3365 "parser.c"
        break;
      case 68:
// 1186 "parser.lemon"
{
	yygotominor.yy175 = json_object_new_string("scoped");
  yy_destructor(5,&yymsp[0].minor);
}
// 3373 "parser.c"
        break;
      case 69:
// 1190 "parser.lemon"
{
	yygotominor.yy175 = json_object_new_string("inline");
  yy_destructor(48,&yymsp[0].minor);
}
// 3381 "parser.c"
        break;
      case 70:
// 1194 "parser.lemon"
{
	yygotominor.yy175 = json_object_new_string("abstract");
  yy_destructor(38,&yymsp[0].minor);
}
// 3389 "parser.c"
        break;
      case 71:
// 1198 "parser.lemon"
{
	yygotominor.yy175 = json_object_new_string("final");
  yy_destructor(39,&yymsp[0].minor);
}
// 3397 "parser.c"
        break;
      case 74:
// 1210 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_parameter(NULL, NULL, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 3404 "parser.c"
        break;
      case 75:
// 1214 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_parameter(yymsp[-1].minor.yy175, NULL, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 3411 "parser.c"
        break;
      case 76:
// 1218 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_parameter(NULL, yymsp[-1].minor.yy175, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 3418 "parser.c"
        break;
      case 77:
// 1222 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_parameter(NULL, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(43,&yymsp[-1].minor);
}
// 3426 "parser.c"
        break;
      case 78:
// 1226 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_parameter(yymsp[-3].minor.yy175, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(43,&yymsp[-1].minor);
}
// 3434 "parser.c"
        break;
      case 79:
// 1230 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_parameter(NULL, yymsp[-3].minor.yy175, yymsp[-2].minor.yy0, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(43,&yymsp[-1].minor);
}
// 3442 "parser.c"
        break;
      case 80:
// 1234 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(13,&yymsp[-2].minor);
  yy_destructor(14,&yymsp[0].minor);
}
// 3451 "parser.c"
        break;
      case 81:
// 1238 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_type(XX_TYPE_INTEGER);
  yy_destructor(49,&yymsp[0].minor);
}
// 3459 "parser.c"
        break;
      case 82:
// 1242 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_type(XX_TYPE_UINTEGER);
  yy_destructor(50,&yymsp[0].minor);
}
// 3467 "parser.c"
        break;
      case 83:
// 1246 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_type(XX_TYPE_LONG);
  yy_destructor(51,&yymsp[0].minor);
}
// 3475 "parser.c"
        break;
      case 84:
// 1250 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_type(XX_TYPE_ULONG);
  yy_destructor(52,&yymsp[0].minor);
}
// 3483 "parser.c"
        break;
      case 85:
// 1254 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_type(XX_TYPE_CHAR);
  yy_destructor(53,&yymsp[0].minor);
}
// 3491 "parser.c"
        break;
      case 86:
// 1258 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_type(XX_TYPE_UCHAR);
  yy_destructor(54,&yymsp[0].minor);
}
// 3499 "parser.c"
        break;
      case 87:
// 1262 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_type(XX_TYPE_DOUBLE);
  yy_destructor(55,&yymsp[0].minor);
}
// 3507 "parser.c"
        break;
      case 88:
// 1266 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_type(XX_TYPE_BOOL);
  yy_destructor(56,&yymsp[0].minor);
}
// 3515 "parser.c"
        break;
      case 89:
// 1270 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_type(XX_TYPE_STRING);
  yy_destructor(57,&yymsp[0].minor);
}
// 3523 "parser.c"
        break;
      case 90:
// 1274 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_type(XX_TYPE_ARRAY);
  yy_destructor(58,&yymsp[0].minor);
}
// 3531 "parser.c"
        break;
      case 91:
// 1278 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_type(XX_TYPE_VAR);
  yy_destructor(59,&yymsp[0].minor);
}
// 3539 "parser.c"
        break;
      case 112:
// 1362 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_break_statement(status->scanner_state);
  yy_destructor(60,&yymsp[-1].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 3548 "parser.c"
        break;
      case 113:
// 1366 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_continue_statement(status->scanner_state);
  yy_destructor(61,&yymsp[-1].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 3557 "parser.c"
        break;
      case 114:
// 1370 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_if_statement(yymsp[-2].minor.yy175, NULL, NULL, status->scanner_state);
  yy_destructor(62,&yymsp[-3].minor);
  yy_destructor(40,&yymsp[-1].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3567 "parser.c"
        break;
      case 115:
// 1374 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_if_statement(yymsp[-5].minor.yy175, NULL, NULL, status->scanner_state);
  yy_destructor(62,&yymsp[-6].minor);
  yy_destructor(40,&yymsp[-4].minor);
  yy_destructor(41,&yymsp[-3].minor);
  yy_destructor(63,&yymsp[-2].minor);
  yy_destructor(40,&yymsp[-1].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3580 "parser.c"
        break;
      case 116:
// 1378 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_if_statement(yymsp[-3].minor.yy175, yymsp[-1].minor.yy175, NULL, status->scanner_state);
  yy_destructor(62,&yymsp[-4].minor);
  yy_destructor(40,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3590 "parser.c"
        break;
      case 117:
// 1382 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_if_statement(yymsp[-7].minor.yy175, yymsp[-5].minor.yy175, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(62,&yymsp[-8].minor);
  yy_destructor(40,&yymsp[-6].minor);
  yy_destructor(41,&yymsp[-4].minor);
  yy_destructor(63,&yymsp[-3].minor);
  yy_destructor(40,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3603 "parser.c"
        break;
      case 118:
// 1386 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_if_statement(yymsp[-6].minor.yy175, yymsp[-4].minor.yy175, NULL, status->scanner_state);
  yy_destructor(62,&yymsp[-7].minor);
  yy_destructor(40,&yymsp[-5].minor);
  yy_destructor(41,&yymsp[-3].minor);
  yy_destructor(63,&yymsp[-2].minor);
  yy_destructor(40,&yymsp[-1].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3616 "parser.c"
        break;
      case 119:
// 1390 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_if_statement(yymsp[-6].minor.yy175, NULL, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(62,&yymsp[-7].minor);
  yy_destructor(40,&yymsp[-5].minor);
  yy_destructor(41,&yymsp[-4].minor);
  yy_destructor(63,&yymsp[-3].minor);
  yy_destructor(40,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3629 "parser.c"
        break;
      case 120:
// 1394 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_switch_statement(yymsp[-2].minor.yy175, NULL, status->scanner_state);
  yy_destructor(64,&yymsp[-3].minor);
  yy_destructor(40,&yymsp[-1].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3639 "parser.c"
        break;
      case 121:
// 1398 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_switch_statement(yymsp[-3].minor.yy175, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(64,&yymsp[-4].minor);
  yy_destructor(40,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3649 "parser.c"
        break;
      case 124:
// 1410 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_case_clause(yymsp[-1].minor.yy175, NULL, status->scanner_state);
  yy_destructor(65,&yymsp[-2].minor);
  yy_destructor(66,&yymsp[0].minor);
}
// 3658 "parser.c"
        break;
      case 125:
// 1414 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_case_clause(yymsp[-2].minor.yy175, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(65,&yymsp[-3].minor);
  yy_destructor(66,&yymsp[-1].minor);
}
// 3667 "parser.c"
        break;
      case 126:
// 1418 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_case_clause(NULL, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(67,&yymsp[-2].minor);
  yy_destructor(66,&yymsp[-1].minor);
}
// 3676 "parser.c"
        break;
      case 127:
// 1422 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_loop_statement(NULL, status->scanner_state);
  yy_destructor(68,&yymsp[-2].minor);
  yy_destructor(40,&yymsp[-1].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3686 "parser.c"
        break;
      case 128:
// 1426 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_loop_statement(yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(68,&yymsp[-3].minor);
  yy_destructor(40,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3696 "parser.c"
        break;
      case 129:
// 1430 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_while_statement(yymsp[-2].minor.yy175, NULL, status->scanner_state);
  yy_destructor(69,&yymsp[-3].minor);
  yy_destructor(40,&yymsp[-1].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3706 "parser.c"
        break;
      case 130:
// 1434 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_while_statement(yymsp[-3].minor.yy175, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(69,&yymsp[-4].minor);
  yy_destructor(40,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3716 "parser.c"
        break;
      case 131:
// 1438 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_do_while_statement(yymsp[-1].minor.yy175, NULL, status->scanner_state);
  yy_destructor(70,&yymsp[-5].minor);
  yy_destructor(40,&yymsp[-4].minor);
  yy_destructor(41,&yymsp[-3].minor);
  yy_destructor(69,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 3728 "parser.c"
        break;
      case 132:
// 1442 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_do_while_statement(yymsp[-1].minor.yy175, yymsp[-4].minor.yy175, status->scanner_state);
  yy_destructor(70,&yymsp[-6].minor);
  yy_destructor(40,&yymsp[-5].minor);
  yy_destructor(41,&yymsp[-3].minor);
  yy_destructor(69,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 3740 "parser.c"
        break;
      case 133:
// 1446 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_for_statement(yymsp[-3].minor.yy175, NULL, yymsp[-5].minor.yy0, 0, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(71,&yymsp[-6].minor);
  yy_destructor(72,&yymsp[-4].minor);
  yy_destructor(40,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3751 "parser.c"
        break;
      case 134:
// 1450 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_for_statement(yymsp[-3].minor.yy175, NULL, yymsp[-6].minor.yy0, 1, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(71,&yymsp[-7].minor);
  yy_destructor(72,&yymsp[-5].minor);
  yy_destructor(73,&yymsp[-4].minor);
  yy_destructor(40,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3763 "parser.c"
        break;
      case 135:
// 1454 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_for_statement(yymsp[-3].minor.yy175, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, 0, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(71,&yymsp[-8].minor);
  yy_destructor(6,&yymsp[-6].minor);
  yy_destructor(72,&yymsp[-4].minor);
  yy_destructor(40,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3775 "parser.c"
        break;
      case 136:
// 1458 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_for_statement(yymsp[-3].minor.yy175, yymsp[-8].minor.yy0, yymsp[-6].minor.yy0, 1, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(71,&yymsp[-9].minor);
  yy_destructor(6,&yymsp[-7].minor);
  yy_destructor(72,&yymsp[-5].minor);
  yy_destructor(73,&yymsp[-4].minor);
  yy_destructor(40,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3788 "parser.c"
        break;
      case 137:
// 1462 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_statement(yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(74,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 3797 "parser.c"
        break;
      case 140:
// 1475 "parser.lemon"
{
	yygotominor.yy175 = json_object_new_string("assign");
  yy_destructor(43,&yymsp[0].minor);
}
// 3805 "parser.c"
        break;
      case 141:
// 1480 "parser.lemon"
{
	yygotominor.yy175 = json_object_new_string("add-assign");
  yy_destructor(75,&yymsp[0].minor);
}
// 3813 "parser.c"
        break;
      case 142:
// 1485 "parser.lemon"
{
	yygotominor.yy175 = json_object_new_string("sub-assign");
  yy_destructor(76,&yymsp[0].minor);
}
// 3821 "parser.c"
        break;
      case 143:
// 1489 "parser.lemon"
{
	yygotominor.yy175 = json_object_new_string("mult-assign");
  yy_destructor(77,&yymsp[0].minor);
}
// 3829 "parser.c"
        break;
      case 144:
// 1493 "parser.lemon"
{
	yygotominor.yy175 = json_object_new_string("div-assign");
  yy_destructor(78,&yymsp[0].minor);
}
// 3837 "parser.c"
        break;
      case 145:
// 1497 "parser.lemon"
{
	yygotominor.yy175 = json_object_new_string("concat-assign");
  yy_destructor(79,&yymsp[0].minor);
}
// 3845 "parser.c"
        break;
      case 146:
// 1502 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("variable", yymsp[-1].minor.yy175, yymsp[-2].minor.yy0, NULL, NULL, yymsp[0].minor.yy175, status->scanner_state);
}
// 3852 "parser.c"
        break;
      case 147:
// 1507 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("object-property", yymsp[-1].minor.yy175, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(80,&yymsp[-3].minor);
}
// 3860 "parser.c"
        break;
      case 148:
// 1512 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("variable-dynamic-object-property", yymsp[-1].minor.yy175, yymsp[-6].minor.yy0, yymsp[-3].minor.yy0, NULL, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(80,&yymsp[-5].minor);
  yy_destructor(40,&yymsp[-4].minor);
  yy_destructor(41,&yymsp[-2].minor);
}
// 3870 "parser.c"
        break;
      case 149:
// 1517 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("string-dynamic-object-property", yymsp[-1].minor.yy175, yymsp[-6].minor.yy0, yymsp[-3].minor.yy0, NULL, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(80,&yymsp[-5].minor);
  yy_destructor(40,&yymsp[-4].minor);
  yy_destructor(41,&yymsp[-2].minor);
}
// 3880 "parser.c"
        break;
      case 150:
// 1522 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("object-property-append", yymsp[-1].minor.yy175, yymsp[-6].minor.yy0, yymsp[-4].minor.yy0, NULL, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(80,&yymsp[-5].minor);
  yy_destructor(82,&yymsp[-3].minor);
  yy_destructor(83,&yymsp[-2].minor);
}
// 3890 "parser.c"
        break;
      case 151:
// 1527 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("object-property-array-index", yymsp[-1].minor.yy175, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, yymsp[-3].minor.yy175, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(80,&yymsp[-6].minor);
  yy_destructor(82,&yymsp[-4].minor);
  yy_destructor(83,&yymsp[-2].minor);
}
// 3900 "parser.c"
        break;
      case 152:
// 1532 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("static-property", yymsp[-1].minor.yy175, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(84,&yymsp[-3].minor);
}
// 3908 "parser.c"
        break;
      case 153:
// 1537 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("static-property-append", yymsp[-1].minor.yy175, yymsp[-6].minor.yy0, yymsp[-4].minor.yy0, NULL, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(84,&yymsp[-5].minor);
  yy_destructor(82,&yymsp[-3].minor);
  yy_destructor(83,&yymsp[-2].minor);
}
// 3918 "parser.c"
        break;
      case 154:
// 1542 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("static-property-array-index", yymsp[-1].minor.yy175, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, yymsp[-3].minor.yy175, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(84,&yymsp[-6].minor);
  yy_destructor(82,&yymsp[-4].minor);
  yy_destructor(83,&yymsp[-2].minor);
}
// 3928 "parser.c"
        break;
      case 155:
// 1547 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("variable-append", yymsp[-1].minor.yy175, yymsp[-4].minor.yy0, NULL, NULL, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(82,&yymsp[-3].minor);
  yy_destructor(83,&yymsp[-2].minor);
}
// 3937 "parser.c"
        break;
      case 156:
// 1552 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("array-index", yymsp[-1].minor.yy175, yymsp[-5].minor.yy0, NULL, yymsp[-3].minor.yy175, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(82,&yymsp[-4].minor);
  yy_destructor(83,&yymsp[-2].minor);
}
// 3946 "parser.c"
        break;
      case 157:
// 1557 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("incr", NULL, yymsp[-3].minor.yy0, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(80,&yymsp[-2].minor);
  yy_destructor(85,&yymsp[0].minor);
}
// 3955 "parser.c"
        break;
      case 158:
// 1562 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("decr", NULL, yymsp[-3].minor.yy0, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(80,&yymsp[-2].minor);
  yy_destructor(86,&yymsp[0].minor);
}
// 3964 "parser.c"
        break;
      case 159:
// 1567 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("incr", NULL, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(85,&yymsp[0].minor);
}
// 3972 "parser.c"
        break;
      case 160:
// 1572 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("decr", NULL, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(86,&yymsp[0].minor);
}
// 3980 "parser.c"
        break;
      case 162:
// 1580 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_echo_statement(yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(87,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 3989 "parser.c"
        break;
      case 165:
// 1592 "parser.lemon"
{
	yygotominor.yy175 = yymsp[0].minor.yy175;;
}
// 3996 "parser.c"
        break;
      case 166:
// 1597 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_mcall_statement(yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(33,&yymsp[0].minor);
}
// 4004 "parser.c"
        break;
      case 167:
// 1602 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_fcall_statement(yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(33,&yymsp[0].minor);
}
// 4012 "parser.c"
        break;
      case 168:
// 1607 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_scall_statement(yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(33,&yymsp[0].minor);
}
// 4020 "parser.c"
        break;
      case 169:
// 1612 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_return_statement(yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(88,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 4029 "parser.c"
        break;
      case 170:
// 1617 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_return_statement(NULL, status->scanner_state);
  yy_destructor(88,&yymsp[-1].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 4038 "parser.c"
        break;
      case 171:
// 1622 "parser.lemon"
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
// 4052 "parser.c"
        break;
      case 172:
// 1631 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state),
		yymsp[-2].minor.yy175,
		status->scanner_state
	);
  yy_destructor(89,&yymsp[-5].minor);
  yy_destructor(82,&yymsp[-3].minor);
  yy_destructor(83,&yymsp[-1].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 4067 "parser.c"
        break;
      case 173:
// 1640 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_throw_exception(yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(90,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 4076 "parser.c"
        break;
      case 174:
// 1644 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_statement(XX_T_TYPE_INTEGER, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 4085 "parser.c"
        break;
      case 175:
// 1648 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_statement(XX_T_TYPE_UINTEGER, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(50,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 4094 "parser.c"
        break;
      case 176:
// 1652 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_statement(XX_T_TYPE_CHAR, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(53,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 4103 "parser.c"
        break;
      case 177:
// 1656 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_statement(XX_T_TYPE_UCHAR, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 4112 "parser.c"
        break;
      case 178:
// 1660 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_statement(XX_T_TYPE_LONG, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(51,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 4121 "parser.c"
        break;
      case 179:
// 1664 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_statement(XX_T_TYPE_ULONG, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(52,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 4130 "parser.c"
        break;
      case 180:
// 1668 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_statement(XX_T_TYPE_DOUBLE, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(55,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 4139 "parser.c"
        break;
      case 181:
// 1672 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_statement(XX_T_TYPE_STRING, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(57,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 4148 "parser.c"
        break;
      case 182:
// 1676 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_statement(XX_T_TYPE_BOOL, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(56,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 4157 "parser.c"
        break;
      case 183:
// 1680 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_statement(XX_T_TYPE_VAR, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(59,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 4166 "parser.c"
        break;
      case 186:
// 1692 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_variable(yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 4173 "parser.c"
        break;
      case 187:
// 1696 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_variable(yymsp[-2].minor.yy0, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(43,&yymsp[-1].minor);
}
// 4181 "parser.c"
        break;
      case 189:
// 1704 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("not", yymsp[0].minor.yy175, NULL, NULL, status->scanner_state);
  yy_destructor(29,&yymsp[-1].minor);
}
// 4189 "parser.c"
        break;
      case 190:
// 1708 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("isset", yymsp[0].minor.yy175, NULL, NULL, status->scanner_state);
  yy_destructor(25,&yymsp[-1].minor);
}
// 4197 "parser.c"
        break;
      case 191:
// 1712 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("require", yymsp[0].minor.yy175, NULL, NULL, status->scanner_state);
  yy_destructor(7,&yymsp[-1].minor);
}
// 4205 "parser.c"
        break;
      case 192:
// 1716 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("equals", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(11,&yymsp[-1].minor);
}
// 4213 "parser.c"
        break;
      case 193:
// 1720 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("not-equals", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(18,&yymsp[-1].minor);
}
// 4221 "parser.c"
        break;
      case 194:
// 1724 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("identical", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(12,&yymsp[-1].minor);
}
// 4229 "parser.c"
        break;
      case 195:
// 1728 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("not-identical", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(17,&yymsp[-1].minor);
}
// 4237 "parser.c"
        break;
      case 196:
// 1732 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("less", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(13,&yymsp[-1].minor);
}
// 4245 "parser.c"
        break;
      case 197:
// 1736 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("greater", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(14,&yymsp[-1].minor);
}
// 4253 "parser.c"
        break;
      case 198:
// 1740 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("less-equal", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(15,&yymsp[-1].minor);
}
// 4261 "parser.c"
        break;
      case 199:
// 1744 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("greater-equal", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(16,&yymsp[-1].minor);
}
// 4269 "parser.c"
        break;
      case 200:
// 1748 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("list", yymsp[-1].minor.yy175, NULL, NULL, status->scanner_state);
  yy_destructor(47,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 4278 "parser.c"
        break;
      case 201:
// 1752 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("cast", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(47,&yymsp[-3].minor);
  yy_destructor(30,&yymsp[-1].minor);
}
// 4287 "parser.c"
        break;
      case 202:
// 1756 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("type-hint", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(13,&yymsp[-3].minor);
  yy_destructor(14,&yymsp[-1].minor);
}
// 4296 "parser.c"
        break;
      case 203:
      case 264:
// 1760 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("property-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(80,&yymsp[-1].minor);
}
// 4305 "parser.c"
        break;
      case 204:
// 1764 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("static-property-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(84,&yymsp[-1].minor);
}
// 4313 "parser.c"
        break;
      case 205:
// 1768 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("static-constant-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(84,&yymsp[-1].minor);
}
// 4321 "parser.c"
        break;
      case 206:
      case 263:
// 1773 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), yymsp[-1].minor.yy175, NULL, status->scanner_state);
  yy_destructor(82,&yymsp[-2].minor);
  yy_destructor(83,&yymsp[0].minor);
}
// 4331 "parser.c"
        break;
      case 207:
// 1778 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("add", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(19,&yymsp[-1].minor);
}
// 4339 "parser.c"
        break;
      case 208:
// 1783 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("sub", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(20,&yymsp[-1].minor);
}
// 4347 "parser.c"
        break;
      case 209:
// 1788 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("mul", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(22,&yymsp[-1].minor);
}
// 4355 "parser.c"
        break;
      case 210:
// 1793 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("div", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(23,&yymsp[-1].minor);
}
// 4363 "parser.c"
        break;
      case 211:
// 1798 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("mod", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(24,&yymsp[-1].minor);
}
// 4371 "parser.c"
        break;
      case 212:
// 1803 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("concat", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(21,&yymsp[-1].minor);
}
// 4379 "parser.c"
        break;
      case 213:
// 1808 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("and", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(9,&yymsp[-1].minor);
}
// 4387 "parser.c"
        break;
      case 214:
// 1813 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("or", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(10,&yymsp[-1].minor);
}
// 4395 "parser.c"
        break;
      case 215:
// 1818 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("instanceof", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(8,&yymsp[-1].minor);
}
// 4403 "parser.c"
        break;
      case 216:
// 1823 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("fetch", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(26,&yymsp[-3].minor);
  yy_destructor(6,&yymsp[-1].minor);
}
// 4412 "parser.c"
        break;
      case 217:
// 1828 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("typeof", yymsp[0].minor.yy175, NULL, NULL, status->scanner_state);
  yy_destructor(27,&yymsp[-1].minor);
}
// 4420 "parser.c"
        break;
      case 218:
// 1833 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_literal(XX_T_CONSTANT, yymsp[0].minor.yy0, status->scanner_state);
}
// 4427 "parser.c"
        break;
      case 220:
      case 254:
      case 256:
// 1843 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_literal(XX_T_INTEGER, yymsp[0].minor.yy0, status->scanner_state);
}
// 4436 "parser.c"
        break;
      case 221:
      case 253:
      case 258:
// 1848 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_literal(XX_T_STRING, yymsp[0].minor.yy0, status->scanner_state);
}
// 4445 "parser.c"
        break;
      case 222:
      case 257:
// 1853 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_literal(XX_T_CHAR, yymsp[0].minor.yy0, status->scanner_state);
}
// 4453 "parser.c"
        break;
      case 223:
      case 259:
// 1858 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_literal(XX_T_DOUBLE, yymsp[0].minor.yy0, status->scanner_state);
}
// 4461 "parser.c"
        break;
      case 224:
      case 260:
// 1863 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_literal(XX_T_NULL, NULL, status->scanner_state);
  yy_destructor(94,&yymsp[0].minor);
}
// 4470 "parser.c"
        break;
      case 225:
      case 262:
// 1868 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_literal(XX_T_TRUE, NULL, status->scanner_state);
  yy_destructor(95,&yymsp[0].minor);
}
// 4479 "parser.c"
        break;
      case 226:
      case 261:
// 1873 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_literal(XX_T_FALSE, NULL, status->scanner_state);
  yy_destructor(96,&yymsp[0].minor);
}
// 4488 "parser.c"
        break;
      case 227:
// 1878 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("empty-array", NULL, NULL, NULL, status->scanner_state);
  yy_destructor(82,&yymsp[-1].minor);
  yy_destructor(83,&yymsp[0].minor);
}
// 4497 "parser.c"
        break;
      case 228:
// 1883 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("array", yymsp[-1].minor.yy175, NULL, NULL, status->scanner_state);
  yy_destructor(82,&yymsp[-2].minor);
  yy_destructor(83,&yymsp[0].minor);
}
// 4506 "parser.c"
        break;
      case 229:
// 1888 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_new_instance(yymsp[0].minor.yy0, NULL, status->scanner_state);
  yy_destructor(28,&yymsp[-1].minor);
}
// 4514 "parser.c"
        break;
      case 230:
// 1893 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_new_instance(yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(28,&yymsp[-3].minor);
  yy_destructor(47,&yymsp[-1].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 4524 "parser.c"
        break;
      case 231:
// 1898 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_new_instance(yymsp[-3].minor.yy0, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(28,&yymsp[-4].minor);
  yy_destructor(47,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 4534 "parser.c"
        break;
      case 232:
// 1903 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_fcall(yymsp[-3].minor.yy0, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(47,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 4543 "parser.c"
        break;
      case 233:
// 1908 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_fcall(yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(47,&yymsp[-1].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 4552 "parser.c"
        break;
      case 234:
// 1913 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_scall(yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(84,&yymsp[-4].minor);
  yy_destructor(47,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 4562 "parser.c"
        break;
      case 235:
// 1918 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_scall(yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(84,&yymsp[-3].minor);
  yy_destructor(47,&yymsp[-1].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 4572 "parser.c"
        break;
      case 236:
// 1923 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_mcall(1, yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(80,&yymsp[-4].minor);
  yy_destructor(47,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 4582 "parser.c"
        break;
      case 237:
// 1928 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_mcall(1, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(80,&yymsp[-3].minor);
  yy_destructor(47,&yymsp[-1].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 4592 "parser.c"
        break;
      case 238:
// 1933 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_mcall(2, yymsp[-7].minor.yy0, yymsp[-4].minor.yy0, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(80,&yymsp[-6].minor);
  yy_destructor(40,&yymsp[-5].minor);
  yy_destructor(41,&yymsp[-3].minor);
  yy_destructor(47,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 4604 "parser.c"
        break;
      case 239:
// 1938 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_mcall(2, yymsp[-6].minor.yy0, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(80,&yymsp[-5].minor);
  yy_destructor(40,&yymsp[-4].minor);
  yy_destructor(41,&yymsp[-2].minor);
  yy_destructor(47,&yymsp[-1].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 4616 "parser.c"
        break;
      case 240:
// 1943 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_mcall(3, yymsp[-7].minor.yy0, yymsp[-4].minor.yy0, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(80,&yymsp[-6].minor);
  yy_destructor(40,&yymsp[-5].minor);
  yy_destructor(41,&yymsp[-3].minor);
  yy_destructor(47,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 4628 "parser.c"
        break;
      case 241:
// 1948 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_mcall(3, yymsp[-6].minor.yy0, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(80,&yymsp[-5].minor);
  yy_destructor(40,&yymsp[-4].minor);
  yy_destructor(41,&yymsp[-2].minor);
  yy_destructor(47,&yymsp[-1].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 4640 "parser.c"
        break;
      case 250:
// 1987 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_array_item(yymsp[-2].minor.yy175, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(66,&yymsp[-1].minor);
}
// 4648 "parser.c"
        break;
      case 251:
// 1991 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_array_item(NULL, yymsp[0].minor.yy175, status->scanner_state);
}
// 4655 "parser.c"
        break;
      case 266:
// 2052 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_comment(yymsp[0].minor.yy0, status->scanner_state);
}
// 4662 "parser.c"
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
// 865 "parser.lemon"


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

// 4738 "parser.c"
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
