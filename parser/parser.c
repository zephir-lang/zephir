/* Driver template for the LEMON parser generator.
** The author disclaims copyright to this source code.
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


// 789 "parser.c"
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
#define YYNOCODE 161
#define YYACTIONTYPE unsigned short int
#define xx_TOKENTYPE xx_parser_token*
typedef union {
  xx_TOKENTYPE yy0;
  json_object* yy95;
  int yy321;
} YYMINORTYPE;
#define YYSTACKDEPTH 100
#define xx_ARG_SDECL xx_parser_status *status;
#define xx_ARG_PDECL ,xx_parser_status *status
#define xx_ARG_FETCH xx_parser_status *status = yypParser->status
#define xx_ARG_STORE yypParser->status = status
#define YYNSTATE 501
#define YYNRULE 243
#define YYERRORSYMBOL 95
#define YYERRSYMDT yy321
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
 /*     0 */    96,  161,  165,  159,  197,   93,  348,  325,  154,  365,
 /*    10 */    20,   23,  473,  479,  478,  466,  495,  472,  446,  239,
 /*    20 */   167,  442,  137,   13,  138,  139,  140,  141,  142,  143,
 /*    30 */   144,  145,  146,  147,  148,  133,  135,   95,  109,  113,
 /*    40 */   115,  117,  119,  111,  107,  121,  123,  131,  125,  127,
 /*    50 */   129,  240,  241,  242,  243,  244,  175,  252,  121,  123,
 /*    60 */   131,  125,  127,  129,  168,  169,  353,  170,  171,  172,
 /*    70 */   173,  174,  289,  228,   34,  358,  157,  239,  285,  290,
 /*    80 */   204,  205,  317,  326,  335,  338,  329,  332,  341,  347,
 /*    90 */   344,  158,  350,   53,   55,   57,  417,   66,  429,  204,
 /*   100 */   205,   77,   81,   86,  226,  208,  207,  233,  220,  240,
 /*   110 */   241,  242,  243,  244,  353,  265,  125,  127,  129,  284,
 /*   120 */   295,  299,  387,  358,  314,    3,    4,    5,    6,    7,
 /*   130 */   317,  326,  335,  338,  329,  332,  341,  347,  344,  289,
 /*   140 */   350,   53,   55,   57,  323,   66,  288,  204,  205,   77,
 /*   150 */    81,   86,  226,  400,  474,  233,  477,  478,  466,  427,
 /*   160 */   445,  446,  353,   82,  442,  442,  225,  284,  295,  299,
 /*   170 */    85,  358,  314,  430,  204,  205,  395,  184,  317,  326,
 /*   180 */   335,  338,  329,  332,  341,  347,  344,  158,  350,   53,
 /*   190 */    55,   57,   19,   66,  485,  204,  205,   77,   81,   86,
 /*   200 */   226,    8,  206,  233,   11,  247,  480,  486,  493,  276,
 /*   210 */   353,  358,  432,  204,  205,  284,  295,  299,  399,  358,
 /*   220 */   314,   14,  180,  181,  192,   19,  317,  326,  335,  338,
 /*   230 */   329,  332,  341,  347,  344,  260,  350,   53,   55,   57,
 /*   240 */   277,   66,  302,  204,  205,   77,   81,   86,  226,  186,
 /*   250 */   187,  233,  260,  188,  189,  191,  190,  269,  353,  379,
 /*   260 */   204,  205,  225,  284,  295,  299,  382,  358,  314,  239,
 /*   270 */   204,  205,  234,  283,  317,  326,  335,  338,  329,  332,
 /*   280 */   341,  347,  344,  158,  350,   53,   55,   57,  151,   66,
 /*   290 */   498,  204,  205,   77,   81,   86,  226,  201,  207,  233,
 /*   300 */   128,  240,  241,  242,  243,  244,  353,  324,  204,  205,
 /*   310 */   149,  284,  295,  299,  397,  358,  314,   17,  204,  205,
 /*   320 */    70,  386,  317,  326,  335,  338,  329,  332,  341,  347,
 /*   330 */   344,  158,  350,   53,   55,   57,   12,   66,   16,  204,
 /*   340 */   205,   77,   81,   86,  226,  222,  207,  233,  475,  458,
 /*   350 */   466,   99,  184,  446,  353,   58,  442,  257,  225,  284,
 /*   360 */   295,  299,  392,  358,  314,  163,  204,  205,  301,  304,
 /*   370 */   317,  326,  335,  338,  329,  332,  341,  347,  344,  260,
 /*   380 */   350,   53,   55,   57,  256,   66,  322,  204,  205,   77,
 /*   390 */    81,   86,  226,  494,  177,  233,  247,  196,  181,  192,
 /*   400 */   246,  212,  353,   91,  204,  205,  225,  284,  295,  299,
 /*   410 */   393,  358,  314,  217,  204,  205,  438,  198,  317,  326,
 /*   420 */   335,  338,  329,  332,  341,  347,  344,  158,  350,   53,
 /*   430 */    55,   57,  487,   66,  396,  204,  205,   77,   81,   86,
 /*   440 */   226,  214,  207,  233,  444,   10,   26,  460,  445,  446,
 /*   450 */   353,   67,  442,  442,  225,  284,  295,  299,  364,  358,
 /*   460 */   314,  497,  204,  205,  287,   19,  317,  326,  335,  338,
 /*   470 */   329,  332,  341,  347,  344,  110,  350,   53,   55,   57,
 /*   480 */     9,   66,  258,  204,  205,   77,   81,   86,  226,  394,
 /*   490 */   286,  233,  416,  405,  409,  279,  436,  488,  353,  416,
 /*   500 */   405,  409,  300,  284,  295,  299,  376,  358,  314,  401,
 /*   510 */   405,  409,  236,  245,  317,  326,  335,  338,  329,  332,
 /*   520 */   341,  347,  344,   94,  350,   53,   55,   57,  162,   66,
 /*   530 */    56,  204,  205,   77,   81,   86,  226,  730,  235,  233,
 /*   540 */   247,  318,  325,  483,  272,  367,  353,   19,  204,  205,
 /*   550 */   122,  284,  295,  299,  377,  358,  314,  356,  204,  205,
 /*   560 */   354,   59,  317,  326,  335,  338,  329,  332,  341,  347,
 /*   570 */   344,  124,  350,   53,   55,   57,  489,   66,  213,  204,
 /*   580 */   205,   77,   81,   86,  226,  380,  203,  233,  247,  327,
 /*   590 */   325,  490,  280,  449,  353,   19,  204,  205,  120,  284,
 /*   600 */   295,  299,  388,  358,  314,  275,  204,  205,  320,  202,
 /*   610 */   317,  326,  335,  338,  329,  332,  341,  347,  344,  126,
 /*   620 */   350,   53,   55,   57,   83,   66,   62,  204,  205,   77,
 /*   630 */    81,   86,  226,  450,  319,  233,  247,  330,  325,  262,
 /*   640 */   268,  451,  353,  237,  204,  205,  100,  284,  295,  299,
 /*   650 */    89,  358,  314,  333,  325,  179,  336,  325,  317,  326,
 /*   660 */   335,  338,  329,  332,  341,  347,  344,  315,  350,   53,
 /*   670 */    55,   57,  452,   66,   18,  204,  205,   77,   81,   86,
 /*   680 */   226,   26,  203,  233,   25,  339,  325,  442,  342,  325,
 /*   690 */   353,  455,  462,  345,  325,  284,  295,  299,  381,  358,
 /*   700 */   314,  463,  102,  104,  454,  209,  317,  326,  335,  338,
 /*   710 */   329,  332,  341,  347,  344,  296,  350,   53,   55,   57,
 /*   720 */   378,   66,  227,  204,  205,   77,   81,   86,  226,  178,
 /*   730 */   400,  233,  263,  468,  351,  325,  184,  456,  353,  152,
 /*   740 */   457,  116,  469,  284,  295,  299,   61,  358,  314,  204,
 /*   750 */   205,  320,  320,  437,  317,  326,  335,  338,  329,  332,
 /*   760 */   341,  347,  344,  130,  350,   53,   55,   57,   63,   66,
 /*   770 */   238,  204,  205,   77,   81,   86,  226,  328,  331,  233,
 /*   780 */    28,   97,  491,  183,  278,   87,  353,  464,   31,  112,
 /*   790 */   305,  284,  295,  299,  441,  358,  314,  204,  205,  320,
 /*   800 */   320,   78,  317,  326,  335,  338,  329,  332,  341,  347,
 /*   810 */   344,  373,  350,   53,   55,   57,  465,   66,  390,  204,
 /*   820 */   205,   77,   81,   86,  226,  334,  337,  233,  267,  106,
 /*   830 */   389,   75,  101,  470,  353,   90,  729,  204,  205,  284,
 /*   840 */   295,  299,   32,  358,  314,  471,  308,  311,  320,  156,
 /*   850 */   317,  326,  335,  338,  329,  332,  341,  347,  344,  136,
 /*   860 */   350,   53,   55,   57,  164,   66,  103,  204,  205,   77,
 /*   870 */    81,   86,  226,   74,  340,  233,  270,   29,  294,  182,
 /*   880 */   496,   92,  353,   21,  355,  108,  292,  284,  295,  299,
 /*   890 */   439,  358,  314,  204,  205,  320,  501,  271,  317,  326,
 /*   900 */   335,  338,  329,  332,  341,  347,  344,  368,  350,   53,
 /*   910 */    55,   57,  728,   66,  481,  204,  205,   77,   81,   86,
 /*   920 */   226,  343,  203,  233,  384,  366,  482,  407,  357,  408,
 /*   930 */   353,  410,  309,  160,  411,  284,  295,  299,  371,  358,
 /*   940 */   314,  204,  205,  320,  320,  215,  317,  326,  335,  338,
 /*   950 */   329,  332,  341,  347,  344,  166,  350,   53,   55,   57,
 /*   960 */   499,   66,  412,  204,  205,   77,   81,   86,  226,  346,
 /*   970 */   349,  233,  310,  307,  414,   98,  415,  406,  353,  568,
 /*   980 */   404,  229,  312,  284,  295,  299,  435,  358,  314,  204,
 /*   990 */   205,  306,  320,  313,  317,  326,  335,  338,  329,  332,
 /*  1000 */   341,  347,  344,  361,  350,   53,   55,   57,  249,   66,
 /*  1010 */    72,  204,  205,   77,   81,   86,  226,   68,  352,  233,
 /*  1020 */   250,  403,   54,  303,  484,  105,  353,  492,  199,  134,
 /*  1030 */    15,  284,  295,  299,   80,  358,  314,  204,  205,  321,
 /*  1040 */   405,  405,  317,  326,  335,  338,  329,  332,  341,  347,
 /*  1050 */   344,  118,  350,   53,   55,   57,  254,   66,  405,  204,
 /*  1060 */   205,   77,   81,   86,  226,  405,  405,  233,  405,  405,
 /*  1070 */   405,  405,  405,  405,  353,  405,  405,  114,  405,  284,
 /*  1080 */   295,  299,  433,  358,  314,  204,  205,  405,  405,  405,
 /*  1090 */   317,  326,  335,  338,  329,  332,  341,  347,  344,   69,
 /*  1100 */   350,   53,   55,   57,  405,   66,  405,  405,  132,   77,
 /*  1110 */    81,   86,  226,  405,  405,  233,  204,  205,  405,  405,
 /*  1120 */   405,  405,  353,   73,  405,  383,  219,  284,  295,  299,
 /*  1130 */    65,  358,  314,  405,  204,  205,  405,  405,  317,  326,
 /*  1140 */   335,  338,  329,  332,  341,  347,  344,  153,  350,   53,
 /*  1150 */    55,   57,  405,   66,  405,  204,  205,   77,   81,   86,
 /*  1160 */   226,  405,  405,  233,  405,  405,  405,  405,  405,  405,
 /*  1170 */   353,  405,  405,  405,  405,  284,  295,  299,  232,  358,
 /*  1180 */   314,  418,  419,  421,  420,  422,  317,  326,  335,  338,
 /*  1190 */   329,  332,  341,  347,  344,  405,  350,   53,   55,   57,
 /*  1200 */   405,   66,  405,  405,  405,   77,   81,   86,  226,  405,
 /*  1210 */   405,  233,  405,  405,  405,  405,  424,  425,  353,   22,
 /*  1220 */   459,  405,  453,  284,  295,  299,  423,  358,  314,  221,
 /*  1230 */   405,  405,  405,  405,  317,  326,  335,  338,  329,  332,
 /*  1240 */   341,  347,  344,  203,  350,   53,   55,   57,  405,   66,
 /*  1250 */   405,   71,  405,   77,   81,   86,  226,  405,  405,  233,
 /*  1260 */   405,   52,  155,  218,  405,  211,  223,  405,  405,  405,
 /*  1270 */   405,  284,  295,  299,  363,   73,  314,  383,  405,  359,
 /*  1280 */    36,   37,   38,   39,   40,   41,   42,   43,   44,   45,
 /*  1290 */    46,   47,   48,   49,   50,   51,   52,  405,  745,    1,
 /*  1300 */     2,  500,    4,    5,    6,    7,  405,  291,  293,   79,
 /*  1310 */   405,  185,  405,  405,  359,   36,   37,   38,   39,   40,
 /*  1320 */    41,   42,   43,   44,   45,   46,   47,   48,   49,   50,
 /*  1330 */    51,   52,  443,   24,  458,  466,  405,  472,  446,  405,
 /*  1340 */   405,  442,  291,  293,   88,  405,  405,  405,  405,  359,
 /*  1350 */    36,   37,   38,   39,   40,   41,   42,   43,   44,   45,
 /*  1360 */    46,   47,   48,   49,   50,   51,  405,  186,  187,  405,
 /*  1370 */   405,  188,  189,  191,  190,   52,  405,  291,  293,  405,
 /*  1380 */   418,  419,  421,  420,  422,  405,  405,  405,   76,  405,
 /*  1390 */   405,  193,  405,  359,   36,   37,   38,   39,   40,   41,
 /*  1400 */    42,   43,   44,   45,   46,   47,   48,   49,   50,   51,
 /*  1410 */   405,   52,  405,  405,  405,  424,  425,  405,  526,  476,
 /*  1420 */   405,  291,  293,  405,  440,  423,  405,  405,  405,  359,
 /*  1430 */    36,   37,   38,   39,   40,   41,   42,   43,   44,   45,
 /*  1440 */    46,   47,   48,   49,   50,   51,   52,  195,  194,  405,
 /*  1450 */   405,  188,  189,  191,  190,  405,  405,  291,  293,  434,
 /*  1460 */   405,  405,  405,  405,  359,   36,   37,   38,   39,   40,
 /*  1470 */    41,   42,   43,   44,   45,   46,   47,   48,   49,   50,
 /*  1480 */    51,   52,  405,  405,  405,  405,  405,  405,  405,  405,
 /*  1490 */   405,  405,  291,  293,   64,  405,  405,  405,  405,  359,
 /*  1500 */    36,   37,   38,   39,   40,   41,   42,   43,   44,   45,
 /*  1510 */    46,   47,   48,   49,   50,   51,  405,  405,  405,  405,
 /*  1520 */   405,  405,  405,  405,  405,   52,  405,  291,  293,  405,
 /*  1530 */   418,  419,  421,  420,  422,  405,  405,  405,  398,  405,
 /*  1540 */   405,  405,  405,  359,   36,   37,   38,   39,   40,   41,
 /*  1550 */    42,   43,   44,   45,   46,   47,   48,   49,   50,   51,
 /*  1560 */   405,   52,  405,  405,  405,  424,  425,  405,  529,  426,
 /*  1570 */   405,  291,  293,  405,  391,  423,  405,  405,  405,  359,
 /*  1580 */    36,   37,   38,   39,   40,   41,   42,   43,   44,   45,
 /*  1590 */    46,   47,   48,   49,   50,   51,   52,  405,  405,  405,
 /*  1600 */   405,  405,  405,  405,  405,  405,  405,  291,  293,  385,
 /*  1610 */   405,  405,  405,  405,  359,   36,   37,   38,   39,   40,
 /*  1620 */    41,   42,   43,   44,   45,   46,   47,   48,   49,   50,
 /*  1630 */    51,   52,  405,  405,  405,  405,  405,  405,  405,  405,
 /*  1640 */   405,  405,  291,  293,   60,  405,  405,  405,  405,  359,
 /*  1650 */    36,   37,   38,   39,   40,   41,   42,   43,   44,   45,
 /*  1660 */    46,   47,   48,   49,   50,   51,  405,  405,  405,  405,
 /*  1670 */   405,  405,  405,  405,  405,   52,  405,  291,  293,  405,
 /*  1680 */   418,  419,  421,  420,  422,  405,  405,  405,  375,  405,
 /*  1690 */   405,  405,  405,  359,   36,   37,   38,   39,   40,   41,
 /*  1700 */    42,   43,   44,   45,   46,   47,   48,   49,   50,   51,
 /*  1710 */   405,   52,  405,  405,  405,  424,  425,  405,  528,  426,
 /*  1720 */   405,  291,  293,  405,   84,  423,  405,  405,  405,  359,
 /*  1730 */    36,   37,   38,   39,   40,   41,   42,   43,   44,   45,
 /*  1740 */    46,   47,   48,   49,   50,   51,   52,  405,  405,  405,
 /*  1750 */   405,  405,  405,  405,  405,  405,  405,  291,  293,  231,
 /*  1760 */   405,  405,  405,  405,  359,   36,   37,   38,   39,   40,
 /*  1770 */    41,   42,   43,   44,   45,   46,   47,   48,   49,   50,
 /*  1780 */    51,   52,  405,  405,  405,  405,  405,  405,  405,  405,
 /*  1790 */   405,  405,  291,  293,  370,  405,  405,  405,  405,  359,
 /*  1800 */    36,   37,   38,   39,   40,   41,   42,   43,   44,   45,
 /*  1810 */    46,   47,   48,   49,   50,   51,  405,  405,  405,  405,
 /*  1820 */   405,  405,  405,  405,  405,   52,  405,  291,  293,  418,
 /*  1830 */   419,  421,  420,  422,  405,  405,  405,  405,   33,  405,
 /*  1840 */   405,  405,  405,  359,   36,   37,   38,   39,   40,   41,
 /*  1850 */    42,   43,   44,   45,   46,   47,   48,   49,   50,   51,
 /*  1860 */   405,   52,  405,  405,  424,  425,  405,  525,  447,  405,
 /*  1870 */   453,  291,  293,  405,  423,  405,  405,  405,  405,   35,
 /*  1880 */    36,   37,   38,   39,   40,   41,   42,   43,   44,   45,
 /*  1890 */    46,   47,   48,   49,   50,   51,   96,  161,  165,  159,
 /*  1900 */   197,   93,  224,  405,  154,  405,  405,  291,  293,  418,
 /*  1910 */   419,  421,  420,  422,  405,  405,  167,  405,  137,  405,
 /*  1920 */    96,  161,  165,  159,  197,   93,  405,  405,  154,  405,
 /*  1930 */   405,  405,  405,  405,  405,  405,  405,  239,  405,  405,
 /*  1940 */   167,  405,  137,  405,  424,  425,  405,  521,  459,  247,
 /*  1950 */   453,  405,  175,  264,  423,  405,  405,  204,  205,  405,
 /*  1960 */   168,  169,  405,  170,  171,  172,  173,  174,  372,  240,
 /*  1970 */   241,  242,  243,  244,  248,  273,  175,  261,  281,  282,
 /*  1980 */   405,  405,  193,  405,  168,  169,  405,  170,  171,  172,
 /*  1990 */   173,  174,   96,  161,  165,  159,  197,   93,  247,  405,
 /*  2000 */   154,  298,  259,  405,  405,  405,  204,  205,  405,  405,
 /*  2010 */   405,  405,  167,  405,  137,   96,  161,  165,  159,  197,
 /*  2020 */    93,  247,  405,  154,  247,  255,  405,  405,  251,  204,
 /*  2030 */   205,  176,  204,  205,  405,  167,  405,  137,  195,  194,
 /*  2040 */   405,  405,  188,  189,  191,  190,  405,  405,  175,  405,
 /*  2050 */   405,  405,  405,  405,  405,  405,  168,  169,  405,  170,
 /*  2060 */   171,  172,  173,  174,  405,  405,  405,  405,  405,  405,
 /*  2070 */   405,  175,  274,  405,  405,  405,  405,  405,  405,  168,
 /*  2080 */   169,  405,  170,  171,  172,  173,  174,   96,  161,  165,
 /*  2090 */   159,  197,   93,  405,  405,  154,  405,  405,  405,  405,
 /*  2100 */   405,  405,  405,  405,  405,  405,  405,  167,  405,  137,
 /*  2110 */    96,  161,  165,  159,  197,   93,  405,  405,  154,  405,
 /*  2120 */   405,  405,  405,  405,  405,  405,  405,  405,  405,  405,
 /*  2130 */   167,  405,  137,  405,  405,  405,  405,  405,  405,  405,
 /*  2140 */   405,  405,  405,  175,  266,  405,  405,  405,  405,  405,
 /*  2150 */   405,  168,  169,  405,  170,  171,  172,  173,  174,  405,
 /*  2160 */   405,  405,  405,  405,  405,  405,  175,  253,   96,  161,
 /*  2170 */   165,  159,  197,   93,  168,  169,  154,  170,  171,  172,
 /*  2180 */   173,  174,  418,  419,  421,  420,  422,  405,  167,  405,
 /*  2190 */   137,   96,  161,  165,  159,  197,   93,  216,  405,  154,
 /*  2200 */   405,   96,  161,  165,  159,  197,   93,  200,  405,  154,
 /*  2210 */   405,  167,  461,  137,  405,  405,  360,  424,  425,  405,
 /*  2220 */   405,  167,  405,  137,  175,  428,  405,  423,  405,  405,
 /*  2230 */   405,  405,  168,  169,  405,  170,  171,  172,  173,  174,
 /*  2240 */   405,  418,  419,  421,  420,  422,  405,  175,  405,  405,
 /*  2250 */   405,  405,  405,  405,  405,  168,  169,  175,  170,  171,
 /*  2260 */   172,  173,  174,  405,  405,  168,  169,  405,  170,  171,
 /*  2270 */   172,  173,  174,  405,  405,  405,  424,  425,  405,   96,
 /*  2280 */   161,  165,  159,  197,   93,  210,  423,  154,  405,  418,
 /*  2290 */   419,  421,  420,  422,  405,  405,  405,  405,  405,  167,
 /*  2300 */   405,  137,  405,  405,  405,  405,  405,  133,  135,   95,
 /*  2310 */   109,  113,  115,  117,  119,  111,  107,  121,  123,  131,
 /*  2320 */   125,  127,  129,  405,  424,  425,  405,  522,  459,  405,
 /*  2330 */   453,  405,  413,  405,  423,  175,  405,  405,  374,  405,
 /*  2340 */   405,  405,  405,  168,  169,  405,  170,  171,  172,  173,
 /*  2350 */   174,  405,  402,  405,  405,  405,  405,  405,  405,   96,
 /*  2360 */   161,  165,  159,  197,   93,  405,  405,  154,  138,  139,
 /*  2370 */   140,  141,  142,  143,  144,  145,  146,  147,  148,  167,
 /*  2380 */   405,  137,  405,  405,  405,  405,  405,  133,  135,   95,
 /*  2390 */   109,  113,  115,  117,  119,  111,  107,  121,  123,  131,
 /*  2400 */   125,  127,  129,  405,  405,  405,  405,  405,  405,  405,
 /*  2410 */   405,  405,  405,  405,  405,  175,  405,  405,  369,  405,
 /*  2420 */   405,  405,  405,  168,  169,  405,  170,  171,  172,  173,
 /*  2430 */   174,  133,  135,   95,  109,  113,  115,  117,  119,  111,
 /*  2440 */   107,  121,  123,  131,  125,  127,  129,   95,  109,  113,
 /*  2450 */   115,  117,  119,  111,  107,  121,  123,  131,  125,  127,
 /*  2460 */   129,  405,  230,  405,  405,  405,  405,  405,  405,  405,
 /*  2470 */   133,  135,   95,  109,  113,  115,  117,  119,  111,  107,
 /*  2480 */   121,  123,  131,  125,  127,  129,  418,  419,  421,  420,
 /*  2490 */   422,  405,  150,  133,  135,   95,  109,  113,  115,  117,
 /*  2500 */   119,  111,  107,  121,  123,  131,  125,  127,  129,  405,
 /*  2510 */   405,  405,  405,  405,  405,  405,  467,  405,  405,  405,
 /*  2520 */   405,  424,  425,  502,  362,  405,  405,  405,  405,   27,
 /*  2530 */   405,  423,  133,  135,   95,  109,  113,  115,  117,  119,
 /*  2540 */   111,  107,  121,  123,  131,  125,  127,  129,  418,  419,
 /*  2550 */   421,  420,  422,    8,  405,  405,   11,  316,  480,  486,
 /*  2560 */   493,  405,  405,  358,  405,  133,  135,   95,  109,  113,
 /*  2570 */   115,  117,  119,  111,  107,  121,  123,  131,  125,  127,
 /*  2580 */   129,  405,  405,  424,  425,  405,  524,  426,  405,  405,
 /*  2590 */   297,  405,  405,  423,  405,  405,  405,  405,  133,  135,
 /*  2600 */    95,  109,  113,  115,  117,  119,  111,  107,  121,  123,
 /*  2610 */   131,  125,  127,  129,  133,  135,   95,  109,  113,  115,
 /*  2620 */   117,  119,  111,  107,  121,  123,  131,  125,  127,  129,
 /*  2630 */   405,  405,  405,  405,  405,  405,  413,  405,  405,  405,
 /*  2640 */   405,  405,  405,  405,  405,  405,  405,  405,  413,  405,
 /*  2650 */   405,  405,  405,  405,  431,  405,  402,  405,  405,  405,
 /*  2660 */   405,  405,  405,  405,  405,  405,   30,  405,  402,  405,
 /*  2670 */   405,  643,  138,  139,  140,  141,  142,  143,  144,  145,
 /*  2680 */   146,  147,  148,  405,  138,  139,  140,  141,  142,  143,
 /*  2690 */   144,  145,  146,  147,  148,  418,  419,  421,  420,  422,
 /*  2700 */   418,  419,  421,  420,  422,  405,  418,  419,  421,  420,
 /*  2710 */   422,  405,  405,  405,  405,  405,  405,  405,  405,  405,
 /*  2720 */   405,  405,  405,  405,  405,  405,  405,  405,  405,  405,
 /*  2730 */   424,  425,  405,  527,  426,  424,  425,  405,  523,  426,
 /*  2740 */   423,  424,  425,  405,  405,  423,  405,  448,  405,  405,
 /*  2750 */   405,  423,  405,  418,  419,  421,  420,  422,  418,  419,
 /*  2760 */   421,  420,  422,  405,  405,  405,  405,  405,  405,  405,
 /*  2770 */   405,  405,  405,  405,  405,  405,  405,  405,  405,  405,
 /*  2780 */   405,  405,  405,  405,  405,  405,  405,  405,  424,  425,
 /*  2790 */   405,  405,  405,  424,  425,  405,   27,  405,  423,  405,
 /*  2800 */   405,  428,  405,  423,
};
static YYCODETYPE yy_lookahead[] = {
 /*     0 */    23,   24,   25,   26,   27,   28,  151,  152,   31,    6,
 /*    10 */   106,  107,  108,  109,  110,  111,   31,  113,  114,   41,
 /*    20 */    43,  117,   45,  104,   47,   48,   49,   50,   51,   52,
 /*    30 */    53,   54,   55,   56,   57,    7,    8,    9,   10,   11,
 /*    40 */    12,   13,   14,   15,   16,   17,   18,   19,   20,   21,
 /*    50 */    22,   73,   74,   75,   76,   77,   79,   79,   17,   18,
 /*    60 */    19,   20,   21,   22,   87,   88,   31,   90,   91,   92,
 /*    70 */    93,   94,  141,   70,   39,   40,   45,   41,  147,  148,
 /*    80 */   149,  150,   47,   48,   49,   50,   51,   52,   53,   54,
 /*    90 */    55,  141,   57,   58,   59,   60,  117,   62,   31,  149,
 /*   100 */   150,   66,   67,   68,   69,  155,  156,   72,   80,   73,
 /*   110 */    74,   75,   76,   77,   31,   79,   20,   21,   22,   84,
 /*   120 */    85,   86,   39,   40,   89,   99,  100,  101,  102,  103,
 /*   130 */    47,   48,   49,   50,   51,   52,   53,   54,   55,  141,
 /*   140 */    57,   58,   59,   60,   41,   62,  148,  149,  150,   66,
 /*   150 */    67,   68,   69,    6,  107,   72,  109,  110,  111,  111,
 /*   160 */   113,  114,   31,  138,  117,  117,  141,   84,   85,   86,
 /*   170 */    39,   40,   89,   45,  149,  150,   29,  112,   47,   48,
 /*   180 */    49,   50,   51,   52,   53,   54,   55,  141,   57,   58,
 /*   190 */    59,   60,   38,   62,  104,  149,  150,   66,   67,   68,
 /*   200 */    69,   30,  156,   72,   33,  141,   35,   36,   37,  145,
 /*   210 */    31,   40,   38,  149,  150,   84,   85,   86,   39,   40,
 /*   220 */    89,   34,  157,  158,  159,   38,   47,   48,   49,   50,
 /*   230 */    51,   52,   53,   54,   55,  141,   57,   58,   59,   60,
 /*   240 */   146,   62,   31,  149,  150,   66,   67,   68,   69,   87,
 /*   250 */    88,   72,  141,   91,   92,   93,   94,  146,   31,  138,
 /*   260 */   149,  150,  141,   84,   85,   86,   39,   40,   89,   41,
 /*   270 */   149,  150,  142,  143,   47,   48,   49,   50,   51,   52,
 /*   280 */    53,   54,   55,  141,   57,   58,   59,   60,  119,   62,
 /*   290 */    31,  149,  150,   66,   67,   68,   69,  155,  156,   72,
 /*   300 */   141,   73,   74,   75,   76,   77,   31,  112,  149,  150,
 /*   310 */   141,   84,   85,   86,   39,   40,   89,   38,  149,  150,
 /*   320 */   139,  140,   47,   48,   49,   50,   51,   52,   53,   54,
 /*   330 */    55,  141,   57,   58,   59,   60,   31,   62,  105,  149,
 /*   340 */   150,   66,   67,   68,   69,  155,  156,   72,  109,  110,
 /*   350 */   111,   79,  112,  114,   31,  138,  117,   80,  141,   84,
 /*   360 */    85,   86,   39,   40,   89,    6,  149,  150,   78,   79,
 /*   370 */    47,   48,   49,   50,   51,   52,   53,   54,   55,  141,
 /*   380 */    57,   58,   59,   60,  146,   62,   31,  149,  150,   66,
 /*   390 */    67,   68,   69,   35,  154,   72,  141,  157,  158,  159,
 /*   400 */   145,   31,   31,  138,  149,  150,  141,   84,   85,   86,
 /*   410 */    39,   40,   89,   43,  149,  150,   38,   31,   47,   48,
 /*   420 */    49,   50,   51,   52,   53,   54,   55,  141,   57,   58,
 /*   430 */    59,   60,   35,   62,   38,  149,  150,   66,   67,   68,
 /*   440 */    69,  155,  156,   72,  109,   32,  111,  111,  113,  114,
 /*   450 */    31,  138,  117,  117,  141,   84,   85,   86,   39,   40,
 /*   460 */    89,   34,  149,  150,    6,   38,   47,   48,   49,   50,
 /*   470 */    51,   52,   53,   54,   55,  141,   57,   58,   59,   60,
 /*   480 */    31,   62,  144,  149,  150,   66,   67,   68,   69,  115,
 /*   490 */    32,   72,  118,  119,  120,  144,  115,   31,   31,  118,
 /*   500 */   119,  120,   31,   84,   85,   86,   39,   40,   89,  118,
 /*   510 */   119,  120,    6,  144,   47,   48,   49,   50,   51,   52,
 /*   520 */    53,   54,   55,  141,   57,   58,   59,   60,   31,   62,
 /*   530 */    32,  149,  150,   66,   67,   68,   69,   64,   32,   72,
 /*   540 */   141,  151,  152,   34,  145,   70,   31,   38,  149,  150,
 /*   550 */   141,   84,   85,   86,   39,   40,   89,   78,  149,  150,
 /*   560 */    81,   38,   47,   48,   49,   50,   51,   52,   53,   54,
 /*   570 */    55,  141,   57,   58,   59,   60,  104,   62,   45,  149,
 /*   580 */   150,   66,   67,   68,   69,   32,    6,   72,  141,  151,
 /*   590 */   152,   34,  145,   43,   31,   38,  149,  150,  141,   84,
 /*   600 */    85,   86,   39,   40,   89,  144,  149,  150,    6,   29,
 /*   610 */    47,   48,   49,   50,   51,   52,   53,   54,   55,  141,
 /*   620 */    57,   58,   59,   60,   38,   62,   61,  149,  150,   66,
 /*   630 */    67,   68,   69,   41,   32,   72,  141,  151,  152,   31,
 /*   640 */   145,  112,   31,  143,  149,  150,   31,   84,   85,   86,
 /*   650 */    39,   40,   89,  151,  152,    6,  151,  152,   47,   48,
 /*   660 */    49,   50,   51,   52,   53,   54,   55,  141,   57,   58,
 /*   670 */    59,   60,   32,   62,   39,  149,  150,   66,   67,   68,
 /*   680 */    69,  111,    6,   72,  114,  151,  152,  117,  151,  152,
 /*   690 */    31,   41,   32,  151,  152,   84,   85,   86,   39,   40,
 /*   700 */    89,   41,   87,   88,   43,   29,   47,   48,   49,   50,
 /*   710 */    51,   52,   53,   54,   55,  141,   57,   58,   59,   60,
 /*   720 */    67,   62,   31,  149,  150,   66,   67,   68,   69,   80,
 /*   730 */     6,   72,  144,   32,  151,  152,  112,  112,   31,   29,
 /*   740 */    32,  141,   41,   84,   85,   86,   39,   40,   89,  149,
 /*   750 */   150,    6,    6,   29,   47,   48,   49,   50,   51,   52,
 /*   760 */    53,   54,   55,  141,   57,   58,   59,   60,   38,   62,
 /*   770 */    31,  149,  150,   66,   67,   68,   69,   32,   32,   72,
 /*   780 */    31,  153,   31,  159,   80,   38,   31,  112,   38,  141,
 /*   790 */    31,   84,   85,   86,   39,   40,   89,  149,  150,    6,
 /*   800 */     6,   38,   47,   48,   49,   50,   51,   52,   53,   54,
 /*   810 */    55,  141,   57,   58,   59,   60,   32,   62,   38,  149,
 /*   820 */   150,   66,   67,   68,   69,   32,   32,   72,  144,  141,
 /*   830 */    61,   64,   80,  112,   31,   67,   64,  149,  150,   84,
 /*   840 */    85,   86,   39,   40,   89,   32,   87,   88,    6,   31,
 /*   850 */    47,   48,   49,   50,   51,   52,   53,   54,   55,  141,
 /*   860 */    57,   58,   59,   60,  153,   62,   80,  149,  150,   66,
 /*   870 */    67,   68,   69,  112,   32,   72,   80,   45,   32,   64,
 /*   880 */   104,   32,   31,   39,   31,  141,   32,   84,   85,   86,
 /*   890 */    39,   40,   89,  149,  150,    6,    0,  144,   47,   48,
 /*   900 */    49,   50,   51,   52,   53,   54,   55,  141,   57,   58,
 /*   910 */    59,   60,   64,   62,   31,  149,  150,   66,   67,   68,
 /*   920 */    69,   32,    6,   72,   64,   31,  104,   41,   31,  112,
 /*   930 */    31,   31,   80,  141,   41,   84,   85,   86,   39,   40,
 /*   940 */    89,  149,  150,    6,    6,   29,   47,   48,   49,   50,
 /*   950 */    51,   52,   53,   54,   55,  141,   57,   58,   59,   60,
 /*   960 */   104,   62,  112,  149,  150,   66,   67,   68,   69,   32,
 /*   970 */    32,   72,   32,   32,   31,   31,   12,   31,   31,   31,
 /*   980 */   112,  141,   80,   84,   85,   86,   39,   40,   89,  149,
 /*   990 */   150,   80,    6,   32,   47,   48,   49,   50,   51,   52,
 /*  1000 */    53,   54,   55,  141,   57,   58,   59,   60,   31,   62,
 /*  1010 */   140,  149,  150,   66,   67,   68,   69,   38,   32,   72,
 /*  1020 */   144,   41,   32,   32,   31,   80,   31,  104,   45,  141,
 /*  1030 */    31,   84,   85,   86,   39,   40,   89,  149,  150,  152,
 /*  1040 */   160,  160,   47,   48,   49,   50,   51,   52,   53,   54,
 /*  1050 */    55,  141,   57,   58,   59,   60,  144,   62,  160,  149,
 /*  1060 */   150,   66,   67,   68,   69,  160,  160,   72,  160,  160,
 /*  1070 */   160,  160,  160,  160,   31,  160,  160,  141,  160,   84,
 /*  1080 */    85,   86,   39,   40,   89,  149,  150,  160,  160,  160,
 /*  1090 */    47,   48,   49,   50,   51,   52,   53,   54,   55,   39,
 /*  1100 */    57,   58,   59,   60,  160,   62,  160,  160,  141,   66,
 /*  1110 */    67,   68,   69,  160,  160,   72,  149,  150,  160,  160,
 /*  1120 */   160,  160,   31,   63,  160,   65,  141,   84,   85,   86,
 /*  1130 */    39,   40,   89,  160,  149,  150,  160,  160,   47,   48,
 /*  1140 */    49,   50,   51,   52,   53,   54,   55,  141,   57,   58,
 /*  1150 */    59,   60,  160,   62,  160,  149,  150,   66,   67,   68,
 /*  1160 */    69,  160,  160,   72,  160,  160,  160,  160,  160,  160,
 /*  1170 */    31,  160,  160,  160,  160,   84,   85,   86,   39,   40,
 /*  1180 */    89,    1,    2,    3,    4,    5,   47,   48,   49,   50,
 /*  1190 */    51,   52,   53,   54,   55,  160,   57,   58,   59,   60,
 /*  1200 */   160,   62,  160,  160,  160,   66,   67,   68,   69,  160,
 /*  1210 */   160,   72,  160,  160,  160,  160,   36,   37,   31,   39,
 /*  1220 */    40,  160,   42,   84,   85,   86,   46,   40,   89,   45,
 /*  1230 */   160,  160,  160,  160,   47,   48,   49,   50,   51,   52,
 /*  1240 */    53,   54,   55,    6,   57,   58,   59,   60,  160,   62,
 /*  1250 */   160,   39,  160,   66,   67,   68,   69,  160,  160,   72,
 /*  1260 */   160,  103,   78,   79,  160,   81,   29,  160,  160,  160,
 /*  1270 */   160,   84,   85,   86,  116,   63,   89,   65,  160,  121,
 /*  1280 */   122,  123,  124,  125,  126,  127,  128,  129,  130,  131,
 /*  1290 */   132,  133,  134,  135,  136,  137,  103,  160,   96,   97,
 /*  1300 */    98,   99,  100,  101,  102,  103,  160,  149,  150,  116,
 /*  1310 */   160,   31,  160,  160,  121,  122,  123,  124,  125,  126,
 /*  1320 */   127,  128,  129,  130,  131,  132,  133,  134,  135,  136,
 /*  1330 */   137,  103,  108,  109,  110,  111,  160,  113,  114,  160,
 /*  1340 */   160,  117,  149,  150,  116,  160,  160,  160,  160,  121,
 /*  1350 */   122,  123,  124,  125,  126,  127,  128,  129,  130,  131,
 /*  1360 */   132,  133,  134,  135,  136,  137,  160,   87,   88,  160,
 /*  1370 */   160,   91,   92,   93,   94,  103,  160,  149,  150,  160,
 /*  1380 */     1,    2,    3,    4,    5,  160,  160,  160,  116,  160,
 /*  1390 */   160,   31,  160,  121,  122,  123,  124,  125,  126,  127,
 /*  1400 */   128,  129,  130,  131,  132,  133,  134,  135,  136,  137,
 /*  1410 */   160,  103,  160,  160,  160,   36,   37,  160,   39,   40,
 /*  1420 */   160,  149,  150,  160,  116,   46,  160,  160,  160,  121,
 /*  1430 */   122,  123,  124,  125,  126,  127,  128,  129,  130,  131,
 /*  1440 */   132,  133,  134,  135,  136,  137,  103,   87,   88,  160,
 /*  1450 */   160,   91,   92,   93,   94,  160,  160,  149,  150,  116,
 /*  1460 */   160,  160,  160,  160,  121,  122,  123,  124,  125,  126,
 /*  1470 */   127,  128,  129,  130,  131,  132,  133,  134,  135,  136,
 /*  1480 */   137,  103,  160,  160,  160,  160,  160,  160,  160,  160,
 /*  1490 */   160,  160,  149,  150,  116,  160,  160,  160,  160,  121,
 /*  1500 */   122,  123,  124,  125,  126,  127,  128,  129,  130,  131,
 /*  1510 */   132,  133,  134,  135,  136,  137,  160,  160,  160,  160,
 /*  1520 */   160,  160,  160,  160,  160,  103,  160,  149,  150,  160,
 /*  1530 */     1,    2,    3,    4,    5,  160,  160,  160,  116,  160,
 /*  1540 */   160,  160,  160,  121,  122,  123,  124,  125,  126,  127,
 /*  1550 */   128,  129,  130,  131,  132,  133,  134,  135,  136,  137,
 /*  1560 */   160,  103,  160,  160,  160,   36,   37,  160,   39,   40,
 /*  1570 */   160,  149,  150,  160,  116,   46,  160,  160,  160,  121,
 /*  1580 */   122,  123,  124,  125,  126,  127,  128,  129,  130,  131,
 /*  1590 */   132,  133,  134,  135,  136,  137,  103,  160,  160,  160,
 /*  1600 */   160,  160,  160,  160,  160,  160,  160,  149,  150,  116,
 /*  1610 */   160,  160,  160,  160,  121,  122,  123,  124,  125,  126,
 /*  1620 */   127,  128,  129,  130,  131,  132,  133,  134,  135,  136,
 /*  1630 */   137,  103,  160,  160,  160,  160,  160,  160,  160,  160,
 /*  1640 */   160,  160,  149,  150,  116,  160,  160,  160,  160,  121,
 /*  1650 */   122,  123,  124,  125,  126,  127,  128,  129,  130,  131,
 /*  1660 */   132,  133,  134,  135,  136,  137,  160,  160,  160,  160,
 /*  1670 */   160,  160,  160,  160,  160,  103,  160,  149,  150,  160,
 /*  1680 */     1,    2,    3,    4,    5,  160,  160,  160,  116,  160,
 /*  1690 */   160,  160,  160,  121,  122,  123,  124,  125,  126,  127,
 /*  1700 */   128,  129,  130,  131,  132,  133,  134,  135,  136,  137,
 /*  1710 */   160,  103,  160,  160,  160,   36,   37,  160,   39,   40,
 /*  1720 */   160,  149,  150,  160,  116,   46,  160,  160,  160,  121,
 /*  1730 */   122,  123,  124,  125,  126,  127,  128,  129,  130,  131,
 /*  1740 */   132,  133,  134,  135,  136,  137,  103,  160,  160,  160,
 /*  1750 */   160,  160,  160,  160,  160,  160,  160,  149,  150,  116,
 /*  1760 */   160,  160,  160,  160,  121,  122,  123,  124,  125,  126,
 /*  1770 */   127,  128,  129,  130,  131,  132,  133,  134,  135,  136,
 /*  1780 */   137,  103,  160,  160,  160,  160,  160,  160,  160,  160,
 /*  1790 */   160,  160,  149,  150,  116,  160,  160,  160,  160,  121,
 /*  1800 */   122,  123,  124,  125,  126,  127,  128,  129,  130,  131,
 /*  1810 */   132,  133,  134,  135,  136,  137,  160,  160,  160,  160,
 /*  1820 */   160,  160,  160,  160,  160,  103,  160,  149,  150,    1,
 /*  1830 */     2,    3,    4,    5,  160,  160,  160,  160,  116,  160,
 /*  1840 */   160,  160,  160,  121,  122,  123,  124,  125,  126,  127,
 /*  1850 */   128,  129,  130,  131,  132,  133,  134,  135,  136,  137,
 /*  1860 */   160,  103,  160,  160,   36,   37,  160,   39,   40,  160,
 /*  1870 */    42,  149,  150,  160,   46,  160,  160,  160,  160,  121,
 /*  1880 */   122,  123,  124,  125,  126,  127,  128,  129,  130,  131,
 /*  1890 */   132,  133,  134,  135,  136,  137,   23,   24,   25,   26,
 /*  1900 */    27,   28,   29,  160,   31,  160,  160,  149,  150,    1,
 /*  1910 */     2,    3,    4,    5,  160,  160,   43,  160,   45,  160,
 /*  1920 */    23,   24,   25,   26,   27,   28,  160,  160,   31,  160,
 /*  1930 */   160,  160,  160,  160,  160,  160,  160,   41,  160,  160,
 /*  1940 */    43,  160,   45,  160,   36,   37,  160,   39,   40,  141,
 /*  1950 */    42,  160,   79,  145,   46,  160,  160,  149,  150,  160,
 /*  1960 */    87,   88,  160,   90,   91,   92,   93,   94,   71,   73,
 /*  1970 */    74,   75,   76,   77,   78,   79,   79,   81,   82,   83,
 /*  1980 */   160,  160,   31,  160,   87,   88,  160,   90,   91,   92,
 /*  1990 */    93,   94,   23,   24,   25,   26,   27,   28,  141,  160,
 /*  2000 */    31,   32,  145,  160,  160,  160,  149,  150,  160,  160,
 /*  2010 */   160,  160,   43,  160,   45,   23,   24,   25,   26,   27,
 /*  2020 */    28,  141,  160,   31,  141,  145,  160,  160,  145,  149,
 /*  2030 */   150,   80,  149,  150,  160,   43,  160,   45,   87,   88,
 /*  2040 */   160,  160,   91,   92,   93,   94,  160,  160,   79,  160,
 /*  2050 */   160,  160,  160,  160,  160,  160,   87,   88,  160,   90,
 /*  2060 */    91,   92,   93,   94,  160,  160,  160,  160,  160,  160,
 /*  2070 */   160,   79,   80,  160,  160,  160,  160,  160,  160,   87,
 /*  2080 */    88,  160,   90,   91,   92,   93,   94,   23,   24,   25,
 /*  2090 */    26,   27,   28,  160,  160,   31,  160,  160,  160,  160,
 /*  2100 */   160,  160,  160,  160,  160,  160,  160,   43,  160,   45,
 /*  2110 */    23,   24,   25,   26,   27,   28,  160,  160,   31,  160,
 /*  2120 */   160,  160,  160,  160,  160,  160,  160,  160,  160,  160,
 /*  2130 */    43,  160,   45,  160,  160,  160,  160,  160,  160,  160,
 /*  2140 */   160,  160,  160,   79,   80,  160,  160,  160,  160,  160,
 /*  2150 */   160,   87,   88,  160,   90,   91,   92,   93,   94,  160,
 /*  2160 */   160,  160,  160,  160,  160,  160,   79,   80,   23,   24,
 /*  2170 */    25,   26,   27,   28,   87,   88,   31,   90,   91,   92,
 /*  2180 */    93,   94,    1,    2,    3,    4,    5,  160,   43,  160,
 /*  2190 */    45,   23,   24,   25,   26,   27,   28,   29,  160,   31,
 /*  2200 */   160,   23,   24,   25,   26,   27,   28,   29,  160,   31,
 /*  2210 */   160,   43,   31,   45,  160,  160,   71,   36,   37,  160,
 /*  2220 */   160,   43,  160,   45,   79,   44,  160,   46,  160,  160,
 /*  2230 */   160,  160,   87,   88,  160,   90,   91,   92,   93,   94,
 /*  2240 */   160,    1,    2,    3,    4,    5,  160,   79,  160,  160,
 /*  2250 */   160,  160,  160,  160,  160,   87,   88,   79,   90,   91,
 /*  2260 */    92,   93,   94,  160,  160,   87,   88,  160,   90,   91,
 /*  2270 */    92,   93,   94,  160,  160,  160,   36,   37,  160,   23,
 /*  2280 */    24,   25,   26,   27,   28,   29,   46,   31,  160,    1,
 /*  2290 */     2,    3,    4,    5,  160,  160,  160,  160,  160,   43,
 /*  2300 */   160,   45,  160,  160,  160,  160,  160,    7,    8,    9,
 /*  2310 */    10,   11,   12,   13,   14,   15,   16,   17,   18,   19,
 /*  2320 */    20,   21,   22,  160,   36,   37,  160,   39,   40,  160,
 /*  2330 */    42,  160,   11,  160,   46,   79,  160,  160,   38,  160,
 /*  2340 */   160,  160,  160,   87,   88,  160,   90,   91,   92,   93,
 /*  2350 */    94,  160,   31,  160,  160,  160,  160,  160,  160,   23,
 /*  2360 */    24,   25,   26,   27,   28,  160,  160,   31,   47,   48,
 /*  2370 */    49,   50,   51,   52,   53,   54,   55,   56,   57,   43,
 /*  2380 */   160,   45,  160,  160,  160,  160,  160,    7,    8,    9,
 /*  2390 */    10,   11,   12,   13,   14,   15,   16,   17,   18,   19,
 /*  2400 */    20,   21,   22,  160,  160,  160,  160,  160,  160,  160,
 /*  2410 */   160,  160,  160,  160,  160,   79,  160,  160,   38,  160,
 /*  2420 */   160,  160,  160,   87,   88,  160,   90,   91,   92,   93,
 /*  2430 */    94,    7,    8,    9,   10,   11,   12,   13,   14,   15,
 /*  2440 */    16,   17,   18,   19,   20,   21,   22,    9,   10,   11,
 /*  2450 */    12,   13,   14,   15,   16,   17,   18,   19,   20,   21,
 /*  2460 */    22,  160,   38,  160,  160,  160,  160,  160,  160,  160,
 /*  2470 */     7,    8,    9,   10,   11,   12,   13,   14,   15,   16,
 /*  2480 */    17,   18,   19,   20,   21,   22,    1,    2,    3,    4,
 /*  2490 */     5,  160,   29,    7,    8,    9,   10,   11,   12,   13,
 /*  2500 */    14,   15,   16,   17,   18,   19,   20,   21,   22,  160,
 /*  2510 */   160,  160,  160,  160,  160,  160,   31,  160,  160,  160,
 /*  2520 */   160,   36,   37,    0,   38,  160,  160,  160,  160,   44,
 /*  2530 */   160,   46,    7,    8,    9,   10,   11,   12,   13,   14,
 /*  2540 */    15,   16,   17,   18,   19,   20,   21,   22,    1,    2,
 /*  2550 */     3,    4,    5,   30,  160,  160,   33,   32,   35,   36,
 /*  2560 */    37,  160,  160,   40,  160,    7,    8,    9,   10,   11,
 /*  2570 */    12,   13,   14,   15,   16,   17,   18,   19,   20,   21,
 /*  2580 */    22,  160,  160,   36,   37,  160,   39,   40,  160,  160,
 /*  2590 */    32,  160,  160,   46,  160,  160,  160,  160,    7,    8,
 /*  2600 */     9,   10,   11,   12,   13,   14,   15,   16,   17,   18,
 /*  2610 */    19,   20,   21,   22,    7,    8,    9,   10,   11,   12,
 /*  2620 */    13,   14,   15,   16,   17,   18,   19,   20,   21,   22,
 /*  2630 */   160,  160,  160,  160,  160,  160,   11,  160,  160,  160,
 /*  2640 */   160,  160,  160,  160,  160,  160,  160,  160,   11,  160,
 /*  2650 */   160,  160,  160,  160,   29,  160,   31,  160,  160,  160,
 /*  2660 */   160,  160,  160,  160,  160,  160,   29,  160,   31,  160,
 /*  2670 */   160,   80,   47,   48,   49,   50,   51,   52,   53,   54,
 /*  2680 */    55,   56,   57,  160,   47,   48,   49,   50,   51,   52,
 /*  2690 */    53,   54,   55,   56,   57,    1,    2,    3,    4,    5,
 /*  2700 */     1,    2,    3,    4,    5,  160,    1,    2,    3,    4,
 /*  2710 */     5,  160,  160,  160,  160,  160,  160,  160,  160,  160,
 /*  2720 */   160,  160,  160,  160,  160,  160,  160,  160,  160,  160,
 /*  2730 */    36,   37,  160,   39,   40,   36,   37,  160,   39,   40,
 /*  2740 */    46,   36,   37,  160,  160,   46,  160,   42,  160,  160,
 /*  2750 */   160,   46,  160,    1,    2,    3,    4,    5,    1,    2,
 /*  2760 */     3,    4,    5,  160,  160,  160,  160,  160,  160,  160,
 /*  2770 */   160,  160,  160,  160,  160,  160,  160,  160,  160,  160,
 /*  2780 */   160,  160,  160,  160,  160,  160,  160,  160,   36,   37,
 /*  2790 */   160,  160,  160,   36,   37,  160,   44,  160,   46,  160,
 /*  2800 */   160,   44,  160,   46,
};
#define YY_SHIFT_USE_DFLT (-24)
static short yy_shift_ofst[] = {
 /*     0 */   171,  896, 2523,  -24,  -24,  -24,  -24,  -24,  449,  413,
 /*    10 */   -24,  305,  187,  -24,  999,  279,  -24,  635,  -24, 1180,
 /*    20 */   844,  -24,  -24, 1908, 2547,  -24, 2752,  749,  832, 2637,
 /*    30 */   750,  803,  -24,   35,  -24,  -24,  -24,  -24,  -24,  -24,
 /*    40 */   -24,  -24,  -24,  -24,  -24,  -24,  -24,  -24,  -24,  -24,
 /*    50 */   -24,  -24,  -24,  990,  -24,  498,  -24, 2336,  523,  563,
 /*    60 */   707,  565,  730,   83, 1091,  -24, 2336,  979, 1060,  -24,
 /*    70 */  1212,  -24,  -24,  162,  767, 1187, 1187,  763,  227,  995,
 /*    80 */   -24, 2336,  586,  659,  131,  -24,  747,  515,  611,  768,
 /*    90 */  2336,  849,  -24, 2336,  -24, 2336,  944,  -24,  272,  615,
 /*   100 */   752,  -24,  786,  -24,  945,  -24,   41, 2336,   41, 2336,
 /*   110 */    41, 2336,   41, 2336,   41, 2336,   41, 2336,   41, 2336,
 /*   120 */    41, 2336,   96, 2336,   96, 2336,  -24, 2336,  -24, 2336,
 /*   130 */   -24, 2336,   96, 2336, 2438, 2336, 2438,  -23,  -24,  -24,
 /*   140 */   -24,  -24,  -24,  -24,  -24,  -24,  -24,  -24,  -24, 2463,
 /*   150 */   -24,  710, 2336,  -24, 1184,  818,   31, 2256, 2607, 2336,
 /*   160 */   -24,  497,  359,  944,  -24, 2336,  -24,  -24,  -24,  -24,
 /*   170 */   -24,  -24,  -24,  -24,  -24, 1951,  -24,  649,  -24, 1360,
 /*   180 */   -24,  815, 1280,  -24,  -24,  -24,  -24,  -24,  -24,  -24,
 /*   190 */   -24,  -24,  -24,  848,  772,  473,  -24,  386,  983, 2178,
 /*   200 */   -24,  580,  -24, 2336,  -24,  -24,  -24,  -24,  676,  -24,
 /*   210 */   -24,  370,  533, 2168,  916,  -24,  -24,  -24, 2336,   28,
 /*   220 */   -24, 1873, 1237,  -24,  -24, 2607,  691,    3, 2145, 2424,
 /*   230 */  1187, 1139,  -24,  739,  506,  -24,  739,  -24, 1896,  -24,
 /*   240 */   -24,  -24,  -24,  -24,  -24, 2336,  -24, 2607,  977,  -22,
 /*   250 */  2336,  -24, 2087,  228, 2336,  -24,  277,  228, 2336,  -24,
 /*   260 */  2591,  608,   36, 2336,  -24, 2064,  228, 2336,  -24,  796,
 /*   270 */   228, 2336,  -24, 1992,  228, 2336,  -24,  704,  228, 2336,
 /*   280 */   -24,  -24,  -24,  -24, 2336,  458,  -24, 2336,  -24, 2607,
 /*   290 */   -24,  854,  -24,  846,  -24, 1969, 2558,  -24,  -24,  471,
 /*   300 */   290,  211,  991,  -24,  759,  911,  941,  -24,  852,  940,
 /*   310 */   -24,  902,  961,  -24, 2336, 2525,  -24,  355,  602,  -24,
 /*   320 */   355,  -24,  103,  162,  -24,  -24,  355,  745,  -24,  355,
 /*   330 */   746,  -24,  355,  793,  -24,  355,  794,  -24,  355,  842,
 /*   340 */   -24,  355,  889,  -24,  355,  937,  -24,  355,  938,  -24,
 /*   350 */   355,  986,  -24,  479,  853,  533,  897,   31,  -24,  -24,
 /*   360 */  2336, 2486, 1187,  419,  -24,  894,  475, 1897, 2380, 1187,
 /*   370 */   899,  -24, 2336, 2300, 1187,  467,  -24,  653, 2336,  553,
 /*   380 */   -24,  -24,  -24,  860, 1187, 1187,  -24,  -24,  769,  780,
 /*   390 */   371,  323,  -24,  -24,  147,  396,  275,  -24,  179,  -24,
 /*   400 */  2321,  -24,  980,  162,  -24,  946,  886,  162,  -24,  900,
 /*   410 */   893,  162,  -24,  943,  964,  948,  -24,  -24,  -24,  -24,
 /*   420 */   -24,  -24,  -24,  -24,  -24,  -24, 2240, 2757,   67,  128,
 /*   430 */  2625,  174, 1043,  -24,  947,  -24,  724,  378,  851,  -24,
 /*   440 */   755,  -24,  -24, 1828, 1679,  -24,  -24, 2705,  550,  592,
 /*   450 */   162,  640,  -24,  661,  650,  162,  708,  -24,  -24, 2705,
 /*   460 */  2181,  660,  -24,  162,  784,  -24, 2485,  701,  -24,  162,
 /*   470 */   813,  -24,  -24, 2288, 1379, 1529, 2240, 2694,  -24, 2699,
 /*   480 */   883,  509,  -24,  993,  154,  -24,  397,  466,  557,  -24,
 /*   490 */   751,  154,  -24,  358,  -15,  427,  -24,  259,  154,  -24,
 /*   500 */   -24,
};
#define YY_REDUCE_USE_DFLT (-146)
static short yy_reduce_ofst[] = {
 /*     0 */  1202, -146,   26, -146, -146, -146, -146, -146, -146, -146,
 /*    10 */  -146, -146,  -81, -146, -146,  233, -146, -146, -146,  -96,
 /*    20 */  -146, -146, -146, 1224,  570, -146,  -21, -146, -146,  374,
 /*    30 */  -146, 1722, -146, 1758, -146, -146, -146, -146, -146, -146,
 /*    40 */  -146, -146, -146, -146, -146, -146, -146, -146, -146, -146,
 /*    50 */  -146, -146, -146, -146, -146, -146, -146,  217, -146, 1528,
 /*    60 */  1758, -146, -146, 1378, 1758, -146,  313, -146,  181, -146,
 /*    70 */   870, -146, -146,  761, -146, 1272, 1758, -146, 1193, 1758,
 /*    80 */  -146,   25, -146, 1608, 1758, -146, -146, 1228, 1758, -146,
 /*    90 */   265, -146, -146,  382, -146,  688,  628, -146, -146, -146,
 /*   100 */  -146, -146, -146, -146, -146, -146, -146,  744, -146,  334,
 /*   110 */  -146,  648, -146,  936, -146,  600, -146,  910, -146,  457,
 /*   120 */  -146,  409, -146,  430, -146,  478, -146,  159, -146,  622,
 /*   130 */  -146,  967, -146,  888, -146,  718, -146,  169, -146, -146,
 /*   140 */  -146, -146, -146, -146, -146, -146, -146, -146, -146, -146,
 /*   150 */  -146, -146, 1006, -146, -146, -146, -146,  -50, -146,  792,
 /*   160 */  -146, -146, -146,  711, -146,  814, -146, -146, -146, -146,
 /*   170 */  -146, -146, -146, -146, -146,  240, -146, -146, -146,   65,
 /*   180 */  -146, -146,  624, -146, -146, -146, -146, -146, -146, -146,
 /*   190 */  -146, -146, -146, -146, -146, -146, -146, -146, -146,  142,
 /*   200 */  -146, -146, -146,   46, -146, -146, -146, -146, -146, -146,
 /*   210 */  -146, -146, -146,  286, -146, -146, -146, -146,  985, -146,
 /*   220 */  -146,  190, -146, -146, -146, -146, -146, -146,  840, -146,
 /*   230 */  1643, 1758, -146,  130, -146, -146,  500, -146,  369, -146,
 /*   240 */  -146, -146, -146, -146, -146,  255, -146, -146, -146,  876,
 /*   250 */  1883, -146,  238,  912, 1880, -146, -146,  338, 1857, -146,
 /*   260 */  -146, -146,  588, 1808, -146,  111,  684,  495, -146, -146,
 /*   270 */   753,  399, -146,   94,  461,   64, -146, -146,  351,  447,
 /*   280 */  -146, -146, -146, -146,  -69, -146, -146,   -2, -146, -146,
 /*   290 */  -146, -146, -146, -146, -146,  574, -146, -146, -146, -146,
 /*   300 */  -146, -146, -146, -146, -146, -146, -146, -146, -146, -146,
 /*   310 */  -146, -146, -146, -146,  526, -146, -146,  390, -146, -146,
 /*   320 */   887, -146, -146,  195, -146, -146,  438, -146, -146,  486,
 /*   330 */  -146, -146,  502, -146, -146,  505, -146, -146,  534, -146,
 /*   340 */  -146,  537, -146, -146,  542, -146, -146, -145, -146, -146,
 /*   350 */   583, -146, -146, -146, -146, -146, -146, -146, -146, -146,
 /*   360 */   862, -146, 1158, 1758, -146, -146, -146,  766, -146, 1678,
 /*   370 */  1758, -146,  670, -146, 1572, 1758, -146, -146,  121, -146,
 /*   380 */  -146, -146, -146, -146, 1493, 1758, -146, -146, -146, -146,
 /*   390 */  1458, 1758, -146, -146, -146, -146, 1422, -146, 1758, -146,
 /*   400 */   391, -146, -146,  868, -146, -146, -146,  817, -146, -146,
 /*   410 */  -146,  850, -146, -146, -146, -146, -146, -146, -146, -146,
 /*   420 */  -146, -146, -146, -146, -146, -146,   48,  -21, -146, -146,
 /*   430 */   381, -146, 1343, -146, 1758, -146, -146, -146, 1308, -146,
 /*   440 */  1758, -146, -146,  335,  570, -146, -146,   48, -146, -146,
 /*   450 */   529, -146, -146, -146, -146,  625, -146, -146, -146,  336,
 /*   460 */   -21, -146, -146,  675, -146, -146,  -21, -146, -146,  721,
 /*   470 */  -146, -146, -146,   47,  239,  570,  336,  570, -146,  570,
 /*   480 */  -146,  822, -146, -146,   90, -146, -146, -146,  472, -146,
 /*   490 */  -146,  923, -146, -146, -146,  776, -146, -146,  856, -146,
 /*   500 */  -146,
};
static YYACTIONTYPE yy_default[] = {
 /*     0 */   744,  744,  744,  503,  505,  506,  507,  508,  744,  744,
 /*    10 */   509,  744,  744,  510,  744,  744,  511,  744,  520,  744,
 /*    20 */   744,  518,  519,  744,  744,  538,  744,  744,  744,  744,
 /*    30 */   744,  744,  542,  744,  544,  580,  582,  583,  584,  585,
 /*    40 */   586,  587,  588,  589,  590,  591,  592,  593,  594,  595,
 /*    50 */   596,  597,  598,  744,  599,  744,  600,  744,  744,  744,
 /*    60 */   744,  603,  744,  744,  744,  604,  744,  744,  744,  607,
 /*    70 */   744,  608,  609,  744,  744,  744,  611,  744,  744,  744,
 /*    80 */   614,  744,  744,  744,  744,  616,  744,  744,  744,  744,
 /*    90 */   744,  744,  618,  744,  672,  744,  744,  673,  744,  744,
 /*   100 */   744,  739,  744,  740,  744,  741,  674,  744,  675,  744,
 /*   110 */   676,  744,  677,  744,  678,  744,  679,  744,  680,  744,
 /*   120 */   681,  744,  688,  744,  689,  744,  690,  744,  691,  744,
 /*   130 */   692,  744,  693,  744,  694,  744,  695,  744,  569,  570,
 /*   140 */   571,  572,  573,  574,  575,  576,  577,  578,  579,  744,
 /*   150 */   682,  744,  744,  683,  700,  744,  684,  744,  723,  744,
 /*   160 */   696,  744,  744,  744,  697,  744,  698,  699,  701,  702,
 /*   170 */   703,  704,  705,  706,  707,  744,  708,  744,  709,  744,
 /*   180 */   724,  744,  744,  726,  731,  732,  733,  734,  735,  736,
 /*   190 */   737,  738,  727,  732,  734,  733,  725,  744,  710,  744,
 /*   200 */   711,  744,  712,  744,  715,  716,  721,  722,  744,  719,
 /*   210 */   720,  744,  685,  744,  744,  717,  718,  686,  744,  744,
 /*   220 */   687,  744,  744,  713,  714,  742,  744,  744,  744,  744,
 /*   230 */   744,  744,  619,  744,  744,  623,  744,  624,  744,  626,
 /*   240 */   627,  628,  629,  630,  631,  744,  632,  671,  744,  744,
 /*   250 */   744,  633,  744,  744,  744,  634,  744,  744,  744,  635,
 /*   260 */   744,  744,  744,  744,  636,  744,  744,  744,  637,  744,
 /*   270 */   744,  744,  638,  744,  744,  744,  639,  744,  744,  744,
 /*   280 */   640,  641,  642,  625,  744,  744,  644,  744,  645,  647,
 /*   290 */   646,  744,  648,  744,  649,  744,  744,  650,  651,  744,
 /*   300 */   744,  744,  744,  652,  744,  744,  744,  653,  744,  744,
 /*   310 */   654,  744,  744,  655,  744,  744,  656,  744,  744,  657,
 /*   320 */   744,  667,  669,  744,  670,  668,  744,  744,  658,  744,
 /*   330 */   744,  659,  744,  744,  660,  744,  744,  661,  744,  744,
 /*   340 */   662,  744,  744,  663,  744,  744,  664,  744,  744,  665,
 /*   350 */   744,  744,  666,  744,  744,  744,  744,  744,  743,  581,
 /*   360 */   744,  744,  744,  744,  620,  744,  744,  744,  744,  744,
 /*   370 */   744,  621,  744,  744,  744,  744,  622,  744,  744,  744,
 /*   380 */   617,  615,  613,  744,  744,  612,  610,  605,  601,  744,
 /*   390 */   744,  744,  606,  602,  744,  744,  744,  543,  744,  545,
 /*   400 */   744,  560,  562,  744,  565,  744,  563,  744,  566,  744,
 /*   410 */   564,  744,  567,  744,  744,  744,  561,  550,  552,  553,
 /*   420 */   554,  555,  556,  557,  558,  559,  744,  744,  744,  744,
 /*   430 */   744,  744,  744,  546,  744,  548,  744,  744,  744,  547,
 /*   440 */   744,  549,  551,  744,  744,  536,  539,  744,  744,  744,
 /*   450 */   744,  744,  540,  744,  744,  744,  744,  541,  530,  744,
 /*   460 */   744,  744,  532,  744,  744,  534,  744,  744,  533,  744,
 /*   470 */   744,  535,  537,  744,  744,  744,  744,  744,  531,  744,
 /*   480 */   744,  744,  512,  744,  744,  513,  744,  744,  744,  514,
 /*   490 */   744,  744,  515,  744,  744,  744,  516,  744,  744,  517,
 /*   500 */   504,
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
  "xx_class_body",  "xx_interface_body",  "xx_class_definition",  "xx_class_properties_definition",
  "xx_class_consts_definition",  "xx_class_methods_definition",  "xx_class_property_definition",  "xx_visibility_list",
  "xx_literal_expr",  "xx_class_const_definition",  "xx_class_method_definition",  "xx_parameter_list",
  "xx_statement_list",  "xx_visibility",  "xx_parameter",  "xx_parameter_type",
  "xx_parameter_cast",  "xx_statement",  "xx_let_statement",  "xx_if_statement",
  "xx_loop_statement",  "xx_echo_statement",  "xx_return_statement",  "xx_mcall_statement",
  "xx_scall_statement",  "xx_unset_statement",  "xx_throw_statement",  "xx_declare_statement",
  "xx_break_statement",  "xx_continue_statement",  "xx_while_statement",  "xx_do_while_statement",
  "xx_switch_statement",  "xx_for_statement",  "xx_eval_expr",  "xx_case_clauses",
  "xx_case_clause",  "xx_common_expr",  "xx_let_assignments",  "xx_let_assignment",
  "xx_assignment_operator",  "xx_assign_expr",  "xx_index_expr",  "xx_echo_expressions",
  "xx_echo_expression",  "xx_mcall_expr",  "xx_scall_expr",  "xx_declare_variable_list",
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
 /*   9 */ "xx_interface_def ::= INTERFACE IDENTIFIER xx_class_body",
 /*  10 */ "xx_interface_def ::= INTERFACE IDENTIFIER EXTENDS IDENTIFIER xx_interface_body",
 /*  11 */ "xx_class_def ::= CLASS IDENTIFIER xx_class_body",
 /*  12 */ "xx_class_def ::= CLASS IDENTIFIER EXTENDS IDENTIFIER xx_class_body",
 /*  13 */ "xx_class_def ::= ABSTRACT CLASS IDENTIFIER xx_class_body",
 /*  14 */ "xx_class_def ::= ABSTRACT CLASS IDENTIFIER EXTENDS IDENTIFIER xx_class_body",
 /*  15 */ "xx_class_def ::= FINAL CLASS IDENTIFIER xx_class_body",
 /*  16 */ "xx_class_def ::= FINAL CLASS IDENTIFIER EXTENDS IDENTIFIER xx_class_body",
 /*  17 */ "xx_class_body ::= BRACKET_OPEN xx_class_definition BRACKET_CLOSE",
 /*  18 */ "xx_class_body ::= BRACKET_OPEN BRACKET_CLOSE",
 /*  19 */ "xx_interface_body ::= BRACKET_OPEN BRACKET_CLOSE",
 /*  20 */ "xx_class_definition ::= xx_class_properties_definition",
 /*  21 */ "xx_class_definition ::= xx_class_consts_definition",
 /*  22 */ "xx_class_definition ::= xx_class_methods_definition",
 /*  23 */ "xx_class_definition ::= xx_class_properties_definition xx_class_methods_definition",
 /*  24 */ "xx_class_definition ::= xx_class_properties_definition xx_class_consts_definition",
 /*  25 */ "xx_class_definition ::= xx_class_consts_definition xx_class_properties_definition",
 /*  26 */ "xx_class_definition ::= xx_class_consts_definition xx_class_methods_definition",
 /*  27 */ "xx_class_definition ::= xx_class_properties_definition xx_class_consts_definition xx_class_methods_definition",
 /*  28 */ "xx_class_definition ::= xx_class_consts_definition xx_class_properties_definition xx_class_methods_definition",
 /*  29 */ "xx_class_properties_definition ::= xx_class_properties_definition xx_class_property_definition",
 /*  30 */ "xx_class_properties_definition ::= xx_class_property_definition",
 /*  31 */ "xx_class_property_definition ::= COMMENT xx_visibility_list IDENTIFIER DOTCOMMA",
 /*  32 */ "xx_class_property_definition ::= xx_visibility_list IDENTIFIER DOTCOMMA",
 /*  33 */ "xx_class_property_definition ::= COMMENT xx_visibility_list IDENTIFIER ASSIGN xx_literal_expr DOTCOMMA",
 /*  34 */ "xx_class_property_definition ::= xx_visibility_list IDENTIFIER ASSIGN xx_literal_expr DOTCOMMA",
 /*  35 */ "xx_class_consts_definition ::= xx_class_consts_definition xx_class_const_definition",
 /*  36 */ "xx_class_consts_definition ::= xx_class_const_definition",
 /*  37 */ "xx_class_methods_definition ::= xx_class_methods_definition xx_class_method_definition",
 /*  38 */ "xx_class_methods_definition ::= xx_class_method_definition",
 /*  39 */ "xx_class_const_definition ::= COMMENT CONST CONSTANT ASSIGN xx_literal_expr DOTCOMMA",
 /*  40 */ "xx_class_const_definition ::= CONST CONSTANT ASSIGN xx_literal_expr DOTCOMMA",
 /*  41 */ "xx_class_method_definition ::= xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE BRACKET_OPEN BRACKET_CLOSE",
 /*  42 */ "xx_class_method_definition ::= xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE BRACKET_OPEN BRACKET_CLOSE",
 /*  43 */ "xx_class_method_definition ::= xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  44 */ "xx_class_method_definition ::= xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  45 */ "xx_class_method_definition ::= COMMENT xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE BRACKET_OPEN BRACKET_CLOSE",
 /*  46 */ "xx_class_method_definition ::= COMMENT xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE BRACKET_OPEN BRACKET_CLOSE",
 /*  47 */ "xx_class_method_definition ::= COMMENT xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  48 */ "xx_class_method_definition ::= COMMENT xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  49 */ "xx_visibility_list ::= xx_visibility_list xx_visibility",
 /*  50 */ "xx_visibility_list ::= xx_visibility",
 /*  51 */ "xx_visibility ::= PUBLIC",
 /*  52 */ "xx_visibility ::= PROTECTED",
 /*  53 */ "xx_visibility ::= PRIVATE",
 /*  54 */ "xx_visibility ::= STATIC",
 /*  55 */ "xx_visibility ::= SCOPED",
 /*  56 */ "xx_visibility ::= INLINE",
 /*  57 */ "xx_visibility ::= ABSTRACT",
 /*  58 */ "xx_visibility ::= FINAL",
 /*  59 */ "xx_parameter_list ::= xx_parameter_list COMMA xx_parameter",
 /*  60 */ "xx_parameter_list ::= xx_parameter",
 /*  61 */ "xx_parameter ::= IDENTIFIER",
 /*  62 */ "xx_parameter ::= xx_parameter_type IDENTIFIER",
 /*  63 */ "xx_parameter ::= xx_parameter_cast IDENTIFIER",
 /*  64 */ "xx_parameter ::= IDENTIFIER ASSIGN xx_literal_expr",
 /*  65 */ "xx_parameter ::= xx_parameter_type IDENTIFIER ASSIGN xx_literal_expr",
 /*  66 */ "xx_parameter ::= xx_parameter_cast IDENTIFIER ASSIGN xx_literal_expr",
 /*  67 */ "xx_parameter_cast ::= LESS IDENTIFIER GREATER",
 /*  68 */ "xx_parameter_type ::= TYPE_INTEGER",
 /*  69 */ "xx_parameter_type ::= TYPE_UINTEGER",
 /*  70 */ "xx_parameter_type ::= TYPE_LONG",
 /*  71 */ "xx_parameter_type ::= TYPE_ULONG",
 /*  72 */ "xx_parameter_type ::= TYPE_CHAR",
 /*  73 */ "xx_parameter_type ::= TYPE_UCHAR",
 /*  74 */ "xx_parameter_type ::= TYPE_DOUBLE",
 /*  75 */ "xx_parameter_type ::= TYPE_BOOL",
 /*  76 */ "xx_parameter_type ::= TYPE_STRING",
 /*  77 */ "xx_parameter_type ::= TYPE_ARRAY",
 /*  78 */ "xx_parameter_type ::= TYPE_VAR",
 /*  79 */ "xx_statement_list ::= xx_statement_list xx_statement",
 /*  80 */ "xx_statement_list ::= xx_statement",
 /*  81 */ "xx_statement ::= xx_let_statement",
 /*  82 */ "xx_statement ::= xx_if_statement",
 /*  83 */ "xx_statement ::= xx_loop_statement",
 /*  84 */ "xx_statement ::= xx_echo_statement",
 /*  85 */ "xx_statement ::= xx_return_statement",
 /*  86 */ "xx_statement ::= xx_mcall_statement",
 /*  87 */ "xx_statement ::= xx_scall_statement",
 /*  88 */ "xx_statement ::= xx_unset_statement",
 /*  89 */ "xx_statement ::= xx_throw_statement",
 /*  90 */ "xx_statement ::= xx_declare_statement",
 /*  91 */ "xx_statement ::= xx_break_statement",
 /*  92 */ "xx_statement ::= xx_continue_statement",
 /*  93 */ "xx_statement ::= xx_while_statement",
 /*  94 */ "xx_statement ::= xx_do_while_statement",
 /*  95 */ "xx_statement ::= xx_switch_statement",
 /*  96 */ "xx_statement ::= xx_for_statement",
 /*  97 */ "xx_statement ::= xx_comment",
 /*  98 */ "xx_break_statement ::= BREAK DOTCOMMA",
 /*  99 */ "xx_continue_statement ::= CONTINUE DOTCOMMA",
 /* 100 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN BRACKET_CLOSE",
 /* 101 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN BRACKET_CLOSE ELSE BRACKET_OPEN BRACKET_CLOSE",
 /* 102 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 103 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE ELSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 104 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE ELSE BRACKET_OPEN BRACKET_CLOSE",
 /* 105 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN BRACKET_CLOSE ELSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 106 */ "xx_switch_statement ::= SWITCH xx_eval_expr BRACKET_OPEN BRACKET_CLOSE",
 /* 107 */ "xx_switch_statement ::= SWITCH xx_eval_expr BRACKET_OPEN xx_case_clauses BRACKET_CLOSE",
 /* 108 */ "xx_case_clauses ::= xx_case_clauses xx_case_clause",
 /* 109 */ "xx_case_clauses ::= xx_case_clause",
 /* 110 */ "xx_case_clause ::= CASE xx_literal_expr COLON xx_statement_list",
 /* 111 */ "xx_case_clause ::= DEFAULT COLON xx_statement_list",
 /* 112 */ "xx_loop_statement ::= LOOP BRACKET_OPEN BRACKET_CLOSE",
 /* 113 */ "xx_loop_statement ::= LOOP BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 114 */ "xx_while_statement ::= WHILE xx_eval_expr BRACKET_OPEN BRACKET_CLOSE",
 /* 115 */ "xx_while_statement ::= WHILE xx_eval_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 116 */ "xx_do_while_statement ::= DO BRACKET_OPEN BRACKET_CLOSE WHILE xx_eval_expr DOTCOMMA",
 /* 117 */ "xx_do_while_statement ::= DO BRACKET_OPEN xx_statement_list BRACKET_CLOSE WHILE xx_eval_expr DOTCOMMA",
 /* 118 */ "xx_for_statement ::= FOR IDENTIFIER IN xx_common_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 119 */ "xx_for_statement ::= FOR IDENTIFIER IN REVERSE xx_common_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 120 */ "xx_for_statement ::= FOR IDENTIFIER COMMA IDENTIFIER IN xx_common_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 121 */ "xx_for_statement ::= FOR IDENTIFIER COMMA IDENTIFIER IN REVERSE xx_common_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 122 */ "xx_let_statement ::= LET xx_let_assignments DOTCOMMA",
 /* 123 */ "xx_let_assignments ::= xx_let_assignments COMMA xx_let_assignment",
 /* 124 */ "xx_let_assignments ::= xx_let_assignment",
 /* 125 */ "xx_assignment_operator ::= ASSIGN",
 /* 126 */ "xx_assignment_operator ::= ADDASSIGN",
 /* 127 */ "xx_assignment_operator ::= SUBASSIGN",
 /* 128 */ "xx_assignment_operator ::= MULASSIGN",
 /* 129 */ "xx_assignment_operator ::= DIVASSIGN",
 /* 130 */ "xx_assignment_operator ::= CONCATASSIGN",
 /* 131 */ "xx_let_assignment ::= IDENTIFIER xx_assignment_operator xx_assign_expr",
 /* 132 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER xx_assignment_operator xx_assign_expr",
 /* 133 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER SBRACKET_OPEN SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 134 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER SBRACKET_OPEN xx_index_expr SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 135 */ "xx_let_assignment ::= IDENTIFIER DOUBLECOLON IDENTIFIER xx_assignment_operator xx_assign_expr",
 /* 136 */ "xx_let_assignment ::= IDENTIFIER DOUBLECOLON IDENTIFIER SBRACKET_OPEN SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 137 */ "xx_let_assignment ::= IDENTIFIER DOUBLECOLON IDENTIFIER SBRACKET_OPEN xx_index_expr SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 138 */ "xx_let_assignment ::= IDENTIFIER SBRACKET_OPEN SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 139 */ "xx_let_assignment ::= IDENTIFIER SBRACKET_OPEN xx_index_expr SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 140 */ "xx_let_assignment ::= IDENTIFIER INCR",
 /* 141 */ "xx_let_assignment ::= IDENTIFIER DECR",
 /* 142 */ "xx_index_expr ::= xx_common_expr",
 /* 143 */ "xx_echo_statement ::= ECHO xx_echo_expressions DOTCOMMA",
 /* 144 */ "xx_echo_expressions ::= xx_echo_expressions COMMA xx_echo_expression",
 /* 145 */ "xx_echo_expressions ::= xx_echo_expression",
 /* 146 */ "xx_echo_expression ::= xx_common_expr",
 /* 147 */ "xx_mcall_statement ::= xx_mcall_expr DOTCOMMA",
 /* 148 */ "xx_scall_statement ::= xx_scall_expr DOTCOMMA",
 /* 149 */ "xx_return_statement ::= RETURN xx_common_expr DOTCOMMA",
 /* 150 */ "xx_return_statement ::= RETURN DOTCOMMA",
 /* 151 */ "xx_unset_statement ::= UNSET IDENTIFIER ARROW IDENTIFIER DOTCOMMA",
 /* 152 */ "xx_unset_statement ::= UNSET IDENTIFIER SBRACKET_OPEN IDENTIFIER SBRACKET_CLOSE DOTCOMMA",
 /* 153 */ "xx_unset_statement ::= UNSET IDENTIFIER SBRACKET_OPEN INTEGER SBRACKET_CLOSE DOTCOMMA",
 /* 154 */ "xx_unset_statement ::= UNSET IDENTIFIER SBRACKET_OPEN STRING SBRACKET_CLOSE DOTCOMMA",
 /* 155 */ "xx_throw_statement ::= THROW xx_common_expr DOTCOMMA",
 /* 156 */ "xx_declare_statement ::= TYPE_INTEGER xx_declare_variable_list DOTCOMMA",
 /* 157 */ "xx_declare_statement ::= TYPE_UINTEGER xx_declare_variable_list DOTCOMMA",
 /* 158 */ "xx_declare_statement ::= TYPE_CHAR xx_declare_variable_list DOTCOMMA",
 /* 159 */ "xx_declare_statement ::= TYPE_UCHAR xx_declare_variable_list DOTCOMMA",
 /* 160 */ "xx_declare_statement ::= TYPE_LONG xx_declare_variable_list DOTCOMMA",
 /* 161 */ "xx_declare_statement ::= TYPE_ULONG xx_declare_variable_list DOTCOMMA",
 /* 162 */ "xx_declare_statement ::= TYPE_DOUBLE xx_declare_variable_list DOTCOMMA",
 /* 163 */ "xx_declare_statement ::= TYPE_STRING xx_declare_variable_list DOTCOMMA",
 /* 164 */ "xx_declare_statement ::= TYPE_BOOL xx_declare_variable_list DOTCOMMA",
 /* 165 */ "xx_declare_statement ::= TYPE_VAR xx_declare_variable_list DOTCOMMA",
 /* 166 */ "xx_declare_variable_list ::= xx_declare_variable_list COMMA xx_declare_variable",
 /* 167 */ "xx_declare_variable_list ::= xx_declare_variable",
 /* 168 */ "xx_declare_variable ::= IDENTIFIER",
 /* 169 */ "xx_declare_variable ::= IDENTIFIER ASSIGN xx_literal_expr",
 /* 170 */ "xx_assign_expr ::= xx_common_expr",
 /* 171 */ "xx_common_expr ::= NOT xx_common_expr",
 /* 172 */ "xx_common_expr ::= ISSET xx_isset_expr",
 /* 173 */ "xx_common_expr ::= xx_common_expr EQUALS xx_common_expr",
 /* 174 */ "xx_common_expr ::= xx_common_expr NOTEQUALS xx_common_expr",
 /* 175 */ "xx_common_expr ::= xx_common_expr IDENTICAL xx_common_expr",
 /* 176 */ "xx_common_expr ::= xx_common_expr NOTIDENTICAL xx_common_expr",
 /* 177 */ "xx_common_expr ::= xx_common_expr LESS xx_common_expr",
 /* 178 */ "xx_common_expr ::= xx_common_expr GREATER xx_common_expr",
 /* 179 */ "xx_common_expr ::= xx_common_expr LESSEQUAL xx_common_expr",
 /* 180 */ "xx_common_expr ::= xx_common_expr GREATEREQUAL xx_common_expr",
 /* 181 */ "xx_common_expr ::= PARENTHESES_OPEN xx_common_expr PARENTHESES_CLOSE",
 /* 182 */ "xx_common_expr ::= PARENTHESES_OPEN xx_parameter_type PARENTHESES_CLOSE xx_common_expr",
 /* 183 */ "xx_common_expr ::= IDENTIFIER ARROW IDENTIFIER",
 /* 184 */ "xx_common_expr ::= IDENTIFIER DOUBLECOLON IDENTIFIER",
 /* 185 */ "xx_common_expr ::= IDENTIFIER DOUBLECOLON CONSTANT",
 /* 186 */ "xx_common_expr ::= IDENTIFIER SBRACKET_OPEN xx_common_expr SBRACKET_CLOSE",
 /* 187 */ "xx_common_expr ::= xx_common_expr ADD xx_common_expr",
 /* 188 */ "xx_common_expr ::= xx_common_expr SUB xx_common_expr",
 /* 189 */ "xx_common_expr ::= xx_common_expr MUL xx_common_expr",
 /* 190 */ "xx_common_expr ::= xx_common_expr DIV xx_common_expr",
 /* 191 */ "xx_common_expr ::= xx_common_expr MOD xx_common_expr",
 /* 192 */ "xx_common_expr ::= xx_common_expr CONCAT xx_common_expr",
 /* 193 */ "xx_common_expr ::= xx_common_expr AND xx_common_expr",
 /* 194 */ "xx_common_expr ::= xx_common_expr OR xx_common_expr",
 /* 195 */ "xx_common_expr ::= INSTANCEOF xx_common_expr",
 /* 196 */ "xx_common_expr ::= FETCH IDENTIFIER COMMA xx_isset_expr",
 /* 197 */ "xx_common_expr ::= TYPEOF xx_common_expr",
 /* 198 */ "xx_common_expr ::= CONSTANT",
 /* 199 */ "xx_common_expr ::= IDENTIFIER",
 /* 200 */ "xx_common_expr ::= INTEGER",
 /* 201 */ "xx_common_expr ::= STRING",
 /* 202 */ "xx_common_expr ::= CHAR",
 /* 203 */ "xx_common_expr ::= DOUBLE",
 /* 204 */ "xx_common_expr ::= NULL",
 /* 205 */ "xx_common_expr ::= TRUE",
 /* 206 */ "xx_common_expr ::= FALSE",
 /* 207 */ "xx_common_expr ::= SBRACKET_OPEN SBRACKET_CLOSE",
 /* 208 */ "xx_common_expr ::= SBRACKET_OPEN xx_array_list SBRACKET_CLOSE",
 /* 209 */ "xx_common_expr ::= NEW IDENTIFIER",
 /* 210 */ "xx_common_expr ::= NEW IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 211 */ "xx_common_expr ::= NEW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 212 */ "xx_common_expr ::= IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 213 */ "xx_common_expr ::= IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 214 */ "xx_common_expr ::= xx_mcall_expr",
 /* 215 */ "xx_common_expr ::= xx_scall_expr",
 /* 216 */ "xx_scall_expr ::= IDENTIFIER DOUBLECOLON IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 217 */ "xx_scall_expr ::= IDENTIFIER DOUBLECOLON IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 218 */ "xx_mcall_expr ::= IDENTIFIER ARROW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 219 */ "xx_mcall_expr ::= IDENTIFIER ARROW IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 220 */ "xx_call_parameters ::= xx_call_parameters COMMA xx_call_parameter",
 /* 221 */ "xx_call_parameters ::= xx_call_parameter",
 /* 222 */ "xx_call_parameter ::= xx_common_expr",
 /* 223 */ "xx_array_list ::= xx_array_list COMMA xx_array_item",
 /* 224 */ "xx_array_list ::= xx_array_item",
 /* 225 */ "xx_array_item ::= xx_array_key COLON xx_array_value",
 /* 226 */ "xx_array_item ::= xx_array_value",
 /* 227 */ "xx_array_key ::= IDENTIFIER",
 /* 228 */ "xx_array_key ::= STRING",
 /* 229 */ "xx_array_key ::= INTEGER",
 /* 230 */ "xx_array_value ::= xx_literal_expr",
 /* 231 */ "xx_array_value ::= IDENTIFIER",
 /* 232 */ "xx_literal_expr ::= INTEGER",
 /* 233 */ "xx_literal_expr ::= STRING",
 /* 234 */ "xx_literal_expr ::= DOUBLE",
 /* 235 */ "xx_literal_expr ::= NULL",
 /* 236 */ "xx_literal_expr ::= FALSE",
 /* 237 */ "xx_literal_expr ::= TRUE",
 /* 238 */ "xx_isset_expr ::= IDENTIFIER SBRACKET_OPEN IDENTIFIER SBRACKET_CLOSE",
 /* 239 */ "xx_isset_expr ::= IDENTIFIER SBRACKET_OPEN INTEGER SBRACKET_CLOSE",
 /* 240 */ "xx_isset_expr ::= IDENTIFIER SBRACKET_OPEN STRING SBRACKET_CLOSE",
 /* 241 */ "xx_eval_expr ::= xx_common_expr",
 /* 242 */ "xx_comment ::= COMMENT",
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
// 860 "parser.lemon"
{
	/*if ((yypminor->yy0)) {
		if ((yypminor->yy0)->free_flag) {
			efree((yypminor->yy0)->token);
		}
		efree((yypminor->yy0));
	}*/
}
// 2168 "parser.c"
      break;
    case 97:
// 873 "parser.lemon"
{ json_object_put((yypminor->yy95)); }
// 2173 "parser.c"
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
  { 104, 3 },
  { 104, 2 },
  { 105, 2 },
  { 106, 1 },
  { 106, 1 },
  { 106, 1 },
  { 106, 2 },
  { 106, 2 },
  { 106, 2 },
  { 106, 2 },
  { 106, 3 },
  { 106, 3 },
  { 107, 2 },
  { 107, 1 },
  { 110, 4 },
  { 110, 3 },
  { 110, 6 },
  { 110, 5 },
  { 108, 2 },
  { 108, 1 },
  { 109, 2 },
  { 109, 1 },
  { 113, 6 },
  { 113, 5 },
  { 114, 7 },
  { 114, 8 },
  { 114, 8 },
  { 114, 9 },
  { 114, 8 },
  { 114, 9 },
  { 114, 9 },
  { 114, 10 },
  { 111, 2 },
  { 111, 1 },
  { 117, 1 },
  { 117, 1 },
  { 117, 1 },
  { 117, 1 },
  { 117, 1 },
  { 117, 1 },
  { 117, 1 },
  { 117, 1 },
  { 115, 3 },
  { 115, 1 },
  { 118, 1 },
  { 118, 2 },
  { 118, 2 },
  { 118, 3 },
  { 118, 4 },
  { 118, 4 },
  { 120, 3 },
  { 119, 1 },
  { 119, 1 },
  { 119, 1 },
  { 119, 1 },
  { 119, 1 },
  { 119, 1 },
  { 119, 1 },
  { 119, 1 },
  { 119, 1 },
  { 119, 1 },
  { 119, 1 },
  { 116, 2 },
  { 116, 1 },
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
  { 121, 1 },
  { 121, 1 },
  { 121, 1 },
  { 121, 1 },
  { 121, 1 },
  { 121, 1 },
  { 132, 2 },
  { 133, 2 },
  { 123, 4 },
  { 123, 7 },
  { 123, 5 },
  { 123, 9 },
  { 123, 8 },
  { 123, 8 },
  { 136, 4 },
  { 136, 5 },
  { 139, 2 },
  { 139, 1 },
  { 140, 4 },
  { 140, 3 },
  { 124, 3 },
  { 124, 4 },
  { 134, 4 },
  { 134, 5 },
  { 135, 6 },
  { 135, 7 },
  { 137, 7 },
  { 137, 8 },
  { 137, 9 },
  { 137, 10 },
  { 122, 3 },
  { 142, 3 },
  { 142, 1 },
  { 144, 1 },
  { 144, 1 },
  { 144, 1 },
  { 144, 1 },
  { 144, 1 },
  { 144, 1 },
  { 143, 3 },
  { 143, 5 },
  { 143, 7 },
  { 143, 8 },
  { 143, 5 },
  { 143, 7 },
  { 143, 8 },
  { 143, 5 },
  { 143, 6 },
  { 143, 2 },
  { 143, 2 },
  { 146, 1 },
  { 125, 3 },
  { 147, 3 },
  { 147, 1 },
  { 148, 1 },
  { 127, 2 },
  { 128, 2 },
  { 126, 3 },
  { 126, 2 },
  { 129, 5 },
  { 129, 6 },
  { 129, 6 },
  { 129, 6 },
  { 130, 3 },
  { 131, 3 },
  { 131, 3 },
  { 131, 3 },
  { 131, 3 },
  { 131, 3 },
  { 131, 3 },
  { 131, 3 },
  { 131, 3 },
  { 131, 3 },
  { 131, 3 },
  { 151, 3 },
  { 151, 1 },
  { 152, 1 },
  { 152, 3 },
  { 145, 1 },
  { 141, 2 },
  { 141, 2 },
  { 141, 3 },
  { 141, 3 },
  { 141, 3 },
  { 141, 3 },
  { 141, 3 },
  { 141, 3 },
  { 141, 3 },
  { 141, 3 },
  { 141, 3 },
  { 141, 4 },
  { 141, 3 },
  { 141, 3 },
  { 141, 3 },
  { 141, 4 },
  { 141, 3 },
  { 141, 3 },
  { 141, 3 },
  { 141, 3 },
  { 141, 3 },
  { 141, 3 },
  { 141, 3 },
  { 141, 3 },
  { 141, 2 },
  { 141, 4 },
  { 141, 2 },
  { 141, 1 },
  { 141, 1 },
  { 141, 1 },
  { 141, 1 },
  { 141, 1 },
  { 141, 1 },
  { 141, 1 },
  { 141, 1 },
  { 141, 1 },
  { 141, 2 },
  { 141, 3 },
  { 141, 2 },
  { 141, 4 },
  { 141, 5 },
  { 141, 4 },
  { 141, 3 },
  { 141, 1 },
  { 141, 1 },
  { 150, 6 },
  { 150, 5 },
  { 149, 6 },
  { 149, 5 },
  { 155, 3 },
  { 155, 1 },
  { 156, 1 },
  { 154, 3 },
  { 154, 1 },
  { 157, 3 },
  { 157, 1 },
  { 158, 1 },
  { 158, 1 },
  { 158, 1 },
  { 159, 1 },
  { 159, 1 },
  { 112, 1 },
  { 112, 1 },
  { 112, 1 },
  { 112, 1 },
  { 112, 1 },
  { 112, 1 },
  { 153, 4 },
  { 153, 4 },
  { 153, 4 },
  { 138, 1 },
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
// 869 "parser.lemon"
{
	status->ret = yymsp[0].minor.yy95;
}
// 2633 "parser.c"
        break;
      case 1:
      case 4:
      case 5:
      case 6:
      case 7:
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
      case 142:
      case 170:
      case 214:
      case 215:
      case 222:
      case 230:
      case 241:
// 875 "parser.lemon"
{
	yygotominor.yy95 = yymsp[0].minor.yy95;
}
// 2668 "parser.c"
        break;
      case 2:
      case 29:
      case 35:
      case 37:
      case 49:
      case 79:
      case 108:
// 879 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_list(yymsp[-1].minor.yy95, yymsp[0].minor.yy95);
}
// 2681 "parser.c"
        break;
      case 3:
      case 30:
      case 36:
      case 38:
      case 50:
      case 60:
      case 80:
      case 109:
      case 124:
      case 145:
      case 167:
      case 221:
      case 224:
// 883 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_list(NULL, yymsp[0].minor.yy95);
}
// 2700 "parser.c"
        break;
      case 8:
// 903 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_namespace(yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 2709 "parser.c"
        break;
      case 9:
// 907 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_interface(yymsp[-1].minor.yy0, yymsp[0].minor.yy95, NULL, status->scanner_state);
  yy_destructor(33,&yymsp[-2].minor);
}
// 2717 "parser.c"
        break;
      case 10:
// 911 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_interface(yymsp[-3].minor.yy0, yymsp[0].minor.yy95, yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(33,&yymsp[-4].minor);
  yy_destructor(34,&yymsp[-2].minor);
}
// 2726 "parser.c"
        break;
      case 11:
// 915 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_class(yymsp[-1].minor.yy0, yymsp[0].minor.yy95, 0, 0, NULL, status->scanner_state);
  yy_destructor(35,&yymsp[-2].minor);
}
// 2734 "parser.c"
        break;
      case 12:
// 919 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy95, 0, 0, yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(35,&yymsp[-4].minor);
  yy_destructor(34,&yymsp[-2].minor);
}
// 2743 "parser.c"
        break;
      case 13:
// 923 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_class(yymsp[-1].minor.yy0, yymsp[0].minor.yy95, 1, 0, NULL, status->scanner_state);
  yy_destructor(36,&yymsp[-3].minor);
  yy_destructor(35,&yymsp[-2].minor);
}
// 2752 "parser.c"
        break;
      case 14:
// 927 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy95, 1, 0, yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(36,&yymsp[-5].minor);
  yy_destructor(35,&yymsp[-4].minor);
  yy_destructor(34,&yymsp[-2].minor);
}
// 2762 "parser.c"
        break;
      case 15:
// 931 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_class(yymsp[-1].minor.yy0, yymsp[0].minor.yy95, 0, 1, NULL, status->scanner_state);
  yy_destructor(37,&yymsp[-3].minor);
  yy_destructor(35,&yymsp[-2].minor);
}
// 2771 "parser.c"
        break;
      case 16:
// 935 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy95, 0, 1, yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(37,&yymsp[-5].minor);
  yy_destructor(35,&yymsp[-4].minor);
  yy_destructor(34,&yymsp[-2].minor);
}
// 2781 "parser.c"
        break;
      case 17:
// 939 "parser.lemon"
{
	yygotominor.yy95 = yymsp[-1].minor.yy95;
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 2790 "parser.c"
        break;
      case 18:
      case 19:
// 943 "parser.lemon"
{
	yygotominor.yy95 = NULL;
  yy_destructor(38,&yymsp[-1].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 2800 "parser.c"
        break;
      case 20:
// 951 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_class_definition(yymsp[0].minor.yy95, NULL, NULL, status->scanner_state);
}
// 2807 "parser.c"
        break;
      case 21:
// 955 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_class_definition(NULL, NULL, yymsp[0].minor.yy95, status->scanner_state);
}
// 2814 "parser.c"
        break;
      case 22:
// 959 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_class_definition(NULL, yymsp[0].minor.yy95, NULL, status->scanner_state);
}
// 2821 "parser.c"
        break;
      case 23:
// 963 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_class_definition(yymsp[-1].minor.yy95, yymsp[0].minor.yy95, NULL, status->scanner_state);
}
// 2828 "parser.c"
        break;
      case 24:
// 967 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_class_definition(yymsp[-1].minor.yy95, NULL, yymsp[0].minor.yy95, status->scanner_state);
}
// 2835 "parser.c"
        break;
      case 25:
// 971 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_class_definition(yymsp[0].minor.yy95, NULL, yymsp[-1].minor.yy95, status->scanner_state);
}
// 2842 "parser.c"
        break;
      case 26:
// 975 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_class_definition(NULL, yymsp[0].minor.yy95, yymsp[-1].minor.yy95, status->scanner_state);
}
// 2849 "parser.c"
        break;
      case 27:
// 979 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_class_definition(yymsp[-2].minor.yy95, yymsp[0].minor.yy95, yymsp[-1].minor.yy95, status->scanner_state);
}
// 2856 "parser.c"
        break;
      case 28:
// 983 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_class_definition(yymsp[-1].minor.yy95, yymsp[0].minor.yy95, yymsp[-2].minor.yy95, status->scanner_state);
}
// 2863 "parser.c"
        break;
      case 31:
// 995 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_class_property(yymsp[-2].minor.yy95, yymsp[-1].minor.yy0, NULL, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(32,&yymsp[0].minor);
}
// 2871 "parser.c"
        break;
      case 32:
// 999 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_class_property(yymsp[-2].minor.yy95, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(32,&yymsp[0].minor);
}
// 2879 "parser.c"
        break;
      case 33:
// 1003 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_class_property(yymsp[-4].minor.yy95, yymsp[-3].minor.yy0, yymsp[-1].minor.yy95, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(41,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 2888 "parser.c"
        break;
      case 34:
// 1007 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_class_property(yymsp[-4].minor.yy95, yymsp[-3].minor.yy0, yymsp[-1].minor.yy95, NULL, status->scanner_state);
  yy_destructor(41,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 2897 "parser.c"
        break;
      case 39:
// 1027 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy95, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(42,&yymsp[-4].minor);
  yy_destructor(41,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 2907 "parser.c"
        break;
      case 40:
// 1031 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy95, NULL, status->scanner_state);
  yy_destructor(42,&yymsp[-4].minor);
  yy_destructor(41,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 2917 "parser.c"
        break;
      case 41:
// 1035 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_class_method(yymsp[-6].minor.yy95, yymsp[-4].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(44,&yymsp[-5].minor);
  yy_destructor(45,&yymsp[-3].minor);
  yy_destructor(29,&yymsp[-2].minor);
  yy_destructor(38,&yymsp[-1].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 2929 "parser.c"
        break;
      case 42:
// 1039 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_class_method(yymsp[-7].minor.yy95, yymsp[-5].minor.yy0, yymsp[-3].minor.yy95, NULL, NULL, status->scanner_state);
  yy_destructor(44,&yymsp[-6].minor);
  yy_destructor(45,&yymsp[-4].minor);
  yy_destructor(29,&yymsp[-2].minor);
  yy_destructor(38,&yymsp[-1].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 2941 "parser.c"
        break;
      case 43:
// 1043 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_class_method(yymsp[-7].minor.yy95, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy95, NULL, status->scanner_state);
  yy_destructor(44,&yymsp[-6].minor);
  yy_destructor(45,&yymsp[-4].minor);
  yy_destructor(29,&yymsp[-3].minor);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 2953 "parser.c"
        break;
      case 44:
// 1047 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_class_method(yymsp[-8].minor.yy95, yymsp[-6].minor.yy0, yymsp[-4].minor.yy95, yymsp[-1].minor.yy95, NULL, status->scanner_state);
  yy_destructor(44,&yymsp[-7].minor);
  yy_destructor(45,&yymsp[-5].minor);
  yy_destructor(29,&yymsp[-3].minor);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 2965 "parser.c"
        break;
      case 45:
// 1051 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_class_method(yymsp[-6].minor.yy95, yymsp[-4].minor.yy0, NULL, NULL, yymsp[-7].minor.yy0, status->scanner_state);
  yy_destructor(44,&yymsp[-5].minor);
  yy_destructor(45,&yymsp[-3].minor);
  yy_destructor(29,&yymsp[-2].minor);
  yy_destructor(38,&yymsp[-1].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 2977 "parser.c"
        break;
      case 46:
// 1055 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_class_method(yymsp[-7].minor.yy95, yymsp[-5].minor.yy0, yymsp[-3].minor.yy95, NULL, yymsp[-8].minor.yy0, status->scanner_state);
  yy_destructor(44,&yymsp[-6].minor);
  yy_destructor(45,&yymsp[-4].minor);
  yy_destructor(29,&yymsp[-2].minor);
  yy_destructor(38,&yymsp[-1].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 2989 "parser.c"
        break;
      case 47:
// 1059 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_class_method(yymsp[-7].minor.yy95, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy95, yymsp[-8].minor.yy0, status->scanner_state);
  yy_destructor(44,&yymsp[-6].minor);
  yy_destructor(45,&yymsp[-4].minor);
  yy_destructor(29,&yymsp[-3].minor);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 3001 "parser.c"
        break;
      case 48:
// 1063 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_class_method(yymsp[-8].minor.yy95, yymsp[-6].minor.yy0, yymsp[-4].minor.yy95, yymsp[-1].minor.yy95, yymsp[-9].minor.yy0, status->scanner_state);
  yy_destructor(44,&yymsp[-7].minor);
  yy_destructor(45,&yymsp[-5].minor);
  yy_destructor(29,&yymsp[-3].minor);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 3013 "parser.c"
        break;
      case 51:
// 1075 "parser.lemon"
{
	yygotominor.yy95 = json_object_new_string("public");
  yy_destructor(1,&yymsp[0].minor);
}
// 3021 "parser.c"
        break;
      case 52:
// 1079 "parser.lemon"
{
	yygotominor.yy95 = json_object_new_string("protected");
  yy_destructor(2,&yymsp[0].minor);
}
// 3029 "parser.c"
        break;
      case 53:
// 1083 "parser.lemon"
{
	yygotominor.yy95 = json_object_new_string("private");
  yy_destructor(4,&yymsp[0].minor);
}
// 3037 "parser.c"
        break;
      case 54:
// 1087 "parser.lemon"
{
	yygotominor.yy95 = json_object_new_string("static");
  yy_destructor(3,&yymsp[0].minor);
}
// 3045 "parser.c"
        break;
      case 55:
// 1091 "parser.lemon"
{
	yygotominor.yy95 = json_object_new_string("scoped");
  yy_destructor(5,&yymsp[0].minor);
}
// 3053 "parser.c"
        break;
      case 56:
// 1095 "parser.lemon"
{
	yygotominor.yy95 = json_object_new_string("inline");
  yy_destructor(46,&yymsp[0].minor);
}
// 3061 "parser.c"
        break;
      case 57:
// 1099 "parser.lemon"
{
	yygotominor.yy95 = json_object_new_string("abstract");
  yy_destructor(36,&yymsp[0].minor);
}
// 3069 "parser.c"
        break;
      case 58:
// 1103 "parser.lemon"
{
	yygotominor.yy95 = json_object_new_string("final");
  yy_destructor(37,&yymsp[0].minor);
}
// 3077 "parser.c"
        break;
      case 59:
      case 123:
      case 144:
      case 166:
      case 220:
      case 223:
// 1107 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_list(yymsp[-2].minor.yy95, yymsp[0].minor.yy95);
  yy_destructor(6,&yymsp[-1].minor);
}
// 3090 "parser.c"
        break;
      case 61:
// 1115 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_parameter(NULL, NULL, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 3097 "parser.c"
        break;
      case 62:
// 1119 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_parameter(yymsp[-1].minor.yy95, NULL, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 3104 "parser.c"
        break;
      case 63:
// 1123 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_parameter(NULL, yymsp[-1].minor.yy95, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 3111 "parser.c"
        break;
      case 64:
// 1127 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_parameter(NULL, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy95, status->scanner_state);
  yy_destructor(41,&yymsp[-1].minor);
}
// 3119 "parser.c"
        break;
      case 65:
// 1131 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_parameter(yymsp[-3].minor.yy95, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy95, status->scanner_state);
  yy_destructor(41,&yymsp[-1].minor);
}
// 3127 "parser.c"
        break;
      case 66:
// 1135 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_parameter(NULL, yymsp[-3].minor.yy95, yymsp[-2].minor.yy0, yymsp[0].minor.yy95, status->scanner_state);
  yy_destructor(41,&yymsp[-1].minor);
}
// 3135 "parser.c"
        break;
      case 67:
// 1139 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(11,&yymsp[-2].minor);
  yy_destructor(12,&yymsp[0].minor);
}
// 3144 "parser.c"
        break;
      case 68:
// 1143 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_type(XX_TYPE_INTEGER);
  yy_destructor(47,&yymsp[0].minor);
}
// 3152 "parser.c"
        break;
      case 69:
// 1147 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_type(XX_TYPE_UINTEGER);
  yy_destructor(48,&yymsp[0].minor);
}
// 3160 "parser.c"
        break;
      case 70:
// 1151 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_type(XX_TYPE_LONG);
  yy_destructor(49,&yymsp[0].minor);
}
// 3168 "parser.c"
        break;
      case 71:
// 1155 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_type(XX_TYPE_ULONG);
  yy_destructor(50,&yymsp[0].minor);
}
// 3176 "parser.c"
        break;
      case 72:
// 1159 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_type(XX_TYPE_CHAR);
  yy_destructor(51,&yymsp[0].minor);
}
// 3184 "parser.c"
        break;
      case 73:
// 1163 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_type(XX_TYPE_UCHAR);
  yy_destructor(52,&yymsp[0].minor);
}
// 3192 "parser.c"
        break;
      case 74:
// 1167 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_type(XX_TYPE_DOUBLE);
  yy_destructor(53,&yymsp[0].minor);
}
// 3200 "parser.c"
        break;
      case 75:
// 1171 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_type(XX_TYPE_BOOL);
  yy_destructor(54,&yymsp[0].minor);
}
// 3208 "parser.c"
        break;
      case 76:
// 1175 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_type(XX_TYPE_STRING);
  yy_destructor(55,&yymsp[0].minor);
}
// 3216 "parser.c"
        break;
      case 77:
// 1179 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_type(XX_TYPE_ARRAY);
  yy_destructor(56,&yymsp[0].minor);
}
// 3224 "parser.c"
        break;
      case 78:
// 1183 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_type(XX_TYPE_VAR);
  yy_destructor(57,&yymsp[0].minor);
}
// 3232 "parser.c"
        break;
      case 98:
// 1263 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_break_statement(status->scanner_state);
  yy_destructor(58,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3241 "parser.c"
        break;
      case 99:
// 1267 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_continue_statement(status->scanner_state);
  yy_destructor(59,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3250 "parser.c"
        break;
      case 100:
// 1271 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_if_statement(yymsp[-2].minor.yy95, NULL, NULL, status->scanner_state);
  yy_destructor(60,&yymsp[-3].minor);
  yy_destructor(38,&yymsp[-1].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 3260 "parser.c"
        break;
      case 101:
// 1275 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_if_statement(yymsp[-5].minor.yy95, NULL, NULL, status->scanner_state);
  yy_destructor(60,&yymsp[-6].minor);
  yy_destructor(38,&yymsp[-4].minor);
  yy_destructor(39,&yymsp[-3].minor);
  yy_destructor(61,&yymsp[-2].minor);
  yy_destructor(38,&yymsp[-1].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 3273 "parser.c"
        break;
      case 102:
// 1279 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_if_statement(yymsp[-3].minor.yy95, yymsp[-1].minor.yy95, NULL, status->scanner_state);
  yy_destructor(60,&yymsp[-4].minor);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 3283 "parser.c"
        break;
      case 103:
// 1283 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_if_statement(yymsp[-7].minor.yy95, yymsp[-5].minor.yy95, yymsp[-1].minor.yy95, status->scanner_state);
  yy_destructor(60,&yymsp[-8].minor);
  yy_destructor(38,&yymsp[-6].minor);
  yy_destructor(39,&yymsp[-4].minor);
  yy_destructor(61,&yymsp[-3].minor);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 3296 "parser.c"
        break;
      case 104:
// 1287 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_if_statement(yymsp[-6].minor.yy95, yymsp[-4].minor.yy95, NULL, status->scanner_state);
  yy_destructor(60,&yymsp[-7].minor);
  yy_destructor(38,&yymsp[-5].minor);
  yy_destructor(39,&yymsp[-3].minor);
  yy_destructor(61,&yymsp[-2].minor);
  yy_destructor(38,&yymsp[-1].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 3309 "parser.c"
        break;
      case 105:
// 1291 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_if_statement(yymsp[-6].minor.yy95, NULL, yymsp[-1].minor.yy95, status->scanner_state);
  yy_destructor(60,&yymsp[-7].minor);
  yy_destructor(38,&yymsp[-5].minor);
  yy_destructor(39,&yymsp[-4].minor);
  yy_destructor(61,&yymsp[-3].minor);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 3322 "parser.c"
        break;
      case 106:
// 1295 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_switch_statement(yymsp[-2].minor.yy95, NULL, status->scanner_state);
  yy_destructor(62,&yymsp[-3].minor);
  yy_destructor(38,&yymsp[-1].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 3332 "parser.c"
        break;
      case 107:
// 1299 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_switch_statement(yymsp[-3].minor.yy95, yymsp[-1].minor.yy95, status->scanner_state);
  yy_destructor(62,&yymsp[-4].minor);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 3342 "parser.c"
        break;
      case 110:
// 1311 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_case_clause(yymsp[-2].minor.yy95, yymsp[0].minor.yy95, status->scanner_state);
  yy_destructor(63,&yymsp[-3].minor);
  yy_destructor(64,&yymsp[-1].minor);
}
// 3351 "parser.c"
        break;
      case 111:
// 1315 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_case_clause(NULL, yymsp[0].minor.yy95, status->scanner_state);
  yy_destructor(65,&yymsp[-2].minor);
  yy_destructor(64,&yymsp[-1].minor);
}
// 3360 "parser.c"
        break;
      case 112:
// 1319 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_loop_statement(NULL, status->scanner_state);
  yy_destructor(66,&yymsp[-2].minor);
  yy_destructor(38,&yymsp[-1].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 3370 "parser.c"
        break;
      case 113:
// 1323 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_loop_statement(yymsp[-1].minor.yy95, status->scanner_state);
  yy_destructor(66,&yymsp[-3].minor);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 3380 "parser.c"
        break;
      case 114:
// 1327 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_while_statement(yymsp[-2].minor.yy95, NULL, status->scanner_state);
  yy_destructor(67,&yymsp[-3].minor);
  yy_destructor(38,&yymsp[-1].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 3390 "parser.c"
        break;
      case 115:
// 1331 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_while_statement(yymsp[-3].minor.yy95, yymsp[-1].minor.yy95, status->scanner_state);
  yy_destructor(67,&yymsp[-4].minor);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 3400 "parser.c"
        break;
      case 116:
// 1335 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_do_while_statement(yymsp[-1].minor.yy95, NULL, status->scanner_state);
  yy_destructor(68,&yymsp[-5].minor);
  yy_destructor(38,&yymsp[-4].minor);
  yy_destructor(39,&yymsp[-3].minor);
  yy_destructor(67,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3412 "parser.c"
        break;
      case 117:
// 1339 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_do_while_statement(yymsp[-1].minor.yy95, yymsp[-4].minor.yy95, status->scanner_state);
  yy_destructor(68,&yymsp[-6].minor);
  yy_destructor(38,&yymsp[-5].minor);
  yy_destructor(39,&yymsp[-3].minor);
  yy_destructor(67,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3424 "parser.c"
        break;
      case 118:
// 1343 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_for_statement(yymsp[-3].minor.yy95, NULL, yymsp[-5].minor.yy0, 0, yymsp[-1].minor.yy95, status->scanner_state);
  yy_destructor(69,&yymsp[-6].minor);
  yy_destructor(70,&yymsp[-4].minor);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 3435 "parser.c"
        break;
      case 119:
// 1347 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_for_statement(yymsp[-3].minor.yy95, NULL, yymsp[-6].minor.yy0, 1, yymsp[-1].minor.yy95, status->scanner_state);
  yy_destructor(69,&yymsp[-7].minor);
  yy_destructor(70,&yymsp[-5].minor);
  yy_destructor(71,&yymsp[-4].minor);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 3447 "parser.c"
        break;
      case 120:
// 1351 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_for_statement(yymsp[-3].minor.yy95, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, 0, yymsp[-1].minor.yy95, status->scanner_state);
  yy_destructor(69,&yymsp[-8].minor);
  yy_destructor(6,&yymsp[-6].minor);
  yy_destructor(70,&yymsp[-4].minor);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 3459 "parser.c"
        break;
      case 121:
// 1355 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_for_statement(yymsp[-3].minor.yy95, yymsp[-8].minor.yy0, yymsp[-6].minor.yy0, 1, yymsp[-1].minor.yy95, status->scanner_state);
  yy_destructor(69,&yymsp[-9].minor);
  yy_destructor(6,&yymsp[-7].minor);
  yy_destructor(70,&yymsp[-5].minor);
  yy_destructor(71,&yymsp[-4].minor);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 3472 "parser.c"
        break;
      case 122:
// 1359 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_let_statement(yymsp[-1].minor.yy95, status->scanner_state);
  yy_destructor(72,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3481 "parser.c"
        break;
      case 125:
// 1372 "parser.lemon"
{
	yygotominor.yy95 = json_object_new_string("assign");
  yy_destructor(41,&yymsp[0].minor);
}
// 3489 "parser.c"
        break;
      case 126:
// 1377 "parser.lemon"
{
	yygotominor.yy95 = json_object_new_string("add-assign");
  yy_destructor(73,&yymsp[0].minor);
}
// 3497 "parser.c"
        break;
      case 127:
// 1382 "parser.lemon"
{
	yygotominor.yy95 = json_object_new_string("sub-assign");
  yy_destructor(74,&yymsp[0].minor);
}
// 3505 "parser.c"
        break;
      case 128:
// 1386 "parser.lemon"
{
	yygotominor.yy95 = json_object_new_string("mult-assign");
  yy_destructor(75,&yymsp[0].minor);
}
// 3513 "parser.c"
        break;
      case 129:
// 1390 "parser.lemon"
{
	yygotominor.yy95 = json_object_new_string("div-assign");
  yy_destructor(76,&yymsp[0].minor);
}
// 3521 "parser.c"
        break;
      case 130:
// 1394 "parser.lemon"
{
	yygotominor.yy95 = json_object_new_string("concat-assign");
  yy_destructor(77,&yymsp[0].minor);
}
// 3529 "parser.c"
        break;
      case 131:
// 1399 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_let_assignment("variable", yymsp[-1].minor.yy95, yymsp[-2].minor.yy0, NULL, NULL, yymsp[0].minor.yy95, status->scanner_state);
}
// 3536 "parser.c"
        break;
      case 132:
// 1404 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_let_assignment("object-property", yymsp[-1].minor.yy95, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, yymsp[0].minor.yy95, status->scanner_state);
  yy_destructor(78,&yymsp[-3].minor);
}
// 3544 "parser.c"
        break;
      case 133:
// 1409 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_let_assignment("object-property-append", yymsp[-1].minor.yy95, yymsp[-6].minor.yy0, yymsp[-4].minor.yy0, NULL, yymsp[0].minor.yy95, status->scanner_state);
  yy_destructor(78,&yymsp[-5].minor);
  yy_destructor(79,&yymsp[-3].minor);
  yy_destructor(80,&yymsp[-2].minor);
}
// 3554 "parser.c"
        break;
      case 134:
// 1414 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_let_assignment("object-property-array-index", yymsp[-1].minor.yy95, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, yymsp[-3].minor.yy95, yymsp[0].minor.yy95, status->scanner_state);
  yy_destructor(78,&yymsp[-6].minor);
  yy_destructor(79,&yymsp[-4].minor);
  yy_destructor(80,&yymsp[-2].minor);
}
// 3564 "parser.c"
        break;
      case 135:
// 1419 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_let_assignment("static-property", yymsp[-1].minor.yy95, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, yymsp[0].minor.yy95, status->scanner_state);
  yy_destructor(81,&yymsp[-3].minor);
}
// 3572 "parser.c"
        break;
      case 136:
// 1424 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_let_assignment("static-property-append", yymsp[-1].minor.yy95, yymsp[-6].minor.yy0, yymsp[-4].minor.yy0, NULL, yymsp[0].minor.yy95, status->scanner_state);
  yy_destructor(81,&yymsp[-5].minor);
  yy_destructor(79,&yymsp[-3].minor);
  yy_destructor(80,&yymsp[-2].minor);
}
// 3582 "parser.c"
        break;
      case 137:
// 1429 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_let_assignment("static-property-array-index", yymsp[-1].minor.yy95, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, yymsp[-3].minor.yy95, yymsp[0].minor.yy95, status->scanner_state);
  yy_destructor(81,&yymsp[-6].minor);
  yy_destructor(79,&yymsp[-4].minor);
  yy_destructor(80,&yymsp[-2].minor);
}
// 3592 "parser.c"
        break;
      case 138:
// 1434 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_let_assignment("variable-append", yymsp[-1].minor.yy95, yymsp[-4].minor.yy0, NULL, NULL, yymsp[0].minor.yy95, status->scanner_state);
  yy_destructor(79,&yymsp[-3].minor);
  yy_destructor(80,&yymsp[-2].minor);
}
// 3601 "parser.c"
        break;
      case 139:
// 1439 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_let_assignment("array-index", yymsp[-1].minor.yy95, yymsp[-5].minor.yy0, NULL, yymsp[-3].minor.yy95, yymsp[0].minor.yy95, status->scanner_state);
  yy_destructor(79,&yymsp[-4].minor);
  yy_destructor(80,&yymsp[-2].minor);
}
// 3610 "parser.c"
        break;
      case 140:
// 1444 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_let_assignment("incr", NULL, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(82,&yymsp[0].minor);
}
// 3618 "parser.c"
        break;
      case 141:
// 1449 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_let_assignment("decr", NULL, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(83,&yymsp[0].minor);
}
// 3626 "parser.c"
        break;
      case 143:
// 1457 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_echo_statement(yymsp[-1].minor.yy95, status->scanner_state);
  yy_destructor(84,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3635 "parser.c"
        break;
      case 146:
// 1469 "parser.lemon"
{
	yygotominor.yy95 = yymsp[0].minor.yy95;;
}
// 3642 "parser.c"
        break;
      case 147:
// 1474 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_mcall_statement(yymsp[-1].minor.yy95, status->scanner_state);
  yy_destructor(32,&yymsp[0].minor);
}
// 3650 "parser.c"
        break;
      case 148:
// 1479 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_scall_statement(yymsp[-1].minor.yy95, status->scanner_state);
  yy_destructor(32,&yymsp[0].minor);
}
// 3658 "parser.c"
        break;
      case 149:
// 1484 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_return_statement(yymsp[-1].minor.yy95, status->scanner_state);
  yy_destructor(85,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3667 "parser.c"
        break;
      case 150:
// 1489 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_return_statement(NULL, status->scanner_state);
  yy_destructor(85,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3676 "parser.c"
        break;
      case 151:
// 1494 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(86,&yymsp[-4].minor);
  yy_destructor(78,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3690 "parser.c"
        break;
      case 152:
// 1503 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(86,&yymsp[-5].minor);
  yy_destructor(79,&yymsp[-3].minor);
  yy_destructor(80,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3705 "parser.c"
        break;
      case 153:
// 1512 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_INTEGER, yymsp[-2].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(86,&yymsp[-5].minor);
  yy_destructor(79,&yymsp[-3].minor);
  yy_destructor(80,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3720 "parser.c"
        break;
      case 154:
// 1521 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_STRING, yymsp[-2].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(86,&yymsp[-5].minor);
  yy_destructor(79,&yymsp[-3].minor);
  yy_destructor(80,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3735 "parser.c"
        break;
      case 155:
// 1530 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_throw_exception(yymsp[-1].minor.yy95, status->scanner_state);
  yy_destructor(89,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3744 "parser.c"
        break;
      case 156:
// 1534 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_declare_statement(XX_T_TYPE_INTEGER, yymsp[-1].minor.yy95, status->scanner_state);
  yy_destructor(47,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3753 "parser.c"
        break;
      case 157:
// 1538 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_declare_statement(XX_T_TYPE_UINTEGER, yymsp[-1].minor.yy95, status->scanner_state);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3762 "parser.c"
        break;
      case 158:
// 1542 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_declare_statement(XX_T_TYPE_CHAR, yymsp[-1].minor.yy95, status->scanner_state);
  yy_destructor(51,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3771 "parser.c"
        break;
      case 159:
// 1546 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_declare_statement(XX_T_TYPE_UCHAR, yymsp[-1].minor.yy95, status->scanner_state);
  yy_destructor(52,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3780 "parser.c"
        break;
      case 160:
// 1550 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_declare_statement(XX_T_TYPE_LONG, yymsp[-1].minor.yy95, status->scanner_state);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3789 "parser.c"
        break;
      case 161:
// 1554 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_declare_statement(XX_T_TYPE_ULONG, yymsp[-1].minor.yy95, status->scanner_state);
  yy_destructor(50,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3798 "parser.c"
        break;
      case 162:
// 1558 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_declare_statement(XX_T_TYPE_DOUBLE, yymsp[-1].minor.yy95, status->scanner_state);
  yy_destructor(53,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3807 "parser.c"
        break;
      case 163:
// 1562 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_declare_statement(XX_T_TYPE_STRING, yymsp[-1].minor.yy95, status->scanner_state);
  yy_destructor(55,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3816 "parser.c"
        break;
      case 164:
// 1566 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_declare_statement(XX_T_TYPE_BOOL, yymsp[-1].minor.yy95, status->scanner_state);
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3825 "parser.c"
        break;
      case 165:
// 1570 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_declare_statement(XX_T_TYPE_VAR, yymsp[-1].minor.yy95, status->scanner_state);
  yy_destructor(57,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3834 "parser.c"
        break;
      case 168:
// 1582 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_declare_variable(yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 3841 "parser.c"
        break;
      case 169:
// 1586 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_declare_variable(yymsp[-2].minor.yy0, yymsp[0].minor.yy95, status->scanner_state);
  yy_destructor(41,&yymsp[-1].minor);
}
// 3849 "parser.c"
        break;
      case 171:
// 1594 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_expr("not", yymsp[0].minor.yy95, NULL, NULL, status->scanner_state);
  yy_destructor(28,&yymsp[-1].minor);
}
// 3857 "parser.c"
        break;
      case 172:
// 1598 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_expr("isset", yymsp[0].minor.yy95, NULL, NULL, status->scanner_state);
  yy_destructor(23,&yymsp[-1].minor);
}
// 3865 "parser.c"
        break;
      case 173:
// 1602 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_expr("equals", yymsp[-2].minor.yy95, yymsp[0].minor.yy95, NULL, status->scanner_state);
  yy_destructor(9,&yymsp[-1].minor);
}
// 3873 "parser.c"
        break;
      case 174:
// 1606 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_expr("not-equals", yymsp[-2].minor.yy95, yymsp[0].minor.yy95, NULL, status->scanner_state);
  yy_destructor(16,&yymsp[-1].minor);
}
// 3881 "parser.c"
        break;
      case 175:
// 1610 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_expr("identical", yymsp[-2].minor.yy95, yymsp[0].minor.yy95, NULL, status->scanner_state);
  yy_destructor(10,&yymsp[-1].minor);
}
// 3889 "parser.c"
        break;
      case 176:
// 1614 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_expr("not-identical", yymsp[-2].minor.yy95, yymsp[0].minor.yy95, NULL, status->scanner_state);
  yy_destructor(15,&yymsp[-1].minor);
}
// 3897 "parser.c"
        break;
      case 177:
// 1618 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_expr("less", yymsp[-2].minor.yy95, yymsp[0].minor.yy95, NULL, status->scanner_state);
  yy_destructor(11,&yymsp[-1].minor);
}
// 3905 "parser.c"
        break;
      case 178:
// 1622 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_expr("greater", yymsp[-2].minor.yy95, yymsp[0].minor.yy95, NULL, status->scanner_state);
  yy_destructor(12,&yymsp[-1].minor);
}
// 3913 "parser.c"
        break;
      case 179:
// 1626 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_expr("less-equal", yymsp[-2].minor.yy95, yymsp[0].minor.yy95, NULL, status->scanner_state);
  yy_destructor(13,&yymsp[-1].minor);
}
// 3921 "parser.c"
        break;
      case 180:
// 1630 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_expr("greater-equal", yymsp[-2].minor.yy95, yymsp[0].minor.yy95, NULL, status->scanner_state);
  yy_destructor(14,&yymsp[-1].minor);
}
// 3929 "parser.c"
        break;
      case 181:
// 1634 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_expr("list", yymsp[-1].minor.yy95, NULL, NULL, status->scanner_state);
  yy_destructor(45,&yymsp[-2].minor);
  yy_destructor(29,&yymsp[0].minor);
}
// 3938 "parser.c"
        break;
      case 182:
// 1638 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_expr("cast", yymsp[-2].minor.yy95, yymsp[0].minor.yy95, NULL, status->scanner_state);
  yy_destructor(45,&yymsp[-3].minor);
  yy_destructor(29,&yymsp[-1].minor);
}
// 3947 "parser.c"
        break;
      case 183:
// 1642 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_expr("property-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(78,&yymsp[-1].minor);
}
// 3955 "parser.c"
        break;
      case 184:
// 1646 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_expr("static-property-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(81,&yymsp[-1].minor);
}
// 3963 "parser.c"
        break;
      case 185:
// 1650 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_expr("static-constant-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(81,&yymsp[-1].minor);
}
// 3971 "parser.c"
        break;
      case 186:
// 1655 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), yymsp[-1].minor.yy95, NULL, status->scanner_state);
  yy_destructor(79,&yymsp[-2].minor);
  yy_destructor(80,&yymsp[0].minor);
}
// 3980 "parser.c"
        break;
      case 187:
// 1660 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_expr("add", yymsp[-2].minor.yy95, yymsp[0].minor.yy95, NULL, status->scanner_state);
  yy_destructor(17,&yymsp[-1].minor);
}
// 3988 "parser.c"
        break;
      case 188:
// 1665 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_expr("sub", yymsp[-2].minor.yy95, yymsp[0].minor.yy95, NULL, status->scanner_state);
  yy_destructor(18,&yymsp[-1].minor);
}
// 3996 "parser.c"
        break;
      case 189:
// 1670 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_expr("mul", yymsp[-2].minor.yy95, yymsp[0].minor.yy95, NULL, status->scanner_state);
  yy_destructor(20,&yymsp[-1].minor);
}
// 4004 "parser.c"
        break;
      case 190:
// 1675 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_expr("div", yymsp[-2].minor.yy95, yymsp[0].minor.yy95, NULL, status->scanner_state);
  yy_destructor(21,&yymsp[-1].minor);
}
// 4012 "parser.c"
        break;
      case 191:
// 1680 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_expr("mod", yymsp[-2].minor.yy95, yymsp[0].minor.yy95, NULL, status->scanner_state);
  yy_destructor(22,&yymsp[-1].minor);
}
// 4020 "parser.c"
        break;
      case 192:
// 1685 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_expr("concat", yymsp[-2].minor.yy95, yymsp[0].minor.yy95, NULL, status->scanner_state);
  yy_destructor(19,&yymsp[-1].minor);
}
// 4028 "parser.c"
        break;
      case 193:
// 1690 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_expr("and", yymsp[-2].minor.yy95, yymsp[0].minor.yy95, NULL, status->scanner_state);
  yy_destructor(7,&yymsp[-1].minor);
}
// 4036 "parser.c"
        break;
      case 194:
// 1695 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_expr("or", yymsp[-2].minor.yy95, yymsp[0].minor.yy95, NULL, status->scanner_state);
  yy_destructor(8,&yymsp[-1].minor);
}
// 4044 "parser.c"
        break;
      case 195:
// 1700 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_expr("instanceof", yymsp[0].minor.yy95, NULL, NULL, status->scanner_state);
  yy_destructor(26,&yymsp[-1].minor);
}
// 4052 "parser.c"
        break;
      case 196:
// 1705 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_expr("fetch", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), yymsp[0].minor.yy95, NULL, status->scanner_state);
  yy_destructor(24,&yymsp[-3].minor);
  yy_destructor(6,&yymsp[-1].minor);
}
// 4061 "parser.c"
        break;
      case 197:
// 1710 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_expr("typeof", yymsp[0].minor.yy95, NULL, NULL, status->scanner_state);
  yy_destructor(25,&yymsp[-1].minor);
}
// 4069 "parser.c"
        break;
      case 198:
// 1715 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_literal(XX_T_CONSTANT, yymsp[0].minor.yy0, status->scanner_state);
}
// 4076 "parser.c"
        break;
      case 199:
      case 227:
      case 231:
// 1720 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state);
}
// 4085 "parser.c"
        break;
      case 200:
      case 229:
      case 232:
// 1725 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_literal(XX_T_INTEGER, yymsp[0].minor.yy0, status->scanner_state);
}
// 4094 "parser.c"
        break;
      case 201:
      case 228:
      case 233:
// 1730 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_literal(XX_T_STRING, yymsp[0].minor.yy0, status->scanner_state);
}
// 4103 "parser.c"
        break;
      case 202:
// 1735 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_literal(XX_T_CHAR, yymsp[0].minor.yy0, status->scanner_state);
}
// 4110 "parser.c"
        break;
      case 203:
      case 234:
// 1740 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_literal(XX_T_DOUBLE, yymsp[0].minor.yy0, status->scanner_state);
}
// 4118 "parser.c"
        break;
      case 204:
      case 235:
// 1745 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_literal(XX_T_NULL, NULL, status->scanner_state);
  yy_destructor(92,&yymsp[0].minor);
}
// 4127 "parser.c"
        break;
      case 205:
      case 237:
// 1750 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_literal(XX_T_TRUE, NULL, status->scanner_state);
  yy_destructor(93,&yymsp[0].minor);
}
// 4136 "parser.c"
        break;
      case 206:
      case 236:
// 1755 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_literal(XX_T_FALSE, NULL, status->scanner_state);
  yy_destructor(94,&yymsp[0].minor);
}
// 4145 "parser.c"
        break;
      case 207:
// 1760 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_expr("empty-array", NULL, NULL, NULL, status->scanner_state);
  yy_destructor(79,&yymsp[-1].minor);
  yy_destructor(80,&yymsp[0].minor);
}
// 4154 "parser.c"
        break;
      case 208:
// 1765 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_expr("array", yymsp[-1].minor.yy95, NULL, NULL, status->scanner_state);
  yy_destructor(79,&yymsp[-2].minor);
  yy_destructor(80,&yymsp[0].minor);
}
// 4163 "parser.c"
        break;
      case 209:
// 1770 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_new_instance(yymsp[0].minor.yy0, NULL, status->scanner_state);
  yy_destructor(27,&yymsp[-1].minor);
}
// 4171 "parser.c"
        break;
      case 210:
// 1775 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_new_instance(yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(27,&yymsp[-3].minor);
  yy_destructor(45,&yymsp[-1].minor);
  yy_destructor(29,&yymsp[0].minor);
}
// 4181 "parser.c"
        break;
      case 211:
// 1780 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_new_instance(yymsp[-3].minor.yy0, yymsp[-1].minor.yy95, status->scanner_state);
  yy_destructor(27,&yymsp[-4].minor);
  yy_destructor(45,&yymsp[-2].minor);
  yy_destructor(29,&yymsp[0].minor);
}
// 4191 "parser.c"
        break;
      case 212:
// 1785 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_fcall(yymsp[-3].minor.yy0, yymsp[-1].minor.yy95, status->scanner_state);
  yy_destructor(45,&yymsp[-2].minor);
  yy_destructor(29,&yymsp[0].minor);
}
// 4200 "parser.c"
        break;
      case 213:
// 1790 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_fcall(yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(45,&yymsp[-1].minor);
  yy_destructor(29,&yymsp[0].minor);
}
// 4209 "parser.c"
        break;
      case 216:
// 1805 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_scall(yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy95, status->scanner_state);
  yy_destructor(81,&yymsp[-4].minor);
  yy_destructor(45,&yymsp[-2].minor);
  yy_destructor(29,&yymsp[0].minor);
}
// 4219 "parser.c"
        break;
      case 217:
// 1810 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_scall(yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(81,&yymsp[-3].minor);
  yy_destructor(45,&yymsp[-1].minor);
  yy_destructor(29,&yymsp[0].minor);
}
// 4229 "parser.c"
        break;
      case 218:
// 1815 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_mcall(yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy95, status->scanner_state);
  yy_destructor(78,&yymsp[-4].minor);
  yy_destructor(45,&yymsp[-2].minor);
  yy_destructor(29,&yymsp[0].minor);
}
// 4239 "parser.c"
        break;
      case 219:
// 1820 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_mcall(yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(78,&yymsp[-3].minor);
  yy_destructor(45,&yymsp[-1].minor);
  yy_destructor(29,&yymsp[0].minor);
}
// 4249 "parser.c"
        break;
      case 225:
// 1844 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_array_item(yymsp[-2].minor.yy95, yymsp[0].minor.yy95, status->scanner_state);
  yy_destructor(64,&yymsp[-1].minor);
}
// 4257 "parser.c"
        break;
      case 226:
// 1848 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_array_item(NULL, yymsp[0].minor.yy95, status->scanner_state);
}
// 4264 "parser.c"
        break;
      case 238:
// 1897 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(79,&yymsp[-2].minor);
  yy_destructor(80,&yymsp[0].minor);
}
// 4273 "parser.c"
        break;
      case 239:
// 1902 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_INTEGER, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(79,&yymsp[-2].minor);
  yy_destructor(80,&yymsp[0].minor);
}
// 4282 "parser.c"
        break;
      case 240:
// 1907 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_STRING, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(79,&yymsp[-2].minor);
  yy_destructor(80,&yymsp[0].minor);
}
// 4291 "parser.c"
        break;
      case 242:
// 1915 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_comment(yymsp[0].minor.yy0, status->scanner_state);
}
// 4298 "parser.c"
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
// 826 "parser.lemon"


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

// 4374 "parser.c"
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
			/*case XX_T_INTERFACE:
				xx_(xx_parser, XX_INTERFACE, NULL, parser_status);
				break;*/
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
