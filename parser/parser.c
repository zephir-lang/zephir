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
 /*     0 */   174,  169,  178,  182,  184,  186,  188,  180,  176,  190,
 /*    10 */   192,  200,  194,  196,  198,  143,  164,  170,  219,  223,
 /*    20 */   217,  249,  167,   94,  172,  212,   96,  506,  512,  511,
 /*    30 */   499,  454,  505,  479,   64,   42,   46,   84,  225,  147,
 /*    40 */   206,  443,   53,   54,   55,   56,   57,   58,   59,   60,
 /*    50 */    61,   62,   63,  202,  204,  169,  178,  182,  184,  186,
 /*    60 */   188,  180,  176,  190,  192,  200,  194,  196,  198,  181,
 /*    70 */   476,   97,  491,  499,  233,  505,  479,  256,  258,  257,
 /*    80 */    84,  413,  226,  227,  265,  228,  229,  230,  231,  232,
 /*    90 */   475,  418,   66,   67,   69,   68,   70,  270,  377,  386,
 /*   100 */   395,  398,  389,  392,  401,  407,  404,  245,  410,  127,
 /*   110 */   129,  131,  166,  140,  402,  385,   19,  151,  155,  160,
 /*   120 */   282,   22,  237,  289,  413,   85,  171,  279,   84,   72,
 /*   130 */    73,   31,  173,  107,  418,  342,  355,  359,  104,   71,
 /*   140 */   374,  377,  386,  395,  398,  389,  392,  401,  407,  404,
 /*   150 */   243,  410,  127,  129,  131,   29,  140,  234,  365,  425,
 /*   160 */   151,  155,  160,  282,  247,  246,  289,  413,   38,   39,
 /*   170 */    41,   40,   32,   42,   46,   75,  139,  418,  342,  355,
 /*   180 */   359,  102,   84,  374,  377,  386,  395,  398,  389,  392,
 /*   190 */   401,  407,  404,  245,  410,  127,  129,  131,  236,  140,
 /*   200 */   194,  196,  198,  151,  155,  160,  282,   36,   37,  289,
 /*   210 */   413,   38,   39,   41,   40,  368,  371,  144,  446,  436,
 /*   220 */   418,  342,  355,  359,  101,  284,  374,  377,  386,  395,
 /*   230 */   398,  389,  392,  401,  407,  404,   14,  410,  127,  129,
 /*   240 */   131,   17,  140,   65,   31,   95,  151,  155,  160,  282,
 /*   250 */   247,  246,  289,  413,   38,   39,   41,   40,  290,  341,
 /*   260 */   242,  274,  447,  418,  342,  355,  359,  295,   82,  374,
 /*   270 */   377,  386,  395,  398,  389,  392,  401,  407,  404,  461,
 /*   280 */   410,  127,  129,  131,   91,  140,   84,  255,  210,  151,
 /*   290 */   155,  160,  282,  516,  416,  289,  413,  414,  255,  296,
 /*   300 */   297,  298,  299,  300,  292,  424,  418,  342,  355,  359,
 /*   310 */   241,  254,  374,  377,  386,  395,  398,  389,  392,  401,
 /*   320 */   407,  404,  262,  410,  127,  129,  131,   92,  140,  255,
 /*   330 */   255,  291,  151,  155,  160,  282,  361,  364,  289,  413,
 /*   340 */   190,  192,  200,  194,  196,  198,  345,  380,  453,  418,
 /*   350 */   342,  355,  359,  268,  276,  374,  377,  386,  395,  398,
 /*   360 */   389,  392,  401,  407,  404,  493,  410,  127,  129,  131,
 /*   370 */   380,  140,   84,  344,  379,  151,  155,  160,  282,   36,
 /*   380 */    37,  289,  413,   38,   39,   41,   40,  378,  385,  387,
 /*   390 */   385,  431,  418,  342,  355,  359,  380,  388,  374,  377,
 /*   400 */   386,  395,  398,  389,  392,  401,  407,  404,   92,  410,
 /*   410 */   127,  129,  131,  380,  140,   90,  390,  385,  151,  155,
 /*   420 */   160,  282,  347,  391,  289,  413,  393,  385,  343,  348,
 /*   430 */   256,  258,  257,  380,  452,  418,  342,  355,  359,  380,
 /*   440 */   394,  374,  377,  386,  395,  398,  389,  392,  401,  407,
 /*   450 */   404,  214,  410,  127,  129,  131,  380,  140,  396,  385,
 /*   460 */   397,  151,  155,  160,  282,  303,  400,  289,  413,  315,
 /*   470 */   399,  385,  380,  256,  258,  257,  380,  288,  418,  342,
 /*   480 */   355,  359,  380,  403,  374,  377,  386,  395,  398,  389,
 /*   490 */   392,  401,  407,  404,  215,  410,  127,  129,  131,  406,
 /*   500 */   140,  405,  385,  409,  151,  155,  160,  282,  303,  412,
 /*   510 */   289,  413,  326,  408,  385,  495,  256,  258,  257,   88,
 /*   520 */   135,  418,  342,  355,  359,  496,   87,  374,  377,  386,
 /*   530 */   395,  398,  389,  392,  401,  407,  404,  524,  410,  127,
 /*   540 */   129,  131,   92,  140,  411,  385,  220,  151,  155,  160,
 /*   550 */   282,  347,   81,  289,  413,   64,   42,   46,  346,  256,
 /*   560 */   258,  257,  221,  457,  418,  342,  355,  359,  250,  222,
 /*   570 */   374,  377,  386,  395,  398,  389,  392,  401,  407,  404,
 /*   580 */   251,  410,  127,  129,  131,  501,  140,  240,   31,  780,
 /*   590 */   151,  155,  160,  282,  303,  502,  289,  413,  334,   31,
 /*   600 */   781,  782,  256,  258,  257,   83,  448,  418,  342,  355,
 /*   610 */   359,  515,  455,  374,  377,  386,  395,  398,  389,  392,
 /*   620 */   401,  407,  404,  471,  410,  127,  129,  131,  266,  140,
 /*   630 */    78,   80,  531,  151,  155,  160,  282,   92,  216,  289,
 /*   640 */   413,    3,    4,    5,    6,    7,  256,  258,  257,  442,
 /*   650 */   418,  342,  355,  359,  259,  518,  374,  377,  386,  395,
 /*   660 */   398,  389,  392,  401,  407,  404,   77,  410,  127,  129,
 /*   670 */   131,  517,  140,  283,  294,  293,  151,  155,  160,  282,
 /*   680 */   303,  301,  289,  413,  302,  514,  305,  519,  256,  258,
 /*   690 */   257,  313,  459,  418,  342,  355,  359,  306,  310,  374,
 /*   700 */   377,  386,  395,  398,  389,  392,  401,  407,  404,  314,
 /*   710 */   410,  127,  129,  131,  320,  140,  613,  321,  325,  151,
 /*   720 */   155,  160,  282,  316,  329,  289,  413,   99,  327,  328,
 /*   730 */    98,  256,  258,  257,   84,  154,  418,  342,  355,  359,
 /*   740 */   333,  336,  374,  377,  386,  395,  398,  389,  392,  401,
 /*   750 */   407,  404,  337,  410,  127,  129,  131,   52,  140,   51,
 /*   760 */    49,  350,  151,  155,  160,  282,  303,  352,  289,  413,
 /*   770 */   330,  354,   48,  360,  256,  258,  257,  362,  441,  418,
 /*   780 */   342,  355,  359,  363,  366,  374,  377,  386,  395,  398,
 /*   790 */   389,  392,  401,  407,  404,  367,  410,  127,  129,  131,
 /*   800 */   369,  140,  370,  372,  373,  151,  155,  160,  282,  303,
 /*   810 */    47,  289,  413,  307,  382,  381,  383,  256,  258,  257,
 /*   820 */   521,  467,  418,  342,  355,  359,   45,  384,  374,  377,
 /*   830 */   386,  395,  398,  389,  392,  401,  407,  404,  522,  410,
 /*   840 */   127,  129,  131,  523,  140,  525,  528,  526,  151,  155,
 /*   850 */   160,  282,  316,  529,  289,  413,  242,  312,   44,  415,
 /*   860 */   256,  258,  257,  417,  159,  418,  342,  355,  359,   43,
 /*   870 */   426,  374,  377,  386,  395,  398,  389,  392,  401,  407,
 /*   880 */   404,  427,  410,  127,  129,  131,   34,  140,  438,   35,
 /*   890 */   440,  151,  155,  160,  282,   22,  444,  289,  413,   21,
 /*   900 */    30,  235,   84,  449,  248,  239,  244,  105,  418,  342,
 /*   910 */   355,  359,  450,   27,  374,  377,  386,  395,  398,  389,
 /*   920 */   392,  401,  407,  404,  456,  410,  127,  129,  131,   25,
 /*   930 */   140,   24,  530,  463,  151,  155,  160,  282,  303,  464,
 /*   940 */   289,  413,  311,  466,  472,   16,  256,  258,  257,   15,
 /*   950 */   437,  418,  342,  355,  359,   17,   13,  374,  377,  386,
 /*   960 */   395,  398,  389,  392,  401,  407,  404,   12,  410,  127,
 /*   970 */   129,  131,  482,  140,  483,  484,  485,  151,  155,  160,
 /*   980 */   282,  303,  487,  289,  413,  338,  489,  488,  490,  256,
 /*   990 */   258,  257,  532,  469,  418,  342,  355,  359,   10,  498,
 /*  1000 */   374,  377,  386,  395,  398,  389,  392,  401,  407,  404,
 /*  1010 */   497,  410,  127,  129,  131,    9,  140,  503,  504,  533,
 /*  1020 */   151,  155,  160,  282,  316,  535,  289,  413,  142,  335,
 /*  1030 */   146,  148,  256,  258,  257,  149,  473,  418,  342,  355,
 /*  1040 */   359,  137,  136,  374,  377,  386,  395,  398,  389,  392,
 /*  1050 */   401,  407,  404,  152,  410,  127,  129,  131,  133,  140,
 /*  1060 */   130,  157,  128,  151,  155,  160,  282,  303,  161,  289,
 /*  1070 */   413,  322,  429,  429,  429,  256,  258,  257,  429,  163,
 /*  1080 */   418,  342,  355,  359,  274,  429,  374,  377,  386,  395,
 /*  1090 */   398,  389,  392,  401,  407,  404,  429,  410,  127,  129,
 /*  1100 */   131,  429,  140,  429,  429,  429,  151,  155,  160,  282,
 /*  1110 */   429,   28,  289,  413,   64,   42,   46,  213,  271,   50,
 /*  1120 */   264,  429,  429,  418,  342,  355,  359,  429,  429,  374,
 /*  1130 */   377,  386,  395,  398,  389,  392,  401,  407,  404,   33,
 /*  1140 */   410,  127,  129,  131,  429,  140,  429,  429,  429,  151,
 /*  1150 */   155,  160,  282,  126,  429,  289,   53,   54,   55,   56,
 /*  1160 */    57,   58,   59,   60,   61,   62,   63,  342,  355,  359,
 /*  1170 */   134,  429,  374,  429,  429,  419,  109,  110,  111,  112,
 /*  1180 */   113,  114,  115,  116,  117,  118,  119,  120,  121,  122,
 /*  1190 */   123,  124,  125,  126,  795,    1,    2,  534,    4,    5,
 /*  1200 */     6,    7,  429,  195,  349,  351,  353,  429,  429,  429,
 /*  1210 */   287,  256,  258,  257,  429,  419,  109,  110,  111,  112,
 /*  1220 */   113,  114,  115,  116,  117,  118,  119,  120,  121,  122,
 /*  1230 */   123,  124,  125,  242,  477,  126,   99,  429,  478,  479,
 /*  1240 */   429,  429,  429,   84,  349,  351,  353,   66,   67,   69,
 /*  1250 */    68,   70,  106,  145,  429,  429,  429,  419,  109,  110,
 /*  1260 */   111,  112,  113,  114,  115,  116,  117,  118,  119,  120,
 /*  1270 */   121,  122,  123,  124,  125,  126,  429,  147,  429,  443,
 /*  1280 */   429,  238,  239,  244,   72,   73,  349,  351,  353,  429,
 /*  1290 */   481,  429,  162,  429,   71,  429,  429,  419,  109,  110,
 /*  1300 */   111,  112,  113,  114,  115,  116,  117,  118,  119,  120,
 /*  1310 */   121,  122,  123,  124,  125,  429,  126,  429,  507,  429,
 /*  1320 */   510,  511,  499,  429,  478,  479,  349,  351,  353,   84,
 /*  1330 */    89,  429,  513,  451,  429,   92,  429,  429,  419,  109,
 /*  1340 */   110,  111,  112,  113,  114,  115,  116,  117,  118,  119,
 /*  1350 */   120,  121,  122,  123,  124,  125,  429,  429,  126,  429,
 /*  1360 */   429,  156,  429,  429,  281,  429,  429,  349,  351,  353,
 /*  1370 */   295,  429,  256,  258,  257,  150,  429,  429,  429,  429,
 /*  1380 */   419,  109,  110,  111,  112,  113,  114,  115,  116,  117,
 /*  1390 */   118,  119,  120,  121,  122,  123,  124,  125,  126,  429,
 /*  1400 */   429,  429,  296,  297,  298,  299,  300,  429,  323,  349,
 /*  1410 */   351,  353,  429,  429,  429,  474,  429,  429,  429,  429,
 /*  1420 */   419,  109,  110,  111,  112,  113,  114,  115,  116,  117,
 /*  1430 */   118,  119,  120,  121,  122,  123,  124,  125,  429,  126,
 /*  1440 */   429,  429,  429,  508,  491,  499,  429,  429,  479,  349,
 /*  1450 */   351,  353,   84,  429,  429,  429,  430,  429,  429,  429,
 /*  1460 */   429,  419,  109,  110,  111,  112,  113,  114,  115,  116,
 /*  1470 */   117,  118,  119,  120,  121,  122,  123,  124,  125,  429,
 /*  1480 */   429,  126,  429,  429,  132,  429,  429,  281,  429,  429,
 /*  1490 */   349,  351,  353,  429,  429,  256,  258,  257,  445,  429,
 /*  1500 */   429,  429,  429,  419,  109,  110,  111,  112,  113,  114,
 /*  1510 */   115,  116,  117,  118,  119,  120,  121,  122,  123,  124,
 /*  1520 */   125,  126,  429,  429,  429,  429,  429,  429,  141,  429,
 /*  1530 */   429,  281,  349,  351,  353,  429,  429,  429,  435,  256,
 /*  1540 */   258,  257,  429,  419,  109,  110,  111,  112,  113,  114,
 /*  1550 */   115,  116,  117,  118,  119,  120,  121,  122,  123,  124,
 /*  1560 */   125,  470,  126,  429,   64,   42,   46,  429,  429,  429,
 /*  1570 */   429,  429,  349,  351,  353,  429,  429,  429,  429,  138,
 /*  1580 */   429,  429,  429,  429,  419,  109,  110,  111,  112,  113,
 /*  1590 */   114,  115,  116,  117,  118,  119,  120,  121,  122,  123,
 /*  1600 */   124,  125,  429,  429,  126,  429,  429,  439,  429,  429,
 /*  1610 */   281,  429,  429,  349,  351,  353,  429,  429,  256,  258,
 /*  1620 */   257,  423,  429,  429,  429,  429,  419,  109,  110,  111,
 /*  1630 */   112,  113,  114,  115,  116,  117,  118,  119,  120,  121,
 /*  1640 */   122,  123,  124,  125,  126,  429,  429,  429,  429,  429,
 /*  1650 */   429,  165,  429,  429,  281,  349,  351,  353,  429,  429,
 /*  1660 */   429,  468,  256,  258,  257,  429,  419,  109,  110,  111,
 /*  1670 */   112,  113,  114,  115,  116,  117,  118,  119,  120,  121,
 /*  1680 */   122,  123,  124,  125,  429,  126,  429,  429,  429,  429,
 /*  1690 */   193,  429,  429,  429,  429,  349,  351,  353,  256,  258,
 /*  1700 */   257,  429,  153,  429,  429,  429,  429,  419,  109,  110,
 /*  1710 */   111,  112,  113,  114,  115,  116,  117,  118,  119,  120,
 /*  1720 */   121,  122,  123,  124,  125,  429,  429,  126,  429,  429,
 /*  1730 */   429,  429,  429,  272,  429,  429,  349,  351,  353,  429,
 /*  1740 */   429,  256,  258,  257,  458,  429,  429,  429,  429,  419,
 /*  1750 */   109,  110,  111,  112,  113,  114,  115,  116,  117,  118,
 /*  1760 */   119,  120,  121,  122,  123,  124,  125,  126,  429,  429,
 /*  1770 */   429,  429,  429,  429,  429,  429,  429,  191,  349,  351,
 /*  1780 */   353,  429,  429,  429,  158,  256,  258,  257,  429,  419,
 /*  1790 */   109,  110,  111,  112,  113,  114,  115,  116,  117,  118,
 /*  1800 */   119,  120,  121,  122,  123,  124,  125,  429,  126,  429,
 /*  1810 */   429,  429,  429,  189,  429,  429,  278,  429,  349,  351,
 /*  1820 */   353,  256,  258,  257,  256,  258,  257,  429,  429,  429,
 /*  1830 */   108,  109,  110,  111,  112,  113,  114,  115,  116,  117,
 /*  1840 */   118,  119,  120,  121,  122,  123,  124,  125,  174,  209,
 /*  1850 */   429,  429,  429,  429,  429,  429,  429,  429,  429,  349,
 /*  1860 */   351,  353,  429,  429,  429,  170,  219,  223,  217,  249,
 /*  1870 */   167,  252,  207,  212,  174,  429,  429,  429,  429,  429,
 /*  1880 */   256,  258,  257,  295,  429,  429,  225,  429,  206,  429,
 /*  1890 */   429,  170,  219,  223,  217,  249,  167,  277,  429,  212,
 /*  1900 */     8,  429,  429,   11,  429,   86,  429,  520,  527,  295,
 /*  1910 */   429,  418,  225,  429,  206,  296,  297,  298,  299,  300,
 /*  1920 */   304,  331,  233,  319,  339,  340,  429,  429,  429,  429,
 /*  1930 */   226,  227,  429,  228,  229,  230,  231,  232,  536,  429,
 /*  1940 */   429,  296,  297,  298,  299,  300,  429,  308,  233,  429,
 /*  1950 */   317,  318,  429,  429,  429,  429,  226,  227,  429,  228,
 /*  1960 */   229,  230,  231,  232,  174,  429,  429,  429,  429,    8,
 /*  1970 */   429,  429,   11,  429,   86,  429,  520,  527,  429,  429,
 /*  1980 */   418,  170,  219,  223,  217,  249,  167,  263,  429,  212,
 /*  1990 */   174,  187,  429,  429,  429,  429,  429,  429,  429,  256,
 /*  2000 */   258,  257,  225,  429,  206,  429,  429,  170,  219,  223,
 /*  2010 */   217,  249,  167,  429,  429,  212,  216,  429,  429,  429,
 /*  2020 */   429,  429,  429,  429,  256,  258,  257,  429,  225,  429,
 /*  2030 */   206,  267,  260,  429,  429,  285,  429,  429,  233,  429,
 /*  2040 */   429,  429,  429,  256,  258,  257,  226,  227,  429,  228,
 /*  2050 */   229,  230,  231,  232,  429,  429,  432,  429,  429,  429,
 /*  2060 */   429,  429,  429,  429,  233,  429,   66,   67,   69,   68,
 /*  2070 */    70,  429,  226,  227,  216,  228,  229,  230,  231,  232,
 /*  2080 */   174,  429,  256,  258,  257,  185,  429,  429,  429,  275,
 /*  2090 */   260,  429,  429,  256,  258,  257,  429,  170,  219,  223,
 /*  2100 */   217,  249,  167,   72,   73,  212,  358,  174,  429,  429,
 /*  2110 */   429,   76,  429,   71,  429,  429,  429,  429,  225,  429,
 /*  2120 */   206,  429,  429,  429,  170,  219,  223,  217,  249,  167,
 /*  2130 */   183,  429,  212,   50,  216,  429,  429,  429,  256,  258,
 /*  2140 */   257,  429,  256,  258,  257,  225,  429,  206,  179,  253,
 /*  2150 */   260,  103,  429,   33,  233,  429,  256,  258,  257,  429,
 /*  2160 */   429,  429,  226,  227,  429,  228,  229,  230,  231,  232,
 /*  2170 */    53,   54,   55,   56,   57,   58,   59,   60,   61,   62,
 /*  2180 */    63,  233,  309,  429,  429,  429,  429,   50,  429,  226,
 /*  2190 */   227,  429,  228,  229,  230,  231,  232,  174,  429,  429,
 /*  2200 */   280,  429,  429,  429,  429,  465,  429,   33,  256,  258,
 /*  2210 */   257,  429,  429,  429,  170,  219,  223,  217,  249,  167,
 /*  2220 */   269,  429,  212,  174,   53,   54,   55,   56,   57,   58,
 /*  2230 */    59,   60,   61,   62,   63,  225,  429,  206,  429,  429,
 /*  2240 */   170,  219,  223,  217,  249,  167,  429,  429,  212,  216,
 /*  2250 */   429,  429,  429,  429,  429,  429,  174,  256,  258,  257,
 /*  2260 */   177,  225,  429,  206,  261,  260,  429,  429,  256,  258,
 /*  2270 */   257,  233,  429,  170,  219,  223,  217,  249,  167,  226,
 /*  2280 */   227,  212,  228,  229,  230,  231,  232,  429,  429,  429,
 /*  2290 */   429,  428,  429,  429,  225,  429,  206,  233,  324,  256,
 /*  2300 */   258,  257,  429,  429,  429,  226,  227,  429,  228,  229,
 /*  2310 */   230,  231,  232,  429,  202,  204,  169,  178,  182,  184,
 /*  2320 */   186,  188,  180,  176,  190,  192,  200,  194,  196,  198,
 /*  2330 */   233,  332,   66,   67,   69,   68,   70,  429,  226,  227,
 /*  2340 */   429,  228,  229,  230,  231,  232,  174,  429,  429,  433,
 /*  2350 */   429,  429,  429,  429,  429,  429,  429,  256,  258,  257,
 /*  2360 */   429,  429,  429,  170,  219,  223,  217,  249,  167,   72,
 /*  2370 */    73,  212,  421,  174,  429,  429,  429,  100,  429,   71,
 /*  2380 */   256,  258,  257,  429,  225,  429,  206,  429,  273,  429,
 /*  2390 */   170,  219,  223,  217,  249,  167,  429,  429,  212,   50,
 /*  2400 */   429,  429,  429,  429,  429,  429,  168,  429,  429,  429,
 /*  2410 */   429,  225,  420,  206,  256,  258,  257,   26,  429,   33,
 /*  2420 */   233,  429,  429,  429,  429,  429,  429,  429,  226,  227,
 /*  2430 */   429,  228,  229,  230,  231,  232,   53,   54,   55,   56,
 /*  2440 */    57,   58,   59,   60,   61,   62,   63,  233,  429,  429,
 /*  2450 */   429,  429,  429,  429,  429,  226,  227,  429,  228,  229,
 /*  2460 */   230,  231,  232,  429,  202,  204,  169,  178,  182,  184,
 /*  2470 */   186,  188,  180,  176,  190,  192,  200,  194,  196,  198,
 /*  2480 */   202,  204,  169,  178,  182,  184,  186,  188,  180,  176,
 /*  2490 */   190,  192,  200,  194,  196,  198,  422,  429,  375,  429,
 /*  2500 */   429,  429,  429,  429,  429,  429,  256,  258,  257,  429,
 /*  2510 */   429,  429,  434,  429,  429,  429,  429,  429,  429,  429,
 /*  2520 */   429,  202,  204,  169,  178,  182,  184,  186,  188,  180,
 /*  2530 */   176,  190,  192,  200,  194,  196,  198,  429,  429,  175,
 /*  2540 */   429,  429,  356,  429,  429,  429,  357,  256,  258,  257,
 /*  2550 */   256,  258,  257,  429,  429,  202,  204,  169,  178,  182,
 /*  2560 */   184,  186,  188,  180,  176,  190,  192,  200,  194,  196,
 /*  2570 */   198,  202,  204,  169,  178,  182,  184,  186,  188,  180,
 /*  2580 */   176,  190,  192,  200,  194,  196,  198,  202,  204,  169,
 /*  2590 */   178,  182,  184,  186,  188,  180,  176,  190,  192,  200,
 /*  2600 */   194,  196,  198,  286,  429,  429,  429,  429,  429,  208,
 /*  2610 */   202,  204,  169,  178,  182,  184,  186,  188,  180,  176,
 /*  2620 */   190,  192,  200,  194,  196,  198,  429,  429,  429,  692,
 /*  2630 */   429,  429,  429,  429,  429,  376,  429,  429,  202,  204,
 /*  2640 */   169,  178,  182,  184,  186,  188,  180,  176,  190,  192,
 /*  2650 */   200,  194,  196,  198,  202,  204,  169,  178,  182,  184,
 /*  2660 */   186,  188,  180,  176,  190,  192,  200,  194,  196,  198,
 /*  2670 */   429,   50,  211,  429,  429,  205,  429,  429,  203,  429,
 /*  2680 */   256,  258,  257,  256,  258,  257,  256,  258,  257,   79,
 /*  2690 */   429,   33,  429,   66,   67,   69,   68,   70,  429,  429,
 /*  2700 */   429,  429,  429,  429,  429,  429,  429,  429,   53,   54,
 /*  2710 */    55,   56,   57,   58,   59,   60,   61,   62,   63,   66,
 /*  2720 */    67,   69,   68,   70,  429,  429,  429,  429,  429,  218,
 /*  2730 */    72,   73,  429,   93,  492,  429,  486,  256,  258,  257,
 /*  2740 */    71,  224,   66,   67,   69,   68,   70,  429,  429,  256,
 /*  2750 */   258,  257,  429,  429,  429,  429,   72,   73,  429,  564,
 /*  2760 */   480,  429,  486,  429,  429,  201,   71,  429,   66,   67,
 /*  2770 */    69,   68,   70,  256,  258,  257,  429,  429,  429,   72,
 /*  2780 */    73,  429,  560,  492,  429,  486,  429,  429,  429,   71,
 /*  2790 */   429,   66,   67,   69,   68,   70,   66,   67,   69,   68,
 /*  2800 */    70,  429,  429,  429,  199,   72,   73,  429,  561,  492,
 /*  2810 */   429,  486,  256,  258,  257,   71,  429,   66,   67,   69,
 /*  2820 */    68,   70,   66,   67,   69,   68,   70,  429,   72,   73,
 /*  2830 */   429,  566,  460,   72,   73,  429,   20,   74,   71,  429,
 /*  2840 */   429,  429,  197,   71,  429,   66,   67,   69,   68,   70,
 /*  2850 */   256,  258,  257,  429,   72,   73,  429,  567,  460,   72,
 /*  2860 */    73,  429,  568,  460,   71,  429,  429,  429,  429,   71,
 /*  2870 */   429,   66,   67,   69,   68,   70,   66,   67,   69,   68,
 /*  2880 */    70,  429,   72,   73,  429,  565,  509,  429,  429,  429,
 /*  2890 */   429,  429,   71,  429,   66,   67,   69,   68,   70,  429,
 /*  2900 */   429,  429,  429,  429,  429,  429,  429,  429,   72,   73,
 /*  2910 */   429,  562,  460,   72,   73,  429,   18,   74,   71,  429,
 /*  2920 */   429,  429,  429,   71,  429,  500,  429,  429,  429,  429,
 /*  2930 */   429,   72,   73,   66,   67,   69,   68,   70,  429,  100,
 /*  2940 */   429,   71,  429,  429,  429,  429,  429,  429,  429,  429,
 /*  2950 */   429,  429,  429,  429,  429,  429,  429,  429,  429,  429,
 /*  2960 */   429,  429,  429,  429,  429,  429,  429,  429,  429,  429,
 /*  2970 */    72,   73,  429,  563,  460,   66,   67,   69,   68,   70,
 /*  2980 */    71,   66,   67,   69,   68,   70,   66,   67,   69,   68,
 /*  2990 */    70,  429,  429,  429,  429,  429,  429,  429,  429,  429,
 /*  3000 */   429,  429,  429,  429,  429,  429,  494,  429,  429,  429,
 /*  3010 */   429,  429,   72,   73,  429,  429,  429,  429,   72,   73,
 /*  3020 */   462,  429,   71,   72,   73,  429,   23,  429,   71,  429,
 /*  3030 */   429,  462,  429,   71,
};
static YYCODETYPE yy_lookahead[] = {
 /*     0 */     7,   10,   11,   12,   13,   14,   15,   16,   17,   18,
 /*    10 */    19,   20,   21,   22,   23,   41,   69,   24,   25,   26,
 /*    20 */    27,   28,   29,  109,   32,   32,  112,  113,  114,  115,
 /*    30 */   116,  121,  118,  119,  124,  125,  126,  123,   45,   65,
 /*    40 */    47,   67,   49,   50,   51,   52,   53,   54,   55,   56,
 /*    50 */    57,   58,   59,    8,    9,   10,   11,   12,   13,   14,
 /*    60 */    15,   16,   17,   18,   19,   20,   21,   22,   23,  148,
 /*    70 */   113,  114,  115,  116,   81,  118,  119,  156,  157,  158,
 /*    80 */   123,   32,   89,   90,   32,   92,   93,   94,   95,   96,
 /*    90 */    41,   42,    1,    2,    3,    4,    5,   45,   49,   50,
 /*   100 */    51,   52,   53,   54,   55,   56,   57,   32,   59,   60,
 /*   110 */    61,   62,   33,   64,  159,  160,  111,   68,   69,   70,
 /*   120 */    71,  116,    6,   74,   32,  120,  161,   82,  123,   38,
 /*   130 */    39,    6,   81,   41,   42,   86,   87,   88,   40,   48,
 /*   140 */    91,   49,   50,   51,   52,   53,   54,   55,   56,   57,
 /*   150 */    32,   59,   60,   61,   62,   30,   64,   82,   32,    6,
 /*   160 */    68,   69,   70,   71,   89,   90,   74,   32,   93,   94,
 /*   170 */    95,   96,  124,  125,  126,  116,   41,   42,   86,   87,
 /*   180 */    88,   47,  123,   91,   49,   50,   51,   52,   53,   54,
 /*   190 */    55,   56,   57,   32,   59,   60,   61,   62,   82,   64,
 /*   200 */    21,   22,   23,   68,   69,   70,   71,   89,   90,   74,
 /*   210 */    32,   93,   94,   95,   96,   89,   90,  146,  147,   41,
 /*   220 */    42,   86,   87,   88,   32,   72,   91,   49,   50,   51,
 /*   230 */    52,   53,   54,   55,   56,   57,   35,   59,   60,   61,
 /*   240 */    62,   40,   64,  123,    6,   41,   68,   69,   70,   71,
 /*   250 */    89,   90,   74,   32,   93,   94,   95,   96,  149,  150,
 /*   260 */   117,   47,   41,   42,   86,   87,   88,   43,   30,   91,
 /*   270 */    49,   50,   51,   52,   53,   54,   55,   56,   57,  116,
 /*   280 */    59,   60,   61,   62,  107,   64,  123,    6,   30,   68,
 /*   290 */    69,   70,   71,    6,   80,   74,   32,   83,    6,   75,
 /*   300 */    76,   77,   78,   79,    6,   41,   42,   86,   87,   88,
 /*   310 */   167,   30,   91,   49,   50,   51,   52,   53,   54,   55,
 /*   320 */    56,   57,   30,   59,   60,   61,   62,   40,   64,    6,
 /*   330 */     6,   33,   68,   69,   70,   71,   80,   81,   74,   32,
 /*   340 */    18,   19,   20,   21,   22,   23,    6,    6,   41,   42,
 /*   350 */    86,   87,   88,   30,   30,   91,   49,   50,   51,   52,
 /*   360 */    53,   54,   55,   56,   57,  116,   59,   60,   61,   62,
 /*   370 */     6,   64,  123,   33,   33,   68,   69,   70,   71,   89,
 /*   380 */    90,   74,   32,   93,   94,   95,   96,  159,  160,  159,
 /*   390 */   160,   41,   42,   86,   87,   88,    6,   33,   91,   49,
 /*   400 */    50,   51,   52,   53,   54,   55,   56,   57,   40,   59,
 /*   410 */    60,   61,   62,    6,   64,   32,  159,  160,   68,   69,
 /*   420 */    70,   71,  148,   33,   74,   32,  159,  160,  154,  155,
 /*   430 */   156,  157,  158,    6,   41,   42,   86,   87,   88,    6,
 /*   440 */    33,   91,   49,   50,   51,   52,   53,   54,   55,   56,
 /*   450 */    57,   32,   59,   60,   61,   62,    6,   64,  159,  160,
 /*   460 */    33,   68,   69,   70,   71,  148,   33,   74,   32,  152,
 /*   470 */   159,  160,    6,  156,  157,  158,    6,   41,   42,   86,
 /*   480 */    87,   88,    6,   33,   91,   49,   50,   51,   52,   53,
 /*   490 */    54,   55,   56,   57,   47,   59,   60,   61,   62,   33,
 /*   500 */    64,  159,  160,   33,   68,   69,   70,   71,  148,   33,
 /*   510 */    74,   32,  152,  159,  160,   33,  156,  157,  158,  107,
 /*   520 */    41,   42,   86,   87,   88,   43,   32,   91,   49,   50,
 /*   530 */    51,   52,   53,   54,   55,   56,   57,   35,   59,   60,
 /*   540 */    61,   62,   40,   64,  159,  160,   32,   68,   69,   70,
 /*   550 */    71,  148,  121,   74,   32,  124,  125,  126,  155,  156,
 /*   560 */   157,  158,    6,   41,   42,   86,   87,   88,   32,  161,
 /*   570 */    91,   49,   50,   51,   52,   53,   54,   55,   56,   57,
 /*   580 */    47,   59,   60,   61,   62,   33,   64,   66,    6,   66,
 /*   590 */    68,   69,   70,   71,  148,   43,   74,   32,  152,    6,
 /*   600 */    66,   66,  156,  157,  158,   33,   41,   42,   86,   87,
 /*   610 */    88,  107,   30,   91,   49,   50,   51,   52,   53,   54,
 /*   620 */    55,   56,   57,   30,   59,   60,   61,   62,   47,   64,
 /*   630 */    47,   33,   35,   68,   69,   70,   71,   40,  148,   74,
 /*   640 */    32,  101,  102,  103,  104,  105,  156,  157,  158,   41,
 /*   650 */    42,   86,   87,   88,  164,   32,   91,   49,   50,   51,
 /*   660 */    52,   53,   54,   55,   56,   57,   32,   59,   60,   61,
 /*   670 */    62,  110,   64,   32,   32,  150,   68,   69,   70,   71,
 /*   680 */   148,  151,   74,   32,  152,  108,   32,  110,  156,  157,
 /*   690 */   158,   82,   41,   42,   86,   87,   88,  151,  151,   91,
 /*   700 */    49,   50,   51,   52,   53,   54,   55,   56,   57,  151,
 /*   710 */    59,   60,   61,   62,   32,   64,   32,  151,  151,   68,
 /*   720 */    69,   70,   71,  148,  151,   74,   32,  116,  153,   82,
 /*   730 */   119,  156,  157,  158,  123,   41,   42,   86,   87,   88,
 /*   740 */   151,   82,   91,   49,   50,   51,   52,   53,   54,   55,
 /*   750 */    56,   57,  151,   59,   60,   61,   62,   13,   64,   32,
 /*   760 */   117,   33,   68,   69,   70,   71,  148,   33,   74,   32,
 /*   770 */   152,   33,   43,   32,  156,  157,  158,   32,   41,   42,
 /*   780 */    86,   87,   88,   33,   82,   91,   49,   50,   51,   52,
 /*   790 */    53,   54,   55,   56,   57,   33,   59,   60,   61,   62,
 /*   800 */    82,   64,   33,   82,   33,   68,   69,   70,   71,  148,
 /*   810 */    32,   74,   32,  152,   32,  160,   43,  156,  157,  158,
 /*   820 */    36,   41,   42,   86,   87,   88,  117,  117,   91,   49,
 /*   830 */    50,   51,   52,   53,   54,   55,   56,   57,   32,   59,
 /*   840 */    60,   61,   62,  107,   64,   32,   36,  107,   68,   69,
 /*   850 */    70,   71,  148,   32,   74,   32,  117,  153,   43,   32,
 /*   860 */   156,  157,  158,   32,   41,   42,   86,   87,   88,   32,
 /*   870 */    32,   91,   49,   50,   51,   52,   53,   54,   55,   56,
 /*   880 */    57,   72,   59,   60,   61,   62,   43,   64,   69,  117,
 /*   890 */    33,   68,   69,   70,   71,  116,   66,   74,   32,  120,
 /*   900 */    33,  162,  123,   63,  165,  166,  167,   41,   42,   86,
 /*   910 */    87,   88,   40,   33,   91,   49,   50,   51,   52,   53,
 /*   920 */    54,   55,   56,   57,   40,   59,   60,   61,   62,   47,
 /*   930 */    64,   32,  107,   32,   68,   69,   70,   71,  148,   47,
 /*   940 */    74,   32,  152,   40,   40,  106,  156,  157,  158,   32,
 /*   950 */    41,   42,   86,   87,   88,   40,  106,   91,   49,   50,
 /*   960 */    51,   52,   53,   54,   55,   56,   57,   32,   59,   60,
 /*   970 */    61,   62,   45,   64,   43,  117,   33,   68,   69,   70,
 /*   980 */    71,  148,   45,   74,   32,  152,  117,   43,   33,  156,
 /*   990 */   157,  158,   32,   41,   42,   86,   87,   88,   33,   33,
 /*  1000 */    91,   49,   50,   51,   52,   53,   54,   55,   56,   57,
 /*  1010 */   117,   59,   60,   61,   62,   32,   64,  117,   33,  107,
 /*  1020 */    68,   69,   70,   71,  148,    0,   74,   32,   40,  153,
 /*  1030 */   147,  117,  156,  157,  158,   66,   41,   42,   86,   87,
 /*  1040 */    88,   40,   63,   91,   49,   50,   51,   52,   53,   54,
 /*  1050 */    55,   56,   57,   40,   59,   60,   61,   62,   40,   64,
 /*  1060 */    33,   40,   33,   68,   69,   70,   71,  148,   40,   74,
 /*  1070 */    32,  152,  168,  168,  168,  156,  157,  158,  168,   41,
 /*  1080 */    42,   86,   87,   88,   47,  168,   91,   49,   50,   51,
 /*  1090 */    52,   53,   54,   55,   56,   57,  168,   59,   60,   61,
 /*  1100 */    62,  168,   64,  168,  168,  168,   68,   69,   70,   71,
 /*  1110 */   168,  121,   74,   32,  124,  125,  126,   80,   81,   12,
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
 /*  1230 */   142,  143,  144,  117,  114,  105,  116,  168,  118,  119,
 /*  1240 */   168,  168,  168,  123,  156,  157,  158,    1,    2,    3,
 /*  1250 */     4,    5,  122,   41,  168,  168,  168,  127,  128,  129,
 /*  1260 */   130,  131,  132,  133,  134,  135,  136,  137,  138,  139,
 /*  1270 */   140,  141,  142,  143,  144,  105,  168,   65,  168,   67,
 /*  1280 */   168,  165,  166,  167,   38,   39,  156,  157,  158,  168,
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
 /*  1440 */   168,  168,  168,  114,  115,  116,  168,  168,  119,  156,
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
 /*  1560 */   144,  121,  105,  168,  124,  125,  126,  168,  168,  168,
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
 /*  1840 */   137,  138,  139,  140,  141,  142,  143,  144,    7,  125,
 /*  1850 */   168,  168,  168,  168,  168,  168,  168,  168,  168,  156,
 /*  1860 */   157,  158,  168,  168,  168,   24,   25,   26,   27,   28,
 /*  1870 */    29,   30,  148,   32,    7,  168,  168,  168,  168,  168,
 /*  1880 */   156,  157,  158,   43,  168,  168,   45,  168,   47,  168,
 /*  1890 */   168,   24,   25,   26,   27,   28,   29,   30,  168,   32,
 /*  1900 */    31,  168,  168,   34,  168,   36,  168,   38,   39,   43,
 /*  1910 */   168,   42,   45,  168,   47,   75,   76,   77,   78,   79,
 /*  1920 */    80,   81,   81,   83,   84,   85,  168,  168,  168,  168,
 /*  1930 */    89,   90,  168,   92,   93,   94,   95,   96,    0,  168,
 /*  1940 */   168,   75,   76,   77,   78,   79,  168,   81,   81,  168,
 /*  1950 */    84,   85,  168,  168,  168,  168,   89,   90,  168,   92,
 /*  1960 */    93,   94,   95,   96,    7,  168,  168,  168,  168,   31,
 /*  1970 */   168,  168,   34,  168,   36,  168,   38,   39,  168,  168,
 /*  1980 */    42,   24,   25,   26,   27,   28,   29,   30,  168,   32,
 /*  1990 */     7,  148,  168,  168,  168,  168,  168,  168,  168,  156,
 /*  2000 */   157,  158,   45,  168,   47,  168,  168,   24,   25,   26,
 /*  2010 */    27,   28,   29,  168,  168,   32,  148,  168,  168,  168,
 /*  2020 */   168,  168,  168,  168,  156,  157,  158,  168,   45,  168,
 /*  2030 */    47,  163,  164,  168,  168,  148,  168,  168,   81,  168,
 /*  2040 */   168,  168,  168,  156,  157,  158,   89,   90,  168,   92,
 /*  2050 */    93,   94,   95,   96,  168,  168,   73,  168,  168,  168,
 /*  2060 */   168,  168,  168,  168,   81,  168,    1,    2,    3,    4,
 /*  2070 */     5,  168,   89,   90,  148,   92,   93,   94,   95,   96,
 /*  2080 */     7,  168,  156,  157,  158,  148,  168,  168,  168,  163,
 /*  2090 */   164,  168,  168,  156,  157,  158,  168,   24,   25,   26,
 /*  2100 */    27,   28,   29,   38,   39,   32,   33,    7,  168,  168,
 /*  2110 */   168,   46,  168,   48,  168,  168,  168,  168,   45,  168,
 /*  2120 */    47,  168,  168,  168,   24,   25,   26,   27,   28,   29,
 /*  2130 */   148,  168,   32,   12,  148,  168,  168,  168,  156,  157,
 /*  2140 */   158,  168,  156,  157,  158,   45,  168,   47,  148,  163,
 /*  2150 */   164,   30,  168,   32,   81,  168,  156,  157,  158,  168,
 /*  2160 */   168,  168,   89,   90,  168,   92,   93,   94,   95,   96,
 /*  2170 */    49,   50,   51,   52,   53,   54,   55,   56,   57,   58,
 /*  2180 */    59,   81,   82,  168,  168,  168,  168,   12,  168,   89,
 /*  2190 */    90,  168,   92,   93,   94,   95,   96,    7,  168,  168,
 /*  2200 */   148,  168,  168,  168,  168,   30,  168,   32,  156,  157,
 /*  2210 */   158,  168,  168,  168,   24,   25,   26,   27,   28,   29,
 /*  2220 */    30,  168,   32,    7,   49,   50,   51,   52,   53,   54,
 /*  2230 */    55,   56,   57,   58,   59,   45,  168,   47,  168,  168,
 /*  2240 */    24,   25,   26,   27,   28,   29,  168,  168,   32,  148,
 /*  2250 */   168,  168,  168,  168,  168,  168,    7,  156,  157,  158,
 /*  2260 */   148,   45,  168,   47,  163,  164,  168,  168,  156,  157,
 /*  2270 */   158,   81,  168,   24,   25,   26,   27,   28,   29,   89,
 /*  2280 */    90,   32,   92,   93,   94,   95,   96,  168,  168,  168,
 /*  2290 */   168,  148,  168,  168,   45,  168,   47,   81,   82,  156,
 /*  2300 */   157,  158,  168,  168,  168,   89,   90,  168,   92,   93,
 /*  2310 */    94,   95,   96,  168,    8,    9,   10,   11,   12,   13,
 /*  2320 */    14,   15,   16,   17,   18,   19,   20,   21,   22,   23,
 /*  2330 */    81,   82,    1,    2,    3,    4,    5,  168,   89,   90,
 /*  2340 */   168,   92,   93,   94,   95,   96,    7,  168,  168,  148,
 /*  2350 */   168,  168,  168,  168,  168,  168,  168,  156,  157,  158,
 /*  2360 */   168,  168,  168,   24,   25,   26,   27,   28,   29,   38,
 /*  2370 */    39,   32,  148,    7,  168,  168,  168,   46,  168,   48,
 /*  2380 */   156,  157,  158,  168,   45,  168,   47,  168,   82,  168,
 /*  2390 */    24,   25,   26,   27,   28,   29,  168,  168,   32,   12,
 /*  2400 */   168,  168,  168,  168,  168,  168,  148,  168,  168,  168,
 /*  2410 */   168,   45,   73,   47,  156,  157,  158,   30,  168,   32,
 /*  2420 */    81,  168,  168,  168,  168,  168,  168,  168,   89,   90,
 /*  2430 */   168,   92,   93,   94,   95,   96,   49,   50,   51,   52,
 /*  2440 */    53,   54,   55,   56,   57,   58,   59,   81,  168,  168,
 /*  2450 */   168,  168,  168,  168,  168,   89,   90,  168,   92,   93,
 /*  2460 */    94,   95,   96,  168,    8,    9,   10,   11,   12,   13,
 /*  2470 */    14,   15,   16,   17,   18,   19,   20,   21,   22,   23,
 /*  2480 */     8,    9,   10,   11,   12,   13,   14,   15,   16,   17,
 /*  2490 */    18,   19,   20,   21,   22,   23,   40,  168,  148,  168,
 /*  2500 */   168,  168,  168,  168,  168,  168,  156,  157,  158,  168,
 /*  2510 */   168,  168,   40,  168,  168,  168,  168,  168,  168,  168,
 /*  2520 */   168,    8,    9,   10,   11,   12,   13,   14,   15,   16,
 /*  2530 */    17,   18,   19,   20,   21,   22,   23,  168,  168,  148,
 /*  2540 */   168,  168,  148,  168,  168,  168,   33,  156,  157,  158,
 /*  2550 */   156,  157,  158,  168,  168,    8,    9,   10,   11,   12,
 /*  2560 */    13,   14,   15,   16,   17,   18,   19,   20,   21,   22,
 /*  2570 */    23,    8,    9,   10,   11,   12,   13,   14,   15,   16,
 /*  2580 */    17,   18,   19,   20,   21,   22,   23,    8,    9,   10,
 /*  2590 */    11,   12,   13,   14,   15,   16,   17,   18,   19,   20,
 /*  2600 */    21,   22,   23,   40,  168,  168,  168,  168,  168,   30,
 /*  2610 */     8,    9,   10,   11,   12,   13,   14,   15,   16,   17,
 /*  2620 */    18,   19,   20,   21,   22,   23,  168,  168,  168,   82,
 /*  2630 */   168,  168,  168,  168,  168,   33,  168,  168,    8,    9,
 /*  2640 */    10,   11,   12,   13,   14,   15,   16,   17,   18,   19,
 /*  2650 */    20,   21,   22,   23,    8,    9,   10,   11,   12,   13,
 /*  2660 */    14,   15,   16,   17,   18,   19,   20,   21,   22,   23,
 /*  2670 */    40,   12,  148,  168,  168,  148,  168,  168,  148,  168,
 /*  2680 */   156,  157,  158,  156,  157,  158,  156,  157,  158,   30,
 /*  2690 */   168,   32,  168,    1,    2,    3,    4,    5,  168,  168,
 /*  2700 */   168,  168,  168,  168,  168,  168,  168,  168,   49,   50,
 /*  2710 */    51,   52,   53,   54,   55,   56,   57,   58,   59,    1,
 /*  2720 */     2,    3,    4,    5,  168,  168,  168,  168,  168,  148,
 /*  2730 */    38,   39,  168,   41,   42,  168,   44,  156,  157,  158,
 /*  2740 */    48,  148,    1,    2,    3,    4,    5,  168,  168,  156,
 /*  2750 */   157,  158,  168,  168,  168,  168,   38,   39,  168,   41,
 /*  2760 */    42,  168,   44,  168,  168,  148,   48,  168,    1,    2,
 /*  2770 */     3,    4,    5,  156,  157,  158,  168,  168,  168,   38,
 /*  2780 */    39,  168,   41,   42,  168,   44,  168,  168,  168,   48,
 /*  2790 */   168,    1,    2,    3,    4,    5,    1,    2,    3,    4,
 /*  2800 */     5,  168,  168,  168,  148,   38,   39,  168,   41,   42,
 /*  2810 */   168,   44,  156,  157,  158,   48,  168,    1,    2,    3,
 /*  2820 */     4,    5,    1,    2,    3,    4,    5,  168,   38,   39,
 /*  2830 */   168,   41,   42,   38,   39,  168,   41,   42,   48,  168,
 /*  2840 */   168,  168,  148,   48,  168,    1,    2,    3,    4,    5,
 /*  2850 */   156,  157,  158,  168,   38,   39,  168,   41,   42,   38,
 /*  2860 */    39,  168,   41,   42,   48,  168,  168,  168,  168,   48,
 /*  2870 */   168,    1,    2,    3,    4,    5,    1,    2,    3,    4,
 /*  2880 */     5,  168,   38,   39,  168,   41,   42,  168,  168,  168,
 /*  2890 */   168,  168,   48,  168,    1,    2,    3,    4,    5,  168,
 /*  2900 */   168,  168,  168,  168,  168,  168,  168,  168,   38,   39,
 /*  2910 */   168,   41,   42,   38,   39,  168,   41,   42,   48,  168,
 /*  2920 */   168,  168,  168,   48,  168,   32,  168,  168,  168,  168,
 /*  2930 */   168,   38,   39,    1,    2,    3,    4,    5,  168,   46,
 /*  2940 */   168,   48,  168,  168,  168,  168,  168,  168,  168,  168,
 /*  2950 */   168,  168,  168,  168,  168,  168,  168,  168,  168,  168,
 /*  2960 */   168,  168,  168,  168,  168,  168,  168,  168,  168,  168,
 /*  2970 */    38,   39,  168,   41,   42,    1,    2,    3,    4,    5,
 /*  2980 */    48,    1,    2,    3,    4,    5,    1,    2,    3,    4,
 /*  2990 */     5,  168,  168,  168,  168,  168,  168,  168,  168,  168,
 /*  3000 */   168,  168,  168,  168,  168,  168,   32,  168,  168,  168,
 /*  3010 */   168,  168,   38,   39,  168,  168,  168,  168,   38,   39,
 /*  3020 */    46,  168,   48,   38,   39,  168,   46,  168,   48,  168,
 /*  3030 */   168,   46,  168,   48,
};
#define YY_SHIFT_USE_DFLT (-54)
static short yy_shift_ofst[] = {
 /*     0 */  1869, 1025, 1938,  -54,  -54,  -54,  -54,  -54,  983,  965,
 /*    10 */   -54,  935,  201,  -54,  917,  915,  -54, 2875,  -54, 2795,
 /*    20 */   -54,  -54, 2980,  899,  882, 2387,  880,  -54,  125,  867,
 /*    30 */   -54, 1107,  -54,  843,  290,  -54,  -54,  -54,  -54,  -54,
 /*    40 */   -54,  -54,  837,  815,  290,  -54,  778,  729,  290,  -54,
 /*    50 */   727,  744,  684,  -54,  -54,  -54,  -54,  -54,  -54,  -54,
 /*    60 */   -54,  -54,  -54,  -54,  -54,  -54,  -54,  -54,  -54,  -54,
 /*    70 */   -54,  -54,  -54,  -54,   91, 2065,  634,  583, 2659,  598,
 /*    80 */   -54,  238,  572,  -54,  -54,  -54,  494, 1295,  -54,  383,
 /*    90 */   368,  -54, 2692,  -54,  204,  -54, 2741, 2932,  -54, 2331,
 /*   100 */   192,  134, 2121,   98,  866,  -54,   92,  -54,  -54,  -54,
 /*   110 */   -54,  -54,  -54,  -54,  -54,  -54,  -54,  -54,  -54,  -54,
 /*   120 */   -54,  -54,  -54,  -54,  -54,  -54,  -54, 1029,  -54, 1027,
 /*   130 */   -54, 2366, 1018,  565,  479,  979, 1001,  221,  135,  -54,
 /*   140 */  2366,  988,  -26,  -54, 1212,  -54,  -54,  290,  969, 1081,
 /*   150 */  1081, 1013,  608,  694,  -54, 2366, 1021,  737,  823,  -54,
 /*   160 */  1028,  909, 1038,  -53, 2366,   79,  -54, 2366,  -54, 2366,
 /*   170 */    -8,  -54,   51, 2366, 2366, 2646, 2366,  322, 2366,  322,
 /*   180 */  2366,  322, 2366,  322, 2366,  322, 2366,  322, 2366,  322,
 /*   190 */  2366,  179, 2366,  179, 2366,  -54, 2366,  -54, 2366,  -54,
 /*   200 */  2366,  179, 2366,   -9, 2366,   -9,   -7, 2579,  -54,  258,
 /*   210 */  2366,  -54, 1037,  419,  447, 1957, 2646, 2366,  -54,  514,
 /*   220 */   556,   -8,  -54, 2366,  -54,  -54,  -54,  -54,  -54,  -54,
 /*   230 */   -54,  -54,  -54,   75,  -54,  116,  -54,  161,  -54,  521,
 /*   240 */   118,  -54,  -54,  -54,  -54,  523,  534,  535,  -54,  536,
 /*   250 */   533, 1841,  -54,  281,  -54, 2366,  -54,  -54,  -54,  -54,
 /*   260 */   -54,  292,  -54,  -54,   52,  581, 2190,  323,  -54,  -54,
 /*   270 */   -54, 2366, 2306,  -54, 1867,  324,  -54,  -54,   45,  -54,
 /*   280 */   322, 2646,  641,  153, 2339, 2563, 1081,  436,  -54,  642,
 /*   290 */   298,  -54,  642,  -54, 1840,  -54,  -54,  -54,  -54,  -54,
 /*   300 */   -54, 2366,  -54, 2646,  654, 1866, 2366,  -54, 2100,  224,
 /*   310 */  2366,  -54,  609,  224, 2366,  -54, 2547,  -54,  -54,  682,
 /*   320 */  1327, 2366,  -54, 2216,  224, 2366,  -54,  647,  224, 2366,
 /*   330 */   -54, 2249,  224, 2366,  -54,  659,  224, 2366,  -54,  -54,
 /*   340 */   -54,  -54, 2366,  340,  -54, 2366,  -54, 2646,  -54,  728,
 /*   350 */   -54,  734,  -54,  738,  -54, 2073, 2513,  -54,  -54,  741,
 /*   360 */   256,  745,  750,  -54,  126,  702,  762,  -54,  718,  769,
 /*   370 */   -54,  721,  771,  -54, 2366, 2602,  -54,  782,  341,  -54,
 /*   380 */   782,  -54,  773,  290,  -54,  -54,  782,  364,  -54,  782,
 /*   390 */   390,  -54,  782,  407,  -54,  782,  427,  -54,  782,  433,
 /*   400 */   -54,  782,  450,  -54,  782,  466,  -54,  782,  470,  -54,
 /*   410 */   782,  476,  -54,  214,  827,  581,  831,  447,  -54,  -54,
 /*   420 */  2366, 2456, 1081,  264,  -54,  838,  809, 1983, 2630, 1081,
 /*   430 */   350,  -54, 2366, 2472, 1081,  178,  -54,  819, 2366,  857,
 /*   440 */   -54,  -54,  -54,  830, 1081, 1081,  -54,  -54,  840,  872,
 /*   450 */   307,  393,  -54,  -54,  582,  884,  522,  -54,  651,  -54,
 /*   460 */    91, 2985,  901,  892, 2175,  903,  780,  -54,  952,  -54,
 /*   470 */   593,  904,  995,  -54,   49,  -54, 2718, 2816,  -54,  -54,
 /*   480 */  1246,  927,  931,  290,  943,  -54,  937,  944,  290,  955,
 /*   490 */   -54,  -54, 1246, 2974,  482,  -54,  290,  966,  -54, 2893,
 /*   500 */   552,  -54,  290,  985,  -54,  -54, 2767, 2844, 2821,   91,
 /*   510 */  2790,  -54, 2870,  623,  287,  -54,  623,  -54,  -54,  -54,
 /*   520 */   784,  806,  502,  -54,  813,  368,  -54,  810,  821,  597,
 /*   530 */   -54,  960,  368,  -54,  -54,
};
#define YY_REDUCE_USE_DFLT (-91)
static short yy_reduce_ofst[] = {
 /*     0 */  1096,  -91,  540,  -91,  -91,  -91,  -91,  -91,  -91,  -91,
 /*    10 */   -91,  -91,  850,  -91,  -91,  839,  -91,    5,  -91,  779,
 /*    20 */   -91,  -91,  120,  -91,  -91,  990,  -91,  -91,  -91,  -91,
 /*    30 */   -91,   48,  -91,  -91,  772,  -91,  -91,  -91,  -91,  -91,
 /*    40 */   -91,  -91,  -91,  -91,  709,  -91,  -91,  -91,  643,  -91,
 /*    50 */   -91,  -91,  -91,  -91,  -91,  -91,  -91,  -91,  -91,  -91,
 /*    60 */   -91,  -91,  -91,  -91,  -91,  -91,  -91,  -91,  -91,  -91,
 /*    70 */   -91,  -91,  -91,  -91,   59,  120,  -91,  -91,  431,  -91,
 /*    80 */   -91,  -91,  -91,  -91,  -91,  -91,  -91,  412,  -91,  -91,
 /*    90 */   177,  -91,  -86,  -91,  -91,  -91,  -43,  611,  -91,  120,
 /*   100 */   -91,  -91,  -90,  -91, 1130,  -91, 1703,  -91,  -91,  -91,
 /*   110 */   -91,  -91,  -91,  -91,  -91,  -91,  -91,  -91,  -91,  -91,
 /*   120 */   -91,  -91,  -91,  -91,  -91,  -91,  -91,  -91,  -91,  -91,
 /*   130 */   -91, 1339,  -91, 1048, 1703,  -91,  -91, 1457, 1703,  -91,
 /*   140 */  1383,  -91,   71,  -91,  883,  -91,  -91,  914,  -91, 1253,
 /*   150 */  1703,  -91, 1580, 1703,  -91, 1216,  -91, 1662, 1703,  -91,
 /*   160 */   -91, 1170, 1703,  -91, 1506,  -91,  -91, 2258,  -91, 2052,
 /*   170 */   -35,  -91,  -91, 1668, 2391,  -91, 2112,  -91, 2000,  -91,
 /*   180 */   -79,  -91, 1982,  -91, 1937,  -91, 1843,  -91, 1665,  -91,
 /*   190 */  1629,  -91, 1542,  -91, 1055,  -91, 2694,  -91, 2656,  -91,
 /*   200 */  2617,  -91, 2530,  -91, 2527,  -91, 1724,  -91,  -91,  -91,
 /*   210 */  2524,  -91,  -91,  -91,  -91, 2101,  -91, 2581,  -91,  -91,
 /*   220 */   -91,  408,  -91, 2593,  -91,  -91,  -91,  -91,  -91,  -91,
 /*   230 */   -91,  -91,  -91,  739,  -91,  -91,  -91, 1116,  -91,  -91,
 /*   240 */   143,  -91,  -91,  -91,  -91,  -91,  -91,  -91,  -91,  -91,
 /*   250 */   -91, 1986,  -91,  -91,  -91,  490,  -91,  -91,  -91,  -91,
 /*   260 */   -91,  -91,  -91,  -91,  -91,  -91, 1868,  -91,  -91,  -91,
 /*   270 */   -91, 1585,  -91,  -91, 1926,  -91,  -91,  -91,  -91,  -91,
 /*   280 */   -91,  -91,  -91,  -91, 1887,  -91, 1088, 1703,  -91,  109,
 /*   290 */   -91,  -91,  525,  -91,  530,  -91,  -91,  -91,  -91,  -91,
 /*   300 */   -91,  532,  -91,  -91,  -91,  546,  661,  -91,  704,  547,
 /*   310 */   790,  -91,  -91,  558,  317,  -91,  -91,  -91,  -91,  -91,
 /*   320 */   566,  919,  -91,  575,  567,  360,  -91,  -91,  573,  618,
 /*   330 */   -91,  876,  589,  446,  -91,  -91,  601,  833,  -91,  -91,
 /*   340 */   -91,  -91,  274,  -91,  -91,  403,  -91,  -91,  -91,  -91,
 /*   350 */   -91,  -91,  -91,  -91,  -91, 2394,  -91,  -91,  -91,  -91,
 /*   360 */   -91,  -91,  -91,  -91,  -91,  -91,  -91,  -91,  -91,  -91,
 /*   370 */   -91,  -91,  -91,  -91, 2350,  -91,  -91,  228,  -91,  -91,
 /*   380 */   655,  -91,  -91,  710,  -91,  -91,  230,  -91,  -91,  257,
 /*   390 */   -91,  -91,  267,  -91,  -91,  299,  -91,  -91,  311,  -91,
 /*   400 */   -91,  -45,  -91,  -91,  342,  -91,  -91,  354,  -91,  -91,
 /*   410 */   385,  -91,  -91,  -91,  -91,  -91,  -91,  -91,  -91,  -91,
 /*   420 */  2224,  -91, 1499, 1703,  -91,  -91,  -91, 2143,  -91, 1334,
 /*   430 */  1703,  -91, 2201,  -91, 1416, 1703,  -91,  -91, 1462,  -91,
 /*   440 */   -91,  -91,  -91,  -91, 1376, 1703,  -91,  -91,  -91,  -91,
 /*   450 */  1211, 1703,  -91,  -91,  -91,  -91, 1622,  -91, 1703,  -91,
 /*   460 */   163,  120,  -91,  -91, 1440,  -91, 1539,  -91, 1703,  -91,
 /*   470 */   -91,  -91, 1293,  -91, 1703,  -91, 1120,  611,  -91,  -91,
 /*   480 */   163,  -91,  -91,  858,  -91,  -91,  -91,  -91,  869,  -91,
 /*   490 */   -91,  -91,  249,  120,  -91,  -91,  893,  -91,  -91,  120,
 /*   500 */   -91,  -91,  900,  -91,  -91,  -91, 1206, 1329,  611,  249,
 /*   510 */   611,  -91,  611,  577,  504,  -91,  561,  -91,  -91,  -91,
 /*   520 */   -91,  -91,  736,  -91,  -91,  740,  -91,  -91,  -91,  825,
 /*   530 */   -91,  -91,  912,  -91,  -91,
};
static YYACTIONTYPE yy_default[] = {
 /*     0 */   794,  794,  794,  537,  539,  540,  541,  542,  794,  794,
 /*    10 */   543,  794,  794,  544,  794,  794,  545,  794,  558,  794,
 /*    20 */   559,  579,  794,  794,  794,  794,  794,  591,  794,  794,
 /*    30 */   592,  794,  605,  607,  794,  610,  785,  786,  787,  788,
 /*    40 */   789,  790,  794,  608,  794,  611,  794,  609,  794,  612,
 /*    50 */   794,  794,  794,  614,  615,  616,  617,  618,  619,  620,
 /*    60 */   621,  622,  623,  624,  606,  595,  597,  598,  599,  600,
 /*    70 */   601,  602,  603,  604,  794,  794,  794,  794,  794,  794,
 /*    80 */   593,  794,  794,  594,  596,  580,  794,  794,  546,  794,
 /*    90 */   794,  547,  794,  553,  794,  554,  794,  794,  577,  794,
 /*   100 */   794,  794,  794,  794,  794,  583,  794,  585,  625,  627,
 /*   110 */   628,  629,  630,  631,  632,  633,  634,  635,  636,  637,
 /*   120 */   638,  639,  640,  641,  642,  643,  644,  794,  645,  794,
 /*   130 */   646,  794,  794,  794,  794,  649,  794,  794,  794,  650,
 /*   140 */   794,  794,  794,  653,  794,  654,  655,  794,  794,  657,
 /*   150 */   658,  794,  794,  794,  661,  794,  794,  794,  794,  663,
 /*   160 */   794,  794,  794,  794,  794,  794,  665,  794,  722,  794,
 /*   170 */   794,  723,  794,  794,  794,  724,  794,  726,  794,  727,
 /*   180 */   794,  728,  794,  729,  794,  730,  794,  731,  794,  732,
 /*   190 */   794,  739,  794,  740,  794,  741,  794,  742,  794,  743,
 /*   200 */   794,  744,  794,  745,  794,  746,  794,  794,  733,  794,
 /*   210 */   794,  734,  751,  794,  735,  794,  775,  794,  747,  794,
 /*   220 */   794,  794,  748,  794,  749,  750,  752,  753,  754,  755,
 /*   230 */   756,  757,  758,  794,  759,  794,  760,  794,  776,  794,
 /*   240 */   794,  778,  783,  784,  779,  784,  786,  785,  777,  794,
 /*   250 */   761,  794,  762,  794,  763,  794,  770,  771,  772,  773,
 /*   260 */   774,  794,  768,  769,  794,  736,  794,  794,  766,  767,
 /*   270 */   737,  794,  794,  738,  794,  794,  764,  765,  794,  791,
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
 /* 248 */ "xx_array_value ::= xx_literal_expr",
 /* 249 */ "xx_array_value ::= IDENTIFIER",
 /* 250 */ "xx_literal_expr ::= INTEGER",
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
// 896 "parser.lemon"
{
	/*if ((yypminor->yy0)) {
		if ((yypminor->yy0)->free_flag) {
			efree((yypminor->yy0)->token);
		}
		efree((yypminor->yy0));
	}*/
}
// 2277 "parser.c"
      break;
    case 99:
// 909 "parser.lemon"
{ json_object_put((yypminor->yy175)); }
// 2282 "parser.c"
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
// 2758 "parser.c"
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
// 911 "parser.lemon"
{
	yygotominor.yy175 = yymsp[0].minor.yy175;
}
// 2795 "parser.c"
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
// 2809 "parser.c"
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
// 2830 "parser.c"
        break;
      case 8:
// 939 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_namespace(yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(31,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 2839 "parser.c"
        break;
      case 9:
// 943 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_interface(yymsp[-1].minor.yy0, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(34,&yymsp[-2].minor);
}
// 2847 "parser.c"
        break;
      case 10:
// 947 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_interface(yymsp[-3].minor.yy0, yymsp[0].minor.yy175, yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(34,&yymsp[-4].minor);
  yy_destructor(35,&yymsp[-2].minor);
}
// 2856 "parser.c"
        break;
      case 11:
// 951 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class(yymsp[-1].minor.yy0, yymsp[0].minor.yy175, 0, 0, NULL, NULL, status->scanner_state);
  yy_destructor(36,&yymsp[-2].minor);
}
// 2864 "parser.c"
        break;
      case 12:
// 955 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy175, 0, 0, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(36,&yymsp[-4].minor);
  yy_destructor(35,&yymsp[-2].minor);
}
// 2873 "parser.c"
        break;
      case 13:
// 959 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy175, 0, 0, NULL, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(36,&yymsp[-4].minor);
  yy_destructor(37,&yymsp[-2].minor);
}
// 2882 "parser.c"
        break;
      case 14:
// 963 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class(yymsp[-1].minor.yy0, yymsp[0].minor.yy175, 1, 0, NULL, NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-3].minor);
  yy_destructor(36,&yymsp[-2].minor);
}
// 2891 "parser.c"
        break;
      case 15:
// 967 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy175, 1, 0, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-5].minor);
  yy_destructor(36,&yymsp[-4].minor);
  yy_destructor(35,&yymsp[-2].minor);
}
// 2901 "parser.c"
        break;
      case 16:
// 971 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class(yymsp[-1].minor.yy0, yymsp[0].minor.yy175, 0, 1, NULL, NULL, status->scanner_state);
  yy_destructor(39,&yymsp[-3].minor);
  yy_destructor(36,&yymsp[-2].minor);
}
// 2910 "parser.c"
        break;
      case 17:
// 975 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy175, 0, 1, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(39,&yymsp[-5].minor);
  yy_destructor(36,&yymsp[-4].minor);
  yy_destructor(35,&yymsp[-2].minor);
}
// 2920 "parser.c"
        break;
      case 18:
      case 23:
// 979 "parser.lemon"
{
	yygotominor.yy175 = NULL;
  yy_destructor(40,&yymsp[-1].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 2930 "parser.c"
        break;
      case 19:
      case 24:
// 983 "parser.lemon"
{
	yygotominor.yy175 = yymsp[-1].minor.yy175;
  yy_destructor(40,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 2940 "parser.c"
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
// 2954 "parser.c"
        break;
      case 22:
      case 216:
      case 245:
      case 249:
// 995 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state);
}
// 2964 "parser.c"
        break;
      case 25:
// 1007 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_definition(yymsp[0].minor.yy175, NULL, NULL, status->scanner_state);
}
// 2971 "parser.c"
        break;
      case 26:
// 1011 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_definition(NULL, NULL, yymsp[0].minor.yy175, status->scanner_state);
}
// 2978 "parser.c"
        break;
      case 27:
// 1015 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_definition(NULL, yymsp[0].minor.yy175, NULL, status->scanner_state);
}
// 2985 "parser.c"
        break;
      case 28:
// 1019 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_definition(yymsp[-1].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
}
// 2992 "parser.c"
        break;
      case 29:
// 1023 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_definition(yymsp[-1].minor.yy175, NULL, yymsp[0].minor.yy175, status->scanner_state);
}
// 2999 "parser.c"
        break;
      case 30:
// 1027 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_definition(yymsp[0].minor.yy175, NULL, yymsp[-1].minor.yy175, status->scanner_state);
}
// 3006 "parser.c"
        break;
      case 31:
// 1031 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_definition(NULL, yymsp[0].minor.yy175, yymsp[-1].minor.yy175, status->scanner_state);
}
// 3013 "parser.c"
        break;
      case 32:
// 1035 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_definition(yymsp[-2].minor.yy175, yymsp[0].minor.yy175, yymsp[-1].minor.yy175, status->scanner_state);
}
// 3020 "parser.c"
        break;
      case 33:
// 1039 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_definition(yymsp[-1].minor.yy175, yymsp[0].minor.yy175, yymsp[-2].minor.yy175, status->scanner_state);
}
// 3027 "parser.c"
        break;
      case 36:
// 1055 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_property(yymsp[-2].minor.yy175, yymsp[-1].minor.yy0, NULL, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(33,&yymsp[0].minor);
}
// 3035 "parser.c"
        break;
      case 37:
// 1059 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_property(yymsp[-2].minor.yy175, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(33,&yymsp[0].minor);
}
// 3043 "parser.c"
        break;
      case 38:
// 1063 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_property(yymsp[-4].minor.yy175, yymsp[-3].minor.yy0, yymsp[-1].minor.yy175, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(43,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 3052 "parser.c"
        break;
      case 39:
// 1067 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_property(yymsp[-4].minor.yy175, yymsp[-3].minor.yy0, yymsp[-1].minor.yy175, NULL, status->scanner_state);
  yy_destructor(43,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 3061 "parser.c"
        break;
      case 46:
// 1095 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy175, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(44,&yymsp[-4].minor);
  yy_destructor(43,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 3071 "parser.c"
        break;
      case 47:
// 1099 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy175, NULL, status->scanner_state);
  yy_destructor(44,&yymsp[-4].minor);
  yy_destructor(43,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 3081 "parser.c"
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
// 3093 "parser.c"
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
// 3105 "parser.c"
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
// 3117 "parser.c"
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
// 3129 "parser.c"
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
// 3141 "parser.c"
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
// 3153 "parser.c"
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
// 3165 "parser.c"
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
// 3177 "parser.c"
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
// 3188 "parser.c"
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
// 3199 "parser.c"
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
// 3210 "parser.c"
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
// 3221 "parser.c"
        break;
      case 62:
// 1159 "parser.lemon"
{
	yygotominor.yy175 = json_object_new_string("public");
  yy_destructor(1,&yymsp[0].minor);
}
// 3229 "parser.c"
        break;
      case 63:
// 1163 "parser.lemon"
{
	yygotominor.yy175 = json_object_new_string("protected");
  yy_destructor(2,&yymsp[0].minor);
}
// 3237 "parser.c"
        break;
      case 64:
// 1167 "parser.lemon"
{
	yygotominor.yy175 = json_object_new_string("private");
  yy_destructor(4,&yymsp[0].minor);
}
// 3245 "parser.c"
        break;
      case 65:
// 1171 "parser.lemon"
{
	yygotominor.yy175 = json_object_new_string("static");
  yy_destructor(3,&yymsp[0].minor);
}
// 3253 "parser.c"
        break;
      case 66:
// 1175 "parser.lemon"
{
	yygotominor.yy175 = json_object_new_string("scoped");
  yy_destructor(5,&yymsp[0].minor);
}
// 3261 "parser.c"
        break;
      case 67:
// 1179 "parser.lemon"
{
	yygotominor.yy175 = json_object_new_string("inline");
  yy_destructor(48,&yymsp[0].minor);
}
// 3269 "parser.c"
        break;
      case 68:
// 1183 "parser.lemon"
{
	yygotominor.yy175 = json_object_new_string("abstract");
  yy_destructor(38,&yymsp[0].minor);
}
// 3277 "parser.c"
        break;
      case 69:
// 1187 "parser.lemon"
{
	yygotominor.yy175 = json_object_new_string("final");
  yy_destructor(39,&yymsp[0].minor);
}
// 3285 "parser.c"
        break;
      case 72:
// 1199 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_parameter(NULL, NULL, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 3292 "parser.c"
        break;
      case 73:
// 1203 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_parameter(yymsp[-1].minor.yy175, NULL, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 3299 "parser.c"
        break;
      case 74:
// 1207 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_parameter(NULL, yymsp[-1].minor.yy175, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 3306 "parser.c"
        break;
      case 75:
// 1211 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_parameter(NULL, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(43,&yymsp[-1].minor);
}
// 3314 "parser.c"
        break;
      case 76:
// 1215 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_parameter(yymsp[-3].minor.yy175, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(43,&yymsp[-1].minor);
}
// 3322 "parser.c"
        break;
      case 77:
// 1219 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_parameter(NULL, yymsp[-3].minor.yy175, yymsp[-2].minor.yy0, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(43,&yymsp[-1].minor);
}
// 3330 "parser.c"
        break;
      case 78:
// 1223 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(12,&yymsp[-2].minor);
  yy_destructor(13,&yymsp[0].minor);
}
// 3339 "parser.c"
        break;
      case 79:
// 1227 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_type(XX_TYPE_INTEGER);
  yy_destructor(49,&yymsp[0].minor);
}
// 3347 "parser.c"
        break;
      case 80:
// 1231 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_type(XX_TYPE_UINTEGER);
  yy_destructor(50,&yymsp[0].minor);
}
// 3355 "parser.c"
        break;
      case 81:
// 1235 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_type(XX_TYPE_LONG);
  yy_destructor(51,&yymsp[0].minor);
}
// 3363 "parser.c"
        break;
      case 82:
// 1239 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_type(XX_TYPE_ULONG);
  yy_destructor(52,&yymsp[0].minor);
}
// 3371 "parser.c"
        break;
      case 83:
// 1243 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_type(XX_TYPE_CHAR);
  yy_destructor(53,&yymsp[0].minor);
}
// 3379 "parser.c"
        break;
      case 84:
// 1247 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_type(XX_TYPE_UCHAR);
  yy_destructor(54,&yymsp[0].minor);
}
// 3387 "parser.c"
        break;
      case 85:
// 1251 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_type(XX_TYPE_DOUBLE);
  yy_destructor(55,&yymsp[0].minor);
}
// 3395 "parser.c"
        break;
      case 86:
// 1255 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_type(XX_TYPE_BOOL);
  yy_destructor(56,&yymsp[0].minor);
}
// 3403 "parser.c"
        break;
      case 87:
// 1259 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_type(XX_TYPE_STRING);
  yy_destructor(57,&yymsp[0].minor);
}
// 3411 "parser.c"
        break;
      case 88:
// 1263 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_type(XX_TYPE_ARRAY);
  yy_destructor(58,&yymsp[0].minor);
}
// 3419 "parser.c"
        break;
      case 89:
// 1267 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_type(XX_TYPE_VAR);
  yy_destructor(59,&yymsp[0].minor);
}
// 3427 "parser.c"
        break;
      case 110:
// 1351 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_break_statement(status->scanner_state);
  yy_destructor(60,&yymsp[-1].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 3436 "parser.c"
        break;
      case 111:
// 1355 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_continue_statement(status->scanner_state);
  yy_destructor(61,&yymsp[-1].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 3445 "parser.c"
        break;
      case 112:
// 1359 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_if_statement(yymsp[-2].minor.yy175, NULL, NULL, status->scanner_state);
  yy_destructor(62,&yymsp[-3].minor);
  yy_destructor(40,&yymsp[-1].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3455 "parser.c"
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
// 3468 "parser.c"
        break;
      case 114:
// 1367 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_if_statement(yymsp[-3].minor.yy175, yymsp[-1].minor.yy175, NULL, status->scanner_state);
  yy_destructor(62,&yymsp[-4].minor);
  yy_destructor(40,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3478 "parser.c"
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
// 3491 "parser.c"
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
// 3504 "parser.c"
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
// 3517 "parser.c"
        break;
      case 118:
// 1383 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_switch_statement(yymsp[-2].minor.yy175, NULL, status->scanner_state);
  yy_destructor(64,&yymsp[-3].minor);
  yy_destructor(40,&yymsp[-1].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3527 "parser.c"
        break;
      case 119:
// 1387 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_switch_statement(yymsp[-3].minor.yy175, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(64,&yymsp[-4].minor);
  yy_destructor(40,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3537 "parser.c"
        break;
      case 122:
// 1399 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_case_clause(yymsp[-1].minor.yy175, NULL, status->scanner_state);
  yy_destructor(65,&yymsp[-2].minor);
  yy_destructor(66,&yymsp[0].minor);
}
// 3546 "parser.c"
        break;
      case 123:
// 1403 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_case_clause(yymsp[-2].minor.yy175, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(65,&yymsp[-3].minor);
  yy_destructor(66,&yymsp[-1].minor);
}
// 3555 "parser.c"
        break;
      case 124:
// 1407 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_case_clause(NULL, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(67,&yymsp[-2].minor);
  yy_destructor(66,&yymsp[-1].minor);
}
// 3564 "parser.c"
        break;
      case 125:
// 1411 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_loop_statement(NULL, status->scanner_state);
  yy_destructor(68,&yymsp[-2].minor);
  yy_destructor(40,&yymsp[-1].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3574 "parser.c"
        break;
      case 126:
// 1415 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_loop_statement(yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(68,&yymsp[-3].minor);
  yy_destructor(40,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3584 "parser.c"
        break;
      case 127:
// 1419 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_while_statement(yymsp[-2].minor.yy175, NULL, status->scanner_state);
  yy_destructor(69,&yymsp[-3].minor);
  yy_destructor(40,&yymsp[-1].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3594 "parser.c"
        break;
      case 128:
// 1423 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_while_statement(yymsp[-3].minor.yy175, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(69,&yymsp[-4].minor);
  yy_destructor(40,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3604 "parser.c"
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
// 3616 "parser.c"
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
// 3628 "parser.c"
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
// 3639 "parser.c"
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
// 3651 "parser.c"
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
// 3663 "parser.c"
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
// 3676 "parser.c"
        break;
      case 135:
// 1451 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_statement(yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(74,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 3685 "parser.c"
        break;
      case 138:
// 1464 "parser.lemon"
{
	yygotominor.yy175 = json_object_new_string("assign");
  yy_destructor(43,&yymsp[0].minor);
}
// 3693 "parser.c"
        break;
      case 139:
// 1469 "parser.lemon"
{
	yygotominor.yy175 = json_object_new_string("add-assign");
  yy_destructor(75,&yymsp[0].minor);
}
// 3701 "parser.c"
        break;
      case 140:
// 1474 "parser.lemon"
{
	yygotominor.yy175 = json_object_new_string("sub-assign");
  yy_destructor(76,&yymsp[0].minor);
}
// 3709 "parser.c"
        break;
      case 141:
// 1478 "parser.lemon"
{
	yygotominor.yy175 = json_object_new_string("mult-assign");
  yy_destructor(77,&yymsp[0].minor);
}
// 3717 "parser.c"
        break;
      case 142:
// 1482 "parser.lemon"
{
	yygotominor.yy175 = json_object_new_string("div-assign");
  yy_destructor(78,&yymsp[0].minor);
}
// 3725 "parser.c"
        break;
      case 143:
// 1486 "parser.lemon"
{
	yygotominor.yy175 = json_object_new_string("concat-assign");
  yy_destructor(79,&yymsp[0].minor);
}
// 3733 "parser.c"
        break;
      case 144:
// 1491 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("variable", yymsp[-1].minor.yy175, yymsp[-2].minor.yy0, NULL, NULL, yymsp[0].minor.yy175, status->scanner_state);
}
// 3740 "parser.c"
        break;
      case 145:
// 1496 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("object-property", yymsp[-1].minor.yy175, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(80,&yymsp[-3].minor);
}
// 3748 "parser.c"
        break;
      case 146:
// 1501 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("object-property-append", yymsp[-1].minor.yy175, yymsp[-6].minor.yy0, yymsp[-4].minor.yy0, NULL, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(80,&yymsp[-5].minor);
  yy_destructor(81,&yymsp[-3].minor);
  yy_destructor(82,&yymsp[-2].minor);
}
// 3758 "parser.c"
        break;
      case 147:
// 1506 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("object-property-array-index", yymsp[-1].minor.yy175, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, yymsp[-3].minor.yy175, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(80,&yymsp[-6].minor);
  yy_destructor(81,&yymsp[-4].minor);
  yy_destructor(82,&yymsp[-2].minor);
}
// 3768 "parser.c"
        break;
      case 148:
// 1511 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("static-property", yymsp[-1].minor.yy175, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(83,&yymsp[-3].minor);
}
// 3776 "parser.c"
        break;
      case 149:
// 1516 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("static-property-append", yymsp[-1].minor.yy175, yymsp[-6].minor.yy0, yymsp[-4].minor.yy0, NULL, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(83,&yymsp[-5].minor);
  yy_destructor(81,&yymsp[-3].minor);
  yy_destructor(82,&yymsp[-2].minor);
}
// 3786 "parser.c"
        break;
      case 150:
// 1521 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("static-property-array-index", yymsp[-1].minor.yy175, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, yymsp[-3].minor.yy175, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(83,&yymsp[-6].minor);
  yy_destructor(81,&yymsp[-4].minor);
  yy_destructor(82,&yymsp[-2].minor);
}
// 3796 "parser.c"
        break;
      case 151:
// 1526 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("variable-append", yymsp[-1].minor.yy175, yymsp[-4].minor.yy0, NULL, NULL, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(81,&yymsp[-3].minor);
  yy_destructor(82,&yymsp[-2].minor);
}
// 3805 "parser.c"
        break;
      case 152:
// 1531 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("array-index", yymsp[-1].minor.yy175, yymsp[-5].minor.yy0, NULL, yymsp[-3].minor.yy175, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(81,&yymsp[-4].minor);
  yy_destructor(82,&yymsp[-2].minor);
}
// 3814 "parser.c"
        break;
      case 153:
// 1536 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("incr", NULL, yymsp[-3].minor.yy0, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(80,&yymsp[-2].minor);
  yy_destructor(84,&yymsp[0].minor);
}
// 3823 "parser.c"
        break;
      case 154:
// 1541 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("decr", NULL, yymsp[-3].minor.yy0, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(80,&yymsp[-2].minor);
  yy_destructor(85,&yymsp[0].minor);
}
// 3832 "parser.c"
        break;
      case 155:
// 1546 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("incr", NULL, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(84,&yymsp[0].minor);
}
// 3840 "parser.c"
        break;
      case 156:
// 1551 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("decr", NULL, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(85,&yymsp[0].minor);
}
// 3848 "parser.c"
        break;
      case 158:
// 1559 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_echo_statement(yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(86,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 3857 "parser.c"
        break;
      case 161:
// 1571 "parser.lemon"
{
	yygotominor.yy175 = yymsp[0].minor.yy175;;
}
// 3864 "parser.c"
        break;
      case 162:
// 1576 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_mcall_statement(yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(33,&yymsp[0].minor);
}
// 3872 "parser.c"
        break;
      case 163:
// 1581 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_fcall_statement(yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(33,&yymsp[0].minor);
}
// 3880 "parser.c"
        break;
      case 164:
// 1586 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_scall_statement(yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(33,&yymsp[0].minor);
}
// 3888 "parser.c"
        break;
      case 165:
// 1591 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_return_statement(yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(87,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 3897 "parser.c"
        break;
      case 166:
// 1596 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_return_statement(NULL, status->scanner_state);
  yy_destructor(87,&yymsp[-1].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 3906 "parser.c"
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
// 3920 "parser.c"
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
// 3935 "parser.c"
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
// 3950 "parser.c"
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
// 3965 "parser.c"
        break;
      case 171:
// 1637 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_throw_exception(yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(91,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 3974 "parser.c"
        break;
      case 172:
// 1641 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_statement(XX_T_TYPE_INTEGER, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 3983 "parser.c"
        break;
      case 173:
// 1645 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_statement(XX_T_TYPE_UINTEGER, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(50,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 3992 "parser.c"
        break;
      case 174:
// 1649 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_statement(XX_T_TYPE_CHAR, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(53,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 4001 "parser.c"
        break;
      case 175:
// 1653 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_statement(XX_T_TYPE_UCHAR, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 4010 "parser.c"
        break;
      case 176:
// 1657 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_statement(XX_T_TYPE_LONG, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(51,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 4019 "parser.c"
        break;
      case 177:
// 1661 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_statement(XX_T_TYPE_ULONG, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(52,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 4028 "parser.c"
        break;
      case 178:
// 1665 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_statement(XX_T_TYPE_DOUBLE, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(55,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 4037 "parser.c"
        break;
      case 179:
// 1669 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_statement(XX_T_TYPE_STRING, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(57,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 4046 "parser.c"
        break;
      case 180:
// 1673 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_statement(XX_T_TYPE_BOOL, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(56,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 4055 "parser.c"
        break;
      case 181:
// 1677 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_statement(XX_T_TYPE_VAR, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(59,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[0].minor);
}
// 4064 "parser.c"
        break;
      case 184:
// 1689 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_variable(yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 4071 "parser.c"
        break;
      case 185:
// 1693 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_variable(yymsp[-2].minor.yy0, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(43,&yymsp[-1].minor);
}
// 4079 "parser.c"
        break;
      case 187:
// 1701 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("not", yymsp[0].minor.yy175, NULL, NULL, status->scanner_state);
  yy_destructor(29,&yymsp[-1].minor);
}
// 4087 "parser.c"
        break;
      case 188:
// 1705 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("isset", yymsp[0].minor.yy175, NULL, NULL, status->scanner_state);
  yy_destructor(24,&yymsp[-1].minor);
}
// 4095 "parser.c"
        break;
      case 189:
// 1709 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("require", yymsp[0].minor.yy175, NULL, NULL, status->scanner_state);
  yy_destructor(7,&yymsp[-1].minor);
}
// 4103 "parser.c"
        break;
      case 190:
// 1713 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("equals", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(10,&yymsp[-1].minor);
}
// 4111 "parser.c"
        break;
      case 191:
// 1717 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("not-equals", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(17,&yymsp[-1].minor);
}
// 4119 "parser.c"
        break;
      case 192:
// 1721 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("identical", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(11,&yymsp[-1].minor);
}
// 4127 "parser.c"
        break;
      case 193:
// 1725 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("not-identical", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(16,&yymsp[-1].minor);
}
// 4135 "parser.c"
        break;
      case 194:
// 1729 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("less", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(12,&yymsp[-1].minor);
}
// 4143 "parser.c"
        break;
      case 195:
// 1733 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("greater", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(13,&yymsp[-1].minor);
}
// 4151 "parser.c"
        break;
      case 196:
// 1737 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("less-equal", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(14,&yymsp[-1].minor);
}
// 4159 "parser.c"
        break;
      case 197:
// 1741 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("greater-equal", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(15,&yymsp[-1].minor);
}
// 4167 "parser.c"
        break;
      case 198:
// 1745 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("list", yymsp[-1].minor.yy175, NULL, NULL, status->scanner_state);
  yy_destructor(47,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 4176 "parser.c"
        break;
      case 199:
// 1749 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("cast", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(47,&yymsp[-3].minor);
  yy_destructor(30,&yymsp[-1].minor);
}
// 4185 "parser.c"
        break;
      case 200:
// 1753 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("property-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(80,&yymsp[-1].minor);
}
// 4193 "parser.c"
        break;
      case 201:
// 1757 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("static-property-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(83,&yymsp[-1].minor);
}
// 4201 "parser.c"
        break;
      case 202:
// 1761 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("static-constant-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(83,&yymsp[-1].minor);
}
// 4209 "parser.c"
        break;
      case 203:
      case 256:
// 1766 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), yymsp[-1].minor.yy175, NULL, status->scanner_state);
  yy_destructor(81,&yymsp[-2].minor);
  yy_destructor(82,&yymsp[0].minor);
}
// 4219 "parser.c"
        break;
      case 204:
// 1771 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("add", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(18,&yymsp[-1].minor);
}
// 4227 "parser.c"
        break;
      case 205:
// 1776 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("sub", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(19,&yymsp[-1].minor);
}
// 4235 "parser.c"
        break;
      case 206:
// 1781 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("mul", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(21,&yymsp[-1].minor);
}
// 4243 "parser.c"
        break;
      case 207:
// 1786 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("div", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(22,&yymsp[-1].minor);
}
// 4251 "parser.c"
        break;
      case 208:
// 1791 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("mod", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(23,&yymsp[-1].minor);
}
// 4259 "parser.c"
        break;
      case 209:
// 1796 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("concat", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(20,&yymsp[-1].minor);
}
// 4267 "parser.c"
        break;
      case 210:
// 1801 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("and", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(8,&yymsp[-1].minor);
}
// 4275 "parser.c"
        break;
      case 211:
// 1806 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("or", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(9,&yymsp[-1].minor);
}
// 4283 "parser.c"
        break;
      case 212:
// 1811 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("instanceof", yymsp[0].minor.yy175, NULL, NULL, status->scanner_state);
  yy_destructor(27,&yymsp[-1].minor);
}
// 4291 "parser.c"
        break;
      case 213:
// 1816 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("fetch", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(25,&yymsp[-3].minor);
  yy_destructor(6,&yymsp[-1].minor);
}
// 4300 "parser.c"
        break;
      case 214:
// 1821 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("typeof", yymsp[0].minor.yy175, NULL, NULL, status->scanner_state);
  yy_destructor(26,&yymsp[-1].minor);
}
// 4308 "parser.c"
        break;
      case 215:
// 1826 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_literal(XX_T_CONSTANT, yymsp[0].minor.yy0, status->scanner_state);
}
// 4315 "parser.c"
        break;
      case 217:
      case 247:
      case 250:
// 1836 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_literal(XX_T_INTEGER, yymsp[0].minor.yy0, status->scanner_state);
}
// 4324 "parser.c"
        break;
      case 218:
      case 246:
      case 251:
// 1841 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_literal(XX_T_STRING, yymsp[0].minor.yy0, status->scanner_state);
}
// 4333 "parser.c"
        break;
      case 219:
// 1846 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_literal(XX_T_CHAR, yymsp[0].minor.yy0, status->scanner_state);
}
// 4340 "parser.c"
        break;
      case 220:
      case 252:
// 1851 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_literal(XX_T_DOUBLE, yymsp[0].minor.yy0, status->scanner_state);
}
// 4348 "parser.c"
        break;
      case 221:
      case 253:
// 1856 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_literal(XX_T_NULL, NULL, status->scanner_state);
  yy_destructor(94,&yymsp[0].minor);
}
// 4357 "parser.c"
        break;
      case 222:
      case 255:
// 1861 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_literal(XX_T_TRUE, NULL, status->scanner_state);
  yy_destructor(95,&yymsp[0].minor);
}
// 4366 "parser.c"
        break;
      case 223:
      case 254:
// 1866 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_literal(XX_T_FALSE, NULL, status->scanner_state);
  yy_destructor(96,&yymsp[0].minor);
}
// 4375 "parser.c"
        break;
      case 224:
// 1871 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("empty-array", NULL, NULL, NULL, status->scanner_state);
  yy_destructor(81,&yymsp[-1].minor);
  yy_destructor(82,&yymsp[0].minor);
}
// 4384 "parser.c"
        break;
      case 225:
// 1876 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("array", yymsp[-1].minor.yy175, NULL, NULL, status->scanner_state);
  yy_destructor(81,&yymsp[-2].minor);
  yy_destructor(82,&yymsp[0].minor);
}
// 4393 "parser.c"
        break;
      case 226:
// 1881 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_new_instance(yymsp[0].minor.yy0, NULL, status->scanner_state);
  yy_destructor(28,&yymsp[-1].minor);
}
// 4401 "parser.c"
        break;
      case 227:
// 1886 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_new_instance(yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(28,&yymsp[-3].minor);
  yy_destructor(47,&yymsp[-1].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 4411 "parser.c"
        break;
      case 228:
// 1891 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_new_instance(yymsp[-3].minor.yy0, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(28,&yymsp[-4].minor);
  yy_destructor(47,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 4421 "parser.c"
        break;
      case 229:
// 1896 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_fcall(yymsp[-3].minor.yy0, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(47,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 4430 "parser.c"
        break;
      case 230:
// 1901 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_fcall(yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(47,&yymsp[-1].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 4439 "parser.c"
        break;
      case 231:
// 1906 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_scall(yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(83,&yymsp[-4].minor);
  yy_destructor(47,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 4449 "parser.c"
        break;
      case 232:
// 1911 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_scall(yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(83,&yymsp[-3].minor);
  yy_destructor(47,&yymsp[-1].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 4459 "parser.c"
        break;
      case 233:
// 1916 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_mcall(yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(80,&yymsp[-4].minor);
  yy_destructor(47,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 4469 "parser.c"
        break;
      case 234:
// 1921 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_mcall(yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(80,&yymsp[-3].minor);
  yy_destructor(47,&yymsp[-1].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 4479 "parser.c"
        break;
      case 243:
// 1960 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_array_item(yymsp[-2].minor.yy175, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(66,&yymsp[-1].minor);
}
// 4487 "parser.c"
        break;
      case 244:
// 1964 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_array_item(NULL, yymsp[0].minor.yy175, status->scanner_state);
}
// 4494 "parser.c"
        break;
      case 258:
// 2021 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_comment(yymsp[0].minor.yy0, status->scanner_state);
}
// 4501 "parser.c"
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

// 4577 "parser.c"
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
