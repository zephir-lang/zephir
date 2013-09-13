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
#define YYNSTATE 567
#define YYNRULE 269
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
 /*     0 */   178,  181,  184,    3,    4,    5,    6,    7,  220,  837,
 /*    10 */     1,    2,  566,    4,    5,    6,    7,  147,  416,  414,
 /*    20 */   174,  229,  233,  249,  171,   98,  355,  224,  100,  535,
 /*    30 */   541,  540,  528,  483,  534,  508,   65,   43,   47,   85,
 /*    40 */   235,  151,  214,  472,   54,   55,   56,   57,   58,   59,
 /*    50 */    60,   61,   62,   63,   64,  212,  208,  210,  173,  183,
 /*    60 */   188,  190,  192,  194,  186,  180,  196,  198,  206,  200,
 /*    70 */   202,  204,   94,  342,  545,  442,  237,  243,  362,  542,
 /*    80 */   405,  256,  258,  257,  496,  447,  236,  238,  239,  240,
 /*    90 */   241,  242,  406,  415,  424,  427,  418,  421,  430,  436,
 /*   100 */   433,   31,  439,  131,  133,  135,  409,  144,  200,  202,
 /*   110 */   204,  155,  159,  164,  308,   96,  557,  315,  545,  442,
 /*   120 */   196,  198,  206,  200,  202,  204,  149,  500,  465,  447,
 /*   130 */   377,  390,  394,  403,  255,  408,  406,  415,  424,  427,
 /*   140 */   418,  421,  430,  436,  433,  331,  439,  131,  133,  135,
 /*   150 */   151,  144,  472,  345,   38,  155,  159,  164,  308,   93,
 /*   160 */   262,  315,   96,  442,   36,   37,   39,   40,   42,   41,
 /*   170 */   568,   76,  143,  447,  377,  390,  394,  403,   85,  524,
 /*   180 */   406,  415,  424,  427,  418,  421,  430,  436,  433,  525,
 /*   190 */   439,  131,  133,  135,  530,  144,   32,   43,   47,  155,
 /*   200 */   159,  164,  308,    8,  531,  315,   11,  442,   87,  409,
 /*   210 */   549,  559,  407,  414,  447,  268,  486,  447,  377,  390,
 /*   220 */   394,  403,  321,  567,  406,  415,  424,  427,  418,  421,
 /*   230 */   430,  436,  433,   31,  439,  131,  133,  135,  429,  144,
 /*   240 */   255,  431,  414,  155,  159,  164,  308,  346,  225,  315,
 /*   250 */   265,  442,  246,  454,  322,  323,  324,  325,  326,   29,
 /*   260 */   498,  447,  377,  390,  394,  403,  254,    9,  406,  415,
 /*   270 */   424,  427,  418,  421,  430,  436,  433,  255,  439,  131,
 /*   280 */   133,  135,  304,  144,  177,  522,  446,  155,  159,  164,
 /*   290 */   308,  382,   85,  315,  287,  442,  350,  378,  383,  256,
 /*   300 */   258,  257,  255,  298,  111,  447,  377,  390,  394,  403,
 /*   310 */   428,  414,  406,  415,  424,  427,  418,  421,  430,  436,
 /*   320 */   433,  310,  439,  131,  133,  135,  409,  144,  292,  434,
 /*   330 */   414,  155,  159,  164,  308,    8,  409,  315,   11,  442,
 /*   340 */    87,  409,  549,  559,  425,  414,  447,  268,  481,  447,
 /*   350 */   377,  390,  394,  403,   10,  426,  406,  415,  424,  427,
 /*   360 */   418,  421,  430,  436,  433,  435,  439,  131,  133,  135,
 /*   370 */   432,  144,  437,  414,  288,  155,  159,  164,  308,  244,
 /*   380 */   445,  315,  226,  442,  443,  409,  247,  256,  258,  257,
 /*   390 */   287,   12,  453,  447,  377,  390,  394,  403,  281,  264,
 /*   400 */   406,  415,  424,  427,  418,  421,  430,  436,  433,  255,
 /*   410 */   439,  131,  133,  135,  423,  144,  440,  414,   15,  155,
 /*   420 */   159,  164,  308,  294,  329,  315,  277,  442,  373,  409,
 /*   430 */    13,  409,  256,  258,  257,  285,  139,  447,  377,  390,
 /*   440 */   394,  403,  422,  414,  406,  415,  424,  427,  418,  421,
 /*   450 */   430,  436,  433,   31,  439,  131,  133,  135,  438,  144,
 /*   460 */   441,  316,  376,  155,  159,  164,  308,   16,  329,  315,
 /*   470 */    82,  442,  369,   65,   43,   47,  256,  258,  257,   83,
 /*   480 */   502,  447,  377,  390,  394,  403,  148,  475,  406,  415,
 /*   490 */   424,  427,  418,  421,  430,  436,  433,   14,  439,  131,
 /*   500 */   133,  135,   17,  144,  547,  276,  545,  155,  159,  164,
 /*   510 */   308,  382,  553,  315,  556,  442,  490,   96,  381,  256,
 /*   520 */   258,  257,  255,   85,  482,  447,  377,  390,  394,  403,
 /*   530 */   419,  414,  406,  415,  424,  427,  418,  421,  430,  436,
 /*   540 */   433,   31,  439,  131,  133,  135,  409,  144,  270,   17,
 /*   550 */   563,  155,  159,  164,  308,   96,  329,  315,  499,  442,
 /*   560 */   357,   65,   43,   47,  256,  258,  257,  484,  477,  447,
 /*   570 */   377,  390,  394,  403,   66,  420,  406,  415,  424,  427,
 /*   580 */   418,  421,  430,  436,  433,   24,  439,  131,  133,  135,
 /*   590 */   318,  144,  396,   25,  399,  155,  159,  164,  308,   27,
 /*   600 */   329,  315,   28,  442,  365,   65,   43,   47,  256,  258,
 /*   610 */   257,   30,  504,  447,  377,  390,  394,  403,   34,  317,
 /*   620 */   406,  415,  424,  427,  418,  421,  430,  436,  433,   44,
 /*   630 */   439,  131,  133,  135,  409,  144,   35,   45,   46,  155,
 /*   640 */   159,  164,  308,   48,  329,  315,   49,  442,  353,  380,
 /*   650 */    50,   52,  256,  258,  257,   53,  471,  447,  377,  390,
 /*   660 */   394,  403,   81,  417,  406,  415,  424,  427,  418,  421,
 /*   670 */   430,  436,  433,   79,  439,  131,  133,  135,  379,  144,
 /*   680 */   647,   78,   88,  155,  159,  164,  308,  342,   84,  315,
 /*   690 */    89,  442,  370,   91,   92,  256,  258,  257,  544,   95,
 /*   700 */   476,  447,  377,  390,  394,  403,   99,  105,  406,  415,
 /*   710 */   424,  427,  418,  421,  430,  436,  433,  106,  439,  131,
 /*   720 */   133,  135,  108,  144,  132,  134,  137,  155,  159,  164,
 /*   730 */   308,  140,  329,  315,  141,  442,  349,  146,  150,  152,
 /*   740 */   256,  258,  257,  153,  167,  447,  377,  390,  394,  403,
 /*   750 */   156,  161,  406,  415,  424,  427,  418,  421,  430,  436,
 /*   760 */   433,  165,  439,  131,  133,  135,  170,  144,  168,  176,
 /*   770 */   175,  155,  159,  164,  308,  218,  329,  315,  221,  442,
 /*   780 */   361,  222,  227,  230,  256,  258,  257,  231,  158,  447,
 /*   790 */   377,  390,  394,  403,  232,  248,  406,  415,  424,  427,
 /*   800 */   418,  421,  430,  436,  433,  250,  439,  131,  133,  135,
 /*   810 */   251,  144,  289,  290,  305,  155,  159,  164,  308,  823,
 /*   820 */   329,  315,  822,  442,  341,  295,  280,  296,  256,  258,
 /*   830 */   257,  309,  470,  447,  377,  390,  394,  403,  320,  319,
 /*   840 */   406,  415,  424,  427,  418,  421,  430,  436,  433,  327,
 /*   850 */   439,  131,  133,  135,  347,  144,  332,  336,  339,  155,
 /*   860 */   159,  164,  308,  340,  329,  315,  348,  442,  337,  351,
 /*   870 */   352,  356,  256,  258,  257,  360,  488,  447,  377,  390,
 /*   880 */   394,  403,  364,  363,  406,  415,  424,  427,  418,  421,
 /*   890 */   430,  436,  433,  368,  439,  131,  133,  135,  371,  144,
 /*   900 */   372,  385,  387,  155,  159,  164,  308,  342,  389,  315,
 /*   910 */   395,  442,  338,  397,  410,  256,  258,  257,  398,  402,
 /*   920 */   109,  447,  377,  390,  394,  403,  411,  412,  406,  415,
 /*   930 */   424,  427,  418,  421,  430,  436,  433,  413,  439,  131,
 /*   940 */   133,  135,  444,  144,  455,  456,  467,  155,  159,  164,
 /*   950 */   308,  469,  329,  315,  473,  442,  333,  479,  478,  485,
 /*   960 */   256,  258,  257,  492,  466,  447,  377,  390,  394,  403,
 /*   970 */   495,  493,  406,  415,  424,  427,  418,  421,  430,  436,
 /*   980 */   433,  501,  439,  131,  133,  135,  514,  144,  511,  512,
 /*   990 */   516,  155,  159,  164,  308,  517,  506,  315,  103,  442,
 /*  1000 */   507,  508,  519,  527,  513,   85,  518,  526,  163,  447,
 /*  1010 */   377,  390,  394,  403,  532,  543,  406,  415,  424,  427,
 /*  1020 */   418,  421,  430,  436,  433,  533,  439,  131,  133,  135,
 /*  1030 */   550,  144,  548,  551,  552,  155,  159,  164,  308,  554,
 /*  1040 */   329,  315,   96,  442,  328,  555,  558,  562,  256,  258,
 /*  1050 */   257,  560,  460,  447,  377,  390,  394,  403,  561,  564,
 /*  1060 */   406,  415,  424,  427,  418,  421,  430,  436,  433,  565,
 /*  1070 */   439,  131,  133,  135,  441,  144,  441,  441,  441,  155,
 /*  1080 */   159,  164,  308,  172,  441,  315,  441,  442,  441,  441,
 /*  1090 */   441,  256,  258,  257,  441,  441,  314,  447,  377,  390,
 /*  1100 */   394,  403,  441,  441,  406,  415,  424,  427,  418,  421,
 /*  1110 */   430,  436,  433,  441,  439,  131,  133,  135,  441,  144,
 /*  1120 */   441,  441,  441,  155,  159,  164,  308,  145,   22,  315,
 /*  1130 */   307,  442,   21,  441,  441,   85,  441,  441,  256,  258,
 /*  1140 */   257,  447,  377,  390,  394,  403,  321,  441,  406,  415,
 /*  1150 */   424,  427,  418,  421,  430,  436,  433,  268,  439,  131,
 /*  1160 */   133,  135,  441,  144,  441,  441,  441,  155,  159,  164,
 /*  1170 */   308,  441,  441,  315,  130,  441,  821,  441,  322,  323,
 /*  1180 */   324,  325,  326,  441,  441,  358,  377,  390,  394,  403,
 /*  1190 */   225,  497,  265,  441,  246,  441,  448,  113,  114,  115,
 /*  1200 */   116,  117,  118,  119,  120,  121,  122,  123,  124,  125,
 /*  1210 */   126,  127,  128,  129,  130,   67,   68,   70,   69,   71,
 /*  1220 */   441,  468,  441,  441,  307,  384,  386,  388,  441,  441,
 /*  1230 */   441,  459,  256,  258,  257,  441,  448,  113,  114,  115,
 /*  1240 */   116,  117,  118,  119,  120,  121,  122,  123,  124,  125,
 /*  1250 */   126,  127,  128,  129,   73,   74,  130,  595,  521,  441,
 /*  1260 */   515,  228,  441,   19,   72,  384,  386,  388,   22,  256,
 /*  1270 */   258,  257,   86,  480,  441,   85,  297,  260,  448,  113,
 /*  1280 */   114,  115,  116,  117,  118,  119,  120,  121,  122,  123,
 /*  1290 */   124,  125,  126,  127,  128,  129,  130,   67,   68,   70,
 /*  1300 */    69,   71,   67,   68,   70,   69,   71,  384,  386,  388,
 /*  1310 */    90,  441,  546,  154,  441,   96,  441,  441,  448,  113,
 /*  1320 */   114,  115,  116,  117,  118,  119,  120,  121,  122,  123,
 /*  1330 */   124,  125,  126,  127,  128,  129,   73,   74,  441,  130,
 /*  1340 */   441,   73,   74,  441,   23,  187,   72,  384,  386,  388,
 /*  1350 */   391,   72,  441,  256,  258,  257,  474,  441,  256,  258,
 /*  1360 */   257,  448,  113,  114,  115,  116,  117,  118,  119,  120,
 /*  1370 */   121,  122,  123,  124,  125,  126,  127,  128,  129,  441,
 /*  1380 */   441,  130,  536,  441,  539,  540,  528,  185,  507,  508,
 /*  1390 */   384,  386,  388,   85,  441,  256,  258,  257,  503,  441,
 /*  1400 */   441,  441,  441,  448,  113,  114,  115,  116,  117,  118,
 /*  1410 */   119,  120,  121,  122,  123,  124,  125,  126,  127,  128,
 /*  1420 */   129,  130,   67,   68,   70,   69,   71,  400,  136,  441,
 /*  1430 */   441,  307,  384,  386,  388,  256,  258,  257,  157,  256,
 /*  1440 */   258,  257,  441,  448,  113,  114,  115,  116,  117,  118,
 /*  1450 */   119,  120,  121,  122,  123,  124,  125,  126,  127,  128,
 /*  1460 */   129,   73,   74,  441,  130,  441,  441,  169,  441,  104,
 /*  1470 */   307,   72,  384,  386,  388,  404,  441,  441,  256,  258,
 /*  1480 */   257,  162,  441,  256,  258,  257,  448,  113,  114,  115,
 /*  1490 */   116,  117,  118,  119,  120,  121,  122,  123,  124,  125,
 /*  1500 */   126,  127,  128,  129,  441,  441,  130,  441,  505,  101,
 /*  1510 */   520,  528,  311,  534,  508,  384,  386,  388,   85,  441,
 /*  1520 */   256,  258,  257,  464,  441,  441,  441,  441,  448,  113,
 /*  1530 */   114,  115,  116,  117,  118,  119,  120,  121,  122,  123,
 /*  1540 */   124,  125,  126,  127,  128,  129,  130,   67,   68,   70,
 /*  1550 */    69,   71,  441,  160,  441,  441,  307,  384,  386,  388,
 /*  1560 */   441,  441,  441,  138,  256,  258,  257,  441,  448,  113,
 /*  1570 */   114,  115,  116,  117,  118,  119,  120,  121,  122,  123,
 /*  1580 */   124,  125,  126,  127,  128,  129,   73,   74,  103,  130,
 /*  1590 */   441,  102,  510,  441,  228,   85,   72,  384,  386,  388,
 /*  1600 */   441,  441,  256,  258,  257,  441,  313,  441,  441,  261,
 /*  1610 */   260,  448,  113,  114,  115,  116,  117,  118,  119,  120,
 /*  1620 */   121,  122,  123,  124,  125,  126,  127,  128,  129,  441,
 /*  1630 */   441,  130,  441,  441,  537,  520,  528,  300,  441,  508,
 /*  1640 */   384,  386,  388,   85,  441,  256,  258,  257,  166,  441,
 /*  1650 */   441,  441,  441,  448,  113,  114,  115,  116,  117,  118,
 /*  1660 */   119,  120,  121,  122,  123,  124,  125,  126,  127,  128,
 /*  1670 */   129,  130,   67,   68,   70,   69,   71,  441,  441,  441,
 /*  1680 */   441,  450,  384,  386,  388,  441,  441,  441,  142,  256,
 /*  1690 */   258,  257,  441,  448,  113,  114,  115,  116,  117,  118,
 /*  1700 */   119,  120,  121,  122,  123,  124,  125,  126,  127,  128,
 /*  1710 */   129,   73,   74,  441,  130,  441,  441,  228,  441,   77,
 /*  1720 */   457,   72,  384,  386,  388,  256,  258,  257,  256,  258,
 /*  1730 */   257,  110,  441,  259,  441,  441,  448,  113,  114,  115,
 /*  1740 */   116,  117,  118,  119,  120,  121,  122,  123,  124,  125,
 /*  1750 */   126,  127,  128,  129,  228,  441,  130,  441,  441,  441,
 /*  1760 */   441,  441,  256,  258,  257,  384,  386,  388,  441,  291,
 /*  1770 */   260,  441,  441,  487,  441,  441,  441,  441,  448,  113,
 /*  1780 */   114,  115,  116,  117,  118,  119,  120,  121,  122,  123,
 /*  1790 */   124,  125,  126,  127,  128,  129,  130,   67,   68,   70,
 /*  1800 */    69,   71,  441,  441,  441,  441,  462,  384,  386,  388,
 /*  1810 */   441,  441,  441,  452,  256,  258,  257,  441,  448,  113,
 /*  1820 */   114,  115,  116,  117,  118,  119,  120,  121,  122,  123,
 /*  1830 */   124,  125,  126,  127,  128,  129,   73,   74,  441,  130,
 /*  1840 */   441,  441,  301,  441,  491,  182,   72,  384,  386,  388,
 /*  1850 */   256,  258,  257,  256,  258,  257,  441,  441,  441,  441,
 /*  1860 */   441,  112,  113,  114,  115,  116,  117,  118,  119,  120,
 /*  1870 */   121,  122,  123,  124,  125,  126,  127,  128,  129,  178,
 /*  1880 */   181,  184,  441,  441,  441,  441,  441,  220,  441,  441,
 /*  1890 */   384,  386,  388,   67,   68,   70,   69,   71,  441,  174,
 /*  1900 */   229,  233,  249,  171,  299,  441,  224,  178,  181,  184,
 /*  1910 */   441,  441,  441,  441,  266,  220,  441,  441,  441,  235,
 /*  1920 */   217,  214,  256,  258,  257,  441,  441,  174,  229,  233,
 /*  1930 */   249,  171,   73,   74,  224,  600,  489,  178,  181,  184,
 /*  1940 */   441,  441,   72,  215,  441,  220,  441,  235,  441,  214,
 /*  1950 */   441,  256,  258,  257,  441,  237,  243,  174,  229,  233,
 /*  1960 */   249,  171,  441,   51,  224,  236,  238,  239,  240,  241,
 /*  1970 */   242,  441,  441,  441,  441,  449,  441,  235,  211,  214,
 /*  1980 */   107,  209,   33,  237,  243,  441,  256,  258,  257,  256,
 /*  1990 */   258,  257,  441,  236,  238,  239,  240,  241,  242,   54,
 /*  2000 */    55,   56,   57,   58,   59,   60,   61,   62,   63,   64,
 /*  2010 */   441,  441,  441,  237,  243,  367,  441,   67,   68,   70,
 /*  2020 */    69,   71,  441,  236,  238,  239,  240,  241,  242,  441,
 /*  2030 */   441,  178,  181,  184,  441,  441,  441,  441,  207,  220,
 /*  2040 */   441,  441,  441,  441,  441,  441,  256,  258,  257,  441,
 /*  2050 */   523,  174,  229,  233,  249,  171,   73,   74,  224,  178,
 /*  2060 */   181,  184,  441,  441,  491,  441,   72,  220,  441,  441,
 /*  2070 */   441,  235,  441,  214,  441,  441,  441,  441,  441,  174,
 /*  2080 */   229,  233,  249,  171,  252,  213,  224,   51,  205,  441,
 /*  2090 */   441,  441,  441,  256,  258,  257,  256,  258,  257,  235,
 /*  2100 */   441,  214,  203,  441,  494,  441,   33,  237,  243,  335,
 /*  2110 */   256,  258,  257,  441,  441,  441,  441,  236,  238,  239,
 /*  2120 */   240,  241,  242,   54,   55,   56,   57,   58,   59,   60,
 /*  2130 */    61,   62,   63,   64,  201,  237,  243,  441,  441,  441,
 /*  2140 */   441,  441,  256,  258,  257,  236,  238,  239,  240,  241,
 /*  2150 */   242,  441,  441,  178,  181,  184,  228,  441,  441,  199,
 /*  2160 */   441,  220,  441,  441,  256,  258,  257,  256,  258,  257,
 /*  2170 */   441,  253,  260,  174,  229,  233,  249,  171,  441,  244,
 /*  2180 */   224,  393,  178,  181,  184,  441,  441,  256,  258,  257,
 /*  2190 */   220,  441,  441,  235,  441,  214,  278,  279,  282,  441,
 /*  2200 */   441,  441,  174,  229,  233,  249,  171,  286,  219,  224,
 /*  2210 */   441,  178,  181,  184,  441,  441,  256,  258,  257,  220,
 /*  2220 */   441,  441,  235,  441,  214,  441,  441,  441,  441,  237,
 /*  2230 */   243,  174,  229,  233,  249,  171,  293,  197,  224,  236,
 /*  2240 */   238,  239,  240,  241,  242,  256,  258,  257,  441,  441,
 /*  2250 */   321,  235,  195,  214,  441,  223,  441,  441,  237,  243,
 /*  2260 */   256,  258,  257,  256,  258,  257,  441,  441,  236,  238,
 /*  2270 */   239,  240,  241,  242,  441,   67,   68,   70,   69,   71,
 /*  2280 */   441,  441,  322,  323,  324,  325,  326,  237,  243,  334,
 /*  2290 */   441,  441,  343,  344,  441,  441,  441,  236,  238,  239,
 /*  2300 */   240,  241,  242,  441,  441,  178,  181,  184,  441,  441,
 /*  2310 */   441,  441,  441,  220,   73,   74,  441,  594,  521,  441,
 /*  2320 */   515,  441,  441,  441,   72,  174,  229,  233,  249,  171,
 /*  2330 */   193,  441,  224,  178,  181,  184,  441,  441,  256,  258,
 /*  2340 */   257,  220,  441,  441,  441,  235,  441,  214,  441,  441,
 /*  2350 */   441,  441,  441,  174,  229,  233,  249,  171,  271,  191,
 /*  2360 */   224,   51,  306,  441,  441,  441,  441,  256,  258,  257,
 /*  2370 */   256,  258,  257,  235,  441,  214,  441,  441,   80,  441,
 /*  2380 */    33,  237,  243,  359,  441,  441,  441,  441,  441,  441,
 /*  2390 */   441,  236,  238,  239,  240,  241,  242,   54,   55,   56,
 /*  2400 */    57,   58,   59,   60,   61,   62,   63,   64,  441,  237,
 /*  2410 */   243,  441,  441,  441,  441,  441,  441,  441,  441,  236,
 /*  2420 */   238,  239,  240,  241,  242,  441,  441,  178,  181,  184,
 /*  2430 */   228,  441,  441,  234,  441,  220,  441,  441,  256,  258,
 /*  2440 */   257,  256,  258,  257,  441,  269,  260,  174,  229,  233,
 /*  2450 */   249,  171,  189,  441,  224,  441,  178,  181,  184,  441,
 /*  2460 */   256,  258,  257,  441,  220,  302,  441,  235,  441,  214,
 /*  2470 */   441,  441,  441,  256,  258,  257,  174,  229,  233,  249,
 /*  2480 */   171,  179,  441,  245,  441,  178,  181,  184,  441,  256,
 /*  2490 */   258,  257,  441,  220,  441,  461,  235,  441,  214,  441,
 /*  2500 */   441,  441,  441,  237,  243,  174,  229,  233,  249,  171,
 /*  2510 */   263,  441,  224,  236,  238,  239,  240,  241,  242,  441,
 /*  2520 */   441,  441,  441,  441,  441,  235,  441,  214,  441,  441,
 /*  2530 */   441,  441,  273,  243,  274,  441,  441,  441,  441,  441,
 /*  2540 */   441,  441,  272,  238,  239,  240,  241,  242,  228,   67,
 /*  2550 */    68,   70,   69,   71,  441,  441,  256,  258,  257,  441,
 /*  2560 */   441,  237,  243,  284,  260,  441,  441,  441,  441,  441,
 /*  2570 */   441,  236,  238,  239,  240,  241,  242,  441,  441,  178,
 /*  2580 */   181,  184,  441,  441,  441,  441,  441,  220,   73,   74,
 /*  2590 */   441,   97,  521,  441,  515,  441,  441,  441,   72,  174,
 /*  2600 */   229,  233,  249,  171,  441,  441,  224,  178,  181,  184,
 /*  2610 */   441,  441,  441,  441,  441,  220,  441,  441,  441,  235,
 /*  2620 */   441,  214,  441,  441,  441,  441,  441,  174,  229,  233,
 /*  2630 */   249,  171,  441,  441,  245,   51,  441,  441,  441,  441,
 /*  2640 */   441,  441,  441,  441,  441,  441,  441,  235,  441,  214,
 /*  2650 */   441,  441,   26,  441,   33,  237,  243,  441,  441,  441,
 /*  2660 */   441,  441,  441,  441,  441,  236,  238,  239,  240,  241,
 /*  2670 */   242,   54,   55,   56,   57,   58,   59,   60,   61,   62,
 /*  2680 */    63,   64,  441,  273,  243,  441,  441,  441,  441,  441,
 /*  2690 */   441,  441,  441,  272,  238,  239,  240,  241,  242,  441,
 /*  2700 */   441,  441,  441,  441,  212,  208,  210,  173,  183,  188,
 /*  2710 */   190,  192,  194,  186,  180,  196,  198,  206,  200,  202,
 /*  2720 */   204,  173,  183,  188,  190,  192,  194,  186,  180,  196,
 /*  2730 */   198,  206,  200,  202,  204,  441,  451,  441,  441,  441,
 /*  2740 */   441,  441,  441,  441,  441,  441,  441,  212,  208,  210,
 /*  2750 */   173,  183,  188,  190,  192,  194,  186,  180,  196,  198,
 /*  2760 */   206,  200,  202,  204,  212,  208,  210,  173,  183,  188,
 /*  2770 */   190,  192,  194,  186,  180,  196,  198,  206,  200,  202,
 /*  2780 */   204,   67,   68,   70,   69,   71,  441,  441,  441,  441,
 /*  2790 */   441,  441,  441,  441,  441,  441,  458,  441,  244,  441,
 /*  2800 */   441,  441,  441,  441,  441,  441,  256,  258,  257,  441,
 /*  2810 */   441,  441,  275,  441,  441,  283,  279,  282,  441,  441,
 /*  2820 */    73,   74,  401,  598,  509,  441,  515,  441,  441,  441,
 /*  2830 */    72,  441,  441,  212,  208,  210,  173,  183,  188,  190,
 /*  2840 */   192,  194,  186,  180,  196,  198,  206,  200,  202,  204,
 /*  2850 */   212,  208,  210,  173,  183,  188,  190,  192,  194,  186,
 /*  2860 */   180,  196,  198,  206,  200,  202,  204,  212,  208,  210,
 /*  2870 */   173,  183,  188,  190,  192,  194,  186,  180,  196,  198,
 /*  2880 */   206,  200,  202,  204,  441,  441,  441,  321,  441,  441,
 /*  2890 */   441,  441,  392,  441,  441,  441,  441,  441,  441,  441,
 /*  2900 */   441,  441,  441,  441,  441,  441,  441,  441,  728,  441,
 /*  2910 */   441,  441,  441,  441,  441,  441,  441,  441,  441,  322,
 /*  2920 */   323,  324,  325,  326,  330,  303,  366,  441,  354,  374,
 /*  2930 */   375,  441,  441,  441,  441,  441,  212,  208,  210,  173,
 /*  2940 */   183,  188,  190,  192,  194,  186,  180,  196,  198,  206,
 /*  2950 */   200,  202,  204,  441,  441,  441,  441,  441,  441,  441,
 /*  2960 */   441,  441,  441,  441,  441,  441,  441,  441,  463,  441,
 /*  2970 */   441,  441,  441,  441,  441,  441,  441,  441,  441,  212,
 /*  2980 */   208,  210,  173,  183,  188,  190,  192,  194,  186,  180,
 /*  2990 */   196,  198,  206,  200,  202,  204,  212,  208,  210,  173,
 /*  3000 */   183,  188,  190,  192,  194,  186,  180,  196,  198,  206,
 /*  3010 */   200,  202,  204,   67,   68,   70,   69,   71,  441,  441,
 /*  3020 */   441,  441,  441,  441,  441,  441,  441,  441,  312,  441,
 /*  3030 */   441,  441,  441,  441,  441,  441,  441,  441,  441,  441,
 /*  3040 */   441,  441,  441,  441,  441,  441,  441,  441,  441,  441,
 /*  3050 */   441,  441,   73,   74,  267,  596,  489,  441,  441,  441,
 /*  3060 */   441,  441,   72,  441,  441,  212,  208,  210,  173,  183,
 /*  3070 */   188,  190,  192,  194,  186,  180,  196,  198,  206,  200,
 /*  3080 */   202,  204,  441,  441,  441,  441,  441,  216,  212,  208,
 /*  3090 */   210,  173,  183,  188,  190,  192,  194,  186,  180,  196,
 /*  3100 */   198,  206,  200,  202,  204,  208,  210,  173,  183,  188,
 /*  3110 */   190,  192,  194,  186,  180,  196,  198,  206,  200,  202,
 /*  3120 */   204,   51,  441,  441,  441,  441,  441,  441,  441,  441,
 /*  3130 */   441,  441,  441,  441,  441,  441,  441,  441,  441,  441,
 /*  3140 */    33,  441,   67,   68,   70,   69,   71,   67,   68,   70,
 /*  3150 */    69,   71,  441,  441,  441,  441,  441,   54,   55,   56,
 /*  3160 */    57,   58,   59,   60,   61,   62,   63,   64,   67,   68,
 /*  3170 */    70,   69,   71,   67,   68,   70,   69,   71,  441,  441,
 /*  3180 */   441,   73,   74,  441,  597,  489,   73,   74,  441,   18,
 /*  3190 */    75,   72,  441,  441,  441,  441,   72,  441,   67,   68,
 /*  3200 */    70,   69,   71,  441,  441,  441,  529,   73,   74,  441,
 /*  3210 */    20,   75,   73,   74,  441,  441,  441,   72,  441,  441,
 /*  3220 */   104,  441,   72,  441,   67,   68,   70,   69,   71,   67,
 /*  3230 */    68,   70,   69,   71,  441,  441,  441,   73,   74,  441,
 /*  3240 */   601,  489,  441,  441,  441,  441,  441,   72,  441,  441,
 /*  3250 */   441,  441,  441,  441,  441,  441,  441,  441,  441,  441,
 /*  3260 */   441,  441,  441,   73,   74,  441,  599,  538,   73,   74,
 /*  3270 */   441,  602,  489,   72,  441,  441,  441,  441,   72,
};
static YYCODETYPE yy_lookahead[] = {
 /*     0 */     7,    8,    9,  103,  104,  105,  106,  107,   15,  100,
 /*    10 */   101,  102,  103,  104,  105,  106,  107,   43,  161,  162,
 /*    20 */    27,   28,   29,   30,   31,  111,   34,   34,  114,  115,
 /*    30 */   116,  117,  118,  123,  120,  121,  126,  127,  128,  125,
 /*    40 */    47,   67,   49,   69,   51,   52,   53,   54,   55,   56,
 /*    50 */    57,   58,   59,   60,   61,   10,   11,   12,   13,   14,
 /*    60 */    15,   16,   17,   18,   19,   20,   21,   22,   23,   24,
 /*    70 */    25,   26,  110,  150,  112,   34,   83,   84,  155,    6,
 /*    80 */    35,  158,  159,  160,   43,   44,   93,   94,   95,   96,
 /*    90 */    97,   98,   51,   52,   53,   54,   55,   56,   57,   58,
 /*   100 */    59,    6,   61,   62,   63,   64,    6,   66,   24,   25,
 /*   110 */    26,   70,   71,   72,   73,   42,  110,   76,  112,   34,
 /*   120 */    21,   22,   23,   24,   25,   26,   43,   32,   43,   44,
 /*   130 */    89,   90,   91,   92,    6,   35,   51,   52,   53,   54,
 /*   140 */    55,   56,   57,   58,   59,   34,   61,   62,   63,   64,
 /*   150 */    67,   66,   69,   42,   83,   70,   71,   72,   73,   39,
 /*   160 */    32,   76,   42,   34,   93,   94,   95,   96,   97,   98,
 /*   170 */     0,  118,   43,   44,   89,   90,   91,   92,  125,   35,
 /*   180 */    51,   52,   53,   54,   55,   56,   57,   58,   59,   45,
 /*   190 */    61,   62,   63,   64,   35,   66,  126,  127,  128,   70,
 /*   200 */    71,   72,   73,   33,   45,   76,   36,   34,   38,    6,
 /*   210 */    40,   41,  161,  162,   44,   49,   43,   44,   89,   90,
 /*   220 */    91,   92,   45,    0,   51,   52,   53,   54,   55,   56,
 /*   230 */    57,   58,   59,    6,   61,   62,   63,   64,   35,   66,
 /*   240 */     6,  161,  162,   70,   71,   72,   73,   34,   82,   76,
 /*   250 */    84,   34,   86,    6,   77,   78,   79,   80,   81,   32,
 /*   260 */    43,   44,   89,   90,   91,   92,   32,   34,   51,   52,
 /*   270 */    53,   54,   55,   56,   57,   58,   59,    6,   61,   62,
 /*   280 */    63,   64,   82,   66,   84,  118,   34,   70,   71,   72,
 /*   290 */    73,  150,  125,   76,   42,   34,   83,  156,  157,  158,
 /*   300 */   159,  160,    6,   32,   43,   44,   89,   90,   91,   92,
 /*   310 */   161,  162,   51,   52,   53,   54,   55,   56,   57,   58,
 /*   320 */    59,   74,   61,   62,   63,   64,    6,   66,   32,  161,
 /*   330 */   162,   70,   71,   72,   73,   33,    6,   76,   36,   34,
 /*   340 */    38,    6,   40,   41,  161,  162,   44,   49,   43,   44,
 /*   350 */    89,   90,   91,   92,   35,   35,   51,   52,   53,   54,
 /*   360 */    55,   56,   57,   58,   59,   35,   61,   62,   63,   64,
 /*   370 */    35,   66,  161,  162,   34,   70,   71,   72,   73,  150,
 /*   380 */    82,   76,   34,   34,   86,    6,   34,  158,  159,  160,
 /*   390 */    42,   34,   43,   44,   89,   90,   91,   92,  169,   47,
 /*   400 */    51,   52,   53,   54,   55,   56,   57,   58,   59,    6,
 /*   410 */    61,   62,   63,   64,   35,   66,  161,  162,   34,   70,
 /*   420 */    71,   72,   73,   83,  150,   76,    6,   34,  154,    6,
 /*   430 */   108,    6,  158,  159,  160,   32,   43,   44,   89,   90,
 /*   440 */    91,   92,  161,  162,   51,   52,   53,   54,   55,   56,
 /*   450 */    57,   58,   59,    6,   61,   62,   63,   64,   35,   66,
 /*   460 */    35,  151,  152,   70,   71,   72,   73,  108,  150,   76,
 /*   470 */   123,   34,  154,  126,  127,  128,  158,  159,  160,   32,
 /*   480 */    43,   44,   89,   90,   91,   92,  148,  149,   51,   52,
 /*   490 */    53,   54,   55,   56,   57,   58,   59,   37,   61,   62,
 /*   500 */    63,   64,   42,   66,  110,   85,  112,   70,   71,   72,
 /*   510 */    73,  150,   37,   76,   39,   34,  118,   42,  157,  158,
 /*   520 */   159,  160,    6,  125,   43,   44,   89,   90,   91,   92,
 /*   530 */   161,  162,   51,   52,   53,   54,   55,   56,   57,   58,
 /*   540 */    59,    6,   61,   62,   63,   64,    6,   66,   32,   42,
 /*   550 */    37,   70,   71,   72,   73,   42,  150,   76,  123,   34,
 /*   560 */   154,  126,  127,  128,  158,  159,  160,   32,   43,   44,
 /*   570 */    89,   90,   91,   92,  125,   35,   51,   52,   53,   54,
 /*   580 */    55,   56,   57,   58,   59,   34,   61,   62,   63,   64,
 /*   590 */     6,   66,   82,   49,   84,   70,   71,   72,   73,   35,
 /*   600 */   150,   76,  123,   34,  154,  126,  127,  128,  158,  159,
 /*   610 */   160,   35,   43,   44,   89,   90,   91,   92,   45,   35,
 /*   620 */    51,   52,   53,   54,   55,   56,   57,   58,   59,   34,
 /*   630 */    61,   62,   63,   64,    6,   66,  119,   45,  119,   70,
 /*   640 */    71,   72,   73,   34,  150,   76,   45,   34,  154,    6,
 /*   650 */   119,   34,  158,  159,  160,   16,   43,   44,   89,   90,
 /*   660 */    91,   92,   35,   35,   51,   52,   53,   54,   55,   56,
 /*   670 */    57,   58,   59,   49,   61,   62,   63,   64,   35,   66,
 /*   680 */    34,   34,   34,   70,   71,   72,   73,  150,   35,   76,
 /*   690 */   109,   34,  155,   34,  109,  158,  159,  160,   34,  109,
 /*   700 */    43,   44,   89,   90,   91,   92,   43,   34,   51,   52,
 /*   710 */    53,   54,   55,   56,   57,   58,   59,   49,   61,   62,
 /*   720 */    63,   64,   42,   66,   35,   35,   42,   70,   71,   72,
 /*   730 */    73,   65,  150,   76,   42,   34,  154,   42,  149,  119,
 /*   740 */   158,  159,  160,   68,   43,   44,   89,   90,   91,   92,
 /*   750 */    42,   42,   51,   52,   53,   54,   55,   56,   57,   58,
 /*   760 */    59,   42,   61,   62,   63,   64,   35,   66,   71,   34,
 /*   770 */   163,   70,   71,   72,   73,   32,  150,   76,   34,   34,
 /*   780 */   154,   16,   49,   34,  158,  159,  160,    6,   43,   44,
 /*   790 */    89,   90,   91,   92,  163,   49,   51,   52,   53,   54,
 /*   800 */    55,   56,   57,   58,   59,   34,   61,   62,   63,   64,
 /*   810 */    49,   66,   43,   49,   34,   70,   71,   72,   73,   68,
 /*   820 */   150,   76,   68,   34,  154,   43,   68,   49,  158,  159,
 /*   830 */   160,   34,   43,   44,   89,   90,   91,   92,   34,  152,
 /*   840 */    51,   52,   53,   54,   55,   56,   57,   58,   59,  153,
 /*   850 */    61,   62,   63,   64,   43,   66,  153,  153,   85,   70,
 /*   860 */    71,   72,   73,  153,  150,   76,  153,   34,  154,   43,
 /*   870 */   153,  153,  158,  159,  160,  153,   43,   44,   89,   90,
 /*   880 */    91,   92,  153,   85,   51,   52,   53,   54,   55,   56,
 /*   890 */    57,   58,   59,  153,   61,   62,   63,   64,   85,   66,
 /*   900 */   153,   35,   35,   70,   71,   72,   73,  150,   35,   76,
 /*   910 */    34,   34,  155,   34,  162,  158,  159,  160,   35,   35,
 /*   920 */    43,   44,   89,   90,   91,   92,   34,   45,   51,   52,
 /*   930 */    53,   54,   55,   56,   57,   58,   59,  119,   61,   62,
 /*   940 */    63,   64,   34,   66,   34,   74,   71,   70,   71,   72,
 /*   950 */    73,   35,  150,   76,   68,   34,  154,   42,   65,   42,
 /*   960 */   158,  159,  160,   34,   43,   44,   89,   90,   91,   92,
 /*   970 */    42,   49,   51,   52,   53,   54,   55,   56,   57,   58,
 /*   980 */    59,   42,   61,   62,   63,   64,   35,   66,   47,   45,
 /*   990 */    47,   70,   71,   72,   73,   45,  116,   76,  118,   34,
 /*  1000 */   120,  121,   35,   35,  119,  125,  119,  119,   43,   44,
 /*  1010 */    89,   90,   91,   92,  119,  112,   51,   52,   53,   54,
 /*  1020 */    55,   56,   57,   58,   59,   35,   61,   62,   63,   64,
 /*  1030 */    38,   66,  109,   34,  109,   70,   71,   72,   73,   34,
 /*  1040 */   150,   76,   42,   34,  154,  109,  109,  109,  158,  159,
 /*  1050 */   160,   38,   43,   44,   89,   90,   91,   92,   34,   34,
 /*  1060 */    51,   52,   53,   54,   55,   56,   57,   58,   59,  109,
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
 /*  1460 */   146,   40,   41,  170,  107,  170,  170,  147,  170,   48,
 /*  1470 */   150,   50,  158,  159,  160,  150,  170,  170,  158,  159,
 /*  1480 */   160,  124,  170,  158,  159,  160,  129,  130,  131,  132,
 /*  1490 */   133,  134,  135,  136,  137,  138,  139,  140,  141,  142,
 /*  1500 */   143,  144,  145,  146,  170,  170,  107,  170,  115,  116,
 /*  1510 */   117,  118,  150,  120,  121,  158,  159,  160,  125,  170,
 /*  1520 */   158,  159,  160,  124,  170,  170,  170,  170,  129,  130,
 /*  1530 */   131,  132,  133,  134,  135,  136,  137,  138,  139,  140,
 /*  1540 */   141,  142,  143,  144,  145,  146,  107,    1,    2,    3,
 /*  1550 */     4,    5,  170,  147,  170,  170,  150,  158,  159,  160,
 /*  1560 */   170,  170,  170,  124,  158,  159,  160,  170,  129,  130,
 /*  1570 */   131,  132,  133,  134,  135,  136,  137,  138,  139,  140,
 /*  1580 */   141,  142,  143,  144,  145,  146,   40,   41,  118,  107,
 /*  1590 */   170,  121,   46,  170,  150,  125,   50,  158,  159,  160,
 /*  1600 */   170,  170,  158,  159,  160,  170,  124,  170,  170,  165,
 /*  1610 */   166,  129,  130,  131,  132,  133,  134,  135,  136,  137,
 /*  1620 */   138,  139,  140,  141,  142,  143,  144,  145,  146,  170,
 /*  1630 */   170,  107,  170,  170,  116,  117,  118,  150,  170,  121,
 /*  1640 */   158,  159,  160,  125,  170,  158,  159,  160,  124,  170,
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
 /*  1750 */   143,  144,  145,  146,  150,  170,  107,  170,  170,  170,
 /*  1760 */   170,  170,  158,  159,  160,  158,  159,  160,  170,  165,
 /*  1770 */   166,  170,  170,  124,  170,  170,  170,  170,  129,  130,
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
 /*  1880 */     8,    9,  170,  170,  170,  170,  170,   15,  170,  170,
 /*  1890 */   158,  159,  160,    1,    2,    3,    4,    5,  170,   27,
 /*  1900 */    28,   29,   30,   31,   32,  170,   34,    7,    8,    9,
 /*  1910 */   170,  170,  170,  170,  150,   15,  170,  170,  170,   47,
 /*  1920 */   127,   49,  158,  159,  160,  170,  170,   27,   28,   29,
 /*  1930 */    30,   31,   40,   41,   34,   43,   44,    7,    8,    9,
 /*  1940 */   170,  170,   50,  150,  170,   15,  170,   47,  170,   49,
 /*  1950 */   170,  158,  159,  160,  170,   83,   84,   27,   28,   29,
 /*  1960 */    30,   31,  170,   15,   34,   93,   94,   95,   96,   97,
 /*  1970 */    98,  170,  170,  170,  170,   75,  170,   47,  150,   49,
 /*  1980 */    32,  150,   34,   83,   84,  170,  158,  159,  160,  158,
 /*  1990 */   159,  160,  170,   93,   94,   95,   96,   97,   98,   51,
 /*  2000 */    52,   53,   54,   55,   56,   57,   58,   59,   60,   61,
 /*  2010 */   170,  170,  170,   83,   84,   85,  170,    1,    2,    3,
 /*  2020 */     4,    5,  170,   93,   94,   95,   96,   97,   98,  170,
 /*  2030 */   170,    7,    8,    9,  170,  170,  170,  170,  150,   15,
 /*  2040 */   170,  170,  170,  170,  170,  170,  158,  159,  160,  170,
 /*  2050 */    34,   27,   28,   29,   30,   31,   40,   41,   34,    7,
 /*  2060 */     8,    9,  170,  170,   48,  170,   50,   15,  170,  170,
 /*  2070 */   170,   47,  170,   49,  170,  170,  170,  170,  170,   27,
 /*  2080 */    28,   29,   30,   31,   32,  150,   34,   15,  150,  170,
 /*  2090 */   170,  170,  170,  158,  159,  160,  158,  159,  160,   47,
 /*  2100 */   170,   49,  150,  170,   32,  170,   34,   83,   84,   85,
 /*  2110 */   158,  159,  160,  170,  170,  170,  170,   93,   94,   95,
 /*  2120 */    96,   97,   98,   51,   52,   53,   54,   55,   56,   57,
 /*  2130 */    58,   59,   60,   61,  150,   83,   84,  170,  170,  170,
 /*  2140 */   170,  170,  158,  159,  160,   93,   94,   95,   96,   97,
 /*  2150 */    98,  170,  170,    7,    8,    9,  150,  170,  170,  150,
 /*  2160 */   170,   15,  170,  170,  158,  159,  160,  158,  159,  160,
 /*  2170 */   170,  165,  166,   27,   28,   29,   30,   31,  170,  150,
 /*  2180 */    34,   35,    7,    8,    9,  170,  170,  158,  159,  160,
 /*  2190 */    15,  170,  170,   47,  170,   49,  167,  168,  169,  170,
 /*  2200 */   170,  170,   27,   28,   29,   30,   31,   32,  150,   34,
 /*  2210 */   170,    7,    8,    9,  170,  170,  158,  159,  160,   15,
 /*  2220 */   170,  170,   47,  170,   49,  170,  170,  170,  170,   83,
 /*  2230 */    84,   27,   28,   29,   30,   31,   32,  150,   34,   93,
 /*  2240 */    94,   95,   96,   97,   98,  158,  159,  160,  170,  170,
 /*  2250 */    45,   47,  150,   49,  170,  150,  170,  170,   83,   84,
 /*  2260 */   158,  159,  160,  158,  159,  160,  170,  170,   93,   94,
 /*  2270 */    95,   96,   97,   98,  170,    1,    2,    3,    4,    5,
 /*  2280 */   170,  170,   77,   78,   79,   80,   81,   83,   84,   84,
 /*  2290 */   170,  170,   87,   88,  170,  170,  170,   93,   94,   95,
 /*  2300 */    96,   97,   98,  170,  170,    7,    8,    9,  170,  170,
 /*  2310 */   170,  170,  170,   15,   40,   41,  170,   43,   44,  170,
 /*  2320 */    46,  170,  170,  170,   50,   27,   28,   29,   30,   31,
 /*  2330 */   150,  170,   34,    7,    8,    9,  170,  170,  158,  159,
 /*  2340 */   160,   15,  170,  170,  170,   47,  170,   49,  170,  170,
 /*  2350 */   170,  170,  170,   27,   28,   29,   30,   31,   32,  150,
 /*  2360 */    34,   15,  150,  170,  170,  170,  170,  158,  159,  160,
 /*  2370 */   158,  159,  160,   47,  170,   49,  170,  170,   32,  170,
 /*  2380 */    34,   83,   84,   85,  170,  170,  170,  170,  170,  170,
 /*  2390 */   170,   93,   94,   95,   96,   97,   98,   51,   52,   53,
 /*  2400 */    54,   55,   56,   57,   58,   59,   60,   61,  170,   83,
 /*  2410 */    84,  170,  170,  170,  170,  170,  170,  170,  170,   93,
 /*  2420 */    94,   95,   96,   97,   98,  170,  170,    7,    8,    9,
 /*  2430 */   150,  170,  170,  150,  170,   15,  170,  170,  158,  159,
 /*  2440 */   160,  158,  159,  160,  170,  165,  166,   27,   28,   29,
 /*  2450 */    30,   31,  150,  170,   34,  170,    7,    8,    9,  170,
 /*  2460 */   158,  159,  160,  170,   15,  150,  170,   47,  170,   49,
 /*  2470 */   170,  170,  170,  158,  159,  160,   27,   28,   29,   30,
 /*  2480 */    31,  150,  170,   34,  170,    7,    8,    9,  170,  158,
 /*  2490 */   159,  160,  170,   15,  170,   75,   47,  170,   49,  170,
 /*  2500 */   170,  170,  170,   83,   84,   27,   28,   29,   30,   31,
 /*  2510 */    32,  170,   34,   93,   94,   95,   96,   97,   98,  170,
 /*  2520 */   170,  170,  170,  170,  170,   47,  170,   49,  170,  170,
 /*  2530 */   170,  170,   83,   84,   85,  170,  170,  170,  170,  170,
 /*  2540 */   170,  170,   93,   94,   95,   96,   97,   98,  150,    1,
 /*  2550 */     2,    3,    4,    5,  170,  170,  158,  159,  160,  170,
 /*  2560 */   170,   83,   84,  165,  166,  170,  170,  170,  170,  170,
 /*  2570 */   170,   93,   94,   95,   96,   97,   98,  170,  170,    7,
 /*  2580 */     8,    9,  170,  170,  170,  170,  170,   15,   40,   41,
 /*  2590 */   170,   43,   44,  170,   46,  170,  170,  170,   50,   27,
 /*  2600 */    28,   29,   30,   31,  170,  170,   34,    7,    8,    9,
 /*  2610 */   170,  170,  170,  170,  170,   15,  170,  170,  170,   47,
 /*  2620 */   170,   49,  170,  170,  170,  170,  170,   27,   28,   29,
 /*  2630 */    30,   31,  170,  170,   34,   15,  170,  170,  170,  170,
 /*  2640 */   170,  170,  170,  170,  170,  170,  170,   47,  170,   49,
 /*  2650 */   170,  170,   32,  170,   34,   83,   84,  170,  170,  170,
 /*  2660 */   170,  170,  170,  170,  170,   93,   94,   95,   96,   97,
 /*  2670 */    98,   51,   52,   53,   54,   55,   56,   57,   58,   59,
 /*  2680 */    60,   61,  170,   83,   84,  170,  170,  170,  170,  170,
 /*  2690 */   170,  170,  170,   93,   94,   95,   96,   97,   98,  170,
 /*  2700 */   170,  170,  170,  170,   10,   11,   12,   13,   14,   15,
 /*  2710 */    16,   17,   18,   19,   20,   21,   22,   23,   24,   25,
 /*  2720 */    26,   13,   14,   15,   16,   17,   18,   19,   20,   21,
 /*  2730 */    22,   23,   24,   25,   26,  170,   42,  170,  170,  170,
 /*  2740 */   170,  170,  170,  170,  170,  170,  170,   10,   11,   12,
 /*  2750 */    13,   14,   15,   16,   17,   18,   19,   20,   21,   22,
 /*  2760 */    23,   24,   25,   26,   10,   11,   12,   13,   14,   15,
 /*  2770 */    16,   17,   18,   19,   20,   21,   22,   23,   24,   25,
 /*  2780 */    26,    1,    2,    3,    4,    5,  170,  170,  170,  170,
 /*  2790 */   170,  170,  170,  170,  170,  170,   42,  170,  150,  170,
 /*  2800 */   170,  170,  170,  170,  170,  170,  158,  159,  160,  170,
 /*  2810 */   170,  170,  164,  170,  170,  167,  168,  169,  170,  170,
 /*  2820 */    40,   41,   85,   43,   44,  170,   46,  170,  170,  170,
 /*  2830 */    50,  170,  170,   10,   11,   12,   13,   14,   15,   16,
 /*  2840 */    17,   18,   19,   20,   21,   22,   23,   24,   25,   26,
 /*  2850 */    10,   11,   12,   13,   14,   15,   16,   17,   18,   19,
 /*  2860 */    20,   21,   22,   23,   24,   25,   26,   10,   11,   12,
 /*  2870 */    13,   14,   15,   16,   17,   18,   19,   20,   21,   22,
 /*  2880 */    23,   24,   25,   26,  170,  170,  170,   45,  170,  170,
 /*  2890 */   170,  170,   35,  170,  170,  170,  170,  170,  170,  170,
 /*  2900 */   170,  170,  170,  170,  170,  170,  170,  170,   85,  170,
 /*  2910 */   170,  170,  170,  170,  170,  170,  170,  170,  170,   77,
 /*  2920 */    78,   79,   80,   81,   82,   85,   84,  170,   86,   87,
 /*  2930 */    88,  170,  170,  170,  170,  170,   10,   11,   12,   13,
 /*  2940 */    14,   15,   16,   17,   18,   19,   20,   21,   22,   23,
 /*  2950 */    24,   25,   26,  170,  170,  170,  170,  170,  170,  170,
 /*  2960 */   170,  170,  170,  170,  170,  170,  170,  170,   42,  170,
 /*  2970 */   170,  170,  170,  170,  170,  170,  170,  170,  170,   10,
 /*  2980 */    11,   12,   13,   14,   15,   16,   17,   18,   19,   20,
 /*  2990 */    21,   22,   23,   24,   25,   26,   10,   11,   12,   13,
 /*  3000 */    14,   15,   16,   17,   18,   19,   20,   21,   22,   23,
 /*  3010 */    24,   25,   26,    1,    2,    3,    4,    5,  170,  170,
 /*  3020 */   170,  170,  170,  170,  170,  170,  170,  170,   42,  170,
 /*  3030 */   170,  170,  170,  170,  170,  170,  170,  170,  170,  170,
 /*  3040 */   170,  170,  170,  170,  170,  170,  170,  170,  170,  170,
 /*  3050 */   170,  170,   40,   41,   85,   43,   44,  170,  170,  170,
 /*  3060 */   170,  170,   50,  170,  170,   10,   11,   12,   13,   14,
 /*  3070 */    15,   16,   17,   18,   19,   20,   21,   22,   23,   24,
 /*  3080 */    25,   26,  170,  170,  170,  170,  170,   32,   10,   11,
 /*  3090 */    12,   13,   14,   15,   16,   17,   18,   19,   20,   21,
 /*  3100 */    22,   23,   24,   25,   26,   11,   12,   13,   14,   15,
 /*  3110 */    16,   17,   18,   19,   20,   21,   22,   23,   24,   25,
 /*  3120 */    26,   15,  170,  170,  170,  170,  170,  170,  170,  170,
 /*  3130 */   170,  170,  170,  170,  170,  170,  170,  170,  170,  170,
 /*  3140 */    34,  170,    1,    2,    3,    4,    5,    1,    2,    3,
 /*  3150 */     4,    5,  170,  170,  170,  170,  170,   51,   52,   53,
 /*  3160 */    54,   55,   56,   57,   58,   59,   60,   61,    1,    2,
 /*  3170 */     3,    4,    5,    1,    2,    3,    4,    5,  170,  170,
 /*  3180 */   170,   40,   41,  170,   43,   44,   40,   41,  170,   43,
 /*  3190 */    44,   50,  170,  170,  170,  170,   50,  170,    1,    2,
 /*  3200 */     3,    4,    5,  170,  170,  170,   34,   40,   41,  170,
 /*  3210 */    43,   44,   40,   41,  170,  170,  170,   50,  170,  170,
 /*  3220 */    48,  170,   50,  170,    1,    2,    3,    4,    5,    1,
 /*  3230 */     2,    3,    4,    5,  170,  170,  170,   40,   41,  170,
 /*  3240 */    43,   44,  170,  170,  170,  170,  170,   50,  170,  170,
 /*  3250 */   170,  170,  170,  170,  170,  170,  170,  170,  170,  170,
 /*  3260 */   170,  170,  170,   40,   41,  170,   43,   44,   40,   41,
 /*  3270 */   170,   43,   44,   50,  170,  170,  170,  170,   50,
};
#define YY_SHIFT_USE_DFLT (-27)
static short yy_shift_ofst[] = {
 /*     0 */   302,  223,  170,  -27,  -27,  -27,  -27,  -27,  233,  319,
 /*    10 */   -27,  357,  460,  -27,  384,  507,  -27, 3146,  -27, 3167,
 /*    20 */   -27,  -27, 1296,  551,  544, 2620,  564,  -27,  227,  576,
 /*    30 */   -27, 3106,  -27,  573,   71,  -27,  -27,  -27,  -27,  -27,
 /*    40 */   -27,  -27,  -27,  595,  592,   71,  -27,  609,  601,   71,
 /*    50 */   -27,  617,  639,  646,  -27,  -27,  -27,  -27,  -27,  -27,
 /*    60 */   -27,  -27,  -27,  -27,  -27,  -27,  -27,  -27,  -27,  -27,
 /*    70 */   -27,  -27,  -27,  -27,  -27, 1301, 1671,  647,  624, 2346,
 /*    80 */   627,  -27,  447,  653,  -27,  -27,  -27,  648, 1273,  -27,
 /*    90 */   659,  120,  -27,  664,   73,  -27, 2548,  -27,  663,  -27,
 /*   100 */  2274, 3141,  -27, 1421,  673,  668, 1948,  680,  877,  -27,
 /*   110 */   261,  -27,  -27,  -27,  -27,  -27,  -27,  -27,  -27,  -27,
 /*   120 */   -27,  -27,  -27,  -27,  -27,  -27,  -27,  -27,  -27,  -27,
 /*   130 */   -27,  689,  -27,  690,  -27, 2572,  684,  525,  393,  666,
 /*   140 */   692,  657,  129,  -27, 2572,  695,  -26,  -27,   83,  -27,
 /*   150 */   -27,   71,  675, 1097, 1097,  708,  613,  745,  -27, 2572,
 /*   160 */   709,  789,  965,  -27,  719,  921,  701,  697, 2572,  731,
 /*   170 */   -27, 2572,  -27, 2572,  735,  -27,  200, 2572, 2572, 3078,
 /*   180 */  2572, 2572, 3078, 2572, 2572, 3078, 2572,   99, 2572,   99,
 /*   190 */  2572,   99, 2572,   99, 2572,   99, 2572,   84, 2572,   84,
 /*   200 */  2572,  -27, 2572,  -27, 2572,  -27, 2572,   84, 2572, 2708,
 /*   210 */  2572, 2708, 2572, 3094,   -7, 3055,  -27,  743, 2572,  -27,
 /*   220 */   744,  765, 2572,   99,  166,  348,  733, 2175, 3078,  749,
 /*   230 */   781,  735,  -27, 2572,  -27,  -27,  -27,  -27,  -27,  -27,
 /*   240 */   -27,  -27,  -27, 2449, 3078, 1108,  352,  746, 2478,  771,
 /*   250 */   761, 2052,  -27,  234,  -27, 2572,  -27,  -27,  -27,  -27,
 /*   260 */   -27,  128,  -27,  -27,  -27, 2572, 2969,  -27, 2326,  516,
 /*   270 */   -27,  -27,  751,  754,  -27,  420,  -27, 2600,  -27,  758,
 /*   280 */  2572,  -27,  -27,  -27,  403,  -27,  -27,  340,  769,  764,
 /*   290 */  2204,  296,  -27,  -27,  782,  778, 1872,  271,  -27,  -27,
 /*   300 */    99,   99, 2840,  -27,  780,  -27,   99, 3078,  797,  247,
 /*   310 */  1900, 2986, 1097, 1053,  -27,  804,  584,  -27,  804,  -27,
 /*   320 */  2842,  -27,  -27,  -27,  -27,  -27,  -27, 2572,  -27, 3078,
 /*   330 */   111, 2205, 2572,  -27, 2024,  177, 2572,  -27,  773,  177,
 /*   340 */  2572,  -27, 2823,  -27,  -27,  213,  811,  177, 2572,  -27,
 /*   350 */   826,  177, 2572,  -27,   -8, 1101, 2572,  -27, 2298,  177,
 /*   360 */  2572,  -27,  798,  177, 2572,  -27, 1930,  177, 2572,  -27,
 /*   370 */   813,  177, 2572,  -27,  -27,  -27,  -27, 2572,  643,  -27,
 /*   380 */  2572,  -27, 3078,  -27,  866,  -27,  867,  -27,  873,  -27,
 /*   390 */  2146, 2857,  -27,  -27,  876,  510,  879,  883,  -27, 2572,
 /*   400 */  2737,  884,  -27, 2572,   45,  -27,  892,  100,  -27,  892,
 /*   410 */   -27,  882,   71,  -27,  -27,  892,  628,  -27,  892,  540,
 /*   420 */   -27,  892,  379,  -27,  892,  320,  -27,  892,  203,  -27,
 /*   430 */   892,  335,  -27,  892,  330,  -27,  892,  423,  -27,  892,
 /*   440 */   425,  -27,  298,  908,  746,  252,  733,  -27,  -27, 2572,
 /*   450 */  2694, 1097,  349,  -27,  910,  871, 2420, 2754, 1097, 1009,
 /*   460 */   -27, 2572, 2926, 1097,   85,  -27,  875, 2572,  916,  -27,
 /*   470 */   -27,  -27,  886, 1097, 1097,  -27,  -27,  893,  915,  481,
 /*   480 */   305,  -27,  -27,  535,  917,  173,  -27,  833,  -27, 1301,
 /*   490 */  1796,  929,  922, 2072,  928,   41,  -27,  217,  -27,   95,
 /*   500 */   939,  437,  -27,  569,  -27, 2780, 3197,  -27,  -27, 1546,
 /*   510 */   941,  944,   71,  951,  -27,  943,  950,   71,  967,  -27,
 /*   520 */   -27, 1546, 2016,  144,  -27,   71,  968,  -27, 3172,  159,
 /*   530 */   -27,   71,  990,  -27,  -27, 1214, 3223, 3228, 1301, 1892,
 /*   540 */   -27, 3012,  664,  -27,  -27,  -27,  664,   73,  -27,  992,
 /*   550 */   999,  475,  -27, 1005, 1000,  -27,  664,   73,  -27, 1013,
 /*   560 */  1024,  513,  -27, 1025, 1000,  -27,  -27,
};
#define YY_REDUCE_USE_DFLT (-144)
static short yy_reduce_ofst[] = {
 /*     0 */   -91, -144, -100, -144, -144, -144, -144, -144, -144, -144,
 /*    10 */  -144, -144,  322, -144, -144,  359, -144, 1150, -144, 1010,
 /*    20 */  -144, -144,  449, -144, -144,  479, -144, -144, -144, -144,
 /*    30 */  -144,   70, -144, -144,  517, -144, -144, -144, -144, -144,
 /*    40 */  -144, -144, -144, -144, -144,  519, -144, -144, -144,  531,
 /*    50 */  -144, -144, -144, -144, -144, -144, -144, -144, -144, -144,
 /*    60 */  -144, -144, -144, -144, -144, -144, -144, -144, -144, -144,
 /*    70 */  -144, -144, -144, -144, -144,   53,  449, -144, -144,  347,
 /*    80 */  -144, -144, -144, -144, -144, -144, -144, -144,  581, -144,
 /*    90 */  -144,  585, -144,  -38,  590, -144,  -86, -144, -144, -144,
 /*   100 */  1393, 1470, -144,  449, -144, -144,  -90, -144, 1607, -144,
 /*   110 */  1732, -144, -144, -144, -144, -144, -144, -144, -144, -144,
 /*   120 */  -144, -144, -144, -144, -144, -144, -144, -144, -144, -144,
 /*   130 */  -144, -144, -144, -144, -144, 1281, -144, 1439, 1732, -144,
 /*   140 */  -144, 1564, 1732, -144,  980, -144,  338, -144,  589, -144,
 /*   150 */  -144,  620, -144, 1189, 1732, -144, 1314, 1732, -144, 1406,
 /*   160 */  -144, 1357, 1732, -144, -144, 1524, 1732, -144, 1320, -144,
 /*   170 */  -144,  933, -144, 2212,  607, -144, -144, 2315, 2331, -144,
 /*   180 */  1692, 1695, -144, 1487, 1237, -144, 1195, -144, 2302, -144,
 /*   190 */  2209, -144, 2180, -144, 2102, -144, 2087, -144, 2009, -144,
 /*   200 */  1984, -144, 1952, -144, 1938, -144, 1888, -144, 1831, -144,
 /*   210 */  1828, -144, 1935, -144, 1793, -144, -144, -144, 2058, -144,
 /*   220 */  -144, -144, 2105, -144, -144, -144, -144, 2398, -144, -144,
 /*   230 */  -144,  631, -144, 2283, -144, -144, -144, -144, -144, -144,
 /*   240 */  -144, -144, -144, 2648, -144, -144, -144, -144, 1444, -144,
 /*   250 */  -144, 2006, -144, -144, -144, 1567, -144, -144, -144, -144,
 /*   260 */  -144, -144, -144, -144, -144, 1764, -144, -144, 2280, -144,
 /*   270 */  -144, -144, -144, -144, -144, -144, -144, 2029, -144, -144,
 /*   280 */   229, -144, -144, -144, -144, -144, -144, -144, -144, -144,
 /*   290 */  1604, -144, -144, -144, -144, -144, 1111, -144, -144, -144,
 /*   300 */  -144, -144, -144, -144, -144, -144, -144, -144, -144, -144,
 /*   310 */  1362, -144, 1482, 1732, -144,  310, -144, -144,  687, -144,
 /*   320 */   696, -144, -144, -144, -144, -144, -144,  890, -144, -144,
 /*   330 */  -144,  703,  802, -144,  757,  704,  714, -144, -144,  710,
 /*   340 */   670, -144, -144, -144, -144, -144, -144,  713,  582, -144,
 /*   350 */  -144,  717,  494, -144, -144,  718,  406, -144,  -77,  722,
 /*   360 */   626, -144, -144,  729,  450, -144,  537,  740,  318, -144,
 /*   370 */  -144,  747,  274, -144, -144, -144, -144,  141, -144, -144,
 /*   380 */   361, -144, -144, -144, -144, -144, -144, -144, -144, -144,
 /*   390 */  1200, -144, -144, -144, -144, -144, -144, -144, -144, 1277,
 /*   400 */  -144, -144, -144, 1325, -144, -144,   51, -144, -144,  752,
 /*   410 */  -144, -144,  818, -144, -144, -143, -144, -144,  369, -144,
 /*   420 */  -144,  281, -144, -144,  183, -144, -144,  149, -144, -144,
 /*   430 */    80, -144, -144,  168, -144, -144,  211, -144, -144,  255,
 /*   440 */  -144, -144, -144, -144, -144, -144, -144, -144, -144, 1531,
 /*   450 */  -144, 1689, 1732, -144, -144, -144, 1570, -144, 1107, 1732,
 /*   460 */  -144, 1656, -144, 1399, 1732, -144, -144, 1074, -144, -144,
 /*   470 */  -144, -144, -144, 1232, 1732, -144, -144, -144, -144, 1149,
 /*   480 */  1732, -144, -144, -144, -144, 1649, -144, 1732, -144,  398,
 /*   490 */   449, -144, -144,  435, -144, 1067, -144, 1732, -144, -144,
 /*   500 */  -144, 1274, -144, 1732, -144,  880, 1470, -144, -144,  398,
 /*   510 */  -144, -144,  885, -144, -144, -144, -144,  887, -144, -144,
 /*   520 */  -144,  167,  449, -144, -144,  888, -144, -144,  449, -144,
 /*   530 */  -144,  895, -144, -144, -144, 1268, 1518, 1470,  167, 1470,
 /*   540 */  -144, 1470,  903, -144, -144, -144,  394,  923, -144, -144,
 /*   550 */  -144,  925, -144, -144,  936, -144,    6,  937, -144, -144,
 /*   560 */  -144,  938, -144, -144,  960, -144, -144,
};
static YYACTIONTYPE yy_default[] = {
 /*     0 */   836,  836,  836,  569,  571,  572,  573,  574,  836,  836,
 /*    10 */   575,  836,  836,  576,  836,  836,  577,  836,  592,  836,
 /*    20 */   593,  613,  836,  836,  836,  836,  836,  625,  836,  836,
 /*    30 */   626,  836,  639,  641,  836,  644,  825,  826,  827,  828,
 /*    40 */   829,  830,  831,  836,  642,  836,  645,  836,  643,  836,
 /*    50 */   646,  836,  836,  836,  648,  649,  650,  651,  652,  653,
 /*    60 */   654,  655,  656,  657,  658,  640,  629,  631,  632,  633,
 /*    70 */   634,  635,  636,  637,  638,  836,  836,  836,  836,  836,
 /*    80 */   836,  627,  836,  836,  628,  630,  614,  836,  836,  578,
 /*    90 */   836,  836,  579,  836,  836,  581,  836,  587,  836,  588,
 /*   100 */   836,  836,  611,  836,  836,  836,  836,  836,  836,  617,
 /*   110 */   836,  619,  659,  661,  662,  663,  664,  665,  666,  667,
 /*   120 */   668,  669,  670,  671,  672,  673,  674,  675,  676,  677,
 /*   130 */   678,  836,  679,  836,  680,  836,  836,  836,  836,  683,
 /*   140 */   836,  836,  836,  684,  836,  836,  836,  687,  836,  688,
 /*   150 */   689,  836,  836,  691,  692,  836,  836,  836,  695,  836,
 /*   160 */   836,  836,  836,  697,  836,  836,  836,  836,  836,  836,
 /*   170 */   699,  836,  756,  836,  836,  757,  836,  836,  836,  758,
 /*   180 */   836,  836,  759,  836,  836,  760,  836,  764,  836,  765,
 /*   190 */   836,  766,  836,  767,  836,  768,  836,  776,  836,  777,
 /*   200 */   836,  778,  836,  779,  836,  780,  836,  781,  836,  782,
 /*   210 */   836,  783,  836,  784,  836,  836,  769,  836,  836,  770,
 /*   220 */   836,  836,  836,  771,  788,  836,  772,  836,  816,  836,
 /*   230 */   836,  836,  785,  836,  786,  787,  789,  790,  791,  792,
 /*   240 */   793,  794,  795,  836,  824,  788,  836,  773,  836,  836,
 /*   250 */   798,  836,  799,  836,  800,  836,  811,  812,  813,  814,
 /*   260 */   815,  836,  803,  804,  774,  836,  836,  775,  836,  836,
 /*   270 */   801,  802,  789,  790,  796,  836,  797,  836,  817,  836,
 /*   280 */   836,  819,  820,  818,  836,  805,  806,  836,  836,  836,
 /*   290 */   836,  836,  807,  808,  836,  836,  836,  836,  809,  810,
 /*   300 */   763,  762,  836,  832,  836,  833,  761,  834,  836,  836,
 /*   310 */   836,  836,  836,  836,  700,  836,  836,  704,  836,  705,
 /*   320 */   836,  707,  708,  709,  710,  711,  712,  836,  713,  755,
 /*   330 */   836,  836,  836,  714,  836,  836,  836,  717,  836,  836,
 /*   340 */   836,  718,  836,  724,  725,  836,  836,  836,  836,  715,
 /*   350 */   836,  836,  836,  716,  836,  836,  836,  719,  836,  836,
 /*   360 */   836,  720,  836,  836,  836,  721,  836,  836,  836,  722,
 /*   370 */   836,  836,  836,  723,  726,  727,  706,  836,  836,  729,
 /*   380 */   836,  730,  732,  731,  836,  733,  836,  734,  836,  735,
 /*   390 */   836,  836,  736,  737,  836,  836,  836,  836,  738,  836,
 /*   400 */   836,  836,  739,  836,  836,  740,  836,  836,  741,  836,
 /*   410 */   751,  753,  836,  754,  752,  836,  836,  742,  836,  836,
 /*   420 */   743,  836,  836,  744,  836,  836,  745,  836,  836,  746,
 /*   430 */   836,  836,  747,  836,  836,  748,  836,  836,  749,  836,
 /*   440 */   836,  750,  836,  836,  836,  836,  836,  835,  660,  836,
 /*   450 */   836,  836,  836,  701,  836,  836,  836,  836,  836,  836,
 /*   460 */   702,  836,  836,  836,  836,  703,  836,  836,  836,  698,
 /*   470 */   696,  694,  836,  836,  693,  690,  685,  681,  836,  836,
 /*   480 */   836,  686,  682,  836,  836,  836,  618,  836,  620,  836,
 /*   490 */   836,  836,  836,  836,  836,  836,  621,  836,  623,  836,
 /*   500 */   836,  836,  622,  836,  624,  836,  836,  609,  612,  836,
 /*   510 */   836,  836,  836,  836,  615,  836,  836,  836,  836,  616,
 /*   520 */   603,  836,  836,  836,  605,  836,  836,  607,  836,  836,
 /*   530 */   606,  836,  836,  608,  610,  836,  836,  836,  836,  836,
 /*   540 */   604,  836,  836,  589,  591,  590,  836,  836,  580,  836,
 /*   550 */   836,  836,  582,  836,  836,  583,  836,  836,  584,  836,
 /*   560 */   836,  836,  585,  836,  836,  586,  570,
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
 /* 265 */ "xx_isset_expr ::= IDENTIFIER SBRACKET_OPEN xx_common_expr SBRACKET_CLOSE",
 /* 266 */ "xx_isset_expr ::= IDENTIFIER ARROW IDENTIFIER",
 /* 267 */ "xx_eval_expr ::= xx_common_expr",
 /* 268 */ "xx_comment ::= COMMENT",
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
// 2349 "parser.c"
      break;
    case 101:
// 913 "parser.lemon"
{ json_object_put((yypminor->yy115)); }
// 2354 "parser.c"
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
// 2840 "parser.c"
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
      case 267:
// 915 "parser.lemon"
{
	yygotominor.yy115 = yymsp[0].minor.yy115;
}
// 2877 "parser.c"
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
// 2891 "parser.c"
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
// 2912 "parser.c"
        break;
      case 8:
// 943 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_namespace(yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(33,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 2921 "parser.c"
        break;
      case 9:
// 947 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_interface(yymsp[-1].minor.yy0, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(36,&yymsp[-2].minor);
}
// 2929 "parser.c"
        break;
      case 10:
// 951 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_interface(yymsp[-3].minor.yy0, yymsp[0].minor.yy115, yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(36,&yymsp[-4].minor);
  yy_destructor(37,&yymsp[-2].minor);
}
// 2938 "parser.c"
        break;
      case 11:
// 955 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class(yymsp[-1].minor.yy0, yymsp[0].minor.yy115, 0, 0, NULL, NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-2].minor);
}
// 2946 "parser.c"
        break;
      case 12:
// 959 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy115, 0, 0, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-4].minor);
  yy_destructor(37,&yymsp[-2].minor);
}
// 2955 "parser.c"
        break;
      case 13:
// 963 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy115, 0, 0, NULL, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(38,&yymsp[-4].minor);
  yy_destructor(39,&yymsp[-2].minor);
}
// 2964 "parser.c"
        break;
      case 14:
// 967 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class(yymsp[-5].minor.yy0, yymsp[0].minor.yy115, 0, 0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(38,&yymsp[-6].minor);
  yy_destructor(37,&yymsp[-4].minor);
  yy_destructor(39,&yymsp[-2].minor);
}
// 2974 "parser.c"
        break;
      case 15:
// 971 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class(yymsp[-1].minor.yy0, yymsp[0].minor.yy115, 1, 0, NULL, NULL, status->scanner_state);
  yy_destructor(40,&yymsp[-3].minor);
  yy_destructor(38,&yymsp[-2].minor);
}
// 2983 "parser.c"
        break;
      case 16:
// 975 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy115, 1, 0, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(40,&yymsp[-5].minor);
  yy_destructor(38,&yymsp[-4].minor);
  yy_destructor(37,&yymsp[-2].minor);
}
// 2993 "parser.c"
        break;
      case 17:
// 979 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy115, 1, 0, NULL, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(40,&yymsp[-5].minor);
  yy_destructor(38,&yymsp[-4].minor);
  yy_destructor(39,&yymsp[-2].minor);
}
// 3003 "parser.c"
        break;
      case 18:
// 983 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class(yymsp[-1].minor.yy0, yymsp[0].minor.yy115, 0, 1, NULL, NULL, status->scanner_state);
  yy_destructor(41,&yymsp[-3].minor);
  yy_destructor(38,&yymsp[-2].minor);
}
// 3012 "parser.c"
        break;
      case 19:
// 987 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy115, 0, 1, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(41,&yymsp[-5].minor);
  yy_destructor(38,&yymsp[-4].minor);
  yy_destructor(37,&yymsp[-2].minor);
}
// 3022 "parser.c"
        break;
      case 20:
      case 25:
// 991 "parser.lemon"
{
	yygotominor.yy115 = NULL;
  yy_destructor(42,&yymsp[-1].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3032 "parser.c"
        break;
      case 21:
      case 26:
// 995 "parser.lemon"
{
	yygotominor.yy115 = yymsp[-1].minor.yy115;
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3042 "parser.c"
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
// 3056 "parser.c"
        break;
      case 24:
      case 221:
      case 254:
// 1007 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state);
}
// 3065 "parser.c"
        break;
      case 27:
// 1019 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_definition(yymsp[0].minor.yy115, NULL, NULL, status->scanner_state);
}
// 3072 "parser.c"
        break;
      case 28:
// 1023 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_definition(NULL, NULL, yymsp[0].minor.yy115, status->scanner_state);
}
// 3079 "parser.c"
        break;
      case 29:
// 1027 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_definition(NULL, yymsp[0].minor.yy115, NULL, status->scanner_state);
}
// 3086 "parser.c"
        break;
      case 30:
// 1031 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_definition(yymsp[-1].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
}
// 3093 "parser.c"
        break;
      case 31:
// 1035 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_definition(yymsp[-1].minor.yy115, NULL, yymsp[0].minor.yy115, status->scanner_state);
}
// 3100 "parser.c"
        break;
      case 32:
// 1039 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_definition(yymsp[0].minor.yy115, NULL, yymsp[-1].minor.yy115, status->scanner_state);
}
// 3107 "parser.c"
        break;
      case 33:
// 1043 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_definition(NULL, yymsp[0].minor.yy115, yymsp[-1].minor.yy115, status->scanner_state);
}
// 3114 "parser.c"
        break;
      case 34:
// 1047 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_definition(yymsp[-2].minor.yy115, yymsp[0].minor.yy115, yymsp[-1].minor.yy115, status->scanner_state);
}
// 3121 "parser.c"
        break;
      case 35:
// 1051 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_definition(yymsp[-1].minor.yy115, yymsp[0].minor.yy115, yymsp[-2].minor.yy115, status->scanner_state);
}
// 3128 "parser.c"
        break;
      case 38:
// 1067 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_property(yymsp[-2].minor.yy115, yymsp[-1].minor.yy0, NULL, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(35,&yymsp[0].minor);
}
// 3136 "parser.c"
        break;
      case 39:
// 1071 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_property(yymsp[-2].minor.yy115, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(35,&yymsp[0].minor);
}
// 3144 "parser.c"
        break;
      case 40:
// 1075 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_property(yymsp[-4].minor.yy115, yymsp[-3].minor.yy0, yymsp[-1].minor.yy115, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(45,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3153 "parser.c"
        break;
      case 41:
// 1079 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_property(yymsp[-4].minor.yy115, yymsp[-3].minor.yy0, yymsp[-1].minor.yy115, NULL, status->scanner_state);
  yy_destructor(45,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3162 "parser.c"
        break;
      case 48:
// 1107 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy115, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(46,&yymsp[-4].minor);
  yy_destructor(45,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3172 "parser.c"
        break;
      case 49:
// 1111 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy115, NULL, status->scanner_state);
  yy_destructor(46,&yymsp[-4].minor);
  yy_destructor(45,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3182 "parser.c"
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
// 3194 "parser.c"
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
// 3206 "parser.c"
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
// 3218 "parser.c"
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
// 3230 "parser.c"
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
// 3242 "parser.c"
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
// 3254 "parser.c"
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
// 3266 "parser.c"
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
// 3278 "parser.c"
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
// 3289 "parser.c"
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
// 3300 "parser.c"
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
// 3311 "parser.c"
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
// 3322 "parser.c"
        break;
      case 64:
// 1171 "parser.lemon"
{
	yygotominor.yy115 = json_object_new_string("public");
  yy_destructor(1,&yymsp[0].minor);
}
// 3330 "parser.c"
        break;
      case 65:
// 1175 "parser.lemon"
{
	yygotominor.yy115 = json_object_new_string("protected");
  yy_destructor(2,&yymsp[0].minor);
}
// 3338 "parser.c"
        break;
      case 66:
// 1179 "parser.lemon"
{
	yygotominor.yy115 = json_object_new_string("private");
  yy_destructor(4,&yymsp[0].minor);
}
// 3346 "parser.c"
        break;
      case 67:
// 1183 "parser.lemon"
{
	yygotominor.yy115 = json_object_new_string("static");
  yy_destructor(3,&yymsp[0].minor);
}
// 3354 "parser.c"
        break;
      case 68:
// 1187 "parser.lemon"
{
	yygotominor.yy115 = json_object_new_string("scoped");
  yy_destructor(5,&yymsp[0].minor);
}
// 3362 "parser.c"
        break;
      case 69:
// 1191 "parser.lemon"
{
	yygotominor.yy115 = json_object_new_string("inline");
  yy_destructor(50,&yymsp[0].minor);
}
// 3370 "parser.c"
        break;
      case 70:
// 1195 "parser.lemon"
{
	yygotominor.yy115 = json_object_new_string("abstract");
  yy_destructor(40,&yymsp[0].minor);
}
// 3378 "parser.c"
        break;
      case 71:
// 1199 "parser.lemon"
{
	yygotominor.yy115 = json_object_new_string("final");
  yy_destructor(41,&yymsp[0].minor);
}
// 3386 "parser.c"
        break;
      case 74:
// 1211 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_parameter(NULL, NULL, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 3393 "parser.c"
        break;
      case 75:
// 1215 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_parameter(yymsp[-1].minor.yy115, NULL, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 3400 "parser.c"
        break;
      case 76:
// 1219 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_parameter(NULL, yymsp[-1].minor.yy115, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 3407 "parser.c"
        break;
      case 77:
// 1223 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_parameter(NULL, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy115, status->scanner_state);
  yy_destructor(45,&yymsp[-1].minor);
}
// 3415 "parser.c"
        break;
      case 78:
// 1227 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_parameter(yymsp[-3].minor.yy115, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy115, status->scanner_state);
  yy_destructor(45,&yymsp[-1].minor);
}
// 3423 "parser.c"
        break;
      case 79:
// 1231 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_parameter(NULL, yymsp[-3].minor.yy115, yymsp[-2].minor.yy0, yymsp[0].minor.yy115, status->scanner_state);
  yy_destructor(45,&yymsp[-1].minor);
}
// 3431 "parser.c"
        break;
      case 80:
// 1235 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(15,&yymsp[-2].minor);
  yy_destructor(16,&yymsp[0].minor);
}
// 3440 "parser.c"
        break;
      case 81:
// 1239 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_type(XX_TYPE_INTEGER);
  yy_destructor(51,&yymsp[0].minor);
}
// 3448 "parser.c"
        break;
      case 82:
// 1243 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_type(XX_TYPE_UINTEGER);
  yy_destructor(52,&yymsp[0].minor);
}
// 3456 "parser.c"
        break;
      case 83:
// 1247 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_type(XX_TYPE_LONG);
  yy_destructor(53,&yymsp[0].minor);
}
// 3464 "parser.c"
        break;
      case 84:
// 1251 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_type(XX_TYPE_ULONG);
  yy_destructor(54,&yymsp[0].minor);
}
// 3472 "parser.c"
        break;
      case 85:
// 1255 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_type(XX_TYPE_CHAR);
  yy_destructor(55,&yymsp[0].minor);
}
// 3480 "parser.c"
        break;
      case 86:
// 1259 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_type(XX_TYPE_UCHAR);
  yy_destructor(56,&yymsp[0].minor);
}
// 3488 "parser.c"
        break;
      case 87:
// 1263 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_type(XX_TYPE_DOUBLE);
  yy_destructor(57,&yymsp[0].minor);
}
// 3496 "parser.c"
        break;
      case 88:
// 1267 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_type(XX_TYPE_BOOL);
  yy_destructor(58,&yymsp[0].minor);
}
// 3504 "parser.c"
        break;
      case 89:
// 1271 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_type(XX_TYPE_STRING);
  yy_destructor(59,&yymsp[0].minor);
}
// 3512 "parser.c"
        break;
      case 90:
// 1275 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_type(XX_TYPE_ARRAY);
  yy_destructor(60,&yymsp[0].minor);
}
// 3520 "parser.c"
        break;
      case 91:
// 1279 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_type(XX_TYPE_VAR);
  yy_destructor(61,&yymsp[0].minor);
}
// 3528 "parser.c"
        break;
      case 112:
// 1363 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_break_statement(status->scanner_state);
  yy_destructor(62,&yymsp[-1].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3537 "parser.c"
        break;
      case 113:
// 1367 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_continue_statement(status->scanner_state);
  yy_destructor(63,&yymsp[-1].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3546 "parser.c"
        break;
      case 114:
// 1371 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_if_statement(yymsp[-2].minor.yy115, NULL, NULL, status->scanner_state);
  yy_destructor(64,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-1].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3556 "parser.c"
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
// 3569 "parser.c"
        break;
      case 116:
// 1379 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_if_statement(yymsp[-3].minor.yy115, yymsp[-1].minor.yy115, NULL, status->scanner_state);
  yy_destructor(64,&yymsp[-4].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3579 "parser.c"
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
// 3592 "parser.c"
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
// 3605 "parser.c"
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
// 3618 "parser.c"
        break;
      case 120:
// 1395 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_switch_statement(yymsp[-2].minor.yy115, NULL, status->scanner_state);
  yy_destructor(66,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-1].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3628 "parser.c"
        break;
      case 121:
// 1399 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_switch_statement(yymsp[-3].minor.yy115, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(66,&yymsp[-4].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3638 "parser.c"
        break;
      case 124:
// 1411 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_case_clause(yymsp[-1].minor.yy115, NULL, status->scanner_state);
  yy_destructor(67,&yymsp[-2].minor);
  yy_destructor(68,&yymsp[0].minor);
}
// 3647 "parser.c"
        break;
      case 125:
// 1415 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_case_clause(yymsp[-2].minor.yy115, yymsp[0].minor.yy115, status->scanner_state);
  yy_destructor(67,&yymsp[-3].minor);
  yy_destructor(68,&yymsp[-1].minor);
}
// 3656 "parser.c"
        break;
      case 126:
// 1419 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_case_clause(NULL, yymsp[0].minor.yy115, status->scanner_state);
  yy_destructor(69,&yymsp[-2].minor);
  yy_destructor(68,&yymsp[-1].minor);
}
// 3665 "parser.c"
        break;
      case 127:
// 1423 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_loop_statement(NULL, status->scanner_state);
  yy_destructor(70,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[-1].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3675 "parser.c"
        break;
      case 128:
// 1427 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_loop_statement(yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(70,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3685 "parser.c"
        break;
      case 129:
// 1431 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_while_statement(yymsp[-2].minor.yy115, NULL, status->scanner_state);
  yy_destructor(71,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-1].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3695 "parser.c"
        break;
      case 130:
// 1435 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_while_statement(yymsp[-3].minor.yy115, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(71,&yymsp[-4].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3705 "parser.c"
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
// 3717 "parser.c"
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
// 3729 "parser.c"
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
// 3740 "parser.c"
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
// 3752 "parser.c"
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
// 3764 "parser.c"
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
// 3777 "parser.c"
        break;
      case 137:
// 1463 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_let_statement(yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(76,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3786 "parser.c"
        break;
      case 140:
// 1476 "parser.lemon"
{
	yygotominor.yy115 = json_object_new_string("assign");
  yy_destructor(45,&yymsp[0].minor);
}
// 3794 "parser.c"
        break;
      case 141:
// 1481 "parser.lemon"
{
	yygotominor.yy115 = json_object_new_string("add-assign");
  yy_destructor(77,&yymsp[0].minor);
}
// 3802 "parser.c"
        break;
      case 142:
// 1486 "parser.lemon"
{
	yygotominor.yy115 = json_object_new_string("sub-assign");
  yy_destructor(78,&yymsp[0].minor);
}
// 3810 "parser.c"
        break;
      case 143:
// 1490 "parser.lemon"
{
	yygotominor.yy115 = json_object_new_string("mult-assign");
  yy_destructor(79,&yymsp[0].minor);
}
// 3818 "parser.c"
        break;
      case 144:
// 1494 "parser.lemon"
{
	yygotominor.yy115 = json_object_new_string("div-assign");
  yy_destructor(80,&yymsp[0].minor);
}
// 3826 "parser.c"
        break;
      case 145:
// 1498 "parser.lemon"
{
	yygotominor.yy115 = json_object_new_string("concat-assign");
  yy_destructor(81,&yymsp[0].minor);
}
// 3834 "parser.c"
        break;
      case 146:
// 1503 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_let_assignment("variable", yymsp[-1].minor.yy115, yymsp[-2].minor.yy0, NULL, NULL, yymsp[0].minor.yy115, status->scanner_state);
}
// 3841 "parser.c"
        break;
      case 147:
// 1508 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_let_assignment("object-property", yymsp[-1].minor.yy115, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, yymsp[0].minor.yy115, status->scanner_state);
  yy_destructor(82,&yymsp[-3].minor);
}
// 3849 "parser.c"
        break;
      case 148:
// 1513 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_let_assignment("variable-dynamic-object-property", yymsp[-1].minor.yy115, yymsp[-6].minor.yy0, yymsp[-3].minor.yy0, NULL, yymsp[0].minor.yy115, status->scanner_state);
  yy_destructor(82,&yymsp[-5].minor);
  yy_destructor(42,&yymsp[-4].minor);
  yy_destructor(43,&yymsp[-2].minor);
}
// 3859 "parser.c"
        break;
      case 149:
// 1518 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_let_assignment("string-dynamic-object-property", yymsp[-1].minor.yy115, yymsp[-6].minor.yy0, yymsp[-3].minor.yy0, NULL, yymsp[0].minor.yy115, status->scanner_state);
  yy_destructor(82,&yymsp[-5].minor);
  yy_destructor(42,&yymsp[-4].minor);
  yy_destructor(43,&yymsp[-2].minor);
}
// 3869 "parser.c"
        break;
      case 150:
// 1523 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_let_assignment("object-property-append", yymsp[-1].minor.yy115, yymsp[-6].minor.yy0, yymsp[-4].minor.yy0, NULL, yymsp[0].minor.yy115, status->scanner_state);
  yy_destructor(82,&yymsp[-5].minor);
  yy_destructor(84,&yymsp[-3].minor);
  yy_destructor(85,&yymsp[-2].minor);
}
// 3879 "parser.c"
        break;
      case 151:
// 1528 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_let_assignment("object-property-array-index", yymsp[-1].minor.yy115, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, yymsp[-3].minor.yy115, yymsp[0].minor.yy115, status->scanner_state);
  yy_destructor(82,&yymsp[-6].minor);
  yy_destructor(84,&yymsp[-4].minor);
  yy_destructor(85,&yymsp[-2].minor);
}
// 3889 "parser.c"
        break;
      case 152:
// 1533 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_let_assignment("static-property", yymsp[-1].minor.yy115, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, yymsp[0].minor.yy115, status->scanner_state);
  yy_destructor(86,&yymsp[-3].minor);
}
// 3897 "parser.c"
        break;
      case 153:
// 1538 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_let_assignment("static-property-append", yymsp[-1].minor.yy115, yymsp[-6].minor.yy0, yymsp[-4].minor.yy0, NULL, yymsp[0].minor.yy115, status->scanner_state);
  yy_destructor(86,&yymsp[-5].minor);
  yy_destructor(84,&yymsp[-3].minor);
  yy_destructor(85,&yymsp[-2].minor);
}
// 3907 "parser.c"
        break;
      case 154:
// 1543 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_let_assignment("static-property-array-index", yymsp[-1].minor.yy115, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, yymsp[-3].minor.yy115, yymsp[0].minor.yy115, status->scanner_state);
  yy_destructor(86,&yymsp[-6].minor);
  yy_destructor(84,&yymsp[-4].minor);
  yy_destructor(85,&yymsp[-2].minor);
}
// 3917 "parser.c"
        break;
      case 155:
// 1548 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_let_assignment("variable-append", yymsp[-1].minor.yy115, yymsp[-4].minor.yy0, NULL, NULL, yymsp[0].minor.yy115, status->scanner_state);
  yy_destructor(84,&yymsp[-3].minor);
  yy_destructor(85,&yymsp[-2].minor);
}
// 3926 "parser.c"
        break;
      case 156:
// 1553 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_let_assignment("array-index", yymsp[-1].minor.yy115, yymsp[-5].minor.yy0, NULL, yymsp[-3].minor.yy115, yymsp[0].minor.yy115, status->scanner_state);
  yy_destructor(84,&yymsp[-4].minor);
  yy_destructor(85,&yymsp[-2].minor);
}
// 3935 "parser.c"
        break;
      case 157:
// 1558 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_let_assignment("incr", NULL, yymsp[-3].minor.yy0, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(82,&yymsp[-2].minor);
  yy_destructor(87,&yymsp[0].minor);
}
// 3944 "parser.c"
        break;
      case 158:
// 1563 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_let_assignment("decr", NULL, yymsp[-3].minor.yy0, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(82,&yymsp[-2].minor);
  yy_destructor(88,&yymsp[0].minor);
}
// 3953 "parser.c"
        break;
      case 159:
// 1568 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_let_assignment("incr", NULL, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(87,&yymsp[0].minor);
}
// 3961 "parser.c"
        break;
      case 160:
// 1573 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_let_assignment("decr", NULL, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(88,&yymsp[0].minor);
}
// 3969 "parser.c"
        break;
      case 162:
// 1581 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_echo_statement(yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(89,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3978 "parser.c"
        break;
      case 165:
// 1593 "parser.lemon"
{
	yygotominor.yy115 = yymsp[0].minor.yy115;;
}
// 3985 "parser.c"
        break;
      case 166:
// 1598 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_mcall_statement(yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(35,&yymsp[0].minor);
}
// 3993 "parser.c"
        break;
      case 167:
// 1603 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_fcall_statement(yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(35,&yymsp[0].minor);
}
// 4001 "parser.c"
        break;
      case 168:
// 1608 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_scall_statement(yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(35,&yymsp[0].minor);
}
// 4009 "parser.c"
        break;
      case 169:
// 1613 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_return_statement(yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(90,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4018 "parser.c"
        break;
      case 170:
// 1618 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_return_statement(NULL, status->scanner_state);
  yy_destructor(90,&yymsp[-1].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4027 "parser.c"
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
// 4041 "parser.c"
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
// 4056 "parser.c"
        break;
      case 173:
// 1641 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_throw_exception(yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(92,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4065 "parser.c"
        break;
      case 174:
// 1645 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_declare_statement(XX_T_TYPE_INTEGER, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(51,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4074 "parser.c"
        break;
      case 175:
// 1649 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_declare_statement(XX_T_TYPE_UINTEGER, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(52,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4083 "parser.c"
        break;
      case 176:
// 1653 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_declare_statement(XX_T_TYPE_CHAR, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(55,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4092 "parser.c"
        break;
      case 177:
// 1657 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_declare_statement(XX_T_TYPE_UCHAR, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(56,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4101 "parser.c"
        break;
      case 178:
// 1661 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_declare_statement(XX_T_TYPE_LONG, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(53,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4110 "parser.c"
        break;
      case 179:
// 1665 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_declare_statement(XX_T_TYPE_ULONG, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4119 "parser.c"
        break;
      case 180:
// 1669 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_declare_statement(XX_T_TYPE_DOUBLE, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(57,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4128 "parser.c"
        break;
      case 181:
// 1673 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_declare_statement(XX_T_TYPE_STRING, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(59,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4137 "parser.c"
        break;
      case 182:
// 1677 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_declare_statement(XX_T_TYPE_BOOL, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(58,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4146 "parser.c"
        break;
      case 183:
// 1681 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_declare_statement(XX_T_TYPE_VAR, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(61,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4155 "parser.c"
        break;
      case 186:
// 1693 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_declare_variable(yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 4162 "parser.c"
        break;
      case 187:
// 1697 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_declare_variable(yymsp[-2].minor.yy0, yymsp[0].minor.yy115, status->scanner_state);
  yy_destructor(45,&yymsp[-1].minor);
}
// 4170 "parser.c"
        break;
      case 189:
// 1705 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("not", yymsp[0].minor.yy115, NULL, NULL, status->scanner_state);
  yy_destructor(31,&yymsp[-1].minor);
}
// 4178 "parser.c"
        break;
      case 190:
// 1709 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("isset", yymsp[0].minor.yy115, NULL, NULL, status->scanner_state);
  yy_destructor(27,&yymsp[-1].minor);
}
// 4186 "parser.c"
        break;
      case 191:
// 1713 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("require", yymsp[0].minor.yy115, NULL, NULL, status->scanner_state);
  yy_destructor(7,&yymsp[-1].minor);
}
// 4194 "parser.c"
        break;
      case 192:
// 1717 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("clone", yymsp[0].minor.yy115, NULL, NULL, status->scanner_state);
  yy_destructor(8,&yymsp[-1].minor);
}
// 4202 "parser.c"
        break;
      case 193:
// 1721 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("empty", yymsp[0].minor.yy115, NULL, NULL, status->scanner_state);
  yy_destructor(9,&yymsp[-1].minor);
}
// 4210 "parser.c"
        break;
      case 194:
// 1725 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("equals", yymsp[-2].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(13,&yymsp[-1].minor);
}
// 4218 "parser.c"
        break;
      case 195:
// 1729 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("not-equals", yymsp[-2].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(20,&yymsp[-1].minor);
}
// 4226 "parser.c"
        break;
      case 196:
// 1733 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("identical", yymsp[-2].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(14,&yymsp[-1].minor);
}
// 4234 "parser.c"
        break;
      case 197:
// 1737 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("not-identical", yymsp[-2].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(19,&yymsp[-1].minor);
}
// 4242 "parser.c"
        break;
      case 198:
// 1741 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("less", yymsp[-2].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(15,&yymsp[-1].minor);
}
// 4250 "parser.c"
        break;
      case 199:
// 1745 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("greater", yymsp[-2].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(16,&yymsp[-1].minor);
}
// 4258 "parser.c"
        break;
      case 200:
// 1749 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("less-equal", yymsp[-2].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(17,&yymsp[-1].minor);
}
// 4266 "parser.c"
        break;
      case 201:
// 1753 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("greater-equal", yymsp[-2].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(18,&yymsp[-1].minor);
}
// 4274 "parser.c"
        break;
      case 202:
// 1757 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("list", yymsp[-1].minor.yy115, NULL, NULL, status->scanner_state);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 4283 "parser.c"
        break;
      case 203:
// 1761 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("cast", yymsp[-2].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(49,&yymsp[-3].minor);
  yy_destructor(32,&yymsp[-1].minor);
}
// 4292 "parser.c"
        break;
      case 204:
// 1765 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("type-hint", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(15,&yymsp[-3].minor);
  yy_destructor(16,&yymsp[-1].minor);
}
// 4301 "parser.c"
        break;
      case 205:
      case 266:
// 1769 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("property-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(82,&yymsp[-1].minor);
}
// 4310 "parser.c"
        break;
      case 206:
// 1773 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("static-property-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(86,&yymsp[-1].minor);
}
// 4318 "parser.c"
        break;
      case 207:
// 1777 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("static-constant-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(86,&yymsp[-1].minor);
}
// 4326 "parser.c"
        break;
      case 208:
      case 265:
// 1782 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), yymsp[-1].minor.yy115, NULL, status->scanner_state);
  yy_destructor(84,&yymsp[-2].minor);
  yy_destructor(85,&yymsp[0].minor);
}
// 4336 "parser.c"
        break;
      case 209:
// 1787 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("add", yymsp[-2].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(21,&yymsp[-1].minor);
}
// 4344 "parser.c"
        break;
      case 210:
// 1792 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("sub", yymsp[-2].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(22,&yymsp[-1].minor);
}
// 4352 "parser.c"
        break;
      case 211:
// 1797 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("mul", yymsp[-2].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(24,&yymsp[-1].minor);
}
// 4360 "parser.c"
        break;
      case 212:
// 1802 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("div", yymsp[-2].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(25,&yymsp[-1].minor);
}
// 4368 "parser.c"
        break;
      case 213:
// 1807 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("mod", yymsp[-2].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(26,&yymsp[-1].minor);
}
// 4376 "parser.c"
        break;
      case 214:
// 1812 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("concat", yymsp[-2].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(23,&yymsp[-1].minor);
}
// 4384 "parser.c"
        break;
      case 215:
// 1817 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("and", yymsp[-2].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(11,&yymsp[-1].minor);
}
// 4392 "parser.c"
        break;
      case 216:
// 1822 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("or", yymsp[-2].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(12,&yymsp[-1].minor);
}
// 4400 "parser.c"
        break;
      case 217:
// 1827 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("instanceof", yymsp[-2].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(10,&yymsp[-1].minor);
}
// 4408 "parser.c"
        break;
      case 218:
// 1832 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("fetch", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(28,&yymsp[-3].minor);
  yy_destructor(6,&yymsp[-1].minor);
}
// 4417 "parser.c"
        break;
      case 219:
// 1837 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("typeof", yymsp[0].minor.yy115, NULL, NULL, status->scanner_state);
  yy_destructor(29,&yymsp[-1].minor);
}
// 4425 "parser.c"
        break;
      case 220:
// 1842 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_literal(XX_T_CONSTANT, yymsp[0].minor.yy0, status->scanner_state);
}
// 4432 "parser.c"
        break;
      case 222:
      case 256:
      case 258:
// 1852 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_literal(XX_T_INTEGER, yymsp[0].minor.yy0, status->scanner_state);
}
// 4441 "parser.c"
        break;
      case 223:
      case 255:
      case 260:
// 1857 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_literal(XX_T_STRING, yymsp[0].minor.yy0, status->scanner_state);
}
// 4450 "parser.c"
        break;
      case 224:
      case 259:
// 1862 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_literal(XX_T_CHAR, yymsp[0].minor.yy0, status->scanner_state);
}
// 4458 "parser.c"
        break;
      case 225:
      case 261:
// 1867 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_literal(XX_T_DOUBLE, yymsp[0].minor.yy0, status->scanner_state);
}
// 4466 "parser.c"
        break;
      case 226:
      case 262:
// 1872 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_literal(XX_T_NULL, NULL, status->scanner_state);
  yy_destructor(96,&yymsp[0].minor);
}
// 4475 "parser.c"
        break;
      case 227:
      case 264:
// 1877 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_literal(XX_T_TRUE, NULL, status->scanner_state);
  yy_destructor(97,&yymsp[0].minor);
}
// 4484 "parser.c"
        break;
      case 228:
      case 263:
// 1882 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_literal(XX_T_FALSE, NULL, status->scanner_state);
  yy_destructor(98,&yymsp[0].minor);
}
// 4493 "parser.c"
        break;
      case 229:
// 1887 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("empty-array", NULL, NULL, NULL, status->scanner_state);
  yy_destructor(84,&yymsp[-1].minor);
  yy_destructor(85,&yymsp[0].minor);
}
// 4502 "parser.c"
        break;
      case 230:
// 1892 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("array", yymsp[-1].minor.yy115, NULL, NULL, status->scanner_state);
  yy_destructor(84,&yymsp[-2].minor);
  yy_destructor(85,&yymsp[0].minor);
}
// 4511 "parser.c"
        break;
      case 231:
// 1897 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_new_instance(yymsp[0].minor.yy0, NULL, status->scanner_state);
  yy_destructor(30,&yymsp[-1].minor);
}
// 4519 "parser.c"
        break;
      case 232:
// 1902 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_new_instance(yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(30,&yymsp[-3].minor);
  yy_destructor(49,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 4529 "parser.c"
        break;
      case 233:
// 1907 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_new_instance(yymsp[-3].minor.yy0, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(30,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 4539 "parser.c"
        break;
      case 234:
// 1912 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_fcall(yymsp[-3].minor.yy0, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 4548 "parser.c"
        break;
      case 235:
// 1917 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_fcall(yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(49,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 4557 "parser.c"
        break;
      case 236:
// 1922 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_scall(yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(86,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 4567 "parser.c"
        break;
      case 237:
// 1927 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_scall(yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(86,&yymsp[-3].minor);
  yy_destructor(49,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 4577 "parser.c"
        break;
      case 238:
// 1932 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_mcall(1, yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(82,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 4587 "parser.c"
        break;
      case 239:
// 1937 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_mcall(1, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(82,&yymsp[-3].minor);
  yy_destructor(49,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 4597 "parser.c"
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
// 4609 "parser.c"
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
// 4621 "parser.c"
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
// 4633 "parser.c"
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
// 4645 "parser.c"
        break;
      case 252:
// 1996 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_array_item(yymsp[-2].minor.yy115, yymsp[0].minor.yy115, status->scanner_state);
  yy_destructor(68,&yymsp[-1].minor);
}
// 4653 "parser.c"
        break;
      case 253:
// 2000 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_array_item(NULL, yymsp[0].minor.yy115, status->scanner_state);
}
// 4660 "parser.c"
        break;
      case 268:
// 2061 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_comment(yymsp[0].minor.yy0, status->scanner_state);
}
// 4667 "parser.c"
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

// 4743 "parser.c"
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
