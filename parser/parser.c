/** The author disclaims copyright to this source code.
*/
/* First off, code is include which follows the "include" declaration
** in the input file. */
#include <stdio.h>
// 45 "parser.lemon"


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

static json_object *xx_ret_class(xx_parser_token *T, json_object *class_definition, int is_abstract, int is_final, xx_parser_token *E, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("class"));
	json_object_object_add(ret, "name", json_object_new_string(T->token));

	json_object_object_add(ret, "abtract", json_object_new_int(is_abstract));
	json_object_object_add(ret, "final", json_object_new_int(is_final));

	if (E) {
		json_object_object_add(ret, "extends", json_object_new_string(E->token));
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

static json_object *xx_ret_class_method(json_object *visibility, xx_parser_token *T, json_object *parameters, json_object *statements, xx_parser_token *D, xx_scanner_state *state)
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


// 803 "parser.c"
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
#define YYNOCODE 163
#define YYACTIONTYPE unsigned short int
#define xx_TOKENTYPE xx_parser_token*
typedef union {
  xx_TOKENTYPE yy0;
  json_object* yy259;
  int yy325;
} YYMINORTYPE;
#define YYSTACKDEPTH 100
#define xx_ARG_SDECL xx_parser_status *status;
#define xx_ARG_PDECL ,xx_parser_status *status
#define xx_ARG_FETCH xx_parser_status *status = yypParser->status
#define xx_ARG_STORE yypParser->status = status
#define YYNSTATE 523
#define YYNRULE 250
#define YYERRORSYMBOL 95
#define YYERRSYMDT yy325
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
 /*     0 */   168,  222,  226,  220,  252,  165,  245,   88,  215,  267,
 /*    10 */    93,   91,   95,  501,  507,  506,  494,   64,  500,  474,
 /*    20 */   228,  272,  209,   83,   52,   53,   54,   55,   56,   57,
 /*    30 */    58,   59,   60,   61,   62,  205,  207,  167,  181,  185,
 /*    40 */   187,  189,  191,  183,  179,  193,  195,  203,  197,  199,
 /*    50 */   201,  347,  241,  242,  247,  248,  236,  193,  195,  203,
 /*    60 */   197,  199,  201,  349,  229,  230,  408,  231,  232,  233,
 /*    70 */   234,  235,   35,   36,  462,  413,   37,   38,   40,   39,
 /*    80 */    79,  245,  372,  381,  390,  393,  384,  387,  396,  402,
 /*    90 */   399,  219,  405,  125,  127,  129,  511,  138,   13,  259,
 /*   100 */   260,  149,  153,  158,  281,  256,  262,  288,  672,  403,
 /*   110 */   380,  250,  249,  164,  408,   37,   38,   40,   39,  339,
 /*   120 */   350,  354,  447,  413,  369,  294,   31,   41,   45,  244,
 /*   130 */   372,  381,  390,  393,  384,  387,  396,  402,  399,  219,
 /*   140 */   405,  125,  127,  129,  212,  138,  355,  259,  260,  149,
 /*   150 */   153,  158,  281,  269,  262,  288,  202,  295,  296,  297,
 /*   160 */   298,  299,  408,  320,  259,  260,  210,  339,  350,  354,
 /*   170 */   137,  413,  369,  294,  259,  260,  289,  338,  372,  381,
 /*   180 */   390,  393,  384,  387,  396,  402,  399,  219,  405,  125,
 /*   190 */   127,  129,  357,  138,  358,  259,  260,  149,  153,  158,
 /*   200 */   281,  277,  262,  288,  274,  295,  296,  297,  298,  299,
 /*   210 */   408,  307,  259,  260,  240,  339,  350,  354,  287,  413,
 /*   220 */   369,    3,    4,    5,    6,    7,  372,  381,  390,  393,
 /*   230 */   384,  387,  396,  402,  399,  315,  405,  125,  127,  129,
 /*   240 */   324,  138,  488,  259,  260,  149,  153,  158,  281,   83,
 /*   250 */   502,  288,  505,  506,  494,  513,  473,  474,  408,  163,
 /*   260 */   361,   83,  280,  339,  350,  354,  454,  413,  369,  294,
 /*   270 */   259,  260,  356,  359,  372,  381,  390,  393,  384,  387,
 /*   280 */   396,  402,  399,  219,  405,  125,  127,  129,  239,  138,
 /*   290 */   362,  259,  260,  149,  153,  158,  281,  263,  262,  288,
 /*   300 */   178,  295,  296,  297,  298,  299,  408,   91,  259,  260,
 /*   310 */   344,  339,  350,  354,  443,  413,  369,  343,  259,  260,
 /*   320 */    82,  364,  372,  381,  390,  393,  384,  387,  396,  402,
 /*   330 */   399,  315,  405,  125,  127,  129,  332,  138,  365,  259,
 /*   340 */   260,  149,  153,  158,  281,    8,  139,  288,   11,  280,
 /*   350 */    85,  508,  515,  245,  408,  413,  367,  259,  260,  339,
 /*   360 */   350,  354,  431,  413,  369,  197,  199,  201,  375,  368,
 /*   370 */   372,  381,  390,  393,  384,  387,  396,  402,  399,  315,
 /*   380 */   405,  125,  127,  129,  311,  138,   21,  259,  260,  149,
 /*   390 */   153,  158,  281,   20,  374,  288,  238,  344,   10,  251,
 /*   400 */   242,  247,  408,  340,  345,  259,  260,  339,  350,  354,
 /*   410 */   470,  413,  369,  397,  380,  375,  394,  380,  372,  381,
 /*   420 */   390,  393,  384,  387,  396,  402,  399,  284,  405,  125,
 /*   430 */   127,  129,   15,  138,  514,  259,  260,  149,  153,  158,
 /*   440 */   281,  407,   19,  288,  472,  213,   98,   23,  473,  474,
 /*   450 */   408,   84,  490,   83,   83,  339,  350,  354,  133,  413,
 /*   460 */   369,  491,  388,  380,  400,  380,  372,  381,  390,  393,
 /*   470 */   384,  387,  396,  402,  399,  219,  405,  125,  127,  129,
 /*   480 */   103,  138,  377,  259,  260,  149,  153,  158,  281,   74,
 /*   490 */   261,  288,  503,  486,  494,   25,   83,  474,  408,  434,
 /*   500 */   376,   83,  280,  339,  350,  354,  442,  413,  369,  411,
 /*   510 */   259,  260,  409,  378,  372,  381,  390,  393,  384,  387,
 /*   520 */   396,  402,  399,  227,  405,  125,  127,  129,   86,  138,
 /*   530 */   456,  259,  260,  149,  153,  158,  281,   83,  449,  288,
 /*   540 */   302,   63,   41,   45,  301,  379,  408,  130,  259,  260,
 /*   550 */   280,  339,  350,  354,  468,  413,  369,  519,  259,  260,
 /*   560 */   217,   91,  372,  381,  390,  393,  384,  387,  396,  402,
 /*   570 */   399,  206,  405,  125,  127,  129,   26,  138,  516,  259,
 /*   580 */   260,  149,  153,  158,  281,   28,  258,  288,   63,   41,
 /*   590 */    45,  218,  465,   87,  408,   63,   41,   45,  188,  339,
 /*   600 */   350,  354,  437,  413,  369,   17,  259,  260,  126,  257,
 /*   610 */   372,  381,  390,  393,  384,  387,  396,  402,  399,  360,
 /*   620 */   405,  125,  127,  129,   14,  138,  373,  380,   17,  149,
 /*   630 */   153,  158,  281,   30,   80,  288,  302,   63,   41,   45,
 /*   640 */   323,  420,  408,  154,  259,  260,  280,  339,  350,  354,
 /*   650 */   464,  413,  369,  512,  259,  260,  466,   91,  372,  381,
 /*   660 */   390,  393,  384,  387,  396,  402,  399,  172,  405,  125,
 /*   670 */   127,  129,  496,  138,  223,  363,  366,  149,  153,  158,
 /*   680 */   281,  497,   98,  288,  302,   97,  391,  380,  319,   83,
 /*   690 */   408,   89,  259,  260,  370,  339,  350,  354,  448,  413,
 /*   700 */   369,  276,  259,  260,  224,  283,  372,  381,  390,  393,
 /*   710 */   384,  387,  396,  402,  399,  170,  405,  125,  127,  129,
 /*   720 */   225,  138,   42,  174,  176,  149,  153,  158,  281,  128,
 /*   730 */   258,  288,  302,   90,  216,  273,  331,  266,  408,  517,
 /*   740 */   259,  260,  423,  339,  350,  354,  432,  413,  369,  131,
 /*   750 */   259,  260,  326,  278,  372,  381,  390,  393,  384,  387,
 /*   760 */   396,  402,  399,  190,  405,  125,  127,  129,   43,  138,
 /*   770 */   410,  259,  260,  149,  153,  158,  281,  268,   23,  288,
 /*   780 */   302,  325,   22,  134,  335,   83,  408,  412,  259,  260,
 /*   790 */   351,  339,  350,  354,  419,  413,  369,  135,  259,  260,
 /*   800 */   322,  421,  372,  381,  390,  393,  384,  387,  396,  402,
 /*   810 */   399,  192,  405,  125,  127,  129,   16,  138,  422,  259,
 /*   820 */   260,  149,  153,  158,  281,  318,  258,  288,  302,  142,
 /*   830 */   441,  518,  327,  375,  408,  317,  259,  260,  194,  339,
 /*   840 */   350,  354,  452,  413,  369,  282,  259,  260,  291,  270,
 /*   850 */   372,  381,  390,  393,  384,  387,  396,  402,  399,  389,
 /*   860 */   405,  125,  127,  129,  520,  138,  385,  380,  140,  149,
 /*   870 */   153,  158,  281,   30,  290,  288,  302,  313,  382,  380,
 /*   880 */   310,  435,  408,  433,  259,  260,  196,  339,  350,  354,
 /*   890 */   161,  413,  369,   94,  259,  260,   81,   44,  372,  381,
 /*   900 */   390,  393,  384,  387,  396,  402,  399,  180,  405,  125,
 /*   910 */   127,  129,  243,  138,  523,  259,  260,  149,  153,  158,
 /*   920 */   281,  406,  380,  288,  302,  439,  312,  144,  306,  444,
 /*   930 */   408,  445,  259,  260,  200,  339,  350,  354,  426,  413,
 /*   940 */   369,  309,  259,  260,  375,  757,  372,  381,  390,  393,
 /*   950 */   384,  387,  396,  402,  399,  428,  405,  125,  127,  129,
 /*   960 */   758,  138,  759,  259,  260,  149,  153,  158,  281,  146,
 /*   970 */   398,  288,  302,  147,  451,  253,  314,  375,  408,  304,
 /*   980 */   259,  260,  204,  339,  350,  354,  157,  413,  369,  305,
 /*   990 */   259,  260,  375,  458,  372,  381,  390,  393,  384,  387,
 /*  1000 */   396,  402,  399,  383,  405,  125,  127,  129,  254,  138,
 /*  1010 */   459,  375,  461,  149,  153,  158,  281,   33,  386,  288,
 /*  1020 */   182,    9,  597,  177,   12,  150,  408,   46,  259,  260,
 /*  1030 */   198,  339,  350,  354,  436,  413,  369,  404,  259,  260,
 /*  1040 */   375,   51,  372,  381,  390,  393,  384,  387,  396,  402,
 /*  1050 */   399,  208,  405,  125,  127,  129,  467,  138,  175,  259,
 /*  1060 */   260,  149,  153,  158,  281,  173,  392,  288,  214,   50,
 /*  1070 */   300,  521,  478,  477,  408,  155,  259,  260,  221,  339,
 /*  1080 */   350,  354,  104,  413,  369,  479,  259,  260,  375,  480,
 /*  1090 */   372,  381,  390,  393,  384,  387,  396,  402,  399,  166,
 /*  1100 */   405,  125,  127,  129,  482,  138,  171,  259,  260,  149,
 /*  1110 */   153,  158,  281,  483,  401,  288,  416,  484,  485,  100,
 /*  1120 */   292,  375,  408,  293,  259,  260,  186,  339,  350,  354,
 /*  1130 */   152,  413,  369,  258,  259,  260,  342,   47,  372,  381,
 /*  1140 */   390,  393,  384,  387,  396,  402,  399,  395,  405,  125,
 /*  1150 */   127,  129,  159,  138,  492,  493,  264,  149,  153,  158,
 /*  1160 */   281,  101,  341,  288,  184,   48,  169,  162,  498,  499,
 /*  1170 */   408,  330,  259,  260,  333,  339,  350,  354,  106,  413,
 /*  1180 */   369,   65,   66,   68,   67,   69,  372,  381,  390,  393,
 /*  1190 */   384,  387,  396,  402,  399,  334,  405,  125,  127,  129,
 /*  1200 */    76,  138,   77,  509,  510,  149,  153,  158,  281,  414,
 /*  1210 */    34,  288,  414,  414,  414,  414,   71,   72,  408,  544,
 /*  1220 */   487,  414,  481,  339,  350,  354,   70,  413,  369,   65,
 /*  1230 */    66,   68,   67,   69,  372,  381,  390,  393,  384,  387,
 /*  1240 */   396,  402,  399,   30,  405,  125,  127,  129,  414,  138,
 /*  1250 */   414,  414,  414,  149,  153,  158,  281,  414,  414,  288,
 /*  1260 */   414,  414,  124,  414,   71,   72,  450,  548,  475,  414,
 /*  1270 */   481,  339,  350,  354,   70,  143,  369,  148,  414,  414,
 /*  1280 */   414,  414,  414,  108,  109,  110,  111,  112,  113,  114,
 /*  1290 */   115,  116,  117,  118,  119,  120,  121,  122,  123,  145,
 /*  1300 */   124,  438,  774,    1,    2,  522,    4,    5,    6,    7,
 /*  1310 */   346,  348,  414,  141,  414,  160,  414,  414,  414,  414,
 /*  1320 */   414,  108,  109,  110,  111,  112,  113,  114,  115,  116,
 /*  1330 */   117,  118,  119,  120,  121,  122,  123,  145,  124,  438,
 /*  1340 */   471,   96,  486,  494,  414,  500,  474,  414,  346,  348,
 /*  1350 */    83,  414,  414,  136,  414,  414,  414,  414,  414,  108,
 /*  1360 */   109,  110,  111,  112,  113,  114,  115,  116,  117,  118,
 /*  1370 */   119,  120,  121,  122,  123,  124,  414,   65,   66,   68,
 /*  1380 */    67,   69,  414,  414,  414,  414,  346,  348,  414,  414,
 /*  1390 */   105,  414,  414,  414,   30,  414,  108,  109,  110,  111,
 /*  1400 */   112,  113,  114,  115,  116,  117,  118,  119,  120,  121,
 /*  1410 */   122,  123,   71,   72,  124,   92,  487,   29,  481,  414,
 /*  1420 */   414,  414,   70,  346,  348,  414,  414,  414,  414,  132,
 /*  1430 */   414,  414,  414,  414,  414,  108,  109,  110,  111,  112,
 /*  1440 */   113,  114,  115,  116,  117,  118,  119,  120,  121,  122,
 /*  1450 */   123,  414,  124,  414,  414,  414,  414,  414,  414,  414,
 /*  1460 */   414,  414,  346,  348,  414,  414,  414,  156,  414,  414,
 /*  1470 */   414,  414,  414,  108,  109,  110,  111,  112,  113,  114,
 /*  1480 */   115,  116,  117,  118,  119,  120,  121,  122,  123,  414,
 /*  1490 */   124,  414,  414,  414,  414,  414,  414,  414,  414,  414,
 /*  1500 */   346,  348,  414,  414,  414,  151,  414,  414,  414,  414,
 /*  1510 */   414,  108,  109,  110,  111,  112,  113,  114,  115,  116,
 /*  1520 */   117,  118,  119,  120,  121,  122,  123,  124,  414,   65,
 /*  1530 */    66,   68,   67,   69,  414,  414,  414,  414,  346,  348,
 /*  1540 */   414,  414,  469,  414,  414,  414,  414,  414,  108,  109,
 /*  1550 */   110,  111,  112,  113,  114,  115,  116,  117,  118,  119,
 /*  1560 */   120,  121,  122,  123,   71,   72,  124,  545,  487,  414,
 /*  1570 */   481,  414,  414,  414,   70,  346,  348,  414,  414,  414,
 /*  1580 */   414,  463,  414,  414,  414,  414,  414,  108,  109,  110,
 /*  1590 */   111,  112,  113,  114,  115,  116,  117,  118,  119,  120,
 /*  1600 */   121,  122,  123,  414,  124,  414,  414,  414,  414,  414,
 /*  1610 */   414,  414,  414,  414,  346,  348,  414,  414,  414,  453,
 /*  1620 */   414,  414,  414,  414,  414,  108,  109,  110,  111,  112,
 /*  1630 */   113,  114,  115,  116,  117,  118,  119,  120,  121,  122,
 /*  1640 */   123,  414,  124,  414,  414,  414,  414,  414,  414,  414,
 /*  1650 */   414,  414,  346,  348,  414,  414,  414,  286,  414,  414,
 /*  1660 */   414,  414,  414,  108,  109,  110,  111,  112,  113,  114,
 /*  1670 */   115,  116,  117,  118,  119,  120,  121,  122,  123,  124,
 /*  1680 */   414,   65,   66,   68,   67,   69,  414,  414,  414,  414,
 /*  1690 */   346,  348,  414,  414,  446,  414,  414,  414,  414,  414,
 /*  1700 */   108,  109,  110,  111,  112,  113,  114,  115,  116,  117,
 /*  1710 */   118,  119,  120,  121,  122,  123,   71,   72,  124,  547,
 /*  1720 */   455,  414,  414,  414,  414,  414,   70,  346,  348,  414,
 /*  1730 */   414,  414,  414,  440,  414,  414,  414,  414,  414,  108,
 /*  1740 */   109,  110,  111,  112,  113,  114,  115,  116,  117,  118,
 /*  1750 */   119,  120,  121,  122,  123,  414,  124,  414,  414,  414,
 /*  1760 */   414,  414,  414,  414,  414,  414,  346,  348,  414,  414,
 /*  1770 */   414,  430,  414,  414,  414,  414,  414,  108,  109,  110,
 /*  1780 */   111,  112,  113,  114,  115,  116,  117,  118,  119,  120,
 /*  1790 */   121,  122,  123,  414,  124,  414,  414,  414,  414,  414,
 /*  1800 */   414,  414,  414,  414,  346,  348,  414,  414,  414,  425,
 /*  1810 */   414,  414,  414,  414,  414,  108,  109,  110,  111,  112,
 /*  1820 */   113,  114,  115,  116,  117,  118,  119,  120,  121,  122,
 /*  1830 */   123,  124,  414,   65,   66,   68,   67,   69,  414,  414,
 /*  1840 */   414,  414,  346,  348,  414,  414,  418,  414,  414,  414,
 /*  1850 */   414,  414,  108,  109,  110,  111,  112,  113,  114,  115,
 /*  1860 */   116,  117,  118,  119,  120,  121,  122,  123,   71,   72,
 /*  1870 */   124,  551,  455,  414,  414,  414,  414,  414,   70,  346,
 /*  1880 */   348,  414,  414,  414,  414,  414,  414,  414,  414,  414,
 /*  1890 */   107,  108,  109,  110,  111,  112,  113,  114,  115,  116,
 /*  1900 */   117,  118,  119,  120,  121,  122,  123,  414,  414,  414,
 /*  1910 */   414,  414,  414,  414,  414,  414,  414,  414,  346,  348,
 /*  1920 */   168,  222,  226,  220,  252,  165,  279,  414,  215,  414,
 /*  1930 */   168,  222,  226,  220,  252,  165,  271,  414,  215,  414,
 /*  1940 */   228,  414,  209,  414,  414,  414,  414,  414,  414,  414,
 /*  1950 */   228,  414,  209,  414,  414,  168,  222,  226,  220,  252,
 /*  1960 */   165,  414,  414,  215,  414,  414,  414,  414,  414,  414,
 /*  1970 */   414,  414,  294,  414,  414,  228,  236,  209,  414,  414,
 /*  1980 */   414,  414,  246,  414,  229,  230,  236,  231,  232,  233,
 /*  1990 */   234,  235,  414,  414,  229,  230,  414,  231,  232,  233,
 /*  2000 */   234,  235,  414,  415,  295,  296,  297,  298,  299,  303,
 /*  2010 */   328,  236,  316,  336,  337,  414,  414,  248,  414,  229,
 /*  2020 */   230,  414,  231,  232,  233,  234,  235,  168,  222,  226,
 /*  2030 */   220,  252,  165,  255,  414,  215,  414,  414,   35,   36,
 /*  2040 */   414,  414,   37,   38,   40,   39,  414,  228,  414,  209,
 /*  2050 */   168,  222,  226,  220,  252,  165,  414,  414,  215,  414,
 /*  2060 */   414,  414,  414,  414,  414,  414,  237,  414,  414,  414,
 /*  2070 */   228,  414,  209,  250,  249,  414,  414,   37,   38,   40,
 /*  2080 */    39,  414,  414,  236,  414,  414,  414,  414,  414,  414,
 /*  2090 */   414,  229,  230,  414,  231,  232,  233,  234,  235,  414,
 /*  2100 */   414,  414,  414,  414,  414,  414,  236,  308,  414,  414,
 /*  2110 */   414,  414,  414,  414,  229,  230,  414,  231,  232,  233,
 /*  2120 */   234,  235,  168,  222,  226,  220,  252,  165,  414,  414,
 /*  2130 */   215,  414,  414,  414,  414,  414,  414,  414,  414,  414,
 /*  2140 */   414,  414,  228,  414,  209,  168,  222,  226,  220,  252,
 /*  2150 */   165,  414,  414,  215,  414,  414,  414,  414,  414,  414,
 /*  2160 */   414,  414,  414,  414,  414,  228,  414,  209,  414,  414,
 /*  2170 */   414,  414,  414,  414,  414,  414,  414,  414,  236,  329,
 /*  2180 */   414,  414,  414,  414,  414,  414,  229,  230,  414,  231,
 /*  2190 */   232,  233,  234,  235,  414,  414,  414,  414,  414,  414,
 /*  2200 */   414,  236,  321,  168,  222,  226,  220,  252,  165,  229,
 /*  2210 */   230,  215,  231,  232,  233,  234,  235,  524,  414,  414,
 /*  2220 */   414,  414,  414,  228,  414,  209,  168,  222,  226,  220,
 /*  2230 */   252,  165,  414,  414,  215,  353,  168,  222,  226,  220,
 /*  2240 */   252,  165,  265,  414,  215,  414,  228,    8,  209,  414,
 /*  2250 */    11,  427,   85,  508,  515,  414,  228,  413,  209,  236,
 /*  2260 */   414,  168,  222,  226,  220,  252,  165,  229,  230,  215,
 /*  2270 */   231,  232,  233,  234,  235,   65,   66,   68,   67,   69,
 /*  2280 */   414,  228,  236,  209,  414,  414,  414,  414,  414,  414,
 /*  2290 */   229,  230,  236,  231,  232,  233,  234,  235,  414,  414,
 /*  2300 */   229,  230,  414,  231,  232,  233,  234,  235,  414,  414,
 /*  2310 */    71,   72,  414,  546,  455,  414,  414,  236,  414,  414,
 /*  2320 */    70,  414,  414,  414,  414,  229,  230,  414,  231,  232,
 /*  2330 */   233,  234,  235,  205,  207,  167,  181,  185,  187,  189,
 /*  2340 */   191,  183,  179,  193,  195,  203,  197,  199,  201,  414,
 /*  2350 */   414,  414,  414,  414,  414,  414,  414,  414,  352,  414,
 /*  2360 */   414,  414,  414,  414,  414,  414,  205,  207,  167,  181,
 /*  2370 */   185,  187,  189,  191,  183,  179,  193,  195,  203,  197,
 /*  2380 */   199,  201,  414,  414,  414,  414,  414,  414,  211,  205,
 /*  2390 */   207,  167,  181,  185,  187,  189,  191,  183,  179,  193,
 /*  2400 */   195,  203,  197,  199,  201,  414,  414,  414,  414,  414,
 /*  2410 */   414,  414,  414,  414,  371,  414,  414,  414,  414,  414,
 /*  2420 */   414,  414,  205,  207,  167,  181,  185,  187,  189,  191,
 /*  2430 */   183,  179,  193,  195,  203,  197,  199,  201,  167,  181,
 /*  2440 */   185,  187,  189,  191,  183,  179,  193,  195,  203,  197,
 /*  2450 */   199,  201,  414,  417,  414,  414,  414,  414,  414,  414,
 /*  2460 */   414,  205,  207,  167,  181,  185,  187,  189,  191,  183,
 /*  2470 */   179,  193,  195,  203,  197,  199,  201,  414,  414,  414,
 /*  2480 */   414,  414,  414,  414,  414,  414,  414,  414,  414,  414,
 /*  2490 */   414,  414,  424,  414,  414,  414,  414,  414,  414,  414,
 /*  2500 */   205,  207,  167,  181,  185,  187,  189,  191,  183,  179,
 /*  2510 */   193,  195,  203,  197,  199,  201,  414,  414,  414,  414,
 /*  2520 */   414,  414,  414,  414,  414,  414,  414,  414,  414,  414,
 /*  2530 */   414,  429,  414,  414,  414,  414,  414,  414,  414,  205,
 /*  2540 */   207,  167,  181,  185,  187,  189,  191,  183,  179,  193,
 /*  2550 */   195,  203,  197,  199,  201,  414,  414,  414,  414,  414,
 /*  2560 */   414,  414,  414,  414,  414,  414,  414,  414,  414,  414,
 /*  2570 */   285,  414,  414,  414,  414,  414,  414,  414,  205,  207,
 /*  2580 */   167,  181,  185,  187,  189,  191,  183,  179,  193,  195,
 /*  2590 */   203,  197,  199,  201,  205,  207,  167,  181,  185,  187,
 /*  2600 */   189,  191,  183,  179,  193,  195,  203,  197,  199,  201,
 /*  2610 */   414,  414,  414,  414,  414,  414,   49,  414,  414,  414,
 /*  2620 */   414,  414,  414,  414,  414,  414,  414,  414,   49,  414,
 /*  2630 */   414,  414,  414,  414,   27,  414,   32,  414,  414,  414,
 /*  2640 */   414,  414,  414,  414,  414,  414,  102,  414,   32,  414,
 /*  2650 */   414,  275,   52,   53,   54,   55,   56,   57,   58,   59,
 /*  2660 */    60,   61,   62,   49,   52,   53,   54,   55,   56,   57,
 /*  2670 */    58,   59,   60,   61,   62,   49,  414,  414,  414,  414,
 /*  2680 */   414,  460,  414,   32,  414,  414,  414,  414,  414,  414,
 /*  2690 */   414,  414,  414,   78,  414,   32,  414,  414,  414,   52,
 /*  2700 */    53,   54,   55,   56,   57,   58,   59,   60,   61,   62,
 /*  2710 */   414,   52,   53,   54,   55,   56,   57,   58,   59,   60,
 /*  2720 */    61,   62,  414,   49,  414,  414,  414,  414,  414,  414,
 /*  2730 */   414,  414,  414,  414,  414,  414,  414,  414,  414,  414,
 /*  2740 */   414,  414,  414,   32,  414,   65,   66,   68,   67,   69,
 /*  2750 */    65,   66,   68,   67,   69,  414,  414,  414,  414,   52,
 /*  2760 */    53,   54,   55,   56,   57,   58,   59,   60,   61,   62,
 /*  2770 */    65,   66,   68,   67,   69,   65,   66,   68,   67,   69,
 /*  2780 */    71,   72,  414,  550,  455,   71,   72,  414,  552,  455,
 /*  2790 */    70,  414,  414,  414,  414,   70,  414,   65,   66,   68,
 /*  2800 */    67,   69,  414,  414,  414,   71,   72,  414,  549,  504,
 /*  2810 */    71,   72,  414,   18,   73,   70,  414,  414,  414,  414,
 /*  2820 */    70,  414,   65,   66,   68,   67,   69,  495,  414,  414,
 /*  2830 */   414,  414,   71,   72,   65,   66,   68,   67,   69,  414,
 /*  2840 */    99,  414,   70,  414,  414,  414,  414,  414,  414,  414,
 /*  2850 */   414,  414,  489,  414,  414,  414,  414,   71,   72,   65,
 /*  2860 */    66,   68,   67,   69,  414,  457,  414,   70,  414,   71,
 /*  2870 */    72,  414,  414,  414,  414,  476,  414,  414,  414,   70,
 /*  2880 */   414,   65,   66,   68,   67,   69,   65,   66,   68,   67,
 /*  2890 */    69,  414,  414,  414,   71,   72,  414,   65,   66,   68,
 /*  2900 */    67,   69,   75,  414,   70,  414,  414,  414,  414,  414,
 /*  2910 */   414,  414,  414,  414,  414,  414,   71,   72,  414,  414,
 /*  2920 */   414,   71,   72,  414,   99,  414,   70,  414,  414,  457,
 /*  2930 */   414,   70,   71,   72,  414,  414,   73,  414,  414,  414,
 /*  2940 */   414,  414,   70,  414,   65,   66,   68,   67,   69,   65,
 /*  2950 */    66,   68,   67,   69,  414,  414,  414,  414,  414,  414,
 /*  2960 */   414,  414,  414,  414,  414,  414,  414,  414,  414,  414,
 /*  2970 */   414,  414,  414,  414,  414,  414,  414,  414,  414,   71,
 /*  2980 */    72,  414,  414,  414,   71,   72,  414,   24,  414,   70,
 /*  2990 */   414,  414,  414,  414,   70,
};
static YYCODETYPE yy_lookahead[] = {
 /*     0 */    23,   24,   25,   26,   27,   28,  113,   34,   31,   31,
 /*    10 */   106,   38,  108,  109,  110,  111,  112,  119,  114,  115,
 /*    20 */    43,   43,   45,  119,   47,   48,   49,   50,   51,   52,
 /*    30 */    53,   54,   55,   56,   57,    7,    8,    9,   10,   11,
 /*    40 */    12,   13,   14,   15,   16,   17,   18,   19,   20,   21,
 /*    50 */    22,   32,  159,  160,  161,   31,   79,   17,   18,   19,
 /*    60 */    20,   21,   22,   32,   87,   88,   31,   90,   91,   92,
 /*    70 */    93,   94,   87,   88,   39,   40,   91,   92,   93,   94,
 /*    80 */    32,  113,   47,   48,   49,   50,   51,   52,   53,   54,
 /*    90 */    55,  143,   57,   58,   59,   60,  105,   62,  104,  151,
 /*   100 */   152,   66,   67,   68,   69,  157,  158,   72,   80,  153,
 /*   110 */   154,   87,   88,   32,   31,   91,   92,   93,   94,   84,
 /*   120 */    85,   86,   39,   40,   89,   41,  120,  121,  122,  161,
 /*   130 */    47,   48,   49,   50,   51,   52,   53,   54,   55,  143,
 /*   140 */    57,   58,   59,   60,  121,   62,   31,  151,  152,   66,
 /*   150 */    67,   68,   69,  157,  158,   72,  143,   73,   74,   75,
 /*   160 */    76,   77,   31,   79,  151,  152,  143,   84,   85,   86,
 /*   170 */    39,   40,   89,   41,  151,  152,  144,  145,   47,   48,
 /*   180 */    49,   50,   51,   52,   53,   54,   55,  143,   57,   58,
 /*   190 */    59,   60,   31,   62,   32,  151,  152,   66,   67,   68,
 /*   200 */    69,  157,  158,   72,  143,   73,   74,   75,   76,   77,
 /*   210 */    31,   79,  151,  152,    6,   84,   85,   86,   39,   40,
 /*   220 */    89,   99,  100,  101,  102,  103,   47,   48,   49,   50,
 /*   230 */    51,   52,   53,   54,   55,  143,   57,   58,   59,   60,
 /*   240 */   148,   62,  112,  151,  152,   66,   67,   68,   69,  119,
 /*   250 */   108,   72,  110,  111,  112,   31,  114,  115,   31,  140,
 /*   260 */    80,  119,  143,   84,   85,   86,   39,   40,   89,   41,
 /*   270 */   151,  152,   78,   79,   47,   48,   49,   50,   51,   52,
 /*   280 */    53,   54,   55,  143,   57,   58,   59,   60,   80,   62,
 /*   290 */    32,  151,  152,   66,   67,   68,   69,  157,  158,   72,
 /*   300 */   143,   73,   74,   75,   76,   77,   31,   38,  151,  152,
 /*   310 */   143,   84,   85,   86,   39,   40,   89,  150,  151,  152,
 /*   320 */    32,   80,   47,   48,   49,   50,   51,   52,   53,   54,
 /*   330 */    55,  143,   57,   58,   59,   60,  148,   62,   32,  151,
 /*   340 */   152,   66,   67,   68,   69,   30,  140,   72,   33,  143,
 /*   350 */    35,   36,   37,  113,   31,   40,   80,  151,  152,   84,
 /*   360 */    85,   86,   39,   40,   89,   20,   21,   22,    6,   32,
 /*   370 */    47,   48,   49,   50,   51,   52,   53,   54,   55,  143,
 /*   380 */    57,   58,   59,   60,  148,   62,   32,  151,  152,   66,
 /*   390 */    67,   68,   69,   39,   32,   72,  156,  143,   32,  159,
 /*   400 */   160,  161,   31,  149,  150,  151,  152,   84,   85,   86,
 /*   410 */    39,   40,   89,  153,  154,    6,  153,  154,   47,   48,
 /*   420 */    49,   50,   51,   52,   53,   54,   55,  143,   57,   58,
 /*   430 */    59,   60,   31,   62,  105,  151,  152,   66,   67,   68,
 /*   440 */    69,   32,  107,   72,  110,   29,  112,  112,  114,  115,
 /*   450 */    31,  116,   32,  119,  119,   84,   85,   86,   39,   40,
 /*   460 */    89,   41,  153,  154,  153,  154,   47,   48,   49,   50,
 /*   470 */    51,   52,   53,   54,   55,  143,   57,   58,   59,   60,
 /*   480 */    38,   62,   31,  151,  152,   66,   67,   68,   69,  112,
 /*   490 */   158,   72,  110,  111,  112,   31,  119,  115,   31,  140,
 /*   500 */   154,  119,  143,   84,   85,   86,   39,   40,   89,   78,
 /*   510 */   151,  152,   81,   41,   47,   48,   49,   50,   51,   52,
 /*   520 */    53,   54,   55,  143,   57,   58,   59,   60,   31,   62,
 /*   530 */   112,  151,  152,   66,   67,   68,   69,  119,  117,   72,
 /*   540 */   143,  120,  121,  122,  147,  113,   31,  140,  151,  152,
 /*   550 */   143,   84,   85,   86,   39,   40,   89,   34,  151,  152,
 /*   560 */    31,   38,   47,   48,   49,   50,   51,   52,   53,   54,
 /*   570 */    55,  143,   57,   58,   59,   60,   45,   62,   35,  151,
 /*   580 */   152,   66,   67,   68,   69,  117,    6,   72,  120,  121,
 /*   590 */   122,   45,  117,  105,   31,  120,  121,  122,  143,   84,
 /*   600 */    85,   86,   39,   40,   89,   38,  151,  152,   32,   29,
 /*   610 */    47,   48,   49,   50,   51,   52,   53,   54,   55,   31,
 /*   620 */    57,   58,   59,   60,   34,   62,  153,  154,   38,   66,
 /*   630 */    67,   68,   69,    6,  117,   72,  143,  120,  121,  122,
 /*   640 */   147,    6,   31,  140,  151,  152,  143,   84,   85,   86,
 /*   650 */    39,   40,   89,   34,  151,  152,   29,   38,   47,   48,
 /*   660 */    49,   50,   51,   52,   53,   54,   55,   31,   57,   58,
 /*   670 */    59,   60,   32,   62,   31,   87,   88,   66,   67,   68,
 /*   680 */    69,   41,  112,   72,  143,  115,  153,  154,  147,  119,
 /*   690 */    31,   31,  151,  152,  143,   84,   85,   86,   39,   40,
 /*   700 */    89,   45,  151,  152,    6,   70,   47,   48,   49,   50,
 /*   710 */    51,   52,   53,   54,   55,   31,   57,   58,   59,   60,
 /*   720 */   155,   62,   31,   87,   88,   66,   67,   68,   69,   32,
 /*   730 */     6,   72,  143,  105,   78,   79,  147,   81,   31,   31,
 /*   740 */   151,  152,  143,   84,   85,   86,   39,   40,   89,   38,
 /*   750 */   151,  152,  146,   29,   47,   48,   49,   50,   51,   52,
 /*   760 */    53,   54,   55,  143,   57,   58,   59,   60,   41,   62,
 /*   770 */    31,  151,  152,   66,   67,   68,   69,   45,  112,   72,
 /*   780 */   143,   80,  116,   61,  147,  119,   31,   31,  151,  152,
 /*   790 */   143,   84,   85,   86,   39,   40,   89,   38,  151,  152,
 /*   800 */   146,   31,   47,   48,   49,   50,   51,   52,   53,   54,
 /*   810 */    55,  143,   57,   58,   59,   60,  104,   62,   70,  151,
 /*   820 */   152,   66,   67,   68,   69,  146,    6,   72,  143,  141,
 /*   830 */   142,  105,  147,    6,   31,   31,  151,  152,  143,   84,
 /*   840 */    85,   86,   39,   40,   89,   31,  151,  152,    6,   29,
 /*   850 */    47,   48,   49,   50,   51,   52,   53,   54,   55,   32,
 /*   860 */    57,   58,   59,   60,   31,   62,  153,  154,   38,   66,
 /*   870 */    67,   68,   69,    6,   32,   72,  143,  146,  153,  154,
 /*   880 */   147,   32,   31,   67,  151,  152,  143,   84,   85,   86,
 /*   890 */    39,   40,   89,   39,  151,  152,   29,  113,   47,   48,
 /*   900 */    49,   50,   51,   52,   53,   54,   55,  143,   57,   58,
 /*   910 */    59,   60,   64,   62,    0,  151,  152,   66,   67,   68,
 /*   920 */    69,  153,  154,   72,  143,   64,   80,  142,  147,   61,
 /*   930 */    31,   38,  151,  152,  143,   84,   85,   86,   39,   40,
 /*   940 */    89,  146,  151,  152,    6,   64,   47,   48,   49,   50,
 /*   950 */    51,   52,   53,   54,   55,  143,   57,   58,   59,   60,
 /*   960 */    64,   62,   64,  151,  152,   66,   67,   68,   69,  113,
 /*   970 */    32,   72,  143,   64,   38,   31,  147,    6,   31,   31,
 /*   980 */   151,  152,  143,   84,   85,   86,   39,   40,   89,  146,
 /*   990 */   151,  152,    6,   31,   47,   48,   49,   50,   51,   52,
 /*  1000 */    53,   54,   55,   32,   57,   58,   59,   60,   45,   62,
 /*  1010 */    45,    6,   38,   66,   67,   68,   69,   41,   32,   72,
 /*  1020 */   143,   31,   31,   80,   31,   38,   31,   31,  151,  152,
 /*  1030 */   143,   84,   85,   86,   39,   40,   89,   32,  151,  152,
 /*  1040 */     6,   12,   47,   48,   49,   50,   51,   52,   53,   54,
 /*  1050 */    55,  143,   57,   58,   59,   60,   38,   62,   80,  151,
 /*  1060 */   152,   66,   67,   68,   69,   80,   32,   72,  143,   31,
 /*  1070 */   146,  105,   41,   43,   31,   38,  151,  152,  143,   84,
 /*  1080 */    85,   86,   39,   40,   89,  113,  151,  152,    6,   32,
 /*  1090 */    47,   48,   49,   50,   51,   52,   53,   54,   55,  143,
 /*  1100 */    57,   58,   59,   60,   43,   62,   79,  151,  152,   66,
 /*  1110 */    67,   68,   69,   41,   32,   72,  143,  113,   32,   31,
 /*  1120 */   145,    6,   31,   31,  151,  152,  143,   84,   85,   86,
 /*  1130 */    39,   40,   89,    6,  151,  152,    6,   41,   47,   48,
 /*  1140 */    49,   50,   51,   52,   53,   54,   55,   32,   57,   58,
 /*  1150 */    59,   60,   38,   62,  113,   32,   29,   66,   67,   68,
 /*  1160 */    69,   45,   32,   72,  143,  113,  155,   67,  113,   32,
 /*  1170 */    31,  146,  151,  152,   80,   84,   85,   86,   39,   40,
 /*  1180 */    89,    1,    2,    3,    4,    5,   47,   48,   49,   50,
 /*  1190 */    51,   52,   53,   54,   55,  146,   57,   58,   59,   60,
 /*  1200 */    31,   62,   45,   35,   31,   66,   67,   68,   69,  162,
 /*  1210 */   113,   72,  162,  162,  162,  162,   36,   37,   31,   39,
 /*  1220 */    40,  162,   42,   84,   85,   86,   46,   40,   89,    1,
 /*  1230 */     2,    3,    4,    5,   47,   48,   49,   50,   51,   52,
 /*  1240 */    53,   54,   55,    6,   57,   58,   59,   60,  162,   62,
 /*  1250 */   162,  162,  162,   66,   67,   68,   69,  162,  162,   72,
 /*  1260 */   162,  162,  103,  162,   36,   37,   29,   39,   40,  162,
 /*  1270 */    42,   84,   85,   86,   46,   39,   89,  118,  162,  162,
 /*  1280 */   162,  162,  123,  124,  125,  126,  127,  128,  129,  130,
 /*  1290 */   131,  132,  133,  134,  135,  136,  137,  138,  139,   63,
 /*  1300 */   103,   65,   96,   97,   98,   99,  100,  101,  102,  103,
 /*  1310 */   151,  152,  162,   39,  162,  118,  162,  162,  162,  162,
 /*  1320 */   123,  124,  125,  126,  127,  128,  129,  130,  131,  132,
 /*  1330 */   133,  134,  135,  136,  137,  138,  139,   63,  103,   65,
 /*  1340 */   109,  110,  111,  112,  162,  114,  115,  162,  151,  152,
 /*  1350 */   119,  162,  162,  118,  162,  162,  162,  162,  123,  124,
 /*  1360 */   125,  126,  127,  128,  129,  130,  131,  132,  133,  134,
 /*  1370 */   135,  136,  137,  138,  139,  103,  162,    1,    2,    3,
 /*  1380 */     4,    5,  162,  162,  162,  162,  151,  152,  162,  162,
 /*  1390 */   118,  162,  162,  162,    6,  123,  124,  125,  126,  127,
 /*  1400 */   128,  129,  130,  131,  132,  133,  134,  135,  136,  137,
 /*  1410 */   138,  139,   36,   37,  103,   39,   40,   29,   42,  162,
 /*  1420 */   162,  162,   46,  151,  152,  162,  162,  162,  162,  118,
 /*  1430 */   162,  162,  162,  162,  123,  124,  125,  126,  127,  128,
 /*  1440 */   129,  130,  131,  132,  133,  134,  135,  136,  137,  138,
 /*  1450 */   139,  162,  103,  162,  162,  162,  162,  162,  162,  162,
 /*  1460 */   162,  162,  151,  152,  162,  162,  162,  118,  162,  162,
 /*  1470 */   162,  162,  123,  124,  125,  126,  127,  128,  129,  130,
 /*  1480 */   131,  132,  133,  134,  135,  136,  137,  138,  139,  162,
 /*  1490 */   103,  162,  162,  162,  162,  162,  162,  162,  162,  162,
 /*  1500 */   151,  152,  162,  162,  162,  118,  162,  162,  162,  162,
 /*  1510 */   123,  124,  125,  126,  127,  128,  129,  130,  131,  132,
 /*  1520 */   133,  134,  135,  136,  137,  138,  139,  103,  162,    1,
 /*  1530 */     2,    3,    4,    5,  162,  162,  162,  162,  151,  152,
 /*  1540 */   162,  162,  118,  162,  162,  162,  162,  123,  124,  125,
 /*  1550 */   126,  127,  128,  129,  130,  131,  132,  133,  134,  135,
 /*  1560 */   136,  137,  138,  139,   36,   37,  103,   39,   40,  162,
 /*  1570 */    42,  162,  162,  162,   46,  151,  152,  162,  162,  162,
 /*  1580 */   162,  118,  162,  162,  162,  162,  123,  124,  125,  126,
 /*  1590 */   127,  128,  129,  130,  131,  132,  133,  134,  135,  136,
 /*  1600 */   137,  138,  139,  162,  103,  162,  162,  162,  162,  162,
 /*  1610 */   162,  162,  162,  162,  151,  152,  162,  162,  162,  118,
 /*  1620 */   162,  162,  162,  162,  123,  124,  125,  126,  127,  128,
 /*  1630 */   129,  130,  131,  132,  133,  134,  135,  136,  137,  138,
 /*  1640 */   139,  162,  103,  162,  162,  162,  162,  162,  162,  162,
 /*  1650 */   162,  162,  151,  152,  162,  162,  162,  118,  162,  162,
 /*  1660 */   162,  162,  123,  124,  125,  126,  127,  128,  129,  130,
 /*  1670 */   131,  132,  133,  134,  135,  136,  137,  138,  139,  103,
 /*  1680 */   162,    1,    2,    3,    4,    5,  162,  162,  162,  162,
 /*  1690 */   151,  152,  162,  162,  118,  162,  162,  162,  162,  123,
 /*  1700 */   124,  125,  126,  127,  128,  129,  130,  131,  132,  133,
 /*  1710 */   134,  135,  136,  137,  138,  139,   36,   37,  103,   39,
 /*  1720 */    40,  162,  162,  162,  162,  162,   46,  151,  152,  162,
 /*  1730 */   162,  162,  162,  118,  162,  162,  162,  162,  123,  124,
 /*  1740 */   125,  126,  127,  128,  129,  130,  131,  132,  133,  134,
 /*  1750 */   135,  136,  137,  138,  139,  162,  103,  162,  162,  162,
 /*  1760 */   162,  162,  162,  162,  162,  162,  151,  152,  162,  162,
 /*  1770 */   162,  118,  162,  162,  162,  162,  123,  124,  125,  126,
 /*  1780 */   127,  128,  129,  130,  131,  132,  133,  134,  135,  136,
 /*  1790 */   137,  138,  139,  162,  103,  162,  162,  162,  162,  162,
 /*  1800 */   162,  162,  162,  162,  151,  152,  162,  162,  162,  118,
 /*  1810 */   162,  162,  162,  162,  123,  124,  125,  126,  127,  128,
 /*  1820 */   129,  130,  131,  132,  133,  134,  135,  136,  137,  138,
 /*  1830 */   139,  103,  162,    1,    2,    3,    4,    5,  162,  162,
 /*  1840 */   162,  162,  151,  152,  162,  162,  118,  162,  162,  162,
 /*  1850 */   162,  123,  124,  125,  126,  127,  128,  129,  130,  131,
 /*  1860 */   132,  133,  134,  135,  136,  137,  138,  139,   36,   37,
 /*  1870 */   103,   39,   40,  162,  162,  162,  162,  162,   46,  151,
 /*  1880 */   152,  162,  162,  162,  162,  162,  162,  162,  162,  162,
 /*  1890 */   123,  124,  125,  126,  127,  128,  129,  130,  131,  132,
 /*  1900 */   133,  134,  135,  136,  137,  138,  139,  162,  162,  162,
 /*  1910 */   162,  162,  162,  162,  162,  162,  162,  162,  151,  152,
 /*  1920 */    23,   24,   25,   26,   27,   28,   29,  162,   31,  162,
 /*  1930 */    23,   24,   25,   26,   27,   28,   29,  162,   31,  162,
 /*  1940 */    43,  162,   45,  162,  162,  162,  162,  162,  162,  162,
 /*  1950 */    43,  162,   45,  162,  162,   23,   24,   25,   26,   27,
 /*  1960 */    28,  162,  162,   31,  162,  162,  162,  162,  162,  162,
 /*  1970 */   162,  162,   41,  162,  162,   43,   79,   45,  162,  162,
 /*  1980 */   162,  162,   31,  162,   87,   88,   79,   90,   91,   92,
 /*  1990 */    93,   94,  162,  162,   87,   88,  162,   90,   91,   92,
 /*  2000 */    93,   94,  162,   71,   73,   74,   75,   76,   77,   78,
 /*  2010 */    79,   79,   81,   82,   83,  162,  162,   31,  162,   87,
 /*  2020 */    88,  162,   90,   91,   92,   93,   94,   23,   24,   25,
 /*  2030 */    26,   27,   28,   29,  162,   31,  162,  162,   87,   88,
 /*  2040 */   162,  162,   91,   92,   93,   94,  162,   43,  162,   45,
 /*  2050 */    23,   24,   25,   26,   27,   28,  162,  162,   31,  162,
 /*  2060 */   162,  162,  162,  162,  162,  162,   80,  162,  162,  162,
 /*  2070 */    43,  162,   45,   87,   88,  162,  162,   91,   92,   93,
 /*  2080 */    94,  162,  162,   79,  162,  162,  162,  162,  162,  162,
 /*  2090 */   162,   87,   88,  162,   90,   91,   92,   93,   94,  162,
 /*  2100 */   162,  162,  162,  162,  162,  162,   79,   80,  162,  162,
 /*  2110 */   162,  162,  162,  162,   87,   88,  162,   90,   91,   92,
 /*  2120 */    93,   94,   23,   24,   25,   26,   27,   28,  162,  162,
 /*  2130 */    31,  162,  162,  162,  162,  162,  162,  162,  162,  162,
 /*  2140 */   162,  162,   43,  162,   45,   23,   24,   25,   26,   27,
 /*  2150 */    28,  162,  162,   31,  162,  162,  162,  162,  162,  162,
 /*  2160 */   162,  162,  162,  162,  162,   43,  162,   45,  162,  162,
 /*  2170 */   162,  162,  162,  162,  162,  162,  162,  162,   79,   80,
 /*  2180 */   162,  162,  162,  162,  162,  162,   87,   88,  162,   90,
 /*  2190 */    91,   92,   93,   94,  162,  162,  162,  162,  162,  162,
 /*  2200 */   162,   79,   80,   23,   24,   25,   26,   27,   28,   87,
 /*  2210 */    88,   31,   90,   91,   92,   93,   94,    0,  162,  162,
 /*  2220 */   162,  162,  162,   43,  162,   45,   23,   24,   25,   26,
 /*  2230 */    27,   28,  162,  162,   31,   32,   23,   24,   25,   26,
 /*  2240 */    27,   28,   29,  162,   31,  162,   43,   30,   45,  162,
 /*  2250 */    33,   71,   35,   36,   37,  162,   43,   40,   45,   79,
 /*  2260 */   162,   23,   24,   25,   26,   27,   28,   87,   88,   31,
 /*  2270 */    90,   91,   92,   93,   94,    1,    2,    3,    4,    5,
 /*  2280 */   162,   43,   79,   45,  162,  162,  162,  162,  162,  162,
 /*  2290 */    87,   88,   79,   90,   91,   92,   93,   94,  162,  162,
 /*  2300 */    87,   88,  162,   90,   91,   92,   93,   94,  162,  162,
 /*  2310 */    36,   37,  162,   39,   40,  162,  162,   79,  162,  162,
 /*  2320 */    46,  162,  162,  162,  162,   87,   88,  162,   90,   91,
 /*  2330 */    92,   93,   94,    7,    8,    9,   10,   11,   12,   13,
 /*  2340 */    14,   15,   16,   17,   18,   19,   20,   21,   22,  162,
 /*  2350 */   162,  162,  162,  162,  162,  162,  162,  162,   32,  162,
 /*  2360 */   162,  162,  162,  162,  162,  162,    7,    8,    9,   10,
 /*  2370 */    11,   12,   13,   14,   15,   16,   17,   18,   19,   20,
 /*  2380 */    21,   22,  162,  162,  162,  162,  162,  162,   29,    7,
 /*  2390 */     8,    9,   10,   11,   12,   13,   14,   15,   16,   17,
 /*  2400 */    18,   19,   20,   21,   22,  162,  162,  162,  162,  162,
 /*  2410 */   162,  162,  162,  162,   32,  162,  162,  162,  162,  162,
 /*  2420 */   162,  162,    7,    8,    9,   10,   11,   12,   13,   14,
 /*  2430 */    15,   16,   17,   18,   19,   20,   21,   22,    9,   10,
 /*  2440 */    11,   12,   13,   14,   15,   16,   17,   18,   19,   20,
 /*  2450 */    21,   22,  162,   38,  162,  162,  162,  162,  162,  162,
 /*  2460 */   162,    7,    8,    9,   10,   11,   12,   13,   14,   15,
 /*  2470 */    16,   17,   18,   19,   20,   21,   22,  162,  162,  162,
 /*  2480 */   162,  162,  162,  162,  162,  162,  162,  162,  162,  162,
 /*  2490 */   162,  162,   38,  162,  162,  162,  162,  162,  162,  162,
 /*  2500 */     7,    8,    9,   10,   11,   12,   13,   14,   15,   16,
 /*  2510 */    17,   18,   19,   20,   21,   22,  162,  162,  162,  162,
 /*  2520 */   162,  162,  162,  162,  162,  162,  162,  162,  162,  162,
 /*  2530 */   162,   38,  162,  162,  162,  162,  162,  162,  162,    7,
 /*  2540 */     8,    9,   10,   11,   12,   13,   14,   15,   16,   17,
 /*  2550 */    18,   19,   20,   21,   22,  162,  162,  162,  162,  162,
 /*  2560 */   162,  162,  162,  162,  162,  162,  162,  162,  162,  162,
 /*  2570 */    38,  162,  162,  162,  162,  162,  162,  162,    7,    8,
 /*  2580 */     9,   10,   11,   12,   13,   14,   15,   16,   17,   18,
 /*  2590 */    19,   20,   21,   22,    7,    8,    9,   10,   11,   12,
 /*  2600 */    13,   14,   15,   16,   17,   18,   19,   20,   21,   22,
 /*  2610 */   162,  162,  162,  162,  162,  162,   11,  162,  162,  162,
 /*  2620 */   162,  162,  162,  162,  162,  162,  162,  162,   11,  162,
 /*  2630 */   162,  162,  162,  162,   29,  162,   31,  162,  162,  162,
 /*  2640 */   162,  162,  162,  162,  162,  162,   29,  162,   31,  162,
 /*  2650 */   162,   80,   47,   48,   49,   50,   51,   52,   53,   54,
 /*  2660 */    55,   56,   57,   11,   47,   48,   49,   50,   51,   52,
 /*  2670 */    53,   54,   55,   56,   57,   11,  162,  162,  162,  162,
 /*  2680 */   162,   29,  162,   31,  162,  162,  162,  162,  162,  162,
 /*  2690 */   162,  162,  162,   29,  162,   31,  162,  162,  162,   47,
 /*  2700 */    48,   49,   50,   51,   52,   53,   54,   55,   56,   57,
 /*  2710 */   162,   47,   48,   49,   50,   51,   52,   53,   54,   55,
 /*  2720 */    56,   57,  162,   11,  162,  162,  162,  162,  162,  162,
 /*  2730 */   162,  162,  162,  162,  162,  162,  162,  162,  162,  162,
 /*  2740 */   162,  162,  162,   31,  162,    1,    2,    3,    4,    5,
 /*  2750 */     1,    2,    3,    4,    5,  162,  162,  162,  162,   47,
 /*  2760 */    48,   49,   50,   51,   52,   53,   54,   55,   56,   57,
 /*  2770 */     1,    2,    3,    4,    5,    1,    2,    3,    4,    5,
 /*  2780 */    36,   37,  162,   39,   40,   36,   37,  162,   39,   40,
 /*  2790 */    46,  162,  162,  162,  162,   46,  162,    1,    2,    3,
 /*  2800 */     4,    5,  162,  162,  162,   36,   37,  162,   39,   40,
 /*  2810 */    36,   37,  162,   39,   40,   46,  162,  162,  162,  162,
 /*  2820 */    46,  162,    1,    2,    3,    4,    5,   31,  162,  162,
 /*  2830 */   162,  162,   36,   37,    1,    2,    3,    4,    5,  162,
 /*  2840 */    44,  162,   46,  162,  162,  162,  162,  162,  162,  162,
 /*  2850 */   162,  162,   31,  162,  162,  162,  162,   36,   37,    1,
 /*  2860 */     2,    3,    4,    5,  162,   44,  162,   46,  162,   36,
 /*  2870 */    37,  162,  162,  162,  162,   42,  162,  162,  162,   46,
 /*  2880 */   162,    1,    2,    3,    4,    5,    1,    2,    3,    4,
 /*  2890 */     5,  162,  162,  162,   36,   37,  162,    1,    2,    3,
 /*  2900 */     4,    5,   44,  162,   46,  162,  162,  162,  162,  162,
 /*  2910 */   162,  162,  162,  162,  162,  162,   36,   37,  162,  162,
 /*  2920 */   162,   36,   37,  162,   44,  162,   46,  162,  162,   44,
 /*  2930 */   162,   46,   36,   37,  162,  162,   40,  162,  162,  162,
 /*  2940 */   162,  162,   46,  162,    1,    2,    3,    4,    5,    1,
 /*  2950 */     2,    3,    4,    5,  162,  162,  162,  162,  162,  162,
 /*  2960 */   162,  162,  162,  162,  162,  162,  162,  162,  162,  162,
 /*  2970 */   162,  162,  162,  162,  162,  162,  162,  162,  162,   36,
 /*  2980 */    37,  162,  162,  162,   36,   37,  162,   44,  162,   46,
 /*  2990 */   162,  162,  162,  162,   46,
};
#define YY_SHIFT_USE_DFLT (-28)
static short yy_shift_ofst[] = {
 /*     0 */   315,  914, 2217,  -28,  -28,  -28,  -28,  -28,  990,  366,
 /*    10 */   -28,  993,  590,  -28,  401,  567,  -28, 2774,  -28,  354,
 /*    20 */   -28, 2896,  -28, 2943,  464,  531, 2605,  -28, 1388,  -28,
 /*    30 */  2712,  -28,  976,  -15,  -28,  -28,  -28,  -28,  -28,  -28,
 /*    40 */   -28,  691,  727,  -15,  -28,  996, 1096,  -15,  -28, 1038,
 /*    50 */  1029,  991,  -28,  -28,  -28,  -28,  -28,  -28,  -28,  -28,
 /*    60 */   -28,  -28,  -28,  -28,  -28,  -28,  -28,  -28,  -28,  -28,
 /*    70 */   -28,  -28,  -28, 2948, 2858, 1169, 1157, 2664,   48,  -28,
 /*    80 */   867,  288,  -28,  -28,  -28,  497,  -27,  -28,  660,  269,
 /*    90 */   -28, 1376,  -28,  854,  -28, 1180, 1680,  -28, 2880, 1088,
 /*   100 */  1116, 2617,  442, 1043,  -28, 1139,  -28,  -28,  -28,  -28,
 /*   110 */   -28,  -28,  -28,  -28,  -28,  -28,  -28,  -28,  -28,  -28,
 /*   120 */   -28,  -28,  -28,  -28,  -28,  576,  -28,  697,  -28, 2238,
 /*   130 */   711,  275,  419,  722,  759,  467,  131,  -28, 2238,  830,
 /*   140 */  1274,  -28, 1236,  -28,  -28,  -15,  909, 1187, 1187,  987,
 /*   150 */   563, 1091,  -28, 2238, 1037,  995,  947,  -28, 1114,  707,
 /*   160 */   851, 1100, 2238,   81,  -28, 2238,  -28, 2238,  684,  -28,
 /*   170 */  1027,  636,  985,  -28,  978,  -28,  943,  -28,   40, 2238,
 /*   180 */    40, 2238,   40, 2238,   40, 2238,   40, 2238,   40, 2238,
 /*   190 */    40, 2238,   40, 2238,  345, 2238,  345, 2238,  -28, 2238,
 /*   200 */   -28, 2238,  -28, 2238,  345, 2238, 2429, 2238, 2429,  -23,
 /*   210 */  2359,  -28,  416, 2238,  -28,  656,  529,  546, 2213, 2587,
 /*   220 */  2238,  -28,  643,  698,  684,  -28, 2238,  -28,  -28,  -28,
 /*   230 */   -28,  -28,  -28,  -28,  -28,  -28, 1986,  -28,  208,  -28,
 /*   240 */    24,  -28,  848, 1951,  -28,  -28,  -28,  -28,  881,  896,
 /*   250 */   898,  -28,  944,  963, 2004,  -28,  580,  -28, 2238,  -28,
 /*   260 */   -28,  -28,  -28, 1127,  -28,  -28,  -22,  732, 1907,  820,
 /*   270 */   -28,  -28,  -28, 2238, 2571,  -28, 1897,  724,  -28,  -28,
 /*   280 */  2587,  814,  635, 1932, 2532, 1187,  179,  -28, 1092,  842,
 /*   290 */   -28, 1092,  -28, 1931,  -28,  -28,  -28,  -28,  -28,  -28,
 /*   300 */  2238,  -28, 2587,  948,  132, 2238,  -28, 2027,  228, 2238,
 /*   310 */   -28,  846,  228, 2238,  -28,   28,  804,   84, 2238,  -28,
 /*   320 */  2122,  228, 2238,  -28,  701,  228, 2238,  -28, 2099,  228,
 /*   330 */  2238,  -28, 1094,  228, 2238,  -28,  -28,  -28,  -28, 2238,
 /*   340 */  1130,  -28, 2238,  -28, 2587,  -28,   19,  -28,   31,  -28,
 /*   350 */  2203, 2326,  -28,  -28,  115,  194,  161,  162,  -28,  588,
 /*   360 */   180,  258,  -28,  241,  306,  -28,  276,  337,  -28, 2238,
 /*   370 */  2382,  -28,  451,  362,  -28,  451,  -28,  472,  -15,  -28,
 /*   380 */   -28,  451,  971,  -28,  451,  986,  -28,  451,  827,  -28,
 /*   390 */   451, 1034,  -28,  451, 1115,  -28,  451,  938,  -28,  451,
 /*   400 */  1082,  -28,  451, 1005,  -28,  451,  409,  -28,  431,  739,
 /*   410 */   732,  756,  546,  -28,  -28, 2238, 2415, 1187,  755,  -28,
 /*   420 */   770,  748, 2180, 2454, 1187,  899,  -28, 2238, 2493, 1187,
 /*   430 */   323,  -28,  816, 2238,  849,  -28,  -28,  -28,  861, 1187,
 /*   440 */  1187,  -28,  -28,  868,  893,  659,   83,  -28,  -28, 1237,
 /*   450 */   936,  803,  -28,  227,  -28, 2948, 2885,  962,  965, 2652,
 /*   460 */   974,   35,  -28,  611,  -28,  627, 1018,  515,  -28,  371,
 /*   470 */   -28, 1228, 1832,  -28,  -28, 2833, 1030, 1031,  -15, 1057,
 /*   480 */   -28, 1061, 1072,  -15, 1086,  -28,  -28, 2833, 2821,  420,
 /*   490 */   -28,  -15, 1123,  -28, 2796,  640,  -28,  -15, 1137,  -28,
 /*   500 */   -28, 1528, 2769, 2749, 2948, 2744,  -28, 2274, 1168, 1173,
 /*   510 */   619,  -28,  224,  269,  -28,  543,  708,  523,  -28,  833,
 /*   520 */   269,  -28,  -28,
};
#define YY_REDUCE_USE_DFLT (-108)
static short yy_reduce_ofst[] = {
 /*     0 */  1206, -108,  122, -108, -108, -108, -108, -108, -108, -108,
 /*    10 */  -108, -108,   -6, -108, -108,  712, -108,  335, -108, -108,
 /*    20 */  -108,  666, -108, -102, -108, -108,  468, -108, -108, -108,
 /*    30 */     6, -108, -108, 1097, -108, -108, -108, -108, -108, -108,
 /*    40 */  -108, -108, -108,  784, -108, -108, -108, 1052, -108, -108,
 /*    50 */  -108, -108, -108, -108, -108, -108, -108, -108, -108, -108,
 /*    60 */  -108, -108, -108, -108, -108, -108, -108, -108, -108, -108,
 /*    70 */  -108, -108, -108,  377, -102, -108, -108,  517, -108, -108,
 /*    80 */  -108, -108, -108, -108, -108, -108,  488, -108, -108,  628,
 /*    90 */  -108,  -96, -108, -108, -108, 1231,  570, -108, -102, -108,
 /*   100 */  -108,  421, -108, 1272, -108, 1767, -108, -108, -108, -108,
 /*   110 */  -108, -108, -108, -108, -108, -108, -108, -108, -108, -108,
 /*   120 */  -108, -108, -108, -108, -108, -108, -108, -108, -108,  407,
 /*   130 */  -108, 1311, 1767, -108, -108, 1235, 1767, -108,  206, -108,
 /*   140 */   688, -108,  785, -108, -108,  856, -108, 1159, 1767, -108,
 /*   150 */  1387, 1767, -108,  503, -108, 1349, 1767, -108, -108, 1197,
 /*   160 */  1767, -108,  119, -108, -108,  956, -108,  157, 1011, -108,
 /*   170 */  -108, -108, -108, -108, -108, -108, -108, -108, -108,  764,
 /*   180 */  -108,  877, -108, 1021, -108,  983, -108,  455, -108,  620,
 /*   190 */  -108,  668, -108,  695, -108,  743, -108,  887, -108,  791,
 /*   200 */  -108,   13, -108,  839, -108,  428, -108,  908, -108,   23,
 /*   210 */  -108, -108, -108,  925, -108, -108, -108, -108,  140, -108,
 /*   220 */   935, -108, -108, -108,  565, -108,  380, -108, -108, -108,
 /*   230 */  -108, -108, -108, -108, -108, -108,  240, -108, -108, -108,
 /*   240 */  -107, -108, -108,  -32, -108, -108, -108, -108, -108, -108,
 /*   250 */  -108, -108, -108, -108,  -52, -108, -108, -108,  332, -108,
 /*   260 */  -108, -108, -108, -108, -108, -108, -108, -108,   -4, -108,
 /*   270 */  -108, -108, -108,   61, -108, -108,   44, -108, -108, -108,
 /*   280 */  -108, -108, -108,  284, -108, 1539, 1767, -108,   32, -108,
 /*   290 */  -108,  975, -108,  924, -108, -108, -108, -108, -108, -108,
 /*   300 */   397, -108, -108, -108,  843,  781, -108,  236,  795,  733,
 /*   310 */  -108, -108,  731,  829, -108, -108, -108,  679,  541, -108,
 /*   320 */    92,  654,  493, -108, -108,  606,  685, -108,  188, 1025,
 /*   330 */   589, -108, -108, 1049,  637, -108, -108, -108, -108,  254,
 /*   340 */  -108, -108,  167, -108, -108, -108, -108, -108, -108, -108,
 /*   350 */   647, -108, -108, -108, -108, -108, -108, -108, -108, -108,
 /*   360 */  -108, -108, -108, -108, -108, -108, -108, -108, -108,  551,
 /*   370 */  -108, -108,  473, -108, -108,  346, -108, -108,  432, -108,
 /*   380 */  -108,  725, -108, -108,  713, -108, -108,  309, -108, -108,
 /*   390 */   533, -108, -108,  263, -108, -108,  260, -108, -108,  311,
 /*   400 */  -108, -108,  -44, -108, -108,  768, -108, -108, -108, -108,
 /*   410 */  -108, -108, -108, -108, -108,  973, -108, 1728, 1767, -108,
 /*   420 */  -108, -108,  599, -108, 1691, 1767, -108,  812, -108, 1653,
 /*   430 */  1767, -108, -108,  359, -108, -108, -108, -108, -108, 1615,
 /*   440 */  1767, -108, -108, -108, -108, 1576, 1767, -108, -108, -108,
 /*   450 */  -108, 1501, -108, 1767, -108,  418, -102, -108, -108,  475,
 /*   460 */  -108, 1463, -108, 1767, -108, -108, -108, 1424, -108, 1767,
 /*   470 */  -108,  334,  570, -108, -108,  418, -108, -108,  972, -108,
 /*   480 */  -108, -108, -108, 1004, -108, -108, -108,  130, -102, -108,
 /*   490 */  -108, 1041, -108, -108, -102, -108, -108, 1055, -108, -108,
 /*   500 */  -108,  142,  382,  570,  130,  570, -108,  570, -108, -108,
 /*   510 */    -9, -108, -108,  329, -108, -108, -108,  726, -108, -108,
 /*   520 */   966, -108, -108,
};
static YYACTIONTYPE yy_default[] = {
 /*     0 */   773,  773,  773,  525,  527,  528,  529,  530,  773,  773,
 /*    10 */   531,  773,  773,  532,  773,  773,  533,  773,  542,  773,
 /*    20 */   543,  773,  563,  773,  773,  773,  773,  575,  773,  576,
 /*    30 */   773,  589,  591,  773,  594,  762,  763,  764,  765,  766,
 /*    40 */   767,  773,  592,  773,  595,  773,  593,  773,  596,  773,
 /*    50 */   773,  773,  598,  599,  600,  601,  602,  603,  604,  605,
 /*    60 */   606,  607,  608,  590,  579,  581,  582,  583,  584,  585,
 /*    70 */   586,  587,  588,  773,  773,  773,  773,  773,  773,  577,
 /*    80 */   773,  773,  578,  580,  564,  773,  773,  534,  773,  773,
 /*    90 */   535,  773,  540,  773,  541,  773,  773,  561,  773,  773,
 /*   100 */   773,  773,  773,  773,  567,  773,  569,  609,  611,  612,
 /*   110 */   613,  614,  615,  616,  617,  618,  619,  620,  621,  622,
 /*   120 */   623,  624,  625,  626,  627,  773,  628,  773,  629,  773,
 /*   130 */   773,  773,  773,  632,  773,  773,  773,  633,  773,  773,
 /*   140 */   773,  636,  773,  637,  638,  773,  773,  773,  640,  773,
 /*   150 */   773,  773,  643,  773,  773,  773,  773,  645,  773,  773,
 /*   160 */   773,  773,  773,  773,  647,  773,  701,  773,  773,  702,
 /*   170 */   773,  773,  773,  768,  773,  769,  773,  770,  703,  773,
 /*   180 */   704,  773,  705,  773,  706,  773,  707,  773,  708,  773,
 /*   190 */   709,  773,  710,  773,  717,  773,  718,  773,  719,  773,
 /*   200 */   720,  773,  721,  773,  722,  773,  723,  773,  724,  773,
 /*   210 */   773,  711,  773,  773,  712,  729,  773,  713,  773,  752,
 /*   220 */   773,  725,  773,  773,  773,  726,  773,  727,  728,  730,
 /*   230 */   731,  732,  733,  734,  735,  736,  773,  737,  773,  738,
 /*   240 */   773,  753,  773,  773,  755,  760,  761,  756,  761,  763,
 /*   250 */   762,  754,  773,  739,  773,  740,  773,  741,  773,  744,
 /*   260 */   745,  750,  751,  773,  748,  749,  773,  714,  773,  773,
 /*   270 */   746,  747,  715,  773,  773,  716,  773,  773,  742,  743,
 /*   280 */   771,  773,  773,  773,  773,  773,  773,  648,  773,  773,
 /*   290 */   652,  773,  653,  773,  655,  656,  657,  658,  659,  660,
 /*   300 */   773,  661,  700,  773,  773,  773,  662,  773,  773,  773,
 /*   310 */   663,  773,  773,  773,  664,  773,  773,  773,  773,  665,
 /*   320 */   773,  773,  773,  666,  773,  773,  773,  667,  773,  773,
 /*   330 */   773,  668,  773,  773,  773,  669,  670,  671,  654,  773,
 /*   340 */   773,  673,  773,  674,  676,  675,  773,  677,  773,  678,
 /*   350 */   773,  773,  679,  680,  773,  773,  773,  773,  681,  773,
 /*   360 */   773,  773,  682,  773,  773,  683,  773,  773,  684,  773,
 /*   370 */   773,  685,  773,  773,  686,  773,  696,  698,  773,  699,
 /*   380 */   697,  773,  773,  687,  773,  773,  688,  773,  773,  689,
 /*   390 */   773,  773,  690,  773,  773,  691,  773,  773,  692,  773,
 /*   400 */   773,  693,  773,  773,  694,  773,  773,  695,  773,  773,
 /*   410 */   773,  773,  773,  772,  610,  773,  773,  773,  773,  649,
 /*   420 */   773,  773,  773,  773,  773,  773,  650,  773,  773,  773,
 /*   430 */   773,  651,  773,  773,  773,  646,  644,  642,  773,  773,
 /*   440 */   641,  639,  634,  630,  773,  773,  773,  635,  631,  773,
 /*   450 */   773,  773,  568,  773,  570,  773,  773,  773,  773,  773,
 /*   460 */   773,  773,  571,  773,  573,  773,  773,  773,  572,  773,
 /*   470 */   574,  773,  773,  559,  562,  773,  773,  773,  773,  773,
 /*   480 */   565,  773,  773,  773,  773,  566,  553,  773,  773,  773,
 /*   490 */   555,  773,  773,  557,  773,  773,  556,  773,  773,  558,
 /*   500 */   560,  773,  773,  773,  773,  773,  554,  773,  773,  773,
 /*   510 */   773,  536,  773,  773,  537,  773,  773,  773,  538,  773,
 /*   520 */   773,  539,  526,
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
  "PRIVATE",       "SCOPED",        "COMMA",         "AND",         
  "OR",            "EQUALS",        "IDENTICAL",     "LESS",        
  "GREATER",       "LESSEQUAL",     "GREATEREQUAL",  "NOTIDENTICAL",
  "NOTEQUALS",     "ADD",           "SUB",           "CONCAT",      
  "MUL",           "DIV",           "MOD",           "ISSET",       
  "FETCH",         "TYPEOF",        "INSTANCEOF",    "NEW",         
  "NOT",           "PARENTHESES_CLOSE",  "NAMESPACE",     "IDENTIFIER",  
  "DOTCOMMA",      "INTERFACE",     "EXTENDS",       "CLASS",       
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
  "DIVASSIGN",     "CONCATASSIGN",  "ARROW",         "SBRACKET_OPEN",
  "SBRACKET_CLOSE",  "DOUBLECOLON",   "INCR",          "DECR",        
  "ECHO",          "RETURN",        "UNSET",         "INTEGER",     
  "STRING",        "THROW",         "CHAR",          "DOUBLE",      
  "NULL",          "TRUE",          "FALSE",         "error",       
  "program",       "xx_language",   "xx_top_statement_list",  "xx_top_statement",
  "xx_namespace_def",  "xx_class_def",  "xx_interface_def",  "xx_comment",  
  "xx_interface_body",  "xx_class_body",  "xx_class_definition",  "xx_interface_methods_definition",
  "xx_class_properties_definition",  "xx_class_consts_definition",  "xx_class_methods_definition",  "xx_class_property_definition",
  "xx_visibility_list",  "xx_literal_expr",  "xx_class_const_definition",  "xx_class_method_definition",
  "xx_interface_method_definition",  "xx_parameter_list",  "xx_statement_list",  "xx_visibility",
  "xx_parameter",  "xx_parameter_type",  "xx_parameter_cast",  "xx_statement",
  "xx_let_statement",  "xx_if_statement",  "xx_loop_statement",  "xx_echo_statement",
  "xx_return_statement",  "xx_mcall_statement",  "xx_scall_statement",  "xx_unset_statement",
  "xx_throw_statement",  "xx_declare_statement",  "xx_break_statement",  "xx_continue_statement",
  "xx_while_statement",  "xx_do_while_statement",  "xx_switch_statement",  "xx_for_statement",
  "xx_eval_expr",  "xx_case_clauses",  "xx_case_clause",  "xx_common_expr",
  "xx_let_assignments",  "xx_let_assignment",  "xx_assignment_operator",  "xx_assign_expr",
  "xx_index_expr",  "xx_echo_expressions",  "xx_echo_expression",  "xx_mcall_expr",
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
 /*  13 */ "xx_class_def ::= ABSTRACT CLASS IDENTIFIER xx_class_body",
 /*  14 */ "xx_class_def ::= ABSTRACT CLASS IDENTIFIER EXTENDS IDENTIFIER xx_class_body",
 /*  15 */ "xx_class_def ::= FINAL CLASS IDENTIFIER xx_class_body",
 /*  16 */ "xx_class_def ::= FINAL CLASS IDENTIFIER EXTENDS IDENTIFIER xx_class_body",
 /*  17 */ "xx_class_body ::= BRACKET_OPEN BRACKET_CLOSE",
 /*  18 */ "xx_class_body ::= BRACKET_OPEN xx_class_definition BRACKET_CLOSE",
 /*  19 */ "xx_interface_body ::= BRACKET_OPEN BRACKET_CLOSE",
 /*  20 */ "xx_interface_body ::= BRACKET_OPEN xx_interface_methods_definition BRACKET_CLOSE",
 /*  21 */ "xx_class_definition ::= xx_class_properties_definition",
 /*  22 */ "xx_class_definition ::= xx_class_consts_definition",
 /*  23 */ "xx_class_definition ::= xx_class_methods_definition",
 /*  24 */ "xx_class_definition ::= xx_class_properties_definition xx_class_methods_definition",
 /*  25 */ "xx_class_definition ::= xx_class_properties_definition xx_class_consts_definition",
 /*  26 */ "xx_class_definition ::= xx_class_consts_definition xx_class_properties_definition",
 /*  27 */ "xx_class_definition ::= xx_class_consts_definition xx_class_methods_definition",
 /*  28 */ "xx_class_definition ::= xx_class_properties_definition xx_class_consts_definition xx_class_methods_definition",
 /*  29 */ "xx_class_definition ::= xx_class_consts_definition xx_class_properties_definition xx_class_methods_definition",
 /*  30 */ "xx_class_properties_definition ::= xx_class_properties_definition xx_class_property_definition",
 /*  31 */ "xx_class_properties_definition ::= xx_class_property_definition",
 /*  32 */ "xx_class_property_definition ::= COMMENT xx_visibility_list IDENTIFIER DOTCOMMA",
 /*  33 */ "xx_class_property_definition ::= xx_visibility_list IDENTIFIER DOTCOMMA",
 /*  34 */ "xx_class_property_definition ::= COMMENT xx_visibility_list IDENTIFIER ASSIGN xx_literal_expr DOTCOMMA",
 /*  35 */ "xx_class_property_definition ::= xx_visibility_list IDENTIFIER ASSIGN xx_literal_expr DOTCOMMA",
 /*  36 */ "xx_class_consts_definition ::= xx_class_consts_definition xx_class_const_definition",
 /*  37 */ "xx_class_consts_definition ::= xx_class_const_definition",
 /*  38 */ "xx_class_methods_definition ::= xx_class_methods_definition xx_class_method_definition",
 /*  39 */ "xx_class_methods_definition ::= xx_class_method_definition",
 /*  40 */ "xx_interface_methods_definition ::= xx_interface_methods_definition DOTCOMMA xx_interface_method_definition",
 /*  41 */ "xx_interface_methods_definition ::= xx_interface_method_definition",
 /*  42 */ "xx_class_const_definition ::= COMMENT CONST CONSTANT ASSIGN xx_literal_expr DOTCOMMA",
 /*  43 */ "xx_class_const_definition ::= CONST CONSTANT ASSIGN xx_literal_expr DOTCOMMA",
 /*  44 */ "xx_class_method_definition ::= xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE BRACKET_OPEN BRACKET_CLOSE",
 /*  45 */ "xx_class_method_definition ::= xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE BRACKET_OPEN BRACKET_CLOSE",
 /*  46 */ "xx_class_method_definition ::= xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  47 */ "xx_class_method_definition ::= xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  48 */ "xx_class_method_definition ::= COMMENT xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE BRACKET_OPEN BRACKET_CLOSE",
 /*  49 */ "xx_class_method_definition ::= COMMENT xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE BRACKET_OPEN BRACKET_CLOSE",
 /*  50 */ "xx_class_method_definition ::= COMMENT xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  51 */ "xx_class_method_definition ::= COMMENT xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  52 */ "xx_interface_method_definition ::= xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /*  53 */ "xx_interface_method_definition ::= xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE",
 /*  54 */ "xx_interface_method_definition ::= COMMENT xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE DOTCOMMA",
 /*  55 */ "xx_interface_method_definition ::= COMMENT xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE DOTCOMMA",
 /*  56 */ "xx_visibility_list ::= xx_visibility_list xx_visibility",
 /*  57 */ "xx_visibility_list ::= xx_visibility",
 /*  58 */ "xx_visibility ::= PUBLIC",
 /*  59 */ "xx_visibility ::= PROTECTED",
 /*  60 */ "xx_visibility ::= PRIVATE",
 /*  61 */ "xx_visibility ::= STATIC",
 /*  62 */ "xx_visibility ::= SCOPED",
 /*  63 */ "xx_visibility ::= INLINE",
 /*  64 */ "xx_visibility ::= ABSTRACT",
 /*  65 */ "xx_visibility ::= FINAL",
 /*  66 */ "xx_parameter_list ::= xx_parameter_list COMMA xx_parameter",
 /*  67 */ "xx_parameter_list ::= xx_parameter",
 /*  68 */ "xx_parameter ::= IDENTIFIER",
 /*  69 */ "xx_parameter ::= xx_parameter_type IDENTIFIER",
 /*  70 */ "xx_parameter ::= xx_parameter_cast IDENTIFIER",
 /*  71 */ "xx_parameter ::= IDENTIFIER ASSIGN xx_literal_expr",
 /*  72 */ "xx_parameter ::= xx_parameter_type IDENTIFIER ASSIGN xx_literal_expr",
 /*  73 */ "xx_parameter ::= xx_parameter_cast IDENTIFIER ASSIGN xx_literal_expr",
 /*  74 */ "xx_parameter_cast ::= LESS IDENTIFIER GREATER",
 /*  75 */ "xx_parameter_type ::= TYPE_INTEGER",
 /*  76 */ "xx_parameter_type ::= TYPE_UINTEGER",
 /*  77 */ "xx_parameter_type ::= TYPE_LONG",
 /*  78 */ "xx_parameter_type ::= TYPE_ULONG",
 /*  79 */ "xx_parameter_type ::= TYPE_CHAR",
 /*  80 */ "xx_parameter_type ::= TYPE_UCHAR",
 /*  81 */ "xx_parameter_type ::= TYPE_DOUBLE",
 /*  82 */ "xx_parameter_type ::= TYPE_BOOL",
 /*  83 */ "xx_parameter_type ::= TYPE_STRING",
 /*  84 */ "xx_parameter_type ::= TYPE_ARRAY",
 /*  85 */ "xx_parameter_type ::= TYPE_VAR",
 /*  86 */ "xx_statement_list ::= xx_statement_list xx_statement",
 /*  87 */ "xx_statement_list ::= xx_statement",
 /*  88 */ "xx_statement ::= xx_let_statement",
 /*  89 */ "xx_statement ::= xx_if_statement",
 /*  90 */ "xx_statement ::= xx_loop_statement",
 /*  91 */ "xx_statement ::= xx_echo_statement",
 /*  92 */ "xx_statement ::= xx_return_statement",
 /*  93 */ "xx_statement ::= xx_mcall_statement",
 /*  94 */ "xx_statement ::= xx_scall_statement",
 /*  95 */ "xx_statement ::= xx_unset_statement",
 /*  96 */ "xx_statement ::= xx_throw_statement",
 /*  97 */ "xx_statement ::= xx_declare_statement",
 /*  98 */ "xx_statement ::= xx_break_statement",
 /*  99 */ "xx_statement ::= xx_continue_statement",
 /* 100 */ "xx_statement ::= xx_while_statement",
 /* 101 */ "xx_statement ::= xx_do_while_statement",
 /* 102 */ "xx_statement ::= xx_switch_statement",
 /* 103 */ "xx_statement ::= xx_for_statement",
 /* 104 */ "xx_statement ::= xx_comment",
 /* 105 */ "xx_break_statement ::= BREAK DOTCOMMA",
 /* 106 */ "xx_continue_statement ::= CONTINUE DOTCOMMA",
 /* 107 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN BRACKET_CLOSE",
 /* 108 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN BRACKET_CLOSE ELSE BRACKET_OPEN BRACKET_CLOSE",
 /* 109 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 110 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE ELSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 111 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE ELSE BRACKET_OPEN BRACKET_CLOSE",
 /* 112 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN BRACKET_CLOSE ELSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 113 */ "xx_switch_statement ::= SWITCH xx_eval_expr BRACKET_OPEN BRACKET_CLOSE",
 /* 114 */ "xx_switch_statement ::= SWITCH xx_eval_expr BRACKET_OPEN xx_case_clauses BRACKET_CLOSE",
 /* 115 */ "xx_case_clauses ::= xx_case_clauses xx_case_clause",
 /* 116 */ "xx_case_clauses ::= xx_case_clause",
 /* 117 */ "xx_case_clause ::= CASE xx_literal_expr COLON xx_statement_list",
 /* 118 */ "xx_case_clause ::= DEFAULT COLON xx_statement_list",
 /* 119 */ "xx_loop_statement ::= LOOP BRACKET_OPEN BRACKET_CLOSE",
 /* 120 */ "xx_loop_statement ::= LOOP BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 121 */ "xx_while_statement ::= WHILE xx_eval_expr BRACKET_OPEN BRACKET_CLOSE",
 /* 122 */ "xx_while_statement ::= WHILE xx_eval_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 123 */ "xx_do_while_statement ::= DO BRACKET_OPEN BRACKET_CLOSE WHILE xx_eval_expr DOTCOMMA",
 /* 124 */ "xx_do_while_statement ::= DO BRACKET_OPEN xx_statement_list BRACKET_CLOSE WHILE xx_eval_expr DOTCOMMA",
 /* 125 */ "xx_for_statement ::= FOR IDENTIFIER IN xx_common_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 126 */ "xx_for_statement ::= FOR IDENTIFIER IN REVERSE xx_common_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 127 */ "xx_for_statement ::= FOR IDENTIFIER COMMA IDENTIFIER IN xx_common_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 128 */ "xx_for_statement ::= FOR IDENTIFIER COMMA IDENTIFIER IN REVERSE xx_common_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 129 */ "xx_let_statement ::= LET xx_let_assignments DOTCOMMA",
 /* 130 */ "xx_let_assignments ::= xx_let_assignments COMMA xx_let_assignment",
 /* 131 */ "xx_let_assignments ::= xx_let_assignment",
 /* 132 */ "xx_assignment_operator ::= ASSIGN",
 /* 133 */ "xx_assignment_operator ::= ADDASSIGN",
 /* 134 */ "xx_assignment_operator ::= SUBASSIGN",
 /* 135 */ "xx_assignment_operator ::= MULASSIGN",
 /* 136 */ "xx_assignment_operator ::= DIVASSIGN",
 /* 137 */ "xx_assignment_operator ::= CONCATASSIGN",
 /* 138 */ "xx_let_assignment ::= IDENTIFIER xx_assignment_operator xx_assign_expr",
 /* 139 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER xx_assignment_operator xx_assign_expr",
 /* 140 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER SBRACKET_OPEN SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 141 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER SBRACKET_OPEN xx_index_expr SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 142 */ "xx_let_assignment ::= IDENTIFIER DOUBLECOLON IDENTIFIER xx_assignment_operator xx_assign_expr",
 /* 143 */ "xx_let_assignment ::= IDENTIFIER DOUBLECOLON IDENTIFIER SBRACKET_OPEN SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 144 */ "xx_let_assignment ::= IDENTIFIER DOUBLECOLON IDENTIFIER SBRACKET_OPEN xx_index_expr SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 145 */ "xx_let_assignment ::= IDENTIFIER SBRACKET_OPEN SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 146 */ "xx_let_assignment ::= IDENTIFIER SBRACKET_OPEN xx_index_expr SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 147 */ "xx_let_assignment ::= IDENTIFIER INCR",
 /* 148 */ "xx_let_assignment ::= IDENTIFIER DECR",
 /* 149 */ "xx_index_expr ::= xx_common_expr",
 /* 150 */ "xx_echo_statement ::= ECHO xx_echo_expressions DOTCOMMA",
 /* 151 */ "xx_echo_expressions ::= xx_echo_expressions COMMA xx_echo_expression",
 /* 152 */ "xx_echo_expressions ::= xx_echo_expression",
 /* 153 */ "xx_echo_expression ::= xx_common_expr",
 /* 154 */ "xx_mcall_statement ::= xx_mcall_expr DOTCOMMA",
 /* 155 */ "xx_scall_statement ::= xx_scall_expr DOTCOMMA",
 /* 156 */ "xx_return_statement ::= RETURN xx_common_expr DOTCOMMA",
 /* 157 */ "xx_return_statement ::= RETURN DOTCOMMA",
 /* 158 */ "xx_unset_statement ::= UNSET IDENTIFIER ARROW IDENTIFIER DOTCOMMA",
 /* 159 */ "xx_unset_statement ::= UNSET IDENTIFIER SBRACKET_OPEN IDENTIFIER SBRACKET_CLOSE DOTCOMMA",
 /* 160 */ "xx_unset_statement ::= UNSET IDENTIFIER SBRACKET_OPEN INTEGER SBRACKET_CLOSE DOTCOMMA",
 /* 161 */ "xx_unset_statement ::= UNSET IDENTIFIER SBRACKET_OPEN STRING SBRACKET_CLOSE DOTCOMMA",
 /* 162 */ "xx_throw_statement ::= THROW xx_common_expr DOTCOMMA",
 /* 163 */ "xx_declare_statement ::= TYPE_INTEGER xx_declare_variable_list DOTCOMMA",
 /* 164 */ "xx_declare_statement ::= TYPE_UINTEGER xx_declare_variable_list DOTCOMMA",
 /* 165 */ "xx_declare_statement ::= TYPE_CHAR xx_declare_variable_list DOTCOMMA",
 /* 166 */ "xx_declare_statement ::= TYPE_UCHAR xx_declare_variable_list DOTCOMMA",
 /* 167 */ "xx_declare_statement ::= TYPE_LONG xx_declare_variable_list DOTCOMMA",
 /* 168 */ "xx_declare_statement ::= TYPE_ULONG xx_declare_variable_list DOTCOMMA",
 /* 169 */ "xx_declare_statement ::= TYPE_DOUBLE xx_declare_variable_list DOTCOMMA",
 /* 170 */ "xx_declare_statement ::= TYPE_STRING xx_declare_variable_list DOTCOMMA",
 /* 171 */ "xx_declare_statement ::= TYPE_BOOL xx_declare_variable_list DOTCOMMA",
 /* 172 */ "xx_declare_statement ::= TYPE_VAR xx_declare_variable_list DOTCOMMA",
 /* 173 */ "xx_declare_variable_list ::= xx_declare_variable_list COMMA xx_declare_variable",
 /* 174 */ "xx_declare_variable_list ::= xx_declare_variable",
 /* 175 */ "xx_declare_variable ::= IDENTIFIER",
 /* 176 */ "xx_declare_variable ::= IDENTIFIER ASSIGN xx_literal_expr",
 /* 177 */ "xx_assign_expr ::= xx_common_expr",
 /* 178 */ "xx_common_expr ::= NOT xx_common_expr",
 /* 179 */ "xx_common_expr ::= ISSET xx_isset_expr",
 /* 180 */ "xx_common_expr ::= xx_common_expr EQUALS xx_common_expr",
 /* 181 */ "xx_common_expr ::= xx_common_expr NOTEQUALS xx_common_expr",
 /* 182 */ "xx_common_expr ::= xx_common_expr IDENTICAL xx_common_expr",
 /* 183 */ "xx_common_expr ::= xx_common_expr NOTIDENTICAL xx_common_expr",
 /* 184 */ "xx_common_expr ::= xx_common_expr LESS xx_common_expr",
 /* 185 */ "xx_common_expr ::= xx_common_expr GREATER xx_common_expr",
 /* 186 */ "xx_common_expr ::= xx_common_expr LESSEQUAL xx_common_expr",
 /* 187 */ "xx_common_expr ::= xx_common_expr GREATEREQUAL xx_common_expr",
 /* 188 */ "xx_common_expr ::= PARENTHESES_OPEN xx_common_expr PARENTHESES_CLOSE",
 /* 189 */ "xx_common_expr ::= PARENTHESES_OPEN xx_parameter_type PARENTHESES_CLOSE xx_common_expr",
 /* 190 */ "xx_common_expr ::= IDENTIFIER ARROW IDENTIFIER",
 /* 191 */ "xx_common_expr ::= IDENTIFIER DOUBLECOLON IDENTIFIER",
 /* 192 */ "xx_common_expr ::= IDENTIFIER DOUBLECOLON CONSTANT",
 /* 193 */ "xx_common_expr ::= IDENTIFIER SBRACKET_OPEN xx_common_expr SBRACKET_CLOSE",
 /* 194 */ "xx_common_expr ::= xx_common_expr ADD xx_common_expr",
 /* 195 */ "xx_common_expr ::= xx_common_expr SUB xx_common_expr",
 /* 196 */ "xx_common_expr ::= xx_common_expr MUL xx_common_expr",
 /* 197 */ "xx_common_expr ::= xx_common_expr DIV xx_common_expr",
 /* 198 */ "xx_common_expr ::= xx_common_expr MOD xx_common_expr",
 /* 199 */ "xx_common_expr ::= xx_common_expr CONCAT xx_common_expr",
 /* 200 */ "xx_common_expr ::= xx_common_expr AND xx_common_expr",
 /* 201 */ "xx_common_expr ::= xx_common_expr OR xx_common_expr",
 /* 202 */ "xx_common_expr ::= INSTANCEOF xx_common_expr",
 /* 203 */ "xx_common_expr ::= FETCH IDENTIFIER COMMA xx_isset_expr",
 /* 204 */ "xx_common_expr ::= TYPEOF xx_common_expr",
 /* 205 */ "xx_common_expr ::= CONSTANT",
 /* 206 */ "xx_common_expr ::= IDENTIFIER",
 /* 207 */ "xx_common_expr ::= INTEGER",
 /* 208 */ "xx_common_expr ::= STRING",
 /* 209 */ "xx_common_expr ::= CHAR",
 /* 210 */ "xx_common_expr ::= DOUBLE",
 /* 211 */ "xx_common_expr ::= NULL",
 /* 212 */ "xx_common_expr ::= TRUE",
 /* 213 */ "xx_common_expr ::= FALSE",
 /* 214 */ "xx_common_expr ::= SBRACKET_OPEN SBRACKET_CLOSE",
 /* 215 */ "xx_common_expr ::= SBRACKET_OPEN xx_array_list SBRACKET_CLOSE",
 /* 216 */ "xx_common_expr ::= NEW IDENTIFIER",
 /* 217 */ "xx_common_expr ::= NEW IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 218 */ "xx_common_expr ::= NEW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 219 */ "xx_common_expr ::= IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 220 */ "xx_common_expr ::= IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 221 */ "xx_common_expr ::= xx_mcall_expr",
 /* 222 */ "xx_common_expr ::= xx_scall_expr",
 /* 223 */ "xx_scall_expr ::= IDENTIFIER DOUBLECOLON IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 224 */ "xx_scall_expr ::= IDENTIFIER DOUBLECOLON IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 225 */ "xx_mcall_expr ::= IDENTIFIER ARROW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 226 */ "xx_mcall_expr ::= IDENTIFIER ARROW IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 227 */ "xx_call_parameters ::= xx_call_parameters COMMA xx_call_parameter",
 /* 228 */ "xx_call_parameters ::= xx_call_parameter",
 /* 229 */ "xx_call_parameter ::= xx_common_expr",
 /* 230 */ "xx_array_list ::= xx_array_list COMMA xx_array_item",
 /* 231 */ "xx_array_list ::= xx_array_item",
 /* 232 */ "xx_array_item ::= xx_array_key COLON xx_array_value",
 /* 233 */ "xx_array_item ::= xx_array_value",
 /* 234 */ "xx_array_key ::= IDENTIFIER",
 /* 235 */ "xx_array_key ::= STRING",
 /* 236 */ "xx_array_key ::= INTEGER",
 /* 237 */ "xx_array_value ::= xx_literal_expr",
 /* 238 */ "xx_array_value ::= IDENTIFIER",
 /* 239 */ "xx_literal_expr ::= INTEGER",
 /* 240 */ "xx_literal_expr ::= STRING",
 /* 241 */ "xx_literal_expr ::= DOUBLE",
 /* 242 */ "xx_literal_expr ::= NULL",
 /* 243 */ "xx_literal_expr ::= FALSE",
 /* 244 */ "xx_literal_expr ::= TRUE",
 /* 245 */ "xx_isset_expr ::= IDENTIFIER SBRACKET_OPEN IDENTIFIER SBRACKET_CLOSE",
 /* 246 */ "xx_isset_expr ::= IDENTIFIER SBRACKET_OPEN INTEGER SBRACKET_CLOSE",
 /* 247 */ "xx_isset_expr ::= IDENTIFIER SBRACKET_OPEN STRING SBRACKET_CLOSE",
 /* 248 */ "xx_eval_expr ::= xx_common_expr",
 /* 249 */ "xx_comment ::= COMMENT",
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
// 875 "parser.lemon"
{
	/*if ((yypminor->yy0)) {
		if ((yypminor->yy0)->free_flag) {
			efree((yypminor->yy0)->token);
		}
		efree((yypminor->yy0));
	}*/
}
// 2234 "parser.c"
      break;
    case 97:
// 888 "parser.lemon"
{ json_object_put((yypminor->yy259)); }
// 2239 "parser.c"
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
  { 96, 1 },
  { 97, 1 },
  { 98, 2 },
  { 98, 1 },
  { 99, 1 },
  { 99, 1 },
  { 99, 1 },
  { 99, 1 },
  { 100, 3 },
  { 102, 3 },
  { 102, 5 },
  { 101, 3 },
  { 101, 5 },
  { 101, 4 },
  { 101, 6 },
  { 101, 4 },
  { 101, 6 },
  { 105, 2 },
  { 105, 3 },
  { 104, 2 },
  { 104, 3 },
  { 106, 1 },
  { 106, 1 },
  { 106, 1 },
  { 106, 2 },
  { 106, 2 },
  { 106, 2 },
  { 106, 2 },
  { 106, 3 },
  { 106, 3 },
  { 108, 2 },
  { 108, 1 },
  { 111, 4 },
  { 111, 3 },
  { 111, 6 },
  { 111, 5 },
  { 109, 2 },
  { 109, 1 },
  { 110, 2 },
  { 110, 1 },
  { 107, 3 },
  { 107, 1 },
  { 114, 6 },
  { 114, 5 },
  { 115, 7 },
  { 115, 8 },
  { 115, 8 },
  { 115, 9 },
  { 115, 8 },
  { 115, 9 },
  { 115, 9 },
  { 115, 10 },
  { 116, 5 },
  { 116, 6 },
  { 116, 7 },
  { 116, 8 },
  { 112, 2 },
  { 112, 1 },
  { 119, 1 },
  { 119, 1 },
  { 119, 1 },
  { 119, 1 },
  { 119, 1 },
  { 119, 1 },
  { 119, 1 },
  { 119, 1 },
  { 117, 3 },
  { 117, 1 },
  { 120, 1 },
  { 120, 2 },
  { 120, 2 },
  { 120, 3 },
  { 120, 4 },
  { 120, 4 },
  { 122, 3 },
  { 121, 1 },
  { 121, 1 },
  { 121, 1 },
  { 121, 1 },
  { 121, 1 },
  { 121, 1 },
  { 121, 1 },
  { 121, 1 },
  { 121, 1 },
  { 121, 1 },
  { 121, 1 },
  { 118, 2 },
  { 118, 1 },
  { 123, 1 },
  { 123, 1 },
  { 123, 1 },
  { 123, 1 },
  { 123, 1 },
  { 123, 1 },
  { 123, 1 },
  { 123, 1 },
  { 123, 1 },
  { 123, 1 },
  { 123, 1 },
  { 123, 1 },
  { 123, 1 },
  { 123, 1 },
  { 123, 1 },
  { 123, 1 },
  { 123, 1 },
  { 134, 2 },
  { 135, 2 },
  { 125, 4 },
  { 125, 7 },
  { 125, 5 },
  { 125, 9 },
  { 125, 8 },
  { 125, 8 },
  { 138, 4 },
  { 138, 5 },
  { 141, 2 },
  { 141, 1 },
  { 142, 4 },
  { 142, 3 },
  { 126, 3 },
  { 126, 4 },
  { 136, 4 },
  { 136, 5 },
  { 137, 6 },
  { 137, 7 },
  { 139, 7 },
  { 139, 8 },
  { 139, 9 },
  { 139, 10 },
  { 124, 3 },
  { 144, 3 },
  { 144, 1 },
  { 146, 1 },
  { 146, 1 },
  { 146, 1 },
  { 146, 1 },
  { 146, 1 },
  { 146, 1 },
  { 145, 3 },
  { 145, 5 },
  { 145, 7 },
  { 145, 8 },
  { 145, 5 },
  { 145, 7 },
  { 145, 8 },
  { 145, 5 },
  { 145, 6 },
  { 145, 2 },
  { 145, 2 },
  { 148, 1 },
  { 127, 3 },
  { 149, 3 },
  { 149, 1 },
  { 150, 1 },
  { 129, 2 },
  { 130, 2 },
  { 128, 3 },
  { 128, 2 },
  { 131, 5 },
  { 131, 6 },
  { 131, 6 },
  { 131, 6 },
  { 132, 3 },
  { 133, 3 },
  { 133, 3 },
  { 133, 3 },
  { 133, 3 },
  { 133, 3 },
  { 133, 3 },
  { 133, 3 },
  { 133, 3 },
  { 133, 3 },
  { 133, 3 },
  { 153, 3 },
  { 153, 1 },
  { 154, 1 },
  { 154, 3 },
  { 147, 1 },
  { 143, 2 },
  { 143, 2 },
  { 143, 3 },
  { 143, 3 },
  { 143, 3 },
  { 143, 3 },
  { 143, 3 },
  { 143, 3 },
  { 143, 3 },
  { 143, 3 },
  { 143, 3 },
  { 143, 4 },
  { 143, 3 },
  { 143, 3 },
  { 143, 3 },
  { 143, 4 },
  { 143, 3 },
  { 143, 3 },
  { 143, 3 },
  { 143, 3 },
  { 143, 3 },
  { 143, 3 },
  { 143, 3 },
  { 143, 3 },
  { 143, 2 },
  { 143, 4 },
  { 143, 2 },
  { 143, 1 },
  { 143, 1 },
  { 143, 1 },
  { 143, 1 },
  { 143, 1 },
  { 143, 1 },
  { 143, 1 },
  { 143, 1 },
  { 143, 1 },
  { 143, 2 },
  { 143, 3 },
  { 143, 2 },
  { 143, 4 },
  { 143, 5 },
  { 143, 4 },
  { 143, 3 },
  { 143, 1 },
  { 143, 1 },
  { 152, 6 },
  { 152, 5 },
  { 151, 6 },
  { 151, 5 },
  { 157, 3 },
  { 157, 1 },
  { 158, 1 },
  { 156, 3 },
  { 156, 1 },
  { 159, 3 },
  { 159, 1 },
  { 160, 1 },
  { 160, 1 },
  { 160, 1 },
  { 161, 1 },
  { 161, 1 },
  { 113, 1 },
  { 113, 1 },
  { 113, 1 },
  { 113, 1 },
  { 113, 1 },
  { 113, 1 },
  { 155, 4 },
  { 155, 4 },
  { 155, 4 },
  { 140, 1 },
  { 103, 1 },
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
// 884 "parser.lemon"
{
	status->ret = yymsp[0].minor.yy259;
}
// 2706 "parser.c"
        break;
      case 1:
      case 4:
      case 5:
      case 6:
      case 7:
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
      case 102:
      case 103:
      case 104:
      case 149:
      case 177:
      case 221:
      case 222:
      case 229:
      case 237:
      case 248:
// 890 "parser.lemon"
{
	yygotominor.yy259 = yymsp[0].minor.yy259;
}
// 2741 "parser.c"
        break;
      case 2:
      case 30:
      case 36:
      case 38:
      case 56:
      case 86:
      case 115:
// 894 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_list(yymsp[-1].minor.yy259, yymsp[0].minor.yy259);
}
// 2754 "parser.c"
        break;
      case 3:
      case 31:
      case 37:
      case 39:
      case 41:
      case 57:
      case 67:
      case 87:
      case 116:
      case 131:
      case 152:
      case 174:
      case 228:
      case 231:
// 898 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_list(NULL, yymsp[0].minor.yy259);
}
// 2774 "parser.c"
        break;
      case 8:
// 918 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_namespace(yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 2783 "parser.c"
        break;
      case 9:
// 922 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_interface(yymsp[-1].minor.yy0, yymsp[0].minor.yy259, NULL, status->scanner_state);
  yy_destructor(33,&yymsp[-2].minor);
}
// 2791 "parser.c"
        break;
      case 10:
// 926 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_interface(yymsp[-3].minor.yy0, yymsp[0].minor.yy259, yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(33,&yymsp[-4].minor);
  yy_destructor(34,&yymsp[-2].minor);
}
// 2800 "parser.c"
        break;
      case 11:
// 930 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class(yymsp[-1].minor.yy0, yymsp[0].minor.yy259, 0, 0, NULL, status->scanner_state);
  yy_destructor(35,&yymsp[-2].minor);
}
// 2808 "parser.c"
        break;
      case 12:
// 934 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy259, 0, 0, yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(35,&yymsp[-4].minor);
  yy_destructor(34,&yymsp[-2].minor);
}
// 2817 "parser.c"
        break;
      case 13:
// 938 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class(yymsp[-1].minor.yy0, yymsp[0].minor.yy259, 1, 0, NULL, status->scanner_state);
  yy_destructor(36,&yymsp[-3].minor);
  yy_destructor(35,&yymsp[-2].minor);
}
// 2826 "parser.c"
        break;
      case 14:
// 942 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy259, 1, 0, yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(36,&yymsp[-5].minor);
  yy_destructor(35,&yymsp[-4].minor);
  yy_destructor(34,&yymsp[-2].minor);
}
// 2836 "parser.c"
        break;
      case 15:
// 946 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class(yymsp[-1].minor.yy0, yymsp[0].minor.yy259, 0, 1, NULL, status->scanner_state);
  yy_destructor(37,&yymsp[-3].minor);
  yy_destructor(35,&yymsp[-2].minor);
}
// 2845 "parser.c"
        break;
      case 16:
// 950 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy259, 0, 1, yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(37,&yymsp[-5].minor);
  yy_destructor(35,&yymsp[-4].minor);
  yy_destructor(34,&yymsp[-2].minor);
}
// 2855 "parser.c"
        break;
      case 17:
      case 19:
// 954 "parser.lemon"
{
	yygotominor.yy259 = NULL;
  yy_destructor(38,&yymsp[-1].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 2865 "parser.c"
        break;
      case 18:
      case 20:
// 958 "parser.lemon"
{
	yygotominor.yy259 = yymsp[-1].minor.yy259;
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 2875 "parser.c"
        break;
      case 21:
// 970 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_definition(yymsp[0].minor.yy259, NULL, NULL, status->scanner_state);
}
// 2882 "parser.c"
        break;
      case 22:
// 974 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_definition(NULL, NULL, yymsp[0].minor.yy259, status->scanner_state);
}
// 2889 "parser.c"
        break;
      case 23:
// 978 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_definition(NULL, yymsp[0].minor.yy259, NULL, status->scanner_state);
}
// 2896 "parser.c"
        break;
      case 24:
// 982 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_definition(yymsp[-1].minor.yy259, yymsp[0].minor.yy259, NULL, status->scanner_state);
}
// 2903 "parser.c"
        break;
      case 25:
// 986 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_definition(yymsp[-1].minor.yy259, NULL, yymsp[0].minor.yy259, status->scanner_state);
}
// 2910 "parser.c"
        break;
      case 26:
// 990 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_definition(yymsp[0].minor.yy259, NULL, yymsp[-1].minor.yy259, status->scanner_state);
}
// 2917 "parser.c"
        break;
      case 27:
// 994 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_definition(NULL, yymsp[0].minor.yy259, yymsp[-1].minor.yy259, status->scanner_state);
}
// 2924 "parser.c"
        break;
      case 28:
// 998 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_definition(yymsp[-2].minor.yy259, yymsp[0].minor.yy259, yymsp[-1].minor.yy259, status->scanner_state);
}
// 2931 "parser.c"
        break;
      case 29:
// 1002 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_definition(yymsp[-1].minor.yy259, yymsp[0].minor.yy259, yymsp[-2].minor.yy259, status->scanner_state);
}
// 2938 "parser.c"
        break;
      case 32:
// 1018 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_property(yymsp[-2].minor.yy259, yymsp[-1].minor.yy0, NULL, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(32,&yymsp[0].minor);
}
// 2946 "parser.c"
        break;
      case 33:
// 1022 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_property(yymsp[-2].minor.yy259, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(32,&yymsp[0].minor);
}
// 2954 "parser.c"
        break;
      case 34:
// 1026 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_property(yymsp[-4].minor.yy259, yymsp[-3].minor.yy0, yymsp[-1].minor.yy259, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(41,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 2963 "parser.c"
        break;
      case 35:
// 1030 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_property(yymsp[-4].minor.yy259, yymsp[-3].minor.yy0, yymsp[-1].minor.yy259, NULL, status->scanner_state);
  yy_destructor(41,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 2972 "parser.c"
        break;
      case 40:
// 1050 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_list(yymsp[-2].minor.yy259, yymsp[0].minor.yy259);
  yy_destructor(32,&yymsp[-1].minor);
}
// 2980 "parser.c"
        break;
      case 42:
// 1058 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy259, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(42,&yymsp[-4].minor);
  yy_destructor(41,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 2990 "parser.c"
        break;
      case 43:
// 1062 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy259, NULL, status->scanner_state);
  yy_destructor(42,&yymsp[-4].minor);
  yy_destructor(41,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3000 "parser.c"
        break;
      case 44:
// 1066 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_method(yymsp[-6].minor.yy259, yymsp[-4].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(44,&yymsp[-5].minor);
  yy_destructor(45,&yymsp[-3].minor);
  yy_destructor(29,&yymsp[-2].minor);
  yy_destructor(38,&yymsp[-1].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 3012 "parser.c"
        break;
      case 45:
// 1070 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_method(yymsp[-7].minor.yy259, yymsp[-5].minor.yy0, yymsp[-3].minor.yy259, NULL, NULL, status->scanner_state);
  yy_destructor(44,&yymsp[-6].minor);
  yy_destructor(45,&yymsp[-4].minor);
  yy_destructor(29,&yymsp[-2].minor);
  yy_destructor(38,&yymsp[-1].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 3024 "parser.c"
        break;
      case 46:
// 1074 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_method(yymsp[-7].minor.yy259, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy259, NULL, status->scanner_state);
  yy_destructor(44,&yymsp[-6].minor);
  yy_destructor(45,&yymsp[-4].minor);
  yy_destructor(29,&yymsp[-3].minor);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 3036 "parser.c"
        break;
      case 47:
// 1078 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_method(yymsp[-8].minor.yy259, yymsp[-6].minor.yy0, yymsp[-4].minor.yy259, yymsp[-1].minor.yy259, NULL, status->scanner_state);
  yy_destructor(44,&yymsp[-7].minor);
  yy_destructor(45,&yymsp[-5].minor);
  yy_destructor(29,&yymsp[-3].minor);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 3048 "parser.c"
        break;
      case 48:
// 1082 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_method(yymsp[-6].minor.yy259, yymsp[-4].minor.yy0, NULL, NULL, yymsp[-7].minor.yy0, status->scanner_state);
  yy_destructor(44,&yymsp[-5].minor);
  yy_destructor(45,&yymsp[-3].minor);
  yy_destructor(29,&yymsp[-2].minor);
  yy_destructor(38,&yymsp[-1].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 3060 "parser.c"
        break;
      case 49:
// 1086 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_method(yymsp[-7].minor.yy259, yymsp[-5].minor.yy0, yymsp[-3].minor.yy259, NULL, yymsp[-8].minor.yy0, status->scanner_state);
  yy_destructor(44,&yymsp[-6].minor);
  yy_destructor(45,&yymsp[-4].minor);
  yy_destructor(29,&yymsp[-2].minor);
  yy_destructor(38,&yymsp[-1].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 3072 "parser.c"
        break;
      case 50:
// 1090 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_method(yymsp[-7].minor.yy259, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy259, yymsp[-8].minor.yy0, status->scanner_state);
  yy_destructor(44,&yymsp[-6].minor);
  yy_destructor(45,&yymsp[-4].minor);
  yy_destructor(29,&yymsp[-3].minor);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 3084 "parser.c"
        break;
      case 51:
// 1094 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_method(yymsp[-8].minor.yy259, yymsp[-6].minor.yy0, yymsp[-4].minor.yy259, yymsp[-1].minor.yy259, yymsp[-9].minor.yy0, status->scanner_state);
  yy_destructor(44,&yymsp[-7].minor);
  yy_destructor(45,&yymsp[-5].minor);
  yy_destructor(29,&yymsp[-3].minor);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 3096 "parser.c"
        break;
      case 52:
// 1098 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_method(yymsp[-4].minor.yy259, yymsp[-2].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(44,&yymsp[-3].minor);
  yy_destructor(45,&yymsp[-1].minor);
  yy_destructor(29,&yymsp[0].minor);
}
// 3106 "parser.c"
        break;
      case 53:
// 1102 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_method(yymsp[-5].minor.yy259, yymsp[-3].minor.yy0, yymsp[-1].minor.yy259, NULL, NULL, status->scanner_state);
  yy_destructor(44,&yymsp[-4].minor);
  yy_destructor(45,&yymsp[-2].minor);
  yy_destructor(29,&yymsp[0].minor);
}
// 3116 "parser.c"
        break;
      case 54:
// 1106 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_method(yymsp[-5].minor.yy259, yymsp[-3].minor.yy0, NULL, NULL, yymsp[-6].minor.yy0, status->scanner_state);
  yy_destructor(44,&yymsp[-4].minor);
  yy_destructor(45,&yymsp[-2].minor);
  yy_destructor(29,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3127 "parser.c"
        break;
      case 55:
// 1110 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_method(yymsp[-6].minor.yy259, yymsp[-4].minor.yy0, yymsp[-2].minor.yy259, NULL, yymsp[-7].minor.yy0, status->scanner_state);
  yy_destructor(44,&yymsp[-5].minor);
  yy_destructor(45,&yymsp[-3].minor);
  yy_destructor(29,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3138 "parser.c"
        break;
      case 58:
// 1122 "parser.lemon"
{
	yygotominor.yy259 = json_object_new_string("public");
  yy_destructor(1,&yymsp[0].minor);
}
// 3146 "parser.c"
        break;
      case 59:
// 1126 "parser.lemon"
{
	yygotominor.yy259 = json_object_new_string("protected");
  yy_destructor(2,&yymsp[0].minor);
}
// 3154 "parser.c"
        break;
      case 60:
// 1130 "parser.lemon"
{
	yygotominor.yy259 = json_object_new_string("private");
  yy_destructor(4,&yymsp[0].minor);
}
// 3162 "parser.c"
        break;
      case 61:
// 1134 "parser.lemon"
{
	yygotominor.yy259 = json_object_new_string("static");
  yy_destructor(3,&yymsp[0].minor);
}
// 3170 "parser.c"
        break;
      case 62:
// 1138 "parser.lemon"
{
	yygotominor.yy259 = json_object_new_string("scoped");
  yy_destructor(5,&yymsp[0].minor);
}
// 3178 "parser.c"
        break;
      case 63:
// 1142 "parser.lemon"
{
	yygotominor.yy259 = json_object_new_string("inline");
  yy_destructor(46,&yymsp[0].minor);
}
// 3186 "parser.c"
        break;
      case 64:
// 1146 "parser.lemon"
{
	yygotominor.yy259 = json_object_new_string("abstract");
  yy_destructor(36,&yymsp[0].minor);
}
// 3194 "parser.c"
        break;
      case 65:
// 1150 "parser.lemon"
{
	yygotominor.yy259 = json_object_new_string("final");
  yy_destructor(37,&yymsp[0].minor);
}
// 3202 "parser.c"
        break;
      case 66:
      case 130:
      case 151:
      case 173:
      case 227:
      case 230:
// 1154 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_list(yymsp[-2].minor.yy259, yymsp[0].minor.yy259);
  yy_destructor(6,&yymsp[-1].minor);
}
// 3215 "parser.c"
        break;
      case 68:
// 1162 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_parameter(NULL, NULL, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 3222 "parser.c"
        break;
      case 69:
// 1166 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_parameter(yymsp[-1].minor.yy259, NULL, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 3229 "parser.c"
        break;
      case 70:
// 1170 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_parameter(NULL, yymsp[-1].minor.yy259, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 3236 "parser.c"
        break;
      case 71:
// 1174 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_parameter(NULL, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy259, status->scanner_state);
  yy_destructor(41,&yymsp[-1].minor);
}
// 3244 "parser.c"
        break;
      case 72:
// 1178 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_parameter(yymsp[-3].minor.yy259, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy259, status->scanner_state);
  yy_destructor(41,&yymsp[-1].minor);
}
// 3252 "parser.c"
        break;
      case 73:
// 1182 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_parameter(NULL, yymsp[-3].minor.yy259, yymsp[-2].minor.yy0, yymsp[0].minor.yy259, status->scanner_state);
  yy_destructor(41,&yymsp[-1].minor);
}
// 3260 "parser.c"
        break;
      case 74:
// 1186 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(11,&yymsp[-2].minor);
  yy_destructor(12,&yymsp[0].minor);
}
// 3269 "parser.c"
        break;
      case 75:
// 1190 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_type(XX_TYPE_INTEGER);
  yy_destructor(47,&yymsp[0].minor);
}
// 3277 "parser.c"
        break;
      case 76:
// 1194 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_type(XX_TYPE_UINTEGER);
  yy_destructor(48,&yymsp[0].minor);
}
// 3285 "parser.c"
        break;
      case 77:
// 1198 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_type(XX_TYPE_LONG);
  yy_destructor(49,&yymsp[0].minor);
}
// 3293 "parser.c"
        break;
      case 78:
// 1202 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_type(XX_TYPE_ULONG);
  yy_destructor(50,&yymsp[0].minor);
}
// 3301 "parser.c"
        break;
      case 79:
// 1206 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_type(XX_TYPE_CHAR);
  yy_destructor(51,&yymsp[0].minor);
}
// 3309 "parser.c"
        break;
      case 80:
// 1210 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_type(XX_TYPE_UCHAR);
  yy_destructor(52,&yymsp[0].minor);
}
// 3317 "parser.c"
        break;
      case 81:
// 1214 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_type(XX_TYPE_DOUBLE);
  yy_destructor(53,&yymsp[0].minor);
}
// 3325 "parser.c"
        break;
      case 82:
// 1218 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_type(XX_TYPE_BOOL);
  yy_destructor(54,&yymsp[0].minor);
}
// 3333 "parser.c"
        break;
      case 83:
// 1222 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_type(XX_TYPE_STRING);
  yy_destructor(55,&yymsp[0].minor);
}
// 3341 "parser.c"
        break;
      case 84:
// 1226 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_type(XX_TYPE_ARRAY);
  yy_destructor(56,&yymsp[0].minor);
}
// 3349 "parser.c"
        break;
      case 85:
// 1230 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_type(XX_TYPE_VAR);
  yy_destructor(57,&yymsp[0].minor);
}
// 3357 "parser.c"
        break;
      case 105:
// 1310 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_break_statement(status->scanner_state);
  yy_destructor(58,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3366 "parser.c"
        break;
      case 106:
// 1314 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_continue_statement(status->scanner_state);
  yy_destructor(59,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3375 "parser.c"
        break;
      case 107:
// 1318 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_if_statement(yymsp[-2].minor.yy259, NULL, NULL, status->scanner_state);
  yy_destructor(60,&yymsp[-3].minor);
  yy_destructor(38,&yymsp[-1].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 3385 "parser.c"
        break;
      case 108:
// 1322 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_if_statement(yymsp[-5].minor.yy259, NULL, NULL, status->scanner_state);
  yy_destructor(60,&yymsp[-6].minor);
  yy_destructor(38,&yymsp[-4].minor);
  yy_destructor(39,&yymsp[-3].minor);
  yy_destructor(61,&yymsp[-2].minor);
  yy_destructor(38,&yymsp[-1].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 3398 "parser.c"
        break;
      case 109:
// 1326 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_if_statement(yymsp[-3].minor.yy259, yymsp[-1].minor.yy259, NULL, status->scanner_state);
  yy_destructor(60,&yymsp[-4].minor);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 3408 "parser.c"
        break;
      case 110:
// 1330 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_if_statement(yymsp[-7].minor.yy259, yymsp[-5].minor.yy259, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(60,&yymsp[-8].minor);
  yy_destructor(38,&yymsp[-6].minor);
  yy_destructor(39,&yymsp[-4].minor);
  yy_destructor(61,&yymsp[-3].minor);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 3421 "parser.c"
        break;
      case 111:
// 1334 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_if_statement(yymsp[-6].minor.yy259, yymsp[-4].minor.yy259, NULL, status->scanner_state);
  yy_destructor(60,&yymsp[-7].minor);
  yy_destructor(38,&yymsp[-5].minor);
  yy_destructor(39,&yymsp[-3].minor);
  yy_destructor(61,&yymsp[-2].minor);
  yy_destructor(38,&yymsp[-1].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 3434 "parser.c"
        break;
      case 112:
// 1338 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_if_statement(yymsp[-6].minor.yy259, NULL, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(60,&yymsp[-7].minor);
  yy_destructor(38,&yymsp[-5].minor);
  yy_destructor(39,&yymsp[-4].minor);
  yy_destructor(61,&yymsp[-3].minor);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 3447 "parser.c"
        break;
      case 113:
// 1342 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_switch_statement(yymsp[-2].minor.yy259, NULL, status->scanner_state);
  yy_destructor(62,&yymsp[-3].minor);
  yy_destructor(38,&yymsp[-1].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 3457 "parser.c"
        break;
      case 114:
// 1346 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_switch_statement(yymsp[-3].minor.yy259, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(62,&yymsp[-4].minor);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 3467 "parser.c"
        break;
      case 117:
// 1358 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_case_clause(yymsp[-2].minor.yy259, yymsp[0].minor.yy259, status->scanner_state);
  yy_destructor(63,&yymsp[-3].minor);
  yy_destructor(64,&yymsp[-1].minor);
}
// 3476 "parser.c"
        break;
      case 118:
// 1362 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_case_clause(NULL, yymsp[0].minor.yy259, status->scanner_state);
  yy_destructor(65,&yymsp[-2].minor);
  yy_destructor(64,&yymsp[-1].minor);
}
// 3485 "parser.c"
        break;
      case 119:
// 1366 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_loop_statement(NULL, status->scanner_state);
  yy_destructor(66,&yymsp[-2].minor);
  yy_destructor(38,&yymsp[-1].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 3495 "parser.c"
        break;
      case 120:
// 1370 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_loop_statement(yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(66,&yymsp[-3].minor);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 3505 "parser.c"
        break;
      case 121:
// 1374 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_while_statement(yymsp[-2].minor.yy259, NULL, status->scanner_state);
  yy_destructor(67,&yymsp[-3].minor);
  yy_destructor(38,&yymsp[-1].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 3515 "parser.c"
        break;
      case 122:
// 1378 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_while_statement(yymsp[-3].minor.yy259, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(67,&yymsp[-4].minor);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 3525 "parser.c"
        break;
      case 123:
// 1382 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_do_while_statement(yymsp[-1].minor.yy259, NULL, status->scanner_state);
  yy_destructor(68,&yymsp[-5].minor);
  yy_destructor(38,&yymsp[-4].minor);
  yy_destructor(39,&yymsp[-3].minor);
  yy_destructor(67,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3537 "parser.c"
        break;
      case 124:
// 1386 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_do_while_statement(yymsp[-1].minor.yy259, yymsp[-4].minor.yy259, status->scanner_state);
  yy_destructor(68,&yymsp[-6].minor);
  yy_destructor(38,&yymsp[-5].minor);
  yy_destructor(39,&yymsp[-3].minor);
  yy_destructor(67,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3549 "parser.c"
        break;
      case 125:
// 1390 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_for_statement(yymsp[-3].minor.yy259, NULL, yymsp[-5].minor.yy0, 0, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(69,&yymsp[-6].minor);
  yy_destructor(70,&yymsp[-4].minor);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 3560 "parser.c"
        break;
      case 126:
// 1394 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_for_statement(yymsp[-3].minor.yy259, NULL, yymsp[-6].minor.yy0, 1, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(69,&yymsp[-7].minor);
  yy_destructor(70,&yymsp[-5].minor);
  yy_destructor(71,&yymsp[-4].minor);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 3572 "parser.c"
        break;
      case 127:
// 1398 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_for_statement(yymsp[-3].minor.yy259, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, 0, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(69,&yymsp[-8].minor);
  yy_destructor(6,&yymsp[-6].minor);
  yy_destructor(70,&yymsp[-4].minor);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 3584 "parser.c"
        break;
      case 128:
// 1402 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_for_statement(yymsp[-3].minor.yy259, yymsp[-8].minor.yy0, yymsp[-6].minor.yy0, 1, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(69,&yymsp[-9].minor);
  yy_destructor(6,&yymsp[-7].minor);
  yy_destructor(70,&yymsp[-5].minor);
  yy_destructor(71,&yymsp[-4].minor);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 3597 "parser.c"
        break;
      case 129:
// 1406 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_let_statement(yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(72,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3606 "parser.c"
        break;
      case 132:
// 1419 "parser.lemon"
{
	yygotominor.yy259 = json_object_new_string("assign");
  yy_destructor(41,&yymsp[0].minor);
}
// 3614 "parser.c"
        break;
      case 133:
// 1424 "parser.lemon"
{
	yygotominor.yy259 = json_object_new_string("add-assign");
  yy_destructor(73,&yymsp[0].minor);
}
// 3622 "parser.c"
        break;
      case 134:
// 1429 "parser.lemon"
{
	yygotominor.yy259 = json_object_new_string("sub-assign");
  yy_destructor(74,&yymsp[0].minor);
}
// 3630 "parser.c"
        break;
      case 135:
// 1433 "parser.lemon"
{
	yygotominor.yy259 = json_object_new_string("mult-assign");
  yy_destructor(75,&yymsp[0].minor);
}
// 3638 "parser.c"
        break;
      case 136:
// 1437 "parser.lemon"
{
	yygotominor.yy259 = json_object_new_string("div-assign");
  yy_destructor(76,&yymsp[0].minor);
}
// 3646 "parser.c"
        break;
      case 137:
// 1441 "parser.lemon"
{
	yygotominor.yy259 = json_object_new_string("concat-assign");
  yy_destructor(77,&yymsp[0].minor);
}
// 3654 "parser.c"
        break;
      case 138:
// 1446 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_let_assignment("variable", yymsp[-1].minor.yy259, yymsp[-2].minor.yy0, NULL, NULL, yymsp[0].minor.yy259, status->scanner_state);
}
// 3661 "parser.c"
        break;
      case 139:
// 1451 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_let_assignment("object-property", yymsp[-1].minor.yy259, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, yymsp[0].minor.yy259, status->scanner_state);
  yy_destructor(78,&yymsp[-3].minor);
}
// 3669 "parser.c"
        break;
      case 140:
// 1456 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_let_assignment("object-property-append", yymsp[-1].minor.yy259, yymsp[-6].minor.yy0, yymsp[-4].minor.yy0, NULL, yymsp[0].minor.yy259, status->scanner_state);
  yy_destructor(78,&yymsp[-5].minor);
  yy_destructor(79,&yymsp[-3].minor);
  yy_destructor(80,&yymsp[-2].minor);
}
// 3679 "parser.c"
        break;
      case 141:
// 1461 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_let_assignment("object-property-array-index", yymsp[-1].minor.yy259, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, yymsp[-3].minor.yy259, yymsp[0].minor.yy259, status->scanner_state);
  yy_destructor(78,&yymsp[-6].minor);
  yy_destructor(79,&yymsp[-4].minor);
  yy_destructor(80,&yymsp[-2].minor);
}
// 3689 "parser.c"
        break;
      case 142:
// 1466 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_let_assignment("static-property", yymsp[-1].minor.yy259, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, yymsp[0].minor.yy259, status->scanner_state);
  yy_destructor(81,&yymsp[-3].minor);
}
// 3697 "parser.c"
        break;
      case 143:
// 1471 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_let_assignment("static-property-append", yymsp[-1].minor.yy259, yymsp[-6].minor.yy0, yymsp[-4].minor.yy0, NULL, yymsp[0].minor.yy259, status->scanner_state);
  yy_destructor(81,&yymsp[-5].minor);
  yy_destructor(79,&yymsp[-3].minor);
  yy_destructor(80,&yymsp[-2].minor);
}
// 3707 "parser.c"
        break;
      case 144:
// 1476 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_let_assignment("static-property-array-index", yymsp[-1].minor.yy259, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, yymsp[-3].minor.yy259, yymsp[0].minor.yy259, status->scanner_state);
  yy_destructor(81,&yymsp[-6].minor);
  yy_destructor(79,&yymsp[-4].minor);
  yy_destructor(80,&yymsp[-2].minor);
}
// 3717 "parser.c"
        break;
      case 145:
// 1481 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_let_assignment("variable-append", yymsp[-1].minor.yy259, yymsp[-4].minor.yy0, NULL, NULL, yymsp[0].minor.yy259, status->scanner_state);
  yy_destructor(79,&yymsp[-3].minor);
  yy_destructor(80,&yymsp[-2].minor);
}
// 3726 "parser.c"
        break;
      case 146:
// 1486 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_let_assignment("array-index", yymsp[-1].minor.yy259, yymsp[-5].minor.yy0, NULL, yymsp[-3].minor.yy259, yymsp[0].minor.yy259, status->scanner_state);
  yy_destructor(79,&yymsp[-4].minor);
  yy_destructor(80,&yymsp[-2].minor);
}
// 3735 "parser.c"
        break;
      case 147:
// 1491 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_let_assignment("incr", NULL, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(82,&yymsp[0].minor);
}
// 3743 "parser.c"
        break;
      case 148:
// 1496 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_let_assignment("decr", NULL, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(83,&yymsp[0].minor);
}
// 3751 "parser.c"
        break;
      case 150:
// 1504 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_echo_statement(yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(84,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3760 "parser.c"
        break;
      case 153:
// 1516 "parser.lemon"
{
	yygotominor.yy259 = yymsp[0].minor.yy259;;
}
// 3767 "parser.c"
        break;
      case 154:
// 1521 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_mcall_statement(yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(32,&yymsp[0].minor);
}
// 3775 "parser.c"
        break;
      case 155:
// 1526 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_scall_statement(yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(32,&yymsp[0].minor);
}
// 3783 "parser.c"
        break;
      case 156:
// 1531 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_return_statement(yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(85,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3792 "parser.c"
        break;
      case 157:
// 1536 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_return_statement(NULL, status->scanner_state);
  yy_destructor(85,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3801 "parser.c"
        break;
      case 158:
// 1541 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(86,&yymsp[-4].minor);
  yy_destructor(78,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3815 "parser.c"
        break;
      case 159:
// 1550 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(86,&yymsp[-5].minor);
  yy_destructor(79,&yymsp[-3].minor);
  yy_destructor(80,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3830 "parser.c"
        break;
      case 160:
// 1559 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_INTEGER, yymsp[-2].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(86,&yymsp[-5].minor);
  yy_destructor(79,&yymsp[-3].minor);
  yy_destructor(80,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3845 "parser.c"
        break;
      case 161:
// 1568 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_STRING, yymsp[-2].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(86,&yymsp[-5].minor);
  yy_destructor(79,&yymsp[-3].minor);
  yy_destructor(80,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3860 "parser.c"
        break;
      case 162:
// 1577 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_throw_exception(yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(89,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3869 "parser.c"
        break;
      case 163:
// 1581 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_declare_statement(XX_T_TYPE_INTEGER, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(47,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3878 "parser.c"
        break;
      case 164:
// 1585 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_declare_statement(XX_T_TYPE_UINTEGER, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3887 "parser.c"
        break;
      case 165:
// 1589 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_declare_statement(XX_T_TYPE_CHAR, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(51,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3896 "parser.c"
        break;
      case 166:
// 1593 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_declare_statement(XX_T_TYPE_UCHAR, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(52,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3905 "parser.c"
        break;
      case 167:
// 1597 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_declare_statement(XX_T_TYPE_LONG, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3914 "parser.c"
        break;
      case 168:
// 1601 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_declare_statement(XX_T_TYPE_ULONG, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(50,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3923 "parser.c"
        break;
      case 169:
// 1605 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_declare_statement(XX_T_TYPE_DOUBLE, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(53,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3932 "parser.c"
        break;
      case 170:
// 1609 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_declare_statement(XX_T_TYPE_STRING, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(55,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3941 "parser.c"
        break;
      case 171:
// 1613 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_declare_statement(XX_T_TYPE_BOOL, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3950 "parser.c"
        break;
      case 172:
// 1617 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_declare_statement(XX_T_TYPE_VAR, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(57,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3959 "parser.c"
        break;
      case 175:
// 1629 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_declare_variable(yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 3966 "parser.c"
        break;
      case 176:
// 1633 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_declare_variable(yymsp[-2].minor.yy0, yymsp[0].minor.yy259, status->scanner_state);
  yy_destructor(41,&yymsp[-1].minor);
}
// 3974 "parser.c"
        break;
      case 178:
// 1641 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("not", yymsp[0].minor.yy259, NULL, NULL, status->scanner_state);
  yy_destructor(28,&yymsp[-1].minor);
}
// 3982 "parser.c"
        break;
      case 179:
// 1645 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("isset", yymsp[0].minor.yy259, NULL, NULL, status->scanner_state);
  yy_destructor(23,&yymsp[-1].minor);
}
// 3990 "parser.c"
        break;
      case 180:
// 1649 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("equals", yymsp[-2].minor.yy259, yymsp[0].minor.yy259, NULL, status->scanner_state);
  yy_destructor(9,&yymsp[-1].minor);
}
// 3998 "parser.c"
        break;
      case 181:
// 1653 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("not-equals", yymsp[-2].minor.yy259, yymsp[0].minor.yy259, NULL, status->scanner_state);
  yy_destructor(16,&yymsp[-1].minor);
}
// 4006 "parser.c"
        break;
      case 182:
// 1657 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("identical", yymsp[-2].minor.yy259, yymsp[0].minor.yy259, NULL, status->scanner_state);
  yy_destructor(10,&yymsp[-1].minor);
}
// 4014 "parser.c"
        break;
      case 183:
// 1661 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("not-identical", yymsp[-2].minor.yy259, yymsp[0].minor.yy259, NULL, status->scanner_state);
  yy_destructor(15,&yymsp[-1].minor);
}
// 4022 "parser.c"
        break;
      case 184:
// 1665 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("less", yymsp[-2].minor.yy259, yymsp[0].minor.yy259, NULL, status->scanner_state);
  yy_destructor(11,&yymsp[-1].minor);
}
// 4030 "parser.c"
        break;
      case 185:
// 1669 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("greater", yymsp[-2].minor.yy259, yymsp[0].minor.yy259, NULL, status->scanner_state);
  yy_destructor(12,&yymsp[-1].minor);
}
// 4038 "parser.c"
        break;
      case 186:
// 1673 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("less-equal", yymsp[-2].minor.yy259, yymsp[0].minor.yy259, NULL, status->scanner_state);
  yy_destructor(13,&yymsp[-1].minor);
}
// 4046 "parser.c"
        break;
      case 187:
// 1677 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("greater-equal", yymsp[-2].minor.yy259, yymsp[0].minor.yy259, NULL, status->scanner_state);
  yy_destructor(14,&yymsp[-1].minor);
}
// 4054 "parser.c"
        break;
      case 188:
// 1681 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("list", yymsp[-1].minor.yy259, NULL, NULL, status->scanner_state);
  yy_destructor(45,&yymsp[-2].minor);
  yy_destructor(29,&yymsp[0].minor);
}
// 4063 "parser.c"
        break;
      case 189:
// 1685 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("cast", yymsp[-2].minor.yy259, yymsp[0].minor.yy259, NULL, status->scanner_state);
  yy_destructor(45,&yymsp[-3].minor);
  yy_destructor(29,&yymsp[-1].minor);
}
// 4072 "parser.c"
        break;
      case 190:
// 1689 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("property-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(78,&yymsp[-1].minor);
}
// 4080 "parser.c"
        break;
      case 191:
// 1693 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("static-property-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(81,&yymsp[-1].minor);
}
// 4088 "parser.c"
        break;
      case 192:
// 1697 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("static-constant-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(81,&yymsp[-1].minor);
}
// 4096 "parser.c"
        break;
      case 193:
// 1702 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), yymsp[-1].minor.yy259, NULL, status->scanner_state);
  yy_destructor(79,&yymsp[-2].minor);
  yy_destructor(80,&yymsp[0].minor);
}
// 4105 "parser.c"
        break;
      case 194:
// 1707 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("add", yymsp[-2].minor.yy259, yymsp[0].minor.yy259, NULL, status->scanner_state);
  yy_destructor(17,&yymsp[-1].minor);
}
// 4113 "parser.c"
        break;
      case 195:
// 1712 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("sub", yymsp[-2].minor.yy259, yymsp[0].minor.yy259, NULL, status->scanner_state);
  yy_destructor(18,&yymsp[-1].minor);
}
// 4121 "parser.c"
        break;
      case 196:
// 1717 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("mul", yymsp[-2].minor.yy259, yymsp[0].minor.yy259, NULL, status->scanner_state);
  yy_destructor(20,&yymsp[-1].minor);
}
// 4129 "parser.c"
        break;
      case 197:
// 1722 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("div", yymsp[-2].minor.yy259, yymsp[0].minor.yy259, NULL, status->scanner_state);
  yy_destructor(21,&yymsp[-1].minor);
}
// 4137 "parser.c"
        break;
      case 198:
// 1727 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("mod", yymsp[-2].minor.yy259, yymsp[0].minor.yy259, NULL, status->scanner_state);
  yy_destructor(22,&yymsp[-1].minor);
}
// 4145 "parser.c"
        break;
      case 199:
// 1732 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("concat", yymsp[-2].minor.yy259, yymsp[0].minor.yy259, NULL, status->scanner_state);
  yy_destructor(19,&yymsp[-1].minor);
}
// 4153 "parser.c"
        break;
      case 200:
// 1737 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("and", yymsp[-2].minor.yy259, yymsp[0].minor.yy259, NULL, status->scanner_state);
  yy_destructor(7,&yymsp[-1].minor);
}
// 4161 "parser.c"
        break;
      case 201:
// 1742 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("or", yymsp[-2].minor.yy259, yymsp[0].minor.yy259, NULL, status->scanner_state);
  yy_destructor(8,&yymsp[-1].minor);
}
// 4169 "parser.c"
        break;
      case 202:
// 1747 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("instanceof", yymsp[0].minor.yy259, NULL, NULL, status->scanner_state);
  yy_destructor(26,&yymsp[-1].minor);
}
// 4177 "parser.c"
        break;
      case 203:
// 1752 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("fetch", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), yymsp[0].minor.yy259, NULL, status->scanner_state);
  yy_destructor(24,&yymsp[-3].minor);
  yy_destructor(6,&yymsp[-1].minor);
}
// 4186 "parser.c"
        break;
      case 204:
// 1757 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("typeof", yymsp[0].minor.yy259, NULL, NULL, status->scanner_state);
  yy_destructor(25,&yymsp[-1].minor);
}
// 4194 "parser.c"
        break;
      case 205:
// 1762 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_literal(XX_T_CONSTANT, yymsp[0].minor.yy0, status->scanner_state);
}
// 4201 "parser.c"
        break;
      case 206:
      case 234:
      case 238:
// 1767 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state);
}
// 4210 "parser.c"
        break;
      case 207:
      case 236:
      case 239:
// 1772 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_literal(XX_T_INTEGER, yymsp[0].minor.yy0, status->scanner_state);
}
// 4219 "parser.c"
        break;
      case 208:
      case 235:
      case 240:
// 1777 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_literal(XX_T_STRING, yymsp[0].minor.yy0, status->scanner_state);
}
// 4228 "parser.c"
        break;
      case 209:
// 1782 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_literal(XX_T_CHAR, yymsp[0].minor.yy0, status->scanner_state);
}
// 4235 "parser.c"
        break;
      case 210:
      case 241:
// 1787 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_literal(XX_T_DOUBLE, yymsp[0].minor.yy0, status->scanner_state);
}
// 4243 "parser.c"
        break;
      case 211:
      case 242:
// 1792 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_literal(XX_T_NULL, NULL, status->scanner_state);
  yy_destructor(92,&yymsp[0].minor);
}
// 4252 "parser.c"
        break;
      case 212:
      case 244:
// 1797 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_literal(XX_T_TRUE, NULL, status->scanner_state);
  yy_destructor(93,&yymsp[0].minor);
}
// 4261 "parser.c"
        break;
      case 213:
      case 243:
// 1802 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_literal(XX_T_FALSE, NULL, status->scanner_state);
  yy_destructor(94,&yymsp[0].minor);
}
// 4270 "parser.c"
        break;
      case 214:
// 1807 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("empty-array", NULL, NULL, NULL, status->scanner_state);
  yy_destructor(79,&yymsp[-1].minor);
  yy_destructor(80,&yymsp[0].minor);
}
// 4279 "parser.c"
        break;
      case 215:
// 1812 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("array", yymsp[-1].minor.yy259, NULL, NULL, status->scanner_state);
  yy_destructor(79,&yymsp[-2].minor);
  yy_destructor(80,&yymsp[0].minor);
}
// 4288 "parser.c"
        break;
      case 216:
// 1817 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_new_instance(yymsp[0].minor.yy0, NULL, status->scanner_state);
  yy_destructor(27,&yymsp[-1].minor);
}
// 4296 "parser.c"
        break;
      case 217:
// 1822 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_new_instance(yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(27,&yymsp[-3].minor);
  yy_destructor(45,&yymsp[-1].minor);
  yy_destructor(29,&yymsp[0].minor);
}
// 4306 "parser.c"
        break;
      case 218:
// 1827 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_new_instance(yymsp[-3].minor.yy0, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(27,&yymsp[-4].minor);
  yy_destructor(45,&yymsp[-2].minor);
  yy_destructor(29,&yymsp[0].minor);
}
// 4316 "parser.c"
        break;
      case 219:
// 1832 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_fcall(yymsp[-3].minor.yy0, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(45,&yymsp[-2].minor);
  yy_destructor(29,&yymsp[0].minor);
}
// 4325 "parser.c"
        break;
      case 220:
// 1837 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_fcall(yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(45,&yymsp[-1].minor);
  yy_destructor(29,&yymsp[0].minor);
}
// 4334 "parser.c"
        break;
      case 223:
// 1852 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_scall(yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(81,&yymsp[-4].minor);
  yy_destructor(45,&yymsp[-2].minor);
  yy_destructor(29,&yymsp[0].minor);
}
// 4344 "parser.c"
        break;
      case 224:
// 1857 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_scall(yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(81,&yymsp[-3].minor);
  yy_destructor(45,&yymsp[-1].minor);
  yy_destructor(29,&yymsp[0].minor);
}
// 4354 "parser.c"
        break;
      case 225:
// 1862 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_mcall(yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(78,&yymsp[-4].minor);
  yy_destructor(45,&yymsp[-2].minor);
  yy_destructor(29,&yymsp[0].minor);
}
// 4364 "parser.c"
        break;
      case 226:
// 1867 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_mcall(yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(78,&yymsp[-3].minor);
  yy_destructor(45,&yymsp[-1].minor);
  yy_destructor(29,&yymsp[0].minor);
}
// 4374 "parser.c"
        break;
      case 232:
// 1891 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_array_item(yymsp[-2].minor.yy259, yymsp[0].minor.yy259, status->scanner_state);
  yy_destructor(64,&yymsp[-1].minor);
}
// 4382 "parser.c"
        break;
      case 233:
// 1895 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_array_item(NULL, yymsp[0].minor.yy259, status->scanner_state);
}
// 4389 "parser.c"
        break;
      case 245:
// 1944 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(79,&yymsp[-2].minor);
  yy_destructor(80,&yymsp[0].minor);
}
// 4398 "parser.c"
        break;
      case 246:
// 1949 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_INTEGER, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(79,&yymsp[-2].minor);
  yy_destructor(80,&yymsp[0].minor);
}
// 4407 "parser.c"
        break;
      case 247:
// 1954 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_STRING, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(79,&yymsp[-2].minor);
  yy_destructor(80,&yymsp[0].minor);
}
// 4416 "parser.c"
        break;
      case 249:
// 1962 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_comment(yymsp[0].minor.yy0, status->scanner_state);
}
// 4423 "parser.c"
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
// 841 "parser.lemon"


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

// 4499 "parser.c"
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
			/*case XX_T_IMPLEMENTS:
				xx_(xx_parser, XX_IMPLEMENTS, NULL, parser_status);
				break;*/
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
