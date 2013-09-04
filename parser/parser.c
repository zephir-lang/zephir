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

	json_object_object_add(ret, "type", json_object_new_string("case"));

	if (expr) {
		json_object_object_add(ret, "expr", expr);
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


// 823 "parser.c"
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
#define YYNSTATE 536
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
 /*     0 */   175,  170,  179,  183,  185,  187,  189,  181,  177,  191,
 /*    10 */   193,  201,  195,  197,  199,  246,  457,  171,  220,  224,
 /*    20 */   218,  250,  168,   95,  526,  213,   97,  507,  513,  512,
 /*    30 */   500,  455,  506,  480,   65,   43,   47,   85,  226,  515,
 /*    40 */   207,  520,   54,   55,   56,   57,   58,   59,   60,   61,
 /*    50 */    62,   63,   64,  203,  205,  170,  179,  183,  185,  187,
 /*    60 */   189,  181,  177,  191,  193,  201,  195,  197,  199,   32,
 /*    70 */    43,   47,  248,  247,  234,   37,   39,   40,   42,   41,
 /*    80 */   167,  414,  227,  228,  266,  229,  230,  231,  232,  233,
 /*    90 */   106,  419,   67,   68,   70,   69,   71,  271,  378,  387,
 /*   100 */   396,  399,  390,  393,  402,  408,  405,  256,  411,  128,
 /*   110 */   130,  132,   25,  141,   31,   76,   19,  152,  156,  161,
 /*   120 */   283,   22,   85,  290,  414,   86,   31,  693,   85,   73,
 /*   130 */    74,  269,  238,  136,  419,  343,  356,  360,   29,   72,
 /*   140 */   375,  378,  387,  396,  399,  390,  393,  402,  408,  405,
 /*   150 */    83,  411,  128,  130,  132,  173,  141,  195,  197,  199,
 /*   160 */   152,  156,  161,  283,  223,  275,  290,  414,   36,   38,
 /*   170 */   781,   37,   39,   40,   42,   41,  468,  419,  343,  356,
 /*   180 */   360,  145,  447,  375,  378,  387,  396,  399,  390,  393,
 /*   190 */   402,  408,  405,  366,  411,  128,  130,  132,  417,  141,
 /*   200 */    48,  415,   14,  152,  156,  161,  283,   17,  237,  290,
 /*   210 */   414,  191,  193,  201,  195,  197,  199,  144,  426,  289,
 /*   220 */   419,  343,  356,  360,  296,   66,  375,  378,  387,  396,
 /*   230 */   399,  390,  393,  402,  408,  405,  243,  411,  128,  130,
 /*   240 */   132,  148,  141,  444,  256,   93,  152,  156,  161,  283,
 /*   250 */   369,  372,  290,  414,  462,  256,  297,  298,  299,  300,
 /*   260 */   301,   85,  140,  419,  343,  356,  360,  293,  255,  375,
 /*   270 */   378,  387,  396,  399,  390,  393,  402,  408,  405,  263,
 /*   280 */   411,  128,  130,  132,  285,  141,  242,  256,  517,  152,
 /*   290 */   156,  161,  283,  348,  292,  290,  414,  291,  342,  344,
 /*   300 */   349,  257,  259,  258,  346,  454,  419,  343,  356,  360,
 /*   310 */    24,  277,  375,  378,  387,  396,  399,  390,  393,  402,
 /*   320 */   408,  405,   93,  411,  128,  130,  132,  383,  141,  379,
 /*   330 */   386,  345,  152,  156,  161,  283,  348,   28,  290,  414,
 /*   340 */    65,   43,   47,  347,  257,  259,  258,  381,  108,  419,
 /*   350 */   343,  356,  360,  362,  365,  375,  378,  387,  396,  399,
 /*   360 */   390,  393,  402,  408,  405,  531,  411,  128,  130,  132,
 /*   370 */   381,  141,  388,  386,  380,  152,  156,  161,  283,  304,
 /*   380 */   464,  290,  414,  316,  391,  386,  381,  257,  259,  258,
 /*   390 */   381,  453,  419,  343,  356,  360,  381,  389,  375,  378,
 /*   400 */   387,  396,  399,  390,  393,  402,  408,  405,  527,  411,
 /*   410 */   128,  130,  132,  392,  141,  394,  386,  395,  152,  156,
 /*   420 */   161,  283,  304,  398,  290,  414,  312,  397,  386,  381,
 /*   430 */   257,  259,  258,  381,  476,  419,  343,  356,  360,  400,
 /*   440 */   386,  375,  378,  387,  396,  399,  390,  393,  402,  408,
 /*   450 */   405,  326,  411,  128,  130,  132,  401,  141,  403,  386,
 /*   460 */   404,  152,  156,  161,  283,  317,  525,  290,  414,  100,
 /*   470 */   313,   93,   99,  257,  259,  258,   85,  425,  419,  343,
 /*   480 */   356,  360,  381,  782,  375,  378,  387,  396,  399,  390,
 /*   490 */   393,  402,  408,  405,  467,  411,  128,  130,  132,  381,
 /*   500 */   141,  406,  386,  465,  152,  156,  161,  283,  304,  407,
 /*   510 */   290,  414,  308,  409,  386,  381,  257,  259,  258,  103,
 /*   520 */   474,  419,  343,  356,  360,  783,  410,  375,  378,  387,
 /*   530 */   396,  399,  390,  393,  402,  408,  405,  165,  411,  128,
 /*   540 */   130,  132,  413,  141,  412,  386,  361,  152,  156,  161,
 /*   550 */   283,  304,  243,  290,  414,  303,   31,  329,  496,  257,
 /*   560 */   259,  258,  494,  470,  419,  343,  356,  360,  497,   85,
 /*   570 */   375,  378,  387,  396,  399,  390,  393,  402,  408,  405,
 /*   580 */   456,  411,  128,  130,  132,  174,  141,  529,   31,  502,
 /*   590 */   152,  156,  161,  283,  363,  217,  290,  414,  243,  503,
 /*   600 */   239,  240,  245,  257,  259,  258,  458,  419,  343,  356,
 /*   610 */   360,  260,  472,  375,  378,  387,  396,  399,  390,  393,
 /*   620 */   402,  408,  405,  364,  411,  128,  130,  132,  330,  141,
 /*   630 */    16,  473,   17,  152,  156,  161,  283,   22,  530,  290,
 /*   640 */   414,   21,  284,  236,   85,  215,  249,  240,  245,  460,
 /*   650 */   419,  343,  356,  360,   15,  306,  375,  378,  387,  396,
 /*   660 */   399,  390,  393,  402,  408,  405,   44,  411,  128,  130,
 /*   670 */   132,  518,  141,  251,   13,  416,  152,  156,  161,  283,
 /*   680 */   304,  367,  290,  414,  323,  267,  307,  418,  257,  259,
 /*   690 */   258,   12,  449,  419,  343,  356,  360,  368,  483,  375,
 /*   700 */   378,  387,  396,  399,  390,  393,  402,  408,  405,  216,
 /*   710 */   411,  128,  130,  132,  484,  141,  486,  334,  485,  152,
 /*   720 */   156,  161,  283,  304,  532,  290,  414,  339,  614,   93,
 /*   730 */   370,  257,  259,  258,   92,  443,  419,  343,  356,  360,
 /*   740 */   488,  371,  375,  378,  387,  396,  399,  390,  393,  402,
 /*   750 */   408,  405,  489,  411,  128,  130,  132,  252,  141,   79,
 /*   760 */   491,  490,  152,  156,  161,  283,  304,  337,  290,  414,
 /*   770 */   335,  374,  373,   53,  257,  259,  258,  338,  432,  419,
 /*   780 */   343,  356,  360,  533,  427,  375,  378,  387,  396,  399,
 /*   790 */   390,  393,  402,  408,  405,   10,  411,  128,  130,  132,
 /*   800 */   428,  141,   52,   46,  498,  152,  156,  161,  283,  317,
 /*   810 */   499,  290,  414,   89,  336,   45,  311,  257,  259,  258,
 /*   820 */     9,  155,  419,  343,  356,  360,   84,  102,  375,  378,
 /*   830 */   387,  396,  399,  390,  393,  402,  408,  405,  516,  411,
 /*   840 */   128,  130,  132,  504,  141,  505,  295,  314,  152,  156,
 /*   850 */   161,  283,  304,   88,  290,  414,  331,  382,   35,   34,
 /*   860 */   257,  259,  258,  241,  437,  419,  343,  356,  360,  315,
 /*   870 */   384,  375,  378,  387,  396,  399,  390,  393,  402,  408,
 /*   880 */   405,   78,  411,  128,  130,  132,  534,  141,  385,  439,
 /*   890 */   105,  152,  156,  161,  283,   50,  286,  290,  414,    3,
 /*   900 */     4,    5,    6,    7,  257,  259,  258,  442,  419,  343,
 /*   910 */   356,  360,  536,  441,  375,  378,  387,  396,  399,  390,
 /*   920 */   393,  402,  408,  405,  143,  411,  128,  130,  132,  519,
 /*   930 */   141,  294,  522,   81,  152,  156,  161,  283,  304,  147,
 /*   940 */   290,  414,  327,  172,   49,  445,  257,  259,  258,   96,
 /*   950 */   448,  419,  343,  356,  360,  138,  149,  375,  378,  387,
 /*   960 */   396,  399,  390,  393,  402,  408,  405,  523,  411,  128,
 /*   970 */   130,  132,  150,  141,   30,  302,  137,  152,  156,  161,
 /*   980 */   283,  317,  134,  290,  414,  153,  328,  211,  351,  257,
 /*   990 */   259,  258,  321,  160,  419,  343,  356,  360,  131,  450,
 /*  1000 */   375,  378,  387,  396,  399,  390,  393,  402,  408,  405,
 /*  1010 */   524,  411,  128,  130,  132,  451,  141,  158,  353,  129,
 /*  1020 */   152,  156,  161,  283,  178,  471,  290,  414,   65,   43,
 /*  1030 */    47,   27,  257,  259,  258,  221,  438,  419,  343,  356,
 /*  1040 */   360,  322,  162,  375,  378,  387,  396,  399,  390,  393,
 /*  1050 */   402,  408,  405,   91,  411,  128,  130,  132,  355,  141,
 /*  1060 */   222,  430,  430,  152,  156,  161,  283,  281,   82,  290,
 /*  1070 */   414,   65,   43,   47,  430,  257,  259,  258,  430,  164,
 /*  1080 */   419,  343,  356,  360,  275,  430,  375,  378,  387,  396,
 /*  1090 */   399,  390,  393,  402,  408,  405,  430,  411,  128,  130,
 /*  1100 */   132,  430,  141,  430,  430,  430,  152,  156,  161,  283,
 /*  1110 */   430,  430,  290,  414,  430,  430,  430,  214,  272,   51,
 /*  1120 */   265,  430,  430,  419,  343,  356,  360,  430,  430,  375,
 /*  1130 */   378,  387,  396,  399,  390,  393,  402,  408,  405,   33,
 /*  1140 */   411,  128,  130,  132,  430,  141,  430,  430,  430,  152,
 /*  1150 */   156,  161,  283,  127,  430,  290,   54,   55,   56,   57,
 /*  1160 */    58,   59,   60,   61,   62,   63,   64,  343,  356,  360,
 /*  1170 */   431,  430,  375,  430,  430,  420,  110,  111,  112,  113,
 /*  1180 */   114,  115,  116,  117,  118,  119,  120,  121,  122,  123,
 /*  1190 */   124,  125,  126,  127,  797,    1,    2,  535,    4,    5,
 /*  1200 */     6,    7,  430,  202,  350,  352,  354,  430,  430,  430,
 /*  1210 */   151,  257,  259,  258,  430,  420,  110,  111,  112,  113,
 /*  1220 */   114,  115,  116,  117,  118,  119,  120,  121,  122,  123,
 /*  1230 */   124,  125,  126,  430,  478,  127,  100,  430,  479,  480,
 /*  1240 */   430,  430,  430,   85,  350,  352,  354,   67,   68,   70,
 /*  1250 */    69,   71,  135,  146,  430,  430,  430,  420,  110,  111,
 /*  1260 */   112,  113,  114,  115,  116,  117,  118,  119,  120,  121,
 /*  1270 */   122,  123,  124,  125,  126,  127,  430,  148,  430,  444,
 /*  1280 */   430,  430,  430,  430,   73,   74,  350,  352,  354,  430,
 /*  1290 */   482,  430,  154,  430,   72,  430,  430,  420,  110,  111,
 /*  1300 */   112,  113,  114,  115,  116,  117,  118,  119,  120,  121,
 /*  1310 */   122,  123,  124,  125,  126,  430,  127,  430,  508,  430,
 /*  1320 */   511,  512,  500,  430,  479,  480,  350,  352,  354,   85,
 /*  1330 */    90,  430,  514,  159,  430,   93,  430,  430,  420,  110,
 /*  1340 */   111,  112,  113,  114,  115,  116,  117,  118,  119,  120,
 /*  1350 */   121,  122,  123,  124,  125,  126,  430,  430,  127,  430,
 /*  1360 */   430,  166,  430,  430,  282,  430,  430,  350,  352,  354,
 /*  1370 */   296,  430,  257,  259,  258,  163,  430,  430,  430,  430,
 /*  1380 */   420,  110,  111,  112,  113,  114,  115,  116,  117,  118,
 /*  1390 */   119,  120,  121,  122,  123,  124,  125,  126,  127,  430,
 /*  1400 */   430,  430,  297,  298,  299,  300,  301,  430,  324,  350,
 /*  1410 */   352,  354,  430,  430,  430,  475,  430,  430,  430,  430,
 /*  1420 */   420,  110,  111,  112,  113,  114,  115,  116,  117,  118,
 /*  1430 */   119,  120,  121,  122,  123,  124,  125,  126,  430,  127,
 /*  1440 */   430,  430,  477,   98,  492,  500,  430,  506,  480,  350,
 /*  1450 */   352,  354,   85,  430,  430,  430,  107,  430,  430,  430,
 /*  1460 */   430,  420,  110,  111,  112,  113,  114,  115,  116,  117,
 /*  1470 */   118,  119,  120,  121,  122,  123,  124,  125,  126,  430,
 /*  1480 */   430,  127,  430,  430,  440,  430,  430,  282,  430,  430,
 /*  1490 */   350,  352,  354,  430,  430,  257,  259,  258,  469,  430,
 /*  1500 */   430,  430,  430,  420,  110,  111,  112,  113,  114,  115,
 /*  1510 */   116,  117,  118,  119,  120,  121,  122,  123,  124,  125,
 /*  1520 */   126,  127,  430,  430,  430,  430,  430,  430,  157,  430,
 /*  1530 */   430,  282,  350,  352,  354,  430,  430,  430,  459,  257,
 /*  1540 */   259,  258,  430,  420,  110,  111,  112,  113,  114,  115,
 /*  1550 */   116,  117,  118,  119,  120,  121,  122,  123,  124,  125,
 /*  1560 */   126,  430,  127,  430,  430,  430,  509,  492,  500,  430,
 /*  1570 */   430,  480,  350,  352,  354,   85,  430,  430,  430,  288,
 /*  1580 */   430,  430,  430,  430,  420,  110,  111,  112,  113,  114,
 /*  1590 */   115,  116,  117,  118,  119,  120,  121,  122,  123,  124,
 /*  1600 */   125,  126,  430,  430,  127,  430,  430,  142,  430,  430,
 /*  1610 */   282,  430,  430,  350,  352,  354,  430,  430,  257,  259,
 /*  1620 */   258,  452,  430,  430,  430,  430,  420,  110,  111,  112,
 /*  1630 */   113,  114,  115,  116,  117,  118,  119,  120,  121,  122,
 /*  1640 */   123,  124,  125,  126,  127,  430,  430,  430,  430,  430,
 /*  1650 */   430,  133,  430,  430,  282,  350,  352,  354,  430,  430,
 /*  1660 */   430,  446,  257,  259,  258,  430,  420,  110,  111,  112,
 /*  1670 */   113,  114,  115,  116,  117,  118,  119,  120,  121,  122,
 /*  1680 */   123,  124,  125,  126,  430,  127,  430,  430,  430,  430,
 /*  1690 */   192,  430,  430,  430,  430,  350,  352,  354,  257,  259,
 /*  1700 */   258,  430,  139,  430,  430,  430,  430,  420,  110,  111,
 /*  1710 */   112,  113,  114,  115,  116,  117,  118,  119,  120,  121,
 /*  1720 */   122,  123,  124,  125,  126,  430,  430,  127,  430,  430,
 /*  1730 */   430,  430,  430,  190,  430,  430,  350,  352,  354,  430,
 /*  1740 */   430,  257,  259,  258,  436,  430,  430,  430,  430,  420,
 /*  1750 */   110,  111,  112,  113,  114,  115,  116,  117,  118,  119,
 /*  1760 */   120,  121,  122,  123,  124,  125,  126,  127,  430,  430,
 /*  1770 */   430,  430,  430,  430,  430,  430,  430,  429,  350,  352,
 /*  1780 */   354,  430,  430,  430,  424,  257,  259,  258,  430,  420,
 /*  1790 */   110,  111,  112,  113,  114,  115,  116,  117,  118,  119,
 /*  1800 */   120,  121,  122,  123,  124,  125,  126,  430,  127,  430,
 /*  1810 */   430,  430,  430,  196,  430,  430,  188,  430,  350,  352,
 /*  1820 */   354,  257,  259,  258,  257,  259,  258,  430,  430,  430,
 /*  1830 */   109,  110,  111,  112,  113,  114,  115,  116,  117,  118,
 /*  1840 */   119,  120,  121,  122,  123,  124,  125,  126,  175,  430,
 /*  1850 */   430,  430,  430,  430,  430,  430,  430,  430,  430,  350,
 /*  1860 */   352,  354,  430,  430,  430,  171,  220,  224,  218,  250,
 /*  1870 */   168,  264,  430,  213,  175,  430,    8,  430,  430,   11,
 /*  1880 */   430,   87,  430,  521,  528,  430,  226,  419,  207,  430,
 /*  1890 */   430,  171,  220,  224,  218,  250,  168,  430,  430,  213,
 /*  1900 */   217,  430,  430,  430,  430,  430,  430,  175,  257,  259,
 /*  1910 */   258,  430,  226,  430,  207,  268,  261,  430,  430,  210,
 /*  1920 */   430,  430,  234,  430,  171,  220,  224,  218,  250,  168,
 /*  1930 */   227,  228,  213,  229,  230,  231,  232,  233,  430,  430,
 /*  1940 */   430,  430,  208,  430,  430,  226,  430,  207,  234,  310,
 /*  1950 */   257,  259,  258,  430,  430,  430,  227,  228,  430,  229,
 /*  1960 */   230,  231,  232,  233,  430,  203,  205,  170,  179,  183,
 /*  1970 */   185,  187,  189,  181,  177,  191,  193,  201,  195,  197,
 /*  1980 */   199,  234,  325,  430,  430,  430,  430,   51,  430,  227,
 /*  1990 */   228,  194,  229,  230,  231,  232,  233,  175,  430,  257,
 /*  2000 */   259,  258,  430,  430,  430,  466,  430,   33,  430,   67,
 /*  2010 */    68,   70,   69,   71,  171,  220,  224,  218,  250,  168,
 /*  2020 */   430,  430,  213,  175,   54,   55,   56,   57,   58,   59,
 /*  2030 */    60,   61,   62,   63,   64,  226,  430,  207,  430,  274,
 /*  2040 */   171,  220,  224,  218,  250,  168,   73,   74,  213,   94,
 /*  2050 */   493,  430,  487,  430,  430,  430,   72,  246,  296,  217,
 /*  2060 */   430,  226,  430,  207,  430,  244,  430,  257,  259,  258,
 /*  2070 */   430,  234,  333,  430,  276,  261,  430,  430,  430,  227,
 /*  2080 */   228,  430,  229,  230,  231,  232,  233,  430,  430,  433,
 /*  2090 */   297,  298,  299,  300,  301,  305,  332,  234,  320,  340,
 /*  2100 */   341,  430,  430,  430,  430,  227,  228,  235,  229,  230,
 /*  2110 */   231,  232,  233,  175,  248,  247,  430,   37,   39,   40,
 /*  2120 */    42,   41,   36,   38,  430,   37,   39,   40,   42,   41,
 /*  2130 */   171,  220,  224,  218,  250,  168,  278,  430,  213,  175,
 /*  2140 */   296,  430,  430,  430,  430,  430,  430,  430,  430,  430,
 /*  2150 */   430,  226,  537,  207,  430,  430,  171,  220,  224,  218,
 /*  2160 */   250,  168,  430,  430,  213,  430,  430,  430,  430,  430,
 /*  2170 */   430,  430,  297,  298,  299,  300,  301,  226,  309,  207,
 /*  2180 */   430,  318,  319,    8,  430,  430,   11,  234,   87,  430,
 /*  2190 */   521,  528,  430,  430,  419,  227,  228,  430,  229,  230,
 /*  2200 */   231,  232,  233,  430,  430,  421,  186,  430,  430,  430,
 /*  2210 */   434,  430,  430,  234,  257,  259,  258,  430,  257,  259,
 /*  2220 */   258,  227,  228,  217,  229,  230,  231,  232,  233,  175,
 /*  2230 */   430,  257,  259,  258,  204,  430,  430,  430,  262,  261,
 /*  2240 */   430,  430,  257,  259,  258,  430,  171,  220,  224,  218,
 /*  2250 */   250,  168,  270,  430,  213,  175,  206,  430,  430,  430,
 /*  2260 */   430,  430,  430,  430,  257,  259,  258,  226,  430,  207,
 /*  2270 */   430,  430,  171,  220,  224,  218,  250,  168,  253,  430,
 /*  2280 */   213,  217,  430,  430,  430,  430,  430,  430,  430,  257,
 /*  2290 */   259,  258,  430,  226,  430,  207,  254,  261,  430,  430,
 /*  2300 */   430,  430,  430,  234,  430,  430,  430,  430,  430,  430,
 /*  2310 */   430,  227,  228,  422,  229,  230,  231,  232,  233,  430,
 /*  2320 */   430,  257,  259,  258,  430,  430,  430,  430,  430,  234,
 /*  2330 */   430,   67,   68,   70,   69,   71,  430,  227,  228,  212,
 /*  2340 */   229,  230,  231,  232,  233,  175,  430,  257,  259,  258,
 /*  2350 */   182,  430,  430,  430,  430,  430,  430,  430,  257,  259,
 /*  2360 */   258,  430,  171,  220,  224,  218,  250,  168,   73,   74,
 /*  2370 */   213,  359,  175,  430,  430,  430,  463,  430,   72,  430,
 /*  2380 */   430,  430,  430,  226,  430,  207,  430,  430,  430,  171,
 /*  2390 */   220,  224,  218,  250,  168,  184,  430,  213,   51,  376,
 /*  2400 */   430,  430,  430,  257,  259,  258,  430,  257,  259,  258,
 /*  2410 */   226,  430,  207,  198,  430,  430,   26,  430,   33,  234,
 /*  2420 */   430,  257,  259,  258,  430,  430,  430,  227,  228,  430,
 /*  2430 */   229,  230,  231,  232,  233,   54,   55,   56,   57,   58,
 /*  2440 */    59,   60,   61,   62,   63,   64,  234,  430,  430,  430,
 /*  2450 */   430,  430,  430,  430,  227,  228,  430,  229,  230,  231,
 /*  2460 */   232,  233,  430,  203,  205,  170,  179,  183,  185,  187,
 /*  2470 */   189,  181,  177,  191,  193,  201,  195,  197,  199,  203,
 /*  2480 */   205,  170,  179,  183,  185,  187,  189,  181,  177,  191,
 /*  2490 */   193,  201,  195,  197,  199,  203,  205,  170,  179,  183,
 /*  2500 */   185,  187,  189,  181,  177,  191,  193,  201,  195,  197,
 /*  2510 */   199,  287,  176,  430,  430,  180,  430,  430,  430,  430,
 /*  2520 */   257,  259,  258,  257,  259,  258,  430,  435,  430,  430,
 /*  2530 */   430,  430,  219,  430,  430,  430,  430,  280,  430,  430,
 /*  2540 */   257,  259,  258,  430,  430,  430,  203,  205,  170,  179,
 /*  2550 */   183,  185,  187,  189,  181,  177,  191,  193,  201,  195,
 /*  2560 */   197,  199,  203,  205,  170,  179,  183,  185,  187,  189,
 /*  2570 */   181,  177,  191,  193,  201,  195,  197,  199,  430,  430,
 /*  2580 */   357,  430,  430,  225,  430,  430,  430,  358,  257,  259,
 /*  2590 */   258,  257,  259,  258,  430,  430,  203,  205,  170,  179,
 /*  2600 */   183,  185,  187,  189,  181,  177,  191,  193,  201,  195,
 /*  2610 */   197,  199,  430,  430,  430,  430,  430,  430,  209,  203,
 /*  2620 */   205,  170,  179,  183,  185,  187,  189,  181,  177,  191,
 /*  2630 */   193,  201,  195,  197,  199,  203,  205,  170,  179,  183,
 /*  2640 */   185,  187,  189,  181,  177,  191,  193,  201,  195,  197,
 /*  2650 */   199,  423,   67,   68,   70,   69,   71,  169,  430,  430,
 /*  2660 */   377,  430,  430,  430,  430,  257,  259,  258,  430,  203,
 /*  2670 */   205,  170,  179,  183,  185,  187,  189,  181,  177,  191,
 /*  2680 */   193,  201,  195,  197,  199,   51,  430,  430,  430,   73,
 /*  2690 */    74,  430,  565,  481,  430,  487,   51,  430,  430,   72,
 /*  2700 */   430,  430,  430,   80,  430,   33,  430,   67,   68,   70,
 /*  2710 */    69,   71,  430,  430,  104,  430,   33,  430,  430,  430,
 /*  2720 */   430,  430,   54,   55,   56,   57,   58,   59,   60,   61,
 /*  2730 */    62,   63,   64,   54,   55,   56,   57,   58,   59,   60,
 /*  2740 */    61,   62,   63,   64,   73,   74,  430,  561,  493,  430,
 /*  2750 */   487,  430,  430,  200,   72,  430,   67,   68,   70,   69,
 /*  2760 */    71,  257,  259,  258,  430,  430,  430,  279,  430,  430,
 /*  2770 */   273,  430,  430,  430,  430,  257,  259,  258,  257,  259,
 /*  2780 */   258,  430,  430,  430,  430,  430,  430,  430,  430,  430,
 /*  2790 */   430,  430,  430,   73,   74,  430,  562,  493,  430,  487,
 /*  2800 */   430,  430,  430,   72,  430,   67,   68,   70,   69,   71,
 /*  2810 */    67,   68,   70,   69,   71,  430,  430,  430,  430,  430,
 /*  2820 */   430,  430,  430,  430,  430,  430,  430,  430,  430,  430,
 /*  2830 */   430,  430,  430,  430,  430,  430,  430,  430,  430,  430,
 /*  2840 */   430,  430,   73,   74,  430,  569,  461,   73,   74,  430,
 /*  2850 */   567,  461,   72,  430,  430,  430,  430,   72,  430,   67,
 /*  2860 */    68,   70,   69,   71,   67,   68,   70,   69,   71,  430,
 /*  2870 */   430,  430,  430,  430,  430,  430,  430,  430,  430,  430,
 /*  2880 */   430,  430,  430,  430,  430,  430,  430,  430,  430,  430,
 /*  2890 */   430,  430,  430,  430,  430,  501,   73,   74,  430,  564,
 /*  2900 */   461,   73,   74,  430,  430,  430,   72,  430,  430,  101,
 /*  2910 */   430,   72,  430,   67,   68,   70,   69,   71,   67,   68,
 /*  2920 */    70,   69,   71,  430,  430,  430,  430,  430,  430,  430,
 /*  2930 */   430,  430,  430,  430,  430,  430,  430,  430,  430,  430,
 /*  2940 */   430,  430,  430,  430,  430,  430,  430,  430,  430,  430,
 /*  2950 */    73,   74,  430,   18,   75,   73,   74,  430,  563,  461,
 /*  2960 */    72,  430,  430,  430,  430,   72,  430,   67,   68,   70,
 /*  2970 */    69,   71,   67,   68,   70,   69,   71,  430,  430,  430,
 /*  2980 */   430,  430,  430,  430,  430,  430,  430,  430,  430,  430,
 /*  2990 */   430,  430,  430,  430,  430,  430,  430,  430,  430,  430,
 /*  3000 */   430,  430,  430,  430,   73,   74,  430,  568,  461,   73,
 /*  3010 */    74,  430,  566,  510,   72,  430,  430,  430,  430,   72,
 /*  3020 */   430,   67,   68,   70,   69,   71,  430,  430,  430,  430,
 /*  3030 */   430,  430,  430,  430,  430,  430,  430,  430,  430,  430,
 /*  3040 */   430,  430,  430,  430,  430,  430,  430,  430,  430,  430,
 /*  3050 */   430,  430,  495,  430,  430,  430,  430,  430,   73,   74,
 /*  3060 */    67,   68,   70,   69,   71,  430,  463,  430,   72,  430,
 /*  3070 */   430,  430,  430,  430,  430,  430,  430,  430,  430,  430,
 /*  3080 */   430,  430,  430,  430,  430,  430,  430,  430,  430,  430,
 /*  3090 */   430,  430,  430,  430,  430,  430,  430,   73,   74,  430,
 /*  3100 */    20,   75,   67,   68,   70,   69,   71,   72,   67,   68,
 /*  3110 */    70,   69,   71,   67,   68,   70,   69,   71,  430,  430,
 /*  3120 */   430,  430,  430,  430,  430,  430,  430,  430,  430,  430,
 /*  3130 */   430,  430,  430,  430,  430,  430,  430,  430,  430,   73,
 /*  3140 */    74,  430,  430,  430,  430,   73,   74,  101,  430,   72,
 /*  3150 */    73,   74,  430,   23,  430,   72,  430,  430,   77,  430,
 /*  3160 */    72,
};
static YYCODETYPE yy_lookahead[] = {
 /*     0 */     7,   10,   11,   12,   13,   14,   15,   16,   17,   18,
 /*    10 */    19,   20,   21,   22,   23,   32,   40,   24,   25,   26,
 /*    20 */    27,   28,   29,  109,   32,   32,  112,  113,  114,  115,
 /*    30 */   116,  121,  118,  119,  124,  125,  126,  123,   45,  108,
 /*    40 */    47,  110,   49,   50,   51,   52,   53,   54,   55,   56,
 /*    50 */    57,   58,   59,    8,    9,   10,   11,   12,   13,   14,
 /*    60 */    15,   16,   17,   18,   19,   20,   21,   22,   23,  124,
 /*    70 */   125,  126,   89,   90,   81,   92,   93,   94,   95,   96,
 /*    80 */    33,   32,   89,   90,   32,   92,   93,   94,   95,   96,
 /*    90 */    41,   42,    1,    2,    3,    4,    5,   45,   49,   50,
 /*   100 */    51,   52,   53,   54,   55,   56,   57,    6,   59,   60,
 /*   110 */    61,   62,   47,   64,    6,  116,  111,   68,   69,   70,
 /*   120 */    71,  116,  123,   74,   32,  120,    6,   82,  123,   38,
 /*   130 */    39,   30,    6,   41,   42,   86,   87,   88,   30,   48,
 /*   140 */    91,   49,   50,   51,   52,   53,   54,   55,   56,   57,
 /*   150 */    30,   59,   60,   61,   62,   32,   64,   21,   22,   23,
 /*   160 */    68,   69,   70,   71,  161,   47,   74,   32,   89,   90,
 /*   170 */    66,   92,   93,   94,   95,   96,   41,   42,   86,   87,
 /*   180 */    88,  146,  147,   91,   49,   50,   51,   52,   53,   54,
 /*   190 */    55,   56,   57,   32,   59,   60,   61,   62,   80,   64,
 /*   200 */    32,   83,   35,   68,   69,   70,   71,   40,   82,   74,
 /*   210 */    32,   18,   19,   20,   21,   22,   23,   41,    6,   41,
 /*   220 */    42,   86,   87,   88,   43,  123,   91,   49,   50,   51,
 /*   230 */    52,   53,   54,   55,   56,   57,  117,   59,   60,   61,
 /*   240 */    62,   65,   64,   67,    6,   40,   68,   69,   70,   71,
 /*   250 */    89,   90,   74,   32,  116,    6,   75,   76,   77,   78,
 /*   260 */    79,  123,   41,   42,   86,   87,   88,    6,   30,   91,
 /*   270 */    49,   50,   51,   52,   53,   54,   55,   56,   57,   30,
 /*   280 */    59,   60,   61,   62,   72,   64,  167,    6,    6,   68,
 /*   290 */    69,   70,   71,  148,   33,   74,   32,  149,  150,  154,
 /*   300 */   155,  156,  157,  158,    6,   41,   42,   86,   87,   88,
 /*   310 */    32,   30,   91,   49,   50,   51,   52,   53,   54,   55,
 /*   320 */    56,   57,   40,   59,   60,   61,   62,   32,   64,  159,
 /*   330 */   160,   33,   68,   69,   70,   71,  148,  121,   74,   32,
 /*   340 */   124,  125,  126,  155,  156,  157,  158,    6,   41,   42,
 /*   350 */    86,   87,   88,   80,   81,   91,   49,   50,   51,   52,
 /*   360 */    53,   54,   55,   56,   57,  107,   59,   60,   61,   62,
 /*   370 */     6,   64,  159,  160,   33,   68,   69,   70,   71,  148,
 /*   380 */    32,   74,   32,  152,  159,  160,    6,  156,  157,  158,
 /*   390 */     6,   41,   42,   86,   87,   88,    6,   33,   91,   49,
 /*   400 */    50,   51,   52,   53,   54,   55,   56,   57,  107,   59,
 /*   410 */    60,   61,   62,   33,   64,  159,  160,   33,   68,   69,
 /*   420 */    70,   71,  148,   33,   74,   32,  152,  159,  160,    6,
 /*   430 */   156,  157,  158,    6,   41,   42,   86,   87,   88,  159,
 /*   440 */   160,   91,   49,   50,   51,   52,   53,   54,   55,   56,
 /*   450 */    57,  151,   59,   60,   61,   62,   33,   64,  159,  160,
 /*   460 */    33,   68,   69,   70,   71,  148,   35,   74,   32,  116,
 /*   470 */   153,   40,  119,  156,  157,  158,  123,   41,   42,   86,
 /*   480 */    87,   88,    6,   66,   91,   49,   50,   51,   52,   53,
 /*   490 */    54,   55,   56,   57,   40,   59,   60,   61,   62,    6,
 /*   500 */    64,  159,  160,   47,   68,   69,   70,   71,  148,   33,
 /*   510 */    74,   32,  152,  159,  160,    6,  156,  157,  158,   47,
 /*   520 */    41,   42,   86,   87,   88,   66,   33,   91,   49,   50,
 /*   530 */    51,   52,   53,   54,   55,   56,   57,   69,   59,   60,
 /*   540 */    61,   62,   33,   64,  159,  160,   32,   68,   69,   70,
 /*   550 */    71,  148,  117,   74,   32,  152,    6,   82,   33,  156,
 /*   560 */   157,  158,  116,   41,   42,   86,   87,   88,   43,  123,
 /*   570 */    91,   49,   50,   51,   52,   53,   54,   55,   56,   57,
 /*   580 */    30,   59,   60,   61,   62,   81,   64,   36,    6,   33,
 /*   590 */    68,   69,   70,   71,   32,  148,   74,   32,  117,   43,
 /*   600 */   165,  166,  167,  156,  157,  158,   41,   42,   86,   87,
 /*   610 */    88,  164,   30,   91,   49,   50,   51,   52,   53,   54,
 /*   620 */    55,   56,   57,   33,   59,   60,   61,   62,  151,   64,
 /*   630 */   106,   40,   40,   68,   69,   70,   71,  116,   32,   74,
 /*   640 */    32,  120,   32,  162,  123,   32,  165,  166,  167,   41,
 /*   650 */    42,   86,   87,   88,   32,   32,   91,   49,   50,   51,
 /*   660 */    52,   53,   54,   55,   56,   57,   32,   59,   60,   61,
 /*   670 */    62,  110,   64,   32,  106,   32,   68,   69,   70,   71,
 /*   680 */   148,   82,   74,   32,  152,   47,  151,   32,  156,  157,
 /*   690 */   158,   32,   41,   42,   86,   87,   88,   33,   45,   91,
 /*   700 */    49,   50,   51,   52,   53,   54,   55,   56,   57,   47,
 /*   710 */    59,   60,   61,   62,   43,   64,   33,  151,  117,   68,
 /*   720 */    69,   70,   71,  148,   35,   74,   32,  152,   32,   40,
 /*   730 */    82,  156,  157,  158,  107,   41,   42,   86,   87,   88,
 /*   740 */    45,   33,   91,   49,   50,   51,   52,   53,   54,   55,
 /*   750 */    56,   57,   43,   59,   60,   61,   62,   47,   64,   47,
 /*   760 */    33,  117,   68,   69,   70,   71,  148,   82,   74,   32,
 /*   770 */   152,   33,   82,   13,  156,  157,  158,  151,   41,   42,
 /*   780 */    86,   87,   88,   32,   32,   91,   49,   50,   51,   52,
 /*   790 */    53,   54,   55,   56,   57,   33,   59,   60,   61,   62,
 /*   800 */    72,   64,   32,  117,  117,   68,   69,   70,   71,  148,
 /*   810 */    33,   74,   32,  107,  153,   43,  151,  156,  157,  158,
 /*   820 */    32,   41,   42,   86,   87,   88,   33,   32,   91,   49,
 /*   830 */    50,   51,   52,   53,   54,   55,   56,   57,  107,   59,
 /*   840 */    60,   61,   62,  117,   64,   33,   32,   82,   68,   69,
 /*   850 */    70,   71,  148,   32,   74,   32,  152,  160,  117,   43,
 /*   860 */   156,  157,  158,   66,   41,   42,   86,   87,   88,  151,
 /*   870 */    43,   91,   49,   50,   51,   52,   53,   54,   55,   56,
 /*   880 */    57,   32,   59,   60,   61,   62,  107,   64,  117,   69,
 /*   890 */    40,   68,   69,   70,   71,  117,  148,   74,   32,  101,
 /*   900 */   102,  103,  104,  105,  156,  157,  158,   41,   42,   86,
 /*   910 */    87,   88,    0,   33,   91,   49,   50,   51,   52,   53,
 /*   920 */    54,   55,   56,   57,   40,   59,   60,   61,   62,   32,
 /*   930 */    64,  150,   36,   33,   68,   69,   70,   71,  148,  147,
 /*   940 */    74,   32,  152,  161,   43,   66,  156,  157,  158,   41,
 /*   950 */    41,   42,   86,   87,   88,   40,  117,   91,   49,   50,
 /*   960 */    51,   52,   53,   54,   55,   56,   57,   32,   59,   60,
 /*   970 */    61,   62,   66,   64,   33,  151,   63,   68,   69,   70,
 /*   980 */    71,  148,   40,   74,   32,   40,  153,   30,   33,  156,
 /*   990 */   157,  158,   32,   41,   42,   86,   87,   88,   33,   63,
 /*  1000 */    91,   49,   50,   51,   52,   53,   54,   55,   56,   57,
 /*  1010 */   107,   59,   60,   61,   62,   40,   64,   40,   33,   33,
 /*  1020 */    68,   69,   70,   71,  148,  121,   74,   32,  124,  125,
 /*  1030 */   126,   33,  156,  157,  158,   32,   41,   42,   86,   87,
 /*  1040 */    88,  151,   40,   91,   49,   50,   51,   52,   53,   54,
 /*  1050 */    55,   56,   57,   32,   59,   60,   61,   62,   33,   64,
 /*  1060 */     6,  168,  168,   68,   69,   70,   71,  148,  121,   74,
 /*  1070 */    32,  124,  125,  126,  168,  156,  157,  158,  168,   41,
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
 /*  1190 */   142,  143,  144,  105,   98,   99,  100,  101,  102,  103,
 /*  1200 */   104,  105,  168,  148,  156,  157,  158,  168,  168,  168,
 /*  1210 */   122,  156,  157,  158,  168,  127,  128,  129,  130,  131,
 /*  1220 */   132,  133,  134,  135,  136,  137,  138,  139,  140,  141,
 /*  1230 */   142,  143,  144,  168,  114,  105,  116,  168,  118,  119,
 /*  1240 */   168,  168,  168,  123,  156,  157,  158,    1,    2,    3,
 /*  1250 */     4,    5,  122,   41,  168,  168,  168,  127,  128,  129,
 /*  1260 */   130,  131,  132,  133,  134,  135,  136,  137,  138,  139,
 /*  1270 */   140,  141,  142,  143,  144,  105,  168,   65,  168,   67,
 /*  1280 */   168,  168,  168,  168,   38,   39,  156,  157,  158,  168,
 /*  1290 */    44,  168,  122,  168,   48,  168,  168,  127,  128,  129,
 /*  1300 */   130,  131,  132,  133,  134,  135,  136,  137,  138,  139,
 /*  1310 */   140,  141,  142,  143,  144,  168,  105,  168,  112,  168,
 /*  1320 */   114,  115,  116,  168,  118,  119,  156,  157,  158,  123,
 /*  1330 */    35,  168,   37,  122,  168,   40,  168,  168,  127,  128,
 /*  1340 */   129,  130,  131,  132,  133,  134,  135,  136,  137,  138,
 /*  1350 */   139,  140,  141,  142,  143,  144,  168,  168,  105,  168,
 /*  1360 */   168,  145,  168,  168,  148,  168,  168,  156,  157,  158,
 /*  1370 */    43,  168,  156,  157,  158,  122,  168,  168,  168,  168,
 /*  1380 */   127,  128,  129,  130,  131,  132,  133,  134,  135,  136,
 /*  1390 */   137,  138,  139,  140,  141,  142,  143,  144,  105,  168,
 /*  1400 */   168,  168,   75,   76,   77,   78,   79,  168,   81,  156,
 /*  1410 */   157,  158,  168,  168,  168,  122,  168,  168,  168,  168,
 /*  1420 */   127,  128,  129,  130,  131,  132,  133,  134,  135,  136,
 /*  1430 */   137,  138,  139,  140,  141,  142,  143,  144,  168,  105,
 /*  1440 */   168,  168,  113,  114,  115,  116,  168,  118,  119,  156,
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
 /*  1680 */   141,  142,  143,  144,  168,  105,  168,  168,  168,  168,
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
 /*  1870 */    29,   30,  168,   32,    7,  168,   31,  168,  168,   34,
 /*  1880 */   168,   36,  168,   38,   39,  168,   45,   42,   47,  168,
 /*  1890 */   168,   24,   25,   26,   27,   28,   29,  168,  168,   32,
 /*  1900 */   148,  168,  168,  168,  168,  168,  168,    7,  156,  157,
 /*  1910 */   158,  168,   45,  168,   47,  163,  164,  168,  168,  125,
 /*  1920 */   168,  168,   81,  168,   24,   25,   26,   27,   28,   29,
 /*  1930 */    89,   90,   32,   92,   93,   94,   95,   96,  168,  168,
 /*  1940 */   168,  168,  148,  168,  168,   45,  168,   47,   81,   82,
 /*  1950 */   156,  157,  158,  168,  168,  168,   89,   90,  168,   92,
 /*  1960 */    93,   94,   95,   96,  168,    8,    9,   10,   11,   12,
 /*  1970 */    13,   14,   15,   16,   17,   18,   19,   20,   21,   22,
 /*  1980 */    23,   81,   82,  168,  168,  168,  168,   12,  168,   89,
 /*  1990 */    90,  148,   92,   93,   94,   95,   96,    7,  168,  156,
 /*  2000 */   157,  158,  168,  168,  168,   30,  168,   32,  168,    1,
 /*  2010 */     2,    3,    4,    5,   24,   25,   26,   27,   28,   29,
 /*  2020 */   168,  168,   32,    7,   49,   50,   51,   52,   53,   54,
 /*  2030 */    55,   56,   57,   58,   59,   45,  168,   47,  168,   82,
 /*  2040 */    24,   25,   26,   27,   28,   29,   38,   39,   32,   41,
 /*  2050 */    42,  168,   44,  168,  168,  168,   48,   32,   43,  148,
 /*  2060 */   168,   45,  168,   47,  168,   32,  168,  156,  157,  158,
 /*  2070 */   168,   81,   82,  168,  163,  164,  168,  168,  168,   89,
 /*  2080 */    90,  168,   92,   93,   94,   95,   96,  168,  168,   73,
 /*  2090 */    75,   76,   77,   78,   79,   80,   81,   81,   83,   84,
 /*  2100 */    85,  168,  168,  168,  168,   89,   90,   82,   92,   93,
 /*  2110 */    94,   95,   96,    7,   89,   90,  168,   92,   93,   94,
 /*  2120 */    95,   96,   89,   90,  168,   92,   93,   94,   95,   96,
 /*  2130 */    24,   25,   26,   27,   28,   29,   30,  168,   32,    7,
 /*  2140 */    43,  168,  168,  168,  168,  168,  168,  168,  168,  168,
 /*  2150 */   168,   45,    0,   47,  168,  168,   24,   25,   26,   27,
 /*  2160 */    28,   29,  168,  168,   32,  168,  168,  168,  168,  168,
 /*  2170 */   168,  168,   75,   76,   77,   78,   79,   45,   81,   47,
 /*  2180 */   168,   84,   85,   31,  168,  168,   34,   81,   36,  168,
 /*  2190 */    38,   39,  168,  168,   42,   89,   90,  168,   92,   93,
 /*  2200 */    94,   95,   96,  168,  168,   73,  148,  168,  168,  168,
 /*  2210 */   148,  168,  168,   81,  156,  157,  158,  168,  156,  157,
 /*  2220 */   158,   89,   90,  148,   92,   93,   94,   95,   96,    7,
 /*  2230 */   168,  156,  157,  158,  148,  168,  168,  168,  163,  164,
 /*  2240 */   168,  168,  156,  157,  158,  168,   24,   25,   26,   27,
 /*  2250 */    28,   29,   30,  168,   32,    7,  148,  168,  168,  168,
 /*  2260 */   168,  168,  168,  168,  156,  157,  158,   45,  168,   47,
 /*  2270 */   168,  168,   24,   25,   26,   27,   28,   29,   30,  168,
 /*  2280 */    32,  148,  168,  168,  168,  168,  168,  168,  168,  156,
 /*  2290 */   157,  158,  168,   45,  168,   47,  163,  164,  168,  168,
 /*  2300 */   168,  168,  168,   81,  168,  168,  168,  168,  168,  168,
 /*  2310 */   168,   89,   90,  148,   92,   93,   94,   95,   96,  168,
 /*  2320 */   168,  156,  157,  158,  168,  168,  168,  168,  168,   81,
 /*  2330 */   168,    1,    2,    3,    4,    5,  168,   89,   90,  148,
 /*  2340 */    92,   93,   94,   95,   96,    7,  168,  156,  157,  158,
 /*  2350 */   148,  168,  168,  168,  168,  168,  168,  168,  156,  157,
 /*  2360 */   158,  168,   24,   25,   26,   27,   28,   29,   38,   39,
 /*  2370 */    32,   33,    7,  168,  168,  168,   46,  168,   48,  168,
 /*  2380 */   168,  168,  168,   45,  168,   47,  168,  168,  168,   24,
 /*  2390 */    25,   26,   27,   28,   29,  148,  168,   32,   12,  148,
 /*  2400 */   168,  168,  168,  156,  157,  158,  168,  156,  157,  158,
 /*  2410 */    45,  168,   47,  148,  168,  168,   30,  168,   32,   81,
 /*  2420 */   168,  156,  157,  158,  168,  168,  168,   89,   90,  168,
 /*  2430 */    92,   93,   94,   95,   96,   49,   50,   51,   52,   53,
 /*  2440 */    54,   55,   56,   57,   58,   59,   81,  168,  168,  168,
 /*  2450 */   168,  168,  168,  168,   89,   90,  168,   92,   93,   94,
 /*  2460 */    95,   96,  168,    8,    9,   10,   11,   12,   13,   14,
 /*  2470 */    15,   16,   17,   18,   19,   20,   21,   22,   23,    8,
 /*  2480 */     9,   10,   11,   12,   13,   14,   15,   16,   17,   18,
 /*  2490 */    19,   20,   21,   22,   23,    8,    9,   10,   11,   12,
 /*  2500 */    13,   14,   15,   16,   17,   18,   19,   20,   21,   22,
 /*  2510 */    23,   40,  148,  168,  168,  148,  168,  168,  168,  168,
 /*  2520 */   156,  157,  158,  156,  157,  158,  168,   40,  168,  168,
 /*  2530 */   168,  168,  148,  168,  168,  168,  168,   82,  168,  168,
 /*  2540 */   156,  157,  158,  168,  168,  168,    8,    9,   10,   11,
 /*  2550 */    12,   13,   14,   15,   16,   17,   18,   19,   20,   21,
 /*  2560 */    22,   23,    8,    9,   10,   11,   12,   13,   14,   15,
 /*  2570 */    16,   17,   18,   19,   20,   21,   22,   23,   40,  168,
 /*  2580 */   148,  168,  168,  148,  168,  168,  168,   33,  156,  157,
 /*  2590 */   158,  156,  157,  158,  168,  168,    8,    9,   10,   11,
 /*  2600 */    12,   13,   14,   15,   16,   17,   18,   19,   20,   21,
 /*  2610 */    22,   23,  168,  168,  168,  168,  168,  168,   30,    8,
 /*  2620 */     9,   10,   11,   12,   13,   14,   15,   16,   17,   18,
 /*  2630 */    19,   20,   21,   22,   23,    8,    9,   10,   11,   12,
 /*  2640 */    13,   14,   15,   16,   17,   18,   19,   20,   21,   22,
 /*  2650 */    23,   40,    1,    2,    3,    4,    5,  148,  168,  168,
 /*  2660 */    33,  168,  168,  168,  168,  156,  157,  158,  168,    8,
 /*  2670 */     9,   10,   11,   12,   13,   14,   15,   16,   17,   18,
 /*  2680 */    19,   20,   21,   22,   23,   12,  168,  168,  168,   38,
 /*  2690 */    39,  168,   41,   42,  168,   44,   12,  168,  168,   48,
 /*  2700 */   168,  168,  168,   30,  168,   32,  168,    1,    2,    3,
 /*  2710 */     4,    5,  168,  168,   30,  168,   32,  168,  168,  168,
 /*  2720 */   168,  168,   49,   50,   51,   52,   53,   54,   55,   56,
 /*  2730 */    57,   58,   59,   49,   50,   51,   52,   53,   54,   55,
 /*  2740 */    56,   57,   58,   59,   38,   39,  168,   41,   42,  168,
 /*  2750 */    44,  168,  168,  148,   48,  168,    1,    2,    3,    4,
 /*  2760 */     5,  156,  157,  158,  168,  168,  168,  148,  168,  168,
 /*  2770 */   148,  168,  168,  168,  168,  156,  157,  158,  156,  157,
 /*  2780 */   158,  168,  168,  168,  168,  168,  168,  168,  168,  168,
 /*  2790 */   168,  168,  168,   38,   39,  168,   41,   42,  168,   44,
 /*  2800 */   168,  168,  168,   48,  168,    1,    2,    3,    4,    5,
 /*  2810 */     1,    2,    3,    4,    5,  168,  168,  168,  168,  168,
 /*  2820 */   168,  168,  168,  168,  168,  168,  168,  168,  168,  168,
 /*  2830 */   168,  168,  168,  168,  168,  168,  168,  168,  168,  168,
 /*  2840 */   168,  168,   38,   39,  168,   41,   42,   38,   39,  168,
 /*  2850 */    41,   42,   48,  168,  168,  168,  168,   48,  168,    1,
 /*  2860 */     2,    3,    4,    5,    1,    2,    3,    4,    5,  168,
 /*  2870 */   168,  168,  168,  168,  168,  168,  168,  168,  168,  168,
 /*  2880 */   168,  168,  168,  168,  168,  168,  168,  168,  168,  168,
 /*  2890 */   168,  168,  168,  168,  168,   32,   38,   39,  168,   41,
 /*  2900 */    42,   38,   39,  168,  168,  168,   48,  168,  168,   46,
 /*  2910 */   168,   48,  168,    1,    2,    3,    4,    5,    1,    2,
 /*  2920 */     3,    4,    5,  168,  168,  168,  168,  168,  168,  168,
 /*  2930 */   168,  168,  168,  168,  168,  168,  168,  168,  168,  168,
 /*  2940 */   168,  168,  168,  168,  168,  168,  168,  168,  168,  168,
 /*  2950 */    38,   39,  168,   41,   42,   38,   39,  168,   41,   42,
 /*  2960 */    48,  168,  168,  168,  168,   48,  168,    1,    2,    3,
 /*  2970 */     4,    5,    1,    2,    3,    4,    5,  168,  168,  168,
 /*  2980 */   168,  168,  168,  168,  168,  168,  168,  168,  168,  168,
 /*  2990 */   168,  168,  168,  168,  168,  168,  168,  168,  168,  168,
 /*  3000 */   168,  168,  168,  168,   38,   39,  168,   41,   42,   38,
 /*  3010 */    39,  168,   41,   42,   48,  168,  168,  168,  168,   48,
 /*  3020 */   168,    1,    2,    3,    4,    5,  168,  168,  168,  168,
 /*  3030 */   168,  168,  168,  168,  168,  168,  168,  168,  168,  168,
 /*  3040 */   168,  168,  168,  168,  168,  168,  168,  168,  168,  168,
 /*  3050 */   168,  168,   32,  168,  168,  168,  168,  168,   38,   39,
 /*  3060 */     1,    2,    3,    4,    5,  168,   46,  168,   48,  168,
 /*  3070 */   168,  168,  168,  168,  168,  168,  168,  168,  168,  168,
 /*  3080 */   168,  168,  168,  168,  168,  168,  168,  168,  168,  168,
 /*  3090 */   168,  168,  168,  168,  168,  168,  168,   38,   39,  168,
 /*  3100 */    41,   42,    1,    2,    3,    4,    5,   48,    1,    2,
 /*  3110 */     3,    4,    5,    1,    2,    3,    4,    5,  168,  168,
 /*  3120 */   168,  168,  168,  168,  168,  168,  168,  168,  168,  168,
 /*  3130 */   168,  168,  168,  168,  168,  168,  168,  168,  168,   38,
 /*  3140 */    39,  168,  168,  168,  168,   38,   39,   46,  168,   48,
 /*  3150 */    38,   39,  168,   46,  168,   48,  168,  168,   46,  168,
 /*  3160 */    48,
};
#define YY_SHIFT_USE_DFLT (-25)
static short yy_shift_ofst[] = {
 /*     0 */  1845,  912, 2152,  -25,  -25,  -25,  -25,  -25,  788,  762,
 /*    10 */   -25,  659,  167,  -25,  622,  592,  -25, 2912,  -25, 3059,
 /*    20 */   -25,  -25, 3107,  278,   65, 2386,  998,  -25,  108,  941,
 /*    30 */   -25, 1107,  -25,  816,   79,  -25,  -25,  -25,  -25,  -25,
 /*    40 */   -25,  -25,  -25,  634,  772,   79,  -25,  168,  901,   79,
 /*    50 */   -25,  770,  760,  696,  -25,  -25,  -25,  -25,  -25,  -25,
 /*    60 */   -25,  -25,  -25,  -25,  -25,  -25,  -25,  -25,  -25,  -25,
 /*    70 */   -25,  -25,  -25,  -25,  -25,   91, 3112,  849,  712, 2673,
 /*    80 */   900,  -25,  120,  793,  -25,  -25,  -25,  821, 1295,  -25,
 /*    90 */  1021,  205,  -25, 2008,  -25,  908,  -25, 2706, 2858,  -25,
 /*   100 */  3101,  795,  472, 2684,  850,   49,  -25,  307,  -25,  -25,
 /*   110 */   -25,  -25,  -25,  -25,  -25,  -25,  -25,  -25,  -25,  -25,
 /*   120 */   -25,  -25,  -25,  -25,  -25,  -25,  -25,  -25,  986,  -25,
 /*   130 */   965,  -25, 2365,  942,  651,   92,  913,  915,  909,  221,
 /*   140 */   -25, 2365,  884,  176,  -25, 1212,  -25,  -25,   79,  906,
 /*   150 */  1081, 1081,  945,  694,  780,  -25, 2365,  977,  866,  952,
 /*   160 */   -25, 1002,  995, 1038,  468, 2365,   47,  -25, 2365,  -25,
 /*   170 */  2365,  123,  -25,  504, 2365, 2365, 2661, 2365,  193, 2365,
 /*   180 */   193, 2365,  193, 2365,  193, 2365,  193, 2365,  193, 2365,
 /*   190 */   193, 2365,  136, 2365,  136, 2365,  -25, 2365,  -25, 2365,
 /*   200 */   -25, 2365,  136, 2365,   -9, 2365,   -9,   -7, 2588,  -25,
 /*   210 */   957, 2365,  -25, 1037,  613,  662, 1841, 2661, 2365,  -25,
 /*   220 */  1003, 1054,  123,  -25, 2365,  -25,  -25,  -25,  -25,  -25,
 /*   230 */   -25,  -25,  -25,  -25, 2025,  -25,  126,  -25,  -17,  -25,
 /*   240 */   797, 2033,  -25,  -25,  -25,  -25,  104,  417,  459,  -25,
 /*   250 */   641,  710, 2248,  -25,  238,  -25, 2365,  -25,  -25,  -25,
 /*   260 */   -25,  -25,  249,  -25,  -25,   52,  638, 2222,  101,  -25,
 /*   270 */   -25,  -25, 2365, 1957,  -25, 2106,  281,  -25,  -25, 2455,
 /*   280 */   -25,  193, 2661,  610,  212, 2132, 2471, 1081,  178,  -25,
 /*   290 */   814,  261,  -25,  814,  -25, 2015,  -25,  -25,  -25,  -25,
 /*   300 */   -25,  -25, 2365,  -25, 2661,  623, 2097, 2365,  -25, 1867,
 /*   310 */   181, 2365,  -25,  765,  181, 2365,  -25,   45,  -25,  -25,
 /*   320 */   960, 1327, 2365,  -25, 1900,  181, 2365,  -25,  475,  181,
 /*   330 */  2365,  -25, 1990,  181, 2365,  -25,  685,  181, 2365,  -25,
 /*   340 */   -25,  -25,  -25, 2365,  298,  -25, 2365,  -25, 2661,  -25,
 /*   350 */   955,  -25,  985,  -25, 1025,  -25, 2338, 2554,  -25,  -25,
 /*   360 */   514,  273,  562,  590,  -25,  161,  599,  664,  -25,  648,
 /*   370 */   708,  -25,  690,  738,  -25, 2365, 2627,  -25,  295,  341,
 /*   380 */   -25,  295,  -25,  827,   79,  -25,  -25,  295,  364,  -25,
 /*   390 */   295,  380,  -25,  295,  384,  -25,  295,  390,  -25,  295,
 /*   400 */   423,  -25,  295,  427,  -25,  295,  476,  -25,  295,  493,
 /*   410 */   -25,  295,  509,  -25,  118,  643,  638,  655,  662,  -25,
 /*   420 */   -25, 2365, 2611, 1081,  436,  -25,  752,  728, 2016, 2538,
 /*   430 */  1081,  737,  -25, 2365, 2487, 1081,  823,  -25,  820, 2365,
 /*   440 */   880,  -25,  -25,  -25,  879, 1081, 1081,  -25,  -25,  936,
 /*   450 */   975,  264,  350,  -25,  -25,  550,  -24,  565,  -25,  608,
 /*   460 */   -25,   91, 2330,  348,  456, 1975,  454,  135,  -25,  522,
 /*   470 */   -25,  582,  591,  479,  -25,  393,  -25, 2651, 2966,  -25,
 /*   480 */   -25, 1246,  653,  671,   79,  683,  -25,  695,  709,   79,
 /*   490 */   727,  -25,  -25, 1246, 3020,  525,  -25,   79,  777,  -25,
 /*   500 */  2863,  556,  -25,   79,  812,  -25,  -25, 2755, 2971, 2804,
 /*   510 */    91, 2809,  -25, 2917,  897,  282,  -25,  897,  -25,  -25,
 /*   520 */   -25,  896,  935,  431,  -25,   -8,  205,  -25,  551,  606,
 /*   530 */   689,  -25,  751,  205,  -25,  -25,
};
#define YY_REDUCE_USE_DFLT (-91)
static short yy_reduce_ofst[] = {
 /*     0 */  1096,  -91,  798,  -91,  -91,  -91,  -91,  -91,  -91,  -91,
 /*    10 */   -91,  -91,  568,  -91,  -91,  524,  -91,    5,  -91,  521,
 /*    20 */   -91,  -91,  102,  -91,  -91,  216,  -91,  -91,  -91,  -91,
 /*    30 */   -91,  -55,  -91,  -91,  741,  -91,  -91,  -91,  -91,  -91,
 /*    40 */   -91,  -91,  -91,  -91,  -91,  686,  -91,  -91,  -91,  778,
 /*    50 */   -91,  -91,  -91,  -91,  -91,  -91,  -91,  -91,  -91,  -91,
 /*    60 */   -91,  -91,  -91,  -91,  -91,  -91,  -91,  -91,  -91,  -91,
 /*    70 */   -91,  -91,  -91,  -91,  -91,   -1,  102,  -91,  -91,  947,
 /*    80 */   -91,  -91,  -91,  -91,  -91,  -91,  -91,  -91,  706,  -91,
 /*    90 */   -91,  627,  -91,  -86,  -91,  -91,  -91, 1329,  353,  -91,
 /*   100 */   102,  -91,  -91,  -90,  -91, 1334,  -91, 1703,  -91,  -91,
 /*   110 */   -91,  -91,  -91,  -91,  -91,  -91,  -91,  -91,  -91,  -91,
 /*   120 */   -91,  -91,  -91,  -91,  -91,  -91,  -91,  -91,  -91,  -91,
 /*   130 */   -91,  -91, 1506,  -91, 1130, 1703,  -91,  -91, 1580, 1703,
 /*   140 */   -91, 1462,  -91,   35,  -91,  792,  -91,  -91,  839,  -91,
 /*   150 */  1088, 1703,  -91, 1170, 1703,  -91, 1383,  -91, 1211, 1703,
 /*   160 */   -91,  -91, 1253, 1703,  -91, 1216,  -91,  -91, 2509,  -91,
 /*   170 */   919,  782,  -91,  -91, 2619, 2364,  -91,  876,  -91, 2367,
 /*   180 */   -91, 2202,  -91, 2247,  -91, 2058,  -91, 1668,  -91, 1585,
 /*   190 */   -91, 1542,  -91, 1843,  -91, 1665,  -91, 2265,  -91, 2605,
 /*   200 */   -91, 1055,  -91, 2086,  -91, 2108,  -91, 1794,  -91,  -91,
 /*   210 */   -91, 2191,  -91,  -91,  -91,  -91, 2075,  -91, 2384,  -91,
 /*   220 */   -91,  -91,    3,  -91, 2435,  -91,  -91,  -91,  -91,  -91,
 /*   230 */   -91,  -91,  -91,  -91,  481,  -91,  -91,  -91,  435,  -91,
 /*   240 */   -91,  119,  -91,  -91,  -91,  -91,  -91,  -91,  -91,  -91,
 /*   250 */   -91,  -91, 2133,  -91,  -91,  -91,  447,  -91,  -91,  -91,
 /*   260 */   -91,  -91,  -91,  -91,  -91,  -91,  -91, 1752,  -91,  -91,
 /*   270 */   -91,  -91, 2622,  -91,  -91, 1911,  -91,  -91,  -91,  -91,
 /*   280 */   -91,  -91,  -91,  -91,  -91,  748,  -91, 1457, 1703,  -91,
 /*   290 */   148,  -91,  -91,  781,  -91,  824,  -91,  -91,  -91,  -91,
 /*   300 */   -91,  -91,  403,  -91,  -91,  -91,  535,  360,  -91,  317,
 /*   310 */   665,  274,  -91,  -91,  718,  231,  -91,  -91,  -91,  -91,
 /*   320 */   -91,  890,  532,  -91,  833,  300,  790,  -91,  -91,  477,
 /*   330 */   704,  -91,  661,  566,  618,  -91,  -91,  626,  575,  -91,
 /*   340 */   -91,  -91,  -91,  145,  -91,  -91,  188,  -91,  -91,  -91,
 /*   350 */   -91,  -91,  -91,  -91,  -91,  -91, 2432,  -91,  -91,  -91,
 /*   360 */   -91,  -91,  -91,  -91,  -91,  -91,  -91,  -91,  -91,  -91,
 /*   370 */   -91,  -91,  -91,  -91,  -91, 2251,  -91,  -91,  170,  -91,
 /*   380 */   -91,  697,  -91,  -91,  771,  -91,  -91,  213,  -91,  -91,
 /*   390 */   225,  -91,  -91,  256,  -91,  -91,  268,  -91,  -91,  280,
 /*   400 */   -91,  -91,  299,  -91,  -91,  342,  -91,  -91,  354,  -91,
 /*   410 */   -91,  385,  -91,  -91,  -91,  -91,  -91,  -91,  -91,  -91,
 /*   420 */   -91, 2165,  -91, 1662, 1703,  -91,  -91,  -91, 1629,  -91,
 /*   430 */  1048, 1703,  -91, 2062,  -91, 1622, 1703,  -91,  -91, 1339,
 /*   440 */   -91,  -91,  -91,  -91,  -91, 1539, 1703,  -91,  -91,  -91,
 /*   450 */   -91, 1499, 1703,  -91,  -91,  -91,  -91, 1416,  -91, 1703,
 /*   460 */   -91,  138,  102,  -91,  -91,  904,  -91, 1376,  -91, 1703,
 /*   470 */   -91,  -91,  -91, 1293,  -91, 1703,  -91, 1120,  353,  -91,
 /*   480 */   -91,  138,  -91,  -91,  601,  -91,  -91,  -91,  -91,  644,
 /*   490 */   -91,  -91,  -91,  446,  102,  -91,  -91,  687,  -91,  -91,
 /*   500 */   102,  -91,  -91,  726,  -91,  -91,  -91, 1206, 1452,  353,
 /*   510 */   446,  353,  -91,  353,  -69,  731,  -91,  561,  -91,  -91,
 /*   520 */   -91,  -91,  -91,  903,  -91,  -91,  301,  -91,  -91,  -91,
 /*   530 */   258,  -91,  -91,  779,  -91,  -91,
};
static YYACTIONTYPE yy_default[] = {
 /*     0 */   796,  796,  796,  538,  540,  541,  542,  543,  796,  796,
 /*    10 */   544,  796,  796,  545,  796,  796,  546,  796,  559,  796,
 /*    20 */   560,  580,  796,  796,  796,  796,  796,  592,  796,  796,
 /*    30 */   593,  796,  606,  608,  796,  611,  786,  787,  788,  789,
 /*    40 */   790,  791,  792,  796,  609,  796,  612,  796,  610,  796,
 /*    50 */   613,  796,  796,  796,  615,  616,  617,  618,  619,  620,
 /*    60 */   621,  622,  623,  624,  625,  607,  596,  598,  599,  600,
 /*    70 */   601,  602,  603,  604,  605,  796,  796,  796,  796,  796,
 /*    80 */   796,  594,  796,  796,  595,  597,  581,  796,  796,  547,
 /*    90 */   796,  796,  548,  796,  554,  796,  555,  796,  796,  578,
 /*   100 */   796,  796,  796,  796,  796,  796,  584,  796,  586,  626,
 /*   110 */   628,  629,  630,  631,  632,  633,  634,  635,  636,  637,
 /*   120 */   638,  639,  640,  641,  642,  643,  644,  645,  796,  646,
 /*   130 */   796,  647,  796,  796,  796,  796,  650,  796,  796,  796,
 /*   140 */   651,  796,  796,  796,  654,  796,  655,  656,  796,  796,
 /*   150 */   658,  659,  796,  796,  796,  662,  796,  796,  796,  796,
 /*   160 */   664,  796,  796,  796,  796,  796,  796,  666,  796,  723,
 /*   170 */   796,  796,  724,  796,  796,  796,  725,  796,  727,  796,
 /*   180 */   728,  796,  729,  796,  730,  796,  731,  796,  732,  796,
 /*   190 */   733,  796,  740,  796,  741,  796,  742,  796,  743,  796,
 /*   200 */   744,  796,  745,  796,  746,  796,  747,  796,  796,  734,
 /*   210 */   796,  796,  735,  752,  796,  736,  796,  776,  796,  748,
 /*   220 */   796,  796,  796,  749,  796,  750,  751,  753,  754,  755,
 /*   230 */   756,  757,  758,  759,  796,  760,  796,  761,  796,  777,
 /*   240 */   796,  796,  779,  784,  785,  780,  785,  788,  786,  778,
 /*   250 */   796,  762,  796,  763,  796,  764,  796,  771,  772,  773,
 /*   260 */   774,  775,  796,  769,  770,  796,  737,  796,  796,  767,
 /*   270 */   768,  738,  796,  796,  739,  796,  796,  765,  766,  796,
 /*   280 */   793,  726,  794,  796,  796,  796,  796,  796,  796,  667,
 /*   290 */   796,  796,  671,  796,  672,  796,  674,  675,  676,  677,
 /*   300 */   678,  679,  796,  680,  722,  796,  796,  796,  681,  796,
 /*   310 */   796,  796,  682,  796,  796,  796,  683,  796,  689,  690,
 /*   320 */   796,  796,  796,  684,  796,  796,  796,  685,  796,  796,
 /*   330 */   796,  686,  796,  796,  796,  687,  796,  796,  796,  688,
 /*   340 */   691,  692,  673,  796,  796,  694,  796,  695,  697,  696,
 /*   350 */   796,  698,  796,  699,  796,  700,  796,  796,  701,  702,
 /*   360 */   796,  796,  796,  796,  703,  796,  796,  796,  704,  796,
 /*   370 */   796,  705,  796,  796,  706,  796,  796,  707,  796,  796,
 /*   380 */   708,  796,  718,  720,  796,  721,  719,  796,  796,  709,
 /*   390 */   796,  796,  710,  796,  796,  711,  796,  796,  712,  796,
 /*   400 */   796,  713,  796,  796,  714,  796,  796,  715,  796,  796,
 /*   410 */   716,  796,  796,  717,  796,  796,  796,  796,  796,  795,
 /*   420 */   627,  796,  796,  796,  796,  668,  796,  796,  796,  796,
 /*   430 */   796,  796,  669,  796,  796,  796,  796,  670,  796,  796,
 /*   440 */   796,  665,  663,  661,  796,  796,  660,  657,  652,  648,
 /*   450 */   796,  796,  796,  653,  649,  796,  796,  796,  585,  796,
 /*   460 */   587,  796,  796,  796,  796,  796,  796,  796,  588,  796,
 /*   470 */   590,  796,  796,  796,  589,  796,  591,  796,  796,  576,
 /*   480 */   579,  796,  796,  796,  796,  796,  582,  796,  796,  796,
 /*   490 */   796,  583,  570,  796,  796,  796,  572,  796,  796,  574,
 /*   500 */   796,  796,  573,  796,  796,  575,  577,  796,  796,  796,
 /*   510 */   796,  796,  571,  796,  796,  796,  549,  796,  556,  558,
 /*   520 */   557,  796,  796,  796,  550,  796,  796,  551,  796,  796,
 /*   530 */   796,  552,  796,  796,  553,  539,
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
 /* 248 */ "xx_array_value ::= xx_literal_expr",
 /* 249 */ "xx_array_value ::= IDENTIFIER",
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
// 896 "parser.lemon"
{
	/*if ((yypminor->yy0)) {
		if ((yypminor->yy0)->free_flag) {
			efree((yypminor->yy0)->token);
		}
		efree((yypminor->yy0));
	}*/
}
// 2304 "parser.c"
      break;
    case 99:
// 909 "parser.lemon"
{ json_object_put((yypminor->yy175)); }
// 2309 "parser.c"
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
// 905 "parser.lemon"
{
	status->ret = yymsp[0].minor.yy175;
}
// 2786 "parser.c"
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
      case 258:
// 911 "parser.lemon"
{
	yygotominor.yy175 = yymsp[0].minor.yy175;
}
// 2823 "parser.c"
        break;
      case 2:
      case 34:
      case 40:
      case 42:
      case 44:
      case 60:
      case 90:
      case 120:
// 915 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_list(yymsp[-1].minor.yy175, yymsp[0].minor.yy175);
}
// 2837 "parser.c"
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
// 919 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_list(NULL, yymsp[0].minor.yy175);
}
// 2858 "parser.c"
        break;
      case 8:
// 939 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_namespace(yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(31,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 2867 "parser.c"
        break;
      case 9:
// 943 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_interface(yymsp[-1].minor.yy0, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(34,&yymsp[-2].minor);
}
// 2875 "parser.c"
        break;
      case 10:
// 947 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_interface(yymsp[-3].minor.yy0, yymsp[0].minor.yy175, yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(34,&yymsp[-4].minor);
  yy_destructor(35,&yymsp[-2].minor);
}
// 2884 "parser.c"
        break;
      case 11:
// 951 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class(yymsp[-1].minor.yy0, yymsp[0].minor.yy175, 0, 0, NULL, NULL, status->scanner_state);
  yy_destructor(36,&yymsp[-2].minor);
}
// 2892 "parser.c"
        break;
      case 12:
// 955 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy175, 0, 0, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(36,&yymsp[-4].minor);
  yy_destructor(35,&yymsp[-2].minor);
}
// 2901 "parser.c"
        break;
      case 13:
// 959 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy175, 0, 0, NULL, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(36,&yymsp[-4].minor);
  yy_destructor(37,&yymsp[-2].minor);
}
// 2910 "parser.c"
        break;
      case 14:
// 963 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class(yymsp[-1].minor.yy0, yymsp[0].minor.yy175, 1, 0, NULL, NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-3].minor);
  yy_destructor(36,&yymsp[-2].minor);
}
// 2919 "parser.c"
        break;
      case 15:
// 967 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy175, 1, 0, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-5].minor);
  yy_destructor(36,&yymsp[-4].minor);
  yy_destructor(35,&yymsp[-2].minor);
}
// 2929 "parser.c"
        break;
      case 16:
// 971 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class(yymsp[-1].minor.yy0, yymsp[0].minor.yy175, 0, 1, NULL, NULL, status->scanner_state);
  yy_destructor(39,&yymsp[-3].minor);
  yy_destructor(36,&yymsp[-2].minor);
}
// 2938 "parser.c"
        break;
      case 17:
// 975 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy175, 0, 1, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(39,&yymsp[-5].minor);
  yy_destructor(36,&yymsp[-4].minor);
  yy_destructor(35,&yymsp[-2].minor);
}
// 2948 "parser.c"
        break;
      case 18:
      case 23:
// 979 "parser.lemon"
{
	yygotominor.yy175 = NULL;
  yy_destructor(40,&yymsp[-1].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 2958 "parser.c"
        break;
      case 19:
      case 24:
// 983 "parser.lemon"
{
	yygotominor.yy175 = yymsp[-1].minor.yy175;
  yy_destructor(40,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 2968 "parser.c"
        break;
      case 20:
      case 70:
      case 136:
      case 159:
      case 182:
      case 238:
      case 241:
// 987 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_list(yymsp[-2].minor.yy175, yymsp[0].minor.yy175);
  yy_destructor(6,&yymsp[-1].minor);
}
// 2982 "parser.c"
        break;
      case 22:
      case 216:
      case 245:
      case 249:
// 995 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state);
}
// 2992 "parser.c"
        break;
      case 25:
// 1007 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_definition(yymsp[0].minor.yy175, NULL, NULL, status->scanner_state);
}
// 2999 "parser.c"
        break;
      case 26:
// 1011 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_definition(NULL, NULL, yymsp[0].minor.yy175, status->scanner_state);
}
// 3006 "parser.c"
        break;
      case 27:
// 1015 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_definition(NULL, yymsp[0].minor.yy175, NULL, status->scanner_state);
}
// 3013 "parser.c"
        break;
      case 28:
// 1019 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_definition(yymsp[-1].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
}
// 3020 "parser.c"
        break;
      case 29:
// 1023 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_definition(yymsp[-1].minor.yy175, NULL, yymsp[0].minor.yy175, status->scanner_state);
}
// 3027 "parser.c"
        break;
      case 30:
// 1027 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_definition(yymsp[0].minor.yy175, NULL, yymsp[-1].minor.yy175, status->scanner_state);
}
// 3034 "parser.c"
        break;
      case 31:
// 1031 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_definition(NULL, yymsp[0].minor.yy175, yymsp[-1].minor.yy175, status->scanner_state);
}
// 3041 "parser.c"
        break;
      case 32:
// 1035 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_definition(yymsp[-2].minor.yy175, yymsp[0].minor.yy175, yymsp[-1].minor.yy175, status->scanner_state);
}
// 3048 "parser.c"
        break;
      case 33:
// 1039 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_definition(yymsp[-1].minor.yy175, yymsp[0].minor.yy175, yymsp[-2].minor.yy175, status->scanner_state);
}
// 3055 "parser.c"
        break;
      case 36:
// 1055 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_property(yymsp[-2].minor.yy175, yymsp[-1].minor.yy0, NULL, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(33,&yymsp[0].minor);
}
// 3063 "parser.c"
        break;
      case 37:
// 1059 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_property(yymsp[-2].minor.yy175, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(33,&yymsp[0].minor);
}
// 3071 "parser.c"
        break;
      case 38:
// 1063 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_property(yymsp[-4].minor.yy175, yymsp[-3].minor.yy0, yymsp[-1].minor.yy175, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(43,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 3080 "parser.c"
        break;
      case 39:
// 1067 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_property(yymsp[-4].minor.yy175, yymsp[-3].minor.yy0, yymsp[-1].minor.yy175, NULL, status->scanner_state);
  yy_destructor(43,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 3089 "parser.c"
        break;
      case 46:
// 1095 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy175, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(44,&yymsp[-4].minor);
  yy_destructor(43,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 3099 "parser.c"
        break;
      case 47:
// 1099 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy175, NULL, status->scanner_state);
  yy_destructor(44,&yymsp[-4].minor);
  yy_destructor(43,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 3109 "parser.c"
        break;
      case 48:
// 1103 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(yymsp[-6].minor.yy175, yymsp[-4].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(46,&yymsp[-5].minor);
  yy_destructor(47,&yymsp[-3].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(40,&yymsp[-1].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3121 "parser.c"
        break;
      case 49:
// 1107 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(yymsp[-7].minor.yy175, yymsp[-5].minor.yy0, yymsp[-3].minor.yy175, NULL, NULL, status->scanner_state);
  yy_destructor(46,&yymsp[-6].minor);
  yy_destructor(47,&yymsp[-4].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(40,&yymsp[-1].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3133 "parser.c"
        break;
      case 50:
// 1111 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(yymsp[-7].minor.yy175, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy175, NULL, status->scanner_state);
  yy_destructor(46,&yymsp[-6].minor);
  yy_destructor(47,&yymsp[-4].minor);
  yy_destructor(30,&yymsp[-3].minor);
  yy_destructor(40,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3145 "parser.c"
        break;
      case 51:
// 1115 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(yymsp[-8].minor.yy175, yymsp[-6].minor.yy0, yymsp[-4].minor.yy175, yymsp[-1].minor.yy175, NULL, status->scanner_state);
  yy_destructor(46,&yymsp[-7].minor);
  yy_destructor(47,&yymsp[-5].minor);
  yy_destructor(30,&yymsp[-3].minor);
  yy_destructor(40,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3157 "parser.c"
        break;
      case 52:
// 1119 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(yymsp[-6].minor.yy175, yymsp[-4].minor.yy0, NULL, NULL, yymsp[-7].minor.yy0, status->scanner_state);
  yy_destructor(46,&yymsp[-5].minor);
  yy_destructor(47,&yymsp[-3].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(40,&yymsp[-1].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3169 "parser.c"
        break;
      case 53:
// 1123 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(yymsp[-7].minor.yy175, yymsp[-5].minor.yy0, yymsp[-3].minor.yy175, NULL, yymsp[-8].minor.yy0, status->scanner_state);
  yy_destructor(46,&yymsp[-6].minor);
  yy_destructor(47,&yymsp[-4].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(40,&yymsp[-1].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3181 "parser.c"
        break;
      case 54:
// 1127 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(yymsp[-7].minor.yy175, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy175, yymsp[-8].minor.yy0, status->scanner_state);
  yy_destructor(46,&yymsp[-6].minor);
  yy_destructor(47,&yymsp[-4].minor);
  yy_destructor(30,&yymsp[-3].minor);
  yy_destructor(40,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3193 "parser.c"
        break;
      case 55:
// 1131 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(yymsp[-8].minor.yy175, yymsp[-6].minor.yy0, yymsp[-4].minor.yy175, yymsp[-1].minor.yy175, yymsp[-9].minor.yy0, status->scanner_state);
  yy_destructor(46,&yymsp[-7].minor);
  yy_destructor(47,&yymsp[-5].minor);
  yy_destructor(30,&yymsp[-3].minor);
  yy_destructor(40,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3205 "parser.c"
        break;
      case 56:
// 1135 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(yymsp[-5].minor.yy175, yymsp[-3].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(46,&yymsp[-4].minor);
  yy_destructor(47,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[-1].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 3216 "parser.c"
        break;
      case 57:
// 1139 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(yymsp[-6].minor.yy175, yymsp[-4].minor.yy0, yymsp[-2].minor.yy175, NULL, NULL, status->scanner_state);
  yy_destructor(46,&yymsp[-5].minor);
  yy_destructor(47,&yymsp[-3].minor);
  yy_destructor(30,&yymsp[-1].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 3227 "parser.c"
        break;
      case 58:
// 1143 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(yymsp[-5].minor.yy175, yymsp[-3].minor.yy0, NULL, NULL, yymsp[-6].minor.yy0, status->scanner_state);
  yy_destructor(46,&yymsp[-4].minor);
  yy_destructor(47,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[-1].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 3238 "parser.c"
        break;
      case 59:
// 1147 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(yymsp[-6].minor.yy175, yymsp[-4].minor.yy0, yymsp[-2].minor.yy175, NULL, yymsp[-7].minor.yy0, status->scanner_state);
  yy_destructor(46,&yymsp[-5].minor);
  yy_destructor(47,&yymsp[-3].minor);
  yy_destructor(30,&yymsp[-1].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 3249 "parser.c"
        break;
      case 62:
// 1159 "parser.lemon"
{
	yygotominor.yy175 = json_object_new_string("public");
  yy_destructor(1,&yymsp[0].minor);
}
// 3257 "parser.c"
        break;
      case 63:
// 1163 "parser.lemon"
{
	yygotominor.yy175 = json_object_new_string("protected");
  yy_destructor(2,&yymsp[0].minor);
}
// 3265 "parser.c"
        break;
      case 64:
// 1167 "parser.lemon"
{
	yygotominor.yy175 = json_object_new_string("private");
  yy_destructor(4,&yymsp[0].minor);
}
// 3273 "parser.c"
        break;
      case 65:
// 1171 "parser.lemon"
{
	yygotominor.yy175 = json_object_new_string("static");
  yy_destructor(3,&yymsp[0].minor);
}
// 3281 "parser.c"
        break;
      case 66:
// 1175 "parser.lemon"
{
	yygotominor.yy175 = json_object_new_string("scoped");
  yy_destructor(5,&yymsp[0].minor);
}
// 3289 "parser.c"
        break;
      case 67:
// 1179 "parser.lemon"
{
	yygotominor.yy175 = json_object_new_string("inline");
  yy_destructor(48,&yymsp[0].minor);
}
// 3297 "parser.c"
        break;
      case 68:
// 1183 "parser.lemon"
{
	yygotominor.yy175 = json_object_new_string("abstract");
  yy_destructor(38,&yymsp[0].minor);
}
// 3305 "parser.c"
        break;
      case 69:
// 1187 "parser.lemon"
{
	yygotominor.yy175 = json_object_new_string("final");
  yy_destructor(39,&yymsp[0].minor);
}
// 3313 "parser.c"
        break;
      case 72:
// 1199 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_parameter(NULL, NULL, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 3320 "parser.c"
        break;
      case 73:
// 1203 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_parameter(yymsp[-1].minor.yy175, NULL, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 3327 "parser.c"
        break;
      case 74:
// 1207 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_parameter(NULL, yymsp[-1].minor.yy175, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 3334 "parser.c"
        break;
      case 75:
// 1211 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_parameter(NULL, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(43,&yymsp[-1].minor);
}
// 3342 "parser.c"
        break;
      case 76:
// 1215 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_parameter(yymsp[-3].minor.yy175, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(43,&yymsp[-1].minor);
}
// 3350 "parser.c"
        break;
      case 77:
// 1219 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_parameter(NULL, yymsp[-3].minor.yy175, yymsp[-2].minor.yy0, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(43,&yymsp[-1].minor);
}
// 3358 "parser.c"
        break;
      case 78:
// 1223 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(12,&yymsp[-2].minor);
  yy_destructor(13,&yymsp[0].minor);
}
// 3367 "parser.c"
        break;
      case 79:
// 1227 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_type(XX_TYPE_INTEGER);
  yy_destructor(49,&yymsp[0].minor);
}
// 3375 "parser.c"
        break;
      case 80:
// 1231 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_type(XX_TYPE_UINTEGER);
  yy_destructor(50,&yymsp[0].minor);
}
// 3383 "parser.c"
        break;
      case 81:
// 1235 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_type(XX_TYPE_LONG);
  yy_destructor(51,&yymsp[0].minor);
}
// 3391 "parser.c"
        break;
      case 82:
// 1239 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_type(XX_TYPE_ULONG);
  yy_destructor(52,&yymsp[0].minor);
}
// 3399 "parser.c"
        break;
      case 83:
// 1243 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_type(XX_TYPE_CHAR);
  yy_destructor(53,&yymsp[0].minor);
}
// 3407 "parser.c"
        break;
      case 84:
// 1247 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_type(XX_TYPE_UCHAR);
  yy_destructor(54,&yymsp[0].minor);
}
// 3415 "parser.c"
        break;
      case 85:
// 1251 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_type(XX_TYPE_DOUBLE);
  yy_destructor(55,&yymsp[0].minor);
}
// 3423 "parser.c"
        break;
      case 86:
// 1255 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_type(XX_TYPE_BOOL);
  yy_destructor(56,&yymsp[0].minor);
}
// 3431 "parser.c"
        break;
      case 87:
// 1259 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_type(XX_TYPE_STRING);
  yy_destructor(57,&yymsp[0].minor);
}
// 3439 "parser.c"
        break;
      case 88:
// 1263 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_type(XX_TYPE_ARRAY);
  yy_destructor(58,&yymsp[0].minor);
}
// 3447 "parser.c"
        break;
      case 89:
// 1267 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_type(XX_TYPE_VAR);
  yy_destructor(59,&yymsp[0].minor);
}
// 3455 "parser.c"
        break;
      case 110:
// 1351 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_break_statement(status->scanner_state);
  yy_destructor(60,&yymsp[-1].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 3464 "parser.c"
        break;
      case 111:
// 1355 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_continue_statement(status->scanner_state);
  yy_destructor(61,&yymsp[-1].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 3473 "parser.c"
        break;
      case 112:
// 1359 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_if_statement(yymsp[-2].minor.yy175, NULL, NULL, status->scanner_state);
  yy_destructor(62,&yymsp[-3].minor);
  yy_destructor(40,&yymsp[-1].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3483 "parser.c"
        break;
      case 113:
// 1363 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_if_statement(yymsp[-5].minor.yy175, NULL, NULL, status->scanner_state);
  yy_destructor(62,&yymsp[-6].minor);
  yy_destructor(40,&yymsp[-4].minor);
  yy_destructor(41,&yymsp[-3].minor);
  yy_destructor(63,&yymsp[-2].minor);
  yy_destructor(40,&yymsp[-1].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3496 "parser.c"
        break;
      case 114:
// 1367 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_if_statement(yymsp[-3].minor.yy175, yymsp[-1].minor.yy175, NULL, status->scanner_state);
  yy_destructor(62,&yymsp[-4].minor);
  yy_destructor(40,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3506 "parser.c"
        break;
      case 115:
// 1371 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_if_statement(yymsp[-7].minor.yy175, yymsp[-5].minor.yy175, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(62,&yymsp[-8].minor);
  yy_destructor(40,&yymsp[-6].minor);
  yy_destructor(41,&yymsp[-4].minor);
  yy_destructor(63,&yymsp[-3].minor);
  yy_destructor(40,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3519 "parser.c"
        break;
      case 116:
// 1375 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_if_statement(yymsp[-6].minor.yy175, yymsp[-4].minor.yy175, NULL, status->scanner_state);
  yy_destructor(62,&yymsp[-7].minor);
  yy_destructor(40,&yymsp[-5].minor);
  yy_destructor(41,&yymsp[-3].minor);
  yy_destructor(63,&yymsp[-2].minor);
  yy_destructor(40,&yymsp[-1].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3532 "parser.c"
        break;
      case 117:
// 1379 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_if_statement(yymsp[-6].minor.yy175, NULL, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(62,&yymsp[-7].minor);
  yy_destructor(40,&yymsp[-5].minor);
  yy_destructor(41,&yymsp[-4].minor);
  yy_destructor(63,&yymsp[-3].minor);
  yy_destructor(40,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3545 "parser.c"
        break;
      case 118:
// 1383 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_switch_statement(yymsp[-2].minor.yy175, NULL, status->scanner_state);
  yy_destructor(64,&yymsp[-3].minor);
  yy_destructor(40,&yymsp[-1].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3555 "parser.c"
        break;
      case 119:
// 1387 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_switch_statement(yymsp[-3].minor.yy175, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(64,&yymsp[-4].minor);
  yy_destructor(40,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3565 "parser.c"
        break;
      case 122:
// 1399 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_case_clause(yymsp[-1].minor.yy175, NULL, status->scanner_state);
  yy_destructor(65,&yymsp[-2].minor);
  yy_destructor(66,&yymsp[0].minor);
}
// 3574 "parser.c"
        break;
      case 123:
// 1403 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_case_clause(yymsp[-2].minor.yy175, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(65,&yymsp[-3].minor);
  yy_destructor(66,&yymsp[-1].minor);
}
// 3583 "parser.c"
        break;
      case 124:
// 1407 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_case_clause(NULL, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(67,&yymsp[-2].minor);
  yy_destructor(66,&yymsp[-1].minor);
}
// 3592 "parser.c"
        break;
      case 125:
// 1411 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_loop_statement(NULL, status->scanner_state);
  yy_destructor(68,&yymsp[-2].minor);
  yy_destructor(40,&yymsp[-1].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3602 "parser.c"
        break;
      case 126:
// 1415 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_loop_statement(yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(68,&yymsp[-3].minor);
  yy_destructor(40,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3612 "parser.c"
        break;
      case 127:
// 1419 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_while_statement(yymsp[-2].minor.yy175, NULL, status->scanner_state);
  yy_destructor(69,&yymsp[-3].minor);
  yy_destructor(40,&yymsp[-1].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3622 "parser.c"
        break;
      case 128:
// 1423 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_while_statement(yymsp[-3].minor.yy175, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(69,&yymsp[-4].minor);
  yy_destructor(40,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3632 "parser.c"
        break;
      case 129:
// 1427 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_do_while_statement(yymsp[-1].minor.yy175, NULL, status->scanner_state);
  yy_destructor(70,&yymsp[-5].minor);
  yy_destructor(40,&yymsp[-4].minor);
  yy_destructor(41,&yymsp[-3].minor);
  yy_destructor(69,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 3644 "parser.c"
        break;
      case 130:
// 1431 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_do_while_statement(yymsp[-1].minor.yy175, yymsp[-4].minor.yy175, status->scanner_state);
  yy_destructor(70,&yymsp[-6].minor);
  yy_destructor(40,&yymsp[-5].minor);
  yy_destructor(41,&yymsp[-3].minor);
  yy_destructor(69,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 3656 "parser.c"
        break;
      case 131:
// 1435 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_for_statement(yymsp[-3].minor.yy175, NULL, yymsp[-5].minor.yy0, 0, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(71,&yymsp[-6].minor);
  yy_destructor(72,&yymsp[-4].minor);
  yy_destructor(40,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3667 "parser.c"
        break;
      case 132:
// 1439 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_for_statement(yymsp[-3].minor.yy175, NULL, yymsp[-6].minor.yy0, 1, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(71,&yymsp[-7].minor);
  yy_destructor(72,&yymsp[-5].minor);
  yy_destructor(73,&yymsp[-4].minor);
  yy_destructor(40,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3679 "parser.c"
        break;
      case 133:
// 1443 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_for_statement(yymsp[-3].minor.yy175, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, 0, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(71,&yymsp[-8].minor);
  yy_destructor(6,&yymsp[-6].minor);
  yy_destructor(72,&yymsp[-4].minor);
  yy_destructor(40,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3691 "parser.c"
        break;
      case 134:
// 1447 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_for_statement(yymsp[-3].minor.yy175, yymsp[-8].minor.yy0, yymsp[-6].minor.yy0, 1, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(71,&yymsp[-9].minor);
  yy_destructor(6,&yymsp[-7].minor);
  yy_destructor(72,&yymsp[-5].minor);
  yy_destructor(73,&yymsp[-4].minor);
  yy_destructor(40,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3704 "parser.c"
        break;
      case 135:
// 1451 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_statement(yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(74,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 3713 "parser.c"
        break;
      case 138:
// 1464 "parser.lemon"
{
	yygotominor.yy175 = json_object_new_string("assign");
  yy_destructor(43,&yymsp[0].minor);
}
// 3721 "parser.c"
        break;
      case 139:
// 1469 "parser.lemon"
{
	yygotominor.yy175 = json_object_new_string("add-assign");
  yy_destructor(75,&yymsp[0].minor);
}
// 3729 "parser.c"
        break;
      case 140:
// 1474 "parser.lemon"
{
	yygotominor.yy175 = json_object_new_string("sub-assign");
  yy_destructor(76,&yymsp[0].minor);
}
// 3737 "parser.c"
        break;
      case 141:
// 1478 "parser.lemon"
{
	yygotominor.yy175 = json_object_new_string("mult-assign");
  yy_destructor(77,&yymsp[0].minor);
}
// 3745 "parser.c"
        break;
      case 142:
// 1482 "parser.lemon"
{
	yygotominor.yy175 = json_object_new_string("div-assign");
  yy_destructor(78,&yymsp[0].minor);
}
// 3753 "parser.c"
        break;
      case 143:
// 1486 "parser.lemon"
{
	yygotominor.yy175 = json_object_new_string("concat-assign");
  yy_destructor(79,&yymsp[0].minor);
}
// 3761 "parser.c"
        break;
      case 144:
// 1491 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("variable", yymsp[-1].minor.yy175, yymsp[-2].minor.yy0, NULL, NULL, yymsp[0].minor.yy175, status->scanner_state);
}
// 3768 "parser.c"
        break;
      case 145:
// 1496 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("object-property", yymsp[-1].minor.yy175, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(80,&yymsp[-3].minor);
}
// 3776 "parser.c"
        break;
      case 146:
// 1501 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("object-property-append", yymsp[-1].minor.yy175, yymsp[-6].minor.yy0, yymsp[-4].minor.yy0, NULL, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(80,&yymsp[-5].minor);
  yy_destructor(81,&yymsp[-3].minor);
  yy_destructor(82,&yymsp[-2].minor);
}
// 3786 "parser.c"
        break;
      case 147:
// 1506 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("object-property-array-index", yymsp[-1].minor.yy175, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, yymsp[-3].minor.yy175, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(80,&yymsp[-6].minor);
  yy_destructor(81,&yymsp[-4].minor);
  yy_destructor(82,&yymsp[-2].minor);
}
// 3796 "parser.c"
        break;
      case 148:
// 1511 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("static-property", yymsp[-1].minor.yy175, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(83,&yymsp[-3].minor);
}
// 3804 "parser.c"
        break;
      case 149:
// 1516 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("static-property-append", yymsp[-1].minor.yy175, yymsp[-6].minor.yy0, yymsp[-4].minor.yy0, NULL, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(83,&yymsp[-5].minor);
  yy_destructor(81,&yymsp[-3].minor);
  yy_destructor(82,&yymsp[-2].minor);
}
// 3814 "parser.c"
        break;
      case 150:
// 1521 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("static-property-array-index", yymsp[-1].minor.yy175, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, yymsp[-3].minor.yy175, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(83,&yymsp[-6].minor);
  yy_destructor(81,&yymsp[-4].minor);
  yy_destructor(82,&yymsp[-2].minor);
}
// 3824 "parser.c"
        break;
      case 151:
// 1526 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("variable-append", yymsp[-1].minor.yy175, yymsp[-4].minor.yy0, NULL, NULL, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(81,&yymsp[-3].minor);
  yy_destructor(82,&yymsp[-2].minor);
}
// 3833 "parser.c"
        break;
      case 152:
// 1531 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("array-index", yymsp[-1].minor.yy175, yymsp[-5].minor.yy0, NULL, yymsp[-3].minor.yy175, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(81,&yymsp[-4].minor);
  yy_destructor(82,&yymsp[-2].minor);
}
// 3842 "parser.c"
        break;
      case 153:
// 1536 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("incr", NULL, yymsp[-3].minor.yy0, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(80,&yymsp[-2].minor);
  yy_destructor(84,&yymsp[0].minor);
}
// 3851 "parser.c"
        break;
      case 154:
// 1541 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("decr", NULL, yymsp[-3].minor.yy0, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(80,&yymsp[-2].minor);
  yy_destructor(85,&yymsp[0].minor);
}
// 3860 "parser.c"
        break;
      case 155:
// 1546 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("incr", NULL, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(84,&yymsp[0].minor);
}
// 3868 "parser.c"
        break;
      case 156:
// 1551 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("decr", NULL, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(85,&yymsp[0].minor);
}
// 3876 "parser.c"
        break;
      case 158:
// 1559 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_echo_statement(yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(86,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 3885 "parser.c"
        break;
      case 161:
// 1571 "parser.lemon"
{
	yygotominor.yy175 = yymsp[0].minor.yy175;;
}
// 3892 "parser.c"
        break;
      case 162:
// 1576 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_mcall_statement(yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(33,&yymsp[0].minor);
}
// 3900 "parser.c"
        break;
      case 163:
// 1581 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_fcall_statement(yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(33,&yymsp[0].minor);
}
// 3908 "parser.c"
        break;
      case 164:
// 1586 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_scall_statement(yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(33,&yymsp[0].minor);
}
// 3916 "parser.c"
        break;
      case 165:
// 1591 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_return_statement(yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(87,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 3925 "parser.c"
        break;
      case 166:
// 1596 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_return_statement(NULL, status->scanner_state);
  yy_destructor(87,&yymsp[-1].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 3934 "parser.c"
        break;
      case 167:
// 1601 "parser.lemon"
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
// 3948 "parser.c"
        break;
      case 168:
// 1610 "parser.lemon"
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
// 3963 "parser.c"
        break;
      case 169:
// 1619 "parser.lemon"
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
// 3978 "parser.c"
        break;
      case 170:
// 1628 "parser.lemon"
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
// 3993 "parser.c"
        break;
      case 171:
// 1637 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_throw_exception(yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(91,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 4002 "parser.c"
        break;
      case 172:
// 1641 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_statement(XX_T_TYPE_INTEGER, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 4011 "parser.c"
        break;
      case 173:
// 1645 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_statement(XX_T_TYPE_UINTEGER, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(50,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 4020 "parser.c"
        break;
      case 174:
// 1649 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_statement(XX_T_TYPE_CHAR, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(53,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 4029 "parser.c"
        break;
      case 175:
// 1653 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_statement(XX_T_TYPE_UCHAR, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 4038 "parser.c"
        break;
      case 176:
// 1657 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_statement(XX_T_TYPE_LONG, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(51,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 4047 "parser.c"
        break;
      case 177:
// 1661 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_statement(XX_T_TYPE_ULONG, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(52,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 4056 "parser.c"
        break;
      case 178:
// 1665 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_statement(XX_T_TYPE_DOUBLE, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(55,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 4065 "parser.c"
        break;
      case 179:
// 1669 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_statement(XX_T_TYPE_STRING, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(57,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 4074 "parser.c"
        break;
      case 180:
// 1673 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_statement(XX_T_TYPE_BOOL, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(56,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 4083 "parser.c"
        break;
      case 181:
// 1677 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_statement(XX_T_TYPE_VAR, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(59,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 4092 "parser.c"
        break;
      case 184:
// 1689 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_variable(yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 4099 "parser.c"
        break;
      case 185:
// 1693 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_variable(yymsp[-2].minor.yy0, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(43,&yymsp[-1].minor);
}
// 4107 "parser.c"
        break;
      case 187:
// 1701 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("not", yymsp[0].minor.yy175, NULL, NULL, status->scanner_state);
  yy_destructor(29,&yymsp[-1].minor);
}
// 4115 "parser.c"
        break;
      case 188:
// 1705 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("isset", yymsp[0].minor.yy175, NULL, NULL, status->scanner_state);
  yy_destructor(24,&yymsp[-1].minor);
}
// 4123 "parser.c"
        break;
      case 189:
// 1709 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("require", yymsp[0].minor.yy175, NULL, NULL, status->scanner_state);
  yy_destructor(7,&yymsp[-1].minor);
}
// 4131 "parser.c"
        break;
      case 190:
// 1713 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("equals", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(10,&yymsp[-1].minor);
}
// 4139 "parser.c"
        break;
      case 191:
// 1717 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("not-equals", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(17,&yymsp[-1].minor);
}
// 4147 "parser.c"
        break;
      case 192:
// 1721 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("identical", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(11,&yymsp[-1].minor);
}
// 4155 "parser.c"
        break;
      case 193:
// 1725 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("not-identical", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(16,&yymsp[-1].minor);
}
// 4163 "parser.c"
        break;
      case 194:
// 1729 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("less", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(12,&yymsp[-1].minor);
}
// 4171 "parser.c"
        break;
      case 195:
// 1733 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("greater", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(13,&yymsp[-1].minor);
}
// 4179 "parser.c"
        break;
      case 196:
// 1737 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("less-equal", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(14,&yymsp[-1].minor);
}
// 4187 "parser.c"
        break;
      case 197:
// 1741 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("greater-equal", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(15,&yymsp[-1].minor);
}
// 4195 "parser.c"
        break;
      case 198:
// 1745 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("list", yymsp[-1].minor.yy175, NULL, NULL, status->scanner_state);
  yy_destructor(47,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 4204 "parser.c"
        break;
      case 199:
// 1749 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("cast", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(47,&yymsp[-3].minor);
  yy_destructor(30,&yymsp[-1].minor);
}
// 4213 "parser.c"
        break;
      case 200:
// 1753 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("property-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(80,&yymsp[-1].minor);
}
// 4221 "parser.c"
        break;
      case 201:
// 1757 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("static-property-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(83,&yymsp[-1].minor);
}
// 4229 "parser.c"
        break;
      case 202:
// 1761 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("static-constant-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(83,&yymsp[-1].minor);
}
// 4237 "parser.c"
        break;
      case 203:
      case 257:
// 1766 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), yymsp[-1].minor.yy175, NULL, status->scanner_state);
  yy_destructor(81,&yymsp[-2].minor);
  yy_destructor(82,&yymsp[0].minor);
}
// 4247 "parser.c"
        break;
      case 204:
// 1771 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("add", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(18,&yymsp[-1].minor);
}
// 4255 "parser.c"
        break;
      case 205:
// 1776 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("sub", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(19,&yymsp[-1].minor);
}
// 4263 "parser.c"
        break;
      case 206:
// 1781 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("mul", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(21,&yymsp[-1].minor);
}
// 4271 "parser.c"
        break;
      case 207:
// 1786 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("div", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(22,&yymsp[-1].minor);
}
// 4279 "parser.c"
        break;
      case 208:
// 1791 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("mod", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(23,&yymsp[-1].minor);
}
// 4287 "parser.c"
        break;
      case 209:
// 1796 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("concat", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(20,&yymsp[-1].minor);
}
// 4295 "parser.c"
        break;
      case 210:
// 1801 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("and", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(8,&yymsp[-1].minor);
}
// 4303 "parser.c"
        break;
      case 211:
// 1806 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("or", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(9,&yymsp[-1].minor);
}
// 4311 "parser.c"
        break;
      case 212:
// 1811 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("instanceof", yymsp[0].minor.yy175, NULL, NULL, status->scanner_state);
  yy_destructor(27,&yymsp[-1].minor);
}
// 4319 "parser.c"
        break;
      case 213:
// 1816 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("fetch", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(25,&yymsp[-3].minor);
  yy_destructor(6,&yymsp[-1].minor);
}
// 4328 "parser.c"
        break;
      case 214:
// 1821 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("typeof", yymsp[0].minor.yy175, NULL, NULL, status->scanner_state);
  yy_destructor(26,&yymsp[-1].minor);
}
// 4336 "parser.c"
        break;
      case 215:
// 1826 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_literal(XX_T_CONSTANT, yymsp[0].minor.yy0, status->scanner_state);
}
// 4343 "parser.c"
        break;
      case 217:
      case 247:
      case 250:
// 1836 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_literal(XX_T_INTEGER, yymsp[0].minor.yy0, status->scanner_state);
}
// 4352 "parser.c"
        break;
      case 218:
      case 246:
      case 252:
// 1841 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_literal(XX_T_STRING, yymsp[0].minor.yy0, status->scanner_state);
}
// 4361 "parser.c"
        break;
      case 219:
      case 251:
// 1846 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_literal(XX_T_CHAR, yymsp[0].minor.yy0, status->scanner_state);
}
// 4369 "parser.c"
        break;
      case 220:
      case 253:
// 1851 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_literal(XX_T_DOUBLE, yymsp[0].minor.yy0, status->scanner_state);
}
// 4377 "parser.c"
        break;
      case 221:
      case 254:
// 1856 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_literal(XX_T_NULL, NULL, status->scanner_state);
  yy_destructor(94,&yymsp[0].minor);
}
// 4386 "parser.c"
        break;
      case 222:
      case 256:
// 1861 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_literal(XX_T_TRUE, NULL, status->scanner_state);
  yy_destructor(95,&yymsp[0].minor);
}
// 4395 "parser.c"
        break;
      case 223:
      case 255:
// 1866 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_literal(XX_T_FALSE, NULL, status->scanner_state);
  yy_destructor(96,&yymsp[0].minor);
}
// 4404 "parser.c"
        break;
      case 224:
// 1871 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("empty-array", NULL, NULL, NULL, status->scanner_state);
  yy_destructor(81,&yymsp[-1].minor);
  yy_destructor(82,&yymsp[0].minor);
}
// 4413 "parser.c"
        break;
      case 225:
// 1876 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("array", yymsp[-1].minor.yy175, NULL, NULL, status->scanner_state);
  yy_destructor(81,&yymsp[-2].minor);
  yy_destructor(82,&yymsp[0].minor);
}
// 4422 "parser.c"
        break;
      case 226:
// 1881 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_new_instance(yymsp[0].minor.yy0, NULL, status->scanner_state);
  yy_destructor(28,&yymsp[-1].minor);
}
// 4430 "parser.c"
        break;
      case 227:
// 1886 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_new_instance(yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(28,&yymsp[-3].minor);
  yy_destructor(47,&yymsp[-1].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 4440 "parser.c"
        break;
      case 228:
// 1891 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_new_instance(yymsp[-3].minor.yy0, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(28,&yymsp[-4].minor);
  yy_destructor(47,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 4450 "parser.c"
        break;
      case 229:
// 1896 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_fcall(yymsp[-3].minor.yy0, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(47,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 4459 "parser.c"
        break;
      case 230:
// 1901 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_fcall(yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(47,&yymsp[-1].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 4468 "parser.c"
        break;
      case 231:
// 1906 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_scall(yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(83,&yymsp[-4].minor);
  yy_destructor(47,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 4478 "parser.c"
        break;
      case 232:
// 1911 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_scall(yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(83,&yymsp[-3].minor);
  yy_destructor(47,&yymsp[-1].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 4488 "parser.c"
        break;
      case 233:
// 1916 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_mcall(yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(80,&yymsp[-4].minor);
  yy_destructor(47,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 4498 "parser.c"
        break;
      case 234:
// 1921 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_mcall(yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(80,&yymsp[-3].minor);
  yy_destructor(47,&yymsp[-1].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 4508 "parser.c"
        break;
      case 243:
// 1960 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_array_item(yymsp[-2].minor.yy175, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(66,&yymsp[-1].minor);
}
// 4516 "parser.c"
        break;
      case 244:
// 1964 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_array_item(NULL, yymsp[0].minor.yy175, status->scanner_state);
}
// 4523 "parser.c"
        break;
      case 259:
// 2025 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_comment(yymsp[0].minor.yy0, status->scanner_state);
}
// 4530 "parser.c"
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
// 862 "parser.lemon"


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

// 4606 "parser.c"
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
